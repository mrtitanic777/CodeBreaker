; AUDDRV -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 5113 instructions recovered by recursive descent

Declare_Virtual_Device AUDDRV, 4, 0, AUDDRV_Control, 033F6h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x65c  R-X
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
        dw  033F6h                      ; DDB_Req_Device_Number
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
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], ch
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
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + ebx], dl
        xor    dh, byte ptr [esi]
        cmp    byte ptr [esi], bh
        cmp    bh, byte ptr [eax + eax]
loc_1_0115:
        and    byte ptr [eax - 0x339f1f40], 0xcc
AUDDRV_Control:
        cmp    eax, 8
        je     loc_5_111E
loc_1_0125:
        cmp    eax, 0xb
        je     loc_5_10AB
loc_1_012E:
        cmp    eax, 3
        je     loc_5_111E
loc_1_0137:
        cmp    eax, 0x1b
        je     loc_7_0000
loc_1_0140:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_0149:
        cmp    eax, 0x22
        je     loc_4_07D3
loc_1_0152:
        clc    
        ret    
sub_1_0154:
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
        je     loc_1_01AB
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_01AB
loc_1_0187:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0xc], 1
        jne    loc_1_019B
        cmp    dword ptr [edx + edi], ebx
        jne    loc_1_01A1
loc_1_0197:
        popfd  
        clc    
        jmp    loc_1_01AF
loc_1_019B:
        cmp    word ptr [edx + edi], bx
        je     loc_1_0197
loc_1_01A1:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_01AB
        jmp    loc_1_0187
loc_1_01AB:
        popfd  
        xor    edi, edi
        stc    
loc_1_01AF:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_1_01B5:
        push   eax
        push   ebx
        pushfd 
        add    ebx, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_01E2
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_01E2
loc_1_01CD:
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_01D8
        mov    esi, eax
        popfd  
        clc    
        jmp    loc_1_01E6
loc_1_01D8:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_01E2
        jmp    loc_1_01CD
loc_1_01E2:
        popfd  
        xor    esi, esi
        stc    
loc_1_01E6:
        pop    ebx
        pop    eax
        ret    
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h
        dd  loc_1_00D8               ; fixup
loc_1_01F4:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_024B
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_1_024B
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_024B
loc_1_0215:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x33], ebx
        jne    loc_1_0221
        or     ecx, 1
loc_1_0221:
        cmp    dword ptr [edi + 0x3b], ebx
        jne    loc_1_0229
        or     ecx, 2
loc_1_0229:
        cmp    dword ptr [edi + 0x43], ebx
        jne    loc_1_0231
        or     ecx, 4
loc_1_0231:
        jecxz  loc_1_023C
        push   eax
        mov    eax, ecx
        call   sub_5_117F
loc_1_023B:
        pop    eax
loc_1_023C:
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_024B
        jmp    loc_1_0215
loc_1_024B:
        popfd  
        popal  
        jmp    loc_1_00D8
        db  090h
loc_1_0254:
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012800400
        mov    eax, eax
loc_1_025C:
        cld    
        push   edi
        push   edx
        push   ebx
        mov    edi, edx
        mov    ebx, dword ptr [edi + 0x33]
        or     ebx, ebx
        je     loc_1_0273
        cmp    word ptr [edi + 0x84], 0
        je     loc_1_0285
loc_1_0273:
        push   eax
        movzx  edx, word ptr [edi + 6]
        add    edx, 0xe
        in     al, dx
        pop    eax
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        jmp    loc_1_028B
loc_1_0285:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_028B:
        pop    ebx
        pop    edx
        pop    edi
        clc    
        ret    
loc_1_0290:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_02A0:
        push   edi
        push   1
        push   eax
        call   sub_1_0154
        add    esp, 8
        or     edi, edi
        je     loc_1_02D1
        cmp    dword ptr [edi + 0x33], ebx
        jne    loc_1_02C7
        jecxz  loc_1_02BF
loc_1_02B7:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_02D1
loc_1_02BF:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_02D1
loc_1_02C7:
        test   word ptr [edi + 4], 0x8000
        jne    loc_1_02BF
        je     loc_1_02B7
loc_1_02D1:
        pop    edi
        clc    
        ret    
loc_1_02D4:
        cld    
        push   edi
        push   edx
        push   ebx
        mov    edi, edx
        movsx  edx, word ptr [edi + 0xa]
        or     edx, edx
        js     loc_1_02FC
        mov    ebx, dword ptr [edi + 0x43]
        or     ebx, ebx
        jne    loc_1_0304
        push   eax
        push   ecx
        mov    ecx, 0x32
loc_1_02F0:
        inc    edx
        in     al, dx
        or     al, al
        js     loc_1_02FA
        dec    edx
        in     al, dx
        loop   loc_1_02F0
loc_1_02FA:
        pop    ecx
        pop    eax
loc_1_02FC:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        jmp    loc_1_030A
loc_1_0304:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_030A:
        pop    ebx
        pop    edx
        pop    edi
        clc    
        ret    
        db  090h
loc_1_0310:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_0320:
        push   edi
        push   5
        push   eax
        call   sub_1_0154
        add    esp, 8
        or     edi, edi
        je     loc_1_0351
        cmp    dword ptr [edi + 0x43], ebx
        jne    loc_1_0347
        jecxz  loc_1_033F
loc_1_0337:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_0351
loc_1_033F:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_0351
loc_1_0347:
        test   word ptr [edi + 4], 0x4000
        jne    loc_1_033F
        je     loc_1_0337
loc_1_0351:
        pop    edi
        clc    
        ret    
sub_1_0354:
        push   ecx
        push   edx
        add    edx, 0xe
        mov    ecx, 0x5dc
loc_1_035E:
        in     al, dx
        or     al, al
        js     loc_1_0368
        loop   loc_1_035E
        stc    
        jmp    loc_1_036D
loc_1_0368:
        sub    edx, 4
        in     al, dx
        clc    
loc_1_036D:
        pop    edx
        pop    ecx
        ret    
sub_1_0370:
        push   eax
        push   ecx
        push   edx
        add    edx, 0xc
        push   eax
        mov    ecx, 0xa0000
loc_1_037C:
        in     al, dx
        or     al, al
        jns    loc_1_0387
        loop   loc_1_037C
        pop    eax
        stc    
        jmp    loc_1_038A
loc_1_0387:
        pop    eax
        out    dx, al
        clc    
loc_1_038A:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_038E:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    loc_1_039E
loc_1_039E:
        jmp    loc_1_03A0
loc_1_03A0:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_0354
        pop    edx
        pop    eax
        ret    
sub_1_03B1:
        push   eax
        call   sub_1_038E
        jb     loc_1_03C9
        mov    al, 0x10
        call   sub_1_0370
        jb     loc_1_03C9
        mov    al, 0x80
        call   sub_1_0370
loc_1_03C9:
        pop    eax
        ret    
sub_1_03CB:
        push   eax
        mov    al, 0xc6
        call   sub_1_0370
        jb     loc_1_03F9
        mov    al, 0xc3
        call   sub_1_0370
        jb     loc_1_03F9
        call   sub_1_0354
        jb     loc_1_03F9
        test   al, 1
        jne    loc_1_03F9
        mov    al, 0xc2
        call   sub_1_0370
        jb     loc_1_03F9
        mov    al, 1
        call   sub_1_0370
loc_1_03F9:
        pop    eax
        ret    
sub_1_03FB:
        push   eax
        push   edx
        push   edi
        push   esi
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0434
        cld    
        movzx  edx, word ptr [edi + 6]
        call   sub_1_03CB
        jb     loc_1_0418
        call   sub_1_04F1
loc_1_0418:
        add    dx, 4
        add    edi, 0x63
        mov    esi, 0                        ; -> obj1:0x010C
loc_1_0424:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_1_0434
        out    dx, al
        inc    dx
        mov    al, byte ptr [edi]
        inc    edi
        out    dx, al
        dec    dx
        jmp    loc_1_0424
loc_1_0434:
        pop    esi
        pop    edi
        pop    edx
        pop    eax
        clc    
        ret    
sub_1_043A:
        push   eax
        push   edx
        push   edi
        push   esi
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0465
        cld    
        movzx  edx, word ptr [edi + 6]
        add    dx, 4
        add    edi, 0x63
        mov    esi, 0                        ; -> obj1:0x010C
loc_1_0457:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_1_0465
        out    dx, al
        inc    dx
        in     al, dx
        stosb  byte ptr es:[edi], al
        dec    dx
        jmp    loc_1_0457
loc_1_0465:
        pop    esi
        pop    edi
        pop    edx
        pop    eax
        clc    
        ret    
sub_1_046B:
        push   eax
        push   ecx
        push   esi
        cld    
        xor    cl, cl
loc_1_0471:
        lodsw  ax, word ptr [esi]
        cmp    ax, bx
        je     loc_1_0481
        inc    cl
        cmp    ax, -1
        jne    loc_1_0471
        stc    
loc_1_0481:
        mov    bl, cl
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_1_0487:
        push   eax
        push   ebx
        call   sub_1_046B
        jb     loc_1_04B4
        mov    al, bl
        shl    al, 2
        or     al, 0x10
        push   eax
        mov    al, 0xc6
        call   sub_1_0370
        pop    eax
        jb     loc_1_04B4
        push   eax
        mov    al, 0xb1
        call   sub_1_0370
        pop    eax
        jb     loc_1_04B4
        call   sub_1_0370
        jae    loc_1_04B7
loc_1_04B4:
        stc    
        jmp    loc_1_04B8
loc_1_04B7:
        clc    
loc_1_04B8:
        pop    ebx
        pop    eax
        ret    
sub_1_04BB:
        push   eax
        push   ebx
        call   sub_1_046B
        jb     loc_1_04EA
        mov    al, bl
        inc    al
        shl    al, 2
        or     al, 0x10
        push   eax
        mov    al, 0xc6
        call   sub_1_0370
        pop    eax
        jb     loc_1_04EA
        push   eax
        mov    al, 0xb2
        call   sub_1_0370
        pop    eax
        jb     loc_1_04EA
        call   sub_1_0370
        jae    loc_1_04ED
loc_1_04EA:
        stc    
        jmp    loc_1_04EE
loc_1_04ED:
        clc    
loc_1_04EE:
        pop    ebx
        pop    eax
        ret    
sub_1_04F1:
        push   eax
        push   edx
        movzx  edx, word ptr [edi + 6]
        mov    al, 0xc6
        call   sub_1_0370
        jb     loc_1_0545
        mov    al, 0xb1
        call   sub_1_0370
        jb     loc_1_0545
        mov    al, byte ptr [edi + 0x6d]
        call   sub_1_0370
        jb     loc_1_0545
        mov    al, 0xb2
        call   sub_1_0370
        jb     loc_1_0545
        mov    al, byte ptr [edi + 0x6e]
        call   sub_1_0370
        jb     loc_1_0545
        mov    al, 0xe1
        call   sub_1_0370
        call   sub_1_0354
        call   sub_1_0354
        add    dx, 4
        mov    al, 0x40
        out    dx, al
        inc    dx
        mov    al, byte ptr [edi + 0x6f]
        out    dx, al
        clc    
loc_1_0545:
        pop    edx
        pop    eax
        ret    
sub_1_0548:
        push   eax
        push   edx
        movzx  edx, word ptr [edi + 6]
        call   sub_1_038E
        jb     loc_1_05A4
        mov    al, 0xc6
        call   sub_1_0370
        jb     loc_1_05A4
        mov    al, 0xc0
        call   sub_1_0370
        jb     loc_1_05A4
        mov    al, 0xb1
        call   sub_1_0370
        jb     loc_1_05A4
        call   sub_1_0354
        jb     loc_1_05A4
        mov    byte ptr [edi + 0x6d], al
        mov    al, 0xc0
        call   sub_1_0370
        jb     loc_1_05A4
        mov    al, 0xb2
        call   sub_1_0370
        jb     loc_1_05A4
        call   sub_1_0354
        jb     loc_1_05A4
        mov    byte ptr [edi + 0x6e], al
        add    dx, 4
        mov    al, 0x40
        out    dx, al
        inc    dx
        in     al, dx
        mov    byte ptr [edi + 0x6f], al
        clc    
loc_1_05A4:
        pop    edx
        pop    eax
        ret    
sub_1_05A7:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        cmp    ecx, -1
        mov    ecx, eax
        mov    ah, 2
        je     loc_1_05B8
        or     ah, 1
loc_1_05B8:
        cmp    ecx, -1
        je     loc_1_05DE
        call   sub_1_046B
        jb     loc_1_05F0
        movzx  ebx, bl
        or     ah, byte ptr [ebx]            ; -> loc_1_0115
        mov    bx, cx
        mov    esi, edi
        call   sub_1_046B
        jb     loc_1_05F0
        shl    bl, 3
        or     ah, bl
loc_1_05DE:
        add    dx, 4
        mov    al, 0x40
        out    dx, al
        inc    dx
        mov    al, ah
        and    al, 0x1f
        out    dx, al
        mov    al, ah
        out    dx, al
        clc    
loc_1_05F0:
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_1_05F6:
        push   eax
        push   edx
        add    edx, 4
        in     al, dx
        mov    ah, al
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        push   eax
        and    al, 0x1f
        out    dx, al
        pop    eax
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        clc    
        pop    edx
        pop    eax
        ret    
sub_1_0611:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd 
        cli    
        popfd  
        clc    
        popal  
        ret    
sub_1_061F:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd 
        cli    
        popfd  
        clc    
        popal  
        ret    
sub_1_062D:
        push   eax
        push   ecx
        mov    ecx, 6
loc_1_0634:
        in     al, dx
        jmp    loc_1_0637
loc_1_0637:
        loop   loc_1_0634
        pop    ecx
        pop    eax
        ret    
sub_1_063C:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_062D
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_062D
        pop    edx
        pop    ebx
        ret    
        db  0CCh, 0CCh, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x11d  R-X
        db  004h, 000h, 000h, 000h
        dd  obj2:0x0018              ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        dec    esp
        db  000h
loc_2_0010:
        wait   
        db  000h
loc_2_0012:
        ljmp   0x55cc:0xcc010500
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x90
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    loc_2_0047
        jae    loc_2_0098
        jne    0x99
        and    byte ptr fs:[0x65687420], ch
        and    byte ptr [ebp + 0x53], al
        push   ebx
        and    byte ptr [ecx + 0x75], al
        imul   ebp, dword ptr fs:[edi + 0x44], 0x65766972
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x6e], ch
loc_2_0047:
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     loc_2_00BC
        jb     loc_2_0077
        popal  
        jo     0xca
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[ebp + 0x6e], dl
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xdc
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    0x93
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
loc_2_0077:
        and    byte ptr [0x65687420], ch
        and    byte ptr [ebp + 0x53], al
        push   ebx
        and    byte ptr [esi + 0x4d], al
        and    byte ptr [ebx + 0x79], dl
        outsb  dx, byte ptr [esi]
        je     loc_2_00F2
        jae    loc_2_00F6
        jp     0xf4
        jb     loc_2_00B1
        imul   esi, dword ptr [ebx + 0x20], 0x75206e69
loc_2_0098:
        jae    0xff
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0x10b
        jb     loc_2_00C6
        popal  
        jo     loc_2_0119
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
loc_2_00B1:
        add    byte ptr cs:[ebp + 0x6e], dl
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x12b
loc_2_00BC:
        and    byte ptr [ecx + 0x63], ah
        arpl   word ptr [ebp + 0x73], sp
        jae    0xe4
        dec    ebp
        dec    ecx
loc_2_00C6:
        inc    esp
        dec    ecx
        and    byte ptr [eax + 0x6f], dh
        jb     0x141
        and    byte ptr [0x65687420], ch
        and    byte ptr [ebp + 0x53], al
        push   ebx
        and    byte ptr [ebp + 0x50], cl
        push   ebp
        sub    eax, 0x20313034
        imul   esi, dword ptr [ebx + 0x20], 0x75206e69
        jae    0x14e
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0x15a
loc_2_00F2:
        jb     0x115
        popal  
loc_2_00F6:
        jo     0x168
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[ebp + 0x53], al
        xor    dword ptr [esi], esi
        cmp    byte ptr [eax], bh
        and    byte ptr [ecx + ebp*2 + 0x72], al
        arpl   word ptr gs:[eax + 0x53], si
        outsd  dx, dword ptr [esi]
        jne    0x182
        and    byte ptr fs:[edx + esi*2 + 0x69], al
loc_2_0119:
        jbe    0x180
        jb     0x11d
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x151  R-X
loc_3_0000:
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_01F4
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
        call   sub_1_0154
loc_3_005D:
        add    esp, 8
        or     edi, edi
        je     loc_3_00BD
        mov    ebx, dword ptr [edi + 0x33]
        or     ebx, ebx
        je     loc_3_00BD
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_00BD
        call   sub_1_01B5
loc_3_0078:
        jb     loc_3_00BD
        call   sub_1_061F
loc_3_007F:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 0x4b]
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
        call   sub_1_038E
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
        call   sub_1_0154
loc_3_00D4:
        add    esp, 8
        or     edi, edi
        je     loc_3_014B
        push   edx
        movzx  edx, word ptr [edi + 6]
        call   sub_1_038E
loc_3_00E5:
        pop    edx
        jb     loc_3_00ED
        call   sub_1_04F1
loc_3_00ED:
        mov    ebx, dword ptr [edi + 0x33]
        or     ebx, ebx
        je     loc_3_014B
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_014B
        test   word ptr [edi + 4], 0x100
        je     loc_3_014B
        call   sub_1_01B5
loc_3_0109:
        jb     loc_3_014B
        call   sub_1_0611
loc_3_0110:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 0x4b]
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
        call   sub_1_038E
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

VxD__LTEXT SEGMENT   ; object 4  vsize=0x946  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0154
loc_4_000E:
        add    esp, 8
        or     edi, edi
        je     loc_4_0027
        cmp    dword ptr [edi + 0x33], 0
        jne    loc_4_002E
        cmp    dword ptr [edi + 0x3b], 0
        jne    loc_4_002E
        cmp    dword ptr [edi + 0x43], 0
        jne    loc_4_002E
loc_4_0027:
        mov    eax, 0
        jmp    loc_4_0033
loc_4_002E:
        mov    eax, 0xffffffff
loc_4_0033:
        pop    edi
        pop    ebp
        ret    
sub_4_0036:
        cmp    eax, 0x40
        jle    loc_4_0040
        mov    eax, 0x40
loc_4_0040:
        add    eax, 3
        shr    eax, 2
loc_4_0046:
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
        jne    loc_4_0088
        mov    eax, ecx
        shr    eax, 1
        jne    loc_4_0046
        xor    ecx, ecx
        stc    
        ret    
loc_4_0088:
        shl    ecx, 0xc
        clc    
        ret    
sub_4_008D:
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_4_00C8
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_00C8
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
loc_4_00C3:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
loc_4_00C8:
        stc    
        jmp    loc_4_00C3
sub_4_00CB:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     loc_4_0103
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_0103
loc_4_00E6:
        cmp    dword ptr [eax + 4], edi
        je     loc_4_00F5
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_0103
        jmp    loc_4_00E6
loc_4_00F5:
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_4_0103:
        popfd  
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_4_0109:
        jmp    loc_4_0115
loc_4_010B:
        mov    esi, 0
        jmp    loc_4_01CD
loc_4_0115:
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
        je     loc_4_01C7
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_0150:
        push   ecx
        mov    ecx, 0xa
        mov    esi, 0                        ; -> loc_4_010B
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        loop   loc_4_0150
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_016B:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0xa
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_019E
        add    edi, 6
        add    esi, 0xa
        loop   loc_4_016B
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_4_01CD
loc_4_019E:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_01B9
        add    edi, 2
loc_4_01AB:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_01AB
loc_4_01B9:
        push   0
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_01C7:
        xor    eax, eax
        mov    dword ptr [esp + 0x1c], eax
loc_4_01CD:
        popal  
        add    esp, 0xc
        pop    ebp
        ret    
sub_4_01D3:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_01DD:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_01DD
        pop    eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret    
sub_4_01FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_04EA
        movzx  edx, word ptr [ebp + 0xc]
        push   2
        push   edx
        call   sub_1_0154
loc_4_0224:
        add    esp, 8
        or     edi, edi
        jne    loc_4_04EA
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_04EA
        mov    dword ptr [ebp - 4], eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    dword ptr [eax], 0
        push   1
        push   0x86
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_04EA
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x53], eax
        mov    word ptr [edi + 0x84], 0
        mov    ax, word ptr [ebp + 0x28]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [edi + 0x13], ax
        mov    byte ptr [edi + 0xf], 0x16
        mov    ax, word ptr [ebp + 0x18]
        mov    byte ptr [edi + 0x10], al
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edi + 8], ax
        mov    ax, word ptr [ebp + 0x2c]
        mov    word ptr [edi + 0xa], ax
        mov    ax, word ptr [ebp + 0x30]
        mov    byte ptr [edi + 0xc], al
        mov    ax, word ptr [ebp + 0x1c]
        mov    byte ptr [edi + 0xd], al
        mov    ax, word ptr [ebp + 0x20]
        mov    byte ptr [edi + 0xe], al
        call   sub_5_1348
loc_4_02C5:
        mov    word ptr [edi + 0x15], ax
        call   sub_1_0548
loc_4_02CE:
        jb     loc_4_04EA
        push   edi
        movzx  ecx, byte ptr [edi + 0xd]
        mov    word ptr [0], cx              ; -> obj6:0x027C
        mov    dword ptr [0x1c], edi         ; -> obj6:0x0298
        mov    edi, 0                        ; -> obj6:0x027C
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edi
        jae    loc_4_0306
        push   0                             ; -> obj6:0x0303
        call   sub_5_085C
loc_4_02FE:
        add    esp, 4
        jmp    loc_4_04EA
loc_4_0306:
        mov    dword ptr [edi + 0x2f], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 4
        jne    loc_4_0323
        or     word ptr [edi + 4], 0x8000
loc_4_0323:
        movsx  edx, word ptr [ebp + 0x2c]
        cmp    edx, -1
        je     loc_4_037B
        push   edi
        movzx  ecx, byte ptr [edi + 0xc]
        mov    word ptr [0], cx              ; -> obj6:0x029C
        mov    dword ptr [0x1c], edi         ; -> obj6:0x02B8
        mov    edi, 0                        ; -> obj6:0x029C
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edi
        jae    loc_4_035E
        push   0                             ; -> obj6:0x031B
        call   sub_5_085C
loc_4_0356:
        add    esp, 4
        jmp    loc_4_04EA
loc_4_035E:
        mov    dword ptr [edi + 0x2b], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 4
        jne    loc_4_037B
        or     word ptr [edi + 4], 0x4000
loc_4_037B:
        mov    esi, 0                        ; -> obj6:0x01F2
        cmp    word ptr [ebp + 0x14], -1
        jne    loc_4_038C
        mov    esi, 0                        ; -> obj6:0x0190
loc_4_038C:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0xc]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0399:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0399
        mov    edx, dword ptr [esp]
        call   sub_4_0109
        pop    edi
        mov    dword ptr [edi + 0x57], eax
        or     eax, eax
        jne    loc_4_03C4
        push   0                             ; -> obj6:0x02BC
        call   sub_5_085C
loc_4_03BC:
        add    esp, 4
        jmp    loc_4_04EA
loc_4_03C4:
        movsx  edx, word ptr [ebp + 0x2c]
        cmp    edx, -1
        je     loc_4_040A
        mov    word ptr [edi + 0xa], dx
        mov    esi, 0                        ; -> obj6:0x026E
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_03DF:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_03DF
        mov    edx, dword ptr [esp]
        call   sub_4_0109
        pop    edi
        mov    dword ptr [edi + 0x5f], eax
        or     eax, eax
        jne    loc_4_040A
        push   0                             ; -> obj6:0x02D4
        call   sub_5_085C
loc_4_0402:
        add    esp, 4
        jmp    loc_4_04EA
loc_4_040A:
        movsx  edx, word ptr [ebp + 0x14]
        cmp    edx, -1
        je     loc_4_0454
        mov    word ptr [edi + 8], dx
        mov    dx, 0x388
        mov    esi, 0                        ; -> obj6:0x0254
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0429:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0429
        mov    edx, dword ptr [esp]
        call   sub_4_0109
        pop    edi
        mov    dword ptr [edi + 0x5b], eax
        or     eax, eax
        jne    loc_4_0454
        push   0                             ; -> obj6:0x02EC
        call   sub_5_085C
loc_4_044C:
        add    esp, 4
        jmp    loc_4_04EA
loc_4_0454:
        mov    eax, dword ptr [ebp + 0x24]
        call   sub_4_0036
        mov    dword ptr [edi + 0x17], eax
        mov    dword ptr [edi + 0x1b], ebx
        mov    dword ptr [edi + 0x23], ecx
        mov    dword ptr [edi + 0x1f], edx
        jae    loc_4_0479
        push   0                             ; -> obj6:0x034E
        call   sub_5_085C
loc_4_0474:
        add    esp, 4
        jmp    loc_4_04EA
loc_4_0479:
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
        mov    word ptr [edi + 0x27], ax
        movzx  eax, word ptr [ebp + 0x20]
        mov    esi, 0                        ; -> loc_1_0254
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [edi + 0x4b], eax
        jae    loc_4_04C1
        push   0                             ; -> obj6:0x0336
        call   sub_5_085C
loc_4_04BC:
        add    esp, 4
        jmp    loc_4_04EA
loc_4_04C1:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_04C8:
        call   sub_4_008D
        jb     loc_4_04E9
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_04C8
        pop    ebx
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_04E3:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_04E9:
        pop    ebx
loc_4_04EA:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     loc_4_04F6
        call   sub_4_0500
loc_4_04F6:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_04E3
sub_4_0500:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_0610
        xor    esi, esi
        xchg   dword ptr [edi + 0x4f], esi
        or     esi, esi
        je     loc_4_0523
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_0523:
        mov    eax, dword ptr [edi + 0x4b]
        or     eax, eax
        je     loc_4_0530
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_0530:
        movzx  eax, word ptr [edi + 0x27]
        or     eax, eax
        je     loc_4_0553
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        mov    eax, dword ptr [edi + 0x17]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_0553:
        mov    eax, dword ptr [edi + 0x5b]
        or     eax, eax
        je     loc_4_0582
        mov    esi, 0                        ; -> obj6:0x0254
        movzx  ecx, word ptr [esi]
        mov    edx, 0x388
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_056D:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_056D
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x5b]
        call   sub_4_01D3
        pop    edi
loc_4_0582:
        mov    eax, dword ptr [edi + 0x5f]
        or     eax, eax
        je     loc_4_05B0
        mov    esi, 0                        ; -> obj6:0x026E
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 0xa]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_059B:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_059B
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x5f]
        call   sub_4_01D3
        pop    edi
loc_4_05B0:
        mov    eax, dword ptr [edi + 0x57]
        or     eax, eax
        je     loc_4_05EA
        mov    esi, 0                        ; -> obj6:0x01F2
        cmp    word ptr [edi + 8], -1
        jne    loc_4_05C8
        mov    esi, 0                        ; -> obj6:0x0190
loc_4_05C8:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_05D5:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_05D5
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x57]
        call   sub_4_01D3
        pop    edi
loc_4_05EA:
        mov    eax, dword ptr [edi + 0x2b]
        or     eax, eax
        je     loc_4_05F7
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_05F7:
        mov    eax, dword ptr [edi + 0x2f]
        or     eax, eax
        je     loc_4_0604
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_0604:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_0610:
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
sub_4_062D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_4_07E8
        jb     loc_4_0701
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 8], dx
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_1_03CB
loc_4_065C:
        test   dword ptr [ebp + 0x28], 0x50
        jne    loc_4_067C
        movsx  eax, word ptr [ebp + 0x2c]
        movsx  ecx, word ptr [ebp + 0x10]
        mov    bx, word ptr [ebp + 0x30]
        mov    edi, dword ptr [ebp + 0x34]
        mov    esi, dword ptr [ebp + 0x38]
        call   sub_1_05A7
loc_4_067C:
        call   sub_1_05F6
loc_4_0681:
        movsx  edx, word ptr [ebp + 0x10]
        cmp    edx, -1
        je     loc_4_0691
        call   sub_4_0865
        jb     loc_4_06FB
loc_4_0691:
        movsx  edx, word ptr [ebp + 0x2c]
        cmp    edx, -1
        je     loc_4_06A1
        call   sub_4_08D0
        jb     loc_4_06FB
loc_4_06A1:
        test   dword ptr [ebp + 0x28], 0x50
        jne    loc_4_06C6
        movzx  edx, word ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x1c]
        mov    bx, word ptr [ebp + 0x14]
        call   sub_1_0487
loc_4_06BA:
        mov    esi, dword ptr [ebp + 0x20]
        mov    bx, word ptr [ebp + 0x18]
        call   sub_1_04BB
loc_4_06C6:
        push   dword ptr [ebp + 0x30]
        push   dword ptr [ebp + 0x2c]
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_01FC
        add    esp, 0x2c
        or     eax, eax
        je     loc_4_06FB
        mov    dword ptr [esp + 0x1c], 0
loc_4_06FB:
        popal  
        add    esp, 8
        pop    ebp
        ret    
loc_4_0701:
        mov    dword ptr [esp + 0x1c], 0x24
        jmp    loc_4_06FB
sub_4_070B:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        pushfd 
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     loc_4_0764
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0154
loc_4_072A:
        add    esp, 8
        or     edi, edi
        je     loc_4_0764
        mov    ebx, dword ptr [edi + 0x33]
        or     ebx, ebx
        je     loc_4_0742
        mov    eax, 1
        call   sub_5_117F
loc_4_0742:
        mov    ebx, dword ptr [edi + 0x43]
        or     ebx, ebx
        je     loc_4_0753
        mov    eax, 4
        call   sub_5_117F
loc_4_0753:
        mov    ebx, dword ptr [edi + 0x3b]
        or     ebx, ebx
        je     loc_4_0764
        mov    eax, 2
        call   sub_5_117F
loc_4_0764:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_07B1
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_07B1
loc_4_077A:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x53], ebx
        je     loc_4_078B
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_07B1
        jmp    loc_4_077A
loc_4_078B:
        popfd  
        pushfd 
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0794:
        call   sub_4_00CB
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0794
        pop    ebx
        mov    edi, eax
        call   sub_4_0500
        mov    eax, 0xffffffff
loc_4_07B1:
        popfd  
        pop    ebx
        pop    edi
        pop    ebp
        ret    
loc_4_07B6:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_08AD
loc_4_07CD:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_07D3:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_07B6
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
        db  0CCh
sub_4_07E8:
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
        call   sub_1_0354
loc_4_07FE:
        jb     loc_4_0852
        cmp    al, 0xaa
        jne    loc_4_0852
        mov    al, 0xe1
        call   sub_1_0370
loc_4_080B:
        jb     loc_4_0852
        call   sub_1_0354
loc_4_0812:
        jb     loc_4_0852
        mov    ah, al
        call   sub_1_0354
loc_4_081B:
        jb     loc_4_0852
        movzx  eax, ax
        mov    dword ptr [esp + 4], eax
        mov    al, 0xe7
        call   sub_1_0370
loc_4_082B:
        jb     loc_4_0852
        call   sub_1_0354
loc_4_0832:
        jb     loc_4_0852
        mov    ah, al
        call   sub_1_0354
loc_4_083B:
        jb     loc_4_0852
        cmp    ah, 0x68
        jne    loc_4_0852
        mov    ah, al
        and    ah, 0xf
        cmp    ah, 8
        jb     loc_4_0852
        and    al, 0xf0
        cmp    al, 0x80
        je     loc_4_085B
loc_4_0852:
        xor    eax, eax
        mov    dword ptr [esp + 4], eax
        stc    
        jmp    loc_4_0862
loc_4_085B:
        movzx  eax, ah
        mov    dword ptr [esp], eax
        clc    
loc_4_0862:
        pop    edx
        pop    eax
        ret    
sub_4_0865:
        push   eax
        push   ebx
        push   ecx
        pushfd 
        cli    
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_063C
loc_4_0876:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_063C
loc_4_0882:
        mov    ebx, 2
        mov    al, 0x80
        call   sub_1_063C
loc_4_088E:
        mov    ebx, 4
        mov    al, 1
        call   sub_1_063C
loc_4_089A:
        mov    cx, 0x4000
loc_4_089E:
        in     al, dx
        test   al, 0x40
        jne    loc_4_08A5
        loop   loc_4_089E
loc_4_08A5:
        push   eax
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_063C
loc_4_08B2:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_063C
loc_4_08BE:
        pop    eax
        popfd  
        test   al, 0x40
        je     loc_4_08CB
        test   al, 6
        jne    loc_4_08CB
        clc    
        jmp    loc_4_08CC
loc_4_08CB:
        stc    
loc_4_08CC:
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_4_08D0:
        enter  8, 0
        push   eax
        push   ecx
        push   edx
        mov    dword ptr [ebp - 8], 0
loc_4_08DE:
        mov    dword ptr [ebp - 4], 0
        mov    ecx, 0x40
loc_4_08EA:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 0x80
        jne    loc_4_08F4
        in     al, dx
        loop   loc_4_08EA
loc_4_08F4:
        mov    ecx, 0x1000
loc_4_08F9:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 0x40
        je     loc_4_0916
        loop   loc_4_08F9
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_0940
        mov    dword ptr [ebp - 4], 0xffffffff
        inc    edx
        mov    al, 0xff
        out    dx, al
        dec    edx
        jmp    loc_4_08F4
loc_4_0916:
        mov    al, 0xff
        inc    edx
        out    dx, al
        dec    edx
        mov    ecx, 0x1000
loc_4_0920:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 0x80
        je     loc_4_0938
        loop   loc_4_0920
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_0940
        mov    dword ptr [ebp - 8], 0xffffffff
        jmp    loc_4_08DE
loc_4_0938:
        in     al, dx
        cmp    al, 0xfe
        jne    loc_4_0940
        clc    
        jmp    loc_4_0941
loc_4_0940:
        stc    
loc_4_0941:
        pop    edx
        pop    ecx
        pop    eax
        leave  
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x2761  R-X
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
        call   sub_5_2668
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_051D
        mov    edi, 0                        ; -> obj6:0x00E8
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
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0x150]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_01F1
        xor    eax, eax
        jmp    loc_5_051D
loc_5_01F1:
        push   1
        lea    eax, [ebp - 0x250]
        push   0x100
        push   eax
        push   0
        push   ebx
        call   sub_5_2668
        add    esp, 0x14
        mov    esi, eax
        test   esi, esi
        je     loc_5_0218
        push   dword ptr [ebp - 0x10]
        jmp    loc_5_0513
loc_5_0218:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x250]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_023E
        mov    esi, 0x1d
        jmp    loc_5_04E8
loc_5_023E:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_037E
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x50]
        mov    dword ptr [ebp - 8], 0x40
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj6:0x00D0
        push   dword ptr [ebp - 0x10]
        call   sub_5_2660
        add    esp, 0x18
        test   eax, eax
        je     loc_5_0279
        mov    esi, 0x1d
        jmp    loc_5_04E8
loc_5_0279:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_02DE
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x00B0
        push   dword ptr [ebp - 0xc]
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_02AB
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x00B0
        push   dword ptr [ebp - 0xc]
        call   sub_5_2650
        add    esp, 0xc
loc_5_02AB:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_030F
        push   dword ptr [ebp - 8]
        lea    ecx, [ebp - 0x50]
        push   ecx
        push   1
        push   0
        push   0                             ; -> obj6:0x0014
        push   dword ptr [ebp - 4]
        call   sub_5_2648
        add    esp, 0x18
        push   dword ptr [ebp - 4]
        call   sub_5_2638
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_02DE:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x50]
        mov    dword ptr [ebp - 8], 0x40
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj6:0x0098
        push   dword ptr [ebp - 0x10]
        call   sub_5_2660
        add    esp, 0x18
        test   eax, eax
        je     loc_5_0319
        mov    esi, 0x1d
        jmp    loc_5_04E8
loc_5_030F:
        mov    esi, 0x1d
        jmp    loc_5_04E8
loc_5_0319:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_037E
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0078
        push   dword ptr [ebp - 0xc]
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_034B
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0078
        push   dword ptr [ebp - 0xc]
        call   sub_5_2650
        add    esp, 0xc
loc_5_034B:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_03AF
        push   dword ptr [ebp - 8]
        lea    ecx, [ebp - 0x50]
        push   ecx
        push   1
        push   0
        push   0                             ; -> obj6:0x0014
        push   dword ptr [ebp - 4]
        call   sub_5_2648
        add    esp, 0x18
        push   dword ptr [ebp - 4]
        call   sub_5_2638
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_037E:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x50]
        mov    dword ptr [ebp - 8], 0x40
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj6:0x0068
        push   dword ptr [ebp - 0x10]
        call   sub_5_2660
        add    esp, 0x18
        test   eax, eax
        je     loc_5_03B9
        mov    esi, 0x1d
        jmp    loc_5_04E8
loc_5_03AF:
        mov    esi, 0x1d
        jmp    loc_5_04E8
loc_5_03B9:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_041E
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0050
        push   dword ptr [ebp - 0xc]
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_03EB
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0050
        push   dword ptr [ebp - 0xc]
        call   sub_5_2650
        add    esp, 0xc
loc_5_03EB:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_044F
        push   dword ptr [ebp - 8]
        lea    ecx, [ebp - 0x50]
        push   ecx
        push   1
        push   0
        push   0                             ; -> obj6:0x0014
        push   dword ptr [ebp - 4]
        call   sub_5_2648
        add    esp, 0x18
        push   dword ptr [ebp - 4]
        call   sub_5_2638
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_041E:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x50]
        mov    dword ptr [ebp - 8], 0x40
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj6:0x0040
        push   dword ptr [ebp - 0x10]
        call   sub_5_2660
        add    esp, 0x18
        test   eax, eax
        je     loc_5_0459
        mov    esi, 0x1d
        jmp    loc_5_04E8
loc_5_044F:
        mov    esi, 0x1d
        jmp    loc_5_04E8
loc_5_0459:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_04C2
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0028
        push   dword ptr [ebp - 0xc]
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_048B
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0028
        push   dword ptr [ebp - 0xc]
        call   sub_5_2650
        add    esp, 0xc
loc_5_048B:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0524
        push   dword ptr [ebp - 8]
        lea    ecx, [ebp - 0x50]
        push   ecx
        push   1
        push   0
        push   0                             ; -> obj6:0x0014
        push   dword ptr [ebp - 4]
        call   sub_5_2648
        add    esp, 0x18
        push   dword ptr [ebp - 4]
        call   sub_5_2638
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_04C2:
        push   dword ptr [ebp - 0x10]
        call   sub_5_2638
        add    esp, 4
        lea    eax, [ebp - 0x150]
        mov    dword ptr [ebp - 0x10], 0
        push   eax
        push   0x80000002
        call   sub_5_2640
        add    esp, 8
loc_5_04E8:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_5_04F9
        push   dword ptr [ebp - 0x10]
        call   sub_5_2638
        add    esp, 4
loc_5_04F9:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_050A
        push   dword ptr [ebp - 0xc]
        call   sub_5_2638
        add    esp, 4
loc_5_050A:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_051B
        push   dword ptr [ebp - 4]
loc_5_0513:
        call   sub_5_2638
        add    esp, 4
loc_5_051B:
        mov    eax, esi
loc_5_051D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_0524:
        mov    esi, 0x1d
        jmp    loc_5_04E8
sub_5_052B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x110
        test   byte ptr [ebp + 0x20], 0x50
        push   ebx
        push   esi
        push   edi
        je     loc_5_0544
        xor    eax, eax
        jmp    loc_5_0855
loc_5_0544:
        push   1
        lea    eax, [ebp - 0x110]
        push   0x100
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_5_2668
        add    esp, 0x14
        mov    dword ptr [ebp - 0x10], eax
        test   eax, eax
        jne    loc_5_0855
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x110]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_0590
        mov    eax, 0x13
        jmp    loc_5_0855
loc_5_0590:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_5_06F4
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj6:0x00B0
        push   dword ptr [ebp - 0xc]
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_05C7
        push   dword ptr [ebp - 0xc]
        call   sub_5_2638
        add    esp, 4
        mov    eax, 0x13
        jmp    loc_5_0855
loc_5_05C7:
        mov    esi, 0                        ; -> obj6:0x0000
        mov    edi, 0x100
        xor    ebx, ebx
loc_5_05D3:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x110]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   ecx
        push   ebx
        push   ebx
        push   dword ptr [esi]
        push   dword ptr [ebp - 8]
        call   sub_5_2660
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_05F9
        cmp    dword ptr [ebp - 4], ebx
        jne    loc_5_0604
loc_5_05F9:
        add    esi, 4
        cmp    esi, 0xc                      ; -> obj6:0x000C
        jb     loc_5_05D3
loc_5_0604:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0835
        push   0
        lea    ecx, [ebp - 0x110]
        push   0xfff
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   sub_5_0068
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_0835
        push   dword ptr [ebp - 8]
        call   sub_5_2638
        add    esp, 4
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj6:0x0078
        push   dword ptr [ebp - 0xc]
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_0667
        push   dword ptr [ebp - 0xc]
        call   sub_5_2638
        add    esp, 4
        mov    eax, 0x13
        jmp    loc_5_0855
loc_5_0667:
        mov    esi, 0                        ; -> obj6:0x0000
        mov    edi, 0x100
        xor    ebx, ebx
loc_5_0673:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x110]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   ecx
        push   ebx
        push   ebx
        push   dword ptr [esi]
        push   dword ptr [ebp - 8]
        call   sub_5_2660
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_0699
        cmp    dword ptr [ebp - 4], ebx
        jne    loc_5_06A4
loc_5_0699:
        add    esi, 4
        cmp    esi, 0xc                      ; -> obj6:0x000C
        jb     loc_5_0673
loc_5_06A4:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0835
        push   1
        mov    esi, dword ptr [ebp + 0x10]
        push   0xf
        lea    eax, [ebp - 0x110]
        push   esi
        push   eax
        call   sub_5_0068
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_0835
        cmp    word ptr [esi], 0xffff
        je     loc_5_06E9
loc_5_06D4:
        cmp    word ptr [esi], 2
        jne    loc_5_06DF
        mov    word ptr [esi], 9
loc_5_06DF:
        add    esi, 2
        cmp    word ptr [esi], 0xffff
        jne    loc_5_06D4
loc_5_06E9:
        push   dword ptr [ebp - 8]
        call   sub_5_2638
        add    esp, 4
loc_5_06F4:
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj6:0x0050
        push   dword ptr [ebp - 0xc]
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_0721
        push   dword ptr [ebp - 0xc]
        call   sub_5_2638
        add    esp, 4
        mov    eax, 0x13
        jmp    loc_5_0855
loc_5_0721:
        mov    esi, 0                        ; -> obj6:0x0000
        mov    edi, 0x100
        xor    ebx, ebx
loc_5_072D:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x110]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   ecx
        push   ebx
        push   ebx
        push   dword ptr [esi]
        push   dword ptr [ebp - 8]
        call   sub_5_2660
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_0753
        cmp    dword ptr [ebp - 4], ebx
        jne    loc_5_075E
loc_5_0753:
        add    esi, 4
        cmp    esi, 0xc                      ; -> obj6:0x000C
        jb     loc_5_072D
loc_5_075E:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0835
        push   1
        mov    esi, dword ptr [ebp + 0x18]
        push   0xf
        lea    eax, [ebp - 0x110]
        push   esi
        push   eax
        call   sub_5_0068
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_0835
        cmp    word ptr [esi], 0xffff
        je     loc_5_07A3
loc_5_078E:
        cmp    word ptr [esi], 2
        jne    loc_5_0799
        mov    word ptr [esi], 9
loc_5_0799:
        add    esi, 2
        cmp    word ptr [esi], 0xffff
        jne    loc_5_078E
loc_5_07A3:
        push   dword ptr [ebp - 8]
        call   sub_5_2638
        add    esp, 4
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj6:0x0028
        push   dword ptr [ebp - 0xc]
        call   sub_5_2658
        add    esp, 0xc
        test   eax, eax
        je     loc_5_07D8
        push   dword ptr [ebp - 0xc]
        call   sub_5_2638
        add    esp, 4
        mov    eax, 0x13
        jmp    loc_5_0855
loc_5_07D8:
        mov    esi, 0                        ; -> obj6:0x0000
        mov    edi, 0x100
        xor    ebx, ebx
loc_5_07E4:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x110]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   ecx
        push   ebx
        push   ebx
        push   dword ptr [esi]
        push   dword ptr [ebp - 8]
        call   sub_5_2660
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_080A
        cmp    dword ptr [ebp - 4], ebx
        jne    loc_5_0815
loc_5_080A:
        add    esi, 4
        cmp    esi, 0xc                      ; -> obj6:0x000C
        jb     loc_5_07E4
loc_5_0815:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0835
        push   1
        lea    ecx, [ebp - 0x110]
        push   3
        push   dword ptr [ebp + 0x1c]
        push   ecx
        call   sub_5_0068
        add    esp, 0x10
        test   eax, eax
        je     loc_5_083C
loc_5_0835:
        mov    dword ptr [ebp - 0x10], 0x13
loc_5_083C:
        push   dword ptr [ebp - 8]
        call   sub_5_2638
        add    esp, 4
        push   dword ptr [ebp - 0xc]
        call   sub_5_2638
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x10]
loc_5_0855:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_085C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        VMMCall  VMM.Open_Boot_Log               ; cd2088010100
        test   eax, eax
        jne    loc_5_08A9
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0     ; -> obj6:0x00F0
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
loc_5_08A9:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_08AD:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x158
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_5_0D26
        cmp    eax, 1
        je     loc_5_0901
        cmp    eax, 2
        je     loc_5_0D11
        cmp    eax, 3
        je     loc_5_0C83
        cmp    eax, 4
        je     loc_5_0CCF
        cmp    eax, 8
        je     loc_5_0CD7
        cmp    eax, 0xd
        je     loc_5_0D11
        mov    eax, 1
        jmp    loc_5_0D28
loc_5_0901:
        mov    eax, 0xfffe
        mov    ecx, 0xffff
        mov    word ptr [ebp - 0x54], ax
        mov    word ptr [ebp - 0x52], ax
        mov    word ptr [ebp - 0x50], ax
        mov    word ptr [ebp - 0x4e], ax
        mov    word ptr [ebp - 0x4c], cx
        mov    word ptr [ebp - 0x60], ax
        mov    word ptr [ebp - 0x5e], ax
        mov    word ptr [ebp - 0x5c], ax
        mov    word ptr [ebp - 0x5a], ax
        mov    word ptr [ebp - 0x58], ax
        mov    word ptr [ebp - 0x56], cx
        mov    word ptr [ebp - 0x48], ax
        mov    word ptr [ebp - 0x46], ax
        mov    word ptr [ebp - 0x44], ax
        mov    word ptr [ebp - 0x42], ax
        mov    word ptr [ebp - 0x40], cx
        push   0
        mov    word ptr [ebp - 0x3c], ax
        mov    esi, dword ptr [ebp + 0x10]
        mov    word ptr [ebp - 0x3a], ax
        push   esi
        mov    word ptr [ebp - 0x38], ax
        lea    eax, [ebp - 0x158]
        mov    word ptr [ebp - 0x36], cx
        push   eax
        call   sub_5_2680
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_0D28
        mov    eax, 0xffff
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 6], ax
        mov    word ptr [ebp - 0xc], ax
        movzx  eax, word ptr [ebp - 0xfc]
        cmp    eax, 1
        je     loc_5_09AD
        cmp    eax, 2
        je     loc_5_09D1
        cmp    eax, 3
        je     loc_5_0A25
        mov    eax, 0x13
        jmp    loc_5_0D28
loc_5_09AD:
        cmp    word ptr [ebp - 0xd2], 0x10
        je     loc_5_09C1
        mov    eax, 0x13
        jmp    loc_5_0D28
loc_5_09C1:
        mov    ax, word ptr [ebp - 0xfa]
        mov    word ptr [ebp - 2], ax
        jmp    loc_5_0A70
loc_5_09D1:
        cmp    word ptr [ebp - 0xd2], 0x10
        je     loc_5_09E5
        mov    eax, 0x13
        jmp    loc_5_0D28
loc_5_09E5:
        mov    ax, word ptr [ebp - 0xfa]
        mov    word ptr [ebp - 2], ax
        movzx  eax, word ptr [ebp - 0xd0]
        cmp    eax, 2
        je     loc_5_0A0B
        cmp    eax, 4
        je     loc_5_0A18
        mov    eax, 0x13
        jmp    loc_5_0D28
loc_5_0A0B:
        mov    ax, word ptr [ebp - 0xf8]
        mov    word ptr [ebp - 6], ax
        jmp    loc_5_0A70
loc_5_0A18:
        mov    ax, word ptr [ebp - 0xf8]
        mov    word ptr [ebp - 4], ax
        jmp    loc_5_0A70
loc_5_0A25:
        cmp    word ptr [ebp - 0xd2], 0x10
        jne    loc_5_0C79
        cmp    word ptr [ebp - 0xd0], 4
        jne    loc_5_0C79
        cmp    word ptr [ebp - 0xce], 2
        jne    loc_5_0C79
        mov    ax, word ptr [ebp - 0xfa]
        mov    cx, word ptr [ebp - 0xf8]
        mov    dx, word ptr [ebp - 0xf6]
        mov    word ptr [ebp - 2], ax
        mov    word ptr [ebp - 4], cx
        mov    word ptr [ebp - 6], dx
loc_5_0A70:
        cmp    word ptr [ebp - 0xaa], 1
        jb     loc_5_0C6F
        cmp    word ptr [ebp - 0x9a], 1
        jb     loc_5_0C6F
        movzx  ax, byte ptr [ebp - 0xa8]
        cmp    word ptr [ebp - 6], 0xffff
        mov    word ptr [ebp - 8], ax
        je     loc_5_0AC0
        cmp    word ptr [ebp - 0xaa], 2
        jae    loc_5_0AB4
        mov    eax, 0x13
        jmp    loc_5_0D28
loc_5_0AB4:
        movzx  ax, byte ptr [ebp - 0xa7]
        mov    word ptr [ebp - 0xc], ax
loc_5_0AC0:
        movzx  ax, byte ptr [ebp - 0x98]
        cmp    word ptr [ebp - 2], 0xffff
        mov    word ptr [ebp - 0xa], ax
        jne    loc_5_0ADE
        mov    eax, 0x13
        jmp    loc_5_0D28
loc_5_0ADE:
        cmp    word ptr [ebp - 4], 0xffff
        je     loc_5_0AEE
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 4], ax
loc_5_0AEE:
        xor    edi, edi
        lea    eax, [ebp - 0x34]
        push   edi
        lea    ecx, [ebp - 0x10]
        push   edi
        mov    dword ptr [ebp - 0x34], edi
        push   eax
        push   ecx
        push   esi
        call   sub_5_2678
        add    esp, 0x14
        test   eax, eax
        je     loc_5_0B0D
        mov    dword ptr [ebp - 0x10], edi
loc_5_0B0D:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, 8
        je     loc_5_0B21
        cmp    eax, 0x10
        je     loc_5_0B28
        cmp    eax, 0x40
        je     loc_5_0B2F
        jmp    loc_5_0B34
loc_5_0B21:
        mov    edi, 0x20
        jmp    loc_5_0B34
loc_5_0B28:
        mov    edi, 0x10
        jmp    loc_5_0B34
loc_5_0B2F:
        mov    edi, 0x40
loc_5_0B34:
        mov    ax, word ptr [ebp - 6]
        sub    ax, 0xffff
        cmp    ax, 1
        sbb    ebx, ebx
        inc    ebx
        push   ebx
        push   esi
        call   sub_5_016A
        add    esp, 8
        test   eax, eax
        jne    loc_5_0D28
        push   edi
        lea    eax, [ebp - 0x3c]
        push   eax
        lea    ecx, [ebp - 0x48]
        push   ecx
        lea    eax, [ebp - 0x60]
        push   ebx
        lea    ecx, [ebp - 0x54]
        push   eax
        push   ecx
        push   esi
        call   sub_5_052B
        add    esp, 0x1c
        test   eax, eax
        jne    loc_5_0D28
        lea    eax, [ebp - 0x60]
        lea    ecx, [ebp - 0x54]
        push   eax
        push   ecx
        lea    eax, [ebp - 0x3c]
        lea    ecx, [ebp - 0x48]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 6]
        push   edi
        push   0x20
        push   eax
        push   ecx
        push   dword ptr [ebp - 0xa]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_4_062D
loc_5_0BA3:
        add    esp, 0x34
        mov    edi, eax
        test   edi, edi
        jne    loc_5_0C5B
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj6:0x0124
        mov    ax, word ptr [ebp - 4]
        mov    dword ptr [ebp - 0x28], esi
        sub    ax, 0xffff
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 0x30], eax
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0001h              ; cd2001004a04
        add    esp, 0xc
        mov    dword ptr [ebp - 0x24], ebx
        mov    dword ptr [ebp - 0x1c], esi
        mov    dword ptr [ebp - 0x20], 0     ; -> obj6:0x0114
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0001h              ; cd2001004a04
        add    esp, 0xc
        test   edi, edi
        jne    loc_5_0C68
        cmp    word ptr [ebp - 8], 9
        mov    eax, 2
        je     loc_5_0C1D
        movzx  eax, word ptr [ebp - 8]
loc_5_0C1D:
        movzx  ecx, word ptr [ebp - 0xa]
        movzx  edx, word ptr [ebp - 2]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 0x80]
        push   0                             ; -> obj6:0x0104
        push   ecx
        call   sub_5_2670
        add    esp, 0x14
        lea    ecx, [ebp - 0x80]
        mov    dword ptr [ebp - 0x14], 0     ; -> obj6:0x00FC
        mov    dword ptr [ebp - 0x18], ecx
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_5_0C5B:
        test   edi, edi
        jne    loc_5_0C68
        push   esi
        call   sub_5_1E0D
        add    esp, 4
loc_5_0C68:
        mov    eax, edi
        jmp    loc_5_0D28
loc_5_0C6F:
        mov    eax, 0x13
        jmp    loc_5_0D28
loc_5_0C79:
        mov    eax, 0x13
        jmp    loc_5_0D28
loc_5_0C83:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_0C99
        cmp    eax, 1
        je     loc_5_0CB4
        mov    eax, 1
        jmp    loc_5_0D28
loc_5_0C99:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_0CA1:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_0D28
loc_5_0CB4:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_0CBC:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_0D28
loc_5_0CCF:
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_5_0D11
        jmp    loc_5_0D26
loc_5_0CD7:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x7fffffff
        cmp    eax, 4
        jb     loc_5_0D0A
        cmp    eax, 5
        jbe    loc_5_0CF5
        cmp    eax, 6
        jb     loc_5_0D0A
        cmp    eax, 8
        jbe    loc_5_0CFF
        jmp    loc_5_0D0A
loc_5_0CF5:
        push   dword ptr [ebp + 0x10]
        call   sub_3_004C
loc_5_0CFD:
        jmp    loc_5_0D07
loc_5_0CFF:
        push   dword ptr [ebp + 0x10]
        call   sub_3_00C3
loc_5_0D07:
        add    esp, 4
loc_5_0D0A:
        mov    eax, 1
        jmp    loc_5_0D28
loc_5_0D11:
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_5_1EBC
        add    esp, 4
        push   esi
        call   sub_4_070B
loc_5_0D23:
        add    esp, 4
loc_5_0D26:
        xor    eax, eax
loc_5_0D28:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh
sub_5_0D30:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 0x105
        mov    al, 1
        call   sub_1_063C
loc_5_0D44:
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_063C
loc_5_0D50:
        mov    ebx, 0x104
        mov    al, 0x3f
        call   sub_1_063C
loc_5_0D5C:
        mov    ebx, 8
        xor    al, al
        call   sub_1_063C
loc_5_0D68:
        mov    ebx, 0xbd
        mov    al, 0xc0
        call   sub_1_063C
loc_5_0D74:
        mov    ecx, 0x15
        mov    ebx, 0x40
        mov    al, 0x3f
loc_5_0D80:
        xor    bh, bh
        call   sub_1_063C
loc_5_0D87:
        mov    bh, 1
        call   sub_1_063C
loc_5_0D8E:
        inc    bl
        loop   loc_5_0D80
        mov    ecx, 8
        mov    ebx, 0xb0
        xor    eax, eax
loc_5_0D9E:
        xor    bh, bh
        call   sub_1_063C
loc_5_0DA5:
        mov    bh, 1
        call   sub_1_063C
loc_5_0DAC:
        inc    bl
        loop   loc_5_0D9E
        mov    ebx, 0x104
        xor    eax, eax
        call   sub_1_063C
loc_5_0DBC:
        mov    ebx, 0x105
        call   sub_1_063C
loc_5_0DC6:
        call   sub_1_062D
loc_5_0DCB:
        call   sub_1_062D
loc_5_0DD0:
        call   sub_1_062D
loc_5_0DD5:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret    
        db  08Bh, 0C0h
loc_5_0DDC:
        push   eax
        mov    eax, dword ptr [esi + 0x33]
        cmp    eax, ebx
        je     loc_5_0DFC
        or     eax, eax
        jne    loc_5_0E0F
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_1251
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0E0F
loc_5_0DFC:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0E08
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0E08:
        je     loc_5_0E0D
        in     al, dx
        jmp    loc_5_0E0E
loc_5_0E0D:
        out    dx, al
loc_5_0E0E:
        ret    
loc_5_0E0F:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
loc_5_0E20:
        push   eax
        mov    eax, dword ptr [esi + 0x43]
        cmp    eax, ebx
        je     loc_5_0E40
        or     eax, eax
        jne    loc_5_0E53
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 4
        call   sub_5_1251
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0E53
loc_5_0E40:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0E4C
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0E4C:
        je     loc_5_0E51
        in     al, dx
        jmp    loc_5_0E52
loc_5_0E51:
        out    dx, al
loc_5_0E52:
        ret    
loc_5_0E53:
        mov    eax, 4
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
loc_5_0E64:
        push   eax
        mov    eax, dword ptr [esi + 0x3b]
        cmp    eax, ebx
        je     loc_5_0E89
        or     eax, eax
        jne    loc_5_0E9C
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_1251
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0E9C
        call   sub_5_0D30
loc_5_0E89:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0E95
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0E95:
        je     loc_5_0E9A
        in     al, dx
        jmp    loc_5_0E9B
loc_5_0E9A:
        out    dx, al
loc_5_0E9B:
        ret    
loc_5_0E9C:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_0EAD:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0EBB:
        cmp    ecx, 3
        jbe    loc_5_0ED0
        cmp    ecx, 8
        je     loc_5_0ED0
        cmp    ecx, 9
        je     loc_5_0ED0
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
loc_5_0ED0:
        inc    dx
        cmp    cx, si
        je     loc_5_0EDB
        inc    cx
        jmp    loc_5_0EBB
loc_5_0EDB:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0EE1:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0EEF:
        cmp    ecx, 3
        jbe    loc_5_0F04
        cmp    ecx, 8
        je     loc_5_0F04
        cmp    ecx, 9
        je     loc_5_0F04
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
loc_5_0F04:
        inc    dx
        cmp    cx, si
        je     loc_5_0F0F
        inc    cx
        jmp    loc_5_0EEF
loc_5_0F0F:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0F15:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 4
        mov    eax, 0x60
        call   sub_1_063C
loc_5_0F2B:
        call   sub_1_062D
loc_5_0F30:
        pop    edx
        pop    ebx
        pop    eax
        ret    
sub_5_0F34:
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
loc_5_0F7A:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_0F8B
        inc    cx
        jmp    loc_5_0F7A
loc_5_0F8B:
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0F90:
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
loc_5_0FD7:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0FE8
        inc    cx
        jmp    loc_5_0FD7
loc_5_0FE8:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0FEE:
        push   esi
        push   eax
        push   ecx
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0xa]
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
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0xa]
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
        jne    loc_5_10AA
        call   sub_1_01B5
loc_5_1043:
        jb     loc_5_10AA
        cmp    eax, 2
        je     loc_5_1069
        cmp    eax, 4
        je     loc_5_1083
        test   dword ptr [esi], 1
        jne    loc_5_10AA
        or     dword ptr [esi], 1
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        jmp    loc_5_109B
loc_5_1069:
        test   dword ptr [esi], 2
        jne    loc_5_10AA
        or     dword ptr [esi], 2
        movzx  ecx, word ptr [0xe]           ; -> loc_2_000E
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        jmp    loc_5_109B
loc_5_1083:
        test   dword ptr [esi], 4
        jne    loc_5_10AA
        or     dword ptr [esi], 4
        movzx  ecx, word ptr [0x10]          ; -> loc_2_0010
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
loc_5_109B:
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_5_10AA:
        ret    
loc_5_10AB:
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_1107
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        sti    
        je     loc_5_1107
loc_5_10BF:
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 0x33], ebx
        jne    loc_5_10D3
        or     eax, 1
        mov    dword ptr [edi + 0x37], 0xffffffff
loc_5_10D3:
        cmp    dword ptr [edi + 0x3b], ebx
        jne    loc_5_10E2
        or     eax, 2
        mov    dword ptr [edi + 0x3f], 0xffffffff
loc_5_10E2:
        cmp    dword ptr [edi + 0x43], ebx
        jne    loc_5_10F1
        or     eax, 4
        mov    dword ptr [edi + 0x47], 0xffffffff
loc_5_10F1:
        or     eax, eax
        je     loc_5_10FA
        call   sub_5_117F
loc_5_10FA:
        pop    eax
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        sti    
        je     loc_5_1107
        jmp    loc_5_10BF
loc_5_1107:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_5_111C
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_111C:
        clc    
        ret    
loc_5_111E:
        pushal 
        pushfd 
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    eax, 5
        mov    ecx, 0x24
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_5_1177
        mov    dword ptr [edi + ebx], esi
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_117B
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        sti    
        je     loc_5_117B
loc_5_114F:
        mov    edi, dword ptr [eax]
        call   sub_4_008D
loc_5_1156:
        jb     loc_5_1164
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        sti    
        je     loc_5_117B
        jmp    loc_5_114F
loc_5_1164:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_5_1177
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_1177:
        popfd  
        stc    
        jmp    loc_5_117D
loc_5_117B:
        popfd  
        clc    
loc_5_117D:
        popal  
        ret    
sub_5_117F:
        push   eax
        push   edx
        push   esi
        call   sub_1_01B5
loc_5_1187:
        jb     loc_5_124C
        test   eax, 4
        je     loc_5_11B1
        cmp    dword ptr [edi + 0x43], ebx
        jne    loc_5_11B1
        movzx  edx, word ptr [edi + 6]
        call   sub_1_05F6
loc_5_11A2:
        call   sub_5_0FEE
        mov    dword ptr [edi + 0x43], 0
        and    dword ptr [esi], 0xfffffffb
loc_5_11B1:
        test   eax, 2
        je     loc_5_11D1
        cmp    dword ptr [edi + 0x3b], ebx
        jne    loc_5_11D1
        call   sub_5_0F15
        call   sub_5_0F34
        mov    dword ptr [edi + 0x3b], 0
        and    dword ptr [esi], 0xfffffffd
loc_5_11D1:
        test   eax, 1
        je     loc_5_120D
        cmp    dword ptr [edi + 0x33], ebx
        jne    loc_5_120D
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_11EE
        mov    eax, dword ptr [edi + 0x4b]
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
loc_5_11EE:
        call   sub_5_0EAD
        movzx  edx, word ptr [edi + 6]
        call   sub_1_03B1
loc_5_11FC:
        jb     loc_5_1203
        call   sub_1_03FB
loc_5_1203:
        mov    dword ptr [edi + 0x33], 0
        and    dword ptr [esi], 0xfffffffe
loc_5_120D:
        cmp    word ptr [edi + 0xa], -1
        je     loc_5_1230
        mov    eax, dword ptr [edi + 0x43]
        or     eax, eax
        jne    loc_5_1230
        mov    eax, dword ptr [edi + 0x2b]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_5_1230:
        mov    eax, dword ptr [edi + 0x33]
        or     eax, eax
        jne    loc_5_124C
        mov    eax, dword ptr [edi + 0x2f]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_5_124C:
        pop    esi
        pop    edx
        pop    eax
        clc    
        ret    
sub_5_1251:
        push   edx
        push   ecx
        push   eax
        test   eax, 4
        je     loc_5_127A
        cmp    word ptr [edi + 0xa], -1
        je     loc_5_12B6
        cmp    dword ptr [edi + 0x43], 0
        je     loc_5_126F
        cmp    dword ptr [edi + 0x43], ebx
        je     loc_5_127A
        jmp    loc_5_12B6
loc_5_126F:
        mov    dword ptr [edi + 0x43], ebx
        call   sub_5_1011
        mov    dword ptr [edi + 0x47], ebx
loc_5_127A:
        test   eax, 2
        je     loc_5_12A0
        cmp    word ptr [edi + 8], -1
        je     loc_5_12B6
        cmp    dword ptr [edi + 0x3b], 0
        je     loc_5_1295
        cmp    dword ptr [edi + 0x3b], ebx
        je     loc_5_12A0
        jmp    loc_5_12B6
loc_5_1295:
        mov    dword ptr [edi + 0x3b], ebx
        call   sub_5_0F90
        mov    dword ptr [edi + 0x3f], ebx
loc_5_12A0:
        test   eax, 1
        je     loc_5_12EB
        cmp    dword ptr [edi + 0x33], 0
        je     loc_5_12BC
        cmp    dword ptr [edi + 0x33], ebx
        je     loc_5_1343
loc_5_12B6:
        stc    
        jmp    loc_5_1344
loc_5_12BC:
        mov    dword ptr [edi + 0x33], ebx
        cmp    dword ptr [edi + 0x37], ebx
        je     loc_5_12CD
        movzx  edx, word ptr [edi + 6]
        call   sub_1_038E
loc_5_12CD:
        mov    dword ptr [edi + 0x37], ebx
        call   sub_1_043A
loc_5_12D5:
        call   sub_5_0EE1
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_12EB
        mov    eax, dword ptr [edi + 0x4b]
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
loc_5_12EB:
        mov    eax, dword ptr [esp]
        test   eax, 5
        je     loc_5_1343
        push   eax
        test   eax, 4
        je     loc_5_131C
        mov    eax, dword ptr [edi + 0x2b]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_5_1316
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_5_131C
loc_5_1316:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_5_131C:
        pop    eax
        test   eax, 1
        je     loc_5_1343
        mov    eax, dword ptr [edi + 0x2f]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_5_133D
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_5_1343
loc_5_133D:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_5_1343:
        clc    
loc_5_1344:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_5_1348:
        mov    eax, 0x400
        clc    
        ret    
sub_5_134F:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0154
loc_5_135D:
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     loc_5_136C
        mov    eax, dword ptr [edi + 0x80]
loc_5_136C:
        pop    edi
        pop    ebp
        ret    
sub_5_136F:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0154
loc_5_137D:
        add    esp, 8
        or     edi, edi
        je     loc_5_138D
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0x80], eax
loc_5_138D:
        pop    edi
        pop    ebp
        ret    
AUDDRV_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 4
        jae    loc_5_13BA
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj6:0x0170
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_5_13BA
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_5_13BA
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_13BA:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_13C0:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_0154
loc_5_13CC:
        add    esp, 8
        jb     loc_5_13F2
        movzx  eax, word ptr [ebp + 0x10]
        test   eax, 0xffffffff
        jne    loc_5_13F2
        mov    eax, dword ptr [edi + 0x4b]
        or     eax, eax
        je     loc_5_13F2
        push   ecx
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        mov    word ptr [ebp + 0x1c], cx
        pop    ecx
        clc    
        jmp    loc_5_13F9
loc_5_13F2:
        mov    word ptr [ebp + 0x1c], 0xffff
        stc    
loc_5_13F9:
        pop    edi
        ret    
loc_5_13FB:
        call   sub_5_1348
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_5_1406:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1448
        test   eax, 1
        je     loc_5_142D
        push   2
        push   edx
        call   sub_1_0154
loc_5_1421:
        add    esp, 8
        or     edi, edi
        je     loc_5_1448
        mov    ecx, dword ptr [edi + 0x37]
        jmp    loc_5_1450
loc_5_142D:
        test   eax, 2
        je     loc_5_1448
        push   3
        push   edx
        call   sub_1_0154
loc_5_143C:
        add    esp, 8
        or     edi, edi
        je     loc_5_1448
        mov    ecx, dword ptr [edi + 0x3f]
        jmp    loc_5_1450
loc_5_1448:
        mov    word ptr [ebp + 0x1c], 1
loc_5_144E:
        stc    
        ret    
loc_5_1450:
        mov    word ptr [ebp + 0x1c], 0
        call   sub_5_1251
        jb     loc_5_145E
        ret    
loc_5_145E:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_144E
loc_5_1466:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_14C2
        test   eax, 1
        je     loc_5_14A7
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_5_1493
        push   2
        push   edx
        call   sub_1_0154
loc_5_1489:
        add    esp, 8
        or     edi, edi
        jne    loc_5_14A2
        add    edx, 4
loc_5_1493:
        push   2
        push   edx
        call   sub_1_0154
loc_5_149B:
        add    esp, 8
        or     edi, edi
        je     loc_5_14C2
loc_5_14A2:
        mov    ecx, dword ptr [edi + 0x33]
        jmp    loc_5_14CA
loc_5_14A7:
        test   eax, 2
        je     loc_5_14C2
        push   3
        push   edx
        call   sub_1_0154
loc_5_14B6:
        add    esp, 8
        or     edi, edi
        je     loc_5_14C2
        mov    ecx, dword ptr [edi + 0x3b]
        jmp    loc_5_14CA
loc_5_14C2:
        mov    word ptr [ebp + 0x1c], 1
loc_5_14C8:
        stc    
        ret    
loc_5_14CA:
        cmp    ecx, ebx
        je     loc_5_14D6
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_14C8
loc_5_14D6:
        call   sub_5_117F
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_14E3:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 0xfffe
        jne    loc_5_1554
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 1
        je     loc_5_14FD
        mov    ecx, dword ptr [ebp + 0x18]
loc_5_14FD:
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_1554
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_1554
        mov    edi, dword ptr [eax]
        jecxz  loc_5_1525
loc_5_1514:
        cmp    dword ptr [edi + 0x53], ecx
        je     loc_5_1525
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_1554
        mov    edi, dword ptr [eax]
        jmp    loc_5_1514
loc_5_1525:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_1554
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_1554
        cmp    ecx, 0x86
        ja     loc_5_1554
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_1554:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_155D:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1592
        push   3
        push   edx
        call   sub_1_0154
loc_5_156D:
        add    esp, 8
        or     edi, edi
        je     loc_5_1592
        mov    ecx, dword ptr [edi + 0x3f]
        mov    word ptr [ebp + 0x1c], 0
        mov    eax, 2
        call   sub_5_1251
        jb     loc_5_158A
        ret    
loc_5_158A:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_1598
loc_5_1592:
        mov    word ptr [ebp + 0x1c], 1
loc_5_1598:
        stc    
        ret    
loc_5_159A:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_15D2
        push   3
        push   edx
        call   sub_1_0154
loc_5_15AA:
        add    esp, 8
        or     edi, edi
        je     loc_5_15D2
        mov    ecx, dword ptr [edi + 0x3b]
        cmp    ecx, ebx
        je     loc_5_15C0
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_15D8
loc_5_15C0:
        mov    eax, 2
        call   sub_5_117F
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_15D2:
        mov    word ptr [ebp + 0x1c], 1
loc_5_15D8:
        stc    
        ret    
loc_5_15DA:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_5_1650
        mov    ecx, dword ptr [ebp + 0x18]
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_1650
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_1650
loc_5_15FB:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x53], ecx
        je     loc_5_160C
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_1650
        jmp    loc_5_15FB
loc_5_160C:
        popfd  
        pushfd 
        cmp    word ptr [edi + 8], -1
        je     loc_5_1650
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_1650
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_1650
        cmp    ecx, 0x1c
        ja     loc_5_1650
        mov    word ptr [edi + 4], 1
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 0x53]
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_1650:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_1659:
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
        call   sub_1_0154
loc_5_16B1:
        add    esp, 8
        or     edi, edi
        je     loc_5_16DD
        lea    eax, [edi + 0x4f]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_1912
        add    esp, 0x10
        or     eax, eax
        je     loc_5_16DD
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_5_16DD:
        mov    word ptr [ebp + 0x1c], 0
        mov    word ptr [ebp + 0x14], 0
        stc    
        ret    
loc_5_16EB:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        call   sub_1_0154
loc_5_1701:
        add    esp, 8
        or     edi, edi
        je     loc_5_1717
        lea    eax, [edi + 0x4f]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_1AA2
        add    esp, 8
loc_5_1717:
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_171F:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1754
        push   4
        push   edx
        call   sub_1_0154
loc_5_172F:
        add    esp, 8
        or     edi, edi
        je     loc_5_1754
        mov    ecx, dword ptr [edi + 0x47]
        mov    word ptr [ebp + 0x1c], 0
        mov    eax, 4
        call   sub_5_1251
        jb     loc_5_174C
        ret    
loc_5_174C:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_175A
loc_5_1754:
        mov    word ptr [ebp + 0x1c], 1
loc_5_175A:
        stc    
        ret    
loc_5_175C:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1794
        push   4
        push   edx
        call   sub_1_0154
loc_5_176C:
        add    esp, 8
        or     edi, edi
        je     loc_5_1794
        mov    ecx, dword ptr [edi + 0x43]
        cmp    ecx, ebx
        je     loc_5_1782
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_179A
loc_5_1782:
        mov    eax, 4
        call   sub_5_117F
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_1794:
        mov    word ptr [ebp + 0x1c], 1
loc_5_179A:
        stc    
        ret    
loc_5_179C:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_5_1818
        mov    ecx, dword ptr [ebp + 0x18]
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_1818
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_1818
loc_5_17BD:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x53], ecx
        je     loc_5_17CE
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_1818
        jmp    loc_5_17BD
loc_5_17CE:
        popfd  
        pushfd 
        cmp    word ptr [edi + 0xa], -1
        je     loc_5_1818
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_1818
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_1818
        cmp    ecx, 0x24
        ja     loc_5_1818
        mov    word ptr [edi + 4], 0
        mov    ax, word ptr [esi + 0xa]
        mov    word ptr [edi + 6], ax
        mov    al, byte ptr [esi + 0xc]
        mov    byte ptr [edi + 8], al
        mov    eax, dword ptr [esi + 0x53]
        mov    dword ptr [edi + 0x10], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_1818:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
sub_5_1821:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x6c
        mov    eax, 0xffffffff
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     loc_5_190B
        lea    eax, [ebp - 0x6c]
        push   eax
        call   sub_5_1B36
        add    esp, 4
        call   sub_5_1B24
        call   sub_5_1B73
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x14]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1B65
        movzx  ecx, si
        shr    esi, 0x10
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_1B56
        add    esp, 8
        call   sub_5_1B30
        movzx  esi, word ptr [edi + 0x14]
        shl    esi, 0x10
        movzx  eax, word ptr [edi + 0x1c]
        or     esi, eax
        call   sub_5_1B2A
        lea    ecx, [ebp - 0x6c]
        push   ecx
        call   sub_5_1B46
        add    esp, 4
        mov    eax, esi
loc_5_190B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_1912:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    loc_5_1938
        push   0x15
        push   4
        call   sub_5_26CC
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_5_1936
        xor    eax, eax
        jmp    loc_5_1A9D
loc_5_1936:
        mov    dword ptr [esi], ebp
loc_5_1938:
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    loc_5_1955
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax + 8], 0
loc_5_1955:
        push   ebp
        call   sub_5_26BC
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_1A49
loc_5_1965:
        mov    ecx, dword ptr [esp + 0x1c]
        lea    edx, [ebx + 0xc]
loc_5_196C:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    loc_5_198C
        or     al, al
        je     loc_5_1988
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    loc_5_198C
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    loc_5_196C
loc_5_1988:
        xor    eax, eax
        jmp    loc_5_1991
loc_5_198C:
        sbb    eax, eax
        sbb    eax, -1
loc_5_1991:
        test   eax, eax
        je     loc_5_19A2
        push   ebx
        push   ebp
        call   sub_5_26AC
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_1965
loc_5_19A2:
        test   ebx, ebx
        je     loc_5_1A49
        cmp    dword ptr [ebx + 4], 0
        je     loc_5_19B7
        xor    eax, eax
        jmp    loc_5_1A9D
loc_5_19B7:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_1821
        add    esp, 0x14
        test   eax, eax
        je     loc_5_19E4
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_1AA2
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_1A9D
loc_5_19E4:
        mov    eax, dword ptr [esp + 0x20]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_1821
        add    esp, 0x14
        test   eax, eax
        je     loc_5_1A1B
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_1AA2
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_1A9D
loc_5_1A1B:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_1821
        add    esp, 0x14
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_1821
        add    esp, 0x14
        jmp    loc_5_1A9B
loc_5_1A49:
        push   ebp
        call   sub_5_2698
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_1A59
        xor    eax, eax
        jmp    loc_5_1A9D
loc_5_1A59:
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
        call   sub_5_2688
loc_5_1A9B:
        mov    eax, ebx
loc_5_1A9D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_1AA2:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        test   edi, edi
        je     loc_5_1AEE
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_5_1ACC
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_1821
        add    esp, 0x14
        mov    dword ptr [edi], 0
loc_5_1ACC:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     loc_5_1AEE
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_1821
        mov    dword ptr [edi + 4], 0
        add    esp, 0x14
loc_5_1AEE:
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_1B1F
        push   edi
        push   eax
        call   sub_5_2700
        push   edi
        push   dword ptr [esi]
        call   sub_5_26F0
        push   dword ptr [esi]
        call   sub_5_26BC
        test   eax, eax
        jne    loc_5_1B19
        push   dword ptr [esi]
        call   sub_5_26E0
loc_5_1B19:
        mov    dword ptr [esi], 0
loc_5_1B1F:
        pop    edi
        pop    esi
        ret    
        db  0CCh, 0CCh
sub_5_1B24:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083800100
sub_5_1B2A:
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
sub_5_1B30:
        VMMCall  VMM.Resume_Exec                 ; cd2085800100
sub_5_1B36:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        pop    ebp
        ret    
sub_5_1B46:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        pop    ebp
        ret    
sub_5_1B56:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048800100
sub_5_1B65:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        pop    ebp
        ret    
sub_5_1B73:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
loc_5_1B7E:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0040
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_1BB3
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_1BC1
loc_5_1BB3:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1BC1:
        pop    edi
        pop    esi
        ret    0xc
loc_5_1BC6:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        inc    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_1BD4:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        dec    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_1BE2:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        push   0xff
        push   0                             ; -> obj6:0x039C
        lea    esi, [edi + 0x104]
        push   esi
        mov    dword ptr [edi], 9
        call   sub_5_2714
        add    esp, 0xc
        call   sub_5_2180
        push   0xff
        lea    ecx, [edi + 4]
        push   eax
        push   ecx
        call   sub_5_2714
        mov    edx, dword ptr [esp + 0x18]
        add    esp, 0xc
        xor    ecx, ecx
        mov    dword ptr [edi + 0x210], ecx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 0x204], eax
        mov    dword ptr [edi + 0x20c], ecx
        mov    word ptr [edi + 0x208], 0x33f6
        mov    eax, 0xb3
        cmp    byte ptr [esi], cl
        je     loc_5_1C56
loc_5_1C4B:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_5_1C4B
loc_5_1C56:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 0x20a], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_1C6D:
        push   esi
        push   edi
        mov    edi, 0x8878000a
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_5_1C92
        push   dword ptr [esi + 4]
        call   sub_5_2264
        add    esp, 4
        test   eax, eax
        jne    loc_5_1C92
        xor    edi, edi
        inc    dword ptr [esi + 0xc]
loc_5_1C92:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_1C99:
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
loc_5_1CE4:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 0x8007000e
        mov    esi, dword ptr [esp + 0x2c]
        test   byte ptr [esp + 0x1c], 0xf0
        mov    dword ptr [esi], ebx
        je     loc_5_1D06
        mov    eax, 0x8878001e
        jmp    loc_5_1DCC
loc_5_1D06:
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_1DAB
        push   0x201
        push   0x31
        call   sub_5_2724
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_1DA4
        mov    dword ptr [ebx], 0            ; -> obj6:0x03B8
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_22A9
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1000
        jbe    loc_5_1DB0
        cmp    eax, 0xffff
        jae    loc_5_1DB0
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_5_1DB0
        cmp    dword ptr [ebx + 0x10], 0
        je     loc_5_1DB0
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_5_1DB0
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        push   ebx
        xor    edi, edi
        call   sub_5_1F24
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
        call   sub_5_22F0
        add    esp, 0x14
        jmp    loc_5_1DB0
loc_5_1DA4:
        mov    edi, 0x8007000e
        jmp    loc_5_1DB0
loc_5_1DAB:
        mov    edi, 0x8878000a
loc_5_1DB0:
        test   edi, edi
        je     loc_5_1DC5
        test   ebx, ebx
        je     loc_5_1DCA
        push   0
        push   ebx
        call   sub_5_271C
        add    esp, 8
        jmp    loc_5_1DCA
loc_5_1DC5:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 0x10]
loc_5_1DCA:
        mov    eax, edi
loc_5_1DCC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
loc_5_1DD3:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        mov    eax, 0x80004001
        ret    0xc
loc_5_1DE5:
        mov    eax, 0x80004005
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0xc]
        dec    edx
        mov    dword ptr [ecx + 0xc], edx
        jne    loc_5_1E0A
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_5_1E0A
        push   dword ptr [ecx + 4]
        call   sub_5_2244
        add    esp, 4
        xor    eax, eax
loc_5_1E0A:
        ret    4
sub_5_1E0D:
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
        call   sub_5_2724
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_5_1E8C
        mov    edi, dword ptr [ebp + 8]
        push   1
        push   0                             ; -> obj6:0x03A8
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi], 0            ; -> obj6:0x0378
        call   sub_5_2740
        test   eax, eax
        jne    loc_5_1E8C
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
        jne    loc_5_1E8C
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_136F
        add    esp, 8
loc_5_1E8C:
        test   ebx, ebx
        jne    loc_5_1EB5
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_1EA6
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_272C
loc_5_1EA6:
        test   esi, esi
        je     loc_5_1EB5
        push   0
        push   esi
        call   sub_5_271C
        add    esp, 8
loc_5_1EB5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_1EBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_134F
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_5_1F1E
        push   0
        push   edi
        call   sub_5_136F
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
        call   sub_5_272C
        push   0
        push   esi
        call   sub_5_271C
        add    esp, 8
loc_5_1F1E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_1F24:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [edx], eax
        jbe    loc_5_1F36
        mov    dword ptr [edx], eax
loc_5_1F36:
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
loc_5_1F56:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x20]
        dec    esi
        mov    dword ptr [eax + 0x20], esi
        jne    loc_5_1F7B
        cmp    dword ptr [eax + 8], 0
        jne    loc_5_1F7B
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 0x10]
        call   sub_5_271C
        add    esp, 8
loc_5_1F7B:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_1F81:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0050
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_1FB6
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_1FC4
loc_5_1FB6:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1FC4:
        pop    edi
        pop    esi
        ret    0xc
loc_5_1FC9:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x20]
        inc    eax
        mov    dword ptr [ecx + 0x20], eax
        ret    4
loc_5_1FD7:
        mov    eax, 0x80004001
        ret    0x20
loc_5_1FDF:
        mov    eax, 0x80004001
        ret    0x14
loc_5_1FE7:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_5_2040
        cmp    word ptr [eax], 1
        jne    loc_5_2040
        mov    edx, dword ptr [eax + 4]
        cmp    edx, 0xafc8
        jae    loc_5_2040
        mov    cx, word ptr [eax + 0xe]
        cmp    cx, 8
        je     loc_5_2010
        cmp    cx, 0x10
        jne    loc_5_2040
loc_5_2010:
        mov    ax, word ptr [eax + 2]
        cmp    ax, 1
        je     loc_5_2020
        cmp    ax, 2
        jne    loc_5_2040
loc_5_2020:
        movzx  eax, ax
        movzx  ecx, cx
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 0xc]
        push   edx
        movzx  ecx, word ptr [eax + 0x2e]
        push   1
        push   ecx
        call   sub_5_22F0
        add    esp, 0x14
        xor    eax, eax
        jmp    loc_5_2045
loc_5_2040:
        mov    eax, 0x88780064
loc_5_2045:
        ret    8
loc_5_2048:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_5_206D
        cmp    eax, 0xafc8
        jae    loc_5_206D
        mov    ecx, dword ptr [esp + 4]
        push   eax
        movzx  edx, word ptr [ecx + 0x2e]
        push   edx
        call   sub_5_2364
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_2072
loc_5_206D:
        mov    eax, 0x88780064
loc_5_2072:
        ret    8
loc_5_2075:
        mov    eax, 0x80004001
        ret    8
loc_5_207D:
        mov    eax, 0x80004001
        ret    8
loc_5_2085:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    loc_5_20B7
        push   dword ptr [esi + 0x18]
        call   sub_5_218E
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 0x30], 3
        jbe    loc_5_20A4
        add    eax, eax
loc_5_20A4:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_5_20CB
loc_5_20B7:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_20CB:
        xor    eax, eax
        pop    esi
        ret    0xc
loc_5_20D1:
        push   esi
        mov    eax, 0x80004005
        mov    esi, dword ptr [esp + 8]
        push   edi
        cmp    dword ptr [esi + 8], 0
        jne    loc_5_2146
        push   dword ptr [esi + 0x18]
        movzx  edi, word ptr [esi + 0x2e]
        call   sub_5_21A9
        add    esp, 4
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 0x18]
        call   sub_5_21CD
        add    esp, 0xc
        push   dword ptr [esi + 0x18]
        call   sub_5_21EE
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_21B7
        mov    ax, word ptr [esi + 0x1c]
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0x10
        jne    loc_5_2125
        shr    ax, 1
loc_5_2125:
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_5_212E
        shr    ax, 1
loc_5_212E:
        dec    ax
        movzx  ecx, ax
        push   ecx
        push   edi
        call   sub_5_23C8
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_2146:
        pop    edi
        pop    esi
        ret    0x10
loc_5_214B:
        push   esi
        mov    eax, 0x80004005
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 0
        je     loc_5_217C
        movzx  eax, word ptr [esi + 0x2e]
        push   eax
        call   sub_5_25A6
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_21A9
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_217C:
        pop    esi
        ret    4
sub_5_2180:
        movzx  eax, word ptr [0x12]          ; -> loc_2_0012
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_5_218E:
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
sub_5_21A9:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
        pop    ebp
        ret    
sub_5_21B7:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Phys_Unmask_Channel ; cd2019000400
        pop    ebx
        pop    ebp
        ret    
sub_5_21CD:
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
sub_5_21EE:
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
        call   sub_1_0370
loc_5_2224:
        xor    eax, eax
        jae    loc_5_222D
        mov    eax, 1
loc_5_222D:
        pop    edx
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        call   sub_1_0354
loc_5_223F:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret    
sub_5_2244:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_0154
loc_5_2254:
        add    esp, 8
        mov    word ptr [edi + 0x84], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret    
sub_5_2264:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0xffff
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_0154
loc_5_227A:
        add    esp, 8
        mov    eax, dword ptr [edi + 0x33]
        or     eax, eax
        je     loc_5_2290
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        je     loc_5_2290
        jmp    loc_5_22A2
loc_5_2290:
        mov    word ptr [edi + 0x84], 1
        movzx  edx, word ptr [edi + 6]
        mov    edx, 0
loc_5_22A2:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret    
sub_5_22A9:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   eax
        call   sub_1_0154
loc_5_22BC:
        add    esp, 8
        mov    ax, word ptr [edi + 6]
        mov    word ptr [esi + 0x2e], ax
        mov    ah, byte ptr [edi + 0xe]
        mov    byte ptr [esi + 0x30], ah
        mov    eax, dword ptr [edi + 0x1b]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [edi + 0x1f]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [edi + 0x23]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [edi + 0x4b]
        mov    dword ptr [esi + 0x18], eax
        xor    ax, ax
        mov    word ptr [esi + 0x2c], ax
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_5_22F0:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edi
        cmp    word ptr [ebp + 0xc], 1
        jne    loc_5_235F
        cmp    word ptr [ebp + 0x10], 0xfa0
        jb     loc_5_235F
        cmp    word ptr [ebp + 0x10], 0xafc8
        ja     loc_5_235F
        cmp    word ptr [ebp + 0x14], 8
        je     loc_5_231A
        cmp    word ptr [ebp + 0x14], 0x10
        jne    loc_5_235F
loc_5_231A:
        cmp    word ptr [ebp + 0x18], 1
        jb     loc_5_235F
        cmp    word ptr [ebp + 0x18], 2
        ja     loc_5_235F
        and    word ptr [0], 0xfff9          ; -> obj6:0x03E8
        cmp    word ptr [ebp + 0x14], 0x10
        jne    loc_5_233F
        or     word ptr [0], 2               ; -> obj6:0x03E8
loc_5_233F:
        cmp    word ptr [ebp + 0x18], 2
        jne    loc_5_234E
        or     word ptr [0], 4               ; -> obj6:0x03E8
loc_5_234E:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_5_2364
        add    esp, 8
        clc    
        jmp    loc_5_2360
loc_5_235F:
        stc    
loc_5_2360:
        pop    edi
        pop    eax
        pop    ebp
        ret    
sub_5_2364:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0xc6
        call   sub_1_0370
loc_5_2375:
        jb     loc_5_23C1
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
        call   sub_1_0370
loc_5_2396:
        pop    eax
        jb     loc_5_23C1
        call   sub_1_0370
loc_5_239E:
        jb     loc_5_23C1
        push   edx
        mov    dx, 3
        mov    ax, 0xb9e
        div    bx
        neg    al
        pop    edx
        push   eax
        mov    al, 0xa2
        call   sub_1_0370
loc_5_23B7:
        pop    eax
        jb     loc_5_23C1
        call   sub_1_0370
loc_5_23BF:
        jmp    loc_5_23C2
loc_5_23C1:
        stc    
loc_5_23C2:
        pop    edi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_5_23C8:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0xd1
        call   sub_1_0370
loc_5_23D8:
        jb     loc_5_25A0
        add    edx, 6
        mov    al, 2
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    loc_5_23EC
loc_5_23EC:
        jmp    loc_5_23EE
loc_5_23EE:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        mov    al, 0xc6
        call   sub_1_0370
loc_5_23FE:
        jb     loc_5_25A0
        mov    al, 0xa4
        call   sub_1_0370
loc_5_240B:
        jb     loc_5_25A0
        mov    eax, dword ptr [ebp + 0xc]
        neg    ax
        call   sub_1_0370
loc_5_241C:
        jb     loc_5_25A0
        push   eax
        mov    al, 0xa5
        call   sub_1_0370
loc_5_242A:
        pop    eax
        jb     loc_5_25A0
        mov    al, ah
        call   sub_1_0370
loc_5_2438:
        jb     loc_5_25A0
        mov    al, 0xa8
        call   sub_1_0370
loc_5_2445:
        jb     loc_5_25A0
        mov    al, 0xf5
        test   word ptr [0], 4               ; -> obj6:0x03E8
        jne    loc_5_245A
        mov    al, 0xf6
loc_5_245A:
        call   sub_1_0370
loc_5_245F:
        jb     loc_5_25A0
        mov    al, 0xc0
        call   sub_1_0370
loc_5_246C:
        jb     loc_5_25A0
        mov    al, 0xb1
        call   sub_1_0370
loc_5_2479:
        jb     loc_5_25A0
        call   sub_1_0354
loc_5_2484:
        jb     loc_5_25A0
        and    al, 0xf
        or     al, 0x50
        push   eax
        mov    al, 0xb1
        call   sub_1_0370
loc_5_2496:
        pop    eax
        jb     loc_5_25A0
        call   sub_1_0370
loc_5_24A2:
        jb     loc_5_25A0
        mov    al, 0xc0
        call   sub_1_0370
loc_5_24AF:
        jb     loc_5_25A0
        mov    al, 0xb2
        call   sub_1_0370
loc_5_24BC:
        jb     loc_5_25A0
        call   sub_1_0354
loc_5_24C7:
        jb     loc_5_25A0
        and    al, 0xf
        or     al, 0x50
        push   eax
        mov    al, 0xb2
        call   sub_1_0370
loc_5_24D9:
        pop    eax
        jb     loc_5_25A0
        call   sub_1_0370
loc_5_24E5:
        jb     loc_5_25A0
        test   word ptr [0], 2               ; -> obj6:0x03E8
        je     loc_5_2545
        mov    al, 0xb6
        call   sub_1_0370
loc_5_24FD:
        jb     loc_5_25A0
        xor    al, al
        call   sub_1_0370
loc_5_250A:
        jb     loc_5_25A0
        mov    al, 0xb7
        call   sub_1_0370
loc_5_2517:
        jb     loc_5_25A0
        mov    al, 0x71
        call   sub_1_0370
loc_5_2524:
        jb     loc_5_25A0
        mov    al, 0xb7
        call   sub_1_0370
loc_5_252D:
        jb     loc_5_25A0
        mov    al, 0xbc
        test   word ptr [0], 4               ; -> obj6:0x03E8
        jne    loc_5_253E
        mov    al, 0xf4
loc_5_253E:
        call   sub_1_0370
loc_5_2543:
        jb     loc_5_25A0
loc_5_2545:
        mov    al, 0xb7
        call   sub_1_0370
loc_5_254C:
        jb     loc_5_25A0
        mov    al, 0xd0
        test   word ptr [0], 4               ; -> obj6:0x03E8
        je     loc_5_255D
        mov    al, 0x98
loc_5_255D:
        test   word ptr [0], 2               ; -> obj6:0x03E8
        je     loc_5_256A
        or     al, 0x24
loc_5_256A:
        call   sub_1_0370
loc_5_256F:
        jb     loc_5_25A0
        mov    al, 0xc0
        call   sub_1_0370
loc_5_2578:
        jb     loc_5_25A0
        mov    al, 0xb8
        call   sub_1_0370
loc_5_2581:
        jb     loc_5_25A0
        call   sub_1_0354
loc_5_2588:
        jb     loc_5_25A0
        and    al, 0x30
        or     al, 5
        push   eax
        mov    al, 0xb8
        call   sub_1_0370
loc_5_2596:
        pop    eax
        jb     loc_5_25A0
        call   sub_1_0370
loc_5_259E:
        jmp    loc_5_25A1
loc_5_25A0:
        stc    
loc_5_25A1:
        pop    edi
        pop    edx
        pop    eax
        pop    ebp
        ret    
sub_5_25A6:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0xd3
        call   sub_1_0370
loc_5_25B6:
        jb     loc_5_25EE
        mov    al, 0xc0
        call   sub_1_0370
loc_5_25BF:
        jb     loc_5_25EE
        mov    al, 0xb8
        call   sub_1_0370
loc_5_25C8:
        jb     loc_5_25EE
        call   sub_1_0354
loc_5_25CF:
        jb     loc_5_25EE
        and    al, 0x30
        push   eax
        mov    al, 0xb8
        call   sub_1_0370
loc_5_25DB:
        pop    eax
        jb     loc_5_25EE
        call   sub_1_0370
loc_5_25E3:
        jmp    loc_5_25EF
        db  0B0h, 0D0h, 0E8h
        dd  sub_1_0370               ; fixup
        db  0EBh, 001h
loc_5_25EE:
        stc    
loc_5_25EF:
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
        call   sub_5_23C8
        add    esp, 0xc
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_25A6
        add    esp, 8
        pop    ebp
        ret    
        db  050h, 052h, 0C1h, 0E8h, 008h, 024h, 0F0h, 0C1h, 0EBh, 00Ch, 00Ah, 0C3h, 083h, 0C2h, 004h, 050h
        db  0B0h, 014h, 0EEh, 058h, 066h, 042h, 0EEh, 0F8h, 05Ah, 058h, 0C3h
sub_5_2638:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_5_2640:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        int3   
        int3   
sub_5_2648:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        int3   
        int3   
sub_5_2650:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        int3   
        int3   
sub_5_2658:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_5_2660:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_5_2668:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_5_2670:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_5_2678:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_5_2680:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_5_2688:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2698:
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
sub_5_26AC:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_26BC:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_26CC:
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
sub_5_26E0:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_26F0:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2700:
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
sub_5_2714:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_5_271C:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_5_2724:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_5_272C:
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
sub_5_2740:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_5_275E
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_275E:
        ret    0x10
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x3ea  RW-
        dd  obj6:0x0020              ; fixup
        dd  obj6:0x0014              ; fixup
        dd  obj6:0x000C              ; fixup
        db  044h, 065h, 066h, 061h, 075h, 06Ch, 074h, 000h, 044h, 065h, 074h, 065h, 063h, 074h, 065h, 064h
        db  000h, 000h, 000h, 000h, 046h, 06Fh, 072h, 063h, 065h, 064h, 000h, 000h, 043h, 06Fh, 06Eh, 066h
        db  069h, 067h, 05Ch, 044h, 04Dh, 041h, 020h, 053h, 065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh
        db  073h, 000h, 000h, 000h, 044h, 04Dh, 041h, 020h, 053h, 065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh
        db  06Eh, 073h, 000h, 000h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 05Ch, 049h, 052h, 051h, 020h, 053h
        db  065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 000h, 000h, 049h, 052h, 051h, 020h
        db  053h, 065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 000h, 043h, 06Fh, 06Eh, 066h
        db  069h, 067h, 05Ch, 04Dh, 050h, 055h, 034h, 030h, 031h, 020h, 049h, 052h, 051h, 020h, 053h, 065h
        db  06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 000h, 000h, 000h, 04Dh, 050h, 055h, 034h
        db  030h, 031h, 020h, 049h, 052h, 051h, 020h, 053h, 065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh
        db  073h, 000h, 000h, 000h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 05Ch, 04Dh, 050h, 055h, 034h, 030h
        db  031h, 020h, 050h, 06Fh, 072h, 074h, 020h, 053h, 065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh
        db  073h, 000h, 000h, 000h, 04Dh, 050h, 055h, 034h, 030h, 031h, 020h, 050h, 06Fh, 072h, 074h, 020h
        db  053h, 065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 000h, 05Ch, 043h, 06Fh, 06Eh
        db  066h, 069h, 067h, 000h, 041h, 055h, 044h, 044h, 052h, 056h, 03Ah, 020h, 000h, 000h, 000h, 000h
        db  042h, 04Ch, 041h, 053h, 054h, 045h, 052h, 000h, 041h, 025h, 030h, 033h, 058h, 020h, 049h, 025h
        db  064h, 020h, 044h, 025h, 064h, 000h, 000h, 000h, 06Dh, 069h, 064h, 069h, 05Ch, 065h, 073h, 073h
        db  06Dh, 070h, 075h, 02Eh, 064h, 072h, 076h, 000h, 06Dh, 069h, 064h, 069h, 05Ch, 065h, 073h, 073h
        db  066h, 06Dh, 02Eh, 064h, 072h, 076h, 000h, 000h
        dd  loc_5_13FB               ; fixup
        dd  loc_5_14E3               ; fixup
        dd  loc_5_1406               ; fixup
        dd  loc_5_1466               ; fixup
        dd  loc_5_13C0               ; fixup
        dd  loc_5_13FB               ; fixup
        dd  loc_5_15DA               ; fixup
        dd  loc_5_155D               ; fixup
        dd  loc_5_159A               ; fixup
        dd  loc_5_1659               ; fixup
        dd  loc_5_16EB               ; fixup
        dd  loc_5_13FB               ; fixup
        dd  loc_5_179C               ; fixup
        dd  loc_5_171F               ; fixup
        dd  loc_5_175C               ; fixup
        db  005h, 000h, 000h, 000h
        dd  obj6:0x0134              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj6:0x0148              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj6:0x0158              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj6:0x0160              ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_5_0DDC               ; fixup
        db  001h, 000h
        dd  loc_5_0DDC               ; fixup
        db  002h, 000h
        dd  loc_5_0DDC               ; fixup
        db  003h, 000h
        dd  loc_5_0DDC               ; fixup
        db  004h, 000h
        dd  loc_5_0DDC               ; fixup
        db  005h, 000h
        dd  loc_5_0DDC               ; fixup
        db  006h, 000h
        dd  loc_5_0DDC               ; fixup
        db  007h, 000h
        dd  loc_5_0DDC               ; fixup
        db  008h, 000h
        dd  loc_5_0DDC               ; fixup
        db  009h, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Ah, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Bh, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Ch, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Dh, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Eh, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Fh, 000h
        dd  loc_5_0DDC               ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_5_0E64               ; fixup
        db  001h, 000h
        dd  loc_5_0E64               ; fixup
        db  002h, 000h
        dd  loc_5_0E64               ; fixup
        db  003h, 000h
        dd  loc_5_0E64               ; fixup
        db  004h, 000h
        dd  loc_5_0DDC               ; fixup
        db  005h, 000h
        dd  loc_5_0DDC               ; fixup
        db  006h, 000h
        dd  loc_5_0DDC               ; fixup
        db  007h, 000h
        dd  loc_5_0DDC               ; fixup
        db  008h, 000h
        dd  loc_5_0E64               ; fixup
        db  009h, 000h
        dd  loc_5_0E64               ; fixup
        db  00Ah, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Bh, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Ch, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Dh, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Eh, 000h
        dd  loc_5_0DDC               ; fixup
        db  00Fh, 000h
        dd  loc_5_0DDC               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_5_0E64               ; fixup
        db  001h, 000h
        dd  loc_5_0E64               ; fixup
        db  002h, 000h
        dd  loc_5_0E64               ; fixup
        db  003h, 000h
        dd  loc_5_0E64               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_5_0E20               ; fixup
        db  001h, 000h
        dd  loc_5_0E20               ; fixup
        db  000h, 000h, 004h, 000h
        dd  loc_1_025C               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0290               ; fixup
        dd  loc_1_02A0               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h
        dd  loc_1_02D4               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0310               ; fixup
        dd  loc_1_0320               ; fixup
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
        db  020h, 062h, 075h, 066h, 066h, 065h, 072h, 00Ah, 000h, 000h, 000h, 000h, 087h, 083h, 081h, 082h
        db  000h, 08Bh, 089h, 08Ah, 000h, 000h, 000h, 000h
        dd  loc_5_1B7E               ; fixup
        dd  loc_5_1BC6               ; fixup
        dd  loc_5_1BD4               ; fixup
        dd  loc_5_1BE2               ; fixup
        dd  loc_5_1C6D               ; fixup
        dd  loc_5_1DE5               ; fixup
        dd  loc_5_1C99               ; fixup
        dd  loc_5_1CE4               ; fixup
        dd  loc_5_1DD3               ; fixup
        db  045h, 053h, 031h, 036h, 038h, 038h, 02Eh, 056h, 058h, 044h, 000h, 000h, 064h, 073h, 06Fh, 075h
        db  06Eh, 064h, 02Eh, 076h, 078h, 064h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_5_1F81               ; fixup
        dd  loc_5_1FC9               ; fixup
        dd  loc_5_1F56               ; fixup
        dd  loc_5_1FD7               ; fixup
        dd  loc_5_1FDF               ; fixup
        dd  loc_5_1FE7               ; fixup
        dd  loc_5_2048               ; fixup
        dd  loc_5_2075               ; fixup
        dd  loc_5_207D               ; fixup
        dd  loc_5_2085               ; fixup
        dd  loc_5_20D1               ; fixup
        dd  loc_5_214B               ; fixup
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
        mov    esi, 0                        ; -> loc_1_01F4
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        clc    
        ret    
loc_7_0070:
        pop    ebx
        jmp    loc_7_0011
VxD__LTEXT ENDS

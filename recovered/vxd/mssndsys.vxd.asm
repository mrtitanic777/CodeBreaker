; mssndsys -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 6953 instructions recovered by recursive descent

Declare_Virtual_Device mssndsys, 4, 0, mssndsys_Control, 0045Fh, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x13ac  R-X
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
        dw  0045Fh                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'mssndsys'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  mssndsys_Control         ; DDB_Control_Proc
        dd  mssndsys_PM_API          ; DDB_V86_API_Proc
        dd  mssndsys_PM_API          ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00DC:
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0115:
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x3f9f9f9f], bl
        aas    
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cld    
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
loc_1_0136:
        add    dword ptr [edi], ecx
        add    byte ptr [esi], cl
        add    eax, dword ptr [edx]
        add    eax, 0xd060407
        or     dword ptr [ebx], ecx
        db  00Ch
loc_1_0144:
        mov    cl, 0x17
        add    byte ptr [eax], al
        mov    ds, word ptr [eax + eax]
        add    byte ptr [eax + 0x22], ah
        add    byte ptr [eax], al
        dec    eax
        sub    byte ptr [eax], al
        add    al, cl
        xor    al, 0
        add    byte ptr [edx], ch
        inc    esp
        add    byte ptr [eax], al
        sti    
        dec    edi
        add    byte ptr [eax], al
        lahf   
        pushal 
        add    byte ptr [eax], al
        push   cs
        je     loc_1_0167
loc_1_0167:
        add    byte ptr [ecx], bl
        jg     loc_1_016B
loc_1_016B:
        add    byte ptr [ebp - 0x76], ch
        add    byte ptr [eax], al
        neg    byte ptr [edi - 0x4c1e0000]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_017C:
        xchg   byte ptr [0x19dc0000], dl
        add    byte ptr [eax], al
        inc    eax
        pop    ds
        add    byte ptr [eax], al
        and    byte ptr [0x2b110000], 0
        add    byte ptr [eax - 0x2bffffc2], al
        dec    ecx
        add    byte ptr [eax], al
        and    dl, byte ptr [esi]
        add    byte ptr [ebx + ebp*2], bl
        add    byte ptr [eax], al
        add    byte ptr [ebp], bh
        add    byte ptr [ebx], dh
        add    dword ptr [eax], 0x93a800
        db  000h
loc_1_01AC:
        inc    esp
        lodsb  al, byte ptr [esi]
        add    byte ptr [eax], al
sub_1_01B0:
        cmp    byte ptr [ebx + 0x140000], 0
        add    byte ptr [eax], al            ; -> loc_1_0D53
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_1050
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    dword ptr [eax], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    dword ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        aam    0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fiadd  dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loop   loc_1_0206
loc_1_0206:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        in     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        call   sub_1_124C
loc_1_0219:
        add    byte ptr [eax], al
        add    al, ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_1189
        add    byte ptr [eax], al
        add    cl, ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_125C
        add    byte ptr [eax], al
        add    dl, dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_1284
        add    byte ptr [eax], al
        db  000h
mssndsys_Control:
        cmp    eax, 8
        je     loc_5_1125
loc_1_023D:
        cmp    eax, 0xb
        je     loc_5_10BF
loc_1_0246:
        cmp    eax, 3
        je     loc_5_1125
loc_1_024F:
        cmp    eax, 0x1b
        je     loc_7_0000
loc_1_0258:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_0261:
        cmp    eax, 0x22
        je     loc_4_0B1A
loc_1_026A:
        clc    
        ret    
sub_1_026C:
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
        je     loc_1_02C3
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_02C3
loc_1_029F:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0xc], 1
        jne    loc_1_02B3
        cmp    dword ptr [edx + edi], ebx
        jne    loc_1_02B9
loc_1_02AF:
        popfd  
        clc    
        jmp    loc_1_02C7
loc_1_02B3:
        cmp    word ptr [edx + edi], bx
        je     loc_1_02AF
loc_1_02B9:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_02C3
        jmp    loc_1_029F
loc_1_02C3:
        popfd  
        xor    edi, edi
        stc    
loc_1_02C7:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
        db  0B8h, 000h, 004h, 000h, 000h, 0C3h
sub_1_02D3:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     loc_1_02F0
        mov    eax, dword ptr [edi + 0xb4]
loc_1_02F0:
        pop    edi
        pop    ebp
        ret    
sub_1_02F3:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
        add    esp, 8
        or     edi, edi
        je     loc_1_0311
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0xb4], eax
loc_1_0311:
        pop    edi
        pop    ebp
        ret    
sub_1_0314:
        push   eax
        push   ebx
        pushfd 
        add    ebx, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_0341
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_0341
loc_1_032C:
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_0337
        mov    esi, eax
        popfd  
        clc    
        jmp    loc_1_0345
loc_1_0337:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_0341
        jmp    loc_1_032C
loc_1_0341:
        popfd  
        xor    esi, esi
        stc    
loc_1_0345:
        pop    ebx
        pop    eax
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_00D8               ; fixup
loc_1_0350:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_039F
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_1_039F
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_039F
loc_1_0371:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x24], ebx
        jne    loc_1_037D
        or     ecx, 1
loc_1_037D:
        cmp    dword ptr [edi + 0x30], ebx
        jne    loc_1_0385
        or     ecx, 2
loc_1_0385:
        jecxz  loc_1_0390
        push   eax
        mov    eax, ecx
        call   sub_5_119E
loc_1_038F:
        pop    eax
loc_1_0390:
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_039F
        jmp    loc_1_0371
loc_1_039F:
        popfd  
        popal  
        jmp    loc_1_00D8
        db  090h
loc_1_03A8:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_03EC
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        and    dl, 0x3f
        push   edi
        push   2
        push   eax
        call   sub_1_026C
        add    esp, 8
        or     edi, edi
        jne    loc_1_03D4
        push   3
        push   eax
        call   sub_1_026C
        add    esp, 8
loc_1_03D4:
        or     edi, edi
        je     loc_1_03E3
        test   word ptr [edi + 4], 4
        je     loc_1_03E3
        or     dl, 0x40
loc_1_03E3:
        pop    edi
        xor    dh, dh
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
loc_1_03EC:
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012000400
        ret    
        db  090h
loc_1_03F4:
        push   eax
        push   edi
        push   edx
        mov    edi, edx
        mov    eax, dword ptr [edi + 0x24]
        or     eax, eax
        je     loc_1_0487
        cmp    word ptr [edi + 0xb8], 0
        jne    loc_1_0487
        mov    ebx, eax
        test   word ptr [edi + 0x52], 8
        je     loc_1_0465
        movzx  edx, word ptr [edi + 0x22]
        in     al, dx
        test   al, 0x80
        je     loc_1_0465
        push   ebx
        mov    ebx, dword ptr [edi + 0x30]
        or     ebx, ebx
        je     loc_1_044E
        test   word ptr [edi + 4], 0x2000
        jne    loc_1_044E
        mov    byte ptr [edi + 0xac], al
        or     word ptr [edi + 4], 0x2000
        push   edx
        push   esi
        mov    esi, 0                        ; -> loc_5_0E9A
        mov    edx, edi
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        pop    edx
loc_1_044E:
        pop    ebx
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_04B3
        push   ebx
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0937
        pop    ebx
loc_1_0465:
        test   word ptr [edi + 4], 0x200
        je     loc_1_047C
        test   word ptr [edi + 4], 0x4000
        je     loc_1_047C
        call   sub_1_0954
        jb     loc_1_04A6
loc_1_047C:
        pop    edx
        pop    edi
        pop    eax
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        clc    
        ret    
loc_1_0487:
        test   word ptr [edi + 0x52], 8
        je     loc_1_04A6
        movzx  edx, word ptr [edi + 0x22]
        in     al, dx
        test   al, 0x80
        je     loc_1_04A6
        push   ebx
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0937
        pop    ebx
loc_1_04A6:
        movzx  edx, word ptr [edi + 0x42]
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_1_04B3:
        pop    edx
        pop    edi
        pop    eax
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_04C0:
        push   eax
        push   edi
        push   edx
        mov    edi, edx
        test   word ptr [edi + 4], 2
        jne    loc_1_04E1
        push   esi
        or     word ptr [edi + 4], 2
        mov    esi, 0                        ; -> loc_1_0534
        mov    edx, edi
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
loc_1_04E1:
        movzx  edx, word ptr [edi + 0x42]
        add    edx, 0xa
        in     al, dx
        test   al, 0x40
        je     loc_1_051B
        mov    eax, dword ptr [edi + 0x24]
        or     eax, eax
        je     loc_1_050E
        mov    ebx, eax
        test   word ptr [edi + 4], 0x4000
        je     loc_1_0505
        call   sub_1_0954
        jb     loc_1_050E
loc_1_0505:
        mov    eax, dword ptr [edi + 0x2c]
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_050E:
        movzx  edx, word ptr [edi + 0x42]
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_1_051B:
        movzx  edx, word ptr [edi + 0x48]
        in     al, dx
        test   al, 0x20
        je     loc_1_0529
        pop    edx
        pop    edi
        pop    eax
        stc    
        ret    
loc_1_0529:
        pop    edx
        pop    edi
        pop    eax
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
loc_1_0534:
        pushfd 
        cli    
        mov    edi, edx
        and    word ptr [edi + 4], 0xfffd
        mov    eax, dword ptr [edi + 0x2c]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0xa1
        jne    loc_1_055A
        movzx  edx, word ptr [edi + 0x42]
        add    edx, 0xa
        in     al, dx
        test   al, 0x40
        jne    loc_1_0563
loc_1_055A:
        movzx  edx, word ptr [edi + 0x48]
        in     al, dx
        test   al, 0x20
        je     loc_1_0570
loc_1_0563:
        movzx  edx, word ptr [edi + 0x42]
        add    edx, 0xa
        mov    al, 8
        out    dx, al
        xor    al, al
        out    dx, al
loc_1_0570:
        popfd  
        ret    
        db  08Bh, 0C0h
loc_1_0574:
        push   ecx
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 8
        je     loc_1_058F
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_1_058F:
        pop    ecx
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_0594:
        push   edi
        push   1
        push   eax
        call   sub_1_026C
        add    esp, 8
        or     edi, edi
        je     loc_1_05C3
        test   word ptr [edi + 4], 8
        jne    loc_1_05CD
        cmp    dword ptr [edi + 0x24], ebx
        jne    loc_1_05C3
        jecxz  loc_1_05BB
loc_1_05B3:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_05CD
loc_1_05BB:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_05CD
loc_1_05C3:
        test   word ptr [edi + 4], 0x8000
        jne    loc_1_05BB
        je     loc_1_05B3
loc_1_05CD:
        pop    edi
        clc    
        ret    
sub_1_05D0:
        push   eax
        push   ecx
        push   edx
        add    dx, 0
        mov    ecx, 0x20000
        in     al, dx
loc_1_05DD:
        in     al, dx
        or     al, al
        jns    loc_1_05E5
        loop   loc_1_05DD
        stc    
loc_1_05E5:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_05E9:
        call   sub_1_05D0
        jb     loc_1_0606
        pushfd 
        cli    
        push   edx
        add    dx, 0
        mov    al, ah
        or     al, byte ptr [0]              ; -> obj1:0x0114
        out    dx, al
        inc    dx
        in     al, dx
        pop    edx
        popfd  
        clc    
loc_1_0606:
        ret    
sub_1_0607:
        call   sub_1_05D0
        jb     loc_1_0628
        pushfd 
        cli    
        push   eax
        push   edx
        add    dx, 0
        xchg   al, ah
        or     al, byte ptr [0]              ; -> obj1:0x0114
        out    dx, al
        inc    dx
        mov    al, ah
        out    dx, al
        pop    edx
        pop    eax
        popfd  
        clc    
loc_1_0628:
        ret    
sub_1_0629:
        push   edx
        push   ecx
        push   eax
        pushfd 
        cli    
        cmp    word ptr [edi + 0x4c], 0
        jne    loc_1_0680
        mov    ecx, 6
        push   esi
        xor    esi, esi
loc_1_063D:
        mov    eax, esi
        mov    ah, al
        call   sub_1_05E9
        mov    byte ptr [esi], al            ; -> obj1:0x0126
        or     al, 0x80
        call   sub_1_0607
        inc    esi
        loop   loc_1_063D
        mov    ecx, 2
loc_1_065B:
        mov    eax, esi
        mov    ah, al
        call   sub_1_05E9
        mov    byte ptr [esi], al            ; -> obj1:0x0126
        push   eax
        and    al, 0xf
        cmp    al, 0xd
        pop    eax
        jbe    loc_1_0676
        mov    al, 0xd
        jmp    loc_1_0678
loc_1_0676:
        and    al, 0xf0
loc_1_0678:
        call   sub_1_0607
        loop   loc_1_065B
        pop    esi
loc_1_0680:
        mov    al, byte ptr [0]              ; -> obj1:0x0114
        or     al, 0x40
        mov    byte ptr [0], al              ; -> obj1:0x0114
        call   sub_1_05D0
        jb     loc_1_069C
        add    dx, 0
        out    dx, al
        popfd  
        clc    
        jmp    loc_1_069E
loc_1_069C:
        popfd  
        stc    
loc_1_069E:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_1_06A2:
        push   edx
        push   ecx
        push   eax
        pushfd 
        cli    
        mov    al, byte ptr [0]              ; -> obj1:0x0114
        and    al, 0xbf
        mov    byte ptr [0], al              ; -> obj1:0x0114
        call   sub_1_05D0
        jb     loc_1_070A
        or     al, 9
        add    dx, 0
        out    dx, al
        inc    dx
        in     al, dx
        test   al, 8
        je     loc_1_06E0
        mov    ecx, 0x1388
loc_1_06CD:
        in     al, dx
        test   al, 0x20
        jne    loc_1_06D4
        loop   loc_1_06CD
loc_1_06D4:
        mov    ecx, 0x1388
loc_1_06D9:
        in     al, dx
        test   al, 0x20
        je     loc_1_06E0
        loop   loc_1_06D9
loc_1_06E0:
        sub    dx, 1
        cmp    word ptr [edi + 0x4c], 0
        jne    loc_1_0706
        mov    ecx, 8
        push   esi
        xor    esi, esi
loc_1_06F3:
        mov    eax, esi
        mov    ah, al
        mov    al, byte ptr [esi]            ; -> obj1:0x0126
        call   sub_1_0607
        inc    esi
        loop   loc_1_06F3
        pop    esi
loc_1_0706:
        popfd  
        clc    
        jmp    loc_1_070C
loc_1_070A:
        popfd  
        stc    
loc_1_070C:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_1_0710:
        push   edx
        push   ecx
        push   eax
        mov    cx, ax
        mov    ah, 0xa
        call   sub_1_05E9
        and    al, 0x3f
        mov    ah, byte ptr [0]              ; -> loc_1_0115
        or     cx, cx
        je     loc_1_072F
        or     ah, 0x40
        jmp    loc_1_0742
loc_1_072F:
        and    ah, 0xbf
        push   eax
        mov    ecx, 0x5000
        mov    ah, 0xa
loc_1_073A:
        call   sub_1_05E9
        loop   loc_1_073A
        pop    eax
loc_1_0742:
        mov    byte ptr [0], ah              ; -> loc_1_0115
        or     al, ah
        mov    ah, 0xa
        call   sub_1_0607
        pop    eax
        pop    ecx
        pop    edx
        clc    
        ret    
sub_1_0756:
        push   edx
        push   ecx
        push   eax
        pushfd 
        cli    
        mov    al, byte ptr [0]              ; -> obj1:0x0114
        or     al, 0x20
        mov    byte ptr [0], al              ; -> obj1:0x0114
        call   sub_1_05D0
        jb     loc_1_0777
        add    dx, 0
        out    dx, al
        popfd  
        clc    
        jmp    loc_1_0779
loc_1_0777:
        popfd  
        stc    
loc_1_0779:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_1_077D:
        push   edx
        push   ecx
        push   eax
        pushfd 
        cli    
        mov    al, byte ptr [0]              ; -> obj1:0x0114
        and    al, 0xdf
        mov    byte ptr [0], al              ; -> obj1:0x0114
        call   sub_1_05D0
        jb     loc_1_0777
        add    dx, 0
        out    dx, al
        popfd  
        clc    
        jmp    loc_1_07A0
        db  09Dh, 0F9h
loc_1_07A0:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_1_07A4:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 0x42]
        push   edi
        push   esi
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        call   sub_1_0314
        jb     loc_1_07C4
        mov    edi, dword ptr [esp]
        mov    eax, dword ptr [esi + 0xa]
        mov    dword ptr [edi + 0xa], eax
loc_1_07C4:
        pop    esi
        pop    edi
        pushfd 
        cli    
        cmp    word ptr [edi + 0x4c], 0
        jne    loc_1_07D9
        mov    eax, 1
        call   sub_1_0710
loc_1_07D9:
        call   sub_1_0629
        mov    ecx, 8
        mov    ah, 0xf
loc_1_07E5:
        mov    al, byte ptr [ecx + esi + 0xf]
        cmp    ah, 0xa
        jne    loc_1_07F6
        or     al, byte ptr [0]              ; -> loc_1_0115
        jmp    loc_1_0806
loc_1_07F6:
        cmp    ah, 9
        jne    loc_1_07FF
        and    al, 0xc
        jmp    loc_1_0806
loc_1_07FF:
        cmp    ah, 0xa
        jne    loc_1_0806
        and    al, 0xfd
loc_1_0806:
        call   sub_1_0607
        dec    ah
        loop   loc_1_07E5
        call   sub_1_06A2
        mov    ecx, 7
        mov    ah, 7
loc_1_081B:
        mov    al, byte ptr [ecx + esi + 8]
        call   sub_1_0607
        dec    ah
        loop   loc_1_081B
        cmp    word ptr [edi + 0x4c], 0
        jne    loc_1_0836
        xor    eax, eax
        call   sub_1_0710
loc_1_0836:
        test   word ptr [edi + 4], 0x200
        je     loc_1_0870
        add    edx, 0xa
        push   edx
        mov    eax, 6
        out    dx, al
        movzx  edx, word ptr [edi + 0xae]
        mov    ecx, 0x4000
        mov    eax, 0x80
loc_1_0859:
        out    dx, al
        loop   loc_1_0859
        xchg   dword ptr [esp], edx
        mov    al, 7
        out    dx, al
        pop    edx
        mov    ecx, 0x4000
        mov    eax, 0x80
loc_1_086D:
        out    dx, al
        loop   loc_1_086D
loc_1_0870:
        popfd  
        clc    
        popal  
        ret    
sub_1_0874:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 0x42]
        pushfd 
        cli    
        mov    ecx, 0xf
        mov    ah, 0xf
loc_1_0885:
        call   sub_1_05E9
        mov    byte ptr [ecx + esi + 8], al
        dec    ah
        loop   loc_1_0885
        popfd  
        clc    
        popal  
        ret    
sub_1_0896:
        push   eax
        push   ecx
        push   edx
        push   esi
        xor    ecx, ecx
        movzx  eax, ax
loc_1_089F:
        cmp    eax, dword ptr [ecx]          ; -> loc_1_0144
        jbe    loc_1_08B6
        add    ecx, 4
        cmp    dword ptr [ecx], -1           ; -> loc_1_0144
        jne    loc_1_089F
        sub    ecx, 4
loc_1_08B6:
        mov    eax, dword ptr [ecx]          ; -> loc_1_017C
        mov    dword ptr [esi + 0x90], eax
        shr    ecx, 2
        mov    al, byte ptr [ecx]            ; -> loc_1_0136
        cmp    al, byte ptr [esi + 0x8c]
        je     loc_1_0929
        mov    byte ptr [esi + 0x8c], al
        movzx  edx, word ptr [esi + 0x42]
        cmp    word ptr [esi + 0x4c], 0
        jne    loc_1_08EE
        mov    eax, 1
        call   sub_1_0710
loc_1_08EE:
        call   sub_1_0629
        jb     loc_1_0929
        mov    ah, 8
        mov    al, byte ptr [esi + 0x8c]
        call   sub_1_0607
        call   sub_1_06A2
        cmp    byte ptr [esi + 0xb], 0x8a
        je     loc_1_0913
        cmp    byte ptr [esi + 0xb], 0x81
        jne    loc_1_091B
loc_1_0913:
        mov    ecx, 0xc8
loc_1_0918:
        in     al, dx
        loop   loc_1_0918
loc_1_091B:
        cmp    word ptr [esi + 0x4c], 0
        jne    loc_1_0929
        xor    eax, eax
        call   sub_1_0710
loc_1_0929:
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_092E:
        push   eax
        in     al, dx
        jmp    loc_1_0932
loc_1_0932:
        in     al, dx
        jmp    loc_1_0935
loc_1_0935:
        pop    eax
        ret    
sub_1_0937:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_092E
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_092E
        pop    edx
        pop    ebx
        ret    
sub_1_0954:
        test   word ptr [edi + 0x94], 1
        je     loc_1_09D9
        pushal 
        mov    ax, word ptr [edi + 0x98]
        sub    word ptr [edi + 0xa2], ax
        cmc    
        pushfd 
        jb     loc_1_0980
        mov    ax, word ptr [edi + 0xa0]
        mov    word ptr [edi + 0xa2], ax
loc_1_0980:
        movzx  eax, word ptr [edi + 0x9a]
        add    ax, word ptr [edi + 0x98]
        cmp    word ptr [edi + 0x96], ax
        ja     loc_1_0999
        xor    eax, eax
loc_1_0999:
        mov    word ptr [edi + 0x9a], ax
        mov    esi, dword ptr [edi + 0x9c]
        add    esi, eax
        movzx  ecx, word ptr [edi + 0x96]
        sub    ecx, eax
        mov    word ptr [edi + 0xa4], cx
        movzx  edx, word ptr [edi + 0x42]
        add    edx, 0xa
        mov    al, 4
        out    dx, al
        movzx  ecx, word ptr [edi + 0x98]
        inc    ecx
        shr    ecx, 1
        movzx  edx, word ptr [edi + 0xae]
        cld    
        rep outsw dx, word ptr [esi]
        popfd  
        popal  
loc_1_09D9:
        ret    
        db  08Bh, 0C0h
loc_1_09DC:
        pushal 
        push   6
        push   eax
        call   sub_1_026C
        add    esp, 8
        or     edi, edi
        je     loc_1_0A8D
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        test   dl, 1
        je     loc_1_0A26
        test   word ptr [edi + 0x94], 2
        jne    loc_1_0A8D
        or     word ptr [edi + 0x94], 2
        mov    word ptr [edi + 0x96], 0
        mov    word ptr [edi + 0xa4], 0xffff
        jmp    loc_1_0A86
loc_1_0A26:
        test   word ptr [edi + 0x94], 2
        jne    loc_1_0A4D
        cmp    word ptr [edi + 0xa4], -1
        je     loc_1_0A4D
        pushfd 
        cli    
        mov    cx, word ptr [edi + 0xa4]
        VxDCall  VDMAD.VDMAD_Set_Virt_Count      ; cd2021000400
        popfd  
        jmp    loc_1_0A8D
loc_1_0A4D:
        mov    word ptr [edi + 0x9a], 0
        mov    word ptr [edi + 0x96], cx
        mov    word ptr [edi + 0x98], cx
        mov    word ptr [edi + 0xa4], cx
        mov    dword ptr [edi + 0x9c], esi
        and    word ptr [edi + 0x94], 0xfffc
        test   dl, 0x10
        je     loc_1_0A86
        or     word ptr [edi + 0x94], 1
loc_1_0A86:
        popal  
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012800400
loc_1_0A8D:
        popal  
        ret    
        db  090h
sub_1_0A90:
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        or     eax, eax
        je     loc_1_0AA0
        cmp    eax, ebx
        jne    loc_1_0B39
loc_1_0AA0:
        test   word ptr [esi + 4], 0x4000
        jne    loc_1_0B48
        cmp    dword ptr [esi + 0x24], ebx
        je     loc_1_0AEB
        push   ebx
        mov    ebx, dword ptr [esi + 0x28]
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0AC4
        mov    dword ptr [esi + 0x28], 0xffffffff
loc_1_0AC4:
        pop    ebx
        mov    eax, 1
        test   word ptr [esi + 0x52], 8
        je     loc_1_0AD5
        or     eax, 2
loc_1_0AD5:
        mov    edi, esi
        call   sub_5_123C
loc_1_0ADC:
        jb     loc_1_0B39
        test   word ptr [esi + 0x52], 8
        je     loc_1_0AEB
        call   sub_5_0DF0
loc_1_0AEB:
        cmp    word ptr [esi + 0x4c], 0
        je     loc_1_0AFD
        push   edx
        movzx  edx, word ptr [esi + 0x42]
        call   sub_1_0756
        pop    edx
loc_1_0AFD:
        mov    eax, dword ptr [0]            ; -> obj6:0x0104
        VxDCall  VKD.VKD_Local_Enable_Hot_Key    ; cd2003000d00
        mov    eax, dword ptr [0]            ; -> obj6:0x0100
        VxDCall  VKD.VKD_Local_Enable_Hot_Key    ; cd2003000d00
        mov    byte ptr [esi + 0x8c], 0xff
        mov    byte ptr [esi + 0x8d], 5
        or     word ptr [esi + 4], 0x4000
        or     word ptr [edi + 0x94], 2
        and    word ptr [edi + 0x94], 0xfffe
        jmp    loc_1_0B48
loc_1_0B39:
        mov    edi, esi
        mov    eax, 3
        call   sub_5_119E
loc_1_0B45:
        stc    
        jmp    loc_1_0B49
loc_1_0B48:
        clc    
loc_1_0B49:
        pop    eax
        ret    
        db  090h
loc_1_0B4C:
        xor    eax, eax
        dec    eax
        ret    
sub_1_0B50:
        push   eax
        xor    eax, eax
        mov    dword ptr [esi + 0x70], eax
        mov    word ptr [esi + 0x88], ax
        mov    word ptr [esi + 0xa0], ax
        mov    word ptr [esi + 0xa2], ax
        and    word ptr [esi + 0x8a], 2
        mov    dword ptr [esi + 0x68], eax
        mov    dword ptr [esi + 0x6c], eax
        mov    byte ptr [esi + 0x85], 0x7f
        mov    byte ptr [esi + 0x84], 0x7f
        mov    byte ptr [esi + 0x87], 0
        mov    eax, 0xaa
        call   sub_1_0C5E
        pop    eax
        ret    
        db  08Bh, 0C0h
loc_1_0B9C:
        call   sub_1_0A90
        jb     loc_1_0BD9
        cmp    ecx, 4
        jbe    loc_1_0BAE
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0BAE:
        jb     loc_1_0BD9
        test   al, 1
        je     loc_1_0BD2
        test   word ptr [esi + 0x8a], 2
        je     loc_1_0BC4
        call   sub_1_1148
loc_1_0BC4:
        call   sub_1_0B50
        or     byte ptr [esi + 0x85], 0x80
        jmp    loc_1_0BD9
loc_1_0BD2:
        and    byte ptr [esi + 0x85], 0x7f
loc_1_0BD9:
        ret    
        db  08Bh, 0C0h
loc_1_0BDC:
        call   sub_1_0A90
        jb     loc_1_0C00
        movzx  edx, word ptr [esi + 0x22]
        cmp    ecx, 4
        jbe    loc_1_0BF2
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0BF2:
        jb     loc_1_0BFF
        cmp    ah, 2
        jne    loc_1_0BFC
        add    edx, 2
loc_1_0BFC:
        out    dx, al
        jmp    loc_1_0C00
loc_1_0BFF:
        in     al, dx
loc_1_0C00:
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_0C04:
        call   sub_1_0A90
        jb     loc_1_0C23
        cmp    ecx, 4
        jbe    loc_1_0C16
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0C16:
        jb     loc_1_0C20
        movzx  edx, word ptr [esi + 0x22]
        inc    edx
        out    dx, al
        clc    
        ret    
loc_1_0C20:
        xor    eax, eax
        dec    eax
loc_1_0C23:
        ret    
loc_1_0C24:
        call   sub_1_0A90
        jb     loc_1_0C5D
        cmp    ecx, 4
        jbe    loc_1_0C36
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0C36:
        je     loc_1_0C5D
        push   ecx
        mov    ecx, dword ptr [esi + 0x68]
        cmp    ecx, dword ptr [esi + 0x6c]
        je     loc_1_0C4E
        inc    ecx
        cmp    ecx, 0x10
        jb     loc_1_0C49
        xor    ecx, ecx
loc_1_0C49:
        cmp    ecx, dword ptr [esi + 0x6c]
        jne    loc_1_0C55
loc_1_0C4E:
        and    byte ptr [esi + 0x84], 0x7f
loc_1_0C55:
        mov    al, byte ptr [ecx + esi + 0x74]
        mov    dword ptr [esi + 0x68], ecx
        pop    ecx
loc_1_0C5D:
        ret    
sub_1_0C5E:
        push   ecx
        mov    ecx, dword ptr [esi + 0x6c]
        inc    ecx
        cmp    ecx, 0x10
        jb     loc_1_0C6A
        xor    ecx, ecx
loc_1_0C6A:
        mov    byte ptr [ecx + esi + 0x74], al
        mov    dword ptr [esi + 0x6c], ecx
        or     byte ptr [esi + 0x84], 0x80
        pop    ecx
        ret    
        db  08Bh, 0C0h
loc_1_0C7C:
        call   sub_1_0A90
        jb     loc_1_0CCD
        cmp    ecx, 4
        jbe    loc_1_0C8E
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0C8E:
        jb     loc_1_0CBE
        push   eax
        push   ecx
        mov    ecx, dword ptr [esi + 0x70]
        jecxz  loc_1_0C9C
        call   ecx
        clc    
        jmp    loc_1_0CBB
loc_1_0C9C:
        movzx  eax, al
        mov    ecx, 0x10
loc_1_0CA4:
        cmp    eax, dword ptr [ecx*8 - 8]    ; -> loc_1_01AC
        je     loc_1_0CB2
        loop   loc_1_0CA4
        stc    
        jmp    loc_1_0CBB
loc_1_0CB2:
        call   sub_1_01B0
        clc    
loc_1_0CBB:
        pop    ecx
        pop    eax
        ret    
loc_1_0CBE:
        mov    al, byte ptr [esi + 0x85]
        and    byte ptr [esi + 0x85], 0x7f
        clc    
        ret    
loc_1_0CCD:
        stc    
        ret    
        db  090h, 066h, 0F7h, 086h, 08Ah, 000h, 000h, 000h, 001h, 000h, 075h, 053h, 050h, 052h, 00Fh, 0B7h
        db  056h, 042h, 0E8h, 043h, 0F9h, 0FFh, 0FFh, 0B4h, 00Fh, 032h, 0C0h, 0E8h, 018h, 0F9h, 0FFh, 0FFh
        db  0B4h, 00Eh, 0E8h, 011h, 0F9h, 0FFh, 0FFh, 0B4h, 009h, 0B0h, 041h, 0E8h, 008h, 0F9h, 0FFh, 0FFh
        db  0E8h, 09Eh, 0F9h, 0FFh, 0FFh, 0B4h, 006h, 08Ah, 086h, 08Dh, 000h, 000h, 000h, 0E8h, 0F6h, 0F8h
        db  0FFh, 0FFh, 0B4h, 007h, 0E8h, 0EFh, 0F8h, 0FFh, 0FFh, 0B4h, 00Ah, 0A0h
        dd  loc_1_0115               ; fixup
        db  0E8h, 0E3h, 0F8h, 0FFh, 0FFh, 066h, 083h, 08Eh, 08Ah, 000h, 000h, 000h, 001h, 05Ah, 058h, 0C6h
        db  086h, 086h, 000h, 000h, 000h, 000h, 0C7h, 046h, 070h
        dd  loc_1_0D40               ; fixup
        db  0C3h, 02Eh, 08Bh, 0C0h
loc_1_0D40:
        push   edx
        movzx  edx, word ptr [esi + 0x42]
        add    dx, 3
        out    dx, al
        mov    dword ptr [esi + 0x70], 0
        pop    edx
        ret    
loc_1_0D53:
        push   edx
        movzx  edx, word ptr [esi + 0x42]
        test   word ptr [esi + 0x8a], 1
        je     loc_1_0D9D
        cmp    word ptr [esi + 0x4c], 0
        jne    loc_1_0D74
        mov    eax, 1
        call   sub_1_0710
loc_1_0D74:
        call   sub_1_0629
        mov    ah, 9
        mov    al, 4
        call   sub_1_0607
        call   sub_1_06A2
        cmp    word ptr [esi + 0x4c], 0
        jne    loc_1_0D95
        xor    eax, eax
        call   sub_1_0710
loc_1_0D95:
        and    word ptr [esi + 0x8a], 0xfffe
loc_1_0D9D:
        push   ecx
        mov    eax, dword ptr [esi + 0x2c]
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        test   ecx, 2
        pop    ecx
        je     loc_1_0DB8
        or     word ptr [esi + 0x8a], 4
loc_1_0DB8:
        pushfd 
        cli    
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        call   sub_1_0607
        mov    ah, 9
        mov    al, 4
        call   sub_1_0607
        xor    al, al
        mov    dx, word ptr [esi + 0x42]
        add    dx, 2
        out    dx, al
        sub    dx, 2
        mov    word ptr [esi + 0xa0], 0
        mov    word ptr [esi + 0xa2], 0
        mov    byte ptr [esi + 0x86], 0
        mov    dword ptr [esi + 0x70], 0     ; -> loc_1_0E30
        and    word ptr [esi + 0x8a], 0xffef
        popfd  
        pop    edx
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_0E0C:
        pushfd 
        push   edx
        push   ecx
        push   eax
        cli    
        or     byte ptr [esi + 0x85], 0x80
        or     word ptr [esi + 0x8a], 2
        movzx  ecx, word ptr [esi + 0xa0]
        movzx  edx, word ptr [esi + 0x42]
        jmp    loc_1_0E78
        db  02Eh, 08Bh, 0C0h
loc_1_0E30:
        pushfd 
        push   edx
        push   ecx
        push   eax
        mov    cx, word ptr [esi + 0xa0]
        mov    cl, ch
        mov    ch, al
        mov    word ptr [esi + 0xa0], cx
        mov    cl, byte ptr [esi + 0x86]
        inc    cl
        cmp    cl, 2
        jb     loc_1_0F08
        mov    dword ptr [esi + 0x70], 0
        mov    byte ptr [esi + 0x86], 0
        cli    
        movzx  edx, word ptr [esi + 0x42]
        movzx  ecx, word ptr [esi + 0xa0]
        mov    word ptr [esi + 0xa2], cx
loc_1_0E78:
        test   word ptr [esi + 4], 0x200
        je     loc_1_0EAE
        movzx  ecx, word ptr [esi + 0x98]
        dec    ecx
        cld    
        push   ecx
        inc    ecx
        add    edx, 0xa
        push   edx
        mov    al, 6
        out    dx, al
        push   esi
        shr    ecx, 1
        movzx  edx, word ptr [esi + 0xae]
        mov    esi, dword ptr [esi + 0x9c]
        rep outsw dx, word ptr [esi]
        pop    esi
        pop    edx
        mov    al, 7
        out    dx, al
        sub    edx, 0xa
        pop    ecx
loc_1_0EAE:
        mov    ah, 0xf
        mov    al, cl
        call   sub_1_0607
        mov    ah, 0xe
        mov    al, ch
        call   sub_1_0607
        mov    ah, 6
        mov    al, byte ptr [esi + 0x8d]
        cmp    byte ptr [esi + 0x87], 0xff
        je     loc_1_0ED3
        or     al, 0x80
loc_1_0ED3:
        call   sub_1_0607
        mov    ah, 7
        call   sub_1_0607
        mov    ah, 9
        mov    al, 1
        call   sub_1_0607
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        or     al, 2
        call   sub_1_0607
        or     byte ptr [esi + 0x85], 0x80
        or     word ptr [esi + 0x8a], 2
        xor    cx, cx
loc_1_0F08:
        mov    byte ptr [esi + 0x86], cl
        pop    eax
        pop    ecx
        pop    edx
        popfd  
        ret    
        db  090h
loc_1_0F14:
        pushfd 
        push   eax
        push   ecx
        push   edx
        cli    
        movzx  edx, word ptr [esi + 0x42]
        cmp    word ptr [esi + 0x4c], 0
        je     loc_1_0F29
        call   sub_1_077D
loc_1_0F29:
        test   word ptr [esi + 0x8a], 1
        je     loc_1_0F6E
        cmp    word ptr [esi + 0x4c], 0
        jne    loc_1_0F45
        mov    eax, 1
        call   sub_1_0710
loc_1_0F45:
        call   sub_1_0629
        mov    ah, 9
        mov    al, 4
        call   sub_1_0607
        call   sub_1_06A2
        cmp    word ptr [esi + 0x4c], 0
        jne    loc_1_0F66
        xor    eax, eax
        call   sub_1_0710
loc_1_0F66:
        and    word ptr [esi + 0x8a], 0xfffe
loc_1_0F6E:
        movzx  ecx, word ptr [esi + 0xa0]
        mov    word ptr [esi + 0xa2], cx
        or     word ptr [esi + 0x8a], 0x12
        test   word ptr [esi + 4], 0x200
        je     loc_1_1009
        push   ebx
        push   edx
        mov    ecx, 4
        movzx  eax, word ptr [esi + 0x96]
        cmp    ax, word ptr [esi + 0xa0]
        jbe    loc_1_0FAB
        movzx  eax, word ptr [esi + 0xa0]
        inc    eax
loc_1_0FAB:
        push   eax
loc_1_0FAC:
        mov    eax, dword ptr [esp]
        xor    ebx, ebx
        bts    ebx, ecx
        dec    ebx
        mov    edx, eax
        shr    edx, cl
        cmp    edx, 0x40
        jbe    loc_1_0FC2
        and    eax, ebx
        je     loc_1_0FC7
loc_1_0FC2:
        dec    ecx
        jecxz  loc_1_0FC7
        jmp    loc_1_0FAC
loc_1_0FC7:
        pop    eax
        pop    edx
        pop    ebx
        shr    eax, cl
        xchg   ecx, eax
        mov    word ptr [esi + 0x98], cx
        add    edx, 0xa
        mov    al, 6
        out    dx, al
        movzx  ecx, word ptr [esi + 0x98]
        cld    
        push   esi
        push   ecx
        push   edx
        movzx  edx, word ptr [edi + 0xae]
        mov    esi, dword ptr [esi + 0x9c]
        shr    ecx, 1
        rep outsw dx, word ptr [esi]
        xchg   dword ptr [esp], edx
        mov    al, 7
        out    dx, al
        pop    edx
        mov    ecx, dword ptr [esp]
        shr    ecx, 1
        rep outsw dx, word ptr [esi]
        pop    ecx
        pop    esi
        dec    ecx
loc_1_1009:
        movzx  edx, word ptr [esi + 0x42]
        mov    ah, 0xf
        mov    al, cl
        call   sub_1_0607
        mov    ah, 0xe
        mov    al, ch
        call   sub_1_0607
        mov    ah, 6
        mov    al, byte ptr [esi + 0x8d]
        call   sub_1_0607
        mov    ah, 7
        call   sub_1_0607
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        or     al, 2
        call   sub_1_0607
        mov    ah, 9
        mov    al, 1
        call   sub_1_0607
        pop    edx
        pop    ecx
        pop    eax
        popfd  
        ret    
        db  090h
loc_1_1050:
        mov    word ptr [esi + 0x88], 0
        mov    byte ptr [esi + 0x86], 0
        mov    dword ptr [esi + 0x70], 0     ; -> loc_1_1068
        ret    
loc_1_1068:
        push   eax
        push   ebx
        push   edx
        neg    al
        movzx  ebx, al
        or     ebx, ebx
        jne    loc_1_1079
        mov    ebx, 0x100
loc_1_1079:
        mov    edx, 0xf
        mov    eax, 0x4240
        div    bx
        mov    word ptr [esi + 0x88], ax
        mov    dword ptr [esi + 0x70], 0
        call   sub_1_0896
        pop    edx
        pop    ebx
        pop    eax
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_10A0:
        mov    word ptr [esi + 0xa0], 0
        mov    word ptr [esi + 0xa2], 0
        mov    byte ptr [esi + 0x86], 0
        mov    dword ptr [esi + 0x70], 0     ; -> loc_1_10C4
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_10C4:
        push   edx
        movzx  edx, word ptr [esi + 0xa0]
        mov    dl, dh
        mov    dh, al
        mov    word ptr [esi + 0xa0], dx
        mov    dl, byte ptr [esi + 0x86]
        inc    dl
        cmp    dl, 2
        jb     loc_1_1101
        cmp    edx, 0x10
        jae    loc_1_10F5
        mov    edx, 0x10
        mov    word ptr [esi + 0xa0], dx
loc_1_10F5:
        mov    word ptr [esi + 0xa2], dx
        xor    edx, edx
        mov    dword ptr [esi + 0x70], edx
loc_1_1101:
        mov    byte ptr [esi + 0x86], dl
        pop    edx
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_110C:
        mov    byte ptr [esi + 0x87], 0xff
        ret    
loc_1_1114:
        mov    byte ptr [esi + 0x87], 0
        ret    
loc_1_111C:
        push   eax
        mov    al, byte ptr [esi + 0x87]
        call   sub_1_0C5E
        pop    eax
        ret    
loc_1_112A:
        cmp    word ptr [esi + 0x4c], 0
        je     loc_1_113C
        push   edx
        movzx  edx, word ptr [esi + 0x42]
        call   sub_1_0756
        pop    edx
loc_1_113C:
        and    word ptr [esi + 0x8a], 0xffef
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_1148:
        push   eax
        push   edx
        movzx  edx, word ptr [esi + 0x42]
        mov    ah, 6
        mov    al, 0x3f
        call   sub_1_0607
        inc    ah
        call   sub_1_0607
        mov    al, 4
        mov    ah, 9
        call   sub_1_0607
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        call   sub_1_0607
        xor    al, al
        add    dx, 2
        out    dx, al
        sub    dx, 2
        and    word ptr [esi + 0x8a], 0xfffd
        pop    edx
        pop    eax
        ret    
loc_1_1189:
        mov    dword ptr [esi + 0x70], 0     ; -> loc_1_1191
        ret    
loc_1_1191:
        push   eax
        not    al
        call   sub_1_0C5E
        mov    dword ptr [esi + 0x70], 0
        pop    eax
        ret    
loc_1_11A2:
        mov    dword ptr [esi + 0x70], 0     ; -> loc_1_11AA
        ret    
loc_1_11AA:
        push   esi
        push   edx
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 0xa8]
        or     eax, eax
        jne    loc_1_11BB
        mov    eax, dword ptr [esi + 0x38]
loc_1_11BB:
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        test   dl, 1
        jne    loc_1_122A
        mov    al, byte ptr [esp]
        mov    ah, byte ptr [esi + 0x8f]
        push   ebx
        test   word ptr [esi + 0x8a], 8
        jne    loc_1_1200
        mov    ah, al
        and    ah, 6
        shl    ah, 1
        mov    bl, 0x40
        test   al, 0x10
        je     loc_1_11EA
        mov    bl, 0x20
loc_1_11EA:
        sub    bl, ah
        add    al, bl
        xor    ah, ah
        mov    byte ptr [esi + 0x8f], al
        or     word ptr [esi + 0x8a], 8
        jmp    loc_1_120A
loc_1_1200:
        xor    al, 0xa5
        add    al, ah
        mov    byte ptr [esi + 0x8f], al
loc_1_120A:
        mov    byte ptr [esi], al
        inc    esi
        dec    ecx
        cmp    ecx, -1
        jne    loc_1_1216
        or     dl, 1
loc_1_1216:
        pop    ebx
        mov    eax, dword ptr [esi + 0xa8]
        or     eax, eax
        jne    loc_1_1224
        mov    eax, dword ptr [esi + 0x38]
loc_1_1224:
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
loc_1_122A:
        mov    dword ptr [esi + 0x70], 0
        pop    eax
        pop    ecx
        pop    edx
        pop    esi
        ret    
loc_1_1236:
        mov    dword ptr [esi + 0x70], 0     ; -> loc_1_123E
        ret    
loc_1_123E:
        mov    byte ptr [esi + 0x8e], al
        mov    dword ptr [esi + 0x70], 0
        ret    
sub_1_124C:
        push   eax
        mov    al, byte ptr [esi + 0x8e]
        call   sub_1_0C5E
        pop    eax
        ret    
        db  08Bh, 0C0h
loc_1_125C:
        push   eax
        mov    eax, 0x202
        call   sub_1_0C5E
        shr    eax, 8
        call   sub_1_0C5E
        mov    eax, 0x5742
        call   sub_1_0C5E
        shr    eax, 8
        call   sub_1_0C5E
        pop    eax
        ret    
        db  090h
loc_1_1284:
        push   eax
        mov    eax, dword ptr [esi + 0x2c]
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        pop    eax
        ret    
loc_1_1290:
        call   sub_1_0A90
        jb     loc_1_1339
        cmp    ecx, 4
        jbe    loc_1_12A6
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_12A6:
        je     loc_1_1339
        push   eax
        push   ecx
        push   edx
        test   word ptr [esi + 0x8a], 0x10
        je     loc_1_12CA
        pushfd 
        cli    
        movzx  edx, word ptr [esi + 0x42]
        add    edx, 2
        out    dx, al
        sub    edx, 2
        popfd  
        jmp    loc_1_132E
loc_1_12CA:
        test   word ptr [esi + 0x8a], 4
        je     loc_1_12DF
        and    word ptr [esi + 0x8a], 0xfffb
        jmp    loc_1_132E
loc_1_12DF:
        mov    eax, dword ptr [esi + 0x2c]
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        test   ecx, 2
        je     loc_1_132E
        pushfd 
        cli    
        movzx  edx, word ptr [esi + 0x42]
        mov    ah, 9
        mov    al, 4
        call   sub_1_0607
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        call   sub_1_0607
        add    edx, 2
        out    dx, al
        sub    edx, 2
        popfd  
        test   word ptr [esi + 0x8a], 2
        je     loc_1_132E
        movzx  ecx, word ptr [esi + 0xa0]
        or     ecx, ecx
        jne    loc_1_132E
        call   sub_1_1148
loc_1_132E:
        pop    edx
        pop    ecx
        pop    eax
        mov    al, byte ptr [esi + 0x84]
        clc    
        ret    
loc_1_1339:
        stc    
        ret    
sub_1_133B:
        push   eax
        cmp    word ptr [edi + 0x4c], 0
        je     loc_1_134E
        push   edx
        movzx  edx, word ptr [edi + 0x42]
        call   sub_1_077D
        pop    edx
loc_1_134E:
        mov    eax, dword ptr [0]            ; -> obj6:0x0104
        VxDCall  VKD.VKD_Local_Disable_Hot_Key   ; cd2004000d00
        mov    eax, dword ptr [0]            ; -> obj6:0x0100
        VxDCall  VKD.VKD_Local_Disable_Hot_Key   ; cd2004000d00
        mov    ax, word ptr [edi + 0x8a]
        test   ax, 2
        je     loc_1_1378
        mov    esi, edi
        call   sub_1_1148
loc_1_1378:
        and    word ptr [edi + 4], 0xbfff
        pop    eax
        ret    
loc_1_1380:
        push   eax
        cmp    al, 0x49
        jne    loc_1_1399
        mov    al, byte ptr [esi + 0x8d]
        or     al, al
        je     loc_1_13A9
        dec    al
        mov    byte ptr [esi + 0x8d], al
        jmp    loc_1_13A9
loc_1_1399:
        mov    al, byte ptr [esi + 0x8d]
        inc    al
        and    al, 0x3f
        mov    byte ptr [esi + 0x8d], al
loc_1_13A9:
        pop    eax
        ret    
        db  0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xee  R-X
        db  003h, 000h, 000h, 000h
        dd  loc_2_0014               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        push   edx
        db  000h
loc_2_0010:
        mov    cl, 0
        fiadd  dword ptr [eax]
loc_2_0014:
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     loc_2_008C
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    loc_2_0043
        jae    loc_2_0094
        jne    0x95
        and    byte ptr fs:[0x65687420], ch
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0xa9
        and    byte ptr [ebx + 0x6f], dl
        jne    0xa9
        and    byte ptr fs:[ebx + 0x79], dl
        jae    loc_2_00B5
        insd   dword ptr es:[edi], dx
loc_2_0043:
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0xbe
        jb     loc_2_0079
        popal  
        jo     loc_2_00CC
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[ebp + 0x6e], dl
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xde
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    0x95
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
loc_2_0079:
        and    byte ptr [0x65687420], ch
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0xfa
        and    byte ptr [ebx + 0x6f], dl
        jne    0xfa
loc_2_008C:
        and    byte ptr fs:[ebx + 0x79], dl
        jae    0x106
        insd   dword ptr es:[edi], dx
loc_2_0094:
        and    byte ptr [ebp + 0x49], cl
        inc    esp
        dec    ecx
        and    byte ptr [ebx + 0x79], dl
        outsb  dx, byte ptr [esi]
        je     0x107
        jae    0x10b
        jp     0x109
        jb     0xc6
        imul   esi, dword ptr [ebx + 0x20], 0x20657375
        bound  edi, qword ptr [ecx + 0x20]
        popal  
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x11d
loc_2_00B5:
        jb     0xd8
        popal  
        jo     0x12b
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x13f
loc_2_00CC:
        and    byte ptr [ebx + 0x6f], dl
        jne    0x13f
        and    byte ptr fs:[ebx + 0x79], dl
        jae    0x14b
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + ebp*2 + 0x72], al
        arpl   word ptr gs:[eax + 0x53], si
        outsd  dx, dword ptr [esi]
        jne    0x153
        and    byte ptr fs:[edx + esi*2 + 0x69], al
        jbe    0x151
        jb     0xee
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x1d9  R-X
loc_3_0000:
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_0350
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        mov    eax, dword ptr [0]            ; -> obj6:0x0100
        VxDCall  VKD.VKD_Remove_Hot_Key          ; cd2002000d00
        mov    eax, dword ptr [0]            ; -> obj6:0x0104
        VxDCall  VKD.VKD_Remove_Hot_Key          ; cd2002000d00
        xor    esi, esi
        xchg   dword ptr [0], esi            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_3_0060
        VMMCall  VMM.List_Destroy                ; cd209c000100
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        push   ebx
loc_3_0045:
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_3_0054
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_3_0054:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_3_0045
        pop    ebx
loc_3_0060:
        clc    
        ret    
sub_3_0062:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
loc_3_0072:
        add    esp, 8
        or     edi, edi
        je     loc_3_00DC
        mov    ebx, dword ptr [edi + 0x24]
        or     ebx, ebx
        je     loc_3_00DC
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_00DC
        call   sub_1_0314
loc_3_008D:
        jb     loc_3_00DC
        call   sub_1_0874
loc_3_0094:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_3_00C0
        mov    edi, esi
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        mov    word ptr [edi + 0x30], dx
        mov    dword ptr [edi + 0x28], ecx
        mov    dword ptr [edi + 0x2c], esi
        mov    esi, edi
        test   word ptr [esi + 0x30], 1
        jne    loc_3_00C0
        VxDCall  VDMAD.VDMAD_Mask_Channel        ; cd2007000400
loc_3_00C0:
        pop    edi
        movzx  edx, word ptr [edi + 0x42]
        mov    ah, 9
        xor    al, al
        call   sub_1_0607
loc_3_00CE:
        inc    ah
        call   sub_1_0607
loc_3_00D5:
        pop    edx
        pop    ecx
        or     word ptr [edi + 4], 0x10
loc_3_00DC:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_3_00E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
loc_3_00F4:
        add    esp, 8
        or     edi, edi
        je     loc_3_01D1
        movzx  eax, word ptr [edi + 6]
        mov    dword ptr [ebp - 4], eax
        movzx  eax, byte ptr [edi + 8]
        mov    dword ptr [ebp - 8], eax
        movzx  eax, byte ptr [edi + 9]
        mov    dword ptr [ebp - 0xc], eax
        movzx  eax, byte ptr [edi + 0xa]
        mov    dword ptr [ebp - 0x10], eax
        test   word ptr [edi + 4], 0x100
        jne    loc_3_0154
        test   word ptr [edi + 4], 0x200
        jne    loc_3_0149
        test   word ptr [edi + 0x52], 4
        je     loc_3_0154
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        call   sub_4_081C
loc_3_0144:
        add    esp, 0x10
        jmp    loc_3_0154
loc_3_0149:
        push   dword ptr [ebp - 4]
        call   sub_4_087E
loc_3_0151:
        add    esp, 4
loc_3_0154:
        mov    ebx, dword ptr [edi + 0x24]
        or     ebx, ebx
        je     loc_3_01D1
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_01D1
        test   word ptr [edi + 4], 0x10
        je     loc_3_01D1
        call   sub_1_0314
loc_3_0170:
        jb     loc_3_01D1
        or     byte ptr [esi + 0x11], 8
        call   sub_1_07A4
loc_3_017B:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_3_01A4
        mov    edi, esi
        mov    dx, word ptr [edi + 0x30]
        mov    ecx, dword ptr [edi + 0x28]
        mov    esi, dword ptr [edi + 0x2c]
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
        mov    esi, edi
        test   dl, 1
        jne    loc_3_01A4
        VxDCall  VDMAD.VDMAD_UnMask_Channel      ; cd2008000400
loc_3_01A4:
        pop    edi
        movzx  edx, word ptr [edi + 0x42]
        mov    ecx, 9
        mov    ah, cl
        and    byte ptr [ecx + esi + 8], 0xf7
        mov    al, byte ptr [ecx + esi + 8]
        call   sub_1_0607
loc_3_01BE:
        inc    ecx
        inc    ah
        mov    al, byte ptr [ecx + esi + 8]
        call   sub_1_0607
loc_3_01CA:
        pop    edx
        pop    ecx
        and    word ptr [edi + 4], 0xffef
loc_3_01D1:
        pop    esi
        pop    edi
        pop    ebx
        add    esp, 0x10
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x13a0  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
loc_4_000E:
        add    esp, 8
        or     edi, edi
        je     loc_4_0021
        cmp    dword ptr [edi + 0x24], 0
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
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
loc_4_003F:
        add    esp, 8
        or     edi, edi
        je     loc_4_006C
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi], 0
        test   word ptr [edi + 4], 0x1000
        jne    loc_4_006C
        mov    eax, 0xffffffff
        test   word ptr [edi + 4], 0x200
        je     loc_4_0071
        or     dword ptr [esi], 1
        or     dword ptr [esi], 0x10
        jmp    loc_4_0071
loc_4_006C:
        mov    eax, 0
loc_4_0071:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_4_0075:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi], 0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
loc_4_008D:
        add    esp, 8
        or     edi, edi
        je     loc_4_00A1
        movzx  eax, word ptr [edi + 4]
        mov    dword ptr [esi], eax
        mov    eax, 0xffffffff
        jmp    loc_4_00A6
loc_4_00A1:
        mov    eax, 0
loc_4_00A6:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_4_00AA:
        cmp    eax, 0x40
        jle    loc_4_00B4
        mov    eax, 0x40
loc_4_00B4:
        add    eax, 3
        shr    eax, 2
loc_4_00BA:
        mov    ebx, eax
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        mov    ecx, ebx
        push   eax
        VxDCall  VDMAD.VDMAD_Get_Max_Phys_Page   ; cd201e000400
        mov    edx, 0xfff
        cmp    eax, edx
        jae    loc_4_00DF
        mov    edx, eax
loc_4_00DF:
        pop    eax
        push   ecx
        sub    esp, 4
        mov    ebx, esp
        push   0xe
        push   ebx
        push   edx
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
        jne    loc_4_010B
        mov    eax, ecx
        shr    eax, 1
        jne    loc_4_00BA
        xor    ecx, ecx
        stc    
        ret    
loc_4_010B:
        shl    ecx, 0xc
        clc    
        ret    
sub_4_0110:
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_4_0177
        cli    
        VMMCall  VMM.List_Allocate               ; cd209d000100
        sti    
        jb     loc_4_0177
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 0x34
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        pop    eax
        mov    edi, dword ptr [esp]
        push   eax
        push   edi
        push   esi
        mov    dword ptr [eax + 4], edi
        lea    edi, [eax + 8]
        mov    esi, 0                        ; -> obj1:0x0116
        mov    ecx, 0x10
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        pop    edi
        pop    eax
        test   word ptr [edi + 4], 0x200
        je     loc_4_0169
        mov    byte ptr [eax + 0xc], 0
        mov    byte ptr [eax + 0xd], 0
loc_4_0169:
        cli    
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        sti    
        clc    
loc_4_0172:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
loc_4_0177:
        stc    
        jmp    loc_4_0172
sub_4_017A:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     loc_4_01B2
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_01B2
loc_4_0195:
        cmp    dword ptr [eax + 4], edi
        je     loc_4_01A4
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_01B2
        jmp    loc_4_0195
loc_4_01A4:
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_4_01B2:
        popfd  
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_4_01B8:
        jmp    loc_4_01C4
loc_4_01BA:
        mov    esi, 0
        jmp    loc_4_027D
loc_4_01C4:
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
        je     loc_4_0277
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_01FF:
        push   ecx
        mov    ecx, 0xa
        lea    esi, [0]                      ; -> loc_4_01BA
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        loop   loc_4_01FF
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_021B:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0xa
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_024E
        add    edi, 6
        add    esi, 0xa
        loop   loc_4_021B
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_4_027D
loc_4_024E:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_0269
        add    edi, 2
loc_4_025B:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_025B
loc_4_0269:
        push   0
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_0277:
        xor    eax, eax
        mov    dword ptr [esp + 0x1c], eax
loc_4_027D:
        popal  
        add    esp, 0xc
        pop    ebp
        ret    
sub_4_0283:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_028D:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_028D
        pop    eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret    
sub_4_02AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_06AA
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_06AA
        mov    dword ptr [ebp - 4], eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    dword ptr [eax], 0
        push   1
        push   0xba
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_06AA
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x54], eax
        mov    ax, word ptr [ebp + 0x30]
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 0xb8], 0
        mov    ax, word ptr [ebp + 0x34]
        mov    word ptr [edi + 0x52], ax
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [edi + 6], ax
        sub    word ptr [edi + 6], 4
        mov    word ptr [edi + 0x42], ax
        mov    ax, word ptr [ebp + 0x20]
        mov    byte ptr [edi + 8], al
        test   dword ptr [ebp + 0x30], 0x200
        jne    loc_4_0356
        mov    ax, word ptr [ebp + 0x24]
        mov    byte ptr [edi + 9], al
        mov    ax, word ptr [ebp + 0x28]
        mov    byte ptr [edi + 0xa], al
        cmp    ax, word ptr [ebp + 0x24]
        je     loc_4_035B
loc_4_0356:
        or     word ptr [edi + 0x52], 0x40
loc_4_035B:
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edi + 0x4e], ax
        cmp    word ptr [ebp + 0x10], -1
        je     loc_4_0390
        push   dword ptr [ebp + 0x10]
        call   sub_4_0BC4
        add    esp, 4
        or     eax, eax
        je     loc_4_06AA
        or     word ptr [edi + 0x52], 4
        mov    ah, al
        and    ah, 0x38
        shr    ah, 3
        and    al, 7
        mov    word ptr [edi + 0xe], ax
loc_4_0390:
        call   sub_5_1309
loc_4_0395:
        mov    word ptr [edi + 0xc], ax
        mov    word ptr [2], 4               ; -> obj6:0x01CB
        mov    dword ptr [4], 0              ; -> obj6:0x01CD
        test   dword ptr [ebp + 0x30], 0x200
        je     loc_4_0442
        xor    eax, eax
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [edi + 0x1c], 0x8000
        sub    esp, 8
        mov    eax, esp
        mov    edx, eax
        add    edx, 4
        push   edx
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_5_0441
loc_4_03DF:
        add    esp, 0xc
        mov    dx, word ptr [ebp + 0xc]
        add    dx, word ptr [esp]
        mov    word ptr [edi + 0xae], dx
        mov    dx, word ptr [esp + 4]
        mov    word ptr [edi + 0xb0], dx
        add    esp, 8
        cmp    eax, 0
        jne    loc_4_06AA
        cmp    dx, 0x100
        jne    loc_4_0442
        push   dword ptr [ebp + 8]
        call   sub_5_051D
loc_4_0418:
        add    esp, 4
        cmp    eax, -1
        je     loc_4_06AA
        add    eax, 0x14
        mov    word ptr [edi + 0x48], ax
        or     word ptr [edi + 4], 8
        or     word ptr [2], 2               ; -> obj6:0x01CB
        mov    dword ptr [4], 0              ; -> obj6:0x01CD
loc_4_0442:
        push   edi
        movzx  ecx, byte ptr [edi + 8]
        mov    word ptr [0], cx              ; -> obj6:0x01C9
        mov    dword ptr [0x1c], edi         ; -> obj6:0x01E5
        mov    edi, 0                        ; -> obj6:0x01C9
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edi
        jae    loc_4_0474
        push   0                             ; -> obj6:0x024F
        call   sub_5_0000
loc_4_046C:
        add    esp, 4
        jmp    loc_4_06AA
loc_4_0474:
        mov    dword ptr [edi + 0x2c], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 4
        jne    loc_4_0491
        or     word ptr [edi + 4], 0x8000
loc_4_0491:
        test   word ptr [edi + 4], 8
        je     loc_4_04A1
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_4_04AD
loc_4_04A1:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_4_04AD:
        test   word ptr [edi + 0x52], 4
        je     loc_4_04F6
        movzx  ecx, word ptr [0]             ; -> obj6:0x0108
        mov    esi, 0                        ; -> obj6:0x0108
        movzx  edx, word ptr [ebp + 0x10]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04CB:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04CB
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 0x58], eax
        or     eax, eax
        jne    loc_4_04F6
        push   0                             ; -> obj6:0x01E9
        call   sub_5_0000
loc_4_04EE:
        add    esp, 4
        jmp    loc_4_06AA
loc_4_04F6:
        mov    esi, 0                        ; -> obj6:0x0122
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_4_0E24
        mov    byte ptr [edi + 0xb], ah
        xor    ah, ah
        mov    word ptr [edi + 0x4c], ax
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0516:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0516
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 0x5c], eax
        or     eax, eax
        jne    loc_4_0541
        push   0                             ; -> obj6:0x0205
        call   sub_5_0000
loc_4_0539:
        add    esp, 4
        jmp    loc_4_06AA
loc_4_0541:
        movsx  edx, word ptr [ebp + 0x18]
        cmp    edx, -1
        je     loc_4_058C
        mov    word ptr [edi + 0x22], dx
        or     word ptr [edi + 0x52], 8
        mov    esi, 0                        ; -> obj6:0x013C
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0561:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0561
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 0x60], eax
        or     eax, eax
        jne    loc_4_058C
        push   0                             ; -> obj6:0x0236
        call   sub_5_0000
loc_4_0584:
        add    esp, 4
        jmp    loc_4_06AA
loc_4_058C:
        movsx  edx, word ptr [ebp + 0x1c]
        cmp    edx, -1
        je     loc_4_05D8
        mov    word ptr [edi + 0x40], dx
        or     word ptr [edi + 4], 0x1000
        mov    esi, 0                        ; -> obj6:0x0156
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_05AD:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_05AD
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 0x64], eax
        or     eax, eax
        jne    loc_4_05D8
        push   0                             ; -> obj6:0x021F
        call   sub_5_0000
loc_4_05D0:
        add    esp, 4
        jmp    loc_4_06AA
loc_4_05D8:
        test   dword ptr [ebp + 0x30], 0x200
        jne    loc_4_0681
        mov    eax, dword ptr [ebp + 0x2c]
        call   sub_4_00AA
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0x14], ebx
        mov    dword ptr [edi + 0x1c], ecx
        mov    dword ptr [edi + 0x18], edx
        jae    loc_4_060D
        push   0                             ; -> obj6:0x027F
        call   sub_5_0000
loc_4_0605:
        add    esp, 4
        jmp    loc_4_06AA
loc_4_060D:
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
        movzx  eax, word ptr [ebp + 0x24]
        mov    esi, 0                        ; -> loc_1_03A8
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [edi + 0x38], eax
        jae    loc_4_0655
        push   0                             ; -> obj6:0x0267
        call   sub_5_0000
loc_4_0650:
        add    esp, 4
        jmp    loc_4_06AA
loc_4_0655:
        mov    ax, word ptr [ebp + 0x28]
        cmp    ax, word ptr [ebp + 0x24]
        je     loc_4_0681
        movzx  eax, al
        mov    esi, 0                        ; -> loc_1_03A8
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [edi + 0x3c], eax
        jae    loc_4_0681
        push   0                             ; -> obj6:0x0267
        call   sub_5_0000
loc_4_067C:
        add    esp, 4
        jmp    loc_4_06AA
loc_4_0681:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0688:
        call   sub_4_0110
        jb     loc_4_06A9
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0688
        pop    ebx
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_06A3:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_06A9:
        pop    ebx
loc_4_06AA:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     loc_4_06B6
        call   sub_4_06C0
loc_4_06B6:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_06A3
sub_4_06C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_07FF
        xor    esi, esi
        xchg   dword ptr [edi + 0x44], esi
        or     esi, esi
        je     loc_4_06E3
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_06E3:
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_4_06F0
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_06F0:
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_4_06FD
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_06FD:
        movzx  eax, word ptr [edi + 0x20]
        or     eax, eax
        je     loc_4_0720
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        mov    eax, dword ptr [edi + 0x10]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_0720:
        mov    eax, dword ptr [edi + 0x64]
        or     eax, eax
        je     loc_4_074E
        mov    esi, 0                        ; -> obj6:0x0156
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 0x40]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0739:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0739
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x64]
        call   sub_4_0283
        pop    edi
loc_4_074E:
        mov    eax, dword ptr [edi + 0x60]
        or     eax, eax
        je     loc_4_077C
        mov    esi, 0                        ; -> obj6:0x013C
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 0x22]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0767:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0767
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x60]
        call   sub_4_0283
        pop    edi
loc_4_077C:
        mov    eax, dword ptr [edi + 0x5c]
        or     eax, eax
        je     loc_4_07AA
        mov    esi, 0                        ; -> obj6:0x0122
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 0x42]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0795:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0795
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x5c]
        call   sub_4_0283
        pop    edi
loc_4_07AA:
        mov    eax, dword ptr [edi + 0x58]
        or     eax, eax
        je     loc_4_07D8
        mov    esi, 0                        ; -> obj6:0x0108
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_07C3:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_07C3
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x58]
        call   sub_4_0283
        pop    edi
loc_4_07D8:
        mov    eax, dword ptr [edi + 0x2c]
        or     eax, eax
        je     loc_4_07F3
        cli    
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        sti    
loc_4_07F3:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_07FF:
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
sub_4_081C:
        push   ebp
        mov    ebp, esp
        pushal 
        xor    eax, eax
        xor    ecx, ecx
        mov    al, byte ptr [ebp + 0xc]
loc_4_0827:
        cmp    byte ptr [ecx], 0xff          ; -> obj6:0x01BF
        je     loc_4_087B
        cmp    al, byte ptr [ecx]            ; -> obj6:0x01BF
        je     loc_4_083B
        inc    ecx
        jmp    loc_4_0827
loc_4_083B:
        mov    ah, byte ptr [ecx]            ; -> obj6:0x01C4
        and    ah, 0x3f
        xor    ecx, ecx
        mov    al, byte ptr [ebp + 0x10]
loc_4_0849:
        cmp    byte ptr [ecx], 0xff          ; -> obj6:0x01B8
        je     loc_4_087B
        cmp    al, byte ptr [ecx]            ; -> obj6:0x01B8
        je     loc_4_085D
        inc    ecx
        jmp    loc_4_0849
loc_4_085D:
        or     ah, byte ptr [ecx]            ; -> obj6:0x01BC
        mov    cx, word ptr [ebp + 0x10]
        cmp    cx, word ptr [ebp + 0x14]
        je     loc_4_0870
        or     ah, 4
loc_4_0870:
        mov    al, ah
        movzx  edx, word ptr [ebp + 8]
        out    dx, al
        clc    
loc_4_0878:
        popal  
        pop    ebp
        ret    
loc_4_087B:
        stc    
        jmp    loc_4_0878
sub_4_087E:
        push   ebp
        mov    ebp, esp
        pushal 
        movzx  edx, word ptr [ebp + 8]
        xor    al, al
        add    edx, 0xa
        out    dx, al
        mov    ecx, 0x3e8
        sub    edx, 0xa
loc_4_0894:
        mov    al, 0xa
        out    dx, al
        in     al, dx
        cmp    al, 0xa
        je     loc_4_089E
        loop   loc_4_0894
loc_4_089E:
        popal  
        pop    ebp
        ret    
sub_4_08A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        movzx  edx, word ptr [ebp + 0xc]
        push   4
        push   edx
        call   sub_1_026C
loc_4_08BC:
        add    esp, 8
        or     edi, edi
        jne    loc_4_0A23
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 0x30], 0x100
        jne    loc_4_09EB
        test   dword ptr [ebp + 0x30], 0x200
        jne    loc_4_09E0
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_4_0B7C
        jb     loc_4_0A29
        movsx  edx, word ptr [ebp + 0x18]
        cmp    edx, -1
        je     loc_4_090B
        call   sub_4_0DB9
        jb     loc_4_0A29
loc_4_090B:
        movsx  edx, word ptr [ebp + 0x10]
        cmp    edx, -1
        je     loc_4_0973
        test   dword ptr [ebp + 0x30], 0x400
        jne    loc_4_095D
        push   edx
        call   sub_4_0BC4
        add    esp, 4
        or     eax, eax
        je     loc_4_0A29
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x10]
        call   sub_4_0CE1
        add    esp, 8
        or     eax, eax
        je     loc_4_0A23
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x10]
        call   sub_4_0D3C
        add    esp, 0xc
        or     eax, eax
        je     loc_4_0A23
loc_4_095D:
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x10]
        call   sub_4_081C
        add    esp, 0x10
        jmp    loc_4_09EB
loc_4_0973:
        cmp    word ptr [ebp + 0x14], -1
        je     loc_4_09B7
        push   1
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        call   sub_4_0C0C
        add    esp, 0xc
        or     eax, eax
        je     loc_4_09AC
        or     dword ptr [ebp - 4], 0x10
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x14]
        call   sub_4_0C61
        add    esp, 0x10
        or     eax, eax
        je     loc_4_0A23
        jmp    loc_4_09EB
loc_4_09AC:
        call   sub_4_0D92
        jb     loc_4_09B7
        or     dword ptr [ebp - 4], 0x30
loc_4_09B7:
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0xc]
        call   sub_4_0F94
        add    esp, 8
        or     eax, eax
        je     loc_4_0A23
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0xc]
        call   sub_4_114C
        add    esp, 0xc
        or     eax, eax
        je     loc_4_0A23
        jmp    loc_4_09EB
loc_4_09E0:
        push   dword ptr [ebp + 0xc]
        call   sub_4_087E
        add    esp, 4
loc_4_09EB:
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0x30]
        push   dword ptr [ebp + 0x2c]
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_02AC
        add    esp, 0x30
        or     eax, eax
        je     loc_4_0A23
        mov    dword ptr [esp + 0x1c], 0
loc_4_0A23:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_0A29:
        mov    dword ptr [esp + 0x1c], 0x24
        jmp    loc_4_0A23
sub_4_0A33:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd 
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_026C
loc_4_0A44:
        add    esp, 8
        or     edi, edi
        je     loc_4_0AF2
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     loc_4_0A80
        mov    ebx, dword ptr [edi + 0x24]
        or     ebx, ebx
        je     loc_4_0A6F
        mov    eax, 1
        call   sub_5_119E
loc_4_0A6F:
        mov    ebx, dword ptr [edi + 0x30]
        or     ebx, ebx
        je     loc_4_0A80
        mov    eax, 2
        call   sub_5_119E
loc_4_0A80:
        cli    
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_4_0A8E
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
loc_4_0A8E:
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_4_0A9B
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
loc_4_0A9B:
        test   word ptr [edi + 0x52], 4
        je     loc_4_0AAA
        movzx  edx, word ptr [edi + 6]
        xor    eax, eax
        out    dx, al
loc_4_0AAA:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_0AF7
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_0AF7
loc_4_0ABF:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x54], ebx
        je     loc_4_0AD0
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_0AF7
        jmp    loc_4_0ABF
loc_4_0AD0:
        sti    
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0AD8:
        call   sub_4_017A
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0AD8
        pop    ebx
        mov    edi, eax
        call   sub_4_06C0
        je     loc_4_0AF2
loc_4_0AF2:
        mov    eax, 0xffffffff
loc_4_0AF7:
        popfd  
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_4_0AFD:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_0647
loc_4_0B14:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_0B1A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, esp
        push   0
        push   ebx
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003003300
        add    esp, 0xc
        cmp    eax, 0xd
        je     loc_4_0B64
        cmp    eax, 0
        je     loc_4_0B3B
        jmp    loc_4_0B76
loc_4_0B3B:
        push   edi
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        call   sub_1_026C
loc_4_0B48:
        add    esp, 8
        or     edi, edi
        pop    edi
        je     loc_4_0B64
        push   3
        push   eax
        push   0                             ; -> loc_4_11E4
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e003300
        add    esp, 0x10
        jmp    loc_4_0B76
loc_4_0B64:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_0AFD
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
loc_4_0B76:
        add    esp, 4
        pop    ebp
        stc    
        ret    
sub_4_0B7C:
        push   eax
        push   ecx
        push   edx
        xor    ecx, ecx
        in     al, dx
        test   al, 0x80
        jne    loc_4_0BBC
        mov    cl, al
        mov    al, 0xc
        out    dx, al
        inc    edx
        in     al, dx
        test   al, 0x80
        je     loc_4_0B93
        and    al, 0xbf
loc_4_0B93:
        cmp    al, 9
        je     loc_4_0BA5
        cmp    al, 0xa
        je     loc_4_0BA5
        cmp    al, 0x81
        je     loc_4_0BA5
        cmp    al, 0x8a
        je     loc_4_0BA5
        jmp    loc_4_0BB8
loc_4_0BA5:
        shl    ecx, 8
        mov    cl, al
        xor    al, al
        out    dx, al
        in     al, dx
        cmp    al, cl
        je     loc_4_0BBF
        mov    al, cl
        out    dx, al
        shr    ecx, 8
loc_4_0BB8:
        dec    edx
        mov    al, cl
        out    dx, al
loc_4_0BBC:
        stc    
        jmp    loc_4_0BC0
loc_4_0BBF:
        clc    
loc_4_0BC0:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_4_0BC4:
        push   ebp
        mov    ebp, esp
        push   edx
        movzx  edx, word ptr [ebp + 8]
        add    dx, 3
        in     al, dx
        and    al, 0x3f
        cmp    al, 4
        jne    loc_4_0BDE
        movzx  eax, al
        clc    
        jmp    loc_4_0BE1
        db  0C3h
loc_4_0BDE:
        xor    eax, eax
        stc    
loc_4_0BE1:
        pop    edx
        pop    ebp
        ret    
sub_4_0BE4:
        jmp    loc_4_0BEC
loc_4_0BE6:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
loc_4_0BEC:
        push   ecx
        push   edi
        push   esi
        mov    esi, 0xfffea
        lea    edi, [0]                      ; -> loc_4_0BE6
        mov    ecx, 6
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_0C07
        clc    
        jmp    loc_4_0C08
loc_4_0C07:
        stc    
loc_4_0C08:
        pop    esi
        pop    edi
        pop    ecx
        ret    
sub_4_0C0C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        call   sub_4_0BE4
        jb     loc_4_0C51
        movzx  edx, word ptr [ebp + 0xc]
        mov    ah, 0x10
        cmp    word ptr [ebp + 8], 0x608
        jne    loc_4_0C29
        or     ah, 0x20
loc_4_0C29:
        in     al, dx
        cmp    al, 0xff
        je     loc_4_0C36
        and    al, 0x30
        cmp    al, ah
        jne    loc_4_0C36
        jmp    loc_4_0C58
loc_4_0C36:
        movzx  eax, word ptr [ebp + 0x10]
        test   eax, 1
        jne    loc_4_0C51
        call   sub_4_0D92
        jb     loc_4_0C51
        add    edx, 4
        cmp    dx, word ptr [ebp + 8]
        je     loc_4_0C58
loc_4_0C51:
        mov    eax, 0
        jmp    loc_4_0C5D
loc_4_0C58:
        mov    eax, 0xffffffff
loc_4_0C5D:
        pop    edx
        pop    ebx
        pop    ebp
        ret    
sub_4_0C61:
        push   ebp
        mov    ebp, esp
        pushal 
        movzx  edx, word ptr [ebp + 8]
        add    edx, 3
        in     al, dx
        cmp    al, 0xff
        je     loc_4_0CD7
        mov    ah, al
        movzx  ecx, al
        shr    ecx, 4
        or     ecx, ecx
        jne    loc_4_0C7F
        jmp    loc_4_0CD7
loc_4_0C7F:
        inc    ecx
        and    ecx, 3
        mov    al, byte ptr [ecx]            ; -> obj6:0x01BF
        cmp    al, byte ptr [ebp + 0xc]
        jne    loc_4_0CD7
        movzx  ecx, ah
        shr    ecx, 6
        jecxz  loc_4_0CD7
        dec    ecx
        mov    al, byte ptr [ecx]            ; -> obj6:0x01B8
        cmp    al, byte ptr [ebp + 0x10]
        cmp    word ptr [ebp + 0x14], -1
        je     loc_4_0CCC
        cmp    al, byte ptr [ebp + 0x14]
        je     loc_4_0CCC
        sub    edx, 2
        in     al, dx
        cmp    al, 0xff
        je     loc_4_0CD7
        movzx  ecx, al
        shr    ecx, 6
        or     ecx, ecx
        je     loc_4_0CD7
        jecxz  loc_4_0CD7
        dec    ecx
        mov    al, byte ptr [ecx]            ; -> obj6:0x01B8
        cmp    al, byte ptr [ebp + 0x14]
        jne    loc_4_0CD7
loc_4_0CCC:
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_0CD4:
        popal  
        pop    ebp
        ret    
loc_4_0CD7:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_0CD4
sub_4_0CE1:
        push   ebp
        mov    ebp, esp
        pushfd 
        cli    
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0xc]
        xor    ecx, ecx
loc_4_0CF2:
        mov    ah, byte ptr [ecx]            ; -> obj6:0x01C4
        cmp    ah, 0xff
        je     loc_4_0D32
        cmp    byte ptr [ecx], al            ; -> obj6:0x01BF
        je     loc_4_0D08
        inc    ecx
        jmp    loc_4_0CF2
loc_4_0D08:
        xchg   ah, al
        out    dx, al
        add    dx, 3
        in     al, dx
        test   al, 0x40
        je     loc_4_0D32
        mov    edx, 1
        mov    cl, ah
        shl    edx, cl
        test   al, 0x80
        je     loc_4_0D27
        and    edx, 0x2ff
loc_4_0D27:
        or     edx, edx
        je     loc_4_0D32
        mov    eax, 0xffffffff
        jmp    loc_4_0D37
loc_4_0D32:
        mov    eax, 0
loc_4_0D37:
        pop    edx
        pop    ecx
        popfd  
        pop    ebp
        ret    
sub_4_0D3C:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0x10]
        mov    ah, byte ptr [ebp + 0xc]
        xor    ecx, ecx
loc_4_0D4D:
        cmp    byte ptr [ecx], 0xff          ; -> obj6:0x01B8
        je     loc_4_0D89
        cmp    byte ptr [ecx], ah            ; -> obj6:0x01B8
        je     loc_4_0D61
        inc    ecx
        jmp    loc_4_0D4D
loc_4_0D61:
        push   eax
        add    dx, 3
        in     al, dx
        test   al, 0x80
        pop    eax
        je     loc_4_0D70
        or     ah, ah
        je     loc_4_0D89
loc_4_0D70:
        cmp    al, ah
        je     loc_4_0D82
        or     ah, ah
        je     loc_4_0D7E
        or     al, al
        jne    loc_4_0D89
        jmp    loc_4_0D82
loc_4_0D7E:
        cmp    al, 1
        jne    loc_4_0D89
loc_4_0D82:
        mov    eax, 0xffffffff
        jmp    loc_4_0D8E
loc_4_0D89:
        mov    eax, 0
loc_4_0D8E:
        pop    edx
        pop    ecx
        pop    ebp
        ret    
sub_4_0D92:
        pushal 
        call   sub_4_0BE4
        jb     loc_4_0DB6
        push   edx
        mov    edx, 0xc7c
        in     al, dx
        pop    edx
        mov    bl, al
        mov    bh, bl
        and    bh, 0xf0
        cmp    bh, 0x10
        jne    loc_4_0DB6
        test   bl, 3
        jne    loc_4_0DB6
        popal  
        clc    
        ret    
loc_4_0DB6:
        popal  
        stc    
        ret    
sub_4_0DB9:
        push   eax
        push   ebx
        push   ecx
        pushfd 
        cli    
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0937
loc_4_0DCA:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_0937
loc_4_0DD6:
        mov    ebx, 2
        mov    al, 0x80
        call   sub_1_0937
loc_4_0DE2:
        mov    ebx, 4
        mov    al, 1
        call   sub_1_0937
loc_4_0DEE:
        mov    cx, 0x4000
loc_4_0DF2:
        in     al, dx
        test   al, 0x40
        jne    loc_4_0DF9
        loop   loc_4_0DF2
loc_4_0DF9:
        push   eax
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0937
loc_4_0E06:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_0937
loc_4_0E12:
        pop    eax
        popfd  
        test   al, 0x40
        je     loc_4_0E1F
        test   al, 6
        jne    loc_4_0E1F
        clc    
        jmp    loc_4_0E20
loc_4_0E1F:
        stc    
loc_4_0E20:
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_4_0E24:
        push   ebx
        mov    bh, 9
        mov    ah, 0xc
        call   sub_1_05E9
loc_4_0E2E:
        jb     loc_4_0E50
        mov    bh, al
        cmp    al, 0xa
        je     loc_4_0E54
        cmp    al, 0x8a
        jne    loc_4_0E50
        mov    al, 0x40
        mov    ah, 0xc
        call   sub_1_0607
loc_4_0E43:
        jmp    loc_4_0E45
loc_4_0E45:
        call   sub_1_05E9
loc_4_0E4A:
        test   al, 0x40
        jne    loc_4_0E58
        jmp    loc_4_0E54
loc_4_0E50:
        mov    bl, 0
        jmp    loc_4_0E5A
loc_4_0E54:
        mov    bl, 1
        jmp    loc_4_0E5A
loc_4_0E58:
        mov    bl, 2
loc_4_0E5A:
        movzx  eax, bx
        pop    ebx
        ret    
sub_4_0E5F:
        push   eax
        push   ecx
        push   edx
        mov    eax, 1
        call   sub_1_0710
loc_4_0E6C:
        mov    ah, 6
        call   sub_1_05E9
loc_4_0E73:
        shl    eax, 8
        push   eax
        mov    ah, 7
        call   sub_1_05E9
loc_4_0E7E:
        movzx  eax, al
        or     dword ptr [esp], eax
        mov    eax, 0x80
        mov    ah, 6
        call   sub_1_0607
loc_4_0E90:
        inc    ah
        call   sub_1_0607
loc_4_0E97:
        mov    byte ptr [0], 0x40            ; -> obj1:0x0114
        mov    eax, 0x4b
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    ah, 8
        call   sub_1_05E9
loc_4_0EB1:
        push   eax
        mov    al, 3
        call   sub_1_0607
loc_4_0EB9:
        xor    eax, eax
        mov    ah, 0xf
        call   sub_1_0607
loc_4_0EC2:
        dec    ah
        call   sub_1_0607
loc_4_0EC9:
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        or     al, 2
        call   sub_1_0607
loc_4_0ED7:
        mov    ah, 9
        mov    al, 0x41
        call   sub_1_0607
loc_4_0EE0:
        mov    eax, 0xb
        add    edx, 0
        out    dx, al
        mov    byte ptr [0], 0               ; -> obj1:0x0114
        call   sub_1_05D0
loc_4_0EF5:
        add    edx, 3
        xor    eax, eax
        out    dx, al
        sub    edx, 3
        add    edx, 2
        mov    ecx, 0x1000
loc_4_0F06:
        in     al, dx
        test   al, 1
        jne    loc_4_0F14
        loop   loc_4_0F06
        mov    ecx, 0x1f4
loc_4_0F12:
        loop   loc_4_0F12
loc_4_0F14:
        sub    edx, 2
        or     byte ptr [0], 0x40            ; -> obj1:0x0114
        mov    eax, 0xb
        or     al, byte ptr [0]              ; -> obj1:0x0114
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    ah, 9
        xor    al, al
        call   sub_1_0607
loc_4_0F39:
        pop    eax
        call   sub_1_0607
loc_4_0F3F:
        and    byte ptr [0], 0xbf            ; -> obj1:0x0114
        mov    eax, 0xb
        or     al, byte ptr [0]              ; -> obj1:0x0114
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    eax, dword ptr [esp]
        mov    ah, 7
        call   sub_1_0607
loc_4_0F62:
        pop    eax
        shr    eax, 8
        mov    ah, 6
        call   sub_1_0607
loc_4_0F6D:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_4_0F71:
        push   eax
        add    edx, 2
        in     al, dx
        sub    edx, 2
        test   al, 1
        je     loc_4_0F86
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_4_0F86:
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        call   sub_1_0607
loc_4_0F92:
        pop    eax
        ret    
sub_4_0F94:
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
        jmp    loc_4_0FC0
loc_4_0FC0:
        out    0x20, al
        jmp    loc_4_0FC4
loc_4_0FC4:
        call   sub_4_0F71
        in     al, 0xa0
        mov    ah, al
        in     al, 0x20
        test   ecx, eax
        jne    loc_4_0FEC
        call   sub_4_0E5F
        push   ecx
        mov    ecx, 0x1f4
loc_4_0FDE:
        in     al, dx
        loop   loc_4_0FDE
        pop    ecx
        in     al, 0xa0
        mov    ah, al
        in     al, 0x20
        test   ecx, eax
        jne    loc_4_0FF8
loc_4_0FEC:
        push   ecx
        push   eax
        call   sub_5_0051
loc_4_0FF3:
        add    esp, 8
        xor    eax, eax
loc_4_0FF8:
        call   sub_4_0F71
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
sub_4_1010:
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
        jne    loc_4_103F
        add    al, 0x48
        jmp    loc_4_1041
loc_4_103F:
        add    al, 0x44
loc_4_1041:
        out    0xb, al
        mov    edx, ebx
        shl    edx, 1
        add    edx, 0
        mov    eax, dword ptr [ebp + 0x10]
        out    dx, al
        shr    eax, 8
        out    dx, al
        movzx  ebx, word ptr [ebp + 0xc]
        movzx  edx, byte ptr [ebx]           ; -> obj6:0x029C
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
        mov    ah, 0xf
        mov    al, 0xf
        call   sub_1_0607
loc_4_1082:
        mov    ah, 0xe
        xor    al, al
        call   sub_1_0607
loc_4_108B:
        mov    ah, 6
        mov    al, 0x3f
        call   sub_1_0607
loc_4_1094:
        mov    ah, 7
        call   sub_1_0607
loc_4_109B:
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        call   sub_1_0607
loc_4_10A7:
        mov    byte ptr [0], 0x40            ; -> obj1:0x0114
        mov    eax, 0x4b
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    al, 1
        test   dword ptr [ebp + 0x14], 1
        je     loc_4_10C7
        mov    al, 2
loc_4_10C7:
        test   dword ptr [ebp + 0x14], 2
        je     loc_4_10DD
        or     al, 4
        test   dword ptr [ebp + 0x14], 1
        je     loc_4_10DD
        or     al, 0x40
loc_4_10DD:
        mov    ah, 9
        call   sub_1_0607
loc_4_10E4:
        mov    eax, 0xb
        add    edx, 0
        out    dx, al
        mov    byte ptr [0], 0               ; -> obj1:0x0114
        call   sub_1_05D0
loc_4_10F9:
        mov    edx, ebx
        shl    edx, 1
        add    edx, 1
        mov    ecx, 0xffff
loc_4_1105:
        xor    al, al
        out    0xc, al
        jmp    loc_4_110B
loc_4_110B:
        in     al, dx
        mov    ah, al
        jmp    loc_4_1110
loc_4_1110:
        in     al, dx
        cmp    ax, -1
        je     loc_4_112B
        loop   loc_4_1105
        push   eax
        push   ebx
        call   sub_5_007E
loc_4_1120:
        add    esp, 8
        mov    dword ptr [esp + 0x1c], 0
loc_4_112B:
        movzx  edx, word ptr [ebp + 8]
        mov    ah, 9
        xor    al, al
        call   sub_1_0607
loc_4_1138:
        mov    eax, ebx
        and    eax, 3
        or     eax, 4
        out    0xa, al
        add    edx, 2
        xor    al, al
        out    dx, al
        popal  
        popfd  
        pop    ebp
        ret    
sub_4_114C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        pushal 
        movzx  edx, word ptr [ebp + 8]
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
        je     loc_4_11C6
        mov    edi, edx
        mov    ecx, 0x400
        mov    eax, 0x80808080
        cld    
        rep stosd dword ptr es:[edi], eax
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_1010
        add    esp, 0x10
        or     eax, eax
        je     loc_4_11C6
        mov    ecx, 1
        mov    ax, word ptr [ebp + 0xc]
        cmp    ax, word ptr [ebp + 0x10]
        jne    loc_4_11B4
        or     ecx, 2
loc_4_11B4:
        push   ecx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_4_1010
        add    esp, 0x10
loc_4_11C6:
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [ebp - 8]
        or     eax, eax
        je     loc_4_11DD
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_11DD:
        popal  
        add    esp, 8
        pop    ebp
        ret    
        db  0CCh
loc_4_11E4:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_0BEE
loc_4_11FB:
        add    esp, 0x14
        pop    ebp
        clc    
        ret    
sub_4_1201:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, esp
        push   0
        push   dword ptr [ebp + 8]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003003300
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_4_1231
        mov    eax, dword ptr [esp]
        push   edi
        push   0
        push   eax
        call   sub_1_026C
loc_4_1229:
        add    esp, 8
        mov    eax, edi
        pop    edi
        jmp    loc_4_1233
loc_4_1231:
        xor    eax, eax
loc_4_1233:
        add    esp, 4
        leave  
        ret    
sub_4_1238:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_4_1201
        add    esp, 4
        or     eax, eax
        je     loc_4_129E
        mov    edi, eax
        mov    esi, dword ptr [ebp + 0x10]
        movsx  eax, word ptr [esi]
        cmp    eax, -1
        je     loc_4_127F
        mov    esi, 0                        ; -> loc_1_09DC
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [edi + 0xa8], eax
        jb     loc_4_129E
        push   ebx
        push   ecx
        xor    ebx, ebx
        mov    ecx, 1
        VxDCall  VDMAD.VDMAD_Set_Channel_Callbacks ; cd201f000400
        pop    ecx
        pop    ebx
        jmp    loc_4_1286
loc_4_127F:
        movzx  eax, byte ptr [edi + 9]
        mov    word ptr [esi], ax
loc_4_1286:
        movzx  eax, byte ptr [edi + 8]
        mov    esi, dword ptr [ebp + 0x14]
        mov    word ptr [esi], ax
        push   dword ptr [ebp + 0xc]
        push   edi
        call   sub_4_12A7
        add    esp, 8
        jmp    loc_4_12A3
loc_4_129E:
        mov    eax, 0x13
loc_4_12A3:
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_4_12A7:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, edi
        call   sub_1_0B50
loc_4_12B8:
        mov    word ptr [edi + 0x40], dx
        push   edi
        mov    esi, 0                        ; -> obj6:0x0318
        movzx  ecx, word ptr [esi]
        mov    edi, esi
        add    esi, 2
loc_4_12CA:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_12CA
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 0x64], eax
        or     eax, eax
        je     loc_4_12ED
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_12FB
loc_4_12ED:
        mov    word ptr [edi + 0x40], 0
        mov    dword ptr [esp + 0x1c], 0x13
loc_4_12FB:
        popal  
        leave  
        ret    
sub_4_12FE:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_4_1201
        add    esp, 4
        or     eax, eax
        je     loc_4_1332
        push   eax
        mov    eax, dword ptr [eax + 0xa8]
        or     eax, eax
        je     loc_4_1321
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_1321:
        pop    eax
        push   eax
        call   sub_4_1339
        add    esp, 4
        mov    eax, 0
        jmp    loc_4_1337
loc_4_1332:
        mov    eax, 0x13
loc_4_1337:
        pop    ebp
        ret    
sub_4_1339:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x64]
        or     eax, eax
        je     loc_4_1375
        mov    esi, 0                        ; -> obj6:0x0318
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 0x40]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_1359:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_1359
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x64]
        call   sub_4_0283
        pop    edi
        mov    dword ptr [edi + 0x64], 0
loc_4_1375:
        popal  
        leave  
        ret    
sub_4_1378:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_4_1201
        add    esp, 4
        or     eax, eax
        je     loc_4_139E
        test   word ptr [eax + 4], 0x4000
        je     loc_4_1399
        mov    eax, 0xffffffff
        jmp    loc_4_139E
loc_4_1399:
        mov    eax, 0
loc_4_139E:
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x23e5  R-X
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
        mov    ebp, esp
        sub    esp, 0x80
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   0                             ; -> obj6:0x000C
        lea    edx, [ebp - 0x80]
        push   edx
        call   sub_5_226C
        add    esp, 0x10
        lea    ecx, [ebp - 0x80]
        push   ecx
        call   0
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_007E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x80
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   0                             ; -> obj6:0x0028
        lea    edx, [ebp - 0x80]
        push   edx
        call   sub_5_226C
        add    esp, 0x10
        lea    ecx, [ebp - 0x80]
        push   ecx
        call   0
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_00AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x214
        push   ebx
        lea    eax, [ebp - 0x114]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        push   0
        push   0x100
        push   eax
        push   0
        push   ebx
        call   sub_5_22A4
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_027A
        mov    edi, 0                        ; -> obj6:0x005C
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0xffffffff
        lea    edi, [ebp - 0x114]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x114]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_2294
        add    esp, 0xc
        test   eax, eax
        je     loc_5_0132
        xor    eax, eax
        jmp    loc_5_027A
loc_5_0132:
        push   1
        lea    eax, [ebp - 0x214]
        push   0x100
        push   eax
        push   0
        push   ebx
        call   sub_5_22A4
        add    esp, 0x14
        mov    esi, eax
        test   esi, esi
        je     loc_5_0159
        push   dword ptr [ebp - 0xc]
        jmp    loc_5_0270
loc_5_0159:
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0x214]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_2294
        add    esp, 0xc
        test   eax, eax
        je     loc_5_017F
        mov    esi, 0x1d
        jmp    loc_5_0245
loc_5_017F:
        lea    eax, [ebp - 0x14]
        lea    ecx, [ebp - 1]
        mov    dword ptr [ebp - 0x14], 1
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj6:0x004C
        push   dword ptr [ebp - 0xc]
        call   sub_5_229C
        add    esp, 0x18
        test   eax, eax
        je     loc_5_01B0
        mov    esi, 0x1d
        jmp    loc_5_0245
loc_5_01B0:
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_5_021F
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj6:0x0044
        push   dword ptr [ebp - 0x10]
        call   sub_5_2294
        add    esp, 0xc
        test   eax, eax
        je     loc_5_01E2
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj6:0x0044
        push   dword ptr [ebp - 0x10]
        call   sub_5_228C
        add    esp, 0xc
loc_5_01E2:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_0281
        push   1
        lea    eax, [ebp - 1]
        mov    dword ptr [ebp - 0x14], 1
        push   eax
        push   3
        push   0
        push   0                             ; -> obj6:0x004C
        push   dword ptr [ebp - 8]
        call   sub_5_2284
        add    esp, 0x18
        push   dword ptr [ebp - 8]
        call   sub_5_2274
        mov    dword ptr [ebp - 8], 0
        add    esp, 4
loc_5_021F:
        push   dword ptr [ebp - 0xc]
        call   sub_5_2274
        add    esp, 4
        lea    eax, [ebp - 0x114]
        mov    dword ptr [ebp - 0xc], 0
        push   eax
        push   0x80000002
        call   sub_5_227C
        add    esp, 8
loc_5_0245:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_0256
        push   dword ptr [ebp - 0xc]
        call   sub_5_2274
        add    esp, 4
loc_5_0256:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_5_0267
        push   dword ptr [ebp - 0x10]
        call   sub_5_2274
        add    esp, 4
loc_5_0267:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_0278
        push   dword ptr [ebp - 8]
loc_5_0270:
        call   sub_5_2274
        add    esp, 4
loc_5_0278:
        mov    eax, esi
loc_5_027A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_0281:
        mov    esi, 0x1d
        jmp    loc_5_0245
sub_5_0288:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 1]
        mov    dword ptr [ebp - 8], 1
        push   ecx
        push   3
        push   0                             ; -> obj6:0x004C
        push   0                             ; -> obj6:0x0044
        push   dword ptr [ebp + 8]
        call   sub_5_22AC
        add    esp, 0x1c
        test   eax, eax
        jne    loc_5_02C3
        cmp    byte ptr [ebp - 1], 1
        sbb    eax, eax
        inc    eax
        jmp    loc_5_02C5
loc_5_02C3:
        xor    eax, eax
loc_5_02C5:
        test   eax, eax
        je     loc_5_02CF
        mov    eax, dword ptr [ebp + 0xc]
        or     byte ptr [eax], 4
loc_5_02CF:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_02D3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xd8
        push   esi
        lea    ecx, [ebp - 0xd8]
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_5_22B4
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_043B
        movzx  eax, word ptr [ebp - 0x7c]
        cmp    eax, 1
        je     loc_5_0316
        cmp    eax, 2
        je     loc_5_0351
        cmp    eax, 3
        je     loc_5_03BC
        jmp    loc_5_0436
loc_5_0316:
        movzx  eax, word ptr [ebp - 0x52]
        cmp    eax, 4
        je     loc_5_0329
        cmp    eax, 8
        je     loc_5_0338
        jmp    loc_5_0436
loc_5_0329:
        mov    ax, word ptr [ebp - 0x7a]
        mov    esi, dword ptr [ebp + 0x14]
        mov    word ptr [esi], ax
        jmp    loc_5_03EF
loc_5_0338:
        mov    ax, word ptr [ebp - 0x7a]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ecx], ax
        mov    esi, dword ptr [ebp + 0x14]
        add    ax, 4
        mov    word ptr [esi], ax
        jmp    loc_5_03EF
loc_5_0351:
        movzx  eax, word ptr [ebp - 0x52]
        cmp    eax, 4
        je     loc_5_0364
        cmp    eax, 8
        je     loc_5_039C
        jmp    loc_5_0436
loc_5_0364:
        mov    ax, word ptr [ebp - 0x7a]
        mov    esi, dword ptr [ebp + 0x14]
        mov    word ptr [esi], ax
        mov    edi, dword ptr [ebp + 0x10]
        mov    ax, word ptr [ebp - 0x78]
        push   0
        mov    word ptr [edi], ax
        push   eax
        mov    cx, word ptr [esi]
        push   ecx
        call   sub_4_0C0C
loc_5_0384:
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_03EF
        mov    ax, word ptr [ebp - 0x78]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [edi], 0xffff
        mov    word ptr [ecx], ax
        jmp    loc_5_03EF
loc_5_039C:
        mov    ax, word ptr [ebp - 0x7a]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dx, word ptr [ebp - 0x78]
        mov    esi, dword ptr [ebp + 0x14]
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 0x18]
        add    ax, 4
        mov    word ptr [esi], ax
        mov    word ptr [ecx], dx
        jmp    loc_5_03EF
loc_5_03BC:
        cmp    word ptr [ebp - 0x52], 4
        jne    loc_5_0436
        cmp    word ptr [ebp - 0x50], 4
        jne    loc_5_0436
        cmp    word ptr [ebp - 0x4e], 0x10
        jne    loc_5_0436
        mov    ax, word ptr [ebp - 0x7a]
        mov    esi, dword ptr [ebp + 0x14]
        mov    dx, word ptr [ebp - 0x78]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    di, word ptr [ebp - 0x76]
        mov    word ptr [esi], ax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    word ptr [ecx], dx
        mov    word ptr [eax], di
loc_5_03EF:
        cmp    word ptr [ebp - 0x2a], 1
        jb     loc_5_0436
        cmp    word ptr [ebp - 0x1a], 1
        jb     loc_5_0436
        mov    eax, dword ptr [ebp + 0x20]
        movzx  cx, byte ptr [ebp - 0x28]
        mov    word ptr [eax], cx
        mov    eax, dword ptr [ebp + 0x24]
        movzx  cx, byte ptr [ebp - 0x18]
        mov    word ptr [eax], cx
        jbe    loc_5_0422
        mov    ecx, dword ptr [ebp + 0x28]
        movzx  dx, byte ptr [ebp - 0x17]
        mov    word ptr [ecx], dx
        jmp    loc_5_0428
loc_5_0422:
        mov    edx, dword ptr [ebp + 0x28]
        mov    word ptr [edx], cx
loc_5_0428:
        movzx  eax, word ptr [esi]
        inc    eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x13
        jmp    loc_5_043B
loc_5_0436:
        mov    eax, 0x13
loc_5_043B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0441:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        lea    ecx, [ebp - 0xc]
        push   0
        lea    edx, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], 4
        push   eax
        push   ecx
        push   edx
        push   dword ptr [ebp + 8]
        call   sub_5_22BC
        mov    cx, word ptr [ebp - 8]
        add    esp, 0x14
        mov    word ptr [ebp - 0x38], cx
        lea    ecx, [ebp - 0x38]
        mov    dword ptr [ebp - 4], ecx
        push   ebx
        push   ecx
        push   esi
        mov    eax, 4
        mov    ebx, dword ptr [ebp - 4]
        mov    ecx, 0x25
        xor    edx, edx
        xor    esi, esi
        VxDCall  Dev_097C.Service_0001h          ; cd2001007c09
        pop    esi
        pop    ecx
        pop    ebx
        test   ax, ax
        je     loc_5_04A0
        mov    eax, 0x13
        jmp    loc_5_0517
loc_5_04A0:
        mov    ax, word ptr [ebp - 0x22]
        and    eax, 0xf
        cmp    eax, 5
        je     loc_5_04B3
        cmp    eax, 0xb
        je     loc_5_04FD
        jmp    loc_5_0515
loc_5_04B3:
        mov    ax, word ptr [ebp - 0x22]
        and    eax, 0xf000
        cmp    eax, 0xe000
        je     loc_5_04D8
        cmp    eax, 0xf000
        mov    eax, dword ptr [ebp + 0x10]
        je     loc_5_04F2
        test   eax, eax
        je     loc_5_0509
        mov    word ptr [eax], 3
        jmp    loc_5_0509
loc_5_04D8:
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_5_04E4
        mov    word ptr [eax], 1
loc_5_04E4:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_0515
        mov    word ptr [eax], 4
        jmp    loc_5_0515
loc_5_04F2:
        test   eax, eax
        je     loc_5_0509
        mov    word ptr [eax], 2
        jmp    loc_5_0509
loc_5_04FD:
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_5_0509
        mov    word ptr [eax], 0x100
loc_5_0509:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_0515
        mov    word ptr [eax], 0xfffc
loc_5_0515:
        xor    eax, eax
loc_5_0517:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_051D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xdc
        push   0
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_5_22C4
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_0560
        push   0
        lea    ecx, [ebp - 0xdc]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_5_22B4
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_0560
        cmp    word ptr [ebp - 0x80], 2
        jne    loc_5_0560
        movzx  eax, word ptr [ebp - 0x7c]
        jmp    loc_5_0565
loc_5_0560:
        mov    eax, 0xffffffff
loc_5_0565:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0569:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xdc
        push   esi
        lea    eax, [ebp - 0xdc]
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_5_22B4
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_0642
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_5_0441
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_0642
        movzx  eax, word ptr [ebp - 0x80]
        cmp    eax, 1
        je     loc_5_05BF
        cmp    eax, 2
        je     loc_5_05F5
        mov    eax, 0x13
        jmp    loc_5_0642
loc_5_05BF:
        movzx  eax, word ptr [ebp - 0x56]
        cmp    eax, 0x10
        je     loc_5_05CF
        mov    eax, 0x13
        jmp    loc_5_0642
loc_5_05CF:
        mov    ax, word ptr [ebp - 0x7e]
        mov    ecx, dword ptr [ebp + 0xc]
        add    ax, 4
        cmp    word ptr [ebp - 2], 0x100
        mov    word ptr [ecx], ax
        jne    loc_5_061D
        mov    ax, word ptr [ebp - 0x7e]
        mov    edx, dword ptr [ebp + 0x10]
        add    ax, 8
        mov    word ptr [edx], ax
        jmp    loc_5_061D
loc_5_05F5:
        movzx  eax, word ptr [ebp - 0x56]
        cmp    eax, 4
        je     loc_5_0605
        mov    eax, 0x13
        jmp    loc_5_0642
loc_5_0605:
        mov    ax, word ptr [ebp - 0x7c]
        mov    ecx, dword ptr [ebp + 0xc]
        add    ax, 4
        mov    edx, dword ptr [ebp + 0x10]
        mov    si, word ptr [ebp - 0x7e]
        mov    word ptr [ecx], ax
        mov    word ptr [edx], si
loc_5_061D:
        cmp    word ptr [ebp - 0x2e], 1
        jae    loc_5_062B
        mov    eax, 0x13
        jmp    loc_5_0642
loc_5_062B:
        mov    eax, dword ptr [ebp + 0x14]
        movzx  dx, byte ptr [ebp - 0x2c]
        mov    word ptr [eax], dx
        movzx  eax, word ptr [ecx]
        inc    eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x13
loc_5_0642:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0647:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x174
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_5_0696
        cmp    eax, 1
        je     loc_5_07E6
        cmp    eax, 2
        je     loc_5_09C8
        cmp    eax, 3
        je     loc_5_0A15
        cmp    eax, 4
        je     loc_5_09C8
        cmp    eax, 8
        je     loc_5_0A59
        cmp    eax, 0xd
        je     loc_5_09C8
        jmp    loc_5_0A8C
loc_5_0696:
        mov    di, 0xffff
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 0x54]
        push   ebx
        lea    ecx, [ebp - 0x30]
        push   eax
        mov    esi, dword ptr [ebp + 0x10]
        push   ecx
        mov    dword ptr [ebp - 0x1c], ebx
        push   esi
        mov    dword ptr [ebp - 0x54], ebx
        call   sub_5_22BC
        add    esp, 0x14
        test   eax, eax
        je     loc_5_06BF
        mov    dword ptr [ebp - 0x30], ebx
loc_5_06BF:
        cmp    dword ptr [ebp - 0x30], 1
        je     loc_5_06D2
        cmp    dword ptr [ebp - 0x30], 0
        je     loc_5_06D2
        xor    eax, eax
        jmp    loc_5_0A91
loc_5_06D2:
        push   1
        lea    eax, [ebp - 0x24]
        push   esi
        push   eax
        call   sub_5_22E4
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_06EE
        mov    eax, ebx
        jmp    loc_5_0A91
loc_5_06EE:
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x14]
        push   0
        push   dword ptr [ebp - 0x24]
        push   edx
        call   sub_5_22DC
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_07CE
        xor    esi, esi
        test   ebx, ebx
        jne    loc_5_0759
loc_5_0713:
        mov    eax, dword ptr [ebp - 0x18]
        cmp    eax, 2
        je     loc_5_0722
        cmp    eax, 4
        je     loc_5_0739
        jmp    loc_5_073C
loc_5_0722:
        mov    eax, dword ptr [ebp - 0x14]
        cmp    word ptr [eax + 0xe], 8
        jne    loc_5_073C
        mov    dword ptr [ebp - 0x1c], 1
        mov    di, word ptr [eax + 0x10]
        jmp    loc_5_073C
loc_5_0739:
        mov    esi, dword ptr [ebp - 0x14]
loc_5_073C:
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x14]
        push   0
        push   dword ptr [ebp - 0x14]
        push   edx
        call   sub_5_22DC
        add    esp, 0x14
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_0713
loc_5_0759:
        cmp    ebx, 0xf
        jne    loc_5_07D8
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_5_07DF
        test   esi, esi
        je     loc_5_07DF
        push   edi
        call   sub_4_0BC4
loc_5_076E:
        add    esp, 4
        test   eax, eax
        je     loc_5_07DF
        xor    ebx, ebx
loc_5_0777:
        mov    edx, 1
        mov    cl, bl
        shl    edx, cl
        movzx  eax, word ptr [esi + 4]
        test   edx, eax
        je     loc_5_07A6
        push   ebx
        push   edi
        call   sub_4_0CE1
loc_5_078F:
        add    esp, 8
        test   eax, eax
        jne    loc_5_07A6
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        not    ax
        and    word ptr [esi + 4], ax
loc_5_07A6:
        inc    ebx
        cmp    ebx, 0x10
        jl     loc_5_0777
        push   0
        lea    ecx, [ebp - 0x24]
        push   dword ptr [ebp - 0x24]
        push   ecx
        call   sub_5_22D4
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_06EE
        xor    eax, eax
        jmp    loc_5_0A91
loc_5_07CE:
        mov    eax, 0x13
        jmp    loc_5_0A91
loc_5_07D8:
        mov    eax, ebx
        jmp    loc_5_0A91
loc_5_07DF:
        xor    eax, eax
        jmp    loc_5_0A91
loc_5_07E6:
        push   0
        lea    eax, [ebp - 0x174]
        push   0x100
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x28], 0
        push   eax
        push   0
        push   esi
        call   sub_5_22A4
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_0A91
        xor    edi, edi
        lea    eax, [ebp - 0x50]
        push   edi
        lea    ecx, [ebp - 0x2c]
        push   edi
        mov    dword ptr [ebp - 0x50], edi
        push   eax
        push   ecx
        push   esi
        call   sub_5_22BC
        add    esp, 0x14
        test   eax, eax
        je     loc_5_0830
        mov    dword ptr [ebp - 0x2c], edi
loc_5_0830:
        mov    eax, 0xffff
        mov    word ptr [ebp - 0xc], ax
        mov    word ptr [ebp - 0xe], ax
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 2], ax
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 0x2c]
        cmp    eax, 8
        je     loc_5_085D
        cmp    eax, 0x10
        je     loc_5_087F
        cmp    eax, 0x40
        je     loc_5_087F
        jmp    loc_5_0897
loc_5_085D:
        lea    eax, [ebp - 6]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 8]
        or     byte ptr [ebp - 0x27], 2
        push   ecx
        push   edx
        push   esi
        call   sub_5_0569
        add    esp, 0x10
        test   eax, eax
        je     loc_5_08C8
        jmp    loc_5_0A91
loc_5_087F:
        mov    eax, dword ptr [ebp - 0x2c]
        sub    eax, 0x10
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xfffffd00
        add    eax, 0x400
        or     dword ptr [ebp - 0x28], eax
loc_5_0897:
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xa]
        push   eax
        lea    edx, [ebp - 6]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        lea    eax, [ebp - 0xe]
        push   edx
        lea    ecx, [ebp - 0xc]
        push   eax
        push   ecx
        push   esi
        call   sub_5_02D3
        add    esp, 0x24
        test   eax, eax
        jne    loc_5_0A91
loc_5_08C8:
        push   esi
        call   sub_5_00AB
        add    esp, 4
        test   eax, eax
        jne    loc_5_0A91
        lea    eax, [ebp - 0x28]
        push   eax
        push   esi
        call   sub_5_0288
        add    esp, 8
        push   dword ptr [ebp - 0x28]
        push   0x20
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xa]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 2]
        push   dword ptr [ebp - 0xe]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_4_08A1
loc_5_0909:
        add    esp, 0x2c
        mov    edi, eax
        test   edi, edi
        jne    loc_5_09C1
        cmp    word ptr [ebp - 4], 0xffff
        jne    loc_5_0932
        push   1
        push   0                             ; -> loc_5_0A98
        push   esi
        call   sub_5_22CC
        add    esp, 0xc
        mov    edi, eax
        jmp    loc_5_0980
loc_5_0932:
        cmp    word ptr [ebp - 6], 9
        mov    eax, 2
        je     loc_5_0942
        movzx  eax, word ptr [ebp - 6]
loc_5_0942:
        movzx  ecx, word ptr [ebp - 0xa]
        movzx  edx, word ptr [ebp - 4]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 0x74]
        push   0                             ; -> obj6:0x0074
        push   ecx
        call   sub_5_226C
        add    esp, 0x14
        lea    ecx, [ebp - 0x74]
        mov    dword ptr [ebp - 0x48], 0     ; -> obj6:0x006C
        mov    dword ptr [ebp - 0x4c], ecx
        push   dword ptr [ebp - 0x4c]
        push   dword ptr [ebp - 0x48]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_5_0980:
        mov    dword ptr [ebp - 0x40], 0     ; -> obj6:0x0064
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [ebp - 0x3c], esi
        sub    ax, 0xffff
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 0x44], eax
        push   dword ptr [ebp - 0x44]
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0001h              ; cd2001004a04
        add    esp, 0xc
        test   edi, edi
        jne    loc_5_09C1
        push   esi
        call   sub_5_1ED8
        add    esp, 4
loc_5_09C1:
        mov    eax, edi
        jmp    loc_5_0A91
loc_5_09C8:
        lea    eax, [ebp - 0x38]
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        call   sub_4_0075
loc_5_09D5:
        add    esp, 8
        test   eax, eax
        je     loc_5_0A8C
        test   byte ptr [ebp - 0x37], 0x10
        je     loc_5_09FF
        mov    dword ptr [ebp - 0x34], 0     ; -> obj6:0x006C
        push   dword ptr [ebp - 0x34]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0004h              ; cd2004004a04
        add    esp, 4
loc_5_09FF:
        push   esi
        call   sub_5_1FB3
        add    esp, 4
        push   esi
        call   sub_4_0A33
loc_5_0A0E:
        add    esp, 4
        xor    eax, eax
        jmp    loc_5_0A91
loc_5_0A15:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_0A23
        cmp    eax, 1
        je     loc_5_0A3E
        jmp    loc_5_0A8C
loc_5_0A23:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_0A2B:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_0A91
loc_5_0A3E:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_0A46:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_0A91
loc_5_0A59:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x7fffffff
        cmp    eax, 4
        jb     loc_5_0A8C
        cmp    eax, 5
        jbe    loc_5_0A77
        cmp    eax, 6
        jb     loc_5_0A8C
        cmp    eax, 8
        jbe    loc_5_0A81
        jmp    loc_5_0A8C
loc_5_0A77:
        push   dword ptr [ebp + 0x10]
        call   sub_3_0062
loc_5_0A7F:
        jmp    loc_5_0A89
loc_5_0A81:
        push   dword ptr [ebp + 0x10]
        call   sub_3_00E1
loc_5_0A89:
        add    esp, 4
loc_5_0A8C:
        mov    eax, 1
loc_5_0A91:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_0A98:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        cmp    dword ptr [ebp + 8], 5
        push   ebx
        push   esi
        jne    loc_5_0BE3
        push   0
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_5_2304
        add    esp, 0xc
        test   eax, eax
        je     loc_5_0BE3
        mov    ebx, 1
        lea    eax, [ebp - 0x1c]
        push   ebx
        lea    ecx, [ebp - 1]
        push   eax
        mov    dword ptr [ebp - 0x1c], ebx
        push   ecx
        push   3
        push   0                             ; -> obj6:0x009C
        push   0                             ; -> obj6:0x0044
        push   esi
        call   sub_5_22AC
        add    esp, 0x1c
        test   eax, eax
        je     loc_5_0AF1
        mov    byte ptr [ebp - 1], bl
loc_5_0AF1:
        cmp    byte ptr [ebp - 1], 0
        je     loc_5_0BE3
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        call   sub_4_0030
loc_5_0B05:
        add    esp, 8
        test   eax, eax
        je     loc_5_0BE3
        push   0
        lea    eax, [ebp - 0x14]
        push   esi
        push   0                             ; -> obj6:0x0084
        push   eax
        call   sub_5_22FC
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_0BE8
        push   0
        lea    ecx, [ebp - 0xc]
        push   0x3000
        push   dword ptr [ebp - 0x14]
        push   ecx
        call   sub_5_22F4
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_0BE8
        mov    word ptr [ebp - 0x34], 1
        mov    word ptr [ebp - 0x32], 0xc
        mov    word ptr [ebp - 0x28], 0xfff0
        mov    word ptr [ebp - 0x26], 0x10
        mov    word ptr [ebp - 0x24], 0x220
        mov    word ptr [ebp - 0x22], 0x24f
        xor    ebx, ebx
        mov    word ptr [ebp - 0x30], bx
        mov    byte ptr [ebp - 0x2a], bl
        mov    word ptr [ebp - 0x2e], bx
        mov    byte ptr [ebp - 0x29], bl
        mov    word ptr [ebp - 0x2c], bx
        push   ebx
        mov    word ptr [ebp - 0x20], bx
        push   0x18
        lea    eax, [ebp - 0x34]
        lea    edx, [ebp - 0x18]
        push   eax
        mov    byte ptr [ebp - 0x1e], bl
        push   2
        mov    byte ptr [ebp - 0x1d], bl
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_5_22EC
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_0BE8
        test   byte ptr [ebp - 0x10], 1
        je     loc_5_0BDF
        test   byte ptr [ebp - 0x10], 0x10
        je     loc_5_0BDF
        mov    byte ptr [ebp - 6], 2
        xor    ebx, ebx
        lea    eax, [ebp - 8]
        push   ebx
        mov    byte ptr [ebp - 8], bl
        push   4
        lea    edx, [ebp - 0x18]
        push   eax
        mov    byte ptr [ebp - 7], bl
        push   3
        mov    byte ptr [ebp - 5], bl
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_5_22EC
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_0BE8
loc_5_0BDF:
        xor    eax, eax
        jmp    loc_5_0BE8
loc_5_0BE3:
        mov    eax, 1
loc_5_0BE8:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0BEE:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x108
        test   eax, eax
        push   esi
        push   edi
        je     loc_5_0C2B
        cmp    eax, 1
        je     loc_5_0C32
        cmp    eax, 2
        je     loc_5_0CF1
        cmp    eax, 3
        je     loc_5_0D19
        cmp    eax, 4
        je     loc_5_0CF1
        mov    eax, 1
        jmp    loc_5_0D45
loc_5_0C2B:
        xor    eax, eax
        jmp    loc_5_0D45
loc_5_0C32:
        push   0
        lea    ecx, [ebp - 0x108]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_5_22B4
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_0D45
        mov    eax, 0xffff
        cmp    word ptr [ebp - 0xac], 0
        mov    word ptr [ebp - 2], ax
        mov    word ptr [ebp - 4], ax
        jne    loc_5_0C6F
        mov    eax, 0x13
        jmp    loc_5_0D45
loc_5_0C6F:
        cmp    word ptr [ebp - 0x4a], 0
        mov    di, word ptr [ebp - 0xaa]
        je     loc_5_0C86
        movzx  ax, byte ptr [ebp - 0x48]
        mov    word ptr [ebp - 2], ax
loc_5_0C86:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 2]
        push   eax
        push   ecx
        push   edi
        push   dword ptr [ebp + 0x10]
        call   sub_4_1238
loc_5_0C97:
        add    esp, 0x10
        mov    esi, eax
        test   esi, esi
        jne    loc_5_0CED
        cmp    word ptr [ebp - 4], 9
        mov    eax, 2
        je     loc_5_0CB0
        movzx  eax, word ptr [ebp - 4]
loc_5_0CB0:
        movzx  ecx, word ptr [ebp - 2]
        movzx  edx, di
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 0x30]
        push   0                             ; -> obj6:0x0074
        push   ecx
        call   sub_5_226C
        add    esp, 0x14
        lea    ecx, [ebp - 0x30]
        mov    dword ptr [ebp - 0xc], 0      ; -> obj6:0x006C
        mov    dword ptr [ebp - 0x10], ecx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_5_0CED:
        mov    eax, esi
        jmp    loc_5_0D45
loc_5_0CF1:
        push   dword ptr [ebp + 0x10]
        call   sub_4_12FE
loc_5_0CF9:
        mov    dword ptr [ebp - 8], 0        ; -> obj6:0x006C
        add    esp, 4
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0004h              ; cd2004004a04
        add    esp, 4
        xor    eax, eax
        jmp    loc_5_0D45
loc_5_0D19:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_0D2C
        cmp    eax, 1
        je     loc_5_0D2C
        mov    eax, 1
        jmp    loc_5_0D45
loc_5_0D2C:
        push   dword ptr [ebp + 0x10]
        call   sub_4_1378
loc_5_0D34:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
loc_5_0D45:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh
loc_5_0D4C:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_0D56
        jmp    loc_5_0D68
loc_5_0D56:
        cmp    ecx, 4
        jbe    loc_5_0D61
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0D61:
        je     loc_5_0D64
        in     al, dx
loc_5_0D64:
        ret    
        db  02Eh, 08Bh, 0C0h
loc_5_0D68:
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        cmp    eax, ebx
        je     loc_5_0D88
        or     eax, eax
        jne    loc_5_0D9B
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_123C
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0D9B
loc_5_0D88:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0D94
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0D94:
        je     loc_5_0D99
        in     al, dx
        jmp    loc_5_0D9A
loc_5_0D99:
        out    dx, al
loc_5_0D9A:
        ret    
loc_5_0D9B:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_1069
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
loc_5_0DAC:
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        cmp    eax, ebx
        je     loc_5_0DCC
        or     eax, eax
        jne    loc_5_0DDF
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_123C
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0DDF
loc_5_0DCC:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0DD8
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0DD8:
        je     loc_5_0DDD
        in     al, dx
        jmp    loc_5_0DDE
loc_5_0DDD:
        out    dx, al
loc_5_0DDE:
        ret    
loc_5_0DDF:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_1069
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_0DF0:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 0x22]
        mov    ebx, 0x105
        mov    al, 1
        call   sub_1_0937
loc_5_0E04:
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0937
loc_5_0E10:
        mov    ebx, 0x104
        mov    al, 0x3f
        call   sub_1_0937
loc_5_0E1C:
        mov    ebx, 8
        xor    al, al
        call   sub_1_0937
loc_5_0E28:
        mov    ebx, 0xbd
        mov    al, 0xc0
        call   sub_1_0937
loc_5_0E34:
        mov    ecx, 0x15
        mov    ebx, 0x40
        mov    al, 0x3f
loc_5_0E40:
        xor    bh, bh
        call   sub_1_0937
loc_5_0E47:
        mov    bh, 1
        call   sub_1_0937
loc_5_0E4E:
        inc    bl
        loop   loc_5_0E40
        mov    ecx, 8
        mov    ebx, 0xb0
        xor    eax, eax
loc_5_0E5E:
        xor    bh, bh
        call   sub_1_0937
loc_5_0E65:
        mov    bh, 1
        call   sub_1_0937
loc_5_0E6C:
        inc    bl
        loop   loc_5_0E5E
        mov    ebx, 0x104
        xor    eax, eax
        call   sub_1_0937
loc_5_0E7C:
        mov    ebx, 0x105
        call   sub_1_0937
loc_5_0E86:
        call   sub_1_092E
loc_5_0E8B:
        call   sub_1_092E
loc_5_0E90:
        call   sub_1_092E
loc_5_0E95:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret    
loc_5_0E9A:
        mov    edi, edx
        mov    ebx, dword ptr [edi + 0x30]
        or     ebx, ebx
        je     loc_5_0EAE
        movzx  edx, word ptr [edi + 0x22]
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        ret    
loc_5_0EAE:
        ret    
        db  090h
loc_5_0EB0:
        test   word ptr [esi + 4], 0x2000
        je     loc_5_0ED8
        cmp    ecx, 4
        jbe    loc_5_0EC3
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0EC3:
        je     loc_5_0F0E
        and    word ptr [esi + 4], 0xdfff
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        mov    al, byte ptr [esi + 0xac]
        ret    
loc_5_0ED8:
        push   eax
        mov    eax, dword ptr [esi + 0x30]
        cmp    eax, ebx
        je     loc_5_0EFD
        or     eax, eax
        jne    loc_5_0F10
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_123C
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0F10
        call   sub_5_0DF0
loc_5_0EFD:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0F09
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0F09:
        je     loc_5_0F0E
        in     al, dx
        jmp    loc_5_0F0F
loc_5_0F0E:
        out    dx, al
loc_5_0F0F:
        ret    
loc_5_0F10:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_1069
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_0F21:
        push   esi
        push   eax
        push   ecx
        push   edx
        test   word ptr [edi + 0x52], 4
        je     loc_5_0F48
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0F37:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_0F48
        inc    cx
        jmp    loc_5_0F37
loc_5_0F48:
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x42]
loc_5_0F52:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_0F63
        inc    cx
        jmp    loc_5_0F52
loc_5_0F63:
        test   word ptr [edi + 4], 0x1000
        je     loc_5_0F86
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x40]
loc_5_0F75:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_0F86
        inc    cx
        jmp    loc_5_0F75
loc_5_0F86:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0F8C:
        push   esi
        push   eax
        push   ecx
        push   edx
        test   word ptr [edi + 0x52], 4
        je     loc_5_0FBB
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_0FBB
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0FAA:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0FBB
        inc    cx
        jmp    loc_5_0FAA
loc_5_0FBB:
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x42]
        add    edx, 0
loc_5_0FC8:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0FD9
        inc    cx
        jmp    loc_5_0FC8
loc_5_0FD9:
        test   word ptr [edi + 4], 0x1000
        je     loc_5_0FFC
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x40]
loc_5_0FEB:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0FFC
        inc    cx
        jmp    loc_5_0FEB
loc_5_0FFC:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_1002:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 0x22]
        mov    ebx, 4
        mov    eax, 0x60
        call   sub_1_0937
loc_5_1018:
        call   sub_1_092E
loc_5_101D:
        pop    edx
        pop    ebx
        pop    eax
        ret    
sub_5_1021:
        push   esi
        push   eax
        push   ecx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x22]
loc_5_102E:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_103F
        inc    cx
        jmp    loc_5_102E
loc_5_103F:
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_1044:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x22]
loc_5_1052:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_1063
        inc    cx
        jmp    loc_5_1052
loc_5_1063:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_1069:
        test   word ptr [edi + 4], 1
        jne    loc_5_10BE
        call   sub_1_0314
loc_5_1076:
        jb     loc_5_10BE
        cmp    eax, 2
        je     loc_5_1097
        test   dword ptr [esi], 1
        jne    loc_5_10BE
        or     dword ptr [esi], 1
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        jmp    loc_5_10AF
loc_5_1097:
        test   dword ptr [esi], 2
        jne    loc_5_10BE
        or     dword ptr [esi], 2
        movzx  ecx, word ptr [0xe]           ; -> loc_2_000E
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
loc_5_10AF:
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_5_10BE:
        ret    
loc_5_10BF:
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_110D
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_110D
loc_5_10D3:
        popfd  
        pushfd 
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 0x24], ebx
        jne    loc_5_10E9
        or     eax, 1
        mov    dword ptr [edi + 0x28], 0xffffffff
loc_5_10E9:
        cmp    dword ptr [edi + 0x30], ebx
        jne    loc_5_10F8
        or     eax, 2
        mov    dword ptr [edi + 0x34], 0xffffffff
loc_5_10F8:
        or     eax, eax
        je     loc_5_1101
        call   sub_5_119E
loc_5_1101:
        pop    eax
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_110D
        jmp    loc_5_10D3
loc_5_110D:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_5_1122
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_1122:
        popfd  
        clc    
        ret    
loc_5_1125:
        pushal 
        pushfd 
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    eax, 5
        mov    ecx, 0x34
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_5_1196
        mov    dword ptr [edi + ebx], esi
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_116B
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        sti    
        je     loc_5_116B
loc_5_1156:
        mov    edi, dword ptr [eax]
        call   sub_4_0110
loc_5_115D:
        jb     loc_5_1183
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        sti    
        je     loc_5_116B
        jmp    loc_5_1156
loc_5_116B:
        mov    eax, dword ptr [0]            ; -> obj6:0x0104
        VxDCall  VKD.VKD_Local_Disable_Hot_Key   ; cd2004000d00
        mov    eax, dword ptr [0]            ; -> obj6:0x0100
        VxDCall  VKD.VKD_Local_Disable_Hot_Key   ; cd2004000d00
        jmp    loc_5_119A
loc_5_1183:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_5_1196
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_1196:
        popfd  
        stc    
        jmp    loc_5_119C
loc_5_119A:
        popfd  
        clc    
loc_5_119C:
        popal  
        ret    
sub_5_119E:
        push   eax
        push   edx
        push   esi
        call   sub_1_0314
loc_5_11A6:
        jb     loc_5_1237
        test   eax, 2
        je     loc_5_11CC
        cmp    dword ptr [edi + 0x30], ebx
        jne    loc_5_11CC
        call   sub_5_1002
        call   sub_5_1021
        mov    dword ptr [edi + 0x30], 0
        and    dword ptr [esi], 0xfffffffd
loc_5_11CC:
        test   eax, 1
        je     loc_5_1237
        cmp    dword ptr [edi + 0x24], ebx
        jne    loc_5_1237
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_11FE
        test   word ptr [edi + 4], 0x200
        jne    loc_5_11FE
        mov    eax, dword ptr [edi + 0x38]
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_5_11FE
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
loc_5_11FE:
        test   word ptr [edi + 4], 0x4000
        je     loc_5_120B
        call   sub_1_133B
loc_5_120B:
        call   sub_5_0F21
        mov    dword ptr [edi + 0x24], 0
        mov    eax, dword ptr [edi + 0x2c]
        test   word ptr [edi + 4], 8
        jne    loc_5_122E
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_5_122E:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        and    dword ptr [esi], 0xfffffffe
loc_5_1237:
        pop    esi
        pop    edx
        pop    eax
        clc    
        ret    
sub_5_123C:
        push   eax
        push   ecx
        push   edx
        push   esi
        test   eax, 2
        je     loc_5_125F
        cmp    dword ptr [edi + 0x30], 0
        je     loc_5_1254
        cmp    dword ptr [edi + 0x30], ebx
        je     loc_5_125F
        jmp    loc_5_1279
loc_5_1254:
        mov    dword ptr [edi + 0x30], ebx
        call   sub_5_1044
        mov    dword ptr [edi + 0x34], ebx
loc_5_125F:
        test   eax, 1
        je     loc_5_1304
        cmp    dword ptr [edi + 0x24], 0
        je     loc_5_1280
        cmp    dword ptr [edi + 0x24], ebx
        je     loc_5_1303
loc_5_1279:
        stc    
        jb     loc_5_1304
loc_5_1280:
        mov    dword ptr [edi + 0x24], ebx
        cmp    dword ptr [edi + 0x28], ebx
        je     loc_5_12AE
        mov    ebx, dword ptr [edi + 0x28]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_5_129F
        call   sub_1_0314
loc_5_1298:
        jb     loc_5_129F
        call   sub_1_0874
loc_5_129F:
        mov    ebx, dword ptr [edi + 0x24]
        call   sub_1_0314
loc_5_12A7:
        jb     loc_5_12AE
        call   sub_1_07A4
loc_5_12AE:
        mov    dword ptr [edi + 0x28], ebx
        call   sub_5_0F8C
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_12DC
        test   word ptr [edi + 4], 0x200
        jne    loc_5_12DC
        mov    eax, dword ptr [edi + 0x38]
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_5_12DC
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
loc_5_12DC:
        test   word ptr [edi + 4], 8
        jne    loc_5_1303
        mov    eax, dword ptr [edi + 0x2c]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_5_12FD
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_5_1303
loc_5_12FD:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_5_1303:
        clc    
loc_5_1304:
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_5_1309:
        mov    eax, 0x400
        clc    
        ret    
mssndsys_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 3
        jae    loc_5_133A
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj6:0x00E8
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_5_133A
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_5_133A
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_133A:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_1340:
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_13A1
        pushfd 
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        or     eax, eax
        je     loc_5_13A1
        mov    edi, dword ptr [eax]
        cmp    ebx, dword ptr [edi + 0x24]
        jne    loc_5_13A1
        xchg   esi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        cmp    esi, -1
        je     loc_5_13A1
        movzx  ecx, word ptr [ebp + 0x10]
        movzx  eax, word ptr [ebp + 0x18]
        add    ecx, eax
        cmp    ecx, dword ptr [edi + 0x1c]
        ja     loc_5_13A1
        movzx  ecx, word ptr [ebp + 0x18]
        mov    edi, dword ptr [edi + 0x18]
        movzx  eax, word ptr [ebp + 0x10]
        add    edi, eax
        or     ecx, ecx
        je     loc_5_13A1
        cld    
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        adc    cl, cl
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        clc    
        ret    
loc_5_13A1:
        mov    word ptr [ebp + 0x1c], 0
        stc    
        ret    
loc_5_13A9:
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_140A
        pushfd 
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        or     eax, eax
        je     loc_5_140A
        mov    esi, dword ptr [eax]
        cmp    ebx, dword ptr [esi + 0x24]
        jne    loc_5_140A
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_140A
        movzx  ecx, word ptr [ebp + 0x10]
        movzx  eax, word ptr [ebp + 0x18]
        add    ecx, eax
        cmp    ecx, dword ptr [esi + 0x1c]
        ja     loc_5_140A
        movzx  ecx, word ptr [ebp + 0x18]
        mov    esi, dword ptr [esi + 0x18]
        movzx  eax, word ptr [ebp + 0x10]
        add    esi, eax
        or     ecx, ecx
        je     loc_5_140A
        cld    
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        adc    cl, cl
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        clc    
        ret    
loc_5_140A:
        mov    word ptr [ebp + 0x1c], 0
        stc    
        ret    
loc_5_1412:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_026C
loc_5_141E:
        add    esp, 8
        jb     loc_5_1452
        movzx  eax, word ptr [ebp + 0x10]
        or     eax, eax
        je     loc_5_1452
        cmp    eax, 1
        jne    loc_5_1435
        mov    eax, dword ptr [edi + 0x38]
        jmp    loc_5_143F
loc_5_1435:
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        jne    loc_5_143F
        mov    eax, dword ptr [edi + 0x38]
loc_5_143F:
        or     eax, eax
        je     loc_5_1452
        push   ecx
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        mov    word ptr [ebp + 0x1c], cx
        pop    ecx
        clc    
        jmp    loc_5_1459
loc_5_1452:
        mov    word ptr [ebp + 0x1c], 0xffff
        stc    
loc_5_1459:
        pop    edi
        ret    
loc_5_145B:
        call   sub_5_1309
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_5_1466:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_14BA
        test   eax, 1
        je     loc_5_149F
        push   4
        push   edx
        call   sub_1_026C
loc_5_1481:
        add    esp, 8
        or     edi, edi
        jne    loc_5_149A
        add    edx, 4
        push   4
        push   edx
        call   sub_1_026C
loc_5_1493:
        add    esp, 8
        or     edi, edi
        je     loc_5_14BA
loc_5_149A:
        mov    ecx, dword ptr [edi + 0x28]
        jmp    loc_5_14C2
loc_5_149F:
        test   eax, 2
        je     loc_5_14BA
        push   5
        push   edx
        call   sub_1_026C
loc_5_14AE:
        add    esp, 8
        or     edi, edi
        je     loc_5_14BA
        mov    ecx, dword ptr [edi + 0x34]
        jmp    loc_5_14C2
loc_5_14BA:
        mov    word ptr [ebp + 0x1c], 1
loc_5_14C0:
        stc    
        ret    
loc_5_14C2:
        mov    word ptr [ebp + 0x1c], 0
        call   sub_5_123C
        jb     loc_5_14D0
        ret    
loc_5_14D0:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_14C0
loc_5_14D8:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1534
        test   eax, 1
        je     loc_5_1519
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_5_1505
        push   4
        push   edx
        call   sub_1_026C
loc_5_14FB:
        add    esp, 8
        or     edi, edi
        jne    loc_5_1514
        add    edx, 4
loc_5_1505:
        push   4
        push   edx
        call   sub_1_026C
loc_5_150D:
        add    esp, 8
        or     edi, edi
        je     loc_5_1534
loc_5_1514:
        mov    ecx, dword ptr [edi + 0x24]
        jmp    loc_5_153C
loc_5_1519:
        test   eax, 2
        je     loc_5_1534
        push   5
        push   edx
        call   sub_1_026C
loc_5_1528:
        add    esp, 8
        or     edi, edi
        je     loc_5_1534
        mov    ecx, dword ptr [edi + 0x30]
        jmp    loc_5_153C
loc_5_1534:
        mov    word ptr [ebp + 0x1c], 1
loc_5_153A:
        stc    
        ret    
loc_5_153C:
        cmp    ecx, ebx
        je     loc_5_1548
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_153A
loc_5_1548:
        call   sub_5_119E
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_1555:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 0xfffe
        jne    loc_5_15C6
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 1
        je     loc_5_156F
        mov    ecx, dword ptr [ebp + 0x18]
loc_5_156F:
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_15C6
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_15C6
        mov    edi, dword ptr [eax]
        jecxz  loc_5_1597
loc_5_1586:
        cmp    dword ptr [edi + 0x54], ecx
        je     loc_5_1597
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_15C6
        mov    edi, dword ptr [eax]
        jmp    loc_5_1586
loc_5_1597:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_15C6
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_15C6
        cmp    ecx, 0xba
        ja     loc_5_15C6
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_15C6:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_15CF:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1604
        push   5
        push   edx
        call   sub_1_026C
loc_5_15DF:
        add    esp, 8
        or     edi, edi
        je     loc_5_1604
        mov    ecx, dword ptr [edi + 0x34]
        mov    word ptr [ebp + 0x1c], 0
        mov    eax, 2
        call   sub_5_123C
        jb     loc_5_15FC
        ret    
loc_5_15FC:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_160A
loc_5_1604:
        mov    word ptr [ebp + 0x1c], 1
loc_5_160A:
        stc    
        ret    
loc_5_160C:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1644
        push   5
        push   edx
        call   sub_1_026C
loc_5_161C:
        add    esp, 8
        or     edi, edi
        je     loc_5_1644
        mov    ecx, dword ptr [edi + 0x30]
        cmp    ecx, ebx
        je     loc_5_1632
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_164A
loc_5_1632:
        mov    eax, 2
        call   sub_5_119E
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_1644:
        mov    word ptr [ebp + 0x1c], 1
loc_5_164A:
        stc    
        ret    
loc_5_164C:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_5_16C5
        mov    ecx, dword ptr [ebp + 0x18]
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_16C5
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_16C5
        mov    edi, dword ptr [eax]
loc_5_166F:
        cmp    dword ptr [edi + 0x54], ecx
        je     loc_5_1680
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_16C5
        mov    edi, dword ptr [eax]
        jmp    loc_5_166F
loc_5_1680:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_16C5
        test   word ptr [esi + 0x52], 8
        je     loc_5_16C5
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_16C5
        cmp    ecx, 0x1c
        ja     loc_5_16C5
        mov    word ptr [edi + 4], 1
        mov    ax, word ptr [esi + 0x22]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 0x54]
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_16C5:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_16CE:
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
        call   sub_1_026C
loc_5_1726:
        add    esp, 8
        or     edi, edi
        je     loc_5_1752
        lea    eax, [edi + 0x44]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_1885
        add    esp, 0x10
        or     eax, eax
        je     loc_5_1752
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_5_1752:
        mov    word ptr [ebp + 0x1c], 0
        mov    word ptr [ebp + 0x14], 0
        stc    
        ret    
loc_5_1760:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        call   sub_1_026C
loc_5_1776:
        add    esp, 8
        or     edi, edi
        je     loc_5_178C
        lea    eax, [edi + 0x44]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_1A15
        add    esp, 8
loc_5_178C:
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
sub_5_1794:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x6c
        mov    eax, 0xffffffff
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     loc_5_187E
        lea    eax, [ebp - 0x6c]
        push   eax
        call   sub_5_1AAA
        add    esp, 4
        call   sub_5_1A98
        call   sub_5_1AE7
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x14]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1AD9
        movzx  ecx, si
        shr    esi, 0x10
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_1ACA
        add    esp, 8
        call   sub_5_1AA4
        movzx  esi, word ptr [edi + 0x14]
        shl    esi, 0x10
        movzx  eax, word ptr [edi + 0x1c]
        or     esi, eax
        call   sub_5_1A9E
        lea    ecx, [ebp - 0x6c]
        push   ecx
        call   sub_5_1ABA
        add    esp, 4
        mov    eax, esi
loc_5_187E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_1885:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    loc_5_18AB
        push   0x15
        push   4
        call   sub_5_2350
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_5_18A9
        xor    eax, eax
        jmp    loc_5_1A10
loc_5_18A9:
        mov    dword ptr [esi], ebp
loc_5_18AB:
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    loc_5_18C8
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax + 8], 0
loc_5_18C8:
        push   ebp
        call   sub_5_2340
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_19BC
loc_5_18D8:
        mov    ecx, dword ptr [esp + 0x1c]
        lea    edx, [ebx + 0xc]
loc_5_18DF:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    loc_5_18FF
        or     al, al
        je     loc_5_18FB
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    loc_5_18FF
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    loc_5_18DF
loc_5_18FB:
        xor    eax, eax
        jmp    loc_5_1904
loc_5_18FF:
        sbb    eax, eax
        sbb    eax, -1
loc_5_1904:
        test   eax, eax
        je     loc_5_1915
        push   ebx
        push   ebp
        call   sub_5_2330
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_18D8
loc_5_1915:
        test   ebx, ebx
        je     loc_5_19BC
        cmp    dword ptr [ebx + 4], 0
        je     loc_5_192A
        xor    eax, eax
        jmp    loc_5_1A10
loc_5_192A:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_1794
        add    esp, 0x14
        test   eax, eax
        je     loc_5_1957
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_1A15
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_1A10
loc_5_1957:
        mov    eax, dword ptr [esp + 0x20]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_1794
        add    esp, 0x14
        test   eax, eax
        je     loc_5_198E
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_1A15
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_1A10
loc_5_198E:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_1794
        add    esp, 0x14
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_1794
        add    esp, 0x14
        jmp    loc_5_1A0E
loc_5_19BC:
        push   ebp
        call   sub_5_231C
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_19CC
        xor    eax, eax
        jmp    loc_5_1A10
loc_5_19CC:
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
        call   sub_5_230C
loc_5_1A0E:
        mov    eax, ebx
loc_5_1A10:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_1A15:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        test   edi, edi
        je     loc_5_1A61
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_5_1A3F
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_1794
        add    esp, 0x14
        mov    dword ptr [edi], 0
loc_5_1A3F:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     loc_5_1A61
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_1794
        mov    dword ptr [edi + 4], 0
        add    esp, 0x14
loc_5_1A61:
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_1A92
        push   edi
        push   eax
        call   sub_5_2384
        push   edi
        push   dword ptr [esi]
        call   sub_5_2374
        push   dword ptr [esi]
        call   sub_5_2340
        test   eax, eax
        jne    loc_5_1A8C
        push   dword ptr [esi]
        call   sub_5_2364
loc_5_1A8C:
        mov    dword ptr [esi], 0
loc_5_1A92:
        pop    edi
        pop    esi
        ret    
        db  0CCh, 0CCh, 0CCh
sub_5_1A98:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083800100
sub_5_1A9E:
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
sub_5_1AA4:
        VMMCall  VMM.Resume_Exec                 ; cd2085800100
sub_5_1AAA:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        pop    ebp
        ret    
sub_5_1ABA:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        pop    ebp
        ret    
sub_5_1ACA:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048800100
sub_5_1AD9:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        pop    ebp
        ret    
sub_5_1AE7:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  0CCh, 0CCh
sub_5_1AF4:
        movzx  eax, word ptr [0x10]          ; -> loc_2_0010
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_5_1B02:
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
sub_5_1B1D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
        pop    ebp
        ret    
sub_5_1B2B:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Phys_Unmask_Channel ; cd2019000400
        pop    ebx
        pop    ebp
        ret    
sub_5_1B41:
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
sub_5_1B62:
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
sub_5_1B86:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        mov    al, byte ptr [ebp + 0x10]
        call   sub_1_0607
loc_5_1B98:
        xor    eax, eax
        jae    loc_5_1BA1
        mov    eax, 1
loc_5_1BA1:
        pop    edx
        pop    ebp
        ret    
sub_5_1BA4:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        call   sub_1_05E9
loc_5_1BB3:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret    
sub_5_1BB8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_026C
loc_5_1BC8:
        add    esp, 8
        mov    word ptr [edi + 0xb8], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret    
sub_5_1BD8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0xffff
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_026C
loc_5_1BEE:
        add    esp, 8
        mov    eax, dword ptr [edi + 0x24]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        je     loc_5_1C00
        jmp    loc_5_1C0E
loc_5_1C00:
        mov    word ptr [edi + 0xb8], 1
        mov    edx, 0
loc_5_1C0E:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret    
sub_5_1C15:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   eax
        call   sub_1_026C
loc_5_1C28:
        add    esp, 8
        mov    ax, word ptr [edi + 0x42]
        mov    word ptr [esi + 0x30], ax
        mov    ah, byte ptr [edi + 9]
        mov    byte ptr [esi + 0x32], ah
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [edi + 0x38]
        mov    dword ptr [esi + 0x18], eax
        mov    ax, word ptr [edi + 0x52]
        mov    word ptr [esi + 0x2e], ax
        mov    ax, word ptr [edi + 0x4c]
        mov    word ptr [esi + 0x2c], ax
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_5_1C65:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0040
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_1C9A
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_1CA8
loc_5_1C9A:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1CA8:
        pop    edi
        pop    esi
        ret    0xc
loc_5_1CAD:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        inc    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_1CBB:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        dec    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_1CC9:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [esi], 0xb
        call   sub_5_1AF4
        push   0xff
        lea    ecx, [esi + 4]
        push   eax
        lea    edi, [esi + 0x104]
        push   ecx
        call   sub_5_2398
        add    esp, 0xc
        push   0xff
        push   0                             ; -> obj6:0x02CC
        push   edi
        call   sub_5_2398
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 0xc
        xor    ecx, ecx
        mov    dword ptr [esi + 0x210], ecx
        mov    edx, dword ptr [eax + 4]
        mov    eax, 0xb3
        mov    dword ptr [esi + 0x204], edx
        mov    dword ptr [esi + 0x20c], ecx
        mov    word ptr [esi + 0x208], 0x45f
        cmp    byte ptr [edi], cl
        je     loc_5_1D3D
loc_5_1D32:
        movsx  ecx, byte ptr [edi]
        add    eax, ecx
        inc    edi
        cmp    byte ptr [edi], 0
        jne    loc_5_1D32
loc_5_1D3D:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 0x20a], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_1D54:
        push   esi
        push   edi
        mov    edi, 0x8878000a
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_5_1D79
        push   dword ptr [esi + 4]
        call   sub_5_1BD8
        add    esp, 4
        test   eax, eax
        jne    loc_5_1D79
        xor    edi, edi
        inc    dword ptr [esi + 0xc]
loc_5_1D79:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_1D80:
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
loc_5_1DCB:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 0x8007000e
        mov    esi, dword ptr [esp + 0x2c]
        test   byte ptr [esp + 0x1c], 0xe0
        mov    dword ptr [esi], ebx
        jne    loc_5_1E72
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 0x10], ebx
        jne    loc_5_1E6B
        push   0x201
        push   0x33
        call   sub_5_23A8
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_1E64
        mov    dword ptr [ebx], 0            ; -> obj6:0x02E8
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_1C15
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1000
        jbe    loc_5_1E7B
        cmp    eax, 0xffff
        jae    loc_5_1E7B
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_5_1E7B
        cmp    dword ptr [ebx + 0x10], 0
        je     loc_5_1E7B
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_5_1E7B
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        push   ebx
        xor    edi, edi
        call   sub_5_201B
        mov    eax, dword ptr [esp + 0x24]
        add    esp, 0xc
        movzx  ecx, word ptr [eax + 0xe]
        mov    dword ptr [ebx + 0x24], ecx
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [ebx + 0x28], edx
        jmp    loc_5_1E7B
loc_5_1E64:
        mov    edi, 0x8007000e
        jmp    loc_5_1E7B
loc_5_1E6B:
        mov    edi, 0x8878000a
        jmp    loc_5_1E7B
loc_5_1E72:
        mov    edi, 0x8878001e
        mov    ebp, dword ptr [esp + 0x14]
loc_5_1E7B:
        test   edi, edi
        je     loc_5_1E90
        test   ebx, ebx
        je     loc_5_1E95
        push   0
        push   ebx
        call   sub_5_23A0
        add    esp, 8
        jmp    loc_5_1E95
loc_5_1E90:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 0x10]
loc_5_1E95:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
loc_5_1E9E:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        mov    eax, 0x80004001
        ret    0xc
loc_5_1EB0:
        mov    eax, 0x80004005
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0xc]
        dec    edx
        mov    dword ptr [ecx + 0xc], edx
        jne    loc_5_1ED5
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_5_1ED5
        push   dword ptr [ecx + 4]
        call   sub_5_1BB8
        add    esp, 4
        xor    eax, eax
loc_5_1ED5:
        ret    4
sub_5_1ED8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        push   esi
        lea    eax, [ebp - 0x18]
        push   esi
        lea    ecx, [ebp - 8]
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        push   ecx
        mov    dword ptr [ebp - 4], esi
        push   ebx
        mov    dword ptr [ebp - 0x18], esi
        call   sub_5_22BC
        add    esp, 0x14
        test   eax, eax
        je     loc_5_1F06
        mov    dword ptr [ebp - 8], esi
loc_5_1F06:
        cmp    dword ptr [ebp - 8], 8
        je     loc_5_1FAC
        push   0x201
        push   0x14
        call   sub_5_23A8
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     loc_5_1F83
        push   1
        lea    eax, [ebp - 4]
        push   0                             ; -> obj6:0x02DC
        lea    ecx, [ebp - 0x1c]
        push   eax
        mov    dword ptr [edi + 4], ebx
        push   ecx
        mov    dword ptr [edi], 0            ; -> obj6:0x02A8
        call   sub_5_23C4
        test   eax, eax
        jne    loc_5_1F83
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], edi
        mov    dword ptr [ebp - 0x14], 0
        mov    dword ptr [edi + 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        VxDCall  Dev_357E.Service_0001h          ; cd2001007e35
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_1F83
        push   edi
        mov    esi, 1
        push   ebx
        call   sub_1_02F3
loc_5_1F80:
        add    esp, 8
loc_5_1F83:
        test   esi, esi
        jne    loc_5_1FAC
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_1F9D
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_23B0
loc_5_1F9D:
        test   edi, edi
        je     loc_5_1FAC
        push   0
        push   edi
        call   sub_5_23A0
        add    esp, 8
loc_5_1FAC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_1FB3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_02D3
loc_5_1FC4:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_5_2015
        push   0
        push   edi
        call   sub_1_02F3
loc_5_1FD5:
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
        call   sub_5_23B0
        push   0
        push   esi
        call   sub_5_23A0
        add    esp, 8
loc_5_2015:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_201B:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [edx], eax
        jbe    loc_5_202D
        mov    dword ptr [edx], eax
loc_5_202D:
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
loc_5_204D:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x20]
        dec    esi
        mov    dword ptr [eax + 0x20], esi
        jne    loc_5_2072
        cmp    dword ptr [eax + 8], 0
        jne    loc_5_2072
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 0x10]
        call   sub_5_23A0
        add    esp, 8
loc_5_2072:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_2078:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0050
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_20AD
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_20BB
loc_5_20AD:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_20BB:
        pop    edi
        pop    esi
        ret    0xc
loc_5_20C0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x20]
        inc    eax
        mov    dword ptr [ecx + 0x20], eax
        ret    4
loc_5_20CE:
        mov    eax, 0x80004001
        ret    0x20
loc_5_20D6:
        mov    eax, 0x80004001
        ret    0x14
loc_5_20DE:
        mov    eax, 0x80004001
        ret    8
loc_5_20E6:
        mov    eax, 0x80004001
        ret    8
loc_5_20EE:
        mov    eax, 0x80004001
        ret    8
loc_5_20F6:
        mov    eax, 0x80004001
        ret    8
loc_5_20FE:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    loc_5_2130
        push   dword ptr [esi + 0x18]
        call   sub_5_1B02
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 0x32], 3
        jbe    loc_5_211D
        add    eax, eax
loc_5_211D:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_5_2144
loc_5_2130:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_2144:
        xor    eax, eax
        pop    esi
        ret    0xc
loc_5_214A:
        push   ebx
        mov    eax, 0x80004005
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 8], 0
        jne    loc_5_2209
        movzx  edi, word ptr [esi + 0x30]
        push   0
        push   0xa
        push   edi
        call   sub_5_1B86
        add    esp, 0xc
        lea    edx, [edi + 2]
        xor    al, al
        push   dword ptr [esi + 0x18]
        out    dx, al
        call   sub_5_1B1D
        add    esp, 4
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 0x18]
        call   sub_5_1B41
        add    esp, 0xc
        push   dword ptr [esi + 0x18]
        call   sub_5_1B62
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_1B2B
        mov    bx, word ptr [esi + 0x1c]
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0x10
        jne    loc_5_21B6
        shr    bx, 1
loc_5_21B6:
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_5_21BF
        shr    bx, 1
loc_5_21BF:
        dec    bx
        push   ebx
        push   0xf
        shr    bx, 8
        push   edi
        call   sub_5_1B86
        add    esp, 0xc
        push   ebx
        push   0xe
        push   edi
        call   sub_5_1B86
        add    esp, 0xc
        mov    al, 1
        test   byte ptr [esi + 0x2e], 0x40
        jne    loc_5_21E7
        mov    al, 5
loc_5_21E7:
        push   eax
        push   9
        push   edi
        call   sub_5_1B86
        add    esp, 0xc
        push   2
        push   0xa
        push   edi
        call   sub_5_1B86
        add    esp, 0xc
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_2209:
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
loc_5_220F:
        push   esi
        mov    eax, 0x80004005
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        cmp    dword ptr [edi + 8], 0
        je     loc_5_2264
        movzx  esi, word ptr [edi + 0x30]
        push   0xa
        push   esi
        call   sub_5_1BA4
        add    esp, 8
        and    al, 0xc0
        push   eax
        push   0xa
        push   esi
        call   sub_5_1B86
        add    esp, 0xc
        lea    edx, [esi + 2]
        xor    al, al
        push   0
        out    dx, al
        push   9
        push   esi
        call   sub_5_1B86
        add    esp, 0xc
        push   dword ptr [edi + 0x18]
        call   sub_5_1B1D
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [edi + 8], 0
loc_5_2264:
        pop    edi
        pop    esi
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_5_226C:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_5_2274:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_5_227C:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        int3   
        int3   
sub_5_2284:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        int3   
        int3   
sub_5_228C:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        int3   
        int3   
sub_5_2294:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_5_229C:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_5_22A4:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_5_22AC:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_5_22B4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_5_22BC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_5_22C4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        int3   
        int3   
sub_5_22CC:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        int3   
        int3   
sub_5_22D4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Log_Conf ; cd201b803300
        int3   
        int3   
sub_5_22DC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_5_22E4:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_5_22EC:
        VxDCall  CONFIGMG._CONFIGMG_Add_Res_Des  ; cd201c803300
        int3   
        int3   
sub_5_22F4:
        VxDCall  CONFIGMG._CONFIGMG_Add_Empty_Log_Conf ; cd2018803300
        int3   
        int3   
sub_5_22FC:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        int3   
        int3   
sub_5_2304:
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004803300
        int3   
        int3   
sub_5_230C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_231C:
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
sub_5_2330:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2340:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_2350:
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
sub_5_2364:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_2374:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2384:
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
sub_5_2398:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_5_23A0:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_5_23A8:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_5_23B0:
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
sub_5_23C4:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_5_23E2
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_23E2:
        ret    0x10
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x37a  RW-
        db  04Dh, 053h, 053h, 04Eh, 044h, 053h, 059h, 053h, 03Ah, 020h, 000h, 000h, 049h, 052h, 051h, 020h
        db  06Eh, 06Fh, 074h, 020h, 072h, 065h, 073h, 070h, 06Fh, 06Eh, 064h, 069h, 06Eh, 067h, 020h, 028h
        db  025h, 078h, 03Ah, 025h, 078h, 029h, 00Ah, 000h, 044h, 04Dh, 041h, 020h, 06Eh, 06Fh, 074h, 020h
        db  072h, 065h, 073h, 070h, 06Fh, 06Eh, 064h, 069h, 06Eh, 067h, 020h, 028h, 025h, 078h, 03Ah, 025h
        db  078h, 029h, 00Ah, 000h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 000h, 000h, 053h, 069h, 06Eh, 067h
        db  06Ch, 065h, 020h, 04Dh, 06Fh, 064h, 065h, 020h, 044h, 04Dh, 041h, 000h, 05Ch, 043h, 06Fh, 06Eh
        db  066h, 069h, 067h, 000h, 04Dh, 049h, 044h, 049h, 000h, 000h, 000h, 000h, 042h, 04Ch, 041h, 053h
        db  054h, 045h, 052h, 000h, 041h, 025h, 030h, 033h, 058h, 020h, 049h, 025h, 064h, 020h, 044h, 025h
        db  064h, 000h, 000h, 000h, 056h, 049h, 052h, 054h, 055h, 041h, 04Ch, 05Ch, 057h, 053h, 053h, 02Dh
        db  053h, 042h, 045h, 06Dh, 075h, 06Ch, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 053h, 06Fh, 075h, 06Eh
        db  064h, 020h, 042h, 06Ch, 061h, 073h, 074h, 065h, 072h, 020h, 045h, 06Dh, 075h, 06Ch, 061h, 074h
        db  069h, 06Fh, 06Eh, 000h
        dd  loc_5_145B               ; fixup
        dd  loc_5_1555               ; fixup
        dd  loc_5_1466               ; fixup
        dd  loc_5_14D8               ; fixup
        dd  loc_5_1340               ; fixup
        dd  loc_5_13A9               ; fixup
        dd  loc_5_1412               ; fixup
        dd  loc_5_145B               ; fixup
        dd  loc_5_164C               ; fixup
        dd  loc_5_15CF               ; fixup
        dd  loc_5_160C               ; fixup
        dd  loc_5_16CE               ; fixup
        dd  loc_5_1760               ; fixup
        db  007h, 000h, 000h, 000h
        dd  obj6:0x00B4              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj6:0x00D0              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj6:0x00E0              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  loc_5_0D4C               ; fixup
        db  001h, 000h
        dd  loc_5_0D4C               ; fixup
        db  002h, 000h
        dd  loc_5_0D4C               ; fixup
        db  003h, 000h
        dd  loc_5_0D4C               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_5_0D68               ; fixup
        db  001h, 000h
        dd  loc_5_0D68               ; fixup
        db  002h, 000h
        dd  loc_5_0D68               ; fixup
        db  003h, 000h
        dd  loc_5_0D68               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_5_0EB0               ; fixup
        db  001h, 000h
        dd  loc_5_0EB0               ; fixup
        db  002h, 000h
        dd  loc_5_0EB0               ; fixup
        db  003h, 000h
        dd  loc_5_0EB0               ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_5_0DAC               ; fixup
        db  001h, 000h
        dd  loc_5_0DAC               ; fixup
        db  002h, 000h
        dd  loc_5_0DAC               ; fixup
        db  003h, 000h
        dd  loc_5_0DAC               ; fixup
        db  004h, 000h
        dd  loc_5_0DAC               ; fixup
        db  005h, 000h
        dd  loc_5_0DAC               ; fixup
        db  006h, 000h
        dd  loc_5_0DAC               ; fixup
        db  007h, 000h
        dd  loc_5_0DAC               ; fixup
        db  008h, 000h
        dd  loc_5_0DAC               ; fixup
        db  009h, 000h
        dd  loc_5_0DAC               ; fixup
        db  00Ah, 000h
        dd  loc_5_0DAC               ; fixup
        db  00Bh, 000h
        dd  loc_5_0DAC               ; fixup
        db  00Ch, 000h
        dd  loc_5_0DAC               ; fixup
        db  00Dh, 000h
        dd  loc_5_0DAC               ; fixup
        db  00Eh, 000h
        dd  loc_5_0DAC               ; fixup
        db  00Fh, 000h
        dd  loc_5_0DAC               ; fixup
        db  000h, 001h, 003h, 0FFh, 001h, 002h, 003h, 007h, 009h, 00Ah, 00Bh, 0FFh, 048h, 050h, 058h, 060h
        db  0FFh, 000h, 000h, 004h, 000h
        dd  loc_1_03F4               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0574               ; fixup
        dd  loc_1_0594               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 075h, 06Eh, 061h, 062h
        db  06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 041h
        db  075h, 074h, 06Fh, 053h, 065h, 06Ch, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 074h
        db  06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 043h, 04Fh, 044h, 045h, 043h
        db  00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh
        db  063h, 061h, 074h, 065h, 020h, 053h, 042h, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h
        db  074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 04Fh, 050h, 04Ch, 033h
        db  00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh
        db  063h, 061h, 074h, 065h, 020h, 049h, 052h, 051h, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h
        db  020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 044h, 04Dh, 041h
        db  00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh
        db  063h, 061h, 074h, 065h, 020h, 062h, 075h, 066h, 066h, 065h, 072h, 00Ah, 000h, 000h, 000h, 087h
        db  083h, 081h, 082h, 000h, 08Bh, 089h, 08Ah, 000h, 000h, 000h, 000h
        dd  loc_5_1C65               ; fixup
        dd  loc_5_1CAD               ; fixup
        dd  loc_5_1CBB               ; fixup
        dd  loc_5_1CC9               ; fixup
        dd  loc_5_1D54               ; fixup
        dd  loc_5_1EB0               ; fixup
        dd  loc_5_1D80               ; fixup
        dd  loc_5_1DCB               ; fixup
        dd  loc_5_1E9E               ; fixup
        db  04Dh, 053h, 053h, 04Eh, 044h, 053h, 059h, 053h, 02Eh, 056h, 058h, 044h, 000h, 000h, 000h, 000h
        db  064h, 073h, 06Fh, 075h, 06Eh, 064h, 02Eh, 076h, 078h, 064h, 000h, 000h
        dd  loc_5_2078               ; fixup
        dd  loc_5_20C0               ; fixup
        dd  loc_5_204D               ; fixup
        dd  loc_5_20CE               ; fixup
        dd  loc_5_20D6               ; fixup
        dd  loc_5_20DE               ; fixup
        dd  loc_5_20E6               ; fixup
        dd  loc_5_20EE               ; fixup
        dd  loc_5_20F6               ; fixup
        dd  loc_5_20FE               ; fixup
        dd  loc_5_214A               ; fixup
        dd  loc_5_220F               ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_1_0B4C               ; fixup
        db  001h, 000h
        dd  loc_1_0B4C               ; fixup
        db  002h, 000h
        dd  loc_1_0B4C               ; fixup
        db  003h, 000h
        dd  loc_1_0B4C               ; fixup
        db  004h, 000h
        dd  loc_1_0B4C               ; fixup
        db  005h, 000h
        dd  loc_1_0B4C               ; fixup
        db  006h, 000h
        dd  loc_1_0B9C               ; fixup
        db  007h, 000h
        dd  loc_1_0B4C               ; fixup
        db  008h, 000h
        dd  loc_1_0BDC               ; fixup
        db  009h, 000h
        dd  loc_1_0C04               ; fixup
        db  00Ah, 000h
        dd  loc_1_0C24               ; fixup
        db  00Bh, 000h
        dd  loc_1_0B4C               ; fixup
        db  00Ch, 000h
        dd  loc_1_0C7C               ; fixup
        db  00Dh, 000h
        dd  loc_1_0B4C               ; fixup
        db  00Eh, 000h
        dd  loc_1_1290               ; fixup
        db  00Fh, 000h
        dd  loc_1_0B4C               ; fixup
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0xb8  R-X
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
        mov    ecx, 0x34
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_7_00B2
        mov    dword ptr [edi + ebx], esi
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_7_003D
        pop    ebx
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_0350
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    al, 0x49
        mov    ah, 1
        mov    ebx, 0xf3830180
        mov    cl, 0x85
        mov    esi, 0                        ; -> loc_1_1380
        xor    edx, edx
        xor    edi, edi
        VxDCall  VKD.VKD_Define_Hot_Key          ; cd2001000d00
        jb     loc_7_00B0
        mov    dword ptr [0], eax            ; -> obj6:0x0104
        mov    al, 0x51
        mov    ah, 1
        mov    ebx, 0xf3830180
        mov    cl, 0x85
        mov    esi, 0                        ; -> loc_1_1380
        xor    edx, edx
        xor    edi, edi
        VxDCall  VKD.VKD_Define_Hot_Key          ; cd2001000d00
        jb     loc_7_00B0
        mov    dword ptr [0], eax            ; -> obj6:0x0100
loc_7_00B0:
        clc    
        ret    
loc_7_00B2:
        pop    ebx
        jmp    loc_7_0011
VxD__LTEXT ENDS

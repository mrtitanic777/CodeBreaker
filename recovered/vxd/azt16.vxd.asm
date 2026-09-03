; azt16 -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 5634 instructions recovered by recursive descent

Declare_Virtual_Device azt16, 4, 0, azt16_Control, 0045Fh, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xa1c  R-X
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
        db  'azt16   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  azt16_Control            ; DDB_Control_Proc
        dd  azt16_PM_API             ; DDB_V86_API_Proc
        dd  azt16_PM_API             ; DDB_PM_API_Proc
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
        add    byte ptr [esp + ecx], cl
        or     al, 0xc
        or     byte ptr [eax], cl
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax - 4], al
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
        add    byte ptr [esp + ebp*4], al
        db  000h, 080h, 0BBh, 000h, 000h
azt16_Control:
        cmp    eax, 8
        je     loc_5_0B7D
loc_1_01BD:
        cmp    eax, 0xb
        je     loc_5_0B17
loc_1_01C6:
        cmp    eax, 3
        je     loc_5_0B7D
loc_1_01CF:
        cmp    eax, 0x1b
        je     loc_7_0000
loc_1_01D8:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_01E1:
        cmp    eax, 0x22
        je     loc_4_0A54
loc_1_01EA:
        clc    
        ret    
sub_1_01EC:
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
        je     loc_1_0243
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_0243
loc_1_021F:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0xc], 1
        jne    loc_1_0233
        cmp    dword ptr [edx + edi], ebx
        jne    loc_1_0239
loc_1_022F:
        popfd  
        clc    
        jmp    loc_1_0247
loc_1_0233:
        cmp    word ptr [edx + edi], bx
        je     loc_1_022F
loc_1_0239:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_0243
        jmp    loc_1_021F
loc_1_0243:
        popfd  
        xor    edi, edi
        stc    
loc_1_0247:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
        db  0B8h, 000h, 004h, 000h, 000h, 0C3h
sub_1_0253:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_01EC
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     loc_1_0270
        mov    eax, dword ptr [edi + 0xb8]
loc_1_0270:
        pop    edi
        pop    ebp
        ret    
sub_1_0273:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_01EC
        add    esp, 8
        or     edi, edi
        je     loc_1_0291
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0xb8], eax
loc_1_0291:
        pop    edi
        pop    ebp
        ret    
sub_1_0294:
        push   eax
        push   ebx
        pushfd 
        add    ebx, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_02C1
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_02C1
loc_1_02AC:
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_02B7
        mov    esi, eax
        popfd  
        clc    
        jmp    loc_1_02C5
loc_1_02B7:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_02C1
        jmp    loc_1_02AC
loc_1_02C1:
        popfd  
        xor    esi, esi
        stc    
loc_1_02C5:
        pop    ebx
        pop    eax
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  loc_1_00D8               ; fixup
loc_1_02D0:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_031F
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_1_031F
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_031F
loc_1_02F1:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x24], ebx
        jne    loc_1_02FD
        or     ecx, 1
loc_1_02FD:
        cmp    dword ptr [edi + 0x30], ebx
        jne    loc_1_0305
        or     ecx, 2
loc_1_0305:
        jecxz  loc_1_0310
        push   eax
        mov    eax, ecx
        call   sub_5_0BE0
loc_1_030F:
        pop    eax
loc_1_0310:
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_031F
        jmp    loc_1_02F1
loc_1_031F:
        popfd  
        popal  
        jmp    loc_1_00D8
        db  090h
loc_1_0328:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_036C
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        and    dl, 0x3f
        push   edi
        push   2
        push   eax
        call   sub_1_01EC
        add    esp, 8
        or     edi, edi
        jne    loc_1_0354
        push   3
        push   eax
        call   sub_1_01EC
        add    esp, 8
loc_1_0354:
        or     edi, edi
        je     loc_1_0363
        test   word ptr [edi + 4], 4
        je     loc_1_0363
        or     dl, 0x40
loc_1_0363:
        pop    edi
        xor    dh, dh
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
loc_1_036C:
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012000400
        ret    
        db  090h
loc_1_0374:
        push   eax
        push   edi
        push   edx
        mov    edi, edx
        mov    eax, dword ptr [edi + 0x24]
        or     eax, eax
        je     loc_1_03EC
        cmp    word ptr [edi + 0xbc], 0
        jne    loc_1_03EC
        mov    ebx, eax
        test   word ptr [edi + 0x52], 8
        je     loc_1_03E1
        movzx  edx, word ptr [edi + 0x22]
        in     al, dx
        test   al, 0x80
        je     loc_1_03E1
        push   ebx
        mov    ebx, dword ptr [edi + 0x30]
        or     ebx, ebx
        je     loc_1_03CA
        test   word ptr [edi + 4], 0x2000
        jne    loc_1_03CA
        mov    byte ptr [edi + 0xac], al
        or     word ptr [edi + 4], 0x2000
        push   edx
        push   esi
        mov    esi, 0                        ; -> loc_5_08F1
        mov    edx, edi
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        pop    edx
loc_1_03CA:
        pop    ebx
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_0418
        push   ebx
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_07FB
        pop    ebx
loc_1_03E1:
        pop    edx
        pop    edi
        pop    eax
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        clc    
        ret    
loc_1_03EC:
        test   word ptr [edi + 0x52], 8
        je     loc_1_040B
        movzx  edx, word ptr [edi + 0x22]
        in     al, dx
        test   al, 0x80
        je     loc_1_040B
        push   ebx
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_07FB
        pop    ebx
loc_1_040B:
        movzx  edx, word ptr [edi + 0x42]
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_1_0418:
        pop    edx
        pop    edi
        pop    eax
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
        db  090h
loc_1_0424:
        push   ecx
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 8
        je     loc_1_043F
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_1_043F:
        pop    ecx
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_0444:
        push   edi
        push   1
        push   eax
        call   sub_1_01EC
        add    esp, 8
        or     edi, edi
        je     loc_1_0473
        test   word ptr [edi + 4], 8
        jne    loc_1_047D
        cmp    dword ptr [edi + 0x24], ebx
        jne    loc_1_0473
        jecxz  loc_1_046B
loc_1_0463:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_047D
loc_1_046B:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_047D
loc_1_0473:
        test   word ptr [edi + 4], 0x8000
        jne    loc_1_046B
        je     loc_1_0463
loc_1_047D:
        pop    edi
        clc    
        ret    
sub_1_0480:
        push   eax
        push   ecx
        push   edx
        add    dx, 0
        mov    ecx, 0x20000
        in     al, dx
loc_1_048D:
        in     al, dx
        or     al, al
        jns    loc_1_0495
        loop   loc_1_048D
        stc    
loc_1_0495:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_0499:
        call   sub_1_0480
        jb     loc_1_04B6
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
loc_1_04B6:
        ret    
sub_1_04B7:
        call   sub_1_0480
        jb     loc_1_04D8
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
loc_1_04D8:
        ret    
sub_1_04D9:
        push   edx
        push   ecx
        push   eax
        pushfd 
        cli    
        cmp    word ptr [edi + 0x4c], 0
        jne    loc_1_0530
        mov    ecx, 6
        push   esi
        xor    esi, esi
loc_1_04ED:
        mov    eax, esi
        mov    ah, al
        call   sub_1_0499
        mov    byte ptr [esi], al            ; -> obj1:0x0126
        or     al, 0x80
        call   sub_1_04B7
        inc    esi
        loop   loc_1_04ED
        mov    ecx, 2
loc_1_050B:
        mov    eax, esi
        mov    ah, al
        call   sub_1_0499
        mov    byte ptr [esi], al            ; -> obj1:0x0126
        push   eax
        and    al, 0xf
        cmp    al, 0xd
        pop    eax
        jbe    loc_1_0526
        mov    al, 0xd
        jmp    loc_1_0528
loc_1_0526:
        and    al, 0xf0
loc_1_0528:
        call   sub_1_04B7
        loop   loc_1_050B
        pop    esi
loc_1_0530:
        mov    al, byte ptr [0]              ; -> obj1:0x0114
        or     al, 0x40
        mov    byte ptr [0], al              ; -> obj1:0x0114
        call   sub_1_0480
        jb     loc_1_054C
        add    dx, 0
        out    dx, al
        popfd  
        clc    
        jmp    loc_1_054E
loc_1_054C:
        popfd  
        stc    
loc_1_054E:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_1_0552:
        push   edx
        push   ecx
        push   eax
        pushfd 
        cli    
        mov    al, byte ptr [0]              ; -> obj1:0x0114
        and    al, 0xbf
        mov    byte ptr [0], al              ; -> obj1:0x0114
        call   sub_1_0480
        jb     loc_1_05BA
        or     al, 9
        add    dx, 0
        out    dx, al
        inc    dx
        in     al, dx
        test   al, 8
        je     loc_1_0590
        mov    ecx, 0x1388
loc_1_057D:
        in     al, dx
        test   al, 0x20
        jne    loc_1_0584
        loop   loc_1_057D
loc_1_0584:
        mov    ecx, 0x1388
loc_1_0589:
        in     al, dx
        test   al, 0x20
        je     loc_1_0590
        loop   loc_1_0589
loc_1_0590:
        sub    dx, 1
        cmp    word ptr [edi + 0x4c], 0
        jne    loc_1_05B6
        mov    ecx, 8
        push   esi
        xor    esi, esi
loc_1_05A3:
        mov    eax, esi
        mov    ah, al
        mov    al, byte ptr [esi]            ; -> obj1:0x0126
        call   sub_1_04B7
        inc    esi
        loop   loc_1_05A3
        pop    esi
loc_1_05B6:
        popfd  
        clc    
        jmp    loc_1_05BC
loc_1_05BA:
        popfd  
        stc    
loc_1_05BC:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_1_05C0:
        push   edx
        push   ecx
        push   eax
        mov    cx, ax
        mov    ah, 0xa
        call   sub_1_0499
        and    al, 0x3f
        mov    ah, byte ptr [0]              ; -> loc_1_0115
        or     cx, cx
        je     loc_1_05DF
        or     ah, 0x40
        jmp    loc_1_05F2
loc_1_05DF:
        and    ah, 0xbf
        push   eax
        mov    ecx, 0x5000
        mov    ah, 0xa
loc_1_05EA:
        call   sub_1_0499
        loop   loc_1_05EA
        pop    eax
loc_1_05F2:
        mov    byte ptr [0], ah              ; -> loc_1_0115
        or     al, ah
        mov    ah, 0xa
        call   sub_1_04B7
        pop    eax
        pop    ecx
        pop    edx
        clc    
        ret    
        db  052h, 051h, 050h, 09Ch, 0FAh, 0A0h
        dd  obj1:0x0114              ; fixup
        db  00Ch, 020h, 0A2h
        dd  obj1:0x0114              ; fixup
        db  0E8h, 064h, 0FEh, 0FFh, 0FFh, 072h, 009h, 066h, 083h, 0C2h, 000h, 0EEh, 09Dh, 0F8h, 0EBh, 002h
        db  09Dh, 0F9h, 058h, 059h, 05Ah, 0C3h, 052h, 051h, 050h, 09Ch, 0FAh, 0A0h
        dd  obj1:0x0114              ; fixup
        db  024h, 0DFh, 0A2h
        dd  obj1:0x0114              ; fixup
        db  0E8h, 03Dh, 0FEh, 0FFh, 0FFh, 072h, 0E2h, 066h, 083h, 0C2h, 000h, 0EEh, 09Dh, 0F8h, 0EBh, 002h
        db  09Dh, 0F9h, 058h, 059h, 05Ah, 0C3h
sub_1_0654:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 0x42]
        push   edi
        push   esi
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        call   sub_1_0294
        jb     loc_1_0674
        mov    edi, dword ptr [esp]
        mov    eax, dword ptr [esi + 0xa]
        mov    dword ptr [edi + 0xa], eax
loc_1_0674:
        pop    esi
        pop    edi
        pushfd 
        cli    
        cmp    word ptr [edi + 0x4c], 0
        jne    loc_1_0689
        mov    eax, 1
        call   sub_1_05C0
loc_1_0689:
        call   sub_1_04D9
        mov    ecx, 8
        mov    ah, 0xf
loc_1_0695:
        mov    al, byte ptr [ecx + esi + 0xf]
        cmp    ah, 0xa
        jne    loc_1_06A6
        or     al, byte ptr [0]              ; -> loc_1_0115
        jmp    loc_1_06B6
loc_1_06A6:
        cmp    ah, 9
        jne    loc_1_06AF
        and    al, 0xc
        jmp    loc_1_06B6
loc_1_06AF:
        cmp    ah, 0xa
        jne    loc_1_06B6
        and    al, 0xfd
loc_1_06B6:
        call   sub_1_04B7
        dec    ah
        loop   loc_1_0695
        mov    ah, 0x1c
        mov    al, byte ptr [esi + 0x24]
        call   sub_1_04B7
        call   sub_1_0552
        mov    ecx, 7
        mov    ah, 7
loc_1_06D5:
        mov    al, byte ptr [ecx + esi + 8]
        call   sub_1_04B7
        dec    ah
        loop   loc_1_06D5
        cmp    word ptr [edi + 0x4c], 0
        jne    loc_1_06F0
        xor    eax, eax
        call   sub_1_05C0
loc_1_06F0:
        test   word ptr [edi + 4], 0x200
        je     loc_1_072A
        add    edx, 0xa
        push   edx
        mov    eax, 6
        out    dx, al
        movzx  edx, word ptr [edi + 0xae]
        mov    ecx, 0x4000
        mov    eax, 0x80
loc_1_0713:
        out    dx, al
        loop   loc_1_0713
        xchg   dword ptr [esp], edx
        mov    al, 7
        out    dx, al
        pop    edx
        mov    ecx, 0x4000
        mov    eax, 0x80
loc_1_0727:
        out    dx, al
        loop   loc_1_0727
loc_1_072A:
        popfd  
        clc    
        popal  
        ret    
sub_1_072E:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 0x42]
        pushfd 
        cli    
        mov    ecx, 0xf
        mov    ah, 0xf
loc_1_073F:
        call   sub_1_0499
        mov    byte ptr [ecx + esi + 8], al
        dec    ah
        loop   loc_1_073F
        mov    ah, 0x1c
        call   sub_1_0499
        mov    byte ptr [esi + 0x24], al
        popfd  
        clc    
        popal  
        ret    
        db  050h, 051h, 052h, 056h, 033h, 0C9h, 00Fh, 0B7h, 0C0h, 03Bh, 081h
        dd  loc_1_0144               ; fixup
        db  076h, 00Fh, 083h, 0C1h, 004h, 083h, 0B9h
        dd  loc_1_0144               ; fixup
        db  0FFh, 075h, 0ECh, 083h, 0E9h, 004h, 08Bh, 081h
        dd  loc_1_017C               ; fixup
        db  089h, 086h, 090h, 000h, 000h, 000h, 0C1h, 0E9h, 002h, 08Ah, 081h
        dd  loc_1_0136               ; fixup
        db  03Ah, 086h, 08Ch, 000h, 000h, 000h, 074h, 056h, 088h, 086h, 08Ch, 000h, 000h, 000h, 00Fh, 0B7h
        db  056h, 042h, 066h, 083h, 07Eh, 04Ch, 000h, 075h, 00Ah, 0B8h, 001h, 000h, 000h, 000h, 0E8h, 00Eh
        db  0FEh, 0FFh, 0FFh, 0E8h, 022h, 0FDh, 0FFh, 0FFh, 072h, 034h, 0B4h, 008h, 08Ah, 086h, 08Ch, 000h
        db  000h, 000h, 0E8h, 0F1h, 0FCh, 0FFh, 0FFh, 0E8h, 087h, 0FDh, 0FFh, 0FFh, 080h, 07Eh, 00Bh, 08Ah
        db  074h, 006h, 080h, 07Eh, 00Bh, 081h, 075h, 008h, 0B9h, 0C8h, 000h, 000h, 000h, 0ECh, 0E2h, 0FDh
        db  066h, 083h, 07Eh, 04Ch, 000h, 075h, 007h, 033h, 0C0h, 0E8h, 0D3h, 0FDh, 0FFh, 0FFh, 05Eh, 05Ah
        db  059h, 058h, 0C3h
sub_1_07F2:
        push   eax
        in     al, dx
        jmp    loc_1_07F6
loc_1_07F6:
        in     al, dx
        jmp    loc_1_07F9
loc_1_07F9:
        pop    eax
        ret    
sub_1_07FB:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_07F2
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_07F2
        pop    edx
        pop    ebx
        ret    
sub_1_0818:
        pushal 
        mov    ecx, 0x8000
        mov    dx, 0x388
loc_1_0822:
        in     al, dx
        loop   loc_1_0822
        popal  
        ret    
sub_1_0827:
        push   ecx
        push   edx
        add    edx, 0xe
        mov    ecx, 0x5dc
loc_1_0831:
        in     al, dx
        or     al, al
        js     loc_1_083B
        loop   loc_1_0831
        stc    
        jmp    loc_1_0840
loc_1_083B:
        sub    edx, 4
        in     al, dx
        clc    
loc_1_0840:
        pop    edx
        pop    ecx
        ret    
sub_1_0843:
        push   eax
        push   ecx
        push   edx
        add    edx, 0xc
        push   eax
        xor    ecx, ecx
        mov    ah, 0xa
loc_1_084E:
        in     al, dx
        or     al, al
        jns    loc_1_085D
        loop   loc_1_084E
        dec    ah
        jne    loc_1_084E
        pop    eax
        stc    
        jmp    loc_1_0860
loc_1_085D:
        pop    eax
        out    dx, al
        clc    
loc_1_0860:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_0864:
        push   eax
        push   edx
        push   ecx
        add    edx, 6
        mov    al, 1
        out    dx, al
        sub    edx, 6
        jmp    loc_1_0872
loc_1_0872:
        jmp    loc_1_0874
loc_1_0874:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_0818
        mov    ecx, 0x600
loc_1_0887:
        call   sub_1_0827
        jb     loc_1_0892
        cmp    al, 0xaa
        je     loc_1_089C
loc_1_0892:
        call   sub_1_0818
        loop   loc_1_0887
        stc    
        jmp    loc_1_089D
loc_1_089C:
        clc    
loc_1_089D:
        pop    ecx
        pop    edx
        pop    eax
        ret    
sub_1_08A1:
        cmp    byte ptr [edi + 0xb4], 0x11
        jne    loc_1_0908
        cmp    byte ptr [edi + 0xb5], 0
        je     loc_1_0908
        pushal 
        movzx  edx, word ptr [edi + 0x40]
        mov    al, 9
        call   sub_1_0843
        jb     loc_1_090A
        mov    al, 1
        call   sub_1_0843
        jb     loc_1_090A
        call   sub_1_0818
        movzx  edx, word ptr [edi + 0x40]
        add    dx, 0x404
        mov    ecx, 0x10
loc_1_08DD:
        in     al, dx
        test   al, 8
        jne    loc_1_08E9
        call   sub_1_0818
        loop   loc_1_08DD
loc_1_08E9:
        mov    byte ptr [edi + 0xb5], 0
        movzx  edx, word ptr [edi + 0x40]
        call   sub_1_0864
        mov    al, 0x40
        call   sub_1_0843
        mov    al, 1
        call   sub_1_0843
        popal  
loc_1_0908:
        clc    
        ret    
loc_1_090A:
        popal  
        stc    
        ret    
sub_1_090D:
        cmp    byte ptr [edi + 0xb4], 0x11
        jne    loc_1_0966
        cmp    byte ptr [edi + 0xb5], 1
        je     loc_1_0966
        pushal 
        movzx  edx, word ptr [edi + 0x40]
        call   sub_1_0864
        jb     loc_1_0968
        mov    al, 9
        call   sub_1_0843
        jb     loc_1_0968
        mov    al, 0
        call   sub_1_0843
        jb     loc_1_0968
        call   sub_1_0818
        movzx  edx, word ptr [edi + 0x40]
        add    dx, 0x404
        mov    ecx, 0x10
loc_1_0950:
        in     al, dx
        test   al, 8
        je     loc_1_095E
        call   sub_1_0818
        loop   loc_1_0950
        jmp    loc_1_0968
loc_1_095E:
        mov    byte ptr [edi + 0xb5], 1
        popal  
loc_1_0966:
        clc    
        ret    
loc_1_0968:
        popal  
        stc    
        ret    
sub_1_096B:
        pushal 
        movzx  edx, word ptr [edi + 0x40]
        mov    ecx, 5
        mov    bx, ax
loc_1_0978:
        mov    al, 8
        call   sub_1_0843
        jb     loc_1_09D3
        mov    al, 8
        call   sub_1_0843
        jb     loc_1_09D3
        mov    al, bh
        call   sub_1_0843
        jb     loc_1_09D3
        mov    al, bl
        call   sub_1_0843
        jb     loc_1_09D3
        call   sub_1_0818
        call   sub_1_0818
        mov    al, 8
        call   sub_1_0843
        jb     loc_1_09D3
        mov    al, 7
        call   sub_1_0843
        jb     loc_1_09D3
        mov    al, bh
        call   sub_1_0843
        jb     loc_1_09D3
        call   sub_1_0818
        call   sub_1_0827
        jb     loc_1_09D3
        cmp    al, bl
        je     loc_1_09D6
        loop   loc_1_0978
loc_1_09D3:
        stc    
        popal  
        ret    
loc_1_09D6:
        clc    
        popal  
        ret    
sub_1_09D9:
        pushal 
        cmp    byte ptr [edi + 0xb4], 0x11
        je     loc_1_09EE
        cmp    byte ptr [edi + 0xb4], 0xc
        je     loc_1_09F7
        jmp    loc_1_0A16
loc_1_09EE:
        mov    ah, 0xb
        mov    ecx, 4
        jmp    loc_1_09FE
loc_1_09F7:
        mov    ah, 0xc
        mov    ecx, 3
loc_1_09FE:
        movzx  edx, word ptr [edi + 0x40]
        add    dx, 0x400
loc_1_0A07:
        in     al, dx
        call   sub_1_096B
        inc    ah
        inc    dx
        loop   loc_1_0A07
        clc    
        popal  
        ret    
loc_1_0A16:
        stc    
        popal  
        ret    
        db  0CCh, 0CCh, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xda  R-X
        db  003h, 000h, 000h, 000h
        dd  obj2:0x0014              ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        dec    edi
        db  000h
loc_2_0010:
        stosd  dword ptr es:[edi], eax
        add    dh, al
        add    byte ptr [ebp + 0x6e], dl
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     loc_2_008C
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    loc_2_0043
        jae    0x94
        jne    0x95
        and    byte ptr fs:[0x65687420], ch
        and    byte ptr [ecx + 0x7a], al
        je     0x98
        arpl   word ptr [eax + 0x20], bp
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    0xa8
        and    byte ptr fs:[ebx + 0x61], al
        jb     0xa4
        and    byte ptr [ecx + 0x73], ch
loc_2_0043:
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0xbb
        jb     loc_2_0076
        popal  
        jo     0xc9
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[ebp + 0x6e], dl
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xdb
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    loc_2_0092
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
loc_2_0076:
        and    byte ptr [0x65687420], ch
        and    byte ptr [ecx + 0x7a], al
        je     0xe6
        arpl   word ptr [eax + 0x20], bp
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    0xf6
        and    byte ptr fs:[ebx + 0x61], al
loc_2_008C:
        jb     0xf2
        and    byte ptr [ebp + 0x49], cl
        inc    esp
loc_2_0092:
        dec    ecx
        and    byte ptr [ebx + 0x79], dl
        outsb  dx, byte ptr [esi]
        je     0x101
        jae    0x105
        jp     0x103
        jb     0xc0
        imul   esi, dword ptr [ebx + 0x20], 0x20657375
        bound  edi, qword ptr [ecx + 0x20]
        popal  
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x117
        jb     0xd2
        popal  
        jo     0x125
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[ecx + 0x7a], al
        je     0x128
        arpl   word ptr [eax + 0x20], bp
        inc    esp
        imul   esi, dword ptr [edx + 0x65], 0x53207463
        outsd  dx, dword ptr [esi]
        jne    0x13f
        and    byte ptr fs:[edx + esi*2 + 0x69], al
        jbe    0x13d
        jb     0xda
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x1c3  R-X
loc_3_0000:
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_02D0
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
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_01EC
loc_3_005C:
        add    esp, 8
        or     edi, edi
        je     loc_3_00C6
        mov    ebx, dword ptr [edi + 0x24]
        or     ebx, ebx
        je     loc_3_00C6
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_00C6
        call   sub_1_0294
loc_3_0077:
        jb     loc_3_00C6
        call   sub_1_072E
loc_3_007E:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_3_00AA
        mov    edi, esi
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        mov    word ptr [edi + 0x30], dx
        mov    dword ptr [edi + 0x28], ecx
        mov    dword ptr [edi + 0x2c], esi
        mov    esi, edi
        test   word ptr [esi + 0x30], 1
        jne    loc_3_00AA
        VxDCall  VDMAD.VDMAD_Mask_Channel        ; cd2007000400
loc_3_00AA:
        pop    edi
        movzx  edx, word ptr [edi + 0x42]
        mov    ah, 9
        xor    al, al
        call   sub_1_04B7
loc_3_00B8:
        inc    ah
        call   sub_1_04B7
loc_3_00BF:
        pop    edx
        pop    ecx
        or     word ptr [edi + 4], 0x10
loc_3_00C6:
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
sub_3_00CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_01EC
loc_3_00DE:
        add    esp, 8
        or     edi, edi
        je     loc_3_01BB
        movzx  eax, word ptr [edi + 6]
        mov    dword ptr [ebp - 4], eax
        movzx  eax, byte ptr [edi + 8]
        mov    dword ptr [ebp - 8], eax
        movzx  eax, byte ptr [edi + 9]
        mov    dword ptr [ebp - 0xc], eax
        movzx  eax, byte ptr [edi + 0xa]
        mov    dword ptr [ebp - 0x10], eax
        test   word ptr [edi + 4], 0x100
        jne    loc_3_013E
        test   word ptr [edi + 4], 0x200
        jne    loc_3_0133
        test   word ptr [edi + 0x52], 4
        je     loc_3_013E
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        call   sub_4_0756
loc_3_012E:
        add    esp, 0x10
        jmp    loc_3_013E
loc_3_0133:
        push   dword ptr [ebp - 4]
        call   sub_4_07B8
loc_3_013B:
        add    esp, 4
loc_3_013E:
        mov    ebx, dword ptr [edi + 0x24]
        or     ebx, ebx
        je     loc_3_01BB
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_01BB
        test   word ptr [edi + 4], 0x10
        je     loc_3_01BB
        call   sub_1_0294
loc_3_015A:
        jb     loc_3_01BB
        or     byte ptr [esi + 0x11], 8
        call   sub_1_0654
loc_3_0165:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_3_018E
        mov    edi, esi
        mov    dx, word ptr [edi + 0x30]
        mov    ecx, dword ptr [edi + 0x28]
        mov    esi, dword ptr [edi + 0x2c]
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
        mov    esi, edi
        test   dl, 1
        jne    loc_3_018E
        VxDCall  VDMAD.VDMAD_UnMask_Channel      ; cd2008000400
loc_3_018E:
        pop    edi
        movzx  edx, word ptr [edi + 0x42]
        mov    ecx, 9
        mov    ah, cl
        and    byte ptr [ecx + esi + 8], 0xf7
        mov    al, byte ptr [ecx + esi + 8]
        call   sub_1_04B7
loc_3_01A8:
        inc    ecx
        inc    ah
        mov    al, byte ptr [ecx + esi + 8]
        call   sub_1_04B7
loc_3_01B4:
        pop    edx
        pop    ecx
        and    word ptr [edi + 4], 0xffef
loc_3_01BB:
        pop    esi
        pop    edi
        pop    ebx
        add    esp, 0x10
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1379  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_01EC
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
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi], 0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_01EC
loc_4_0048:
        add    esp, 8
        or     edi, edi
        je     loc_4_005C
        movzx  eax, word ptr [edi + 4]
        mov    dword ptr [esi], eax
        mov    eax, 0xffffffff
        jmp    loc_4_0061
loc_4_005C:
        mov    eax, 0
loc_4_0061:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_4_0065:
        cmp    eax, 0x40
        jle    loc_4_006F
        mov    eax, 0x40
loc_4_006F:
        add    eax, 3
        shr    eax, 2
loc_4_0075:
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
        jae    loc_4_009A
        mov    edx, eax
loc_4_009A:
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
        jne    loc_4_00C6
        mov    eax, ecx
        shr    eax, 1
        jne    loc_4_0075
        xor    ecx, ecx
        stc    
        ret    
loc_4_00C6:
        shl    ecx, 0xc
        clc    
        ret    
sub_4_00CB:
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_4_0136
        cli    
        VMMCall  VMM.List_Allocate               ; cd209d000100
        sti    
        jb     loc_4_0136
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
        mov    byte ptr [eax + 0x24], 0x4b
        test   word ptr [edi + 4], 0x200
        je     loc_4_0128
        mov    byte ptr [eax + 0xc], 0
        mov    byte ptr [eax + 0xd], 0
loc_4_0128:
        cli    
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        sti    
        clc    
loc_4_0131:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
loc_4_0136:
        stc    
        jmp    loc_4_0131
sub_4_0139:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     loc_4_0171
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_0171
loc_4_0154:
        cmp    dword ptr [eax + 4], edi
        je     loc_4_0163
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_0171
        jmp    loc_4_0154
loc_4_0163:
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_4_0171:
        popfd  
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_4_0177:
        jmp    loc_4_0183
loc_4_0179:
        mov    esi, 0
        jmp    loc_4_023C
loc_4_0183:
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
        je     loc_4_0236
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_01BE:
        push   ecx
        mov    ecx, 0xa
        lea    esi, [0]                      ; -> loc_4_0179
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        loop   loc_4_01BE
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_01DA:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0xa
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_020D
        add    edi, 6
        add    esi, 0xa
        loop   loc_4_01DA
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_4_023C
loc_4_020D:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_0228
        add    edi, 2
loc_4_021A:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_021A
loc_4_0228:
        push   0
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_0236:
        xor    eax, eax
        mov    dword ptr [esp + 0x1c], eax
loc_4_023C:
        popal  
        add    esp, 0xc
        pop    ebp
        ret    
sub_4_0242:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_024C:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_024C
        pop    eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret    
sub_4_026B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_05DA
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_05DA
        mov    dword ptr [ebp - 4], eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    dword ptr [eax], 0
        push   1
        push   0xbe
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_05DA
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x54], eax
        mov    ax, word ptr [ebp + 0x30]
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 0xbc], 0
        mov    ax, word ptr [ebp + 0x34]
        mov    word ptr [edi + 0x52], ax
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [edi + 6], ax
        sub    word ptr [edi + 6], 4
        mov    word ptr [edi + 0x42], ax
        mov    ax, word ptr [ebp + 0x20]
        mov    byte ptr [edi + 8], al
        test   dword ptr [ebp + 0x30], 0x200
        jne    loc_4_0315
        mov    ax, word ptr [ebp + 0x24]
        mov    byte ptr [edi + 9], al
        mov    ax, word ptr [ebp + 0x28]
        mov    byte ptr [edi + 0xa], al
        cmp    ax, word ptr [ebp + 0x24]
        je     loc_4_031A
loc_4_0315:
        or     word ptr [edi + 0x52], 0x40
loc_4_031A:
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edi + 0x4e], ax
        cmp    word ptr [ebp + 0x10], -1
        je     loc_4_034F
        push   dword ptr [ebp + 0x10]
        call   sub_4_0ABF
        add    esp, 4
        or     eax, eax
        je     loc_4_05DA
        or     word ptr [edi + 0x52], 4
        mov    ah, al
        and    ah, 0x38
        shr    ah, 3
        and    al, 7
        mov    word ptr [edi + 0xe], ax
loc_4_034F:
        call   sub_5_0D43
loc_4_0354:
        mov    word ptr [edi + 0xc], ax
        mov    word ptr [2], 4               ; -> obj6:0x018F
        mov    dword ptr [4], 0              ; -> obj6:0x0191
        push   edi
        movzx  ecx, byte ptr [edi + 8]
        mov    word ptr [0], cx              ; -> obj6:0x018D
        mov    dword ptr [0x1c], edi         ; -> obj6:0x01A9
        mov    edi, 0                        ; -> obj6:0x018D
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edi
        jae    loc_4_039D
        push   0                             ; -> obj6:0x0213
        call   sub_5_0000
loc_4_0395:
        add    esp, 4
        jmp    loc_4_05DA
loc_4_039D:
        mov    dword ptr [edi + 0x2c], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 4
        jne    loc_4_03BA
        or     word ptr [edi + 4], 0x8000
loc_4_03BA:
        test   word ptr [edi + 4], 8
        je     loc_4_03CA
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_4_03D6
loc_4_03CA:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_4_03D6:
        test   word ptr [edi + 0x52], 4
        je     loc_4_041F
        movzx  ecx, word ptr [0]             ; -> obj6:0x00CC
        mov    esi, 0                        ; -> obj6:0x00CC
        movzx  edx, word ptr [ebp + 0x10]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_03F4:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_03F4
        mov    edx, dword ptr [esp]
        call   sub_4_0177
        pop    edi
        mov    dword ptr [edi + 0x58], eax
        or     eax, eax
        jne    loc_4_041F
        push   0                             ; -> obj6:0x01AD
        call   sub_5_0000
loc_4_0417:
        add    esp, 4
        jmp    loc_4_05DA
loc_4_041F:
        mov    esi, 0                        ; -> obj6:0x00E6
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_4_0D1F
        mov    byte ptr [edi + 0xb], ah
        xor    ah, ah
        mov    word ptr [edi + 0x4c], ax
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_043F:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_043F
        mov    edx, dword ptr [esp]
        call   sub_4_0177
        pop    edi
        mov    dword ptr [edi + 0x5c], eax
        or     eax, eax
        jne    loc_4_046A
        push   0                             ; -> obj6:0x01C9
        call   sub_5_0000
loc_4_0462:
        add    esp, 4
        jmp    loc_4_05DA
loc_4_046A:
        movsx  edx, word ptr [ebp + 0x18]
        cmp    edx, -1
        je     loc_4_04B5
        mov    word ptr [edi + 0x22], dx
        or     word ptr [edi + 0x52], 8
        mov    esi, 0                        ; -> obj6:0x0100
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_048A:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_048A
        mov    edx, dword ptr [esp]
        call   sub_4_0177
        pop    edi
        mov    dword ptr [edi + 0x60], eax
        or     eax, eax
        jne    loc_4_04B5
        push   0                             ; -> obj6:0x01FA
        call   sub_5_0000
loc_4_04AD:
        add    esp, 4
        jmp    loc_4_05DA
loc_4_04B5:
        movsx  edx, word ptr [ebp + 0x1c]
        cmp    edx, -1
        je     loc_4_0501
        mov    word ptr [edi + 0x40], dx
        or     word ptr [edi + 4], 0x1000
        mov    esi, 0                        ; -> obj6:0x011A
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04D6:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04D6
        mov    edx, dword ptr [esp]
        call   sub_4_0177
        pop    edi
        mov    dword ptr [edi + 0x64], eax
        or     eax, eax
        jne    loc_4_0501
        push   0                             ; -> obj6:0x01E3
        call   sub_5_0000
loc_4_04F9:
        add    esp, 4
        jmp    loc_4_05DA
loc_4_0501:
        test   dword ptr [ebp + 0x30], 0x200
        jne    loc_4_05AA
        mov    eax, dword ptr [ebp + 0x2c]
        call   sub_4_0065
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0x14], ebx
        mov    dword ptr [edi + 0x1c], ecx
        mov    dword ptr [edi + 0x18], edx
        jae    loc_4_0536
        push   0                             ; -> obj6:0x0243
        call   sub_5_0000
loc_4_052E:
        add    esp, 4
        jmp    loc_4_05DA
loc_4_0536:
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
        mov    esi, 0                        ; -> loc_1_0328
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [edi + 0x38], eax
        jae    loc_4_057E
        push   0                             ; -> obj6:0x022B
        call   sub_5_0000
loc_4_0579:
        add    esp, 4
        jmp    loc_4_05DA
loc_4_057E:
        mov    ax, word ptr [ebp + 0x28]
        cmp    ax, word ptr [ebp + 0x24]
        je     loc_4_05AA
        movzx  eax, al
        mov    esi, 0                        ; -> loc_1_0328
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [edi + 0x3c], eax
        jae    loc_4_05AA
        push   0                             ; -> obj6:0x022B
        call   sub_5_0000
loc_4_05A5:
        add    esp, 4
        jmp    loc_4_05DA
loc_4_05AA:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_05B1:
        call   sub_4_00CB
        jb     loc_4_05D9
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_05B1
        pop    ebx
        call   sub_4_1330
        jb     loc_4_05DA
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_05D3:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_05D9:
        pop    ebx
loc_4_05DA:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     loc_4_05E6
        call   sub_4_05F0
loc_4_05E6:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_05D3
sub_4_05F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_0739
        xor    esi, esi
        xchg   dword ptr [edi + 0x44], esi
        or     esi, esi
        je     loc_4_0613
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_0613:
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_4_0620
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_0620:
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_4_062D
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_062D:
        movzx  eax, word ptr [edi + 0x20]
        or     eax, eax
        je     loc_4_0650
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        mov    eax, dword ptr [edi + 0x10]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_0650:
        mov    eax, dword ptr [edi + 0x64]
        or     eax, eax
        je     loc_4_067E
        mov    esi, 0                        ; -> obj6:0x011A
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 0x40]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0669:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0669
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x64]
        call   sub_4_0242
        pop    edi
loc_4_067E:
        mov    eax, dword ptr [edi + 0x60]
        or     eax, eax
        je     loc_4_06AC
        mov    esi, 0                        ; -> obj6:0x0100
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 0x22]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0697:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0697
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x60]
        call   sub_4_0242
        pop    edi
loc_4_06AC:
        mov    eax, dword ptr [edi + 0x5c]
        or     eax, eax
        je     loc_4_06DA
        mov    esi, 0                        ; -> obj6:0x00E6
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 0x42]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_06C5:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_06C5
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x5c]
        call   sub_4_0242
        pop    edi
loc_4_06DA:
        mov    eax, dword ptr [edi + 0x58]
        or     eax, eax
        je     loc_4_0708
        mov    esi, 0                        ; -> obj6:0x00CC
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_06F3:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_06F3
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x58]
        call   sub_4_0242
        pop    edi
loc_4_0708:
        mov    eax, dword ptr [edi + 0x2c]
        or     eax, eax
        je     loc_4_0723
        cli    
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        sti    
loc_4_0723:
        call   sub_1_08A1
loc_4_0728:
        call   sub_1_09D9
loc_4_072D:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_0739:
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
sub_4_0756:
        push   ebp
        mov    ebp, esp
        pushal 
        xor    eax, eax
        xor    ecx, ecx
        mov    al, byte ptr [ebp + 0xc]
loc_4_0761:
        cmp    byte ptr [ecx], 0xff          ; -> obj6:0x0183
        je     loc_4_07B5
        cmp    al, byte ptr [ecx]            ; -> obj6:0x0183
        je     loc_4_0775
        inc    ecx
        jmp    loc_4_0761
loc_4_0775:
        mov    ah, byte ptr [ecx]            ; -> obj6:0x0188
        and    ah, 0x3f
        xor    ecx, ecx
        mov    al, byte ptr [ebp + 0x10]
loc_4_0783:
        cmp    byte ptr [ecx], 0xff          ; -> obj6:0x017C
        je     loc_4_07B5
        cmp    al, byte ptr [ecx]            ; -> obj6:0x017C
        je     loc_4_0797
        inc    ecx
        jmp    loc_4_0783
loc_4_0797:
        or     ah, byte ptr [ecx]            ; -> obj6:0x0180
        mov    cx, word ptr [ebp + 0x10]
        cmp    cx, word ptr [ebp + 0x14]
        je     loc_4_07AA
        or     ah, 4
loc_4_07AA:
        mov    al, ah
        movzx  edx, word ptr [ebp + 8]
        out    dx, al
        clc    
loc_4_07B2:
        popal  
        pop    ebp
        ret    
loc_4_07B5:
        stc    
        jmp    loc_4_07B2
sub_4_07B8:
        push   ebp
        mov    ebp, esp
        pushal 
        movzx  edx, word ptr [ebp + 8]
        xor    al, al
        add    edx, 0xa
        out    dx, al
        mov    ecx, 0x3e8
        sub    edx, 0xa
loc_4_07CE:
        mov    al, 0xa
        out    dx, al
        in     al, dx
        cmp    al, 0xa
        je     loc_4_07D8
        loop   loc_4_07CE
loc_4_07D8:
        popal  
        pop    ebp
        ret    
sub_4_07DB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        movzx  edx, word ptr [ebp + 0xc]
        push   4
        push   edx
        call   sub_1_01EC
loc_4_07F6:
        add    esp, 8
        or     edi, edi
        jne    loc_4_095D
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 0x30], 0x100
        jne    loc_4_0925
        test   dword ptr [ebp + 0x30], 0x200
        jne    loc_4_091A
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_4_0A74
        jb     loc_4_0963
        movsx  edx, word ptr [ebp + 0x18]
        cmp    edx, -1
        je     loc_4_0845
        call   sub_4_0CB4
        jb     loc_4_0963
loc_4_0845:
        movsx  edx, word ptr [ebp + 0x10]
        cmp    edx, -1
        je     loc_4_08AD
        test   dword ptr [ebp + 0x30], 0x400
        jne    loc_4_0897
        push   edx
        call   sub_4_0ABF
        add    esp, 4
        or     eax, eax
        je     loc_4_0963
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x10]
        call   sub_4_0BDC
        add    esp, 8
        or     eax, eax
        je     loc_4_095D
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x10]
        call   sub_4_0C37
        add    esp, 0xc
        or     eax, eax
        je     loc_4_095D
loc_4_0897:
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x10]
        call   sub_4_0756
        add    esp, 0x10
        jmp    loc_4_0925
loc_4_08AD:
        cmp    word ptr [ebp + 0x14], -1
        je     loc_4_08F1
        push   1
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        call   sub_4_0B07
        add    esp, 0xc
        or     eax, eax
        je     loc_4_08E6
        or     dword ptr [ebp - 4], 0x10
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x14]
        call   sub_4_0B5C
        add    esp, 0x10
        or     eax, eax
        je     loc_4_095D
        jmp    loc_4_0925
loc_4_08E6:
        call   sub_4_0C8D
        jb     loc_4_08F1
        or     dword ptr [ebp - 4], 0x30
loc_4_08F1:
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0xc]
        call   sub_4_0E8F
        add    esp, 8
        or     eax, eax
        je     loc_4_095D
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0xc]
        call   sub_4_1047
        add    esp, 0xc
        or     eax, eax
        je     loc_4_095D
        jmp    loc_4_0925
loc_4_091A:
        push   dword ptr [ebp + 0xc]
        call   sub_4_07B8
        add    esp, 4
loc_4_0925:
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
        call   sub_4_026B
        add    esp, 0x30
        or     eax, eax
        je     loc_4_095D
        mov    dword ptr [esp + 0x1c], 0
loc_4_095D:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_0963:
        mov    dword ptr [esp + 0x1c], 0x24
        jmp    loc_4_095D
sub_4_096D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd 
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_01EC
loc_4_097E:
        add    esp, 8
        or     edi, edi
        je     loc_4_0A2C
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     loc_4_09BA
        mov    ebx, dword ptr [edi + 0x24]
        or     ebx, ebx
        je     loc_4_09A9
        mov    eax, 1
        call   sub_5_0BE0
loc_4_09A9:
        mov    ebx, dword ptr [edi + 0x30]
        or     ebx, ebx
        je     loc_4_09BA
        mov    eax, 2
        call   sub_5_0BE0
loc_4_09BA:
        cli    
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_4_09C8
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
loc_4_09C8:
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_4_09D5
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
loc_4_09D5:
        test   word ptr [edi + 0x52], 4
        je     loc_4_09E4
        movzx  edx, word ptr [edi + 6]
        xor    eax, eax
        out    dx, al
loc_4_09E4:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_0A31
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_0A31
loc_4_09F9:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x54], ebx
        je     loc_4_0A0A
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_0A31
        jmp    loc_4_09F9
loc_4_0A0A:
        sti    
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0A12:
        call   sub_4_0139
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0A12
        pop    ebx
        mov    edi, eax
        call   sub_4_05F0
        je     loc_4_0A2C
loc_4_0A2C:
        mov    eax, 0xffffffff
loc_4_0A31:
        popfd  
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_4_0A37:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_03A2
loc_4_0A4E:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_0A54:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_0A37
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        add    esp, 4
        pop    ebp
        stc    
        ret    
        db  0CCh, 0CCh
sub_4_0A74:
        push   eax
        push   ecx
        push   edx
        xor    ecx, ecx
        in     al, dx
        test   al, 0x80
        jne    loc_4_0AB7
        mov    cl, al
        mov    al, 0xc
        out    dx, al
        inc    edx
        xor    al, al
        out    dx, al
        in     al, dx
        test   al, 0x80
        je     loc_4_0A8E
        and    al, 0xbf
loc_4_0A8E:
        cmp    al, 9
        je     loc_4_0AA0
        cmp    al, 0xa
        je     loc_4_0AA0
        cmp    al, 0x81
        je     loc_4_0AA0
        cmp    al, 0x8a
        je     loc_4_0AA0
        jmp    loc_4_0AB3
loc_4_0AA0:
        shl    ecx, 8
        mov    cl, al
        xor    al, al
        out    dx, al
        in     al, dx
        cmp    al, cl
        je     loc_4_0ABA
        mov    al, cl
        out    dx, al
        shr    ecx, 8
loc_4_0AB3:
        dec    edx
        mov    al, cl
        out    dx, al
loc_4_0AB7:
        stc    
        jmp    loc_4_0ABB
loc_4_0ABA:
        clc    
loc_4_0ABB:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_4_0ABF:
        push   ebp
        mov    ebp, esp
        push   edx
        movzx  edx, word ptr [ebp + 8]
        add    dx, 3
        in     al, dx
        and    al, 0x3f
        cmp    al, 4
        jne    loc_4_0AD9
        movzx  eax, al
        clc    
        jmp    loc_4_0ADC
        db  0C3h
loc_4_0AD9:
        xor    eax, eax
        stc    
loc_4_0ADC:
        pop    edx
        pop    ebp
        ret    
sub_4_0ADF:
        jmp    loc_4_0AE7
loc_4_0AE1:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
loc_4_0AE7:
        push   ecx
        push   edi
        push   esi
        mov    esi, 0xfffea
        lea    edi, [0]                      ; -> loc_4_0AE1
        mov    ecx, 6
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_0B02
        clc    
        jmp    loc_4_0B03
loc_4_0B02:
        stc    
loc_4_0B03:
        pop    esi
        pop    edi
        pop    ecx
        ret    
sub_4_0B07:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        call   sub_4_0ADF
        jb     loc_4_0B4C
        movzx  edx, word ptr [ebp + 0xc]
        mov    ah, 0x10
        cmp    word ptr [ebp + 8], 0x608
        jne    loc_4_0B24
        or     ah, 0x20
loc_4_0B24:
        in     al, dx
        cmp    al, 0xff
        je     loc_4_0B31
        and    al, 0x30
        cmp    al, ah
        jne    loc_4_0B31
        jmp    loc_4_0B53
loc_4_0B31:
        movzx  eax, word ptr [ebp + 0x10]
        test   eax, 1
        jne    loc_4_0B4C
        call   sub_4_0C8D
        jb     loc_4_0B4C
        add    edx, 4
        cmp    dx, word ptr [ebp + 8]
        je     loc_4_0B53
loc_4_0B4C:
        mov    eax, 0
        jmp    loc_4_0B58
loc_4_0B53:
        mov    eax, 0xffffffff
loc_4_0B58:
        pop    edx
        pop    ebx
        pop    ebp
        ret    
sub_4_0B5C:
        push   ebp
        mov    ebp, esp
        pushal 
        movzx  edx, word ptr [ebp + 8]
        add    edx, 3
        in     al, dx
        cmp    al, 0xff
        je     loc_4_0BD2
        mov    ah, al
        movzx  ecx, al
        shr    ecx, 4
        or     ecx, ecx
        jne    loc_4_0B7A
        jmp    loc_4_0BD2
loc_4_0B7A:
        inc    ecx
        and    ecx, 3
        mov    al, byte ptr [ecx]            ; -> obj6:0x0183
        cmp    al, byte ptr [ebp + 0xc]
        jne    loc_4_0BD2
        movzx  ecx, ah
        shr    ecx, 6
        jecxz  loc_4_0BD2
        dec    ecx
        mov    al, byte ptr [ecx]            ; -> obj6:0x017C
        cmp    al, byte ptr [ebp + 0x10]
        cmp    word ptr [ebp + 0x14], -1
        je     loc_4_0BC7
        cmp    al, byte ptr [ebp + 0x14]
        je     loc_4_0BC7
        sub    edx, 2
        in     al, dx
        cmp    al, 0xff
        je     loc_4_0BD2
        movzx  ecx, al
        shr    ecx, 6
        or     ecx, ecx
        je     loc_4_0BD2
        jecxz  loc_4_0BD2
        dec    ecx
        mov    al, byte ptr [ecx]            ; -> obj6:0x017C
        cmp    al, byte ptr [ebp + 0x14]
        jne    loc_4_0BD2
loc_4_0BC7:
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_0BCF:
        popal  
        pop    ebp
        ret    
loc_4_0BD2:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_0BCF
sub_4_0BDC:
        push   ebp
        mov    ebp, esp
        pushfd 
        cli    
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0xc]
        xor    ecx, ecx
loc_4_0BED:
        mov    ah, byte ptr [ecx]            ; -> obj6:0x0188
        cmp    ah, 0xff
        je     loc_4_0C2D
        cmp    byte ptr [ecx], al            ; -> obj6:0x0183
        je     loc_4_0C03
        inc    ecx
        jmp    loc_4_0BED
loc_4_0C03:
        xchg   ah, al
        out    dx, al
        add    dx, 3
        in     al, dx
        test   al, 0x40
        je     loc_4_0C2D
        mov    edx, 1
        mov    cl, ah
        shl    edx, cl
        test   al, 0x80
        je     loc_4_0C22
        and    edx, 0x2ff
loc_4_0C22:
        or     edx, edx
        je     loc_4_0C2D
        mov    eax, 0xffffffff
        jmp    loc_4_0C32
loc_4_0C2D:
        mov    eax, 0
loc_4_0C32:
        pop    edx
        pop    ecx
        popfd  
        pop    ebp
        ret    
sub_4_0C37:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0x10]
        mov    ah, byte ptr [ebp + 0xc]
        xor    ecx, ecx
loc_4_0C48:
        cmp    byte ptr [ecx], 0xff          ; -> obj6:0x017C
        je     loc_4_0C84
        cmp    byte ptr [ecx], ah            ; -> obj6:0x017C
        je     loc_4_0C5C
        inc    ecx
        jmp    loc_4_0C48
loc_4_0C5C:
        push   eax
        add    dx, 3
        in     al, dx
        test   al, 0x80
        pop    eax
        je     loc_4_0C6B
        or     ah, ah
        je     loc_4_0C84
loc_4_0C6B:
        cmp    al, ah
        je     loc_4_0C7D
        or     ah, ah
        je     loc_4_0C79
        or     al, al
        jne    loc_4_0C84
        jmp    loc_4_0C7D
loc_4_0C79:
        cmp    al, 1
        jne    loc_4_0C84
loc_4_0C7D:
        mov    eax, 0xffffffff
        jmp    loc_4_0C89
loc_4_0C84:
        mov    eax, 0
loc_4_0C89:
        pop    edx
        pop    ecx
        pop    ebp
        ret    
sub_4_0C8D:
        pushal 
        call   sub_4_0ADF
        jb     loc_4_0CB1
        push   edx
        mov    edx, 0xc7c
        in     al, dx
        pop    edx
        mov    bl, al
        mov    bh, bl
        and    bh, 0xf0
        cmp    bh, 0x10
        jne    loc_4_0CB1
        test   bl, 3
        jne    loc_4_0CB1
        popal  
        clc    
        ret    
loc_4_0CB1:
        popal  
        stc    
        ret    
sub_4_0CB4:
        push   eax
        push   ebx
        push   ecx
        pushfd 
        cli    
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_07FB
loc_4_0CC5:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_07FB
loc_4_0CD1:
        mov    ebx, 2
        mov    al, 0x80
        call   sub_1_07FB
loc_4_0CDD:
        mov    ebx, 4
        mov    al, 1
        call   sub_1_07FB
loc_4_0CE9:
        mov    cx, 0x4000
loc_4_0CED:
        in     al, dx
        test   al, 0x40
        jne    loc_4_0CF4
        loop   loc_4_0CED
loc_4_0CF4:
        push   eax
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_07FB
loc_4_0D01:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_07FB
loc_4_0D0D:
        pop    eax
        popfd  
        test   al, 0x40
        je     loc_4_0D1A
        test   al, 6
        jne    loc_4_0D1A
        clc    
        jmp    loc_4_0D1B
loc_4_0D1A:
        stc    
loc_4_0D1B:
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_4_0D1F:
        push   ebx
        mov    bh, 9
        mov    ah, 0xc
        call   sub_1_0499
loc_4_0D29:
        jb     loc_4_0D4B
        mov    bh, al
        cmp    al, 0xa
        je     loc_4_0D4F
        cmp    al, 0x8a
        jne    loc_4_0D4B
        mov    al, 0x40
        mov    ah, 0xc
        call   sub_1_04B7
loc_4_0D3E:
        jmp    loc_4_0D40
loc_4_0D40:
        call   sub_1_0499
loc_4_0D45:
        test   al, 0x40
        jne    loc_4_0D53
        jmp    loc_4_0D4F
loc_4_0D4B:
        mov    bl, 0
        jmp    loc_4_0D55
loc_4_0D4F:
        mov    bl, 1
        jmp    loc_4_0D55
loc_4_0D53:
        mov    bl, 2
loc_4_0D55:
        movzx  eax, bx
        pop    ebx
        ret    
sub_4_0D5A:
        push   eax
        push   ecx
        push   edx
        mov    eax, 1
        call   sub_1_05C0
loc_4_0D67:
        mov    ah, 6
        call   sub_1_0499
loc_4_0D6E:
        shl    eax, 8
        push   eax
        mov    ah, 7
        call   sub_1_0499
loc_4_0D79:
        movzx  eax, al
        or     dword ptr [esp], eax
        mov    eax, 0x80
        mov    ah, 6
        call   sub_1_04B7
loc_4_0D8B:
        inc    ah
        call   sub_1_04B7
loc_4_0D92:
        mov    byte ptr [0], 0x40            ; -> obj1:0x0114
        mov    eax, 0x4b
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    ah, 8
        call   sub_1_0499
loc_4_0DAC:
        push   eax
        mov    al, 3
        call   sub_1_04B7
loc_4_0DB4:
        xor    eax, eax
        mov    ah, 0xf
        call   sub_1_04B7
loc_4_0DBD:
        dec    ah
        call   sub_1_04B7
loc_4_0DC4:
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        or     al, 2
        call   sub_1_04B7
loc_4_0DD2:
        mov    ah, 9
        mov    al, 0x41
        call   sub_1_04B7
loc_4_0DDB:
        mov    eax, 0xb
        add    edx, 0
        out    dx, al
        mov    byte ptr [0], 0               ; -> obj1:0x0114
        call   sub_1_0480
loc_4_0DF0:
        add    edx, 3
        xor    eax, eax
        out    dx, al
        sub    edx, 3
        add    edx, 2
        mov    ecx, 0x1000
loc_4_0E01:
        in     al, dx
        test   al, 1
        jne    loc_4_0E0F
        loop   loc_4_0E01
        mov    ecx, 0x1f4
loc_4_0E0D:
        loop   loc_4_0E0D
loc_4_0E0F:
        sub    edx, 2
        or     byte ptr [0], 0x40            ; -> obj1:0x0114
        mov    eax, 0xb
        or     al, byte ptr [0]              ; -> obj1:0x0114
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    ah, 9
        xor    al, al
        call   sub_1_04B7
loc_4_0E34:
        pop    eax
        call   sub_1_04B7
loc_4_0E3A:
        and    byte ptr [0], 0xbf            ; -> obj1:0x0114
        mov    eax, 0xb
        or     al, byte ptr [0]              ; -> obj1:0x0114
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    eax, dword ptr [esp]
        mov    ah, 7
        call   sub_1_04B7
loc_4_0E5D:
        pop    eax
        shr    eax, 8
        mov    ah, 6
        call   sub_1_04B7
loc_4_0E68:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_4_0E6C:
        push   eax
        add    edx, 2
        in     al, dx
        sub    edx, 2
        test   al, 1
        je     loc_4_0E81
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_4_0E81:
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        call   sub_1_04B7
loc_4_0E8D:
        pop    eax
        ret    
sub_4_0E8F:
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
        jmp    loc_4_0EBB
loc_4_0EBB:
        out    0x20, al
        jmp    loc_4_0EBF
loc_4_0EBF:
        call   sub_4_0E6C
        in     al, 0xa0
        mov    ah, al
        in     al, 0x20
        test   ecx, eax
        jne    loc_4_0EE7
        call   sub_4_0D5A
        push   ecx
        mov    ecx, 0x1f4
loc_4_0ED9:
        in     al, dx
        loop   loc_4_0ED9
        pop    ecx
        in     al, 0xa0
        mov    ah, al
        in     al, 0x20
        test   ecx, eax
        jne    loc_4_0EF3
loc_4_0EE7:
        push   ecx
        push   eax
        call   sub_5_0051
loc_4_0EEE:
        add    esp, 8
        xor    eax, eax
loc_4_0EF3:
        call   sub_4_0E6C
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
sub_4_0F0B:
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
        jne    loc_4_0F3A
        add    al, 0x48
        jmp    loc_4_0F3C
loc_4_0F3A:
        add    al, 0x44
loc_4_0F3C:
        out    0xb, al
        mov    edx, ebx
        shl    edx, 1
        add    edx, 0
        mov    eax, dword ptr [ebp + 0x10]
        out    dx, al
        shr    eax, 8
        out    dx, al
        movzx  ebx, word ptr [ebp + 0xc]
        movzx  edx, byte ptr [ebx]           ; -> obj6:0x0260
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
        call   sub_1_04B7
loc_4_0F7D:
        mov    ah, 0xe
        xor    al, al
        call   sub_1_04B7
loc_4_0F86:
        mov    ah, 6
        mov    al, 0x3f
        call   sub_1_04B7
loc_4_0F8F:
        mov    ah, 7
        call   sub_1_04B7
loc_4_0F96:
        mov    ah, 0xa
        mov    al, byte ptr [0]              ; -> loc_1_0115
        call   sub_1_04B7
loc_4_0FA2:
        mov    byte ptr [0], 0x40            ; -> obj1:0x0114
        mov    eax, 0x4b
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    al, 1
        test   dword ptr [ebp + 0x14], 1
        je     loc_4_0FC2
        mov    al, 2
loc_4_0FC2:
        test   dword ptr [ebp + 0x14], 2
        je     loc_4_0FD8
        or     al, 4
        test   dword ptr [ebp + 0x14], 1
        je     loc_4_0FD8
        or     al, 0x40
loc_4_0FD8:
        mov    ah, 9
        call   sub_1_04B7
loc_4_0FDF:
        mov    eax, 0xb
        add    edx, 0
        out    dx, al
        mov    byte ptr [0], 0               ; -> obj1:0x0114
        call   sub_1_0480
loc_4_0FF4:
        mov    edx, ebx
        shl    edx, 1
        add    edx, 1
        mov    ecx, 0xffff
loc_4_1000:
        xor    al, al
        out    0xc, al
        jmp    loc_4_1006
loc_4_1006:
        in     al, dx
        mov    ah, al
        jmp    loc_4_100B
loc_4_100B:
        in     al, dx
        cmp    ax, -1
        je     loc_4_1026
        loop   loc_4_1000
        push   eax
        push   ebx
        call   sub_5_007E
loc_4_101B:
        add    esp, 8
        mov    dword ptr [esp + 0x1c], 0
loc_4_1026:
        movzx  edx, word ptr [ebp + 8]
        mov    ah, 9
        xor    al, al
        call   sub_1_04B7
loc_4_1033:
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
sub_4_1047:
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
        je     loc_4_10C1
        mov    edi, edx
        mov    ecx, 0x400
        mov    eax, 0x80808080
        cld    
        rep stosd dword ptr es:[edi], eax
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_0F0B
        add    esp, 0x10
        or     eax, eax
        je     loc_4_10C1
        mov    ecx, 1
        mov    ax, word ptr [ebp + 0xc]
        cmp    ax, word ptr [ebp + 0x10]
        jne    loc_4_10AF
        or     ecx, 2
loc_4_10AF:
        push   ecx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_4_0F0B
        add    esp, 0x10
loc_4_10C1:
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [ebp - 8]
        or     eax, eax
        je     loc_4_10D8
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_10D8:
        popal  
        add    esp, 8
        pop    ebp
        ret    
        db  0CCh, 0CCh
sub_4_10E0:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        xor    ah, ah
        mov    dx, word ptr [ebp + 8]
        cmp    dx, 0x220
        je     loc_4_10FB
        inc    ah
loc_4_10FB:
        mov    dx, 0x620
        in     al, dx
        test   al, 1
        je     loc_4_1108
        mov    dx, 0x640
loc_4_1108:
        add    dx, 4
        in     al, dx
        or     al, 0x80
        out    dx, al
        sub    dx, 4
        in     al, dx
        and    al, 0xfe
        or     al, ah
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x404
        in     al, dx
        and    al, 0x7f
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        call   sub_1_0864
loc_4_1130:
        jb     loc_4_1325
        mov    dx, word ptr [ebp + 8]
        mov    al, 8
        call   sub_1_0843
loc_4_1141:
        jb     loc_4_1325
        mov    al, 5
        call   sub_1_0843
loc_4_114E:
        jb     loc_4_1325
        call   sub_1_0827
loc_4_1159:
        jb     loc_4_1325
        cmp    al, 0x12
        jne    loc_4_1165
        mov    al, 0x11
loc_4_1165:
        cmp    al, 0x11
        jne    loc_4_1272
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x404
        in     al, dx
        or     al, 0x80
        out    dx, al
        xor    ah, ah
        mov    bx, word ptr [ebp + 0x10]
        cmp    bx, 0xa
        jne    loc_4_118B
        or     ah, 0x20
        jmp    loc_4_11AA
loc_4_118B:
        cmp    bx, 7
        jne    loc_4_1196
        or     ah, 0x10
        jmp    loc_4_11AA
loc_4_1196:
        cmp    bx, 5
        jne    loc_4_11A1
        or     ah, 8
        jmp    loc_4_11AA
loc_4_11A1:
        cmp    bx, 9
        jne    loc_4_11AA
        or     ah, 4
loc_4_11AA:
        mov    bx, word ptr [ebp + 0x14]
        cmp    bx, 3
        jne    loc_4_11B9
        or     ah, 0xc0
        jmp    loc_4_11CD
loc_4_11B9:
        cmp    bx, 1
        jne    loc_4_11C4
        or     ah, 0x80
        jmp    loc_4_11CD
loc_4_11C4:
        cmp    bx, 0
        jne    loc_4_11CD
        or     ah, 0x40
loc_4_11CD:
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x400
        in     al, dx
        and    al, 3
        or     al, ah
        out    dx, al
        xor    ah, ah
        mov    bx, word ptr [ebp + 0xc]
        cmp    bx, 0x530
        jne    loc_4_11EE
        or     ah, 4
        jmp    loc_4_1210
loc_4_11EE:
        cmp    bx, 0x604
        jne    loc_4_11FA
        or     ah, 5
        jmp    loc_4_1210
loc_4_11FA:
        cmp    bx, 0xe80
        jne    loc_4_1206
        or     ah, 6
        jmp    loc_4_1210
loc_4_1206:
        cmp    bx, 0xf40
        jne    loc_4_1210
        or     ah, 7
loc_4_1210:
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x401
        in     al, dx
        and    al, 0xf8
        or     al, ah
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x404
        in     al, dx
        and    al, 0x7f
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        mov    al, 9
        call   sub_1_0843
loc_4_1237:
        jb     loc_4_1325
        mov    al, 0
        call   sub_1_0843
loc_4_1244:
        jb     loc_4_1325
        call   sub_1_0818
loc_4_124F:
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x404
        mov    ecx, 0x10
loc_4_125D:
        in     al, dx
        test   al, 8
        je     loc_4_131A
        call   sub_1_0818
loc_4_126B:
        loop   loc_4_125D
        jmp    loc_4_1325
loc_4_1272:
        cmp    al, 0xc
        jne    loc_4_1318
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x404
        in     al, dx
        or     al, 0x80
        out    dx, al
        xor    ah, ah
        mov    bx, word ptr [ebp + 0x10]
        cmp    bx, 7
        jne    loc_4_1298
        or     ah, 8
        jmp    loc_4_12B7
loc_4_1298:
        cmp    bx, 5
        jne    loc_4_12A3
        or     ah, 4
        jmp    loc_4_12B7
loc_4_12A3:
        cmp    bx, 3
        jne    loc_4_12AE
        or     ah, 2
        jmp    loc_4_12B7
loc_4_12AE:
        cmp    bx, 9
        jne    loc_4_12B7
        or     ah, 1
loc_4_12B7:
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x401
        in     al, dx
        and    al, 0xf0
        or     al, ah
        out    dx, al
        xor    ah, ah
        mov    bx, word ptr [ebp + 0xc]
        cmp    bx, 0x530
        jne    loc_4_12D8
        or     ah, 4
        jmp    loc_4_12FA
loc_4_12D8:
        cmp    bx, 0x604
        jne    loc_4_12E4
        or     ah, 5
        jmp    loc_4_12FA
loc_4_12E4:
        cmp    bx, 0xe80
        jne    loc_4_12F0
        or     ah, 6
        jmp    loc_4_12FA
loc_4_12F0:
        cmp    bx, 0xf40
        jne    loc_4_12FA
        or     ah, 7
loc_4_12FA:
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x402
        in     al, dx
        and    al, 0xf8
        or     al, ah
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        add    dx, 0x404
        in     al, dx
        and    al, 0x7f
        out    dx, al
        jmp    loc_4_131A
loc_4_1318:
        jmp    loc_4_1325
loc_4_131A:
        mov    dword ptr [esp + 0x1c], 0
        popal  
        pop    ebp
        ret    
loc_4_1325:
        mov    dword ptr [esp + 0x1c], 0x24
        popal  
        pop    ebp
        ret    
sub_4_1330:
        pushal 
        movzx  edx, word ptr [edi + 0x40]
        mov    al, 8
        call   sub_1_0843
loc_4_133C:
        jb     loc_4_1376
        mov    al, 5
        call   sub_1_0843
loc_4_1345:
        jb     loc_4_1376
        call   sub_1_0827
loc_4_134C:
        jb     loc_4_1376
        cmp    al, 0x12
        jne    loc_4_1354
        mov    al, 0x11
loc_4_1354:
        cmp    al, 0x11
        jne    loc_4_1368
        mov    byte ptr [edi + 0xb4], 0x11
        mov    byte ptr [edi + 0xb5], 1
        jmp    loc_4_1373
loc_4_1368:
        cmp    al, 0xc
        jne    loc_4_1376
        mov    byte ptr [edi + 0xb4], 0xc
loc_4_1373:
        clc    
        popal  
        ret    
loc_4_1376:
        stc    
        popal  
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x1e0d  R-X
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
        push   0                             ; -> obj6:0x0008
        lea    edx, [ebp - 0x80]
        push   edx
        call   sub_5_1CC4
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
        push   0                             ; -> obj6:0x0024
        lea    edx, [ebp - 0x80]
        push   edx
        call   sub_5_1CC4
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
        call   sub_5_1CFC
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_027A
        mov    edi, 0                        ; -> obj6:0x0058
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
        call   sub_5_1CEC
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
        call   sub_5_1CFC
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
        call   sub_5_1CEC
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
        push   0                             ; -> obj6:0x0048
        push   dword ptr [ebp - 0xc]
        call   sub_5_1CF4
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
        push   0                             ; -> obj6:0x0040
        push   dword ptr [ebp - 0x10]
        call   sub_5_1CEC
        add    esp, 0xc
        test   eax, eax
        je     loc_5_01E2
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj6:0x0040
        push   dword ptr [ebp - 0x10]
        call   sub_5_1CE4
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
        push   0                             ; -> obj6:0x0048
        push   dword ptr [ebp - 8]
        call   sub_5_1CDC
        add    esp, 0x18
        push   dword ptr [ebp - 8]
        call   sub_5_1CCC
        mov    dword ptr [ebp - 8], 0
        add    esp, 4
loc_5_021F:
        push   dword ptr [ebp - 0xc]
        call   sub_5_1CCC
        add    esp, 4
        lea    eax, [ebp - 0x114]
        mov    dword ptr [ebp - 0xc], 0
        push   eax
        push   0x80000002
        call   sub_5_1CD4
        add    esp, 8
loc_5_0245:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_0256
        push   dword ptr [ebp - 0xc]
        call   sub_5_1CCC
        add    esp, 4
loc_5_0256:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_5_0267
        push   dword ptr [ebp - 0x10]
        call   sub_5_1CCC
        add    esp, 4
loc_5_0267:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_0278
        push   dword ptr [ebp - 8]
loc_5_0270:
        call   sub_5_1CCC
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
        push   0                             ; -> obj6:0x0048
        push   0                             ; -> obj6:0x0040
        push   dword ptr [ebp + 8]
        call   sub_5_1D04
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
        push   0
        lea    ecx, [ebp - 0xd8]
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_5_1D0C
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_039E
        cmp    word ptr [ebp - 0x7c], 3
        jne    loc_5_0399
        cmp    word ptr [ebp - 0x52], 0x10
        jne    loc_5_0399
        cmp    word ptr [ebp - 0x50], 8
        jne    loc_5_0399
        cmp    word ptr [ebp - 0x4e], 4
        jne    loc_5_0399
        mov    cx, word ptr [ebp - 0x7a]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dx, word ptr [ebp - 0x78]
        mov    word ptr [eax], cx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, dx
        add    ax, 4
        mov    word ptr [ecx], dx
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    word ptr [ebp - 0x2a], 2
        mov    dx, word ptr [ebp - 0x76]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax], dx
        jb     loc_5_0399
        cmp    word ptr [ebp - 0x1a], 2
        jb     loc_5_0399
        mov    eax, dword ptr [ebp + 0x24]
        movzx  dx, byte ptr [ebp - 0x28]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 0x20]
        movzx  dx, byte ptr [ebp - 0x27]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 0x30]
        movzx  dx, byte ptr [ebp - 0x18]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 0x28]
        movzx  dx, byte ptr [ebp - 0x17]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 0x2c]
        mov    word ptr [eax], dx
        movzx  eax, word ptr [ecx]
        inc    eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x13
        jmp    loc_5_039E
loc_5_0399:
        mov    eax, 0x13
loc_5_039E:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_03A2:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x16c
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_5_03F1
        cmp    eax, 1
        je     loc_5_0541
        cmp    eax, 2
        je     loc_5_06CA
        cmp    eax, 3
        je     loc_5_0717
        cmp    eax, 4
        je     loc_5_06CA
        cmp    eax, 8
        je     loc_5_075B
        cmp    eax, 0xd
        je     loc_5_06CA
        jmp    loc_5_078E
loc_5_03F1:
        mov    di, 0xffff
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 0x4c]
        push   ebx
        lea    ecx, [ebp - 0x34]
        push   eax
        mov    esi, dword ptr [ebp + 0x10]
        push   ecx
        mov    dword ptr [ebp - 0x20], ebx
        push   esi
        mov    dword ptr [ebp - 0x4c], ebx
        call   sub_5_1D14
        add    esp, 0x14
        test   eax, eax
        je     loc_5_041A
        mov    dword ptr [ebp - 0x34], ebx
loc_5_041A:
        cmp    dword ptr [ebp - 0x34], 1
        je     loc_5_042D
        cmp    dword ptr [ebp - 0x34], 0
        je     loc_5_042D
        xor    eax, eax
        jmp    loc_5_0793
loc_5_042D:
        push   1
        lea    eax, [ebp - 0x28]
        push   esi
        push   eax
        call   sub_5_1D2C
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_0449
        mov    eax, ebx
        jmp    loc_5_0793
loc_5_0449:
        push   0
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    edx, [ebp - 0x18]
        push   0
        push   dword ptr [ebp - 0x28]
        push   edx
        call   sub_5_1D24
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_0529
        xor    esi, esi
        test   ebx, ebx
        jne    loc_5_04B4
loc_5_046E:
        mov    eax, dword ptr [ebp - 0x1c]
        cmp    eax, 2
        je     loc_5_047D
        cmp    eax, 4
        je     loc_5_0494
        jmp    loc_5_0497
loc_5_047D:
        mov    eax, dword ptr [ebp - 0x18]
        cmp    word ptr [eax + 0xe], 8
        jne    loc_5_0497
        mov    dword ptr [ebp - 0x20], 1
        mov    di, word ptr [eax + 0x10]
        jmp    loc_5_0497
loc_5_0494:
        mov    esi, dword ptr [ebp - 0x18]
loc_5_0497:
        push   0
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    edx, [ebp - 0x18]
        push   0
        push   dword ptr [ebp - 0x18]
        push   edx
        call   sub_5_1D24
        add    esp, 0x14
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_046E
loc_5_04B4:
        cmp    ebx, 0xf
        jne    loc_5_0533
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_5_053A
        test   esi, esi
        je     loc_5_053A
        push   edi
        call   sub_4_0ABF
loc_5_04C9:
        add    esp, 4
        test   eax, eax
        je     loc_5_053A
        xor    ebx, ebx
loc_5_04D2:
        mov    edx, 1
        mov    cl, bl
        shl    edx, cl
        movzx  eax, word ptr [esi + 4]
        test   edx, eax
        je     loc_5_0501
        push   ebx
        push   edi
        call   sub_4_0BDC
loc_5_04EA:
        add    esp, 8
        test   eax, eax
        jne    loc_5_0501
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        not    ax
        and    word ptr [esi + 4], ax
loc_5_0501:
        inc    ebx
        cmp    ebx, 0x10
        jl     loc_5_04D2
        push   0
        lea    ecx, [ebp - 0x28]
        push   dword ptr [ebp - 0x28]
        push   ecx
        call   sub_5_1D1C
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_0449
        xor    eax, eax
        jmp    loc_5_0793
loc_5_0529:
        mov    eax, 0x13
        jmp    loc_5_0793
loc_5_0533:
        mov    eax, ebx
        jmp    loc_5_0793
loc_5_053A:
        xor    eax, eax
        jmp    loc_5_0793
loc_5_0541:
        push   0
        lea    eax, [ebp - 0x16c]
        push   0x100
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x2c], 0
        push   eax
        push   0
        push   esi
        call   sub_5_1CFC
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_0793
        xor    edi, edi
        lea    eax, [ebp - 0x48]
        push   edi
        lea    ecx, [ebp - 0x30]
        push   edi
        mov    dword ptr [ebp - 0x48], edi
        push   eax
        push   ecx
        push   esi
        call   sub_5_1D14
        add    esp, 0x14
        test   eax, eax
        je     loc_5_058B
        mov    dword ptr [ebp - 0x30], edi
loc_5_058B:
        mov    eax, 0xffff
        mov    word ptr [ebp - 6], ax
        mov    word ptr [ebp - 0xc], ax
        mov    word ptr [ebp - 0xa], ax
        mov    word ptr [ebp - 0xe], ax
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp - 0x30]
        cmp    eax, 0x10
        je     loc_5_05B1
        cmp    eax, 0x40
        jne    loc_5_05C9
loc_5_05B1:
        mov    eax, dword ptr [ebp - 0x30]
        sub    eax, 0x10
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xfffffd00
        add    eax, 0x400
        or     dword ptr [ebp - 0x2c], eax
loc_5_05C9:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x14]
        push   eax
        lea    edx, [ebp - 0x12]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        lea    ecx, [ebp - 0x10]
        push   eax
        lea    edx, [ebp - 2]
        push   ecx
        lea    eax, [ebp - 0xe]
        push   edx
        lea    ecx, [ebp - 0xa]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        lea    eax, [ebp - 6]
        push   edx
        push   eax
        push   esi
        call   sub_5_02D3
        add    esp, 0x2c
        test   eax, eax
        jne    loc_5_0793
        push   esi
        call   sub_5_00AB
        add    esp, 4
        test   eax, eax
        jne    loc_5_0793
        lea    eax, [ebp - 0x2c]
        push   eax
        push   esi
        call   sub_5_0288
        add    esp, 8
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 2]
        call   sub_4_10E0
loc_5_0631:
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_0793
        push   dword ptr [ebp - 0x2c]
        push   0x20
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x12]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 2]
        push   dword ptr [ebp - 0xe]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 0xa]
        push   esi
        call   sub_4_07DB
loc_5_065F:
        add    esp, 0x2c
        mov    edi, eax
        test   edi, edi
        jne    loc_5_06C3
        cmp    word ptr [ebp - 4], 9
        mov    eax, 2
        je     loc_5_0678
        movzx  eax, word ptr [ebp - 4]
loc_5_0678:
        movzx  ecx, word ptr [ebp - 8]
        movzx  edx, word ptr [ebp - 2]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 0x6c]
        push   0                             ; -> obj6:0x0068
        push   ecx
        call   sub_5_1CC4
        add    esp, 0x14
        lea    ecx, [ebp - 0x6c]
        mov    dword ptr [ebp - 0x40], 0     ; -> obj6:0x0060
        mov    dword ptr [ebp - 0x44], ecx
        push   dword ptr [ebp - 0x44]
        push   dword ptr [ebp - 0x40]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
        test   edi, edi
        jne    loc_5_06C3
        push   esi
        call   sub_5_1914
        add    esp, 4
loc_5_06C3:
        mov    eax, edi
        jmp    loc_5_0793
loc_5_06CA:
        lea    eax, [ebp - 0x3c]
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        call   sub_4_0030
loc_5_06D7:
        add    esp, 8
        test   eax, eax
        je     loc_5_078E
        test   byte ptr [ebp - 0x3b], 0x10
        je     loc_5_0701
        mov    dword ptr [ebp - 0x38], 0     ; -> obj6:0x0060
        push   dword ptr [ebp - 0x38]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0004h              ; cd2004004a04
        add    esp, 4
loc_5_0701:
        push   esi
        call   sub_5_19EF
        add    esp, 4
        push   esi
        call   sub_4_096D
loc_5_0710:
        add    esp, 4
        xor    eax, eax
        jmp    loc_5_0793
loc_5_0717:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_0725
        cmp    eax, 1
        je     loc_5_0740
        jmp    loc_5_078E
loc_5_0725:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_072D:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_0793
loc_5_0740:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_0748:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_0793
loc_5_075B:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x7fffffff
        cmp    eax, 4
        jb     loc_5_078E
        cmp    eax, 5
        jbe    loc_5_0779
        cmp    eax, 6
        jb     loc_5_078E
        cmp    eax, 8
        jbe    loc_5_0783
        jmp    loc_5_078E
loc_5_0779:
        push   dword ptr [ebp + 0x10]
        call   sub_3_004C
loc_5_0781:
        jmp    loc_5_078B
loc_5_0783:
        push   dword ptr [ebp + 0x10]
        call   sub_3_00CB
loc_5_078B:
        add    esp, 4
loc_5_078E:
        mov    eax, 1
loc_5_0793:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
loc_5_079C:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_07A6
        jmp    loc_5_07B8
loc_5_07A6:
        cmp    ecx, 4
        jbe    loc_5_07B1
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_07B1:
        je     loc_5_07B4
        in     al, dx
loc_5_07B4:
        ret    
        db  02Eh, 08Bh, 0C0h
loc_5_07B8:
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        cmp    eax, ebx
        je     loc_5_07D8
        or     eax, eax
        jne    loc_5_07EB
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_0C76
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_07EB
loc_5_07D8:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_07E4
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_07E4:
        je     loc_5_07E9
        in     al, dx
        jmp    loc_5_07EA
loc_5_07E9:
        out    dx, al
loc_5_07EA:
        ret    
loc_5_07EB:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_0AC1
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
loc_5_07FC:
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        cmp    eax, ebx
        je     loc_5_081C
        or     eax, eax
        jne    loc_5_0836
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_0C76
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0836
loc_5_081C:
        pop    eax
        mov    edi, esi
        call   sub_1_08A1
loc_5_0824:
        cmp    ecx, 4
        jbe    loc_5_082F
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_082F:
        je     loc_5_0834
        in     al, dx
        jmp    loc_5_0835
loc_5_0834:
        out    dx, al
loc_5_0835:
        ret    
loc_5_0836:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_0AC1
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_0847:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 0x22]
        mov    ebx, 0x105
        mov    al, 1
        call   sub_1_07FB
loc_5_085B:
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_07FB
loc_5_0867:
        mov    ebx, 0x104
        mov    al, 0x3f
        call   sub_1_07FB
loc_5_0873:
        mov    ebx, 8
        xor    al, al
        call   sub_1_07FB
loc_5_087F:
        mov    ebx, 0xbd
        mov    al, 0xc0
        call   sub_1_07FB
loc_5_088B:
        mov    ecx, 0x15
        mov    ebx, 0x40
        mov    al, 0x3f
loc_5_0897:
        xor    bh, bh
        call   sub_1_07FB
loc_5_089E:
        mov    bh, 1
        call   sub_1_07FB
loc_5_08A5:
        inc    bl
        loop   loc_5_0897
        mov    ecx, 8
        mov    ebx, 0xb0
        xor    eax, eax
loc_5_08B5:
        xor    bh, bh
        call   sub_1_07FB
loc_5_08BC:
        mov    bh, 1
        call   sub_1_07FB
loc_5_08C3:
        inc    bl
        loop   loc_5_08B5
        mov    ebx, 0x104
        xor    eax, eax
        call   sub_1_07FB
loc_5_08D3:
        mov    ebx, 0x105
        call   sub_1_07FB
loc_5_08DD:
        call   sub_1_07F2
loc_5_08E2:
        call   sub_1_07F2
loc_5_08E7:
        call   sub_1_07F2
loc_5_08EC:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret    
loc_5_08F1:
        mov    edi, edx
        mov    ebx, dword ptr [edi + 0x30]
        or     ebx, ebx
        je     loc_5_0905
        movzx  edx, word ptr [edi + 0x22]
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        ret    
loc_5_0905:
        ret    
        db  08Bh, 0C0h
loc_5_0908:
        test   word ptr [esi + 4], 0x2000
        je     loc_5_0930
        cmp    ecx, 4
        jbe    loc_5_091B
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_091B:
        je     loc_5_0966
        and    word ptr [esi + 4], 0xdfff
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        mov    al, byte ptr [esi + 0xac]
        ret    
loc_5_0930:
        push   eax
        mov    eax, dword ptr [esi + 0x30]
        cmp    eax, ebx
        je     loc_5_0955
        or     eax, eax
        jne    loc_5_0968
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_0C76
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0968
        call   sub_5_0847
loc_5_0955:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0961
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0961:
        je     loc_5_0966
        in     al, dx
        jmp    loc_5_0967
loc_5_0966:
        out    dx, al
loc_5_0967:
        ret    
loc_5_0968:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_0AC1
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_0979:
        push   esi
        push   eax
        push   ecx
        push   edx
        test   word ptr [edi + 0x52], 4
        je     loc_5_09A0
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_098F:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_09A0
        inc    cx
        jmp    loc_5_098F
loc_5_09A0:
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x42]
loc_5_09AA:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_09BB
        inc    cx
        jmp    loc_5_09AA
loc_5_09BB:
        test   word ptr [edi + 4], 0x1000
        je     loc_5_09DE
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x40]
loc_5_09CD:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_09DE
        inc    cx
        jmp    loc_5_09CD
loc_5_09DE:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_09E4:
        push   esi
        push   eax
        push   ecx
        push   edx
        test   word ptr [edi + 0x52], 4
        je     loc_5_0A13
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_0A13
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0A02:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0A13
        inc    cx
        jmp    loc_5_0A02
loc_5_0A13:
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x42]
        add    edx, 0
loc_5_0A20:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0A31
        inc    cx
        jmp    loc_5_0A20
loc_5_0A31:
        test   word ptr [edi + 4], 0x1000
        je     loc_5_0A54
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x40]
loc_5_0A43:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0A54
        inc    cx
        jmp    loc_5_0A43
loc_5_0A54:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0A5A:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 0x22]
        mov    ebx, 4
        mov    eax, 0x60
        call   sub_1_07FB
loc_5_0A70:
        call   sub_1_07F2
loc_5_0A75:
        pop    edx
        pop    ebx
        pop    eax
        ret    
sub_5_0A79:
        push   esi
        push   eax
        push   ecx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x22]
loc_5_0A86:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_0A97
        inc    cx
        jmp    loc_5_0A86
loc_5_0A97:
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0A9C:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0x22]
loc_5_0AAA:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0ABB
        inc    cx
        jmp    loc_5_0AAA
loc_5_0ABB:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0AC1:
        test   word ptr [edi + 4], 1
        jne    loc_5_0B16
        call   sub_1_0294
loc_5_0ACE:
        jb     loc_5_0B16
        cmp    eax, 2
        je     loc_5_0AEF
        test   dword ptr [esi], 1
        jne    loc_5_0B16
        or     dword ptr [esi], 1
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        jmp    loc_5_0B07
loc_5_0AEF:
        test   dword ptr [esi], 2
        jne    loc_5_0B16
        or     dword ptr [esi], 2
        movzx  ecx, word ptr [0xe]           ; -> loc_2_000E
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
loc_5_0B07:
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_5_0B16:
        ret    
loc_5_0B17:
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0B65
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_0B65
loc_5_0B2B:
        popfd  
        pushfd 
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 0x24], ebx
        jne    loc_5_0B41
        or     eax, 1
        mov    dword ptr [edi + 0x28], 0xffffffff
loc_5_0B41:
        cmp    dword ptr [edi + 0x30], ebx
        jne    loc_5_0B50
        or     eax, 2
        mov    dword ptr [edi + 0x34], 0xffffffff
loc_5_0B50:
        or     eax, eax
        je     loc_5_0B59
        call   sub_5_0BE0
loc_5_0B59:
        pop    eax
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_0B65
        jmp    loc_5_0B2B
loc_5_0B65:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_5_0B7A
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_0B7A:
        popfd  
        clc    
        ret    
loc_5_0B7D:
        pushal 
        pushfd 
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    eax, 5
        mov    ecx, 0x34
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_5_0BD8
        mov    dword ptr [edi + ebx], esi
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0BC3
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        sti    
        je     loc_5_0BC3
loc_5_0BAE:
        mov    edi, dword ptr [eax]
        call   sub_4_00CB
loc_5_0BB5:
        jb     loc_5_0BC5
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        sti    
        je     loc_5_0BC3
        jmp    loc_5_0BAE
loc_5_0BC3:
        jmp    loc_5_0BDC
loc_5_0BC5:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_5_0BD8
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_0BD8:
        popfd  
        stc    
        jmp    loc_5_0BDE
loc_5_0BDC:
        popfd  
        clc    
loc_5_0BDE:
        popal  
        ret    
sub_5_0BE0:
        push   eax
        push   edx
        push   esi
        call   sub_1_0294
loc_5_0BE8:
        jb     loc_5_0C71
        test   eax, 2
        je     loc_5_0C0E
        cmp    dword ptr [edi + 0x30], ebx
        jne    loc_5_0C0E
        call   sub_5_0A5A
        call   sub_5_0A79
        mov    dword ptr [edi + 0x30], 0
        and    dword ptr [esi], 0xfffffffd
loc_5_0C0E:
        test   eax, 1
        je     loc_5_0C71
        cmp    dword ptr [edi + 0x24], ebx
        jne    loc_5_0C71
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_0C40
        test   word ptr [edi + 4], 0x200
        jne    loc_5_0C40
        mov    eax, dword ptr [edi + 0x38]
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_5_0C40
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
loc_5_0C40:
        call   sub_1_090D
loc_5_0C45:
        call   sub_5_0979
        mov    dword ptr [edi + 0x24], 0
        mov    eax, dword ptr [edi + 0x2c]
        test   word ptr [edi + 4], 8
        jne    loc_5_0C68
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_5_0C68:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        and    dword ptr [esi], 0xfffffffe
loc_5_0C71:
        pop    esi
        pop    edx
        pop    eax
        clc    
        ret    
sub_5_0C76:
        push   eax
        push   ecx
        push   edx
        push   esi
        test   eax, 2
        je     loc_5_0C99
        cmp    dword ptr [edi + 0x30], 0
        je     loc_5_0C8E
        cmp    dword ptr [edi + 0x30], ebx
        je     loc_5_0C99
        jmp    loc_5_0CB3
loc_5_0C8E:
        mov    dword ptr [edi + 0x30], ebx
        call   sub_5_0A9C
        mov    dword ptr [edi + 0x34], ebx
loc_5_0C99:
        test   eax, 1
        je     loc_5_0D3E
        cmp    dword ptr [edi + 0x24], 0
        je     loc_5_0CBA
        cmp    dword ptr [edi + 0x24], ebx
        je     loc_5_0D3D
loc_5_0CB3:
        stc    
        jb     loc_5_0D3E
loc_5_0CBA:
        mov    dword ptr [edi + 0x24], ebx
        cmp    dword ptr [edi + 0x28], ebx
        je     loc_5_0CE8
        mov    ebx, dword ptr [edi + 0x28]
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_5_0CD9
        call   sub_1_0294
loc_5_0CD2:
        jb     loc_5_0CD9
        call   sub_1_072E
loc_5_0CD9:
        mov    ebx, dword ptr [edi + 0x24]
        call   sub_1_0294
loc_5_0CE1:
        jb     loc_5_0CE8
        call   sub_1_0654
loc_5_0CE8:
        mov    dword ptr [edi + 0x28], ebx
        call   sub_5_09E4
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_0D16
        test   word ptr [edi + 4], 0x200
        jne    loc_5_0D16
        mov    eax, dword ptr [edi + 0x38]
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_5_0D16
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
loc_5_0D16:
        test   word ptr [edi + 4], 8
        jne    loc_5_0D3D
        mov    eax, dword ptr [edi + 0x2c]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_5_0D37
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_5_0D3D
loc_5_0D37:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_5_0D3D:
        clc    
loc_5_0D3E:
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_5_0D43:
        mov    eax, 0x400
        clc    
        ret    
        db  0CCh, 0CCh
azt16_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 3
        jae    loc_5_0D76
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj6:0x00AC
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_5_0D76
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_5_0D76
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_0D76:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_0D7C:
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0DDD
        pushfd 
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        or     eax, eax
        je     loc_5_0DDD
        mov    edi, dword ptr [eax]
        cmp    ebx, dword ptr [edi + 0x24]
        jne    loc_5_0DDD
        xchg   esi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        cmp    esi, -1
        je     loc_5_0DDD
        movzx  ecx, word ptr [ebp + 0x10]
        movzx  eax, word ptr [ebp + 0x18]
        add    ecx, eax
        cmp    ecx, dword ptr [edi + 0x1c]
        ja     loc_5_0DDD
        movzx  ecx, word ptr [ebp + 0x18]
        mov    edi, dword ptr [edi + 0x18]
        movzx  eax, word ptr [ebp + 0x10]
        add    edi, eax
        or     ecx, ecx
        je     loc_5_0DDD
        cld    
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        adc    cl, cl
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        clc    
        ret    
loc_5_0DDD:
        mov    word ptr [ebp + 0x1c], 0
        stc    
        ret    
loc_5_0DE5:
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0E46
        pushfd 
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        or     eax, eax
        je     loc_5_0E46
        mov    esi, dword ptr [eax]
        cmp    ebx, dword ptr [esi + 0x24]
        jne    loc_5_0E46
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_0E46
        movzx  ecx, word ptr [ebp + 0x10]
        movzx  eax, word ptr [ebp + 0x18]
        add    ecx, eax
        cmp    ecx, dword ptr [esi + 0x1c]
        ja     loc_5_0E46
        movzx  ecx, word ptr [ebp + 0x18]
        mov    esi, dword ptr [esi + 0x18]
        movzx  eax, word ptr [ebp + 0x10]
        add    esi, eax
        or     ecx, ecx
        je     loc_5_0E46
        cld    
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        adc    cl, cl
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        clc    
        ret    
loc_5_0E46:
        mov    word ptr [ebp + 0x1c], 0
        stc    
        ret    
loc_5_0E4E:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_01EC
loc_5_0E5A:
        add    esp, 8
        jb     loc_5_0E8E
        movzx  eax, word ptr [ebp + 0x10]
        or     eax, eax
        je     loc_5_0E8E
        cmp    eax, 1
        jne    loc_5_0E71
        mov    eax, dword ptr [edi + 0x38]
        jmp    loc_5_0E7B
loc_5_0E71:
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        jne    loc_5_0E7B
        mov    eax, dword ptr [edi + 0x38]
loc_5_0E7B:
        or     eax, eax
        je     loc_5_0E8E
        push   ecx
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        mov    word ptr [ebp + 0x1c], cx
        pop    ecx
        clc    
        jmp    loc_5_0E95
loc_5_0E8E:
        mov    word ptr [ebp + 0x1c], 0xffff
        stc    
loc_5_0E95:
        pop    edi
        ret    
loc_5_0E97:
        call   sub_5_0D43
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_5_0EA2:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_0EF6
        test   eax, 1
        je     loc_5_0EDB
        push   4
        push   edx
        call   sub_1_01EC
loc_5_0EBD:
        add    esp, 8
        or     edi, edi
        jne    loc_5_0ED6
        add    edx, 4
        push   4
        push   edx
        call   sub_1_01EC
loc_5_0ECF:
        add    esp, 8
        or     edi, edi
        je     loc_5_0EF6
loc_5_0ED6:
        mov    ecx, dword ptr [edi + 0x28]
        jmp    loc_5_0EFE
loc_5_0EDB:
        test   eax, 2
        je     loc_5_0EF6
        push   5
        push   edx
        call   sub_1_01EC
loc_5_0EEA:
        add    esp, 8
        or     edi, edi
        je     loc_5_0EF6
        mov    ecx, dword ptr [edi + 0x34]
        jmp    loc_5_0EFE
loc_5_0EF6:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0EFC:
        stc    
        ret    
loc_5_0EFE:
        mov    word ptr [ebp + 0x1c], 0
        call   sub_5_0C76
        jb     loc_5_0F0C
        ret    
loc_5_0F0C:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_0EFC
loc_5_0F14:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_0F70
        test   eax, 1
        je     loc_5_0F55
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_5_0F41
        push   4
        push   edx
        call   sub_1_01EC
loc_5_0F37:
        add    esp, 8
        or     edi, edi
        jne    loc_5_0F50
        add    edx, 4
loc_5_0F41:
        push   4
        push   edx
        call   sub_1_01EC
loc_5_0F49:
        add    esp, 8
        or     edi, edi
        je     loc_5_0F70
loc_5_0F50:
        mov    ecx, dword ptr [edi + 0x24]
        jmp    loc_5_0F78
loc_5_0F55:
        test   eax, 2
        je     loc_5_0F70
        push   5
        push   edx
        call   sub_1_01EC
loc_5_0F64:
        add    esp, 8
        or     edi, edi
        je     loc_5_0F70
        mov    ecx, dword ptr [edi + 0x30]
        jmp    loc_5_0F78
loc_5_0F70:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0F76:
        stc    
        ret    
loc_5_0F78:
        cmp    ecx, ebx
        je     loc_5_0F84
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_0F76
loc_5_0F84:
        call   sub_5_0BE0
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_0F91:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 0xfffe
        jne    loc_5_1002
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 1
        je     loc_5_0FAB
        mov    ecx, dword ptr [ebp + 0x18]
loc_5_0FAB:
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_1002
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_1002
        mov    edi, dword ptr [eax]
        jecxz  loc_5_0FD3
loc_5_0FC2:
        cmp    dword ptr [edi + 0x54], ecx
        je     loc_5_0FD3
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_1002
        mov    edi, dword ptr [eax]
        jmp    loc_5_0FC2
loc_5_0FD3:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_1002
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_1002
        cmp    ecx, 0xbe
        ja     loc_5_1002
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_1002:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_100B:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1040
        push   5
        push   edx
        call   sub_1_01EC
loc_5_101B:
        add    esp, 8
        or     edi, edi
        je     loc_5_1040
        mov    ecx, dword ptr [edi + 0x34]
        mov    word ptr [ebp + 0x1c], 0
        mov    eax, 2
        call   sub_5_0C76
        jb     loc_5_1038
        ret    
loc_5_1038:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_1046
loc_5_1040:
        mov    word ptr [ebp + 0x1c], 1
loc_5_1046:
        stc    
        ret    
loc_5_1048:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1080
        push   5
        push   edx
        call   sub_1_01EC
loc_5_1058:
        add    esp, 8
        or     edi, edi
        je     loc_5_1080
        mov    ecx, dword ptr [edi + 0x30]
        cmp    ecx, ebx
        je     loc_5_106E
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_1086
loc_5_106E:
        mov    eax, 2
        call   sub_5_0BE0
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_1080:
        mov    word ptr [ebp + 0x1c], 1
loc_5_1086:
        stc    
        ret    
loc_5_1088:
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
        mov    edi, dword ptr [eax]
loc_5_10AB:
        cmp    dword ptr [edi + 0x54], ecx
        je     loc_5_10BC
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_1101
        mov    edi, dword ptr [eax]
        jmp    loc_5_10AB
loc_5_10BC:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_1101
        test   word ptr [esi + 0x52], 8
        je     loc_5_1101
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_1101
        cmp    ecx, 0x1c
        ja     loc_5_1101
        mov    word ptr [edi + 4], 1
        mov    ax, word ptr [esi + 0x22]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 0x54]
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
        call   sub_1_01EC
loc_5_1162:
        add    esp, 8
        or     edi, edi
        je     loc_5_118E
        lea    eax, [edi + 0x44]
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
        call   sub_1_01EC
loc_5_11B2:
        add    esp, 8
        or     edi, edi
        je     loc_5_11C8
        lea    eax, [edi + 0x44]
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
        call   sub_5_1D78
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
        call   sub_5_1D68
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
        call   sub_5_1D58
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
        call   sub_5_1D44
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
        call   sub_5_1D34
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
        call   sub_5_1DAC
        push   edi
        push   dword ptr [esi]
        call   sub_5_1D9C
        push   dword ptr [esi]
        call   sub_5_1D68
        test   eax, eax
        jne    loc_5_14C8
        push   dword ptr [esi]
        call   sub_5_1D8C
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
        db  0CCh, 0CCh
sub_5_1530:
        movzx  eax, word ptr [0x10]          ; -> loc_2_0010
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_5_153E:
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
sub_5_1559:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
        pop    ebp
        ret    
sub_5_1567:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Phys_Unmask_Channel ; cd2019000400
        pop    ebx
        pop    ebp
        ret    
sub_5_157D:
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
sub_5_159E:
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
sub_5_15C2:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        mov    al, byte ptr [ebp + 0x10]
        call   sub_1_04B7
loc_5_15D4:
        xor    eax, eax
        jae    loc_5_15DD
        mov    eax, 1
loc_5_15DD:
        pop    edx
        pop    ebp
        ret    
sub_5_15E0:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        call   sub_1_0499
loc_5_15EF:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret    
sub_5_15F4:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_01EC
loc_5_1604:
        add    esp, 8
        mov    word ptr [edi + 0xbc], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret    
sub_5_1614:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0xffff
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_01EC
loc_5_162A:
        add    esp, 8
        mov    eax, dword ptr [edi + 0x24]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        je     loc_5_163C
        jmp    loc_5_164A
loc_5_163C:
        mov    word ptr [edi + 0xbc], 1
        mov    edx, 0
loc_5_164A:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret    
sub_5_1651:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   eax
        call   sub_1_01EC
loc_5_1664:
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
loc_5_16A1:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0040
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_16D6
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_16E4
loc_5_16D6:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_16E4:
        pop    edi
        pop    esi
        ret    0xc
loc_5_16E9:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        inc    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_16F7:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        dec    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_1705:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [esi], 0xb
        call   sub_5_1530
        push   0xff
        lea    ecx, [esi + 4]
        push   eax
        lea    edi, [esi + 0x104]
        push   ecx
        call   sub_5_1DC0
        add    esp, 0xc
        push   0xff
        push   0                             ; -> obj6:0x028C
        push   edi
        call   sub_5_1DC0
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
        je     loc_5_1779
loc_5_176E:
        movsx  ecx, byte ptr [edi]
        add    eax, ecx
        inc    edi
        cmp    byte ptr [edi], 0
        jne    loc_5_176E
loc_5_1779:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 0x20a], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_1790:
        push   esi
        push   edi
        mov    edi, 0x8878000a
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_5_17B5
        push   dword ptr [esi + 4]
        call   sub_5_1614
        add    esp, 4
        test   eax, eax
        jne    loc_5_17B5
        xor    edi, edi
        inc    dword ptr [esi + 0xc]
loc_5_17B5:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_17BC:
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
loc_5_1807:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 0x8007000e
        mov    esi, dword ptr [esp + 0x2c]
        test   byte ptr [esp + 0x1c], 0xe0
        mov    dword ptr [esi], ebx
        jne    loc_5_18AE
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 0x10], ebx
        jne    loc_5_18A7
        push   0x201
        push   0x33
        call   sub_5_1DD0
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_18A0
        mov    dword ptr [ebx], 0            ; -> obj6:0x02A8
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_1651
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1000
        jbe    loc_5_18B7
        cmp    eax, 0xffff
        jae    loc_5_18B7
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_5_18B7
        cmp    dword ptr [ebx + 0x10], 0
        je     loc_5_18B7
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_5_18B7
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        push   ebx
        xor    edi, edi
        call   sub_5_1A57
        mov    eax, dword ptr [esp + 0x24]
        add    esp, 0xc
        movzx  ecx, word ptr [eax + 0xe]
        mov    dword ptr [ebx + 0x24], ecx
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [ebx + 0x28], edx
        jmp    loc_5_18B7
loc_5_18A0:
        mov    edi, 0x8007000e
        jmp    loc_5_18B7
loc_5_18A7:
        mov    edi, 0x8878000a
        jmp    loc_5_18B7
loc_5_18AE:
        mov    edi, 0x8878001e
        mov    ebp, dword ptr [esp + 0x14]
loc_5_18B7:
        test   edi, edi
        je     loc_5_18CC
        test   ebx, ebx
        je     loc_5_18D1
        push   0
        push   ebx
        call   sub_5_1DC8
        add    esp, 8
        jmp    loc_5_18D1
loc_5_18CC:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 0x10]
loc_5_18D1:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
loc_5_18DA:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        mov    eax, 0x80004001
        ret    0xc
loc_5_18EC:
        mov    eax, 0x80004005
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0xc]
        dec    edx
        mov    dword ptr [ecx + 0xc], edx
        jne    loc_5_1911
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_5_1911
        push   dword ptr [ecx + 4]
        call   sub_5_15F4
        add    esp, 4
        xor    eax, eax
loc_5_1911:
        ret    4
sub_5_1914:
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
        call   sub_5_1D14
        add    esp, 0x14
        test   eax, eax
        je     loc_5_1942
        mov    dword ptr [ebp - 8], esi
loc_5_1942:
        cmp    dword ptr [ebp - 8], 8
        je     loc_5_19E8
        push   0x201
        push   0x14
        call   sub_5_1DD0
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     loc_5_19BF
        push   1
        lea    eax, [ebp - 4]
        push   0                             ; -> obj6:0x0298
        lea    ecx, [ebp - 0x1c]
        push   eax
        mov    dword ptr [edi + 4], ebx
        push   ecx
        mov    dword ptr [edi], 0            ; -> obj6:0x0268
        call   sub_5_1DEC
        test   eax, eax
        jne    loc_5_19BF
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
        jne    loc_5_19BF
        push   edi
        mov    esi, 1
        push   ebx
        call   sub_1_0273
loc_5_19BC:
        add    esp, 8
loc_5_19BF:
        test   esi, esi
        jne    loc_5_19E8
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_19D9
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1DD8
loc_5_19D9:
        test   edi, edi
        je     loc_5_19E8
        push   0
        push   edi
        call   sub_5_1DC8
        add    esp, 8
loc_5_19E8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_19EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_0253
loc_5_1A00:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_5_1A51
        push   0
        push   edi
        call   sub_1_0273
loc_5_1A11:
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
        call   sub_5_1DD8
        push   0
        push   esi
        call   sub_5_1DC8
        add    esp, 8
loc_5_1A51:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_1A57:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [edx], eax
        jbe    loc_5_1A69
        mov    dword ptr [edx], eax
loc_5_1A69:
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
loc_5_1A89:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x20]
        dec    esi
        mov    dword ptr [eax + 0x20], esi
        jne    loc_5_1AAE
        cmp    dword ptr [eax + 8], 0
        jne    loc_5_1AAE
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 0x10]
        call   sub_5_1DC8
        add    esp, 8
loc_5_1AAE:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_1AB4:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0050
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_1AE9
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_1AF7
loc_5_1AE9:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1AF7:
        pop    edi
        pop    esi
        ret    0xc
loc_5_1AFC:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x20]
        inc    eax
        mov    dword ptr [ecx + 0x20], eax
        ret    4
loc_5_1B0A:
        mov    eax, 0x80004001
        ret    0x20
loc_5_1B12:
        mov    eax, 0x80004001
        ret    0x14
loc_5_1B1A:
        mov    eax, 0x80004001
        ret    8
loc_5_1B22:
        mov    eax, 0x80004001
        ret    8
loc_5_1B2A:
        mov    eax, 0x80004001
        ret    8
loc_5_1B32:
        mov    eax, 0x80004001
        ret    8
loc_5_1B3A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    loc_5_1B6C
        push   dword ptr [esi + 0x18]
        call   sub_5_153E
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 0x32], 3
        jbe    loc_5_1B59
        add    eax, eax
loc_5_1B59:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_5_1B80
loc_5_1B6C:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_1B80:
        xor    eax, eax
        pop    esi
        ret    0xc
loc_5_1B86:
        push   ebx
        mov    eax, 0x80004005
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 8], 0
        jne    loc_5_1C64
        movzx  edi, word ptr [esi + 0x30]
        push   0
        push   0xa
        push   edi
        call   sub_5_15C2
        add    esp, 0xc
        lea    edx, [edi + 2]
        xor    al, al
        push   dword ptr [esi + 0x18]
        out    dx, al
        call   sub_5_1559
        add    esp, 4
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 0x18]
        call   sub_5_157D
        add    esp, 0xc
        push   dword ptr [esi + 0x18]
        call   sub_5_159E
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_1567
        mov    bx, word ptr [esi + 0x1c]
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0x10
        jne    loc_5_1BF2
        shr    bx, 1
loc_5_1BF2:
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_5_1BFB
        shr    bx, 1
loc_5_1BFB:
        dec    bx
        push   ebx
        push   0xf
        push   edi
        call   sub_5_15C2
        add    esp, 0xc
        mov    al, bh
        push   eax
        push   0xe
        push   edi
        call   sub_5_15C2
        add    esp, 0xc
        cmp    word ptr [esi + 0x2c], 2
        jne    loc_5_1C38
        push   ebx
        push   0x1f
        push   edi
        mov    bl, bh
        call   sub_5_15C2
        add    esp, 0xc
        push   ebx
        push   0x1e
        push   edi
        call   sub_5_15C2
        add    esp, 0xc
loc_5_1C38:
        test   byte ptr [esi + 0x2e], 0x40
        mov    al, 1
        jne    loc_5_1C42
        mov    al, 5
loc_5_1C42:
        push   eax
        push   9
        push   edi
        call   sub_5_15C2
        add    esp, 0xc
        push   2
        push   0xa
        push   edi
        call   sub_5_15C2
        add    esp, 0xc
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_1C64:
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
loc_5_1C6A:
        push   esi
        mov    eax, 0x80004005
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        cmp    dword ptr [edi + 8], 0
        je     loc_5_1CBF
        movzx  esi, word ptr [edi + 0x30]
        push   0xa
        push   esi
        call   sub_5_15E0
        add    esp, 8
        and    al, 0xc0
        push   eax
        push   0xa
        push   esi
        call   sub_5_15C2
        add    esp, 0xc
        lea    edx, [esi + 2]
        xor    al, al
        push   0
        out    dx, al
        push   9
        push   esi
        call   sub_5_15C2
        add    esp, 0xc
        push   dword ptr [edi + 0x18]
        call   sub_5_1559
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [edi + 8], 0
loc_5_1CBF:
        pop    edi
        pop    esi
        ret    4
sub_5_1CC4:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_5_1CCC:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_5_1CD4:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        int3   
        int3   
sub_5_1CDC:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        int3   
        int3   
sub_5_1CE4:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        int3   
        int3   
sub_5_1CEC:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_5_1CF4:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_5_1CFC:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_5_1D04:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_5_1D0C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_5_1D14:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_5_1D1C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Log_Conf ; cd201b803300
        int3   
        int3   
sub_5_1D24:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_5_1D2C:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_5_1D34:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1D44:
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
sub_5_1D58:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1D68:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1D78:
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
sub_5_1D8C:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1D9C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1DAC:
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
sub_5_1DC0:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_5_1DC8:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_5_1DD0:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_5_1DD8:
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
sub_5_1DEC:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_5_1E0A
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_1E0A:
        ret    0x10
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x2d8  RW-
        db  041h, 05Ah, 054h, 031h, 036h, 03Ah, 020h, 000h, 049h, 052h, 051h, 020h, 06Eh, 06Fh, 074h, 020h
        db  072h, 065h, 073h, 070h, 06Fh, 06Eh, 064h, 069h, 06Eh, 067h, 020h, 028h, 025h, 078h, 03Ah, 025h
        db  078h, 029h, 00Ah, 000h, 044h, 04Dh, 041h, 020h, 06Eh, 06Fh, 074h, 020h, 072h, 065h, 073h, 070h
        db  06Fh, 06Eh, 064h, 069h, 06Eh, 067h, 020h, 028h, 025h, 078h, 03Ah, 025h, 078h, 029h, 00Ah, 000h
        db  043h, 06Fh, 06Eh, 066h, 069h, 067h, 000h, 000h, 053h, 069h, 06Eh, 067h, 06Ch, 065h, 020h, 04Dh
        db  06Fh, 064h, 065h, 020h, 044h, 04Dh, 041h, 000h, 05Ch, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 000h
        db  042h, 04Ch, 041h, 053h, 054h, 045h, 052h, 000h, 041h, 025h, 030h, 033h, 058h, 020h, 049h, 025h
        db  064h, 020h, 044h, 025h, 064h, 000h, 000h, 000h
        dd  loc_5_0E97               ; fixup
        dd  loc_5_0F91               ; fixup
        dd  loc_5_0EA2               ; fixup
        dd  loc_5_0F14               ; fixup
        dd  loc_5_0D7C               ; fixup
        dd  loc_5_0DE5               ; fixup
        dd  loc_5_0E4E               ; fixup
        dd  loc_5_0E97               ; fixup
        dd  loc_5_1088               ; fixup
        dd  loc_5_100B               ; fixup
        dd  loc_5_1048               ; fixup
        dd  loc_5_110A               ; fixup
        dd  loc_5_119C               ; fixup
        db  007h, 000h, 000h, 000h
        dd  obj6:0x0078              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj6:0x0094              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj6:0x00A4              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  loc_5_079C               ; fixup
        db  001h, 000h
        dd  loc_5_079C               ; fixup
        db  002h, 000h
        dd  loc_5_079C               ; fixup
        db  003h, 000h
        dd  loc_5_079C               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_5_07B8               ; fixup
        db  001h, 000h
        dd  loc_5_07B8               ; fixup
        db  002h, 000h
        dd  loc_5_07B8               ; fixup
        db  003h, 000h
        dd  loc_5_07B8               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_5_0908               ; fixup
        db  001h, 000h
        dd  loc_5_0908               ; fixup
        db  002h, 000h
        dd  loc_5_0908               ; fixup
        db  003h, 000h
        dd  loc_5_0908               ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_5_07FC               ; fixup
        db  001h, 000h
        dd  loc_5_07FC               ; fixup
        db  002h, 000h
        dd  loc_5_07FC               ; fixup
        db  003h, 000h
        dd  loc_5_07FC               ; fixup
        db  004h, 000h
        dd  loc_5_07FC               ; fixup
        db  005h, 000h
        dd  loc_5_07FC               ; fixup
        db  006h, 000h
        dd  loc_5_07FC               ; fixup
        db  007h, 000h
        dd  loc_5_07FC               ; fixup
        db  008h, 000h
        dd  loc_5_07FC               ; fixup
        db  009h, 000h
        dd  loc_5_07FC               ; fixup
        db  00Ah, 000h
        dd  loc_5_07FC               ; fixup
        db  00Bh, 000h
        dd  loc_5_07FC               ; fixup
        db  00Ch, 000h
        dd  loc_5_07FC               ; fixup
        db  00Dh, 000h
        dd  loc_5_07FC               ; fixup
        db  00Eh, 000h
        dd  loc_5_07FC               ; fixup
        db  00Fh, 000h
        dd  loc_5_07FC               ; fixup
        db  000h, 001h, 003h, 0FFh, 001h, 002h, 003h, 007h, 009h, 00Ah, 00Bh, 0FFh, 048h, 050h, 058h, 060h
        db  0FFh, 000h, 000h, 004h, 000h
        dd  loc_1_0374               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0424               ; fixup
        dd  loc_1_0444               ; fixup
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
        db  083h, 081h, 082h, 000h, 08Bh, 089h, 08Ah
        dd  loc_5_16A1               ; fixup
        dd  loc_5_16E9               ; fixup
        dd  loc_5_16F7               ; fixup
        dd  loc_5_1705               ; fixup
        dd  loc_5_1790               ; fixup
        dd  loc_5_18EC               ; fixup
        dd  loc_5_17BC               ; fixup
        dd  loc_5_1807               ; fixup
        dd  loc_5_18DA               ; fixup
        db  041h, 05Ah, 054h, 031h, 036h, 02Eh, 056h, 058h, 044h, 000h, 000h, 000h, 064h, 073h, 06Fh, 075h
        db  06Eh, 064h, 02Eh, 076h, 078h, 064h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_5_1AB4               ; fixup
        dd  loc_5_1AFC               ; fixup
        dd  loc_5_1A89               ; fixup
        dd  loc_5_1B0A               ; fixup
        dd  loc_5_1B12               ; fixup
        dd  loc_5_1B1A               ; fixup
        dd  loc_5_1B22               ; fixup
        dd  loc_5_1B2A               ; fixup
        dd  loc_5_1B32               ; fixup
        dd  loc_5_1B3A               ; fixup
        dd  loc_5_1B86               ; fixup
        dd  loc_5_1C6A               ; fixup
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
        mov    ecx, 0x34
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_7_0070
        mov    dword ptr [edi + ebx], esi
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_7_003D
        pop    ebx
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_02D0
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        clc    
        ret    
loc_7_0070:
        pop    ebx
        jmp    loc_7_0011
VxD__LTEXT ENDS

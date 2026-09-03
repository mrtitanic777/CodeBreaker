; VIP -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 19031 instructions recovered by recursive descent

Declare_Virtual_Device VIP, 3, 0, VIP_Control, 00489h, Undefined_Init_Order
;   DDK version 0400h   init_order 054008000h   flags 00000h
;   service table: obj1:0xbadc, 9 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xc304  R-X
VIP_Service_00:
        mov    ax, 0x100
        clc    
        ret    
        db  0CDh, 020h, 000h, 000h, 09Ah, 004h, 072h, 006h, 0CDh, 020h, 003h, 080h, 09Ah, 004h, 0C3h
VIP_Service_06:
        jmp    loc_1_101C
VIP_Service_07:
        jmp    loc_1_1090
        db  056h, 02Bh, 0F6h, 087h, 035h
        dd  obj1:0xBBF3              ; fixup
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h, 05Eh, 0CDh, 020h, 009h, 080h, 088h, 004h
loc_1_0035:
        mov    dword ptr [0xbbf3], 0         ; -> obj1:0xBBF3
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
sub_1_0045:
        push   esi
        push   ebx
        sub    ebx, ebx
        cmp    dword ptr [0xbbf3], ebx       ; -> obj1:0xBBF3
        jne    loc_1_0069
        mov    esi, 0x35                     ; -> loc_1_0035
        mov    ecx, 0x800
        sub    eax, eax
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [0xbbf3], esi       ; -> obj1:0xBBF3
loc_1_0069:
        pop    ebx
        pop    esi
        ret    
VIP_Service_01:
        jmp    loc_1_0AA8
VIP_Service_02:
        jmp    sub_1_0348
VIP_Service_03:
        sub    eax, eax
VIP_Service_04:
        jmp    sub_1_0BF4
VIP_Service_08:
        jmp    loc_1_02D8
sub_1_0082:
        jecxz  loc_1_0097
        test   byte ptr [ebx], 0x20
        jne    loc_1_0098
        movzx  edx, cx
        shr    ecx, 0x10
        shl    ecx, 4
        add    ecx, edx
        add    ecx, dword ptr [ebx + 4]
loc_1_0097:
        ret    
loc_1_0098:
        pushal 
        shr    ecx, 0x10
        push   0
        push   ecx
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        mov    edx, dword ptr [esp + 0x18]
        movzx  edx, dx
        add    ecx, edx
        mov    dword ptr [esp + 0x18], ecx
        popal  
        ret    
loc_1_00B8:
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        cld    
        mov    eax, dword ptr [edx + 4]
        mov    edi, dword ptr [eax + 0x1c]
        mov    ebx, dword ptr [eax + 0x20]
        movzx  ecx, word ptr [ebx]
        cmp    ecx, dword ptr [edx + 0x10]
        jbe    loc_1_00D2
        mov    ecx, dword ptr [edx + 0x10]
loc_1_00D2:
        mov    word ptr [ebx], cx
        mov    ebx, dword ptr [edx + 0xc]
        push   ecx
loc_1_00D9:
        mov    ecx, dword ptr [ebx + 0xc]
        cmp    ecx, dword ptr [esp]
        jbe    loc_1_00E4
        mov    ecx, dword ptr [esp]
loc_1_00E4:
        sub    dword ptr [esp], ecx
        mov    esi, dword ptr [ebx + 8]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [esp], 0
        jne    loc_1_00D9
        pop    ecx
        mov    ecx, dword ptr [edx + 8]
        test   ecx, ecx
        jne    loc_1_012B
        mov    ebx, dword ptr [edx + 0x14]
        mov    edi, dword ptr [eax + 0x30]
        mov    cl, byte ptr [ebx + 9]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [ebx + 0xa]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [ebx + 0xb]
        mov    byte ptr [edi + 2], cl
        mov    esi, dword ptr [ebx]
        movzx  ecx, byte ptr [ebx + 8]
        cmp    cl, byte ptr [edi + 3]
        jbe    loc_1_0121
        mov    cl, byte ptr [edi + 3]
loc_1_0121:
        mov    byte ptr [edi + 3], cl
        mov    edi, dword ptr [eax + 0x34]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    ecx, ecx
loc_1_012B:
        mov    dword ptr [eax + 0x3c], ecx
        mov    ebx, dword ptr [eax + 0x38]
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        pop    esi
        pop    edi
        pop    ebx
        ret    
VIP_V86_API:
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    word ptr [eax], 0
        jne    loc_1_021E
        sub    esp, 0x40
        mov    edi, esp
        mov    dword ptr [edi + 0x38], ebx
        mov    ecx, dword ptr [eax + 0x10]
        call   sub_1_0082
        mov    dword ptr [edi + 0x1c], ecx
        mov    ecx, dword ptr [eax + 0x14]
        call   sub_1_0082
        mov    dword ptr [edi + 0x20], ecx
        mov    ecx, dword ptr [eax + 0x18]
        call   sub_1_0082
        mov    dl, byte ptr [ecx + 3]
        mov    byte ptr [edi + 0x2c], dl
        mov    dl, byte ptr [ecx]
        mov    byte ptr [edi + 0x2d], dl
        mov    dl, byte ptr [ecx + 1]
        mov    byte ptr [edi + 0x2e], dl
        mov    dl, byte ptr [ecx + 2]
        mov    byte ptr [edi + 0x2f], dl
        mov    ecx, dword ptr [ecx + 4]
        call   sub_1_0082
        mov    dword ptr [edi + 0x24], ecx
        mov    ecx, dword ptr [eax + 0x1c]
        call   sub_1_0082
        mov    dword ptr [edi + 0x30], ecx
        mov    ecx, dword ptr [ecx + 4]
        call   sub_1_0082
        mov    dword ptr [edi + 0x34], ecx
        mov    ecx, dword ptr [eax + 0xa]
        call   sub_1_0082
        lea    edx, [edi + 0x24]
        push   edx
        push   dword ptr [eax + 2]
        push   0xb8                          ; -> loc_1_00B8
        movzx  edx, word ptr [eax + 0xe]
        push   edx
        push   ecx
        push   dword ptr [eax + 6]
        push   edi
        mov    dword ptr [edi + 0x3c], 0x2bf7
        call   sub_1_9B58
        add    esp, 0x1c
        cmp    eax, 0x2bf7
        jne    loc_1_0214
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_1_01E7:
        mov    eax, dword ptr [edi + 0x3c]
        cmp    eax, 0x2bf7
        jne    loc_1_01FF
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        jmp    loc_1_01E7
loc_1_01FF:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
loc_1_0205:
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        add    esp, 0x40
        ret    
loc_1_0214:
        mov    ecx, dword ptr [edi + 0x20]
        mov    word ptr [ecx], 0
        jmp    loc_1_0205
loc_1_021E:
        mov    word ptr [ebp + 0x1c], 0x2b03
        mov    word ptr [ebp + 0x14], 0
        ret    
VIP_Service_05:
        jmp    loc_1_9FF0
        db  0C3h
loc_1_0231:
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        push   0x2e74                        ; -> loc_1_2E74
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        ret    
sub_1_0247:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        test   edx, edx
        je     loc_1_0269
        movzx  edx, word ptr [0xe]           ; -> obj3:0x000E
        add    edx, dword ptr [4]            ; -> loc_3_0004
        jmp    loc_1_0276
loc_1_0269:
        movzx  edx, word ptr [0x10]          ; -> loc_3_0010
        add    edx, dword ptr [4]            ; -> loc_3_0004
loc_1_0276:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   edx
        push   ecx
        push   eax
        push   0x140001
        push   0                             ; -> loc_3_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        mov    ecx, eax
        mov    eax, 0x10001030
        sub    edi, edi
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, dword ptr [esp]
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_1_02A6:
        VMMCall  VMM.Enable_VM_Ints              ; cd201a800100
loc_1_02AC:
        mov    dword ptr [0xbbf7], 1         ; -> obj1:0xBBF7
        clc    
        ret    
loc_1_02B8:
        stc    
        ret    
VIP_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0x1b
        je     loc_2_0000
        cmp    eax, 0x1c
        je     loc_1_02B8
        cmp    eax, 4
        je     loc_1_02AC
        clc    
        ret    
loc_1_02D8:
        mov    edx, dword ptr [esp + 4]
        cmp    edx, 0xffff
        ja     loc_1_0318
        mov    eax, dword ptr [0xc090]       ; -> obj1:0xC090
        or     eax, eax
        je     loc_1_0311
        inc    ecx
        mov    ecx, dword ptr [0xc050]       ; -> obj1:0xC050
loc_1_02F4:
        cmp    ecx, eax
        je     loc_1_02FE
        cmp    word ptr [eax + 0x16], dx
        je     loc_1_030C
loc_1_02FE:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_02F4
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret    
        db  090h
loc_1_030C:
        mov    dword ptr [0xbc04], eax       ; -> obj1:0xBC04
loc_1_0311:
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret    
loc_1_0318:
        mov    eax, 1
        mov    dword ptr [0xbc04], 0         ; -> obj1:0xBC04
        ret    
sub_1_0328:
        push   esi
        mov    esi, dword ptr [0xc090]       ; -> obj1:0xC090
        or     esi, esi
        je     loc_1_0346
        nop    
loc_1_0334:
        mov    eax, dword ptr [esi + 0xc]
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 0x10]
        add    esp, 4
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_0334
loc_1_0346:
        pop    esi
        ret    
sub_1_0348:
        sub    esp, 4
        push   esi
        push   edi
        sub    eax, eax
        sub    esi, esi
        mov    edx, dword ptr [0xc068]       ; -> loc_1_C068
        cmp    edx, eax
        jle    loc_1_038C
        mov    edi, 0xc00c                   ; -> obj1:0xC00C
        mov    cl, byte ptr [esp + 0x10]
loc_1_0364:
        cmp    byte ptr [edi], cl
        je     loc_1_0374
        add    edi, 0x14
        inc    esi
        cmp    edx, esi
        jg     loc_1_0364
        jmp    loc_1_0390
        db  08Bh, 0FFh
loc_1_0374:
        lea    eax, [esi*4]
        lea    eax, [eax + eax*4 + 0xbffc]   ; -> loc_1_BFFC
        mov    dword ptr [esp + 8], 0
        jmp    loc_1_0390
loc_1_038C:
        mov    cl, byte ptr [esp + 0x10]
loc_1_0390:
        or     eax, eax
        jne    loc_1_03AF
        cmp    edx, 4
        jge    loc_1_03D7
        lea    eax, [edx*4]
        lea    eax, [eax + eax*4 + 0xbffc]   ; -> loc_1_BFFC
        mov    dword ptr [esp + 8], 1
loc_1_03AF:
        mov    byte ptr [eax + 0x10], cl
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0xc], ecx
        add    edx, dword ptr [esp + 8]
        mov    dword ptr [0xc068], edx       ; -> loc_1_C068
loc_1_03D7:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_03E0:
        mov    edx, dword ptr [esp + 8]
        cmp    dl, 0x7f
        jne    loc_1_03F0
        mov    eax, 0x2b03
        ret    
        db  090h
loc_1_03F0:
        mov    ecx, dword ptr [0xc090]       ; -> obj1:0xC090
        or     ecx, ecx
        je     loc_1_041D
        inc    eax
        mov    eax, dword ptr [0xc050]       ; -> obj1:0xC050
loc_1_0400:
        cmp    ecx, eax
        je     loc_1_0413
        test   byte ptr [ecx + 0x14], 1
        je     loc_1_0413
        or     edx, edx
        je     loc_1_0419
        cmp    dword ptr [ecx + 4], edx
        je     loc_1_0419
loc_1_0413:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_0400
loc_1_0419:
        or     ecx, ecx
        jne    loc_1_0424
loc_1_041D:
        mov    eax, 0x2b03
        ret    
        db  090h
loc_1_0424:
        cmp    dword ptr [esp + 0xc], 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   dword ptr [esp + 8]
        push   ecx
        call   sub_1_A7D8
        add    esp, 0xc
        ret    
loc_1_043C:
        push   dword ptr [esp + 4]
        call   sub_1_7044
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_044C:
        mov    eax, dword ptr [0xc090]       ; -> obj1:0xC090
        or     eax, eax
        mov    ecx, dword ptr [esp + 4]
        je     loc_1_047C
loc_1_0459:
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_0464
        test   byte ptr [eax + 0x14], 1
        jne    loc_1_046C
loc_1_0464:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_0459
        jmp    loc_1_047C
loc_1_046C:
        mov    ax, word ptr [eax + 0x3c]
        mov    ecx, dword ptr [esp + 8]
        mov    word ptr [ecx], ax
        mov    al, 1
        ret    
        db  08Bh, 0FFh
loc_1_047C:
        cmp    cl, 0x7f
        jne    loc_1_0494
        mov    eax, dword ptr [0xc050]       ; -> obj1:0xC050
        mov    ax, word ptr [eax + 0x3c]
        mov    ecx, dword ptr [esp + 8]
        mov    word ptr [ecx], ax
        mov    al, 1
        ret    
loc_1_0494:
        sub    al, al
        ret    
        db  090h
sub_1_0498:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x24]
        mov    al, byte ptr [esi + 8]
        mov    byte ptr [esp + 0x13], al
        movzx  ebx, al
        push   ebx
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        jne    loc_1_04CC
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_04CC:
        mov    esi, dword ptr [esi]
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [esp + 0x1c], 0x28
        mov    byte ptr [esp + 0x1d], 0x28
        mov    byte ptr [esp + 0x1e], 0x28
        mov    esi, dword ptr [esp + 0x28]
        and    byte ptr [esi + 0xb], 0x7f
        mov    byte ptr [esp + 0x12], 0
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_069E
loc_1_0508:
        mov    edx, dword ptr [esp + 0x18]
        movzx  ecx, byte ptr [esp + 0x12]
        add    edx, ecx
        mov    bl, byte ptr [edx]
        or     bl, bl
        je     loc_1_069E
        cmp    bl, 1
        jne    loc_1_052C
        inc    byte ptr [esp + 0x12]
        jmp    loc_1_0690
        db  090h
loc_1_052C:
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [esp + 0x11], al
        movzx  eax, bl
        cmp    eax, 7
        je     loc_1_055C
        cmp    eax, 0x44
        je     loc_1_0590
        cmp    eax, 0x83
        je     loc_1_0618
        cmp    eax, 0x89
        je     loc_1_0610
        jmp    loc_1_0688
        db  090h
loc_1_055C:
        mov    al, byte ptr [esp + 0x12]
        mov    byte ptr [esp + 0x1d], al
        cmp    dword ptr [esp + 0x30], 0
        jne    loc_1_0688
        lea    edi, [edx + 3]
        movzx  esi, byte ptr [esp + 0x11]
        sub    esi, 3
        sub    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, esi
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        jmp    loc_1_0684
        db  090h
loc_1_0590:
        mov    al, byte ptr [esp + 0x12]
        mov    byte ptr [esp + 0x1e], al
        cmp    dword ptr [esp + 0x30], 0
        jne    loc_1_0688
        mov    byte ptr [edx + 2], 5
        mov    al, byte ptr [edx + 3]
        and    al, 0xf
        mov    byte ptr [edx + 3], al
        and    eax, 0xff
        or     eax, eax
        jl     loc_1_0688
        cmp    eax, 1
        jle    loc_1_05CC
        cmp    eax, 3
        je     loc_1_05EC
        jmp    loc_1_0688
        db  090h
loc_1_05CC:
        sub    eax, eax
        movzx  esi, byte ptr [esp + 0x11]
        sub    esi, 4
        lea    edi, [edx + 4]
        mov    ecx, esi
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, esi
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        jmp    loc_1_0688
loc_1_05EC:
        lea    edi, [edx + 8]
        cmp    byte ptr [esp + 0x11], 0xc
        jb     loc_1_0688
        mov    cl, byte ptr [esp + 0x11]
        sub    cl, 8
        shr    cl, 2
        sub    eax, eax
        and    ecx, 0xff
        rep stosd dword ptr es:[edi], eax
        jmp    loc_1_0688
loc_1_0610:
        mov    esi, dword ptr [esp + 0x28]
        or     byte ptr [esi + 0xb], 0x80
loc_1_0618:
        cmp    byte ptr [esp + 0x11], 3
        je     loc_1_0684
        mov    al, byte ptr [edx + 2]
        sub    al, 5
        and    eax, 0xff
        add    eax, dword ptr [esp + 0x18]
        lea    esi, [eax + ecx]
        mov    dword ptr [esp + 0x14], esi
        mov    eax, esi
        lea    edi, [edx + 3]
        mov    ebx, edi
        mov    ebp, dword ptr [esi]
        mov    esi, dword ptr [esp + 0x28]
        mov    dword ptr [esi + 4], ebp
        cmp    edi, dword ptr [esp + 0x14]
        jae    loc_1_065C
loc_1_064A:
        mov    esi, dword ptr [eax]
        mov    ebp, dword ptr [ebx]
        mov    dword ptr [eax], ebp
        sub    eax, 4
        mov    dword ptr [ebx], esi
        add    ebx, 4
        cmp    ebx, eax
        jb     loc_1_064A
loc_1_065C:
        mov    eax, dword ptr [esp + 0x18]
        movzx  ebx, byte ptr [esp + 0x11]
        sub    ebx, 7
        lea    esi, [ecx + eax + 7]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x2c]
        mov    esi, dword ptr [esp + 0x14]
        mov    dword ptr [esi], eax
loc_1_0684:
        mov    byte ptr [edx + 2], 4
loc_1_0688:
        mov    al, byte ptr [esp + 0x11]
        add    byte ptr [esp + 0x12], al
loc_1_0690:
        mov    al, byte ptr [esp + 0x12]
        cmp    byte ptr [esp + 0x13], al
        ja     loc_1_0508
loc_1_069E:
        cmp    dword ptr [esp + 0x30], 0
        je     loc_1_06BA
        push   dword ptr [esp + 0x30]
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_6264
        add    esp, 0xc
loc_1_06BA:
        mov    al, byte ptr [esp + 0x13]
        mov    esi, dword ptr [esp + 0x28]
        mov    byte ptr [esi + 8], al
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_06D8:
        mov    ecx, dword ptr [esp + 4]
        movzx  edx, byte ptr [ecx + 1]
        mov    eax, dword ptr [esp + 8]
        shl    eax, 2
        add    eax, 3
        cmp    eax, edx
        ja     loc_1_0708
        cmp    dword ptr [esp + 0xc], edx
        jb     loc_1_0708
        inc    edx
        test   dl, 3
        jne    loc_1_0708
        cmp    byte ptr [ecx + 2], 4
        je     loc_1_0704
        sub    al, al
        ret    
        db  090h
loc_1_0704:
        mov    al, 1
        ret    
        db  090h
loc_1_0708:
        sub    al, al
        ret    
        db  090h
sub_1_070C:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], 0
        mov    cl, byte ptr [0xc158]         ; -> obj1:0xC158
        mov    byte ptr [eax + 9], cl
        mov    cl, byte ptr [0xc234]         ; -> loc_1_C234
        mov    byte ptr [eax + 0xa], cl
        mov    byte ptr [eax + 0xb], 0
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 8], 0
        ret    
sub_1_0738:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x13], 0
        mov    edx, dword ptr [esp + 0x28]
        mov    dword ptr [edx + 4], 0
        and    byte ptr [edx + 0xb], 0x7f
        cmp    dword ptr [esp + 0x24], 0
        jne    loc_1_0770
        mov    dword ptr [edx], 0
        mov    byte ptr [edx + 8], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_0770:
        mov    esi, dword ptr [esp + 0x24]
        and    esi, 3
        mov    eax, dword ptr [esp + 0x24]
        je     loc_1_0783
        and    eax, 0xfffffffc
        add    eax, 4
loc_1_0783:
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        jne    loc_1_07A4
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_07A4:
        or     esi, esi
        mov    edx, dword ptr [esp + 0x24]
        je     loc_1_07B2
        and    edx, 0xfffffffc
        add    edx, 4
loc_1_07B2:
        sub    eax, eax
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        sub    ebx, ebx
        mov    dword ptr [esp + 0x14], 0x2bf7
        cmp    dword ptr [esp + 0x24], eax
        je     loc_1_0A03
        mov    ebp, dword ptr [esp + 0x20]
loc_1_07DE:
        cmp    dword ptr [esp + 0x14], 0x2bf7
        jne    loc_1_0A15
        mov    cl, byte ptr [ebp]
        movzx  eax, cl
        cmp    eax, 0x44
        jg     loc_1_0810
        je     loc_1_08BC
        or     eax, eax
        je     loc_1_0834
        cmp    eax, 1
        je     loc_1_0844
        cmp    eax, 7
        je     loc_1_0858
        jmp    loc_1_0826
        db  08Dh, 049h, 000h
loc_1_0810:
        cmp    eax, 0x83
        je     loc_1_0969
        cmp    eax, 0x89
        je     loc_1_0948
loc_1_0826:
        mov    dword ptr [esp + 0x14], 0x2aff
        jmp    loc_1_09F8
        db  090h
loc_1_0834:
        mov    dword ptr [esp + 0x14], 0
        jmp    loc_1_09F8
        db  08Dh, 049h, 000h
loc_1_0844:
        inc    ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    byte ptr [esi + ebx], cl
        inc    ebx
        dec    dword ptr [esp + 0x24]
        jmp    loc_1_09F8
        db  08Bh, 0FFh
loc_1_0858:
        test   byte ptr [esp + 0x13], 4
        je     loc_1_086C
        mov    dword ptr [esp + 0x14], 0x2aff
        jmp    loc_1_09F8
loc_1_086C:
        push   dword ptr [esp + 0x24]
        or     byte ptr [esp + 0x17], 4
        push   1
        push   ebp
        call   sub_1_06D8
        add    esp, 0xc
        or     al, al
        jne    loc_1_0894
        mov    dword ptr [esp + 0x14], 0x2aff
        jmp    loc_1_09F8
        db  08Dh, 049h, 000h
loc_1_0894:
        mov    esi, ebp
        movzx  eax, byte ptr [ebp + 1]
        mov    edi, dword ptr [esp + 0x18]
        add    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebx, eax
        add    ebp, eax
        sub    dword ptr [esp + 0x24], eax
        jmp    loc_1_09F8
        db  090h
loc_1_08BC:
        test   byte ptr [esp + 0x13], 8
        je     loc_1_08D0
        mov    dword ptr [esp + 0x14], 0x2aff
        jmp    loc_1_09F8
loc_1_08D0:
        mov    cl, byte ptr [ebp + 3]
        or     byte ptr [esp + 0x13], 8
        mov    al, cl
        and    al, 0xf
        shr    cl, 4
        or     cl, cl
        jne    loc_1_0938
        or     al, al
        je     loc_1_08EF
        cmp    al, 1
        je     loc_1_08EF
        cmp    al, 3
        jne    loc_1_0938
loc_1_08EF:
        movzx  eax, byte ptr [ebp + 1]
        cmp    eax, dword ptr [esp + 0x24]
        ja     loc_1_0928
        cmp    eax, 8
        jb     loc_1_0928
        cmp    byte ptr [ebp + 2], 5
        jne    loc_1_0928
        mov    edi, dword ptr [esp + 0x18]
        add    edi, ebx
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebx, eax
        add    ebp, eax
        sub    dword ptr [esp + 0x24], eax
        jmp    loc_1_09F8
        db  090h
loc_1_0928:
        mov    dword ptr [esp + 0x14], 0x2aff
        jmp    loc_1_09F8
        db  08Dh, 049h, 000h
loc_1_0938:
        mov    dword ptr [esp + 0x14], 0x2aff
        jmp    loc_1_09F8
        db  08Dh, 049h, 000h
loc_1_0948:
        test   byte ptr [esp + 0x13], 3
        je     loc_1_095C
        mov    dword ptr [esp + 0x14], 0x2aff
        jmp    loc_1_09F8
loc_1_095C:
        mov    edx, dword ptr [esp + 0x28]
        or     byte ptr [edx + 0xb], 0x80
        or     byte ptr [esp + 0x13], 1
loc_1_0969:
        cmp    cl, 0x83
        jne    loc_1_098A
        test   byte ptr [esp + 0x13], 3
        je     loc_1_0980
        mov    dword ptr [esp + 0x14], 0x2aff
        jmp    loc_1_09F8
        db  090h
loc_1_0980:
        cmp    cl, 0x83
        jne    loc_1_098A
        or     byte ptr [esp + 0x13], 2
loc_1_098A:
        push   dword ptr [esp + 0x24]
        push   2
        push   ebp
        call   sub_1_06D8
        add    esp, 0xc
        or     al, al
        jne    loc_1_09A8
        mov    dword ptr [esp + 0x14], 0x2aff
        jmp    loc_1_09F8
        db  090h
loc_1_09A8:
        mov    al, byte ptr [ebp]
        mov    esi, dword ptr [esp + 0x18]
        mov    byte ptr [esi + ebx], al
        inc    ebp
        inc    ebx
        movzx  eax, byte ptr [ebp]
        inc    ebp
        sub    dword ptr [esp + 0x24], eax
        sub    eax, 4
        mov    byte ptr [esi + ebx], al
        inc    ebx
        mov    cl, byte ptr [ebp]
        mov    byte ptr [esi + ebx], cl
        inc    ebp
        inc    ebx
        mov    ecx, dword ptr [ebp]
        mov    edx, dword ptr [esp + 0x28]
        mov    dword ptr [edx + 4], ecx
        add    ebp, 4
        lea    edx, [eax - 3]
        mov    edi, esi
        add    edi, ebx
        mov    esi, ebp
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    ebx, [eax + ebx - 3]
        lea    ebp, [eax + ebp - 3]
loc_1_09F8:
        cmp    dword ptr [esp + 0x24], 0
        jne    loc_1_07DE
loc_1_0A03:
        cmp    dword ptr [esp + 0x14], 0x2bf7
        jne    loc_1_0A15
        mov    dword ptr [esp + 0x14], 0
loc_1_0A15:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_0A34
        push   dword ptr [esp + 0x18]
        call   sub_1_B944
        add    esp, 4
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_0A34:
        test   bl, 3
        je     loc_1_0A3F
        and    ebx, 0xfffffffc
        add    ebx, 4
loc_1_0A3F:
        cmp    ebx, 0x28
        jbe    loc_1_0A60
        push   dword ptr [esp + 0x18]
        call   sub_1_B944
        add    esp, 4
        mov    eax, 0x2b09
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_0A60:
        sub    eax, eax
        mov    edx, dword ptr [esp + 0x28]
        mov    esi, dword ptr [esp + 0x18]
        mov    dword ptr [edx], esi
        mov    byte ptr [edx + 8], bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
sub_1_0A78:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_0AA1
        mov    dword ptr [esi], 0
        push   eax
        call   sub_1_B944
        add    esp, 4
        mov    byte ptr [esi + 8], 0
        mov    dword ptr [esi + 4], 0
        and    byte ptr [esi + 0xb], 0x7f
loc_1_0AA1:
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_0AA8:
        cmp    dword ptr [esp + 8], 0x48
        jae    loc_1_0AB8
        mov    eax, 0x2af9
        ret    
        db  08Dh, 049h, 000h
loc_1_0AB8:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 1
        mov    dword ptr [eax + 4], 0x14
        mov    dword ptr [eax + 8], 0x6848   ; -> sub_1_6848
        mov    dword ptr [eax + 0xc], 0x348  ; -> sub_1_0348
        mov    dword ptr [eax + 0x10], 0x7a5c ; -> loc_1_7A5C
        mov    dword ptr [eax + 0x14], 0x7bec ; -> loc_1_7BEC
        mov    dword ptr [eax + 0x18], 0x43c ; -> loc_1_043C
        mov    dword ptr [eax + 0x1c], 0x44c ; -> loc_1_044C
        mov    dword ptr [eax + 0x20], 0x78bc ; -> loc_1_78BC
        mov    dword ptr [eax + 0x24], 0x78fc ; -> loc_1_78FC
        mov    dword ptr [eax + 0x28], 0x70c ; -> sub_1_070C
        mov    dword ptr [eax + 0x2c], 0x498 ; -> sub_1_0498
        mov    dword ptr [eax + 0x30], 0x738 ; -> sub_1_0738
        mov    dword ptr [eax + 0x34], 0xa78 ; -> sub_1_0A78
        mov    dword ptr [eax + 0x38], 0xab7c ; -> loc_1_AB7C
        mov    dword ptr [eax + 0x3c], 0xafcc ; -> loc_1_AFCC
        mov    dword ptr [eax + 0x40], 0xb2ac ; -> loc_1_B2AC
        mov    dword ptr [eax + 0x44], 0x3e0 ; -> loc_1_03E0
        sub    eax, eax
        ret    
loc_1_0B3C:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        sub    esi, 0x24
        sub    edi, edi
        push   esi
        call   sub_1_96CC
        add    esp, 4
        push   esi
        call   sub_1_AA10
        add    esp, 4
        cmp    dword ptr [esp + 0x14], edi
        je     loc_1_0BD8
        lea    eax, [esi + 0x1c]
        mov    edx, dword ptr [eax]
        cmp    edx, edi
        je     loc_1_0B8C
loc_1_0B69:
        mov    cl, byte ptr [edx + 0xf]
        dec    cl
        mov    byte ptr [edx + 0xf], cl
        jne    loc_1_0B84
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], edi
        mov    edi, edx
        inc    dword ptr [0xc218]            ; -> obj1:0xC218
        jmp    loc_1_0B86
        db  090h
loc_1_0B84:
        mov    eax, edx
loc_1_0B86:
        mov    edx, dword ptr [eax]
        or     edx, edx
        jne    loc_1_0B69
loc_1_0B8C:
        or     edi, edi
        je     loc_1_0BBC
loc_1_0B90:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_1_0BAF
        push   0
        push   1
        push   0xb
        lea    eax, [ebx + 0x1c]
        push   eax
        push   dword ptr [esi + 4]
        call   sub_1_9610
        add    esp, 0x14
loc_1_0BAF:
        push   ebx
        call   sub_1_4E1C
        add    esp, 4
        or     edi, edi
        jne    loc_1_0B90
loc_1_0BBC:
        push   0
        push   0xb3c                         ; -> loc_1_0B3C
        push   0x1f4
        add    esi, 0x24
        push   esi
        call   sub_1_B920
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0BD8:
        push   esi
        push   0xb3c                         ; -> loc_1_0B3C
        push   0x1f4
        add    esi, 0x24
        push   esi
        call   sub_1_B920
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_0BF4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [0xc090]       ; -> obj1:0xC090
        or     ebx, ebx
        je     loc_1_0D58
        mov    ax, word ptr [esp + 0x14]
        nop    
loc_1_0C0C:
        cmp    word ptr [ebx + 0x16], ax
        je     loc_1_0C18
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_1_0C0C
loc_1_0C18:
        or     ebx, ebx
        je     loc_1_0D58
        cmp    ebx, dword ptr [0xc050]       ; -> obj1:0xC050
        je     loc_1_0D58
        mov    edi, dword ptr [ebx + 0xc]
        inc    dword ptr [0xbbfc]            ; -> obj1:0xBBFC
        mov    esi, dword ptr [esp + 0x18]
        or     esi, esi
        mov    ax, word ptr [ebx + 0x14]
        jne    loc_1_0CC8
        test   al, 1
        je     loc_1_0CB8
        and    ax, 0xfffe
        mov    word ptr [ebx + 0x14], ax
        mov    eax, dword ptr [edi + 0x34]
        dec    eax
        mov    dword ptr [edi + 0x34], eax
        mov    esi, 0x7774                   ; -> loc_1_7774
        je     loc_1_0C64
        mov    esi, 0x7794                   ; -> loc_1_7794
loc_1_0C64:
        push   ebx
        call   sub_1_AAC4
        add    esp, 4
        push   0
        mov    ax, word ptr [ebx + 0x16]
        push   eax
        push   dword ptr [ebx + 0x18]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_0FE0
        add    esp, 0x14
        push   0
        push   edi
        push   esi
        call   sub_1_77EC
        add    esp, 0xc
        mov    eax, dword ptr [0xc050]       ; -> obj1:0xC050
        push   dword ptr [eax + 0xc]
        push   -1
        push   -1
        push   dword ptr [ebx + 4]
        call   sub_1_7F8C
        add    esp, 0x10
        push   0
        push   dword ptr [ebx + 4]
        push   0
        push   dword ptr [edi + 4]
        call   dword ptr [edi + 0x1c]
        add    esp, 0x10
loc_1_0CB8:
        pop    ebp
        dec    dword ptr [0xbbfc]            ; -> obj1:0xBBFC
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0CC8:
        test   al, 1
        jne    loc_1_0D48
        mov    dword ptr [ebx + 4], esi
        mov    ebp, dword ptr [esp + 0x1c]
        mov    dword ptr [ebx + 8], ebp
        or     ax, 1
        mov    word ptr [ebx + 0x14], ax
        inc    dword ptr [edi + 0x34]
        push   ebx
        call   sub_1_8EF4
        add    esp, 4
        or     eax, eax
        mov    edi, 1
        jne    loc_1_0CF5
        sub    edi, edi
loc_1_0CF5:
        or     edi, edi
        je     loc_1_0D0F
        mov    eax, dword ptr [ebx + 0xc]
        push   ebp
        push   esi
        push   0
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 0x18]
        add    esp, 0x10
        mov    edi, eax
        or     edi, edi
        jne    loc_1_0D24
loc_1_0D0F:
        push   0
        push   0
        mov    ax, word ptr [ebx + 0x16]
        push   eax
        call   sub_1_0BF4
        add    esp, 0xc
        jmp    loc_1_0D4A
        db  08Bh, 0FFh
loc_1_0D24:
        push   ebx
        call   sub_1_AA74
        add    esp, 4
        push   1
        mov    ax, word ptr [ebx + 0x16]
        push   eax
        push   dword ptr [ebx + 0x18]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_0FE0
        add    esp, 0x14
        jmp    loc_1_0D4A
        db  090h
loc_1_0D48:
        sub    edi, edi
loc_1_0D4A:
        mov    eax, edi
        dec    dword ptr [0xbbfc]            ; -> obj1:0xBBFC
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_0D58:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_0D60:
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 0x1c], 0
        mov    dword ptr [edx + 0x20], 0
        mov    ax, word ptr [0xc1bd]         ; -> obj1:0xC1BD
        mov    word ptr [edx + 0x16], ax
        inc    word ptr [0xc1bd]             ; -> obj1:0xC1BD
        mov    eax, dword ptr [edx + 0xc]
        lea    ecx, [eax + 0x28]
        cmp    dword ptr [ecx + 0x10], 0
        je     loc_1_0D99
        nop    
loc_1_0D90:
        mov    ecx, dword ptr [ecx + 0x10]
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_1_0D90
loc_1_0D99:
        mov    dword ptr [ecx + 0x10], edx
        mov    dword ptr [edx + 0x10], 0
        test   byte ptr [edx + 0x14], 1
        je     loc_1_0DAC
        inc    dword ptr [eax + 0x34]
loc_1_0DAC:
        push   0
        mov    dword ptr [edx + 0x30], 0
        mov    dword ptr [edx + 0x38], 0
        mov    eax, dword ptr [0xbf6c]       ; -> loc_1_BF6C
        mov    dword ptr [edx + 0x34], eax
        push   0xb3c                         ; -> loc_1_0B3C
        push   0x1f4
        add    edx, 0x24
        push   edx
        call   sub_1_B920
        add    esp, 0x10
        mov    eax, 1
        ret    
sub_1_0DE0:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x28]
        mov    ebx, dword ptr [esp + 0x2c]
        movzx  eax, word ptr [esi + 0x3c]
        mov    ecx, dword ptr [ebx + 4]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        sub    ax, 0x14
        mov    word ptr [esi + 0x3c], ax
        call   sub_1_B94A
        mov    eax, dword ptr [ebx + 4]
        sub    eax, 0x14
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_0E2C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_0E2C:
        push   0x5c
        push   1
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_BA02
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_0E5C
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_0E5C:
        push   dword ptr [esp + 0x14]
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_BA0E
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_0E94
        push   dword ptr [esp + 0x14]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_0E94:
        sub    eax, eax
        mov    edi, dword ptr [esp + 0x18]
        add    edi, 0x38
        mov    ecx, 0x17
        rep stosd dword ptr es:[edi], eax
        push   1
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_0ED8
        push   dword ptr [esp + 0x14]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_0ED8:
        push   dword ptr [ebx + 4]
        push   ebp
        push   dword ptr [esp + 0x24]
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_0F1C
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA2C
        push   dword ptr [esp + 0x14]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_1_0F1C:
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA50
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x40], ebp
        test   byte ptr [esi + 0x14], 1
        je     loc_1_0F70
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   0
        push   dword ptr [ebx]
        call   dword ptr [ebx + 0x1c]
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0F70
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA2C
        push   dword ptr [esp + 0x14]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_1_0F70:
        test   byte ptr [esi + 0x14], 4
        je     loc_1_0FB4
        push   0
        mov    eax, dword ptr [esi + 0xc]
        push   dword ptr [eax + 0x3c]
        push   2
        push   dword ptr [ebx]
        call   dword ptr [ebx + 0x1c]
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0FB4
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA2C
        push   dword ptr [esp + 0x14]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_0FB4:
        test   byte ptr [ebx + 0x38], 1
        je     loc_1_0FBF
        or     word ptr [esi + 0x14], 2
loc_1_0FBF:
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [edx + 0x2c]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0x38], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edx + 0x2c], eax
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
sub_1_0FE0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, 0xc06c                   ; -> loc_1_C06C
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x1c]
loc_1_0FF5:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     loc_1_100C
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   ebx
        push   edi
        push   esi
        call   eax
        add    esp, 0x14
loc_1_100C:
        add    ebp, 4
        cmp    ebp, 0xc08c                   ; -> loc_1_C08C
        jb     loc_1_0FF5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_101C:
        push   esi
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_102C
        mov    eax, dword ptr [esp + 8]
        sub    ecx, ecx
        jmp    loc_1_1032
loc_1_102C:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 8]
loc_1_1032:
        sub    esi, esi
        mov    edx, 0xc06c                   ; -> loc_1_C06C
loc_1_1039:
        cmp    dword ptr [edx], ecx
        je     loc_1_1050
        add    edx, 4
        inc    esi
        cmp    edx, 0xc08c                   ; -> loc_1_C08C
        jb     loc_1_1039
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_1050:
        mov    dword ptr [esi*4 + 0xc06c], eax ; -> loc_1_C06C
        mov    eax, 1
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_1060:
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0xc2cc                   ; -> loc_1_C2CC
        mov    si, word ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x14]
loc_1_1071:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     loc_1_107E
        push   edi
        push   esi
        call   eax
        add    esp, 8
loc_1_107E:
        add    ebx, 4
        cmp    ebx, 0xc2ec                   ; -> obj1:0xC2EC
        jb     loc_1_1071
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_1090:
        push   esi
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_10A0
        mov    edx, dword ptr [esp + 8]
        sub    esi, esi
        jmp    loc_1_10A6
loc_1_10A0:
        sub    edx, edx
        mov    esi, dword ptr [esp + 8]
loc_1_10A6:
        sub    ecx, ecx
        mov    eax, 0xc2cc                   ; -> loc_1_C2CC
loc_1_10AD:
        cmp    dword ptr [eax], esi
        je     loc_1_10C0
        add    eax, 4
        inc    ecx
        cmp    eax, 0xc2ec                   ; -> obj1:0xC2EC
        jb     loc_1_10AD
        sub    eax, eax
        pop    esi
        ret    
loc_1_10C0:
        pop    esi
        mov    dword ptr [ecx*4 + 0xc2cc], edx ; -> loc_1_C2CC
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
sub_1_10D0:
        sub    esp, 0x48
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B94A
        mov    dword ptr [esp + 0x24], 0
        lea    eax, [esp + 0x30]
        push   eax
        push   dword ptr [esp + 0x60]
        call   sub_1_B520
        add    esp, 8
        or     eax, eax
        jne    loc_1_1108
        mov    eax, 0x2b2a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x48
        ret    
        db  08Bh, 0FFh
loc_1_1108:
        push   dword ptr [esp + 0x30]
        mov    dword ptr [esp + 0x2c], 0
        lea    eax, [esp + 0x3c]
        push   eax
        call   sub_1_B5FC
        add    esp, 8
        or     eax, eax
        je     loc_1_11FF
        push   dword ptr [0xc13c]            ; -> obj1:0xC13C
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_11FF
        mov    edx, dword ptr [0xc13c]       ; -> obj1:0xC13C
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        cmp    dword ptr [esp + 0x38], eax
        mov    dword ptr [ebx + 0x3c], eax
        jne    loc_1_1169
        mov    dword ptr [ebx + 0x3c], 0xffffffff
loc_1_1169:
        mov    edi, dword ptr [esp + 0x6c]
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ebx + 0x10], eax
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [ebx + 0x14], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [ebx + 0x18], eax
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [ebx + 0x1c], eax
        mov    eax, dword ptr [edi + 0x2c]
        mov    dword ptr [ebx + 0x20], eax
        mov    eax, dword ptr [edi + 0x30]
        mov    dword ptr [ebx + 0x24], eax
        mov    eax, dword ptr [edi + 0x34]
        mov    dword ptr [ebx + 0x28], eax
        mov    dword ptr [ebx + 0x2c], 0
        mov    eax, dword ptr [edi + 4]
        sub    eax, 0x14
        mov    dword ptr [ebx + 0x40], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 0x44], eax
        mov    eax, dword ptr [edi + 0x38]
        and    eax, 2
        or     eax, eax
        mov    eax, 0
        setne  al
        mov    dword ptr [ebx + 0x48], eax
        mov    eax, dword ptr [edi + 0x3c]
        mov    dword ptr [ebx + 0x4c], eax
        mov    eax, dword ptr [edi + 0x40]
        mov    dword ptr [ebx + 0x50], eax
        push   dword ptr [esp + 0x30]
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_B728
        add    esp, 8
        mov    dword ptr [esp + 0x28], eax
        or     eax, eax
        jne    loc_1_122C
        push   ebx
        call   sub_1_B944
        add    esp, 4
loc_1_11FF:
        push   dword ptr [esp + 0x30]
        call   sub_1_B8F4
        add    esp, 4
loc_1_120B:
        cmp    dword ptr [esp + 0x28], 0
        je     loc_1_121E
        push   dword ptr [esp + 0x28]
        call   sub_1_B944
        add    esp, 4
loc_1_121E:
        pop    ebp
        mov    eax, 0x2b2a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x48
        ret    
        db  090h
loc_1_122C:
        push   dword ptr [esp + 0x30]
        call   sub_1_B8F4
        add    esp, 4
        mov    eax, dword ptr [0xc0b4]       ; -> obj1:0xC0B4
        mov    dword ptr [ebx], eax
        mov    dword ptr [0xc0b4], ebx       ; -> obj1:0xC0B4
        cmp    dword ptr [0xc1c8], 0         ; -> loc_1_C1C8
        jne    loc_1_1254
        mov    dword ptr [0xc1c8], ebx       ; -> loc_1_C1C8
loc_1_1254:
        sub    eax, eax
        inc    dword ptr [0xc054]            ; -> loc_1_C054
        mov    ecx, 1
        inc    esi
        mov    esi, dword ptr [0xc058]       ; -> obj1:0xC058
loc_1_1268:
        test   esi, ecx
        je     loc_1_1278
        add    ecx, ecx
        inc    eax
        cmp    eax, 0x20
        jb     loc_1_1268
        jmp    loc_1_1287
        db  08Bh, 0FFh
loc_1_1278:
        or     esi, ecx
        lea    edx, [eax + 1]
        mov    dword ptr [esp + 0x2c], edx
        mov    dword ptr [0xc058], esi       ; -> obj1:0xC058
loc_1_1287:
        cmp    eax, 0x20
        je     loc_1_120B
        mov    edx, dword ptr [esp + 0x2c]
        mov    dword ptr [ebx + 0x30], edx
        mov    dword ptr [esp + 0x18], 0
        cmp    dword ptr [esp + 0x20], 0
        jbe    loc_1_146C
        mov    ebp, dword ptr [esp + 0x28]
loc_1_12AE:
        mov    ecx, dword ptr [ebp]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    ecx, 0x10
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [esp + 0x14], edx
        mov    al, dl
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_120B
        cmp    al, 0xf0
        jne    loc_1_12F1
        cmp    edx, -1
        jne    loc_1_120B
loc_1_12F1:
        mov    eax, 0xc090                   ; -> obj1:0xC090
        mov    esi, dword ptr [0xc090]       ; -> obj1:0xC090
        or     esi, esi
        je     loc_1_1324
loc_1_1300:
        test   byte ptr [esi + 0x14], 8
        je     loc_1_1310
        mov    eax, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_1300
        jmp    loc_1_1320
loc_1_1310:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi], 0
        dec    dword ptr [0xc154]            ; -> obj1:0xC154
loc_1_1320:
        or     esi, esi
        jne    loc_1_1338
loc_1_1324:
        push   0x44
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_120B
loc_1_1338:
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 4], ecx
        mov    eax, dword ptr [ebp + 4]
        mov    edx, eax
        and    edx, 0xff00
        mov    dword ptr [esp + 0x1c], eax
        shl    dword ptr [esp + 0x1c], 0x10
        or     edx, dword ptr [esp + 0x1c]
        shl    edx, 8
        mov    dword ptr [esp + 0x10], eax
        and    dword ptr [esp + 0x10], 0xff0000
        shr    eax, 0x10
        or     dword ptr [esp + 0x10], eax
        shr    dword ptr [esp + 0x10], 8
        or     edx, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 8], edx
        mov    eax, dword ptr [esp + 0x3c]
        sub    eax, 0x44
        sbb    edx, edx
        not    edx
        and    eax, edx
        add    eax, 0x44
        mov    word ptr [esi + 0x3c], ax
        mov    eax, dword ptr [esp + 0x60]
        mov    dword ptr [esi + 0x18], eax
        mov    dword ptr [esi + 0xc], ebx
        cmp    ecx, 1
        sbb    eax, eax
        inc    ax
        mov    word ptr [esi + 0x14], ax
        or     ax, 8
        cmp    dword ptr [esp + 0x18], 0
        mov    word ptr [esi + 0x14], ax
        jne    loc_1_13F2
        or     ax, 4
        push   dword ptr [esp + 0x2c]
        mov    word ptr [esi + 0x14], ax
        mov    dword ptr [esp + 0x28], esi
        push   edi
        push   0x4bcc                        ; -> sub_1_4BCC
        push   0x5840                        ; -> sub_1_5840
        push   0x6df4                        ; -> sub_1_6DF4
        push   0x5bd8                        ; -> sub_1_5BD8
        push   0x5514                        ; -> sub_1_5514
        push   esi
        push   dword ptr [esp + 0x7c]
        call   dword ptr [esp + 0x8c]
        add    esp, 0x24
        or     eax, eax
        je     loc_1_120B
loc_1_13F2:
        push   edi
        mov    eax, dword ptr [0xc090]       ; -> obj1:0xC090
        mov    dword ptr [esi], eax
        mov    dword ptr [0xc090], esi       ; -> obj1:0xC090
        inc    dword ptr [0xc154]            ; -> obj1:0xC154
        push   esi
        call   sub_1_0DE0
        add    esp, 8
        or     eax, eax
        je     loc_1_120B
        push   esi
        call   sub_1_0D60
        add    esp, 4
        or     eax, eax
        je     loc_1_120B
        lea    eax, [esp + 0x44]
        push   eax
        push   dword ptr [esp + 0x44]
        push   esi
        call   sub_1_8FF8
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1454
        mov    ax, word ptr [esi + 0x14]
        test   al, 1
        je     loc_1_1454
        and    ax, 0xfffe
        mov    word ptr [esi + 0x14], ax
        mov    eax, dword ptr [esi + 0xc]
        dec    dword ptr [eax + 0x34]
loc_1_1454:
        add    ebp, 8
        inc    dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, dword ptr [esp + 0x20]
        jb     loc_1_12AE
        jmp    loc_1_1470
        db  090h
loc_1_146C:
        mov    esi, dword ptr [esp + 0x34]
loc_1_1470:
        push   dword ptr [edi]
        call   dword ptr [edi + 0x28]
        add    esp, 4
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_1492
        push   1
        mov    eax, dword ptr [esp + 0x28]
        mov    ax, word ptr [eax + 0x16]
        push   eax
        call   sub_1_1060
        add    esp, 8
loc_1_1492:
        sub    edi, edi
        cmp    dword ptr [esp + 0x20], edi
        jbe    loc_1_14D6
loc_1_149A:
        push   1
        mov    ax, word ptr [esi + 0x16]
        push   eax
        push   dword ptr [esi + 0x18]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   sub_1_0FE0
        add    esp, 0x14
        cmp    dword ptr [esi + 4], 0
        jne    loc_1_14C4
        mov    ax, word ptr [esi + 0x16]
        push   eax
        call   6
        jmp    loc_1_14CA
loc_1_14C4:
        push   esi
        call   sub_1_AA74
loc_1_14CA:
        inc    edi
        add    esp, 4
        mov    esi, dword ptr [esi]
        cmp    edi, dword ptr [esp + 0x20]
        jb     loc_1_149A
loc_1_14D6:
        push   dword ptr [esp + 0x28]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x48
        ret    
sub_1_14EC:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 0xc]
        mov    ebx, dword ptr [0xc090]       ; -> obj1:0xC090
        cmp    ebx, edi
        je     loc_1_15F5
loc_1_1506:
        cmp    dword ptr [ebx + 0xc], esi
        jne    loc_1_15EB
        or     edi, edi
        jne    loc_1_1515
        mov    edi, ebx
loc_1_1515:
        mov    ax, word ptr [ebx + 0x14]
        test   al, 1
        je     loc_1_1570
        and    ax, 0xfffe
        mov    word ptr [ebx + 0x14], ax
        push   ebx
        call   sub_1_AAC4
        add    esp, 4
        push   0
        mov    ax, word ptr [ebx + 0x16]
        push   eax
        push   dword ptr [ebx + 0x18]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_0FE0
        add    esp, 0x14
        push   0
        push   esi
        push   0x7774                        ; -> loc_1_7774
        call   sub_1_77EC
        add    esp, 0xc
        mov    eax, dword ptr [0xc050]       ; -> obj1:0xC050
        push   dword ptr [eax + 0xc]
        push   -1
        push   -1
        push   dword ptr [ebx + 4]
        call   sub_1_7F8C
        add    esp, 0x10
        jmp    loc_1_1586
        db  090h
loc_1_1570:
        push   0
        mov    ax, word ptr [ebx + 0x16]
        push   eax
        push   dword ptr [ebx + 0x18]
        push   0
        push   0
        call   sub_1_0FE0
        add    esp, 0x14
loc_1_1586:
        cmp    dword ptr [0xbc04], ebx       ; -> obj1:0xBC04
        jne    loc_1_1598
        mov    dword ptr [0xbc04], 0         ; -> obj1:0xBC04
loc_1_1598:
        lea    eax, [ebx + 0x24]
        and    word ptr [ebx + 0x14], 0xfff7
        push   eax
        call   sub_1_B926
        add    esp, 4
        mov    eax, dword ptr [ebx + 0x1c]
        or     eax, eax
        je     loc_1_15C6
loc_1_15B1:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx + 0x1c], ecx
        push   eax
        call   sub_1_4E1C
        add    esp, 4
        mov    eax, dword ptr [ebx + 0x1c]
        or     eax, eax
        jne    loc_1_15B1
loc_1_15C6:
        mov    eax, dword ptr [ebx + 0x20]
        or     eax, eax
        je     loc_1_15EB
loc_1_15CD:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx + 0x20], ecx
        push   0
        push   0
        push   0
        push   0x2b0b
        push   eax
        call   dword ptr [eax + 8]
        add    esp, 0x14
        mov    eax, dword ptr [ebx + 0x20]
        or     eax, eax
        jne    loc_1_15CD
loc_1_15EB:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_1_1506
loc_1_15F5:
        mov    eax, 1
        mov    ecx, dword ptr [esi + 0x30]
        dec    cl
        shl    eax, cl
        not    eax
        and    dword ptr [0xc058], eax       ; -> obj1:0xC058
        or     edi, edi
        je     loc_1_161C
        push   0
        mov    ax, word ptr [edi + 0x16]
        push   eax
        call   sub_1_1060
        add    esp, 8
loc_1_161C:
        mov    eax, dword ptr [esi + 0x2c]
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [edi + 4]
        push   dword ptr [eax + 0x10]
        call   sub_1_BA08
        push   dword ptr [edi + 8]
        call   sub_1_BA2C
        push   ebx
        call   sub_1_B944
        add    esp, 4
        cmp    esi, dword ptr [0xc1c8]       ; -> loc_1_C1C8
        jne    loc_1_1682
        push   dword ptr [0xc1c8]            ; -> loc_1_C1C8
        push   -1
        push   -1
        push   -1
        call   sub_1_7F8C
        add    esp, 0x10
        push   dword ptr [0xc1c8]            ; -> loc_1_C1C8
        push   -1
        push   -1
        push   0
        call   sub_1_7F8C
        add    esp, 0x10
        mov    dword ptr [0xc1c8], 0         ; -> loc_1_C1C8
        mov    dword ptr [0xbd6c], 0xffff    ; -> obj1:0xBD6C
loc_1_1682:
        mov    ecx, 0xc0b4                   ; -> obj1:0xC0B4
        cmp    dword ptr [0xc0b4], esi       ; -> obj1:0xC0B4
        je     loc_1_169C
        nop    
loc_1_1690:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_1_169C
        mov    ecx, eax
        cmp    dword ptr [ecx], esi
        jne    loc_1_1690
loc_1_169C:
        cmp    dword ptr [ecx], 0
        je     loc_1_16B4
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        dec    dword ptr [0xc054]            ; -> loc_1_C054
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_16B4:
        cmp    dword ptr [0xc1c8], 0         ; -> loc_1_C1C8
        jne    loc_1_1724
        cmp    dword ptr [0xc054], 1         ; -> loc_1_C054
        je     loc_1_1724
        mov    eax, dword ptr [0xc0b4]       ; -> obj1:0xC0B4
        mov    dword ptr [0xc1c8], eax       ; -> loc_1_C1C8
        mov    esi, dword ptr [0xc090]       ; -> obj1:0xC090
        or     esi, esi
        je     loc_1_1724
loc_1_16DA:
        test   byte ptr [esi + 0x14], 1
        je     loc_1_171E
        cmp    esi, dword ptr [0xc050]       ; -> obj1:0xC050
        je     loc_1_171E
        movzx  eax, word ptr [esi + 0x3c]
        mov    ecx, dword ptr [0xbd6c]       ; -> obj1:0xBD6C
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        mov    dword ptr [0xbd6c], eax       ; -> obj1:0xBD6C
        push   1
        push   2
        push   1
        push   eax
        push   dword ptr [0xc1c8]            ; -> loc_1_C1C8
        push   -1
        push   -1
        mov    eax, dword ptr [esi + 0xc]
        push   dword ptr [eax + 0x3c]
        call   sub_1_7F60
        add    esp, 0x20
loc_1_171E:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_16DA
loc_1_1724:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1728:
        sub    esp, 0x2c
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [esp + 0x1c], eax
        cmp    eax, 1
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [esp + 0x28], eax
        jne    loc_1_1752
loc_1_1752:
        mov    esi, dword ptr [esp + 0x38]
        lea    edi, [esi + 0x88]
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 0x8c], 0
        mov    dword ptr [esi + 0x90], 0
        call   sub_1_02A6
        lea    eax, [esp + 0xc]
        push   eax
        push   dword ptr [esi + 4]
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_B9AE
        cmp    dword ptr [esp + 8], 0x103
        jne    loc_1_17A4
        push   edi
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 8], eax
loc_1_17A4:
        mov    eax, dword ptr [esp + 0x4c]
        or     eax, eax
        je     loc_1_17B2
        mov    ecx, dword ptr [esp + 0x30]
        mov    dword ptr [eax], ecx
loc_1_17B2:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 0x2c
        ret    
sub_1_17BC:
        push   esi
        mov    edx, dword ptr [esp + 8]
        movzx  eax, byte ptr [edx + 0x5a]
        mov    esi, dword ptr [esp + 0xc]
        cmp    eax, dword ptr [esi + 0xc]
        jb     loc_1_17D8
        mov    eax, dword ptr [edx + 0x14]
        mov    dword ptr [esi], eax
        mov    dword ptr [edx + 0x14], esi
        pop    esi
        ret    
loc_1_17D8:
        push   esi
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edx + 0x18]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 0x18], eax
        call   sub_1_BA3E
        pop    esi
        ret    
        db  090h
sub_1_17EC:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esi + 0xd4]
        cmp    dword ptr [esi + 0xd8], eax
        jbe    loc_1_1835
        push   0xc
        call   sub_1_B93E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_1835
        push   0x20
        lea    eax, [edi + 4]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_1840
        push   edi
        call   sub_1_B944
        add    esp, 4
loc_1_1835:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
loc_1_1840:
        movzx  eax, byte ptr [esi + 0x5a]
        shl    eax, 5
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        jne    loc_1_1874
        push   dword ptr [edi + 4]
        call   sub_1_BA2C
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
loc_1_1874:
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0xdc]
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 0xdc], edi
        add    dword ptr [esi + 0xd4], 0x20
        sub    ebp, ebp
loc_1_1894:
        mov    ecx, ebp
        movzx  eax, byte ptr [esi + 0x5a]
        push   eax
        imul   ecx, eax
        add    ecx, dword ptr [esp + 0x1c]
        push   ecx
        push   dword ptr [edi + 4]
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_18DA
        or     ebp, ebp
        je     loc_1_18D0
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_17BC
        add    esp, 8
        jmp    loc_1_18D4
        db  090h
loc_1_18D0:
        mov    ebx, dword ptr [esp + 0x10]
loc_1_18D4:
        inc    ebp
        cmp    ebp, 0x20
        jb     loc_1_1894
loc_1_18DA:
        mov    eax, ebx
        sub    ebp, 0x20
        add    dword ptr [esi + 0xd4], ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
sub_1_18F0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x18]
        mov    bl, byte ptr [esp + 0x20]
        cmp    byte ptr [edi + 0x5a], bl
        jb     loc_1_197C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [esp + 0xc], eax
        or     eax, eax
        je     loc_1_1940
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        movzx  eax, bl
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [esp + 0x1c]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 0xc]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_1940:
        push   edi
        call   sub_1_17EC
        add    esp, 4
        mov    dword ptr [esp + 0xc], eax
        or     eax, eax
        je     loc_1_196E
        mov    dword ptr [eax], 0
        movzx  eax, bl
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [esp + 0x1c]
        mov    dword ptr [esi], eax
loc_1_196E:
        mov    eax, dword ptr [esp + 0xc]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_197C:
        mov    ecx, dword ptr [edi + 0x18]
        mov    esi, dword ptr [esp + 0x1c]
        mov    dword ptr [esi], ecx
        or     ecx, ecx
        je     loc_1_19D0
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi + 0x18], eax
        movzx  eax, bl
        push   eax
        push   ecx
        push   dword ptr [edi + 0xc]
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_19B8
        mov    eax, dword ptr [esp + 0xc]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_19B8:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 0x18], eax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_19D0:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h, 053h, 056h, 08Bh, 074h, 024h, 014h, 00Bh, 0F6h, 074h, 02Bh, 08Bh, 044h, 024h
        db  00Ch, 08Bh, 04Ch, 024h, 010h, 08Ah, 019h, 08Ah, 0D3h, 0C0h, 0EAh, 004h, 081h, 0E2h, 0FFh, 000h
        db  000h, 000h, 08Ah, 092h
        dd  obj1:0xBC68              ; fixup
        db  083h, 0E3h, 00Fh, 00Ah, 093h
        dd  obj1:0xBC78              ; fixup
        db  088h, 010h, 040h, 041h, 04Eh, 075h, 0DDh, 05Eh, 05Bh, 0C3h
sub_1_1A14:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ax, word ptr [esp + 0x3c]
        mov    edx, dword ptr [esp + 0x24]
        mov    word ptr [edx], ax
        mov    word ptr [edx + 2], 8
        mov    ebx, dword ptr [esp + 0x1c]
        mov    cl, byte ptr [ebx + 0x5c]
        mov    byte ptr [edx + 4], cl
        mov    byte ptr [edx + 5], 4
        mov    ax, word ptr [esp + 0x28]
        mov    word ptr [edx + 6], ax
        add    edx, 8
        mov    esi, dword ptr [esp + 0x30]
        or     esi, esi
        jne    loc_1_1A52
        lea    esi, [ebx + 0x54]
loc_1_1A52:
        mov    edi, edx
        movzx  ebp, cl
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, ebp
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [edx], eax
        add    edx, 4
        mov    esi, dword ptr [esp + 0x2c]
        or     esi, esi
        mov    edi, edx
        je     loc_1_1A8C
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_1A9C
        db  08Bh, 0FFh
loc_1_1A8C:
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
loc_1_1A9C:
        sub    edi, edi
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [edx + ebp], eax
        cmp    dword ptr [esp + 0x40], edi
        je     loc_1_1AC4
        cmp    byte ptr [ebx + 0x5b], 0
        je     loc_1_1AC4
        mov    edi, 1
        mov    dword ptr [esp + 0x10], 0xc0010011
        mov    esi, dword ptr [esp + 0x20]
        jmp    loc_1_1B16
loc_1_1AC4:
        inc    dword ptr [ebx + 0x50]
        mov    esi, dword ptr [esp + 0x20]
        push   esi
        push   dword ptr [ebx + 4]
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_B99C
        cmp    dword ptr [esp + 0x10], 0x103
        je     loc_1_1B16
        mov    edi, 1
        dec    dword ptr [ebx + 0x50]
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_1AFC
        mov    eax, dword ptr [esi + 4]
        add    dword ptr [ebx + 0x44], eax
        jmp    loc_1_1B16
        db  08Bh, 0FFh
loc_1_1AFC:
        cmp    dword ptr [esp + 0x10], 0xc000009a
        jne    loc_1_1B10
        inc    dword ptr [ebx + 0xac]
        jmp    loc_1_1B16
        db  08Bh, 0FFh
loc_1_1B10:
        inc    dword ptr [ebx + 0xb0]
loc_1_1B16:
        or     edi, edi
        je     loc_1_1B38
        lea    eax, [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_BA5C
        push   dword ptr [esp + 0x14]
        push   ebx
        call   sub_1_17BC
        add    esp, 8
        push   esi
        call   sub_1_BA1A
loc_1_1B38:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_1B44:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x15], 0
        mov    byte ptr [esp + 0x14], 0
        mov    ebx, dword ptr [esp + 0x38]
        lea    edx, [ebx + 0x20]
        sub    ebp, ebp
loc_1_1B5E:
        mov    eax, dword ptr [edx + 8]
        or     eax, eax
        je     loc_1_1B7C
        mov    ecx, dword ptr [edx + 0xc]
        mov    esi, dword ptr [esp + 0x3c]
        and    esi, ecx
        mov    edi, eax
        and    edi, ecx
        cmp    esi, edi
        je     loc_1_1B84
        or     ebp, ebp
        jne    loc_1_1B7C
        mov    ebp, eax
loc_1_1B7C:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_1_1B5E
        jmp    loc_1_1B87
loc_1_1B84:
        mov    ebp, dword ptr [edx + 8]
loc_1_1B87:
        or     ebp, ebp
        jne    loc_1_1B98
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Dh, 049h, 000h
loc_1_1B98:
        push   dword ptr [ebx + 0x10]
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        call   sub_1_BA0E
        cmp    dword ptr [esp + 0x30], 0
        je     loc_1_1BC4
        inc    dword ptr [ebx + 0xac]
        mov    eax, dword ptr [esp + 0x30]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  090h
loc_1_1BC4:
        mov    eax, dword ptr [esp + 0x24]
        mov    byte ptr [eax + 0x3c], 0
        inc    dword ptr [ebx + 0x4c]
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     loc_1_1C00
        cmp    eax, 1
        je     loc_1_1C48
        cmp    eax, 2
        je     loc_1_1C84
        cmp    eax, 7
        je     loc_1_1CAC
        inc    dword ptr [ebx + 0xb0]
        mov    eax, 0xc0010019
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
loc_1_1C00:
        mov    esi, 0xbc0c                   ; -> obj1:0xBC0C
        mov    byte ptr [esp + 0x13], 0xe
        cmp    byte ptr [ebx + 0x61], 0
        jne    loc_1_1C24
        mov    dword ptr [esp + 0x1c], 0
        mov    word ptr [esp + 0x16], 0x100
        jmp    loc_1_1C38
        db  08Dh, 049h, 000h
loc_1_1C24:
        mov    byte ptr [esp + 0x14], 8
        mov    dword ptr [esp + 0x1c], 0xbc54 ; -> loc_1_BC54
        mov    word ptr [esp + 0x16], 0x600
loc_1_1C38:
        mov    dword ptr [esp + 0x20], 6
        jmp    loc_1_1CCD
        db  08Dh, 049h, 000h
loc_1_1C48:
        mov    esi, 0xbc1c                   ; -> loc_1_BC1C
        mov    dword ptr [esp + 0x20], 8
        mov    byte ptr [esp + 0x14], 8
        mov    dword ptr [esp + 0x1c], 0xbc54 ; -> loc_1_BC54
        mov    byte ptr [esp + 0x13], 0xe
        cmp    byte ptr [esp + 0x40], 1
        mov    word ptr [esp + 0x16], 0x600
        jne    loc_1_1CCD
        mov    byte ptr [esp + 0x13], 0x10
        mov    byte ptr [esp + 0x15], 0x80
        jmp    loc_1_1CCD
        db  08Dh, 049h, 000h
loc_1_1C84:
        mov    esi, 0xbc30                   ; -> loc_1_BC30
        mov    byte ptr [esp + 0x13], 0xd
        mov    dword ptr [esp + 0x1c], 0xbc54 ; -> loc_1_BC54
        mov    byte ptr [esp + 0x14], 8
        mov    dword ptr [esp + 0x20], 7
        mov    word ptr [esp + 0x16], 0x100
        jmp    loc_1_1CCD
loc_1_1CAC:
        mov    esi, 0xbc40                   ; -> obj1:0xBC40
        mov    byte ptr [esp + 0x13], 3
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0
        mov    word ptr [esp + 0x16], 0x700
loc_1_1CCD:
        mov    al, byte ptr [esp + 0x14]
        add    al, byte ptr [esp + 0x13]
        add    al, 0x1c
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   ebx
        call   sub_1_18F0
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1D08
        push   dword ptr [esp + 0x24]
        call   sub_1_BA1A
        inc    dword ptr [ebx + 0xac]
        mov    eax, 0xc000009a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Bh, 0FFh
loc_1_1D08:
        cmp    dword ptr [ebx + 8], 7
        jne    loc_1_1D12
        sub    dword ptr [eax + 0xc], 0xa
loc_1_1D12:
        mov    edi, dword ptr [esp + 0x18]
        movzx  ecx, byte ptr [esp + 0x13]
        mov    dword ptr [esp + 0x28], ecx
        mov    edx, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    dword ptr [esp + 0x44], 0
        jne    loc_1_1D3D
        lea    ecx, [ebx + 0x54]
        mov    dword ptr [esp + 0x44], ecx
loc_1_1D3D:
        cmp    dword ptr [ebx + 8], 0
        jne    loc_1_1D53
        cmp    byte ptr [ebx + 0x61], 0
        je     loc_1_1D53
        mov    ecx, dword ptr [esp + 0x18]
        mov    word ptr [ecx + 0xc], 0x2400
loc_1_1D53:
        movzx  edx, byte ptr [ebx + 0x5c]
        mov    edi, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x44]
        mov    ecx, edx
        shr    ecx, 2
        add    edi, dword ptr [esp + 0x20]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [esp + 0x18]
        mov    cl, byte ptr [esp + 0x15]
        mov    edx, dword ptr [esp + 0x20]
        or     byte ptr [esi + edx], cl
        movzx  ecx, byte ptr [esp + 0x14]
        mov    dword ptr [esp + 0x2c], ecx
        mov    edx, ecx
        mov    edi, dword ptr [esp + 0x28]
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, edx
        shr    ecx, 2
        add    edi, dword ptr [esp + 0x18]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   eax
        push   dword ptr [esp + 0x28]
        call   sub_1_BA50
        push   dword ptr [esp + 0x48]
        push   dword ptr [esp + 0x1a]
        push   ebp
        push   dword ptr [esp + 0x48]
        push   dword ptr [esp + 0x54]
        push   0
        push   0x100
        mov    eax, dword ptr [esp + 0x34]
        add    eax, dword ptr [esp + 0x48]
        add    eax, dword ptr [esp + 0x44]
        push   eax
        push   dword ptr [esp + 0x44]
        push   ebx
        call   sub_1_1A14
        add    esp, 0x28
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Dh, 049h, 000h
sub_1_1DEC:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x24]
        push   dword ptr [ebp + 0x10]
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_BA0E
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_1E20
        inc    dword ptr [ebp + 0xac]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_1E20:
        mov    eax, dword ptr [esp + 0x18]
        mov    byte ptr [eax + 0x3c], 0
        inc    dword ptr [ebp + 0x48]
        mov    al, byte ptr [ebp + 0x60]
        mov    byte ptr [esp + 0x13], al
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_1E40
        add    al, byte ptr [ebp + 0x61]
        mov    byte ptr [esp + 0x13], al
loc_1_1E40:
        cmp    dword ptr [ebp + 8], 1
        mov    ebx, dword ptr [esp + 0x38]
        jne    loc_1_1E4E
        add    byte ptr [esp + 0x13], bl
loc_1_1E4E:
        mov    al, byte ptr [esp + 0x13]
        add    al, 0x1c
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ebp
        call   sub_1_18F0
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1E80
        inc    dword ptr [ebp + 0xac]
        push   dword ptr [esp + 0x18]
        call   sub_1_BA1A
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_1E80:
        mov    ecx, dword ptr [ebp + 8]
        or     ecx, ecx
        je     loc_1_1EC4
        cmp    ecx, 1
        je     loc_1_1F10
        cmp    ecx, 2
        je     loc_1_1F78
        cmp    ecx, 7
        je     loc_1_1FA8
        inc    dword ptr [ebp + 0xb0]
        push   eax
        push   ebp
        call   sub_1_17BC
        add    esp, 8
        push   dword ptr [esp + 0x18]
        call   sub_1_BA1A
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_1EC4:
        mov    ecx, dword ptr [esp + 0x14]
        mov    edi, ecx
        mov    esi, dword ptr [esp + 0x30]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebp + 0x54]
        lea    edi, [ecx + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        cmp    dword ptr [esp + 0x3c], 0
        jne    loc_1_1EF0
        mov    word ptr [ecx + 0xc], 0x608
        mov    bx, 0x100
        jmp    loc_1_1FC8
loc_1_1EF0:
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        mov    word ptr [ecx + 0xc], 0x2400
        mov    bx, 0x600
        mov    edi, dword ptr [esp + 0x14]
        add    edi, 0xe
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_1FC8
        db  08Dh, 049h, 000h
loc_1_1F10:
        mov    edx, dword ptr [esp + 0x14]
        mov    ecx, edx
        mov    byte ptr [ecx], 0x10
        mov    byte ptr [edx + 1], 0x40
        lea    edi, [edx + 2]
        mov    esi, dword ptr [esp + 0x30]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebp + 0x54]
        lea    edi, [edx + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        or     ebx, ebx
        je     loc_1_1F61
        mov    edi, dword ptr [esp + 0x14]
        add    edi, 0xe
        mov    esi, dword ptr [esp + 0x34]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x14]
        and    byte ptr [ecx + 0xe], 0x1f
        mov    ecx, dword ptr [esp + 0x14]
        xor    byte ptr [ecx + 0xf], 0x80
        or     byte ptr [edx + 8], 0x80
loc_1_1F61:
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        mov    ecx, dword ptr [esp + 0x14]
        lea    edi, [ecx + ebx + 0xe]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    bx, 0x600
        jmp    loc_1_1FC8
        db  08Bh, 0FFh
loc_1_1F78:
        mov    edx, dword ptr [esp + 0x14]
        mov    ecx, edx
        mov    byte ptr [ecx], 0x57
        lea    edi, [edx + 1]
        mov    esi, dword ptr [esp + 0x30]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebp + 0x54]
        lea    edi, [edx + 7]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        mov    edi, dword ptr [esp + 0x14]
        add    edi, 0xd
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    bx, 0x100
        jmp    loc_1_1FC8
loc_1_1FA8:
        mov    dl, byte ptr [ebp + 0x54]
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, esi
        mov    byte ptr [ecx], dl
        mov    ecx, dword ptr [esp + 0x30]
        mov    cl, byte ptr [ecx]
        mov    byte ptr [esi + 1], cl
        mov    byte ptr [esi + 2], 0xd5
        sub    dword ptr [eax + 0xc], 0xa
        mov    bx, 0x700
loc_1_1FC8:
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA50
        push   1
        push   ebx
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x34]
        push   0
        push   dword ptr [esp + 0x44]
        push   0x200
        movzx  eax, byte ptr [esp + 0x2f]
        add    eax, dword ptr [esp + 0x30]
        push   eax
        push   dword ptr [esp + 0x38]
        push   ebp
        call   sub_1_1A14
        add    esp, 0x28
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
sub_1_2008:
        mov    edx, dword ptr [esp + 4]
        add    edx, 0x10
        cmp    dword ptr [edx], 0
        je     loc_1_2036
        mov    ecx, dword ptr [esp + 8]
loc_1_2018:
        mov    eax, dword ptr [edx]
        cmp    ecx, eax
        je     loc_1_2028
        lea    edx, [eax + 0x18]
        cmp    dword ptr [edx], 0
        jne    loc_1_2018
        ret    
        db  090h
loc_1_2028:
        add    ecx, 0x18
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    dword ptr [ecx + 4], 0
loc_1_2036:
        ret    
        db  090h
sub_1_2038:
        mov    cl, byte ptr [esp + 0xb]
        and    ecx, 0x1f
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x1c]
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     loc_1_205C
loc_1_204D:
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 8], ecx
        je     loc_1_205E
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_204D
loc_1_205C:
        sub    eax, eax
loc_1_205E:
        ret    
        db  090h
sub_1_2060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x34]
        cmp    dword ptr [ebp + 0xc], edx
        je     loc_1_20EC
        mov    al, byte ptr [ebp + 0xc]
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_20EC
        add    esi, 0x20
        nop    
loc_1_2080:
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_1_20C5
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, eax
        not    ecx
        and    ecx, edx
        mov    edi, dword ptr [ebp - 4]
        and    edi, eax
        or     ecx, edi
        cmp    ecx, dword ptr [ebp + 0xc]
        je     loc_1_20D4
        mov    al, byte ptr [ebp - 4]
        shr    al, 2
        and    eax, 0xff
        and    eax, 0xfffffffc
        mov    eax, dword ptr [eax + 0xbd00] ; -> loc_1_BD00
        mov    edi, eax
        not    edi
        and    edi, edx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, eax
        or     edi, ecx
        cmp    edi, dword ptr [ebp + 0xc]
        je     loc_1_20E0
loc_1_20C5:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_2080
        sub    eax, eax
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_20D4:
        pop    edi
        mov    eax, 1
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_20E0:
        pop    edi
        mov    eax, 1
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_20EC:
        pop    edi
        mov    eax, 1
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
sub_1_20F8:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x28]
        mov    ebx, dword ptr [esp + 0x28]
        push   ebx
        call   sub_1_2060
        add    esp, 8
        or     eax, eax
        je     loc_1_2388
        cmp    byte ptr [ebx + 0x5b], 0
        jne    loc_1_2370
        mov    al, byte ptr [ebx + 0x60]
        add    al, byte ptr [ebx + 0x61]
        cmp    dword ptr [ebx + 8], 1
        jne    loc_1_2130
        add    al, 2
loc_1_2130:
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   ebx
        call   sub_1_18F0
        add    esp, 0xc
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        je     loc_1_2358
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     loc_1_2190
        cmp    eax, 1
        je     loc_1_2220
        cmp    eax, 2
        je     loc_1_2258
        cmp    eax, 7
        je     loc_1_22B0
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [eax], 0xc0010019
        push   dword ptr [esp + 0x14]
        push   ebx
        call   sub_1_17BC
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_2190:
        mov    ebp, dword ptr [esp + 0x10]
        mov    al, byte ptr [esp + 0x28]
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_21AC
        mov    esi, 0xbc0c                   ; -> obj1:0xBC0C
        mov    edi, ebp
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        jmp    loc_1_21C4
        db  08Bh, 0FFh
loc_1_21AC:
        mov    esi, 0xbc44                   ; -> loc_1_BC44
        mov    edi, dword ptr [esp + 0x10]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 0xffff7f00
        or     dword ptr [ebp + 2], eax
loc_1_21C4:
        lea    esi, [ebx + 0x54]
        lea    edi, [ebp + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        cmp    byte ptr [ebx + 0x61], 0
        jne    loc_1_21E0
        mov    word ptr [ebp + 0xc], 8
        jmp    loc_1_22C3
        db  08Bh, 0FFh
loc_1_21E0:
        lea    eax, [esp + 0x1c]
        push   eax
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x3c]
        call   sub_1_BA20
        mov    eax, dword ptr [esp + 0x1c]
        add    ax, 8
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebp + 0xc], cx
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0xe
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        mov    edi, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    word ptr [eax + 6], 8
        jmp    loc_1_22C3
        db  090h
loc_1_2220:
        mov    esi, 0xbc1c                   ; -> loc_1_BC1C
        mov    eax, dword ptr [esp + 0x10]
        mov    edi, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebx + 0x54]
        lea    edi, [eax + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        or     byte ptr [eax + 8], 0x80
        add    eax, 0xe
        mov    byte ptr [eax], 0xc2
        mov    byte ptr [eax + 1], 0x70
        add    eax, 2
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        mov    edi, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    word ptr [eax + 6], 8
        jmp    loc_1_22C3
loc_1_2258:
        mov    ecx, dword ptr [esp + 0x10]
        mov    al, byte ptr [esp + 0x28]
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_2274
        mov    esi, 0xbc30                   ; -> loc_1_BC30
        mov    edi, ecx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_228D
        db  090h
loc_1_2274:
        mov    esi, 0xbc4c                   ; -> loc_1_BC4C
        mov    edi, dword ptr [esp + 0x10]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 0xffff7f00
        or     dword ptr [ecx + 3], eax
loc_1_228D:
        lea    esi, [ebx + 0x54]
        lea    edi, [ecx + 7]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0xd
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        mov    edi, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    word ptr [eax + 6], 8
        jmp    loc_1_22C3
        db  08Bh, 0FFh
loc_1_22B0:
        mov    dl, byte ptr [ebx + 0x54]
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, eax
        mov    byte ptr [ecx], dl
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [eax + 2], 0xd4
loc_1_22C3:
        inc    dword ptr [ebx + 0x4c]
        inc    dword ptr [ebx + 0x50]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x30]
        call   sub_1_BA50
        push   dword ptr [esp + 0x2c]
        push   dword ptr [ebx + 4]
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_B99C
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esp + 0x18], 0x103
        je     loc_1_237A
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_2310
        mov    eax, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [eax + 4]
        add    dword ptr [ebx + 0x44], eax
        jmp    loc_1_232A
        db  08Bh, 0FFh
loc_1_2310:
        cmp    dword ptr [esp + 0x18], 0xc000009a
        jne    loc_1_2324
        inc    dword ptr [ebx + 0xac]
        jmp    loc_1_232A
        db  08Bh, 0FFh
loc_1_2324:
        inc    dword ptr [ebx + 0xb0]
loc_1_232A:
        dec    dword ptr [ebx + 0x50]
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x30]
        call   sub_1_BA5C
        push   dword ptr [esp + 0x14]
        push   ebx
        call   sub_1_17BC
        add    esp, 8
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_2358:
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [eax], 0xc000009a
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_2370:
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [eax], 0xc0010011
loc_1_237A:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_2388:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
sub_1_2394:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x24]
        cmp    byte ptr [ebx + 0x5b], 0
        jne    loc_1_2530
        mov    ebp, dword ptr [esp + 0x2c]
        mov    al, byte ptr [ebp + 0x19]
        cmp    al, 2
        jne    loc_1_24C4
        mov    dword ptr [ebp + 0x14], 0x78
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   ebx
        call   sub_1_18F0
        add    esp, 0xc
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        je     loc_1_24B0
        inc    dword ptr [ebx + 0x48]
        inc    dword ptr [ebx + 0x50]
        movzx  eax, byte ptr [ebp + 0x18]
        lea    esi, [ebp + 0x1a]
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    dword ptr [ebx + 8], 0
        jne    loc_1_2434
        cmp    byte ptr [ebp + 0x18], 0xe
        je     loc_1_2434
        lea    eax, [esp + 0x1c]
        push   eax
        push   0
        push   0
        push   0
        mov    esi, dword ptr [esp + 0x38]
        push   esi
        call   sub_1_BA20
        mov    eax, dword ptr [esp + 0x1c]
        add    ax, 8
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esp + 0x18]
        mov    word ptr [eax + 0xc], cx
        jmp    loc_1_2438
        db  090h
loc_1_2434:
        mov    esi, dword ptr [esp + 0x28]
loc_1_2438:
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_BA50
        push   esi
        push   dword ptr [ebx + 4]
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_B99C
        cmp    dword ptr [esp + 0x10], 0x103
        je     loc_1_24A1
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_246C
        mov    eax, dword ptr [esi + 4]
        add    dword ptr [ebx + 0x44], eax
        jmp    loc_1_2486
        db  08Dh, 049h, 000h
loc_1_246C:
        cmp    dword ptr [esp + 0x10], 0xc000009a
        jne    loc_1_2480
        inc    dword ptr [ebx + 0xac]
        jmp    loc_1_2486
        db  08Bh, 0FFh
loc_1_2480:
        inc    dword ptr [ebx + 0xb0]
loc_1_2486:
        dec    dword ptr [ebx + 0x50]
        lea    eax, [esp + 0x14]
        push   eax
        push   esi
        call   sub_1_BA5C
        push   dword ptr [esp + 0x14]
        push   ebx
        call   sub_1_17BC
        add    esp, 8
loc_1_24A1:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_24B0:
        pop    ebp
        inc    dword ptr [ebx + 0xac]
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_24C4:
        cmp    al, 1
        ja     loc_1_251C
        mov    edi, dword ptr [ebp + 0xc]
        call   sub_1_B92C
        mov    esi, dword ptr [esp + 0x28]
        mov    dword ptr [ebp + 0xc], esi
        mov    ecx, eax
        sub    ecx, dword ptr [ebp + 4]
        cmp    ecx, 0x3e8
        jbe    loc_1_24FD
        mov    dword ptr [ebp + 4], eax
        mov    byte ptr [ebp + 0x19], 1
        push   1
        push   0
        push   1
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_1_1B44
        add    esp, 0x14
loc_1_24FD:
        or     edi, edi
        je     loc_1_250E
        push   0
        push   edi
        push   dword ptr [ebx]
        call   sub_1_5BD8
        add    esp, 0xc
loc_1_250E:
        pop    ebp
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_251C:
        pop    ebp
        inc    dword ptr [ebx + 0xb0]
        mov    eax, 0xc001000f
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_2530:
        pop    ebp
        mov    eax, 0xc0010011
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_2540:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    al, byte ptr [esp + 0x23]
        and    eax, 0x1f
        mov    dword ptr [esp + 0x14], eax
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        mov    esi, dword ptr [esp + 0x24]
        push   esi
        call   sub_1_2038
        add    esp, 0xc
        or     eax, eax
        je     loc_1_2580
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_2580:
        mov    ebp, 0x28
        cmp    dword ptr [esi + 8], 1
        je     loc_1_2596
        movzx  eax, byte ptr [esi + 0x60]
        movzx  ecx, byte ptr [esi + 0x61]
        lea    ebp, [eax + ecx]
loc_1_2596:
        add    ebp, 0x1a
        push   ebp
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_25B4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_25B4:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebp
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ebx + 8], eax
        cmp    dword ptr [esi + 8], 1
        jne    loc_1_25E0
        cmp    dword ptr [0xc1c4], 0         ; -> obj1:0xC1C4
        mov    byte ptr [ebx + 0x19], 0
        je     loc_1_25E4
loc_1_25E0:
        mov    byte ptr [ebx + 0x19], 1
loc_1_25E4:
        mov    dword ptr [ebx + 0x10], 0
        call   sub_1_B92C
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 0x14], 0x78
        mov    ecx, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esp + 0x14]
        lea    eax, [ecx + eax*4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [eax], ebx
        inc    dword ptr [esi + 0x68]
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx], edx
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_2624:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x30]
        or     esi, esi
        je     loc_1_2670
        lea    ebp, [esi + 0x18]
        mov    eax, dword ptr [ebp + 4]
        or     eax, eax
        mov    edi, dword ptr [esp + 0x2c]
        je     loc_1_2678
        cmp    dword ptr [eax + 8], edi
        jne    loc_1_2664
        push   dword ptr [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x30]
        call   sub_1_2394
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_2664:
        push   esi
        push   eax
        call   sub_1_2008
        add    esp, 8
        jmp    loc_1_2678
loc_1_2670:
        mov    edi, dword ptr [esp + 0x2c]
        mov    ebp, dword ptr [esp + 0x1c]
loc_1_2678:
        lea    eax, [esp + 0x10]
        push   eax
        push   edi
        mov    ebx, dword ptr [esp + 0x2c]
        push   ebx
        call   sub_1_2038
        add    esp, 0xc
        or     eax, eax
        je     loc_1_26BC
        or     esi, esi
        je     loc_1_269F
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [eax + 0x10], esi
        mov    dword ptr [ebp + 4], eax
loc_1_269F:
        push   dword ptr [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x30]
        push   ebx
        call   sub_1_2394
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_26BC:
        lea    eax, [esp + 0x14]
        push   eax
        mov    esi, dword ptr [esp + 0x2c]
        push   esi
        push   edi
        push   ebx
        call   sub_1_20F8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_26E0
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_26E0:
        lea    eax, [esp + 0x10]
        push   eax
        push   edi
        push   ebx
        call   sub_1_2540
        add    esp, 0xc
        db  00Bh
sub_1_26F0:
        sal    byte ptr [ecx + esi*2 - 0x76], 0x48
        sbb    dword ptr [eax + 0x3d7701f9], eax
        cmp    dword ptr [eax + 0xc], 0
        jne    loc_1_2720
        mov    dword ptr [eax + 0xc], esi
        push   1
        push   0
        push   ecx
        push   edi
        push   ebx
        call   sub_1_1B44
        add    esp, 0x14
        mov    eax, 0x103
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_2720:
        push   dword ptr [esp + 0x10]
        push   eax
        push   esi
        push   ebx
        call   sub_1_2394
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h, 080h, 0F9h, 002h, 075h, 017h, 0FFh, 074h, 024h, 010h, 050h, 056h, 053h, 0E8h, 04Bh, 0FCh
        db  0FFh, 0FFh, 083h, 0C4h, 010h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 010h, 0C3h, 05Dh, 0B8h, 09Ah
        db  000h, 000h, 0C0h, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 010h, 0C3h, 08Dh, 049h, 000h, 05Dh, 0B8h, 09Ah
        db  000h, 000h, 0C0h, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 010h, 0C3h, 08Dh, 049h, 000h
sub_1_2774:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        je     loc_1_2790
        nop    
loc_1_2780:
        add    ecx, 0x18
        mov    dword ptr [ecx + 4], 0
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_2780
loc_1_2790:
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        ret    
        db  08Dh, 049h, 000h
loc_1_279C:
        sub    esp, 4
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x1c]
        add    eax, dword ptr [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x1c]
        push   dword ptr [eax + 4]
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_B9A2
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_27D4:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    byte ptr [eax + 0x97], 2
        mov    byte ptr [eax + 0x5b], 2
        lea    esi, [eax + 0x88]
        mov    dword ptr [esi], 0
        mov    dword ptr [eax + 0x8c], 0
        mov    dword ptr [eax + 0x90], 0
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_1_2830
        push   eax
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_B996
        cmp    dword ptr [esp + 4], 0x103
        jne    loc_1_2830
        push   esi
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 4], eax
loc_1_2830:
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_2838:
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        lea    edi, [eax + 0x18]
        mov    ecx, dword ptr [edi + 4]
        or     ecx, ecx
        je     loc_1_2855
        push   eax
        push   ecx
        call   sub_1_2008
        add    esp, 8
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
loc_1_2855:
        pop    edi
        ret    
        db  090h
sub_1_2858:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ebx + 0xcc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        mov    ebp, 0
        je     loc_1_2886
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
loc_1_2886:
        or     ebp, ebp
        jne    loc_1_28A0
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_28A0
        mov    esi, 0xc000009a
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_28A0:
        mov    eax, ebp
        mov    edx, dword ptr [ebx + 0xc8]
        sub    ecx, ecx
        cmp    dword ptr [ebx + 0xcc], ecx
        jbe    loc_1_28E3
loc_1_28B2:
        mov    esi, dword ptr [ebx + 8]
        or     esi, esi
        jne    loc_1_28C0
        mov    esi, 0xbc44                   ; -> loc_1_BC44
        jmp    loc_1_28CA
loc_1_28C0:
        cmp    esi, 2
        jne    loc_1_28D5
        mov    esi, 0xbc4d                   ; -> loc_1_BC4D
loc_1_28CA:
        mov    edi, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, dword ptr [edx + 4]
        or     dword ptr [eax + 2], esi
loc_1_28D5:
        mov    edx, dword ptr [edx]
        add    eax, 6
        inc    ecx
        cmp    dword ptr [ebx + 0xcc], ecx
        ja     loc_1_28B2
loc_1_28E3:
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        jne    loc_1_2908
        push   0
        push   dword ptr [esp + 0x14]
        push   ebp
        push   0x1010103
        push   1
        push   ebx
        call   sub_1_1728
        add    esp, 0x18
        mov    esi, eax
        jmp    loc_1_292C
        db  08Dh, 049h, 000h
loc_1_2908:
        cmp    eax, 2
        jne    loc_1_2928
        push   0
        push   dword ptr [esp + 0x14]
        push   ebp
        push   0x3010103
        push   1
        push   ebx
        call   sub_1_1728
        add    esp, 0x18
        mov    esi, eax
        jmp    loc_1_292C
loc_1_2928:
        mov    esi, dword ptr [esp + 0x14]
loc_1_292C:
        or     ebp, ebp
        je     loc_1_2939
        push   ebp
        call   sub_1_B944
        add    esp, 4
loc_1_2939:
        cmp    esi, 0xc0010009
        jne    loc_1_295E
        lea    eax, [ebx + 0x64]
        or     dword ptr [eax], 4
        push   0
        push   4
        push   eax
        push   0x1010e
        push   1
        push   ebx
        call   sub_1_1728
        add    esp, 0x18
        mov    esi, eax
loc_1_295E:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_2968:
        mov    edx, dword ptr [esp + 4]
        add    edx, 0xc8
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     loc_1_2989
        mov    ecx, dword ptr [esp + 8]
loc_1_297C:
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_2989
        mov    edx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_297C
loc_1_2989:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], edx
        ret    
sub_1_2990:
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_1_29AA
        cmp    eax, 2
        jne    loc_1_29F6
loc_1_29AA:
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esp + 0x18]
        push   esi
        call   sub_1_2968
        add    esp, 0xc
        or     eax, eax
        je     loc_1_29F4
        mov    ecx, dword ptr [eax + 8]
        dec    ecx
        mov    dword ptr [eax + 8], ecx
        jne    loc_1_29F6
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [edx], ecx
        dec    dword ptr [esi + 0xcc]
        push   eax
        call   sub_1_B944
        add    esp, 4
        push   esi
        call   sub_1_2858
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_29F4:
        sub    edi, edi
loc_1_29F6:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_2A00:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [esp + 0x14]
        cmp    byte ptr [esi + 0x5b], 0
        je     loc_1_2A20
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_2A20:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_1_2A2C
        cmp    eax, 2
        jne    loc_1_2AAA
loc_1_2A2C:
        mov    ebx, dword ptr [esp + 0x18]
        and    ebx, 0xffff7f00
        lea    eax, [esp + 0xc]
        push   eax
        push   ebx
        push   esi
        call   sub_1_2968
        add    esp, 0xc
        or     eax, eax
        je     loc_1_2A58
        inc    dword ptr [eax + 8]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_2A58:
        push   0xc
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        je     loc_1_2AA8
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], 1
        mov    ecx, dword ptr [esi + 0xc8]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0xc8], eax
        inc    dword ptr [esi + 0xcc]
        push   esi
        call   sub_1_2858
        add    esp, 4
        or     eax, eax
        je     loc_1_2AAA
        push   ebx
        push   esi
        call   sub_1_2990
        add    esp, 8
        sub    edi, edi
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_2AA8:
        sub    edi, edi
loc_1_2AAA:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
sub_1_2AB4:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        or     eax, eax
        je     loc_1_2B00
        cmp    eax, 4
        je     loc_1_2B7C
        cmp    eax, 2
        jne    loc_1_2AE0
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0x34], ecx
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_2AE0:
        cmp    eax, 1
        jne    loc_1_2AF8
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x10]
        call   sub_1_2A00
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_1_2AF8:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_2B00:
        cmp    eax, 4
        je     loc_1_2B81
        sub    edi, edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x28], edi
        jne    loc_1_2B28
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0x28], ecx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x2c], ecx
        mov    dword ptr [esi + 0x24], 3
        jmp    loc_1_2B53
        db  090h
loc_1_2B28:
        push   0x10
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        je     loc_1_2B58
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [eax + 4], 3
        mov    ecx, dword ptr [esi + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0x20], eax
loc_1_2B53:
        mov    edi, 1
loc_1_2B58:
        cmp    edi, 1
        jne    loc_1_2B76
        cmp    byte ptr [esi + 0x5b], 0
        jne    loc_1_2B76
        push   1
        push   0
        push   1
        push   dword ptr [esp + 0x20]
        push   esi
        call   sub_1_1B44
        add    esp, 0x14
loc_1_2B76:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_2B7C:
        cmp    eax, 4
        jne    loc_1_2BB6
loc_1_2B81:
        push   0xc
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        je     loc_1_2BB4
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 8], ecx
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esi + 0x30]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0x30], eax
        inc    dword ptr [esi + 0x6c]
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
loc_1_2BB4:
        sub    eax, eax
loc_1_2BB6:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_2BBC:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        or     eax, eax
        jne    loc_1_2C14
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [edi + 0x28], eax
        jne    loc_1_2BE4
        mov    dword ptr [edi + 0x28], 0
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_2BE4:
        add    edi, 0x20
        mov    esi, dword ptr [edi]
        or     esi, esi
        je     loc_1_2C0B
loc_1_2BED:
        cmp    dword ptr [esi + 8], eax
        je     loc_1_2BFA
        mov    edi, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_2BED
loc_1_2BFA:
        or     esi, esi
        je     loc_1_2C0B
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_2C0B:
        pop    edi
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
        pop    esi
        ret    
loc_1_2C14:
        cmp    eax, 4
        jne    loc_1_2C60
        mov    edi, dword ptr [esp + 0xc]
        lea    edx, [edi + 0x30]
        mov    esi, dword ptr [edx]
        or     esi, esi
        je     loc_1_2C54
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x18]
loc_1_2C2E:
        cmp    dword ptr [esi + 4], eax
        jne    loc_1_2C38
        cmp    dword ptr [esi + 8], ecx
        je     loc_1_2C40
loc_1_2C38:
        mov    edx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_2C2E
loc_1_2C40:
        or     esi, esi
        je     loc_1_2C54
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        dec    dword ptr [edi + 0x6c]
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_2C54:
        pop    edi
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_2C60:
        cmp    eax, 1
        jne    loc_1_2C78
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x10]
        call   sub_1_2990
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_1_2C78:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_2C80:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    dword ptr [esp + 0x14], eax
        sub    ebp, ebp
        mov    eax, dword ptr [esp + 0x24]
        add    eax, 0x20
loc_1_2C99:
        mov    ecx, dword ptr [eax + 4]
        or     ecx, ecx
        je     loc_1_2CA4
        dec    ecx
        mov    dword ptr [eax + 4], ecx
loc_1_2CA4:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_2C99
        mov    eax, dword ptr [esp + 0x24]
        mov    ebx, dword ptr [eax + 0x1c]
        mov    dword ptr [esp + 0x18], 0x20
loc_1_2CB9:
        mov    edi, ebx
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_2D70
loc_1_2CC5:
        mov    al, byte ptr [esi + 0x19]
        mov    byte ptr [esp + 0x13], 0
        cmp    al, 2
        mov    ecx, 0x927c0
        je     loc_1_2CDB
        mov    ecx, 0x3e8
loc_1_2CDB:
        cmp    dword ptr [esi + 4], -1
        je     loc_1_2D1F
        mov    edx, dword ptr [esp + 0x14]
        sub    edx, dword ptr [esi + 4]
        cmp    edx, ecx
        ja     loc_1_2CF9
        cmp    al, 2
        jne    loc_1_2D1F
        mov    ecx, dword ptr [esi + 0x14]
        dec    ecx
        mov    dword ptr [esi + 0x14], ecx
        jne    loc_1_2D1F
loc_1_2CF9:
        or     al, al
        je     loc_1_2D38
        mov    eax, dword ptr [esi + 0xc]
        or     eax, eax
        je     loc_1_2D09
        mov    dword ptr [eax + 0x38], ebp
        mov    ebp, eax
loc_1_2D09:
        push   esi
        push   edi
        call   sub_1_2774
        add    esp, 8
        mov    eax, dword ptr [esp + 0x24]
        dec    dword ptr [eax + 0x68]
        mov    byte ptr [esp + 0x13], 1
loc_1_2D1F:
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_2D64
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_B944
        add    esp, 4
        jmp    loc_1_2D68
        db  08Dh, 049h, 000h
loc_1_2D38:
        mov    edi, dword ptr [esi + 8]
        call   sub_1_B92C
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esi + 4], eax
        mov    byte ptr [esi + 0x19], 1
        push   1
        push   0
        push   1
        push   edi
        push   dword ptr [esp + 0x34]
        call   sub_1_1B44
        add    esp, 0x14
        mov    edi, ebx
        mov    esi, dword ptr [ebx]
        jmp    loc_1_2D68
loc_1_2D64:
        mov    edi, esi
        mov    esi, dword ptr [esi]
loc_1_2D68:
        or     esi, esi
        jne    loc_1_2CC5
loc_1_2D70:
        add    ebx, 4
        dec    dword ptr [esp + 0x18]
        jne    loc_1_2CB9
        or     ebp, ebp
        je     loc_1_2D9B
loc_1_2D81:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp + 0x38]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        push   dword ptr [eax]
        call   sub_1_5BD8
        add    esp, 0xc
        or     ebp, ebp
        jne    loc_1_2D81
loc_1_2D9B:
        push   dword ptr [esp + 0x24]
        push   0x2c80                        ; -> loc_1_2C80
        push   0x3e8
        mov    eax, dword ptr [esp + 0x30]
        add    eax, 0x70
        push   eax
        call   sub_1_B920
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
sub_1_2DC4:
        mov    edx, dword ptr [esp + 4]
        add    edx, 0x20
        mov    eax, 4
        mov    ecx, dword ptr [esp + 8]
loc_1_2DD4:
        cmp    dword ptr [edx + 8], ecx
        je     loc_1_2DE0
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_1_2DD4
        ret    
loc_1_2DE0:
        mov    eax, dword ptr [edx + 4]
        ret    
sub_1_2DE4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_2DC4
        add    esp, 8
        cmp    eax, 4
        je     loc_1_2E04
        mov    al, 1
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_2E04:
        mov    edx, dword ptr [esi + 0x30]
        or     edx, edx
        je     loc_1_2E5F
        nop    
loc_1_2E0C:
        mov    esi, dword ptr [edx + 8]
        mov    eax, esi
        and    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [edx + 4], eax
        jne    loc_1_2E59
        cmp    esi, -1
        je     loc_1_2E64
        cmp    eax, dword ptr [ebp + 0xc]
        je     loc_1_2E59
        mov    ecx, esi
        not    ecx
        or     ecx, eax
        cmp    ecx, dword ptr [ebp + 0xc]
        je     loc_1_2E59
        mov    al, byte ptr [ebp + 0xc]
        shr    al, 2
        and    eax, 0xff
        and    eax, 0xfffffffc
        mov    eax, dword ptr [eax + 0xbd00] ; -> loc_1_BD00
        cmp    eax, esi
        je     loc_1_2E6C
        mov    ecx, eax
        and    ecx, dword ptr [ebp + 0xc]
        cmp    ecx, dword ptr [ebp + 0xc]
        je     loc_1_2E59
        not    eax
        or     eax, ecx
        cmp    eax, dword ptr [ebp + 0xc]
        jne    loc_1_2E6C
loc_1_2E59:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_1_2E0C
loc_1_2E5F:
        sub    al, al
        pop    esi
        leave  
        ret    
loc_1_2E64:
        pop    esi
        mov    al, 1
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_2E6C:
        pop    esi
        mov    al, 1
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_2E74:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        sub    edi, edi
        inc    esi
        mov    esi, dword ptr [esp + 0x38]
loc_1_2E84:
        mov    ecx, dword ptr [esi + 0x18]
        and    ecx, 0xff
        cmp    ecx, 0x63
        jbe    loc_1_2EB0
        mov    ebp, 0x64
        mov    eax, ecx
        sub    edx, edx
        div    ebp
        add    al, 0x30
        mov    byte ptr [esp + ebx + 0x10], al
        inc    ebx
        mov    eax, ecx
        sub    edx, edx
        div    ebp
        mov    ecx, edx
        jmp    loc_1_2EB5
        db  08Bh, 0FFh
loc_1_2EB0:
        cmp    ecx, 9
        jbe    loc_1_2ECF
loc_1_2EB5:
        mov    eax, ecx
        mov    ebp, 0xa
        sub    edx, edx
        div    ebp
        add    al, 0x30
        mov    byte ptr [esp + ebx + 0x10], al
        inc    ebx
        mov    eax, ecx
        sub    edx, edx
        div    ebp
        mov    ecx, edx
loc_1_2ECF:
        inc    ebx
        add    cl, 0x30
        mov    byte ptr [esp + ebx + 0xf], cl
        cmp    edi, 3
        je     loc_1_2EE2
        mov    byte ptr [esp + ebx + 0x10], 0x2e
        inc    ebx
loc_1_2EE2:
        inc    edi
        shr    dword ptr [esi + 0x18], 8
        cmp    edi, 4
        jb     loc_1_2E84
        mov    byte ptr [esp + ebx + 0x10], 0
        sub    ecx, ecx
        cmp    dword ptr [esi + 0x1c], ecx
        jbe    loc_1_2F45
        lea    eax, [esp + 0x20]
loc_1_2EFC:
        mov    dl, byte ptr [esi + ecx + 0x20]
        shr    dl, 4
        cmp    dl, 0xa
        jb     loc_1_2F10
        add    dl, 0x37
        jmp    loc_1_2F13
        db  08Dh, 049h, 000h
loc_1_2F10:
        add    dl, 0x30
loc_1_2F13:
        mov    byte ptr [eax], dl
        mov    dl, byte ptr [esi + ecx + 0x20]
        and    dl, 0xf
        cmp    dl, 0xa
        jb     loc_1_2F28
        add    dl, 0x37
        jmp    loc_1_2F2B
        db  08Bh, 0FFh
loc_1_2F28:
        add    dl, 0x30
loc_1_2F2B:
        mov    byte ptr [eax + 1], dl
        mov    edx, dword ptr [esi + 0x1c]
        sub    edx, ecx
        cmp    edx, 1
        je     loc_1_2F3C
        mov    byte ptr [eax + 2], 0x3a
loc_1_2F3C:
        inc    ecx
        add    eax, 3
        cmp    dword ptr [esi + 0x1c], ecx
        ja     loc_1_2EFC
loc_1_2F45:
        mov    eax, dword ptr [esi + 0x1c]
        lea    eax, [eax + eax*2]
        mov    byte ptr [esp + eax + 0x1f], 0
        push   dword ptr [esi + 0x14]
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_0247
        add    esp, 0xc
        push   esi
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Bh, 0FFh
sub_1_2F78:
        sub    esp, 0x3c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0
        call   sub_1_B92C
        mov    dword ptr [esp + 0x24], eax
        mov    ebx, dword ptr [esp + 0x50]
        mov    ecx, dword ptr [ebx + 8]
        cmp    ecx, 7
        je     loc_1_3048
        cmp    dword ptr [esp + 0x60], 0x1c
        jae    loc_1_2FBC
        mov    eax, 0x10001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
loc_1_2FBC:
        mov    ebp, dword ptr [esp + 0x5c]
        mov    ax, word ptr [ebp]
        cmp    ax, 0x100
        je     loc_1_2FE0
        cmp    ax, 0x600
        je     loc_1_2FE0
        mov    eax, 0x10001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  08Dh, 049h, 000h
loc_1_2FE0:
        cmp    byte ptr [ebp + 4], 6
        je     loc_1_2FF4
        mov    eax, 0x10001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  090h
loc_1_2FF4:
        or     ecx, ecx
        jne    loc_1_3008
        cmp    byte ptr [ebx + 0x61], 0
        jne    loc_1_3008
        mov    dword ptr [esp + 0x2c], 0
        jmp    loc_1_3014
loc_1_3008:
        cmp    dword ptr [esp + 0x64], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 0x2c], eax
loc_1_3014:
        cmp    ecx, 1
        jne    loc_1_3034
        mov    eax, dword ptr [esp + 0x58]
        sub    eax, 0xe
        cmp    eax, 2
        jbe    loc_1_3034
        mov    dword ptr [esp + 0x20], eax
        mov    ecx, dword ptr [esp + 0x54]
        add    ecx, 0xe
        mov    dword ptr [esp + 0x1c], ecx
loc_1_3034:
        lea    ecx, [ebp + 8]
        mov    dword ptr [esp + 0x14], ecx
        lea    ecx, [ebp + 0xe]
        mov    dword ptr [esp + 0x18], ecx
        lea    eax, [ebp + 0x18]
        jmp    loc_1_30A5
        db  090h
loc_1_3048:
        cmp    dword ptr [esp + 0x60], 0x12
        jae    loc_1_305C
        mov    eax, 0x10001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
loc_1_305C:
        mov    ebp, dword ptr [esp + 0x5c]
        cmp    word ptr [ebp], 0x700
        je     loc_1_3078
        mov    eax, 0x10001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  08Dh, 049h, 000h
loc_1_3078:
        cmp    byte ptr [ebp + 4], 1
        je     loc_1_308C
        mov    eax, 0x10001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  090h
loc_1_308C:
        lea    ecx, [ebp + 8]
        mov    dword ptr [esp + 0x2c], 0
        mov    dword ptr [esp + 0x14], ecx
        inc    ecx
        mov    dword ptr [esp + 0x18], ecx
        mov    eax, ecx
        add    eax, 5
loc_1_30A5:
        mov    dword ptr [esp + 0x28], eax
        cmp    word ptr [ebp + 2], 8
        je     loc_1_30C0
        mov    eax, 0x10001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  08Dh, 049h, 000h
loc_1_30C0:
        cmp    byte ptr [ebp + 5], 4
        je     loc_1_30D4
        mov    eax, 0x10001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  090h
loc_1_30D4:
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [ecx]
        or     eax, eax
        jne    loc_1_30EC
        mov    eax, 0x10001
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  090h
loc_1_30EC:
        push   eax
        push   ebx
        call   sub_1_2DC4
        add    esp, 8
        cmp    eax, 4
        je     loc_1_320C
        mov    cl, byte ptr [ebx + 0x5c]
        cmp    byte ptr [ebp + 4], cl
        jne    loc_1_3127
        lea    edi, [ebx + 0x54]
        movzx  edx, cl
        mov    ecx, edx
        shr    ecx, 2
        mov    esi, dword ptr [esp + 0x14]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_3121
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_1_3121:
        je     loc_1_31FC
loc_1_3127:
        or     eax, eax
        je     loc_1_315C
        cmp    word ptr [ebp + 6], 0x200
        jne    loc_1_315C
        mov    byte ptr [ebx + 0x5b], 2
        mov    byte ptr [ebx + 0x96], 2
        push   0
        push   dword ptr [esp + 0x18]
        push   1
        mov    ecx, dword ptr [esp + 0x24]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_1B44
        add    esp, 0x14
        mov    esi, 1
        jmp    loc_1_319F
loc_1_315C:
        cmp    word ptr [ebp + 6], 0x100
        jne    loc_1_319D
        mov    eax, dword ptr [esp + 0x28]
        push   dword ptr [eax]
        push   ebx
        call   sub_1_2DC4
        add    esp, 8
        cmp    eax, 4
        je     loc_1_319D
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x38]
        push   dword ptr [eax]
        mov    ecx, dword ptr [esp + 0x2c]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_1DEC
        add    esp, 0x1c
loc_1_319D:
        sub    esi, esi
loc_1_319F:
        movzx  eax, byte ptr [ebp + 4]
        add    eax, 0x20
        push   eax
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        je     loc_1_31FC
        mov    ecx, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0x14], esi
        movzx  edx, byte ptr [ebp + 4]
        mov    dword ptr [eax + 0x1c], edx
        lea    edi, [eax + 0x20]
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [eax + 0xc], 0
        mov    dword ptr [eax + 4], 0x231    ; -> loc_1_0231
        mov    ecx, dword ptr [0xbf6c]       ; -> loc_1_BF6C
        mov    dword ptr [eax + 0x10], ecx
        push   eax
        push   eax
        call   sub_1_B932
        add    esp, 8
loc_1_31FC:
        pop    ebp
        mov    eax, 0x10001
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  08Dh, 049h, 000h
loc_1_320C:
        mov    cx, word ptr [ebx + 0x94]
        mov    word ptr [esp + 0x12], cx
        mov    eax, dword ptr [esp + 0x28]
        push   dword ptr [eax]
        push   ebx
        call   sub_1_2DE4
        add    esp, 8
        mov    byte ptr [esp + 0x10], al
        lea    eax, [esp + 0x34]
        push   eax
        mov    ecx, dword ptr [esp + 0x1c]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_2038
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_3270
        cmp    byte ptr [esp + 0x10], 0
        je     loc_1_3260
        lea    eax, [esp + 0x34]
        push   eax
        mov    ecx, dword ptr [esp + 0x1c]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_2540
        add    esp, 0xc
        jmp    loc_1_3278
loc_1_3260:
        pop    ebp
        mov    eax, 0x10001
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  08Dh, 049h, 000h
loc_1_3270:
        mov    ecx, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x34], ecx
loc_1_3278:
        or     eax, eax
        je     loc_1_3465
        mov    edx, dword ptr [eax + 4]
        cmp    edx, -1
        jne    loc_1_3290
        mov    dword ptr [esp + 0x24], 0xffffffff
loc_1_3290:
        mov    ecx, dword ptr [ebx + 8]
        or     ecx, ecx
        je     loc_1_32B4
        cmp    ecx, 1
        je     loc_1_32FC
        cmp    ecx, 2
        je     loc_1_33E0
        cmp    ecx, 7
        je     loc_1_3410
        jmp    loc_1_3434
        db  090h
loc_1_32B4:
        lea    ecx, [eax + 0x1a]
        mov    edi, ecx
        mov    esi, dword ptr [esp + 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebx + 0x54]
        lea    edi, [ecx + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        cmp    dword ptr [esp + 0x2c], 0
        mov    word ptr [ecx + 0xc], 8
        je     loc_1_32F0
        lea    edi, [eax + 0x28]
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [eax + 0x18], 0x16
        mov    word ptr [eax + 0x2e], 8
        jmp    loc_1_3429
        db  090h
loc_1_32F0:
        mov    byte ptr [eax + 0x18], 0xe
        jmp    loc_1_3429
        db  08Dh, 049h, 000h
loc_1_32FC:
        cmp    byte ptr [eax + 0x19], 2
        jne    loc_1_3314
        mov    ecx, dword ptr [esp + 0x24]
        sub    ecx, edx
        cmp    ecx, 0x3e8
        jbe    loc_1_3434
loc_1_3314:
        lea    edx, [eax + 0x1a]
        mov    byte ptr [edx], 0x10
        mov    byte ptr [edx + 1], 0x40
        lea    edi, [edx + 2]
        mov    esi, dword ptr [esp + 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebx + 0x54]
        lea    edi, [edx + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_33AB
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x3c], ecx
        lea    edi, [eax + 0x28]
        mov    esi, dword ptr [esp + 0x1c]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x3c]
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    cl, byte ptr [ecx + 1]
        and    cl, 0x70
        shr    cl, 4
        mov    byte ptr [esp + 0x30], cl
        sub    byte ptr [esp + 0x30], 4
        mov    cl, byte ptr [esp + 0x38]
        sbb    byte ptr [esp + 0x38], cl
        mov    cl, byte ptr [esp + 0x30]
        and    cl, byte ptr [esp + 0x38]
        mov    byte ptr [esp + 0x30], cl
        add    cl, 4
        mov    byte ptr [esp + 0x11], cl
        and    ecx, 0xff
        mov    cx, word ptr [ecx*2 + 0xbc88] ; -> loc_1_BC88
        or     byte ptr [edx + 8], 0x80
        mov    word ptr [esp + 0x12], cx
        xor    byte ptr [eax + 0x29], 0x80
        sub    word ptr [esp + 0x12], 8
        and    byte ptr [eax + 0x28], 0x1f
loc_1_33AB:
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        mov    ecx, dword ptr [esp + 0x20]
        lea    edi, [ecx + eax + 0x28]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [eax + 0x19], 2
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 0x20]
        add    cl, 0x16
        mov    byte ptr [eax + 0x18], cl
        and    ecx, 0xff
        mov    word ptr [ecx + eax + 0x18], 8
        jmp    loc_1_3434
        db  08Bh, 0FFh
loc_1_33E0:
        lea    ecx, [eax + 0x1a]
        mov    byte ptr [ecx], 0x57
        lea    edi, [ecx + 1]
        mov    esi, dword ptr [esp + 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebx + 0x54]
        lea    edi, [ecx + 7]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        lea    edi, [eax + 0x27]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [eax + 0x18], 0x15
        mov    word ptr [eax + 0x2d], 8
        jmp    loc_1_3429
        db  090h
loc_1_3410:
        lea    edx, [eax + 0x1a]
        mov    cl, byte ptr [ebx + 0x54]
        mov    byte ptr [edx], cl
        mov    ecx, dword ptr [esp + 0x14]
        mov    cl, byte ptr [ecx]
        mov    byte ptr [edx + 1], cl
        mov    byte ptr [edx + 2], 0xd4
        mov    byte ptr [eax + 0x18], 3
loc_1_3429:
        mov    byte ptr [eax + 0x19], 2
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [eax + 4], ecx
loc_1_3434:
        mov    esi, dword ptr [eax + 0xc]
        or     esi, esi
        je     loc_1_3465
        mov    dword ptr [eax + 0xc], 0
        push   dword ptr [esp + 0x34]
        push   eax
        push   esi
        push   ebx
        call   sub_1_2394
        add    esp, 0x10
        cmp    eax, 0x103
        je     loc_1_3465
        push   0
        push   esi
        push   dword ptr [ebx]
        call   sub_1_5BD8
        add    esp, 0xc
loc_1_3465:
        mov    cx, word ptr [esp + 0x12]
        cmp    word ptr [ebx + 0x94], cx
        jbe    loc_1_3497
        movzx  eax, cx
        mov    dword ptr [esp + 0x44], eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esp + 0x48], eax
        push   8
        lea    eax, [esp + 0x48]
        push   eax
        push   3
        push   dword ptr [ebx]
        call   sub_1_6DF4
        add    esp, 0x10
loc_1_3497:
        cmp    byte ptr [esp + 0x10], 0
        je     loc_1_34CB
        cmp    word ptr [ebp + 6], 0x100
        jne    loc_1_34CB
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x38]
        push   dword ptr [eax]
        mov    ecx, dword ptr [esp + 0x2c]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_1DEC
        add    esp, 0x1c
loc_1_34CB:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    
        db  08Dh, 049h, 000h
sub_1_34D8:
        push   ebx
        push   esi
        push   edi
        push   0
        push   4
        push   0xbc08                        ; -> obj1:0xBC08
        push   0x1010f
        push   1
        mov    edi, dword ptr [esp + 0x24]
        push   edi
        call   sub_1_1728
        add    esp, 0x18
        or     eax, eax
        je     loc_1_3504
        mov    byte ptr [edi + 0x5b], 2
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_3504:
        push   0
        push   4
        lea    eax, [edi + 0x64]
        push   eax
        push   0x1010e
        push   1
        push   edi
        call   sub_1_1728
        add    esp, 0x18
        or     eax, eax
        jne    loc_1_3578
        mov    byte ptr [edi + 0x96], 1
        mov    byte ptr [edi + 0x97], 1
        mov    byte ptr [edi + 0x5b], 0
        sub    ebx, ebx
        lea    esi, [edi + 0x20]
        mov    ecx, esi
loc_1_3539:
        mov    eax, dword ptr [ecx + 8]
        or     eax, eax
        je     loc_1_3559
        mov    dword ptr [ecx + 4], 3
        mov    ebx, ecx
        push   1
        push   0
        push   1
        push   eax
        push   edi
        call   sub_1_1B44
        add    esp, 0x14
loc_1_3559:
        mov    ecx, esi
        cmp    ebx, esi
        je     loc_1_356A
        nop    
loc_1_3560:
        or     ecx, ecx
        je     loc_1_3570
        mov    ecx, dword ptr [ecx]
        cmp    ecx, ebx
        jne    loc_1_3560
loc_1_356A:
        or     ecx, ecx
        je     loc_1_3574
        mov    ecx, dword ptr [ecx]
loc_1_3570:
        or     ecx, ecx
        jne    loc_1_3539
loc_1_3574:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_3578:
        mov    byte ptr [edi + 0x5b], 2
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_3580:
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x88
        push   eax
        call   sub_1_B95C
        add    esp, 8
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_359C:
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x88
        push   eax
        call   sub_1_B95C
        add    esp, 8
        ret    8
        db  08Dh, 049h, 000h
loc_1_35B8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        lea    ebp, [edi + 0x38]
        mov    esi, dword ptr [esp + 0x18]
        dec    dword ptr [esi + 0x50]
        mov    ebx, dword ptr [esp + 0x20]
        or     ebx, ebx
        jne    loc_1_35E0
        mov    eax, dword ptr [edi + 4]
        add    dword ptr [esi + 0x44], eax
        jmp    loc_1_35F6
        db  08Dh, 049h, 000h
loc_1_35E0:
        cmp    ebx, 0xc000009a
        jne    loc_1_35F0
        inc    dword ptr [esi + 0xac]
        jmp    loc_1_35F6
loc_1_35F0:
        inc    dword ptr [esi + 0xb0]
loc_1_35F6:
        lea    eax, [esp + 0x10]
        push   eax
        push   edi
        call   sub_1_BA5C
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_17BC
        add    esp, 8
        cmp    byte ptr [ebp + 4], 0
        je     loc_1_362C
        push   ebx
        push   edi
        push   dword ptr [esi]
        call   sub_1_5BD8
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0xc
        db  08Bh, 0FFh
loc_1_362C:
        push   edi
        call   sub_1_BA1A
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0xc
loc_1_363C:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax]
        call   sub_1_5840
        add    esp, 0x10
        ret    0x10
        db  08Dh, 049h, 000h
loc_1_365C:
        ret    8
        db  090h
loc_1_3660:
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x88
        push   eax
        call   sub_1_B95C
        add    esp, 8
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_367C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        cmp    byte ptr [eax + 0x5b], 0
        jne    loc_1_37B8
        movzx  ecx, byte ptr [eax + 0x60]
        cmp    ecx, dword ptr [esp + 0x20]
        ja     loc_1_37B8
        mov    esi, dword ptr [esp + 0x2c]
        add    dword ptr [eax + 0x38], esi
        mov    ecx, dword ptr [eax + 8]
        cmp    ecx, 7
        je     loc_1_3708
        or     ecx, ecx
        mov    ebp, dword ptr [esp + 0x1c]
        jne    loc_1_36C8
        mov    cx, word ptr [ebp + 0xc]
        mov    bh, cl
        mov    bl, ch
        cmp    bx, 0x600
        jb     loc_1_36C8
        sub    edi, edi
        jmp    loc_1_372B
        db  08Bh, 0FFh
loc_1_36C8:
        cmp    dword ptr [esp + 0x28], 8
        jb     loc_1_36F4
        mov    ecx, dword ptr [esp + 0x24]
        cmp    byte ptr [ecx], 0xaa
        jne    loc_1_36F4
        cmp    byte ptr [ecx + 1], 0xaa
        jne    loc_1_36F4
        cmp    byte ptr [ecx + 2], 3
        jne    loc_1_36F4
        mov    dx, word ptr [ecx + 6]
        mov    bh, dl
        mov    bl, dh
        mov    edi, 8
        jmp    loc_1_372F
        db  090h
loc_1_36F4:
        pop    ebp
        inc    dword ptr [eax + 0xa8]
        mov    eax, 0x10001
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
        db  08Bh, 0FFh
loc_1_3708:
        sub    edi, edi
        mov    ebp, dword ptr [esp + 0x1c]
        mov    cl, byte ptr [ebp + 2]
        cmp    cl, 0xd4
        jne    loc_1_371C
        mov    bx, 0x800
        jmp    loc_1_372B
loc_1_371C:
        cmp    cl, 0xd5
        jne    loc_1_3728
        mov    bx, 0x806
        jmp    loc_1_372B
        db  090h
loc_1_3728:
        sub    bx, bx
loc_1_372B:
        mov    ecx, dword ptr [esp + 0x24]
loc_1_372F:
        movzx  edx, byte ptr [eax + 0x5f]
        mov    dl, byte ptr [edx + ebp]
        and    dl, byte ptr [eax + 0x5d]
        cmp    dl, byte ptr [eax + 0x5e]
        mov    edx, 0
        sete   dl
        cmp    bx, 0x800
        jne    loc_1_3778
        inc    dword ptr [eax + edx*4 + 0x3c]
        push   edx
        push   edi
        push   dword ptr [esp + 0x20]
        sub    esi, edi
        push   esi
        mov    edx, dword ptr [esp + 0x38]
        sub    edx, edi
        push   edx
        add    ecx, edi
        push   ecx
        push   dword ptr [eax]
        call   sub_1_5514
        add    esp, 0x1c
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
        db  08Dh, 049h, 000h
loc_1_3778:
        cmp    bx, 0x806
        jne    loc_1_37A4
        inc    dword ptr [eax + edx*4 + 0x3c]
        push   edi
        mov    edx, dword ptr [esp + 0x2c]
        sub    edx, edi
        push   edx
        add    ecx, edi
        push   ecx
        push   dword ptr [esp + 0x2c]
        push   ebp
        push   eax
        call   sub_1_2F78
        add    esp, 0x18
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
        db  090h
loc_1_37A4:
        pop    ebp
        inc    dword ptr [eax + 0xa8]
        mov    eax, 0x10001
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
        db  08Bh, 0FFh
loc_1_37B8:
        pop    ebp
        mov    eax, 0x10001
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
loc_1_37C4:
        call   sub_1_4BCC
        ret    4
loc_1_37CC:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax]
        call   sub_1_6DF4
        add    esp, 0x10
        ret    0x10
        db  08Dh, 049h, 000h
loc_1_37EC:
        ret    4
        db  090h
sub_1_37F0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [esp + 0x10], eax
        mov    edx, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [edx + 4]
        mov    eax, dword ptr [ecx + 0xb8]
        mov    ebx, dword ptr [esp + 0x20]
        mov    dword ptr [ebx], eax
        movzx  eax, byte ptr [ecx + 0x5c]
        mov    dword ptr [ebx + 4], eax
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        je     loc_1_3838
        cmp    ecx, 1
        je     loc_1_383C
        cmp    ecx, 2
        je     loc_1_3844
        cmp    ecx, 7
        je     loc_1_3844
        sub    ecx, ecx
        jmp    loc_1_3849
        db  08Bh, 0FFh
loc_1_3838:
        sub    ecx, ecx
        jmp    loc_1_3849
loc_1_383C:
        mov    ecx, 2
        jmp    loc_1_3849
        db  090h
loc_1_3844:
        mov    ecx, 1
loc_1_3849:
        lea    edi, [ebx + 8]
        lea    esi, [ebp + ecx + 0x1a]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x10], eax
        cmp    byte ptr [ebp + 0x19], 2
        jne    loc_1_3880
        mov    eax, 0
        cmp    dword ptr [ebp + 4], -1
        sete   al
        add    eax, 3
        mov    dword ptr [ebx + 0x14], eax
        jmp    loc_1_3887
        db  08Bh, 0FFh
loc_1_3880:
        mov    dword ptr [ebx + 0x14], 2
loc_1_3887:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     loc_1_38A0
        mov    dword ptr [edx + 4], eax
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_38A0:
        mov    ecx, dword ptr [edx]
        inc    ecx
        cmp    ecx, 0x20
        jae    loc_1_38BE
        mov    eax, dword ptr [esp + 0x10]
        lea    eax, [eax + ecx*4]
loc_1_38B0:
        cmp    dword ptr [eax], 0
        jne    loc_1_38D8
        add    eax, 4
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_1_38B0
loc_1_38BE:
        sub    eax, eax
        mov    dword ptr [edx], 0
        mov    dword ptr [edx + 4], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_38D8:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + ecx*4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx], ecx
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_38F4:
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax + 0x1c]
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    loc_1_3940
        or     eax, eax
        jne    loc_1_3940
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 1
        lea    eax, [ecx + esi*4]
loc_1_391A:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jne    loc_1_392D
        add    eax, 4
        inc    esi
        cmp    esi, 0x20
        jb     loc_1_391A
        or     ecx, ecx
        je     loc_1_393C
loc_1_392D:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_393C:
        sub    eax, eax
        pop    esi
        ret    
loc_1_3940:
        cmp    esi, 0x20
        jae    loc_1_3956
        mov    ecx, dword ptr [ecx + esi*4]
        or     ecx, ecx
        je     loc_1_3956
loc_1_394C:
        cmp    ecx, eax
        je     loc_1_3964
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_394C
loc_1_3956:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        ret    
loc_1_3964:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 1
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_3978:
        sub    esp, 0x68
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        mov    ecx, dword ptr [esp + 0x88]
        mov    ebp, dword ptr [ecx]
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x80]
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        cmp    edx, 0x280
        mov    ebx, dword ptr [esp + 0x7c]
        jne    loc_1_39B2
        cmp    dword ptr [ebx + 0xbc], eax
        je     loc_1_39CA
loc_1_39B2:
        cmp    edx, 0x200
        jne    loc_1_3E48
        cmp    dword ptr [ebx + 0xc0], eax
        jne    loc_1_3E48
loc_1_39CA:
        mov    ecx, dword ptr [esp + 0x88]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [esi + 0xc], 0x100
        je     loc_1_39F0
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_39F0:
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 0x100
        jne    loc_1_3A64
        cmp    dword ptr [esi + 0x10], 1
        jne    loc_1_3A54
        cmp    ebp, 4
        jb     loc_1_3A44
        sub    edx, 0x280
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 0x7e
        add    eax, 0x202
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x10]
        push   eax
        push   4
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x90]
        call   sub_1_AAE0
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3A44:
        pop    ebp
        mov    eax, 0x21
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3A54:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3A64:
        cmp    edx, 0x280
        jne    loc_1_3C88
        cmp    eax, 0x300
        mov    eax, dword ptr [esi + 0x10]
        jne    loc_1_3B78
        cmp    eax, 1
        jne    loc_1_3ABC
        cmp    ebp, 4
        jb     loc_1_3AAC
        lea    eax, [esp + 0x10]
        push   eax
        push   4
        add    ebx, 0x6c
        push   ebx
        push   dword ptr [esp + 0x90]
        call   sub_1_AAE0
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
loc_1_3AAC:
        pop    ebp
        mov    eax, 0x21
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3ABC:
        cmp    eax, 0x101
        je     loc_1_3AD0
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
loc_1_3AD0:
        mov    eax, dword ptr [esp + 0x8c]
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     loc_1_3B00
        mov    eax, dword ptr [ebx + 0x30]
        or     eax, eax
        je     loc_1_3AF2
loc_1_3AE4:
        cmp    eax, esi
        je     loc_1_3AEE
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_3AE4
loc_1_3AEE:
        or     eax, eax
        jne    loc_1_3B03
loc_1_3AF2:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  090h
loc_1_3B00:
        mov    esi, dword ptr [ebx + 0x30]
loc_1_3B03:
        or     esi, esi
        je     loc_1_3B52
        nop    
loc_1_3B08:
        mov    eax, ebp
        sub    eax, edi
        cmp    eax, 0xc
        jl     loc_1_3B52
        mov    dword ptr [esp + 0x18], 1
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 0x20], eax
        add    edi, 0xc
        lea    eax, [esp + 0x10]
        push   eax
        push   0xc
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x90]
        call   sub_1_AAE0
        add    esp, 0x10
        mov    dword ptr [esp + 0x84], eax
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_3B08
loc_1_3B52:
        cmp    esi, 1
        mov    ecx, dword ptr [esp + 0x88]
        mov    dword ptr [ecx], edi
        mov    eax, dword ptr [esp + 0x8c]
        mov    dword ptr [eax], esi
        cmc    
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3B78:
        cmp    eax, 1
        jne    loc_1_3BD8
        cmp    ebp, 8
        jb     loc_1_3BC8
        mov    ecx, dword ptr [esp + 0x88]
        mov    dword ptr [ecx], 8
        mov    eax, dword ptr [ebx + 0x68]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [ebx + 0xb8]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 0x10]
        push   eax
        push   8
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x90]
        call   sub_1_AAE0
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3BC8:
        pop    ebp
        mov    eax, 0x21
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3BD8:
        cmp    eax, 0x101
        jne    loc_1_3C78
        lea    eax, [esp + 0x14]
        push   eax
        push   ebx
        push   dword ptr [esp + 0x94]
        call   sub_1_38F4
        add    esp, 0xc
        mov    esi, eax
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_3C10
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Bh, 0FFh
loc_1_3C10:
        or     esi, esi
        je     loc_1_3C5D
loc_1_3C14:
        mov    eax, ebp
        sub    eax, edi
        cmp    eax, 0x18
        jl     loc_1_3C5D
        lea    eax, [esp + 0x18]
        push   eax
        push   ebx
        push   dword ptr [esp + 0x94]
        call   sub_1_37F0
        add    esp, 0xc
        mov    esi, eax
        add    edi, 0x18
        lea    eax, [esp + 0x10]
        push   eax
        push   0x18
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x90]
        call   sub_1_AAE0
        add    esp, 0x10
        mov    dword ptr [esp + 0x84], eax
        or     esi, esi
        jne    loc_1_3C14
loc_1_3C5D:
        cmp    esi, 1
        mov    ecx, dword ptr [esp + 0x88]
        mov    dword ptr [ecx], edi
        cmc    
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  090h
loc_1_3C78:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3C88:
        cmp    eax, 0x200
        je     loc_1_3C9C
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
loc_1_3C9C:
        cmp    dword ptr [esi + 0x10], 1
        jne    loc_1_3E38
        cmp    ebp, 0x5c
        jae    loc_1_3CB8
        mov    eax, 0x21
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
loc_1_3CB8:
        mov    eax, dword ptr [ebx + 0xb8]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     loc_1_3CE0
        cmp    eax, 1
        je     loc_1_3CEC
        cmp    eax, 2
        je     loc_1_3CF8
        mov    dword ptr [esp + 0x1c], 1
        jmp    loc_1_3D00
        db  08Dh, 049h, 000h
loc_1_3CE0:
        mov    dword ptr [esp + 0x1c], 6
        jmp    loc_1_3D00
        db  08Bh, 0FFh
loc_1_3CEC:
        mov    dword ptr [esp + 0x1c], 9
        jmp    loc_1_3D00
        db  08Bh, 0FFh
loc_1_3CF8:
        mov    dword ptr [esp + 0x1c], 0xf
loc_1_3D00:
        movzx  eax, word ptr [ebx + 0x94]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebx + 0x98]
        mov    dword ptr [esp + 0x24], eax
        movzx  eax, byte ptr [ebx + 0x5c]
        mov    dword ptr [esp + 0x28], eax
        lea    esi, [ebx + 0x54]
        lea    edi, [esp + 0x2c]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  eax, byte ptr [ebx + 0x96]
        mov    dword ptr [esp + 0x34], eax
        movzx  eax, byte ptr [ebx + 0x97]
        mov    dword ptr [esp + 0x38], eax
        mov    eax, dword ptr [ebx + 0x9c]
        mov    dword ptr [esp + 0x3c], eax
        mov    eax, dword ptr [ebx + 0x38]
        mov    dword ptr [esp + 0x40], eax
        mov    eax, dword ptr [ebx + 0x3c]
        mov    dword ptr [esp + 0x44], eax
        mov    eax, dword ptr [ebx + 0x40]
        mov    dword ptr [esp + 0x48], eax
        mov    eax, dword ptr [ebx + 0xa0]
        mov    dword ptr [esp + 0x4c], eax
        mov    eax, dword ptr [ebx + 0xa4]
        mov    dword ptr [esp + 0x50], eax
        mov    eax, dword ptr [ebx + 0xa8]
        mov    dword ptr [esp + 0x54], eax
        mov    eax, dword ptr [ebx + 0x44]
        mov    dword ptr [esp + 0x58], eax
        mov    eax, dword ptr [ebx + 0x48]
        mov    dword ptr [esp + 0x5c], eax
        mov    eax, dword ptr [ebx + 0x4c]
        mov    dword ptr [esp + 0x60], eax
        mov    eax, dword ptr [ebx + 0xac]
        mov    dword ptr [esp + 0x64], eax
        mov    eax, dword ptr [ebx + 0xb0]
        mov    dword ptr [esp + 0x68], eax
        mov    eax, dword ptr [ebx + 0x50]
        mov    dword ptr [esp + 0x6c], eax
        mov    eax, dword ptr [ebx + 0xb4]
        mov    dword ptr [esp + 0x70], eax
        lea    eax, [esp + 0x10]
        push   eax
        push   0x5c
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x90]
        call   sub_1_AAE0
        add    esp, 0x10
        mov    edx, dword ptr [ebx + 0xb4]
        lea    ecx, [edx + 0x5c]
        cmp    ecx, ebp
        ja     loc_1_3E1C
        or     edx, edx
        je     loc_1_3E00
        lea    ecx, [esp + 0x10]
        push   ecx
        push   edx
        push   dword ptr [ebx + 0xc4]
        push   eax
        call   sub_1_AAE0
        add    esp, 0x10
loc_1_3E00:
        mov    eax, dword ptr [ebx + 0xb4]
        add    eax, 0x5c
        mov    ecx, dword ptr [esp + 0x88]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
loc_1_3E1C:
        mov    eax, 9
        mov    ecx, dword ptr [esp + 0x88]
        mov    dword ptr [ecx], 0x5c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Bh, 0FFh
loc_1_3E38:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3E48:
        pop    ebp
        mov    eax, 0xfe
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x68
        ret    
        db  08Dh, 049h, 000h
loc_1_3E58:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 4]
        cmp    eax, 0x280
        mov    ebx, dword ptr [esp + 0x1c]
        jne    loc_1_3E7B
        cmp    dword ptr [ebx + 0xbc], ecx
        je     loc_1_3E92
loc_1_3E7B:
        cmp    eax, 0x200
        jne    loc_1_4274
        cmp    dword ptr [ebx + 0xc0], ecx
        jne    loc_1_4274
loc_1_3E92:
        cmp    dword ptr [edx + 0xc], 0x100
        je     loc_1_3EA8
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_3EA8:
        cmp    eax, 0x200
        jne    loc_1_3F60
        cmp    dword ptr [edx + 8], 0x200
        je     loc_1_3ECC
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_3ECC:
        cmp    dword ptr [edx + 0x10], 1
        jne    loc_1_3F50
        cmp    dword ptr [esp + 0x28], 0x5c
        jb     loc_1_3F40
        mov    edx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [edx + 0x1c]
        cmp    eax, 1
        je     loc_1_3EFC
        cmp    eax, 2
        je     loc_1_3F1C
        cmp    eax, 3
        je     loc_1_3F34
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_3EFC:
        mov    byte ptr [ebx + 0x96], 1
        cmp    byte ptr [ebx + 0x97], 1
        jne    loc_1_3F10
        mov    byte ptr [ebx + 0x5b], 0
loc_1_3F10:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_3F1C:
        sub    eax, eax
        mov    byte ptr [ebx + 0x96], 2
        mov    byte ptr [ebx + 0x5b], 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_3F34:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_3F40:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_3F50:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_3F60:
        mov    eax, dword ptr [edx + 8]
        cmp    eax, 0x300
        jne    loc_1_409C
        cmp    dword ptr [edx + 0x10], 0x101
        jne    loc_1_408C
        cmp    dword ptr [esp + 0x28], 0xc
        jb     loc_1_408C
        mov    edx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esp + 0x10], eax
        mov    esi, dword ptr [edx + 8]
        mov    eax, dword ptr [edx]
        cmp    eax, 1
        jne    loc_1_4058
        mov    eax, esi
        and    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x10], eax
        jne    loc_1_4048
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_4048
        cmp    dword ptr [esp + 0x10], -1
        je     loc_1_4048
        mov    cl, byte ptr [esp + 0x10]
        and    cl, 0xf0
        cmp    cl, 0xe0
        je     loc_1_4048
        push   eax
        push   ebx
        call   sub_1_2060
        add    esp, 8
        or     eax, eax
        je     loc_1_3FEC
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_3FEC:
        inc    eax
        lea    eax, [ebx + 0x20]
loc_1_3FF0:
        mov    ecx, dword ptr [eax + 8]
        or     ecx, ecx
        je     loc_1_4001
        mov    edx, esi
        and    edx, ecx
        cmp    edx, dword ptr [esp + 0x10]
        je     loc_1_4007
loc_1_4001:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_3FF0
loc_1_4007:
        or     eax, eax
        je     loc_1_4018
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_4018:
        push   esi
        push   dword ptr [esp + 0x14]
        push   4
        push   ebx
        call   sub_1_2AB4
        add    esp, 0x10
        or     eax, eax
        je     loc_1_4038
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_4038:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_4048:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_4058:
        cmp    eax, 2
        jne    loc_1_407C
        push   esi
        push   dword ptr [esp + 0x14]
        push   4
        push   ebx
        call   sub_1_2BBC
        add    esp, 0x10
        or     eax, eax
        je     loc_1_407C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_407C:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_408C:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_409C:
        cmp    eax, 0x200
        je     loc_1_40B0
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_40B0:
        cmp    dword ptr [edx + 0x10], 0x101
        jne    loc_1_4264
        cmp    dword ptr [esp + 0x28], 0x18
        jb     loc_1_4264
        mov    edx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [edx + 0x14]
        cmp    eax, 2
        jne    loc_1_4140
        mov    cl, byte ptr [edx + 0x13]
        and    ecx, 0x1f
        mov    eax, dword ptr [ebx + 0x1c]
        lea    ecx, [eax + ecx*4]
        mov    esi, dword ptr [ecx]
        or     esi, esi
        je     loc_1_4130
        mov    eax, dword ptr [edx + 0x10]
loc_1_40E9:
        cmp    dword ptr [esi + 8], eax
        je     loc_1_40F6
        mov    ecx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_40E9
loc_1_40F6:
        or     esi, esi
        je     loc_1_4130
        push   esi
        push   ecx
        call   sub_1_2774
        add    esp, 8
        dec    dword ptr [ebx + 0x68]
        mov    eax, dword ptr [esi + 0xc]
        or     eax, eax
        je     loc_1_411B
        push   0
        push   eax
        push   dword ptr [ebx]
        call   sub_1_5BD8
        add    esp, 0xc
loc_1_411B:
        push   esi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_4130:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_4140:
        cmp    eax, 3
        je     loc_1_4158
        cmp    eax, 4
        je     loc_1_4158
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_4158:
        movzx  eax, byte ptr [ebx + 0x5c]
        mov    edx, dword ptr [esp + 0x24]
        cmp    eax, dword ptr [edx + 4]
        je     loc_1_4174
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_4174:
        lea    eax, [esp + 0x14]
        push   eax
        mov    edx, dword ptr [esp + 0x28]
        push   dword ptr [edx + 0x10]
        push   ebx
        call   sub_1_2540
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_419C
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_419C:
        lea    ecx, [ebp + 0x1a]
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     loc_1_41B8
        cmp    eax, 1
        je     loc_1_41C0
        cmp    eax, 2
        je     loc_1_41DC
        cmp    eax, 7
        je     loc_1_41F0
        jmp    loc_1_4207
        db  090h
loc_1_41B8:
        mov    byte ptr [ebp + 0x18], 0xe
        jmp    loc_1_4207
        db  08Bh, 0FFh
loc_1_41C0:
        mov    byte ptr [ecx], 0x10
        mov    byte ptr [ecx + 1], 0x40
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        lea    edi, [ecx + 0xe]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    ecx, 2
        mov    byte ptr [ebp + 0x18], 0x16
        jmp    loc_1_4207
        db  08Bh, 0FFh
loc_1_41DC:
        mov    byte ptr [ecx], 0x57
        mov    esi, 0xbc54                   ; -> loc_1_BC54
        lea    edi, [ecx + 0xd]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        inc    ecx
        mov    byte ptr [ebp + 0x18], 0x15
        jmp    loc_1_4207
loc_1_41F0:
        mov    al, byte ptr [ebx + 0x54]
        mov    byte ptr [ecx], al
        mov    edx, dword ptr [esp + 0x24]
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ecx + 1], al
        mov    byte ptr [ecx + 2], 0xd4
        mov    byte ptr [ebp + 0x18], 3
loc_1_4207:
        cmp    dword ptr [ebx + 8], 7
        je     loc_1_422D
        mov    esi, dword ptr [esp + 0x24]
        add    esi, 8
        mov    edi, ecx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebx + 0x54]
        lea    edi, [ecx + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movzx  eax, byte ptr [ebp + 0x18]
        mov    word ptr [eax + ebp + 0x18], 8
loc_1_422D:
        mov    edx, dword ptr [esp + 0x24]
        cmp    dword ptr [edx + 0x14], 4
        jne    loc_1_424C
        mov    dword ptr [ebp + 4], 0xffffffff
        mov    byte ptr [ebp + 0x19], 2
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_424C:
        call   sub_1_B92C
        mov    dword ptr [ebp + 4], eax
        mov    byte ptr [ebp + 0x19], 2
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_4264:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_4274:
        pop    ebp
        mov    eax, 0xfe
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
sub_1_4284:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        lea    eax, [esi + 0x70]
        push   eax
        call   sub_1_B926
        add    esp, 4
        lea    edi, [esi + 0x88]
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 0x8c], 0
        mov    dword ptr [esi + 0x90], 0
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_42E4
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_B996
        cmp    dword ptr [esp + 0x14], 0x103
        jne    loc_1_42E4
        push   edi
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 0x14], eax
loc_1_42E4:
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_432C
        sub    ebx, ebx
        inc    edi
        mov    edi, eax
loc_1_42F4:
        cmp    ebx, 0x20
        jae    loc_1_4320
        cmp    dword ptr [edi], 0
        je     loc_1_4318
loc_1_42FE:
        mov    ebp, dword ptr [edi]
        push   ebp
        push   edi
        call   sub_1_2774
        add    esp, 8
        push   ebp
        call   sub_1_B944
        add    esp, 4
        cmp    dword ptr [edi], 0
        jne    loc_1_42FE
loc_1_4318:
        inc    ebx
        add    edi, 4
        jmp    loc_1_42F4
        db  08Bh, 0FFh
loc_1_4320:
        push   dword ptr [esp + 0x10]
        call   sub_1_B944
        add    esp, 4
loc_1_432C:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [esi + 0x1c], 0
        or     eax, eax
        je     loc_1_4340
        push   eax
        call   sub_1_BA08
loc_1_4340:
        mov    eax, dword ptr [esi + 0xc]
        or     eax, eax
        je     loc_1_434D
        push   eax
        call   sub_1_BA2C
loc_1_434D:
        mov    edi, dword ptr [esi + 0xdc]
        or     edi, edi
        je     loc_1_4386
        nop    
loc_1_4358:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0xdc], eax
        push   dword ptr [edi + 4]
        call   sub_1_BA2C
        push   dword ptr [edi + 8]
        call   sub_1_B944
        add    esp, 4
        push   edi
        call   sub_1_B944
        add    esp, 4
        mov    edi, dword ptr [esi + 0xdc]
        or     edi, edi
        jne    loc_1_4358
loc_1_4386:
        mov    eax, dword ptr [esi + 0xd0]
        or     eax, eax
        je     loc_1_4399
        push   eax
        call   sub_1_B944
        add    esp, 4
loc_1_4399:
        push   esi
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_43AC:
        push   dword ptr [esp + 4]
        call   sub_1_34D8
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_43BC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [eax]
        sub    edx, edx
        sub    edi, edi
        cmp    esi, edx
        je     loc_1_4410
        mov    ebx, esi
        mov    ecx, dword ptr [esp + 0x18]
loc_1_43D4:
        mov    eax, dword ptr [ecx]
        cmp    eax, 0x280
        jne    loc_1_43F0
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        sub    eax, edx
        sbb    ebp, ebp
        not    ebp
        and    eax, ebp
        add    edx, eax
        jmp    loc_1_4405
        db  08Dh, 049h, 000h
loc_1_43F0:
        cmp    eax, 0x200
        jne    loc_1_4405
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        sub    eax, edi
        sbb    ebp, ebp
        not    ebp
        and    eax, ebp
        add    edi, eax
loc_1_4405:
        add    ecx, 8
        dec    ebx
        jne    loc_1_43D4
        jmp    loc_1_4414
        db  08Dh, 049h, 000h
loc_1_4410:
        mov    ecx, dword ptr [esp + 0x18]
loc_1_4414:
        add    esi, 2
        cmp    esi, 0x20
        jbe    loc_1_4424
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_4424:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0xbc], edx
        mov    dword ptr [eax + 0xc0], edi
        mov    dword ptr [ecx], 0x280
        mov    dword ptr [ecx + 4], edx
        add    ecx, 8
        mov    dword ptr [ecx], 0x200
        mov    dword ptr [ecx + 4], edi
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [eax], esi
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_445C:
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x1c], 0
        push   0xe0
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_4488
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
loc_1_4488:
        mov    edi, esi
        mov    eax, dword ptr [esp + 0x50]
        mov    dword ptr [eax], esi
        sub    eax, eax
        mov    ecx, 0x38
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esi + 0x7c], eax
        mov    dword ptr [esi + 0x84], eax
        mov    eax, dword ptr [0xbf6c]       ; -> loc_1_BF6C
        mov    dword ptr [esi + 0x80], eax
        mov    dword ptr [esp + 0x34], 0
        mov    dword ptr [esp + 0x38], 1
        mov    dword ptr [esp + 0x3c], 2
        mov    dword ptr [esp + 0x40], 7
        mov    byte ptr [esi + 0x5b], 1
        mov    byte ptr [esi + 0x96], 2
        mov    byte ptr [esi + 0x97], 2
        mov    dword ptr [esi + 0x34], 0xffffffff
        mov    dword ptr [esi + 0xd8], 0x64
        call   sub_1_B918
        mov    dword ptr [0xc1c4], eax       ; -> obj1:0xC1C4
        push   8
        push   dword ptr [0xbcd8]            ; -> obj1:0xBCD8
        lea    eax, [esi + 0x10]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_BA02
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_452C
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_452C:
        push   dword ptr [0xbcdc]            ; -> obj1:0xBCDC
        lea    eax, [esi + 0xc]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_455C
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_455C:
        push   0x80
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esi + 0x1c], eax
        mov    edi, eax
        or     edi, edi
        jne    loc_1_4588
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Dh, 049h, 000h
loc_1_4588:
        sub    eax, eax
        mov    ecx, 0x20
        rep stosd dword ptr es:[edi], eax
        lea    ebx, [esi + 0x88]
        mov    dword ptr [ebx], eax
        mov    dword ptr [esi + 0x8c], eax
        mov    dword ptr [esi + 0x90], eax
        lea    edi, [esi + 4]
        push   eax
        push   eax
        mov    ebp, dword ptr [esp + 0x50]
        push   ebp
        push   esi
        push   dword ptr [0xbfe4]            ; -> loc_1_BFE4
        push   4
        lea    eax, [esp + 0x4c]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        push   edi
        lea    eax, [esp + 0x54]
        push   eax
        lea    eax, [esp + 0x40]
        push   eax
        call   sub_1_B990
        cmp    dword ptr [esp + 0x18], 0x103
        jne    loc_1_45E9
        push   ebx
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 0x18], eax
loc_1_45E9:
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + eax*4 + 0x34]
        mov    dword ptr [esi + 8], eax
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_4614
        mov    dword ptr [edi], 0
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
loc_1_4614:
        or     eax, eax
        je     loc_1_4644
        cmp    eax, 1
        je     loc_1_4688
        cmp    eax, 2
        je     loc_1_46AC
        cmp    eax, 7
        je     loc_1_46D8
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_4644:
        mov    byte ptr [esp + 0x13], 6
        mov    byte ptr [esp + 0x17], 1
        mov    byte ptr [esp + 0x16], 1
        mov    byte ptr [esp + 0x15], 0
        mov    edi, 0x1010102
        mov    bl, 0xe
        mov    byte ptr [esp + 0x14], bl
        push   ebp
        call   sub_1_B910
        add    esp, 4
        or     eax, eax
        jne    loc_1_467C
        sub    cl, cl
        mov    edx, 0xb
        jmp    loc_1_46FE
loc_1_467C:
        mov    cl, 8
        mov    bl, 0x16
        mov    edx, 0xb
        jmp    loc_1_46FE
        db  090h
loc_1_4688:
        mov    byte ptr [esp + 0x13], 6
        mov    byte ptr [esp + 0x17], 0x80
        mov    byte ptr [esp + 0x16], 0x80
        mov    byte ptr [esp + 0x15], 2
        mov    edi, 0x2010102
        mov    bl, 0x28
        mov    byte ptr [esp + 0x14], 0xe
        mov    cl, 8
        jmp    loc_1_46F9
loc_1_46AC:
        mov    byte ptr [esp + 0x13], 6
        mov    byte ptr [esp + 0x17], 1
        mov    byte ptr [esp + 0x16], 1
        mov    byte ptr [esp + 0x15], 1
        mov    edi, 0x3010102
        mov    bl, 0x15
        mov    byte ptr [esp + 0x14], 0xd
        mov    cl, 8
        mov    edx, 0xb
        jmp    loc_1_46FE
        db  08Dh, 049h, 000h
loc_1_46D8:
        mov    byte ptr [esp + 0x13], 1
        mov    byte ptr [esp + 0x17], 0xff
        mov    byte ptr [esp + 0x16], 0
        mov    byte ptr [esp + 0x15], 1
        mov    edi, 0x6010102
        mov    bl, 3
        mov    byte ptr [esp + 0x14], bl
        sub    cl, cl
loc_1_46F9:
        mov    edx, 9
loc_1_46FE:
        mov    al, byte ptr [esp + 0x17]
        mov    byte ptr [esi + 0x5d], al
        mov    al, byte ptr [esp + 0x16]
        mov    byte ptr [esi + 0x5e], al
        mov    al, byte ptr [esp + 0x15]
        mov    byte ptr [esi + 0x5f], al
        mov    al, byte ptr [esp + 0x13]
        mov    byte ptr [esi + 0x5c], al
        mov    al, byte ptr [esp + 0x14]
        mov    byte ptr [esi + 0x60], al
        mov    byte ptr [esi + 0x61], cl
        mov    dword ptr [esi + 0x64], edx
        push   0
        movzx  eax, byte ptr [esp + 0x17]
        push   eax
        lea    eax, [esi + 0x54]
        push   eax
        push   edi
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 0x18
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        je     loc_1_475C
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_475C:
        push   0
        push   4
        lea    eax, [esp + 0x28]
        push   eax
        push   0x10106
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 0x18
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        je     loc_1_4790
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
loc_1_4790:
        movzx  eax, byte ptr [esi + 0x61]
        sub    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 1
        jne    loc_1_47A8
        sub    dword ptr [esp + 0x20], 0x12
        jmp    loc_1_47C3
        db  090h
loc_1_47A8:
        cmp    eax, 2
        jne    loc_1_47C3
        mov    eax, dword ptr [esp + 0x20]
        sub    eax, 0x1100
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 0x1100
        mov    dword ptr [esp + 0x20], eax
loc_1_47C3:
        push   0
        mov    eax, dword ptr [esp + 0x24]
        mov    word ptr [esi + 0x94], ax
        push   4
        lea    eax, [esp + 0x34]
        push   eax
        push   0x10107
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 0x18
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        jne    loc_1_4802
        mov    eax, dword ptr [esp + 0x2c]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    dword ptr [esi + 0x98], eax
loc_1_4802:
        push   0
        push   4
        lea    eax, [esp + 0x30]
        push   eax
        push   0x10113
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 0x18
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        je     loc_1_4830
        mov    eax, dword ptr [esp + 0x4c]
        mov    dword ptr [eax], 0
        jmp    loc_1_4847
        db  090h
loc_1_4830:
        mov    ecx, 0
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 1
        or     eax, eax
        setne  cl
        mov    eax, dword ptr [esp + 0x4c]
        mov    dword ptr [eax], ecx
loc_1_4847:
        lea    edi, [esi + 0xc4]
        lea    eax, [esp + 0x24]
        push   eax
        push   0
        push   edi
        push   0x1010d
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 0x18
        mov    dword ptr [esp + 0x18], eax
        cmp    eax, 0xc0010014
        je     loc_1_4877
        cmp    eax, 0xc0010016
        jne    loc_1_48B4
loc_1_4877:
        push   dword ptr [esp + 0x24]
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_48B4
        push   0
        push   dword ptr [esp + 0x28]
        push   ebp
        push   0x1010d
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 0x18
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        jne    loc_1_48B4
        mov    dword ptr [edi], ebp
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [esi + 0xb4], eax
loc_1_48B4:
        test   bl, 3
        je     loc_1_48BF
        and    bl, 0xfc
        add    bl, 4
loc_1_48BF:
        push   esi
        mov    byte ptr [esi + 0x5a], bl
        call   sub_1_17EC
        add    esp, 4
        or     eax, eax
        je     loc_1_48D9
        push   eax
        push   esi
        call   sub_1_17BC
        add    esp, 8
loc_1_48D9:
        movzx  edi, bl
        lea    eax, [edi + 0x1c]
        imul   eax, dword ptr [0xbcd8]       ; -> obj1:0xBCD8
        push   eax
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        jne    loc_1_4908
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_4908:
        mov    dword ptr [esi + 0xd0], eax
        mov    dword ptr [esi + 0x18], 0
        mov    dword ptr [esp + 0x1c], 0
        mov    ecx, dword ptr [0xbcd8]       ; -> obj1:0xBCD8
        or     ecx, ecx
        je     loc_1_493E
        nop    
loc_1_4928:
        mov    edx, dword ptr [esi + 0x18]
        mov    dword ptr [eax], edx
        mov    dword ptr [esi + 0x18], eax
        lea    eax, [edi + eax + 0x1c]
        inc    dword ptr [esp + 0x1c]
        cmp    dword ptr [esp + 0x1c], ecx
        jb     loc_1_4928
loc_1_493E:
        push   esi
        push   0x2c80                        ; -> loc_1_2C80
        push   0x3e8
        add    esi, 0x70
        push   esi
        call   sub_1_B920
        add    esp, 0x10
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_4964:
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [ecx + 0xb8], eax
        mov    eax, 1
        ret    
loc_1_4980:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x4c
        call   sub_1_B94A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_445C
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_49B0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0xc0000001
        leave  
        ret    0x14
        db  090h
loc_1_49B0:
        call   sub_1_B94A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x4c], eax
        mov    dword ptr [ebp - 0x3c], 0x2624 ; -> loc_1_2624
        mov    dword ptr [ebp - 0x38], 0x279c ; -> loc_1_279C
        mov    dword ptr [ebp - 0x34], 0x27d4 ; -> loc_1_27D4
        mov    dword ptr [ebp - 0x30], 0x2ab4 ; -> sub_1_2AB4
        mov    dword ptr [ebp - 0x2c], 0x2bbc ; -> sub_1_2BBC
        mov    dword ptr [ebp - 0x28], 0x2838 ; -> loc_1_2838
        mov    dword ptr [ebp - 0x24], 0x43ac ; -> loc_1_43AC
        mov    dword ptr [ebp - 0x20], 0x3978 ; -> loc_1_3978
        mov    dword ptr [ebp - 0x1c], 0x3e58 ; -> loc_1_3E58
        mov    dword ptr [ebp - 0x18], 0x43bc ; -> loc_1_43BC
        movzx  eax, word ptr [eax + 0x94]
        mov    dword ptr [ebp - 0x48], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x98]
        mov    dword ptr [ebp - 0x44], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 0x5c]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x54
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x4c]
        push   eax
        push   0x4964                        ; -> loc_1_4964
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        call   sub_1_10D0
        add    esp, 0x14
        or     eax, eax
        je     loc_1_4A68
        push   dword ptr [ebp - 4]
        call   sub_1_4284
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0xc0000001
        leave  
        ret    0x14
        db  08Bh, 0FFh
loc_1_4A68:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        leave  
        ret    0x14
        db  08Dh, 049h, 000h
loc_1_4A78:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        lea    eax, [esi + 0x64]
        mov    dword ptr [eax], 0
        push   0
        push   4
        push   eax
        push   0x1010e
        push   1
        push   esi
        call   sub_1_1728
        add    esp, 0x18
        mov    byte ptr [esi + 0x5b], 2
        mov    byte ptr [esi + 0x96], 2
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_4ABF
        push   eax
        call   sub_1_14EC
        add    esp, 4
        mov    dword ptr [esi], 0
loc_1_4ABF:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_4B0C
        lea    edi, [esi + 0x88]
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 0x8c], 0
        mov    dword ptr [esi + 0x90], 0
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_1_B996
        cmp    dword ptr [esp + 8], 0x103
        jne    loc_1_4B14
        push   edi
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 8], eax
        jmp    loc_1_4B14
        db  08Bh, 0FFh
loc_1_4B0C:
        mov    dword ptr [esp + 8], 0
loc_1_4B14:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esp + 8], 0
        jne    loc_1_4B35
        mov    dword ptr [esi + 4], 0
        push   esi
        call   sub_1_4284
        add    esp, 4
loc_1_4B35:
        pop    edi
        pop    esi
        add    esp, 4
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_4B40:
        sub    esp, 4
        call   sub_1_6DB8
        cmp    dword ptr [0xbbf7], 0         ; -> obj1:0xBBF7
        je     loc_1_4B6B
        push   dword ptr [0xbfe4]            ; -> loc_1_BFE4
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_B98A
        push   0
        call   sub_1_B962
        add    esp, 4
loc_1_4B6B:
        add    esp, 4
        ret    
        db  090h
sub_1_4B70:
        push   esi
        push   edi
        mov    eax, dword ptr [0xbfe8]       ; -> loc_1_BFE8
        mov    dl, byte ptr [esp + 0xc]
        cmp    byte ptr [eax + 0x10], dl
        jne    loc_1_4B88
        mov    eax, dword ptr [eax + 4]
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_4B88:
        sub    eax, eax
        sub    esi, esi
        mov    ecx, dword ptr [0xc068]       ; -> loc_1_C068
        cmp    ecx, eax
        jle    loc_1_4BC7
        inc    edi
        mov    edi, 0xc00c                   ; -> obj1:0xC00C
loc_1_4B9C:
        cmp    byte ptr [edi], dl
        je     loc_1_4BAC
        add    edi, 0x14
        inc    esi
        cmp    ecx, esi
        jg     loc_1_4B9C
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_4BAC:
        lea    eax, [esi*4]
        lea    ecx, [eax + eax*4]
        lea    eax, [ecx + 0xbffc]           ; -> loc_1_BFFC
        mov    dword ptr [0xbfe8], eax       ; -> loc_1_BFE8
        mov    eax, dword ptr [ecx + 0xc000] ; -> loc_1_C000
loc_1_4BC7:
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_4BCC:
        push   esi
        push   edi
        sub    edi, edi
        cmp    dword ptr [0xc068], edi       ; -> loc_1_C068
        jle    loc_1_4BF1
        mov    esi, 0xc008                   ; -> loc_1_C008
loc_1_4BDD:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_4BE5
        call   eax
loc_1_4BE5:
        inc    edi
        add    esi, 0x14
        cmp    dword ptr [0xc068], edi       ; -> loc_1_C068
        jg     loc_1_4BDD
loc_1_4BF1:
        pop    edi
        pop    esi
        ret    
sub_1_4BF4:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        lea    esi, [eax + 0x1c]
        mov    eax, dword ptr [eax + 0x1c]
        or     eax, eax
        je     loc_1_4C33
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dx, word ptr [esp + 0x20]
        mov    bl, byte ptr [esp + 0x24]
        mov    edi, dword ptr [esp + 0x18]
loc_1_4C16:
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_4C2B
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_4C2B
        cmp    word ptr [eax + 0xc], dx
        jne    loc_1_4C2B
        cmp    byte ptr [eax + 0xe], bl
        je     loc_1_4C33
loc_1_4C2B:
        mov    esi, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_4C16
loc_1_4C33:
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], esi
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_4C40:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [ecx]
        movzx  ecx, byte ptr [ecx + 8]
        mov    dword ptr [esp + 0x10], ecx
        cmp    ecx, eax
        je     loc_1_4CD3
        mov    ebp, dword ptr [esp + 0x1c]
        nop    
loc_1_4C60:
        mov    cl, byte ptr [esi]
        or     cl, cl
        je     loc_1_4CD3
        cmp    cl, 1
        jne    loc_1_4C70
        inc    eax
        inc    esi
        jmp    loc_1_4CCD
        db  090h
loc_1_4C70:
        mov    dl, byte ptr [esi + 1]
        movzx  edi, dl
        lea    ebx, [edi + eax]
        cmp    ebx, dword ptr [esp + 0x10]
        ja     loc_1_4CE0
        mov    bl, byte ptr [esi + 2]
        dec    bl
        cmp    cl, 0x44
        jne    loc_1_4C90
        cmp    dl, 4
        jb     loc_1_4CEC
        jmp    loc_1_4CC9
loc_1_4C90:
        cmp    dl, 3
        jb     loc_1_4CF8
        cmp    cl, 0x83
        je     loc_1_4CB0
        cmp    cl, 0x89
        je     loc_1_4CB0
        cmp    cl, 7
        jne    loc_1_4CC9
        cmp    bl, dl
        jae    loc_1_4CC9
        mov    byte ptr [ebp + 1], al
        jmp    loc_1_4CC9
        db  08Dh, 049h, 000h
loc_1_4CB0:
        cmp    bl, dl
        jae    loc_1_4CC9
        mov    edx, edi
        and    ebx, 0xff
        sub    edx, ebx
        cmp    edx, 4
        jb     loc_1_4D04
        mov    byte ptr [ebp + 3], cl
        mov    byte ptr [ebp], al
loc_1_4CC9:
        add    eax, edi
        add    esi, edi
loc_1_4CCD:
        cmp    eax, dword ptr [esp + 0x10]
        jb     loc_1_4C60
loc_1_4CD3:
        pop    ebp
        mov    al, 0x28
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_4CE0:
        inc    al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_4CEC:
        inc    al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_4CF8:
        inc    al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_4D04:
        inc    al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_4D10:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [esi + 0xc]
        mov    ebx, dword ptr [0xc090]       ; -> obj1:0xC090
        or     ebx, ebx
        je     loc_1_4D6A
        mov    edi, dword ptr [esp + 0x1c]
loc_1_4D29:
        test   byte ptr [ebx + 0x14], 8
        je     loc_1_4D64
        cmp    dword ptr [ebx + 0xc], ebp
        jne    loc_1_4D64
        push   ebx
        push   edi
        call   sub_1_6FA4
        add    esp, 8
        test   al, 1
        je     loc_1_4D64
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x30]
        push   1
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x34]
        push   dword ptr [ebx + 4]
        push   dword ptr [esp + 0x38]
        push   edi
        push   esi
        call   dword ptr [esp + 0x38]
        add    esp, 0x24
loc_1_4D64:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_1_4D29
loc_1_4D6A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_4D70:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        mov    al, byte ptr [edi + 9]
        push   eax
        call   sub_1_4B70
        add    esp, 4
        or     eax, eax
        je     loc_1_4DFC
        inc    dword ptr [0xc1f8]            ; -> obj1:0xC1F8
        cmp    byte ptr [esp + 0x24], 0
        jne    loc_1_4DD4
        mov    esi, dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x20]
        mov    cl, byte ptr [edi + 9]
        push   ecx
        push   0
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esi + 4]
        push   dword ptr [edi + 0xc]
        push   dword ptr [edi + 0x10]
        push   dword ptr [esp + 0x2c]
        call   eax
        add    esp, 0x24
        or     eax, eax
        je     loc_1_4E18
        push   0
        push   3
        push   3
        push   edi
        push   dword ptr [esi + 4]
        call   sub_1_9610
        add    esp, 0x14
        pop    edi
        pop    esi
        ret    
loc_1_4DD4:
        mov    cl, byte ptr [edi + 9]
        push   dword ptr [esp + 0x20]
        push   ecx
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [edi + 0xc]
        push   dword ptr [edi + 0x10]
        push   dword ptr [esp + 0x24]
        push   eax
        call   sub_1_4D10
        add    esp, 0x20
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_4DFC:
        inc    dword ptr [0xc1f0]            ; -> obj1:0xC1F0
        push   0
        push   2
        push   3
        push   edi
        mov    esi, dword ptr [esp + 0x20]
        push   dword ptr [esi + 4]
        call   sub_1_9610
        add    esp, 0x14
loc_1_4E18:
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_4E1C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_4E3A
loc_1_4E29:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_B944
        add    esp, 4
        or     edi, edi
        jne    loc_1_4E29
loc_1_4E3A:
        push   esi
        call   sub_1_B944
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_4E48:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x40]
        mov    eax, dword ptr [edx + 0xc]
        test   byte ptr [esp + 0x4c], 1
        mov    word ptr [esp + 0x12], ax
        je     loc_1_4E88
        push   dword ptr [esp + 0x4c]
        push   0
        push   0
        push   eax
        push   dword ptr [edx + 8]
        push   dword ptr [esp + 0x5c]
        mov    ebp, dword ptr [esp + 0x5c]
        push   ebp
        push   dword ptr [esp + 0x58]
        call   sub_1_89E0
        add    esp, 0x20
        jmp    loc_1_4E8C
        db  08Dh, 049h, 000h
loc_1_4E88:
        mov    ebp, dword ptr [esp + 0x44]
loc_1_4E8C:
        mov    al, byte ptr [ebp + 9]
        push   eax
        mov    ax, word ptr [ebp + 4]
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [esp + 0x48]
        lea    eax, [esp + 0x30]
        push   eax
        call   sub_1_4BF4
        add    esp, 0x18
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_4F66
        push   0x60
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_4EE8
        call   sub_1_0045
        inc    dword ptr [0xc218]            ; -> obj1:0xC218
        push   dword ptr [esp + 0x40]
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Dh, 049h, 000h
loc_1_4EE8:
        mov    al, byte ptr [ebp + 9]
        push   eax
        mov    ax, word ptr [ebp + 4]
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [esp + 0x48]
        lea    eax, [esp + 0x30]
        push   eax
        call   sub_1_4BF4
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        je     loc_1_4F1C
        push   ebx
        call   sub_1_B944
        add    esp, 4
        mov    ebx, esi
        jmp    loc_1_4F66
        db  090h
loc_1_4F1C:
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 8], eax
        mov    ax, word ptr [ebp + 4]
        mov    word ptr [ebx + 0xc], ax
        mov    al, byte ptr [ebp + 9]
        mov    byte ptr [ebx + 0xe], al
        mov    al, byte ptr [0xbfec]         ; -> loc_1_BFEC
        mov    byte ptr [ebx + 0xf], al
        mov    word ptr [ebx + 0x14], 0xffff
        mov    dword ptr [ebx + 0x10], 0
        mov    word ptr [ebx + 0x16], 0
        mov    dword ptr [ebx + 0x18], 0
loc_1_4F66:
        mov    ax, word ptr [ebp + 6]
        and    ax, 0xff1f
        mov    ch, al
        mov    cl, ah
        shl    cx, 3
        mov    word ptr [esp + 0x10], cx
        or     cx, cx
        jne    loc_1_4F9D
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [ebx + 0x18], eax
        add    eax, 8
        lea    edi, [ebx + 0x1c]
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_4F9D:
        test   byte ptr [ebp + 6], 0x20
        jne    loc_1_4FB1
        mov    ax, word ptr [esp + 0x10]
        add    ax, word ptr [esp + 0x12]
        mov    word ptr [ebx + 0x14], ax
loc_1_4FB1:
        mov    al, byte ptr [ebp + 8]
        sub    al, 0xfe
        sbb    cl, cl
        and    al, cl
        add    al, 0xfe
        movzx  ecx, al
        inc    ecx
        movzx  eax, byte ptr [ebx + 0xf]
        cmp    ecx, eax
        jg     loc_1_4FCA
        mov    ecx, eax
loc_1_4FCA:
        mov    byte ptr [ebx + 0xf], cl
        mov    di, word ptr [esp + 0x10]
        mov    ax, word ptr [esp + 0x12]
        mov    cx, di
        lea    ebp, [eax + ecx - 1]
        lea    ecx, [ebx + 0x10]
        mov    dword ptr [esp + 0x14], ecx
        mov    esi, dword ptr [ebx + 0x10]
        or     esi, esi
        je     loc_1_504B
loc_1_4FEC:
        cmp    word ptr [esi + 0x10], di
        ja     loc_1_5010
        mov    ax, word ptr [esi + 0x12]
        cmp    di, ax
        ja     loc_1_5003
        cmp    bp, ax
        jbe    loc_1_5048
        lea    edi, [eax + 1]
loc_1_5003:
        mov    dword ptr [esp + 0x14], esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_4FEC
        jmp    loc_1_504B
        db  090h
loc_1_5010:
        cmp    word ptr [esi + 0x10], bp
        ja     loc_1_504B
loc_1_5016:
        cmp    word ptr [esi + 0x12], bp
        jae    loc_1_5034
        mov    eax, dword ptr [esi + 0xc]
        sub    word ptr [ebx + 0x16], ax
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_B944
        add    esp, 4
        jmp    loc_1_503A
        db  08Bh, 0FFh
loc_1_5034:
        mov    bp, word ptr [esi + 0x10]
        dec    bp
loc_1_503A:
        or     esi, esi
        je     loc_1_504B
        cmp    word ptr [esi + 0x10], bp
        jbe    loc_1_5016
        jmp    loc_1_504B
        db  08Bh, 0FFh
loc_1_5048:
        lea    ebp, [edi - 1]
loc_1_504B:
        mov    cx, bp
        sub    cx, di
        inc    cx
        mov    word ptr [esp + 0x20], di
        mov    ax, di
        sub    ax, word ptr [esp + 0x10]
        mov    word ptr [esp + 0x20], ax
        movzx  eax, cx
        mov    dword ptr [esp + 0x24], eax
        mov    edx, dword ptr [esp + 0x40]
        mov    dword ptr [edx + 0xc], eax
        mov    word ptr [edx + 0x12], bp
        mov    word ptr [edx + 0x10], di
        mov    ax, word ptr [ebx + 0x16]
        add    ax, cx
        movzx  ecx, word ptr [esp + 0x20]
        mov    word ptr [ebx + 0x16], ax
        add    dword ptr [edx + 8], ecx
        mov    dword ptr [edx], esi
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], edx
        cmp    word ptr [ebx + 0x14], ax
        jne    loc_1_5186
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], eax
        lea    eax, [ebx + 0x1c]
        mov    dword ptr [esp + 0x18], eax
        mov    al, byte ptr [eax + 8]
        mov    byte ptr [esp + 0x31], al
        mov    eax, dword ptr [esp + 0x18]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [esp + 0x32], al
        mov    byte ptr [esp + 0x33], 0
        cmp    dword ptr [ebx + 0x18], 0x14
        je     loc_1_50E4
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x14
        mov    dword ptr [esp + 0x28], eax
        mov    eax, dword ptr [ebx + 0x18]
        sub    al, 0x14
        mov    byte ptr [esp + 0x30], al
        jmp    loc_1_50F1
        db  08Bh, 0FFh
loc_1_50E4:
        mov    dword ptr [esp + 0x28], 0
        mov    byte ptr [esp + 0x30], 0
loc_1_50F1:
        mov    ebp, dword ptr [ebx + 0x10]
        cmp    dword ptr [ebp + 0xc], 0xc8
        jae    loc_1_5154
loc_1_50FD:
        mov    edx, dword ptr [ebp]
        or     edx, edx
        je     loc_1_5154
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, 0xc8
        sub    eax, edi
        mov    ecx, dword ptr [edx + 0xc]
        sub    eax, ecx
        sbb    esi, esi
        and    eax, esi
        add    eax, ecx
        mov    esi, dword ptr [edx + 8]
        mov    ecx, eax
        shr    ecx, 2
        add    edi, dword ptr [ebp + 8]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [ebp + 0xc], eax
        add    dword ptr [edx + 8], eax
        mov    ecx, dword ptr [edx + 0xc]
        sub    ecx, eax
        mov    dword ptr [edx + 0xc], ecx
        jne    loc_1_514B
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebp], eax
        push   edx
        call   sub_1_B944
        add    esp, 4
loc_1_514B:
        cmp    dword ptr [ebp + 0xc], 0xc8
        jb     loc_1_50FD
loc_1_5154:
        inc    dword ptr [0xc214]            ; -> loc_1_C214
        push   dword ptr [esp + 0x4c]
        lea    eax, [esp + 0x2c]
        push   eax
        movzx  eax, word ptr [ebx + 0x14]
        push   eax
        push   ebp
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x4c]
        push   dword ptr [esp + 0x54]
        call   sub_1_4D70
        add    esp, 0x1c
        push   ebx
        call   sub_1_4E1C
        add    esp, 4
loc_1_5186:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Bh, 0FFh
sub_1_5190:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        lea    edi, [esi + 0x38]
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_51CA
        mov    ecx, dword ptr [edi + 0x10]
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx + 0xc], eax
        mov    al, byte ptr [edi + 0x14]
        push   eax
        movzx  eax, byte ptr [edi + 0x15]
        push   eax
        lea    eax, [edi + 0x18]
        push   eax
        push   ecx
        push   dword ptr [esp + 0x20]
        push   dword ptr [edi + 0xc]
        call   sub_1_4E48
        add    esp, 0x18
loc_1_51CA:
        lea    eax, [esp + 8]
        push   eax
        push   esi
        call   sub_1_BA5C
        push   dword ptr [esp + 8]
        call   sub_1_BA3E
        and    byte ptr [edi + 5], 0xfb
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [eax + 0x2c]
        mov    dword ptr [edi], ecx
        mov    dword ptr [eax + 0x2c], esi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  090h
sub_1_51F8:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        inc    dword ptr [0xc210]            ; -> obj1:0xC210
        mov    eax, dword ptr [esp + 0x28]
        movzx  eax, word ptr [eax + 6]
        test   eax, 0xffffff1f
        mov    eax, dword ptr [esp + 0x38]
        jne    loc_1_5228
        sub    eax, 0xc8
        sbb    ecx, ecx
        not    ecx
        and    eax, ecx
        add    eax, 0xc8
loc_1_5228:
        add    eax, 0x14
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_53A4
        lea    edi, [ebx + 0x14]
        mov    dword ptr [ebx + 8], edi
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [ebx + 4], 0
        mov    eax, dword ptr [esp + 0x34]
        cmp    dword ptr [esp + 0x38], eax
        jbe    loc_1_5368
        push   dword ptr [esp + 0x38]
        push   edi
        push   dword ptr [0xc2f4]            ; -> loc_1_C2F4
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_5298
        inc    dword ptr [0xc218]            ; -> obj1:0xC218
        push   ebx
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_5298:
        mov    eax, dword ptr [esp + 0x20]
        mov    ebp, dword ptr [eax + 0xc]
        mov    eax, dword ptr [ebp + 0x2c]
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_5350
        mov    edi, eax
        add    edi, 0x38
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp + 0x2c], eax
        or     byte ptr [edi + 5], 4
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [edi + 0xc], eax
        mov    al, byte ptr [esp + 0x3c]
        mov    byte ptr [edi + 0x14], al
        mov    al, byte ptr [esp + 0x2c]
        mov    byte ptr [edi + 0x15], al
        mov    dword ptr [edi + 0x10], ebx
        mov    eax, dword ptr [esp + 0x2c]
        add    eax, 8
        add    edi, 0x18
        mov    esi, dword ptr [esp + 0x28]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x14]
        call   sub_1_BA50
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x54]
        push   dword ptr [esp + 0x54]
        push   dword ptr [ebp + 4]
        call   dword ptr [ebp + 0xc]
        add    esp, 0x1c
        mov    dword ptr [esp + 0x14], eax
        cmp    eax, 0x103
        je     loc_1_53AF
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x2c]
        call   sub_1_5190
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_5350:
        push   ebx
        call   sub_1_B944
        add    esp, 4
        inc    dword ptr [0xc218]            ; -> obj1:0xC218
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
loc_1_5368:
        mov    eax, dword ptr [esp + 0x38]
        mov    esi, dword ptr [esp + 0x30]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x30]
        push   ebx
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x38]
        call   sub_1_4E48
        add    esp, 0x18
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
loc_1_53A4:
        call   sub_1_0045
        inc    dword ptr [0xc218]            ; -> obj1:0xC218
loc_1_53AF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
sub_1_53B8:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        mov    al, byte ptr [esi]
        and    eax, 0xf
        lea    edx, [eax*4]
        lea    eax, [esi + 0x14]
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], eax
        sub    dl, 0x14
        mov    byte ptr [ecx + 8], dl
        mov    byte ptr [esp + 4], 0x28
        lea    eax, [esp + 4]
        push   eax
        push   ecx
        call   sub_1_4C40
        add    esp, 8
        cmp    al, 0x28
        jae    loc_1_5418
        and    eax, 0xff
        add    eax, 0x14
        push   eax
        push   0
        push   0xc
        push   esi
        mov    eax, dword ptr [esp + 0x1c]
        push   dword ptr [eax + 4]
        call   sub_1_9610
        add    esp, 0x14
        mov    al, 0xff
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_5418:
        cmp    byte ptr [esp + 4], 0x28
        je     loc_1_5430
        test   byte ptr [esp + 0x18], 1
        jne    loc_1_5430
        mov    al, 6
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_5430:
        sub    al, al
        pop    esi
        add    esp, 4
        ret    
        db  090h
sub_1_5438:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x34]
        add    esi, 0x38
        cmp    dword ptr [esp + 0x38], 0
        jne    loc_1_54F8
        lea    edi, [esi + 0x18]
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [esp + 0x19], al
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [esp + 0x1a], al
        movzx  eax, word ptr [edi + 6]
        shl    eax, 8
        shr    eax, 0xd
        and    al, 2
        mov    byte ptr [esp + 0x1b], al
        cmp    byte ptr [esi + 0x15], 0x14
        je     loc_1_548C
        lea    eax, [edi + 0x14]
        mov    dword ptr [esp + 0x10], eax
        mov    al, byte ptr [esi + 0x15]
        sub    al, 0x14
        mov    byte ptr [esp + 0x18], al
        jmp    loc_1_5499
        db  08Bh, 0FFh
loc_1_548C:
        mov    dword ptr [esp + 0x10], 0
        mov    byte ptr [esp + 0x18], 0
loc_1_5499:
        mov    bl, byte ptr [esi + 0x14]
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 0x24], eax
        mov    ebp, dword ptr [esp + 0x3c]
        mov    dword ptr [esp + 0x28], ebp
        push   ebx
        lea    eax, [esp + 0x14]
        push   eax
        push   ebp
        lea    eax, [esp + 0x28]
        push   eax
        push   edi
        push   dword ptr [esi + 0xc]
        push   dword ptr [esp + 0x48]
        call   sub_1_4D70
        add    esp, 0x1c
        test   bl, 1
        je     loc_1_54F8
        push   ebx
        push   0
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        movzx  eax, byte ptr [esi + 0x15]
        push   eax
        push   edi
        push   dword ptr [esp + 0x4c]
        call   sub_1_89E0
        add    esp, 0x20
loc_1_54F8:
        mov    eax, dword ptr [esp + 0x30]
        mov    ecx, dword ptr [eax + 0xc]
        mov    eax, dword ptr [ecx + 0x2c]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [ecx + 0x2c], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
sub_1_5514:
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x4c]
        mov    dword ptr [esp + 0x14], esi
        sub    ebp, ebp
        inc    dword ptr [0xc1e0]            ; -> obj1:0xC1E0
        mov    edi, dword ptr [esp + 0x50]
        cmp    edi, ebp
        je     loc_1_5830
        mov    al, byte ptr [esi]
        and    eax, 0xf
        lea    ebx, [eax*4]
        cmp    ebx, 0x14
        jb     loc_1_5830
        cmp    edi, ebx
        jb     loc_1_5830
        push   ebx
        push   esi
        call   sub_1_5C30
        add    esp, 8
        cmp    ax, 0xffff
        jne    loc_1_5830
        mov    ax, word ptr [esi + 2]
        mov    cx, ax
        and    ecx, 0xff
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     ecx, eax
        mov    dword ptr [esp + 0x18], ecx
        mov    al, byte ptr [esi]
        and    al, 0xf0
        cmp    al, 0x40
        jne    loc_1_5830
        cmp    ecx, 0x14
        jbe    loc_1_5830
        mov    eax, dword ptr [esp + 0x54]
        cmp    eax, ecx
        jb     loc_1_5830
        sub    dword ptr [esp + 0x18], ebx
        add    esi, ebx
        sub    edi, ebx
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [eax + 0x10]
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [esp + 0x24], eax
        lea    eax, [esp + 0x24]
        push   eax
        push   ecx
        call   sub_1_714C
        add    esp, 8
        mov    byte ptr [esp + 0x13], al
        cmp    dword ptr [esp + 0x60], ebp
        je     loc_1_55EC
        test   al, 1
        jne    loc_1_55EC
        inc    dword ptr [0xc1e4]            ; -> loc_1_C1E4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Dh, 049h, 000h
loc_1_55EC:
        mov    eax, dword ptr [esp + 0x14]
        mov    al, byte ptr [eax + 8]
        mov    byte ptr [esp + 0x31], al
        mov    eax, dword ptr [esp + 0x14]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [esp + 0x32], al
        mov    eax, dword ptr [esp + 0x14]
        movzx  eax, word ptr [eax + 6]
        shl    eax, 8
        shr    eax, 0xd
        and    al, 2
        mov    byte ptr [esp + 0x33], al
        mov    dword ptr [esp + 0x28], 0
        mov    byte ptr [esp + 0x30], 0
        cmp    byte ptr [esp + 0x13], 6
        jae    loc_1_57F0
        cmp    ebx, 0x14
        je     loc_1_5668
        push   dword ptr [esp + 0x13]
        lea    eax, [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x54]
        call   sub_1_53B8
        add    esp, 0x10
        or     al, al
        je     loc_1_5668
        cmp    al, 6
        je     loc_1_57F0
        inc    dword ptr [0xc1e4]            ; -> loc_1_C1E4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_5668:
        mov    eax, dword ptr [esp + 0x14]
        movzx  eax, word ptr [eax + 6]
        test   eax, 0xffffff3f
        jne    loc_1_57C0
        cmp    dword ptr [esp + 0x18], edi
        jbe    loc_1_5738
        mov    eax, dword ptr [esp + 0x48]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esp + 0x1c], ecx
        mov    eax, dword ptr [ecx + 0x2c]
        mov    dword ptr [esp + 0x20], eax
        or     eax, eax
        je     loc_1_5728
        mov    ebp, eax
        add    ebp, 0x38
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx + 0x2c], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [ebp + 0xc], eax
        mov    al, byte ptr [esp + 0x13]
        mov    byte ptr [ebp + 0x14], al
        mov    byte ptr [ebp + 0x15], bl
        lea    eax, [ebx + 8]
        lea    edi, [ebp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x20]
        push   ebx
        push   dword ptr [esp + 0x6c]
        push   dword ptr [esp + 0x6c]
        mov    ecx, dword ptr [esp + 0x34]
        push   dword ptr [ecx + 4]
        call   dword ptr [ecx + 0xc]
        add    esp, 0x1c
        mov    esi, dword ptr [ebp + 8]
        cmp    eax, 0x103
        je     loc_1_5836
        or     eax, eax
        je     loc_1_5738
        inc    dword ptr [0xc1f4]            ; -> loc_1_C1F4
        mov    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ecx + 0x2c]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0x2c], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  090h
loc_1_5728:
        pop    ebp
        inc    dword ptr [0xc1f4]            ; -> loc_1_C1F4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_5738:
        mov    dword ptr [esp + 0x3c], esi
        mov    dword ptr [esp + 0x34], 0
        mov    dword ptr [esp + 0x38], 0
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0x40], eax
        push   dword ptr [esp + 0x13]
        lea    eax, [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x40]
        push   eax
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x60]
        call   sub_1_4D70
        add    esp, 0x1c
        test   byte ptr [esp + 0x13], 1
        je     loc_1_579F
        push   dword ptr [esp + 0x13]
        push   0
        push   0
        push   dword ptr [esp + 0x24]
        push   esi
        push   ebx
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x64]
        call   sub_1_89E0
        add    esp, 0x20
loc_1_579F:
        or     ebp, ebp
        je     loc_1_5836
        mov    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ecx + 0x2c]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0x2c], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
loc_1_57C0:
        push   dword ptr [esp + 0x5c]
        push   dword ptr [esp + 0x5c]
        push   dword ptr [esp + 0x1b]
        push   dword ptr [esp + 0x24]
        push   edi
        push   esi
        push   ebx
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x6c]
        call   sub_1_51F8
        add    esp, 0x28
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  090h
loc_1_57F0:
        cmp    byte ptr [esp + 0x13], 0xff
        je     loc_1_5820
        push   dword ptr [esp + 0x13]
        push   dword ptr [esp + 0x60]
        push   dword ptr [esp + 0x60]
        push   edi
        push   esi
        push   ebx
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x64]
        call   sub_1_89E0
        add    esp, 0x20
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_5820:
        pop    ebp
        inc    dword ptr [0xc1e8]            ; -> obj1:0xC1E8
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_5830:
        inc    dword ptr [0xc1e4]            ; -> loc_1_C1E4
loc_1_5836:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
sub_1_5840:
        mov    ecx, dword ptr [esp + 8]
        lea    eax, [ecx + 0x38]
        mov    al, byte ptr [eax + 5]
        test   al, 8
        jne    loc_1_5874
        test   al, 4
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   ecx
        push   dword ptr [esp + 0x10]
        jne    loc_1_5868
        call   sub_1_5438
        add    esp, 0x10
        ret    
loc_1_5868:
        call   sub_1_5190
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_5874:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   ecx
        call   sub_1_8710
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
sub_1_5888:
        mov    eax, dword ptr [0xbce0]       ; -> obj1:0xBCE0
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [0xbce0], ecx       ; -> obj1:0xBCE0
        ret    
        db  08Bh, 0FFh
sub_1_589C:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        or     esi, esi
        je     loc_1_58C1
loc_1_58A8:
        lea    eax, [esp + 4]
        push   eax
        push   esi
        call   sub_1_BA68
        push   esi
        call   sub_1_BA3E
        mov    esi, dword ptr [esp + 4]
        or     esi, esi
        jne    loc_1_58A8
loc_1_58C1:
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_58C8:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        lea    edi, [esi + 0x38]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 0x10], eax
        mov    al, byte ptr [edi + 5]
        test   al, 0x18
        jne    loc_1_5904
        push   esi
        call   sub_1_BA14
        mov    eax, dword ptr [0xc2ec]       ; -> obj1:0xC2EC
        mov    dword ptr [edi], eax
        mov    dword ptr [0xc2ec], esi       ; -> obj1:0xC2EC
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
loc_1_5904:
        and    al, 0xef
        mov    byte ptr [edi + 5], al
        mov    ebx, dword ptr [esp + 0x10]
        mov    eax, ebx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x10], eax
        test   byte ptr [edi + 5], 1
        je     loc_1_5957
        mov    ebp, eax
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x14]
        call   sub_1_BA68
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ebp
        call   sub_1_BA44
        test   byte ptr [edi + 5], 8
        jne    loc_1_594D
        push   dword ptr [esp + 0x14]
        call   sub_1_B944
        add    esp, 4
loc_1_594D:
        push   ebp
        call   sub_1_BA3E
        and    byte ptr [edi + 5], 0xfe
loc_1_5957:
        test   byte ptr [edi + 5], 2
        je     loc_1_5975
        push   dword ptr [esp + 0x10]
        call   sub_1_589C
        add    esp, 4
        mov    dword ptr [esp + 0x10], 0
        and    byte ptr [edi + 5], 0xfd
loc_1_5975:
        test   byte ptr [edi + 5], 8
        jne    loc_1_5997
        push   ebx
        call   sub_1_5888
        add    esp, 4
        push   esi
        call   sub_1_BA14
        mov    eax, dword ptr [0xc2ec]       ; -> obj1:0xC2EC
        mov    dword ptr [edi], eax
        mov    dword ptr [0xc2ec], esi       ; -> obj1:0xC2EC
loc_1_5997:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
sub_1_59A4:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0xbcec]       ; -> obj1:0xBCEC
        cmp    dword ptr [0xbcf0], eax       ; -> obj1:0xBCF0
        jbe    loc_1_59EA
        push   0xc
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_59EA
        lea    ebx, [esi + 4]
        push   0x14
        push   0x10
        push   ebx
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA02
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_59F4
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_59EA:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_59F4:
        sub    ebp, ebp
        mov    eax, dword ptr [0xbce4]       ; -> obj1:0xBCE4
        mov    dword ptr [esi], eax
        mov    dword ptr [0xbce4], esi       ; -> obj1:0xBCE4
        add    dword ptr [0xbcec], 0x10      ; -> obj1:0xBCEC
        sub    esi, esi
loc_1_5A0C:
        push   dword ptr [ebx]
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_BA0E
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_5A56
        mov    edi, dword ptr [esp + 0x10]
        add    edi, 0x38
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax + 0x3c], 1
        or     esi, esi
        je     loc_1_5A4C
        push   dword ptr [esp + 0x10]
        call   sub_1_58C8
        add    esp, 4
        jmp    loc_1_5A50
loc_1_5A4C:
        mov    ebp, dword ptr [esp + 0x10]
loc_1_5A50:
        inc    esi
        cmp    esi, 0x10
        jb     loc_1_5A0C
loc_1_5A56:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_5A60:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0xbcf8]       ; -> obj1:0xBCF8
        cmp    eax, dword ptr [0xbcf4]       ; -> obj1:0xBCF4
        jbe    loc_1_5AA4
        push   0xc
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_5AA4
        lea    edi, [esi + 4]
        push   0x10
        push   edi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_5AB0
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_5AA4:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_5AB0:
        push   0x140
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_5AE0
        push   dword ptr [edi]
        call   sub_1_BA2C
        push   esi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_5AE0:
        mov    dword ptr [esi + 8], ebx
        mov    eax, dword ptr [0xbce8]       ; -> obj1:0xBCE8
        mov    dword ptr [esi], eax
        mov    dword ptr [0xbce8], esi       ; -> obj1:0xBCE8
        sub    esi, esi
        add    dword ptr [0xbcf4], 0x10      ; -> obj1:0xBCF4
        sub    ebp, ebp
loc_1_5AFC:
        push   0x14
        push   ebx
        push   dword ptr [edi]
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_5B39
        or     ebp, ebp
        je     loc_1_5B2C
        push   dword ptr [esp + 0x10]
        call   sub_1_5888
        add    esp, 4
        jmp    loc_1_5B30
        db  08Dh, 049h, 000h
loc_1_5B2C:
        mov    esi, dword ptr [esp + 0x10]
loc_1_5B30:
        inc    ebp
        add    ebx, 0x14
        cmp    ebp, 0x10
        jb     loc_1_5AFC
loc_1_5B39:
        mov    eax, esi
        sub    ebp, 0x10
        add    dword ptr [0xbcf4], ebp       ; -> obj1:0xBCF4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_5B4C:
        mov    eax, dword ptr [0xc2ec]       ; -> obj1:0xC2EC
        or     eax, eax
        je     loc_1_5B60
        mov    ecx, dword ptr [eax + 0x38]
        mov    dword ptr [0xc2ec], ecx       ; -> obj1:0xC2EC
        ret    
        db  090h
loc_1_5B60:
        jmp    sub_1_59A4
        db  08Dh, 049h, 000h
sub_1_5B68:
        mov    eax, dword ptr [0xbce0]       ; -> obj1:0xBCE0
        or     eax, eax
        je     loc_1_5B80
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0xbce0], ecx       ; -> obj1:0xBCE0
        mov    dword ptr [eax], 0
        ret    
loc_1_5B80:
        jmp    loc_1_5A60
        db  08Dh, 049h, 000h
sub_1_5B88:
        push   esi
        push   edi
        call   sub_1_5B4C
        mov    edi, eax
        or     edi, edi
        je     loc_1_5BC1
        call   sub_1_5B68
        mov    esi, eax
        or     esi, esi
        je     loc_1_5BB8
        push   esi
        push   edi
        call   sub_1_BA56
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], edi
        or     byte ptr [edi + 0x3d], 0x10
        mov    eax, dword ptr [esi + 4]
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_5BB8:
        push   edi
        call   sub_1_58C8
        add    esp, 4
loc_1_5BC1:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_5BC8:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        add    eax, dword ptr [edx + 4]
        mov    dword ptr [edx + 4], eax
        ret    
        db  090h
sub_1_5BD8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        lea    ecx, [eax + 0x38]
        mov    edx, dword ptr [ecx + 0xc]
        mov    esi, dword ptr [edx]
        mov    edi, dword ptr [ecx + 0x10]
        mov    ebx, dword ptr [ecx + 8]
        push   eax
        call   sub_1_58C8
        add    esp, 4
        or     ebx, ebx
        jne    loc_1_5C08
        push   eax
        push   edi
        call   esi
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_5C08:
        push   -1
        push   ebx
        call   sub_1_5BC8
        add    esp, 8
        or     eax, eax
        jne    loc_1_5C29
        mov    ebp, dword ptr [ebx]
        push   ebx
        call   sub_1_B944
        add    esp, 4
        push   ebp
        push   edi
        call   esi
        add    esp, 8
loc_1_5C29:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_5C30:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        sub    esi, esi
        mov    ecx, dword ptr [esp + 0x10]
        cmp    ecx, 1
        jle    loc_1_5C59
        mov    edx, ecx
        shr    edx, 1
        lea    eax, [edx*2]
        sub    ecx, eax
loc_1_5C4E:
        movzx  eax, word ptr [edi]
        add    esi, eax
        add    edi, 2
        dec    edx
        jne    loc_1_5C4E
loc_1_5C59:
        or     ecx, ecx
        je     loc_1_5C62
        movzx  eax, byte ptr [edi]
        add    esi, eax
loc_1_5C62:
        mov    eax, esi
        shr    eax, 0x10
        and    esi, 0xffff
        add    eax, esi
        mov    ecx, eax
        shr    ecx, 0x10
        add    eax, ecx
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_5C7C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0x14
        push   dword ptr [esp + 0x30]
        call   sub_1_5C30
        add    esp, 8
        movzx  edi, ax
        mov    ebx, dword ptr [esp + 0x30]
        or     ebx, ebx
        mov    esi, dword ptr [esp + 0x24]
        je     loc_1_5D1A
        lea    ebp, [esi + 0x38]
        push   dword ptr [esp + 0x34]
        push   ebx
        push   dword ptr [0xc2f4]            ; -> loc_1_C2F4
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_5CE4
        push   ebx
        call   sub_1_B944
        add    esp, 4
        push   esi
        call   sub_1_58C8
        add    esp, 4
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_5CE4:
        or     byte ptr [ebp + 5], 1
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_BA56
        push   dword ptr [esp + 0x34]
        push   ebx
        call   sub_1_5C30
        add    esp, 8
        and    eax, 0xffff
        add    edi, eax
        mov    eax, edi
        shr    eax, 0x10
        and    edi, 0xffff
        add    edi, eax
        mov    eax, edi
        shr    eax, 0x10
        add    edi, eax
loc_1_5D1A:
        not    di
        push   dword ptr [esp + 0x28]
        mov    eax, dword ptr [esp + 0x30]
        mov    word ptr [eax + 0xa], di
        push   esi
        call   sub_1_BA56
        push   0
        push   dword ptr [esp + 0x24]
        push   esi
        mov    eax, dword ptr [esp + 0x28]
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 8]
        add    esp, 0x10
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, 0x103
        jne    loc_1_5D5C
        mov    eax, 0x2bf7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_5D5C:
        push   esi
        call   sub_1_58C8
        add    esp, 4
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_5D78
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_5D78:
        pop    ebp
        mov    eax, 0x2b00
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
sub_1_5D88:
        mov    eax, dword ptr [0xbc04]       ; -> obj1:0xBC04
        or     eax, eax
        je     loc_1_5DC0
        mov    cx, word ptr [eax + 0x14]
        and    cl, 9
        cmp    cl, 8
        jne    loc_1_5DC0
        push   0
        push   0
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [eax + 0xc]
        call   sub_1_5C7C
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_5DC0:
        push   dword ptr [esp + 8]
        call   sub_1_58C8
        add    esp, 4
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        mov    eax, 0x2afb
        ret    
sub_1_5DD8:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        inc    ecx
        mov    ecx, dword ptr [esp + 0x20]
loc_1_5DE8:
        mov    eax, dword ptr [edi + 0xc]
        cmp    ecx, eax
        jb     loc_1_5E04
        sub    ecx, eax
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_5DE8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_5E04:
        lea    ebx, [esp + 0x14]
        mov    dword ptr [esp + 0x14], 0
        mov    esi, dword ptr [esp + 0x24]
loc_1_5E14:
        mov    eax, dword ptr [edi + 0xc]
        sub    eax, ecx
        sub    eax, esi
        sbb    edx, edx
        and    eax, edx
        lea    ebp, [eax + esi]
        push   ebp
        mov    eax, dword ptr [edi + 4]
        add    eax, ecx
        push   eax
        push   dword ptr [0xc2f4]            ; -> loc_1_C2F4
        push   ebx
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_5E4F
        sub    ecx, ecx
        mov    ebx, dword ptr [ebx]
        mov    edi, dword ptr [edi]
        sub    esi, ebp
        je     loc_1_5E4F
        cmp    edi, ecx
        jne    loc_1_5E14
loc_1_5E4F:
        or     esi, esi
        jne    loc_1_5E60
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_5E60:
        mov    esi, dword ptr [esp + 0x14]
        or     esi, esi
        je     loc_1_5E76
loc_1_5E68:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_BA3E
        or     esi, esi
        jne    loc_1_5E68
loc_1_5E76:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_5E80:
        sub    esp, 0x48
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x68]
        add    eax, 0x38
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x24], 0
        mov    word ptr [esp + 0x14], 0
        sub    ebx, ebx
        mov    byte ptr [esp + 0x13], bl
        mov    ebp, dword ptr [esp + 0x60]
        sub    ebp, dword ptr [esp + 0x7c]
        and    ebp, 0xfffffff8
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esp + 0x2c], eax
        mov    ecx, dword ptr [esp + 0x6c]
        mov    ax, word ptr [ecx + 6]
        test   al, 0x40
        je     loc_1_5F0C
        push   dword ptr [esp + 0x68]
        call   sub_1_58C8
        add    esp, 4
        cmp    dword ptr [esp + 0x78], ebx
        je     loc_1_5EE3
        push   dword ptr [esp + 0x78]
        call   sub_1_B944
        add    esp, 4
loc_1_5EE3:
        cmp    dword ptr [esp + 0x80], 0
        jne    loc_1_5EF9
        push   dword ptr [esp + 0x2c]
        call   sub_1_B944
        add    esp, 4
loc_1_5EF9:
        pop    ebp
        inc    dword ptr [0xc220]            ; -> loc_1_C220
        mov    eax, 0x2b01
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x48
        ret    
loc_1_5F0C:
        and    ax, 0xff1f
        mov    ch, al
        mov    cl, ah
        lea    eax, [ecx*8]
        cmp    dword ptr [esp + 0x78], 0
        mov    word ptr [esp + 0x16], ax
        je     loc_1_5F82
        mov    edx, dword ptr [esp + 0x78]
        lea    edi, [esp + 0x30]
        sub    eax, eax
        mov    ecx, 0xa
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esp + 0x78]
        cmp    byte ptr [eax], 0
        je     loc_1_5F77
loc_1_5F41:
        mov    al, byte ptr [edx]
        test   al, 0x80
        je     loc_1_5F64
        movzx  eax, byte ptr [edx + 1]
        lea    edi, [esp + ebx + 0x30]
        mov    esi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebx, eax
        jmp    loc_1_5F70
        db  090h
loc_1_5F64:
        cmp    al, 1
        jne    loc_1_5F6C
        inc    edx
        jmp    loc_1_5F72
        db  090h
loc_1_5F6C:
        movzx  eax, byte ptr [edx + 1]
loc_1_5F70:
        add    edx, eax
loc_1_5F72:
        cmp    byte ptr [edx], 0
        jne    loc_1_5F41
loc_1_5F77:
        test   bl, 3
        je     loc_1_5F82
        and    ebx, 0xfffffffc
        add    ebx, 4
loc_1_5F82:
        mov    eax, dword ptr [esp + 0x28]
        or     byte ptr [eax + 5], 2
loc_1_5F8A:
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_5B88
        add    esp, 4
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        je     loc_1_60CC
        movzx  eax, word ptr [esp + 0x14]
        mov    dword ptr [esp + 0x20], eax
        push   ebp
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x78]
        call   sub_1_5DD8
        add    esp, 0xc
        or     eax, eax
        je     loc_1_60C0
        mov    edx, dword ptr [esp + 0x1c]
        add    edx, 0x38
        mov    edi, edx
        mov    esi, dword ptr [esp + 0x28]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x6c]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        and    byte ptr [edx + 5], 0xf7
        mov    ecx, dword ptr [esp + 0x7c]
        add    ecx, 0x14
        shr    ecx, 2
        add    cl, 0x40
        mov    esi, dword ptr [esp + 0x18]
        mov    byte ptr [esi], cl
        mov    ecx, dword ptr [esp + 0x7c]
        add    ecx, ebp
        add    ecx, 0x14
        mov    cl, ch
        mov    dx, word ptr [esp + 0x7c]
        add    dx, bp
        add    dx, 0x14
        mov    ch, dl
        mov    word ptr [esi + 2], cx
        movzx  ecx, word ptr [esp + 0x16]
        add    ecx, dword ptr [esp + 0x20]
        sar    ecx, 3
        push   dword ptr [esp + 0x7c]
        mov    dx, cx
        push   dword ptr [esp + 0x7c]
        or     dx, 0x2000
        mov    dl, dh
        mov    dh, cl
        push   dword ptr [esp + 0x20]
        mov    word ptr [esi + 6], dx
        push   eax
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x78]
        push   dword ptr [esp + 0x74]
        call   sub_1_5C7C
        add    esp, 0x1c
        cmp    eax, 0x2bf7
        jne    loc_1_6064
        inc    dword ptr [esp + 0x24]
loc_1_6064:
        inc    dword ptr [0xc224]            ; -> obj1:0xC224
        add    word ptr [esp + 0x14], bp
        sub    dword ptr [esp + 0x74], ebp
        or     ebx, ebx
        je     loc_1_60A4
        mov    dword ptr [esp + 0x7c], ebx
        push   ebx
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 0x78], eax
        or     eax, eax
        je     loc_1_60CC
        lea    esi, [esp + 0x30]
        mov    edi, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_60B4
        db  08Bh, 0FFh
loc_1_60A4:
        mov    dword ptr [esp + 0x78], 0
        mov    dword ptr [esp + 0x7c], 0
loc_1_60B4:
        cmp    dword ptr [esp + 0x74], ebp
        ja     loc_1_5F8A
        jmp    loc_1_60D1
loc_1_60C0:
        push   dword ptr [esp + 0x1c]
        call   sub_1_58C8
        add    esp, 4
loc_1_60CC:
        mov    byte ptr [esp + 0x13], 1
loc_1_60D1:
        cmp    byte ptr [esp + 0x13], 0
        jne    loc_1_61B0
        movzx  esi, word ptr [esp + 0x14]
        push   dword ptr [esp + 0x74]
        push   esi
        push   dword ptr [esp + 0x78]
        call   sub_1_5DD8
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_6118
        cmp    dword ptr [esp + 0x78], 0
        je     loc_1_6109
        push   dword ptr [esp + 0x78]
        call   sub_1_B944
        add    esp, 4
loc_1_6109:
        push   dword ptr [esp + 0x68]
        call   sub_1_58C8
        jmp    loc_1_61CC
        db  090h
loc_1_6118:
        mov    edx, dword ptr [esp + 0x7c]
        add    edx, 0x14
        shr    edx, 2
        add    dl, 0x40
        mov    ecx, dword ptr [esp + 0x6c]
        mov    byte ptr [ecx], dl
        mov    edx, dword ptr [esp + 0x74]
        add    edx, dword ptr [esp + 0x7c]
        add    edx, 0x14
        mov    dl, dh
        mov    ecx, dword ptr [esp + 0x74]
        add    cx, word ptr [esp + 0x7c]
        add    cx, 0x14
        mov    dh, cl
        mov    ecx, dword ptr [esp + 0x6c]
        mov    word ptr [ecx + 2], dx
        movzx  edx, word ptr [esp + 0x16]
        add    edx, esi
        sar    edx, 3
        mov    bx, word ptr [ecx + 6]
        and    bx, 0x20
        push   dword ptr [esp + 0x7c]
        mov    cx, dx
        push   dword ptr [esp + 0x7c]
        shr    cx, 8
        push   dword ptr [esp + 0x74]
        or     bx, cx
        mov    bh, dl
        mov    ecx, dword ptr [esp + 0x78]
        mov    word ptr [ecx + 6], bx
        push   eax
        push   dword ptr [esp + 0x78]
        push   dword ptr [esp + 0x78]
        push   dword ptr [esp + 0x74]
        call   sub_1_5C7C
        add    esp, 0x1c
        cmp    eax, 0x2bf7
        jne    loc_1_61A1
        inc    dword ptr [esp + 0x24]
loc_1_61A1:
        inc    dword ptr [0xc224]            ; -> obj1:0xC224
        inc    dword ptr [0xc21c]            ; -> obj1:0xC21C
        jmp    loc_1_61D5
        db  090h
loc_1_61B0:
        push   dword ptr [esp + 0x68]
        call   sub_1_58C8
        add    esp, 4
        cmp    dword ptr [esp + 0x78], 0
        je     loc_1_61CF
        push   dword ptr [esp + 0x78]
        call   sub_1_B944
loc_1_61CC:
        add    esp, 4
loc_1_61CF:
        inc    dword ptr [0xc204]            ; -> loc_1_C204
loc_1_61D5:
        mov    ecx, dword ptr [esp + 0x80]
        or     ecx, ecx
        jne    loc_1_621C
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x30]
        call   sub_1_5BC8
        add    esp, 8
        or     eax, eax
        jne    loc_1_620C
        push   dword ptr [esp + 0x2c]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x48
        ret    
        db  08Bh, 0FFh
loc_1_620C:
        pop    ebp
        mov    eax, 0x2bf7
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x48
        ret    
        db  08Dh, 049h, 000h
loc_1_621C:
        mov    eax, dword ptr [esp + 0x24]
        add    dword ptr [ecx], eax
        mov    eax, 0x2bf7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x48
        ret    
        db  090h
sub_1_6230:
        push   ebx
        mov    ecx, dword ptr [esp + 8]
        mov    al, byte ptr [ecx + 2]
        dec    al
        mov    bl, byte ptr [ecx + 1]
        cmp    al, bl
        jae    loc_1_625F
        movzx  edx, al
        movzx  eax, bl
        sub    eax, edx
        cmp    eax, 4
        jae    loc_1_6254
        sub    al, al
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_6254:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [edx + ecx], eax
        add    byte ptr [ecx + 2], 4
loc_1_625F:
        pop    ebx
        mov    al, 1
        ret    
        db  090h
sub_1_6264:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        or     esi, esi
        je     loc_1_6367
        mov    edi, dword ptr [esp + 0x1c]
        or     edi, edi
        je     loc_1_6367
        mov    bl, byte ptr [edi]
        cmp    bl, 0x28
        mov    ebp, dword ptr [esp + 0x20]
        je     loc_1_62AC
        push   ebp
        movzx  eax, bl
        add    eax, esi
        push   eax
        call   sub_1_6230
        add    esp, 8
        or     al, al
        jne    loc_1_62AC
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_62AC:
        mov    bl, byte ptr [edi + 1]
        cmp    bl, 0x28
        je     loc_1_62D4
        push   ebp
        movzx  eax, bl
        add    eax, esi
        push   eax
        call   sub_1_6230
        add    esp, 8
        or     al, al
        jne    loc_1_62D4
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_62D4:
        mov    bl, byte ptr [edi + 2]
        cmp    bl, 0x28
        je     loc_1_6367
        movzx  eax, bl
        add    esi, eax
        mov    cl, byte ptr [esi + 2]
        dec    cl
        mov    byte ptr [esp + 0x12], cl
        mov    al, byte ptr [esi + 3]
        mov    cl, al
        and    cl, 0xf
        mov    byte ptr [esp + 0x13], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [esp + 0x11], cl
        mov    cl, byte ptr [esp + 0x12]
        cmp    cl, byte ptr [esp + 0x11]
        jae    loc_1_63CC
        call   sub_1_B92C
        add    eax, dword ptr [0xc1d0]       ; -> loc_1_C1D0
        or     eax, dword ptr [0xc094]       ; -> obj1:0xC094
        mov    ecx, 0x5265c00
        sub    edx, edx
        div    ecx
        mov    edi, edx
        shr    edi, 0x10
        mov    eax, edx
        and    eax, 0xff0000
        or     edi, eax
        shr    edi, 8
        mov    eax, edx
        shl    eax, 0x10
        and    edx, 0xff00
        or     eax, edx
        shl    eax, 8
        or     edi, eax
        movzx  eax, byte ptr [esp + 0x13]
        movzx  edx, byte ptr [esp + 0x12]
        lea    ecx, [edx + esi]
        or     eax, eax
        je     loc_1_6374
        cmp    eax, 1
        je     loc_1_63A0
        cmp    eax, 3
        je     loc_1_639C
loc_1_6367:
        pop    ebp
        mov    al, 0x28
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_6374:
        movzx  eax, byte ptr [esp + 0x11]
        sub    eax, edx
        cmp    eax, 4
        jae    loc_1_638C
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_638C:
        mov    dword ptr [ecx], edi
        add    byte ptr [esi + 2], 4
        mov    al, 0x28
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_639C:
        cmp    dword ptr [ecx], ebp
        jne    loc_1_6367
loc_1_63A0:
        movzx  eax, byte ptr [esp + 0x11]
        sub    eax, edx
        cmp    eax, 8
        jae    loc_1_63B8
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_63B8:
        mov    dword ptr [ecx], ebp
        mov    dword ptr [ecx + 4], edi
        add    byte ptr [esi + 2], 8
        mov    al, 0x28
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_63CC:
        mov    cl, al
        and    cl, 0xf0
        cmp    cl, 0xf0
        je     loc_1_63E8
        add    al, 0x10
        mov    byte ptr [esi + 3], al
        mov    al, 0x28
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_63E8:
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_63F4:
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x50]
        add    eax, 0x38
        mov    dword ptr [esp + 0x3c], eax
        mov    dword ptr [esp + 0x30], 0
        mov    eax, dword ptr [0xc154]       ; -> obj1:0xC154
        lea    eax, [eax + eax*2]
        shl    eax, 2
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 0x20], eax
        or     eax, eax
        jne    loc_1_6438
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  090h
loc_1_6438:
        mov    eax, dword ptr [0xc154]       ; -> obj1:0xC154
        lea    edx, [eax + eax*2]
        shl    edx, 2
        mov    edi, dword ptr [esp + 0x20]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        cmp    byte ptr [esp + 0x68], al
        je     loc_1_6468
        mov    dword ptr [esp + 0x40], eax
        sub    edi, edi
        jmp    loc_1_6479
        db  08Dh, 049h, 000h
loc_1_6468:
        mov    eax, dword ptr [esp + 0x48]
        mov    ecx, dword ptr [eax + 4]
        and    ecx, dword ptr [eax + 8]
        mov    dword ptr [esp + 0x40], ecx
        mov    edi, dword ptr [eax + 0xc]
loc_1_6479:
        mov    esi, dword ptr [0xc090]       ; -> obj1:0xC090
        mov    dword ptr [esp + 0x34], 0
        mov    dword ptr [esp + 0x10], 0
        or     esi, esi
        je     loc_1_657B
        nop    
loc_1_6498:
        test   byte ptr [esi + 0x14], 1
        je     loc_1_6571
        cmp    esi, dword ptr [0xc050]       ; -> obj1:0xC050
        je     loc_1_6571
        mov    ebx, dword ptr [esi + 4]
        and    ebx, dword ptr [esi + 8]
        cmp    dword ptr [esp + 0x40], ebx
        je     loc_1_6571
        cmp    dword ptr [esi + 0xc], edi
        je     loc_1_6571
        push   esi
        push   dword ptr [esp + 0x50]
        call   sub_1_6FA4
        add    esp, 8
        test   al, 1
        je     loc_1_6571
        sub    ecx, ecx
        cmp    ecx, dword ptr [esp + 0x10]
        je     loc_1_653D
        mov    eax, dword ptr [esp + 0x20]
        inc    edx
        mov    edx, dword ptr [esi + 8]
loc_1_64EC:
        mov    ebp, dword ptr [eax]
        and    ebp, edx
        cmp    ebp, ebx
        je     loc_1_6508
        mov    ebp, dword ptr [esi + 0xc]
        cmp    dword ptr [eax + 4], ebp
        je     loc_1_6508
        add    eax, 0xc
        inc    ecx
        cmp    ecx, dword ptr [esp + 0x10]
        jb     loc_1_64EC
        jmp    loc_1_6537
loc_1_6508:
        lea    eax, [ecx + ecx*2]
        mov    edx, dword ptr [esp + 0x20]
        lea    ebx, [edx + eax*4 + 8]
        movzx  eax, word ptr [esi + 0x3c]
        mov    edx, dword ptr [ebx]
        sub    eax, edx
        sbb    ebp, ebp
        and    eax, ebp
        add    eax, edx
        mov    dword ptr [ebx], eax
        mov    edx, dword ptr [esp + 0x5c]
        add    edx, dword ptr [esp + 0x64]
        cmp    edx, eax
        jbe    loc_1_6537
        mov    dword ptr [esp + 0x34], 1
loc_1_6537:
        cmp    ecx, dword ptr [esp + 0x10]
        jne    loc_1_6571
loc_1_653D:
        lea    eax, [ecx + ecx*2]
        mov    edx, dword ptr [esp + 0x20]
        lea    ecx, [edx + eax*4]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ecx + 4], eax
        movzx  eax, word ptr [esi + 0x3c]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [esp + 0x5c]
        add    ecx, dword ptr [esp + 0x64]
        cmp    ecx, eax
        jbe    loc_1_656D
        mov    dword ptr [esp + 0x34], 1
loc_1_656D:
        inc    dword ptr [esp + 0x10]
loc_1_6571:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_6498
loc_1_657B:
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_6598
        push   dword ptr [esp + 0x20]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
loc_1_6598:
        cmp    dword ptr [esp + 0x10], 1
        ja     loc_1_65BC
        cmp    dword ptr [esp + 0x34], 0
        jne    loc_1_65BC
        mov    dword ptr [esp + 0x38], 0
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [eax + 8], 0
        jmp    loc_1_6600
        db  090h
loc_1_65BC:
        push   8
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 0x38], eax
        or     eax, eax
        jne    loc_1_65E8
        push   dword ptr [esp + 0x20]
        call   sub_1_B944
        add    esp, 4
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  090h
loc_1_65E8:
        mov    eax, dword ptr [esp + 0x58]
        mov    ecx, dword ptr [esp + 0x38]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], 0
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [eax + 8], ecx
loc_1_6600:
        mov    dword ptr [esp + 0x28], 0
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_67D8
        mov    ebx, dword ptr [esp + 0x20]
        add    ebx, 8
loc_1_661A:
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esp + 0x2c], eax
        cmp    eax, 1
        je     loc_1_6714
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_5B88
        add    esp, 4
        mov    dword ptr [esp + 0x1c], eax
        or     eax, eax
        jne    loc_1_6650
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        jmp    loc_1_67C3
        db  090h
loc_1_6650:
        push   dword ptr [esp + 0x5c]
        push   0
        push   dword ptr [esp + 0x60]
        call   sub_1_5DD8
        add    esp, 0xc
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        jne    loc_1_6684
        push   dword ptr [esp + 0x14]
        call   sub_1_58C8
        add    esp, 4
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        jmp    loc_1_67C3
        db  08Dh, 049h, 000h
loc_1_6684:
        mov    ecx, 5
        mov    edi, dword ptr [esp + 0x14]
        add    edi, 0x38
        mov    esi, dword ptr [esp + 0x3c]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [esp + 0x54]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x14]
        or     byte ptr [eax + 0x3d], 2
        mov    eax, dword ptr [esp + 0x14]
        and    byte ptr [eax + 0x3d], 0xf7
        cmp    dword ptr [esp + 0x60], 0
        je     loc_1_6710
        push   dword ptr [esp + 0x64]
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_66F4
        push   dword ptr [esp + 0x18]
        call   sub_1_589C
        add    esp, 4
        push   dword ptr [esp + 0x14]
        call   sub_1_58C8
        add    esp, 4
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        jmp    loc_1_67C3
        db  08Dh, 049h, 000h
loc_1_66F4:
        mov    edi, ebp
        mov    eax, dword ptr [esp + 0x64]
        mov    esi, dword ptr [esp + 0x60]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_6730
        db  08Bh, 0FFh
loc_1_6710:
        sub    ebp, ebp
        jmp    loc_1_6730
loc_1_6714:
        mov    eax, dword ptr [esp + 0x54]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x50]
        mov    dword ptr [esp + 0x14], eax
        mov    ebp, dword ptr [esp + 0x60]
        mov    eax, dword ptr [esp + 0x58]
        mov    dword ptr [esp + 0x18], eax
loc_1_6730:
        push   dword ptr [ebx - 8]
        push   dword ptr [esp + 0x70]
        push   ebp
        call   sub_1_6264
        add    esp, 0xc
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [esp + 0x5c]
        add    eax, dword ptr [esp + 0x64]
        cmp    eax, ecx
        jbe    loc_1_6794
        lea    eax, [esp + 0x30]
        push   eax
        push   dword ptr [esp + 0x68]
        push   ebp
        push   dword ptr [esp + 0x68]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x68]
        push   ecx
        push   dword ptr [ebx - 4]
        call   sub_1_5E80
        add    esp, 0x28
        mov    dword ptr [esp + 0x24], eax
        cmp    dword ptr [esp + 0x2c], 1
        je     loc_1_67C3
        push   dword ptr [esp + 0x18]
        call   sub_1_589C
        add    esp, 4
        jmp    loc_1_67C3
        db  08Dh, 049h, 000h
loc_1_6794:
        push   dword ptr [esp + 0x64]
        push   ebp
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x60]
        push   dword ptr [ebx - 4]
        call   sub_1_5C7C
        add    esp, 0x1c
        mov    dword ptr [esp + 0x24], eax
        cmp    eax, 0x2bf7
        jne    loc_1_67C3
        inc    dword ptr [esp + 0x30]
loc_1_67C3:
        add    ebx, 0xc
        inc    dword ptr [esp + 0x28]
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x28], eax
        jb     loc_1_661A
loc_1_67D8:
        cmp    dword ptr [esp + 0x38], 0
        je     loc_1_6830
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x3c]
        call   sub_1_5BC8
        add    esp, 8
        or     eax, eax
        push   dword ptr [esp + 0x20]
        jne    loc_1_6818
        call   sub_1_B944
        add    esp, 4
        push   dword ptr [esp + 0x38]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Dh, 049h, 000h
loc_1_6818:
        call   sub_1_B944
        add    esp, 4
        mov    eax, 0x2bf7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Dh, 049h, 000h
loc_1_6830:
        push   dword ptr [esp + 0x20]
        call   sub_1_B944
        add    esp, 4
        mov    eax, dword ptr [esp + 0x24]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
sub_1_6848:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        inc    dword ptr [0xc1fc]            ; -> loc_1_C1FC
        call   sub_1_5B4C
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_6CE8
        lea    ecx, [ebp + 0x40]
        mov    dword ptr [esp + 0x28], ecx
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [ebp + 0x44], eax
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [ebp + 0x48], eax
        mov    esi, dword ptr [esp + 0x4c]
        or     esi, esi
        je     loc_1_69F0
        cmp    byte ptr [esi + 0x10], 0
        je     loc_1_69C4
        inc    dword ptr [esi + 0x14]
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [ecx + 0xc]
        cmp    eax, -1
        jne    loc_1_68A7
        mov    eax, dword ptr [esp + 0x40]
loc_1_68A7:
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ecx + 0x18]
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [esp + 0x48]
        cmp    dword ptr [eax], 0
        jne    loc_1_69B8
        cmp    byte ptr [esi + 0x11], 1
        je     loc_1_69B8
        mov    ecx, dword ptr [esp + 0x3c]
        cmp    dword ptr [esp + 0x24], ecx
        jb     loc_1_69B8
        mov    ecx, dword ptr [esp + 0x38]
        add    dword ptr [ecx + 0xc], 0x14
        push   ecx
        push   ebp
        call   sub_1_BA56
        mov    ecx, dword ptr [esp + 0x38]
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [esp + 0x30]
        mov    al, byte ptr [eax + 0x10]
        mov    byte ptr [edi + 9], al
        mov    word ptr [edi + 0xa], 0
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [edi + 0x10], eax
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [esp + 0x48]
        mov    al, byte ptr [eax + 9]
        mov    byte ptr [edi + 8], al
        mov    eax, dword ptr [esp + 0x48]
        mov    al, byte ptr [eax + 0xa]
        mov    byte ptr [edi + 1], al
        mov    eax, dword ptr [esp + 0x48]
        movzx  eax, byte ptr [eax + 0xb]
        shl    eax, 0xd
        shr    eax, 8
        and    ax, 0x40
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [0xc08c]       ; -> loc_1_C08C
        mov    word ptr [edi + 4], ax
        inc    dword ptr [0xc08c]            ; -> loc_1_C08C
        mov    byte ptr [edi], 0x45
        mov    eax, dword ptr [esp + 0x3c]
        add    eax, 0x14
        mov    al, ah
        mov    ecx, dword ptr [esp + 0x3c]
        add    cx, 0x14
        mov    ah, cl
        mov    word ptr [edi + 2], ax
        push   0x14
        push   edi
        call   sub_1_5C30
        add    esp, 8
        not    ax
        mov    word ptr [edi + 0xa], ax
        push   esi
        push   dword ptr [esp + 0x18]
        push   ebp
        mov    eax, dword ptr [esp + 0x2c]
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 8]
        add    esp, 0x10
        dec    dword ptr [esi + 0x14]
        cmp    eax, 0x103
        je     loc_1_69A8
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_69A8:
        pop    ebp
        mov    eax, 0x2bf7
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_69B8:
        dec    dword ptr [esi + 0x14]
        mov    al, byte ptr [esi + 0x11]
        mov    byte ptr [esp + 0x13], al
        jmp    loc_1_69F8
loc_1_69C4:
        push   esi
        call   sub_1_783C
        add    esp, 4
        or     eax, eax
        jne    loc_1_69F0
        inc    dword ptr [0xc208]            ; -> obj1:0xC208
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        mov    eax, 0x2afb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_69F0:
        mov    dword ptr [esp + 0x14], 0
loc_1_69F8:
        call   sub_1_5B68
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_6A20
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_6A20:
        mov    ecx, dword ptr [esp + 0x38]
        mov    edi, dword ptr [ecx + 4]
        lea    esi, [edi + 0x14]
        mov    eax, dword ptr [ecx + 0xc]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   ebx
        push   ebp
        call   sub_1_BA56
        mov    ebx, dword ptr [ebx + 4]
        mov    eax, dword ptr [esp + 0x30]
        mov    al, byte ptr [eax + 0x10]
        mov    byte ptr [ebx + 9], al
        mov    word ptr [ebx + 0xa], 0
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [ebx + 0x10], eax
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [esp + 0x48]
        mov    al, byte ptr [eax + 9]
        mov    byte ptr [ebx + 8], al
        mov    eax, dword ptr [esp + 0x48]
        mov    al, byte ptr [eax + 0xa]
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [esp + 0x48]
        movzx  eax, byte ptr [eax + 0xb]
        shl    eax, 0xd
        shr    eax, 8
        and    ax, 0x40
        mov    word ptr [ebx + 6], ax
        mov    eax, dword ptr [0xc08c]       ; -> loc_1_C08C
        mov    word ptr [ebx + 4], ax
        inc    dword ptr [0xc08c]            ; -> loc_1_C08C
        or     byte ptr [ebp + 0x3d], 0x10
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_6B08
        mov    eax, dword ptr [esp + 0x48]
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_1_6AB9
        mov    dword ptr [esp + 0x40], eax
        mov    dword ptr [ebx + 0x10], eax
loc_1_6AB9:
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x4c]
        push   dword ptr [esp + 0x4c]
        call   sub_1_7500
        add    esp, 0x10
        mov    dword ptr [esp + 0x20], eax
        or     eax, eax
        jne    loc_1_6AF8
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        inc    dword ptr [0xc208]            ; -> obj1:0xC208
        mov    eax, 0x2afb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_6AF8:
        push   dword ptr [esp + 0x40]
        call   sub_1_7044
        add    esp, 4
        mov    byte ptr [esp + 0x13], al
loc_1_6B08:
        mov    eax, dword ptr [esp + 0x48]
        cmp    dword ptr [eax], 0
        je     loc_1_6BA0
        test   byte ptr [eax + 0xb], 0x80
        je     loc_1_6B44
        mov    eax, dword ptr [esp + 0x40]
        cmp    dword ptr [esp + 0x14], eax
        je     loc_1_6B44
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        inc    dword ptr [0xc208]            ; -> obj1:0xC208
        mov    eax, 0x2afb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_6B44:
        mov    eax, dword ptr [esp + 0x48]
        movzx  eax, byte ptr [eax + 8]
        mov    dword ptr [esp + 0x18], eax
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 0x1c], eax
        or     eax, eax
        jne    loc_1_6B80
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_6B80:
        mov    edx, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x48]
        mov    esi, dword ptr [eax]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_6BB0
        db  08Bh, 0FFh
loc_1_6BA0:
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x18], 0
loc_1_6BB0:
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x14
        shr    eax, 2
        add    al, 0x40
        mov    byte ptr [ebx], al
        mov    edx, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [esp + 0x3c]
        add    edx, dword ptr [esp + 0x18]
        add    ax, word ptr [esp + 0x18]
        add    ax, 0x14
        mov    ah, al
        lea    ecx, [edx + 0x14]
        mov    al, ch
        cmp    byte ptr [esp + 0x13], 1
        mov    word ptr [ebx + 2], ax
        jne    loc_1_6C34
        cmp    dword ptr [esp + 0x44], 0
        jne    loc_1_6C08
        push   ebx
        push   dword ptr [esp + 0x3c]
        push   ebp
        push   dword ptr [esp + 0x4c]
        call   sub_1_5D88
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_6C08:
        push   0
        push   1
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x4c]
        push   dword ptr [esp + 0x4c]
        push   ebx
        push   ebp
        push   dword ptr [esp + 0x60]
        push   0
        call   sub_1_63F4
        add    esp, 0x28
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_6C34:
        cmp    dword ptr [esp + 0x24], edx
        jae    loc_1_6CC0
        push   8
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        jne    loc_1_6C7C
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_6C5F
        push   dword ptr [esp + 0x1c]
        call   sub_1_B944
        add    esp, 4
loc_1_6C5F:
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_6C7C:
        push   0
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], 0
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [ecx], eax
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x48]
        push   dword ptr [esp + 0x48]
        push   ebx
        push   ebp
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x44]
        call   sub_1_5E80
        add    esp, 0x28
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_6CC0:
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x20]
        push   ebx
        push   dword ptr [esp + 0x44]
        push   ebp
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x38]
        call   sub_1_5C7C
        add    esp, 0x1c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_6CE8:
        pop    ebp
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        mov    eax, 0x2afe
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
sub_1_6CFC:
        push   ebx
        push   esi
        sub    eax, eax
        sub    edx, edx
        mov    ecx, dword ptr [0xc068]       ; -> loc_1_C068
        cmp    ecx, eax
        jle    loc_1_6D32
        mov    esi, 0xc00c                   ; -> obj1:0xC00C
        mov    bl, byte ptr [esp + 0xc]
loc_1_6D15:
        cmp    byte ptr [esi], bl
        je     loc_1_6D24
        add    esi, 0x14
        inc    edx
        cmp    edx, ecx
        jl     loc_1_6D15
        pop    esi
        pop    ebx
        ret    
loc_1_6D24:
        lea    eax, [edx*4]
        mov    eax, dword ptr [eax + eax*4 + 0xc004] ; -> obj1:0xC004
loc_1_6D32:
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_6D38:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x1c]
        call   sub_1_6CFC
        add    esp, 4
        or     eax, eax
        je     loc_1_6D70
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x28]
        push   0
        mov    edi, dword ptr [esp + 0x24]
        push   edi
        mov    esi, dword ptr [esp + 0x24]
        push   esi
        push   0x2b0c
        push   0
        call   eax
        add    esp, 0x1c
        jmp    loc_1_6D78
        db  08Bh, 0FFh
loc_1_6D70:
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x18]
loc_1_6D78:
        sub    ebx, ebx
        cmp    dword ptr [0xc068], ebx       ; -> loc_1_C068
        jle    loc_1_6DB3
        mov    ebp, 0xc004                   ; -> obj1:0xC004
        nop    
loc_1_6D88:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     loc_1_6DA7
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x28]
        push   0
        push   edi
        push   esi
        push   0x2b0d
        push   1
        call   eax
        add    esp, 0x1c
loc_1_6DA7:
        inc    ebx
        add    ebp, 0x14
        cmp    ebx, dword ptr [0xc068]       ; -> loc_1_C068
        jl     loc_1_6D88
loc_1_6DB3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_6DB8:
        push   esi
        push   edi
        sub    edi, edi
        cmp    dword ptr [0xc068], edi       ; -> loc_1_C068
        jle    loc_1_6DF1
        mov    esi, 0xc004                   ; -> obj1:0xC004
loc_1_6DC9:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_6DE5
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0x2b0e
        push   1
        call   eax
        add    esp, 0x1c
loc_1_6DE5:
        inc    edi
        add    esi, 0x14
        cmp    edi, dword ptr [0xc068]       ; -> loc_1_C068
        jl     loc_1_6DC9
loc_1_6DF1:
        pop    edi
        pop    esi
        ret    
sub_1_6DF4:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, 1
        je     loc_1_6E10
        cmp    eax, 2
        je     loc_1_6E60
        cmp    eax, 3
        je     loc_1_6E7C
        pop    esi
        add    esp, 4
        ret    
loc_1_6E10:
        cmp    dword ptr [esp + 0x18], 4
        jb     loc_1_6EC5
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [eax + 0xc]
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx + 0x40], eax
        sub    eax, 0x14
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [edx + 0x38]
        or     eax, eax
        je     loc_1_6E48
loc_1_6E39:
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [eax + 0x3c], cx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_6E39
loc_1_6E48:
        lea    eax, [esp + 4]
        push   eax
        push   edx
        push   0x77b0                        ; -> loc_1_77B0
        call   sub_1_77EC
        add    esp, 0xc
        pop    esi
        add    esp, 4
        ret    
loc_1_6E60:
        cmp    dword ptr [esp + 0x18], 4
        jb     loc_1_6EC5
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [eax + 0x44], ecx
        pop    esi
        add    esp, 4
        ret    
loc_1_6E7C:
        cmp    dword ptr [esp + 0x18], 8
        jb     loc_1_6EC5
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esi]
        sub    eax, 0x14
        mov    dword ptr [esp + 4], eax
        add    esi, 4
        lea    eax, [esp + 4]
        push   eax
        push   esi
        push   0x77cc                        ; -> loc_1_77CC
        call   sub_1_77EC
        add    esp, 0xc
        push   1
        push   2
        push   1
        push   dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x1c]
        push   dword ptr [eax + 0xc]
        push   -1
        push   -1
        push   dword ptr [esi]
        call   sub_1_7F60
        add    esp, 0x20
loc_1_6EC5:
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_6ECC:
        mov    edx, dword ptr [esp + 8]
        cmp    edx, -1
        je     loc_1_6EFC
        or     edx, edx
        je     loc_1_6EFC
        mov    al, byte ptr [esp + 4]
        shr    al, 2
        and    eax, 0xff
        and    eax, 0xfffffffc
        mov    eax, dword ptr [eax + 0xbd00] ; -> loc_1_BD00
        mov    ecx, eax
        and    ecx, edx
        cmp    ecx, eax
        je     loc_1_6EFE
        sub    eax, eax
        ret    
        db  08Dh, 049h, 000h
loc_1_6EFC:
        mov    eax, edx
loc_1_6EFE:
        ret    
        db  090h
sub_1_6F00:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x38]
        or     ecx, ecx
        je     loc_1_6F22
        inc    eax
        mov    eax, dword ptr [esp + 8]
loc_1_6F10:
        test   byte ptr [ecx + 0x14], 1
        je     loc_1_6F1B
        cmp    dword ptr [ecx + 4], eax
        je     loc_1_6F28
loc_1_6F1B:
        mov    ecx, dword ptr [ecx + 0x10]
        or     ecx, ecx
        jne    loc_1_6F10
loc_1_6F22:
        sub    eax, eax
        ret    
        db  08Dh, 049h, 000h
loc_1_6F28:
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
sub_1_6F30:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0x38]
        or     eax, eax
        je     loc_1_6F7C
        sub    ecx, ecx
        mov    esi, dword ptr [esp + 0x10]
loc_1_6F44:
        test   byte ptr [eax + 0x14], 1
        je     loc_1_6F60
        mov    edx, dword ptr [eax + 8]
        mov    edi, dword ptr [eax + 4]
        and    edi, edx
        mov    ebx, esi
        and    ebx, edx
        cmp    edi, ebx
        je     loc_1_6F9E
        or     ecx, ecx
        jne    loc_1_6F60
        mov    ecx, eax
loc_1_6F60:
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_6F44
        or     ecx, ecx
        jne    loc_1_6F74
        cmp    esi, -1
        je     loc_1_6F7C
        or     esi, esi
        je     loc_1_6F7C
loc_1_6F74:
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_6F7C:
        mov    eax, dword ptr [0xc090]       ; -> obj1:0xC090
        or     eax, eax
        je     loc_1_6F9C
        inc    ecx
        mov    ecx, dword ptr [0xc050]       ; -> obj1:0xC050
loc_1_6F8C:
        cmp    ecx, eax
        je     loc_1_6F96
        test   byte ptr [eax + 0x14], 1
        jne    loc_1_6F9E
loc_1_6F96:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_6F8C
loc_1_6F9C:
        sub    eax, eax
loc_1_6F9E:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_6FA4:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi + 0xc]
        mov    edx, dword ptr [eax + 0x3c]
        test   byte ptr [edi + 0x14], 1
        je     loc_1_7030
        mov    eax, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 4]
        mov    ecx, eax
        not    ecx
        and    ecx, edx
        and    eax, esi
        or     ecx, eax
        cmp    ecx, dword ptr [esp + 0xc]
        jne    loc_1_6FD4
        mov    al, 3
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_6FD4:
        mov    al, byte ptr [esp + 0xc]
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_7008
        mov    al, byte ptr [esp + 0xc]
        shr    al, 2
        and    eax, 0xff
        and    eax, 0xfffffffc
        mov    eax, dword ptr [eax + 0xbd00] ; -> loc_1_BD00
        mov    ecx, eax
        not    ecx
        and    ecx, edx
        and    eax, esi
        or     ecx, eax
        cmp    ecx, dword ptr [esp + 0xc]
        jne    loc_1_7030
        mov    al, 1
        pop    edi
        pop    esi
        ret    
loc_1_7008:
        cmp    dword ptr [0xc2f8], 2         ; -> loc_1_C2F8
        jne    loc_1_7030
        mov    eax, dword ptr [edi + 0x40]
        or     eax, eax
        je     loc_1_7030
loc_1_7018:
        mov    ecx, dword ptr [esp + 0xc]
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_7027
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_7018
loc_1_7027:
        or     eax, eax
        je     loc_1_7030
        mov    al, 5
        pop    edi
        pop    esi
        ret    
loc_1_7030:
        cmp    edx, dword ptr [esp + 0xc]
        jne    loc_1_703C
        mov    al, 1
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_703C:
        sub    al, al
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_7044:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    al, byte ptr [ebp + 8]
        and    al, 0xf0
        cmp    al, 0xf0
        jne    loc_1_705C
        cmp    dword ptr [ebp + 8], -1
        je     loc_1_705C
        mov    al, 0xff
        pop    esi
        leave  
        ret    
loc_1_705C:
        mov    esi, dword ptr [0xc090]       ; -> obj1:0xC090
loc_1_7062:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 4], eax
        jne    loc_1_7070
        test   byte ptr [esi + 0x14], 1
        jne    loc_1_70B4
loc_1_7070:
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    loc_1_7126
        test   byte ptr [esi + 0x14], 1
        je     loc_1_70A0
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp + 8]
        and    eax, ecx
        mov    edx, dword ptr [esi + 4]
        and    edx, ecx
        cmp    edx, eax
        jne    loc_1_70A0
        cmp    dword ptr [ebp + 8], eax
        je     loc_1_70BC
loc_1_70A0:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_7062
        cmp    byte ptr [ebp + 8], 0x7f
        jne    loc_1_70C4
        sub    al, al
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_70B4:
        sub    al, al
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_70BC:
        pop    esi
        mov    al, 0xff
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_70C4:
        mov    al, byte ptr [ebp + 8]
        and    al, 0xf0
        cmp    al, 0xe0
        jne    loc_1_70E4
        cmp    dword ptr [0xc2f8], 0         ; -> loc_1_C2F8
        je     loc_1_70DC
        mov    al, 9
        pop    esi
        leave  
        ret    
        db  090h
loc_1_70DC:
        pop    esi
        mov    al, 0xff
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_70E4:
        mov    al, byte ptr [ebp + 8]
        shr    al, 2
        and    eax, 0xff
        and    eax, 0xfffffffc
        mov    ecx, dword ptr [eax + 0xbd00] ; -> loc_1_BD00
        mov    eax, ecx
        not    eax
        or     eax, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ebp + 8]
        jne    loc_1_710C
        mov    al, 7
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_710C:
        mov    eax, dword ptr [ebp + 8]
        and    eax, ecx
        cmp    eax, dword ptr [ebp + 8]
        je     loc_1_7124
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_7124
        mov    al, 6
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_7124:
        mov    al, 0xff
loc_1_7126:
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
sub_1_712C:
        movzx  eax, byte ptr [esp + 6]
        movzx  ecx, byte ptr [esp + 7]
        add    eax, ecx
        movzx  ecx, byte ptr [esp + 5]
        add    eax, ecx
        movzx  ecx, byte ptr [esp + 4]
        add    eax, ecx
        and    eax, 0x1f
        ret    
        db  08Bh, 0FFh
sub_1_714C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esi]
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [ebx + 4], eax
        jne    loc_1_716C
        test   byte ptr [ebx + 0x14], 1
        je     loc_1_716C
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_716C:
        push   ebx
        push   dword ptr [esp + 0x18]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    loc_1_727A
        mov    ebp, dword ptr [ebx + 0xc]
        mov    edi, dword ptr [0xc090]       ; -> obj1:0xC090
loc_1_718A:
        cmp    ebx, edi
        je     loc_1_71B3
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [edi + 4], eax
        jne    loc_1_719D
        test   byte ptr [edi + 0x14], 1
        jne    loc_1_71D0
loc_1_719D:
        cmp    dword ptr [edi + 0xc], ebp
        jne    loc_1_71B3
        push   edi
        push   dword ptr [esp + 0x18]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    loc_1_71DC
loc_1_71B3:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_718A
        cmp    byte ptr [esp + 0x14], 0x7f
        jne    loc_1_71E4
        mov    eax, dword ptr [0xc050]       ; -> obj1:0xC050
        mov    dword ptr [esi], eax
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_71D0:
        mov    dword ptr [esi], edi
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_71DC:
        mov    dword ptr [esi], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_71E4:
        mov    cl, byte ptr [esp + 0x14]
        and    cl, 0xf0
        cmp    cl, 0xe0
        jne    loc_1_7208
        cmp    dword ptr [0xc2f8], 0         ; -> loc_1_C2F8
        je     loc_1_7200
        mov    al, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_7200:
        pop    ebp
        mov    al, 0xff
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_7208:
        mov    esi, dword ptr [esi]
        mov    al, byte ptr [esp + 0x14]
        shr    al, 2
        and    eax, 0xff
        and    eax, 0xfffffffc
        mov    edi, dword ptr [eax + 0xbd00] ; -> loc_1_BD00
        mov    edx, edi
        and    edx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esi + 0xc]
        mov    eax, dword ptr [eax + 0x3c]
        not    edi
        and    eax, edi
        or     eax, edx
        cmp    eax, dword ptr [esp + 0x14]
        jne    loc_1_7240
        mov    al, 7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_7240:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_7278
        cmp    dword ptr [esp + 0x14], edx
        je     loc_1_7278
        cmp    cl, 0xf0
        jne    loc_1_7259
        cmp    dword ptr [esp + 0x14], -1
        jne    loc_1_7278
loc_1_7259:
        mov    eax, dword ptr [0xbc04]       ; -> obj1:0xBC04
        or     eax, eax
        je     loc_1_7270
        cmp    eax, esi
        jne    loc_1_7270
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_7270:
        pop    ebp
        mov    al, 6
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_7278:
        mov    al, 0xff
loc_1_727A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_7280:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_1_6ECC
        add    esp, 8
        mov    ecx, esi
        and    ecx, eax
        push   ecx
        call   sub_1_712C
        add    esp, 4
        lea    ebx, [eax*4 + 0xc248]         ; -> obj1:0xC248
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     loc_1_72D4
        mov    ecx, dword ptr [esp + 0x18]
        inc    edx
        mov    edx, dword ptr [esp + 0x1c]
loc_1_72B8:
        cmp    dword ptr [eax + 4], esi
        jne    loc_1_72CC
        cmp    dword ptr [eax + 8], edi
        jne    loc_1_72CC
        cmp    dword ptr [eax + 0xc], ecx
        jne    loc_1_72CC
        cmp    dword ptr [eax + 0x1c], edx
        je     loc_1_72D4
loc_1_72CC:
        mov    ebx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_72B8
loc_1_72D4:
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [ecx], ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_72E0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x20]
        mov    esi, dword ptr [eax*4 + 0xc248] ; -> obj1:0xC248
        mov    eax, dword ptr [esp + 0x24]
loc_1_72F6:
        or     esi, esi
        je     loc_1_7304
        cmp    dword ptr [esi + 0x10], eax
        jbe    loc_1_7310
        mov    esi, dword ptr [esi]
        jmp    loc_1_72F6
        db  090h
loc_1_7304:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_7310:
        mov    edi, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0x10], 0
        mov    eax, dword ptr [esp + 0x28]
loc_1_7320:
        test   byte ptr [esi + 0x26], 1
        je     loc_1_7335
        cmp    dword ptr [esi + 0x10], eax
        jb     loc_1_7340
        mov    ecx, dword ptr [esi + 8]
        and    ecx, edi
        cmp    ecx, dword ptr [esi + 4]
        je     loc_1_734C
loc_1_7335:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_7320
        jmp    loc_1_73B0
        db  08Dh, 049h, 000h
loc_1_7340:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_734C:
        mov    dword ptr [esp + 0x10], esi
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_73B0
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esi + 0x1c]
        call   sub_1_6F00
        add    esp, 8
        or     eax, eax
        jne    loc_1_73B0
        mov    ebx, dword ptr [esi + 0x10]
        mov    ebp, dword ptr [esi + 0x14]
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     loc_1_73B0
loc_1_7376:
        cmp    dword ptr [esi + 0x10], ebx
        jne    loc_1_73B0
        cmp    dword ptr [esi + 0x14], ebp
        jne    loc_1_73B0
        test   byte ptr [esi + 0x26], 1
        je     loc_1_73A3
        mov    eax, dword ptr [esi + 8]
        and    eax, edi
        cmp    eax, dword ptr [esi + 4]
        jne    loc_1_73A3
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esi + 0x1c]
        call   sub_1_6F00
        add    esp, 8
        or     eax, eax
        jne    loc_1_73AC
loc_1_73A3:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_7376
        jmp    loc_1_73B0
        db  090h
loc_1_73AC:
        mov    dword ptr [esp + 0x10], esi
loc_1_73B0:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_73BC:
        push   esi
        push   edi
        sub    esi, esi
        call   sub_1_B92C
        mov    ecx, 0x3e8
        sub    edx, edx
        div    ecx
        mov    edi, eax
        push   esi
        call   sub_1_712C
        add    esp, 4
        mov    ecx, dword ptr [eax*4 + 0xc248] ; -> obj1:0xC248
        cmp    ecx, esi
        je     loc_1_7412
        mov    eax, dword ptr [esp + 0xc]
loc_1_73E8:
        cmp    dword ptr [ecx + 8], 0
        jne    loc_1_740C
        mov    dx, word ptr [ecx + 0x26]
        test   dl, 1
        jne    loc_1_740C
        or     eax, eax
        je     loc_1_7400
        cmp    dword ptr [ecx + 0xc], eax
        jne    loc_1_740C
loc_1_7400:
        inc    esi
        or     dx, 1
        mov    word ptr [ecx + 0x26], dx
        mov    dword ptr [ecx + 0x30], edi
loc_1_740C:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_73E8
loc_1_7412:
        mov    eax, esi
        add    dword ptr [0xc2c8], esi       ; -> loc_1_C2C8
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_7420:
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        cmp    esi, 0x20
        jne    loc_1_7450
        push   esi
        push   esi
        push   dword ptr [esp + 0x10]
        call   sub_1_712C
        add    esp, 4
        push   eax
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        call   sub_1_72E0
        add    esp, 0x14
        or     eax, eax
        je     loc_1_7450
        pop    esi
        ret    
        db  090h
loc_1_7450:
        or     esi, esi
        je     loc_1_748E
        push   1
        push   esi
        mov    al, byte ptr [esp + 0x10]
        shr    al, 2
        and    eax, 0xff
        and    eax, 0xfffffffc
        mov    eax, dword ptr [eax + 0xbd00] ; -> loc_1_BD00
        and    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_712C
        add    esp, 4
        push   eax
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        call   sub_1_72E0
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_74AC
loc_1_748E:
        push   0
        push   esi
        push   0
        call   sub_1_712C
        add    esp, 4
        push   eax
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        call   sub_1_72E0
        add    esp, 0x14
loc_1_74AC:
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_74B0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [ebx + 0x14]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_6ECC
        add    esp, 8
        mov    ecx, dword ptr [ebx + 4]
        and    ecx, eax
        push   ecx
        call   sub_1_712C
        add    esp, 4
        lea    eax, [eax*4 + 0xc248]         ; -> obj1:0xC248
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_74FC
loc_1_74E6:
        mov    edx, dword ptr [ecx + 0x10]
        cmp    esi, edx
        ja     loc_1_74FC
        jne    loc_1_74F4
        cmp    dword ptr [ecx + 0x14], edi
        ja     loc_1_74FC
loc_1_74F4:
        mov    eax, ecx
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_74E6
loc_1_74FC:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_7500:
        push   esi
        push   edi
        push   0x20
        push   dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_1_7420
        add    esp, 0xc
        or     eax, eax
        je     loc_1_753C
        mov    edi, dword ptr [eax + 0x1c]
        mov    edx, dword ptr [eax + 0xc]
        cmp    edx, -1
        mov    ecx, dword ptr [esp + 0x14]
        jne    loc_1_752C
        mov    dword ptr [ecx], esi
        jmp    loc_1_752E
loc_1_752C:
        mov    dword ptr [ecx], edx
loc_1_752E:
        mov    ecx, dword ptr [eax + 0x18]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
loc_1_753C:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_7544:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    ecx, 0x3e8
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 0x10], eax
        mov    esi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esi + 4]
        mov    ebp, dword ptr [ebx + 0x1c]
        mov    ecx, dword ptr [ebx + 4]
        mov    edi, dword ptr [esp + 0x1c]
        mov    dword ptr [edi], ecx
        mov    eax, dword ptr [ebp + 0x30]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 0xc], 0xffffffff
        mov    dword ptr [edi + 0x10], 0xffffffff
        mov    dword ptr [edi + 0x14], 0xffffffff
        mov    dword ptr [edi + 0x2c], 0xffffffff
        mov    eax, dword ptr [ebx + 0xc]
        cmp    eax, -1
        jne    loc_1_75BE
        push   ebp
        push   ecx
        call   sub_1_6F30
        add    esp, 8
        cmp    dword ptr [ebp + 0x38], 0
        je     loc_1_75BC
        or     eax, eax
        je     loc_1_75BC
        mov    eax, dword ptr [eax + 4]
        jmp    loc_1_75BE
        db  08Dh, 049h, 000h
loc_1_75BC:
        mov    eax, dword ptr [edi]
loc_1_75BE:
        mov    dword ptr [edi + 0x18], eax
        test   byte ptr [ebx + 0x26], 1
        je     loc_1_75D0
        movzx  eax, word ptr [ebx + 0x24]
        mov    dword ptr [edi + 0x1c], eax
        jmp    loc_1_75D7
loc_1_75D0:
        mov    dword ptr [edi + 0x1c], 2
loc_1_75D7:
        mov    eax, dword ptr [ebx + 0x2c]
        mov    dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, dword ptr [ebx + 0x30]
        mov    dword ptr [edi + 0x24], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 0x28], eax
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     loc_1_7604
        mov    dword ptr [esi + 4], eax
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_7604:
        mov    ecx, dword ptr [esi]
        inc    ecx
        cmp    ecx, 0x20
        jae    loc_1_7624
        inc    eax
        lea    eax, [ecx*4 + 0xc248]         ; -> obj1:0xC248
loc_1_7614:
        cmp    dword ptr [eax], 0
        jne    loc_1_763C
        add    eax, 4
        inc    ecx
        cmp    eax, 0xc2c8                   ; -> loc_1_C2C8
        jb     loc_1_7614
loc_1_7624:
        sub    eax, eax
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_763C:
        pop    ebp
        mov    eax, dword ptr [ecx*4 + 0xc248] ; -> obj1:0xC248
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi], ecx
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_7658:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    loc_1_76A4
        or     eax, eax
        jne    loc_1_76A4
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 1
        inc    eax
        lea    eax, [esi*4 + 0xc248]         ; -> obj1:0xC248
loc_1_767C:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jne    loc_1_7691
        add    eax, 4
        inc    esi
        cmp    eax, 0xc2c8                   ; -> loc_1_C2C8
        jb     loc_1_767C
        or     ecx, ecx
        je     loc_1_76A0
loc_1_7691:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_76A0:
        sub    eax, eax
        pop    esi
        ret    
loc_1_76A4:
        cmp    esi, 0x20
        jae    loc_1_76BE
        mov    ecx, dword ptr [esi*4 + 0xc248] ; -> obj1:0xC248
        or     ecx, ecx
        je     loc_1_76BE
loc_1_76B4:
        cmp    ecx, eax
        je     loc_1_76CC
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_76B4
loc_1_76BE:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        ret    
loc_1_76CC:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 1
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_76E0:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        mov    edi, dword ptr [eax + 0x1c]
        mov    esi, dword ptr [eax + 0x20]
        mov    dword ptr [eax + 0x20], 0
        or     esi, esi
        je     loc_1_771B
        nop    
loc_1_76F8:
        cmp    byte ptr [esi + 0x10], 0
        je     loc_1_7715
        mov    byte ptr [esi + 0x10], 0
        mov    dword ptr [esi + 4], edi
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_7715
        push   esi
        push   dword ptr [edi + 4]
        call   dword ptr [edi + 0x14]
        add    esp, 8
loc_1_7715:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_76F8
loc_1_771B:
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_7720:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        dec    dword ptr [0xc230]            ; -> loc_1_C230
        cmp    dword ptr [esi + 8], 0
        jne    loc_1_775E
        dec    dword ptr [0xc1cc]            ; -> obj1:0xC1CC
        test   byte ptr [esi + 0x26], 1
        je     loc_1_774B
        dec    dword ptr [0xc2c8]            ; -> loc_1_C2C8
loc_1_774B:
        cmp    dword ptr [0xc2c8], 0         ; -> loc_1_C2C8
        jne    loc_1_775E
        push   0
        call   sub_1_73BC
        add    esp, 4
loc_1_775E:
        push   esi
        call   sub_1_76E0
        add    esp, 4
        push   esi
        call   sub_1_B944
        add    esp, 4
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_7774:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [ecx + 0x1c], eax
        jne    loc_1_778C
        mov    eax, dword ptr [eax + 0x3c]
        cmp    dword ptr [ecx + 4], eax
        je     loc_1_778C
        sub    eax, eax
        ret    
loc_1_778C:
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
loc_1_7794:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [ecx + 0x1c], eax
        jne    loc_1_77AA
        push   ecx
        call   sub_1_76E0
        add    esp, 4
loc_1_77AA:
        mov    eax, 1
        ret    
loc_1_77B0:
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0x1c], ecx
        jne    loc_1_77C6
        mov    dword ptr [eax + 0x18], edx
loc_1_77C6:
        mov    eax, 1
        ret    
loc_1_77CC:
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0xc], ecx
        jne    loc_1_77E4
        mov    dword ptr [eax + 0x18], edx
loc_1_77E4:
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
sub_1_77EC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, 0xc248                   ; -> obj1:0xC248
        mov    edi, dword ptr [esp + 0x14]
loc_1_77F9:
        mov    ebp, esi
        mov    ebx, dword ptr [esi]
        or     ebx, ebx
        je     loc_1_7829
loc_1_7801:
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   edi
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_7820
        push   ebx
        push   ebp
        call   sub_1_7720
        add    esp, 8
        jmp    loc_1_7822
        db  090h
loc_1_7820:
        mov    ebp, ebx
loc_1_7822:
        mov    ebx, dword ptr [ebp]
        or     ebx, ebx
        jne    loc_1_7801
loc_1_7829:
        add    esi, 4
        cmp    esi, 0xc2c8                   ; -> loc_1_C2C8
        jb     loc_1_77F9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_783C:
        push   esi
        push   edi
        mov    eax, dword ptr [0xc090]       ; -> obj1:0xC090
        or     eax, eax
        mov    esi, dword ptr [esp + 0xc]
        je     loc_1_7860
        nop    
loc_1_784C:
        test   byte ptr [eax + 0x14], 1
        je     loc_1_785A
        mov    ecx, dword ptr [esi + 0xc]
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_7860
loc_1_785A:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_784C
loc_1_7860:
        or     eax, eax
        jne    loc_1_786C
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_786C:
        push   0x20
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        call   sub_1_7420
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        je     loc_1_78B4
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_78AA
        cmp    byte ptr [esi + 0x10], 0
        jne    loc_1_78AA
        mov    eax, dword ptr [esi + 4]
        push   esi
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 0x14]
        add    esp, 8
        mov    dword ptr [esi + 4], edi
        mov    byte ptr [esi + 0x10], 1
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [esi], eax
        mov    dword ptr [edi + 0x20], esi
loc_1_78AA:
        pop    edi
        mov    eax, 1
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_78B4:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_78BC:
        push   0x20
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_1_7420
        add    esp, 0xc
        or     eax, eax
        je     loc_1_78F4
        mov    ecx, dword ptr [esp + 0xc]
        or     ecx, ecx
        je     loc_1_78DF
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [ecx], edx
loc_1_78DF:
        mov    ecx, dword ptr [esp + 0x10]
        or     ecx, ecx
        je     loc_1_78EF
        mov    eax, dword ptr [eax + 0x1c]
        mov    eax, dword ptr [eax + 0x44]
        mov    dword ptr [ecx], eax
loc_1_78EF:
        sub    eax, eax
        ret    
        db  08Bh, 0FFh
loc_1_78F4:
        mov    eax, 0x2afb
        ret    
        db  08Bh, 0FFh
loc_1_78FC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    ecx, 0x3e8
        sub    edx, edx
        div    ecx
        mov    edi, eax
        cmp    dword ptr [0xc15c], 0         ; -> obj1:0xC15C
        je     loc_1_7A27
        push   0x20
        mov    esi, dword ptr [esp + 0x20]
        push   esi
        mov    ebp, dword ptr [esp + 0x20]
        push   ebp
        call   sub_1_7420
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_7A27
        mov    eax, edi
        sub    eax, dword ptr [ebx + 0x30]
        cmp    eax, 0x3c
        jbe    loc_1_7A27
        cmp    dword ptr [ebx + 0xc], -1
        je     loc_1_7A27
        cmp    dword ptr [ebx + 0x2c], 4
        jne    loc_1_79B4
        and    word ptr [ebx + 0x26], 0xfffe
        push   0x20
        push   esi
        push   ebp
        call   sub_1_7420
        add    esp, 0xc
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        jne    loc_1_7984
        or     word ptr [ebx + 0x26], 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_7984:
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [ebx + 0x1c]
        push   dword ptr [ebx + 0xc]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_7280
        add    esp, 0x14
        push   eax
        push   dword ptr [esp + 0x14]
        call   sub_1_7720
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_79B4:
        cmp    dword ptr [ebx + 8], 0
        jne    loc_1_7A27
        cmp    dword ptr [0xc1cc], 1         ; -> obj1:0xC1CC
        jbe    loc_1_7A27
        push   ebx
        call   sub_1_76E0
        add    esp, 4
        cmp    dword ptr [0xc2c8], 1         ; -> loc_1_C2C8
        jne    loc_1_79E8
        push   0
        call   sub_1_73BC
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_79E8:
        dec    dword ptr [0xc2c8]            ; -> loc_1_C2C8
        and    word ptr [ebx + 0x26], 0xfffe
        push   0
        push   0
        push   0
        call   sub_1_712C
        add    esp, 4
        push   eax
        push   esi
        push   ebp
        call   sub_1_72E0
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_7A24
        push   0
        call   sub_1_73BC
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_7A24:
        mov    dword ptr [eax + 0x30], edi
loc_1_7A27:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
sub_1_7A30:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x20]
        or     eax, eax
        je     loc_1_7A58
        mov    ecx, dword ptr [esp + 8]
        inc    edx
        mov    edx, dword ptr [esp + 0xc]
loc_1_7A44:
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_7A52
        or     edx, edx
        je     loc_1_7A58
        cmp    dword ptr [eax + 0xc], edx
        je     loc_1_7A58
loc_1_7A52:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_7A44
loc_1_7A58:
        ret    
        db  08Dh, 049h, 000h
loc_1_7A5C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x30]
        mov    esi, dword ptr [eax + 4]
        or     esi, esi
        jne    loc_1_7A72
        mov    esi, dword ptr [esp + 0x1c]
loc_1_7A72:
        cmp    dword ptr [0xbbfc], 0         ; -> obj1:0xBBFC
        je     loc_1_7A88
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_7A88:
        push   esi
        call   sub_1_7044
        add    esp, 4
        mov    byte ptr [esp + 0x13], al
        mov    edi, dword ptr [esp + 0x28]
        mov    byte ptr [edi], al
        cmp    al, 0xff
        je     loc_1_7BE0
        mov    ebx, dword ptr [esp + 0x20]
        or     ebx, ebx
        je     loc_1_7AD4
        mov    eax, dword ptr [0xc090]       ; -> obj1:0xC090
        or     eax, eax
        je     loc_1_7AC9
loc_1_7AB4:
        test   byte ptr [eax + 0x14], 1
        je     loc_1_7ABF
        cmp    dword ptr [eax + 4], ebx
        je     loc_1_7AC5
loc_1_7ABF:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_7AB4
loc_1_7AC5:
        or     eax, eax
        jne    loc_1_7AD4
loc_1_7AC9:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_7AD4:
        push   0x20
        push   ebx
        push   esi
        call   sub_1_7420
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_7BD4
        mov    ecx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [esp + 0x2c]
        mov    word ptr [eax], cx
        cmp    bl, 0x7f
        jne    loc_1_7B0C
        cmp    dword ptr [ebp + 0x1c], 0xc0b8 ; -> obj1:0xC0B8
        je     loc_1_7B0C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_7B0C:
        cmp    dword ptr [ebp + 0xc], -1
        je     loc_1_7B15
        or     byte ptr [edi], 0x10
loc_1_7B15:
        push   ebx
        push   esi
        push   ebp
        call   sub_1_7A30
        add    esp, 0xc
        mov    ecx, eax
        or     ecx, ecx
        jne    loc_1_7BBC
        cmp    byte ptr [esp + 0x13], 0
        jne    loc_1_7B38
        mov    dword ptr [esp + 0x14], esi
        jmp    loc_1_7B5B
        db  090h
loc_1_7B38:
        push   dword ptr [ebp + 0x1c]
        push   esi
        call   sub_1_6F30
        add    esp, 8
        or     eax, eax
        jne    loc_1_7B54
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_7B54:
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 0x14], eax
loc_1_7B5B:
        push   0x20
        call   sub_1_B93E
        add    esp, 4
        mov    edx, eax
        mov    edi, dword ptr [esp + 0x24]
        mov    dword ptr [edi], edx
        or     edx, edx
        je     loc_1_7BAE
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        cmp    ebx, eax
        jne    loc_1_7B8C
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0xc], eax
        jmp    loc_1_7B8F
        db  08Dh, 049h, 000h
loc_1_7B8C:
        mov    dword ptr [edx + 0xc], ebx
loc_1_7B8F:
        mov    dword ptr [edx + 8], esi
        mov    al, byte ptr [esp + 0x13]
        mov    byte ptr [edx + 0x11], al
        mov    word ptr [edx + 0x12], 1
        mov    dword ptr [edx + 4], ebp
        mov    byte ptr [edx + 0x10], 1
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [edx], eax
        mov    dword ptr [ebp + 0x20], edx
loc_1_7BAE:
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_7BBC:
        mov    eax, dword ptr [ecx + 0xc]
        inc    word ptr [ecx + 0x12]
        mov    edi, dword ptr [esp + 0x24]
        mov    dword ptr [edi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_7BD4:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_7BE0:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_7BEC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        or     esi, esi
        je     loc_1_7C3A
        mov    ax, word ptr [esi + 0x12]
        dec    ax
        mov    word ptr [esi + 0x12], ax
        jne    loc_1_7C3A
        cmp    byte ptr [esi + 0x10], 0
        je     loc_1_7C24
        mov    ecx, dword ptr [esi + 4]
        mov    edx, dword ptr [ecx + 0x1c]
        add    ecx, 0x20
        je     loc_1_7C1E
loc_1_7C12:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        je     loc_1_7C1E
        mov    ecx, eax
        or     ecx, ecx
        jne    loc_1_7C12
loc_1_7C1E:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        jmp    loc_1_7C27
loc_1_7C24:
        mov    edx, dword ptr [esi + 4]
loc_1_7C27:
        push   esi
        push   dword ptr [edx + 4]
        call   dword ptr [edx + 0x14]
        add    esp, 8
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_7C3A:
        pop    esi
        ret    
sub_1_7C3C:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    ecx, 0x3e8
        sub    edx, edx
        div    ecx
        mov    ebx, eax
        mov    eax, dword ptr [esp + 0x30]
        and    eax, dword ptr [esp + 0x2c]
        cmp    eax, dword ptr [esp + 0x2c]
        je     loc_1_7C70
        mov    eax, 0x2b0a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_7C70:
        cmp    dword ptr [esp + 0x48], 0
        je     loc_1_7C94
        cmp    dword ptr [esp + 0x48], 1
        je     loc_1_7C94
        cmp    dword ptr [esp + 0x48], 2
        je     loc_1_7C94
        mov    eax, 0x2b03
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_7C94:
        mov    eax, 0
        mov    edi, dword ptr [esp + 0x34]
        cmp    edi, -1
        setne  al
        add    eax, 3
        mov    dword ptr [esp + 0x24], eax
        mov    ebp, dword ptr [esp + 0x3c]
        sub    ebp, 0x44
        sbb    eax, eax
        not    eax
        and    ebp, eax
        add    ebp, 0x44
        mov    ecx, dword ptr [esp + 0x38]
        cmp    dword ptr [ecx + 0x34], 0
        jne    loc_1_7CE0
        cmp    dword ptr [ecx + 0x38], 0
        je     loc_1_7CE0
        mov    eax, dword ptr [esp + 0x2c]
        cmp    dword ptr [ecx + 0x3c], eax
        je     loc_1_7CE0
        mov    eax, 0x2b03
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
loc_1_7CE0:
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x3c]
        push   edi
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x3c]
        call   sub_1_7280
        add    esp, 0x14
        mov    esi, eax
        or     esi, esi
        je     loc_1_7D84
        mov    dword ptr [esi + 0x18], ebp
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [esi + 0x2c], eax
        mov    dword ptr [esi + 0x30], ebx
        mov    dword ptr [esi + 0x34], ebx
        mov    bx, word ptr [esi + 0x26]
        mov    ax, bx
        and    ax, 0xfffd
        or     ax, 1
        cmp    dword ptr [esi + 8], 0
        mov    word ptr [esi + 0x26], ax
        jne    loc_1_7D38
        test   bl, 1
        jne    loc_1_7D38
        inc    dword ptr [0xc2c8]            ; -> loc_1_C2C8
loc_1_7D38:
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [esi + 0x14]
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [ecx], eax
        mov    edi, dword ptr [esp + 0x40]
        mov    dword ptr [esi + 0x14], edi
        cmp    edi, edx
        jb     loc_1_7D78
        test   bl, 1
        je     loc_1_7D78
        push   esi
        call   sub_1_74B0
        add    esp, 4
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    dword ptr [eax], esi
        push   esi
        call   sub_1_76E0
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
loc_1_7D78:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [esp + 0x18], eax
        jmp    loc_1_7E2C
loc_1_7D84:
        push   0x38
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_7DA4
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
loc_1_7DA4:
        inc    dword ptr [0xc230]            ; -> loc_1_C230
        mov    dword ptr [esp + 0x18], 0
        mov    eax, dword ptr [esp + 0x30]
        or     eax, eax
        je     loc_1_7DC9
loc_1_7DBA:
        mov    ecx, eax
        and    ecx, 1
        add    dword ptr [esp + 0x18], ecx
        shr    eax, 1
        or     eax, eax
        jne    loc_1_7DBA
loc_1_7DC9:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [esi + 8], eax
        or     eax, eax
        jne    loc_1_7DEE
        inc    dword ptr [0xc1cc]            ; -> obj1:0xC1CC
        inc    dword ptr [0xc2c8]            ; -> loc_1_C2C8
loc_1_7DEE:
        mov    dword ptr [esi + 0xc], edi
        mov    edi, dword ptr [esp + 0x40]
        mov    dword ptr [esi + 0x14], edi
        mov    dword ptr [esi + 0x18], ebp
        mov    ecx, dword ptr [esp + 0x38]
        mov    dword ptr [esi + 0x1c], ecx
        mov    dword ptr [esi + 0x20], 0
        mov    ax, word ptr [esp + 0x24]
        mov    word ptr [esi + 0x24], ax
        mov    word ptr [esi + 0x26], 1
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [esi + 0x2c], eax
        mov    dword ptr [esi + 0x30], ebx
        mov    dword ptr [esi + 0x34], ebx
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [esi + 0x28], eax
loc_1_7E2C:
        push   esi
        call   sub_1_74B0
        add    esp, 4
        mov    dword ptr [esp + 0x20], eax
        push   dword ptr [esp + 0x18]
        push   0
        push   dword ptr [esp + 0x34]
        call   sub_1_7420
        add    esp, 0xc
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_7F45
        mov    edx, eax
        mov    eax, dword ptr [edx + 0x14]
        mov    dword ptr [esp + 0x1c], eax
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edx + 0x10]
        cmp    dword ptr [esp + 0x18], eax
        ja     loc_1_7E8A
        cmp    edi, dword ptr [esp + 0x1c]
        jb     loc_1_7E86
        jne    loc_1_7F45
        mov    ecx, dword ptr [esp + 0x38]
        cmp    dword ptr [edx + 0x1c], ecx
        je     loc_1_7F45
loc_1_7E86:
        cmp    dword ptr [esp + 0x18], eax
loc_1_7E8A:
        jne    loc_1_7E9A
        cmp    edi, dword ptr [esp + 0x1c]
        mov    dword ptr [esp + 0x14], 0
        je     loc_1_7EA2
loc_1_7E9A:
        mov    dword ptr [esp + 0x14], 1
loc_1_7EA2:
        mov    edx, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [edx + 0x1c]
        mov    ebp, edx
        add    ebp, 0x20
        mov    edi, dword ptr [ebp]
        or     edi, edi
        je     loc_1_7F15
loc_1_7EB5:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_7ECF
        push   dword ptr [edi + 0xc]
        push   dword ptr [esp + 0x3c]
        call   sub_1_6F00
        add    esp, 8
        or     eax, eax
        je     loc_1_7F0C
loc_1_7ECF:
        mov    eax, dword ptr [edi + 8]
        and    eax, dword ptr [esp + 0x30]
        cmp    eax, dword ptr [esp + 0x2c]
        jne    loc_1_7F0C
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp], eax
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_7F00
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [edi + 4], esi
        push   edi
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 0x14]
        add    esp, 8
        jmp    loc_1_7F0E
        db  08Bh, 0FFh
loc_1_7F00:
        mov    dword ptr [edi + 4], ebx
        mov    byte ptr [edi + 0x10], 0
        jmp    loc_1_7F0E
        db  08Dh, 049h, 000h
loc_1_7F0C:
        mov    ebp, edi
loc_1_7F0E:
        mov    edi, dword ptr [ebp]
        or     edi, edi
        jne    loc_1_7EB5
loc_1_7F15:
        mov    edx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [edx]
        mov    dword ptr [esp + 0x10], edx
        or     edx, edx
        je     loc_1_7F45
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [edx + 0x10], eax
        jne    loc_1_7F45
        mov    eax, dword ptr [esp + 0x1c]
        cmp    dword ptr [edx + 0x14], eax
        jne    loc_1_7F45
        mov    eax, dword ptr [edx + 8]
        and    eax, dword ptr [esp + 0x2c]
        cmp    eax, dword ptr [edx + 4]
        je     loc_1_7EA2
loc_1_7F45:
        mov    eax, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
sub_1_7F60:
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        call   sub_1_7C3C
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
sub_1_7F8C:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        call   sub_1_7280
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_7FBC
        mov    eax, 0x2b04
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_7FBC:
        cmp    dword ptr [eax + 0x28], 0
        jne    loc_1_7FCC
        mov    eax, 0x2b03
        add    esp, 4
        ret    
        db  090h
loc_1_7FCC:
        push   eax
        push   dword ptr [esp + 4]
        call   sub_1_7720
        add    esp, 8
        sub    eax, eax
        add    esp, 4
        ret    
        db  090h
sub_1_7FE0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x2c]
        or     ebx, ebx
        je     loc_1_809E
        cmp    bl, 0x7f
        je     loc_1_809E
        push   0x20
        mov    edi, dword ptr [esp + 0x2c]
        push   edi
        mov    esi, dword ptr [esp + 0x2c]
        push   esi
        call   sub_1_7420
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_809E
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, -1
        je     loc_1_809E
        cmp    eax, dword ptr [esp + 0x20]
        jne    loc_1_809E
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_804C
        push   ebx
        call   sub_1_73BC
        add    esp, 4
        or     eax, eax
        je     loc_1_804C
        push   ebp
        call   sub_1_76E0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_804C:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   edi
        push   ebx
        call   sub_1_7500
        add    esp, 0x10
        or     eax, eax
        je     loc_1_809E
        cmp    dword ptr [esp + 0x14], ebx
        jne    loc_1_809E
        mov    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ecx + 8]
        mov    edx, dword ptr [ecx + 4]
        and    edx, eax
        and    eax, ebx
        cmp    edx, eax
        jne    loc_1_809E
        cmp    ebx, esi
        jne    loc_1_8085
        mov    ebx, 0xffffffff
loc_1_8085:
        push   1
        push   4
        push   1
        push   dword ptr [esp + 0x1c]
        push   dword ptr [ecx + 0xc]
        push   ebx
        push   -1
        push   esi
        call   sub_1_7F60
        add    esp, 0x20
loc_1_809E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
sub_1_80A8:
        mov    edx, 0xa
        mov    ecx, 0xbd68                   ; -> loc_1_BD68
        mov    eax, dword ptr [esp + 4]
loc_1_80B6:
        cmp    dword ptr [ecx], eax
        ja     loc_1_80C6
        sub    ecx, 4
        dec    edx
        cmp    ecx, 0xbd40                   ; -> loc_1_BD40
        jne    loc_1_80B6
loc_1_80C6:
        mov    eax, dword ptr [edx*4 + 0xbd40] ; -> loc_1_BD40
        ret    
        db  08Bh, 0FFh
sub_1_80D0:
        sub    ecx, ecx
        mov    edx, 0xbd40                   ; -> loc_1_BD40
        inc    eax
        mov    eax, dword ptr [esp + 4]
loc_1_80DC:
        cmp    dword ptr [edx], eax
        jb     loc_1_80EC
        add    edx, 4
        inc    ecx
        cmp    edx, 0xbd68                   ; -> loc_1_BD68
        jb     loc_1_80DC
loc_1_80EC:
        mov    eax, dword ptr [ecx*4 + 0xbd40] ; -> loc_1_BD40
        ret    
sub_1_80F4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0xc060], 0         ; -> loc_1_C060
        je     loc_1_81D4
        mov    bp, word ptr [esp + 0x1c]
        or     bp, bp
        je     loc_1_8123
        sub    bp, 0x44
        sbb    ax, ax
        not    ax
        and    bp, ax
        sub    bp, -0x30
loc_1_8123:
        push   0x20
        mov    esi, dword ptr [esp + 0x1c]
        mov    al, byte ptr [esi]
        push   dword ptr [esi + 0xc]
        and    ax, 0xf
        push   dword ptr [esi + 0x10]
        shl    ax, 2
        mov    word ptr [esp + 0x1e], ax
        call   sub_1_7420
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_81D4
        mov    edi, dword ptr [ebx + 0x18]
        movzx  eax, bp
        cmp    eax, edi
        ja     loc_1_81D4
        or     bp, bp
        jne    loc_1_8194
        mov    cx, word ptr [esi + 2]
        mov    ah, cl
        mov    al, ch
        sub    ax, 0x14
        movzx  ecx, ax
        cmp    ecx, edi
        jb     loc_1_8177
        sub    ax, word ptr [esp + 0x12]
loc_1_8177:
        and    eax, 0xffff
        cmp    eax, edi
        jbe    loc_1_8188
        mov    bp, di
        jmp    loc_1_8194
        db  08Dh, 049h, 000h
loc_1_8188:
        push   eax
        call   sub_1_80D0
        add    esp, 4
        mov    bp, ax
loc_1_8194:
        and    ebp, 0xffff
        cmp    ebp, edi
        je     loc_1_81B9
        push   1
        push   4
        push   dword ptr [ebx + 0x14]
        push   ebp
        push   dword ptr [ebx + 0x1c]
        push   dword ptr [ebx + 0xc]
        push   -1
        push   dword ptr [esi + 0x10]
        call   sub_1_7C3C
        add    esp, 0x20
loc_1_81B9:
        push   ebp
        movzx  eax, word ptr [esp + 0x16]
        add    eax, esi
        push   eax
        mov    al, byte ptr [esi + 9]
        push   eax
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 0x10]
        call   sub_1_6D38
        add    esp, 0x14
loc_1_81D4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_81DC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    ecx, 0x3e8
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 0x10], eax
        mov    ebx, 0xc248                   ; -> obj1:0xC248
loc_1_81FA:
        mov    edi, ebx
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_830C
loc_1_8206:
        cmp    dword ptr [esi + 8], -1
        jne    loc_1_830C
        lea    edx, [esi + 0x26]
        mov    cx, word ptr [edx]
        test   cl, 1
        je     loc_1_8300
        cmp    dword ptr [0xc060], 0         ; -> loc_1_C060
        je     loc_1_82D8
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, dword ptr [esi + 0x34]
        test   cl, 2
        je     loc_1_8240
        cmp    eax, 0x78
        jmp    loc_1_8245
        db  08Dh, 049h, 000h
loc_1_8240:
        cmp    eax, 0x258
loc_1_8245:
        mov    eax, 0
        setae  al
        or     eax, eax
        je     loc_1_82D8
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x34], eax
        mov    ebp, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 0x18]
        cmp    eax, dword ptr [ebp + 0x40]
        jae    loc_1_82D0
        or     cx, 2
        mov    word ptr [edx], cx
        push   eax
        call   sub_1_80A8
        add    esp, 4
        cmp    eax, dword ptr [ebp + 0x40]
        jb     loc_1_8284
        mov    edi, dword ptr [ebp + 0x40]
        jmp    loc_1_8291
        db  08Dh, 049h, 000h
loc_1_8284:
        push   dword ptr [esi + 0x18]
        call   sub_1_80A8
        add    esp, 4
        mov    edi, eax
loc_1_8291:
        mov    dword ptr [esi + 0x18], edi
        mov    esi, dword ptr [esi + 4]
        mov    ebp, dword ptr [ebp + 0x38]
        or     ebp, ebp
        je     loc_1_82C8
loc_1_829E:
        test   byte ptr [ebp + 0x14], 1
        je     loc_1_82C1
        movzx  eax, word ptr [ebp + 0x3c]
        sub    eax, edi
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edi
        push   eax
        push   0
        push   0
        push   dword ptr [ebp + 4]
        push   esi
        call   sub_1_6D38
        add    esp, 0x14
loc_1_82C1:
        mov    ebp, dword ptr [ebp + 0x10]
        or     ebp, ebp
        jne    loc_1_829E
loc_1_82C8:
        mov    edi, ebx
        mov    esi, dword ptr [ebx]
        jmp    loc_1_8304
        db  08Bh, 0FFh
loc_1_82D0:
        and    cx, 0xfffd
        mov    word ptr [edx], cx
loc_1_82D8:
        cmp    dword ptr [esi + 0x2c], 4
        jne    loc_1_8300
        cmp    dword ptr [esi + 0x20], 0
        jne    loc_1_8300
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, dword ptr [esi + 0x30]
        cmp    eax, 0x258
        jbe    loc_1_8300
        push   esi
        push   edi
        call   sub_1_7720
        add    esp, 8
        mov    esi, dword ptr [edi]
        jmp    loc_1_8304
loc_1_8300:
        mov    edi, esi
        mov    esi, dword ptr [esi]
loc_1_8304:
        or     esi, esi
        jne    loc_1_8206
loc_1_830C:
        add    ebx, 4
        cmp    ebx, 0xc2c8                   ; -> loc_1_C2C8
        jb     loc_1_81FA
        push   0
        push   0x81dc                        ; -> loc_1_81DC
        push   0xea60
        push   0xc124                        ; -> obj1:0xC124
        call   sub_1_B920
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_833C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 8], 0
        mov    dword ptr [esi + 0x14], 0
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        lea    edi, [esi + 0x38]
        mov    eax, dword ptr [edi + 0x2c]
        or     eax, eax
        je     loc_1_8377
        push   eax
        call   sub_1_B944
        add    esp, 4
        mov    dword ptr [edi + 0x2c], 0
loc_1_8377:
        mov    eax, dword ptr [edi + 0x24]
        or     eax, eax
        je     loc_1_8395
        mov    edx, dword ptr [0xc05c]       ; -> loc_1_C05C
        mov    ecx, dword ptr [edi + 0x28]
        mov    dword ptr [ecx], edx
        mov    dword ptr [0xc05c], eax       ; -> loc_1_C05C
        mov    dword ptr [edi + 0x24], 0
loc_1_8395:
        mov    eax, dword ptr [0xbfac]       ; -> loc_1_BFAC
        mov    dword ptr [edi], eax
        mov    dword ptr [0xbfac], esi       ; -> loc_1_BFAC
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_83A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0xc1c0]       ; -> loc_1_C1C0
        sub    esi, dword ptr [0xc120]       ; -> obj1:0xC120
        sub    esi, 0x14
        sbb    eax, eax
        and    esi, eax
        add    esi, 0x14
        mov    ebx, 0
        je     loc_1_8419
        lea    eax, [esi*4]
        lea    eax, [eax + eax*4]
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_8426
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_8419
        push   0x50
        push   esi
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA02
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_8430
        push   dword ptr [ebp - 0x10]
        call   sub_1_BA2C
loc_1_8419:
        or     ebx, ebx
        je     loc_1_8426
        push   ebx
        call   sub_1_B944
        add    esp, 4
loc_1_8426:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_8430:
        add    dword ptr [0xc120], esi       ; -> obj1:0xC120
        or     esi, esi
        je     loc_1_84A2
loc_1_843A:
        push   0x14
        push   ebx
        push   dword ptr [ebp - 0x10]
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA32
        push   dword ptr [ebp - 0x14]
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA0E
        mov    edi, dword ptr [ebp - 4]
        add    edi, 0x38
        sub    eax, eax
        mov    ecx, 0x14
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 0x38
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x1c], eax
        mov    dword ptr [ecx + 0x20], ebx
        mov    byte ptr [ecx + 5], 8
        mov    byte ptr [ecx + 4], 1
        mov    eax, dword ptr [0xbcfc]       ; -> obj1:0xBCFC
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10], eax
        push   dword ptr [ebp - 4]
        call   sub_1_833C
        add    esp, 4
        add    ebx, 0x14
        dec    esi
        jne    loc_1_843A
loc_1_84A2:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_84AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [0xbff8]       ; -> loc_1_BFF8
        sub    esi, dword ptr [0xc2fc]       ; -> loc_1_C2FC
        sub    esi, 0x7800
        sbb    eax, eax
        and    esi, eax
        add    esi, 0x7800
        cmp    esi, 0x100
        jb     loc_1_854C
        shr    esi, 8
        mov    ebx, esi
        shl    ebx, 8
        push   ebx
        call   sub_1_B93E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_1_8559
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_854C
        or     esi, esi
        je     loc_1_853B
loc_1_850A:
        push   0x100
        push   edi
        push   dword ptr [ebp - 0xc]
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA32
        mov    eax, dword ptr [0xc05c]       ; -> loc_1_C05C
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0xc05c], eax       ; -> loc_1_C05C
        add    edi, 0x100
        dec    esi
        jne    loc_1_850A
loc_1_853B:
        pop    edi
        add    dword ptr [0xc2fc], ebx       ; -> loc_1_C2FC
        mov    eax, 1
        pop    esi
        pop    ebx
        leave  
        ret    
        db  090h
loc_1_854C:
        or     edi, edi
        je     loc_1_8559
        push   edi
        call   sub_1_B944
        add    esp, 4
loc_1_8559:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_8560:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0xc]
        lea    eax, [ecx + 0x38]
        test   byte ptr [eax + 0x4e], 1
        jne    loc_1_8578
        mov    esi, dword ptr [eax + 0x30]
        add    esi, 0x54
        jmp    loc_1_857E
        db  090h
loc_1_8578:
        mov    esi, dword ptr [0xc148]       ; -> obj1:0xC148
loc_1_857E:
        push   ecx
        call   sub_1_833C
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        dec    eax
        mov    dword ptr [esi + 8], eax
        cmp    dword ptr [esi + 0xc], 0
        je     loc_1_85E3
        cmp    dword ptr [esi + 0x10], 0
        jne    loc_1_85E3
        mov    dword ptr [esi + 0x10], 1
        cmp    eax, dword ptr [0xc240]       ; -> loc_1_C240
        jae    loc_1_85DC
loc_1_85A9:
        mov    ecx, dword ptr [esi + 0xc]
        or     ecx, ecx
        je     loc_1_85DC
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    edi, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], edi
        dec    ecx
        mov    dword ptr [esi + 0xc], ecx
        inc    dword ptr [esi + 8]
        sub    edx, 0x4c
        push   dword ptr [edx + 0x7c]
        push   edx
        call   sub_1_85E8
        add    esp, 8
        mov    eax, dword ptr [0xc240]       ; -> loc_1_C240
        cmp    dword ptr [esi + 8], eax
        jb     loc_1_85A9
loc_1_85DC:
        mov    dword ptr [esi + 0x10], 0
loc_1_85E3:
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_85E8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        lea    ebx, [edi + 0x38]
        mov    esi, dword ptr [edi + 8]
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 4], 0x14
        mov    dword ptr [edi + 0x14], 1
        mov    eax, dword ptr [eax + 4]
        lea    ecx, [eax + 0x13]
        shr    ecx, 0xc
        shr    eax, 0xc
        sub    ecx, eax
        inc    ecx
        mov    dword ptr [edi], ecx
        cmp    byte ptr [ebx + 0x4e], 1
        je     loc_1_86CC
        movzx  eax, byte ptr [ebx + 0x4c]
        mov    ebp, dword ptr [esp + 0x18]
        lea    ecx, [eax + ebp]
        cmp    ecx, dword ptr [ebx + 0x38]
        ja     loc_1_865C
        push   eax
        push   dword ptr [ebx + 0x2c]
        push   dword ptr [ebx + 0x20]
        push   esi
        push   edi
        push   dword ptr [ebx + 0x40]
        push   dword ptr [ebx + 0x30]
        call   sub_1_5C7C
        add    esp, 0x1c
        jmp    loc_1_86F4
        db  08Bh, 0FFh
loc_1_865C:
        push   8
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        jne    loc_1_867C
        push   esi
        push   edi
        call   sub_1_8560
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_867C:
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [ebx + 8], eax
        push   0
        movzx  eax, byte ptr [ebx + 0x4c]
        push   eax
        push   dword ptr [ebx + 0x2c]
        push   ebp
        push   esi
        push   dword ptr [ebx + 0x20]
        push   edi
        push   dword ptr [ebx + 0x40]
        push   dword ptr [ebx + 0x38]
        push   dword ptr [ebx + 0x30]
        call   sub_1_5E80
        add    esp, 0x28
        mov    ebp, eax
        cmp    ebp, 0x2b01
        jne    loc_1_86F6
        push   0
        push   4
        push   3
        push   dword ptr [ebx + 0x20]
        mov    eax, dword ptr [ebx + 0x3c]
        push   dword ptr [eax + 4]
        call   sub_1_9610
        add    esp, 0x14
        jmp    loc_1_86F6
loc_1_86CC:
        lea    eax, [ebx + 0x48]
        push   eax
        mov    al, byte ptr [ebx + 0x4d]
        push   eax
        movzx  eax, byte ptr [ebx + 0x4c]
        push   eax
        push   dword ptr [ebx + 0x2c]
        mov    ebp, dword ptr [esp + 0x28]
        push   ebp
        push   esi
        push   dword ptr [ebx + 0x20]
        push   edi
        push   dword ptr [ebx + 0x40]
        push   dword ptr [ebx + 0x3c]
        call   sub_1_63F4
        add    esp, 0x28
loc_1_86F4:
        mov    ebp, eax
loc_1_86F6:
        cmp    ebp, 0x2bf7
        je     loc_1_8708
        push   esi
        push   edi
        call   sub_1_8560
        add    esp, 8
loc_1_8708:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_8710:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        lea    edx, [ecx + 0x38]
        mov    esi, dword ptr [edx + 0x30]
        test   byte ptr [edx + 0x4e], 1
        je     loc_1_872C
        mov    esi, dword ptr [0xc148]       ; -> obj1:0xC148
        jmp    loc_1_872F
        db  08Dh, 049h, 000h
loc_1_872C:
        add    esi, 0x54
loc_1_872F:
        mov    eax, dword ptr [esi + 8]
        cmp    eax, dword ptr [0xc240]       ; -> loc_1_C240
        jae    loc_1_8754
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_1_8754
        inc    eax
        mov    dword ptr [esi + 8], eax
        push   dword ptr [esp + 0x10]
        push   ecx
        call   sub_1_85E8
        add    esp, 8
        pop    esi
        ret    
        db  090h
loc_1_8754:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 0x44], eax
        lea    eax, [edx + 0x14]
        mov    dword ptr [eax], esi
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edx + 0x18], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], eax
        inc    dword ptr [esi + 0xc]
        pop    esi
        ret    
sub_1_8770:
        push   esi
        push   edi
        call   sub_1_B92C
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esi + 0xc]
        or     ecx, ecx
        je     loc_1_87CC
        lea    edi, [ecx + 1]
        sub    edx, edx
        div    edi
        cmp    edx, ecx
        jne    loc_1_8794
        sub    al, al
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_8794:
        mov    edi, dword ptr [esi]
        mov    eax, edx
        dec    edx
        or     eax, eax
        je     loc_1_87A6
loc_1_879D:
        mov    edi, dword ptr [edi]
        mov    eax, edx
        dec    edx
        or     eax, eax
        jne    loc_1_879D
loc_1_87A6:
        mov    edx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        dec    ecx
        mov    dword ptr [esi + 0xc], ecx
        sub    edi, 0x4c
        push   edi
        call   sub_1_833C
        add    esp, 4
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        mov    al, 1
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_87CC:
        sub    al, al
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_87D4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x24]
        shr    esi, 8
        mov    eax, dword ptr [esp + 0x24]
        and    eax, 0xff
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_1_87F4
        inc    esi
loc_1_87F4:
        inc    edi
        mov    edi, dword ptr [esp + 0x1c]
        add    edi, 0x38
loc_1_87FC:
        sub    eax, eax
        mov    ebx, dword ptr [0xc05c]       ; -> loc_1_C05C
        mov    ecx, 0xc05c                   ; -> loc_1_C05C
loc_1_8809:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        je     loc_1_881B
        mov    dword ptr [ecx + 0xc], 0x100
        inc    eax
        cmp    eax, esi
        jb     loc_1_8809
loc_1_881B:
        cmp    eax, esi
        je     loc_1_88C0
        mov    eax, dword ptr [0xbff8]       ; -> loc_1_BFF8
        sub    eax, dword ptr [0xc2fc]       ; -> loc_1_C2FC
        cmp    eax, 0x100
        ja     loc_1_883E
        call   sub_1_84AC
        or     eax, eax
        jne    loc_1_87FC
loc_1_883E:
        test   byte ptr [edi + 0x4e], 1
        jne    loc_1_884C
        mov    ebp, dword ptr [edi + 0x30]
        add    ebp, 0x54
        jmp    loc_1_8852
loc_1_884C:
        mov    ebp, dword ptr [0xc148]       ; -> obj1:0xC148
loc_1_8852:
        push   ebp
        call   sub_1_8770
        add    esp, 4
        or     al, al
        mov    ebx, dword ptr [0xc0b4]       ; -> obj1:0xC0B4
        jne    loc_1_87FC
        test   byte ptr [edi + 0x4e], 1
        jne    loc_1_88A0
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_88B0
        or     ebx, ebx
        je     loc_1_8893
loc_1_8875:
        lea    eax, [ebx + 0x54]
        push   eax
        call   sub_1_8770
        add    esp, 4
        or     al, al
        jne    loc_1_888B
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_1_8875
loc_1_888B:
        or     ebx, ebx
        jne    loc_1_87FC
loc_1_8893:
        pop    ebp
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_88A0:
        pop    ebp
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_88B0:
        pop    ebp
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_88C0:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [0xc05c], eax       ; -> loc_1_C05C
        mov    dword ptr [ecx], 0
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_88DB
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0xc], eax
loc_1_88DB:
        push   ebx
        mov    dword ptr [edi + 0x24], ebx
        mov    dword ptr [edi + 0x28], ecx
        push   dword ptr [esp + 0x20]
        call   sub_1_BA50
        mov    eax, dword ptr [esp + 0x28]
        cmp    eax, dword ptr [esp + 0x24]
        jae    loc_1_8928
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + 0xc]
        lea    ecx, [esp + 0x24]
        push   ecx
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x44]
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 0xc]
        add    esp, 0x1c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_8928:
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_8965
        inc    edx
        mov    edx, dword ptr [esp + 0x20]
loc_1_8934:
        mov    edi, dword ptr [ebx + 4]
        mov    ebp, dword ptr [esp + 0x24]
        sub    ebp, 0x100
        sbb    eax, eax
        and    ebp, eax
        add    ebp, 0x100
        mov    esi, edx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, ebp
        sub    dword ptr [esp + 0x24], ebp
        mov    ebx, dword ptr [ebx]
        jne    loc_1_8934
loc_1_8965:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
sub_1_8970:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        mov    bl, byte ptr [esp + 0x14]
loc_1_897A:
        mov    edx, dword ptr [0xbfac]       ; -> loc_1_BFAC
        or     edx, edx
        jne    loc_1_89BC
        mov    eax, dword ptr [0xc1c0]       ; -> loc_1_C1C0
        cmp    dword ptr [0xc120], eax       ; -> obj1:0xC120
        jbe    loc_1_899A
        call   sub_1_83A8
        or     eax, eax
        jne    loc_1_897A
loc_1_899A:
        test   bl, 1
        lea    eax, [esi + 0x54]
        je     loc_1_89A7
        mov    eax, dword ptr [0xc148]       ; -> obj1:0xC148
loc_1_89A7:
        push   eax
        call   sub_1_8770
        add    esp, 4
        or     al, al
        jne    loc_1_897A
        sub    eax, eax
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_89BC:
        lea    ecx, [edx + 0x38]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [0xbfac], eax       ; -> loc_1_BFAC
        or     byte ptr [ecx + 5], 0x10
        mov    dword ptr [ecx + 8], 0
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ecx + 0x20]
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_89E0:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x12], 0
        cmp    byte ptr [0xbfa4], 0          ; -> loc_1_BFA4
        je     loc_1_8EB0
        mov    ebx, dword ptr [esp + 0x48]
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [esp + 0x18], eax
        test   byte ptr [esp + 0x60], 1
        je     loc_1_8A64
        cmp    byte ptr [0xc150], 0          ; -> obj1:0xC150
        jne    loc_1_8A30
        cmp    byte ptr [esp + 0x60], 6
        jbe    loc_1_8EEB
        inc    dword ptr [0xc1e8]            ; -> obj1:0xC1E8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_8A30:
        cmp    dword ptr [esp + 0x18], -1
        je     loc_1_8EEB
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_8EEB
        cmp    byte ptr [esp + 0x60], 3
        je     loc_1_8EEB
        mov    al, byte ptr [esp + 0x18]
        and    al, 0xf0
        cmp    al, 0xe0
        jne    loc_1_8A89
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8A64:
        cmp    byte ptr [esp + 0x60], 6
        jne    loc_1_8A89
        mov    ecx, dword ptr [esp + 0x44]
        push   dword ptr [ecx + 0xc]
        push   dword ptr [ebx + 0xc]
        call   sub_1_6F30
        add    esp, 8
        mov    dword ptr [esp + 0x44], eax
        or     eax, eax
        je     loc_1_8EEB
loc_1_8A89:
        cmp    byte ptr [ebx + 8], 1
        ja     loc_1_8AB4
        inc    dword ptr [0xc1e4]            ; -> loc_1_C1E4
        push   0
        push   0
        push   0xb
        push   ebx
        mov    ecx, dword ptr [esp + 0x54]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 0x14
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8AB4:
        mov    ax, word ptr [ebx + 2]
        mov    cx, ax
        and    ecx, 0xff
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     ecx, eax
        sub    ecx, dword ptr [esp + 0x4c]
        mov    dword ptr [esp + 0x2c], ecx
        mov    byte ptr [esp + 0x17], 0
        mov    byte ptr [esp + 0x14], 0x28
        mov    byte ptr [esp + 0x15], 0x28
        mov    byte ptr [esp + 0x16], 0x28
        cmp    dword ptr [esp + 0x4c], 0x14
        je     loc_1_8C28
        lea    eax, [ebx + 0x14]
        mov    dword ptr [esp + 0x34], eax
        mov    eax, dword ptr [esp + 0x4c]
        sub    al, 0x14
        mov    byte ptr [esp + 0x3c], al
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        call   sub_1_4C40
        add    esp, 8
        cmp    al, 0x28
        jae    loc_1_8B48
        inc    dword ptr [0xc1e4]            ; -> loc_1_C1E4
        and    eax, 0xff
        add    eax, 0x14
        push   eax
        push   0
        push   0xc
        push   ebx
        mov    ecx, dword ptr [esp + 0x54]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 0x14
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_8B48:
        movzx  eax, byte ptr [esp + 0x3c]
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_8B6C
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Bh, 0FFh
loc_1_8B6C:
        mov    edi, ebp
        mov    al, byte ptr [esp + 0x3c]
        mov    byte ptr [esp + 0x13], al
        and    eax, 0xff
        mov    esi, dword ptr [esp + 0x34]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    byte ptr [esp + 0x14], 0x28
        je     loc_1_8C2F
        cmp    byte ptr [esp + 0x60], 6
        jb     loc_1_8BC8
        cmp    byte ptr [esp + 0x17], 0x89
        jne    loc_1_8BC0
        push   ebp
        call   sub_1_B944
        add    esp, 4
        inc    dword ptr [0xc1e8]            ; -> obj1:0xC1E8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Dh, 049h, 000h
loc_1_8BC0:
        mov    byte ptr [esp + 0x14], 0x28
        jmp    loc_1_8C2F
        db  090h
loc_1_8BC8:
        movzx  eax, byte ptr [esp + 0x14]
        add    eax, ebp
        mov    cl, byte ptr [eax + 2]
        dec    cl
        and    ecx, 0xff
        mov    eax, dword ptr [ecx + eax]
        mov    dword ptr [esp + 0x18], eax
        push   eax
        call   sub_1_7044
        add    esp, 4
        mov    byte ptr [esp + 0x60], al
        cmp    al, 0xff
        jne    loc_1_8C20
        push   0
        push   5
        push   3
        push   ebx
        mov    ecx, dword ptr [esp + 0x54]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 0x14
        inc    dword ptr [0xc1e4]            ; -> loc_1_C1E4
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8C20:
        mov    byte ptr [esp + 0x12], 1
        jmp    loc_1_8C2F
        db  090h
loc_1_8C28:
        sub    ebp, ebp
        mov    byte ptr [esp + 0x13], 0
loc_1_8C2F:
        inc    dword ptr [0xc1ec]            ; -> obj1:0xC1EC
        lea    eax, [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        mov    ecx, dword ptr [esp + 0x4c]
        push   dword ptr [ecx + 4]
        push   dword ptr [esp + 0x24]
        call   sub_1_7500
        add    esp, 0x10
        mov    dword ptr [esp + 0x1c], eax
        or     eax, eax
        jne    loc_1_8C90
        inc    dword ptr [0xc208]            ; -> obj1:0xC208
        push   0
        push   1
        push   3
        push   ebx
        mov    ecx, dword ptr [esp + 0x54]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 0x14
        or     ebp, ebp
        je     loc_1_8EEB
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8C90:
        cmp    byte ptr [esp + 0x17], 0x89
        jne    loc_1_8CD0
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [esp + 0x24], eax
        je     loc_1_8CD0
        inc    dword ptr [0xc208]            ; -> obj1:0xC208
        push   0
        push   5
        push   3
        push   ebx
        mov    ecx, dword ptr [esp + 0x54]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 0x14
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Bh, 0FFh
loc_1_8CD0:
        cmp    byte ptr [esp + 0x60], 1
        je     loc_1_8D64
        or     ebp, ebp
        je     loc_1_8D64
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        call   sub_1_6F30
        add    esp, 8
        or     eax, eax
        jne    loc_1_8D08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_8D08:
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 0x28], eax
        cmp    al, 0x7f
        jne    loc_1_8D1C
        mov    eax, dword ptr [esp + 0x18]
        jmp    loc_1_8D20
        db  08Dh, 049h, 000h
loc_1_8D1C:
        mov    eax, dword ptr [esp + 0x28]
loc_1_8D20:
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ebp
        call   sub_1_6264
        add    esp, 0xc
        cmp    al, 0x28
        je     loc_1_8D64
        inc    dword ptr [0xc1e4]            ; -> loc_1_C1E4
        and    eax, 0xff
        add    eax, 0x14
        push   eax
        push   0
        push   0xc
        push   ebx
        push   dword ptr [esp + 0x38]
        call   sub_1_9610
        add    esp, 0x14
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_1_8D64:
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x44]
        cmp    dword ptr [ecx + 0xc], eax
        jne    loc_1_8DA7
        mov    edx, dword ptr [ecx + 4]
        mov    esi, dword ptr [ecx + 8]
        mov    ecx, edx
        and    ecx, esi
        mov    eax, esi
        and    eax, dword ptr [esp + 0x24]
        cmp    eax, ecx
        jne    loc_1_8DA7
        mov    eax, dword ptr [ebx + 0xc]
        and    eax, esi
        cmp    eax, ecx
        jne    loc_1_8DA7
        cmp    byte ptr [esp + 0x14], 0x28
        jne    loc_1_8DA7
        push   dword ptr [esp + 0x24]
        push   1
        push   5
        push   ebx
        push   edx
        call   sub_1_9610
        add    esp, 0x14
loc_1_8DA7:
        push   dword ptr [esp + 0x60]
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x28]
        push   eax
        call   sub_1_8970
        add    esp, 0xc
        or     eax, eax
        je     loc_1_8E94
        mov    edi, eax
        mov    esi, ebx
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x10], ecx
        mov    cl, byte ptr [ebx + 8]
        dec    cl
        mov    byte ptr [eax + 8], cl
        mov    word ptr [eax + 0xa], 0
        mov    edx, dword ptr [esp + 0x20]
        add    edx, 0x38
        mov    dword ptr [edx + 0x2c], ebp
        mov    al, byte ptr [esp + 0x13]
        mov    byte ptr [edx + 0x4c], al
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edx + 0x30], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [edx + 0x38], eax
        mov    ecx, dword ptr [esp + 0x44]
        mov    dword ptr [edx + 0x3c], ecx
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [edx + 0x40], eax
        mov    al, byte ptr [esp + 0x12]
        mov    byte ptr [edx + 0x4d], al
        mov    al, byte ptr [esp + 0x60]
        mov    byte ptr [edx + 0x4e], al
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 0x48], eax
        push   dword ptr [esp + 0x5c]
        push   dword ptr [esp + 0x5c]
        push   dword ptr [esp + 0x54]
        push   dword ptr [esp + 0x60]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x64]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x60]
        call   sub_1_87D4
        add    esp, 0x20
        cmp    eax, 0x103
        je     loc_1_8EEB
        or     eax, eax
        jne    loc_1_8E78
        push   dword ptr [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x28]
        call   sub_1_8710
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Bh, 0FFh
loc_1_8E78:
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        push   dword ptr [esp + 0x20]
        call   sub_1_833C
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  08Bh, 0FFh
loc_1_8E94:
        inc    dword ptr [0xc204]            ; -> loc_1_C204
        or     ebp, ebp
        je     loc_1_8EEB
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
loc_1_8EB0:
        cmp    byte ptr [esp + 0x60], 1
        je     loc_1_8EEB
        cmp    byte ptr [esp + 0x60], 3
        je     loc_1_8EEB
        inc    dword ptr [0xc1e8]            ; -> obj1:0xC1E8
        test   byte ptr [esp + 0x60], 1
        jne    loc_1_8EEB
        cmp    byte ptr [esp + 0x60], 0
        jne    loc_1_8EEB
        push   0
        push   5
        push   3
        push   dword ptr [esp + 0x54]
        mov    ecx, dword ptr [esp + 0x54]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 0x14
loc_1_8EEB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
        db  090h
sub_1_8EF4:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   1
        push   2
        push   1
        push   0x100
        mov    eax, dword ptr [0xc050]       ; -> obj1:0xC050
        push   dword ptr [eax + 0xc]
        push   -1
        push   -1
        push   dword ptr [esi + 4]
        call   sub_1_7F60
        add    esp, 0x20
        or     eax, eax
        je     loc_1_8F28
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_8F28:
        mov    al, byte ptr [esi + 4]
        shr    al, 2
        and    eax, 0xff
        and    eax, 0xfffffffc
        mov    ecx, dword ptr [eax + 0xbd00] ; -> loc_1_BD00
        mov    edi, dword ptr [esi + 0xc]
        mov    ebx, dword ptr [edi + 0x3c]
        mov    eax, ecx
        not    eax
        and    ebx, eax
        mov    eax, dword ptr [esi + 4]
        and    eax, ecx
        or     ebx, eax
        push   0x20
        push   0
        push   ebx
        call   sub_1_7420
        add    esp, 0xc
        or     eax, eax
        mov    eax, 0
        jne    loc_1_8F7E
        push   0
        push   2
        push   1
        movzx  eax, word ptr [esi + 0x3c]
        push   eax
        push   edi
        push   -1
        push   -1
        push   ebx
        call   sub_1_7C3C
        add    esp, 0x20
loc_1_8F7E:
        or     eax, eax
        je     loc_1_8F88
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_8F88:
        cmp    dword ptr [0xc2f8], 0         ; -> loc_1_C2F8
        je     loc_1_8FC0
        push   0
        push   2
        push   1
        movzx  eax, word ptr [esi + 0x3c]
        push   eax
        push   dword ptr [esi + 0xc]
        push   -1
        push   0xe0
        push   0xe0
        call   sub_1_7F60
        add    esp, 0x20
        or     eax, eax
        je     loc_1_8FC0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_8FC0:
        mov    ecx, dword ptr [esi + 8]
        push   0
        push   2
        push   1
        movzx  eax, word ptr [esi + 0x3c]
        push   eax
        push   dword ptr [esi + 0xc]
        push   -1
        push   ecx
        mov    eax, dword ptr [esi + 4]
        and    eax, ecx
        push   eax
        call   sub_1_7F60
        add    esp, 0x20
        or     eax, eax
        je     loc_1_8FEC
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_8FEC:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_8FF8:
        push   ebx
        push   esi
        push   edi
        call   sub_1_B94A
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [0xc050], esi       ; -> obj1:0xC050
        je     loc_1_90BB
        movzx  eax, word ptr [esi + 0x3c]
        mov    ecx, dword ptr [0xbd6c]       ; -> obj1:0xBD6C
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        mov    dword ptr [0xbd6c], eax       ; -> obj1:0xBD6C
        push   1
        push   2
        push   1
        push   eax
        push   dword ptr [0xc1c8]            ; -> loc_1_C1C8
        push   -1
        push   -1
        mov    eax, dword ptr [esi + 0xc]
        push   dword ptr [eax + 0x3c]
        call   sub_1_7F60
        add    esp, 0x20
        test   byte ptr [esi + 0x14], 1
        je     loc_1_90BB
        push   esi
        call   sub_1_8EF4
        add    esp, 4
        or     eax, eax
        jne    loc_1_9060
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_9060:
        mov    edi, dword ptr [esp + 0x14]
        or     edi, edi
        je     loc_1_90BB
        mov    ebx, dword ptr [esp + 0x18]
loc_1_906C:
        mov    edx, dword ptr [ebx]
        mov    ecx, edx
        and    ecx, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    edx, 0x10
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        cmp    dword ptr [esi + 4], ecx
        push   1
        push   2
        push   1
        movzx  eax, word ptr [esi + 0x3c]
        push   eax
        push   dword ptr [esi + 0xc]
        jne    loc_1_90A8
        push   -1
        jmp    loc_1_90A9
loc_1_90A8:
        push   ecx
loc_1_90A9:
        push   0
        push   0
        call   sub_1_7F60
        add    esp, 0x20
        add    ebx, 4
        dec    edi
        jne    loc_1_906C
loc_1_90BB:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret    
sub_1_90C4:
        movzx  eax, byte ptr [esp + 8]
        cmp    eax, 0x12
        ja     loc_1_912F
        jmp    dword ptr [eax*4 + 0x9130]    ; -> obj1:0x9130
        db  08Dh, 049h, 000h
loc_1_90D8:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x20]
        ret    
loc_1_90E0:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 8]
        ret    
loc_1_90E8:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x14]
        ret    
loc_1_90F0:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x18]
        ret    
loc_1_90F8:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x1c]
        ret    
loc_1_9100:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0xc]
        ret    
loc_1_9108:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x10]
        ret    
loc_1_9110:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x24]
        ret    
loc_1_9118:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x28]
        ret    
loc_1_9120:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x2c]
        ret    
loc_1_9128:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0x30]
loc_1_912F:
        ret    
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_90D8
        dd  loc_1_912F
        dd  loc_1_912F
        dd  loc_1_90E0
        dd  loc_1_90E8
        dd  loc_1_90F0
        dd  loc_1_912F
        dd  loc_1_912F
        dd  loc_1_90F8
        dd  loc_1_912F
        dd  loc_1_912F
        dd  loc_1_9100
        dd  loc_1_9108
        dd  loc_1_9110
        dd  loc_1_9118
        dd  loc_1_912F
        dd  loc_1_912F
        dd  loc_1_9120
        dd  loc_1_9128
sub_1_917C:
        sub    esp, 8
        push   esi
        push   edi
        mov    eax, dword ptr [0xc244]       ; -> obj1:0xC244
        mov    dword ptr [esp + 8], eax
        or     eax, eax
        jne    loc_1_91B0
        push   0x60
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 8], eax
        or     eax, eax
        jne    loc_1_91A8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_91A8:
        inc    dword ptr [0xc064]            ; -> loc_1_C064
        jmp    loc_1_91B7
loc_1_91B0:
        mov    eax, dword ptr [eax]
        mov    dword ptr [0xc244], eax       ; -> obj1:0xC244
loc_1_91B7:
        mov    esi, dword ptr [esp + 0x14]
        lea    eax, [esi + 0x14]
        push   eax
        push   dword ptr [esp + 0xc]
        push   dword ptr [0xc2f4]            ; -> loc_1_C2F4
        mov    edi, dword ptr [esp + 0x24]
        push   edi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_9200
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 0xc], esi
        add    dword ptr [esp + 8], 0x14
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_9200:
        mov    eax, dword ptr [0xc064]       ; -> loc_1_C064
        cmp    dword ptr [0xc0ac], eax       ; -> loc_1_C0AC
        jae    loc_1_9228
        dec    dword ptr [0xc064]            ; -> loc_1_C064
        push   dword ptr [esp + 8]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  090h
loc_1_9228:
        mov    eax, dword ptr [0xc244]       ; -> obj1:0xC244
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0xc244], eax       ; -> obj1:0xC244
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret    
sub_1_9244:
        sub    esp, 8
        push   esi
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 8]
        push   eax
        mov    esi, dword ptr [esp + 0x18]
        push   esi
        call   sub_1_BA44
        mov    eax, dword ptr [0xc064]       ; -> loc_1_C064
        cmp    dword ptr [0xc0ac], eax       ; -> loc_1_C0AC
        jae    loc_1_9288
        dec    dword ptr [0xc064]            ; -> loc_1_C064
        push   dword ptr [esp + 4]
        call   sub_1_B944
        add    esp, 4
        push   esi
        call   sub_1_BA3E
        pop    esi
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_9288:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [0xc244]       ; -> obj1:0xC244
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0xc244], eax       ; -> obj1:0xC244
        call   sub_1_BA3E
        pop    esi
        add    esp, 8
        ret    
        db  090h
sub_1_92A8:
        mov    eax, dword ptr [esp + 4]
        lea    edx, [eax + 0x20]
        mov    eax, dword ptr [eax + 0x20]
        or     eax, eax
        je     loc_1_92D0
        mov    cx, word ptr [esp + 8]
        nop    
loc_1_92BC:
        cmp    word ptr [eax + 0xc], cx
        je     loc_1_92CC
        mov    edx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_92BC
        ret    
        db  090h
loc_1_92CC:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
loc_1_92D0:
        ret    
        db  08Dh, 049h, 000h
sub_1_92D4:
        sub    esp, 4
        push   esi
        lea    eax, [esp + 4]
        push   eax
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_1_BA68
        push   esi
        call   sub_1_9244
        add    esp, 4
        cmp    dword ptr [esp + 4], 0
        je     loc_1_930C
        push   dword ptr [esp + 0xc]
        call   sub_1_B944
        add    esp, 4
        push   dword ptr [esp + 4]
        call   sub_1_BA3E
loc_1_930C:
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_9314:
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 0xc]
loc_1_931C:
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        call   sub_1_5C30
        add    esp, 8
        and    eax, 0xffff
        add    edi, eax
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_931C
        mov    eax, edi
        and    eax, 0xffff
        shr    edi, 0x10
        add    eax, edi
        mov    ecx, eax
        shr    ecx, 0x10
        add    eax, ecx
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_9350:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        inc    dword ptr [0xbf70]            ; -> loc_1_BF70
        lea    eax, [esp + 0x1c]
        push   eax
        push   8
        call   sub_1_917C
        add    esp, 8
        mov    dword ptr [esp + 0x18], eax
        cmp    eax, ebx
        jne    loc_1_938C
        inc    dword ptr [0xbf74]            ; -> loc_1_BF74
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
loc_1_938C:
        mov    al, byte ptr [esp + 0x34]
        mov    ecx, dword ptr [esp + 0x18]
        mov    byte ptr [ecx], al
        mov    byte ptr [ecx + 1], 0
        mov    ax, word ptr [esp + 0x38]
        mov    word ptr [ecx + 4], ax
        mov    ax, word ptr [esp + 0x3c]
        mov    word ptr [ecx + 6], ax
        push   8
        push   dword ptr [esp + 0x1c]
        call   sub_1_5C30
        add    esp, 8
        cmp    dword ptr [esp + 0x44], 0
        mov    word ptr [esp + 0x12], ax
        not    ax
        mov    ecx, dword ptr [esp + 0x18]
        mov    word ptr [ecx + 2], ax
        je     loc_1_94D1
        push   dword ptr [esp + 0x44]
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_9408
        push   dword ptr [esp + 0x1c]
        call   sub_1_9244
        add    esp, 4
        inc    dword ptr [0xbf74]            ; -> loc_1_BF74
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_9408:
        mov    edx, dword ptr [esp + 0x44]
        mov    dword ptr [esp + 0x14], 0
        mov    ebp, dword ptr [esp + 0x40]
loc_1_9418:
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    edi, dword ptr [esp + 0x14]
        add    edi, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ebp, dword ptr [ebp]
        add    dword ptr [esp + 0x14], eax
        sub    edx, eax
        jne    loc_1_9418
        push   dword ptr [esp + 0x44]
        push   ebx
        push   dword ptr [0xc2f4]            ; -> loc_1_C2F4
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x34]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_9490
        push   ebx
        call   sub_1_B944
        add    esp, 4
        push   dword ptr [esp + 0x1c]
        call   sub_1_9244
        add    esp, 4
        inc    dword ptr [0xbf74]            ; -> loc_1_BF74
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_9490:
        push   dword ptr [esp + 0x44]
        push   ebx
        call   sub_1_5C30
        add    esp, 8
        and    eax, 0xffff
        movzx  ecx, word ptr [esp + 0x12]
        add    eax, ecx
        mov    ecx, eax
        shr    ecx, 0x10
        and    eax, 0xffff
        add    eax, ecx
        mov    ecx, eax
        shr    ecx, 0x10
        add    eax, ecx
        not    ax
        mov    ecx, dword ptr [esp + 0x18]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx], eax
loc_1_94D1:
        push   dword ptr [esp + 0x34]
        push   0xbf70                        ; -> loc_1_BF70
        call   sub_1_90C4
        add    esp, 8
        push   0
        push   dword ptr [esp + 0x4c]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x38]
        mov    eax, dword ptr [esp + 0x54]
        add    eax, 8
        push   eax
        push   dword ptr [esp + 0x30]
        push   ebx
        push   0xbffc                        ; -> loc_1_BFFC
        call   sub_1_6848
        add    esp, 0x20
        mov    esi, eax
        cmp    esi, 0x2bf7
        je     loc_1_9521
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   sub_1_92D4
        add    esp, 8
loc_1_9521:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  090h
sub_1_952C:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        inc    dword ptr [0xbf70]            ; -> loc_1_BF70
        movzx  ebx, byte ptr [esp + 0x3c]
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [ebx + 8]
        push   eax
        call   sub_1_917C
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_9568
        inc    dword ptr [0xbf74]            ; -> loc_1_BF74
        mov    eax, 0x2afe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_9568:
        mov    al, byte ptr [esp + 0x2c]
        mov    byte ptr [ebp], al
        mov    al, byte ptr [esp + 0x30]
        mov    byte ptr [ebp + 1], al
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [ebp + 4], eax
        mov    esi, dword ptr [esp + 0x38]
        or     esi, esi
        je     loc_1_9596
        lea    edi, [ebp + 8]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_9596:
        lea    eax, [ebx + 8]
        push   eax
        push   ebp
        call   sub_1_5C30
        add    esp, 8
        not    ax
        mov    word ptr [ebp + 2], ax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_070C
        add    esp, 4
        push   dword ptr [esp + 0x2c]
        push   0xbf70                        ; -> loc_1_BF70
        call   sub_1_90C4
        add    esp, 8
        push   0
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        lea    eax, [ebx + 8]
        push   eax
        push   dword ptr [esp + 0x24]
        push   0
        push   0xbffc                        ; -> loc_1_BFFC
        call   sub_1_6848
        add    esp, 0x20
        mov    esi, eax
        cmp    esi, 0x2bf7
        je     loc_1_9606
        push   dword ptr [esp + 0x10]
        push   0
        call   sub_1_92D4
        add    esp, 8
loc_1_9606:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_9610:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    al, byte ptr [esi]
        and    al, 0xf
        lea    ebx, [eax*4]
        cmp    byte ptr [esi + 9], 1
        jne    loc_1_9638
        movzx  eax, bl
        cmp    byte ptr [eax + esi], 8
        je     loc_1_9638
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_9638:
        push   dword ptr [esi + 0x10]
        call   sub_1_7044
        add    esp, 4
        cmp    al, 0xff
        je     loc_1_96C4
        test   al, 1
        jne    loc_1_96C4
        push   dword ptr [esi + 0xc]
        call   sub_1_7044
        add    esp, 4
        cmp    al, 0xff
        je     loc_1_96BC
        test   al, 1
        jne    loc_1_96BC
        cmp    byte ptr [esi + 0x10], 0x7f
        jne    loc_1_9668
        or     al, al
        jne    loc_1_96BC
loc_1_9668:
        mov    edi, dword ptr [esp + 0x10]
        or     edi, edi
        je     loc_1_96B4
        push   edi
        call   sub_1_7044
        add    esp, 4
        or     al, al
        jne    loc_1_96B4
        movzx  eax, word ptr [esi + 6]
        test   eax, 0xffffff1f
        je     loc_1_9690
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_9690:
        add    bl, 8
        push   ebx
        push   esi
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esi + 0xc]
        push   edi
        call   sub_1_952C
        add    esp, 0x1c
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_96B4:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_96BC:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_96C4:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_96CC:
        push   esi
        push   edi
        sub    esi, esi
        call   sub_1_B92C
        mov    ecx, dword ptr [esp + 0xc]
        lea    edi, [ecx + 0x20]
        mov    edx, dword ptr [ecx + 0x20]
        cmp    edx, esi
        je     loc_1_9704
        nop    
loc_1_96E4:
        cmp    byte ptr [edx + 0xe], 0
        je     loc_1_96FC
        cmp    dword ptr [edx + 4], eax
        jae    loc_1_96FC
        mov    ecx, dword ptr [edx]
        mov    dword ptr [edi], ecx
        mov    dword ptr [edx], esi
        mov    esi, edx
        mov    edx, ecx
        jmp    loc_1_9700
        db  090h
loc_1_96FC:
        mov    edi, edx
        mov    edx, dword ptr [edx]
loc_1_9700:
        or     edx, edx
        jne    loc_1_96E4
loc_1_9704:
        or     esi, esi
        je     loc_1_9722
loc_1_9708:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   0
        push   0x2b02
        push   eax
        call   dword ptr [eax + 8]
        add    esp, 0x14
        or     esi, esi
        jne    loc_1_9708
loc_1_9722:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_9728:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 4]
        mov    edx, dword ptr [0xc090]       ; -> obj1:0xC090
        or     edx, edx
        je     loc_1_9773
loc_1_973A:
        cmp    word ptr [edx + 0x16], cx
        je     loc_1_9746
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_1_973A
loc_1_9746:
        or     edx, edx
        je     loc_1_9773
        mov    ax, word ptr [eax + 6]
        push   eax
        push   edx
        call   sub_1_92A8
        add    esp, 8
        or     eax, eax
        je     loc_1_9773
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   eax
        call   dword ptr [eax + 8]
        add    esp, 0x14
loc_1_9773:
        ret    
loc_1_9774:
        sub    esp, 0x10
        cmp    byte ptr [esp + 0x14], 0
        jne    loc_1_97B5
        mov    ecx, dword ptr [esp + 0x2c]
        cmp    byte ptr [ecx], 8
        jne    loc_1_97B5
        mov    dword ptr [esp], 0
        lea    eax, [esp + 0x24]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0xc], 4
        push   0
        push   4
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esp + 0x24]
        push   ecx
        call   sub_1_9728
        add    esp, 0x14
loc_1_97B5:
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
sub_1_97BC:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 3
        je     loc_1_97DC
        cmp    eax, 4
        je     loc_1_9820
        cmp    eax, 0xb
        je     loc_1_9828
        cmp    eax, 0xc
        je     loc_1_9840
        mov    eax, 0x2b2a
        ret    
        db  090h
loc_1_97DC:
        movzx  eax, byte ptr [esp + 8]
        cmp    eax, 0xc
        ja     loc_1_97F0
        jmp    dword ptr [eax*4 + 0x9848]    ; -> obj1:0x9848
        db  08Dh, 049h, 000h
loc_1_97F0:
        mov    eax, 0x2afa
        ret    
        db  08Bh, 0FFh
loc_1_97F8:
        add    eax, 0x2afa
        ret    
        db  08Bh, 0FFh
loc_1_9800:
        mov    eax, 0x2b01
        ret    
        db  08Bh, 0FFh
loc_1_9808:
        mov    eax, 0x2b04
        ret    
        db  08Bh, 0FFh
loc_1_9810:
        mov    eax, 0x2afa
        ret    
        db  08Bh, 0FFh
loc_1_9818:
        mov    eax, 0x2afb
        ret    
        db  08Bh, 0FFh
loc_1_9820:
        mov    eax, 0x2b08
        ret    
        db  08Bh, 0FFh
loc_1_9828:
        cmp    byte ptr [esp + 8], 0
        jne    loc_1_9838
        mov    eax, 0x2b05
        ret    
        db  08Dh, 049h, 000h
loc_1_9838:
        mov    eax, 0x2b06
        ret    
        db  08Bh, 0FFh
loc_1_9840:
        mov    eax, 0x2b07
        ret    
        db  08Bh, 0FFh
        ; ---- dispatch table (13 entries) ----
        dd  loc_1_97F8
        dd  loc_1_97F8
        dd  loc_1_97F8
        dd  loc_1_97F8
        dd  loc_1_9800
        dd  loc_1_9808
        dd  loc_1_9810
        dd  loc_1_9818
        dd  loc_1_9810
        dd  loc_1_9810
        dd  loc_1_9818
        dd  loc_1_9810
        dd  loc_1_9818
loc_1_987C:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        inc    dword ptr [0xbfb0]            ; -> loc_1_BFB0
        push   dword ptr [esp + 0x28]
        call   sub_1_7044
        add    esp, 4
        mov    ebp, dword ptr [esp + 0x34]
        cmp    ebp, 8
        jb     loc_1_9B00
        cmp    al, 0xff
        je     loc_1_9B00
        test   al, 1
        jne    loc_1_9B00
        cmp    byte ptr [esp + 0x24], 0x7f
        jne    loc_1_98C1
        or     al, al
        jne    loc_1_9B00
loc_1_98C1:
        mov    esi, dword ptr [esp + 0x30]
        push   esi
        call   sub_1_9314
        add    esp, 4
        cmp    ax, 0xffff
        jne    loc_1_9B00
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esp + 0x38], 0
        je     loc_1_98EC
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_98EC:
        lea    eax, [edi + 8]
        mov    dword ptr [esi + 8], eax
        sub    dword ptr [esi + 0xc], 8
        sub    ebp, 8
        mov    ebx, eax
        jne    loc_1_98FF
        sub    ebx, ebx
loc_1_98FF:
        mov    al, byte ptr [edi]
        movzx  ecx, al
        cmp    ecx, 0x11
        ja     loc_1_9910
        jmp    dword ptr [ecx*4 + 0x9b10]    ; -> obj1:0x9B10
loc_1_9910:
        push   eax
        push   0xbfb0                        ; -> loc_1_BFB0
        call   sub_1_90C4
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_9928:
        inc    dword ptr [0xbfd0]            ; -> loc_1_BFD0
        push   dword ptr [esp + 0x40]
        push   ebp
        push   esi
        push   0
        push   edi
        call   sub_1_9728
        add    esp, 0x14
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_994C:
        or     ebx, ebx
        je     loc_1_9A40
        cmp    ebp, 0x14
        jb     loc_1_9A40
        mov    al, byte ptr [ebx]
        and    eax, 0xf
        lea    esi, [eax*4]
        lea    eax, [esi + 8]
        cmp    eax, ebp
        jbe    loc_1_9980
        inc    dword ptr [0xbfb4]            ; -> loc_1_BFB4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_9980:
        push   dword ptr [ebx + 0xc]
        call   sub_1_7044
        add    esp, 4
        or     al, al
        je     loc_1_99A0
        inc    dword ptr [0xbfb4]            ; -> loc_1_BFB4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
loc_1_99A0:
        mov    al, byte ptr [edi]
        cmp    al, 5
        je     loc_1_9A14
        push   eax
        push   0xbfb0                        ; -> loc_1_BFB0
        call   sub_1_90C4
        add    esp, 8
        mov    al, byte ptr [ebx + 9]
        push   eax
        call   sub_1_6CFC
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_99EE
        lea    eax, [ebx + esi]
        push   eax
        push   dword ptr [edi + 4]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0xc]
        push   dword ptr [ebx + 0x10]
        mov    al, byte ptr [edi + 1]
        push   eax
        mov    al, byte ptr [edi]
        push   eax
        call   sub_1_97BC
        add    esp, 8
        push   eax
        push   0
        call   ebp
        add    esp, 0x1c
loc_1_99EE:
        cmp    byte ptr [edi + 1], 4
        jne    loc_1_9AF3
        mov    ax, word ptr [edi + 6]
        mov    ch, al
        mov    cl, ah
        push   ecx
        push   ebx
        call   sub_1_80F4
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_9A14:
        inc    dword ptr [0xbfc8]            ; -> loc_1_BFC8
        push   dword ptr [edi + 4]
        push   dword ptr [ebx + 0xc]
        push   dword ptr [ebx + 0x10]
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        call   sub_1_7FE0
        add    esp, 0x14
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_9A40:
        sub    eax, eax
        inc    dword ptr [0xbfb4]            ; -> loc_1_BFB4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_9A50:
        inc    dword ptr [0xbfcc]            ; -> loc_1_BFCC
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_070C
        add    esp, 4
        mov    ecx, dword ptr [esp + 0x40]
        mov    al, byte ptr [ecx + 0xa]
        mov    byte ptr [esp + 0x1a], al
        mov    al, byte ptr [ecx + 0xb]
        mov    byte ptr [esp + 0x1b], al
        cmp    dword ptr [ecx], 0
        je     loc_1_9A90
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_0498
        add    esp, 0x10
loc_1_9A90:
        lea    eax, [esp + 0x10]
        push   eax
        push   ebp
        push   esi
        mov    ax, word ptr [edi + 6]
        push   eax
        mov    ax, word ptr [edi + 4]
        push   eax
        push   0
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x44]
        call   sub_1_9350
        add    esp, 0x20
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_9ACC:
        inc    dword ptr [0xbfdc]            ; -> loc_1_BFDC
        push   4
        mov    eax, dword ptr [esp + 0x24]
        add    eax, 8
        push   eax
        push   dword ptr [edi + 4]
        push   0
        push   0x12
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x44]
        call   sub_1_952C
        add    esp, 0x1c
loc_1_9AF3:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_9B00:
        sub    eax, eax
        inc    dword ptr [0xbfb4]            ; -> loc_1_BFB4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        ; ---- dispatch table (18 entries) ----
        dd  loc_1_9928
        dd  loc_1_9910
        dd  loc_1_9910
        dd  loc_1_994C
        dd  loc_1_994C
        dd  loc_1_994C
        dd  loc_1_9910
        dd  loc_1_9910
        dd  loc_1_9A50
        dd  loc_1_9910
        dd  loc_1_9910
        dd  loc_1_994C
        dd  loc_1_994C
        dd  loc_1_9910
        dd  loc_1_9910
        dd  loc_1_9910
        dd  loc_1_9910
        dd  loc_1_9ACC
sub_1_9B58:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x50]
        cmp    byte ptr [esi + 9], 0
        jne    loc_1_9B78
        mov    eax, 0x2aff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Bh, 0FFh
loc_1_9B78:
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_070C
        add    esp, 4
        mov    al, byte ptr [esi + 9]
        mov    byte ptr [esp + 0x21], al
        mov    al, byte ptr [esi + 0xb]
        mov    byte ptr [esp + 0x23], al
        mov    al, byte ptr [esi + 0xa]
        and    al, 0xfc
        mov    byte ptr [esp + 0x22], al
        mov    cl, byte ptr [esi + 8]
        or     cl, cl
        je     loc_1_9BBE
        lea    eax, [esp + 0x18]
        push   eax
        movzx  eax, cl
        push   eax
        push   dword ptr [esi]
        call   sub_1_0738
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_9D1D
loc_1_9BBE:
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_9BCC
        mov    esi, dword ptr [esp + 0x1c]
        jmp    loc_1_9BD0
        db  090h
loc_1_9BCC:
        mov    esi, dword ptr [esp + 0x4c]
loc_1_9BD0:
        push   esi
        call   sub_1_7044
        add    esp, 4
        cmp    al, 0xff
        jne    loc_1_9BF8
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        mov    eax, 0x2b0a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  090h
loc_1_9BF8:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   0
        push   esi
        call   sub_1_7500
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_9C2C
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        mov    eax, 0x2afb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  090h
loc_1_9C2C:
        mov    ebx, 0
        cmp    dword ptr [0xbbfc], 0         ; -> obj1:0xBBFC
        jne    loc_1_9C46
        push   eax
        push   esi
        call   sub_1_6F30
        add    esp, 8
        mov    ebx, eax
loc_1_9C46:
        or     ebx, ebx
        jne    loc_1_9C64
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        mov    eax, 0x2afb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
loc_1_9C64:
        call   sub_1_B92C
        add    eax, dword ptr [esp + 0x3c]
        mov    edi, dword ptr [esp + 0x38]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [edi + 8], eax
        mov    byte ptr [edi + 0xe], 0
        mov    bp, word ptr [ebx + 0x3e]
        inc    bp
        mov    word ptr [ebx + 0x3e], bp
        mov    word ptr [edi + 0xc], bp
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [edi], eax
        mov    dword ptr [ebx + 0x20], edi
        mov    dword ptr [esp + 0x24], 0
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x2c], eax
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [esp + 0x30], eax
        lea    ecx, [esp + 0x18]
        push   ecx
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   ebp
        mov    ax, word ptr [ebx + 0x16]
        push   eax
        push   8
        push   dword ptr [ebx + 4]
        push   esi
        call   sub_1_9350
        add    esp, 0x20
        mov    esi, eax
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_0A78
        add    esp, 4
        cmp    esi, 0x2bf7
        je     loc_1_9D00
        or     esi, esi
        je     loc_1_9D00
        push   ebp
        push   ebx
        call   sub_1_92A8
        add    esp, 8
        or     eax, eax
        je     loc_1_9D00
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  090h
loc_1_9D00:
        mov    eax, dword ptr [ebx + 0x20]
        or     eax, eax
        je     loc_1_9D18
        nop    
loc_1_9D08:
        cmp    eax, edi
        je     loc_1_9D14
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_9D08
        jmp    loc_1_9D18
loc_1_9D14:
        mov    byte ptr [edi + 0xe], 1
loc_1_9D18:
        mov    eax, 0x2bf7
loc_1_9D1D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Dh, 049h, 000h
sub_1_9D28:
        sub    esp, 0xc
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x18]
        lea    esi, [ecx + eax]
        cmp    eax, 0x14
        jae    loc_1_9D48
        mov    eax, 0x2af9
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
loc_1_9D48:
        mov    ax, word ptr [ecx + 0xa]
        or     ax, ax
        je     loc_1_9D7C
        mov    dx, word ptr [ecx + 8]
        cmp    dx, 0x14
        jb     loc_1_9D6E
        and    edx, 0xffff
        and    eax, 0xffff
        add    edx, eax
        add    edx, ecx
        cmp    edx, esi
        jbe    loc_1_9D7C
loc_1_9D6E:
        pop    edi
        mov    eax, 0x2b2a
        pop    esi
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_9D7C:
        mov    dl, byte ptr [ecx + 0x12]
        or     dl, dl
        je     loc_1_9DA8
        mov    ax, word ptr [ecx + 0x10]
        cmp    ax, 0x14
        jb     loc_1_9D9D
        movzx  edi, dl
        and    eax, 0xffff
        add    edi, eax
        add    edi, ecx
        cmp    edi, esi
        jbe    loc_1_9DA8
loc_1_9D9D:
        pop    edi
        mov    eax, 0x2b2a
        pop    esi
        add    esp, 0xc
        ret    
loc_1_9DA8:
        cmp    byte ptr [ecx + 0xc], 0
        je     loc_1_9DEC
        mov    byte ptr [esp + 0x10], dl
        cmp    byte ptr [ecx + 0x12], 0
        jbe    loc_1_9DC4
        movzx  eax, word ptr [ecx + 0x10]
        add    eax, ecx
        mov    dword ptr [esp + 8], eax
        jmp    loc_1_9DCC
loc_1_9DC4:
        mov    dword ptr [esp + 8], 0
loc_1_9DCC:
        mov    dword ptr [esp + 0xc], 0
        mov    al, byte ptr [ecx + 0xd]
        mov    byte ptr [esp + 0x11], al
        mov    al, byte ptr [ecx + 0xe]
        mov    byte ptr [esp + 0x12], al
        mov    al, byte ptr [ecx + 0xf]
        mov    byte ptr [esp + 0x13], al
        jmp    loc_1_9E10
        db  090h
loc_1_9DEC:
        mov    byte ptr [esp + 0x10], 0
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 0xc], 0
        mov    byte ptr [esp + 0x11], 0x20
        mov    byte ptr [esp + 0x12], 0
        mov    byte ptr [esp + 0x13], 0
loc_1_9E10:
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [ecx]
        push   dword ptr [esp + 0x2c]
        movzx  eax, word ptr [ecx + 0xa]
        push   eax
        movzx  eax, word ptr [ecx + 8]
        add    eax, ecx
        push   eax
        push   dword ptr [ecx + 4]
        push   dword ptr [esp + 0x38]
        call   sub_1_9B58
        add    esp, 0x1c
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
sub_1_9E3C:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, 0x1c
        mov    ecx, dword ptr [esp + 0x24]
        mov    ebx, dword ptr [ecx + 0x14]
        mov    ebp, dword ptr [esp + 0x2c]
        cmp    dword ptr [esp + 0x34], 0
        je     loc_1_9E68
        mov    ecx, dword ptr [esp + 0x34]
        mov    cl, byte ptr [ecx + 8]
        mov    byte ptr [esp + 0x13], cl
        jmp    loc_1_9E6D
        db  090h
loc_1_9E68:
        mov    byte ptr [esp + 0x13], 0
loc_1_9E6D:
        lea    ecx, [ebx + 0x1c]
        mov    byte ptr [ebx + 0x17], 0
        mov    dword ptr [ebx + 0x18], ecx
        mov    word ptr [ebx + 0xc], 0
        mov    dword ptr [ebx + 0x10], ecx
        mov    edx, dword ptr [esp + 0x28]
        or     edx, edx
        je     loc_1_9EA0
        cmp    dword ptr [esp + 0x30], 0
        jne    loc_1_9EA0
        mov    word ptr [ebx + 0xe], 0
        mov    dword ptr [ebx + 4], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_9EA0:
        or     edx, edx
        je     loc_1_9EB4
        mov    edi, ebx
        mov    esi, dword ptr [ebp + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [esp + 0x30], 0
        sub    ebp, ebp
loc_1_9EB4:
        mov    esi, dword ptr [esp + 0x30]
        movzx  ecx, byte ptr [esp + 0x13]
        mov    dword ptr [esp + 0x18], ecx
        add    esi, ecx
        add    esi, 0x1c
        mov    ecx, dword ptr [esp + 0x24]
        cmp    esi, dword ptr [ecx + 0x18]
        jbe    loc_1_9EE4
        mov    word ptr [ebx + 0xe], 0
        mov    dword ptr [ebx + 4], 0x2af9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_9EE4:
        mov    dword ptr [ebx + 4], edx
        mov    word ptr [ebx + 0xe], 1
        call   sub_1_B92C
        mov    ecx, dword ptr [esp + 0x24]
        sub    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [ebx + 8], eax
        cmp    dword ptr [esp + 0x34], 0
        je     loc_1_9F3F
        mov    ecx, dword ptr [esp + 0x34]
        mov    al, byte ptr [ecx + 9]
        mov    byte ptr [ebx + 0x14], al
        mov    al, byte ptr [ecx + 0xa]
        mov    byte ptr [ebx + 0x15], al
        mov    al, byte ptr [ecx + 0xb]
        mov    byte ptr [ebx + 0x16], al
        mov    cl, byte ptr [esp + 0x13]
        mov    byte ptr [ebx + 0x17], cl
        or     cl, cl
        je     loc_1_9F3F
        mov    eax, dword ptr [esp + 0x18]
        mov    edi, dword ptr [ebx + 0x18]
        mov    ecx, dword ptr [esp + 0x34]
        mov    esi, dword ptr [ecx]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_9F3F:
        mov    ax, word ptr [esp + 0x30]
        mov    word ptr [esp + 0x1c], ax
        movzx  ecx, byte ptr [ebx + 0x17]
        mov    word ptr [ebx + 0xc], ax
        mov    eax, dword ptr [ebx + 0x18]
        add    eax, ecx
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [ebx + 0x10], eax
        cmp    dword ptr [esp + 0x30], 0
        je     loc_1_9F93
        mov    edx, eax
loc_1_9F66:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esp + 0x30]
        sub    eax, ecx
        sbb    esi, esi
        and    eax, esi
        add    eax, ecx
        mov    edi, edx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, eax
        sub    dword ptr [esp + 0x30], eax
        mov    ebp, dword ptr [ebp]
        jne    loc_1_9F66
loc_1_9F93:
        movzx  eax, word ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x18]
        lea    eax, [eax + ecx + 0x1c]
        mov    ecx, dword ptr [ebx + 0x18]
        sub    ecx, ebx
        mov    dword ptr [ebx + 0x18], ecx
        mov    ecx, dword ptr [esp + 0x14]
        sub    ecx, ebx
        mov    dword ptr [ebx + 0x10], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_9FBC:
        push   esi
        push   edi
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esp + 0x1c]
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        push   esi
        call   sub_1_9E3C
        add    esp, 0x14
        mov    dword ptr [esi + 0x18], eax
        push   edi
        add    esi, 0x1c
        push   esi
        call   sub_1_B95C
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_9FF0:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        call   sub_1_B92C
        mov    dword ptr [esp + 0x1c], eax
        mov    esi, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x20], esi
        mov    edi, dword ptr [esp + 0x44]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esp + 0x24], eax
        mov    dword ptr [esp + 0x28], 0
        mov    dword ptr [esp + 0x2c], 0
        mov    dword ptr [esp + 0x30], 0
        push   0x9fbc                        ; -> loc_1_9FBC
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax]
        mov    ebx, dword ptr [esp + 0x44]
        push   ebx
        call   sub_1_9D28
        add    esp, 0x10
        cmp    eax, 0x2bf7
        jne    loc_1_A074
        lea    eax, [esp + 0x28]
        push   eax
        call   sub_1_B950
        add    esp, 4
        or     eax, eax
        jne    loc_1_A062
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi], eax
loc_1_A062:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
loc_1_A074:
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Dh, 049h, 000h
loc_1_A084:
        sub    esp, 0x60
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x13], 0
        cmp    dword ptr [0xbd78], 0         ; -> obj1:0xBD78
        jne    loc_1_A1DE
        mov    dword ptr [0xbd78], 1         ; -> obj1:0xBD78
        mov    eax, dword ptr [0xbd70]       ; -> obj1:0xBD70
        mov    dword ptr [esp + 0x24], eax
        or     eax, eax
        je     loc_1_A1C8
loc_1_A0B8:
        mov    eax, dword ptr [esp + 0x24]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [0xbd70], eax       ; -> obj1:0xBD70
        dec    dword ptr [0xc1b4]            ; -> obj1:0xC1B4
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   0
        push   0
        push   dword ptr [esp + 0x34]
        call   sub_1_BA20
        mov    eax, dword ptr [esp + 0x28]
        add    dword ptr [0xc1a0], eax       ; -> sub_1_C1A0
        add    dword ptr [0xc188], eax       ; -> obj1:0xC188
        cmp    dword ptr [0xc17c], 1         ; -> obj1:0xC17C
        jne    loc_1_A19C
        mov    ebx, dword ptr [esp + 0x28]
        sub    ebx, 0x44
        sbb    eax, eax
        and    ebx, eax
        add    ebx, 0x44
        mov    ebp, 0
        lea    eax, [esp + 0x2c]
        mov    dword ptr [esp + 0x18], eax
        je     loc_1_A16D
        nop    
loc_1_A11C:
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA44
        mov    eax, ebx
        sub    eax, ebp
        mov    ecx, dword ptr [esp + 0x20]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        mov    edi, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebp, eax
        add    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x18]
        call   sub_1_BA68
        cmp    ebp, ebx
        jb     loc_1_A11C
loc_1_A16D:
        inc    dword ptr [0xc18c]            ; -> loc_1_C18C
        mov    byte ptr [esp + 0x13], 1
        push   0
        push   0
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   ebx
        lea    eax, [esp + 0x40]
        push   eax
        push   dword ptr [esp + 0x90]
        call   sub_1_5514
        add    esp, 0x1c
        jmp    loc_1_A1A2
        db  090h
loc_1_A19C:
        inc    dword ptr [0xc194]            ; -> obj1:0xC194
loc_1_A1A2:
        push   0
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x80]
        call   sub_1_5BD8
        add    esp, 0xc
        mov    eax, dword ptr [0xbd70]       ; -> obj1:0xBD70
        mov    dword ptr [esp + 0x24], eax
        or     eax, eax
        jne    loc_1_A0B8
loc_1_A1C8:
        mov    dword ptr [0xbd78], 0         ; -> obj1:0xBD78
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_A1DE
        call   sub_1_4BCC
loc_1_A1DE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x60
        ret    
        db  08Bh, 0FFh
loc_1_A1E8:
        inc    dword ptr [0xc1a4]            ; -> obj1:0xC1A4
        cmp    dword ptr [0xc17c], 1         ; -> obj1:0xC17C
        jne    loc_1_A248
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x24], 0
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_A214
        mov    dword ptr [0xbd70], eax       ; -> obj1:0xBD70
        jmp    loc_1_A21D
        db  08Bh, 0FFh
loc_1_A214:
        mov    ecx, dword ptr [0xbd74]       ; -> obj1:0xBD74
        mov    dword ptr [ecx + 0x24], eax
loc_1_A21D:
        mov    dword ptr [0xbd74], eax       ; -> obj1:0xBD74
        inc    dword ptr [0xc1b4]            ; -> obj1:0xC1B4
        cmp    dword ptr [0xbd78], 0         ; -> obj1:0xBD78
        jne    loc_1_A242
        push   dword ptr [esp + 4]
        push   0xc098                        ; -> loc_1_C098
        call   sub_1_B932
        add    esp, 8
loc_1_A242:
        mov    eax, 0x103
        ret    
loc_1_A248:
        sub    eax, eax
        inc    dword ptr [0xc1ac]            ; -> loc_1_C1AC
        ret    
        db  08Dh, 049h, 000h
loc_1_A254:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        lea    eax, [esp + 0x18]
        push   eax
        push   0
        push   0
        push   dword ptr [esp + 0x40]
        call   sub_1_BA20
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA44
        mov    esi, dword ptr [esp + 0x38]
        cmp    dword ptr [esp + 0x10], esi
        ja     loc_1_A2B7
loc_1_A28C:
        sub    esi, dword ptr [esp + 0x10]
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x18]
        call   sub_1_BA68
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA44
        cmp    dword ptr [esp + 0x10], esi
        jbe    loc_1_A28C
loc_1_A2B7:
        push   0
        add    dword ptr [esp + 0x20], esi
        sub    dword ptr [esp + 0x14], esi
        lea    eax, [esp + 0x1c]
        push   eax
        push   0
        push   0
        push   dword ptr [esp + 0x50]
        call   sub_1_BA20
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_BA44
        sub    ebx, ebx
        mov    ebp, dword ptr [esp + 0x3c]
        cmp    ebp, ebx
        je     loc_1_A394
loc_1_A2F4:
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [esp + 0x10]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        mov    edi, dword ptr [esp + 0x24]
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    ebx, eax
        add    dword ptr [esp + 0x24], eax
        add    dword ptr [esp + 0x1c], eax
        sub    ebp, eax
        sub    dword ptr [esp + 0x10], eax
        sub    dword ptr [esp + 0x20], eax
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_A35D
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x18]
        call   sub_1_BA68
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_A394
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA44
loc_1_A35D:
        cmp    dword ptr [esp + 0x20], 0
        jne    loc_1_A38C
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA68
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_A394
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_BA44
loc_1_A38C:
        or     ebp, ebp
        jne    loc_1_A2F4
loc_1_A394:
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [eax], ebx
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
loc_1_A3A4:
        ret    
        db  08Dh, 049h, 000h
loc_1_A3A8:
        ret    
        db  08Dh, 049h, 000h
loc_1_A3AC:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0
        mov    esi, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [ecx + 4]
        cmp    dword ptr [ecx], 0x200
        jne    loc_1_A4D4
        cmp    eax, dword ptr [0xbfa8]       ; -> loc_1_BFA8
        jne    loc_1_A4D4
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 0xc], 0x100
        je     loc_1_A3F8
        mov    eax, 0x1a
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_1_A3F8:
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 0x100
        jne    loc_1_A448
        cmp    dword ptr [ecx + 0x10], 1
        jne    loc_1_A43C
        cmp    edi, 4
        jb     loc_1_A430
        lea    eax, [esp + 8]
        push   eax
        push   4
        push   0xbd98                        ; -> loc_1_BD98
        push   dword ptr [esp + 0x24]
        call   sub_1_AAE0
        add    esp, 0x10
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_A430:
        pop    edi
        mov    eax, 0x21
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_1_A43C:
        pop    edi
        mov    eax, 0x1a
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_1_A448:
        cmp    eax, 0x200
        je     loc_1_A45C
        mov    eax, 0x1a
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_A45C:
        cmp    dword ptr [ecx + 0x10], 1
        jne    loc_1_A4C8
        cmp    edi, 0x5c
        jae    loc_1_A474
        mov    eax, 0x21
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_A474:
        lea    eax, [esp + 8]
        push   eax
        push   0x5c
        push   0xc160                        ; -> obj1:0xC160
        push   dword ptr [esp + 0x24]
        call   sub_1_AAE0
        add    esp, 0x10
        cmp    edi, 0x76
        jb     loc_1_A4B4
        lea    ecx, [esp + 8]
        push   ecx
        push   0x1a
        push   0xbd7c                        ; -> loc_1_BD7C
        push   eax
        call   sub_1_AAE0
        add    esp, 0x10
        mov    dword ptr [esi], 0x76
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_1_A4B4:
        pop    edi
        mov    dword ptr [esi], 0x5c
        mov    eax, 9
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_A4C8:
        pop    edi
        mov    eax, 0x1a
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_1_A4D4:
        pop    edi
        mov    eax, 0xfe
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_1_A4E0:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 4]
        cmp    dword ptr [eax], 0x200
        jne    loc_1_A554
        cmp    ecx, dword ptr [0xbfa8]       ; -> loc_1_BFA8
        jne    loc_1_A554
        cmp    dword ptr [eax + 8], 0x200
        jne    loc_1_A54C
        cmp    dword ptr [eax + 0xc], 0x100
        jne    loc_1_A54C
        cmp    dword ptr [eax + 0x10], 1
        jne    loc_1_A544
        cmp    dword ptr [esp + 0x10], 0x5c
        jb     loc_1_A53C
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        cmp    eax, 1
        je     loc_1_A534
        cmp    eax, 2
        je     loc_1_A534
        cmp    eax, 3
        je     loc_1_A539
        mov    eax, 0x1a
        ret    
        db  08Bh, 0FFh
loc_1_A534:
        mov    dword ptr [0xc17c], eax       ; -> obj1:0xC17C
loc_1_A539:
        sub    eax, eax
        ret    
loc_1_A53C:
        mov    eax, 0x1a
        ret    
        db  08Bh, 0FFh
loc_1_A544:
        mov    eax, 0x1a
        ret    
        db  08Bh, 0FFh
loc_1_A54C:
        mov    eax, 0x1a
        ret    
        db  08Bh, 0FFh
loc_1_A554:
        mov    eax, 0xfe
        ret    
        db  08Bh, 0FFh
loc_1_A55C:
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
loc_1_A564:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x1c]
        mov    edx, dword ptr [ecx]
        sub    edi, edi
        cmp    edx, edi
        je     loc_1_A598
        mov    esi, edx
        mov    eax, dword ptr [esp + 0x18]
loc_1_A57A:
        cmp    dword ptr [eax], 0x200
        jne    loc_1_A590
        mov    ebx, dword ptr [eax + 4]
        inc    ebx
        sub    ebx, edi
        sbb    ebp, ebp
        not    ebp
        and    ebx, ebp
        add    edi, ebx
loc_1_A590:
        add    eax, 8
        dec    esi
        jne    loc_1_A57A
        jmp    loc_1_A59C
loc_1_A598:
        mov    eax, dword ptr [esp + 0x18]
loc_1_A59C:
        inc    edx
        cmp    edx, 0x20
        jbe    loc_1_A5AC
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_A5AC:
        pop    ebp
        mov    dword ptr [0xbfa8], edi       ; -> loc_1_BFA8
        mov    dword ptr [eax], 0x200
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [ecx], edx
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_A5C8:
        mov    eax, dword ptr [0xbff4]       ; -> loc_1_BFF4
        imul   eax, eax, 0x48c27395
        or     eax, eax
        jge    loc_1_A5DD
        add    eax, 0x7fffffff
        inc    eax
loc_1_A5DD:
        or     eax, eax
        mov    dword ptr [0xbff4], eax       ; -> loc_1_BFF4
        jne    loc_1_A5FE
        call   sub_1_B92C
        add    eax, dword ptr [0xbff0]       ; -> loc_1_BFF0
        mov    ecx, 0x5f5e100
        cdq    
        idiv   ecx
        or     edx, 1
        mov    eax, edx
loc_1_A5FE:
        sub    edx, edx
        mov    dword ptr [0xbff4], eax       ; -> loc_1_BFF4
        mov    ecx, 0x14
        div    ecx
        lea    eax, [edx + 1]
        ret    
sub_1_A610:
        mov    eax, dword ptr [esp + 4]
        lea    edx, [eax + 0x40]
        mov    eax, dword ptr [eax + 0x40]
        or     eax, eax
        je     loc_1_A636
        mov    ecx, dword ptr [esp + 8]
loc_1_A622:
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_A630
        mov    edx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_A622
        ret    
loc_1_A630:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], edx
loc_1_A636:
        ret    
        db  090h
loc_1_A638:
        sub    esp, 4
        push   esi
        push   edi
        cmp    dword ptr [0xc2f8], 2         ; -> loc_1_C2F8
        je     loc_1_A650
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_A650:
        mov    edi, dword ptr [esp + 0x18]
        push   edi
        call   sub_1_7044
        add    esp, 4
        cmp    al, 0xff
        je     loc_1_A734
        test   al, 1
        jne    loc_1_A734
        cmp    dword ptr [esp + 0x24], 8
        jne    loc_1_A72C
        mov    eax, dword ptr [esp + 0x20]
        cmp    dword ptr [eax + 0xc], 8
        jne    loc_1_A72C
        mov    esi, dword ptr [eax + 8]
        push   8
        push   esi
        call   sub_1_5C30
        add    esp, 8
        cmp    ax, 0xffff
        je     loc_1_A6A4
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_A6A4:
        mov    al, byte ptr [esi]
        cmp    al, 0x11
        je     loc_1_A6B8
        cmp    al, 0x12
        je     loc_1_A6B8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_A6B8:
        cmp    dword ptr [esp + 0x1c], edi
        jne    loc_1_A6C8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_A6C8:
        mov    ecx, dword ptr [esp + 0x10]
        test   byte ptr [ecx + 0x14], 1
        je     loc_1_A736
        cmp    al, 0x11
        jne    loc_1_A700
        mov    esi, dword ptr [ecx + 0x40]
        or     esi, esi
        je     loc_1_A736
loc_1_A6DD:
        cmp    dword ptr [esi + 4], 0x10000e0
        je     loc_1_A6F4
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_1_A6F4
        call   sub_1_A5C8
        mov    dword ptr [esi + 0xc], eax
loc_1_A6F4:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_A6DD
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_1_A700:
        mov    eax, dword ptr [esi + 4]
        cmp    eax, dword ptr [esp + 0x14]
        jne    loc_1_A736
        lea    edx, [esp + 8]
        push   edx
        push   eax
        push   ecx
        call   sub_1_A610
        add    esp, 0xc
        or     eax, eax
        je     loc_1_A736
        mov    dword ptr [eax + 0xc], 0
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_A72C:
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_1_A734:
        sub    eax, eax
loc_1_A736:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
sub_1_A73C:
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        cmp    esi, 0x10000e0
        je     loc_1_A7D1
        lea    eax, [esp + 8]
        push   eax
        push   8
        call   sub_1_917C
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     loc_1_A7D1
        mov    byte ptr [edi], 0x12
        mov    byte ptr [edi + 1], 0
        mov    word ptr [edi + 2], 0
        mov    dword ptr [edi + 4], esi
        push   8
        push   edi
        call   sub_1_5C30
        add    esp, 8
        not    ax
        mov    word ptr [edi + 2], ax
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_1_070C
        add    esp, 4
        mov    byte ptr [esp + 0x15], 1
        push   0
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   esi
        push   8
        push   dword ptr [esp + 0x1c]
        push   0
        push   dword ptr [0xc04c]            ; -> loc_1_C04C
        call   sub_1_6848
        add    esp, 0x20
        cmp    eax, 0x2bf7
        je     loc_1_A7D1
        push   dword ptr [esp + 8]
        push   0
        call   sub_1_92D4
        add    esp, 8
loc_1_A7D1:
        pop    edi
        pop    esi
        add    esp, 0x10
        ret    
        db  090h
sub_1_A7D8:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0xc2f8], 2         ; -> loc_1_C2F8
        je     loc_1_A7F8
        mov    eax, 0x2b03
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_A7F8:
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0
        cmp    byte ptr [0xc2f0], 0          ; -> loc_1_C2F0
        je     loc_1_A84C
        mov    eax, 0xc0b0                   ; -> loc_1_C0B0
        cmp    dword ptr [0xc0b0], 0         ; -> loc_1_C0B0
        je     loc_1_A82F
        nop    
loc_1_A828:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    loc_1_A828
loc_1_A82F:
        lea    ecx, [esp + 0x14]
        mov    dword ptr [esp + 0x14], 0
        mov    dword ptr [eax], ecx
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_B950
        add    esp, 4
        jmp    loc_1_A853
loc_1_A84C:
        mov    byte ptr [0xc2f0], 1          ; -> loc_1_C2F0
loc_1_A853:
        sub    esi, esi
        mov    edi, dword ptr [esp + 0x28]
        test   byte ptr [edi + 0x14], 1
        jne    loc_1_A870
        cmp    dword ptr [esp + 0x30], 2
        je     loc_1_A870
        mov    esi, 0x2b03
        jmp    loc_1_A9D1
loc_1_A870:
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x30]
        push   edi
        call   sub_1_A610
        add    esp, 0xc
        mov    ebx, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [esp + 0x30]
        or     ecx, ecx
        je     loc_1_A8A4
        cmp    ecx, 1
        je     loc_1_A958
        cmp    ecx, 2
        je     loc_1_A99C
        jmp    loc_1_A9D1
loc_1_A8A4:
        or     eax, eax
        jne    loc_1_A950
        mov    al, byte ptr [esp + 0x2c]
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_A8C0
        mov    esi, 0x2b03
        jmp    loc_1_A9D1
loc_1_A8C0:
        push   0x10
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_A944
        push   0
        push   dword ptr [esp + 0x30]
        push   1
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 0x18]
        add    esp, 0x10
        or     eax, eax
        je     loc_1_A930
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [ebp + 8], 1
        mov    dword ptr [ebp + 0xc], 0
        mov    eax, dword ptr [edi + 0x40]
        mov    dword ptr [ebp], eax
        mov    dword ptr [edi + 0x40], ebp
        cmp    dword ptr [esp + 0x2c], 0x10000e0
        je     loc_1_A9D1
        call   sub_1_A5C8
        mov    dword ptr [ebp + 0xc], eax
        push   dword ptr [edi + 4]
        push   dword ptr [esp + 0x30]
        call   sub_1_A73C
        add    esp, 8
        jmp    loc_1_A9D1
        db  08Dh, 049h, 000h
loc_1_A930:
        push   ebp
        call   sub_1_B944
        add    esp, 4
        mov    esi, 0x2afe
        jmp    loc_1_A9D1
        db  090h
loc_1_A944:
        mov    esi, 0x2afe
        jmp    loc_1_A9D1
        db  08Bh, 0FFh
loc_1_A950:
        inc    dword ptr [eax + 8]
        jmp    loc_1_A9D1
loc_1_A958:
        or     eax, eax
        je     loc_1_A994
        cmp    dword ptr [esp + 0x2c], 0x10000e0
        je     loc_1_A9D1
        mov    ecx, dword ptr [eax + 8]
        dec    ecx
        mov    dword ptr [eax + 8], ecx
        jne    loc_1_A9D1
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [edx], ecx
        push   eax
        call   sub_1_B944
        add    esp, 4
        push   0
        push   dword ptr [esp + 0x30]
        push   1
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 0x1c]
        add    esp, 0x10
        jmp    loc_1_A9D1
        db  090h
loc_1_A994:
        mov    esi, 0x2b03
        jmp    loc_1_A9D1
        db  090h
loc_1_A99C:
        mov    ebp, dword ptr [edi + 0x40]
        mov    dword ptr [edi + 0x40], 0
        or     ebp, ebp
        je     loc_1_A9D1
loc_1_A9AA:
        push   0
        push   dword ptr [ebp + 4]
        push   1
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 0x1c]
        add    esp, 0x10
        mov    dword ptr [esp + 0x10], ebp
        mov    ebp, dword ptr [ebp]
        push   dword ptr [esp + 0x10]
        call   sub_1_B944
        add    esp, 4
        or     ebp, ebp
        jne    loc_1_A9AA
loc_1_A9D1:
        mov    ecx, dword ptr [0xc0b0]       ; -> loc_1_C0B0
        or     ecx, ecx
        je     loc_1_A9FC
        mov    eax, ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [0xc0b0], eax       ; -> loc_1_C0B0
        push   0
        add    ecx, 4
        push   ecx
        call   sub_1_B95C
        add    esp, 8
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_A9FC:
        mov    eax, esi
        mov    byte ptr [0xc2f0], 0          ; -> loc_1_C2F0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
sub_1_AA10:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0xc2f8], 2         ; -> loc_1_C2F8
        jne    loc_1_AA70
        mov    esi, dword ptr [esp + 0x10]
        lea    ebx, [esi + 0x40]
        mov    edi, dword ptr [ebx]
        or     edi, edi
        je     loc_1_AA70
loc_1_AA29:
        mov    eax, dword ptr [edi + 0xc]
        or     eax, eax
        je     loc_1_AA68
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_AA68
        test   byte ptr [esi + 0x14], 1
        je     loc_1_AA68
        inc    dword ptr [edi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [edi + 4]
        call   sub_1_A73C
        add    esp, 8
        mov    eax, dword ptr [edi + 8]
        dec    eax
        mov    dword ptr [edi + 8], eax
        jne    loc_1_AA68
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx], eax
        push   edi
        call   sub_1_B944
        add    esp, 4
        mov    edi, dword ptr [ebx]
        jmp    loc_1_AA6C
        db  090h
loc_1_AA68:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
loc_1_AA6C:
        or     edi, edi
        jne    loc_1_AA29
loc_1_AA70:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_AA74:
        cmp    dword ptr [0xc2f8], 2         ; -> loc_1_C2F8
        jne    loc_1_AA90
        push   0
        push   0x10000e0
        push   dword ptr [esp + 0xc]
        call   sub_1_A7D8
        add    esp, 0xc
loc_1_AA90:
        cmp    dword ptr [0xbff0], 0         ; -> loc_1_BFF0
        jne    loc_1_AAC1
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [0xbff0], eax       ; -> loc_1_BFF0
        call   sub_1_B92C
        add    eax, dword ptr [0xbff0]       ; -> loc_1_BFF0
        mov    ecx, 0x5f5e100
        cdq    
        idiv   ecx
        or     edx, 1
        mov    dword ptr [0xbff4], edx       ; -> loc_1_BFF4
loc_1_AAC1:
        ret    
        db  08Bh, 0FFh
sub_1_AAC4:
        cmp    dword ptr [0xc2f8], 2         ; -> loc_1_C2F8
        jne    loc_1_AADD
        push   2
        push   0
        push   dword ptr [esp + 0xc]
        call   sub_1_A7D8
        add    esp, 0xc
loc_1_AADD:
        ret    
        db  08Bh, 0FFh
sub_1_AAE0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x28]
        mov    esi, dword ptr [eax]
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x24]
        call   sub_1_BA44
        mov    edx, dword ptr [esp + 0x10]
        add    edx, esi
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, esi
        mov    ebx, dword ptr [esp + 0x24]
loc_1_AB10:
        mov    ecx, eax
        sub    ecx, ebx
        sbb    esi, esi
        and    ecx, esi
        lea    ebp, [ecx + ebx]
        mov    edi, edx
        mov    esi, dword ptr [esp + 0x20]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, ebp
        add    dword ptr [esp + 0x20], ebp
        sub    ebx, ebp
        je     loc_1_AB64
        sub    eax, ebp
        jne    loc_1_AB10
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x24]
        call   sub_1_BA44
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x14]
        jmp    loc_1_AB10
loc_1_AB64:
        sub    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x1c]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_AB7C:
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x54]
        mov    ebp, dword ptr [eax]
        sub    ebx, ebx
        mov    dword ptr [esp + 0x10], ebx
        mov    esi, dword ptr [esp + 0x4c]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        cmp    ecx, 0x301
        je     loc_1_ABF0
        cmp    ecx, 0x380
        je     loc_1_ABE8
        mov    ebx, dword ptr [0xc0b4]       ; -> obj1:0xC0B4
        or     ebx, ebx
        je     loc_1_ABDA
        mov    edi, dword ptr [esp + 0x50]
loc_1_ABB6:
        push   dword ptr [esp + 0x58]
        push   dword ptr [esp + 0x58]
        push   edi
        push   esi
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 0x20]
        add    esp, 0x14
        cmp    eax, 0xfe
        jne    loc_1_AFC1
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_1_ABB6
loc_1_ABDA:
        pop    ebp
        mov    eax, 0xfe
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  090h
loc_1_ABE8:
        cmp    ecx, 0x301
        jne    loc_1_ABFC
loc_1_ABF0:
        cmp    eax, dword ptr [0xc238]       ; -> obj1:0xC238
        jne    loc_1_AFBC
loc_1_ABFC:
        cmp    eax, dword ptr [0xc14c]       ; -> obj1:0xC14C
        jne    loc_1_AFBC
        mov    eax, dword ptr [esp + 0x54]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 0x100
        jne    loc_1_AC8C
        cmp    dword ptr [esi + 0xc], 0x100
        jne    loc_1_AC7C
        cmp    dword ptr [esi + 0x10], 1
        jne    loc_1_AC7C
        cmp    ebp, 4
        jb     loc_1_AC6C
        sub    ecx, 0x301
        cmp    ecx, 1
        sbb    eax, eax
        and    eax, 0xffffff83
        add    eax, 0x380
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x10]
        push   eax
        push   4
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x5c]
        call   sub_1_AAE0
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Dh, 049h, 000h
loc_1_AC6C:
        pop    ebp
        mov    eax, 0x21
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Dh, 049h, 000h
loc_1_AC7C:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Dh, 049h, 000h
loc_1_AC8C:
        cmp    eax, 0x200
        jne    loc_1_AFAC
        cmp    dword ptr [esi + 0xc], 0x100
        jne    loc_1_AFAC
        cmp    ecx, 0x380
        jne    loc_1_AD18
        cmp    dword ptr [esi + 0x10], 1
        je     loc_1_ACC0
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  090h
loc_1_ACC0:
        cmp    ebp, 0x68
        jae    loc_1_ACD4
        mov    eax, 0x21
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Bh, 0FFh
loc_1_ACD4:
        lea    eax, [esp + 0x10]
        push   eax
        push   0x34
        push   0xbfb0                        ; -> loc_1_BFB0
        push   dword ptr [esp + 0x5c]
        call   sub_1_AAE0
        add    esp, 0x10
        lea    ecx, [esp + 0x10]
        push   ecx
        push   0x34
        push   0xbf70                        ; -> loc_1_BF70
        push   eax
        call   sub_1_AAE0
        add    esp, 0x10
        mov    eax, dword ptr [esp + 0x54]
        mov    dword ptr [eax], 0x68
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Dh, 049h, 000h
loc_1_AD18:
        mov    eax, dword ptr [esi + 0x10]
        cmp    eax, 1
        je     loc_1_AD4C
        cmp    eax, 0x101
        je     loc_1_AD98
        cmp    eax, 0x102
        je     loc_1_AE10
        cmp    eax, 0x103
        je     loc_1_AEF0
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Bh, 0FFh
loc_1_AD4C:
        cmp    ebp, 0x5c
        jae    loc_1_AD60
        mov    eax, 0x21
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Bh, 0FFh
loc_1_AD60:
        mov    eax, dword ptr [0xc054]       ; -> loc_1_C054
        mov    dword ptr [0xc228], eax       ; -> loc_1_C228
        mov    eax, dword ptr [0xc154]       ; -> obj1:0xC154
        mov    dword ptr [0xc22c], eax       ; -> obj1:0xC22C
        lea    eax, [esp + 0x10]
        push   eax
        push   0x5c
        push   0xc1d8                        ; -> obj1:0xC1D8
        push   dword ptr [esp + 0x5c]
        call   sub_1_AAE0
        add    esp, 0x10
        mov    ebx, 0x5c
        sub    ebp, ebp
        jmp    loc_1_AF9B
loc_1_AD98:
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x5c]
        call   sub_1_7658
        add    esp, 8
        mov    esi, eax
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_ADC0
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  090h
loc_1_ADC0:
        or     esi, esi
        je     loc_1_AE02
        mov    edi, dword ptr [esp + 0x50]
loc_1_ADC8:
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, 0x30
        jl     loc_1_AE02
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x5c]
        call   sub_1_7544
        add    esp, 8
        mov    esi, eax
        add    ebx, 0x30
        lea    eax, [esp + 0x10]
        push   eax
        push   0x30
        lea    eax, [esp + 0x20]
        push   eax
        push   edi
        call   sub_1_AAE0
        add    esp, 0x10
        mov    edi, eax
        or     esi, esi
        jne    loc_1_ADC8
loc_1_AE02:
        cmp    esi, 1
        cmc    
        sbb    ebp, ebp
        and    ebp, 9
        jmp    loc_1_AF9B
loc_1_AE10:
        mov    esi, dword ptr [0xc090]       ; -> obj1:0xC090
        mov    eax, dword ptr [esp + 0x58]
        mov    ax, word ptr [eax]
        or     ax, ax
        je     loc_1_AE44
        or     esi, esi
        je     loc_1_AE36
loc_1_AE26:
        cmp    word ptr [esi + 0x16], ax
        je     loc_1_AE32
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_AE26
loc_1_AE32:
        or     esi, esi
        jne    loc_1_AE4C
loc_1_AE36:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  090h
loc_1_AE44:
        or     esi, esi
        je     loc_1_AED1
loc_1_AE4C:
        mov    edi, dword ptr [esp + 0x50]
loc_1_AE50:
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, 0x18
        jl     loc_1_AECD
        mov    ax, word ptr [esi + 0x14]
        test   al, 8
        je     loc_1_AEC7
        test   al, 1
        je     loc_1_AE78
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 0x20], eax
        jmp    loc_1_AE88
        db  08Dh, 049h, 000h
loc_1_AE78:
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x20], 0
loc_1_AE88:
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [eax + 0x30]
        mov    dword ptr [esp + 0x1c], ecx
        mov    eax, dword ptr [eax + 0x3c]
        and    eax, 1
        mov    dword ptr [esp + 0x24], eax
        mov    dword ptr [esp + 0x28], 0xffff
        mov    ax, word ptr [esi + 0x16]
        mov    word ptr [esp + 0x2c], ax
        lea    eax, [esp + 0x10]
        push   eax
        push   0x18
        lea    eax, [esp + 0x20]
        push   eax
        push   edi
        call   sub_1_AAE0
        add    esp, 0x10
        mov    edi, eax
        add    ebx, 0x18
loc_1_AEC7:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_AE50
loc_1_AECD:
        or     esi, esi
        jne    loc_1_AED8
loc_1_AED1:
        sub    ebp, ebp
        jmp    loc_1_AF9B
loc_1_AED8:
        mov    ebp, 9
        mov    ax, word ptr [esi + 0x16]
        mov    ecx, dword ptr [esp + 0x58]
        mov    word ptr [ecx], ax
        jmp    loc_1_AF9B
        db  08Dh, 049h, 000h
loc_1_AEF0:
        mov    esi, dword ptr [0xc090]       ; -> obj1:0xC090
        mov    eax, dword ptr [esp + 0x58]
        mov    eax, dword ptr [eax]
        or     esi, esi
        je     loc_1_AF15
loc_1_AF00:
        test   byte ptr [esi + 0x14], 1
        je     loc_1_AF0B
        cmp    dword ptr [esi + 4], eax
        je     loc_1_AF11
loc_1_AF0B:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_AF00
loc_1_AF11:
        or     esi, esi
        jne    loc_1_AF20
loc_1_AF15:
        mov    ebp, 0x1a
        jmp    loc_1_AF9B
        db  090h
loc_1_AF20:
        cmp    ebp, 0x10
        jae    loc_1_AF2C
        mov    ebp, 0x21
        jmp    loc_1_AF9B
loc_1_AF2C:
        mov    esi, dword ptr [esi + 0xc]
        mov    eax, dword ptr [esi + 0x48]
        and    eax, 1
        or     eax, eax
        mov    eax, 0
        setne  al
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esi + 0x40]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esi + 0x44]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [esi + 0x4c]
        mov    dword ptr [esp + 0x24], eax
        mov    ebx, 0x10
        add    eax, ebx
        cmp    eax, ebp
        mov    ebp, 0x21
        ja     loc_1_AF9B
        sub    ebp, ebp
        lea    eax, [esp + 0x10]
        push   eax
        push   ebx
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x5c]
        call   sub_1_AAE0
        add    esp, 0x10
        lea    ecx, [esp + 0x10]
        push   ecx
        push   dword ptr [esi + 0x4c]
        push   dword ptr [esi + 0x50]
        push   eax
        call   sub_1_AAE0
        add    esp, 0x10
        mov    ebx, dword ptr [esi + 0x4c]
        add    ebx, 0x10
loc_1_AF9B:
        mov    eax, dword ptr [esp + 0x54]
        mov    dword ptr [eax], ebx
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  090h
loc_1_AFAC:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Dh, 049h, 000h
loc_1_AFBC:
        mov    eax, 0xfe
loc_1_AFC1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Dh, 049h, 000h
loc_1_AFCC:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ebx + 4]
        cmp    dword ptr [ebx], 0x301
        je     loc_1_B020
        mov    ebp, dword ptr [0xc0b4]       ; -> obj1:0xC0B4
        or     ebp, ebp
        je     loc_1_B012
        mov    esi, dword ptr [esp + 0x20]
        mov    edi, dword ptr [esp + 0x24]
loc_1_AFF4:
        push   edi
        push   esi
        push   ebx
        push   dword ptr [ebp + 4]
        call   dword ptr [ebp + 0x24]
        add    esp, 0x10
        cmp    eax, 0xfe
        jne    loc_1_B2A1
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    loc_1_AFF4
loc_1_B012:
        pop    ebp
        mov    eax, 0xfe
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_B020:
        cmp    eax, dword ptr [0xc238]       ; -> obj1:0xC238
        je     loc_1_B038
        mov    eax, 0xfe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B038:
        mov    eax, dword ptr [ebx + 0x10]
        cmp    eax, 0x101
        jne    loc_1_B250
        cmp    dword ptr [esp + 0x24], 0x30
        jae    loc_1_B05C
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_B05C:
        sub    edi, edi
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, ebx
        je     loc_1_B240
        cmp    al, 0x7f
        je     loc_1_B240
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_B240
        cmp    al, 0xf0
        jne    loc_1_B09A
        cmp    dword ptr [esp + 0x10], -1
        jne    loc_1_B240
loc_1_B09A:
        cmp    byte ptr [esp + 0x14], 0x7f
        je     loc_1_B230
        mov    al, byte ptr [esp + 0x14]
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_B230
        cmp    al, 0xf0
        jne    loc_1_B0C2
        cmp    dword ptr [esp + 0x14], -1
        jne    loc_1_B230
loc_1_B0C2:
        mov    eax, dword ptr [0xc140]       ; -> obj1:0xC140
        cmp    dword ptr [esi + 4], eax
        jne    loc_1_B0DC
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B0DC:
        mov    ebp, dword ptr [0xc090]       ; -> obj1:0xC090
        or     ebp, ebp
        je     loc_1_B131
loc_1_B0E6:
        or     edi, edi
        jne    loc_1_B0F7
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 4]
        cmp    dword ptr [eax + 0x30], ecx
        jne    loc_1_B0F7
        mov    edi, ebp
loc_1_B0F7:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [ebp + 4], eax
        jne    loc_1_B108
        test   byte ptr [ebp + 0x14], 1
        je     loc_1_B108
        mov    ebx, ebp
loc_1_B108:
        push   ebp
        push   dword ptr [esp + 0x14]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    loc_1_B144
        push   ebp
        push   dword ptr [esp + 0x18]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    loc_1_B154
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    loc_1_B0E6
loc_1_B131:
        or     edi, edi
        jne    loc_1_B164
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_B144:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B154:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B164:
        or     ebx, ebx
        je     loc_1_B1AC
        mov    eax, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [esi + 4]
        cmp    dword ptr [eax + 0x30], ecx
        je     loc_1_B180
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_B180:
        mov    eax, dword ptr [esi + 0x1c]
        cmp    eax, 3
        je     loc_1_B19C
        cmp    eax, 2
        je     loc_1_B19C
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_B19C:
        mov    edi, ebx
        or     ebx, ebx
        je     loc_1_B1AC
        mov    eax, 0xffffffff
        jmp    loc_1_B1BF
        db  08Dh, 049h, 000h
loc_1_B1AC:
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [esp + 0x10], eax
        mov    eax, 0xffffffff
        je     loc_1_B1BF
        mov    eax, dword ptr [esp + 0x10]
loc_1_B1BF:
        cmp    dword ptr [esi + 0x1c], 2
        je     loc_1_B1E8
        push   1
        push   dword ptr [esi + 0x20]
        push   dword ptr [esi + 8]
        movzx  ecx, word ptr [edi + 0x3c]
        push   ecx
        push   dword ptr [edi + 0xc]
        push   eax
        push   dword ptr [esi + 0x28]
        push   dword ptr [esp + 0x30]
        call   sub_1_7F60
        add    esp, 0x20
        jmp    loc_1_B1FB
        db  090h
loc_1_B1E8:
        push   dword ptr [edi + 0xc]
        push   eax
        push   dword ptr [esi + 0x28]
        push   dword ptr [esp + 0x20]
        call   sub_1_7F8C
        add    esp, 0x10
loc_1_B1FB:
        or     eax, eax
        jne    loc_1_B20C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B20C:
        cmp    eax, 0x2afe
        jne    loc_1_B220
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_B220:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B230:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B240:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B250:
        cmp    eax, 1
        jne    loc_1_B29C
        mov    esi, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esi + 4]
        cmp    eax, 0xff
        ja     loc_1_B28C
        cmp    byte ptr [0xc1d4], 0          ; -> obj1:0xC1D4
        jne    loc_1_B271
        cmp    dword ptr [esi], 1
        je     loc_1_B28C
loc_1_B271:
        pop    ebp
        mov    dword ptr [0xc158], eax       ; -> obj1:0xC158
        cmp    dword ptr [esi], 1
        sete   al
        mov    byte ptr [0xbfa4], al         ; -> loc_1_BFA4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_B28C:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B29C:
        mov    eax, 0x1a
loc_1_B2A1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_B2AC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x18]
        sub    edi, edi
        sub    ebx, ebx
        cmp    edx, ebx
        je     loc_1_B302
        mov    dword ptr [esp + 0x10], edx
loc_1_B2C9:
        mov    eax, dword ptr [ecx]
        cmp    eax, 0x301
        jne    loc_1_B2E4
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        sub    eax, edi
        sbb    ebp, ebp
        not    ebp
        and    eax, ebp
        add    edi, eax
        jmp    loc_1_B2F9
        db  08Bh, 0FFh
loc_1_B2E4:
        cmp    eax, 0x380
        jne    loc_1_B2F9
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        sub    eax, ebx
        sbb    ebp, ebp
        not    ebp
        and    eax, ebp
        add    ebx, eax
loc_1_B2F9:
        add    ecx, 8
        dec    dword ptr [esp + 0x10]
        jne    loc_1_B2C9
loc_1_B302:
        mov    dword ptr [0xc238], edi       ; -> obj1:0xC238
        mov    dword ptr [0xc14c], ebx       ; -> obj1:0xC14C
        add    edx, 2
        cmp    edx, 0x20
        jbe    loc_1_B324
        mov    eax, 0x20
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_B324:
        mov    dword ptr [ecx], 0x301
        mov    dword ptr [ecx + 4], edi
        add    ecx, 8
        mov    dword ptr [ecx], 0x380
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [esi], edx
        mov    edi, dword ptr [0xc0b4]       ; -> obj1:0xC0B4
        or     edi, edi
        je     loc_1_B35D
loc_1_B345:
        push   esi
        push   dword ptr [esp + 0x1c]
        push   dword ptr [edi + 4]
        call   dword ptr [edi + 0x28]
        add    esp, 0xc
        or     eax, eax
        je     loc_1_B368
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_B345
loc_1_B35D:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_B368:
        pop    ebp
        mov    eax, 0x21
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_B378:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B94A
        push   0x30
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_B3A0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_B3A0:
        mov    dword ptr [esp + 0x20], 0xbd9c ; -> loc_1_BD9C
        mov    word ptr [esp + 0x1c], 6
        mov    word ptr [esp + 0x1e], 6
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_B9BA
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_B3E4
        push   esi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_B3E4:
        mov    dword ptr [esp + 0x20], 0xbf44 ; -> obj1:0xBF44
        mov    word ptr [esp + 0x1c], 0xd
        mov    word ptr [esp + 0x1e], 0xd
        push   0
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_B428
        mov    dword ptr [0xc144], 0         ; -> obj1:0xC144
        jmp    loc_1_B434
        db  090h
loc_1_B428:
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [0xc144], eax       ; -> obj1:0xC144
loc_1_B434:
        mov    dword ptr [esp + 0x20], 0xbf54 ; -> loc_1_BF54
        mov    word ptr [esp + 0x1c], 0x15
        mov    word ptr [esp + 0x1e], 0x15
        push   0
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_B478
        mov    dword ptr [0xc23c], 0         ; -> obj1:0xC23C
        jmp    loc_1_B484
        db  090h
loc_1_B478:
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [0xc23c], eax       ; -> obj1:0xC23C
loc_1_B484:
        mov    ebx, 0xbe2c                   ; -> obj1:0xBE2C
loc_1_B489:
        mov    edi, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 0xc]
        add    ebp, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   dword ptr [ebx + 4]
        mov    word ptr [esp + 0x20], cx
        mov    ax, cx
        mov    word ptr [esp + 0x22], ax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 0x24], eax
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_B4DC
        cmp    dword ptr [ebx + 0x10], 0
        jne    loc_1_B504
        mov    eax, dword ptr [ebx + 8]
        jmp    loc_1_B4E3
        db  090h
loc_1_B4DC:
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 4]
loc_1_B4E3:
        mov    dword ptr [ebp], eax
        add    ebx, 0x14
        cmp    ebx, 0xbee0                   ; -> loc_1_BEE0
        jb     loc_1_B489
        push   dword ptr [esp + 0x18]
        call   sub_1_BA80
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_B504:
        push   dword ptr [esp + 0x18]
        call   sub_1_BA80
        push   esi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
sub_1_B520:
        sub    esp, 4
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 0x10]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_B9BA
        cmp    dword ptr [esp], 0
        jne    loc_1_B548
        mov    eax, 1
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_B548:
        sub    eax, eax
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_B550:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, ecx
        sub    esi, esi
loc_1_B55D:
        sub    ebx, ebx
        nop    
loc_1_B560:
        mov    al, byte ptr [edx]
        cmp    al, 0x30
        jl     loc_1_B584
        cmp    al, 0x39
        jg     loc_1_B584
        movsx  eax, al
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [eax + ecx*2 - 0x30]
        cmp    ecx, 0xff
        ja     loc_1_B5C0
        inc    edx
        mov    ebx, 1
        jmp    loc_1_B560
loc_1_B584:
        movsx  eax, al
        or     eax, eax
        je     loc_1_B5C8
        cmp    eax, 9
        je     loc_1_B5A8
        cmp    eax, 0x20
        je     loc_1_B5A8
        cmp    eax, 0x2c
        je     loc_1_B5C8
        cmp    eax, 0x2e
        je     loc_1_B5AC
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_B5A8:
        inc    edx
        jmp    loc_1_B560
        db  090h
loc_1_B5AC:
        cmp    esi, 3
        jae    loc_1_B5F4
        or     ebx, ebx
        je     loc_1_B5F4
        inc    edx
        shl    edi, 8
        add    edi, ecx
        sub    ecx, ecx
        inc    esi
        jmp    loc_1_B55D
loc_1_B5C0:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_B5C8:
        cmp    esi, 3
        jne    loc_1_B5EC
        or     ebx, ebx
        je     loc_1_B5EC
        shl    edi, 8
        add    ecx, edi
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], edx
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_B5EC:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_B5F4:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_B5FC:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, 0xbef8                   ; -> loc_1_BEF8
        mov    esi, dword ptr [esp + 0x2c]
loc_1_B60C:
        mov    edi, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 0xc]
        add    ebp, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   dword ptr [ebx + 4]
        mov    word ptr [esp + 0x24], cx
        mov    ax, cx
        mov    word ptr [esp + 0x26], ax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 0x28], eax
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x38]
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_B660
        cmp    dword ptr [ebx + 0x10], 0
        jne    loc_1_B6CC
        mov    eax, dword ptr [ebx + 8]
        jmp    loc_1_B667
        db  08Bh, 0FFh
loc_1_B660:
        mov    eax, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 4]
loc_1_B667:
        mov    dword ptr [ebp], eax
        add    ebx, 0x14
        cmp    ebx, 0xbf20                   ; -> loc_1_BF20
        jb     loc_1_B60C
        mov    dword ptr [esi + 8], 0
        lea    edi, [esi + 0xc]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    word ptr [esp + 0x20], 0xf
        mov    word ptr [esp + 0x22], 0xf
        mov    dword ptr [esp + 0x24], 0xbf34 ; -> loc_1_BF34
        push   2
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x38]
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_B6D8
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_B6CC:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_B6D8:
        sub    edi, edi
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [eax + 8]
loc_1_B6E1:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   ecx
        call   sub_1_B550
        add    esp, 0xc
        or     eax, eax
        je     loc_1_B718
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [esi + ecx*4 + 0xc], eax
        inc    dword ptr [esi + 8]
        mov    eax, dword ptr [esp + 0x10]
        cmp    byte ptr [eax], 0
        je     loc_1_B718
        mov    ecx, eax
        inc    ecx
        inc    edi
        cmp    edi, 5
        jb     loc_1_B6E1
loc_1_B718:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
sub_1_B728:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 0x20], 0xa
        mov    word ptr [esp + 0x22], 0xa
        mov    dword ptr [esp + 0x24], 0xbf20 ; -> loc_1_BF20
        push   2
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x38]
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_B770
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
loc_1_B770:
        mov    esi, 1
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + 8]
        cmp    byte ptr [eax], 0
        je     loc_1_B78D
loc_1_B781:
        cmp    byte ptr [eax], 0x2c
        jne    loc_1_B787
        inc    esi
loc_1_B787:
        inc    eax
        cmp    byte ptr [eax], 0
        jne    loc_1_B781
loc_1_B78D:
        lea    eax, [esi*8]
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_B7B0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Dh, 049h, 000h
loc_1_B7B0:
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [eax + 8]
        cmp    esi, ebx
        je     loc_1_B7F2
        mov    ebp, edi
        nop    
loc_1_B7C0:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_B550
        add    esp, 0xc
        or     eax, eax
        je     loc_1_B810
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 0x10]
        cmp    byte ptr [eax], 0
        je     loc_1_B7F2
        mov    ecx, eax
        inc    ecx
        add    ebp, 8
        inc    ebx
        cmp    esi, ebx
        ja     loc_1_B7C0
loc_1_B7F2:
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 1
        je     loc_1_B824
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_B810:
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  090h
loc_1_B824:
        mov    dword ptr [esp + 0x24], 0xbf2c ; -> loc_1_BF2C
        mov    word ptr [esp + 0x20], 7
        mov    word ptr [esp + 0x22], 7
        push   2
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x38]
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_BA7A
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_B870
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_B870:
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [eax + 8]
        cmp    esi, ebx
        je     loc_1_B8B2
        lea    ebp, [edi + 4]
loc_1_B880:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_B550
        add    esp, 0xc
        or     eax, eax
        je     loc_1_B8D0
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 0x10]
        cmp    byte ptr [eax], 0
        je     loc_1_B8B2
        mov    ecx, eax
        inc    ecx
        add    ebp, 8
        inc    ebx
        cmp    esi, ebx
        ja     loc_1_B880
loc_1_B8B2:
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 1
        je     loc_1_B8E4
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  08Bh, 0FFh
loc_1_B8D0:
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  090h
loc_1_B8E4:
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [eax], esi
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
sub_1_B8F4:
        push   dword ptr [esp + 4]
        call   sub_1_BA80
        ret    
        db  08Bh, 0FFh
sub_1_B900:
        push   dword ptr [esp + 4]
        call   sub_1_B944
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_B910:
        mov    eax, dword ptr [0xc144]       ; -> obj1:0xC144
        ret    
        db  08Bh, 0FFh
sub_1_B918:
        mov    eax, dword ptr [0xc23c]       ; -> obj1:0xC23C
        ret    
        db  08Bh, 0FFh
sub_1_B920:
        VxDCall  Dev_0488.Service_8001h          ; cd2001808804
sub_1_B926:
        VxDCall  Dev_0488.Service_8002h          ; cd2002808804
sub_1_B92C:
        VMMCall  VMM.Get_System_Time             ; cd203f800100
sub_1_B932:
        VxDCall  Dev_0488.Service_8003h          ; cd2003808804
        VxDCall  Dev_0488.Service_8004h          ; cd2004808804
sub_1_B93E:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_B944:
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
sub_1_B94A:
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
sub_1_B950:
        VxDCall  Dev_0488.Service_8005h          ; cd2005808804
        VxDCall  Dev_0488.Service_800Dh          ; cd200d808804
sub_1_B95C:
        VxDCall  Dev_0488.Service_8006h          ; cd2006808804
sub_1_B962:
        mov    eax, dword ptr [0xbf6c]       ; -> loc_1_BF6C
        VxDCall  Dev_0488.Service_8009h          ; cd2009808804
        mov    eax, dword ptr [0xbf6c]       ; -> loc_1_BF6C
        VxDCall  Dev_0488.Service_800Ch          ; cd200c808804
        mov    eax, dword ptr [0xbf6c]       ; -> loc_1_BF6C
        VxDCall  Dev_0488.Service_800Bh          ; cd200b808804
        add    ch, cl
        and    byte ptr [ecx], ch
        sub    byte ptr [eax], 0
sub_1_B98A:
        VxDCall  VPOWERD.Service_802Ah           ; cd202a802800
sub_1_B990:
        VxDCall  VPOWERD.Service_802Bh           ; cd202b802800
sub_1_B996:
        VxDCall  VPOWERD.Service_802Ch           ; cd202c802800
sub_1_B99C:
        VxDCall  VPOWERD.Service_802Dh           ; cd202d802800
sub_1_B9A2:
        VxDCall  VPOWERD.Service_802Eh           ; cd202e802800
        VxDCall  VPOWERD.Service_802Fh           ; cd202f802800
sub_1_B9AE:
        VxDCall  VPOWERD.Service_8030h           ; cd2030802800
        VxDCall  VPOWERD.Service_8050h           ; cd2050802800
sub_1_B9BA:
        VxDCall  VPOWERD.Service_8051h           ; cd2051802800
        VxDCall  VPOWERD.Service_8052h           ; cd2052802800
        VxDCall  VPOWERD.Service_8053h           ; cd2053802800
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
sub_1_BA02:
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
sub_1_BA08:
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
sub_1_BA0E:
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
sub_1_BA14:
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
sub_1_BA1A:
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_BA20:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_BA26:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_BA2C:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_BA32:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
sub_1_BA3E:
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_BA44:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
sub_1_BA50:
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
sub_1_BA56:
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
sub_1_BA5C:
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_BA68:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_BA7A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_BA80:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
        dd  VIP_Service_00           ; fixup
        dd  VIP_Service_01           ; fixup
        dd  VIP_Service_02           ; fixup
        dd  VIP_Service_03           ; fixup
        dd  VIP_Service_04           ; fixup
        dd  VIP_Service_05           ; fixup
        dd  VIP_Service_06           ; fixup
        dd  VIP_Service_07           ; fixup
        dd  VIP_Service_08           ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00489h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VIP     '                  ; DDB_Name
        dd  054008000h                  ; DDB_Init_Order
        dd  VIP_Control              ; DDB_Control_Proc
        dd  VIP_V86_API              ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0xBADC              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 054h, 068h, 065h, 020h, 073h, 079h, 073h, 074h
        db  065h, 06Dh, 020h, 068h, 061h, 073h, 020h, 064h, 065h, 074h, 065h, 063h, 074h, 065h, 064h, 020h
        db  061h, 020h, 063h, 06Fh, 06Eh, 066h, 06Ch, 069h, 063h, 074h, 020h, 077h, 069h, 074h, 068h, 020h
        db  074h, 068h, 065h, 020h, 073h, 079h, 073h, 074h, 065h, 06Dh, 020h, 068h, 061h, 076h, 069h, 06Eh
        db  067h, 020h, 068h, 061h, 072h, 064h, 077h, 061h, 072h, 065h, 020h, 061h, 064h, 064h, 072h, 065h
        db  073h, 073h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 020h, 066h, 06Fh, 072h, 020h, 049h, 050h, 020h, 061h, 064h, 064h, 072h
        db  065h, 073h, 073h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 000h, 054h, 068h, 065h, 020h, 06Ch, 06Fh, 063h, 061h, 06Ch, 020h, 069h, 06Eh
        db  074h, 065h, 072h, 066h, 061h, 063h, 065h, 020h, 068h, 061h, 073h, 020h, 062h, 065h, 065h, 06Eh
        db  020h, 064h, 069h, 073h, 061h, 062h, 06Ch, 065h, 064h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
loc_1_BBFB:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax - 0x1000000], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 006h, 000h, 000h
loc_1_BC1C:
        adc    byte ptr [eax - 1], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 082h, 070h, 000h, 000h, 000h
        db  000h
loc_1_BC30:
        push   edi
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0D5h, 000h
loc_1_BC44:
        add    dword ptr [eax], eax
        pop    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_BC4C:
        push   edi
loc_1_BC4D:
        add    dword ptr [eax], eax
        pop    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BC54:
        stosb  byte ptr es:[edi], al
        stosb  byte ptr es:[edi], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BC60:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [esp + ecx], al
        add    cl, byte ptr [edx]
        push   es
        push   cs
        add    dword ptr [ecx], ecx
        add    eax, 0x70b030d
        sldt   word ptr [eax - 0x5fdf3fc0]
        pushal 
        loopne loc_1_BC91
        nop    
        push   eax
        sal    byte ptr [eax], 1
        mov    al, 0x70
        db  0F0h
loc_1_BC88:
        add    al, 2
        fadd   qword ptr [0x11780804]
        db  0FFh
loc_1_BC91:
        pop    ds
        add    byte ptr [eax], al
loc_1_BC94:
        add    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    byte ptr [0x359c0000], 0      ; -> loc_1_359C
        add    byte ptr [eax + 0x3c000035], bh ; -> loc_1_35B8
        add    byte ptr ss:[eax], al
        pop    esp
        add    byte ptr ss:[eax], al
        pushal 
        add    byte ptr ss:[eax], al
        jl     0xbcea
        add    byte ptr [eax], al
        les    esi, ptr [edi]
        add    byte ptr [eax], al
        int3   
        aaa    
        add    byte ptr [eax], al
        in     al, dx
        aaa    
        add    byte ptr [eax], al
        or     byte ptr [ecx], 0
        js     loc_1_BD12
        add    byte ptr [eax], al
        inc    eax
        dec    ebx
        add    byte ptr [eax], al
        push   es
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
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
        db  0FFh, 0FFh, 00Fh, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_BD00:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
loc_1_BD12:
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 0E0h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_BD40:
        jmp    0xbd41
        db  000h, 000h, 0ECh, 07Ch, 000h, 000h, 0E6h, 045h, 000h, 000h, 0D2h, 01Fh, 000h, 000h, 0ECh, 010h
        db  000h, 000h, 0BEh, 007h, 000h, 000h, 0C0h, 005h, 000h, 000h, 0DAh, 003h, 000h, 000h, 0E8h, 001h
        db  000h, 000h, 014h, 001h, 000h, 000h
loc_1_BD68:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_BD7C:
        dec    ebp
        push   ebx
        and    byte ptr [ebx + eax*2 + 0x50], dl
        and    byte ptr [edi + ebp*2 + 0x6f], cl
        jo     loc_1_BDEA
        popal  
        arpl   word ptr [ebx + 0x20], bp
        imul   ebp, dword ptr [esi + 0x74], 0x61667265
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
loc_1_BD98:
        add    al, byte ptr [edx]
        add    byte ptr [eax], al
loc_1_BD9C:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        db  000h
loc_1_BDA4:
        inc    ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x52]
        outsd  dx, dword ptr [esi]
        jne    0xbe22
        db  069h, 06Eh, 067h, 000h, 000h, 000h
loc_1_BDB4:
        inc    esi
        outsd  dx, dword ptr [esi]
        jb     0xbe2f
        popal  
        jb     loc_1_BE1F
        inc    edx
        jb     0xbe2d
        popal  
        arpl   word ptr fs:[ecx + 0x73], sp
        je     loc_1_BE38
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x61], al
        inc    edi
        push   edi
        inc    esp
        je     0xbe37
        arpl   word ptr [eax + eax], si
        add    byte ptr [eax], al
loc_1_BDD8:
        push   eax
        dec    ebp
        push   esp
        push   ebp
        inc    esp
        imul   esi, dword ptr [ebx + 0x63], 0x7265766f
        jns    loc_1_BDE6
loc_1_BDE6:
        add    byte ptr [eax], al
loc_1_BDE8:
        dec    ecx
        inc    edi
loc_1_BDEA:
        dec    ebp
        push   eax
        dec    esp
        jbe    0xbe55
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        db  000h
loc_1_BDF4:
        inc    esp
        popaw  
        jne    loc_1_BE66
        je     loc_1_BE50
        push   esp
        dec    esp
        add    byte ptr [eax], al
loc_1_BE00:
        inc    esp
        popaw  
        jne    loc_1_BE72
        je     loc_1_BE5C
        dec    edi
        push   ebx
        add    byte ptr [eax], al
loc_1_BE0C:
        push   edx
        outsd  dx, dword ptr [esi]
        jne    0xbe84
        imul   ebp, dword ptr gs:[esi + 0x67], 0x53667542
        imul   edi, dword ptr [edx + 0x65], 0x756f5200
loc_1_BE1F:
        je     0xbe86
        imul   ebp, dword ptr [esi + 0x67], 0x6b636150
        je     loc_1_BE9E
        add    byte ptr [ebp + edi*4], ah    ; -> loc_1_BDA4
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BE38:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    ah, 0xbd
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
loc_1_BE50:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        enter  0xbd, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BE5C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BE66:
        add    byte ptr [eax], al
        fdivr  dword ptr [ebp]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
loc_1_BE72:
        add    byte ptr [eax], al
        sbb    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        call   0xbf3e
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh                        ; -> loc_1_BDF4
        mov    ebp, 0
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        and    al, 0
loc_1_BE9E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BEB8:
        or     al, 0xbe
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    dword ptr [eax], eax
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    al, 0xbe
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BEE0:
        pop    edx
        jb     0xbf53
        inc    edx
        jb     loc_1_BF56
        popal  
        arpl   word ptr fs:[ecx + 0x73], sp
        je     loc_1_BEEE
loc_1_BEEE:
        add    byte ptr [eax], al
loc_1_BEF0:
        dec    ebp
        popal  
        js     loc_1_BF41
        push   esp
        push   ebp
        add    byte ptr [eax], al
loc_1_BEF8:
        loopne loc_1_BEB8
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dd  loc_1_BEF0               ; fixup
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_BF20:
        dec    ecx
        push   eax
        inc    ecx
        jb     loc_1_BF8C
        jae    loc_1_BF9C
        add    byte ptr [eax], al
        db  000h
loc_1_BF2C:
        dec    ecx
        push   eax
        dec    ebp
        popal  
        jae    0xbf9d
        add    byte ptr [eax], al
loc_1_BF34:
        inc    esp
        popaw  
        jne    loc_1_BFA6
        je     0xbf83
        popal  
        je     loc_1_BFA4
        ja     loc_1_BFA2
loc_1_BF41:
        jns    loc_1_BF43
loc_1_BF43:
        add    byte ptr [ebp + 0x74], al
        push   0x656e7265
        je     loc_1_BFA0
        dec    esi
        inc    ecx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF54:
        inc    ecx
        push   edx
loc_1_BF56:
        push   eax
        inc    ecx
        insb   byte ptr es:[edi], dx
        ja     loc_1_BFBC
        jns    loc_1_BFD0
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    0xbfd3
        arpl   word ptr [ebp + 0x52], sp
        outsd  dx, dword ptr [esi]
        jne    0xbfdb
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_BF6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF74:
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
loc_1_BF8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF9C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFA0:
        add    byte ptr [eax], al
loc_1_BFA2:
        add    byte ptr [eax], al
loc_1_BFA4:
        add    byte ptr [eax], al
loc_1_BFA6:
        add    byte ptr [eax], al
loc_1_BFA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFAC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFB0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFB4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFBC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFCC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BFFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_C000:
        inc    byte ptr [0xbbfb]
        jne    loc_1_C024
loc_1_C008:
        mov    edi, 0xbf70
        mov    ecx, 0xc300
        sub    ecx, edi
        shr    ecx, 2
        sub    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        call   sub_1_C2B8
        cmp    eax, 1
loc_1_C024:
        ret    
        db  080h, 03Fh, 000h, 074h, 03Ah, 080h, 03Fh, 020h, 074h, 005h, 080h, 03Fh, 009h, 075h, 003h, 047h
        db  0EBh, 0EEh, 080h, 03Fh, 02Ch, 074h, 02Bh, 08Bh, 0F7h, 047h
loc_1_C03F:
        cmp    byte ptr [edi], 0
        je     loc_1_C062
        cmp    byte ptr [edi], 0x2c
        jne    loc_1_C04F
        mov    byte ptr [edi], 0
loc_1_C04C:
        inc    edi
        jmp    loc_1_C062
loc_1_C04F:
        cmp    byte ptr [edi], 0x20
        je     loc_1_C05C
loc_1_C054:
        cmp    byte ptr [edi], 9
        je     loc_1_C05C
        inc    edi
        jmp    loc_1_C03F
loc_1_C05C:
        mov    byte ptr [edi], 0
        inc    edi
loc_1_C060:
        jmp    loc_1_C03F
loc_1_C062:
        clc    
        ret    
loc_1_C064:
        sub    esi, esi
        ret    
        db  0F9h
loc_1_C068:
        ret    
        db  050h, 0CDh, 020h
loc_1_C06C:
        mov    dl, 0
        add    dword ptr [eax], eax
        mov    edx, eax
        pop    eax
        ret    
        db  050h, 052h, 0CDh, 020h, 0ACh, 000h, 001h, 000h, 03Bh, 054h, 024h, 004h, 074h, 003h, 08Bh, 0D0h
        db  0F9h, 058h, 058h, 0F5h, 0C3h
sub_1_C089:
        cmp    byte ptr [eax], 0
loc_1_C08C:
        je     loc_1_C09B
        cmp    byte ptr [eax], 0x20
        je     loc_1_C098
        cmp    byte ptr [eax], 9
        jne    loc_1_C09B
loc_1_C098:
        inc    eax
        jmp    sub_1_C089
loc_1_C09B:
        ret    
loc_1_C09C:
        push   ebx
        push   ecx
        push   esi
        mov    eax, edx
        sub    ecx, ecx
        db  02Bh
loc_1_C0A4:
        shr    byte ptr [ebx], cl
        fld    xword ptr [ebx]
        db  0F6h
loc_1_C0A9:
        shl    edx, 8
loc_1_C0AC:
        or     edx, esi
        sub    esi, esi
loc_1_C0B0:
        call   sub_1_C089
        mov    bl, byte ptr [eax]
        sub    bl, 0x30
        db  080h, 0FBh
loc_1_C0BC:
        or     dword ptr [edi + 0x48], esi
loc_1_C0BF:
        imul   esi, esi, 0xa
        add    esi, ebx
loc_1_C0C4:
        inc    eax
        call   sub_1_C089
        cmp    byte ptr [eax], 0x2e
        je     loc_1_C0E5
        cmp    byte ptr [eax], 0
        je     0xc0fb
loc_1_C0D4:
        cmp    byte ptr [eax], 0x2c
        je     0xc0fb
        mov    bl, byte ptr [eax]
        sub    bl, 0x30
        cmp    bl, 9
        ja     loc_1_C107
        jmp    loc_1_C0BF
loc_1_C0E5:
        inc    eax
        inc    ecx
        cmp    ecx, 4
        jb     loc_1_C0A9
        call   sub_1_C089
        cmp    byte ptr [eax], 0
loc_1_C0F4:
        je     0xc0fb
        cmp    byte ptr [eax], 0x2c
        jne    loc_1_C107
        db  083h
loc_1_C0FC:
        stc    
        add    esi, dword ptr [ebp + 7]
        shl    edx, 8
        or     edx, esi
        jmp    loc_1_C10A
loc_1_C107:
        sub    edx, edx
        stc    
loc_1_C10A:
        pop    esi
        pop    ecx
        pop    ebx
        ret    
sub_1_C10E:
        push   ebp
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0x2c
        mov    eax, 0x21
        db  0CDh
loc_1_C138:
        and    byte ptr [eax + eax - 0x3fd4ffff], al
        movzx  ecx, byte ptr [ebp + 0x19]
        imul   ecx, ecx, 0x36ee80
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 0x18]
        imul   ecx, ecx, 0xea60
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 0x15]
        imul   ecx, ecx, 0x3e8
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 0x14]
        imul   ecx, ecx, 0xa
        add    eax, ecx
loc_1_C16C:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebx
        pop    ebp
        ret    
sub_1_C184:
        sub    ecx, ecx
        movzx  edx, byte ptr [esi]
loc_1_C189:
        imul   ecx, ecx, 0xa
loc_1_C18C:
        sub    edx, 0x30
        add    ecx, edx
        inc    esi
        movzx  edx, byte ptr [esi]
        cmp    dl, 0x30
        jb     loc_1_C19F
        cmp    dl, 0x39
        jb     loc_1_C189
loc_1_C19F:
        ret    
sub_1_C1A0:
        push   esi
        mov    esi, 0xb4c
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
loc_1_C1AC:
        jb     loc_1_C22A
        mov    esi, edx
        add    esi, 3
        sub    eax, eax
        cmp    byte ptr [esi], 0x2d
loc_1_C1B8:
        jne    loc_1_C1C2
        mov    eax, 0x2932e00
        inc    esi
loc_1_C1C0:
        jmp    loc_1_C1C8
loc_1_C1C2:
        cmp    byte ptr [esi], 0x2b
        jne    loc_1_C1C8
        inc    esi
loc_1_C1C8:
        cmp    byte ptr [esi], 0x30
        jb     loc_1_C22A
        cmp    byte ptr [esi], 0x39
loc_1_C1D0:
        ja     loc_1_C22A
        call   sub_1_C184
        cmp    ecx, 0x17
        ja     loc_1_C22A
loc_1_C1DC:
        imul   ecx, ecx, 0x36ee80
        add    eax, ecx
loc_1_C1E4:
        cmp    byte ptr [esi], 0x3a
        jne    loc_1_C228
        inc    esi
        cmp    byte ptr [esi], 0x30
        jb     loc_1_C22A
        cmp    byte ptr [esi], 0x39
        ja     loc_1_C22A
loc_1_C1F4:
        call   sub_1_C184
        cmp    ecx, 0x3b
loc_1_C1FC:
        ja     loc_1_C22A
        imul   ecx, ecx, 0xea60
loc_1_C204:
        add    eax, ecx
        cmp    byte ptr [esi], 0x3a
        jne    loc_1_C228
        inc    esi
loc_1_C20C:
        cmp    byte ptr [esi], 0x30
        jb     loc_1_C22A
        cmp    byte ptr [esi], 0x39
loc_1_C214:
        ja     loc_1_C22A
        call   sub_1_C184
        cmp    ecx, 0x3b
        ja     loc_1_C22A
loc_1_C220:
        imul   ecx, ecx, 0x3e8
        add    eax, ecx
loc_1_C228:
        pop    esi
        ret    
loc_1_C22A:
        mov    eax, 0x1b77400
        pop    esi
loc_1_C230:
        ret    
        db  000h, 000h, 000h
loc_1_C234:
        push   esi
        push   edi
        mov    dword ptr [0xc1d0], 0
loc_1_C240:
        call   sub_1_C1A0
        mov    esi, eax
        cmp    esi, -1
        jne    loc_1_C25C
        mov    dword ptr [0xc094], 0x80000000
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_C25C:
        call   sub_1_C10E
        cmp    eax, 0x5265c00
        jbe    loc_1_C278
        mov    dword ptr [0xc094], 0x80
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_C278:
        mov    edi, eax
        call   0x3258
        sub    edi, eax
        lea    eax, [edi + esi]
        mov    dword ptr [0xc1d0], eax
        mov    dword ptr [0xc094], 0
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh, 056h, 08Bh, 035h, 090h, 0C0h, 000h, 000h, 00Bh, 0F6h, 074h, 010h, 090h, 056h, 0E8h
        db  0DEh, 06Fh, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 036h, 00Bh, 0F6h, 075h, 0F1h, 05Eh, 0C3h, 08Dh
        db  049h, 000h
sub_1_C2B8:
        sub    esp, 4
        push   esi
        call   0xcb28
        or     eax, eax
        jne    loc_1_C2CC
        sub    eax, eax
        pop    esi
loc_1_C2C8:
        add    esp, 4
        ret    
loc_1_C2CC:
        call   0x3294
        call   sub_1_26F0
        mov    esi, eax
        or     esi, esi
        jne    loc_1_C2E4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_1_C2E4:
        push   esi
        mov    eax, dword ptr [esi + 0x2c]
        inc    eax
        mov    dword ptr [0xbc00], eax
        db  0E8h, 009h
loc_1_C2F0:
        push   es
        add    byte ptr [eax], al
        db  083h
loc_1_C2F4:
        les    eax, ptr [ebx]
        nop    
loc_1_C2F8:
        rol    byte ptr [eax], 0
        db  00Bh
loc_1_C2FC:
        sal    byte ptr [ebp + 9], 0x2b
loc_1_C300:
        rcr    byte ptr [esi - 0x7d], 0xc4
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1570  R-X
loc_2_0000:
        inc    byte ptr [0xbbfb]             ; -> loc_1_BBFB
        jne    loc_2_0024
        mov    edi, 0xbf70                   ; -> loc_1_BF70
        mov    ecx, 0xc300                   ; -> loc_1_C300
        sub    ecx, edi
        shr    ecx, 2
        sub    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        call   sub_2_02B8
        cmp    eax, 1
loc_2_0024:
        ret    
        db  080h, 03Fh, 000h, 074h, 03Ah, 080h, 03Fh, 020h, 074h, 005h, 080h, 03Fh, 009h, 075h, 003h, 047h
        db  0EBh, 0EEh, 080h, 03Fh, 02Ch, 074h, 02Bh, 08Bh, 0F7h, 047h, 080h, 03Fh, 000h, 074h, 01Eh, 080h
        db  03Fh, 02Ch, 075h, 006h, 0C6h, 007h, 000h, 047h, 0EBh, 013h, 080h, 03Fh, 020h, 074h, 008h, 080h
        db  03Fh, 009h, 074h, 003h, 047h, 0EBh, 0E3h, 0C6h, 007h, 000h, 047h, 0EBh, 0DDh, 0F8h, 0C3h, 02Bh
        db  0F6h, 0C3h, 0F9h, 0C3h
loc_2_0069:
        push   eax
        VMMCall  VMM.Convert_Boolean_String      ; cd20b2000100
        mov    edx, eax
        pop    eax
        ret    
loc_2_0074:
        push   eax
        push   edx
        VMMCall  VMM.Convert_Decimal_String      ; cd20ac000100
        cmp    edx, dword ptr [esp + 4]
        je     loc_2_0085
        mov    edx, eax
        stc    
loc_2_0085:
        pop    eax
        pop    eax
        cmc    
        ret    
sub_2_0089:
        cmp    byte ptr [eax], 0
        je     loc_2_009B
        cmp    byte ptr [eax], 0x20
        je     loc_2_0098
        cmp    byte ptr [eax], 9
        jne    loc_2_009B
loc_2_0098:
        inc    eax
        jmp    sub_2_0089
loc_2_009B:
        ret    
loc_2_009C:
        push   ebx
        push   ecx
        push   esi
        mov    eax, edx
        sub    ecx, ecx
        sub    edx, edx
        sub    ebx, ebx
        sub    esi, esi
loc_2_00A9:
        shl    edx, 8
        or     edx, esi
        sub    esi, esi
        call   sub_2_0089
        mov    bl, byte ptr [eax]
        sub    bl, 0x30
        cmp    bl, 9
        ja     loc_2_0107
loc_2_00BF:
        imul   esi, esi, 0xa
        add    esi, ebx
        inc    eax
        call   sub_2_0089
        cmp    byte ptr [eax], 0x2e
        je     loc_2_00E5
        cmp    byte ptr [eax], 0
        je     loc_2_00FB
        cmp    byte ptr [eax], 0x2c
        je     loc_2_00FB
        mov    bl, byte ptr [eax]
        sub    bl, 0x30
        cmp    bl, 9
        ja     loc_2_0107
        jmp    loc_2_00BF
loc_2_00E5:
        inc    eax
        inc    ecx
        cmp    ecx, 4
        jb     loc_2_00A9
        call   sub_2_0089
        cmp    byte ptr [eax], 0
        je     loc_2_00FB
        cmp    byte ptr [eax], 0x2c
        jne    loc_2_0107
loc_2_00FB:
        cmp    ecx, 3
        jne    loc_2_0107
        shl    edx, 8
        or     edx, esi
        jmp    loc_2_010A
loc_2_0107:
        sub    edx, edx
        stc    
loc_2_010A:
        pop    esi
        pop    ecx
        pop    ebx
        ret    
sub_2_010E:
        push   ebp
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0x2c
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        sub    eax, eax
        movzx  ecx, byte ptr [ebp + 0x19]
        imul   ecx, ecx, 0x36ee80
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 0x18]
        imul   ecx, ecx, 0xea60
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 0x15]
        imul   ecx, ecx, 0x3e8
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 0x14]
        imul   ecx, ecx, 0xa
        add    eax, ecx
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebx
        pop    ebp
        ret    
sub_2_0184:
        sub    ecx, ecx
        movzx  edx, byte ptr [esi]
loc_2_0189:
        imul   ecx, ecx, 0xa
        sub    edx, 0x30
        add    ecx, edx
        inc    esi
        movzx  edx, byte ptr [esi]
        cmp    dl, 0x30
        jb     loc_2_019F
        cmp    dl, 0x39
        jb     loc_2_0189
loc_2_019F:
        ret    
sub_2_01A0:
        push   esi
        mov    esi, 0xb4c                    ; -> loc_2_0B4C
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        jb     loc_2_022A
        mov    esi, edx
        add    esi, 3
        sub    eax, eax
        cmp    byte ptr [esi], 0x2d
        jne    loc_2_01C2
        mov    eax, 0x2932e00
        inc    esi
        jmp    loc_2_01C8
loc_2_01C2:
        cmp    byte ptr [esi], 0x2b
        jne    loc_2_01C8
        inc    esi
loc_2_01C8:
        cmp    byte ptr [esi], 0x30
        jb     loc_2_022A
        cmp    byte ptr [esi], 0x39
        ja     loc_2_022A
        call   sub_2_0184
        cmp    ecx, 0x17
        ja     loc_2_022A
        imul   ecx, ecx, 0x36ee80
        add    eax, ecx
        cmp    byte ptr [esi], 0x3a
        jne    loc_2_0228
        inc    esi
        cmp    byte ptr [esi], 0x30
        jb     loc_2_022A
        cmp    byte ptr [esi], 0x39
        ja     loc_2_022A
        call   sub_2_0184
        cmp    ecx, 0x3b
        ja     loc_2_022A
        imul   ecx, ecx, 0xea60
        add    eax, ecx
        cmp    byte ptr [esi], 0x3a
        jne    loc_2_0228
        inc    esi
        cmp    byte ptr [esi], 0x30
        jb     loc_2_022A
        cmp    byte ptr [esi], 0x39
        ja     loc_2_022A
        call   sub_2_0184
        cmp    ecx, 0x3b
        ja     loc_2_022A
        imul   ecx, ecx, 0x3e8
        add    eax, ecx
loc_2_0228:
        pop    esi
        ret    
loc_2_022A:
        mov    eax, 0x1b77400
        pop    esi
        ret    
        db  000h, 000h, 000h
sub_2_0234:
        push   esi
        push   edi
        mov    dword ptr [0xc1d0], 0         ; -> loc_1_C1D0
        call   sub_2_01A0
        mov    esi, eax
        cmp    esi, -1
        jne    loc_2_025C
        mov    dword ptr [0xc094], 0x80000000 ; -> obj1:0xC094
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_2_025C:
        call   sub_2_010E
        cmp    eax, 0x5265c00
        jbe    loc_2_0278
        mov    dword ptr [0xc094], 0x80      ; -> obj1:0xC094
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_2_0278:
        mov    edi, eax
        call   sub_1_B92C
        sub    edi, eax
        lea    eax, [edi + esi]
        mov    dword ptr [0xc1d0], eax       ; -> loc_1_C1D0
        mov    dword ptr [0xc094], 0         ; -> obj1:0xC094
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_2_0298:
        push   esi
        mov    esi, dword ptr [0xc090]       ; -> obj1:0xC090
        or     esi, esi
        je     loc_2_02B3
        nop    
loc_2_02A4:
        push   esi
        call   sub_1_B944
        add    esp, 4
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_2_02A4
loc_2_02B3:
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_2_02B8:
        sub    esp, 4
        push   esi
        call   sub_2_0B28
        or     eax, eax
        jne    loc_2_02CC
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
loc_2_02CC:
        call   sub_1_B94A
        call   sub_1_B378
        mov    esi, eax
        or     esi, esi
        jne    loc_2_02E4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_2_02E4:
        push   esi
        mov    eax, dword ptr [esi + 0x2c]
        inc    eax
        mov    dword ptr [0xbc00], eax       ; -> obj1:0xBC00
        call   sub_2_08FC
        add    esp, 4
        mov    dword ptr [0xc090], eax       ; -> obj1:0xC090
        or     eax, eax
        jne    loc_2_0308
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_2_0308:
        call   sub_2_04C4
        or     eax, eax
        jne    loc_2_0328
        push   dword ptr [0xc090]            ; -> obj1:0xC090
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_2_0328:
        call   sub_1_B94A
        push   esi
        call   sub_2_0500
        add    esp, 4
        or     eax, eax
        jne    loc_2_0350
        push   dword ptr [0xc090]            ; -> obj1:0xC090
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_2_0350:
        mov    byte ptr [0xbfec], 0x3c       ; -> loc_1_BFEC
        mov    dword ptr [0xbfe8], 0xbffc    ; -> loc_1_BFE8
        cmp    dword ptr [esi], 0
        mov    dword ptr [0xc13c], 0x54      ; -> obj1:0xC13C
        je     loc_2_037A
        mov    dword ptr [0xc13c], 0x68      ; -> obj1:0xC13C
loc_2_037A:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [0xc15c], eax       ; -> obj1:0xC15C
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [0xc060], eax       ; -> loc_1_C060
        mov    edx, dword ptr [esi + 0x20]
        mov    dword ptr [0xc2f8], edx       ; -> loc_1_C2F8
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, 0xff
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 0xff
        mov    dword ptr [0xc158], eax       ; -> obj1:0xC158
        mov    eax, dword ptr [esi + 0x28]
        and    eax, 0xfc
        mov    dword ptr [0xc234], eax       ; -> loc_1_C234
        cmp    edx, 2
        jbe    loc_2_03C5
        mov    dword ptr [0xc2f8], 0         ; -> loc_1_C2F8
loc_2_03C5:
        call   sub_2_0234
        cmp    dword ptr [0xc154], 0         ; -> obj1:0xC154
        je     loc_2_04AC
        mov    dword ptr [0xc058], 1         ; -> obj1:0xC058
        cmp    dword ptr [esi], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        mov    dword ptr [0xc1d8], eax       ; -> obj1:0xC1D8
        mov    eax, dword ptr [0xc158]       ; -> obj1:0xC158
        mov    dword ptr [0xc1dc], eax       ; -> loc_1_C1DC
        mov    dword ptr [0xc20c], 0x3c      ; -> loc_1_C20C
        call   sub_1_59A4
        or     eax, eax
        jne    loc_2_0428
        call   sub_1_0328
        call   sub_2_0298
        push   esi
        call   sub_1_B900
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_2_0428:
        push   eax
        call   sub_1_58C8
        add    esp, 4
        push   0x32
        push   0xc2f4                        ; -> loc_1_C2F4
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_1_BA26
        call   sub_1_B94A
        push   5
        call   sub_2_0890
        add    esp, 4
        call   sub_2_0AD0
        or     eax, eax
        jne    loc_2_0464
        mov    dword ptr [0xc2f8], 1         ; -> loc_1_C2F8
loc_2_0464:
        call   sub_1_B94A
        push   esi
        call   sub_2_05A0
        add    esp, 4
        push   esi
        call   sub_1_B900
        add    esp, 4
        call   sub_1_B94A
        call   sub_1_B94A
        mov    esi, dword ptr [0xc090]       ; -> obj1:0xC090
        or     esi, esi
        je     loc_2_049F
        nop    
loc_2_0490:
        push   esi
        call   sub_1_AA74
        add    esp, 4
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_2_0490
loc_2_049F:
        pop    esi
        mov    eax, 1
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_2_04AC:
        call   sub_2_0298
        push   esi
        call   sub_1_B900
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_2_04C4:
        sub    esp, 4
        mov    dword ptr [0xbcd2], 0xbc60    ; -> obj1:0xBCD2
        push   0x42
        push   0xbc94                        ; -> loc_1_BC94
        push   0xbfe4                        ; -> loc_1_BFE4
        lea    eax, [esp + 0xc]
        push   eax
        call   0xffff7308                    ; -> obj1:0xB984
        cmp    dword ptr [esp], 0
        jne    loc_2_04F8
        mov    eax, 1
        add    esp, 4
        ret    
        db  090h
loc_2_04F8:
        sub    eax, eax
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_2_0500:
        push   edi
        call   sub_1_B94A
        mov    dword ptr [0xc1cc], 0         ; -> obj1:0xC1CC
        mov    dword ptr [0xc2c8], 0         ; -> loc_1_C2C8
        mov    edi, 0xc248                   ; -> obj1:0xC248
        sub    eax, eax
        mov    ecx, 0x20
        rep stosd dword ptr es:[edi], eax
        push   eax
        push   2
        push   1
        push   0x100
        mov    eax, dword ptr [0xc050]       ; -> obj1:0xC050
        push   dword ptr [eax + 0xc]
        push   -1
        push   0xff
        push   0x7f
        call   sub_1_7F60
        add    esp, 0x20
        mov    byte ptr [0xc150], 0          ; -> obj1:0xC150
        mov    eax, dword ptr [esp + 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [0xbfa4], al         ; -> loc_1_BFA4
        mov    byte ptr [0xc1d4], al         ; -> obj1:0xC1D4
        mov    dword ptr [0xc130], 0         ; -> obj1:0xC130
        mov    dword ptr [0xc138], 0         ; -> loc_1_C138
        mov    eax, dword ptr [0xbf6c]       ; -> loc_1_BF6C
        mov    dword ptr [0xc134], eax       ; -> obj1:0xC134
        push   0
        push   0x81dc                        ; -> loc_1_81DC
        push   0xea60
        push   0xc124                        ; -> obj1:0xC124
        call   sub_1_B920
        add    esp, 0x10
        mov    eax, 1
        pop    edi
        ret    
sub_2_05A0:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        sub    ebx, ebx
        mov    dword ptr [0xc148], ebx       ; -> obj1:0xC148
        cmp    byte ptr [0xc1d4], bl         ; -> obj1:0xC1D4
        je     loc_2_0880
        call   sub_1_B94A
        push   0x14
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [0xbcfc], eax       ; -> obj1:0xBCFC
        cmp    eax, ebx
        je     loc_2_06ED
        mov    dword ptr [eax], 0x8560       ; -> sub_1_8560
        mov    edi, dword ptr [esp + 0x38]
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [0xbff8], eax       ; -> loc_1_BFF8
        mov    edx, dword ptr [edi + 0x14]
        mov    dword ptr [0xc1c0], edx       ; -> loc_1_C1C0
        mov    ecx, dword ptr [edi + 8]
        sub    ecx, eax
        sbb    ebp, ebp
        and    ecx, ebp
        lea    ebp, [ecx + eax]
        mov    eax, dword ptr [edi + 0xc]
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    dword ptr [esp + 0x24], eax
        mov    ecx, dword ptr [0xc090]       ; -> obj1:0xC090
        cmp    ecx, ebx
        je     loc_2_063F
loc_2_0619:
        mov    eax, dword ptr [ecx + 0xc]
        lea    edx, [eax + 0x54]
        mov    dword ptr [edx], edx
        mov    dword ptr [eax + 0x58], edx
        mov    dword ptr [eax + 0x64], 0
        mov    dword ptr [eax + 0x5c], 0
        mov    dword ptr [eax + 0x60], 0
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_2_0619
loc_2_063F:
        push   0x14
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [0xc148], eax       ; -> obj1:0xC148
        or     eax, eax
        je     loc_2_06ED
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [0xc148]       ; -> obj1:0xC148
        mov    dword ptr [eax + 4], eax
        mov    eax, dword ptr [0xc148]       ; -> obj1:0xC148
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [0xc148]       ; -> obj1:0xC148
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [0xc148]       ; -> obj1:0xC148
        mov    dword ptr [eax + 0x10], 0
        call   sub_1_B94A
        shr    ebp, 8
        mov    eax, ebp
        shl    eax, 8
        mov    dword ptr [esp + 0x30], eax
        mov    eax, dword ptr [esp + 0x24]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_2_06ED
        push   dword ptr [esp + 0x24]
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_2_06ED
        push   0x50
        push   dword ptr [esp + 0x28]
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_BA02
        cmp    dword ptr [esp + 0x18], 0
        je     loc_2_0754
loc_2_06E4:
        push   dword ptr [esp + 0x1c]
        call   sub_1_BA2C
loc_2_06ED:
        cmp    dword ptr [0xbcfc], 0         ; -> obj1:0xBCFC
        je     loc_2_0704
        push   dword ptr [0xbcfc]            ; -> obj1:0xBCFC
        call   sub_1_B944
        add    esp, 4
loc_2_0704:
        cmp    dword ptr [0xc148], 0         ; -> obj1:0xC148
        je     loc_2_071B
        push   dword ptr [0xc148]            ; -> obj1:0xC148
        call   sub_1_B944
        add    esp, 4
loc_2_071B:
        or     esi, esi
        je     loc_2_0728
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_2_0728:
        or     ebx, ebx
        je     loc_2_0735
        push   ebx
        call   sub_1_B944
        add    esp, 4
loc_2_0735:
        mov    byte ptr [0xc150], 0          ; -> obj1:0xC150
        mov    byte ptr [0xbfa4], 0          ; -> loc_1_BFA4
        mov    byte ptr [0xc1d4], 0          ; -> obj1:0xC1D4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
loc_2_0754:
        call   sub_1_B94A
        push   dword ptr [esp + 0x30]
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_2_077C
        push   dword ptr [esp + 0x20]
        call   sub_1_BA08
        jmp    loc_2_06E4
        db  08Dh, 049h, 000h
loc_2_077C:
        push   ebp
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 0x18], 0
        je     loc_2_07A4
        push   dword ptr [esp + 0x20]
        call   sub_1_BA08
        jmp    loc_2_06E4
        db  08Dh, 049h, 000h
loc_2_07A4:
        cmp    dword ptr [esp + 0x24], 0
        je     loc_2_082A
        inc    eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0x2c], eax
loc_2_07B4:
        push   0x14
        push   esi
        push   dword ptr [esp + 0x24]
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        call   sub_1_BA32
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_BA0E
        mov    edi, dword ptr [esp + 0x14]
        add    edi, 0x38
        sub    eax, eax
        mov    ecx, 0x14
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [esp + 0x14]
        add    ecx, 0x38
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x1c], eax
        mov    dword ptr [ecx + 0x20], esi
        mov    byte ptr [ecx + 5], 8
        mov    byte ptr [ecx + 4], 1
        mov    eax, dword ptr [0xbcfc]       ; -> obj1:0xBCFC
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 0x10], eax
        push   dword ptr [esp + 0x14]
        call   sub_1_833C
        add    esp, 4
        add    esi, 0x14
        dec    dword ptr [esp + 0x2c]
        jne    loc_2_07B4
loc_2_082A:
        or     ebp, ebp
        je     loc_2_0864
loc_2_082E:
        push   0x100
        push   ebx
        push   dword ptr [esp + 0x30]
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        call   sub_1_BA32
        mov    eax, dword ptr [0xc05c]       ; -> loc_1_C05C
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0xc05c], eax       ; -> loc_1_C05C
        add    ebx, 0x100
        dec    ebp
        jne    loc_2_082E
loc_2_0864:
        mov    dword ptr [0xc240], 3         ; -> loc_1_C240
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [0xc120], eax       ; -> obj1:0xC120
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [0xc2fc], eax       ; -> loc_1_C2FC
loc_2_0880:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Dh, 049h, 000h
sub_2_0890:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    dword ptr [0xc0ac], esi       ; -> loc_1_C0AC
        mov    dword ptr [0xc064], 0         ; -> loc_1_C064
        mov    dword ptr [0xc244], 0         ; -> obj1:0xC244
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     loc_2_08DE
loc_2_08B6:
        push   0x60
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        mov    ecx, dword ptr [0xc244]       ; -> obj1:0xC244
        je     loc_2_08DE
        mov    dword ptr [eax], ecx
        mov    dword ptr [0xc244], eax       ; -> obj1:0xC244
        inc    dword ptr [0xc064]            ; -> loc_1_C064
        mov    eax, esi
        dec    esi
        or     eax, eax
        jne    loc_2_08B6
loc_2_08DE:
        push   0
        push   0x9774                        ; -> loc_1_9774
        push   0x92d4                        ; -> sub_1_92D4
        push   0x987c                        ; -> loc_1_987C
        push   1
        call   sub_1_0348
        add    esp, 0x14
        pop    esi
        ret    
        db  090h
sub_2_08FC:
        sub    esp, 0x44
        push   edi
        call   sub_1_B94A
        push   0x44
        call   sub_1_B93E
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        mov    dword ptr [0xc050], edx       ; -> obj1:0xC050
        jne    loc_2_0924
        mov    eax, edx
        pop    edi
        add    esp, 0x44
        ret    
        db  08Bh, 0FFh
loc_2_0924:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x11
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [edx + 4], 0x100007f
        mov    dword ptr [edx + 8], 0xff
        mov    word ptr [edx + 0x3e], 1
        mov    word ptr [edx + 0x14], 0xd
        mov    word ptr [edx + 0x3c], 0x100
        mov    dword ptr [edx + 0xc], 0xc0b8 ; -> obj1:0xC0B8
        mov    dword ptr [0xc0bc], edx       ; -> loc_1_C0BC
        mov    dword ptr [0xc0c0], 0xa1e8    ; -> obj1:0xC0C0
        mov    dword ptr [0xc0c4], 0xa254    ; -> loc_1_C0C4
        mov    dword ptr [0xc0c8], 0xa3a4    ; -> obj1:0xC0C8
        mov    dword ptr [0xc0cc], 0xa3a8    ; -> obj1:0xC0CC
        mov    dword ptr [0xc0d8], 0xa3ac    ; -> obj1:0xC0D8
        mov    dword ptr [0xc0dc], 0xa4e0    ; -> obj1:0xC0DC
        mov    dword ptr [0xc0e0], 0xa564    ; -> obj1:0xC0E0
        mov    dword ptr [0xc0d0], 0xa55c    ; -> obj1:0xC0D0
        mov    dword ptr [0xc0d4], 0xa55c    ; -> loc_1_C0D4
        mov    dword ptr [0xc0f4], 0xffffffff ; -> loc_1_C0F4
        mov    dword ptr [0xc0fc], 0x989680  ; -> loc_1_C0FC
        mov    dword ptr [0xc0f8], 0x100     ; -> obj1:0xC0F8
        mov    dword ptr [esp + 8], 0x114
        mov    eax, dword ptr [0xc140]       ; -> obj1:0xC140
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0x1c], 0xa3a4 ; -> loc_1_A3A4
        mov    dword ptr [esp + 0x20], 0xa55c ; -> loc_1_A55C
        mov    dword ptr [esp + 0x24], 0xa55c ; -> loc_1_A55C
        mov    dword ptr [esp + 0x3c], 1
        mov    ecx, dword ptr [0xc054]       ; -> loc_1_C054
        inc    ecx
        mov    dword ptr [0xc140], ecx       ; -> obj1:0xC140
        mov    dword ptr [0xc0e8], ecx       ; -> obj1:0xC0E8
        mov    dword ptr [0xc0a4], 0         ; -> loc_1_C0A4
        mov    dword ptr [0xc09c], 0xa084    ; -> loc_1_C09C
        mov    eax, dword ptr [0xbf6c]       ; -> loc_1_BF6C
        mov    dword ptr [0xc0a8], eax       ; -> obj1:0xC0A8
        mov    dword ptr [0xc160], ecx       ; -> obj1:0xC160
        mov    dword ptr [0xc164], 0x18      ; -> obj1:0xC164
        mov    dword ptr [0xc168], 0x114     ; -> obj1:0xC168
        mov    dword ptr [0xc16c], 0x989680  ; -> loc_1_C16C
        mov    dword ptr [0xc17c], 1         ; -> obj1:0xC17C
        mov    dword ptr [0xc180], 1         ; -> obj1:0xC180
        mov    dword ptr [0xc1b8], 0x1a      ; -> loc_1_C1B8
        mov    dword ptr [0xc0b4], 0xc0b8    ; -> obj1:0xC0B4
        mov    dword ptr [0xc054], ecx       ; -> loc_1_C054
        inc    dword ptr [0xc154]            ; -> obj1:0xC154
        lea    eax, [esp + 4]
        push   eax
        push   edx
        call   sub_1_0DE0
        add    esp, 8
        or     eax, eax
        jne    loc_2_0AA8
        sub    eax, eax
        pop    edi
        add    esp, 0x44
        ret    
        db  090h
loc_2_0AA8:
        push   dword ptr [0xc050]            ; -> obj1:0xC050
        call   sub_1_0D60
        add    esp, 4
        or     eax, eax
        jne    loc_2_0AC4
        sub    eax, eax
        pop    edi
        add    esp, 0x44
        ret    
        db  08Dh, 049h, 000h
loc_2_0AC4:
        pop    edi
        mov    eax, dword ptr [0xc050]       ; -> obj1:0xC050
        add    esp, 0x44
        ret    
        db  08Bh, 0FFh
sub_2_0AD0:
        cmp    dword ptr [0xc2f8], 2         ; -> loc_1_C2F8
        je     loc_2_0AE0
        mov    eax, 1
        ret    
        db  090h
loc_2_0AE0:
        mov    dword ptr [0xc0b0], 0         ; -> loc_1_C0B0
        mov    byte ptr [0xc2f0], 0          ; -> loc_1_C2F0
        mov    dword ptr [0xbff0], 0         ; -> loc_1_BFF0
        push   0
        push   0
        push   0x92d4                        ; -> sub_1_92D4
        push   0xa638                        ; -> loc_1_A638
        push   2
        call   sub_1_0348
        add    esp, 0x14
        mov    dword ptr [0xc04c], eax       ; -> loc_1_C04C
        or     eax, eax
        je     loc_2_0B24
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
loc_2_0B24:
        sub    eax, eax
        ret    
        db  090h
sub_2_0B28:
        VxDCall  Dev_0488.Service_0000h          ; cd2000008804
        jb     loc_2_0B44
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_2_0B44
        VxDCall  Dev_0488.Service_000Ah          ; cd200a008804
        mov    dword ptr [0xbf6c], eax       ; -> loc_1_BF6C
        clc    
loc_2_0B44:
        cmc    
        sbb    eax, eax
        ret    
        db  001h, 000h, 000h, 000h
loc_2_0B4C:
        push   esp
        pop    edx
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [edx + 0x69], al
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr fs:[esi + 0x67], 0x69724400
        jbe    0xcc6
        jb     loc_2_0C63
loc_2_0C63:
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0xccc
        jb     loc_2_0CCF
        popal  
        arpl   word ptr [ebp + 0x73], sp
        add    byte ptr [ebp + 0x66], al
        popal  
        jne    loc_2_0CE0
        je     0xcbd
        popal  
        je     loc_2_0CDE
        ja     0xcdc
        jns    loc_2_0C7D
loc_2_0C7D:
        inc    ebp
        je     loc_2_0CE8
        jb     loc_2_0CF1
        je     loc_2_0CD9
        dec    esi
        inc    ecx
        push   eax
        add    byte ptr [ecx + 0x52], al
        push   eax
        inc    ecx
        insb   byte ptr es:[edi], dx
        ja     0xcf2
        jns    loc_2_0D06
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    loc_2_0D09
        arpl   word ptr [ebp + 0x52], sp
        outsd  dx, dword ptr [esi]
        jne    loc_2_0D11
        add    byte ptr gs:[ebp + 0x6e], al
        popal  
        bound  ebp, qword ptr [ebp + 0x52]
        outsd  dx, dword ptr [esi]
        jne    loc_2_0D1D
        imul   ebp, dword ptr [esi + 0x67], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], ch            ; -> loc_2_0069
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0CCF:
        inc    esi
        outsd  dx, dword ptr [esi]
        jb     0xd4a
        popal  
        jb     0xd3a
        inc    edx
        jb     0xd48
loc_2_0CD9:
        popal  
        arpl   word ptr fs:[ecx + 0x73], sp
loc_2_0CDE:
        je     0xd53
loc_2_0CE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0CE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], ch            ; -> loc_2_0069
loc_2_0CF1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        popal  
        inc    edi
        push   edi
        inc    esp
loc_2_0D06:
        je     loc_2_0D6E
loc_2_0D09:
        arpl   word ptr [eax + eax], si
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0D11:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0D1D:
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0x10000
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        dec    ebp
        push   esp
        push   ebp
        inc    esp
        imul   esi, dword ptr [ebx + 0x63], 0x7265766f
        jns    loc_2_0D3D
loc_2_0D3D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0x10000
        add    byte ptr [eax], al
        sbb    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        inc    edi
        dec    ebp
        push   eax
        dec    esp
        jbe    0xdcc
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0D6E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dh      ; -> loc_2_0074
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x66], al
        popal  
        jne    loc_2_0E01
        je     loc_2_0DEB
        push   esp
        dec    esp
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
        je     loc_2_0DB1
loc_2_0DB1:
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        popaw  
        jne    loc_2_0E31
        je     loc_2_0E1B
        dec    edi
        push   ebx
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
        je     loc_2_0DE1
loc_2_0DE1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0DEB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   edx
        outsd  dx, dword ptr [esi]
        jne    0xe67
        imul   ebp, dword ptr [esi + 0x67], 0x53667542
        imul   edi, dword ptr [edx + 0x65], 0
loc_2_0E01:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        je     loc_2_0E11
loc_2_0E11:
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    dword ptr [eax], eax
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0E1B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   edx
        outsd  dx, dword ptr [esi]
        jne    loc_2_0E97
        imul   ebp, dword ptr [esi + 0x67], 0x6b636150
        je     loc_2_0EA0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0E31:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        je     loc_2_0E41
loc_2_0E41:
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        push   eax
        inc    ecx
        jb     loc_2_0EBB
        jae    loc_2_0ECB
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
        add    byte ptr [eax + eax], bl      ; -> loc_2_009C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        dec    ecx
        push   eax
        dec    ebp
        popal  
        jae    0xef0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0E97:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pushfd 
loc_2_0EA0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        pop    edx
        jb     loc_2_0F22
        inc    edx
        jb     loc_2_0F25
        popal  
        arpl   word ptr fs:[ecx + 0x73], sp
loc_2_0EBB:
        je     loc_2_0EBD
loc_2_0EBD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0ECB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
        sbb    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        popal  
        js     loc_2_0F30
        push   esp
        push   ebp
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
        je     loc_2_0F01
loc_2_0F01:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 000h, 000h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0F22:
        add    byte ptr [eax], al
        db  000h
loc_2_0F25:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_2_0F30:
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
        add    byte ptr [eax + 0x4c], cl
        push   ebx
        and    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x9d  R-X
loc_3_0000:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_3_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  066h, 000h
loc_3_0010:
        xchg   dword ptr [eax], eax
        mov    dword ptr [eax], eax
loc_3_0014:
        push   esp
        push   0x79732065
        jae    loc_3_0090
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x61], ch
        jae    loc_3_0043
        je     0x8c
        arpl   word ptr [ebp + 0x64], si
        and    byte ptr [ecx + 0x20], ah
        arpl   word ptr [edi + 0x6e], bp
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x74], 0x726f6620
        and    byte ptr [ecx + 0x50], cl
        and    byte ptr [ecx + 0x64], ah
        jb     0xa8
loc_3_0043:
        jae    0xb8
        and    byte ptr [0x69772073], ah
        je     0xb5
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x79], dh
        jae    0xca
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x61], ch
        jbe    0xc6
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + si + 0x61], ch
        jb     0xc8
        ja     0xc7
        jb     0xcd
        and    byte ptr [ecx + 0x64], ah
        jb     0xd3
        jae    0xe3
        and    byte ptr [0x202e7332], ah
        and    eax, 0x54007333
        push   0x6e692065
        je     0xe7
        jb     0xea
        popal  
        arpl   word ptr [ebp + 0x20], sp
        push   0x62207361
        outsb  dx, byte ptr gs:[esi]
loc_3_0090:
        and    byte ptr [ecx + ebp*2 + 0x73], ah
        popal  
        bound  ebp, qword ptr [ebp + 0x64]
        add    byte ptr cs:[eax], ah
        db  018h
VxD__LTEXT ENDS

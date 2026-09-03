; MSTCP -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 12978 instructions recovered by recursive descent

Declare_Virtual_Device MSTCP, 3, 0, MSTCP_Control, 0048Ah, Undefined_Init_Order
;   DDK version 030Ah   init_order 05400A000h   flags 00000h
;   service table: obj1:0x9294, 1 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x9764  R-X
sub_1_0000:
        VxDCall  Dev_0489.Service_8002h          ; cd2002808904
sub_1_0006:
        VxDCall  Dev_0488.Service_8007h          ; cd2007808804
sub_1_000C:
        VxDCall  Dev_0489.Service_8006h          ; cd2006808904
MSTCP_Service_00:
        mov    ax, 0x100
        clc    
        ret    
MSTCP_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
MSTCP_V86_API:
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        cmp    word ptr [eax], 0
        jne    loc_1_0044
        call   sub_1_009C
        jmp    loc_1_0055
loc_1_0044:
        cmp    word ptr [eax], 1
        jne    loc_1_0051
        call   sub_1_0110
        jmp    loc_1_0055
loc_1_0051:
        mov    ax, 0xffff
loc_1_0055:
        pop    edx
        mov    word ptr [ebp + 0x1c], ax
        ret    
sub_1_005B:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        jecxz  loc_1_007E
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        test   byte ptr [ebx], 0x20
        jne    loc_1_007F
        movzx  edx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, edx
        add    eax, dword ptr [ebx + 4]
        pop    ebx
loc_1_007E:
        ret    
loc_1_007F:
        shr    eax, 0x10
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    ebx
        mov    edx, dword ptr [esp + 4]
        movzx  edx, dx
        add    eax, edx
        ret    
        db  000h, 000h
sub_1_009C:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        lea    edi, [esp + 0xc]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    esi, dword ptr [esp + 0x34]
        push   dword ptr [esi + 2]
        call   sub_1_005B
        add    esp, 4
        mov    edi, eax
        push   dword ptr [esi + 0xa]
        call   sub_1_005B
        add    esp, 4
        mov    ebx, eax
        push   dword ptr [esi + 6]
        call   sub_1_005B
        add    esp, 4
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x1c], 0
        push   dword ptr [esi + 0xe]
        call   sub_1_005B
        add    esp, 4
        push   eax
        push   ebx
        lea    eax, [esp + 0x24]
        push   eax
        push   edi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_74F0
        add    esp, 0x14
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Bh, 0FFh
sub_1_0110:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        lea    edi, [esp + 0xc]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    esi, dword ptr [esp + 0x20]
        push   dword ptr [esi + 2]
        call   sub_1_005B
        add    esp, 4
        mov    edi, eax
        push   dword ptr [esi + 0xa]
        call   sub_1_005B
        add    esp, 4
        mov    ebx, eax
        push   dword ptr [esi + 6]
        call   sub_1_005B
        add    esp, 4
        push   dword ptr [ebx]
        push   eax
        push   edi
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_7878
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_0164:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_01A8
        mov    eax, dword ptr [0x9648]       ; -> loc_1_9648
        mov    dword ptr [eax], 0x400
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0x401
        mov    dword ptr [eax + 0xc], 0
        mov    dword ptr [0x973c], 2         ; -> loc_1_973C
        push   0x973c                        ; -> loc_1_973C
        push   eax
        call   sub_1_95D0
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_01A8:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_01D1
        push   0
        push   0
        lea    eax, [esp + 0xc]
        push   eax
        push   0x5a94                        ; -> loc_1_5A94
        call   sub_1_5AB0
        add    esp, 0x10
        push   dword ptr [esp + 4]
        call   sub_1_05CC
        add    esp, 4
loc_1_01D1:
        ret    
        db  08Bh, 0FFh
sub_1_01D4:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, edx
        and    ecx, 0xffff
        cmp    dword ptr [0x95f8], ecx       ; -> loc_1_95F8
        jbe    loc_1_01FF
        mov    eax, dword ptr [0x9620]       ; -> loc_1_9620
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     loc_1_01FF
        movzx  ecx, word ptr [eax + 0x3a]
        shr    edx, 0x10
        cmp    ecx, edx
        je     loc_1_0201
loc_1_01FF:
        sub    eax, eax
loc_1_0201:
        ret    
        db  08Bh, 0FFh
sub_1_0204:
        push   ebx
        push   esi
        push   edi
loc_1_0207:
        mov    edx, dword ptr [0x9620]       ; -> loc_1_9620
        mov    ebx, dword ptr [0x95f8]       ; -> loc_1_95F8
        mov    ecx, dword ptr [0x9684]       ; -> loc_1_9684
        sub    eax, eax
        cmp    ebx, eax
        je     loc_1_0234
        nop    
loc_1_0220:
        cmp    ebx, ecx
        jne    loc_1_0226
        sub    ecx, ecx
loc_1_0226:
        cmp    dword ptr [edx + ecx*4], 0
        je     loc_1_0232
        inc    ecx
        inc    eax
        cmp    ebx, eax
        ja     loc_1_0220
loc_1_0232:
        cmp    ebx, eax
loc_1_0234:
        ja     loc_1_0298
        cmp    ebx, 0x100
        je     loc_1_02DC
        sub    ebx, 0xf0
        sbb    eax, eax
        and    ebx, eax
        add    ebx, 0x100
        lea    eax, [ebx*4]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        or     eax, eax
        mov    edx, dword ptr [0x9620]       ; -> loc_1_9620
        mov    ecx, dword ptr [0x95f8]       ; -> loc_1_95F8
        je     loc_1_02D0
        shl    ecx, 2
        mov    edi, eax
        mov    esi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x9620], eax       ; -> loc_1_9620
        mov    dword ptr [0x95f8], ebx       ; -> loc_1_95F8
        push   edx
        call   sub_1_90FC
        add    esp, 4
        jmp    loc_1_0207
        db  090h
loc_1_0298:
        mov    si, word ptr [0x9658]         ; -> loc_1_9658
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x3a], si
        mov    byte ptr [eax + 0x35], cl
        mov    dword ptr [edx + ecx*4], eax
        lea    eax, [ecx + 1]
        mov    dword ptr [0x9684], eax       ; -> loc_1_9684
        movzx  eax, si
        shl    eax, 0x10
        and    ecx, 0xffff
        or     eax, ecx
        inc    si
        mov    word ptr [0x9658], si         ; -> loc_1_9658
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_02D0:
        pop    edi
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_02DC:
        pop    edi
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_02E8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [ecx + 0x2c]
        mov    edx, dword ptr [esp + 0xc]
        mov    dword ptr [edx], eax
        movzx  eax, word ptr [ecx + 0x30]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_1_0318
        cmp    byte ptr [eax + 0x34], 0x11
        jne    loc_1_0318
        mov    dword ptr [esi + 4], eax
        mov    eax, 1
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_0318:
        mov    ecx, dword ptr [esi]
loc_1_031A:
        or     eax, eax
        je     loc_1_032E
loc_1_031E:
        cmp    byte ptr [eax + 0x34], 0x11
        je     loc_1_032A
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_031E
loc_1_032A:
        or     eax, eax
        jne    loc_1_0356
loc_1_032E:
        lea    edx, [ecx*4 + 0x96f8]         ; -> loc_1_96F8
loc_1_0335:
        inc    ecx
        add    edx, 4
        cmp    edx, 0x9738                   ; -> loc_1_9738
        jae    loc_1_034D
        cmp    dword ptr [edx], 0
        je     loc_1_0335
        mov    eax, dword ptr [ecx*4 + 0x96f8] ; -> loc_1_96F8
loc_1_034D:
        cmp    ecx, 0x10
        jne    loc_1_031A
        or     eax, eax
        je     loc_1_0364
loc_1_0356:
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], eax
        mov    eax, 1
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_0364:
        sub    eax, eax
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_0378:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    loc_1_03D4
        or     eax, eax
        jne    loc_1_03D4
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 1
        inc    eax
        lea    eax, [esi*4 + 0x96f8]         ; -> loc_1_96F8
loc_1_039C:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_03B2
loc_1_03A2:
        cmp    byte ptr [ecx + 0x34], 0x11
        je     loc_1_03AE
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_03A2
loc_1_03AE:
        or     ecx, ecx
        jne    loc_1_03C1
loc_1_03B2:
        inc    esi
        add    eax, 4
        cmp    eax, 0x9738                   ; -> loc_1_9738
        jb     loc_1_039C
        or     ecx, ecx
        je     loc_1_03D0
loc_1_03C1:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_03D0:
        sub    eax, eax
        pop    esi
        ret    
loc_1_03D4:
        cmp    esi, 0x10
        jae    loc_1_03EE
        mov    ecx, dword ptr [esi*4 + 0x96f8] ; -> loc_1_96F8
        or     ecx, ecx
        je     loc_1_03EE
loc_1_03E4:
        cmp    ecx, eax
        je     loc_1_03FC
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_03E4
loc_1_03EE:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        ret    
loc_1_03FC:
        cmp    byte ptr [ecx + 0x34], 0x11
        jne    loc_1_03EE
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 1
        mov    eax, 1
        pop    esi
        ret    
        db  090h
sub_1_0414:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    si, word ptr [ebp + 0xc]
        movzx  ebx, si
        mov    dl, byte ptr [ebp + 0x10]
        inc    edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_0428:
        mov    eax, dword ptr [ebp + 8]
        add    eax, ebx
        and    eax, 0xf
        mov    eax, dword ptr [eax*4 + 0x96f8] ; -> loc_1_96F8
        or     eax, eax
        je     loc_1_0459
        nop    
loc_1_043C:
        cmp    eax, edi
        jbe    loc_1_0453
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2c], ecx
        jne    loc_1_0453
        cmp    word ptr [eax + 0x30], si
        jne    loc_1_0453
        cmp    byte ptr [eax + 0x34], dl
        je     loc_1_046E
loc_1_0453:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_043C
loc_1_0459:
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_046C
        mov    dword ptr [ebp + 8], 0
        sub    edi, edi
        jmp    loc_1_0428
        db  08Bh, 0FFh
loc_1_046C:
        sub    eax, eax
loc_1_046E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  090h
sub_1_0474:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi]
        mov    al, byte ptr [esi + 0xa]
        push   eax
        mov    ax, word ptr [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   sub_1_0414
        add    esp, 0x10
        or     eax, eax
        je     loc_1_049B
        mov    dword ptr [esi], eax
        mov    ecx, dword ptr [eax + 0x2c]
        mov    dword ptr [esi + 4], ecx
loc_1_049B:
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_04A0:
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ecx + 4], eax
        mov    ax, word ptr [esp + 8]
        mov    word ptr [ecx + 8], ax
        mov    al, byte ptr [esp + 0xc]
        mov    byte ptr [ecx + 0xa], al
        push   ecx
        call   sub_1_0474
        add    esp, 4
        ret    
        db  090h
sub_1_04CC:
        mov    edx, dword ptr [esp + 4]
        movzx  eax, word ptr [edx + 0x30]
        add    eax, dword ptr [edx + 0x2c]
        and    eax, 0xf
        lea    eax, [eax*4 + 0x96f8]         ; -> loc_1_96F8
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_04F4
loc_1_04E8:
        cmp    ecx, edx
        ja     loc_1_04F4
        mov    eax, ecx
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_04E8
loc_1_04F4:
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax], edx
        cmp    byte ptr [edx + 0x34], 0x11
        jne    loc_1_0504
        inc    dword ptr [0x958c]            ; -> obj1:0x958C
loc_1_0504:
        ret    
        db  08Dh, 049h, 000h
sub_1_0508:
        mov    eax, dword ptr [esp + 4]
        movzx  ecx, word ptr [eax + 0x30]
        add    ecx, dword ptr [eax + 0x2c]
        and    ecx, 0xf
        lea    ecx, [ecx*4 + 0x96f8]         ; -> loc_1_96F8
        mov    edx, dword ptr [ecx]
        or     edx, edx
        je     loc_1_0544
loc_1_0524:
        cmp    edx, eax
        je     loc_1_0534
        mov    ecx, edx
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_1_0524
        ret    
        db  08Dh, 049h, 000h
loc_1_0534:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        cmp    byte ptr [edx + 0x34], 0x11
        jne    loc_1_0544
        dec    dword ptr [0x958c]            ; -> obj1:0x958C
loc_1_0544:
        ret    
        db  08Dh, 049h, 000h
sub_1_0548:
        push   ebx
        inc    ecx
        mov    ecx, 0x96f8                   ; -> loc_1_96F8
        mov    bl, byte ptr [esp + 0xc]
        mov    dx, word ptr [esp + 8]
loc_1_0558:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_1_056F
loc_1_055E:
        cmp    word ptr [eax + 0x30], dx
        jne    loc_1_0569
        cmp    byte ptr [eax + 0x34], bl
        je     loc_1_057C
loc_1_0569:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_055E
loc_1_056F:
        add    ecx, 4
        cmp    ecx, 0x9738                   ; -> loc_1_9738
        jb     loc_1_0558
        sub    eax, eax
loc_1_057C:
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_0580:
        push   esi
        mov    eax, dword ptr [esp + 8]
        lea    esi, [eax + 4]
        sub    edx, edx
        mov    eax, dword ptr [eax]
        cmp    eax, edx
        jle    loc_1_05A4
loc_1_0590:
        cmp    word ptr [esi + 2], 2
        je     loc_1_05A8
        movzx  ecx, word ptr [esi]
        add    ecx, 4
        add    esi, ecx
        inc    edx
        cmp    edx, eax
        jl     loc_1_0590
loc_1_05A4:
        sub    al, al
        pop    esi
        ret    
loc_1_05A8:
        cmp    word ptr [esi], 0xe
        jb     loc_1_05C8
        add    esi, 4
        mov    cx, word ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax], cx
        mov    ecx, dword ptr [esi + 2]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    al, 1
        pop    esi
        ret    
loc_1_05C8:
        sub    al, al
        pop    esi
        ret    
sub_1_05CC:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x14]
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x1c]
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x20], eax
        sub    edi, edi
        mov    dword ptr [esp + 0x10], 0x96f8 ; -> loc_1_96F8
        mov    esi, dword ptr [esp + 0x28]
loc_1_05F9:
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        je     loc_1_06EA
loc_1_0608:
        cmp    dword ptr [ebx + 0x2c], esi
        jne    loc_1_06E0
        mov    ax, word ptr [ebx + 0x32]
        test   al, 0x80
        je     loc_1_06E0
        and    ax, 0xff7f
        mov    word ptr [ebx + 0x32], ax
        lea    eax, [ebx + 0x20]
        push   eax
        call   sub_1_95C4
        add    esp, 4
        mov    ecx, dword ptr [ebx + 4]
        or     ecx, ecx
        je     loc_1_064E
        lea    eax, [ebx + 4]
loc_1_063C:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    loc_1_063C
        mov    dword ptr [eax], edi
        mov    edi, ecx
        mov    dword ptr [ebx + 4], 0
loc_1_064E:
        lea    ecx, [ebx + 8]
        mov    eax, dword ptr [ecx]
        sub    eax, ebx
        cmp    eax, 8
        je     loc_1_0683
loc_1_065A:
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [esp + 0x18], edx
        lea    eax, [esp + 0x14]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx]
        sub    eax, ebx
        cmp    eax, 8
        jne    loc_1_065A
loc_1_0683:
        lea    ecx, [ebx + 0x18]
        mov    eax, dword ptr [ecx]
        sub    eax, ebx
        cmp    eax, 0x18
        je     loc_1_06B9
loc_1_0690:
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [esp + 0x20], edx
        lea    eax, [esp + 0x1c]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx]
        sub    eax, ebx
        cmp    eax, 0x18
        jne    loc_1_0690
loc_1_06B9:
        mov    ebp, dword ptr [ebx + 0x94]
        or     ebp, ebp
        je     loc_1_06D6
        nop    
loc_1_06C4:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        push   eax
        call   sub_1_90FC
        add    esp, 4
        or     ebp, ebp
        jne    loc_1_06C4
loc_1_06D6:
        mov    dword ptr [ebx + 0x94], 0
loc_1_06E0:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_1_0608
loc_1_06EA:
        add    dword ptr [esp + 0x10], 4
        cmp    dword ptr [esp + 0x10], 0x9738 ; -> loc_1_9738
        jb     loc_1_05F9
        or     edi, edi
        je     loc_1_071F
loc_1_0701:
        mov    esi, edi
        mov    edi, dword ptr [edi]
        push   0
        push   6
        push   dword ptr [esi + 0x14]
        call   dword ptr [esi + 0x10]
        add    esp, 0xc
        push   esi
        call   sub_1_0BC4
        add    esp, 4
        or     edi, edi
        jne    loc_1_0701
loc_1_071F:
        lea    eax, [esp + 0x1c]
        cmp    dword ptr [esp + 0x1c], eax
        je     loc_1_075E
loc_1_0729:
        mov    esi, dword ptr [esp + 0x1c]
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 0x1c]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   6
        push   dword ptr [esi + 0x18]
        call   dword ptr [esi + 0x14]
        add    esp, 0xc
        push   esi
        call   sub_1_144C
        add    esp, 4
        lea    eax, [esp + 0x1c]
        cmp    dword ptr [esp + 0x1c], eax
        jne    loc_1_0729
loc_1_075E:
        lea    eax, [esp + 0x14]
        cmp    dword ptr [esp + 0x14], eax
        je     loc_1_07AD
loc_1_0768:
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x14], eax
        lea    eax, [esp + 0x14]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   6
        push   dword ptr [esi + 0x18]
        call   dword ptr [esi + 0x14]
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_079A
        push   eax
        call   sub_1_13A8
        add    esp, 4
loc_1_079A:
        push   esi
        call   sub_1_1418
        add    esp, 4
        lea    eax, [esp + 0x14]
        cmp    dword ptr [esp + 0x14], eax
        jne    loc_1_0768
loc_1_07AD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 049h, 000h
loc_1_07B8:
        push   dword ptr [esp + 8]
        call   sub_1_1120
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_07C8:
        push   ebx
        mov    eax, dword ptr [esp + 0xc]
        mov    byte ptr [eax], 0
        mov    ecx, dword ptr [esp + 0x10]
        mov    byte ptr [ecx], 0
        mov    edx, dword ptr [esp + 8]
        or     edx, edx
        je     loc_1_07FE
        cmp    byte ptr [edx], 0
        je     loc_1_07FE
loc_1_07E4:
        mov    bl, byte ptr [edx]
        cmp    bl, 1
        jne    loc_1_07F0
        mov    byte ptr [eax], 1
        jmp    loc_1_07F8
loc_1_07F0:
        cmp    bl, 2
        jne    loc_1_07F8
        mov    byte ptr [ecx], 1
loc_1_07F8:
        inc    edx
        cmp    byte ptr [edx], 0
        jne    loc_1_07E4
loc_1_07FE:
        pop    ebx
        ret    
loc_1_0800:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 0xc]
        call   sub_1_0580
        add    esp, 0xc
        or     al, al
        jne    loc_1_082C
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_082C:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        push   dword ptr [ebp + 0x14]
        call   sub_1_07C8
        add    esp, 0xc
        push   0x98
        call   sub_1_90F6
        add    esp, 4
        mov    esi, eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_087C
        push   dword ptr [ebp - 8]
        call   sub_1_95A8
        add    esp, 4
        or     al, al
        je     loc_1_087C
        or     esi, esi
        je     loc_1_0871
        push   esi
        call   sub_1_90FC
        add    esp, 4
loc_1_0871:
        pop    edi
        mov    eax, 3
        pop    esi
        pop    ebx
        leave  
        ret    
        db  090h
loc_1_087C:
        cmp    word ptr [ebp - 4], 0
        jne    loc_1_0914
        mov    ax, word ptr [0x92f4]         ; -> obj1:0x92F4
        mov    word ptr [ebp - 4], ax
        sub    edi, edi
        mov    ebx, dword ptr [ebp + 0x10]
loc_1_0896:
        cmp    word ptr [ebp - 4], 0x1388
        jbe    loc_1_08A4
        mov    word ptr [ebp - 4], 0x401
loc_1_08A4:
        mov    ah, byte ptr [ebp - 4]
        mov    al, byte ptr [ebp - 3]
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_08BC
        push   ebx
        push   eax
        call   sub_1_0548
        add    esp, 8
        jmp    loc_1_08CB
loc_1_08BC:
        push   0
        push   ebx
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_1_0414
        add    esp, 0x10
loc_1_08CB:
        or     eax, eax
        je     loc_1_08DC
        inc    edi
        inc    word ptr [ebp - 4]
        cmp    edi, 0xf88
        jl     loc_1_0896
loc_1_08DC:
        cmp    edi, 0xf88
        jne    loc_1_08FC
        or     esi, esi
        je     loc_1_08F1
        push   esi
        call   sub_1_90FC
        add    esp, 4
loc_1_08F1:
        pop    edi
        mov    eax, 4
        pop    esi
        pop    ebx
        leave  
        ret    
        db  090h
loc_1_08FC:
        mov    ax, word ptr [ebp - 4]
        inc    ax
        mov    word ptr [0x92f4], ax         ; -> obj1:0x92F4
        mov    ah, byte ptr [ebp - 4]
        mov    al, byte ptr [ebp - 3]
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_096F
loc_1_0914:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_096C
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0934
        mov    ebx, dword ptr [ebp + 0x10]
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_1_0548
        add    esp, 8
        jmp    loc_1_0948
        db  08Dh, 049h, 000h
loc_1_0934:
        push   0
        mov    ebx, dword ptr [ebp + 0x10]
        push   ebx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        call   sub_1_0414
        add    esp, 0x10
loc_1_0948:
        or     eax, eax
        je     loc_1_096F
        cmp    byte ptr [ebp - 2], 0
        jne    loc_1_096F
        or     esi, esi
        je     loc_1_095F
        push   esi
        call   sub_1_90FC
        add    esp, 4
loc_1_095F:
        pop    edi
        mov    eax, 2
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_096C:
        mov    ebx, dword ptr [ebp + 0x10]
loc_1_096F:
        or     esi, esi
        je     loc_1_0A2C
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x26
        rep stosd dword ptr es:[edi], eax
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_95B8
        add    esp, 4
        lea    eax, [esi + 0x3c]
        push   eax
        call   sub_1_95B8
        add    esp, 4
        mov    byte ptr [esi + 0x45], 1
        mov    dword ptr [esi + 0x48], 0
        mov    dword ptr [esi + 0x5c], 0
        mov    dword ptr [esi + 0x54], 0x7b8 ; -> loc_1_07B8
        mov    eax, dword ptr [0x94b4]       ; -> obj1:0x94B4
        mov    dword ptr [esi + 0x60], eax
        lea    eax, [esi + 8]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0xc], eax
        lea    eax, [esi + 0x18]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x1c], eax
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi + 0x30], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x2c], eax
        mov    byte ptr [esi + 0x34], bl
        mov    word ptr [esi + 0x32], 0x80
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_09F0
        mov    word ptr [esi + 0x32], 0x180
loc_1_09F0:
        push   esi
        or     word ptr [esi + 0x32], 8
        call   sub_1_0204
        add    esp, 4
        mov    edi, eax
        cmp    edi, -1
        push   esi
        jne    loc_1_0A18
        call   sub_1_90FC
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0A18:
        call   sub_1_04CC
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0A2C:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
sub_1_0A38:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x18]
        test   byte ptr [esi + 0x32], 0x20
        je     loc_1_0A56
        mov    ecx, dword ptr [esi + 0x10]
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
loc_1_0A56:
        push   esi
        movzx  ecx, byte ptr [esi + 0x35]
        mov    eax, dword ptr [0x9620]       ; -> loc_1_9620
        mov    dword ptr [eax + ecx*4], 0
        call   sub_1_0508
        add    esp, 4
        mov    ecx, dword ptr [esi + 0x4c]
        or     ecx, ecx
        je     loc_1_0AC1
loc_1_0A76:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_1_0AAD
        cmp    byte ptr [eax + 0x84], 0
        je     loc_1_0A99
        test   byte ptr [eax + 0x86], 1
        jne    loc_1_0A99
        inc    dword ptr [eax + 0x74]
        mov    dword ptr [eax + 0xc8], edi
        mov    edi, eax
loc_1_0A99:
        mov    dword ptr [eax + 0xcc], 0
        mov    dword ptr [eax + 0xdc], 0
loc_1_0AAD:
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    loc_1_0A76
loc_1_0AC1:
        lea    ebx, [esi + 0x18]
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 0x18
        je     loc_1_0AF6
loc_1_0ACD:
        mov    ebp, dword ptr [ebx]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        push   0
        push   7
        push   dword ptr [ebp + 0x18]
        call   dword ptr [ebp + 0x14]
        add    esp, 0xc
        push   ebp
        call   sub_1_144C
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 0x18
        jne    loc_1_0ACD
loc_1_0AF6:
        lea    ebx, [esi + 8]
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 8
        je     loc_1_0B3B
loc_1_0B02:
        mov    ebp, dword ptr [ebx]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        push   0
        push   7
        push   dword ptr [ebp + 0x18]
        call   dword ptr [ebp + 0x14]
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_0B29
        push   eax
        call   sub_1_13A8
        add    esp, 4
loc_1_0B29:
        push   ebp
        call   sub_1_1418
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 8
        jne    loc_1_0B02
loc_1_0B3B:
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_1_95C4
        add    esp, 4
        mov    ebx, dword ptr [esi + 0x94]
        or     ebx, ebx
        je     loc_1_0B74
loc_1_0B52:
        push   0
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_95D4
        add    esp, 0xc
        mov    eax, ebx
        mov    ebx, dword ptr [ebx]
        push   eax
        call   sub_1_90FC
        add    esp, 4
        or     ebx, ebx
        jne    loc_1_0B52
loc_1_0B74:
        push   esi
        call   sub_1_90FC
        add    esp, 4
        or     edi, edi
        je     loc_1_0BA7
        mov    ebx, dword ptr [esp + 0x10]
loc_1_0B85:
        dec    dword ptr [edi + 0x74]
        or     dword ptr [edi + 0x58], 0x40000000
        mov    esi, dword ptr [edi + 0xc8]
        push   ebx
        push   0x40
        push   edi
        call   sub_1_6398
        add    esp, 0xc
        mov    edi, esi
        or     esi, esi
        jne    loc_1_0B85
loc_1_0BA7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
sub_1_0BB0:
        mov    eax, dword ptr [0x969c]       ; -> loc_1_969C
        or     eax, eax
        je     loc_1_0BC2
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [0x969c], ecx       ; -> loc_1_969C
loc_1_0BC2:
        ret    
        db  090h
sub_1_0BC4:
        mov    ecx, dword ptr [0x969c]       ; -> loc_1_969C
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x10], ecx
        mov    dword ptr [0x969c], eax       ; -> loc_1_969C
        ret    
        db  090h
loc_1_0BD8:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        push   dword ptr [ebx]
        call   sub_1_01D4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_0BF8
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0BF8:
        mov    ax, word ptr [esi + 0x32]
        test   al, 0x40
        jne    loc_1_0C28
        cmp    word ptr [esi + 0x38], 0
        jne    loc_1_0C28
        or     ax, 0x40
        mov    word ptr [esi + 0x32], ax
        and    ax, 0xff7f
        mov    word ptr [esi + 0x32], ax
        push   esi
        call   sub_1_0A38
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_0C28:
        test   al, 1
        jne    loc_1_0CA4
        mov    edi, dword ptr [esi + 4]
        call   sub_1_0BB0
        or     eax, eax
        je     loc_1_0C98
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 0x14], ecx
        mov    cx, word ptr [esi + 0x32]
        and    cx, 0xfffd
        mov    word ptr [esi + 0x32], cx
        or     cx, 1
        mov    word ptr [esi + 0x32], cx
        and    cx, 0xff7f
        mov    word ptr [esi + 0x32], cx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], 0
        or     edi, edi
        je     loc_1_0C8E
        nop    
loc_1_0C70:
        push   0
        push   7
        push   dword ptr [edi + 0x14]
        call   dword ptr [edi + 0x10]
        add    esp, 0xc
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_0BC4
        add    esp, 4
        or     edi, edi
        jne    loc_1_0C70
loc_1_0C8E:
        pop    edi
        mov    eax, 0xff
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_0C98:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_0CA4:
        pop    edi
        mov    eax, 6
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_0CB0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        lea    esi, [eax + 0x94]
        mov    eax, dword ptr [eax + 0x94]
        or     eax, eax
        je     loc_1_0CDF
        mov    ecx, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 0x10]
loc_1_0CCD:
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_0CD7
        cmp    dword ptr [eax + 8], edx
        je     loc_1_0CDF
loc_1_0CD7:
        mov    esi, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_0CCD
loc_1_0CDF:
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], esi
        pop    esi
        ret    
        db  090h
sub_1_0CE8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x1c], 5
        jne    loc_1_0D4C
        mov    ebp, dword ptr [esp + 0x18]
        add    ebp, 0x20
        push   ebp
        call   sub_1_95C4
        add    esp, 4
        push   ebp
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        call   sub_1_95C0
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_0D28
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_0D28:
        cmp    eax, 0x2afe
        jne    loc_1_0D3C
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0D3C:
        pop    ebp
        mov    eax, 0xb
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_0D4C:
        mov    ecx, dword ptr [esp + 0x20]
        or     ecx, ecx
        jne    loc_1_0D64
        mov    eax, 0xb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_0D64:
        sub    esi, esi
        mov    eax, dword ptr [esp + 0x1c]
        dec    eax
        cmp    eax, 6
        ja     loc_1_0D78
        jmp    dword ptr [eax*4 + 0xf54]     ; -> obj1:0x0F54
        db  090h
loc_1_0D78:
        pop    ebp
        mov    esi, 0xb
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_0D88:
        mov    ebx, dword ptr [esp + 0x24]
        mov    al, byte ptr [ebx]
        mov    ebp, dword ptr [esp + 0x18]
        mov    byte ptr [ebp + 0x29], al
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_0DA0:
        mov    ebx, dword ptr [esp + 0x24]
        mov    al, byte ptr [ebx]
        mov    ebp, dword ptr [esp + 0x18]
        mov    byte ptr [ebp + 0x45], al
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_0DB8:
        cmp    ecx, 4
        jb     loc_1_0DF8
        mov    ebx, dword ptr [esp + 0x24]
        mov    edi, dword ptr [ebx]
        or     edi, edi
        je     loc_1_0DE4
        push   edi
        call   sub_1_95A8
        add    esp, 4
        or     al, al
        je     loc_1_0DE4
        mov    eax, 0xb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_0DE4:
        mov    eax, esi
        mov    ebp, dword ptr [esp + 0x18]
        mov    dword ptr [ebp + 0x48], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_0DF8:
        pop    ebp
        mov    esi, 0xb
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_0E08:
        mov    ebx, dword ptr [esp + 0x24]
        cmp    byte ptr [ebx], 0
        mov    ebp, dword ptr [esp + 0x18]
        je     loc_1_0E24
        or     word ptr [ebp + 0x32], 8
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0E24:
        mov    eax, esi
        and    word ptr [ebp + 0x32], 0xfff7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0E34:
        cmp    ecx, 8
        jb     loc_1_0F44
        mov    ebx, dword ptr [esp + 0x24]
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        mov    ebp, dword ptr [esp + 0x24]
        push   ebp
        call   sub_1_0CB0
        add    esp, 0x10
        cmp    dword ptr [esp + 0x1c], 6
        jne    loc_1_0EB0
        or     eax, eax
        je     loc_1_0E74
        mov    esi, 0xb
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_0E74:
        push   0xc
        call   sub_1_90F6
        add    esp, 4
        or     eax, eax
        jne    loc_1_0E94
        mov    esi, 1
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_0E94:
        mov    ecx, dword ptr [ebp + 0x94]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp + 0x94], eax
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 8], ecx
        jmp    loc_1_0ED5
        db  090h
loc_1_0EB0:
        or     eax, eax
        jne    loc_1_0EC4
        mov    esi, 0xb
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_0EC4:
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [edx], ecx
        push   eax
        call   sub_1_90FC
        add    esp, 4
loc_1_0ED5:
        mov    eax, 0
        cmp    dword ptr [esp + 0x1c], 6
        sete   al
        push   eax
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        call   sub_1_95D4
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        je     loc_1_0F49
        cmp    dword ptr [esp + 0x1c], 6
        jne    loc_1_0F26
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        push   ebp
        call   sub_1_0CB0
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0F26
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [edx], ecx
        push   eax
        call   sub_1_90FC
        add    esp, 4
loc_1_0F26:
        sub    edi, 0x2afe
        cmp    edi, 1
        sbb    esi, esi
        and    esi, 0xfffffff6
        add    esi, 0xb
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_0F44:
        mov    esi, 0xb
loc_1_0F49:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_0D88
        dd  loc_1_0DA0
        dd  loc_1_0DB8
        dd  loc_1_0E08
        dd  loc_1_0D78
        dd  loc_1_0E34
        dd  loc_1_0E34
sub_1_0F70:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   dword ptr [edi]
        call   sub_1_01D4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_0F90
        mov    eax, 6
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_0F90:
        mov    ax, word ptr [esi + 0x32]
        test   al, 0x80
        je     loc_1_1040
        test   al, 0x40
        jne    loc_1_0FEC
        cmp    word ptr [esi + 0x38], 0
        jne    loc_1_0FEC
        or     ax, 0x40
        push   dword ptr [esp + 0x18]
        mov    word ptr [esi + 0x32], ax
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   esi
        call   sub_1_0CE8
        add    esp, 0x10
        mov    edi, eax
        mov    ax, word ptr [esi + 0x32]
        test   al, 7
        jne    loc_1_0FDC
        and    ax, 0xffbf
        mov    word ptr [esi + 0x32], ax
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_0FDC:
        push   esi
        call   sub_1_1120
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_0FEC:
        call   sub_1_0BB0
        or     eax, eax
        je     loc_1_1038
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [eax + 0x14], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0xc], ecx
        or     word ptr [esi + 0x32], 2
        add    esi, 4
        cmp    dword ptr [esi], 0
        je     loc_1_102B
loc_1_1024:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    loc_1_1024
loc_1_102B:
        mov    dword ptr [esi], eax
        mov    eax, 0xff
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_1038:
        pop    edi
        mov    eax, 1
        pop    esi
        ret    
loc_1_1040:
        pop    edi
        mov    eax, 6
        pop    esi
        ret    
loc_1_1048:
        push   dword ptr [esp + 4]
        call   sub_1_01D4
        add    esp, 4
        or     eax, eax
        je     loc_1_1100
        test   byte ptr [eax + 0x32], 0x80
        je     loc_1_1100
        sub    edx, edx
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, 5
        ja     loc_1_1078
        jmp    dword ptr [ecx*4 + 0x1108]    ; -> obj1:0x1108
loc_1_1078:
        mov    edx, 0xa
        mov    eax, edx
        ret    
loc_1_1080:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x64], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x68], ecx
        mov    eax, edx
        ret    
        db  08Dh, 049h, 000h
loc_1_1094:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x6c], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x70], ecx
        mov    eax, edx
        ret    
        db  08Dh, 049h, 000h
loc_1_10A8:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x74], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x78], ecx
        mov    eax, edx
        ret    
        db  08Dh, 049h, 000h
loc_1_10BC:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x7c], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x80], ecx
        mov    eax, edx
        ret    
loc_1_10D0:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x84], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x88], ecx
        mov    eax, edx
        ret    
        db  090h
loc_1_10E8:
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x8c], ecx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x90], ecx
        mov    eax, edx
        ret    
        db  090h
loc_1_1100:
        mov    eax, 6
        ret    
        db  08Bh, 0FFh
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_1080
        dd  loc_1_1094
        dd  loc_1_10A8
        dd  loc_1_10BC
        dd  loc_1_10D0
        dd  loc_1_10E8
sub_1_1120:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        test   byte ptr [esi + 0x32], 7
        je     loc_1_11DF
loc_1_1130:
        mov    edi, dword ptr [esi + 4]
        mov    ax, word ptr [esi + 0x32]
        test   al, 1
        jne    loc_1_11E8
        test   al, 2
        je     loc_1_1183
        nop    
loc_1_1144:
        mov    edi, dword ptr [esi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        jne    loc_1_1156
        and    word ptr [esi + 0x32], 0xfffd
loc_1_1156:
        push   dword ptr [edi + 0xc]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_1_0CE8
        add    esp, 0x10
        push   0
        push   eax
        push   dword ptr [edi + 0x14]
        call   dword ptr [edi + 0x10]
        add    esp, 0xc
        push   edi
        call   sub_1_0BC4
        add    esp, 4
        test   byte ptr [esi + 0x32], 2
        jne    loc_1_1144
loc_1_1183:
        mov    dx, word ptr [esi + 0x32]
        test   dl, 4
        je     loc_1_11D5
        lea    ecx, [esi + 8]
        mov    edi, dword ptr [ecx]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 8
        je     loc_1_11CC
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        inc    word ptr [esi + 0x38]
        and    dx, 0xffbf
        mov    word ptr [esi + 0x32], dx
        push   edi
        push   esi
        call   sub_1_1590
        add    esp, 8
        mov    ax, word ptr [esi + 0x38]
        dec    ax
        mov    word ptr [esi + 0x38], ax
        jne    loc_1_11DF
        or     word ptr [esi + 0x32], 0x40
        jmp    loc_1_11D5
        db  090h
loc_1_11CC:
        and    dx, 0xfffb
        mov    word ptr [esi + 0x32], dx
loc_1_11D5:
        test   byte ptr [esi + 0x32], 7
        jne    loc_1_1130
loc_1_11DF:
        pop    edi
        and    word ptr [esi + 0x32], 0xffbf
        pop    esi
        ret    
loc_1_11E8:
        push   esi
        call   sub_1_0A38
        add    esp, 4
        push   0
        push   0
        push   dword ptr [edi + 0x14]
        call   dword ptr [edi + 0x10]
        add    esp, 0xc
        push   edi
        call   sub_1_0BC4
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_120C:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x38]
        dec    ax
        mov    word ptr [ecx + 0x38], ax
        jne    loc_1_123D
        mov    ax, word ptr [ecx + 0x32]
        test   al, 0x40
        jne    loc_1_123D
        test   al, 7
        je     loc_1_123D
        or     ax, 0x40
        mov    word ptr [ecx + 0x32], ax
        push   ecx
        add    ecx, 0x50
        push   ecx
        call   sub_1_90EA
        add    esp, 8
loc_1_123D:
        ret    
        db  08Bh, 0FFh
sub_1_1240:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x38]
        dec    ax
        mov    word ptr [ecx + 0x38], ax
        jne    loc_1_126D
        mov    ax, word ptr [ecx + 0x32]
        test   al, 0x40
        jne    loc_1_126D
        test   al, 7
        je     loc_1_126D
        or     ax, 0x40
        mov    word ptr [ecx + 0x32], ax
        push   ecx
        call   sub_1_1120
        add    esp, 4
loc_1_126D:
        ret    
        db  08Bh, 0FFh
sub_1_1270:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        mov    eax, dword ptr [0x92f8]       ; -> obj1:0x92F8
        cmp    eax, dword ptr [0x92fc]       ; -> obj1:0x92FC
        jae    loc_1_139C
        push   0xc
        call   sub_1_90F6
        add    esp, 4
        mov    edi, eax
        cmp    edi, esi
        jne    loc_1_12A4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_12A4:
        push   5
        lea    eax, [edi + 4]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_91DE
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_12D0
        push   edi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_12D0:
        mov    ebx, dword ptr [0x9594]       ; -> obj1:0x9594
        add    ebx, 8
        lea    eax, [ebx + ebx*4]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    dword ptr [esp + 0x1c], eax
        or     eax, eax
        jne    loc_1_1308
        push   dword ptr [edi + 4]
        call   sub_1_91E4
        push   edi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_1308:
        sub    ebp, ebp
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esp + 0x18], eax
loc_1_1315:
        push   ebx
        push   dword ptr [esp + 0x1c]
        push   dword ptr [edi + 4]
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_91EA
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_1374
        or     ebp, ebp
        je     loc_1_1348
        push   dword ptr [esp + 0x10]
        call   sub_1_13A8
        add    esp, 4
        jmp    loc_1_134C
        db  08Dh, 049h, 000h
loc_1_1348:
        mov    esi, dword ptr [esp + 0x10]
loc_1_134C:
        inc    ebp
        add    dword ptr [esp + 0x18], ebx
        cmp    ebp, 5
        jb     loc_1_1315
        add    dword ptr [0x92f8], 5         ; -> obj1:0x92F8
        mov    eax, dword ptr [0x9300]       ; -> obj1:0x9300
        mov    dword ptr [edi], eax
        mov    dword ptr [0x9300], edi       ; -> obj1:0x9300
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_1374:
        push   dword ptr [edi + 4]
        call   sub_1_91E4
        push   edi
        call   sub_1_90FC
        add    esp, 4
        push   dword ptr [esp + 0x1c]
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_139C:
        sub    esi, esi
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_13A8:
        mov    eax, dword ptr [0x960c]       ; -> loc_1_960C
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x960c], ecx       ; -> loc_1_960C
        ret    
        db  08Bh, 0FFh
sub_1_13BC:
        mov    edx, dword ptr [esp + 4]
        mov    ax, word ptr [edx + 0x32]
        test   al, 0x20
        jne    loc_1_13F6
        and    ax, 0xfffb
        mov    word ptr [edx + 0x32], ax
        or     ax, 0x20
        mov    word ptr [edx + 0x32], ax
        lea    ecx, [edx + 0x10]
        mov    eax, dword ptr [0x9660]       ; -> loc_1_9660
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0x9660]       ; -> loc_1_9660
        mov    dword ptr [edx + 0x14], eax
        mov    dword ptr [0x9660], ecx       ; -> loc_1_9660
        mov    dword ptr [ecx], 0x965c       ; -> loc_1_965C
loc_1_13F6:
        ret    
        db  090h
sub_1_13F8:
        mov    eax, dword ptr [0x9694]       ; -> loc_1_9694
        or     eax, eax
        je     loc_1_140C
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x9694], ecx       ; -> loc_1_9694
        ret    
        db  08Bh, 0FFh
loc_1_140C:
        push   0x20
        call   sub_1_90F6
        add    esp, 4
        ret    
        db  090h
sub_1_1418:
        mov    ecx, dword ptr [0x9694]       ; -> loc_1_9694
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x9694], eax       ; -> loc_1_9694
        ret    
        db  08Bh, 0FFh
sub_1_142C:
        mov    eax, dword ptr [0x9644]       ; -> loc_1_9644
        or     eax, eax
        je     loc_1_1440
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x9644], ecx       ; -> loc_1_9644
        ret    
        db  08Bh, 0FFh
loc_1_1440:
        push   0x20
        call   sub_1_90F6
        add    esp, 4
        ret    
        db  090h
sub_1_144C:
        mov    ecx, dword ptr [0x9644]       ; -> loc_1_9644
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x9644], eax       ; -> loc_1_9644
        ret    
        db  08Bh, 0FFh
loc_1_1460:
        push   esi
        cmp    dword ptr [0x95ec], 0x95ec    ; -> loc_1_95EC
        je     loc_1_14DC
loc_1_146D:
        mov    esi, dword ptr [0x95ec]       ; -> loc_1_95EC
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x95ec], eax       ; -> loc_1_95EC
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], 0x95ec   ; -> loc_1_95EC
        sub    esi, 0x10
        mov    ax, word ptr [esi + 0x32]
        and    ax, 0xffdf
        mov    word ptr [esi + 0x32], ax
        test   al, 0x40
        jne    loc_1_14C8
        lea    edx, [esi + 8]
        mov    ecx, dword ptr [edx]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 8
        je     loc_1_14D0
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        inc    word ptr [esi + 0x38]
        push   ecx
        push   esi
        call   sub_1_1590
        add    esp, 8
        push   esi
        call   sub_1_1240
        add    esp, 4
        jmp    loc_1_14D0
        db  08Bh, 0FFh
loc_1_14C8:
        or     ax, 4
        mov    word ptr [esi + 0x32], ax
loc_1_14D0:
        cmp    dword ptr [0x95ec], 0x95ec    ; -> loc_1_95EC
        jne    loc_1_146D
loc_1_14DC:
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_14E0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        mov    edi, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x18]
        cmp    dword ptr [0x965c], 0x965c    ; -> loc_1_965C
        mov    si, word ptr [ebp + 0x1c]
        jne    loc_1_150C
        push   dword ptr [esp + 0x18]
        call   sub_1_13A8
        add    esp, 4
        jmp    loc_1_1572
loc_1_150C:
        mov    ecx, dword ptr [0x965c]       ; -> loc_1_965C
        mov    eax, ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x965c], eax       ; -> loc_1_965C
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 4], 0x965c   ; -> loc_1_965C
        sub    ecx, 0x10
        mov    eax, dword ptr [ecx + 8]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 8
        je     loc_1_156C
        mov    edx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x10], edx
        lea    eax, [ecx + 0x10]
        mov    edx, dword ptr [0x95f0]       ; -> loc_1_95F0
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [0x95f0]       ; -> loc_1_95F0
        mov    dword ptr [ecx + 0x14], edx
        mov    dword ptr [0x95f0], eax       ; -> loc_1_95F0
        mov    dword ptr [eax], 0x95ec       ; -> loc_1_95EC
        push   0
        push   0x974c                        ; -> loc_1_974C
        call   sub_1_90EA
        add    esp, 8
        jmp    loc_1_1572
        db  08Bh, 0FFh
loc_1_156C:
        and    word ptr [ecx + 0x32], 0xffdf
loc_1_1572:
        push   ebp
        call   sub_1_1418
        add    esp, 4
        or     edi, edi
        je     loc_1_158B
        movzx  eax, si
        push   eax
        push   0
        push   ebx
        call   edi
        add    esp, 0xc
loc_1_158B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1590:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x28]
        inc    esi
        mov    esi, dword ptr [esp + 0x2c]
loc_1_15A0:
        mov    ebx, dword ptr [esi + 0x10]
        or     ebx, ebx
        jne    loc_1_15CE
        mov    eax, dword ptr [0x960c]       ; -> loc_1_960C
        or     eax, eax
        je     loc_1_15BC
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [0x960c], ecx       ; -> loc_1_960C
        jmp    loc_1_15C1
loc_1_15BC:
        call   sub_1_1270
loc_1_15C1:
        mov    ebx, eax
        or     eax, eax
        je     loc_1_1754
        mov    dword ptr [esi + 0x10], eax
loc_1_15CE:
        mov    al, byte ptr [esi + 8]
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_15E0
        mov    edi, dword ptr [ebp + 0x2c]
        lea    eax, [ebp + 0x20]
        jmp    loc_1_15E6
        db  090h
loc_1_15E0:
        mov    edi, dword ptr [ebp + 0x48]
        lea    eax, [ebp + 0x3c]
loc_1_15E6:
        mov    dword ptr [esp + 0x20], eax
        test   byte ptr [ebp + 0x33], 1
        jne    loc_1_161C
        push   dword ptr [esp + 0x20]
        lea    eax, [esp + 0x1a]
        push   eax
        lea    eax, [esp + 0x1b]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   edi
        push   dword ptr [esi + 8]
        call   sub_1_95A0
        add    esp, 0x18
        mov    dword ptr [esp + 0x18], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    loc_1_1631
loc_1_161C:
        mov    eax, 1
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x1c], 0
loc_1_1631:
        or     eax, eax
        je     loc_1_170C
        or     edi, edi
        je     loc_1_1641
        mov    dword ptr [esp + 0x18], edi
loc_1_1641:
        mov    edi, dword ptr [ebx + 4]
        add    edi, dword ptr [0x9594]       ; -> obj1:0x9594
        mov    dword ptr [ebx + 0xc], 8
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebx], eax
        mov    ax, word ptr [ebp + 0x30]
        mov    word ptr [edi], ax
        mov    ax, word ptr [esi + 0x1e]
        mov    word ptr [edi + 2], ax
        mov    ax, word ptr [esi + 0x1c]
        add    ax, 8
        mov    word ptr [esp + 0x14], ax
        mov    ah, al
        mov    al, byte ptr [esp + 0x15]
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 6], 0
        test   byte ptr [ebp + 0x32], 8
        je     loc_1_16C4
        push   ebx
        movzx  ecx, word ptr [esi + 0xa]
        movzx  edx, word ptr [esi + 8]
        add    ecx, edx
        movzx  edx, word ptr [esp + 0x1e]
        add    ecx, edx
        movzx  edx, word ptr [esp + 0x1c]
        add    ecx, edx
        and    eax, 0xffff
        add    ecx, eax
        add    ecx, 0x1100
        push   ecx
        call   sub_1_1D1C
        add    esp, 8
        cmp    ax, 0xffff
        je     loc_1_16C0
        not    ax
loc_1_16C0:
        mov    word ptr [edi + 6], ax
loc_1_16C4:
        inc    dword ptr [0x9588]            ; -> obj1:0x9588
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x20]
        push   dword ptr [esi + 8]
        movzx  eax, word ptr [esp + 0x24]
        push   eax
        push   ebx
        push   esi
        push   dword ptr [0x95e4]            ; -> loc_1_95E4
        call   sub_1_9598
        add    esp, 0x20
        mov    edi, eax
        push   dword ptr [esp + 0x1c]
        call   sub_1_95A4
        add    esp, 4
        cmp    edi, 0x2bf7
        je     loc_1_173A
        jmp    loc_1_1730
        db  08Dh, 049h, 000h
loc_1_170C:
        mov    eax, 3
        cmp    byte ptr [esp + 0x13], 0xff
        je     loc_1_171D
        mov    eax, 0x1c
loc_1_171D:
        push   0
        push   eax
        push   dword ptr [esi + 0x18]
        call   dword ptr [esi + 0x14]
        add    esp, 0xc
        mov    dword ptr [esi + 0x14], 0
loc_1_1730:
        push   ebx
        push   esi
        call   sub_1_14E0
        add    esp, 8
loc_1_173A:
        lea    ecx, [ebp + 8]
        mov    esi, dword ptr [ecx]
        mov    eax, esi
        sub    eax, ebp
        cmp    eax, 8
        je     loc_1_1774
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        jmp    loc_1_15A0
loc_1_1754:
        lea    ecx, [ebp + 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [ecx], esi
        push   ebp
        call   sub_1_13BC
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_1774:
        and    word ptr [ebp + 0x32], 0xfffb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
loc_1_1784:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        cmp    ebx, 0xfff7
        ja     loc_1_18C0
        call   sub_1_13F8
        mov    edi, eax
        mov    ebp, dword ptr [esp + 0x14]
        push   dword ptr [ebp]
        call   sub_1_01D4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_18A8
        test   byte ptr [esi + 0x32], 0x80
        je     loc_1_18A8
        or     edi, edi
        je     loc_1_189C
        lea    eax, [edi + 0x1e]
        push   eax
        lea    eax, [edi + 8]
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        push   dword ptr [eax + 0x14]
        call   sub_1_0580
        add    esp, 0xc
        or     al, al
        je     loc_1_1888
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [edi + 0x1c], bx
        mov    ax, word ptr [esi + 0x32]
        test   al, 0x20
        jne    loc_1_1864
        test   al, 0x40
        jne    loc_1_185C
        mov    eax, dword ptr [0x960c]       ; -> loc_1_960C
        or     eax, eax
        je     loc_1_1820
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x960c], ecx       ; -> loc_1_960C
        jmp    loc_1_1825
        db  090h
loc_1_1820:
        call   sub_1_1270
loc_1_1825:
        or     eax, eax
        mov    dword ptr [edi + 0x10], eax
        je     loc_1_1850
        inc    word ptr [esi + 0x38]
        push   edi
        push   esi
        call   sub_1_1590
        add    esp, 8
        push   esi
        call   sub_1_1240
        add    esp, 4
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_1850:
        push   esi
        call   sub_1_13BC
        add    esp, 4
        jmp    loc_1_1864
        db  090h
loc_1_185C:
        or     ax, 4
        mov    word ptr [esi + 0x32], ax
loc_1_1864:
        add    esi, 8
        mov    dword ptr [edi + 0x10], 0
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [edi], esi
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_1888:
        push   edi
        call   sub_1_1418
        add    esp, 4
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_189C:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_18A8:
        or     edi, edi
        je     loc_1_18B5
        push   edi
        call   sub_1_1418
        add    esp, 4
loc_1_18B5:
        pop    ebp
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_18C0:
        pop    ebp
        mov    eax, 0x23
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_18CC:
        push   ebx
        push   esi
        push   edi
        call   sub_1_142C
        mov    edi, eax
        mov    ebx, dword ptr [esp + 0x10]
        push   dword ptr [ebx]
        call   sub_1_01D4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_1994
        test   byte ptr [esi + 0x32], 0x80
        je     loc_1_1994
        or     edi, edi
        je     loc_1_1988
        mov    ecx, dword ptr [esp + 0x14]
        or     ecx, ecx
        je     loc_1_1924
        cmp    dword ptr [ecx + 0x10], 0
        je     loc_1_1924
        lea    eax, [edi + 0x1e]
        push   eax
        lea    eax, [edi + 8]
        push   eax
        push   dword ptr [ecx + 0x14]
        call   sub_1_0580
        add    esp, 0xc
        jmp    loc_1_1933
        db  090h
loc_1_1924:
        mov    al, 1
        mov    dword ptr [edi + 8], 0
        mov    word ptr [edi + 0x1e], 0
loc_1_1933:
        or     al, al
        je     loc_1_1974
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x10], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    word ptr [edi + 0x1c], ax
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 0x1c], edi
        add    esi, 0x18
        mov    dword ptr [edi], esi
        mov    eax, 0xff
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_1974:
        push   edi
        call   sub_1_144C
        add    esp, 4
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_1988:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_1994:
        or     edi, edi
        je     loc_1_19A1
        push   edi
        call   sub_1_144C
        add    esp, 4
loc_1_19A1:
        pop    edi
        mov    eax, 6
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_19AC:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], 0
        mov    dword ptr [esp + 0x10], 0
        mov    esi, dword ptr [esp + 0x38]
        test   byte ptr [esi + 0x32], 0x80
        je     loc_1_1B30
        mov    edi, dword ptr [esi + 0x18]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x18
        mov    ebx, dword ptr [esp + 0x3c]
        mov    bp, word ptr [esp + 0x40]
        je     loc_1_1A64
loc_1_19E6:
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     loc_1_19F1
        cmp    ebx, eax
        jne    loc_1_19FF
loc_1_19F1:
        mov    ax, word ptr [edi + 0x1e]
        or     ax, ax
        je     loc_1_1A0C
        cmp    bp, ax
        je     loc_1_1A0C
loc_1_19FF:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x18
        jne    loc_1_19E6
        jmp    loc_1_1A64
loc_1_1A0C:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        push   8
        movzx  eax, word ptr [edi + 0x1c]
        push   eax
        push   dword ptr [edi + 0xc]
        push   dword ptr [esp + 0x50]
        call   sub_1_1DD0
        add    esp, 0x10
        mov    esi, eax
        push   ebp
        push   ebx
        push   dword ptr [esp + 0x54]
        push   dword ptr [edi + 0x10]
        call   sub_1_1F9C
        add    esp, 0x10
        inc    dword ptr [0x957c]            ; -> obj1:0x957C
        push   esi
        push   eax
        push   dword ptr [edi + 0x18]
        call   dword ptr [edi + 0x14]
        add    esp, 0xc
        push   edi
        call   sub_1_144C
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Dh, 049h, 000h
loc_1_1A64:
        mov    edi, dword ptr [esi + 0x84]
        or     edi, edi
        je     loc_1_1B20
        mov    eax, dword ptr [esi + 0x88]
        mov    dword ptr [esp + 0x18], eax
        inc    word ptr [esi + 0x38]
        push   ebp
        push   ebx
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_1F70
        add    esp, 0xc
        inc    dword ptr [0x957c]            ; -> obj1:0x957C
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x48]
        mov    eax, dword ptr [eax + 8]
        add    eax, 8
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x54]
        sub    eax, 8
        push   eax
        mov    eax, dword ptr [esp + 0x54]
        mov    eax, dword ptr [eax + 0xc]
        sub    eax, 8
        push   eax
        push   0x200
        mov    eax, dword ptr [esp + 0x64]
        push   dword ptr [eax]
        movzx  eax, byte ptr [eax + 8]
        push   eax
        lea    eax, [esp + 0x3c]
        push   eax
        push   0x16
        push   dword ptr [esp + 0x40]
        call   edi
        add    esp, 0x2c
        cmp    eax, 0x18
        jne    loc_1_1B0F
        mov    eax, dword ptr [esp + 0x14]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   dword ptr [esp + 0x50]
        call   sub_1_1DD0
        add    esp, 0x10
        push   eax
        push   0
        mov    eax, dword ptr [esp + 0x18]
        push   dword ptr [eax + 0xc]
        call   dword ptr [eax + 8]
        add    esp, 0xc
loc_1_1B0F:
        push   esi
        call   sub_1_120C
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
loc_1_1B20:
        pop    ebp
        inc    dword ptr [0x9584]            ; -> obj1:0x9584
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Bh, 0FFh
loc_1_1B30:
        pop    ebp
        inc    dword ptr [0x9584]            ; -> obj1:0x9584
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Bh, 0FFh
loc_1_1B40:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x2c]
        call   sub_1_95A8
        add    esp, 4
        test   al, 1
        je     loc_1_1B78
        cmp    dword ptr [esp + 0x2c], 0
        jne    loc_1_1B66
        cmp    byte ptr [esp + 0x3c], 0
        jne    loc_1_1B78
loc_1_1B66:
        sub    eax, eax
        inc    dword ptr [0x9584]            ; -> obj1:0x9584
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_1B78:
        mov    esi, dword ptr [esp + 0x34]
        mov    edi, dword ptr [esi + 8]
        mov    ax, word ptr [edi + 4]
        mov    bx, ax
        and    ebx, 0xff
        shl    ebx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     ebx, eax
        cmp    ebx, 8
        jae    loc_1_1BB0
        inc    dword ptr [0x9584]            ; -> obj1:0x9584
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_1BB0:
        cmp    dword ptr [esp + 0x38], ebx
        je     loc_1_1BD6
        jbe    loc_1_1C28
        mov    ecx, esi
        mov    edx, ebx
        or     esi, esi
        je     loc_1_1BD6
loc_1_1BC0:
        mov    eax, dword ptr [ecx + 0xc]
        sub    eax, edx
        sbb    ebp, ebp
        and    eax, ebp
        add    eax, edx
        mov    dword ptr [ecx + 0xc], eax
        sub    edx, eax
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_1BC0
loc_1_1BD6:
        cmp    word ptr [edi + 6], 0
        je     loc_1_1C38
        push   esi
        mov    ah, bl
        mov    al, bh
        movzx  ecx, ax
        movzx  eax, word ptr [esp + 0x32]
        add    ecx, eax
        movzx  eax, word ptr [esp + 0x30]
        add    ecx, eax
        movzx  eax, word ptr [esp + 0x2e]
        add    ecx, eax
        movzx  eax, word ptr [esp + 0x2c]
        add    ecx, eax
        add    ecx, 0x1100
        push   ecx
        call   sub_1_1D9C
        add    esp, 8
        cmp    ax, 0xffff
        je     loc_1_1C38
        inc    dword ptr [0x9584]            ; -> obj1:0x9584
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_1C28:
        sub    eax, eax
        inc    dword ptr [0x9584]            ; -> obj1:0x9584
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_1C38:
        cmp    byte ptr [esp + 0x3c], 0
        jne    loc_1_1C94
        push   0
        push   0x11
        mov    ax, word ptr [edi + 2]
        push   eax
        push   dword ptr [esp + 0x34]
        call   sub_1_0414
        add    esp, 0x10
        or     eax, eax
        je     loc_1_1C80
        push   dword ptr [esp + 0x10]
        mov    ebp, dword ptr [esp + 0x48]
        push   ebp
        push   ebx
        push   esi
        mov    cx, word ptr [edi]
        push   ecx
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_19AC
        add    esp, 0x1c
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_1C80:
        pop    ebp
        inc    dword ptr [0x9580]            ; -> obj1:0x9580
        mov    eax, 0x2b2a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_1C94:
        lea    eax, [esp + 0x14]
        push   eax
        push   0x11
        mov    ax, word ptr [edi + 2]
        push   eax
        push   dword ptr [esp + 0x3c]
        call   sub_1_04A0
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_1CC0
        inc    dword ptr [0x9580]            ; -> obj1:0x9580
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_1CC0:
        mov    ebp, dword ptr [esp + 0x44]
loc_1_1CC4:
        mov    cx, word ptr [edi]
        push   dword ptr [esp + 0x10]
        push   ebp
        push   ebx
        push   esi
        push   ecx
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_19AC
        add    esp, 0x1c
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_0474
        add    esp, 4
        or     eax, eax
        jne    loc_1_1CC4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_1CF8:
        cmp    byte ptr [esp + 4], 1
        jne    loc_1_1D1B
        cmp    dword ptr [esp + 8], 0x2b0e
        jne    loc_1_1D1B
        push   0
        push   0
        push   0
        push   0
        push   0x11
        call   0
        add    esp, 0x14
loc_1_1D1B:
        ret    
sub_1_1D1C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [0x9594]       ; -> obj1:0x9594
        sub    ebp, ebp
        sub    esi, esi
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, ecx
        shl    eax, 0x10
        mov    edx, ecx
        shr    edx, 0x10
        or     eax, edx
        add    eax, ecx
        shr    eax, 0x10
        inc    edi
        mov    edi, dword ptr [esp + 0x18]
loc_1_1D44:
        test   ebp, 1
        je     loc_1_1D61
        mov    ecx, eax
        and    ecx, 0xff
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        mov    eax, ecx
        xor    esi, 1
loc_1_1D61:
        mov    ecx, dword ptr [edi + 4]
        add    ecx, ebx
        push   dword ptr [edi + 0xc]
        push   ecx
        push   eax
        call   sub_1_8F49
        add    esp, 0xc
        sub    ebx, ebx
        mov    ebp, dword ptr [edi + 0xc]
        mov    edi, dword ptr [edi]
        cmp    edi, ebx
        jne    loc_1_1D44
        cmp    esi, ebx
        je     loc_1_1D94
        mov    ecx, eax
        and    ecx, 0xff
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        mov    eax, ecx
loc_1_1D94:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_1_1D9C:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, ecx
        shl    eax, 0x10
        mov    edx, ecx
        shr    edx, 0x10
        or     eax, edx
        add    eax, ecx
        shr    eax, 0x10
        mov    esi, dword ptr [esp + 0xc]
loc_1_1DB6:
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   eax
        call   sub_1_8F49
        add    esp, 0xc
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_1DB6
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_1DD0:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ebx, dword ptr [esp + 0x24]
        cmp    ebx, eax
        je     loc_1_1E8A
        mov    dword ptr [esp + 0x10], eax
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [esp + 0x14], ecx
        mov    edx, dword ptr [esp + 0x20]
        inc    ecx
        mov    ecx, dword ptr [edx + 0xc]
        sub    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [esp + 0x18], ecx
loc_1_1E00:
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x14]
        sub    edi, ecx
        sbb    esi, esi
        and    edi, esi
        add    edi, ecx
        mov    ecx, dword ptr [esp + 0x28]
        sub    edi, ecx
        sbb    esi, esi
        and    edi, esi
        lea    ebp, [edi + ecx]
        mov    esi, dword ptr [edx + 8]
        mov    edi, dword ptr [ebx + 4]
        add    edi, dword ptr [esp + 0x10]
        mov    ecx, ebp
        shr    ecx, 2
        add    esi, dword ptr [esp + 0x2c]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    eax, ebp
        sub    dword ptr [esp + 0x18], ebp
        jne    loc_1_1E58
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     loc_1_1E8A
        mov    dword ptr [esp + 0x2c], 0
        mov    ecx, dword ptr [edx + 0xc]
        mov    dword ptr [esp + 0x18], ecx
        jmp    loc_1_1E5C
loc_1_1E58:
        add    dword ptr [esp + 0x2c], ebp
loc_1_1E5C:
        sub    dword ptr [esp + 0x14], ebp
        jne    loc_1_1E7C
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     loc_1_1E8A
        mov    dword ptr [esp + 0x10], 0
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [esp + 0x14], ecx
        jmp    loc_1_1E80
        db  08Dh, 049h, 000h
loc_1_1E7C:
        add    dword ptr [esp + 0x10], ebp
loc_1_1E80:
        sub    dword ptr [esp + 0x28], ebp
        jne    loc_1_1E00
loc_1_1E8A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
sub_1_1E94:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [esp + 0x20]
        cmp    dword ptr [ebx + 0xc], ebp
        ja     loc_1_1EAF
loc_1_1EA5:
        sub    ebp, dword ptr [ebx + 0xc]
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx + 0xc], ebp
        jbe    loc_1_1EA5
loc_1_1EAF:
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_1EF2
loc_1_1EB6:
        mov    edx, dword ptr [ebx + 0xc]
        sub    edx, ebp
        mov    eax, edx
        mov    ecx, dword ptr [esp + 0x1c]
        sub    eax, ecx
        sbb    esi, esi
        and    eax, esi
        add    eax, ecx
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [esp + 0x14]
        add    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    edx, eax
        jne    loc_1_1EE8
        mov    ebx, dword ptr [ebx]
        sub    ebp, ebp
loc_1_1EE8:
        add    dword ptr [esp + 0x14], eax
        sub    dword ptr [esp + 0x1c], eax
        jne    loc_1_1EB6
loc_1_1EF2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_1EF8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x24]
        mov    esi, dword ptr [ecx]
        mov    eax, dword ptr [esp + 0x18]
        mov    edx, dword ptr [eax + 4]
        add    edx, esi
        mov    ecx, dword ptr [eax + 0xc]
        sub    ecx, esi
        mov    dword ptr [esp + 0x10], ecx
        inc    ebx
        mov    ebx, dword ptr [esp + 0x20]
loc_1_1F1C:
        mov    edi, edx
        mov    ecx, dword ptr [esp + 0x10]
        sub    ecx, ebx
        sbb    esi, esi
        and    ecx, esi
        lea    ebp, [ecx + ebx]
        mov    esi, dword ptr [esp + 0x1c]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    edx, ebp
        add    dword ptr [esp + 0x1c], ebp
        sub    ebx, ebp
        je     loc_1_1F5C
        sub    dword ptr [esp + 0x10], ebp
        jne    loc_1_1F1C
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esp + 0x10], ecx
        jmp    loc_1_1F1C
        db  090h
loc_1_1F5C:
        sub    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_1F70:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 1
        add    ecx, 4
        mov    word ptr [ecx + 2], 2
        mov    word ptr [ecx], 0xe
        mov    ax, word ptr [esp + 0xc]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 6], eax
        ret    
        db  08Dh, 049h, 000h
sub_1_1F9C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    edx, dword ptr [esp + 0x14]
        cmp    edx, ebx
        je     loc_1_201C
        mov    dword ptr [edx], ebx
        mov    ebp, dword ptr [edx + 8]
        cmp    ebp, ebx
        je     loc_1_1FE0
        mov    ecx, dword ptr [esp + 0x18]
        movzx  eax, byte ptr [ecx + 8]
        cmp    eax, ebp
        jle    loc_1_1FC8
        mov    ebx, 9
        jmp    loc_1_1FCA
        db  08Bh, 0FFh
loc_1_1FC8:
        mov    ebp, eax
loc_1_1FCA:
        mov    esi, dword ptr [ecx]
        mov    edi, dword ptr [edx + 0xc]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [edx + 8], ebp
loc_1_1FE0:
        mov    eax, dword ptr [edx + 0x10]
        or     eax, eax
        je     loc_1_201C
        cmp    eax, 0x16
        jb     loc_1_2010
        mov    dword ptr [edx + 0x10], 0x16
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x20]
        push   dword ptr [edx + 0x14]
        call   sub_1_1F70
        add    esp, 0xc
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_2010:
        mov    ebx, 0x1a
        mov    dword ptr [edx + 0x10], 0
loc_1_201C:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_2024:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        test   byte ptr [esi + 0x58], 1
        jne    loc_1_20F5
        lea    eax, [esp + 0xe]
        push   eax
        push   dword ptr [esi + 0x68]
        call   sub_1_95AC
        add    esp, 8
        or     al, al
        je     loc_1_20F5
        sub    word ptr [esp + 0xe], 0x14
        mov    ax, word ptr [esi + 0x80]
        mov    cx, word ptr [esp + 0xe]
        sub    ax, cx
        sbb    dx, dx
        and    ax, dx
        add    ax, cx
        mov    word ptr [esp + 0xe], ax
        mov    ebx, dword ptr [esi + 0x94]
        movzx  edi, ax
        mov    eax, ebx
        sub    edx, edx
        div    edi
        cmp    eax, 4
        jae    loc_1_20D8
        cmp    word ptr [esp + 0xe], 0x3fff
        ja     loc_1_20A0
        lea    eax, [edi*4]
        mov    dword ptr [esi + 0x94], eax
        jmp    loc_1_20C6
loc_1_20A0:
        sub    edx, edx
        mov    eax, 0xffff
        div    edi
        cmp    eax, 1
        je     loc_1_20BC
        imul   eax, edi
        mov    dword ptr [esi + 0x94], eax
        jmp    loc_1_20C6
        db  08Dh, 049h, 000h
loc_1_20BC:
        mov    dword ptr [esi + 0x94], 0xffff
loc_1_20C6:
        pop    edi
        mov    eax, dword ptr [esi + 0x94]
        mov    dword ptr [esi + 0x44], eax
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_20D8:
        mov    ecx, edi
        imul   ecx, eax
        cmp    ecx, ebx
        je     loc_1_20F5
        inc    eax
        imul   eax, edi
        cmp    eax, 0xffff
        ja     loc_1_20F5
        mov    dword ptr [esi + 0x94], eax
        mov    dword ptr [esi + 0x44], eax
loc_1_20F5:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_20FC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_22C1
        mov    eax, dword ptr [esi + 0x58]
        test   al, 0x40
        jne    loc_1_22C1
        cmp    dword ptr [esi + 0xa0], 0
        je     loc_1_22C1
        or     eax, 0x40
        mov    dword ptr [esi + 0x58], eax
loc_1_2131:
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0xa0]
loc_1_2139:
        test   byte ptr [edi + 0x17], 0x80
        je     loc_1_2190
        mov    eax, dword ptr [edi]
        cmp    dword ptr [esi + 0x50], edi
        jne    loc_1_2149
        mov    dword ptr [esi + 0x50], eax
loc_1_2149:
        or     eax, eax
        mov    dword ptr [esi + 0xa0], eax
        jne    loc_1_215A
        mov    dword ptr [esi + 0x60], 0x42d4 ; -> loc_1_42D4
loc_1_215A:
        mov    eax, dword ptr [edi + 0x18]
        or     eax, eax
        je     loc_1_2166
        mov    word ptr [eax], 0x420
loc_1_2166:
        push   dword ptr [edi + 0xc]
        push   0
        push   dword ptr [edi + 8]
        call   dword ptr [edi + 4]
        add    esp, 0xc
        or     ebx, ebx
        je     loc_1_2184
        push   edi
        call   sub_1_408C
        add    esp, 4
        jmp    loc_1_2186
        db  090h
loc_1_2184:
        mov    ebx, edi
loc_1_2186:
        mov    edi, dword ptr [esi + 0xa0]
        or     edi, edi
        jne    loc_1_2139
loc_1_2190:
        or     edi, edi
        jne    loc_1_22B0
        mov    byte ptr [esi + 0x83], 0
        cmp    dword ptr [esi + 0xac], 0
        je     loc_1_222C
        cmp    dword ptr [esi + 0xdc], 0
        je     loc_1_229C
        cmp    dword ptr [esi + 0x54], 0
        jne    loc_1_229C
        push   ebx
        push   esi
        call   sub_1_487C
        add    esp, 8
        push   esi
        call   sub_1_80C8
        add    esp, 4
        cmp    dword ptr [esi + 0xa0], 0
        jne    loc_1_2131
        cmp    dword ptr [esi + 0xac], 0
        jne    loc_1_22BD
        test   dword ptr [esi + 0x58], 0x1100000
        je     loc_1_22BD
loc_1_21FD:
        mov    eax, dword ptr [esi + 0x58]
        test   eax, 0x1000000
        je     loc_1_2274
        and    eax, 0xffffffbf
        mov    dword ptr [esi + 0x58], eax
        inc    dword ptr [esi + 0x74]
        push   dword ptr [esp + 0xc]
        push   1
        and    eax, 0x200000
        push   eax
        push   esi
        call   sub_1_70A4
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_222C:
        push   ebx
        call   sub_1_408C
        add    esp, 4
        test   dword ptr [esi + 0x58], 0x1100000
        jne    loc_1_21FD
        cmp    dword ptr [esi + 0xdc], 0
        je     loc_1_2260
        cmp    dword ptr [esi + 0x54], 0
        jne    loc_1_2260
        mov    dword ptr [esi + 0x60], 0x4620 ; -> loc_1_4620
        and    dword ptr [esi + 0x58], 0xffffffbf
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_2260:
        pop    edi
        mov    dword ptr [esi + 0x60], 0x42d4 ; -> loc_1_42D4
        and    dword ptr [esi + 0x58], 0xffffffbf
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_2274:
        test   eax, 0x100000
        je     loc_1_22C1
        and    eax, 0xffefffff
        mov    dword ptr [esi + 0x58], eax
        push   0x16
        push   0
        push   esi
        call   sub_1_6FB8
        add    esp, 0xc
        and    dword ptr [esi + 0x58], 0xffffffbf
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_229C:
        push   ebx
        call   sub_1_408C
        add    esp, 4
        and    dword ptr [esi + 0x58], 0xffffffbf
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_22B0:
        or     ebx, ebx
        je     loc_1_22BD
        push   ebx
        call   sub_1_408C
        add    esp, 4
loc_1_22BD:
        and    dword ptr [esi + 0x58], 0xffffffbf
loc_1_22C1:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_22C8:
        sub    esp, 4
        push   esi
        push   edi
        cmp    dword ptr [0x95dc], 0x95dc    ; -> loc_1_95DC
        je     loc_1_2382
        mov    edi, dword ptr [esp + 8]
loc_1_22E1:
        mov    esi, dword ptr [0x95dc]       ; -> loc_1_95DC
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x95dc], eax       ; -> loc_1_95DC
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], 0x95dc   ; -> loc_1_95DC
        sub    esi, 0xd0
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_2364
loc_1_2308:
        mov    eax, dword ptr [esi + 0x58]
        test   ah, 7
        je     loc_1_2364
        test   ah, 1
        je     loc_1_2326
        and    eax, 0xfffffeff
        mov    dword ptr [esi + 0x58], eax
        push   esi
        call   sub_1_20FC
        add    esp, 4
loc_1_2326:
        mov    eax, dword ptr [esi + 0x58]
        test   ah, 4
        je     loc_1_2342
        and    eax, 0xfffffbff
        mov    dword ptr [esi + 0x58], eax
        inc    dword ptr [esi + 0x74]
        push   esi
        call   sub_1_8824
        add    esp, 4
loc_1_2342:
        mov    eax, dword ptr [esi + 0x58]
        test   ah, 2
        je     loc_1_235B
        and    eax, 0xfffffdff
        mov    dword ptr [esi + 0x58], eax
        push   esi
        call   sub_1_80C8
        add    esp, 4
loc_1_235B:
        test   byte ptr [esi + 0x86], 1
        je     loc_1_2308
loc_1_2364:
        push   edi
        and    dword ptr [esi + 0x58], 0xffffffdf
        push   esi
        call   sub_1_63D8
        add    esp, 8
        cmp    dword ptr [0x95dc], 0x95dc    ; -> loc_1_95DC
        jne    loc_1_22E1
loc_1_2382:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
sub_1_2388:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 0x58]
        or     eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0x58], eax
        test   al, 0x20
        jne    loc_1_23C7
        or     eax, 0x20
        mov    dword ptr [edx + 0x58], eax
        inc    dword ptr [edx + 0x74]
        lea    ecx, [edx + 0xd0]
        mov    eax, dword ptr [0x95e0]       ; -> loc_1_95E0
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0x95e0]       ; -> loc_1_95E0
        mov    dword ptr [edx + 0xd4], eax
        mov    dword ptr [0x95e0], ecx       ; -> loc_1_95E0
        mov    dword ptr [ecx], 0x95dc       ; -> loc_1_95DC
loc_1_23C7:
        ret    
sub_1_23C8:
        push   esi
        test   byte ptr [0x9650], 3          ; -> loc_1_9650
        je     loc_1_24A7
        test   byte ptr [0x9650], 0x10       ; -> loc_1_9650
        jne    loc_1_24A7
        or     dword ptr [0x9650], 0x10      ; -> loc_1_9650
loc_1_23EA:
        test   byte ptr [0x9650], 1          ; -> loc_1_9650
        je     loc_1_2437
        cmp    dword ptr [0x9744], 0x9744    ; -> loc_1_9744
        je     loc_1_2430
        mov    esi, dword ptr [0x9744]       ; -> loc_1_9744
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x9744], eax       ; -> loc_1_9744
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], 0x9744   ; -> loc_1_9744
        push   0
        push   dword ptr [esi + 0x10]
        push   dword ptr [esi + 0xc]
        call   dword ptr [esi + 8]
        add    esp, 0xc
        push   esi
        call   sub_1_5EE8
        add    esp, 4
        jmp    loc_1_2437
loc_1_2430:
        and    dword ptr [0x9650], 0xfffffffe ; -> loc_1_9650
loc_1_2437:
        test   byte ptr [0x9650], 2          ; -> loc_1_9650
        je     loc_1_2493
        cmp    dword ptr [0x9610], 0x9610    ; -> loc_1_9610
        je     loc_1_248C
        mov    esi, dword ptr [0x9610]       ; -> loc_1_9610
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x9610], eax       ; -> loc_1_9610
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], 0x9610   ; -> loc_1_9610
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        mov    ecx, 0
        jne    loc_1_2473
        mov    ecx, dword ptr [esi + 0x14]
loc_1_2473:
        push   ecx
        push   eax
        push   dword ptr [esi + 0xc]
        call   dword ptr [esi + 8]
        add    esp, 0xc
        push   esi
        call   sub_1_7CD4
        add    esp, 4
        jmp    loc_1_2493
        db  08Dh, 049h, 000h
loc_1_248C:
        and    dword ptr [0x9650], 0xfffffffd ; -> loc_1_9650
loc_1_2493:
        test   byte ptr [0x9650], 3          ; -> loc_1_9650
        jne    loc_1_23EA
        and    dword ptr [0x9650], 0xffffffef ; -> loc_1_9650
loc_1_24A7:
        call   sub_1_22C8
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_24B0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0xb4]
        or     esi, esi
        je     loc_1_2509
        mov    dword ptr [eax + 0xb4], 0
        mov    cx, word ptr [eax + 0x6c]
        push   ecx
        push   dword ptr [eax + 0x64]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esi + 0x14]
        call   sub_1_1F9C
        add    esp, 0x10
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x10], eax
        or     dword ptr [0x9650], 1         ; -> loc_1_9650
        mov    eax, dword ptr [0x9748]       ; -> loc_1_9748
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [0x9748]       ; -> loc_1_9748
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [0x9748], esi       ; -> loc_1_9748
        mov    dword ptr [esi], 0x9744       ; -> loc_1_9744
loc_1_2509:
        pop    esi
        ret    
        db  090h
sub_1_250C:
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 0x4c]
        test   byte ptr [edi + 0x32], 0x80
        jne    loc_1_252C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Dh, 049h, 000h
loc_1_252C:
        mov    edx, dword ptr [edi + 0x4c]
        or     edx, edx
        mov    si, word ptr [esp + 0x58]
        je     loc_1_257C
loc_1_2538:
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     loc_1_256B
        cmp    byte ptr [eax + 0x84], 1
        jne    loc_1_256B
        test   byte ptr [eax + 0x86], 1
        jne    loc_1_256B
        mov    ecx, dword ptr [eax + 0x64]
        or     ecx, ecx
        je     loc_1_255D
        cmp    dword ptr [esp + 0x54], ecx
        jne    loc_1_256B
loc_1_255D:
        mov    cx, word ptr [eax + 0x6c]
        or     cx, cx
        je     loc_1_2574
        cmp    si, cx
        je     loc_1_2574
loc_1_256B:
        mov    edx, dword ptr [edx + 8]
        or     edx, edx
        jne    loc_1_2538
        jmp    loc_1_2580
loc_1_2574:
        mov    ebx, 1
        jmp    loc_1_2580
        db  090h
loc_1_257C:
        mov    eax, dword ptr [esp + 0x1c]
loc_1_2580:
        or     ebx, ebx
        je     loc_1_25A4
        inc    dword ptr [eax + 0x74]
        mov    ecx, dword ptr [eax + 0xb4]
        test   byte ptr [ecx + 0x18], 1
        jne    loc_1_2710
        or     dword ptr [eax + 0x58], 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  090h
loc_1_25A4:
        cmp    dword ptr [edi + 0x64], 0
        je     loc_1_270E
        call   sub_1_5D70
        mov    ebx, eax
        call   sub_1_5EFC
        mov    ebp, eax
        or     ebx, ebx
        je     loc_1_26F4
        or     ebp, ebp
        je     loc_1_2701
        mov    eax, dword ptr [edi + 0x64]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [edi + 0x68]
        mov    dword ptr [esp + 0x18], eax
        push   esi
        push   dword ptr [esp + 0x58]
        lea    eax, [esp + 0x38]
        push   eax
        call   sub_1_1F70
        add    esp, 0xc
        inc    word ptr [edi + 0x38]
        mov    byte ptr [ebx + 0x84], 1
        mov    dword ptr [ebx + 0xb4], ebp
        or     dword ptr [ebx + 0x58], 4
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    esi, dword ptr [esp + 0x64]
        push   dword ptr [esi]
        movzx  eax, byte ptr [esi + 8]
        push   eax
        push   0
        push   0
        lea    eax, [esp + 0x48]
        push   eax
        push   0x16
        push   dword ptr [esp + 0x38]
        call   dword ptr [esp + 0x38]
        add    esp, 0x24
        cmp    eax, 0x18
        jne    loc_1_26CC
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [ebp + 0xc], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [ebp + 0x14], eax
        mov    ebp, dword ptr [edi + 0x4c]
        mov    eax, 0xf
        or     ebp, ebp
        je     loc_1_26A1
loc_1_2656:
        mov    ecx, dword ptr [esp + 0x10]
        cmp    dword ptr [ebp + 0x10], ecx
        jne    loc_1_2665
        test   byte ptr [ebp + 0xd], 3
        je     loc_1_2670
loc_1_2665:
        mov    ebp, dword ptr [ebp + 8]
        or     ebp, ebp
        jne    loc_1_2656
        jmp    loc_1_269D
        db  08Bh, 0FFh
loc_1_2670:
        mov    dword ptr [ebx + 0x74], 1
        push   dword ptr [esp + 0x28]
        push   ebx
        push   ebp
        call   sub_1_66C8
        add    esp, 0xc
        or     eax, eax
        je     loc_1_2690
        sub    ebp, ebp
        jmp    loc_1_269D
        db  08Dh, 049h, 000h
loc_1_2690:
        mov    dword ptr [ebp], ebx
        mov    dword ptr [ebx + 0xcc], ebp
        inc    word ptr [ebp + 0xe]
loc_1_269D:
        or     ebp, ebp
        jne    loc_1_26B7
loc_1_26A1:
        push   eax
        push   esi
        push   ebx
        call   sub_1_24B0
        add    esp, 0xc
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        sub    ebx, ebx
loc_1_26B7:
        push   edi
        call   sub_1_120C
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Bh, 0FFh
loc_1_26CC:
        push   ebp
        call   sub_1_5EE8
        add    esp, 4
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        push   edi
        call   sub_1_120C
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        db  08Dh, 049h, 000h
loc_1_26F4:
        or     ebp, ebp
        je     loc_1_2701
        push   ebp
        call   sub_1_5EE8
        add    esp, 4
loc_1_2701:
        or     ebx, ebx
        je     loc_1_270E
        push   ebx
        call   sub_1_5E04
        add    esp, 4
loc_1_270E:
        sub    eax, eax
loc_1_2710:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
sub_1_2718:
        mov    edx, dword ptr [esp + 4]
        mov    al, byte ptr [edx + 0xc]
        and    al, 0xf3
        shr    al, 2
        movzx  ecx, al
        sub    ecx, 0x14
        add    edx, 0x14
        or     ecx, ecx
        je     loc_1_275C
loc_1_2731:
        mov    al, byte ptr [edx]
        or     al, al
        je     loc_1_275C
        cmp    al, 1
        jne    loc_1_2740
        inc    edx
        dec    ecx
        jmp    loc_1_2758
        db  090h
loc_1_2740:
        cmp    al, 2
        je     loc_1_2764
        mov    al, byte ptr [edx + 1]
        or     al, al
        je     loc_1_275C
        and    eax, 0xff
        cmp    ecx, eax
        jb     loc_1_275C
        sub    ecx, eax
        add    edx, eax
loc_1_2758:
        or     ecx, ecx
        jne    loc_1_2731
loc_1_275C:
        mov    ax, 0x218
        ret    
        db  08Dh, 049h, 000h
loc_1_2764:
        cmp    byte ptr [edx + 1], 4
        jne    loc_1_275C
        mov    cx, word ptr [edx + 2]
        or     cx, cx
        je     loc_1_275C
        mov    ah, cl
        mov    al, ch
        ret    
sub_1_2778:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        test   byte ptr [eax + 0x11], 4
        mov    esi, dword ptr [esp + 0x10]
        jne    loc_1_27A5
        cmp    byte ptr [esi + 0x84], 0xa
        jne    loc_1_279C
        mov    word ptr [esi + 0x90], 0x960
loc_1_279C:
        push   esi
        call   sub_1_80C8
        add    esp, 4
loc_1_27A5:
        push   dword ptr [esp + 4]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    esi
        add    esp, 4
        ret    
        db  090h
sub_1_27B8:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x24]
        mov    ebx, dword ptr [esp + 0x28]
        sub    ebx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [esp + 0x14], eax
        cmp    ebx, eax
        jne    loc_1_28A4
        cmp    byte ptr [esi + 0x84], 4
        jne    loc_1_28A4
        lea    edx, [esi + 0x38]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [edx], edx
        mov    dword ptr [esi + 0x3c], edx
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0x28], 0
        mov    dword ptr [esi + 0x2c], 0
        mov    dword ptr [esi + 0x30], 0
        mov    dword ptr [esi + 0x34], 0
        mov    dword ptr [esi + 0x14], 0
loc_1_281D:
        mov    edi, ecx
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edi + 0x10], 0
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        or     eax, eax
        jg     loc_1_2891
        mov    eax, dword ptr [edi + 0x2c]
        or     eax, eax
        je     loc_1_2847
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 0x2c], 0
loc_1_2847:
        mov    eax, dword ptr [0x966c]       ; -> loc_1_966C
        sub    eax, dword ptr [edi + 0x30]
        add    dword ptr [esi + 0xc4], eax
        mov    ebx, dword ptr [esi + 0xbc]
        mov    eax, dword ptr [edi + 0x14]
        add    eax, ebx
        mov    dword ptr [esi + 0xbc], eax
        cmp    ebx, eax
        mov    eax, 0
        seta   al
        add    dword ptr [esi + 0xc0], eax
        mov    eax, dword ptr [0x9614]       ; -> loc_1_9614
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [0x9614]       ; -> loc_1_9614
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [0x9614], edi       ; -> loc_1_9614
        mov    dword ptr [edi], 0x9610       ; -> loc_1_9610
loc_1_2891:
        cmp    ecx, edx
        jne    loc_1_281D
        or     dword ptr [0x9650], 2         ; -> loc_1_9650
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_28A4:
        mov    ecx, dword ptr [esi + 0x38]
        mov    dword ptr [esp + 0x18], ebx
        mov    edx, ecx
        mov    dword ptr [esp + 0x10], 0
        or     ebx, ebx
        mov    edi, dword ptr [esp + 0x1c]
        jle    loc_1_290C
loc_1_28BD:
        lea    eax, [esi + 0x38]
        cmp    edx, eax
        je     loc_1_290C
        mov    edi, edx
        mov    eax, dword ptr [edx + 0x20]
        cmp    ebx, eax
        jl     loc_1_28DC
        sub    ebx, eax
        mov    dword ptr [esp + 0x10], edx
        mov    edx, dword ptr [edx]
        or     ebx, ebx
        jg     loc_1_28BD
        jmp    loc_1_290C
        db  090h
loc_1_28DC:
        mov    ebp, dword ptr [edi + 0x28]
        sub    dword ptr [edi + 0x20], ebx
        mov    eax, dword ptr [ebp + 0xc]
        sub    eax, dword ptr [edi + 0x24]
        cmp    ebx, eax
        jge    loc_1_28F8
        add    ebx, dword ptr [edi + 0x24]
        mov    dword ptr [edi + 0x24], ebx
        sub    ebx, ebx
        jmp    loc_1_290C
        db  08Bh, 0FFh
loc_1_28F8:
        sub    ebx, eax
        mov    ebp, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    ebx, eax
        jge    loc_1_28F8
        mov    dword ptr [edi + 0x24], ebx
        mov    dword ptr [edi + 0x28], ebp
        sub    ebx, ebx
loc_1_290C:
        mov    eax, dword ptr [esp + 0x28]
        sub    eax, dword ptr [esi + 8]
        or     eax, eax
        jle    loc_1_295B
        lea    eax, [esi + 0x38]
        cmp    edx, eax
        je     loc_1_2938
        mov    edi, edx
        mov    dword ptr [esi + 0x28], edx
        mov    eax, dword ptr [edx + 0x28]
        mov    dword ptr [esi + 0x2c], eax
        mov    eax, dword ptr [edx + 0x24]
        mov    dword ptr [esi + 0x30], eax
        mov    eax, dword ptr [edx + 0x20]
        mov    dword ptr [esi + 0x34], eax
        jmp    loc_1_2954
        db  090h
loc_1_2938:
        mov    dword ptr [esi + 0x28], 0
        mov    dword ptr [esi + 0x2c], 0
        mov    dword ptr [esi + 0x30], 0
        mov    dword ptr [esi + 0x34], 0
loc_1_2954:
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esi + 8], eax
loc_1_295B:
        sub    ebx, dword ptr [esp + 0x18]
        add    ebx, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0x14], ebx
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [esi + 4], eax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_2A14
        lea    eax, [esi + 0x38]
        cmp    edx, eax
        jne    loc_1_2988
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x3c], eax
        jmp    loc_1_2991
        db  08Bh, 0FFh
loc_1_2988:
        mov    dword ptr [esi + 0x38], edx
        lea    eax, [esi + 0x38]
        mov    dword ptr [edx + 4], eax
loc_1_2991:
        cmp    edx, ecx
        je     loc_1_2A0D
loc_1_2995:
        mov    ebx, dword ptr [ecx]
        mov    dword ptr [ecx + 0x10], 0
        mov    eax, dword ptr [ecx + 0x18]
        dec    eax
        mov    dword ptr [ecx + 0x18], eax
        or     eax, eax
        jg     loc_1_2A07
        mov    eax, dword ptr [ecx + 0x2c]
        or     eax, eax
        je     loc_1_29BD
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx + 0x2c], 0
loc_1_29BD:
        mov    eax, dword ptr [0x966c]       ; -> loc_1_966C
        sub    eax, dword ptr [edi + 0x30]
        add    dword ptr [esi + 0xc4], eax
        mov    ebp, dword ptr [esi + 0xbc]
        mov    eax, dword ptr [edi + 0x14]
        add    eax, ebp
        mov    dword ptr [esi + 0xbc], eax
        cmp    ebp, eax
        mov    eax, 0
        seta   al
        add    dword ptr [esi + 0xc0], eax
        mov    eax, dword ptr [0x9614]       ; -> loc_1_9614
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0x9614]       ; -> loc_1_9614
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [0x9614], ecx       ; -> loc_1_9614
        mov    dword ptr [ecx], 0x9610       ; -> loc_1_9610
loc_1_2A07:
        mov    ecx, ebx
        cmp    ebx, edx
        jne    loc_1_2995
loc_1_2A0D:
        or     dword ptr [0x9650], 2         ; -> loc_1_9650
loc_1_2A14:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_2A1C:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        or     edi, edi
        mov    eax, dword ptr [esp + 0xc]
        je     loc_1_2A54
loc_1_2A2A:
        mov    ecx, edi
        mov    esi, dword ptr [eax + 0xc]
        sub    ecx, esi
        sbb    edx, edx
        and    ecx, edx
        add    ecx, esi
        sub    edi, ecx
        add    dword ptr [eax + 8], ecx
        sub    esi, ecx
        mov    dword ptr [eax + 0xc], esi
        jne    loc_1_2A4B
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_2A54
        mov    eax, ecx
loc_1_2A4B:
        or     edi, edi
        jne    loc_1_2A2A
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_2A54:
        pop    edi
        pop    esi
        ret    
        db  090h
sub_1_2A58:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     loc_1_2A82
loc_1_2A61:
        cmp    dword ptr [eax + 4], 3
        jne    loc_1_2A7C
        mov    esi, dword ptr [eax]
        push   eax
        call   sub_1_90FC
        add    esp, 4
        mov    eax, esi
        or     eax, eax
        jne    loc_1_2A61
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_2A7C:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_2A61
loc_1_2A82:
        pop    esi
        ret    
sub_1_2A84:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi + 0x9c]
        mov    ebx, dword ptr [esi + 0x40]
loc_1_2A95:
        or     edi, edi
        je     loc_1_2AF5
        mov    ecx, dword ptr [edi + 4]
        mov    eax, ebx
        sub    eax, ecx
        js     loc_1_2AFC
        mov    eax, dword ptr [edi + 0xc]
        and    eax, 0x100
        or     eax, eax
        mov    eax, 0
        setne  al
        add    eax, dword ptr [edi + 8]
        add    eax, ecx
        mov    ecx, ebx
        sub    ecx, eax
        js     loc_1_2B04
        push   dword ptr [edi + 0x14]
        call   sub_1_2A58
        add    esp, 4
        mov    ebp, dword ptr [edi]
        push   edi
        call   sub_1_90FC
        add    esp, 4
        mov    edi, ebp
        mov    dword ptr [esi + 0x9c], ebp
        or     ebp, ebp
        jne    loc_1_2A95
        mov    al, byte ptr [esi + 0x82]
        dec    al
        mov    byte ptr [esi + 0x82], al
        jne    loc_1_2AF5
        and    dword ptr [esi + 0x5c], 0xfffffffe
loc_1_2AF5:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_2AFC:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_2B04:
        mov    ebp, ebx
        sub    ebp, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [ecx], ebx
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [ecx + 0xc], eax
        cmp    dword ptr [edi + 8], ebp
        je     loc_1_2B3C
        push   ebp
        push   dword ptr [edi + 0x14]
        call   sub_1_40F0
        add    esp, 8
        mov    ebx, eax
        mov    ecx, dword ptr [edi + 8]
        sub    ecx, ebp
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [eax], ecx
        jmp    loc_1_2B56
        db  090h
loc_1_2B3C:
        push   dword ptr [edi + 0x14]
        call   sub_1_2A58
        add    esp, 4
        mov    ebx, 0x9674                   ; -> loc_1_9674
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [eax], 0
loc_1_2B56:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x9c], eax
        or     eax, eax
        jne    loc_1_2B76
        mov    al, byte ptr [esi + 0x82]
        dec    al
        mov    byte ptr [esi + 0x82], al
        jne    loc_1_2B76
        and    dword ptr [esi + 0x5c], 0xfffffffe
loc_1_2B76:
        push   edi
        call   sub_1_90FC
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_2B88:
        push   ebx
        push   esi
        push   edi
        push   0x1c
        call   sub_1_90F6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_2BA4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_2BA4:
        mov    ebx, dword ptr [esp + 0x1c]
        lea    eax, [ebx + 0x10]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_2BCC
        push   esi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_2BCC:
        mov    dword ptr [edi + 0xc], ebx
        mov    dword ptr [edi + 4], 3
        mov    dword ptr [edi], 0
        lea    eax, [edi + 0x10]
        mov    dword ptr [edi + 8], eax
        or     ebx, ebx
        je     loc_1_2BF6
        push   0
        push   ebx
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_1E94
        add    esp, 0x10
loc_1_2BF6:
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 8], ebx
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esi + 0x10], eax
        mov    dword ptr [esi + 0x14], edi
        mov    dword ptr [esi + 0x18], edi
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    dword ptr [eax], esi
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_2C28:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x28]
        mov    eax, dword ptr [eax + 0x10]
        and    eax, 0x100
        or     eax, eax
        mov    ecx, 0
        setne  cl
        mov    eax, dword ptr [esp + 0x28]
        add    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 0x30]
        add    eax, ecx
        mov    dword ptr [esp + 0x1c], eax
        mov    ebx, dword ptr [esp + 0x24]
        add    ebx, 0x9c
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_2DC8
loc_1_2C69:
        mov    edx, dword ptr [esi + 0xc]
        and    edx, 0x100
        lea    ecx, [esi + 4]
        mov    dword ptr [esp + 0x14], ecx
        mov    ebp, dword ptr [ecx]
        lea    eax, [esi + 8]
        mov    dword ptr [esp + 0x18], eax
        cmp    edx, 1
        sbb    ecx, ecx
        inc    ecx
        add    ecx, dword ptr [eax]
        lea    edi, [ecx + ebp]
        mov    eax, dword ptr [esp + 0x28]
        mov    ecx, dword ptr [eax]
        mov    eax, ecx
        sub    eax, edi
        or     eax, eax
        jg     loc_1_2DB8
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, ebp
        js     loc_1_2DC8
        cmp    dword ptr [esp + 0x30], 0
        mov    eax, dword ptr [esp + 0x28]
        je     loc_1_2EA0
        mov    eax, dword ptr [eax + 0x10]
        test   ah, 1
        je     loc_1_2CD0
        mov    edx, dword ptr [esp + 0x1c]
        sub    edx, edi
        or     edx, edx
        jle    loc_1_2DFD
loc_1_2CD0:
        sub    ebp, ecx
        mov    dword ptr [esp + 0x10], ebp
        or     ebp, ebp
        jle    loc_1_2D60
        test   ah, 0x20
        jne    loc_1_2D10
        test   byte ptr [esi + 0xd], 0x20
        je     loc_1_2D10
        push   ebp
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   ebx
        call   sub_1_2B88
        add    esp, 0x10
        or     eax, eax
        je     loc_1_2D03
        mov    eax, dword ptr [ebx]
        mov    esi, dword ptr [eax]
loc_1_2D03:
        mov    dword ptr [esp + 0x10], 0
        jmp    loc_1_2D60
        db  08Dh, 049h, 000h
loc_1_2D10:
        lea    eax, [ebp + 0x10]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_2DFD
        mov    dword ptr [ebx + 4], 3
        mov    dword ptr [ebx + 0xc], ebp
        lea    eax, [ebx + 0x10]
        mov    dword ptr [ebx + 8], eax
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        push   eax
        call   sub_1_1E94
        add    esp, 0x10
        mov    eax, dword ptr [esp + 0x18]
        add    dword ptr [eax], ebp
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [ebx], eax
        mov    dword ptr [esi + 0x14], ebx
        mov    eax, dword ptr [esp + 0x28]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], eax
loc_1_2D60:
        mov    edx, dword ptr [esi + 0xc]
        test   dh, 1
        jne    loc_1_2E08
        mov    eax, dword ptr [esp + 0x28]
        mov    ecx, dword ptr [eax]
        mov    eax, ecx
        sub    eax, edi
        mov    ebp, dword ptr [esp + 0x30]
        add    ebp, eax
        or     ebp, ebp
        jle    loc_1_2E54
        mov    eax, dword ptr [esp + 0x28]
        test   byte ptr [eax + 0x11], 0x20
        je     loc_1_2E0A
        test   dh, 0x20
        jne    loc_1_2E0A
        cmp    dword ptr [esp + 0x10], 0
        jg     loc_1_2E0A
        sub    edi, ecx
        push   edi
        push   dword ptr [esp + 0x30]
        call   sub_1_40F0
        add    esp, 8
        mov    dword ptr [esp + 0x2c], eax
        mov    eax, dword ptr [esp + 0x28]
        add    dword ptr [eax], edi
        sub    dword ptr [eax + 0xc], edi
        jmp    loc_1_2DBC
loc_1_2DB8:
        or     edx, edx
        jne    loc_1_2DFD
loc_1_2DBC:
        mov    ebx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_2C69
loc_1_2DC8:
        mov    eax, dword ptr [esp + 0x24]
        mov    esi, dword ptr [eax + 0x9c]
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        push   ebx
        call   sub_1_2B88
        add    esp, 0x10
        or     eax, eax
        je     loc_1_2DFD
        or     esi, esi
        jne    loc_1_2DFD
        mov    eax, dword ptr [esp + 0x24]
        inc    byte ptr [eax + 0x82]
        or     dword ptr [eax + 0x5c], 1
loc_1_2DFD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_2E08:
        sub    ebp, ebp
loc_1_2E0A:
        or     ebp, ebp
        jle    loc_1_2E54
        lea    eax, [ebp + 0x10]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_2E54
        mov    dword ptr [ebx + 4], 3
        mov    dword ptr [ebx + 0xc], ebp
        lea    ecx, [ebx + 0x10]
        mov    dword ptr [ebx + 8], ecx
        mov    eax, dword ptr [esp + 0x28]
        sub    edi, dword ptr [eax]
        push   edi
        push   ebp
        push   dword ptr [esp + 0x34]
        push   ecx
        call   sub_1_1E94
        add    esp, 0x10
        add    dword ptr [esi + 8], ebp
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0x18], ebx
loc_1_2E54:
        mov    eax, dword ptr [esp + 0x28]
        test   byte ptr [eax + 0x11], 0x20
        je     loc_1_2DFD
        mov    edx, dword ptr [eax + 0xc]
        add    edx, dword ptr [eax]
        mov    eax, dword ptr [esi + 0xc]
        test   ah, 0x20
        je     loc_1_2E88
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 0x10]
        mov    ecx, edx
        sub    ecx, eax
        jns    loc_1_2E90
        mov    edx, eax
        sub    edx, dword ptr [esi + 4]
        mov    dword ptr [esi + 0x10], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_2E88:
        or     eax, 0x2000
        mov    dword ptr [esi + 0xc], eax
loc_1_2E90:
        pop    ebp
        sub    edx, dword ptr [esi + 4]
        mov    dword ptr [esi + 0x10], edx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_1_2EA0:
        test   byte ptr [eax + 0x11], 1
        je     loc_1_2DFD
        mov    ecx, dword ptr [esi + 0xc]
        test   ch, 1
        jne    loc_1_2DFD
        cmp    dword ptr [eax], edi
        jne    loc_1_2DFD
        or     ecx, 0x100
        mov    dword ptr [esi + 0xc], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  090h
loc_1_2ED0:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        inc    dword ptr [0x9564]            ; -> obj1:0x9564
        mov    ebx, dword ptr [esp + 0x40]
        mov    edi, dword ptr [ebx + 8]
        cmp    byte ptr [esp + 0x48], 0
        jne    loc_1_407C
        cmp    dword ptr [esp + 0x44], 0x14
        jb     loc_1_406C
        mov    ax, word ptr [esp + 0x44]
        push   ebx
        mov    ch, al
        mov    cl, ah
        and    ecx, 0xffff
        movzx  eax, word ptr [esp + 0x3e]
        add    ecx, eax
        movzx  eax, word ptr [esp + 0x3c]
        add    ecx, eax
        movzx  eax, word ptr [esp + 0x3a]
        add    ecx, eax
        movzx  eax, word ptr [esp + 0x38]
        add    ecx, eax
        add    ecx, 0x600
        push   ecx
        call   sub_1_1D9C
        add    esp, 8
        cmp    ax, 0xffff
        jne    loc_1_406C
        mov    ecx, dword ptr [edi + 4]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [esp + 0x18], edx
        mov    ecx, dword ptr [edi + 8]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [esp + 0x1c], edx
        mov    ax, word ptr [edi + 0xe]
        mov    cx, ax
        and    ecx, 0xff
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     ecx, eax
        mov    dword ptr [esp + 0x20], ecx
        mov    ax, word ptr [edi + 0x12]
        mov    cx, ax
        and    ecx, 0xff
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     ecx, eax
        mov    dword ptr [esp + 0x24], ecx
        movzx  eax, word ptr [edi + 0xc]
        mov    dword ptr [esp + 0x28], eax
        mov    al, byte ptr [edi + 0xc]
        and    al, 0xf3
        shr    al, 2
        and    eax, 0xff
        cmp    eax, dword ptr [esp + 0x44]
        ja     loc_1_405C
        sub    dword ptr [esp + 0x44], eax
        sub    dword ptr [ebx + 0xc], eax
        add    dword ptr [ebx + 8], eax
        mov    ax, word ptr [edi + 2]
        push   eax
        mov    ax, word ptr [edi]
        push   eax
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x40]
        call   sub_1_5B50
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        je     loc_1_3E8C
        mov    eax, dword ptr [0x966c]       ; -> loc_1_966C
        mov    dword ptr [esi + 0x98], eax
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [esi + 0x40], eax
        jne    loc_1_3274
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 0x3700
        cmp    eax, dword ptr [esi + 0x5c]
        jne    loc_1_3274
        sub    edi, edi
        inc    dword ptr [esi + 0x74]
        mov    ecx, dword ptr [esi + 4]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 0x1c]
        jns    loc_1_3150
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [esi + 0xc]
        cmp    eax, edi
        jg     loc_1_3150
        push   dword ptr [esp + 0x1c]
        push   esi
        call   sub_1_27B8
        add    esp, 8
        mov    eax, dword ptr [esi + 0x8c]
        cmp    eax, edi
        je     loc_1_30F8
        mov    ecx, dword ptr [esp + 0x1c]
        sub    ecx, dword ptr [esi + 0x78]
        cmp    ecx, edi
        jle    loc_1_30F8
        mov    edx, dword ptr [0x966c]       ; -> loc_1_966C
        sub    dx, ax
        mov    dword ptr [esi + 0x8c], edi
        mov    ax, word ptr [esi + 0x7c]
        mov    cx, ax
        shr    cx, 3
        sub    dx, cx
        lea    edi, [eax + edx]
        mov    word ptr [esi + 0x7c], di
        movsx  eax, dx
        cdq    
        xor    eax, edx
        sub    eax, edx
        mov    cx, word ptr [esi + 0x7e]
        mov    dx, cx
        shr    dx, 3
        sub    ax, dx
        add    ax, cx
        mov    word ptr [esi + 0x7e], ax
        shr    di, 2
        movzx  ecx, di
        and    eax, 0xffff
        add    ecx, eax
        sar    ecx, 1
        cmp    ecx, 3
        jg     loc_1_30E7
        mov    ecx, 3
loc_1_30E7:
        cmp    ecx, 0x960
        jl     loc_1_30F4
        mov    ecx, 0x960
loc_1_30F4:
        mov    word ptr [esi + 0x72], cx
loc_1_30F8:
        mov    edi, dword ptr [esi + 0x1c]
        movzx  eax, word ptr [esi + 0x70]
        mov    ecx, dword ptr [esi + 0x18]
        cmp    edi, ecx
        jae    loc_1_311C
        cmp    dword ptr [esi + 0x20], edi
        jbe    loc_1_3110
        jmp    loc_1_3117
        db  08Dh, 049h, 000h
loc_1_3110:
        imul   eax, eax
        sub    edx, edx
        div    edi
loc_1_3117:
        add    edi, eax
        mov    dword ptr [esi + 0x1c], edi
loc_1_311C:
        mov    byte ptr [esi + 0x85], 0
        mov    eax, dword ptr [esp + 0x1c]
        cmp    dword ptr [esi + 0xc], eax
        jne    loc_1_3138
        mov    word ptr [esi + 0x90], 0
        jmp    loc_1_3143
        db  090h
loc_1_3138:
        mov    ax, word ptr [esi + 0x72]
        mov    word ptr [esi + 0x90], ax
loc_1_3143:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [esp + 0x20]
        jmp    loc_1_31A4
loc_1_3150:
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [esi + 0xc]
        or     eax, eax
        jle    loc_1_3174
        push   esi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_3174:
        cmp    dword ptr [esp + 0x1c], ecx
        jne    loc_1_31CC
        mov    eax, dword ptr [esi + 0x48]
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 0x18]
        js     loc_1_3196
        cmp    dword ptr [esp + 0x18], eax
        jne    loc_1_31CC
        mov    eax, dword ptr [esi + 0x4c]
        sub    eax, dword ptr [esp + 0x1c]
        or     eax, eax
        jg     loc_1_31CC
loc_1_3196:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esp + 0x20]
loc_1_31A4:
        sub    eax, ecx
        sbb    edx, edx
        not    edx
        and    eax, edx
        add    eax, ecx
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x48], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0x4c], eax
        cmp    dword ptr [esi + 0x14], 1
        cmc    
        sbb    edi, edi
        and    edi, 0x400
loc_1_31CC:
        mov    ebp, dword ptr [esi + 0x44]
        mov    eax, dword ptr [esp + 0x44]
        cmp    ebp, eax
        jl     loc_1_31D9
        mov    ebp, eax
loc_1_31D9:
        or     ebp, ebp
        je     loc_1_3240
        or     dword ptr [esi + 0x5c], 2
        push   ebp
        push   ebx
        push   dword ptr [esp + 0x30]
        push   esi
        call   dword ptr [esi + 0x60]
        add    esp, 0x10
        add    dword ptr [esi + 0x40], eax
        sub    dword ptr [esi + 0x44], eax
        and    dword ptr [esi + 0x5c], 0xfffffffd
        mov    edx, dword ptr [esi + 0x58]
        mov    ecx, edx
        and    ecx, 0x800000
        cmp    ecx, 1
        cmc    
        sbb    ecx, ecx
        and    ecx, 0x400
        or     edi, ecx
        and    edx, 0xff7fffff
        mov    dword ptr [esi + 0x58], edx
        test   dh, 8
        jne    loc_1_3238
        cmp    ebp, eax
        jne    loc_1_3238
        or     edx, 0x800
        mov    dword ptr [esi + 0x58], edx
        mov    word ptr [esi + 0x92], 2
        jmp    loc_1_324F
        db  090h
loc_1_3238:
        or     edi, 0x200
        jmp    loc_1_324F
loc_1_3240:
        cmp    dword ptr [esp + 0x44], 1
        cmc    
        sbb    eax, eax
        and    eax, 0x200
        or     edi, eax
loc_1_324F:
        or     edi, edi
        je     loc_1_325D
        push   edi
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_325D:
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_3274:
        mov    al, byte ptr [esi + 0x84]
        cmp    al, 3
        jne    loc_1_3284
        test   byte ptr [esi + 0x58], 4
        je     loc_1_328E
loc_1_3284:
        cmp    al, 4
        je     loc_1_3298
        test   byte ptr [esi + 0x5c], 2
        je     loc_1_3298
loc_1_328E:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_3298:
        or     al, al
        je     loc_1_3E68
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_3E68
        mov    ecx, dword ptr [esi + 0x58]
        and    ecx, 0x1200000
        cmp    ecx, 0x1000000
        je     loc_1_3E68
        cmp    al, 2
        jne    loc_1_34E0
        test   byte ptr [esp + 0x29], 0x10
        je     loc_1_3300
        mov    eax, dword ptr [esp + 0x1c]
        cmp    dword ptr [esi + 8], eax
        je     loc_1_3300
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x50]
        push   edi
        call   sub_1_82D8
        add    esp, 0x14
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_3300:
        test   byte ptr [esp + 0x29], 4
        je     loc_1_3354
        movzx  eax, byte ptr [esi + 0x85]
        cmp    eax, dword ptr [0x964c]       ; -> loc_1_964C
        jne    loc_1_3340
        push   0xe
        push   dword ptr [esp + 0x54]
        push   esi
        call   sub_1_24B0
        add    esp, 0xc
        push   dword ptr [esp + 0x10]
        push   0x10
        push   esi
        call   sub_1_6398
        add    esp, 0xc
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_3340:
        sub    eax, eax
        mov    word ptr [esi + 0x90], 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_3354:
        test   byte ptr [esp + 0x29], 2
        je     loc_1_34D4
        inc    dword ptr [esi + 0x74]
        inc    dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x40], eax
        test   byte ptr [esp + 0x29], 0x20
        je     loc_1_338C
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_3384
        dec    dword ptr [esp + 0x24]
        jmp    loc_1_338C
        db  08Dh, 049h, 000h
loc_1_3384:
        and    dword ptr [esp + 0x28], 0xffffdfff
loc_1_338C:
        push   edi
        call   sub_1_2718
        add    esp, 4
        mov    word ptr [esi + 0x80], ax
        mov    eax, dword ptr [esp + 0x50]
        cmp    dword ptr [eax], 0
        je     loc_1_33E0
        test   byte ptr [esi + 0x58], 2
        jne    loc_1_33FB
        push   0
        push   dword ptr [esp + 0x3c]
        lea    eax, [esi + 0xe4]
        push   eax
        push   dword ptr [esp + 0x5c]
        call   sub_1_95BC
        add    esp, 0x10
        push   dword ptr [esi + 0xf0]
        call   sub_1_95A4
        add    esp, 4
        push   esi
        call   sub_1_6BE4
        add    esp, 4
        jmp    loc_1_33FB
        db  090h
loc_1_33E0:
        mov    ax, word ptr [esi + 0x70]
        mov    cx, word ptr [esi + 0x80]
        sub    ax, cx
        sbb    dx, dx
        and    ax, dx
        add    ax, cx
        mov    word ptr [esi + 0x70], ax
loc_1_33FB:
        push   esi
        mov    byte ptr [esi + 0x85], 0
        mov    word ptr [esi + 0x90], 0
        call   sub_1_2024
        add    esp, 4
        test   byte ptr [esp + 0x29], 0x10
        je     loc_1_34A0
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x48], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0x4c], eax
        push   esi
        call   sub_1_845C
        add    esp, 4
        push   0
        push   dword ptr [esp + 0x54]
        push   esi
        call   sub_1_24B0
        add    esp, 0xc
        push   esi
        call   sub_1_80C8
        add    esp, 4
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_3487
        and    dword ptr [esp + 0x28], 0xfffffdff
        mov    eax, dword ptr [esp + 0x28]
        and    ah, 0x37
        cmp    ah, 0x10
        jne    loc_1_34EA
        cmp    dword ptr [esp + 0x44], 0
        jne    loc_1_34EA
loc_1_3487:
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_34A0:
        mov    eax, dword ptr [esi + 4]
        mov    byte ptr [esi + 0x84], 3
        mov    dword ptr [esi + 8], eax
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_7E10
        add    esp, 8
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_34D4:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_34E0:
        lea    eax, [esi + 0x74]
        mov    dword ptr [esp + 0x14], eax
        inc    dword ptr [esi + 0x74]
loc_1_34EA:
        cmp    dword ptr [esi + 0x44], 0
        je     loc_1_3664
        mov    eax, dword ptr [esp + 0x18]
        sub    eax, dword ptr [esi + 0x40]
        jns    loc_1_35AB
        test   byte ptr [esp + 0x29], 2
        je     loc_1_3518
        and    dword ptr [esp + 0x28], 0xfffffdff
        inc    dword ptr [esp + 0x18]
        dec    dword ptr [esp + 0x24]
loc_1_3518:
        mov    ecx, dword ptr [esi + 0x40]
        sub    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 0x100
        or     eax, eax
        mov    eax, 0
        setne  al
        mov    edx, dword ptr [esp + 0x44]
        add    edx, eax
        cmp    edx, ecx
        jg     loc_1_357C
        test   byte ptr [esp + 0x29], 0x10
        je     loc_1_3630
        mov    eax, dword ptr [esi + 4]
        sub    eax, dword ptr [esp + 0x1c]
        jns    loc_1_3630
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [esi + 0xc]
        or     eax, eax
        jg     loc_1_3630
        mov    dword ptr [esp + 0x44], 0
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x40]
        mov    dword ptr [esp + 0x18], eax
        and    dword ptr [esp + 0x28], 0xffffd8ff
loc_1_357C:
        sub    dword ptr [esp + 0x44], ecx
        add    dword ptr [esp + 0x18], ecx
        sub    dword ptr [esp + 0x24], ecx
        push   ecx
        push   ebx
        call   sub_1_2A1C
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [esp + 0x24], 0
        jge    loc_1_35AB
        mov    dword ptr [esp + 0x24], 0
        and    dword ptr [esp + 0x28], 0xffffdfff
loc_1_35AB:
        mov    edx, dword ptr [esp + 0x28]
        and    edx, 0x100
        mov    eax, dword ptr [esp + 0x28]
        and    eax, 0x200
        or     eax, eax
        mov    eax, 0
        setne  al
        cmp    edx, 1
        sbb    ecx, ecx
        inc    ecx
        add    eax, ecx
        add    eax, dword ptr [esp + 0x44]
        je     loc_1_35D7
        dec    eax
loc_1_35D7:
        mov    ecx, dword ptr [esi + 0x40]
        mov    ebp, dword ptr [esi + 0x44]
        lea    ecx, [ecx + ebp - 1]
        mov    ebp, eax
        sub    ebp, ecx
        add    ebp, dword ptr [esp + 0x18]
        or     ebp, ebp
        jle    loc_1_36B6
        mov    ebp, dword ptr [esp + 0x18]
        sub    ebp, ecx
        or     ebp, ebp
        jle    loc_1_3648
        sub    ecx, dword ptr [esp + 0x18]
        cmp    ecx, -1
        jne    loc_1_3616
        cmp    dword ptr [esp + 0x44], 0
        jne    loc_1_3616
        test   byte ptr [esp + 0x29], 3
        je     loc_1_36B6
loc_1_3616:
        push   esi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_3630:
        push   esi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_3648:
        or     edx, edx
        je     loc_1_3655
        and    dword ptr [esp + 0x28], 0xfffffeff
        dec    eax
loc_1_3655:
        sub    eax, ecx
        add    eax, dword ptr [esp + 0x18]
        sub    dword ptr [esp + 0x44], eax
        jmp    loc_1_36B6
        db  08Dh, 049h, 000h
loc_1_3664:
        mov    eax, dword ptr [esi + 0x40]
        cmp    dword ptr [esp + 0x18], eax
        je     loc_1_3683
        test   byte ptr [esp + 0x29], 4
        je     loc_1_36D0
        mov    ecx, dword ptr [esp + 0x18]
        sub    ecx, eax
        cmp    ecx, 1
        jne    loc_1_36D0
        mov    dword ptr [esp + 0x18], eax
loc_1_3683:
        cmp    dword ptr [esp + 0x44], 0
        jne    loc_1_3691
        test   byte ptr [esp + 0x29], 3
        je     loc_1_36B6
loc_1_3691:
        and    dword ptr [esp + 0x28], 0xfffffcff
        mov    dword ptr [esp + 0x44], 0
        test   byte ptr [esp + 0x29], 4
        jne    loc_1_36BD
        push   0x200
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_36B6:
        test   byte ptr [esp + 0x29], 4
        je     loc_1_3730
loc_1_36BD:
        cmp    byte ptr [esi + 0x84], 3
        jne    loc_1_36E8
        test   byte ptr [esi + 0x58], 8
        je     loc_1_36E8
        mov    al, 0x10
        jmp    loc_1_36EA
loc_1_36D0:
        push   esi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_36E8:
        mov    al, 0x80
loc_1_36EA:
        push   dword ptr [esp + 0x10]
        push   eax
        push   esi
        call   sub_1_6398
        add    esp, 0xc
        cmp    byte ptr [esi + 0x84], 0xa
        je     loc_1_3719
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   0x14
        push   dword ptr [esp + 0x54]
        push   esi
        call   sub_1_6FB8
        add    esp, 0xc
loc_1_3719:
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_3730:
        test   byte ptr [esp + 0x29], 2
        je     loc_1_3798
        push   dword ptr [esp + 0x10]
        push   0x40
        push   esi
        call   sub_1_6398
        add    esp, 0xc
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x50]
        push   edi
        call   sub_1_82D8
        add    esp, 0x14
        cmp    byte ptr [esi + 0x84], 0xa
        je     loc_1_3780
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   0x14
        push   dword ptr [esp + 0x54]
        push   esi
        call   sub_1_6FB8
        add    esp, 0xc
loc_1_3780:
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_3798:
        test   byte ptr [esp + 0x29], 0x10
        je     loc_1_3E50
        mov    al, byte ptr [esi + 0x84]
        cmp    al, 3
        jne    loc_1_3844
        mov    ecx, dword ptr [esi + 4]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 0x1c]
        jns    loc_1_3814
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [esi + 0xc]
        or     eax, eax
        jg     loc_1_3814
        mov    byte ptr [esi + 0x85], 0
        mov    word ptr [esi + 0x90], 0
        inc    ecx
        mov    dword ptr [esi + 4], ecx
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x48], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0x4c], eax
        push   esi
        call   sub_1_845C
        add    esp, 4
        push   0
        push   dword ptr [esp + 0x54]
        push   esi
        call   sub_1_24B0
        add    esp, 0xc
        jmp    loc_1_3B67
loc_1_3814:
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x50]
        push   edi
        call   sub_1_82D8
        add    esp, 0x14
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_3844:
        mov    edx, dword ptr [esi + 4]
        mov    ecx, edx
        sub    ecx, dword ptr [esp + 0x1c]
        jns    loc_1_3A38
        mov    ecx, dword ptr [esp + 0x1c]
        sub    ecx, dword ptr [esi + 0xc]
        or     ecx, ecx
        jg     loc_1_3A38
        push   dword ptr [esp + 0x1c]
        push   esi
        call   sub_1_27B8
        add    esp, 8
        mov    eax, dword ptr [esi + 0x8c]
        or     eax, eax
        je     loc_1_38F4
        mov    ecx, dword ptr [esp + 0x1c]
        sub    ecx, dword ptr [esi + 0x78]
        or     ecx, ecx
        jle    loc_1_38F4
        mov    edx, dword ptr [0x966c]       ; -> loc_1_966C
        sub    dx, ax
        mov    dword ptr [esi + 0x8c], 0
        mov    ax, word ptr [esi + 0x7c]
        mov    cx, ax
        shr    cx, 3
        sub    dx, cx
        lea    edi, [eax + edx]
        mov    word ptr [esi + 0x7c], di
        movsx  eax, dx
        cdq    
        xor    eax, edx
        sub    eax, edx
        mov    cx, word ptr [esi + 0x7e]
        mov    dx, cx
        shr    dx, 3
        sub    ax, dx
        add    ax, cx
        mov    word ptr [esi + 0x7e], ax
        shr    di, 2
        movzx  ecx, di
        and    eax, 0xffff
        add    ecx, eax
        sar    ecx, 1
        cmp    ecx, 3
        jg     loc_1_38E3
        mov    ecx, 3
loc_1_38E3:
        cmp    ecx, 0x960
        jl     loc_1_38F0
        mov    ecx, 0x960
loc_1_38F0:
        mov    word ptr [esi + 0x72], cx
loc_1_38F4:
        mov    eax, dword ptr [esi + 0x58]
        test   ah, 0x10
        je     loc_1_3944
        and    eax, 0xffffefff
        mov    dword ptr [esi + 0x58], eax
        mov    byte ptr [esi + 0xd9], 0
        mov    ecx, 0x218
        movzx  eax, byte ptr [esi + 0xec]
        sub    ecx, eax
        movzx  eax, word ptr [esi + 0x80]
        cmp    ecx, eax
        jl     loc_1_3926
        mov    ecx, eax
loc_1_3926:
        movzx  eax, cx
        mov    word ptr [esi + 0x70], cx
        mov    dword ptr [esi + 0x1c], eax
        mov    al, byte ptr [esi + 0x82]
        dec    al
        mov    byte ptr [esi + 0x82], al
        jne    loc_1_3944
        and    dword ptr [esi + 0x5c], 0xfffffffe
loc_1_3944:
        mov    edi, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        cmp    edi, ecx
        jae    loc_1_3970
        cmp    dword ptr [esi + 0x20], edi
        movzx  eax, word ptr [esi + 0x70]
        jbe    loc_1_395C
        jmp    loc_1_3963
        db  08Dh, 049h, 000h
loc_1_395C:
        imul   eax, eax
        sub    edx, edx
        div    edi
loc_1_3963:
        add    edi, eax
        sub    edi, ecx
        sbb    eax, eax
        and    edi, eax
        add    edi, ecx
        mov    dword ptr [esi + 0x1c], edi
loc_1_3970:
        mov    ecx, dword ptr [esi + 0xc]
        mov    byte ptr [esi + 0x85], 0
        cmp    dword ptr [esp + 0x1c], ecx
        lea    edi, [esi + 0x90]
        je     loc_1_3990
        mov    ax, word ptr [esi + 0x72]
        mov    word ptr [edi], ax
        jmp    loc_1_3995
        db  090h
loc_1_3990:
        mov    word ptr [edi], 0
loc_1_3995:
        mov    eax, dword ptr [esi + 0x58]
        test   eax, 0x20000000
        je     loc_1_39C3
        cmp    dword ptr [esp + 0x1c], ecx
        jne    loc_1_39C3
        and    eax, 0xdffeffff
        mov    dword ptr [esi + 0x58], eax
        movzx  eax, byte ptr [esi + 0x84]
        cmp    eax, 5
        je     loc_1_39D0
        cmp    eax, 8
        je     loc_1_3A00
        cmp    eax, 9
        je     loc_1_3A1C
loc_1_39C3:
        mov    eax, 1
        jmp    loc_1_3A96
        db  08Dh, 049h, 000h
loc_1_39D0:
        push   0
        mov    byte ptr [esi + 0x84], 6
        push   dword ptr [esp + 0x54]
        push   esi
        call   sub_1_24B0
        add    esp, 0xc
        mov    al, byte ptr [0x9654]         ; -> loc_1_9654
        mov    byte ptr [esi + 0x85], al
        mov    word ptr [edi], 0x960
        mov    eax, 1
        jmp    loc_1_3A96
loc_1_3A00:
        push   dword ptr [esp + 0x10]
        push   0
        push   1
        push   esi
        call   sub_1_70A4
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_3A1C:
        push   dword ptr [esp + 0x10]
        push   0
        push   0
        push   esi
        call   sub_1_70A4
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_3A38:
        cmp    al, 6
        jne    loc_1_3A45
        mov    word ptr [esi + 0x90], 0x960
loc_1_3A45:
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [esi + 0xc]
        or     eax, eax
        jle    loc_1_3A68
        push   esi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_3A68:
        cmp    dword ptr [esp + 0x1c], edx
        jne    loc_1_3A94
        mov    eax, dword ptr [esi + 0x48]
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 0x18]
        js     loc_1_3A8A
        cmp    dword ptr [esp + 0x18], eax
        jne    loc_1_3A94
        mov    eax, dword ptr [esi + 0x4c]
        sub    eax, dword ptr [esp + 0x1c]
        or     eax, eax
        jg     loc_1_3A94
loc_1_3A8A:
        mov    eax, 1
        jmp    loc_1_3A96
        db  08Dh, 049h, 000h
loc_1_3A94:
        sub    eax, eax
loc_1_3A96:
        or     eax, eax
        je     loc_1_3B67
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esp + 0x20]
        sub    eax, ecx
        sbb    edx, edx
        not    edx
        and    eax, edx
        add    eax, ecx
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x48], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0x4c], eax
        cmp    dword ptr [esp + 0x20], 0
        jne    loc_1_3B14
        mov    eax, dword ptr [esi + 0x38]
        sub    eax, esi
        cmp    eax, 0x38
        je     loc_1_3B67
        mov    eax, dword ptr [esi + 0x58]
        and    eax, 0xfffffbff
        mov    dword ptr [esi + 0x58], eax
        mov    byte ptr [esi + 0x85], 0
        mov    cx, word ptr [esi + 0x72]
        mov    word ptr [esi + 0x90], cx
        test   eax, 0x80000
        jne    loc_1_3B67
        or     eax, 0x80000
        mov    dword ptr [esi + 0x58], eax
        inc    byte ptr [esi + 0x82]
        or     dword ptr [esi + 0x5c], 1
        jmp    loc_1_3B67
loc_1_3B14:
        mov    eax, dword ptr [esi + 0x58]
        test   eax, 0x80000
        je     loc_1_3B4D
        mov    byte ptr [esi + 0x85], 0
        and    eax, 0xffb7ffff
        mov    dword ptr [esi + 0x58], eax
        push   dword ptr [esi + 4]
        push   esi
        call   sub_1_5534
        add    esp, 8
        mov    al, byte ptr [esi + 0x82]
        dec    al
        mov    byte ptr [esi + 0x82], al
        jne    loc_1_3B4D
        and    dword ptr [esi + 0x5c], 0xfffffffe
loc_1_3B4D:
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_3B59
        test   byte ptr [esi + 0x5a], 1
        je     loc_1_3B67
loc_1_3B59:
        push   0x400
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_3B67:
        cmp    dword ptr [esp + 0x44], 0
        ja     loc_1_3B8C
        test   byte ptr [esp + 0x29], 1
        jne    loc_1_3B8C
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_3B8C:
        mov    cl, byte ptr [esi + 0x84]
        cmp    cl, 4
        jb     loc_1_3E38
        cmp    cl, 6
        ja     loc_1_3E38
        mov    eax, dword ptr [esi + 0x58]
        and    eax, 0x1000000
        jne    loc_1_3E38
        mov    edx, dword ptr [esp + 0x18]
        cmp    dword ptr [esi + 0x40], edx
        jne    loc_1_3DDC
        mov    eax, dword ptr [esi + 0x5c]
        test   al, 2
        je     loc_1_3BE0
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_3BE0:
        or     eax, 2
        mov    dword ptr [esi + 0x5c], eax
loc_1_3BE6:
        test   byte ptr [esp + 0x29], 0x20
        je     loc_1_3C0E
        lea    eax, [esp + 0x44]
        push   eax
        push   ebx
        lea    eax, [esp + 0x20]
        push   eax
        push   esi
        call   sub_1_4EF8
        add    esp, 0x10
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_3DA0
loc_1_3C0E:
        cmp    dword ptr [esp + 0x44], 0
        jbe    loc_1_3C70
        push   dword ptr [esp + 0x44]
        push   ebx
        push   dword ptr [esp + 0x30]
        push   esi
        call   dword ptr [esi + 0x60]
        add    esp, 0x10
        mov    edi, eax
        add    dword ptr [esi + 0x40], edi
        sub    dword ptr [esi + 0x44], edi
        mov    eax, dword ptr [esi + 0x58]
        test   ah, 8
        je     loc_1_3C48
        push   0x200
        push   esi
        call   sub_1_2388
        add    esp, 8
        jmp    loc_1_3C59
        db  08Dh, 049h, 000h
loc_1_3C48:
        or     eax, 0x800
        mov    dword ptr [esi + 0x58], eax
        mov    word ptr [esi + 0x92], 2
loc_1_3C59:
        cmp    edi, dword ptr [esp + 0x44]
        jne    loc_1_3CF8
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_3DA0
loc_1_3C70:
        mov    edx, dword ptr [esi + 0x58]
        test   dh, 0x80
        je     loc_1_3CCC
        test   edx, 0x4000000
        jne    loc_1_3C9C
        mov    eax, dword ptr [esi + 0x40]
        cmp    dword ptr [esi + 0x108], eax
        jne    loc_1_3C95
        mov    eax, dword ptr [esi + 0x10c]
        inc    eax
        mov    dword ptr [esi + 0x40], eax
loc_1_3C95:
        sub    eax, eax
        jmp    loc_1_3CA2
        db  08Dh, 049h, 000h
loc_1_3C9C:
        mov    eax, dword ptr [esi + 0x94]
loc_1_3CA2:
        mov    ecx, dword ptr [esi + 0x40]
        sub    ecx, dword ptr [esi + 0x10c]
        sub    ecx, eax
        je     loc_1_3CCC
        and    edx, 0xffff7fff
        mov    dword ptr [esi + 0x58], edx
        mov    al, byte ptr [esi + 0x82]
        dec    al
        mov    byte ptr [esi + 0x82], al
        jne    loc_1_3CCC
        and    dword ptr [esi + 0x5c], 0xfffffffe
loc_1_3CCC:
        test   byte ptr [esp + 0x29], 1
        jne    loc_1_3D0C
        lea    eax, [esp + 0x44]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   esi
        call   sub_1_2A84
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_3BE6
        jmp    loc_1_3DA0
        db  08Dh, 049h, 000h
loc_1_3CF8:
        push   0x200
        push   esi
        call   sub_1_2388
        add    esp, 8
        jmp    loc_1_3DA0
        db  090h
loc_1_3D0C:
        sub    edi, edi
        inc    dword ptr [esi + 0x40]
        push   0x200
        push   esi
        call   sub_1_2388
        add    esp, 8
        push   esi
        call   sub_1_4DD8
        add    esp, 4
        movzx  eax, byte ptr [esi + 0x84]
        cmp    eax, 3
        jl     loc_1_3D8D
        cmp    eax, 4
        jle    loc_1_3D48
        cmp    eax, 5
        je     loc_1_3D60
        cmp    eax, 6
        je     loc_1_3D70
        jmp    loc_1_3D8D
        db  08Dh, 049h, 000h
loc_1_3D48:
        inc    byte ptr [esi + 0x82]
        mov    byte ptr [esi + 0x84], 7
        or     dword ptr [esi + 0x5c], 1
        mov    edi, 1
        jmp    loc_1_3D8D
loc_1_3D60:
        mov    edi, 1
        mov    byte ptr [esi + 0x84], 8
        jmp    loc_1_3D8D
        db  08Bh, 0FFh
loc_1_3D70:
        inc    dword ptr [esi + 0x74]
        mov    word ptr [esi + 0x90], 0
        push   dword ptr [esp + 0x10]
        push   1
        push   1
        push   esi
        call   sub_1_70A4
        add    esp, 0x10
loc_1_3D8D:
        or     edi, edi
        je     loc_1_3DA0
        push   0x16
        push   dword ptr [esp + 0x54]
        push   esi
        call   sub_1_6FB8
        add    esp, 0xc
loc_1_3DA0:
        mov    eax, dword ptr [esi + 0x58]
        and    dword ptr [esi + 0x5c], 0xfffffffd
        test   eax, 0x800000
        je     loc_1_3DC4
        and    eax, 0xff7fffff
        mov    dword ptr [esi + 0x58], eax
        push   0x400
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_3DC4:
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_3DDC:
        cmp    cl, 4
        jb     loc_1_3E20
        cmp    cl, 6
        ja     loc_1_3E20
        or     eax, eax
        jne    loc_1_3E20
        push   dword ptr [esp + 0x44]
        push   ebx
        lea    eax, [esp + 0x20]
        push   eax
        push   esi
        call   sub_1_2C28
        add    esp, 0x10
        push   esi
        call   sub_1_80C8
        add    esp, 4
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_3E20:
        push   esi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_3E38:
        push   esi
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_3E50:
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_3E68:
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x50]
        push   edi
        call   sub_1_82D8
        add    esp, 0x14
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_3E8C:
        test   byte ptr [esp + 0x29], 4
        je     loc_1_3EA0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_3EA0:
        push   dword ptr [esp + 0x38]
        call   sub_1_95A8
        add    esp, 4
        test   al, 1
        jne    loc_1_4050
        cmp    al, 0xff
        je     loc_1_4050
        mov    eax, dword ptr [esp + 0x28]
        and    ah, 0x12
        cmp    ah, 2
        jne    loc_1_402C
        push   0
        push   6
        mov    ax, word ptr [edi + 2]
        push   eax
        push   dword ptr [esp + 0x40]
        call   sub_1_0414
        add    esp, 0x10
        or     eax, eax
        je     loc_1_402C
        push   dword ptr [esp + 0x50]
        mov    cx, word ptr [edi]
        push   ecx
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_250C
        add    esp, 0x14
        mov    esi, eax
        or     esi, esi
        je     loc_1_402C
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [esi + 0x64], eax
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [esi + 0x68], eax
        mov    ax, word ptr [edi]
        mov    word ptr [esi + 0x6c], ax
        mov    ax, word ptr [edi + 2]
        mov    word ptr [esi + 0x6e], ax
        inc    dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x40], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x10], eax
        push   edi
        call   sub_1_2718
        add    esp, 4
        mov    word ptr [esi + 0x80], ax
        mov    byte ptr [esi + 0x84], 3
        inc    dword ptr [0x9554]            ; -> obj1:0x9554
        or     dword ptr [esi + 0x5c], 2
        push   esi
        call   sub_1_5BE8
        add    esp, 4
        or     eax, eax
        jne    loc_1_3F98
        push   0x13
        push   dword ptr [esp + 0x54]
        push   esi
        call   sub_1_24B0
        add    esp, 0xc
        dec    dword ptr [esi + 0x74]
        push   dword ptr [esp + 0x10]
        push   0x40
        push   esi
        call   sub_1_6398
        add    esp, 0xc
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h
loc_1_3F98:
        mov    eax, dword ptr [esi + 0x58]
        and    dword ptr [esi + 0x5c], 0xfffffffd
        test   eax, 0x800000
        je     loc_1_3FBC
        and    eax, 0xff7fffff
        mov    dword ptr [esi + 0x58], eax
        push   0x400
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_3FBC:
        mov    eax, dword ptr [esp + 0x50]
        cmp    dword ptr [eax], 0
        je     loc_1_3FE5
        test   byte ptr [esi + 0x58], 2
        jne    loc_1_3FE5
        push   0
        push   dword ptr [esp + 0x3c]
        lea    eax, [esi + 0xe4]
        push   eax
        push   dword ptr [esp + 0x5c]
        call   sub_1_95BC
        add    esp, 0x10
loc_1_3FE5:
        test   byte ptr [esi + 0x58], 4
        je     loc_1_4004
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_6C9C
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_4004:
        push   0
        push   dword ptr [esp + 0x54]
        push   esi
        call   sub_1_24B0
        add    esp, 0xc
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_402C:
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x38]
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x50]
        push   edi
        call   sub_1_82D8
        add    esp, 0x14
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  090h
loc_1_4050:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_405C:
        sub    eax, eax
        inc    dword ptr [0x9570]            ; -> obj1:0x9570
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_406C:
        sub    eax, eax
        inc    dword ptr [0x9570]            ; -> obj1:0x9570
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_1_407C:
        sub    eax, eax
        inc    dword ptr [0x9570]            ; -> obj1:0x9570
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
sub_1_408C:
        mov    eax, dword ptr [0x9304]       ; -> obj1:0x9304
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x9304], ecx       ; -> obj1:0x9304
        ret    
        db  08Bh, 0FFh
sub_1_40A0:
        mov    eax, dword ptr [0x9304]       ; -> obj1:0x9304
        or     eax, eax
        je     loc_1_40B4
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x9304], ecx       ; -> obj1:0x9304
        ret    
        db  08Bh, 0FFh
loc_1_40B4:
        mov    eax, dword ptr [0x9308]       ; -> obj1:0x9308
        cmp    dword ptr [0x930c], eax       ; -> obj1:0x930C
        mov    eax, 0
        jbe    loc_1_40D0
        push   0x24
        call   sub_1_90F6
        add    esp, 4
loc_1_40D0:
        or     eax, eax
        je     loc_1_40DA
        inc    dword ptr [0x9308]            ; -> obj1:0x9308
loc_1_40DA:
        ret    
        db  090h
sub_1_40DC:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax], 0
        je     loc_1_40EC
loc_1_40E5:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    loc_1_40E5
loc_1_40EC:
        ret    
        db  08Dh, 049h, 000h
sub_1_40F0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        or     edi, edi
        mov    esi, dword ptr [esp + 0xc]
        je     loc_1_4130
loc_1_40FE:
        mov    eax, dword ptr [esi + 0xc]
        cmp    edi, eax
        jb     loc_1_4124
        sub    edi, eax
        mov    eax, esi
        mov    esi, dword ptr [esi]
        cmp    dword ptr [eax + 4], 3
        jne    loc_1_411A
        push   eax
        call   sub_1_90FC
        add    esp, 4
loc_1_411A:
        or     edi, edi
        jne    loc_1_40FE
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_4124:
        mov    eax, esi
        sub    dword ptr [esi + 0xc], edi
        add    dword ptr [esi + 8], edi
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_4130:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_4138:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x20]
        cmp    dword ptr [ebp + 4], 3
        je     loc_1_4268
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esp + 0x28]
        sub    eax, ebx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, ebx
        mov    dword ptr [esp + 0x18], eax
        add    eax, 0x10
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        je     loc_1_425C
        mov    edx, eax
        mov    dword ptr [esp + 0x10], edx
        mov    ecx, eax
        mov    dword ptr [ecx], 0
        mov    ecx, eax
        mov    dword ptr [ecx + 4], 3
        mov    ecx, eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 0xc], eax
        mov    edi, dword ptr [esp + 0x14]
        add    edi, 0x10
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 8], edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    ebx, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [esp + 0x20], ecx
        or     ecx, ecx
        je     loc_1_4235
loc_1_41C6:
        mov    ecx, dword ptr [esp + 0x20]
        cmp    dword ptr [ecx + 4], 3
        je     loc_1_4235
        or     ebx, ebx
        je     loc_1_4235
        mov    eax, dword ptr [ecx + 0xc]
        sub    eax, ebx
        sbb    ecx, ecx
        and    eax, ecx
        lea    ebp, [eax + ebx]
        lea    eax, [ebp + 0x10]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        or     eax, eax
        je     loc_1_4244
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 3
        mov    dword ptr [eax + 0xc], ebp
        lea    edi, [eax + 0x10]
        mov    dword ptr [eax + 8], edi
        mov    ecx, dword ptr [esp + 0x20]
        mov    esi, dword ptr [ecx + 8]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [edx], eax
        mov    dword ptr [esp + 0x10], eax
        sub    ebx, ebp
        mov    ecx, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [esp + 0x20], ecx
        or     ecx, ecx
        jne    loc_1_41C6
loc_1_4235:
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [edx], ecx
        jmp    loc_1_4282
        db  08Dh, 049h, 000h
loc_1_4244:
        push   dword ptr [esp + 0x14]
        call   sub_1_2A58
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_425C:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_4268:
        mov    dword ptr [esp + 0x14], ebp
        mov    dword ptr [esp + 0x10], ebp
        add    ebp, 0xc
        mov    ebx, dword ptr [esp + 0x28]
        cmp    dword ptr [ebp], ebx
        jbe    loc_1_427F
        mov    dword ptr [ebp], ebx
loc_1_427F:
        sub    ebx, dword ptr [ebp]
loc_1_4282:
        or     ebx, ebx
        je     loc_1_42A1
loc_1_4286:
        mov    edx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [edx]
        mov    dword ptr [esp + 0x10], edx
        cmp    dword ptr [edx + 0xc], ebx
        jbe    loc_1_4298
        mov    dword ptr [edx + 0xc], ebx
loc_1_4298:
        mov    edx, dword ptr [esp + 0x10]
        sub    ebx, dword ptr [edx + 0xc]
        jne    loc_1_4286
loc_1_42A1:
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     loc_1_42BE
        push   eax
        call   sub_1_2A58
        add    esp, 4
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [edx], 0
loc_1_42BE:
        mov    eax, dword ptr [esp + 0x24]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_42D4:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        push   esi
        lea    eax, [esp + 0xc]
        push   eax
        mov    edi, dword ptr [esp + 0x20]
        push   edi
        call   sub_1_4138
        add    esp, 0xc
        or     eax, eax
        je     loc_1_4340
        mov    edx, dword ptr [esp + 0x10]
        cmp    dword ptr [edx + 0xac], 0
        jne    loc_1_4320
        mov    dword ptr [edx + 0xac], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0xb0], eax
        add    dword ptr [edx + 0xa8], esi
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  090h
loc_1_4320:
        mov    ecx, dword ptr [edx + 0xb0]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0xb0], eax
        add    dword ptr [edx + 0xa8], esi
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_1_4340:
        push   edi
        call   sub_1_2A58
        add    esp, 4
        sub    esi, esi
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  090h
sub_1_4354:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], 0
        mov    ecx, dword ptr [esp + 0x3c]
        mov    ebx, dword ptr [ecx + 0x50]
        mov    ebp, dword ptr [ebx + 0x20]
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [ebx + 0x1c]
        sub    eax, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [ebp + 0xc]
        sub    ecx, edx
        sub    eax, ecx
        sbb    esi, esi
        and    eax, esi
        add    eax, ecx
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebp + 4]
        add    eax, edx
        mov    dword ptr [esp + 0x1c], eax
        mov    ecx, dword ptr [esp + 0x44]
        mov    dword ptr [esp + 0x18], ecx
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 0x28], eax
        mov    eax, dword ptr [esp + 0x40]
        and    eax, 0x800
        cmp    eax, 1
        cmc    
        sbb    eax, eax
        and    eax, 0x80000000
        mov    dword ptr [esp + 0x2c], eax
        mov    dword ptr [esp + 0x30], eax
        or     dword ptr [ebx + 0x14], eax
loc_1_43C5:
        mov    edx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x24]
        sub    edx, eax
        sbb    ecx, ecx
        and    edx, ecx
        add    edx, eax
        mov    eax, dword ptr [esp + 0x48]
        sub    eax, dword ptr [esp + 0x14]
        sub    edx, eax
        sbb    ecx, ecx
        and    edx, ecx
        add    eax, edx
        mov    dword ptr [esp + 0x10], eax
        mov    edi, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [esp + 0x28]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [esp + 0x14], eax
        add    dword ptr [ebx + 0xc], eax
        sub    dword ptr [esp + 0x24], eax
        jne    loc_1_4448
        mov    edx, dword ptr [esp + 0x18]
        mov    ecx, edx
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [esp + 0x18], ecx
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, dword ptr [esp + 0x48]
        je     loc_1_4434
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 0x28], eax
loc_1_4434:
        cmp    dword ptr [edx + 4], 3
        jne    loc_1_4450
        push   edx
        call   sub_1_90FC
        add    esp, 4
        jmp    loc_1_4450
        db  08Dh, 049h, 000h
loc_1_4448:
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [esp + 0x28], eax
loc_1_4450:
        mov    eax, dword ptr [esp + 0x10]
        sub    dword ptr [esp + 0x20], eax
        jne    loc_1_44E8
        mov    eax, dword ptr [ebx + 0x1c]
        sub    eax, dword ptr [ebx + 0xc]
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     loc_1_4488
        or     eax, eax
        je     loc_1_4488
        mov    ecx, dword ptr [ebp + 0xc]
        sub    ecx, eax
        sbb    edx, edx
        and    ecx, edx
        add    eax, ecx
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_1_44F0
        db  090h
loc_1_4488:
        mov    eax, dword ptr [ebx + 0x14]
        or     eax, 0x80000000
        mov    dword ptr [ebx + 0x14], eax
        test   ah, 1
        je     loc_1_44A9
        push   0x200
        push   dword ptr [esp + 0x40]
        call   sub_1_2388
        add    esp, 8
loc_1_44A9:
        mov    ebx, dword ptr [ebx]
        mov    dword ptr [esp + 0x30], 1
        or     ebx, ebx
        je     loc_1_44FE
        mov    ebp, dword ptr [ebx + 0x20]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebx + 0x1c]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, dword ptr [esp + 0x48]
        je     loc_1_44F0
        mov    eax, dword ptr [esp + 0x2c]
        or     dword ptr [ebx + 0x14], eax
        jmp    loc_1_44F0
        db  08Bh, 0FFh
loc_1_44E8:
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [esp + 0x1c], eax
loc_1_44F0:
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, dword ptr [esp + 0x48]
        jne    loc_1_43C5
loc_1_44FE:
        or     ebx, ebx
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [ecx + 0x50], ebx
        je     loc_1_4520
        mov    dword ptr [ebx + 0x20], ebp
        mov    eax, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebx + 0x10], eax
        mov    dword ptr [ecx + 0x60], 0x4354 ; -> sub_1_4354
        jmp    loc_1_452B
        db  090h
loc_1_4520:
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [ecx + 0x60], 0x42d4 ; -> loc_1_42D4
loc_1_452B:
        mov    ecx, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [ecx + 0x54]
        mov    ecx, dword ptr [esp + 0x14]
        mov    edx, eax
        sub    ecx, edx
        sbb    esi, esi
        and    ecx, esi
        add    ecx, edx
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [ecx + 0x54], eax
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, dword ptr [esp + 0x48]
        je     loc_1_45D8
        mov    eax, dword ptr [esp + 0x28]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [esp + 0x48]
        sub    eax, dword ptr [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_1_4138
        add    esp, 0xc
        or     eax, eax
        je     loc_1_45DF
        mov    ecx, dword ptr [esp + 0x3c]
        mov    ecx, dword ptr [ecx + 0xac]
        or     ecx, ecx
        jne    loc_1_45AC
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [ecx + 0xac], eax
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [ecx + 0xb0], eax
        jmp    loc_1_45BC
        db  090h
loc_1_45AC:
        mov    edx, dword ptr [esp + 0x34]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esp + 0x3c]
        mov    dword ptr [ecx + 0xac], eax
loc_1_45BC:
        mov    eax, dword ptr [esp + 0x48]
        sub    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x3c]
        add    dword ptr [ecx + 0xa8], eax
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [esp + 0x14], eax
        jmp    loc_1_45EB
loc_1_45D8:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_45EB
loc_1_45DF:
        push   dword ptr [esp + 0x18]
        call   sub_1_2A58
        add    esp, 4
loc_1_45EB:
        cmp    dword ptr [esp + 0x30], 0
        je     loc_1_4608
        push   0x100
        push   dword ptr [esp + 0x40]
        call   sub_1_2388
        add    esp, 8
        jmp    loc_1_4613
        db  08Dh, 049h, 000h
loc_1_4608:
        mov    ecx, dword ptr [esp + 0x3c]
        mov    byte ptr [ecx + 0x83], 5
loc_1_4613:
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  090h
loc_1_4620:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        call   sub_1_40A0
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_4864
        mov    esi, dword ptr [esp + 0x34]
        mov    edi, dword ptr [esp + 0x40]
        mov    dword ptr [esi + 0x54], edi
        or     dword ptr [esi + 0x58], 0x80
        mov    ecx, dword ptr [esp + 0x38]
        and    ecx, 0x800
        or     ecx, 0x1440
        shr    ecx, 1
        lea    eax, [esp + 0x1c]
        push   eax
        mov    ebx, dword ptr [esp + 0x40]
        push   dword ptr [ebx + 8]
        lea    eax, [esp + 0x18]
        push   eax
        push   edi
        push   dword ptr [ebx + 0xc]
        push   ecx
        push   dword ptr [esi + 0xb8]
        push   dword ptr [esi + 0xe0]
        call   dword ptr [esi + 0xdc]
        add    esp, 0x20
        mov    dword ptr [esp + 0x18], eax
        and    dword ptr [esi + 0x58], 0xffffff7f
        cmp    eax, 0x18
        jne    loc_1_4760
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ebp + 0x20], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ebp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [ebp + 0x18], eax
        mov    eax, dword ptr [esp + 0x2c]
        movzx  eax, word ptr [eax]
        mov    dword ptr [ebp + 0x14], eax
        mov    dword ptr [ebp + 0x10], 0
        mov    dword ptr [ebp + 0xc], 0
        mov    eax, dword ptr [esi + 0xa0]
        or     eax, eax
        jne    loc_1_46FC
        mov    dword ptr [esi + 0xa0], ebp
        mov    dword ptr [esi + 0xa4], ebp
        mov    dword ptr [ebp], 0
        jmp    loc_1_4705
        db  090h
loc_1_46FC:
        mov    dword ptr [ebp], eax
        mov    dword ptr [esi + 0xa0], ebp
loc_1_4705:
        mov    dword ptr [esi + 0x50], ebp
        mov    dword ptr [esi + 0x60], 0x4354 ; -> sub_1_4354
        mov    eax, dword ptr [esp + 0x10]
        sub    dword ptr [esi + 0x54], eax
        sub    edi, dword ptr [esp + 0x10]
        je     loc_1_4748
        push   dword ptr [esp + 0x10]
        push   ebx
        call   sub_1_40F0
        add    esp, 8
        push   edi
        push   eax
        push   dword ptr [esp + 0x40]
        push   esi
        call   sub_1_4354
        add    esp, 0x10
        add    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_4748:
        push   ebx
        call   sub_1_2A58
        add    esp, 4
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_4760:
        push   ebp
        call   sub_1_408C
        add    esp, 4
        cmp    dword ptr [esp + 0x18], 0x17
        jne    loc_1_4778
        mov    dword ptr [esp + 0x10], 0
loc_1_4778:
        mov    eax, dword ptr [esp + 0x10]
        sub    dword ptr [esi + 0x54], eax
        cmp    dword ptr [esi + 0xa0], 0
        je     loc_1_478F
        mov    dword ptr [esi + 0x60], 0x4354 ; -> sub_1_4354
loc_1_478F:
        cmp    edi, dword ptr [esp + 0x10]
        jne    loc_1_47AC
        push   ebx
        call   sub_1_2A58
        add    esp, 4
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Bh, 0FFh
loc_1_47AC:
        push   dword ptr [esp + 0x10]
        push   ebx
        call   sub_1_40F0
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [esi + 0xa0], 0
        jne    loc_1_4840
        mov    dword ptr [esi + 0x60], 0x42d4 ; -> loc_1_42D4
        mov    eax, edi
        sub    eax, dword ptr [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ebx
        call   sub_1_4138
        add    esp, 0xc
        or     eax, eax
        je     loc_1_4828
        mov    ecx, dword ptr [esi + 0xac]
        or     ecx, ecx
        jne    loc_1_4800
        mov    dword ptr [esi + 0xac], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0xb0], eax
        jmp    loc_1_480C
loc_1_4800:
        mov    edx, dword ptr [esp + 0x14]
        mov    dword ptr [edx], ecx
        mov    dword ptr [esi + 0xac], eax
loc_1_480C:
        mov    eax, edi
        sub    eax, dword ptr [esp + 0x10]
        add    dword ptr [esi + 0xa8], eax
        mov    dword ptr [esp + 0x10], edi
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_4828:
        push   ebx
        call   sub_1_2A58
        add    esp, 4
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_4840:
        sub    edi, dword ptr [esp + 0x10]
        push   edi
        push   ebx
        push   dword ptr [esp + 0x40]
        push   esi
        call   sub_1_4354
        add    esp, 0x10
        add    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_4864:
        push   dword ptr [esp + 0x3c]
        call   sub_1_2A58
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Bh, 0FFh
sub_1_487C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [ebp + 8]
loc_1_488E:
        mov    edx, dword ptr [esi + 0xa8]
        mov    dword ptr [esi + 0x54], edx
        or     dword ptr [esi + 0x58], 0x80
        mov    ecx, dword ptr [esi + 0xac]
        lea    eax, [ebp - 0x18]
        push   eax
        push   dword ptr [ecx + 8]
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        push   dword ptr [ecx + 0xc]
        push   0x620
        push   dword ptr [esi + 0xb8]
        push   dword ptr [esi + 0xe0]
        call   dword ptr [esi + 0xdc]
        add    esp, 0x20
        and    dword ptr [esi + 0x58], 0xffffff7f
        cmp    eax, 0x18
        je     loc_1_4944
        cmp    eax, 0x17
        jne    loc_1_48E5
        mov    dword ptr [ebp - 4], 0
loc_1_48E5:
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [esi + 0x54], eax
        mov    edi, dword ptr [esi + 0xa8]
        mov    eax, dword ptr [esi + 0xac]
        cmp    dword ptr [ebp - 4], edi
        je     loc_1_4A0C
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_1_40F0
        add    esp, 8
        cmp    dword ptr [esi + 0xa0], 0
        jne    loc_1_4A50
        mov    dword ptr [esi + 0xac], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [esi + 0xa8], eax
        cmp    dword ptr [esi + 0x54], 0
        jne    loc_1_4A81
        cmp    dword ptr [esi + 0xdc], 0
        jne    loc_1_488E
        jmp    loc_1_4A81
loc_1_4944:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx + 0x20], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, word ptr [eax]
        mov    dword ptr [ecx + 0x14], eax
        mov    dword ptr [ecx + 0x10], 0
        mov    dword ptr [ecx + 0xc], 0
        mov    eax, dword ptr [esi + 0xa0]
        or     eax, eax
        jne    loc_1_499C
        mov    dword ptr [esi + 0xa0], ecx
        mov    dword ptr [esi + 0xa4], ecx
        mov    dword ptr [ecx], 0
        jmp    loc_1_49A4
        db  08Bh, 0FFh
loc_1_499C:
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0xa0], ecx
loc_1_49A4:
        mov    dword ptr [esi + 0x50], ecx
        mov    dword ptr [esi + 0x60], 0x4354 ; -> sub_1_4354
        mov    edi, dword ptr [esi + 0xa8]
        mov    eax, dword ptr [esi + 0xac]
        mov    dword ptr [esi + 0xa8], 0
        mov    dword ptr [esi + 0xac], 0
        mov    ecx, dword ptr [ebp - 4]
        sub    dword ptr [esi + 0x54], ecx
        sub    edi, dword ptr [ebp - 4]
        je     loc_1_49FC
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_1_40F0
        add    esp, 8
        push   edi
        push   eax
        push   0x800
        push   esi
        call   sub_1_4354
        add    esp, 0x10
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_49FC:
        push   eax
        call   sub_1_2A58
        add    esp, 4
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_4A0C:
        mov    dword ptr [esi + 0xa8], 0
        mov    dword ptr [esi + 0xac], 0
        cmp    dword ptr [esi + 0xa0], 0
        jne    loc_1_4A3C
        cmp    dword ptr [esi + 0xdc], 0
        je     loc_1_4A43
        mov    dword ptr [esi + 0x60], 0x4620 ; -> loc_1_4620
        jmp    loc_1_4A43
        db  090h
loc_1_4A3C:
        mov    dword ptr [esi + 0x60], 0x4354 ; -> sub_1_4354
loc_1_4A43:
        push   eax
        call   sub_1_2A58
        add    esp, 4
        jmp    loc_1_4A81
        db  08Bh, 0FFh
loc_1_4A50:
        mov    dword ptr [esi + 0x60], 0x4354 ; -> sub_1_4354
        mov    dword ptr [esi + 0xa8], 0
        mov    dword ptr [esi + 0xac], 0
        sub    edi, dword ptr [ebp - 4]
        push   edi
        push   eax
        push   0x800
        push   esi
        call   sub_1_4354
        add    esp, 0x10
        add    dword ptr [ebp - 4], eax
loc_1_4A81:
        push   dword ptr [ebp + 0xc]
        call   sub_1_408C
        add    esp, 4
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_4A90:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        mov    edi, dword ptr [esp + 0x40]
        or     edi, edi
        mov    esi, dword ptr [esp + 0x3c]
        je     loc_1_4AD8
        cmp    dword ptr [esi + 0xfc], 0
        jne    loc_1_4ABA
        test   byte ptr [esi + 0x59], 0x40
        je     loc_1_4B0C
loc_1_4ABA:
        add    esi, 0xfc
        cmp    dword ptr [esi], 0
        je     loc_1_4ACC
loc_1_4AC5:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    loc_1_4AC5
loc_1_4ACC:
        mov    dword ptr [esi], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Bh, 0FFh
loc_1_4AD8:
        mov    edi, dword ptr [esi + 0xfc]
        or     edi, edi
        je     loc_1_4DCE
        test   byte ptr [esi + 0x59], 0x40
        jne    loc_1_4DCE
        mov    ebx, dword ptr [esi + 0x100]
        mov    dword ptr [esi + 0xfc], 0
        mov    dword ptr [esi + 0x100], 0
        jmp    loc_1_4B10
loc_1_4B0C:
        mov    ebx, dword ptr [esp + 0x44]
loc_1_4B10:
        inc    byte ptr [esi + 0x82]
        or     dword ptr [esi + 0x58], 0x4000
        or     dword ptr [esi + 0x5c], 1
loc_1_4B21:
        mov    dword ptr [esp + 0x10], 0
        mov    ebp, dword ptr [esi + 0xf8]
        or     ebp, ebp
        je     loc_1_4B40
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esi + 0xf8], eax
        jmp    loc_1_4BB6
        db  08Bh, 0FFh
loc_1_4B40:
        lea    ecx, [esi + 0xa0]
        mov    ebp, dword ptr [ecx]
        or     ebp, ebp
        je     loc_1_4C04
loc_1_4B50:
        test   byte ptr [ebp + 0x14], 0x40
        je     loc_1_4B5C
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_4B68
loc_1_4B5C:
        mov    ecx, ebp
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    loc_1_4B50
        jmp    loc_1_4BB6
        db  090h
loc_1_4B68:
        cmp    dword ptr [esi + 0xa4], ebp
        jne    loc_1_4B88
        cmp    dword ptr [esi + 0xa0], ebp
        mov    dword ptr [esi + 0xa4], 0
        je     loc_1_4B88
        mov    dword ptr [esi + 0xa4], ecx
loc_1_4B88:
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [esi + 0x50], ebp
        jne    loc_1_4BB6
        mov    dword ptr [esi + 0x50], eax
        or     eax, eax
        jne    loc_1_4BB6
        cmp    dword ptr [esi + 0xdc], 0
        je     loc_1_4BAF
        cmp    dword ptr [esi + 0x54], 0
        mov    dword ptr [esi + 0x60], 0x4620 ; -> loc_1_4620
        je     loc_1_4BB6
loc_1_4BAF:
        mov    dword ptr [esi + 0x60], 0x42d4 ; -> loc_1_42D4
loc_1_4BB6:
        or     ebp, ebp
        je     loc_1_4C04
        push   0
        push   ebx
        push   dword ptr [ebp + 0x20]
        push   edi
        call   sub_1_1DD0
        add    esp, 0x10
        mov    dword ptr [esp + 0x10], eax
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   dword ptr [ebp + 4]
        add    esp, 0xc
        push   ebp
        call   sub_1_408C
        add    esp, 4
        mov    eax, dword ptr [esi + 0x104]
        mov    ecx, eax
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, edx
        sbb    ebp, ebp
        and    ecx, ebp
        add    ecx, edx
        sub    eax, ecx
        mov    dword ptr [esi + 0x104], eax
        jmp    loc_1_4D0D
        db  090h
loc_1_4C04:
        cmp    dword ptr [esi + 0x104], 0
        jne    loc_1_4D96
        mov    ecx, dword ptr [esi + 0xb8]
        mov    eax, dword ptr [esi + 0xcc]
        or     eax, eax
        je     loc_1_4D8C
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_1_4D96
        test   byte ptr [eax + 0x32], 0x80
        je     loc_1_4D96
        mov    edx, dword ptr [eax + 0x8c]
        or     edx, edx
        je     loc_1_4D96
        mov    eax, dword ptr [eax + 0x90]
        mov    dword ptr [esi + 0x104], ebx
        lea    ebp, [esp + 0x24]
        push   ebp
        push   dword ptr [edi + 8]
        lea    ebp, [esp + 0x18]
        push   ebp
        push   ebx
        push   dword ptr [edi + 0xc]
        push   0x640
        push   ecx
        push   eax
        call   edx
        add    esp, 0x20
        cmp    eax, 0x18
        jne    loc_1_4CE4
        cmp    ebx, dword ptr [esp + 0x10]
        mov    eax, 0
        je     loc_1_4CA3
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_40F0
        add    esp, 8
        mov    edi, eax
        push   0
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   edi
        call   sub_1_1DD0
        add    esp, 0x10
loc_1_4CA3:
        add    dword ptr [esp + 0x10], eax
        mov    ecx, dword ptr [esi + 0x104]
        mov    edx, ecx
        mov    ebp, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x14], ebp
        sub    ecx, ebp
        mov    ebp, dword ptr [esp + 0x18]
        sbb    dword ptr [esp + 0x18], ebp
        and    ecx, dword ptr [esp + 0x18]
        add    ecx, dword ptr [esp + 0x14]
        sub    edx, ecx
        mov    dword ptr [esi + 0x104], edx
        push   eax
        push   0
        push   dword ptr [esp + 0x38]
        call   dword ptr [esp + 0x38]
        add    esp, 0xc
        jmp    loc_1_4D0D
        db  08Dh, 049h, 000h
loc_1_4CE4:
        cmp    eax, 0x17
        jne    loc_1_4CF1
        mov    dword ptr [esp + 0x10], 0
loc_1_4CF1:
        mov    eax, dword ptr [esi + 0x104]
        mov    ecx, eax
        mov    edx, dword ptr [esp + 0x10]
        sub    eax, edx
        sbb    ebp, ebp
        and    eax, ebp
        add    eax, edx
        sub    ecx, eax
        mov    dword ptr [esi + 0x104], ecx
loc_1_4D0D:
        cmp    ebx, dword ptr [esp + 0x10]
        jne    loc_1_4D30
        push   edi
        call   sub_1_2A58
        add    esp, 4
        mov    edi, dword ptr [esi + 0xfc]
        lea    ebp, [esi + 0x100]
        mov    ebx, dword ptr [ebp]
        jmp    loc_1_4D70
        db  08Dh, 049h, 000h
loc_1_4D30:
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_40F0
        add    esp, 8
        mov    edi, eax
        lea    ebp, [esi + 0x100]
        sub    ebx, dword ptr [esp + 0x10]
        add    ebx, dword ptr [ebp]
        lea    eax, [esi + 0xfc]
        mov    dword ptr [esp + 0x1c], eax
        cmp    dword ptr [esi + 0xfc], 0
        je     loc_1_4D70
        push   edi
        call   sub_1_40DC
        add    esp, 4
        mov    ecx, dword ptr [esi + 0xfc]
        mov    dword ptr [eax], ecx
loc_1_4D70:
        or     edi, edi
        mov    dword ptr [esi + 0xfc], 0
        mov    dword ptr [ebp], 0
        jne    loc_1_4B21
        jmp    loc_1_4DB3
        db  090h
loc_1_4D8C:
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [eax], ecx
loc_1_4D96:
        push   edi
        call   sub_1_40DC
        add    esp, 4
        mov    ecx, dword ptr [esi + 0xfc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0xfc], edi
        add    dword ptr [esi + 0x100], ebx
loc_1_4DB3:
        mov    al, byte ptr [esi + 0x82]
        and    dword ptr [esi + 0x58], 0xffffbfff
        dec    al
        mov    byte ptr [esi + 0x82], al
        jne    loc_1_4DCE
        and    dword ptr [esi + 0x5c], 0xfffffffe
loc_1_4DCE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
        db  08Bh, 0FFh
sub_1_4DD8:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx + 0xa0]
        mov    eax, esi
        or     eax, eax
        je     loc_1_4DF6
loc_1_4DE9:
        or     dword ptr [eax + 0x14], 0x80000000
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_4DE9
loc_1_4DF6:
        mov    ecx, dword ptr [edx + 0xf8]
        mov    eax, ecx
        or     eax, eax
        je     loc_1_4E0F
loc_1_4E02:
        or     dword ptr [eax + 0x14], 0x80000000
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_4E02
loc_1_4E0F:
        or     esi, esi
        jne    loc_1_4E17
        or     ecx, ecx
        je     loc_1_4E25
loc_1_4E17:
        push   0x100
        push   edx
        call   sub_1_2388
        add    esp, 8
loc_1_4E25:
        pop    esi
        ret    
        db  090h
sub_1_4E28:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x24]
        or     edi, edi
        mov    esi, dword ptr [esp + 0x20]
        je     loc_1_4E94
        lea    eax, [esi + 0x10]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_4E78
        mov    dword ptr [ebx + 0xc], esi
        mov    dword ptr [ebx + 4], 3
        lea    eax, [ebx + 0x10]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx], 0
        push   dword ptr [esp + 0x1c]
        push   esi
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_1E94
        add    esp, 0x10
        mov    dword ptr [edi], ebx
        jmp    loc_1_4E94
        db  08Bh, 0FFh
loc_1_4E78:
        mov    dword ptr [edi], 0
        mov    edi, dword ptr [esp + 0x28]
        or     edi, edi
        je     loc_1_4EF2
        mov    dword ptr [edi], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_4E94:
        mov    edi, dword ptr [esp + 0x28]
        or     edi, edi
        je     loc_1_4EF2
        mov    ebx, dword ptr [esp + 0x18]
        sub    ebx, dword ptr [esp + 0x1c]
        sub    ebx, esi
        lea    eax, [ebx + 0x10]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_4EEC
        mov    dword ptr [ebp + 0xc], ebx
        mov    dword ptr [ebp + 4], 3
        lea    eax, [ebp + 0x10]
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp], 0
        mov    ecx, dword ptr [esp + 0x1c]
        add    ecx, esi
        push   ecx
        push   ebx
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_1E94
        add    esp, 0x10
        mov    dword ptr [edi], ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_4EEC:
        mov    dword ptr [edi], 0
loc_1_4EF2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_4EF8:
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x48]
        mov    ecx, dword ptr [esi + 0x58]
        test   ch, 0x20
        je     loc_1_4F3C
        mov    edi, dword ptr [esp + 0x4c]
        mov    ebp, dword ptr [edi + 0xc]
        or     ebp, ebp
        je     loc_1_4F2C
        mov    eax, dword ptr [esp + 0x54]
        cmp    dword ptr [eax], ebp
        jb     loc_1_4F2C
        mov    dword ptr [esp + 0x14], 1
        dec    ebp
        jmp    loc_1_4F58
        db  08Dh, 049h, 000h
loc_1_4F2C:
        pop    ebp
        and    dword ptr [edi + 0x10], 0xffffdfff
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  090h
loc_1_4F3C:
        sub    ebp, ebp
        mov    edi, dword ptr [esp + 0x4c]
        mov    edx, dword ptr [edi + 0xc]
        inc    edx
        mov    eax, dword ptr [esp + 0x54]
        mov    eax, dword ptr [eax]
        sub    edx, eax
        sbb    ebx, ebx
        and    edx, ebx
        add    eax, edx
        mov    dword ptr [esp + 0x14], eax
loc_1_4F58:
        mov    eax, dword ptr [esp + 0x54]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [esp + 0x14]
        sub    eax, ebp
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [edi]
        add    eax, ebp
        mov    dword ptr [esp + 0x1c], eax
        mov    edx, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x28], edx
        lea    eax, [edx + eax - 1]
        mov    dword ptr [esp + 0x18], eax
        test   ecx, 0x4000000
        jne    loc_1_5120
        mov    dword ptr [esp + 0x10], 0
        test   byte ptr [esi + 0x59], 0x80
        je     loc_1_5058
        mov    edx, dword ptr [esi + 0x40]
        mov    ebx, dword ptr [esi + 0x108]
        mov    eax, edx
        sub    eax, ebx
        jns    loc_1_5058
        mov    ebp, dword ptr [esi + 0x10c]
        mov    ecx, ebp
        sub    ecx, ebx
        inc    ecx
        mov    eax, dword ptr [esp + 0x54]
        mov    eax, dword ptr [eax]
        sub    ebx, edx
        cmp    ebx, eax
        jl     loc_1_4FCA
        mov    ebx, eax
loc_1_4FCA:
        sub    edx, ebp
        lea    ebp, [edx + eax]
        or     ebp, ebp
        jne    loc_1_4FD5
        sub    ebp, ebp
loc_1_4FD5:
        or     ebp, ebp
        lea    edx, [esp + 0x10]
        jne    loc_1_4FDF
        sub    edx, edx
loc_1_4FDF:
        push   edx
        push   0
        push   ecx
        push   ebx
        push   eax
        push   dword ptr [esp + 0x64]
        call   sub_1_4E28
        add    esp, 0x18
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_5041
        mov    eax, dword ptr [esi + 0x10c]
        inc    eax
        mov    dword ptr [esp + 0x30], eax
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esp + 0x40], eax
        mov    eax, dword ptr [esp + 0x18]
        sub    eax, dword ptr [esp + 0x30]
        mov    dword ptr [esp + 0x3c], eax
        test   byte ptr [esi + 0x59], 0x20
        je     loc_1_5020
        inc    dword ptr [esp + 0x3c]
loc_1_5020:
        push   ebp
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 0x38], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esp + 0x3c], eax
        push   dword ptr [esp + 0x14]
        lea    eax, [esp + 0x38]
        push   eax
        push   esi
        call   sub_1_2C28
        add    esp, 0x10
loc_1_5041:
        mov    eax, dword ptr [esp + 0x54]
        mov    dword ptr [eax], ebx
        and    dword ptr [edi + 0x10], 0xffffdfff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_5058:
        lea    eax, [esp + 0x10]
        cmp    dword ptr [esp + 0x24], 0
        jne    loc_1_5065
        sub    eax, eax
loc_1_5065:
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x1c]
        push   ebp
        mov    eax, dword ptr [esp + 0x64]
        push   dword ptr [eax]
        push   dword ptr [esp + 0x64]
        call   sub_1_4E28
        add    esp, 0x18
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_50C5
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [esp + 0x28]
        add    eax, ebp
        mov    dword ptr [esp + 0x30], eax
        mov    eax, dword ptr [edi + 0x10]
        and    eax, 0xffffdfff
        mov    dword ptr [esp + 0x40], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 0x34], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esp + 0x38], eax
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x14]
        lea    eax, [esp + 0x38]
        push   eax
        push   esi
        call   sub_1_2C28
        add    esp, 0x10
loc_1_50C5:
        cmp    dword ptr [esp + 0x20], 0
        je     loc_1_5110
        mov    eax, dword ptr [esi + 0x58]
        test   ah, 0x80
        jne    loc_1_50E6
        or     eax, 0x8000
        mov    dword ptr [esi + 0x58], eax
        inc    byte ptr [esi + 0x82]
        or     dword ptr [esi + 0x5c], 1
loc_1_50E6:
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0x108], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x10c], eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x28]
        push   esi
        call   sub_1_4A90
        add    esp, 0x10
loc_1_5110:
        mov    eax, dword ptr [esp + 0x54]
        mov    dword ptr [eax], ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_5120:
        and    dword ptr [edi + 0x10], 0xffffdfff
        test   ch, 0x80
        je     loc_1_5144
        mov    eax, dword ptr [esp + 0x18]
        sub    eax, dword ptr [esi + 0x10c]
        or     eax, eax
        jg     loc_1_5157
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_1_5144:
        or     ecx, 0x8000
        mov    dword ptr [esi + 0x58], ecx
        inc    byte ptr [esi + 0x82]
        or     dword ptr [esi + 0x5c], 1
loc_1_5157:
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0x108], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x10c], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
        db  090h
loc_1_5174:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        push   dword ptr [esi]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_53DC
        mov    edi, dword ptr [eax]
        or     edi, edi
        je     loc_1_53CC
        mov    ebp, dword ptr [esp + 0x1c]
        mov    bx, word ptr [ebp]
        mov    al, byte ptr [edi + 0x84]
        cmp    al, 4
        jb     loc_1_51B1
        cmp    al, 6
        jbe    loc_1_51D5
loc_1_51B1:
        cmp    dword ptr [edi + 0xa8], 0
        je     loc_1_51BF
        test   bl, 0x20
        jne    loc_1_51D5
loc_1_51BF:
        cmp    dword ptr [edi + 0x100], 0
        je     loc_1_53BC
        test   bl, 0x40
        je     loc_1_53BC
loc_1_51D5:
        test   byte ptr [edi + 0x86], 1
        jne    loc_1_53BC
        call   sub_1_40A0
        or     eax, eax
        je     loc_1_53AC
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [eax + 0x20], ecx
        mov    ecx, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x18], ebp
        mov    dword ptr [eax + 0x10], 0
        mov    dword ptr [eax + 0xc], 0
        movzx  ecx, bx
        mov    dword ptr [eax + 0x14], ecx
        mov    cx, bx
        and    cl, 0x60
        cmp    cl, 0x40
        je     loc_1_5348
        mov    dword ptr [eax], 0
        cmp    dword ptr [edi + 0xa0], 0
        jne    loc_1_5248
        mov    dword ptr [edi + 0xa0], eax
        jmp    loc_1_5250
loc_1_5248:
        mov    ecx, dword ptr [edi + 0xa4]
        mov    dword ptr [ecx], eax
loc_1_5250:
        mov    dword ptr [edi + 0xa4], eax
        test   bl, 0x40
        je     loc_1_5294
        cmp    dword ptr [edi + 0x100], 0
        je     loc_1_5294
        inc    dword ptr [edi + 0x74]
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_4A90
        add    esp, 0x10
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_63D8
        add    esp, 8
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_5294:
        cmp    dword ptr [edi + 0x50], 0
        jne    loc_1_5338
        mov    dword ptr [edi + 0x50], eax
        test   byte ptr [edi + 0x58], 0x80
        jne    loc_1_5338
        mov    dword ptr [edi + 0x60], 0x4354 ; -> sub_1_4354
        mov    eax, dword ptr [edi + 0xac]
        or     eax, eax
        jne    loc_1_52CC
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_52CC:
        mov    ecx, dword ptr [edi + 0xa8]
        mov    dword ptr [edi + 0xac], 0
        mov    dword ptr [edi + 0xa8], 0
        inc    dword ptr [edi + 0x74]
        push   ecx
        push   eax
        push   0x800
        push   edi
        call   sub_1_4354
        add    esp, 0x10
        mov    esi, dword ptr [edi + 0x44]
        push   edi
        call   sub_1_7DDC
        add    esp, 4
        cmp    eax, esi
        je     loc_1_5317
        push   0x200
        push   edi
        call   sub_1_2388
        add    esp, 8
loc_1_5317:
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_63D8
        add    esp, 8
        call   sub_1_22C8
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_5338:
        pop    ebp
        mov    eax, 0xff
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_5348:
        lea    ecx, [edi + 0xf8]
        cmp    dword ptr [ecx], 0
        je     loc_1_535B
loc_1_5354:
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [ecx], 0
        jne    loc_1_5354
loc_1_535B:
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], eax
        cmp    dword ptr [edi + 0xfc], 0
        je     loc_1_539C
        inc    dword ptr [edi + 0x74]
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_4A90
        add    esp, 0x10
        push   dword ptr [esp + 0x10]
        push   edi
        call   sub_1_63D8
        add    esp, 8
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_539C:
        pop    ebp
        mov    eax, 0xff
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_53AC:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_53BC:
        pop    ebp
        mov    eax, 0x19
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_53CC:
        pop    ebp
        mov    eax, 0x19
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_53DC:
        pop    ebp
        mov    eax, 0xf
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_53EC:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 4]
        test   byte ptr [edx + 0x86], 1
        je     loc_1_540C
        mov    esi, dword ptr [esp + 0xc]
        mov    dword ptr [esi], 1
        jmp    loc_1_541A
        db  08Dh, 049h, 000h
loc_1_540C:
        movzx  eax, byte ptr [edx + 0x84]
        inc    eax
        mov    esi, dword ptr [esp + 0xc]
        mov    dword ptr [esi], eax
loc_1_541A:
        mov    eax, dword ptr [edx + 0x68]
        mov    dword ptr [esi + 4], eax
        movzx  eax, word ptr [edx + 0x6e]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edx + 0x64]
        mov    dword ptr [esi + 0xc], eax
        movzx  eax, word ptr [edx + 0x6c]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     loc_1_5444
        mov    dword ptr [ecx + 4], eax
        mov    eax, 1
        pop    esi
        ret    
loc_1_5444:
        mov    edx, dword ptr [ecx]
        inc    edx
        cmp    edx, 0x20
        jae    loc_1_5464
        inc    eax
        lea    eax, [edx*4 + 0x94bc]         ; -> obj1:0x94BC
loc_1_5454:
        cmp    dword ptr [eax], 0
        jne    loc_1_5478
        add    eax, 4
        inc    edx
        cmp    eax, 0x953c                   ; -> obj1:0x953C
        jb     loc_1_5454
loc_1_5464:
        sub    eax, eax
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_5478:
        pop    esi
        mov    eax, dword ptr [edx*4 + 0x94bc] ; -> obj1:0x94BC
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], edx
        mov    eax, 1
        ret    
        db  090h
loc_1_548C:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    loc_1_54D8
        or     eax, eax
        jne    loc_1_54D8
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 1
        inc    eax
        lea    eax, [esi*4 + 0x94bc]         ; -> obj1:0x94BC
loc_1_54B0:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jne    loc_1_54C5
        add    eax, 4
        inc    esi
        cmp    eax, 0x953c                   ; -> obj1:0x953C
        jb     loc_1_54B0
        or     ecx, ecx
        je     loc_1_54D4
loc_1_54C5:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_54D4:
        sub    eax, eax
        pop    esi
        ret    
loc_1_54D8:
        cmp    esi, 0x20
        jae    loc_1_54F2
        mov    ecx, dword ptr [esi*4 + 0x94bc] ; -> obj1:0x94BC
        or     ecx, ecx
        je     loc_1_54F2
loc_1_54E8:
        cmp    ecx, eax
        je     loc_1_5500
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_1_54E8
loc_1_54F2:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        ret    
loc_1_5500:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 1
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_1_5514:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax*4 + 0x94bc] ; -> obj1:0x94BC
        or     eax, eax
        je     loc_1_5532
        inc    ecx
        mov    ecx, dword ptr [esp + 8]
loc_1_5528:
        cmp    eax, ecx
        ja     loc_1_5532
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_5528
loc_1_5532:
        ret    
        db  090h
sub_1_5534:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, eax
        mov    ecx, dword ptr [esp + 0xc]
        sub    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 8], eax
        cmp    dword ptr [ecx + 0xc], eax
        je     loc_1_5552
        and    dword ptr [ecx + 0x58], 0xefffffff
loc_1_5552:
        mov    al, byte ptr [ecx + 0x84]
        cmp    al, 3
        jbe    loc_1_55AF
        cmp    al, 0xa
        je     loc_1_55AF
        mov    esi, dword ptr [ecx + 0x38]
        mov    eax, esi
        sub    eax, ecx
        cmp    eax, 0x38
        je     loc_1_55AF
        or     edx, edx
        je     loc_1_557D
loc_1_5570:
        mov    eax, dword ptr [esi + 0x20]
        cmp    edx, eax
        jb     loc_1_557D
        sub    edx, eax
        mov    esi, dword ptr [esi]
        jne    loc_1_5570
loc_1_557D:
        mov    dword ptr [ecx + 0x28], esi
        mov    eax, dword ptr [esi + 0x20]
        sub    eax, edx
        mov    dword ptr [ecx + 0x34], eax
        mov    eax, dword ptr [esi + 0x28]
        mov    esi, dword ptr [esi + 0x24]
        or     edx, edx
        je     loc_1_55A6
loc_1_5592:
        mov    edi, dword ptr [eax + 0xc]
        sub    edi, esi
        cmp    edx, edi
        jb     loc_1_55A6
        sub    edx, edi
        mov    esi, 0
        mov    eax, dword ptr [eax]
        jne    loc_1_5592
loc_1_55A6:
        mov    dword ptr [ecx + 0x2c], eax
        lea    eax, [edx + esi]
        mov    dword ptr [ecx + 0x30], eax
loc_1_55AF:
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_55B4:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x18], 0
        inc    dword ptr [0x966c]            ; -> loc_1_966C
        mov    dword ptr [esp + 0x10], 0
loc_1_55D1:
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax*4 + 0x94bc] ; -> obj1:0x94BC
        or     esi, esi
        je     loc_1_59A6
loc_1_55E4:
        lea    eax, [esi + 0x84]
        mov    dword ptr [esp + 0x1c], eax
        mov    cl, byte ptr [eax]
        or     cl, cl
        je     loc_1_599C
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_599C
        mov    ax, word ptr [esi + 0x90]
        or     ax, ax
        je     loc_1_57EC
        dec    ax
        mov    word ptr [esi + 0x90], ax
        jne    loc_1_57EC
        mov    al, byte ptr [esi + 0x85]
        inc    al
        mov    byte ptr [esi + 0x85], al
        cmp    cl, 2
        jne    loc_1_5640
        mov    ebx, dword ptr [0x964c]       ; -> loc_1_964C
        jmp    loc_1_5646
        db  090h
loc_1_5640:
        mov    ebx, dword ptr [0x9654]       ; -> loc_1_9654
loc_1_5646:
        and    eax, 0xff
        cmp    eax, ebx
        ja     loc_1_590C
        mov    dword ptr [esi + 0x8c], 0
        movzx  eax, word ptr [esi + 0x72]
        add    eax, eax
        cmp    eax, 0x960
        jl     loc_1_566F
        mov    eax, 0x960
loc_1_566F:
        push   dword ptr [esi + 4]
        mov    word ptr [esi + 0x72], ax
        push   esi
        call   sub_1_5534
        add    esp, 8
        lea    edi, [esi + 0x58]
        mov    eax, dword ptr [edi]
        test   eax, 0x80000
        jne    loc_1_56B8
        movzx  ecx, word ptr [esi + 0x70]
        mov    edx, dword ptr [esi + 0x10]
        mov    eax, dword ptr [esi + 0x1c]
        sub    edx, eax
        sbb    ebp, ebp
        and    edx, ebp
        add    edx, eax
        shr    edx, 1
        lea    eax, [ecx*2]
        sub    edx, eax
        sbb    ebp, ebp
        not    ebp
        and    edx, ebp
        add    edx, eax
        mov    dword ptr [esi + 0x20], edx
        mov    dword ptr [esi + 0x1c], ecx
        jmp    loc_1_56BF
loc_1_56B8:
        or     eax, 0x400000
        mov    dword ptr [edi], eax
loc_1_56BF:
        cmp    dword ptr [0x9698], 0         ; -> loc_1_9698
        je     loc_1_576E
        lea    eax, [ebx + 1]
        shr    eax, 1
        movzx  ecx, byte ptr [esi + 0x85]
        cmp    eax, ecx
        jne    loc_1_576E
        mov    ebp, dword ptr [edi]
        mov    edx, ebp
        and    edx, 0x1000
        jne    loc_1_5700
        test   byte ptr [esi + 0xef], 2
        je     loc_1_576E
        mov    eax, dword ptr [esi + 0xc]
        sub    eax, dword ptr [esi + 4]
        cmp    eax, 8
        jle    loc_1_576E
loc_1_5700:
        mov    al, byte ptr [esi + 0xd9]
        lea    ecx, [eax + 1]
        mov    byte ptr [esi + 0xd9], cl
        cmp    al, 2
        jae    loc_1_5740
        or     edx, edx
        jne    loc_1_5729
        or     ebp, 0x1000
        mov    dword ptr [edi], ebp
        inc    byte ptr [esi + 0x82]
        or     dword ptr [esi + 0x5c], 1
loc_1_5729:
        mov    al, byte ptr [esi + 0x85]
        and    byte ptr [esi + 0xef], 0xfd
        dec    al
        mov    byte ptr [esi + 0x85], al
        jmp    loc_1_576E
loc_1_5740:
        or     edx, edx
        je     loc_1_5760
        and    ebp, 0xffffefff
        mov    dword ptr [edi], ebp
        mov    al, byte ptr [esi + 0x82]
        dec    al
        mov    byte ptr [esi + 0x82], al
        jne    loc_1_5760
        and    dword ptr [esi + 0x5c], 0xfffffffe
loc_1_5760:
        or     byte ptr [esi + 0xef], 2
        mov    byte ptr [esi + 0xd9], 0
loc_1_576E:
        cmp    dword ptr [0x9664], 0         ; -> loc_1_9664
        je     loc_1_5794
        movzx  eax, byte ptr [esi + 0x85]
        inc    ebx
        shr    ebx, 1
        cmp    eax, ebx
        jne    loc_1_5794
        push   dword ptr [esi + 0x68]
        push   dword ptr [esi + 0x64]
        call   sub_1_95B4
        add    esp, 8
loc_1_5794:
        mov    eax, dword ptr [esp + 0x1c]
        movzx  eax, byte ptr [eax]
        sub    eax, 2
        cmp    eax, 8
        ja     loc_1_57EC
        jmp    dword ptr [eax*4 + 0x5a08]    ; -> obj1:0x5A08
        db  08Bh, 0FFh
loc_1_57AC:
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_7E10
        add    esp, 8
        push   esi
        push   dword ptr [esp + 0x14]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    loc_1_599E
        db  08Dh, 049h, 000h
loc_1_57D0:
        or     dword ptr [edi], 0x10000
loc_1_57D6:
        push   0x400
        mov    dword ptr [esp + 0x1c], 1
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_57EC:
        mov    ax, word ptr [esi + 0xda]
        or     ax, ax
        je     loc_1_5820
        dec    ax
        mov    word ptr [esi + 0xda], ax
        jne    loc_1_5820
        or     dword ptr [esi + 0x58], 0x400000
        mov    dword ptr [esp + 0x18], 1
        push   0x400
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_5820:
        mov    al, byte ptr [esi + 0x83]
        or     al, al
        je     loc_1_5845
        dec    al
        mov    byte ptr [esi + 0x83], al
        jne    loc_1_5845
        push   esi
        call   sub_1_4DD8
        add    esp, 4
        mov    dword ptr [esp + 0x18], 1
loc_1_5845:
        mov    ax, word ptr [esi + 0x92]
        or     ax, ax
        je     loc_1_5872
        dec    ax
        mov    word ptr [esi + 0x92], ax
        jne    loc_1_5872
        mov    dword ptr [esp + 0x18], 1
        push   0x200
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_5872:
        mov    eax, dword ptr [esp + 0x1c]
        cmp    byte ptr [eax], 4
        jne    loc_1_595B
        test   byte ptr [esi + 0x5b], 2
        je     loc_1_595B
        mov    ecx, dword ptr [0x966c]       ; -> loc_1_966C
        sub    ecx, dword ptr [esi + 0x98]
        cmp    ecx, dword ptr [0x961c]       ; -> loc_1_961C
        jbe    loc_1_5954
        sub    ecx, dword ptr [0x961c]       ; -> loc_1_961C
        movzx  eax, byte ptr [esi + 0x87]
        mov    edx, dword ptr [0x95fc]       ; -> loc_1_95FC
        imul   edx, eax
        cmp    edx, ecx
        jae    loc_1_595B
        cmp    dword ptr [0x9654], eax       ; -> loc_1_9654
        jbe    loc_1_590C
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_7FBC
        add    esp, 8
        push   esi
        push   dword ptr [esp + 0x14]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    loc_1_599E
loc_1_58E8:
        push   dword ptr [esp + 0x14]
        push   1
        push   esi
        call   sub_1_6398
        add    esp, 0xc
        push   esi
        push   dword ptr [esp + 0x14]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    loc_1_599E
        db  090h
loc_1_590C:
        inc    dword ptr [esi + 0x74]
        push   dword ptr [esp + 0x14]
        push   0x20
        push   esi
        call   sub_1_6398
        add    esp, 0xc
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   0x15
        push   0
        push   esi
        call   sub_1_6FB8
        add    esp, 0xc
        push   dword ptr [esp + 0x20]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        push   esi
        push   dword ptr [esp + 0x14]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    loc_1_599E
        db  08Bh, 0FFh
loc_1_5954:
        mov    byte ptr [esi + 0x87], 0
loc_1_595B:
        test   dword ptr [esi + 0x58], 0x20010008
        je     loc_1_599C
        mov    ecx, dword ptr [esi + 0xb4]
        mov    ax, word ptr [ecx + 0x1a]
        or     ax, ax
        je     loc_1_599C
        dec    ax
        mov    word ptr [ecx + 0x1a], ax
        jne    loc_1_599C
        push   dword ptr [esp + 0x14]
        push   0x20
        push   esi
        call   sub_1_6398
        add    esp, 0xc
        push   esi
        push   dword ptr [esp + 0x14]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    loc_1_599E
        db  090h
loc_1_599C:
        mov    esi, dword ptr [esi]
loc_1_599E:
        or     esi, esi
        jne    loc_1_55E4
loc_1_59A6:
        inc    dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x10], 0x20
        jb     loc_1_55D1
        mov    eax, dword ptr [0x953c]       ; -> obj1:0x953C
        cmp    dword ptr [0x966c], eax       ; -> loc_1_966C
        mov    esi, 0
        jne    loc_1_59D3
        mov    esi, 1
        add    dword ptr [0x953c], 0xa       ; -> obj1:0x953C
loc_1_59D3:
        push   0
        push   0x55b4                        ; -> loc_1_55B4
        push   0x64
        push   0x962c                        ; -> loc_1_962C
        call   sub_1_90D8
        add    esp, 0x10
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_59F5
        call   sub_1_22C8
loc_1_59F5:
        or     esi, esi
        je     loc_1_59FE
        call   sub_1_23C8
loc_1_59FE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh
        ; ---- dispatch table (9 entries) ----
        dd  loc_1_57AC
        dd  loc_1_57AC
        dd  loc_1_57D6
        dd  loc_1_57D0
        dd  loc_1_57EC
        dd  loc_1_57D6
        dd  loc_1_57D0
        dd  loc_1_57D0
        dd  loc_1_58E8
loc_1_5A2C:
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [ecx + 0x64], eax
        jne    loc_1_5A8A
        cmp    dword ptr [ecx + 0x68], edx
        jne    loc_1_5A8A
        test   byte ptr [ecx + 0xef], 2
        je     loc_1_5A8A
        mov    eax, dword ptr [esp + 0x14]
        mov    esi, dword ptr [eax]
        movzx  eax, byte ptr [ecx + 0xec]
        sub    esi, eax
        movzx  eax, word ptr [ecx + 0x80]
        sub    eax, esi
        sbb    edx, edx
        and    eax, edx
        add    ax, si
        mov    word ptr [ecx + 0x70], ax
        and    eax, 0xffff
        cmp    dword ptr [ecx + 0x1c], eax
        jae    loc_1_5A8A
        mov    dword ptr [ecx + 0x1c], eax
        add    eax, eax
        cmp    dword ptr [ecx + 0x20], eax
        jae    loc_1_5A8A
        mov    dword ptr [ecx + 0x20], eax
loc_1_5A8A:
        pop    esi
        mov    eax, 1
        ret    
        db  08Dh, 049h, 000h
loc_1_5A94:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x68], ecx
        jne    loc_1_5AA8
        sub    eax, eax
        ret    
        db  08Bh, 0FFh
loc_1_5AA8:
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
sub_1_5AB0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0x14]
loc_1_5AC1:
        mov    ebp, dword ptr [ebx*4 + 0x94bc] ; -> obj1:0x94BC
        or     ebp, ebp
        je     loc_1_5B3B
loc_1_5ACC:
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   ebp
        call   dword ptr [esp + 0x2c]
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_5B34
        test   byte ptr [ebp + 0x86], 1
        jne    loc_1_5B25
        inc    dword ptr [ebp + 0x74]
        push   esi
        push   0x40
        push   ebp
        call   sub_1_6398
        add    esp, 0xc
        push   ebp
        call   sub_1_60F4
        add    esp, 4
        cmp    byte ptr [ebp + 0x84], 0xa
        je     loc_1_5B1B
        push   0x13
        push   0
        push   ebp
        call   sub_1_6FB8
        add    esp, 0xc
loc_1_5B1B:
        push   edi
        push   ebp
        call   sub_1_63D8
        add    esp, 8
loc_1_5B25:
        push   ebp
        push   ebx
        call   sub_1_5514
        add    esp, 8
        mov    ebp, eax
        jmp    loc_1_5B37
        db  090h
loc_1_5B34:
        mov    ebp, dword ptr [ebp]
loc_1_5B37:
        or     ebp, ebp
        jne    loc_1_5ACC
loc_1_5B3B:
        inc    ebx
        cmp    ebx, 0x20
        jb     loc_1_5AC1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
sub_1_5B50:
        push   esi
        push   edi
        mov    eax, dword ptr [0x9618]       ; -> loc_1_9618
        or     eax, eax
        mov    edx, dword ptr [esp + 0xc]
        je     loc_1_5B88
        cmp    dword ptr [eax + 0x68], edx
        jne    loc_1_5B88
        mov    ecx, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x64], ecx
        mov    cx, word ptr [esp + 0x14]
        jne    loc_1_5B8D
        cmp    word ptr [eax + 0x6c], cx
        mov    si, word ptr [esp + 0x18]
        jne    loc_1_5B92
        cmp    word ptr [eax + 0x6e], si
        jne    loc_1_5B92
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_5B88:
        mov    cx, word ptr [esp + 0x14]
loc_1_5B8D:
        mov    si, word ptr [esp + 0x18]
loc_1_5B92:
        movzx  edi, byte ptr [esp + 0x12]
        movzx  eax, byte ptr [esp + 0x13]
        add    edi, eax
        movzx  eax, byte ptr [esp + 0x11]
        add    edi, eax
        movzx  eax, byte ptr [esp + 0x10]
        add    edi, eax
        and    edi, 0x1f
        mov    eax, dword ptr [edi*4 + 0x94bc] ; -> obj1:0x94BC
        or     eax, eax
        je     loc_1_5BE5
loc_1_5BBA:
        mov    edi, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x64], edi
        jne    loc_1_5BD4
        cmp    dword ptr [eax + 0x68], edx
        jne    loc_1_5BD4
        cmp    word ptr [eax + 0x6c], cx
        jne    loc_1_5BD4
        cmp    word ptr [eax + 0x6e], si
        je     loc_1_5BE0
loc_1_5BD4:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_5BBA
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_5BE0:
        mov    dword ptr [0x9618], eax       ; -> loc_1_9618
loc_1_5BE5:
        pop    edi
        pop    esi
        ret    
sub_1_5BE8:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        movzx  eax, byte ptr [esi + 0x66]
        movzx  edi, byte ptr [esi + 0x65]
        add    edi, eax
        movzx  eax, byte ptr [esi + 0x67]
        add    edi, eax
        movzx  eax, byte ptr [esi + 0x64]
        add    edi, eax
        and    edi, 0x1f
        mov    ax, word ptr [esi + 0x6e]
        push   eax
        mov    ax, word ptr [esi + 0x6c]
        push   eax
        push   dword ptr [esi + 0x64]
        push   dword ptr [esi + 0x68]
        call   sub_1_5B50
        add    esp, 0x10
        or     eax, eax
        je     loc_1_5C28
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_5C28:
        lea    ecx, [edi*4 + 0x94bc]         ; -> obj1:0x94BC
        cmp    dword ptr [ecx], 0
        je     loc_1_5C41
loc_1_5C34:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        jbe    loc_1_5C41
        mov    ecx, eax
        cmp    dword ptr [ecx], 0
        jne    loc_1_5C34
loc_1_5C41:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx], esi
        or     dword ptr [esi + 0x58], 0x80000000
        inc    dword ptr [0x9578]            ; -> obj1:0x9578
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
sub_1_5C5C:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 0x58]
        test   edx, 0x80000000
        je     loc_1_5CAF
        movzx  esi, byte ptr [ecx + 0x64]
        movzx  eax, byte ptr [ecx + 0x65]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 0x66]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 0x67]
        add    esi, eax
        and    esi, 0x1f
        lea    esi, [esi*4 + 0x94bc]         ; -> obj1:0x94BC
loc_1_5C8C:
        mov    eax, dword ptr [esi]
        cmp    ecx, eax
        je     loc_1_5C9C
        mov    esi, eax
        or     esi, esi
        jne    loc_1_5C8C
        jmp    loc_1_5CAF
        db  08Bh, 0FFh
loc_1_5C9C:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        and    edx, 0x7fffffff
        mov    dword ptr [ecx + 0x58], edx
        dec    dword ptr [0x9578]            ; -> obj1:0x9578
loc_1_5CAF:
        cmp    dword ptr [0x9618], ecx       ; -> loc_1_9618
        jne    loc_1_5CC1
        mov    dword ptr [0x9618], 0         ; -> loc_1_9618
loc_1_5CC1:
        pop    esi
        ret    
        db  090h
sub_1_5CC4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_90E4
        sub    edx, edx
        sub    esi, esi
        mov    ecx, 0x94bc                   ; -> obj1:0x94BC
loc_1_5CD9:
        mov    ebx, ecx
        mov    edi, dword ptr [ecx]
        or     edi, edi
        je     loc_1_5D1F
loc_1_5CE1:
        cmp    byte ptr [edi + 0x84], 0xa
        jne    loc_1_5D17
        cmp    dword ptr [edi + 0x74], 0
        jne    loc_1_5D17
        test   byte ptr [edi + 0x86], 1
        jne    loc_1_5D17
        or     esi, esi
        je     loc_1_5D09
        mov    ebp, eax
        sub    ebp, dword ptr [edi + 0x98]
        cmp    ebp, edx
        jbe    loc_1_5D17
loc_1_5D09:
        mov    esi, edi
        mov    dword ptr [esp + 0x10], ebx
        mov    edx, eax
        sub    edx, dword ptr [edi + 0x98]
loc_1_5D17:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_5CE1
loc_1_5D1F:
        add    ecx, 4
        cmp    ecx, 0x953c                   ; -> obj1:0x953C
        jb     loc_1_5CD9
        or     esi, esi
        je     loc_1_5D64
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ecx
        and    dword ptr [esi + 0x58], 0x7fffffff
        push   dword ptr [esi + 0xf0]
        call   sub_1_95A4
        add    esp, 4
        dec    dword ptr [0x9578]            ; -> obj1:0x9578
        cmp    dword ptr [0x9618], esi       ; -> loc_1_9618
        jne    loc_1_5D64
        mov    dword ptr [0x9618], 0         ; -> loc_1_9618
loc_1_5D64:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_5D70:
        push   edi
        mov    edx, dword ptr [0x9310]       ; -> obj1:0x9310
        or     edx, edx
        je     loc_1_5D84
        mov    eax, dword ptr [edx]
        mov    dword ptr [0x9310], eax       ; -> obj1:0x9310
        jmp    loc_1_5DC6
loc_1_5D84:
        mov    eax, dword ptr [0x931c]       ; -> loc_1_931C
        cmp    eax, dword ptr [0x9314]       ; -> obj1:0x9314
        ja     loc_1_5D9C
        call   sub_1_5CC4
        mov    edx, eax
        or     edx, edx
        jne    loc_1_5DC6
loc_1_5D9C:
        mov    eax, dword ptr [0x9318]       ; -> obj1:0x9318
        cmp    eax, dword ptr [0x9314]       ; -> obj1:0x9314
        jbe    loc_1_5E00
        push   0x110
        call   sub_1_90F6
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    loc_1_5DC0
        mov    eax, edx
        pop    edi
        ret    
loc_1_5DC0:
        inc    dword ptr [0x9314]            ; -> obj1:0x9314
loc_1_5DC6:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x44
        rep stosd dword ptr es:[edi], eax
        lea    eax, [edx + 0x38]
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 0x3c], eax
        mov    dword ptr [edx + 0x28], 0
        mov    eax, dword ptr [0x966c]       ; -> loc_1_966C
        mov    dword ptr [edx + 0x98], eax
        mov    byte ptr [edx + 0x82], 1
        mov    dword ptr [edx + 0x5c], 0x1001
        mov    eax, edx
        pop    edi
        ret    
        db  08Dh, 049h, 000h
loc_1_5E00:
        sub    eax, eax
        pop    edi
        ret    
sub_1_5E04:
        mov    eax, dword ptr [0x931c]       ; -> loc_1_931C
        cmp    eax, dword ptr [0x9314]       ; -> obj1:0x9314
        jae    loc_1_5E24
        dec    dword ptr [0x9314]            ; -> obj1:0x9314
        push   dword ptr [esp + 4]
        call   sub_1_90FC
        add    esp, 4
        ret    
loc_1_5E24:
        mov    eax, dword ptr [0x9310]       ; -> obj1:0x9310
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x9310], ecx       ; -> obj1:0x9310
        ret    
        db  08Bh, 0FFh
loc_1_5E38:
        ret    
        db  08Dh, 049h, 000h
loc_1_5E3C:
        ret    
        db  08Dh, 049h, 000h
sub_1_5E40:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_5E5E
        push   esi
        push   eax
        call   sub_1_64A8
        add    esp, 8
loc_1_5E5E:
        push   esi
        call   sub_1_90FC
        add    esp, 4
        push   0
        push   0
        push   ebx
        call   edi
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_5E78:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [esi + 0x18]
        test   byte ptr [esi + 0xd], 1
        mov    word ptr [esi + 0xe], 1
        jne    loc_1_5EB0
        mov    eax, dword ptr [esi + 4]
        cmp    eax, ebp
        je     loc_1_5EA8
        push   esi
        push   eax
        call   sub_1_64A8
        add    esp, 8
loc_1_5EA8:
        and    byte ptr [esi + 0xd], 0xfd
        jmp    loc_1_5EB5
        db  08Bh, 0FFh
loc_1_5EB0:
        mov    ebp, 1
loc_1_5EB5:
        or     ebp, ebp
        mov    word ptr [esi + 0xe], 0
        je     loc_1_5ED4
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_5E40
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_5ED4:
        push   0
        push   0
        push   ebx
        call   edi
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_1_5EE8:
        mov    ecx, dword ptr [0x96f4]       ; -> loc_1_96F4
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x96f4], eax       ; -> loc_1_96F4
        ret    
        db  08Bh, 0FFh
sub_1_5EFC:
        mov    eax, dword ptr [0x96f4]       ; -> loc_1_96F4
        or     eax, eax
        je     loc_1_5F10
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x96f4], ecx       ; -> loc_1_96F4
        ret    
        db  08Bh, 0FFh
loc_1_5F10:
        mov    eax, dword ptr [0x95f4]       ; -> loc_1_95F4
        cmp    dword ptr [0x9320], eax       ; -> obj1:0x9320
        mov    eax, 0
        jbe    loc_1_5F2C
        push   0x1c
        call   sub_1_90F6
        add    esp, 4
loc_1_5F2C:
        or     eax, eax
        je     loc_1_5F36
        inc    dword ptr [0x95f4]            ; -> loc_1_95F4
loc_1_5F36:
        ret    
        db  090h
sub_1_5F38:
        mov    edx, dword ptr [esp + 4]
        mov    eax, edx
        and    eax, 0xffffff
        cmp    dword ptr [0x9628], eax       ; -> loc_1_9628
        jbe    loc_1_5F64
        mov    ecx, dword ptr [0x9324]       ; -> obj1:0x9324
        mov    eax, dword ptr [ecx + eax*4]
        or     eax, eax
        je     loc_1_5F66
        shr    edx, 0x18
        cmp    byte ptr [eax + 0xc], dl
        je     loc_1_5F66
        sub    eax, eax
        ret    
        db  090h
loc_1_5F64:
        sub    eax, eax
loc_1_5F66:
        ret    
        db  090h
sub_1_5F68:
        push   ebx
        push   esi
        push   edi
        push   ebp
loc_1_5F6C:
        mov    edx, dword ptr [0x9324]       ; -> obj1:0x9324
        mov    ebx, dword ptr [0x9628]       ; -> loc_1_9628
loc_1_5F78:
        sub    eax, eax
        mov    esi, dword ptr [0x9688]       ; -> loc_1_9688
        cmp    ebx, eax
        je     loc_1_5F96
loc_1_5F84:
        cmp    ebx, esi
        jne    loc_1_5F8A
        sub    esi, esi
loc_1_5F8A:
        cmp    dword ptr [edx + esi*4], 0
        je     loc_1_5F96
        inc    esi
        inc    eax
        cmp    eax, ebx
        jb     loc_1_5F84
loc_1_5F96:
        cmp    eax, ebx
        mov    dword ptr [0x9324], edx       ; -> obj1:0x9324
        mov    dword ptr [0x9628], ebx       ; -> loc_1_9628
        jb     loc_1_602C
        cmp    ebx, dword ptr [0x95e8]       ; -> loc_1_95E8
        je     loc_1_606C
        add    ebx, 0x10
        mov    eax, dword ptr [0x95e8]       ; -> loc_1_95E8
        sub    ebx, eax
        sbb    ecx, ecx
        and    ebx, ecx
        add    ebx, eax
        lea    eax, [ebx*4]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        mov    ebp, dword ptr [0x9324]       ; -> obj1:0x9324
        mov    esi, dword ptr [0x9628]       ; -> loc_1_9628
        je     loc_1_6060
        lea    ecx, [ebx*4]
        mov    edi, edx
        sub    eax, eax
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        lea    ecx, [esi*4]
        mov    edi, edx
        mov    esi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x9324], edx       ; -> obj1:0x9324
        mov    dword ptr [0x9628], ebx       ; -> loc_1_9628
        cmp    ebp, eax
        je     loc_1_5F78
        push   ebp
        call   sub_1_90FC
        add    esp, 4
        jmp    loc_1_5F6C
        db  08Bh, 0FFh
loc_1_602C:
        mov    dword ptr [0x9324], edx       ; -> obj1:0x9324
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [edx + esi*4], eax
        lea    ecx, [esi + 1]
        mov    dword ptr [0x9688], ecx       ; -> loc_1_9688
        mov    cl, byte ptr [0x9690]         ; -> loc_1_9690
        inc    cl
        mov    byte ptr [0x9690], cl         ; -> loc_1_9690
        mov    byte ptr [eax + 0xc], cl
        movzx  eax, cl
        shl    eax, 0x18
        or     eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_6060:
        pop    ebp
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_606C:
        pop    ebp
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_6078:
        mov    ecx, dword ptr [esp + 4]
        and    ecx, 0xffffff
        mov    eax, dword ptr [0x9324]       ; -> obj1:0x9324
        mov    dword ptr [eax + ecx*4], 0
        ret    
        db  090h
sub_1_6090:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0x2afa
        je     loc_1_60B8
        cmp    eax, 0x2afb
        je     loc_1_60C0
        cmp    eax, 0x2afc
        je     loc_1_60C8
        cmp    eax, 0x2afd
        je     loc_1_60D0
        mov    eax, dword ptr [esp + 8]
        ret    
        db  08Dh, 049h, 000h
loc_1_60B8:
        mov    eax, 0x1b
        ret    
        db  08Bh, 0FFh
loc_1_60C0:
        mov    eax, 0x1c
        ret    
        db  08Bh, 0FFh
loc_1_60C8:
        mov    eax, 0x1d
        ret    
        db  08Bh, 0FFh
loc_1_60D0:
        mov    eax, 0x1e
        ret    
        db  08Bh, 0FFh
sub_1_60D8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0xcc]
        or     ecx, ecx
        je     loc_1_60F0
        cmp    dword ptr [ecx], eax
        jne    loc_1_60F0
        mov    dword ptr [ecx], 0
loc_1_60F0:
        ret    
        db  08Dh, 049h, 000h
sub_1_60F4:
        push   dword ptr [esp + 4]
        call   sub_1_60D8
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_6104:
        sub    esp, 4
        push   esi
        sub    edx, edx
        mov    ecx, dword ptr [esp + 0xc]
        mov    esi, dword ptr [ecx + 0xcc]
        cmp    esi, edx
        je     loc_1_6131
        mov    ax, word ptr [esi + 0xe]
        dec    ax
        mov    word ptr [esi + 0xe], ax
        jne    loc_1_6127
        mov    edx, dword ptr [esi + 0x1c]
loc_1_6127:
        mov    dword ptr [ecx + 0xcc], 0
loc_1_6131:
        or     edx, edx
        je     loc_1_613F
        push   dword ptr [esp + 4]
        push   esi
        call   edx
        add    esp, 8
loc_1_613F:
        pop    esi
        add    esp, 4
        ret    
sub_1_6144:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    bl, byte ptr [esi + 0x84]
        or     bl, bl
        je     loc_1_6393
        cmp    bl, 2
        je     loc_1_617C
        cmp    bl, 3
        je     loc_1_617C
        cmp    bl, 4
        je     loc_1_616E
        cmp    bl, 7
        jne    loc_1_6182
loc_1_616E:
        inc    dword ptr [0x955c]            ; -> obj1:0x955C
        dec    dword ptr [0x9560]            ; -> obj1:0x9560
        jmp    loc_1_6182
loc_1_617C:
        inc    dword ptr [0x9558]            ; -> obj1:0x9558
loc_1_6182:
        push   esi
        mov    byte ptr [esi + 0x84], 0
        call   sub_1_60D8
        add    esp, 4
        push   esi
        call   sub_1_5C5C
        add    esp, 4
        cmp    bl, 3
        jbe    loc_1_61B4
        cmp    bl, 9
        jae    loc_1_61B4
        test   byte ptr [esi + 0x5b], 0x40
        je     loc_1_61B4
        push   esi
        call   sub_1_81FC
        add    esp, 4
loc_1_61B4:
        lea    eax, [esi + 0xe4]
        push   eax
        call   sub_1_95C4
        add    esp, 4
        push   dword ptr [esi + 0xf0]
        call   sub_1_95A4
        add    esp, 4
        mov    al, byte ptr [esi + 0xd8]
        test   al, 0x80
        je     loc_1_61E4
        mov    edi, 0x14
        jmp    loc_1_621F
loc_1_61E4:
        test   al, 0x40
        je     loc_1_61F0
        mov    edi, 0x13
        jmp    loc_1_621F
        db  090h
loc_1_61F0:
        test   al, 0x20
        je     loc_1_61F8
        push   0x15
        jmp    loc_1_620F
loc_1_61F8:
        test   al, 0x10
        je     loc_1_6204
        mov    edi, 0xe
        jmp    loc_1_621F
        db  090h
loc_1_6204:
        test   al, 8
        mov    edi, 0
        je     loc_1_621F
        push   0x1c
loc_1_620F:
        push   dword ptr [esi + 0x88]
        call   sub_1_6090
        add    esp, 8
        mov    edi, eax
loc_1_621F:
        mov    ebx, dword ptr [esi + 0xb4]
        or     ebx, ebx
        je     loc_1_623E
        push   0
        push   edi
        push   dword ptr [ebx + 0xc]
        call   dword ptr [ebx + 8]
        add    esp, 0xc
        push   ebx
        call   sub_1_5EE8
        add    esp, 4
loc_1_623E:
        mov    ebx, dword ptr [esi + 0xf4]
        or     ebx, ebx
        je     loc_1_625D
        push   0
        push   edi
        push   dword ptr [ebx + 0xc]
        call   dword ptr [ebx + 8]
        add    esp, 0xc
        push   ebx
        call   sub_1_5EE8
        add    esp, 4
loc_1_625D:
        lea    ebx, [esi + 0x38]
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 0x38
        je     loc_1_62B8
loc_1_6269:
        mov    ebp, dword ptr [ebx]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        mov    eax, dword ptr [ebp + 0x18]
        dec    eax
        mov    dword ptr [ebp + 0x18], eax
        or     eax, eax
        jg     loc_1_62AC
        mov    eax, dword ptr [ebp + 0x2c]
        or     eax, eax
        je     loc_1_6292
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp + 0x2c], 0
loc_1_6292:
        push   0
        push   edi
        push   dword ptr [ebp + 0xc]
        call   dword ptr [ebp + 8]
        add    esp, 0xc
        push   ebp
        call   sub_1_7CD4
        add    esp, 4
        jmp    loc_1_62AF
        db  08Dh, 049h, 000h
loc_1_62AC:
        mov    dword ptr [ebp + 0x10], edi
loc_1_62AF:
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 0x38
        jne    loc_1_6269
loc_1_62B8:
        cmp    dword ptr [esi + 0xa0], 0
        je     loc_1_62ED
loc_1_62C1:
        mov    ebx, dword ptr [esi + 0xa0]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0xa0], eax
        push   0
        push   edi
        push   dword ptr [ebx + 8]
        call   dword ptr [ebx + 4]
        add    esp, 0xc
        push   ebx
        call   sub_1_408C
        add    esp, 4
        cmp    dword ptr [esi + 0xa0], 0
        jne    loc_1_62C1
loc_1_62ED:
        cmp    dword ptr [esi + 0xf8], 0
        je     loc_1_6322
loc_1_62F6:
        mov    ebx, dword ptr [esi + 0xf8]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0xf8], eax
        push   0
        push   edi
        push   dword ptr [ebx + 8]
        call   dword ptr [ebx + 4]
        add    esp, 0xc
        push   ebx
        call   sub_1_408C
        add    esp, 4
        cmp    dword ptr [esi + 0xf8], 0
        jne    loc_1_62F6
loc_1_6322:
        mov    eax, dword ptr [esi + 0xac]
        or     eax, eax
        je     loc_1_6335
        push   eax
        call   sub_1_2A58
        add    esp, 4
loc_1_6335:
        mov    eax, dword ptr [esi + 0xfc]
        or     eax, eax
        je     loc_1_6348
        push   eax
        call   sub_1_2A58
        add    esp, 4
loc_1_6348:
        cmp    dword ptr [esi + 0x9c], 0
        je     loc_1_6381
loc_1_6351:
        mov    edi, dword ptr [esi + 0x9c]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x9c], eax
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_636F
        push   eax
        call   sub_1_2A58
        add    esp, 4
loc_1_636F:
        push   edi
        call   sub_1_90FC
        add    esp, 4
        cmp    dword ptr [esi + 0x9c], 0
        jne    loc_1_6351
loc_1_6381:
        push   esi
        call   sub_1_6104
        add    esp, 4
        push   esi
        call   sub_1_5E04
        add    esp, 4
loc_1_6393:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_6398:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [esp + 8]
        or     byte ptr [eax + 0xd8], cl
        mov    cl, byte ptr [eax + 0x86]
        test   cl, 1
        jne    loc_1_63D7
        or     cl, 1
        mov    byte ptr [eax + 0x86], cl
        inc    byte ptr [eax + 0x82]
        or     dword ptr [eax + 0x5c], 1
        cmp    dword ptr [eax + 0x74], 0
        jne    loc_1_63D7
        push   dword ptr [esp + 0xc]
        push   eax
        call   sub_1_6144
        add    esp, 8
loc_1_63D7:
        ret    
sub_1_63D8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x74]
        dec    ecx
        mov    dword ptr [eax + 0x74], ecx
        jne    loc_1_6401
        mov    cl, byte ptr [eax + 0x86]
        or     cl, cl
        je     loc_1_6401
        test   cl, 1
        je     loc_1_6401
        push   dword ptr [esp + 8]
        push   eax
        call   sub_1_6144
        add    esp, 8
loc_1_6401:
        ret    
        db  08Bh, 0FFh
loc_1_6404:
        push   esi
        push   edi
        push   0x28
        call   sub_1_90F6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_64A0
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0xa
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x10], eax
        push   esi
        call   sub_1_5F68
        add    esp, 4
        cmp    eax, -1
        je     loc_1_648C
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], eax
        mov    word ptr [esi + 0xe], 0
        mov    byte ptr [esi + 0xd], 0
        cmp    dword ptr [0x9624], 1         ; -> loc_1_9624
        cmc    
        sbb    eax, eax
        and    eax, 0x2000
        or     eax, 0x20000
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [0x9668]       ; -> loc_1_9668
        or     eax, eax
        je     loc_1_6478
        mov    dword ptr [esi + 0x24], eax
        mov    byte ptr [esi + 0xd], 4
        jmp    loc_1_647F
loc_1_6478:
        mov    dword ptr [esi + 0x24], 0x2000
loc_1_647F:
        sub    eax, eax
        mov    dword ptr [esi + 0x1c], 0x5e38 ; -> loc_1_5E38
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_648C:
        push   esi
        call   sub_1_90FC
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_64A0:
        pop    edi
        mov    eax, 1
        pop    esi
        ret    
sub_1_64A8:
        mov    edx, dword ptr [esp + 4]
        add    edx, 0x44
        cmp    dword ptr [edx + 8], 0
        je     loc_1_64D9
        mov    ecx, dword ptr [esp + 8]
loc_1_64B9:
        mov    eax, dword ptr [edx + 8]
        cmp    ecx, eax
        je     loc_1_64CC
        mov    edx, eax
        cmp    dword ptr [edx + 8], 0
        jne    loc_1_64B9
        ret    
        db  08Dh, 049h, 000h
loc_1_64CC:
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [ecx + 4], 0
loc_1_64D9:
        ret    
        db  08Bh, 0FFh
loc_1_64DC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [edi]
        push   ebx
        call   sub_1_5F38
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_65A4
        push   ebx
        call   sub_1_6078
        add    esp, 4
        mov    cl, byte ptr [esi + 0xd]
        or     cl, 1
        mov    byte ptr [esi + 0xd], cl
        cmp    word ptr [esi + 0xe], 0
        je     loc_1_6584
        mov    edx, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 0x18], eax
        mov    dword ptr [esi + 0x1c], 0x5e40 ; -> sub_1_5E40
        test   cl, 2
        je     loc_1_653C
        push   0
        push   0x20
        push   ebx
        call   edx
        add    esp, 0xc
loc_1_653C:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_6578
        cmp    byte ptr [eax + 0x84], 0
        je     loc_1_6578
        or     dword ptr [eax + 0x58], 0x40000000
        test   byte ptr [eax + 0x86], 1
        jne    loc_1_656A
        push   dword ptr [esp + 0xc]
        push   0x40
        push   eax
        call   sub_1_6398
        add    esp, 0xc
loc_1_656A:
        pop    edi
        mov    eax, 0xff
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_6578:
        pop    edi
        mov    eax, 0xff
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_6584:
        mov    dword ptr [esi + 0x14], 0x5e3c ; -> loc_1_5E3C
        push   dword ptr [esp + 0xc]
        push   esi
        call   sub_1_5E40
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_65A4:
        pop    edi
        mov    eax, 0xf
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_65B0:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [eax]
        push   dword ptr [esp + 0x10]
        call   sub_1_01D4
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_1_65D4
        mov    eax, 6
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_65D4:
        push   esi
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_6604
        cmp    dword ptr [eax + 4], 0
        je     loc_1_65F0
        mov    eax, 0x10
        pop    edi
        pop    esi
        ret    
        db  090h
loc_1_65F0:
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [edi + 0x4c]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [edi + 0x4c], eax
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_6604:
        pop    edi
        mov    eax, 0xf
        pop    esi
        ret    
loc_1_660C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_6670
        mov    ecx, dword ptr [eax + 4]
        or     ecx, ecx
        je     loc_1_6668
        cmp    dword ptr [eax], 0
        jne    loc_1_6660
        cmp    word ptr [eax + 0xe], 0
        jne    loc_1_6640
        push   eax
        push   ecx
        call   sub_1_64A8
        add    esp, 8
        sub    eax, eax
        pop    esi
        ret    
loc_1_6640:
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 0x14], ecx
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0x1c], 0x5e78 ; -> loc_1_5E78
        or     byte ptr [eax + 0xd], 2
        mov    eax, 0xff
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_6660:
        pop    esi
        mov    eax, 0x12
        ret    
        db  090h
loc_1_6668:
        pop    esi
        mov    eax, 0x11
        ret    
        db  090h
loc_1_6670:
        pop    esi
        mov    eax, 0xf
        ret    
        db  090h
sub_1_6678:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_1_95B8
        add    esp, 4
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     loc_1_66C4
        mov    eax, dword ptr [ecx + 0xc]
        or     eax, eax
        je     loc_1_66C4
        push   esi
        push   dword ptr [ecx + 8]
        push   eax
        call   sub_1_95C0
        add    esp, 0xc
        or     eax, eax
        je     loc_1_66C0
        cmp    eax, 0x2afe
        jne    loc_1_66B8
        mov    eax, 1
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_66B8:
        pop    esi
        mov    eax, 0xb
        ret    
        db  090h
loc_1_66C0:
        sub    eax, eax
        pop    esi
        ret    
loc_1_66C4:
        sub    eax, eax
        pop    esi
        ret    
sub_1_66C8:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    cl, byte ptr [esi + 0xd]
        test   cl, 3
        je     loc_1_66E0
        mov    eax, 0xf
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_66E0:
        cmp    dword ptr [esi], 0
        jne    loc_1_67A4
        mov    edi, dword ptr [esi + 4]
        or     edi, edi
        je     loc_1_679C
        mov    eax, dword ptr [edi + 0x2c]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [edx + 0x68], eax
        mov    ax, word ptr [edi + 0x30]
        mov    word ptr [edx + 0x6e], ax
        mov    eax, dword ptr [edi + 0x7c]
        mov    dword ptr [edx + 0xdc], eax
        mov    edi, dword ptr [edi + 0x80]
        mov    dword ptr [edx + 0xe0], edi
        or     eax, eax
        mov    dword ptr [edx + 0x60], 0x42d4 ; -> loc_1_42D4
        je     loc_1_672D
        mov    dword ptr [edx + 0x60], 0x4620 ; -> loc_1_4620
loc_1_672D:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edx + 0xb8], eax
        mov    edi, dword ptr [esi + 0x20]
        or     edi, dword ptr [edx + 0x58]
        mov    dword ptr [edx + 0x58], edi
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [edx + 0x94], eax
        mov    dword ptr [edx + 0x44], eax
        test   cl, 4
        je     loc_1_6756
        or     edi, 1
        mov    dword ptr [edx + 0x58], edi
loc_1_6756:
        mov    ecx, dword ptr [edx + 0x58]
        test   ecx, 0x2000000
        je     loc_1_6773
        mov    eax, dword ptr [0x966c]       ; -> loc_1_966C
        mov    dword ptr [edx + 0x98], eax
        mov    byte ptr [edx + 0x87], 0
loc_1_6773:
        mov    eax, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_6787
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_6787
        or     ecx, 2
        mov    dword ptr [edx + 0x58], ecx
loc_1_6787:
        add    edx, 0xe4
        push   edx
        push   eax
        call   sub_1_6678
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_679C:
        pop    edi
        mov    eax, 0x11
        pop    esi
        ret    
loc_1_67A4:
        pop    edi
        mov    eax, 0x12
        pop    esi
        ret    
loc_1_67AC:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x24]
        mov    esi, dword ptr [ebp]
        cmp    dword ptr [esp + 0x2c], 0
        je     loc_1_6A74
        mov    eax, dword ptr [esp + 0x2c]
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_6A74
        lea    eax, [esp + 0x16]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   ecx
        call   sub_1_0580
        add    esp, 0xc
        or     al, al
        je     loc_1_6A74
        push   dword ptr [esp + 0x18]
        call   sub_1_95A8
        add    esp, 4
        mov    byte ptr [esp + 0x13], al
        cmp    al, 0xff
        je     loc_1_6A64
        test   al, 1
        jne    loc_1_6A64
        cmp    word ptr [esp + 0x16], 0
        je     loc_1_6A64
        call   sub_1_5EFC
        mov    edi, eax
        or     edi, edi
        jne    loc_1_6834
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_6834:
        call   sub_1_5D70
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_6858
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_6858:
        mov    eax, dword ptr [esp + 0x28]
        or     eax, eax
        je     loc_1_688C
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_688C
        mov    ecx, 0x64
        sub    edx, edx
        div    ecx
        inc    eax
        cmp    eax, 0xffff
        jbe    loc_1_6884
        mov    eax, 0xffff
        mov    word ptr [edi + 0x1a], ax
        jmp    loc_1_6892
        db  08Bh, 0FFh
loc_1_6884:
        inc    eax
        mov    word ptr [edi + 0x1a], ax
        jmp    loc_1_6892
        db  090h
loc_1_688C:
        mov    word ptr [edi + 0x1a], 0
loc_1_6892:
        mov    eax, dword ptr [esp + 0x30]
        mov    word ptr [edi + 0x18], 0
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ebx + 0x64], eax
        mov    ax, word ptr [esp + 0x16]
        mov    word ptr [ebx + 0x6c], ax
        push   esi
        call   sub_1_5F38
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_6A40
        push   dword ptr [esp + 0x2c]
        push   ebx
        push   esi
        call   sub_1_66C8
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_6A45
        or     dword ptr [ebx + 0x58], 8
        lea    eax, [ebx + 0xe4]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x1b]
        push   eax
        lea    eax, [ebx + 0xf0]
        push   eax
        push   dword ptr [ebx + 0x68]
        push   dword ptr [esp + 0x2c]
        call   sub_1_95A0
        add    esp, 0x18
        or     eax, eax
        jne    loc_1_6920
        mov    ebp, 0x1c
        jmp    loc_1_6A45
loc_1_6920:
        mov    dword ptr [esi], ebx
        inc    word ptr [esi + 0xe]
        mov    dword ptr [ebx + 0xcc], esi
        cmp    dword ptr [ebx + 0x68], 0
        jne    loc_1_6935
        mov    dword ptr [ebx + 0x68], eax
loc_1_6935:
        mov    eax, dword ptr [0x9600]       ; -> loc_1_9600
        sub    word ptr [esp + 0x14], 0x14
        or     eax, eax
        jne    loc_1_6978
        test   byte ptr [esp + 0x13], 0x10
        je     loc_1_6974
        lea    edx, [ebx + 0x70]
        mov    cx, word ptr [esp + 0x14]
        sub    cx, 0x218
        sbb    ax, ax
        and    cx, ax
        movzx  ax, byte ptr [ebx + 0xec]
        add    cx, 0x218
        sub    cx, ax
        mov    word ptr [edx], cx
        jmp    loc_1_6995
        db  090h
loc_1_6974:
        or     eax, eax
        je     loc_1_697F
loc_1_6978:
        mov    byte ptr [ebx + 0xef], 2
loc_1_697F:
        lea    edx, [ebx + 0x70]
        mov    ax, word ptr [esp + 0x14]
        movzx  cx, byte ptr [ebx + 0xec]
        sub    ax, cx
        mov    word ptr [edx], ax
loc_1_6995:
        push   ebx
        mov    ax, word ptr [edx]
        mov    word ptr [ebx + 0x80], ax
        call   sub_1_8494
        add    esp, 4
        mov    dword ptr [ebx + 0x74], 1
        mov    byte ptr [ebx + 0x84], 2
        inc    dword ptr [0x9550]            ; -> obj1:0x9550
        mov    dword ptr [ebx + 0xb4], edi
        push   ebx
        call   sub_1_5BE8
        add    esp, 4
        or     eax, eax
        jne    loc_1_6A04
        mov    dword ptr [ebx + 0xb4], 0
        dec    dword ptr [ebx + 0x74]
        push   dword ptr [esp + 0x1c]
        push   0x40
        push   ebx
        call   sub_1_6398
        add    esp, 0xc
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_6A04:
        test   byte ptr [ebx + 0x86], 1
        jne    loc_1_6A23
        cmp    byte ptr [ebx + 0x84], 2
        jne    loc_1_6A23
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   sub_1_7E10
        add    esp, 8
loc_1_6A23:
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   sub_1_63D8
        add    esp, 8
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_6A40:
        mov    ebp, 0xf
loc_1_6A45:
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_6A64:
        pop    ebp
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_6A74:
        pop    ebp
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_6A84:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [edi]
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_6AF4
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [eax + 0x14]
        or     ecx, ecx
        je     loc_1_6AF4
        lea    eax, [esp + 0x12]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   ecx
        call   sub_1_0580
        add    esp, 0xc
        or     al, al
        jne    loc_1_6AC8
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_6AC8:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_6B03
        push   dword ptr [esp + 0x14]
        call   sub_1_95A8
        add    esp, 4
        cmp    al, 0xff
        je     loc_1_6AE4
        test   al, 1
        je     loc_1_6B03
loc_1_6AE4:
        pop    ebp
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_6AF4:
        mov    dword ptr [esp + 0x14], 0
        mov    word ptr [esp + 0x12], 0
loc_1_6B03:
        call   sub_1_5EFC
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_6B1C
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_6B1C:
        call   sub_1_5D70
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_6B40
        push   ebp
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_6B40:
        mov    ax, word ptr [esp + 0x20]
        mov    word ptr [ebp + 0x18], ax
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebx + 0xb4], ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebx + 0x64], eax
        mov    ax, word ptr [esp + 0x12]
        mov    word ptr [ebx + 0x6c], ax
        mov    byte ptr [ebx + 0x84], 1
        push   esi
        call   sub_1_5F38
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_1_6BCC
        push   dword ptr [esp + 0x24]
        push   ebx
        push   esi
        call   sub_1_66C8
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        jne    loc_1_6BB8
        mov    dword ptr [esi], ebx
        mov    dword ptr [ebx + 0xcc], esi
        inc    word ptr [esi + 0xe]
        mov    edi, 0xff
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_6BB8:
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_6BCC:
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        mov    edi, 0xf
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_6BE4:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        lea    eax, [esi + 0xe4]
        push   eax
        lea    eax, [esp + 0xa]
        push   eax
        lea    eax, [esp + 0xd]
        push   eax
        lea    eax, [esi + 0xf0]
        push   eax
        push   dword ptr [esi + 0x68]
        push   dword ptr [esi + 0x64]
        call   sub_1_95A0
        add    esp, 0x18
        sub    word ptr [esp + 6], 0x14
        mov    eax, dword ptr [0x9600]       ; -> loc_1_9600
        or     eax, eax
        jne    loc_1_6C64
        test   byte ptr [esp + 5], 0x10
        je     loc_1_6C60
        mov    ax, word ptr [esp + 6]
        sub    ax, 0x218
        sbb    cx, cx
        and    ax, cx
        add    ax, 0x218
        movzx  ecx, ax
        movzx  eax, byte ptr [esi + 0xec]
        sub    ecx, eax
        movzx  eax, word ptr [esi + 0x80]
        cmp    ecx, eax
        jl     loc_1_6C55
        mov    ecx, eax
loc_1_6C55:
        mov    word ptr [esi + 0x70], cx
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_6C60:
        or     eax, eax
        je     loc_1_6C6B
loc_1_6C64:
        mov    byte ptr [esi + 0xef], 2
loc_1_6C6B:
        movzx  ax, byte ptr [esi + 0xec]
        sub    word ptr [esp + 6], ax
        mov    ax, word ptr [esi + 0x80]
        mov    cx, word ptr [esp + 6]
        sub    ax, cx
        sbb    dx, dx
        and    ax, dx
        add    ax, cx
        mov    word ptr [esi + 0x70], ax
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_6C9C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_1_6BE4
        add    esp, 4
        push   esi
        call   sub_1_8494
        add    esp, 4
        push   esi
        call   sub_1_2024
        add    esp, 4
        mov    edi, dword ptr [esp + 0x10]
        push   edi
        push   esi
        call   sub_1_7E10
        add    esp, 8
        push   edi
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_1_6CD8:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x24]
        mov    esi, dword ptr [edi]
        call   sub_1_5EFC
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_6D00
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_6D00:
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [ebx + 0x14], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 0xc], eax
        push   esi
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_6DB4
        mov    ebp, dword ptr [eax]
        or     ebp, ebp
        je     loc_1_6DB4
        test   byte ptr [ebp + 0x86], 1
        jne    loc_1_6DB4
        cmp    byte ptr [ebp + 0x84], 3
        jne    loc_1_6DB4
        test   byte ptr [ebp + 0x58], 4
        jne    loc_1_6DB4
        cmp    dword ptr [ebp + 0xb4], 0
        jne    loc_1_6DB4
        mov    eax, dword ptr [esp + 0x28]
        or     eax, eax
        je     loc_1_6D8A
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_6D8A
        lea    ecx, [esp + 0x14]
        push   ecx
        push   eax
        call   sub_1_6678
        add    esp, 8
        or     eax, eax
        jne    loc_1_6DB4
        lea    edi, [ebp + 0xe4]
        push   edi
        call   sub_1_95C4
        add    esp, 4
        lea    esi, [esp + 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        or     dword ptr [ebp + 0x58], 2
loc_1_6D8A:
        mov    dword ptr [ebp + 0xb4], ebx
        or     dword ptr [ebp + 0x58], 4
        inc    dword ptr [ebp + 0x74]
        push   dword ptr [esp + 0x10]
        push   ebp
        call   sub_1_6C9C
        add    esp, 8
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_1_6DB4:
        push   ebx
        mov    esi, 0xf
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_1_6DCC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x18]
        push   dword ptr [ebp]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_6F74
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     loc_1_6F74
        mov    bx, word ptr [esp + 0x20]
        test   bl, 2
        je     loc_1_6E40
        cmp    byte ptr [esi + 0x84], 0
        je     loc_1_6E30
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_6E26
        or     dword ptr [esi + 0x58], 0x40000000
        push   dword ptr [esp + 0x10]
        push   0x40
        push   esi
        call   sub_1_6398
        add    esp, 0xc
loc_1_6E26:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_6E30:
        pop    ebp
        mov    eax, 0x19
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_6E40:
        test   byte ptr [esi + 0x86], 1
        je     loc_1_6E58
        mov    eax, 0xf
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_6E58:
        call   sub_1_5EFC
        mov    edi, eax
        or     edi, edi
        je     loc_1_6F64
        mov    word ptr [edi + 0x18], 0
        mov    dword ptr [edi + 0x14], 0
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0xc], eax
        test   bl, 1
        jne    loc_1_6F30
        mov    eax, dword ptr [esp + 0x1c]
        or     eax, eax
        je     loc_1_6EBC
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_6EBC
        mov    ecx, 0x64
        sub    edx, edx
        div    ecx
        inc    eax
        cmp    eax, 0xffff
        jbe    loc_1_6EB4
        mov    eax, 0xffff
        mov    word ptr [edi + 0x1a], ax
        jmp    loc_1_6EC2
        db  090h
loc_1_6EB4:
        inc    eax
        mov    word ptr [edi + 0x1a], ax
        jmp    loc_1_6EC2
        db  090h
loc_1_6EBC:
        mov    word ptr [edi + 0x1a], 0
loc_1_6EC2:
        mov    al, byte ptr [esi + 0x84]
        cmp    al, 4
        jne    loc_1_6EE0
        mov    byte ptr [esi + 0x84], 5
        inc    byte ptr [esi + 0x82]
        or     dword ptr [esi + 0x5c], 1
        jmp    loc_1_6EEB
        db  090h
loc_1_6EE0:
        cmp    al, 7
        jne    loc_1_6F18
        mov    byte ptr [esi + 0x84], 9
loc_1_6EEB:
        push   esi
        dec    dword ptr [0x9560]            ; -> obj1:0x9560
        mov    dword ptr [esi + 0xb4], edi
        or     dword ptr [esi + 0x58], 0x10000
        inc    dword ptr [esi + 0x74]
        call   sub_1_8824
        add    esp, 4
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
loc_1_6F18:
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, 0x19
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_6F30:
        mov    word ptr [edi + 0x1a], 0
        cmp    dword ptr [esi + 0xf4], 0
        jne    loc_1_6F54
        mov    dword ptr [esi + 0xf4], edi
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_6F54:
        pop    ebp
        mov    eax, 0x19
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_6F64:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_1_6F74:
        pop    ebp
        mov    eax, 0xf
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_1_6F84:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0xa8], 0
        jne    loc_1_6FB4
        cmp    dword ptr [eax + 0x100], 0
        jne    loc_1_6FB4
        cmp    dword ptr [eax + 0xa0], 0
        jne    loc_1_6FB4
        cmp    dword ptr [eax + 0xf8], 0
        jne    loc_1_6FB4
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
loc_1_6FB4:
        sub    eax, eax
        ret    
        db  090h
sub_1_6FB8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        cmp    byte ptr [ebx + 0x84], 3
        jbe    loc_1_709D
        test   byte ptr [ebx + 0x58], 0x10
        jne    loc_1_709D
        mov    esi, dword ptr [esp + 0x1c]
        cmp    esi, 0x16
        jne    loc_1_6FFC
        push   ebx
        call   sub_1_6F84
        add    esp, 4
        or     eax, eax
        jne    loc_1_6FFC
        or     dword ptr [ebx + 0x58], 0x100000
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_1_6FFC:
        mov    eax, dword ptr [ebx + 0x58]
        or     eax, 0x10
        mov    dword ptr [ebx + 0x58], eax
        and    eax, 0xffefffff
        mov    dword ptr [ebx + 0x58], eax
        mov    edi, dword ptr [ebx + 0xf4]
        or     edi, edi
        je     loc_1_703C
        mov    dword ptr [ebx + 0xf4], 0
        push   0
        push   esi
        push   dword ptr [edi + 0xc]
        call   dword ptr [edi + 8]
        add    esp, 0xc
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_703C:
        mov    edx, dword ptr [ebx + 0xb8]
        mov    eax, dword ptr [ebx + 0xcc]
        or     eax, eax
        je     loc_1_709D
        mov    edi, dword ptr [eax + 4]
        or     edi, edi
        je     loc_1_709D
        mov    ecx, dword ptr [edi + 0x6c]
        mov    ebx, dword ptr [edi + 0x70]
        or     ecx, ecx
        je     loc_1_709D
        inc    word ptr [edi + 0x38]
        mov    eax, dword ptr [esp + 0x18]
        or     eax, eax
        je     loc_1_7074
        movzx  ebp, byte ptr [eax + 8]
        mov    eax, dword ptr [eax]
        jmp    loc_1_7078
        db  08Dh, 049h, 000h
loc_1_7074:
        sub    ebp, ebp
        sub    eax, eax
loc_1_7078:
        sub    esi, 0x16
        cmp    esi, 1
        sbb    esi, esi
        and    esi, 2
        add    esi, 2
        push   esi
        push   eax
        push   ebp
        push   0
        push   0
        push   edx
        push   ebx
        call   ecx
        add    esp, 0x1c
        push   edi
        call   sub_1_120C
        add    esp, 4
loc_1_709D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_70A4:
        push   esi
        cmp    dword ptr [esp + 0x10], 0
        mov    esi, dword ptr [esp + 8]
        je     loc_1_7120
        push   esi
        call   sub_1_6F84
        add    esp, 4
        or     eax, eax
        je     loc_1_70FC
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_70D8
        call   sub_1_90E4
        mov    dword ptr [esi + 0x98], eax
        mov    byte ptr [esi + 0x84], 0xa
        jmp    loc_1_70E7
loc_1_70D8:
        push   dword ptr [esp + 0x14]
        push   0
        push   esi
        call   sub_1_6398
        add    esp, 0xc
loc_1_70E7:
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   0x16
        push   0
        push   esi
        call   sub_1_6FB8
        jmp    loc_1_7166
loc_1_70FC:
        cmp    dword ptr [esp + 0xc], 1
        cmc    
        sbb    eax, eax
        and    eax, 0x200000
        or     eax, 0x1000000
        or     dword ptr [esi + 0x58], eax
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    esi
        ret    
loc_1_7120:
        mov    eax, dword ptr [esi + 0x58]
        test   al, 0x10
        je     loc_1_719C
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_7144
        call   sub_1_90E4
        mov    dword ptr [esi + 0x98], eax
        mov    byte ptr [esi + 0x84], 0xa
        jmp    loc_1_7153
        db  08Bh, 0FFh
loc_1_7144:
        push   dword ptr [esp + 0x14]
        push   0
        push   esi
        call   sub_1_6398
        add    esp, 0xc
loc_1_7153:
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   0
        push   0
        push   esi
        call   sub_1_24B0
loc_1_7166:
        add    esp, 0xc
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_718B
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_718B
        mov    word ptr [esi + 0x90], 0x960
        push   esi
        call   sub_1_6104
        add    esp, 4
loc_1_718B:
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_1_719C:
        cmp    dword ptr [esp + 0xc], 1
        cmc    
        sbb    ecx, ecx
        and    ecx, 0x200000
        or     ecx, eax
        or     ecx, 0x1000000
        mov    dword ptr [esi + 0x58], ecx
        push   0
        push   0
        push   esi
        call   sub_1_24B0
        add    esp, 0xc
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_71D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xe0
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 1
        je     loc_1_7214
        cmp    eax, 2
        je     loc_1_7220
        cmp    eax, 3
        je     loc_1_7290
        cmp    eax, 4
        je     loc_1_735C
        cmp    eax, 5
        je     loc_1_7478
        mov    eax, 0x1f
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_7214:
        pop    edi
        mov    eax, 0x1f
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_7220:
        mov    dword ptr [ebp - 0xe0], 0x100
        mov    dword ptr [ebp - 0xdc], 0xffffffff
        mov    dword ptr [ebp - 0xd8], 0
        mov    dword ptr [ebp - 0xd4], 0xfff7
        mov    dword ptr [ebp - 0xd0], 0x9af
        mov    dword ptr [ebp - 0xcc], 1
        mov    dword ptr [ebp - 0xc8], 0xffff
        mov    dword ptr [ebp - 0xc4], 0
        mov    eax, dword ptr [0x94b8]       ; -> obj1:0x94B8
        mov    dword ptr [ebp - 0xc0], eax
        mov    dword ptr [ebp - 0xbc], 0
        mov    esi, 0x28
        jmp    loc_1_7496
        db  090h
loc_1_7290:
        sub    eax, eax
        mov    esi, 0x1a
        lea    edi, [ebp - 0xe0]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebp - 0xe0], 1
        cmp    dword ptr [ebp + 0x18], eax
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax]
        je     loc_1_7318
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_730C
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     loc_1_72E8
        mov    ax, word ptr [ecx + 0x6e]
        push   eax
        push   dword ptr [ecx + 0x68]
        lea    eax, [ebp - 0xdc]
        push   eax
        call   sub_1_1F70
        add    esp, 0xc
        jmp    loc_1_7496
        db  08Bh, 0FFh
loc_1_72E8:
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_1_730C
        mov    cx, word ptr [eax + 0x30]
        push   ecx
        push   dword ptr [eax + 0x2c]
        lea    eax, [ebp - 0xdc]
        push   eax
        call   sub_1_1F70
        add    esp, 0xc
        jmp    loc_1_7496
        db  090h
loc_1_730C:
        pop    edi
        mov    eax, 0xf
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_7318:
        call   sub_1_01D4
        add    esp, 4
        or     eax, eax
        jne    loc_1_7330
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_7330:
        test   byte ptr [eax + 0x32], 0x80
        jne    loc_1_7340
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_7340:
        mov    cx, word ptr [eax + 0x30]
        push   ecx
        push   dword ptr [eax + 0x2c]
        lea    eax, [ebp - 0xdc]
        push   eax
        call   sub_1_1F70
        add    esp, 0xc
        jmp    loc_1_7496
loc_1_735C:
        mov    esi, 0x31
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_746C
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        je     loc_1_746C
        lea    edi, [ebp - 0xe0]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        movzx  eax, byte ptr [ebx + 0x84]
        mov    dword ptr [ebp - 0xe0], eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebx + 0x68]
        push   dword ptr [ebx + 0x64]
        call   sub_1_95B0
        add    esp, 0x10
        or     eax, eax
        je     loc_1_73CC
        mov    dword ptr [ebp - 0xc8], 0xffffffff
        mov    dword ptr [ebp - 0xc4], 0xffffffff
        jmp    loc_1_743D
loc_1_73CC:
        sub    edx, edx
        mov    dword ptr [ebp - 0xc4], 0
        mov    ecx, 0xa
        mov    eax, dword ptr [ebx + 0xc4]
        div    ecx
        or     eax, eax
        je     loc_1_7434
        mov    ecx, dword ptr [ebx + 0xbc]
        mov    dword ptr [ebp - 0x20], ecx
        mov    ecx, dword ptr [ebx + 0xc0]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x14], ecx
        mov    eax, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x10]
        div    dword ptr [ebp - 0xc]
        or     ecx, ecx
        je     loc_1_7420
        mov    dword ptr [ecx], edx
loc_1_7420:
        mov    ecx, dword ptr [ebp - 4]
        sub    ecx, eax
        sbb    edx, edx
        and    ecx, edx
        add    ecx, eax
        mov    dword ptr [ebp - 0xc8], ecx
        jmp    loc_1_743D
        db  090h
loc_1_7434:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc8], eax
loc_1_743D:
        mov    ax, word ptr [ebx + 0x7c]
        shr    ax, 2
        movzx  ecx, ax
        movzx  eax, word ptr [ebx + 0x7e]
        add    ecx, eax
        and    ecx, 0xfffffffe
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    dword ptr [ebp - 0xc0], ecx
        mov    dword ptr [ebp - 0xc4], 0
        jmp    loc_1_7496
        db  08Dh, 049h, 000h
loc_1_746C:
        pop    edi
        mov    eax, 0xf
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
loc_1_7478:
        sub    eax, eax
        lea    edi, [ebp - 0xe0]
        mov    ecx, 0x30
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [ebp - 0xe0], 0x100
        mov    esi, 0xc0
loc_1_7496:
        lea    edx, [ebp - 0xe0]
        mov    dword ptr [ebp - 8], 0
        mov    edi, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [edi]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, ebx
        sub    eax, esi
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, esi
        push   eax
        push   edx
        push   dword ptr [ebp + 0x10]
        call   sub_1_1EF8
        add    esp, 0x10
        cmp    ebx, esi
        jae    loc_1_74D4
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_74D4:
        mov    dword ptr [edi], esi
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_74E0:
        mov    eax, 0xfe
        ret    
        db  08Bh, 0FFh
loc_1_74E8:
        mov    eax, 0xfe
        ret    
        db  08Bh, 0FFh
sub_1_74F0:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x40]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 0x10], 0
        mov    ecx, dword ptr [esp + 0x38]
        mov    esi, dword ptr [ecx]
        or     esi, esi
        jne    loc_1_7584
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 8], 0x100
        jne    loc_1_7574
        cmp    dword ptr [ecx + 0xc], 0x100
        jne    loc_1_7574
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_1_7574
        mov    eax, dword ptr [0x973c]       ; -> loc_1_973C
        shl    eax, 3
        cmp    eax, dword ptr [esp + 0x14]
        jbe    loc_1_754C
        mov    eax, 0x21
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_754C:
        mov    edx, dword ptr [esp + 0x40]
        mov    dword ptr [edx], eax
        lea    ecx, [esp + 0x10]
        push   ecx
        push   eax
        push   dword ptr [0x9648]            ; -> loc_1_9648
        push   dword ptr [esp + 0x48]
        call   sub_1_1EF8
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_7574:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_7584:
        cmp    esi, 0x400
        je     loc_1_75B4
        cmp    esi, 0x401
        je     loc_1_75B4
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x44]
        push   dword ptr [esp + 0x44]
        push   ecx
        call   sub_1_95C8
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Bh, 0FFh
loc_1_75B4:
        cmp    dword ptr [ecx + 4], 0
        je     loc_1_75C8
        mov    eax, 0xfe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_75C8:
        mov    edx, dword ptr [esp + 0x40]
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 0x100
        jne    loc_1_764C
        cmp    dword ptr [ecx + 0xc], 0x100
        jne    loc_1_763C
        cmp    dword ptr [ecx + 0x10], 1
        jne    loc_1_763C
        cmp    dword ptr [esp + 0x14], 4
        jb     loc_1_762C
        cmp    esi, 0x400
        mov    eax, 0
        sete   al
        add    eax, 0x403
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 0x10]
        push   eax
        push   4
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x48]
        call   sub_1_1EF8
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_762C:
        pop    ebp
        mov    eax, 0x21
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_763C:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_764C:
        cmp    eax, 0x200
        jne    loc_1_785C
        mov    eax, dword ptr [ecx + 0xc]
        cmp    eax, 0x300
        jne    loc_1_7724
        cmp    dword ptr [esp + 0x14], 8
        jb     loc_1_7714
        cmp    dword ptr [ecx + 0x10], 5
        jne    loc_1_7714
        mov    eax, dword ptr [esp + 0x34]
        push   dword ptr [eax]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_7704
        mov    edx, dword ptr [eax]
        or     edx, edx
        je     loc_1_7704
        mov    eax, dword ptr [edx + 0x58]
        and    eax, 0x4008000
        cmp    eax, 0x4008000
        jne    loc_1_76C8
        mov    ecx, dword ptr [edx + 0x10c]
        mov    eax, ecx
        sub    eax, dword ptr [edx + 0x108]
        inc    eax
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [edx + 0xa8]
        sub    eax, dword ptr [edx + 0x40]
        add    eax, ecx
        mov    dword ptr [esp + 0x20], eax
        jmp    loc_1_76D8
        db  08Dh, 049h, 000h
loc_1_76C8:
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0
loc_1_76D8:
        mov    edx, dword ptr [esp + 0x40]
        mov    dword ptr [edx], 8
        lea    eax, [esp + 0x10]
        push   eax
        push   8
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x48]
        call   sub_1_1EF8
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_7704:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_7714:
        pop    ebp
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_7724:
        cmp    eax, 0x100
        je     loc_1_7738
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_7738:
        mov    eax, dword ptr [ecx + 0x10]
        cmp    eax, 1
        je     loc_1_7754
        cmp    eax, 0x101
        je     loc_1_777C
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_7754:
        mov    eax, 1
        cmp    esi, 0x401
        jne    loc_1_7770
        mov    esi, 0x14
        mov    edi, 0x957c                   ; -> obj1:0x957C
        jmp    loc_1_779E
        db  08Dh, 049h, 000h
loc_1_7770:
        mov    esi, 0x3c
        mov    edi, 0x9540                   ; -> obj1:0x9540
        jmp    loc_1_779E
loc_1_777C:
        sub    eax, eax
        cmp    esi, 0x401
        jne    loc_1_7794
        mov    esi, 8
        mov    edi, 0x9328                   ; -> loc_1_9328
        jmp    loc_1_779E
        db  08Bh, 0FFh
loc_1_7794:
        mov    esi, 0x14
        mov    edi, 0x9330                   ; -> loc_1_9330
loc_1_779E:
        or     eax, eax
        je     loc_1_77DC
        cmp    esi, dword ptr [esp + 0x14]
        jbe    loc_1_77B8
        mov    eax, 0x21
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_77B8:
        mov    edx, dword ptr [esp + 0x40]
        mov    dword ptr [edx], esi
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        push   edi
        push   dword ptr [esp + 0x48]
        call   sub_1_1EF8
        add    esp, 0x10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_77DC:
        sub    ebx, ebx
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x48]
        call   dword ptr [edi]
        add    esp, 8
        mov    ebp, eax
        cmp    dword ptr [esp + 0x18], ebx
        jne    loc_1_7804
        mov    eax, 0x1a
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  08Dh, 049h, 000h
loc_1_7804:
        or     ebp, ebp
        je     loc_1_7844
loc_1_7808:
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, ebx
        cmp    eax, esi
        jl     loc_1_7844
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [esp + 0x48]
        call   dword ptr [edi + 4]
        add    esp, 8
        mov    ebp, eax
        add    ebx, esi
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x48]
        call   sub_1_1EF8
        add    esp, 0x10
        mov    dword ptr [esp + 0x3c], eax
        or     ebp, ebp
        jne    loc_1_7808
loc_1_7844:
        cmp    ebp, 1
        mov    edx, dword ptr [esp + 0x40]
        mov    dword ptr [edx], ebx
        cmc    
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        db  090h
loc_1_785C:
        cmp    eax, 0x300
        mov    eax, 0x1a
        jne    loc_1_7870
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
loc_1_7870:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
sub_1_7878:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esi]
        cmp    eax, 0x400
        je     loc_1_78AC
        cmp    eax, 0x401
        je     loc_1_78AC
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   esi
        call   sub_1_95CC
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_78AC:
        cmp    dword ptr [esi + 4], 0
        je     loc_1_78C0
        mov    eax, 0xfe
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_78C0:
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 0x100
        jne    loc_1_78D8
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_78D8:
        cmp    eax, 0x200
        jne    loc_1_7B40
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 0x300
        jne    loc_1_7A38
        mov    edi, 0x1a
        cmp    dword ptr [esp + 0x24], 4
        jae    loc_1_7908
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_7908:
        mov    eax, dword ptr [esp + 0x18]
        push   dword ptr [eax]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_7A2D
        sub    edi, edi
        mov    edx, dword ptr [esi + 0x10]
        cmp    edx, 6
        jne    loc_1_79A0
        mov    ecx, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0xffff
        jbe    loc_1_7944
        mov    edi, 0x1a
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_7944:
        cmp    dword ptr [eax + 0x24], ecx
        jb     loc_1_794E
        cmp    dword ptr [eax], 0
        jne    loc_1_7995
loc_1_794E:
        mov    esi, dword ptr [eax]
        or     byte ptr [eax + 0xd], 4
        mov    dword ptr [eax + 0x24], ecx
        or     esi, esi
        je     loc_1_7995
        mov    al, byte ptr [esi + 0x84]
        cmp    al, 4
        jb     loc_1_7995
        cmp    al, 6
        ja     loc_1_7995
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_7995
        or     dword ptr [esi + 0x58], 1
        mov    dword ptr [esi + 0x94], ecx
        inc    dword ptr [esi + 0x74]
        push   esi
        call   sub_1_80C8
        add    esp, 4
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_63D8
        add    esp, 8
loc_1_7995:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_79A0:
        sub    esi, esi
        mov    ecx, dword ptr [esp + 0x20]
        mov    ebx, dword ptr [ecx]
        cmp    edx, 1
        je     loc_1_79C4
        cmp    edx, 2
        je     loc_1_79D4
        cmp    edx, 3
        je     loc_1_79DC
        cmp    edx, 4
        je     loc_1_79E4
        mov    edi, 0x1a
        jmp    loc_1_79E9
        db  090h
loc_1_79C4:
        cmp    ebx, 1
        sbb    ebx, ebx
        neg    ebx
        mov    esi, 0x20000
        jmp    loc_1_79E9
        db  08Bh, 0FFh
loc_1_79D4:
        mov    esi, 0x2000000
        jmp    loc_1_79E9
        db  090h
loc_1_79DC:
        mov    esi, 0x4000000
        jmp    loc_1_79E9
        db  090h
loc_1_79E4:
        mov    esi, 0x2000
loc_1_79E9:
        or     edi, edi
        jne    loc_1_7A2D
        or     ebx, ebx
        je     loc_1_79F8
        or     dword ptr [eax + 0x20], esi
        jmp    loc_1_79FF
        db  08Bh, 0FFh
loc_1_79F8:
        mov    ecx, esi
        not    ecx
        and    dword ptr [eax + 0x20], ecx
loc_1_79FF:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_7A2D
        or     ebx, ebx
        je     loc_1_7A10
        or     dword ptr [eax + 0x58], esi
        jmp    loc_1_7A15
        db  08Bh, 0FFh
loc_1_7A10:
        not    esi
        and    dword ptr [eax + 0x58], esi
loc_1_7A15:
        cmp    edx, 2
        jne    loc_1_7A2D
        mov    ecx, dword ptr [0x966c]       ; -> loc_1_966C
        mov    dword ptr [eax + 0x98], ecx
        mov    byte ptr [eax + 0x87], 0
loc_1_7A2D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_7A38:
        cmp    eax, 0x200
        jne    loc_1_7A60
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esi + 0x10]
        push   dword ptr [esp + 0x24]
        call   sub_1_0F70
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_7A60:
        cmp    eax, 0x100
        je     loc_1_7A74
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_7A74:
        cmp    dword ptr [esi + 0x10], 0x101
        jne    loc_1_7B34
        cmp    dword ptr [esp + 0x24], 0x14
        je     loc_1_7A94
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_7A94:
        mov    ecx, dword ptr [esp + 0x20]
        cmp    dword ptr [ecx], 0xc
        je     loc_1_7AAC
        mov    eax, 0x1a
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_1_7AAC:
        push   dword ptr [ecx + 8]
        push   dword ptr [ecx + 0x10]
        push   dword ptr [ecx + 0xc]
        push   dword ptr [ecx + 4]
        call   sub_1_5B50
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        je     loc_1_7B28
        inc    dword ptr [esi + 0x74]
        cmp    byte ptr [esi + 0x84], 0
        je     loc_1_7B10
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_7B10
        or     dword ptr [esi + 0x58], 0x40000000
        push   dword ptr [esp + 0xc]
        push   0x40
        push   esi
        call   sub_1_6398
        add    esp, 0xc
        cmp    byte ptr [esi + 0x84], 0xa
        je     loc_1_7B10
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   0x14
        push   0
        push   esi
        call   sub_1_6FB8
        add    esp, 0xc
loc_1_7B10:
        push   dword ptr [esp + 0xc]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_7B28:
        pop    edi
        mov    eax, 0x1a
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_7B34:
        pop    edi
        mov    eax, 0x1a
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_7B40:
        cmp    eax, 0x300
        mov    eax, 0xfe
        jne    loc_1_7B54
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
loc_1_7B54:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
sub_1_7B5C:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x9338]       ; -> loc_1_9338
        cmp    eax, dword ptr [0x9608]       ; -> loc_1_9608
        jbe    loc_1_7C74
        push   0xc
        call   sub_1_90F6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_7B90
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_7B90:
        push   0x10
        lea    eax, [esi + 4]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_91DE
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_7BBC
        push   esi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_7BBC:
        mov    edi, dword ptr [0x9594]       ; -> obj1:0x9594
        add    edi, 0x20
        mov    eax, edi
        shl    eax, 4
        push   eax
        call   sub_1_90F6
        add    esp, 4
        or     eax, eax
        jne    loc_1_7BF4
        push   dword ptr [esi + 4]
        call   sub_1_91E4
        push   esi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_7BF4:
        sub    ebp, ebp
        mov    dword ptr [esi + 8], eax
        add    dword ptr [0x9608], 0x10      ; -> loc_1_9608
        mov    ecx, dword ptr [0x968c]       ; -> loc_1_968C
        mov    dword ptr [esi], ecx
        mov    dword ptr [0x968c], esi       ; -> loc_1_968C
        sub    ebx, ebx
        mov    dword ptr [esp + 0x18], eax
loc_1_7C14:
        push   edi
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esi + 4]
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        call   sub_1_91EA
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_7C5E
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0xc], 0x14
        or     ebx, ebx
        je     loc_1_7C50
        push   dword ptr [esp + 0x10]
        call   sub_1_7CB8
        add    esp, 4
        jmp    loc_1_7C54
        db  090h
loc_1_7C50:
        mov    ebp, dword ptr [esp + 0x10]
loc_1_7C54:
        inc    ebx
        add    dword ptr [esp + 0x18], edi
        cmp    ebx, 0x10
        jb     loc_1_7C14
loc_1_7C5E:
        mov    eax, ebp
        sub    ebx, 0x10
        add    dword ptr [0x9608], ebx       ; -> loc_1_9608
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_7C74:
        mov    eax, dword ptr [0x96f0]       ; -> loc_1_96F0
        or     eax, eax
        je     loc_1_7C90
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x96f0], ecx       ; -> loc_1_96F0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_7C90:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
sub_1_7C9C:
        mov    eax, dword ptr [0x96f0]       ; -> loc_1_96F0
        or     eax, eax
        je     loc_1_7CB0
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x96f0], ecx       ; -> loc_1_96F0
        ret    
        db  08Bh, 0FFh
loc_1_7CB0:
        jmp    sub_1_7B5C
        db  08Dh, 049h, 000h
sub_1_7CB8:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0xc], 0x14
        mov    ecx, dword ptr [0x96f0]       ; -> loc_1_96F0
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x96f0], eax       ; -> loc_1_96F0
        ret    
        db  08Dh, 049h, 000h
sub_1_7CD4:
        mov    ecx, dword ptr [0x9670]       ; -> loc_1_9670
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x9670], eax       ; -> loc_1_9670
        ret    
        db  08Bh, 0FFh
sub_1_7CE8:
        mov    eax, dword ptr [0x9670]       ; -> loc_1_9670
        or     eax, eax
        je     loc_1_7CFC
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x9670], ecx       ; -> loc_1_9670
        ret    
        db  08Bh, 0FFh
loc_1_7CFC:
        mov    eax, dword ptr [0x9738]       ; -> loc_1_9738
        cmp    dword ptr [0x933c], eax       ; -> obj1:0x933C
        mov    eax, 0
        jbe    loc_1_7D18
        push   0x34
        call   sub_1_90F6
        add    esp, 4
loc_1_7D18:
        or     eax, eax
        je     loc_1_7D22
        inc    dword ptr [0x9738]            ; -> loc_1_9738
loc_1_7D22:
        ret    
        db  090h
sub_1_7D24:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        or     esi, esi
        je     loc_1_7DBC
        mov    eax, dword ptr [esp + 0x14]
        mov    edi, dword ptr [eax]
        mov    ax, word ptr [esi + 8]
        or     ax, ax
        je     loc_1_7D4D
        and    eax, 0xffff
        nop    
loc_1_7D48:
        mov    edi, dword ptr [edi]
        dec    eax
        jne    loc_1_7D48
loc_1_7D4D:
        sub    ebx, ebx
        cmp    word ptr [esi + 0xa], bx
        jbe    loc_1_7D68
loc_1_7D55:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_91F6
        inc    ebx
        movzx  eax, word ptr [esi + 0xa]
        cmp    eax, ebx
        ja     loc_1_7D55
loc_1_7D68:
        mov    edx, dword ptr [esi]
        sub    edi, edi
        cmp    dword ptr [esi + 4], edi
        jbe    loc_1_7DBC
loc_1_7D71:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 0x18]
        dec    eax
        mov    dword ptr [edx + 0x18], eax
        or     eax, eax
        jg     loc_1_7DB4
        mov    eax, dword ptr [edx + 0x2c]
        or     eax, eax
        je     loc_1_7D92
        mov    dword ptr [eax], 0
        mov    dword ptr [edx + 0x2c], 0
loc_1_7D92:
        mov    eax, dword ptr [0x9614]       ; -> loc_1_9614
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [0x9614]       ; -> loc_1_9614
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [0x9614], edx       ; -> loc_1_9614
        mov    dword ptr [edx], 0x9610       ; -> loc_1_9610
        or     dword ptr [0x9650], 2         ; -> loc_1_9650
loc_1_7DB4:
        mov    edx, ecx
        inc    edi
        cmp    dword ptr [esi + 4], edi
        ja     loc_1_7D71
loc_1_7DBC:
        push   dword ptr [esp + 0x14]
        call   sub_1_7CB8
        add    esp, 4
        test   byte ptr [0x9650], 2          ; -> loc_1_9650
        je     loc_1_7DD6
        call   sub_1_23C8
loc_1_7DD6:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_7DDC:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edx + 0x94]
        mov    esi, eax
        sub    esi, dword ptr [edx + 0xa8]
        movzx  edi, word ptr [edx + 0x70]
        shr    eax, 1
        sub    edi, eax
        sbb    ecx, ecx
        and    edi, ecx
        add    edi, eax
        mov    eax, esi
        sub    eax, dword ptr [edx + 0x44]
        cmp    edi, eax
        jg     loc_1_7E0A
        mov    dword ptr [edx + 0x44], esi
loc_1_7E0A:
        pop    edi
        mov    eax, dword ptr [edx + 0x44]
        pop    esi
        ret    
sub_1_7E10:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_7C9C
        mov    ebx, eax
        mov    esi, dword ptr [esp + 0x18]
        mov    ax, word ptr [esi + 0x72]
        mov    word ptr [esi + 0x90], ax
        or     ebx, ebx
        je     loc_1_7FB5
        mov    edi, dword ptr [ebx + 4]
        add    edi, dword ptr [0x9594]       ; -> obj1:0x9594
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 0xc], 0x18
        mov    ax, word ptr [esi + 0x6e]
        mov    word ptr [edi], ax
        mov    ax, word ptr [esi + 0x6c]
        mov    word ptr [edi + 2], ax
        mov    ecx, dword ptr [esi + 8]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        mov    eax, ecx
        shr    eax, 0x18
        or     edx, eax
        mov    dword ptr [edi + 4], edx
        inc    ecx
        mov    dword ptr [esi + 8], ecx
        sub    ecx, dword ptr [esi + 0xc]
        or     ecx, ecx
        jle    loc_1_7EA0
        inc    dword ptr [0x9568]            ; -> obj1:0x9568
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 0xc], eax
        jmp    loc_1_7EA6
        db  08Dh, 049h, 000h
loc_1_7EA0:
        inc    dword ptr [0x956c]            ; -> obj1:0x956C
loc_1_7EA6:
        mov    ecx, dword ptr [esi + 0x40]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [edi + 8], edx
        cmp    byte ptr [esi + 0x84], 3
        mov    word ptr [edi + 0xc], 0x1260
        je     loc_1_7EE4
        mov    word ptr [edi + 0xc], 0x260
loc_1_7EE4:
        mov    ax, word ptr [esi + 0x44]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edi + 0xe], cx
        mov    word ptr [edi + 0x10], 0
        lea    eax, [edi + 0x14]
        mov    dword ptr [esp + 0x10], eax
        lea    eax, [esp + 0xe]
        push   eax
        push   dword ptr [esi + 0x68]
        call   sub_1_95AC
        add    esp, 8
        or     al, al
        jne    loc_1_7F24
        push   ebx
        call   sub_1_7CB8
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_1_7F24:
        mov    eax, dword ptr [esp + 0x10]
        sub    word ptr [esp + 0xe], 0x14
        mov    byte ptr [eax], 2
        inc    dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], 4
        inc    dword ptr [esp + 0x10]
        mov    ah, byte ptr [esp + 0xe]
        mov    al, byte ptr [esp + 0xf]
        mov    ecx, dword ptr [esp + 0x10]
        mov    word ptr [ecx], ax
        inc    dword ptr [esi + 0x74]
        push   ebx
        mov    eax, dword ptr [esi + 0x24]
        add    eax, 0x1800
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [edi + 0x10], ax
        push   dword ptr [esi + 0xf0]
        lea    eax, [esi + 0xe4]
        push   eax
        push   dword ptr [esi + 0x68]
        push   dword ptr [esi + 0x64]
        push   0x18
        push   ebx
        push   0
        push   dword ptr [0x95d8]            ; -> loc_1_95D8
        call   sub_1_9598
        add    esp, 0x20
        mov    dword ptr [esi + 0x88], eax
        cmp    eax, 0x2bf7
        je     loc_1_7FA8
        push   ebx
        call   sub_1_7CB8
        add    esp, 4
loc_1_7FA8:
        push   dword ptr [esp + 0x1c]
        push   esi
        call   sub_1_63D8
        add    esp, 8
loc_1_7FB5:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_7FBC:
        push   ebx
        push   esi
        push   edi
        call   sub_1_7C9C
        mov    edi, eax
        or     edi, edi
        je     loc_1_80C3
        mov    ebx, dword ptr [edi + 4]
        add    ebx, dword ptr [0x9594]       ; -> obj1:0x9594
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 0xc], 0x15
        mov    esi, dword ptr [esp + 0x10]
        mov    ax, word ptr [esi + 0x6e]
        mov    word ptr [ebx], ax
        mov    ax, word ptr [esi + 0x6c]
        mov    word ptr [ebx + 2], ax
        mov    ecx, dword ptr [esi + 4]
        dec    ecx
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 4], edx
        inc    dword ptr [0x956c]            ; -> obj1:0x956C
        mov    ecx, dword ptr [esi + 0x40]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 8], edx
        mov    word ptr [ebx + 0xc], 0x1050
        push   esi
        call   sub_1_7DDC
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0xe], cx
        mov    word ptr [ebx + 0x10], 0
        push   edi
        mov    eax, dword ptr [esi + 0x24]
        add    eax, 0x1500
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 0x10], ax
        inc    byte ptr [esi + 0x87]
        push   dword ptr [esi + 0xf0]
        lea    eax, [esi + 0xe4]
        push   eax
        push   dword ptr [esi + 0x68]
        push   dword ptr [esi + 0x64]
        push   0x15
        push   edi
        push   0
        push   dword ptr [0x95d8]            ; -> loc_1_95D8
        call   sub_1_9598
        add    esp, 0x20
        cmp    eax, 0x2bf7
        je     loc_1_80C3
        push   edi
        call   sub_1_7CB8
        add    esp, 4
loc_1_80C3:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_1_80C8:
        push   ebx
        push   esi
        push   edi
        call   sub_1_7C9C
        mov    edi, eax
        or     edi, edi
        je     loc_1_81F8
        mov    ebx, dword ptr [edi + 4]
        add    ebx, dword ptr [0x9594]       ; -> obj1:0x9594
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [esp + 0x10]
        mov    ax, word ptr [esi + 0x6e]
        mov    word ptr [ebx], ax
        mov    ax, word ptr [esi + 0x6c]
        mov    word ptr [ebx + 2], ax
        mov    ecx, dword ptr [esi + 0x40]
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 8], edx
        cmp    dword ptr [esi + 0x10], 0
        je     loc_1_8130
        mov    ecx, dword ptr [esi + 8]
        jmp    loc_1_8133
loc_1_8130:
        mov    ecx, dword ptr [esi + 4]
loc_1_8133:
        test   byte ptr [esi + 0x5b], 0x20
        je     loc_1_8149
        mov    eax, dword ptr [esi + 0xc]
        sub    eax, ecx
        cmp    eax, 1
        mov    word ptr [ebx + 0xc], 0x1150
        je     loc_1_814F
loc_1_8149:
        mov    word ptr [ebx + 0xc], 0x1050
loc_1_814F:
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 4], edx
        push   esi
        call   sub_1_7DDC
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0xe], cx
        mov    word ptr [ebx + 0x10], 0
        push   edi
        mov    eax, dword ptr [esi + 0x24]
        add    eax, 0x1400
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 0x10], ax
        and    dword ptr [esi + 0x58], 0xfffff5ff
        mov    word ptr [esi + 0x92], 0
        inc    dword ptr [0x9568]            ; -> obj1:0x9568
        push   dword ptr [esi + 0xf0]
        lea    eax, [esi + 0xe4]
        push   eax
        push   dword ptr [esi + 0x68]
        push   dword ptr [esi + 0x64]
        push   0x14
        push   edi
        push   0
        push   dword ptr [0x95d8]            ; -> loc_1_95D8
        call   sub_1_9598
        add    esp, 0x20
        mov    dword ptr [esi + 0x88], eax
        cmp    eax, 0x2bf7
        je     loc_1_81F8
        push   edi
        call   sub_1_7CB8
        add    esp, 4
loc_1_81F8:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_81FC:
        push   ebx
        push   esi
        push   edi
        call   sub_1_7C9C
        mov    edi, eax
        or     edi, edi
        je     loc_1_82D2
        mov    ebx, dword ptr [edi + 4]
        add    ebx, dword ptr [0x9594]       ; -> obj1:0x9594
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [esp + 0x10]
        mov    ax, word ptr [esi + 0x6e]
        mov    word ptr [ebx], ax
        mov    ax, word ptr [esi + 0x6c]
        mov    word ptr [ebx + 2], ax
        cmp    dword ptr [esi + 0x10], 0
        je     loc_1_823C
        mov    ecx, dword ptr [esi + 0xc]
        jmp    loc_1_823F
        db  090h
loc_1_823C:
        mov    ecx, dword ptr [esi + 4]
loc_1_823F:
        mov    edx, ecx
        shl    edx, 0x10
        mov    eax, ecx
        and    eax, 0xff00
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [ebx + 4], edx
        mov    word ptr [ebx + 0xc], 0x450
        mov    word ptr [ebx + 0xe], 0
        mov    word ptr [ebx + 0x10], 0
        push   edi
        mov    eax, dword ptr [esi + 0x24]
        add    eax, 0x1400
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 0x10], ax
        inc    dword ptr [0x9568]            ; -> obj1:0x9568
        inc    dword ptr [0x9574]            ; -> obj1:0x9574
        push   dword ptr [esi + 0xf0]
        lea    eax, [esi + 0xe4]
        push   eax
        push   dword ptr [esi + 0x68]
        push   dword ptr [esi + 0x64]
        push   0x14
        push   edi
        push   0
        push   dword ptr [0x95d8]            ; -> loc_1_95D8
        call   sub_1_9598
        add    esp, 0x20
        cmp    eax, 0x2bf7
        je     loc_1_82D2
        push   edi
        call   sub_1_7CB8
        add    esp, 4
loc_1_82D2:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_82D8:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x20]
        test   byte ptr [ebx + 0xd], 4
        jne    loc_1_8454
        call   sub_1_7C9C
        mov    esi, eax
        or     esi, esi
        je     loc_1_8454
        mov    edi, dword ptr [esi + 4]
        add    edi, dword ptr [0x9594]       ; -> obj1:0x9594
        mov    dword ptr [esi], 0
        mov    ax, word ptr [ebx + 0xc]
        test   ah, 2
        mov    edx, dword ptr [esp + 0x24]
        je     loc_1_8319
        inc    edx
loc_1_8319:
        test   ah, 1
        je     loc_1_831F
        inc    edx
loc_1_831F:
        test   ah, 0x10
        je     loc_1_8334
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 4], eax
        mov    word ptr [edi + 0xc], 0x450
        jmp    loc_1_8390
        db  08Bh, 0FFh
loc_1_8334:
        mov    dword ptr [edi + 4], 0
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, eax
        and    ecx, 0xff00
        mov    ebp, eax
        shl    ebp, 0x10
        or     ecx, ebp
        shl    ecx, 8
        mov    ebp, eax
        and    ebp, 0xff0000
        shr    ebp, 8
        or     ecx, ebp
        shr    eax, 0x18
        or     ecx, eax
        add    ecx, edx
        mov    edx, ecx
        and    edx, 0xff00
        mov    eax, ecx
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0xff0000
        shr    eax, 8
        or     edx, eax
        shr    ecx, 0x18
        or     edx, ecx
        mov    dword ptr [edi + 8], edx
        mov    word ptr [edi + 0xc], 0x1450
loc_1_8390:
        mov    word ptr [edi + 0xe], 0
        mov    ax, word ptr [ebx]
        mov    word ptr [edi + 2], ax
        mov    ax, word ptr [ebx + 2]
        mov    word ptr [edi], ax
        mov    word ptr [edi + 0x10], 0
        push   esi
        movzx  ecx, word ptr [esp + 0x32]
        movzx  eax, word ptr [esp + 0x2e]
        add    ecx, eax
        movzx  eax, word ptr [esp + 0x30]
        add    ecx, eax
        movzx  eax, word ptr [esp + 0x2c]
        add    ecx, eax
        add    ecx, 0x1a00
        push   ecx
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [edi + 0x10], ax
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_95B8
        add    esp, 4
        mov    eax, dword ptr [esp + 0x30]
        cmp    dword ptr [eax], 0
        je     loc_1_8407
        push   0
        push   dword ptr [esp + 0x2c]
        lea    ecx, [esp + 0x18]
        push   ecx
        push   eax
        call   sub_1_95BC
        add    esp, 0x10
loc_1_8407:
        inc    dword ptr [0x9568]            ; -> obj1:0x9568
        inc    dword ptr [0x9574]            ; -> obj1:0x9574
        push   0
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x34]
        push   0x14
        push   esi
        push   0
        push   dword ptr [0x95d8]            ; -> loc_1_95D8
        call   sub_1_9598
        add    esp, 0x20
        cmp    eax, 0x2bf7
        je     loc_1_8446
        push   esi
        call   sub_1_7CB8
        add    esp, 4
loc_1_8446:
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_95C4
        add    esp, 4
loc_1_8454:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_845C:
        mov    eax, dword ptr [esp + 4]
        movzx  ecx, word ptr [eax + 0x70]
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x20], 0xffffffff
        mov    byte ptr [eax + 0x84], 4
        mov    cl, byte ptr [eax + 0x82]
        dec    cl
        mov    byte ptr [eax + 0x82], cl
        jne    loc_1_8489
        and    dword ptr [eax + 0x5c], 0xfffffffe
loc_1_8489:
        inc    dword ptr [0x9560]            ; -> obj1:0x9560
        and    dword ptr [eax + 0x58], 0xfffffff7
        ret    
sub_1_8494:
        push   esi
        mov    esi, dword ptr [esp + 8]
        call   sub_1_90E4
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x88], 0
        movzx  ecx, word ptr [esi + 0x68]
        movzx  eax, word ptr [esi + 0x6a]
        add    ecx, eax
        movzx  eax, word ptr [esi + 0x64]
        add    ecx, eax
        movzx  eax, word ptr [esi + 0x66]
        add    ecx, eax
        add    ecx, 0x600
        mov    dword ptr [esi + 0x24], ecx
        mov    byte ptr [esi + 0x85], 0
        mov    dword ptr [esi + 0x8c], 0
        mov    word ptr [esi + 0x7c], 0
        mov    word ptr [esi + 0x7e], 0x3c
        mov    word ptr [esi + 0x72], 0x1e
        mov    word ptr [esi + 0x90], 0
        mov    word ptr [esi + 0x92], 0
        pop    esi
        ret    
        db  090h
loc_1_8508:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        cmp    byte ptr [esp + 0x1c], 0
        jne    loc_1_86BC
        mov    edi, dword ptr [esp + 0x34]
        mov    ax, word ptr [edi]
        push   eax
        mov    ax, word ptr [edi + 2]
        push   eax
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x34]
        call   sub_1_5B50
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        je     loc_1_8745
        mov    al, byte ptr [esi + 0x84]
        or     al, al
        je     loc_1_8745
        cmp    al, 0xa
        je     loc_1_8745
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_8745
        mov    ebx, dword ptr [esp + 0x20]
        lea    eax, [ebx - 0x2afa]
        cmp    eax, 0x12
        ja     loc_1_8745
        jmp    dword ptr [eax*4 + 0x874c]    ; -> obj1:0x874C
        db  08Dh, 049h, 000h
loc_1_8580:
        pop    edi
        mov    dword ptr [esi + 0x88], ebx
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_1_8590:
        mov    dword ptr [esi + 0x88], ebx
        inc    dword ptr [esi + 0x74]
        push   dword ptr [esp + 0x10]
        push   8
        push   esi
        call   sub_1_6398
        add    esp, 0xc
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   0x1c
        push   ebx
        call   sub_1_6090
        add    esp, 8
        push   eax
        push   0
        push   esi
        call   sub_1_6FB8
        add    esp, 0xc
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_85DC:
        movzx  edx, word ptr [esi + 0x70]
        mov    edi, dword ptr [esi + 0x10]
        mov    eax, dword ptr [esi + 0x1c]
        sub    edi, eax
        sbb    ecx, ecx
        and    edi, ecx
        add    edi, eax
        shr    edi, 1
        lea    eax, [edx*2]
        sub    edi, eax
        sbb    ecx, ecx
        not    ecx
        and    edi, ecx
        add    edi, eax
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x1c], edx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_8610:
        movzx  eax, byte ptr [esi + 0xec]
        sub    eax, 0x14
        add    dword ptr [esp + 0x30], eax
        mov    edx, dword ptr [edi + 4]
        mov    ecx, edx
        and    ecx, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     ecx, eax
        shr    edx, 0x18
        or     ecx, edx
        mov    ax, word ptr [esp + 0x30]
        cmp    word ptr [esi + 0x70], ax
        jb     loc_1_867C
        mov    eax, ecx
        sub    eax, dword ptr [esi + 4]
        js     loc_1_867C
        mov    eax, ecx
        sub    eax, dword ptr [esi + 8]
        jns    loc_1_867C
        push   ecx
        push   esi
        call   sub_1_5534
        add    esp, 8
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [esi + 0x1c], eax
        push   0x400
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_867C:
        movzx  eax, word ptr [esi + 0x80]
        mov    ecx, dword ptr [esp + 0x30]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    ax, cx
        mov    word ptr [esi + 0x70], ax
        and    eax, 0xffff
        cmp    dword ptr [esi + 0x1c], eax
        jae    loc_1_8745
        mov    dword ptr [esi + 0x1c], eax
        add    eax, eax
        cmp    dword ptr [esi + 0x20], eax
        jae    loc_1_8745
        mov    dword ptr [esi + 0x20], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_86BC:
        mov    ebx, dword ptr [esp + 0x20]
        cmp    ebx, 0x2b0d
        je     loc_1_86D8
        cmp    ebx, 0x2b0e
        je     loc_1_8708
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
loc_1_86D8:
        mov    eax, dword ptr [esp + 0x30]
        sub    eax, 0x14
        mov    dword ptr [esp + 0xc], eax
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   0x5a2c                        ; -> loc_1_5A2C
        call   sub_1_5AB0
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_1_8708:
        push   0
        push   0
        push   0
        push   0
        push   6
        call   0
        add    esp, 0x14
        push   0
        push   0x92ec                        ; -> loc_1_92EC
        call   6
        add    esp, 8
        push   0
        push   0x164                         ; -> loc_1_0164
        call   sub_1_000C
        add    esp, 8
        push   0x92ec                        ; -> loc_1_92EC
        call   sub_1_911A
        add    esp, 4
loc_1_8745:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_8580
        dd  loc_1_8580
        dd  loc_1_8590
        dd  loc_1_8580
        dd  loc_1_8745
        dd  loc_1_8745
        dd  loc_1_8745
        dd  loc_1_8580
        dd  loc_1_8745
        dd  loc_1_8745
        dd  loc_1_8580
        dd  loc_1_8580
        dd  loc_1_8580
        dd  loc_1_8580
        dd  loc_1_85DC
        dd  loc_1_8745
        dd  loc_1_8745
        dd  loc_1_8745
        dd  loc_1_8610
sub_1_8798:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0xc]
        mov    ax, word ptr [ecx + 0x6e]
        mov    esi, dword ptr [esp + 0x10]
        mov    word ptr [esi], ax
        mov    ax, word ptr [ecx + 0x6c]
        mov    word ptr [esi + 2], ax
        mov    edx, dword ptr [ecx + 8]
        mov    edi, edx
        and    edi, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     edi, eax
        shl    edi, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     edi, eax
        shr    edx, 0x18
        or     edi, edx
        mov    dword ptr [esi + 4], edi
        mov    edx, dword ptr [ecx + 0x40]
        mov    edi, edx
        and    edi, 0xff00
        mov    eax, edx
        shl    eax, 0x10
        or     edi, eax
        shl    edi, 8
        mov    eax, edx
        and    eax, 0xff0000
        shr    eax, 8
        or     edi, eax
        shr    edx, 0x18
        or     edi, edx
        mov    dword ptr [esi + 8], edi
        mov    word ptr [esi + 0xc], 0x1050
        mov    dword ptr [esi + 0x10], 0
        push   ecx
        call   sub_1_7DDC
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi + 0xe], cx
        pop    edi
        pop    esi
        ret    
sub_1_8824:
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x54]
        mov    eax, dword ptr [ebp + 0x58]
        test   eax, 0x40000
        jne    loc_1_8DB8
        test   byte ptr [ebp + 0x5c], 2
        jne    loc_1_8DB8
        or     eax, 0x40000
        mov    dword ptr [ebp + 0x58], eax
        test   eax, 0x10000000
        jne    loc_1_8CE9
loc_1_885A:
        mov    edx, dword ptr [ebp + 8]
        sub    edx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp + 0x14]
        sub    eax, edx
        mov    dword ptr [esp + 0x40], eax
        mov    esi, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp + 0x10]
        sub    esi, eax
        sbb    ecx, ecx
        and    esi, ecx
        add    esi, eax
        sub    esi, edx
        mov    edx, dword ptr [ebp + 0x58]
        mov    eax, edx
        and    eax, 0x400000
        shr    eax, 0x16
        cmp    esi, eax
        jg     loc_1_888C
        mov    esi, eax
loc_1_888C:
        mov    dword ptr [esp + 0x3c], esi
        movzx  esi, word ptr [ebp + 0x70]
        mov    edi, dword ptr [esp + 0x3c]
        mov    eax, dword ptr [esp + 0x40]
        sub    edi, eax
        sbb    ecx, ecx
        and    edi, ecx
        add    edi, eax
        sub    edi, esi
        sbb    eax, eax
        and    edi, eax
        lea    ecx, [edi + esi]
        mov    dword ptr [esp + 0x2c], ecx
        cmp    ecx, esi
        je     loc_1_88E1
        or     ecx, ecx
        je     loc_1_88BF
        cmp    ecx, dword ptr [esp + 0x40]
        je     loc_1_88E1
loc_1_88BF:
        cmp    dword ptr [esp + 0x3c], 0
        je     loc_1_8CF8
        test   edx, 0x410000
        jne    loc_1_88E1
        mov    eax, dword ptr [ebp + 0x18]
        shr    eax, 1
        cmp    eax, dword ptr [esp + 0x2c]
        ja     loc_1_8CF8
loc_1_88E1:
        call   sub_1_7C9C
        mov    dword ptr [esp + 0x38], eax
        or     eax, eax
        je     loc_1_8D71
        mov    edi, eax
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [esp + 0x30], edx
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [0x9594]       ; -> obj1:0x9594
        mov    dword ptr [esp + 0x34], eax
        mov    ebx, eax
        add    ebx, 0x14
        push   eax
        push   ebp
        call   sub_1_8798
        add    esp, 8
        mov    word ptr [ebx + 8], 0
        mov    word ptr [ebx + 0xa], 0
        mov    dword ptr [ebx + 4], 0
        mov    esi, dword ptr [esp + 0x2c]
        or     esi, esi
        je     loc_1_8AFC
        mov    edx, dword ptr [esp + 0x30]
        mov    dword ptr [ebx], edx
loc_1_893C:
        mov    edx, dword ptr [esp + 0x30]
        inc    dword ptr [edx + 0x18]
        inc    dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebp + 0x30]
        or     ecx, ecx
        jne    loc_1_8988
        mov    eax, dword ptr [ebp + 0x34]
        cmp    esi, eax
        jb     loc_1_8988
        cmp    word ptr [ebx + 0xa], 0
        jne    loc_1_8988
        cmp    dword ptr [edx + 0x2c], 0
        jne    loc_1_8988
        mov    ecx, dword ptr [ebp + 0x2c]
        mov    dword ptr [edi], ecx
loc_1_8966:
        inc    word ptr [ebx + 8]
        mov    edi, dword ptr [edi]
        cmp    dword ptr [edi], 0
        jne    loc_1_8966
        mov    edx, dword ptr [esp + 0x30]
        mov    dword ptr [edx + 0x2c], edi
        sub    esi, eax
        mov    dword ptr [ebp + 0x34], 0
        jmp    loc_1_8A60
        db  08Bh, 0FFh
loc_1_8988:
        lea    eax, [ebp + 0x2c]
        mov    dword ptr [esp + 0x44], eax
        mov    eax, dword ptr [ebp + 0x2c]
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x14], ecx
loc_1_899A:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x24]
        call   sub_1_91FC
        mov    eax, dword ptr [esp + 0x14]
        sub    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x18]
        sub    eax, esi
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, esi
        mov    dword ptr [esp + 0x10], eax
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        add    eax, dword ptr [esp + 0x18]
        push   eax
        push   dword ptr [0x9740]            ; -> loc_1_9740
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x34]
        push   eax
        call   sub_1_91EA
        cmp    dword ptr [esp + 0x24], 0
        jne    loc_1_8A2C
        inc    word ptr [ebx + 0xa]
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x18], eax
        ja     loc_1_8A18
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x14], 0
        jmp    loc_1_8A20
        db  090h
loc_1_8A18:
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [esp + 0x14], eax
loc_1_8A20:
        mov    eax, dword ptr [esp + 0x10]
        sub    dword ptr [ebp + 0x34], eax
        sub    esi, eax
        jmp    loc_1_8A44
        db  090h
loc_1_8A2C:
        cmp    word ptr [ebx + 0xa], 0
        jne    loc_1_8A3E
        cmp    word ptr [ebx + 8], 0
        je     loc_1_8D64
loc_1_8A3E:
        sub    dword ptr [esp + 0x2c], esi
        sub    esi, esi
loc_1_8A44:
        or     esi, esi
        je     loc_1_8A52
        cmp    dword ptr [ebp + 0x34], 0
        jne    loc_1_899A
loc_1_8A52:
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ebp + 0x2c], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebp + 0x30], eax
loc_1_8A60:
        mov    edx, dword ptr [esp + 0x30]
        mov    al, byte ptr [edx + 0x1c]
        and    al, 1
        je     loc_1_8A9F
        mov    edx, dword ptr [ebp + 0x34]
        mov    ecx, dword ptr [ebp + 0x58]
        and    ecx, 0x2000
        or     ecx, ecx
        mov    cx, 0
        sete   cl
        sub    dx, cx
        sub    dx, si
        mov    ecx, dword ptr [esp + 0x2c]
        add    cx, dx
        mov    dh, cl
        mov    dl, ch
        mov    ecx, dword ptr [esp + 0x34]
        or     word ptr [ecx + 0xc], 0x2000
        mov    word ptr [ecx + 0x12], dx
loc_1_8A9F:
        cmp    dword ptr [ebp + 0x34], 0
        jne    loc_1_8AF2
        mov    ecx, dword ptr [esp + 0x34]
        or     word ptr [ecx + 0xc], 0x800
        mov    edx, dword ptr [esp + 0x30]
        mov    ecx, dword ptr [edx]
        mov    edx, ebp
        sub    edx, ecx
        cmp    edx, -0x38
        je     loc_1_8AE4
        mov    dword ptr [esp + 0x30], ecx
        mov    edx, dword ptr [ecx + 0x20]
        mov    dword ptr [ebp + 0x34], edx
        mov    edx, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp + 0x30], edx
        mov    edx, dword ptr [ecx + 0x28]
        mov    dword ptr [ebp + 0x2c], edx
        mov    dword ptr [ebp + 0x28], ecx
        or     al, al
        je     loc_1_8AF2
        test   byte ptr [ecx + 0x1c], 1
        jne    loc_1_8AF2
        jmp    loc_1_8B16
        db  090h
loc_1_8AE4:
        mov    dword ptr [ebp + 0x28], 0
        mov    dword ptr [ebp + 0x2c], 0
loc_1_8AF2:
        or     esi, esi
        jne    loc_1_893C
        jmp    loc_1_8B16
loc_1_8AFC:
        test   byte ptr [ebp + 0x5a], 1
        je     loc_1_8DA4
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [eax], 0
loc_1_8B16:
        sub    dword ptr [esp + 0x2c], esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esp + 0x2c]
        lea    eax, [edx + esi]
        mov    dword ptr [ebp + 8], eax
        cmp    dword ptr [ebp + 0xc], esi
        jne    loc_1_8B40
        mov    dword ptr [ebp + 0xc], eax
        inc    dword ptr [0x9568]            ; -> obj1:0x9568
        cmp    dword ptr [ebp + 0x8c], 0
        jne    loc_1_8B73
        jmp    loc_1_8B65
loc_1_8B40:
        inc    dword ptr [0x956c]            ; -> obj1:0x956C
        mov    eax, dword ptr [ebp + 8]
        sub    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        jle    loc_1_8B73
        inc    dword ptr [0x9568]            ; -> obj1:0x9568
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp + 0xc], eax
        cmp    dword ptr [ebp + 0x8c], 0
        jne    loc_1_8B73
loc_1_8B65:
        mov    eax, dword ptr [0x966c]       ; -> loc_1_966C
        mov    dword ptr [ebp + 0x8c], eax
        mov    dword ptr [ebp + 0x78], esi
loc_1_8B73:
        mov    ecx, dword ptr [esp + 0x2c]
        cmp    ecx, dword ptr [esp + 0x40]
        jne    loc_1_8BB4
        mov    edi, dword ptr [ebp + 0x58]
        test   edi, 0x10000
        je     loc_1_8BB4
        cmp    dword ptr [esp + 0x3c], edx
        jle    loc_1_8BB4
        mov    ecx, dword ptr [esp + 0x34]
        or     word ptr [ecx + 0xc], 0x100
        mov    eax, dword ptr [ebp + 8]
        inc    eax
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp + 0xc], eax
        or     edi, 0x30000000
        mov    dword ptr [ebp + 0x58], edi
        and    edi, 0xfffeffff
        mov    dword ptr [ebp + 0x58], edi
loc_1_8BB4:
        sub    eax, eax
        add    dword ptr [esp + 0x2c], 0x14
        push   dword ptr [esp + 0x38]
        mov    al, byte ptr [esp + 0x31]
        mov    ah, byte ptr [esp + 0x30]
        add    eax, dword ptr [ebp + 0x24]
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    ecx, dword ptr [esp + 0x34]
        mov    word ptr [ecx + 0x10], ax
        cmp    word ptr [ebp + 0x90], 0
        mov    eax, dword ptr [0x966c]       ; -> loc_1_966C
        jne    loc_1_8BF8
        mov    cx, word ptr [ebp + 0x72]
        mov    word ptr [ebp + 0x90], cx
loc_1_8BF8:
        mov    word ptr [ebp + 0x92], 0
        and    dword ptr [ebp + 0x58], 0xffbff5ff
        mov    word ptr [ebp + 0xda], 0
        mov    dword ptr [ebp + 0x98], eax
        push   dword ptr [ebp + 0xf0]
        lea    eax, [ebp + 0xe4]
        push   eax
        push   dword ptr [ebp + 0x68]
        push   dword ptr [ebp + 0x64]
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x4c]
        push   ebx
        push   dword ptr [0x95d8]            ; -> loc_1_95D8
        call   sub_1_9598
        add    esp, 0x20
        mov    edi, eax
        mov    dword ptr [ebp + 0x88], edi
        cmp    edi, 0x2bf7
        je     loc_1_8CDF
        push   dword ptr [esp + 0x38]
        push   ebx
        call   sub_1_7D24
        add    esp, 8
        or     edi, edi
        je     loc_1_8CDF
        mov    eax, esi
        sub    eax, dword ptr [ebp + 4]
        js     loc_1_8C7F
        mov    eax, esi
        sub    eax, dword ptr [ebp + 8]
        jns    loc_1_8C7F
        push   esi
        push   ebp
        call   sub_1_5534
        add    esp, 8
loc_1_8C7F:
        cmp    word ptr [ebp + 0x90], 0
        jne    loc_1_8C94
        mov    ax, word ptr [ebp + 0x72]
        mov    word ptr [ebp + 0x90], ax
loc_1_8C94:
        cmp    edi, 0x2b01
        jne    loc_1_8CE9
        push   0
        lea    eax, [esp + 0x4c]
        push   eax
        push   dword ptr [ebp + 0x68]
        push   dword ptr [ebp + 0x64]
        call   sub_1_95B0
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_8CE9
        movzx  eax, byte ptr [ebp + 0xec]
        sub    eax, 0x14
        add    dword ptr [esp + 0x48], eax
        mov    ax, word ptr [ebp + 0x80]
        mov    ecx, dword ptr [esp + 0x48]
        sub    ax, cx
        sbb    dx, dx
        and    ax, dx
        add    ax, cx
        mov    word ptr [ebp + 0x70], ax
loc_1_8CDF:
        test   byte ptr [ebp + 0x5b], 0x10
        je     loc_1_885A
loc_1_8CE9:
        and    dword ptr [ebp + 0x58], 0xff3bfbff
        jmp    loc_1_8DC0
        db  08Dh, 049h, 000h
loc_1_8CF8:
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_1_8D48
        mov    eax, dword ptr [ebp + 0x58]
        test   eax, 0x80000
        jne    loc_1_8D30
        or     eax, 0x80000
        mov    dword ptr [ebp + 0x58], eax
        mov    byte ptr [ebp + 0x85], 0
        inc    byte ptr [ebp + 0x82]
        mov    ax, word ptr [ebp + 0x72]
        or     dword ptr [ebp + 0x5c], 1
        mov    word ptr [ebp + 0x90], ax
        jmp    loc_1_8CE9
        db  08Bh, 0FFh
loc_1_8D30:
        cmp    word ptr [ebp + 0x90], 0
        jne    loc_1_8CE9
        mov    ax, word ptr [ebp + 0x72]
        mov    word ptr [ebp + 0x90], ax
        jmp    loc_1_8CE9
        db  090h
loc_1_8D48:
        cmp    dword ptr [esp + 0x2c], 0
        je     loc_1_8CE9
        cmp    word ptr [ebp + 0xda], 0
        jne    loc_1_8CE9
        mov    word ptr [ebp + 0xda], 0x32
        jmp    loc_1_8CE9
loc_1_8D64:
        push   dword ptr [esp + 0x38]
        push   ebx
        call   sub_1_7D24
        add    esp, 8
loc_1_8D71:
        cmp    word ptr [ebp + 0x90], 0
        jne    loc_1_8D86
        mov    ax, word ptr [ebp + 0x72]
        mov    word ptr [ebp + 0x90], ax
loc_1_8D86:
        and    dword ptr [ebp + 0x58], 0xffbbfbff
        push   dword ptr [esp + 0x4c]
        push   ebp
        call   sub_1_63D8
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
        db  08Bh, 0FFh
loc_1_8DA4:
        push   dword ptr [esp + 0x38]
        call   sub_1_7CB8
        add    esp, 4
        jmp    loc_1_8CE9
        db  08Dh, 049h, 000h
loc_1_8DB8:
        or     eax, 0x800000
        mov    dword ptr [ebp + 0x58], eax
loc_1_8DC0:
        push   dword ptr [esp + 0x4c]
        push   ebp
        call   sub_1_63D8
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
        db  08Dh, 049h, 000h
loc_1_8DD8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        push   dword ptr [ebx]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     loc_1_8F28
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     loc_1_8F1C
        mov    al, byte ptr [esi + 0x84]
        cmp    al, 4
        je     loc_1_8E0E
        cmp    al, 7
        jne    loc_1_8F10
loc_1_8E0E:
        test   byte ptr [esi + 0x86], 1
        jne    loc_1_8F10
        mov    edi, dword ptr [esp + 0x1c]
        or     edi, edi
        je     loc_1_8F08
        call   sub_1_7CE8
        or     eax, eax
        je     loc_1_8EFC
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 0xc], ecx
        mov    edx, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0x28], edx
        mov    dword ptr [eax + 0x14], edi
        mov    dword ptr [eax + 0x20], edi
        mov    dword ptr [eax + 0x18], 1
        mov    dword ptr [eax + 0x24], 0
        mov    dword ptr [eax + 0x2c], 0
        mov    ecx, dword ptr [0x966c]       ; -> loc_1_966C
        mov    dword ptr [eax + 0x30], ecx
        test   byte ptr [esp + 0x18], 0x20
        setne  cl
        mov    byte ptr [eax + 0x1c], cl
        mov    ebx, dword ptr [esi + 0x14]
        add    ebx, edi
        mov    dword ptr [esi + 0x14], ebx
        mov    ecx, dword ptr [esi + 0x38]
        sub    ecx, esi
        cmp    ecx, 0x38
        mov    ecx, 0
        sete   cl
        mov    ebp, dword ptr [esi + 0x3c]
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [esi + 0x3c], eax
        lea    ebp, [esi + 0x38]
        mov    dword ptr [eax], ebp
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_8EB5
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [esi + 0x2c], edx
        mov    dword ptr [esi + 0x30], 0
        mov    dword ptr [esi + 0x34], edi
loc_1_8EB5:
        or     ecx, ecx
        je     loc_1_8ED0
        inc    dword ptr [esi + 0x74]
        push   esi
        call   sub_1_8824
        add    esp, 4
        mov    eax, 0xff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_8ED0:
        test   byte ptr [esi + 0x5a], 2
        je     loc_1_8EE6
        mov    ecx, dword ptr [esi + 4]
        sub    ecx, dword ptr [esi + 0xc]
        add    ecx, ebx
        movzx  eax, word ptr [esi + 0x70]
        cmp    ecx, eax
        jb     loc_1_8EF2
loc_1_8EE6:
        push   esi
        inc    dword ptr [esi + 0x74]
        call   sub_1_8824
        add    esp, 4
loc_1_8EF2:
        pop    ebp
        mov    eax, 0xff
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_8EFC:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_8F08:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_8F10:
        pop    ebp
        mov    eax, 0x19
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_8F1C:
        pop    ebp
        mov    eax, 0x19
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_8F28:
        pop    ebp
        mov    eax, 0xf
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_8F34:
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
        ror    eax, 0x10
        mov    ecx, eax
        ror    ecx, 0x10
        add    eax, ecx
        shr    eax, 0x10
        ret    
sub_1_8F49:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        jecxz  loc_1_8FC4
        test   esi, 3
        je     loc_1_8FA4
        test   esi, 1
        je     loc_1_8F8F
        movzx  edx, byte ptr [esi]
        add    eax, edx
        adc    eax, 0
        mov    edx, 0x8f34                   ; -> loc_1_8F34
        xchg   dword ptr [esp], edx
        push   edx
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
        ror    eax, 0x10
        inc    esi
        dec    ecx
        je     loc_1_8FC4
        test   esi, 3
        je     loc_1_8FA4
loc_1_8F8F:
        cmp    ecx, 1
        je     loc_1_8FF1
        movzx  edx, word ptr [esi]
        add    eax, edx
        adc    eax, 0
        add    esi, 2
        sub    ecx, 2
        je     loc_1_8FC4
loc_1_8FA4:
        cmp    ecx, 4
        jb     loc_1_8FC0
        mov    edx, ecx
        shr    ecx, 2
        and    dl, 3
        mov    eax, eax
loc_1_8FB4:
        add    eax, dword ptr [esi]
        adc    eax, 0
        add    esi, 4
        loop   loc_1_8FB4
        mov    ecx, edx
loc_1_8FC0:
        test   cl, cl
        jne    loc_1_8FD0
loc_1_8FC4:
        pop    esi
        mov    ecx, eax
        ror    ecx, 0x10
        add    eax, ecx
        shr    eax, 0x10
        ret    
loc_1_8FD0:
        cmp    cl, 1
        je     loc_1_8FF1
        movzx  edx, word ptr [esi]
        add    eax, edx
        adc    eax, 0
        add    esi, 2
        sub    cl, 2
        jne    loc_1_8FF1
        pop    esi
        mov    ecx, eax
        ror    ecx, 0x10
        add    eax, ecx
        shr    eax, 0x10
        ret    
loc_1_8FF1:
        movzx  edx, byte ptr [esi]
        add    eax, edx
        adc    eax, 0
        pop    esi
        mov    ecx, eax
        ror    ecx, 0x10
        add    eax, ecx
        shr    eax, 0x10
        ret    
        db  000h, 000h, 000h
sub_1_9008:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    edi, 0x92ec                   ; -> loc_1_92EC
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    word ptr [ebp - 0x14], cx
        mov    ax, cx
        mov    word ptr [ebp - 0x12], ax
        mov    dword ptr [ebp - 0x10], 0x92ec ; -> loc_1_92EC
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_9172
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_9054
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_9054:
        mov    esi, 0x9404                   ; -> obj1:0x9404
loc_1_9059:
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 8]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    word ptr [ebp - 0x14], cx
        mov    ax, cx
        mov    word ptr [ebp - 0x12], ax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0x10], eax
        push   0
        lea    eax, [ebp - 0x14]
        push   eax
        push   dword ptr [ebp - 0xc]
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_9232
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_90A0
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_1_90C8
        mov    eax, dword ptr [esi + 4]
        jmp    loc_1_90A6
loc_1_90A0:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
loc_1_90A6:
        mov    dword ptr [ebx], eax
        add    esi, 0x10
        cmp    esi, 0x94b4                   ; -> obj1:0x94B4
        jb     loc_1_9059
        push   dword ptr [ebp - 0xc]
        call   sub_1_9238
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_90C8:
        push   dword ptr [ebp - 0xc]
        call   sub_1_9238
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  090h
sub_1_90D8:
        VxDCall  Dev_0488.Service_8001h          ; cd2001808804
sub_1_90DE:
        VxDCall  Dev_0488.Service_8002h          ; cd2002808804
sub_1_90E4:
        VMMCall  VMM.Get_System_Time             ; cd203f800100
sub_1_90EA:
        VxDCall  Dev_0488.Service_8003h          ; cd2003808804
        VxDCall  Dev_0488.Service_8004h          ; cd2004808804
sub_1_90F6:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_90FC:
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
sub_1_9102:
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
        VxDCall  Dev_0488.Service_8005h          ; cd2005808804
        VxDCall  Dev_0488.Service_800Dh          ; cd200d808804
        VxDCall  Dev_0488.Service_8006h          ; cd2006808804
sub_1_911A:
        mov    eax, dword ptr [0x94b4]       ; -> obj1:0x94B4
        VxDCall  Dev_0488.Service_8009h          ; cd2009808804
        mov    eax, dword ptr [0x94b4]       ; -> obj1:0x94B4
        VxDCall  Dev_0488.Service_800Ch          ; cd200c808804
        mov    eax, dword ptr [0x94b4]       ; -> obj1:0x94B4
        VxDCall  Dev_0488.Service_800Bh          ; cd200b808804
        add    ch, cl
        and    byte ptr [ecx], ch
        sub    byte ptr [eax], 0
        VxDCall  VPOWERD.Service_802Ah           ; cd202a802800
        VxDCall  VPOWERD.Service_802Bh           ; cd202b802800
        VxDCall  VPOWERD.Service_802Ch           ; cd202c802800
        VxDCall  VPOWERD.Service_802Dh           ; cd202d802800
        VxDCall  VPOWERD.Service_802Eh           ; cd202e802800
        VxDCall  VPOWERD.Service_802Fh           ; cd202f802800
        VxDCall  VPOWERD.Service_8030h           ; cd2030802800
        VxDCall  VPOWERD.Service_8050h           ; cd2050802800
sub_1_9172:
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
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_91DE:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_91E4:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_91EA:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
sub_1_91F6:
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_91FC:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_9232:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_9238:
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
        dd  MSTCP_Service_00         ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  0048Ah                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'MSTCP   '                  ; DDB_Name
        dd  05400A000h                  ; DDB_Init_Order
        dd  MSTCP_Control            ; DDB_Control_Proc
        dd  MSTCP_V86_API            ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x9294              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 0FFh, 000h, 000h, 000h
loc_1_92EC:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_931C:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_9328:
        js     loc_1_932D
        add    byte ptr [eax], al
        dd  loc_1_02E8               ; fixup
loc_1_932D:
        add    al, byte ptr [eax]
        db  000h
loc_1_9330:
        mov    word ptr [eax + eax], ss
        in     al, dx
        push   ebx
        add    byte ptr [eax], al
loc_1_9338:
        inc    eax
        add    dword ptr [eax], eax
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_9344:
        inc    esp
        popal  
        inc    edi
        push   edi
        inc    esp
        je     0x93b3
        arpl   word ptr [eax + eax], si
        add    byte ptr [eax], al
loc_1_9354:
        push   eax
        dec    ebp
        push   esp
        push   ebp
        inc    esp
        imul   esi, dword ptr [ebx + 0x63], 0x7265766f
        jns    loc_1_9362
loc_1_9362:
        add    byte ptr [eax], al
loc_1_9364:
        push   eax
        dec    ebp
        push   esp
        push   ebp
        inc    edx
        insb   byte ptr es:[edi], dx
        popal  
        arpl   word ptr [ebx + 0x48], bp
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        inc    esp
        je     0x93da
        db  063h, 074h, 000h
loc_1_9378:
        dec    ebx
        jo     0x93be
        insb   byte ptr es:[edi], dx
        imul   esi, dword ptr [esi + 0x65], 0x656d6954
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x65], cl
        jo     loc_1_93CE
        insb   byte ptr es:[edi], dx
        imul   esi, dword ptr [esi + 0x65], 0x65746e49
        jb     0x940d
        popal  
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x66], al
        popal  
        jne    0x940e
        je     loc_1_93F6
        arpl   word ptr [esi + 0x57], si
        imul   ebp, dword ptr [esi + 0x64], 0x776f
        add    byte ptr [eax], al
loc_1_93B0:
        dec    ebp
        popal  
        js     0x93f7
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        jae    loc_1_93BF
loc_1_93BF:
        add    byte ptr [ebp + 0x61], cl
        js     loc_1_9407
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[edx + edx*2 + 0x65], si
        je     loc_1_9440
loc_1_93CE:
        imul   esp, dword ptr [ebp + 0x73], 0x4d000000
        popal  
        js     0x941c
        popal  
        je     0x943c
        push   edx
        je     0x9451
        imul   esp, dword ptr [ebp + 0x73], 0x53420000
        inc    esp
        push   ebp
        jb     0x9451
        outsb  dx, byte ptr gs:[esi]
        je     loc_1_93EE
loc_1_93EE:
        add    byte ptr [eax], al
loc_1_93F0:
        push   eax
        jb     loc_1_9458
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
loc_1_93F6:
        dec    eax
        popal  
        jo     0x943f
        outsd  dx, dword ptr [esi]
        jne    loc_1_946D
        je     loc_1_9401
loc_1_9401:
        add    byte ptr [eax], al
        add    byte ptr [ebx + edx*4], al    ; -> loc_1_9344
loc_1_9407:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [esi + edx*4], ah    ; -> loc_1_9664
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + edx*4], dl    ; -> loc_1_9354
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_9600
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   ebx, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cwde   
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        js     0x93c9
        add    byte ptr [eax], al
        add    ch, bl
        insd   dword ptr es:[edi], dx
        add    byte ptr [esi + edx*4], bl    ; -> loc_1_961C
        add    byte ptr [eax], al
loc_1_9440:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    byte ptr [ebx + 0x3e80000], dl
        add    byte ptr [eax], al
        cld    
        xchg   ebp, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pushfd 
        xchg   ebx, eax
        add    byte ptr [eax], al
loc_1_9458:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   0x96
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x64000093], dh ; -> loc_1_93B0
        add    byte ptr [eax], al
        add    al, ch                        ; -> loc_1_95E8
loc_1_946D:
        xchg   ebp, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rcl    byte ptr [ebx + 0x30000], 0
        add    byte ptr [esi + edx*4], cl    ; -> loc_1_964C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dl                        ; -> obj1:0x93D4
        xchg   ebx, eax
        add    byte ptr [eax], al
        add    eax, 0x54000000               ; -> loc_1_9654
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        in     al, 0x93
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        and    al, 0x96
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dd  loc_1_93F0               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_9604               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_9598:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_95D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_95FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9600:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9604:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9608:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_960C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9610:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9614:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9618:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_961C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9620:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9624:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9628:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_962C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9638:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_963C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9640:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9644:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9648:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_964C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9650:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9654:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9658:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_965C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9660:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9664:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9668:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_966C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9670:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9674:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9678:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_967C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9680:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9684:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9688:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_968C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9694:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_969C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_96F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9738:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_973C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9740:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9744:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9748:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_974C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9750:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9758:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_975C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9760:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x70c  R-X
loc_2_0000:
        inc    byte ptr [0x92e8]             ; -> obj1:0x92E8
        jne    loc_2_0024
        mov    edi, 0x94b8                   ; -> obj1:0x94B8
        mov    ecx, 0x9760                   ; -> loc_1_9760
        sub    ecx, edi
        shr    ecx, 2
        sub    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        call   sub_2_0038
        cmp    eax, 1
loc_2_0024:
        ret    
sub_2_0025:
        VxDCall  Dev_0489.Service_0000h          ; cd2000008904
        sbb    al, al
        not    al
        ret    
sub_2_0030:
        VxDCall  Dev_0489.Service_8001h          ; cd2001808904
        add    byte ptr [eax], al
sub_2_0038:
        sub    esp, 0x80
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        call   sub_2_06E8
        cmp    eax, esi
        jne    loc_2_0058
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x80
        ret    
loc_2_0058:
        call   sub_2_0025
        or     al, al
        jne    loc_2_0070
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x80
        ret    
        db  08Dh, 049h, 000h
loc_2_0070:
        call   sub_1_9008
        or     al, al
        jne    loc_2_0088
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x80
        ret    
        db  08Dh, 049h, 000h
loc_2_0088:
        call   sub_1_90E4
        mov    dword ptr [0x94b8], eax       ; -> obj1:0x94B8
        mov    ecx, 0x64
        mov    eax, dword ptr [0x961c]       ; -> loc_1_961C
        sub    edx, edx
        div    ecx
        mov    dword ptr [0x961c], eax       ; -> loc_1_961C
        mov    eax, dword ptr [0x95fc]       ; -> loc_1_95FC
        sub    edx, edx
        div    ecx
        mov    dword ptr [0x95fc], eax       ; -> loc_1_95FC
        call   sub_1_9102
        push   0x48
        push   0x9590                        ; -> obj1:0x9590
        call   sub_2_0030
        add    esp, 8
        or     eax, eax
        jne    loc_2_0394
        cmp    dword ptr [0x9590], 1         ; -> obj1:0x9590
        jne    loc_2_0394
        push   1
        push   0x164                         ; -> loc_1_0164
        call   sub_1_000C
        add    esp, 8
        or     eax, eax
        je     loc_2_0394
        call   sub_2_03D8
        or     eax, eax
        je     loc_2_0394
        call   sub_1_9102
        call   sub_2_0470
        or     eax, eax
        je     loc_2_0394
        mov    eax, dword ptr [0x95e8]       ; -> loc_1_95E8
        sub    eax, 0xfffffe
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 0xfffffe
        mov    dword ptr [0x95e8], eax       ; -> loc_1_95E8
        call   sub_1_9102
        call   sub_2_05C4
        or     eax, eax
        je     loc_2_0394
        call   sub_1_9102
        call   sub_2_0560
        or     eax, eax
        je     loc_2_0394
        mov    esi, 1
        call   sub_1_9102
        call   sub_2_04E8
        or     eax, eax
        je     loc_2_0394
        call   sub_1_9102
        call   sub_2_0618
        or     eax, eax
        je     loc_2_0394
        mov    edi, 0x9540                   ; -> obj1:0x9540
        sub    eax, eax
        mov    ecx, 0xf
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [0x9540], 4         ; -> obj1:0x9540
        mov    dword ptr [0x9544], 0x12c     ; -> obj1:0x9544
        mov    dword ptr [0x9548], 0x3a980   ; -> obj1:0x9548
        mov    dword ptr [0x954c], 0xffffffff ; -> obj1:0x954C
        mov    edi, 0x957c                   ; -> obj1:0x957C
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        push   eax
        push   0x1cf8                        ; -> loc_1_1CF8
        push   0x14e0                        ; -> sub_1_14E0
        push   0x1b40                        ; -> loc_1_1B40
        push   0x11
        call   sub_1_0000
        add    esp, 0x14
        mov    dword ptr [0x95e4], eax       ; -> loc_1_95E4
        or     eax, eax
        je     loc_2_0394
        mov    dword ptr [0x96a0], 0x800     ; -> loc_1_96A0
        mov    dword ptr [0x96a4], 0xbd8     ; -> loc_1_96A4
        mov    dword ptr [0x96d0], 0x1784    ; -> loc_1_96D0
        mov    dword ptr [0x96d4], 0x18cc    ; -> loc_1_96D4
        mov    dword ptr [0x96d8], 0x1048    ; -> loc_1_96D8
        mov    dword ptr [0x96a8], 0x6404    ; -> loc_1_96A8
        mov    dword ptr [0x96ac], 0x64dc    ; -> loc_1_96AC
        mov    dword ptr [0x96b0], 0x65b0    ; -> loc_1_96B0
        mov    dword ptr [0x96b4], 0x660c    ; -> loc_1_96B4
        mov    dword ptr [0x96b8], 0x67ac    ; -> loc_1_96B8
        mov    dword ptr [0x96bc], 0x6dcc    ; -> loc_1_96BC
        mov    dword ptr [0x96c0], 0x6a84    ; -> loc_1_96C0
        mov    dword ptr [0x96c4], 0x6cd8    ; -> loc_1_96C4
        mov    dword ptr [0x96c8], 0x5174    ; -> loc_1_96C8
        mov    dword ptr [0x96cc], 0x8dd8    ; -> loc_1_96CC
        mov    dword ptr [0x96dc], 0x71d4    ; -> loc_1_96DC
        mov    dword ptr [0x96e0], 0x74e0    ; -> loc_1_96E0
        mov    dword ptr [0x96e4], 0x74e8    ; -> loc_1_96E4
        mov    dword ptr [0x96e8], 0x74f0    ; -> loc_1_96E8
        mov    dword ptr [0x96ec], 0x7878    ; -> loc_1_96EC
        push   0x96a0                        ; -> loc_1_96A0
        push   0x92ec                        ; -> loc_1_92EC
        call   sub_1_0006
        add    esp, 8
        or     eax, eax
        je     loc_2_0394
        call   sub_1_9102
        push   0x100
        call   sub_1_90F6
        add    esp, 4
        mov    dword ptr [0x9648], eax       ; -> loc_1_9648
        or     eax, eax
        je     loc_2_0394
        mov    dword ptr [eax], 0x400
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0x401
        mov    dword ptr [eax + 0xc], 0
        mov    dword ptr [0x973c], 2         ; -> loc_1_973C
        push   0x973c                        ; -> loc_1_973C
        push   eax
        call   sub_1_95D0
        add    esp, 8
        call   sub_1_9102
        mov    eax, dword ptr [0x9604]       ; -> loc_1_9604
        sub    eax, 0x20
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 0x20
        mov    dword ptr [0x9604], eax       ; -> loc_1_9604
        mov    ebx, 0
        je     loc_2_035C
        inc    esi
        lea    esi, [esp + 0xc]
loc_2_0338:
        push   0x4000
        call   sub_1_90F6
        add    esp, 4
        or     eax, eax
        je     loc_2_035C
        mov    dword ptr [esi], eax
        call   sub_1_9102
        add    esi, 4
        inc    ebx
        cmp    ebx, dword ptr [0x9604]       ; -> loc_1_9604
        jb     loc_2_0338
loc_2_035C:
        sub    edi, edi
        mov    dword ptr [0x9604], ebx       ; -> loc_1_9604
        cmp    ebx, edi
        je     loc_2_0382
        lea    esi, [esp + 0xc]
loc_2_036C:
        push   dword ptr [esi]
        call   sub_1_90FC
        add    esp, 4
        add    esi, 4
        inc    edi
        cmp    edi, dword ptr [0x9604]       ; -> loc_1_9604
        jb     loc_2_036C
loc_2_0382:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        add    esp, 0x80
        ret    
        db  08Dh, 049h, 000h
loc_2_0394:
        cmp    dword ptr [0x95e4], 0         ; -> loc_1_95E4
        je     loc_2_03AF
        push   0
        push   0
        push   0
        push   0
        push   0x11
        call   sub_1_0000
        add    esp, 0x14
loc_2_03AF:
        call   sub_2_06BC
        call   sub_2_055C
        or     esi, esi
        je     loc_2_03C2
        call   sub_2_05B4
loc_2_03C2:
        call   sub_2_05CC
        call   sub_1_9102
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x80
        ret    
sub_2_03D8:
        push   esi
        push   edi
        mov    word ptr [0x9658], 1          ; -> loc_1_9658
        mov    dword ptr [0x95f8], 0x20      ; -> loc_1_95F8
        mov    dword ptr [0x9684], 0         ; -> loc_1_9684
        push   0x80
        call   sub_1_90F6
        add    esp, 4
        mov    dword ptr [0x9620], eax       ; -> loc_1_9620
        or     eax, eax
        jne    loc_2_0414
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_2_0414:
        push   0x78
        call   sub_1_90F6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_2_0438
        push   dword ptr [0x9620]            ; -> loc_1_9620
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  090h
loc_2_0438:
        inc    edi
        mov    edi, 5
        mov    dword ptr [0x969c], 0         ; -> loc_1_969C
loc_2_0448:
        push   esi
        call   sub_1_0BC4
        add    esp, 4
        add    esi, 0x18
        dec    edi
        jne    loc_2_0448
        mov    edi, 0x96f8                   ; -> loc_1_96F8
        sub    eax, eax
        mov    ecx, 0x10
        rep stosd dword ptr es:[edi], eax
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_2_0470:
        mov    dword ptr [0x9694], 0         ; -> loc_1_9694
        mov    dword ptr [0x9644], 0         ; -> loc_1_9644
        call   sub_1_1270
        or     eax, eax
        je     loc_2_04E4
        push   eax
        call   sub_1_13A8
        add    esp, 4
        mov    dword ptr [0x965c], 0x965c    ; -> loc_1_965C
        mov    dword ptr [0x9660], 0x965c    ; -> loc_1_9660
        mov    dword ptr [0x95ec], 0x95ec    ; -> loc_1_95EC
        mov    dword ptr [0x95f0], 0x95ec    ; -> loc_1_95F0
        mov    dword ptr [0x9758], 0         ; -> loc_1_9758
        mov    dword ptr [0x9750], 0x1460    ; -> loc_1_9750
        mov    eax, dword ptr [0x94b4]       ; -> obj1:0x94B4
        mov    dword ptr [0x975c], eax       ; -> loc_1_975C
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
loc_2_04E4:
        sub    eax, eax
        ret    
        db  090h
sub_2_04E8:
        mov    dword ptr [0x9744], 0x9744    ; -> loc_1_9744
        mov    dword ptr [0x9748], 0x9744    ; -> loc_1_9748
        mov    dword ptr [0x9610], 0x9610    ; -> loc_1_9610
        mov    dword ptr [0x9614], 0x9610    ; -> loc_1_9614
        mov    dword ptr [0x95dc], 0x95dc    ; -> loc_1_95DC
        mov    dword ptr [0x95e0], 0x95dc    ; -> loc_1_95E0
        mov    dword ptr [0x9650], 0         ; -> loc_1_9650
        mov    dword ptr [0x9678], 0         ; -> loc_1_9678
        mov    dword ptr [0x9680], 0         ; -> loc_1_9680
        mov    dword ptr [0x9674], 0         ; -> loc_1_9674
        mov    dword ptr [0x967c], 0         ; -> loc_1_967C
        mov    eax, 1
        ret    
sub_2_055C:
        ret    
        db  08Dh, 049h, 000h
sub_2_0560:
        push   edi
        mov    edi, 0x94bc                   ; -> obj1:0x94BC
        sub    eax, eax
        mov    ecx, 0x20
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [0x9618], eax       ; -> loc_1_9618
        mov    dword ptr [0x966c], eax       ; -> loc_1_966C
        mov    dword ptr [0x953c], 0xa       ; -> obj1:0x953C
        mov    dword ptr [0x9638], eax       ; -> loc_1_9638
        mov    dword ptr [0x9640], eax       ; -> loc_1_9640
        mov    eax, dword ptr [0x94b4]       ; -> obj1:0x94B4
        mov    dword ptr [0x963c], eax       ; -> loc_1_963C
        push   0
        push   0x55b4                        ; -> loc_1_55B4
        push   0x64
        push   0x962c                        ; -> loc_1_962C
        call   sub_1_90D8
        add    esp, 0x10
        mov    eax, 1
        pop    edi
        ret    
sub_2_05B4:
        push   0x962c                        ; -> loc_1_962C
        call   sub_1_90DE
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_2_05C4:
        mov    eax, 1
        ret    
        db  08Bh, 0FFh
sub_2_05CC:
        ret    
        db  08Dh, 049h, 000h
sub_2_05D0:
        push   esi
        mov    esi, dword ptr [0x968c]       ; -> loc_1_968C
        mov    dword ptr [0x968c], 0         ; -> loc_1_968C
        mov    dword ptr [0x9608], 0         ; -> loc_1_9608
        or     esi, esi
        je     loc_2_0614
        nop    
loc_2_05F0:
        push   dword ptr [esi + 4]
        call   sub_1_91E4
        push   dword ptr [esi + 8]
        call   sub_1_90FC
        add    esp, 4
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_90FC
        add    esp, 4
        or     esi, esi
        jne    loc_2_05F0
loc_2_0614:
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_2_0618:
        sub    esp, 4
        mov    dword ptr [0x968c], 0         ; -> loc_1_968C
        mov    dword ptr [0x9608], 0         ; -> loc_1_9608
        call   sub_1_7B5C
        or     eax, eax
        je     loc_2_06B4
        push   eax
        call   sub_1_7CB8
        add    esp, 4
        push   0x64
        push   0x9740                        ; -> loc_1_9740
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_91DE
        cmp    dword ptr [esp], 0
        je     loc_2_0664
        call   sub_2_05D0
        sub    eax, eax
        add    esp, 4
        ret    
loc_2_0664:
        push   0x23c8                        ; -> sub_1_23C8
        push   0x8508                        ; -> loc_1_8508
        push   0x7d24                        ; -> sub_1_7D24
        push   0x2ed0                        ; -> loc_1_2ED0
        push   6
        call   sub_1_0000
        add    esp, 0x14
        mov    dword ptr [0x95d8], eax       ; -> loc_1_95D8
        or     eax, eax
        jne    loc_2_06A4
        call   sub_2_05D0
        push   dword ptr [0x9740]            ; -> loc_1_9740
        call   sub_1_91E4
        sub    eax, eax
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
loc_2_06A4:
        mov    eax, 1
        mov    byte ptr [0x9340], 1          ; -> obj1:0x9340
        add    esp, 4
        ret    
loc_2_06B4:
        sub    eax, eax
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_2_06BC:
        cmp    byte ptr [0x9340], 0          ; -> obj1:0x9340
        je     loc_2_06E7
        push   0
        push   0
        push   0
        push   0
        push   6
        call   sub_1_0000
        add    esp, 0x14
        call   sub_2_05D0
        push   dword ptr [0x9740]            ; -> loc_1_9740
        call   sub_1_91E4
loc_2_06E7:
        ret    
sub_2_06E8:
        VxDCall  Dev_0488.Service_0000h          ; cd2000008804
        jb     loc_2_0704
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_2_0704
        VxDCall  Dev_0488.Service_000Ah          ; cd200a008804
        mov    dword ptr [0x94b4], eax       ; -> obj1:0x94B4
        clc    
loc_2_0704:
        cmc    
        sbb    eax, eax
        ret    
        db  048h, 04Ch, 053h, 020h
VxD__LTEXT ENDS

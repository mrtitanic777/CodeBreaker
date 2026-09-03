; VDEF -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 480 instructions recovered by recursive descent

Declare_Virtual_Device VDEF, 3, 0, VDEF_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A0010100h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x4ec  R-X
loc_1_0000:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cmp    byte ptr [ebp + 4], 4
        je     loc_1_0014
        cmp    byte ptr [ebp + 4], 0
        jne    loc_1_0026
loc_1_0014:
        call   sub_1_005B
        mov    word ptr [ebp + 0x1a], ax
        mov    dword ptr [ebp + 0x10], 0x1d4 ; -> loc_1_01D4
        jmp    loc_1_002C
loc_1_0026:
        xor    eax, eax
        mov    word ptr [ebp + 0x1a], ax
loc_1_002C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0031:
        push   ebp
        push   edi
        push   ebx
        mov    ebp, dword ptr [esp + 0x10]
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ebx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        xor    eax, eax
        mov    word ptr [ebp + 0x1a], ax
        pop    ebx
        pop    edi
        pop    ebp
        ret    
sub_1_005B:
        movzx  edi, word ptr [ebp + 0x18]
        mov    ebx, dword ptr [ebp + 0x28]
        mov    esi, 0x45c                    ; -> loc_1_045C
loc_1_0067:
        mov    esi, dword ptr [esi]
        cmp    esi, 0x45c                    ; -> loc_1_045C
        je     loc_1_008F
        cmp    bl, byte ptr [esi + 0x10]
        jne    loc_1_0067
        mov    eax, edi
        mov    byte ptr [esi + 0x11], al
        mov    ax, word ptr [ebp + 0x1a]
        mov    word ptr [esi + 0x12], ax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp + 0x1c], eax
        mov    eax, 0x11c
        ret    
loc_1_008F:
        push   0
        push   0x14
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_00EC
        mov    ecx, edi
        mov    byte ptr [eax + 0x11], cl
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [0x464], ecx        ; -> obj1:0x0464
        mov    dword ptr [edi + 0x18], 0     ; -> loc_1_0000
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [eax + 8], edi
        mov    byte ptr [eax + 0x10], bl
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dword ptr [eax + 0xc], ebx
        mov    bx, word ptr [ebp + 0x1a]
        mov    word ptr [eax + 0x12], bx
        mov    dword ptr [ebp + 0x1c], eax
        pushfd 
        cli    
        mov    ebx, 0x45c                    ; -> loc_1_045C
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebx], eax
        popfd  
        sub    eax, eax
        ret    
loc_1_00EC:
        mov    eax, 5
        ret    
        db  000h, 000h
        dd  loc_1_011C               ; fixup
        dd  loc_1_0115               ; fixup
        dd  loc_1_0115               ; fixup
        dd  loc_1_011D               ; fixup
loc_1_0104:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        ret    
        db  0E8h, 001h, 000h, 000h, 000h, 0C3h
loc_1_0115:
        mov    word ptr [ebx + 2], 0xffff
        ret    
loc_1_011C:
        ret    
loc_1_011D:
        ret    
        db  000h, 000h
sub_1_0120:
        pushfd 
        mov    esi, dword ptr [edi + 8]
        mov    bx, word ptr [esi + 0xa]
        mov    ax, word ptr [esi + 8]
        shl    ax, 3
        add    bx, ax
        mov    eax, dword ptr [esi + 0xc]
        sub    esp, 0x10
        mov    esi, esp
        mov    word ptr [esi + 4], bx
        mov    dword ptr [esi + 6], eax
        mov    byte ptr [esi + 0xe], 2
        mov    word ptr [esi], 2
        push   esi
        call   dword ptr [0x468]             ; -> obj1:0x0468
        mov    ebx, dword ptr [esi + 0xa]
        add    esp, 0x14
        add    ebx, eax
        mov    dword ptr [ebx + 0x20], eax
        mov    eax, ebx
        add    eax, 0x58
        mov    dword ptr [ebx + 0x48], eax
        sub    eax, eax
        mov    word ptr [ebx + 4], ax
        mov    dword ptr [ebx + 0x14], eax
        mov    dword ptr [ebx + 0x18], eax
        mov    byte ptr [ebx + 0x4c], al
        mov    dword ptr [ebx + 8], 0x409
        mov    eax, dword ptr [edi + 8]
        popfd  
        clc    
        ret    
sub_1_0181:
        pushfd 
        sub    ebx, dword ptr [ebx + 0x20]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], ebx
        mov    word ptr [esi], 4
        push   esi
        call   dword ptr [0x468]             ; -> obj1:0x0468
        add    esp, 0xc
        popfd  
        ret    
sub_1_019E:
        push   ebp
        mov    esi, eax
        mov    al, byte ptr [edi + 0x10]
        mov    byte ptr [esi + 0x4d], al
        mov    edi, dword ptr [edi + 8]
        or     byte ptr [esi + 0xa], 8
        test   byte ptr [edi + 1], 0x40
        je     loc_1_01BE
        and    byte ptr [esi + 0xa], 0xf7
        mov    eax, dword ptr [edi + 0x20]
        add    dword ptr [esi + 0x10], eax
loc_1_01BE:
        mov    dword ptr [esi + 0x44], edi
        mov    dword ptr [esi], 0
        mov    edi, dword ptr [edi + 0x1c]
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        cld    
        pop    ebp
        ret    
        db  000h
loc_1_01D4:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        inc    edx
        add    al, byte ptr [eax]
        add    byte ptr [edx + eax], dl      ; -> loc_1_0214
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        add    byte ptr [edx], 0
        add    byte ptr [edx + eax], dl      ; -> loc_1_0214
        add    byte ptr [eax], al
        adc    al, 2
        add    byte ptr [eax], al
        inc    eax
        add    eax, dword ptr [eax]
        db  000h
loc_1_0214:
        push   ebp
        push   edi
        mov    ebp, dword ptr [esp + 0xc]
        mov    edi, dword ptr [ebp + 0x1c]
        movzx  eax, word ptr [edi + 0x12]
        cmp    eax, 0x15
        je     loc_1_023B
        cmp    eax, 0x28
        je     loc_1_023B
        mov    eax, 0x20
        cmp    byte ptr [edi + 0x11], 1
        je     loc_1_023B
        mov    eax, 0x1f
loc_1_023B:
        mov    word ptr [ebp + 0x1a], ax
        pop    edi
        pop    ebp
        ret    
loc_1_0242:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        xor    eax, eax
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        ret    
        db  000h
VDEF_Control:
        cmp    eax, 0
        je     loc_2_0000
        cmp    eax, 1
        je     loc_2_0002
        cmp    eax, 2
        je     loc_2_002B
        cmp    eax, 4
        je     loc_1_0279
        cmp    eax, 0x14
        je     loc_1_0277
        clc    
        ret    
loc_1_0277:
        clc    
        ret    
loc_1_0279:
        clc    
        ret    
        db  0F8h, 0C3h, 000h, 000h, 000h
loc_1_0280:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x1c]
        push   ebp
        mov    edi, dword ptr [ebp + 0x1c]
        test   byte ptr [edi + 0x11], 1
        jne    loc_1_0325
loc_1_0298:
        call   sub_1_0120
        mov    word ptr [ebx + 4], 0xd
        or     dword ptr [ebx + 8], 0x110
        mov    esi, edi
        mov    edi, ebx
        mov    ebx, dword ptr [ebp + 0x28]
        push   ebx
        mov    dword ptr [edi + 0x38], ebx
        mov    ax, word ptr [ebx + 0x10]
        mov    word ptr [edi + 0x2c], ax
        dec    al
        mov    byte ptr [edi + 0x4d], al
        movzx  eax, word ptr [ebx + 0x18]
        mov    dword ptr [edi + 0x30], eax
        test   byte ptr [ebp + 0x18], 1
        je     loc_1_02DB
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 0x34], eax
        and    byte ptr [edi + 8], 0xef
        jmp    loc_1_02E8
loc_1_02DB:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [edi + 0x34], eax
loc_1_02E8:
        mov    ax, word ptr [ebx + 0x1c]
        mov    word ptr [edi + 0x2e], ax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [edi + 0x44], eax
        mov    eax, edi
        push   edi
        mov    edi, esi
        call   sub_1_019E
        pop    edi
        pop    ebx
        pop    ebp
        mov    eax, dword ptr [edi + 0x3c]
        mov    word ptr [ebx + 0x1c], ax
        cmp    word ptr [edi + 6], 0x10
        jae    loc_1_0313
        xor    eax, eax
loc_1_0313:
        mov    word ptr [ebp + 0x1a], ax
        mov    ebx, edi
        call   sub_1_0181
loc_1_031E:
        pop    ebp
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_1_0325:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        cmp    ebx, dword ptr [edi + 0xc]
        pop    ebx
        je     loc_1_0298
        pop    ebp
        mov    word ptr [ebp + 0x1a], 5
        jmp    loc_1_031E
        db  000h
loc_1_0340:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        push   ebx
        push   esi
        push   edi
        xor    ax, ax
        mov    word ptr [ebp + 0x1a], ax
        cmp    byte ptr [ebp + 4], 2
        je     loc_1_03AA
        mov    edi, dword ptr [ebp + 0x1c]
        call   sub_1_03BD
        call   sub_1_0120
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx + 0x18], eax
        mov    eax, dword ptr [ebp + 0x24]
        mov    dword ptr [ebx + 0x10], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx + 0x1c], eax
        call   sub_1_043A
        or     byte ptr [ebx + 9], 1
        cmp    byte ptr [ebp + 4], 0
        je     loc_1_0389
        mov    word ptr [ebx + 4], 1
loc_1_0389:
        mov    eax, ebx
        call   sub_1_019E
        xor    ax, ax
        movzx  ecx, word ptr [ebx + 6]
        cmp    ecx, 0x10
        ja     loc_1_03AF
loc_1_039C:
        mov    word ptr [ebp + 0x1a], ax
        call   sub_1_0181
        call   sub_1_0419
loc_1_03AA:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_03AF:
        mov    eax, ecx
        movzx  ecx, word ptr [ebx + 4]
        VxDCall  BLOCKDEV.Service_000Eh          ; cd200e001000
        jmp    loc_1_039C
sub_1_03BD:
        mov    eax, dword ptr [ebp]
        mov    esi, dword ptr [edi + 8]
        mul    dword ptr [esi + 0x10]
        mov    edx, 0x80000000
        cmp    byte ptr [ebp + 4], 0
        je     loc_1_03D3
        xor    edx, edx
loc_1_03D3:
        or     edx, 0x40000000
        mov    esi, dword ptr [ebp + 0x14]
        add    eax, esi
        dec    eax
        shr    esi, 0xc
        shr    eax, 0xc
        sub    eax, esi
        inc    eax
        mov    dword ptr [ebp + 0x38], eax
        push   edx
        push   eax
        push   esi
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        and    eax, eax
        je     loc_1_0414
        mov    edx, dword ptr [ebp + 0x14]
        and    edx, 0xfff
        or     eax, edx
        mov    dword ptr [ebp + 0x14], eax
        shr    eax, 0xc
        mov    dword ptr [ebp + 0x34], eax
        or     byte ptr [ebp + 0x3c], 1
        ret    
loc_1_0414:
        or     byte ptr [ebp + 0x3c], 2
        ret    
sub_1_0419:
        test   byte ptr [ebp + 0x3c], 1
        je     loc_1_0439
        mov    esi, dword ptr [ebp + 0x34]
        mov    eax, dword ptr [ebp + 0x38]
        push   0x40000000
        push   eax
        push   esi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        xor    dword ptr [ebp + 0x3c], 1
loc_1_0439:
        ret    
sub_1_043A:
        test   byte ptr [ebp + 0x3c], 2
        jne    loc_1_0457
        push   edi
        push   ebx
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 0x44], eax
        push   ebx
        call   dword ptr [0x478]             ; -> obj1:0x0478
        or     eax, eax
        pop    eax
        pop    ebx
        pop    edi
        jne    loc_1_0457
        ret    
loc_1_0457:
        or     byte ptr [ebx + 8], 0x40
        ret    
loc_1_045C:
        pop    esp
        add    al, 0
        add    byte ptr [esp + eax], bl      ; -> loc_1_045C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 000h, 000h, 000h, 056h, 044h, 045h, 046h, 020h
        db  020h, 020h, 020h, 000h, 001h, 001h, 0A0h
        dd  VDEF_Control             ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h
        db  033h, 076h, 073h, 052h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x94  R-X
loc_2_0000:
        clc    
        ret    
loc_2_0002:
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_2_002A
        VxDCall  BLOCKDEV.BlockDev_Get_Version   ; cd2000001000
        jb     loc_2_002A
        push   1
        push   0x22
        push   0                             ; -> loc_1_0000
        VxDCall  IFSMgr.IFSMgr_RegisterMount     ; cd2001004000
        add    esp, 0xc
        cld    
        call   sub_2_002D
loc_2_002A:
        ret    
loc_2_002B:
        clc    
        ret    
sub_2_002D:
        mov    dword ptr [0x64], 0x80        ; -> loc_2_0064
        mov    dword ptr [0x68], 0x104       ; -> loc_2_0068
        mov    dword ptr [0x6c], 0x468       ; -> loc_2_006C
        push   0x5c                          ; -> loc_2_005C
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        clc    
        ret    
        db  000h
loc_2_005C:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
loc_2_0064:
        add    al, 0
        add    byte ptr [eax], al
loc_2_0068:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_006C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        popaw  
        jne    0xe2
        je     0x98
        inc    esi
        imul   ebp, dword ptr [ebp + 0x53], 0x447379
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
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x8  R-X
        db  033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h
VxD__LTEXT ENDS

; VCACHE -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2270 instructions recovered by recursive descent

Declare_Virtual_Device VCACHE, 3, 1, VCACHE_Control, 0048Bh, Undefined_Init_Order
;   DDK version 0400h   init_order 000000100h   flags 00000h
;   service table: obj1:0x83c, 25 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x16a0  R-X
        db  060h, 08Bh, 0FEh, 0FCh, 033h, 0C0h, 0B9h, 000h, 004h, 000h, 000h, 0F3h, 0ABh, 08Bh, 0C6h, 08Dh
        db  058h, 040h, 03Bh, 0DFh, 077h, 027h, 089h, 000h, 089h, 040h, 004h, 089h, 040h, 038h, 089h, 040h
        db  03Ch, 0B9h
        dd  loc_1_0A70               ; fixup
        db  08Bh, 051h, 004h, 089h, 002h, 089h, 008h, 089h, 050h, 004h, 089h, 041h, 004h, 0FFh, 005h
        dd  obj1:0x09A8              ; fixup
        db  08Bh, 0C3h, 0EBh, 0D2h, 08Bh, 00Dh
        dd  obj1:0x09B8              ; fixup
        db  08Bh, 03Dh
        dd  obj1:0x09B0              ; fixup
        db  033h, 0C0h, 0FCh, 0F2h, 0AFh, 089h, 077h, 0FCh, 0FFh, 005h
        dd  obj1:0x09B4              ; fixup
        db  061h, 0C3h
sub_1_0059:
        mov    ecx, dword ptr [ecx]
        push   ebx
        push   edx
        cmp    ecx, 0
        jge    loc_1_008F
loc_1_0062:
        mov    esi, dword ptr [0xa70]        ; -> loc_1_0A70
        mov    ebx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [ebx + 4], edx
        mov    dword ptr [edx], ebx
        mov    ebx, dword ptr [0x9a4]        ; -> obj1:0x09A4
        cmp    ebx, 0
        je     loc_1_00DA
        mov    edx, dword ptr [ebx + 0x100]
        mov    dword ptr [0x9a4], edx        ; -> obj1:0x09A4
        mov    dword ptr [esi + 0x10], ebx
        pop    edx
        pop    ebx
        ret    
loc_1_008F:
        cmp    dword ptr [0x9a8], 0          ; -> obj1:0x09A8
        jg     loc_1_00C3
        cmp    dword ptr [0x9ac], 2          ; -> obj1:0x09AC
        jb     loc_1_00DA
        mov    esi, dword ptr [0x9a4]        ; -> obj1:0x09A4
        cmp    esi, 0
        je     loc_1_00DA
        mov    ebx, dword ptr [esi + 0x100]
        mov    dword ptr [0x9a4], ebx        ; -> obj1:0x09A4
        dec    dword ptr [0x9ac]             ; -> obj1:0x09AC
        call   0
loc_1_00C3:
        cmp    dword ptr [0x9ac], 0          ; -> obj1:0x09AC
        je     loc_1_00DA
        dec    dword ptr [0x9ac]             ; -> obj1:0x09AC
        dec    dword ptr [0x9a8]             ; -> obj1:0x09A8
        jmp    loc_1_0062
loc_1_00DA:
        xor    esi, esi
        pop    edx
        pop    ebx
        ret    
sub_1_00DF:
        push   ebx
        push   edx
        mov    ebx, dword ptr [esi + 0x10]
        mov    edx, dword ptr [0x9a4]        ; -> obj1:0x09A4
        mov    dword ptr [ebx + 0x100], edx
        mov    dword ptr [0x9a4], ebx        ; -> obj1:0x09A4
        mov    dword ptr [esi + 0x10], 0
        mov    ebx, 0xa70                    ; -> loc_1_0A70
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [edx], esi
        mov    dword ptr [esi], ebx
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [ebx + 4], esi
        pop    edx
        pop    ebx
        ret    
VCache_RelinquishPage:
        push   edx
        pushfd 
        cli    
        mov    edx, dword ptr [0x910]        ; -> loc_1_0910
        sub    edx, dword ptr [0x9b4]        ; -> obj1:0x09B4
        cmp    edx, dword ptr [0x8f0]        ; -> obj1:0x08F0
        jbe    loc_1_01FD
        cmp    dword ptr [0x9ac], 0          ; -> obj1:0x09AC
        je     loc_1_0187
        mov    eax, dword ptr [0x9a4]        ; -> obj1:0x09A4
        cmp    eax, 0
        je     loc_1_01FD
        mov    edx, dword ptr [eax + 0x100]
        mov    dword ptr [0x9a4], edx        ; -> obj1:0x09A4
        dec    dword ptr [0x9ac]             ; -> obj1:0x09AC
loc_1_0156:
        dec    dword ptr [0x910]             ; -> loc_1_0910
        mov    edx, dword ptr [0x910]        ; -> loc_1_0910
        cmp    edx, 0x80
        jae    loc_1_0179
        shr    edx, 3
        mov    dword ptr [0x90c], edx        ; -> loc_1_090C
        mov    edx, dword ptr [0x910]        ; -> loc_1_0910
loc_1_0179:
        lea    edx, [edx + edx*2]
        shr    edx, 1
        mov    dword ptr [0x8f8], edx        ; -> loc_1_08F8
        popfd  
        pop    edx
        ret    
loc_1_0187:
        push   esi
        mov    esi, 0xa30                    ; -> loc_1_0A30
loc_1_018D:
        mov    esi, dword ptr [esi + 0x3c]
        cmp    esi, 0xa30                    ; -> loc_1_0A30
        je     loc_1_01FC
        test   dword ptr [esi + 0x30], 0xffffff
        jne    loc_1_018D
        mov    edx, dword ptr [0x908]        ; -> loc_1_0908
        sub    edx, dword ptr [esi + 0x34]
        cmp    edx, dword ptr [0x90c]        ; -> loc_1_090C
        jbe    loc_1_01FC
        movzx  edx, byte ptr [esi + 0x33]
        sub    edx, 0x64
        shl    edx, 4
        add    edx, 0x9c8                    ; -> obj1:0x09C8
        cmp    dword ptr [edx], 0
        jle    loc_1_018D
        dec    dword ptr [edx]
        call   sub_1_0354
        xor    ecx, ecx
        mov    byte ptr [esi + 0x33], cl
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0xc], ecx
        xchg   dword ptr [esi + 0x10], ecx
        mov    eax, 0xa70                    ; -> loc_1_0A70
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [edx], esi
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [eax + 4], esi
        inc    dword ptr [0x9a8]             ; -> obj1:0x09A8
        mov    eax, ecx
        pop    esi
        jmp    loc_1_0156
loc_1_01FC:
        pop    esi
loc_1_01FD:
        popfd  
        pop    edx
        sub    eax, eax
        ret    
VCache_UseThisPage:
        push   edx
        pushfd 
        cli    
        mov    edx, dword ptr [0x9a4]        ; -> obj1:0x09A4
        mov    dword ptr [eax + 0x100], edx
        mov    dword ptr [0x9a4], eax        ; -> obj1:0x09A4
        inc    dword ptr [0x9ac]             ; -> obj1:0x09AC
        inc    dword ptr [0x910]             ; -> loc_1_0910
        mov    edx, dword ptr [0x910]        ; -> loc_1_0910
        cmp    edx, 0x80
        ja     loc_1_023F
        shr    edx, 3
        mov    dword ptr [0x90c], edx        ; -> loc_1_090C
        mov    edx, dword ptr [0x910]        ; -> loc_1_0910
loc_1_023F:
        lea    edx, [edx + edx*2]
        shr    edx, 1
        mov    dword ptr [0x8f8], edx        ; -> loc_1_08F8
        popfd  
        pop    edx
        ret    
VCache_SwapBuffers:
        mov    al, byte ptr [esi + 0x33]
        cmp    byte ptr [edi + 0x33], al
        stc    
        jne    loc_1_026D
        cmp    word ptr [esi + 0x30], 1
        jb     loc_1_026D
        cmp    word ptr [edi + 0x30], 1
        jb     loc_1_026D
        mov    eax, dword ptr [esi + 0x10]
        xchg   dword ptr [edi + 0x10], eax
        mov    dword ptr [esi + 0x10], eax
loc_1_026D:
        ret    
VCACHE_Control:
        cmp    eax, 0
        je     loc_2_0000
        cmp    eax, 1
        je     loc_2_002D
        cmp    eax, 2
        je     loc_2_0273
        clc    
        ret    
VCache_Get_Version:
        mov    eax, 0x100
        xor    ebx, ebx
        clc    
        ret    
VCache_GetSize:
        sub    ah, 0x64
        jb     loc_1_02BA
        cmp    byte ptr [0x9a0], ah          ; -> obj1:0x09A0
        jbe    loc_1_02BA
        movzx  edx, ah
        shl    edx, 4
        mov    eax, dword ptr [0x910]        ; -> loc_1_0910
        sub    eax, dword ptr [0x924]        ; -> obj1:0x0924
        add    eax, dword ptr [edx + 0x9cc]  ; -> obj1:0x09CC
        jmp    loc_1_02BF
loc_1_02BA:
        mov    eax, dword ptr [0x910]        ; -> loc_1_0910
loc_1_02BF:
        mov    edx, dword ptr [0x8f4]        ; -> loc_1_08F4
        mov    ecx, dword ptr [0x8f0]        ; -> obj1:0x08F0
        ret    
VCache_CheckAvail:
        mov    dl, ah
        sub    ah, 0x64
        jb     loc_1_0350
        cmp    byte ptr [0x9a0], ah          ; -> obj1:0x09A0
        jbe    loc_1_0350
        movzx  eax, ah
        shl    eax, 4
        add    eax, 0x9c8                    ; -> obj1:0x09C8
        push   edi
        mov    edi, dword ptr [0x910]        ; -> loc_1_0910
        sub    edi, dword ptr [0x924]        ; -> obj1:0x0924
        sub    edi, dword ptr [eax]
        push   ecx
        mov    ebx, 0xa30                    ; -> loc_1_0A30
        sub    ecx, dword ptr [0x9ac]        ; -> obj1:0x09AC
        jbe    loc_1_0346
        sub    edi, dword ptr [0x9b4]        ; -> obj1:0x09B4
        jae    loc_1_030D
        sub    edi, edi
loc_1_030D:
        mov    eax, eax
loc_1_0310:
        mov    ebx, dword ptr [ebx + 0x3c]
        cmp    ebx, 0xa30                    ; -> loc_1_0A30
        je     loc_1_034A
        test   dword ptr [ebx + 0x30], 0xffffff
        jne    loc_1_0310
        cmp    byte ptr [ebx + 0x33], 0
        je     loc_1_033F
        mov    eax, dword ptr [0x908]        ; -> loc_1_0908
        sub    eax, dword ptr [ebx + 0x34]
        cmp    eax, dword ptr [0x8f8]        ; -> loc_1_08F8
        jb     loc_1_034A
        cmp    byte ptr [ebx + 0x33], dl
        je     loc_1_0344
loc_1_033F:
        or     edi, edi
        je     loc_1_0310
        dec    edi
loc_1_0344:
        loop   loc_1_0310
loc_1_0346:
        pop    eax
        pop    edi
        clc    
        ret    
loc_1_034A:
        pop    eax
        sub    eax, ecx
        pop    edi
        stc    
        ret    
loc_1_0350:
        xor    eax, eax
        stc    
        ret    
sub_1_0354:
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esi + 0x38]
        mov    edx, dword ptr [esi + 0x3c]
        mov    dword ptr [ecx + 0x3c], edx
        mov    dword ptr [edx + 0x38], ecx
        mov    ecx, dword ptr [esi + 8]
        or     ecx, dword ptr [esi + 0xc]
        je     loc_1_03B2
        movzx  ecx, byte ptr [esi + 0x33]
        mov    dword ptr [esi + 0x38], esi
        mov    dword ptr [esi + 0x3c], esi
        sti    
        pushal 
        inc    dword ptr [0x930]             ; -> obj1:0x0930
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [0x934]        ; -> obj1:0x0934
        xor    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edx*4 + 0x938], eax ; -> obj1:0x0938
        inc    edx
        cmp    edx, 0x1a
        jb     loc_1_039F
        xor    edx, edx
loc_1_039F:
        mov    dword ptr [0x934], edx        ; -> obj1:0x0934
        sub    ecx, 0x64
        shl    ecx, 4
        call   sub_1_09D0
        popal  
loc_1_03B2:
        ret    
        db  090h
VCache_FindBlock:
        push   ecx
        and    al, 0xdf
        push   eax
        mov    ecx, eax
        mov    eax, ebx
        or     eax, edi
        je     loc_1_0456
        xor    eax, eax
        shld   eax, ebx, 0x10
        xor    eax, ebx
        xor    eax, edi
        and    eax, dword ptr [0x914]        ; -> loc_1_0914
        add    eax, dword ptr [0x918]        ; -> loc_1_0918
        mov    esi, eax
loc_1_03DC:
        mov    esi, dword ptr [esi]
        cmp    esi, eax
        je     loc_1_0433
        cmp    dword ptr [esi + 8], ebx
        jne    loc_1_03DC
        cmp    dword ptr [esi + 0xc], edi
        jne    loc_1_03DC
        cmp    byte ptr [esi + 0x33], ch
        jne    loc_1_03DC
        test   cl, 2
        je     loc_1_03FC
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
loc_1_03FC:
        mov    eax, 0xa30                    ; -> loc_1_0A30
        mov    ecx, 0x1a
        mov    eax, eax
loc_1_0408:
        mov    eax, dword ptr [eax + 0x3c]
        cmp    eax, 0xa30                    ; -> loc_1_0A30
        je     loc_1_041E
        cmp    eax, esi
        loopne loc_1_0408
        jne    loc_1_041E
        inc    dword ptr [0x92c]             ; -> obj1:0x092C
loc_1_041E:
        pop    eax
        pop    ecx
        test   al, 4
        jne    loc_1_0428
        mov    eax, dword ptr [esi + 0x10]
        ret    
loc_1_0428:
        VxDCall  Dev_048B.Service_0006h          ; cd2006008b04
        clc    
        mov    eax, dword ptr [esi + 0x10]
        ret    
loc_1_0433:
        push   eax
        push   ecx
        push   edi
        mov    eax, ebx
        mov    ecx, 0x1a
        xor    eax, edi
        mov    edi, 0x938                    ; -> obj1:0x0938
        repne scasd eax, dword ptr es:[edi]
        jne    loc_1_044E
        inc    dword ptr [0x928]             ; -> obj1:0x0928
loc_1_044E:
        pop    edi
        pop    ecx
        pop    eax
        test   cl, 1
        jne    loc_1_0460
loc_1_0456:
        pop    eax
        pop    ecx
        xor    esi, esi
        stc    
        ret    
loc_1_045C:
        pop    ebp
        pop    eax
        jmp    loc_1_0456
loc_1_0460:
        sub    edx, edx
        mov    dl, ch
        sub    dl, 0x64
        jb     loc_1_0456
        cmp    dl, byte ptr [0x9a0]          ; -> obj1:0x09A0
        jae    loc_1_0456
        shl    edx, 4
        add    edx, 0x9c8                    ; -> obj1:0x09C8
        push   edx
        push   ebp
        push   ecx
        cli    
        mov    ecx, edx
        call   sub_1_0059
        test   esi, esi
        je     loc_1_0495
        mov    ecx, dword ptr [esp + 8]
        inc    dword ptr [ecx]
        pop    ecx
        jmp    loc_1_05B8
loc_1_0495:
        sti    
        pop    ecx
        bt     ecx, 4
        cmc    
        sbb    ebp, ebp
        and    ebp, dword ptr [0x8f8]        ; -> loc_1_08F8
        mov    esi, 0xa30                    ; -> loc_1_0A30
        mov    eax, eax
loc_1_04AC:
        mov    esi, dword ptr [esi + 0x3c]
        cmp    esi, 0xa30                    ; -> loc_1_0A30
        je     loc_1_045C
        test   dword ptr [esi + 0x30], 0xffffff
        jne    loc_1_04AC
        push   0x4ac                         ; -> loc_1_04AC
        cmp    byte ptr [esi + 0x33], 0
        je     sub_1_0578
        pop    ecx
        mov    ecx, dword ptr [0x908]        ; -> loc_1_0908
        sub    ecx, dword ptr [esi + 0x34]
        push   0x4ac                         ; -> loc_1_04AC
        cmp    ebp, ecx
        jbe    sub_1_0578
        pop    ecx
        mov    ecx, dword ptr [0x904]        ; -> obj1:0x0904
loc_1_04ED:
        mov    edx, dword ptr [0x9b0]        ; -> obj1:0x09B0
        lea    esi, [edx + ecx*4]
        ror    ecx, 0x10
        mov    edx, dword ptr [esi]
        cmp    edx, 0
        je     loc_1_0556
        mov    esi, dword ptr [0x900]        ; -> loc_1_0900
        and    esi, 0xfff
        add    esi, edx
        add    edx, 0xfc0
        mov    cx, 0x40
loc_1_0518:
        add    esi, 0x40
        cmp    esi, edx
        jbe    loc_1_0527
        mov    esi, edx
        sub    esi, 0xfc0
loc_1_0527:
        cmp    dword ptr [esi + 0x10], 0
        je     loc_1_054E
        test   dword ptr [esi + 0x30], 0xffffff
        jne    loc_1_054E
        push   edx
        mov    edx, dword ptr [0x908]        ; -> loc_1_0908
        sub    edx, dword ptr [esi + 0x34]
        cmp    edx, dword ptr [0x90c]        ; -> loc_1_090C
        pop    edx
        jbe    loc_1_054E
        call   sub_1_0578
loc_1_054E:
        dec    cx
        cmp    cx, 0
        jne    loc_1_0518
loc_1_0556:
        ror    ecx, 0x10
        inc    ecx
        cmp    ecx, dword ptr [0x9b4]        ; -> obj1:0x09B4
        jb     loc_1_0564
        sub    ecx, ecx
loc_1_0564:
        cmp    ecx, dword ptr [0x904]        ; -> obj1:0x0904
        jne    loc_1_04ED
        jmp    loc_1_045C
loc_1_0571:
        sti    
        pop    edx
        pop    ecx
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_0578:
        cli    
        push   ecx
        push   edx
        mov    ecx, dword ptr [esp + 0x10]
        movzx  edx, byte ptr [esp + 0x15]
        cmp    dword ptr [ecx + 8], -1
        je     loc_1_0571
        cmp    byte ptr [esi + 0x33], dl
        je     loc_1_05AA
        movzx  edx, byte ptr [esi + 0x33]
        sub    edx, 0x64
        jb     loc_1_05A8
        shl    edx, 4
        add    edx, 0x9c8                    ; -> obj1:0x09C8
        cmp    dword ptr [edx], 0
        jle    loc_1_0571
        dec    dword ptr [edx]
loc_1_05A8:
        inc    dword ptr [ecx]
loc_1_05AA:
        pop    ecx
        pop    ecx
        pop    ecx
        cmp    byte ptr [esi + 0x33], 0
        je     loc_1_05B8
        call   sub_1_0354
loc_1_05B8:
        xor    ecx, ecx
        mov    dword ptr [esi + 0x14], ecx
        mov    dword ptr [esi + 0x18], ecx
        mov    dword ptr [esi + 0x1c], ecx
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], ecx
        mov    dword ptr [esi + 0x28], ecx
        mov    dword ptr [esi + 0x2c], ecx
        cli    
        mov    dword ptr [esi + 8], ebx
        mov    dword ptr [esi + 0xc], edi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [eax], esi
        mov    edx, 0xa30                    ; -> loc_1_0A30
        mov    ecx, dword ptr [edx + 0x38]
        mov    dword ptr [esi + 0x38], ecx
        mov    dword ptr [esi + 0x3c], edx
        mov    dword ptr [ecx + 0x3c], esi
        mov    dword ptr [edx + 0x38], esi
        pop    ebp
        pop    eax
        pop    eax
        mov    byte ptr [esi + 0x33], ah
        mov    dword ptr [0x900], esi        ; -> loc_1_0900
        mov    ecx, dword ptr [0x904]        ; -> obj1:0x0904
        inc    ecx
        cmp    ecx, dword ptr [0x9b4]        ; -> obj1:0x09B4
        jb     loc_1_0613
        xor    ecx, ecx
loc_1_0613:
        mov    dword ptr [0x904], ecx        ; -> obj1:0x0904
        sti    
        mov    ecx, dword ptr [0x908]        ; -> loc_1_0908
        mov    dword ptr [esi + 0x34], ecx
        inc    ecx
        mov    dword ptr [0x908], ecx        ; -> loc_1_0908
        test   al, 2
        je     loc_1_0634
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
loc_1_0634:
        mov    eax, dword ptr [esi + 0x10]
        pop    ecx
        stc    
        ret    
        db  08Bh, 0C0h
VCache_FreeBlock:
        pushfd 
        cli    
        cmp    dword ptr [esi + 0x38], esi
        je     loc_1_06A5
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [esi], esi
        mov    dword ptr [esi + 4], esi
        mov    ecx, dword ptr [esi + 0x38]
        mov    edx, dword ptr [esi + 0x3c]
        mov    dword ptr [ecx + 0x3c], edx
        mov    dword ptr [edx + 0x38], ecx
        mov    dword ptr [esi + 0x38], esi
        mov    dword ptr [esi + 0x3c], esi
        xor    ecx, ecx
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0xc], ecx
        mov    word ptr [esi + 0x30], cx
        mov    cl, byte ptr [esi + 0x33]
        sub    ecx, 0x64
        shl    ecx, 4
        mov    dword ptr [esi + 0x30], 0
        push   ecx
        call   sub_1_00DF
        pop    ecx
        mov    edx, dword ptr [ecx + 0x9c8]  ; -> obj1:0x09C8
        cmp    edx, 0
        jle    loc_1_069E
        inc    dword ptr [0x9ac]             ; -> obj1:0x09AC
        inc    dword ptr [0x9a8]             ; -> obj1:0x09A8
loc_1_069E:
        dec    edx
        mov    dword ptr [ecx + 0x9c8], edx  ; -> obj1:0x09C8
loc_1_06A5:
        popfd  
        ret    
        db  090h
VCache_MakeMRU:
        mov    eax, 0xa30                    ; -> loc_1_0A30
        cmp    dword ptr [eax + 0x38], esi
        je     loc_1_06CD
        mov    edx, dword ptr [esi + 0x38]
        mov    ecx, dword ptr [esi + 0x3c]
        mov    dword ptr [edx + 0x3c], ecx
        mov    dword ptr [ecx + 0x38], edx
        mov    ecx, dword ptr [eax + 0x38]
        mov    dword ptr [esi + 0x38], ecx
        mov    dword ptr [esi + 0x3c], eax
        mov    dword ptr [ecx + 0x3c], esi
        mov    dword ptr [eax + 0x38], esi
loc_1_06CD:
        mov    ecx, dword ptr [0x908]        ; -> loc_1_0908
        mov    dword ptr [esi + 0x34], ecx
        inc    ecx
        mov    dword ptr [0x908], ecx        ; -> loc_1_0908
        ret    
        db  08Bh, 0C0h
VCache_Hold:
        inc    word ptr [esi + 0x30]
        ret    
        db  02Eh, 08Bh, 0C0h
VCache_Unhold:
        dec    word ptr [esi + 0x30]
        ret    
        db  02Eh, 08Bh, 0C0h
VCache_TestHold:
        movzx  eax, word ptr [esi + 0x30]
        or     eax, eax
        ret    
VCache_Enum:
        cmp    ah, 0x64
        jb     loc_1_074C
        xor    edi, edi
        mov    esi, dword ptr [0x9b0]        ; -> obj1:0x09B0
loc_1_0704:
        cmp    edi, dword ptr [0x9b8]        ; -> obj1:0x09B8
        jae    loc_1_074C
        push   esi
        push   edi
        mov    esi, dword ptr [esi]
        cmp    esi, 0
        je     loc_1_0727
        lea    edi, [esi + 0xfc0]
loc_1_071B:
        cmp    byte ptr [esi + 0x33], ah
        je     loc_1_072F
loc_1_0720:
        add    esi, 0x40
        cmp    esi, edi
        jbe    loc_1_071B
loc_1_0727:
        pop    edi
        pop    esi
        inc    edi
        add    esi, 4
        jmp    loc_1_0704
loc_1_072F:
        cmp    dword ptr [esi + 8], 0
        jne    loc_1_073B
        cmp    dword ptr [esi + 0xc], 0
        je     loc_1_0720
loc_1_073B:
        push   eax
        push   esi
        push   edx
        push   edi
        cmp    word ptr [esi + 0x30], 0
        call   edx
        pop    edi
        pop    edx
        pop    esi
        pop    eax
        jmp    loc_1_0720
loc_1_074C:
        ret    
VCACHE_PM_API:
        stc    
        ret    
        db  090h
VCache_TestHandle:
        pushfd 
        push   eax
        push   ecx
        push   edx
        mov    ecx, dword ptr [0x9b8]        ; -> obj1:0x09B8
        mov    edx, dword ptr [0x9b0]        ; -> obj1:0x09B0
        mov    eax, esi
        sub    eax, 0x1000
        jb     loc_1_0793
loc_1_0769:
        cmp    dword ptr [edx], eax
        ja     loc_1_0774
loc_1_076D:
        add    edx, 4
        loop   loc_1_0769
        jmp    loc_1_0793
loc_1_0774:
        cmp    dword ptr [edx], esi
        ja     loc_1_076D
        mov    eax, esi
        sub    eax, dword ptr [edx]
        xor    edx, edx
        mov    ecx, 0x40
        div    ecx
        test   edx, edx
        jne    loc_1_0793
        pop    edx
        pop    ecx
        pop    eax
        cmp    byte ptr [esi + 0x33], ah
        jne    loc_1_0798
loc_1_0791:
        popfd  
        ret    
loc_1_0793:
        pop    edx
        pop    ecx
        pop    eax
        jmp    loc_1_0791
loc_1_0798:
        jmp    loc_1_0791
VCache_RecalcSums:
        ret    
        db  090h
VCache_VerifySums:
        xor    ecx, ecx
        ret    
        db  090h
VCache_GetStats:
        xor    ebx, ebx
        xor    ecx, ecx
        xor    edi, edi
        xchg   dword ptr [0x928], ebx        ; -> obj1:0x0928
        xchg   dword ptr [0x92c], ecx        ; -> obj1:0x092C
        xchg   dword ptr [0x930], edi        ; -> obj1:0x0930
        mov    edx, dword ptr [0x8fc]        ; -> loc_1_08FC
        ret    
loc_1_07BF:
        push   0
        push   0
        push   0
        push   0x1290                        ; -> loc_1_1290
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0x10
        ret    
sub_1_07D4:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_07DA:
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
sub_1_07E0:
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
        add    byte ptr [eax], al
sub_1_07E8:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_1_07F0:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_1_07F8:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        add    byte ptr [eax], al
sub_1_0800:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        add    byte ptr [eax], al
sub_1_0808:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        add    byte ptr [eax], al
sub_1_0810:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        add    byte ptr [eax], al
sub_1_0818:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_1_0834:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        add    byte ptr [eax], al
        dd  VCache_Get_Version       ; fixup
        dd  VCache_Register          ; fixup
        dd  VCache_GetSize           ; fixup
        dd  VCache_CheckAvail        ; fixup
        dd  VCache_FindBlock         ; fixup
        dd  VCache_FreeBlock         ; fixup
        dd  VCache_MakeMRU           ; fixup
        dd  VCache_Hold              ; fixup
        dd  VCache_Unhold            ; fixup
        dd  VCache_Enum              ; fixup
        dd  VCache_TestHandle        ; fixup
        dd  VCache_VerifySums        ; fixup
        dd  VCache_RecalcSums        ; fixup
        dd  VCache_TestHold          ; fixup
        dd  VCache_GetStats          ; fixup
        dd  VCache_Deregister        ; fixup
        dd  VCache_AdjustMinimum     ; fixup
        dd  VCache_SwapBuffers       ; fixup
        dd  VCache_RelinquishPage    ; fixup
        dd  VCache_UseThisPage       ; fixup
        dd  _VCache_CreateLookupCache ; fixup
        dd  _VCache_CloseLookupCache ; fixup
        dd  _VCache_DeleteLookupCache ; fixup
        dd  _VCache_Lookup           ; fixup
        dd  _VCache_UpdateLookup     ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0048Bh                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VCACHE  '                  ; DDB_Name
        dd  000000100h                  ; DDB_Init_Order
        dd  VCACHE_Control           ; DDB_Control_Proc
        dd  VCACHE_PM_API            ; DDB_V86_API_Proc
        dd  VCACHE_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x083C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 011h, 0F1h, 0FFh, 0FFh
loc_1_08F4:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_08F8:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_08FC:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_0900:
        adc    ecx, esi
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_0908:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_090C:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_0910:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_0914:
        adc    ecx, esi
        db  0FFh, 0FFh
loc_1_0918:
        adc    ecx, esi
        db  0FFh, 0FFh, 011h, 0F1h, 0FFh, 0FFh
loc_1_0920:
        adc    ecx, esi
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_09D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A30:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0A80:
        push   esi
        sub    eax, eax
        cmp    ecx, eax
        je     loc_1_0A90
loc_1_0A87:
        movzx  esi, byte ptr [edx]
        add    eax, esi
        inc    edx
        dec    ecx
        jne    loc_1_0A87
loc_1_0A90:
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AA0:
        cmp    dword ptr [0x15e8], 0         ; -> obj1:0x15E8
        jne    loc_1_0ABF
        push   0
        push   0x493e0
        push   0x7bf                         ; -> loc_1_07BF
        call   sub_1_0818
        mov    dword ptr [0x15e8], eax       ; -> obj1:0x15E8
loc_1_0ABF:
        ret    
sub_1_0AC0:
        push   esi
        push   edi
        mov    edi, dword ptr [ecx + 8]
        mov    esi, dword ptr [edi]
        cmp    ecx, esi
        je     loc_1_0AEB
        mov    edx, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ecx], esi
        mov    dword ptr [ecx + 4], edi
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [edi], ecx
        or     dword ptr [edi + 0x1c], 0x20000
        call   sub_1_0AA0
loc_1_0AEB:
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_1_0AF0:
        push   ebx
        push   esi
        mov    eax, dword ptr [0x15e0]       ; -> obj1:0x15E0
        or     eax, eax
        je     loc_1_0B30
loc_1_0AFB:
        mov    edx, ecx
        mov    esi, dword ptr [eax + 0xc]
loc_1_0B00:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [esi]
        jne    loc_1_0B20
        or     bl, bl
        je     loc_1_0B1C
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    loc_1_0B20
        add    edx, 2
        add    esi, 2
        or     bl, bl
        jne    loc_1_0B00
loc_1_0B1C:
        sub    edx, edx
        jmp    loc_1_0B25
loc_1_0B20:
        sbb    edx, edx
        sbb    edx, -1
loc_1_0B25:
        or     edx, edx
        je     loc_1_0B32
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    loc_1_0AFB
loc_1_0B30:
        sub    eax, eax
loc_1_0B32:
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B40:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    dword ptr [esp + 0x1c], 4
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esi + 0x1c]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   0
        push   0x15f0                        ; -> loc_1_15F0
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0808
        add    esp, 0x18
        or     eax, eax
        jne    loc_1_0DA8
        mov    dword ptr [esp + 0x1c], 4
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esi + 0x14]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   0
        push   0x15f8                        ; -> loc_1_15F8
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0808
        add    esp, 0x18
        or     eax, eax
        jne    loc_1_0DA8
        mov    dword ptr [esp + 0x1c], 4
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esi + 0x18]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   0
        push   0x1604                        ; -> obj1:0x1604
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0808
        add    esp, 0x18
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_0BE4
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
loc_1_0BE4:
        sub    edi, edi
        cmp    ebp, edi
        jne    loc_1_0DA6
        mov    edi, edi
loc_1_0BF0:
        cmp    dword ptr [esi + 0x14], edi
        jbe    loc_1_0DA6
        push   edi
        push   0x1610                        ; -> obj1:0x1610
        lea    eax, [esp + 0x28]
        push   0x1614                        ; -> loc_1_1614
        push   eax
        call   sub_1_0834
        add    esp, 0x10
        push   edi
        push   0x161c                        ; -> obj1:0x161C
        lea    eax, [esp + 0x30]
        push   0x1624                        ; -> loc_1_1624
        push   eax
        call   sub_1_0834
        mov    dword ptr [esp + 0x20], 0
        lea    eax, [esp + 0x20]
        add    esp, 0x10
        push   eax
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        lea    eax, [esp + 0x2c]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0808
        add    esp, 0x18
        mov    ebp, eax
        cmp    ebp, 0xea
        je     loc_1_0C62
        or     ebp, ebp
        jne    loc_1_0D79
loc_1_0C62:
        mov    dword ptr [esp + 0x14], 0
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        lea    eax, [esp + 0x34]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0808
        add    esp, 0x18
        mov    ebp, eax
        cmp    ebp, 0xea
        je     loc_1_0C9B
        or     ebp, ebp
        jne    loc_1_0D79
loc_1_0C9B:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        lea    eax, [eax + ecx + 0x28]
        push   eax
        call   sub_1_07D4
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_0D70
        mov    dword ptr [ebx + 8], esi
        lea    ecx, [ebx + 0x28]
        mov    dword ptr [ebx + 0x20], ecx
        mov    eax, dword ptr [esp + 0x10]
        add    eax, ecx
        mov    dword ptr [ebx + 0x24], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebx + 0x14], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x10], eax
        lea    eax, [esp + 0x10]
        mov    dword ptr [ebx + 0x18], edi
        mov    dword ptr [ebx + 0x1c], 0
        push   eax
        lea    eax, [esp + 0x1c]
        push   ecx
        push   eax
        lea    eax, [esp + 0x2c]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0808
        add    esp, 0x18
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_0D60
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [ebx + 0x24]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x34]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0808
        add    esp, 0x18
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_0D55
        mov    edx, dword ptr [ebx + 0x20]
        mov    ecx, dword ptr [esp + 0x10]
        call   sub_1_0A80
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 4], ebx
        mov    dword ptr [ebx], esi
        jmp    loc_1_0D75
loc_1_0D55:
        push   ebx
        call   sub_1_07DA
        add    esp, 4
        jmp    loc_1_0D75
loc_1_0D60:
        push   ebx
        call   sub_1_07DA
        add    esp, 4
        jmp    loc_1_0D75
        db  005h, 000h, 000h, 000h, 000h
loc_1_0D70:
        mov    ebp, 0xe
loc_1_0D75:
        or     ebp, ebp
        je     loc_1_0D9D
loc_1_0D79:
        cmp    dword ptr [esi + 4], esi
        je     loc_1_0D9D
        mov    edi, edi
loc_1_0D80:
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    ebx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [esi + 4], ebx
        call   sub_1_07DA
        add    esp, 4
        cmp    ebx, esi
        jne    loc_1_0D80
loc_1_0D9D:
        inc    edi
        or     ebp, ebp
        je     loc_1_0BF0
loc_1_0DA6:
        mov    eax, ebp
loc_1_0DA8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
sub_1_0DB0:
        or     dword ptr [ecx + 0x1c], 0x10000
        jmp    sub_1_0AA0
        db  08Dh, 064h, 024h, 000h
sub_1_0DC0:
        or     dword ptr [ecx + 0x1c], 0x10000
        mov    ecx, dword ptr [ecx + 8]
        jmp    sub_1_0DB0
        db  090h
sub_1_0DD0:
        mov    eax, dword ptr [ecx + 8]
        or     dword ptr [ecx + 0x1c], 0x20000
        dec    dword ptr [eax + 0x14]
        or     dword ptr [eax + 0x1c], 0x20000
        jmp    sub_1_0AA0
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DF0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, edx
        mov    esi, ecx
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, 0x24
        push   ecx
        call   sub_1_07D4
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_0E87
        lea    edx, [ebp + 0x24]
        mov    dword ptr [ebp + 0xc], edx
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [ebp + 0x20], 0
        mov    dword ptr [ebp + 0x1c], 0
        mov    dword ptr [ebp + 0x10], 0
        mov    dword ptr [ebp], ebp
        mov    ecx, ebp
        mov    dword ptr [ebp + 4], ebp
        mov    dword ptr [ebp + 0x14], 0
        mov    dword ptr [ebp + 0x18], ebx
        mov    eax, dword ptr [0x15e0]       ; -> obj1:0x15E0
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [0x15e0], ebp       ; -> obj1:0x15E0
        call   sub_1_0DB0
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ebp
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_1_0E87:
        mov    eax, 0xe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0EA0:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], edx
        mov    ebp, ecx
        lea    eax, [esp + 0x14]
        push   eax
        push   ebp
        mov    eax, dword ptr [0x169c]       ; -> loc_1_169C
        push   eax
        call   sub_1_07E8
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        jne    loc_1_0F91
        call   sub_1_07E0
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, 0x24
        push   ecx
        call   sub_1_07D4
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_0F85
        lea    edx, [ebx + 0x24]
        mov    dword ptr [ebx + 0xc], edx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebx + 0x20], eax
        mov    dword ptr [ebx + 0x10], 0
        mov    dword ptr [ebx], ebx
        mov    ecx, ebx
        mov    dword ptr [ebx + 4], ebx
        mov    dword ptr [ebx + 0x1c], 0
        call   sub_1_0B40
        mov    esi, eax
        or     esi, esi
        je     loc_1_0F70
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_1_07F0
        add    esp, 4
        push   ebp
        mov    eax, dword ptr [0x169c]       ; -> loc_1_169C
        push   eax
        call   sub_1_0800
        add    esp, 8
        push   ebx
        call   sub_1_07DA
        add    esp, 4
        call   sub_1_07E0
        jmp    loc_1_0F95
        db  08Dh, 049h, 000h
loc_1_0F70:
        mov    eax, dword ptr [0x15e0]       ; -> obj1:0x15E0
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [0x15e0], ebx       ; -> obj1:0x15E0
        call   sub_1_07E0
        jmp    loc_1_0F95
loc_1_0F85:
        mov    esi, 0xe
        call   sub_1_07E0
        jmp    loc_1_0F95
loc_1_0F91:
        mov    ebx, dword ptr [esp + 0x18]
loc_1_0F95:
        or     esi, esi
        jne    loc_1_0FA6
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_0FA6
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ebx
loc_1_0FA6:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_0FB0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], edx
        mov    ebx, ecx
        mov    edx, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x10]
        call   sub_1_0A80
        mov    edx, eax
        mov    eax, ebx
        mov    edi, edi
loc_1_0FD0:
        mov    eax, dword ptr [eax]
        cmp    eax, ebx
        je     loc_1_100D
        cmp    dword ptr [eax + 0xc], edx
        jne    loc_1_0FD0
        test   byte ptr [eax + 0x1e], 2
        jne    loc_1_0FD0
        mov    ebp, dword ptr [eax + 0x10]
        cmp    dword ptr [esp + 0x10], ebp
        jne    loc_1_0FD0
        mov    ecx, ebp
        shr    ecx, 2
        mov    esi, dword ptr [eax + 0x20]
        mov    edi, dword ptr [esp + 0x18]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_0FD0
        mov    ecx, ebp
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_0FD0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
loc_1_100D:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1020:
        push   0x169c                        ; -> loc_1_169C
        push   0x162c                        ; -> loc_1_162C
        push   0x80000002
        call   sub_1_07F8
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1045
        mov    dword ptr [0x15ec], 1         ; -> obj1:0x15EC
loc_1_1045:
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1050:
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, ecx
        mov    esi, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        push   0x1660                        ; -> loc_1_1660
        lea    eax, [esp + 0x10]
        push   0x1664                        ; -> loc_1_1664
        push   eax
        call   sub_1_0834
        add    esp, 0x10
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        mov    eax, dword ptr [edi + 0x20]
        push   eax
        push   3
        lea    eax, [esp + 0x14]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0810
        add    esp, 0x18
        or     eax, eax
        jne    loc_1_10CE
        mov    eax, dword ptr [edi + 0x18]
        push   eax
        push   0x166c                        ; -> loc_1_166C
        lea    eax, [esp + 0x10]
        push   0x1674                        ; -> loc_1_1674
        push   eax
        call   sub_1_0834
        add    esp, 0x10
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        mov    eax, dword ptr [edi + 0x24]
        push   eax
        push   3
        lea    eax, [esp + 0x14]
        push   0
        push   eax
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0810
        add    esp, 0x18
loc_1_10CE:
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10E0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    eax, dword ptr [esi + 0x1c]
        test   eax, 0x20000
        je     loc_1_11E0
        and    eax, 0xfffdffff
        mov    dword ptr [esi + 0x1c], eax
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_07F0
        add    esp, 4
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        mov    eax, dword ptr [0x169c]       ; -> loc_1_169C
        push   eax
        call   sub_1_0800
        add    esp, 8
        lea    eax, [esi + 0x20]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        mov    eax, dword ptr [0x169c]       ; -> loc_1_169C
        push   eax
        call   sub_1_07F8
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1175
        sub    edi, edi
        mov    ebx, dword ptr [esi]
        cmp    esi, ebx
        je     loc_1_1172
loc_1_1143:
        test   byte ptr [ebx + 0x1e], 2
        je     loc_1_1161
        mov    ebp, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax + 4], ebp
        push   ebx
        call   sub_1_07DA
        add    esp, 4
        mov    ebx, ebp
        jmp    loc_1_116C
loc_1_1161:
        mov    dword ptr [ebx + 0x18], edi
        inc    edi
        mov    ecx, ebx
        call   sub_1_1050
loc_1_116C:
        mov    ebx, dword ptr [ebx]
        cmp    esi, ebx
        jne    loc_1_1143
loc_1_1172:
        mov    dword ptr [esi + 0x14], edi
loc_1_1175:
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_1214
        push   4
        lea    eax, [esi + 0x14]
        push   eax
        push   3
        push   0
        push   0x167c                        ; -> loc_1_167C
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0810
        add    esp, 0x18
        or     eax, eax
        jne    loc_1_1214
        push   4
        lea    eax, [esi + 0x18]
        push   eax
        push   3
        push   0
        push   0x1688                        ; -> obj1:0x1688
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0810
        add    esp, 0x18
        or     eax, eax
        jne    loc_1_1214
        push   4
        lea    eax, [esi + 0x1c]
        push   eax
        push   3
        push   0
        push   0x1694                        ; -> obj1:0x1694
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_0810
        add    esp, 0x18
        jmp    loc_1_1214
        db  005h, 000h, 000h, 000h, 000h
loc_1_11E0:
        test   eax, 0x10000
        je     loc_1_1214
        and    eax, 0xfffeffff
        mov    dword ptr [esi + 0x1c], eax
        mov    edi, dword ptr [esi]
        cmp    esi, edi
        je     loc_1_1214
loc_1_11F5:
        mov    eax, dword ptr [edi + 0x1c]
        test   eax, 0x10000
        je     loc_1_120E
        and    eax, 0xfffeffff
        mov    dword ptr [edi + 0x1c], eax
        mov    ecx, edi
        call   sub_1_1050
loc_1_120E:
        mov    edi, dword ptr [edi]
        cmp    esi, edi
        jne    loc_1_11F5
loc_1_1214:
        cmp    dword ptr [esi + 0x10], 0
        jne    loc_1_127B
        mov    edi, dword ptr [esi]
        mov    ecx, edi
        cmp    esi, ecx
        je     loc_1_123B
loc_1_1222:
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        push   ecx
        call   sub_1_07DA
        add    esp, 4
        mov    ecx, edi
        cmp    esi, edi
        jne    loc_1_1222
loc_1_123B:
        mov    ecx, dword ptr [0x15e0]       ; -> obj1:0x15E0
        cmp    esi, ecx
        jne    loc_1_1250
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [0x15e0], eax       ; -> obj1:0x15E0
        jmp    loc_1_1266
        db  090h
loc_1_1250:
        cmp    dword ptr [ecx + 8], esi
        je     loc_1_125D
loc_1_1255:
        mov    ecx, dword ptr [ecx + 8]
        cmp    dword ptr [ecx + 8], esi
        jne    loc_1_1255
loc_1_125D:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], eax
loc_1_1266:
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        call   sub_1_07F0
        add    esp, 4
        push   esi
        call   sub_1_07DA
        add    esp, 4
loc_1_127B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1290:
        push   esi
        mov    dword ptr [0x15ec], 2         ; -> obj1:0x15EC
        mov    dword ptr [0x15e8], 0         ; -> obj1:0x15E8
        mov    esi, dword ptr [0x15e0]       ; -> obj1:0x15E0
        or     esi, esi
        je     loc_1_12BE
        nop    
loc_1_12B0:
        mov    ecx, esi
        call   sub_1_10E0
        mov    esi, dword ptr [esi + 8]
        or     esi, esi
        jne    loc_1_12B0
loc_1_12BE:
        mov    dword ptr [0x15ec], 1         ; -> obj1:0x15EC
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_12D0:
        mov    eax, dword ptr [0x15e0]       ; -> obj1:0x15E0
        or     eax, eax
        je     loc_1_12E4
loc_1_12D9:
        cmp    eax, ecx
        je     loc_1_12E4
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    loc_1_12D9
loc_1_12E4:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_VCache_CreateLookupCache:
        sub    esp, 8
        push   esi
        push   edi
        cmp    dword ptr [0x15ec], 1         ; -> obj1:0x15EC
        je     loc_1_1309
        mov    eax, 1
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_1_1309:
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, esi
        call   sub_1_0AF0
        mov    dword ptr [esp + 8], eax
        or     eax, eax
        jne    loc_1_1380
        lea    edx, [esp + 8]
        mov    ecx, esi
        call   sub_1_0EA0
        mov    edi, eax
        cmp    edi, 0xe
        jne    loc_1_1361
        push   0x2000
        call   sub_1_07D4
        mov    dword ptr [esp + 0xc], eax
        add    esp, 4
        or     eax, eax
        je     loc_1_1361
        push   eax
        call   sub_1_07DA
        mov    dword ptr [esp + 0xc], 0
        lea    edx, [esp + 0xc]
        add    esp, 4
        mov    ecx, esi
        call   sub_1_0EA0
        mov    edi, eax
loc_1_1361:
        or     edi, edi
        je     loc_1_1388
        lea    eax, [esp + 8]
        push   eax
        mov    ecx, esi
        mov    eax, dword ptr [esp + 0x20]
        mov    edx, dword ptr [esp + 0x1c]
        push   eax
        call   sub_1_0DF0
        mov    edi, eax
        jmp    loc_1_1384
        db  08Bh, 0FFh
loc_1_1380:
        mov    edi, dword ptr [esp + 0xc]
loc_1_1384:
        or     edi, edi
        jne    loc_1_1399
loc_1_1388:
        mov    eax, dword ptr [esp + 8]
        inc    dword ptr [eax + 0x10]
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
loc_1_1399:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_VCache_CloseLookupCache:
        cmp    dword ptr [0x15ec], 1         ; -> obj1:0x15EC
        je     loc_1_13C0
        mov    eax, 1
        ret    
        db  090h
loc_1_13C0:
        mov    ecx, dword ptr [esp + 4]
        call   sub_1_12D0
        or     eax, eax
        je     loc_1_13E0
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        je     loc_1_13E0
        dec    ecx
        mov    dword ptr [eax + 0x10], ecx
        call   sub_1_0AA0
        sub    eax, eax
        ret    
loc_1_13E0:
        mov    eax, 0x57
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_VCache_DeleteLookupCache:
        cmp    dword ptr [0x15ec], 1         ; -> obj1:0x15EC
        je     loc_1_1400
        mov    eax, 1
        ret    
        db  090h
loc_1_1400:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_140A
        sub    eax, eax
        ret    
loc_1_140A:
        sub    eax, eax
        ret    
        db  08Dh, 049h, 000h
_VCache_Lookup:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x15ec], 1         ; -> obj1:0x15EC
        je     loc_1_1425
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1425:
        mov    ecx, dword ptr [esp + 0x10]
        call   sub_1_12D0
        or     eax, eax
        jne    loc_1_1440
        mov    eax, 0x57
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_1440:
        mov    ecx, dword ptr [esp + 0x18]
        mov    edx, dword ptr [esp + 0x14]
        push   ecx
        mov    ecx, eax
        call   sub_1_0FB0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_1496
        mov    ecx, ebx
        call   sub_1_0AC0
        mov    edx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ebx + 0x14]
        cmp    dword ptr [edx], eax
        ja     loc_1_1488
        mov    edi, dword ptr [esp + 0x20]
        mov    esi, dword ptr [ebx + 0x24]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [ebx + 0x14]
        mov    dword ptr [edx], ecx
        pop    edi
        pop    esi
        sub    eax, eax
        pop    ebx
        ret    
loc_1_1488:
        mov    ecx, dword ptr [ebx + 0x14]
        mov    dword ptr [edx], ecx
        pop    edi
        pop    esi
        mov    eax, 0xea
        pop    ebx
        ret    
loc_1_1496:
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
_VCache_UpdateLookup:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0x15ec], 1         ; -> obj1:0x15EC
        je     loc_1_14B7
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_14B7:
        mov    ecx, dword ptr [esp + 0x14]
        call   sub_1_12D0
        mov    edi, eax
        or     edi, edi
        jne    loc_1_14D0
        mov    eax, 0x57
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_14D0:
        mov    esi, dword ptr [esp + 0x1c]
        mov    edx, dword ptr [esp + 0x18]
        push   esi
        mov    ecx, edi
        call   sub_1_0FB0
        mov    ebp, eax
        or     ebp, ebp
        mov    ebx, dword ptr [esp + 0x20]
        je     loc_1_14F8
        cmp    dword ptr [ebp + 0x14], ebx
        je     loc_1_14F8
        mov    ecx, ebp
        call   sub_1_0DD0
        sub    ebp, ebp
loc_1_14F8:
        or     ebp, ebp
        jne    loc_1_1590
        mov    eax, dword ptr [esp + 0x18]
        lea    eax, [ebx + eax + 0x28]
        push   eax
        call   sub_1_07D4
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_15B0
        mov    eax, dword ptr [edi + 0x14]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_153E
        mov    ecx, dword ptr [edi + 4]
        test   byte ptr [ecx + 0x1e], 2
        je     loc_1_1539
        mov    eax, 0x20000
loc_1_1531:
        mov    ecx, dword ptr [ecx + 4]
        test   dword ptr [ecx + 0x1c], eax
        jne    loc_1_1531
loc_1_1539:
        call   sub_1_0DD0
loc_1_153E:
        mov    eax, dword ptr [edi + 0x14]
        inc    eax
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [ebp + 0x1c], 0
        mov    dword ptr [ebp + 8], edi
        mov    dword ptr [ebp + 4], ebp
        lea    edi, [ebp + 0x28]
        mov    dword ptr [ebp], ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ebp + 0x10], eax
        mov    dword ptr [ebp + 0x20], edi
        add    eax, edi
        mov    dword ptr [ebp + 0x24], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [ebp + 0x20]
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_1_0A80
        mov    dword ptr [ebp + 0xc], eax
        mov    ecx, ebp
        call   sub_1_0AC0
        jmp    loc_1_15B0
loc_1_1590:
        cmp    dword ptr [ebp + 0x14], ebx
        jne    loc_1_15B0
        mov    ecx, ebx
        shr    ecx, 2
        mov    esi, dword ptr [ebp + 0x24]
        mov    edi, dword ptr [esp + 0x24]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_15B0
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_15B0
        sub    ebp, ebp
loc_1_15B0:
        or     ebp, ebp
        je     loc_1_15D3
        mov    dword ptr [ebp + 0x14], ebx
        mov    edi, dword ptr [ebp + 0x24]
        mov    esi, dword ptr [esp + 0x24]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, ebp
        call   sub_1_0DC0
loc_1_15D3:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_15F0:
        inc    esi
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_1_15F6
loc_1_15F6:
        add    byte ptr [eax], al
loc_1_15F8:
        dec    esi
        jne    0x1668
        inc    ebp
        insb   byte ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsb  dx, byte ptr gs:[esi]
        je     0x1676
        add    byte ptr [ebp + 0x61], cl
        js     0x164d
        insb   byte ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsb  dx, byte ptr gs:[esi]
        je     0x1682
        add    byte ptr [ebx + 0x65], cl
        jns    loc_1_1614
loc_1_1614:
        and    eax, 0x34302573
        jne    loc_1_161B
loc_1_161B:
        add    byte ptr [ecx + 0x74], al
        popal  
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1624:
        and    eax, 0x34302573
        jne    0x162b
        db  000h
loc_1_162C:
        push   ebx
        jns    0x16a2
        je     loc_1_1696
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x16a8
        jb     0x169d
        outsb  dx, byte ptr [esi]
        je     0x167e
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x16b1
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x16a1
        push   ebx
        jb     0x16bf
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
        push   esi
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        pop    esp
        dec    esp
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   esi, dword ptr [ebp + 0x70], 0
loc_1_1660:
        dec    ebx
        jns    loc_1_1664
loc_1_1664:
        and    eax, 0x34302573
        jne    0x166b
        db  000h
loc_1_166C:
        inc    esp
        popal  
        je     0x16d1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1674:
        and    eax, 0x34302573
        jne    0x167b
        db  000h
loc_1_167C:
        dec    esi
        jne    0x16ec
        inc    ebp
        insb   byte ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsb  dx, byte ptr gs:[esi]
        je     0x16fa
        add    byte ptr [ebp + 0x61], cl
        js     0x16d1
        insb   byte ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsb  dx, byte ptr gs:[esi]
        je     0x1706
        add    byte ptr [esi + 0x6c], al
loc_1_1696:
        popal  
        jae    loc_1_169A
loc_1_169A:
        add    byte ptr [eax], al
loc_1_169C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2d8  R-X
loc_2_0000:
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        cmp    eax, 0x22
        jne    loc_2_002B
        mov    esi, 0x2ad                    ; -> loc_2_02AD
        lodsd  eax, dword ptr [esi]
loc_2_0011:
        xchg   edi, eax
        lodsd  eax, dword ptr [esi]
        push   esi
        mov    esi, 0x280                    ; -> loc_2_0280
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        pop    esi
        xchg   ebx, eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [eax], ebx
        lodsd  eax, dword ptr [esi]
        test   eax, eax
        jne    loc_2_0011
        clc    
        ret    
loc_2_002B:
        stc    
        ret    
loc_2_002D:
        push   0
        VMMCall  VMM._GetFreePageCount           ; cd205a000100
        add    esp, 4
        mov    ebx, edx
        mov    dword ptr [0x27c], ebx        ; -> obj2:0x027C
        mov    eax, dword ptr [0x8f4]        ; -> loc_1_08F4
        add    eax, 3
        shr    eax, 2
        mov    dword ptr [0x8f4], eax        ; -> loc_1_08F4
        mov    eax, dword ptr [0x8f0]        ; -> obj1:0x08F0
        add    eax, 3
        shr    eax, 2
        jne    loc_2_0077
        mov    eax, ebx
        xor    edx, edx
        mov    ecx, 0xa
        cmp    eax, 0x500
        jb     loc_2_0072
        mov    ecx, 6
loc_2_0072:
        div    ecx
        shr    eax, 2
loc_2_0077:
        sub    ebx, 0x180
        cmp    ebx, 8
        jg     loc_2_0087
        mov    ebx, 8
loc_2_0087:
        cmp    eax, ebx
        jbe    loc_2_008D
        mov    eax, ebx
loc_2_008D:
        cmp    eax, 8
        ja     loc_2_0097
        mov    eax, 8
loc_2_0097:
        mov    dword ptr [0x8f0], eax        ; -> obj1:0x08F0
        cmp    dword ptr [0x8f4], eax        ; -> loc_1_08F4
        jae    loc_2_00AA
        mov    dword ptr [0x8f4], ebx        ; -> loc_1_08F4
loc_2_00AA:
        cmp    dword ptr [0x8f4], ebx        ; -> loc_1_08F4
        jbe    loc_2_00B8
        mov    dword ptr [0x8f4], ebx        ; -> loc_1_08F4
loc_2_00B8:
        mov    eax, dword ptr [0x8f4]        ; -> loc_1_08F4
        cmp    eax, 0x32000
        jb     loc_2_00DB
        mov    eax, 0x32000
        mov    dword ptr [0x8f4], eax        ; -> loc_1_08F4
        cmp    eax, dword ptr [0x8f0]        ; -> obj1:0x08F0
        jae    loc_2_00DB
        mov    dword ptr [0x8f0], eax        ; -> obj1:0x08F0
loc_2_00DB:
        mov    ecx, 1
loc_2_00E0:
        add    ecx, ecx
        cmp    eax, ecx
        ja     loc_2_00E0
        shr    ecx, 1
        push   ecx
        mov    ecx, 0x40
        add    eax, ecx
        dec    eax
        xor    edx, edx
        div    ecx
        mov    dword ptr [0x9b8], eax        ; -> obj1:0x09B8
        shl    eax, 2
        pop    ecx
        mov    edi, eax
        lea    eax, [edi + ecx*8]
        push   ecx
        dec    ecx
        shl    ecx, 3
        mov    dword ptr [0x914], ecx        ; -> loc_1_0914
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        pop    ecx
        test   eax, eax
        je     loc_2_024A
        mov    dword ptr [0x9b0], eax        ; -> obj1:0x09B0
        add    edi, eax
        mov    dword ptr [0x918], edi        ; -> loc_1_0918
        cld    
loc_2_0131:
        mov    eax, edi
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        loop   loc_2_0131
        xor    eax, eax
        push   8
        push   dword ptr [0x8f4]             ; -> loc_1_08F4
        push   0x80080000
        VMMCall  VMM._PageReserve                ; cd201d010100
        add    esp, 0xc
        test   eax, eax
        je     loc_2_024A
        mov    dword ptr [0x920], eax        ; -> loc_1_0920
        mov    dword ptr [0x8fc], eax        ; -> loc_1_08FC
        push   eax
        mov    eax, 0x40
        cmp    eax, dword ptr [0x8f0]        ; -> obj1:0x08F0
        jae    loc_2_0174
        mov    eax, dword ptr [0x8f0]        ; -> obj1:0x08F0
loc_2_0174:
        mov    ecx, dword ptr [0x8f4]        ; -> loc_1_08F4
        sub    ecx, 0x400
        cmp    ecx, eax
        jg     loc_2_0186
        mov    ecx, eax
loc_2_0186:
        pop    eax
        cmp    ecx, dword ptr [0x8f4]        ; -> loc_1_08F4
        jbe    loc_2_0195
        mov    ecx, dword ptr [0x8f4]        ; -> loc_1_08F4
loc_2_0195:
        cmp    ecx, 0x900
        jbe    loc_2_01A2
        mov    ecx, 0x900
loc_2_01A2:
        mov    dword ptr [0x910], ecx        ; -> loc_1_0910
        shr    eax, 0xc
        push   8
        push   0
        push   3
        push   ecx
        push   eax
        VMMCall  VMM._PageCommit                 ; cd201e010100
        add    esp, 0x14
        test   eax, eax
        je     loc_2_024A
        mov    ecx, dword ptr [0x910]        ; -> loc_1_0910
        mov    eax, ecx
        shr    eax, 3
        cmp    eax, 0x10
        jb     loc_2_01D9
        mov    eax, 0x10
loc_2_01D9:
        mov    dword ptr [0x90c], eax        ; -> loc_1_090C
        cmp    eax, dword ptr [0x8f0]        ; -> obj1:0x08F0
        jbe    loc_2_01EB
        mov    dword ptr [0x8f0], eax        ; -> obj1:0x08F0
loc_2_01EB:
        mov    eax, dword ptr [0x8f8]        ; -> loc_1_08F8
        test   eax, eax
        jne    loc_2_01FE
        lea    eax, [ecx + ecx*2]
        shr    eax, 1
        mov    dword ptr [0x8f8], eax        ; -> loc_1_08F8
loc_2_01FE:
        inc    eax
        mov    dword ptr [0x908], eax        ; -> loc_1_0908
        mov    eax, ecx
        shr    eax, 1
        mov    edx, dword ptr [0x8fc]        ; -> loc_1_08FC
        mov    dword ptr [0x9ac], ecx        ; -> obj1:0x09AC
loc_2_0214:
        mov    edi, dword ptr [0x9a4]        ; -> obj1:0x09A4
        mov    dword ptr [edx + 0x100], edi
        mov    dword ptr [0x9a4], edx        ; -> obj1:0x09A4
        lea    edx, [edx + 0x1000]
        loop   loc_2_0214
        mov    esi, 0xa70                    ; -> loc_1_0A70
        mov    dword ptr [esi], esi
        mov    dword ptr [esi + 4], esi
        mov    esi, 0xa30                    ; -> loc_1_0A30
        mov    dword ptr [esi + 0x38], esi
        mov    dword ptr [esi + 0x3c], esi
        call   sub_1_1020
        clc    
        ret    
loc_2_024A:
        mov    eax, dword ptr [0x9b0]        ; -> obj1:0x09B0
        test   eax, eax
        je     loc_2_0271
        push   eax
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e004000
        add    esp, 4
        mov    dword ptr [0x9b0], 0          ; -> obj1:0x09B0
        mov    dword ptr [0x918], 0          ; -> loc_1_0918
loc_2_0271:
        stc    
        ret    
loc_2_0273:
        mov    byte ptr [0x9c5], 1           ; -> obj1:0x09C5
        clc    
        ret    
        db  000h, 000h, 000h, 000h
loc_2_0280:
        push   esi
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        db  000h
loc_2_0287:
        insd   dword ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x66], 0x63656c69
        popal  
        arpl   word ptr [eax + 0x65], bp
        db  000h
loc_2_0294:
        insd   dword ptr es:[edi], dx
        popal  
        js     0x2fe
        imul   ebp, dword ptr [ebp + 0x63], 0x65686361
        db  000h
loc_2_02A1:
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        inc    edx
        jne    0x30f
        push   edx
        push   edx
        push   esp
        db  000h
loc_2_02AD:
        xchg   dword ptr [edx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        lock or byte ptr [eax], al
        add    byte ptr [edx + eax], dl      ; -> loc_2_0294
        add    byte ptr [eax], al
        hlt    
        or     byte ptr [eax], al
        add    byte ptr [ecx + 2], ah        ; -> loc_2_02A1
        add    byte ptr [eax], al
        add    al, bh                        ; -> loc_1_08F8
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x7c  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 010h, 0E8h, 01Eh, 000h, 072h, 00Bh, 066h, 033h, 0D2h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h, 049h, 046h
        db  053h, 024h, 048h, 04Ch, 050h, 024h, 000h, 066h, 060h, 01Eh, 083h, 0ECh, 008h, 08Bh, 0FCh, 0B8h
        db  040h, 03Dh, 0BAh, 01Eh, 000h, 0CDh, 021h, 072h, 03Bh, 08Bh, 0D8h, 08Bh, 0D7h, 066h, 0C7h, 005h
        db  070h, 0E9h, 035h, 037h, 0B8h, 002h, 044h, 0B9h, 008h, 000h, 0CDh, 021h, 08Bh, 0D0h, 09Ch, 0B4h
        db  03Eh, 0CDh, 021h, 09Dh, 072h, 01Dh, 083h, 0FAh, 008h, 075h, 018h, 066h, 081h, 03Dh, 035h, 037h
        db  070h, 0EFh, 075h, 00Fh, 06Ah, 008h, 0FFh, 05Dh, 004h, 083h, 0C4h, 002h, 00Bh, 0C0h, 075h, 003h
        db  0F8h, 0EBh, 001h, 0F9h, 066h, 058h, 066h, 058h, 01Fh, 066h, 061h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x24c  R-X
VCache_Register:
        push   ecx
        mov    eax, dword ptr [0x9a0]        ; -> obj1:0x09A0
        cmp    esi, -1
        je     loc_4_00D3
        cmp    al, 0xa
        jae    loc_4_00D3
        shl    eax, 4
        add    eax, 0x9c8                    ; -> obj1:0x09C8
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 4], ecx
        add    ecx, dword ptr [0x924]        ; -> obj1:0x0924
        jb     loc_4_00D3
        cmp    dword ptr [0x910], ecx        ; -> loc_1_0910
        jb     loc_4_00D3
        mov    dword ptr [0x924], ecx        ; -> obj1:0x0924
        mov    dword ptr [eax], 0
        inc    dword ptr [0x9a0]             ; -> obj1:0x09A0
        mov    ecx, dword ptr [eax + 4]
        jecxz  loc_4_00A9
        push   ebx
        push   edx
        push   edi
        push   esi
loc_4_0058:
        push   eax
        push   ecx
        mov    ah, byte ptr [0x9a0]          ; -> obj1:0x09A0
        add    ah, 0x63
        mov    al, 0x13
        mov    ebx, ecx
        mov    edi, ecx
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        pop    ecx
        pop    eax
        or     esi, esi
        je     loc_4_00B3
        loop   loc_4_0058
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
loc_4_0080:
        mov    ah, byte ptr [0x9a0]          ; -> obj1:0x09A0
        add    ah, 0x63
        xor    al, al
        mov    ebx, ecx
        mov    edi, ecx
        push   ecx
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        pop    ecx
        loop   loc_4_0080
        pop    esi
        pop    edi
        pop    edx
        pop    ebx
loc_4_00A9:
        mov    al, byte ptr [0x9a0]          ; -> obj1:0x09A0
        add    al, 0x63
        clc    
loc_4_00B1:
        pop    ecx
        ret    
loc_4_00B3:
        mov    ebx, eax
        mov    edx, 0xd6                     ; -> loc_4_00D6
        mov    ah, byte ptr [0x9a0]          ; -> obj1:0x09A0
        add    ah, 0x63
        VxDCall  Dev_048B.Service_0009h          ; cd2009008b04
        pop    esi
        pop    edi
        pop    edx
        pop    ebx
        dec    dword ptr [0x9a0]             ; -> obj1:0x09A0
loc_4_00D3:
        stc    
        jmp    loc_4_00B1
loc_4_00D6:
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        ret    
VCache_Deregister:
        push   eax
        xor    ecx, ecx
        VxDCall  Dev_048B.Service_0010h          ; cd2010008b04
        pop    eax
        jb     loc_4_0111
        movzx  esi, ah
        sub    esi, 0x64
        shl    esi, 4
        add    esi, 0x9c8                    ; -> obj1:0x09C8
        or     ebx, 0xffffffff
        xchg   dword ptr [esi + 8], ebx
        mov    edx, 0x112                    ; -> loc_4_0112
        push   esi
        VxDCall  Dev_048B.Service_0009h          ; cd2009008b04
        pop    esi
loc_4_0111:
        ret    
loc_4_0112:
        mov    word ptr [esi + 0x30], 0
        mov    byte ptr [esi + 0x32], 0
        pushal 
        call   ebx
        popal  
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        ret    
VCache_AdjustMinimum:
        cmp    ecx, 0x40
        ja     loc_4_0249
        movzx  esi, ah
        sub    esi, 0x64
        jb     loc_4_0249
        cmp    esi, dword ptr [0x9a0]        ; -> obj1:0x09A0
        jae    loc_4_0248
        shl    esi, 4
        add    esi, 0x9c8                    ; -> obj1:0x09C8
        cmp    dword ptr [esi + 4], ecx
        jb     loc_4_0180
        xchg   dword ptr [esi + 4], ecx
        sub    ecx, dword ptr [esi + 4]
        mov    ebx, dword ptr [esi]
        add    dword ptr [esi], ecx
        sub    dword ptr [0x924], ecx        ; -> obj1:0x0924
        cmp    ebx, 0
        jge    loc_4_017F
        neg    ebx
        cmp    ebx, ecx
        ja     loc_4_0173
        mov    ecx, ebx
loc_4_0173:
        add    dword ptr [0x9a8], ecx        ; -> obj1:0x09A8
        add    dword ptr [0x9ac], ecx        ; -> obj1:0x09AC
loc_4_017F:
        ret    
loc_4_0180:
        sub    ecx, dword ptr [esi + 4]
        sub    eax, eax
        mov    edx, dword ptr [esi]
        cmp    edx, 0
        jle    loc_4_01A5
        mov    eax, edx
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        add    dword ptr [esi + 4], eax
        sub    dword ptr [esi], eax
        add    dword ptr [0x924], eax        ; -> obj1:0x0924
        sub    ecx, eax
        je     loc_4_0219
loc_4_01A5:
        push   eax
        mov    eax, esi
        sub    eax, 0x9c8                    ; -> obj1:0x09C8
        shr    eax, 4
        mov    ah, al
        add    ah, 0x64
        pushal 
        mov    edx, 0x6e0                    ; -> VCache_Hold
        VxDCall  Dev_048B.Service_0009h          ; cd2009008b04
        popal  
        push   eax
        mov    al, 0x13
        mov    ebx, esi
        mov    edi, esi
        sub    ecx, dword ptr [esi]
        push   0
        jmp    loc_4_01D5
loc_4_01CF:
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_4_01D5:
        inc    edi
        push   eax
        push   ecx
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        pop    ecx
        pop    eax
        jae    loc_4_01CF
        or     esi, esi
        je     loc_4_021B
        push   esi
        loop   loc_4_01D5
        xor    esi, esi
        xchg   dword ptr [ebx], esi
        add    dword ptr [ebx + 4], esi
        add    dword ptr [0x924], esi        ; -> obj1:0x0924
loc_4_01F6:
        pop    esi
        cmp    esi, 0
        je     loc_4_020A
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        jmp    loc_4_01F6
loc_4_020A:
        pop    eax
        pushal 
        mov    edx, 0x6e8                    ; -> VCache_Unhold
        VxDCall  Dev_048B.Service_0009h          ; cd2009008b04
        popal  
        pop    edx
loc_4_0219:
        clc    
        ret    
loc_4_021B:
        pop    esi
        cmp    esi, 0
        je     loc_4_022F
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        jmp    loc_4_021B
loc_4_022F:
        pop    eax
        pushal 
        mov    edx, 0x6e8                    ; -> VCache_Unhold
        VxDCall  Dev_048B.Service_0009h          ; cd2009008b04
        popal  
        pop    ecx
        neg    ecx
        add    ecx, dword ptr [esi + 4]
        call   VCache_AdjustMinimum
loc_4_0248:
        stc    
loc_4_0249:
        ret    
        db  023h, 056h
VxD__LTEXT ENDS

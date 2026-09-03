; VNetSup -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 3095 instructions recovered by recursive descent

VxD__LTEXT SEGMENT   ; object 1  vsize=0x121c  R-X
loc_1_0000:
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [esi + 0x30], al     ; -> loc_3_0046
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx], ch            ; -> loc_1_006B
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx], ch            ; -> loc_1_006B
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx], ch            ; -> loc_1_006B
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx], ch            ; -> loc_1_006B
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx], ch            ; -> loc_1_006B
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx], ch            ; -> loc_1_006B
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx], ch            ; -> loc_1_006B
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx], ch            ; -> loc_1_006B
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx], ch            ; -> loc_1_006B
        add    byte ptr [eax], al
        imul   eax, dword ptr [eax], 0
        add    ch, al                        ; -> loc_1_01C5
        add    dword ptr [eax], eax
        add    byte ptr [0x17], bh
        jae    loc_1_006C
        jmp    loc_1_0000
loc_1_006B:
        clc    
loc_1_006C:
        ret    
sub_1_006D:
        or     byte ptr [edx], al
        add    byte ptr [eax], al
        scasd  eax, dword ptr es:[edi]
        add    byte ptr [eax], al
        add    ch, cl                        ; -> loc_1_00CD
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        mov    bh, 0x4d
        sbb    al, 0x81
        stc    
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        jae    loc_1_00A3
        sub    eax, eax
        push   eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        push   eax
        call   sub_1_006D
        add    esp, 8
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_1_00A3:
        mov    word ptr [ebp + 0x1c], 1
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_1_00AF:
        mov    ebx, dword ptr [0x1192]       ; -> obj1:0x1192
        inc    dword ptr [0x1192]            ; -> obj1:0x1192
        push   dword ptr [ebx*4 + 0x119a]    ; -> loc_1_119A
        push   0
        push   dword ptr [ebx*4 + 0x1196]    ; -> loc_1_1196
        jmp    loc_1_00E9
loc_1_00CD:
        mov    ebx, dword ptr [0x1202]       ; -> obj1:0x1202
        inc    dword ptr [0x1202]            ; -> obj1:0x1202
        push   dword ptr [ebx*4 + 0x120a]    ; -> loc_1_120A
        push   1
        push   dword ptr [ebx*4 + 0x1206]    ; -> loc_1_1206
loc_1_00E9:
        mov    edi, eax
        mov    esi, eax
        mov    ecx, 0xffffffff
        sub    al, al
        repne scasb al, byte ptr es:[edi]
        not    ecx
        cmp    ecx, 1
        jbe    loc_1_0125
        mov    edi, ecx
        push   ecx
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
        pop    ecx
        test   eax, eax
        je     loc_1_0125
        push   eax
        push   edi
        mov    ecx, edi
        mov    edi, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        push   edi
        push   eax
        call   sub_1_02F8
        add    esp, 8
        VxDCall  IFSMgr.IFSMgr_SetErrString      ; cd2026004000
        pop    ecx
loc_1_0125:
        add    esp, 8
        pop    eax
        ret    
loc_1_012A:
        call   sub_1_1038
        cmp    eax, 1
        jae    loc_1_0135
        ret    
loc_1_0135:
        ja     loc_1_013D
        jmp    dword ptr [0x1119]            ; -> obj1:0x1119
loc_1_013D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   dword ptr [0x1119]            ; -> obj1:0x1119
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 0x1a], 1
        jne    loc_1_0162
        mov    word ptr [eax + 0x1a], 0
loc_1_0162:
        pop    ebp
        ret    
sub_1_0164:
        push   esi
        push   edi
        push   ebx
        movzx  ecx, byte ptr [esp + 0x10]
        cmp    ecx, 3
        jae    loc_1_0193
        mov    ebx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x14]
        push   ebx
        push   eax
        call   sub_1_006D
        add    esp, 8
        mov    ebx, dword ptr [esp + 0x18]
        mov    dword ptr [ebx], eax
        xor    eax, eax
        jmp    loc_1_0198
loc_1_0193:
        mov    eax, 1
loc_1_0198:
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_1_019C:
        mov    eax, dword ptr [0x1110]       ; -> obj1:0x1110
        clc    
        ret    
loc_1_01A3:
        mov    eax, 0x1134                   ; -> obj1:0x1134
        cmp    byte ptr [0x1118], 1          ; -> loc_1_1118
        ret    
loc_1_01B0:
        jmp    sub_1_026C
loc_1_01B5:
        ret    
        db  090h, 090h
loc_1_01B8:
        jmp    loc_1_0344
        db  090h, 090h, 090h
loc_1_01C0:
        jmp    loc_1_0B98
loc_1_01C5:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_01CF
        clc    
        ret    
loc_1_01CF:
        mov    ecx, dword ptr [0x111d]       ; -> sub_1_111D
        jecxz  loc_1_01E2
loc_1_01D7:
        push   ecx
        call   sub_1_111D
        pop    ecx
        loop   loc_1_01D7
loc_1_01E2:
        clc    
        ret    
sub_1_01E4:
        VxDCall  IFSMgr.BCSToUni                 ; cd2042804000
        VxDCall  IFSMgr.UniToBCSPath             ; cd2041804000
sub_1_01F0:
        VxDCall  IFSMgr.UniToBCS                 ; cd2040804000
        VxDCall  IFSMgr.UniToUpper               ; cd2043804000
sub_1_01FC:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_0202:
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
loc_1_0208:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [0x2000]       ; -> obj2:0x0000
        or     esi, esi
        je     loc_1_0228
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0xc]
loc_1_021D:
        cmp    dword ptr [esi + 0x10], eax
        je     loc_1_0234
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_021D
loc_1_0228:
        pop    edi
        mov    eax, 2
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_0234:
        movzx  edx, word ptr [esi + 0x14]
        add    edx, 8
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        je     loc_1_0254
        cmp    dword ptr [eax], edx
        jge    loc_1_0254
        mov    dword ptr [eax], edx
        mov    eax, 0xea
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_0254:
        mov    ecx, edx
        add    esi, 4
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    eax, eax
        pop    edi
        pop    esi
        leave  
        ret    
        db  090h
sub_1_026C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [0x2000]       ; -> obj2:0x0000
        mov    edx, eax
        mov    edi, eax
        or     eax, eax
        mov    esi, dword ptr [ebp + 0x10]
        je     loc_1_02AF
        mov    ecx, dword ptr [esi + 4]
loc_1_0284:
        cmp    dword ptr [edx + 0x10], ecx
        je     loc_1_0294
        mov    edi, edx
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_1_0284
        jmp    loc_1_02AF
        db  090h
loc_1_0294:
        cmp    eax, edx
        mov    eax, dword ptr [edx]
        jne    loc_1_02A4
        mov    dword ptr [0x2000], eax       ; -> obj2:0x0000
        jmp    loc_1_02A6
        db  08Dh, 049h, 000h
loc_1_02A4:
        mov    dword ptr [edi], eax
loc_1_02A6:
        push   edx
        call   sub_1_0202
        add    esp, 4
loc_1_02AF:
        movzx  eax, word ptr [esi + 8]
        add    eax, 0xc
        push   eax
        call   sub_1_01FC
        add    esp, 4
        or     eax, eax
        je     loc_1_02F1
        mov    ecx, dword ptr [0x2000]       ; -> obj2:0x0000
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x2000], eax       ; -> obj2:0x0000
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 8], ecx
        movzx  edx, word ptr [esi + 8]
        lea    edi, [eax + 0xc]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_02F1:
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
sub_1_02F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        lea    eax, [edi*2]
        push   eax
        call   sub_1_01FC
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_033E
        push   0
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   ebx
        call   sub_1_01E4
        add    esp, 0x10
        push   1
        push   edi
        push   eax
        push   ebx
        push   esi
        call   sub_1_01F0
        add    esp, 0x14
        push   ebx
        call   sub_1_0202
        add    esp, 4
loc_1_033E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  090h
loc_1_0344:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   esi
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        push   0x15
        lea    eax, [ebp - 0x18]
        push   eax
        push   0x2004                        ; -> obj2:0x0004
        push   dword ptr [ebp + 0xc]
        call   sub_1_0E58
        add    esp, 0x14
        lea    eax, [esi + 0x18]
        push   eax
        push   0x18
        push   esi
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_1_0E58
        add    esp, 0x14
        pop    esi
        leave  
        ret    
        db  08Bh, 0FFh
sub_1_0380:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 8], 0x67452301
        mov    dword ptr [eax + 0xc], 0xefcdab89
        mov    dword ptr [eax + 0x10], 0x98badcfe
        mov    dword ptr [eax + 0x14], 0x10325476
        leave  
        ret    
        db  08Dh, 049h, 000h
sub_1_03B4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi]
        mov    ecx, edx
        and    ecx, 0x1f8
        shr    ecx, 3
        mov    eax, dword ptr [ebp + 0x10]
        lea    eax, [edx + eax*8]
        cmp    eax, edx
        jae    loc_1_03DA
        inc    dword ptr [esi + 4]
loc_1_03DA:
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 0x10]
        shr    eax, 0x1d
        add    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        dec    dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_0452
        mov    edi, dword ptr [ebp + 0xc]
loc_1_03F2:
        mov    al, byte ptr [edi]
        mov    byte ptr [ecx + esi + 0x18], al
        inc    edi
        inc    ecx
        cmp    ecx, 0x40
        jne    loc_1_0448
        sub    edx, edx
        lea    ecx, [ebp - 0x40]
loc_1_0404:
        movzx  eax, byte ptr [edx + esi + 0x1b]
        shl    eax, 0x10
        movzx  ebx, byte ptr [edx + esi + 0x19]
        or     eax, ebx
        shl    eax, 8
        movzx  ebx, byte ptr [edx + esi + 0x1a]
        shl    ebx, 0x10
        or     eax, ebx
        movzx  ebx, byte ptr [edx + esi + 0x18]
        or     eax, ebx
        mov    dword ptr [ecx], eax
        add    ecx, 4
        add    edx, 4
        lea    eax, [ebp]
        cmp    ecx, eax
        jb     loc_1_0404
        lea    eax, [ebp - 0x40]
        push   eax
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_0518
        add    esp, 8
        sub    ecx, ecx
loc_1_0448:
        mov    eax, dword ptr [ebp + 0x10]
        dec    dword ptr [ebp + 0x10]
        or     eax, eax
        jne    loc_1_03F2
loc_1_0452:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  090h
sub_1_0458:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edi]
        and    eax, 0x1f8
        shr    eax, 3
        cmp    eax, 0x38
        jl     loc_1_0484
        mov    ecx, 0x78
        jmp    loc_1_0489
loc_1_0484:
        mov    ecx, 0x38
loc_1_0489:
        sub    ecx, eax
        push   ecx
        push   0x200c                        ; -> obj2:0x000C
        push   edi
        call   sub_1_03B4
        add    esp, 0xc
        sub    edx, edx
        inc    ecx
        lea    ecx, [ebp - 0x40]
loc_1_04A0:
        movzx  esi, byte ptr [edx + edi + 0x1b]
        shl    esi, 0x10
        movzx  eax, byte ptr [edx + edi + 0x19]
        or     esi, eax
        shl    esi, 8
        movzx  eax, byte ptr [edx + edi + 0x1a]
        shl    eax, 0x10
        or     esi, eax
        movzx  eax, byte ptr [edx + edi + 0x18]
        or     esi, eax
        mov    dword ptr [ecx], esi
        add    ecx, 4
        add    edx, 4
        lea    eax, [ebp - 8]
        cmp    ecx, eax
        jb     loc_1_04A0
        lea    esi, [edi + 8]
        lea    eax, [ebp - 0x40]
        push   eax
        push   esi
        call   sub_1_0518
        add    esp, 8
        sub    eax, eax
        mov    edx, 4
loc_1_04E9:
        mov    cl, byte ptr [esi]
        mov    byte ptr [eax + edi + 0x58], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [eax + edi + 0x59], cl
        mov    ecx, dword ptr [esi]
        shr    ecx, 0x10
        mov    byte ptr [eax + edi + 0x5a], cl
        mov    ecx, dword ptr [esi]
        shr    ecx, 0x18
        mov    byte ptr [eax + edi + 0x5b], cl
        add    esi, 4
        add    eax, 4
        dec    edx
        jne    loc_1_04E9
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
sub_1_0518:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x9c
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 0x10], eax
        mov    esi, eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    ebx, eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], eax
        mov    edi, eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    ecx, eax
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, ebx
        not    eax
        and    eax, ecx
        mov    dword ptr [ebp - 0x9c], edi
        and    dword ptr [ebp - 0x9c], ebx
        or     eax, dword ptr [ebp - 0x9c]
        add    eax, dword ptr [edx]
        add    esi, eax
        lea    eax, [esi*8]
        shr    esi, 0x1d
        or     eax, esi
        mov    esi, eax
        not    esi
        and    esi, edi
        mov    dword ptr [ebp - 0x98], ebx
        and    dword ptr [ebp - 0x98], eax
        or     esi, dword ptr [ebp - 0x98]
        add    esi, dword ptr [edx + 4]
        add    ecx, esi
        mov    esi, ecx
        shl    esi, 7
        shr    ecx, 0x19
        or     esi, ecx
        mov    ecx, esi
        not    ecx
        and    ecx, ebx
        mov    dword ptr [ebp - 0x94], esi
        and    dword ptr [ebp - 0x94], eax
        or     ecx, dword ptr [ebp - 0x94]
        add    ecx, dword ptr [edx + 8]
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 0x15
        shl    edi, 0xb
        or     ecx, edi
        mov    edi, ecx
        not    edi
        and    edi, eax
        mov    dword ptr [ebp - 0x90], esi
        and    dword ptr [ebp - 0x90], ecx
        or     edi, dword ptr [ebp - 0x90]
        add    edi, dword ptr [edx + 0xc]
        add    ebx, edi
        mov    edi, ebx
        shl    edi, 0x13
        shr    ebx, 0xd
        or     edi, ebx
        mov    ebx, edi
        not    ebx
        and    ebx, esi
        mov    dword ptr [ebp - 0x8c], ecx
        and    dword ptr [ebp - 0x8c], edi
        or     ebx, dword ptr [ebp - 0x8c]
        add    ebx, dword ptr [edx + 0x10]
        add    eax, ebx
        lea    ebx, [eax*8]
        shr    eax, 0x1d
        or     ebx, eax
        mov    eax, ebx
        not    eax
        and    eax, ecx
        mov    dword ptr [ebp - 0x88], edi
        and    dword ptr [ebp - 0x88], ebx
        or     eax, dword ptr [ebp - 0x88]
        add    eax, dword ptr [edx + 0x14]
        add    esi, eax
        mov    eax, esi
        shl    eax, 7
        shr    esi, 0x19
        or     eax, esi
        mov    esi, eax
        not    esi
        and    esi, edi
        mov    dword ptr [ebp - 0x84], eax
        and    dword ptr [ebp - 0x84], ebx
        or     esi, dword ptr [ebp - 0x84]
        add    esi, dword ptr [edx + 0x18]
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 0x15
        shl    ecx, 0xb
        or     esi, ecx
        mov    ecx, esi
        not    ecx
        and    ecx, ebx
        mov    dword ptr [ebp - 0x80], eax
        and    dword ptr [ebp - 0x80], esi
        or     ecx, dword ptr [ebp - 0x80]
        add    ecx, dword ptr [edx + 0x1c]
        add    edi, ecx
        mov    ecx, edi
        shl    ecx, 0x13
        shr    edi, 0xd
        or     ecx, edi
        mov    edi, ecx
        not    edi
        and    edi, eax
        mov    dword ptr [ebp - 0x7c], esi
        and    dword ptr [ebp - 0x7c], ecx
        or     edi, dword ptr [ebp - 0x7c]
        add    edi, dword ptr [edx + 0x20]
        add    ebx, edi
        lea    edi, [ebx*8]
        shr    ebx, 0x1d
        or     edi, ebx
        mov    ebx, edi
        not    ebx
        and    ebx, esi
        mov    dword ptr [ebp - 0x78], ecx
        and    dword ptr [ebp - 0x78], edi
        or     ebx, dword ptr [ebp - 0x78]
        add    ebx, dword ptr [edx + 0x24]
        add    eax, ebx
        mov    ebx, eax
        shl    ebx, 7
        shr    eax, 0x19
        or     ebx, eax
        mov    eax, ebx
        not    eax
        and    eax, ecx
        mov    dword ptr [ebp - 0x74], ebx
        and    dword ptr [ebp - 0x74], edi
        or     eax, dword ptr [ebp - 0x74]
        add    eax, dword ptr [edx + 0x28]
        add    esi, eax
        mov    eax, esi
        shr    eax, 0x15
        shl    esi, 0xb
        or     eax, esi
        mov    esi, eax
        not    esi
        and    esi, edi
        mov    dword ptr [ebp - 0x70], ebx
        and    dword ptr [ebp - 0x70], eax
        or     esi, dword ptr [ebp - 0x70]
        add    esi, dword ptr [edx + 0x2c]
        add    ecx, esi
        mov    esi, ecx
        shl    esi, 0x13
        shr    ecx, 0xd
        or     esi, ecx
        mov    ecx, esi
        not    ecx
        and    ecx, ebx
        mov    dword ptr [ebp - 0x6c], eax
        and    dword ptr [ebp - 0x6c], esi
        or     ecx, dword ptr [ebp - 0x6c]
        add    ecx, dword ptr [edx + 0x30]
        add    edi, ecx
        lea    ecx, [edi*8]
        shr    edi, 0x1d
        or     ecx, edi
        mov    edi, ecx
        not    edi
        and    edi, eax
        mov    dword ptr [ebp - 0x68], esi
        and    dword ptr [ebp - 0x68], ecx
        or     edi, dword ptr [ebp - 0x68]
        add    edi, dword ptr [edx + 0x34]
        add    ebx, edi
        mov    edi, ebx
        shl    edi, 7
        shr    ebx, 0x19
        or     edi, ebx
        mov    ebx, edi
        not    ebx
        and    ebx, esi
        mov    dword ptr [ebp - 0x64], edi
        and    dword ptr [ebp - 0x64], ecx
        or     ebx, dword ptr [ebp - 0x64]
        add    ebx, dword ptr [edx + 0x38]
        add    eax, ebx
        mov    ebx, eax
        shr    ebx, 0x15
        shl    eax, 0xb
        or     ebx, eax
        mov    eax, ebx
        not    eax
        and    eax, ecx
        mov    dword ptr [ebp - 0x60], edi
        and    dword ptr [ebp - 0x60], ebx
        or     eax, dword ptr [ebp - 0x60]
        add    eax, dword ptr [edx + 0x3c]
        add    esi, eax
        mov    eax, esi
        shl    eax, 0x13
        shr    esi, 0xd
        or     eax, esi
        mov    esi, ebx
        or     esi, eax
        and    esi, edi
        mov    dword ptr [ebp - 0x5c], ebx
        and    dword ptr [ebp - 0x5c], eax
        or     esi, dword ptr [ebp - 0x5c]
        add    esi, dword ptr [edx]
        add    esi, 0x5a827999
        add    ecx, esi
        lea    esi, [ecx*8]
        shr    ecx, 0x1d
        or     esi, ecx
        mov    ecx, eax
        or     ecx, esi
        mov    dword ptr [ebp - 0x58], ecx
        mov    ecx, eax
        and    ecx, esi
        mov    dword ptr [ebp - 0x54], ecx
        mov    ecx, ebx
        and    ecx, dword ptr [ebp - 0x58]
        or     ecx, dword ptr [ebp - 0x54]
        add    ecx, dword ptr [edx + 0x10]
        add    ecx, 0x5a827999
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 0x1b
        shl    edi, 5
        or     ecx, edi
        mov    edi, ecx
        and    edi, dword ptr [ebp - 0x58]
        or     edi, dword ptr [ebp - 0x54]
        add    edi, dword ptr [edx + 0x20]
        add    edi, 0x5a827999
        add    ebx, edi
        mov    edi, ebx
        shr    edi, 0x17
        shl    ebx, 9
        or     edi, ebx
        mov    ebx, edi
        or     ebx, esi
        and    ebx, ecx
        mov    dword ptr [ebp - 0x50], edi
        and    dword ptr [ebp - 0x50], esi
        or     ebx, dword ptr [ebp - 0x50]
        add    ebx, dword ptr [edx + 0x30]
        add    ebx, 0x5a827999
        add    eax, ebx
        mov    ebx, eax
        shr    ebx, 0x13
        shl    eax, 0xd
        or     ebx, eax
        mov    eax, edi
        or     eax, ebx
        and    eax, ecx
        mov    dword ptr [ebp - 0x4c], edi
        and    dword ptr [ebp - 0x4c], ebx
        or     eax, dword ptr [ebp - 0x4c]
        add    eax, dword ptr [edx + 4]
        add    eax, 0x5a827999
        add    esi, eax
        lea    eax, [esi*8]
        shr    esi, 0x1d
        or     eax, esi
        mov    esi, ebx
        or     esi, eax
        mov    dword ptr [ebp - 0x48], esi
        mov    esi, ebx
        and    esi, eax
        mov    dword ptr [ebp - 0x44], esi
        mov    esi, edi
        and    esi, dword ptr [ebp - 0x48]
        or     esi, dword ptr [ebp - 0x44]
        add    esi, dword ptr [edx + 0x14]
        add    esi, 0x5a827999
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 0x1b
        shl    ecx, 5
        or     esi, ecx
        mov    ecx, esi
        and    ecx, dword ptr [ebp - 0x48]
        or     ecx, dword ptr [ebp - 0x44]
        add    ecx, dword ptr [edx + 0x24]
        add    ecx, 0x5a827999
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 0x17
        shl    edi, 9
        or     ecx, edi
        mov    edi, ecx
        or     edi, eax
        and    edi, esi
        mov    dword ptr [ebp - 0x40], ecx
        and    dword ptr [ebp - 0x40], eax
        or     edi, dword ptr [ebp - 0x40]
        add    edi, dword ptr [edx + 0x34]
        add    edi, 0x5a827999
        add    ebx, edi
        mov    edi, ebx
        shr    edi, 0x13
        shl    ebx, 0xd
        or     edi, ebx
        mov    ebx, ecx
        or     ebx, edi
        and    ebx, esi
        mov    dword ptr [ebp - 0x3c], ecx
        and    dword ptr [ebp - 0x3c], edi
        or     ebx, dword ptr [ebp - 0x3c]
        add    ebx, dword ptr [edx + 8]
        add    ebx, 0x5a827999
        add    eax, ebx
        lea    ebx, [eax*8]
        shr    eax, 0x1d
        or     ebx, eax
        mov    eax, edi
        or     eax, ebx
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, edi
        and    eax, ebx
        mov    dword ptr [ebp - 0x34], eax
        mov    eax, ecx
        and    eax, dword ptr [ebp - 0x38]
        or     eax, dword ptr [ebp - 0x34]
        add    eax, dword ptr [edx + 0x18]
        add    eax, 0x5a827999
        add    esi, eax
        mov    eax, esi
        shr    eax, 0x1b
        shl    esi, 5
        or     eax, esi
        mov    esi, eax
        and    esi, dword ptr [ebp - 0x38]
        or     esi, dword ptr [ebp - 0x34]
        add    esi, dword ptr [edx + 0x28]
        add    esi, 0x5a827999
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 0x17
        shl    ecx, 9
        or     esi, ecx
        mov    ecx, esi
        or     ecx, ebx
        and    ecx, eax
        mov    dword ptr [ebp - 0x30], esi
        and    dword ptr [ebp - 0x30], ebx
        or     ecx, dword ptr [ebp - 0x30]
        add    ecx, dword ptr [edx + 0x38]
        add    ecx, 0x5a827999
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 0x13
        shl    edi, 0xd
        or     ecx, edi
        mov    edi, esi
        or     edi, ecx
        and    edi, eax
        mov    dword ptr [ebp - 0x2c], esi
        and    dword ptr [ebp - 0x2c], ecx
        or     edi, dword ptr [ebp - 0x2c]
        add    edi, dword ptr [edx + 0xc]
        add    edi, 0x5a827999
        add    ebx, edi
        lea    edi, [ebx*8]
        shr    ebx, 0x1d
        or     edi, ebx
        mov    ebx, ecx
        or     ebx, edi
        mov    dword ptr [ebp - 0x28], ebx
        mov    ebx, ecx
        and    ebx, edi
        mov    dword ptr [ebp - 0x24], ebx
        mov    ebx, esi
        and    ebx, dword ptr [ebp - 0x28]
        or     ebx, dword ptr [ebp - 0x24]
        add    ebx, dword ptr [edx + 0x1c]
        add    ebx, 0x5a827999
        add    eax, ebx
        mov    ebx, eax
        shr    ebx, 0x1b
        shl    eax, 5
        or     ebx, eax
        mov    eax, ebx
        and    eax, dword ptr [ebp - 0x28]
        or     eax, dword ptr [ebp - 0x24]
        add    eax, dword ptr [edx + 0x2c]
        add    eax, 0x5a827999
        add    esi, eax
        mov    eax, esi
        shr    eax, 0x17
        shl    esi, 9
        or     eax, esi
        mov    esi, eax
        or     esi, edi
        and    esi, ebx
        mov    dword ptr [ebp - 0x20], eax
        and    dword ptr [ebp - 0x20], edi
        or     esi, dword ptr [ebp - 0x20]
        add    esi, dword ptr [edx + 0x3c]
        add    esi, 0x5a827999
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 0x13
        shl    ecx, 0xd
        or     esi, ecx
        mov    ecx, ebx
        xor    ecx, eax
        xor    ecx, esi
        add    ecx, dword ptr [edx]
        add    ecx, 0x6ed9eba1
        add    edi, ecx
        lea    ecx, [edi*8]
        shr    edi, 0x1d
        or     ecx, edi
        mov    edi, eax
        xor    edi, esi
        xor    edi, ecx
        add    edi, dword ptr [edx + 0x20]
        add    edi, 0x6ed9eba1
        add    ebx, edi
        mov    edi, ebx
        shr    edi, 0x17
        shl    ebx, 9
        or     edi, ebx
        mov    ebx, edi
        xor    ebx, esi
        xor    ebx, ecx
        add    ebx, dword ptr [edx + 0x10]
        add    ebx, 0x6ed9eba1
        add    eax, ebx
        mov    ebx, eax
        shr    ebx, 0x15
        shl    eax, 0xb
        or     ebx, eax
        mov    eax, edi
        xor    eax, ebx
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, ecx
        xor    eax, dword ptr [ebp - 0x1c]
        add    eax, dword ptr [edx + 0x30]
        add    eax, 0x6ed9eba1
        add    esi, eax
        mov    eax, esi
        shl    eax, 0xf
        shr    esi, 0x11
        or     eax, esi
        mov    esi, eax
        xor    esi, dword ptr [ebp - 0x1c]
        add    esi, dword ptr [edx + 8]
        add    esi, 0x6ed9eba1
        add    ecx, esi
        lea    esi, [ecx*8]
        shr    ecx, 0x1d
        or     esi, ecx
        mov    ecx, ebx
        xor    ecx, eax
        xor    ecx, esi
        add    ecx, dword ptr [edx + 0x28]
        add    ecx, 0x6ed9eba1
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 0x17
        shl    edi, 9
        or     ecx, edi
        mov    edi, ecx
        xor    edi, eax
        xor    edi, esi
        add    edi, dword ptr [edx + 0x18]
        add    edi, 0x6ed9eba1
        add    ebx, edi
        mov    edi, ebx
        shr    edi, 0x15
        shl    ebx, 0xb
        or     edi, ebx
        mov    ebx, ecx
        xor    ebx, edi
        mov    dword ptr [ebp - 0x18], ebx
        mov    ebx, esi
        xor    ebx, dword ptr [ebp - 0x18]
        add    ebx, dword ptr [edx + 0x38]
        add    ebx, 0x6ed9eba1
        add    eax, ebx
        mov    ebx, eax
        shl    ebx, 0xf
        shr    eax, 0x11
        or     ebx, eax
        mov    eax, ebx
        xor    eax, dword ptr [ebp - 0x18]
        add    eax, dword ptr [edx + 4]
        add    eax, 0x6ed9eba1
        add    esi, eax
        lea    eax, [esi*8]
        shr    esi, 0x1d
        or     eax, esi
        mov    esi, edi
        xor    esi, ebx
        xor    esi, eax
        add    esi, dword ptr [edx + 0x24]
        add    esi, 0x6ed9eba1
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 0x17
        shl    ecx, 9
        or     esi, ecx
        mov    ecx, esi
        xor    ecx, ebx
        xor    ecx, eax
        add    ecx, dword ptr [edx + 0x14]
        add    ecx, 0x6ed9eba1
        add    edi, ecx
        mov    ecx, edi
        shr    ecx, 0x15
        shl    edi, 0xb
        or     ecx, edi
        mov    edi, esi
        xor    edi, ecx
        mov    dword ptr [ebp - 0x14], edi
        mov    edi, eax
        xor    edi, dword ptr [ebp - 0x14]
        add    edi, dword ptr [edx + 0x34]
        add    edi, 0x6ed9eba1
        add    ebx, edi
        mov    edi, ebx
        shl    edi, 0xf
        shr    ebx, 0x11
        or     edi, ebx
        mov    ebx, edi
        xor    ebx, dword ptr [ebp - 0x14]
        add    ebx, dword ptr [edx + 0xc]
        add    ebx, 0x6ed9eba1
        add    eax, ebx
        lea    ebx, [eax*8]
        shr    eax, 0x1d
        or     ebx, eax
        mov    eax, ecx
        xor    eax, edi
        xor    eax, ebx
        add    eax, dword ptr [edx + 0x2c]
        add    eax, 0x6ed9eba1
        add    esi, eax
        mov    eax, esi
        shr    eax, 0x17
        shl    esi, 9
        or     eax, esi
        mov    esi, eax
        xor    esi, edi
        xor    esi, ebx
        add    esi, dword ptr [edx + 0x1c]
        add    esi, 0x6ed9eba1
        add    ecx, esi
        mov    esi, ecx
        shr    esi, 0x15
        shl    ecx, 0xb
        or     esi, ecx
        mov    ecx, eax
        xor    ecx, esi
        xor    ecx, ebx
        add    ecx, dword ptr [edx + 0x3c]
        add    ecx, 0x6ed9eba1
        add    edi, ecx
        mov    edx, edi
        shl    edx, 0xf
        shr    edi, 0x11
        or     edx, edi
        add    ebx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], ebx
        add    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 4], edx
        add    esi, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], esi
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xc], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_0B98:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x80
        push   esi
        push   edi
        lea    edi, [ebp - 0x18]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        lea    eax, [ebp - 0x80]
        push   eax
        call   sub_1_0380
        add    esp, 4
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x80]
        push   eax
        call   sub_1_03B4
        add    esp, 0xc
        lea    eax, [ebp - 0x80]
        push   eax
        call   sub_1_0458
        add    esp, 4
        lea    esi, [ebp - 0x28]
        lea    edi, [ebp - 0x18]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebp + 0x14]
        lea    ecx, [eax + 0x18]
        push   ecx
        push   0x18
        push   eax
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_1_0E58
        add    esp, 0x14
        pop    edi
        pop    esi
        leave  
        ret    
        db  090h
loc_1_0C00:
        pop    es
        daa    
        comiss xmm2, dword ptr [edi]
        aaa    
        pop    ds
        aas    
        push   es
        push   cs
        push   ss
        push   ds
        add    eax, 0x152d0d25
        xor    eax, 0x24043d1d
        or     al, 0x2c
        adc    al, 0x34
        sbb    al, 0x3c
        add    esp, dword ptr [ebx]
        or     ebp, dword ptr [ebx]
        adc    esi, dword ptr [ebx]
        sbb    edi, dword ptr [ebx]
        add    ah, byte ptr [edx]
        or     ch, byte ptr [edx]
        adc    dh, byte ptr [edx]
        sbb    bh, byte ptr [edx]
        add    dword ptr [ecx], esp
        or     dword ptr [ecx], ebp
        adc    dword ptr [ecx], esi
        sbb    dword ptr [ecx], edi
        add    byte ptr [eax], ah
        or     byte ptr [eax], ch
        adc    byte ptr [eax], dh
        sbb    byte ptr [eax], bh
loc_1_0C40:
        cmp    dword ptr [ecx], esi
        sub    dword ptr [ecx], esp
        sbb    dword ptr [ecx], edx
        or     dword ptr [ecx], eax
        cmp    esi, dword ptr [ebx]
        sub    esp, dword ptr [ebx]
        sbb    edx, dword ptr [ebx]
        or     eax, dword ptr [ebx]
        cmp    eax, 0x1d252d35
        adc    eax, 0x373f050d
        das    
        daa    
        pop    ds
        pop    ss
        sysret 
        cmp    byte ptr [eax], dh
        sub    byte ptr [eax], ah
        sbb    byte ptr [eax], dl
        or     byte ptr [eax], al
        cmp    dh, byte ptr [edx]
        sub    ah, byte ptr [edx]
        sbb    dl, byte ptr [edx]
        or     al, byte ptr [edx]
        cmp    al, 0x34
        sub    al, 0x24
        sbb    al, 0x14
        or     al, 4
        push   ds
        push   ss
        push   cs
        push   es
loc_1_0C80:
        cmp    byte ptr [eax], dh
        sub    byte ptr [eax], ah
        sbb    byte ptr [eax], dl
        or     byte ptr [eax], al
        cmp    dword ptr [ecx], esi
        sub    dword ptr [ecx], esp
        sbb    dword ptr [ecx], edx
        or     dword ptr [ecx], eax
        cmp    dh, byte ptr [edx]
        sub    ah, byte ptr [edx]
        sbb    dl, byte ptr [edx]
        or     al, byte ptr [edx]
        cmp    esi, dword ptr [ebx]
        sub    esp, dword ptr [ebx]
loc_1_0C9C:
        push   ds
        push   ss
        push   cs
        push   es
        cmp    eax, 0x1d252d35
        adc    eax, 0x343c050d
        sub    al, 0x24
        sbb    al, 0x14
        or     al, 4
        sbb    edx, dword ptr [ebx]
        or     eax, dword ptr [ebx]
loc_1_0CB8:
        add    dword ptr [edx], eax
        add    al, 6
        or     byte ptr [edx], cl
        or     al, 0xe
        movups xmmword ptr [ebx], xmm2
        adc    eax, 0x1c1b1917
        sbb    al, 0x1b
        sbb    dword ptr [edi], edx
        adc    eax, 0xe0f1113
        or     al, 0xa
        or     byte ptr [esi], al
        add    al, 2
        add    dword ptr [0x170a10], ecx
        add    al, 2
        sbb    ecx, dword ptr [esi]
        add    eax, 0x12160914
        or     eax, dword ptr [ebx]
        sbb    dword ptr [edi], eax
        clts   
        sbb    dl, byte ptr [ebx]
        or     al, 1
        or     al, 0x17
        add    cl, byte ptr [eax]
        adc    bl, byte ptr [edx]
        add    dword ptr [ebx], ecx
        push   ss
        adc    byte ptr [ebx + edx], al
        unpcklps xmm1, xmmword ptr [edx]
        sbb    eax, dword ptr [0x150d1118]
        pop    es
        add    byte ptr [ebx], al
loc_1_0D08:
        pop    ds
        add    byte ptr [ecx], al
        add    al, byte ptr [ebx]
        add    al, 3
        add    al, 5
        push   es
        pop    es
        or     byte ptr [edi], al
        or     byte ptr [ecx], cl
loc_1_0D17:
        or     cl, byte ptr [ebx]
        or     al, 0xb
        or     al, 0xd
        push   cs
        movups xmm1, xmmword ptr [edi]
        adc    byte ptr [ecx], dl
        adc    dl, byte ptr [ebx]
        adc    al, 0x13
        adc    al, 0x15
        push   ss
        pop    ss
        sbb    byte ptr [edi], dl
        sbb    byte ptr [ecx], bl
        sbb    bl, byte ptr [ebx]
        sbb    al, 0x1b
        sbb    al, 0x1d
        push   ds
        pop    ds
        db  000h
loc_1_0D38:
        push   cs
        hlt    
        jge    0xd7d
        loop   0xd6d
        fistp  dword ptr [eax]
        mov    dword ptr [0x95bcc66a], eax
        pop    ecx
        xor    byte ptr [edi + 0x288ec1f4], al
        dec    ebp
        xchg   esi, eax
        adc    bh, byte ptr [ebx + 0x5f]
        mov    esp, 0xaa3e739
        sar    byte ptr gs:[edi], 1
        ror    dword ptr [eax + 0x7e], 1
        imul   byte ptr [ebx]
        and    esp, 0xffffffc9
        pop    es
        adc    ch, byte ptr [ebp + 0x5ab5906c]
        ror    byte ptr [esi - 0xbc5e459], 1
        dec    ebp
        and    dword ptr [ebp + 0x9c67c68], esi
        push   ebx
        loop   loc_1_0D17
        fidiv  dword ptr [eax + 9]
        sahf   
        inc    ebx
        outsd  dx, dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        and    dword ptr [ebp - 0x4b3418a4], ecx
        sbb    byte ptr [0x6809d4a6], bl
        lahf   
        xor    dword ptr [eax + 0x4b], 0xfffffff1
        loop   loc_1_0DD0
        mov    ch, 0x5a
        db  02Eh, 0C7h, 0D7h, 08Dh, 0BEh, 053h, 060h, 0F6h, 009h, 03Ah
loc_1_0DA0:
        inc    ecx
        jb     0xdcb
        lds    ebx, ptr [ebx]
        lodsb  al, byte ptr [esi]
        in     al, 0x9f
        cmp    dh, dh
        db  009h
loc_1_0DAB:
        pushal 
        lodsb  al, byte ptr [esi]
        sbb    edx, edi
        lea    ebx, [edi - 0x3a41acbf]
        jb     loc_1_0DDF
        in     al, 0xe2
        mov    esp, 0x7a47c124
        fist   dword ptr [esi]
        pop    eax
        add    eax, 0x903daff3
        mov    gs, word ptr [ecx - 0x4c]
        add    cl, 0x7b
        sbb    ch, ch
        daa    
        db  0D8h
loc_1_0DD0:
        outsd  dx, dword ptr [esi]
        stc    
        or     al, 0x95
        cmpsb  byte ptr [esi], byte ptr es:[edi]
        inc    ebx
        push   eax
        lodsb  al, byte ptr ds:[esi]
        int1   
        dec    edx
        das    
        jns    loc_1_0DA0
        xchg   esi, eax
loc_1_0DDF:
        pop    eax
        pushal 
        sbb    eax, 0xb70ee4d3
        xor    eax, 0x2f3e498b
        lds    edx, ptr [edx - 0x485c03a8]
        loopne 0xe07
        jp     0xe56
        or     eax, 0xbd4d68b
        mov    dl, 0x7e
        dec    edi
        nop    
        sbb    byte ptr [ebp - 0x38a6c31d], ch
        and    eax, 0x616186fa
        mov    ah, 0xdb
        lea    ebx, [ebx + eax*2]
        cmpsd  dword ptr [esi], dword ptr es:[edi]
        jle    loc_1_0DAB
        pop    edi
        push   es
        clc    
        loopne loc_1_0E3B
        cmp    edx, eax
        sbb    eax, 0xa684d8f2
        aas    
        jnp    loc_1_0E61
        retf   0x6359
        db  0BEh, 005h, 0E0h, 09Ch, 027h, 027h, 01Bh, 0E4h, 071h, 049h, 0ACh, 08Eh, 0D2h, 0F0h, 0C6h, 09Ah
        db  00Dh, 03Fh, 053h, 065h, 0B8h
loc_1_0E38:
        or     byte ptr [eax], dl
        push   ss
loc_1_0E3B:
        push   ds
        or     al, 0x1b
        add    dword ptr [ecx], edx
        pop    ss
        nop    dword ptr [0x91319]
        pop    es
        or     eax, 0x1c030218
        or     dl, byte ptr [edx]
        pop    ds
        or     edx, dword ptr [0xe1a0406]
        db  014h
sub_1_0E58:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 0x10]
loc_1_0E61:
        mov    esi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 8]
        call   sub_1_0E8A
        add    dword ptr [ebp + 8], 7
        mov    ecx, dword ptr [ebp + 0x14]
        sub    ecx, 8
        mov    dword ptr [ebp + 0x14], ecx
        cmp    ecx, 8
        jae    loc_1_0E61
        xor    eax, eax
        rep stosb byte ptr es:[edi], al
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
sub_1_0E8A:
        push   ebp
        mov    ebp, ebx
        and    ebp, 0xfffffffc
        push   edi
        push   eax
        lea    edi, [ebp + 0x4c]
        call   sub_1_0FF0
        mov    esi, 0xc40                    ; -> loc_1_0C40
        lea    ebx, [ebp + 0x4c]
        lea    edi, [ebp + 0x8c]
        mov    ecx, 0x40
loc_1_0EAD:
        lodsb  al, byte ptr [esi]
        xlatb  
        stosb  byte ptr es:[edi], al
        loop   loc_1_0EAD
        pop    esi
        lea    edi, [ebp + 0x4c]
        call   sub_1_1006
        mov    dword ptr [ebp + 4], 0xcb8    ; -> loc_1_0CB8
        mov    byte ptr [ebp + 3], 0x10
loc_1_0EC6:
        lea    esi, [ebp + 0xac]
        lea    edi, [ebp + 0xc]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xor    esi, esi
        mov    ebx, esi
        mov    edx, 3
        mov    dword ptr [ebp + 8], ebx
loc_1_0EE2:
        mov    ecx, 6
        xor    eax, eax
        cmp    esi, 0x18
        jge    loc_1_0F25
loc_1_0EEE:
        mov    edi, dword ptr [ebp + 4]
        mov    bl, byte ptr [edi]
        add    bl, byte ptr [esi + 0xcd8]    ; -> obj1:0x0CD8
        cmp    bl, 0x1c
        jl     loc_1_0F01
        sub    bl, 0x1c
loc_1_0F01:
        mov    bl, byte ptr [ebx + 0xc80]    ; -> loc_1_0C80
        lea    edi, [ebp + 0x4c]
        mov    al, byte ptr [ebx + edi]
        mov    bl, byte ptr [esi + 0xd08]    ; -> loc_1_0D08
        lea    edi, [ebp + 0xac]
        xor    al, byte ptr [edi + ebx]
        shr    al, 1
        rcl    ah, 1
        inc    esi
        loop   loc_1_0EEE
        jmp    loc_1_0F5A
loc_1_0F25:
        mov    edi, dword ptr [ebp + 4]
        mov    bl, byte ptr [edi]
        add    bl, byte ptr [esi + 0xcd8]    ; -> obj1:0x0CD8
        cmp    bl, 0x1c
        jl     loc_1_0F38
        sub    bl, 0x1c
loc_1_0F38:
        mov    bl, byte ptr [ebx + 0xc9c]    ; -> loc_1_0C9C
        lea    edi, [ebp + 0x4c]
        mov    al, byte ptr [ebx + edi]
        mov    bl, byte ptr [esi + 0xd08]    ; -> loc_1_0D08
        lea    edi, [ebp + 0xac]
        xor    al, byte ptr [edi + ebx]
        shr    al, 1
        rcl    ah, 1
        inc    esi
        loop   loc_1_0F25
loc_1_0F5A:
        mov    ecx, 4
        mov    bl, ah
        add    ebx, dword ptr [ebp + 8]
        shr    ebx, 1
        mov    al, byte ptr [ebx + 0xd38]    ; -> loc_1_0D38
        jae    loc_1_0F70
        shr    al, cl
loc_1_0F70:
        mov    edi, edx
loc_1_0F72:
        xor    ebx, ebx
        mov    bl, byte ptr [edi + 0xe38]    ; -> loc_1_0E38
        mov    ah, byte ptr [ebp + ebx + 0x8c]
        xor    ah, al
        mov    byte ptr [ebp + ebx + 0x2c], ah
        shr    al, 1
        dec    edi
        loop   loc_1_0F72
        xor    ebx, ebx
        add    dword ptr [ebp + 8], 0x40
        add    edx, 4
        cmp    esi, 0x30
        jne    loc_1_0EE2
        lea    esi, [ebp + 0x2c]
        lea    edi, [ebp + 0xac]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebp + 0xc]
        lea    edi, [ebp + 0x8c]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        inc    dword ptr [ebp + 4]
        dec    byte ptr [ebp + 3]
        jne    loc_1_0EC6
        mov    esi, 0xc00                    ; -> loc_1_0C00
        lea    ebx, [ebp + 0x8c]
        pop    edi
        mov    edx, 8
loc_1_0FDB:
        mov    ecx, 8
loc_1_0FE0:
        lodsb  al, byte ptr [esi]
        xlatb  
        shr    al, 1
        adc    ah, ah
        loop   loc_1_0FE0
        mov    byte ptr [edi], ah
        inc    edi
        dec    edx
        jne    loc_1_0FDB
        pop    ebp
        ret    
sub_1_0FF0:
        mov    edx, 8
loc_1_0FF5:
        mov    ecx, 8
        lodsb  al, byte ptr [esi]
loc_1_0FFB:
        add    eax, eax
        mov    byte ptr [edi], ah
        inc    edi
        loop   loc_1_0FFB
        dec    edx
        jne    loc_1_0FF5
        ret    
sub_1_1006:
        mov    edx, 7
        mov    ebx, 2
loc_1_1010:
        mov    ecx, 9
        lodsb  al, byte ptr [esi]
loc_1_1016:
        cmp    ecx, ebx
        jne    loc_1_1029
        mov    byte ptr [edi], 1
        cmp    ebx, 9
        jne    loc_1_102D
        mov    ebx, 2
        jmp    loc_1_102D
loc_1_1029:
        shl    al, 1
        rcl    byte ptr [edi]
loc_1_102D:
        inc    edi
        loop   loc_1_1016
        inc    ebx
        dec    edx
        jne    loc_1_1010
        ret    
        db  000h, 000h, 000h
sub_1_1038:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        movzx  eax, word ptr [esi + 0x1c]
        cmp    eax, 0x5f8a
        je     loc_1_1058
        mov    eax, 1
        pop    edi
        pop    esi
        leave  
        ret    
        db  090h
loc_1_1058:
        cmp    dword ptr [ebp + 0x18], 0
        jne    loc_1_1070
        mov    edi, dword ptr [ebp + 0xc]
        mov    word ptr [edi + 0x1a], 0x32
        sub    eax, eax
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_1_1070:
        mov    edx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [ebp - 4], eax
        mov    edi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   edx
        push   ecx
        call   sub_1_0164
        add    esp, 0x10
        mov    word ptr [edi + 0x1a], ax
        or     ax, ax
        jne    loc_1_10A6
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [esi + 0x1c], ax
        mov    word ptr [edi + 0x1a], ax
loc_1_10A6:
        cmp    dword ptr [ebp - 8], 0xea
        jne    loc_1_10B5
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x14], eax
loc_1_10B5:
        sub    eax, eax
        pop    edi
        pop    esi
        leave  
        ret    
        db  090h
loc_1_10BC:
        pushfd 
        add    dword ptr [eax], eax
        add    byte ptr [ebx - 0x4cffffff], ah ; -> loc_1_01A3
        xor    byte ptr [eax], al
        add    byte ptr [eax - 0x4affffff], dh ; -> loc_1_01B0
        add    dword ptr [eax], eax
        add    byte ptr [eax - 0x3fffffff], bh ; -> loc_1_01B8
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    eax, dword ptr [eax + 0x304]
        add    byte ptr [esi + 0x4e], dl
        je     0x113c
        jne    loc_1_115B
        and    byte ptr [eax], al
        add    byte ptr [ecx], 0xa0
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh            ; -> obj1:0x0079
        add    byte ptr [eax], al
        jns    loc_1_10FA
loc_1_10FA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    esp, 0x7000010
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        db  000h
loc_1_1118:
        add    edi, edi
        db  0FFh, 0FFh, 0FFh
sub_1_111D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1121:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_115B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1179:
        add    byte ptr [eax], al
loc_1_117B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_1196:
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_119A:
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        xor    al, 0
        add    byte ptr [eax], al
        xor    eax, 0x36000000
        add    byte ptr [eax], al
        add    byte ptr [edi], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        add    byte ptr [eax], al
        add    byte ptr [ebx], bh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
        cmp    eax, 0x3e000000
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [ebp], al
        add    byte ptr [eax], al
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        dec    edx
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_1206:
        add    byte ptr [eax + eax], 0
loc_1_120A:
        add    dword ptr [eax + eax], 0x48200
        add    byte ptr [edx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 2  vsize=0x50  RW-
        db  000h, 000h, 000h, 000h, 04Bh, 047h, 053h, 021h, 040h, 023h, 024h, 025h, 080h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xce0  R-X
loc_3_0000:
        sub    eax, eax
        mov    edi, 0x204c                   ; -> obj2:0x004C
        mov    ecx, 0x204c                   ; -> obj2:0x004C
        sub    ecx, edi
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [0x1114], ebx       ; -> obj1:0x1114
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_3_0043
        cmp    eax, 0x22
        jne    loc_3_0043
        call   sub_3_01B8
        push   0x377c                        ; -> obj3:0x077C
        push   0x86
        push   0x30a
        call   sub_1_026C
        add    esp, 0xc
        ret    
loc_3_0043:
        int3   
        clc    
        ret    
loc_3_0046:
        mov    esi, 0x12a                    ; -> loc_1_012A
        mov    eax, 0x400024
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0x1119], esi       ; -> obj1:0x1119
        clc    
        ret    
        db  0CDh, 020h, 02Ch, 080h, 040h, 000h
sub_3_0064:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, 0x30ab                   ; -> loc_3_00AB
        mov    edx, dword ptr [esp + 0x10]
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        mov    eax, edx
        sbb    edx, edx
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], edx
        pop    edi
        pop    esi
        ret    
sub_3_0086:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, 0x30ab                   ; -> loc_3_00AB
        mov    eax, dword ptr [esp + 0x10]
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        sbb    edx, edx
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], edx
        or     edx, 1
        and    eax, edx
        pop    edi
        pop    esi
        ret    
loc_3_00AB:
        dec    esi
        je     loc_3_0126
        outsd  dx, dword ptr [esi]
        jb     0x11d
        add    byte ptr [ebx + 0x111d0d], cl ; -> sub_1_111D
        add    bh, bh
        add    eax, 0x111d                   ; -> sub_1_111D
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ecx*4 + 0x1121], eax ; -> loc_1_1121
        ret    
sub_3_00CB:
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    eax, edx
        ret    
sub_3_00D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        sub    esi, esi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 8
        ja     loc_3_018B
        jmp    dword ptr [eax*4 + 0x3194]    ; -> obj3:0x0194
        db  08Dh, 049h, 000h
loc_3_00F4:
        lea    eax, [ebp - 4]
        push   eax
        push   0x3cc0                        ; -> obj3:0x0CC0
        push   dword ptr [ebp + 8]
        call   sub_3_0064
        add    esp, 0xc
        mov    edx, eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_3_018B
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], 1
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
loc_3_0126:
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp + 0x14]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], ecx
        mov    esi, 1
        mov    eax, esi
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_0158:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_3_0086
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        jne    loc_3_018B
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], 3
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], 4
        mov    esi, 1
loc_3_018B:
        mov    eax, esi
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h
        ; ---- dispatch table (9 entries) ----
        dd  loc_3_00F4
        dd  loc_3_00F4
        dd  loc_3_00F4
        dd  loc_3_018B
        dd  loc_3_018B
        dd  loc_3_0158
        dd  loc_3_0158
        dd  loc_3_00F4
        dd  loc_3_00F4
sub_3_01B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x78], 0
        mov    dword ptr [ebp - 0x74], 2
        mov    dword ptr [ebp - 0x70], 1
        lea    eax, [ebp - 0x58]
        mov    dword ptr [ebp - 0x6c], eax
        mov    dword ptr [ebp - 0x68], 0x37a8 ; -> loc_3_07A8
        mov    dword ptr [ebp - 0x64], 0x80000002
        push   dword ptr [ebp - 0x6c]
        push   dword ptr [ebp - 0x68]
        push   dword ptr [ebp - 0x64]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0228
        push   0x37a8                        ; -> loc_3_07A8
        push   0x17db
        call   sub_3_0734
        add    esp, 8
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
loc_3_0228:
        mov    ebx, 0x3980                   ; -> loc_3_0980
loc_3_022D:
        sub    eax, eax
        mov    dword ptr [ebp - 0x10], 0x50
        lea    edi, [ebp - 0xc8]
        mov    ecx, 0x14
        rep stosd dword ptr es:[edi], eax
        test   byte ptr [ebx], 1
        je     loc_3_0270
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc8]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebx + 0xc]
        push   dword ptr [ebx + 8]
        call   sub_3_00D4
        add    esp, 0x14
        or     eax, eax
        je     loc_3_0270
        sub    esi, esi
        jmp    loc_3_035F
loc_3_0270:
        cmp    dword ptr [ebx + 4], 0
        je     loc_3_0314
        lea    eax, [ebp - 0x14]
        mov    dword ptr [ebp - 0x54], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 0x50], eax
        mov    dword ptr [ebp - 0x4c], 0x80000002
        push   dword ptr [ebp - 0x54]
        push   dword ptr [ebp - 0x50]
        push   dword ptr [ebp - 0x4c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        jne    loc_3_035F
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x48], eax
        lea    eax, [ebp - 0xc8]
        mov    dword ptr [ebp - 0x44], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x40], eax
        mov    dword ptr [ebp - 0x3c], 0
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 0x38], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x34], eax
        push   dword ptr [ebp - 0x48]
        push   dword ptr [ebp - 0x44]
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        push   dword ptr [ebp - 0x38]
        push   dword ptr [ebp - 0x34]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    esi, eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x30], eax
        push   dword ptr [ebp - 0x30]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        jmp    loc_3_035F
loc_3_0314:
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x2c], eax
        lea    eax, [ebp - 0xc8]
        mov    dword ptr [ebp - 0x28], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x24], eax
        mov    dword ptr [ebp - 0x20], 0
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x58]
        mov    dword ptr [ebp - 0x18], eax
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    esi, eax
loc_3_035F:
        mov    edx, dword ptr [ebx + 0xc]
        cmp    edx, 8
        ja     loc_3_053E
        jmp    dword ptr [edx*4 + 0x36fc]    ; -> obj3:0x06FC
        db  08Bh, 0FFh
loc_3_0374:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_3_0388
        or     esi, esi
        jne    loc_3_0388
        lea    esi, [ebp - 0xc8]
        jmp    loc_3_0393
        db  08Bh, 0FFh
loc_3_0388:
        or     edx, edx
        je     loc_3_05C8
        mov    esi, dword ptr [ebx + 0x18]
loc_3_0393:
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [ebp - 0xc], eax
        mov    ecx, eax
        cmp    dword ptr [ebx + 0x10], ecx
        ja     loc_3_05D8
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, ecx
        jae    loc_3_03C4
        cmp    edx, 2
        jne    loc_3_0630
        mov    dword ptr [ebp - 0xc], eax
loc_3_03C4:
        or     edx, edx
        jne    loc_3_03CC
        jmp    loc_3_03CF
        db  08Bh, 0FFh
loc_3_03CC:
        mov    eax, dword ptr [ebp - 0xc]
loc_3_03CF:
        inc    eax
        push   eax
        call   sub_1_01FC
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_3_063C
        mov    eax, dword ptr [ebx + 0x1c]
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [ebp - 4]
        mov    byte ptr [eax + edi], 0
        jmp    loc_3_053E
loc_3_040C:
        cmp    dword ptr [ebp - 8], 3
        jne    loc_3_0420
        or     esi, esi
        jne    loc_3_0420
        mov    eax, dword ptr [ebp - 0xc8]
        jmp    loc_3_0423
        db  08Bh, 0FFh
loc_3_0420:
        mov    eax, dword ptr [ebx + 0x18]
loc_3_0423:
        cmp    dword ptr [ebx + 0x10], eax
        ja     loc_3_0648
        cmp    dword ptr [ebx + 0x14], eax
        jb     loc_3_0654
        mov    ecx, dword ptr [ebx + 0x1c]
        mov    dword ptr [ecx], eax
        jmp    loc_3_053E
        db  090h
loc_3_0440:
        cmp    dword ptr [ebp - 8], 3
        jne    loc_3_0454
        or     esi, esi
        jne    loc_3_0454
        mov    ecx, dword ptr [ebp - 0xc8]
        jmp    loc_3_046F
        db  08Bh, 0FFh
loc_3_0454:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_3_046C
        or     esi, esi
        jne    loc_3_046C
        movzx  ecx, byte ptr [ebp - 0xc8]
        sub    ecx, 0x30
        jmp    loc_3_046F
        db  08Bh, 0FFh
loc_3_046C:
        mov    ecx, dword ptr [ebx + 0x18]
loc_3_046F:
        cmp    dword ptr [ebx + 0x10], ecx
        ja     loc_3_0660
        cmp    dword ptr [ebx + 0x14], ecx
        jb     loc_3_0660
        cmp    edx, 6
        jne    loc_3_048A
        mov    ecx, dword ptr [ebp + ecx*4 - 0x78]
loc_3_048A:
        mov    eax, dword ptr [ebx + 0x1c]
        mov    byte ptr [eax], cl
        jmp    loc_3_053E
loc_3_0494:
        mov    edi, dword ptr [ebx + 0x1c]
        mov    dword ptr [ebp - 4], edi
loc_3_049A:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_3_04AA
        or     esi, esi
        lea    eax, [ebp - 0xc8]
        je     loc_3_04AD
loc_3_04AA:
        mov    eax, dword ptr [ebx + 0x18]
loc_3_04AD:
        sub    esi, esi
        nop    
loc_3_04B0:
        cmp    byte ptr [eax], 0
        je     loc_3_0534
loc_3_04B5:
        mov    cl, byte ptr [eax]
        cmp    cl, 0x20
        je     loc_3_04C1
        cmp    cl, 9
        jne    loc_3_04C4
loc_3_04C1:
        inc    eax
        jmp    loc_3_04B5
loc_3_04C4:
        mov    cl, byte ptr [eax]
        cmp    cl, 0x30
        jl     loc_3_0684
        cmp    cl, 0x39
        jg     loc_3_0684
        movsx  ecx, cl
        sub    ecx, 0x30
        inc    eax
        cmp    dword ptr [ebx + 0x10], ecx
        ja     loc_3_066C
        cmp    dword ptr [ebx + 0x14], ecx
        jb     loc_3_0678
        cmp    edx, 7
        jne    loc_3_0508
        inc    esi
        cmp    esi, 8
        jge    loc_3_0511
        mov    edi, dword ptr [ebp - 4]
        mov    byte ptr [edi], cl
        inc    dword ptr [ebp - 4]
        jmp    loc_3_0511
        db  08Bh, 0FFh
loc_3_0508:
        mov    edi, 1
        shl    edi, cl
        or     esi, edi
loc_3_0511:
        mov    cl, byte ptr [eax]
        cmp    cl, 0x20
        je     loc_3_051D
        cmp    cl, 9
        jne    loc_3_0520
loc_3_051D:
        inc    eax
        jmp    loc_3_0511
loc_3_0520:
        mov    cl, byte ptr [eax]
        or     cl, cl
        je     loc_3_0534
        inc    eax
        cmp    cl, 0x2c
        je     loc_3_04B0
        jmp    loc_3_0684
        db  08Dh, 049h, 000h
loc_3_0534:
        cmp    edx, 8
        jne    loc_3_053E
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [eax], esi
loc_3_053E:
        add    ebx, 0x20
        cmp    ebx, 0x3cc0                   ; -> obj3:0x0CC0
        jb     loc_3_022D
        mov    eax, dword ptr [ebp - 0x58]
        mov    dword ptr [ebp - 0x5c], eax
        push   dword ptr [ebp - 0x5c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        cmp    dword ptr [0x1134], 0         ; -> obj1:0x1134
        je     loc_3_057C
        mov    eax, dword ptr [0x1134]       ; -> obj1:0x1134
        cmp    byte ptr [eax], 0
        jne    loc_3_06D2
loc_3_057C:
        call   sub_3_00CB
        mov    esi, eax
        or     esi, esi
        je     loc_3_06D2
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    ebx, [ecx - 1]
        or     ebx, ebx
        jle    loc_3_06D2
        push   ebx
        call   sub_1_01FC
        add    esp, 4
        or     eax, eax
        jne    loc_3_06B8
        push   8
        call   sub_3_0720
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_3_05C8:
        push   dword ptr [ebx + 8]
        push   0x17d5
        jmp    loc_3_068C
        db  08Dh, 049h, 000h
loc_3_05D8:
        mov    edi, 0x3cc4                   ; -> obj3:0x0CC4
        mov    ecx, 2
        mov    esi, dword ptr [ebx + 8]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_0600
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    loc_3_0600
        push   dword ptr [ebx + 8]
        push   0x1874
        jmp    loc_3_068C
        db  08Bh, 0FFh
loc_3_0600:
        mov    edi, 0x3cd0                   ; -> obj3:0x0CD0
        mov    ecx, 3
        mov    esi, dword ptr [ebx + 8]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_0624
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
        jne    loc_3_0624
        push   dword ptr [ebx + 8]
        push   0x1875
        jmp    loc_3_068C
        db  08Dh, 049h, 000h
loc_3_0624:
        push   dword ptr [ebx + 8]
        push   0x17d6
        jmp    loc_3_068C
        db  08Bh, 0FFh
loc_3_0630:
        push   dword ptr [ebx + 8]
        push   0x17d7
        jmp    loc_3_068C
        db  08Bh, 0FFh
loc_3_063C:
        push   8
        call   sub_3_0720
        add    esp, 4
        jmp    loc_3_0694
loc_3_0648:
        push   dword ptr [ebx + 8]
        push   0x17d8
        jmp    loc_3_068C
        db  08Bh, 0FFh
loc_3_0654:
        push   dword ptr [ebx + 8]
        push   0x17d9
        jmp    loc_3_068C
        db  08Bh, 0FFh
loc_3_0660:
        push   dword ptr [ebx + 8]
        push   0x17da
        jmp    loc_3_068C
        db  08Bh, 0FFh
loc_3_066C:
        push   dword ptr [ebx + 8]
        push   0x17d8
        jmp    loc_3_068C
        db  08Bh, 0FFh
loc_3_0678:
        push   dword ptr [ebx + 8]
        push   0x17d9
        jmp    loc_3_068C
        db  08Bh, 0FFh
loc_3_0684:
        push   dword ptr [ebx + 8]
        push   0x17e9
loc_3_068C:
        call   sub_3_0734
        add    esp, 8
loc_3_0694:
        mov    eax, dword ptr [ebp - 0x58]
        mov    dword ptr [ebp - 0x60], eax
        push   dword ptr [ebp - 0x60]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Bh, 0FFh
loc_3_06B8:
        mov    edi, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [ebx + eax - 1], 0
        mov    dword ptr [0x1134], eax       ; -> obj1:0x1134
loc_3_06D2:
        cmp    dword ptr [0x1140], 0         ; -> obj1:0x1140
        je     loc_3_06F2
        mov    edi, dword ptr [0x1140]       ; -> obj1:0x1140
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [0x1144], ecx       ; -> obj1:0x1144
loc_3_06F2:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  08Dh, 049h, 000h
        ; ---- dispatch table (9 entries) ----
        dd  loc_3_0374
        dd  loc_3_0374
        dd  loc_3_0374
        dd  loc_3_040C
        dd  loc_3_040C
        dd  loc_3_0440
        dd  loc_3_0440
        dd  loc_3_0494
        dd  loc_3_049A
sub_3_0720:
        push   ebp
        mov    ebp, esp
        mov    byte ptr [0x1118], 0          ; -> loc_1_1118
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x377c], eax       ; -> obj3:0x077C
        leave  
        ret    
sub_3_0734:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_0720
        add    esp, 4
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    word ptr [0x3784], cx         ; -> loc_3_0784
        mov    eax, ecx
        mov    edi, 0x3788                   ; -> loc_3_0788
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [0x3787], 1          ; -> obj3:0x0787
        pop    edi
        pop    esi
        leave  
        ret    
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 080h, 004h, 000h, 000h
loc_3_0784:
        or     al, 0
        add    byte ptr [eax], al
loc_3_0788:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_07A8:
        push   ebx
        jns    0x81e
        je     loc_3_0812
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    loc_3_0824
        jb     loc_3_0819
        outsb  dx, byte ptr [esi]
        je     loc_3_07FA
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x82d
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_3_081D
        push   ebx
        jb     loc_3_083B
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
        push   esi
        dec    esi
        je     loc_3_0846
        jne    0x845
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax*2 + 0x4e], cl
        push   edx
        dec    edi
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x79], dl
        jae    0x85c
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    loc_3_0860
        jb     loc_3_0855
        outsb  dx, byte ptr [esi]
        je     loc_3_0836
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_3_0869
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
loc_3_07FA:
        je     loc_3_0859
        arpl   word ptr [edi + 0x6e], bp
        je     loc_3_0874
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        arpl   word ptr [edi + 0x6d], bp
        jo     0x87f
        je     loc_3_0871
        jb     loc_3_087C
        popal  
        insd   dword ptr es:[edi], dx
        pop    esp
loc_3_0812:
        arpl   word ptr [edi + 0x6d], bp
        jo     0x88c
        je     loc_3_087E
loc_3_0819:
        jb     loc_3_0889
        popal  
        insd   dword ptr es:[edi], dx
loc_3_081D:
        add    byte ptr gs:[eax], al
loc_3_0820:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
loc_3_0824:
        push   ebp
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0830:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0834:
        push   edi
        dec    edi
loc_3_0836:
        push   edx
        dec    ebx
        inc    edi
        push   edx
        dec    edi
loc_3_083B:
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4f], al
loc_3_0846:
        dec    ebp
        dec    ebp
        inc    ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x55], cl
        dec    ebp
        inc    esp
        inc    edi
loc_3_0855:
        push   edx
        inc    ecx
        dec    ebp
        inc    edx
loc_3_0859:
        push   ebp
        inc    esi
        add    byte ptr [esi + 0x55], cl
        dec    ebp
        push   edi
loc_3_0860:
        dec    edi
        push   edx
        dec    ebx
        inc    edx
        push   ebp
        inc    esi
        add    byte ptr [eax], al
loc_3_0868:
        push   ebx
loc_3_0869:
        dec    ecx
        pop    edx
        push   edi
        dec    edi
        push   edx
        dec    ebx
        inc    edx
        push   ebp
loc_3_0871:
        inc    esi
        add    byte ptr [eax], al
loc_3_0874:
        dec    ebx
        inc    ebp
        inc    ebp
        push   eax
        inc    ebx
        dec    edi
        dec    esi
        dec    esi
loc_3_087C:
        add    byte ptr [eax], al
loc_3_087E:
        add    byte ptr [eax], al
loc_3_0880:
        push   ebx
        inc    ebp
        push   ebx
        push   ebx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        dec    edi
loc_3_0889:
        push   ebp
        push   esp
        add    byte ptr [ebx + 0x48], al
        inc    ecx
        push   edx
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x48], al
        inc    ecx
        push   edx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08A4:
        inc    ebp
        pop    eax
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x41], dl
        push   edi
        inc    edx
        push   ebp
        inc    esi
        add    byte ptr [eax], al
loc_3_08B8:
        push   eax
        push   edx
        dec    ecx
        dec    edi
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08C4:
        dec    ebp
        inc    ecx
        dec    ecx
        dec    esi
        push   esp
        inc    ecx
        dec    ecx
        dec    esi
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        push   edx
        dec    esp
        dec    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
loc_3_08D8:
        dec    esp
        dec    ebp
        inc    ecx
        dec    esi
        dec    esi
        dec    edi
        push   ebp
        dec    esi
        inc    ebx
        inc    ebp
        add    byte ptr [eax], al
loc_3_08E4:
        inc    ecx
        push   ebp
        inc    esp
        dec    ecx
        push   esp
        inc    ebp
        dec    esi
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08F4:
        inc    ecx
        push   ebp
        inc    esp
        dec    ecx
        push   esp
        inc    ebp
        push   esi
        inc    ebp
        dec    esi
        push   esp
        push   ebx
        add    byte ptr [ecx + 0x55], al
        inc    esp
        dec    ecx
        push   esp
        dec    esp
        dec    edi
        inc    edi
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0910:
        push   eax
        push   edx
        inc    ebp
        inc    esi
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4c], dl
        dec    edi
        push   edi
        dec    esp
        inc    ecx
        dec    esi
        inc    ecx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + ecx*2 + 0x52], al
        inc    ebp
        inc    ebx
        push   esp
        dec    eax
        dec    edi
        push   ebx
        push   esp
        add    byte ptr [eax], al
loc_3_093C:
        inc    ecx
        push   ebp
        push   esp
        dec    edi
        inc    esp
        dec    ecx
        push   ebx
        inc    ebx
        dec    edi
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        add    byte ptr [eax], al
loc_3_094C:
        inc    esp
        push   edx
        dec    edi
        push   eax
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0958:
        dec    esi
        dec    edi
        dec    esi
        push   eax
        dec    esi
        push   eax
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        push   eax
        dec    edi
        push   edx
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_096C:
        dec    esi
        dec    edi
        dec    esi
        push   eax
        dec    esi
        push   eax
        push   eax
        dec    ebp
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        push   eax
        dec    edi
        push   edx
        push   esp
        push   ebx
        add    byte ptr [eax], al
loc_3_0980:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fdiv   dword ptr [edi]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        add    byte ptr [eax], al
        add    al, ah                        ; -> obj3:0x07E0
        aaa    
        add    byte ptr [eax], al
        xor    al, 0x11
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        in     al, 0x37
        add    byte ptr [eax], al
        and    byte ptr [eax], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        add    byte ptr [eax], dh            ; -> loc_3_0830
        cmp    byte ptr [eax], al
        add    byte ptr [eax], bh            ; -> obj1:0x1138
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + edi], dh      ; -> loc_3_0834
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        add    byte ptr [eax + 0x38], al     ; -> obj3:0x0840
        add    byte ptr [eax], al
        cmp    al, 0x11
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        cmp    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax + edi], cl      ; -> obj3:0x084C
        add    byte ptr [eax + 0x11], al     ; -> obj1:0x1140
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        cmp    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x11], cl     ; -> obj1:0x1148
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    esp
        cmp    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        dec    esp
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x38], ch     ; -> loc_3_0868
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    al, 0xb
        add    byte ptr [eax], al
        push   eax
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + edi], dh      ; -> loc_3_0874
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ch
        std    
        add    byte ptr [eax], al
        pop    eax
        add    al, byte ptr [eax]
        add    byte ptr [ecx + edx], dl      ; -> obj1:0x1154
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x3000038], al ; -> loc_3_0880
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        jne    loc_3_0A97
loc_3_0A97:
        add    byte ptr [0x58000000], ch     ; -> obj1:0x1158
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + edi + 0x30000], cl ; -> obj3:0x088C
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [ecx + edx], bl      ; -> obj1:0x115C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x3000038], bl ; -> obj3:0x0898
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ch
        std    
        add    byte ptr [eax], al
        cli    
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x11], ah     ; -> obj1:0x1160
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        movsb  byte ptr es:[edi], byte ptr [esi]
        cmp    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax + edi + 0x11640000], ch ; -> obj3:0x08AC
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    al, 0x38
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   0x11
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x3000038], bh ; -> loc_3_08B8
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        and    eax, dword ptr [eax]
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        insb   byte ptr es:[edi], dx
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, al                        ; -> loc_3_08C4
        cmp    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x11], bh     ; -> loc_1_1179
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fdivr  dword ptr [eax]
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x11], bh     ; -> obj1:0x117A
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        in     al, 0x38
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x11], bh     ; -> loc_1_117B
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        hlt    
        cmp    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    dword ptr [ebx]
        add    byte ptr [eax], al
        inc    dword ptr [ebx]
        add    byte ptr [eax], al
        jl     0xbcf
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    3, eax
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    byte ptr [ecx], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl            ; -> loc_3_0910
        cmp    dword ptr [eax], eax
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [ecx + edi], bl      ; -> obj3:0x091C
        add    byte ptr [eax], al
        mov    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [ecx], bh
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    ds
        add    byte ptr [eax], al
        add    byte ptr [ecx + edi], ch      ; -> obj3:0x092C
        add    byte ptr [eax], al
        test   byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    byte ptr [ecx], bh
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x11], bh     ; -> obj1:0x1178
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, 0x39
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        call   0x3c000d56
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x11], dh     ; -> obj1:0x1170
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esp
        cmp    dword ptr [eax], eax
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [0x40000000], al
        or     eax, dword ptr [eax]
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx + edx], dh      ; -> obj1:0x1174
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x39], bl     ; -> loc_3_0958
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x11], dl     ; -> obj1:0x1190
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + edi], ch      ; -> loc_3_096C
        add    byte ptr [0], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x11], dl     ; -> obj1:0x1191
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x4f], dl
        push   edx
        dec    ebx
        inc    edi
        push   edx
        dec    edi
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4f], al
        dec    ebp
        push   eax
        push   ebp
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xa2  R-X
        test   ebx, 0x2a750003
        cmp    eax, 0x2572030a
        mov    eax, 0x21cd3000
        cmp    al, 7
        ja     0x33c0032
        jb     0x30
        ja     loc_4_0021
        cmp    ah, 0x1e
        jb     0x30
loc_4_0021:
        call   0x820f0043
        or     byte ptr [eax], al
        xor    ebx, ebx
        xor    esi, esi
        mov    eax, 0x33c30000
        db  0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h, 049h, 046h, 053h, 024h, 048h, 04Ch, 050h, 024h, 000h
        db  066h, 060h, 01Eh, 083h, 0ECh, 008h, 08Bh, 0FCh, 0B8h, 040h, 03Dh, 0BAh, 038h, 000h, 0CDh, 021h
        db  00Fh, 082h, 044h, 000h, 08Bh, 0D8h, 08Bh, 0D7h, 066h, 0C7h, 005h, 070h, 0E9h, 035h, 037h, 0B8h
        db  002h, 044h, 0B9h, 008h, 000h, 0CDh, 021h, 08Bh, 0D0h, 09Ch, 0B4h, 03Eh, 0CDh, 021h, 09Dh, 00Fh
        db  082h, 024h, 000h, 083h, 0FAh, 008h, 00Fh, 085h, 01Dh, 000h, 066h, 081h, 03Dh, 035h, 037h, 070h
        db  0EFh, 00Fh, 085h, 012h, 000h, 06Ah, 008h, 0FFh, 05Dh, 004h, 083h, 0C4h, 002h, 00Bh, 0C0h, 00Fh
        db  085h, 004h, 000h, 0F8h, 0EBh, 002h, 090h, 0F9h, 066h, 058h, 066h, 058h, 01Fh, 066h, 061h, 0C3h
        db  01Ah
VxD__LTEXT ENDS

; OCTK32 -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 10542 instructions recovered by recursive descent

Declare_Virtual_Device OCTK32, 3, 10, OCTK32_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x77a5  R-X
loc_1_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x4ff]
        mov    eax, edi
        or     dword ptr [esi + 0xb7], 0x8000
        push   eax
        call   sub_1_0984
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x4ce]
        push   eax
        call   sub_1_0888
        test   byte ptr [esi + 0xb7], 0x10
        je     loc_1_0059
        push   esi
        push   0                             ; -> loc_4_0350
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
        push   0x9c4
        call   sub_1_08A0
loc_1_0059:
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_0894
        mov    eax, dword ptr [esi + 0x545]
        push   eax
        call   sub_2_2270
loc_1_0071:
        push   1
        push   1
        push   esi
        call   sub_4_1BE0
loc_1_007B:
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [esi + 0x3e], 7
        jne    loc_1_00A4
        mov    eax, 1
        mov    cl, byte ptr [esi + 0x2f]
        shl    eax, cl
        not    eax
        and    dword ptr [0x1d], eax         ; -> obj1:0x779D
        jmp    loc_1_00CD
loc_1_00A4:
        mov    ecx, dword ptr [esi + 0x4f7]
        cmp    ecx, 0x1f
        mov    eax, 1
        ja     loc_1_00C0
        shl    eax, cl
        not    eax
        and    dword ptr [0x10], eax         ; -> obj1:0x7790
        jmp    loc_1_00CD
loc_1_00C0:
        sub    cl, 0x20
        shl    eax, cl
        not    eax
        and    dword ptr [0x14], eax         ; -> obj1:0x7794
loc_1_00CD:
        push   edi
        call   sub_1_083E
        push   edi
        call   sub_1_07D8
        mov    dx, 0x21
        mov    ecx, dword ptr [esi + 0x20]
        cmp    ecx, 7
        jbe    loc_1_00ED
        mov    dx, 0xa1
        sub    cx, 8
loc_1_00ED:
        in     al, dx
        mov    bx, 1
        shl    bx, cl
        and    ax, 0xff
        or     al, bl
        out    dx, al
        push   0
        push   0x683
        push   esi
        call   sub_1_08AC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0120:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    eax, [esi + 0x4ce]
        push   eax
        call   sub_1_0888
        push   esi
        push   0                             ; -> loc_4_0350
        add    esi, 0x515
        push   esi
        call   sub_1_089A
        push   0x9c4
        call   sub_1_08A0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0160:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_07CC
        push   0
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        call   sub_1_07C0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0190:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0x10]
        lea    eax, [ecx + 0xc]
        mov    dx, ax
        sub    al, al
        out    dx, al
        lea    eax, [ecx + 0xd]
        mov    dx, ax
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, 0x20
        out    dx, al
        lea    edx, [ecx + 5]
        sub    al, al
        out    dx, al
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0x4a
        mov    edi, esi
        mov    dword ptr [ebp - 0xc], 3
loc_1_01D0:
        mov    ax, word ptr [edi]
        mov    word ptr [ebp - 2], ax
        add    edi, 2
        shl    al, 8
        or     al, byte ptr [ebp - 1]
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        dec    dword ptr [ebp - 0xc]
        jne    loc_1_01D0
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [ecx + 2]
        mov    ax, word ptr [eax + 0x57]
        mov    word ptr [ebp - 0x10], ax
        shl    al, 8
        or     al, byte ptr [ebp - 0xf]
        mov    byte ptr [ebp - 0x14], al
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, byte ptr [ebp - 0x10]
        out    dx, al
        mov    dword ptr [ebp - 8], 3
loc_1_0216:
        mov    bx, word ptr [esi]
        not    bx
        add    esi, 2
        mov    ax, bx
        shl    al, 8
        or     al, bh
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 3]
        mov    al, bl
        out    dx, al
        dec    dword ptr [ebp - 8]
        jne    loc_1_0216
        mov    al, byte ptr [ebp - 0x14]
        not    al
        lea    edx, [ecx + 2]
        out    dx, al
        mov    al, byte ptr [ebp - 0x10]
        not    al
        lea    edx, [ecx + 3]
        out    dx, al
        lea    eax, [ecx + 0xc]
        mov    dx, ax
        mov    al, 1
        out    dx, al
        lea    eax, [ecx + 0xd]
        mov    dx, ax
        sub    al, al
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0270:
        mov    al, byte ptr [0xa38]          ; -> obj1:0x0A38
        cmp    al, 0
        jne    loc_1_0295
        inc    byte ptr [0xa38]              ; -> obj1:0x0A38
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0297
        push   0
        push   0
        call   sub_2_0AE0
        cmp    eax, 0
        jne    loc_1_0297
loc_1_0295:
        clc    
        ret    
loc_1_0297:
        stc    
        ret    
OCTK32_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_02A8
        jmp    dword ptr [eax*4 + 0x2aa]     ; -> obj1:0x02AA
loc_1_02A8:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_0270
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_02A8
        dd  loc_1_0270
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0320:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0350
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0350:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0x10]
        add    ecx, 2
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 8
        cmp    dword ptr [ebp + 0x14], 0xe
        jb     loc_1_0470
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     loc_1_0414
        mov    al, byte ptr [ecx]
        cmp    al, 0xff
        jne    loc_1_0392
        cmp    byte ptr [ecx + 1], 0xff
        je     loc_1_039C
loc_1_0392:
        cmp    al, 0xc0
        jne    loc_1_03B8
        cmp    byte ptr [ecx + 1], 0
        jne    loc_1_03B8
loc_1_039C:
        cmp    byte ptr [ecx + 2], 0xff
        jne    loc_1_03B8
        cmp    byte ptr [ecx + 3], 0xff
        jne    loc_1_03B8
        cmp    byte ptr [ecx + 4], 0xff
        jne    loc_1_03B8
        cmp    byte ptr [ecx + 5], 0xff
        mov    byte ptr [ebp - 2], 1
        je     loc_1_03BC
loc_1_03B8:
        mov    byte ptr [ebp - 2], 0
loc_1_03BC:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_03D0
        mov    dword ptr [ebp - 8], 8
        jmp    loc_1_047B
        db  08Bh, 0FFh
loc_1_03D0:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 0x1000
        jne    loc_1_03EC
        mov    dword ptr [ebp - 8], 0x4000
loc_1_03EC:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 2]
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        shl    eax, 0x10
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        or     edx, eax
        mov    dword ptr [ebp - 0xc], edx
        jmp    loc_1_047B
loc_1_0414:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x10], 0x30
        je     loc_1_0460
        mov    edx, dword ptr [esi + 0x1a]
        lea    eax, [esi + 0x18]
        cmp    dword ptr [ecx + 2], edx
        jne    loc_1_0437
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_043E
loc_1_0437:
        mov    dword ptr [ebp - 0x10], 1
loc_1_043E:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_0450
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_047E
        db  08Dh, 049h, 000h
loc_1_0450:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_047E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0460:
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_047E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0470:
        mov    dword ptr [ebp - 8], 0x20
        mov    byte ptr [ebp - 1], 0
loc_1_047B:
        mov    esi, dword ptr [ebp + 8]
loc_1_047E:
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_0561
loc_1_0489:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    loc_1_04CD
        test   ah, 0x40
        je     loc_1_04A2
        mov    eax, dword ptr [edi + 0xc]
        test   dword ptr [ebp - 0xc], eax
        jne    loc_1_04CD
loc_1_04A2:
        test   byte ptr [ebp - 7], 0x40
        je     loc_1_04AD
        test   ch, 0x20
        jne    loc_1_04CD
loc_1_04AD:
        test   byte ptr [ebp - 7], 0x10
        je     loc_1_04B9
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_1_04CD
loc_1_04B9:
        test   cl, 0x10
        je     loc_1_04C4
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_04CD
loc_1_04C4:
        test   cl, 0x20
        je     loc_1_0556
loc_1_04CD:
        inc    dword ptr [edi + 0x18]
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_0802
        mov    byte ptr [edi + 0x1e], 1
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_0556
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_050F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_050F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_0520
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_0526
        db  08Bh, 0FFh
loc_1_0520:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_0526:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_0535
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0808
loc_1_0535:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x2a]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_08AC
        mov    edi, ebx
        jmp    loc_1_0559
loc_1_0556:
        mov    edi, dword ptr [edi + 0x10]
loc_1_0559:
        or     edi, edi
        jne    loc_1_0489
loc_1_0561:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0570:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0580
        pop    ebp
        ret    4
sub_1_0580:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_05FE
loc_1_0590:
        inc    dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_05A6
        mov    byte ptr [edi + 0x1e], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0808
loc_1_05A6:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_05F7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_05BC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_05BC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_05D0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_05D6
        db  005h, 000h, 000h, 000h, 000h
loc_1_05D0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_05D6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x2a]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_08AC
        mov    edi, ebx
        jmp    loc_1_05FA
loc_1_05F7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_05FA:
        or     edi, edi
        jne    loc_1_0590
loc_1_05FE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        shl    eax, 0x10
        or     ecx, eax
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [edx + 3]
        shl    eax, 0x10
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [edx + 5]
        or     ecx, eax
        mov    al, byte ptr [edx]
        and    al, byte ptr [edx + 2]
        and    al, 0x80
        mov    byte ptr [ebp - 1], al
        or     al, al
        mov    eax, dword ptr [ebp + 8]
        je     loc_1_0662
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_0662
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_0662
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0662:
        mov    esi, dword ptr [eax + 0x10]
        or     esi, esi
        jne    loc_1_0672
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0672:
        test   esi, 0x20
        je     loc_1_0683
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0683:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     loc_1_06A0
        test   esi, 0x10
        je     loc_1_06A0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_06A0:
        mov    eax, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [eax]
        and    cl, 0x80
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     loc_1_072A
        mov    cl, byte ptr [eax]
        cmp    cl, 0xff
        jne    loc_1_06BC
        cmp    byte ptr [eax + 1], 0xff
        je     loc_1_06C7
loc_1_06BC:
        cmp    cl, 0xc0
        jne    loc_1_06E3
        cmp    byte ptr [eax + 1], 0
        jne    loc_1_06E3
loc_1_06C7:
        cmp    byte ptr [eax + 2], 0xff
        jne    loc_1_06E3
        cmp    byte ptr [eax + 3], 0xff
        jne    loc_1_06E3
        cmp    byte ptr [eax + 4], 0xff
        jne    loc_1_06E3
        cmp    byte ptr [eax + 5], 0xff
        mov    byte ptr [ebp - 1], 1
        je     loc_1_06E7
loc_1_06E3:
        mov    byte ptr [ebp - 1], 0
loc_1_06E7:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_0710
        test   esi, 8
        je     loc_1_0700
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_0700:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0710:
        test   esi, 0x6000
        je     loc_1_0721
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0721:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_072A:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0740:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsb dx, byte ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h
sub_1_077D:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insb byte ptr es:[edi], dx
        pop    edi
        ret    0xc
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_07BA:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_07C0:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_07C6:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_07CC:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_07D2:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_07D8:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_07DE:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_07E4:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_07EA:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_07F6:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_07FC:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_0802:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_0808:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_080E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_0814:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
sub_1_081A:
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_082C:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
sub_1_083E:
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8057h           ; cd2057802800
        VxDCall  VPOWERD.Service_8058h           ; cd2058802800
        VxDCall  VPOWERD.Service_8059h           ; cd2059802800
        VxDCall  VPOWERD.Service_805Ah           ; cd205a802800
        VxDCall  VPOWERD.Service_805Bh           ; cd205b802800
        VxDCall  VPOWERD.Service_805Ch           ; cd205c802800
        VxDCall  VPOWERD.Service_805Dh           ; cd205d802800
        VxDCall  VPOWERD.Service_805Eh           ; cd205e802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_0882:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_0888:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_088E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_0894:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_089A:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_08A0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_08A6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_08AC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
sub_1_08B2:
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
sub_1_08B8:
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
sub_1_08BE:
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
sub_1_08CA:
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_08D0:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_08D6:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_08DC:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_08E2:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
sub_1_08EE:
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_08F4:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
sub_1_0900:
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
sub_1_090C:
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_0918:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_0924:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_092A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_0930:
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
sub_1_0972:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_0978:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
sub_1_097E:
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
sub_1_0984:
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_098C:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    0xc
        db  056h, 057h, 09Ch, 08Bh, 04Ch, 024h, 018h, 08Bh, 074h, 024h, 014h, 08Bh, 07Ch, 024h, 010h, 03Bh
        db  0F7h, 073h, 00Dh, 00Bh, 0C9h, 074h, 01Fh, 0FDh, 049h, 003h, 0F1h, 003h, 0F9h, 041h, 0EBh, 014h
        db  08Bh, 0C6h, 02Bh, 0C7h, 083h, 0F8h, 004h, 072h, 00Bh, 08Ah, 0C1h, 024h, 003h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Ah, 0C8h, 0F3h, 0A4h, 09Dh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'OCTK32  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  OCTK32_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  040h, 028h, 023h, 029h, 06Fh, 06Ch, 069h, 074h, 06Fh, 06Bh, 02Eh, 063h, 020h, 024h, 052h, 065h
        db  076h, 069h, 073h, 069h, 06Fh, 06Eh, 03Ah, 020h, 020h, 020h, 031h, 02Eh, 031h, 038h, 020h, 020h
        db  024h, 000h, 000h, 000h, 040h, 028h, 023h, 029h, 040h, 021h, 023h, 021h, 020h, 04Fh, 06Ch, 069h
        db  063h, 06Fh, 06Dh, 020h, 054h, 06Fh, 06Bh, 065h, 06Eh, 02Dh, 052h, 069h, 06Eh, 067h, 020h, 044h
        db  072h, 069h, 076h, 065h, 072h, 020h, 066h, 06Fh, 072h, 020h, 043h, 068h, 069h, 063h, 061h, 067h
        db  06Fh, 02Ch, 020h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 020h, 032h, 02Eh, 030h, 033h, 000h
        db  028h, 043h, 029h, 020h, 043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 04Fh, 06Ch
        db  069h, 063h, 06Fh, 06Dh, 020h, 041h, 02Fh, 053h, 020h, 031h, 039h, 039h, 033h, 02Dh, 031h, 039h
        db  039h, 035h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 041h, 064h, 061h, 070h, 074h, 065h, 072h, 000h
loc_1_0AD0:
        inc    ecx
        popal  
        jo     0xb49
        jb     loc_1_0AD8
loc_1_0AD8:
        dec    ebp
        popal  
        js     0xb22
        jb     0xb3f
        insd   dword ptr es:[edi], dx
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0
loc_1_0AE8:
        dec    ebp
        popal  
        js     0xb32
        jb     0xb4f
        insd   dword ptr es:[edi], dx
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0
        inc    edx
        jne    loc_1_0B6E
        dec    esi
        jne    loc_1_0B6B
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x75], al
        jae    loc_1_0B56
        jne    loc_1_0B77
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x75], al
        jae    0xb68
        jns    loc_1_0B86
        add    byte ptr gs:[edx + 0x75], al
        jae    loc_1_0B70
        jns    loc_1_0B8E
        add    byte ptr gs:[edx + esi*2 + 0x61], dl
        outsb  dx, byte ptr [esi]
        jae    0xb93
        imul   esi, dword ptr [edx + eax*2 + 0x75], 0x72656666
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x72540000
        popal  
        outsb  dx, byte ptr [esi]
        jae    0xba7
        imul   esi, dword ptr [edx + eax*2 + 0x75], 0x72656666
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x72540000
        popal  
        outsb  dx, byte ptr [esi]
        jae    0xbbb
        imul   esi, dword ptr [edx + eax*2 + 0x75], 0x72656666
loc_1_0B56:
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    0xbc8
        je     loc_1_0B5C
loc_1_0B5C:
        push   esp
        jb     loc_1_0BC0
        outsb  dx, byte ptr [esi]
        jae    0xbcf
        imul   esi, dword ptr [edx + eax*2 + 0x75], 0x72656666
        inc    ebx
loc_1_0B6B:
        outsd  dx, dword ptr [esi]
        jne    loc_1_0BDC
loc_1_0B6E:
        je     loc_1_0B70
loc_1_0B70:
        push   edx
        arpl   word ptr gs:[edx + 0x75], ax
        inc    bx
loc_1_0B77:
        outsd  dx, dword ptr [esi]
        jne    0xbe8
        je     loc_1_0B7C
loc_1_0B7C:
        push   edx
        arpl   word ptr gs:[edx + 0x75], ax
        inc    bx
        outsd  dx, dword ptr [esi]
        jne    loc_1_0BF4
loc_1_0B86:
        je     loc_1_0B88
loc_1_0B88:
        inc    esi
        jb     0xbec
        insd   dword ptr es:[edi], dx
        push   ebx
loc_1_0B8E:
        je     loc_1_0BF1
        je     loc_1_0C07
        jae    loc_1_0B94
loc_1_0B94:
        inc    esi
        jb     loc_1_0BF8
        insd   dword ptr es:[edi], dx
        push   ebx
        je     0xbfd
        je     0xc13
        jae    loc_1_0BA0
loc_1_0BA0:
        inc    ebp
        popal  
        jb     loc_1_0C10
        jns    loc_1_0BF8
        insb   byte ptr es:[edi], dx
        popal  
        jae    0xc11
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BB0:
        inc    ebp
        popal  
        jb     0xc20
        jns    0xc08
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_1_0C21
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BC0:
        dec    edi
        arpl   word ptr [ebx + ebp*2 + 0x33], si
        xor    al, byte ptr [eax]
        add    byte ptr [edi], al
        add    byte ptr [edi], al
        add    byte ptr [eax + 0x40000001], al ; -> loc_1_0BC0
        sub    byte ptr [ebx], ah
        sub    dword ptr [ecx + 0x64], esp
        popal  
        jo     loc_1_0C4D
        jb     loc_1_0C0A
loc_1_0BDC:
        arpl   word ptr [eax], sp
        and    al, 0x52
        jbe    loc_1_0C4C
        jae    0xc4e
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        xor    dword ptr [edi], esi
        and    byte ptr [eax], ah
loc_1_0BF1:
        and    al, 0
        db  000h
loc_1_0BF4:
        or     al, 0
        or     al, 0
loc_1_0BF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BFC:
        or     al, 0
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        sub    byte ptr [ebx], ah
loc_1_0C07:
        sub    dword ptr [ecx + 0x64], esp
loc_1_0C0A:
        jo     loc_1_0C3F
        xor    ch, byte ptr [esi]
        arpl   word ptr [eax], sp
loc_1_0C10:
        and    al, 0x52
        jbe    0xc7e
        jae    0xc80
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        cmp    byte ptr [eax], ah
loc_1_0C21:
        and    byte ptr [eax + eax], ah
        inc    eax
        sub    byte ptr [ebx], ah
        sub    dword ptr [ecx + 0x64], esp
        jo     loc_1_0C95
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [esi + ebp + 0x63], 0x65522420
        jbe    0xca0
        jae    0xca2
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
loc_1_0C3F:
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], edi
        and    byte ptr [eax], ah
        and    al, 0
        add    byte ptr [eax + 0x28], al
        and    ebp, dword ptr [ecx]
loc_1_0C4C:
        popal  
loc_1_0C4D:
        jo     0xcb9
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [ebx], 0x20632e32
        and    al, 0x52
        jbe    loc_1_0CC5
        jae    loc_1_0CC7
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        cmp    dword ptr [eax], esp
        and    byte ptr [eax + eax], ah
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [0x7000000], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
loc_1_0C95:
        add    byte ptr [eax], al
        add    byte ptr [0x6000000], al
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [eax], al
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x28], al
        and    ebp, dword ptr [ecx]
        jb     loc_1_0D17
        jno    0xd29
        jae    0xd2b
        arpl   word ptr cs:[eax], sp
        and    al, 0x52
        jbe    0xd28
        jae    loc_1_0D2A
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    ah, byte ptr [eax]
loc_1_0CC5:
        and    byte ptr [eax], ah
loc_1_0CC7:
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], esi
        and    byte ptr [eax + eax], ah
        db  000h
loc_1_0CD0:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
loc_1_0D17:
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
loc_1_0D2A:
        add    al, byte ptr [eax]
        add    eax, 0x1000201
        add    al, byte ptr [edx]
        add    byte ptr [edx], al
        add    al, byte ptr [edx]
        add    byte ptr [ebx], al
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0x6000202
        add    al, byte ptr [edx]
        add    byte ptr [edi], al
        add    al, byte ptr [edx]
        add    byte ptr [eax], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx], cl
        add    al, byte ptr [edx]
        add    byte ptr [ebx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], cl
        add    al, byte ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [edx], eax
        add    al, 1
        add    dword ptr [edx], eax
        add    eax, 0x6020101
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx + eax]
        add    al, byte ptr [edx]
        add    eax, 0x6020202
        add    al, byte ptr [edx]
        add    al, byte ptr [edi]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        inc    dword ptr [edx]
        add    dword ptr [ecx], eax
        inc    dword ptr [ebx]
        add    dword ptr [ecx], eax
        inc    dword ptr [ecx + eax]
        add    edi, edi
        add    eax, 0x1ff0101
        add    al, byte ptr [edx]
        inc    dword ptr [edx]
        add    al, byte ptr [edx]
        db  0FFh
loc_1_0DBC:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 0x6000101
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0x10000101
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
loc_1_0E18:
        xor    dword ptr [esi], esi
        das    
        xor    al, 0x20
        push   esp
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x72655320
        jbe    loc_1_0E92
        jb     loc_1_0E4F
        inc    ecx
        popal  
        jo     0xea8
        jb     loc_1_0E37
loc_1_0E37:
        add    byte ptr [eax + 0x28], al
        and    ebp, dword ptr [ecx]
        js     0xeab
        imul   esi, dword ptr [edx + esi*2 + 0x63], 0x20632e76
        and    al, 0x52
        jbe    0xeb4
        jae    loc_1_0EB6
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_1_0E4F:
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        xor    dword ptr [ecx], esi
        and    byte ptr [eax], ah
        and    al, 0
        add    byte ptr [eax + 0x28], al
        and    ebp, dword ptr [ecx]
        js     loc_1_0ED4
        xor    esi, dword ptr [edx]
        arpl   word ptr cs:[eax], sp
        and    al, 0x52
        jbe    0xed5
        jae    0xed7
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], esi
        and    byte ptr [eax], ah
        and    al, 0
        inc    eax
        sub    byte ptr [ebx], ah
        sub    dword ptr [edi + ebp*2 + 0x77], esp
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebx + esi + 0x32], sp
        arpl   word ptr cs:[eax], sp
        and    al, 0x52
        jbe    0xef9
        jae    0xefb
loc_1_0E92:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        xor    dword ptr [esi], ebp
        xor    byte ptr [eax], ah
        and    byte ptr [eax + eax], ah
        add    byte ptr [eax], ah
        push   esp
        dec    ebp
        push   ebx
        and    byte ptr [ebp + 0x61], al
        insb   byte ptr es:[di], dx
        and    byte ptr gs:[ebp + 0x41], cl
        inc    ebx
        and    byte ptr [ebx + 0x6f], ah
        and    byte ptr gs:[esi + 0x65], dh
loc_1_0EB6:
        jb     loc_1_0F2B
        imul   ebp, dword ptr [edi + 0x6e], 0x322e3220
        xor    esp, dword ptr [eax]
        ja     0xf2c
        je     loc_1_0F2D
        and    byte ptr [edi + 0x70], ch
        je     loc_1_0F33
        insd   dword ptr es:[edi], dx
        imul   edi, dword ptr [edx + 0x61], 0x6e6f6974
        jae    loc_1_0EF4
loc_1_0ED4:
        jbe    0xf07
        xor    dword ptr [esi], ebp
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
        in     al, 0xea
loc_1_0EDE:
        add    byte ptr [eax], al
        mov    al, 0
        inc    eax
        add    byte ptr [eax + ebp*2 + 0x69], dl
        jae    loc_1_0F09
        arpl   word ptr [edi + 0x64], bp
        and    byte ptr gs:[ecx + 0x73], ch
        and    byte ptr [edx + 0x65], dh
        insb   byte ptr es:[edi], dx
loc_1_0EF4:
        popal  
        jae    0xf5d
        and    byte ptr fs:[ebp + 0x6e], dh
        jb     0xf20
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x2065636e
        dec    edi
loc_1_0F09:
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        jns    0xf39
        and    byte ptr [ecx + 0x6c], al
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x69], dh
        push   0x72207374
        jae    loc_1_0F82
        jb     0xf95
        add    dword ptr cs:[eax + 1], esp
        inc    edx
        or     byte ptr [eax], al
        push   ss
        or     byte ptr [ebx], al
loc_1_0F2B:
        mov    al, 4
loc_1_0F2D:
        loopne loc_1_0F2F
loc_1_0F2F:
        add    byte ptr [ecx], al
        and    byte ptr [eax], al
loc_1_0F33:
        add    bh, bh
        call   dword ptr [ebx]
        cld    
        add    eax, dword ptr [eax + 0x4201a007]
        or     byte ptr [eax], al
        pop    es
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        pop    es
        mov    edi, dword ptr [0x1600100]
        inc    edx
        add    byte ptr [eax - 0x74f8fded], al
        sub    eax, 0x10bc800
        dec    edx
        add    al, byte ptr [eax]
        mov    al, byte ptr [0xc8a00900]
        add    byte ptr [ecx], al
        add    ah, al
        add    byte ptr [eax + 0x401a007], ah
        add    byte ptr [eax], ah
        add    dword ptr [eax + 0xfe4001], esp
        dec    eax
        and    al, ah
        sub    al, byte ptr [ecx]
        inc    edx
        add    al, 0xe0
        add    dword ptr [edx], eax
        add    al, 0xe0
loc_1_0F82:
        or     dword ptr [edx], edx
        add    al, 0xe0
        add    dword ptr [edx + 0x1c016001], eax
        add    byte ptr [esi + edx], al
        add    eax, dword ptr [ecx]
        loopne loc_1_0F94
        inc    eax
loc_1_0F94:
        or     al, 0
        push   es
        mov    al, byte ptr [0xa007aeb1]
        add    dword ptr [eax + eax], eax
        sub    eax, 0xe020c2
        add    cl, byte ptr [eax + 0x141623e2]
        pop    es
        mov    al, byte ptr [0x2e000401]
        add    dword ptr [eax + 1], esp
        inc    edx
        add    eax, dword ptr [eax]
        push   ss
        or     eax, 0x401a007
        add    byte ptr [ecx], ah
        pop    es
        mov    byte ptr [eax - 0x76f900], dh
        dec    byte ptr [ecx]
        test   al, 9
        test   eax, 0x2e08a07
        push   es
        mov    al, byte ptr [0x561072f5]
        pop    es
        mov    byte ptr [eax - 0x5076f900], ah
        dec    byte ptr [ecx]
        test   al, 9
        test   eax, 0x7ae08a07
        push   es
        mov    al, byte ptr [0x4b1072f5]
        add    eax, 0x70401a0
        mov    byte ptr [eax], al
        or     byte ptr [edi], al
        mov    dword ptr [eax], eax
        jp     0xffa
        add    byte ptr [eax], al
        add    dword ptr [esi], eax
        mov    al, byte ptr [0x4010aaf0]
        push   es
        mov    al, byte ptr [0xe002aeb1]
        add    esi, esp
        pop    es
        mov    byte ptr [eax], al
        mov    al, byte ptr [0xfe008907]
        push   es
        mov    al, byte ptr [0x3510aaf0]
        add    ah, al
        add    byte ptr [eax + 0x7e01e004], ah
        add    al, 0xc8
        add    cl, byte ptr [ecx]
        add    esi, edx
        bound  ecx, qword ptr [eax - 0x3f]
        loopne loc_1_102A
        inc    eax
loc_1_102A:
        add    dword ptr [eax + 0x44001], esp
        into   
        sbb    byte ptr [esi], al
        or     dword ptr [esi], edx
        std    
        add    dword ptr [eax + 0x404001], esp
        enter  0x107, 0x40
        pop    es
        mov    byte ptr [ebx], al
        hlt    
        pop    es
        mov    dword ptr [edi], edi
        inc    byte ptr [esi]
        mov    al, byte ptr [0x1910aaf0]
        add    ah, al
        add    byte ptr [eax - 0x2d0e5ffa], ah
        adc    byte ptr [eax + ecx*8], dl
        or     byte ptr [esp + eax*2], al
        enter  0x409, 0x46
        push   es
        mov    al, byte ptr [0xd1000f1]
        pop    es
        and    eax, 0x13b1c07e
        enter  1, -0x20
        add    eax, 0x60401a0
        xchg   edx, eax
        adc    byte ptr [ebx], al
        shl    byte ptr [eax], 0xe0
        adc    ch, dh
        add    eax, esp
        add    dword ptr [eax + eax], eax
        adc    byte ptr [ebx], al
        mov    al, 0x10
        inc    dword ptr [ecx]
        mov    al, byte ptr [0x800401]
        add    eax, dword ptr [eax - 0x5fff7ff9]
        add    al, 0xc2
        int3   
        add    byte ptr [edi], 0x81
        mov    cl, 0x9c
        add    al, byte ptr [edx + 0x2161e00]
        pop    es
        xor    dword ptr [ecx - 0x737e3b5a], 0x2e91681
        add    byte ptr [eax], 0x7c
        push   ss
        add    eax, dword ptr [eax]
        add    byte ptr [edi], cl
        add    al, 0x5b
        pop    es
        and    ecx, 0x82070a
        add    al, 4
        loopne 0x10c6
        add    byte ptr [eax], 1
        add    ah, 0x91
        db  08Ch, 0B1h, 016h, 0D8h, 002h, 082h, 000h, 07Ch, 016h, 0FAh, 0C8h, 020h, 0E0h, 004h, 001h, 082h
        db  0E8h, 020h, 0E0h, 00Ch, 001h, 082h, 0C8h, 020h, 0E0h, 010h, 001h, 080h, 007h, 081h, 0E0h, 088h
        db  0C0h, 0B1h, 013h, 007h, 0C4h, 0B1h, 010h, 0FCh, 007h, 0A0h, 001h, 004h, 000h, 02Eh, 004h, 060h
        db  0B1h, 09Ch, 007h, 081h, 0E0h, 034h, 007h, 082h, 005h, 0FCh, 007h, 083h, 000h, 00Ah, 0CCh, 0B1h
        db  006h, 043h, 016h, 0FDh, 002h, 002h, 006h, 000h, 0D0h, 060h, 001h, 04Eh, 013h, 0EDh, 002h, 021h
        db  0F7h, 000h, 002h, 021h, 0C0h, 000h, 0DCh, 081h, 0D0h, 060h, 001h, 04Fh, 0C0h, 0C1h, 009h, 041h
        db  002h, 021h, 0F0h, 000h, 0DCh, 081h, 002h, 043h, 00Fh, 000h, 002h, 023h, 0F0h, 000h, 0DCh, 083h
        db  002h, 001h, 00Ch, 032h, 0C0h, 0A0h, 004h, 044h, 0C0h, 0E0h, 004h, 046h, 0C1h, 003h, 061h, 002h
        db  005h, 084h, 0C8h, 004h, 004h, 048h, 0C1h, 003h, 005h, 084h, 0A1h, 004h, 0A1h, 001h, 0C8h, 004h
        db  00Ch, 030h, 0C1h, 003h, 005h, 084h, 004h, 0F1h, 006h, 004h, 016h, 0FDh, 002h, 008h, 0B0h, 000h
        db  009h, 0A8h, 0C2h, 060h, 00Ch, 030h, 002h, 029h, 003h, 0FFh, 009h, 0A9h, 002h, 029h, 000h, 040h
        db  007h, 080h, 0A0h, 000h, 009h, 0A0h, 007h, 08Ah, 0AFh, 0FEh, 002h, 02Ah, 003h, 0FFh, 009h, 0AAh
        db  002h, 001h, 00Ch, 032h, 002h, 005h, 000h, 000h, 0C1h, 003h, 005h, 084h, 007h, 011h, 005h, 0C1h
        db  005h, 085h, 006h, 004h, 013h, 00Bh, 080h, 085h, 01Ah, 0F9h, 080h, 005h, 01Ah, 0F8h, 082h, 085h
        db  01Ah, 0F5h, 082h, 005h, 01Ah, 0F4h, 082h, 045h, 01Ah, 0F1h, 010h, 0F1h, 02Dh, 020h, 000h, 002h
        db  001h, 060h, 001h, 040h, 040h, 000h, 016h, 006h, 007h, 08Ah, 008h, 000h, 001h, 0A0h, 001h, 040h
        db  040h, 000h, 010h, 048h, 001h, 060h, 001h, 042h, 080h, 000h, 016h, 006h, 007h, 08Ah, 010h, 000h
        db  001h, 0A0h, 001h, 042h, 080h, 000h, 010h, 03Eh, 001h, 060h, 001h, 002h, 010h, 000h, 016h, 00Ah
        db  001h, 060h, 001h, 000h, 004h, 000h, 016h, 006h, 007h, 08Ah, 080h, 000h, 001h, 0A0h, 001h, 002h
        db  010h, 000h, 010h, 030h, 001h, 060h, 001h, 002h, 008h, 000h, 016h, 00Ah, 001h, 060h, 001h, 000h
        db  004h, 000h, 016h, 006h, 001h, 0A0h, 001h, 002h, 008h, 000h, 002h, 00Dh, 000h, 001h, 010h, 00Dh
        db  001h, 060h, 001h, 002h, 004h, 000h, 016h, 016h, 001h, 060h, 001h, 000h, 008h, 000h, 016h, 012h
        db  001h, 0A0h, 001h, 002h, 004h, 000h, 002h, 00Dh, 000h, 002h, 0C3h, 0A0h, 001h, 00Eh, 0C3h, 0E0h
        db  001h, 010h, 007h, 08Ah, 020h, 000h, 001h, 060h, 001h, 000h, 080h, 000h, 013h, 00Bh, 007h, 08Ah
        db  040h, 000h, 010h, 008h, 007h, 08Ah, 000h, 004h, 010h, 005h, 007h, 08Ah, 000h, 002h, 010h, 002h
        db  007h, 08Ah, 000h, 008h, 003h, 000h, 000h, 000h, 004h, 0E0h, 001h, 082h, 007h, 08Bh, 005h, 0E0h
        db  0CEh, 0CAh, 0CEh, 0CDh, 0CEh, 0CEh, 0C6h, 0CFh, 0C3h, 020h, 007h, 058h, 023h, 020h, 0E0h, 004h
        db  013h, 012h, 007h, 08Bh, 0FFh, 018h, 002h, 08Ah, 080h, 000h, 013h, 00Ah, 005h, 08Bh, 0A2h, 0CDh
        db  002h, 08Ah, 040h, 000h, 013h, 005h, 002h, 08Ah, 020h, 000h, 013h, 002h, 007h, 08Bh, 0FFh, 01Dh
        db  0C8h, 00Bh, 001h, 004h, 010h, 00Dh, 0D3h, 020h, 001h, 005h, 011h, 0FDh, 0D8h, 020h, 007h, 0DFh
        db  001h, 017h, 007h, 08Bh, 0FFh, 080h, 0C8h, 00Bh, 001h, 004h, 0E8h, 020h, 0E0h, 00Ah, 001h, 000h
        db  0C2h, 0E0h, 001h, 004h, 022h, 0E0h, 0E1h, 088h, 016h, 0FBh, 002h, 0E0h, 000h, 0A0h, 004h, 0E0h
        db  001h, 082h, 0E8h, 020h, 0E0h, 00Ah, 001h, 000h, 0C2h, 0E0h, 001h, 000h, 022h, 0E0h, 0E0h, 006h
        db  013h, 0F8h, 001h, 0A0h, 001h, 040h, 0F6h, 000h, 004h, 060h, 0B0h, 082h, 003h, 000h, 000h, 002h
        db  007h, 0A0h, 001h, 002h, 0DFh, 0FFh, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 003h, 000h, 000h, 002h
        db  007h, 009h, 0C2h, 0A0h, 001h, 004h, 001h, 08Ah, 000h, 080h, 052h, 04Ah, 0D2h, 089h, 0C8h, 00Ah
        db  001h, 004h, 0D2h, 0A0h, 001h, 004h, 016h, 0F9h, 005h, 049h, 001h, 089h, 080h, 000h, 001h, 049h
        db  040h, 000h, 013h, 00Eh, 0F8h, 009h, 007h, 03Ah, 0C2h, 060h, 007h, 036h, 016h, 003h, 003h, 090h
        db  0FFh, 0FFh, 003h, 080h, 004h, 0E0h, 007h, 036h, 004h, 054h, 003h, 090h, 0FFh, 0FFh, 003h, 080h
        db  022h, 060h, 0E1h, 088h, 013h, 0C2h, 004h, 0E0h, 001h, 082h, 004h, 060h, 0B3h, 0D2h, 003h, 000h
        db  000h, 002h, 004h, 0C9h, 0C1h, 0A0h, 006h, 034h, 016h, 004h, 006h, 0A0h, 0C1h, 0EAh, 004h, 0E0h
        db  008h, 0D2h, 007h, 086h, 005h, 0E8h, 001h, 0A0h, 001h, 040h, 080h, 000h, 0C2h, 020h, 001h, 084h
        db  048h, 020h, 0E0h, 008h, 001h, 084h, 022h, 020h, 0E0h, 008h, 013h, 008h, 001h, 060h, 001h, 0AEh
        db  000h, 001h, 016h, 004h, 001h, 0E0h, 006h, 034h, 080h, 000h, 010h, 006h, 0C2h, 020h, 008h, 0E4h
        db  013h, 006h, 001h, 0E0h, 006h, 034h, 008h, 000h, 004h, 0E0h, 006h, 030h, 007h, 009h, 005h, 0A0h
        db  005h, 0EEh, 006h, 020h, 005h, 0ECh, 016h, 002h, 0C2h, 016h, 016h, 003h, 0C2h, 049h, 016h, 012h
        db  003h, 080h, 0C5h, 098h, 0C1h, 0E8h, 000h, 002h, 0E9h, 0E0h, 0E0h, 014h, 000h, 004h, 004h, 0D7h
        db  002h, 027h, 000h, 008h, 006h, 0A0h, 0C1h, 080h, 0C2h, 016h, 013h, 004h, 0C8h, 028h, 000h, 008h
        db  005h, 0ECh, 013h, 0EFh, 004h, 054h, 003h, 000h, 000h, 002h, 0C1h, 0E0h, 001h, 086h, 002h, 047h
        db  000h, 00Eh, 0C2h, 0A7h, 0E1h, 092h, 004h, 05Ah, 007h, 08Ah, 0A0h, 000h, 0C8h, 00Ah, 001h, 086h
        db  0A1h, 0C7h, 002h, 027h, 0E1h, 09Ah, 0E8h, 037h, 006h, 034h, 0E8h, 017h, 006h, 0D2h, 004h, 0E0h
        db  006h, 030h, 004h, 060h, 0B9h, 038h, 0E8h, 00Ah, 006h, 0D2h, 001h, 0E0h, 006h, 034h, 008h, 000h
        db  004h, 0E0h, 006h, 030h, 0E0h, 020h, 0E0h, 018h, 004h, 05Bh, 005h, 0A0h, 008h, 0D2h, 088h, 020h
        db  008h, 0D2h, 0E0h, 016h, 01Ah, 0E5h, 004h, 0E0h, 008h, 0D2h, 006h, 0A0h, 0DFh, 0ECh, 003h, 080h
        db  005h, 0A0h, 008h, 0E4h, 003h, 080h, 0C3h, 001h, 013h, 0FBh, 001h, 060h, 009h, 018h, 000h, 001h
        db  013h, 002h, 005h, 0A0h, 008h, 0E4h, 003h, 000h, 000h, 002h, 0C0h, 0E0h, 001h, 06Ch, 0C3h, 020h
        db  001h, 08Ah, 098h, 020h, 001h, 083h, 0E2h, 002h, 016h, 03Ch, 0C0h, 041h, 013h, 006h, 0C8h, 001h
        db  001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h, 011h, 001h, 010h, 070h, 001h, 060h, 001h, 09Ch, 000h, 040h
        db  016h, 06Eh, 0D8h, 020h, 008h, 0E1h, 001h, 083h, 0C0h, 041h, 013h, 004h, 0C8h, 001h, 001h, 08Ah
        db  0C8h, 001h, 008h, 0CAh, 007h, 086h, 000h, 043h, 0C8h, 006h, 001h, 06Ch, 002h, 007h, 0FCh, 002h
        db  0C2h, 017h, 004h, 060h, 0B6h, 0DEh, 004h, 0E0h, 008h, 0CAh, 061h, 0C7h, 007h, 008h, 001h, 060h
        db  0FCh, 006h, 000h, 040h, 013h, 002h, 002h, 008h, 000h, 001h, 010h, 009h, 0C2h, 04Ch, 0C3h, 020h
        db  0FCh, 000h, 013h, 02Eh, 0C8h, 00Ch, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h, 011h, 01Fh, 001h, 04Bh
        db  001h, 000h, 016h, 0F4h, 022h, 0C8h, 013h, 016h, 001h, 0CBh, 040h, 000h, 0C8h, 00Bh, 0FCh, 002h
        db  010h, 011h, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 038h, 0C1h, 0E0h, 008h, 0CAh, 0C3h, 001h
        db  013h, 021h, 0C2h, 04Ch, 013h, 015h, 0C8h, 00Ch, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h, 011h, 006h
        db  081h, 0CCh, 013h, 0D1h, 0C2h, 04Ch, 0C3h, 020h, 0FCh, 000h, 010h, 0F4h, 0C8h, 009h, 001h, 06Ch
        db  0C2h, 0E0h, 0FCh, 002h, 016h, 01Eh, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C3h, 009h, 010h, 019h
        db  0C8h, 009h, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h, 016h, 005h, 007h, 0A0h, 0FCh, 002h, 080h, 000h
        db  0C3h, 009h, 010h, 00Fh, 0C2h, 0E0h, 00Ch, 002h, 011h, 001h, 010h, 00Fh, 0D8h, 020h, 0E2h, 002h
        db  001h, 083h, 009h, 08Bh, 009h, 08Bh, 009h, 08Bh, 009h, 08Bh, 007h, 0A0h, 001h, 08Ah, 000h, 043h
        db  010h, 004h, 0C8h, 00Ch, 001h, 08Ah, 0C8h, 00Ch, 008h, 0CAh, 083h, 001h, 016h, 02Fh, 0C8h, 003h
        db  001h, 06Ch, 001h, 040h, 000h, 010h, 016h, 017h, 0C2h, 0E0h, 006h, 02Eh, 013h, 003h, 0C2h, 0E0h
        db  005h, 0F8h, 016h, 003h, 0C2h, 0E0h, 009h, 014h, 013h, 00Eh, 001h, 0E0h, 007h, 03Ah, 080h, 000h
        db  001h, 080h, 000h, 010h, 0C2h, 0E0h, 007h, 036h, 013h, 006h, 004h, 0E0h, 007h, 036h, 001h, 080h
        db  000h, 020h, 004h, 060h, 0B9h, 038h, 001h, 040h, 000h, 020h, 013h, 0F9h, 003h, 090h, 011h, 0FFh
        db  003h, 080h, 001h, 008h, 004h, 000h, 016h, 014h, 001h, 060h, 009h, 018h, 000h, 001h, 016h, 010h
        db  001h, 088h, 01Ah, 000h, 001h, 0C8h, 001h, 000h, 0C5h, 0C8h, 010h, 00Ah, 0C1h, 081h, 0C8h, 001h
        db  001h, 06Ch, 002h, 007h, 0FCh, 000h, 0C0h, 077h, 0C2h, 017h, 001h, 048h, 018h, 000h, 013h, 0E9h
        db  001h, 040h, 000h, 040h, 016h, 01Bh, 001h, 080h, 000h, 045h, 0C1h, 046h, 0D0h, 020h, 0FCh, 007h
        db  028h, 020h, 0E0h, 006h, 081h, 060h, 008h, 0CAh, 016h, 004h, 004h, 0E0h, 008h, 0CAh, 001h, 0C0h
        db  000h, 004h, 0C2h, 0E0h, 0FCh, 008h, 011h, 008h, 0C2h, 0E0h, 0FCh, 00Eh, 015h, 007h, 013h, 006h
        db  0C2h, 0E0h, 0FCh, 014h, 015h, 003h, 013h, 002h, 001h, 0C0h, 000h, 001h, 001h, 048h, 001h, 000h
        db  013h, 005h, 001h, 000h, 040h, 080h, 013h, 06Ch, 004h, 060h, 0B5h, 066h, 001h, 0C0h, 000h, 040h
        db  001h, 040h, 000h, 004h, 016h, 00Bh, 001h, 048h, 000h, 001h, 016h, 003h, 001h, 040h, 040h, 000h
        db  013h, 005h, 001h, 0C8h, 040h, 000h, 005h, 0A0h, 008h, 0E4h, 0C5h, 0C8h, 005h, 0C7h, 0C2h, 0D7h
        db  062h, 0C4h, 005h, 00Bh, 002h, 02Bh, 0FFh, 0FCh, 0C5h, 0CBh, 015h, 002h, 081h, 046h, 013h, 063h
        db  001h, 008h, 05Eh, 000h, 016h, 060h, 001h, 008h, 000h, 088h, 013h, 00Dh, 001h, 040h, 000h, 001h
        db  013h, 00Ah, 001h, 008h, 000h, 002h, 016h, 007h, 001h, 048h, 000h, 001h, 016h, 004h, 007h, 08Ah
        db  000h, 080h, 006h, 0A0h, 0B5h, 01Eh, 002h, 086h, 000h, 043h, 016h, 010h, 001h, 040h, 040h, 000h
        db  016h, 00Bh, 001h, 008h, 000h, 003h, 013h, 008h, 0C2h, 084h, 002h, 02Ah, 0FFh, 0D8h, 0C8h, 006h
        db  001h, 06Ch, 068h, 00Ah, 0FCh, 004h, 010h, 05Fh, 004h, 060h, 0B8h, 0A2h, 0C0h, 000h, 011h, 0FCh
        db  0C2h, 060h, 009h, 018h, 001h, 040h, 040h, 000h, 016h, 00Ah, 001h, 048h, 000h, 001h, 013h, 035h
        db  001h, 048h, 000h, 002h, 013h, 00Ah, 001h, 049h, 000h, 004h, 016h, 0EEh, 010h, 006h, 001h, 049h
        db  000h, 002h, 013h, 003h, 001h, 008h, 000h, 003h, 013h, 068h, 001h, 049h, 000h, 001h, 016h, 00Dh
        db  0C8h, 006h, 001h, 06Ch, 002h, 007h, 0FCh, 002h, 0A1h, 0E0h, 008h, 0DEh, 0CDh, 0E0h, 005h, 0EEh
        db  0C5h, 0E0h, 0FCh, 004h, 0C8h, 020h, 008h, 0DEh, 0FCh, 004h, 001h, 000h, 040h, 080h, 016h, 001h
        db  0C1h, 046h, 004h, 0E0h, 0FCh, 000h, 007h, 087h, 005h, 0F8h, 0C2h, 017h, 013h, 008h, 005h, 0C7h
        db  0C8h, 017h, 001h, 06Ch, 0C8h, 005h, 0FCh, 000h, 0C5h, 0C6h, 004h, 060h, 0B5h, 066h, 0CDh, 0C5h
        db  0C5h, 0C6h, 004h, 060h, 0B5h, 066h, 004h, 060h, 0B8h, 086h, 0C8h, 006h, 001h, 06Ch, 081h, 085h
        db  013h, 01Ah, 0C2h, 0E0h, 0FCh, 004h, 015h, 017h, 0C2h, 086h, 0A2h, 08Ah, 0C1h, 0AAh, 00Ch, 032h
        db  0C8h, 006h, 001h, 06Ch, 0A8h, 00Bh, 0FCh, 004h, 009h, 01Ah, 0C8h, 00Ah, 001h, 06Ch, 0C2h, 0E0h
        db  0FCh, 002h, 004h, 0E0h, 0FCh, 000h, 0C8h, 006h, 001h, 06Ch, 0C8h, 00Bh, 0FCh, 002h, 006h, 0A0h
        db  0C0h, 0ECh, 0C8h, 006h, 001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 001h, 0A0h, 0FCh, 002h, 000h, 002h
        db  007h, 087h, 006h, 030h, 001h, 0E7h, 000h, 004h, 000h, 040h, 004h, 0D7h, 002h, 027h, 000h, 00Ch
        db  0C2h, 005h, 006h, 0A0h, 0C1h, 096h, 001h, 0C0h, 000h, 020h, 004h, 060h, 0B5h, 066h, 001h, 048h
        db  018h, 000h, 013h, 003h, 001h, 048h, 010h, 000h, 016h, 002h, 005h, 0A0h, 008h, 0E4h, 002h, 086h
        db  000h, 043h, 013h, 003h, 001h, 000h, 040h, 080h, 013h, 0ABh, 0C8h, 006h, 001h, 06Ch, 004h, 0E0h
        db  0FCh, 000h, 0C2h, 085h, 006h, 0A0h, 0C0h, 0ECh, 010h, 0E8h, 003h, 000h, 000h, 002h, 0C2h, 020h
        db  001h, 08Ch, 0C0h, 0E0h, 001h, 06Ch, 0C2h, 020h, 005h, 0F4h, 0C8h, 028h, 000h, 008h, 001h, 06Ch
        db  0C1h, 0E8h, 000h, 00Ah, 0C3h, 020h, 0FCh, 002h, 001h, 00Ch, 0FEh, 000h, 016h, 03Fh, 001h, 047h
        db  000h, 040h, 013h, 054h, 004h, 0E0h, 005h, 0F4h, 0CAh, 00Ch, 000h, 008h, 001h, 047h, 000h, 008h
        db  016h, 006h, 0C2h, 0A8h, 000h, 006h, 006h, 0A0h, 0C0h, 0ECh, 004h, 0E8h, 000h, 006h, 001h, 007h
        db  000h, 020h, 016h, 029h, 0C1h, 0E8h, 000h, 002h, 001h, 0E7h, 000h, 004h, 000h, 020h, 0C2h, 0D7h
        db  013h, 003h, 004h, 0D7h, 001h, 0C3h, 080h, 000h, 002h, 027h, 000h, 010h, 006h, 0A0h, 0C1h, 080h
        db  0C2h, 020h, 005h, 0F0h, 013h, 007h, 0C2h, 0E0h, 005h, 0F6h, 016h, 004h, 0C8h, 018h, 005h, 0F0h
        db  006h, 0A0h, 0BBh, 0A4h, 0C8h, 003h, 001h, 06Ch, 011h, 003h, 003h, 090h, 011h, 0FFh, 003h, 080h
        db  003h, 090h, 011h, 0F8h, 002h, 0E0h, 000h, 0C0h, 0C3h, 060h, 000h, 0FAh, 0C3h, 0A0h, 000h, 0FCh
        db  0C3h, 0E0h, 000h, 0FEh, 004h, 054h, 004h, 0E8h, 000h, 002h, 010h, 0E2h, 0C2h, 0E8h, 000h, 008h
        db  0C2h, 0A8h, 000h, 006h, 0C3h, 00Ch, 011h, 033h, 023h, 020h, 0E0h, 00Ah, 013h, 045h, 023h, 020h
        db  0E0h, 010h, 013h, 046h, 023h, 020h, 0E0h, 00Eh, 013h, 013h, 021h, 0E0h, 0E0h, 016h, 016h, 0B2h
        db  023h, 020h, 0E0h, 006h, 016h, 003h, 027h, 020h, 0E4h, 06Ah, 013h, 00Ah, 0C2h, 0E8h, 000h, 008h
        db  0C2h, 0A8h, 000h, 006h, 001h, 04Ch, 000h, 088h, 016h, 0A5h, 001h, 00Ch, 000h, 044h, 016h, 0A2h
        db  006h, 020h, 008h, 0C8h, 013h, 09Fh, 0C8h, 00Ah, 001h, 06Ch, 0C8h, 020h, 0E0h, 004h, 0FCh, 002h
        db  0C8h, 00Bh, 001h, 06Ch, 007h, 0A0h, 0FCh, 002h, 081h, 000h, 0C3h, 020h, 001h, 080h, 001h, 0A0h
        db  001h, 080h, 0C4h, 000h, 0C8h, 00Ch, 001h, 080h, 0C8h, 00Ah, 001h, 08Ch, 010h, 0B3h, 0C2h, 00Ah
        db  013h, 00Fh, 0C8h, 008h, 001h, 06Ch, 0C2h, 0A0h, 0FCh, 000h, 0C3h, 020h, 0FCh, 002h, 023h, 020h
        db  0E0h, 012h, 016h, 0F5h, 0C8h, 00Bh, 001h, 06Ch, 0C8h, 00Ch, 0FCh, 002h, 004h, 060h, 0B8h, 0BEh
        db  007h, 08Ah, 004h, 000h, 004h, 060h, 0B3h, 07Ch, 007h, 08Ah, 000h, 020h, 004h, 060h, 0B3h, 07Ch
        db  007h, 08Ah, 002h, 000h, 027h, 020h, 0E0h, 00Eh, 016h, 002h, 006h, 0A0h, 0B5h, 01Eh, 0C2h, 0A8h
        db  000h, 006h, 004h, 060h, 0B8h, 0DCh, 003h, 000h, 000h, 002h, 001h, 0C0h, 000h, 010h, 0C2h, 0E0h
        db  006h, 02Eh, 013h, 003h, 0C2h, 0E0h, 005h, 0F8h, 016h, 003h, 0C2h, 0E0h, 009h, 014h, 013h, 005h
        db  001h, 0E0h, 007h, 03Ah, 080h, 000h, 001h, 080h, 000h, 010h, 003h, 090h, 07Fh, 0FFh, 003h, 080h
        db  003h, 000h, 000h, 002h, 0C2h, 020h, 005h, 0F6h, 0E2h, 020h, 005h, 0F4h, 016h, 02Fh, 0D8h, 020h
        db  008h, 0E0h, 001h, 080h, 0C8h, 02Bh, 000h, 006h, 001h, 08Ch, 007h, 0A0h, 008h, 0C8h, 000h, 004h
        db  0C8h, 00Bh, 005h, 0F4h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 00Bh
        db  0C3h, 020h, 005h, 0F4h, 013h, 00Fh, 0C2h, 0A8h, 000h, 00Ah, 001h, 04Ah, 000h, 010h, 016h, 017h
        db  022h, 0A0h, 0E0h, 004h, 016h, 01Bh, 0C3h, 008h, 006h, 0A0h, 0BBh, 062h, 0C2h, 00Ch, 0C3h, 020h
        db  005h, 0F4h, 016h, 014h, 001h, 068h, 000h, 00Ah, 000h, 010h, 013h, 003h, 0C2h, 0E0h, 005h, 0F6h
        db  016h, 006h, 006h, 0A0h, 0BBh, 0A4h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 0C2h, 00Bh, 007h, 087h
        db  005h, 0F0h, 006h, 0A0h, 0C1h, 080h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 007h, 087h, 005h, 0F0h
        db  006h, 0A0h, 0C1h, 0C6h, 003h, 080h, 003h, 000h, 000h, 002h, 007h, 087h, 005h, 0F0h, 0C2h, 0CBh
        db  016h, 008h, 006h, 0A0h, 0BBh, 062h, 007h, 020h, 005h, 0F6h, 0CBh, 060h, 005h, 0F4h, 000h, 002h
        db  003h, 080h, 004h, 0E0h, 005h, 0F6h, 0C2h, 020h, 005h, 0F4h, 016h, 005h, 0C2h, 017h, 013h, 003h
        db  0C5h, 0D8h, 006h, 0A0h, 0BBh, 0A4h, 003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 08Bh, 006h, 0A0h
        db  0BBh, 062h, 0C1h, 0CAh, 002h, 027h, 000h, 010h, 007h, 088h, 005h, 0F0h, 0C1h, 088h, 0C2h, 018h
        db  013h, 026h, 082h, 0A8h, 000h, 002h, 016h, 0FAh, 0C2h, 0E8h, 000h, 00Ah, 022h, 0E0h, 0E0h, 01Eh
        db  016h, 0F5h, 0C5h, 098h, 022h, 0E0h, 0E0h, 01Ch, 016h, 00Bh, 0C8h, 028h, 000h, 006h, 000h, 0F4h
        db  002h, 0E0h, 000h, 0E0h, 006h, 0A0h, 0C0h, 0ECh, 002h, 0E0h, 000h, 0C0h, 004h, 0E8h, 000h, 006h
        db  022h, 0E0h, 0E0h, 018h, 013h, 0E4h, 0EAh, 020h, 0E0h, 022h, 000h, 00Ah, 0EAh, 0A0h, 0E0h, 018h
        db  000h, 004h, 004h, 0DAh, 006h, 0A0h, 0C1h, 080h, 006h, 047h, 0C2h, 006h, 010h, 0D8h, 0C8h, 006h
        db  005h, 0F2h, 0CBh, 060h, 005h, 0F4h, 000h, 002h, 004h, 054h, 0C2h, 020h, 005h, 0F4h, 013h, 013h
        db  001h, 0E0h, 001h, 09Ch, 040h, 000h, 00Bh, 08Bh, 00Bh, 08Bh, 001h, 060h, 001h, 09Ch, 040h, 000h
        db  016h, 00Ah, 0C2h, 060h, 001h, 06Ch, 0C8h, 028h, 000h, 006h, 001h, 06Ch, 0C2h, 0A0h, 0FCh, 002h
        db  011h, 003h, 0C8h, 009h, 001h, 06Ch, 004h, 05Bh, 0C8h, 009h, 001h, 06Ch, 0C2h, 04Bh, 007h, 087h
        db  005h, 0F0h, 006h, 0A0h, 0C1h, 0C6h, 004h, 0E0h, 005h, 0F4h, 004h, 059h, 0C2h, 0A8h, 000h, 00Ah
        db  011h, 00Dh, 0F2h, 0A0h, 008h, 0E0h, 0D8h, 00Ah, 001h, 080h, 0C8h, 028h, 000h, 006h, 001h, 08Ch
        db  007h, 0A0h, 008h, 0C8h, 000h, 004h, 0C8h, 008h, 005h, 0F4h, 004h, 05Bh, 0C3h, 020h, 001h, 06Ch
        db  0C8h, 028h, 000h, 006h, 001h, 06Ch, 0C8h, 020h, 0FCh, 00Eh, 001h, 0BCh, 0C8h, 020h, 0FCh, 010h
        db  001h, 0B0h, 0C8h, 020h, 0FCh, 012h, 001h, 0B2h, 0C8h, 00Ch, 001h, 06Ch, 0F2h, 0A0h, 008h, 0E0h
        db  001h, 08Ah, 010h, 000h, 001h, 0A0h, 001h, 080h, 0C4h, 000h, 010h, 0DDh, 007h, 033h, 007h, 033h
        db  010h, 00Ch, 007h, 013h, 007h, 023h, 000h, 002h, 0C8h, 0CBh, 000h, 006h, 002h, 023h, 000h, 018h
        db  0CCh, 0E0h, 001h, 06Ch, 0CCh, 0CDh, 0CCh, 0CEh, 0CCh, 0CFh, 007h, 083h, 006h, 030h, 0C1h, 0D3h
        db  013h, 00Ah, 007h, 083h, 007h, 036h, 0C1h, 0D3h, 013h, 006h, 007h, 083h, 000h, 0A0h, 000h, 093h
        db  0C8h, 00Ch, 001h, 06Ch, 003h, 080h, 007h, 063h, 000h, 002h, 015h, 039h, 0C2h, 063h, 000h, 004h
        db  042h, 063h, 000h, 006h, 013h, 0DBh, 0C3h, 063h, 000h, 01Ah, 0D2h, 049h, 013h, 00Ch, 006h, 0C9h
        db  072h, 049h, 0D2h, 069h, 0E1h, 0C2h, 006h, 0C9h, 072h, 049h, 048h, 0E9h, 0E0h, 004h, 000h, 004h
        db  0CBh, 049h, 000h, 002h, 004h, 052h, 0D2h, 069h, 0E1h, 0C2h, 006h, 0C9h, 072h, 049h, 002h, 029h
        db  000h, 010h, 0C2h, 0C3h, 0A2h, 0E9h, 0E1h, 0AAh, 0C3h, 01Bh, 002h, 089h, 000h, 012h, 016h, 00Dh
        db  0C8h, 00Ch, 001h, 06Ch, 0C6h, 0E0h, 0FCh, 000h, 016h, 00Dh, 048h, 0E9h, 0E0h, 004h, 000h, 004h
        db  0CBh, 049h, 000h, 002h, 0CBh, 04Ch, 000h, 004h, 004h, 052h, 0C6h, 0DCh, 016h, 003h, 048h, 0E9h
        db  0E0h, 004h, 000h, 004h, 0CBh, 049h, 000h, 002h, 0CBh, 04Ch, 000h, 004h, 004h, 052h, 003h, 090h
        db  001h, 0FFh, 000h, 093h, 0C8h, 00Ch, 001h, 06Ch, 003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 0DBh
        db  0C2h, 063h, 000h, 004h, 042h, 04Bh, 013h, 09Dh, 0D2h, 049h, 013h, 00Eh, 006h, 0C9h, 072h, 049h
        db  0D2h, 069h, 0E1h, 0C2h, 006h, 0C9h, 072h, 049h, 048h, 0E9h, 0E0h, 004h, 000h, 004h, 0CBh, 049h
        db  000h, 002h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 0D2h, 069h, 0E1h, 0C2h, 006h, 0C9h, 072h, 049h
        db  002h, 029h, 000h, 010h, 0C2h, 0C3h, 0A2h, 0E9h, 0E1h, 0AAh, 0C3h, 01Bh, 002h, 089h, 000h, 012h
        db  016h, 00Fh, 0C8h, 00Ch, 001h, 06Ch, 0C6h, 0E0h, 0FCh, 000h, 016h, 003h, 048h, 0E9h, 0E0h, 004h
        db  000h, 004h, 0CBh, 049h, 000h, 002h, 0CBh, 04Ch, 000h, 004h, 003h, 090h, 0FFh, 0FFh, 003h, 080h
        db  0C6h, 0DCh, 013h, 0F4h, 010h, 0F6h, 003h, 000h, 000h, 002h, 0C2h, 0BBh, 0C1h, 0BBh, 0D1h, 086h
        db  013h, 003h, 0EAh, 086h, 000h, 004h, 010h, 013h, 0D1h, 0A6h, 0E1h, 0C2h, 006h, 0C6h, 071h, 086h
        db  0C1h, 0CAh, 0A1h, 0E6h, 0E1h, 0BAh, 0EAh, 0A6h, 0E0h, 014h, 000h, 004h, 0C2h, 01Bh, 002h, 086h
        db  000h, 002h, 016h, 003h, 006h, 0A0h, 0C1h, 0A6h, 010h, 002h, 006h, 0A0h, 0C1h, 080h, 004h, 0DAh
        db  003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 0ABh, 000h, 006h, 007h, 08Ch, 005h, 0E8h, 0C2h, 05Ch
        db  013h, 016h, 0C1h, 0A0h, 005h, 0ECh, 081h, 08Ah, 01Ah, 01Ah, 0C1h, 0C6h, 0C2h, 009h, 0C2h, 059h
        db  013h, 020h, 0A1h, 0E9h, 000h, 008h, 082h, 087h, 012h, 0F9h, 0A2h, 0A9h, 000h, 008h, 062h, 087h
        db  0CAh, 0CAh, 000h, 008h, 06Ah, 04Ah, 000h, 008h, 0C6h, 0C9h, 0C6h, 00Bh, 003h, 080h, 0CAh, 0CAh
        db  000h, 008h, 0C8h, 00Ah, 005h, 0ECh, 004h, 0DBh, 0CFh, 00Bh, 0C7h, 00Bh, 003h, 080h, 061h, 08Ah
        db  0CAh, 046h, 000h, 008h, 0CAh, 0CAh, 000h, 008h, 0C8h, 00Ah, 005h, 0ECh, 0C6h, 0C9h, 0C7h, 00Bh
        db  003h, 080h, 062h, 087h, 0CAh, 0CAh, 000h, 008h, 004h, 0DBh, 0C6h, 00Bh, 0CBh, 00Bh, 000h, 002h
        db  003h, 080h, 003h, 000h, 000h, 002h, 0C1h, 0BBh, 0C2h, 0DBh, 007h, 08Ch, 005h, 0E8h, 0C2h, 04Ch
        db  004h, 0EDh, 000h, 002h, 0C2h, 009h, 0C2h, 059h, 013h, 018h, 081h, 0A9h, 000h, 002h, 016h, 0FAh
        db  082h, 0E9h, 000h, 004h, 016h, 0F7h, 0CBh, 049h, 000h, 004h, 0C2h, 099h, 0C6h, 00Ah, 013h, 00Ah
        db  083h, 008h, 013h, 004h, 0AAh, 0A9h, 000h, 008h, 000h, 008h, 003h, 080h, 0A8h, 02Ah, 000h, 008h
        db  005h, 0ECh, 003h, 080h, 0CBh, 008h, 000h, 002h, 003h, 080h, 007h, 02Dh, 000h, 002h, 007h, 08Ch
        db  000h, 008h, 0A3h, 006h, 0C2h, 04Ch, 0C2h, 009h, 0C2h, 059h, 013h, 013h, 082h, 0E9h, 000h, 004h
        db  016h, 0FAh, 007h, 0ADh, 000h, 002h, 000h, 001h, 0CBh, 049h, 000h, 004h, 0C6h, 019h, 013h, 001h
        db  003h, 080h, 083h, 008h, 016h, 004h, 049h, 0A0h, 0E0h, 014h, 000h, 004h, 003h, 080h, 0CBh, 008h
        db  000h, 002h, 003h, 080h, 003h, 000h, 000h, 002h, 006h, 00Bh, 011h, 015h, 013h, 043h, 007h, 08Bh
        db  04Eh, 000h, 001h, 060h, 001h, 042h, 000h, 080h, 013h, 002h, 007h, 08Bh, 03Ah, 000h, 0C8h, 00Bh
        db  001h, 044h, 001h, 0E0h, 001h, 040h, 002h, 000h, 001h, 0E0h, 001h, 040h, 020h, 000h, 007h, 084h
        db  0BCh, 004h, 004h, 060h, 0BCh, 012h, 0C8h, 020h, 0E0h, 016h, 000h, 0E0h, 0C2h, 0E0h, 009h, 018h
        db  022h, 0E0h, 0E0h, 010h, 016h, 003h, 0E8h, 020h, 0E0h, 014h, 000h, 0E0h, 0C8h, 020h, 0E0h, 004h
        db  001h, 082h, 0C8h, 020h, 000h, 0E2h, 001h, 08Ah, 004h, 0E0h, 008h, 0CAh, 004h, 0E0h, 005h, 0F4h
        db  004h, 0E0h, 005h, 0F8h, 004h, 0E0h, 005h, 0F0h, 004h, 0E0h, 007h, 042h, 007h, 0A0h, 001h, 088h
        db  000h, 020h, 0C2h, 0E0h, 008h, 0E2h, 013h, 009h, 007h, 0A0h, 001h, 088h, 000h, 080h, 0E8h, 020h
        db  0E0h, 016h, 001h, 080h, 001h, 0E0h, 001h, 082h, 003h, 000h, 007h, 08Bh, 0A0h, 000h, 0E8h, 00Bh
        db  001h, 086h, 003h, 080h, 004h, 0E0h, 001h, 086h, 001h, 0E0h, 001h, 09Ch, 000h, 040h, 001h, 0E0h
        db  001h, 09Ch, 040h, 000h, 004h, 0CBh, 003h, 0B0h, 006h, 00Bh, 013h, 004h, 001h, 060h, 001h, 09Ch
        db  040h, 000h, 016h, 0F9h, 004h, 0E0h, 001h, 080h, 0E8h, 020h, 0E0h, 008h, 009h, 018h, 007h, 08Bh
        db  080h, 000h, 0C8h, 00Bh, 007h, 098h, 0C8h, 00Bh, 007h, 078h, 007h, 08Bh, 087h, 06Fh, 048h, 00Bh
        db  007h, 03Ah, 0C2h, 0E0h, 007h, 050h, 002h, 08Bh, 007h, 058h, 013h, 010h, 0E8h, 020h, 0E0h, 00Ah
        db  001h, 000h, 0C2h, 0E0h, 001h, 000h, 022h, 0E0h, 0E0h, 006h, 013h, 0F8h, 007h, 08Bh, 007h, 058h
        db  0C8h, 00Bh, 007h, 050h, 007h, 08Bh, 0C4h, 036h, 0C8h, 00Bh, 007h, 052h, 003h, 080h, 003h, 000h
        db  000h, 002h, 0C2h, 0E0h, 008h, 0CCh, 013h, 00Ch, 006h, 020h, 008h, 0CEh, 0C8h, 00Bh, 001h, 06Ch
        db  0C8h, 020h, 0FCh, 000h, 008h, 0CCh, 0CBh, 04Bh, 000h, 002h, 003h, 090h, 0FFh, 0FFh, 003h, 080h
        db  0C0h, 041h, 013h, 00Fh, 080h, 081h, 013h, 00Dh, 0A0h, 082h, 0C2h, 0E2h, 00Ch, 032h, 009h, 012h
        db  0C8h, 00Bh, 001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 0C3h, 020h, 0FCh, 002h, 011h, 007h, 0C8h, 002h
        db  0FCh, 000h, 004h, 0EDh, 000h, 002h, 004h, 0E0h, 001h, 06Ch, 003h, 080h, 0CBh, 042h, 000h, 002h
        db  0C8h, 002h, 001h, 06Ch, 0C0h, 08Bh, 003h, 080h, 003h, 000h, 000h, 002h, 007h, 083h, 080h, 000h
        db  0C2h, 060h, 009h, 02Ch, 0C1h, 009h, 002h, 024h, 0FFh, 0F8h, 008h, 0A9h, 002h, 001h, 000h, 000h
        db  002h, 002h, 000h, 000h, 002h, 00Bh, 000h, 000h, 002h, 00Ch, 000h, 000h, 002h, 007h, 000h, 000h
        db  0CBh, 02Ch, 00Ch, 032h, 00Ch, 032h, 013h, 006h, 005h, 08Bh, 005h, 0CCh, 088h, 00Bh, 004h, 046h
        db  01Bh, 027h, 010h, 0F6h, 0C2h, 009h, 0C2h, 08Bh, 006h, 008h, 013h, 00Ah, 005h, 08Bh, 005h, 0CCh
        db  088h, 00Bh, 004h, 046h, 01Bh, 01Dh, 0CBh, 02Ch, 00Ch, 032h, 00Ch, 032h, 016h, 0EDh, 010h, 0F4h
        db  0C0h, 082h, 013h, 014h, 0C8h, 002h, 001h, 06Ch, 0C8h, 00Ah, 0FCh, 000h, 0C8h, 00Ah, 001h, 06Ch
        db  004h, 0E0h, 0FCh, 000h, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C8h, 004h, 0FCh, 004h, 0C2h, 00Ah
        db  0A2h, 008h, 0CAh, 002h, 00Ch, 032h, 0C0h, 08Ah, 005h, 087h, 010h, 0D6h, 0C0h, 04Ah, 010h, 0EEh
        db  0CBh, 047h, 000h, 002h, 004h, 0E0h, 001h, 06Ch, 007h, 08Bh, 000h, 043h, 004h, 0E0h, 00Ch, 000h
        db  003h, 000h, 000h, 002h, 0C8h, 00Bh, 001h, 06Ch, 0C2h, 08Bh, 006h, 0A0h, 0C0h, 0F0h, 003h, 090h
        db  000h, 07Fh, 003h, 080h, 001h, 0A0h, 0FCh, 002h, 000h, 020h, 001h, 060h, 009h, 018h, 000h, 001h
        db  016h, 00Bh, 002h, 00Ah, 0FCh, 002h, 0A2h, 0A0h, 008h, 0DEh, 0CEh, 0A0h, 005h, 0EEh, 0C6h, 0A0h
        db  0FCh, 004h, 0C8h, 020h, 008h, 0DEh, 0FCh, 004h, 007h, 08Ah, 005h, 0F8h, 0C2h, 05Ah, 013h, 008h
        db  005h, 0CAh, 0C2h, 05Ah, 0C8h, 009h, 001h, 06Ch, 0C8h, 00Bh, 0FCh, 000h, 0C6h, 08Bh, 010h, 002h
        db  0CEh, 08Bh, 0C6h, 08Bh, 020h, 020h, 0E0h, 01Ah, 016h, 005h, 0E8h, 020h, 0E0h, 004h, 007h, 03Ah
        db  004h, 0E0h, 007h, 036h, 003h, 090h, 000h, 07Fh, 003h, 080h, 003h, 000h, 000h, 002h, 0C8h, 00Bh
        db  001h, 06Ch, 004h, 0CCh, 004h, 0E0h, 0FCh, 000h, 0C2h, 08Bh, 006h, 0A0h, 0C0h, 0FEh, 003h, 090h
        db  000h, 07Fh, 003h, 080h, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C8h, 020h, 0E1h, 08Eh, 0FCh, 004h
        db  0C0h, 041h, 016h, 00Fh, 0D8h, 020h, 0E2h, 002h, 001h, 083h, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh
        db  00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 0C8h, 00Ah
        db  001h, 08Ah, 004h, 05Bh, 0C8h, 00Ah, 001h, 06Ch, 0C3h, 020h, 0FCh, 000h, 004h, 0E0h, 0FCh, 000h
        db  002h, 08Ah, 000h, 043h, 013h, 0DFh, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C8h, 004h, 0FCh, 004h
        db  098h, 020h, 009h, 032h, 008h, 0CFh, 013h, 00Ah, 0C8h, 020h, 008h, 0CCh, 0FCh, 000h, 0C8h, 00Ah
        db  008h, 0CCh, 005h, 0A0h, 008h, 0CEh, 0C2h, 08Ch, 016h, 0E5h, 004h, 05Bh, 0C0h, 041h, 013h, 010h
        db  0A2h, 08Ah, 0CAh, 082h, 00Ch, 032h, 009h, 01Ah, 0C8h, 002h, 001h, 06Ch, 0C8h, 00Ah, 0FCh, 000h
        db  0C0h, 08Ah, 098h, 020h, 001h, 083h, 0E2h, 002h, 013h, 009h, 0C2h, 08Ch, 016h, 0D3h, 004h, 05Bh
        db  0C0h, 04Ah, 0C0h, 08Ah, 098h, 020h, 001h, 083h, 0E2h, 002h, 016h, 011h, 001h, 0E0h, 001h, 09Ch
        db  000h, 040h, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 0FCh, 001h, 060h, 00Ch, 002h, 001h, 000h
        db  013h, 0ECh, 0D8h, 020h, 008h, 0E1h, 001h, 083h, 007h, 0A0h, 00Ch, 002h, 080h, 000h, 0C8h, 00Ah
        db  001h, 08Ah, 0C8h, 00Ah, 008h, 0CAh, 010h, 0E1h, 004h, 0D8h, 0C2h, 057h, 016h, 003h, 0CDh, 0C8h
        db  0C5h, 0C8h, 004h, 05Bh, 005h, 0C7h, 0C2h, 057h, 0C6h, 048h, 0C5h, 0C8h, 004h, 05Bh, 0C8h, 008h
        db  001h, 06Ch, 0A2h, 008h, 0CAh, 020h, 0FCh, 000h, 00Ch, 032h, 009h, 018h, 010h, 002h, 0C8h, 008h
        db  001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 0C2h, 057h, 016h, 003h, 0CDh, 0C8h, 0C5h, 0C8h, 004h, 05Bh
        db  005h, 0C7h, 0C8h, 017h, 001h, 06Ch, 0C8h, 008h, 0FCh, 000h, 0C5h, 0C8h, 004h, 05Bh, 0C6h, 017h
        db  016h, 002h, 0C9h, 0C8h, 000h, 002h, 0C5h, 0C8h, 004h, 05Bh, 0C2h, 017h, 0C8h, 008h, 001h, 06Ch
        db  013h, 007h, 0C5h, 0E0h, 0FCh, 000h, 0A2h, 008h, 0C8h, 028h, 00Ch, 032h, 0FCh, 000h, 009h, 018h
        db  004h, 05Bh, 001h, 060h, 001h, 082h, 020h, 000h, 016h, 00Ah, 0C2h, 060h, 001h, 084h, 001h, 0A0h
        db  001h, 082h, 020h, 000h, 001h, 0E0h, 001h, 082h, 020h, 000h, 0C8h, 009h, 001h, 084h, 004h, 0C9h
        db  004h, 05Bh, 006h, 0A0h, 0C4h, 022h, 004h, 0D3h, 004h, 0E0h, 001h, 002h, 0E8h, 020h, 0E0h, 014h
        db  001h, 000h, 0C8h, 020h, 0E0h, 016h, 001h, 004h, 02Ch, 005h, 048h, 020h, 0E0h, 014h, 001h, 000h
        db  007h, 08Ch, 00Ah, 000h, 007h, 08Dh, 007h, 0D8h, 007h, 08Eh, 000h, 018h, 0CFh, 07Ch, 006h, 04Eh
        db  016h, 0FDh, 002h, 0E0h, 007h, 0D8h, 007h, 08Fh, 0FFh, 011h, 002h, 08Bh, 059h, 03Bh, 016h, 021h
        db  002h, 08Ah, 059h, 03Bh, 013h, 01Eh, 005h, 08Fh, 020h, 020h, 0E0h, 016h, 016h, 001h, 010h, 019h
        db  020h, 020h, 0E0h, 004h, 016h, 016h, 001h, 000h, 000h, 0BFh, 016h, 013h, 007h, 08Bh, 040h, 0C0h
        db  001h, 000h, 060h, 000h, 013h, 010h, 001h, 040h, 060h, 000h, 016h, 00Bh, 007h, 08Bh, 044h, 0C4h
        db  0C3h, 0A0h, 001h, 002h, 048h, 00Eh, 001h, 002h, 001h, 04Eh, 010h, 000h, 016h, 004h, 007h, 08Fh
        db  0FFh, 018h, 004h, 060h, 0C3h, 0F8h, 0C3h, 00Bh, 0C3h, 04Bh, 020h, 020h, 0E0h, 00Ah, 016h, 002h
        db  002h, 06Bh, 020h, 020h, 020h, 020h, 0E0h, 00Ch, 016h, 002h, 002h, 06Ch, 020h, 000h, 020h, 020h
        db  0E0h, 00Eh, 016h, 002h, 002h, 06Ch, 000h, 020h, 005h, 08Fh, 020h, 020h, 0E0h, 010h, 016h, 007h
        db  002h, 06Dh, 000h, 020h, 021h, 020h, 0E0h, 022h, 013h, 0E4h, 0C1h, 004h, 016h, 002h, 007h, 084h
        db  07Fh, 0FEh, 005h, 08Fh, 020h, 020h, 0E0h, 012h, 016h, 002h, 002h, 06Dh, 020h, 000h, 021h, 060h
        db  0E0h, 022h, 013h, 0D7h, 0C1h, 045h, 016h, 002h, 007h, 085h, 07Fh, 0FEh, 005h, 08Fh, 0D1h, 086h
        db  013h, 00Bh, 025h, 0A0h, 0E0h, 026h, 013h, 008h, 005h, 08Fh, 026h, 020h, 0E0h, 022h, 016h, 004h
        db  005h, 08Fh, 026h, 0A0h, 0E0h, 022h, 013h, 002h, 004h, 060h, 0C3h, 0F8h, 0D8h, 001h, 008h, 09Eh
        db  0D8h, 020h, 007h, 0DBh, 008h, 0B2h, 0D8h, 002h, 008h, 0A8h, 0D8h, 020h, 007h, 0DDh, 008h, 094h
        db  002h, 0E0h, 007h, 058h, 006h, 0A0h, 0C4h, 022h, 007h, 089h, 0E3h, 05Eh, 004h, 0E0h, 001h, 01Ah
        db  0C8h, 020h, 007h, 0E4h, 001h, 018h, 0C8h, 019h, 001h, 00Ch, 0C8h, 039h, 001h, 00Ah, 0C8h, 039h
        db  001h, 012h, 016h, 009h, 0C3h, 079h, 002h, 00Fh, 0E0h, 000h, 063h, 04Fh, 002h, 02Dh, 0A0h, 000h
        db  0C8h, 00Dh, 001h, 014h, 010h, 002h, 0C8h, 039h, 001h, 014h, 0C3h, 0F9h, 0C8h, 03Fh, 001h, 00Eh
        db  0C8h, 01Fh, 001h, 010h, 004h, 0E0h, 008h, 0C6h, 0C2h, 0B9h, 0D8h, 01Ah, 001h, 000h, 006h, 096h
        db  002h, 089h, 0E3h, 086h, 016h, 0E0h, 007h, 08Fh, 0FFh, 01Ch, 007h, 08Ch, 00Ah, 000h, 007h, 08Dh
        db  0E3h, 086h, 007h, 08Eh, 000h, 010h, 08Fh, 07Ch, 016h, 03Bh, 006h, 04Eh, 016h, 0FCh, 007h, 08Ch
        db  0E3h, 096h, 007h, 08Dh, 00Ah, 000h, 007h, 08Eh, 0E3h, 0A6h, 063h, 08Ch, 0CFh, 07Ch, 006h, 04Eh
        db  016h, 0FDh, 004h, 0E0h, 008h, 0E2h, 001h, 020h, 001h, 042h, 004h, 000h, 016h, 002h, 007h, 020h
        db  008h, 0E2h, 0C2h, 060h, 001h, 062h, 004h, 0E0h, 001h, 062h, 007h, 08Eh, 080h, 000h, 007h, 08Ch
        db  008h, 0E6h, 007h, 08Dh, 000h, 006h, 0DFh, 03Eh, 005h, 08Eh, 006h, 00Dh, 016h, 0FCh, 0D3h, 0FEh
        db  006h, 0CFh, 005h, 08Eh, 0D3h, 0FEh, 006h, 0CFh, 007h, 08Ch, 008h, 0E6h, 0C8h, 009h, 001h, 062h
        db  004h, 0C9h, 0A3h, 05Ch, 0E2h, 07Ch, 0A3h, 05Ch, 0E2h, 07Ch, 0A3h, 05Ch, 0E2h, 07Ch, 013h, 002h
        db  083h, 0CDh, 013h, 009h, 007h, 020h, 008h, 0E6h, 010h, 006h, 007h, 08Fh, 0FFh, 019h, 0A3h, 0CDh
        db  0C8h, 00Fh, 001h, 004h, 010h, 0FFh, 001h, 0A0h, 001h, 002h, 010h, 000h, 0C3h, 0E0h, 007h, 0EEh
        db  043h, 0E0h, 0E0h, 006h, 0D8h, 00Fh, 001h, 000h, 0C0h, 020h, 0E0h, 004h, 004h, 0E0h, 006h, 0FEh
        db  004h, 0D3h, 004h, 0E0h, 001h, 004h, 004h, 060h, 0C4h, 036h, 007h, 08Ch, 00Ah, 000h, 007h, 08Dh
        db  000h, 018h, 007h, 08Eh, 059h, 03Bh, 0CFh, 00Eh, 006h, 04Dh, 016h, 0FDh, 004h, 05Bh, 0C3h, 0D3h
        db  043h, 0C5h, 016h, 00Ch, 0C3h, 0E0h, 007h, 098h, 011h, 003h, 002h, 0E0h, 007h, 098h, 004h, 051h
        db  0C3h, 0E0h, 007h, 078h, 011h, 003h, 002h, 0E0h, 007h, 078h, 004h, 051h, 02Ch, 005h, 0A0h, 041h
        db  004h, 021h, 0E2h, 0C2h, 007h, 08Bh, 0C4h, 036h, 001h, 000h, 040h, 000h, 013h, 005h, 001h, 0C0h
        db  020h, 000h, 001h, 0DDh, 000h, 080h, 004h, 05Bh, 0C2h, 04Bh, 0D2h, 0A0h, 007h, 0EFh, 007h, 08Ch
        db  000h, 008h, 001h, 09Dh, 000h, 080h, 0C0h, 0BDh, 0C3h, 0A0h, 007h, 0EAh, 0C3h, 0E0h, 007h, 0ECh
        db  006h, 0A0h, 0C6h, 02Eh, 001h, 0C0h, 040h, 000h, 0D8h, 002h, 001h, 017h, 002h, 062h, 0FFh, 080h
        db  006h, 0A0h, 0C6h, 08Ah, 0C8h, 002h, 001h, 004h, 004h, 059h, 001h, 080h, 040h, 000h, 001h, 000h
        db  000h, 0FEh, 013h, 014h, 0C3h, 0C0h, 073h, 0CFh, 0D3h, 0EFh, 0E1h, 0C2h, 006h, 0CFh, 073h, 0CFh
        db  0C3h, 0AFh, 0EAh, 080h, 0C3h, 09Eh, 001h, 04Eh, 000h, 080h, 013h, 008h, 040h, 02Fh, 0E0h, 014h
        db  0A3h, 0CFh, 004h, 02Fh, 0E2h, 0F2h, 001h, 040h, 040h, 000h, 013h, 0B1h, 001h, 040h, 020h, 000h
        db  013h, 004h, 001h, 000h, 000h, 0FEh, 016h, 0E6h, 010h, 0AAh, 007h, 08Dh, 008h, 094h, 001h, 05Dh
        db  000h, 080h, 013h, 008h, 002h, 02Dh, 000h, 00Ah, 002h, 08Dh, 008h, 0C6h, 016h, 0F8h, 001h, 080h
        db  020h, 000h, 010h, 0EFh, 004h, 058h, 001h, 040h, 040h, 000h, 016h, 005h, 0E0h, 020h, 0E0h, 014h
        db  002h, 065h, 058h, 000h, 010h, 094h, 0D8h, 020h, 007h, 0DEh, 001h, 017h, 007h, 08Fh, 0FFh, 086h
        db  0C8h, 00Fh, 001h, 004h, 001h, 0C0h, 040h, 000h, 002h, 045h, 0A7h, 0FFh, 010h, 088h, 0C3h, 020h
        db  006h, 0FEh, 027h, 020h, 0E3h, 03Ah, 013h, 007h, 023h, 020h, 0E0h, 022h, 013h, 01Ah, 002h, 065h
        db  0DFh, 0FFh, 040h, 020h, 0E0h, 014h, 0E0h, 020h, 0E0h, 016h, 0C8h, 00Ch, 008h, 098h, 007h, 08Dh
        db  008h, 094h, 004h, 058h, 048h, 020h, 0E0h, 008h, 006h, 0FEh, 0C3h, 020h, 008h, 098h, 027h, 020h
        db  0E3h, 03Ah, 016h, 019h, 003h, 080h, 0C3h, 002h, 0C2h, 06Ch, 000h, 00Ah, 006h, 099h, 004h, 060h
        db  0C4h, 036h, 0D2h, 0A0h, 007h, 0EFh, 007h, 08Ch, 000h, 001h, 007h, 08Dh, 006h, 006h, 004h, 0CEh
        db  0C3h, 0E0h, 006h, 008h, 013h, 001h, 006h, 097h, 0D8h, 020h, 006h, 007h, 001h, 017h, 007h, 08Bh
        db  0FFh, 082h, 0C8h, 00Bh, 001h, 004h, 006h, 0A0h, 0CAh, 072h, 004h, 060h, 0C4h, 036h, 0C2h, 0A0h
        db  007h, 0EEh, 007h, 08Ch, 000h, 006h, 007h, 08Dh, 008h, 0A0h, 0C3h, 0A0h, 007h, 0E6h, 0C3h, 0E0h
        db  007h, 0E8h, 006h, 097h, 0D2h, 0A0h, 007h, 0EFh, 007h, 08Dh, 008h, 0A6h, 004h, 0DDh, 007h, 08Ch
        db  000h, 002h, 006h, 097h, 007h, 08Dh, 080h, 000h, 0C2h, 0A0h, 008h, 0A0h, 088h, 00Ah, 006h, 00Ch
        db  01Bh, 014h, 007h, 082h, 0C5h, 0FEh, 0C3h, 0A0h, 008h, 0A2h, 0C3h, 0E0h, 008h, 0A4h, 007h, 08Bh
        db  0E3h, 00Eh, 002h, 08Ah, 000h, 014h, 01Ah, 004h, 007h, 08Bh, 0EAh, 07Ch, 002h, 02Ah, 0FFh, 0ECh
        db  0A2h, 08Ah, 0A2h, 0CAh, 0C2h, 0DBh, 013h, 001h, 006h, 09Bh, 0C8h, 020h, 008h, 0A0h, 008h, 0A4h
        db  0C8h, 020h, 0E0h, 020h, 008h, 0A0h, 0C8h, 00Dh, 008h, 0A2h, 007h, 08Dh, 008h, 09Eh, 0E0h, 020h
        db  0E0h, 018h, 002h, 065h, 058h, 000h, 004h, 058h, 002h, 045h, 0A7h, 0FFh, 003h, 080h, 0C0h, 060h
        db  005h, 0EEh, 002h, 021h, 003h, 0E8h, 001h, 020h, 001h, 002h, 000h, 006h, 016h, 007h, 088h, 001h
        db  005h, 0EEh, 016h, 0F9h, 010h, 03Dh, 0D0h, 060h, 001h, 003h, 013h, 0F1h, 004h, 0E0h, 001h, 01Ah
        db  002h, 001h, 001h, 00Ah, 0CCh, 04Ch, 0CCh, 04Ch, 0CCh, 04Eh, 0CCh, 04Fh, 007h, 0B1h, 000h, 040h
        db  0CCh, 04Dh, 0D8h, 00Ah, 001h, 000h, 004h, 05Bh, 0C0h, 060h, 005h, 0EEh, 002h, 021h, 003h, 0E8h
        db  001h, 020h, 001h, 002h, 000h, 006h, 016h, 007h, 088h, 001h, 005h, 0EEh, 016h, 0F9h, 010h, 020h
        db  0D0h, 060h, 001h, 003h, 013h, 0F1h, 004h, 0E0h, 001h, 01Ah, 002h, 001h, 001h, 00Ah, 0CCh, 04Ch
        db  0CCh, 04Ch, 0CCh, 04Eh, 0CCh, 04Fh, 007h, 0B1h, 000h, 040h, 0CCh, 04Dh, 0D8h, 00Ah, 001h, 000h
        db  003h, 0B0h, 0D0h, 060h, 001h, 003h, 013h, 001h, 004h, 05Bh, 0C0h, 060h, 005h, 0EEh, 002h, 021h
        db  003h, 0E8h, 001h, 020h, 001h, 002h, 000h, 006h, 016h, 0F7h, 088h, 001h, 005h, 0EEh, 016h, 0F9h
        db  004h, 0CDh, 007h, 08Ah, 040h, 000h, 0C3h, 020h, 001h, 000h, 001h, 00Ch, 080h, 000h, 013h, 002h
        db  007h, 08Ah, 020h, 000h, 0C3h, 0A0h, 001h, 00Eh, 0C3h, 0E0h, 001h, 010h, 003h, 0B0h, 0C3h, 020h
        db  007h, 058h, 023h, 020h, 0E0h, 004h, 013h, 002h, 004h, 060h, 0C3h, 0F2h, 004h, 060h, 0B3h, 07Ch
        db  007h, 08Dh, 020h, 000h, 020h, 020h, 0E0h, 00Ah, 016h, 001h, 004h, 05Bh, 002h, 00Dh, 00Ch, 032h
        db  0C2h, 05Dh, 011h, 001h, 004h, 0DDh, 005h, 0CDh, 088h, 00Dh, 00Ch, 030h, 016h, 0F9h, 0C2h, 060h
        db  006h, 00Ah, 007h, 08Dh, 009h, 018h, 006h, 0A0h, 0CAh, 0BAh, 002h, 009h, 000h, 06Eh, 0C3h, 0E0h
        db  008h, 0E2h, 016h, 003h, 001h, 0E0h, 009h, 018h, 000h, 010h, 0C2h, 0E0h, 009h, 018h, 002h, 00Fh
        db  001h, 000h, 026h, 0C9h, 013h, 002h, 004h, 060h, 0C9h, 044h, 002h, 009h, 012h, 000h, 001h, 04Bh
        db  000h, 010h, 013h, 002h, 002h, 009h, 013h, 000h, 0D8h, 009h, 008h, 0E0h, 007h, 08Fh, 040h, 000h
        db  007h, 089h, 009h, 01Ah, 004h, 0CBh, 0E2h, 0F9h, 0E2h, 0F9h, 0E2h, 0F9h, 016h, 007h, 007h, 08Bh
        db  008h, 0E6h, 007h, 08Ch, 009h, 01Ah, 0CFh, 03Bh, 0CFh, 03Bh, 0C7h, 01Bh, 0C3h, 020h, 009h, 01Ah
        db  011h, 018h, 007h, 08Fh, 020h, 000h, 007h, 089h, 009h, 028h, 006h, 0A0h, 0C7h, 06Ch, 006h, 0A0h
        db  0C7h, 06Ch, 010h, 011h, 0C3h, 019h, 016h, 002h, 007h, 08Ch, 000h, 01Ah, 002h, 08Ch, 000h, 00Eh
        db  013h, 006h, 002h, 08Ch, 000h, 014h, 013h, 003h, 002h, 08Ch, 000h, 01Ah, 016h, 002h, 0CEh, 04Ch
        db  004h, 05Bh, 004h, 060h, 0C9h, 044h, 007h, 08Fh, 010h, 000h, 0C2h, 0D9h, 011h, 0FAh, 016h, 002h
        db  007h, 08Bh, 004h, 000h, 0C3h, 04Bh, 002h, 08Dh, 000h, 020h, 014h, 002h, 002h, 00Dh, 000h, 020h
        db  002h, 08Dh, 004h, 000h, 012h, 002h, 002h, 00Dh, 004h, 000h, 002h, 02Dh, 0FFh, 0F8h, 0C8h, 00Dh
        db  008h, 0DEh, 002h, 02Bh, 003h, 0FFh, 001h, 08Bh, 003h, 0FFh, 0CEh, 04Bh, 004h, 0CBh, 004h, 0E0h
        db  005h, 0F8h, 004h, 0E0h, 009h, 014h, 0C8h, 020h, 00Ch, 030h, 009h, 02Eh, 007h, 0A0h, 009h, 030h
        db  0DFh, 0FEh, 007h, 08Dh, 0DFh, 0FEh, 0C3h, 0E0h, 007h, 0D8h, 023h, 0E0h, 0E0h, 016h, 016h, 024h
        db  0C3h, 0E0h, 00Ch, 030h, 063h, 04Fh, 004h, 0FFh, 004h, 0FFh, 006h, 04Dh, 016h, 0FDh, 007h, 08Dh
        db  0DFh, 0FEh, 004h, 020h, 0EAh, 064h, 0C3h, 0A0h, 0EAh, 064h, 0C3h, 0EEh, 000h, 012h, 016h, 0C1h
        db  0C3h, 06Eh, 000h, 018h, 009h, 0ADh, 007h, 08Ch, 0E0h, 000h, 009h, 0ACh, 063h, 00Dh, 013h, 00Ch
        db  0C3h, 06Eh, 000h, 018h, 009h, 0ADh, 002h, 02Dh, 000h, 040h, 00Ah, 01Dh, 002h, 02Dh, 00Ch, 032h
        db  007h, 0BDh, 07Fh, 0FFh, 006h, 00Ch, 016h, 0FCh, 0C3h, 020h, 004h, 046h, 002h, 08Ch, 000h, 080h
        db  01Ah, 013h, 002h, 0ACh, 0C8h, 00Ch, 000h, 09Ah, 002h, 0E0h, 000h, 080h, 007h, 088h, 000h, 080h
        db  0C2h, 060h, 004h, 046h, 006h, 0A0h, 0F1h, 000h, 010h, 002h, 000h, 09Dh, 010h, 005h, 000h, 09Dh
        db  007h, 08Fh, 008h, 000h, 004h, 060h, 0C9h, 044h, 02Dh, 04Bh, 0C3h, 081h, 005h, 0C9h, 007h, 08Fh
        db  010h, 000h, 002h, 08Eh, 000h, 002h, 011h, 06Ah, 007h, 08Fh, 004h, 000h, 005h, 0C9h, 0C2h, 0D9h
        db  026h, 0E0h, 0E0h, 026h, 016h, 002h, 002h, 02Bh, 000h, 006h, 0CEh, 04Bh, 0C3h, 04Bh, 072h, 0CBh
        db  002h, 02Eh, 0FFh, 0FEh, 083h, 08Bh, 01Bh, 05Ah, 006h, 0CDh, 073h, 04Dh, 082h, 0CDh, 01Bh, 056h
        db  004h, 0E0h, 008h, 0CCh, 004h, 0E0h, 008h, 0CEh, 0C3h, 0B9h, 0C3h, 0D9h, 007h, 089h, 000h, 012h
        db  007h, 08Dh, 008h, 0ECh, 006h, 0A0h, 0CAh, 0BAh, 0C3h, 060h, 007h, 0D8h, 023h, 060h, 0E0h, 016h
        db  016h, 009h, 0E8h, 020h, 0E0h, 010h, 009h, 018h, 0E8h, 020h, 0E0h, 018h, 007h, 098h, 0E8h, 020h
        db  0E0h, 012h, 007h, 078h, 0C3h, 060h, 009h, 018h, 023h, 060h, 0E0h, 01Eh, 016h, 003h, 048h, 020h
        db  0E3h, 0A6h, 009h, 018h, 023h, 060h, 0E0h, 022h, 013h, 006h, 027h, 060h, 0E3h, 0A8h, 013h, 003h
        db  0E8h, 020h, 0E0h, 010h, 009h, 018h, 02Dh, 020h, 000h, 000h, 007h, 08Eh, 000h, 000h, 006h, 0A0h
        db  0CAh, 09Ah, 005h, 04Eh, 02Ch, 00Eh, 0C0h, 0A0h, 007h, 0FEh, 0C3h, 0EFh, 000h, 006h, 016h, 01Bh
        db  0C3h, 0A0h, 009h, 020h, 0C3h, 0E0h, 009h, 022h, 006h, 0A0h, 0C9h, 080h, 0C3h, 0A0h, 009h, 024h
        db  0C3h, 0E0h, 009h, 026h, 006h, 0A0h, 0C9h, 09Eh, 0E0h, 020h, 0E0h, 00Ah, 0C3h, 060h, 007h, 0D8h
        db  023h, 060h, 0E0h, 016h, 016h, 005h, 004h, 0E0h, 006h, 02Eh, 041h, 060h, 0E0h, 004h, 02Eh, 04Dh
        db  007h, 08Dh, 080h, 000h, 004h, 052h, 073h, 0CFh, 002h, 02Fh, 002h, 000h, 0C3h, 04Fh, 004h, 052h
        db  020h, 020h, 0E0h, 00Ah, 013h, 003h, 007h, 08Dh, 010h, 000h, 004h, 05Bh, 040h, 020h, 0E0h, 00Ah
        db  002h, 040h, 0F0h, 0FFh, 007h, 08Eh, 000h, 002h, 006h, 0A0h, 0CAh, 09Ah, 005h, 04Eh, 02Ch, 00Eh
        db  0C0h, 0A0h, 007h, 0FEh, 006h, 0A0h, 0CAh, 072h, 0C3h, 060h, 007h, 0D8h, 023h, 060h, 0E0h, 016h
        db  016h, 066h, 004h, 020h, 0EAh, 078h, 010h, 063h, 002h, 06Eh, 080h, 000h, 007h, 08Dh, 0C0h, 000h
        db  0C8h, 00Dh, 001h, 0A6h, 0C8h, 00Eh, 009h, 020h, 0C8h, 00Fh, 009h, 022h, 0C8h, 00Eh, 001h, 0A8h
        db  0C8h, 00Fh, 001h, 0AAh, 010h, 012h, 001h, 08Fh, 000h, 001h, 007h, 08Ah, 009h, 024h, 0E3h, 0A0h
        db  009h, 000h, 0CEh, 08Eh, 001h, 09Ah, 0FFh, 0FEh, 0E3h, 0E0h, 009h, 002h, 0E6h, 08Fh, 007h, 08Ah
        db  001h, 0ACh, 0CEh, 08Eh, 001h, 09Ah, 0FFh, 0FEh, 0E6h, 08Fh, 020h, 020h, 0E0h, 00Ah, 013h, 03Fh
        db  007h, 08Dh, 010h, 000h, 004h, 05Bh, 020h, 020h, 0E0h, 00Ah, 013h, 003h, 002h, 00Dh, 010h, 000h
        db  004h, 05Bh, 0C3h, 08Eh, 013h, 004h, 001h, 0E0h, 009h, 002h, 001h, 000h, 010h, 006h, 001h, 0A0h
        db  009h, 002h, 001h, 000h, 001h, 0A0h, 009h, 026h, 001h, 000h, 0C3h, 0A0h, 009h, 024h, 0C3h, 0E0h
        db  009h, 026h, 0E3h, 0A0h, 009h, 000h, 0E3h, 0E0h, 009h, 002h, 0C8h, 00Eh, 001h, 0ACh, 0C8h, 00Fh
        db  001h, 0AEh, 0C8h, 00Eh, 009h, 024h, 0C8h, 00Fh, 009h, 026h, 010h, 019h, 002h, 06Eh, 080h, 000h
        db  0C8h, 00Eh, 001h, 0A6h, 020h, 020h, 0E0h, 00Ah, 013h, 012h, 002h, 00Dh, 010h, 000h, 004h, 05Bh
        db  007h, 08Dh, 007h, 028h, 007h, 089h, 000h, 00Eh, 006h, 0A0h, 0CAh, 0C6h, 007h, 08Dh, 007h, 028h
        db  004h, 0FDh, 002h, 08Dh, 007h, 036h, 016h, 0FCh, 048h, 020h, 0E0h, 014h, 006h, 0FEh, 007h, 08Dh
        db  080h, 000h, 004h, 052h, 0C2h, 0A0h, 007h, 0EEh, 007h, 08Ch, 000h, 004h, 007h, 08Dh, 008h, 0A2h
        db  006h, 097h, 0C2h, 07Dh, 0C3h, 05Dh, 043h, 060h, 0E0h, 022h, 006h, 0A0h, 0CAh, 0C6h, 010h, 0EFh
        db  0C8h, 00Eh, 006h, 006h, 0C8h, 00Fh, 006h, 008h, 010h, 0EAh, 007h, 08Ch, 0E0h, 014h, 048h, 01Ch
        db  008h, 094h, 048h, 01Ch, 008h, 0A8h, 048h, 01Ch, 008h, 0B2h, 002h, 040h, 0C0h, 000h, 004h, 0E0h
        db  006h, 006h, 007h, 08Ch, 040h, 010h, 044h, 0CCh, 004h, 0E0h, 006h, 0FEh, 007h, 085h, 080h, 040h
        db  004h, 05Bh, 0C8h, 002h, 007h, 0FEh, 007h, 08Fh, 007h, 0F4h, 0CBh, 0CEh, 000h, 002h, 007h, 08Eh
        db  007h, 036h, 0CBh, 0CEh, 000h, 004h, 007h, 08Dh, 006h, 030h, 007h, 08Eh, 000h, 010h, 02Ch, 04Dh
        db  004h, 05Bh, 0C8h, 020h, 007h, 0E2h, 001h, 01Ah, 0C2h, 0A0h, 007h, 0F2h, 010h, 005h, 0C8h, 020h
        db  007h, 0E0h, 001h, 01Ah, 0D2h, 0A0h, 007h, 0F3h, 0C8h, 00Bh, 008h, 09Ch, 0C3h, 009h, 013h, 00Ah
        db  006h, 0A0h, 0C6h, 068h, 0C2h, 0A0h, 001h, 000h, 0E2h, 0A0h, 0E0h, 006h, 0A3h, 04Ch, 0A3h, 0CCh
        db  017h, 001h, 005h, 08Eh, 062h, 04Ch, 0C2h, 0E0h, 008h, 09Ch, 004h, 05Bh, 007h, 08Dh, 010h, 000h
        db  020h, 020h, 0E0h, 00Ah, 013h, 001h, 004h, 05Bh, 002h, 00Dh, 000h, 06Eh, 0C3h, 0E0h, 008h, 0E2h
        db  016h, 002h, 001h, 0CEh, 000h, 010h, 027h, 08Dh, 013h, 003h, 002h, 00Dh, 001h, 000h, 004h, 052h
        db  003h, 000h, 000h, 002h, 0C3h, 060h, 009h, 018h, 002h, 04Dh, 080h, 008h, 002h, 04Eh, 07Fh, 0F7h
        db  0E3h, 08Dh, 0C3h, 0E0h, 007h, 0D8h, 023h, 0E0h, 0E0h, 016h, 013h, 004h, 007h, 08Dh, 000h, 006h
        db  027h, 08Dh, 013h, 002h, 0E3h, 0A0h, 0E0h, 010h, 0C8h, 00Eh, 009h, 018h, 002h, 00Dh, 012h, 000h
        db  001h, 04Eh, 000h, 010h, 013h, 002h, 002h, 00Dh, 013h, 000h, 0D8h, 00Dh, 008h, 0E0h, 0C3h, 060h
        db  001h, 080h, 002h, 04Eh, 000h, 001h, 002h, 04Dh, 0FFh, 0FEh, 0E3h, 04Eh, 0C8h, 00Dh, 001h, 080h
        db  0D8h, 020h, 0E2h, 042h, 008h, 0E1h, 001h, 020h, 009h, 018h, 000h, 006h, 013h, 003h, 0D8h, 020h
        db  0E1h, 0D2h, 008h, 0E1h, 098h, 020h, 001h, 083h, 0E2h, 002h, 013h, 003h, 0D8h, 020h, 008h, 0E1h
        db  001h, 083h, 003h, 000h, 000h, 00Fh, 004h, 060h, 0CAh, 046h, 020h, 020h, 0E0h, 00Ah, 013h, 003h
        db  002h, 00Dh, 010h, 000h, 004h, 05Bh, 002h, 009h, 000h, 008h, 002h, 00Dh, 009h, 00Ah, 006h, 0A0h
        db  0CAh, 0BAh, 007h, 0A0h, 002h, 002h, 000h, 000h, 002h, 00Dh, 004h, 000h, 0C3h, 0E0h, 009h, 00Ah
        db  001h, 00Fh, 07Ch, 000h, 013h, 001h, 004h, 052h, 0C3h, 08Fh, 002h, 04Eh, 000h, 00Fh, 013h, 0FBh
        db  002h, 08Eh, 000h, 00Fh, 013h, 0F8h, 002h, 00Dh, 040h, 000h, 0C2h, 04Fh, 009h, 049h, 002h, 049h
        db  000h, 03Fh, 001h, 009h, 000h, 001h, 016h, 0EFh, 002h, 089h, 000h, 006h, 01Ah, 0ECh, 002h, 089h
        db  000h, 020h, 014h, 0E9h, 006h, 0C9h, 009h, 01Fh, 002h, 04Fh, 040h, 000h, 0E2h, 04Fh, 002h, 069h
        db  080h, 000h, 0C8h, 009h, 009h, 00Ah, 002h, 00Fh, 0FFh, 0FFh, 0C2h, 04Eh, 009h, 01Fh, 006h, 009h
        db  016h, 0FDh, 005h, 04Fh, 002h, 00Dh, 020h, 000h, 0C2h, 060h, 009h, 00Ch, 013h, 0D4h, 026h, 04Fh
        db  016h, 0D2h, 002h, 00Dh, 010h, 000h, 0C2h, 060h, 009h, 00Eh, 013h, 0CDh, 026h, 04Fh, 016h, 0CBh
        db  002h, 00Dh, 030h, 000h, 088h, 020h, 009h, 00Ch, 009h, 00Eh, 013h, 0C5h, 0C3h, 0E0h, 009h, 00Ch
        db  0C2h, 04Eh, 00Ah, 01Fh, 006h, 009h, 016h, 0FDh, 0E3h, 0E0h, 009h, 010h, 0C8h, 00Fh, 009h, 00Ch
        db  0C3h, 0E0h, 009h, 00Eh, 0C2h, 04Eh, 00Ah, 01Fh, 006h, 009h, 016h, 0FDh, 0E3h, 0E0h, 009h, 010h
        db  0C8h, 00Fh, 009h, 00Eh, 002h, 00Fh, 0FFh, 0FFh, 0C2h, 04Eh, 00Ah, 01Fh, 006h, 009h, 016h, 0FDh
        db  002h, 00Dh, 008h, 000h, 0C2h, 060h, 009h, 010h, 026h, 04Fh, 016h, 0A5h, 005h, 04Fh, 0C8h, 00Fh
        db  009h, 010h, 002h, 00Fh, 002h, 002h, 002h, 00Eh, 000h, 003h, 0C3h, 060h, 001h, 040h, 002h, 00Ch
        db  0CCh, 0C8h, 001h, 0A0h, 001h, 040h, 004h, 000h, 005h, 0CFh, 002h, 009h, 055h, 055h, 006h, 09Ch
        db  005h, 049h, 006h, 09Ch, 007h, 009h, 006h, 09Ch, 005h, 049h, 006h, 09Ch, 006h, 00Eh, 016h, 0F4h
        db  001h, 0A0h, 001h, 040h, 040h, 000h, 0C8h, 00Dh, 001h, 040h, 002h, 009h, 000h, 008h, 002h, 00Eh
        db  009h, 00Ah, 002h, 00Fh, 002h, 002h, 0CFh, 0FEh, 006h, 049h, 016h, 0FDh, 004h, 060h, 0CAh, 046h
        db  0C7h, 0C9h, 082h, 05Fh, 016h, 001h, 004h, 05Bh, 001h, 0A0h, 001h, 040h, 040h, 000h, 0C8h, 00Dh
        db  001h, 040h, 002h, 00Dh, 001h, 000h, 004h, 052h, 007h, 08Dh, 010h, 000h, 020h, 020h, 0E0h, 00Ah
        db  016h, 00Ah, 007h, 08Dh, 008h, 000h, 020h, 020h, 0E0h, 010h, 013h, 005h, 007h, 08Dh, 040h, 000h
        db  001h, 04Fh, 000h, 001h, 016h, 001h, 004h, 05Bh, 0E0h, 020h, 0E0h, 010h, 007h, 08Dh, 007h, 036h
        db  007h, 089h, 008h, 04Ah, 0CAh, 04Dh, 000h, 002h, 006h, 0A0h, 0CEh, 0E0h, 007h, 089h, 008h, 062h
        db  0CAh, 04Dh, 000h, 002h, 006h, 0A0h, 0CEh, 0E0h, 007h, 089h, 008h, 07Ah, 0CAh, 04Dh, 000h, 002h
        db  006h, 0A0h, 0CEh, 0E0h, 004h, 0E0h, 008h, 0B8h, 004h, 0E0h, 008h, 0BAh, 0C8h, 00Eh, 008h, 022h
        db  0C8h, 00Fh, 008h, 024h, 0C8h, 00Eh, 008h, 040h, 0C8h, 00Fh, 008h, 042h, 004h, 0E0h, 008h, 030h
        db  004h, 0E0h, 008h, 036h, 002h, 0E0h, 007h, 078h, 004h, 0E0h, 008h, 046h, 004h, 0E0h, 008h, 048h
        db  040h, 020h, 0E3h, 042h, 0E0h, 020h, 0E0h, 00Ch, 004h, 060h, 0D2h, 032h, 001h, 080h, 0F0h, 000h
        db  010h, 00Dh, 001h, 0C0h, 0F0h, 000h, 040h, 020h, 0E0h, 006h, 010h, 008h, 001h, 0C0h, 0F0h, 000h
        db  001h, 080h, 020h, 000h, 0C3h, 0E0h, 008h, 046h, 016h, 001h, 004h, 05Bh, 0C0h, 04Bh, 004h, 020h
        db  0EAh, 07Ch, 001h, 040h, 020h, 000h, 016h, 0FBh, 004h, 051h, 0C2h, 0A0h, 007h, 0D8h, 001h, 04Ah
        db  000h, 040h, 016h, 001h, 004h, 05Bh, 002h, 0E0h, 007h, 078h, 020h, 020h, 0E0h, 00Ch, 016h, 0EFh
        db  0C2h, 043h, 013h, 002h, 006h, 0A0h, 0CEh, 0CEh, 02Fh, 020h, 007h, 036h, 040h, 020h, 0E0h, 00Ch
        db  006h, 0A0h, 0CDh, 074h, 0C3h, 0A0h, 008h, 046h, 016h, 0FBh, 006h, 0A0h, 0CEh, 008h, 007h, 08Eh
        db  008h, 0B6h, 007h, 09Eh, 080h, 000h, 020h, 020h, 0E0h, 010h, 016h, 005h, 007h, 08Fh, 008h, 022h
        db  0CFh, 0BFh, 0CFh, 0BFh, 0C7h, 09Fh, 006h, 0A0h, 0CEh, 02Ch, 003h, 0B0h, 0E8h, 020h, 0E3h, 03Eh
        db  007h, 062h, 006h, 0A0h, 0CEh, 008h, 003h, 0B0h, 048h, 020h, 0E3h, 03Eh, 007h, 062h, 040h, 020h
        db  0E3h, 042h, 0E0h, 020h, 0E0h, 004h, 003h, 0B0h, 048h, 020h, 0E0h, 010h, 007h, 058h, 004h, 05Bh
        db  001h, 080h, 0F0h, 000h, 0E0h, 020h, 0E0h, 004h, 0C3h, 0E0h, 008h, 0B2h, 023h, 0E0h, 0E0h, 014h
        db  013h, 002h, 006h, 09Bh, 010h, 0B4h, 003h, 0B0h, 0E8h, 020h, 0E0h, 01Eh, 007h, 058h, 010h, 0AFh
        db  006h, 09Bh, 003h, 080h, 002h, 0E0h, 007h, 058h, 007h, 08Dh, 008h, 0B2h, 006h, 0A0h, 0C4h, 060h
        db  002h, 0E0h, 007h, 078h, 004h, 05Bh, 0C2h, 060h, 007h, 05Ch, 006h, 020h, 008h, 046h, 0C2h, 0A9h
        db  000h, 008h, 006h, 0A0h, 0CEh, 0CEh, 001h, 069h, 000h, 00Ah, 000h, 001h, 013h, 036h, 0C3h, 049h
        db  002h, 02Dh, 000h, 00Eh, 0C3h, 00Ah, 0D3h, 01Dh, 001h, 08Ch, 084h, 000h, 001h, 0CCh, 040h, 000h
        db  001h, 00Ah, 05Eh, 000h, 013h, 008h, 002h, 00Eh, 023h, 000h, 0DBh, 04Eh, 000h, 001h, 001h, 0CCh
        db  004h, 000h, 0D7h, 04Ch, 010h, 001h, 0C7h, 04Ch, 0C3h, 0A9h, 000h, 010h, 0C3h, 0E9h, 000h, 012h
        db  0CAh, 041h, 000h, 010h, 002h, 02Fh, 000h, 004h, 017h, 001h, 005h, 08Eh, 007h, 08Ch, 000h, 002h
        db  0D2h, 0A0h, 007h, 0F1h, 006h, 0A0h, 0C6h, 02Eh, 0C0h, 069h, 000h, 010h, 0C8h, 029h, 000h, 014h
        db  008h, 0B8h, 0C8h, 029h, 000h, 016h, 008h, 0BAh, 007h, 0A0h, 008h, 0B6h, 040h, 000h, 001h, 069h
        db  000h, 00Eh, 008h, 000h, 016h, 002h, 006h, 0A0h, 0CEh, 02Ch, 001h, 040h, 040h, 000h, 016h, 001h
        db  004h, 051h, 004h, 060h, 0CDh, 086h, 0C3h, 0A9h, 000h, 00Ch, 013h, 006h, 068h, 00Eh, 008h, 048h
        db  0C3h, 0A9h, 000h, 006h, 013h, 001h, 02Eh, 01Eh, 004h, 0E9h, 000h, 006h, 004h, 0E9h, 000h, 00Ch
        db  007h, 029h, 000h, 004h, 004h, 05Bh, 007h, 081h, 020h, 020h, 007h, 089h, 008h, 022h, 0CEh, 041h
        db  0CEh, 063h, 000h, 010h, 0C6h, 063h, 000h, 012h, 006h, 0A0h, 0C6h, 08Ah, 0C2h, 043h, 013h, 002h
        db  006h, 0A0h, 0CEh, 0CEh, 0E0h, 020h, 0E0h, 010h, 004h, 060h, 0CDh, 0B4h, 006h, 0A0h, 0C6h, 08Ah
        db  0C3h, 0A0h, 008h, 022h, 0C3h, 0E0h, 008h, 024h, 001h, 04Fh, 000h, 001h, 016h, 01Dh, 001h, 040h
        db  004h, 000h, 016h, 0E1h, 006h, 0A0h, 0CDh, 074h, 0C3h, 0A0h, 008h, 046h, 016h, 0FBh, 0C2h, 0A0h
        db  007h, 0F0h, 007h, 08Ch, 000h, 004h, 007h, 08Dh, 008h, 022h, 0C3h, 0A0h, 008h, 040h, 0C3h, 0E0h
        db  008h, 042h, 006h, 0A0h, 0C6h, 068h, 0C3h, 0A0h, 008h, 022h, 0C3h, 0E0h, 008h, 024h, 001h, 04Fh
        db  000h, 001h, 016h, 002h, 004h, 060h, 0CDh, 0B4h, 0C2h, 0A0h, 007h, 0F0h, 0C3h, 020h, 009h, 02Ah
        db  007h, 08Dh, 008h, 022h, 006h, 0A0h, 0C6h, 068h, 006h, 0C0h, 0D0h, 020h, 008h, 026h, 006h, 0C0h
        db  001h, 040h, 004h, 000h, 016h, 00Ah, 001h, 040h, 000h, 080h, 013h, 007h, 0C8h, 00Eh, 008h, 022h
        db  0C8h, 00Fh, 008h, 024h, 006h, 0A0h, 0CDh, 06Ah, 010h, 0DEh, 0C8h, 00Eh, 008h, 040h, 0C8h, 00Fh
        db  008h, 042h, 001h, 040h, 004h, 000h, 013h, 006h, 001h, 040h, 000h, 020h, 016h, 030h, 007h, 081h
        db  020h, 010h, 010h, 0A3h, 007h, 083h, 008h, 04Ah, 007h, 063h, 000h, 004h, 011h, 00Eh, 007h, 083h
        db  008h, 062h, 007h, 063h, 000h, 004h, 011h, 009h, 007h, 083h, 008h, 07Ah, 007h, 063h, 000h, 004h
        db  011h, 004h, 060h, 0C3h, 006h, 0A0h, 0CDh, 074h, 010h, 0EDh, 0C8h, 0E0h, 008h, 026h, 000h, 00Eh
        db  0C8h, 0E0h, 008h, 040h, 000h, 010h, 0C8h, 0E0h, 008h, 042h, 000h, 012h, 004h, 0C7h, 001h, 040h
        db  000h, 020h, 016h, 0DDh, 0C8h, 020h, 008h, 028h, 008h, 044h, 016h, 009h, 007h, 081h, 020h, 080h
        db  0C8h, 0E0h, 008h, 040h, 000h, 014h, 0C8h, 0E0h, 008h, 042h, 000h, 016h, 010h, 0D2h, 0C8h, 0E0h
        db  008h, 040h, 000h, 014h, 0C8h, 0E0h, 008h, 042h, 000h, 016h, 001h, 080h, 004h, 000h, 007h, 082h
        db  008h, 02Ah, 0C1h, 032h, 011h, 008h, 0C1h, 072h, 0C1h, 092h, 007h, 082h, 008h, 03Ch, 0C1h, 004h
        db  016h, 007h, 004h, 060h, 0D0h, 0E0h, 0C1h, 072h, 0C1h, 0B2h, 001h, 084h, 080h, 000h, 013h, 0F9h
        db  068h, 004h, 008h, 044h, 0C1h, 0C7h, 016h, 023h, 098h, 020h, 008h, 049h, 009h, 033h, 016h, 00Ah
        db  007h, 081h, 020h, 040h, 0C1h, 0E0h, 008h, 046h, 013h, 065h, 006h, 0A0h, 0CDh, 074h, 010h, 0F4h
        db  006h, 0A0h, 0CDh, 064h, 02Dh, 081h, 0C2h, 001h, 013h, 0FBh, 005h, 0A0h, 008h, 048h, 0C1h, 0C3h
        db  002h, 027h, 000h, 006h, 006h, 0A0h, 0C1h, 0A6h, 005h, 0A3h, 000h, 00Ch, 0C8h, 008h, 001h, 06Ch
        db  0C8h, 008h, 008h, 092h, 002h, 008h, 0FCh, 004h, 0C1h, 0F8h, 0C1h, 004h, 013h, 059h, 0D2h, 0E0h
        db  001h, 003h, 016h, 00Bh, 0C2h, 0E0h, 007h, 03Ah, 042h, 0E0h, 007h, 062h, 026h, 0E0h, 0E3h, 03Ch
        db  013h, 002h, 006h, 0A0h, 0CDh, 064h, 006h, 0A0h, 0C6h, 08Ah, 0C8h, 020h, 007h, 0E2h, 001h, 01Ah
        db  002h, 00Bh, 001h, 00Ah, 0CEh, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h, 0CEh, 0C6h, 004h, 0FBh, 002h, 009h
        db  004h, 000h, 0A2h, 048h, 0C6h, 0C9h, 0A8h, 020h, 008h, 092h, 001h, 012h, 0D8h, 020h, 007h, 0F2h
        db  001h, 000h, 0C2h, 047h, 081h, 0C4h, 014h, 001h, 0C2h, 044h, 061h, 0C9h, 0A2h, 009h, 0A1h, 089h
        db  017h, 001h, 005h, 085h, 061h, 009h, 016h, 0AEh, 002h, 082h, 008h, 03Ch, 016h, 005h, 001h, 040h
        db  000h, 010h, 013h, 026h, 004h, 060h, 0CFh, 014h, 004h, 060h, 0D0h, 01Ah, 006h, 0C1h, 0D8h, 0C1h
        db  000h, 00Fh, 006h, 0A0h, 0CDh, 074h, 0E0h, 020h, 0E0h, 006h, 0C2h, 043h, 007h, 081h, 0D2h, 032h
        db  004h, 060h, 0CEh, 046h, 020h, 020h, 0E0h, 012h, 016h, 005h, 007h, 081h, 000h, 044h, 020h, 020h
        db  0E0h, 012h, 013h, 0ECh, 004h, 060h, 0CEh, 0F2h, 007h, 081h, 020h, 080h, 010h, 0F8h, 007h, 081h
        db  020h, 080h, 010h, 0F0h, 007h, 081h, 020h, 002h, 010h, 0F2h, 007h, 081h, 020h, 004h, 010h, 0EFh
        db  0C8h, 023h, 000h, 008h, 001h, 06Ch, 005h, 007h, 0A1h, 0E0h, 000h, 0E8h, 002h, 00Ch, 0FCh, 002h
        db  007h, 0BCh, 081h, 000h, 0CFh, 007h, 0D2h, 0E0h, 001h, 003h, 016h, 00Bh, 0C2h, 0E0h, 007h, 03Ah
        db  042h, 0E0h, 007h, 062h, 026h, 0E0h, 0E3h, 03Ch, 013h, 002h, 006h, 0A0h, 0CDh, 064h, 006h, 0A0h
        db  0C6h, 08Ah, 0C8h, 023h, 000h, 006h, 001h, 06Ch, 0C2h, 0A3h, 000h, 00Eh, 001h, 04Ah, 001h, 000h
        db  013h, 00Bh, 002h, 00Ch, 0FCh, 00Eh, 0C2h, 05Ch, 002h, 049h, 080h, 000h, 002h, 00Dh, 009h, 01Ah
        db  0E2h, 07Dh, 0CFh, 009h, 0CFh, 03Dh, 0CFh, 03Dh, 002h, 00Ch, 0FCh, 000h, 0C3h, 06Ch, 000h, 006h
        db  002h, 04Dh, 0E0h, 0FFh, 002h, 04Ah, 002h, 000h, 0A2h, 08Ah, 0A2h, 08Ah, 0E3h, 04Ah, 0CBh, 00Dh
        db  000h, 006h, 006h, 0CDh, 0C2h, 0E0h, 008h, 044h, 016h, 0B7h, 002h, 00Bh, 000h, 00Fh, 082h, 0ECh
        db  000h, 004h, 011h, 0B2h, 0C3h, 0ECh, 000h, 00Eh, 015h, 011h, 013h, 010h, 0C2h, 06Ch, 000h, 014h
        db  002h, 049h, 01Fh, 000h, 013h, 0ACh, 006h, 0C9h, 002h, 089h, 000h, 012h, 01Bh, 0A8h, 001h, 049h
        db  000h, 001h, 013h, 0A5h, 0A2h, 0C9h, 082h, 0ECh, 000h, 004h, 011h, 0A1h, 0A3h, 04Dh, 018h, 0A2h
        db  011h, 014h, 001h, 060h, 009h, 018h, 080h, 000h, 013h, 018h, 009h, 01Dh, 0A2h, 0CCh, 0C2h, 0EBh
        db  000h, 008h, 009h, 07Bh, 002h, 04Bh, 000h, 01Eh, 0C3h, 0A0h, 006h, 0F0h, 023h, 0ABh, 0E0h, 004h
        db  016h, 094h, 027h, 060h, 0E3h, 040h, 016h, 091h, 0A3h, 04Dh, 0A3h, 04Dh, 0A3h, 04Dh, 006h, 0CDh
        db  002h, 04Dh, 000h, 007h, 088h, 00Dh, 006h, 0EEh, 015h, 00Dh, 007h, 0A3h, 000h, 00Ah, 002h, 004h
        db  003h, 090h, 040h, 07Fh, 02Fh, 053h, 005h, 0A0h, 008h, 046h, 004h, 0C3h, 001h, 0C0h, 004h, 000h
        db  004h, 060h, 0CFh, 018h, 001h, 060h, 009h, 018h, 080h, 000h, 013h, 0EFh, 002h, 001h, 020h, 008h
        db  004h, 060h, 0D1h, 016h, 007h, 08Dh, 010h, 000h, 020h, 020h, 0E0h, 00Ah, 016h, 00Ah, 007h, 08Dh
        db  008h, 000h, 020h, 020h, 0E0h, 00Eh, 013h, 005h, 007h, 08Dh, 040h, 000h, 001h, 04Fh, 000h, 001h
        db  016h, 001h, 004h, 05Bh, 0E0h, 020h, 0E0h, 00Eh, 007h, 0A0h, 008h, 0ACh, 080h, 000h, 0C8h, 00Eh
        db  007h, 0F4h, 0C8h, 00Fh, 007h, 0F6h, 0C8h, 00Eh, 008h, 014h, 0C8h, 00Fh, 008h, 016h, 004h, 0E0h
        db  008h, 002h, 004h, 0E0h, 008h, 008h, 002h, 0E0h, 007h, 098h, 040h, 020h, 0E3h, 04Eh, 007h, 020h
        db  006h, 02Eh, 004h, 060h, 0D6h, 008h, 070h, 000h, 0C0h, 04Bh, 004h, 0E0h, 006h, 02Eh, 010h, 00Bh
        db  0F0h, 020h, 0E3h, 04Dh, 010h, 002h, 0F0h, 020h, 0E3h, 04Ch, 0C0h, 04Bh, 004h, 0E0h, 006h, 02Eh
        db  001h, 0E0h, 007h, 062h, 000h, 040h, 0E8h, 020h, 0E3h, 048h, 007h, 062h, 004h, 020h, 0EAh, 07Ch
        db  001h, 040h, 020h, 000h, 013h, 004h, 010h, 0FAh, 001h, 040h, 040h, 000h, 016h, 0F7h, 007h, 020h
        db  006h, 02Eh, 050h, 020h, 0E3h, 052h, 004h, 051h, 024h, 020h, 0E3h, 054h, 013h, 0EFh, 080h, 0E0h
        db  008h, 020h, 013h, 003h, 0C2h, 0E0h, 008h, 020h, 02Eh, 01Bh, 0C3h, 003h, 0C8h, 00Ch, 001h, 06Ch
        db  0C2h, 0E0h, 0FCh, 002h, 001h, 04Bh, 001h, 000h, 013h, 003h, 0C3h, 020h, 0FCh, 000h, 010h, 0F6h
        db  080h, 0CCh, 013h, 003h, 0C8h, 020h, 0FCh, 000h, 005h, 0F8h, 004h, 0E0h, 0FCh, 000h, 024h, 020h
        db  0E3h, 050h, 013h, 00Bh, 007h, 08Bh, 000h, 001h, 0C3h, 020h, 0EAh, 060h, 0CBh, 00Bh, 000h, 018h
        db  0CBh, 003h, 000h, 014h, 004h, 020h, 0EAh, 060h, 010h, 001h, 02Eh, 013h, 040h, 020h, 0E3h, 054h
        db  007h, 020h, 006h, 02Eh, 004h, 060h, 0D6h, 008h, 010h, 0C1h, 002h, 0E0h, 007h, 058h, 007h, 08Dh
        db  008h, 0A8h, 006h, 0A0h, 0C4h, 060h, 002h, 0E0h, 007h, 098h, 004h, 05Bh, 002h, 0E0h, 007h, 058h
        db  020h, 020h, 0E0h, 006h, 016h, 010h, 0E0h, 020h, 0E0h, 01Ch, 002h, 0E0h, 007h, 098h, 001h, 0C0h
        db  080h, 000h, 010h, 0A4h, 0C1h, 020h, 008h, 0A8h, 021h, 020h, 0E0h, 014h, 016h, 006h, 0E8h, 020h
        db  0E0h, 01Ch, 007h, 058h, 003h, 080h, 002h, 0E0h, 007h, 098h, 0D8h, 020h, 007h, 0DCh, 001h, 017h
        db  007h, 08Fh, 0FFh, 08Eh, 0C8h, 00Fh, 001h, 004h, 0E8h, 020h, 0E0h, 006h, 007h, 058h, 001h, 080h
        db  080h, 000h, 004h, 05Bh, 0C2h, 0E0h, 008h, 020h, 082h, 0C3h, 013h, 003h, 02Dh, 0DBh, 0C8h, 003h
        db  008h, 020h, 0C8h, 003h, 001h, 06Ch, 0C2h, 060h, 0FCh, 002h, 001h, 049h, 001h, 000h, 016h, 002h
        db  004h, 060h, 0D5h, 04Ch, 0C0h, 0E0h, 005h, 0F8h, 013h, 0FDh, 003h, 000h, 000h, 002h, 0C0h, 0E0h
        db  005h, 0F8h, 0C8h, 003h, 001h, 06Ch, 0C8h, 020h, 0FCh, 000h, 005h, 0F8h, 003h, 000h, 000h, 00Fh
        db  0C8h, 003h, 001h, 06Ch, 007h, 088h, 0FCh, 002h, 0C2h, 078h, 0C1h, 0F8h, 001h, 049h, 001h, 000h
        db  016h, 04Dh, 001h, 009h, 05Eh, 000h, 016h, 029h, 001h, 049h, 000h, 002h, 016h, 00Bh, 001h, 060h
        db  008h, 012h, 002h, 000h, 016h, 00Ah, 002h, 027h, 000h, 004h, 088h, 007h, 009h, 02Ch, 012h, 005h
        db  002h, 027h, 0FFh, 0FCh, 001h, 0A0h, 008h, 012h, 002h, 000h, 0C1h, 0C7h, 015h, 037h, 02Dh, 0D3h
        db  0C0h, 0E0h, 008h, 020h, 0A8h, 007h, 008h, 01Eh, 0A8h, 007h, 008h, 01Ch, 015h, 00Ch, 0C8h, 020h
        db  008h, 014h, 007h, 0F4h, 0C8h, 020h, 008h, 016h, 007h, 0F6h, 0C8h, 020h, 008h, 018h, 008h, 014h
        db  0C8h, 020h, 008h, 01Ah, 008h, 016h, 004h, 060h, 0D5h, 04Ch, 006h, 0A0h, 0C6h, 08Ah, 02Dh, 0D3h
        db  0C2h, 0E0h, 008h, 020h, 082h, 0C3h, 013h, 001h, 02Dh, 0DBh, 088h, 020h, 008h, 016h, 008h, 010h
        db  016h, 00Dh, 088h, 020h, 008h, 014h, 008h, 00Eh, 016h, 009h, 004h, 0E0h, 008h, 01Ch, 007h, 082h
        db  007h, 0FCh, 061h, 004h, 004h, 0E0h, 008h, 01Eh, 004h, 060h, 0D6h, 010h, 0C8h, 020h, 008h, 00Eh
        db  007h, 0F4h, 0C8h, 020h, 008h, 010h, 007h, 0F6h, 004h, 060h, 0D6h, 008h, 0A8h, 007h, 008h, 01Eh
        db  0C1h, 004h, 016h, 019h, 002h, 082h, 008h, 00Eh, 016h, 008h, 001h, 060h, 007h, 0F6h, 000h, 001h
        db  016h, 002h, 006h, 0A0h, 0D7h, 07Ah, 006h, 0A0h, 0D6h, 0FAh, 0C1h, 032h, 011h, 007h, 0C1h, 072h
        db  0C1h, 092h, 007h, 082h, 008h, 00Eh, 0C1h, 004h, 016h, 006h, 010h, 0ECh, 0C1h, 072h, 0C1h, 0B2h
        db  001h, 084h, 080h, 000h, 013h, 0E7h, 0D2h, 0E0h, 001h, 003h, 016h, 00Ah, 001h, 020h, 007h, 03Ah
        db  070h, 000h, 013h, 004h, 006h, 0A0h, 0D2h, 09Eh, 007h, 020h, 006h, 02Eh, 006h, 0A0h, 0C6h, 08Ah
        db  0C8h, 020h, 007h, 0E0h, 001h, 01Ah, 002h, 00Bh, 001h, 00Ah, 0CEh, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h
        db  0CEh, 0C6h, 004h, 0FBh, 002h, 009h, 004h, 000h, 0A2h, 048h, 0C6h, 0C9h, 0A8h, 003h, 001h, 012h
        db  0D8h, 020h, 007h, 0F3h, 001h, 000h, 0C2h, 047h, 081h, 0C4h, 014h, 001h, 0C2h, 044h, 0A8h, 009h
        db  008h, 01Ch, 061h, 009h, 0A2h, 009h, 0A1h, 089h, 017h, 001h, 005h, 085h, 061h, 0C9h, 016h, 0B8h
        db  001h, 040h, 000h, 040h, 016h, 015h, 007h, 087h, 000h, 020h, 061h, 0E0h, 008h, 01Ch, 081h, 0C4h
        db  01Ah, 008h, 0A8h, 007h, 008h, 01Eh, 0A8h, 007h, 008h, 01Ch, 061h, 007h, 0A1h, 087h, 017h, 001h
        db  005h, 085h, 001h, 080h, 000h, 040h, 0C8h, 003h, 001h, 06Ch, 0C1h, 0E0h, 0FCh, 004h, 010h, 09Eh
        db  004h, 060h, 0D3h, 09Ch, 0D2h, 0E0h, 001h, 003h, 016h, 00Ah, 001h, 020h, 007h, 03Ah, 070h, 000h
        db  013h, 004h, 006h, 0A0h, 0D2h, 09Eh, 007h, 020h, 006h, 02Eh, 006h, 0A0h, 0C6h, 08Ah, 003h, 090h
        db  040h, 03Fh, 02Dh, 0D3h, 001h, 060h, 007h, 0F6h, 000h, 001h, 016h, 002h, 006h, 0A0h, 0D7h, 07Ah
        db  0D2h, 060h, 008h, 012h, 001h, 089h, 0F1h, 000h, 001h, 0C9h, 070h, 000h, 088h, 020h, 008h, 016h
        db  008h, 010h, 016h, 004h, 088h, 020h, 008h, 014h, 008h, 00Eh, 013h, 014h, 001h, 089h, 010h, 000h
        db  007h, 08Dh, 008h, 01Ch, 007h, 09Dh, 050h, 000h, 0D2h, 0A0h, 007h, 0F1h, 007h, 08Ch, 000h, 002h
        db  0C3h, 0A0h, 008h, 014h, 0C3h, 0E0h, 008h, 016h, 002h, 02Fh, 000h, 004h, 017h, 001h, 005h, 08Eh
        db  006h, 0A0h, 0C6h, 02Eh, 007h, 08Dh, 007h, 0F8h, 0CFh, 049h, 0C7h, 060h, 008h, 01Eh, 006h, 04Dh
        db  0D2h, 0A0h, 007h, 0F1h, 007h, 08Ch, 000h, 004h, 0C3h, 0A0h, 008h, 00Eh, 0C3h, 0E0h, 008h, 010h
        db  0A3h, 0CCh, 017h, 001h, 005h, 08Eh, 006h, 0A0h, 0C6h, 02Eh, 0C8h, 020h, 008h, 014h, 008h, 0AEh
        db  0C8h, 020h, 008h, 016h, 008h, 0B0h, 001h, 009h, 00Ch, 000h, 013h, 00Ah, 001h, 049h, 004h, 000h
        db  016h, 005h, 006h, 0A0h, 0D3h, 042h, 006h, 0A0h, 0D2h, 0AEh, 010h, 002h, 006h, 0A0h, 0D3h, 042h
        db  001h, 0C0h, 000h, 080h, 006h, 0A0h, 0D6h, 0FAh, 0C2h, 0E0h, 007h, 042h, 013h, 02Eh, 0C2h, 0E0h
        db  006h, 02Eh, 013h, 02Bh, 002h, 0E0h, 007h, 058h, 007h, 08Fh, 0FFh, 0BFh, 02Ch, 00Fh, 002h, 0E0h
        db  007h, 098h, 0C0h, 0E0h, 007h, 05Ch, 0C8h, 003h, 008h, 020h, 0C8h, 003h, 001h, 06Ch, 0C2h, 0C3h
        db  0A2h, 0CBh, 0C2h, 0EBh, 00Ch, 032h, 013h, 015h, 0C8h, 00Bh, 0FCh, 000h, 0C3h, 00Bh, 0C3h, 04Bh
        db  0C8h, 00Bh, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 000h, 016h, 0FAh, 003h, 000h, 000h, 002h, 0C8h, 020h
        db  005h, 0F8h, 0FCh, 000h, 016h, 002h, 0C8h, 00Dh, 005h, 0FAh, 0C8h, 00Ch, 005h, 0F8h, 003h, 000h
        db  000h, 00Fh, 001h, 040h, 000h, 020h, 016h, 020h, 010h, 01Fh, 0C3h, 0A0h, 006h, 02Eh, 013h, 003h
        db  0C0h, 0E0h, 005h, 0F8h, 016h, 012h, 0C3h, 020h, 009h, 014h, 013h, 002h, 004h, 020h, 0EAh, 058h
        db  02Eh, 04Fh, 001h, 0C0h, 080h, 000h, 001h, 0A0h, 007h, 062h, 080h, 000h, 0C3h, 08Eh, 013h, 003h
        db  001h, 0A0h, 007h, 062h, 000h, 040h, 004h, 060h, 0D2h, 0C4h, 0C8h, 003h, 001h, 06Ch, 0C8h, 020h
        db  0FCh, 000h, 005h, 0F8h, 0C8h, 003h, 008h, 020h, 001h, 060h, 009h, 018h, 004h, 000h, 013h, 002h
        db  004h, 060h, 0D3h, 0D8h, 007h, 08Ch, 000h, 00Eh, 0C8h, 003h, 001h, 06Ch, 0C2h, 020h, 0FCh, 00Eh
        db  015h, 00Ah, 013h, 009h, 0C2h, 020h, 0FCh, 014h, 002h, 048h, 01Fh, 000h, 006h, 0C8h, 002h, 088h
        db  000h, 012h, 01Bh, 0EEh, 0A3h, 008h, 007h, 088h, 0FCh, 002h, 0C2h, 078h, 0C1h, 0F8h, 083h, 007h
        db  01Ah, 0E7h, 061h, 0CCh, 0C8h, 007h, 0FCh, 004h, 0C1h, 0CCh, 001h, 0C0h, 000h, 040h, 004h, 060h
        db  0D3h, 0E4h, 0C1h, 00Bh, 0C2h, 0A0h, 007h, 0F0h, 0C3h, 020h, 009h, 028h, 007h, 08Dh, 007h, 0F4h
        db  0C3h, 0A0h, 007h, 0F4h, 0C3h, 0E0h, 007h, 0F6h, 006h, 0A0h, 0C6h, 02Eh, 0C8h, 020h, 008h, 014h
        db  008h, 018h, 0C8h, 020h, 008h, 016h, 008h, 01Ah, 0C8h, 00Eh, 008h, 014h, 0C8h, 00Fh, 008h, 016h
        db  007h, 082h, 007h, 0FCh, 004h, 0E0h, 008h, 01Ch, 001h, 040h, 000h, 080h, 016h, 006h, 0C8h, 00Eh
        db  008h, 00Eh, 0C8h, 00Fh, 008h, 010h, 004h, 0E0h, 008h, 01Eh, 006h, 0A0h, 0C6h, 08Ah, 0C2h, 0E0h
        db  007h, 0F8h, 011h, 00Dh, 0C8h, 00Eh, 007h, 0F4h, 0C8h, 00Fh, 007h, 0F6h, 0C8h, 020h, 008h, 018h
        db  008h, 014h, 0C8h, 020h, 008h, 01Ah, 008h, 016h, 006h, 0A0h, 0D2h, 0A8h, 010h, 0CBh, 001h, 040h
        db  000h, 080h, 016h, 002h, 0C8h, 00Bh, 008h, 012h, 0C2h, 0C4h, 061h, 004h, 001h, 080h, 000h, 080h
        db  004h, 05Bh, 0C0h, 08Bh, 0C2h, 0A0h, 007h, 0F0h, 007h, 08Ch, 000h, 004h, 007h, 08Dh, 007h, 0F4h
        db  0C3h, 0A0h, 008h, 014h, 0C3h, 0E0h, 008h, 016h, 006h, 0A0h, 0C6h, 068h, 001h, 060h, 007h, 0F6h
        db  000h, 001h, 013h, 004h, 007h, 0A0h, 008h, 0ACh, 080h, 000h, 004h, 052h, 0C3h, 0E0h, 008h, 0A8h
        db  023h, 0E0h, 0E0h, 014h, 016h, 007h, 0D0h, 020h, 0E0h, 004h, 0E8h, 020h, 0E0h, 01Ah, 007h, 058h
        db  004h, 060h, 0D2h, 0B4h, 007h, 0A0h, 008h, 0ACh, 040h, 000h, 0C8h, 020h, 008h, 014h, 008h, 0AEh
        db  0C8h, 020h, 008h, 016h, 008h, 0B0h, 006h, 0A0h, 0D3h, 042h, 006h, 0A0h, 0D2h, 0AEh, 010h, 0D2h
        db  0C2h, 0ADh, 000h, 002h, 0C2h, 06Dh, 000h, 000h, 016h, 005h, 007h, 0AAh, 000h, 002h, 007h, 036h
        db  02Ch, 09Ah, 003h, 080h, 02Ch, 0EAh, 000h, 002h, 0CBh, 041h, 000h, 000h, 003h, 080h, 0C3h, 02Dh
        db  000h, 018h, 007h, 0ACh, 000h, 002h, 007h, 036h, 04Bh, 020h, 0EAh, 098h, 000h, 00Ah, 0EBh, 020h
        db  0EAh, 092h, 000h, 00Ah, 02Eh, 09Ch, 003h, 080h, 0C2h, 0A0h, 0E0h, 022h, 004h, 060h, 0B3h, 07Ch
        db  0C0h, 0EDh, 000h, 018h, 006h, 0A0h, 0D8h, 048h, 003h, 080h, 0C2h, 044h, 0C0h, 0C3h, 013h, 002h
        db  006h, 0A0h, 0D8h, 048h, 0C3h, 019h, 0CBh, 009h, 000h, 018h, 005h, 0C9h, 0CBh, 019h, 000h, 016h
        db  0C2h, 04Ch, 002h, 02Ch, 000h, 01Ah, 0CFh, 00Dh, 0CFh, 00Eh, 0C7h, 00Fh, 000h, 099h, 004h, 05Bh
        db  007h, 08Ch, 008h, 0BCh, 0C2h, 09Ch, 022h, 0A0h, 0E0h, 014h, 013h, 006h, 0C2h, 0A0h, 007h, 058h
        db  022h, 0A0h, 0E0h, 020h, 016h, 001h, 003h, 080h, 0C1h, 003h, 004h, 0C3h, 007h, 08Ah, 000h, 004h
        db  0A2h, 084h, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 002h, 0E0h, 007h, 058h
        db  007h, 08Dh, 008h, 0BCh, 0C8h, 00Bh, 007h, 0C2h, 006h, 0A0h, 0C4h, 060h, 0C2h, 0E0h, 007h, 0C2h
        db  0E0h, 020h, 0E0h, 020h, 002h, 0E0h, 007h, 0B8h, 004h, 05Bh, 0C3h, 02Dh, 000h, 018h, 0C2h, 08Ch
        db  0C2h, 060h, 001h, 06Ch, 0C8h, 00Ah, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 000h, 013h, 002h, 0C2h, 08Bh
        db  010h, 0F9h, 0C8h, 009h, 001h, 06Ch, 007h, 08Bh, 005h, 0F8h, 0C2h, 05Bh, 013h, 00Ch, 005h, 0CBh
        db  0C2h, 05Bh, 0C6h, 0CAh, 0C2h, 0E0h, 001h, 06Ch, 0C8h, 009h, 001h, 06Ch, 0C8h, 00Ch, 0FCh, 000h
        db  0C8h, 00Bh, 001h, 06Ch, 010h, 002h, 0CEh, 0CCh, 0C6h, 0CAh, 0C2h, 0A0h, 000h, 0E0h, 022h, 0A0h
        db  0E0h, 01Ah, 016h, 006h, 0E8h, 020h, 0E0h, 004h, 007h, 03Ah, 048h, 020h, 0E0h, 01Ah, 000h, 0E0h
        db  003h, 080h, 0D3h, 0E0h, 0E3h, 0ADh, 0C1h, 0E0h, 006h, 0A8h, 016h, 005h, 002h, 007h, 006h, 0A2h
        db  006h, 0A0h, 0C1h, 0D2h, 016h, 00Bh, 0C1h, 0E0h, 006h, 0BAh, 016h, 020h, 002h, 007h, 006h, 0B4h
        db  006h, 0A0h, 0C1h, 0D2h, 013h, 01Bh, 002h, 007h, 006h, 0B8h, 010h, 002h, 002h, 007h, 006h, 0A6h
        db  0C1h, 060h, 0FCh, 002h, 0C8h, 025h, 000h, 00Ch, 0FCh, 002h, 0C9h, 0C5h, 000h, 00Ch, 0CDh, 0F5h
        db  0CDh, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0C5h, 0F5h, 001h, 0B7h, 000h, 028h, 002h, 027h
        db  0FFh, 0F4h, 007h, 0A7h, 000h, 004h, 0DAh, 032h, 02Eh, 097h, 010h, 0D5h, 003h, 000h, 000h, 002h
        db  006h, 0A0h, 0C1h, 0EAh, 003h, 000h, 000h, 00Fh, 02Ch, 020h, 0EDh, 03Ah, 093h, 0E0h, 0E3h, 0ADh
        db  016h, 003h, 002h, 081h, 000h, 016h, 016h, 0C7h, 0C1h, 021h, 0EAh, 0A2h, 004h, 054h, 093h, 0E0h
        db  0E0h, 010h, 016h, 003h, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 00Bh, 0D3h, 0CFh, 016h, 009h, 023h, 0A0h
        db  0E0h, 008h, 016h, 006h, 007h, 084h, 000h, 020h, 0E8h, 004h, 006h, 0D2h, 0D2h, 0A0h, 0E0h, 00Ch
        db  004h, 060h, 0DAh, 0AAh, 004h, 060h, 0DDh, 01Ah, 010h, 07Bh, 0C1h, 022h, 000h, 004h, 004h, 0E2h
        db  000h, 002h, 004h, 054h, 0C8h, 002h, 001h, 06Ch, 0A0h, 082h, 0C8h, 022h, 00Ch, 032h, 0FCh, 000h
        db  002h, 002h, 0FCh, 000h, 093h, 0E0h, 0E3h, 0ACh, 013h, 070h, 006h, 0A0h, 0DBh, 0ACh, 048h, 0A0h
        db  0E0h, 004h, 000h, 00Eh, 002h, 085h, 000h, 007h, 013h, 00Eh, 001h, 00Eh, 000h, 003h, 013h, 00Bh
        db  023h, 0A0h, 0E0h, 022h, 016h, 003h, 0D2h, 0A0h, 0E0h, 00Eh, 010h, 002h, 0D2h, 0A0h, 0E3h, 0AAh
        db  001h, 08Eh, 000h, 003h, 010h, 05Ah, 0C8h, 005h, 006h, 0FCh, 0C0h, 0C3h, 016h, 053h, 043h, 0A0h
        db  0E0h, 010h, 088h, 022h, 000h, 00Eh, 009h, 01Ah, 016h, 00Ah, 088h, 022h, 000h, 010h, 009h, 01Ch
        db  016h, 006h, 088h, 022h, 000h, 012h, 009h, 01Eh, 016h, 002h, 0E3h, 0A0h, 0E0h, 010h, 002h, 085h
        db  000h, 009h, 013h, 002h, 006h, 0A0h, 0E1h, 0B8h, 0A1h, 045h, 0C1h, 065h, 0E3h, 0AEh, 004h, 055h
        db  0C0h, 062h, 000h, 004h, 0C8h, 022h, 000h, 006h, 001h, 06Ch, 002h, 082h, 004h, 048h, 01Bh, 002h
        db  043h, 0A0h, 0E0h, 00Ch, 0C1h, 022h, 000h, 00Eh, 004h, 051h, 0C0h, 042h, 004h, 0E1h, 000h, 002h
        db  0C0h, 0A2h, 000h, 00Ch, 0C1h, 022h, 000h, 00Ah, 021h, 020h, 0E0h, 018h, 013h, 007h, 0C8h, 0A1h
        db  000h, 00Ah, 000h, 00Ah, 0C8h, 0A1h, 000h, 008h, 000h, 008h, 010h, 0E2h, 0C8h, 022h, 000h, 006h
        db  001h, 06Ch, 006h, 0A0h, 0E0h, 076h, 004h, 060h, 0DAh, 088h, 0C8h, 002h, 006h, 0D4h, 0C1h, 062h
        db  000h, 002h, 0C1h, 065h, 0E3h, 0D0h, 004h, 055h, 010h, 00Bh, 007h, 085h, 003h, 0F4h, 004h, 0F5h
        db  0CDh, 060h, 0EDh, 018h, 006h, 0A0h, 0E2h, 0A2h, 0E3h, 0A0h, 0E0h, 00Ch, 02Eh, 0A0h, 003h, 0F4h
        db  004h, 060h, 0D8h, 0EEh, 006h, 0A0h, 0DFh, 042h, 010h, 00Ch, 006h, 0A0h, 0E0h, 076h, 010h, 009h
        db  006h, 0A0h, 0E2h, 02Ah, 010h, 006h, 006h, 0A0h, 0E0h, 076h, 0C8h, 003h, 008h, 0DCh, 0D2h, 0A0h
        db  0E3h, 0ACh, 006h, 0A0h, 0DBh, 01Ch, 092h, 0A0h, 0E0h, 026h, 016h, 00Ch, 0D3h, 0E0h, 0E0h, 026h
        db  023h, 0E0h, 0E0h, 014h, 013h, 00Ah, 0C1h, 00Ah, 083h, 0C4h, 013h, 007h, 0C3h, 0C4h, 0C1h, 024h
        db  0E3h, 0D4h, 004h, 054h, 093h, 0CAh, 013h, 0DCh, 0D3h, 0CAh, 003h, 0B0h, 0D8h, 00Fh, 006h, 059h
        db  071h, 004h, 0C1h, 024h, 0E3h, 0E0h, 004h, 054h, 023h, 0A0h, 0E0h, 00Ch, 013h, 0D1h, 0C3h, 04Dh
        db  013h, 0CFh, 007h, 086h, 000h, 002h, 007h, 084h, 000h, 026h, 023h, 046h, 013h, 003h, 006h, 044h
        db  0A1h, 086h, 010h, 0FBh, 043h, 046h, 010h, 0B9h, 007h, 085h, 007h, 01Ch, 007h, 086h, 004h, 01Ah
        db  06Dh, 076h, 06Dh, 076h, 06Dh, 076h, 005h, 0C6h, 06Dh, 076h, 06Dh, 076h, 06Dh, 076h, 007h, 083h
        db  090h, 000h, 010h, 0BEh, 0C3h, 00Bh, 007h, 086h, 001h, 000h, 007h, 085h, 080h, 000h, 0C1h, 020h
        db  006h, 0D2h, 013h, 037h, 004h, 0C4h, 0C0h, 060h, 006h, 0D2h, 020h, 045h, 013h, 004h, 005h, 084h
        db  009h, 015h, 016h, 0F9h, 010h, 02Eh, 0D3h, 0CFh, 016h, 006h, 023h, 0E0h, 0E0h, 014h, 016h, 003h
        db  001h, 00Eh, 000h, 003h, 013h, 003h, 004h, 0E0h, 006h, 0D2h, 010h, 023h, 0D0h, 064h, 007h, 01Ch
        db  0B0h, 046h, 018h, 010h, 0D9h, 001h, 007h, 01Ch, 023h, 060h, 0E0h, 020h, 013h, 00Bh, 007h, 081h
        db  000h, 018h, 0C0h, 061h, 0E3h, 0F0h, 088h, 011h, 0EDh, 018h, 013h, 004h, 002h, 008h, 080h, 018h
        db  006h, 0A0h, 0DEh, 0F6h, 0D0h, 064h, 007h, 028h, 0B0h, 046h, 018h, 008h, 0D9h, 001h, 007h, 028h
        db  0B0h, 046h, 017h, 004h, 007h, 083h, 080h, 040h, 006h, 0A0h, 0E2h, 02Ah, 048h, 005h, 006h, 0D2h
        db  016h, 0CAh, 0C1h, 020h, 008h, 0E4h, 016h, 001h, 004h, 05Ch, 002h, 004h, 000h, 007h, 006h, 020h
        db  008h, 0E4h, 010h, 0C9h, 0C3h, 00Bh, 004h, 0C5h, 0C0h, 042h, 004h, 0C7h, 0C2h, 020h, 001h, 06Ch
        db  0A1h, 0E1h, 000h, 004h, 0C8h, 011h, 001h, 06Ch, 016h, 0FBh, 0C8h, 008h, 001h, 06Ch, 004h, 0C8h
        db  043h, 0A0h, 0E0h, 01Ah, 0C1h, 022h, 000h, 00Eh, 015h, 00Dh, 013h, 00Ch, 0E3h, 0A0h, 0E0h, 01Ah
        db  006h, 0A0h, 0E2h, 014h, 0C2h, 008h, 013h, 048h, 002h, 088h, 000h, 012h, 01Bh, 045h, 022h, 020h
        db  0E0h, 022h, 013h, 042h, 0C1h, 002h, 0A1h, 008h, 005h, 008h, 002h, 028h, 0FFh, 0F2h, 0A2h, 007h
        db  007h, 083h, 080h, 001h, 002h, 088h, 000h, 004h, 011h, 06Eh, 0C2h, 064h, 000h, 016h, 0D2h, 049h
        db  016h, 002h, 081h, 002h, 016h, 031h, 001h, 009h, 0F0h, 000h, 016h, 028h, 0C1h, 049h, 071h, 045h
        db  004h, 0C3h, 002h, 085h, 000h, 009h, 013h, 07Ch, 007h, 083h, 080h, 002h, 0C1h, 0A4h, 000h, 014h
        db  081h, 088h, 016h, 076h, 005h, 083h, 002h, 085h, 000h, 010h, 01Bh, 013h, 005h, 083h, 099h, 049h
        db  0EAh, 0C2h, 013h, 00Ah, 098h, 009h, 0E0h, 00Eh, 016h, 06Bh, 098h, 025h, 0EAh, 0C2h, 0E0h, 00Ch
        db  016h, 067h, 0C1h, 0E0h, 006h, 0ECh, 016h, 064h, 004h, 0C3h, 0C2h, 052h, 013h, 00Fh, 007h, 083h
        db  080h, 009h, 0C1h, 0E0h, 009h, 018h, 001h, 047h, 010h, 000h, 016h, 05Ah, 0C0h, 0A0h, 001h, 06Ch
        db  006h, 0A0h, 0E0h, 0CEh, 004h, 060h, 0DAh, 088h, 004h, 060h, 0DAh, 092h, 007h, 089h, 007h, 00Eh
        db  004h, 0C7h, 0D1h, 0E5h, 0EAh, 0D3h, 013h, 005h, 006h, 0C7h, 002h, 027h, 0EAh, 0E4h, 0CEh, 077h
        db  015h, 0FEh, 0C0h, 044h, 002h, 021h, 000h, 018h, 002h, 028h, 0FFh, 0FCh, 013h, 036h, 0C1h, 091h
        db  0D1h, 086h, 013h, 01Fh, 006h, 0C6h, 007h, 087h, 007h, 00Eh, 0C0h, 0F7h, 002h, 046h, 0BFh, 0FFh
        db  002h, 043h, 03Fh, 0FFh, 091h, 0A0h, 0EDh, 03Fh, 016h, 009h, 003h, 0B0h, 098h, 020h, 0E0h, 00Eh
        db  006h, 05Dh, 016h, 00Fh, 0C8h, 021h, 000h, 002h, 007h, 00Ch, 010h, 017h, 082h, 047h, 01Bh, 00Ch
        db  090h, 0C6h, 016h, 0EBh, 006h, 047h, 004h, 0F7h, 003h, 0B0h, 098h, 020h, 006h, 05Dh, 006h, 057h
        db  013h, 00Ch, 007h, 083h, 080h, 005h, 010h, 01Ch, 0C0h, 0D1h, 020h, 0E0h, 0E0h, 016h, 016h, 003h
        db  007h, 083h, 080h, 008h, 010h, 015h, 044h, 060h, 0E0h, 026h, 071h, 086h, 0A0h, 046h, 062h, 006h
        db  007h, 083h, 080h, 005h, 0C2h, 008h, 015h, 0CBh, 016h, 00Bh, 004h, 0C3h, 007h, 087h, 007h, 00Eh
        db  0C0h, 077h, 082h, 047h, 01Bh, 005h, 020h, 060h, 0E0h, 006h, 016h, 0FAh, 007h, 083h, 080h, 007h
        db  004h, 05Ch, 092h, 0A0h, 0E0h, 00Eh, 016h, 00Fh, 0C8h, 020h, 0E0h, 020h, 007h, 008h, 004h, 0E0h
        db  001h, 084h, 005h, 060h, 007h, 002h, 013h, 03Bh, 048h, 020h, 0E0h, 006h, 001h, 082h, 007h, 083h
        db  0C0h, 000h, 006h, 0A0h, 0E2h, 02Ah, 0C8h, 020h, 0E0h, 01Eh, 007h, 002h, 0E3h, 0A0h, 0E0h, 004h
        db  002h, 008h, 080h, 024h, 006h, 0A0h, 0DEh, 0F6h, 010h, 034h, 0C1h, 020h, 001h, 084h, 002h, 044h
        db  088h, 000h, 013h, 01Eh, 048h, 004h, 001h, 084h, 006h, 020h, 007h, 002h, 016h, 0F1h, 023h, 0A0h
        db  0E0h, 022h, 016h, 005h, 043h, 0A0h, 0E0h, 022h, 0D2h, 0A0h, 0E0h, 00Eh, 010h, 0D5h, 023h, 0E0h
        db  0E0h, 014h, 013h, 004h, 098h, 020h, 0E3h, 0ABh, 006h, 065h, 016h, 006h, 092h, 0A0h, 0E0h, 00Eh
        db  013h, 0CBh, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 0D7h, 007h, 083h, 000h, 002h, 004h, 060h, 0DAh, 0A2h
        db  0E8h, 020h, 0E0h, 006h, 001h, 082h, 007h, 020h, 007h, 002h, 043h, 0A0h, 0E0h, 004h, 0C8h, 020h
        db  0E4h, 070h, 001h, 086h, 088h, 020h, 0E0h, 020h, 007h, 008h, 016h, 003h, 0C8h, 020h, 0EAh, 0FCh
        db  007h, 008h, 004h, 060h, 0DAh, 0AAh, 001h, 00Eh, 000h, 003h, 013h, 016h, 0D3h, 0CFh, 016h, 008h
        db  023h, 0A0h, 0E0h, 020h, 016h, 003h, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 002h, 0D2h, 0A0h, 0E0h, 00Eh
        db  001h, 08Eh, 000h, 003h, 010h, 009h, 0C1h, 060h, 001h, 084h, 021h, 060h, 0E0h, 00Ah, 016h, 004h
        db  007h, 083h, 084h, 000h, 004h, 060h, 0DAh, 0A2h, 0C8h, 020h, 0E0h, 02Eh, 001h, 084h, 002h, 008h
        db  080h, 006h, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 0AAh, 0E3h, 060h, 0E0h, 020h, 004h, 060h
        db  0DAh, 0AAh, 093h, 0E0h, 0E0h, 026h, 016h, 010h, 023h, 0A0h, 0E0h, 008h, 016h, 00Dh, 023h, 0A0h
        db  0E0h, 006h, 013h, 002h, 0E3h, 060h, 0E0h, 01Ch, 0E3h, 060h, 0E0h, 018h, 043h, 0A0h, 0E0h, 006h
        db  002h, 008h, 080h, 03Ch, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 0AAh, 092h, 0A0h, 0E3h, 0AAh
        db  013h, 003h, 092h, 0A0h, 0E3h, 0ABh, 016h, 01Eh, 023h, 0E0h, 0E0h, 014h, 013h, 008h, 098h, 020h
        db  0E3h, 0ABh, 006h, 065h, 013h, 004h, 007h, 083h, 000h, 007h, 004h, 060h, 0DAh, 0A2h, 0D2h, 0A0h
        db  0E0h, 00Eh, 0C8h, 020h, 0E0h, 020h, 007h, 008h, 027h, 0A0h, 0E0h, 004h, 016h, 00Bh, 0C8h, 020h
        db  0E0h, 01Eh, 007h, 008h, 093h, 0E0h, 0E3h, 0AAh, 016h, 005h, 023h, 0A0h, 0E0h, 012h, 013h, 002h
        db  006h, 020h, 007h, 008h, 004h, 060h, 0DAh, 0AAh, 023h, 0E0h, 0E0h, 014h, 013h, 036h, 003h, 0B0h
        db  098h, 020h, 0E0h, 00Eh, 006h, 06Fh, 016h, 00Fh, 0D3h, 0CFh, 016h, 02Fh, 0D2h, 0A0h, 0E3h, 0AAh
        db  004h, 060h, 0DAh, 0AAh, 092h, 0A0h, 0E0h, 00Ch, 016h, 028h, 023h, 0E0h, 0E0h, 014h, 013h, 0F6h
        db  007h, 083h, 000h, 006h, 010h, 007h, 007h, 083h, 000h, 005h, 093h, 0E0h, 0E0h, 00Eh, 016h, 002h
        db  007h, 083h, 000h, 007h, 004h, 060h, 0DAh, 0A2h, 0E3h, 060h, 0E0h, 012h, 023h, 0E0h, 0E0h, 014h
        db  013h, 00Dh, 098h, 020h, 0E0h, 00Ch, 006h, 065h, 016h, 003h, 0D8h, 020h, 0E3h, 0ABh, 006h, 065h
        db  010h, 00Ch, 0C1h, 020h, 001h, 084h, 021h, 020h, 0E0h, 006h, 016h, 0D6h, 048h, 020h, 0E0h, 006h
        db  001h, 084h, 002h, 008h, 080h, 030h, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 0AAh, 0E3h, 0A0h
        db  0E0h, 00Ah, 0C2h, 008h, 011h, 002h, 043h, 0A0h, 0E0h, 00Ah, 042h, 020h, 0E0h, 004h, 002h, 028h
        db  0E3h, 0F0h, 0C0h, 058h, 0C0h, 002h, 088h, 011h, 0EDh, 018h, 016h, 003h, 02Ch, 0D1h, 0C0h, 058h
        db  004h, 0D1h, 0C0h, 080h, 001h, 00Eh, 010h, 000h, 013h, 00Fh, 0CCh, 060h, 0EDh, 018h, 005h, 0C8h
        db  0CCh, 078h, 016h, 003h, 006h, 041h, 0CCh, 060h, 006h, 0D6h, 0C4h, 058h, 016h, 002h, 0C4h, 060h
        db  007h, 000h, 002h, 021h, 0FFh, 0FAh, 02Ch, 091h, 004h, 05Bh, 0C3h, 00Bh, 006h, 0A0h, 0DFh, 0DEh
        db  006h, 0A0h, 0DFh, 0B8h, 0C2h, 008h, 016h, 005h, 0C2h, 062h, 000h, 002h, 026h, 060h, 0E4h, 06Ah
        db  016h, 00Dh, 0C2h, 042h, 005h, 0C9h, 0CEh, 060h, 0EDh, 03Ch, 0C6h, 060h, 0EAh, 0FEh, 006h, 0A0h
        db  0E0h, 020h, 0CAh, 018h, 000h, 00Ah, 046h, 020h, 0E0h, 026h, 016h, 004h, 0C0h, 0A0h, 001h, 06Ch
        db  02Eh, 012h, 010h, 01Dh, 0C1h, 012h, 013h, 005h, 0C1h, 060h, 001h, 06Ch, 02Eh, 014h, 0C8h, 005h
        db  001h, 06Ch, 004h, 0D2h, 006h, 048h, 007h, 084h, 000h, 002h, 0C1h, 048h, 0C0h, 0A0h, 001h, 06Ch
        db  0C0h, 002h, 006h, 0A0h, 0E2h, 0A2h, 0C5h, 060h, 0FCh, 002h, 002h, 007h, 006h, 0A2h, 002h, 025h
        db  0FFh, 0F4h, 0C8h, 005h, 0FCh, 002h, 0C2h, 020h, 001h, 06Ch, 006h, 0A0h, 0C1h, 096h, 004h, 05Ch
        db  0C2h, 042h, 002h, 029h, 000h, 008h, 0C2h, 039h, 002h, 048h, 0C0h, 000h, 002h, 088h, 0C0h, 000h
        db  016h, 008h, 08Eh, 060h, 0E0h, 02Eh, 016h, 005h, 086h, 060h, 0E0h, 02Eh, 016h, 002h, 004h, 0C8h
        db  004h, 05Bh, 007h, 008h, 004h, 05Bh, 088h, 020h, 0E1h, 090h, 001h, 06Ch, 016h, 002h, 004h, 060h
        db  0DAh, 092h, 004h, 05Bh, 007h, 088h, 001h, 0AEh, 0E8h, 020h, 0E0h, 00Eh, 001h, 080h, 006h, 008h
        db  016h, 0FEh, 048h, 020h, 0E0h, 00Eh, 001h, 080h, 004h, 05Bh, 004h, 0C2h, 023h, 0A0h, 0E0h, 00Ch
        db  016h, 00Ah, 02Fh, 020h, 006h, 030h, 007h, 082h, 0FFh, 0DFh, 02Ch, 002h, 002h, 082h, 003h, 0F4h
        db  016h, 0FAh, 043h, 0A0h, 0E0h, 00Ch, 004h, 05Bh, 0C2h, 042h, 007h, 088h, 000h, 00Eh, 0A2h, 009h
        db  002h, 029h, 000h, 008h, 0CEh, 078h, 0CEh, 078h, 0CEh, 078h, 0CEh, 060h, 009h, 01Ah, 0CEh, 060h
        db  009h, 01Ch, 0CEh, 060h, 009h, 01Eh, 023h, 0A0h, 0E0h, 01Ah, 016h, 00Fh, 0C2h, 058h, 002h, 049h
        db  01Fh, 080h, 02Ah, 060h, 0E0h, 014h, 0E8h, 0A0h, 0E0h, 004h, 000h, 00Eh, 0C6h, 009h, 002h, 049h
        db  01Fh, 000h, 006h, 0C9h, 0A2h, 009h, 0A8h, 089h, 000h, 004h, 002h, 028h, 000h, 002h, 0C2h, 058h
        db  00Ah, 049h, 002h, 049h, 0F0h, 000h, 0D6h, 009h, 004h, 0E2h, 000h, 006h, 004h, 05Bh, 007h, 000h
        db  0C0h, 082h, 013h, 053h, 0C0h, 0A0h, 001h, 06Ch, 0C1h, 0A0h, 0FCh, 006h, 002h, 046h, 000h, 00Fh
        db  002h, 086h, 000h, 001h, 012h, 03Dh, 088h, 006h, 006h, 0F2h, 016h, 012h, 002h, 001h, 0FCh, 00Eh
        db  088h, 031h, 006h, 0F4h, 016h, 00Dh, 088h, 031h, 006h, 0F6h, 016h, 00Ah, 088h, 031h, 006h, 0F8h
        db  016h, 007h, 002h, 086h, 000h, 002h, 016h, 02Ch, 088h, 020h, 007h, 00Ah, 006h, 0FAh, 013h, 028h
        db  0C1h, 020h, 009h, 018h, 001h, 044h, 008h, 000h, 013h, 006h, 002h, 086h, 000h, 002h, 016h, 020h
        db  001h, 044h, 000h, 080h, 016h, 01Dh, 007h, 000h, 023h, 0E0h, 0E0h, 014h, 016h, 019h, 002h, 082h
        db  000h, 043h, 013h, 016h, 002h, 000h, 0FCh, 002h, 0C0h, 040h, 001h, 0B0h, 000h, 020h, 001h, 060h
        db  009h, 018h, 000h, 001h, 016h, 007h, 0A0h, 060h, 008h, 0DEh, 0CCh, 060h, 005h, 0EEh, 0C4h, 050h
        db  0C4h, 020h, 008h, 0DEh, 007h, 080h, 007h, 036h, 007h, 081h, 000h, 040h, 02Ch, 040h, 004h, 0C0h
        db  007h, 084h, 006h, 0F2h, 0CDh, 006h, 002h, 001h, 0FCh, 00Eh, 0CDh, 031h, 0CDh, 031h, 0CDh, 031h
        db  0C5h, 020h, 007h, 00Ah, 0C0h, 000h, 013h, 001h, 02Eh, 012h, 004h, 0E0h, 001h, 06Ch, 004h, 05Bh
        db  0C8h, 00Bh, 008h, 0D4h, 006h, 0A0h, 0E1h, 046h, 002h, 008h, 080h, 042h, 006h, 0A0h, 0DEh, 0F6h
        db  002h, 008h, 080h, 030h, 006h, 0A0h, 0DEh, 0F6h, 0C2h, 0E0h, 008h, 0D4h, 004h, 05Bh, 0C3h, 00Bh
        db  048h, 020h, 0EBh, 000h, 001h, 082h, 048h, 020h, 0E4h, 06Eh, 001h, 080h, 048h, 020h, 0E0h, 022h
        db  001h, 0AEh, 048h, 020h, 0E0h, 022h, 009h, 026h, 043h, 060h, 0E0h, 018h, 043h, 0A0h, 0E0h, 008h
        db  002h, 008h, 000h, 042h, 006h, 0A0h, 0DEh, 0F6h, 004h, 05Ch, 0C3h, 00Bh, 0E8h, 020h, 0E0h, 00Eh
        db  001h, 082h, 0E8h, 020h, 0E0h, 022h, 001h, 0AEh, 0E8h, 020h, 0E0h, 022h, 009h, 026h, 0E3h, 0A0h
        db  0E0h, 008h, 0E3h, 060h, 0E0h, 018h, 043h, 0A0h, 0E0h, 006h, 002h, 008h, 080h, 03Ch, 006h, 0A0h
        db  0DEh, 0F6h, 002h, 008h, 080h, 042h, 006h, 0A0h, 0DEh, 0F6h, 004h, 05Ch, 0C3h, 00Bh, 007h, 083h
        db  068h, 000h, 006h, 0A0h, 0E2h, 02Ah, 007h, 083h, 080h, 010h, 006h, 0A0h, 0E2h, 02Ah, 004h, 05Ch
        db  0C3h, 00Bh, 006h, 0A0h, 0E2h, 014h, 0A2h, 002h, 0C2h, 068h, 000h, 014h, 002h, 029h, 0FFh, 0FCh
        db  013h, 024h, 002h, 028h, 000h, 018h, 007h, 087h, 000h, 00Eh, 007h, 081h, 007h, 00Eh, 004h, 0F1h
        db  006h, 047h, 015h, 0FDh, 0C0h, 058h, 003h, 0B0h, 078h, 001h, 006h, 063h, 002h, 041h, 000h, 03Fh
        db  013h, 00Eh, 002h, 081h, 000h, 01Fh, 01Bh, 00Bh, 0A0h, 041h, 0C0h, 061h, 0E4h, 050h, 0C1h, 0F8h
        db  006h, 0C7h, 071h, 0C7h, 006h, 047h, 0CCh, 078h, 006h, 047h, 015h, 0FDh, 010h, 004h, 0C0h, 058h
        db  006h, 0C1h, 070h, 041h, 0A2h, 001h, 0C2h, 049h, 015h, 0E5h, 004h, 05Ch, 023h, 0A0h, 0E0h, 01Ah
        db  013h, 002h, 004h, 0C8h, 004h, 05Bh, 0C2h, 022h, 000h, 014h, 002h, 048h, 01Fh, 000h, 006h, 0C8h
        db  004h, 05Bh, 002h, 083h, 000h, 00Fh, 01Bh, 017h, 0C1h, 0A0h, 006h, 0D4h, 013h, 035h, 002h, 026h
        db  000h, 004h, 0CDh, 0A0h, 0EDh, 018h, 0C5h, 083h, 013h, 004h, 0C2h, 04Ah, 00Ah, 039h, 0E0h, 0C9h
        db  0C5h, 083h, 007h, 086h, 007h, 036h, 007h, 087h, 000h, 010h, 0C2h, 020h, 006h, 0D4h, 004h, 0E0h
        db  006h, 0D4h, 02Ch, 046h, 004h, 05Bh, 0C0h, 060h, 006h, 0FEh, 0C2h, 020h, 009h, 018h, 002h, 048h
        db  060h, 000h, 022h, 020h, 0E0h, 006h, 016h, 004h, 0E2h, 020h, 0E0h, 00Ah, 0E2h, 020h, 0E0h, 018h
        db  00Ah, 013h, 018h, 004h, 005h, 041h, 048h, 003h, 006h, 0FEh, 010h, 006h, 002h, 083h, 000h, 002h
        db  016h, 001h, 009h, 013h, 0E8h, 003h, 006h, 0FEh, 040h, 0C8h, 040h, 0C1h, 013h, 005h, 007h, 088h
        db  007h, 036h, 007h, 089h, 040h, 000h, 02Ch, 048h, 004h, 05Bh, 004h, 0C9h, 0C1h, 024h, 0EBh, 016h
        db  0C1h, 084h, 071h, 086h, 002h, 026h, 0EBh, 0C8h, 006h, 0C4h, 071h, 004h, 002h, 024h, 0EBh, 03Eh
        db  0D2h, 014h, 009h, 0C8h, 0A2h, 008h, 003h, 0B0h, 0D8h, 034h, 006h, 05Fh, 002h, 047h, 000h, 00Fh
        db  0A1h, 0C7h, 0C2h, 028h, 0EBh, 004h, 004h, 058h, 0CDh, 076h, 006h, 047h, 016h, 0FDh, 010h, 032h
        db  0C2h, 036h, 010h, 026h, 009h, 017h, 0A1h, 047h, 010h, 02Dh, 009h, 017h, 061h, 047h, 010h, 02Ah
        db  043h, 0A0h, 0E0h, 016h, 004h, 05Bh, 043h, 0A0h, 0E0h, 016h, 0C2h, 049h, 016h, 003h, 0C2h, 044h
        db  0C8h, 006h, 008h, 0D4h, 0C1h, 027h, 0ECh, 0D8h, 0C1h, 084h, 071h, 086h, 002h, 026h, 0EDh, 00Eh
        db  006h, 0C4h, 071h, 004h, 002h, 024h, 0ECh, 0F4h, 010h, 0D3h, 0C1h, 009h, 0C1h, 0A0h, 008h, 0D4h
        db  004h, 0C9h, 010h, 010h, 0C2h, 036h, 0D5h, 078h, 041h, 060h, 0E0h, 022h, 005h, 0C5h, 010h, 00Ah
        db  0CDh, 078h, 006h, 047h, 015h, 0FDh, 010h, 006h, 023h, 0A0h, 0E0h, 016h, 016h, 0CDh, 0C2h, 049h
        db  016h, 0ECh, 010h, 0D6h, 0E3h, 0A0h, 0E0h, 016h, 010h, 0BBh, 002h, 008h, 080h, 05Ah, 006h, 0A0h
        db  0DEh, 0F6h, 010h, 03Eh, 092h, 0A0h, 0E0h, 00Ch, 016h, 015h, 002h, 044h, 05Eh, 000h, 016h, 014h
        db  048h, 020h, 0E4h, 06Eh, 001h, 080h, 006h, 0A0h, 0E1h, 072h, 0C8h, 020h, 001h, 09Eh, 001h, 09Eh
        db  02Eh, 0E0h, 000h, 001h, 043h, 0A0h, 0E0h, 018h, 0D2h, 0A0h, 0E0h, 026h, 007h, 083h, 000h, 010h
        db  006h, 0A0h, 0E2h, 02Ah, 004h, 060h, 0DAh, 0AAh, 007h, 084h, 000h, 008h, 004h, 060h, 0DAh, 072h
        db  007h, 085h, 002h, 003h, 0C8h, 005h, 006h, 0CEh, 043h, 0A0h, 0E0h, 012h, 004h, 0E0h, 006h, 0FAh
        db  006h, 0A0h, 0E1h, 0A4h, 002h, 008h, 080h, 048h, 006h, 0A0h, 0DEh, 0F6h, 010h, 011h, 027h, 0A0h
        db  0E0h, 02Ch, 016h, 004h, 002h, 008h, 080h, 054h, 006h, 0A0h, 0DEh, 0F6h, 007h, 083h, 0A8h, 000h
        db  088h, 020h, 007h, 008h, 0E0h, 020h, 016h, 002h, 007h, 083h, 0E8h, 000h, 006h, 0A0h, 0E2h, 02Ah
        db  002h, 008h, 000h, 036h, 006h, 0A0h, 0DEh, 0F6h, 0E8h, 020h, 0E0h, 00Ch, 001h, 082h, 023h, 0A0h
        db  0E0h, 018h, 013h, 006h, 0E3h, 0A0h, 0E0h, 018h, 02Eh, 0E0h, 000h, 000h, 0C0h, 041h, 016h, 0FAh
        db  006h, 0A0h, 0E0h, 002h, 0C0h, 082h, 013h, 002h, 002h, 04Fh, 0FFh, 080h, 004h, 0C4h, 0D1h, 00Fh
        db  006h, 0C4h, 004h, 060h, 0DAh, 072h, 006h, 0A0h, 0E4h, 024h, 002h, 008h, 080h, 036h, 007h, 0A0h
        db  006h, 0D6h, 0E4h, 012h, 006h, 0A0h, 0DEh, 0F6h, 010h, 010h, 006h, 0A0h, 0E4h, 024h, 0D1h, 020h
        db  006h, 0CEh, 013h, 0E6h, 078h, 020h, 0E0h, 012h, 006h, 0CEh, 010h, 0E2h, 0C1h, 020h, 004h, 016h
        db  00Ah, 014h, 006h, 0C4h, 091h, 00Ah, 016h, 001h, 004h, 05Bh, 004h, 060h, 0DAh, 0AAh, 004h, 0CAh
        db  004h, 0CDh, 004h, 0CEh, 004h, 0CFh, 004h, 0E0h, 006h, 0A8h, 004h, 0E0h, 006h, 0BAh, 007h, 084h
        db  001h, 0A0h, 007h, 085h, 000h, 010h, 004h, 0F4h, 006h, 045h, 015h, 0FDh, 007h, 084h, 006h, 0D8h
        db  007h, 085h, 007h, 034h, 061h, 044h, 004h, 0F4h, 006h, 045h, 015h, 0FDh, 007h, 084h, 000h, 0C8h
        db  0C8h, 004h, 007h, 000h, 007h, 084h, 07Fh, 0FFh, 0C8h, 004h, 006h, 0F0h, 007h, 084h, 000h, 006h
        db  0C8h, 004h, 006h, 0EEh, 003h, 0B0h, 0D8h, 020h, 0E0h, 010h, 006h, 065h, 004h, 060h, 0DAh, 0AAh
        db  0F8h, 020h, 0EDh, 063h, 001h, 082h, 0C8h, 020h, 0E0h, 010h, 006h, 0C6h, 0C8h, 020h, 0E0h, 020h
        db  006h, 0C8h, 02Eh, 0E0h, 000h, 000h, 006h, 0A0h, 0E0h, 002h, 0C8h, 020h, 009h, 01Ah, 001h, 0A0h
        db  0C8h, 020h, 009h, 01Ch, 001h, 0A2h, 0C8h, 020h, 009h, 01Eh, 001h, 0A4h, 0C8h, 020h, 009h, 01Ch
        db  001h, 0B0h, 0C8h, 020h, 009h, 01Eh, 001h, 0B2h, 0C8h, 020h, 009h, 01Eh, 006h, 0CCh, 0F8h, 020h
        db  0EDh, 062h, 001h, 080h, 006h, 0A0h, 0DFh, 0ECh, 048h, 020h, 0E0h, 004h, 001h, 080h, 004h, 0E0h
        db  001h, 09Eh, 002h, 008h, 080h, 012h, 006h, 0A0h, 0DEh, 0F6h, 007h, 084h, 000h, 00Ah, 007h, 085h
        db  003h, 0F4h, 088h, 020h, 006h, 0C6h, 0E0h, 020h, 01Bh, 004h, 007h, 084h, 000h, 01Ch, 007h, 085h
        db  003h, 0F8h, 006h, 0A0h, 0E2h, 0A2h, 007h, 085h, 0E5h, 016h, 0C8h, 005h, 003h, 0F8h, 023h, 0A0h
        db  0E0h, 00Ch, 013h, 023h, 0E3h, 0A0h, 0E0h, 00Ch, 02Eh, 0A0h, 003h, 0F4h, 010h, 01Eh, 003h, 0B0h
        db  098h, 020h, 0E0h, 010h, 006h, 065h, 016h, 019h, 002h, 044h, 05Eh, 000h, 016h, 004h, 006h, 020h
        db  006h, 0C6h, 016h, 0D7h, 010h, 008h, 003h, 0B0h, 098h, 020h, 0E0h, 010h, 006h, 065h, 016h, 00Dh
        db  006h, 020h, 006h, 0C8h, 016h, 0C4h, 001h, 00Eh, 000h, 003h, 013h, 003h, 007h, 083h, 082h, 000h
        db  010h, 002h, 007h, 083h, 000h, 001h, 004h, 060h, 0DAh, 0A2h, 004h, 060h, 0DAh, 088h, 001h, 00Eh
        db  000h, 003h, 013h, 00Ah, 002h, 008h, 080h, 00Ch, 006h, 0A0h, 0DEh, 0F6h, 0E3h, 0E0h, 0E0h, 014h
        db  0C8h, 020h, 0E4h, 070h, 001h, 086h, 010h, 026h, 048h, 020h, 0E0h, 00Ch, 001h, 082h, 02Eh, 0E0h
        db  000h, 001h, 0C1h, 060h, 008h, 0D0h, 00Ah, 035h, 0E8h, 005h, 001h, 082h, 0C1h, 020h, 009h, 018h
        db  001h, 004h, 000h, 006h, 013h, 006h, 0D8h, 020h, 0E1h, 0D2h, 008h, 0E1h, 0D8h, 020h, 0E1h, 0D2h
        db  001h, 083h, 021h, 020h, 0E0h, 022h, 016h, 003h, 0E8h, 020h, 0E0h, 022h, 001h, 080h, 021h, 020h
        db  0E0h, 004h, 016h, 004h, 0E3h, 0A0h, 0E0h, 014h, 004h, 060h, 0DEh, 094h, 002h, 008h, 080h, 000h
        db  006h, 0A0h, 0DEh, 0F6h, 0E8h, 020h, 0E0h, 008h, 001h, 082h, 004h, 060h, 0DAh, 088h, 0C8h, 020h
        db  0E4h, 070h, 001h, 086h, 002h, 008h, 080h, 000h, 006h, 0A0h, 0DEh, 0F6h, 0C2h, 020h, 008h, 0D0h
        db  0A2h, 008h, 005h, 008h, 0C8h, 028h, 0E0h, 022h, 006h, 0CAh, 0C8h, 020h, 0E0h, 020h, 006h, 0C6h
        db  0C8h, 020h, 0E0h, 020h, 006h, 0C8h, 0E3h, 060h, 0E0h, 016h, 004h, 060h, 0DAh, 0AAh, 0C1h, 044h
        db  002h, 044h, 05Eh, 000h, 016h, 0F8h, 025h, 060h, 0E4h, 06Ah, 016h, 00Fh, 006h, 020h, 006h, 0C6h
        db  016h, 0F2h, 006h, 020h, 006h, 0CAh, 013h, 003h, 005h, 0A0h, 006h, 0CCh, 010h, 0E6h, 003h, 0B0h
        db  0D8h, 020h, 0E0h, 00Ch, 006h, 065h, 004h, 060h, 0DAh, 0AAh, 006h, 020h, 006h, 0C8h, 016h, 0E3h
        db  088h, 020h, 009h, 01Eh, 006h, 0CCh, 016h, 003h, 007h, 083h, 000h, 008h, 010h, 002h, 007h, 083h
        db  000h, 00Ch, 004h, 060h, 0DAh, 0A2h, 004h, 060h, 0DAh, 0AAh, 023h, 0A0h, 0E0h, 008h, 013h, 003h
        db  023h, 060h, 0E0h, 012h, 016h, 006h, 003h, 0B0h, 0D8h, 020h, 0E3h, 0ABh, 006h, 065h, 004h, 060h
        db  0DAh, 0AAh, 002h, 008h, 080h, 000h, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 088h, 002h, 008h
        db  080h, 000h, 006h, 0A0h, 0DEh, 0F6h, 0C8h, 020h, 0E0h, 01Eh, 006h, 0C6h, 0C8h, 020h, 0E0h, 01Eh
        db  006h, 0C8h, 0E3h, 060h, 0E0h, 010h, 004h, 060h, 0DAh, 0AAh, 023h, 0E0h, 0E0h, 014h, 013h, 019h
        db  0C1h, 044h, 002h, 044h, 01Eh, 000h, 016h, 0F5h, 025h, 060h, 0E4h, 06Ah, 016h, 006h, 006h, 020h
        db  006h, 0C8h, 016h, 0EFh, 004h, 0C2h, 004h, 060h, 0EAh, 0F4h, 002h, 008h, 080h, 01Eh, 006h, 0A0h
        db  0DEh, 0F6h, 010h, 007h, 006h, 020h, 006h, 0C6h, 016h, 0E4h, 007h, 083h, 000h, 009h, 006h, 0A0h
        db  0DAh, 0A2h, 004h, 060h, 0DAh, 088h, 004h, 0CEh, 004h, 0E0h, 008h, 0DCh, 0D3h, 0E0h, 0E3h, 0ACh
        db  0C2h, 08Fh, 0C8h, 020h, 0E4h, 072h, 001h, 086h, 048h, 020h, 0E0h, 008h, 001h, 082h, 007h, 086h
        db  000h, 005h, 007h, 084h, 006h, 072h, 0C1h, 054h, 013h, 001h, 02Ch, 0D4h, 002h, 024h, 000h, 00Ah
        db  006h, 006h, 016h, 0F9h, 002h, 008h, 080h, 02Ah, 006h, 0A0h, 0DEh, 0F6h, 02Ch, 020h, 0E0h, 01Ah
        db  004h, 060h, 0D9h, 054h, 006h, 0A0h, 0E1h, 046h, 004h, 0CDh, 023h, 0A0h, 0E0h, 01Ch, 013h, 00Dh
        db  001h, 00Eh, 000h, 003h, 013h, 00Ah, 0E3h, 0A0h, 0E0h, 01Ch, 003h, 0B0h, 0D8h, 020h, 0E0h, 010h
        db  006h, 065h, 0D2h, 0A0h, 0E0h, 026h, 004h, 0CFh, 010h, 008h, 02Dh, 020h, 000h, 001h, 0C0h, 0E0h
        db  008h, 0DCh, 006h, 0A0h, 0E2h, 02Ah, 0D2h, 0A0h, 0E3h, 0ADh, 004h, 060h, 0DAh, 0AAh, 006h, 0A0h
        db  0DFh, 0B8h, 0C2h, 008h, 013h, 019h, 007h, 083h, 080h, 080h, 023h, 0E0h, 0E0h, 014h, 013h, 002h
        db  007h, 083h, 000h, 00Ah, 004h, 060h, 0DAh, 09Eh, 0C1h, 020h, 006h, 006h, 013h, 00Dh, 006h, 0A0h
        db  0DFh, 0B8h, 0C2h, 008h, 013h, 009h, 007h, 083h, 000h, 00Bh, 023h, 0E0h, 0E0h, 014h, 016h, 002h
        db  007h, 083h, 080h, 001h, 004h, 060h, 0DAh, 09Eh, 007h, 083h, 080h, 00Ah, 004h, 060h, 0DAh, 08Ch
        db  093h, 0E0h, 0E0h, 00Eh, 013h, 05Eh, 093h, 0E0h, 0E0h, 010h, 013h, 017h, 023h, 0E0h, 0E0h, 014h
        db  013h, 004h, 007h, 083h, 000h, 007h, 004h, 060h, 0DAh, 09Eh, 007h, 083h, 0A0h, 000h, 006h, 0A0h
        db  0E2h, 02Ah, 007h, 083h, 048h, 000h, 006h, 0A0h, 0E2h, 02Ah, 0D2h, 0A0h, 0E0h, 010h, 0C8h, 020h
        db  0E0h, 01Ch, 006h, 0CAh, 0C8h, 020h, 0E0h, 020h, 006h, 0CCh, 006h, 0A0h, 0E1h, 046h, 002h, 008h
        db  080h, 04Eh, 006h, 0A0h, 0DEh, 0F6h, 023h, 0A0h, 0E0h, 01Ch, 013h, 020h, 088h, 020h, 009h, 01Ah
        db  007h, 00Eh, 016h, 01Ch, 088h, 020h, 009h, 01Ch, 007h, 010h, 016h, 018h, 088h, 020h, 009h, 01Eh
        db  007h, 012h, 016h, 014h, 088h, 020h, 007h, 00Ah, 0E0h, 022h, 013h, 010h, 006h, 020h, 006h, 0CAh
        db  016h, 038h, 0E3h, 0A0h, 0E0h, 020h, 010h, 006h, 023h, 0E0h, 0E0h, 014h, 016h, 0CAh, 0E3h, 0A0h
        db  0E0h, 022h, 004h, 0C2h, 0D2h, 0A0h, 0E3h, 0ACh, 004h, 060h, 0DAh, 092h, 0C8h, 020h, 0E0h, 01Ch
        db  006h, 0CAh, 088h, 0A0h, 006h, 0DCh, 000h, 00Eh, 016h, 010h, 088h, 0A0h, 006h, 0DEh, 000h, 010h
        db  016h, 00Ch, 088h, 0A0h, 006h, 0E0h, 000h, 012h, 016h, 008h, 006h, 020h, 006h, 0CCh, 016h, 019h
        db  0E8h, 020h, 0E0h, 00Eh, 001h, 082h, 0E3h, 0A0h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 020h, 006h, 0CCh
        db  010h, 010h, 023h, 0A0h, 0E0h, 010h, 016h, 008h, 0C1h, 064h, 000h, 006h, 021h, 060h, 0E0h, 00Ch
        db  013h, 008h, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 005h, 088h, 020h, 007h, 00Ah, 007h, 008h, 012h, 096h
        db  010h, 000h, 004h, 060h, 0DAh, 092h, 093h, 0E0h, 0E0h, 00Eh, 013h, 050h, 093h, 0E0h, 0E3h, 0ABh
        db  013h, 04Dh, 093h, 0E0h, 0E3h, 0AAh, 013h, 01Ch, 006h, 0A0h, 0E1h, 0A4h, 023h, 0A0h, 0E0h, 010h
        db  013h, 045h, 023h, 0A0h, 0E0h, 008h, 016h, 006h, 0E3h, 060h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 022h
        db  007h, 006h, 010h, 034h, 023h, 0E0h, 0E0h, 014h, 016h, 031h, 0C1h, 060h, 009h, 018h, 021h, 060h
        db  0E0h, 012h, 016h, 02Ch, 006h, 0A0h, 0E9h, 008h, 010h, 031h, 0D2h, 0A0h, 0E3h, 0AAh, 010h, 02Eh
        db  0E3h, 0A0h, 0E0h, 012h, 006h, 0A0h, 0E9h, 008h, 0C1h, 064h, 000h, 006h, 021h, 060h, 0E0h, 00Ch
        db  013h, 025h, 088h, 020h, 007h, 00Eh, 006h, 0DCh, 016h, 014h, 088h, 020h, 007h, 010h, 006h, 0DEh
        db  016h, 010h, 088h, 020h, 007h, 012h, 006h, 0E0h, 016h, 00Ch, 098h, 020h, 006h, 0CEh, 006h, 0CFh
        db  013h, 015h, 006h, 020h, 006h, 0CEh, 016h, 012h, 0E3h, 060h, 0E0h, 01Ah, 0D2h, 0A0h, 0E0h, 00Ch
        db  010h, 00Dh, 0E3h, 060h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 032h, 007h, 006h, 006h, 0A0h, 0E1h, 046h
        db  002h, 008h, 080h, 048h, 006h, 0A0h, 0DEh, 0F6h, 0D2h, 0A0h, 0E3h, 0ABh, 004h, 060h, 0DAh, 092h
        db  088h, 022h, 000h, 00Eh, 009h, 01Ah, 01Ah, 0C9h, 01Bh, 00Bh, 088h, 022h, 000h, 010h, 009h, 01Ch
        db  01Ah, 0C4h, 01Bh, 006h, 088h, 022h, 000h, 012h, 009h, 01Eh, 01Ah, 0BFh, 01Bh, 001h, 004h, 05Bh
        db  0C1h, 060h, 001h, 06Ch, 002h, 085h, 000h, 043h, 013h, 0E1h, 093h, 0E0h, 0E3h, 0AAh, 016h, 0DEh
        db  0C8h, 0A0h, 0EDh, 04Ah, 000h, 002h, 007h, 084h, 000h, 00Eh, 0C1h, 042h, 0C0h, 0A0h, 001h, 06Ch
        db  0C0h, 002h, 002h, 025h, 000h, 048h, 007h, 081h, 0E9h, 05Ah, 007h, 083h, 0AEh, 014h, 004h, 060h
        db  0ECh, 066h, 002h, 002h, 0FCh, 000h, 010h, 0CAh, 098h, 020h, 0E0h, 00Eh, 006h, 065h, 016h, 003h
        db  0D8h, 020h, 0E3h, 0AAh, 006h, 065h, 093h, 0E0h, 0E3h, 0ABh, 013h, 00Dh, 023h, 0A0h, 0E0h, 008h
        db  016h, 019h, 023h, 0A0h, 0E0h, 010h, 013h, 016h, 0E3h, 060h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 020h
        db  007h, 006h, 006h, 0A0h, 0E1h, 046h, 043h, 0A0h, 0E0h, 018h, 02Eh, 0E0h, 000h, 001h, 0D2h, 0A0h
        db  0E0h, 026h, 007h, 083h, 000h, 010h, 006h, 0A0h, 0E2h, 02Ah, 023h, 0E0h, 0E0h, 014h, 016h, 002h
        db  006h, 0A0h, 0E1h, 028h, 043h, 0A0h, 0E0h, 02Ch, 0C8h, 020h, 0E0h, 020h, 008h, 0D6h, 004h, 060h
        db  0DAh, 092h, 043h, 0A0h, 0E0h, 00Eh, 0C1h, 0A0h, 008h, 0D6h, 013h, 002h, 006h, 020h, 008h, 0D6h
        db  023h, 0E0h, 0E0h, 014h, 013h, 003h, 023h, 0A0h, 0E0h, 008h, 016h, 026h, 002h, 008h, 080h, 042h
        db  006h, 0A0h, 0DEh, 0F6h, 023h, 0A0h, 0E0h, 008h, 016h, 01Fh, 023h, 0A0h, 0E0h, 010h, 016h, 00Ah
        db  0C1h, 022h, 000h, 002h, 025h, 020h, 0E4h, 06Ah, 016h, 023h, 007h, 083h, 080h, 020h, 006h, 0A0h
        db  0E2h, 02Ah, 010h, 012h, 006h, 0A0h, 0E1h, 046h, 023h, 0E0h, 0E0h, 014h, 016h, 002h, 006h, 0A0h
        db  0E1h, 028h, 0E3h, 060h, 0E0h, 01Eh, 0C8h, 020h, 0E0h, 020h, 007h, 006h, 023h, 0A0h, 0E0h, 008h
        db  016h, 003h, 023h, 0A0h, 0E0h, 006h, 013h, 051h, 098h, 020h, 0E0h, 00Eh, 006h, 065h, 016h, 003h
        db  0D8h, 020h, 0E3h, 0AAh, 006h, 065h, 0C1h, 022h, 000h, 002h, 025h, 020h, 0E4h, 06Ah, 013h, 00Eh
        db  007h, 083h, 000h, 020h, 006h, 0A0h, 0E2h, 02Ah, 0C8h, 022h, 000h, 00Eh, 006h, 0E6h, 0C8h, 022h
        db  000h, 010h, 006h, 0E8h, 0C8h, 022h, 000h, 012h, 006h, 0EAh, 010h, 037h, 088h, 022h, 000h, 00Eh
        db  006h, 0DCh, 016h, 008h, 088h, 022h, 000h, 010h, 006h, 0DEh, 016h, 004h, 088h, 022h, 000h, 012h
        db  006h, 0E0h, 013h, 00Bh, 0C8h, 022h, 000h, 00Eh, 006h, 0DCh, 0C8h, 022h, 000h, 010h, 006h, 0DEh
        db  0C8h, 022h, 000h, 012h, 006h, 0E0h, 0E3h, 060h, 0E0h, 014h, 023h, 0A0h, 0E0h, 00Eh, 016h, 008h
        db  0C1h, 0A0h, 008h, 0D6h, 016h, 01Ah, 007h, 086h, 010h, 000h, 0E8h, 006h, 006h, 0D2h, 010h, 015h
        db  0E3h, 0A0h, 0E0h, 00Eh, 023h, 0A0h, 0E0h, 008h, 016h, 009h, 0E3h, 0A0h, 0E0h, 006h, 004h, 0E0h
        db  006h, 0E6h, 004h, 0E0h, 006h, 0E8h, 004h, 0E0h, 006h, 0EAh, 010h, 007h, 002h, 008h, 080h, 036h
        db  007h, 0A0h, 006h, 0D6h, 0DEh, 0C0h, 006h, 0A0h, 0DEh, 0F6h, 004h, 060h, 0DAh, 092h, 098h, 020h
        db  006h, 065h, 0E0h, 010h, 016h, 003h, 0D8h, 020h, 0E0h, 00Eh, 006h, 065h, 004h, 060h, 0DAh, 092h
        db  023h, 0E0h, 0E0h, 014h, 013h, 002h, 004h, 060h, 0DAh, 092h, 010h, 02Eh, 003h, 0B0h, 098h, 020h
        db  0E3h, 0ABh, 006h, 06Fh, 016h, 019h, 0C2h, 024h, 000h, 008h, 011h, 016h, 0E3h, 0E0h, 0E0h, 014h
        db  007h, 083h, 000h, 000h, 006h, 0A0h, 0E2h, 02Ah, 023h, 0A0h, 0E0h, 014h, 013h, 004h, 002h, 008h
        db  080h, 006h, 006h, 0A0h, 0DEh, 0F6h, 002h, 008h, 000h, 01Eh, 006h, 0A0h, 0DEh, 0F6h, 023h, 0A0h
        db  0E0h, 008h, 013h, 002h, 006h, 0A0h, 0E1h, 028h, 0C0h, 082h, 016h, 002h, 004h, 060h, 0DAh, 0AAh
        db  0E8h, 020h, 0EDh, 066h, 006h, 0F0h, 099h, 020h, 0E0h, 00Eh, 000h, 016h, 016h, 005h, 004h, 0E0h
        db  006h, 0ECh, 048h, 020h, 0E0h, 014h, 006h, 0F0h, 007h, 083h, 000h, 001h, 004h, 060h, 0DAh, 08Ch
        db  0C2h, 064h, 000h, 014h, 002h, 024h, 000h, 018h, 0C1h, 0C4h, 061h, 0C2h, 002h, 027h, 0FFh, 0FCh
        db  0C1h, 074h, 0C1h, 085h, 071h, 045h, 002h, 085h, 000h, 027h, 016h, 046h, 0C1h, 054h, 002h, 045h
        db  0FFh, 0CFh, 016h, 042h, 004h, 0C8h, 0C1h, 064h, 000h, 008h, 015h, 006h, 013h, 005h, 0C2h, 024h
        db  000h, 00Eh, 002h, 048h, 01Fh, 000h, 006h, 0C8h, 002h, 028h, 000h, 011h, 0A2h, 004h, 098h, 018h
        db  0EDh, 06Bh, 016h, 032h, 0C1h, 042h, 002h, 025h, 000h, 004h, 065h, 047h, 0C2h, 035h, 0CDh, 074h
        db  006h, 048h, 015h, 0FDh, 0C0h, 0A0h, 001h, 06Ch, 0C1h, 002h, 0C8h, 004h, 001h, 06Ch, 0C1h, 0A0h
        db  0FCh, 000h, 013h, 005h, 0C8h, 020h, 0EBh, 002h, 0FCh, 002h, 0C1h, 006h, 010h, 0F6h, 0C8h, 002h
        db  001h, 06Ch, 0C8h, 020h, 0EDh, 04Ah, 0FCh, 002h, 007h, 081h, 0EBh, 0D0h, 0C0h, 004h, 007h, 083h
        db  002h, 010h, 007h, 084h, 000h, 00Eh, 010h, 03Bh, 007h, 084h, 000h, 00Ch, 0C0h, 0E2h, 000h, 008h
        db  002h, 005h, 0FCh, 000h, 0C0h, 0A0h, 001h, 06Ch, 0C0h, 002h, 0C1h, 0A0h, 0FCh, 000h, 013h, 02Fh
        db  004h, 0E0h, 0FCh, 000h, 02Eh, 016h, 010h, 02Bh, 0C8h, 0A0h, 0EDh, 06Ch, 000h, 00Eh, 0C8h, 0A0h
        db  0EDh, 06Eh, 000h, 010h, 0C8h, 0A0h, 0EDh, 070h, 000h, 012h, 007h, 083h, 080h, 006h, 004h, 060h
        db  0DAh, 08Ch, 004h, 060h, 0DAh, 0AAh, 007h, 084h, 000h, 010h, 007h, 085h, 000h, 034h, 010h, 009h
        db  007h, 084h, 000h, 012h, 007h, 085h, 000h, 032h, 010h, 004h, 007h, 084h, 000h, 014h, 007h, 085h
        db  000h, 038h, 006h, 0A0h, 0DFh, 0DEh, 0C8h, 085h, 000h, 004h, 006h, 0A0h, 0E0h, 020h, 0C8h, 0A0h
        db  0EDh, 04Ah, 000h, 002h, 0C0h, 0A0h, 001h, 06Ch, 0C0h, 002h, 006h, 048h, 0C1h, 048h, 006h, 0A0h
        db  0E2h, 0A2h, 002h, 007h, 006h, 0A2h, 0C5h, 060h, 0FCh, 002h, 002h, 025h, 0FFh, 0F4h, 0C8h, 005h
        db  0FCh, 002h, 0C2h, 020h, 001h, 06Ch, 006h, 0A0h, 0C1h, 096h, 004h, 060h, 0DAh, 088h, 006h, 0A0h
        db  0E2h, 0A2h, 002h, 007h, 006h, 0B4h, 010h, 0EFh, 0C3h, 00Bh, 003h, 000h, 000h, 002h, 007h, 082h
        db  000h, 0C0h, 0C8h, 020h, 000h, 00Ch, 000h, 0C0h, 0C8h, 020h, 000h, 00Eh, 000h, 0C2h, 0C8h, 020h
        db  000h, 010h, 000h, 0C4h, 0C8h, 020h, 000h, 012h, 000h, 0C6h, 0C8h, 020h, 000h, 014h, 000h, 0C8h
        db  0C8h, 020h, 000h, 016h, 000h, 0CAh, 0C8h, 020h, 000h, 004h, 000h, 0CCh, 0C8h, 020h, 000h, 006h
        db  000h, 0CEh, 0C8h, 002h, 000h, 00Ch, 007h, 0A0h, 000h, 00Eh, 0EDh, 010h, 0C8h, 002h, 000h, 010h
        db  007h, 0A0h, 000h, 012h, 0EDh, 01Ah, 0C8h, 002h, 000h, 014h, 007h, 0A0h, 000h, 016h, 0EDh, 04Ah
        db  0C8h, 002h, 000h, 004h, 007h, 0A0h, 000h, 006h, 0EDh, 060h, 001h, 060h, 001h, 01Ch, 000h, 004h
        db  016h, 009h, 001h, 0E0h, 001h, 040h, 008h, 000h, 001h, 0A0h, 001h, 040h, 040h, 000h, 001h, 0E0h
        db  001h, 040h, 004h, 000h, 006h, 0A0h, 0F0h, 020h, 002h, 005h, 080h, 000h, 0D8h, 005h, 004h, 080h
        db  004h, 0C7h, 003h, 000h, 000h, 00Fh, 007h, 088h, 010h, 000h, 002h, 009h, 020h, 000h, 007h, 08Ah
        db  0EDh, 078h, 002h, 003h, 0E6h, 000h, 004h, 05Ah, 003h, 000h, 000h, 000h, 0D2h, 020h, 001h, 087h
        db  010h, 006h, 003h, 000h, 000h, 000h, 0C2h, 020h, 001h, 08Ah, 002h, 008h, 01Ah, 000h, 0C2h, 060h
        db  000h, 0AEh, 0DAh, 048h, 004h, 080h, 005h, 089h, 002h, 089h, 000h, 006h, 015h, 007h, 007h, 088h
        db  080h, 000h, 0DAh, 048h, 004h, 080h, 0C8h, 009h, 000h, 0AEh, 003h, 080h, 002h, 0E0h, 000h, 0A0h
        db  004h, 05Ch, 003h, 000h, 000h, 000h, 001h, 060h, 001h, 09Ch, 000h, 020h, 013h, 0E2h, 0C2h, 020h
        db  001h, 08Ch, 002h, 008h, 01Ch, 000h, 010h, 0E3h, 003h, 000h, 000h, 000h, 001h, 060h, 001h, 040h
        db  040h, 000h, 016h, 0ECh, 001h, 0A0h, 001h, 040h, 040h, 000h, 002h, 008h, 002h, 000h, 010h, 0D7h
        db  0C0h, 0B3h, 006h, 092h, 010h, 0FDh, 0C0h, 0B3h, 0C0h, 048h, 0CCh, 072h, 0CCh, 072h, 0C1h, 032h
        db  0CCh, 044h, 0DCh, 072h, 006h, 004h, 016h, 0FDh, 004h, 05Bh, 0C0h, 048h, 002h, 002h, 0E9h, 092h
        db  007h, 084h, 000h, 006h, 010h, 0F6h, 002h, 002h, 0E5h, 0E0h, 0C0h, 049h, 007h, 084h, 000h, 006h
        db  010h, 0F0h, 0C0h, 0B3h, 0C1h, 032h, 002h, 001h, 000h, 001h, 0D0h, 044h, 006h, 0C1h, 002h, 044h
        db  000h, 0FFh, 010h, 0E7h, 0C1h, 033h, 0C0h, 073h, 0D1h, 044h, 002h, 044h, 000h, 0FFh, 0DCh, 045h
        db  006h, 004h, 016h, 0FDh, 004h, 05Ah, 006h, 0A0h, 0EFh, 0A0h, 0C8h, 033h, 001h, 09Eh, 004h, 05Ah
        db  006h, 0A0h, 0EDh, 09Eh, 0C1h, 089h, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ah
        db  0C1h, 088h, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ch, 004h, 0C2h, 0C1h, 0C7h
        db  016h, 003h, 006h, 002h, 016h, 0FCh, 010h, 04Dh, 004h, 05Ah, 006h, 0A0h, 0EEh, 0EAh, 0C1h, 089h
        db  009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ah, 0C1h, 088h, 009h, 0A6h, 002h, 066h
        db  000h, 040h, 0C8h, 006h, 001h, 08Ch, 0C8h, 033h, 001h, 09Eh, 010h, 0E8h, 08Ah, 033h, 000h, 002h
        db  016h, 038h, 08Ah, 073h, 000h, 002h, 016h, 035h, 004h, 05Ah, 08Ah, 020h, 0E9h, 08Ch, 000h, 002h
        db  016h, 030h, 08Ah, 060h, 0E9h, 090h, 000h, 002h, 016h, 02Ch, 007h, 082h, 0EAh, 036h, 010h, 001h
        db  0C0h, 0B3h, 002h, 004h, 004h, 080h, 0D1h, 052h, 013h, 003h, 09Dh, 032h, 016h, 022h, 010h, 0FBh
        db  007h, 085h, 080h, 000h, 0D8h, 005h, 004h, 080h, 004h, 0C7h, 004h, 05Ah, 0C8h, 020h, 000h, 0C0h
        db  000h, 00Ch, 0C8h, 020h, 000h, 0C2h, 000h, 00Eh, 0C8h, 020h, 000h, 0C4h, 000h, 010h, 0C8h, 020h
        db  000h, 0C6h, 000h, 012h, 0C8h, 020h, 000h, 0C8h, 000h, 014h, 0C8h, 020h, 000h, 0CAh, 000h, 016h
        db  0C8h, 020h, 000h, 0CCh, 000h, 004h, 0C8h, 020h, 000h, 0CEh, 000h, 006h, 003h, 000h, 000h, 00Fh
        db  005h, 0CCh, 004h, 05Ch, 004h, 0E0h, 001h, 082h, 002h, 002h, 0E5h, 0DAh, 0C8h, 032h, 001h, 082h
        db  0C8h, 032h, 001h, 080h, 006h, 0A0h, 0EEh, 0B6h, 0C8h, 012h, 001h, 082h, 0C2h, 0CAh, 007h, 084h
        db  007h, 0D0h, 004h, 0E0h, 001h, 084h, 006h, 004h, 016h, 0FCh, 0C1h, 020h, 001h, 084h, 016h, 0E9h
        db  002h, 004h, 000h, 032h, 007h, 085h, 080h, 000h, 0D8h, 005h, 004h, 080h, 004h, 0C7h, 0C1h, 060h
        db  001h, 086h, 006h, 004h, 016h, 0FCh, 0C1h, 020h, 001h, 084h, 004h, 05Bh, 0C0h, 0B3h, 0C4h, 0B3h
        db  004h, 05Bh, 0C0h, 048h, 0C0h, 0B3h, 0A0h, 073h, 0C4h, 042h, 004h, 05Bh, 088h, 033h, 001h, 084h
        db  016h, 0E6h, 004h, 05Ah, 0C1h, 089h, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ah
        db  004h, 05Bh, 004h, 0C5h, 007h, 0A0h, 001h, 09Ch, 000h, 040h, 001h, 060h, 001h, 09Ch, 000h, 040h
        db  013h, 003h, 006h, 005h, 016h, 0F7h, 004h, 05Ch, 004h, 05Bh, 006h, 0A0h, 0EFh, 03Eh, 0C0h, 060h
        db  001h, 040h, 0C8h, 005h, 001h, 040h, 0C5h, 002h, 0C8h, 001h, 001h, 040h, 004h, 05Ah, 006h, 0A0h
        db  0EFh, 03Eh, 0A1h, 008h, 010h, 0F4h, 0C0h, 0B3h, 0C1h, 033h, 0C1h, 060h, 001h, 040h, 001h, 085h
        db  004h, 000h, 001h, 0C5h, 010h, 000h, 004h, 05Bh, 0C1h, 008h, 0C2h, 009h, 0C2h, 044h, 004h, 05Bh
        db  002h, 005h, 000h, 0C8h, 006h, 005h, 016h, 0FEh, 004h, 05Bh, 0C1h, 033h, 0C0h, 003h, 0C0h, 0C4h
        db  004h, 05Bh, 0C0h, 0C0h, 004h, 05Bh, 094h, 0E0h, 001h, 09Eh, 016h, 0C2h, 005h, 0C3h, 004h, 05Bh
        db  0C0h, 073h, 006h, 0A0h, 0EFh, 0B8h, 002h, 02Dh, 000h, 008h, 007h, 085h, 000h, 008h, 09Fh, 071h
        db  016h, 0B7h, 006h, 005h, 016h, 0FCh, 004h, 05Ah, 002h, 002h, 0E5h, 0E6h, 004h, 060h, 0EDh, 0A2h
        db  08Ch, 0E9h, 000h, 004h, 016h, 0ADh, 004h, 05Bh, 0C1h, 020h, 001h, 080h, 007h, 085h, 007h, 0D0h
        db  001h, 0E0h, 001h, 080h, 004h, 000h, 006h, 045h, 016h, 0FEh, 0C8h, 004h, 001h, 080h, 004h, 05Bh
        db  0C1h, 033h, 0C3h, 048h, 0C1h, 004h, 013h, 004h, 002h, 02Dh, 004h, 000h, 006h, 004h, 016h, 0FCh
        db  004h, 05Bh, 0C3h, 08Dh, 006h, 0A0h, 0EFh, 0B8h, 0C1h, 08Dh, 009h, 0A6h, 002h, 066h, 000h, 040h
        db  0C7h, 086h, 004h, 05Ah, 0C1h, 08Dh, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ah
        db  004h, 05Bh, 0C1h, 08Dh, 009h, 0A6h, 002h, 066h, 000h, 040h, 0C8h, 006h, 001h, 08Ch, 004h, 05Bh
        db  0C0h, 04Dh, 002h, 004h, 000h, 028h, 007h, 085h, 000h, 000h, 0F0h, 000h, 006h, 0F8h, 055h, 000h
        db  004h, 060h, 0EDh, 0C6h, 0C0h, 04Dh, 0C0h, 0B3h, 0C1h, 032h, 004h, 060h, 0EDh, 08Ah, 0C1h, 033h
        db  001h, 060h, 001h, 01Ch, 000h, 004h, 016h, 001h, 004h, 05Bh, 0C0h, 0C4h, 004h, 05Bh, 007h, 089h
        db  0E5h, 028h, 0C2h, 039h, 013h, 007h, 0C6h, 039h, 086h, 039h, 016h, 025h, 0C6h, 039h, 086h, 039h
        db  016h, 022h, 010h, 0F7h, 002h, 002h, 0F0h, 03Eh, 004h, 0C4h, 004h, 0C5h, 0C2h, 039h, 013h, 002h
        db  004h, 060h, 0F0h, 07Ah, 002h, 002h, 0F0h, 04Ch, 004h, 0C4h, 0C2h, 039h, 013h, 003h, 0C1h, 079h
        db  004h, 060h, 0F0h, 07Ah, 002h, 002h, 0F0h, 05Ch, 004h, 0C5h, 0C2h, 039h, 013h, 003h, 0C1h, 039h
        db  004h, 060h, 0F0h, 07Ah, 0C0h, 079h, 0C0h, 0B9h, 060h, 081h, 005h, 0C2h, 009h, 012h, 004h, 0F1h
        db  006h, 002h, 016h, 0FDh, 004h, 05Bh, 004h, 05Ch, 002h, 001h, 0AAh, 0AAh, 0C6h, 001h, 0E0h, 044h
        db  040h, 045h, 080h, 058h, 016h, 0F8h, 002h, 001h, 000h, 014h, 006h, 001h, 016h, 0FEh, 002h, 001h
        db  055h, 055h, 0C6h, 001h, 0E0h, 044h, 040h, 045h, 080h, 058h, 016h, 0EDh, 004h, 052h, 0C0h, 048h
        db  0C0h, 089h, 060h, 081h, 005h, 0C2h, 004h, 05Bh, 0C3h, 00Bh, 006h, 0A0h, 0F0h, 0A0h, 0CCh, 041h
        db  006h, 042h, 016h, 0FDh, 006h, 0A0h, 0F0h, 0A0h, 0C1h, 001h, 08Ch, 044h, 016h, 012h, 005h, 0C4h
        db  006h, 042h, 016h, 0FBh, 002h, 004h, 0F0h, 0E6h, 002h, 003h, 001h, 001h, 006h, 094h, 002h, 003h
        db  05Ah, 05Ah, 006h, 094h, 005h, 043h, 006h, 094h, 007h, 003h, 006h, 094h, 004h, 0C3h, 006h, 094h
        db  005h, 0CCh, 004h, 05Ch, 0C1h, 0CBh, 006h, 0A0h, 0F0h, 0A0h, 0CCh, 043h, 006h, 042h, 016h, 0FDh
        db  006h, 0A0h, 0F0h, 0A0h, 08Ch, 043h, 016h, 0F5h, 006h, 042h, 016h, 0FCh, 004h, 057h, 0C2h, 08Bh
        db  0C0h, 008h, 0C1h, 049h, 005h, 085h, 002h, 080h, 000h, 040h, 011h, 003h, 002h, 080h, 000h, 04Fh
        db  012h, 045h, 002h, 001h, 0F0h, 0A0h, 009h, 0A1h, 080h, 001h, 013h, 040h, 002h, 001h, 0F1h, 0D0h
        db  009h, 0A1h, 080h, 001h, 013h, 03Bh, 0C0h, 060h, 000h, 006h, 009h, 0A1h, 080h, 001h, 013h, 036h
        db  005h, 081h, 080h, 001h, 013h, 033h, 0C0h, 04Ah, 009h, 0A1h, 080h, 001h, 013h, 02Fh, 0C8h, 000h
        db  001h, 06Ah, 002h, 080h, 000h, 080h, 014h, 017h, 002h, 001h, 0F8h, 000h, 0C1h, 0A0h, 001h, 040h
        db  001h, 0A0h, 001h, 040h, 004h, 000h, 002h, 002h, 010h, 000h, 002h, 003h, 004h, 000h, 0CCh, 0B1h
        db  006h, 043h, 016h, 0FDh, 001h, 0A0h, 001h, 040h, 040h, 000h, 002h, 008h, 0F8h, 010h, 0C8h, 006h
        db  001h, 040h, 0C0h, 000h, 013h, 002h, 002h, 008h, 0F8h, 000h, 002h, 009h, 0FBh, 0FEh, 006h, 0A0h
        db  0F0h, 0AAh, 010h, 025h, 002h, 080h, 000h, 080h, 014h, 009h, 002h, 001h, 0F8h, 000h, 002h, 002h
        db  010h, 000h, 002h, 003h, 004h, 000h, 0CCh, 072h, 006h, 043h, 016h, 0FDh, 005h, 080h, 002h, 080h
        db  000h, 080h, 012h, 004h, 001h, 060h, 001h, 004h, 000h, 020h, 013h, 005h, 081h, 040h, 016h, 0ABh
        db  002h, 080h, 000h, 080h, 014h, 00Bh, 007h, 0A0h, 001h, 06Ah, 000h, 07Eh, 002h, 002h, 010h, 000h
        db  002h, 003h, 004h, 000h, 004h, 0C1h, 0CCh, 081h, 006h, 043h, 016h, 0FDh, 005h, 0CAh, 004h, 05Ah
        db  002h, 000h, 0F1h, 0C2h, 002h, 001h, 0F2h, 0F2h, 002h, 040h, 0FCh, 000h, 002h, 041h, 0FCh, 000h
        db  080h, 040h, 013h, 004h, 007h, 0A0h, 001h, 004h, 000h, 03Ch, 004h, 05Bh, 004h, 0C0h, 002h, 001h
        db  000h, 008h, 002h, 002h, 012h, 000h, 0C1h, 0E0h, 001h, 040h, 001h, 0A0h, 001h, 040h, 004h, 000h
        db  002h, 003h, 001h, 000h, 0C8h, 000h, 001h, 06Ah, 0CCh, 0A0h, 0F8h, 010h, 005h, 080h, 006h, 003h
        db  016h, 0F9h, 002h, 022h, 002h, 000h, 006h, 001h, 016h, 0F3h, 001h, 0A0h, 001h, 040h, 040h, 000h
        db  0C8h, 007h, 001h, 040h, 002h, 000h, 008h, 000h, 0C0h, 040h, 006h, 001h, 0C8h, 001h, 001h, 06Ah
        db  002h, 061h, 080h, 000h, 0C8h, 001h, 0F8h, 010h, 006h, 000h, 016h, 0F6h, 004h, 0C0h, 004h, 0C8h
        db  004h, 0C9h, 002h, 003h, 008h, 000h, 0C8h, 000h, 001h, 06Ah, 0C1h, 080h, 002h, 066h, 080h, 000h
        db  0C1h, 020h, 0F8h, 010h, 081h, 006h, 016h, 015h, 0C2h, 008h, 013h, 006h, 005h, 080h, 006h, 003h
        db  016h, 0F2h, 0C2h, 008h, 013h, 00Dh, 010h, 019h, 007h, 0A0h, 0F8h, 010h, 055h, 055h, 0C1h, 020h
        db  0F8h, 010h, 002h, 084h, 055h, 055h, 016h, 002h, 0C2h, 006h, 010h, 0F0h, 081h, 006h, 013h, 0EEh
        db  004h, 05Bh, 007h, 0A0h, 0F8h, 010h, 055h, 055h, 0C1h, 060h, 0F8h, 010h, 081h, 005h, 013h, 003h
        db  002h, 085h, 055h, 055h, 016h, 0F5h, 0C2h, 008h, 013h, 0E1h, 0C2h, 040h, 006h, 009h, 002h, 048h
        db  007h, 0FFh, 004h, 0C0h, 002h, 001h, 000h, 008h, 002h, 002h, 012h, 000h, 002h, 003h, 001h, 000h
        db  0C8h, 000h, 001h, 06Ah, 0C8h, 032h, 0F8h, 010h, 005h, 080h, 006h, 003h, 016h, 0F9h, 002h, 022h
        db  002h, 000h, 006h, 001h, 016h, 0F3h, 002h, 088h, 000h, 040h, 015h, 013h, 002h, 089h, 000h, 04Fh
        db  011h, 010h, 004h, 0C0h, 002h, 002h, 012h, 000h, 002h, 001h, 000h, 008h, 002h, 003h, 001h, 000h
        db  0CCh, 080h, 006h, 003h, 016h, 0FDh, 002h, 022h, 002h, 000h, 006h, 001h, 016h, 0F7h, 005h, 0CBh
        db  004h, 05Bh, 007h, 0A0h, 001h, 004h, 000h, 037h, 004h, 05Bh, 002h, 0E0h, 000h, 0A0h, 007h, 088h
        db  000h, 0C0h, 002h, 009h, 0F3h, 050h, 007h, 084h, 0E5h, 0ECh, 002h, 005h, 000h, 001h, 0C2h, 08Bh
        db  004h, 0CCh, 006h, 0A0h, 0F3h, 05Ah, 02Ch, 060h, 000h, 001h, 006h, 099h, 02Ch, 0A0h, 000h, 002h
        db  006h, 099h, 02Dh, 020h, 000h, 004h, 006h, 099h, 02Eh, 020h, 000h, 008h, 006h, 099h, 02Fh, 0A0h
        db  000h, 010h, 005h, 08Ch, 016h, 009h, 0CCh, 080h, 0C4h, 081h, 007h, 083h, 0F3h, 09Eh, 0C0h, 088h
        db  004h, 002h, 005h, 08Ch, 016h, 001h, 010h, 033h, 002h, 0E0h, 000h, 0A0h, 004h, 05Ah, 005h, 08Ch
        db  016h, 0FBh, 0CCh, 080h, 0C4h, 081h, 00Ah, 015h, 0C0h, 0B4h, 0C0h, 012h, 0CCh, 088h, 0C0h, 052h
        db  0C4h, 0B4h, 006h, 042h, 004h, 05Bh, 007h, 02Dh, 000h, 018h, 08Bh, 041h, 000h, 00Ah, 016h, 0ECh
        db  082h, 0C1h, 016h, 0EAh, 002h, 0C2h, 002h, 042h, 002h, 000h, 016h, 0E6h, 003h, 080h, 007h, 081h
        db  000h, 001h, 010h, 0F1h, 002h, 001h, 000h, 002h, 010h, 0EEh, 002h, 001h, 000h, 004h, 010h, 0EBh
        db  002h, 001h, 000h, 008h, 010h, 0E8h, 002h, 001h, 000h, 010h, 010h, 0E5h, 002h, 0A1h, 08Bh, 041h
        db  000h, 010h, 013h, 002h, 004h, 060h, 0F3h, 04Ah, 007h, 02Dh, 000h, 018h, 003h, 080h, 002h, 009h
        db  008h, 000h, 002h, 003h, 000h, 004h, 004h, 0C7h, 006h, 0A0h, 0F4h, 0CAh, 001h, 060h, 001h, 01Ch
        db  000h, 004h, 016h, 01Ch, 001h, 0A0h, 001h, 040h, 008h, 000h, 001h, 0E0h, 001h, 040h, 010h, 000h
        db  002h, 004h, 000h, 001h, 0CEh, 044h, 006h, 0C4h, 0C6h, 044h, 001h, 0A0h, 001h, 040h, 010h, 000h
        db  006h, 049h, 001h, 0A0h, 001h, 040h, 040h, 000h, 001h, 0E0h, 001h, 040h, 004h, 000h, 001h, 0E0h
        db  001h, 040h, 008h, 000h, 006h, 0A0h, 0F5h, 068h, 006h, 0A0h, 0F5h, 068h, 005h, 0C7h, 002h, 004h
        db  0E4h, 0A6h, 004h, 0E0h, 003h, 0D0h, 0C1h, 074h, 0C1h, 0B4h, 005h, 086h, 013h, 01Ch, 002h, 0E0h
        db  000h, 0C0h, 002h, 000h, 000h, 000h, 0C0h, 040h, 0C0h, 080h, 0C0h, 0C0h, 0C1h, 000h, 0C1h, 040h
        db  0C1h, 080h, 0C1h, 0C0h, 0C2h, 000h, 0C2h, 040h, 0C2h, 080h, 0C2h, 0C0h, 0C3h, 000h, 0C3h, 040h
        db  0C3h, 080h, 0C3h, 0C0h, 004h, 0A0h, 000h, 0AAh, 003h, 0D0h, 003h, 0D0h, 010h, 03Fh, 005h, 085h
        db  081h, 085h, 013h, 0E1h, 010h, 0E4h, 005h, 0C7h, 002h, 005h, 07Fh, 0FFh, 0A1h, 045h, 003h, 0D0h
        db  003h, 0D0h, 010h, 034h, 0CCh, 0C0h, 0C4h, 0C1h, 002h, 003h, 000h, 028h, 006h, 0A0h, 0F4h, 0CAh
        db  001h, 0E0h, 001h, 042h, 010h, 000h, 005h, 0C7h, 003h, 0D0h, 003h, 0D0h, 010h, 027h, 005h, 0C7h
        db  0C1h, 0A0h, 001h, 04Ah, 007h, 0A0h, 001h, 04Ah, 00Eh, 000h, 001h, 0A0h, 001h, 040h, 002h, 000h
        db  007h, 020h, 001h, 044h, 0C1h, 060h, 001h, 044h, 002h, 085h, 0FFh, 000h, 016h, 017h, 001h, 0E0h
        db  001h, 040h, 022h, 000h, 002h, 005h, 000h, 080h, 006h, 005h, 003h, 0D0h, 016h, 0FDh, 0C1h, 060h
        db  001h, 046h, 002h, 085h, 0FFh, 000h, 013h, 00Ah, 002h, 005h, 021h, 0DDh, 006h, 005h, 010h, 000h
        db  010h, 000h, 010h, 000h, 010h, 000h, 010h, 000h, 003h, 0D0h, 016h, 0F8h, 010h, 051h, 0C8h, 006h
        db  001h, 04Ah, 0CCh, 0C0h, 0C4h, 0C1h, 010h, 04Bh, 0C0h, 013h, 0CCh, 0C8h, 0C0h, 053h, 002h, 002h
        db  0F4h, 0DAh, 0C4h, 0C2h, 006h, 043h, 004h, 05Bh, 0C0h, 060h, 000h, 0AEh, 002h, 0C4h, 002h, 044h
        db  000h, 00Fh, 088h, 044h, 0E4h, 08Ch, 016h, 03Ch, 002h, 081h, 000h, 008h, 013h, 027h, 0C1h, 021h
        db  0E4h, 09Eh, 0C1h, 014h, 021h, 021h, 0E4h, 07Ch, 016h, 033h, 0C1h, 021h, 0E4h, 084h, 002h, 081h
        db  000h, 000h, 013h, 00Bh, 002h, 00Dh, 000h, 0A0h, 083h, 084h, 013h, 009h, 005h, 0C4h, 083h, 084h
        db  013h, 006h, 005h, 0C4h, 083h, 084h, 013h, 003h, 010h, 023h, 081h, 00Eh, 016h, 021h, 0C1h, 021h
        db  0E4h, 09Eh, 045h, 021h, 0E4h, 07Ch, 001h, 0E0h, 001h, 042h, 010h, 000h, 001h, 0A0h, 001h, 042h
        db  010h, 000h, 0C3h, 0A1h, 0E4h, 096h, 002h, 00Fh, 000h, 02Fh, 003h, 080h, 001h, 0A0h, 001h, 040h
        db  002h, 000h, 001h, 0A0h, 001h, 040h, 080h, 000h, 0C0h, 06Dh, 000h, 00Ah, 013h, 009h, 002h, 081h
        db  000h, 00Ah, 015h, 006h, 002h, 00Eh, 0F4h, 0C0h, 002h, 00Fh, 000h, 00Fh, 003h, 080h, 005h, 0CAh
        db  002h, 0E0h, 000h, 0A0h, 004h, 05Ah, 0CEh, 059h, 088h, 020h, 0E4h, 0A6h, 0E4h, 0A6h, 010h, 0F8h
        db  004h, 0C1h, 062h, 048h, 005h, 089h, 0C0h, 0A0h, 001h, 06Ch, 0C8h, 008h, 001h, 06Ch, 002h, 003h
        db  0FCh, 000h, 002h, 004h, 002h, 000h, 0A0h, 073h, 006h, 004h, 016h, 0FDh, 005h, 088h, 006h, 009h
        db  016h, 0F4h, 0C8h, 002h, 001h, 06Ch, 086h, 081h, 016h, 001h, 005h, 0CBh, 004h, 05Bh, 028h, 043h
        db  029h, 031h, 039h, 038h, 033h, 02Dh, 038h, 038h, 020h, 042h, 059h, 020h, 054h, 049h, 0C2h, 04Bh
        db  002h, 0A8h, 000h, 098h, 007h, 083h, 000h, 002h, 002h, 028h, 000h, 008h, 0C6h, 023h, 0E4h, 0F8h
        db  006h, 048h, 0C0h, 0C4h, 00Ah, 073h, 017h, 065h, 006h, 0A0h, 0F6h, 096h, 0C1h, 0C8h, 005h, 0C7h
        db  002h, 003h, 000h, 0A5h, 003h, 0B0h, 0CDh, 0F8h, 0CDh, 0F8h, 002h, 0A6h, 062h, 006h, 002h, 088h
        db  000h, 00Ah, 016h, 057h, 029h, 003h, 016h, 055h, 029h, 005h, 080h, 0C4h, 016h, 052h, 009h, 015h
        db  017h, 050h, 009h, 015h, 018h, 04Eh, 002h, 085h, 000h, 029h, 016h, 04Bh, 005h, 0C6h, 000h, 096h
        db  007h, 003h, 004h, 0C4h, 006h, 045h, 000h, 095h, 005h, 044h, 016h, 043h, 081h, 044h, 016h, 041h
        db  003h, 000h, 000h, 005h, 002h, 0C4h, 003h, 000h, 000h, 00Ah, 002h, 044h, 000h, 00Fh, 002h, 084h
        db  000h, 005h, 016h, 037h, 002h, 0C4h, 003h, 000h, 000h, 00Fh, 002h, 044h, 000h, 00Fh, 002h, 084h
        db  000h, 00Ah, 016h, 02Fh, 002h, 004h, 0FFh, 0FEh, 013h, 02Ch, 015h, 02Bh, 01Ah, 02Ah, 005h, 084h
        db  012h, 028h, 015h, 027h, 01Ah, 026h, 018h, 025h, 005h, 084h, 016h, 023h, 01Bh, 022h, 017h, 021h
        db  005h, 084h, 013h, 01Fh, 01Ah, 01Eh, 011h, 01Dh, 006h, 004h, 016h, 01Bh, 002h, 0A5h, 0C1h, 0C5h
        db  002h, 025h, 000h, 006h, 002h, 003h, 0A5h, 0A5h, 0C1h, 083h, 000h, 095h, 038h, 003h, 000h, 094h
        db  002h, 083h, 06Bh, 02Eh, 016h, 00Eh, 002h, 084h, 01Ch, 059h, 016h, 00Bh, 002h, 024h, 000h, 069h
        db  000h, 095h, 03Ch, 003h, 000h, 094h, 081h, 083h, 016h, 004h, 002h, 084h, 000h, 069h, 016h, 001h
        db  005h, 0C9h, 004h, 059h, 0D0h, 0C3h, 013h, 0FDh, 01Ch, 001h, 010h, 0FBh, 090h, 0E0h, 0E4h, 0FFh
        db  016h, 0F8h, 006h, 0C3h, 0D0h, 0C3h, 01Ch, 0F5h, 016h, 0F4h, 090h, 0E0h, 0E4h, 0FCh, 016h, 0F1h
        db  004h, 05Bh, 0C3h, 00Bh, 002h, 009h, 0E5h, 000h, 006h, 0A0h, 0F0h, 024h, 005h, 0CCh, 004h, 05Ch
        db  007h, 088h, 0B0h, 000h, 007h, 089h, 0FFh, 0FEh, 009h, 0A8h, 009h, 0A9h, 007h, 08Ah, 0E0h, 002h
        db  006h, 0A0h, 0F5h, 072h, 000h, 000h, 007h, 088h, 0A0h, 000h, 007h, 089h, 0AFh, 0FEh, 009h, 0A8h
        db  009h, 0A9h, 007h, 08Ah, 0E0h, 07Ah, 006h, 0A0h, 0F5h, 072h, 000h, 000h, 006h, 0A0h, 0F5h, 0B0h
        db  000h, 000h, 010h, 0E6h, 010h, 0E5h, 000h, 000h, 0F6h, 0F8h, 008h, 0C1h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 001h, 0D2h, 0E2h, 023h, 061h, 0E5h, 080h, 000h, 040h, 000h, 020h, 000h, 010h, 000h
        db  008h, 000h, 004h, 000h, 002h, 000h, 001h, 000h, 000h, 080h, 000h, 040h, 000h, 020h, 000h, 010h
        db  000h, 008h, 000h, 004h, 000h, 002h, 000h, 001h, 0FFh, 000h, 000h, 0FFh, 0FFh, 0F0h, 0F0h, 000h
        db  000h, 00Fh, 0FFh, 0FFh, 07Fh, 0FFh, 000h, 003h, 000h, 000h, 000h, 0D4h, 0E7h, 0E7h, 0E7h, 0F2h
        db  0F2h, 0F3h, 028h, 043h, 029h, 020h, 043h, 04Fh, 050h, 059h, 052h, 049h, 047h, 048h, 054h, 020h
        db  049h, 042h, 04Dh, 020h, 031h, 039h, 038h, 033h, 02Ch, 034h, 02Ch, 035h, 02Ch, 036h, 028h, 043h
        db  029h, 020h, 043h, 04Fh, 050h, 059h, 052h, 049h, 047h, 048h, 054h, 020h, 054h, 049h, 020h, 031h
        db  039h, 038h, 033h, 02Dh, 038h, 039h, 02Ch, 039h, 030h, 02Ch, 031h, 02Ch, 032h, 000h, 000h, 000h
        db  0A6h, 00Eh, 0F5h, 0B0h, 0F2h, 0FCh, 0ECh, 070h, 0F6h, 0B4h, 000h, 000h, 000h, 0C4h, 0BCh, 0A6h
        db  006h, 04Ah, 006h, 050h, 006h, 04Ch, 0D8h, 0EAh, 006h, 04Eh, 000h, 00Fh, 006h, 032h, 000h, 001h
        db  007h, 050h, 007h, 058h, 007h, 052h, 0C2h, 00Ah, 007h, 054h, 000h, 00Fh, 007h, 038h, 000h, 001h
        db  000h, 0BAh, 000h, 0A0h, 000h, 0BCh, 0F6h, 0C2h, 000h, 0BEh, 000h, 00Fh, 007h, 05Eh, 007h, 03Ah
        db  007h, 062h, 080h, 040h, 007h, 064h, 0C6h, 08Ah, 007h, 066h, 0C6h, 068h, 007h, 068h, 0C4h, 05Ch
        db  007h, 098h, 080h, 000h, 007h, 078h, 080h, 000h, 008h, 094h, 000h, 004h, 008h, 096h, 000h, 001h
        db  008h, 09Eh, 000h, 008h, 008h, 0A8h, 000h, 00Ah, 008h, 0AAh, 000h, 006h, 008h, 0B2h, 000h, 00Ch
        db  008h, 0B4h, 000h, 004h, 001h, 0AEh, 000h, 000h, 008h, 0D0h, 000h, 000h, 009h, 014h, 000h, 000h
        db  006h, 00Ch, 000h, 013h, 006h, 00Ah, 000h, 020h, 000h, 000h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h
        db  0B2h, 0D8h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h, 0B4h, 0EEh, 000h, 0E0h, 0B5h, 05Eh, 000h, 0E0h
        db  0B8h, 0B2h, 000h, 0E0h, 0B4h, 004h, 000h, 0C0h, 0B4h, 014h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h
        db  0B3h, 078h, 000h, 0C0h, 0B4h, 066h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h
        db  0B3h, 078h, 000h, 0E0h, 0B3h, 078h, 000h, 0E0h, 0B3h, 078h, 000h, 0C0h, 0BCh, 0B2h, 000h, 0C0h
        db  0BDh, 01Eh, 000h, 0C0h, 0BDh, 05Ah, 000h, 0C0h, 0BDh, 0CAh, 000h, 0C0h, 0BEh, 04Ch, 000h, 0E0h
        db  0BFh, 0A0h, 000h, 0E0h, 0BFh, 046h, 000h, 0E0h, 0C0h, 0A2h, 000h, 0E0h, 0C0h, 038h, 000h, 0E0h
        db  0BAh, 00Eh, 000h, 0C0h, 0BAh, 062h, 000h, 0C0h, 0BAh, 0BEh, 000h, 0C0h, 0BAh, 0F0h, 000h, 0C0h
        db  0BAh, 038h, 000h, 0C0h, 0B3h, 072h, 000h, 0C0h, 0B3h, 072h, 03Fh, 000h, 07Fh, 000h, 05Eh, 000h
        db  000h, 030h, 000h, 028h, 000h, 043h, 0B6h, 0A4h, 0B6h, 0A4h, 0B5h, 008h, 0B5h, 000h, 0B5h, 032h
        db  0B5h, 032h, 0B5h, 04Eh, 0B6h, 0A4h, 040h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 008h, 000h
        db  020h, 000h, 008h, 000h, 080h, 000h, 008h, 000h, 001h, 000h, 010h, 000h, 000h, 000h, 000h, 008h
        db  000h, 00Ch, 000h, 010h, 000h, 014h, 010h, 00Eh, 00Ch, 00Ch, 00Ah, 00Ah, 00Ah, 00Ah, 008h, 008h
        db  008h, 008h, 008h, 008h, 008h, 008h, 000h, 000h, 0A1h, 0D2h, 000h, 011h, 006h, 006h, 000h, 000h
        db  0A1h, 0E2h, 000h, 021h, 004h, 004h, 000h, 000h, 0A2h, 002h, 000h, 041h, 002h, 002h, 000h, 000h
        db  0A2h, 042h, 000h, 081h, 000h, 000h, 000h, 000h, 0A2h, 0C2h, 007h, 08Eh, 007h, 098h, 0D6h, 072h
        db  007h, 058h, 0C5h, 026h, 007h, 058h, 0C4h, 0A2h, 007h, 058h, 0C5h, 096h, 007h, 058h, 0C4h, 0FEh
        db  007h, 098h, 0D2h, 0D0h, 007h, 098h, 0D2h, 0C8h, 007h, 078h, 0CDh, 08Ah, 007h, 058h, 0C5h, 05Eh
        db  007h, 098h, 0D6h, 02Ah, 007h, 078h, 0CEh, 03Eh, 007h, 058h, 0D3h, 040h, 007h, 058h, 0C5h, 00Eh
        db  007h, 058h, 0C5h, 04Ch, 007h, 058h, 0C6h, 010h, 007h, 098h, 0D7h, 0BCh, 007h, 098h, 0D3h, 06Ch
        db  007h, 078h, 0CEh, 028h, 007h, 0B8h, 0D8h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 0C6h, 0D8h
        db  0CCh, 0E0h, 0CDh, 092h, 0D2h, 04Ch, 0C9h, 048h, 0C9h, 080h, 0C9h, 09Eh, 0CAh, 028h, 0CAh, 04Ch
        db  0CAh, 068h, 0CAh, 0F4h, 0CAh, 0F4h, 0CAh, 014h, 0CBh, 092h, 0C9h, 0CEh, 000h, 000h, 000h, 000h
        db  02Ch, 000h, 00Ch, 000h, 00Fh, 001h, 0FEh, 0FFh, 058h, 000h, 00Eh, 000h, 0FEh, 0FFh, 000h, 00Eh
        db  070h, 000h, 080h, 040h, 05Eh, 000h, 0C0h, 0A0h, 0FFh, 0DFh, 018h, 000h, 0E0h, 000h, 078h, 000h
        db  050h, 000h, 060h, 000h, 070h, 000h, 00Ch, 000h, 000h, 006h, 000h, 000h, 0E3h, 086h, 007h, 0E6h
        db  007h, 0EFh, 000h, 008h, 000h, 000h, 0E3h, 08Ch, 007h, 0EAh, 007h, 0EFh, 000h, 006h, 000h, 040h
        db  00Ah, 000h, 007h, 0E6h, 007h, 0EEh, 000h, 008h, 000h, 040h, 00Ah, 006h, 007h, 0EAh, 007h, 0EEh
        db  000h, 000h, 0C1h, 0E2h, 0D4h, 08Bh, 0FFh, 0FFh, 0D1h, 0D7h, 0C5h, 0D9h, 0C3h, 0D4h, 059h, 03Bh
        db  008h, 0E6h, 005h, 0FCh, 009h, 01Ah, 006h, 0D8h, 004h, 006h, 0EAh, 07Ch, 008h, 0E2h, 004h, 048h
        db  008h, 080h, 000h, 006h, 006h, 00Ah, 00Ch, 00Eh, 0DAh, 092h, 0E7h, 050h, 0E7h, 078h, 0E8h, 05Eh
        db  0E9h, 060h, 0E9h, 0BAh, 0EAh, 014h, 0EAh, 0C6h, 0DAh, 092h, 0EBh, 048h, 0EAh, 0D8h, 0E7h, 036h
        db  0EAh, 0E4h, 0EAh, 0E4h, 0ECh, 00Eh, 0ECh, 018h, 0ECh, 022h, 0E4h, 036h, 0E6h, 0BEh, 0DAh, 088h
        db  0E4h, 088h, 0E5h, 056h, 0E5h, 0C6h, 0E6h, 042h, 0E6h, 066h, 0DAh, 0E0h, 0DAh, 088h, 0E3h, 0A6h
        db  0E3h, 088h, 0E3h, 042h, 0DAh, 088h, 0E6h, 0CAh, 0DAh, 088h, 006h, 072h, 0DEh, 080h, 007h, 008h
        db  006h, 072h, 0DDh, 0DEh, 001h, 0F4h, 006h, 072h, 0DDh, 0BEh, 007h, 008h, 006h, 07Ch, 0E5h, 02Eh
        db  000h, 004h, 006h, 07Ch, 0DEh, 002h, 000h, 000h, 006h, 07Ch, 0E6h, 0ACh, 000h, 0FAh, 006h, 086h
        db  0DDh, 052h, 000h, 005h, 006h, 090h, 0E6h, 0FCh, 000h, 028h, 006h, 090h, 0DEh, 0DAh, 001h, 004h
        db  006h, 090h, 000h, 000h, 000h, 002h, 006h, 090h, 0DEh, 00Ah, 002h, 0BCh, 006h, 09Ah, 0DEh, 090h
        db  005h, 0DCh, 006h, 09Ah, 0DEh, 034h, 000h, 064h, 006h, 09Ah, 0DEh, 094h, 000h, 014h, 006h, 09Ah
        db  0E7h, 0F0h, 006h, 040h, 006h, 09Ah, 0DEh, 09Ch, 000h, 064h, 000h, 000h, 007h, 00Ah, 007h, 00Eh
        db  007h, 004h, 006h, 0D8h, 007h, 000h, 006h, 0F0h, 006h, 0EEh, 006h, 0ECh, 007h, 00Ch, 006h, 0E6h
        db  007h, 018h, 050h, 000h, 000h, 0CCh, 000h, 003h, 084h, 000h, 0A8h, 000h, 0A0h, 000h, 020h, 000h
        db  080h, 000h, 040h, 000h, 008h, 000h, 040h, 000h, 080h, 000h, 040h, 000h, 010h, 000h, 0F5h, 070h
        db  0F4h, 036h, 0F4h, 050h, 0F4h, 06Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 009h
        db  0F4h, 0D8h, 0F4h, 040h, 0F4h, 056h, 0F4h, 070h, 001h, 040h, 001h, 042h, 001h, 042h, 001h, 042h
        db  000h, 000h, 000h, 07Fh, 000h, 0A0h, 000h, 0FFh, 002h, 010h, 002h, 01Fh, 002h, 030h, 002h, 03Fh
        db  002h, 050h, 002h, 05Fh, 002h, 070h, 002h, 07Fh, 002h, 090h, 002h, 09Fh, 002h, 0B0h, 002h, 0BFh
        db  002h, 0D0h, 002h, 0DFh, 002h, 0E1h, 002h, 0FFh, 003h, 001h, 003h, 07Fh, 003h, 081h, 003h, 08Fh
        db  003h, 091h, 003h, 09Fh, 003h, 0A1h, 003h, 0AFh, 003h, 0B1h, 003h, 0BFh, 003h, 0C1h, 003h, 0CFh
        db  003h, 0E1h, 003h, 0FFh, 007h, 0C0h, 007h, 0FFh, 00Ch, 000h, 00Fh, 0FFh, 030h, 000h, 037h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FEh, 0BCh, 000h, 007h, 002h, 05Eh, 001h, 000h, 0BAh, 0FFh, 0BAh, 080h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 00Ch, 001h, 00Ah, 001h, 00Eh, 001h, 010h, 001h, 014h
        db  000h, 000h, 001h, 012h, 0F8h, 000h, 001h, 016h, 0FFh, 000h, 000h, 000h, 000h, 000h, 001h, 00Ah
        db  001h, 01Ch, 001h, 082h, 096h, 066h, 096h, 066h, 055h, 055h, 000h, 000h, 001h, 082h, 08Ah, 02Ah
        db  08Ah, 02Ah, 0C9h, 018h, 0C9h, 018h, 001h, 086h, 0A2h, 0AAh, 0A0h, 01Eh, 055h, 055h, 054h, 01Eh
        db  001h, 08Ah, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 08Ch, 050h, 000h, 000h, 000h
        db  0A8h, 000h, 000h, 000h, 001h, 08Eh, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 090h
        db  050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 092h, 050h, 000h, 000h, 000h, 0A8h, 000h
        db  000h, 000h, 001h, 094h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 096h, 050h, 000h
        db  000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 098h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h
        db  001h, 09Ah, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 000h, 001h, 09Ch, 055h, 055h, 07Fh, 0C0h
        db  0AAh, 0AAh, 07Fh, 0C0h, 000h, 000h, 001h, 0A2h, 001h, 0A4h, 001h, 0A8h, 001h, 0AAh, 001h, 0AEh
        db  001h, 0B0h, 001h, 0B2h, 001h, 080h, 000h, 000h, 001h, 088h, 0FFh, 000h, 001h, 09Eh, 000h, 0FFh
        db  001h, 0A0h, 080h, 000h, 001h, 0ACh, 080h, 000h, 000h, 000h, 001h, 0A6h, 080h, 000h, 000h, 000h
        db  001h, 080h, 001h, 0B2h, 088h, 000h, 006h, 000h, 0C8h, 000h, 000h, 000h, 080h, 000h, 000h, 030h
        db  000h, 000h, 080h, 000h, 000h, 002h, 000h, 044h, 0F3h, 080h, 000h, 048h, 0F3h, 086h, 000h, 050h
        db  0F3h, 08Ch, 000h, 060h, 0F3h, 092h, 000h, 078h, 0F3h, 098h, 0EEh, 09Ch, 0EDh, 0AAh, 0E9h, 0FEh
        db  0EDh, 0BCh, 055h, 014h, 001h, 0A0h, 0EDh, 07Eh, 0E9h, 092h, 0EDh, 0D8h, 0EEh, 032h, 0EDh, 092h
        db  0EEh, 0EAh, 000h, 000h, 000h, 01Eh, 0EDh, 0D8h, 0EEh, 024h, 041h, 000h, 041h, 001h, 0EEh, 048h
        db  0EAh, 035h, 0EDh, 0AAh, 0EAh, 00Ah, 0EDh, 07Eh, 0E9h, 0C6h, 0EDh, 0E8h, 0EDh, 0F4h, 0EEh, 048h
        db  0EAh, 030h, 0EEh, 0F4h, 000h, 000h, 0EEh, 0C8h, 0EDh, 07Eh, 0E9h, 0BCh, 0EDh, 0E8h, 0EDh, 0F4h
        db  0EEh, 0C8h, 0EEh, 0F4h, 000h, 000h, 0EDh, 07Eh, 0E9h, 0B2h, 0EDh, 09Eh, 0EDh, 0DCh, 0EDh, 0F4h
        db  0EEh, 0C8h, 0EDh, 07Eh, 0E9h, 0BCh, 0EDh, 0E8h, 0EDh, 0F4h, 0EEh, 0C8h, 0EEh, 0F4h, 020h, 000h
        db  0EDh, 0BCh, 055h, 014h, 001h, 0A0h, 0EDh, 0AAh, 0EAh, 012h, 0EDh, 07Eh, 0E9h, 092h, 0EEh, 0EAh
        db  055h, 050h, 000h, 00Ch, 0EDh, 0D8h, 0EEh, 024h, 001h, 000h, 000h, 000h, 0EEh, 048h, 0EAh, 037h
        db  0EDh, 092h, 0EEh, 0EAh, 055h, 055h, 000h, 00Ch, 0EDh, 0E8h, 0EEh, 032h, 0EDh, 092h, 0EEh, 0EAh
        db  0FFh, 0FFh, 000h, 008h, 0EEh, 0EAh, 010h, 002h, 000h, 006h, 0EDh, 0D8h, 0EEh, 024h, 001h, 000h
        db  001h, 001h, 0EEh, 048h, 0EAh, 042h, 0EDh, 092h, 0EEh, 0EAh, 0C0h, 000h, 000h, 008h, 0EEh, 0EAh
        db  0FFh, 0FFh, 000h, 00Ah, 0EEh, 0EAh, 0FFh, 0FFh, 000h, 00Ch, 0EEh, 0EAh, 010h, 00Dh, 000h, 006h
        db  0EDh, 0D8h, 0EEh, 024h, 001h, 000h, 001h, 001h, 0EEh, 048h, 0EAh, 036h, 0EEh, 0F4h, 020h, 008h
        db  0EDh, 092h, 0EEh, 0E4h, 001h, 082h, 0C9h, 002h, 0EDh, 0D8h, 0EEh, 048h, 0EAh, 042h, 0EEh, 0F4h
        db  020h, 034h, 0EDh, 092h, 0EEh, 0EAh, 010h, 000h, 000h, 006h, 0EDh, 0D8h, 0EFh, 058h, 0EEh, 048h
        db  0EAh, 03Ah, 0EEh, 0E4h, 001h, 09Ch, 000h, 040h, 0EDh, 0AAh, 0EAh, 012h, 0EDh, 0BCh, 000h, 0FFh
        db  007h, 080h, 0EFh, 0B8h, 000h, 003h, 0EFh, 0F8h, 0F0h, 006h, 0E9h, 0D4h, 0EFh, 0CAh, 000h, 000h
        db  0F0h, 006h, 0E9h, 0DEh, 0EFh, 0CAh, 000h, 004h, 0F0h, 006h, 0E9h, 0E6h, 0EFh, 0CAh, 000h, 007h
        db  0F0h, 006h, 0E9h, 0EEh, 0EFh, 0B8h, 000h, 001h, 0F0h, 006h, 0E9h, 0F6h, 0EFh, 0CAh, 000h, 002h
        db  0F0h, 006h, 0E9h, 0F6h, 0EFh, 0CAh, 000h, 006h, 0F0h, 006h, 0E9h, 0F6h, 0EFh, 0CAh, 000h, 005h
        db  0F0h, 006h, 0E9h, 0F6h, 0EFh, 0B8h, 000h, 001h, 0EFh, 0DCh, 0EFh, 0B8h, 000h, 003h, 0EFh, 0EAh
        db  0EDh, 0F4h, 0EFh, 078h, 0E9h, 09Ah, 000h, 001h, 0EFh, 078h, 0E9h, 0E7h, 000h, 002h, 0EFh, 078h
        db  0E9h, 0F1h, 000h, 006h, 0EFh, 078h, 0E9h, 0FCh, 000h, 005h, 0EEh, 048h, 0EAh, 036h, 0EEh, 0C8h
        db  0EDh, 07Eh, 0E9h, 092h, 0EDh, 0E8h, 0EFh, 058h, 0EDh, 09Eh, 0EEh, 024h, 001h, 000h, 080h, 000h
        db  0EEh, 048h, 0EAh, 03Ah, 0EDh, 092h, 0EFh, 090h, 0EEh, 0E4h, 001h, 080h, 08Eh, 041h, 0EDh, 0DCh
        db  0EEh, 024h, 001h, 000h, 01Bh, 001h, 0EFh, 098h, 0FFh, 0E4h, 0EEh, 048h, 0EAh, 03Eh, 0EFh, 050h
        db  0EDh, 0AAh, 0EAh, 018h, 0EDh, 09Eh, 0EEh, 0FCh, 0EDh, 0CEh, 0E0h, 000h, 0EFh, 058h, 0EEh, 048h
        db  0EAh, 048h, 0EDh, 0CEh, 0E8h, 000h, 0EDh, 0F4h, 0EEh, 048h, 0EAh, 047h, 0EDh, 0CEh, 008h, 000h
        db  0EFh, 058h, 0EEh, 048h, 0EAh, 048h, 0EDh, 0CEh, 0F8h, 000h, 0EDh, 0F4h, 0EEh, 048h, 0EAh, 047h
        db  0EEh, 0E4h, 001h, 080h, 002h, 000h, 0EDh, 0CEh, 0E0h, 000h, 0EDh, 0F4h, 0EEh, 048h, 0EAh, 047h
        db  0EEh, 0E4h, 001h, 084h, 000h, 000h, 0EEh, 0F4h, 000h, 034h, 0EDh, 0CEh, 000h, 000h, 0EFh, 058h
        db  0EEh, 0F4h, 060h, 034h, 0EFh, 0A0h, 0EEh, 0E4h, 001h, 084h, 000h, 000h, 0EEh, 048h, 0EAh, 048h
        db  0EEh, 0E4h, 001h, 082h, 0C8h, 000h, 0EDh, 0CEh, 0E0h, 000h, 0EFh, 058h, 0EDh, 0CEh, 010h, 000h
        db  0EFh, 058h, 0EEh, 0F4h, 060h, 034h, 0EEh, 0E4h, 001h, 080h, 006h, 000h, 0EDh, 0CEh, 000h, 010h
        db  0EFh, 00Ah, 0EEh, 0C8h, 0EEh, 0E4h, 001h, 084h, 000h, 000h, 0EEh, 0F4h, 000h, 034h, 0EDh, 07Eh
        db  0E9h, 092h, 0EDh, 0AAh, 0EAh, 01Eh, 0EFh, 062h, 0E9h, 054h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h
        db  0F0h, 040h, 000h, 006h, 0C7h, 000h, 0EEh, 032h, 0EFh, 06Eh, 0E0h, 000h, 0EDh, 092h, 0EDh, 09Eh
        db  0EEh, 002h, 0D0h, 040h, 000h, 006h, 0E0h, 000h, 0EEh, 032h, 0EFh, 06Eh, 0C0h, 000h, 0EDh, 092h
        db  0EDh, 09Eh, 0EEh, 002h, 090h, 040h, 000h, 006h, 0A0h, 000h, 0EEh, 032h, 0EFh, 06Eh, 080h, 000h
        db  0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 050h, 040h, 000h, 006h, 060h, 000h, 0EEh, 032h, 0EFh, 06Eh
        db  040h, 000h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 010h, 040h, 000h, 006h, 020h, 000h, 0EEh, 032h
        db  0EFh, 06Eh, 000h, 000h, 0EFh, 062h, 0E9h, 054h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 0D0h, 040h
        db  000h, 006h, 0A6h, 000h, 0EEh, 032h, 0EFh, 06Eh, 0C0h, 000h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h
        db  090h, 040h, 000h, 006h, 0C0h, 000h, 0EEh, 032h, 0EFh, 06Eh, 080h, 000h, 0EDh, 092h, 0EDh, 09Eh
        db  0EEh, 002h, 050h, 040h, 000h, 006h, 040h, 000h, 0EEh, 032h, 0EFh, 06Eh, 040h, 000h, 0EDh, 092h
        db  0EDh, 09Eh, 0EEh, 002h, 070h, 040h, 000h, 006h, 060h, 000h, 0EEh, 032h, 0EFh, 06Eh, 060h, 000h
        db  0F0h, 010h, 0E9h, 052h, 0EDh, 0AAh, 0EAh, 024h, 0EDh, 07Eh, 0E9h, 092h, 0EFh, 036h, 055h, 055h
        db  000h, 016h, 0EDh, 0D8h, 0EEh, 024h, 000h, 000h, 000h, 000h, 0EEh, 048h, 0EAh, 04Dh, 0EEh, 09Ch
        db  0EDh, 0AAh, 0EAh, 024h, 0EEh, 0EAh, 055h, 055h, 000h, 016h, 0EDh, 092h, 0EDh, 0D8h, 0EEh, 032h
        db  0EDh, 0BCh, 000h, 0FFh, 008h, 000h, 0EDh, 0BCh, 000h, 0FFh, 00Ch, 000h, 0EDh, 0BCh, 000h, 0FFh
        db  010h, 000h, 0EDh, 0BCh, 000h, 0FFh, 014h, 000h, 0EDh, 0BCh, 000h, 0FFh, 018h, 000h, 0EDh, 0BCh
        db  000h, 0FFh, 01Ch, 000h, 0EDh, 0BCh, 000h, 0FFh, 020h, 000h, 0EDh, 0BCh, 000h, 0FFh, 024h, 000h
        db  0EDh, 0BCh, 000h, 0FFh, 028h, 000h, 0EDh, 0BCh, 000h, 0FFh, 02Ch, 000h, 0EEh, 064h, 0EDh, 092h
        db  0EDh, 09Eh, 0EEh, 002h, 030h, 040h, 000h, 006h, 001h, 000h, 0EEh, 032h, 0EFh, 06Eh, 020h, 000h
        db  0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 070h, 040h, 000h, 006h, 043h, 000h, 0EEh, 032h, 0EFh, 06Eh
        db  060h, 000h, 0EDh, 092h, 0EDh, 09Eh, 0EEh, 002h, 0B0h, 040h, 000h, 006h, 085h, 000h, 0EEh, 032h
        db  0EFh, 06Eh, 0A0h, 000h, 0EFh, 06Ah, 001h, 000h, 001h, 003h, 001h, 001h, 000h, 000h, 081h, 000h
        db  000h, 01Ah, 010h, 040h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h
        db  082h, 072h, 0A9h, 04Ah, 05Ah, 0A5h, 0E7h, 0DAh, 009h, 003h, 09Dh, 011h, 000h, 000h, 081h, 000h
        db  000h, 004h, 090h, 0D8h, 010h, 000h, 000h, 000h, 081h, 000h, 000h, 004h, 090h, 0D8h, 0B4h, 0D8h
        db  000h, 000h, 081h, 000h, 000h, 008h, 090h, 0D8h, 016h, 046h, 040h, 000h, 0B4h, 0D8h, 000h, 008h
        db  000h, 000h, 080h, 000h, 000h, 013h, 010h, 040h, 000h, 016h, 000h, 000h, 080h, 000h, 000h, 010h
        db  000h, 015h, 000h, 000h, 080h, 000h, 000h, 00Fh, 000h, 015h, 000h, 000h, 081h, 000h, 000h, 00Fh
        db  000h, 006h, 000h, 000h, 080h, 000h, 000h, 012h, 080h, 00Ah, 09Eh, 040h, 0C8h, 000h, 000h, 000h
        db  080h, 05Eh, 000h, 00Fh, 080h, 006h, 0FEh, 040h, 0CCh, 000h, 000h, 000h, 080h, 004h, 08Eh, 040h
        db  0C9h, 000h, 080h, 004h, 006h, 000h, 0CCh, 000h, 080h, 004h, 00Ah, 040h, 0C8h, 000h, 080h, 00Ah
        db  08Ah, 040h, 0C8h, 000h, 000h, 000h, 080h, 05Eh, 000h, 00Fh, 008h, 00Ah, 01Ch, 080h, 000h, 00Ah
        db  01Ah, 01Ch, 080h, 000h, 00Ch, 01Ch, 080h, 000h, 01Ah, 01Ch, 080h, 000h, 00Eh, 01Ah, 01Ch, 080h
        db  000h, 004h, 080h, 000h, 002h, 080h, 000h, 002h, 080h, 000h, 000h, 000h, 0AAh, 050h, 000h, 02Dh
        db  000h, 000h, 000h, 000h, 0AAh, 07Ch, 002h, 0F6h, 007h, 058h, 0C4h, 036h, 0E0h, 016h, 008h, 094h
        db  008h, 09Eh, 008h, 0A8h, 0E0h, 016h, 008h, 0B2h, 008h, 0BCh, 007h, 0B8h, 0D7h, 0D8h, 002h, 080h
        db  007h, 0B8h, 0D7h, 0F6h, 0FFh, 084h, 007h, 0B8h, 0D8h, 018h, 007h, 0B8h, 0D8h, 092h, 0D9h, 072h
        db  0D9h, 066h, 0D9h, 08Ch, 0D9h, 090h, 0D9h, 088h, 0DAh, 070h, 0DAh, 070h, 0DAh, 070h, 0D9h, 092h
        db  0D9h, 09Ch, 0DAh, 018h, 0DAh, 062h, 0DAh, 070h, 0DAh, 070h, 0DAh, 070h, 0DAh, 070h, 000h, 007h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 001h, 008h, 008h, 005h, 008h, 008h, 008h, 000h
        db  000h, 002h, 004h, 004h, 004h, 004h, 000h, 008h, 00Ah, 000h, 000h, 00Ch, 010h, 000h, 000h, 000h
        db  000h, 000h, 041h, 004h, 00Bh, 006h, 0C2h, 008h, 0E6h, 000h, 0E7h, 000h, 006h, 004h, 007h, 004h
        db  003h, 004h, 004h, 006h, 005h, 004h, 088h, 004h, 000h, 003h, 000h, 01Ch, 00Ch, 000h, 080h, 000h
        db  0E2h, 0D0h, 0E2h, 0D8h, 0E3h, 01Ch, 0E2h, 0DCh, 0E2h, 0E8h, 0E2h, 0EEh, 0E3h, 012h, 0E2h, 0E2h
        db  0E3h, 030h, 006h, 000h, 000h, 000h, 007h, 006h, 00Eh, 014h, 014h, 01Eh, 02Ah, 04Ch, 038h, 07Ch
        db  03Eh, 078h, 043h, 092h, 058h, 09Ch, 04Dh, 0B2h, 02Fh, 064h, 01Dh, 034h, 06Ch, 0D0h, 023h, 03Eh
        db  01Dh, 02Ah, 065h, 0C0h, 072h, 0E0h, 078h, 0EEh, 07Fh, 0FCh, 001h, 032h, 057h, 001h, 011h, 032h
        db  051h, 081h, 056h, 002h, 055h, 003h, 011h, 054h, 081h, 056h, 002h, 055h, 002h, 054h, 081h, 056h
        db  076h, 001h, 034h, 002h, 055h, 002h, 054h, 081h, 058h, 002h, 055h, 002h, 054h, 081h, 058h, 002h
        db  012h, 011h, 052h, 002h, 083h, 058h, 005h, 052h, 004h, 081h, 058h, 005h, 052h, 003h, 015h, 05Ch
        db  05Bh, 053h, 087h, 052h, 003h, 011h, 051h, 041h, 051h, 078h, 011h, 034h, 011h, 081h, 031h, 020h
        db  057h, 054h, 053h, 001h, 012h, 05Ah, 051h, 081h, 031h, 020h, 057h, 05Bh, 05Ah, 001h, 011h, 001h
        db  011h, 051h, 081h, 031h, 020h, 057h, 001h, 015h, 001h, 013h, 001h, 011h, 011h, 011h, 051h, 081h
        db  058h, 005h, 052h, 002h, 054h, 05Bh, 081h, 05Dh, 005h, 052h, 002h, 054h, 081h, 058h, 002h, 050h
        db  003h, 013h, 081h, 058h, 002h, 050h, 003h, 011h, 054h, 081h, 05Dh, 072h, 003h, 050h, 003h, 013h
        db  001h, 013h, 054h, 040h, 000h, 00Eh, 006h, 020h, 006h, 056h, 0E3h, 0FEh, 000h, 024h, 010h, 002h
        db  000h, 016h, 000h, 002h, 004h, 001h, 007h, 008h, 0E3h, 0FEh, 000h, 020h, 010h, 003h, 000h, 012h
        db  000h, 003h, 0E3h, 04Ch, 08Eh, 014h, 000h, 020h, 010h, 004h, 000h, 012h, 000h, 004h, 0DAh, 0AAh
        db  000h, 020h, 0E0h, 005h, 000h, 012h, 000h, 005h, 0DAh, 0AAh, 000h, 020h, 060h, 006h, 000h, 012h
        db  000h, 006h, 0E5h, 0F6h, 000h, 012h, 060h, 001h, 009h, 01Ah, 006h, 0CCh, 000h, 004h, 000h, 007h
        db  000h, 000h, 006h, 030h, 0E5h, 016h, 005h, 0F0h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  005h, 0E2h, 000h, 008h, 0FFh, 026h, 005h, 0DCh, 0E6h, 082h, 000h, 03Eh, 060h, 000h, 0C0h, 000h
        db  000h, 000h, 000h, 002h, 000h, 030h, 050h, 020h, 00Ch, 023h, 005h, 0FCh, 006h, 052h, 006h, 056h
        db  000h, 000h, 081h, 000h, 000h, 016h, 060h, 000h, 0C0h, 000h, 000h, 000h, 000h, 010h, 000h, 008h
        db  040h, 02Ah, 004h, 02Ah, 006h, 056h, 000h, 026h, 0ECh, 063h, 006h, 02Bh, 009h, 020h, 000h, 022h
        db  000h, 024h, 0ECh, 079h, 00Ch, 023h, 005h, 0FCh, 008h, 028h, 008h, 0E6h, 008h, 029h, 007h, 058h
        db  007h, 078h, 007h, 098h, 000h, 023h, 000h, 02Ah, 0ECh, 087h, 004h, 006h, 006h, 0F0h, 004h, 007h
        db  006h, 0EEh, 000h, 024h, 0DAh, 0AAh, 000h, 034h, 060h, 000h, 0C0h, 000h, 000h, 000h, 000h, 010h
        db  000h, 026h, 040h, 025h, 0DAh, 0AAh, 000h, 020h, 060h, 000h, 0C0h, 000h, 000h, 000h, 000h, 010h
        db  000h, 012h, 040h, 026h, 0DAh, 0AAh, 000h, 01Ah, 060h, 000h, 000h, 00Ch, 060h, 027h, 008h, 00Ah
        db  006h, 0E6h, 0DAh, 0AAh, 000h, 024h, 060h, 000h, 000h, 016h, 060h, 028h, 004h, 030h, 007h, 006h
        db  0DBh, 000h, 081h, 000h, 000h, 030h, 060h, 000h, 000h, 022h, 060h, 029h, 008h, 02Dh, 007h, 01Ch
        db  008h, 02Eh, 007h, 022h, 000h, 000h, 002h, 008h, 001h, 006h, 006h, 014h, 008h, 018h, 00Ch, 020h
        db  00Eh, 026h, 00Fh, 030h, 011h, 034h, 012h, 03Eh, 014h, 042h, 016h, 046h, 00Ah, 01Ch, 018h, 04Ah
        db  003h, 013h, 083h, 011h, 011h, 001h, 081h, 011h, 081h, 012h, 001h, 013h, 083h, 052h, 085h, 081h
        db  011h, 085h, 081h, 012h, 081h, 012h, 081h, 019h, 085h, 060h, 0C0h, 000h, 000h, 000h, 000h, 008h
        db  009h, 01Ah, 000h, 000h, 006h, 030h, 0EBh, 0D0h, 006h, 054h, 006h, 050h, 002h, 038h, 004h, 021h
        db  008h, 0D0h, 006h, 00Bh, 006h, 0D8h, 008h, 002h, 006h, 0DCh, 0C0h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 041h, 000h, 041h, 0AEh, 014h, 000h, 000h, 081h, 000h, 004h, 009h, 007h, 00Ch, 000h, 041h
        db  000h, 041h, 002h, 014h, 000h, 000h, 081h, 000h, 006h, 00Bh, 006h, 0D8h, 006h, 02Ch, 009h, 024h
        db  014h, 022h, 008h, 0ECh, 000h, 041h, 000h, 041h, 002h, 054h, 000h, 000h, 081h, 000h, 006h, 0D8h
        db  084h, 000h, 048h, 000h, 0FFh, 0FCh, 000h, 009h, 0C0h, 000h, 000h, 000h, 000h, 010h, 000h, 000h
        db  0ADh, 072h, 002h, 08Fh, 000h, 000h, 000h, 000h, 0FFh, 0B8h, 000h, 048h, 028h, 043h, 029h, 031h
        db  039h, 038h, 033h, 02Dh, 038h, 039h, 02Ch, 039h, 030h, 02Ch, 031h, 02Ch, 032h, 020h, 054h, 065h
        db  078h, 061h, 073h, 020h, 049h, 06Eh, 073h, 074h, 072h, 075h, 06Dh, 065h, 06Eh, 074h, 073h, 00Ah
        db  028h, 043h, 029h, 031h, 039h, 038h, 033h, 02Ch, 034h, 02Ch, 035h, 02Ch, 036h, 020h, 049h, 042h
        db  04Dh, 020h, 043h, 06Fh, 072h, 070h, 000h, 000h, 000h, 000h, 000h, 000h, 0A0h, 034h, 0FFh, 0B8h
        db  000h, 080h, 0B0h, 03Eh, 000h, 000h, 0C0h, 038h, 000h, 036h, 003h, 000h, 000h, 002h, 0C8h, 00Bh
        db  001h, 06Ch, 0C2h, 08Bh, 001h, 060h, 009h, 018h, 000h, 004h, 016h, 00Dh, 001h, 060h, 0FCh, 002h
        db  000h, 020h, 016h, 009h, 002h, 08Bh, 000h, 043h, 013h, 006h, 001h, 060h, 007h, 058h, 002h, 000h
        db  016h, 002h, 004h, 060h, 0FEh, 072h, 006h, 0A0h, 0C0h, 0F0h, 003h, 090h, 000h, 07Fh, 003h, 080h
        db  000h, 000h, 0CDh, 004h, 00Ah, 088h, 010h, 01Bh, 045h, 04Fh, 020h, 056h, 032h, 032h, 033h, 02Eh
        db  031h, 031h, 02Eh, 030h, 030h, 020h, 020h, 028h, 043h, 029h, 020h, 043h, 06Fh, 070h, 079h, 072h
        db  069h, 067h, 068h, 074h, 020h, 04Fh, 06Ch, 069h, 063h, 06Fh, 06Dh, 020h, 031h, 039h, 039h, 034h
        db  02Eh, 000h, 001h, 060h, 007h, 058h, 014h, 000h, 016h, 0FCh, 004h, 060h, 0CEh, 00Ah, 007h, 089h
        db  014h, 08Eh, 0D2h, 0E0h, 009h, 032h, 009h, 08Bh, 003h, 000h, 000h, 002h, 02Dh, 081h, 0C0h, 041h
        db  013h, 019h, 010h, 007h, 02Dh, 081h, 0C0h, 041h, 013h, 00Fh, 0C8h, 00Ah, 001h, 06Ch, 0C8h, 001h
        db  0FCh, 000h, 0CEh, 041h, 0C8h, 001h, 001h, 06Ch, 0C2h, 081h, 004h, 0E0h, 0FCh, 004h, 007h, 0A0h
        db  0FCh, 002h, 080h, 000h, 006h, 00Bh, 016h, 0EEh, 0C8h, 020h, 001h, 06Ch, 008h, 02Ch, 0C8h, 020h
        db  001h, 06Ch, 008h, 042h, 0D8h, 020h, 009h, 032h, 008h, 0CFh, 007h, 08Fh, 0D2h, 004h, 0C3h, 08Eh
        db  011h, 002h, 007h, 08Fh, 0D1h, 034h, 0C8h, 00Fh, 008h, 050h, 007h, 08Fh, 0D2h, 080h, 088h, 00Fh
        db  000h, 016h, 013h, 003h, 0C8h, 020h, 008h, 050h, 000h, 016h, 003h, 000h, 000h, 00Fh, 004h, 020h
        db  0EAh, 07Ch, 001h, 020h, 0FCh, 000h, 00Fh, 0FFh, 013h, 010h, 001h, 0A0h, 0FCh, 000h, 0C0h, 000h
        db  0C0h, 060h, 0FCh, 000h, 0C8h, 001h, 001h, 06Ch, 0C1h, 041h, 004h, 0E0h, 0FCh, 004h, 007h, 0A0h
        db  0FCh, 002h, 080h, 000h, 005h, 0A0h, 010h, 082h, 010h, 0ECh, 001h, 043h, 080h, 000h, 016h, 022h
        db  004h, 0C3h, 003h, 000h, 000h, 002h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 009h, 007h, 0A0h
        db  001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 003h, 000h, 000h, 00Fh, 010h, 012h
        db  001h, 0E0h, 000h, 0E0h, 000h, 002h, 003h, 000h, 000h, 00Fh, 010h, 00Ch, 003h, 000h, 000h, 00Fh
        db  0C0h, 086h, 002h, 042h, 000h, 07Fh, 0A0h, 082h, 0C8h, 005h, 001h, 06Ch, 001h, 020h, 0FCh, 000h
        db  00Fh, 0FFh, 016h, 0CBh, 088h, 006h, 010h, 086h, 013h, 0F9h, 0C1h, 022h, 014h, 08Eh, 0C8h, 004h
        db  001h, 06Ch, 0C0h, 060h, 0FCh, 000h, 011h, 041h, 003h, 000h, 000h, 002h, 0C2h, 08Ah, 016h, 014h
        db  0C8h, 004h, 001h, 08Ch, 0C2h, 0C4h, 0C3h, 004h, 0C8h, 00Ch, 001h, 06Ch, 001h, 020h, 0FCh, 000h
        db  00Fh, 0FFh, 013h, 003h, 0C3h, 020h, 0FCh, 000h, 010h, 0F7h, 005h, 086h, 007h, 08Ah, 000h, 001h
        db  001h, 084h, 07Fh, 0FFh, 0E2h, 084h, 010h, 0D2h, 005h, 086h, 0C8h, 009h, 001h, 06Ch, 013h, 014h
        db  0C8h, 004h, 0FCh, 000h, 001h, 0E0h, 0FCh, 000h, 040h, 000h, 0C2h, 044h, 001h, 084h, 07Fh, 0FFh
        db  0E1h, 0C4h, 0C8h, 009h, 001h, 06Ch, 001h, 020h, 0FCh, 000h, 00Fh, 0FFh, 013h, 003h, 0C2h, 060h
        db  0FCh, 000h, 010h, 0F7h, 005h, 087h, 010h, 0BAh, 0C2h, 004h, 0C2h, 044h, 001h, 084h, 07Fh, 0FFh
        db  0E1h, 0C4h, 0C8h, 009h, 001h, 06Ch, 001h, 020h, 0FCh, 000h, 00Fh, 0FFh, 013h, 003h, 0C2h, 060h
        db  0FCh, 000h, 010h, 0F7h, 007h, 087h, 000h, 001h, 010h, 0A9h, 0C0h, 0A0h, 0FCh, 004h, 002h, 022h
        db  0FCh, 006h, 0C0h, 012h, 0C8h, 001h, 001h, 06Ch, 0C8h, 000h, 0FCh, 004h, 003h, 000h, 000h, 002h
        db  0C2h, 08Ah, 016h, 032h, 0C8h, 004h, 001h, 08Ch, 0C2h, 0C4h, 0C3h, 020h, 0FCh, 000h, 005h, 086h
        db  007h, 08Ah, 000h, 001h, 001h, 084h, 07Fh, 0FFh, 0E2h, 084h, 003h, 000h, 000h, 00Fh, 0C3h, 020h
        db  0FCh, 000h, 011h, 005h, 001h, 00Ch, 00Fh, 0FFh, 016h, 0FAh, 004h, 060h, 0CEh, 00Ah, 0C0h, 0A0h
        db  0FCh, 004h, 002h, 022h, 0FCh, 006h, 0C0h, 032h, 013h, 0F8h, 001h, 000h, 0FFh, 0FCh, 013h, 005h
        db  0C8h, 00Ch, 001h, 06Ch, 0C8h, 000h, 0FCh, 004h, 010h, 0EAh, 0C0h, 092h, 0C8h, 00Ch, 001h, 06Ch
        db  0C8h, 002h, 0FCh, 006h, 0C8h, 000h, 0FCh, 004h, 007h, 0A0h, 0FCh, 000h, 040h, 000h, 007h, 0A0h
        db  0FCh, 002h, 081h, 000h, 004h, 060h, 0CEh, 00Ah, 005h, 086h, 0C8h, 009h, 001h, 06Ch, 013h, 00Ch
        db  0C8h, 004h, 0FCh, 000h, 001h, 0E0h, 0FCh, 000h, 040h, 000h, 0C8h, 001h, 001h, 06Ch, 005h, 087h
        db  001h, 084h, 07Fh, 0FFh, 0E1h, 0C4h, 010h, 008h, 0C2h, 004h, 0C8h, 001h, 001h, 06Ch, 007h, 087h
        db  000h, 001h, 001h, 084h, 07Fh, 0FFh, 0E1h, 0C4h, 0C2h, 060h, 0FCh, 000h, 003h, 000h, 000h, 00Fh
        db  0C0h, 020h, 0FCh, 000h, 011h, 001h, 010h, 0FCh, 003h, 000h, 000h, 002h, 0C2h, 049h, 013h, 0B5h
        db  0C2h, 040h, 0C0h, 0A0h, 0FCh, 004h, 002h, 022h, 0FCh, 006h, 0C0h, 032h, 013h, 017h, 001h, 000h
        db  0FFh, 0FCh, 013h, 007h, 0C8h, 009h, 001h, 06Ch, 0C8h, 000h, 0FCh, 004h, 003h, 000h, 000h, 00Fh
        db  010h, 0E7h, 0C0h, 092h, 0C8h, 009h, 001h, 06Ch, 0C8h, 002h, 0FCh, 006h, 0C8h, 000h, 0FCh, 004h
        db  007h, 0A0h, 0FCh, 000h, 040h, 000h, 007h, 0A0h, 0FCh, 002h, 081h, 000h, 004h, 060h, 0CEh, 00Ah
        db  0C2h, 084h, 0C3h, 00Ah, 0C8h, 00Ah, 001h, 06Ah, 001h, 020h, 0F8h, 002h, 081h, 000h, 013h, 00Fh
        db  001h, 020h, 0F8h, 002h, 0FEh, 000h, 016h, 012h, 0C2h, 0A0h, 0F8h, 000h, 001h, 00Ah, 00Fh, 0FFh
        db  016h, 0F0h, 0C1h, 060h, 001h, 06Ah, 004h, 0CAh, 001h, 04Bh, 000h, 001h, 004h, 05Bh, 0C2h, 0A0h
        db  0F8h, 000h, 001h, 00Ah, 00Fh, 0FFh, 016h, 0E6h, 006h, 0A0h, 0D1h, 0A2h, 0C2h, 0A0h, 0F8h, 002h
        db  011h, 009h, 001h, 00Ah, 002h, 000h, 016h, 053h, 001h, 00Ah, 014h, 000h, 013h, 05Dh, 001h, 04Ah
        db  010h, 000h, 016h, 027h, 004h, 0CAh, 0D2h, 0A0h, 007h, 02Ch, 002h, 08Ah, 0FEh, 000h, 013h, 008h
        db  002h, 08Ah, 07Fh, 000h, 013h, 005h, 0D2h, 0A0h, 007h, 020h, 002h, 08Ah, 07Fh, 000h, 016h, 00Bh
        db  001h, 0E0h, 006h, 0D2h, 008h, 000h, 001h, 0E0h, 006h, 034h, 008h, 000h, 004h, 0E0h, 006h, 030h
        db  001h, 0CBh, 000h, 001h, 010h, 00Eh, 0D2h, 0A0h, 007h, 02Ch, 002h, 02Ah, 001h, 000h, 018h, 002h
        db  0D8h, 00Ah, 007h, 02Ch, 0D2h, 0A0h, 007h, 020h, 002h, 02Ah, 001h, 000h, 018h, 002h, 0D8h, 00Ah
        db  007h, 020h, 0C8h, 00Ch, 001h, 06Ah, 007h, 0A0h, 0F8h, 002h, 080h, 000h, 001h, 060h, 0F8h, 000h
        db  040h, 000h, 016h, 004h, 007h, 0A0h, 0F8h, 002h, 081h, 000h, 010h, 013h, 001h, 020h, 0F8h, 000h
        db  00Fh, 0FFh, 016h, 006h, 001h, 060h, 0F8h, 000h, 040h, 000h, 013h, 00Bh, 006h, 0A0h, 0D1h, 0A2h
        db  0C2h, 0A0h, 0F8h, 004h, 013h, 006h, 0C8h, 020h, 0F8h, 000h, 001h, 06Ah, 010h, 0E4h, 006h, 0A0h
        db  0D1h, 0A2h, 007h, 08Ah, 001h, 08Ch, 0C6h, 08Ch, 001h, 04Bh, 000h, 001h, 004h, 05Bh, 001h, 04Ah
        db  006h, 000h, 013h, 00Ah, 001h, 0E0h, 006h, 0D2h, 002h, 000h, 001h, 0E0h, 006h, 034h, 008h, 000h
        db  004h, 0E0h, 006h, 030h, 001h, 0CBh, 000h, 001h, 001h, 020h, 0F8h, 000h, 040h, 000h, 013h, 0C9h
        db  0D2h, 0A0h, 0E0h, 012h, 0C8h, 00Ah, 0F8h, 002h, 0C3h, 020h, 0F8h, 000h, 001h, 00Ch, 00Fh, 0FFh
        db  013h, 008h, 0C8h, 00Ch, 001h, 06Ah, 0C2h, 0A0h, 0F8h, 002h, 011h, 0DBh, 0C2h, 08Ch, 004h, 060h
        db  0CFh, 0C2h, 0C1h, 060h, 001h, 06Ah, 004h, 0CAh, 001h, 04Bh, 000h, 001h, 004h, 05Bh, 0C2h, 08Ah
        db  016h, 057h, 007h, 08Ah, 001h, 08Ch, 0C0h, 082h, 013h, 00Dh, 0C6h, 081h, 0C8h, 009h, 001h, 06Ah
        db  0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h, 0C0h, 0C0h, 0C1h, 001h, 0C1h, 042h
        db  004h, 0C2h, 010h, 046h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h
        db  008h, 028h, 004h, 0C3h, 010h, 03Dh, 003h, 000h, 000h, 002h, 086h, 08Ah, 001h, 060h, 001h, 09Ch
        db  040h, 000h, 016h, 03Eh, 001h, 003h, 000h, 0FFh, 013h, 03Bh, 0C8h, 005h, 001h, 06Ah, 001h, 020h
        db  0F8h, 002h, 0FFh, 0FFh, 013h, 004h, 001h, 020h, 0F8h, 002h, 0FEh, 000h, 013h, 007h, 006h, 0A0h
        db  0CFh, 0BEh, 013h, 0CDh, 0C2h, 08Ah, 016h, 010h, 007h, 08Ah, 001h, 08Ch, 0C0h, 082h, 013h, 00Fh
        db  0C6h, 081h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h
        db  0C0h, 0C0h, 0C1h, 001h, 0C1h, 042h, 004h, 0C2h, 003h, 090h, 000h, 01Fh, 003h, 080h, 0C8h, 009h
        db  001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h, 004h, 0C3h, 003h, 090h
        db  000h, 01Fh, 003h, 080h, 0C8h, 00Bh, 008h, 044h, 002h, 0E0h, 008h, 030h, 004h, 060h, 0B3h, 07Ch
        db  007h, 08Bh, 000h, 0DAh, 0CEh, 0CDh, 0CEh, 0CEh, 0C6h, 0CFh, 002h, 0E0h, 000h, 0C0h, 004h, 054h
        db  003h, 090h, 0FFh, 0FFh, 003h, 080h, 0C2h, 08Ah, 016h, 0F3h, 007h, 08Ah, 001h, 08Ch, 0C0h, 082h
        db  013h, 00Fh, 0C6h, 081h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h
        db  008h, 028h, 006h, 0A0h, 0F7h, 0FEh, 0C0h, 0C0h, 0C1h, 001h, 0C1h, 042h, 004h, 0C2h, 010h, 0E0h
        db  0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h, 006h, 0A0h
        db  0F7h, 0FEh, 004h, 0C3h, 010h, 0D5h, 003h, 000h, 000h, 002h, 086h, 08Ah, 001h, 060h, 001h, 09Ch
        db  040h, 000h, 016h, 0D6h, 001h, 003h, 000h, 0FFh, 013h, 0D3h, 0C8h, 005h, 001h, 06Ah, 001h, 020h
        db  0F8h, 002h, 0FFh, 0FFh, 013h, 004h, 001h, 020h, 0F8h, 002h, 0FEh, 000h, 013h, 007h, 006h, 0A0h
        db  0CFh, 0BEh, 013h, 0C9h, 0C2h, 08Ah, 016h, 0C4h, 007h, 08Ah, 001h, 08Ch, 0C0h, 082h, 013h, 011h
        db  0C6h, 081h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h
        db  006h, 0A0h, 0F7h, 0FEh, 0C0h, 0C0h, 0C1h, 001h, 0C1h, 042h, 004h, 0C2h, 003h, 090h, 000h, 01Fh
        db  003h, 080h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h
        db  006h, 0A0h, 0F7h, 0FEh, 004h, 0C3h, 003h, 090h, 000h, 01Fh, 003h, 080h, 003h, 090h, 0FFh, 0FFh
        db  003h, 080h, 003h, 000h, 000h, 002h, 0C2h, 020h, 001h, 08Ch, 001h, 060h, 001h, 09Ch, 040h, 000h
        db  016h, 0F5h, 0C0h, 0E0h, 001h, 06Ch, 0C2h, 020h, 005h, 0F4h, 013h, 002h, 004h, 060h, 0B8h, 0C2h
        db  002h, 0E0h, 008h, 030h, 001h, 003h, 000h, 0FFh, 013h, 029h, 001h, 043h, 004h, 000h, 013h, 026h
        db  0C8h, 005h, 001h, 06Ah, 001h, 020h, 0F8h, 002h, 0FFh, 0FFh, 013h, 004h, 001h, 020h, 0F8h, 002h
        db  0FEh, 000h, 013h, 00Ch, 006h, 0A0h, 0CFh, 0BEh, 016h, 005h, 001h, 0E0h, 000h, 0E6h, 080h, 000h
        db  005h, 0A0h, 016h, 038h, 0C2h, 08Ah, 016h, 036h, 007h, 08Ah, 001h, 08Ch, 0C8h, 009h, 001h, 06Ah
        db  0C8h, 004h, 0F8h, 000h, 0C2h, 045h, 0E8h, 003h, 008h, 028h, 0C1h, 060h, 008h, 050h, 002h, 085h
        db  0D2h, 004h, 016h, 002h, 006h, 0A0h, 0F7h, 0FEh, 002h, 043h, 003h, 000h, 002h, 0E0h, 000h, 0E0h
        db  001h, 020h, 008h, 036h, 001h, 000h, 016h, 01Ch, 0C2h, 020h, 005h, 0F0h, 013h, 002h, 004h, 060h
        db  0B9h, 024h, 002h, 0E0h, 008h, 030h, 001h, 083h, 006h, 000h, 016h, 005h, 0C1h, 042h, 013h, 008h
        db  0C0h, 0C0h, 0C1h, 001h, 004h, 0C2h, 0D8h, 020h, 008h, 0E0h, 001h, 080h, 0C8h, 004h, 001h, 08Ch
        db  002h, 0E0h, 000h, 0E0h, 007h, 0A0h, 000h, 014h, 008h, 030h, 0C8h, 020h, 008h, 050h, 000h, 016h
        db  004h, 060h, 0B9h, 02Ch, 002h, 0E0h, 000h, 0E0h, 004h, 060h, 0B9h, 02Ch, 003h, 000h, 000h, 002h
        db  0C0h, 0E0h, 001h, 06Ch, 0C3h, 020h, 001h, 08Ah, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 058h
        db  001h, 020h, 009h, 018h, 000h, 006h, 016h, 004h, 001h, 0A0h, 001h, 082h, 000h, 006h, 010h, 003h
        db  001h, 0E0h, 001h, 082h, 000h, 006h, 0C8h, 001h, 001h, 08Ah, 0C8h, 001h, 008h, 0CAh, 007h, 0A0h
        db  0E1h, 09Eh, 0B5h, 04Eh, 007h, 0A0h, 000h, 012h, 0D4h, 02Ch, 0C2h, 0E0h, 00Ch, 002h, 013h, 002h
        db  004h, 060h, 0B5h, 09Ch, 007h, 0A0h, 00Ch, 002h, 080h, 000h, 010h, 03Ah, 0C2h, 0E0h, 000h, 012h
        db  002h, 08Bh, 0D4h, 02Ch, 016h, 035h, 004h, 060h, 0D4h, 0A2h, 0C2h, 0E0h, 000h, 012h, 002h, 08Bh
        db  0D4h, 02Ch, 016h, 02Eh, 004h, 060h, 0D4h, 0CAh, 0C0h, 041h, 013h, 002h, 006h, 0A0h, 0FCh, 0BAh
        db  004h, 060h, 0D5h, 076h, 0C0h, 041h, 013h, 008h, 0C8h, 001h, 001h, 06Ch, 0C2h, 0E0h, 0FCh, 002h
        db  011h, 003h, 004h, 0CCh, 004h, 060h, 0D4h, 0C2h, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 0ECh
        db  0C3h, 001h, 013h, 014h, 001h, 020h, 009h, 018h, 000h, 006h, 016h, 004h, 001h, 0A0h, 001h, 082h
        db  000h, 006h, 010h, 003h, 001h, 0E0h, 001h, 082h, 000h, 006h, 0C8h, 001h, 001h, 08Ah, 0C8h, 001h
        db  008h, 0CAh, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h, 004h, 060h, 0B5h, 09Ch, 004h, 060h, 0B5h, 082h
        db  004h, 060h, 0D5h, 076h, 004h, 060h, 0B5h, 0F2h, 001h, 000h, 0C0h, 000h, 016h, 002h, 004h, 060h
        db  0D5h, 0FAh, 004h, 060h, 0B6h, 0C4h, 004h, 060h, 0D6h, 07Eh, 004h, 060h, 0D6h, 0CEh, 003h, 000h
        db  000h, 002h, 0C0h, 0E0h, 001h, 06Ch, 0C1h, 0E0h, 010h, 08Ch, 088h, 007h, 010h, 088h, 013h, 032h
        db  0C2h, 007h, 005h, 087h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0C2h, 028h, 010h, 08Eh, 0C2h, 0C8h
        db  0C8h, 008h, 001h, 06Ch, 0C8h, 004h, 0FCh, 004h, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0A2h, 008h
        db  0CAh, 00Ah, 00Ch, 032h, 0C2h, 0A0h, 001h, 06Ch, 0C2h, 020h, 0FCh, 000h, 016h, 0F1h, 088h, 007h
        db  010h, 088h, 013h, 00Ch, 0C2h, 007h, 005h, 087h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0C2h, 028h
        db  010h, 08Eh, 0C8h, 008h, 0FCh, 000h, 010h, 0E4h, 0C0h, 04Bh, 010h, 009h, 0C0h, 041h, 013h, 0FCh
        db  0C8h, 002h, 001h, 06Ch, 0C8h, 00Bh, 0FCh, 000h, 0A2h, 0CBh, 0CAh, 0C2h, 00Ch, 032h, 0C0h, 08Ah
        db  0C8h, 007h, 010h, 08Ch, 0C3h, 020h, 001h, 08Ah, 001h, 020h, 001h, 082h, 000h, 006h, 013h, 00Ah
        db  001h, 060h, 001h, 082h, 000h, 004h, 013h, 006h, 004h, 060h, 0D3h, 0C2h, 001h, 080h, 000h, 020h
        db  004h, 060h, 0B9h, 038h, 001h, 060h, 001h, 09Ch, 000h, 040h, 013h, 0A4h, 083h, 001h, 013h, 054h
        db  001h, 040h, 000h, 040h, 016h, 0A1h, 0C1h, 041h, 004h, 0C6h, 0C8h, 001h, 001h, 06Ch, 0C2h, 020h
        db  0FCh, 002h, 0D0h, 020h, 0FCh, 007h, 028h, 020h, 0E0h, 006h, 001h, 000h, 0C0h, 000h, 016h, 09Bh
        db  088h, 001h, 008h, 0CAh, 013h, 09Ah, 001h, 048h, 001h, 000h, 016h, 05Dh, 026h, 020h, 016h, 034h
        db  013h, 043h, 001h, 008h, 05Eh, 088h, 016h, 042h, 068h, 004h, 0FCh, 004h, 005h, 020h, 0FCh, 004h
        db  0C1h, 081h, 0C0h, 060h, 0FCh, 000h, 083h, 001h, 016h, 0E0h, 006h, 020h, 016h, 032h, 016h, 00Dh
        db  0C3h, 020h, 001h, 084h, 001h, 0A0h, 001h, 082h, 020h, 000h, 001h, 0E0h, 001h, 082h, 020h, 000h
        db  0C8h, 00Ch, 001h, 084h, 007h, 0A0h, 016h, 032h, 000h, 032h, 0C3h, 020h, 001h, 08Ah, 013h, 005h
        db  002h, 08Ch, 000h, 043h, 013h, 002h, 083h, 001h, 016h, 0C8h, 0C1h, 086h, 013h, 015h, 0C2h, 020h
        db  010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0CAh, 005h, 010h, 08Eh, 004h, 0E0h, 0FCh, 000h
        db  005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 006h, 007h, 0A0h, 001h, 004h
        db  0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 0C8h, 003h, 001h, 06Ch, 001h, 040h, 000h, 020h
        db  013h, 09Dh, 003h, 090h, 011h, 0FFh, 003h, 080h, 004h, 060h, 0D6h, 0D8h, 001h, 008h, 05Eh, 000h
        db  016h, 077h, 001h, 048h, 000h, 001h, 016h, 0B8h, 0C2h, 060h, 0FCh, 008h, 011h, 0B5h, 0C2h, 060h
        db  0FCh, 00Eh, 013h, 004h, 015h, 003h, 0C2h, 060h, 0FCh, 014h, 011h, 0AEh, 007h, 08Ah, 000h, 080h
        db  006h, 0A0h, 0B5h, 01Eh, 010h, 0A9h, 0C2h, 041h, 0C0h, 060h, 0FCh, 000h, 0C1h, 086h, 013h, 017h
        db  0C2h, 020h, 010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0CAh, 005h, 010h, 08Eh, 0C8h, 006h
        db  001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h
        db  013h, 006h, 007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 0C8h, 004h
        db  008h, 01Eh, 0C1h, 049h, 001h, 080h, 000h, 040h, 083h, 001h, 013h, 0BEh, 0C8h, 001h, 001h, 06Ch
        db  0C2h, 020h, 0FCh, 002h, 001h, 048h, 001h, 000h, 016h, 05Bh, 001h, 0C0h, 000h, 040h, 026h, 020h
        db  016h, 034h, 013h, 003h, 001h, 000h, 000h, 004h, 013h, 004h, 001h, 0C8h, 040h, 000h, 001h, 080h
        db  000h, 005h, 001h, 008h, 05Eh, 088h, 013h, 019h, 001h, 008h, 05Eh, 000h, 016h, 044h, 001h, 048h
        db  000h, 001h, 016h, 013h, 0C8h, 005h, 001h, 06Ch, 0C2h, 060h, 0FCh, 008h, 011h, 006h, 0C2h, 060h
        db  0FCh, 00Eh, 013h, 003h, 015h, 002h, 0C2h, 060h, 0FCh, 014h, 0C8h, 001h, 001h, 06Ch, 0C2h, 049h
        db  011h, 004h, 007h, 08Ah, 000h, 080h, 006h, 0A0h, 0B5h, 01Eh, 0C1h, 081h, 0C0h, 060h, 0FCh, 000h
        db  0C2h, 020h, 0FCh, 004h, 062h, 004h, 005h, 008h, 0C8h, 008h, 0FCh, 004h, 0A2h, 020h, 008h, 01Eh
        db  0C8h, 005h, 001h, 06Ch, 0C8h, 008h, 0FCh, 004h, 0C8h, 006h, 001h, 06Ch, 004h, 060h, 0D5h, 014h
        db  0C1h, 086h, 013h, 017h, 0C2h, 020h, 010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0CAh, 005h
        db  010h, 08Eh, 0C8h, 006h, 001h, 06Ch, 004h, 0E0h, 0FCh, 000h, 005h, 0A0h, 010h, 080h, 001h, 060h
        db  007h, 058h, 040h, 000h, 013h, 006h, 007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h
        db  040h, 000h, 004h, 060h, 0B6h, 0C4h, 0C1h, 081h, 0C0h, 060h, 0FCh, 000h, 004h, 060h, 0B8h, 086h
        db  0A8h, 004h, 008h, 01Eh, 0C0h, 060h, 0FCh, 000h, 083h, 001h, 016h, 098h, 004h, 060h, 0D5h, 076h
        db  004h, 0E0h, 008h, 0CAh, 001h, 048h, 001h, 000h, 016h, 022h, 0C2h, 001h, 0C0h, 060h, 0FCh, 000h
        db  004h, 0E0h, 0FCh, 000h, 007h, 0A0h, 0FCh, 002h, 080h, 000h, 0C8h, 004h, 0FCh, 004h, 0C0h, 041h
        db  013h, 01Ah, 0C3h, 00Ch, 016h, 001h, 0C3h, 008h, 0C8h, 002h, 001h, 06Ch, 0C8h, 008h, 0FCh, 000h
        db  0A2h, 008h, 0CAh, 002h, 00Ch, 032h, 0C0h, 0A0h, 0FCh, 000h, 0C8h, 006h, 001h, 06Ch, 016h, 003h
        db  0C1h, 041h, 004h, 060h, 0D5h, 014h, 0C8h, 001h, 0FCh, 000h, 004h, 060h, 0D5h, 014h, 001h, 0C0h
        db  000h, 004h, 004h, 060h, 0D5h, 0B4h, 0C0h, 048h, 007h, 08Bh, 0D5h, 048h, 004h, 060h, 0FCh, 0B8h
        db  001h, 060h, 007h, 058h, 002h, 000h, 013h, 002h, 004h, 060h, 0B8h, 0A2h, 0A8h, 020h, 0E0h, 01Eh
        db  0FCh, 004h, 0C2h, 020h, 0FCh, 004h, 0C8h, 005h, 001h, 06Ch, 010h, 004h, 0A2h, 004h, 0C8h, 020h
        db  0FCh, 000h, 001h, 06Ch, 001h, 060h, 0FCh, 002h, 001h, 000h, 016h, 0F8h, 0C8h, 005h, 001h, 06Ch
        db  0C8h, 008h, 0FCh, 004h, 0C2h, 020h, 010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0CAh, 005h
        db  010h, 08Eh, 005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 006h, 007h, 0A0h
        db  001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 004h, 060h, 0B5h, 066h, 000h, 000h
        db  0F6h, 0F8h, 007h, 0FAh, 0A8h, 0C3h, 0A9h, 0A0h, 0CFh, 0ECh, 0E9h, 0E3h, 0EFh, 0EDh, 0A0h, 0B1h
        db  0B9h, 0B9h, 0B4h, 000h, 000h, 000h, 002h, 00Dh, 010h, 000h, 020h, 00Dh, 016h, 00Fh, 002h, 00Dh
        db  008h, 000h, 001h, 040h, 004h, 000h, 013h, 00Ah, 002h, 00Dh, 004h, 000h, 001h, 040h, 002h, 000h
        db  016h, 005h, 002h, 00Dh, 040h, 000h, 001h, 04Fh, 000h, 001h, 016h, 001h, 004h, 05Bh, 001h, 0C0h
        db  004h, 000h, 001h, 085h, 000h, 040h, 004h, 020h, 0EAh, 07Ch, 003h, 000h, 000h, 002h, 0C2h, 060h
        db  010h, 08Ch, 088h, 009h, 010h, 088h, 013h, 041h, 002h, 0E0h, 000h, 0E0h, 0C0h, 0E0h, 001h, 06Ch
        db  0C1h, 0E0h, 010h, 08Ch, 0C2h, 007h, 005h, 087h, 002h, 048h, 001h, 0FFh, 0A2h, 008h, 0C2h, 028h
        db  010h, 08Eh, 0C2h, 0C8h, 0C8h, 008h, 001h, 06Ch, 0C8h, 004h, 0FCh, 004h, 007h, 0A0h, 0FCh, 002h
        db  080h, 000h, 0A2h, 008h, 0CAh, 00Ah, 00Ch, 032h, 0C2h, 0A0h, 001h, 06Ch, 0C2h, 020h, 0FCh, 000h
        db  016h, 0F1h, 088h, 007h, 010h, 088h, 013h, 00Ah, 0C2h, 007h, 005h, 087h, 002h, 048h, 001h, 0FFh
        db  0A2h, 008h, 0C2h, 028h, 010h, 08Eh, 0C8h, 008h, 0FCh, 000h, 010h, 0E4h, 0C0h, 041h, 013h, 009h
        db  0C8h, 002h, 001h, 06Ch, 0C8h, 00Bh, 0FCh, 000h, 0A2h, 0CBh, 0CAh, 0C2h, 00Ch, 032h, 0C0h, 08Ah
        db  010h, 002h, 0C0h, 08Ah, 0C0h, 04Bh, 006h, 0A0h, 0FCh, 0BAh, 0C8h, 007h, 010h, 08Ch, 0C8h, 003h
        db  001h, 06Ch, 002h, 0E0h, 000h, 0C0h, 004h, 060h, 0B4h, 06Ah, 004h, 060h, 0B4h, 06Ah, 007h, 0A0h
        db  000h, 02Ah, 0F7h, 03Eh, 007h, 0A0h, 001h, 08Ah, 000h, 043h, 004h, 060h, 0B6h, 062h, 007h, 0A0h
        db  000h, 02Ah, 0F7h, 03Eh, 0C8h, 00Ah, 001h, 08Ah, 004h, 05Bh, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h
        db  007h, 0A0h, 00Ch, 002h, 080h, 000h, 004h, 060h, 0C1h, 076h, 0C8h, 004h, 001h, 06Ah, 010h, 003h
        db  0C8h, 020h, 0F8h, 000h, 001h, 06Ah, 0C3h, 020h, 0F8h, 002h, 001h, 04Ch, 001h, 000h, 016h, 0F8h
        db  0C1h, 060h, 010h, 084h, 002h, 045h, 000h, 07Fh, 006h, 0CCh, 0D9h, 04Ch, 015h, 08Eh, 013h, 00Dh
        db  005h, 0A0h, 010h, 084h, 006h, 003h, 001h, 003h, 000h, 0FFh, 013h, 052h, 0C1h, 020h, 0F8h, 000h
        db  0C8h, 004h, 001h, 06Ah, 016h, 0E8h, 006h, 0A0h, 0D1h, 0A2h, 001h, 00Ch, 000h, 0FEh, 016h, 0F0h
        db  0C3h, 020h, 001h, 06Ah, 0C8h, 004h, 001h, 06Ah, 001h, 020h, 0F8h, 00Eh, 080h, 000h, 013h, 007h
        db  0C1h, 020h, 0F8h, 014h, 001h, 084h, 060h, 0FFh, 002h, 084h, 002h, 000h, 01Bh, 033h, 0C1h, 020h
        db  0F8h, 008h, 011h, 00Ch, 088h, 020h, 0F8h, 00Ch, 001h, 0A4h, 016h, 02Fh, 088h, 020h, 0F8h, 00Ah
        db  001h, 0A2h, 016h, 02Bh, 088h, 004h, 001h, 0A0h, 013h, 025h, 010h, 027h, 0C1h, 020h, 0F8h, 00Ah
        db  011h, 009h, 025h, 020h, 001h, 0ACh, 016h, 01Eh, 0C1h, 020h, 0F8h, 00Ch, 025h, 020h, 001h, 0AEh
        db  016h, 019h, 010h, 01Bh, 002h, 084h, 0FFh, 0FFh, 016h, 00Ah, 088h, 004h, 0F8h, 00Ch, 016h, 007h
        db  041h, 020h, 0F8h, 008h, 013h, 00Fh, 002h, 084h, 03Fh, 0FFh, 013h, 00Ch, 010h, 00Eh, 088h, 004h
        db  001h, 0A8h, 016h, 00Bh, 088h, 020h, 0F8h, 00Ch, 001h, 0AAh, 016h, 007h, 088h, 020h, 0F8h, 008h
        db  001h, 0A6h, 016h, 003h, 0D9h, 060h, 0EEh, 000h, 015h, 08Eh, 0C8h, 00Ch, 001h, 06Ah, 010h, 0A8h
        db  004h, 05Bh, 003h, 000h, 000h, 002h, 0C2h, 00Bh, 0C3h, 020h, 005h, 0F4h, 016h, 008h, 0C3h, 020h
        db  008h, 036h, 001h, 00Ch, 000h, 0FFh, 013h, 01Ah, 001h, 04Ch, 004h, 000h, 013h, 017h, 0C2h, 0A8h
        db  000h, 00Ah, 001h, 04Ah, 000h, 010h, 016h, 03Eh, 001h, 04Ah, 080h, 000h, 016h, 019h, 0C3h, 008h
        db  006h, 0A0h, 0F9h, 098h, 0C2h, 00Ch, 0C3h, 020h, 005h, 0F4h, 016h, 012h, 0C3h, 020h, 008h, 036h
        db  001h, 00Ch, 000h, 0FFh, 013h, 003h, 001h, 04Ch, 004h, 000h, 016h, 00Ah, 001h, 068h, 000h, 00Ah
        db  000h, 010h, 013h, 003h, 001h, 00Ch, 001h, 000h, 016h, 025h, 006h, 0A0h, 0BBh, 0A4h, 010h, 004h
        db  007h, 087h, 005h, 0F0h, 006h, 0A0h, 0C1h, 0C6h, 001h, 0E0h, 008h, 036h, 002h, 000h, 007h, 0A0h
        db  000h, 014h, 000h, 0E0h, 007h, 0A0h, 000h, 016h, 0D2h, 080h, 003h, 090h, 0FFh, 0FFh, 003h, 080h
        db  003h, 000h, 000h, 002h, 0C2h, 020h, 008h, 036h, 016h, 00Ch, 0D8h, 020h, 008h, 0E0h, 001h, 080h
        db  0C8h, 02Bh, 000h, 006h, 001h, 08Ch, 007h, 0A0h, 008h, 0C8h, 000h, 004h, 0C8h, 00Bh, 005h, 0F4h
        db  010h, 005h, 0C2h, 00Bh, 007h, 087h, 005h, 0F0h, 006h, 0A0h, 0C1h, 080h, 001h, 0E0h, 008h, 036h
        db  002h, 000h, 007h, 0A0h, 000h, 014h, 000h, 0E0h, 007h, 0A0h, 000h, 016h, 0D2h, 080h, 003h, 090h
        db  0FFh, 0FFh, 003h, 080h, 0C2h, 020h, 005h, 0F4h, 013h, 002h, 004h, 060h, 0BBh, 068h, 002h, 0A8h
        db  0C8h, 008h, 008h, 04Ah, 002h, 0E0h, 008h, 030h, 001h, 003h, 000h, 0FFh, 013h, 060h, 001h, 043h
        db  004h, 000h, 013h, 05Dh, 001h, 0E0h, 001h, 09Ch, 040h, 000h, 00Bh, 0ABh, 00Bh, 0ABh, 001h, 060h
        db  001h, 09Ch, 040h, 000h, 016h, 054h, 0C8h, 004h, 001h, 06Ah, 0C2h, 0E0h, 0F8h, 002h, 011h, 04Ah
        db  0C3h, 084h, 0C2h, 083h, 004h, 0C3h, 004h, 0CFh, 0C3h, 00Eh, 001h, 060h, 0F8h, 000h, 040h, 000h
        db  013h, 009h, 001h, 060h, 0F8h, 002h, 080h, 000h, 013h, 013h, 0C3h, 020h, 0F8h, 000h, 0C8h, 00Ch
        db  001h, 06Ah, 010h, 0F3h, 005h, 083h, 006h, 00Ah, 0C3h, 0CCh, 0C3h, 0A0h, 0F8h, 000h, 001h, 00Eh
        db  00Fh, 0FFh, 013h, 006h, 0C8h, 00Eh, 001h, 06Ah, 001h, 060h, 0F8h, 002h, 080h, 000h, 016h, 0E3h
        db  0C8h, 00Fh, 001h, 06Ah, 013h, 02Eh, 0C8h, 020h, 0F8h, 000h, 001h, 06Ah, 001h, 060h, 0F8h, 002h
        db  080h, 000h, 016h, 004h, 007h, 0A0h, 0F8h, 002h, 080h, 000h, 010h, 0F5h, 0C8h, 00Fh, 001h, 06Ah
        db  007h, 0A0h, 0F8h, 000h, 040h, 000h, 0C8h, 009h, 001h, 06Ah, 0C8h, 004h, 0F8h, 000h, 0C2h, 04Ch
        db  0C3h, 0C5h, 0E8h, 003h, 008h, 028h, 0C1h, 060h, 008h, 050h, 002h, 085h, 0D2h, 004h, 016h, 002h
        db  006h, 0A0h, 0F7h, 0FEh, 0C0h, 0CAh, 0C1h, 00Eh, 0C1h, 04Fh, 007h, 08Ah, 001h, 08Ch, 001h, 003h
        db  000h, 0FFh, 013h, 003h, 001h, 0C3h, 004h, 000h, 010h, 002h, 002h, 043h, 003h, 000h, 000h, 09Dh
        db  004h, 05Bh, 0C8h, 004h, 001h, 06Ah, 0C8h, 020h, 0F8h, 000h, 001h, 06Ah, 001h, 060h, 0F8h, 002h
        db  080h, 000h, 016h, 0F0h, 007h, 0A0h, 0F8h, 002h, 080h, 000h, 010h, 0F5h, 003h, 000h, 000h, 002h
        db  007h, 087h, 005h, 0F0h, 0C2h, 0CBh, 016h, 00Fh, 006h, 0A0h, 0F9h, 098h, 001h, 0E0h, 008h, 036h
        db  001h, 000h, 007h, 0A0h, 000h, 014h, 000h, 0E0h, 007h, 0A0h, 000h, 016h, 0D2h, 080h, 0CBh, 060h
        db  005h, 0F4h, 000h, 002h, 003h, 080h, 0C2h, 020h, 008h, 036h, 001h, 088h, 001h, 000h, 0C8h, 008h
        db  008h, 036h, 001h, 008h, 004h, 000h, 016h, 003h, 001h, 008h, 000h, 0FFh, 016h, 008h, 0C2h, 020h
        db  005h, 0F4h, 016h, 005h, 0C2h, 017h, 013h, 004h, 0C5h, 0D8h, 006h, 0A0h, 0BBh, 0A4h, 003h, 080h
        db  002h, 0E0h, 008h, 030h, 001h, 083h, 006h, 000h, 016h, 005h, 0C1h, 042h, 013h, 008h, 0C0h, 0C0h
        db  0C1h, 001h, 004h, 0C2h, 0D8h, 020h, 008h, 0E0h, 001h, 080h, 0C8h, 004h, 001h, 08Ch, 002h, 0E0h
        db  000h, 0C0h, 007h, 0A0h, 000h, 014h, 008h, 030h, 0C8h, 020h, 008h, 050h, 000h, 016h, 003h, 080h
        db  003h, 000h, 000h, 002h, 007h, 009h, 0C2h, 0A0h, 001h, 004h, 001h, 08Ah, 000h, 080h, 052h, 04Ah
        db  0D2h, 089h, 0C8h, 00Ah, 001h, 004h, 0D2h, 0A0h, 001h, 004h, 016h, 0F9h, 002h, 089h, 05Fh, 0FFh
        db  016h, 01Bh, 001h, 060h, 000h, 0E0h, 000h, 002h, 013h, 004h, 088h, 020h, 010h, 080h, 010h, 088h
        db  013h, 009h, 007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0A0h, 000h, 0E0h, 000h, 002h, 003h, 090h
        db  0FFh, 0FFh, 003h, 080h, 001h, 0A0h, 007h, 058h, 040h, 000h, 001h, 020h, 007h, 058h, 020h, 0FEh
        db  016h, 02Eh, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 005h, 049h, 001h, 089h, 080h, 000h, 001h, 049h
        db  040h, 000h, 013h, 02Fh, 001h, 049h, 020h, 000h, 016h, 01Bh, 001h, 060h, 000h, 0E0h, 000h, 002h
        db  013h, 007h, 001h, 049h, 001h, 000h, 013h, 00Bh, 088h, 020h, 010h, 080h, 010h, 088h, 013h, 007h
        db  007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0A0h, 000h, 0E0h, 000h, 002h, 010h, 007h, 001h, 0A0h
        db  007h, 058h, 040h, 000h, 001h, 020h, 007h, 058h, 020h, 0FEh, 016h, 002h, 001h, 089h, 020h, 000h
        db  001h, 089h, 001h, 000h, 0D2h, 049h, 016h, 005h, 003h, 090h, 0FFh, 0FFh, 003h, 080h, 007h, 089h
        db  020h, 000h, 0F8h, 009h, 007h, 03Ah, 0C2h, 060h, 007h, 036h, 013h, 0F6h, 004h, 0E0h, 007h, 036h
        db  004h, 054h, 004h, 060h, 0B4h, 058h, 003h, 000h, 000h, 002h, 001h, 040h, 040h, 000h, 016h, 007h
        db  001h, 0DDh, 000h, 080h, 001h, 0C0h, 020h, 000h, 003h, 000h, 000h, 00Fh, 004h, 05Bh, 003h, 000h
        db  000h, 00Fh, 004h, 060h, 0C4h, 070h, 003h, 000h, 000h, 00Fh, 0C0h, 082h, 010h, 004h, 006h, 0A0h
        db  0C6h, 068h, 001h, 0C2h, 0FFh, 080h, 003h, 000h, 000h, 002h, 001h, 040h, 040h, 000h, 013h, 0F3h
        db  0C8h, 002h, 001h, 004h, 001h, 0C0h, 040h, 000h, 003h, 000h, 000h, 00Fh, 004h, 059h, 003h, 000h
        db  000h, 002h, 001h, 040h, 040h, 000h, 013h, 00Bh, 007h, 0A0h, 001h, 004h, 0FFh, 086h, 002h, 060h
        db  040h, 000h, 001h, 085h, 058h, 000h, 003h, 000h, 000h, 00Fh, 004h, 060h, 0C4h, 036h, 002h, 060h
        db  000h, 080h, 002h, 065h, 058h, 000h, 010h, 0F7h, 0C0h, 0E0h, 007h, 05Ch, 0C8h, 003h, 001h, 06Ch
        db  0C3h, 060h, 0FCh, 004h, 0C2h, 0C3h, 0A2h, 0CBh, 0C2h, 0EBh, 00Ch, 032h, 0C8h, 00Bh, 0FCh, 000h
        db  013h, 00Ah, 0C8h, 00Bh, 001h, 06Ch, 0A3h, 060h, 0FCh, 004h, 0C2h, 0E0h, 0FCh, 000h, 016h, 0F9h
        db  0A8h, 020h, 0E0h, 01Eh, 0FCh, 004h, 002h, 02Dh, 000h, 004h, 0C8h, 003h, 001h, 06Ch, 0C8h, 00Dh
        db  0FCh, 004h, 003h, 000h, 000h, 002h, 0C2h, 020h, 010h, 080h, 002h, 048h, 001h, 0FFh, 0A2h, 008h
        db  0CAh, 003h, 010h, 08Eh, 005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 006h
        db  007h, 0A0h, 001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 003h, 000h, 000h, 00Fh
        db  004h, 020h, 0EAh, 07Ch, 0C0h, 081h, 001h, 020h, 001h, 082h, 000h, 006h, 013h, 026h, 001h, 020h
        db  001h, 082h, 000h, 004h, 016h, 022h, 007h, 0A0h, 016h, 02Eh, 003h, 0E8h, 001h, 0E0h, 001h, 09Ch
        db  000h, 040h, 006h, 020h, 016h, 02Eh, 013h, 021h, 001h, 060h, 001h, 09Ch, 000h, 040h, 016h, 0F9h
        db  001h, 060h, 00Ch, 002h, 001h, 000h, 013h, 015h, 001h, 020h, 009h, 018h, 000h, 006h, 016h, 004h
        db  001h, 0A0h, 001h, 082h, 000h, 002h, 010h, 003h, 001h, 0E0h, 001h, 082h, 000h, 006h, 00Bh, 0BBh
        db  00Bh, 0BBh, 00Bh, 0ABh, 007h, 0A0h, 00Ch, 002h, 080h, 000h, 0C8h, 001h, 001h, 08Ah, 0C8h, 001h
        db  008h, 0CAh, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h, 004h, 05Bh, 007h, 0A0h, 0E1h, 09Eh, 0D3h, 04Ah
        db  007h, 0A0h, 000h, 012h, 0D3h, 04Ah, 007h, 020h, 016h, 030h, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h
        db  004h, 05Bh, 001h, 060h, 016h, 030h, 0FFh, 0FFh, 013h, 01Ah, 007h, 0A0h, 016h, 02Eh, 003h, 0E8h
        db  001h, 0E0h, 001h, 09Ch, 000h, 040h, 006h, 020h, 016h, 02Eh, 013h, 006h, 001h, 060h, 001h, 09Ch
        db  000h, 040h, 016h, 0F9h, 004h, 060h, 0C1h, 062h, 007h, 0A0h, 0E1h, 09Eh, 0D3h, 04Ah, 007h, 0A0h
        db  000h, 012h, 0D3h, 04Ah, 007h, 0A0h, 000h, 02Ah, 0B4h, 066h, 007h, 020h, 016h, 030h, 004h, 060h
        db  0C1h, 042h, 001h, 0E0h, 0FCh, 002h, 000h, 020h, 007h, 08Bh, 0B8h, 07Eh, 004h, 060h, 0C1h, 096h
        db  007h, 08Fh, 010h, 080h, 004h, 0FFh, 002h, 08Fh, 018h, 000h, 016h, 0FCh, 007h, 08Fh, 008h, 022h
        db  004h, 0FFh, 002h, 08Fh, 008h, 050h, 016h, 0FCh, 007h, 0A0h, 008h, 044h, 001h, 08Ch, 007h, 08Fh
        db  0D1h, 034h, 0C8h, 00Fh, 008h, 050h, 007h, 0A0h, 017h, 0A0h, 017h, 0A0h, 007h, 0A0h, 017h, 0A2h
        db  0FEh, 03Eh, 004h, 0E0h, 017h, 0A6h, 004h, 0E0h, 016h, 030h, 004h, 060h, 0C4h, 036h, 007h, 0A0h
        db  016h, 032h, 000h, 032h, 004h, 060h, 0C6h, 0D8h, 007h, 020h, 016h, 032h, 004h, 060h, 0C9h, 048h
        db  001h, 060h, 007h, 058h, 040h, 000h, 016h, 004h, 001h, 060h, 001h, 004h, 020h, 000h, 016h, 0FCh
        db  007h, 0A0h, 001h, 004h, 0FFh, 080h, 004h, 060h, 0B3h, 0D2h, 002h, 02Eh, 000h, 011h, 0C2h, 060h
        db  001h, 062h, 004h, 0E0h, 001h, 062h, 007h, 08Ch, 008h, 0E6h, 0CFh, 01Eh, 011h, 018h, 0C3h, 05Eh
        db  0C3h, 0FEh, 0CFh, 01Eh, 0E3h, 05Eh, 0A3h, 0FEh, 0CFh, 01Eh, 0E3h, 05Eh, 013h, 010h, 0A3h, 0FEh
        db  083h, 0FEh, 016h, 00Dh, 007h, 08Fh, 000h, 004h, 0C3h, 07Eh, 005h, 04Dh, 083h, 06Eh, 0FFh, 0F6h
        db  016h, 006h, 006h, 00Fh, 016h, 0F9h, 0C8h, 009h, 001h, 062h, 004h, 060h, 0C3h, 0FEh, 0C8h, 009h
        db  001h, 062h, 007h, 020h, 008h, 0E6h, 004h, 060h, 0C3h, 0FEh, 003h, 000h, 000h, 002h, 0C0h, 0A0h
        db  001h, 06Ah, 0C3h, 00Eh, 009h, 0ACh, 0C8h, 00Ch, 001h, 06Ah, 0C3h, 00Eh, 002h, 04Ch, 003h, 0FFh
        db  0D8h, 0ECh, 0F8h, 000h, 017h, 0C0h, 005h, 083h, 002h, 043h, 000h, 03Fh, 0D8h, 0ECh, 0F8h, 001h
        db  017h, 0C0h, 0C8h, 002h, 001h, 06Ah, 005h, 0CEh, 003h, 090h, 0EFh, 0FFh, 003h, 080h, 004h, 0CBh
        db  010h, 004h, 0A2h, 0C4h, 0C8h, 020h, 0FCh, 000h, 001h, 06Ch, 001h, 060h, 0FCh, 002h, 001h, 000h
        db  016h, 0F8h, 0A8h, 020h, 0E0h, 01Eh, 0FCh, 004h, 0A2h, 0E0h, 0FCh, 004h, 0C8h, 00Ah, 001h, 06Ch
        db  0C8h, 00Bh, 0FCh, 004h, 0C2h, 0E0h, 010h, 080h, 002h, 04Bh, 001h, 0FFh, 0A2h, 0CBh, 0CAh, 0CAh
        db  010h, 08Eh, 005h, 0A0h, 010h, 080h, 001h, 060h, 007h, 058h, 040h, 000h, 013h, 006h, 007h, 0A0h
        db  001h, 004h, 0FFh, 08Ch, 001h, 0E0h, 007h, 058h, 040h, 000h, 003h, 090h, 000h, 07Fh, 003h, 080h
        db  001h, 060h, 009h, 018h, 000h, 004h, 013h, 005h, 007h, 020h, 016h, 034h, 007h, 08Dh, 080h, 000h
        db  004h, 052h, 001h, 060h, 009h, 018h, 000h, 002h, 013h, 0F7h, 007h, 0A0h, 016h, 034h, 000h, 003h
        db  010h, 0F5h, 0BAh, 0BEh, 0FAh, 0CEh, 042h, 041h, 042h, 045h, 046h, 041h, 043h, 045h, 000h, 000h
        db  0B2h, 02Ch, 000h, 004h, 002h, 002h, 006h, 003h, 000h, 000h, 0B2h, 090h, 000h, 002h, 00Bh, 0FFh
        db  000h, 000h, 0B3h, 0BEh, 000h, 004h, 004h, 060h, 0FDh, 0D4h, 000h, 000h, 0B5h, 05Eh, 000h, 004h
        db  004h, 060h, 0D4h, 02Ch, 000h, 000h, 0B5h, 066h, 000h, 004h, 004h, 060h, 0D3h, 09Ah, 000h, 000h
        db  0B6h, 052h, 000h, 004h, 004h, 060h, 0F7h, 0D2h, 000h, 000h, 0B6h, 062h, 000h, 004h, 004h, 060h
        db  0D3h, 0A8h, 000h, 000h, 0B7h, 0FEh, 000h, 004h, 004h, 060h, 0D7h, 02Eh, 000h, 000h, 0B8h, 07Ah
        db  000h, 004h, 004h, 060h, 0FDh, 076h, 000h, 000h, 0B9h, 018h, 000h, 004h, 004h, 060h, 0D2h, 0FEh
        db  000h, 000h, 0BAh, 0F6h, 000h, 004h, 006h, 0A0h, 0F9h, 098h, 000h, 000h, 0BBh, 084h, 000h, 004h
        db  0C1h, 0E0h, 0FCh, 002h, 000h, 000h, 0BEh, 062h, 000h, 004h, 007h, 08Bh, 04Eh, 000h, 000h, 000h
        db  0C0h, 0E6h, 000h, 004h, 004h, 060h, 0F7h, 0E2h, 000h, 000h, 0C1h, 054h, 000h, 004h, 004h, 060h
        db  0FDh, 036h, 000h, 000h, 0C1h, 070h, 000h, 004h, 004h, 060h, 0F7h, 0EEh, 000h, 000h, 0C2h, 022h
        db  000h, 006h, 001h, 020h, 001h, 000h, 000h, 080h, 000h, 000h, 0C3h, 0ECh, 000h, 004h, 004h, 060h
        db  0FDh, 0EEh, 000h, 000h, 0C4h, 01Eh, 000h, 004h, 004h, 060h, 0FDh, 084h, 000h, 000h, 0C4h, 03Ah
        db  000h, 004h, 004h, 060h, 0C4h, 054h, 000h, 000h, 0C4h, 060h, 000h, 004h, 004h, 060h, 0FBh, 0DAh
        db  000h, 000h, 0C4h, 088h, 000h, 004h, 004h, 060h, 0FCh, 002h, 000h, 000h, 0C7h, 002h, 000h, 004h
        db  007h, 089h, 000h, 060h, 000h, 000h, 0C9h, 038h, 000h, 004h, 004h, 060h, 0FEh, 0C4h, 000h, 000h
        db  0CBh, 000h, 000h, 004h, 007h, 08Dh, 000h, 060h, 000h, 000h, 0CBh, 08Eh, 000h, 004h, 004h, 060h
        db  0FEh, 0C4h, 000h, 000h, 0A0h, 002h, 000h, 002h, 07Ch, 097h, 000h, 000h, 0A0h, 07Ch, 000h, 002h
        db  046h, 05Dh, 000h, 000h, 0A0h, 034h, 000h, 00Ah, 000h, 002h, 023h, 0F1h, 0F1h, 0F0h, 0F0h, 0E7h
        db  0E7h, 0E7h, 000h, 000h, 0A0h, 0AEh, 000h, 002h, 008h, 022h, 000h, 000h, 0A0h, 0B2h, 000h, 002h
        db  0CDh, 030h, 000h, 000h, 0A1h, 018h, 000h, 002h, 0D4h, 02Ch, 000h, 000h, 0A1h, 01Ch, 000h, 002h
        db  0D2h, 080h, 000h, 000h, 0A1h, 024h, 000h, 002h, 0FBh, 014h, 000h, 000h, 0A1h, 070h, 000h, 002h
        db  0F8h, 0D6h, 000h, 000h, 0A1h, 074h, 000h, 002h, 0FAh, 090h, 000h, 000h, 0A1h, 07Ch, 000h, 002h
        db  0F9h, 054h, 000h, 000h, 0A2h, 0E6h, 000h, 004h, 016h, 00Eh, 0FCh, 04Ch, 000h, 000h, 0A2h, 0CCh
        db  000h, 002h, 0C4h, 0A6h, 000h, 000h, 0A2h, 0D4h, 000h, 002h, 0FCh, 022h, 000h, 000h, 0A2h, 0F4h
        db  000h, 002h, 0FCh, 022h, 000h, 000h, 0A3h, 014h, 000h, 002h, 0FDh, 0C2h, 000h, 000h, 0A3h, 01Ah
        db  000h, 002h, 0F7h, 00Ah, 000h, 000h, 0A3h, 01Ch, 000h, 002h, 0FDh, 0CCh, 000h, 000h, 0AEh, 000h
        db  000h, 002h, 0CCh, 000h, 000h, 000h, 0F1h, 0F2h, 000h, 002h, 000h, 004h, 000h, 000h, 0F2h, 0A8h
        db  000h, 002h, 000h, 004h, 000h, 000h, 0F2h, 0DCh, 000h, 002h, 000h, 004h, 000h, 000h, 0F2h, 028h
        db  000h, 002h, 004h, 000h, 000h, 000h, 0F2h, 046h, 000h, 002h, 004h, 000h, 0DEh, 0ADh, 0BEh, 0EFh
        db  000h, 000h, 044h, 045h, 041h, 044h, 042h, 045h, 045h, 046h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2710  R-X
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        dec    eax
        cmp    eax, 0xe
        ja     loc_2_007A
        jmp    dword ptr [eax*4 + 0x80]      ; -> obj2:0x0080
loc_2_0014:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x545]
        push   eax
        call   sub_2_2270
        jmp    loc_2_0028
loc_2_0025:
        mov    esi, dword ptr [ebp + 8]
loc_2_0028:
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_0894
loc_2_0034:
        jmp    loc_2_0039
loc_2_0036:
        mov    esi, dword ptr [ebp + 8]
loc_2_0039:
        push   1
        push   1
        push   esi
        call   sub_4_1BE0
loc_2_0043:
        jmp    loc_2_0048
loc_2_0045:
        mov    esi, dword ptr [ebp + 8]
loc_2_0048:
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_07D8
loc_2_0054:
        jmp    loc_2_0059
loc_2_0056:
        mov    esi, dword ptr [ebp + 8]
loc_2_0059:
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_2_006D
loc_2_006A:
        mov    esi, dword ptr [ebp + 8]
loc_2_006D:
        push   0
        push   0x683
        push   esi
        call   sub_1_08AC
loc_2_007A:
        pop    esi
        pop    ebp
        ret    8
        db  090h
        ; ---- dispatch table (15 entries) ----
        dd  loc_2_0014
        dd  loc_2_0025
        dd  loc_2_0025
        dd  loc_2_0036
        dd  loc_2_0025
        dd  loc_2_0056
        dd  loc_2_0056
        dd  loc_2_0045
        dd  loc_2_007A
        dd  loc_2_007A
        dd  loc_2_007A
        dd  loc_2_0045
        dd  loc_2_006A
        dd  loc_2_0056
        dd  loc_2_006A
        db  08Dh, 064h, 024h, 000h
sub_2_00C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        cmp    byte ptr [0x1c], 0            ; -> obj1:0x779C
        jne    loc_2_0130
        push   4
        mov    esi, dword ptr [ebp + 8]
        push   0xfffd9
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0972
loc_2_00EE:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0144
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax], 0x45
        jne    loc_2_011F
        cmp    byte ptr [eax + 1], 0x49
        jne    loc_2_011F
        cmp    byte ptr [eax + 2], 0x53
        jne    loc_2_011F
        cmp    byte ptr [eax + 3], 0x41
        jne    loc_2_011F
        mov    dword ptr [0x18], 1           ; -> obj1:0x7798
        mov    dword ptr [esi + 0x46], 1
loc_2_011F:
        mov    byte ptr [0x1c], 1            ; -> obj1:0x779C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0130:
        cmp    dword ptr [0x18], 1           ; -> obj1:0x7798
        jne    loc_2_0144
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [0x18]         ; -> obj1:0x7798
        mov    dword ptr [esi + 0x46], eax
loc_2_0144:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0150:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        mov    word ptr [ebp - 0x10], 0xa20
        mov    word ptr [ebp - 0xe], 0xa24
        mov    word ptr [ebp - 0xc], 0xa50
        mov    word ptr [ebp - 0xa], 0xa54
        sub    bl, bl
        mov    word ptr [ebp - 8], 0x1a20
        mov    word ptr [ebp - 6], 0x1a24
        mov    word ptr [ebp - 4], 0x1a50
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 2], 0x1a54
        mov    eax, dword ptr [esi + 0x4f7]
        cmp    eax, 7
        jbe    loc_2_01D2
        mov    dword ptr [esi + 0x4ba], 0xaa00000a
        mov    dword ptr [esi + 0x4be], 5
        mov    dword ptr [esi + 0x4c2], 2
        mov    dword ptr [esi + 0x4c6], 0x1000
        mov    bl, 1
        mov    dword ptr [esi + 0x4ca], eax
        mov    dword ptr [esi + 0x4f7], 0xff
loc_2_01D2:
        mov    eax, dword ptr [esi + 0x4f7]
        mov    eax, dword ptr [ebp + eax*2 - 0x10]
        and    eax, 0xffff
        mov    dword ptr [esi + 0x4fb], eax
        push   esi
        call   sub_2_15E0
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_2_024D
        cmp    eax, 6
        mov    eax, dword ptr [ebp + 0xc]
        jne    loc_2_0216
        mov    cl, byte ptr [eax]
        test   cl, 0x40
        je     loc_2_0216
        test   cl, 0x80
        jne    loc_2_0216
        push   6
        push   eax
        lea    eax, [esi + 0x50]
        push   eax
        call   sub_1_098C
loc_2_0214:
        jmp    loc_2_024D
loc_2_0216:
        mov    dword ptr [esi + 0x4ba], 0xaa000009
        mov    dword ptr [esi + 0x4be], 5
        mov    dword ptr [esi + 0x4c2], 2
        mov    dword ptr [esi + 0x4c6], 0x1200
        mov    bl, 1
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    dword ptr [esi + 0x4ca], eax
loc_2_024D:
        mov    eax, dword ptr [esi + 0x3a]
        or     eax, eax
        je     loc_2_029C
        cmp    eax, 0x200
        jb     loc_2_0262
        cmp    eax, 0x1fd0
        jbe    loc_2_029C
loc_2_0262:
        mov    dword ptr [esi + 0x4ba], 0xaa00000a
        mov    dword ptr [esi + 0x4be], 5
        mov    dword ptr [esi + 0x4c2], 2
        mov    dword ptr [esi + 0x4c6], 0x1700
        mov    bl, 1
        mov    eax, dword ptr [esi + 0x3a]
        mov    dword ptr [esi + 0x4ca], eax
        mov    dword ptr [esi + 0x3a], 0
loc_2_029C:
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_02B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0x84]         ; -> obj1:0x0AC4
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x683
        push   eax
        call   sub_1_08A6
loc_2_02CF:
        or     eax, eax
        jne    loc_2_0857
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0x1a0
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [8]            ; -> obj1:0x7788
        mov    dword ptr [ecx + 0x503], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x4f7], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x3e], 0xb
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x24]
        mov    dword ptr [ecx + 0xc3], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x28]
        mov    dword ptr [ecx + 0xc7], eax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x2d5], 0x3f8
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x2c]
        mov    word ptr [ecx + 0x2d7], ax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp + 0x30]
        mov    byte ptr [eax + 0xe6], cl
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp + 0x34]
        mov    byte ptr [eax + 0x39], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x535], 0xf0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x38]
        mov    dword ptr [ecx + 0x3a], eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0150
        mov    bl, al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_2100
        lea    eax, [ebp - 0xc]
        push   eax
        push   0                             ; -> obj1:0x7780
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1920
        or     al, al
        je     loc_2_03A5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_14F0
loc_2_03A5:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0            ; -> obj1:0x7780
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [4]            ; -> obj1:0x7784
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [4]            ; -> obj1:0x7784
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [4], eax            ; -> obj1:0x7784
        mov    eax, dword ptr [eax + 0x3e]
        cmp    eax, 7
        je     loc_2_0416
        cmp    eax, 9
        je     loc_2_0416
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x4f7]
        cmp    ecx, 0x1f
        mov    eax, 1
        ja     loc_2_0409
        shl    eax, cl
        or     dword ptr [0x10], eax         ; -> obj1:0x7790
        jmp    loc_2_0429
loc_2_0409:
        sub    cl, 0x20
        shl    eax, cl
        or     dword ptr [0x14], eax         ; -> obj1:0x7794
        jmp    loc_2_0429
loc_2_0416:
        mov    eax, dword ptr [ebp - 4]
        mov    edx, 1
        mov    cl, byte ptr [eax + 0x2f]
        shl    edx, cl
        or     dword ptr [0x1d], edx         ; -> obj1:0x779D
loc_2_0429:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x42]
        mov    dword ptr [ecx + 0x46], eax
        lea    edi, [ebp - 0x40]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    byte ptr [ebp - 0x3c], al
        mov    dword ptr [ebp - 0x40], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x42]
        or     eax, eax
        je     loc_2_0466
        cmp    eax, 1
        je     loc_2_0470
        cmp    eax, 2
        je     loc_2_0480
        mov    dword ptr [ebp - 0x36], 1
        jmp    loc_2_048B
loc_2_0466:
        mov    dword ptr [ebp - 0x36], 1
        jmp    loc_2_048B
        db  090h
loc_2_0470:
        mov    dword ptr [ebp - 0x36], 2
        jmp    loc_2_048B
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0480:
        mov    dword ptr [ebp - 0x36], 3
        mov    byte ptr [ebp - 0x3b], 1
loc_2_048B:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_04B0
        mov    eax, dword ptr [ebp - 4]
        cmp    word ptr [eax + 0xc], 0
        jne    loc_2_04A4
        mov    dword ptr [ebp - 0x2a], 1
        jmp    loc_2_04B7
loc_2_04A4:
        mov    dword ptr [ebp - 0x2a], 2
        jmp    loc_2_04B7
        db  08Dh, 049h, 000h
loc_2_04B0:
        mov    dword ptr [ebp - 0x2a], 0
loc_2_04B7:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x26], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xa]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x22], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x1a], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x16], eax
        lea    eax, [ebp - 0x40]
        push   eax
        push   esi
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 0x503]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x4ff
        push   eax
        call   sub_1_07D2
loc_2_0518:
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    loc_2_07D0
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_05A7
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x4c2], 2
        jne    loc_2_0560
        mov    ecx, dword ptr [eax + 0x4ba]
        mov    eax, dword ptr [eax + 0x4c6]
        push   eax
        push   ecx
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_0978
loc_2_0555:
        add    esp, 0x14
        jmp    loc_2_0590
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0560:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x4ba]
        mov    eax, dword ptr [eax + 0x4ca]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x4c6]
        push   eax
        push   ecx
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_0978
loc_2_058D:
        add    esp, 0x18
loc_2_0590:
        push   0xc
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x34
loc_2_05A7:
        or     bl, bl
        je     loc_2_0610
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x4c2], 2
        jne    loc_2_05E0
        mov    ecx, dword ptr [eax + 0x4ba]
        mov    eax, dword ptr [eax + 0x4c6]
        push   eax
        push   ecx
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_0978
loc_2_05D7:
        add    esp, 0x14
        jmp    loc_2_0610
        db  08Dh, 064h, 024h, 000h
loc_2_05E0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x4ba]
        mov    eax, dword ptr [eax + 0x4ca]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x4c6]
        push   eax
        push   ecx
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_0978
loc_2_060D:
        add    esp, 0x18
loc_2_0610:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x46], 0
        jne    loc_2_061F
        push   eax
        call   sub_2_00C0
loc_2_061F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_19E0
        or     al, al
        jne    loc_2_0665
        push   0xc000001f
        push   5
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_0978
loc_2_0649:
        add    esp, 0x14
        push   8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x34
loc_2_0665:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x20]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x52d]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    eax, dword ptr [eax + 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x24]
        push   eax
        push   0                             ; -> loc_4_1EF0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0                             ; -> loc_4_1E50
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x515
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_088E
loc_2_06B2:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_06F3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0xc0000018
        push   5
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000005
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_0978
loc_2_06D9:
        add    esp, 0x18
        push   4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x34
loc_2_06F3:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_1890
loc_2_06FE:
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_2_0740
        push   eax
        push   0xc0000008
        push   5
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000003
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_0978
loc_2_0723:
        add    esp, 0x18
        push   5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x34
        db  08Dh, 049h, 000h
loc_2_0740:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x545
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x531
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x50
        push   eax
        push   0                             ; -> loc_4_2F00
        push   0                             ; -> loc_4_2E60
        push   0                             ; -> loc_4_2E20
        push   0                             ; -> loc_4_2DE0
        call   sub_2_21A0
        or     al, al
        jne    loc_2_07B0
        push   0xc000001c
        push   5
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_0978
loc_2_0793:
        add    esp, 0x14
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x34
        db  090h
loc_2_07B0:
        push   0                             ; -> loc_1_0120
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_097E
loc_2_07C5:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x34
loc_2_07D0:
        mov    byte ptr [ebp - 0x3c], 0
        mov    dword ptr [ebp - 0x40], 0
        mov    dword ptr [ebp - 0x2a], 0
        lea    eax, [ebp - 0x40]
        push   eax
        push   esi
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 0x503]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x4ff
        push   eax
        call   sub_1_07D2
loc_2_0801:
        or     eax, eax
        jne    loc_2_0840
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0xc000000c
        push   5
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_0978
loc_2_0826:
        add    esp, 0x18
        push   0xc
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x34
loc_2_0840:
        push   0xe
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x34
loc_2_0857:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x34
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0870:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x74
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 0x2b]
        mov    byte ptr [ebp - 0x2c], 0
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 4], 0
        mov    dword ptr [ebp - 0x18], 0
        mov    word ptr [ebp - 0x5c], 8
        mov    word ptr [ebp - 0x5a], 8
        mov    dword ptr [ebp - 0x58], 0x90  ; -> loc_1_0AD0
        mov    word ptr [ebp - 0x54], 0xd
        mov    word ptr [ebp - 0x52], 0xd
        mov    dword ptr [ebp - 0x50], 0xa8  ; -> loc_1_0AE8
        mov    word ptr [ebp - 0x4c], 0xa
        mov    word ptr [ebp - 0x4a], 0xa
        mov    dword ptr [ebp - 0x48], 0xc4  ; -> obj1:0x0B04
        mov    word ptr [ebp - 0x44], 8
        mov    word ptr [ebp - 0x42], 8
        mov    dword ptr [ebp - 0x40], 0xd8  ; -> obj1:0x0B18
        mov    word ptr [ebp - 0x34], 0xc
        mov    word ptr [ebp - 0x32], 0xc
        mov    dword ptr [ebp - 0x30], 0x154 ; -> loc_1_0B94
        sub    edi, edi
        mov    word ptr [ebp - 0x3c], 0xd
        mov    word ptr [ebp - 0x3a], 0xd
        mov    dword ptr [ebp - 0x38], 0x170 ; -> loc_1_0BB0
        mov    dword ptr [ebp - 0x1c], 0
        sub    bl, bl
        mov    dword ptr [ebp - 0x20], 0
        mov    dword ptr [ebp - 0x24], 0
        mov    byte ptr [ebp - 1], 1
        mov    esi, dword ptr [ebp + 0xc]
        cmp    esi, edi
        je     loc_2_0A97
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0924
loc_2_0940:
        cmp    dword ptr [ebp - 8], edi
        je     loc_2_0951
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0951:
        push   0
        lea    eax, [ebp - 0x5c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_092A
loc_2_0968:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0977
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x1c], eax
loc_2_0977:
        push   0
        lea    eax, [ebp - 0x54]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_092A
loc_2_098E:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_099A
        mov    eax, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [eax + 4]
loc_2_099A:
        push   0
        lea    eax, [ebp - 0x4c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_092A
loc_2_09B1:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_09C0
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x20], eax
loc_2_09C0:
        push   0
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_092A
loc_2_09D7:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_09E6
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x24], eax
loc_2_09E6:
        push   2
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_092A
loc_2_09FD:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0A16
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax]
        sub    al, 0x79
        cmp    al, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ebp - 1], al
loc_2_0A16:
        push   2
        lea    eax, [ebp - 0x34]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_092A
loc_2_0A2D:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0A43
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax]
        sub    al, 0x79
        cmp    al, 1
        sbb    bl, bl
        neg    bl
loc_2_0A43:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_082C
loc_2_0A58:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0A90
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_2_0A97
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax + 2]
        or     al, byte ptr [ecx + 4]
        or     al, byte ptr [ecx + 5]
        or     al, byte ptr [ecx + 1]
        or     al, byte ptr [ecx + 3]
        or     al, byte ptr [ecx]
        je     loc_2_0A97
        push   6
        mov    eax, ecx
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        call   sub_1_098C
loc_2_0A8B:
        jmp    loc_2_0A97
        db  08Dh, 049h, 000h
loc_2_0A90:
        mov    dword ptr [ebp - 0x18], 0
loc_2_0A97:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0930
loc_2_0AA0:
        push   edi
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   ebx
        push   0x100
        push   0x80
        push   0x3f8
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_2_02B0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_2_0AE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x50
        mov    word ptr [ebp - 0x18], 2
        mov    word ptr [ebp - 0x16], 0
        mov    dword ptr [ebp - 0x14], 0x12013e80
        mov    dword ptr [ebp - 0x10], 0x120126c0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_07BA
loc_2_0B13:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0xc], eax          ; -> obj1:0x778C
        mov    dword ptr [0x18], 0xff        ; -> obj1:0x7798
        mov    byte ptr [0x1c], 0            ; -> obj1:0x779C
        mov    dword ptr [4], 0              ; -> obj1:0x7784
        mov    dword ptr [0], 0              ; -> obj1:0x7780
        mov    byte ptr [ebp - 0x50], 3
        mov    byte ptr [ebp - 0x4f], 0
        mov    dword ptr [ebp - 0x4a], 0     ; -> loc_4_2FB0
        mov    dword ptr [ebp - 0x46], 0     ; -> loc_4_31A0
        mov    dword ptr [ebp - 0x42], 0     ; -> sub_4_4500
        mov    dword ptr [ebp - 0x3e], 0     ; -> loc_4_4780
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_4_3270
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_4_2F10
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_4_2F60
        mov    dword ptr [ebp - 0x2e], 0     ; -> loc_1_0160
        mov    dword ptr [ebp - 0x2a], 0     ; -> loc_2_0870
        mov    dword ptr [ebp - 0x26], 0     ; -> loc_1_0000
        mov    eax, dword ptr [0x188]        ; -> obj1:0x0BC8
        mov    ecx, dword ptr [0x18c]        ; -> obj1:0x0BCC
        mov    dword ptr [ebp - 0x22], eax
        lea    eax, [ebp - 0x50]
        mov    dword ptr [ebp - 0x1e], ecx
        push   0x36
        push   eax
        push   0                             ; -> obj1:0x7780
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   8                             ; -> obj1:0x7788
        push   eax
        call   sub_1_07C6
loc_2_0BBC:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0BCB
        mov    eax, dword ptr [ebp - 8]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0BCB:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_07C0
loc_2_0BD6:
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0BF0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x4ba], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x4be], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x4c2], eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 0x4c6], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0x4ca], eax
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        mov    word ptr [ebp - 0x10], 0x240
        mov    word ptr [ebp - 0xe], 0x280
        mov    word ptr [ebp - 0xc], 0x2c0
        mov    word ptr [ebp - 0xa], 0x300
        sub    ax, ax
        mov    word ptr [ebp - 8], 0x320
        mov    word ptr [ebp - 6], 0x340
        mov    word ptr [ebp - 4], 0x360
        mov    dx, word ptr [ebp + 0xc]
        and    dx, 0xfff0
        sub    dx, 0x10
loc_2_0C70:
        mov    ecx, eax
        and    ecx, 0xffff
        cmp    word ptr [ebp + ecx*2 - 0x10], dx
        je     loc_2_0C90
        inc    ax
        cmp    ax, 7
        jb     loc_2_0C70
        sub    al, al
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_2_0C90:
        in     al, dx
        and    eax, 0xff
        and    eax, 0xfffffffa
        cmp    eax, 0x22
        je     loc_2_0CA6
        sub    al, al
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0CA6:
        add    dx, 3
        in     al, dx
        and    eax, 0xff
        and    al, 0xc0
        cmp    al, 0x40
        jne    loc_2_0CC0
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_2_0CC0:
        sub    al, al
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0CD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x3e], 0xb
        je     loc_2_0CF0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0CF0:
        mov    di, word ptr [esi + 0x12]
        mov    dword ptr [esi + 0x42], 0
        lea    edx, [edi + 3]
        in     al, dx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, 7
loc_2_0D05:
        mov    bl, cl
        xor    bl, al
        mov    al, bl
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, cl
        mov    bl, al
        xor    bl, byte ptr [ebp - 1]
        and    ebx, 0xff
        test   ebx, 0xffffffbf
        jne    loc_2_0D27
        dec    cl
        jne    loc_2_0D05
loc_2_0D27:
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        or     cl, cl
        jne    loc_2_0D49
        add    di, 2
        mov    word ptr [esi + 0x12], di
        mov    dword ptr [esi + 0x3e], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D49:
        lea    edx, [edi + 2]
        in     al, dx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, 0x3f
        and    al, cl
        mov    byte ptr [ebp - 2], al
        out    dx, al
loc_2_0D59:
        mov    bl, cl
        xor    bl, al
        mov    al, bl
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, cl
        cmp    al, byte ptr [ebp - 2]
        jne    loc_2_0D6D
        dec    cl
        jne    loc_2_0D59
loc_2_0D6D:
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        or     cl, cl
        jne    loc_2_0DC6
        inc    dx
        in     al, dx
        mov    cl, al
        sub    ebx, ebx
loc_2_0D7C:
        in     al, dx
        cmp    al, cl
        jne    loc_2_0DA4
        inc    ebx
        cmp    ebx, 0xa
        jl     loc_2_0D7C
        test   al, 6
        jne    loc_2_0DB0
        mov    dword ptr [esi + 0x3e], 5
        mov    dword ptr [esi + 0x42], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0DA4:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_0DB0:
        test   al, 4
        je     loc_2_0DC6
        mov    dword ptr [esi + 0x3e], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0DC6:
        lea    edx, [edi + 1]
        in     al, dx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, 7
loc_2_0DD0:
        mov    bl, cl
        xor    bl, al
        mov    al, bl
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, cl
        mov    bl, al
        xor    bl, byte ptr [ebp - 1]
        mov    edi, ebx
        and    edi, 0xff
        test   edi, 0xffffffbf
        jne    loc_2_0DF4
        dec    cl
        jne    loc_2_0DD0
loc_2_0DF4:
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        or     cl, cl
        je     loc_2_0E07
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0E07:
        mov    cl, byte ptr [ebp - 1]
        mov    al, cl
        sub    edi, edi
        mov    edi, edi
loc_2_0E10:
        xor    cl, 0x40
        xor    al, 0x40
        out    dx, al
        in     al, dx
        in     al, dx
        cmp    al, cl
        jne    loc_2_0E22
        inc    edi
        cmp    edi, 8
        jl     loc_2_0E10
loc_2_0E22:
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        cmp    edi, 8
        jne    loc_2_0E44
        mov    dword ptr [esi + 0x3e], 2
        mov    dword ptr [esi + 0x42], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0E44:
        mov    al, byte ptr [ebp - 1]
        and    al, 0xbf
        out    dx, al
        mov    dword ptr [esi + 0x3e], 3
        mov    dword ptr [esi + 0x42], 2
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0E70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0x10]
        lea    eax, [ecx + 4]
        mov    word ptr [ebp - 0xc], ax
        mov    dx, ax
        in     al, dx
        mov    bl, al
        lea    edx, [ecx + 5]
        mov    word ptr [ebp - 8], dx
        in     al, dx
        mov    si, ax
        and    esi, 0xffff00ff
        shl    si, 8
        mov    al, bl
        and    eax, 0xffff00ff
        or     si, ax
        test   si, 1
        je     loc_2_0EE0
        mov    eax, esi
        and    eax, 0xffff
        push   eax
        push   0xc0000001
        push   1
        push   3
        push   0xaa000004
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0EE0:
        lea    edx, [ecx + 2]
        in     al, dx
        lea    edx, [ecx + 3]
        in     al, dx
        sub    edi, edi
loc_2_0EEA:
        mov    dx, word ptr [ebp - 0xc]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [ebp - 8]
        in     al, dx
        mov    bl, al
        and    ebx, 0xffff00ff
        shl    bx, 8
        mov    al, byte ptr [ebp - 2]
        and    eax, 0xffff00ff
        or     bx, ax
        mov    eax, esi
        mov    edx, ebx
        and    edx, 0xffff
        and    eax, 0xffff
        sub    edx, eax
        cmp    edx, 2
        jne    loc_2_0F70
        lea    edx, [ecx + 0xa]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [ecx + 0xb]
        in     al, dx
        mov    si, ax
        and    esi, 0xffff00ff
        shl    si, 8
        mov    al, byte ptr [ebp - 1]
        and    eax, 0xffff00ff
        or     si, ax
        cmp    bx, si
        jne    loc_2_0FA0
        mov    dx, cx
        in     al, dx
        lea    edx, [ecx + 1]
        in     al, dx
        lea    edx, [ecx + 2]
        in     al, dx
        lea    edx, [ecx + 3]
        in     al, dx
        inc    edi
        cmp    edi, 8
        jl     loc_2_0EEA
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0F70:
        mov    eax, ebx
        and    eax, 0xffff
        push   eax
        push   0xc0000002
        push   1
        push   3
        push   0xaa000004
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0FA0:
        push   0
        push   0xc0000012
        push   1
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   0xaa000004
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0FD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0xb
        mov    dword ptr [esi + 0x3e], ebx
        mov    ax, word ptr [esi + 0xa]
        mov    word ptr [ebp - 4], ax
        cmp    ax, 0x20
        jne    loc_2_103E
        mov    dx, word ptr [esi + 0x12]
        inc    dx
        sub    di, di
        mov    ecx, 4
loc_2_1000:
        in     al, dx
        and    ax, 0xff
        shr    al, 4
        and    ax, 0xff
        shl    di, 4
        or     ax, di
        mov    di, ax
        dec    ecx
        jne    loc_2_1000
        sub    eax, eax
        add    eax, 0
loc_2_1020:
        cmp    di, 0x9436
        je     loc_2_1094
        mov    cx, di
        shr    cx, 0xc
        shl    di, 4
        or     cx, di
        mov    di, cx
        inc    eax
        cmp    eax, 4
        jl     loc_2_1020
loc_2_103E:
        cmp    word ptr [ebp - 4], 0x20
        je     loc_2_10D3
        mov    dx, word ptr [esi + 0x12]
        add    dx, 2
        sub    cx, cx
        mov    edi, 4
loc_2_1059:
        in     al, dx
        and    ax, 0xff
        shr    al, 4
        and    ax, 0xff
        shl    cx, 4
        or     ax, cx
        mov    cx, ax
        dec    edi
        jne    loc_2_1059
        sub    eax, eax
loc_2_1074:
        cmp    cx, 0x9246
        je     loc_2_10C3
        mov    di, cx
        shr    di, 0xc
        shl    cx, 4
        or     di, cx
        mov    cx, di
        inc    eax
        cmp    eax, 4
        jl     loc_2_1074
        jmp    loc_2_10D3
loc_2_1094:
        mov    ebx, 6
        mov    cx, word ptr [esi + 0x12]
        mov    dword ptr [esi + 0x42], 0
        lea    edx, [ecx + 1]
        in     al, dx
        and    al, 0xf7
        out    dx, al
        mov    dx, cx
        mov    al, 0xfe
        out    dx, al
        lea    edx, [ecx + 2]
        in     al, dx
        or     al, 0x40
        out    dx, al
        mov    ecx, 0x14
loc_2_10BD:
        in     al, dx
        dec    ecx
        jne    loc_2_10BD
        jmp    loc_2_10D3
loc_2_10C3:
        mov    word ptr [esi + 0x12], dx
        mov    dword ptr [esi + 0x42], 0
        mov    ebx, 4
loc_2_10D3:
        push   esi
        call   sub_2_0E70
        or     al, al
        je     loc_2_116B
        cmp    ebx, 0xb
        je     loc_2_1111
        mov    dword ptr [esi + 0x3e], ebx
loc_2_10E9:
        mov    eax, dword ptr [esi + 0x3e]
        cmp    eax, 5
        je     loc_2_113C
        push   eax
        push   0xc0000011
        push   1
        push   3
        push   0xc100000d
        push   esi
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1111:
        push   esi
        call   sub_2_0CD0
        or     al, al
        jne    loc_2_10E9
        push   0
        push   0xc0000005
        push   2
        push   2
        push   0xaa000003
        push   esi
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_113C:
        or     eax, eax
        jne    loc_2_1160
        push   eax
        push   0xc0000011
        push   1
        push   3
        push   0xc100000d
        push   esi
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1160:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_116B:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1180:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        and    dword ptr [esi + 0xb7], 0xfffffffe
        call   sub_2_0CD0
        or     al, al
        je     loc_2_1215
        mov    byte ptr [esi + 0x1e], 0
        push   esi
        call   sub_2_2080
        or     al, al
        je     loc_2_11B4
        or     dword ptr [esi + 0xb7], 1
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_2_11B4:
        mov    eax, dword ptr [esi + 0x3e]
        cmp    eax, 5
        je     loc_2_11DB
        push   eax
        push   0xc0000011
        push   1
        push   3
        push   0xc100000d
        push   esi
        call   sub_2_0BF0
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    4
loc_2_11DB:
        or     eax, eax
        push   eax
        push   0xc0000011
        push   1
        push   3
        jne    loc_2_1200
        push   0xc100000d
        push   esi
        call   sub_2_0BF0
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_1200:
        push   0xc100000e
        push   esi
        call   sub_2_0BF0
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    4
loc_2_1215:
        push   0
        push   0xc0000005
        push   2
        push   2
        push   0xaa000003
        push   esi
        call   sub_2_0BF0
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1240:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    si, si
        sub    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
loc_2_1251:
        push   0x2710
        call   sub_1_08A0
loc_2_125B:
        mov    cx, si
        mov    si, word ptr [edi + 0x10]
        lea    edx, [esi + 6]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [esi + 7]
        in     al, dx
        mov    si, ax
        and    esi, 0xffff00ff
        shl    si, 8
        mov    al, byte ptr [ebp - 1]
        and    eax, 0xffff00ff
        or     si, ax
        mov    word ptr [edi + 0xb1], si
        cmp    cx, si
        je     loc_2_1298
        test   si, 0x40
        jne    loc_2_12E0
loc_2_1298:
        add    ebx, 0x2710
        cmp    ebx, 0x5b8d80
        jle    loc_2_1251
        test   si, 0x30
        je     loc_2_12C9
        mov    eax, esi
        and    eax, 0xffff
        push   eax
        push   0xc0000017
        push   4
        push   3
        push   0xc1000001
        push   edi
        call   sub_2_0BF0
loc_2_12C9:
        and    dword ptr [edi + 0xb7], 0xfffffffc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_12E0:
        or     dword ptr [edi + 0xb7], 2
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1300:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 0xc000
        cmp    dword ptr [edx + 0x3e], 9
        jne    loc_2_1322
        cmp    byte ptr [edx + 0x18], 0
        je     loc_2_1326
        mov    ax, 0xc800
        jmp    loc_2_1326
loc_2_1322:
        mov    ax, 0xd000
loc_2_1326:
        mov    edi, dword ptr [ebp + 0x10]
        mov    dx, word ptr [edi + 4]
        mov    ch, dl
        mov    cl, dh
        or     cx, cx
        je     loc_2_1443
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
loc_2_1342:
        mov    ax, word ptr [edi]
        mov    dx, ax
        and    edx, 0xff
        shl    edx, 8
        mov    dword ptr [ebp - 0xc], edx
        mov    dx, ax
        shr    dx, 8
        mov    esi, edx
        and    esi, 0xffff
        or     esi, dword ptr [ebp - 0xc]
        and    esi, dword ptr [ebp - 0x10]
        cmp    esi, 1
        sbb    bl, bl
        neg    bl
        or     bl, bl
        je     loc_2_13A0
        shl    al, 8
        or     al, dl
        mov    edx, dword ptr [ebp + 8]
        mov    si, word ptr [edx + 0x10]
        lea    edx, [esi + 0xc]
        out    dx, al
        lea    edx, [esi + 0xd]
        mov    al, byte ptr [ebp - 0xb]
        out    dx, al
        mov    ax, word ptr [edi + 2]
        shl    al, 8
        or     al, byte ptr [edi + 3]
        lea    edx, [esi + 4]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [edi + 2]
        out    dx, al
loc_2_13A0:
        test   cl, 1
        je     loc_2_13F0
        shr    cx, 1
        mov    dx, word ptr [edi + 6]
        mov    ah, dl
        mov    al, dh
        mov    word ptr [ebp - 2], ax
        or     bl, bl
        je     loc_2_13E8
        mov    ax, cx
        dec    cx
        or     ax, ax
        je     loc_2_13E8
        mov    edx, dword ptr [ebp + 8]
        mov    si, word ptr [edx + 0x10]
        lea    ebx, [esi + 2]
        add    si, 3
loc_2_13D0:
        mov    dx, bx
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    ax, cx
        dec    cx
        or     ax, ax
        jne    loc_2_13D0
loc_2_13E8:
        add    edi, 8
        jmp    loc_2_1432
        db  08Dh, 049h, 000h
loc_2_13F0:
        lea    esi, [edi + 6]
        mov    eax, ecx
        and    eax, 0xfffe
        lea    edi, [esi + eax]
        cmp    edi, esi
        jbe    loc_2_1432
loc_2_1401:
        or     bl, bl
        je     loc_2_142B
        mov    cx, word ptr [esi]
        mov    ax, cx
        shl    al, 8
        mov    edx, dword ptr [ebp + 8]
        or     al, ch
        mov    dx, word ptr [edx + 0x10]
        mov    word ptr [ebp - 8], dx
        add    dx, 2
        out    dx, al
        mov    dx, word ptr [ebp - 8]
        add    dx, 3
        mov    al, cl
        out    dx, al
loc_2_142B:
        add    esi, 2
        cmp    edi, esi
        ja     loc_2_1401
loc_2_1432:
        mov    ax, word ptr [edi + 4]
        mov    ch, al
        mov    cl, ah
        or     cx, cx
        jne    loc_2_1342
loc_2_1443:
        mov    edx, dword ptr [ebp + 8]
        mov    al, 1
        mov    cx, word ptr [edx + 0x10]
        pop    edi
        lea    edx, [ecx + 0xc]
        out    dx, al
        lea    edx, [ecx + 0xd]
        sub    al, al
        out    dx, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
sub_2_1460:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 0x80
        mov    dx, word ptr [esi + 0x10]
        add    dx, 8
        out    dx, al
        push   0x2710
        call   sub_1_08A0
loc_2_147C:
        mov    cx, word ptr [esi + 0x10]
        add    cx, 8
        mov    dx, cx
        in     al, dx
        and    al, 0x64
        or     al, 1
        cmp    dword ptr [esi + 0x35], 0x9c40
        jne    loc_2_1497
        or     al, 2
loc_2_1497:
        mov    dx, cx
        out    dx, al
        push   0x2710
        call   sub_1_08A0
loc_2_14A5:
        mov    cx, word ptr [esi + 0x10]
        add    cx, 9
        mov    dx, cx
        sub    al, al
        out    dx, al
        push   0x2710
        call   sub_1_08A0
loc_2_14BD:
        push   2                             ; -> loc_1_0EDE
        push   0xa00
        push   esi
        call   sub_2_1300
        mov    cx, word ptr [esi + 0x10]
        add    cx, 8
        mov    dx, cx
        in     al, dx
        and    al, 0xbf
        or     al, 8
        out    dx, al
        push   esi
        call   sub_2_1240
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_14F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi], 0xc0000001
        push   edi
        call   sub_2_1180
        or     eax, eax
        jne    loc_2_15CC
        push   edi
        call   sub_2_1460
        or     eax, eax
        jne    loc_2_15B6
        push   edi
        call   sub_1_0190
loc_2_1523:
        cmp    dword ptr [edi + 0x42], 0
        lea    ecx, [edi + 0x52d]
        jne    loc_2_1537
        mov    dword ptr [ecx], 1
        jmp    loc_2_153D
loc_2_1537:
        mov    dword ptr [ecx], 2
loc_2_153D:
        mov    eax, dword ptr [edi + 0x3e]
        cmp    eax, 7
        je     loc_2_154A
        cmp    eax, 9
        jne    loc_2_1556
loc_2_154A:
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [ecx], 2
loc_2_1556:
        cmp    dword ptr [edi + 0x35], 0x27100
        mov    eax, 0x1fd0
        je     loc_2_156D
        mov    eax, 0x1178
        mov    byte ptr [edi + 0x39], 0
loc_2_156D:
        mov    ecx, dword ptr [edi + 0x3a]
        or     ecx, ecx
        je     loc_2_1578
        cmp    eax, ecx
        jae    loc_2_157B
loc_2_1578:
        mov    dword ptr [edi + 0x3a], eax
loc_2_157B:
        cmp    dword ptr [edi + 0x42], 0
        jne    loc_2_158F
        mov    eax, dword ptr [edi + 0xc3]
        cmp    dword ptr [edi + 0x3a], eax
        jbe    loc_2_158F
        mov    dword ptr [edi + 0x3a], eax
loc_2_158F:
        cmp    word ptr [edi + 0x54], 0
        jne    loc_2_15AA
        cmp    dword ptr [edi + 0x50], 0
        jne    loc_2_15AA
        mov    ax, word ptr [edi + 0x4e]
        mov    word ptr [edi + 0x54], ax
        mov    eax, dword ptr [edi + 0x4a]
        mov    dword ptr [edi + 0x50], eax
loc_2_15AA:
        mov    dword ptr [esi], 0
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_2_15B6:
        push   0
        push   0xc0000007
        push   1
        push   2
        push   0xc1000009
        push   edi
        call   sub_2_0BF0
loc_2_15CC:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_15E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x70
        mov    word ptr [ebp - 0x70], 0xa20
        mov    word ptr [ebp - 0x6e], 0xa30
        mov    word ptr [ebp - 0x6c], 0xa18
        mov    word ptr [ebp - 0x6a], 0xa18
        mov    word ptr [ebp - 0x68], 0xc
        mov    word ptr [ebp - 0x66], 0xa30
        mov    word ptr [ebp - 0x64], 0x10
        mov    word ptr [ebp - 0x62], 0xa24
        mov    word ptr [ebp - 0x60], 0xa40
        mov    word ptr [ebp - 0x5e], 0xa28
        mov    word ptr [ebp - 0x5c], 0xa24
        mov    word ptr [ebp - 0x5a], 0xc
        mov    word ptr [ebp - 0x58], 0xa40
        mov    word ptr [ebp - 0x56], 0x10
        mov    word ptr [ebp - 0x54], 0xa50
        mov    word ptr [ebp - 0x52], 0xa60
        mov    word ptr [ebp - 0x50], 0xa58
        mov    word ptr [ebp - 0x4e], 0xa50
        mov    word ptr [ebp - 0x4c], 4
        mov    word ptr [ebp - 0x4a], 0xa58
        mov    word ptr [ebp - 0x48], 0x18
        mov    word ptr [ebp - 0x46], 0xa54
        mov    word ptr [ebp - 0x44], 0xa70
        mov    word ptr [ebp - 0x42], 0xa80
        mov    word ptr [ebp - 0x40], 0xa54
        mov    word ptr [ebp - 0x3e], 4
        mov    word ptr [ebp - 0x3c], 0xa70
        mov    word ptr [ebp - 0x3a], 0x18
        mov    word ptr [ebp - 0x38], 0x1a20
        mov    word ptr [ebp - 0x36], 0x1a30
        mov    word ptr [ebp - 0x34], 0x1a18
        mov    word ptr [ebp - 0x32], 0x1a18
        mov    word ptr [ebp - 0x30], 0xc
        mov    word ptr [ebp - 0x2e], 0x1a30
        mov    word ptr [ebp - 0x2c], 0x10
        mov    word ptr [ebp - 0x2a], 0x1a24
        mov    word ptr [ebp - 0x28], 0x1a40
        mov    word ptr [ebp - 0x26], 0x1a28
        mov    word ptr [ebp - 0x24], 0x1a24
        mov    word ptr [ebp - 0x22], 0xc
        mov    word ptr [ebp - 0x20], 0x1a40
        mov    word ptr [ebp - 0x1e], 0x10
        mov    word ptr [ebp - 0x1c], 0x1a50
        mov    word ptr [ebp - 0x1a], 0x1a60
        mov    word ptr [ebp - 0x18], 0x1a58
        mov    word ptr [ebp - 0x16], 0x1a50
        mov    word ptr [ebp - 0x14], 4
        mov    word ptr [ebp - 0x12], 0x1a58
        mov    word ptr [ebp - 0x10], 0x18
        mov    word ptr [ebp - 0xe], 0x1a54
        mov    word ptr [ebp - 0xc], 0x1a70
        mov    word ptr [ebp - 0xa], 0x1a80
        mov    word ptr [ebp - 8], 0x1a54
        mov    word ptr [ebp - 6], 4
        mov    word ptr [ebp - 4], 0x1a70
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebp - 2], 0x18
        cmp    dword ptr [ecx + 0x3e], 0xb
        jne    loc_2_17F7
        mov    edx, dword ptr [ecx + 0x4f7]
        mov    eax, edx
        shl    edx, 3
        sub    edx, eax
        mov    ax, word ptr [ebp + edx*2 - 0x6e]
        mov    word ptr [ecx + 0x10], ax
        mov    eax, dword ptr [ecx + 0x4f7]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 0x70]
        mov    word ptr [ecx + 0x12], ax
        mov    eax, dword ptr [ecx + 0x4f7]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 0x6c]
        mov    word ptr [ecx + 0x14], ax
        mov    eax, dword ptr [ecx + 0x4f7]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 0x6c]
        add    ax, 4
        mov    word ptr [ecx + 0x16], ax
        mov    edx, dword ptr [ecx + 0x4f7]
        mov    eax, edx
        shl    edx, 3
        sub    edx, eax
        mov    ax, word ptr [ebp + edx*2 - 0x6a]
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [ecx + 0x4f7]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 0x66]
        mov    word ptr [ecx + 0xc], ax
        mov    eax, dword ptr [ecx + 0x4f7]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 0x68]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [ecx + 0x4f7]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 0x64]
        mov    word ptr [ecx + 0xe], ax
loc_2_17F7:
        mov    byte ptr [ecx + 0x29], 0
        mov    dword ptr [ecx + 0xce], 0x400
        mov    dword ptr [ecx + 0xd2], 0x20000
        mov    dword ptr [ecx + 0x4e7], 0xa
        mov    dword ptr [ecx + 0x4eb], 0xa
        mov    dword ptr [ecx + 0x4ef], 0x4b0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1840:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, 1
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, 0x1f
        ja     loc_2_1880
        mov    cl, bl
        mov    esi, dword ptr [ebp + 0x10]
        shl    eax, cl
        test   dword ptr [esi + 0x10], eax
        je     loc_2_1898
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
loc_2_1880:
        lea    ecx, [ebx - 0x20]
        mov    esi, dword ptr [ebp + 0x10]
        shl    eax, cl
        test   dword ptr [esi + 0x14], eax
        je     loc_2_1898
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_1898:
        cmp    ebx, 0x1b
        jbe    loc_2_18C0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x4fb]
        push   ecx
        push   eax
        call   sub_2_0C30
        or     al, al
        je     loc_2_18C0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
loc_2_18C0:
        cmp    ebx, 0x1f
        ja     loc_2_18FA
        lea    edx, [ebp - 8]
loc_2_18C8:
        mov    eax, dword ptr [edx]
        cmp    ebx, eax
        jne    loc_2_18DD
        mov    edi, 1
        mov    cl, byte ptr [edx + 4]
        shl    edi, cl
        test   dword ptr [esi + 0x10], edi
        jne    loc_2_1905
loc_2_18DD:
        cmp    dword ptr [edx + 4], ebx
        jne    loc_2_18F0
        mov    edi, 1
        mov    cl, al
        shl    edi, cl
        test   dword ptr [esi + 0x10], edi
        jne    loc_2_1910
loc_2_18F0:
        add    edx, 8
        lea    eax, [ebp]
        cmp    edx, eax
        jb     loc_2_18C8
loc_2_18FA:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_1905:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_1910:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_2_1920:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [edi], 0
        mov    ecx, dword ptr [esi + 0x4f7]
        cmp    ecx, 7
        ja     loc_2_19A0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        push   ecx
        call   sub_2_1840
        or     al, al
        jne    loc_2_1963
        push   esi
        call   sub_2_15E0
        push   esi
        call   sub_2_0FD0
        or     al, al
        je     loc_2_19A0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
loc_2_1963:
        mov    dword ptr [edi], 0xc001001e
        mov    dword ptr [esi + 0x4ba], 0xaa00000d
        mov    dword ptr [esi + 0x4be], 5
        mov    eax, dword ptr [esi + 0x4f7]
        mov    dword ptr [esi + 0x4c6], eax
        sub    al, al
        mov    dword ptr [esi + 0x4c2], 2
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
loc_2_19A0:
        mov    dword ptr [edi], 0xc0010006
        mov    dword ptr [esi + 0x4ba], 0xaa000004
        mov    dword ptr [esi + 0x4be], 5
        mov    eax, dword ptr [esi + 0x4f7]
        mov    dword ptr [esi + 0x4c6], eax
        sub    al, al
        mov    dword ptr [esi + 0x4c2], 2
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_19E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x54
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x85], 0
        lea    ebx, [esi + 0x95]
        mov    dword ptr [esi + 0x91], 0
        mov    dword ptr [ebx], 0
        cmp    dword ptr [esi + 0x3e], 9
        jne    loc_2_1A20
        mov    dword ptr [esi + 0x91], 0x4e
        jmp    loc_2_1A27
        db  08Bh, 0FFh
loc_2_1A20:
        add    dword ptr [esi + 0x85], 0x4e
loc_2_1A27:
        mov    eax, dword ptr [0]            ; -> obj1:0x0AC4
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x85]
        push   eax
        lea    eax, [esi + 0x89]
        push   eax
        call   sub_1_08A6
loc_2_1A42:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_2_1A54
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1A54:
        mov    eax, dword ptr [esi + 0x89]
        mov    dword ptr [esi + 0x8d], eax
        mov    edx, dword ptr [esi + 0x85]
        mov    edi, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [0]            ; -> obj1:0x0AC4
        push   eax
        push   0
        push   0x2000
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_08A6
loc_2_1A8E:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_2_1AB0
        push   0
        push   0
        push   esi
        call   sub_4_1BE0
loc_2_1A9F:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1AB0:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x2a9], eax
        cmp    dword ptr [esi + 0x3e], 9
        mov    eax, dword ptr [esi + 0x89]
        jne    loc_2_1B10
        mov    dword ptr [esi + 0x99], eax
        mov    eax, dword ptr [esi + 0x8d]
        mov    dword ptr [esi + 0x9d], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0AC4
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x91]
        push   eax
        push   ebx
        call   sub_1_08A6
loc_2_1AEC:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_2_1B04
        mov    dword ptr [ebx], 0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1B04:
        mov    edx, dword ptr [ebx]
        mov    ecx, edx
        jmp    loc_2_1B34
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1B10:
        add    eax, 0x4e
        mov    dword ptr [esi + 0x99], eax
        mov    eax, dword ptr [esi + 0x8d]
        add    eax, 0x4e
        mov    dword ptr [esi + 0x9d], eax
        mov    ecx, dword ptr [esi + 0x89]
        mov    edx, dword ptr [esi + 0x8d]
loc_2_1B34:
        mov    dword ptr [esi + 0x65], ecx
        lea    eax, [ecx + 6]
        mov    dword ptr [esi + 0x69], eax
        lea    eax, [ecx + 0xe]
        mov    dword ptr [esi + 0x6d], eax
        add    ecx, 0x2e
        mov    dword ptr [esi + 0x71], ecx
        mov    dword ptr [esi + 0x75], edx
        lea    eax, [edx + 6]
        mov    dword ptr [esi + 0x79], eax
        lea    eax, [edx + 0xe]
        mov    dword ptr [esi + 0x7d], eax
        lea    eax, [edx + 0x2e]
        mov    dword ptr [esi + 0x81], eax
        add    edx, 0x3c
        mov    eax, edx
        shr    eax, 0x10
        mov    cx, dx
        and    ecx, 0xff
        shl    ecx, 0x10
        mov    cl, al
        shl    ecx, 8
        shr    dx, 8
        and    edx, 0xffff
        shl    edx, 0x10
        or     ecx, edx
        shr    ax, 8
        and    eax, 0xffff
        or     ecx, eax
        mov    eax, dword ptr [esi + 0x6d]
        push   0
        mov    dword ptr [eax + 0x1c], ecx
        push   esi
        call   sub_4_4DE0
loc_2_1BA2:
        lea    edi, [esi + 0x629]
        push   0
        push   1
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_08B2
loc_2_1BB6:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_1BD1
        push   1
        push   1
        push   esi
        call   sub_4_1BE0
loc_2_1BC6:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1BD1:
        mov    dword ptr [esi + 0x645], 0
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 0x62d]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_08BE
loc_2_1BEE:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_1C10
        push   1
        push   1
        push   esi
        call   sub_4_1BE0
loc_2_1BFE:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1C10:
        mov    dword ptr [esi + 0x645], 1
        mov    eax, dword ptr [0]            ; -> obj1:0x0AC4
        push   eax
        push   0
        lea    eax, [esi + 0x639]
        push   0x4b
        push   eax
        call   sub_1_08A6
loc_2_1C30:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_2_1C50
        push   1
        push   1
        push   esi
        call   sub_4_1BE0
loc_2_1C41:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_1C50:
        mov    dword ptr [esi + 0x645], 2
        lea    ebx, [esi + 0x631]
        push   1
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_08D6
loc_2_1C6C:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_1C90
        push   1
        push   1
        push   esi
        call   sub_4_1BE0
loc_2_1C7C:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1C90:
        mov    dword ptr [esi + 0x645], 3
        push   0x4b
        mov    eax, dword ptr [esi + 0x639]
        push   eax
        mov    eax, dword ptr [ebx]
        lea    edi, [esi + 0x635]
        push   eax
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_08E2
loc_2_1CB6:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_1CD1
        push   1
        push   1
        push   esi
        call   sub_4_1BE0
loc_2_1CC6:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1CD1:
        mov    dword ptr [esi + 0x645], 4
        mov    byte ptr [ebp - 0x54], 0x10
        mov    byte ptr [ebp - 0x53], 0x40
        mov    byte ptr [ebp - 0x52], 0xc0
        mov    byte ptr [ebp - 0x51], 0
        mov    byte ptr [ebp - 0x50], 0
        mov    byte ptr [ebp - 0x4f], 0
        mov    byte ptr [ebp - 0x4e], 0x20
        mov    byte ptr [ebp - 0x4d], 0
        mov    byte ptr [ebp - 0x4c], 0x80
        mov    byte ptr [ebp - 0x4b], 0
        mov    byte ptr [ebp - 0x4a], 0
        mov    byte ptr [ebp - 0x49], 0
        mov    byte ptr [ebp - 0x48], 0
        mov    byte ptr [ebp - 0x47], 0
        mov    byte ptr [ebp - 0x46], 0xc2
        mov    byte ptr [ebp - 0x45], 0x70
        mov    byte ptr [ebp - 0x44], 0xf4
        mov    byte ptr [ebp - 0x43], 0xf4
        mov    byte ptr [ebp - 0x42], 3
        mov    byte ptr [ebp - 0x41], 0
        mov    byte ptr [ebp - 0x40], 0x38
        mov    byte ptr [ebp - 0x3f], 0x84
        mov    byte ptr [ebp - 0x3e], 1
        mov    byte ptr [ebp - 0x3d], 0
        mov    byte ptr [ebp - 0x3c], 0xa
        mov    byte ptr [ebp - 0x3b], 0x40
        mov    byte ptr [ebp - 0x3a], 3
        mov    byte ptr [ebp - 0x39], 0
        mov    byte ptr [ebp - 0x38], 0
        mov    byte ptr [ebp - 0x37], 0
        mov    byte ptr [ebp - 0x36], 0
        mov    byte ptr [ebp - 0x35], 0
        mov    byte ptr [ebp - 0x34], 0
        mov    byte ptr [ebp - 0x33], 0
        mov    byte ptr [ebp - 0x32], 6
        mov    byte ptr [ebp - 0x31], 0xc0
        mov    byte ptr [ebp - 0x30], 1
        mov    byte ptr [ebp - 0x2f], 0
        mov    byte ptr [ebp - 0x2e], 1
        mov    byte ptr [ebp - 0x2d], 0
        mov    byte ptr [ebp - 0x2c], 6
        mov    byte ptr [ebp - 0x2b], 0xc0
        mov    byte ptr [ebp - 0x2a], 2
        mov    byte ptr [ebp - 0x29], 0x80
        mov    byte ptr [ebp - 0x28], 0
        mov    byte ptr [ebp - 0x27], 0
        mov    byte ptr [ebp - 0x26], 0xc
        mov    byte ptr [ebp - 0x25], 0xc0
        mov    byte ptr [ebp - 0x24], 0xb
        mov    byte ptr [ebp - 0x23], 0
        mov    byte ptr [ebp - 0x22], 0
        mov    byte ptr [ebp - 0x21], 0
        mov    byte ptr [ebp - 0x20], 0
        mov    byte ptr [ebp - 0x1f], 0
        mov    byte ptr [ebp - 0x1e], 0
        mov    byte ptr [ebp - 0x1d], 0
        mov    byte ptr [ebp - 0x1c], 0
        mov    byte ptr [ebp - 0x1b], 0
        mov    byte ptr [ebp - 0x1a], 0x12
        mov    byte ptr [ebp - 0x19], 0x40
        mov    byte ptr [ebp - 0x18], 0xc
        mov    byte ptr [ebp - 0x17], 0
        mov    byte ptr [ebp - 0x16], 0x32
        mov    byte ptr [ebp - 0x15], 0x30
        mov    byte ptr [ebp - 0x14], 0x32
        mov    byte ptr [ebp - 0x13], 0x30
        mov    byte ptr [ebp - 0x12], 0x32
        mov    byte ptr [ebp - 0x11], 0x30
        mov    byte ptr [ebp - 0x10], 0x39
        sub    eax, eax
        mov    byte ptr [ebp - 0xf], 0x36
        mov    byte ptr [ebp - 0xe], 0x58
        mov    byte ptr [ebp - 0xd], 0x35
        mov    byte ptr [ebp - 0xc], 0x37
        mov    byte ptr [ebp - 0xb], 0x36
        mov    byte ptr [ebp - 0xa], 0
        lea    esp, [esp]
loc_2_1E10:
        mov    cl, byte ptr [ebp + eax - 0x54]
        mov    edx, dword ptr [esi + 0x639]
        mov    byte ptr [eax + edx], cl
        inc    eax
        cmp    eax, 0x4b
        jb     loc_2_1E10
        mov    ecx, dword ptr [esi + 0x639]
        mov    ax, word ptr [esi + 0x54]
        mov    word ptr [ecx + 0x35], ax
        mov    ecx, dword ptr [esi + 0x639]
        mov    eax, dword ptr [esi + 0x50]
        mov    dword ptr [ecx + 0x31], eax
        mov    ax, word ptr [esi + 0x54]
        mov    ecx, dword ptr [esi + 0x639]
        lea    ebx, [esi + 0x67f]
        mov    word ptr [ecx + 0xc], ax
        mov    ecx, dword ptr [esi + 0x639]
        mov    eax, dword ptr [esi + 0x50]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esi + 0x639]
        or     byte ptr [eax + 8], 0x80
        mov    eax, dword ptr [0]            ; -> obj1:0x0AC4
        push   eax
        push   0
        push   0x20
        push   ebx
        call   sub_1_08A6
loc_2_1E77:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_2_1E93
        push   1
        push   1
        push   esi
        call   sub_4_1BE0
loc_2_1E88:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1E93:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 0x18], 0x8002
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 0x14], 1
        mov    dword ptr [esi + 0x645], 5
        mov    eax, dword ptr [edi]
        push   eax
        mov    eax, dword ptr [esi + 0x62d]
        push   eax
        call   sub_1_0900
loc_2_1EC3:
        mov    dword ptr [esi + 0x645], 6
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1EE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    si, word ptr [eax + 0x12]
        add    si, 2
        mov    dx, si
        in     al, dx
        and    al, 0xcf
        or     al, byte ptr [ebp + 0xc]
        mov    bl, al
        out    dx, al
        push   0x1388
        call   sub_1_08A0
loc_2_1F06:
        or     bl, 0x20
        mov    dx, si
        mov    al, bl
        out    dx, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x12]
        add    ax, 2
        mov    word ptr [ebp - 2], ax
        mov    al, byte ptr [ebp + 0xc]
        and    ax, 0x3f
        or     ax, 0x180
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        and    al, 0xf
        or     al, 0x60
        out    dx, al
        push   0
        push   esi
        call   sub_2_1EE0
        mov    bx, 0x100
        mov    edi, 9
loc_2_1F62:
        mov    ax, bx
        and    ax, word ptr [ebp - 4]
        cmp    ax, 1
        cmc    
        sbb    al, al
        and    al, 0x10
        push   eax
        push   esi
        call   sub_2_1EE0
        shr    bx, 1
        dec    edi
        jne    loc_2_1F62
        push   0x1388
        call   sub_1_08A0
loc_2_1F89:
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        sub    di, di
        mov    ebx, 0xf
        mov    ax, dx
        inc    ax
        mov    word ptr [ebp - 8], ax
        nop    
loc_2_1FA0:
        push   0x1388
        call   sub_1_08A0
loc_2_1FAA:
        push   0
        push   esi
        call   sub_2_1EE0
        mov    dx, word ptr [ebp - 8]
        in     al, dx
        and    ax, 0xff
        and    ax, 0x80
        cmp    ax, 1
        mov    cl, bl
        sbb    eax, eax
        inc    eax
        shl    ax, cl
        or     di, ax
        dec    ebx
        jns    loc_2_1FA0
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        and    al, 0x2f
        out    dx, al
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1FF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 2], 0
        lea    edi, [esi + 0x4a]
        sub    ebx, ebx
loc_2_2007:
        lea    eax, [ebx + 0x3c]
        push   eax
        push   esi
        call   sub_2_1F20
        mov    word ptr [edi], ax
        add    edi, 2
        mov    ch, al
        mov    cl, ah
        add    word ptr [ebp - 2], cx
        inc    ebx
        cmp    ebx, 3
        jb     loc_2_2007
        push   0x3f
        push   esi
        call   sub_2_1F20
        cmp    word ptr [esi + 0x54], 0
        jne    loc_2_2048
        cmp    dword ptr [esi + 0x50], 0
        jne    loc_2_2048
        mov    cx, word ptr [esi + 0x4e]
        mov    word ptr [esi + 0x54], cx
        mov    ecx, dword ptr [esi + 0x4a]
        mov    dword ptr [esi + 0x50], ecx
loc_2_2048:
        cmp    word ptr [esi + 0x4e], 0
        jne    loc_2_2055
        cmp    dword ptr [esi + 0x4a], 0
        je     loc_2_206A
loc_2_2055:
        mov    ch, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        cmp    cx, ax
        jne    loc_2_206A
        test   byte ptr [esi + 0x4a], 0x80
        jne    loc_2_206A
        mov    byte ptr [esi + 0x56], 1
loc_2_206A:
        mov    word ptr [esi + 0x57], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2080:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0x12]
        mov    byte ptr [esi + 0x56], 0
        cmp    dword ptr [esi + 0x3e], 5
        je     loc_2_20A0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_20A0:
        lea    edx, [edi + 1]
        in     al, dx
        mov    cl, al
        and    ecx, 7
        mov    ecx, dword ptr [ecx*4 + 0x24] ; -> obj1:0x0C6C
        mov    dword ptr [esi + 0x20], ecx
        mov    cl, al
        and    cl, 0x38
        shr    cl, 1
        and    ecx, 0xff
        lea    edx, [edi + 3]
        mov    ecx, dword ptr [ecx + 0x44]   ; -> obj1:0x0C8C
        mov    dword ptr [esi + 0x1a], ecx
        in     al, dx
        and    al, 1
        cmp    al, 1
        mov    byte ptr [esi + 0x18], 0
        sbb    eax, eax
        and    eax, 0xfffe2b40
        add    eax, 0x27100
        mov    dword ptr [esi + 0x35], eax
        push   esi
        call   sub_2_1FF0
        mov    al, byte ptr [esi + 0x56]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2100:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [eax + 0x53d]
        mov    dword ptr [eax + 0x541], ecx
        mov    dword ptr [ecx], ecx
        mov    dword ptr [eax + 0x539], 0
        mov    dword ptr [eax + 0x54d], 0
        mov    dword ptr [eax + 0x549], 0
        mov    dword ptr [eax + 0x561], 8
        mov    byte ptr [eax + 0x55f], 0
        mov    dword ptr [eax + 0x589], 9
        mov    byte ptr [eax + 0x587], 0
        mov    dword ptr [eax + 0x5b1], 0xb
        mov    byte ptr [eax + 0x5af], 0
        mov    dword ptr [eax + 0x601], 4
        mov    byte ptr [eax + 0x5ff], 0
        mov    dword ptr [eax + 0x5d9], 0xa
        mov    byte ptr [eax + 0x5d7], 0
        mov    dword ptr [eax + 0x667], 0xb
        mov    byte ptr [eax + 0x665], 0
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_21A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [0x777c]       ; -> obj1:0x777C
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   0x32
        push   eax
        call   sub_1_08A6
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_21D0
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  090h
loc_2_21D0:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax + 0x2e], 0xffffffff
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 0x18], al
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [ecx + 0x19], al
        add    ecx, 0x18
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [edx + 3]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1e], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x22], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x26], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x2a], eax
        mov    al, 1
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2270:
        push   ebp
        mov    ebp, esp
        push   0
        push   0x32
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_08AC
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2290:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x2e], 0
        jne    loc_2_22A9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_22A9:
        mov    eax, dword ptr [0x777c]       ; -> obj1:0x777C
        push   eax
        push   0
        push   0x1f
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_08A6
        or     eax, eax
        je     loc_2_22C9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_22C9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x2e]
        mov    edi, edi
loc_2_22D0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_22E3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_22D0
        jmp    loc_2_22F2
loc_2_22E3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x2e], eax
loc_2_22F2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_2308
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_2308:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 1
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 0x1c], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1e], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  090h
sub_2_2370:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2690
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_2399
        cmp    ebx, 0x103
        jne    loc_2_23B1
loc_2_2399:
        push   0
        push   0x7778                        ; -> obj1:0x7778
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_2460
        or     eax, eax
        je     loc_2_23B1
        mov    ebx, eax
loc_2_23B1:
        or     ebx, ebx
        je     loc_2_23BD
        cmp    ebx, 0x103
        jne    loc_2_23E8
loc_2_23BD:
        cmp    byte ptr [edi + 0x1d], 0
        je     loc_2_23E8
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [edi + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_2_23E8
        push   0
        push   0x7778                        ; -> obj1:0x7778
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_2500
        or     eax, eax
        je     loc_2_23E8
        mov    ebx, eax
loc_2_23E8:
        or     ebx, ebx
        je     loc_2_23F4
        cmp    ebx, 0x103
        jne    loc_2_2455
loc_2_23F4:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_2_2450
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_240A
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_240A:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_2419
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_241F
loc_2_2419:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_2_241F:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_2_242E
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0808
loc_2_242E:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_08AC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_2450:
        mov    ebx, 0xc001000e
loc_2_2455:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_2460:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        xor    edx, edx
        mov    ebx, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        mov    dl, byte ptr [eax]
        shl    edx, 0x10
        or     ecx, edx
        shl    ecx, 8
        xor    edx, edx
        mov    dl, byte ptr [eax + 1]
        shl    edx, 0x10
        or     ecx, edx
        mov    al, byte ptr [eax + 3]
        and    eax, 0xff
        or     ecx, eax
        mov    eax, ecx
        mov    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [esi + 4], 0
        or     eax, eax
        je     loc_2_24C0
loc_2_24B3:
        mov    ecx, dword ptr [eax + 0xc]
        or     dword ptr [esi + 4], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_24B3
loc_2_24C0:
        mov    ecx, dword ptr [esi + 4]
        cmp    ecx, ebx
        je     loc_2_24F4
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 0x1e]
        or     eax, eax
        je     loc_2_24F6
        cmp    eax, 0x103
        je     loc_2_24F6
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0xc], ecx
        mov    dword ptr [esi + 4], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_24F4:
        sub    eax, eax
loc_2_24F6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
sub_2_2500:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 0x14]
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        mov    al, byte ptr [ecx + 2]
        shl    edx, 0x10
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 1]
        shl    eax, 0x10
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        or     edx, eax
        mov    dword ptr [ebp - 4], edx
        mov    al, byte ptr [0x7778]         ; -> obj1:0x7778
        cmp    byte ptr [ecx], al
        jne    loc_2_25A8
        mov    al, byte ptr [0x7779]         ; -> obj1:0x7779
        cmp    byte ptr [ecx + 1], al
        jne    loc_2_25A8
        mov    al, byte ptr [0x777a]         ; -> obj1:0x777A
        cmp    byte ptr [ecx + 2], al
        jne    loc_2_25A8
        mov    al, byte ptr [0x777b]         ; -> obj1:0x777B
        cmp    byte ptr [ecx + 3], al
        jne    loc_2_25A8
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_2589
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [ebx + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_2623
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2589:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_259D
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_259D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_25A8:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    loc_2_25E0
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_25C4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_25C4:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_2623
        mov    byte ptr [ebx + 0x1d], 1
        inc    dword ptr [esi + 0xc]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
loc_2_25E0:
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 1
        jbe    loc_2_25F6
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_25F6:
        mov    ebx, dword ptr [ebp + 0xc]
        jne    loc_2_2610
        cmp    byte ptr [ebx + 0x1d], 0
        jne    loc_2_2610
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
loc_2_2610:
        cmp    eax, 1
        jne    loc_2_2623
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_2623
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        mov    byte ptr [ebx + 0x1d], 0
loc_2_2623:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   edi
        call   dword ptr [esi + 0x22]
        or     eax, eax
        je     loc_2_2656
        cmp    eax, 0x103
        je     loc_2_2656
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0xc], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2656:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_2670
        mov    byte ptr [ebx + 0x1d], 0
        mov    dword ptr [esi + 0xc], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2670:
        mov    byte ptr [ebx + 0x1d], 1
        mov    dword ptr [esi + 0xc], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2690:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        mov    dword ptr [esi + 0x10], 0
        je     loc_2_26CD
        lea    esp, [esp]
loc_2_26C0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0x10], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_26C0
loc_2_26CD:
        mov    ecx, dword ptr [esi + 0x10]
        cmp    ebx, ecx
        je     loc_2_2701
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 0x26]
        or     eax, eax
        je     loc_2_2703
        cmp    eax, 0x103
        je     loc_2_2703
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0x10], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_2701:
        sub    eax, eax
loc_2_2703:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x4f90  R-X
loc_4_0000:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [ecx + 0x10]
        add    dx, 7
        mov    al, byte ptr [ecx + 0x34]
        out    dx, al
        mov    byte ptr [ecx + 0x34], 0
        mov    al, 1
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_4_0020:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    si, word ptr [ecx + 0x10]
        lea    edx, [esi + 4]
        in     al, dx
        mov    bl, al
        lea    edi, [esi + 5]
        mov    dx, di
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        shl    dx, 8
        mov    al, bl
        and    eax, 0xffff00ff
        or     dx, ax
        mov    word ptr [ebp - 2], dx
        mov    bx, word ptr [ecx + 0x643]
        lea    edx, [esi + 4]
        mov    al, bl
        out    dx, al
        mov    dx, di
        mov    al, bh
        out    dx, al
        lea    edi, [esi + 2]
        mov    dx, di
        in     al, dx
        add    si, 3
        mov    dx, si
        in     al, dx
        mov    dx, di
        in     al, dx
        mov    dx, si
        in     al, dx
        sub    esi, esi
loc_4_0083:
        mov    di, word ptr [ecx + 0x10]
        lea    edx, [edi + 2]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 3]
        in     al, dx
        and    ax, 0xff
        shl    ax, 8
        mov    dl, bl
        and    edx, 0xffff00ff
        or     ax, dx
        lea    edx, [esi + ecx]
        add    esi, 2
        mov    byte ptr [edx + 0x63d], ah
        mov    byte ptr [edx + 0x63e], al
        cmp    esi, 6
        jl     loc_4_0083
        mov    cx, word ptr [ecx + 0x10]
        mov    al, byte ptr [ebp - 2]
        lea    edx, [ecx + 4]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0x10]
        mov    dx, di
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 1]
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        shl    dx, 8
        and    cx, 0xff
        or     dx, cx
        mov    word ptr [ebp - 4], dx
        lea    ecx, [edi + 4]
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        add    di, 5
        mov    dx, di
        in     al, dx
        mov    bl, al
        and    ebx, 0xffff00ff
        shl    bx, 8
        mov    al, byte ptr [ebp - 2]
        and    eax, 0xffff00ff
        or     bx, ax
        mov    dx, cx
        mov    al, 6
        out    dx, al
        mov    dx, di
        mov    al, 0xa
        out    dx, al
        call   sub_4_1E20
        mov    cx, word ptr [esi + 0x10]
        lea    edx, [ecx + 2]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [ecx + 3]
        in     al, dx
        and    ax, 0xff
        shl    ax, 8
        mov    cl, byte ptr [ebp - 1]
        and    ecx, 0xffff00ff
        or     ax, cx
        mov    cx, word ptr [esi + 0x10]
        mov    word ptr [esi + 0x643], ax
        lea    edx, [ecx + 4]
        mov    al, bl
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, bh
        out    dx, al
        mov    dx, cx
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        lea    edx, [ecx + 1]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_4_01A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 0x10]
        add    dx, 7
        mov    al, 0x98
        out    dx, al
        mov    al, 1
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_01C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x65]
        cmp    word ptr [eax], 0
        je     loc_4_01D9
        sub    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_4_01D9:
        mov    ax, word ptr [ebp + 0xc]
        mov    dl, ah
        mov    dh, al
        mov    eax, dword ptr [ecx + 0x65]
        or     dword ptr [ecx + 0xb7], 0x800
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, eax
        shr    edx, 8
        mov    ebx, eax
        shr    ebx, 0x18
        mov    esi, dword ptr [ecx + 0x65]
        mov    bh, dh
        mov    word ptr [esi + 2], bx
        mov    dh, al
        mov    eax, dword ptr [ecx + 0x65]
        push   ecx
        mov    word ptr [eax + 4], dx
        push   0                             ; -> loc_4_01A0
        add    ecx, 0x515
        push   ecx
        call   sub_1_089A
loc_4_0222:
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0230:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        or     dword ptr [ecx + 0xb7], 0x80
        cmp    byte ptr [ecx + 0x39], 1
        sbb    eax, eax
        and    eax, 0x1000
        mov    edx, dword ptr [ecx + 0x6d]
        mov    dword ptr [ecx + 0xa1], 0
        mov    word ptr [edx], ax
        mov    eax, dword ptr [ecx + 0x6d]
        or     word ptr [eax], 2
        mov    eax, dword ptr [ecx + 0x6d]
        mov    edx, dword ptr [ecx + 0x6d]
        mov    ax, word ptr [eax]
        mov    word ptr [ecx + 0x649], ax
        mov    ax, word ptr [ecx + 0x54]
        mov    word ptr [edx + 6], ax
        mov    edx, dword ptr [ecx + 0x6d]
        mov    eax, dword ptr [ecx + 0x50]
        mov    dword ptr [edx + 2], eax
        mov    eax, dword ptr [ecx + 0x61]
        mov    ebx, eax
        shr    ebx, 0x10
        mov    dx, ax
        and    edx, 0xff
        shl    edx, 0x10
        mov    dl, bl
        shl    edx, 8
        shr    ax, 8
        and    eax, 0xffff
        shl    eax, 0x10
        shr    bx, 8
        or     edx, eax
        mov    eax, ebx
        and    eax, 0xffff
        or     edx, eax
        mov    eax, dword ptr [ecx + 0x6d]
        mov    dword ptr [eax + 8], edx
        mov    ebx, dword ptr [ecx + 0x5d]
        mov    ax, bx
        shr    ebx, 0x10
        mov    dx, ax
        and    edx, 0xff
        shl    edx, 0x10
        mov    dl, bl
        shl    edx, 8
        shr    ax, 8
        and    eax, 0xffff
        shl    eax, 0x10
        shr    bx, 8
        or     edx, eax
        mov    eax, ebx
        and    eax, 0xffff
        or     edx, eax
        mov    eax, dword ptr [ecx + 0x6d]
        mov    dword ptr [eax + 0xc], edx
        mov    eax, dword ptr [ecx + 0x6d]
        mov    word ptr [eax + 0x10], 0xe00
        mov    eax, dword ptr [ecx + 0x6d]
        mov    word ptr [eax + 0x12], 0x1a00
        mov    eax, dword ptr [ecx + 0x6d]
        mov    word ptr [eax + 0x14], 4
        mov    eax, dword ptr [ecx + 0x6d]
        mov    word ptr [eax + 0x16], 0
        mov    eax, dword ptr [ecx + 0x6d]
        mov    word ptr [eax + 0x18], 0
        mov    eax, dword ptr [ecx + 0x6d]
        mov    byte ptr [eax + 0x1a], 0x80
        mov    eax, dword ptr [ecx + 0x6d]
        mov    byte ptr [eax + 0x1b], 0x80
        mov    eax, dword ptr [ecx + 0x7d]
        push   eax
        push   3
        push   ecx
        call   sub_4_01C0
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0350:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0xb7]
        and    ecx, 0xffffff69
        mov    dword ptr [eax + 0xb7], ecx
        or     ecx, 0x1000
        mov    dword ptr [eax + 0xb7], ecx
        mov    cx, word ptr [eax + 0x10]
        sub    al, al
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 0xff
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0390:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    si, word ptr [ecx + 0x10]
        lea    edx, [esi + 6]
        in     al, dx
        mov    bl, al
        lea    edx, [esi + 7]
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        shl    dx, 8
        mov    al, bl
        and    eax, 0xffff00ff
        or     dx, ax
        mov    ax, dx
        and    ax, 0xf
        mov    word ptr [ecx + 0xb1], ax
        cmp    dl, 0x40
        jne    loc_4_03FB
        mov    eax, dword ptr [ecx + 0xb7]
        and    eax, 0xffffefff
        mov    dword ptr [ecx + 0xb7], eax
        or     eax, 2
        mov    dword ptr [ecx + 0xb7], eax
        mov    dword ptr [ecx + 0x61f], 0
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_03FB:
        sub    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0410:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x24], 0x9f00
        cmp    dword ptr [esi + 0x3e], 9
        jne    loc_4_0436
        mov    word ptr [ebp - 0x1a], 0xfffe
        mov    word ptr [ebp - 0x1c], 0xfffe
        jmp    loc_4_0467
loc_4_0436:
        cmp    dword ptr [esi + 0x46], 1
        jne    loc_4_0450
        cmp    dword ptr [esi + 0x42], 0
        jne    loc_4_0450
        mov    word ptr [ebp - 0x1a], 0x18
        jmp    loc_4_045F
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0450:
        mov    eax, dword ptr [esi + 0x42]
        mov    ax, word ptr [eax*2 + 0x24]   ; -> loc_1_0BF4
        mov    word ptr [ebp - 0x1a], ax
loc_4_045F:
        mov    ax, word ptr [ebp - 0x1a]
        mov    word ptr [ebp - 0x1c], ax
loc_4_0467:
        mov    word ptr [ebp - 0x18], 0x101
        mov    eax, dword ptr [esi + 0x75]
        mov    ecx, eax
        shl    ecx, 0x10
        shr    eax, 0x10
        or     ecx, eax
        mov    dword ptr [ebp - 0x16], ecx
        mov    eax, dword ptr [esi + 0x79]
        mov    ecx, eax
        shl    ecx, 0x10
        shr    eax, 0x10
        or     ecx, eax
        mov    dword ptr [ebp - 0x12], ecx
        push   esi
        call   sub_1_0190
loc_4_0493:
        mov    dx, word ptr [esi + 0x10]
        add    dx, 0xc
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [esi + 0x10]
        add    dx, 0xd
        sub    al, al
        out    dx, al
        mov    dx, word ptr [esi + 0x10]
        add    dx, 4
        out    dx, al
        mov    dx, word ptr [esi + 0x10]
        add    dx, 5
        mov    al, 0xa
        out    dx, al
        lea    edi, [ebp - 0x24]
        mov    ax, word ptr [esi + 0x10]
        add    ax, 2
        mov    word ptr [ebp - 0xc], ax
        mov    ax, word ptr [esi + 0x10]
        add    ax, 3
        mov    word ptr [ebp - 8], ax
        mov    ebx, 0xb
loc_4_04E0:
        mov    cx, word ptr [edi]
        mov    dx, word ptr [ebp - 0xc]
        mov    al, cl
        out    dx, al
        mov    dx, word ptr [ebp - 8]
        mov    al, ch
        out    dx, al
        add    edi, 2
        dec    ebx
        jne    loc_4_04E0
        mov    di, word ptr [esi + 0x10]
        add    di, 8
        mov    dx, di
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    bx, word ptr [esi + 0x10]
        add    bx, 9
        mov    dx, bx
        in     al, dx
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    al, byte ptr [ebp - 1]
        and    eax, 0xffff00ff
        or     cx, ax
        or     cx, 8
        cmp    dword ptr [esi + 0x3e], 9
        je     loc_4_0538
        or     cx, 0x100
loc_4_0538:
        mov    dx, di
        mov    al, cl
        out    dx, al
        or     dword ptr [esi + 0xb7], 0x4000
        mov    dx, bx
        mov    al, ch
        mov    cx, word ptr [esi + 0x10]
        out    dx, al
        lea    edx, [ecx + 6]
        mov    al, 0x80
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 0x90
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0570:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0x10]
        lea    edx, [edi + 6]
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 7]
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        shl    dx, 8
        mov    al, cl
        and    eax, 0xffff00ff
        or     dx, ax
        test   dl, 0x10
        je     loc_4_05BA
        mov    word ptr [esi + 0xb3], dx
        and    dword ptr [esi + 0xb7], 0xffffbfff
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_05BA:
        test   dl, 0x60
        jne    loc_4_0630
        mov    eax, dword ptr [esi + 0x65]
        and    dword ptr [esi + 0xb7], 0xffffbfff
        cmp    word ptr [eax], 0
        jne    loc_4_0620
        cmp    word ptr [eax + 2], 0xe2c1
        jne    loc_4_0620
        cmp    word ptr [eax + 4], 0x8bd4
        jne    loc_4_0620
        mov    eax, dword ptr [esi + 0x69]
        cmp    word ptr [eax], 0xffff
        jne    loc_4_0620
        cmp    word ptr [eax + 2], 0xd7d1
        jne    loc_4_0620
        cmp    word ptr [eax + 4], 0xd9c5
        jne    loc_4_0620
        cmp    word ptr [eax + 6], 0xd4c3
        jne    loc_4_0620
        push   esi
        call   sub_4_00E0
        or     dword ptr [esi + 0xb7], 4
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0620:
        mov    word ptr [esi + 0xb3], dx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_4_0630:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_0640:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    al, al
        mov    cx, word ptr [esi + 0x10]
        and    dword ptr [esi + 0xb7], 0xffffff69
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 0xff
        out    dx, al
        sub    bx, bx
        sub    edi, edi
        add    eax, 0
loc_4_0670:
        push   0x2710
        call   sub_1_08A0
loc_4_067A:
        mov    cx, bx
        mov    bx, word ptr [esi + 0x10]
        lea    edx, [ebx + 6]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [ebx + 7]
        in     al, dx
        mov    bl, al
        and    ebx, 0xffff00ff
        shl    bx, 8
        mov    al, byte ptr [ebp - 1]
        and    eax, 0xffff00ff
        or     bx, ax
        mov    word ptr [esi + 0xb1], bx
        cmp    cx, bx
        je     loc_4_06B4
        cmp    bl, 0x40
        je     loc_4_0710
loc_4_06B4:
        add    edi, 0x2710
        cmp    edi, 0x5b8d80
        jl     loc_4_0670
        test   bl, 0x30
        je     loc_4_06F4
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_06F4
        and    ebx, 0xf
        push   ebx
        push   0xc0000017
        push   0xc1000001
        push   3
        push   0xc1000001
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_06F1:
        add    esp, 0x18
loc_4_06F4:
        mov    dword ptr [esi + 0x61f], 0xc0000001
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0710:
        or     dword ptr [esi + 0xb7], 2
        mov    dword ptr [esi + 0x61f], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_0730:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 1], 0
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x28], 0x9f00
        cmp    dword ptr [esi + 0x3e], 9
        jne    loc_4_075A
        mov    word ptr [ebp - 0x1e], 0xfffe
        mov    word ptr [ebp - 0x20], 0xfffe
        jmp    loc_4_0787
loc_4_075A:
        cmp    dword ptr [esi + 0x46], 1
        jne    loc_4_0770
        cmp    dword ptr [esi + 0x42], 0
        jne    loc_4_0770
        mov    word ptr [ebp - 0x1e], 0x18
        jmp    loc_4_077F
        db  08Bh, 0FFh
loc_4_0770:
        mov    eax, dword ptr [esi + 0x42]
        mov    ax, word ptr [eax*2 + 0x2c]   ; -> loc_1_0BFC
        mov    word ptr [ebp - 0x1e], ax
loc_4_077F:
        mov    ax, word ptr [ebp - 0x1e]
        mov    word ptr [ebp - 0x20], ax
loc_4_0787:
        mov    word ptr [ebp - 0x1c], 0x101
        mov    eax, dword ptr [esi + 0x75]
        mov    ecx, eax
        shl    ecx, 0x10
        shr    eax, 0x10
        or     ecx, eax
        mov    dword ptr [ebp - 0x1a], ecx
        mov    eax, dword ptr [esi + 0x79]
        mov    ecx, eax
        shl    ecx, 0x10
        shr    eax, 0x10
        or     ecx, eax
        mov    dword ptr [ebp - 0x16], ecx
        push   esi
        call   sub_1_0190
loc_4_07B3:
        mov    dword ptr [ebp - 0x10], 0
loc_4_07BA:
        cmp    byte ptr [ebp - 1], 0
        je     loc_4_07D2
        push   esi
        push   0                             ; -> loc_4_0640
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_07D2:
        mov    byte ptr [ebp - 1], 1
        mov    dx, word ptr [esi + 0x10]
        add    dx, 0xc
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [esi + 0x10]
        add    dx, 0xd
        sub    al, al
        out    dx, al
        mov    dx, word ptr [esi + 0x10]
        add    dx, 4
        out    dx, al
        mov    dx, word ptr [esi + 0x10]
        add    dx, 5
        mov    al, 0xa
        out    dx, al
        lea    edi, [ebp - 0x28]
        mov    ax, word ptr [esi + 0x10]
        add    ax, 2
        mov    word ptr [ebp - 0xc], ax
        mov    ax, word ptr [esi + 0x10]
        add    ax, 3
        mov    word ptr [ebp - 8], ax
        mov    ebx, 0xb
loc_4_0820:
        mov    cx, word ptr [edi]
        mov    dx, word ptr [ebp - 0xc]
        mov    al, cl
        out    dx, al
        mov    dx, word ptr [ebp - 8]
        mov    al, ch
        out    dx, al
        add    edi, 2
        dec    ebx
        jne    loc_4_0820
        mov    di, word ptr [esi + 0x10]
        add    di, 8
        mov    dx, di
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi + 0x10]
        add    cx, 9
        mov    dx, cx
        in     al, dx
        mov    bl, al
        and    ebx, 0xffff00ff
        shl    bx, 8
        mov    al, byte ptr [ebp - 2]
        and    eax, 0xffff00ff
        or     bx, ax
        or     bx, 8
        cmp    dword ptr [esi + 0x3e], 9
        je     loc_4_0878
        or     bx, 0x100
loc_4_0878:
        mov    dx, di
        mov    al, bl
        out    dx, al
        mov    dx, cx
        mov    al, bh
        out    dx, al
        mov    dx, word ptr [esi + 0x10]
        add    dx, 6
        mov    al, 0x80
        out    dx, al
        mov    dx, word ptr [esi + 0x10]
        add    dx, 7
        mov    al, 0x90
        mov    byte ptr [esi + 0x4e6], 0
        mov    dword ptr [esi + 0x4da], 0
        mov    dword ptr [esi + 0x4de], 0    ; -> loc_4_1CC0
        mov    dword ptr [esi + 0x4e2], esi
        out    dx, al
        push   0x2710
        lea    eax, [esi + 0x4da]
        push   eax
        call   sub_1_0882
loc_4_08CC:
        lea    esp, [esp]
loc_4_08D0:
        cmp    byte ptr [esi + 0x4e6], 0
        jne    loc_4_09B0
        push   0x3e8
        call   sub_1_08A0
loc_4_08E7:
        mov    di, word ptr [esi + 0x10]
        lea    edx, [edi + 6]
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 7]
        in     al, dx
        mov    bl, al
        and    ebx, 0xffff00ff
        shl    bx, 8
        mov    al, cl
        and    eax, 0xffff00ff
        or     bx, ax
        test   bl, 0x10
        jne    loc_4_0A00
        test   bl, 0x60
        jne    loc_4_08D0
        mov    eax, dword ptr [esi + 0x65]
        cmp    word ptr [eax], 0
        jne    loc_4_0A58
        cmp    word ptr [eax + 2], 0xe2c1
        jne    loc_4_0A58
        cmp    word ptr [eax + 4], 0x8bd4
        jne    loc_4_0A58
        mov    eax, dword ptr [esi + 0x69]
        cmp    word ptr [eax], 0xffff
        jne    loc_4_0A58
        cmp    word ptr [eax + 2], 0xd7d1
        jne    loc_4_0A58
        cmp    word ptr [eax + 4], 0xd9c5
        jne    loc_4_0A58
        cmp    word ptr [eax + 6], 0xd4c3
        jne    loc_4_0A58
        cmp    byte ptr [esi + 0x4e6], 0
        jne    loc_4_0989
        lea    eax, [ebp - 3]
        push   eax
        lea    eax, [esi + 0x4da]
        push   eax
        call   sub_1_0888
loc_4_0989:
        or     dword ptr [esi + 0xb7], 4
        push   esi
        push   0                             ; -> sub_4_00E0
        add    esi, 0x515
        push   esi
        call   sub_1_089A
loc_4_09A2:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_4_09B0:
        inc    dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x10], 3
        jb     loc_4_07BA
        and    dword ptr [esi + 0xb7], 0xfffffffa
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_09F2
        mov    eax, ebx
        and    eax, 0xffff
        push   eax
        push   0xc0000009
        push   3
        push   3
        push   0xaa000008
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_09EF:
        add    esp, 0x18
loc_4_09F2:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0A00:
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_0A31
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        and    ebx, 0xf
        push   ebx
        push   0xc0000008
        push   0xc1000002
        push   4
        push   0xc1000002
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_0A2E:
        add    esp, 0x1c
loc_4_0A31:
        cmp    byte ptr [esi + 0x4e6], 0
        jne    loc_4_0A4A
        lea    eax, [ebp - 3]
        push   eax
        lea    eax, [esi + 0x4da]
        push   eax
        call   sub_1_0888
loc_4_0A4A:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0A58:
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_0A7E
        push   0xc000000a
        push   3
        push   2
        push   0xaa00000b
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_0A7B:
        add    esp, 0x14
loc_4_0A7E:
        cmp    byte ptr [esi + 0x4e6], 0
        jne    loc_4_0A97
        lea    eax, [ebp - 3]
        push   eax
        lea    eax, [esi + 0x4da]
        push   eax
        call   sub_1_0888
loc_4_0A97:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xa1]
        or     eax, eax
        je     loc_4_0AE0
        cmp    eax, 1
        je     loc_4_0B12
        cmp    eax, 2
        je     loc_4_0B34
        cmp    eax, 3
        je     loc_4_0BA8
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_0AE0:
        sub    eax, eax
        mov    dword ptr [esi + 0xa1], 1
        cmp    byte ptr [esi + 0xe6], al
        je     loc_4_0AF9
        mov    eax, 0x8000
loc_4_0AF9:
        mov    ecx, eax
        shl    ecx, 0x10
        shr    eax, 0x10
        or     ecx, eax
        push   ecx
        push   4
        push   esi
        call   sub_4_01C0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_0B12:
        mov    dword ptr [esi + 0xa1], 2
        push   1
        push   esi
        call   sub_4_4DE0
        push   0
        push   6
        push   esi
        call   sub_4_01C0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_0B34:
        mov    eax, dword ptr [esi + 0x59]
        test   eax, 0x8020
        je     loc_4_0BA8
        xor    edi, edi
        mov    di, word ptr [esi + 0x649]
        test   al, 0x20
        je     loc_4_0B51
        or     edi, 0x200
loc_4_0B51:
        test   ah, 0x80
        je     loc_4_0B5C
        or     edi, 0x400
loc_4_0B5C:
        mov    dword ptr [esi + 0xa1], 3
        mov    edx, edi
        shr    edx, 0x10
        mov    cx, di
        and    ecx, 0xff
        shl    ecx, 0x10
        mov    cl, dl
        shr    di, 8
        shl    ecx, 8
        mov    eax, edi
        and    eax, 0xffff
        shl    eax, 0x10
        shr    dx, 8
        or     ecx, eax
        mov    eax, edx
        and    eax, 0xffff
        or     ecx, eax
        push   ecx
        push   0xd
        push   esi
        call   sub_4_01C0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_0BA8:
        mov    eax, dword ptr [esi + 0xb7]
        and    eax, 0xffffff77
        mov    dword ptr [esi + 0xb7], eax
        or     eax, 0x10
        mov    dword ptr [esi + 0xb7], eax
        mov    dword ptr [esi + 0x4ef], 0x4b0
        push   esi
        push   0                             ; -> loc_4_0020
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_0BDE:
        push   esi
        call   sub_4_3480
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0BF0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x4eb], 0xa
        je     loc_4_0C22
        test   byte ptr [esi + 0xb7], 0x10
        je     loc_4_0C22
        mov    eax, dword ptr [esi + 0x4ef]
        dec    eax
        mov    dword ptr [esi + 0x4ef], eax
        jne    loc_4_0C22
        mov    dword ptr [esi + 0x4eb], 0xa
loc_4_0C22:
        mov    eax, dword ptr [esi + 0xb7]
        test   al, 0x40
        je     loc_4_0C36
        cmp    word ptr [esi + 0x623], 4
        je     loc_4_0C44
loc_4_0C36:
        test   al, 0x58
        je     loc_4_0C44
        and    al, 0x24
        cmp    al, 0x24
        jne    loc_4_0F35
loc_4_0C44:
        push   esi
        call   sub_4_3440
        or     al, al
        je     loc_4_0F35
        mov    eax, dword ptr [esi + 0x4e7]
        dec    eax
        mov    dword ptr [esi + 0x4e7], eax
        jne    loc_4_0F35
        mov    eax, dword ptr [esi + 0xb7]
        cmp    eax, 1
        jne    loc_4_0CA0
        push   esi
        push   0                             ; -> loc_4_0350
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_0C82:
        mov    dword ptr [esi + 0x4e7], 6
        mov    dword ptr [esi + 0x4f3], 0x1e
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_0CA0:
        and    eax, 0xd1bf
        cmp    eax, 0x1001
        jne    loc_4_0D66
        push   esi
        push   0                             ; -> loc_4_0390
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_0CC2:
        or     al, al
        mov    dword ptr [esi + 0x4e7], 0xa
        je     loc_4_0CD5
        pop    esi
        pop    ebp
        ret    4
loc_4_0CD5:
        mov    eax, dword ptr [esi + 0x4f3]
        dec    eax
        mov    dword ptr [esi + 0x4f3], eax
        jne    loc_4_0F35
        and    dword ptr [esi + 0xb7], 0xffffeffd
        mov    ax, word ptr [esi + 0xb1]
        test   al, 0x30
        je     loc_4_0D30
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_0D59
        and    eax, 0xffff
        push   eax
        push   0xc0000017
        push   0xc1000001
        push   3
        push   0xc1000001
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_0D29:
        add    esp, 0x18
        jmp    loc_4_0D59
        db  08Bh, 0FFh
loc_4_0D30:
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_0D59
        push   0xc0000017
        push   0xc1000001
        push   2
        push   0xc1000001
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_0D56:
        add    esp, 0x14
loc_4_0D59:
        or     word ptr [esi + 0x619], 8
        jmp    loc_4_0EE8
loc_4_0D66:
        cmp    eax, 3
        jne    loc_4_0D96
        push   esi
        push   0                             ; -> loc_4_0410
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_0D7D:
        mov    dword ptr [esi + 0x4e7], 0xa
        mov    dword ptr [esi + 0x4f3], 0x44c
        pop    esi
        pop    ebp
        ret    4
loc_4_0D96:
        cmp    eax, 0x4003
        jne    loc_4_0E81
        push   esi
        push   0                             ; -> loc_4_0570
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_0DB3:
        or     al, al
        je     loc_4_0E50
        test   byte ptr [esi + 0xb7], 4
        je     loc_4_0DD3
        mov    dword ptr [esi + 0x4e7], 0xa
        pop    esi
        pop    ebp
        ret    4
loc_4_0DD3:
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_0E30
        mov    ax, word ptr [esi + 0xb3]
        test   al, 0x10
        je     loc_4_0E10
        and    eax, 0xffff
        push   eax
        push   0xc0000008
        push   0xc1000002
        push   3
        push   0xc1000002
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_0E0A:
        add    esp, 0x18
        jmp    loc_4_0E30
        db  090h
loc_4_0E10:
        push   0xc000000a
        push   0xaa00000b
        push   2
        push   0xaa00000b
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_0E2D:
        add    esp, 0x14
loc_4_0E30:
        or     word ptr [esi + 0x619], 8
        and    dword ptr [esi + 0xb7], 0xffffaff9
        jmp    loc_4_0EE8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0E50:
        mov    dword ptr [esi + 0x4e7], 0xa
        mov    eax, dword ptr [esi + 0x4f3]
        dec    eax
        mov    dword ptr [esi + 0x4f3], eax
        jne    loc_4_0F35
        and    dword ptr [esi + 0xb7], 0xffffaff9
        or     word ptr [esi + 0x619], 8
        jmp    loc_4_0EE8
loc_4_0E81:
        cmp    eax, 7
        jne    loc_4_0EE8
        push   esi
        call   sub_4_3440
        or     al, al
        je     loc_4_0EB0
        mov    eax, dword ptr [esi + 0xb7]
        test   al, 0x20
        jne    loc_4_0EB0
        test   al, 0x40
        je     loc_4_0EA7
        mov    word ptr [esi + 0x623], 5
loc_4_0EA7:
        push   esi
        call   sub_4_0230
        jmp    loc_4_0EE8
        db  090h
loc_4_0EB0:
        mov    eax, dword ptr [esi + 0xb7]
        and    eax, 0xfffffff7
        mov    dword ptr [esi + 0xb7], eax
        test   al, 4
        je     loc_4_0EE0
        test   eax, 0x2148
        jne    loc_4_0EE0
        push   esi
        push   0                             ; -> loc_4_1870
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_0EDC:
        jmp    loc_4_0EE8
        db  08Bh, 0FFh
loc_4_0EE0:
        or     word ptr [esi + 0x619], 0x10
loc_4_0EE8:
        mov    eax, dword ptr [esi + 0x4eb]
        add    eax, eax
        mov    dword ptr [esi + 0x4eb], eax
        cmp    eax, 0x280
        jbe    loc_4_0F07
        mov    dword ptr [esi + 0x4eb], 0x280
loc_4_0F07:
        mov    eax, dword ptr [esi + 0x4eb]
        mov    dword ptr [esi + 0x4e7], eax
        test   byte ptr [esi + 0x619], 8
        je     loc_4_0F35
        test   byte ptr [esi + 0xb7], 0x40
        push   esi
        je     loc_4_0F30
        call   sub_4_2C80
        pop    esi
        pop    ebp
        ret    4
loc_4_0F30:
        call   sub_4_3380
loc_4_0F35:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0F40:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    dword ptr [ecx + 0x48e], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 6]
        and    eax, 0xff
        add    dword ptr [ecx + 0x492], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 7]
        and    eax, 0xff
        add    dword ptr [ecx + 0x43a], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        add    dword ptr [ecx + 0x496], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 3]
        and    eax, 0xff
        add    dword ptr [ecx + 0x49a], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        add    dword ptr [ecx + 0x4a2], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        add    dword ptr [ecx + 0x4aa], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        add    dword ptr [ecx + 0x4ae], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        add    dword ptr [ecx + 0x49e], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 9]
        and    eax, 0xff
        add    dword ptr [ecx + 0x4a6], eax
        mov    eax, dword ptr [ecx + 0x71]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        add    dword ptr [ecx + 0x496], eax
        mov    eax, dword ptr [ecx + 0x71]
        pop    ebp
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        add    dword ptr [ecx + 0x49a], eax
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1020:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 1
        mov    cx, word ptr [esi + 0x10]
        lea    edx, [ecx + 0xc]
        out    dx, al
        lea    edx, [ecx + 0xd]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, 0xe0
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, 5
        out    dx, al
        lea    ecx, [esi + 0xbb]
        mov    edi, 4
loc_4_1053:
        mov    bx, word ptr [esi + 0x10]
        lea    edx, [ebx + 2]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [ebx + 3]
        in     al, dx
        and    ax, 0xff
        shl    ax, 8
        mov    dl, byte ptr [ebp - 1]
        and    edx, 0xffff00ff
        or     ax, dx
        mov    word ptr [ecx], ax
        add    ecx, 2
        dec    edi
        jne    loc_4_1053
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_1090:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   0                             ; -> loc_4_1020
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_10A9:
        xor    eax, eax
        mov    ax, word ptr [esi + 0xc1]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0xbf]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0xbd]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0xbb]
        push   eax
        push   0xc0000024
        push   0xc1000003
        push   6
        push   0xc1000003
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_10EE:
        add    esp, 0x24
        push   5
        push   esi
        call   sub_4_1890
        mov    dword ptr [esi + 0xb7], 0x20
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_1110:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0xa7]
        test   di, 0x8000
        mov    word ptr [esi + 0xab], di
        mov    dword ptr [esi + 0xad], 0
        je     loc_4_1142
        mov    dword ptr [esi + 0xad], 0x8000
loc_4_1142:
        test   di, 0x4000
        je     loc_4_1153
        or     dword ptr [esi + 0xad], 0x4000
loc_4_1153:
        test   di, 0x2000
        je     loc_4_1164
        or     dword ptr [esi + 0xad], 0x2000
loc_4_1164:
        test   di, 0x1000
        je     loc_4_1175
        or     dword ptr [esi + 0xad], 0x1000
loc_4_1175:
        test   di, 0x800
        je     loc_4_1186
        or     dword ptr [esi + 0xad], 0x800
loc_4_1186:
        test   di, 0x400
        je     loc_4_1197
        or     dword ptr [esi + 0xad], 0x400
loc_4_1197:
        test   di, 0x100
        je     loc_4_11A8
        or     dword ptr [esi + 0xad], 0x200
loc_4_11A8:
        test   di, 0x80
        je     loc_4_11BF
        or     dword ptr [esi + 0xad], 0x100
        push   esi
        call   sub_4_3410
loc_4_11BF:
        test   di, 0x40
        je     loc_4_11D0
        or     dword ptr [esi + 0xad], 0x80
loc_4_11D0:
        test   di, 0x20
        je     loc_4_11DE
        or     dword ptr [esi + 0xad], 0x40
loc_4_11DE:
        mov    bx, di
        and    bx, 0xd00
        je     loc_4_1298
        test   di, 0x500
        je     loc_4_11FA
        or     dword ptr [esi + 0xb7], 0x20
loc_4_11FA:
        push   5
        push   esi
        call   sub_4_1890
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_1298
        mov    eax, ebx
        and    eax, 0xffff
        cmp    eax, 0x100
        je     loc_4_1230
        cmp    eax, 0x400
        je     loc_4_1250
        cmp    eax, 0x800
        je     loc_4_1270
        jmp    loc_4_1298
        db  08Dh, 049h, 000h
loc_4_1230:
        mov    eax, edi
        and    eax, 0xffff
        push   eax
        push   0xc0000023
        push   0xc100000c
        push   3
        push   0xc100000c
        jmp    loc_4_1289
        db  005h, 000h, 000h, 000h, 000h
loc_4_1250:
        mov    eax, edi
        and    eax, 0xffff
        push   eax
        push   0xc0000023
        push   0xc100000b
        push   3
        push   0xc100000b
        jmp    loc_4_1289
        db  005h, 000h, 000h, 000h, 000h
loc_4_1270:
        mov    eax, edi
        and    eax, 0xffff
        push   eax
        push   0xc0000023
        push   0x8100000a
        push   3
        push   0x8100000a
loc_4_1289:
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_1295:
        add    esp, 0x18
loc_4_1298:
        push   4
        lea    eax, [esi + 0xad]
        push   eax
        push   0x40010006
        push   esi
        call   sub_4_34D0
        push   esi
        push   0                             ; -> loc_4_0020
        add    esi, 0x515
        push   esi
        call   sub_1_089A
loc_4_12BE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_12D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0xa5]
        cmp    eax, 3
        je     loc_4_1303
        cmp    eax, 7
        jl     loc_4_1518
        cmp    eax, 0xa
        jle    loc_4_1484
        cmp    eax, 0xd
        jne    loc_4_1518
loc_4_1303:
        test   byte ptr [esi + 0xb7], 0x80
        je     loc_4_1484
        mov    ax, word ptr [esi + 0xa7]
        mov    word ptr [esi + 0xb5], ax
        cmp    ah, 0x80
        jne    loc_4_1350
        mov    word ptr [esi + 0xb5], 0
        mov    word ptr [esi + 0xab], 0
        mov    dword ptr [esi + 0xad], 0
        push   esi
        call   sub_4_0AB0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_4_1350:
        test   ah, 2
        je     loc_4_1440
        and    ax, 0xff
        mov    word ptr [esi + 0xb5], ax
        and    ax, 0xf
        cmp    ax, 7
        je     loc_4_1384
        cmp    ax, 8
        je     loc_4_1384
        cmp    ax, 9
        je     loc_4_1384
        cmp    ax, 0xa
        jne    loc_4_146F
loc_4_1384:
        or     dword ptr [esi + 0xb7], 0x20
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_146F
        mov    cx, word ptr [esi + 0xb5]
        mov    ax, cx
        and    eax, 0xf
        cmp    eax, 7
        je     loc_4_13C0
        cmp    eax, 8
        je     loc_4_13E0
        cmp    eax, 9
        je     loc_4_1400
        cmp    eax, 0xa
        je     loc_4_1420
        jmp    loc_4_146F
        db  08Bh, 0FFh
loc_4_13C0:
        mov    eax, ecx
        and    eax, 0xffff
        push   eax
        push   0xc000001e
        push   0xc1000004
        push   3
        push   0xc1000004
        jmp    loc_4_1460
        db  08Bh, 0FFh
loc_4_13E0:
        mov    eax, ecx
        and    eax, 0xffff
        push   eax
        push   0xc000001e
        push   0xc1000005
        push   3
        push   0xc1000005
        jmp    loc_4_1460
        db  005h, 000h, 000h, 000h, 000h
loc_4_1400:
        mov    eax, ecx
        and    eax, 0xffff
        push   eax
        push   0xc000001e
        push   0xc1000006
        push   3
        push   0xc1000006
        jmp    loc_4_1460
        db  005h, 000h, 000h, 000h, 000h
loc_4_1420:
        mov    eax, ecx
        and    eax, 0xffff
        push   eax
        push   0xc000001e
        push   0xc1000007
        push   3
        push   0xc1000007
        jmp    loc_4_1460
        db  005h, 000h, 000h, 000h, 000h
loc_4_1440:
        cmp    dword ptr [esi + 0x4eb], 0xa
        jne    loc_4_146F
        and    eax, 0xffff
        push   eax
        push   0xc000001e
        push   0x81000008
        push   3
        push   0x81000008
loc_4_1460:
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_146C:
        add    esp, 0x18
loc_4_146F:
        push   5
        push   esi
        call   sub_4_1890
        push   esi
        call   sub_4_3480
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1484:
        mov    eax, dword ptr [esi + 0x549]
        mov    byte ptr [eax + 0xe], 3
        cmp    word ptr [esi + 0xa7], 0x8000
        mov    dword ptr [esi + 0x61b], 0
        je     loc_4_14AD
        mov    dword ptr [esi + 0x61b], 0xc0000001
loc_4_14AD:
        cmp    word ptr [esi + 0xa5], 0xa
        jne    loc_4_14C8
        cmp    word ptr [esi + 0xa7], 0x8000
        jne    loc_4_14C8
        push   esi
        call   sub_4_0F40
loc_4_14C8:
        cmp    word ptr [esi + 0xa5], 0xd
        jne    loc_4_1518
        test   byte ptr [esi + 0xb8], 2
        je     loc_4_1503
        and    dword ptr [esi + 0x59], 0xffff7fdf
        and    dword ptr [esi + 0xb7], 0xfffffbff
        lea    eax, [ebp - 1]
        push   eax
        add    esi, 0x64b
        push   esi
        call   sub_1_0888
loc_4_14FC:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1503:
        test   dword ptr [esi + 0x59], 0x8020
        je     loc_4_1518
        push   0
        push   0
        push   esi
        push   0
        call   sub_4_1DB0
loc_4_1518:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_1520:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 0xb7], 0x2000
        mov    eax, dword ptr [esi + 0x69]
        mov    ax, word ptr [eax]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esi + 0x69]
        mov    word ptr [esi + 0xa5], cx
        mov    ax, word ptr [eax + 2]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esi + 0x69]
        mov    word ptr [esi + 0xa7], cx
        mov    ax, word ptr [eax + 4]
        mov    ch, al
        mov    cl, ah
        xor    eax, eax
        mov    al, byte ptr [esi + 0x33]
        mov    word ptr [esi + 0xa9], cx
        or     eax, eax
        je     loc_4_1580
        cmp    eax, 4
        je     loc_4_1588
        cmp    eax, 6
        je     loc_4_1590
        cmp    eax, 8
        je     loc_4_15B6
        jmp    loc_4_15BC
loc_4_1580:
        push   esi
        call   sub_4_1090
        jmp    loc_4_15BC
loc_4_1588:
        push   esi
        call   sub_4_1110
        jmp    loc_4_15BC
loc_4_1590:
        mov    eax, dword ptr [esi + 0xb7]
        and    eax, 0xfffff7ff
        mov    dword ptr [esi + 0xb7], eax
        test   al, 0x80
        je     loc_4_15BC
        cmp    dword ptr [esi + 0xa1], 0
        je     loc_4_15BC
        push   esi
        call   sub_4_0AB0
        jmp    loc_4_15BC
loc_4_15B6:
        push   esi
        call   sub_4_12D0
loc_4_15BC:
        cmp    word ptr [esi + 0x619], 0
        je     loc_4_15CC
        push   esi
        call   sub_4_3380
loc_4_15CC:
        and    dword ptr [esi + 0xb7], 0xffffdfff
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_15E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    si, word ptr [ebx + 0x10]
        mov    ax, word ptr [ebx + 0x31]
        lea    edx, [esi + 9]
        shr    ax, 8
        or     al, 8
        out    dx, al
        lea    edx, [esi + 0xe]
        in     al, dx
        mov    cl, al
        lea    edx, [esi + 0xf]
        in     al, dx
        mov    di, ax
        and    edi, 0xffff00ff
        shl    di, 8
        mov    al, cl
        and    eax, 0xffff00ff
        or     di, ax
        je     loc_4_184E
        lea    edx, [esi + 6]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        lea    edx, [esi + 7]
        in     al, dx
        mov    cl, al
        and    ecx, 0xffff00ff
        shl    cx, 8
        mov    dl, byte ptr [ebp - 2]
        and    edx, 0xffff00ff
        or     cx, dx
        lea    edx, [esi + 4]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [esi + 5]
        in     al, dx
        and    ax, 0xff
        shl    ax, 8
        mov    dl, byte ptr [ebp - 1]
        and    edx, 0xffff00ff
        or     ax, dx
        and    ecx, 0xffff
        shl    ecx, 0x10
        and    eax, 0xffff
        or     ecx, eax
        test   byte ptr [ebx + 0x32], 4
        je     loc_4_1755
        xor    eax, eax
        mov    al, byte ptr [ebx + 0x1e]
        or     eax, eax
        je     loc_4_16A0
        cmp    eax, 1
        je     loc_4_16C0
        cmp    eax, 2
        je     loc_4_16F0
        cmp    eax, 4
        je     loc_4_16F0
        jmp    loc_4_184E
        db  08Bh, 0FFh
loc_4_16A0:
        mov    eax, edi
        and    eax, 0xffff
        push   eax
        push   ecx
        mov    eax, esi
        and    eax, 0xffff
        push   eax
        call   sub_1_077D
loc_4_16B6:
        jmp    loc_4_184E
        db  005h, 000h, 000h, 000h, 000h
loc_4_16C0:
        mov    dword ptr [ebp - 0x2c], ecx
        mov    word ptr [ebp - 0x16], di
        mov    ax, word ptr [ebx + 0x10]
        mov    word ptr [ebp - 0x14], ax
        push   ecx
        push   edx
        push   edi
        movzx  ecx, word ptr [ebp - 0x16]
        mov    edi, dword ptr [ebp - 0x2c]
        mov    dx, word ptr [ebp - 0x14]
        shr    cx, 1
        rep insw word ptr es:[edi], dx
        adc    cx, cx
        rep insb byte ptr es:[edi], dx
        pop    edi
        pop    edx
        pop    ecx
        jmp    loc_4_184E
loc_4_16F0:
        mov    dword ptr [ebp - 0x28], ecx
        mov    word ptr [ebp - 0x10], di
        mov    ax, word ptr [ebx + 0x10]
        add    ax, 0x10
        mov    word ptr [ebp - 0x12], ax
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    dx, word ptr [ebp - 0x12]
        movzx  ecx, word ptr [ebp - 0x10]
        mov    edi, dword ptr [ebp - 0x28]
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        cmp    ax, cx
        jb     loc_4_173F
        sub    ax, cx
        shr    cx, 1
        jae    loc_4_1731
        sub    dl, 0x10
        insb   byte ptr es:[edi], dx
        add    dl, 0x10
loc_4_1731:
        rep insw word ptr es:[edi], dx
        mov    cx, ax
        shr    cx, 2
        rep insd dword ptr es:[edi], dx
        and    al, 3
loc_4_173F:
        mov    cl, al
        shr    cx, 1
        rep insw word ptr es:[edi], dx
        adc    cx, cx
        rep insb byte ptr es:[edi], dx
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        jmp    loc_4_184E
loc_4_1755:
        xor    eax, eax
        mov    al, byte ptr [ebx + 0x1e]
        or     eax, eax
        je     loc_4_1776
        cmp    eax, 1
        je     loc_4_1791
        cmp    eax, 2
        je     loc_4_17C1
        cmp    eax, 4
        je     loc_4_17F0
        jmp    loc_4_184E
loc_4_1776:
        mov    eax, edi
        and    eax, 0xffff
        push   eax
        push   ecx
        mov    eax, esi
        and    eax, 0xffff
        push   eax
        call   sub_1_0740
loc_4_178C:
        jmp    loc_4_184E
loc_4_1791:
        mov    dword ptr [ebp - 0x24], ecx
        mov    word ptr [ebp - 0xe], di
        mov    ax, word ptr [ebx + 0x10]
        mov    word ptr [ebp - 0xc], ax
        push   ecx
        push   edx
        push   esi
        movzx  ecx, word ptr [ebp - 0xe]
        mov    esi, dword ptr [ebp - 0x24]
        mov    dx, word ptr [ebp - 0xc]
        shr    cx, 1
        rep outsw dx, word ptr [esi]
        adc    cx, cx
        rep outsb dx, byte ptr [esi]
        pop    esi
        pop    edx
        pop    ecx
        jmp    loc_4_184E
loc_4_17C1:
        mov    dword ptr [ebp - 0x20], ecx
        mov    word ptr [ebp - 0xa], di
        mov    ax, word ptr [ebx + 0x10]
        mov    word ptr [ebp - 8], ax
        push   ecx
        push   edx
        push   esi
        movzx  ecx, word ptr [ebp - 0xa]
        mov    esi, dword ptr [ebp - 0x20]
        mov    dx, word ptr [ebp - 8]
        shr    cx, 1
        rep outsw dx, word ptr [esi]
        adc    cx, cx
        rep outsb dx, byte ptr [esi]
        pop    esi
        pop    edx
        pop    ecx
        jmp    loc_4_184E
        db  08Bh, 0FFh
loc_4_17F0:
        mov    dword ptr [ebp - 0x1c], ecx
        mov    word ptr [ebp - 6], di
        mov    ax, word ptr [ebx + 0x10]
        add    ax, 0x10
        mov    word ptr [ebp - 4], ax
        push   ecx
        push   edx
        push   esi
        movzx  ecx, word ptr [ebp - 6]
        mov    esi, dword ptr [ebp - 0x1c]
        mov    dx, word ptr [ebp - 4]
        mov    eax, ecx
        mov    cx, si
        neg    cl
        and    cx, 3
        cmp    ax, cx
        jb     loc_4_183E
        sub    ax, cx
        shr    cx, 1
        jae    loc_4_1830
        sub    dl, 0x10
        outsb  dx, byte ptr [esi]
        add    dl, 0x10
loc_4_1830:
        rep outsw dx, word ptr [esi]
        mov    cx, ax
        shr    cx, 2
        rep outsd dx, dword ptr [esi]
        and    al, 3
loc_4_183E:
        mov    cl, al
        shr    cx, 1
        rep outsw dx, word ptr [esi]
        adc    cx, cx
        rep outsb dx, byte ptr [esi]
        pop    esi
        pop    edx
        pop    ecx
loc_4_184E:
        mov    cx, word ptr [ebx + 0x10]
        lea    edx, [ecx + 9]
loc_4_1855:
        in     al, dx
        test   al, 8
        jne    loc_4_1855
        lea    edx, [ecx + 8]
        mov    ax, word ptr [ebx + 0x31]
        or     al, 8
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_4_1870:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 0x10]
        add    dx, 7
        mov    al, 0x88
        out    dx, al
        mov    al, 1
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        sub    eax, eax
        mov    edi, dword ptr [ebp + 0xc]
        cmp    edi, 5
        ja     loc_4_1A5B
        jmp    dword ptr [edi*4 + 0x1a64]    ; -> obj4:0x1A64
        db  08Dh, 049h, 000h
loc_4_18B0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xb7]
        test   al, 0x40
        jne    loc_4_18CA
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_18CA:
        test   al, 8
        je     loc_4_18D8
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_18D8:
        push   esi
        push   0                             ; -> loc_4_0350
        add    esi, 0x515
        push   esi
        call   sub_1_089A
loc_4_18EA:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_18F4:
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_0730
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1905:
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0xb7]
        test   cl, 0x10
        je     loc_4_1920
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_4_1920:
        test   cl, 8
        je     loc_4_1932
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1932:
        test   cl, 0x40
        jne    loc_4_1940
        or     ecx, 8
        mov    dword ptr [esi + 0xb7], ecx
loc_4_1940:
        test   byte ptr [esi + 0xb7], 2
        jne    loc_4_1974
        push   esi
        push   0                             ; -> loc_4_0640
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_195B:
        mov    eax, dword ptr [esi + 0x61f]
        or     eax, eax
        je     loc_4_1974
        and    dword ptr [esi + 0xb7], 0xfffffff7
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1974:
        test   byte ptr [esi + 0xb7], 4
        jne    loc_4_1996
        push   esi
        call   sub_4_0730
        or     eax, eax
        je     loc_4_1996
        and    dword ptr [esi + 0xb7], 0xfffffff7
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1996:
        cmp    edi, 2
        jne    loc_4_1A5B
        mov    eax, dword ptr [esi + 0xb7]
        test   al, 0x20
        je     loc_4_19D0
        and    eax, 0xfffffff7
        mov    dword ptr [esi + 0xb7], eax
        lea    eax, [ebp - 1]
        push   eax
        add    esi, 0x64b
        push   esi
        call   sub_1_0888
loc_4_19C2:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_4_19D0:
        push   esi
        call   sub_4_0230
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_19E3:
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 1]
        and    dword ptr [esi + 0xb7], 0xffffffe9
        push   eax
        lea    eax, [esi + 0x64b]
        push   eax
        call   sub_1_0888
loc_4_19FD:
        push   0
        push   7
        push   esi
        call   sub_4_01C0
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1A14:
        mov    esi, dword ptr [ebp + 8]
        and    dword ptr [esi + 0xb7], 0xffffff61
        or     word ptr [esi + 0x619], 8
        mov    eax, dword ptr [esi + 0x4eb]
        mov    dword ptr [esi + 0x4e7], eax
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x64b]
        push   eax
        call   sub_1_0888
loc_4_1A45:
        mov    eax, dword ptr [esi + 0x549]
        or     eax, eax
        je     loc_4_1A59
        cmp    byte ptr [eax + 0xe], 2
        jne    loc_4_1A59
        mov    byte ptr [eax + 0xe], 3
loc_4_1A59:
        sub    eax, eax
loc_4_1A5B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
        ; ---- dispatch table (6 entries) ----
        dd  loc_4_18B0
        dd  loc_4_18F4
        dd  loc_4_1905
        dd  loc_4_1905
        dd  loc_4_19E3
        dd  loc_4_1A14
        db  08Dh, 064h, 024h, 000h
sub_4_1A80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x61], ecx
        test   byte ptr [eax + 0xb7], 0x10
        jne    loc_4_1A9B
        sub    eax, eax
        pop    ebp
        ret    0xc
loc_4_1A9B:
        push   ecx
        push   8
        push   eax
        call   sub_4_01C0
        or     al, al
        je     loc_4_1AB5
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0xc], 8
        mov    byte ptr [eax + 0xe], 2
loc_4_1AB5:
        mov    eax, 0x103
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_4_1AC0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x5d], ecx
        test   byte ptr [eax + 0xb7], 0x10
        jne    loc_4_1ADB
        sub    eax, eax
        pop    ebp
        ret    0xc
loc_4_1ADB:
        push   ecx
        push   9
        push   eax
        call   sub_4_01C0
        or     al, al
        je     loc_4_1AF5
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0xc], 9
        mov    byte ptr [eax + 0xe], 2
loc_4_1AF5:
        mov    eax, 0x103
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_4_1B00:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0xb7], 0x10
        jne    loc_4_1B15
        sub    eax, eax
        pop    ebp
        ret    8
loc_4_1B15:
        mov    ecx, dword ptr [eax + 0x81]
        push   ecx
        push   0xa
        push   eax
        call   sub_4_01C0
        or     al, al
        je     loc_4_1B35
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0xc], 0xa
        mov    byte ptr [eax + 0xe], 2
loc_4_1B35:
        mov    eax, 0x103
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_1B40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0xb7], 0x10
        jne    loc_4_1B55
        sub    eax, eax
        pop    ebp
        ret    8
loc_4_1B55:
        mov    ecx, dword ptr [ebp + 0xc]
        push   4
        mov    word ptr [ecx + 0xc], 7
        mov    byte ptr [ecx + 0xe], 2
        push   eax
        call   sub_4_1890
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_1B70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xb7], 0x10
        jne    loc_4_1B87
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0xc
loc_4_1B87:
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, eax
        shr    edx, 0x10
        mov    cx, ax
        and    ecx, 0xff
        shl    ecx, 0x10
        mov    cl, dl
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0xffff
        shl    eax, 0x10
        shr    dx, 8
        or     ecx, eax
        mov    eax, edx
        and    eax, 0xffff
        or     ecx, eax
        push   ecx
        push   0xd
        push   esi
        call   sub_4_01C0
        or     al, al
        je     loc_4_1BD5
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0xc], 0xd
        mov    byte ptr [eax + 0xe], 2
loc_4_1BD5:
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0xc
        db  090h
sub_4_1BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    eax, dword ptr [esi + 0x85]
        push   eax
        mov    eax, dword ptr [esi + 0x89]
        push   eax
        call   sub_1_08AC
loc_4_1BFF:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_4_1C1C
        mov    eax, dword ptr [esi + 0x2a9]
        or     eax, eax
        je     loc_4_1C1C
        push   0
        push   0x2000
        push   eax
        call   sub_1_08AC
loc_4_1C1C:
        mov    eax, dword ptr [esi + 0x645]
        cmp    eax, 6
        ja     loc_4_1C90
        jmp    dword ptr [eax*4 + 0x1c98]    ; -> obj4:0x1C98
        db  08Bh, 0FFh
loc_4_1C30:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 0x62d]
        push   eax
        call   sub_1_090C
loc_4_1C40:
        push   0
        push   0x20
        mov    eax, dword ptr [esi + 0x67f]
        push   eax
        call   sub_1_08AC
loc_4_1C50:
        mov    eax, dword ptr [esi + 0x635]
        push   eax
        call   sub_1_08EE
loc_4_1C5C:
        mov    eax, dword ptr [esi + 0x631]
        push   eax
        call   sub_1_08DC
loc_4_1C68:
        push   0
        push   0x4b
        mov    eax, dword ptr [esi + 0x639]
        push   eax
        call   sub_1_08AC
loc_4_1C78:
        mov    eax, dword ptr [esi + 0x62d]
        push   eax
        call   sub_1_08CA
loc_4_1C84:
        mov    eax, dword ptr [esi + 0x629]
        push   eax
        call   sub_1_08B8
loc_4_1C90:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
        ; ---- dispatch table (7 entries) ----
        dd  loc_4_1C84
        dd  loc_4_1C78
        dd  loc_4_1C68
        dd  loc_4_1C5C
        dd  loc_4_1C50
        dd  loc_4_1C40
        dd  loc_4_1C30
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1CC0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        pop    ebp
        mov    byte ptr [eax + 0x4e6], 1
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x531], 0
        jle    loc_4_1D03
        push   0x64
        add    esi, 0x4ce
        push   esi
        call   sub_1_0882
loc_4_1CFE:
        pop    esi
        pop    ebp
        ret    0x10
loc_4_1D03:
        push   esi
        call   sub_4_0BF0
        mov    eax, dword ptr [esi + 0xb7]
        test   al, 0x10
        je     loc_4_1D99
        test   dword ptr [esi + 0x59], 0x8020
        je     loc_4_1D7D
        test   ah, 2
        je     loc_4_1D7D
        test   ah, 4
        jne    loc_4_1D7D
        or     eax, 0x400
        mov    dword ptr [esi + 0xb7], eax
        push   0x80000025
        push   0x8100000f
        push   2
        push   0x8100000f
        mov    eax, dword ptr [esi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_1D52:
        add    esp, 0x14
        xor    eax, eax
        mov    ax, word ptr [esi + 0x649]
        mov    dword ptr [esi + 0x667], 0xb
        mov    dword ptr [esi + 0x65f], eax
        push   0
        lea    eax, [esi + 0x657]
        push   eax
        push   esi
        call   sub_4_3530
loc_4_1D7D:
        cmp    word ptr [esi + 0x619], 0
        je     loc_4_1D99
        push   esi
        push   0                             ; -> loc_4_1870
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_1D99:
        push   0x64
        add    esi, 0x4ce
        push   esi
        call   sub_1_0882
loc_4_1DA7:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h
sub_4_1DB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        test   dword ptr [esi + 0x59], 0x8020
        je     loc_4_1E0D
        test   byte ptr [esi + 0xb7], 0x10
        je     loc_4_1DFC
        mov    ecx, dword ptr [esi + 0x639]
        mov    ax, word ptr [esi + 0x641]
        mov    word ptr [ecx + 0x1f], ax
        mov    ecx, dword ptr [esi + 0x639]
        mov    eax, dword ptr [esi + 0x63d]
        mov    dword ptr [ecx + 0x1b], eax
        mov    eax, dword ptr [esi + 0x62d]
        push   eax
        mov    eax, dword ptr [esi + 0x67f]
        push   eax
        call   sub_4_4500
loc_4_1DFC:
        push   0xfa00
        add    esi, 0x64b
        push   esi
        call   sub_1_0882
loc_4_1E0D:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1E20:
        in     al, dx
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1E30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 0x10]
        add    dx, 9
        mov    al, 8
        out    dx, al
        mov    al, 1
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_1E50:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0x10]
        lea    edx, [edi + 6]
        in     al, dx
        mov    cl, al
        test   cl, 0x80
        je     loc_4_1EA6
        sub    al, al
        out    dx, al
        and    cl, 0xf
        mov    byte ptr [esi + 0x33], cl
        cmp    cl, 0xc
        jne    loc_4_1E96
        test   byte ptr [esi + 0xb7], 0x48
        je     loc_4_1E90
        mov    dx, di
        add    dx, 7
        mov    al, 0xa1
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_1E90:
        push   esi
        call   sub_4_4960
loc_4_1E96:
        mov    byte ptr [esi + 0x29], 1
        mov    byte ptr [esi + 0x34], 0xa0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_1EA6:
        lea    edx, [edi + 8]
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 9]
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        shl    dx, 8
        mov    al, cl
        and    eax, 0xffff00ff
        or     dx, ax
        mov    word ptr [esi + 0x31], dx
        and    dh, 0xb
        cmp    dh, 3
        jne    loc_4_1EE4
        push   esi
        call   sub_4_15E0
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_1EE4:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_4_1EF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    di, word ptr [esi + 0x10]
        lea    edx, [edi + 8]
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 9]
        in     al, dx
        mov    dl, al
        and    edx, 0xffff00ff
        shl    dx, 8
        mov    al, cl
        and    eax, 0xffff00ff
        or     dx, ax
        and    dh, 0xb
        cmp    dh, 3
        jne    loc_4_1F36
        push   esi
        push   0                             ; -> loc_4_1E30
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_1F36:
        cmp    byte ptr [esi + 0x29], 0
        je     loc_4_1F69
        mov    byte ptr [esi + 0x29], 0
        cmp    byte ptr [esi + 0x33], 0xc
        push   esi
        jne    loc_4_1F52
        call   sub_4_4B70
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_4_1F52:
        call   sub_4_1520
        push   esi
        push   0                             ; -> loc_4_0000
        add    esi, 0x515
        push   esi
        call   sub_1_089A
loc_4_1F69:
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
sub_4_1F70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x1c], 1
        sub    edi, edi
        mov    dword ptr [ebp - 8], 0
        mov    word ptr [ebp - 2], 0
        mov    dword ptr [ebp - 0xc], edi
        lea    ecx, [ebp - 8]
        mov    esi, 4
        mov    edx, dword ptr [ebp + 0x10]
        mov    ebx, edx
        and    ebx, 0xffff0000
        cmp    ebx, 0x2020000
        ja     loc_4_1FE0
        je     loc_4_2470
        cmp    ebx, 0x10000
        je     loc_4_2002
        cmp    ebx, 0x20000
        je     loc_4_2213
        cmp    ebx, 0x2010000
        je     loc_4_22C0
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
        db  08Dh, 049h, 000h
loc_4_1FE0:
        cmp    ebx, 0xff010000
        je     loc_4_24F7
        cmp    ebx, 0xff020000
        je     loc_4_2470
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
loc_4_2002:
        lea    eax, [edx - 0x10101]
        cmp    eax, 0x12
        ja     loc_4_2014
        jmp    dword ptr [eax*4 + 0x638]     ; -> obj4:0x25A8
loc_4_2014:
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
        db  08Bh, 0FFh
loc_4_2020:
        cmp    byte ptr [ebp + 0x24], 0
        jne    loc_4_2035
        mov    ecx, 0x110                    ; -> loc_1_0DBC
        mov    esi, 0x5c
        jmp    loc_4_256F
loc_4_2035:
        mov    ecx, 0x24                     ; -> loc_1_0CD0
        mov    esi, 0xec
        jmp    loc_4_256F
loc_4_2044:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xb7]
        test   al, 0x20
        je     loc_4_205A
        mov    dword ptr [ebp - 0xc], 4
        jmp    loc_4_208A
loc_4_205A:
        test   al, 0x40
        je     loc_4_2067
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_4_208A
loc_4_2067:
        test   al, 4
        jne    loc_4_2078
        test   al, 8
        je     loc_4_2078
        mov    dword ptr [ebp - 0xc], 1
        jmp    loc_4_208A
loc_4_2078:
        test   al, 0x10
        mov    dword ptr [ebp - 0xc], 4
        je     loc_4_208A
        mov    dword ptr [ebp - 0xc], 0
loc_4_208A:
        lea    ecx, [ebp - 0xc]
        mov    esi, 4
        jmp    loc_4_256F
loc_4_2097:
        lea    ecx, [ebp - 0x1c]
        mov    esi, 4
        jmp    loc_4_256F
loc_4_20A4:
        mov    dword ptr [ebp - 8], 0xf0
        jmp    loc_4_256F
loc_4_20B0:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x2d5]
        mov    dword ptr [ebp - 0x10], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x2d7]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x10]
        imul   eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebx + 0x3a]
        cmp    dword ptr [ebp - 0x14], eax
        jae    loc_4_20E3
        mov    eax, dword ptr [ebp - 0x14]
loc_4_20E3:
        mov    dword ptr [ebp - 8], eax
        cmp    edx, 0x10106
        jne    loc_4_256F
        sub    dword ptr [ebp - 8], 0xe
        jmp    loc_4_256F
        db  005h, 000h, 000h, 000h, 000h
loc_4_2100:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x35]
        jmp    loc_4_256C
        db  005h, 000h, 000h, 000h, 000h
loc_4_2110:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xd2]
        jmp    loc_4_256C
        db  08Bh, 0FFh
loc_4_2120:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x2d7]
        mov    dx, word ptr [ebx + 0x2d5]
        imul   edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    loc_4_256F
loc_4_2140:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xce]
        jmp    loc_4_256C
        db  08Bh, 0FFh
loc_4_2150:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x2d5]
        jmp    loc_4_256C
loc_4_2161:
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x4a
        mov    esi, 4
        jmp    loc_4_256F
loc_4_2171:
        mov    ecx, 0x16c                    ; -> loc_1_0E18
        mov    esi, 0x1f
        jmp    loc_4_256F
loc_4_2180:
        cmp    byte ptr [ebp + 0x24], 0
        je     loc_4_2191
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x59]
        jmp    loc_4_256C
loc_4_2191:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_4_256C
        db  090h
loc_4_21A0:
        cmp    byte ptr [ebp + 0x24], 0
        jne    loc_4_21B8
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 8], eax
        sub    dword ptr [ebp - 8], 0x20
        jmp    loc_4_256F
loc_4_21B8:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, ebx
        mov    edx, dword ptr [ebx + 0x53d]
        mov    dword ptr [ebp - 8], 0x20
        sub    eax, edx
        cmp    eax, 0xfffffac3
        je     loc_4_21EB
loc_4_21D3:
        mov    eax, dword ptr [edx + 0x1c]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_4_21DE
        mov    dword ptr [ebp - 8], eax
loc_4_21DE:
        mov    edx, dword ptr [edx]
        mov    eax, ebx
        sub    eax, edx
        cmp    eax, 0xfffffac3
        jne    loc_4_21D3
loc_4_21EB:
        sub    dword ptr [ebp - 8], 0x20
        jmp    loc_4_256F
loc_4_21F4:
        mov    word ptr [ebp - 2], 0x300
        lea    ecx, [ebp - 2]
        mov    esi, 2
        jmp    loc_4_256F
loc_4_2207:
        mov    dword ptr [ebp - 8], 7
        jmp    loc_4_256F
loc_4_2213:
        cmp    byte ptr [ebp + 0x24], 0
        je     loc_4_22B0
        mov    eax, edx
        and    eax, 0xff
        dec    eax
        and    edx, 0xff00
        cmp    edx, 0x100
        je     loc_4_2245
        cmp    edx, 0x200
        je     loc_4_2263
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
loc_4_2245:
        cmp    eax, 5
        jb     loc_4_2254
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
loc_4_2254:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 0x42a]
        jmp    loc_4_256C
loc_4_2263:
        cmp    eax, 0xe
        jb     loc_4_2272
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
loc_4_2272:
        mov    edx, eax
        and    edx, 0xfffffffe
        cmp    edx, 0xc
        jae    loc_4_22A1
        test   al, 1
        mov    ebx, dword ptr [ebp + 8]
        je     loc_4_2290
        mov    eax, dword ptr [ebx + edx*2 + 0x46e]
        jmp    loc_4_256C
        db  090h
loc_4_2290:
        lea    ecx, [ebx + edx*4 + 0x43e]
        mov    esi, 8
        jmp    loc_4_256F
loc_4_22A1:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 0x456]
        jmp    loc_4_256C
loc_4_22B0:
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_22C0:
        sub    edx, 0x2010101
        cmp    edx, 6
        ja     loc_4_22D2
        jmp    dword ptr [edx*4 + 0x684]     ; -> obj4:0x25F4
loc_4_22D2:
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
        db  08Dh, 064h, 024h, 000h
loc_4_22E0:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x4a]
        add    ebx, 0x4a
        mov    byte ptr [ebp - 0x24], al
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 0x23], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 0x22], al
        lea    ecx, [ebp - 0x24]
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [ebp - 0x21], al
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 0x20], al
        mov    esi, 6
        mov    al, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 0x1f], al
        jmp    loc_4_256F
loc_4_2317:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x50]
        add    ebx, 0x50
        mov    byte ptr [ebp - 0x24], al
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 0x23], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 0x22], al
        lea    ecx, [ebp - 0x24]
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [ebp - 0x21], al
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 0x20], al
        mov    esi, 6
        mov    al, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 0x1f], al
        jmp    loc_4_256F
        db  08Bh, 0FFh
loc_4_2350:
        cmp    byte ptr [ebp + 0x24], 0
        jne    loc_4_2361
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_4_2367
loc_4_2361:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x5d]
loc_4_2367:
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0xff00
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xff0000
        mov    ebx, dword ptr [ebp - 8]
        shr    ebx, 0x10
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    loc_4_256F
        db  005h, 000h, 000h, 000h, 000h
loc_4_23A0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x61]
        mov    dword ptr [ebp - 8], eax
        mov    edx, eax
        and    edx, 0xff00
        shl    eax, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xff0000
        mov    ebx, dword ptr [ebp - 8]
        shr    ebx, 0x10
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    loc_4_256F
loc_4_23D6:
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0xb5
        mov    esi, 2
        jmp    loc_4_256F
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_23F0:
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0xad
        mov    esi, 2
        jmp    loc_4_256F
loc_4_2403:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xb7]
        test   al, 0x10
        je     loc_4_241C
        mov    dword ptr [ebp - 8], 1
        jmp    loc_4_256F
loc_4_241C:
        test   al, 8
        je     loc_4_2430
        mov    dword ptr [ebp - 8], 3
        jmp    loc_4_256F
        db  08Dh, 064h, 024h, 000h
loc_4_2430:
        cmp    word ptr [ebx + 0xb5], 0
        je     loc_4_2446
        mov    dword ptr [ebp - 8], 5
        jmp    loc_4_256F
loc_4_2446:
        test   byte ptr [ebx + 0xac], 0xd
        je     loc_4_2460
        mov    dword ptr [ebp - 8], 6
        jmp    loc_4_256F
        db  005h, 000h, 000h, 000h, 000h
loc_4_2460:
        mov    dword ptr [ebp - 8], 2
        jmp    loc_4_256F
        db  08Dh, 064h, 024h, 000h
loc_4_2470:
        cmp    byte ptr [ebp + 0x24], 0
        je     loc_4_24F0
        mov    eax, edx
        and    eax, 0xff
        dec    eax
        and    edx, 0xff00
        cmp    edx, 0x100
        je     loc_4_24A0
        cmp    edx, 0x200
        je     loc_4_24C0
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
        db  08Bh, 0FFh
loc_4_24A0:
        cmp    eax, 2
        jb     loc_4_24B0
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
        db  090h
loc_4_24B0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 0x48e]
        jmp    loc_4_256C
        db  090h
loc_4_24C0:
        cmp    ebx, 0xff020000
        jne    loc_4_24CB
        add    eax, 7
loc_4_24CB:
        cmp    eax, 9
        jb     loc_4_24E0
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_24E0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 0x496]
        jmp    loc_4_256C
        db  090h
loc_4_24F0:
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
loc_4_24F7:
        cmp    byte ptr [ebp + 0x24], 0
        jne    loc_4_2504
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
loc_4_2504:
        cmp    edx, 0xff010101
        je     loc_4_2533
        cmp    edx, 0xff010102
        je     loc_4_2540
        cmp    edx, 0xff010103
        je     loc_4_2548
        cmp    edx, 0xff010104
        je     loc_4_2551
        cmp    edx, 0xff010105
        je     loc_4_2560
        mov    edi, 0xc00000bb
        jmp    loc_4_256F
loc_4_2533:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        jmp    loc_4_256C
        db  08Bh, 0FFh
loc_4_2540:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x20]
        jmp    loc_4_256C
loc_4_2548:
        mov    dword ptr [ebp - 8], 0xffff
        jmp    loc_4_256F
loc_4_2551:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x539]
        jmp    loc_4_256C
        db  08Dh, 064h, 024h, 000h
loc_4_2560:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xb1]
loc_4_256C:
        mov    dword ptr [ebp - 8], eax
loc_4_256F:
        or     edi, edi
        jne    loc_4_259D
        cmp    esi, dword ptr [ebp + 0x18]
        jbe    loc_4_258D
        mov    eax, dword ptr [ebp + 0x20]
        mov    edi, 0xc0010014
        mov    dword ptr [eax], esi
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_4_258D:
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_098C
loc_4_2598:
        mov    eax, dword ptr [ebp + 0x1c]
        add    dword ptr [eax], esi
loc_4_259D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        ; ---- dispatch table (26 entries) ----
        dd  loc_4_2020
        dd  loc_4_2044
        dd  loc_4_2097
        dd  loc_4_2097
        dd  loc_4_20A4
        dd  loc_4_20B0
        dd  loc_4_2100
        dd  loc_4_2110
        dd  loc_4_2120
        dd  loc_4_2140
        dd  loc_4_2150
        dd  loc_4_2161
        dd  loc_4_2171
        dd  loc_4_2180
        dd  loc_4_21A0
        dd  loc_4_21F4
        dd  loc_4_20B0
        dd  loc_4_2014
        dd  loc_4_2207
        dd  loc_4_22E0
        dd  loc_4_2317
        dd  loc_4_2350
        dd  loc_4_23A0
        dd  loc_4_23D6
        dd  loc_4_23F0
        dd  loc_4_2403
sub_4_2610:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esi + 0x14]
        cmp    edi, 4
        je     loc_4_2644
        mov    ebx, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_4_2748
loc_4_2644:
        cmp    ecx, 0x1010e
        je     loc_4_2690
        cmp    ecx, 0x1010f
        je     loc_4_26D0
        cmp    ecx, 0x10112
        je     loc_4_2748
        cmp    ecx, 0x2010103
        je     loc_4_2710
        cmp    ecx, 0x2010104
        je     loc_4_272C
        mov    ebx, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_4_2748
loc_4_2690:
        push   4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_098C
loc_4_269C:
        test   dword ptr [ebp - 8], 0x2056
        je     loc_4_26B0
        mov    ebx, 0xc00000bb
        jmp    loc_4_2748
        db  090h
loc_4_26B0:
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 0x545]
        push   eax
        call   sub_2_2690
loc_4_26CD:
        jmp    loc_4_2746
        db  090h
loc_4_26D0:
        push   4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_098C
loc_4_26DC:
        cmp    dword ptr [ebp - 4], 0xf0
        ja     loc_4_2708
        add    dword ptr [ebp - 4], 0x20
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 0x535], eax
        jae    loc_4_2748
        mov    dword ptr [ecx + 0x535], eax
        jmp    loc_4_2748
loc_4_2708:
        mov    ebx, 0xc0010014
        jmp    loc_4_2748
        db  090h
loc_4_2710:
        push   1
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 0x545]
        push   eax
        call   sub_2_2460
loc_4_272A:
        jmp    loc_4_2746
loc_4_272C:
        push   1
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 0x545]
        push   eax
        call   sub_2_2500
loc_4_2746:
        mov    ebx, eax
loc_4_2748:
        or     ebx, ebx
        jne    loc_4_2756
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], 0
loc_4_2756:
        cmp    ebx, 0x103
        je     loc_4_2768
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_4_28A0
loc_4_2768:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2780:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    edi, dword ptr [esi + 0x14]
        mov    dl, byte ptr [ebp + 0x14]
        or     dl, dl
        je     loc_4_27E0
        cmp    byte ptr [ebp + 0x18], 0
        je     loc_4_27E0
        mov    ebx, edi
        and    ebx, 0xffff0000
        cmp    ebx, 0x2020000
        je     loc_4_27C8
        cmp    ebx, 0xff020000
        jne    loc_4_27E0
loc_4_27C8:
        push   esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_4_1B00
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_27E0:
        push   edx
        lea    edx, [ebp - 4]
        push   edx
        lea    edx, [ebp - 8]
        push   edx
        push   eax
        push   ecx
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_4_1F70
        mov    ebx, eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], eax
        cmp    byte ptr [ebp + 0x18], 0
        je     loc_4_2814
        push   ebx
        push   edi
        call   sub_4_28A0
loc_4_2814:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
sub_4_2820:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        jne    loc_4_2845
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x10], 2
        jne    loc_4_2845
        push   0
        push   1
        push   ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_2780
loc_4_2845:
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2850:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ecx + 0x10]
        or     eax, eax
        jl     loc_4_2894
        cmp    eax, 1
        jle    loc_4_286B
        cmp    eax, 2
        je     loc_4_2880
        pop    ebp
        ret    0x10
loc_4_286B:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_07FC
loc_4_287C:
        pop    ebp
        ret    0x10
loc_4_2880:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        mov    eax, dword ptr [eax + 0x4ff]
        push   eax
        call   sub_1_081A
loc_4_2894:
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_28A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x549]
        mov    byte ptr [ecx + 0xe], 0
        mov    eax, dword ptr [ecx]
        or     eax, eax
        mov    dword ptr [esi + 0x549], eax
        jne    loc_4_28D1
        mov    dword ptr [esi + 0x54d], 0
        and    word ptr [esi + 0x619], 0xfffd
loc_4_28D1:
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        push   edi
        push   esi
        call   sub_4_2850
        or     edi, edi
        je     loc_4_28E7
        dec    dword ptr [edi + 0x14]
loc_4_28E7:
        or     word ptr [esi + 0x619], 0x10
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2900:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x549]
        or     eax, eax
        je     loc_4_2943
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, 2
loc_4_291B:
        cmp    dword ptr [eax + 0x10], ebx
        jne    loc_4_2930
        push   0
        push   1
        push   eax
        push   0
        push   esi
        call   sub_4_2780
        jmp    loc_4_2932
        db  090h
loc_4_2930:
        mov    eax, edi
loc_4_2932:
        push   eax
        push   esi
        call   sub_4_28A0
        mov    eax, dword ptr [esi + 0x549]
        or     eax, eax
        jne    loc_4_291B
loc_4_2943:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2950:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x549]
        mov    al, byte ptr [ecx + 0xe]
        cmp    al, 3
        jne    loc_4_2984
        mov    eax, dword ptr [esi + 0x61b]
        push   eax
        push   ecx
        push   esi
        call   sub_4_2820
        mov    eax, dword ptr [esi + 0x61b]
        push   eax
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_2984:
        cmp    al, 1
        jne    loc_4_2A61
        test   byte ptr [esi + 0xb7], 8
        jne    loc_4_2A61
        mov    eax, dword ptr [ecx + 0x10]
        cmp    eax, 0xb
        ja     loc_4_2A61
        jmp    dword ptr [eax*4 + 0xaf8]     ; -> obj4:0x2A68
loc_4_29AC:
        push   1
        push   0
        push   ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        push   esi
        call   sub_4_2780
        pop    esi
        pop    ebp
        ret    4
loc_4_29C0:
        push   ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        push   esi
        call   sub_4_2610
        pop    esi
        pop    ebp
        ret    4
loc_4_29D0:
        push   1
        push   1
        push   ecx
        push   0
        push   esi
        call   sub_4_2780
        pop    esi
        pop    ebp
        ret    4
loc_4_29E2:
        push   ecx
        push   esi
        call   sub_4_1B40
        or     eax, eax
        jne    loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_29FA:
        mov    eax, dword ptr [ecx + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1AC0
        or     eax, eax
        jne    loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_2A16:
        mov    eax, dword ptr [ecx + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1A80
        or     eax, eax
        jne    loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_2A32:
        push   ecx
        push   esi
        call   sub_4_1B00
        or     eax, eax
        jne    loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_2A4A:
        mov    eax, dword ptr [ecx + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1B70
        or     eax, eax
        jne    loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
loc_4_2A61:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
        ; ---- dispatch table (12 entries) ----
        dd  loc_4_29AC
        dd  loc_4_29C0
        dd  loc_4_29D0
        dd  loc_4_2A61
        dd  loc_4_29E2
        dd  loc_4_2A61
        dd  loc_4_2A61
        dd  loc_4_2A61
        dd  loc_4_29FA
        dd  loc_4_2A16
        dd  loc_4_2A32
        dd  loc_4_2A4A
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2AA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_4_2AAC:
        mov    byte ptr [ebp - 1], 1
        and    word ptr [esi + 0x619], 0xfffb
        mov    edi, dword ptr [esi + 0x53d]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xfffffac3
        je     loc_4_2B7E
        sub    ebx, ebx
loc_4_2AD0:
        test   byte ptr [edi + 0x18], 4
        je     loc_4_2B3C
        or     word ptr [esi + 0x619], 4
        mov    eax, dword ptr [edi + 0x18]
        test   al, 8
        je     loc_4_2B17
        cmp    byte ptr [esi + 0x55f], bl
        jne    loc_4_2B17
        cmp    byte ptr [esi + 0x587], bl
        jne    loc_4_2B17
        cmp    byte ptr [esi + 0x5af], bl
        jne    loc_4_2B17
        and    eax, 0xfffffff7
        mov    dword ptr [edi + 0x18], eax
        push   ebx
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x545]
        push   eax
        call   sub_2_2370
loc_4_2B14:
        dec    dword ptr [edi + 0x14]
loc_4_2B17:
        cmp    dword ptr [edi + 0x14], ebx
        jne    loc_4_2B3C
        cmp    dword ptr [esi + 0x539], 1
        ja     loc_4_2B50
        test   byte ptr [esi + 0xb7], 0x10
        je     loc_4_2B50
        push   edi
        lea    eax, [esi + 0x5f1]
        push   eax
        push   esi
        call   sub_4_3530
loc_4_2B3C:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xfffffac3
        jne    loc_4_2AD0
        jmp    loc_4_2B7E
        db  005h, 000h, 000h, 000h, 000h
loc_4_2B50:
        dec    dword ptr [esi + 0x539]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [edi + 0xc]
        push   eax
        call   sub_1_07E4
loc_4_2B70:
        push   0
        push   0x20
        push   edi
        call   sub_1_08AC
loc_4_2B7A:
        mov    byte ptr [ebp - 1], 0
loc_4_2B7E:
        cmp    byte ptr [ebp - 1], 0
        je     loc_4_2AAC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2BA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
        test   byte ptr [esi + 0xb7], 0x20
        je     loc_4_2BD4
        mov    dword ptr [ebp - 4], 0xc0010007
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0xb5]
        or     ebx, 0xc0011000
loc_4_2BD4:
        mov    edi, dword ptr [esi + 0x53d]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xfffffac3
        je     loc_4_2C44
loc_4_2BE5:
        mov    eax, dword ptr [edi + 0x18]
        test   al, 1
        je     loc_4_2C37
        and    eax, 0xfffffffe
        mov    dword ptr [edi + 0x18], eax
        or     eax, 2
        mov    dword ptr [edi + 0x18], eax
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [edi + 0xc]
        push   eax
        call   sub_1_07DE
loc_4_2C06:
        test   byte ptr [esi + 0xb7], 0x20
        je     loc_4_2C37
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   0x20
        push   edi
        call   sub_1_08AC
loc_4_2C28:
        mov    eax, dword ptr [esi + 0x539]
        dec    eax
        mov    dword ptr [esi + 0x539], eax
        je     loc_4_2C44
loc_4_2C37:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xfffffac3
        jne    loc_4_2BE5
loc_4_2C44:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_2C50:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0xc0000001
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_36B0
        push   esi
        call   sub_4_2BA0
        and    word ptr [esi + 0x619], 0xfff7
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2C80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0x80010004
        and    dword ptr [esi + 0xb7], 0xffffffbf
        mov    eax, dword ptr [esi + 0x625]
        and    word ptr [esi + 0x619], 0xfffe
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_07F6
loc_4_2CAB:
        or     word ptr [esi + 0x619], 0x18
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2CC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x623]
        cmp    eax, 7
        ja     loc_4_2D6F
        jmp    dword ptr [eax*4 + 0xe4c]     ; -> obj4:0x2DBC
loc_4_2CE0:
        mov    word ptr [esi + 0x623], 1
        mov    dword ptr [esi + 0x4eb], 0xa
        mov    dword ptr [esi + 0x4e7], 0xa
loc_4_2CFD:
        push   0
        push   esi
        call   sub_4_1890
        or     eax, eax
        je     loc_4_2D74
        push   esi
        call   sub_4_2C80
        pop    esi
        pop    ebp
        ret    4
loc_4_2D14:
        test   byte ptr [esi + 0xb7], 0x10
        jne    loc_4_2D30
        push   esi
        call   sub_4_2C80
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_2D30:
        mov    word ptr [esi + 0x623], 7
loc_4_2D39:
        push   0
        push   0
        push   0x40010005
        push   esi
        call   sub_4_34D0
        and    dword ptr [esi + 0xb7], 0xffffffbf
        and    word ptr [esi + 0x619], 0xfffe
        mov    eax, dword ptr [esi + 0x625]
        push   0
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_07F6
loc_4_2D69:
        push   esi
        call   sub_4_2BA0
loc_4_2D6F:
        pop    esi
        pop    ebp
        ret    4
loc_4_2D74:
        mov    word ptr [esi + 0x623], 2
loc_4_2D7D:
        push   0
        push   0
        push   0x40010004
        push   esi
        call   sub_4_34D0
        mov    word ptr [esi + 0x623], 3
loc_4_2D95:
        push   0xc001000c
        push   esi
        call   sub_4_36B0
        push   0xc001000c
        push   esi
        call   sub_4_2900
        mov    word ptr [esi + 0x623], 4
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
        ; ---- dispatch table (8 entries) ----
        dd  loc_4_2CE0
        dd  loc_4_2CFD
        dd  loc_4_2D7D
        dd  loc_4_2D95
        dd  loc_4_2D6F
        dd  loc_4_2D6F
        dd  loc_4_2D14
        dd  loc_4_2D39
        db  08Dh, 064h, 024h, 000h
loc_4_2DE0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    byte ptr [ebp + 0x18], 0
        mov    esi, dword ptr [ecx + 8]
        je     loc_4_2E03
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   esi
        call   sub_4_1AC0
        pop    esi
        pop    ebp
        ret    0x14
loc_4_2E03:
        lea    edx, [esi + 0x551]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 8], eax
        push   ecx
        push   edx
        push   esi
        call   sub_4_3530
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_4_2E20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    byte ptr [ebp + 0x18], 0
        mov    esi, dword ptr [ecx + 8]
        je     loc_4_2E43
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   esi
        call   sub_4_1A80
        pop    esi
        pop    ebp
        ret    0x14
loc_4_2E43:
        lea    edx, [esi + 0x579]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 8], eax
        push   ecx
        push   edx
        push   esi
        call   sub_4_3530
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_4_2E60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [esi + 8]
        test   eax, 0x8020
        jne    loc_4_2E7E
        and    dword ptr [edx + 0xb7], 0xfffffdff
loc_4_2E7E:
        test   byte ptr [edx + 0xb8], 2
        je     loc_4_2E8C
        and    eax, 0xffff7fdf
loc_4_2E8C:
        mov    ecx, dword ptr [edx + 0x59]
        xor    ecx, eax
        test   ecx, 0x8020
        mov    dword ptr [edx + 0x59], eax
        je     loc_4_2EEB
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0x649]
        test   al, 0x20
        je     loc_4_2EAF
        or     ecx, 0x200
loc_4_2EAF:
        test   ah, 0x80
        je     loc_4_2EBA
        or     ecx, 0x400
loc_4_2EBA:
        cmp    byte ptr [ebp + 0x18], 0
        je     loc_4_2ED0
        push   ecx
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   edx
        call   sub_4_1B70
        pop    esi
        pop    ebp
        ret    0x14
loc_4_2ED0:
        mov    dword ptr [edx + 0x5a9], ecx
        lea    eax, [edx + 0x5a1]
        push   esi
        push   eax
        push   edx
        call   sub_4_3530
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
loc_4_2EEB:
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2F00:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2F10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        mov    edi, dword ptr [ebx + 8]
        push   0
        push   ebx
        call   sub_4_32F0
        mov    esi, eax
        cmp    esi, 0x103
        jne    loc_4_2F55
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        je     loc_4_2F4D
        cmp    ecx, 1
        je     loc_4_2F4D
        mov    esi, 0xc00000bb
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_4_2F4D:
        push   ebx
        push   eax
        push   edi
        call   sub_4_3530
loc_4_2F55:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_4_2F60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0xb7], 0x10
        je     loc_4_2F93
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x10], 2
        jne    loc_4_2F8A
        push   0
        push   ecx
        push   eax
        call   sub_4_3530
        mov    eax, 0x103
        pop    ebp
        ret    8
loc_4_2F8A:
        mov    eax, 0xc00000bb
        pop    ebp
        ret    8
loc_4_2F93:
        push   0
        push   1
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   0
        push   eax
        call   sub_4_2780
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    dword ptr [esi], 0
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, edx
        je     loc_4_2FE1
        mov    eax, dword ptr [ebp + 0x14]
loc_4_2FD0:
        cmp    dword ptr [eax], 1
        je     loc_4_2FDD
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     loc_4_2FD0
loc_4_2FDD:
        cmp    ecx, edx
        jne    loc_4_2FF0
loc_4_2FE1:
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_4_2FF0:
        mov    eax, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edx
        test   byte ptr [edi + 0xb7], 0x20
        je     loc_4_3048
        xor    eax, eax
        mov    ax, word ptr [edi + 0xb5]
        push   eax
        push   0xc000001e
        push   0x81000008
        push   3
        push   0x81000008
        mov    eax, dword ptr [edi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_3028:
        add    esp, 0x18
        xor    eax, eax
        mov    ax, word ptr [edi + 0xb5]
        or     eax, 0xc0011000
        mov    dword ptr [esi], eax
        mov    eax, 0xc0010007
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_3048:
        mov    eax, dword ptr [0]            ; -> obj1:0x0AC4
        push   eax
        push   0
        push   0x20
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_08A6
loc_4_305B:
        or     eax, eax
        je     loc_4_3089
        push   0xc000000f
        push   6
        push   2
        push   0xaa000002
        mov    eax, dword ptr [edi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_3079:
        add    esp, 0x14
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_4_3089:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax + 0xc], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x10
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x545]
        push   eax
        call   sub_2_2290
loc_4_30E2:
        or     al, al
        jne    loc_4_3120
        push   0xc000001d
        push   6
        push   2
        push   0xaa000002
        mov    eax, dword ptr [edi + 0x4ff]
        push   eax
        call   sub_1_0978
loc_4_3100:
        add    esp, 0x14
        push   0
        push   0x20
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_08AC
loc_4_3110:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 049h, 000h
loc_4_3120:
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 0x18], 8
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 0xf0
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [edi + 0x53d]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 0x541]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edi + 0x541]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0x541], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        mov    eax, 0x103
        inc    dword ptr [edi + 0x539]
        test   byte ptr [edi + 0xb7], 0x40
        jne    loc_4_317E
        push   2
        push   edi
        call   sub_4_1890
loc_4_317E:
        cmp    eax, 0x103
        je     loc_4_3195
        mov    eax, dword ptr [ebp - 4]
        and    dword ptr [eax + 0x18], 0xfffffffe
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 0x18], 2
        sub    eax, eax
loc_4_3195:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 049h, 000h
loc_4_31A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 8]
        mov    eax, dword ptr [edx + 0x18]
        test   al, 4
        je     loc_4_31C0
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_31C0:
        and    eax, 0xfffffffd
        mov    dword ptr [edx + 0x18], eax
        or     eax, 4
        mov    dword ptr [edx + 0x18], eax
        mov    edx, dword ptr [ecx + 0x53d]
        mov    dword ptr [ecx + 0x535], 0xf0
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, 0xfffffac3
        je     loc_4_3211
        mov    esi, 3
loc_4_31EC:
        mov    eax, edx
        mov    edx, dword ptr [edx]
        test   dword ptr [eax + 0x18], esi
        je     loc_4_3206
        mov    eax, dword ptr [eax + 0x1c]
        cmp    dword ptr [ecx + 0x535], eax
        jae    loc_4_3206
        mov    dword ptr [ecx + 0x535], eax
loc_4_3206:
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, 0xfffffac3
        jne    loc_4_31EC
loc_4_3211:
        mov    dx, word ptr [ecx + 0x619]
        or     dx, 4
        mov    word ptr [ecx + 0x619], dx
        mov    eax, dword ptr [ecx + 0xb7]
        test   al, 4
        je     loc_4_3250
        test   eax, 0x2148
        jne    loc_4_3250
        push   ecx
        push   0                             ; -> loc_4_1870
        add    ecx, 0x515
        push   ecx
        call   sub_1_089A
loc_4_3246:
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
loc_4_3250:
        or     dx, 0x10
        mov    word ptr [ecx + 0x619], dx
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3270:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        mov    edi, dword ptr [ebx + 8]
        push   1
        push   ebx
        call   sub_4_32F0
        mov    esi, eax
        cmp    esi, 0x103
        jne    loc_4_32DF
        mov    dword ptr [edi + 0x625], ebx
        or     word ptr [edi + 0x619], 1
        mov    word ptr [edi + 0x623], 0
        mov    eax, dword ptr [edi + 0xb7]
        test   al, 4
        je     loc_4_32D0
        test   eax, 0x2148
        jne    loc_4_32D0
        push   edi
        push   0                             ; -> loc_4_1870
        lea    eax, [edi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_32CA:
        jmp    loc_4_32D8
        db  08Dh, 064h, 024h, 000h
loc_4_32D0:
        or     word ptr [edi + 0x619], 0x10
loc_4_32D8:
        or     dword ptr [edi + 0xb7], 0x40
loc_4_32DF:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_32F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x18]
        test   al, 2
        jne    loc_4_3320
        test   al, 4
        je     loc_4_3310
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0xc
        db  090h
loc_4_3310:
        mov    eax, 0xc0010011
        pop    esi
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_3320:
        mov    eax, 0x103
        mov    ecx, dword ptr [edx + 0xb7]
        test   cl, 0x40
        je     loc_4_3353
        cmp    byte ptr [ebp + 0xc], 0
        jne    loc_4_3349
        mov    cx, word ptr [edx + 0x623]
        cmp    cx, 2
        jbe    loc_4_3379
        cmp    cx, 7
        jae    loc_4_3379
loc_4_3349:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0xc
loc_4_3353:
        and    ecx, 0x10
        cmp    ecx, 1
        sbb    ecx, ecx
        neg    ecx
        mov    esi, dword ptr [ebp + 0x10]
        and    esi, 0xff
        test   esi, ecx
        je     loc_4_3379
        cmp    byte ptr [edx + 0xe6], 1
        cmc    
        sbb    eax, eax
        and    eax, 0xc0000001
loc_4_3379:
        pop    esi
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_4_3380:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 0xb7], 0x100
        mov    ax, word ptr [esi + 0x619]
        or     ax, 0x10
        mov    word ptr [esi + 0x619], ax
        test   al, 0x10
        je     loc_4_33F6
loc_4_33A7:
        mov    ax, word ptr [esi + 0x619]
        and    ax, 0xffef
        mov    word ptr [esi + 0x619], ax
        test   al, 1
        je     loc_4_33C5
        push   esi
        call   sub_4_2CC0
        jmp    loc_4_33ED
loc_4_33C5:
        test   al, 8
        je     loc_4_33CF
        push   esi
        call   sub_4_2C50
loc_4_33CF:
        test   byte ptr [esi + 0x619], 2
        je     loc_4_33DE
        push   esi
        call   sub_4_2950
loc_4_33DE:
        test   byte ptr [esi + 0x619], 4
        je     loc_4_33ED
        push   esi
        call   sub_4_2AA0
loc_4_33ED:
        test   byte ptr [esi + 0x619], 0x10
        jne    loc_4_33A7
loc_4_33F6:
        and    dword ptr [esi + 0xb7], 0xfffffeff
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3410:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x5d7], 0
        jne    loc_4_342E
        push   0
        lea    ecx, [eax + 0x5c9]
        push   ecx
        push   eax
        call   sub_4_3530
loc_4_342E:
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3440:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 3
        mov    ecx, dword ptr [edx + 0x53d]
loc_4_3452:
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 0xfffffac3
        je     loc_4_346D
        mov    eax, ecx
        mov    ecx, dword ptr [ecx]
        test   dword ptr [eax + 0x18], esi
        je     loc_4_3452
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_4_346D:
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3480:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xb7], 0x40
        je     loc_4_34A8
        cmp    word ptr [esi + 0x623], 5
        jne    loc_4_34A8
        mov    word ptr [esi + 0x623], 6
        pop    esi
        pop    ebp
        ret    4
loc_4_34A8:
        push   esi
        call   sub_4_2BA0
        test   byte ptr [esi + 0xb7], 0x10
        jne    loc_4_34BF
        or     word ptr [esi + 0x619], 8
loc_4_34BF:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_34D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 0x53d]
        sub    eax, edi
        cmp    eax, 0xfffffac3
        je     loc_4_351C
        mov    esi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 0x10]
        mov    edi, edi
loc_4_34F0:
        test   byte ptr [edi + 0x18], 2
        je     loc_4_350E
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   ebx
        push   esi
        mov    eax, dword ptr [edi + 0xc]
        push   eax
        call   sub_1_080E
loc_4_3505:
        mov    eax, dword ptr [edi + 0xc]
        push   eax
        call   sub_1_0814
loc_4_350E:
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [ebp + 8]
        sub    eax, edi
        cmp    eax, 0xfffffac3
        jne    loc_4_34F0
loc_4_351C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3530:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 4], esi
        mov    byte ptr [eax + 0xe], 1
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        cmp    dword ptr [edx + 0x549], 0
        jne    loc_4_3560
        mov    dword ptr [edx + 0x549], eax
        jmp    loc_4_3568
        db  005h, 000h, 000h, 000h, 000h
loc_4_3560:
        mov    ecx, dword ptr [edx + 0x54d]
        mov    dword ptr [ecx], eax
loc_4_3568:
        mov    dword ptr [edx + 0x54d], eax
        mov    cx, word ptr [edx + 0x619]
        or     cx, 0x12
        mov    word ptr [edx + 0x619], cx
        mov    eax, dword ptr [edx + 0xb7]
        test   al, 4
        je     loc_4_35A5
        test   eax, 0x2148
        jne    loc_4_35A5
        push   edx
        push   0                             ; -> loc_4_1870
        add    edx, 0x515
        push   edx
        call   sub_1_089A
loc_4_35A3:
        jmp    loc_4_35B0
loc_4_35A5:
        or     cx, 0x10
        mov    word ptr [edx + 0x619], cx
loc_4_35B0:
        or     esi, esi
        je     loc_4_35B7
        inc    dword ptr [esi + 0x14]
loc_4_35B7:
        pop    esi
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_4_35C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [ebp + 0xc], 8
        jb     loc_4_3623
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 2]
        test   cl, 0x80
        je     loc_4_3623
        cmp    cl, 0xff
        jne    loc_4_35E2
        cmp    byte ptr [eax + 3], 0xff
        je     loc_4_35ED
loc_4_35E2:
        cmp    cl, 0xc0
        jne    loc_4_3609
        cmp    byte ptr [eax + 3], 0
        jne    loc_4_3609
loc_4_35ED:
        cmp    byte ptr [eax + 4], 0xff
        jne    loc_4_3609
        cmp    byte ptr [eax + 5], 0xff
        jne    loc_4_3609
        cmp    byte ptr [eax + 6], 0xff
        jne    loc_4_3609
        cmp    byte ptr [eax + 7], 0xff
        mov    byte ptr [ebp - 1], 1
        je     loc_4_360D
loc_4_3609:
        mov    byte ptr [ebp - 1], 0
loc_4_360D:
        cmp    byte ptr [ebp - 1], 0
        je     loc_4_361B
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_361B:
        mov    al, 2
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3623:
        sub    al, al
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_4_3630:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    ecx, 8
        mov    eax, dword ptr [ebp + 0xc]
        jb     loc_4_3648
        test   byte ptr [eax + 2], 0x80
        jne    loc_4_3648
        sub    eax, eax
        jmp    loc_4_3654
loc_4_3648:
        push   ecx
        push   eax
        call   sub_4_35C0
        and    eax, 0xff
loc_4_3654:
        mov    edx, dword ptr [ebp + 8]
        inc    dword ptr [edx + 0x42a]
        or     eax, eax
        jne    loc_4_3670
        lea    eax, [edx + 0x46e]
        add    edx, 0x43e
        jmp    loc_4_368F
        db  090h
loc_4_3670:
        cmp    eax, 1
        jne    loc_4_3683
        lea    eax, [edx + 0x476]
        add    edx, 0x44e
        jmp    loc_4_368F
loc_4_3683:
        lea    eax, [edx + 0x472]
        add    edx, 0x446
loc_4_368F:
        inc    dword ptr [eax]
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [ebp + 0x14]
        add    eax, ecx
        mov    dword ptr [edx], eax
        cmp    ecx, eax
        jbe    loc_4_36A1
        inc    dword ptr [edx + 4]
loc_4_36A1:
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_36B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0
        cmp    byte ptr [esi + 0xe6], 0
        mov    word ptr [esi + 0xe4], 0
        mov    dword ptr [esi + 0xde], 0
        je     loc_4_36DD
        mov    edi, dword ptr [ebp + 0xc]
loc_4_36DD:
        cmp    word ptr [esi + 0xe2], 0
        je     loc_4_3738
loc_4_36E7:
        mov    ecx, dword ptr [esi + 0xd6]
        dec    word ptr [esi + 0xe2]
        mov    ebx, dword ptr [ecx + 0x28]
        lea    eax, [ecx + 0x24]
        inc    dword ptr [esi + 0x432]
        cmp    dword ptr [esi + 0xda], ecx
        jne    loc_4_3712
        mov    dword ptr [esi + 0xda], 0
loc_4_3712:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0xd6], eax
        test   byte ptr [ebx + 0x19], 0x80
        jne    loc_4_372E
        push   edi
        push   ecx
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        call   sub_1_07EA
loc_4_372B:
        dec    dword ptr [ebx + 0x14]
loc_4_372E:
        cmp    word ptr [esi + 0xe2], 0
        jne    loc_4_36E7
loc_4_3738:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_4_3740:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0xeb]
        cmp    word ptr [esi + 0xe9], ax
        je     loc_4_3846
loc_4_3760:
        mov    edi, dword ptr [esi + 0xd6]
        cmp    byte ptr [esi + 0xe6], 0
        lea    eax, [edi + 0x24]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        je     loc_4_379B
        mov    ax, word ptr [esi + 0xeb]
        and    eax, 0x7f
        mov    al, byte ptr [esi + eax + 0x1f3]
        sub    al, 0xcc
        cmp    al, 1
        cmc    
        sbb    ebx, ebx
        and    ebx, 0x10001
        jmp    loc_4_379D
loc_4_379B:
        sub    ebx, ebx
loc_4_379D:
        push   0
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_08D0
loc_4_37AD:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_08F4
loc_4_37BE:
        or     ebx, ebx
        jne    loc_4_37E0
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_4_3630
        jmp    loc_4_37E6
        db  090h
loc_4_37E0:
        inc    dword ptr [esi + 0x432]
loc_4_37E6:
        cmp    dword ptr [esi + 0xda], edi
        jne    loc_4_37F8
        mov    dword ptr [esi + 0xda], 0
loc_4_37F8:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0xd6], eax
        dec    word ptr [esi + 0xe2]
        dec    word ptr [esi + 0xe4]
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [esi + 0xeb]
        test   byte ptr [eax + 0x19], 0x80
        jne    loc_4_3832
        push   ebx
        push   edi
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_07EA
loc_4_382C:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x14]
loc_4_3832:
        mov    ax, word ptr [esi + 0xeb]
        cmp    word ptr [esi + 0xe9], ax
        jne    loc_4_3760
loc_4_3846:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_3850:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    ecx, ecx
        mov    ax, word ptr [ebp + 0x10]
        cmp    ax, 0x15
        jb     loc_4_38C6
        cmp    ax, 0x25
        ja     loc_4_38C6
        mov    edx, dword ptr [ebp + 0xc]
        cmp    byte ptr [edx + 1], 0x40
        jne    loc_4_38C6
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x54]
        cmp    word ptr [edx + 6], ax
        jne    loc_4_38C6
        mov    eax, dword ptr [esi + 0x50]
        cmp    dword ptr [edx + 2], eax
        jne    loc_4_38C6
        test   byte ptr [edx + 8], 0x80
        je     loc_4_3890
        mov    cl, byte ptr [edx + 0xe]
        and    ecx, 0x1f
loc_4_3890:
        lea    eax, [ecx + edx]
        mov    cl, byte ptr [eax + 0xe]
        and    cl, 0xef
        cmp    cl, 0xe4
        jne    loc_4_38C6
        cmp    byte ptr [eax + 0x10], 3
        jne    loc_4_38C6
        cmp    byte ptr [eax + 0x11], 0
        jne    loc_4_38C6
        cmp    byte ptr [eax + 0x12], 4
        jne    loc_4_38C6
        cmp    byte ptr [eax + 0x13], 0x90
        jne    loc_4_38C6
        cmp    byte ptr [eax + 0x14], 7
        jne    loc_4_38C6
        or     dword ptr [esi + 0xb7], 0x200
loc_4_38C6:
        pop    esi
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_4_38D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    esp, [esp]
loc_4_38E0:
        cmp    word ptr [esi + 0x2dd], 0
        je     loc_4_3A42
        mov    bx, word ptr [esi + 0x426]
        lea    edi, [esi + 0x2e3]
        inc    dword ptr [esi + 0x42e]
        mov    al, byte ptr [edi + 2]
        test   al, 0x80
        je     loc_4_3970
        cmp    al, 0xff
        jne    loc_4_3912
        cmp    byte ptr [edi + 3], 0xff
        je     loc_4_391C
loc_4_3912:
        cmp    al, 0xc0
        jne    loc_4_3938
        cmp    byte ptr [edi + 3], 0
        jne    loc_4_3938
loc_4_391C:
        cmp    byte ptr [edi + 4], 0xff
        jne    loc_4_3938
        cmp    byte ptr [edi + 5], 0xff
        jne    loc_4_3938
        cmp    byte ptr [edi + 6], 0xff
        jne    loc_4_3938
        cmp    byte ptr [edi + 7], 0xff
        mov    byte ptr [ebp - 1], 1
        je     loc_4_393C
loc_4_3938:
        mov    byte ptr [ebp - 1], 0
loc_4_393C:
        cmp    byte ptr [ebp - 1], 0
        je     loc_4_3956
        lea    eax, [esi + 0x482]
        mov    dword ptr [ebp - 0xc], eax
        lea    edx, [esi + 0x466]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_4_3993
loc_4_3956:
        lea    eax, [esi + 0x47e]
        mov    dword ptr [ebp - 0xc], eax
        lea    edx, [esi + 0x45e]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_4_3993
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_3970:
        lea    eax, [esi + 0x47a]
        mov    dword ptr [ebp - 0xc], eax
        lea    edx, [esi + 0x456]
        mov    dword ptr [ebp - 8], edx
        test   dword ptr [esi + 0x59], 0x8020
        je     loc_4_3993
        push   ebx
        push   edi
        push   esi
        call   sub_4_3850
loc_4_3993:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, ebx
        inc    dword ptr [eax]
        mov    edx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    eax, dword ptr [edx]
        add    eax, ecx
        mov    dword ptr [edx], eax
        cmp    ecx, eax
        jbe    loc_4_39B0
        inc    dword ptr [edx + 4]
loc_4_39B0:
        mov    ax, 0xe
        test   byte ptr [edi + 8], 0x80
        je     loc_4_39C5
        mov    al, byte ptr [edi + 0xe]
        and    ax, 0x1f
        add    ax, 0xe
loc_4_39C5:
        mov    word ptr [esi + 0x428], ax
        sub    bx, ax
        mov    word ptr [esi + 0x426], bx
        mov    edx, dword ptr [esi + 0x535]
        cmp    edx, ecx
        jae    loc_4_39F0
        mov    ecx, eax
        and    ecx, 0xffff
        sub    edx, ecx
        jmp    loc_4_39F8
        db  08Dh, 064h, 024h, 000h
loc_4_39F0:
        mov    edx, ebx
        and    edx, 0xffff
loc_4_39F8:
        mov    ecx, eax
        and    ecx, 0xffff
        mov    eax, ebx
        and    eax, 0xffff
        push   eax
        push   edx
        lea    eax, [ecx + edi]
        push   eax
        push   ecx
        push   edi
        push   esi
        mov    eax, dword ptr [esi + 0x545]
        push   eax
        call   sub_1_0320
loc_4_3A1C:
        mov    eax, dword ptr [esi + 0x545]
        push   eax
        call   sub_1_0570
loc_4_3A28:
        push   esi
        push   0                             ; -> loc_4_4BE0
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_3A3A:
        or     al, al
        je     loc_4_38E0
loc_4_3A42:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_3A50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        lea    eax, [edi + 0x279]
        mov    si, word ptr [edi + 0x273]
        mov    word ptr [ebp - 0xc], si
        mov    dword ptr [ebp - 0x24], eax
        lea    edx, [edi + 0x2ad]
        mov    dword ptr [ebp - 0x28], edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0x20], ecx
        add    dword ptr [ebp - 0x24], 8
        mov    ax, word ptr [eax + 4]
        dec    word ptr [edi + 0x277]
        mov    word ptr [ebp - 8], ax
        mov    dword ptr [ebp - 0x30], ecx
        mov    word ptr [ebp - 0x18], ax
        mov    cx, word ptr [edi + 0xed]
        mov    ax, cx
        and    eax, 0x7f
        mov    bx, word ptr [edi + eax*2 + 0xf1]
        inc    cx
        mov    word ptr [edi + 0xed], cx
        mov    word ptr [ebp - 0xe], bx
        mov    ax, word ptr [edi + 0x10]
        add    ax, 0xc
        mov    word ptr [ebp - 0x16], ax
        mov    dx, word ptr [edi + 0x16]
        sub    al, al
        out    dx, al
        mov    dx, word ptr [ebp - 0x16]
        mov    ax, 0x10
        out    dx, ax
        sub    dx, 2
        sub    ax, ax
        out    dx, ax
        mov    dx, word ptr [edi + 0x10]
        in     al, dx
        mov    edx, dword ptr [ebp - 0x28]
        and    ecx, 0x7f
        mov    ax, word ptr [edx]
        mov    word ptr [ebp - 4], ax
        mov    ax, si
        sub    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [edi + ecx*2 + 0xf1]
        or     cx, 0x8000
        mov    ah, cl
        mov    al, ch
        mov    dx, word ptr [edi + 0x14]
        out    dx, ax
        mov    ax, 0x80
        out    dx, ax
        mov    ah, byte ptr [ebp - 4]
        mov    al, byte ptr [ebp - 3]
        out    dx, ax
        or     si, si
        je     loc_4_3DED
loc_4_3B30:
        cmp    word ptr [ebp - 8], 0
        jne    loc_4_3B4B
        mov    eax, dword ptr [ebp - 0x24]
        add    dword ptr [ebp - 0x24], 8
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0x20], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
loc_4_3B4B:
        mov    ax, word ptr [ebp - 4]
        mov    cx, word ptr [ebp - 8]
        cmp    ax, cx
        jb     loc_4_3B5B
        mov    ax, cx
loc_4_3B5B:
        mov    ecx, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x2c], ecx
        mov    word ptr [ebp - 0xa], ax
        mov    cx, word ptr [edi + 0x14]
        mov    word ptr [ebp - 0x10], cx
        push   ecx
        push   edx
        push   esi
        mov    dx, word ptr [ebp - 0x10]
        movzx  ecx, word ptr [ebp - 0xa]
        mov    esi, dword ptr [ebp - 0x2c]
        shr    cx, 2
        rep outsd dx, dword ptr [esi]
        mov    cx, word ptr [ebp - 0xa]
        and    cx, 3
        rep outsb dx, byte ptr [esi]
        pop    esi
        pop    edx
        pop    ecx
        mov    ecx, eax
        and    ecx, 0xffff
        add    dword ptr [ebp - 0x20], ecx
        sub    word ptr [ebp - 8], ax
        sub    word ptr [ebp - 0xc], ax
        sub    word ptr [ebp - 4], ax
        jne    loc_4_3D0A
        cmp    word ptr [ebp - 6], 2
        jbe    loc_4_3D1A
        add    dword ptr [ebp - 0x28], 4
        mov    edx, dword ptr [ebp - 0x28]
        mov    ax, word ptr [ebp - 6]
        cmp    word ptr [edx + 2], ax
        jae    loc_4_3BE0
        mov    ax, word ptr [edx]
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [ebp - 4]
        jmp    loc_4_3C15
loc_4_3BE0:
        cmp    word ptr [ebp - 6], 0x3f8
        jbe    loc_4_3BF8
        mov    cx, 0x396
        sub    word ptr [ebp - 6], 0x396
        mov    word ptr [ebp - 4], cx
        jmp    loc_4_3C15
loc_4_3BF8:
        mov    ax, word ptr [ebp - 6]
        dec    ax
        and    ax, 0xfffe
        mov    word ptr [ebp - 4], ax
        mov    cx, ax
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 6], ax
loc_4_3C15:
        mov    ah, cl
        mov    al, ch
        mov    dx, word ptr [edi + 0x14]
        out    dx, ax
        mov    dx, word ptr [edi + 0x16]
        sub    al, al
        out    dx, al
        mov    cx, word ptr [edi + 0x10]
        or     bx, 0x400
        mov    ax, bx
        shr    ax, 6
        lea    edx, [ecx + 0xc]
        out    dx, ax
        lea    esi, [ecx + 0xa]
        shl    bx, 0xa
        cmp    byte ptr [ebp - 1], 0
        mov    dx, si
        mov    ax, bx
        out    dx, ax
        mov    dx, cx
        in     al, dx
        je     loc_4_3CBD
        mov    byte ptr [ebp - 1], 0
        cmp    byte ptr [edi + 0xe6], 0
        jne    loc_4_3C6B
        cmp    word ptr [edi + 0x277], 0x40
        jae    loc_4_3C71
loc_4_3C6B:
        or     word ptr [ebp - 0xe], 0x8000
loc_4_3C71:
        lea    edx, [ecx + 0xc]
        mov    ax, 1
        out    dx, ax
        mov    bx, word ptr [edi + 0x1f1]
        mov    ax, bx
        and    ax, 0x7f
        add    ax, ax
        add    ax, 0x148e
        mov    dx, si
        out    dx, ax
        mov    dx, cx
        mov    ax, word ptr [ebp - 0xe]
        out    dx, ax
        inc    bx
        mov    word ptr [edi + 0x1f1], bx
        mov    dx, word ptr [edi + 0x10]
        add    dx, 4
        mov    ax, 0x1086
        out    dx, ax
        sub    dx, 4
        mov    ax, bx
        out    dx, ax
loc_4_3CBD:
        mov    cx, word ptr [edi + 0xed]
        dec    word ptr [edi + 0x277]
        mov    ax, cx
        and    eax, 0x7f
        mov    bx, word ptr [edi + eax*2 + 0xf1]
        inc    cx
        mov    word ptr [edi + 0xed], cx
        and    ecx, 0x7f
        mov    cx, word ptr [edi + ecx*2 + 0xf1]
        or     cx, 0x8000
        mov    ah, cl
        mov    al, ch
        mov    dx, word ptr [edi + 0x14]
        out    dx, ax
        mov    ax, 0x80
        out    dx, ax
        mov    ah, byte ptr [ebp - 4]
        mov    al, byte ptr [ebp - 3]
        out    dx, ax
loc_4_3D0A:
        cmp    word ptr [ebp - 0xc], 0
        jne    loc_4_3B30
        jmp    loc_4_3DED
loc_4_3D1A:
        mov    ah, byte ptr [ebp - 6]
        mov    al, byte ptr [ebp - 5]
        mov    dx, word ptr [edi + 0x14]
        out    dx, ax
        mov    ax, word ptr [ebp - 6]
        cmp    ax, word ptr [ebp - 8]
        jbe    loc_4_3D7B
        mov    ecx, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x38], ecx
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 0x14], ax
        mov    ax, word ptr [edi + 0x14]
        mov    word ptr [ebp - 0x1c], ax
        push   ecx
        push   edx
        push   esi
        mov    dx, word ptr [ebp - 0x1c]
        movzx  ecx, word ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0x38]
        shr    cx, 2
        rep outsd dx, dword ptr [esi]
        mov    cx, word ptr [ebp - 0x14]
        and    cx, 3
        rep outsb dx, byte ptr [esi]
        pop    esi
        pop    edx
        pop    ecx
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 6], ax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0x20], ecx
loc_4_3D7B:
        mov    ecx, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x34], ecx
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 0x12], ax
        mov    ax, word ptr [edi + 0x14]
        mov    word ptr [ebp - 0x1a], ax
        push   ecx
        push   edx
        push   esi
        mov    dx, word ptr [ebp - 0x1a]
        movzx  ecx, word ptr [ebp - 0x12]
        mov    esi, dword ptr [ebp - 0x34]
        shr    cx, 2
        rep outsd dx, dword ptr [esi]
        mov    cx, word ptr [ebp - 0x12]
        and    cx, 3
        rep outsb dx, byte ptr [esi]
        pop    esi
        pop    edx
        pop    ecx
        mov    dx, word ptr [edi + 0x16]
        sub    al, al
        out    dx, al
        mov    cx, word ptr [edi + 0x10]
        lea    edx, [ecx + 0xc]
        or     bx, 0x400
        mov    ax, bx
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        mov    ax, bx
        shl    ax, 0xa
        out    dx, ax
        mov    dx, cx
        in     al, dx
        inc    word ptr [edi + 0xed]
        dec    word ptr [edi + 0x277]
loc_4_3DED:
        mov    dx, word ptr [edi + 0x16]
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [edi + 0x10]
        add    dx, 0xc
        cmp    byte ptr [edi + 0xe6], 0
        mov    ax, 1
        out    dx, ax
        jne    loc_4_3E25
        mov    eax, dword ptr [edi + 0x273]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 0x30]
        push   eax
        push   edi
        call   sub_4_3630
loc_4_3E25:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    bx, word ptr [edi + 0x273]
        mov    cx, word ptr [edi + 0xed]
        mov    ax, cx
        and    eax, 0x7f
        mov    ax, word ptr [edi + eax*2 + 0xf1]
        dec    word ptr [edi + 0x277]
        inc    cx
        mov    word ptr [edi + 0xed], cx
        mov    cx, word ptr [edi + 0x10]
        mov    word ptr [ebp - 6], ax
        lea    esi, [ecx + 0xc]
        mov    dx, word ptr [edi + 0x16]
        sub    al, al
        out    dx, al
        mov    dx, si
        mov    ax, 0x10
        out    dx, ax
        lea    edx, [esi - 2]
        sub    ax, ax
        out    dx, ax
        mov    dx, cx
        in     al, dx
        mov    dx, word ptr [edi + 0x14]
        mov    ax, 0x40
        out    dx, ax
        mov    ax, 0x81
        out    dx, ax
        mov    cx, bx
        mov    ah, cl
        mov    al, ch
        out    dx, ax
        mov    ecx, dword ptr [edi + 0x279]
        lea    eax, [edi + 0x279]
        mov    dword ptr [ebp - 0x10], ecx
        mov    cx, word ptr [eax + 4]
        mov    word ptr [ebp - 8], cx
        or     bx, bx
        je     loc_4_3F05
loc_4_3EC4:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], ecx
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [ebp - 2], cx
        mov    cx, word ptr [edi + 0x14]
        mov    word ptr [ebp - 4], cx
        push   ecx
        push   edx
        push   esi
        mov    dx, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 2]
        mov    esi, dword ptr [ebp - 0xc]
        shr    cx, 2
        rep outsd dx, dword ptr [esi]
        mov    cx, word ptr [ebp - 2]
        and    cx, 3
        rep outsb dx, byte ptr [esi]
        pop    esi
        pop    edx
        pop    ecx
        sub    bx, word ptr [eax + 4]
        add    eax, 8
        or     bx, bx
        jne    loc_4_3EC4
loc_4_3F05:
        mov    dx, word ptr [edi + 0x16]
        sub    al, al
        mov    bx, word ptr [edi + 0x10]
        out    dx, al
        lea    esi, [ebx + 0xc]
        mov    cx, word ptr [ebp - 6]
        or     cx, 0x400
        mov    ax, cx
        shr    ax, 6
        mov    dx, si
        out    dx, ax
        mov    ax, cx
        shl    ax, 0xa
        lea    edx, [esi - 2]
        out    dx, ax
        mov    dx, bx
        in     al, dx
        cmp    byte ptr [edi + 0xe6], 0
        mov    dx, word ptr [edi + 0x16]
        mov    al, 1
        out    dx, al
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        jne    loc_4_3F5B
        cmp    word ptr [edi + 0x277], 0x40
        jae    loc_4_3F61
loc_4_3F5B:
        or     word ptr [ebp - 6], 0x8000
loc_4_3F61:
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        mov    bx, word ptr [edi + 0x1f1]
        mov    ax, bx
        and    ax, 0x7f
        add    ax, ax
        add    ax, 0x148e
        lea    edx, [esi - 2]
        out    dx, ax
        lea    edx, [esi - 0xc]
        mov    ax, word ptr [ebp - 6]
        out    dx, ax
        inc    bx
        mov    word ptr [edi + 0x1f1], bx
        mov    dx, word ptr [edi + 0x10]
        add    dx, 4
        mov    ax, 0x1086
        out    dx, ax
        sub    dx, 4
        cmp    byte ptr [edi + 0xe6], 0
        mov    ax, bx
        out    dx, ax
        jne    loc_4_3FD0
        mov    eax, dword ptr [edi + 0x273]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        push   edi
        call   sub_4_3630
loc_4_3FD0:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_3FE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x3f5], 0
        je     loc_4_4080
        mov    dword ptr [ebx + 0x3dd], 0
        mov    dword ptr [ebx + 0x3e1], 0
        mov    ax, word ptr [ebx + 0x2dd]
        mov    word ptr [ebx + 0x3d3], ax
        mov    eax, dword ptr [ebx + 0x3e9]
        mov    dword ptr [ebp - 0xc], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x2d5]
        cmp    eax, dword ptr [ebp - 0xc]
        ja     loc_4_40F9
loc_4_4036:
        mov    si, word ptr [ebx + 0x10]
        mov    cx, word ptr [ebx + 0x3d3]
        lea    edx, [esi + 0xc]
        mov    ax, cx
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        shl    cx, 0xa
        mov    ax, cx
        out    dx, ax
        mov    dx, si
        in     ax, dx
        mov    word ptr [ebx + 0x3d3], ax
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x2d5]
        sub    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], eax
        jae    loc_4_4036
        jmp    loc_4_40F9
        db  08Dh, 064h, 024h, 000h
loc_4_4080:
        lea    eax, [ebx + 0x2d5]
        mov    cx, word ptr [ebx + 0x2d5]
        sub    cx, word ptr [ebx + 0x3d9]
        add    cx, 6
        mov    dx, word ptr [ebx + 0x16]
        mov    al, 1
        mov    si, word ptr [ebx + 0x10]
        out    dx, al
        lea    edx, [esi + 0xc]
        lea    eax, [ebx + 0x3d3]
        mov    di, word ptr [ebx + 0x3d3]
        or     di, 0x400
        mov    ax, di
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        shl    di, 0xa
        add    di, cx
        mov    ax, di
        out    dx, ax
        mov    dx, si
        in     al, dx
        and    cx, 3
        mov    ax, cx
        dec    cx
        or     ax, ax
        je     loc_4_40F2
        mov    dx, word ptr [ebx + 0x14]
loc_4_40E7:
        in     al, dx
        mov    ax, cx
        dec    cx
        or     ax, ax
        jne    loc_4_40E7
loc_4_40F2:
        mov    dword ptr [ebp - 0xc], 0
loc_4_40F9:
        lea    eax, [ebx + 0x3f6]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebx + 0x3d5]
        mov    ecx, eax
        cmp    ecx, 6
        jb     loc_4_4114
        mov    ecx, 6
loc_4_4114:
        mov    dword ptr [ebp - 0x1c], ecx
        sub    eax, ecx
        mov    dword ptr [ebx + 0x3d5], eax
        cmp    dword ptr [ebx + 0x3d9], 0
        je     loc_4_42CA
loc_4_412C:
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_4_413F
        cmp    dword ptr [ebx + 0x3e1], 0
        je     loc_4_42CA
loc_4_413F:
        cmp    dword ptr [ebx + 0x3dd], 0
        jne    loc_4_41FE
        mov    eax, dword ptr [ebp - 0xc]
        mov    dx, word ptr [ebx + 0x16]
        lea    ecx, [eax + 6]
        mov    al, 1
        out    dx, al
        mov    ax, word ptr [ebx + 0x10]
        add    ax, 0xc
        mov    word ptr [ebp - 0x10], ax
        mov    si, word ptr [ebx + 0x3d3]
        mov    di, si
        or     di, 0x400
        mov    ax, di
        shr    ax, 6
        mov    dx, word ptr [ebp - 0x10]
        out    dx, ax
        mov    ax, dx
        sub    ax, 2
        mov    word ptr [ebp - 0x18], ax
        mov    ax, di
        shl    ax, 0xa
        add    ax, cx
        mov    dx, word ptr [ebp - 0x18]
        out    dx, ax
        mov    dx, word ptr [ebx + 0x10]
        in     al, dx
        and    cx, 3
        mov    ax, cx
        dec    cx
        or     ax, ax
        je     loc_4_41BE
        mov    dx, word ptr [ebx + 0x14]
loc_4_41B3:
        in     al, dx
        mov    ax, cx
        dec    cx
        or     ax, ax
        jne    loc_4_41B3
loc_4_41BE:
        mov    ax, si
        shr    ax, 6
        mov    dx, word ptr [ebp - 0x10]
        out    dx, ax
        mov    ax, si
        shl    ax, 0xa
        mov    dx, word ptr [ebp - 0x18]
        out    dx, ax
        mov    dx, word ptr [ebx + 0x10]
        in     ax, dx
        mov    word ptr [ebx + 0x3d3], ax
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x2d5]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx + 0x3dd], eax
        mov    dword ptr [ebp - 0xc], 0
loc_4_41FE:
        cmp    dword ptr [ebx + 0x3e1], 0
        jne    loc_4_4225
        mov    eax, dword ptr [ebp - 0x14]
        dec    dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 0x3e5], eax
        mov    eax, dword ptr [ebp - 0x14]
        add    dword ptr [ebp - 0x14], 8
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx + 0x3e1], eax
loc_4_4225:
        mov    eax, dword ptr [ebx + 0x3dd]
        mov    ecx, dword ptr [ebx + 0x3e1]
        cmp    eax, ecx
        jb     loc_4_4237
        mov    eax, ecx
loc_4_4237:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebx + 0x3d9]
        cmp    dword ptr [ebp - 8], eax
        jbe    loc_4_4248
        mov    dword ptr [ebp - 8], eax
loc_4_4248:
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_42BD
        mov    eax, dword ptr [ebx + 0x3e5]
        mov    dword ptr [ebp - 0x20], eax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebx + 0x14]
        mov    word ptr [ebp - 4], ax
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    dx, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 2]
        mov    edi, dword ptr [ebp - 0x20]
        cmp    cx, 0x20
        jb     loc_4_429C
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        sub    ax, cx
        rep insb byte ptr es:[edi], dx
        mov    cx, ax
        shr    cx, 2
        rep insd dword ptr es:[edi], dx
        mov    cx, ax
        and    cx, 3
loc_4_429C:
        rep insb byte ptr es:[edi], dx
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebx + 0x3e5], eax
        sub    dword ptr [ebx + 0x3d9], eax
        sub    dword ptr [ebx + 0x3dd], eax
        sub    dword ptr [ebx + 0x3e1], eax
loc_4_42BD:
        cmp    dword ptr [ebx + 0x3d9], 0
        jne    loc_4_412C
loc_4_42CA:
        mov    esi, dword ptr [ebx + 0x3d9]
        or     esi, esi
        je     loc_4_42DD
        cmp    dword ptr [ebx + 0x3d5], 0
        jne    loc_4_4302
loc_4_42DD:
        mov    dx, word ptr [ebx + 0x16]
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [ebx + 0x10]
        add    dx, 0xc
        mov    ax, 1
        out    dx, ax
        mov    ecx, dword ptr [ebx + 0x3ed]
        mov    eax, dword ptr [ebx + 0x3f1]
        sub    ecx, esi
        mov    dword ptr [eax], ecx
loc_4_4302:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_4310:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0xe2]
        cmp    word ptr [esi + 0xe4], ax
        je     loc_4_44EC
        sub    ebx, ebx
loc_4_4332:
        mov    eax, dword ptr [esi + 0xb7]
        and    al, 0x50
        cmp    al, 0x10
        jne    loc_4_44EC
        cmp    word ptr [esi + 0x277], 0xf
        jb     loc_4_44E8
        mov    ecx, dword ptr [esi + 0xde]
        mov    dword ptr [ebp - 0x18], ecx
        mov    eax, ecx
        add    eax, 0x24
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0xde], eax
        cmp    byte ptr [esi + 0xe6], bl
        je     loc_4_4380
        inc    word ptr [esi + 0xe4]
        jmp    loc_4_43A3
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_4380:
        mov    ecx, dword ptr [ebp - 0x18]
        cmp    dword ptr [esi + 0xda], ecx
        jne    loc_4_4391
        mov    dword ptr [esi + 0xda], ebx
loc_4_4391:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0xd6], eax
        dec    word ptr [esi + 0xe2]
loc_4_43A3:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        mov    dword ptr [esi + 0x273], eax
        mov    eax, dword ptr [ebp - 0x14]
        lea    edi, [esi + 0x279]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        cmp    eax, 6
        jae    loc_4_4410
        cmp    eax, ebx
        je     loc_4_4410
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_4473
loc_4_43E2:
        lea    eax, [edi + 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_08F4
loc_4_43F0:
        cmp    dword ptr [edi + 4], ebx
        je     loc_4_43F8
        add    edi, 8
loc_4_43F8:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0918
loc_4_4405:
        cmp    dword ptr [ebp - 4], ebx
        jne    loc_4_43E2
        jmp    loc_4_4473
        db  08Dh, 064h, 024h, 000h
loc_4_4410:
        mov    eax, dword ptr [esi + 0x2a9]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 4], ebx
        je     loc_4_4462
        mov    edi, edi
loc_4_4420:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_08F4
loc_4_4431:
        cmp    dword ptr [ebp - 8], ebx
        je     loc_4_4450
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_098C
loc_4_4447:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
loc_4_4450:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0918
loc_4_445D:
        cmp    dword ptr [ebp - 4], ebx
        jne    loc_4_4420
loc_4_4462:
        mov    eax, dword ptr [esi + 0x2a9]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x273]
        mov    dword ptr [edi + 4], eax
loc_4_4473:
        cmp    dword ptr [esi + 0x273], 0x161
        push   esi
        jae    loc_4_4487
        push   0                             ; -> loc_4_3E30
        jmp    loc_4_448C
loc_4_4487:
        push   0                             ; -> loc_4_3A50
loc_4_448C:
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_4498:
        cmp    byte ptr [esi + 0xe6], bl
        jne    loc_4_44CB
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        test   byte ptr [eax + 0x19], 0x80
        jne    loc_4_44BA
        push   ebx
        mov    ecx, dword ptr [ebp - 0x18]
        push   ecx
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_07EA
loc_4_44BA:
        cmp    byte ptr [esi + 0xe6], bl
        jne    loc_4_44CB
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 0x14]
loc_4_44CB:
        mov    ax, word ptr [esi + 0xe2]
        cmp    word ptr [esi + 0xe4], ax
        jne    loc_4_4332
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_44E8:
        mov    byte ptr [esi + 0x2c], 1
loc_4_44EC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4500:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0
        lea    eax, [ebp - 0x18]
        mov    byte ptr [ebp - 1], 1
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
        mov    esi, dword ptr [edi + 8]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_1_08D0
loc_4_4535:
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [esi + 0x3a], eax
        jae    loc_4_4550
        mov    eax, 0xc001000f
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_4_4550:
        test   byte ptr [edi + 0x18], 2
        je     loc_4_45A0
        mov    eax, dword ptr [esi + 0xb7]
        and    al, 0x50
        cmp    al, 0x10
        jne    loc_4_45A0
        cmp    byte ptr [esi + 0x2b], 0
        jne    loc_4_45B8
        mov    ax, word ptr [esi + 0xe4]
        cmp    word ptr [esi + 0xe2], ax
        jne    loc_4_45B8
        cmp    word ptr [esi + 0x277], 0xf
        jae    loc_4_458E
        mov    byte ptr [ebp - 2], 1
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [esi + 0x2c], 1
loc_4_458E:
        cmp    byte ptr [esi + 0xe6], 0
        je     loc_4_45C0
        mov    byte ptr [ebp - 2], 1
        jmp    loc_4_45C0
        db  08Dh, 049h, 000h
loc_4_45A0:
        push   1
        push   0
        push   edi
        call   sub_4_32F0
        mov    dword ptr [ebp - 0x14], eax
        cmp    eax, 0x103
        jne    loc_4_4767
loc_4_45B8:
        mov    byte ptr [ebp - 2], 1
        mov    byte ptr [ebp - 1], 0
loc_4_45C0:
        cmp    byte ptr [ebp - 2], 0
        je     loc_4_4665
        mov    eax, dword ptr [ebp - 0x18]
        mov    word ptr [ebx + 0x30], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 0x2c], eax
        cmp    dword ptr [ebp - 0x1c], 0xffff
        jbe    loc_4_45E7
        mov    dword ptr [ebp - 0x1c], 0
loc_4_45E7:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    word ptr [ebx + 0x32], ax
        mov    dword ptr [ebx + 0x24], 0
        mov    dword ptr [ebx + 0x28], edi
        cmp    dword ptr [esi + 0xd6], 0
        jne    loc_4_4620
        mov    dword ptr [esi + 0xde], ebx
        lea    ecx, [esi + 0xde]
        mov    dword ptr [esi + 0xda], ebx
        mov    dword ptr [esi + 0xd6], ebx
        jmp    loc_4_463C
        db  005h, 000h, 000h, 000h, 000h
loc_4_4620:
        mov    eax, dword ptr [esi + 0xda]
        lea    ecx, [esi + 0xde]
        mov    dword ptr [eax + 0x24], ebx
        mov    dword ptr [esi + 0xda], ebx
        cmp    dword ptr [ecx], 0
        jne    loc_4_463C
        mov    dword ptr [ecx], ebx
loc_4_463C:
        inc    word ptr [esi + 0xe2]
        inc    dword ptr [edi + 0x14]
        cmp    byte ptr [ebp - 1], 0
        je     loc_4_4658
        inc    word ptr [esi + 0xe4]
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [ecx], eax
loc_4_4658:
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_4_4665
        mov    dword ptr [ebp - 0x14], 0x103
loc_4_4665:
        cmp    byte ptr [ebp - 1], 0
        je     loc_4_475B
        mov    byte ptr [esi + 0x2b], 1
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [esi + 0x273], eax
        lea    edi, [esi + 0x279]
        cmp    dword ptr [ebp - 0x1c], 6
        jae    loc_4_46C4
        cmp    dword ptr [ebp - 0x1c], 0
        jbe    loc_4_46C4
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_4719
        sub    ebx, ebx
loc_4_469A:
        lea    eax, [edi + 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_08F4
loc_4_46A8:
        cmp    dword ptr [edi + 4], ebx
        je     loc_4_46B0
        add    edi, 8
loc_4_46B0:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0918
loc_4_46BD:
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_4_469A
        jmp    loc_4_4719
loc_4_46C4:
        mov    ebx, dword ptr [esi + 0x2a9]
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_470B
loc_4_46D0:
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_08F4
loc_4_46E1:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_4_46F8
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   ebx
        call   sub_1_098C
loc_4_46F5:
        add    ebx, dword ptr [ebp - 0xc]
loc_4_46F8:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0918
loc_4_4705:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_46D0
loc_4_470B:
        mov    eax, dword ptr [esi + 0x2a9]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [edi + 4], eax
loc_4_4719:
        cmp    dword ptr [ebp - 0x18], 0x161
        push   esi
        jae    loc_4_4730
        push   0                             ; -> loc_4_3E30
        jmp    loc_4_4735
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_4730:
        push   0                             ; -> loc_4_3A50
loc_4_4735:
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_4741:
        mov    ax, word ptr [esi + 0xe4]
        cmp    word ptr [esi + 0xe2], ax
        je     loc_4_4757
        push   esi
        call   sub_4_4310
loc_4_4757:
        mov    byte ptr [esi + 0x2b], 0
loc_4_475B:
        mov    eax, dword ptr [ebp - 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_4767:
        mov    eax, dword ptr [ebp - 0x14]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_4780:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [eax + 8]
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0x426]
        cmp    edi, ebx
        jb     loc_4_47B3
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_4_47B3:
        sub    ebx, edi
        cmp    dword ptr [ebp + 0x14], ebx
        jae    loc_4_47C0
        mov    ebx, dword ptr [ebp + 0x14]
        jmp    loc_4_47C3
        db  090h
loc_4_47C0:
        mov    dword ptr [ebp + 0x14], ebx
loc_4_47C3:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x428]
        push   0
        add    edi, eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [esi + 0x3d5]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_08D0
loc_4_47E6:
        lea    eax, [ebx + edi]
        cmp    eax, dword ptr [esi + 0x535]
        ja     loc_4_48C0
        mov    dword ptr [ebp - 0xc], 0
        lea    eax, [edi + esi + 0x2e3]
        mov    dword ptr [ebp - 0x14], eax
        or     ebx, ebx
        je     loc_4_48AA
        mov    edi, edi
loc_4_4810:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_4_483E
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_48AA
        lea    eax, [ebp - 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_08F4
loc_4_4831:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0918
loc_4_483E:
        mov    edx, dword ptr [ebp - 0xc]
        cmp    edx, ebx
        jb     loc_4_4847
        mov    edx, ebx
loc_4_4847:
        or     edx, edx
        je     loc_4_48A2
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x18], eax
        mov    word ptr [ebp - 2], dx
        push   eax
        push   ecx
        push   esi
        push   edi
        movzx  ecx, word ptr [ebp - 2]
        mov    esi, dword ptr [ebp - 0x1c]
        mov    edi, dword ptr [ebp - 0x18]
        cmp    cx, 0x20
        jb     loc_4_488F
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        sub    ax, cx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    cx, ax
        shr    cx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cx, ax
        and    cx, 3
loc_4_488F:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        sub    ebx, edx
        je     loc_4_48A2
        add    dword ptr [ebp - 0x14], edx
        add    dword ptr [ebp - 0x10], edx
        sub    dword ptr [ebp - 0xc], edx
loc_4_48A2:
        or     ebx, ebx
        jne    loc_4_4810
loc_4_48AA:
        mov    ecx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x1c]
        sub    ecx, ebx
        mov    dword ptr [eax], ecx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  090h
loc_4_48C0:
        mov    dword ptr [esi + 0x3d9], ebx
        mov    dword ptr [esi + 0x3e9], edi
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0x3ed], eax
        or     ebx, ebx
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [esi + 0x3f1], eax
        mov    byte ptr [esi + 0x3f5], 1
        je     loc_4_494A
loc_4_48E9:
        mov    edi, dword ptr [esi + 0x3d5]
        or     edi, edi
        je     loc_4_494A
        cmp    edi, 6
        jb     loc_4_48FD
        mov    edi, 6
loc_4_48FD:
        lea    ebx, [esi + 0x3f6]
        or     edi, edi
        je     loc_4_4928
loc_4_4907:
        lea    eax, [ebx + 4]
        push   eax
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_08F4
loc_4_4915:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0918
loc_4_4922:
        add    ebx, 8
        dec    edi
        jne    loc_4_4907
loc_4_4928:
        push   esi
        push   0                             ; -> loc_4_3FE0
        lea    eax, [esi + 0x515]
        push   eax
        call   sub_1_089A
loc_4_493A:
        mov    byte ptr [esi + 0x3f5], 0
        cmp    dword ptr [esi + 0x3d9], 0
        jne    loc_4_48E9
loc_4_494A:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4960:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    bl, bl
        mov    si, word ptr [ecx + 0x10]
        add    si, 0xc
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        lea    edx, [esi - 2]
        mov    ax, 0x1080
        out    dx, ax
        sub    si, 0xa
        mov    dx, si
        in     ax, dx
        mov    word ptr [ecx + 0x2d9], ax
        in     ax, dx
        mov    word ptr [ecx + 0xef], ax
        sub    ax, word ptr [ecx + 0xed]
        add    ax, 0x80
        cmp    byte ptr [ecx + 0xe6], bl
        mov    word ptr [ecx + 0x277], ax
        je     loc_4_4A36
        in     ax, dx
        mov    word ptr [ecx + 0xe7], ax
        mov    si, ax
        sub    si, word ptr [ecx + 0xe9]
        je     loc_4_4A5A
        mov    bl, 1
        jbe    loc_4_4A26
loc_4_49D8:
        mov    di, word ptr [ecx + 0xe9]
        and    di, 0x7f
        mov    dx, word ptr [ecx + 0x10]
        add    dx, 4
        lea    eax, [edi + 0x158e]
        and    ax, 0xfffe
        out    dx, ax
        sub    dx, 4
        test   di, 1
        in     ax, dx
        mov    edx, edi
        je     loc_4_4A15
        and    edx, 0xffff
        mov    byte ptr [ecx + edx + 0x1f3], al
        jmp    loc_4_4A22
loc_4_4A15:
        and    edx, 0xffff
        mov    byte ptr [ecx + edx + 0x1f3], ah
loc_4_4A22:
        dec    si
        jne    loc_4_49D8
loc_4_4A26:
        mov    ax, word ptr [ecx + 0xe7]
        mov    word ptr [ecx + 0xe9], ax
        jmp    loc_4_4A5A
loc_4_4A36:
        cmp    byte ptr [ecx + 0x2c], 0
        je     loc_4_4A5A
        mov    ax, word ptr [ecx + 0xef]
        sub    ax, word ptr [ecx + 0xed]
        add    ax, 0x80
        cmp    ax, 0xf
        jbe    loc_4_4A5A
        mov    bl, 1
        mov    byte ptr [ecx + 0x2c], 0
loc_4_4A5A:
        mov    ax, word ptr [ecx + 0x2db]
        cmp    word ptr [ecx + 0x2d9], ax
        je     loc_4_4B5A
        mov    bl, 1
        mov    dx, word ptr [ecx + 0x10]
        add    dx, 4
        and    ax, 0x1ff
        add    ax, ax
        add    ax, 0x108e
        out    dx, ax
        sub    dx, 4
        in     ax, dx
        mov    word ptr [ecx + 0x2dd], ax
        mov    dx, word ptr [ecx + 0x16]
        mov    al, bl
        mov    si, word ptr [ecx + 0x10]
        out    dx, al
        lea    edx, [esi + 0xc]
        mov    di, word ptr [ecx + 0x2dd]
        or     di, 0x400
        mov    ax, di
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        mov    ax, di
        shl    ax, 0xa
        out    dx, ax
        mov    dx, si
        in     al, dx
        mov    dx, word ptr [ecx + 0x14]
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        mov    word ptr [ecx + 0x2df], dx
        mov    dx, word ptr [ecx + 0x14]
        in     ax, dx
        mov    word ptr [ecx + 0x2e1], ax
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        sub    dx, 4
        mov    word ptr [ecx + 0x426], dx
        lea    eax, [ecx + 0x2e3]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 0x535]
        mov    dx, word ptr [ecx + 0x426]
        cmp    ax, dx
        jb     loc_4_4B13
        mov    ax, dx
loc_4_4B13:
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ecx + 0x14]
        mov    word ptr [ebp - 4], ax
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    dx, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 2]
        mov    edi, dword ptr [ebp - 8]
        cmp    cx, 0x20
        jb     loc_4_4B54
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        sub    ax, cx
        rep insb byte ptr es:[edi], dx
        mov    cx, ax
        shr    cx, 2
        rep insd dword ptr es:[edi], dx
        mov    cx, ax
        and    cx, 3
loc_4_4B54:
        rep insb byte ptr es:[edi], dx
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
loc_4_4B5A:
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4B70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xe6], 0
        je     loc_4_4B86
        push   esi
        call   sub_4_3740
loc_4_4B86:
        mov    ax, word ptr [esi + 0xe4]
        cmp    word ptr [esi + 0xe2], ax
        je     loc_4_4BAA
        cmp    byte ptr [esi + 0x2b], 0
        jne    loc_4_4BAA
        mov    byte ptr [esi + 0x2b], 1
        push   esi
        call   sub_4_4310
        mov    byte ptr [esi + 0x2b], 0
loc_4_4BAA:
        mov    ax, word ptr [esi + 0x2db]
        cmp    word ptr [esi + 0x2d9], ax
        push   esi
        je     loc_4_4BC5
        call   sub_4_38D0
        pop    esi
        pop    ebp
        ret    4
loc_4_4BC5:
        push   0                             ; -> loc_4_0000
        add    esi, 0x515
        push   esi
        call   sub_1_089A
loc_4_4BD6:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_4BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        lea    esp, [esp]
loc_4_4BF0:
        test   byte ptr [ecx + 0x2e1], 1
        jne    loc_4_4C38
        mov    di, word ptr [ecx + 0x10]
        mov    si, word ptr [ecx + 0x2df]
        lea    edx, [edi + 0xc]
        mov    ax, si
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        shl    si, 0xa
        mov    ax, si
        out    dx, ax
        lea    edx, [edi + 2]
        in     ax, dx
        mov    word ptr [ecx + 0x2df], ax
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        mov    word ptr [ecx + 0x2e1], dx
        jmp    loc_4_4BF0
loc_4_4C38:
        mov    ax, word ptr [ecx + 0x2df]
        mov    word ptr [ecx + 0x2dd], ax
        or     ax, ax
        jne    loc_4_4D07
        mov    ax, word ptr [ecx + 0x2db]
        inc    ax
        cmp    word ptr [ecx + 0x2d9], ax
        mov    word ptr [ecx + 0x2db], ax
        jne    loc_4_4C8E
        mov    si, word ptr [ecx + 0x10]
        add    si, 0xc
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        lea    edx, [esi - 2]
        mov    ax, 0x1080
        out    dx, ax
        lea    edx, [esi - 0xa]
        in     ax, dx
        mov    word ptr [ecx + 0x2d9], ax
loc_4_4C8E:
        mov    di, word ptr [ecx + 0x2db]
        cmp    word ptr [ecx + 0x2d9], di
        jne    loc_4_4CD5
        mov    bx, word ptr [ecx + 0x10]
        mov    ax, 1
        lea    esi, [ebx + 0xc]
        mov    dx, si
        out    dx, ax
        lea    edx, [esi - 2]
        mov    ax, 0x1088
        out    dx, ax
        lea    edx, [esi - 0xc]
        mov    ax, di
        out    dx, ax
        lea    edx, [ebx + 7]
        mov    al, byte ptr [ecx + 0x34]
        out    dx, al
        mov    byte ptr [ecx + 0x34], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_4CD5:
        mov    si, word ptr [ecx + 0x10]
        add    si, 0xc
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        lea    edx, [esi - 2]
        and    di, 0x1ff
        add    di, di
        add    di, 0x108e
        mov    ax, di
        out    dx, ax
        lea    edx, [esi - 0xc]
        in     ax, dx
        mov    word ptr [ecx + 0x2dd], ax
loc_4_4D07:
        mov    dx, word ptr [ecx + 0x16]
        mov    al, 1
        mov    si, word ptr [ecx + 0x10]
        out    dx, al
        lea    edx, [esi + 0xc]
        mov    di, word ptr [ecx + 0x2dd]
        or     di, 0x400
        mov    ax, di
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        mov    ax, di
        shl    ax, 0xa
        out    dx, ax
        mov    dx, si
        in     al, dx
        mov    dx, word ptr [ecx + 0x14]
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        mov    word ptr [ecx + 0x2df], dx
        mov    dx, word ptr [ecx + 0x14]
        in     ax, dx
        mov    word ptr [ecx + 0x2e1], ax
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        sub    dx, 4
        mov    word ptr [ecx + 0x426], dx
        lea    eax, [ecx + 0x2e3]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 0x535]
        mov    dx, word ptr [ecx + 0x426]
        cmp    ax, dx
        jb     loc_4_4D88
        mov    ax, dx
loc_4_4D88:
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ecx + 0x14]
        mov    word ptr [ebp - 4], ax
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    dx, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 2]
        mov    edi, dword ptr [ebp - 8]
        cmp    cx, 0x20
        jb     loc_4_4DC9
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        sub    ax, cx
        rep insb byte ptr es:[edi], dx
        mov    cx, ax
        shr    cx, 2
        rep insd dword ptr es:[edi], dx
        mov    cx, ax
        and    cx, 3
loc_4_4DC9:
        rep insb byte ptr es:[edi], dx
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_4DE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 0x2db], 0
        mov    word ptr [esi + 0x2d9], 0
        mov    word ptr [esi + 0x2e1], 0
        mov    word ptr [esi + 0x2df], 0
        mov    word ptr [esi + 0x2dd], 0
        mov    word ptr [esi + 0xef], 0
        mov    word ptr [esi + 0xed], 0
        mov    word ptr [esi + 0x1f1], 0
        mov    word ptr [esi + 0xeb], 0
        mov    word ptr [esi + 0xe9], 0
        mov    word ptr [esi + 0xe7], 0
        mov    dword ptr [esi + 0xde], 0
        mov    dword ptr [esi + 0xda], 0
        mov    dword ptr [esi + 0xd6], 0
        mov    word ptr [esi + 0xe4], 0
        mov    word ptr [esi + 0xe2], 0
        mov    word ptr [esi + 0x277], 0x80
        mov    word ptr [esi + 0x2ad], 0x100
        mov    word ptr [esi + 0x2af], 0x161
        mov    word ptr [esi + 0x2b1], 0x100
        mov    word ptr [esi + 0x2b3], 0x161
        mov    word ptr [esi + 0x2b5], 0x200
        mov    word ptr [esi + 0x2b7], 0x261
        mov    word ptr [esi + 0x2b9], 0x3f6
        mov    word ptr [esi + 0x2bb], 0x457
        mov    word ptr [esi + 0x2bd], 0x3f6
        mov    word ptr [esi + 0x2bf], 0x457
        mov    word ptr [esi + 0x2c1], 0x3f6
        mov    word ptr [esi + 0x2c3], 0x457
        mov    word ptr [esi + 0x2c5], 0x3f6
        mov    word ptr [esi + 0x2c7], 0x457
        cmp    byte ptr [ebp + 0xc], 0
        mov    word ptr [esi + 0x2c9], 0x3f6
        mov    word ptr [esi + 0x2cb], 0x457
        mov    word ptr [esi + 0x2cd], 0x3f6
        mov    word ptr [esi + 0x2cf], 0x457
        mov    word ptr [esi + 0x2d1], 0x3f6
        mov    word ptr [esi + 0x2d3], 0x457
        je     loc_4_4F7C
        sub    di, di
loc_4_4F41:
        mov    cx, word ptr [esi + 0x10]
        add    cx, 0xc
        mov    dx, cx
        mov    ax, 1
        out    dx, ax
        lea    edx, [ecx - 2]
        lea    eax, [edi*2 + 0x148e]
        out    dx, ax
        lea    edx, [ecx - 0xc]
        in     ax, dx
        mov    ecx, edi
        and    ecx, 0xffff
        mov    word ptr [esi + ecx*2 + 0xf1], ax
        inc    di
        cmp    di, 0x80
        jb     loc_4_4F41
loc_4_4F7C:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 032h, 045h, 06Eh, 068h
VxD__LTEXT ENDS

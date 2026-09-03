; NDIS2SUP -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 5187 instructions recovered by recursive descent

Declare_Virtual_Device NDIS2SUP, 1, 0, NDIS2SUP_Control, 00496h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h
;   service table: obj1:0x3818, 2 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3b00  R-X
sub_1_0000:
        push   0
        push   0x12
        call   sub_1_3414
        add    esp, 8
        mov    dword ptr [0x3784], eax       ; -> loc_1_3784
        or     eax, eax
        je     loc_1_0028
        mov    word ptr [eax], 0
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    word ptr [eax + 2], 0
        sub    eax, eax
        ret    
loc_1_0028:
        mov    eax, 0xc0000001
        ret    
        db  08Bh, 0FFh, 06Ah, 000h, 0A1h
        dd  loc_1_3784               ; fixup
        db  050h, 0E8h, 019h, 034h, 000h, 000h, 083h, 0C4h, 008h, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h
        db  024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0050:
        push   esi
        push   edi
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        cmp    word ptr [eax], 0
        jne    loc_1_00A4
        lea    ecx, [ecx]
loc_1_0060:
        mov    esi, dword ptr [0x3784]       ; -> loc_1_3784
        mov    eax, esi
        push   0
        mov    ax, word ptr [eax + 2]
        and    eax, 0xffff
        lea    eax, [eax*2 + 0xa]
        lea    eax, [eax + eax*8]
        push   eax
        call   sub_1_3414
        add    esp, 8
        cmp    esi, dword ptr [0x3784]       ; -> loc_1_3784
        je     loc_1_00D9
        push   0
        push   eax
        call   sub_1_3456
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        add    esp, 8
        cmp    word ptr [eax], 0
        je     loc_1_0060
loc_1_00A4:
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        lea    ecx, [eax*2]
        pop    edi
        lea    esi, [ecx + ecx*8]
        mov    ecx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dx, word ptr [esi + ecx]
        mov    word ptr [ecx], dx
        mov    ecx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [esi + ecx + 0xe], 0
        pop    esi
        ret    
loc_1_00D9:
        or     eax, eax
        jne    loc_1_00E2
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_00E2:
        mov    ecx, dword ptr [0x3784]       ; -> loc_1_3784
        xor    edx, edx
        mov    dx, word ptr [ecx + 2]
        lea    edx, [edx*2 + 2]
        mov    edi, eax
        lea    edx, [edx + edx*8]
        mov    esi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    cx, word ptr [eax + 2]
        inc    cx
        mov    word ptr [eax], cx
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        lea    esi, [ecx + 1]
        add    ecx, 4
        cmp    ecx, esi
        jbe    loc_1_014D
        lea    ecx, [esi*2]
        lea    ecx, [ecx + ecx*8]
        lea    edi, [eax + ecx]
loc_1_0136:
        lea    ecx, [esi + 1]
        mov    word ptr [edi], cx
        add    edi, 0x12
        inc    esi
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        add    ecx, 4
        cmp    ecx, esi
        ja     loc_1_0136
loc_1_014D:
        mov    cx, word ptr [eax + 2]
        add    cx, 4
        mov    word ptr [eax + 2], cx
        and    ecx, 0xffff
        add    ecx, ecx
        xor    esi, esi
        lea    ecx, [ecx + ecx*8]
        mov    word ptr [eax + ecx], 0
        mov    dword ptr [0x3784], eax       ; -> loc_1_3784
        mov    si, word ptr [eax]
        lea    ecx, [esi*2]
        push   0
        lea    edi, [ecx + ecx*8]
        mov    cx, word ptr [eax + edi]
        mov    word ptr [eax], cx
        push   edx
        call   sub_1_3456
        add    esp, 8
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [edi + eax + 0xe], 0
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_01B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi*2]
        mov    ax, word ptr [eax]
        lea    ecx, [ecx + ecx*8]
        mov    edx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    word ptr [ecx + edx], ax
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    word ptr [eax], si
        pop    esi
        pop    ebp
        ret    
        db  08Bh, 0FFh
sub_1_01E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0xffff
        cmp    eax, 0x23
        jg     loc_1_0200
        je     loc_1_0270
        cmp    eax, 0xd
        ja     loc_1_0216
        shl    eax, 2
        jmp    loc_1_0298
loc_1_0200:
        cmp    eax, 0x24
        je     loc_1_0277
        cmp    eax, 0x2a
        je     loc_1_0280
        cmp    eax, 0x2e
        je     loc_1_0284
        cmp    eax, 0xff
        je     loc_1_0290
loc_1_0216:
        sub    eax, eax
        pop    ebp
        ret    
loc_1_021A:
        sub    eax, eax
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0220:
        mov    eax, 0x103
        pop    ebp
        ret    
loc_1_0227:
        mov    eax, 0x10001
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0230:
        mov    eax, 0xc000009a
        pop    ebp
        ret    
loc_1_0237:
        mov    eax, 0xc0000001
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0240:
        mov    eax, 0xc001000d
        pop    ebp
        ret    
loc_1_0247:
        mov    eax, 0xc00000bb
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0250:
        mov    eax, 0x80010004
        pop    ebp
        ret    
loc_1_0257:
        mov    eax, 0x80010003
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0260:
        mov    eax, 0x10001
        pop    ebp
        ret    
loc_1_0267:
        mov    eax, 0xc0010016
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0270:
        mov    eax, 0xc0010006
        pop    ebp
        ret    
loc_1_0277:
        mov    eax, 0xc0010008
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0280:
        sub    eax, eax
        pop    ebp
        ret    
loc_1_0284:
        mov    eax, 0xc000009a
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_0290:
        mov    eax, 0xc0000001
        pop    ebp
        ret    
        db  090h
loc_1_0298:
        sbb    al, byte ptr [edx]
        add    byte ptr [eax], al
        push   ss
        add    al, byte ptr [eax]
        add    byte ptr [eax], ah            ; -> loc_1_0220
        add    al, byte ptr [eax]
        add    byte ptr [edi], ah            ; -> loc_1_0227
        add    al, byte ptr [eax]
        add    byte ptr [edi], ah            ; -> loc_1_0227
        add    al, byte ptr [eax]
        add    byte ptr [edi], ah            ; -> loc_1_0227
        add    al, byte ptr [eax]
        add    byte ptr [eax], dh            ; -> loc_1_0230
        add    al, byte ptr [eax]
        add    byte ptr [edi], dh            ; -> loc_1_0237
        add    al, byte ptr [eax]
        add    byte ptr [eax + 2], al        ; -> loc_1_0240
        add    byte ptr [eax], al
        inc    edi
        add    al, byte ptr [eax]
        add    byte ptr [eax + 2], dl        ; -> loc_1_0250
        add    byte ptr [eax], al
        push   edi
        add    al, byte ptr [eax]
        add    byte ptr [eax + 2], ah        ; -> loc_1_0260
        add    byte ptr [eax], al
        add    al, byte ptr [bx + si]
        add    byte ptr [ebp - 0x75], dl
        in     al, dx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x24]
        cmp    eax, 2
        jne    loc_1_02E5
        mov    eax, 0xc001000d
        pop    ebp
        ret    
loc_1_02E5:
        or     eax, eax
        jne    loc_1_02F0
        mov    eax, 0xc0000001
        pop    ebp
        ret    
loc_1_02F0:
        sub    eax, eax
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0300:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    ecx, dword ptr [ecx + 0x48]
        cmp    ecx, eax
        je     loc_1_031A
        nop    
loc_1_0310:
        or     eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    loc_1_0310
loc_1_031A:
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_0320:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        cmp    dword ptr [ecx + 0xb8], edx
        jbe    loc_1_0385
        lea    edi, [ecx + 0x98]
        lea    esi, [ecx + 0xbc]
        nop    
loc_1_0340:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x18]
        not    eax
        and    eax, dword ptr [esi]
        mov    dword ptr [esi], eax
        jne    loc_1_0376
        mov    eax, dword ptr [ecx + 0xb8]
        mov    ebx, dword ptr [ecx + eax*4 + 0x50]
        mov    dword ptr [esi - 0x68], ebx
        mov    bx, word ptr [ecx + eax*2 + 0x96]
        mov    word ptr [edi], bx
        dec    eax
        mov    eax, dword ptr [ecx + eax*4 + 0xbc]
        mov    dword ptr [esi], eax
        dec    dword ptr [ecx + 0xb8]
loc_1_0376:
        add    edi, 2
        add    esi, 4
        inc    edx
        cmp    dword ptr [ecx + 0xb8], edx
        ja     loc_1_0340
loc_1_0385:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0390:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_041C
        mov    ebx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        nop    
loc_1_03B0:
        push   ebx
        mov    eax, dword ptr [esi + 0xb8]
        push   eax
        lea    eax, [esi + 0x98]
        push   eax
        lea    eax, [esi + 0x54]
        push   eax
        call   sub_1_33CA
        add    esp, 0x10
        or     eax, eax
        je     loc_1_03E0
        mov    ecx, dword ptr [edi + 0x18]
        or     dword ptr [esi + eax*4 + 0xb8], ecx
        jmp    loc_1_040E
        db  005h, 000h, 000h, 000h, 000h
loc_1_03E0:
        mov    eax, dword ptr [esi + 0xb8]
        cmp    eax, 0x10
        jae    loc_1_0425
        lea    ecx, [esi + eax*4]
        mov    edx, dword ptr [ebx]
        mov    dword ptr [ecx + 0x54], edx
        mov    dx, word ptr [ebx + 4]
        mov    word ptr [esi + eax*2 + 0x98], dx
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [ecx + 0xbc], eax
        inc    dword ptr [esi + 0xb8]
loc_1_040E:
        add    ebx, 6
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 0x10], eax
        ja     loc_1_03B0
loc_1_041C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0425:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0440:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0x88
        call   sub_1_3414
        add    esp, 8
        or     eax, eax
        jne    loc_1_0460
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_0460:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 8], 0
        mov    ecx, dword ptr [ebp + 0x10]
        lea    edi, [eax + 0x28]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [eax + 0x10], 1
        mov    dword ptr [eax + 0x14], 0x1010103
        mov    dword ptr [eax + 0x20], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x18], edi
        mov    ecx, dword ptr [ebx + 0xb8]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    dword ptr [eax + 0x1c], ecx
        sub    ecx, ecx
        cmp    dword ptr [ebx + 0xb8], ecx
        jbe    loc_1_04D5
        mov    esi, ebx
        add    esi, 0x98
        mov    edx, ebx
        add    edx, 0x54
loc_1_04B5:
        mov    ebx, dword ptr [edx]
        mov    dword ptr [edi], ebx
        mov    bx, word ptr [esi]
        mov    word ptr [edi + 4], bx
        add    esi, 2
        add    edi, 6
        add    edx, 4
        mov    ebx, dword ptr [ebp + 8]
        inc    ecx
        cmp    dword ptr [ebx + 0xb8], ecx
        ja     loc_1_04B5
loc_1_04D5:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_04E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x4c]
        mov    edx, dword ptr [eax + 0x48]
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [eax + 0x48], ecx
        pop    ebp
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0500:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x48]
        or     edx, edx
        je     loc_1_0540
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, edx
        jne    loc_1_0520
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 0x48], eax
        pop    esi
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0520:
        mov    ecx, dword ptr [edx + 8]
        or     ecx, ecx
        je     loc_1_0540
loc_1_0527:
        cmp    eax, ecx
        je     loc_1_0537
        mov    edx, ecx
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    loc_1_0527
        pop    esi
        pop    ebp
        ret    
loc_1_0537:
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        dec    dword ptr [esi + 0x4c]
loc_1_0540:
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0550:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 1
        mov    eax, dword ptr [esi + 0x108]
loc_1_0565:
        test   eax, ecx
        jne    loc_1_0571
        add    ecx, ecx
        or     ecx, ecx
        jne    loc_1_0565
        jmp    loc_1_0580
loc_1_0571:
        mov    edx, ecx
        not    edx
        and    edx, eax
        mov    dword ptr [esi + 0x108], edx
        mov    dword ptr [ebp - 4], ecx
loc_1_0580:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0590
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
loc_1_0590:
        push   0
        push   0x2c
        call   sub_1_3414
        add    esp, 8
        or     eax, eax
        jne    loc_1_05B0
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [esi + 0x108], eax
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_05B0:
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0xc], 1
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x10], ecx
        mov    dword ptr [eax + 0x14], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0x1c], 0
        mov    dword ptr [eax + 0x20], 0
        mov    dword ptr [eax + 0x28], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
sub_1_05F0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x18]
        or     dword ptr [eax + 0x108], edx
        push   0
        push   ecx
        call   sub_1_3456
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0610:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [edx], 0xffffffff
        mov    eax, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 0x18]
        lea    eax, [eax + edi*4]
        mov    esi, dword ptr [ebp + 0x20]
loc_1_0633:
        or     edi, edi
        je     loc_1_0647
        sub    eax, 4
        dec    edi
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [esi + 0x114], ecx
        jne    loc_1_0633
        mov    dword ptr [edx], edi
loc_1_0647:
        cmp    dword ptr [edx], -1
        jne    loc_1_0657
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebp
        ret    0x24
loc_1_0657:
        cmp    dword ptr [esi + 0x24], 2
        jne    loc_1_0668
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebp
        ret    0x24
loc_1_0668:
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        push   esi
        call   sub_1_0550
        add    esp, 8
        or     eax, eax
        jne    loc_1_0684
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebp
        ret    0x24
loc_1_0684:
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    dword ptr [ecx], eax
        push   esi
        call   sub_1_04E0
        add    esp, 8
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_1_06A0
        mov    dword ptr [esi + 0x24], 1
loc_1_06A0:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_06B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0x1c], 0
        je     loc_1_06D8
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, 0x103
        mov    dword ptr [esi + 0xc], 0
        mov    dword ptr [edi + 0x50], 1
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_06D8:
        cmp    dword ptr [edi + 0x4c], 1
        jne    loc_1_06E5
        mov    dword ptr [edi + 0x24], 0
loc_1_06E5:
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   edi
        call   sub_1_0500
        add    esp, 8
        push   esi
        push   edi
        call   sub_1_05F0
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0710:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        cmp    dword ptr [esi + 0x50], edi
        je     loc_1_078E
        cmp    eax, edi
        jne    loc_1_078E
        mov    ebx, dword ptr [esi + 0x48]
        cmp    ebx, edi
        je     loc_1_0764
loc_1_0735:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebx + 0xc], 0
        jne    loc_1_075D
        cmp    dword ptr [esi + 0x4c], 1
        jne    loc_1_074E
        mov    dword ptr [esi + 0x24], 0
loc_1_074E:
        push   ebx
        push   esi
        call   sub_1_0500
        add    esp, 8
        mov    dword ptr [ebx + 8], edi
        mov    edi, ebx
loc_1_075D:
        mov    ebx, dword ptr [ebp - 4]
        or     ebx, ebx
        jne    loc_1_0735
loc_1_0764:
        mov    dword ptr [esi + 0x50], 0
        or     edi, edi
        je     loc_1_078E
        nop    
loc_1_0770:
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3568
        mov    ebx, dword ptr [edi + 8]
        push   edi
        push   esi
        call   sub_1_05F0
        add    esp, 8
        mov    edi, ebx
        or     ebx, ebx
        jne    loc_1_0770
loc_1_078E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_07A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 4]
        cmp    dword ptr [edi + 0x24], 2
        jne    loc_1_07C0
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_07C0:
        cmp    dword ptr [esi + 0xc], 1
        je     loc_1_07D2
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_07D2:
        mov    dword ptr [esi + 0xc], 2
        push   0
        push   0x2c
        call   sub_1_3414
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_0800
        mov    dword ptr [esi + 0xc], 1
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0800:
        mov    dword ptr [ebx + 0x14], 0x1010e
        mov    dword ptr [ebx + 0x10], 1
        push   3
        push   ebx
        push   esi
        call   sub_1_1B00
        add    esp, 0xc
        cmp    eax, 0x103
        jne    loc_1_082D
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_082D:
        push   0
        push   esi
        push   edi
        push   ebx
        call   sub_1_0850
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0850:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x114], 1
        je     loc_1_0896
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_3456
        add    esp, 8
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0320
        add    esp, 8
        mov    dword ptr [esi + 0xc], 4
        push   0
        push   esi
        call   sub_1_1EB0
        add    esp, 8
        mov    ebx, eax
        jmp    loc_1_08D1
loc_1_0896:
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xc], 3
        mov    dword ptr [edi + 0x14], 0x2010103
        lea    eax, [edi + 0x28]
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [eax], 0
        push   edi
        push   esi
        call   sub_1_1CB0
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_08D1
        push   0
        push   edi
        call   sub_1_3456
        add    esp, 8
loc_1_08D1:
        cmp    ebx, 0x103
        jne    loc_1_08E3
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_08E3:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0900
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0900:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0xc], 5
        jne    loc_1_0914
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_0914:
        mov    dword ptr [eax + 0xc], 5
        cmp    dword ptr [eax + 0x14], 0
        jle    loc_1_092A
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_092A:
        mov    esi, dword ptr [eax + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_06B0
        add    esp, 8
        mov    edi, eax
        cmp    edi, 0x103
        je     loc_1_0951
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_0951
        push   edi
        push   esi
        call   sub_1_3568
loc_1_0951:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0960:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 5
        mov    ecx, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    ax, word ptr [eax + ecx + 0x14]
        and    eax, 0xffff
        sub    eax, 4
        and    eax, dword ptr [0x377c]       ; -> loc_1_377C
        mov    ecx, dword ptr [0x3a7c]       ; -> obj1:0x3A7C
        or     dword ptr [eax + ecx], 0x75690000
        cmp    eax, dword ptr [0x3774]       ; -> loc_1_3774
        jne    loc_1_09EF
        cmp    dword ptr [0x3768], 0         ; -> loc_1_3768
        je     loc_1_09EF
        mov    edx, 0x7569
        sub    esi, esi
loc_1_09A8:
        mov    eax, dword ptr [0x3774]       ; -> loc_1_3774
        add    eax, dword ptr [0x3a7c]       ; -> obj1:0x3A7C
        cmp    word ptr [eax + 2], dx
        jne    loc_1_09EF
        dec    dword ptr [0x3768]            ; -> loc_1_3768
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [0x3774]       ; -> loc_1_3774
        add    ecx, eax
        add    ecx, 4
        and    ecx, dword ptr [0x377c]       ; -> loc_1_377C
        mov    dword ptr [0x3774], ecx       ; -> loc_1_3774
        add    eax, 4
        add    dword ptr [0x3770], eax       ; -> loc_1_3770
        cmp    dword ptr [0x3768], esi       ; -> loc_1_3768
        jne    loc_1_09A8
loc_1_09EF:
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 3
        and    eax, 0xfffffffc
        mov    dword ptr [ebp - 0x18], eax
        or     eax, 0x55490000
        mov    ecx, dword ptr [0x3778]       ; -> loc_1_3778
        mov    edx, dword ptr [0x3a7c]       ; -> obj1:0x3A7C
        mov    dword ptr [ecx + edx], eax
        mov    eax, dword ptr [0x3778]       ; -> loc_1_3778
        add    eax, 4
        and    eax, dword ptr [0x377c]       ; -> loc_1_377C
        mov    dword ptr [0x3778], eax       ; -> loc_1_3778
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 4
        sub    dword ptr [0x3770], eax       ; -> loc_1_3770
        mov    eax, dword ptr [ebp + 0x10]
        shl    eax, 5
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [0x3778]       ; -> loc_1_3778
        mov    ecx, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    edx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + edx + 0x14], ax
        lea    eax, [ebp - 0xc]
        mov    esi, dword ptr [ebp + 0xc]
        push   0
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3654
        mov    dword ptr [ebp - 4], 0
        mov    edi, dword ptr [0x3778]       ; -> loc_1_3778
        add    edi, dword ptr [0x3a7c]       ; -> obj1:0x3A7C
        mov    eax, dword ptr [0x3774]       ; -> loc_1_3774
        cmp    dword ptr [0x3778], eax       ; -> loc_1_3778
        jl     loc_1_0AA2
        mov    ebx, dword ptr [0x3a8c]       ; -> obj1:0x3A8C
        jmp    loc_1_0AA8
loc_1_0AA2:
        mov    ebx, dword ptr [0x3774]       ; -> loc_1_3774
loc_1_0AA8:
        sub    ebx, dword ptr [0x3778]       ; -> loc_1_3778
        cmp    dword ptr [ebp + 0xc], ebx
        ja     loc_1_0AD5
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    edx, dword ptr [ebp - 0x10]
        mov    word ptr [eax + edx + 0xe], 1
        mov    ecx, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    ax, bx
        mov    word ptr [ecx + edx + 0x12], ax
        jmp    loc_1_0B02
loc_1_0AD5:
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    edx, dword ptr [ebp - 0x10]
        mov    word ptr [eax + edx + 0xe], 2
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    word ptr [edx + eax + 0x12], bx
        mov    eax, dword ptr [ebp + 0xc]
        sub    ax, bx
        mov    ecx, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    word ptr [edx + ecx + 0x1a], ax
        mov    dword ptr [ebp - 0x14], ebx
loc_1_0B02:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_0B7A
loc_1_0B08:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0B2C
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_3678
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_369C
loc_1_0B2C:
        cmp    ebx, dword ptr [ebp - 4]
        jl     loc_1_0B54
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_3399
        add    esp, 0xc
        sub    esi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0B76
loc_1_0B54:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_3399
        add    esp, 0xc
        sub    esi, ebx
        add    dword ptr [ebp - 8], ebx
        sub    dword ptr [ebp - 4], ebx
        mov    ebx, dword ptr [ebp + 0xc]
        sub    ebx, dword ptr [ebp - 0x14]
        mov    edi, dword ptr [0x3a7c]       ; -> obj1:0x3A7C
loc_1_0B76:
        or     esi, esi
        jne    loc_1_0B08
loc_1_0B7A:
        mov    eax, dword ptr [0x3778]       ; -> loc_1_3778
        add    eax, dword ptr [ebp - 0x18]
        and    eax, dword ptr [0x377c]       ; -> loc_1_377C
        mov    dword ptr [0x3778], eax       ; -> loc_1_3778
        inc    dword ptr [0x3768]            ; -> loc_1_3768
        mov    eax, dword ptr [0x3774]       ; -> loc_1_3774
        cmp    dword ptr [0x3778], eax       ; -> loc_1_3778
        jle    loc_1_0BE0
        mov    eax, dword ptr [0x3a8c]       ; -> obj1:0x3A8C
        sub    eax, dword ptr [0x3778]       ; -> loc_1_3778
        cmp    eax, 0x104
        jge    loc_1_0BE0
        lea    ecx, [eax - 4]
        or     ecx, 0x75690000
        mov    edx, dword ptr [0x3778]       ; -> loc_1_3778
        mov    esi, dword ptr [0x3a7c]       ; -> obj1:0x3A7C
        mov    dword ptr [edx + esi], ecx
        sub    dword ptr [0x3770], eax       ; -> loc_1_3770
        mov    dword ptr [0x3778], 0         ; -> loc_1_3778
        inc    dword ptr [0x3768]            ; -> loc_1_3768
loc_1_0BE0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0
        call   sub_1_0050
        mov    dword ptr [ebp - 0x10], eax
        or     eax, eax
        je     loc_1_0E8F
        cmp    dword ptr [0x375c], 0         ; -> loc_1_375C
        je     loc_1_0E83
        cmp    dword ptr [0x3710], 0         ; -> loc_1_3710
        jne    loc_1_0E83
        mov    dword ptr [0x3710], 1         ; -> loc_1_3710
        cmp    dword ptr [0x375c], 0         ; -> loc_1_375C
        je     loc_1_0E60
loc_1_0C41:
        mov    eax, dword ptr [0x375c]       ; -> loc_1_375C
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 0x18]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3654
        mov    eax, dword ptr [0x3770]       ; -> loc_1_3770
        cmp    dword ptr [ebp - 0x18], eax
        jge    loc_1_0E60
        cmp    dword ptr [0x376c], -1        ; -> loc_1_376C
        je     loc_1_0E60
        mov    edi, dword ptr [ebp - 8]
        mov    eax, dword ptr [edi + 0x24]
        add    edi, 0x24
        mov    dword ptr [ebp - 0xc], eax
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [0x375c], eax       ; -> loc_1_375C
        or     eax, eax
        jne    loc_1_0C9C
        mov    dword ptr [0x3760], 0         ; -> loc_1_3760
loc_1_0C9C:
        mov    eax, dword ptr [0x376c]       ; -> loc_1_376C
        mov    dword ptr [ebp - 4], eax
        mov    ebx, eax
        shl    ebx, 5
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    eax, dword ptr [ebx + eax]
        mov    dword ptr [0x376c], eax       ; -> loc_1_376C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0A00
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [eax*2]
        mov    dword ptr [ebp - 0x14], 1
        lea    ecx, [ecx + ecx*8]
        mov    edx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + edx + 2], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 0xc], 1
        jne    loc_1_0DE6
        inc    dword ptr [esi + 0x126]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   esi
        call   sub_1_3222
        add    esp, 0xc
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    edi, eax
        mov    eax, dword ptr [esi + 8]
        test   byte ptr [eax + 0x40], 0x80
        jne    loc_1_0DEB
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    eax, dword ptr [ebx + eax + 0x14]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        add    ecx, dword ptr [esi + 0x11c]
        and    eax, 0xffff
        add    eax, ecx
        test   byte ptr [esi + 0xfc], 0x20
        jne    loc_1_0D9B
        cmp    dword ptr [esi + 0x114], 1
        je     loc_1_0D80
        test   byte ptr [eax], 1
        jne    loc_1_0D9B
        lea    ecx, [esi + 0x120]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    loc_1_0D79
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
loc_1_0D79:
        je     loc_1_0D9B
        jmp    loc_1_0DEB
        db  08Dh, 049h, 000h
loc_1_0D80:
        test   byte ptr [eax], 0x80
        jne    loc_1_0D9B
        lea    ecx, [esi + 0x120]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    loc_1_0DEB
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
        jne    loc_1_0DEB
loc_1_0D9B:
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        lea    ecx, [ebx + eax]
        mov    ax, word ptr [ecx + 0xe]
        mov    word ptr [ebp - 0x28], ax
        cmp    word ptr [ebp - 0x28], 1
        mov    ax, word ptr [ecx + 0x12]
        mov    word ptr [ebp - 0x26], ax
        mov    eax, dword ptr [ecx + 0x14]
        mov    dword ptr [ebp - 0x24], eax
        jbe    loc_1_0DCE
        mov    ax, word ptr [ecx + 0x1a]
        mov    word ptr [ebp - 0x20], ax
        mov    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 0x1e], eax
loc_1_0DCE:
        lea    eax, [ebp - 0x28]
        push   eax
        push   esi
        call   sub_1_26E0
        add    esp, 8
        push   esi
        call   sub_1_2530
        add    esp, 4
        jmp    loc_1_0DEB
loc_1_0DE6:
        mov    edi, 0x40010007
loc_1_0DEB:
        cmp    edi, 0x103
        je     loc_1_0E3A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0960
        add    esp, 4
        mov    eax, dword ptr [0x376c]       ; -> loc_1_376C
        mov    ecx, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    dword ptr [ebx + ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x376c], eax       ; -> loc_1_376C
        push   edi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x14], 0
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_356E
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_2350
        add    esp, 8
loc_1_0E3A:
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_0E53
        call   sub_1_0050
        mov    dword ptr [ebp - 0x10], eax
        or     eax, eax
        je     loc_1_0E60
        mov    dword ptr [ebp - 0x14], 0
loc_1_0E53:
        cmp    dword ptr [0x375c], 0         ; -> loc_1_375C
        jne    loc_1_0C41
loc_1_0E60:
        mov    dword ptr [0x3710], 0         ; -> loc_1_3710
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_0E8F
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_01B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0E83:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_01B0
        add    esp, 4
loc_1_0E8F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0EA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + 4]
        cmp    dword ptr [ecx + 0xc], 5
        jne    loc_1_0EC3
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0EC3:
        call   sub_1_0050
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    loc_1_0EE0
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_0EE0:
        cmp    dword ptr [0x3710], 0         ; -> loc_1_3710
        jne    loc_1_10E0
        mov    dword ptr [0x3710], 1         ; -> loc_1_3710
        cmp    dword ptr [0x375c], 0         ; -> loc_1_375C
        jne    loc_1_10D0
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0
        push   0
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_3654
        mov    eax, dword ptr [0x3770]       ; -> loc_1_3770
        cmp    dword ptr [ebp - 0xc], eax
        jge    loc_1_10D3
        cmp    dword ptr [0x376c], -1        ; -> loc_1_376C
        je     loc_1_10D3
        mov    eax, dword ptr [0x376c]       ; -> loc_1_376C
        mov    dword ptr [ebp - 4], eax
        mov    ebx, eax
        shl    ebx, 5
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    eax, dword ptr [ebx + eax]
        mov    dword ptr [0x376c], eax       ; -> loc_1_376C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_0A00
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [eax*2]
        lea    ecx, [ecx + ecx*8]
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [ecx + eax + 2], esi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x24], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x2c], eax
        add    esi, 0x24
        inc    dword ptr [edi + 0x126]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_3222
        add    esp, 0xc
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    esi, eax
        mov    eax, dword ptr [edi + 8]
        test   byte ptr [eax + 0x40], 0x80
        jne    loc_1_1072
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    eax, dword ptr [ebx + eax + 0x14]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        add    ecx, dword ptr [edi + 0x11c]
        and    eax, 0xffff
        add    eax, ecx
        test   byte ptr [edi + 0xfc], 0x20
        jne    loc_1_101B
        cmp    dword ptr [edi + 0x114], 1
        je     loc_1_1000
        test   byte ptr [eax], 1
        jne    loc_1_101B
        lea    ecx, [edi + 0x120]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    loc_1_0FFA
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
loc_1_0FFA:
        je     loc_1_101B
        jmp    loc_1_1072
        db  08Bh, 0FFh
loc_1_1000:
        test   byte ptr [eax], 0x80
        jne    loc_1_101B
        lea    ecx, [edi + 0x120]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    loc_1_1072
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [ecx + 4]
        jne    loc_1_1072
loc_1_101B:
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        lea    ecx, [ebx + eax]
        mov    ax, word ptr [ecx + 0xe]
        mov    word ptr [0x3718], ax         ; -> loc_1_3718
        cmp    word ptr [0x3718], 1          ; -> loc_1_3718
        mov    ax, word ptr [ecx + 0x12]
        mov    word ptr [0x371a], ax         ; -> loc_1_371A
        mov    eax, dword ptr [ecx + 0x14]
        mov    dword ptr [0x371c], eax       ; -> loc_1_371C
        jbe    loc_1_105B
        mov    ax, word ptr [ecx + 0x1a]
        mov    word ptr [0x3720], ax         ; -> loc_1_3720
        mov    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [0x3722], eax       ; -> loc_1_3722
loc_1_105B:
        push   0x3718                        ; -> loc_1_3718
        push   edi
        call   sub_1_26E0
        add    esp, 8
        push   edi
        call   sub_1_2530
        add    esp, 4
loc_1_1072:
        cmp    esi, 0x103
        je     loc_1_10AA
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0960
        add    esp, 4
        mov    eax, dword ptr [0x376c]       ; -> loc_1_376C
        mov    ecx, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    dword ptr [ebx + ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x376c], eax       ; -> loc_1_376C
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_01B0
        add    esp, 4
        jmp    loc_1_10B0
loc_1_10AA:
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [ecx + 0x14]
loc_1_10B0:
        mov    dword ptr [0x3710], 0         ; -> loc_1_3710
        call   sub_1_0BF0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_10D0:
        mov    esi, dword ptr [ebp + 0xc]
loc_1_10D3:
        mov    dword ptr [0x3710], 0         ; -> loc_1_3710
        jmp    loc_1_10E3
        db  090h
loc_1_10E0:
        mov    esi, dword ptr [ebp + 0xc]
loc_1_10E3:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_01B0
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 4
        inc    dword ptr [ecx + 0x14]
        mov    dword ptr [esi + 0x28], 0
        mov    dword ptr [esi + 0x24], ecx
        lea    eax, [esi + 0x24]
        cmp    dword ptr [0x3760], 0         ; -> loc_1_3760
        je     loc_1_1120
        mov    eax, dword ptr [0x3760]       ; -> loc_1_3760
        mov    dword ptr [eax + 0x28], esi
        mov    dword ptr [0x3760], esi       ; -> loc_1_3760
        jmp    loc_1_112C
        db  005h, 000h, 000h, 000h, 000h
loc_1_1120:
        mov    dword ptr [0x3760], esi       ; -> loc_1_3760
        mov    dword ptr [0x375c], esi       ; -> loc_1_375C
loc_1_112C:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1140:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], 0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x18], ecx
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    esi, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], esi
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, dword ptr [ecx + 0x118]
        or     esi, esi
        mov    dword ptr [ebp + 0x10], eax
        jne    loc_1_1186
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1186:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_3654
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_12B3
        mov    ecx, dword ptr [ebp - 0x18]
        mov    edi, dword ptr [ecx + 0x28]
        cmp    dword ptr [ecx + 0x3c], 0
        jne    loc_1_11DB
        mov    eax, dword ptr [ecx + 0x38]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   ecx
        call   sub_1_32E2
        add    esp, 0xc
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    ecx, dword ptr [ebp - 0x18]
        or     eax, eax
        mov    dword ptr [ecx + 0x3c], 1
        jne    loc_1_127B
loc_1_11DB:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ecx + 0x34]
        xor    ebx, ebx
        mov    bx, word ptr [eax]
        cmp    esi, ebx
        jae    loc_1_11EC
        mov    ebx, esi
loc_1_11EC:
        or     ebx, ebx
        jne    loc_1_1203
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1203:
        or     esi, esi
        je     loc_1_1279
loc_1_1207:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1231
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1284
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3678
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_369C
loc_1_1231:
        cmp    dword ptr [ebp - 4], ebx
        jbe    loc_1_1254
        or     ebx, ebx
        je     loc_1_1292
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_3399
        add    esp, 0xc
        sub    esi, ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
        sub    ebx, ebx
        jmp    loc_1_1275
loc_1_1254:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_3399
        add    esp, 0xc
        sub    esi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
loc_1_1275:
        or     esi, esi
        jne    loc_1_1207
loc_1_1279:
        sub    eax, eax
loc_1_127B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1284:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1292:
        mov    ecx, dword ptr [ebp - 0x18]
        pop    edi
        mov    eax, dword ptr [ecx + 0x34]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    dword ptr [ecx], eax
        mov    eax, 0xc0000001
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_12B3:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0xc]
        xor    eax, eax
        mov    ax, word ptr [edx]
        add    ecx, 4
        mov    dword ptr [ebp - 0x1c], eax
loc_1_12C4:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp - 0x1c]
        jae    loc_1_12F7
        mov    eax, dword ptr [ecx]
        mov    edx, eax
        and    edx, 0xffff0fff
        shr    edx, 0xc
        and    eax, 0xffff
        lea    edi, [edx + eax]
        xor    ebx, ebx
        inc    dword ptr [ebp - 0x10]
        mov    bx, word ptr [ecx - 2]
        add    ecx, 6
        cmp    dword ptr [ebp + 0x10], ebx
        jbe    loc_1_1310
        sub    dword ptr [ebp + 0x10], ebx
        jmp    loc_1_12C4
loc_1_12F7:
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_1_1310:
        add    edi, dword ptr [ebp + 0x10]
        sub    ebx, dword ptr [ebp + 0x10]
        cmp    esi, ebx
        jae    loc_1_131C
        mov    ebx, esi
loc_1_131C:
        or     esi, esi
        je     loc_1_1279
        mov    eax, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        lea    eax, [eax + eax*2]
        lea    eax, [edx + eax*2 + 4]
        mov    dword ptr [ebp - 0x14], eax
loc_1_1334:
        or     ebx, ebx
        jne    loc_1_1377
        mov    edx, dword ptr [ebp + 0xc]
        xor    eax, eax
        mov    ax, word ptr [edx]
        cmp    eax, dword ptr [ebp - 0x10]
        jbe    loc_1_13F8
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        lea    edi, [ecx + eax]
        mov    eax, dword ptr [ebp - 0x14]
        xor    ebx, ebx
        add    dword ptr [ebp - 0x14], 6
        mov    bx, word ptr [eax - 2]
        inc    dword ptr [ebp - 0x10]
        cmp    esi, ebx
        jae    loc_1_1377
        mov    ebx, esi
loc_1_1377:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_13A5
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1410
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3678
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_369C
loc_1_13A5:
        cmp    dword ptr [ebp - 4], ebx
        jbe    loc_1_13C4
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_3399
        add    esp, 0xc
        sub    esi, ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
        sub    ebx, ebx
        jmp    loc_1_13E5
loc_1_13C4:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_3399
        add    esp, 0xc
        sub    esi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
loc_1_13E5:
        or     esi, esi
        jne    loc_1_1334
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_13F8:
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        sub    dword ptr [ecx], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  005h, 000h, 000h, 000h, 000h
loc_1_1410:
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        sub    dword ptr [ecx], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1430:
        push   ebp
        mov    ebp, esp
        push   6
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3162
        add    esp, 0x14
        push   eax
        call   sub_1_01E0
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1460:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 4]
        cmp    dword ptr [esi + 0x24], 2
        jne    loc_1_1480
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1480:
        mov    dword ptr [esi + 0x24], 2
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x48]
        cmp    eax, ecx
        je     loc_1_149A
loc_1_1490:
        add    ecx, dword ptr [eax + 0x14]
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    loc_1_1490
loc_1_149A:
        or     ecx, ecx
        jg     loc_1_1536
        mov    byte ptr [esi + 0x1a], 1
        inc    dword ptr [esi + 0x1c]
        mov    edi, dword ptr [esi + 0x48]
        or     edi, edi
        je     loc_1_14D3
        mov    ebx, 1
loc_1_14B5:
        cmp    dword ptr [edi + 0xc], ebx
        jne    loc_1_14CF
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3592
        mov    edi, dword ptr [edi + 8]
loc_1_14CF:
        or     edi, edi
        jne    loc_1_14B5
loc_1_14D3:
        push   esi
        call   sub_1_0710
        add    esp, 4
        push   esi
        call   sub_1_1430
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_152D
        inc    dword ptr [esi + 0x1c]
        mov    edi, dword ptr [esi + 0x48]
        or     edi, edi
        je     loc_1_151D
loc_1_14F5:
        cmp    dword ptr [edi + 0xc], 1
        jne    loc_1_1519
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3592
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3598
        mov    edi, dword ptr [edi + 8]
loc_1_1519:
        or     edi, edi
        jne    loc_1_14F5
loc_1_151D:
        push   esi
        call   sub_1_0710
        add    esp, 4
        mov    dword ptr [esi + 0x24], 1
loc_1_152D:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1536:
        mov    byte ptr [esi + 0x1a], 0
        inc    dword ptr [esi + 0x1c]
        mov    edi, dword ptr [esi + 0x48]
        or     edi, edi
        je     loc_1_1570
        mov    ebx, 1
loc_1_1549:
        cmp    dword ptr [edi + 0xc], ebx
        jne    loc_1_156C
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3592
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3598
        mov    edi, dword ptr [edi + 8]
loc_1_156C:
        or     edi, edi
        jne    loc_1_1549
loc_1_1570:
        push   esi
        call   sub_1_0710
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1590:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        sub    ebx, ebx
        mov    word ptr [ebp - 2], 0
        sub    edx, edx
        mov    dword ptr [ebp - 0x10], edx
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 2
        jne    loc_1_15C0
        sub    esi, esi
        mov    ecx, dword ptr [ebp + 8]
        jmp    loc_1_15C8
        db  08Dh, 064h, 024h, 000h
loc_1_15C0:
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, ecx
        mov    ecx, dword ptr [ecx + 4]
loc_1_15C8:
        mov    edi, dword ptr [ebp + 0x10]
        cmp    edi, 0x20101
        ja     loc_1_15F0
        je     loc_1_1810
        sub    edi, 0x10101
        cmp    edi, 0x10
        ja     loc_1_1654
        shl    edi, 2
        jmp    loc_1_1988
        db  08Dh, 049h, 000h
loc_1_15F0:
        cmp    edi, 0x1010104
        ja     loc_1_1630
        je     loc_1_1900
        cmp    edi, 0x20102
        je     loc_1_1820
        cmp    edi, 0x1010101
        je     loc_1_182B
        cmp    edi, 0x1010102
        je     loc_1_1840
        cmp    edi, 0x1010103
        je     loc_1_1851
        jmp    loc_1_1654
loc_1_1630:
        cmp    edi, 0x2010101
        je     loc_1_182B
        cmp    edi, 0x2010102
        je     loc_1_1840
        cmp    edi, 0x2010103
        je     loc_1_1933
loc_1_1654:
        mov    ebx, 0xc00000bb
        mov    esi, dword ptr [ebp - 0x18]
        jmp    loc_1_1944
loc_1_1661:
        mov    esi, 0x378c                   ; -> loc_1_378C
        mov    edx, 0x64
        jmp    loc_1_1944
loc_1_1670:
        mov    eax, dword ptr [ecx + 0xc]
        mov    eax, dword ptr [eax + 6]
        and    al, 7
        cmp    al, 7
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_168A
        mov    dword ptr [ebp - 0x10], 4
loc_1_168A:
        lea    esi, [ebp - 0x10]
        jmp    loc_1_193F
loc_1_1692:
        mov    eax, dword ptr [ecx + 0x114]
        mov    dword ptr [ebp - 0x14], eax
        lea    esi, [ebp - 0x14]
        jmp    loc_1_193F
loc_1_16A3:
        mov    dword ptr [ebp - 8], 0xd0
        jmp    loc_1_193C
        db  090h
loc_1_16B0:
        mov    eax, dword ptr [0x3a8c]       ; -> obj1:0x3A8C
        sub    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 0x44]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_16CF
        mov    eax, dword ptr [ebp - 8]
loc_1_16CF:
        cmp    dword ptr [ecx + 0x114], 1
        je     loc_1_16E3
        sub    eax, dword ptr [ecx + 0x118]
        jmp    loc_1_1939
loc_1_16E3:
        sub    eax, 0xe
        jmp    loc_1_1939
        db  005h, 000h, 000h, 000h, 000h
loc_1_16F0:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, 0x64
        mov    eax, dword ptr [eax + 0x3c]
        sub    edx, edx
        div    ecx
        jmp    loc_1_1939
loc_1_1704:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 0x46]
        jmp    loc_1_1939
        db  090h
loc_1_1710:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 0x4c]
        jmp    loc_1_1939
        db  005h, 000h, 000h, 000h, 000h
loc_1_1720:
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 0x4a]
        and    eax, 0xffff
        jmp    loc_1_1939
loc_1_1731:
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 0x50]
        and    eax, 0xffff
        jmp    loc_1_1939
loc_1_1742:
        mov    dword ptr [ebp - 8], 0
        mov    edx, dword ptr [ecx + 8]
        add    edx, 0x52
        lea    eax, [ebp - 8]
        mov    cx, word ptr [edx]
        mov    word ptr [eax], cx
        mov    cl, byte ptr [edx + 2]
        mov    byte ptr [eax + 2], cl
        mov    esi, eax
        jmp    loc_1_193F
loc_1_1765:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 0x56]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        lea    esi, [ecx + eax]
        call   sub_1_34B7
        add    esi, eax
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edx, ecx
        jmp    loc_1_1944
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_17A0:
        mov    dword ptr [ebp - 8], 0
        cmp    eax, 2
        jne    loc_1_17B7
        mov    eax, dword ptr [ecx + 0xfc]
        jmp    loc_1_1939
loc_1_17B7:
        mov    eax, dword ptr [esi + 0x1c]
        jmp    loc_1_1939
        db  090h
loc_1_17C0:
        mov    eax, dword ptr [ecx + 0x110]
        jmp    loc_1_1939
        db  005h, 000h, 000h, 000h, 000h
loc_1_17D0:
        mov    word ptr [ebp - 2], 0x300
        lea    esi, [ebp - 2]
        mov    edx, 2
        jmp    loc_1_1944
loc_1_17E3:
        mov    eax, dword ptr [0x3a8c]       ; -> obj1:0x3A8C
        sub    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ax, word ptr [eax + 0x44]
        cmp    word ptr [ebp - 8], ax
        jbe    loc_1_193C
        and    eax, 0xffff
        jmp    loc_1_1939
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1810:
        mov    eax, dword ptr [ecx + 0x126]
        jmp    loc_1_1939
        db  005h, 000h, 000h, 000h, 000h
loc_1_1820:
        mov    eax, dword ptr [ecx + 0x12a]
        jmp    loc_1_1939
loc_1_182B:
        mov    eax, dword ptr [ecx + 8]
        xor    edx, edx
        lea    esi, [eax + 0x14]
        mov    dx, word ptr [eax + 0x12]
        jmp    loc_1_1944
        db  08Dh, 064h, 024h, 000h
loc_1_1840:
        mov    eax, dword ptr [ecx + 8]
        xor    edx, edx
        lea    esi, [eax + 0x24]
        mov    dx, word ptr [eax + 0x12]
        jmp    loc_1_1944
loc_1_1851:
        sub    edi, edi
        cmp    eax, 2
        jne    loc_1_1861
        mov    dword ptr [ebp - 0xc], 0xffffffff
        jmp    loc_1_1867
loc_1_1861:
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0xc], esi
loc_1_1867:
        mov    eax, dword ptr [ecx + 0xb8]
        or     eax, eax
        je     loc_1_1885
        lea    edx, [ecx + 0xbc]
loc_1_1877:
        mov    esi, dword ptr [ebp - 0xc]
        test   dword ptr [edx], esi
        je     loc_1_187F
        inc    edi
loc_1_187F:
        add    edx, 4
        dec    eax
        jne    loc_1_1877
loc_1_1885:
        lea    eax, [edi + edi*2]
        lea    esi, [eax*2]
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [eax]
        cmp    esi, edx
        jbe    loc_1_18AB
        mov    eax, dword ptr [ebp + 0x1c]
        sub    esi, edx
        mov    dword ptr [eax], esi
        mov    eax, 0xc0010014
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_18AB:
        sub    edx, esi
        mov    edi, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edx
        add    dword ptr [edi], esi
        sub    eax, eax
        cmp    dword ptr [ecx + 0xb8], eax
        jbe    loc_1_18F5
        lea    ebx, [ecx + 0x98]
        mov    edi, dword ptr [ebp + 0x14]
        lea    edx, [ecx + 0xbc]
loc_1_18D0:
        mov    esi, dword ptr [ebp - 0xc]
        test   dword ptr [edx], esi
        je     loc_1_18E6
        mov    esi, dword ptr [edx - 0x68]
        mov    dword ptr [edi], esi
        mov    si, word ptr [ebx]
        mov    word ptr [edi + 4], si
        add    edi, 6
loc_1_18E6:
        add    ebx, 2
        add    edx, 4
        inc    eax
        cmp    dword ptr [ecx + 0xb8], eax
        ja     loc_1_18D0
loc_1_18F5:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_1900:
        mov    eax, dword ptr [ecx + 8]
        mov    esi, dword ptr [eax + 0x38]
        call   sub_1_34B7
        mov    ecx, esi
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        add    eax, ecx
        and    esi, 0xffff
        mov    ax, word ptr [eax + esi]
        and    eax, 0xffff
        cmp    eax, 0x10
        jl     loc_1_1939
        mov    eax, 0x10
        jmp    loc_1_1939
loc_1_1933:
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 0x34]
loc_1_1939:
        mov    dword ptr [ebp - 8], eax
loc_1_193C:
        lea    esi, [ebp - 8]
loc_1_193F:
        mov    edx, 4
loc_1_1944:
        or     ebx, ebx
        jne    loc_1_197E
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [eax]
        cmp    edx, ecx
        jbe    loc_1_1966
        mov    eax, dword ptr [ebp + 0x1c]
        sub    edx, ecx
        mov    dword ptr [eax], edx
        mov    ebx, 0xc0010014
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1966:
        mov    edi, dword ptr [ebp + 0x14]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [ebp + 0x20]
        add    dword ptr [edi], edx
        sub    dword ptr [eax], edx
loc_1_197E:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
loc_1_1988:
        popal  
        push   ss
        add    byte ptr [eax], al
        jo     loc_1_19A4
        add    byte ptr [eax], al
        xchg   edx, eax
        push   ss
        add    byte ptr [eax], al
        xchg   edx, eax
        push   ss
        add    byte ptr [eax], al
        mov    dword ptr [0xb0000016], eax   ; -> loc_1_16B0
        push   ss
        add    byte ptr [eax], al
        dd  loc_1_16F0               ; fixup
loc_1_19A4:
        add    al, 0x17
        add    byte ptr [eax], al
        adc    byte ptr [edi], dl
        add    byte ptr [eax], al
        and    byte ptr [edi], dl
        add    byte ptr [eax], al
        xor    dword ptr [edi], edx
        add    byte ptr [eax], al
        inc    edx
        pop    ss
        add    byte ptr [eax], al
        pop    ss
        add    byte ptr [eax], al
        mov    al, byte ptr [0xc0000017]     ; -> loc_1_17C0
        pop    ss
        add    byte ptr [eax], al
        rcl    byte ptr [edi]
        add    byte ptr [eax], al
        jecxz  0x19e1
        add    byte ptr [eax], al
        lea    esp, [esp]
sub_1_19D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ecx + 0x20]
        push   eax
        lea    eax, [ecx + 0x24]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ecx + 0x18]
        push   eax
        mov    eax, dword ptr [ecx + 0x14]
        push   eax
        mov    eax, dword ptr [ecx + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1590
        mov    esp, ebp
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 4]
        call   sub_1_0050
        mov    esi, eax
        lea    eax, [esi*2]
        mov    ecx, dword ptr [ebp + 0xc]
        lea    ebx, [eax + eax*8]
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [ebx + eax + 6], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], 1
        mov    ecx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [ebx + ecx + 0xe], eax
        mov    ax, word ptr [ebp + 0x14]
        push   eax
        push   0
        mov    ax, word ptr [ebp + 0x10]
        push   eax
        push   esi
        push   edi
        call   sub_1_3162
        add    esp, 0x14
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0x103
        je     loc_1_1A96
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1AA9
        push   esi
        call   sub_1_01B0
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1A96:
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [ebx + eax + 0xe], 0
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x14]
loc_1_1AA9:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AC0:
        push   ebp
        mov    ebp, esp
        sub    ax, ax
        mov    ecx, dword ptr [ebp + 8]
        test   ecx, 0xe007
        je     loc_1_1AD5
        mov    ax, 1
loc_1_1AD5:
        test   cl, 8
        je     loc_1_1ADE
        or     ax, 2
loc_1_1ADE:
        test   cl, 0x20
        je     loc_1_1AE7
        or     ax, 4
loc_1_1AE7:
        test   cl, 0x10
        je     loc_1_1AF0
        or     ax, 8
loc_1_1AF0:
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 4]
        call   sub_1_0050
        mov    dword ptr [ebp - 4], eax
        add    eax, eax
        mov    ecx, dword ptr [0x3784]       ; -> loc_1_3784
        lea    ebx, [eax + eax*8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + ecx + 6], eax
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x1c], eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_0300
        add    esp, 4
        mov    dword ptr [edi + 0xfc], eax
        push   eax
        call   sub_1_1AC0
        add    esp, 4
        mov    dword ptr [ebp - 8], 1
        lea    edx, [ebp - 8]
        mov    ecx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [ebx + ecx + 0xe], edx
        push   7
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        call   sub_1_3162
        add    esp, 0x14
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0x103
        je     loc_1_1BA0
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1BB0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_01B0
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1BA0:
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [ebx + eax + 0xe], 0
        inc    dword ptr [esi + 0x14]
loc_1_1BB0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1BC0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    ecx, dword ptr [ecx + 0x48]
        cmp    ecx, eax
        je     loc_1_1BDA
        nop    
loc_1_1BD0:
        or     eax, dword ptr [ecx + 0x20]
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    loc_1_1BD0
loc_1_1BDA:
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_1BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + 4]
        call   sub_1_0050
        mov    esi, eax
        lea    eax, [esi*2]
        mov    ecx, dword ptr [0x3784]       ; -> loc_1_3784
        lea    ebx, [eax + eax*8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + ecx + 6], eax
        mov    eax, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 0x20], eax
        push   edi
        call   sub_1_1BC0
        add    esp, 4
        mov    dword ptr [edi + 0x100], eax
        mov    eax, dword ptr [edi + 0x104]
        mov    ecx, dword ptr [0x3730]       ; -> loc_1_3730
        or     eax, dword ptr [edi + 0x100]
        mov    dword ptr [ecx], eax
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [ebx + eax + 0xe], ecx
        push   0xd
        mov    eax, dword ptr [0x372c]       ; -> loc_1_372C
        push   eax
        push   0
        push   esi
        push   edi
        call   sub_1_3162
        add    esp, 0x14
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0x103
        je     loc_1_1C90
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1CA3
        push   esi
        call   sub_1_01B0
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
loc_1_1C90:
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + eax + 0xe], 0
        inc    dword ptr [ecx + 0x14]
loc_1_1CA3:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_1CB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        cmp    byte ptr [0x3738], 0          ; -> loc_1_3738
        jne    loc_1_1CF0
        mov    byte ptr [0x3738], 1          ; -> loc_1_3738
        push   ecx
        push   eax
        call   sub_1_1BE0
        add    esp, 8
        mov    esi, eax
        cmp    esi, 0x103
        je     loc_1_1CE5
        call   sub_1_1F90
loc_1_1CE5:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1CF0:
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx], eax
        inc    dword ptr [eax + 0x14]
        cmp    dword ptr [0x3740], 0         ; -> loc_1_3740
        je     loc_1_1D20
        mov    eax, dword ptr [0x3740]       ; -> loc_1_3740
        pop    esi
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [0x3740], ecx       ; -> loc_1_3740
        mov    eax, 0x103
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_1D20:
        mov    dword ptr [0x3740], ecx       ; -> loc_1_3740
        mov    dword ptr [0x373c], ecx       ; -> loc_1_373C
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D40:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [eax + 4]
        call   sub_1_0050
        mov    dword ptr [ebp - 0x10], eax
        add    eax, eax
        mov    edi, dword ptr [ebp + 0xc]
        lea    ecx, [eax + eax*8]
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    dword ptr [ecx + eax + 6], edi
loc_1_1D72:
        cmp    dword ptr [edi + 8], 0
        jne    loc_1_1DF0
        mov    eax, dword ptr [esi + 8]
        mov    ebx, dword ptr [eax + 0x38]
        call   sub_1_34B7
        mov    ecx, ebx
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        add    eax, ecx
        and    ebx, 0xffff
        add    eax, ebx
        cmp    word ptr [eax + 2], 0
        je     loc_1_1DE4
        add    eax, 4
        mov    edx, dword ptr [0x3730]       ; -> loc_1_3730
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        mov    edx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [edx + ecx + 0xe], eax
        push   9
        mov    eax, dword ptr [0x372c]       ; -> loc_1_372C
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   esi
        call   sub_1_3162
        add    esp, 0x14
        jmp    loc_1_1E4D
loc_1_1DE4:
        mov    dword ptr [edi + 8], 1
        jmp    loc_1_1E59
        db  08Dh, 049h, 000h
loc_1_1DF0:
        mov    eax, dword ptr [edi + 0x20]
        cmp    dword ptr [edi + 0x1c], eax
        jbe    loc_1_1E63
        mov    edx, dword ptr [edi + 0x18]
        add    edx, eax
        mov    ecx, dword ptr [0x3730]       ; -> loc_1_3730
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ecx + 4], ax
        lea    eax, [ebp - 4]
        add    dword ptr [edi + 0x20], 6
        mov    dword ptr [ebp - 4], 0
        mov    edx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [edx + ecx + 0xe], eax
        push   8
        mov    eax, dword ptr [0x372c]       ; -> loc_1_372C
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   esi
        call   sub_1_3162
        add    esp, 0x14
        and    eax, 0xffff
        cmp    eax, 6
        jbe    loc_1_1E4D
        sub    eax, eax
loc_1_1E4D:
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
loc_1_1E59:
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1D72
loc_1_1E63:
        cmp    dword ptr [ebp - 8], 0x103
        je     loc_1_1E82
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_01B0
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1E82:
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + ecx + 0xe], 0
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1EB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        or     ebx, ebx
        mov    esi, dword ptr [edi + 4]
        je     loc_1_1EE8
        push   edi
        push   esi
        call   sub_1_0320
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x18]
        push   eax
        mov    ecx, 6
        mov    eax, dword ptr [ebx + 0x1c]
        sub    edx, edx
        div    ecx
        push   eax
        push   edi
        push   esi
        call   sub_1_0390
        add    esp, 0x10
loc_1_1EE8:
        push   ebx
        push   edi
        push   esi
        call   sub_1_0440
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        jne    loc_1_1F03
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_1F03:
        cmp    byte ptr [0x3738], 0          ; -> loc_1_3738
        jne    loc_1_1F40
        mov    byte ptr [0x3738], 1          ; -> loc_1_3738
        push   esi
        push   edi
        call   sub_1_1D40
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, 0x103
        je     loc_1_1F40
        push   0
        push   esi
        call   sub_1_3456
        add    esp, 8
        call   sub_1_1F90
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_1F40:
        mov    dword ptr [esi + 4], 0
        mov    dword ptr [esi], edi
        inc    dword ptr [edi + 0x14]
        cmp    dword ptr [0x3740], 0         ; -> loc_1_3740
        je     loc_1_1F70
        mov    eax, dword ptr [0x3740]       ; -> loc_1_3740
        pop    edi
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [0x3740], esi       ; -> loc_1_3740
        mov    eax, 0x103
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
loc_1_1F70:
        mov    dword ptr [0x3740], esi       ; -> loc_1_3740
        mov    dword ptr [0x373c], esi       ; -> loc_1_373C
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1F90:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x373c], 0         ; -> loc_1_373C
        jne    loc_1_1FA7
        mov    byte ptr [0x3738], 0          ; -> loc_1_3738
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1FA7:
        mov    ebx, dword ptr [0x373c]       ; -> loc_1_373C
        mov    esi, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [esi + 4]
        or     eax, eax
        mov    dword ptr [0x373c], eax       ; -> loc_1_373C
        jne    loc_1_1FC8
        mov    dword ptr [0x3740], 0         ; -> loc_1_3740
loc_1_1FC8:
        cmp    dword ptr [edi + 0x24], 1
        jne    loc_1_2035
        cmp    dword ptr [ebx + 0x10], 1
        je     loc_1_1FE0
        mov    eax, 0xc00000bb
        jmp    loc_1_2043
        db  005h, 000h, 000h, 000h, 000h
loc_1_1FE0:
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1010103
        je     loc_1_1FF8
        cmp    eax, 0x2010103
        je     loc_1_2029
        mov    eax, 0xc00000bb
        jmp    loc_1_2043
loc_1_1FF8:
        push   ebx
        push   esi
        call   sub_1_1D40
        add    esp, 8
        cmp    eax, 0x103
        jne    loc_1_204E
        mov    ecx, dword ptr [0x373c]       ; -> loc_1_373C
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [0x373c], ebx       ; -> loc_1_373C
        cmp    dword ptr [0x3740], 0         ; -> loc_1_3740
        jne    loc_1_2043
        mov    dword ptr [0x3740], ebx       ; -> loc_1_3740
        jmp    loc_1_2043
loc_1_2029:
        push   ebx
        push   esi
        call   sub_1_1BE0
        add    esp, 8
        jmp    loc_1_2043
loc_1_2035:
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   0x2d0
        add    esp, 4
loc_1_2043:
        cmp    eax, 0x103
        je     loc_1_20DA
loc_1_204E:
        mov    ecx, dword ptr [ebx + 0x14]
        cmp    ecx, 0x1010103
        je     loc_1_2063
        cmp    ecx, 0x2010103
        je     loc_1_2082
        jmp    loc_1_20B3
loc_1_2063:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 1
        jne    loc_1_207B
        push   eax
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_3580
        jmp    loc_1_20B3
loc_1_207B:
        cmp    ecx, 4
        jne    loc_1_20B3
        jmp    loc_1_209C
loc_1_2082:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 1
        jne    loc_1_2097
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_3580
        jmp    loc_1_20B3
loc_1_2097:
        cmp    ecx, 3
        jne    loc_1_20B3
loc_1_209C:
        push   1
        push   esi
        push   edi
        call   sub_1_0900
        add    esp, 0xc
        push   0
        push   ebx
        call   sub_1_3456
        add    esp, 8
loc_1_20B3:
        cmp    dword ptr [0x373c], 0         ; -> loc_1_373C
        jne    loc_1_20C3
        mov    byte ptr [0x3738], 0          ; -> loc_1_3738
loc_1_20C3:
        push   esi
        push   edi
        call   sub_1_2350
        add    esp, 8
        cmp    dword ptr [0x373c], 0         ; -> loc_1_373C
        jne    loc_1_1FA7
loc_1_20DA:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
sub_1_20E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ecx + 4]
        mov    eax, dword ptr [edx + 0x18]
        mov    ebx, dword ptr [edx + 0x14]
        mov    edi, dword ptr [eax]
        sub    eax, eax
        cmp    ebx, 0x1010e
        je     loc_1_2123
        cmp    ebx, 0x1010f
        je     loc_1_2133
        cmp    ebx, 0x1010103
        je     loc_1_2169
        cmp    ebx, 0x2010103
        je     loc_1_2178
        mov    eax, 0xc00000bb
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2123:
        push   edi
        push   edx
        push   ecx
        call   sub_1_1B00
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2133:
        cmp    edi, 0x100
        jbe    loc_1_2145
        mov    eax, 0xc0010015
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2145:
        cmp    dword ptr [esi + 0x110], edi
        jae    loc_1_2182
        push   0xe
        push   edi
        push   edx
        push   ecx
        call   sub_1_1A10
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_2182
        mov    dword ptr [esi + 0x110], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2169:
        push   edx
        push   ecx
        call   sub_1_1EB0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2178:
        push   edx
        push   ecx
        call   sub_1_1CB0
        add    esp, 8
loc_1_2182:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2190:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        je     loc_1_21C0
        cmp    ecx, 2
        je     loc_1_21C0
        cmp    ecx, 1
        jne    loc_1_21B7
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_20E0
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_21B7:
        mov    eax, 0x10001
        pop    ebp
        ret    8
loc_1_21C0:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19D0
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_21D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x3748]       ; -> loc_1_3748
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_3550
        push   0
        mov    eax, dword ptr [0x3728]       ; -> loc_1_3728
        push   eax
        call   sub_1_3544
        push   0
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        push   eax
        call   sub_1_3456
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2210:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    ecx, ecx
        mov    esi, dword ptr [0x3754]       ; -> loc_1_3754
        cmp    esi, ecx
        je     loc_1_2230
        mov    eax, dword ptr [ebp + 8]
loc_1_2223:
        cmp    esi, eax
        je     loc_1_2230
        mov    ecx, esi
        mov    esi, dword ptr [esi + 0x44]
        or     esi, esi
        jne    loc_1_2223
loc_1_2230:
        or     esi, esi
        je     loc_1_2272
        or     ecx, ecx
        mov    eax, dword ptr [esi + 0x44]
        je     loc_1_2240
        mov    dword ptr [ecx + 0x44], eax
        jmp    loc_1_2245
loc_1_2240:
        mov    dword ptr [0x3754], eax       ; -> loc_1_3754
loc_1_2245:
        mov    eax, dword ptr [esi]
        mov    word ptr [eax*2 + 0x3a98], 0  ; -> obj1:0x3A98
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax*4 + 0x3ab8], 0 ; -> obj1:0x3AB8
        mov    eax, dword ptr [esi + 0x40]
        push   eax
        call   sub_1_355C
        push   0
        push   esi
        call   sub_1_3456
        add    esp, 8
loc_1_2272:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2280:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0xc], 5
        jne    loc_1_22B4
        mov    edi, dword ptr [eax + 0x10]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_06B0
        add    esp, 8
        or     eax, eax
        jne    loc_1_2347
        push   0
        push   edi
        call   sub_1_3568
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_22B4:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x24], 2
        jne    loc_1_2347
        cmp    byte ptr [esi + 0x1a], 0
        jne    loc_1_2347
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x48]
        cmp    eax, ecx
        je     loc_1_22DE
loc_1_22D4:
        add    ecx, dword ptr [eax + 0x14]
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    loc_1_22D4
loc_1_22DE:
        or     ecx, ecx
        jg     loc_1_2347
        push   esi
        call   sub_1_1430
        add    esp, 4
        or     eax, eax
        jne    loc_1_2300
        mov    byte ptr [esi + 0x1a], 1
        mov    dword ptr [esi + 0x24], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  090h
loc_1_2300:
        inc    dword ptr [esi + 0x1c]
        mov    edi, dword ptr [esi + 0x48]
        or     edi, edi
        je     loc_1_2337
        mov    ebx, 1
        nop    
loc_1_2310:
        cmp    dword ptr [edi + 0xc], ebx
        jne    loc_1_2330
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3592
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3598
loc_1_2330:
        mov    edi, dword ptr [edi + 8]
        or     edi, edi
        jne    loc_1_2310
loc_1_2337:
        push   esi
        call   sub_1_0710
        add    esp, 4
        mov    dword ptr [esi + 0x24], 1
loc_1_2347:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_2350:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x14]
        dec    ecx
        mov    dword ptr [eax + 0x14], ecx
        or     ecx, ecx
        jg     loc_1_236E
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2280
        add    esp, 8
loc_1_236E:
        pop    ebp
        ret    
sub_1_2370:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        add    eax, eax
        sub    edi, edi
        lea    ecx, [eax + eax*8]
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    ebx, dword ptr [ecx + eax + 6]
        mov    dword ptr [ebp - 0xc], ecx
        mov    esi, dword ptr [ebx]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp + 0xc]
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff
        sub    ecx, 7
        cmp    ecx, 7
        ja     loc_1_24C9
        jmp    dword ptr [ecx*4 + 0x2508]    ; -> obj1:0x2508
loc_1_23C8:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 1
        jne    loc_1_23E0
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_3580
        jmp    loc_1_24C9
loc_1_23E0:
        cmp    ecx, 2
        jne    loc_1_24C9
        push   1
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ebx
        call   sub_1_0850
        add    esp, 0x10
        jmp    loc_1_24C9
        db  08Bh, 0FFh
loc_1_2400:
        or     eax, eax
        je     loc_1_24C4
        mov    edx, dword ptr [0x3740]       ; -> loc_1_3740
        mov    ecx, dword ptr [0x373c]       ; -> loc_1_373C
        mov    dword ptr [edx + 4], ebx
        cmp    dword ptr [0x373c], ebx       ; -> loc_1_373C
        jne    loc_1_2428
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [0x373c], edx       ; -> loc_1_373C
loc_1_2428:
        cmp    ecx, ebx
        je     loc_1_2439
        lea    esp, [esp]
loc_1_2430:
        mov    edi, ecx
        mov    ecx, dword ptr [ecx + 4]
        cmp    ecx, ebx
        jne    loc_1_2430
loc_1_2439:
        mov    edx, dword ptr [0x3740]       ; -> loc_1_3740
        mov    dword ptr [edx + 4], 0
        cmp    dword ptr [0x3740], edi       ; -> loc_1_3740
        je     loc_1_246F
        cmp    dword ptr [0x3740], ecx       ; -> loc_1_3740
        jne    loc_1_2469
        mov    dword ptr [0x3740], edi       ; -> loc_1_3740
        or     edi, edi
        je     loc_1_246F
        mov    dword ptr [edi + 4], 0
        jmp    loc_1_246F
loc_1_2469:
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [edi + 4], ecx
loc_1_246F:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 1
        jne    loc_1_2487
        push   eax
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_3580
        jmp    loc_1_24B9
loc_1_2487:
        cmp    ecx, 4
        jne    loc_1_24C4
        jmp    loc_1_24AA
        db  08Bh, 0FFh
loc_1_2490:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 1
        jne    loc_1_24A5
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_3580
        jmp    loc_1_24C4
loc_1_24A5:
        cmp    ecx, 3
        jne    loc_1_24C4
loc_1_24AA:
        push   1
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0900
        add    esp, 0xc
loc_1_24B9:
        push   0
        push   ebx
        call   sub_1_3456
        add    esp, 8
loc_1_24C4:
        call   sub_1_1F90
loc_1_24C9:
        mov    eax, dword ptr [0x3784]       ; -> loc_1_3784
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + ecx + 0xe]
        or     eax, eax
        jne    loc_1_24DB
        jmp    loc_1_24E6
loc_1_24DB:
        cmp    dword ptr [eax], 0
        je     loc_1_24F2
        mov    dword ptr [eax], 0
loc_1_24E6:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_01B0
        add    esp, 4
loc_1_24F2:
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2350
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_23C8
        dd  loc_1_2400
        dd  loc_1_2400
        dd  loc_1_23C8
        dd  loc_1_23C8
        dd  loc_1_24C9
        dd  loc_1_2490
        dd  loc_1_23C8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2530:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x1b], 0
        je     loc_1_2598
        mov    byte ptr [esi + 0x1b], 0
        inc    dword ptr [esi + 0x1c]
        mov    edi, dword ptr [esi + 0x48]
        or     edi, edi
        je     loc_1_258F
        lea    esp, [esp]
loc_1_2550:
        cmp    dword ptr [edi + 0xc], 1
        jne    loc_1_2588
        mov    eax, dword ptr [edi + 0x28]
        test   al, 1
        je     loc_1_256C
        and    eax, 0xfffffffe
        mov    dword ptr [edi + 0x28], eax
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_358C
loc_1_256C:
        cmp    dword ptr [edi + 0xc], 1
        jne    loc_1_2588
        mov    eax, dword ptr [edi + 0x28]
        test   al, 2
        je     loc_1_2588
        and    eax, 0xfffffffd
        mov    dword ptr [edi + 0x28], eax
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_358C
loc_1_2588:
        mov    edi, dword ptr [edi + 8]
        or     edi, edi
        jne    loc_1_2550
loc_1_258F:
        push   esi
        call   sub_1_0710
        add    esp, 4
loc_1_2598:
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_25A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0xc0000001
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_1_25D0
        cmp    eax, 3
        je     loc_1_2637
        cmp    eax, 5
        je     loc_1_25E5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_25D0:
        mov    edi, 0x40010006
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    dword ptr [ebp - 4], eax
        mov    ebx, dword ptr [ebp + 8]
        jmp    loc_1_25F4
loc_1_25E5:
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, 0x40010005
        mov    dword ptr [ebx + 0x24], 1
loc_1_25F4:
        inc    dword ptr [ebx + 0x1c]
        mov    esi, dword ptr [ebx + 0x48]
        or     esi, esi
        je     loc_1_262E
        mov    edi, edi
loc_1_2600:
        cmp    dword ptr [esi + 0xc], 1
        jne    loc_1_261F
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_3592
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_3598
loc_1_261F:
        or     dword ptr [esi + 0x28], 2
        mov    byte ptr [ebx + 0x1b], 1
        mov    esi, dword ptr [esi + 8]
        or     esi, esi
        jne    loc_1_2600
loc_1_262E:
        push   ebx
        call   sub_1_0710
        add    esp, 4
loc_1_2637:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
sub_1_2640:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ax, word ptr [ebp + 0xc]
        push   eax
        call   sub_1_01E0
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 8]
        and    edx, 0xffff
        lea    eax, [edx*2]
        push   edx
        lea    eax, [eax + eax*8]
        mov    ecx, dword ptr [0x3784]       ; -> loc_1_3784
        mov    edi, dword ptr [eax + ecx + 2]
        mov    esi, dword ptr [edi + 0x24]
        lea    ebx, [edi + 0x24]
        call   sub_1_01B0
        add    esp, 4
        mov    eax, dword ptr [ebx + 8]
        push   eax
        call   sub_1_0960
        add    esp, 4
        mov    ecx, dword ptr [0x376c]       ; -> loc_1_376C
        mov    edx, dword ptr [ebx + 8]
        shl    edx, 5
        mov    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    dword ptr [edx + eax], ecx
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [0x376c], eax       ; -> loc_1_376C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_1_356E
        push   esi
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_2350
        add    esp, 8
        call   sub_1_0BF0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_26E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    ecx, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x12a]
        mov    ax, word ptr [ecx]
        or     ax, ax
        je     loc_1_2723
        mov    edx, ecx
        add    edx, 2
        and    eax, 0xffff
loc_1_2715:
        xor    ecx, ecx
        mov    cx, word ptr [edx]
        add    edx, 6
        add    dword ptr [ebp - 4], ecx
        dec    eax
        jne    loc_1_2715
loc_1_2723:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        add    ecx, eax
        mov    dword ptr [ebp - 0x10], ecx
        mov    eax, dword ptr [esi + 0x114]
        cmp    eax, 1
        jne    loc_1_2763
        mov    edx, 0xe
        test   byte ptr [ecx + 8], 0x80
        je     loc_1_275D
        mov    dl, byte ptr [ecx + 0xe]
        and    edx, 0x1f
        add    edx, 0xe
loc_1_275D:
        mov    dword ptr [esi + 0x118], edx
loc_1_2763:
        mov    eax, dword ptr [esi + 0x118]
        mov    dword ptr [ebp - 0x14], eax
        add    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x18], eax
        mov    ecx, dword ptr [ebp + 0xc]
        xor    eax, eax
        mov    ax, word ptr [ecx + 2]
        sub    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, dword ptr [ebp - 0x14]
        lea    edx, [esi + 0x120]
        mov    dword ptr [ebp - 4], eax
        mov    edi, dword ptr [esi + 0x11c]
        add    edi, dword ptr [ebp - 0x10]
        inc    dword ptr [esi + 0x1c]
        lea    ecx, [edi + 6]
        mov    dword ptr [ebp - 0xc], 0x20
        mov    eax, dword ptr [edi]
        cmp    eax, dword ptr [edx]
        jne    loc_1_27C0
        mov    ax, word ptr [edi + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_1_27C0
        mov    dword ptr [ebp - 0xc], 0x21
        jmp    loc_1_2836
        db  090h
loc_1_27C0:
        mov    edx, 0x3af8                   ; -> obj1:0x3AF8
        mov    eax, dword ptr [edi]
        cmp    eax, dword ptr [edx]
        jne    loc_1_27E0
        mov    ax, word ptr [edi + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_1_27E0
        mov    dword ptr [ebp - 0xc], 0x28
        jmp    loc_1_2836
        db  08Bh, 0FFh
loc_1_27E0:
        cmp    dword ptr [esi + 0x114], 1
        je     loc_1_2815
        test   byte ptr [edi], 1
        je     loc_1_2836
        mov    dword ptr [ebp - 0xc], 0x24
        push   edi
        mov    eax, dword ptr [esi + 0xb8]
        push   eax
        lea    eax, [esi + 0x98]
        push   eax
        lea    eax, [esi + 0x54]
        push   eax
        call   sub_1_33CA
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_2836
loc_1_2815:
        mov    al, byte ptr [edi]
        test   al, 0x80
        je     loc_1_282D
        mov    dword ptr [ebp - 0xc], 0x1020
        test   al, 0x40
        je     loc_1_282D
        mov    dword ptr [ebp - 0xc], 0x3020
loc_1_282D:
        test   byte ptr [ecx], 0x80
        je     loc_1_2836
        or     dword ptr [ebp - 0xc], 0x10
loc_1_2836:
        mov    ebx, dword ptr [esi + 0x48]
        or     ebx, ebx
        je     loc_1_28AF
loc_1_2840:
        cmp    dword ptr [ebx + 0xc], 1
        jne    loc_1_28A8
        mov    eax, dword ptr [ebp - 0xc]
        test   dword ptr [ebx + 0x1c], eax
        jne    loc_1_287B
        mov    ecx, dword ptr [esi + 0x114]
        cmp    ecx, 1
        je     loc_1_2873
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_286E
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + eax*4 + 0xb8]
        test   dword ptr [ebx + 0x18], eax
        jne    loc_1_287B
loc_1_286E:
        cmp    ecx, 1
        jne    loc_1_28A8
loc_1_2873:
        mov    eax, dword ptr [edi + 2]
        test   dword ptr [ebx + 0x20], eax
        je     loc_1_28A8
loc_1_287B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_3586
        mov    byte ptr [esi + 0x1b], 1
        or     dword ptr [ebx + 0x28], 1
loc_1_28A8:
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    loc_1_2840
loc_1_28AF:
        push   esi
        call   sub_1_0710
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
sub_1_28C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        inc    dword ptr [esi + 0x12a]
        mov    dword ptr [esi + 0x3c], 0
        mov    edi, dword ptr [ebp + 0x14]
        or     edi, edi
        jne    loc_1_28ED
        mov    edi, dword ptr [0x3a90]       ; -> obj1:0x3A90
loc_1_28ED:
        mov    eax, dword ptr [esi + 0x114]
        cmp    eax, 1
        jne    loc_1_2915
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, 0xe
        test   byte ptr [ecx + 8], 0x80
        je     loc_1_290F
        mov    dl, byte ptr [ecx + 0xe]
        and    edx, 0x1f
        add    edx, 0xe
loc_1_290F:
        mov    dword ptr [esi + 0x118], edx
loc_1_2915:
        mov    edx, dword ptr [esi + 0x118]
        mov    dword ptr [ebp - 0xc], edx
        mov    eax, edx
        add    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, edx
        mov    dword ptr [ebp - 0x14], eax
        sub    edi, edx
        mov    dword ptr [ebp - 0x18], edi
        mov    dword ptr [esi + 0x3c], 0
        mov    edi, dword ptr [esi + 0x11c]
        lea    edx, [esi + 0x120]
        add    edi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x1c]
        lea    ecx, [edi + 6]
        mov    dword ptr [ebp - 8], 0x20
        mov    eax, dword ptr [edi]
        cmp    eax, dword ptr [edx]
        jne    loc_1_2970
        mov    ax, word ptr [edi + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_1_2970
        mov    dword ptr [ebp - 8], 0x21
        jmp    loc_1_29E6
        db  090h
loc_1_2970:
        mov    edx, 0x3af8                   ; -> obj1:0x3AF8
        mov    eax, dword ptr [edi]
        cmp    eax, dword ptr [edx]
        jne    loc_1_2990
        mov    ax, word ptr [edi + 4]
        cmp    ax, word ptr [edx + 4]
        jne    loc_1_2990
        mov    dword ptr [ebp - 8], 0x28
        jmp    loc_1_29E6
        db  08Bh, 0FFh
loc_1_2990:
        cmp    dword ptr [esi + 0x114], 1
        je     loc_1_29C5
        test   byte ptr [edi], 1
        je     loc_1_29E6
        mov    dword ptr [ebp - 8], 0x24
        push   edi
        mov    eax, dword ptr [esi + 0xb8]
        push   eax
        lea    eax, [esi + 0x98]
        push   eax
        lea    eax, [esi + 0x54]
        push   eax
        call   sub_1_33CA
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_29E6
loc_1_29C5:
        mov    al, byte ptr [edi]
        test   al, 0x80
        je     loc_1_29DD
        mov    dword ptr [ebp - 8], 0x1020
        test   al, 0x40
        je     loc_1_29DD
        mov    dword ptr [ebp - 8], 0x3020
loc_1_29DD:
        test   byte ptr [ecx], 0x80
        je     loc_1_29E6
        or     dword ptr [ebp - 8], 0x10
loc_1_29E6:
        mov    ebx, dword ptr [esi + 0x48]
        or     ebx, ebx
        je     loc_1_2A5D
loc_1_29F0:
        cmp    dword ptr [ebx + 0xc], 1
        jne    loc_1_2A56
        mov    eax, dword ptr [ebp - 8]
        test   dword ptr [ebx + 0x1c], eax
        jne    loc_1_2A2B
        mov    ecx, dword ptr [esi + 0x114]
        cmp    ecx, 1
        je     loc_1_2A23
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2A1E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + eax*4 + 0xb8]
        test   dword ptr [ebx + 0x18], eax
        jne    loc_1_2A2B
loc_1_2A1E:
        cmp    ecx, 1
        jne    loc_1_2A56
loc_1_2A23:
        mov    eax, dword ptr [edi + 2]
        test   dword ptr [ebx + 0x20], eax
        je     loc_1_2A56
loc_1_2A2B:
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_3586
        mov    byte ptr [esi + 0x1b], 1
        or     dword ptr [ebx + 0x28], 1
loc_1_2A56:
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    loc_1_29F0
loc_1_2A5D:
        push   esi
        call   sub_1_0710
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
loc_1_2A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_2E10
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    loc_1_2AA0
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
loc_1_2AA0:
        push   0
        push   0x12e
        call   sub_1_3414
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_1_2DF7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 0x24], 0
        mov    byte ptr [esi + 0x1a], 0
        mov    dword ptr [esi + 0x3c], 0
        mov    byte ptr [esi + 0x1b], 0
        mov    dword ptr [esi + 0x1c], 0
        mov    dword ptr [esi + 0x110], 0x40
        mov    dword ptr [esi + 0xb8], 0
        mov    dword ptr [esi + 0x48], 0
        mov    dword ptr [esi + 0x4c], 0
        lea    edx, [eax*4]
        mov    dword ptr [esi + 0x50], 0
        mov    dword ptr [esi + 0xfc], 0
        mov    dword ptr [esi + 0x100], 0
        mov    dword ptr [esi + 0x108], 0xffffffff
        mov    dword ptr [esi + 0x126], 0
        mov    dword ptr [esi + 0x12a], 0
        mov    ecx, dword ptr [edx + 0x3a36] ; -> obj1:0x3A36
        mov    eax, ecx
        and    eax, 0xffff0fff
        shr    eax, 0xc
        and    ecx, 0xffff
        add    eax, ecx
        add    eax, dword ptr [0x3714]       ; -> loc_1_3714
        mov    dword ptr [esi + 4], eax
        mov    cx, word ptr [eax + 0x20]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax*2 + 0x3a98], cx ; -> obj1:0x3A98
        mov    dword ptr [edx + 0x3ab8], esi ; -> obj1:0x3AB8
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [eax + 0x28]
        mov    eax, ecx
        and    eax, 0xffff0fff
        shr    eax, 0xc
        and    ecx, 0xffff
        add    eax, ecx
        add    eax, dword ptr [0x3714]       ; -> loc_1_3714
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [eax + 0x38]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        add    ecx, eax
        add    ecx, dword ptr [0x3714]       ; -> loc_1_3714
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esi + 0x14], ecx
        mov    eax, dword ptr [eax + 0x2c]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        add    ecx, eax
        add    ecx, dword ptr [0x3714]       ; -> loc_1_3714
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x30]
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        add    ecx, eax
        add    ecx, dword ptr [0x3714]       ; -> loc_1_3714
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esi + 0x10], ecx
        mov    ax, word ptr [eax + 0x22]
        mov    word ptr [esi + 0x18], ax
        mov    eax, dword ptr [esi + 8]
        add    eax, 0x24
        lea    edx, [esi + 0x120]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        mov    ecx, 0x37f0                   ; -> loc_1_37F0
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [esi + 0x114], 0
        mov    dword ptr [esi + 0x118], 0xe
        mov    dword ptr [esi + 0x11c], 0
        mov    eax, dword ptr [edx + 2]
        add    edx, 2
        cmp    eax, dword ptr [ecx]
        jne    loc_1_2C67
        mov    ax, word ptr [edx + 4]
        cmp    ax, word ptr [ecx + 4]
        jne    loc_1_2C67
        mov    dword ptr [esi + 0x114], 1
        mov    dword ptr [esi + 0x11c], 2
        jmp    loc_1_2C98
loc_1_2C67:
        mov    eax, 0x37f8                   ; -> obj1:0x37F8
        mov    ecx, dword ptr [edx]
        cmp    ecx, dword ptr [eax]
        jne    loc_1_2C98
        mov    cl, byte ptr [edx + 4]
        cmp    cl, byte ptr [eax + 4]
        jne    loc_1_2C98
        mov    dword ptr [esi + 0x114], 2
        mov    dword ptr [esi + 0x118], 0xd
        mov    dword ptr [esi + 0x11c], 1
loc_1_2C98:
        mov    eax, dword ptr [esi + 0x114]
        or     eax, eax
        jne    loc_1_2D09
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 0x38]
        call   sub_1_34B7
        mov    ecx, edi
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        add    eax, ecx
        and    edi, 0xffff
        lea    ecx, [eax + edi]
        sub    edx, edx
        lea    edi, [ecx + 4]
        cmp    word ptr [ecx + 2], dx
        jbe    loc_1_2D1A
        mov    edi, edi
loc_1_2CD0:
        mov    eax, dword ptr [esi + 0xb8]
        mov    ebx, dword ptr [edi]
        mov    dword ptr [esi + eax*4 + 0x54], ebx
        mov    bx, word ptr [edi + 4]
        add    edi, 0x10
        mov    word ptr [esi + eax*2 + 0x98], bx
        mov    dword ptr [esi + eax*4 + 0xbc], 0x80000000
        inc    edx
        xor    eax, eax
        inc    dword ptr [esi + 0xb8]
        mov    ax, word ptr [ecx + 2]
        cmp    eax, edx
        jg     loc_1_2CD0
        jmp    loc_1_2D1A
loc_1_2D09:
        cmp    eax, 1
        jne    loc_1_2D1A
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 0x34]
        mov    dword ptr [esi + 0x104], eax
loc_1_2D1A:
        mov    eax, dword ptr [esi + 8]
        mov    ax, word ptr [eax + 0x44]
        and    eax, 0xffff
        add    eax, 0xc
        mov    ecx, eax
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0xfff
        xor    eax, edx
        sub    eax, edx
        sub    ecx, eax
        lea    eax, [ecx*2]
        cmp    eax, 0x1000
        jge    loc_1_2D4E
        mov    eax, 0x1000
loc_1_2D4E:
        push   0x4208
        push   eax
        call   sub_1_340E
        add    esp, 8
        mov    dword ptr [esi + 0x2c], eax
        or     eax, eax
        je     loc_1_2DF7
        add    eax, 0xa
        mov    dword ptr [esi + 0x34], eax
        add    eax, 2
        mov    dword ptr [esi + 0x28], eax
        mov    eax, dword ptr [esi + 0x2c]
        mov    word ptr [eax], 1
        mov    eax, dword ptr [esi + 0x2c]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0x2c]
        mov    ax, word ptr [eax + 0x44]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [esi + 0x2c]
        shl    eax, 0xc
        mov    dword ptr [esi + 0x30], eax
        add    eax, 0xa
        mov    dword ptr [esi + 0x38], eax
        mov    ecx, dword ptr [esi + 0x2c]
        add    eax, 2
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        add    eax, 0x39b6                   ; -> obj1:0x39B6
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_2EC0
        add    esp, 8
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        mov    eax, dword ptr [0x3748]       ; -> loc_1_3748
        push   eax
        lea    eax, [esi + 0x40]
        push   eax
        call   sub_1_3556
        or     eax, eax
        jne    loc_1_2DF7
        mov    eax, dword ptr [0x3754]       ; -> loc_1_3754
        mov    dword ptr [esi + 0x44], eax
        mov    dword ptr [0x3754], esi       ; -> loc_1_3754
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_2DF7:
        push   0
        push   esi
        call   sub_1_3456
        add    esp, 8
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_2E10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 4]
        sub    eax, eax
        cmp    byte ptr [0x3a76], al         ; -> obj1:0x3A76
        jbe    loc_1_2E99
        mov    dword ptr [ebp - 4], 0x39b6   ; -> obj1:0x39B6
loc_1_2E30:
        mov    ecx, dword ptr [ebp - 4]
loc_1_2E33:
        mov    bl, byte ptr [ecx]
        cmp    bl, 0x61
        jb     loc_1_2E50
        cmp    bl, 0x7a
        ja     loc_1_2E50
        mov    edi, ebx
        and    edi, 0xff
        sub    edi, 0x20
        jmp    loc_1_2E58
        db  08Dh, 064h, 024h, 000h
loc_1_2E50:
        mov    edi, ebx
        and    edi, 0xff
loc_1_2E58:
        mov    dl, byte ptr [esi]
        cmp    dl, 0x61
        jb     loc_1_2E70
        cmp    dl, 0x7a
        ja     loc_1_2E70
        and    edx, 0xff
        sub    edx, 0x20
        jmp    loc_1_2E76
        db  090h
loc_1_2E70:
        and    edx, 0xff
loc_1_2E76:
        cmp    edi, edx
        jne    loc_1_2E82
        or     bl, bl
        je     loc_1_2E82
        inc    ecx
        inc    esi
        jmp    loc_1_2E33
loc_1_2E82:
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [esi], cl
        je     loc_1_2E99
        add    dword ptr [ebp - 4], 0x10
        inc    eax
        xor    ecx, ecx
        mov    cl, byte ptr [0x3a76]         ; -> obj1:0x3A76
        cmp    ecx, eax
        ja     loc_1_2E30
loc_1_2E99:
        xor    ecx, ecx
        mov    cl, byte ptr [0x3a76]         ; -> obj1:0x3A76
        cmp    ecx, eax
        ja     loc_1_2EAA
        mov    eax, 0xffffffff
loc_1_2EAA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2EC0:
        push   ebp
        mov    ebp, esp
        sub    dx, dx
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_2EC9:
        mov    ax, dx
        inc    dx
        and    eax, 0xffff
        cmp    byte ptr [ecx + eax], 0
        jne    loc_1_2EC9
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 2], dx
        mov    word ptr [eax], dx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2EF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2E10
        add    esp, 4
        cmp    eax, -1
        jne    loc_1_2F08
        sub    al, al
        pop    ebp
        ret    
loc_1_2F08:
        mov    al, 1
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
NDIS2SUP_Control:
        cmp    eax, 0x1c
        jae    loc_1_2F1C
        jmp    dword ptr [eax*4 + 0x2f1e]    ; -> obj1:0x2F1E
loc_1_2F1C:
        clc    
        ret    
        ; ---- dispatch table (28 entries) ----
        dd  loc_2_0302
        dd  loc_2_05AE
        dd  loc_2_0649
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F9A
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_1_2F1C
        dd  loc_2_05AE
NDIS2SUP_Service_00:
        mov    eax, 0x30a
        clc    
        ret    
NDIS2SUP_Service_01:
        jmp    loc_1_2EF0
loc_1_2F9A:
        pushfd 
        cld    
        cli    
        mov    ecx, 6
        mov    esi, 0x3880                   ; -> obj1:0x3880
        mov    edi, dword ptr [0x3898]       ; -> obj1:0x3898
        rep movsd dword ptr es:[edi], dword ptr [esi]
        popfd  
        ret    
sub_1_2FB1:
        push   edx
        movzx  edx, cx
        sub    ecx, edx
        shr    ecx, 0xc
        add    ecx, edx
        add    ecx, dword ptr [ebx + 4]
        pop    edx
        ret    
sub_1_2FC1:
        mov    edi, 0x3a98                   ; -> obj1:0x3A98
        mov    ecx, 0x10
        mov    ebx, ecx
        cld    
        dec    ebx
        repne scasw ax, word ptr es:[edi]
        jne    loc_1_2FEA
        sub    ebx, ecx
        mov    edi, ebx
        mov    ecx, 0x3ab8                   ; -> obj1:0x3AB8
        mov    ecx, dword ptr [ecx + edi*4]
        or     ecx, ecx
        je     loc_1_2FEA
        cmp    dword ptr [ecx + 0x24], 0
        jne    loc_1_2FEC
loc_1_2FEA:
        xor    ecx, ecx
loc_1_2FEC:
        ret    
loc_1_2FED:
        mov    cx, word ptr [ebp + 0x34]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x30]
        call   sub_1_2FB1
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 6]
        push   ebx
        mov    bx, word ptr [ecx + 8]
        push   ebx
        mov    bx, word ptr [ecx + 0xa]
        push   ebx
        cld    
        call   sub_1_2370
        add    esp, 0xc
        mov    eax, 0xc
        VMMCall  VMM.Simulate_Far_Ret_N          ; cd204b000100
        ret    
loc_1_3023:
        mov    cx, word ptr [ebp + 0x34]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x30]
        call   sub_1_2FB1
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 6]
        push   ebx
        mov    bx, word ptr [ecx + 8]
        push   ebx
        cld    
        call   sub_1_2640
        add    esp, 8
        mov    eax, 0xa
        VMMCall  VMM.Simulate_Far_Ret_N          ; cd204b000100
        ret    
loc_1_3054:
        mov    cx, word ptr [ebp + 0x34]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x30]
        call   sub_1_2FB1
        mov    edi, ecx
        xor    ecx, ecx
        mov    cx, word ptr [edi + 0x10]
        push   ecx
        mov    cx, word ptr [edi + 0xe]
        push   ecx
        mov    ecx, dword ptr [edi + 0xa]
        call   sub_1_2FB1
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 0x12]
        call   sub_1_2FC1
        push   ecx
        or     ecx, ecx
        je     loc_1_3091
        cld    
        call   sub_1_28C0
loc_1_3091:
        add    esp, 0x10
        mov    eax, 0x10
        VMMCall  VMM.Simulate_Far_Ret_N          ; cd204b000100
        mov    word ptr [ebp + 0x1c], 5
        ret    
loc_1_30A6:
        mov    cx, word ptr [ebp + 0x34]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x30]
        call   sub_1_2FB1
        xor    eax, eax
        mov    ax, word ptr [ecx + 6]
        call   sub_1_2FC1
        push   ecx
        or     ecx, ecx
        je     loc_1_30CC
        cld    
        call   sub_1_2530
loc_1_30CC:
        add    esp, 4
        mov    eax, 4
        VMMCall  VMM.Simulate_Far_Ret_N          ; cd204b000100
        ret    
loc_1_30DB:
        mov    cx, word ptr [ebp + 0x34]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x30]
        call   sub_1_2FB1
        mov    edi, ecx
        mov    ecx, dword ptr [edi + 0xa]
        call   sub_1_2FB1
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 0x12]
        call   sub_1_2FC1
        push   ecx
        or     ecx, ecx
        je     loc_1_310C
        cld    
        call   sub_1_26E0
loc_1_310C:
        add    esp, 8
        mov    eax, 0x10
        VMMCall  VMM.Simulate_Far_Ret_N          ; cd204b000100
        mov    word ptr [ebp + 0x1c], 5
        ret    
loc_1_3121:
        mov    cx, word ptr [ebp + 0x34]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 0x30]
        call   sub_1_2FB1
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 6]
        push   ebx
        mov    bx, word ptr [ecx + 0xc]
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [ecx + 0xe]
        call   sub_1_2FC1
        push   ecx
        or     ecx, ecx
        je     loc_1_3153
        cld    
        call   sub_1_25A0
loc_1_3153:
        add    esp, 0xc
        mov    eax, 0xc
        VMMCall  VMM.Simulate_Far_Ret_N          ; cd204b000100
        ret    
sub_1_3162:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        and    word ptr [ebx + 0x2c], 0xfbff
        mov    ax, word ptr [0x389e]         ; -> obj1:0x389E
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x10]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x14]
        rol    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        rol    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x18]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    edx, dword ptr [ebp + 8]
        mov    ax, word ptr [edx + 0x18]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ecx, dword ptr [edx + 0x10]
        mov    dx, word ptr [ecx + 4]
        mov    cx, word ptr [ecx + 6]
        and    edx, 0xffff
        and    ecx, 0xffff
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cld    
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 0x1c]
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3222:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        mov    ecx, dword ptr [0x3878]       ; -> obj1:0x3878
        VMMCall  VMM.Begin_Non_Serial_Nest_V86_Exec ; cd2086010100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        and    word ptr [ebx + 0x2c], 0xfbff
        mov    ax, word ptr [0x389e]         ; -> obj1:0x389E
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x10]
        rol    eax, 5
        add    eax, dword ptr [0x3a88]       ; -> obj1:0x3A88
        mov    eax, dword ptr [eax + 4]
        rol    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        rol    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    edx, dword ptr [ebp + 8]
        mov    ax, word ptr [edx + 0x18]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ecx, dword ptr [edx + 0x10]
        mov    dx, word ptr [ecx + 8]
        mov    cx, word ptr [ecx + 0xa]
        and    edx, 0xffff
        and    ecx, 0xffff
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cld    
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 0x1c]
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_32E2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        and    word ptr [ebx + 0x2c], 0xfbff
        mov    eax, dword ptr [ebp + 0x10]
        rol    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        rol    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x30]
        rol    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        rol    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ax, word ptr [edx + 0x18]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    ecx, dword ptr [edx + 0x10]
        mov    dx, word ptr [ecx + 0xc]
        mov    cx, word ptr [ecx + 0xe]
        and    edx, 0xffff
        and    ecx, 0xffff
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cld    
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 0x1c]
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    esi
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3399:
        cld    
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        cmp    ecx, 0xc
        jb     loc_1_33C5
        mov    eax, edi
        neg    eax
        and    eax, 3
        xchg   ecx, eax
        sub    eax, ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
loc_1_33C5:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    
sub_1_33CA:
        push   ebx
        push   esi
        push   edi
        cld    
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [edi + edx*4], eax
        xor    ecx, ecx
        dec    ecx
loc_1_33E2:
        repne scasd eax, dword ptr es:[edi]
        mov    ebx, ecx
        inc    ebx
        neg    ebx
        cmp    ebx, edx
        ja     loc_1_340A
        mov    edx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x1c]
        mov    si, word ptr [esi + 4]
        cmp    si, word ptr [edx + ebx*2 - 2]
        mov    edx, dword ptr [esp + 0x18]
        jne    loc_1_33E2
        mov    eax, ebx
loc_1_3406:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_340A:
        xor    eax, eax
        jmp    loc_1_3406
sub_1_340E:
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8800100
sub_1_3414:
        mov    eax, dword ptr [esp + 4]
        add    eax, 4
        cmp    byte ptr [0x3824], 2          ; -> obj1:0x3824
        je     loc_1_343E
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_343D
        mov    dword ptr [eax], 0
        add    eax, 4
loc_1_343D:
        ret    
loc_1_343E:
        push   eax
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
        add    esp, 4
        or     eax, eax
        je     loc_1_343D
        mov    dword ptr [eax], 1
        add    eax, 4
        ret    
sub_1_3456:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_349C
        sub    dword ptr [esp + 4], 4
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx]
        and    eax, 0xfffe
        je     loc_1_3471
        jmp    loc_1_349C
loc_1_3471:
        cmp    byte ptr [0x3824], 2          ; -> obj1:0x3824
        je     loc_1_3480
        VMMCall  VMM._HeapFree                   ; cd2051800100
loc_1_3480:
        mov    edx, dword ptr [esp + 4]
        push   esi
        push   ebx
        mov    esi, 0x349d                   ; -> loc_1_349D
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ebx
        pop    esi
loc_1_349C:
        ret    
loc_1_349D:
        push   0
        push   edx
        cmp    dword ptr [edx], 0
        jne    loc_1_34AF
        VMMCall  VMM._HeapFree                   ; cd2051000100
loc_1_34AB:
        add    esp, 8
        ret    
loc_1_34AF:
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e004000
        jmp    loc_1_34AB
sub_1_34B7:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [ebx + 4]
        pop    ebx
        ret    
        db  000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  06Eh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h
        db  024h, 008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh
        db  074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h
        db  00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh
        db  0F3h, 066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch
        db  08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_353E:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_3544:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_354A:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_3550:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_3556:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_355C:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_3568:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_356E:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_3580:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_3586:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_358C:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_3592:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_3598:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
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
sub_1_3654:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_3678:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_369C:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
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
loc_1_3710:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3714:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3718:
        add    byte ptr [eax], al
loc_1_371A:
        add    byte ptr [eax], al
loc_1_371C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3720:
        add    byte ptr [eax], al
loc_1_3722:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3728:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_372C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3730:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3738:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_373C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3740:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3744:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3748:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3750:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_375C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3760:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3764:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3768:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_376C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3770:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3774:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3778:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_377C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3784:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_378C:
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
        or     eax, 0xe000101
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [edx], eax
loc_1_37F0:
        cmp    byte ptr [eax], dh
        xor    ch, byte ptr [esi]
        xor    eax, 0x46000000
        inc    esp
        inc    esp
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    dl, byte ptr [ebx + 0x55]
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_380C:
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    dl, byte ptr [ebx + 0x55]
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dd  NDIS2SUP_Service_00      ; fixup
        dd  NDIS2SUP_Service_01      ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00496h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'NDIS2SUP'                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  NDIS2SUP_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x3818              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x8a8  R-X
        db  02Bh, 0C0h, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0010:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 0xc], 9
        mov    word ptr [ebp - 0xa], 9
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0x380c   ; -> loc_1_380C
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [0x3714], eax       ; -> loc_1_3714
        push   0
        push   0
        push   0
        push   0x3728                        ; -> loc_1_3728
        call   sub_1_353E
        push   0
        push   0x3f
        call   sub_1_3414
        add    esp, 8
        mov    dword ptr [0x3744], eax       ; -> loc_1_3744
        or     eax, eax
        je     loc_2_01B2
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    dword ptr [eax + 6], 0x610    ; -> loc_1_0610
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    dword ptr [eax + 0xa], 0x7a0  ; -> loc_1_07A0
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    dword ptr [eax + 0xe], 0xea0  ; -> loc_1_0EA0
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    dword ptr [eax + 0x12], 0x1140 ; -> loc_1_1140
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    dword ptr [eax + 0x16], 0x1460 ; -> loc_1_1460
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    dword ptr [eax + 0x1a], 0x2190 ; -> loc_1_2190
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    dword ptr [eax + 0x1e], 0x2190 ; -> loc_1_2190
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x22], 0x21d0 ; -> loc_1_21D0
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        mov    dword ptr [eax + 0x26], 0x2a70 ; -> loc_1_2A70
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        push   0x3f
        mov    dword ptr [eax + 0x2a], 0x2210 ; -> loc_1_2210
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [0x3744]       ; -> loc_1_3744
        mov    dword ptr [edx + 0x2e], eax
        mov    dword ptr [edx + 0x32], ecx
        add    edx, 0x2e
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        push   eax
        push   0
        mov    eax, dword ptr [0x3728]       ; -> loc_1_3728
        push   eax
        lea    eax, [ebp - 4]
        push   0x3748                        ; -> loc_1_3748
        push   eax
        call   sub_1_354A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_01A2
        mov    eax, dword ptr [0x3a77]       ; -> obj1:0x3A77
        mov    dword ptr [0x372c], eax       ; -> loc_1_372C
        mov    ecx, eax
        and    ecx, 0xffff0fff
        shr    ecx, 0xc
        and    eax, 0xffff
        add    ecx, eax
        add    ecx, dword ptr [0x3714]       ; -> loc_1_3714
        mov    dword ptr [0x3730], ecx       ; -> loc_1_3730
        mov    dword ptr [0x3750], 0         ; -> loc_1_3750
        mov    dword ptr [0x3754], 0         ; -> loc_1_3754
        mov    dword ptr [0x3768], 0         ; -> loc_1_3768
        mov    dword ptr [0x3760], 0         ; -> loc_1_3760
        mov    dword ptr [0x375c], 0         ; -> loc_1_375C
        mov    byte ptr [0x3738], 0          ; -> loc_1_3738
        mov    dword ptr [0x3740], 0         ; -> loc_1_3740
        mov    dword ptr [0x373c], 0         ; -> loc_1_373C
        call   sub_2_01C0
        or     eax, eax
        jne    loc_2_01A2
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_01A2:
        push   0
        mov    eax, dword ptr [0x3744]       ; -> loc_1_3744
        push   eax
        call   sub_1_3456
        add    esp, 8
loc_2_01B2:
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_2_01C0:
        push   esi
        cmp    byte ptr [0x3a76], 0          ; -> obj1:0x3A76
        jbe    loc_2_022C
        mov    edx, 0x3a36                   ; -> obj1:0x3A36
        xor    ecx, ecx
        mov    cl, byte ptr [0x3a76]         ; -> obj1:0x3A76
loc_2_01D7:
        mov    eax, dword ptr [edx]
        mov    esi, eax
        and    esi, 0xffff0fff
        shr    esi, 0xc
        and    eax, 0xffff
        add    esi, eax
        add    esi, dword ptr [0x3714]       ; -> loc_1_3714
        mov    eax, dword ptr [esi + 0x28]
        mov    esi, eax
        and    esi, 0xffff0fff
        shr    esi, 0xc
        and    eax, 0xffff
        add    esi, eax
        mov    eax, dword ptr [0x3714]       ; -> loc_1_3714
        mov    ax, word ptr [esi + eax + 0x44]
        and    eax, 0xffff
        mov    esi, dword ptr [0x3a90]       ; -> obj1:0x3A90
        cmp    eax, esi
        ja     loc_2_0221
        mov    eax, esi
loc_2_0221:
        mov    dword ptr [0x3a90], eax       ; -> obj1:0x3A90
        add    edx, 4
        dec    ecx
        jne    loc_2_01D7
loc_2_022C:
        mov    eax, dword ptr [0x3a90]       ; -> obj1:0x3A90
        add    eax, 4
        cmp    dword ptr [0x3a8c], eax       ; -> obj1:0x3A8C
        jae    loc_2_0257
        mov    ecx, 0x8000
loc_2_0241:
        cmp    dword ptr [0x3a8c], ecx       ; -> obj1:0x3A8C
        jae    loc_2_0257
        shl    dword ptr [0x3a8c], 1         ; -> obj1:0x3A8C
        cmp    dword ptr [0x3a8c], eax       ; -> obj1:0x3A8C
        jb     loc_2_0241
loc_2_0257:
        cmp    dword ptr [0x3a8c], 0x1000    ; -> obj1:0x3A8C
        jae    loc_2_026D
        mov    dword ptr [0x3a8c], 0x1000    ; -> obj1:0x3A8C
loc_2_026D:
        push   0x4208
        mov    eax, dword ptr [0x3a8c]       ; -> obj1:0x3A8C
        push   eax
        call   sub_1_340E
        add    esp, 8
        mov    dword ptr [0x3764], eax       ; -> loc_1_3764
        mov    dword ptr [0x3a7c], eax       ; -> obj1:0x3A7C
        or     eax, eax
        jne    loc_2_0295
        mov    eax, 0xc000009a
        pop    esi
        ret    
loc_2_0295:
        mov    eax, dword ptr [0x3764]       ; -> loc_1_3764
        push   eax
        call   sub_2_0672
        add    esp, 4
        mov    dword ptr [0x3774], 0         ; -> loc_1_3774
        mov    dword ptr [0x3778], 0         ; -> loc_1_3778
        mov    eax, dword ptr [0x3a8c]       ; -> obj1:0x3A8C
        sub    eax, 4
        mov    dword ptr [0x3770], eax       ; -> loc_1_3770
        mov    eax, dword ptr [0x3a8c]       ; -> obj1:0x3A8C
        dec    eax
        mov    dword ptr [0x377c], eax       ; -> loc_1_377C
        call   sub_1_0000
        cmp    eax, 0xc0000001
        jne    loc_2_02E2
        mov    eax, 0xc0000001
        pop    esi
        ret    
loc_2_02E2:
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_02F0:
        sub    eax, eax
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0300:
        stc    
        ret    
loc_2_0302:
        clc    
        ret    
sub_2_0304:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        push   0
        push   0x18
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        add    esp, 8
        test   eax, eax
        je     loc_2_03C5
        mov    edi, eax
        mov    dword ptr [edi], 0x5349444e
        mov    dword ptr [edi + 4], 0x24504c48
        mov    byte ptr [edi + 8], 0
        shr    eax, 4
        mov    word ptr [ebp + 0x1c], 0x3d40
        mov    word ptr [ebp + 0x3c], ax
        mov    word ptr [ebp + 0x14], 0
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  ecx, word ptr [ebp + 0x1c]
        test   word ptr [ebp + 0x2c], 1
        jne    loc_2_03BF
        mov    word ptr [ebp + 0x10], cx
        mov    dword ptr [edi], 0x3733e96f
        mov    word ptr [ebp + 0x1c], 0x4402
        mov    word ptr [ebp + 0x18], 8
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   word ptr [ebp + 0x2c], 1
        jne    loc_2_03B0
        cmp    word ptr [ebp + 0x1c], 8
        jne    loc_2_03B0
        cmp    dword ptr [edi], 0xef6f3733
        jne    loc_2_03B0
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [0x387c], eax       ; -> obj1:0x387C
loc_2_03B0:
        mov    byte ptr [ebp + 0x1d], 0x3e
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_2_03BF:
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
loc_2_03C5:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    dword ptr [0x387c], 1         ; -> obj1:0x387C
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
sub_2_03E7:
        push   eax
        push   ebx
        push   ecx
        push   edx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    eax, 6
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        movzx  edx, word ptr [0x387c]        ; -> obj1:0x387C
        mov    cx, word ptr [0x387e]         ; -> obj1:0x387E
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        add    word ptr [ebp + 0x30], 2
        movzx  edx, word ptr [ebp + 0x14]
        movzx  eax, word ptr [ebp + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        cmp    eax, 1
        jb     loc_2_04DD
        cmp    edx, 1
        jb     loc_2_04DD
        movzx  esi, word ptr [0x387e]        ; -> obj1:0x387E
        shl    esi, 4
        add    esi, eax
        mov    edi, 0x38a0                   ; -> obj1:0x38A0
        cmp    word ptr [esi + 0x21], 1
        jae    loc_2_0478
        cmp    byte ptr [esi + 0x2e], 0
        stc    
        jne    loc_2_04DD
loc_2_0478:
        movzx  ecx, byte ptr [esi + 0x2e]
        mov    eax, ecx
        imul   ecx, ecx, 0x1c
        add    ecx, 0x34
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, eax
        imul   ecx, ecx, 0x10
        add    ecx, 2
        mov    edi, 0x39b4                   ; -> obj1:0x39B4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0x3a36                   ; -> obj1:0x3A36
        mov    esi, 0x38d4                   ; -> obj1:0x38D4
        mov    ecx, eax
loc_2_04A2:
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    esi, 0x18
        loop   loc_2_04A2
        mov    al, byte ptr [0x38ce]         ; -> obj1:0x38CE
        mov    byte ptr [0x3a76], al         ; -> obj1:0x3A76
        mov    ax, word ptr [0x38bf]         ; -> obj1:0x38BF
        mov    word ptr [0x389e], ax         ; -> obj1:0x389E
        movzx  esi, word ptr [0x387e]        ; -> obj1:0x387E
        shl    esi, 4
        add    esi, edx
        mov    dword ptr [0x3898], esi       ; -> obj1:0x3898
        mov    edi, 0x3880                   ; -> obj1:0x3880
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        clc    
loc_2_04DD:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_2_04E2:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        cmp    byte ptr [0x38ce], 0          ; -> obj1:0x38CE
        je     loc_2_054F
        mov    esi, 0x3121                   ; -> loc_1_3121
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        push   eax
        mov    esi, 0x30db                   ; -> loc_1_30DB
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        push   eax
        mov    esi, 0x30a6                   ; -> loc_1_30A6
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        push   eax
        mov    esi, 0x3054                   ; -> loc_1_3054
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        push   eax
        mov    esi, 0x3023                   ; -> loc_1_3023
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        push   eax
        mov    esi, 0x2fed                   ; -> loc_1_2FED
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        push   eax
        mov    ecx, 6
        mov    esi, esp
        mov    edi, dword ptr [0x3898]       ; -> obj1:0x3898
        pushfd 
        cli    
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        popfd  
        add    esp, 0x18
loc_2_054F:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_2_0556:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    edi, dword ptr [0x694]        ; -> obj2:0x0694
        mov    esi, 0x698                    ; -> loc_2_0698
        mov    ecx, 0x210
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [0x694]        ; -> obj2:0x0694
        mov    eax, esi
        mov    eax, dword ptr [0x694]        ; -> obj2:0x0694
        add    eax, 0
        mov    dword ptr [0x3a88], eax       ; -> obj1:0x3A88
        mov    eax, esi
        add    esi, 6
        shr    eax, 4
        mov    ecx, 0x10
loc_2_0591:
        mov    word ptr [esi], ax
        add    esi, 0x20
        loop   loc_2_0591
        mov    word ptr [0x3a79], ax         ; -> obj1:0x3A79
        mov    word ptr [0x3a77], 0x200      ; -> obj1:0x3A77
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_2_05AE:
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
        mov    al, byte ptr [0x3a94]         ; -> obj1:0x3A94
        cmp    al, 0
        jne    loc_2_0647
        inc    byte ptr [0x3a94]             ; -> obj1:0x3A94
        push   0x5008
        push   0x1000
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_2_0300
        mov    dword ptr [0x694], eax        ; -> obj2:0x0694
        add    eax, 0x210
        mov    edi, eax
        add    eax, 0x400
        shl    edi, 0xc
        mov    di, 0x400
        mov    dword ptr [0x3878], edi       ; -> obj1:0x3878
        cld    
        call   0
        cmp    eax, 0xc0000001
        je     loc_2_0300
        call   sub_2_0304
        jb     loc_2_0300
        call   sub_2_03E7
        jb     loc_2_0300
        call   sub_2_0556
        push   ebx
        cld    
        call   sub_2_0010
        add    esp, 4
        cmp    eax, 0xc0000001
        je     loc_2_0300
        call   sub_2_04E2
        ret    
loc_2_0647:
        clc    
        ret    
loc_2_0649:
        push   ebx
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        jb     loc_2_0658
        mov    byte ptr [0x3824], bl         ; -> obj1:0x3824
loc_2_0658:
        pop    ebx
        cld    
        call   sub_2_02F0
        cmp    eax, 0xc0000001
        je     loc_2_0300
        inc    dword ptr [0x3820]            ; -> obj1:0x3820
        clc    
        ret    
sub_2_0672:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x3a88]       ; -> obj1:0x3A88
        rol    eax, 0xc
        sub    ecx, 0x20
loc_2_0682:
        add    ecx, 0x20
        mov    dword ptr [ecx + 0x14], eax
        mov    dword ptr [ecx + 0x1c], eax
        cmp    dword ptr [ecx], -1
        jne    loc_2_0682
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0698:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        push   0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0x88000000], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    al, cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    al, ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        push   1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0x88000000], cl
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    al, cl
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0E8h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 054h
        db  054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h, 054h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x86  R-X
        db  04Eh, 044h, 049h, 053h, 048h, 04Ch, 050h, 024h, 000h, 06Fh, 0E9h, 033h, 037h, 000h, 000h, 000h
        db  000h, 0F7h, 0C3h, 003h, 000h, 075h, 069h, 0B8h, 040h, 03Dh, 0BAh, 000h, 000h, 0CDh, 021h, 072h
        db  05Fh, 08Bh, 0D8h, 08Bh, 0FBh, 0BEh, 009h, 000h, 08Bh, 0D6h, 0B9h, 008h, 000h, 0B8h, 002h, 044h
        db  0CDh, 021h, 072h, 046h, 081h, 03Ch, 033h, 037h, 075h, 040h, 081h, 07Ch, 002h, 06Fh, 0EFh, 075h
        db  039h, 08Bh, 0DFh, 0B4h, 03Eh, 0CDh, 021h, 0B8h, 006h, 000h, 050h, 0FFh, 05Ch, 004h, 083h, 0C4h
        db  002h, 00Bh, 0C0h, 074h, 02Bh, 00Bh, 0D2h, 074h, 027h, 00Bh, 0DBh, 074h, 023h, 08Eh, 044h, 006h
        db  08Bh, 0F8h, 026h, 080h, 07Dh, 02Fh, 000h, 075h, 017h, 026h, 083h, 07Dh, 021h, 000h, 074h, 010h
        db  033h, 0C0h, 066h, 033h, 0D2h, 033h, 0F6h, 033h, 0DBh, 0C3h, 08Bh, 0DFh, 0B4h, 03Eh, 0CDh, 021h
        db  0B8h, 001h, 080h, 0EBh, 0EDh, 034h
VxD__LTEXT ENDS

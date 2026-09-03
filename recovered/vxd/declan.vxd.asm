; DECLAN -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 6915 instructions recovered by recursive descent

Declare_Virtual_Device DECLAN, 3, 10, DECLAN_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1ddc  R-X
loc_1_0000:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        pop    ebp
        mov    ax, word ptr [eax + 0x5e]
        or     word ptr [ecx], ax
        mov    eax, dword ptr [edx]
        mov    word ptr [eax + 0x5e], 0
        mov    al, 1
        ret    4
loc_1_0020:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        sub    bl, bl
        cmp    byte ptr [ecx + 0x168], bl
        je     loc_1_0040
        sub    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0040:
        test   byte ptr [ecx], 0x13
        je     loc_1_005A
        cmp    byte ptr [ecx + 0x4b], 0
        jne    loc_1_005A
        mov    bl, 1
        mov    byte ptr [ecx + 0x4b], bl
        mov    dx, word ptr [ecx + 0x41]
        mov    ax, 7
        out    dx, ax
loc_1_005A:
        mov    dx, word ptr [ecx + 0x3d]
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        test   al, 0x80
        je     loc_1_00C1
        and    ax, 0x7f00
        mov    al, 0x40
        cmp    byte ptr [ecx + 0x167], 0
        mov    dx, word ptr [ecx + 0x3d]
        out    dx, ax
        je     loc_1_00B0
        test   byte ptr [ebp - 1], 1
        je     loc_1_00B0
        mov    byte ptr [ecx + 0x167], 0
        test   byte ptr [ecx], 0x13
        je     loc_1_00A1
        or     bl, bl
        je     loc_1_00A1
        mov    byte ptr [ecx + 0x4b], 0
        mov    dx, word ptr [ecx + 0x41]
        mov    ax, 2
        out    dx, ax
loc_1_00A1:
        sub    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_00B0:
        mov    ax, word ptr [ebp - 2]
        or     word ptr [ecx + 0x5e], ax
        mov    al, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_00C1:
        test   byte ptr [ecx], 0x13
        je     loc_1_00D8
        or     bl, bl
        je     loc_1_00D8
        mov    byte ptr [ecx + 0x4b], 0
        mov    dx, word ptr [ecx + 0x41]
        mov    ax, 2
        out    dx, ax
loc_1_00D8:
        sub    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_00F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x168], 0
        jne    loc_1_0167
        cmp    byte ptr [esi + 0xac], 0
        jne    loc_1_0167
        test   byte ptr [esi], 0x13
        je     loc_1_0125
        cmp    byte ptr [esi + 0x4b], 0
        jne    loc_1_0125
        mov    byte ptr [esi + 0x4b], 1
        mov    dx, word ptr [esi + 0x41]
        mov    ax, 7
        out    dx, ax
loc_1_0125:
        mov    byte ptr [esi + 0xac], 1
        push   esi
        call   sub_1_0170
        mov    al, byte ptr [esi + 0x4b]
        and    eax, 0xffff00ff
        test   byte ptr [esi], 0x13
        je     loc_1_0167
        or     ax, ax
        je     loc_1_0167
        mov    byte ptr [esi + 0x4b], 0
        mov    dword ptr [ebp - 0xc], esi
        mov    ax, word ptr [esi + 0x49]
        or     ax, 2
        mov    word ptr [ebp - 4], ax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0xae0                         ; -> loc_1_0AE0
        inc    esi
        push   esi
        call   sub_1_19EA
loc_1_0167:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        mov    esi, dword ptr [ebp + 8]
loc_1_0182:
        mov    byte ptr [esi + 0x17a], 0
        mov    dx, word ptr [esi + 0x3d]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        test   al, 0x80
        je     loc_1_01AB
        and    ax, 0x7f00
        mov    al, 0x40
        mov    dx, word ptr [esi + 0x3d]
        out    dx, ax
        mov    ax, word ptr [ebp - 4]
        or     word ptr [ebp - 2], ax
loc_1_01AB:
        mov    dword ptr [ebp - 0x10], esi
        lea    eax, [ebp - 2]
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   0                             ; -> loc_1_0000
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_19EA
        cmp    byte ptr [esi + 0x149], 0
        jne    loc_1_020B
        test   byte ptr [ebp - 1], 0x5e
        jne    loc_1_0215
        cmp    dword ptr [esi + 0xd5], 0
        jne    loc_1_0215
        cmp    byte ptr [esi + 0x148], 0
        je     loc_1_01F0
        cmp    dword ptr [esi + 0x80], 0
        je     loc_1_0215
loc_1_01F0:
        cmp    byte ptr [esi + 0xfa], 0
        jne    loc_1_020B
        cmp    byte ptr [esi + 0xf9], 0
        je     loc_1_020B
        cmp    dword ptr [esi + 0xfc], 0
        jne    loc_1_0215
loc_1_020B:
        test   byte ptr [ebp - 1], 1
        je     loc_1_0490
loc_1_0215:
        mov    eax, dword ptr [esi + 0x80]
        inc    eax
        mov    dword ptr [esi + 0x80], eax
        test   byte ptr [ebp - 1], 1
        jne    loc_1_0332
        cmp    byte ptr [esi + 0x148], 0
        je     loc_1_0255
        cmp    byte ptr [esi + 0x149], 0
        jne    loc_1_0255
        cmp    eax, 1
        jne    loc_1_0255
        push   esi
        call   sub_2_1150
        mov    byte ptr [esi + 0x149], 1
        jmp    loc_1_0418
loc_1_0255:
        test   byte ptr [ebp - 1], 0x58
        je     loc_1_02A9
        test   byte ptr [ebp - 1], 0x10
        je     loc_1_0269
        inc    dword ptr [esi + 0x13c]
        jmp    loc_1_02A3
loc_1_0269:
        test   byte ptr [ebp - 1], 0x40
        je     loc_1_0291
        inc    dword ptr [esi + 0x138]
        push   1
        push   1
        push   2
        push   0xaa000006
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0x14
        jmp    loc_1_02A3
loc_1_0291:
        inc    dword ptr [esi + 0x134]
        push   0xb
        push   0
        push   0
        push   esi
        call   sub_2_1460
loc_1_02A3:
        and    word ptr [ebp - 2], 0x7ff
loc_1_02A9:
        test   byte ptr [ebp - 1], 4
        je     loc_1_02BF
        push   esi
        call   sub_1_04C0
        or     al, al
        je     loc_1_02BF
        and    word ptr [ebp - 2], 0xfbff
loc_1_02BF:
        test   byte ptr [ebp - 1], 2
        je     loc_1_02F9
        test   byte ptr [ebp - 2], 0x10
        jne    loc_1_02E9
        and    word ptr [ebp - 2], 0xfdff
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_1_0418
        push   0xb
        push   0
        push   0
        jmp    loc_1_0412
loc_1_02E9:
        push   esi
        call   sub_1_0790
        or     al, al
        jne    loc_1_02F9
        and    word ptr [ebp - 2], 0xfdff
loc_1_02F9:
        cmp    byte ptr [esi + 0xfa], 0
        jne    loc_1_031A
        cmp    byte ptr [esi + 0xf9], 0
        je     loc_1_031A
        cmp    dword ptr [esi + 0xfc], 0
        je     loc_1_031A
        push   esi
        call   sub_1_0E30
loc_1_031A:
        cmp    dword ptr [esi + 0xd5], 0
        je     loc_1_0418
        push   esi
        call   sub_1_11B0
        jmp    loc_1_0418
loc_1_0332:
        and    word ptr [ebp - 2], 0xfeff
        mov    byte ptr [esi + 0x148], 0
        mov    byte ptr [esi + 0x149], 0
        mov    edi, dword ptr [esi + 0x14a]
        or     edi, edi
        mov    eax, dword ptr [esi + 0x152]
        mov    ecx, dword ptr [esi + 0x14e]
        je     loc_1_03CB
        cmp    eax, 4
        je     loc_1_03C4
        or     ecx, ecx
        je     loc_1_0370
        push   1
        push   esi
        call   sub_2_14C0
        jmp    loc_1_03D4
        db  090h
loc_1_0370:
        cmp    eax, 8
        jne    loc_1_03D4
        mov    ebx, dword ptr [esi + 0x84]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0xffffff7c
        je     loc_1_03B4
loc_1_0386:
        inc    dword ptr [ebx + 0x18]
        push   0
        push   0
        push   0x40010005
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_195E
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_1964
        dec    dword ptr [ebx + 0x18]
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0xffffff7c
        jne    loc_1_0386
loc_1_03B4:
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_1946
        dec    dword ptr [edi + 0x18]
        jmp    loc_1_03D4
loc_1_03C4:
        or     edi, edi
        je     loc_1_03CB
        dec    dword ptr [edi + 0x18]
loc_1_03CB:
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_1_03DA
loc_1_03D4:
        push   esi
        call   sub_2_00F0
loc_1_03DA:
        mov    ecx, dword ptr [esi + 0x156]
        or     ecx, ecx
        je     loc_1_0418
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x156], eax
        cmp    dword ptr [esi + 0x15a], ecx
        jne    loc_1_03FE
        mov    dword ptr [esi + 0x15a], 0
loc_1_03FE:
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        jne    loc_1_040C
        push   4
        push   0
        jmp    loc_1_040E
loc_1_040C:
        push   eax
        push   ecx
loc_1_040E:
        mov    eax, dword ptr [ecx + 4]
        push   eax
loc_1_0412:
        push   esi
        call   sub_2_1460
loc_1_0418:
        mov    ecx, dword ptr [esi + 0x8c]
        lea    edi, [esi + 0x8c]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 0x8c
        je     loc_1_0484
        mov    eax, esi
        sub    eax, ecx
        cmp    eax, 0xffffff74
        je     loc_1_0484
loc_1_043A:
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [ebx + 0x18], 0
        jne    loc_1_0477
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_1934
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx + 0x14]
        cmp    dword ptr [esi + 0x162], eax
        jne    loc_1_046D
        push   esi
        call   sub_2_01F0
loc_1_046D:
        push   0
        push   0x39
        push   ebx
        call   sub_1_19FC
loc_1_0477:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, dword ptr [edi]
        cmp    eax, 0xffffff74
        jne    loc_1_043A
loc_1_0484:
        dec    dword ptr [esi + 0x80]
        jmp    loc_1_0182
        db  090h
loc_1_0490:
        mov    byte ptr [esi + 0xac], 0
        cmp    byte ptr [esi + 0xfb], 0
        je     loc_1_04B3
        mov    byte ptr [esi + 0xfb], 0
        mov    eax, dword ptr [esi + 0xad]
        push   eax
        call   sub_1_1780
loc_1_04B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_04C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x108]
        lea    edi, [ecx*8]
        mov    dword ptr [ebp - 0xc], ecx
        add    edi, dword ptr [esi + 0xd1]
        mov    eax, dword ptr [esi + 0x64]
        dec    eax
        mov    dword ptr [ebp - 0x14], eax
        sub    al, al
        mov    ebx, 1
        mov    dword ptr [ebp - 0x10], 0
loc_1_04F7:
        mov    cl, byte ptr [edi + 3]
        mov    byte ptr [ebp - 1], cl
        test   cl, 0x80
        jne    loc_1_0700
        cmp    dword ptr [ebp - 0x10], 0xa
        ja     loc_1_070B
        test   cl, 0x40
        je     loc_1_0560
        test   cl, 4
        je     loc_1_0525
        inc    dword ptr [esi + 0x10c]
        jmp    loc_1_0624
loc_1_0525:
        test   byte ptr [ebp - 1], 8
        je     loc_1_0536
        inc    dword ptr [esi + 0x110]
        jmp    loc_1_0624
loc_1_0536:
        test   byte ptr [ebp - 1], 0x10
        je     loc_1_0547
        inc    dword ptr [esi + 0x10c]
        jmp    loc_1_0624
loc_1_0547:
        test   byte ptr [ebp - 1], 0x20
        je     loc_1_0624
        inc    dword ptr [esi + 0x114]
        jmp    loc_1_0624
        db  08Dh, 064h, 024h, 000h
loc_1_0560:
        test   byte ptr [ebp - 1], 1
        je     loc_1_0637
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_1_0716
        or     dword ptr [ebp - 8], 1
        mov    eax, dword ptr [esi + 0x108]
        add    eax, eax
        xor    eax, dword ptr [ebp - 8]
        and    eax, 0xfe
        xor    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 8
        xor    eax, dword ptr [ebp - 8]
        and    eax, 0x7f00
        xor    dword ptr [ebp - 8], eax
        mov    cx, word ptr [edi + 6]
        and    ecx, 0xfff
        sub    ecx, 4
        mov    eax, dword ptr [esi + 0x68]
        cmp    eax, ecx
        jb     loc_1_05B5
        mov    eax, ecx
loc_1_05B5:
        sub    eax, 0xe
        mov    dword ptr [ebp - 0x18], eax
        inc    dword ptr [esi + 0x144]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x15e], eax
        cmp    ecx, 0xe
        mov    byte ptr [esi + 0xfb], 1
        jae    loc_1_05F5
        cmp    ecx, 6
        jb     loc_1_0624
        mov    edx, dword ptr [esi + 0x104]
        mov    eax, dword ptr [esi + 0x108]
        mov    eax, dword ptr [edx + eax*4]
        push   0
        push   0
        push   0
        push   ecx
        push   eax
        push   eax
        jmp    loc_1_0614
loc_1_05F5:
        mov    eax, dword ptr [esi + 0x104]
        mov    edx, dword ptr [esi + 0x108]
        mov    edx, dword ptr [eax + edx*4]
        sub    ecx, 0xe
        push   ecx
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        lea    eax, [edx + 0xe]
        push   eax
        push   0xe
        push   edx
        push   edx
loc_1_0614:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0xad]
        push   eax
        call   sub_1_15A0
loc_1_0624:
        inc    dword ptr [ebp - 0x10]
        push   ebx
        mov    eax, dword ptr [esi + 0x108]
        push   eax
        push   esi
        call   sub_1_0730
        mov    al, 1
loc_1_0637:
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0x14], ecx
        jne    loc_1_0650
        mov    dword ptr [ebp - 0xc], 0
        mov    edi, dword ptr [esi + 0xd1]
        jmp    loc_1_0656
        db  08Bh, 0FFh
loc_1_0650:
        inc    dword ptr [ebp - 0xc]
        add    edi, 8
loc_1_0656:
        or     al, al
        je     loc_1_0667
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x108], ecx
        sub    al, al
        sub    ebx, ebx
loc_1_0667:
        inc    ebx
        mov    ecx, dword ptr [ebp - 0x14]
        inc    ecx
        cmp    ecx, ebx
        jae    loc_1_04F7
        mov    byte ptr [esi + 0x169], 1
        mov    edi, dword ptr [esi + 0x84]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff7c
        je     loc_1_06BE
        lea    esp, [esp]
loc_1_0690:
        inc    dword ptr [edi + 0x18]
        push   0
        push   0
        push   0xc0010002
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_195E
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_1964
        dec    dword ptr [edi + 0x18]
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0xffffff7c
        jne    loc_1_0690
loc_1_06BE:
        push   0x2625a0
        call   sub_1_19F0
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_19E4
        push   0
        push   0xaa000003
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1928
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0700:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_070B:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0716:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0730:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [ecx*8]
        add    edx, dword ptr [eax + 0xd1]
        mov    esi, dword ptr [eax + 0x64]
        dec    esi
        mov    bl, byte ptr [edx + 3]
        mov    edi, dword ptr [ebp + 0x10]
        or     edi, edi
        je     loc_1_0778
        mov    ebx, 0x80
        nop    
loc_1_0760:
        mov    byte ptr [edx + 3], bl
        cmp    esi, ecx
        jne    loc_1_0771
        mov    edx, dword ptr [eax + 0xd1]
        sub    ecx, ecx
        jmp    loc_1_0775
loc_1_0771:
        add    edx, 8
        inc    ecx
loc_1_0775:
        dec    edi
        jne    loc_1_0760
loc_1_0778:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0790:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebx + 0xbd]
        cmp    dword ptr [ebx + 0xc1], ecx
        jne    loc_1_07C0
        cmp    dword ptr [ebx + 0xb5], 0
        jbe    loc_1_07C0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_07C0:
        sub    ecx, dword ptr [ebx + 0xc9]
        sar    ecx, 3
        mov    edx, dword ptr [ebx + 0xc5]
        lea    eax, [ecx + ecx*2]
        lea    edx, [edx + eax*4]
        lea    esi, [ebp - 0x24]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp - 0x20]
        shl    eax, 3
        add    eax, dword ptr [ebx + 0xc9]
        mov    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebp - 0x24]
        mov    dword ptr [ebp - 0x24], 0
        lea    eax, [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3]
        mov    byte ptr [ebp - 1], al
        test   al, 0x80
        je     loc_1_0820
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0820:
        mov    byte ptr [ebp - 2], 1
        lea    edi, [ecx*8]
        add    edi, dword ptr [ebx + 0xc9]
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        push   ebx
        call   sub_1_0A30
        mov    ecx, 0x100
        mov    edx, 0x40
loc_1_0845:
        mov    al, byte ptr [edi + 3]
        mov    byte ptr [ebp - 1], al
        test   byte ptr [ebp - 1], dl
        mov    ax, word ptr [edi + 6]
        mov    word ptr [ebp - 4], ax
        je     loc_1_085D
        test   cx, ax
        je     loc_1_08C8
loc_1_085D:
        mov    al, byte ptr [ebp - 1]
        and    al, 1
        test   word ptr [ebp - 4], cx
        je     loc_1_086E
        inc    dword ptr [ebx + 0x120]
loc_1_086E:
        test   byte ptr [ebp - 1], 2
        je     loc_1_089C
        test   byte ptr [ebp - 1], 4
        je     loc_1_0882
        inc    dword ptr [ebx + 0x128]
        jmp    loc_1_089C
loc_1_0882:
        test   byte ptr [ebp - 1], 8
        je     loc_1_0890
        inc    dword ptr [ebx + 0x12c]
        jmp    loc_1_089C
loc_1_0890:
        test   byte ptr [ebp - 1], 0x10
        je     loc_1_089C
        inc    dword ptr [ebx + 0x130]
loc_1_089C:
        cmp    dword ptr [ebx + 0xcd], edi
        jne    loc_1_08B0
        mov    edi, dword ptr [ebx + 0xc9]
        jmp    loc_1_08B3
        db  08Dh, 064h, 024h, 000h
loc_1_08B0:
        add    edi, 8
loc_1_08B3:
        mov    dword ptr [ebx + 0xbd], edi
        inc    dword ptr [ebx + 0xb5]
        or     al, al
        je     loc_1_0845
        jmp    loc_1_094E
loc_1_08C8:
        test   byte ptr [ebp - 3], 4
        je     loc_1_08D6
        inc    dword ptr [ebx + 0x118]
        jmp    loc_1_08F0
loc_1_08D6:
        test   byte ptr [ebp - 3], 8
        je     loc_1_08E4
        inc    dword ptr [ebx + 0x11c]
        jmp    loc_1_08F0
loc_1_08E4:
        test   byte ptr [ebp - 3], 0x40
        je     loc_1_08F0
        inc    dword ptr [ebx + 0x124]
loc_1_08F0:
        mov    byte ptr [ebp - 2], 0
        cmp    dword ptr [ebp - 8], edi
        jae    loc_1_0920
        mov    ecx, dword ptr [ebx + 0xcd]
        sub    ecx, edi
        sar    ecx, 3
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebx + 0xc9]
        sar    eax, 3
        add    ecx, eax
        add    ecx, 2
        add    dword ptr [ebx + 0xb5], ecx
        jmp    loc_1_092F
        db  08Dh, 049h, 000h
loc_1_0920:
        mov    eax, dword ptr [ebp - 8]
        sub    eax, edi
        sar    eax, 3
        inc    eax
        add    dword ptr [ebx + 0xb5], eax
loc_1_092F:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebx + 0xcd], eax
        jne    loc_1_0942
        mov    eax, dword ptr [ebx + 0xc9]
        jmp    loc_1_0948
loc_1_0942:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
loc_1_0948:
        mov    dword ptr [ebx + 0xbd], eax
loc_1_094E:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_095A
        inc    dword ptr [ebx + 0x140]
loc_1_095A:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 0xdd], eax
        or     eax, eax
        jne    loc_1_0973
        mov    dword ptr [ebx + 0xe1], 0
loc_1_0973:
        push   0
        lea    eax, [ebp - 0x14]
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_1A20
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_1A44
        mov    eax, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [eax + 4]
        test   byte ptr [edi + 0x35], 2
        jne    loc_1_09EA
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebx + 0xad]
        push   eax
        call   sub_1_1820
        or     al, al
        je     loc_1_09EA
        mov    eax, dword ptr [ebp - 0xc]
        mov    cl, byte ptr [ebp - 2]
        mov    byte ptr [eax + 0xa], cl
        cmp    dword ptr [ebx + 0xd5], 0
        jne    loc_1_09D0
        mov    dword ptr [ebx + 0xd5], esi
        jmp    loc_1_09D9
        db  08Bh, 0FFh
loc_1_09D0:
        mov    eax, dword ptr [ebx + 0xd9]
        mov    dword ptr [eax + 0x24], esi
loc_1_09D9:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], 0
        mov    dword ptr [ebx + 0xd9], esi
        jmp    loc_1_0A03
loc_1_09EA:
        cmp    byte ptr [ebp - 2], 1
        sbb    eax, eax
        and    eax, 0xc0000001
        push   eax
        push   esi
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_193A
        dec    dword ptr [edi + 0x18]
loc_1_0A03:
        cmp    byte ptr [ebx + 0xf9], 0
        jne    loc_1_0A1C
        cmp    byte ptr [ebx + 0x148], 0
        jne    loc_1_0A1C
        mov    byte ptr [ebx + 0xf9], 1
loc_1_0A1C:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, esi
        mov    edi, dword ptr [edx + 0xf5]
        shl    ecx, 4
        add    edi, ecx
        mov    eax, dword ptr [edi + 4]
        lea    eax, [edx + eax*4 + 0xe5]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edi + 4], ecx
        mov    dword ptr [eax], esi
        cmp    byte ptr [edx + 0xf9], 0
        jne    loc_1_0A72
        cmp    byte ptr [edx + 0x148], 0
        jne    loc_1_0A72
        mov    byte ptr [edx + 0xf9], 1
loc_1_0A72:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0A80:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [ecx]
        mov    dx, word ptr [eax + 0x39]
        mov    ax, word ptr [ecx + 8]
        out    dx, ax
        sub    al, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0AA0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [ecx]
        mov    dx, word ptr [eax + 0x3d]
        mov    ax, word ptr [ecx + 8]
        out    dx, ax
        sub    al, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [ecx]
        mov    ecx, dword ptr [ecx + 4]
        mov    dx, word ptr [eax + 0x3d]
        in     ax, dx
        mov    word ptr [ecx], ax
        sub    al, al
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0AE0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [ecx]
        mov    dx, word ptr [eax + 0x41]
        mov    ax, word ptr [ecx + 8]
        out    dx, ax
        sub    al, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0B00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x17a], 0
        je     loc_1_0C56
        cmp    dword ptr [esi + 0xdd], 0
        jne    loc_1_0B3E
        cmp    dword ptr [esi + 0xfc], 0
        jne    loc_1_0B3E
        cmp    dword ptr [esi + 0x156], 0
        jne    loc_1_0B3E
        cmp    dword ptr [esi + 0x14e], 0
        je     loc_1_0C56
loc_1_0B3E:
        mov    byte ptr [esi + 0x17a], 0
        push   0
        push   0xaa000003
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    edi, dword ptr [esi + 0x14a]
        or     edi, edi
        je     loc_1_0B87
        cmp    dword ptr [esi + 0x152], 4
        je     loc_1_0B87
        mov    eax, dword ptr [esi + 0x14e]
        or     eax, eax
        je     loc_1_0B87
        push   0
        push   eax
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_194C
        dec    dword ptr [edi + 0x18]
loc_1_0B87:
        cmp    dword ptr [esi + 0x156], 0
        je     loc_1_0BCD
        sub    edi, edi
loc_1_0B92:
        mov    ecx, dword ptr [esi + 0x156]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x156], eax
        cmp    eax, edi
        jne    loc_1_0BAA
        mov    dword ptr [esi + 0x15a], edi
loc_1_0BAA:
        mov    ebx, dword ptr [ecx + 4]
        cmp    ebx, edi
        je     loc_1_0BC5
        cmp    dword ptr [ecx + 8], 4
        je     loc_1_0BC5
        push   edi
        push   ecx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_194C
        dec    dword ptr [ebx + 0x18]
loc_1_0BC5:
        cmp    dword ptr [esi + 0x156], edi
        jne    loc_1_0B92
loc_1_0BCD:
        cmp    dword ptr [esi + 0xdd], 0
        je     loc_1_0C0D
        sub    edi, edi
loc_1_0BD8:
        mov    edx, dword ptr [esi + 0xdd]
        mov    eax, dword ptr [edx + 0x24]
        lea    ecx, [edx + 0x24]
        mov    dword ptr [esi + 0xdd], eax
        cmp    eax, edi
        jne    loc_1_0BF4
        mov    dword ptr [esi + 0xe1], edi
loc_1_0BF4:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_193A
        dec    dword ptr [ebx + 0x18]
        cmp    dword ptr [esi + 0xdd], edi
        jne    loc_1_0BD8
loc_1_0C0D:
        cmp    dword ptr [esi + 0xfc], 0
        je     loc_1_0C4D
        sub    edi, edi
loc_1_0C18:
        mov    edx, dword ptr [esi + 0xfc]
        mov    eax, dword ptr [edx + 0x24]
        lea    ecx, [edx + 0x24]
        mov    dword ptr [esi + 0xfc], eax
        cmp    eax, edi
        jne    loc_1_0C34
        mov    dword ptr [esi + 0x100], edi
loc_1_0C34:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_193A
        dec    dword ptr [ebx + 0x18]
        cmp    dword ptr [esi + 0xfc], edi
        jne    loc_1_0C18
loc_1_0C4D:
        mov    byte ptr [esi + 0x17a], 0
        jmp    loc_1_0C81
loc_1_0C56:
        cmp    dword ptr [esi + 0xdd], 0
        jne    loc_1_0C7A
        cmp    dword ptr [esi + 0xfc], 0
        jne    loc_1_0C7A
        cmp    dword ptr [esi + 0x156], 0
        jne    loc_1_0C7A
        cmp    dword ptr [esi + 0x14e], 0
        je     loc_1_0C81
loc_1_0C7A:
        mov    byte ptr [esi + 0x17a], 1
loc_1_0C81:
        push   0x1388
        add    esi, 0x16e
        push   esi
        call   sub_1_19D2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0CA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0x103
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [esi + 0x169], 0
        je     loc_1_0CD0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_0CD0:
        inc    dword ptr [esi + 0x80]
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_1_0D60
        cmp    byte ptr [edi + 0x1c], 0
        jne    loc_1_0D53
        inc    dword ptr [edi + 0x18]
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_1_1A20
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_0D47
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_1_0D47
        push   ebx
        push   edi
        push   esi
        call   sub_1_0DE0
        inc    dword ptr [edi + 0x18]
        cmp    byte ptr [esi + 0xfa], 0
        jne    loc_1_0D4E
        sub    ebx, ebx
loc_1_0D24:
        cmp    dword ptr [esi + 0xfc], ebx
        je     loc_1_0D4E
        cmp    byte ptr [esi + 0xf9], bl
        je     loc_1_0D4E
        push   esi
        call   sub_1_0E30
        cmp    byte ptr [esi + 0xfa], bl
        je     loc_1_0D24
        dec    dword ptr [edi + 0x18]
        jmp    loc_1_0D87
loc_1_0D47:
        mov    dword ptr [ebp - 4], 0xc000009a
loc_1_0D4E:
        dec    dword ptr [edi + 0x18]
        jmp    loc_1_0D87
loc_1_0D53:
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_0D87
        db  08Dh, 064h, 024h, 000h
loc_1_0D60:
        cmp    dword ptr [esi + 0x152], 8
        jne    loc_1_0D72
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_0D87
loc_1_0D72:
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        push   edi
        push   esi
        call   sub_1_0DE0
        inc    dword ptr [edi + 0x18]
        mov    dword ptr [ebp - 4], 0x103
loc_1_0D87:
        mov    eax, dword ptr [esi + 0x80]
        dec    eax
        mov    dword ptr [esi + 0x80], eax
        jne    loc_1_0DC5
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_1_0DB7
        cmp    dword ptr [esi + 0xd5], 0
        jne    loc_1_0DB7
        mov    eax, dword ptr [esi + 0x8c]
        sub    eax, esi
        cmp    eax, 0x8c
        je     loc_1_0DC5
loc_1_0DB7:
        push   0
        add    esi, 0xa0
        push   esi
        call   sub_1_19D2
loc_1_0DC5:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DE0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [edx + 0x2c], 0
        mov    dword ptr [edx + 0x28], eax
        lea    esi, [edx + 0x24]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x100]
        or     ecx, ecx
        jne    loc_1_0E10
        mov    dword ptr [eax + 0xfc], edx
        jmp    loc_1_0E13
        db  005h, 000h, 000h, 000h, 000h
loc_1_0E10:
        mov    dword ptr [ecx + 0x24], edx
loc_1_0E13:
        mov    dword ptr [eax + 0x100], edx
        mov    dword ptr [esi], 0
        pop    esi
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0xb5], 0
        jne    loc_1_0E55
        mov    byte ptr [esi + 0xf9], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0E55:
        mov    byte ptr [esi + 0xfa], 1
        mov    edi, dword ptr [esi + 0xfc]
        lea    eax, [ebp - 0x10]
        push   eax
        push   0
        push   0
        push   0
        push   edi
        call   sub_1_1A20
        cmp    dword ptr [ebp - 0x10], 0x40
        ja     loc_1_0E80
        mov    al, 1
        jmp    loc_1_0E8D
        db  08Dh, 064h, 024h, 000h
loc_1_0E80:
        cmp    dword ptr [ebp - 0x10], 0x100
        mov    al, 2
        jbe    loc_1_0E8D
        mov    al, 3
loc_1_0E8D:
        xor    ecx, ecx
        mov    cl, al
        cmp    ecx, 3
        jg     loc_1_0ED0
        lea    eax, [esi + ecx*4 + 0xe5]
        lea    ecx, [ecx]
loc_1_0EA0:
        mov    edx, dword ptr [eax]
        cmp    edx, -1
        jne    loc_1_0EB2
        add    eax, 4
        inc    ecx
        cmp    ecx, 3
        jle    loc_1_0EA0
        jmp    loc_1_0ED3
loc_1_0EB2:
        mov    eax, dword ptr [esi + 0xf5]
        mov    ebx, edx
        shl    ebx, 4
        add    eax, ebx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + ecx*4 + 0xe5], eax
        jmp    loc_1_0ED3
        db  08Bh, 0FFh
loc_1_0ED0:
        mov    edx, dword ptr [ebp - 0x14]
loc_1_0ED3:
        cmp    edx, -1
        jne    loc_1_0EF0
        mov    byte ptr [esi + 0xf9], 0
        mov    byte ptr [esi + 0xfa], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0EF0:
        lea    eax, [edi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        mov    word ptr [eax + 8], dx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0xfc], eax
        or     eax, eax
        jne    loc_1_0F10
        mov    dword ptr [esi + 0x100], 0
loc_1_0F10:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 4], ecx
        mov    ebx, dword ptr [esi + 0xb9]
        mov    ecx, dword ptr [esi + 0xc9]
        mov    eax, ebx
        sub    eax, ecx
        sar    eax, 3
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 0x60]
        dec    eax
        mov    edx, dword ptr [ebp - 4]
        inc    edx
        and    eax, edx
        lea    eax, [ecx + eax*8]
        mov    dword ptr [esi + 0xb9], eax
        dec    dword ptr [esi + 0xb5]
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_1030
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0xc5]
        lea    eax, [eax + eax*2]
        lea    ecx, [ecx + eax*4]
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], edi
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 4], 0
        mov    eax, dword ptr [eax + 0xc]
        not    ax
        inc    ax
        mov    word ptr [ebx + 4], ax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [esi + 0x21]
        add    eax, dword ptr [esi + 0x29]
        mov    word ptr [ebx], ax
        shr    eax, 0x10
        mov    byte ptr [ebx + 2], al
        or     byte ptr [ebx + 3], 0x83
        mov    eax, dword ptr [esi + 0x60]
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 1
        jne    loc_1_0FC0
        mov    eax, dword ptr [esi + 0xc9]
        jmp    loc_1_0FCF
        db  08Dh, 049h, 000h
loc_1_0FC0:
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [esi + 0xc9]
        add    eax, 8
loc_1_0FCF:
        mov    dword ptr [esi + 0xc1], eax
        mov    dword ptr [ebp - 0x20], esi
        mov    word ptr [ebp - 0x18], 0x48
        lea    eax, [ebp - 0x20]
        push   eax
        push   0xaa0                         ; -> loc_1_0AA0
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_19EA
        mov    eax, dword ptr [esi + 0xe1]
        or     eax, eax
        je     loc_1_0FFD
        mov    dword ptr [eax + 0x24], edi
loc_1_0FFD:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0xe1], edi
        cmp    dword ptr [esi + 0xdd], 0
        jne    loc_1_101B
        mov    dword ptr [esi + 0xdd], edi
loc_1_101B:
        mov    byte ptr [esi + 0xfa], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_1030:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        lea    eax, [ebp - 0x10]
        mov    edi, dword ptr [esi]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_1A20
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_108F
        sub    ebx, ebx
loc_1_105B:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1A44
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   edi
        call   sub_1_1ADC
        add    edi, dword ptr [ebp - 8]
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1A68
        cmp    dword ptr [ebp - 4], ebx
        jne    loc_1_105B
loc_1_108F:
        cmp    dword ptr [ebp - 0x10], 0x3c
        jae    loc_1_10C0
        mov    edx, 0x3c
        sub    edx, dword ptr [ebp - 0x10]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    dword ptr [esi + 0xc], 0x3c
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_10C0:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [esi + 0xc], eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
sub_1_10D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_119A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A20
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_119A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1A44
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_1195
        mov    edi, dword ptr [ebp + 0xc]
        mov    edi, edi
loc_1_1120:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_114C
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1A68
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1195
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1A44
        jmp    loc_1_1190
loc_1_114C:
        or     edi, edi
        je     loc_1_1169
        cmp    edi, dword ptr [ebp - 4]
        jbe    loc_1_1161
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1190
loc_1_1161:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1169:
        mov    ebx, dword ptr [ebp + 0x10]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     loc_1_1177
        mov    ebx, eax
loc_1_1177:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        add    esi, ebx
        push   eax
        call   sub_1_1ADC
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1190:
        cmp    dword ptr [ebp + 0x10], esi
        ja     loc_1_1120
loc_1_1195:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_119A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 0xd5]
        mov    dword ptr [ebx + 0x15e], esi
        lea    edi, [esi + 0x24]
        mov    byte ptr [ebx + 0xfb], 1
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [edi], 0
        jne    loc_1_11E7
        mov    dword ptr [ebx + 0xd9], 0
loc_1_11E7:
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 0xd5], eax
        mov    dword ptr [ebx + 0x15e], esi
        lea    eax, [ebp - 0xc]
        push   eax
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_1A20
        mov    eax, dword ptr [ebx + 0x162]
        add    eax, 0xe
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    eax, ecx
        jb     loc_1_1217
        mov    eax, ecx
loc_1_1217:
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebx + 0x186]
        push   eax
        push   0xea
        push   0
        push   esi
        call   sub_1_10D0
        cmp    dword ptr [ebp - 4], 0xe
        jae    loc_1_124A
        cmp    dword ptr [ebp - 4], 6
        jb     loc_1_1276
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jmp    loc_1_1261
loc_1_124A:
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0xe
        push   eax
        lea    eax, [ebx + 0x194]
        push   eax
        push   0xe
loc_1_1261:
        lea    eax, [ebx + 0x186]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 0xad]
        push   eax
        call   sub_1_15A0
loc_1_1276:
        cmp    byte ptr [edi + 0xa], 1
        sbb    eax, eax
        and    eax, 0xc0000001
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   esi
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_193A
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        dec    dword ptr [eax + 0x18]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_12A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    dword ptr [ebp + 0x10], 0xe
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 0x80]
        cmp    byte ptr [eax + 0x1c], 0
        jne    loc_1_14A0
        inc    dword ptr [eax + 0x18]
        mov    ecx, dword ptr [ebp + 0xc]
        test   cl, 1
        jne    loc_1_12EB
        mov    edx, dword ptr [ebp + 0x1c]
        push   edx
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_1A6E
        jmp    loc_1_1494
loc_1_12EB:
        mov    eax, dword ptr [esi + 0x64]
        dec    eax
        mov    dword ptr [ebp - 0x24], eax
        mov    edx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 0xc], 0
        mov    edi, ecx
        mov    dword ptr [edx], 0
        and    edi, 0xfe
        shr    edi, 1
        and    ecx, 0x7f00
        shr    ecx, 8
        mov    dword ptr [ebp - 0x20], ecx
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_1A20
        cmp    dword ptr [ebp - 0x28], 0
        je     loc_1_1494
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1A44
        mov    eax, dword ptr [esi + 0x104]
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [eax + edi*4]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [edi*8]
        add    eax, dword ptr [esi + 0xd1]
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x20], edi
        jne    loc_1_1380
        mov    di, word ptr [eax + 6]
        and    edi, 0xfff
        sub    edi, 4
        jmp    loc_1_1383
        db  005h, 000h, 000h, 000h, 000h
loc_1_1380:
        mov    edi, dword ptr [esi + 0x68]
loc_1_1383:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_148C
loc_1_1390:
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_13C3
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1A68
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_148C
        lea    eax, [ebp - 0x14]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1A44
        jmp    loc_1_1480
loc_1_13C3:
        or     edi, edi
        jne    loc_1_1428
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0x20], eax
        je     loc_1_148C
        cmp    dword ptr [ebp - 0x24], eax
        jne    loc_1_13F0
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [esi + 0xd1]
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_13F7
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_13F0:
        inc    dword ptr [ebp - 4]
        add    dword ptr [ebp - 0x18], 8
loc_1_13F7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0x20], eax
        jne    loc_1_1414
        mov    eax, dword ptr [ebp - 0x18]
        mov    di, word ptr [eax + 6]
        and    edi, 0xfff
        sub    edi, 4
        sub    edi, dword ptr [ebp - 0xc]
        jmp    loc_1_1417
loc_1_1414:
        mov    edi, dword ptr [esi + 0x68]
loc_1_1417:
        mov    ecx, dword ptr [esi + 0x104]
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + eax*4]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1480
loc_1_1428:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_144D
        cmp    dword ptr [ebp + 0x10], edi
        jbe    loc_1_143A
        sub    dword ptr [ebp + 0x10], edi
        sub    edi, edi
        jmp    loc_1_1480
loc_1_143A:
        mov    eax, dword ptr [ebp - 8]
        add    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], eax
        sub    edi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp + 0x10], 0
loc_1_144D:
        mov    ebx, dword ptr [ebp + 0x14]
        sub    ebx, dword ptr [ebp - 0xc]
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 0x14]
        cmp    eax, ecx
        jb     loc_1_145E
        mov    eax, ecx
loc_1_145E:
        cmp    ebx, eax
        jb     loc_1_1464
        mov    ebx, eax
loc_1_1464:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        sub    edi, ebx
        push   eax
        call   sub_1_1ADC
        add    dword ptr [ebp - 0x1c], ebx
        add    dword ptr [ebp - 8], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 0x14], ebx
loc_1_1480:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_1_1390
loc_1_148C:
        mov    edx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [edx], eax
loc_1_1494:
        mov    eax, dword ptr [ebp + 8]
        sub    edi, edi
        dec    dword ptr [eax + 0x18]
        jmp    loc_1_14A5
        db  08Bh, 0FFh
loc_1_14A0:
        mov    edi, 0xc001000c
loc_1_14A5:
        mov    eax, dword ptr [esi + 0x80]
        dec    eax
        mov    dword ptr [esi + 0x80], eax
        jne    loc_1_14E3
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_1_14D5
        cmp    dword ptr [esi + 0xd5], 0
        jne    loc_1_14D5
        mov    eax, dword ptr [esi + 0x8c]
        sub    eax, esi
        cmp    eax, 0x8c
        je     loc_1_14E3
loc_1_14D5:
        push   0
        add    esi, 0xa0
        push   esi
        call   sub_1_19D2
loc_1_14E3:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_1_14F0:
        mov    al, byte ptr [0x1dcc]         ; -> obj1:0x1DCC
        cmp    al, 0
        jne    loc_1_1515
        inc    byte ptr [0x1dcc]             ; -> obj1:0x1DCC
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_1517
        push   0
        push   0
        call   sub_2_1590
        cmp    eax, 0
        jne    loc_1_1517
loc_1_1515:
        clc    
        ret    
loc_1_1517:
        stc    
        ret    
DECLAN_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_1528
        jmp    dword ptr [eax*4 + 0x152a]    ; -> obj1:0x152A
loc_1_1528:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_14F0
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_1528
        dd  loc_1_14F0
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_15A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
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
        call   sub_1_15D0
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_15D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_1670
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_1610
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_1605
        cmp    al, 0xff
        jne    loc_1_1605
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_1605
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_1677
loc_1_1605:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_1677
        db  08Bh, 0FFh
loc_1_1610:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_1660
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_1636
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_163D
loc_1_1636:
        mov    dword ptr [ebp - 0x10], 1
loc_1_163D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1650
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_167A
        db  08Dh, 064h, 024h, 000h
loc_1_1650:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_167A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1660:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_167A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1670:
        mov    dword ptr [ebp - 4], 0x20
loc_1_1677:
        mov    esi, dword ptr [ebp + 8]
loc_1_167A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_1771
loc_1_1685:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_16DD
        test   al, 2
        je     loc_1_16C9
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_37B0
        or     al, al
        je     loc_1_16C9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_16DD
loc_1_16C9:
        test   byte ptr [ebp - 4], 2
        je     loc_1_16D4
        test   bl, 4
        jne    loc_1_16DD
loc_1_16D4:
        test   bl, 0x20
        je     loc_1_1766
loc_1_16DD:
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [ebp + 0x24]
        push   eax
        mov    eax, dword ptr [ebp + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_1952
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_1766
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_171F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_171F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_1730
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_1736
        db  08Bh, 0FFh
loc_1_1730:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_1736:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_1745
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1958
loc_1_1745:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_19FC
        mov    edi, ebx
        jmp    loc_1_1769
loc_1_1766:
        mov    edi, dword ptr [edi + 0x10]
loc_1_1769:
        or     edi, edi
        jne    loc_1_1685
loc_1_1771:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1780:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1790
        pop    ebp
        ret    4
sub_1_1790:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_180E
loc_1_17A0:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_1807
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1958
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_1807
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_17CC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_17CC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_17E0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_17E6
        db  005h, 000h, 000h, 000h, 000h
loc_1_17E0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_17E6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 0x1c]
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_19FC
        mov    edi, ebx
        jmp    loc_1_180A
loc_1_1807:
        mov    edi, dword ptr [edi + 0x10]
loc_1_180A:
        or     edi, edi
        jne    loc_1_17A0
loc_1_180E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1820:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_1835
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1835:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_1871
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_1862
        cmp    bl, 0xff
        jne    loc_1_1862
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_1862
        test   al, 8
        je     loc_1_185B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_185B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_1862:
        test   al, 4
        jne    loc_1_186A
        test   al, 2
        je     loc_1_1889
loc_1_186A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1871:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_1889
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_1889
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1889:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h
        db  024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch
        db  000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_190A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_1910:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_1916:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_191C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_1922:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_1928:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_1934:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_193A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_1946:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_194C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_1952:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_1958:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_195E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_1964:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
sub_1_1976:
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_197C:
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
sub_1_19D2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_19D8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_19DE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_19E4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_19EA:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_19F0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_19F6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_19FC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_1A20:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_1A44:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_1A68:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_1A6E:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_1A74:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_1A7A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_1A80:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
sub_1_1A86:
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
sub_1_1AC2:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_1AC8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_1ADC:
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
sub_1_1AFC:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_1B1C
        or     ecx, ecx
        je     loc_1_1B32
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_1B30
loc_1_1B1C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_1B30
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_1B30:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_1B32:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_1B3C:
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
        add    byte ptr [0x10000101], cl
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0x1000201
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_1BB4:
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
        add    byte ptr [0x10000101], cl
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_1C0C:
        dec    esp
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x20], sp
        inc    esp
        inc    ebp
        xor    dh, byte ptr [eax]
        xor    dword ptr [eax], esp
        inc    ecx
        popal  
        jo     loc_1_1C91
        jb     0x1c4e
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C24:
        dec    esp
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x20], sp
        inc    esp
        inc    ebp
        xor    dword ptr [eax], esi
        xor    byte ptr [eax], ah
        inc    ecx
        popal  
        jo     0x1ca9
        jb     0x1c66
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C3C:
        dec    esp
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x20], sp
        inc    esp
        inc    ebp
        xor    al, 0x32
        xor    ah, byte ptr [eax]
        inc    ecx
        popal  
        jo     loc_1_1CC1
        jb     0x1c7e
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C54:
        dec    esp
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x20], sp
        inc    ecx
        popal  
        jo     loc_1_1CD3
        jb     loc_1_1C90
        add    byte ptr [eax], al
        inc    esp
        arpl   word ptr gs:[ecx + 0x6e], bp
        add    byte ptr [eax], al
loc_1_1C6C:
        inc    esp
        arpl   word ptr gs:[ecx + 0x6e], bp
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4f], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x41], cl
        pop    eax
        dec    ebp
loc_1_1C90:
        push   ebp
loc_1_1C91:
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C9C:
        dec    ebp
        inc    ecx
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
loc_1_1CC1:
        push   ebx
        add    byte ptr [eax], al
        dec    ecx
        dec    esi
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4e], cl
        push   esp
loc_1_1CD3:
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x64], al
        popal  
        jo     0x1d55
        jb     0x1d32
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     loc_1_1D61
        jb     0x1d3e
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[edx + 0x41], dl
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_1D00:
        push   edx
        inc    ecx
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        inc    esp
        inc    ebp
        xor    dh, byte ptr [eax]
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1D14:
        inc    esp
        inc    ebp
        xor    dh, byte ptr [eax]
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        inc    esp
        inc    ebp
        xor    dword ptr [eax], esi
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1D24:
        inc    esp
        inc    ebp
        xor    dword ptr [eax], esi
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        inc    ebp
        push   eax
        inc    ebx
        inc    ecx
        add    byte ptr [eax], al
        db  000h
loc_1_1D34:
        inc    esp
        inc    ebp
        push   eax
        inc    ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x43], al
        push   esp
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x43], al
        push   esp
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x34], al
        xor    dh, byte ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x34], al
        xor    dh, byte ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x32], al
        xor    byte ptr [eax], dh
loc_1_1D61:
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x32], al
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 0x31], al
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        db  000h
loc_1_1D74:
        inc    esp
        inc    ebp
        xor    dword ptr [eax], esi
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DECLAN  '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  DECLAN_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x3870  R-X
loc_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0x168], 1
        call   sub_2_1540
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x16e]
        push   eax
        call   sub_1_19D8
        push   0x2625a0
        call   sub_1_19F0
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_19E4
        mov    eax, dword ptr [esi + 0x21]
        mov    dword ptr [ebp - 8], eax
        and    dword ptr [ebp - 8], 0xffff0000
        or     dword ptr [ebp - 8], 0xbffe
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [esi + 0x17c]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 0x21]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 0x17e]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 0x182]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0xad]
        push   eax
        call   sub_2_3110
        mov    dx, word ptr [esi + 0x41]
        mov    ax, 4
        out    dx, ax
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1928
        push   esi
        call   sub_2_0140
        push   0
        push   0x27e
        push   esi
        call   sub_1_19FC
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_00C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0x1dd8]       ; -> obj1:0x1DD8
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_191C
        push   0
        mov    eax, dword ptr [0x1dd4]       ; -> obj1:0x1DD4
        push   eax
        call   sub_1_1910
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_00F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x148], 0
        jne    loc_2_0137
        mov    dword ptr [ebp - 0x18], edi
        mov    word ptr [ebp - 0x10], 0
        lea    esi, [edi + 1]
        lea    eax, [ebp - 0x18]
        push   eax
        push   0xa80                         ; -> loc_1_0A80
        push   esi
        call   sub_1_19EA
        mov    dword ptr [ebp - 0xc], edi
        mov    word ptr [ebp - 4], 0x42
        lea    eax, [ebp - 0xc]
        push   eax
        push   0xaa0                         ; -> loc_1_0AA0
        push   esi
        call   sub_1_19EA
loc_2_0137:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_0140:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x6c]
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [esi + 0x70]
        mov    ebx, dword ptr [esi + 0x74]
        cmp    dword ptr [esi + 0x104], 0
        je     loc_2_0193
        sub    eax, eax
        mov    edx, dword ptr [esi + 0x64]
        cmp    edx, eax
        je     loc_2_017D
        mov    ecx, dword ptr [esi + 0x104]
loc_2_0170:
        cmp    dword ptr [ecx], 0
        je     loc_2_017D
        add    ecx, 4
        inc    eax
        cmp    edx, eax
        ja     loc_2_0170
loc_2_017D:
        push   0
        lea    eax, [edx*4]
        push   eax
        mov    eax, dword ptr [esi + 0x104]
        push   eax
        call   sub_1_19FC
loc_2_0193:
        mov    eax, dword ptr [esi + 0xc5]
        or     eax, eax
        je     loc_2_01AF
        mov    ecx, dword ptr [esi + 0x60]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_19FC
loc_2_01AF:
        mov    edx, dword ptr [esi + 0xf5]
        or     edx, edx
        je     loc_2_01E0
        sub    ecx, ecx
        lea    eax, [edi + ebx]
        mov    esi, dword ptr [ebp - 4]
        add    esi, eax
        je     loc_2_01D4
        mov    eax, edx
loc_2_01C7:
        cmp    dword ptr [eax], 0
        je     loc_2_01D4
        add    eax, 0x10
        inc    ecx
        cmp    ecx, esi
        jb     loc_2_01C7
loc_2_01D4:
        push   0
        shl    esi, 4
        push   esi
        push   edx
        call   sub_1_19FC
loc_2_01E0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_01F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, dword ptr [ecx + 0x84]
        lea    esi, [ecx + 0x84]
        sub    eax, ecx
        cmp    eax, 0x84
        je     loc_2_0226
        mov    edi, edi
loc_2_0210:
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, edx
        jbe    loc_2_021B
        mov    edx, eax
loc_2_021B:
        mov    eax, dword ptr [esi]
        sub    eax, ecx
        cmp    eax, 0x84
        jne    loc_2_0210
loc_2_0226:
        or     edx, edx
        jne    loc_2_022F
        mov    edx, 0xea
loc_2_022F:
        mov    dword ptr [ecx + 0x162], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0240:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        inc    dword ptr [esi + 0x80]
        cmp    byte ptr [ebx + 0x1c], 0
        jne    loc_2_0380
        inc    dword ptr [ebx + 0x18]
        push   0
        mov    eax, dword ptr [ebx + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0xad]
        push   eax
        call   sub_2_3230
        mov    edi, eax
        or     edi, edi
        jne    loc_2_02F0
        cmp    dword ptr [ebx + 0x18], 2
        jne    loc_2_02B0
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0x162]
        cmp    dword ptr [ebx + 0x14], eax
        jne    loc_2_029D
        push   esi
        call   sub_2_01F0
loc_2_029D:
        push   0
        push   0x39
        push   ebx
        call   sub_1_19FC
        jmp    loc_2_0385
        db  08Dh, 064h, 024h, 000h
loc_2_02B0:
        mov    byte ptr [ebx + 0x1c], 1
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 0x8c]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0x90]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0x90]
        mov    edi, 0x103
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0x90], ebx
        sub    dword ptr [ebx + 0x18], 2
        jmp    loc_2_0385
loc_2_02F0:
        cmp    edi, 0x103
        jne    loc_2_0330
        mov    byte ptr [ebx + 0x1c], 1
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 0x8c]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0x90]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0x90]
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0x90], ebx
        sub    dword ptr [ebx + 0x18], 2
        jmp    loc_2_0385
loc_2_0330:
        cmp    edi, 0xc001000e
        jne    loc_2_0374
        mov    byte ptr [ebx + 0x1c], 1
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    edi, 0x103
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 0x8c]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 0x90]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0x90]
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0x90], ebx
        dec    dword ptr [ebx + 0x18]
        jmp    loc_2_0385
loc_2_0374:
        dec    dword ptr [ebx + 0x18]
        jmp    loc_2_0385
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0380:
        mov    edi, 0xc0010002
loc_2_0385:
        mov    eax, dword ptr [esi + 0x80]
        dec    eax
        mov    dword ptr [esi + 0x80], eax
        jne    loc_2_03C3
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_2_03B5
        cmp    dword ptr [esi + 0xd5], 0
        jne    loc_2_03B5
        mov    eax, dword ptr [esi + 0x8c]
        sub    eax, esi
        cmp    eax, 0x8c
        je     loc_2_03C3
loc_2_03B5:
        push   0
        add    esi, 0xa0
        push   esi
        call   sub_1_19D2
loc_2_03C3:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_03D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 8]
        cmp    byte ptr [esi + 0x169], 0
        je     loc_2_03F0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  090h
loc_2_03F0:
        mov    edx, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x80]
        mov    eax, dword ptr [edx + 0x10]
        or     eax, eax
        jne    loc_2_0410
        push   edx
        push   ecx
        push   esi
        call   sub_2_0850
        jmp    loc_2_0422
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0410:
        cmp    eax, 1
        mov    edi, 0x10001
        jne    loc_2_0424
        push   edx
        push   ecx
        push   esi
        call   sub_2_08A0
loc_2_0422:
        mov    edi, eax
loc_2_0424:
        mov    eax, dword ptr [esi + 0x80]
        dec    eax
        mov    dword ptr [esi + 0x80], eax
        jne    loc_2_0462
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_2_0454
        cmp    dword ptr [esi + 0xd5], 0
        jne    loc_2_0454
        mov    eax, dword ptr [esi + 0x8c]
        sub    eax, esi
        cmp    eax, 0x8c
        je     loc_2_0462
loc_2_0454:
        push   0
        add    esi, 0xa0
        push   esi
        call   sub_1_19D2
loc_2_0462:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0470:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x18], 0
        lea    ebx, [ebp - 8]
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x14], 0
        mov    edi, 4
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x1010101
        ja     loc_2_04C0
        je     loc_2_06A0
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_2_04E1
        shl    eax, 2
        jmp    dword ptr [eax + 0x800]       ; -> obj2:0x0800
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_04C0:
        cmp    eax, 0x1010102
        je     loc_2_06C0
        cmp    eax, 0x1010103
        je     loc_2_06E0
        cmp    eax, 0x1010104
        je     loc_2_07B0
loc_2_04E1:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_2_07BE
        db  08Dh, 049h, 000h
loc_2_04F0:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_2_0505
        mov    ebx, 0x1bb4                   ; -> loc_1_1BB4
        mov    edi, 0x58
        jmp    loc_2_07BE
loc_2_0505:
        mov    ebx, 0x1b3c                   ; -> loc_1_1B3C
        mov    edi, 0x78
        jmp    loc_2_07BE
loc_2_0514:
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x148], 0
        mov    dword ptr [ebp - 0x14], 2
        jne    loc_2_052E
        mov    dword ptr [ebp - 0x14], 0
loc_2_052E:
        lea    ebx, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_2_07BE
        db  005h, 000h, 000h, 000h, 000h
loc_2_0540:
        lea    ebx, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_2_07BE
        db  08Dh, 049h, 000h
loc_2_0550:
        mov    dword ptr [ebp - 8], 0xea
        jmp    loc_2_07BE
        db  08Dh, 064h, 024h, 000h
loc_2_0560:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_2_07BE
        db  08Dh, 064h, 024h, 000h
loc_2_0570:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_2_07BE
        db  08Dh, 064h, 024h, 000h
loc_2_0580:
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x70]
        shl    edx, 2
        add    edx, dword ptr [esi + 0x6c]
        mov    eax, dword ptr [esi + 0x74]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*4]
        shl    edx, 6
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*4]
        lea    edx, [edx + eax*2]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_2_07BE
        db  005h, 000h, 000h, 000h, 000h
loc_2_05B0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x68]
        imul   eax, dword ptr [esi + 0x64]
        jmp    loc_2_07BB
        db  090h
loc_2_05C0:
        mov    dword ptr [ebp - 8], 0x40
        jmp    loc_2_07BE
        db  08Dh, 064h, 024h, 000h
loc_2_05D0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x68]
        jmp    loc_2_07BB
loc_2_05DB:
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi]
        cmp    al, 2
        jne    loc_2_05F3
        mov    ebx, 0x1c0c                   ; -> loc_1_1C0C
        mov    edi, 0x15
        jmp    loc_2_07BE
loc_2_05F3:
        cmp    al, 1
        jne    loc_2_0606
        mov    ebx, 0x1c24                   ; -> loc_1_1C24
        mov    edi, 0x15
        jmp    loc_2_07BE
loc_2_0606:
        cmp    al, 0x10
        jne    loc_2_0620
        mov    ebx, 0x1c3c                   ; -> loc_1_1C3C
        mov    edi, 0x15
        jmp    loc_2_07BE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0620:
        mov    ebx, 0x1c54                   ; -> loc_1_1C54
        mov    edi, 0xf
        jmp    loc_2_07BE
        db  090h
loc_2_0630:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0647
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0xad]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_2_07BB
loc_2_0647:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        jmp    loc_2_07BB
loc_2_0655:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0669
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x162]
        jmp    loc_2_07BB
loc_2_0669:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        jmp    loc_2_07BB
loc_2_0674:
        mov    word ptr [ebp - 2], 0x301
        lea    ebx, [ebp - 2]
        mov    edi, 2
        jmp    loc_2_07BE
loc_2_0687:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_2_07BE
loc_2_0693:
        mov    dword ptr [ebp - 8], 0xe
        jmp    loc_2_07BE
        db  090h
loc_2_06A0:
        push   6
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0x50
        push   esi
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_1ADC
        lea    ebx, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_2_07BE
        db  090h
loc_2_06C0:
        push   6
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0x56
        push   esi
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_1ADC
        lea    ebx, [ebp - 0x20]
        mov    edi, 6
        jmp    loc_2_07BE
        db  090h
loc_2_06E0:
        cmp    byte ptr [ebp + 0x14], 0
        mov    esi, dword ptr [ebp + 8]
        je     loc_2_0744
        mov    eax, dword ptr [esi + 0xad]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    ecx, dword ptr [ebp + 0x1c]
        jbe    loc_2_0715
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_2_07BE
loc_2_0715:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0xad]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_3750
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        sub    ebx, ebx
        sub    edi, edi
        jmp    loc_2_07BE
loc_2_0744:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0xad]
        push   eax
        call   sub_2_3650
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    ecx, dword ptr [ebp + 0x1c]
        jbe    loc_2_0777
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0xc], 0xc0010014
        jmp    loc_2_07BE
loc_2_0777:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0xad]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_3690
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        sub    ebx, ebx
        sub    edi, edi
        jmp    loc_2_07BE
        db  08Dh, 049h, 000h
loc_2_07B0:
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x166]
loc_2_07BB:
        mov    dword ptr [ebp - 8], eax
loc_2_07BE:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_07F1
        cmp    edi, dword ptr [ebp + 0x1c]
        jbe    loc_2_07E1
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edi
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_2_07E1:
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_1ADC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_2_07F1:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
        dd  loc_2_04F0               ; fixup
        dd  loc_2_0514               ; fixup
        dd  loc_2_0540               ; fixup
        dd  loc_2_0540               ; fixup
        dd  loc_2_0550               ; fixup
        dd  loc_2_0560               ; fixup
        dd  loc_2_0570               ; fixup
        dd  loc_2_0580               ; fixup
        dd  loc_2_05B0               ; fixup
        dd  loc_2_05C0               ; fixup
        dd  loc_2_05D0               ; fixup
        dd  loc_2_04E1               ; fixup
        dd  loc_2_05DB               ; fixup
        dd  loc_2_0630               ; fixup
        dd  loc_2_0655               ; fixup
        dd  loc_2_0674               ; fixup
        dd  loc_2_0687               ; fixup
        dd  loc_2_04E1               ; fixup
        dd  loc_2_0693               ; fixup
        db  08Dh, 064h, 024h, 000h
sub_2_0850:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0470
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_08A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010e
        je     loc_2_08FA
        cmp    eax, 0x1010f
        je     loc_2_0960
        cmp    eax, 0x10112
        je     loc_2_09F0
        cmp    eax, 0x1010103
        je     loc_2_0A20
        mov    edi, 0xc0010017
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_0A8A
loc_2_08FA:
        cmp    dword ptr [ebp - 4], 4
        je     loc_2_0918
        mov    edi, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_0A8A
loc_2_0918:
        push   4
        push   ecx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1ADC
        test   dword ptr [ebp - 0xc], 0xf050
        je     loc_2_0945
        mov    edi, 0xc00000bb
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_0A8A
loc_2_0945:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   esi
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_2_0AB0
        mov    edi, eax
        jmp    loc_2_0A8A
        db  08Bh, 0FFh
loc_2_0960:
        cmp    dword ptr [ebp - 4], 4
        je     loc_2_0980
        mov    edi, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_0A8A
        db  08Bh, 0FFh
loc_2_0980:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1ADC
        cmp    dword ptr [ebp - 8], 0xea
        ja     loc_2_09E3
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebx + 0x14]
        mov    eax, dword ptr [ecx + 0x162]
        cmp    edx, eax
        jbe    loc_2_09C0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 0x14], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x162], eax
        jmp    loc_2_0A8A
        db  08Dh, 064h, 024h, 000h
loc_2_09C0:
        jne    loc_2_09D8
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_2_09D8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 0x14], eax
        push   ecx
        call   sub_2_01F0
        jmp    loc_2_0A8A
loc_2_09D8:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 0x14], eax
        jmp    loc_2_0A8A
loc_2_09E3:
        mov    edi, 0xc0010014
        jmp    loc_2_0A8A
        db  08Dh, 049h, 000h
loc_2_09F0:
        cmp    dword ptr [ebp - 4], 4
        je     loc_2_0A0B
        mov    edi, 0xc0010014
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_2_0A8A
loc_2_0A0B:
        push   4
        push   ecx
        mov    ebx, dword ptr [ebp + 0xc]
        add    ebx, 0x35
        push   ebx
        call   sub_1_1ADC
        sub    edi, edi
        jmp    loc_2_0A8A
        db  08Bh, 0FFh
loc_2_0A20:
        mov    edi, 6
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    edi
        or     edx, edx
        je     loc_2_0A50
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 0
        mov    eax, 0xc0010014
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
loc_2_0A50:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edi, 0xc0010002
        cmp    byte ptr [ebx + 0x1c], 0
        jne    loc_2_0A8A
        inc    dword ptr [ebx + 0x18]
        push   1
        push   ecx
        mov    ecx, 6
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 0xad]
        push   eax
        call   sub_2_32F0
        mov    edi, eax
        dec    dword ptr [ebx + 0x18]
loc_2_0A8A:
        or     edi, edi
        jne    loc_2_0A9B
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0
loc_2_0A9B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, 0xc0010002
        inc    dword ptr [esi + 0x80]
        cmp    byte ptr [edi + 0x1c], 0
        jne    loc_2_0AEC
        inc    dword ptr [edi + 0x18]
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0xad]
        push   eax
        call   sub_2_35D0
        dec    dword ptr [edi + 0x18]
loc_2_0AEC:
        dec    dword ptr [esi + 0x80]
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0B00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_0470
        mov    edi, eax
        cmp    edi, 0xc00000bb
        jne    loc_2_0C0D
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x20105
        ja     loc_2_0B86
        je     loc_2_0BC8
        cmp    eax, 0x20101
        je     loc_2_0BA2
        cmp    eax, 0x20102
        je     loc_2_0BB0
        cmp    eax, 0x20103
        je     loc_2_0BB8
        cmp    eax, 0x20104
        je     loc_2_0BC0
        mov    edi, 0xc0010017
        jmp    loc_2_0BE9
loc_2_0B86:
        cmp    eax, 0x1020101
        je     loc_2_0BD0
        cmp    eax, 0x1020102
        je     loc_2_0BD8
        cmp    eax, 0x1020103
        je     loc_2_0BE0
        mov    edi, 0xc0010017
        jmp    loc_2_0BE9
loc_2_0BA2:
        mov    eax, dword ptr [ebx + 0x140]
        add    eax, dword ptr [ebx + 0x120]
        jmp    loc_2_0BE6
loc_2_0BB0:
        mov    eax, dword ptr [ebx + 0x144]
        jmp    loc_2_0BE6
loc_2_0BB8:
        mov    eax, dword ptr [ebx + 0x11c]
        jmp    loc_2_0BE6
loc_2_0BC0:
        mov    eax, dword ptr [ebx + 0x110]
        jmp    loc_2_0BE6
loc_2_0BC8:
        mov    eax, dword ptr [ebx + 0x10c]
        jmp    loc_2_0BE6
loc_2_0BD0:
        mov    eax, dword ptr [ebx + 0x114]
        jmp    loc_2_0BE6
loc_2_0BD8:
        mov    eax, dword ptr [ebx + 0x12c]
        jmp    loc_2_0BE6
loc_2_0BE0:
        mov    eax, dword ptr [ebx + 0x130]
loc_2_0BE6:
        mov    dword ptr [ebp - 4], eax
loc_2_0BE9:
        or     edi, edi
        jne    loc_2_0C0D
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_2_0C08
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1ADC
        add    dword ptr [ebp - 8], 4
        jmp    loc_2_0C0D
loc_2_0C08:
        mov    edi, 0xc0010014
loc_2_0C0D:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0C30:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        sub    eax, eax
        mov    ecx, dword ptr [edx + 0x14]
        cmp    ecx, 0x20105
        ja     loc_2_0C75
        cmp    ecx, 0x20101
        jae    loc_2_0CA2
        cmp    ecx, 0x10101
        jb     loc_2_0C9D
        cmp    ecx, 0x1010b
        jbe    loc_2_0CA2
        cmp    ecx, 0x1010d
        jb     loc_2_0C9D
        cmp    ecx, 0x10111
        jbe    loc_2_0CA2
        cmp    ecx, 0x10113
        je     loc_2_0CA2
        jmp    loc_2_0C9D
loc_2_0C75:
        cmp    ecx, 0x1010101
        jb     loc_2_0C9D
        cmp    ecx, 0x1010104
        jbe    loc_2_0CA2
        cmp    ecx, 0x1020101
        jb     loc_2_0C9D
        cmp    ecx, 0x1020103
        jbe    loc_2_0CA2
        cmp    ecx, 0x2010103
        je     loc_2_0CA2
loc_2_0C9D:
        mov    eax, 0xc0010017
loc_2_0CA2:
        or     eax, eax
        jne    loc_2_0CB0
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0B00
loc_2_0CB0:
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0CC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0x103
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x80]
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_2_0D40
        cmp    byte ptr [edi + 0x1c], 0
        jne    loc_2_0D35
        inc    dword ptr [edi + 0x18]
        mov    ebx, dword ptr [esi + 0x84]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0xffffff7c
        je     loc_2_0D25
        nop    
loc_2_0D00:
        inc    dword ptr [ebx + 0x18]
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_195E
        dec    dword ptr [ebx + 0x18]
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0xffffff7c
        jne    loc_2_0D00
loc_2_0D25:
        push   8
        push   0
        push   edi
        push   esi
        call   sub_2_1460
        dec    dword ptr [edi + 0x18]
        jmp    loc_2_0D47
loc_2_0D35:
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_2_0D47
        db  08Bh, 0FFh
loc_2_0D40:
        mov    dword ptr [ebp - 4], 0
loc_2_0D47:
        mov    eax, dword ptr [esi + 0x80]
        dec    eax
        mov    dword ptr [esi + 0x80], eax
        jne    loc_2_0D85
        cmp    byte ptr [esi + 0x148], 0
        jne    loc_2_0D77
        cmp    dword ptr [esi + 0xd5], 0
        jne    loc_2_0D77
        mov    eax, dword ptr [esi + 0x8c]
        sub    eax, esi
        cmp    eax, 0x8c
        je     loc_2_0D85
loc_2_0D77:
        push   0
        add    esi, 0xa0
        push   esi
        call   sub_1_19D2
loc_2_0D85:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xcc
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    edi, dword ptr [esi + 0xb1]
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        sub    cl, cl
        nop    
loc_2_0DC0:
        xor    eax, eax
        mov    al, cl
        mov    dl, byte ptr [esi + eax + 0x56]
        mov    edi, dword ptr [esi + 0xb1]
        mov    byte ptr [eax + edi + 2], dl
        inc    cl
        cmp    cl, 6
        jb     loc_2_0DC0
        mov    ecx, dword ptr [esi + 0xc9]
        sub    ecx, dword ptr [esi + 0x21]
        mov    eax, dword ptr [esi + 0xb1]
        add    ecx, dword ptr [esi + 0x25]
        mov    word ptr [eax + 0x14], cx
        mov    eax, dword ptr [esi + 0xb1]
        shr    ecx, 0x10
        mov    byte ptr [eax + 0x16], cl
        mov    edi, dword ptr [esi + 0xd1]
        mov    eax, dword ptr [esi + 0x25]
        sub    eax, dword ptr [esi + 0x21]
        lea    ecx, [edi + eax]
        mov    byte ptr [ebp - 1], 0
        cmp    dword ptr [esi + 0x64], 0
        jbe    loc_2_0E2C
        mov    edx, 0x80
loc_2_0E19:
        mov    byte ptr [edi + 3], dl
        inc    byte ptr [ebp - 1]
        add    edi, 8
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        cmp    eax, dword ptr [esi + 0x64]
        jb     loc_2_0E19
loc_2_0E2C:
        mov    eax, dword ptr [esi + 0xb1]
        mov    word ptr [eax + 0x10], cx
        mov    eax, dword ptr [esi + 0xb1]
        shr    ecx, 0x10
        mov    byte ptr [eax + 0x12], cl
        mov    ecx, dword ptr [esi + 0x78]
        mov    eax, dword ptr [esi + 0xb1]
        shl    cl, 5
        mov    byte ptr [eax + 0x17], cl
        mov    ecx, dword ptr [esi + 0x7c]
        mov    eax, dword ptr [esi + 0xb1]
        shl    cl, 5
        mov    byte ptr [eax + 0x13], cl
        mov    ecx, dword ptr [esi + 0xad]
        mov    eax, dword ptr [ecx + 0xc]
        test   al, 0x20
        je     loc_2_0E90
        mov    eax, dword ptr [esi + 0xb1]
        pop    edi
        mov    ax, word ptr [eax]
        mov    cx, ax
        mov    eax, dword ptr [esi + 0xb1]
        or     cx, 0x8000
        mov    word ptr [eax], cx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_0E90:
        test   al, 4
        je     loc_2_0EF0
        mov    eax, dword ptr [esi + 0xb1]
        mov    byte ptr [eax + 8], 0xff
        mov    eax, dword ptr [esi + 0xb1]
        mov    byte ptr [eax + 9], 0xff
        mov    eax, dword ptr [esi + 0xb1]
        mov    byte ptr [eax + 0xa], 0xff
        mov    eax, dword ptr [esi + 0xb1]
        mov    byte ptr [eax + 0xb], 0xff
        mov    eax, dword ptr [esi + 0xb1]
        mov    byte ptr [eax + 0xc], 0xff
        mov    eax, dword ptr [esi + 0xb1]
        pop    edi
        mov    byte ptr [eax + 0xd], 0xff
        mov    eax, dword ptr [esi + 0xb1]
        mov    byte ptr [eax + 0xe], 0xff
        mov    eax, dword ptr [esi + 0xb1]
        pop    esi
        mov    byte ptr [eax + 0xf], 0xff
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_0EF0:
        test   al, 2
        je     loc_2_0FBF
        lea    eax, [ebp - 0xcc]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0xc0
        push   ecx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_3750
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_0FBF
        lea    esp, [esp]
loc_2_0F20:
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        lea    eax, [ebp + eax*2 - 0xd2]
        push   eax
        push   6
        call   sub_2_0FD0
        mov    edx, eax
        and    edx, 1
        cmp    edx, 1
        cmc    
        sbb    edx, edx
        and    edx, 0x20
        mov    edi, eax
        and    edi, 2
        cmp    edi, 1
        cmc    
        sbb    edi, edi
        and    edi, 0x10
        or     edx, edi
        mov    edi, eax
        and    edi, 4
        cmp    edi, 1
        cmc    
        sbb    edi, edi
        and    edi, 8
        or     edx, edi
        mov    edi, eax
        and    edi, 8
        cmp    edi, 1
        cmc    
        sbb    edi, edi
        and    edi, 4
        or     edx, edi
        mov    edi, eax
        and    edi, 0x10
        cmp    edi, 1
        cmc    
        sbb    edi, edi
        and    edi, 2
        or     edx, edi
        and    eax, 0x20
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        or     edx, eax
        mov    edi, edx
        shr    edi, 3
        mov    eax, dword ptr [esi + 0xb1]
        mov    al, byte ptr [edi + eax + 8]
        mov    byte ptr [ebp - 1], al
        mov    al, 1
        and    dl, 7
        mov    cl, dl
        shl    al, cl
        or     al, byte ptr [ebp - 1]
        mov    ecx, dword ptr [esi + 0xb1]
        mov    byte ptr [edi + ecx + 8], al
        dec    dword ptr [ebp - 8]
        jne    loc_2_0F20
loc_2_0FBF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0FD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0xffffffff
        cmp    dword ptr [ebp + 8], 0
        je     loc_2_1012
        mov    ecx, dword ptr [ebp + 0xc]
loc_2_0FE4:
        mov    dl, byte ptr [ecx]
        inc    ecx
        mov    esi, 8
loc_2_0FF0:
        mov    edi, eax
        shr    edi, 0x1f
        add    eax, eax
        mov    bl, dl
        and    ebx, 1
        cmp    ebx, edi
        je     loc_2_1008
        xor    eax, 0x4c11db6
        or     eax, 1
loc_2_1008:
        shr    dl, 1
        dec    esi
        jne    loc_2_0FF0
        dec    dword ptr [ebp + 8]
        jne    loc_2_0FE4
loc_2_1012:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1020:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [edx + 8]
        cmp    byte ptr [eax + 0x169], 0
        je     loc_2_103A
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x1c
loc_2_103A:
        mov    esi, dword ptr [ebp + 0x1c]
        or     esi, esi
        jne    loc_2_1044
        lea    esi, [edx + 0x1d]
loc_2_1044:
        cmp    byte ptr [eax + 0x148], 0
        je     loc_2_1090
        cmp    dword ptr [eax + 0x156], 0
        jne    loc_2_1064
        mov    dword ptr [eax + 0x15a], esi
        mov    dword ptr [eax + 0x156], esi
        jmp    loc_2_106C
loc_2_1064:
        mov    eax, dword ptr [eax + 0x15a]
        mov    dword ptr [eax], esi
loc_2_106C:
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], edx
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffb
        add    eax, 9
        mov    dword ptr [esi + 8], eax
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_2_1090:
        cmp    byte ptr [ebp + 0x20], 1
        sbb    ecx, ecx
        and    ecx, 0xfffffffb
        add    ecx, 9
        push   ecx
        push   esi
        push   edx
        push   eax
        call   sub_2_1460
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x1c
        db  090h
loc_2_10B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [edx + 8]
        cmp    byte ptr [eax + 0x169], 0
        je     loc_2_10CA
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0x14
loc_2_10CA:
        mov    esi, dword ptr [ebp + 0x14]
        or     esi, esi
        jne    loc_2_10D4
        lea    esi, [edx + 0x29]
loc_2_10D4:
        cmp    byte ptr [eax + 0x148], 0
        je     loc_2_1120
        cmp    dword ptr [eax + 0x156], 0
        jne    loc_2_10F4
        mov    dword ptr [eax + 0x15a], esi
        mov    dword ptr [eax + 0x156], esi
        jmp    loc_2_10FC
loc_2_10F4:
        mov    eax, dword ptr [eax + 0x15a]
        mov    dword ptr [eax], esi
loc_2_10FC:
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], edx
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        and    eax, 0xfffffffb
        add    eax, 9
        mov    dword ptr [esi + 8], eax
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_2_1120:
        cmp    byte ptr [ebp + 0x18], 1
        sbb    ecx, ecx
        and    ecx, 0xfffffffa
        add    ecx, 0xa
        push   ecx
        push   esi
        push   edx
        push   eax
        call   sub_2_1460
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    0x14
        db  090h
loc_2_1140:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 0x18]
        ret    4
        db  08Dh, 049h, 000h
sub_2_1150:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 3
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp - 8]
loc_2_1166:
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 1
        je     loc_2_1180
        cmp    eax, 2
        je     loc_2_1188
        cmp    eax, 3
        je     loc_2_1190
        jmp    loc_2_1196
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1180:
        mov    edi, dword ptr [esi + 0xdd]
        jmp    loc_2_1196
loc_2_1188:
        mov    edi, dword ptr [esi + 0xfc]
        jmp    loc_2_1196
loc_2_1190:
        mov    edi, dword ptr [esi + 0xd5]
loc_2_1196:
        or     edi, edi
        je     loc_2_11E6
loc_2_119A:
        lea    eax, [edi + 0x24]
        mov    ecx, edi
        mov    edi, dword ptr [eax]
        mov    ebx, dword ptr [eax + 4]
        cmp    dword ptr [esi + 0x152], 8
        je     loc_2_11D0
        cmp    dword ptr [ebp - 4], 1
        je     loc_2_11B9
        cmp    dword ptr [ebp - 4], 3
        jne    loc_2_11E2
loc_2_11B9:
        inc    dword ptr [esi + 0x140]
        push   0
        push   ecx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_193A
        dec    dword ptr [ebx + 0x18]
        jmp    loc_2_11E2
loc_2_11D0:
        dec    dword ptr [ebx + 0x18]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_193A
loc_2_11E2:
        or     edi, edi
        jne    loc_2_119A
loc_2_11E6:
        dec    dword ptr [ebp - 4]
        jne    loc_2_1166
        mov    word ptr [esi + 0x5e], 0
        mov    eax, dword ptr [esi + 0x60]
        mov    dword ptr [esi + 0xb5], eax
        mov    eax, dword ptr [esi + 0xc9]
        mov    dword ptr [esi + 0xb9], eax
        mov    dword ptr [esi + 0xbd], eax
        mov    dword ptr [esi + 0xc1], eax
        mov    edx, 0x80
        mov    byte ptr [esi + 0xf9], 1
        mov    byte ptr [esi + 0xfa], 0
        mov    dword ptr [esi + 0x108], 0
        mov    ecx, dword ptr [esi + 0xd1]
        mov    eax, dword ptr [esi + 0x64]
        lea    eax, [ecx + eax*8]
loc_2_1240:
        mov    byte ptr [ecx + 3], dl
        add    ecx, 8
        cmp    ecx, eax
        jne    loc_2_1240
        mov    ecx, dword ptr [esi + 0xc9]
        mov    eax, dword ptr [esi + 0x60]
        lea    eax, [ecx + eax*8]
        sub    edx, edx
loc_2_1258:
        mov    byte ptr [ecx + 3], dl
        add    ecx, 8
        cmp    ecx, eax
        jne    loc_2_1258
        sub    edx, edx
        mov    eax, dword ptr [esi + 0x6c]
        add    eax, dword ptr [esi + 0x70]
        add    eax, dword ptr [esi + 0x74]
        je     loc_2_128C
        sub    ecx, ecx
loc_2_1271:
        inc    edx
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [ecx + eax + 4], edx
        add    ecx, 0x10
        mov    eax, dword ptr [esi + 0x6c]
        add    eax, dword ptr [esi + 0x70]
        add    eax, dword ptr [esi + 0x74]
        cmp    eax, edx
        ja     loc_2_1271
loc_2_128C:
        mov    dword ptr [esi + 0xe5], 0xffffffff
        mov    dword ptr [esi + 0xe9], 0
        mov    eax, dword ptr [esi + 0x6c]
        shl    eax, 4
        mov    ecx, dword ptr [esi + 0xf5]
        mov    dword ptr [eax + ecx - 0xc], 0xffffffff
        mov    eax, dword ptr [esi + 0x6c]
        mov    dword ptr [esi + 0xed], eax
        mov    ecx, dword ptr [esi + 0x70]
        add    ecx, eax
        shl    ecx, 4
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [ecx + eax - 0xc], 0xffffffff
        mov    eax, dword ptr [esi + 0x6c]
        add    eax, dword ptr [esi + 0x70]
        mov    dword ptr [esi + 0xf1], eax
        mov    eax, dword ptr [esi + 0x70]
        add    eax, dword ptr [esi + 0x6c]
        add    eax, dword ptr [esi + 0x74]
        shl    eax, 4
        mov    ecx, dword ptr [esi + 0xf5]
        mov    dword ptr [eax + ecx - 0xc], 0xffffffff
        cmp    dword ptr [esi + 0x152], 8
        jne    loc_2_1340
        mov    dword ptr [esi + 0xd5], 0
        mov    dword ptr [esi + 0xd9], 0
        mov    dword ptr [esi + 0xdd], 0
        mov    dword ptr [esi + 0xe1], 0
        mov    dword ptr [esi + 0xfc], 0
        mov    dword ptr [esi + 0x100], 0
        jmp    loc_2_1388
loc_2_1340:
        mov    dword ptr [esi + 0xdd], 0
        mov    dword ptr [esi + 0xe1], 0
        mov    edi, dword ptr [esi + 0xfc]
        mov    dword ptr [esi + 0xfc], 0
        or     edi, edi
        mov    dword ptr [esi + 0x100], 0
        je     loc_2_1388
loc_2_1372:
        lea    ecx, [edi + 0x24]
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ecx + 4]
        push   eax
        push   esi
        call   sub_1_0DE0
        or     edi, edi
        jne    loc_2_1372
loc_2_1388:
        push   esi
        call   sub_2_13A0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_13A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x48
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_0DA0
        mov    ebx, dword ptr [edi + 0x25]
        sub    ebx, dword ptr [edi + 0x21]
        add    ebx, dword ptr [edi + 0xb1]
        mov    dword ptr [ebp - 0x48], edi
        lea    esi, [edi + 1]
        mov    word ptr [ebp - 0x40], 1
        lea    eax, [ebp - 0x48]
        push   eax
        push   0xa80                         ; -> loc_1_0A80
        push   esi
        call   sub_1_19EA
        mov    dword ptr [ebp - 0x3c], edi
        mov    word ptr [ebp - 0x34], bx
        lea    eax, [ebp - 0x3c]
        push   eax
        push   0xaa0                         ; -> loc_1_0AA0
        push   esi
        call   sub_1_19EA
        mov    dword ptr [ebp - 0x30], edi
        mov    word ptr [ebp - 0x28], 2
        lea    eax, [ebp - 0x30]
        push   eax
        push   0xa80                         ; -> loc_1_0A80
        push   esi
        call   sub_1_19EA
        mov    dword ptr [ebp - 0x24], edi
        shr    ebx, 0x10
        mov    al, bl
        and    eax, 0xffff00ff
        mov    word ptr [ebp - 0x1c], ax
        lea    eax, [ebp - 0x24]
        push   eax
        push   0xaa0                         ; -> loc_1_0AA0
        push   esi
        call   sub_1_19EA
        mov    dword ptr [ebp - 0x18], edi
        mov    word ptr [ebp - 0x10], 0
        lea    eax, [ebp - 0x18]
        push   eax
        push   0xa80                         ; -> loc_1_0A80
        push   esi
        call   sub_1_19EA
        mov    dword ptr [ebp - 0xc], edi
        mov    word ptr [ebp - 4], 0x41
        lea    eax, [ebp - 0xc]
        push   eax
        push   0xaa0                         ; -> loc_1_0AA0
        push   esi
        call   sub_1_19EA
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_1460:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   0x1540                        ; -> sub_2_1540
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_19EA
        mov    word ptr [esi + 0x5e], 0
        mov    byte ptr [esi + 0x148], 1
        mov    byte ptr [esi + 0x149], 0
        mov    byte ptr [esi + 0xf9], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x14e], eax
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        mov    dword ptr [esi + 0x14a], eax
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0x152], ecx
        je     loc_2_14B3
        inc    dword ptr [eax + 0x18]
loc_2_14B3:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_14C0:
        push   ebp
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ebp + 0xc], 0
        mov    esi, dword ptr [ebp + 8]
        je     loc_2_1500
        push   0
        mov    eax, dword ptr [esi + 0x14e]
        push   eax
        mov    eax, dword ptr [esi + 0x14a]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_194C
        mov    eax, dword ptr [esi + 0x14a]
        mov    dword ptr [esi + 0x14e], 0
        dec    dword ptr [eax + 0x18]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_1500:
        push   0xc0000001
        mov    eax, dword ptr [esi + 0x14e]
        push   eax
        mov    eax, dword ptr [esi + 0x14a]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_194C
        mov    eax, dword ptr [esi + 0x14a]
        mov    dword ptr [esi + 0x14e], 0
        dec    dword ptr [eax + 0x18]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1540:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    ax, ax
        mov    di, word ptr [ecx + 0x39]
        test   byte ptr [ecx], 0x13
        mov    dx, di
        out    dx, ax
        mov    si, word ptr [ecx + 0x3d]
        mov    dx, si
        mov    ax, 4
        out    dx, ax
        je     loc_2_1578
        mov    dx, di
        mov    ax, 3
        out    dx, ax
        mov    dx, si
        mov    ax, 2
        out    dx, ax
loc_2_1578:
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1590:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x50
        mov    word ptr [ebp - 0xc], 7
        mov    word ptr [ebp - 0xa], 7
        mov    dword ptr [ebp - 8], 0x1c6c   ; -> loc_1_1C6C
        mov    word ptr [ebp - 0x18], 1
        mov    word ptr [ebp - 0x16], 0
        mov    dword ptr [ebp - 0x14], 0x2042a310
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        push   0x1dd4                        ; -> obj1:0x1DD4
        call   sub_1_190A
        mov    byte ptr [ebp - 0x50], 3
        mov    byte ptr [ebp - 0x4f], 0
        mov    dword ptr [ebp - 0x4a], 0x2be0 ; -> loc_2_2BE0
        mov    dword ptr [ebp - 0x46], 0x240 ; -> loc_2_0240
        mov    dword ptr [ebp - 0x42], 0xca0 ; -> loc_1_0CA0
        mov    dword ptr [ebp - 0x3e], 0x12a0 ; -> loc_1_12A0
        mov    dword ptr [ebp - 0x3a], 0xcc0 ; -> loc_2_0CC0
        mov    dword ptr [ebp - 0x36], 0x3d0 ; -> loc_2_03D0
        mov    dword ptr [ebp - 0x2a], 0x1670 ; -> loc_2_1670
        mov    dword ptr [ebp - 0x2e], 0xc0  ; -> loc_2_00C0
        mov    dword ptr [ebp - 0x26], 0     ; -> loc_2_0000
        mov    dword ptr [ebp - 0x32], 0xc30 ; -> loc_2_0C30
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x22], eax
        lea    eax, [ebp - 0x50]
        mov    dword ptr [ebp - 0x1e], ecx
        push   0x36
        push   eax
        push   0x1dd8                        ; -> obj1:0x1DD8
        mov    eax, dword ptr [0x1dd4]       ; -> obj1:0x1DD4
        push   eax
        lea    eax, [ebp - 4]
        push   0x1dd8                        ; -> obj1:0x1DD8
        push   eax
        call   sub_1_1916
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_1657
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_1657:
        push   0
        mov    eax, dword ptr [0x1dd4]       ; -> obj1:0x1DD4
        push   eax
        call   sub_1_1910
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_2_1670:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x200
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x48], 0xa
        mov    word ptr [ebp - 0x46], 0xa
        mov    dword ptr [ebp - 0x44], 0x1c80 ; -> obj1:0x1C80
        mov    word ptr [ebp - 0x50], 0xd
        mov    word ptr [ebp - 0x4e], 0xd
        mov    dword ptr [ebp - 0x4c], 0x1c9c ; -> loc_1_1C9C
        mov    word ptr [ebp - 0x40], 0xa
        mov    word ptr [ebp - 0x3e], 0xa
        mov    dword ptr [ebp - 0x3c], 0x1cd0 ; -> obj1:0x1CD0
        mov    word ptr [ebp - 0x88], 0xc
        mov    word ptr [ebp - 0x86], 0xc
        mov    dword ptr [ebp - 0x84], 0x1ce8 ; -> obj1:0x1CE8
        mov    word ptr [ebp - 0x38], 0xb
        mov    word ptr [ebp - 0x36], 0xb
        mov    dword ptr [ebp - 0x34], 0x1d00 ; -> loc_1_1D00
        mov    word ptr [ebp - 0x80], 6
        mov    word ptr [ebp - 0x7e], 6
        mov    dword ptr [ebp - 0x7c], 0x1d14 ; -> loc_1_1D14
        mov    word ptr [ebp - 0x78], 6
        mov    word ptr [ebp - 0x76], 6
        mov    dword ptr [ebp - 0x74], 0x1d24 ; -> loc_1_1D24
        mov    word ptr [ebp - 0x70], 6
        mov    word ptr [ebp - 0x6e], 6
        mov    dword ptr [ebp - 0x6c], 0x1d34 ; -> loc_1_1D34
        mov    word ptr [ebp - 0x68], 6
        mov    word ptr [ebp - 0x66], 6
        mov    dword ptr [ebp - 0x64], 0x1d54 ; -> obj1:0x1D54
        sub    si, si
        mov    word ptr [ebp - 0x60], 6
        mov    word ptr [ebp - 0x5e], 6
        mov    dword ptr [ebp - 0x5c], 0x1d64 ; -> obj1:0x1D64
        mov    word ptr [ebp - 0x58], 6
        sub    bl, bl
        mov    word ptr [ebp - 0x56], 6
        mov    dword ptr [ebp - 0x54], 0x1d74 ; -> loc_1_1D74
        mov    dword ptr [ebp - 0x28], 0x20
        mov    byte ptr [ebp - 2], bl
        mov    eax, dword ptr [0x1b38]       ; -> obj1:0x1B38
        push   eax
        lea    eax, [ebp - 0xc]
        push   0
        push   0x27e
        push   eax
        call   sub_1_19F6
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_1790
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_2_1790:
        mov    edi, dword ptr [ebp - 0xc]
        sub    eax, eax
        mov    ecx, 0x9f
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [0x1dd8]       ; -> obj1:0x1DD8
        mov    dword ptr [ecx + 0x98], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 2
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x39], 0x306
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x3d], 0x304
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x41], 0x300
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax + 0x49], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x25], 0xd0000
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x2d], 0x10000
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x5c], 5
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x5d], 5
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x168], 0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_1A74
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_1830
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1830:
        push   2
        lea    eax, [ebp - 0x88]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_1A7A
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_1870
        push   1
        lea    eax, [ebp - 0x80]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 4
        push   eax
        call   sub_1_1A86
        or     al, al
        je     loc_2_19B2
loc_2_186A:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 2
loc_2_1870:
        push   0
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_1A7A
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_1896
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x28], ecx
loc_2_1896:
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x30]
        push   eax
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_197C
        cmp    dword ptr [ebp - 0x30], 6
        jne    loc_2_18D2
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_18D2
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x2c]
        mov    ecx, edx
        add    eax, 0x56
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edx + 5]
        mov    byte ptr [eax + 5], cl
loc_2_18D2:
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]
        test   al, 3
        je     loc_2_1B71
        push   1
        lea    eax, [ebp - 0x48]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_1A7A
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_1935
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0x300
        jne    loc_2_1A61
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x39], 0x306
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x3d], 0x304
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x41], 0x300
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x4c], 0x30c
loc_2_1935:
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_1A7A
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_1AD0
        mov    eax, dword ptr [ebp - 0x18]
        mov    cl, byte ptr [eax + 4]
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x5c], cl
        mov    eax, dword ptr [ebp - 0xc]
        mov    cl, byte ptr [eax + 0x5c]
        mov    byte ptr [eax + 0x5d], cl
        mov    eax, dword ptr [ebp - 0xc]
        cmp    byte ptr [eax], 2
        jne    loc_2_1AA5
        mov    al, byte ptr [eax + 0x5c]
        cmp    al, 5
        je     loc_2_1AD0
        cmp    al, 9
        je     loc_2_1AD0
        cmp    al, 0xa
        je     loc_2_1AD0
        cmp    al, 0xb
        je     loc_2_1AD0
        cmp    al, 0xf
        je     loc_2_1AD0
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 0x20], 0xaa00000a
        jmp    loc_2_1D52
loc_2_19B2:
        push   1
        lea    eax, [ebp - 0x78]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 4
        push   eax
        call   sub_1_1A86
        or     al, al
        je     loc_2_19D3
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 1
        jmp    loc_2_1870
loc_2_19D3:
        push   1
        lea    eax, [ebp - 0x70]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 4
        push   eax
        call   sub_1_1A86
        or     al, al
        je     loc_2_19F4
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 4
        jmp    loc_2_1870
loc_2_19F4:
        push   1
        lea    eax, [ebp - 0x68]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 4
        push   eax
        call   sub_1_1A86
        or     al, al
        je     loc_2_1A15
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 0x10
        jmp    loc_2_1870
loc_2_1A15:
        push   1
        lea    eax, [ebp - 0x60]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 4
        push   eax
        call   sub_1_1A86
        or     al, al
        je     loc_2_1A30
        jmp    loc_2_186A
loc_2_1A30:
        push   1
        lea    eax, [ebp - 0x58]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 4
        push   eax
        call   sub_1_1A86
        or     al, al
        je     loc_2_1A51
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 1
        jmp    loc_2_1870
loc_2_1A51:
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 0x20], 0xaa00000a
        jmp    loc_2_1D52
loc_2_1A61:
        cmp    eax, 0x200
        jne    loc_2_1A95
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x39], 0x206
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x3d], 0x204
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x41], 0x200
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x4c], 0x20c
        jmp    loc_2_1935
loc_2_1A95:
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 0x20], 0xaa00000d
        jmp    loc_2_1D52
loc_2_1AA5:
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x5c]
        cmp    al, 2
        je     loc_2_1AD0
        cmp    al, 3
        je     loc_2_1AD0
        cmp    al, 4
        je     loc_2_1AD0
        cmp    al, 5
        je     loc_2_1AD0
        cmp    al, 7
        je     loc_2_1AD0
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 0x20], 0xaa00000a
        jmp    loc_2_1D52
        db  090h
loc_2_1AD0:
        push   1
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_1A7A
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_2_1B39
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x25], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x25]
        cmp    eax, 0xc0000
        je     loc_2_1B39
        cmp    eax, 0xc8000
        je     loc_2_1B39
        cmp    eax, 0xd0000
        je     loc_2_1B39
        cmp    eax, 0xd8000
        je     loc_2_1B39
        cmp    eax, 0xe0000
        je     loc_2_1B39
        cmp    eax, 0xe8000
        je     loc_2_1B39
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 0x20], 0xaa00000a
        jmp    loc_2_1D52
loc_2_1B39:
        mov    eax, dword ptr [ebp - 0xc]
        test   byte ptr [eax + 0x26], 0x80
        je     loc_2_1B58
        mov    dword ptr [eax + 0x2d], 0x8000
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x29], 0x8000
        jmp    loc_2_1D52
loc_2_1B58:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x2d], 0x10000
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x29], 0
        jmp    loc_2_1D52
loc_2_1B71:
        cmp    al, 4
        jne    loc_2_1BD0
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x4c], 0x20c
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x5c], 5
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x5d], 5
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x39], 0x206
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x3d], 0x204
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x2d], 0x10000
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x25], 0xd0000
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x41], 0x200
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 1
        jmp    loc_2_1D52
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1BD0:
        cmp    al, 0x10
        jne    loc_2_1D52
        lea    eax, [ebp - 0x200]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x45
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_1976
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_2_1C09
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 0x20], 0xaa00000a
        jmp    loc_2_1D52
loc_2_1C09:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x45]
        shl    eax, 0xc
        add    eax, 0xc06
        mov    dword ptr [ecx + 0x39], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x45]
        shl    eax, 0xc
        add    eax, 0xc04
        mov    dword ptr [ecx + 0x3d], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x45]
        shl    eax, 0xc
        add    eax, 0xc00
        mov    dword ptr [ecx + 0x41], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x45]
        shl    eax, 0xc
        add    eax, 0xc0c
        mov    dword ptr [ecx + 0x4c], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [ebp - 1], 0
        mov    eax, dword ptr [eax + 0x4c]
        lea    edx, [ebp - 0xfc]
        mov    dword ptr [ebp - 0x14], eax
        lea    esp, [esp]
loc_2_1C70:
        mov    al, byte ptr [edx]
        mov    cx, word ptr [edx + 1]
        inc    edx
        mov    word ptr [ebp - 6], cx
        inc    edx
        inc    edx
        test   al, 0x80
        jne    loc_2_1C85
        mov    byte ptr [ebp - 1], 1
loc_2_1C85:
        mov    cx, word ptr [ebp - 0x14]
        cmp    cx, word ptr [ebp - 6]
        jne    loc_2_1C94
        mov    cx, word ptr [edx]
        jmp    loc_2_1CB5
loc_2_1C94:
        mov    ecx, dword ptr [ebp - 0xc]
        xor    edi, edi
        mov    ecx, dword ptr [ecx + 0x45]
        mov    di, word ptr [ebp - 6]
        shl    ecx, 0xc
        sub    ecx, edi
        cmp    ecx, 0xfffff3f8
        jne    loc_2_1CF4
        mov    cl, byte ptr [edx]
        and    ecx, 0xffff00ff
loc_2_1CB5:
        inc    edx
        test   al, 0x40
        je     loc_2_1CDA
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 0x14]
        jne    loc_2_1CD0
        mov    ax, word ptr [edx]
        inc    edx
        jmp    loc_2_1CDD
        db  005h, 000h, 000h, 000h, 000h
loc_2_1CD0:
        mov    al, byte ptr [edx]
        and    eax, 0xffff00ff
        inc    edx
        jmp    loc_2_1CDD
loc_2_1CDA:
        sub    ax, ax
loc_2_1CDD:
        xor    edi, edi
        mov    di, word ptr [ebp - 6]
        cmp    edi, dword ptr [ebp - 0x14]
        jne    loc_2_1CF0
        and    si, ax
        or     si, cx
        jmp    loc_2_1CF4
loc_2_1CF0:
        and    bl, al
        or     bl, cl
loc_2_1CF4:
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_1C70
        mov    ax, si
        and    eax, 0x78
        cmp    eax, 8
        je     loc_2_1D25
        cmp    eax, 0x10
        je     loc_2_1D30
        cmp    eax, 0x20
        je     loc_2_1D39
        cmp    eax, 0x40
        je     loc_2_1D42
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 0x20], 0xaa00000a
        jmp    loc_2_1D52
loc_2_1D25:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x5c], 5
        jmp    loc_2_1D49
        db  08Bh, 0FFh
loc_2_1D30:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x5c], 9
        jmp    loc_2_1D49
loc_2_1D39:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x5c], 0xa
        jmp    loc_2_1D49
loc_2_1D42:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x5c], 0xb
loc_2_1D49:
        mov    eax, dword ptr [ebp - 0xc]
        mov    cl, byte ptr [eax + 0x5c]
        mov    byte ptr [eax + 0x5d], cl
loc_2_1D52:
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_1A80
        lea    edi, [ebp - 0xc0]
        sub    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]
        test   al, 3
        je     loc_2_1D81
        mov    dword ptr [ebp - 0xb6], 1
        jmp    loc_2_1D8F
loc_2_1D81:
        cmp    al, 0x10
        jne    loc_2_1D8F
        mov    dword ptr [ebp - 0xb6], 2
loc_2_1D8F:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0xaa], 1
        mov    al, byte ptr [eax]
        test   al, 3
        je     loc_2_1DBA
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x41]
        mov    dword ptr [ebp - 0xa6], eax
        mov    dword ptr [ebp - 0xa2], 0x10
        jmp    loc_2_1DD4
loc_2_1DBA:
        cmp    al, 0x10
        jne    loc_2_1DD4
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x41]
        mov    dword ptr [ebp - 0xa6], eax
        mov    dword ptr [ebp - 0xa2], 0x90
loc_2_1DD4:
        lea    eax, [ebp - 0xc0]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [eax + 0x98]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x9c
        push   eax
        call   sub_1_1922
        mov    dword ptr [ebp - 0x10], eax
        or     eax, eax
        je     loc_2_1E20
        push   0
        push   0x27e
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_19FC
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_2_1E20:
        cmp    byte ptr [ebp - 2], 0
        je     loc_2_1E70
        push   0
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1928
        push   0
        push   0x27e
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_19FC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
loc_2_1E70:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    byte ptr [eax], 0x10
        jne    loc_2_220C
        mov    ecx, eax
        mov    edx, dword ptr [ecx + 0x45]
        add    ecx, 0x45
        shl    dx, 0xc
        add    dx, 0xc80
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        cmp    ax, 0xa310
        je     loc_2_1EE0
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   0xaa000004
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1928
        push   0
        push   0x27e
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_19FC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  090h
loc_2_1EE0:
        mov    edx, dword ptr [ecx]
        shl    dx, 0xc
        add    dx, 0xc82
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        cmp    ax, 0x2042
        je     loc_2_1F40
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   0xaa000004
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1928
        push   0
        push   0x27e
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_19FC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_2_1F40:
        mov    edx, dword ptr [ecx]
        shl    dx, 0xc
        add    dx, 0xc84
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   al, 1
        jne    loc_2_1F99
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   0xaa00000f
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1928
        push   0
        push   0x27e
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_19FC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_1F99:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dx, word ptr [eax + 0x41]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        test   al, 0x20
        je     loc_2_1FB6
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x2d], 0x8000
        jmp    loc_2_1FD8
loc_2_1FB6:
        test   byte ptr [ebp - 8], 8
        mov    eax, dword ptr [ebp - 0xc]
        je     loc_2_1FD1
        mov    dword ptr [eax + 0x2d], 0x20000
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax + 0x49], 8
        jmp    loc_2_1FD8
loc_2_1FD1:
        mov    dword ptr [eax + 0x2d], 0x10000
loc_2_1FD8:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x2d]
        cmp    eax, 0x8000
        je     loc_2_2000
        cmp    eax, 0x10000
        je     loc_2_20E9
        cmp    eax, 0x20000
        je     loc_2_2192
        jmp    loc_2_220C
loc_2_2000:
        and    esi, 7
        cmp    esi, 4
        je     loc_2_2065
        cmp    esi, 5
        je     loc_2_2086
        cmp    esi, 6
        je     loc_2_20A7
        cmp    esi, 7
        je     loc_2_20C8
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   0xaa00000a
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1928
        push   0
        push   0x27e
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_19FC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_2065:
        xor    eax, eax
        mov    al, bl
        shl    eax, 0x18
        mov    ecx, dword ptr [ebp - 0xc]
        add    eax, 0xc8000
        mov    dword ptr [ecx + 0x25], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x29], 0x8000
        jmp    loc_2_220C
loc_2_2086:
        xor    eax, eax
        mov    al, bl
        shl    eax, 0x18
        mov    ecx, dword ptr [ebp - 0xc]
        add    eax, 0xe8000
        mov    dword ptr [ecx + 0x25], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x29], 0x8000
        jmp    loc_2_220C
loc_2_20A7:
        xor    eax, eax
        mov    al, bl
        shl    eax, 0x18
        mov    ecx, dword ptr [ebp - 0xc]
        add    eax, 0xd8000
        mov    dword ptr [ecx + 0x25], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x29], 0x8000
        jmp    loc_2_220C
loc_2_20C8:
        xor    eax, eax
        mov    al, bl
        shl    eax, 0x18
        mov    ecx, dword ptr [ebp - 0xc]
        add    eax, 0xf8000
        mov    dword ptr [ecx + 0x25], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x29], 0x8000
        jmp    loc_2_220C
loc_2_20E9:
        and    esi, 7
        je     loc_2_2147
        cmp    esi, 1
        je     loc_2_2158
        cmp    esi, 2
        je     loc_2_2169
        cmp    esi, 3
        je     loc_2_2181
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   0xaa00000a
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1928
        push   0
        push   0x27e
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_19FC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_2147:
        xor    eax, eax
        mov    al, bl
        shl    eax, 0x18
        add    eax, 0xc0000
        jmp    loc_2_21FC
loc_2_2158:
        xor    eax, eax
        mov    al, bl
        shl    eax, 0x18
        add    eax, 0xe0000
        jmp    loc_2_21FC
loc_2_2169:
        xor    ecx, ecx
        mov    cl, bl
        shl    ecx, 0x18
        mov    eax, dword ptr [ebp - 0xc]
        add    ecx, 0xd0000
        mov    dword ptr [eax + 0x25], ecx
        jmp    loc_2_2202
loc_2_2181:
        xor    eax, eax
        mov    al, bl
        shl    eax, 0x18
        add    eax, 0xf0000
        jmp    loc_2_21FC
loc_2_2192:
        and    esi, 7
        je     loc_2_21E2
        cmp    esi, 1
        je     loc_2_21F0
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   0xaa00000a
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1928
        push   0
        push   0x27e
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_19FC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_21E2:
        xor    eax, eax
        mov    al, bl
        shl    eax, 0x18
        add    eax, 0xc0000
        jmp    loc_2_21FC
loc_2_21F0:
        xor    eax, eax
        mov    al, bl
        shl    eax, 0x18
        add    eax, 0xd0000
loc_2_21FC:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x25], eax
loc_2_2202:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x29], 0
loc_2_220C:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x4b], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    cl, byte ptr [ebp - 0x28]
        mov    byte ptr [eax + 0x166], cl
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_2270
        mov    dword ptr [ebp - 0x10], eax
        or     eax, eax
        je     loc_2_225C
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x9c]
        push   eax
        call   sub_1_1928
        push   0
        push   0x27e
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_19FC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_225C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2270:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x60], 0x40
        mov    dword ptr [esi + 0x78], 6
        mov    eax, dword ptr [esi + 0x2d]
        cmp    eax, 0x20000
        je     loc_2_22F3
        cmp    eax, 0x10000
        jne    loc_2_22C7
        mov    dword ptr [esi + 0x64], 0x80
        mov    dword ptr [esi + 0x7c], 7
        mov    dword ptr [esi + 0x68], 0x100
        mov    dword ptr [esi + 0x6c], 0xa
        mov    dword ptr [esi + 0x70], 0xa
        mov    dword ptr [esi + 0x74], 4
        jmp    loc_2_231D
loc_2_22C7:
        mov    dword ptr [esi + 0x64], 0x20
        mov    dword ptr [esi + 0x7c], 5
        mov    dword ptr [esi + 0x68], 0x100
        mov    dword ptr [esi + 0x6c], 0xa
        mov    dword ptr [esi + 0x70], 0xa
        mov    dword ptr [esi + 0x74], 2
        jmp    loc_2_231D
loc_2_22F3:
        mov    dword ptr [esi + 0x68], 0x200
        mov    dword ptr [esi + 0x6c], 0x64
        mov    dword ptr [esi + 0x70], 0x32
        mov    dword ptr [esi + 0x74], 0x14
        mov    dword ptr [esi + 0x64], 0x80
        mov    dword ptr [esi + 0x7c], 7
loc_2_231D:
        push   esi
        call   sub_2_2D70
        or     al, al
        je     loc_2_26C7
        lea    edi, [esi + 0x56]
        cmp    byte ptr [edi], 0
        jne    loc_2_2374
        cmp    byte ptr [esi + 0x57], 0
        jne    loc_2_2374
        cmp    byte ptr [esi + 0x58], 0
        jne    loc_2_2374
        cmp    byte ptr [esi + 0x59], 0
        jne    loc_2_2374
        cmp    byte ptr [esi + 0x5a], 0
        jne    loc_2_2374
        cmp    byte ptr [esi + 0x5b], 0
        jne    loc_2_2374
        mov    al, byte ptr [esi + 0x50]
        mov    byte ptr [edi], al
        mov    al, byte ptr [esi + 0x51]
        mov    byte ptr [esi + 0x57], al
        mov    al, byte ptr [esi + 0x52]
        mov    byte ptr [esi + 0x58], al
        mov    al, byte ptr [esi + 0x53]
        mov    byte ptr [esi + 0x59], al
        mov    al, byte ptr [esi + 0x54]
        mov    byte ptr [esi + 0x5a], al
        mov    al, byte ptr [esi + 0x55]
        mov    byte ptr [esi + 0x5b], al
loc_2_2374:
        mov    eax, dword ptr [esi + 0x2d]
        push   eax
        mov    eax, dword ptr [esi + 0x25]
        push   eax
        lea    ebx, [esi + 0x21]
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_1AC2
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_23B8
        push   0
        push   0xaa000001
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_23B8:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 8], eax
        and    dword ptr [ebp - 8], 0xffff0000
        or     dword ptr [ebp - 8], 0xbffe
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        mov    word ptr [esi + 0x17c], ax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0x17e], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 0x2d]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 0x182], eax
        mov    eax, dword ptr [ebx]
        add    ecx, eax
        mov    dword ptr [esi + 0x31], eax
        mov    dword ptr [esi + 0x35], ecx
        push   esi
        call   sub_2_2780
        or     al, al
        jne    loc_2_2430
        push   0
        push   0xaa000002
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_2430:
        lea    eax, [esi + 0x84]
        mov    dword ptr [esi + 0x88], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 0x8c]
        mov    dword ptr [esi + 0x90], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x19], 0xf0  ; -> loc_1_00F0
        mov    dword ptr [esi + 0x1d], 0xf0  ; -> loc_1_00F0
        mov    eax, dword ptr [esi + 0xc9]
        mov    dword ptr [esi + 0xb9], eax
        mov    dword ptr [esi + 0xbd], eax
        mov    dword ptr [esi + 0xc1], eax
        mov    eax, dword ptr [esi + 0x60]
        mov    dword ptr [esi + 0xb5], eax
        shl    eax, 3
        add    eax, dword ptr [esi + 0xc9]
        sub    eax, 8
        mov    dword ptr [esi + 0xcd], eax
        mov    dword ptr [esi + 0xd5], 0
        mov    dword ptr [esi + 0xd9], 0
        mov    dword ptr [esi + 0xdd], 0
        mov    dword ptr [esi + 0xe1], 0
        mov    byte ptr [esi + 0xf9], 1
        mov    byte ptr [esi + 0xfa], 0
        mov    dword ptr [esi + 0xfc], 0
        mov    dword ptr [esi + 0x100], 0
        mov    dword ptr [esi + 0x108], 0
        mov    dword ptr [esi + 0x10c], 0
        mov    dword ptr [esi + 0x110], 0
        mov    dword ptr [esi + 0x114], 0
        mov    dword ptr [esi + 0x118], 0
        mov    dword ptr [esi + 0x11c], 0
        mov    dword ptr [esi + 0x120], 0
        mov    dword ptr [esi + 0x124], 0
        mov    dword ptr [esi + 0x128], 0
        mov    dword ptr [esi + 0x12c], 0
        mov    dword ptr [esi + 0x130], 0
        mov    byte ptr [esi + 0x148], 0
        mov    byte ptr [esi + 0x149], 0
        mov    dword ptr [esi + 0x14a], 0
        lea    ebx, [esi + 1]
        mov    byte ptr [esi + 0x167], 1
        mov    byte ptr [esi + 0x169], 0
        mov    dword ptr [esi + 0x156], 0
        mov    dword ptr [esi + 0x15a], 0
        push   esi
        call   sub_2_1540
        push   1
        push   0
        xor    eax, eax
        mov    al, byte ptr [esi + 0x5d]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 0x5c]
        push   eax
        mov    eax, dword ptr [esi + 0x19]
        push   eax
        push   esi
        push   0x20                          ; -> loc_1_0020
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_19DE
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_25E1
        push   0
        push   0xaa000005
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        push   esi
        call   sub_2_0140
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_25E1:
        lea    eax, [esi + 0xad]
        push   eax
        lea    eax, [esi + 0x94]
        push   eax
        push   edi
        push   0x1140                        ; -> loc_2_1140
        push   0x10b0                        ; -> loc_2_10B0
        xor    eax, eax
        push   0x1020                        ; -> loc_2_1020
        mov    al, byte ptr [esi + 0x166]
        push   eax
        call   sub_2_2FF0
        or     al, al
        jne    loc_2_2641
        push   0
        push   0xaa000002
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        push   esi
        call   sub_2_0140
        push   ebx
        call   sub_1_19E4
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2641:
        push   esi
        call   sub_2_26F0
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     loc_2_2688
        push   0
        push   0xaa000003
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0xad]
        push   eax
        call   sub_2_3110
        push   esi
        call   sub_2_0140
        push   ebx
        call   sub_1_19E4
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2688:
        lea    eax, [esi + 0x16e]
        mov    dword ptr [esi + 0x16a], 0xb00 ; -> loc_1_0B00
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esi + 0x16a]
        mov    dword ptr [esi + 0x172], ecx
        mov    dword ptr [esi + 0x176], esi
        push   0x1388
        push   eax
        call   sub_1_19D2
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_26C7:
        push   0
        push   0xaa000004
        mov    eax, dword ptr [esi + 0x9c]
        push   eax
        call   sub_1_1AC8
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_26F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 0x13
        je     loc_2_2724
        mov    byte ptr [esi + 0x4b], 0
        mov    dword ptr [ebp - 0xc], esi
        mov    ax, word ptr [esi + 0x49]
        or     ax, 2
        mov    word ptr [ebp - 4], ax
        lea    eax, [ebp - 0xc]
        push   eax
        push   0xae0                         ; -> loc_1_0AE0
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_19EA
loc_2_2724:
        push   esi
        call   sub_2_13A0
        push   0x7a120
        call   sub_1_19F0
        cmp    byte ptr [esi + 0x167], 0
        jne    loc_2_2765
        mov    dword ptr [esi + 0xa0], 0
        mov    eax, dword ptr [esi + 0x1d]
        mov    dword ptr [esi + 0xa4], eax
        mov    dword ptr [esi + 0xa8], esi
        push   esi
        call   sub_2_00F0
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2765:
        mov    eax, 0xc0000001
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2780:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x6c]
        mov    dword ptr [ebp - 0xc], edi
        mov    eax, dword ptr [esi + 0x70]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [esi + 0x74]
        mov    ecx, dword ptr [esi + 0x31]
        mov    dword ptr [ebp - 0x18], eax
        lea    edx, [ecx + 0x18]
        mov    eax, dword ptr [esi + 0x35]
        inc    eax
        cmp    eax, edx
        lea    eax, [esi + 0xb1]
        jae    loc_2_27BA
        mov    dword ptr [eax], 0
        jmp    loc_2_27BF
loc_2_27BA:
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0x31], edx
loc_2_27BF:
        cmp    dword ptr [eax], 0
        jne    loc_2_27D5
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_27D5:
        mov    eax, dword ptr [esi + 0x60]
        shl    eax, 3
        add    eax, 7
        and    eax, 0xfffffff8
        jne    loc_2_27F1
        lea    eax, [esi + 0xc9]
        mov    dword ptr [eax], 0
        jmp    loc_2_2815
loc_2_27F1:
        mov    edx, dword ptr [esi + 0x31]
        lea    ecx, [eax + edx]
        mov    eax, dword ptr [esi + 0x35]
        inc    eax
        cmp    eax, ecx
        lea    eax, [esi + 0xc9]
        jae    loc_2_2810
        mov    dword ptr [eax], 0
        jmp    loc_2_2815
        db  08Dh, 049h, 000h
loc_2_2810:
        mov    dword ptr [eax], edx
        mov    dword ptr [esi + 0x31], ecx
loc_2_2815:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_2_2830
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_2830:
        sub    ecx, ecx
        cmp    dword ptr [esi + 0x60], ecx
        jbe    loc_2_2847
        sub    edx, edx
loc_2_2839:
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], edx
        inc    ecx
        add    eax, 8
        cmp    dword ptr [esi + 0x60], ecx
        ja     loc_2_2839
loc_2_2847:
        mov    eax, dword ptr [esi + 0x64]
        shl    eax, 3
        add    eax, 7
        and    eax, 0xfffffff8
        jne    loc_2_2863
        lea    ebx, [esi + 0xd1]
        mov    dword ptr [ebx], 0
        jmp    loc_2_2885
loc_2_2863:
        mov    edx, dword ptr [esi + 0x31]
        lea    ecx, [eax + edx]
        mov    eax, dword ptr [esi + 0x35]
        inc    eax
        cmp    eax, ecx
        lea    ebx, [esi + 0xd1]
        jae    loc_2_2880
        mov    dword ptr [ebx], 0
        jmp    loc_2_2885
        db  090h
loc_2_2880:
        mov    dword ptr [ebx], edx
        mov    dword ptr [esi + 0x31], ecx
loc_2_2885:
        cmp    dword ptr [ebx], 0
        jne    loc_2_28A0
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_2_28A0:
        mov    eax, dword ptr [0x1b38]       ; -> obj1:0x1B38
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x64]
        shl    eax, 2
        push   eax
        lea    eax, [esi + 0x104]
        push   eax
        call   sub_1_19F6
        mov    edi, dword ptr [esi + 0x104]
        or     edi, edi
        jne    loc_2_28D6
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_28D6:
        mov    eax, dword ptr [esi + 0x64]
        lea    ecx, [eax*4]
        sub    eax, eax
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebx]
        cmp    dword ptr [esi + 0x64], eax
        jbe    loc_2_297D
        sub    edx, edx
loc_2_28F7:
        mov    eax, dword ptr [esi + 0x68]
        add    eax, 7
        and    eax, 0xfffffff8
        jne    loc_2_2911
        mov    eax, dword ptr [esi + 0x104]
        mov    dword ptr [edx + eax], 0
        jmp    loc_2_293C
loc_2_2911:
        mov    edi, dword ptr [esi + 0x31]
        add    eax, edi
        mov    ebx, dword ptr [esi + 0x35]
        inc    ebx
        cmp    ebx, eax
        jae    loc_2_2930
        mov    eax, dword ptr [esi + 0x104]
        mov    dword ptr [edx + eax], 0
        jmp    loc_2_293C
        db  08Dh, 049h, 000h
loc_2_2930:
        mov    ebx, dword ptr [esi + 0x104]
        mov    dword ptr [edx + ebx], edi
        mov    dword ptr [esi + 0x31], eax
loc_2_293C:
        mov    eax, dword ptr [esi + 0x104]
        mov    eax, dword ptr [edx + eax]
        or     eax, eax
        je     loc_2_29B2
        sub    eax, dword ptr [esi + 0x21]
        add    eax, dword ptr [esi + 0x29]
        mov    word ptr [ecx], ax
        shr    eax, 0x10
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [esi + 0x68]
        inc    dword ptr [ebp - 4]
        not    ax
        inc    ax
        mov    word ptr [ecx + 4], ax
        mov    byte ptr [ecx + 3], 0x80
        add    edx, 4
        add    ecx, 8
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x64], eax
        ja     loc_2_28F7
loc_2_297D:
        lea    edi, [esi + 0xc5]
        mov    eax, dword ptr [0x1b38]       ; -> obj1:0x1B38
        push   eax
        mov    eax, dword ptr [esi + 0x60]
        push   0
        lea    eax, [eax + eax*2]
        shl    eax, 2
        push   eax
        push   edi
        call   sub_1_19F6
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_2_29C3
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_29B2:
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_29C3:
        mov    edx, dword ptr [esi + 0x60]
        sub    eax, eax
        lea    edx, [edx + edx*2]
        shl    edx, 2
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, dword ptr [ebp - 0x14]
        add    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0x1b38]       ; -> obj1:0x1B38
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 4
        push   eax
        lea    eax, [esi + 0xf5]
        push   eax
        call   sub_1_19F6
        mov    edi, dword ptr [esi + 0xf5]
        or     edi, edi
        jne    loc_2_2A20
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_2A20:
        mov    ecx, dword ptr [ebp - 8]
        shl    ecx, 4
        sub    eax, eax
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esi + 0xe5], 0xffffffff
        mov    dword ptr [esi + 0xe9], eax
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0xc], eax
        je     loc_2_2A96
        sub    edx, edx
loc_2_2A46:
        mov    edi, dword ptr [esi + 0x31]
        mov    ebx, dword ptr [esi + 0x35]
        lea    eax, [edi + 0x40]
        inc    ebx
        cmp    ebx, eax
        jae    loc_2_2A63
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + eax], 0
        jmp    loc_2_2A6F
loc_2_2A63:
        mov    ebx, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + ebx], edi
        mov    dword ptr [esi + 0x31], eax
loc_2_2A6F:
        mov    eax, dword ptr [esi + 0xf5]
        add    eax, edx
        cmp    dword ptr [eax], 0
        je     loc_2_2AE0
        inc    ecx
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + eax + 8], 0x40
        add    edx, 0x10
        cmp    dword ptr [ebp - 0xc], ecx
        ja     loc_2_2A46
loc_2_2A96:
        mov    edx, ecx
        shl    edx, 4
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + eax - 0xc], 0xffffffff
        mov    dword ptr [esi + 0xed], ecx
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, dword ptr [ebp - 0xc]
        cmp    ecx, eax
        mov    dword ptr [ebp - 0x10], eax
        jae    loc_2_2B24
loc_2_2ABC:
        mov    edi, dword ptr [esi + 0x31]
        mov    ebx, dword ptr [esi + 0x35]
        lea    eax, [edi + 0x100]
        inc    ebx
        cmp    ebx, eax
        jae    loc_2_2AF1
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + eax], 0
        jmp    loc_2_2AFD
        db  08Dh, 064h, 024h, 000h
loc_2_2AE0:
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2AF1:
        mov    ebx, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + ebx], edi
        mov    dword ptr [esi + 0x31], eax
loc_2_2AFD:
        mov    eax, dword ptr [esi + 0xf5]
        add    eax, edx
        cmp    dword ptr [eax], 0
        je     loc_2_2B62
        inc    ecx
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + eax + 8], 0x100
        add    edx, 0x10
        cmp    ecx, dword ptr [ebp - 0x10]
        jb     loc_2_2ABC
loc_2_2B24:
        mov    edx, ecx
        shl    edx, 4
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + eax - 0xc], 0xffffffff
        mov    dword ptr [esi + 0xf1], ecx
        cmp    ecx, dword ptr [ebp - 8]
        jae    loc_2_2BA6
loc_2_2B42:
        mov    eax, dword ptr [esi + 0x31]
        mov    ebx, dword ptr [esi + 0x35]
        lea    edi, [eax + 0x5f0]
        inc    ebx
        cmp    ebx, edi
        jae    loc_2_2B73
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + eax], 0
        jmp    loc_2_2B7F
loc_2_2B62:
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2B73:
        mov    ebx, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + ebx], eax
        mov    dword ptr [esi + 0x31], edi
loc_2_2B7F:
        mov    eax, dword ptr [esi + 0xf5]
        add    eax, edx
        cmp    dword ptr [eax], 0
        je     loc_2_2BC2
        inc    ecx
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [edx + eax + 8], 0x5ea
        add    edx, 0x10
        cmp    ecx, dword ptr [ebp - 8]
        jb     loc_2_2B42
loc_2_2BA6:
        shl    ecx, 4
        mov    eax, dword ptr [esi + 0xf5]
        mov    dword ptr [ecx + eax - 0xc], 0xffffffff
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2BC2:
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        sub    esi, esi
        mov    edx, dword ptr [ebp + 0x18]
        cmp    edx, esi
        mov    ecx, dword ptr [ebp + 0x14]
        je     loc_2_2C06
        lea    eax, [ecx + edx*4 - 4]
loc_2_2BF8:
        cmp    dword ptr [eax], 0
        je     loc_2_2C05
        sub    eax, 4
        dec    edx
        jne    loc_2_2BF8
        jmp    loc_2_2C06
loc_2_2C05:
        dec    edx
loc_2_2C06:
        cmp    dword ptr [ecx + edx*4], 0
        je     loc_2_2C19
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_2C19:
        mov    eax, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edx
        inc    dword ptr [edi + 0x80]
        mov    eax, dword ptr [0x1b38]       ; -> obj1:0x1B38
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   0x39
        push   eax
        call   sub_1_19F6
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_2D03
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
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x35], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0xea
        mov    dword ptr [edi + 0x162], 0xea
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0xad]
        push   eax
        call   sub_2_3160
        or     al, al
        jne    loc_2_2CD0
        push   0
        push   0x39
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_19FC
        mov    esi, 0xc0000001
        jmp    loc_2_2D20
loc_2_2CD0:
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [edi + 0x84]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 0x88]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edi + 0x88]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0x88], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x18]
        jmp    loc_2_2D20
loc_2_2D03:
        push   0
        push   1
        push   0xaa000002
        mov    eax, dword ptr [edi + 0x9c]
        mov    esi, 0xc000009a
        push   eax
        call   sub_1_1AC8
        add    esp, 0x10
loc_2_2D20:
        mov    eax, dword ptr [edi + 0x80]
        dec    eax
        mov    dword ptr [edi + 0x80], eax
        jne    loc_2_2D5E
        cmp    byte ptr [edi + 0x148], 0
        jne    loc_2_2D50
        cmp    dword ptr [edi + 0xd5], 0
        jne    loc_2_2D50
        mov    eax, dword ptr [edi + 0x8c]
        sub    eax, edi
        cmp    eax, 0x8c
        je     loc_2_2D5E
loc_2_2D50:
        push   0
        add    edi, 0xa0
        push   edi
        call   sub_1_19D2
loc_2_2D5E:
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0x10], 0xff
        mov    byte ptr [ebp - 0xf], 0
        sub    esi, esi
        mov    byte ptr [ebp - 0xe], 0x55
        mov    byte ptr [ebp - 0xd], 0xaa
        mov    byte ptr [ebp - 0xc], 0xff
        mov    byte ptr [ebp - 0xb], 0
        mov    byte ptr [ebp - 0xa], 0x55
        mov    byte ptr [ebp - 9], 0xaa
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, edi
loc_2_2DA0:
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp + esi - 8], al
        inc    esi
        cmp    esi, 8
        jb     loc_2_2DA0
        mov    esi, 8
loc_2_2DB4:
        sub    eax, eax
loc_2_2DB6:
        mov    dl, byte ptr [ebp + eax - 0x10]
        cmp    byte ptr [ebp + eax - 8], dl
        jne    loc_2_2DC8
        inc    eax
        cmp    eax, 8
        jb     loc_2_2DB6
        jmp    loc_2_2DCD
loc_2_2DC8:
        mov    eax, 0xa
loc_2_2DCD:
        cmp    eax, 0xa
        jne    loc_2_2DF0
        sub    eax, eax
loc_2_2DD4:
        mov    dl, byte ptr [ebp + eax - 7]
        mov    byte ptr [ebp + eax - 8], dl
        inc    eax
        cmp    eax, 7
        jb     loc_2_2DD4
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        inc    esi
        cmp    esi, 0x28
        jb     loc_2_2DB4
loc_2_2DF0:
        cmp    esi, 0x28
        jne    loc_2_2E00
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2E00:
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ecx + 0x50], al
        in     al, dx
        mov    byte ptr [ecx + 0x51], al
        in     al, dx
        mov    byte ptr [ecx + 0x52], al
        in     al, dx
        mov    byte ptr [ecx + 0x53], al
        in     al, dx
        mov    byte ptr [ecx + 0x54], al
        in     al, dx
        mov    byte ptr [ecx + 0x55], al
        mov    al, byte ptr [ecx]
        test   al, 0x12
        jne    loc_2_2E41
        cmp    al, 4
        jne    loc_2_2E36
        mov    dx, 0x800
        sub    al, al
        out    dx, al
        mov    dx, word ptr [ecx + 0x41]
        sub    ax, ax
        out    dx, ax
loc_2_2E36:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2E41:
        sub    eax, eax
        sub    edx, edx
loc_2_2E45:
        cmp    byte ptr [ecx + eax + 0x50], dl
        jne    loc_2_2E53
        inc    eax
        cmp    eax, 6
        jb     loc_2_2E45
        jmp    loc_2_2E58
loc_2_2E53:
        mov    eax, 0xa
loc_2_2E58:
        cmp    eax, 0xa
        je     loc_2_2E68
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2E68:
        test   byte ptr [ecx + 0x50], 1
        je     loc_2_2E79
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2E79:
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    al, byte ptr [ebp - 8]
        cmp    byte ptr [ebp - 1], al
        jne    loc_2_2FD6
        mov    al, byte ptr [ebp - 7]
        cmp    byte ptr [ebp - 2], al
        jne    loc_2_2FD6
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        sub    eax, eax
loc_2_2EE3:
        mov    dl, byte ptr [ebp + eax - 8]
        cmp    byte ptr [ecx + eax + 0x50], dl
        jne    loc_2_2FA9
        inc    eax
        cmp    eax, 6
        jb     loc_2_2EE3
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        sub    eax, eax
        lea    esp, [esp]
loc_2_2F30:
        mov    dl, byte ptr [ebp + eax - 8]
        cmp    byte ptr [ecx + eax + 0x50], dl
        jne    loc_2_2FB4
        inc    eax
        cmp    eax, 6
        jb     loc_2_2F30
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    al, byte ptr [ebp - 8]
        cmp    byte ptr [ebp - 2], al
        jne    loc_2_2FCB
        mov    al, byte ptr [ebp - 7]
        cmp    byte ptr [ebp - 1], al
        jne    loc_2_2FCB
        sub    edi, edi
loc_2_2F62:
        lea    esi, [ebp + edi - 8]
        mov    dx, word ptr [ecx + 0x4c]
        in     al, dx
        mov    byte ptr [esi], al
        cmp    byte ptr [ebp + edi - 0x10], al
        jne    loc_2_2FC0
        inc    edi
        cmp    edi, 8
        jb     loc_2_2F62
        mov    bl, byte ptr [ecx]
        cmp    bl, 4
        jne    loc_2_2F90
        mov    dx, 0x800
        sub    al, al
        out    dx, al
        mov    dx, word ptr [ecx + 0x41]
        sub    ax, ax
        out    dx, ax
loc_2_2F90:
        test   bl, 0x12
        je     loc_2_2F9E
        mov    dx, word ptr [ecx + 0x41]
        sub    ax, ax
        out    dx, ax
loc_2_2F9E:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2FA9:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2FB4:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_2FC0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2FCB:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2FD6:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2FF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x1dd0]       ; -> obj1:0x1DD0
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_19F6
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_3024
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_3024:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_303E
        mov    esi, 2
loc_2_303E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_19F6
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_3072
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_3110
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_3072:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_19F6
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_30A1
        push   eax
        call   sub_2_3110
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_30A1:
        mov    dword ptr [eax + 0x28], 0xffffffff
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 0x2c], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 0x30], dl
        add    eax, 0x2c
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x14], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x18], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x20], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_3110:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_3130
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_19FC
loc_2_3130:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_3146
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_19FC
loc_2_3146:
        push   0
        push   0x32
        push   esi
        call   sub_1_19FC
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_3160:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_3179
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_3179:
        mov    eax, dword ptr [0x1dd0]       ; -> obj1:0x1DD0
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_19F6
        or     eax, eax
        je     loc_2_3199
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_3199:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_31A0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_31B3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_31A0
        jmp    loc_2_31C2
loc_2_31B3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_31C2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_31D8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_31D8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 0x19], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_2_3230:
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
        call   sub_2_35D0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_3259
        cmp    ebx, 0x103
        jne    loc_2_3270
loc_2_3259:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_32F0
        or     eax, eax
        je     loc_2_3270
        mov    ebx, eax
loc_2_3270:
        or     ebx, ebx
        je     loc_2_327C
        cmp    ebx, 0x103
        jne    loc_2_32DB
loc_2_327C:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_32D6
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_3292
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_3292:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_32A1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_32A7
loc_2_32A1:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_32A7:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_32B6
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1958
loc_2_32B6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_19FC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_32D6:
        mov    ebx, 0xc001000e
loc_2_32DB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_32F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x20]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x20], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [esi + 0x24]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_1AFC
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_1AFC
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_3374
        sub    eax, eax
        nop    
loc_2_3350:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 0x1c]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 0x24], edi
        ja     loc_2_3350
loc_2_3374:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_33DF
loc_2_3381:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_33D4
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        call   sub_1_1AFC
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        call   sub_1_1AFC
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_33D7
loc_2_33D4:
        inc    dword ptr [ebp - 4]
loc_2_33D7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_3381
loc_2_33DF:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_34CA
        mov    edi, dword ptr [ebp + 0x18]
loc_2_33F3:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_37B0
        or     al, al
        je     loc_2_342A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_34B8
loc_2_342A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_3504
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        call   sub_1_1AFC
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0x24]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_1AFC
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x14], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 0x19]
        mov    edx, dword ptr [ebp - 0x14]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 0x24]
loc_2_34B8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [ebp + 0x14]
        jb     loc_2_33F3
loc_2_34CA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_3555
        mov    edx, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_34E0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_355B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    loc_2_3542
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    loc_2_3542
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_3549
loc_2_3504:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1AFC
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_1AFC
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_3542:
        mov    dword ptr [ebp - 8], 1
loc_2_3549:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0xc]
        jb     loc_2_34E0
loc_2_3555:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_35C0
loc_2_355B:
        mov    al, byte ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   dword ptr [esi + 0x14]
        mov    edi, eax
        or     edi, edi
        je     loc_2_35C2
        cmp    edi, 0x103
        je     loc_2_35C2
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1AFC
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1AFC
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_35C0:
        sub    edi, edi
loc_2_35C2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_35D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        mov    dword ptr [esi + 0xc], 0
        je     loc_2_360D
        lea    esp, [esp]
loc_2_3600:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_3600
loc_2_360D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ebx, ecx
        je     loc_2_3641
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 0x18]
        or     eax, eax
        je     loc_2_3643
        cmp    eax, 0x103
        je     loc_2_3643
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_3641:
        sub    eax, eax
loc_2_3643:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_3650:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 0x19]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0xff
        mov    edi, dword ptr [edx + 0x24]
        cmp    edi, eax
        je     loc_2_3682
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_3677:
        test   dword ptr [esi], edx
        je     loc_2_367C
        inc    eax
loc_2_367C:
        add    esi, 4
        dec    edi
        jne    loc_2_3677
loc_2_3682:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_3690:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x19]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 0x24], 0
        jbe    loc_2_370C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_36D0:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_36FA
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_3726
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_1AFC
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_36FA:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_36D0
loc_2_370C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_3726:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x18]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_3750:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_3780
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_3780:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 0x24]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_1AFC
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_37B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     loc_2_3849
        dec    esi
        cmp    esi, edx
        jb     loc_2_3849
        nop    
loc_2_37D0:
        cmp    edx, ecx
        jb     loc_2_3849
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_37F3
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_3827
loc_2_37F3:
        jbe    loc_2_3800
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_3827
        db  08Bh, 0FFh
loc_2_3800:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_3817
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_3827
loc_2_3817:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_3827
        mov    dword ptr [ebp - 4], 0
loc_2_3827:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_3859
        jle    loc_2_3838
        or     edx, edx
        je     loc_2_3849
        lea    esi, [edx - 1]
        jmp    loc_2_383B
loc_2_3838:
        lea    ecx, [edx + 1]
loc_2_383B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_37D0
loc_2_3849:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_3859:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS

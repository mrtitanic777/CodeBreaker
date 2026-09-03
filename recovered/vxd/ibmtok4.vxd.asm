; IBMTOK4 -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 6475 instructions recovered by recursive descent

Declare_Virtual_Device IBMTOK4, 3, 10, IBMTOK4_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x41e8  R-X
sub_1_0000:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   edi
        xor    ebx, ebx
        mov    dword ptr [edx + 0xe8], ebx
        mov    dword ptr [edx + 0xec], ebx
        lea    edi, [edx + 0x179]
        xor    eax, eax
        mov    ecx, 0x80
        mov    dword ptr [edx + 0xf0], ebx
        mov    dword ptr [edx + 0xf4], ebx
        mov    dword ptr [edx + 0xcf], ebx
        rep stosd dword ptr es:[edi], eax
        mov    ecx, 1
        mov    dword ptr [edx + 0x8a], ebx
        mov    dword ptr [edx + 0x8e], ebx
        mov    byte ptr [edx + 0xcc], cl
        mov    eax, 0xffff
        mov    byte ptr [edx + 0xcd], cl
        mov    word ptr [edx + 0xd3], ax
        mov    byte ptr [edx + 0x49], bl
        mov    word ptr [edx + 0xd5], ax
        pop    edi
        mov    byte ptr [edx + 0x4b], bl
        mov    byte ptr [edx + 0xce], bl
        pop    ebx
        mov    byte ptr [edx + 0xdb], cl
        pop    ebp
        ret    4
loc_1_0080:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    dword ptr [ebp - 8], edx
        push   edi
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebp + 0x18]
        cmp    edx, eax
        je     loc_1_00AE
        mov    ecx, dword ptr [ebp + 0x14]
loc_1_009D:
        cmp    dword ptr [ecx], 1
        je     loc_1_00AA
        add    ecx, 4
        inc    edx
        cmp    edx, eax
        jb     loc_1_009D
loc_1_00AA:
        cmp    edx, eax
        jne    loc_1_00BC
loc_1_00AE:
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_00BC:
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0x20]
        mov    ecx, dword ptr [0]            ; -> obj4:0x0000
        xor    ebx, ebx
        push   ecx
        mov    dword ptr [eax], edx
        push   ebx
        lea    eax, [ebp - 4]
        push   0x96
        inc    dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_3F46
        test   eax, eax
        jne    loc_1_028D
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x10], ecx
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 0x14], ebx
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 0x1d], bl
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 8], esi
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 0x1c], bl
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 0x86]
        push   edx
        call   sub_2_0F10
loc_1_0136:
        test   al, al
        jne    loc_1_016F
        push   4
        mov    eax, dword ptr [esi + 0x82]
        push   1
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_4018
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        push   0x96
        push   eax
        call   sub_1_3F4C
        mov    dword ptr [ebp - 8], 0xc0000001
        jmp    loc_1_025F
loc_1_016F:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 0xf0
        lea    edx, [esi + 0x62]
        mov    dword ptr [ecx + 0x18], eax
        mov    dword ptr [esi + 0x165], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x66]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0x66]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [eax], edx
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x66], ecx
        mov    ecx, dword ptr [ebp - 4]
        inc    dword ptr [ecx + 0x14]
        cmp    byte ptr [esi + 0x170], dl
        je     loc_1_025F
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], 0x103
        mov    byte ptr [eax + 0x1c], 1
        cmp    byte ptr [esi + 0x171], dl
        jne    loc_1_025F
        mov    byte ptr [esi + 0x171], 1
        xor    ebx, ebx
        xor    eax, eax
        mov    dword ptr [esi + 0x15c], 3
        mov    ecx, 0xf
        mov    byte ptr [esi + 0x37], dl
        mov    bx, word ptr [esi + 0xb6]
        add    ebx, dword ptr [esi + 0xb2]
        mov    edi, ebx
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [ebx], 3
        cmp    byte ptr [esi + 0x2b], 1
        sbb    eax, eax
        and    ax, 0x1000
        mov    al, 1
        mov    word ptr [ebx + 8], ax
loc_1_020E:
        mov    al, byte ptr [esi + edx + 0x4e]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [ebx + edx + 9], al
        jb     loc_1_020E
        mov    ax, word ptr [esi + 0xe4]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x18], cx
        mov    ax, word ptr [esi + 0xe6]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x1a], cx
        mov    ax, word ptr [esi + 0xde]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x1c], cx
        mov    al, byte ptr [esi + 0xdc]
        mov    byte ptr [ebx + 0x1e], al
        mov    edx, dword ptr [esi + 0xae]
        mov    byte ptr [edx + 0x1e4b], 0x20
loc_1_025F:
        mov    eax, dword ptr [esi + 0x5e]
        dec    eax
        mov    dword ptr [esi + 0x5e], eax
        jne    loc_1_0281
        cmp    byte ptr [esi + 0x160], 0
        jne    loc_1_027B
        mov    eax, dword ptr [esi + 0x6a]
        sub    eax, esi
        cmp    eax, 0x6a
        je     loc_1_0281
loc_1_027B:
        push   esi
        call   sub_1_35C8
loc_1_0281:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_028D:
        push   5
        mov    eax, dword ptr [esi + 0x82]
        push   1
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_4018
        add    esp, 0x14
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_02C0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        mov    esi, dword ptr [ecx + 0x62]
        sub    eax, esi
        cmp    eax, -0x62
        je     loc_1_02E9
loc_1_02D5:
        mov    eax, dword ptr [esi + 0x18]
        cmp    eax, edx
        jbe    loc_1_02DE
        mov    edx, eax
loc_1_02DE:
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, -0x62
        jne    loc_1_02D5
loc_1_02E9:
        test   edx, edx
        jne    loc_1_02F2
        mov    edx, 0xf0
loc_1_02F2:
        pop    esi
        mov    dword ptr [ecx + 0x165], edx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0300:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        inc    dword ptr [esi + 0x5e]
        cmp    byte ptr [ebx + 0x1d], 0
        jne    loc_1_0488
        push   0
        inc    dword ptr [ebx + 0x14]
        mov    eax, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [esi + 0x86]
        push   eax
        push   ecx
        call   sub_2_0FF0
loc_1_032E:
        mov    edi, eax
        test   edi, edi
        jne    loc_1_03AF
        cmp    dword ptr [ebx + 0x14], 2
        jne    loc_1_037B
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], edx
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [esi + 0x165]
        cmp    dword ptr [ebx + 0x18], ecx
        jne    loc_1_0369
        push   esi
        call   sub_1_02C0
loc_1_0369:
        push   0
        push   0x96
        push   ebx
        call   sub_1_3F4C
        jmp    loc_1_048D
loc_1_037B:
        mov    byte ptr [ebx + 0x1d], 1
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 0x6a]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edi, 0x103
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 0x6e], ebx
        sub    dword ptr [ebx + 0x14], 2
        jmp    loc_1_048D
loc_1_03AF:
        cmp    edi, 0x103
        jne    loc_1_03FC
        cmp    byte ptr [esi + 0xcc], 0
        je     loc_1_03CD
        mov    byte ptr [esi + 0xcc], 0
        push   esi
        call   sub_1_2DD8
loc_1_03CD:
        mov    byte ptr [ebx + 0x1d], 1
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 0x6a]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 0x6e], ebx
        sub    dword ptr [ebx + 0x14], 2
        jmp    loc_1_048D
loc_1_03FC:
        cmp    edi, 0xc001000e
        jne    loc_1_0434
        mov    byte ptr [ebx + 0x1d], 1
        mov    ecx, dword ptr [ebx + 4]
        mov    edi, 0x103
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 0x6a]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 0x6e], ebx
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_048D
loc_1_0434:
        cmp    edi, 0xc001000d
        jne    loc_1_046C
        mov    byte ptr [ebx + 0x1d], 1
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 0x72]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edi, 0x103
        mov    edx, dword ptr [esi + 0x76]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 0x76]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 0x76], ebx
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_048D
loc_1_046C:
        push   1
        mov    eax, dword ptr [esi + 0x82]
        push   0xa
        push   2
        push   0xaa000006
        push   eax
        call   sub_1_4018
        add    esp, 0x14
        jmp    loc_1_048D
loc_1_0488:
        mov    edi, 0xc0010002
loc_1_048D:
        mov    eax, dword ptr [esi + 0x5e]
        dec    eax
        mov    dword ptr [esi + 0x5e], eax
        jne    loc_1_04AF
        cmp    byte ptr [esi + 0x160], 0
        jne    loc_1_04A9
        mov    eax, dword ptr [esi + 0x6a]
        sub    eax, esi
        cmp    eax, 0x6a
        je     loc_1_04AF
loc_1_04A9:
        push   esi
        call   sub_1_35C8
loc_1_04AF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_04C0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 8]
        xor    edi, edi
        mov    edx, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x5e]
        mov    eax, dword ptr [edx + 0x10]
        test   eax, eax
        jne    loc_1_04E7
        push   edx
        push   ecx
        push   esi
        call   sub_1_0970
        mov    edi, eax
        jmp    loc_1_055F
loc_1_04E7:
        cmp    eax, 1
        jne    loc_1_055A
        cmp    byte ptr [esi + 0x175], 0
        je     loc_1_04FC
        mov    edi, 0xc0010008
        jmp    loc_1_055F
loc_1_04FC:
        cmp    byte ptr [esi + 0x172], 0
        jne    loc_1_051E
        cmp    byte ptr [ecx + 0x1d], 0
        jne    loc_1_0517
        push   edx
        push   ecx
        push   esi
        call   sub_1_09C0
        mov    edi, eax
        jmp    loc_1_055F
loc_1_0517:
        mov    edi, 0xc0010002
        jmp    loc_1_055F
loc_1_051E:
        cmp    byte ptr [esi + 0x160], 0
        je     loc_1_052E
        mov    edi, 0xc001000d
        jmp    loc_1_055F
loc_1_052E:
        cmp    byte ptr [esi + 0x170], 0
        je     loc_1_053E
        mov    edi, 0xc0000001
        jmp    loc_1_055F
loc_1_053E:
        push   3
        mov    eax, dword ptr [esi + 0x82]
        push   0xb
        push   2
        push   0xaa000006
        push   eax
        call   sub_1_4018
        add    esp, 0x14
        jmp    loc_1_055F
loc_1_055A:
        mov    edi, 0x10001
loc_1_055F:
        mov    eax, dword ptr [esi + 0x5e]
        dec    eax
        mov    dword ptr [esi + 0x5e], eax
        jne    loc_1_0581
        cmp    byte ptr [esi + 0x160], 0
        jne    loc_1_057B
        mov    eax, dword ptr [esi + 0x6a]
        sub    eax, esi
        cmp    eax, 0x6a
        je     loc_1_0581
loc_1_057B:
        push   esi
        call   sub_1_35C8
loc_1_0581:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0590:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        lea    ecx, [ebp - 8]
        push   esi
        mov    edx, dword ptr [ebp + 0x10]
        push   edi
        mov    esi, 4
        xor    edi, edi
        mov    eax, edx
        mov    dword ptr [ebp - 0xc], edi
        cmp    eax, 0x2010101
        mov    dword ptr [ebp - 0x10], 1
        ja     loc_1_05D0
        je     loc_1_081B
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_05F1
        jmp    dword ptr [eax*4]             ; -> obj1:0x0924
loc_1_05D0:
        cmp    eax, 0x2010102
        je     loc_1_0852
        cmp    eax, 0x2010103
        je     loc_1_0886
        cmp    eax, 0x2010104
        je     loc_1_08B4
loc_1_05F1:
        mov    edi, 0xc00000bb
        jmp    loc_1_08E8
loc_1_05FB:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_0610
        mov    ecx, 0                        ; -> obj4:0x0088
        mov    esi, 0x5c
        jmp    loc_1_08E8
loc_1_0610:
        mov    ecx, 0                        ; -> obj4:0x0004
        mov    esi, 0x84
        jmp    loc_1_08E8
loc_1_061F:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x160], 0
        je     loc_1_0634
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_1_0668
loc_1_0634:
        cmp    byte ptr [ebx + 0x16f], 0
        jne    loc_1_0661
        cmp    byte ptr [ebx + 0x171], 0
        jne    loc_1_0661
        cmp    byte ptr [ebx + 0x172], 0
        je     loc_1_0658
        mov    dword ptr [ebp - 0xc], 4
        jmp    loc_1_0668
loc_1_0658:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_0668
loc_1_0661:
        mov    dword ptr [ebp - 0xc], 1
loc_1_0668:
        lea    ecx, [ebp - 0xc]
        mov    esi, 4
        jmp    loc_1_08E8
loc_1_0675:
        lea    ecx, [ebp - 0x10]
        mov    esi, 4
        jmp    loc_1_08E8
loc_1_0682:
        mov    dword ptr [ebp - 8], 0xf0
        jmp    loc_1_08E8
loc_1_068E:
        mov    ebx, dword ptr [ebp + 8]
        cmp    edx, 0x10106
        mov    eax, dword ptr [ebx + 0xe0]
        mov    dword ptr [ebp - 8], eax
        jne    loc_1_08E8
        sub    dword ptr [ebp - 8], 0xe
        jmp    loc_1_08E8
loc_1_06AF:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x35], 1
        sbb    eax, eax
        and    eax, 0xfffe2b40
        add    eax, 0x27100
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_08E8
loc_1_06CA:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [ebx + 0xdc]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xde]
        imul   edx, eax
        jmp    loc_1_08E5
loc_1_06E7:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [ebx + 0xe4]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xe6]
        imul   edx, eax
        jmp    loc_1_08E5
loc_1_0704:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xde]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_08E8
loc_1_0718:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xe6]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_08E8
loc_1_072C:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 8]
        lea    eax, [ebx + 0x54]
        push   eax
        push   ecx
        call   sub_1_402C
        and    dword ptr [ebp - 8], 0xffffff00
        cmp    byte ptr [ebx + 0x39], 0
        je     loc_1_074F
        or     dword ptr [ebp - 8], 1
loc_1_074F:
        lea    ecx, [ebp - 8]
        mov    esi, 4
        jmp    loc_1_08E8
loc_1_075C:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x39], 0
        je     loc_1_0774
        mov    ecx, 0                        ; -> obj1:0x4108
        mov    esi, 0x2c
        jmp    loc_1_08E8
loc_1_0774:
        mov    ecx, 0                        ; -> obj1:0x4134
        mov    esi, 0x29
        jmp    loc_1_08E8
loc_1_0783:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_079A
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x96]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_08E8
loc_1_079A:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xc]
        mov    ebx, dword ptr [edx + 8]
        mov    dword ptr [ebp - 8], ebx
        jmp    loc_1_08E8
loc_1_07AB:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_07BC
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x18]
        jmp    loc_1_08E5
loc_1_07BC:
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, ebx
        mov    edx, dword ptr [ebx + 0x62]
        sub    eax, edx
        cmp    eax, -0x62
        je     loc_1_08E8
loc_1_07D6:
        mov    eax, dword ptr [edx + 0x18]
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_07EC
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xf0
        je     loc_1_08E8
loc_1_07EC:
        mov    edx, dword ptr [edx]
        mov    eax, ebx
        sub    eax, edx
        cmp    eax, -0x62
        jne    loc_1_07D6
        jmp    loc_1_08E8
loc_1_07FC:
        lea    ecx, [ebp - 2]
        mov    esi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_08E8
loc_1_080F:
        mov    dword ptr [ebp - 8], 6
        jmp    loc_1_08E8
loc_1_081B:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x54]
        add    ebx, 0x54
        mov    byte ptr [ebp - 0x18], al
        mov    cl, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 0x17], cl
        mov    dl, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 0x16], dl
        mov    al, byte ptr [ebx + 3]
        mov    esi, 6
        mov    byte ptr [ebp - 0x15], al
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 0x14], cl
        lea    ecx, [ebp - 0x18]
        mov    dl, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 0x13], dl
        jmp    loc_1_08E8
loc_1_0852:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x4e]
        add    ebx, 0x4e
        mov    byte ptr [ebp - 0x18], al
        mov    cl, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 0x17], cl
        mov    dl, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 0x16], dl
        mov    al, byte ptr [ebx + 3]
        mov    esi, 6
        mov    byte ptr [ebp - 0x15], al
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 0x14], cl
        lea    ecx, [ebp - 0x18]
        mov    dl, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 0x13], dl
        jmp    loc_1_08E8
loc_1_0886:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_089A
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0xc]
        mov    ebx, dword ptr [edx + 0xc]
        mov    dword ptr [ebp - 8], ebx
        jmp    loc_1_08A6
loc_1_089A:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xa2]
        mov    dword ptr [ebp - 8], eax
loc_1_08A6:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        and    edx, 0xff00
        jmp    loc_1_08C8
loc_1_08B4:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xaa]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        and    edx, 0xff00
loc_1_08C8:
        shl    eax, 0x10
        mov    ebx, dword ptr [ebp - 8]
        shr    ebx, 0x10
        or     edx, eax
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xff0000
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
loc_1_08E5:
        mov    dword ptr [ebp - 8], edx
loc_1_08E8:
        test   edi, edi
        jne    loc_1_0916
        cmp    dword ptr [ebp + 0x1c], esi
        jae    loc_1_0906
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
loc_1_0906:
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   ecx
        push   eax
        call   sub_1_402C
        mov    eax, dword ptr [ebp + 0x24]
        add    dword ptr [eax], esi
loc_1_0916:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_05FB
        dd  loc_1_061F
        dd  loc_1_0675
        dd  loc_1_0675
        dd  loc_1_0682
        dd  loc_1_068E
        dd  loc_1_06AF
        dd  loc_1_06CA
        dd  loc_1_06E7
        dd  loc_1_0704
        dd  loc_1_0718
        dd  loc_1_072C
        dd  loc_1_075C
        dd  loc_1_0783
        dd  loc_1_07AB
        dd  loc_1_07FC
        dd  loc_1_068E
        dd  loc_1_05F1
        dd  loc_1_080F
sub_1_0970:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esi + 0x18]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x14]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_0590
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        mov    edi, dword ptr [esi + 0x1c]
        cmp    edi, 4
        je     loc_1_09F0
        mov    eax, 0xc0010014
        pop    edi
        mov    dword ptr [esi + 0x20], ebx
        mov    dword ptr [esi + 0x24], ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_09F0:
        cmp    eax, 0x2010103
        ja     loc_1_0A18
        je     loc_1_0ABD
        cmp    eax, 0x1010e
        je     loc_1_0A35
        cmp    eax, 0x1010f
        je     loc_1_0A58
        cmp    eax, 0x10112
        je     loc_1_0AB9
        jmp    loc_1_0A23
loc_1_0A18:
        cmp    eax, 0x2010104
        je     loc_1_0ACE
loc_1_0A23:
        mov    ebx, 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_0ADF
loc_1_0A35:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_402C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_0B00
        jmp    loc_1_0ADD
loc_1_0A58:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_402C
        cmp    dword ptr [ebp - 4], 0xf0
        ja     loc_1_0AB2
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x165]
        cmp    edx, dword ptr [ebp - 4]
        jae    loc_1_0A8C
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], edx
        mov    dword ptr [eax + 0x165], edx
        jmp    loc_1_0ADF
loc_1_0A8C:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], edx
        jne    loc_1_0AAA
        mov    edx, dword ptr [ecx + 0x18]
        cmp    edx, dword ptr [ebp - 4]
        jbe    loc_1_0AAA
        mov    edx, dword ptr [ebp - 4]
        push   eax
        mov    dword ptr [ecx + 0x18], edx
        call   sub_1_02C0
        jmp    loc_1_0ADF
loc_1_0AAA:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_0ADF
loc_1_0AB2:
        mov    ebx, 0xc0010014
        jmp    loc_1_0ADF
loc_1_0AB9:
        xor    ebx, ebx
        jmp    loc_1_0ADF
loc_1_0ABD:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   eax
        call   sub_1_0B70
        jmp    loc_1_0ADD
loc_1_0ACE:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   eax
        call   sub_1_0BD0
loc_1_0ADD:
        mov    ebx, eax
loc_1_0ADF:
        test   ebx, ebx
        jne    loc_1_0AED
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_1_0AED:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0B00:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        test   eax, 0x8076
        push   ebx
        push   esi
        push   edi
        je     loc_1_0B1D
        mov    eax, 0xc00000bb
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_0B1D:
        mov    esi, dword ptr [ebp + 0xc]
        push   1
        push   eax
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi + 0xc]
        push   eax
        inc    dword ptr [esi + 0x14]
        push   ecx
        mov    eax, dword ptr [edi + 0x86]
        push   eax
        call   sub_2_1310
loc_1_0B3D:
        mov    ebx, eax
        dec    dword ptr [esi + 0x14]
        cmp    ebx, 0x103
        jne    loc_1_0B60
        cmp    byte ptr [edi + 0xcc], 0
        je     loc_1_0B60
        mov    byte ptr [edi + 0xcc], 0
        push   edi
        call   sub_1_2DD8
loc_1_0B60:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B70:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        push   esi
        push   edi
        push   1
        mov    esi, dword ptr [ebp + 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [esi + 0xc]
        push   ecx
        mov    eax, dword ptr [esi + 8]
        push   edx
        inc    dword ptr [esi + 0x14]
        mov    ecx, dword ptr [eax + 0x86]
        push   ecx
        call   sub_2_10E0
loc_1_0B99:
        mov    edi, eax
        dec    dword ptr [esi + 0x14]
        cmp    edi, 0x103
        jne    loc_1_0BBF
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0xcc], 0
        je     loc_1_0BBF
        mov    byte ptr [eax + 0xcc], 0
        push   eax
        call   sub_1_2DD8
loc_1_0BBF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BD0:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        push   esi
        push   edi
        push   1
        mov    esi, dword ptr [ebp + 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [esi + 0xc]
        push   ecx
        mov    eax, dword ptr [esi + 8]
        push   edx
        inc    dword ptr [esi + 0x14]
        mov    ecx, dword ptr [eax + 0x86]
        push   ecx
        call   sub_2_1180
loc_1_0BF9:
        mov    edi, eax
        dec    dword ptr [esi + 0x14]
        cmp    edi, 0x103
        jne    loc_1_0C1F
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0xcc], 0
        je     loc_1_0C1F
        mov    byte ptr [eax + 0xcc], 0
        push   eax
        call   sub_1_2DD8
loc_1_0C1F:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 8], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        push   edx
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 0x14], ecx
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [esi + 0x14]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_0590
        cmp    eax, 0xc00000bb
        mov    edi, eax
        jne    loc_1_0D57
        xor    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x2010105
        ja     loc_1_0CA3
        je     loc_1_0CF7
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_0CC8
        jmp    dword ptr [eax*4]             ; -> obj1:0x0D70
loc_1_0CA3:
        cmp    eax, 0x2020101
        ja     loc_1_0CC1
        je     loc_1_0D17
        cmp    eax, 0x2010106
        je     loc_1_0D07
        cmp    eax, 0x2010107
        je     loc_1_0D0F
        mov    edi, 0xc0010017
        jmp    loc_1_0D28
loc_1_0CC1:
        cmp    eax, 0x2020102
        je     loc_1_0D1F
loc_1_0CC8:
        mov    edi, 0xc0010017
        jmp    loc_1_0D28
loc_1_0CCF:
        mov    eax, dword ptr [ebx + 0xf8]
        jmp    loc_1_0D25
loc_1_0CD7:
        mov    eax, dword ptr [ebx + 0xfc]
        jmp    loc_1_0D25
loc_1_0CDF:
        mov    eax, dword ptr [ebx + 0x100]
        jmp    loc_1_0D25
loc_1_0CE7:
        mov    eax, dword ptr [ebx + 0x104]
        jmp    loc_1_0D25
loc_1_0CEF:
        mov    eax, dword ptr [ebx + 0x120]
        jmp    loc_1_0D25
loc_1_0CF7:
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x173]
        or     eax, 0xc0011000
        jmp    loc_1_0D25
loc_1_0D07:
        mov    eax, dword ptr [ebx + 0x158]
        jmp    loc_1_0D25
loc_1_0D0F:
        mov    eax, dword ptr [ebx + 0x15c]
        jmp    loc_1_0D25
loc_1_0D17:
        mov    eax, dword ptr [ebx + 0x108]
        jmp    loc_1_0D25
loc_1_0D1F:
        mov    eax, dword ptr [ebx + 0x11c]
loc_1_0D25:
        mov    dword ptr [ebp - 4], eax
loc_1_0D28:
        test   edi, edi
        jne    loc_1_0D57
        cmp    dword ptr [ebp - 0x10], 4
        jb     loc_1_0D47
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_402C
        add    dword ptr [ebp - 0xc], 4
        jmp    loc_1_0D57
loc_1_0D47:
        mov    eax, 4
        mov    edi, 0xc0010014
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 8], eax
loc_1_0D57:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_0CCF
        dd  loc_1_0CD7
        dd  loc_1_0CDF
        dd  loc_1_0CE7
        dd  loc_1_0CEF
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D90:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x5e]
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x10113
        ja     loc_1_0DC2
        je     loc_1_0DFF
        cmp    eax, 0x10101
        jb     loc_1_0DF3
        cmp    eax, 0x10111
        jbe    loc_1_0DFF
        mov    esi, 0xc0010017
        jmp    loc_1_0DFF
loc_1_0DC2:
        cmp    eax, 0x2010107
        ja     loc_1_0DE5
        cmp    eax, 0x2010101
        jae    loc_1_0DFF
        cmp    eax, 0x20101
        jb     loc_1_0DF3
        cmp    eax, 0x20105
        jbe    loc_1_0DFA
        mov    esi, 0xc0010017
        jmp    loc_1_0DFF
loc_1_0DE5:
        cmp    eax, 0x2020101
        jb     loc_1_0DF3
        cmp    eax, 0x2020102
        jbe    loc_1_0DFA
loc_1_0DF3:
        mov    esi, 0xc0010017
        jmp    loc_1_0DFF
loc_1_0DFA:
        mov    esi, 0x103
loc_1_0DFF:
        cmp    esi, 0x103
        jne    loc_1_0E4E
        mov    dword ptr [ecx], 0
        mov    byte ptr [ecx + 4], 0
        cmp    dword ptr [edi + 0x8a], 0
        jne    loc_1_0E28
        mov    dword ptr [edi + 0x8e], ecx
        mov    dword ptr [edi + 0x8a], ecx
        jmp    loc_1_0E30
loc_1_0E28:
        mov    eax, dword ptr [edi + 0x8e]
        mov    dword ptr [eax], ecx
loc_1_0E30:
        cmp    byte ptr [edi + 0xcc], 0
        je     loc_1_0E46
        mov    byte ptr [edi + 0xcc], 0
        push   edi
        call   sub_1_2DD8
loc_1_0E46:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_0E4E:
        test   esi, esi
        jne    loc_1_0E5B
        push   ecx
        push   edi
        call   sub_1_0C30
        mov    esi, eax
loc_1_0E5B:
        mov    eax, dword ptr [edi + 0x5e]
        dec    eax
        mov    dword ptr [edi + 0x5e], eax
        jne    loc_1_0E7D
        cmp    byte ptr [edi + 0x160], 0
        jne    loc_1_0E77
        mov    eax, dword ptr [edi + 0x6a]
        sub    eax, edi
        cmp    eax, 0x6a
        je     loc_1_0E7D
loc_1_0E77:
        push   edi
        call   sub_1_35C8
loc_1_0E7D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0E90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, 0x103
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [edi + 0x5e]
        cmp    byte ptr [edi + 0x160], 0
        je     loc_1_0EB4
        mov    esi, 0xc001000d
        jmp    loc_1_0EDE
loc_1_0EB4:
        cmp    byte ptr [edi + 0x170], 0
        je     loc_1_0EC4
        mov    esi, 0xc0000001
        jmp    loc_1_0EDE
loc_1_0EC4:
        cmp    byte ptr [ebx + 0x1d], 0
        jne    loc_1_0ED9
        push   ebx
        inc    dword ptr [ebx + 0x14]
        push   edi
        call   sub_1_11B0
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_0EDE
loc_1_0ED9:
        mov    esi, 0xc0010002
loc_1_0EDE:
        mov    eax, dword ptr [edi + 0x5e]
        dec    eax
        mov    dword ptr [edi + 0x5e], eax
        jne    loc_1_0F00
        cmp    byte ptr [edi + 0x160], 0
        jne    loc_1_0EFA
        mov    eax, dword ptr [edi + 0x6a]
        sub    eax, edi
        cmp    eax, 0x6a
        je     loc_1_0F00
loc_1_0EFA:
        push   edi
        call   sub_1_35C8
loc_1_0F00:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0F10:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [edx + 8]
        mov    esi, dword ptr [ebp + 0x14]
        test   esi, esi
        jne    loc_1_0F2C
        lea    esi, [edx + 0x1e]
        mov    dword ptr [esi + 0x10], 4
loc_1_0F2C:
        cmp    byte ptr [ecx + 0x160], 0
        je     loc_1_0F3F
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
loc_1_0F3F:
        mov    eax, dword ptr [ebp + 0xc]
        test   al, 0x30
        je     loc_1_0F50
        mov    eax, 0xc0000001
        pop    esi
        pop    ebp
        ret    0x14
loc_1_0F50:
        mov    dword ptr [esi + 4], edx
        cmp    dword ptr [esi + 0x10], 4
        mov    dword ptr [esi + 8], eax
        je     loc_1_0F5F
        mov    dword ptr [esi + 8], eax
loc_1_0F5F:
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 0x8a], 0
        jne    loc_1_0F7C
        mov    dword ptr [ecx + 0x8e], esi
        mov    dword ptr [ecx + 0x8a], esi
        jmp    loc_1_0F84
loc_1_0F7C:
        mov    eax, dword ptr [ecx + 0x8e]
        mov    dword ptr [eax], esi
loc_1_0F84:
        mov    eax, 0x103
        pop    esi
        pop    ebp
        inc    dword ptr [edx + 0x14]
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FA0:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [edx + 8]
        mov    esi, dword ptr [ebp + 0x14]
        test   esi, esi
        jne    loc_1_0FBC
        lea    esi, [edx + 0x46]
        mov    dword ptr [esi + 0x10], 9
loc_1_0FBC:
        cmp    byte ptr [ecx + 0x160], 0
        je     loc_1_0FCF
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
loc_1_0FCF:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 0x8a], 0
        jne    loc_1_0FF5
        mov    dword ptr [ecx + 0x8e], esi
        mov    dword ptr [ecx + 0x8a], esi
        jmp    loc_1_0FFD
loc_1_0FF5:
        mov    eax, dword ptr [ecx + 0x8e]
        mov    dword ptr [eax], esi
loc_1_0FFD:
        mov    eax, 0x103
        pop    esi
        pop    ebp
        inc    dword ptr [edx + 0x14]
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1010:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [edx + 8]
        mov    esi, dword ptr [ebp + 0x14]
        test   esi, esi
        jne    loc_1_102C
        lea    esi, [edx + 0x6e]
        mov    dword ptr [esi + 0x10], 0xa
loc_1_102C:
        cmp    byte ptr [ecx + 0x160], 0
        je     loc_1_103F
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
loc_1_103F:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 0x8a], 0
        jne    loc_1_1065
        mov    dword ptr [ecx + 0x8e], esi
        mov    dword ptr [ecx + 0x8a], esi
        jmp    loc_1_106D
loc_1_1065:
        mov    eax, dword ptr [ecx + 0x8e]
        mov    dword ptr [eax], esi
loc_1_106D:
        mov    eax, 0x103
        pop    esi
        pop    ebp
        inc    dword ptr [edx + 0x14]
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1080:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 0x14]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1090:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   0                             ; -> loc_1_1AE8
        push   esi
        call   sub_1_3F3A
        mov    edx, dword ptr [esi + 0x5a]
        xor    al, al
        inc    edx
        push   0xc350
        out    dx, al
        call   sub_1_3F40
        mov    edx, dword ptr [esi + 0x5a]
        xor    al, al
        add    edx, 2
        cmp    byte ptr [esi + 0x3b], al
        out    dx, al
        je     loc_1_10CF
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e18], 0xc0
loc_1_10CF:
        cmp    byte ptr [esi + 0x39], 0
        je     loc_1_10E4
        mov    cl, byte ptr [esi + 0x48]
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e00], cl
loc_1_10E4:
        push   esi
        push   0                             ; -> loc_1_1AC8
        push   esi
        call   sub_1_3F3A
        mov    eax, dword ptr [esi + 0xae]
        push   0xc001000c
        push   esi
        mov    byte ptr [eax + 0x1e08], 0xc0
        call   sub_1_3818
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_1110:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   0
        xor    eax, eax
        cmp    byte ptr [esi + 0x176], al
        je     loc_1_113B
        mov    byte ptr [esi + 0x176], al
        mov    byte ptr [esi + 0x175], al
        mov    byte ptr [esi + 0x177], al
loc_1_113B:
        xor    ebx, ebx
        push   esi
        mov    byte ptr [esi + 0x160], bl
        mov    byte ptr [esi + 0x169], bl
        mov    byte ptr [esi + 0x16a], bl
        mov    byte ptr [esi + 0x172], bl
        call   sub_1_2E48
        mov    edi, dword ptr [esi + 0x16b]
        test   edi, edi
        je     loc_1_11A5
        mov    ebx, dword ptr [esi + 0x62]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -0x62
        je     loc_1_1197
loc_1_1171:
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   0
        push   0x40010005
        push   eax
        call   sub_1_3EAE
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_3EB4
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -0x62
        jne    loc_1_1171
loc_1_1197:
        push   0
        dec    dword ptr [edi + 0x14]
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3E96
loc_1_11A5:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_11B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        test   edi, edi
        je     loc_1_11E9
        mov    ebx, dword ptr [esi + 0x62]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -0x62
        je     loc_1_11E9
loc_1_11CC:
        push   0
        mov    eax, dword ptr [ebx + 0x10]
        push   0
        push   0x40010004
        push   eax
        call   sub_1_3EAE
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -0x62
        jne    loc_1_11CC
loc_1_11E9:
        mov    eax, 1
        mov    dword ptr [esi + 0x16b], edi
        mov    byte ptr [esi + 0x160], al
        mov    byte ptr [esi + 0x172], al
        mov    dword ptr [esi + 0x161], 0
        test   edi, edi
        je     loc_1_1211
        add    dword ptr [edi + 0x14], eax
loc_1_1211:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1220:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x82]
        push   eax
        call   sub_1_4024
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x1d]
        push   eax
        push   ecx
        call   sub_1_3F28
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_1253
        push   0x7a120
        call   sub_1_3F40
loc_1_1253:
        push   esi
        call   sub_1_3F34
        mov    eax, dword ptr [esi + 0x86]
        push   eax
        call   sub_2_0EF0
loc_1_1265:
        mov    eax, dword ptr [esi + 0x82]
        push   eax
        call   sub_1_3E78
        push   0
        push   0x379
        push   esi
        call   sub_1_3F4C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1290:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 0x172], 1
        cmp    byte ptr [eax + 0xcc], 0
        je     loc_1_12DB
        mov    ecx, dword ptr [eax + 0xb8]
        test   ecx, ecx
        je     loc_1_12DB
        mov    edx, dword ptr [eax + 0xae]
        pop    ebp
        mov    byte ptr [edx + 0x1e08], 0x80
        mov    word ptr [ecx], 0
        mov    byte ptr [ecx + 2], 0
        mov    byte ptr [ecx], 4
        mov    edx, dword ptr [eax + 0xae]
        mov    byte ptr [edx + 0x1e4b], 0x20
        ret    4
loc_1_12DB:
        mov    edx, dword ptr [eax + 0x5a]
        pop    ebp
        inc    edx
        xor    al, al
        out    dx, al
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_1_3E6C
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_3E60
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1320:
        mov    al, byte ptr [0x40d8]         ; -> obj1:0x40D8
        cmp    al, 0
        jne    loc_1_1345
        inc    byte ptr [0x40d8]             ; -> obj1:0x40D8
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_1347
        push   0
        push   0
        call   sub_2_0000
        cmp    eax, 0
        jne    loc_1_1347
loc_1_1345:
        clc    
        ret    
loc_1_1347:
        stc    
        ret    
IBMTOK4_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_1358
        jmp    dword ptr [eax*4 + 0x135a]    ; -> obj1:0x135A
loc_1_1358:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_1320
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1358
        dd  loc_1_1320
        db  000h, 000h
loc_1_13C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], 0x103
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [edi + 8]
        push   0
        push   0
        push   ebx
        call   sub_1_3F70
        cmp    dword ptr [ebp - 8], 0xe
        jb     loc_1_14F3
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0xe0], eax
        jb     loc_1_14F3
        inc    dword ptr [esi + 0x5e]
        cmp    byte ptr [esi + 0x175], 0
        je     loc_1_1422
        mov    dword ptr [ebp - 4], 0xc0010008
        jmp    loc_1_14C5
loc_1_1422:
        cmp    byte ptr [esi + 0x172], 0
        jne    loc_1_1487
        cmp    byte ptr [edi + 0x1d], 0
        lea    eax, [ebx + 0x24]
        je     loc_1_1440
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_14C5
loc_1_1440:
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], ebx
        cmp    dword ptr [esi + 0xe8], 0
        jne    loc_1_1457
        mov    dword ptr [esi + 0xe8], ebx
        jmp    loc_1_1460
loc_1_1457:
        mov    ecx, dword ptr [esi + 0xec]
        mov    dword ptr [ecx + 0x24], ebx
loc_1_1460:
        mov    dword ptr [esi + 0xec], ebx
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 0x14]
        cmp    byte ptr [esi + 0xcc], 0
        je     loc_1_14C5
        mov    byte ptr [esi + 0xcc], 0
        push   esi
        call   sub_1_2DD8
        jmp    loc_1_14C5
loc_1_1487:
        cmp    byte ptr [esi + 0x160], 0
        je     loc_1_1499
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_14C5
loc_1_1499:
        cmp    byte ptr [esi + 0x170], 0
        je     loc_1_14AB
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_14C5
loc_1_14AB:
        push   2
        mov    eax, dword ptr [esi + 0x82]
        push   0xb
        push   2
        push   0xaa000006
        push   eax
        call   sub_1_4018
        add    esp, 0x14
loc_1_14C5:
        mov    eax, dword ptr [esi + 0x5e]
        dec    eax
        mov    dword ptr [esi + 0x5e], eax
        jne    loc_1_14E7
        cmp    byte ptr [esi + 0x160], 0
        jne    loc_1_14E1
        mov    eax, dword ptr [esi + 0x6a]
        sub    eax, esi
        cmp    eax, 0x6a
        je     loc_1_14E7
loc_1_14E1:
        push   esi
        call   sub_1_35C8
loc_1_14E7:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_14F3:
        mov    eax, 0xc001000f
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1508:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x20
        mov    edx, dword ptr [eax + 8]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0x14], edx
        push   edi
        inc    dword ptr [edx + 0x5e]
        cmp    byte ptr [edx + 0x172], 0
        jne    loc_1_16BC
        cmp    byte ptr [eax + 0x1d], 0
        jne    loc_1_16B5
        mov    bx, word ptr [edx + 0xd7]
        mov    ecx, dword ptr [ebp + 0x1c]
        push   0
        lea    edx, [ebp - 8]
        push   edx
        inc    dword ptr [eax + 0x14]
        lea    eax, [ebp - 0x20]
        mov    dword ptr [ecx], 0
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ebp - 0x18], 0
        push   0
        push   ecx
        call   sub_1_3F70
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_1_16AB
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x1c]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3F94
        mov    ah, bl
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, bh
        mov    ecx, eax
        mov    eax, dword ptr [edx + 0xb2]
        and    ecx, 0xffff
        lea    esi, [ecx + eax + 2]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0xd9]
        lea    eax, [esi + ecx + 6]
        mov    dword ptr [ebp - 0x10], eax
        mov    ax, word ptr [esi + 4]
        mov    dh, al
        mov    dl, ah
        mov    eax, edx
        and    eax, 0xffff
        sub    eax, ecx
        cmp    dword ptr [ebp + 0x14], 0
        mov    dword ptr [ebp - 4], eax
        je     loc_1_16A3
        mov    edi, dword ptr [ebp + 0x10]
loc_1_15C9:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_15FC
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3FB8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_16A3
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x1c]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3F94
        jmp    loc_1_1697
loc_1_15FC:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1645
        mov    ax, word ptr [esi]
        test   ax, ax
        je     loc_1_16A3
        mov    ch, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    cl, ah
        mov    esi, ecx
        and    esi, 0xffff
        add    esi, dword ptr [edx + 0xb2]
        mov    cx, word ptr [esi + 4]
        lea    eax, [esi + 6]
        mov    word ptr [ebp - 4], cx
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, ah
        mov    ah, cl
        mov    ecx, eax
        and    ecx, 0xffff
        mov    dword ptr [ebp - 4], ecx
        jmp    loc_1_1697
loc_1_1645:
        test   edi, edi
        je     loc_1_1662
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_165A
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1697
loc_1_165A:
        add    dword ptr [ebp - 0x10], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_1662:
        mov    ebx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    eax, ecx
        jb     loc_1_1674
        mov    eax, ecx
loc_1_1674:
        cmp    ebx, eax
        jb     loc_1_167A
        mov    ebx, eax
loc_1_167A:
        push   ebx
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    ecx, dword ptr [ebp - 0x1c]
        push   ecx
        call   sub_1_402C
        add    dword ptr [ebp - 0x1c], ebx
        add    dword ptr [ebp - 0x10], ebx
        add    dword ptr [ebp - 0x18], ebx
        sub    dword ptr [ebp - 4], ebx
        sub    dword ptr [ebp - 0xc], ebx
loc_1_1697:
        mov    ecx, dword ptr [ebp - 0x18]
        cmp    ecx, dword ptr [ebp + 0x14]
        jb     loc_1_15C9
loc_1_16A3:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [eax], ecx
loc_1_16AB:
        mov    eax, dword ptr [ebp + 8]
        xor    esi, esi
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_1700
loc_1_16B5:
        mov    esi, 0xc001000c
        jmp    loc_1_1700
loc_1_16BC:
        mov    edx, dword ptr [ebp - 0x14]
        cmp    byte ptr [edx + 0x160], 0
        je     loc_1_16CF
        mov    esi, 0xc001000d
        jmp    loc_1_1700
loc_1_16CF:
        mov    edx, dword ptr [ebp - 0x14]
        mov    esi, 0xc0000001
        cmp    byte ptr [edx + 0x171], 0
        jne    loc_1_1700
        push   1
        mov    edx, dword ptr [ebp - 0x14]
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [edx + 0x82]
        push   eax
        call   sub_1_4018
        add    esp, 0x14
        mov    esi, dword ptr [ebp - 4]
loc_1_1700:
        mov    edx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [edx + 0x5e]
        dec    eax
        mov    dword ptr [edx + 0x5e], eax
        jne    loc_1_1728
        cmp    byte ptr [edx + 0x160], 0
        jne    loc_1_171F
        mov    eax, dword ptr [edx + 0x6a]
        sub    eax, edx
        cmp    eax, 0x6a
        je     loc_1_1728
loc_1_171F:
        mov    edx, dword ptr [ebp - 0x14]
        push   edx
        call   sub_1_35C8
loc_1_1728:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  005h, 000h, 000h, 000h, 000h
sub_1_1738:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_180F
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 0x10]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_3F70
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_180F
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_3F94
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_180A
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 0x14]
loc_1_178E:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_17BA
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_3FB8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_180A
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_3F94
        jmp    loc_1_1805
loc_1_17BA:
        test   edi, edi
        je     loc_1_17D7
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_17CF
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_1805
loc_1_17CF:
        add    dword ptr [ebp - 8], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_17D7:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, esi
        cmp    ecx, eax
        jb     loc_1_17E5
        mov    ecx, eax
loc_1_17E5:
        xor    edx, edx
        test   ecx, ecx
        je     loc_1_17FB
loc_1_17EB:
        mov    eax, dword ptr [ebp - 8]
        inc    edx
        cmp    edx, ecx
        mov    al, byte ptr [eax + edx - 1]
        mov    byte ptr [ebx + edx - 1], al
        jb     loc_1_17EB
loc_1_17FB:
        add    ebx, ecx
        add    esi, ecx
        add    dword ptr [ebp - 8], ecx
        sub    dword ptr [ebp - 4], ecx
loc_1_1805:
        cmp    esi, dword ptr [ebp + 0x10]
        jb     loc_1_178E
loc_1_180A:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_180F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  055h, 08Bh, 044h, 024h, 018h, 08Bh, 0ECh, 083h, 0ECh, 014h, 053h, 056h, 057h, 033h, 0F6h, 089h
        db  030h, 039h, 075h, 00Ch, 00Fh, 084h, 0B2h, 000h, 000h, 000h, 056h, 08Dh, 045h, 0F8h, 050h, 08Dh
        db  04Dh, 0ECh, 051h, 08Bh, 055h, 010h, 056h, 052h, 0E8h, 02Bh, 027h, 000h, 000h, 039h, 075h, 0ECh
        db  00Fh, 084h, 096h, 000h, 000h, 000h, 08Dh, 045h, 0FCh, 08Dh, 04Dh, 0F4h, 050h, 08Bh, 055h, 0F8h
        db  051h, 052h, 0E8h, 035h, 027h, 000h, 000h, 08Bh, 04Dh, 008h, 039h, 075h, 00Ch, 089h, 04Dh, 0F0h
        db  074h, 075h, 08Bh, 07Dh, 014h, 083h, 07Dh, 0FCh, 000h, 075h, 026h, 08Dh, 045h, 0F8h, 08Bh, 04Dh
        db  0F8h, 050h, 051h, 0E8h, 038h, 027h, 000h, 000h, 083h, 07Dh, 0F8h, 000h, 074h, 059h, 08Dh, 045h
        db  0FCh, 08Dh, 04Dh, 0F4h, 050h, 08Bh, 055h, 0F8h, 051h, 052h, 0E8h, 0FDh, 026h, 000h, 000h, 0EBh
        db  041h, 085h, 0FFh, 074h, 019h, 03Bh, 07Dh, 0FCh, 076h, 00Ch, 02Bh, 07Dh, 0FCh, 0C7h, 045h, 0FCh
        db  000h, 000h, 000h, 000h, 0EBh, 02Ch, 001h, 07Dh, 0F4h, 029h, 07Dh, 0FCh, 033h, 0FFh, 08Bh, 05Dh
        db  00Ch, 08Bh, 045h, 0FCh, 02Bh, 0DEh, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 053h, 08Bh, 045h, 0F0h
        db  050h, 08Bh, 04Dh, 0F4h, 051h, 003h, 0F3h, 0E8h, 058h, 027h, 000h, 000h, 001h, 05Dh, 0F0h, 029h
        db  05Dh, 0FCh, 039h, 075h, 00Ch, 077h, 08Eh, 08Bh, 045h, 018h, 089h, 030h, 05Fh, 05Eh, 05Bh, 08Bh
        db  0E5h, 05Dh, 0C2h, 014h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        db  055h, 08Bh, 044h, 024h, 01Ch, 08Bh, 0ECh, 083h, 0ECh, 024h, 0C7h, 000h, 000h, 000h, 000h, 000h
        db  053h, 0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h, 083h, 07Dh, 010h, 000h, 056h, 057h, 00Fh, 084h
        db  056h, 001h, 000h, 000h, 06Ah, 000h, 08Dh, 045h, 0F0h, 050h, 08Dh, 04Dh, 0DCh, 051h, 08Bh, 055h
        db  008h, 06Ah, 000h, 052h, 0E8h, 03Fh, 026h, 000h, 000h, 083h, 07Dh, 0DCh, 000h, 00Fh, 084h, 037h
        db  001h, 000h, 000h, 08Dh, 045h, 0FCh, 08Dh, 04Dh, 0E8h, 050h, 08Bh, 055h, 0F0h, 051h, 052h, 0E8h
        db  048h, 026h, 000h, 000h, 06Ah, 000h, 08Dh, 04Dh, 0F4h, 051h, 08Dh, 055h, 0E0h, 052h, 08Bh, 045h
        db  014h, 06Ah, 000h, 050h, 0E8h, 00Fh, 026h, 000h, 000h, 083h, 07Dh, 0E0h, 000h, 00Fh, 084h, 007h
        db  001h, 000h, 000h, 08Dh, 045h, 0F8h, 08Dh, 04Dh, 0ECh, 050h, 08Bh, 055h, 0F4h, 051h, 052h, 0E8h
        db  018h, 026h, 000h, 000h, 083h, 07Dh, 010h, 000h, 00Fh, 084h, 0E4h, 000h, 000h, 000h, 08Bh, 075h
        db  00Ch, 08Bh, 07Dh, 018h, 083h, 07Dh, 0FCh, 000h, 075h, 02Dh, 08Dh, 045h, 0F0h, 08Bh, 04Dh, 0F0h
        db  050h, 051h, 0E8h, 019h, 026h, 000h, 000h, 083h, 07Dh, 0F0h, 000h, 00Fh, 084h, 0C1h, 000h, 000h
        db  000h, 08Dh, 045h, 0FCh, 08Dh, 04Dh, 0E8h, 050h, 08Bh, 055h, 0F0h, 051h, 052h, 0E8h, 0DAh, 025h
        db  000h, 000h, 0E9h, 09Fh, 000h, 000h, 000h, 083h, 07Dh, 0F8h, 000h, 075h, 02Ah, 08Dh, 045h, 0F4h
        db  08Bh, 04Dh, 0F4h, 050h, 051h, 0E8h, 0E6h, 025h, 000h, 000h, 083h, 07Dh, 0F4h, 000h, 00Fh, 084h
        db  08Eh, 000h, 000h, 000h, 08Dh, 045h, 0F8h, 08Dh, 04Dh, 0ECh, 050h, 08Bh, 055h, 0F4h, 051h, 052h
        db  0E8h, 0A7h, 025h, 000h, 000h, 0EBh, 06Fh, 085h, 0F6h, 074h, 019h, 039h, 075h, 0FCh, 073h, 00Ch
        db  02Bh, 075h, 0FCh, 0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h, 0EBh, 05Ah, 001h, 075h, 0E8h, 029h
        db  075h, 0FCh, 033h, 0F6h, 085h, 0FFh, 074h, 019h, 039h, 07Dh, 0F8h, 073h, 00Ch, 02Bh, 07Dh, 0F8h
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h, 0EBh, 03Dh, 001h, 07Dh, 0ECh, 029h, 07Dh, 0F8h, 033h
        db  0FFh, 08Bh, 05Dh, 0F8h, 08Bh, 045h, 0FCh, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 08Bh, 045h, 010h
        db  02Bh, 045h, 0E4h, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 053h, 08Bh, 045h, 0ECh, 050h, 08Bh, 04Dh
        db  0E8h, 051h, 0E8h, 0DDh, 025h, 000h, 000h, 001h, 05Dh, 0E8h, 001h, 05Dh, 0ECh, 001h, 05Dh, 0E4h
        db  029h, 05Dh, 0F8h, 029h, 05Dh, 0FCh, 08Bh, 04Dh, 0E4h, 039h, 04Dh, 010h, 00Fh, 087h, 022h, 0FFh
        db  0FFh, 0FFh, 08Bh, 045h, 01Ch, 08Bh, 04Dh, 0E4h, 089h, 008h, 05Fh, 05Eh, 05Bh, 08Bh, 0E5h, 05Dh
        db  0C2h, 018h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A88:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx]
        mov    dl, byte ptr [eax + 0x49]
        mov    eax, dword ptr [ecx + 4]
        and    dl, 0x24
        pop    ebp
        mov    byte ptr [eax], dl
        mov    al, 1
        mov    ecx, dword ptr [ecx]
        and    byte ptr [ecx + 0x49], 0xdb
        ret    4
loc_1_1AA8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx]
        mov    dl, byte ptr [eax + 0x49]
        mov    eax, dword ptr [ecx + 4]
        and    dl, 0x18
        pop    ebp
        mov    byte ptr [eax], dl
        mov    al, 1
        mov    ecx, dword ptr [ecx]
        and    byte ptr [ecx + 0x49], 0xe7
        ret    4
loc_1_1AC8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 0x169], 1
        mov    al, 1
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1AE8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 0x49], 0
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1B08:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ecx + 0xae]
        cmp    byte ptr [ecx + 0x2c], 0
        mov    bl, byte ptr [eax + 0x1e09]
        jne    loc_1_1B3A
        mov    al, byte ptr [ecx + 0x18]
        inc    al
        mov    byte ptr [ecx + 0x18], al
        cmp    al, 0xff
        jne    loc_1_1B3A
        mov    edx, dword ptr [ecx + 0x5a]
        xor    al, al
        inc    edx
        pop    ebx
        out    dx, al
        pop    ebp
        ret    4
loc_1_1B3A:
        mov    dl, bl
        mov    eax, dword ptr [ecx + 0xae]
        not    dl
        mov    byte ptr [eax + 0x1e29], dl
        cmp    byte ptr [ecx + 0x172], 0
        je     loc_1_1B5F
        mov    al, bl
        and    bl, 0x20
        and    al, 0xdf
        or     byte ptr [ecx + 0x4a], al
        jmp    loc_1_1B66
loc_1_1B5F:
        or     bl, byte ptr [ecx + 0x4a]
        mov    byte ptr [ecx + 0x4a], 0
loc_1_1B66:
        or     byte ptr [ecx + 0x49], bl
        cmp    byte ptr [ecx + 0x169], 0
        je     loc_1_1B79
        mov    byte ptr [ecx + 0x16a], 1
loc_1_1B79:
        cmp    byte ptr [ecx + 0x16f], 0
        je     loc_1_1BF4
        mov    eax, dword ptr [ecx + 0xae]
        mov    dl, byte ptr [eax + 0x1e02]
        mov    al, byte ptr [eax + 0x1e03]
        and    edx, 0xffff00ff
        and    eax, 0xffff00ff
        shl    dx, 8
        add    ax, dx
        test   al, 1
        je     loc_1_1BC0
        cmp    byte ptr [ecx + 0x39], 0
        je     loc_1_1BB9
        mov    edx, dword ptr [ecx + 0x5a]
        xor    al, al
        add    edx, 3
        out    dx, al
loc_1_1BB9:
        xor    al, al
        pop    ebx
        pop    ebp
        ret    4
loc_1_1BC0:
        mov    word ptr [ecx + 0xb6], ax
        and    eax, 0xffff
        mov    edx, dword ptr [ecx + 0xb2]
        cmp    word ptr [edx + eax + 6], 0
        jne    loc_1_1BDE
        mov    byte ptr [ecx + 0x2c], 1
loc_1_1BDE:
        cmp    byte ptr [ecx + 0x39], 0
        je     loc_1_1BED
        mov    edx, dword ptr [ecx + 0x5a]
        xor    al, al
        add    edx, 3
        out    dx, al
loc_1_1BED:
        xor    al, al
        pop    ebx
        pop    ebp
        ret    4
loc_1_1BF4:
        cmp    byte ptr [ecx + 0x39], 0
        je     loc_1_1C03
        mov    edx, dword ptr [ecx + 0x5a]
        xor    al, al
        add    edx, 3
        out    dx, al
loc_1_1C03:
        test   bl, bl
        jne    loc_1_1C27
        mov    eax, dword ptr [ecx + 0xae]
        mov    dl, byte ptr [eax + 0x1e08]
        and    dl, 0x1c
        mov    byte ptr [ecx + 0x4b], dl
        mov    eax, dword ptr [ecx + 0xae]
        not    dl
        mov    byte ptr [eax + 0x1e28], dl
loc_1_1C27:
        cmp    byte ptr [ecx + 0x49], 1
        sbb    al, al
        pop    ebx
        inc    al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1C38:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x29], 0
        mov    al, byte ptr [esi + 0x4b]
        test   al, al
        je     loc_1_1C72
        and    eax, 0xff
        mov    ecx, dword ptr [esi + 0x82]
        push   eax
        push   0xc
        push   8
        push   3
        push   0xaa000003
        push   ecx
        call   sub_1_4018
        mov    byte ptr [esi + 0x4b], 0
        add    esp, 0x18
loc_1_1C72:
        test   byte ptr [esi + 0x49], 0x24
        je     loc_1_1C84
        cmp    byte ptr [esi + 0x4c], 0
        jne    loc_1_1C84
        push   esi
        call   sub_1_1CA8
loc_1_1C84:
        test   byte ptr [esi + 0x49], 0x18
        je     loc_1_1C96
        cmp    byte ptr [esi + 0x4d], 0
        jne    loc_1_1C96
        push   esi
        call   sub_1_2218
loc_1_1C96:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1CA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x5e]
        cmp    byte ptr [esi + 0x160], 0
        je     loc_1_1D02
        cmp    byte ptr [esi + 0x16a], 0
        je     loc_1_1D02
        push   esi
        call   sub_1_2A58
        mov    eax, dword ptr [esi + 0x5e]
        dec    eax
        mov    dword ptr [esi + 0x5e], eax
        jne    loc_1_220C
        cmp    byte ptr [esi + 0x160], 0
        jne    loc_1_1CF3
        mov    eax, dword ptr [esi + 0x6a]
        sub    eax, esi
        cmp    eax, 0x6a
        je     loc_1_220C
loc_1_1CF3:
        push   esi
        call   sub_1_35C8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1D02:
        lea    eax, [ebp - 0xa]
        lea    ecx, [ebp - 8]
        push   ecx
        mov    dword ptr [ebp - 8], esi
        push   0                             ; -> loc_1_1A88
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 0x4c], 1
        push   esi
        call   sub_1_3F3A
        test   byte ptr [ebp - 0xa], 0x24
        je     loc_1_21E6
loc_1_1D28:
        cmp    byte ptr [esi + 0x175], 0
        je     loc_1_1D3E
        cmp    byte ptr [esi + 0x176], 0
        je     loc_1_21E6
loc_1_1D3E:
        test   byte ptr [ebp - 0xa], 0x20
        je     loc_1_20A3
        cmp    byte ptr [esi + 0x171], 0
        je     loc_1_1FD8
        mov    edi, dword ptr [esi + 0xb2]
        xor    eax, eax
        mov    ax, word ptr [esi + 0xb6]
        lea    ecx, [eax + edi]
        cmp    byte ptr [ecx + 2], 0
        jne    loc_1_1EFD
        cmp    byte ptr [esi + 0x3b], 0
        je     loc_1_1E0B
        mov    dx, word ptr [ecx + 0xa]
        mov    ah, dl
        mov    al, dh
        mov    dx, ax
        shr    ax, 0xe
        and    edx, 0x3fff
        add    edx, edi
        mov    byte ptr [esi + 0xc8], al
        mov    dword ptr [esi + 0xb8], edx
        mov    dx, word ptr [ecx + 0xe]
        mov    ah, dl
        mov    al, dh
        mov    dx, ax
        shr    ax, 0xe
        and    edx, 0x3fff
        add    edx, edi
        mov    byte ptr [esi + 0xc9], al
        mov    dword ptr [esi + 0xbc], edx
        mov    dx, word ptr [ecx + 0xc]
        mov    ah, dl
        mov    al, dh
        mov    dx, ax
        shr    ax, 0xe
        and    edx, 0x3fff
        add    edx, edi
        lea    ebx, [esi + 0xc4]
        mov    dword ptr [esi + 0xc0], edx
        mov    byte ptr [esi + 0xca], al
        mov    dx, word ptr [ecx + 8]
        mov    ah, dl
        mov    al, dh
        mov    dx, ax
        shr    ax, 0xe
        and    edx, 0x3fff
        add    edx, edi
        mov    byte ptr [esi + 0xcb], al
        mov    dword ptr [ebx], edx
        jmp    loc_1_1E7B
loc_1_1E0B:
        mov    ax, word ptr [ecx + 0xa]
        lea    edx, [esi + 0xb8]
        mov    dh, al
        mov    dl, ah
        mov    eax, edx
        lea    edx, [esi + 0xbc]
        and    eax, 0xffff
        add    eax, edi
        mov    dword ptr [esi + 0xb8], eax
        mov    ax, word ptr [ecx + 0xe]
        mov    dh, al
        mov    dl, ah
        mov    eax, edx
        lea    edx, [esi + 0xc0]
        and    eax, 0xffff
        add    eax, edi
        mov    dword ptr [esi + 0xbc], eax
        mov    ax, word ptr [ecx + 0xc]
        mov    dh, al
        lea    ebx, [esi + 0xc4]
        mov    dl, ah
        mov    eax, edx
        and    eax, 0xffff
        add    eax, edi
        mov    dword ptr [esi + 0xc0], eax
        mov    ax, word ptr [ecx + 8]
        mov    dh, al
        mov    dl, ah
        mov    eax, edx
        and    eax, 0xffff
        add    eax, edi
        mov    dword ptr [ebx], eax
loc_1_1E7B:
        mov    eax, dword ptr [esi + 0x40]
        add    eax, edi
        cmp    dword ptr [esi + 0xb8], eax
        ja     loc_1_1EFD
        cmp    dword ptr [esi + 0xbc], eax
        ja     loc_1_1EFD
        cmp    dword ptr [esi + 0xc0], eax
        ja     loc_1_1EFD
        cmp    dword ptr [ebx], eax
        ja     loc_1_1EFD
        mov    dword ptr [esi + 0x15c], 1
        mov    byte ptr [esi + 0x171], 0
        mov    edi, dword ptr [esi + 0x62]
        mov    eax, esi
        mov    byte ptr [esi + 0x170], 0
        mov    byte ptr [esi + 0x172], 0
        mov    word ptr [esi + 0x173], 0
        sub    eax, edi
        cmp    eax, -0x62
        je     loc_1_1EF2
loc_1_1ED0:
        cmp    byte ptr [edi + 0x1c], 0
        je     loc_1_1EE7
        mov    byte ptr [edi + 0x1c], 0
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   0
        push   eax
        call   sub_1_3E7E
loc_1_1EE7:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x62
        jne    loc_1_1ED0
loc_1_1EF2:
        push   esi
        call   sub_1_2E48
        jmp    loc_1_20A3
loc_1_1EFD:
        mov    dx, word ptr [ecx + 6]
        mov    word ptr [esi + 0x173], dx
        mov    ah, dl
        mov    al, dh
        mov    word ptr [esi + 0x173], ax
        cmp    ax, 0x24
        jne    loc_1_1F3D
        cmp    byte ptr [esi + 0x36], 1
        jne    loc_1_1F3D
        mov    al, byte ptr [esi + 0x37]
        test   al, al
        jne    loc_1_1F3D
        inc    al
        mov    ecx, dword ptr [esi + 0xae]
        mov    byte ptr [esi + 0x37], al
        mov    byte ptr [ecx + 0x1e4b], 0x20
        jmp    loc_1_20A3
loc_1_1F3D:
        mov    dword ptr [esi + 0x15c], 5
        mov    byte ptr [esi + 0x171], 0
        mov    ax, word ptr [ecx + 6]
        mov    edi, dword ptr [esi + 0x62]
        mov    word ptr [esi + 0x173], ax
        mov    ch, al
        mov    byte ptr [esi + 0x170], 1
        mov    byte ptr [esi + 0x172], 1
        mov    cl, ah
        mov    eax, esi
        mov    word ptr [esi + 0x173], cx
        sub    eax, edi
        cmp    eax, -0x62
        je     loc_1_20A3
loc_1_1F82:
        cmp    byte ptr [edi + 0x1c], 0
        mov    ebx, edi
        je     loc_1_1FCA
        mov    byte ptr [edi + 0x1c], 0
        xor    eax, eax
        mov    ecx, dword ptr [edi + 0x10]
        mov    ax, word ptr [esi + 0x173]
        or     eax, 0xc0011000
        push   eax
        push   0xc0010007
        push   ecx
        call   sub_1_3E7E
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [ebx + 4]
        push   0
        push   0x96
        push   ebx
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [ecx + 4], eax
        call   sub_1_3F4C
        dec    dword ptr [esi + 0x5e]
loc_1_1FCA:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x62
        jne    loc_1_1F82
        jmp    loc_1_20A3
loc_1_1FD8:
        cmp    dword ptr [esi + 0xcf], 0
        je     loc_1_1FFA
        lea    eax, [ebp - 0xb]
        push   eax
        push   esi
        call   sub_1_2D38
        cmp    byte ptr [ebp - 0xb], 0
        je     loc_1_20A3
        jmp    loc_1_209D
loc_1_1FFA:
        cmp    byte ptr [esi + 0xcc], 0
        jne    loc_1_20A3
        mov    eax, dword ptr [esi + 0xb8]
        mov    cl, byte ptr [eax + 2]
        mov    al, byte ptr [eax]
        test   al, al
        je     loc_1_209D
        cmp    al, 8
        je     loc_1_204C
        cmp    al, 7
        je     loc_1_204C
        cmp    al, 6
        je     loc_1_204C
        cmp    al, 0x1e
        je     loc_1_204C
        and    eax, 0xff
        mov    ecx, dword ptr [esi + 0x82]
        push   eax
        push   8
        push   4
        push   3
        push   0xaa00000b
        push   ecx
        call   sub_1_4018
        add    esp, 0x18
        jmp    loc_1_20A3
loc_1_204C:
        mov    eax, dword ptr [esi + 0x92]
        cmp    dword ptr [eax + 0x10], 8
        jne    loc_1_208B
        test   cl, cl
        jne    loc_1_206F
        cmp    dword ptr [eax + 4], 0
        push   esi
        je     loc_1_206A
        call   sub_1_2F88
        jmp    loc_1_206F
loc_1_206A:
        call   sub_1_2F18
loc_1_206F:
        push   0
        mov    eax, dword ptr [esi + 0x92]
        push   0x14
        push   eax
        call   sub_1_3F4C
        mov    dword ptr [esi + 0x92], 0
        jmp    loc_1_209D
loc_1_208B:
        cmp    cl, 1
        sbb    al, al
        neg    al
        push   eax
        push   esi
        call   sub_1_3298
        test   al, al
        je     loc_1_20A3
loc_1_209D:
        push   esi
        call   sub_1_2DD8
loc_1_20A3:
        test   byte ptr [ebp - 0xa], 4
        je     loc_1_21C4
        mov    ecx, dword ptr [esi + 0xbc]
        mov    al, byte ptr [ecx]
        cmp    al, 0xa
        jne    loc_1_2197
        xor    eax, eax
        mov    al, byte ptr [esi + 0xce]
        add    eax, 0x7f
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0x7f
        xor    eax, edx
        sub    eax, edx
        mov    dl, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 8], al
        mov    byte ptr [ebp - 9], dl
        mov    al, byte ptr [ecx + 2]
        test   al, al
        jne    loc_1_20E8
        xor    edi, edi
        jmp    loc_1_2111
loc_1_20E8:
        cmp    al, 0x22
        jne    loc_1_210C
        mov    al, byte ptr [ecx + 6]
        mov    cl, al
        and    al, 0xc
        shr    cl, 6
        shr    al, 2
        cmp    al, cl
        jne    loc_1_2105
        test   cl, cl
        jne    loc_1_2105
        xor    edi, edi
        jmp    loc_1_2111
loc_1_2105:
        mov    edi, 0x10001
        jmp    loc_1_2111
loc_1_210C:
        mov    edi, 0xc0000001
loc_1_2111:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        inc    eax
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0x7f
        xor    eax, edx
        sub    eax, edx
        xor    edx, edx
        mov    dl, al
        mov    byte ptr [ebp - 8], al
        mov    ecx, dword ptr [esi + edx*4 + 0x179]
        test   ecx, ecx
        je     loc_1_216F
        mov    byte ptr [ecx + 0x30], 0
        xor    eax, eax
        test   edi, edi
        mov    al, byte ptr [ecx + 0x31]
        mov    dword ptr [esi + eax*4 + 0x179], 0
        mov    ebx, dword ptr [ecx + 0x28]
        jne    loc_1_2158
        inc    dword ptr [esi + 0xf8]
        jmp    loc_1_215E
loc_1_2158:
        inc    dword ptr [esi + 0x100]
loc_1_215E:
        push   edi
        mov    eax, dword ptr [ecx + 0x2c]
        push   eax
        mov    ecx, dword ptr [ebx + 0x10]
        push   ecx
        call   sub_1_3E8A
        dec    dword ptr [ebx + 0x14]
loc_1_216F:
        mov    al, byte ptr [ebp - 8]
        cmp    al, byte ptr [ebp - 9]
        jne    loc_1_2111
        and    eax, 0xff
        inc    eax
        cdq    
        xor    eax, edx
        push   esi
        sub    eax, edx
        and    eax, 0x7f
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [esi + 0xce], al
        call   sub_1_2DD8
        jmp    loc_1_21B7
loc_1_2197:
        and    eax, 0xff
        mov    ecx, dword ptr [esi + 0x82]
        push   eax
        push   8
        push   4
        push   3
        push   0xaa00000b
        push   ecx
        call   sub_1_4018
        add    esp, 0x18
loc_1_21B7:
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e4b], 1
loc_1_21C4:
        lea    eax, [ebp - 0xa]
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [ebp - 0x14], esi
        push   0                             ; -> loc_1_1A88
        mov    dword ptr [ebp - 0x10], eax
        push   esi
        call   sub_1_3F3A
        test   byte ptr [ebp - 0xa], 0x24
        jne    loc_1_1D28
loc_1_21E6:
        mov    byte ptr [esi + 0x4c], 0
        mov    eax, dword ptr [esi + 0x5e]
        dec    eax
        mov    dword ptr [esi + 0x5e], eax
        jne    loc_1_220C
        cmp    byte ptr [esi + 0x160], 0
        jne    loc_1_2206
        mov    eax, dword ptr [esi + 0x6a]
        sub    eax, esi
        cmp    eax, 0x6a
        je     loc_1_220C
loc_1_2206:
        push   esi
        call   sub_1_35C8
loc_1_220C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_2218:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 0xc]
        push   ecx
        inc    dword ptr [esi + 0x5e]
        push   0                             ; -> loc_1_1AA8
        mov    dword ptr [ebp - 0xc], esi
        push   esi
        mov    dword ptr [ebp - 8], eax
        mov    byte ptr [esi + 0x4d], 1
        call   sub_1_3F3A
        test   byte ptr [ebp - 1], 0x18
        je     loc_1_291C
loc_1_224D:
        test   byte ptr [ebp - 1], 8
        je     loc_1_27CC
        mov    edi, dword ptr [esi + 0xc0]
        xor    eax, eax
        mov    al, byte ptr [edi]
        sub    eax, 0x81
        cmp    eax, 3
        ja     loc_1_2272
        jmp    dword ptr [eax*4]             ; -> obj1:0x294C
loc_1_2272:
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e4b], 2
        jmp    loc_1_27CC
loc_1_2284:
        cmp    byte ptr [esi + 0x175], 0
        je     loc_1_229A
        cmp    byte ptr [esi + 0x176], 0
        je     loc_1_27CC
loc_1_229A:
        mov    bx, word ptr [edi + 6]
        mov    ecx, dword ptr [esi + 0xb2]
        mov    word ptr [esi + 0xd7], bx
        mov    ah, bl
        mov    al, bh
        mov    edx, eax
        mov    ax, word ptr [edi + 0xa]
        and    edx, 0xffff
        mov    word ptr [ebp - 0x24], ax
        lea    ecx, [edx + ecx + 2]
        mov    di, word ptr [ecx + 4]
        mov    eax, dword ptr [ebp - 0x24]
        mov    word ptr [ebp - 0x20], di
        mov    al, ah
        mov    ah, byte ptr [ebp - 0x24]
        mov    edx, eax
        mov    eax, dword ptr [ebp - 0x20]
        and    edx, 0xffff
        mov    al, ah
        mov    ah, byte ptr [ebp - 0x20]
        mov    edi, dword ptr [esi + 0xae]
        mov    dword ptr [ebp - 0x24], edx
        mov    edx, eax
        and    edx, 0xffff
        mov    dword ptr [ebp - 0x20], edx
        lea    edx, [ecx + 6]
        mov    byte ptr [edi + 0x1e4b], 2
        test   byte ptr [edx + 8], 0x80
        je     loc_1_2312
        mov    al, byte ptr [edx + 0xe]
        and    eax, 0x1f
        add    eax, 0xe
        jmp    loc_1_2317
loc_1_2312:
        mov    eax, 0xe
loc_1_2317:
        mov    word ptr [esi + 0xd9], ax
        inc    dword ptr [esi + 0xfc]
        cmp    eax, dword ptr [ebp - 0x20]
        jbe    loc_1_2348
        cmp    dword ptr [ebp - 0x20], 6
        jb     loc_1_236B
        test   edx, edx
        je     loc_1_236B
        push   0
        mov    eax, dword ptr [ebp - 0x20]
        push   0
        push   0
        push   eax
        push   edx
        push   ecx
        mov    ecx, dword ptr [esi + 0x86]
        push   ecx
        jmp    loc_1_2366
loc_1_2348:
        test   edx, edx
        je     loc_1_236B
        mov    edi, dword ptr [ebp - 0x24]
        sub    edi, eax
        push   edi
        mov    edi, dword ptr [ebp - 0x20]
        sub    edi, eax
        push   edi
        lea    edi, [edx + eax]
        push   edi
        push   eax
        push   edx
        mov    eax, dword ptr [esi + 0x86]
        push   ecx
        push   eax
loc_1_2366:
        call   sub_1_39C0
loc_1_236B:
        mov    eax, dword ptr [esi + 0x86]
        push   eax
        call   sub_1_3C10
        cmp    byte ptr [esi + 0xcd], 0
        je     loc_1_23A7
        mov    byte ptr [esi + 0xcd], 0
        mov    byte ptr [esi + 0xdb], 0
        push   ebx
        push   esi
        call   sub_1_3558
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_1_27CC
loc_1_23A7:
        mov    ax, word ptr [esi + 0xd5]
        cmp    ax, 0xffff
        jne    loc_1_23BD
        mov    word ptr [esi + 0xd3], bx
        jmp    loc_1_23D2
loc_1_23BD:
        mov    ch, al
        mov    edx, dword ptr [esi + 0xb2]
        mov    cl, ah
        mov    eax, ecx
        and    eax, 0xffff
        mov    word ptr [eax + edx], bx
loc_1_23D2:
        cmp    byte ptr [esi + 0x178], 0
        mov    word ptr [esi + 0xd5], bx
        jne    loc_1_27CC
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [esi + 0x178], 1
        mov    byte ptr [eax + 0x1e4b], 4
        jmp    loc_1_27CC
loc_1_23FF:
        cmp    byte ptr [esi + 0x175], 0
        je     loc_1_2415
        cmp    byte ptr [esi + 0x176], 0
        je     loc_1_27CC
loc_1_2415:
        xor    eax, eax
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [ebp - 0xc], 0
        mov    ebx, dword ptr [esi + eax*4 + 0x179]
        test   ebx, ebx
        jne    loc_1_2447
        lea    eax, [ebp - 0xd]
        push   eax
        push   esi
        call   sub_1_2D38
        mov    ebx, eax
        test   ebx, ebx
        je     loc_1_27CC
        cmp    byte ptr [ebp - 0xd], 0
        je     loc_1_27CC
loc_1_2447:
        mov    ax, word ptr [edi + 6]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esi + 0xae]
        mov    edi, ecx
        lea    ecx, [ebp - 0x34]
        and    edi, 0xffff
        push   ecx
        add    edi, dword ptr [esi + 0xb2]
        push   0
        mov    byte ptr [eax + 0x1e4b], 2
        push   0
        push   0
        push   ebx
        call   sub_1_3F70
        lea    ecx, [ebp - 0x38]
        mov    edx, dword ptr [ebp - 0x34]
        push   ecx
        push   edi
        push   edx
        push   0
        push   ebx
        call   sub_1_1738
        cmp    byte ptr [esi + 0xcd], 0
        je     loc_1_24B8
        mov    byte ptr [esi + 0xcd], 0
        mov    byte ptr [esi + 0xdb], 1
        push   ebx
        push   esi
        call   sub_1_2EA8
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_1_24DC
loc_1_24B8:
        push   ebx
        push   esi
        call   sub_1_3588
        cmp    byte ptr [esi + 0x178], 0
        jne    loc_1_24DC
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [esi + 0x178], 1
        mov    byte ptr [eax + 0x1e4b], 4
loc_1_24DC:
        cmp    byte ptr [ebp - 0xc], 0
        je     loc_1_27CC
        cmp    byte ptr [esi + 0xcc], 0
        je     loc_1_27CC
        mov    byte ptr [esi + 0xcc], 0
        push   esi
        call   sub_1_2DD8
        jmp    loc_1_27CC
loc_1_2505:
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e4b], 2
        mov    ecx, dword ptr [esi + 0xc0]
        mov    ax, word ptr [ecx + 6]
        mov    ch, al
        test   ah, 0x40
        mov    cl, ah
        je     loc_1_27CC
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        lea    ecx, [ebp - 0x1c]
        push   eax
        push   0
        push   0x14
        push   ecx
        call   sub_1_3F46
        test   eax, eax
        je     loc_1_255F
        push   5
        mov    eax, dword ptr [esi + 0x82]
        push   4
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_4018
        add    esp, 0x14
        jmp    loc_1_27CC
loc_1_255F:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x10], 8
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esi + 0x8a]
        test   eax, eax
        jne    loc_1_2597
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [esi + 0x8e], eax
        mov    dword ptr [esi + 0x8a], eax
        jmp    loc_1_25A5
loc_1_2597:
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    dword ptr [esi + 0x8a], ecx
loc_1_25A5:
        cmp    byte ptr [esi + 0xcc], 0
        je     loc_1_27CC
        mov    byte ptr [esi + 0xcc], 0
        push   esi
        call   sub_1_2DD8
        jmp    loc_1_27CC
loc_1_25C4:
        mov    dword ptr [ebp - 0x14], 0
        mov    ax, word ptr [edi + 6]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e4b], 2
        mov    di, cx
        and    di, 0x80
        mov    word ptr [ebp - 0xc], cx
        je     loc_1_2684
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        lea    ecx, [ebp - 0x18]
        push   eax
        push   0
        push   0x14
        push   ecx
        call   sub_1_3F46
        test   eax, eax
        je     loc_1_2628
        push   5
        mov    eax, dword ptr [esi + 0x82]
        push   4
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_4018
        add    esp, 0x14
        jmp    loc_1_27CC
loc_1_2628:
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x10], 8
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esi + 0x8a]
        test   eax, eax
        jne    loc_1_2660
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [esi + 0x8e], eax
        mov    dword ptr [esi + 0x8a], eax
        jmp    loc_1_266E
loc_1_2660:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [esi + 0x8a], ecx
loc_1_266E:
        cmp    byte ptr [esi + 0xcc], 0
        je     loc_1_2684
        mov    byte ptr [esi + 0xcc], 0
        push   esi
        call   sub_1_2DD8
loc_1_2684:
        mov    ax, word ptr [ebp - 0xc]
        and    ax, 0x20
        mov    word ptr [ebp - 0x2c], ax
        je     loc_1_2696
        or     dword ptr [ebp - 0x14], 0x40
loc_1_2696:
        test   byte ptr [ebp - 0xc], 0x40
        je     loc_1_26A3
        or     dword ptr [ebp - 0x14], 0x80
loc_1_26A3:
        test   di, di
        je     loc_1_26AF
        or     dword ptr [ebp - 0x14], 0x100
loc_1_26AF:
        test   byte ptr [ebp - 0xb], 1
        je     loc_1_26BC
        or     dword ptr [ebp - 0x14], 0x200
loc_1_26BC:
        mov    eax, 0x400
        test   word ptr [ebp - 0xc], ax
        je     loc_1_26CA
        or     dword ptr [ebp - 0x14], eax
loc_1_26CA:
        mov    ax, word ptr [ebp - 0xc]
        and    ax, 0x800
        mov    word ptr [ebp - 0x30], ax
        je     loc_1_26DF
        or     dword ptr [ebp - 0x14], 0x800
loc_1_26DF:
        mov    eax, 0x1000
        test   word ptr [ebp - 0xc], ax
        je     loc_1_26ED
        or     dword ptr [ebp - 0x14], eax
loc_1_26ED:
        mov    eax, 0x2000
        test   word ptr [ebp - 0xc], ax
        je     loc_1_26FB
        or     dword ptr [ebp - 0x14], eax
loc_1_26FB:
        mov    ax, word ptr [ebp - 0xc]
        and    ax, 0x4000
        mov    word ptr [ebp - 0x28], ax
        je     loc_1_2710
        or     dword ptr [ebp - 0x14], 0x4000
loc_1_2710:
        mov    eax, 0x8000
        test   word ptr [ebp - 0xc], ax
        je     loc_1_271E
        or     dword ptr [ebp - 0x14], eax
loc_1_271E:
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_2768
        mov    edi, dword ptr [esi + 0x62]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x62
        je     loc_1_275F
loc_1_2730:
        lea    ebx, [edi + 0x14]
        push   4
        lea    eax, [ebp - 0x14]
        inc    dword ptr [ebx]
        push   eax
        mov    ecx, dword ptr [edi + 0x10]
        push   0x40010006
        push   ecx
        call   sub_1_3EAE
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_3EB4
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        dec    dword ptr [ebx]
        cmp    eax, -0x62
        jne    loc_1_2730
loc_1_275F:
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [esi + 0x158], eax
loc_1_2768:
        mov    ax, word ptr [ebp - 0xc]
        and    ah, 0x50
        cmp    ah, 0x50
        jne    loc_1_2797
        mov    byte ptr [esi + 0x175], 1
        push   0xc0010008
        push   esi
        call   sub_1_3748
        cmp    word ptr [ebp - 0x30], 0x800
        jne    loc_1_27CC
        mov    byte ptr [esi + 0x177], 1
        jmp    loc_1_27CC
loc_1_2797:
        cmp    word ptr [ebp - 0x2c], 0
        je     loc_1_27CC
        cmp    word ptr [ebp - 0x28], 0
        jne    loc_1_27CC
        cmp    byte ptr [esi + 0x175], 0
        je     loc_1_27CC
        cmp    byte ptr [esi + 0x176], 0
        jne    loc_1_27CC
        mov    byte ptr [esi + 0x176], 1
        push   0
        push   esi
        call   sub_1_11B0
        push   esi
        call   sub_1_35C8
loc_1_27CC:
        test   byte ptr [ebp - 1], 0x10
        je     loc_1_28FA
        mov    byte ptr [ebp - 0xc], 0
        xor    bl, bl
        cmp    byte ptr [esi + 0x175], bl
        je     loc_1_27F0
        cmp    byte ptr [esi + 0x176], bl
        je     loc_1_291C
loc_1_27F0:
        mov    cx, word ptr [esi + 0xd3]
        cmp    cx, 0xffff
        je     loc_1_2802
        mov    byte ptr [ebp - 0xc], 1
loc_1_2802:
        mov    edx, dword ptr [esi + 0xf0]
        test   edx, edx
        je     loc_1_280E
        mov    bl, 1
loc_1_280E:
        cmp    byte ptr [ebp - 0xc], 0
        je     loc_1_289D
        test   bl, bl
        je     loc_1_2825
        cmp    byte ptr [esi + 0xdb], 0
        je     loc_1_289D
loc_1_2825:
        cmp    word ptr [esi + 0xd5], cx
        jne    loc_1_2842
        mov    word ptr [esi + 0xd3], 0xffff
        mov    word ptr [esi + 0xd5], 0xffff
        jmp    loc_1_285F
loc_1_2842:
        mov    ah, cl
        mov    edx, dword ptr [esi + 0xb2]
        mov    al, ch
        mov    edi, eax
        and    edi, 0xffff
        mov    ax, word ptr [edi + edx]
        mov    word ptr [esi + 0xd3], ax
loc_1_285F:
        mov    byte ptr [esi + 0xcd], 0
        mov    byte ptr [esi + 0xdb], 0
        push   ecx
        push   esi
        call   sub_1_3558
        test   bl, bl
        je     loc_1_2887
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e4b], 0x14
        jmp    loc_1_28FA
loc_1_2887:
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [esi + 0x178], 0
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_1_28FA
loc_1_289D:
        test   bl, bl
        je     loc_1_28F3
        mov    eax, dword ptr [edx + 0x24]
        push   edx
        push   esi
        mov    dword ptr [esi + 0xf0], eax
        mov    byte ptr [esi + 0xcd], 0
        mov    byte ptr [esi + 0xdb], 1
        call   sub_1_2EA8
        cmp    byte ptr [ebp - 0xc], 0
        jne    loc_1_28E4
        cmp    dword ptr [esi + 0xf0], 0
        jne    loc_1_28E4
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [esi + 0x178], 0
        mov    byte ptr [eax + 0x1e4b], 0x10
        jmp    loc_1_28FA
loc_1_28E4:
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e4b], 0x14
        jmp    loc_1_28FA
loc_1_28F3:
        mov    byte ptr [esi + 0xcd], 1
loc_1_28FA:
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 0x40]
        push   ecx
        mov    dword ptr [ebp - 0x40], esi
        push   0                             ; -> loc_1_1AA8
        mov    dword ptr [ebp - 0x3c], eax
        push   esi
        call   sub_1_3F3A
        test   byte ptr [ebp - 1], 0x18
        jne    loc_1_224D
loc_1_291C:
        mov    byte ptr [esi + 0x4d], 0
        mov    eax, dword ptr [esi + 0x5e]
        dec    eax
        mov    dword ptr [esi + 0x5e], eax
        jne    loc_1_2942
        cmp    byte ptr [esi + 0x160], 0
        jne    loc_1_293C
        mov    eax, dword ptr [esi + 0x6a]
        sub    eax, esi
        cmp    eax, 0x6a
        je     loc_1_2942
loc_1_293C:
        push   esi
        call   sub_1_35C8
loc_1_2942:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
        ; ---- dispatch table (4 entries) ----
        dd  loc_1_2284
        dd  loc_1_23FF
        dd  loc_1_2505
        dd  loc_1_25C4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2968:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x176], 0
        jne    loc_1_29F2
        mov    edi, dword ptr [esi + 0x62]
        mov    eax, esi
        mov    dword ptr [esi + 0x15c], 6
        sub    eax, edi
        cmp    eax, -0x62
        je     loc_1_29CE
loc_1_2993:
        lea    ebx, [edi + 0x14]
        inc    dword ptr [ebx]
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_29A6
        push   4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        jmp    loc_1_29AA
loc_1_29A6:
        push   0
        push   0
loc_1_29AA:
        push   0x40010007
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_3EAE
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3EB4
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        dec    dword ptr [ebx]
        cmp    eax, -0x62
        jne    loc_1_2993
loc_1_29CE:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [esi + 0x82]
        push   ecx
        push   7
        push   3
        push   4
        push   0xaa000003
        push   edx
        call   sub_1_4018
        add    esp, 0x1c
        jmp    loc_1_2A00
loc_1_29F2:
        mov    byte ptr [esi + 0x177], 1
        mov    byte ptr [esi + 0x176], 0
loc_1_2A00:
        mov    dword ptr [esi + 0x161], 4
        push   esi
        call   0
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 0x16b]
        mov    byte ptr [esi + 0x171], cl
        mov    byte ptr [esi + 0x160], cl
        mov    byte ptr [esi + 0x169], cl
        mov    byte ptr [esi + 0x16a], cl
        mov    byte ptr [esi + 0x172], 1
        test   eax, eax
        je     loc_1_2A4C
        push   0xc0000001
        dec    dword ptr [eax + 0x14]
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        call   sub_1_3E96
loc_1_2A4C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h
sub_1_2A58:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x161]
        dec    eax
        cmp    eax, 3
        ja     loc_1_2D13
        jmp    dword ptr [eax*4]             ; -> obj1:0x2D1C
loc_1_2A7B:
        mov    eax, dword ptr [esi + 0xae]
        mov    cl, byte ptr [eax + 0x1e02]
        mov    al, byte ptr [eax + 0x1e03]
        and    ecx, 0xffff00ff
        and    eax, 0xffff00ff
        shl    cx, 8
        mov    ebx, dword ptr [esi + 0xb2]
        add    ax, cx
        mov    word ptr [esi + 0xb6], ax
        mov    edx, eax
        and    edx, 0xffff
        add    ebx, edx
        mov    ax, word ptr [ebx + 6]
        test   ax, ax
        je     loc_1_2AD8
        push   1
        and    eax, 0xffff
        push   eax
        push   0
        push   esi
        call   sub_1_2968
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2AD8:
        mov    edi, ebx
        xor    eax, eax
        mov    byte ptr [esi + 0x37], 0
        mov    ecx, 0xf
        rep stosd dword ptr es:[edi], eax
        push   6
        mov    byte ptr [ebx], 3
        cmp    byte ptr [esi + 0x2b], 1
        sbb    eax, eax
        lea    ecx, [esi + 0x4e]
        and    ax, 0x1000
        push   ecx
        mov    al, 1
        lea    edx, [ebx + 0xa]
        mov    word ptr [ebx + 8], ax
        push   edx
        call   sub_1_402C
        mov    ax, word ptr [esi + 0xe4]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x18], cx
        mov    ax, word ptr [esi + 0xe6]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0x1a], cx
        mov    ax, word ptr [esi + 0xde]
        mov    ch, al
        mov    cl, ah
        pop    edi
        mov    word ptr [ebx + 0x1c], cx
        mov    al, byte ptr [esi + 0xdc]
        mov    byte ptr [ebx + 0x1e], al
        mov    dword ptr [esi + 0x161], 2
        mov    edx, dword ptr [esi + 0xae]
        pop    esi
        pop    ebx
        mov    byte ptr [edx + 0x1e4b], 0x28
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2B5F:
        mov    eax, dword ptr [esi + 0xb2]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0xb6]
        lea    edi, [ecx + eax]
        cmp    byte ptr [edi + 2], 0
        je     loc_1_2BB3
        mov    cx, word ptr [edi + 6]
        mov    word ptr [esi + 0x173], cx
        mov    ah, cl
        mov    al, ch
        push   2
        mov    word ptr [esi + 0x173], ax
        and    eax, 0xffff
        mov    ecx, eax
        push   eax
        or     ecx, 0xc0011000
        mov    dword ptr [ebp - 4], ecx
        lea    ecx, [ebp - 4]
        push   ecx
        push   esi
        call   sub_1_2968
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2BB3:
        mov    cx, word ptr [edi + 0xa]
        mov    dh, cl
        mov    dl, ch
        mov    ecx, edx
        and    ecx, 0xffff
        add    ecx, eax
        mov    dword ptr [esi + 0xb8], ecx
        mov    cx, word ptr [edi + 0xe]
        mov    dh, cl
        mov    dl, ch
        mov    ecx, edx
        and    ecx, 0xffff
        add    ecx, eax
        mov    dword ptr [esi + 0xbc], ecx
        mov    cx, word ptr [edi + 0xc]
        mov    dh, cl
        mov    dl, ch
        mov    ecx, edx
        and    ecx, 0xffff
        add    ecx, eax
        mov    dword ptr [esi + 0xc0], ecx
        mov    cx, word ptr [edi + 8]
        mov    dword ptr [esi + 0xa2], 0
        mov    dh, cl
        push   esi
        mov    dl, ch
        mov    ecx, edx
        and    ecx, 0xffff
        add    ecx, eax
        mov    dword ptr [esi + 0xc4], ecx
        call   sub_1_3438
        test   eax, eax
        jne    loc_1_2C60
        push   esi
        call   sub_1_34E8
        test   eax, eax
        jne    loc_1_2C40
        push   esi
        call   sub_1_1110
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2C40:
        mov    eax, dword ptr [esi + 0xae]
        pop    edi
        mov    dword ptr [esi + 0x161], 4
        mov    byte ptr [eax + 0x1e4b], 0x28
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2C60:
        mov    eax, dword ptr [esi + 0xae]
        pop    edi
        mov    dword ptr [esi + 0x161], 3
        mov    byte ptr [eax + 0x1e4b], 0x28
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2C80:
        mov    eax, dword ptr [esi + 0xb8]
        mov    al, byte ptr [eax + 2]
        test   al, al
        jne    loc_1_2CC6
        push   esi
        call   sub_1_34E8
        test   eax, eax
        jne    loc_1_2CA6
        push   esi
        call   sub_1_1110
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2CA6:
        mov    eax, dword ptr [esi + 0xae]
        pop    edi
        mov    dword ptr [esi + 0x161], 4
        mov    byte ptr [eax + 0x1e4b], 0x28
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2CC6:
        cmp    al, 0xfe
        je     loc_1_2D13
        push   3
        and    eax, 0xff
        push   eax
        push   0
        push   esi
        call   sub_1_2968
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2CE3:
        mov    eax, dword ptr [esi + 0xb8]
        mov    al, byte ptr [eax + 2]
        test   al, al
        jne    loc_1_2CFF
        push   esi
        call   sub_1_1110
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2CFF:
        cmp    al, 0xfe
        je     loc_1_2D13
        push   4
        and    eax, 0xff
        push   eax
        push   0
        push   esi
        call   sub_1_2968
loc_1_2D13:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        ; ---- dispatch table (4 entries) ----
        dd  loc_1_2A7B
        dd  loc_1_2B5F
        dd  loc_1_2C80
        dd  loc_1_2CE3
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D38:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0xb8]
        mov    dl, byte ptr [esi + 2]
        cmp    dl, 0xfe
        jne    loc_1_2D5C
        mov    ecx, dword ptr [ebp + 0xc]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        mov    byte ptr [ecx], 0
        ret    8
loc_1_2D5C:
        mov    eax, dword ptr [edi + 0xcf]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0xcf], 0
        test   eax, eax
        jne    loc_1_2D7E
        xor    eax, eax
        pop    edi
        pop    esi
        mov    byte ptr [ecx], 0
        pop    ebp
        ret    8
loc_1_2D7E:
        mov    byte ptr [ecx], 1
        cmp    dl, 0xff
        lea    ecx, [eax + 0x24]
        je     loc_1_2DAF
        mov    esi, dword ptr [ecx + 4]
        push   0xc0000001
        inc    dword ptr [edi + 0x100]
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [esi + 0x10]
        push   eax
        push   ecx
        call   sub_1_3E8A
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 0x14]
        pop    esi
        pop    ebp
        ret    8
loc_1_2DAF:
        mov    byte ptr [ecx + 0xc], 1
        mov    dl, byte ptr [esi + 1]
        mov    byte ptr [ecx + 0xd], dl
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x31]
        mov    dword ptr [edi + ecx*4 + 0x179], eax
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2DD8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x8a], 0
        je     loc_1_2E07
        push   esi
        call   sub_1_3058
        cmp    eax, 0x103
        jne    loc_1_2E07
        mov    eax, dword ptr [esi + 0xae]
        pop    esi
        pop    ebp
        mov    byte ptr [eax + 0x1e4b], 0x20
        ret    4
loc_1_2E07:
        mov    eax, dword ptr [esi + 0xe8]
        test   eax, eax
        je     loc_1_2E39
        mov    ecx, dword ptr [eax + 0x24]
        push   eax
        push   esi
        mov    dword ptr [esi + 0xe8], ecx
        mov    dword ptr [esi + 0xcf], eax
        call   sub_1_2E78
        mov    ecx, dword ptr [esi + 0xae]
        pop    esi
        pop    ebp
        mov    byte ptr [ecx + 0x1e4b], 0x20
        ret    4
loc_1_2E39:
        mov    byte ptr [esi + 0xcc], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_2E48:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xb8]
        mov    byte ptr [eax + 0xcc], 0
        pop    ebp
        mov    byte ptr [ecx], 0
        mov    byte ptr [ecx + 2], 0xfe
        mov    edx, dword ptr [eax + 0xae]
        mov    byte ptr [edx + 0x1e4b], 0x20
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_2E78:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0xb8]
        mov    byte ptr [eax], 0xa
        mov    byte ptr [eax + 1], cl
        mov    byte ptr [eax + 2], 0xfe
        mov    word ptr [eax + 4], cx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2EA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_3F70
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xc4]
        mov    byte ptr [eax], 0xa
        mov    dl, byte ptr [esi + 0x31]
        pop    esi
        mov    byte ptr [eax + 1], dl
        mov    byte ptr [eax + 2], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, ch
        mov    ch, byte ptr [ebp - 4]
        mov    word ptr [eax + 6], cx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2EF8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 0xb8]
        pop    ebp
        mov    byte ptr [eax], 0x1e
        mov    word ptr [eax + 4], 0
        mov    byte ptr [eax + 2], 0x80
        ret    4
        db  08Bh, 0FFh
sub_1_2F18:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ecx + 0xb8]
        mov    dx, word ptr [eax + 6]
        mov    bh, dl
        mov    bl, dh
        mov    edx, ebx
        and    edx, 0xffff
        add    edx, eax
        mov    ax, word ptr [edx]
        mov    bh, al
        mov    bl, ah
        mov    eax, ebx
        and    eax, 0xffff
        add    dword ptr [ecx + 0xf8], eax
        mov    ax, word ptr [edx + 2]
        mov    bh, al
        mov    bl, ah
        mov    eax, ebx
        xor    ebx, ebx
        and    eax, 0xffff
        add    dword ptr [ecx + 0xfc], eax
        mov    bl, byte ptr [edx + 4]
        add    dword ptr [ecx + 0x100], ebx
        pop    ebx
        mov    dl, byte ptr [edx + 5]
        pop    ebp
        and    edx, 0xff
        add    dword ptr [ecx + 0x104], edx
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2F88:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [ecx + 0xb8]
        mov    al, byte ptr [edx + 6]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x108], eax
        mov    al, byte ptr [edx + 7]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x10c], eax
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x110], eax
        mov    al, byte ptr [edx + 9]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x114], eax
        mov    al, byte ptr [edx + 0xa]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x118], eax
        mov    al, byte ptr [edx + 0xc]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x11c], eax
        mov    al, byte ptr [edx + 0xd]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x120], eax
        mov    al, byte ptr [edx + 0xe]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x124], eax
        mov    al, byte ptr [edx + 0xf]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x128], eax
        mov    dl, byte ptr [edx + 0x10]
        mov    byte ptr [ebp - 4], dl
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 0x12c], eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_3038:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xb8]
        pop    ebp
        mov    byte ptr [ecx], 8
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3058:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x8a], 0
        je     loc_1_3227
        mov    edi, dword ptr [ebp - 4]
loc_1_3074:
        mov    ebx, dword ptr [esi + 0x8a]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0x8a], eax
        test   eax, eax
        jne    loc_1_3090
        mov    dword ptr [esi + 0x8e], 0
loc_1_3090:
        mov    ecx, dword ptr [ebx + 0x10]
        cmp    ecx, 8
        jne    loc_1_30C2
        cmp    dword ptr [ebx + 4], 0
        push   esi
        je     loc_1_30A6
        call   sub_1_3038
        jmp    loc_1_30AB
loc_1_30A6:
        call   sub_1_2EF8
loc_1_30AB:
        mov    eax, dword ptr [esi + 0xae]
        mov    edi, 0x103
        mov    byte ptr [eax + 0x1e4b], 0x20
        jmp    loc_1_3195
loc_1_30C2:
        lea    eax, [ecx - 1]
        cmp    eax, 9
        ja     loc_1_30D9
        xor    edx, edx
        mov    dl, byte ptr [eax]            ; -> obj1:0x3260
        jmp    dword ptr [edx*4]             ; -> obj1:0x3248
loc_1_30D9:
        push   ecx
        mov    eax, dword ptr [esi + 0x82]
        push   1
        push   9
        push   3
        push   0xaa000006
        push   eax
        call   sub_1_4018
        add    esp, 0x18
        jmp    loc_1_3195
loc_1_30F9:
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1010e
        jne    loc_1_3120
        mov    eax, dword ptr [esi + 0x96]
        push   esi
        mov    dword ptr [esi + 0x9a], eax
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0x96], ecx
        call   sub_1_3438
        jmp    loc_1_3193
loc_1_3120:
        cmp    eax, 0x2010103
        mov    eax, dword ptr [ebx + 8]
        jne    loc_1_3187
        push   esi
        mov    dword ptr [esi + 0x9e], eax
        call   sub_1_3438
        jmp    loc_1_3193
loc_1_3138:
        mov    edi, 0x103
        push   esi
        call   sub_1_3038
        mov    byte ptr [ebx + 4], 1
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e4b], 0x20
        jmp    loc_1_3195
loc_1_3156:
        mov    eax, dword ptr [esi + 0x96]
        push   esi
        mov    dword ptr [esi + 0x9a], eax
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0x96], ecx
        call   sub_1_3438
        jmp    loc_1_3193
loc_1_3173:
        mov    eax, dword ptr [ebx + 8]
        push   esi
        mov    dword ptr [esi + 0x9e], eax
        call   sub_1_3438
        jmp    loc_1_3193
loc_1_3184:
        mov    eax, dword ptr [ebx + 8]
loc_1_3187:
        push   esi
        mov    dword ptr [esi + 0xa6], eax
        call   sub_1_34E8
loc_1_3193:
        mov    edi, eax
loc_1_3195:
        cmp    edi, 0x103
        je     loc_1_3232
        test   edi, edi
        jne    loc_1_31FF
        mov    ecx, dword ptr [ebx + 0x10]
        lea    eax, [ecx - 1]
        cmp    eax, 9
        ja     loc_1_31BF
        xor    edx, edx
        mov    dl, byte ptr [eax]            ; -> obj1:0x3280
        jmp    dword ptr [edx*4]             ; -> obj1:0x326C
loc_1_31BF:
        push   ecx
        push   9
        jmp    loc_1_3202
loc_1_31C4:
        mov    eax, dword ptr [ebx + 4]
        push   0
        push   ebx
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        call   sub_1_3E9C
        mov    ecx, dword ptr [ebp - 4]
        dec    dword ptr [ecx + 0x14]
        jmp    loc_1_321A
loc_1_31DE:
        push   0
        mov    eax, dword ptr [esi + 0x7e]
        push   ebx
        push   eax
        call   sub_1_3EBA
        dec    dword ptr [esi + 0x5e]
        jmp    loc_1_321A
loc_1_31EF:
        mov    eax, dword ptr [ebx + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_321A
loc_1_31F7:
        mov    eax, dword ptr [ebx + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_321A
loc_1_31FF:
        push   edi
        push   0xa
loc_1_3202:
        push   5
        mov    eax, dword ptr [esi + 0x82]
        push   3
        push   0xaa000006
        push   eax
        call   sub_1_4018
        add    esp, 0x18
loc_1_321A:
        cmp    dword ptr [esi + 0x8a], 0
        jne    loc_1_3074
loc_1_3227:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3232:
        mov    eax, 0x103
        pop    edi
        mov    dword ptr [esi + 0x92], ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_30F9
        dd  loc_1_3138
        dd  loc_1_3156
        dd  loc_1_3173
        dd  loc_1_3184
        dd  loc_1_30D9
        db  000h, 001h, 005h, 002h, 005h, 005h, 005h, 005h, 003h, 004h, 08Bh, 0FFh
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_31C4
        dd  loc_1_31DE
        dd  loc_1_31EF
        dd  loc_1_31F7
        dd  loc_1_31BF
        db  000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3298:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x92]
        mov    eax, dword ptr [edi + 0x10]
        dec    eax
        cmp    eax, 9
        ja     loc_1_336E
        xor    ecx, ecx
        mov    cl, byte ptr [eax]            ; -> obj1:0x3404
        jmp    dword ptr [ecx*4]             ; -> obj1:0x33F0
loc_1_32C3:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_32FC
        cmp    dword ptr [edi + 0x14], 0x2010104
        jne    loc_1_32DE
        mov    eax, dword ptr [esi + 0xa6]
        mov    dword ptr [esi + 0xaa], eax
loc_1_32DE:
        mov    dword ptr [esi + 0x92], 0
        mov    ebx, dword ptr [edi + 4]
        push   0
        push   edi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_3E9C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_336E
loc_1_32FC:
        mov    dword ptr [esi + 0x92], 0
        mov    ebx, dword ptr [edi + 4]
        push   0xc0000001
        push   edi
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_3E9C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_336E
loc_1_331D:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_334A
        push   esi
        call   sub_1_2F88
        push   edi
        push   esi
        call   sub_1_0C30
        mov    dword ptr [esi + 0x92], 0
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0x7e]
        push   eax
        call   sub_1_3EBA
        dec    dword ptr [esi + 0x5e]
        jmp    loc_1_336E
loc_1_334A:
        push   0xc0000001
        mov    eax, dword ptr [esi + 0x7e]
        mov    dword ptr [esi + 0x92], 0
        push   edi
        push   eax
        call   sub_1_3EBA
        dec    dword ptr [esi + 0x5e]
        jmp    loc_1_336E
loc_1_3368:
        mov    eax, dword ptr [edi + 4]
        dec    dword ptr [eax + 0x14]
loc_1_336E:
        cmp    byte ptr [ebp + 0xc], 0
        jne    loc_1_33E4
        mov    ecx, dword ptr [edi + 0x10]
        lea    eax, [ecx - 1]
        cmp    eax, 9
        ja     loc_1_338E
        xor    edx, edx
        mov    dl, byte ptr [eax]            ; -> obj1:0x3424
        jmp    dword ptr [edx*4]             ; -> obj1:0x3410
loc_1_338E:
        push   ecx
        mov    eax, dword ptr [esi + 0x82]
        push   9
        push   6
        push   3
        push   0xaa000006
        push   eax
        call   sub_1_4018
        add    esp, 0x18
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_33B2:
        cmp    dword ptr [edi + 0x14], 0x1010e
        jne    loc_1_33DA
        mov    eax, dword ptr [esi + 0x9a]
        pop    edi
        mov    dword ptr [esi + 0xa2], 0
        mov    dword ptr [esi + 0x96], eax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_33DA:
        mov    dword ptr [esi + 0x9e], 0
loc_1_33E4:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_32C3
        dd  loc_1_331D
        dd  loc_1_3368
        dd  loc_1_3368
        dd  loc_1_336E
        db  000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h, 08Bh, 0FFh
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_33B2
        dd  loc_1_33E4
        dd  loc_1_33E4
        dd  loc_1_33E4
        dd  loc_1_338E
        db  000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h
sub_1_3438:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x96]
        test   ah, 0x20
        je     loc_1_3452
        mov    esi, 0x7fffffff
        jmp    loc_1_3462
loc_1_3452:
        test   ah, 0x40
        mov    esi, 0
        je     loc_1_3462
        mov    esi, dword ptr [edi + 0x9e]
loc_1_3462:
        cmp    dword ptr [edi + 0xa2], esi
        jne    loc_1_3472
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3472:
        push   esi
        push   edi
        call   sub_1_3498
        mov    eax, 0x103
        mov    dword ptr [edi + 0xa2], esi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3498:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [eax + 0xb8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, ecx
        mov    byte ptr [edx], 7
        mov    byte ptr [edx + 2], 0xfe
        shr    eax, 0x18
        mov    byte ptr [ebp - 4], al
        mov    eax, ecx
        shr    eax, 0x10
        mov    byte ptr [ebp - 3], al
        xor    eax, eax
        mov    byte ptr [ebp - 2], ch
        mov    byte ptr [ebp - 1], cl
loc_1_34CA:
        mov    cl, byte ptr [ebp + eax - 4]
        inc    eax
        cmp    eax, 4
        mov    byte ptr [edx + eax + 5], cl
        jb     loc_1_34CA
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_34E8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xa6]
        push   ecx
        push   eax
        call   sub_1_3508
        mov    eax, 0x103
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3508:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [eax + 0xb8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, ecx
        mov    byte ptr [edx], 6
        mov    byte ptr [edx + 2], 0xfe
        shr    eax, 0x18
        mov    byte ptr [ebp - 4], al
        mov    eax, ecx
        shr    eax, 0x10
        mov    byte ptr [ebp - 3], al
        xor    eax, eax
        mov    byte ptr [ebp - 2], ch
        mov    byte ptr [ebp - 1], cl
loc_1_353A:
        mov    cl, byte ptr [ebp + eax - 4]
        inc    eax
        cmp    eax, 4
        mov    byte ptr [edx + eax + 5], cl
        jb     loc_1_353A
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3558:
        push   ebp
        xor    ecx, ecx
        mov    dx, word ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0xc4]
        mov    byte ptr [eax], 0x81
        mov    byte ptr [eax + 2], cl
        mov    word ptr [eax + 4], cx
        mov    word ptr [eax + 6], dx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3588:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        lea    edx, [ecx + 0x24]
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0xf0]
        test   esi, esi
        jne    loc_1_35A8
        mov    dword ptr [eax + 0xf0], ecx
        jmp    loc_1_35AB
loc_1_35A8:
        mov    dword ptr [esi + 0x24], ecx
loc_1_35AB:
        pop    esi
        mov    dword ptr [eax + 0xf4], ecx
        pop    ebp
        mov    dword ptr [edx], 0
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_35C8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x5e]
        cmp    byte ptr [esi + 0x160], 0
        je     loc_1_3602
        cmp    dword ptr [esi + 0x161], 0
        jne    loc_1_35F5
        mov    dword ptr [esi + 0x161], 1
        push   esi
        call   sub_1_1090
loc_1_35F5:
        cmp    byte ptr [esi + 0x160], 0
        jne    loc_1_36E8
loc_1_3602:
        mov    edi, dword ptr [esi + 0x72]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0x72
        je     loc_1_36E8
        push   0
        inc    dword ptr [edi + 0x14]
        mov    eax, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [esi + 0x86]
        push   eax
        push   ecx
        call   sub_2_0FF0
loc_1_3627:
        test   eax, eax
        jne    loc_1_3684
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        cmp    dword ptr [edi + 0x14], 2
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], edx
        jne    loc_1_366B
        mov    ecx, dword ptr [esi + 0x165]
        cmp    dword ptr [edi + 0x18], ecx
        jne    loc_1_3651
        push   esi
        call   sub_1_02C0
loc_1_3651:
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3E84
        push   0
        push   0x96
        push   edi
        call   sub_1_3F4C
        jmp    loc_1_36E8
loc_1_366B:
        lea    ecx, [esi + 0x6a]
        mov    dword ptr [edi], ecx
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [edi + 4], edx
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [edx], edi
        mov    dword ptr [esi + 0x6e], edi
        sub    dword ptr [edi + 0x14], 2
        jmp    loc_1_36E8
loc_1_3684:
        cmp    eax, 0x103
        jne    loc_1_36CD
        cmp    byte ptr [esi + 0xcc], 0
        je     loc_1_36A1
        mov    byte ptr [esi + 0xcc], 0
        push   esi
        call   sub_1_2DD8
loc_1_36A1:
        mov    byte ptr [edi + 0x1d], 1
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
        lea    ecx, [esi + 0x6a]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edi], ecx
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [edi + 4], edx
        mov    edx, dword ptr [esi + 0x6e]
        mov    dword ptr [edx], edi
        mov    dword ptr [esi + 0x6e], edi
        sub    dword ptr [edi + 0x14], 2
        jmp    loc_1_36E8
loc_1_36CD:
        push   eax
        push   0xa
        push   7
        mov    eax, dword ptr [esi + 0x82]
        push   3
        push   0xaa000006
        push   eax
        call   sub_1_4018
        add    esp, 0x18
loc_1_36E8:
        mov    edi, dword ptr [esi + 0x6a]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0x6a
        je     loc_1_3732
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_3732
        mov    eax, dword ptr [esi + 0x165]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_370B
        push   esi
        call   sub_1_02C0
loc_1_370B:
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_3E84
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        push   0
        push   0x96
        push   edi
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 4], edx
        call   sub_1_3F4C
loc_1_3732:
        pop    edi
        dec    dword ptr [esi + 0x5e]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3748:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x8a], 0
        je     loc_1_37D9
loc_1_375D:
        mov    ecx, dword ptr [esi + 0x8a]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x8a], eax
        test   eax, eax
        jne    loc_1_3779
        mov    dword ptr [esi + 0x8e], 0
loc_1_3779:
        mov    eax, dword ptr [ecx + 0x10]
        dec    eax
        cmp    eax, 9
        ja     loc_1_37D0
        xor    edx, edx
        mov    dl, byte ptr [eax]            ; -> obj1:0x3800
        jmp    dword ptr [edx*4]             ; -> obj1:0x37E8
loc_1_3791:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   ecx
        mov    eax, dword ptr [ebx + 0x10]
        push   eax
        call   sub_1_3E9C
        dec    dword ptr [ebx + 0x14]
        jmp    loc_1_37D0
loc_1_37A4:
        push   edi
        mov    eax, dword ptr [esi + 0x7e]
        push   ecx
        push   eax
        call   sub_1_3EBA
        dec    dword ptr [esi + 0x5e]
        jmp    loc_1_37D0
loc_1_37B4:
        push   0
        push   0x14
        push   ecx
        call   sub_1_3F4C
        mov    dword ptr [esi + 0x92], 0
        jmp    loc_1_37D0
loc_1_37CA:
        mov    eax, dword ptr [ecx + 4]
        dec    dword ptr [eax + 0x14]
loc_1_37D0:
        cmp    dword ptr [esi + 0x8a], 0
        jne    loc_1_375D
loc_1_37D9:
        push   edi
        push   esi
        call   sub_1_3818
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_3791
        dd  loc_1_37A4
        dd  loc_1_37CA
        dd  loc_1_37B4
        dd  loc_1_37CA
        dd  loc_1_37D0
        db  000h, 001h, 005h, 002h, 005h, 005h, 005h, 003h, 004h, 004h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3818:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0xcf]
        test   ecx, ecx
        je     loc_1_384C
        mov    dword ptr [esi + 0xcf], 0
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_3E8A
        dec    dword ptr [edi + 0x14]
loc_1_384C:
        cmp    dword ptr [esi + 0xe8], 0
        je     loc_1_3883
loc_1_3855:
        mov    edx, dword ptr [esi + 0xe8]
        lea    eax, [edx + 0x24]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0xe8], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edx
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_3E8A
        dec    dword ptr [edi + 0x14]
        cmp    dword ptr [esi + 0xe8], 0
        jne    loc_1_3855
loc_1_3883:
        mov    dword ptr [ebp - 4], 0x80
        lea    edi, [esi + 0x179]
loc_1_3890:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        je     loc_1_38C1
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x30], 0
        mov    al, byte ptr [ecx + 0x31]
        push   edx
        mov    dword ptr [esi + eax*4 + 0x179], 0
        mov    ebx, dword ptr [ecx + 0x28]
        mov    eax, dword ptr [ecx + 0x2c]
        push   eax
        mov    ecx, dword ptr [ebx + 0x10]
        push   ecx
        call   sub_1_3E8A
        dec    dword ptr [ebx + 0x14]
loc_1_38C1:
        add    edi, 4
        dec    dword ptr [ebp - 4]
        jne    loc_1_3890
        mov    dword ptr [esi + 0xf0], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_38E8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x29], 0
        je     loc_1_3959
        cmp    dword ptr [esi + 0xcf], 0
        jne    loc_1_3910
        cmp    dword ptr [esi + 0xe8], 0
        jne    loc_1_3910
        cmp    dword ptr [esi + 0x8a], 0
        je     loc_1_3959
loc_1_3910:
        mov    byte ptr [esi + 0x172], 1
        mov    byte ptr [esi + 0x29], 0
        push   0
        push   esi
        call   sub_1_3748
        mov    al, byte ptr [esi + 0x2a]
        cmp    al, 0xa
        jae    loc_1_3953
        inc    al
        push   0
        push   0xaa000003
        mov    byte ptr [esi + 0x2a], al
        mov    ecx, dword ptr [esi + 0x82]
        push   ecx
        call   sub_1_4018
        add    esp, 0xc
        push   0
        push   esi
        call   sub_1_11B0
        push   esi
        call   sub_1_35C8
loc_1_3953:
        mov    byte ptr [esi + 0x29], 0
        jmp    loc_1_3978
loc_1_3959:
        cmp    dword ptr [esi + 0xcf], 0
        jne    loc_1_3974
        cmp    dword ptr [esi + 0xe8], 0
        jne    loc_1_3974
        cmp    dword ptr [esi + 0x8a], 0
        je     loc_1_3978
loc_1_3974:
        mov    byte ptr [esi + 0x29], 1
loc_1_3978:
        cmp    byte ptr [esi + 0x177], 0
        je     loc_1_399F
        cmp    byte ptr [esi + 0x176], 0
        jne    loc_1_399F
        mov    byte ptr [esi + 0x176], 1
        push   0
        push   esi
        call   sub_1_11B0
        push   esi
        call   sub_1_35C8
loc_1_399F:
        push   0x7530
        add    esi, 0x1d
        push   esi
        call   sub_1_3F22
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_39C0:
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
        call   sub_1_39F0
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_39F0:
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
        jb     loc_1_3B10
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     loc_1_3AB4
        mov    al, byte ptr [ecx]
        cmp    al, 0xff
        jne    loc_1_3A32
        cmp    byte ptr [ecx + 1], 0xff
        je     loc_1_3A3C
loc_1_3A32:
        cmp    al, 0xc0
        jne    loc_1_3A58
        cmp    byte ptr [ecx + 1], 0
        jne    loc_1_3A58
loc_1_3A3C:
        cmp    byte ptr [ecx + 2], 0xff
        jne    loc_1_3A58
        cmp    byte ptr [ecx + 3], 0xff
        jne    loc_1_3A58
        cmp    byte ptr [ecx + 4], 0xff
        jne    loc_1_3A58
        cmp    byte ptr [ecx + 5], 0xff
        mov    byte ptr [ebp - 2], 1
        je     loc_1_3A5C
loc_1_3A58:
        mov    byte ptr [ebp - 2], 0
loc_1_3A5C:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_3A70
        mov    dword ptr [ebp - 8], 8
        jmp    loc_1_3B1B
        db  08Bh, 0FFh
loc_1_3A70:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 0x1000
        jne    loc_1_3A8C
        mov    dword ptr [ebp - 8], 0x4000
loc_1_3A8C:
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
        jmp    loc_1_3B1B
loc_1_3AB4:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x10], 0x30
        je     loc_1_3B00
        mov    edx, dword ptr [esi + 0x1a]
        lea    eax, [esi + 0x18]
        cmp    dword ptr [ecx + 2], edx
        jne    loc_1_3AD7
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_3ADE
loc_1_3AD7:
        mov    dword ptr [ebp - 0x10], 1
loc_1_3ADE:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_3AF0
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_3B1E
        db  08Dh, 049h, 000h
loc_1_3AF0:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_3B1E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3B00:
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_3B1E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3B10:
        mov    dword ptr [ebp - 8], 0x20
        mov    byte ptr [ebp - 1], 0
loc_1_3B1B:
        mov    esi, dword ptr [ebp + 8]
loc_1_3B1E:
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_3C01
loc_1_3B29:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    loc_1_3B6D
        test   ah, 0x40
        je     loc_1_3B42
        mov    eax, dword ptr [edi + 0xc]
        test   dword ptr [ebp - 0xc], eax
        jne    loc_1_3B6D
loc_1_3B42:
        test   byte ptr [ebp - 7], 0x40
        je     loc_1_3B4D
        test   ch, 0x20
        jne    loc_1_3B6D
loc_1_3B4D:
        test   byte ptr [ebp - 7], 0x10
        je     loc_1_3B59
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_1_3B6D
loc_1_3B59:
        test   cl, 0x10
        je     loc_1_3B64
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_3B6D
loc_1_3B64:
        test   cl, 0x20
        je     loc_1_3BF6
loc_1_3B6D:
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
        call   sub_1_3EA2
        mov    byte ptr [edi + 0x1e], 1
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_3BF6
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_3BAF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_3BAF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_3BC0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_3BC6
        db  08Bh, 0FFh
loc_1_3BC0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_3BC6:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_3BD5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3EA8
loc_1_3BD5:
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
        call   sub_1_3F4C
        mov    edi, ebx
        jmp    loc_1_3BF9
loc_1_3BF6:
        mov    edi, dword ptr [edi + 0x10]
loc_1_3BF9:
        or     edi, edi
        jne    loc_1_3B29
loc_1_3C01:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3C10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3C20
        pop    ebp
        ret    4
sub_1_3C20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_3C9E
loc_1_3C30:
        inc    dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_3C46
        mov    byte ptr [edi + 0x1e], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3EA8
loc_1_3C46:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_3C97
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_3C5C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_3C5C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_3C70
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_3C76
        db  005h, 000h, 000h, 000h, 000h
loc_1_3C70:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_3C76:
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
        call   sub_1_3F4C
        mov    edi, ebx
        jmp    loc_1_3C9A
loc_1_3C97:
        mov    edi, dword ptr [edi + 0x10]
loc_1_3C9A:
        or     edi, edi
        jne    loc_1_3C30
loc_1_3C9E:
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
        je     loc_1_3D02
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_3D02
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_3D02
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3D02:
        mov    esi, dword ptr [eax + 0x10]
        or     esi, esi
        jne    loc_1_3D12
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3D12:
        test   esi, 0x20
        je     loc_1_3D23
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3D23:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     loc_1_3D40
        test   esi, 0x10
        je     loc_1_3D40
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_3D40:
        mov    eax, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [eax]
        and    cl, 0x80
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     loc_1_3DCA
        mov    cl, byte ptr [eax]
        cmp    cl, 0xff
        jne    loc_1_3D5C
        cmp    byte ptr [eax + 1], 0xff
        je     loc_1_3D67
loc_1_3D5C:
        cmp    cl, 0xc0
        jne    loc_1_3D83
        cmp    byte ptr [eax + 1], 0
        jne    loc_1_3D83
loc_1_3D67:
        cmp    byte ptr [eax + 2], 0xff
        jne    loc_1_3D83
        cmp    byte ptr [eax + 3], 0xff
        jne    loc_1_3D83
        cmp    byte ptr [eax + 4], 0xff
        jne    loc_1_3D83
        cmp    byte ptr [eax + 5], 0xff
        mov    byte ptr [ebp - 1], 1
        je     loc_1_3D87
loc_1_3D83:
        mov    byte ptr [ebp - 1], 0
loc_1_3D87:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_3DB0
        test   esi, 8
        je     loc_1_3DA0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_3DA0:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3DB0:
        test   esi, 0x6000
        je     loc_1_3DC1
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3DC1:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3DCA:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 056h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h, 00Ch
        db  000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh
        db  07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh
        db  0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h
        db  0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_3E5A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_3E60:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_3E66:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_3E6C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_3E72:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_3E78:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_3E7E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_3E84:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_3E8A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_3E96:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_3E9C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_3EA2:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_3EA8:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_3EAE:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_3EB4:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
sub_1_3EBA:
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_3EC0:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_3ECC:
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
sub_1_3F22:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_3F28:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_3F2E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_3F34:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_3F3A:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_3F40:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_3F46:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_3F4C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_3F70:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_3F94:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_3FB8:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_3FC4:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_3FCA:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_3FD0:
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
sub_1_4012:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_4018:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
sub_1_401E:
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
sub_1_4024:
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_402C:
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
        db  'IBMTOK4 '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  IBMTOK4_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_40E0:
        dec    edi
        inc    ebx
        dec    ecx
        push   eax
loc_1_40E4:
        xor    byte ptr [ecx], dh
        xor    dword ptr [esi], esi
loc_1_40E8:
        and    byte ptr [eax], dh
        cmp    dword ptr [ecx], edi
loc_1_40EC:
        push   ebx
        push   edx
        inc    ecx
        dec    ebp
loc_1_40F0:
        xor    al, 0x58
        xor    esi, dword ptr [esi]
loc_1_40F4:
        and    byte ptr [eax], bh
        xor    dword ptr [0x546d6249], esi
        outsd  dx, dword ptr [esi]
        imul   esi, dword ptr [eax + eax], 0x49
        bound  ebp, qword ptr [ebp + 0x54]
        outsd  dx, dword ptr [esi]
        imul   esi, dword ptr [eax + eax], 0x49
        bound  ebp, qword ptr [ebp + 0x20]
        push   esp
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x74654e20
        ja     0x418b
        jb     loc_1_4189
        and    byte ptr [ebx + 0x61], al
        jb     loc_1_4187
        and    byte ptr [esi + 0x6f], ah
        jb     loc_1_4148
        push   eax
        inc    ebx
        and    byte ptr [ecx + 0x2f], cl
        dec    edi
        and    byte ptr [edx + 0x75], ah
        jae    loc_1_4161
        add    byte ptr [ecx + 0x62], cl
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + ebp*2 + 0x6b], dl
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [edx + 0x69], dl
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + 0x65], cl
        je     loc_1_41BE
        outsd  dx, dword ptr [esi]
loc_1_4148:
        jb     loc_1_41B5
        and    byte ptr [ebx + 0x61], al
        jb     loc_1_41B3
        and    byte ptr [esi + 0x6f], ah
        jb     loc_1_4174
        dec    ebp
        inc    ebx
        inc    ecx
        and    byte ptr [edx + 0x75], ah
        jae    loc_1_418A
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
loc_1_4161:
        jne    loc_1_41D6
        push   esp
        jns    loc_1_41D6
        add    byte ptr gs:[edx + 0x75], al
        jae    loc_1_41C0
        jns    0x41de
        add    byte ptr gs:[ecx + 0x4f], cl
        inc    edx
        inc    ecx
loc_1_4174:
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_4178:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
loc_1_4187:
        inc    ebp
        push   ebx
loc_1_4189:
        push   ebx
loc_1_418A:
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
        dec    ebp
        inc    ecx
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x41], cl
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
loc_1_41B3:
        pop    edx
        inc    ebp
loc_1_41B5:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x41], al
        push   edx
        dec    esp
        pop    ecx
        push   esp
loc_1_41BE:
        dec    edi
        dec    ebx
loc_1_41C0:
        inc    ebp
        dec    esi
        push   edx
        inc    ebp
        dec    esp
        inc    ebp
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x41], al
        push   edx
        dec    esp
        pop    ecx
        push   esp
        dec    edi
        dec    ebx
        inc    ebp
        dec    esi
loc_1_41D6:
        push   edx
        inc    ebp
        dec    esp
        inc    ebp
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_41E1:
        add    byte ptr [eax], al
loc_1_41E3:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1390  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x58
        mov    word ptr [ebp - 0x14], 8
        mov    dword ptr [ebp - 0x10], 0     ; -> obj1:0x4100
        mov    word ptr [ebp - 0x12], 8
        mov    word ptr [ebp - 0x20], 0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    ecx, [ebp - 0x20]
        push   ecx
        lea    edx, [ebp - 4]
        mov    word ptr [ebp - 0x1e], 2
        mov    word ptr [ebp - 0x1c], 0xe000
        mov    word ptr [ebp - 0x1a], 0xe001
        push   edx
        call   sub_1_3E5A
loc_2_0044:
        mov    ecx, dword ptr [0]            ; -> obj4:0x0000
        lea    edx, [ebp - 0xc]
        push   ecx
        push   0
        push   8
        push   edx
        call   sub_1_3F46
loc_2_0058:
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_2_006A
        mov    eax, 0xc000009a
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_006A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x58], 3
        mov    byte ptr [ebp - 0x57], 0
        mov    dword ptr [ebp - 0x52], 0     ; -> loc_1_0080
        mov    dword ptr [ebp - 0x4e], 0     ; -> loc_1_0300
        mov    dword ptr [ebp - 0x3e], 0     ; -> loc_1_04C0
        mov    dword ptr [ebp - 0x4a], 0     ; -> loc_1_13C8
        mov    dword ptr [ebp - 0x46], 0     ; -> loc_1_1508
        mov    edx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x42], 0     ; -> loc_1_0E90
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_1_12F0
        push   0x36
        lea    ecx, [ebp - 0x58]
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_1_0D90
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_2_06E0
        mov    dword ptr [ebp - 0x2e], 0     ; -> loc_1_1220
        push   ecx
        mov    dword ptr [ebp - 0x2a], edx
        mov    dword ptr [ebp - 0x26], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0xc]
        lea    ecx, [ebp - 8]
        push   edx
        push   eax
        lea    eax, [edx + 4]
        push   eax
        push   ecx
        call   sub_1_3E66
loc_2_00E7:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_0103
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3E60
loc_2_00F8:
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0103:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_2_0110:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [ebp - 0x2c]
        push   esi
        call   sub_2_0330
        push   esi
        call   sub_1_0000
loc_2_012A:
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        cmp    byte ptr [ebp + 0x14], 1
        sbb    eax, eax
        mov    ecx, dword ptr [esi + 0x5a]
        and    eax, 0xfffffffe
        lea    edi, [esi + 0x82]
        add    eax, 3
        mov    dword ptr [ebp - 0x12], ecx
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x22], eax
        lea    eax, [ebp - 0x2c]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x16], 1
        mov    dword ptr [ebp - 0xe], 4
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + 0x7e]
        push   esi
        push   eax
        push   edi
        call   sub_1_3E72
loc_2_0175:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_2_0322
        cmp    byte ptr [ebp + 0x18], 0
        je     loc_2_01AD
        push   0
        mov    eax, dword ptr [edi]
        push   0xaa00000a
        push   eax
        call   sub_1_4018
loc_2_0195:
        add    esp, 0xc
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3E78
loc_2_01A0:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_01AD:
        push   esi
        call   sub_2_0990
        test   al, al
        jne    loc_2_01CC
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3E78
loc_2_01BF:
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_01CC:
        cmp    byte ptr [esi + 0x3b], 0
        je     loc_2_01DF
        mov    eax, dword ptr [esi + 0xae]
        mov    byte ptr [eax + 0x1e18], 0xc0
loc_2_01DF:
        mov    edx, dword ptr [esi + 0x5a]
        xor    al, al
        inc    edx
        push   0xc350
        out    dx, al
        call   sub_1_3F40
loc_2_01F0:
        mov    edx, dword ptr [esi + 0x5a]
        xor    al, al
        add    edx, 2
        mov    ecx, dword ptr [esi + 0x31]
        out    dx, al
        cmp    byte ptr [esi + 0x39], 1
        mov    eax, 2
        adc    eax, -1
        push   eax
        push   0
        push   ecx
        mov    eax, dword ptr [edi]
        push   ecx
        lea    edx, [ebp - 4]
        push   0                             ; -> loc_1_1C38
        push   esi
        push   0                             ; -> loc_1_1B08
        push   eax
        push   esi
        push   edx
        call   sub_1_3F2E
loc_2_0225:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0301
        push   esi
        call   sub_2_0380
        test   eax, eax
        je     loc_2_026A
        push   1
        mov    eax, dword ptr [edi]
        push   0
        push   2
        push   0xaa000004
        push   eax
        call   sub_1_4018
loc_2_024C:
        add    esp, 0x14
        push   esi
        call   sub_1_3F34
loc_2_0255:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3E78
loc_2_025D:
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_026A:
        lea    eax, [esi + 0x86]
        lea    ecx, [esi + 0x7a]
        push   eax
        lea    edx, [esi + 0x4e]
        push   ecx
        push   edx
        push   0                             ; -> loc_1_1080
        push   0                             ; -> loc_1_0F10
        push   0                             ; -> loc_1_1010
        push   0                             ; -> loc_1_0FA0
        call   sub_2_0E20
        test   al, al
        jne    loc_2_02C7
        push   2
        mov    eax, dword ptr [edi]
        push   0
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_4018
loc_2_02A9:
        add    esp, 0x14
        push   esi
        call   sub_1_3F34
loc_2_02B2:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3E78
loc_2_02BA:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_02C7:
        mov    ecx, 0                        ; -> loc_1_38E8
        lea    eax, [esi + 0x1d]
        push   0x7530
        mov    dword ptr [esi + 0x19], ecx
        push   eax
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0x21], ecx
        mov    dword ptr [esi + 0x25], esi
        call   sub_1_3F22
loc_2_02E9:
        push   0                             ; -> loc_1_1290
        mov    ecx, dword ptr [edi]
        push   esi
        push   ecx
        call   sub_1_401E
loc_2_02F7:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0301:
        push   3
        mov    eax, dword ptr [edi]
        push   0
        push   2
        push   0xaa000005
        push   eax
        call   sub_1_4018
loc_2_0314:
        add    esp, 0x14
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3E78
loc_2_031F:
        mov    eax, dword ptr [ebp - 4]
loc_2_0322:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0330:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        lea    eax, [ecx + 0x62]
        mov    dword ptr [ecx + 0x66], eax
        mov    dword ptr [eax], eax
        lea    eax, [ecx + 0x6a]
        mov    dword ptr [ecx + 0x6e], eax
        mov    dword ptr [eax], eax
        mov    word ptr [ecx + 0xe6], 0x100
        lea    eax, [ecx + 0x72]
        mov    dword ptr [ecx + 0x76], eax
        pop    ebp
        mov    dword ptr [eax], eax
        mov    eax, 1
        mov    word ptr [ecx + 0xdc], ax
        mov    byte ptr [ecx + 0x170], al
        mov    byte ptr [ecx + 0x172], al
        mov    byte ptr [ecx + 0x16f], al
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0380:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x32
        push   esi
        call   sub_2_0690
        cmp    byte ptr [esi + 0x2c], 0
        jne    loc_2_03C6
loc_2_039D:
        test   edi, edi
        je     loc_2_03B2
        push   0x186a0
        dec    edi
        call   sub_1_3F40
loc_2_03AC:
        cmp    byte ptr [esi + 0x2c], 0
        je     loc_2_039D
loc_2_03B2:
        cmp    byte ptr [esi + 0x2c], 0
        jne    loc_2_03C6
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_03C6:
        mov    eax, dword ptr [esi + 0xae]
        mov    cl, byte ptr [eax + 0x1e02]
        mov    al, byte ptr [eax + 0x1e03]
        and    ecx, 0xffff00ff
        and    eax, 0xffff00ff
        shl    cx, 8
        add    ax, cx
        mov    ecx, dword ptr [esi + 0xb2]
        mov    word ptr [esi + 0xb6], ax
        mov    edx, eax
        and    edx, 0xffff
        mov    dword ptr [ebp - 8], ecx
        add    ecx, edx
        mov    ax, word ptr [ecx + 6]
        mov    word ptr [ebp - 4], ax
        test   ax, ax
        je     loc_2_0470
        cmp    ax, 0x30
        jne    loc_2_043F
        push   7
        mov    eax, dword ptr [esi + 0x82]
        push   4
        push   0x32
        push   0xaa00000a
        push   eax
        call   sub_1_4018
loc_2_042E:
        add    esp, 0x14
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_043F:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x82]
        and    eax, 0xffff
        push   eax
        push   7
        push   4
        push   3
        push   0xaa000003
        push   ecx
        call   sub_1_4018
loc_2_045F:
        add    esp, 0x18
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0470:
        mov    byte ptr [esi + 0x16f], 0
        mov    byte ptr [esi + 0x2c], 1
        mov    al, byte ptr [ecx + 1]
        lea    edx, [esi + 0x35]
        mov    byte ptr [ebp - 4], al
        test   byte ptr [ebp - 4], 1
        je     loc_2_048F
        mov    byte ptr [edx], 1
        jmp    loc_2_0492
loc_2_048F:
        mov    byte ptr [edx], 0
loc_2_0492:
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [esi + 0x36], 1
        mov    byte ptr [ebp - 4], al
        test   byte ptr [ebp - 4], 4
        jne    loc_2_04A6
        mov    byte ptr [esi + 0x36], 0
loc_2_04A6:
        mov    edi, dword ptr [esi + 0x40]
        cmp    edi, 0x2000
        jbe    loc_2_05DD
        mov    al, byte ptr [esi + 0xdc]
        cmp    edi, 0x10000
        mov    byte ptr [ebp - 1], al
        jne    loc_2_04CE
        sub    edi, 0x2830
        jmp    loc_2_04D4
loc_2_04CE:
        sub    edi, 0x630
loc_2_04D4:
        cmp    byte ptr [edx], 0
        je     loc_2_04E6
        mov    ax, word ptr [esi + 0x46]
        mov    word ptr [esi + 0xde], ax
        jmp    loc_2_04F1
loc_2_04E6:
        mov    dx, word ptr [esi + 0x44]
        mov    word ptr [esi + 0xde], dx
loc_2_04F1:
        mov    ax, word ptr [esi + 0xde]
        mov    edx, eax
        and    edx, 0xffff
        mov    dword ptr [ebp - 0x10], edx
        cmp    edi, edx
        jae    loc_2_051B
        xor    ebx, ebx
        mov    eax, edi
        mov    bx, word ptr [esi + 0xe6]
        sub    edx, edx
        div    ebx
        shr    eax, 1
        jmp    loc_2_052B
loc_2_051B:
        lea    edx, [esi + 0xe4]
        sub    dx, dx
        div    word ptr [esi + 0xe6]
loc_2_052B:
        inc    ax
        mov    word ptr [esi + 0xe4], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 0xe6]
        xor    ebx, ebx
        mov    bl, byte ptr [ebp - 1]
        mov    dword ptr [ebp - 0xc], eax
        xor    eax, eax
        sub    edx, edx
        mov    ax, word ptr [esi + 0xe4]
        imul   eax, dword ptr [ebp - 0xc]
        sub    edi, eax
        mov    eax, edi
        div    ebx
        cmp    eax, dword ptr [ebp - 0x10]
        jae    loc_2_059C
        cmp    eax, 0x1000
        jae    loc_2_0571
        mov    word ptr [esi + 0xde], 0x800
        jmp    loc_2_059C
loc_2_0571:
        cmp    eax, 0x2000
        jae    loc_2_0583
        mov    word ptr [esi + 0xde], 0x1000
        jmp    loc_2_059C
loc_2_0583:
        mov    word ptr [esi + 0xde], 0x2000
        cmp    eax, 0x4000
        jb     loc_2_059C
        mov    word ptr [esi + 0xde], 0x4000
loc_2_059C:
        mov    eax, dword ptr [esi + 0xe0]
        xor    edx, edx
        mov    dx, word ptr [esi + 0xde]
        cmp    edx, eax
        jbe    loc_2_05B6
        mov    word ptr [esi + 0xde], ax
loc_2_05B6:
        xor    eax, eax
        mov    ax, word ptr [esi + 0xde]
        lea    edx, [eax - 6]
        imul   eax, ebx
        sub    edi, eax
        mov    dword ptr [esi + 0xe0], edx
        mov    eax, edi
        sub    edx, edx
        div    dword ptr [ebp - 0xc]
        add    word ptr [esi + 0xe4], ax
        jmp    loc_2_061E
loc_2_05DD:
        mov    word ptr [esi + 0xde], 0x800
        mov    word ptr [esi + 0xdc], 1
        mov    eax, dword ptr [esi + 0xe0]
        cmp    eax, 0x800
        jae    loc_2_0603
        mov    word ptr [esi + 0xde], ax
loc_2_0603:
        mov    word ptr [esi + 0xe4], 0xf
        xor    eax, eax
        mov    ax, word ptr [esi + 0xde]
        sub    eax, 6
        mov    dword ptr [esi + 0xe0], eax
loc_2_061E:
        mov    ax, word ptr [ecx + 8]
        mov    edi, dword ptr [ebp - 8]
        mov    word ptr [ebp - 4], ax
        push   6
        mov    ah, al
        mov    al, byte ptr [ebp - 3]
        mov    edx, eax
        lea    eax, [esi + 0x54]
        and    edx, 0xffff
        add    edi, edx
        push   edi
        push   eax
        call   sub_1_402C
loc_2_0644:
        lea    eax, [esi + 0x4e]
        cmp    byte ptr [eax], 0
        jne    loc_2_0673
        cmp    byte ptr [esi + 0x4f], 0
        jne    loc_2_0673
        cmp    byte ptr [esi + 0x50], 0
        jne    loc_2_0673
        cmp    byte ptr [esi + 0x51], 0
        jne    loc_2_0673
        cmp    byte ptr [esi + 0x52], 0
        jne    loc_2_0673
        cmp    byte ptr [esi + 0x53], 0
        jne    loc_2_0673
        push   6
        push   edi
        push   eax
        call   sub_1_402C
loc_2_0673:
        mov    eax, dword ptr [esi + 0xae]
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax + 0x1e0f], 0xff
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0690:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0xae]
        mov    byte ptr [eax + 0x1e08], 0xc0
        mov    eax, dword ptr [ecx + 0xae]
        mov    byte ptr [eax + 0x1e0f], 1
        mov    eax, dword ptr [ecx + 0xae]
        mov    byte ptr [eax + 0x1e0c], 0
        cmp    byte ptr [ecx + 0x39], 0
        je     loc_2_06D3
        mov    dl, byte ptr [ecx + 0x48]
        mov    eax, dword ptr [ecx + 0xae]
        mov    byte ptr [eax + 0x1e00], dl
loc_2_06D3:
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_06E0:
        push   ebp
        mov    eax, 8
        mov    ebp, esp
        sub    esp, 0x48
        mov    word ptr [ebp - 0x48], ax
        push   ebx
        mov    word ptr [ebp - 0x46], ax
        push   esi
        push   edi
        mov    eax, 7
        mov    dword ptr [ebp - 0x44], 0     ; -> obj1:0x4168
        mov    dword ptr [ebp - 0x3c], 0     ; -> loc_1_4178
        mov    dword ptr [ebp - 0x34], 0     ; -> obj1:0x41A8
        mov    word ptr [ebp - 0x40], ax
        mov    word ptr [ebp - 0x3e], ax
        mov    eax, 0xe
        mov    word ptr [ebp - 0x38], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 0x36], ax
        mov    dword ptr [ebp - 0xc], ebx
        mov    eax, 0x12
        mov    byte ptr [ebp - 1], bl
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj1:0x41CC
        mov    byte ptr [ebp - 2], 1
        mov    word ptr [ebp - 0x30], ax
        lea    ecx, [ebp - 8]
        mov    word ptr [ebp - 0x2e], ax
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   eax
        push   ebx
        push   0x379
        push   ecx
        call   sub_1_3F46
loc_2_075A:
        test   eax, eax
        je     loc_2_076C
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_076C:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0xde
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xe0], 0x4628
        mov    eax, dword ptr [ebp - 8]
        push   esi
        mov    dword ptr [eax + 0x15c], 2
        mov    edx, dword ptr [ecx + 4]
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [ebp - 0x14]
        mov    dword ptr [eax + 0x7e], edx
        push   ecx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_3FC4
loc_2_07B0:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_07D4
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x379
        push   eax
        call   sub_1_3F4C
loc_2_07C6:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_07D4:
        push   1
        lea    eax, [ebp - 0x48]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_3FCA
loc_2_07EB:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_07FE
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 4], 3
        jne    loc_2_07FE
        mov    byte ptr [ebp - 1], 1
loc_2_07FE:
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_0832
        lea    eax, [ebp - 0x28]
        lea    ecx, [ebp - 0x20]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        push   esi
        push   edx
        call   sub_1_3EC0
loc_2_0816:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_0823
        mov    bl, 1
        jmp    loc_2_093C
loc_2_0823:
        mov    al, byte ptr [ebp - 0x25]
        and    eax, 1
        je     loc_2_0866
        cmp    eax, 1
        je     loc_2_0879
        jmp    loc_2_088A
loc_2_0832:
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_3FCA
loc_2_0849:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0860
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ebp - 2], al
loc_2_0860:
        cmp    byte ptr [ebp - 2], 0
        je     loc_2_0879
loc_2_0866:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x5a], 0xa20
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x38], 1
        jmp    loc_2_088A
loc_2_0879:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x5a], 0xa24
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x38], 2
loc_2_088A:
        push   0
        lea    eax, [ebp - 0x38]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_3FCA
loc_2_08A1:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_08B6
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0xe0], edx
loc_2_08B6:
        mov    eax, dword ptr [ebp - 0x14]
        lea    ecx, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x1c]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_3ECC
loc_2_08CB:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0908
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_2_097B
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x1c]
        add    eax, 0x4e
        mov    edx, ecx
        mov    dl, byte ptr [ecx]
        mov    byte ptr [eax], dl
        mov    dl, byte ptr [ecx + 1]
        mov    byte ptr [eax + 1], dl
        mov    dl, byte ptr [ecx + 2]
        mov    byte ptr [eax + 2], dl
        mov    dl, byte ptr [ecx + 3]
        mov    byte ptr [eax + 3], dl
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 4], dl
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
loc_2_0908:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_3FCA
loc_2_091F:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x2b], 1
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_093C
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 4], 0
        jne    loc_2_093C
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x2b], 0
loc_2_093C:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_3FD0
loc_2_0945:
        push   ebx
        mov    al, byte ptr [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        push   esi
        push   edx
        call   sub_2_0110
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_2_096F
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x379
        push   eax
        call   sub_1_3F4C
loc_2_096F:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_097B:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_2_0908
        mov    bl, 1
        jmp    loc_2_093C
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0990:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [edi + 0x5a]
        in     al, dx
        add    edx, 2
        mov    byte ptr [ebp - 4], al
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xfc
        mov    cl, byte ptr [ebp - 5]
        shl    eax, 0xb
        and    ecx, 1
        shl    ecx, 0x13
        push   0x2000
        or     eax, ecx
        mov    edx, dword ptr [edi + 0x82]
        push   eax
        lea    ecx, [edi + 0xae]
        push   edx
        lea    eax, [ebp - 0xc]
        push   ecx
        push   eax
        call   sub_1_4012
loc_2_09DD:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_0A04
        push   0
        mov    eax, dword ptr [edi + 0x82]
        push   0xaa000001
        push   eax
        call   sub_1_4018
loc_2_09F6:
        add    esp, 0xc
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0A04:
        mov    edx, dword ptr [edi + 0xae]
        lea    ecx, [ebp - 0x18]
loc_2_0A0D:
        xor    esi, esi
        mov    dword ptr [ecx], 0
loc_2_0A15:
        mov    al, byte ptr [edx + esi + 0x1f30]
        mov    ebx, dword ptr [ecx]
        shl    ebx, 4
        and    eax, 0xf
        add    eax, ebx
        add    esi, 2
        mov    dword ptr [ecx], eax
        cmp    esi, 0x10
        jb     loc_2_0A15
        add    edx, 0x10
        add    ecx, 4
        lea    eax, [ebp - 0xc]
        cmp    ecx, eax
        jb     loc_2_0A0D
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [0], eax            ; -> loc_1_40E0
        jne    loc_2_0A66
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [4], eax            ; -> loc_1_40E4
        jne    loc_2_0A66
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [8], eax            ; -> loc_1_40E8
        jne    loc_2_0A66
        lea    eax, [edi + 0x39]
        mov    byte ptr [eax], 1
        jmp    loc_2_0A99
loc_2_0A66:
        mov    eax, dword ptr [ebp - 0x18]
        cmp    dword ptr [0], eax            ; -> loc_1_40EC
        jne    loc_2_0D93
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [4], eax            ; -> loc_1_40F0
        jne    loc_2_0D93
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [8], eax            ; -> loc_1_40F4
        jne    loc_2_0D93
        lea    eax, [edi + 0x39]
        mov    byte ptr [eax], 0
loc_2_0A99:
        mov    edx, dword ptr [edi + 0xae]
        mov    cl, byte ptr [edx + 0x1e01]
        and    cl, 0xc
        shr    cl, 2
        cmp    byte ptr [eax], 0
        je     loc_2_0BCC
        mov    al, byte ptr [edx + 0x1e00]
        mov    byte ptr [edi + 0x48], al
        test   al, al
        jne    loc_2_0AD0
        mov    al, byte ptr [ebp - 4]
        and    al, 0xfd
        shr    al, 1
        or     al, 0x80
        add    al, 2
        mov    byte ptr [edi + 0x48], al
loc_2_0AD0:
        xor    eax, eax
        mov    al, cl
        cmp    eax, 3
        ja     loc_2_0B07
        jmp    dword ptr [eax*4]             ; -> obj2:0x0DB4
loc_2_0AE0:
        mov    al, 4
        cmp    cl, 1
        sbb    esi, esi
        and    esi, 0xffffe000
        add    esi, 0x4000
        jmp    loc_2_0B0D
loc_2_0AF5:
        mov    esi, 0x8000
        mov    al, 8
        jmp    loc_2_0B0D
loc_2_0AFE:
        mov    esi, 0x10000
        mov    al, 0x10
        jmp    loc_2_0B0D
loc_2_0B07:
        mov    esi, dword ptr [ebp - 4]
        mov    al, byte ptr [ebp - 4]
loc_2_0B0D:
        mov    dl, byte ptr [edi + 0x48]
        xor    ecx, ecx
        mov    cl, al
        xor    ebx, ebx
        dec    ecx
        mov    bl, dl
        test   ebx, ecx
        je     loc_2_0B29
        lea    ecx, [eax - 1]
        not    cl
        and    cl, dl
        add    cl, al
        mov    byte ptr [edi + 0x48], cl
loc_2_0B29:
        xor    eax, eax
        push   esi
        mov    al, byte ptr [edi + 0x48]
        mov    ecx, dword ptr [edi + 0x82]
        shl    eax, 0xc
        lea    edx, [edi + 0xb2]
        push   eax
        mov    dword ptr [edi + 0x40], esi
        push   ecx
        lea    eax, [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_4012
loc_2_0B4D:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_0B74
        push   0
        mov    eax, dword ptr [edi + 0x82]
        push   0xaa000001
        push   eax
        call   sub_1_4018
loc_2_0B66:
        add    esp, 0xc
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0B74:
        mov    eax, dword ptr [edi + 0xae]
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 3
        mov    al, byte ptr [eax + 0x1fba]
        cmp    ecx, 3
        ja     loc_2_0C77
        jmp    dword ptr [ecx*4]             ; -> obj2:0x0DC4
loc_2_0B96:
        mov    eax, 2
        jmp    loc_2_0C7A
loc_2_0BA0:
        mov    eax, 3
        jmp    loc_2_0C7A
loc_2_0BAA:
        sub    al, 0xf
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xfffffffc
        add    eax, 0xa
        jmp    loc_2_0C7A
loc_2_0BBB:
        sub    al, 0xf
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xfffffffc
        add    eax, 0xb
        jmp    loc_2_0C7A
loc_2_0BCC:
        xor    eax, eax
        mov    al, cl
        cmp    eax, 3
        ja     loc_2_0BF8
        jmp    dword ptr [eax*4]             ; -> obj2:0x0DD4
loc_2_0BDC:
        mov    esi, 0x2000
        jmp    loc_2_0BFB
loc_2_0BE3:
        mov    esi, 0x4000
        jmp    loc_2_0BFB
loc_2_0BEA:
        mov    esi, 0x8000
        jmp    loc_2_0BFB
loc_2_0BF1:
        mov    esi, 0x10000
        jmp    loc_2_0BFB
loc_2_0BF8:
        mov    esi, dword ptr [ebp - 4]
loc_2_0BFB:
        mov    al, byte ptr [ebp - 5]
        push   esi
        and    eax, 0xfe
        mov    ecx, dword ptr [edi + 0x82]
        shl    eax, 0xc
        lea    edx, [edi + 0xb2]
        push   eax
        push   ecx
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_4012
loc_2_0C1F:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_0C46
        push   0
        mov    eax, dword ptr [edi + 0x82]
        push   0xaa000001
        push   eax
        call   sub_1_4018
loc_2_0C38:
        add    esp, 0xc
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0C46:
        mov    al, byte ptr [ebp - 4]
        mov    dword ptr [edi + 0x40], esi
        and    eax, 3
        cmp    eax, 3
        ja     loc_2_0C77
        jmp    dword ptr [eax*4]             ; -> obj2:0x0DE4
loc_2_0C5B:
        mov    eax, 2
        jmp    loc_2_0C7A
loc_2_0C62:
        mov    eax, 3
        jmp    loc_2_0C7A
loc_2_0C69:
        mov    eax, 0xa
        jmp    loc_2_0C7A
loc_2_0C70:
        mov    eax, 0xb
        jmp    loc_2_0C7A
loc_2_0C77:
        mov    eax, dword ptr [ebp - 4]
loc_2_0C7A:
        mov    edx, dword ptr [edi + 0xae]
        mov    dword ptr [edi + 0x31], eax
        mov    byte ptr [edi + 0x3a], 0
        mov    cl, byte ptr [edx + 0x1fa6]
        and    ecx, 0xf
        lea    eax, [ecx - 0xa]
        cmp    eax, 5
        ja     loc_2_0C9F
        jmp    dword ptr [eax*4]             ; -> obj2:0x0DF4
loc_2_0C9F:
        push   ecx
        mov    eax, dword ptr [edi + 0x82]
        push   6
        push   2
        push   3
        push   0xaa00000a
        push   eax
        call   sub_1_4018
loc_2_0CB7:
        add    esp, 0x18
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0CC5:
        mov    dword ptr [edi + 0x3c], 0x10000
        jmp    loc_2_0CF9
loc_2_0CCE:
        mov    dword ptr [edi + 0x3c], 0x10000
        mov    byte ptr [edi + 0x3a], 1
        jmp    loc_2_0CF9
loc_2_0CDB:
        mov    dword ptr [edi + 0x3c], 0x8000
        jmp    loc_2_0CF9
loc_2_0CE4:
        mov    dword ptr [edi + 0x3c], 0x4000
        jmp    loc_2_0CF9
loc_2_0CED:
        mov    dword ptr [edi + 0x3c], 0x2000
        jmp    loc_2_0CF9
loc_2_0CF6:
        mov    dword ptr [edi + 0x3c], esi
loc_2_0CF9:
        mov    byte ptr [edi + 0x3b], 0
        mov    al, byte ptr [edx + 0x1faa]
        and    eax, 0xf
        cmp    eax, 0xd
        je     loc_2_0D18
        cmp    eax, 0xe
        je     loc_2_0D20
        mov    word ptr [edi + 0x44], 0x800
        jmp    loc_2_0D26
loc_2_0D18:
        mov    word ptr [edi + 0x44], 0x1170
        jmp    loc_2_0D26
loc_2_0D20:
        mov    word ptr [edi + 0x44], 0x1000
loc_2_0D26:
        mov    al, byte ptr [edx + 0x1fac]
        and    eax, 0xf
        sub    eax, 0xb
        cmp    eax, 3
        ja     loc_2_0D3E
        jmp    dword ptr [eax*4]             ; -> obj2:0x0E0C
loc_2_0D3E:
        mov    word ptr [edi + 0x46], 0x800
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D4F:
        mov    word ptr [edi + 0x46], 0x4628
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D60:
        mov    word ptr [edi + 0x46], 0x4000
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D71:
        mov    word ptr [edi + 0x46], 0x2000
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D82:
        mov    word ptr [edi + 0x46], 0x1000
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D93:
        push   0
        mov    eax, dword ptr [edi + 0x82]
        push   0xaa00000d
        push   eax
        call   sub_1_4018
loc_2_0DA6:
        add    esp, 0xc
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        ; ---- dispatch table (26 entries) ----
        dd  loc_2_0AE0
        dd  loc_2_0AE0
        dd  loc_2_0AF5
        dd  loc_2_0AFE
        dd  loc_2_0B96
        dd  loc_2_0BA0
        dd  loc_2_0BAA
        dd  loc_2_0BBB
        dd  loc_2_0BDC
        dd  loc_2_0BE3
        dd  loc_2_0BEA
        dd  loc_2_0BF1
        dd  loc_2_0C5B
        dd  loc_2_0C62
        dd  loc_2_0C69
        dd  loc_2_0C70
        dd  loc_2_0CC5
        dd  loc_2_0CCE
        dd  loc_2_0CDB
        dd  loc_2_0CE4
        dd  loc_2_0CED
        dd  loc_2_0CF6
        dd  loc_2_0D4F
        dd  loc_2_0D60
        dd  loc_2_0D71
        dd  loc_2_0D82
        db  08Dh, 064h, 024h, 000h
sub_2_0E20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [0x41e4]       ; -> obj1:0x41E4
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   0x32
        push   eax
        call   sub_1_3F46
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0E50
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  090h
loc_2_0E50:
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
sub_2_0EF0:
        push   ebp
        mov    ebp, esp
        push   0
        push   0x32
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3F4C
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0F10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x2e], 0
        jne    loc_2_0F29
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0F29:
        mov    eax, dword ptr [0x41e4]       ; -> obj1:0x41E4
        push   eax
        push   0
        push   0x1f
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_3F46
        or     eax, eax
        je     loc_2_0F49
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0F49:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x2e]
        mov    edi, edi
loc_2_0F50:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_0F63
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_0F50
        jmp    loc_2_0F72
loc_2_0F63:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x2e], eax
loc_2_0F72:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_0F88
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_0F88:
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
sub_2_0FF0:
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
        call   sub_2_1310
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_1019
        cmp    ebx, 0x103
        jne    loc_2_1031
loc_2_1019:
        push   0
        push   0x41e0                        ; -> obj1:0x41E0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_10E0
        or     eax, eax
        je     loc_2_1031
        mov    ebx, eax
loc_2_1031:
        or     ebx, ebx
        je     loc_2_103D
        cmp    ebx, 0x103
        jne    loc_2_1068
loc_2_103D:
        cmp    byte ptr [edi + 0x1d], 0
        je     loc_2_1068
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [edi + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_2_1068
        push   0
        push   0x41e0                        ; -> obj1:0x41E0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_1180
        or     eax, eax
        je     loc_2_1068
        mov    ebx, eax
loc_2_1068:
        or     ebx, ebx
        je     loc_2_1074
        cmp    ebx, 0x103
        jne    loc_2_10D5
loc_2_1074:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_2_10D0
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_108A
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_108A:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_1099
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_109F
loc_2_1099:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_2_109F:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_2_10AE
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3EA8
loc_2_10AE:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_3F4C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_10D0:
        mov    ebx, 0xc001000e
loc_2_10D5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_10E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        xor    edx, edx
        mov    esi, dword ptr [edi + 4]
        mov    ecx, dword ptr [ebx + 0xc]
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
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [edi + 4], 0
        or     eax, eax
        je     loc_2_1140
loc_2_1133:
        mov    ecx, dword ptr [eax + 0xc]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_1133
loc_2_1140:
        mov    ecx, dword ptr [edi + 4]
        cmp    esi, ecx
        je     loc_2_1174
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 0x1e]
        or     eax, eax
        je     loc_2_1176
        cmp    eax, 0x103
        je     loc_2_1176
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0xc], ecx
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1174:
        sub    eax, eax
loc_2_1176:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
sub_2_1180:
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
        mov    al, byte ptr [0x41e0]         ; -> obj1:0x41E0
        cmp    byte ptr [ecx], al
        jne    loc_2_1228
        mov    al, byte ptr [0x41e1]         ; -> loc_1_41E1
        cmp    byte ptr [ecx + 1], al
        jne    loc_2_1228
        mov    al, byte ptr [0x41e2]         ; -> obj1:0x41E2
        cmp    byte ptr [ecx + 2], al
        jne    loc_2_1228
        mov    al, byte ptr [0x41e3]         ; -> loc_1_41E3
        cmp    byte ptr [ecx + 3], al
        jne    loc_2_1228
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_1209
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [ebx + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_12A3
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1209:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_121D
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_121D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1228:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    loc_2_1260
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_1244
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1244:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_12A3
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
loc_2_1260:
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 1
        jbe    loc_2_1276
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1276:
        mov    ebx, dword ptr [ebp + 0xc]
        jne    loc_2_1290
        cmp    byte ptr [ebx + 0x1d], 0
        jne    loc_2_1290
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
loc_2_1290:
        cmp    eax, 1
        jne    loc_2_12A3
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_12A3
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        mov    byte ptr [ebx + 0x1d], 0
loc_2_12A3:
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
        je     loc_2_12D6
        cmp    eax, 0x103
        je     loc_2_12D6
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0xc], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_12D6:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_12F0
        mov    byte ptr [ebx + 0x1d], 0
        mov    dword ptr [esi + 0xc], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_12F0:
        mov    byte ptr [ebx + 0x1d], 1
        mov    dword ptr [esi + 0xc], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1310:
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
        je     loc_2_134D
        lea    esp, [esp]
loc_2_1340:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0x10], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_1340
loc_2_134D:
        mov    ecx, dword ptr [esi + 0x10]
        cmp    ebx, ecx
        je     loc_2_1381
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
        je     loc_2_1383
        cmp    eax, 0x103
        je     loc_2_1383
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0x10], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1381:
        sub    eax, eax
loc_2_1383:
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

VxD__LDATA SEGMENT   ; object 4  vsize=0xe4  RW-
        db  0FFh, 0FFh, 0FFh, 0FFh, 001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h, 003h, 001h, 001h, 000h
        db  004h, 001h, 001h, 000h, 005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h, 011h, 001h, 001h, 000h
        db  013h, 001h, 001h, 000h, 012h, 001h, 001h, 000h, 007h, 001h, 001h, 000h, 008h, 001h, 001h, 000h
        db  009h, 001h, 001h, 000h, 00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h, 00Ch, 001h, 001h, 000h
        db  00Dh, 001h, 001h, 000h, 010h, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h, 00Fh, 001h, 001h, 000h
        db  001h, 001h, 002h, 000h, 002h, 001h, 002h, 000h, 003h, 001h, 002h, 000h, 004h, 001h, 002h, 000h
        db  005h, 001h, 002h, 000h, 001h, 001h, 001h, 002h, 002h, 001h, 001h, 002h, 003h, 001h, 001h, 002h
        db  004h, 001h, 001h, 002h, 005h, 001h, 001h, 002h, 006h, 001h, 001h, 002h, 007h, 001h, 001h, 002h
        db  001h, 001h, 002h, 002h, 002h, 001h, 002h, 002h, 001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h
        db  003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h, 005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h
        db  011h, 001h, 001h, 000h, 013h, 001h, 001h, 000h, 012h, 001h, 001h, 000h, 007h, 001h, 001h, 000h
        db  008h, 001h, 001h, 000h, 009h, 001h, 001h, 000h, 00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h
        db  00Ch, 001h, 001h, 000h, 00Dh, 001h, 001h, 000h, 010h, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h
        db  00Fh, 001h, 001h, 000h, 001h, 001h, 001h, 002h, 002h, 001h, 001h, 002h, 003h, 001h, 001h, 002h
        db  004h, 001h, 001h, 002h
VxD__LDATA ENDS

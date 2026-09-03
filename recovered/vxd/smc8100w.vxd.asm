; SMC8100W -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 17023 instructions recovered by recursive descent

Declare_Virtual_Device SMC8100W, 3, 10, SMC8100W_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xfcd0  R-X
        db  055h, 0B8h, 009h, 000h, 000h, 000h, 08Bh, 0ECh, 083h, 0ECh, 018h, 066h, 089h, 045h, 0E8h, 053h
        db  066h, 089h, 045h, 0EAh, 033h, 0DBh, 066h, 089h, 05Dh, 0F0h, 053h, 08Bh, 04Dh, 00Ch, 08Dh, 045h
        db  0F0h, 051h, 08Dh, 055h, 0F8h, 0C7h, 045h, 0ECh
        dd  loc_1_FB80               ; fixup
        db  066h, 0C7h, 045h, 0F2h, 001h, 000h, 066h, 0C7h, 045h, 0F4h, 0C6h, 06Eh, 050h, 052h, 0E8h, 0BBh
        db  0D6h, 000h, 000h, 08Dh, 04Dh, 0F0h, 089h, 00Dh
        dd  obj1:0xFA92              ; fixup
        db  08Bh, 055h, 0F8h, 089h, 015h
        dd  obj1:0xFA4C              ; fixup
        db  088h, 01Dh
        dd  obj1:0xFA96              ; fixup
        db  089h, 01Dh
        dd  obj1:0xFA86              ; fixup
        db  089h, 01Dh
        dd  obj1:0xFA8A              ; fixup
        db  0C6h, 005h
        dd  obj1:0xFA50              ; fixup
        db  003h, 088h, 01Dh
        dd  obj1:0xFA51              ; fixup
        db  089h, 01Dh
        dd  obj1:0xFA52              ; fixup
        db  0C7h, 005h
        dd  obj1:0xFA56              ; fixup
        dd  loc_1_08E0               ; fixup
        db  0C7h, 005h
        dd  obj1:0xFA5A              ; fixup
        dd  loc_1_0C00               ; fixup
        db  0C7h, 005h
        dd  obj1:0xFA5E              ; fixup
        dd  loc_1_16C0               ; fixup
        db  0C7h, 005h
        dd  obj1:0xFA62              ; fixup
        dd  loc_1_2200               ; fixup
        db  0C7h, 005h
        dd  obj1:0xFA66              ; fixup
        dd  loc_1_1A60               ; fixup
        db  08Bh, 045h, 0E8h, 08Bh, 04Dh, 0ECh, 0C7h, 005h
        dd  obj1:0xFA6A              ; fixup
        dd  loc_1_0D60               ; fixup
        db  0C7h, 005h
        dd  obj1:0xFA6E              ; fixup
        dd  loc_1_1520               ; fixup
        db  0C7h, 005h
        dd  obj1:0xFA72              ; fixup
        dd  loc_1_1690               ; fixup
        db  0C7h, 005h
        dd  obj1:0xFA76              ; fixup
        dd  loc_1_0130               ; fixup
        db  0C7h, 005h
        dd  obj1:0xFA7A              ; fixup
        dd  loc_1_15D0               ; fixup
        db  06Ah, 036h, 0A3h
        dd  obj1:0xFA7E              ; fixup
        db  068h
        dd  obj1:0xFA50              ; fixup
        db  08Dh, 045h, 0FCh, 068h
        dd  obj1:0xFA48              ; fixup
        db  089h, 00Dh
        dd  obj1:0xFA82              ; fixup
        db  08Bh, 055h, 0F8h, 052h, 068h
        dd  obj1:0xFA48              ; fixup
        db  050h, 0E8h, 0FDh, 0D5h, 000h, 000h, 039h, 05Dh, 0FCh, 074h, 014h, 053h, 08Bh, 045h, 0F8h, 050h
        db  0E8h, 0E8h, 0D5h, 000h, 000h, 08Bh, 045h, 0FCh, 05Bh, 08Bh, 0E5h, 05Dh, 0C2h, 008h, 000h, 08Bh
        db  045h, 0FCh, 05Bh, 08Bh, 0E5h, 05Dh, 0C2h, 008h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0130:
        push   ebp
        mov    eax, 7
        mov    ebp, esp
        sub    esp, 0x74
        mov    word ptr [ebp - 0x44], ax
        push   ebx
        mov    word ptr [ebp - 0x42], ax
        push   esi
        push   edi
        mov    eax, 4
        mov    dword ptr [ebp - 0x40], 0     ; -> loc_1_FB94
        mov    dword ptr [ebp - 0x30], 0     ; -> obj1:0xFBD8
        mov    dword ptr [ebp - 0x28], 0     ; -> loc_1_FBE8
        mov    word ptr [ebp - 0x34], ax
        mov    word ptr [ebp - 0x32], ax
        mov    eax, 0xb
        mov    dword ptr [ebp - 0x48], 0     ; -> loc_1_FBFC
        mov    word ptr [ebp - 0x2c], ax
        mov    word ptr [ebp - 0x2a], ax
        mov    eax, 8
        mov    word ptr [ebp - 0x4c], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 0x4a], ax
        mov    byte ptr [ebp - 1], bl
        mov    eax, 0x12
        xor    esi, esi
        mov    word ptr [ebp - 0x3c], ax
        mov    byte ptr [ebp - 2], bl
        mov    word ptr [ebp - 0x3a], ax
        mov    byte ptr [ebp - 0x24], bl
        lea    eax, [ebp - 0x23]
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        mov    dword ptr [eax], ebx
        mov    dword ptr [ebp - 0x38], 0     ; -> obj1:0xFC30
        mov    word ptr [ebp - 4], 0x280
        mov    byte ptr [eax + 4], bl
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_D864
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_1_01DC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_01DC:
        push   1
        lea    eax, [ebp - 0x4c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_020C
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        sub    eax, 3
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ebp - 2], al
loc_1_020C:
        cmp    byte ptr [ebp - 2], 1
        je     loc_1_0239
        push   1
        lea    eax, [ebp - 0x44]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_0239
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [ebp - 4], cx
loc_1_0239:
        push   1
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_025C
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [eax + 4]
loc_1_025C:
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 0x1c]
        push   eax
        lea    edx, [ebp - 0x18]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_D76C
        cmp    dword ptr [ebp - 0x1c], 6
        jne    loc_1_02A5
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_02A5
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, eax
        mov    cl, byte ptr [eax]
        mov    byte ptr [ebp - 0x24], cl
        mov    dl, byte ptr [eax + 1]
        mov    byte ptr [ebp - 0x23], dl
        mov    cl, byte ptr [eax + 2]
        mov    byte ptr [ebp - 0x22], cl
        mov    dl, byte ptr [eax + 3]
        mov    byte ptr [ebp - 0x21], dl
        mov    cl, byte ptr [eax + 4]
        mov    byte ptr [ebp - 0x20], cl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 0x1f], al
loc_1_02A5:
        xor    edi, edi
loc_1_02A7:
        mov    eax, dword ptr [0]            ; -> obj1:0xFA44
        lea    ecx, [ebp - 8]
        push   eax
        push   edi
        push   0x47a
        push   ecx
        call   sub_1_D7E6
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_1_02CE
        mov    byte ptr [ebp - 1], 1
        mov    ebx, 0xaa000002
        jmp    loc_1_02A7
loc_1_02CE:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0x11e
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        cmp    byte ptr [ebp - 1], al
        jne    loc_1_030C
        cmp    byte ptr [ebp - 2], 1
        jne    loc_1_030C
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5E93
        test   ax, ax
        je     loc_1_030C
        mov    byte ptr [ebp - 1], 1
        mov    word ptr [ebp - 4], 0
        mov    ebx, 0xaa00000a
loc_1_030C:
        lea    edi, [ebp - 0x74]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    al, byte ptr [ebp - 2]
        mov    dword ptr [ebp - 0x5e], 1
        dec    al
        mov    ecx, dword ptr [ebp - 4]
        cmp    al, 1
        mov    dword ptr [ebp - 0x56], 0x20
        sbb    eax, eax
        lea    edx, [ebp - 0x74]
        and    eax, 2
        and    ecx, 0xffff
        inc    eax
        push   edx
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x6a], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x5a], ecx
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj1:0xFA48
        push   edx
        push   eax
        lea    eax, [edx + 0x4c]
        push   eax
        call   sub_1_D712
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_1_0391
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_D870
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x47a
        push   eax
        call   sub_1_D7EC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0391:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_03DC
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 0x4c]
        push   ecx
        call   sub_1_D8B8
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_D870
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 0x4c]
        push   edx
        call   sub_1_D718
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x47a
        push   ecx
        call   sub_1_D7EC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_03DC:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
        mov    al, byte ptr [ebp - 2]
        mov    edx, dword ptr [ebp - 8]
        test   esi, esi
        mov    byte ptr [edx], al
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x50], 1
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x40], 6
        mov    eax, dword ptr [ebp - 8]
        je     loc_1_0416
        mov    word ptr [eax + 0x204], si
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x36], si
        jmp    loc_1_0429
loc_1_0416:
        mov    ecx, 0x3e80
        mov    word ptr [eax + 0x36], cx
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x204], cx
loc_1_0429:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_62E9
        cmp    ax, 0xffff
        jne    loc_1_0481
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000004
        mov    ecx, dword ptr [eax + 0x4c]
        push   ecx
        call   sub_1_D8B8
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_D870
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 0x4c]
        push   edx
        call   sub_1_D718
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x47a
        push   ecx
        call   sub_1_D7EC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0481:
        cmp    ax, 2
        jne    loc_1_04DA
        push   0
        lea    eax, [ebp - 0x34]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_04B1
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 0xa], dx
loc_1_04B1:
        push   1
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_04DA
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x18], edx
loc_1_04DA:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_D870
        lea    eax, [ebp - 0x24]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   0x10
        push   2
        push   ecx
        call   sub_1_0530
        test   eax, eax
        je     loc_1_0522
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0x4c]
        push   ecx
        call   sub_1_D718
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x47a
        push   ecx
        call   sub_1_D7EC
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0522:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_0530:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x276], eax
        cmp    edi, 2
        jbe    loc_1_0574
        push   0
        mov    eax, dword ptr [esi + 0x4c]
        push   0xaa000002
        push   eax
        call   sub_1_D8B8
        mov    dword ptr [ebp - 4], 0xc000009a
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0574:
        xor    ebx, ebx
        mov    eax, 0                        ; -> sub_1_1D50
        mov    dword ptr [esi + 0x26d], ebx
        mov    dword ptr [esi + 0x327], eax
        mov    dword ptr [esi + 0x31b], ebx
        mov    dword ptr [esi + 0x31f], eax
        mov    dword ptr [esi + 0x265], 0    ; -> obj1:0xFA48
        mov    dword ptr [esi + 0x323], esi
        mov    ecx, dword ptr [0x42]         ; -> obj1:0xFA8A
        mov    dword ptr [esi + 0x269], ecx
        mov    dword ptr [0x42], esi         ; -> obj1:0xFA8A
        movsx  eax, byte ptr [esi + 0xa]
        cmp    byte ptr [esi], 1
        sbb    ecx, ecx
        add    ecx, 2
        push   ecx
        push   ebx
        push   eax
        push   eax
        push   0                             ; -> loc_1_1BE0
        mov    eax, dword ptr [esi + 0x4c]
        push   esi
        lea    edx, [esi + 0x20c]
        push   0                             ; -> loc_1_1BC0
        lea    ecx, [ebp - 4]
        push   eax
        push   edx
        push   ecx
        call   sub_1_D7CE
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_0607
        push   ebx
        mov    eax, dword ptr [esi + 0x4c]
        push   0xaa000005
        push   eax
        call   sub_1_D8B8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0607:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x18]
        mov    ax, word ptr [esi + 0x20]
        mov    edx, dword ptr [esi + 0x4c]
        shl    eax, 0xa
        push   eax
        push   ecx
        push   edx
        lea    eax, [esi + 0x1c]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_D8B2
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0644
        push   0
        mov    eax, dword ptr [esi + 0x4c]
        push   0xaa000001
        push   eax
        call   sub_1_D8B8
        add    esp, 0xc
        jmp    loc_1_0827
loc_1_0644:
        xor    edx, edx
        mov    eax, dword ptr [ebp + 0x14]
loc_1_0649:
        mov    cl, byte ptr [edx + eax]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [edx + esi + 0x23], cl
        jb     loc_1_0649
        mov    word ptr [esi + 0x38], 0x100
        or     byte ptr [esi + 0x200], 0x40
        mov    word ptr [esi + 0x3a], di
        lea    eax, [esi + 0x2cb]
        lea    ecx, [esi + 0x2d3]
        lea    edx, [esi + 0x2cf]
        mov    dword ptr [esi + 0x1c8], eax
        mov    dword ptr [esi + 0x1d0], ecx
        lea    eax, [esi + 0x2c7]
        lea    ecx, [esi + 0x2d7]
        mov    dword ptr [esi + 0x1cc], edx
        mov    dword ptr [esi + 0x1d4], eax
        lea    edx, [esi + 0x2db]
        lea    eax, [esi + 0x2df]
        mov    dword ptr [esi + 0x1d8], ecx
        mov    dword ptr [esi + 0x1a0], edx
        lea    ecx, [esi + 0x2e3]
        lea    edx, [esi + 0x2e7]
        mov    dword ptr [esi + 0x1a4], eax
        mov    dword ptr [esi + 0x1a8], ecx
        lea    eax, [esi + 0x2eb]
        lea    ecx, [esi + 0x2ef]
        mov    dword ptr [esi + 0x1ac], edx
        mov    dword ptr [esi + 0x1b0], eax
        lea    edx, [esi + 0x2f3]
        lea    eax, [esi + 0x2f7]
        mov    dword ptr [esi + 0x1b4], ecx
        mov    dword ptr [esi + 0x1b8], edx
        lea    ecx, [esi + 0x2fb]
        lea    edx, [esi + 0x2ff]
        mov    dword ptr [esi + 0x1bc], eax
        mov    dword ptr [esi + 0x1c0], ecx
        lea    eax, [esi + 0x2b7]
        lea    ecx, [esi + 0x2ab]
        mov    dword ptr [esi + 0x1c4], edx
        mov    dword ptr [esi + 0x1dc], eax
        lea    edx, [esi + 0x2af]
        lea    eax, [esi + 0x2b3]
        mov    dword ptr [esi + 0x1e0], ecx
        mov    dword ptr [esi + 0x1e4], edx
        lea    ecx, [esi + 0x2bb]
        lea    edx, [esi + 0x2bf]
        lea    ebx, [esi + 0x25d]
        mov    dword ptr [esi + 0x1e8], eax
        lea    eax, [esi + 0x2c3]
        mov    dword ptr [esi + 0x1ec], ecx
        mov    dword ptr [esi + 0x1f0], edx
        mov    dword ptr [esi + 0x1f8], eax
        push   esi
        mov    dword ptr [ebx], 0
        mov    dword ptr [esi + 0xab], 0     ; -> loc_1_D9C4
        mov    word ptr [esi + 0x3e], 0
        call   sub_1_5FE8
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_1_07A1
        cmp    eax, 0x13
        jne    loc_1_0827
loc_1_07A1:
        push   esi
        call   sub_1_2B50
        test   ax, ax
        je     loc_1_07C8
        push   0
        mov    eax, dword ptr [esi + 0x4c]
        push   0xaa000003
        push   eax
        call   sub_1_D8B8
        mov    dword ptr [ebp - 4], 0xc0010006
        add    esp, 0xc
        jmp    loc_1_0827
loc_1_07C8:
        xor    edi, edi
loc_1_07CA:
        cmp    word ptr [esi + 0x40], 2
        je     loc_1_07E4
        push   0x3e8
        inc    edi
        call   sub_1_D7E0
        cmp    edi, 0x3e8
        jb     loc_1_07CA
loc_1_07E4:
        push   ebx
        lea    eax, [esi + 0x261]
        push   eax
        lea    ecx, [esi + 0x24]
        push   ecx
        push   0                             ; -> loc_1_1BB0
        push   0                             ; -> loc_1_1B30
        push   0                             ; -> loc_1_28B0
        push   0                             ; -> loc_1_27E0
        call   sub_2_0000
loc_1_0809:
        test   al, al
        jne    loc_1_088C
        push   0
        mov    eax, dword ptr [esi + 0x4c]
        push   0xaa000002
        push   eax
        call   sub_1_D8B8
        mov    dword ptr [ebp - 4], 0xc0000001
        add    esp, 0xc
loc_1_0827:
        lea    eax, [esi + 0x20c]
        push   eax
        call   sub_1_D7D4
        cmp    esi, dword ptr [0x42]         ; -> obj1:0xFA8A
        jne    loc_1_0852
        mov    eax, dword ptr [esi + 0x269]
        pop    edi
        pop    esi
        mov    dword ptr [0x42], eax         ; -> obj1:0xFA8A
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0852:
        mov    ecx, dword ptr [0x42]         ; -> obj1:0xFA8A
        cmp    dword ptr [ecx + 0x269], esi
        je     loc_1_086E
loc_1_0860:
        mov    ecx, dword ptr [ecx + 0x269]
        cmp    dword ptr [ecx + 0x269], esi
        jne    loc_1_0860
loc_1_086E:
        mov    eax, dword ptr [ecx + 0x269]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x269]
        mov    dword ptr [ecx + 0x269], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_088C:
        mov    eax, 0                        ; -> loc_1_24A0
        xor    ecx, ecx
        mov    dword ptr [esi + 0x45a], eax
        mov    dword ptr [esi + 0x45e], ecx
        mov    dword ptr [esi + 0x462], eax
        mov    dword ptr [esi + 0x466], esi
        mov    eax, 0                        ; -> sub_1_29F0
        pop    edi
        mov    dword ptr [esi + 0x46a], eax
        mov    dword ptr [esi + 0x46e], ecx
        mov    dword ptr [esi + 0x472], eax
        mov    dword ptr [esi + 0x476], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08E0:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    edx, dword ptr [ebp + 0x18]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [esi], 0
        mov    dword ptr [ecx], 0xffffffff
        lea    eax, [eax + edx*4]
loc_1_0904:
        test   edx, edx
        je     loc_1_0913
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 1
        jne    loc_1_0904
        mov    dword ptr [ecx], edx
loc_1_0913:
        cmp    dword ptr [ecx], -1
        jne    loc_1_0925
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0925:
        mov    eax, dword ptr [0]            ; -> obj1:0xFA44
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x75
        push   ecx
        call   sub_1_D7E6
        test   eax, eax
        je     loc_1_0949
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0949:
        mov    edi, dword ptr [ebp + 0x20]
        inc    dword ptr [edi + 0x272]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [edi + 0x265]
        mov    dword ptr [eax + 0x10], edx
        mov    edx, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0x28]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x20], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 0xf2
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        mov    dword ptr [edi + 0x317], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        cmp    word ptr [edi + 0x40], 1
        je     loc_1_0A22
        mov    byte ptr [edi + 0x271], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0x456], eax
        cmp    word ptr [edi + 0x40], 2
        je     loc_1_0A22
        push   edi
        call   sub_1_2B50
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_09FD
        mov    byte ptr [edi + 0x303], 1
        mov    esi, 0xfa0
loc_1_09D2:
        push   0x3e8
        call   sub_1_D7E0
        dec    esi
        jne    loc_1_09D2
        push   0x7530
        add    edi, 0x45e
        push   edi
        call   sub_1_D7C2
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_09FD:
        push   0x7530
        add    edi, 0x45e
        push   edi
        mov    byte ptr [edi - 0x15b], 0
        call   sub_1_D7C2
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0A22:
        mov    eax, dword ptr [edi + 0x26d]
        test   eax, eax
        jne    loc_1_0A81
        push   edi
        call   sub_1_2F78
        test   ax, ax
        je     loc_1_0A5C
        push   0x7530
        add    edi, 0x45e
        push   edi
        mov    byte ptr [edi - 0x15b], 1
        call   sub_1_D7C2
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0A5C:
        push   0x7530
        add    edi, 0x45e
        push   edi
        mov    byte ptr [edi - 0x15b], 0
        call   sub_1_D7C2
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0A81:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0x26d], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [edi + 0x25d]
        add    eax, 0x14
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        push   eax
        push   edx
        call   sub_2_00F0
loc_1_0AAA:
        test   al, al
        jne    loc_1_0AD3
        dec    dword ptr [edi + 0x272]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xc]
        mov    eax, 0xc0000001
        mov    dword ptr [edi + 0x26d], ecx
        pop    edi
        mov    dword ptr [esi], 0xc0010010
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0AD3:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    eax, dword ptr [edi + 0x272]
        dec    eax
        mov    dword ptr [edi + 0x272], eax
        jne    loc_1_0B25
        cmp    byte ptr [edi + 0x304], 0
        je     loc_1_0B00
        lea    eax, [edi + 0x43d]
        cmp    byte ptr [eax], 0
        je     loc_1_0B14
loc_1_0B00:
        cmp    dword ptr [edi + 0x28a], 0
        je     loc_1_0B25
        lea    eax, [edi + 0x43d]
        cmp    byte ptr [eax], 0
        jne    loc_1_0B25
loc_1_0B14:
        push   0
        add    edi, 0x31b
        push   edi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_0B25:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  090h
sub_1_0B30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0x456]
        mov    eax, dword ptr [edi + 0x26d]
        lea    ecx, [esi + 0x14]
        mov    dword ptr [esi + 0xc], eax
        push   ecx
        mov    eax, dword ptr [edi + 0x25d]
        mov    dword ptr [edi + 0x26d], esi
        mov    edx, dword ptr [esi]
        push   edx
        push   esi
        push   eax
        call   sub_2_00F0
loc_1_0B61:
        test   al, al
        jne    loc_1_0B9A
        push   0
        dec    dword ptr [edi + 0x272]
        mov    eax, dword ptr [esi + 0xc]
        push   0xc0010007
        mov    byte ptr [edi + 0x271], 0
        mov    dword ptr [edi + 0x26d], eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_D71E
        push   0
        push   0x75
        push   esi
        call   sub_1_D7EC
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0B9A:
        mov    byte ptr [edi + 0x271], 0
        mov    dword ptr [esi + 0x1c], 1
        push   0
        dec    dword ptr [edi + 0x272]
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_D71E
        pop    edi
        pop    esi
        pop    ebp
        ret    4
sub_1_0BC0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x26d]
        test   edx, edx
        je     loc_1_0BE3
loc_1_0BD3:
        mov    esi, dword ptr [edx + 0x18]
        cmp    ecx, esi
        jae    loc_1_0BDC
        mov    ecx, esi
loc_1_0BDC:
        mov    edx, dword ptr [edx + 0xc]
        test   edx, edx
        jne    loc_1_0BD3
loc_1_0BE3:
        test   ecx, ecx
        jne    loc_1_0BEC
        mov    ecx, 0xf2
loc_1_0BEC:
        pop    esi
        mov    dword ptr [eax + 0x317], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x20], 0
        mov    esi, dword ptr [ebx + 8]
        je     loc_1_0C1E
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0C1E:
        inc    dword ptr [esi + 0x272]
        inc    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [esi + 0x26d]
        cmp    ebx, eax
        jne    loc_1_0C3C
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x26d], eax
        jmp    loc_1_0C4F
loc_1_0C3C:
        cmp    dword ptr [eax + 0xc], ebx
        je     loc_1_0C49
loc_1_0C41:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], ebx
        jne    loc_1_0C41
loc_1_0C49:
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0C4F:
        push   0
        mov    dword ptr [esi + 0x452], ebx
        mov    eax, dword ptr [ebx + 0x14]
        mov    ecx, dword ptr [esi + 0x25d]
        push   eax
        push   ecx
        call   sub_2_01D0
loc_1_0C67:
        mov    edi, eax
        test   edi, edi
        jne    loc_1_0C8C
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    eax, 2
        je     loc_1_0C84
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        mov    edi, 0x103
        jmp    loc_1_0CB2
loc_1_0C84:
        sub    eax, 2
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_1_0CB2
loc_1_0C8C:
        cmp    edi, 0x103
        jne    loc_1_0C9E
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        jmp    loc_1_0CB2
loc_1_0C9E:
        cmp    edi, 0xc001000e
        jne    loc_1_0CAF
        mov    byte ptr [ebx + 0x20], 1
        mov    edi, 0x103
loc_1_0CAF:
        dec    dword ptr [ebx + 0x1c]
loc_1_0CB2:
        cmp    dword ptr [ebx + 0x1c], 0
        jne    loc_1_0CE4
        mov    eax, dword ptr [esi + 0x317]
        cmp    dword ptr [ebx + 0x18], eax
        jne    loc_1_0CC9
        push   esi
        call   sub_1_0BC0
loc_1_0CC9:
        cmp    dword ptr [esi + 0x26d], 0
        jne    loc_1_0CE9
        push   esi
        lea    eax, [esi + 0x20c]
        push   0                             ; -> loc_1_2310
        push   eax
        call   sub_1_D7DA
loc_1_0CE4:
        mov    edi, 0x103
loc_1_0CE9:
        cmp    byte ptr [esi + 0x451], 0
        jne    loc_1_0D0A
        push   0x7530
        lea    eax, [esi + 0x45e]
        mov    byte ptr [esi + 0x451], 1
        push   eax
        call   sub_1_D7C2
loc_1_0D0A:
        dec    dword ptr [esi + 0x272]
        jne    loc_1_0D4B
        cmp    byte ptr [esi + 0x304], 0
        je     loc_1_0D26
        lea    eax, [esi + 0x43d]
        cmp    byte ptr [eax], 0
        je     loc_1_0D3A
loc_1_0D26:
        cmp    dword ptr [esi + 0x28a], 0
        je     loc_1_0D4B
        lea    eax, [esi + 0x43d]
        cmp    byte ptr [eax], 0
        jne    loc_1_0D4B
loc_1_0D3A:
        push   0
        add    esi, 0x31b
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_0D4B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [edi + 0x1c]
        inc    dword ptr [esi + 0x272]
        cmp    byte ptr [edi + 0x20], 0
        je     loc_1_0D87
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_0DC9
loc_1_0D87:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        jne    loc_1_0D9E
        push   eax
        push   edi
        push   esi
        call   sub_1_11D0
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0DC9
loc_1_0D9E:
        cmp    ecx, 1
        jne    loc_1_0DC2
        cmp    byte ptr [esi + 0x303], 0
        je     loc_1_0DB5
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_0DC9
loc_1_0DB5:
        push   eax
        push   edi
        push   esi
        call   sub_1_1220
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0DC9
loc_1_0DC2:
        mov    dword ptr [ebp - 4], 0x10001
loc_1_0DC9:
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_0E2B
        mov    eax, dword ptr [ebx + 0x26d]
        cmp    edi, eax
        jne    loc_1_0DE6
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x26d], eax
        jmp    loc_1_0DFF
loc_1_0DE6:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_0DF3
loc_1_0DEB:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_0DEB
loc_1_0DF3:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [ebx + 0x452], edi
loc_1_0DFF:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x317], eax
        jne    loc_1_0E10
        push   ebx
        call   sub_1_0BC0
loc_1_0E10:
        cmp    dword ptr [ebx + 0x26d], 0
        jne    loc_1_0E2B
        push   ebx
        push   0                             ; -> loc_1_2310
        add    ebx, 0x20c
        push   ebx
        call   sub_1_D7DA
loc_1_0E2B:
        mov    eax, dword ptr [esi + 0x272]
        dec    eax
        mov    dword ptr [esi + 0x272], eax
        jne    loc_1_0E73
        cmp    byte ptr [esi + 0x304], 0
        je     loc_1_0E4E
        lea    eax, [esi + 0x43d]
        cmp    byte ptr [eax], 0
        je     loc_1_0E62
loc_1_0E4E:
        cmp    dword ptr [esi + 0x28a], 0
        je     loc_1_0E73
        lea    eax, [esi + 0x43d]
        cmp    byte ptr [eax], 0
        jne    loc_1_0E73
loc_1_0E62:
        push   0
        add    esi, 0x31b
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_0E73:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_0E80:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        lea    ecx, [ebp - 8]
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   edi
        xor    esi, esi
        mov    edi, 4
        mov    dword ptr [ebp - 0xc], esi
        mov    dword ptr [ebp - 0x10], 1
        cmp    eax, 0x2010101
        ja     loc_1_0EBE
        je     loc_1_107F
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_0ECF
        jmp    dword ptr [eax*4]             ; -> obj1:0x116C
loc_1_0EBE:
        sub    eax, 0x2010102
        cmp    eax, 3
        ja     loc_1_0ECF
        jmp    dword ptr [eax*4]             ; -> obj1:0x11B8
loc_1_0ECF:
        mov    esi, 0xc00000bb
        jmp    loc_1_112E
loc_1_0ED9:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_0EEE
        mov    ecx, 0                        ; -> obj1:0xFB14
        mov    edi, 0x60
        jmp    loc_1_112E
loc_1_0EEE:
        mov    ecx, 0                        ; -> loc_1_FA98
        mov    edi, 0x7c
        jmp    loc_1_112E
loc_1_0EFD:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], 4
        cmp    byte ptr [eax + 0x303], 0
        jne    loc_1_0F17
        mov    dword ptr [ebp - 0xc], 0
loc_1_0F17:
        lea    ecx, [ebp - 0xc]
        mov    edi, 4
        jmp    loc_1_112E
loc_1_0F24:
        lea    ecx, [ebp - 0x10]
        mov    edi, 4
        jmp    loc_1_112E
loc_1_0F31:
        mov    dword ptr [ebp - 8], 0xf2
        jmp    loc_1_112E
loc_1_0F3D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x36]
        and    eax, 0xffff
        sub    eax, 0xe
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_112E
loc_1_0F54:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x44]
        test   al, 6
        jne    loc_1_0F6B
        mov    dword ptr [ebp - 8], 0x9c40
        jmp    loc_1_112E
loc_1_0F6B:
        mov    dword ptr [ebp - 8], 0x27100
        jmp    loc_1_112E
loc_1_0F77:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 0x204]
        xor    ebx, ebx
        mov    bx, word ptr [eax + 0x3a]
        imul   edx, ebx
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_112E
loc_1_0F94:
        mov    eax, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    bx, word ptr [eax + 0x16]
        xor    edx, edx
        shl    ebx, 0xa
        mov    dx, word ptr [eax + 0x204]
        mov    ax, word ptr [eax + 0x3a]
        and    eax, 0xffff
        imul   edx, eax
        sub    ebx, edx
        mov    dword ptr [ebp - 8], ebx
        jmp    loc_1_112E
loc_1_0FBF:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 0x36]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_112E
loc_1_0FD0:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 0x36]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_112E
loc_1_0FE1:
        mov    ecx, 0                        ; -> loc_1_FC5C
        mov    edi, 0xd
        jmp    loc_1_112E
loc_1_0FF0:
        mov    ecx, 0                        ; -> loc_1_FC6C
        mov    edi, 0xe
        jmp    loc_1_112E
loc_1_0FFF:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_1019
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x25d]
        mov    ebx, dword ptr [edx + 0x10]
        mov    dword ptr [ebp - 8], ebx
        jmp    loc_1_112E
loc_1_1019:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x14]
        mov    ebx, dword ptr [edx + 8]
        mov    dword ptr [ebp - 8], ebx
        jmp    loc_1_112E
loc_1_102A:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_1041
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x317]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_112E
loc_1_1041:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_112E
loc_1_104F:
        lea    ecx, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_112E
loc_1_1062:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 0x36]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_112E
loc_1_1073:
        mov    dword ptr [ebp - 8], 0xe
        jmp    loc_1_112E
loc_1_107F:
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edi, 6
        push   ecx
        call   sub_1_D8CC
        lea    ecx, [ebp - 0x18]
        jmp    loc_1_112E
loc_1_109E:
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edi, 6
        push   ecx
        call   sub_1_D8CC
        lea    ecx, [ebp - 0x18]
        jmp    loc_1_112E
loc_1_10BA:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_10CE
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x14]
        mov    ebx, dword ptr [edx + 0xc]
        mov    dword ptr [ebp - 8], ebx
        jmp    loc_1_10DA
loc_1_10CE:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x9f]
        mov    dword ptr [ebp - 8], edx
loc_1_10DA:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 0x10
        and    edx, 0xff00
        or     edx, eax
        mov    ebx, dword ptr [ebp - 8]
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        shr    ebx, 0x10
        and    eax, 0xff0000
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_112E
loc_1_1108:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x99]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_112E
loc_1_1116:
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 0x40], 1
        jne    loc_1_112E
        mov    dword ptr [ebp - 8], 0
loc_1_112E:
        test   esi, esi
        jne    loc_1_1160
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    eax, edi
        jae    loc_1_1150
        sub    edi, eax
        mov    esi, 0xc0010014
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], edi
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_1150:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   ecx
        push   eax
        call   sub_1_D8CC
        mov    eax, dword ptr [ebp + 0x24]
        add    dword ptr [eax], edi
loc_1_1160:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  090h
        ; ---- dispatch table (23 entries) ----
        dd  loc_1_0ED9
        dd  loc_1_0EFD
        dd  loc_1_0F24
        dd  loc_1_0F24
        dd  loc_1_0F31
        dd  loc_1_0F3D
        dd  loc_1_0F54
        dd  loc_1_0F77
        dd  loc_1_0F94
        dd  loc_1_0FBF
        dd  loc_1_0FD0
        dd  loc_1_0FE1
        dd  loc_1_0FF0
        dd  loc_1_0FFF
        dd  loc_1_102A
        dd  loc_1_104F
        dd  loc_1_1062
        dd  loc_1_0ECF
        dd  loc_1_1073
        dd  loc_1_109E
        dd  loc_1_10BA
        dd  loc_1_1108
        dd  loc_1_1116
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_11D0:
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
        call   sub_1_0E80
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1220:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        xor    edi, edi
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 4
        mov    eax, dword ptr [esi + 0x14]
        je     loc_1_1254
        mov    eax, 0xc0010014
        mov    dword ptr [esi + 0x20], edi
        mov    dword ptr [esi + 0x24], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1254:
        cmp    eax, 0x2010103
        ja     loc_1_127C
        je     loc_1_1369
        cmp    eax, 0x1010e
        je     loc_1_1299
        cmp    eax, 0x1010f
        je     loc_1_12DA
        cmp    eax, 0x10112
        je     loc_1_1341
        jmp    loc_1_1287
loc_1_127C:
        cmp    eax, 0x2010104
        je     loc_1_137A
loc_1_1287:
        mov    edi, 0xc00000bb
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_138B
loc_1_1299:
        push   4
        lea    eax, [ebp - 0xc]
        push   ecx
        push   eax
        call   sub_1_D8CC
        test   byte ptr [ebp - 0xc], 0x46
        je     loc_1_12C3
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        mov    edi, 0xc00000bb
        jmp    loc_1_138B
loc_1_12C3:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   ebx
        call   sub_1_13B0
        jmp    loc_1_1389
loc_1_12DA:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_D8CC
        cmp    dword ptr [ebp - 4], 0xf2
        ja     loc_1_133A
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebx + 0x317]
        cmp    dword ptr [ebp - 4], edx
        jbe    loc_1_130B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 0x317], eax
        jmp    loc_1_132B
loc_1_130B:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x18]
        cmp    edx, eax
        jne    loc_1_1328
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_1328
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        mov    dword ptr [ecx + 0x18], eax
        call   sub_1_0BC0
        jmp    loc_1_132E
loc_1_1328:
        mov    eax, dword ptr [ebp - 4]
loc_1_132B:
        mov    dword ptr [ecx + 0x18], eax
loc_1_132E:
        mov    eax, dword ptr [ebx + 0x317]
        mov    word ptr [ebx + 0x3c], ax
        jmp    loc_1_138B
loc_1_133A:
        mov    edi, 0xc0010014
        jmp    loc_1_138B
loc_1_1341:
        cmp    dword ptr [ebp - 8], 4
        je     loc_1_1356
        mov    edi, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_138B
loc_1_1356:
        push   4
        xor    edi, edi
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x71
        push   ecx
        call   sub_1_D8CC
        jmp    loc_1_138B
loc_1_1369:
        push   ecx
        mov    ebx, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   ebx
        call   sub_1_29C0
        jmp    loc_1_1389
loc_1_137A:
        push   ecx
        mov    ebx, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   ebx
        call   sub_1_2990
loc_1_1389:
        mov    edi, eax
loc_1_138B:
        test   edi, edi
        jne    loc_1_139C
        mov    dword ptr [esi + 0x24], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x20], eax
loc_1_139C:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_13B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_13E5
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [esi + 0x14]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [eax + 0x25d]
        push   ecx
        call   sub_2_04F0
loc_1_13DD:
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
loc_1_13E5:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
sub_1_13F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0xc], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 8]
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        push   edx
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], ecx
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        mov    eax, dword ptr [esi + 0x14]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_0E80
        cmp    eax, 0xc00000bb
        mov    edi, eax
        jne    loc_1_14E9
        xor    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x2010105
        ja     loc_1_1463
        je     loc_1_14BB
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_1463
        jmp    dword ptr [eax*4]             ; -> obj1:0x1500
loc_1_1463:
        mov    edi, 0xc00000bb
        jmp    loc_1_14D0
loc_1_146A:
        mov    eax, dword ptr [ebx + 0x2a3]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_14D0
loc_1_1475:
        mov    eax, dword ptr [ebx + 0x2a7]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_14D0
loc_1_1480:
        mov    eax, dword ptr [ebx + 0x2ab]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_14D0
loc_1_148B:
        cmp    byte ptr [ebx + 0x50], 0
        je     loc_1_1498
        mov    edi, 0xc00000bb
        jmp    loc_1_14D0
loc_1_1498:
        mov    eax, dword ptr [ebx + 0x2cb]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_14D0
loc_1_14A3:
        cmp    byte ptr [ebx + 0x50], 0
        je     loc_1_14B0
        mov    edi, 0xc00000bb
        jmp    loc_1_14D0
loc_1_14B0:
        mov    eax, dword ptr [ebx + 0x2cf]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_14D0
loc_1_14BB:
        mov    dword ptr [ebp - 4], 1
        cmp    word ptr [ebx + 0x40], 1
        jne    loc_1_14D0
        mov    dword ptr [ebp - 4], 0
loc_1_14D0:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_14E9
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_D8CC
        add    dword ptr [ebp - 8], 4
loc_1_14E9:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_146A
        dd  loc_1_1475
        dd  loc_1_1480
        dd  loc_1_148B
        dd  loc_1_14A3
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1520:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x10113
        ja     loc_1_1547
        je     loc_1_1568
        cmp    eax, 0x10101
        jb     loc_1_1563
        cmp    eax, 0x10111
        jbe    loc_1_1568
        jmp    loc_1_1563
loc_1_1547:
        cmp    eax, 0x2010105
        ja     loc_1_1563
        cmp    eax, 0x2010101
        jae    loc_1_1568
        cmp    eax, 0x20101
        jb     loc_1_1563
        cmp    eax, 0x20104
        jbe    loc_1_1568
loc_1_1563:
        mov    esi, 0xc00000bb
loc_1_1568:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x272]
        test   esi, esi
        jne    loc_1_157E
        push   ecx
        push   edi
        call   sub_1_13F0
        mov    esi, eax
loc_1_157E:
        mov    eax, dword ptr [edi + 0x272]
        dec    eax
        mov    dword ptr [edi + 0x272], eax
        jne    loc_1_15C6
        cmp    byte ptr [edi + 0x304], 0
        je     loc_1_15A1
        lea    eax, [edi + 0x43d]
        cmp    byte ptr [eax], 0
        je     loc_1_15B5
loc_1_15A1:
        cmp    dword ptr [edi + 0x28a], 0
        je     loc_1_15C6
        lea    eax, [edi + 0x43d]
        cmp    byte ptr [eax], 0
        jne    loc_1_15C6
loc_1_15B5:
        push   0
        add    edi, 0x31b
        push   edi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_15C6:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_15D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_64BD
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x45e]
        push   eax
        push   ecx
        call   sub_1_D7C8
        lea    ecx, [ebp - 1]
        lea    edx, [esi + 0x46e]
        push   ecx
        push   edx
        call   sub_1_D7C8
        push   0x9c4
        call   sub_1_D7E0
        mov    byte ptr [esi + 0x43f], 1
        cmp    esi, dword ptr [0x42]         ; -> obj1:0xFA8A
        jne    loc_1_1626
        mov    eax, dword ptr [esi + 0x269]
        mov    dword ptr [0x42], eax         ; -> obj1:0xFA8A
        jmp    loc_1_1654
loc_1_1626:
        mov    ecx, dword ptr [0x42]         ; -> obj1:0xFA8A
        cmp    dword ptr [ecx + 0x269], esi
        je     loc_1_1642
loc_1_1634:
        mov    ecx, dword ptr [ecx + 0x269]
        cmp    dword ptr [ecx + 0x269], esi
        jne    loc_1_1634
loc_1_1642:
        mov    eax, dword ptr [ecx + 0x269]
        mov    edx, dword ptr [eax + 0x269]
        mov    dword ptr [ecx + 0x269], edx
loc_1_1654:
        lea    eax, [esi + 0x20c]
        push   eax
        call   sub_1_D7D4
        mov    eax, dword ptr [esi + 0x25d]
        push   eax
        call   sub_2_00D0
loc_1_166C:
        mov    eax, dword ptr [esi + 0x4c]
        push   eax
        call   sub_1_D718
        push   0
        push   0x47a
        push   esi
        call   sub_1_D7EC
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1690:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0xFA48
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_D70C
        push   0
        mov    ecx, dword ptr [4]            ; -> obj1:0xFA4C
        push   ecx
        call   sub_1_D700
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_16C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        mov    dword ptr [ebp - 4], 0x103
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0xc]
        lea    ebx, [edi + 0x24]
        push   ecx
        push   0
        push   0
        push   edi
        call   sub_1_D810
        cmp    dword ptr [ebp - 8], 0xc
        jb     loc_1_18B9
        xor    eax, eax
        mov    ax, word ptr [esi + 0x36]
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_18B9
        cmp    byte ptr [esi + 0x305], 0
        je     loc_1_171F
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_171F:
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x20], 0
        je     loc_1_1736
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1736:
        mov    eax, dword ptr [ebp + 8]
        push   edi
        push   eax
        push   esi
        inc    dword ptr [eax + 0x1c]
        inc    dword ptr [esi + 0x272]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx], 0
        call   sub_1_1910
        cmp    byte ptr [ebx + 9], 0
        je     loc_1_1787
        cmp    dword ptr [esi + 0x28a], 0
        jne    loc_1_1773
        mov    dword ptr [esi + 0x28e], edi
        mov    dword ptr [esi + 0x28a], edi
        jmp    loc_1_1847
loc_1_1773:
        mov    eax, dword ptr [esi + 0x28e]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x28e], edi
        jmp    loc_1_1847
loc_1_1787:
        cmp    dword ptr [esi + 0x27a], 0
        je     loc_1_17A4
        mov    eax, dword ptr [esi + 0x27e]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x27e], edi
        jmp    loc_1_1841
loc_1_17A4:
        cmp    dword ptr [esi + 0x282], 0
        jne    loc_1_17BD
        xor    ebx, ebx
        mov    dword ptr [esi + 0x286], edi
        mov    dword ptr [esi + 0x282], edi
        jmp    loc_1_17CC
loc_1_17BD:
        mov    ebx, dword ptr [esi + 0x286]
        mov    dword ptr [ebx + 0x24], edi
        mov    dword ptr [esi + 0x286], edi
loc_1_17CC:
        push   esi
        push   edi
        call   sub_1_3760
        and    eax, 0xffff
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 6
        jne    loc_1_1813
        test   ebx, ebx
        jne    loc_1_17FA
        mov    dword ptr [esi + 0x286], 0
        mov    dword ptr [esi + 0x282], 0
        jmp    loc_1_1807
loc_1_17FA:
        mov    dword ptr [esi + 0x286], ebx
        mov    dword ptr [ebx + 0x24], 0
loc_1_1807:
        mov    dword ptr [esi + 0x27a], edi
        mov    dword ptr [esi + 0x27e], edi
loc_1_1813:
        cmp    dword ptr [ebp - 4], 8
        jne    loc_1_1847
        mov    byte ptr [esi + 0x303], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    ax, word ptr [esi + 0x40]
        xor    ecx, ecx
loc_1_1832:
        cmp    ax, 1
        je     loc_1_1841
        inc    ecx
        cmp    ecx, 0xfffff
        jb     loc_1_1832
loc_1_1841:
        cmp    dword ptr [ebp - 4], 8
        je     loc_1_189F
loc_1_1847:
        mov    edi, 0x103
        mov    eax, dword ptr [esi + 0x272]
        dec    eax
        mov    dword ptr [esi + 0x272], eax
        jne    loc_1_1894
        cmp    byte ptr [esi + 0x304], 0
        je     loc_1_186F
        lea    eax, [esi + 0x43d]
        cmp    byte ptr [eax], 0
        je     loc_1_1883
loc_1_186F:
        cmp    dword ptr [esi + 0x28a], 0
        je     loc_1_1894
        lea    eax, [esi + 0x43d]
        cmp    byte ptr [eax], 0
        jne    loc_1_1894
loc_1_1883:
        push   0
        add    esi, 0x31b
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_1894:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_189F:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        dec    dword ptr [esi + 0x272]
        pop    esi
        pop    ebx
        dec    dword ptr [eax + 0x1c]
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_18B9:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_18D0:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        cmp    edx, esi
        je     loc_1_18F2
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_18E5:
        mov    bl, byte ptr [eax + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    loc_1_18FA
        inc    esi
        cmp    esi, edx
        jb     loc_1_18E5
loc_1_18F2:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_18FA:
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1910:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    esi, [eax + 0x24]
        push   6
        lea    ecx, [ebp - 8]
        mov    byte ptr [esi + 9], 0
        mov    byte ptr [esi + 8], 0
        push   2
        push   eax
        push   ecx
        call   sub_1_2120
        mov    edi, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp - 4]
        mov    eax, dword ptr [edi + 0x25d]
        cmp    byte ptr [edi + 0x28], cl
        mov    ebx, dword ptr [eax + 0x10]
        jne    loc_1_1973
        push   6
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [edi + 0x24]
        push   ecx
        call   sub_1_18D0
        test   eax, eax
        jne    loc_1_1973
        mov    eax, ebx
        and    al, 1
        mov    byte ptr [esi + 9], al
        je     loc_1_1973
        mov    byte ptr [esi + 8], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1973:
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax + 0x71], 2
        je     loc_1_1989
        mov    byte ptr [esi + 8], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1989:
        test   bl, 0x20
        je     loc_1_199B
        mov    byte ptr [esi + 8], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_199B:
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [4], al              ; -> obj1:0xFA40
        jne    loc_1_19C9
        push   6
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> loc_1_FA3C
        call   sub_1_18D0
        test   eax, eax
        jne    loc_1_19C9
        and    bl, 8
        pop    edi
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_19C9:
        test   byte ptr [ebp - 8], 0x80
        je     loc_1_1A4D
        test   bh, 0x20
        je     loc_1_19E1
        mov    byte ptr [esi + 8], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_19E1:
        test   bh, 0x40
        je     loc_1_1A10
        cmp    byte ptr [ebp - 8], 0xc0
        jne    loc_1_1A10
        cmp    byte ptr [ebp - 7], 0
        jne    loc_1_1A10
        mov    ax, word ptr [edi + 0x9f]
        test   word ptr [ebp - 6], ax
        jne    loc_1_1A0C
        mov    ax, word ptr [edi + 0xa1]
        test   word ptr [ebp - 4], ax
        je     loc_1_1A10
loc_1_1A0C:
        mov    byte ptr [esi + 8], 1
loc_1_1A10:
        test   bh, 0x10
        je     loc_1_1A51
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xffff00ff
        cmp    word ptr [edi + 0x97], ax
        jne    loc_1_1A51
        mov    ax, word ptr [ebp - 6]
        cmp    word ptr [edi + 0x99], ax
        jne    loc_1_1A51
        mov    ax, word ptr [ebp - 4]
        cmp    word ptr [edi + 0x9b], ax
        jne    loc_1_1A51
        mov    byte ptr [esi + 8], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1A4D:
        mov    byte ptr [esi + 8], 0
loc_1_1A51:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1A60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x20], 0
        mov    esi, dword ptr [edi + 8]
        je     loc_1_1A81
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1A81:
        cmp    byte ptr [esi + 0x304], 0
        je     loc_1_1A94
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1A94:
        cmp    byte ptr [esi + 0x305], 0
        je     loc_1_1AA7
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1AA7:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x46e]
        push   eax
        push   ecx
        call   sub_1_D7C8
        inc    dword ptr [edi + 0x1c]
        inc    dword ptr [esi + 0x272]
        mov    byte ptr [esi + 0x304], 1
        mov    dword ptr [esi + 0x30b], edi
        mov    eax, dword ptr [esi + 0x272]
        dec    eax
        mov    dword ptr [esi + 0x272], eax
        jne    loc_1_1B15
        cmp    byte ptr [esi + 0x304], 0
        je     loc_1_1AF0
        lea    eax, [esi + 0x43d]
        cmp    byte ptr [eax], 0
        je     loc_1_1B04
loc_1_1AF0:
        cmp    dword ptr [esi + 0x28a], 0
        je     loc_1_1B15
        lea    eax, [esi + 0x43d]
        cmp    byte ptr [eax], 0
        jne    loc_1_1B15
loc_1_1B04:
        push   0
        add    esi, 0x31b
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_1B15:
        mov    eax, 0x103
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1B30:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        xor    edi, edi
        push   esi
        call   sub_1_31A0
        test   ax, ax
        je     loc_1_1B79
        mov    ebx, 1
loc_1_1B4F:
        test   edi, edi
        jne    loc_1_1B96
        push   0
        xor    edi, edi
        push   0
        mov    byte ptr [esi + 0x303], bl
        push   esi
        push   0
        call   sub_1_29F0
        mov    ax, word ptr [esi + 0x40]
loc_1_1B6B:
        cmp    ax, bx
        je     loc_1_1B82
        inc    edi
        cmp    edi, 0xfffff
        jb     loc_1_1B6B
loc_1_1B79:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_1B82:
        push   esi
        call   sub_1_31A0
        test   ax, ax
        jne    loc_1_1B4F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_1B96:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1BB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 0x1c]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1BC0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_2A60
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1BE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x272]
loc_1_1BF0:
        mov    byte ptr [esi + 0x450], bl
        cmp    byte ptr [esi + 0x43d], bl
        jne    loc_1_1C1E
        mov    byte ptr [esi + 0x43d], 1
        push   ebx
        push   ebx
        push   esi
        push   ebx
        call   sub_1_1D50
        cmp    dword ptr [esi + 0x28a], ebx
        je     loc_1_1C1E
        cmp    byte ptr [esi + 0x43d], bl
        je     loc_1_1BF0
loc_1_1C1E:
        mov    eax, dword ptr [esi + 0x272]
        dec    eax
        mov    dword ptr [esi + 0x272], eax
        jne    loc_1_1C66
        cmp    byte ptr [esi + 0x304], 0
        je     loc_1_1C3F
        cmp    byte ptr [esi + 0x43d], 0
        je     loc_1_1C51
loc_1_1C3F:
        cmp    dword ptr [esi + 0x28a], 0
        je     loc_1_1C66
        cmp    byte ptr [esi + 0x43d], 0
        jne    loc_1_1C66
loc_1_1C51:
        push   0
        add    esi, 0x31b
        push   esi
        mov    byte ptr [esi + 0x122], 1
        call   sub_1_D7C2
loc_1_1C66:
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h
sub_1_1C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        mov    dword ptr [esi + 0x292], ebx
        mov    byte ptr [esi + 0x296], 1
        push   0
        push   0
        push   ebx
        call   sub_1_D810
        cmp    dword ptr [ebp - 4], 6
        jb     loc_1_1D40
        mov    edi, dword ptr [esi + 0x317]
        mov    eax, dword ptr [ebp - 4]
        add    edi, 0xe
        cmp    edi, eax
        jb     loc_1_1CBA
        mov    edi, eax
loc_1_1CBA:
        push   edi
        lea    eax, [esi + 0x32b]
        push   0
        push   ebx
        push   eax
        call   sub_1_2120
        test   byte ptr [esi + 0x333], 0x80
        je     loc_1_1CE1
        mov    cl, byte ptr [esi + 0x339]
        and    ecx, 0x1f
        add    ecx, 0xe
        jmp    loc_1_1CE6
loc_1_1CE1:
        mov    ecx, 0xe
loc_1_1CE6:
        cmp    dword ptr [ebp - 4], 0xe
        mov    dword ptr [esi + 0x29f], ecx
        jae    loc_1_1D19
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [esi + 0x32b]
        push   0
        mov    edx, dword ptr [esi + 0x25d]
        push   eax
        push   ecx
        push   esi
        push   edx
        call   sub_1_D260
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1D19:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, ecx
        sub    eax, ecx
        push   eax
        push   edi
        lea    eax, [ecx + esi]
        mov    edx, dword ptr [esi + 0x25d]
        add    eax, 0x32b
        push   eax
        push   ecx
        lea    ecx, [esi + 0x32b]
        push   ecx
        push   esi
        push   edx
        call   sub_1_D260
loc_1_1D40:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        inc    dword ptr [esi + 0x272]
        call   sub_1_2A60
        cmp    dword ptr [esi + 0x28a], 0
        je     loc_1_1E26
loc_1_1D72:
        mov    byte ptr [esi + 0x450], 0
        mov    ebx, dword ptr [esi + 0x28a]
        cmp    dword ptr [esi + 0x28e], ebx
        jne    loc_1_1D91
        mov    dword ptr [esi + 0x28e], 0
loc_1_1D91:
        mov    eax, dword ptr [ebx + 0x24]
        push   ebx
        push   esi
        mov    dword ptr [esi + 0x28a], eax
        call   sub_1_1C70
        inc    dword ptr [esi + 0x2a3]
        mov    edi, dword ptr [ebx + 0x28]
        mov    eax, dword ptr [ebx + 0x2e]
        push   eax
        push   ebx
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_D72A
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_1E19
        mov    eax, dword ptr [ebx + 0x26d]
        cmp    edi, eax
        jne    loc_1_1DD4
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x26d], eax
        jmp    loc_1_1DED
loc_1_1DD4:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_1DE1
loc_1_1DD9:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_1DD9
loc_1_1DE1:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [ebx + 0x452], edi
loc_1_1DED:
        mov    eax, dword ptr [ebx + 0x317]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_1DFE
        push   ebx
        call   sub_1_0BC0
loc_1_1DFE:
        cmp    dword ptr [ebx + 0x26d], 0
        jne    loc_1_1E19
        push   ebx
        push   0                             ; -> loc_1_2310
        add    ebx, 0x20c
        push   ebx
        call   sub_1_D7DA
loc_1_1E19:
        cmp    dword ptr [esi + 0x28a], 0
        jne    loc_1_1D72
loc_1_1E26:
        mov    byte ptr [esi + 0x450], 0
        push   esi
        call   sub_1_3800
        and    eax, 0xffff
        cmp    eax, 8
        je     loc_1_1EF4
        cmp    dword ptr [esi + 0x28a], 0
        jne    loc_1_1D72
        cmp    byte ptr [esi + 0x296], 0
        je     loc_1_1E6A
        mov    byte ptr [esi + 0x296], 0
        mov    eax, dword ptr [esi + 0x25d]
        push   eax
        call   sub_1_D4B0
loc_1_1E6A:
        cmp    byte ptr [esi + 0x304], 0
        je     loc_1_20F4
        cmp    dword ptr [esi + 0x272], 1
        jne    loc_1_20F4
        cmp    byte ptr [esi + 0x305], 0
        jne    loc_1_20F4
        push   esi
        lea    eax, [esi + 0x20c]
        mov    byte ptr [esi + 0x304], 0
        mov    byte ptr [esi + 0x305], 1
        push   0                             ; -> sub_1_2A60
        push   eax
        call   sub_1_D7DA
        mov    edi, dword ptr [esi + 0x26d]
        test   edi, edi
        je     loc_1_1F6B
loc_1_1EBF:
        push   0
        inc    dword ptr [edi + 0x1c]
        push   0
        mov    eax, dword ptr [edi]
        push   0x40010004
        push   eax
        call   sub_1_D74E
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_1F60
        mov    eax, dword ptr [ebx + 0x26d]
        cmp    eax, edi
        jne    loc_1_1F1B
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x26d], eax
        jmp    loc_1_1F34
loc_1_1EF4:
        mov    byte ptr [esi + 0x303], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    byte ptr [esi + 0x43d], 0
        pop    edi
        dec    dword ptr [esi + 0x272]
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_1F1B:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_1F28
loc_1_1F20:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_1F20
loc_1_1F28:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [ebx + 0x452], edi
loc_1_1F34:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x317], eax
        jne    loc_1_1F45
        push   ebx
        call   sub_1_0BC0
loc_1_1F45:
        cmp    dword ptr [ebx + 0x26d], 0
        jne    loc_1_1F60
        push   ebx
        push   0                             ; -> loc_1_2310
        add    ebx, 0x20c
        push   ebx
        call   sub_1_D7DA
loc_1_1F60:
        mov    edi, dword ptr [edi + 0xc]
        test   edi, edi
        jne    loc_1_1EBF
loc_1_1F6B:
        push   esi
        mov    edi, 0xbb8
        call   sub_1_3130
loc_1_1F76:
        push   0x3e8
        call   sub_1_D7E0
        dec    edi
        jne    loc_1_1F76
        push   esi
        xor    edi, edi
        call   sub_1_2A60
loc_1_1F8B:
        push   esi
        call   sub_1_2B50
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_1FBB
        mov    byte ptr [esi + 0x303], 1
        test   edi, edi
        jne    loc_1_1FC2
        mov    ebx, 0xbb8
        mov    edi, ebx
loc_1_1FAC:
        push   0x3e8
        call   sub_1_D7E0
        dec    ebx
        jne    loc_1_1FAC
        jmp    loc_1_1F8B
loc_1_1FBB:
        mov    byte ptr [esi + 0x303], 0
loc_1_1FC2:
        push   0x7530
        lea    eax, [esi + 0x45e]
        push   eax
        call   sub_1_D7C2
        cmp    dword ptr [esi + 0x282], 0
        je     loc_1_200B
        mov    eax, dword ptr [esi + 0x27a]
        mov    ecx, dword ptr [esi + 0x286]
        mov    dword ptr [ecx + 0x24], eax
        mov    edx, dword ptr [esi + 0x282]
        mov    dword ptr [esi + 0x27a], edx
        mov    dword ptr [esi + 0x286], 0
        mov    dword ptr [esi + 0x282], 0
loc_1_200B:
        cmp    dword ptr [esi + 0x28a], 0
        je     loc_1_202F
        mov    eax, dword ptr [esi + 0x27a]
        mov    ecx, dword ptr [esi + 0x28e]
        mov    dword ptr [ecx + 0x24], eax
        mov    edx, dword ptr [esi + 0x28a]
        mov    dword ptr [esi + 0x27a], edx
loc_1_202F:
        mov    dword ptr [esi + 0x28e], 0
        mov    dword ptr [esi + 0x28a], 0
        cmp    dword ptr [esi + 0x27a], 0
        je     loc_1_20DF
loc_1_2050:
        mov    ecx, dword ptr [esi + 0x27a]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x27a], eax
        mov    edi, dword ptr [ecx + 0x28]
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_D72A
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_20D2
        mov    eax, dword ptr [ebx + 0x26d]
        cmp    eax, edi
        jne    loc_1_208D
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x26d], eax
        jmp    loc_1_20A6
loc_1_208D:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_209A
loc_1_2092:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_2092
loc_1_209A:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [ebx + 0x452], edi
loc_1_20A6:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x317], eax
        jne    loc_1_20B7
        push   ebx
        call   sub_1_0BC0
loc_1_20B7:
        cmp    dword ptr [ebx + 0x26d], 0
        jne    loc_1_20D2
        push   ebx
        push   0                             ; -> loc_1_2310
        add    ebx, 0x20c
        push   ebx
        call   sub_1_D7DA
loc_1_20D2:
        cmp    dword ptr [esi + 0x27a], 0
        jne    loc_1_2050
loc_1_20DF:
        xor    eax, eax
        pop    edi
        mov    dword ptr [esi + 0x27e], eax
        mov    byte ptr [esi + 0x43d], al
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_20F4:
        push   esi
        lea    eax, [esi + 0x20c]
        push   0                             ; -> sub_1_2AB0
        push   eax
        call   sub_1_D7DA
        mov    byte ptr [esi + 0x43d], 0
        pop    edi
        dec    dword ptr [esi + 0x272]
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2120:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        push   esi
        push   esi
        push   ecx
        call   sub_1_D810
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 4], esi
        je     loc_1_2174
loc_1_2145:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_D834
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     loc_1_2174
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_D858
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2145
loc_1_2174:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2185
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_2185:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_2194:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_21A3
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_21A6
loc_1_21A3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_21A6:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_D8CC
        cmp    esi, edi
        je     loc_1_21E2
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_D858
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_21ED
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_D834
        jmp    loc_1_2194
loc_1_21E2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_21ED:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2200:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0x10], ecx
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        add    esi, dword ptr [ecx + 0x29f]
        cmp    dword ptr [ecx + 0x292], 0
        je     loc_1_2296
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        mov    edi, esi
        push   0
        push   ecx
        call   sub_1_D810
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_2284
        xor    ebx, ebx
loc_1_2242:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_D834
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x10]
        push   ecx
        push   edi
        mov    eax, dword ptr [edx + 0x292]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        call   sub_1_2120
        add    edi, eax
        cmp    eax, dword ptr [ebp - 0xc]
        jb     loc_1_2284
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_D858
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_1_2242
loc_1_2284:
        sub    edi, esi
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], edi
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_2296:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    byte ptr [eax + 0x296], 0
        je     loc_1_22FE
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        mov    edx, dword ptr [ebp + 0x14]
        push   esi
        mov    eax, dword ptr [ebp + 0x1c]
        push   edx
        push   eax
        call   sub_1_3590
        mov    word ptr [ebp - 2], ax
        test   ax, ax
        je     loc_1_22F3
        int3   
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    word ptr [ebp - 2], 8
        mov    dword ptr [eax], 0
        jne    loc_1_22E5
        mov    eax, dword ptr [ebp - 0x10]
        push   0
        push   0
        push   eax
        mov    byte ptr [eax + 0x303], 1
        push   0
        call   sub_1_29F0
loc_1_22E5:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_22F3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_22FE:
        mov    eax, 0xc0010013
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h
loc_1_2310:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0x451], 1
        call   sub_1_3130
        test   ax, ax
        jne    loc_1_2348
        push   0x7530
        lea    eax, [esi + 0x45e]
        push   eax
        call   sub_1_D7C2
        mov    byte ptr [esi + 0x303], 0
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_2348:
        push   0x4e20
        add    esi, 0x45e
        push   esi
        call   sub_1_D7C2
        xor    al, al
        pop    esi
        pop    ebp
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x450], 0
        je     loc_1_2476
        mov    edi, dword ptr [esi + 0x27a]
        test   edi, edi
        je     loc_1_2476
        cmp    byte ptr [esi + 0x304], 0
        jne    loc_1_248B
        cmp    byte ptr [esi + 0x305], 0
        jne    loc_1_248B
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x27a], eax
        test   eax, eax
        jne    loc_1_23B8
        mov    dword ptr [esi + 0x27e], 0
loc_1_23B8:
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_1_3760
        and    eax, 0xffff
loc_1_23C9:
        cmp    eax, 6
        je     loc_1_23F0
        test   eax, eax
        je     loc_1_241A
        cmp    eax, 8
        jne    loc_1_244E
        push   0
        mov    byte ptr [esi + 0x303], bl
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    eax, 6
        jmp    loc_1_23C9
loc_1_23F0:
        mov    byte ptr [esi + 0x303], 1
        mov    eax, dword ptr [esi + 0x27a]
        test   eax, eax
        je     loc_1_240C
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [esi + 0x27a], edi
        jmp    loc_1_244E
loc_1_240C:
        mov    dword ptr [esi + 0x27a], edi
        mov    dword ptr [esi + 0x27e], edi
        jmp    loc_1_244E
loc_1_241A:
        mov    byte ptr [esi + 0x450], 0
        cmp    dword ptr [esi + 0x282], 0
        jne    loc_1_2438
        mov    dword ptr [esi + 0x286], edi
        mov    dword ptr [esi + 0x282], edi
        jmp    loc_1_2447
loc_1_2438:
        mov    eax, dword ptr [esi + 0x286]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x286], edi
loc_1_2447:
        mov    dword ptr [edi + 0x24], 0
loc_1_244E:
        add    esi, 0x444
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        mov    byte ptr [esi + 0xc], 1
        call   sub_1_D7C8
        push   0x2710
        push   esi
        call   sub_1_D7C2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_2476:
        push   0x2710
        add    esi, 0x444
        push   esi
        mov    byte ptr [esi + 0xc], 1
        call   sub_1_D7C2
loc_1_248B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_24A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [esi + 0x26d]
        mov    al, byte ptr [esi + 0x271]
        cmp    al, 1
        jne    loc_1_2512
        cmp    word ptr [esi + 0x40], 1
        je     loc_1_250A
        push   esi
        call   sub_1_2B50
        test   ax, ax
        jne    loc_1_2789
        mov    eax, dword ptr [esi + 0x307]
        test   eax, eax
        lea    ecx, [eax + 1]
        mov    dword ptr [esi + 0x307], ecx
        jne    loc_1_2789
        push   0x7530
        add    esi, 0x45e
        push   esi
        mov    byte ptr [esi - 0x15b], 0
        call   sub_1_D7C2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_250A:
        cmp    al, 1
        je     loc_1_2789
loc_1_2512:
        cmp    byte ptr [esi + 0x451], 1
        jne    loc_1_260B
        mov    byte ptr [esi + 0x451], 0
        cmp    dword ptr [esi + 0x282], 0
        je     loc_1_2568
        xor    edi, edi
loc_1_2531:
        mov    edx, dword ptr [esi + 0x282]
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x282], eax
        test   eax, eax
        jne    loc_1_254C
        mov    dword ptr [esi + 0x286], edi
loc_1_254C:
        mov    ebx, dword ptr [ecx + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_D72A
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x282], edi
        jne    loc_1_2531
loc_1_2568:
        cmp    dword ptr [esi + 0x27a], 0
        je     loc_1_25AA
        xor    edi, edi
loc_1_2573:
        mov    edx, dword ptr [esi + 0x27a]
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x27a], eax
        test   eax, eax
        jne    loc_1_258E
        mov    dword ptr [esi + 0x27e], edi
loc_1_258E:
        mov    ebx, dword ptr [ecx + 4]
        push   0xc001000c
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_D72A
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x27a], edi
        jne    loc_1_2573
loc_1_25AA:
        mov    eax, dword ptr [esi + 0x452]
        xor    ebx, ebx
        push   ebx
        mov    dword ptr [eax + 0x1c], ebx
        mov    byte ptr [esi + 0x451], bl
        mov    byte ptr [esi + 0x303], bl
        mov    eax, dword ptr [esi + 0x452]
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_D724
        push   ebx
        mov    ecx, dword ptr [esi + 0x452]
        push   ebx
        push   0x40010007
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_D74E
        mov    ecx, dword ptr [esi + 0x452]
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_D754
        push   ebx
        mov    ecx, dword ptr [esi + 0x452]
        push   0x75
        push   ecx
        call   sub_1_D7EC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_260B:
        mov    al, byte ptr [esi + 0x305]
        cmp    al, 1
        jne    loc_1_2779
        cmp    word ptr [esi + 0x40], 1
        je     loc_1_262B
        test   al, al
        je     loc_1_262B
        mov    byte ptr [esi + 0x303], 1
loc_1_262B:
        test   edi, edi
        je     loc_1_26D5
loc_1_2633:
        mov    eax, 0
        push   4
        inc    dword ptr [edi + 0x1c]
        cmp    byte ptr [esi + 0x303], 1
        adc    eax, -1
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   0x40010005
        push   ecx
        call   sub_1_D74E
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_D754
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_26CA
        mov    eax, dword ptr [ebx + 0x26d]
        cmp    edi, eax
        jne    loc_1_2685
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x26d], eax
        jmp    loc_1_269E
loc_1_2685:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_2692
loc_1_268A:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_268A
loc_1_2692:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [ebx + 0x452], edi
loc_1_269E:
        mov    eax, dword ptr [ebx + 0x317]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_26AF
        push   ebx
        call   sub_1_0BC0
loc_1_26AF:
        cmp    dword ptr [ebx + 0x26d], 0
        jne    loc_1_26CA
        push   ebx
        push   0                             ; -> loc_1_2310
        add    ebx, 0x20c
        push   ebx
        call   sub_1_D7DA
loc_1_26CA:
        mov    edi, dword ptr [edi + 0xc]
        test   edi, edi
        jne    loc_1_2633
loc_1_26D5:
        mov    byte ptr [esi + 0x305], 0
        mov    eax, 0
        mov    ecx, dword ptr [esi + 0x30b]
        cmp    byte ptr [esi + 0x303], 1
        adc    eax, -1
        mov    edx, dword ptr [ecx]
        and    eax, 0xc0000001
        push   eax
        push   edx
        call   sub_1_D736
        mov    ecx, dword ptr [esi + 0x30b]
        mov    edi, dword ptr [ecx + 8]
        dec    dword ptr [ecx + 0x1c]
        jne    loc_1_2773
        mov    edx, dword ptr [edi + 0x26d]
        cmp    ecx, edx
        jne    loc_1_2722
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edi + 0x26d], eax
        jmp    loc_1_2747
loc_1_2722:
        mov    eax, dword ptr [esi + 0x30b]
        cmp    dword ptr [edx + 0xc], eax
        je     loc_1_2735
loc_1_272D:
        mov    edx, dword ptr [edx + 0xc]
        cmp    dword ptr [edx + 0xc], eax
        jne    loc_1_272D
loc_1_2735:
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    ecx, dword ptr [esi + 0x30b]
        mov    dword ptr [edi + 0x452], ecx
loc_1_2747:
        mov    eax, dword ptr [ecx + 0x18]
        cmp    dword ptr [edi + 0x317], eax
        jne    loc_1_2758
        push   edi
        call   sub_1_0BC0
loc_1_2758:
        cmp    dword ptr [edi + 0x26d], 0
        jne    loc_1_2773
        push   edi
        push   0                             ; -> loc_1_2310
        add    edi, 0x20c
        push   edi
        call   sub_1_D7DA
loc_1_2773:
        dec    dword ptr [esi + 0x272]
loc_1_2779:
        mov    byte ptr [esi + 0x303], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_2789:
        mov    byte ptr [esi + 0x303], 0
        mov    byte ptr [esi + 0x271], 0
        mov    word ptr [esi + 0x40], 0xf
        push   0
        mov    eax, dword ptr [esi + 0x456]
        push   0xc0010007
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_D71E
        push   0
        mov    ecx, dword ptr [esi + 0x456]
        push   0x75
        push   ecx
        call   sub_1_D7EC
        pop    edi
        dec    dword ptr [esi + 0x272]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27E0:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [eax + 8]
        inc    dword ptr [edi + 0x272]
        cmp    byte ptr [edi + 0x305], 0
        je     loc_1_280B
        mov    ebx, 0xc001000d
        jmp    loc_1_289F
loc_1_280B:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, eax
        shr    ecx, 0x18
        mov    byte ptr [ebp - 4], cl
        mov    ecx, eax
        shr    ecx, 0x10
        mov    byte ptr [ebp - 3], cl
        mov    byte ptr [ebp - 2], ah
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [edi + 0x9f], al
        mov    cl, byte ptr [ebp - 3]
        mov    byte ptr [edi + 0xa0], cl
        mov    dl, byte ptr [ebp - 2]
        mov    byte ptr [edi + 0xa1], dl
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edi + 0xa2], al
loc_1_2848:
        push   edi
        call   sub_1_3410
        test   ax, ax
        je     loc_1_289D
        cmp    ax, 8
        mov    ebx, 0xc0000001
        jne    loc_1_289F
        test   esi, esi
        jne    loc_1_288C
        push   0
        xor    esi, esi
        mov    byte ptr [edi + 0x303], 1
        push   0
        push   edi
        push   0
        call   sub_1_29F0
        mov    ax, word ptr [edi + 0x40]
loc_1_287B:
        cmp    ax, 1
        je     loc_1_2848
        inc    esi
        cmp    esi, 0xfffff
        jb     loc_1_287B
        jmp    loc_1_289F
loc_1_288C:
        mov    eax, ebx
        dec    dword ptr [edi + 0x272]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_289D:
        xor    ebx, ebx
loc_1_289F:
        mov    eax, ebx
        dec    dword ptr [edi + 0x272]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_28B0:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        xor    ebx, ebx
        inc    dword ptr [esi + 0x272]
        cmp    byte ptr [esi + 0x305], bl
        je     loc_1_28DA
        mov    edi, 0xc001000d
        jmp    loc_1_2973
loc_1_28DA:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, eax
        shr    ecx, 0x18
        mov    byte ptr [ebp - 4], cl
        mov    ecx, eax
        shr    ecx, 0x10
        mov    byte ptr [ebp - 3], cl
        mov    byte ptr [ebp - 2], ah
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi + 0x99], al
        mov    cl, byte ptr [ebp - 3]
        mov    byte ptr [esi + 0x9a], cl
        mov    dl, byte ptr [ebp - 2]
        mov    byte ptr [esi + 0x9b], dl
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0x9c], al
loc_1_2917:
        push   esi
        call   sub_1_3440
        cmp    ax, 1
        sbb    eax, eax
        inc    ax
        je     loc_1_2971
        cmp    ax, 8
        mov    edi, 0xc0000001
        jne    loc_1_2973
        test   ebx, ebx
        jne    loc_1_2960
        push   0
        xor    ebx, ebx
        mov    byte ptr [esi + 0x303], 1
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    ax, word ptr [esi + 0x40]
loc_1_294F:
        cmp    ax, 1
        je     loc_1_2917
        inc    ebx
        cmp    ebx, 0xffff
        jb     loc_1_294F
        jmp    loc_1_2973
loc_1_2960:
        mov    eax, edi
        dec    dword ptr [esi + 0x272]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_2971:
        xor    edi, edi
loc_1_2973:
        mov    eax, edi
        dec    dword ptr [esi + 0x272]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2990:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [esi + 0x14]
        push   edx
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [eax + 0x25d]
        push   ecx
        call   sub_2_0360
loc_1_29B8:
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
sub_1_29C0:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [esi + 0x14]
        push   edx
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [eax + 0x25d]
        push   ecx
        call   sub_2_02C0
loc_1_29E8:
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
sub_1_29F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x272]
        cmp    byte ptr [esi + 0x303], 1
        jne    loc_1_2A4F
        cmp    byte ptr [esi + 0x305], 0
        jne    loc_1_2A4F
        cmp    byte ptr [esi + 0x304], 0
        jne    loc_1_2A4F
        cmp    dword ptr [esi + 0x26d], 0
        je     loc_1_2A4F
        mov    byte ptr [esi + 0x303], 0
        mov    byte ptr [esi + 0x305], 1
        mov    byte ptr [esi + 0x306], 1
        push   esi
        call   sub_1_2B50
        test   ax, ax
        je     loc_1_2A4F
        mov    byte ptr [esi + 0x305], 0
        mov    byte ptr [esi + 0x306], 0
loc_1_2A4F:
        dec    dword ptr [esi + 0x272]
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2A60:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [ecx + 0x46]
        cmp    eax, 2
        je     loc_1_2A7C
        cmp    eax, 3
        je     loc_1_2A96
        cmp    eax, 5
        jne    loc_1_2AA3
loc_1_2A7C:
        xor    edx, edx
        mov    al, 0x32
        mov    dx, word ptr [ecx + 4]
        pop    ebp
        mov    byte ptr [ecx + 0x198], 0x32
        add    edx, 0x10
        out    dx, al
        xor    ax, ax
        ret    4
loc_1_2A96:
        xor    edx, edx
        mov    dx, word ptr [ecx + 4]
        add    edx, 5
        in     al, dx
        and    al, 0xfb
        out    dx, al
loc_1_2AA3:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2AB0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [ecx + 0x46]
        cmp    eax, 2
        je     loc_1_2AD3
        cmp    eax, 3
        je     loc_1_2AF8
        cmp    eax, 5
        je     loc_1_2B1D
        xor    ax, ax
        pop    ebp
        ret    4
loc_1_2AD3:
        mov    byte ptr [ecx + 0x198], 0x12
        mov    cx, word ptr [ecx + 4]
        mov    al, 0x12
        and    ecx, 0xffff
        lea    edx, [ecx + 0x10]
        pop    ebp
        out    dx, al
        lea    edx, [ecx + 4]
        in     al, dx
        or     al, 0x80
        out    dx, al
        xor    ax, ax
        ret    4
loc_1_2AF8:
        mov    byte ptr [ecx + 0x198], 0x12
        mov    cx, word ptr [ecx + 4]
        mov    al, 0x12
        and    ecx, 0xffff
        lea    edx, [ecx + 4]
        pop    ebp
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        or     al, 4
        out    dx, al
        xor    ax, ax
        ret    4
loc_1_2B1D:
        mov    byte ptr [ecx + 0x198], 0x12
        mov    cx, word ptr [ecx + 4]
        mov    al, 0x12
        and    ecx, 0xffff
        lea    edx, [ecx + 0x10]
        pop    ebp
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        or     al, 4
        out    dx, al
        xor    ax, ax
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A60
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x40], 3
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        shl    eax, 0xa
        dec    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xb0], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x19e], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x97]
        and    eax, 0xffff
        test   al, 0x80
        jne    loc_1_2BC4
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x97]
        and    eax, 0xffff
        or     eax, 0xc0
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x97], ax
loc_1_2BC4:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x99]
        and    eax, 0xffff
        test   al, 0x80
        jne    loc_1_2BF9
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x99]
        and    eax, 0xffff
        or     eax, 0x80
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x99], ax
loc_1_2BF9:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x9d]
        and    eax, 0xffff
        test   al, 0xc0
        jne    loc_1_2C2E
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x9d]
        and    eax, 0xffff
        or     eax, 0xc0
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x9d], ax
loc_1_2C2E:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x9f]
        and    eax, 0xff7f
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x9f], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x91]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_2C6A
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x91], 0x7fff
loc_1_2C6A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x93]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_2C8D
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x93], 6
loc_1_2C8D:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A9B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B03
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_946E
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2CCE
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_2F71
loc_1_2CCE:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_94DC
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2CFD
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_2F71
loc_1_2CFD:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_92CB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_984F
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2D47
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 0x12
        jmp    loc_1_2F71
loc_1_2D47:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9572
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2D76
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_2F71
loc_1_2D76:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x42]
        and    eax, 0xfffd
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x42], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x42]
        and    eax, 0xffff
        or     eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x42], ax
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_2DAE
loc_1_2DAA:
        inc    word ptr [ebp - 0xc]
loc_1_2DAE:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 6
        jge    loc_1_2DEE
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        lea    edx, [eax + ecx + 8]
        in     al, dx
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x2a], al
        jmp    loc_1_2DAA
loc_1_2DEE:
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_2DFD
loc_1_2DF9:
        inc    word ptr [ebp - 0xc]
loc_1_2DFD:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 6
        jge    loc_1_2E2F
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x24]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_2E2F
        jmp    loc_1_2DF9
loc_1_2E2F:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 6
        jne    loc_1_2E84
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_2E4F
loc_1_2E4B:
        inc    word ptr [ebp - 0xc]
loc_1_2E4F:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 6
        jge    loc_1_2E84
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x2a]
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x24], al
        jmp    loc_1_2E4B
loc_1_2E84:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_939D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8E75
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2EBC
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_2F71
loc_1_2EBC:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9A6F
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2EEB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_2F71
loc_1_2EEB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_939D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8E75
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2F23
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_2F71
loc_1_2F23:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2AB0
        push   0x7ff6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9265
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2F60
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_2F71
loc_1_2F60:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        xor    ax, ax
        jmp    loc_1_2F71
loc_1_2F71:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_2F78:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x40]
        cmp    ax, 1
        jne    loc_1_2F91
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_1_2F91:
        cmp    ax, 2
        je     loc_1_2FA0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_2FA0:
        push   esi
        call   sub_1_2A60
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        push   esi
        call   sub_1_9300
        push   0
        push   esi
        call   sub_1_9103
        test   ax, ax
        je     loc_1_2FD8
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_2FD8:
        push   0
        push   esi
        call   sub_1_917D
        test   ax, ax
        je     loc_1_2FFA
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_2FFA:
        push   1
        push   esi
        call   sub_1_9103
        test   ax, ax
        je     loc_1_301C
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_301C:
        push   1
        push   esi
        call   sub_1_917D
        test   ax, ax
        je     loc_1_303E
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_303E:
        mov    byte ptr [esi + 0x19e], 1
        push   esi
        call   sub_1_A6C9
        test   ax, ax
        push   esi
        je     loc_1_3065
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_3065:
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        push   esi
        call   sub_1_2A60
        push   esi
        call   sub_1_67FC
        mov    byte ptr [esi + 0x19e], 0
        mov    word ptr [esi + 0x3e], 0
        push   esi
        call   sub_1_31A0
        mov    ax, word ptr [esi + 0x200]
        and    ax, 7
        cmp    ax, 1
        jne    loc_1_30C4
        push   1
        push   1
        push   esi
        call   sub_1_6B39
        test   ax, ax
        je     loc_1_3112
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_30C4:
        cmp    ax, 2
        jne    loc_1_30EE
        push   2
        push   1
        push   esi
        call   sub_1_6B39
        test   ax, ax
        je     loc_1_3112
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_30EE:
        push   0
        push   2
        push   esi
        call   sub_1_6B39
        test   ax, ax
        je     loc_1_3112
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_3112:
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3130:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 0x40], 1
        je     loc_1_314D
        mov    word ptr [esi + 0x40], 5
        mov    ax, 0xa
        pop    esi
        pop    ebp
        ret    4
loc_1_314D:
        push   esi
        call   sub_1_2A60
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        push   esi
        call   sub_1_9300
        push   esi
        call   sub_1_9201
        test   ax, ax
        push   esi
        je     loc_1_3189
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    word ptr [esi + 0x40], 5
        mov    ax, 0xa
        pop    esi
        pop    ebp
        ret    4
loc_1_3189:
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_31A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0
        mov    eax, dword ptr [esi + 0x25d]
        test   eax, eax
        je     loc_1_31BE
        mov    edi, dword ptr [eax + 0x10]
loc_1_31BE:
        test   edi, 0x10
        lea    ecx, [esi + 0x3e]
        je     loc_1_31D5
        or     byte ptr [esi + 0x19c], 0xc0
        or     byte ptr [ecx], 8
        jmp    loc_1_31E3
loc_1_31D5:
        and    word ptr [esi + 0x19c], 0xffbf
        and    word ptr [ecx], 0xfff7
loc_1_31E3:
        test   edi, 0x8000
        je     loc_1_31F9
        or     byte ptr [esi + 0x19b], 4
        or     word ptr [ecx], 0x120
        jmp    loc_1_3207
loc_1_31F9:
        and    word ptr [esi + 0x19a], 0xfbff
        and    word ptr [ecx], 0xfedf
loc_1_3207:
        test   edi, 0x20
        je     loc_1_321B
        or     byte ptr [esi + 0x19b], 8
        or     byte ptr [ecx], 4
        jmp    loc_1_3229
loc_1_321B:
        and    word ptr [esi + 0x19a], 0xf7ff
        and    word ptr [ecx], 0xfffb
loc_1_3229:
        test   edi, 0x2000
        je     loc_1_323D
        or     byte ptr [esi + 0x19c], 0x10
        or     byte ptr [ecx], 0x40
        jmp    loc_1_324B
loc_1_323D:
        and    word ptr [esi + 0x19c], 0xffef
        and    word ptr [ecx], 0xffbf
loc_1_324B:
        test   edi, 0x4000
        je     loc_1_3277
        push   esi
        call   sub_1_A3FF
        mov    bx, ax
        test   bx, bx
        je     loc_1_32F4
        push   esi
        call   sub_1_682C
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3277:
        mov    ax, word ptr [esi + 0x9d]
        mov    word ptr [esi + 0x9d], 0
        mov    cx, word ptr [esi + 0x9f]
        push   esi
        mov    word ptr [esi + 0x9f], 0
        mov    dx, word ptr [esi + 0xa1]
        mov    word ptr [esi + 0xa1], 0
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], cx
        mov    word ptr [ebp - 4], dx
        call   sub_1_A3FF
        mov    bx, ax
        mov    cx, word ptr [ebp - 8]
        mov    ax, word ptr [ebp - 6]
        mov    dx, word ptr [ebp - 4]
        mov    word ptr [esi + 0x9d], cx
        mov    word ptr [esi + 0x9f], ax
        mov    word ptr [esi + 0xa1], dx
        test   bx, bx
        je     loc_1_32F4
        push   esi
        call   sub_1_682C
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_32F4:
        test   edi, 0x1000
        je     loc_1_3320
        push   esi
        call   sub_1_A564
        mov    di, ax
        test   di, di
        je     loc_1_3392
        push   esi
        call   sub_1_682C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3320:
        mov    di, word ptr [esi + 0x97]
        mov    word ptr [esi + 0x97], 0
        mov    ax, word ptr [esi + 0x99]
        push   esi
        mov    word ptr [esi + 0x99], 0
        mov    cx, word ptr [esi + 0x9b]
        mov    word ptr [esi + 0x9b], 0
        mov    word ptr [ebp - 6], ax
        mov    word ptr [ebp - 4], cx
        call   sub_1_A564
        mov    cx, word ptr [ebp - 6]
        mov    dx, word ptr [ebp - 4]
        mov    word ptr [esi + 0x97], di
        mov    word ptr [esi + 0x99], cx
        mov    word ptr [esi + 0x9b], dx
        test   bx, bx
        je     loc_1_3392
        push   esi
        call   sub_1_682C
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3392:
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        push   esi
        call   sub_1_9300
        lea    eax, [esi + 0x19a]
        push   eax
        push   0xa001
        push   esi
        call   sub_1_7B23
        mov    di, ax
        test   di, di
        je     loc_1_33CE
        push   esi
        call   sub_1_682C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_33CE:
        lea    eax, [esi + 0x19c]
        push   eax
        push   0xa101
        push   esi
        call   sub_1_7B23
        mov    di, ax
        push   esi
        test   di, di
        je     loc_1_33FA
        call   sub_1_682C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_33FA:
        call   sub_1_682C
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_3410:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0x25d]
        test   eax, eax
        je     loc_1_3426
        mov    eax, dword ptr [eax + 0x10]
        jmp    loc_1_3428
loc_1_3426:
        xor    eax, eax
loc_1_3428:
        test   ah, 0x40
        je     loc_1_3437
        push   ecx
        call   sub_1_A3FF
        pop    ebp
        ret    4
loc_1_3437:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_3440:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0x25d]
        test   eax, eax
        je     loc_1_3456
        mov    eax, dword ptr [eax + 0x10]
        jmp    loc_1_3458
loc_1_3456:
        xor    eax, eax
loc_1_3458:
        test   ah, 0x10
        je     loc_1_3467
        push   ecx
        call   sub_1_A3FF
        pop    ebp
        ret    4
loc_1_3467:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_3470:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    dword ptr [ebp - 0xc], eax
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        mov    bx, word ptr [esi + 0xba]
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        push   esi
        call   sub_1_9300
        push   ebx
        push   esi
        call   sub_1_8AF5
        mov    edi, eax
        mov    dword ptr [ebp - 0x18], eax
        mov    ax, word ptr [esi + 0x20]
        shl    ax, 0xa
        cmp    dword ptr [ebp + 8], 0
        mov    word ptr [ebp - 2], ax
        je     loc_1_356E
        mov    eax, ebx
        and    eax, 0xffff
        lea    ecx, [esi + eax*4]
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [ebp - 0x10], ecx
        add    ecx, 0x190
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 8], ecx
loc_1_34DB:
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [eax]
        mov    eax, ebx
        sub    eax, edi
        cmp    eax, dword ptr [ebp + 8]
        jae    loc_1_34EE
        sub    bx, di
        jmp    loc_1_34F2
loc_1_34EE:
        mov    bx, word ptr [ebp + 8]
loc_1_34F2:
        push   edi
        push   esi
        call   sub_1_9300
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 0x14]
        sub    ax, word ptr [esi + 0x1c]
        and    ax, word ptr [esi + 0xb0]
        mov    edx, eax
        and    edx, 0xffff
        sub    ecx, edx
        mov    edx, ebx
        and    edx, 0xffff
        cmp    ecx, edx
        jge    loc_1_3526
        mov    bx, word ptr [ebp - 2]
        sub    bx, ax
loc_1_3526:
        mov    ecx, edi
        xor    eax, eax
        mov    ax, word ptr [esi + 0xb0]
        sub    ecx, dword ptr [esi + 0x1c]
        and    ecx, eax
        and    ebx, 0xffff
        mov    eax, dword ptr [esi + 0x1c]
        push   ebx
        add    eax, ecx
        add    edi, ebx
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        call   sub_1_D8CC
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [ebp + 8], ebx
        add    dword ptr [ebp - 0xc], ebx
        cmp    dword ptr [eax], edi
        ja     loc_1_3564
        mov    eax, dword ptr [ebp - 0x10]
        mov    edi, dword ptr [eax + 0x188]
loc_1_3564:
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_34DB
loc_1_356E:
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        push   esi
        call   sub_1_9300
        push   esi
        call   sub_1_682C
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3590:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x18]
        mov    ax, word ptr [esi + 0xba]
        push   esi
        mov    word ptr [ebp - 4], ax
        mov    dword ptr [ecx], 0
        call   sub_1_67FC
        mov    ecx, dword ptr [esi + 0x1c]
        push   ecx
        push   esi
        call   sub_1_9300
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   esi
        call   sub_1_8AF5
        mov    edi, dword ptr [ebp + 0x10]
        push   0
        add    edi, eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [ebp - 0x18], eax
        mov    ax, word ptr [esi + 0x20]
        push   0
        shl    ax, 0xa
        push   0
        mov    word ptr [ebp - 2], ax
        mov    edx, dword ptr [ebp + 0x14]
        push   edx
        call   sub_1_D810
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_3613
        mov    ecx, dword ptr [ebp - 0x18]
        push   ecx
        push   esi
        call   sub_1_9300
        push   esi
        call   sub_1_682C
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_3613:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        push   edx
        call   sub_1_D834
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_373B
loc_1_362E:
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_1_3701
loc_1_3638:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_3731
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ebx, dword ptr [esi + eax*4 + 0x190]
        lea    ecx, [esi + eax*4]
        mov    eax, ebx
        sub    eax, edi
        mov    dword ptr [ebp - 0xc], ecx
        cmp    eax, dword ptr [ebp + 0xc]
        jae    loc_1_3665
        sub    bx, di
        jmp    loc_1_3669
loc_1_3665:
        mov    bx, word ptr [ebp + 0xc]
loc_1_3669:
        push   edi
        push   esi
        call   sub_1_9300
        mov    eax, edi
        xor    ecx, ecx
        sub    ax, word ptr [esi + 0x1c]
        and    ax, word ptr [esi + 0xb0]
        mov    cx, word ptr [ebp - 2]
        mov    edx, eax
        and    edx, 0xffff
        sub    ecx, edx
        mov    edx, ebx
        and    edx, 0xffff
        cmp    ecx, edx
        jge    loc_1_36A0
        mov    bx, word ptr [ebp - 2]
        sub    bx, ax
loc_1_36A0:
        mov    eax, ebx
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_36B0
        mov    bx, word ptr [ebp - 8]
loc_1_36B0:
        mov    ecx, edi
        xor    eax, eax
        mov    ax, word ptr [esi + 0xb0]
        sub    ecx, dword ptr [esi + 0x1c]
        and    ecx, eax
        and    ebx, 0xffff
        mov    eax, dword ptr [esi + 0x1c]
        push   ebx
        add    eax, ecx
        add    edi, ebx
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_D8CC
        mov    eax, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 8], ebx
        sub    dword ptr [ebp + 0xc], ebx
        add    dword ptr [ebp - 0x10], ebx
        cmp    dword ptr [eax + 0x190], edi
        ja     loc_1_36F2
        mov    edi, dword ptr [eax + 0x188]
loc_1_36F2:
        mov    eax, dword ptr [ebp + 8]
        add    dword ptr [eax], ebx
        cmp    dword ptr [ebp - 8], 0
        ja     loc_1_3638
loc_1_3701:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_373B
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_3731
        lea    eax, [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        call   sub_1_D858
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_373B
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        push   edx
        call   sub_1_D834
loc_1_3731:
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_362E
loc_1_373B:
        mov    ecx, dword ptr [ebp - 0x18]
        push   ecx
        push   esi
        call   sub_1_9300
        push   esi
        call   sub_1_682C
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3760:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    word ptr [esi + 0x40], 1
        je     loc_1_3780
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3780:
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        push   esi
        call   sub_1_9300
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        push   0
        push   0
        push   edi
        call   sub_1_D810
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   1
        push   esi
        call   sub_1_6C63
        cmp    eax, -1
        mov    ebx, eax
        jne    loc_1_37CB
        push   esi
        call   sub_1_682C
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_37CB:
        mov    eax, dword ptr [ebx + 0xe]
        mov    ecx, dword ptr [eax + 0xc]
        push   ecx
        push   edi
        push   esi
        call   sub_1_7096
        push   1
        push   ebx
        push   esi
        call   sub_1_6F4B
        mov    di, ax
        push   esi
        call   sub_1_682C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3800:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x1c], 0
        mov    word ptr [ebp - 0xc], 0xff00
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
loc_1_3834:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0xd0]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xcc]
        mov    al, byte ptr [ecx + eax*2]
        and    eax, 0xff
        test   al, 0xf0
        jne    loc_1_4458
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 5
        je     loc_1_4458
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0xd0]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xcc]
        mov    al, byte ptr [ecx + eax*2]
        mov    byte ptr [ebp - 0x14], al
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0xd0]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xcc]
        mov    al, byte ptr [ecx + eax*2 + 1]
        mov    byte ptr [ebp - 0x18], al
        mov    eax, 1
        mov    cl, byte ptr [ebp - 0x14]
        shl    eax, cl
        mov    ecx, dword ptr [ebp - 0x1c]
        and    ecx, 0xffff
        or     eax, ecx
        mov    word ptr [ebp - 0x1c], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_439A
loc_1_38C1:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x19e]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3A44
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x24], eax
        jmp    loc_1_39D8
loc_1_38ED:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x40], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CED8
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_3A10
loc_1_3914:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 1
        jmp    loc_1_3A10
loc_1_3925:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 2
        jmp    loc_1_3A10
loc_1_3936:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 3
        jmp    loc_1_3A10
loc_1_3947:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x40], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CED8
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_3A10
loc_1_396E:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 5
        jmp    loc_1_3A10
loc_1_397F:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x40], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CED8
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_3A10
loc_1_39A6:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 7
        jmp    loc_1_3A10
loc_1_39B7:
        jmp    loc_1_3A10
loc_1_39BC:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
        db  0E9h, 03Dh, 000h, 000h, 000h, 0E9h, 038h, 000h, 000h, 000h
loc_1_39D8:
        cmp    dword ptr [ebp - 0x24], 8
        ja     loc_1_39BC
        mov    eax, dword ptr [ebp - 0x24]
        jmp    dword ptr [eax*4]             ; -> obj1:0x39EC
        ; ---- dispatch table (9 entries) ----
        dd  loc_1_38ED
        dd  loc_1_3914
        dd  loc_1_3925
        dd  loc_1_3936
        dd  loc_1_3947
        dd  loc_1_396E
        dd  loc_1_397F
        dd  loc_1_39A6
        dd  loc_1_39B7
loc_1_3A10:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_3A3B
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        cmp    eax, 8
        jge    loc_1_3A3B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D0D8
        mov    word ptr [ebp - 8], ax
loc_1_3A3B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
loc_1_3A44:
        jmp    loc_1_43E9
loc_1_3A49:
        jmp    loc_1_43E9
loc_1_3A4E:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x19e]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3A64
loc_1_3A64:
        jmp    loc_1_43E9
loc_1_3A69:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 4
        je     loc_1_3AA8
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3AA8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3AA8:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 2
        je     loc_1_3AE7
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3AE7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3AE7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 1
        je     loc_1_3B37
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 5
        je     loc_1_3B37
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3B37
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3B37:
        jmp    loc_1_43E9
loc_1_3B3C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 4
        je     loc_1_3BD7
        mov    eax, dword ptr [ebp - 0x1c]
        and    eax, 0xffff
        or     eax, 0x800
        mov    word ptr [ebp - 0x1c], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x186], 0
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3B98
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3B98:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3BAD
        jmp    loc_1_43E9
loc_1_3BAD:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A1D0
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3BD7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3BD7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 2
        je     loc_1_3C61
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x184], 0
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3C22
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3C22:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3C37
        jmp    loc_1_43E9
loc_1_3C37:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A1D0
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3C61
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3C61:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 1
        je     loc_1_3CE6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x182], 0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3CAC
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3CAC:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_3CE6
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A1D0
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3CE6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3CE6:
        jmp    loc_1_43E9
loc_1_3CEB:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x199], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xba], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A250
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3D31
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3D31:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 0x80
        je     loc_1_3D7D
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9103
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3D68
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3D68:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x51], 0
        je     loc_1_3D7D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x51]
        inc    dword ptr [eax]
loc_1_3D7D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 0x40
        je     loc_1_3DC9
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_917D
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3DB4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3DB4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x51], 0
        je     loc_1_3DC9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x51]
        inc    dword ptr [eax]
loc_1_3DC9:
        jmp    loc_1_43E9
loc_1_3DCE:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x199], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xba], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A250
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3E14
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3E14:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 0x80
        je     loc_1_3E60
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9103
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3E4B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3E4B:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x51], 0
        je     loc_1_3E60
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x51]
        inc    dword ptr [eax]
loc_1_3E60:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 0x40
        je     loc_1_3EAC
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_917D
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3E97
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3E97:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x51], 0
        je     loc_1_3EAC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x51]
        inc    dword ptr [eax]
loc_1_3EAC:
        jmp    loc_1_43E9
loc_1_3EB1:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x199], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xba], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A250
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3EF7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3EF7:
        jmp    loc_1_43E9
loc_1_3EFC:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x199], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xba], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A250
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_3F42
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_3F42:
        jmp    loc_1_43E9
loc_1_3F47:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 0x40
        je     loc_1_3F69
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x5d], 0
        je     loc_1_3F69
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x5d]
        inc    dword ptr [eax]
loc_1_3F69:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        test   al, 0x80
        je     loc_1_3FCA
        mov    eax, dword ptr [ebp + 8]
        inc    byte ptr [eax + 0x199]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x199]
        and    eax, 0xff
        cmp    eax, 3
        jl     loc_1_3FB5
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x199], 0
        mov    eax, dword ptr [ebp - 0x1c]
        and    eax, 0xf7ff
        mov    word ptr [ebp - 0x1c], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B2C4
loc_1_3FB5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x59], 0
        je     loc_1_3FCA
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x59]
        inc    dword ptr [eax]
loc_1_3FCA:
        jmp    loc_1_43E9
loc_1_3FCF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_4006
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        jmp    loc_1_41F8
loc_1_4006:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        cmp    eax, 7
        jne    loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    ax, word ptr [eax + 0xa]
        and    eax, 0xffff
        cmp    eax, 0x5412
        jne    loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        test   al, 1
        je     loc_1_4061
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B60
loc_1_4061:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        test   al, 2
        je     loc_1_40CF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x8f], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x8d]
        and    eax, 0xffff
        or     eax, 0x8000
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x8d], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D0D8
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
loc_1_40CF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        test   al, 4
        je     loc_1_41A6
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x89], 0
        je     loc_1_41A6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 0x18]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xe4]
        mov    cx, word ptr [ecx + 0x18]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x89]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xe4]
        mov    cx, word ptr [ecx + 0x1a]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x89]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 0x1c]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xe4]
        mov    cx, word ptr [ecx + 0x1c]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x89]
        mov    word ptr [ecx + 4], ax
loc_1_41A6:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        test   al, 8
        je     loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_BC8F
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_41F8:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xe0], 0
        jmp    loc_1_43E9
loc_1_4209:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x28], eax
        jmp    loc_1_42E9
loc_1_4216:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 8
        jmp    loc_1_4319
loc_1_4227:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 9
        jmp    loc_1_4319
loc_1_4238:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 0xa
        jmp    loc_1_4319
loc_1_4249:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 0xb
        jmp    loc_1_4319
loc_1_425A:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x19a]
        and    eax, 0xffff
        test   ah, 0x80
        je     loc_1_42A6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9201
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_42A6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_42A6:
        jmp    loc_1_4319
loc_1_42AB:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 0xd
        jmp    loc_1_4319
loc_1_42BC:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x8d], 0xe
        jmp    loc_1_4319
loc_1_42CD:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
        db  0E9h, 035h, 000h, 000h, 000h, 0E9h, 030h, 000h, 000h, 000h
loc_1_42E9:
        cmp    dword ptr [ebp - 0x28], 6
        ja     loc_1_42CD
        mov    eax, dword ptr [ebp - 0x28]
        jmp    dword ptr [eax*4]             ; -> obj1:0x42FD
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_4216
        dd  loc_1_4227
        dd  loc_1_4238
        dd  loc_1_4249
        dd  loc_1_425A
        dd  loc_1_42AB
        dd  loc_1_42BC
loc_1_4319:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x8d]
        and    eax, 0x7fff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x8d], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D0D8
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        jmp    loc_1_43E9
loc_1_4356:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x40], 2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CED8
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        jmp    loc_1_43E9
loc_1_4383:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
        db  0E9h, 04Fh, 000h, 000h, 000h
loc_1_439A:
        dec    dword ptr [ebp - 0x20]
        cmp    dword ptr [ebp - 0x20], 0xd
        ja     loc_1_4383
        mov    eax, dword ptr [ebp - 0x20]
        jmp    dword ptr [eax*4]             ; -> obj1:0x43B1
        ; ---- dispatch table (14 entries) ----
        dd  loc_1_38C1
        dd  loc_1_3A49
        dd  loc_1_4383
        dd  loc_1_3A4E
        dd  loc_1_3A69
        dd  loc_1_3B3C
        dd  loc_1_3CEB
        dd  loc_1_3DCE
        dd  loc_1_3EB1
        dd  loc_1_3EFC
        dd  loc_1_3F47
        dd  loc_1_3FCF
        dd  loc_1_4209
        dd  loc_1_4356
loc_1_43E9:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0xd0]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xcc]
        mov    al, byte ptr [ecx + eax*2]
        and    eax, 0xff
        or     al, 0xf0
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0xd0]
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0xcc]
        mov    byte ptr [edx + ecx*2], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xd0]
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0xd0]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0xd0]
        cmp    eax, 0x20
        jne    loc_1_4453
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xd0], 0
loc_1_4453:
        jmp    loc_1_3834
loc_1_4458:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9D17
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   ah, ah
        jne    loc_1_44A1
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9D8E
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_44A1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    loc_1_44B2
loc_1_44A1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        xor    ax, ax
        jmp    loc_1_44B2
loc_1_44B2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        db  000h, 000h, 000h
sub_1_44BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    byte ptr [ebp + 0x14], 0
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x18]
        mov    dword ptr [edi], 0
        je     loc_1_4537
        cmp    byte ptr [ebp + 0x10], 0
        je     loc_1_44E0
        mov    dword ptr [edi], 8
loc_1_44E0:
        mov    esi, dword ptr [ebp + 0xc]
        lea    edx, [esi + 0xe]
        in     al, dx
        test   al, 0xf8
        jne    loc_1_465E
        mov    eax, dword ptr [edi]
        or     eax, 0x95
        cmp    byte ptr [ebp + 0x10], 0
        mov    dword ptr [edi], eax
        je     loc_1_4505
        or     eax, 0x800000
        jmp    loc_1_450A
loc_1_4505:
        or     eax, 0x400040
loc_1_450A:
        lea    edx, [esi + 1]
        mov    dword ptr [edi], eax
        in     al, dx
        mov    ecx, dword ptr [edi]
        and    al, 0xc
        out    dx, al
        in     al, dx
        or     ecx, 0x4250000
        test   al, 1
        mov    dword ptr [edi], ecx
        je     loc_1_465E
        or     ecx, 0x80000
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_4537:
        mov    esi, dword ptr [ebp + 0xc]
        xor    ebx, ebx
        lea    edx, [esi + 0xe]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        and    al, 0x1e
        shr    al, 1
        mov    bl, al
        test   ebx, ebx
        je     loc_1_465E
        cmp    byte ptr [ebp + 0x10], 0
        je     loc_1_455E
        mov    dword ptr [edi], 8
loc_1_455E:
        push   edi
        mov    al, byte ptr [ebp + 0x10]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        call   sub_1_466C
        test   byte ptr [ebp - 4], 1
        je     loc_1_4578
        or     dword ptr [edi], 2
        jmp    loc_1_4585
loc_1_4578:
        cmp    ebx, 1
        je     loc_1_4582
        or     dword ptr [edi], 3
        jmp    loc_1_4585
loc_1_4582:
        or     dword ptr [edi], 1
loc_1_4585:
        cmp    ebx, 2
        jb     loc_1_45AA
        test   byte ptr [ebp - 4], 0x20
        je     loc_1_45AA
        mov    ecx, dword ptr [edi]
        mov    eax, ecx
        and    al, 0x12
        cmp    al, 0x12
        je     loc_1_45A2
        mov    eax, ecx
        and    al, 0x13
        cmp    al, 0x13
        jne    loc_1_45AA
loc_1_45A2:
        or     ecx, 0x200000
        mov    dword ptr [edi], ecx
loc_1_45AA:
        cmp    ebx, 3
        jb     loc_1_45F6
        cmp    byte ptr [ebp + 0x10], 0
        jne    loc_1_45F0
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        and    dword ptr [edi], 0xffd0ffb0
        call   sub_1_4EE3
        test   eax, eax
        je     loc_1_45D1
        or     dword ptr [edi], 0x1000000
        jmp    loc_1_45D7
loc_1_45D1:
        or     dword ptr [edi], 0x400000
loc_1_45D7:
        lea    eax, [ebp - 4]
        mov    cl, byte ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   esi
        push   edx
        call   sub_1_484C
        mov    ecx, dword ptr [ebp - 4]
        or     dword ptr [edi], ecx
        jmp    loc_1_4606
loc_1_45F0:
        or     dword ptr [edi], 0x800000
loc_1_45F6:
        push   edi
        mov    al, byte ptr [ebp + 0x10]
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   ecx
        call   sub_1_4C32
loc_1_4606:
        cmp    ebx, 4
        jb     loc_1_464A
        mov    ecx, dword ptr [edi]
        or     ecx, 0x20
        test   ecx, 0x1000000
        mov    dword ptr [edi], ecx
        je     loc_1_464A
        lea    edx, [esi + 7]
        and    ecx, 0xfc3fffff
        in     al, dx
        shr    al, 4
        mov    dword ptr [edi], ecx
        cmp    al, 2
        jne    loc_1_463E
        or     ecx, 0x9000000
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_463E:
        cmp    al, 3
        jne    loc_1_464A
        or     ecx, 0x1000000
        mov    dword ptr [edi], ecx
loc_1_464A:
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4E39
        test   al, al
        je     loc_1_465E
        or     dword ptr [edi], 0x100000
loc_1_465E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
sub_1_466C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EE3
        test   eax, eax
        je     loc_1_4699
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x10
        jmp    loc_1_4845
loc_1_4699:
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 7
        mov    al, 0x35
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x35
        jne    loc_1_46F7
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 7
        mov    al, 0x3a
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x3a
        jne    loc_1_46F7
        mov    byte ptr [ebp - 0xc], 1
loc_1_46F7:
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 7
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x10]
        test   eax, eax
        je     loc_1_4726
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        test   eax, eax
        je     loc_1_4721
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x10
loc_1_4721:
        jmp    loc_1_4845
loc_1_4726:
        mov    dword ptr [ebp - 0x10], 1
        jmp    loc_1_4735
loc_1_4732:
        inc    dword ptr [ebp - 0x10]
loc_1_4735:
        cmp    dword ptr [ebp - 0x10], 6
        jae    loc_1_4775
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, dword ptr [ebp + 0xc]
        mov    edx, eax
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        lea    edx, [ecx + edx + 8]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        je     loc_1_4770
        jmp    loc_1_4775
loc_1_4770:
        jmp    loc_1_4732
loc_1_4775:
        cmp    dword ptr [ebp - 0x10], 6
        jne    loc_1_4799
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, dword ptr [ebp + 0xc]
        mov    edx, eax
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        lea    edx, [ecx + edx + 8]
        in     al, dx
        mov    byte ptr [ebp - 8], al
loc_1_4799:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        jne    loc_1_47B0
        jmp    loc_1_4845
loc_1_47B0:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        test   eax, eax
        je     loc_1_47C8
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x10
        jmp    loc_1_4845
loc_1_47C8:
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 1
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ebp + 0xc]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    al, 1
        inc    edx
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        in     al, dx
        mov    byte ptr [ebp - 0x14], al
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 0x14], al
        mov    al, byte ptr [ebp - 0x14]
        and    eax, 1
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        jne    loc_1_483D
        mov    edx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xfe
        inc    edx
        out    dx, al
        mov    ecx, dword ptr [ebp + 0x14]
        or     dword ptr [ecx], 0x40
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 1
        je     loc_1_4838
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x80000
loc_1_4838:
        jmp    loc_1_4845
loc_1_483D:
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_4845:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_484C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EE3
        test   eax, eax
        je     loc_1_489E
        mov    byte ptr [ebp - 8], 0x4a
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_487F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 0x40
        je     loc_1_4899
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        jmp    loc_1_487F
loc_1_4899:
        jmp    loc_1_492D
loc_1_489E:
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xc
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 2
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xf
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 0xa0
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 3
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xc
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 0x12
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
loc_1_4913:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 0x10
        je     loc_1_492D
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        jmp    loc_1_4913
loc_1_492D:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x10]
        test   eax, eax
        je     loc_1_4940
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 8
loc_1_4940:
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 9
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xc0
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x40
        je     loc_1_496B
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x80
loc_1_496B:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x80
        je     loc_1_4981
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x100
loc_1_4981:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x18
        cmp    al, 8
        jne    loc_1_49ED
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x40
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EE3
        test   eax, eax
        je     loc_1_49CF
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 0x10
        je     loc_1_49CA
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x80000
loc_1_49CA:
        jmp    loc_1_49ED
loc_1_49CF:
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 1
        je     loc_1_49ED
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x80000
loc_1_49ED:
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 8
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 7
        jne    loc_1_4A0F
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 1
        jmp    loc_1_4A47
loc_1_4A0F:
        mov    al, byte ptr [ebp - 8]
        and    eax, 7
        cmp    al, 2
        jne    loc_1_4A28
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 3
        jmp    loc_1_4A47
loc_1_4A28:
        mov    al, byte ptr [ebp - 8]
        and    eax, 7
        cmp    al, 3
        jne    loc_1_4A41
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 4
        jmp    loc_1_4A47
loc_1_4A41:
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 2
loc_1_4A47:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 0x18
        je     loc_1_4A5D
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x200000
loc_1_4A5D:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xe0
        cmp    al, 0x40
        jne    loc_1_4A7B
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x20000
        jmp    loc_1_4B3F
loc_1_4A7B:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xe0
        cmp    al, 0x60
        jne    loc_1_4ACE
        mov    eax, dword ptr [ebp + 0x14]
        test   byte ptr [eax], 0x40
        jne    loc_1_4AA5
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x30000
        jmp    loc_1_4AC9
loc_1_4AA5:
        mov    eax, dword ptr [ebp + 0x14]
        test   byte ptr [eax + 2], 8
        jne    loc_1_4AC0
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x20000
        jmp    loc_1_4AC9
loc_1_4AC0:
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x30000
loc_1_4AC9:
        jmp    loc_1_4B3F
loc_1_4ACE:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xe0
        cmp    al, 0x80
        jne    loc_1_4AEC
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x40000
        jmp    loc_1_4B3F
loc_1_4AEC:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xe0
        cmp    al, 0xa0
        jne    loc_1_4B3F
        mov    eax, dword ptr [ebp + 0x14]
        test   byte ptr [eax], 0x40
        jne    loc_1_4B16
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x50000
        jmp    loc_1_4B3A
loc_1_4B16:
        mov    eax, dword ptr [ebp + 0x14]
        test   byte ptr [eax + 2], 8
        jne    loc_1_4B31
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x40000
        jmp    loc_1_4B3A
loc_1_4B31:
        mov    eax, dword ptr [ebp + 0x14]
        or     dword ptr [eax], 0x50000
loc_1_4B3A:
        jmp    loc_1_4B3F
loc_1_4B3F:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EE3
        test   eax, eax
        je     loc_1_4B9C
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x43
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 4
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    byte ptr [ebp - 8], 0x46
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_4B7D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 0x40
        je     loc_1_4B97
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        jmp    loc_1_4B7D
loc_1_4B97:
        jmp    loc_1_4C2B
loc_1_4B9C:
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xc
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 2
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xf
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 0x80
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 3
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xc
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 0x10
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
loc_1_4C11:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 0x10
        je     loc_1_4C2B
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        jmp    loc_1_4C11
loc_1_4C2B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_4C32:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x14], 2
        jae    loc_1_4CE0
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x10]
        test   eax, eax
        je     loc_1_4C60
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x30000
        jmp    loc_1_4CDB
loc_1_4C60:
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax], 0x40
        je     loc_1_4C95
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax + 2], 8
        je     loc_1_4C87
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x30000
        jmp    loc_1_4C90
loc_1_4C87:
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x20000
loc_1_4C90:
        jmp    loc_1_4CDB
loc_1_4C95:
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax], 0x10
        jne    loc_1_4CAF
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x20000
        jmp    loc_1_4CDB
loc_1_4CAF:
        mov    edx, dword ptr [ebp + 0xc]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 8
        je     loc_1_4CD2
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x40000
        jmp    loc_1_4CDB
loc_1_4CD2:
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x20000
loc_1_4CDB:
        jmp    loc_1_4E32
loc_1_4CE0:
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 0xe
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax], 8
        je     loc_1_4D1F
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x40
        je     loc_1_4D11
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x50000
        jmp    loc_1_4D1A
loc_1_4D11:
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x30000
loc_1_4D1A:
        jmp    loc_1_4E32
loc_1_4D1F:
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_4DBE
loc_1_4D31:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x40
        je     loc_1_4D4C
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x40000
        jmp    loc_1_4D55
loc_1_4D4C:
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x20000
loc_1_4D55:
        jmp    loc_1_4E32
loc_1_4D5A:
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax + 2], 8
        je     loc_1_4D90
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x40
        je     loc_1_4D82
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x50000
        jmp    loc_1_4D8B
loc_1_4D82:
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x30000
loc_1_4D8B:
        jmp    loc_1_4DB4
loc_1_4D90:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x40
        je     loc_1_4DAB
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x40000
        jmp    loc_1_4DB4
loc_1_4DAB:
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax], 0x20000
loc_1_4DB4:
        jmp    loc_1_4E32
loc_1_4DB9:
        jmp    loc_1_4E32
loc_1_4DBE:
        dec    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 0x41
        ja     loc_1_4E32
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]            ; -> obj1:0x4DF0
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x4DE0
        ; ---- dispatch table (4 entries) ----
        dd  loc_1_4D31
        dd  loc_1_4D31
        dd  loc_1_4D5A
        dd  loc_1_4DB9
        db  000h, 000h, 000h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 001h, 001h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 002h
loc_1_4E32:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
sub_1_4E39:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 0x10
        in     al, dx
        mov    byte ptr [ebp - 0xc], al
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0xfb
        mov    byte ptr [ebp - 0xc], al
        mov    edx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0x3f
        or     al, 0x80
        add    edx, 0x10
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 0x1d
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0x3f
        add    edx, 0x10
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 0x1d
        mov    al, 0x18
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0x3f
        or     al, 0x80
        add    edx, 0x10
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 0x1d
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0x3f
        add    edx, 0x10
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 0x1d
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 0x10
        mov    al, byte ptr [ebp - 0xc]
        out    dx, al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x18
        cmp    al, 0x18
        je     loc_1_4ED5
        mov    al, 1
        jmp    loc_1_4ED7
loc_1_4ED5:
        xor    al, al
loc_1_4ED7:
        jmp    loc_1_4EDC
loc_1_4EDC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_4EE3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        mov    al, byte ptr [ebp - 0x10]
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 0x10]
        and    eax, 0xc3
        mov    byte ptr [ebp - 0x10], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        or     al, 0x80
        mov    byte ptr [ebp - 0x10], al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 4
        mov    al, byte ptr [ebp - 0x10]
        out    dx, al
        mov    byte ptr [ebp - 0xc], 0
        mov    al, byte ptr [ebp - 0xc]
        mov    byte ptr [ebp - 8], al
        mov    byte ptr [ebp - 0x10], 0
        jmp    loc_1_4F31
loc_1_4F2E:
        inc    byte ptr [ebp - 0x10]
loc_1_4F31:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        cmp    eax, 6
        jge    loc_1_4FA4
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        lea    edx, [eax + ecx + 8]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 0xc], al
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0xc3
        mov    byte ptr [ebp - 0xc], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        xor    al, 0x80
        mov    byte ptr [ebp - 0xc], al
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 4
        mov    al, byte ptr [ebp - 0xc]
        out    dx, al
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        lea    edx, [ecx + edx + 8]
        in     al, dx
        mov    byte ptr [ebp - 0xc], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        je     loc_1_4F9F
        jmp    loc_1_4FA4
loc_1_4F9F:
        jmp    loc_1_4F2E
loc_1_4FA4:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        cmp    eax, 6
        jne    loc_1_4FC3
        mov    edx, dword ptr [ebp + 0xc]
        add    edx, 4
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        xor    eax, eax
        jmp    loc_1_4FDC
loc_1_4FC3:
        mov    edx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xc3
        add    edx, 4
        out    dx, al
        mov    eax, 1
        jmp    loc_1_4FDC
loc_1_4FDC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_4FE3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x10]
        test   eax, eax
        je     loc_1_5378
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0xc], 1
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x10]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_502C
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x10]
        and    eax, 0xffff
        dec    eax
        or     al, 8
        out    0x96, al
        jmp    loc_1_503C
loc_1_502C:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x10]
        and    eax, 0xffff
        or     al, 8
        out    0x96, al
loc_1_503C:
        mov    edx, 0x101
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, 0x100
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        shl    eax, 8
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        or     eax, ecx
        mov    word ptr [ebp - 0xc], ax
        mov    ax, word ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1c], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 0x6ec6
        je     loc_1_508D
        xor    al, al
        out    0x96, al
        mov    eax, 0xffffffff
        jmp    loc_1_5E8C
loc_1_508D:
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x20], 5
        mov    edx, 0x102
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 0xf0
        mov    byte ptr [ebp - 4], cl
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        shl    ecx, 8
        add    ecx, 0x800
        mov    edx, dword ptr [ebp + 0x18]
        mov    word ptr [edx + 0xe], cx
        mov    edx, 0x105
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xc
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    loc_1_50EB
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 3
        jmp    loc_1_512C
loc_1_50EB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 4
        jne    loc_1_5107
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 4
        jmp    loc_1_512C
loc_1_5107:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 8
        jne    loc_1_5123
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 0xa
        jmp    loc_1_512C
loc_1_5123:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 0xf
loc_1_512C:
        mov    edx, 0x103
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 8
        je     loc_1_5181
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x80
        je     loc_1_5168
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0xd
        add    eax, 0xfd0000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_517C
loc_1_5168:
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0xd
        add    eax, 0xd0000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 4], eax
loc_1_517C:
        jmp    loc_1_51BB
loc_1_5181:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x80
        je     loc_1_51A7
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0xd
        add    eax, 0xfc0000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_51BB
loc_1_51A7:
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0xd
        add    eax, 0xc0000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 4], eax
loc_1_51BB:
        mov    al, byte ptr [ebp - 4]
        and    eax, 0x30
        mov    byte ptr [ebp - 4], al
        shr    byte ptr [ebp - 4], 4
        mov    eax, 8
        mov    cl, byte ptr [ebp - 4]
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x14], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 0x6ec6
        jne    loc_1_51F5
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x12], 0x40
loc_1_51F5:
        mov    edx, 0x104
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 8
        je     loc_1_5224
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0xd
        add    eax, 0xd0000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 8], eax
        jmp    loc_1_5238
loc_1_5224:
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0xd
        add    eax, 0xc0000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 8], eax
loc_1_5238:
        shr    byte ptr [ebp - 4], 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    loc_1_5257
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x18], 8
        jmp    loc_1_5298
loc_1_5257:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 1
        jne    loc_1_5273
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x18], 0x10
        jmp    loc_1_5298
loc_1_5273:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 2
        jne    loc_1_528F
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x18], 0x20
        jmp    loc_1_5298
loc_1_528F:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x18], 0
loc_1_5298:
        mov    edx, 0x105
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 3
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    loc_1_52C5
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x1e], 5
        jmp    loc_1_5306
loc_1_52C5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 1
        jne    loc_1_52E1
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x1e], 4
        jmp    loc_1_5306
loc_1_52E1:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 3
        jne    loc_1_52FD
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x1e], 6
        jmp    loc_1_5306
loc_1_52FD:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x1e], 7
loc_1_5306:
        mov    edx, 0x102
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 2
        jne    loc_1_5332
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xffff
        or     eax, 0x20
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
loc_1_5332:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    al, byte ptr [ebp + 0x14]
        push   eax
        mov    al, byte ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_44BC
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax + 3], 4
        je     loc_1_5368
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x21], 3
loc_1_5368:
        xor    al, al
        out    0x96, al
        xor    eax, eax
        jmp    loc_1_5E8C
        db  0E9h, 014h, 00Bh, 000h, 000h
loc_1_5378:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0xc], 0
        mov    byte ptr [ebp - 8], 0
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_5394
loc_1_5390:
        inc    word ptr [ebp - 0xc]
loc_1_5394:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 8
        jge    loc_1_53D6
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        lea    edx, [eax + ecx + 8]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        add    eax, ecx
        mov    byte ptr [ebp - 8], al
        jmp    loc_1_5390
loc_1_53D6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0xee
        je     loc_1_53F0
        mov    eax, 0xffffffff
        jmp    loc_1_5E8C
loc_1_53F0:
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    al, byte ptr [ebp + 0x14]
        push   eax
        mov    al, byte ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_44BC
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x70000
        cmp    eax, 0x20000
        jb     loc_1_548A
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x70000
        cmp    eax, 0x50000
        ja     loc_1_548A
        mov    eax, 8
        mov    edx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [edx]
        and    edx, 0x70000
        shr    edx, 0x10
        sub    edx, 2
        mov    cl, dl
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x12], ax
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax], 0x80
        je     loc_1_5477
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x14], 0x10
        jmp    loc_1_5485
loc_1_5477:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x14], ax
loc_1_5485:
        jmp    loc_1_5498
loc_1_548A:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x14], ax
loc_1_5498:
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax + 3], 4
        je     loc_1_54AC
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x21], 3
loc_1_54AC:
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax], 0x10
        jne    loc_1_5551
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x1e], 0xffff
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 8]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     loc_1_54F2
        mov    eax, 0xffffffff
        jmp    loc_1_5E8C
loc_1_54F2:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 9]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     loc_1_551C
        mov    eax, 0xffffffff
        jmp    loc_1_5E8C
loc_1_551C:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 0xa]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 3
        je     loc_1_5547
        mov    eax, 0xffffffff
        jmp    loc_1_5E8C
loc_1_5547:
        mov    eax, 1
        jmp    loc_1_5E8C
loc_1_5551:
        mov    eax, dword ptr [ebp + 0x18]
        test   dword ptr [eax], 0x3c00000
        jne    loc_1_556C
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x20], 1
        jmp    loc_1_5594
loc_1_556C:
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x3c00000
        cmp    eax, 0x1000000
        jne    loc_1_558D
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x20], 3
        jmp    loc_1_5594
loc_1_558D:
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x20], 2
loc_1_5594:
        mov    byte ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x3c00000
        cmp    eax, 0x1000000
        jne    loc_1_567F
        mov    byte ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        mov    al, byte ptr [ebp - 0x10]
        and    eax, 0xc3
        mov    byte ptr [ebp - 0x10], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x10]
        or     cl, 0x80
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 0x18]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        lea    edx, [ecx + 0xd]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xc
        mov    byte ptr [ebp - 4], al
        shr    byte ptr [ebp - 4], 2
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x40
        mov    byte ptr [ebp - 8], al
        shr    byte ptr [ebp - 8], 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, byte ptr [ebp - 4]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 0x10]
        out    dx, al
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax], 0x40
        je     loc_1_5663
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj1:0xFCC0
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x16], ax
        jmp    loc_1_567A
loc_1_5663:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> loc_1_FCB8
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x16], ax
loc_1_567A:
        jmp    loc_1_57C9
loc_1_567F:
        mov    eax, dword ptr [ebp + 0x18]
        test   dword ptr [eax], 0x3c00000
        je     loc_1_56AA
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 4
        mov    byte ptr [ebp - 4], al
loc_1_56AA:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x60
        mov    byte ptr [ebp - 8], al
        shr    byte ptr [ebp - 8], 5
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        jne    loc_1_5700
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    loc_1_56F2
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 2
        jmp    loc_1_56FB
loc_1_56F2:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 0xa
loc_1_56FB:
        jmp    loc_1_57C9
loc_1_5700:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 1
        jne    loc_1_5737
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    loc_1_5729
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 3
        jmp    loc_1_5732
loc_1_5729:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 0xb
loc_1_5732:
        jmp    loc_1_57C9
loc_1_5737:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 2
        jne    loc_1_5789
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    loc_1_577B
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax + 2], 0x20
        je     loc_1_576D
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 5
        jmp    loc_1_5776
loc_1_576D:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 4
loc_1_5776:
        jmp    loc_1_5784
loc_1_577B:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 0xf
loc_1_5784:
        jmp    loc_1_57C9
loc_1_5789:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 3
        jne    loc_1_57C0
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    loc_1_57B2
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 7
        jmp    loc_1_57BB
loc_1_57B2:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 4
loc_1_57BB:
        jmp    loc_1_57C9
loc_1_57C0:
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x16], 3
loc_1_57C9:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0x7fff
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x3c00000
        cmp    eax, 0x1000000
        jne    loc_1_5822
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x16]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_581D
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xffff
        or     eax, 0x8000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
loc_1_581D:
        jmp    loc_1_585A
loc_1_5822:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x80
        je     loc_1_585A
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xffff
        or     eax, 0x8000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
loc_1_585A:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dx, word ptr [eax + 0xe]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0x3f
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0x18]
        test   dword ptr [eax], 0x3c00000
        jne    loc_1_589A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 0x40
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        shl    eax, 0xd
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_59DB
loc_1_589A:
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x3c00000
        cmp    eax, 0x1000000
        jne    loc_1_5993
        mov    byte ptr [ebp - 0x14], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 0x14], al
        mov    al, byte ptr [ebp - 0x14]
        and    eax, 0xc3
        mov    byte ptr [ebp - 0x14], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x14]
        or     cl, 0x80
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 0x18]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        lea    edx, [ecx + 0xb]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 0x30
        mov    byte ptr [ebp - 4], cl
        shr    byte ptr [ebp - 4], 4
        mov    word ptr [ebp - 0xc], 8
        mov    cl, byte ptr [ebp - 4]
        shl    word ptr [ebp - 0xc], cl
        mov    cx, word ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp + 0x18]
        mov    word ptr [edx + 0x14], cx
        mov    ecx, dword ptr [ebp + 0x18]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        lea    edx, [ecx + 0xb]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xf
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0x40
        shr    eax, 2
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        or     eax, ecx
        or     al, 0x60
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 0xd
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 4], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x80
        je     loc_1_597B
        mov    eax, dword ptr [ebp + 0x18]
        or     dword ptr [eax + 4], 0xf00000
loc_1_597B:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 0x14]
        out    dx, al
        jmp    loc_1_59DB
loc_1_5993:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 5]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x1f
        mov    byte ptr [ebp - 8], al
        shl    byte ptr [ebp - 8], 3
        mov    al, byte ptr [ebp - 4]
        and    eax, 0x38
        shr    eax, 3
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0xd
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        shl    ecx, 0x10
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 4], eax
loc_1_59DB:
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x3c00000
        cmp    eax, 0x1000000
        jne    loc_1_5A76
        mov    byte ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 0x18], al
        mov    al, byte ptr [ebp - 0x18]
        and    eax, 0xc3
        mov    byte ptr [ebp - 0x18], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x18]
        or     cl, 0x80
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 0x18]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        lea    edx, [ecx + 0xc]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xf
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0x40
        shr    eax, 2
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        or     eax, ecx
        or     al, 0x60
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 0x18]
        out    dx, al
        jmp    loc_1_5A9C
loc_1_5A76:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 3]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0x3e
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 0x40
        mov    byte ptr [ebp - 4], al
loc_1_5A9C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        shl    eax, 0xd
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x3c00000
        cmp    eax, 0x1000000
        jne    loc_1_5B61
        mov    byte ptr [ebp - 0x1c], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 0x1c], al
        mov    al, byte ptr [ebp - 0x1c]
        and    eax, 0xc3
        mov    byte ptr [ebp - 0x1c], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x1c]
        or     cl, 0x80
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 0x18]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        lea    edx, [ecx + 0xc]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0x30
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x30
        jne    loc_1_5B34
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x18], 0
        jmp    loc_1_5B49
loc_1_5B34:
        shr    byte ptr [ebp - 4], 4
        mov    eax, 8
        mov    cl, byte ptr [ebp - 4]
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x18], ax
loc_1_5B49:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 0x1c]
        out    dx, al
        jmp    loc_1_5BAF
loc_1_5B61:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 3]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xc0
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    loc_1_5B9A
        mov    eax, dword ptr [ebp + 0x18]
        mov    word ptr [eax + 0x18], 0
        jmp    loc_1_5BAF
loc_1_5B9A:
        shr    byte ptr [ebp - 4], 6
        mov    eax, 8
        mov    cl, byte ptr [ebp - 4]
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x18], ax
loc_1_5BAF:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 7]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0x9fff
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0xf
        je     loc_1_5BFA
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xffff
        or     eax, 0x2000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
loc_1_5BFA:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xe7ff
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x3c00000
        cmp    eax, 0x1000000
        jne    loc_1_5CAF
        mov    byte ptr [ebp - 0x20], 0
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 0x20], al
        mov    al, byte ptr [ebp - 0x20]
        and    eax, 0xc3
        mov    byte ptr [ebp - 0x20], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x20]
        or     cl, 0x80
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 0x18]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        lea    edx, [ecx + 0xd]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x20
        je     loc_1_5C97
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xffff
        or     eax, 0x1800
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
loc_1_5C97:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 0x20]
        out    dx, al
        jmp    loc_1_5D2B
loc_1_5CAF:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 1
        je     loc_1_5CE7
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xffff
        or     eax, 0x1000
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
loc_1_5CE7:
        mov    eax, dword ptr [ebp + 0x18]
        test   byte ptr [eax], 0x40
        je     loc_1_5D2B
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 5]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x20
        je     loc_1_5D2B
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xffff
        or     eax, 0x800
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
loc_1_5D2B:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xf8ff
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        and    eax, 0x3c00000
        cmp    eax, 0x400000
        je     loc_1_5D83
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1e]
        and    eax, 0xffff
        or     eax, 0xffff
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1e], ax
        jmp    loc_1_5E85
loc_1_5D83:
        mov    al, byte ptr [ebp - 4]
        and    eax, 6
        mov    ecx, 6
        and    ecx, 0xfffffffe
        cmp    eax, ecx
        jne    loc_1_5DB4
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1e]
        and    eax, 0xffff
        or     eax, 6
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1e], ax
        jmp    loc_1_5E2C
loc_1_5DB4:
        mov    al, byte ptr [ebp - 4]
        and    eax, 6
        mov    ecx, 4
        and    ecx, 0xfffffffe
        cmp    eax, ecx
        jne    loc_1_5DE5
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1e]
        and    eax, 0xffff
        or     eax, 4
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1e], ax
        jmp    loc_1_5E2C
loc_1_5DE5:
        mov    al, byte ptr [ebp - 4]
        and    eax, 6
        mov    ecx, 2
        and    ecx, 0xfffffffe
        cmp    eax, ecx
        jne    loc_1_5E16
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1e]
        and    eax, 0xffff
        or     eax, 7
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1e], ax
        jmp    loc_1_5E2C
loc_1_5E16:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1e]
        and    eax, 0xffff
        or     eax, 5
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1e], ax
loc_1_5E2C:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        lea    edx, [eax + 7]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 2
        jne    loc_1_5E62
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0x1a]
        and    eax, 0xffff
        or     eax, 0x20
        mov    ecx, dword ptr [ebp + 0x18]
        mov    word ptr [ecx + 0x1a], ax
loc_1_5E62:
        mov    eax, dword ptr [ebp + 0x18]
        mov    ax, word ptr [eax + 0xe]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6647
        test   eax, eax
        je     loc_1_5E85
        mov    eax, 0x8a
        jmp    loc_1_5E8C
loc_1_5E85:
        xor    eax, eax
        jmp    loc_1_5E8C
loc_1_5E8C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
sub_1_5E93:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_1_D760
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_1_5ECC
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ecx], 0
        mov    ax, 0xffff
        jmp    loc_1_5FE1
loc_1_5ECC:
        mov    al, byte ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], al
        mov    al, byte ptr [ebp - 5]
        mov    byte ptr [ebp - 0xc], al
        mov    al, byte ptr [ebp - 6]
        mov    byte ptr [ebp - 0x10], al
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_5F5C
loc_1_5EF9:
        mov    al, byte ptr [ebp - 6]
        mov    byte ptr [ebp - 0xc], al
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0xfe
        mov    byte ptr [ebp - 0xc], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        shl    eax, 4
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ecx], ax
        jmp    loc_1_5FD9
loc_1_5F1D:
        mov    al, byte ptr [ebp - 6]
        mov    byte ptr [ebp - 0xc], al
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0xf0
        mov    byte ptr [ebp - 0xc], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        shl    eax, 8
        or     eax, 0x800
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ecx], ax
        jmp    loc_1_5FD9
loc_1_5F46:
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [eax], 0
        mov    ax, 0xffff
        jmp    loc_1_5FE1
        db  0E9h, 07Dh, 000h, 000h, 000h
loc_1_5F5C:
        cmp    dword ptr [ebp - 0x20], 0x6ec6
        jg     loc_1_5F8E
        je     loc_1_5F1D
        cmp    dword ptr [ebp - 0x20], 0x61c8
        jl     loc_1_5F46
        cmp    dword ptr [ebp - 0x20], 0x61c9
        jle    loc_1_5F1D
        jmp    loc_1_5F46
loc_1_5F8E:
        cmp    dword ptr [ebp - 0x20], 0xefd5
        jg     loc_1_5FC7
        cmp    dword ptr [ebp - 0x20], 0xefd4
        jge    loc_1_5F1D
        cmp    dword ptr [ebp - 0x20], 0x6fc0
        jl     loc_1_5F46
        cmp    dword ptr [ebp - 0x20], 0x6fc2
        jle    loc_1_5EF9
        jmp    loc_1_5F46
loc_1_5FC7:
        cmp    dword ptr [ebp - 0x20], 0xefe5
        je     loc_1_5EF9
        jmp    loc_1_5F46
loc_1_5FD9:
        xor    ax, ax
        jmp    loc_1_5FE1
loc_1_5FE1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_5FE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x18], 0
        mov    word ptr [ebp - 0x14], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xab]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x95]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_602A
        mov    eax, 0x13
        jmp    loc_1_62E2
loc_1_602A:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_604B
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x1ffd]
        and    eax, 0xff
        cmp    eax, 0x54
        jge    loc_1_6055
loc_1_604B:
        mov    eax, 0x12
        jmp    loc_1_62E2
loc_1_6055:
        mov    word ptr [ebp - 0x18], 0
        mov    word ptr [ebp - 0x10], 0
        jmp    loc_1_6075
loc_1_6066:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        add    eax, 2
        mov    word ptr [ebp - 0x10], ax
loc_1_6075:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 0x2000
        jge    loc_1_60B0
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + ecx]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 0x18], ax
        jmp    loc_1_6066
loc_1_60B0:
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_60CA
        mov    eax, 0x12
        jmp    loc_1_62E2
loc_1_60CA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A9B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_92CB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_984F
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6136
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x1ffd]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x95]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_62C6
loc_1_6136:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x46]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_1_61BE
loc_1_614A:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x198]
        and    eax, 0xfd
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x198], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 0x10]
        mov    al, byte ptr [ecx + 0x198]
        out    dx, al
        jmp    loc_1_61E1
loc_1_617F:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x198]
        and    eax, 0xfd
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x198], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 4]
        mov    al, byte ptr [ecx + 0x198]
        out    dx, al
        jmp    loc_1_61E1
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_61BE:
        cmp    dword ptr [ebp - 0x1c], 2
        je     loc_1_614A
        cmp    dword ptr [ebp - 0x1c], 3
        je     loc_1_617F
        cmp    dword ptr [ebp - 0x1c], 5
        je     loc_1_614A
        jmp    loc_1_61E1
loc_1_61E1:
        push   0x2000
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        call   sub_1_D8CC
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x46]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_6282
loc_1_620A:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x198]
        and    eax, 0xff
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x198], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 0x10]
        mov    al, byte ptr [ecx + 0x198]
        out    dx, al
        jmp    loc_1_62A5
loc_1_6241:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x198]
        and    eax, 0xff
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x198], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 4]
        mov    al, byte ptr [ecx + 0x198]
        out    dx, al
        jmp    loc_1_62A5
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_6282:
        cmp    dword ptr [ebp - 0x20], 2
        je     loc_1_620A
        cmp    dword ptr [ebp - 0x20], 3
        je     loc_1_6241
        cmp    dword ptr [ebp - 0x20], 5
        je     loc_1_620A
        jmp    loc_1_62A5
loc_1_62A5:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_984F
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_62C1
        mov    word ptr [ebp - 0x14], 8
loc_1_62C1:
        jmp    loc_1_62CC
loc_1_62C6:
        mov    word ptr [ebp - 0x14], 0x13
loc_1_62CC:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        jmp    loc_1_62E2
loc_1_62E2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_62E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0xe], 0
        mov    byte ptr [ebp - 8], 0
        mov    byte ptr [ebp - 7], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0xffff00ff
        mov    word ptr [ebp - 0x1c], ax
        mov    dword ptr [ebp - 0x24], 0
        mov    word ptr [ebp - 0x12], 3
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 0x1a], ax
        mov    word ptr [ebp - 0x16], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xffff00ff
        mov    word ptr [ebp - 0x18], ax
        mov    word ptr [ebp - 0xa], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A60
        lea    eax, [ebp - 0x28]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x50]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        dec    eax
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x4c]
        push   eax
        call   sub_1_4FE3
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1fc], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 6], 0
        mov    ax, word ptr [ebp - 0x1a]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 4], ax
        mov    ax, word ptr [ebp - 0x12]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x18], eax
        mov    ax, word ptr [ebp - 0x16]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x16], ax
        mov    ax, word ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x20], ax
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xe], eax
        mov    ax, word ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xc], ax
        mov    ax, word ptr [ebp - 0xa]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x44], ax
        mov    al, byte ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], al
        mov    ax, word ptr [ebp - 0xe]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x200], ax
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x46], ax
        mov    al, byte ptr [ebp - 7]
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x48], ax
        mov    ax, word ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_6450
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xaf], 3
        jmp    loc_1_6479
loc_1_6450:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x1fe], 8
        je     loc_1_646F
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xaf], 1
        jmp    loc_1_6479
loc_1_646F:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xaf], 2
loc_1_6479:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2AB0
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_649A
        mov    ax, 1
        jmp    loc_1_64B6
        db  0E9h, 013h, 000h, 000h, 000h
loc_1_649A:
        cmp    dword ptr [ebp - 4], 1
        jne    loc_1_64AD
        mov    ax, 2
        jmp    loc_1_64B6
loc_1_64AD:
        mov    ax, 0xffff
        jmp    loc_1_64B6
loc_1_64B6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_64BD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x208]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_1_64E2
        xor    ax, ax
        jmp    loc_1_6506
loc_1_64E2:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x208]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_1_64FE
        jmp    loc_1_64FE
loc_1_64FE:
        xor    ax, ax
        jmp    loc_1_6506
loc_1_6506:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x50]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_652E
        xor    ax, ax
        jmp    loc_1_652E
loc_1_652E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x50]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_6556
        xor    ax, ax
        jmp    loc_1_6575
loc_1_6556:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x20a]
        and    eax, 0xca
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x20a], al
        xor    ax, ax
        jmp    loc_1_6575
loc_1_6575:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x50]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_659D
        xor    ax, ax
        jmp    loc_1_659D
loc_1_659D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x50]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_65C5
        xor    ax, ax
        jmp    loc_1_65E6
loc_1_65C5:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x20a]
        and    eax, 0xff
        or     al, 0x35
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x20a], al
        xor    ax, ax
        jmp    loc_1_65E6
loc_1_65E6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x50]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_6615
        xor    ax, ax
        jmp    loc_1_6615
loc_1_6615:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x50]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_6640
        xor    ax, ax
        jmp    loc_1_6640
loc_1_6640:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_6647:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0xc], 0
        mov    byte ptr [ebp - 8], 0
        jmp    loc_1_6662
loc_1_665F:
        inc    byte ptr [ebp - 8]
loc_1_6662:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0x10
        jge    loc_1_678E
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        lea    edx, [eax + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 2
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        lea    edx, [eax + 1]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        lea    edx, [ecx + 3]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0xf
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 4
        or     al, byte ptr [ebp - 4]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        lea    edx, [eax + 3]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        lea    edx, [ecx + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 0x12
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        lea    edx, [eax + 1]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
loc_1_6709:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        lea    edx, [eax + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x10
        jne    loc_1_6709
        mov    byte ptr [ebp - 0x10], 0
        mov    word ptr [ebp - 0x14], 8
        jmp    loc_1_6738
loc_1_6734:
        inc    word ptr [ebp - 0x14]
loc_1_6738:
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 0x10
        jge    loc_1_6775
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        lea    edx, [eax + ecx]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        add    eax, ecx
        mov    byte ptr [ebp - 0x10], al
        jmp    loc_1_6734
loc_1_6775:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 0xc], ax
        jmp    loc_1_665F
loc_1_678E:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        lea    edx, [eax + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 0x10
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        lea    edx, [eax + 1]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xff
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 0xff
        jne    loc_1_67EA
        xor    eax, eax
        jmp    loc_1_67F4
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_67EA:
        mov    eax, 0x8a
        jmp    loc_1_67F4
loc_1_67F4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        db  000h
sub_1_67FC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0xaf], 1
        jne    loc_1_6819
        xor    edx, edx
        mov    dx, word ptr [eax + 4]
        add    edx, 5
        in     al, dx
        or     al, 0x80
        out    dx, al
loc_1_6819:
        xor    ax, ax
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_682C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0xaf], 1
        jne    loc_1_6849
        xor    edx, edx
        mov    dx, word ptr [eax + 4]
        add    edx, 5
        in     al, dx
        and    al, 0x7f
        out    dx, al
loc_1_6849:
        xor    ax, ax
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_685C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0xfffff
loc_1_686C:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_68A0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        je     loc_1_6898
        xor    ax, ax
        jmp    loc_1_68F6
loc_1_6898:
        dec    dword ptr [ebp - 4]
        jmp    loc_1_686C
loc_1_68A0:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_68AF
loc_1_68AC:
        inc    dword ptr [ebp - 4]
loc_1_68AF:
        cmp    dword ptr [ebp - 4], 0x3e8
        jae    loc_1_68ED
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        je     loc_1_68DE
        xor    ax, ax
        jmp    loc_1_68F6
loc_1_68DE:
        push   0x3e8
        call   sub_1_D7E0
        jmp    loc_1_68AC
loc_1_68ED:
        mov    ax, 8
        jmp    loc_1_68F6
loc_1_68F6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_68FD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0xfffff
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x46]
        and    eax, 0xffff
        cmp    eax, 3
        jne    loc_1_69B2
loc_1_6922:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        dec    dword ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 2
        je     loc_1_694F
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_6922
loc_1_694F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 2
        je     loc_1_69AD
        mov    dword ptr [ebp - 4], 0x3e8
        jmp    loc_1_696B
loc_1_6968:
        dec    dword ptr [ebp - 4]
loc_1_696B:
        cmp    dword ptr [ebp - 4], 0
        jbe    loc_1_69AD
        push   0x3e8
        call   sub_1_D7E0
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0xffff
        lea    edx, [ecx + 4]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 2
        jne    loc_1_69A8
        xor    ax, ax
        jmp    loc_1_6A5D
loc_1_69A8:
        jmp    loc_1_6968
loc_1_69AD:
        jmp    loc_1_6A3D
loc_1_69B2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        lea    edx, [eax + 0x10]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        dec    dword ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 1
        je     loc_1_69DF
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_69B2
loc_1_69DF:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 1
        je     loc_1_6A3D
        mov    dword ptr [ebp - 4], 0x3e8
        jmp    loc_1_69FB
loc_1_69F8:
        dec    dword ptr [ebp - 4]
loc_1_69FB:
        cmp    dword ptr [ebp - 4], 0
        jbe    loc_1_6A3D
        push   0x3e8
        call   sub_1_D7E0
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0xffff
        lea    edx, [ecx + 0x10]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 1
        jne    loc_1_6A38
        xor    ax, ax
        jmp    loc_1_6A5D
loc_1_6A38:
        jmp    loc_1_69F8
loc_1_6A3D:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_6A54
        xor    ax, ax
        jmp    loc_1_6A5D
        db  0E9h, 009h, 000h, 000h, 000h
loc_1_6A54:
        mov    ax, 8
        jmp    loc_1_6A5D
loc_1_6A5D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_6A7C
loc_1_6A79:
        inc    dword ptr [ebp - 4]
loc_1_6A7C:
        cmp    dword ptr [ebp - 4], 0xf000
        jae    loc_1_6A8E
        jmp    loc_1_6A79
loc_1_6A8E:
        xor    ax, ax
        jmp    loc_1_6A96
loc_1_6A96:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6A9B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        or     cl, 0x80
        mov    dx, word ptr [eax + 4]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        jmp    loc_1_6AC9
loc_1_6AC9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_6AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 0x7f
        mov    dx, word ptr [eax + 4]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        jmp    loc_1_6AFC
loc_1_6AFC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_6B03:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 0xbf
        mov    dx, word ptr [eax + 4]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        jmp    loc_1_6B32
loc_1_6B32:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_6B39:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_6B68
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_6BCC
loc_1_6B68:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_6B8E
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_6BCC
loc_1_6B8E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax], 0
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xd2]
        mov    word ptr [ecx + 8], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xd2]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        jmp    loc_1_6BCC
loc_1_6BCC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_6BD3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x46]
        and    eax, 0xffff
        cmp    eax, 3
        jne    loc_1_6C17
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 0x198]
        and    ecx, 0xff
        or     cl, 1
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        jmp    loc_1_6C54
loc_1_6C17:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 0x198]
        and    ecx, 0xff
        or     cl, 1
        lea    edx, [eax + 0x10]
        mov    al, cl
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 0x10]
        mov    al, byte ptr [ecx + 0x198]
        out    dx, al
loc_1_6C54:
        xor    ax, ax
        jmp    loc_1_6C5C
loc_1_6C5C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_6C63:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        movsx  eax, word ptr [ecx + eax*2 + 0x176]
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [edx + ecx*2 + 0x16a]
        and    ecx, 0xffff
        cmp    eax, ecx
        jl     loc_1_6CAB
        mov    eax, 0xffffffff
        jmp    loc_1_6F44
loc_1_6CAB:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        inc    eax
        and    eax, 0xfffe
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x17c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [edx + ecx*2 + 0x164]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_6D0C
        mov    eax, 0xffffffff
        jmp    loc_1_6F44
loc_1_6D0C:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x158]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [edx + ecx*4 + 0x14c]
        jb     loc_1_6DEF
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x14c]
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        sub    eax, dword ptr [edx + ecx*4 + 0x158]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x17c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [edx + ecx*2 + 0x164]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_6DCA
        mov    eax, 0xffffffff
        jmp    loc_1_6F44
loc_1_6DCA:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x140]
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x158], eax
loc_1_6DEF:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x17c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*2 + 0x17c], ax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        inc    word ptr [ecx + eax*2 + 0x176]
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0xf4]
        mov    word ptr [ecx + 0xc], ax
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0xf4]
        mov    word ptr [ecx + 0x1a], ax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xf4]
        mov    eax, dword ptr [eax + 0xe]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x158]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x158]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x158]
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff
        inc    ecx
        and    ecx, 0xfffe
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x158], eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xf4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xf4]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0xf4], eax
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_6F44
loc_1_6F44:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_6F4B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x200]
        and    eax, 0xffff
        test   al, 0x40
        je     loc_1_6F7F
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 2], 0x8002
        jmp    loc_1_6F88
loc_1_6F7F:
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 2], 0x8004
loc_1_6F88:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x16a]
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_1_6FDE
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x16]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xf
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx + 0x16]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x16]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xffff
        or     eax, 0x2000
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx + 0x16]
        mov    word ptr [ecx + 2], ax
loc_1_6FDE:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x182]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_7024
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*2 + 0x182], 1
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7034
        mov    word ptr [ebp - 4], ax
loc_1_7024:
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_702D
loc_1_702D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_7034:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7059
        mov    ax, 8
        jmp    loc_1_708F
loc_1_7059:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax], 0xc000
        mov    eax, 0x100
        mov    cl, byte ptr [ebp + 0xc]
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xc8]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    loc_1_708F
loc_1_708F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_7096:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        shl    eax, 0xa
        mov    word ptr [ebp - 0x18], ax
        push   0
        lea    eax, [ebp - 0x24]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_D810
loc_1_70CB:
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_1_71EB
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_D834
loc_1_70E6:
        cmp    dword ptr [ebp - 4], 0
        jbe    loc_1_71D9
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xb0]
        and    ecx, 0xffff
        and    eax, ecx
        mov    word ptr [ebp - 0x2c], ax
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x2c]
        and    ecx, 0xffff
        sub    eax, ecx
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_1_7154
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x2c]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    word ptr [ebp - 0x14], ax
        jmp    loc_1_715B
loc_1_7154:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ebp - 0x14], ax
loc_1_715B:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xb0]
        and    ecx, 0xffff
        and    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        call   sub_1_D8CC
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0x14]
        and    ecx, 0xffff
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        add    dword ptr [ebp + 0x10], eax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        add    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x14]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 0xc], ax
        jmp    loc_1_70E6
loc_1_71D9:
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_D858
        jmp    loc_1_70CB
loc_1_71EB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_7204
loc_1_7204:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_720B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbc]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0xbc], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_723F
loc_1_723F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_7246:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax], 0
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xd2]
        mov    word ptr [ecx + 8], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xd2]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xd2]
        mov    word ptr [ecx + 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        jmp    loc_1_72A6
loc_1_72A6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_72AD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_72C5
loc_1_72C1:
        inc    word ptr [ebp - 4]
loc_1_72C5:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, 3
        jge    loc_1_7362
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xe8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x11c]
        mov    dword ptr [ebp - 0x10], eax
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_730F
loc_1_730B:
        inc    word ptr [ebp - 0xc]
loc_1_730F:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x16a]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_735D
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xe], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        add    dword ptr [ebp - 8], 0x20
        add    dword ptr [ebp - 0x10], 0x20
        jmp    loc_1_730B
loc_1_735D:
        jmp    loc_1_72C1
loc_1_7362:
        xor    eax, eax
        jmp    loc_1_7369
loc_1_7369:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_7370:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x8001
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0x8000
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xa], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xe], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x14
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ebp - 4], 1
        jmp    loc_1_73E7
loc_1_73E3:
        inc    word ptr [ebp - 4]
loc_1_73E7:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xde]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_7468
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x8001
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0x8000
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xa], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xe], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x14
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_73E3
loc_1_7468:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xda], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xd6], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xe2], 0
        xor    eax, eax
        jmp    loc_1_74C9
loc_1_74C9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_74D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_74E8
loc_1_74E4:
        inc    word ptr [ebp - 4]
loc_1_74E8:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, 3
        jge    loc_1_7600
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x11c]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x20
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 2], eax
        mov    word ptr [ebp - 8], 1
        jmp    loc_1_7543
loc_1_753F:
        inc    word ptr [ebp - 8]
loc_1_7543:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x170]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_75AC
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx + 0x10]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x20
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 2], eax
        jmp    loc_1_753F
loc_1_75AC:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x11c]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x11c]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0x11c]
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_74E4
loc_1_7600:
        xor    eax, eax
        jmp    loc_1_7607
loc_1_7607:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_760E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_7626
loc_1_7622:
        inc    word ptr [ebp - 4]
loc_1_7626:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, 3
        jge    loc_1_77C7
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xe8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0x8000
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x20
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x12], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ebp - 0xc], 1
        jmp    loc_1_7693
loc_1_768F:
        inc    word ptr [ebp - 0xc]
loc_1_7693:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x16a]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_770E
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x12]
        mov    dword ptr [ecx + 0x16], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x12]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0x8000
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x20
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x12], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_768F
loc_1_770E:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xe8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x12], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xe8]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0xe8]
        mov    dword ptr [ecx + 0x16], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xe8]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x100], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xe8]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0xf4], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*2 + 0x176], 0
        jmp    loc_1_7622
loc_1_77C7:
        xor    eax, eax
        jmp    loc_1_77CE
loc_1_77CE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_77D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_77ED
loc_1_77E9:
        inc    word ptr [ebp - 8]
loc_1_77ED:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 2
        jge    loc_1_798B
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x128]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x188]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax], 0x8000
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0xa], 0x100
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x20
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 6], eax
        mov    word ptr [ebp - 0xc], 1
        jmp    loc_1_787E
loc_1_787A:
        inc    word ptr [ebp - 0xc]
loc_1_787E:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0xb6]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_790F
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ecx + 0x10]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x10], eax
        add    dword ptr [ebp - 4], 0x100
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0xa], 0x100
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x20
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 6], eax
        jmp    loc_1_787A
loc_1_790F:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x128]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x128]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0x128]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x128]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x138], eax
        jmp    loc_1_77E9
loc_1_798B:
        xor    eax, eax
        jmp    loc_1_7992
loc_1_7992:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_7999:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_79B1
loc_1_79AD:
        inc    word ptr [ebp - 4]
loc_1_79B1:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, 2
        jge    loc_1_7B15
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x10c]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0x8000
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x20
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x12], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ebp - 0xc], 1
        jmp    loc_1_7A1E
loc_1_7A1A:
        inc    word ptr [ebp - 0xc]
loc_1_7A1E:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0xb2]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_7A99
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x12]
        mov    dword ptr [ecx + 0x16], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x12]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0x4000
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x20
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x12], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_7A1A
loc_1_7A99:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x10c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x12], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x10c]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0x10c]
        mov    dword ptr [ecx + 0x16], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x10c]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x114], eax
        jmp    loc_1_79AD
loc_1_7B15:
        xor    eax, eax
        jmp    loc_1_7B1C
loc_1_7B1C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_7B23:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7B52
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7BDE
loc_1_7B52:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7B78
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7BDE
loc_1_7B78:
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_7B87
loc_1_7B83:
        inc    word ptr [ebp - 8]
loc_1_7B87:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_7BCA
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ax, word ptr [ecx + eax*2]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0xe4]
        mov    word ptr [edx + ecx*2], ax
        jmp    loc_1_7B83
loc_1_7BCA:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    loc_1_7BDE
loc_1_7BDE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_7BE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7C14
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7CD4
loc_1_7C14:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7C3A
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7CD4
loc_1_7C3A:
        mov    word ptr [ebp - 0xc], 0
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_7C5E
loc_1_7C4B:
        inc    word ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        add    eax, 2
        mov    word ptr [ebp - 8], ax
loc_1_7C5E:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_7CC0
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 0x10]
        mov    cl, byte ptr [ecx + edx + 1]
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0xe4]
        mov    word ptr [edx + ecx*2], ax
        jmp    loc_1_7C4B
loc_1_7CC0:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    loc_1_7CD4
loc_1_7CD4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_7CDB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7D0A
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7D44
loc_1_7D0A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7D30
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7D44
loc_1_7D30:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    loc_1_7D44
loc_1_7D44:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7D7A
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7DB5
loc_1_7D7A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7DA0
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7DB5
loc_1_7DA0:
        push   0x5412
        push   7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    loc_1_7DB5
loc_1_7DB5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_7DBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7DF7
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7F44
loc_1_7DF7:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7E1D
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_7F44
loc_1_7E1D:
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_7E2C
loc_1_7E28:
        inc    word ptr [ebp - 0xc]
loc_1_7E2C:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 3
        jge    loc_1_7E63
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xe8]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        add    dword ptr [ebp - 8], 4
        jmp    loc_1_7E28
loc_1_7E63:
        jmp    loc_1_7E6C
loc_1_7E68:
        inc    word ptr [ebp - 0xc]
loc_1_7E6C:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 8
        jge    loc_1_7E8F
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        add    dword ptr [ebp - 8], 4
        jmp    loc_1_7E68
loc_1_7E8F:
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_7E9E
loc_1_7E9A:
        inc    word ptr [ebp - 0xc]
loc_1_7E9E:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 2
        jge    loc_1_7EF6
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x10c]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        add    dword ptr [ebp - 8], 4
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x128]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        add    dword ptr [ebp - 8], 4
        jmp    loc_1_7E9A
loc_1_7EF6:
        jmp    loc_1_7EFF
loc_1_7EFB:
        inc    word ptr [ebp - 0xc]
loc_1_7EFF:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 2
        jge    loc_1_7F2F
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        add    dword ptr [ebp - 8], 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        add    dword ptr [ebp - 8], 4
        jmp    loc_1_7EFB
loc_1_7F2F:
        push   0
        push   0x86
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    loc_1_7F44
loc_1_7F44:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_7F4B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7F86
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_84E0
loc_1_7F86:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_7FAC
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_84E0
loc_1_7FAC:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x19a], 0x1024
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x19c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x44]
        and    eax, 0xffff
        cmp    eax, 4
        je     loc_1_7FEE
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x44]
        and    eax, 0xffff
        cmp    eax, 6
        jne    loc_1_800A
loc_1_7FEE:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x19a]
        and    eax, 0xffff
        or     eax, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x19a], ax
loc_1_800A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x200]
        and    eax, 0xffff
        test   al, 0x20
        je     loc_1_803F
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x19a]
        and    eax, 0xffff
        or     eax, 0x100
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x19a], ax
loc_1_803F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x200]
        and    eax, 0xffff
        test   al, 7
        je     loc_1_8074
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x19a]
        and    eax, 0xffff
        or     eax, 0x200
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x19a], ax
loc_1_8074:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x44]
        and    eax, 0xffff
        cmp    eax, 4
        je     loc_1_809E
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x44]
        and    eax, 0xffff
        cmp    eax, 6
        jne    loc_1_80BF
loc_1_809E:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x19c]
        and    eax, 0xffff
        or     eax, 3
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x19c], ax
        jmp    loc_1_80DB
loc_1_80BF:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x19c]
        and    eax, 0xffff
        or     eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x19c], ax
loc_1_80DB:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x19a]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x19c]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x44]
        and    eax, 0xffff
        cmp    eax, 5
        je     loc_1_812D
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x44]
        and    eax, 0xffff
        cmp    eax, 7
        jne    loc_1_81DA
loc_1_812D:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0xfa
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x2710
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x2710
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0xa28
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x6590
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x3a98
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x1b58
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0xa0
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x7d0
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0xa
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x1162
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x7d0
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x1388
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
        jmp    loc_1_8282
loc_1_81DA:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x3e8
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x9c40
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x9c40
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0xa28
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x6590
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x3a98
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x1b58
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0xa0
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x7d0
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0xa
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x4588
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x1f40
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0x4e20
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
loc_1_8282:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x24]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 0x25]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x26]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 0x27]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x28]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 0x29]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x97]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x97]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x99]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x99]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x9b]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x9b]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x9d]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x9d]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x9f]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x9f]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xa1]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xa1]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xa3]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xa3]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xa5]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xa5]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xa7]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xa9]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_84B3
loc_1_84B0:
        inc    dword ptr [ebp - 0xc]
loc_1_84B3:
        cmp    dword ptr [ebp - 0xc], 9
        jae    loc_1_84CE
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
        jmp    loc_1_84B0
loc_1_84CE:
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    loc_1_84E0
loc_1_84E0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_84E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc0]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc0]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xb0]
        and    ecx, 0xffff
        and    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc4]
        mov    word ptr [eax], 0xc00e
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xc4]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xc4]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xcc]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xc4]
        mov    dword ptr [ecx + 0xa], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc4]
        mov    word ptr [eax + 0xe], 0x3e
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax], 0x8000
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 8], 0
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_860D
loc_1_860A:
        inc    dword ptr [ebp - 8]
loc_1_860D:
        cmp    dword ptr [ebp - 8], 0x20
        jae    loc_1_863D
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xcc]
        mov    byte ptr [ecx + eax*2], 0xf0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xcc]
        mov    byte ptr [ecx + eax*2 + 1], 0
        jmp    loc_1_860A
loc_1_863D:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xd0], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7370
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_72AD
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_74D0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_760E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_77D5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7999
        xor    eax, eax
        jmp    loc_1_8686
loc_1_8686:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_868D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        add    eax, 0xfff6
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xc0], eax
        push   0x1c
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xc4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbc]
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0xbc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        push   0xa
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xc8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbc]
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0xbc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xde]
        and    eax, 0xffff
        shl    eax, 2
        lea    eax, [eax + eax*4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xd2], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbc]
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0xbc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        push   0x40
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xcc], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbc]
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0xbc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        push   0x80
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xe4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbc]
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0xbc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x16a]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xe8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x16c]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xec], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x16e]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xf0], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x170]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x11c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x172]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x120], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x174]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x124], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb2]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x10c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb4]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x110], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb6]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x128], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x130], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb8]
        and    eax, 0xffff
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x12c], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x134], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x164]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x140], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x140]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x158], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x14c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x168]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x148], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x148]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x160], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x154], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb6]
        and    eax, 0xffff
        shl    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x188], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x190], eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x166]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x144], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x144]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x15c], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x150], eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbc]
        add    eax, 0xff
        and    eax, 0xff00
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0xbc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb8]
        and    eax, 0xffff
        shl    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x18c], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x194], eax
        xor    eax, eax
        jmp    loc_1_8AEE
loc_1_8AEE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_8AF5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x114]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        movsx  ecx, word ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0x114]
        mov    dword ptr [ecx + 0xe], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_8B37
loc_1_8B37:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_8B3E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0x1c
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0xa
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xde]
        and    eax, 0xffff
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0x40
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0x80
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0xf
        and    eax, 0xfff0
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x16a]
        and    eax, 0xffff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x16c]
        and    eax, 0xffff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x16e]
        and    eax, 0xffff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x170]
        and    eax, 0xffff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x172]
        and    eax, 0xffff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x174]
        and    eax, 0xffff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb2]
        and    eax, 0xffff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb4]
        and    eax, 0xffff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb6]
        and    eax, 0xffff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x164]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x168]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb6]
        and    eax, 0xffff
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        inc    eax
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x166]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        inc    eax
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 0x100
        mov    word ptr [ebp - 4], ax
        mov    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, 0x120
        sub    edx, edx
        div    ecx
        jmp    loc_1_8E6E
loc_1_8E6E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_8E75:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xbc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xde], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x36]
        and    eax, 0xffff
        cmp    eax, 0x100
        jge    loc_1_8EC2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x36], 0x100
        jmp    loc_1_8EE2
loc_1_8EC2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x36]
        and    eax, 0xffff
        cmp    eax, 0x5000
        jle    loc_1_8EE2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x36], 0x5000
loc_1_8EE2:
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 0x36]
        mov    eax, 0x5000
        cdq    
        idiv   ecx
        dec    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x3a], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x3a]
        and    eax, 0xffff
        cmp    eax, 6
        jle    loc_1_8F1E
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3a], 6
        jmp    loc_1_8F3B
loc_1_8F1E:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x3a]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_8F3B
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x3a], 1
loc_1_8F3B:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x16e], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x174], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x168], 0x100
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x180], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x186], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x16a], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x170], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x164], 0x400
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x17c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x182], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x16c], 6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x172], 6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x166], 0x5000
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x17e], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x184], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xb2], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xb6], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xb4], 0x20
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8B3E
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xb8], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_868D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_84E7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7F4B
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_905E
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_908C
loc_1_905E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7DBC
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9084
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_908C
loc_1_9084:
        xor    ax, ax
        jmp    loc_1_908C
loc_1_908C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_9093:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_90C2
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_90FC
loc_1_90C2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax], 0xc000
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xe0], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    loc_1_90FC
loc_1_90FC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_9103:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9128
        mov    ax, 8
        jmp    loc_1_9176
loc_1_9128:
        movsx  eax, word ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_1_9148
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 4], 0x10
        jmp    loc_1_9157
loc_1_9148:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 4], 0x40
loc_1_9157:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax], 0xc000
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    loc_1_9176
loc_1_9176:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_917D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_91AC
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_91FA
loc_1_91AC:
        movsx  eax, word ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_1_91CC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 4], 0x20
        jmp    loc_1_91DB
loc_1_91CC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 4], 0x80
loc_1_91DB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax], 0xc000
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    loc_1_91FA
loc_1_91FA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_9201:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9230
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_925E
loc_1_9230:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax], 0x8001
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    loc_1_925E
loc_1_925E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_9265:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9294
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_92C4
loc_1_9294:
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xc8]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax], 0x8004
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    loc_1_92C4
loc_1_92C4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_92CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        or     cl, 0x40
        mov    dx, word ptr [eax + 4]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        jmp    loc_1_92F9
loc_1_92F9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_9300:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x46]
        and    eax, 0xffff
        cmp    eax, 3
        jne    loc_1_9366
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0xe0
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb0]
        and    eax, 0xffff
        not    eax
        and    eax, dword ptr [ebp + 0xc]
        shr    eax, 0xd
        and    eax, 0xff
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        jmp    loc_1_938E
loc_1_9366:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    ah, 0xc0
        mov    byte ptr [ebp - 8], ah
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        lea    edx, [eax + 0x11]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_938E:
        xor    ax, ax
        jmp    loc_1_9396
loc_1_9396:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_939D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A9B
        push   0x1388
        call   sub_1_D7E0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6AD0
        push   0x1388
        call   sub_1_D7E0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x46]
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_943C
loc_1_93E0:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 0x198]
        and    ecx, 0xff
        or     cl, 0xc
        lea    edx, [eax + 0x10]
        mov    al, cl
        out    dx, al
        jmp    loc_1_945F
loc_1_9409:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 0x198]
        and    ecx, 0xff
        or     cl, 4
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        jmp    loc_1_945F
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_943C:
        cmp    dword ptr [ebp - 8], 2
        je     loc_1_93E0
        cmp    dword ptr [ebp - 8], 3
        je     loc_1_9409
        cmp    dword ptr [ebp - 8], 5
        je     loc_1_93E0
        jmp    loc_1_945F
loc_1_945F:
        xor    ax, ax
        jmp    loc_1_9467
loc_1_9467:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_946E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_948A
loc_1_9483:
        add    dword ptr [ebp - 4], 0x1000
loc_1_948A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        shl    eax, 0xa
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_1_94CD
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + ecx]
        and    eax, 0xffff
        cmp    eax, 0xaa55
        jne    loc_1_94C8
        mov    ax, 0x85
        jmp    loc_1_94D5
loc_1_94C8:
        jmp    loc_1_9483
loc_1_94CD:
        xor    ax, ax
        jmp    loc_1_94D5
loc_1_94D5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_94DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_94F8
loc_1_94F1:
        add    dword ptr [ebp - 0xc], 0x400
loc_1_94F8:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        shl    eax, 0xa
        cmp    eax, dword ptr [ebp - 0xc]
        jbe    loc_1_9563
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        add    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 0x1234
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 0x1234
        jne    loc_1_955E
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        mov    ax, 0x84
        jmp    loc_1_956B
loc_1_955E:
        jmp    loc_1_94F1
loc_1_9563:
        xor    ax, ax
        jmp    loc_1_956B
loc_1_956B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_9572:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0x18], 0
        mov    word ptr [ebp - 0x2c], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x20]
        and    ecx, 0xffff
        mov    ax, word ptr [eax + 0x16]
        and    eax, 0xffff
        cdq    
        idiv   ecx
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0x1c], eax
        mov    word ptr [ebp - 0x24], 0
        jmp    loc_1_95CE
loc_1_95B9:
        inc    word ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0x2c]
        and    eax, 0xffff
        add    eax, 0x8000
        mov    word ptr [ebp - 0x2c], ax
loc_1_95CE:
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x24]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_9702
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        not    eax
        test   eax, eax
        je     loc_1_9702
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x24]
        and    ecx, 0xffff
        imul   eax, ecx
        shl    eax, 0xa
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0x1c]
        inc    eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    ax, word ptr [ebp - 0x2c]
        mov    word ptr [ebp - 0x28], ax
        mov    dword ptr [ebp - 0x20], 1
        jmp    loc_1_963A
loc_1_9636:
        add    dword ptr [ebp - 0x20], 2
loc_1_963A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        shl    eax, 0xa
        dec    eax
        cmp    eax, dword ptr [ebp - 0x20]
        jbe    loc_1_966A
        mov    ax, word ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    edx, dword ptr [ebp - 0x20]
        mov    word ptr [ecx + edx], ax
        inc    word ptr [ebp - 0x28]
        jmp    loc_1_9636
loc_1_966A:
        mov    ax, word ptr [ebp - 0x2c]
        mov    word ptr [ebp - 0x28], ax
        mov    dword ptr [ebp - 0x20], 1
        jmp    loc_1_9686
loc_1_967E:
        add    dword ptr [ebp - 0x20], 2
        inc    word ptr [ebp - 0x28]
loc_1_9686:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        shl    eax, 0xa
        dec    eax
        cmp    eax, dword ptr [ebp - 0x20]
        jbe    loc_1_96FD
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        not    eax
        test   eax, eax
        je     loc_1_96FD
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    ax, word ptr [eax + ecx]
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x28]
        and    ecx, 0xffff
        cmp    eax, ecx
        je     loc_1_96F8
        mov    byte ptr [ebp - 0x18], 1
        mov    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x10], eax
        mov    ax, word ptr [ebp - 0xc]
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 0x28]
        mov    word ptr [ebp - 4], ax
        mov    ax, 0x83
        jmp    loc_1_9848
loc_1_96F8:
        jmp    loc_1_967E
loc_1_96FD:
        jmp    loc_1_95B9
loc_1_9702:
        mov    word ptr [ebp - 0x24], 0
        jmp    loc_1_9711
loc_1_970D:
        inc    word ptr [ebp - 0x24]
loc_1_9711:
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x24]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_9830
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        not    eax
        test   eax, eax
        je     loc_1_9830
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x24]
        and    ecx, 0xffff
        imul   eax, ecx
        shl    eax, 0xa
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    word ptr [ebp - 0x28], 0xffff
        mov    dword ptr [ebp - 0x20], 0
        jmp    loc_1_977A
loc_1_9776:
        add    dword ptr [ebp - 0x20], 2
loc_1_977A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        shl    eax, 0xa
        cmp    eax, dword ptr [ebp - 0x20]
        jbe    loc_1_97A5
        mov    ax, word ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    edx, dword ptr [ebp - 0x20]
        mov    word ptr [ecx + edx], ax
        jmp    loc_1_9776
loc_1_97A5:
        mov    dword ptr [ebp - 0x20], 0
        jmp    loc_1_97B5
loc_1_97B1:
        add    dword ptr [ebp - 0x20], 2
loc_1_97B5:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x20]
        and    eax, 0xffff
        shl    eax, 0xa
        cmp    eax, dword ptr [ebp - 0x20]
        jbe    loc_1_982B
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        not    eax
        test   eax, eax
        je     loc_1_982B
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    ax, word ptr [eax + ecx]
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x28]
        and    ecx, 0xffff
        cmp    eax, ecx
        je     loc_1_9826
        mov    byte ptr [ebp - 0x18], 1
        mov    eax, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x10], eax
        mov    ax, word ptr [ebp - 0xc]
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 0x28]
        mov    word ptr [ebp - 4], ax
        mov    ax, 0x83
        jmp    loc_1_9848
loc_1_9826:
        jmp    loc_1_97B1
loc_1_982B:
        jmp    loc_1_970D
loc_1_9830:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        xor    ax, ax
        jmp    loc_1_9848
loc_1_9848:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_984F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A9B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_92CB
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x46]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_98F8
loc_1_9884:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x198]
        and    eax, 0xfd
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x198], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 0x10]
        mov    al, byte ptr [ecx + 0x198]
        out    dx, al
        jmp    loc_1_991B
loc_1_98B9:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x198]
        and    eax, 0xfd
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x198], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 4]
        mov    al, byte ptr [ecx + 0x198]
        out    dx, al
        jmp    loc_1_991B
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_98F8:
        cmp    dword ptr [ebp - 0xc], 2
        je     loc_1_9884
        cmp    dword ptr [ebp - 0xc], 3
        je     loc_1_98B9
        cmp    dword ptr [ebp - 0xc], 5
        je     loc_1_9884
        jmp    loc_1_991B
loc_1_991B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    al, byte ptr [eax + 0x1ffd]
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x95], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x95]
        and    eax, 0xffff
        cmp    eax, 0x54
        jge    loc_1_9957
        mov    ax, 0x12
        jmp    loc_1_9A68
loc_1_9957:
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_9971
loc_1_9962:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        add    eax, 2
        mov    word ptr [ebp - 4], ax
loc_1_9971:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, 0x2000
        jge    loc_1_99B0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    ax, word ptr [eax + ecx]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    eax, ecx
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_9962
loc_1_99B0:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x46]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_9A3C
loc_1_99C4:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x198]
        and    eax, 0xff
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x198], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 0x10]
        mov    al, byte ptr [ecx + 0x198]
        out    dx, al
        jmp    loc_1_9A5F
loc_1_99FB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x198]
        and    eax, 0xff
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x198], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 4]
        mov    al, byte ptr [ecx + 0x198]
        out    dx, al
        jmp    loc_1_9A5F
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_9A3C:
        cmp    dword ptr [ebp - 0x10], 2
        je     loc_1_99C4
        cmp    dword ptr [ebp - 0x10], 3
        je     loc_1_99FB
        cmp    dword ptr [ebp - 0x10], 5
        je     loc_1_99C4
        jmp    loc_1_9A5F
loc_1_9A5F:
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_9A68
loc_1_9A68:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_9A6F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B39
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9AA2
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_9B59
loc_1_9AA2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9AC8
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_9B59
loc_1_9AC8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        cmp    byte ptr [eax], 0
        je     loc_1_9AE3
        mov    ax, 0x82
        jmp    loc_1_9B59
loc_1_9AE3:
        push   3
        push   0x81
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B39
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9B10
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_9B59
loc_1_9B10:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9B36
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_9B59
loc_1_9B36:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        cmp    byte ptr [eax], 0
        je     loc_1_9B51
        mov    ax, 0x82
        jmp    loc_1_9B59
loc_1_9B51:
        xor    ax, ax
        jmp    loc_1_9B59
loc_1_9B59:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_9B60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x61], 0
        je     loc_1_9B8C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x61]
        add    dword ptr [ecx], eax
loc_1_9B8C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x65], 0
        je     loc_1_9BB5
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x65]
        add    dword ptr [ecx], eax
loc_1_9BB5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x69], 0
        je     loc_1_9BDC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x69]
        add    dword ptr [ecx], eax
loc_1_9BDC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x6d], 0
        je     loc_1_9C06
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xffff
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x6d]
        add    dword ptr [ecx], eax
loc_1_9C06:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x71], 0
        je     loc_1_9C30
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x71]
        add    dword ptr [ecx], eax
loc_1_9C30:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x75], 0
        je     loc_1_9C57
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x75]
        add    dword ptr [ecx], eax
loc_1_9C57:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x79], 0
        je     loc_1_9C81
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xffff
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x79]
        add    dword ptr [ecx], eax
loc_1_9C81:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x7d], 0
        je     loc_1_9CA8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x7d]
        add    dword ptr [ecx], eax
loc_1_9CA8:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x81], 0
        je     loc_1_9CD8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x81]
        add    dword ptr [ecx], eax
loc_1_9CD8:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x85], 0
        je     loc_1_9D08
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 0xa]
        and    eax, 0xffff
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x85]
        add    dword ptr [ecx], eax
loc_1_9D08:
        xor    ax, ax
        jmp    loc_1_9D10
loc_1_9D10:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_9D17:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x46]
        and    eax, 0xffff
        cmp    eax, 3
        jne    loc_1_9D5B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 0x198]
        and    ecx, 0xff
        or     cl, 4
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        jmp    loc_1_9D7F
loc_1_9D5B:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 0x198]
        and    ecx, 0xff
        or     cl, 8
        lea    edx, [eax + 0x10]
        mov    al, cl
        out    dx, al
loc_1_9D7F:
        xor    ax, ax
        jmp    loc_1_9D87
loc_1_9D87:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_9D8E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9DB3
        mov    ax, 8
        jmp    loc_1_9E09
loc_1_9DB3:
        mov    ax, word ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xc8]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        add    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xc8]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc8]
        mov    word ptr [eax], 0xa004
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    loc_1_9E09
loc_1_9E09:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_9E10:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x114]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax + 0xc]
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 2], 0x8000
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x16]
        mov    word ptr [eax + 2], 0x4000
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x114]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x114], eax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        sar    eax, 3
        mov    word ptr [ebp - 8], ax
        cmp    byte ptr [ebp - 0xc], 0
        je     loc_1_9EA6
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        add    eax, 0x20
        mov    word ptr [ebp - 8], ax
loc_1_9EA6:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffe0
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x138]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + eax*4 + 0x130], edx
        ja     loc_1_9F18
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x128]
        add    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        sub    eax, dword ptr [edx + ecx*4 + 0x130]
        mov    dword ptr [ebp - 4], eax
loc_1_9F18:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax], 0x8000
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x138]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x138], eax
        xor    ax, ax
        jmp    loc_1_9F5B
loc_1_9F5B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_9F62:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        movsx  eax, word ptr [ecx + eax*2 + 0x176]
        test   eax, eax
        jg     loc_1_9F91
        mov    ax, 8
        jmp    loc_1_A076
        db  0E9h, 0E5h, 000h, 000h, 000h
loc_1_9F91:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x17c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ecx + 0x1a]
        and    ecx, 0xffff
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*2 + 0x17c], ax
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x17c]
        and    eax, 0xffff
        test   eax, eax
        jge    loc_1_9FF5
        mov    ax, 8
        jmp    loc_1_A076
loc_1_9FF5:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x17c]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_A03A
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x140]
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x158], eax
loc_1_A03A:
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        dec    word ptr [ecx + eax*2 + 0x176]
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x100], eax
        xor    ax, ax
        jmp    loc_1_A076
loc_1_A076:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_A07D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
loc_1_A08C:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x100]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A1C0
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, 5
        je     loc_1_A1C0
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   ah, 0x7e
        je     loc_1_A0E0
        mov    ax, 8
        jmp    loc_1_A1C9
loc_1_A0E0:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x100]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F62
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A116
        mov    ax, 8
        jmp    loc_1_A1C9
loc_1_A116:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_A1BB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x200]
        and    eax, 0xffff
        test   al, 0x40
        je     loc_1_A1A3
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   al, 0x88
        jne    loc_1_A16B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0x10
        call   sub_1_CBF8
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_A19E
loc_1_A16B:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   al, 0x44
        jne    loc_1_A18F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0x11
        call   sub_1_CBF8
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_A19E
loc_1_A18F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0
        call   sub_1_CBF8
        mov    word ptr [ebp - 4], ax
loc_1_A19E:
        jmp    loc_1_A1B2
loc_1_A1A3:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0
        call   sub_1_CBF8
        mov    word ptr [ebp - 4], ax
loc_1_A1B2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
loc_1_A1BB:
        jmp    loc_1_A08C
loc_1_A1C0:
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A1C9
loc_1_A1C9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_A1D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        movsx  eax, word ptr [ecx + eax*2 + 0x176]
        test   eax, eax
        jle    loc_1_A240
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0x182]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_A240
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*2 + 0x182], 1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7034
        mov    word ptr [ebp - 4], ax
loc_1_A240:
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A249
loc_1_A249:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_A250:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xba]
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x114]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0xc], ax
loc_1_A286:
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x114]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A3EF
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        cmp    eax, 5
        je     loc_1_A3EF
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   al, 0x7f
        je     loc_1_A313
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x3e]
        and    eax, 0xffff
        test   al, 0x10
        jne    loc_1_A313
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9E10
        mov    ax, 8
        jmp    loc_1_A3F8
loc_1_A313:
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x114]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        sub    eax, 5
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_A361
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        push   eax
        call   sub_1_CDF8
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_A3C4
loc_1_A361:
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8AF5
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xb0]
        and    ecx, 0xffff
        and    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_C647
        mov    word ptr [ebp - 4], ax
loc_1_A3C4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9E10
        jmp    loc_1_A286
loc_1_A3EF:
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A3F8
loc_1_A3F8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_A3FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0x80
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        lea    eax, [ebp - 8]
        push   eax
        push   0xb601
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A45F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A55D
loc_1_A45F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A485
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A55D
loc_1_A485:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x9f
        push   eax
        push   0xb702
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A4C2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A55D
loc_1_A4C2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A4E8
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A55D
loc_1_A4E8:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x9d
        push   eax
        push   0xb601
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A525
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A55D
loc_1_A525:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A54B
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A55D
loc_1_A54B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A55D
loc_1_A55D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_A564:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        lea    eax, [ebp - 8]
        push   eax
        push   0xb301
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A5C4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A6C2
loc_1_A5C4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A5EA
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A6C2
loc_1_A5EA:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x99
        push   eax
        push   0xb402
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A627
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A6C2
loc_1_A627:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A64D
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A6C2
loc_1_A64D:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x97
        push   eax
        push   0xb301
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A68A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A6C2
loc_1_A68A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A6B0
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A6C2
loc_1_A6B0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_A6C2
loc_1_A6C2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_A6C9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        push   4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B39
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_A6F4
loc_1_A6F0:
        inc    word ptr [ebp - 8]
loc_1_A6F4:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 0x5dc
        jge    loc_1_A765
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_BA5F
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A760
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A73B
        mov    ax, 0x92
        jmp    loc_1_A7DD
        db  0E9h, 025h, 000h, 000h, 000h
loc_1_A73B:
        mov    word ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_BA5F
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A760
        mov    ax, 0x92
        jmp    loc_1_A7DD
loc_1_A760:
        jmp    loc_1_A6F0
loc_1_A765:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B8A2
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A79A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B8A2
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A79A
        mov    ax, 0x92
        jmp    loc_1_A7DD
loc_1_A79A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x114]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_A7CC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x118]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_A7D5
loc_1_A7CC:
        mov    ax, 0x92
        jmp    loc_1_A7DD
loc_1_A7D5:
        xor    ax, ax
        jmp    loc_1_A7DD
loc_1_A7DD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_A7E4:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    cl, byte ptr [eax]
        cmp    cl, 2
        jb     loc_1_A830
        cmp    cl, 0xfe
        ja     loc_1_A830
        mov    dl, byte ptr [ebp + 0x10]
        and    dl, 0xf0
        cmp    dl, 0x40
        je     loc_1_A828
        cmp    cl, 2
        jb     loc_1_A814
        cmp    cl, 0x14
        jae    loc_1_A814
        mov    ax, 0x8006
        pop    ebp
        ret    0xc
loc_1_A814:
        cmp    byte ptr [eax + 2], 0
        jne    loc_1_A820
        cmp    byte ptr [eax + 3], 0
        je     loc_1_A828
loc_1_A820:
        mov    ax, 0x8006
        pop    ebp
        ret    0xc
loc_1_A828:
        mov    ax, 1
        pop    ebp
        ret    0xc
loc_1_A830:
        mov    ax, 0x8005
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_A844:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [eax], 4
        je     loc_1_A85A
        mov    ax, 0x8005
        pop    esi
        pop    ebp
        ret    8
loc_1_A85A:
        add    eax, 2
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   0xbb01
        push   esi
        call   sub_1_7BE5
        push   esi
        call   sub_1_685C
        test   ax, ax
        jne    loc_1_A87B
        mov    ax, 1
loc_1_A87B:
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_A884:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [eax], 6
        je     loc_1_A89A
        mov    ax, 0x8005
        pop    esi
        pop    ebp
        ret    8
loc_1_A89A:
        add    eax, 2
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   0x3b02
        push   esi
        call   sub_1_7BE5
        push   esi
        call   sub_1_685C
        test   ax, ax
        jne    loc_1_A8BB
        mov    ax, 1
loc_1_A8BB:
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_A8C4:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
        cmp    byte ptr [eax], 4
        push   esi
        je     loc_1_A8DF
        mov    ax, 0x8005
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_A8DF:
        mov    cl, byte ptr [eax + 2]
        mov    al, byte ptr [eax + 3]
        and    ecx, 0xffff00ff
        and    eax, 0xffff00ff
        shl    cx, 8
        mov    esi, dword ptr [ebp + 8]
        or     cx, ax
        imul   cx, cx, 0xa
        mov    word ptr [ebp - 2], cx
        lea    ecx, [ebp - 2]
        push   ecx
        push   0xaa01
        push   esi
        call   sub_1_7B23
        push   esi
        call   sub_1_685C
        test   ax, ax
        jne    loc_1_A920
        mov    ax, 1
loc_1_A920:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_A934:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    byte ptr [eax], 4
        je     loc_1_A948
        mov    ax, 0x8005
        pop    ebp
        ret    0xc
loc_1_A948:
        mov    cl, byte ptr [eax + 2]
        mov    edx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff00ff
        mov    al, byte ptr [eax + 3]
        shl    cx, 8
        and    eax, 0xffff00ff
        or     cx, ax
        pop    ebp
        mov    ax, 1
        mov    word ptr [edx], cx
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_A974:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    byte ptr [eax], 4
        je     loc_1_A988
        mov    ax, 0x8005
        pop    ebp
        ret    8
loc_1_A988:
        mov    cl, byte ptr [eax + 2]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0xffff00ff
        mov    al, byte ptr [eax + 3]
        shl    cx, 8
        and    eax, 0xffff00ff
        or     cx, ax
        pop    ebp
        mov    ax, 1
        mov    word ptr [edx + 0x91], cx
        ret    8
        db  08Bh, 0FFh
sub_1_A9B4:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    byte ptr [eax], 4
        je     loc_1_A9C8
        mov    ax, 0x8005
        pop    ebp
        ret    8
loc_1_A9C8:
        mov    cl, byte ptr [eax + 2]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0xffff00ff
        mov    al, byte ptr [eax + 3]
        shl    cx, 8
        and    eax, 0xffff00ff
        or     cx, ax
        pop    ebp
        mov    ax, 1
        mov    word ptr [edx + 0x93], cx
        ret    8
        db  08Bh, 0FFh
sub_1_A9F4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x3e03
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA14:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x3b02
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA34:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0xb702
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA54:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0xb402
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA74:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0xb003
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA94:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        mov    ax, word ptr [esp + 0x14]
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], ah
        mov    byte ptr [ecx + 1], al
        mov    al, byte ptr [edx + 0x24]
        mov    byte ptr [ecx + 8], al
        mov    al, byte ptr [edx + 0x25]
        mov    byte ptr [ecx + 9], al
        mov    al, byte ptr [edx + 0x26]
        mov    byte ptr [ecx + 0xa], al
        mov    al, byte ptr [edx + 0x27]
        mov    byte ptr [ecx + 0xb], al
        mov    al, byte ptr [edx + 0x28]
        mov    byte ptr [ecx + 0xc], al
        xor    eax, eax
        mov    dl, byte ptr [edx + 0x29]
        mov    al, byte ptr [ecx + 0x11]
        mov    byte ptr [ecx + 0xd], dl
        cmp    eax, 0x20
        je     loc_1_AB09
        cmp    eax, 0x2a
        je     loc_1_AB26
        mov    edx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [edx + 9]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [edx + 0xa]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 0xb]
        pop    ebp
        mov    byte ptr [ecx + 5], al
        mov    al, byte ptr [edx + 0xc]
        mov    byte ptr [ecx + 6], al
        xor    ax, ax
        mov    dl, byte ptr [edx + 0xd]
        mov    byte ptr [ecx + 7], dl
        ret    0x10
loc_1_AB09:
        mov    byte ptr [ecx + 2], 0xc0
        mov    byte ptr [ecx + 7], 2
        xor    eax, eax
        pop    ebp
        mov    byte ptr [ecx + 3], al
        mov    byte ptr [ecx + 4], al
        mov    byte ptr [ecx + 5], al
        mov    byte ptr [ecx + 6], al
        xor    ax, ax
        ret    0x10
loc_1_AB26:
        mov    byte ptr [ecx + 2], 0xc0
        mov    byte ptr [ecx + 7], 0x10
        xor    eax, eax
        pop    ebp
        mov    byte ptr [ecx + 3], al
        mov    byte ptr [ecx + 4], al
        mov    byte ptr [ecx + 5], al
        mov    byte ptr [ecx + 6], al
        xor    ax, ax
        ret    0x10
        db  090h
sub_1_AB44:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    byte ptr [eax], 0xfa
        mov    byte ptr [eax + 1], 0x26
        xor    ax, ax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_AB64:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    cx, word ptr [esp + 0x10]
        mov    ebp, esp
        pop    ebp
        mov    byte ptr [eax], 4
        mov    byte ptr [eax + 2], ch
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [eax + 1], 9
        xor    ax, ax
        ret    0xc
sub_1_AB84:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    byte ptr [eax], 4
        mov    byte ptr [eax + 1], 0x2a
        mov    byte ptr [eax + 2], 1
        mov    cl, byte ptr [ebp + 0x10]
        pop    ebp
        mov    byte ptr [eax + 3], cl
        xor    ax, ax
        ret    0xc
        db  090h
sub_1_ABA4:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0xc]
        mov    byte ptr [ecx], 0xc
        mov    byte ptr [ecx + 2], al
        mov    byte ptr [ecx + 3], al
        mov    byte ptr [ecx + 4], al
        mov    byte ptr [ecx + 1], 0x23
        mov    byte ptr [ecx + 5], 0xe2
        mov    byte ptr [ecx + 6], 0xd4
        mov    byte ptr [ecx + 7], 0xc3
        mov    byte ptr [ecx + 8], 0x40
        mov    byte ptr [ecx + 9], 0xe5
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x95]
        shr    dx, 4
        sub    dl, 0x10
        pop    ebp
        mov    byte ptr [ecx + 0xa], dl
        mov    ax, word ptr [eax + 0x95]
        and    al, 0xf
        sub    al, 0x10
        mov    byte ptr [ecx + 0xb], al
        xor    ax, ax
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_AC04:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 0xc]
        mov    byte ptr [eax], 8
        mov    byte ptr [eax + 2], cl
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [eax + 4], cl
        pop    ebp
        mov    byte ptr [eax + 5], cl
        mov    byte ptr [eax + 6], cl
        mov    byte ptr [eax + 7], cl
        mov    byte ptr [eax + 1], 0x29
        xor    ax, ax
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_AC34:
        push   ebp
        mov    ecx, 0xf0
        mov    ebp, esp
        mov    eax, dword ptr [esp + 0xc]
        mov    byte ptr [eax], 0x14
        mov    byte ptr [eax + 2], cl
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [eax + 4], cl
        mov    byte ptr [eax + 5], cl
        mov    byte ptr [eax + 6], cl
        mov    byte ptr [eax + 7], cl
        mov    byte ptr [eax + 8], cl
        mov    byte ptr [eax + 9], cl
        mov    byte ptr [eax + 0xa], cl
        mov    byte ptr [eax + 0xb], cl
        mov    byte ptr [eax + 0xc], cl
        mov    byte ptr [eax + 0xd], cl
        mov    byte ptr [eax + 0xe], cl
        mov    byte ptr [eax + 0xf], cl
        mov    byte ptr [eax + 0x10], cl
        pop    ebp
        mov    byte ptr [eax + 0x11], cl
        mov    byte ptr [eax + 0x12], cl
        mov    byte ptr [eax + 0x13], cl
        mov    byte ptr [eax + 1], 0x22
        xor    ax, ax
        ret    8
sub_1_AC84:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_A9F4
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 1], 2
        mov    byte ptr [ecx], 8
        mov    eax, dword ptr [esi + 0xe4]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0xe4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0xe4]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0xe4]
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        mov    edx, dword ptr [esi + 0xe4]
        mov    ax, word ptr [edx + 4]
        mov    byte ptr [ecx + 6], ah
        mov    edx, dword ptr [esi + 0xe4]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 7], al
        xor    ax, ax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_ACF4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_AA14
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 1], 0xb
        mov    byte ptr [ecx], 6
        mov    eax, dword ptr [esi + 0xe4]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0xe4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0xe4]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0xe4]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        xor    ax, ax
        ret    8
        db  090h
sub_1_AD44:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_AA34
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 1], 0x2c
        mov    byte ptr [ecx], 6
        mov    eax, dword ptr [esi + 0xe4]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0xe4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0xe4]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0xe4]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        xor    ax, ax
        ret    8
        db  090h
sub_1_AD94:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_AA54
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 1], 0x2b
        mov    byte ptr [ecx], 6
        mov    eax, dword ptr [esi + 0xe4]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0xe4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0xe4]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0xe4]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        xor    ax, ax
        ret    8
        db  090h
sub_1_ADE4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_AA74
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 1], 0x28
        mov    byte ptr [ecx], 8
        mov    eax, dword ptr [esi + 0xe4]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0xe4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0xe4]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0xe4]
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        mov    edx, dword ptr [esi + 0xe4]
        mov    ax, word ptr [edx + 4]
        mov    byte ptr [ecx + 6], ah
        mov    edx, dword ptr [esi + 0xe4]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 7], al
        xor    ax, ax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_AE54:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    byte ptr [eax], 4
        mov    byte ptr [eax + 1], 6
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [ecx + 0x91]
        mov    byte ptr [eax + 2], dh
        mov    cl, byte ptr [ecx + 0x91]
        mov    byte ptr [eax + 3], cl
        xor    ax, ax
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_AE84:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    byte ptr [ecx], 4
        mov    byte ptr [ecx + 1], 7
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 0x93]
        mov    byte ptr [ecx + 2], dh
        mov    al, byte ptr [eax + 0x93]
        mov    byte ptr [ecx + 3], al
        xor    ax, ax
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_AEB4:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 0xc]
        pop    ebp
        mov    byte ptr [eax], 4
        mov    byte ptr [eax + 2], cl
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [eax + 1], 0x21
        xor    ax, ax
        ret    8
        db  08Dh, 049h, 000h
sub_1_AED4:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    cx, word ptr [esp + 0x10]
        mov    ebp, esp
        mov    byte ptr [eax], 6
        mov    byte ptr [eax + 2], ch
        mov    byte ptr [eax + 1], 0x20
        mov    byte ptr [eax + 3], cl
        mov    cx, word ptr [ebp + 0x14]
        pop    ebp
        mov    byte ptr [eax + 4], ch
        mov    byte ptr [eax + 5], cl
        xor    ax, ax
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_AF04:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0x1c
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    loc_1_AF2E
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_AF2E:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xe]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        mov    edi, dword ptr [ecx + 0xc]
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [edi + 0x11], 0
        mov    al, byte ptr [eax + 0x10]
        push   ecx
        shl    al, 4
        push   esi
        lea    ebx, [edi + 0x12]
        mov    byte ptr [edi + 0x10], al
        mov    word ptr [edi + 0xe], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 0x14]
        push   ecx
        push   ebx
        push   esi
        call   sub_1_AB64
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0xffff00ff
        add    word ptr [edi + 0xe], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        mov    cl, byte ptr [eax + 0x11]
        mov    dx, word ptr [ebp + 0x10]
        and    ecx, 0xffff00ff
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xffff00ff
        shl    ax, 8
        or     cx, ax
        push   ecx
        push   edx
        push   ebx
        push   esi
        call   sub_1_AED4
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0xffff00ff
        add    ax, word ptr [edi + 0xe]
        mov    word ptr [edi + 0xe], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0xe], cx
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_AFD4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0x34
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    loc_1_AFFE
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_AFFE:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xe]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        mov    edi, dword ptr [ecx + 0xc]
        push   edi
        push   eax
        mov    byte ptr [edi + 0x11], 0x22
        mov    cl, byte ptr [eax + 0x10]
        push   esi
        shl    cl, 4
        lea    ebx, [edi + 0x12]
        mov    byte ptr [edi + 0x10], cl
        mov    word ptr [edi + 0xe], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 0x10]
        push   ecx
        push   ebx
        push   esi
        call   sub_1_AB64
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0xffff00ff
        add    word ptr [edi + 0xe], ax
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        push   ebx
        push   esi
        call   sub_1_ACF4
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AC84
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AEB4
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AD94
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AD44
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0xffff00ff
        add    ax, word ptr [edi + 0xe]
        mov    word ptr [edi + 0xe], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0xe], cx
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_B0E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0x32
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    loc_1_B10E
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_B10E:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xe]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        mov    edi, dword ptr [ecx + 0xc]
        push   edi
        push   eax
        mov    byte ptr [edi + 0x11], 0x23
        mov    cl, byte ptr [eax + 0x10]
        push   esi
        shl    cl, 4
        lea    ebx, [edi + 0x12]
        mov    byte ptr [edi + 0x10], cl
        mov    word ptr [edi + 0xe], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 0x10]
        push   ecx
        push   ebx
        push   esi
        call   sub_1_AB64
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0xffff00ff
        add    word ptr [edi + 0xe], ax
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        push   ebx
        push   esi
        call   sub_1_ABA4
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AC04
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_ADE4
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0xffff00ff
        add    ax, word ptr [edi + 0xe]
        mov    word ptr [edi + 0xe], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0xe], cx
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_B1C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0x38
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    loc_1_B1EE
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_B1EE:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xe]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        mov    edi, dword ptr [ecx + 0xc]
        push   edi
        push   eax
        mov    byte ptr [edi + 0x11], 0x24
        mov    cl, byte ptr [eax + 0x10]
        push   esi
        shl    cl, 4
        lea    ebx, [edi + 0x12]
        mov    byte ptr [edi + 0x10], cl
        mov    word ptr [edi + 0xe], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 0x10]
        push   ecx
        push   ebx
        push   esi
        call   sub_1_AB64
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0xffff00ff
        add    word ptr [edi + 0xe], ax
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        push   ebx
        push   esi
        call   sub_1_AC34
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AD44
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AE54
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AE84
        mov    al, byte ptr [ebx]
        and    eax, 0xffff00ff
        add    ax, word ptr [edi + 0xe]
        mov    word ptr [edi + 0xe], ax
        mov    ch, al
        mov    cl, ah
        push   0
        mov    word ptr [edi + 0xe], cx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_B2C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0x17a]
        add    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x16e]
        and    ecx, 0xffff
        cmp    eax, ecx
        jl     loc_1_B303
        mov    word ptr [ebp - 8], 1
        jmp    loc_1_B59A
loc_1_B303:
        push   0x12
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 0x18], eax
        push   0x12
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0xe]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 0x60
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 1], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x24]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x25]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x26]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xa], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x27]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xb], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x28]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xc], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x29]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xd], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x24]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x25]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x26]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x27]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x28]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x29]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x11], 7
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0xe], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 0x10]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0xe], ax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0xe]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 0x60
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 1], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x24]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x25]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x26]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xa], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x27]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xb], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x28]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xc], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x29]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xd], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x24]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x25]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x26]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x27]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x28]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x29]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x11], 7
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0xe], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 0x10]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0xe], ax
        mov    eax, dword ptr [ebp - 0x14]
        mov    word ptr [eax + 2], 0x8000
        mov    eax, dword ptr [ebp - 0x18]
        mov    word ptr [eax + 2], 0x2000
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0x16]
        mov    word ptr [eax + 2], 0x2000
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x186], 1
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7034
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B545
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_B89B
loc_1_B545:
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_B554
loc_1_B550:
        inc    word ptr [ebp - 0xc]
loc_1_B554:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 0xffff
        jge    loc_1_B580
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        jne    loc_1_B580
        jmp    loc_1_B550
loc_1_B580:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        jne    loc_1_B59A
        mov    word ptr [ebp - 8], 1
loc_1_B59A:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_B893
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B5D1
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_B89B
loc_1_B5D1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B5F7
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_B89B
loc_1_B5F7:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax + 8], 0x85
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax + 0xa], 0xc503
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xd2]
        mov    word ptr [ecx + 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B665
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_B89B
loc_1_B665:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B68B
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_B89B
loc_1_B68B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, 4
        jne    loc_1_B6DC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xffff
        test   ah, ah
        jne    loc_1_B6DC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        cmp    eax, 0xfd00
        je     loc_1_B7F1
loc_1_B6DC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_B7E8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xffff
        test   ah, ah
        jne    loc_1_B7E8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        cmp    eax, 0xfd00
        jne    loc_1_B7E8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax + 8], 0x85
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax + 0xa], 0xf101
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xd2]
        mov    word ptr [ecx + 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B79A
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_B89B
loc_1_B79A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B7C0
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_B89B
loc_1_B7C0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x40
        jne    loc_1_B7E3
        mov    ax, 8
        jmp    loc_1_B89B
loc_1_B7E3:
        jmp    loc_1_B7F1
loc_1_B7E8:
        mov    ax, 8
        jmp    loc_1_B89B
loc_1_B7F1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    word ptr [eax], 0x100
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax + 8], 0x84
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd2]
        mov    word ptr [eax + 0xa], 0xc601
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xe4]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xd2]
        mov    word ptr [ecx + 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B86D
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_B89B
loc_1_B86D:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B893
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_B89B
loc_1_B893:
        xor    ax, ax
        jmp    loc_1_B89B
loc_1_B89B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_B8A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   0x12
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], -1
        jne    loc_1_B8CE
        mov    ax, 6
        jmp    loc_1_BA58
loc_1_B8CE:
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0xe]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 0x60
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 1], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x24]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x25]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x26]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xa], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x27]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xb], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x28]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xc], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x29]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0xd], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x24]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x25]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x26]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x27]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x28]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x29]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x11], 7
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax + 0xe], 4
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 0xc]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0xc]
        mov    word ptr [ecx + 0xe], ax
        push   0
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F4B
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_B9E2
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_BA58
loc_1_B9E2:
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_B9F1
loc_1_B9ED:
        inc    word ptr [ebp - 8]
loc_1_B9F1:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 0xffff
        jge    loc_1_BA1D
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        jne    loc_1_BA1D
        jmp    loc_1_B9ED
loc_1_BA1D:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        je     loc_1_BA47
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   eax, 0x1cc
        je     loc_1_BA50
loc_1_BA47:
        mov    ax, 9
        jmp    loc_1_BA58
loc_1_BA50:
        xor    ax, ax
        jmp    loc_1_BA58
loc_1_BA58:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_BA5F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   0x206
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], -1
        jne    loc_1_BA8E
        mov    ax, 6
        jmp    loc_1_BC88
loc_1_BA8E:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0xe]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 0x60
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 3], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 6], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 7], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x24]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x25]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x26]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xa], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x27]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xb], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x28]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xc], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x29]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0xd], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x11], 8
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0xe], 4
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x12
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AB44
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0x10]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0xe], ax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AB44
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0x10]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0xe], ax
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 0x10]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0xe], ax
        push   0
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F4B
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_BBF7
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_BC88
loc_1_BBF7:
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_BC06
loc_1_BC02:
        inc    word ptr [ebp - 0xc]
loc_1_BC06:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 0xffff
        jge    loc_1_BC32
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        jne    loc_1_BC32
        jmp    loc_1_BC02
loc_1_BC32:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        je     loc_1_BC5C
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   eax, 0x1cc
        je     loc_1_BC65
loc_1_BC5C:
        mov    ax, 0x92
        jmp    loc_1_BC88
loc_1_BC65:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x182], 0
        push   0
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F62
        xor    ax, ax
        jmp    loc_1_BC88
loc_1_BC88:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_BC8F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0x3e
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    loc_1_BCB9
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_BCB9:
        mov    eax, dword ptr [ebp - 4]
        push   0x6000
        mov    ecx, dword ptr [eax + 0xe]
        mov    edi, dword ptr [ecx + 0xc]
        push   edi
        push   0
        push   esi
        lea    ebx, [edi + 0x12]
        mov    byte ptr [edi + 0x11], 0x20
        mov    byte ptr [edi + 0x10], 0x50
        mov    word ptr [edi + 0xe], 4
        call   sub_1_AA94
        push   ebx
        push   esi
        call   sub_1_AC34
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0xffff00ff
        add    word ptr [edi + 0xe], ax
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        push   ebx
        push   esi
        call   sub_1_AC84
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_ABA4
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0xffff00ff
        add    word ptr [edi + 0xe], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AEB4
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0xffff00ff
        add    ax, word ptr [edi + 0xe]
        mov    word ptr [edi + 0xe], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0xe], cx
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_BD64:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x14], 1
        mov    word ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, word ptr [eax + 0xe]
        sub    eax, 4
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x12
        mov    dword ptr [ebp - 4], eax
loc_1_BD97:
        movsx  eax, word ptr [ebp - 0xc]
        test   eax, eax
        jle    loc_1_BE40
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_BE40
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_BDF5
loc_1_BDC7:
        or     dword ptr [ebp - 0x10], 0x40
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A934
        mov    word ptr [ebp - 0x14], ax
        jmp    loc_1_BE04
loc_1_BDE5:
        mov    word ptr [ebp - 0x14], 0x8008
        jmp    loc_1_BE04
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_1_BDF5:
        cmp    dword ptr [ebp - 0x18], 9
        je     loc_1_BDC7
        jmp    loc_1_BDE5
loc_1_BE04:
        movsx  eax, word ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0xff
        sub    eax, ecx
        mov    word ptr [ebp - 0xc], ax
        movsx  eax, word ptr [ebp - 0xc]
        test   eax, eax
        jge    loc_1_BE2B
        mov    word ptr [ebp - 0x14], 0x8002
loc_1_BE2B:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_BD97
loc_1_BE40:
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_BE73
        movsx  eax, word ptr [ebp - 0xc]
        test   eax, eax
        je     loc_1_BE68
        mov    word ptr [ebp - 0x14], 0x8002
        jmp    loc_1_BE73
loc_1_BE68:
        jmp    loc_1_BE73
        db  066h, 0C7h, 045h, 0ECh, 007h, 080h
loc_1_BE73:
        mov    ax, word ptr [ebp - 0x14]
        jmp    loc_1_BE7C
loc_1_BE7C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_BE83:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x10], 1
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xf
        cmp    al, 4
        je     loc_1_BEB3
        mov    ax, 0x8004
        jmp    loc_1_C056
loc_1_BEB3:
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, word ptr [eax + 0xe]
        sub    eax, 4
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x12
        mov    dword ptr [ebp - 4], eax
loc_1_BECA:
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jle    loc_1_C01A
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_C01A
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_BFAA
loc_1_BEFA:
        or     dword ptr [ebp - 0xc], 0x40
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A934
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_BFDE
loc_1_BF18:
        or     dword ptr [ebp - 0xc], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A844
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_BFDE
loc_1_BF32:
        or     dword ptr [ebp - 0xc], 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A884
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_BFDE
loc_1_BF4C:
        or     dword ptr [ebp - 0xc], 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A8C4
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_BFDE
loc_1_BF66:
        or     dword ptr [ebp - 0xc], 0x10
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A974
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_BFDE
loc_1_BF80:
        or     dword ptr [ebp - 0xc], 0x20
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B4
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_BFDE
loc_1_BF9A:
        mov    word ptr [ebp - 0x10], 0x8008
        jmp    loc_1_BFDE
        db  0E9h, 034h, 000h, 000h, 000h
loc_1_BFAA:
        sub    dword ptr [ebp - 0x14], 3
        cmp    dword ptr [ebp - 0x14], 6
        ja     loc_1_BF9A
        mov    eax, dword ptr [ebp - 0x14]
        jmp    dword ptr [eax*4]             ; -> obj1:0xBFC2
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_BF18
        dd  loc_1_BF32
        dd  loc_1_BF4C
        dd  loc_1_BF66
        dd  loc_1_BF80
        dd  loc_1_BF9A
        dd  loc_1_BEFA
loc_1_BFDE:
        movsx  eax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0xff
        sub    eax, ecx
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jge    loc_1_C005
        mov    word ptr [ebp - 0x10], 0x8002
loc_1_C005:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_BECA
loc_1_C01A:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_C04D
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        je     loc_1_C042
        mov    word ptr [ebp - 0x10], 0x8002
        jmp    loc_1_C04D
loc_1_C042:
        jmp    loc_1_C04D
        db  066h, 0C7h, 045h, 0F0h, 007h, 080h
loc_1_C04D:
        mov    ax, word ptr [ebp - 0x10]
        jmp    loc_1_C056
loc_1_C056:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_C05D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x10], 1
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xf
        cmp    al, 5
        je     loc_1_C08D
        mov    ax, 0x8004
        jmp    loc_1_C1FC
loc_1_C08D:
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, word ptr [eax + 0xe]
        sub    eax, 4
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x12
        mov    dword ptr [ebp - 4], eax
loc_1_C0A4:
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jle    loc_1_C1C0
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_C1C0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_C150
loc_1_C0D4:
        or     dword ptr [ebp - 0xc], 0x40
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A934
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_C184
loc_1_C0F2:
        or     dword ptr [ebp - 0xc], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A844
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_C184
loc_1_C10C:
        or     dword ptr [ebp - 0xc], 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A884
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_C184
loc_1_C126:
        or     dword ptr [ebp - 0xc], 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A8C4
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_C184
loc_1_C140:
        mov    word ptr [ebp - 0x10], 0x8008
        jmp    loc_1_C184
        db  0E9h, 034h, 000h, 000h, 000h
loc_1_C150:
        sub    dword ptr [ebp - 0x14], 3
        cmp    dword ptr [ebp - 0x14], 6
        ja     loc_1_C140
        mov    eax, dword ptr [ebp - 0x14]
        jmp    dword ptr [eax*4]             ; -> obj1:0xC168
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_C0F2
        dd  loc_1_C10C
        dd  loc_1_C126
        dd  loc_1_C140
        dd  loc_1_C140
        dd  loc_1_C140
        dd  loc_1_C0D4
loc_1_C184:
        movsx  eax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0xff
        sub    eax, ecx
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jge    loc_1_C1AB
        mov    word ptr [ebp - 0x10], 0x8002
loc_1_C1AB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_C0A4
loc_1_C1C0:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_C1F3
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        je     loc_1_C1E8
        mov    word ptr [ebp - 0x10], 0x8002
        jmp    loc_1_C1F3
loc_1_C1E8:
        jmp    loc_1_C1F3
        db  066h, 0C7h, 045h, 0F0h, 007h, 080h
loc_1_C1F3:
        mov    ax, word ptr [ebp - 0x10]
        jmp    loc_1_C1FC
loc_1_C1FC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_C203:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x10], 1
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        and    eax, 0xf
        cmp    al, 4
        je     loc_1_C233
        mov    ax, 0x8004
        jmp    loc_1_C33A
loc_1_C233:
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, word ptr [eax + 0xe]
        sub    eax, 4
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x12
        mov    dword ptr [ebp - 4], eax
loc_1_C24A:
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jle    loc_1_C2F9
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_C2F9
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_C2AE
loc_1_C27A:
        or     dword ptr [ebp - 0xc], 0x8000
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A7E4
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_C2BD
loc_1_C29E:
        mov    word ptr [ebp - 0x10], 0x8008
        jmp    loc_1_C2BD
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_1_C2AE:
        cmp    dword ptr [ebp - 0x14], 0x27
        je     loc_1_C27A
        jmp    loc_1_C29E
loc_1_C2BD:
        movsx  eax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0xff
        sub    eax, ecx
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jge    loc_1_C2E4
        mov    word ptr [ebp - 0x10], 0x8002
loc_1_C2E4:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_C24A
loc_1_C2F9:
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        cmp    eax, 1
        jne    loc_1_C331
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        je     loc_1_C321
        mov    word ptr [ebp - 0x10], 0x8002
        jmp    loc_1_C331
loc_1_C321:
        test   byte ptr [ebp - 0xb], 0x80
        jne    loc_1_C331
        mov    word ptr [ebp - 0x10], 0x8007
loc_1_C331:
        mov    ax, word ptr [ebp - 0x10]
        jmp    loc_1_C33A
loc_1_C33A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_C341:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        cmp    eax, 0x12
        jg     loc_1_C367
        xor    ax, ax
        jmp    loc_1_C4EC
loc_1_C367:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], -1
        jne    loc_1_C389
        xor    ax, ax
        jmp    loc_1_C4EC
loc_1_C389:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx + 0xe]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x14
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx + 0xe]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        sub    eax, 6
        mov    ecx, dword ptr [ebp - 0xc]
        mov    word ptr [ecx + 0xc], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xffff
        sub    eax, 6
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx + 0xe]
        mov    word ptr [ecx + 0xa], ax
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F4B
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_C408
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_C4EC
loc_1_C408:
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_C417
loc_1_C413:
        inc    word ptr [ebp - 8]
loc_1_C417:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 0xffff
        jge    loc_1_C443
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        jne    loc_1_C443
        jmp    loc_1_C413
loc_1_C443:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        jne    loc_1_C4D7
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7034
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_C47F
        mov    ax, word ptr [ebp - 4]
        jmp    loc_1_C4EC
loc_1_C47F:
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_C48E
loc_1_C48A:
        inc    word ptr [ebp - 8]
loc_1_C48E:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        cmp    eax, 0xffff
        jge    loc_1_C4BA
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        jne    loc_1_C4BA
        jmp    loc_1_C48A
loc_1_C4BA:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   ah, 0x80
        jne    loc_1_C4D7
        mov    ax, 8
        jmp    loc_1_C4EC
loc_1_C4D7:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ecx], ax
        mov    ax, 0x8888
        jmp    loc_1_C4EC
loc_1_C4EC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_C4F3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0x16
        push   0
        push   eax
        call   sub_1_6C63
        cmp    eax, -1
        mov    esi, eax
        jne    loc_1_C518
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_C518:
        mov    eax, dword ptr [esi + 0xe]
        mov    edi, dword ptr [eax + 0xc]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   edi
        mov    byte ptr [edi + 0x11], 0x2a
        mov    cl, byte ptr [eax + 0x10]
        push   eax
        shl    cl, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    ebx, [edi + 0x12]
        mov    byte ptr [edi + 0x10], cl
        mov    word ptr [edi + 0xe], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        push   ebx
        push   eax
        call   sub_1_AB84
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0xffff00ff
        push   esi
        add    ax, word ptr [edi + 0xe]
        mov    word ptr [edi + 0xe], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [edi + 0xe], cx
        push   eax
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_C584:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 0xc]
        movsx  eax, word ptr [eax + 0xe]
        sub    eax, 4
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x12
        mov    dword ptr [ebp - 4], eax
loc_1_C5AC:
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jle    loc_1_C637
        mov    eax, dword ptr [ebp + 0x10]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_C637
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_C5FE
loc_1_C5DE:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A934
        jmp    loc_1_C60D
        db  0E9h, 014h, 000h, 000h, 000h, 0E9h, 00Fh, 000h, 000h, 000h
loc_1_C5FE:
        cmp    dword ptr [ebp - 0xc], 9
        je     loc_1_C5DE
        jmp    loc_1_C60D
loc_1_C60D:
        movsx  eax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0xff
        sub    eax, ecx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_C5AC
loc_1_C637:
        mov    ax, 0x8003
        jmp    loc_1_C640
loc_1_C640:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_C647:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    word ptr [ebp - 0xc], 0
        mov    byte ptr [ebp - 0x10], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xe], ax
        mov    eax, dword ptr [ebp + 0x14]
        and    eax, 0xffff
        test   ah, 4
        je     loc_1_CAE3
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_CA6D
loc_1_C6AC:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_C05D
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_C6DB
        mov    ax, word ptr [ebp - 0x14]
        jmp    loc_1_CBF1
loc_1_C6DB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_AF04
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_C70D
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_CBF1
loc_1_C70D:
        jmp    loc_1_CADE
loc_1_C712:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_BE83
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_C741
        mov    ax, word ptr [ebp - 0x14]
        jmp    loc_1_CBF1
loc_1_C741:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_AF04
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_C773
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_CBF1
loc_1_C773:
        jmp    loc_1_CADE
loc_1_C778:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_BD64
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_1_C7D7
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_C7BD
        mov    ax, word ptr [ebp - 0x14]
        jmp    loc_1_CBF1
        db  0E9h, 01Ah, 000h, 000h, 000h
loc_1_C7BD:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_AF04
        jmp    loc_1_CBF1
loc_1_C7D7:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_AFD4
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_C805
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_CBF1
loc_1_C805:
        jmp    loc_1_CADE
loc_1_C80A:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_BD64
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_1_C869
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_C84F
        mov    ax, word ptr [ebp - 0x14]
        jmp    loc_1_CBF1
        db  0E9h, 01Ah, 000h, 000h, 000h
loc_1_C84F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_AF04
        jmp    loc_1_CBF1
loc_1_C869:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_B1C4
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_C897
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_CBF1
loc_1_C897:
        jmp    loc_1_CADE
loc_1_C89C:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_BD64
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_1_C8FB
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_C8E1
        mov    ax, word ptr [ebp - 0x14]
        jmp    loc_1_CBF1
        db  0E9h, 01Ah, 000h, 000h, 000h
loc_1_C8E1:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_AF04
        jmp    loc_1_CBF1
loc_1_C8FB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_B0E4
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_C929
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_CBF1
loc_1_C929:
        jmp    loc_1_CADE
loc_1_C92E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_C203
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 1
        je     loc_1_C989
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_C96F
        mov    ax, word ptr [ebp - 0x14]
        jmp    loc_1_CBF1
        db  0E9h, 01Ah, 000h, 000h, 000h
loc_1_C96F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_AF04
        jmp    loc_1_CBF1
loc_1_C989:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_C341
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_C9B8
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_CBF1
loc_1_C9B8:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 0x8888
        jne    loc_1_C9FA
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_C4F3
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 8
        jne    loc_1_C9FA
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_CBF1
loc_1_C9FA:
        jmp    loc_1_CADE
loc_1_C9FF:
        jmp    loc_1_CADE
loc_1_CA04:
        mov    byte ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    ax, word ptr [eax + 0x3e]
        and    eax, 0xffff
        test   al, 0x20
        jne    loc_1_CA63
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_C584
        mov    word ptr [ebp - 0x14], ax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_AF04
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_CA63
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_CBF1
loc_1_CA63:
        jmp    loc_1_CADE
        db  0E9h, 071h, 000h, 000h, 000h
loc_1_CA6D:
        cmp    dword ptr [ebp - 0x18], 0x29
        ja     loc_1_CA04
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax]            ; -> obj1:0xCAB4
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0xCA8C
        ; ---- dispatch table (10 entries) ----
        dd  loc_1_C9FF
        dd  loc_1_C92E
        dd  loc_1_C712
        dd  loc_1_C6AC
        dd  loc_1_C778
        dd  loc_1_C89C
        dd  loc_1_C80A
        dd  loc_1_C9FF
        dd  loc_1_C9FF
        dd  loc_1_CA04
        db  000h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 001h, 009h, 009h, 002h, 003h, 004h, 005h
        db  006h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h
        db  007h, 009h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h
loc_1_CADE:
        jmp    loc_1_CB66
loc_1_CAE3:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x11]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_1_CB09
loc_1_CAF6:
        jmp    loc_1_CB66
loc_1_CAFB:
        mov    byte ptr [ebp - 0x10], 0
        jmp    loc_1_CB66
        db  0E9h, 05Dh, 000h, 000h, 000h
loc_1_CB09:
        cmp    dword ptr [ebp - 0x1c], 0x29
        ja     loc_1_CAFB
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax]            ; -> obj1:0xCB3C
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0xCB28
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_CAF6
        dd  loc_1_CAF6
        dd  loc_1_CAF6
        dd  loc_1_CAF6
        dd  loc_1_CAFB
        db  000h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 001h, 001h, 001h, 001h
        db  001h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  002h, 004h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
loc_1_CB66:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ax, word ptr [eax + 0x3e]
        and    eax, 0xffff
        test   al, 0x20
        je     loc_1_CB87
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        test   eax, eax
        je     loc_1_CBAA
loc_1_CB87:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ax, word ptr [eax + 0x3e]
        and    eax, 0xffff
        test   ah, 1
        je     loc_1_CBE8
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        cmp    eax, 1
        jne    loc_1_CBE8
loc_1_CBAA:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xe]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xe]
        and    ecx, 0xffff
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xe], ax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        push   eax
        call   sub_1_CDF8
        mov    word ptr [ebp - 0xc], ax
loc_1_CBE8:
        mov    ax, word ptr [ebp - 0xc]
        jmp    loc_1_CBF1
loc_1_CBF1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_CBF8:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    bx, word ptr [ebp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x282], 0
        jne    loc_1_CC18
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_CC18:
        mov    edi, dword ptr [esi + 0x27a]
        test   edi, edi
        je     loc_1_CCD8
        cmp    byte ptr [esi + 0x304], 0
        jne    loc_1_CCD8
        cmp    byte ptr [esi + 0x305], 0
        jne    loc_1_CCD8
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x27a], eax
        test   eax, eax
        jne    loc_1_CC57
        mov    dword ptr [esi + 0x27e], 0
loc_1_CC57:
        push   esi
        push   edi
        call   sub_1_3760
loc_1_CC5E:
        cmp    ax, 6
        je     loc_1_CC88
        test   ax, ax
        je     loc_1_CCAB
        cmp    ax, 8
        jne    loc_1_CCD8
        mov    byte ptr [esi + 0x303], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    ax, 6
        jmp    loc_1_CC5E
loc_1_CC88:
        mov    eax, dword ptr [esi + 0x27a]
        test   eax, eax
        je     loc_1_CC9D
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [esi + 0x27a], edi
        jmp    loc_1_CCD8
loc_1_CC9D:
        mov    dword ptr [esi + 0x27a], edi
        mov    dword ptr [esi + 0x27e], edi
        jmp    loc_1_CCD8
loc_1_CCAB:
        cmp    dword ptr [esi + 0x282], 0
        jne    loc_1_CCC2
        mov    dword ptr [esi + 0x286], edi
        mov    dword ptr [esi + 0x282], edi
        jmp    loc_1_CCD1
loc_1_CCC2:
        mov    eax, dword ptr [esi + 0x286]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x286], edi
loc_1_CCD1:
        mov    dword ptr [edi + 0x24], 0
loc_1_CCD8:
        mov    ecx, dword ptr [esi + 0x282]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x282], eax
        test   eax, eax
        jne    loc_1_CCF5
        mov    dword ptr [esi + 0x286], 0
loc_1_CCF5:
        mov    edi, dword ptr [ecx + 0x28]
        mov    eax, ebx
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_CD1D
        cmp    eax, 0x10
        je     loc_1_CD21
        cmp    eax, 0x11
        je     loc_1_CD28
        mov    eax, 0xc0000001
        mov    dword ptr [ecx + 0x2e], eax
        inc    dword ptr [esi + 0x2ab]
        jmp    loc_1_CD36
loc_1_CD1D:
        xor    eax, eax
        jmp    loc_1_CD2D
loc_1_CD21:
        mov    eax, 0x10001
        jmp    loc_1_CD2D
loc_1_CD28:
        mov    eax, 0x10002
loc_1_CD2D:
        mov    dword ptr [ecx + 0x2e], eax
        inc    dword ptr [esi + 0x2a3]
loc_1_CD36:
        cmp    byte ptr [ecx + 0x2c], 0
        je     loc_1_CD73
        cmp    dword ptr [esi + 0x28a], 0
        jne    loc_1_CD53
        mov    dword ptr [esi + 0x28e], ecx
        mov    dword ptr [esi + 0x28a], ecx
        jmp    loc_1_CD62
loc_1_CD53:
        mov    eax, dword ptr [esi + 0x28e]
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [esi + 0x28e], ecx
loc_1_CD62:
        xor    ax, ax
        pop    edi
        mov    dword ptr [ecx + 0x24], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_CD73:
        push   eax
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_D72A
        mov    esi, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_CDDF
        mov    eax, dword ptr [esi + 0x26d]
        cmp    eax, edi
        jne    loc_1_CD9A
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esi + 0x26d], eax
        jmp    loc_1_CDB3
loc_1_CD9A:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_CDA7
loc_1_CD9F:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_CD9F
loc_1_CDA7:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [esi + 0x452], edi
loc_1_CDB3:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x317], eax
        jne    loc_1_CDC4
        push   esi
        call   sub_1_0BC0
loc_1_CDC4:
        cmp    dword ptr [esi + 0x26d], 0
        jne    loc_1_CDDF
        push   esi
        push   0                             ; -> loc_1_2310
        add    esi, 0x20c
        push   esi
        call   sub_1_D7DA
loc_1_CDDF:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_CDF8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 0x296], 1
        mov    dword ptr [edi + 0x292], 0
        mov    ebx, dword ptr [edi + 0x317]
        add    ebx, 0xe
        cmp    ebx, esi
        jb     loc_1_CE24
        mov    ebx, esi
loc_1_CE24:
        push   edi
        lea    eax, [edi + 0x32b]
        push   eax
        push   0
        push   ebx
        call   sub_1_3470
        test   ax, ax
        je     loc_1_CE43
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_CE43:
        test   byte ptr [edi + 0x333], 0x80
        je     loc_1_CE5A
        mov    al, byte ptr [edi + 0x339]
        and    eax, 0x1f
        add    eax, 0xe
        jmp    loc_1_CE5F
loc_1_CE5A:
        mov    eax, 0xe
loc_1_CE5F:
        mov    dword ptr [edi + 0x29f], eax
        inc    dword ptr [edi + 0x2a7]
        cmp    esi, 0xe
        jae    loc_1_CE9A
        cmp    esi, 6
        jb     loc_1_CEBF
        push   0
        lea    eax, [edi + 0x32b]
        push   0
        mov    ecx, dword ptr [edi + 0x25d]
        push   0
        push   esi
        push   eax
        push   edi
        push   ecx
        call   sub_1_D260
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_CE9A:
        sub    esi, eax
        sub    ebx, eax
        push   esi
        lea    ecx, [edi + eax]
        push   ebx
        add    ecx, 0x32b
        push   ecx
        mov    edx, dword ptr [edi + 0x25d]
        push   eax
        lea    eax, [edi + 0x32b]
        push   eax
        push   edi
        push   edx
        call   sub_1_D260
loc_1_CEBF:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_CED8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x303], 0
        mov    edi, dword ptr [esi + 0x26d]
        je     loc_1_CF14
        cmp    byte ptr [esi + 0x305], 1
        jne    loc_1_CF14
        add    esi, 0x45e
        push   0
        push   esi
        call   sub_1_D7C2
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_CF14:
        mov    cl, byte ptr [esi + 0x305]
        cmp    cl, 1
        je     loc_1_CFCC
        cmp    byte ptr [esi + 0x271], 0
        jne    loc_1_CFCC
        mov    ax, word ptr [esi + 0x40]
        cmp    ax, 2
        jne    loc_1_CF45
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_CF45:
        cmp    ax, 3
        jne    loc_1_CFC1
        test   edi, edi
        je     loc_1_CF73
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   0x40010007
        push   eax
        call   sub_1_D74E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_D754
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_CF73:
        cmp    byte ptr [esi + 0x451], 1
        jne    loc_1_CFC1
        lea    eax, [esi + 0x45e]
        lea    ecx, [ebp - 1]
        mov    byte ptr [esi + 0x451], 0
        push   ecx
        push   eax
        call   sub_1_D7C8
        push   0
        mov    ecx, dword ptr [esi + 0x452]
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_D724
        mov    ecx, dword ptr [esi + 0x452]
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_D754
        push   0
        mov    ecx, dword ptr [esi + 0x452]
        push   0x75
        push   ecx
        call   sub_1_D7EC
loc_1_CFC1:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_CFCC:
        mov    ax, word ptr [esi + 0x40]
        cmp    ax, 2
        jne    loc_1_D027
        push   esi
        call   sub_1_2F78
        test   ax, ax
        je     loc_1_D001
        add    esi, 0x45e
        push   0
        push   esi
        mov    byte ptr [esi - 0x15b], 1
        call   sub_1_D7C2
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D001:
        add    esi, 0x45e
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_D7C8
        push   0x7530
        push   esi
        call   sub_1_D7C2
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D027:
        cmp    ax, 1
        jne    loc_1_D0BE
        test   cl, cl
        je     loc_1_D08F
        cmp    byte ptr [esi + 0x306], 1
        jne    loc_1_D076
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x45e]
        mov    byte ptr [esi + 0x306], 0
        mov    byte ptr [esi + 0x305], 0
        push   eax
        push   ecx
        call   sub_1_D7C8
        cmp    byte ptr [esi + 0x271], 1
        jne    loc_1_D0BE
        push   esi
        call   sub_1_0B30
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D076:
        push   0
        add    esi, 0x45e
        push   esi
        call   sub_1_D7C2
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D08F:
        cmp    ax, 1
        jne    loc_1_D0BE
        cmp    byte ptr [esi + 0x271], 0
        je     loc_1_D0BE
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x45e]
        mov    dword ptr [esi + 0x307], 0
        push   eax
        push   ecx
        call   sub_1_D7C8
        push   esi
        call   sub_1_0B30
loc_1_D0BE:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_D0D8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        test   byte ptr [eax + 0x8e], 0x80
        push   esi
        mov    esi, dword ptr [eax + 0x26d]
        mov    dword ptr [ebp - 4], 0
        jne    loc_1_D103
        xor    ax, ax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D103:
        mov    cx, word ptr [eax + 0x8f]
        test   cl, 0x20
        je     loc_1_D116
        mov    dword ptr [ebp - 4], 0x40
loc_1_D116:
        test   cl, 0x40
        je     loc_1_D122
        or     dword ptr [ebp - 4], 0x80
loc_1_D122:
        test   cl, 0x80
        je     loc_1_D12E
        or     dword ptr [ebp - 4], 0x100
loc_1_D12E:
        test   ch, 1
        je     loc_1_D13A
        or     dword ptr [ebp - 4], 0x200
loc_1_D13A:
        test   ch, 4
        je     loc_1_D146
        or     dword ptr [ebp - 4], 0x400
loc_1_D146:
        test   ch, 8
        je     loc_1_D152
        or     dword ptr [ebp - 4], 0x800
loc_1_D152:
        test   ch, 0x10
        je     loc_1_D15E
        or     dword ptr [ebp - 4], 0x1000
loc_1_D15E:
        test   ch, 0x20
        je     loc_1_D16A
        or     dword ptr [ebp - 4], 0x2000
loc_1_D16A:
        test   ch, 0x40
        je     loc_1_D176
        or     dword ptr [ebp - 4], 0x4000
loc_1_D176:
        test   ch, 0x80
        je     loc_1_D182
        or     dword ptr [ebp - 4], 0x8000
loc_1_D182:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_D1A9
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [esi + 0x1c]
        push   0x40010006
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_D74E
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_D754
        dec    dword ptr [esi + 0x1c]
loc_1_D1A9:
        xor    ax, ax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_D1B8:
        mov    al, byte ptr [0xd978]         ; -> obj1:0xD978
        cmp    al, 0
        jne    loc_1_D1DD
        inc    byte ptr [0xd978]             ; -> obj1:0xD978
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_D1DF
        push   0
        push   0
        call   0
        cmp    eax, 0
        jne    loc_1_D1DF
loc_1_D1DD:
        clc    
        ret    
loc_1_D1DF:
        stc    
        ret    
SMC8100W_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_D1F0
        jmp    dword ptr [eax*4 + 0xd1f2]    ; -> obj1:0xD1F2
loc_1_D1F0:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_D1B8
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1F0
        dd  loc_1_D1B8
        db  000h, 000h
sub_1_D260:
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
        call   sub_1_D290
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_D290:
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
        jb     loc_1_D3B0
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     loc_1_D354
        mov    al, byte ptr [ecx]
        cmp    al, 0xff
        jne    loc_1_D2D2
        cmp    byte ptr [ecx + 1], 0xff
        je     loc_1_D2DC
loc_1_D2D2:
        cmp    al, 0xc0
        jne    loc_1_D2F8
        cmp    byte ptr [ecx + 1], 0
        jne    loc_1_D2F8
loc_1_D2DC:
        cmp    byte ptr [ecx + 2], 0xff
        jne    loc_1_D2F8
        cmp    byte ptr [ecx + 3], 0xff
        jne    loc_1_D2F8
        cmp    byte ptr [ecx + 4], 0xff
        jne    loc_1_D2F8
        cmp    byte ptr [ecx + 5], 0xff
        mov    byte ptr [ebp - 2], 1
        je     loc_1_D2FC
loc_1_D2F8:
        mov    byte ptr [ebp - 2], 0
loc_1_D2FC:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_D310
        mov    dword ptr [ebp - 8], 8
        jmp    loc_1_D3BB
        db  08Bh, 0FFh
loc_1_D310:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 0x1000
        jne    loc_1_D32C
        mov    dword ptr [ebp - 8], 0x4000
loc_1_D32C:
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
        jmp    loc_1_D3BB
loc_1_D354:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x10], 0x30
        je     loc_1_D3A0
        mov    edx, dword ptr [esi + 0x1a]
        lea    eax, [esi + 0x18]
        cmp    dword ptr [ecx + 2], edx
        jne    loc_1_D377
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_D37E
loc_1_D377:
        mov    dword ptr [ebp - 0x10], 1
loc_1_D37E:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_D390
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_D3BE
        db  08Dh, 049h, 000h
loc_1_D390:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_D3BE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_D3A0:
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_D3BE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_D3B0:
        mov    dword ptr [ebp - 8], 0x20
        mov    byte ptr [ebp - 1], 0
loc_1_D3BB:
        mov    esi, dword ptr [ebp + 8]
loc_1_D3BE:
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_D4A1
loc_1_D3C9:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    loc_1_D40D
        test   ah, 0x40
        je     loc_1_D3E2
        mov    eax, dword ptr [edi + 0xc]
        test   dword ptr [ebp - 0xc], eax
        jne    loc_1_D40D
loc_1_D3E2:
        test   byte ptr [ebp - 7], 0x40
        je     loc_1_D3ED
        test   ch, 0x20
        jne    loc_1_D40D
loc_1_D3ED:
        test   byte ptr [ebp - 7], 0x10
        je     loc_1_D3F9
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_1_D40D
loc_1_D3F9:
        test   cl, 0x10
        je     loc_1_D404
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_D40D
loc_1_D404:
        test   cl, 0x20
        je     loc_1_D496
loc_1_D40D:
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
        call   sub_1_D742
        mov    byte ptr [edi + 0x1e], 1
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_D496
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_D44F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_D44F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_D460
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_D466
        db  08Bh, 0FFh
loc_1_D460:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_D466:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_D475
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_D748
loc_1_D475:
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
        call   sub_1_D7EC
        mov    edi, ebx
        jmp    loc_1_D499
loc_1_D496:
        mov    edi, dword ptr [edi + 0x10]
loc_1_D499:
        or     edi, edi
        jne    loc_1_D3C9
loc_1_D4A1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_D4B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D4C0
        pop    ebp
        ret    4
sub_1_D4C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_D53E
loc_1_D4D0:
        inc    dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_D4E6
        mov    byte ptr [edi + 0x1e], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_D748
loc_1_D4E6:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_D537
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_D4FC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_D4FC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_D510
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_D516
        db  005h, 000h, 000h, 000h, 000h
loc_1_D510:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_D516:
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
        call   sub_1_D7EC
        mov    edi, ebx
        jmp    loc_1_D53A
loc_1_D537:
        mov    edi, dword ptr [edi + 0x10]
loc_1_D53A:
        or     edi, edi
        jne    loc_1_D4D0
loc_1_D53E:
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
        je     loc_1_D5A2
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_D5A2
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_D5A2
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_D5A2:
        mov    esi, dword ptr [eax + 0x10]
        or     esi, esi
        jne    loc_1_D5B2
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_D5B2:
        test   esi, 0x20
        je     loc_1_D5C3
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_D5C3:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     loc_1_D5E0
        test   esi, 0x10
        je     loc_1_D5E0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_D5E0:
        mov    eax, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [eax]
        and    cl, 0x80
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     loc_1_D66A
        mov    cl, byte ptr [eax]
        cmp    cl, 0xff
        jne    loc_1_D5FC
        cmp    byte ptr [eax + 1], 0xff
        je     loc_1_D607
loc_1_D5FC:
        cmp    cl, 0xc0
        jne    loc_1_D623
        cmp    byte ptr [eax + 1], 0
        jne    loc_1_D623
loc_1_D607:
        cmp    byte ptr [eax + 2], 0xff
        jne    loc_1_D623
        cmp    byte ptr [eax + 3], 0xff
        jne    loc_1_D623
        cmp    byte ptr [eax + 4], 0xff
        jne    loc_1_D623
        cmp    byte ptr [eax + 5], 0xff
        mov    byte ptr [ebp - 1], 1
        je     loc_1_D627
loc_1_D623:
        mov    byte ptr [ebp - 1], 0
loc_1_D627:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_D650
        test   esi, 8
        je     loc_1_D640
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_D640:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_D650:
        test   esi, 0x6000
        je     loc_1_D661
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_D661:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_D66A:
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
        db  0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_D700:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_D70C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_D712:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_D718:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_D71E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_D724:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_D72A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_D736:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_D742:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_D748:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_D74E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_D754:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_D760:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_D76C:
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
sub_1_D7C2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_D7C8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_D7CE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_D7D4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_D7DA:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_D7E0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_D7E6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_D7EC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_D810:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_D834:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_D858:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_D864:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_D86A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_D870:
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
sub_1_D8B2:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_D8B8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_D8CC:
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
        db  'SMC8100W'                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  SMC8100W_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_D980:
        inc    eax
        sub    byte ptr [ebx], ah
        sub    dword ptr [eax], esp
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [eax], bh
        xor    esp, dword ptr [ebx + 0x38]
        xor    dh, byte ptr [0x6f542044]
loc_1_D993:
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x6e6f4320
        je     0xda13
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        jb     loc_1_D9C7
        dec    ebp
        imul   esp, dword ptr [ebx + 0x72], 0x646f636f
        and    byte ptr gs:[esi + 0x35], dh
        cmp    byte ptr cs:[eax], dh
        and    byte ptr [eax], ch
        cmp    dword ptr [ebx], esi
        xor    byte ptr [ecx], bh
        xor    byte ptr [ecx], bh
        sub    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_D9C4:
        and    cl, ch
        db  08Eh
loc_1_D9C7:
        jo     loc_1_D9F5
        adc    byte ptr [0x2f162e17], ch
        adc    eax, 0x69c000db
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    eax
        jns    loc_1_DA3F
        je     loc_1_D993
        jb     0xda2f
        ja     0xda31
        ja     0xd9ab
        ja     loc_1_DA49
        jns    loc_1_DA12
        js     loc_1_DA4D
loc_1_D9F5:
        jns    0xda02
        js     loc_1_DA46
loc_1_D9F9:
        ja     0xd9a0
        ja     0xda55
        jns    loc_1_D9FF
loc_1_D9FF:
        jns    0xda59
        jns    loc_1_DA4B
        ja     loc_1_D9F9
        pop    ds
        shr    byte ptr [ebp + 0x25], 0
        call   0x3a71681e
        pop    esi
        or     eax, dword ptr [esi]
loc_1_DA12:
        cmp    eax, 0xa208025e
        pop    esi
        add    dword ptr [eax], eax
        pop    ds
        je     loc_1_DA3D
        mov    al, byte ptr [0xf21fa11f]
        pop    ds
        mov    esi, 0x1b00005e
        pop    ds
        pop    edx
        pop    ds
        sub    dword ptr [edi], ebx
        cmp    ebx, dword ptr [edi]
        cmp    al, 0x1f
        dec    ebx
        pop    ds
        dec    esp
        pop    ds
        dec    ebp
        pop    ds
        dec    esi
        pop    ds
        dec    edi
        pop    ds
        push   eax
loc_1_DA3D:
        pop    ds
        push   ecx
loc_1_DA3F:
        pop    ds
        push   edx
        pop    ds
        push   ebx
        pop    ds
        push   esp
        pop    ds
loc_1_DA46:
        push   ebp
        pop    ds
        push   esi
loc_1_DA49:
        pop    ds
        push   edi
loc_1_DA4B:
        pop    ds
        pop    eax
loc_1_DA4D:
        pop    ds
        pop    ecx
        pop    ds
        sbb    byte ptr [edi], 0x81
        pop    ds
        sbb    byte ptr [edi], 0x83
        pop    ds
        test   byte ptr [edi], bl
        test   dword ptr [edi], ebx
        mov    edi, 0x77004c6a
        pop    ds
        fcomp  qword ptr [esi - 1]
        push   dword ptr [ebp + 0xf]
        adc    dword ptr [esi], ebx
        add    byte ptr [edi + ebx + 0x3e], dh
        pop    ds
        aas    
        pop    ds
        inc    eax
        pop    ds
        pop    ebp
        pop    ds
        pop    esi
        pop    ds
        pop    edi
        pop    ds
        lcall  0xff5e, 0x65ffff5e
        inc    edi
        pop    esi
        cmc    
        call   ptr [ebx]
        push   esp
        invd   
        sbb    edx, dword ptr [eax]
        sub    dword ptr [edi + esi*8 - 1], edx
        sub    dword ptr [eax], edx
        aad    0x1f
        sbb    al, 0x1f
        xchg   dword ptr [edi], ebx
        in     eax, 0x6a
        jbe    loc_1_DA9C
loc_1_DA9C:
        fild   dword ptr [eax]
        rcl    dword ptr [eax + eax + 0x20]
        push   -0x3c
        ficomp word ptr [esi - 1]
        add    ch, ah
        popal  
        retf   0xca00
        db  000h, 06Ah, 070h, 09Bh, 05Eh, 0FFh, 0FFh, 0C0h, 067h, 0E8h, 01Fh, 0F2h, 01Fh, 0BEh, 05Eh, 002h
        db  000h, 0BFh, 06Ah, 07Dh, 000h, 077h, 01Fh, 075h, 00Fh, 074h, 057h, 01Dh, 000h, 074h, 010h, 074h
        db  054h, 01Fh, 000h, 074h, 010h, 0A3h, 05Eh, 0A0h, 00Fh, 061h, 05Eh, 000h, 001h, 0A3h, 06Ah, 08Bh
        db  000h, 04Fh, 070h, 0E8h, 00Fh, 0CAh, 06Bh, 0AAh, 002h, 0E6h, 06Bh, 058h, 004h, 0E8h, 01Fh, 01Bh
        db  054h, 000h, 080h, 0E2h, 06Dh, 079h, 000h, 04Dh, 054h, 008h, 000h, 0E2h, 06Bh, 0E4h, 00Bh, 0E8h
        db  00Fh, 0E8h, 01Fh, 0D5h, 000h, 0C0h, 0C4h, 0C0h, 069h, 0B0h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 06Ah, 072h, 091h, 072h, 0C0h, 070h, 028h, 072h, 009h
        db  072h, 09Dh, 071h, 084h, 071h, 069h, 071h, 046h, 072h, 0C0h, 070h, 0C0h, 070h, 0C0h, 070h, 0C0h
        db  070h, 04Bh, 071h, 023h, 071h, 005h, 071h, 0E8h, 00Fh, 08Bh, 000h, 08Eh, 097h, 01Bh, 054h, 000h
        db  008h, 08Eh, 0C5h, 05Ah, 054h, 040h, 000h, 0DDh, 0C5h, 011h, 053h, 006h, 000h, 0E7h, 0C5h, 0E8h
        db  01Fh, 0B0h, 000h, 03Eh, 04Dh, 0E7h, 0C5h, 0B1h, 000h, 03Fh, 04Dh, 0E7h, 0C5h, 0B2h, 000h, 040h
        db  04Dh, 0E7h, 0C5h, 05Ah, 057h, 040h, 000h, 05Ah, 010h, 029h, 057h, 002h, 000h, 029h, 010h, 0E7h
        db  070h, 011h, 053h, 004h, 000h, 0E7h, 0C5h, 0E8h, 01Fh, 05Ah, 054h, 0BFh, 0FFh, 05Ah, 010h, 029h
        db  057h, 002h, 000h, 029h, 010h, 0E8h, 00Fh, 01Bh, 054h, 000h, 020h, 0FBh, 0C4h, 04Dh, 054h, 004h
        db  000h, 0FBh, 0C4h, 0E8h, 01Fh, 04Dh, 054h, 0FBh, 0FFh, 04Dh, 010h, 05Ah, 057h, 000h, 080h, 05Ah
        db  010h, 029h, 057h, 002h, 000h, 029h, 010h, 0E8h, 00Fh, 0E8h, 01Fh, 0FDh, 070h, 029h, 000h, 08Eh
        db  0C4h, 0E8h, 00Fh, 08Bh, 05Eh, 000h, 080h, 0E8h, 01Fh, 01Bh, 057h, 000h, 008h, 01Bh, 010h, 08Eh
        db  070h, 0E8h, 00Fh, 030h, 05Eh, 020h, 000h, 031h, 05Eh, 004h, 010h, 032h, 05Eh, 000h, 0C0h, 033h
        db  05Eh, 0FFh, 0FFh, 034h, 05Eh, 0FFh, 0FFh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 004h, 000h, 01Ch
        db  05Eh, 000h, 080h, 0E6h, 05Eh, 000h, 080h, 0C4h, 06Bh, 018h, 001h, 087h, 05Eh, 000h, 080h, 069h
        db  05Eh, 00Ch, 000h, 0C4h, 05Eh, 022h, 030h, 0E5h, 05Eh, 008h, 000h, 08Eh, 070h, 05Ah, 057h, 000h
        db  050h, 05Ah, 010h, 029h, 057h, 002h, 000h, 029h, 010h, 0E8h, 00Fh, 030h, 05Eh, 024h, 000h, 031h
        db  05Eh, 002h, 010h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 0FFh, 0FFh, 034h, 05Eh, 0FFh, 0FFh, 038h
        db  05Eh, 016h, 000h, 039h, 05Eh, 002h, 000h, 041h, 05Eh, 001h, 004h, 05Bh, 000h, 042h, 010h, 0C4h
        db  06Bh, 03Ch, 001h, 087h, 05Eh, 000h, 080h, 069h, 05Eh, 00Ch, 000h, 0C4h, 05Eh, 026h, 030h, 01Ch
        db  05Eh, 000h, 040h, 0E6h, 05Eh, 000h, 040h, 0E5h, 05Eh, 008h, 000h, 08Eh, 070h, 0E8h, 00Fh, 030h
        db  05Eh, 020h, 000h, 031h, 05Eh, 003h, 010h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 0FFh, 0FFh, 034h
        db  05Eh, 0FFh, 0FFh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 003h, 000h, 0C4h, 06Bh, 05Ah, 001h, 087h
        db  05Eh, 000h, 080h, 01Ch, 05Eh, 000h, 020h, 0E6h, 05Eh, 000h, 020h, 0C4h, 05Eh, 022h, 030h, 069h
        db  05Eh, 00Ch, 000h, 0E5h, 05Eh, 008h, 000h, 08Eh, 070h, 087h, 055h, 000h, 080h, 0EAh, 06Bh, 0C0h
        db  000h, 0C4h, 06Bh, 082h, 001h, 01Ch, 05Eh, 080h, 000h, 087h, 05Eh, 000h, 080h, 030h, 05Eh, 020h
        db  000h, 031h, 05Eh, 005h, 0F0h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 0FFh, 0FFh, 034h, 05Eh, 0FFh
        db  0FFh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 005h, 000h, 0A7h, 072h, 0F2h, 00Fh, 0C0h, 070h, 087h
        db  055h, 000h, 008h, 0EAh, 06Bh, 0C0h, 000h, 0C4h, 06Bh, 082h, 001h, 01Ch, 05Eh, 040h, 000h, 087h
        db  05Eh, 000h, 008h, 030h, 05Eh, 020h, 000h, 031h, 05Eh, 006h, 070h, 032h, 05Eh, 000h, 0C0h, 033h
        db  05Eh, 0FFh, 0FFh, 034h, 05Eh, 0FFh, 0FFh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 006h, 000h, 0A7h
        db  072h, 01Bh, 054h, 000h, 020h, 0AAh, 0C5h, 0E8h, 00Fh, 0E6h, 05Eh, 020h, 000h, 07Ah, 000h, 07Bh
        db  000h, 07Ch, 000h, 07Dh, 000h, 07Eh, 000h, 07Fh, 000h, 0C0h, 070h, 087h, 055h, 000h, 008h, 0EAh
        db  06Bh, 0C0h, 000h, 0C4h, 06Bh, 082h, 001h, 05Ah, 057h, 000h, 020h, 05Ah, 010h, 029h, 057h, 003h
        db  000h, 029h, 010h, 0E8h, 00Fh, 02Ah, 05Eh, 054h, 000h, 02Bh, 05Eh, 080h, 000h, 0C0h, 06Dh, 0E5h
        db  001h, 0C0h, 06Dh, 0E5h, 001h, 0C0h, 06Dh, 0E5h, 001h, 0C0h, 06Dh, 0E5h, 001h, 0C0h, 06Dh, 0E5h
        db  001h, 0C0h, 06Dh, 0E5h, 001h, 01Ch, 05Eh, 020h, 000h, 087h, 05Eh, 000h, 008h, 030h, 05Eh, 030h
        db  000h, 031h, 05Eh, 000h, 070h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h, 05Eh, 008h
        db  000h, 038h, 05Eh, 022h, 000h, 039h, 05Eh, 029h, 060h, 041h, 05Eh, 02Dh, 008h, 0D3h, 05Eh, 07Ah
        db  042h, 003h, 05Bh, 0D1h, 05Eh, 02Eh, 008h, 000h, 063h, 0DEh, 05Eh, 07Dh, 000h, 003h, 05Bh, 0A7h
        db  072h, 02Ah, 05Ch, 0D1h, 005h, 02Bh, 05Ch, 0FFh, 044h, 0CFh, 04Ah, 0CFh, 010h, 0D1h, 054h, 000h
        db  0FFh, 0E2h, 06Ah, 0F6h, 001h, 0CFh, 05Eh, 0FFh, 000h, 0E8h, 01Fh, 05Ah, 057h, 080h, 000h, 05Ah
        db  010h, 0E8h, 00Fh, 02Bh, 05Ch, 02Bh, 016h, 0D1h, 054h, 000h, 0FFh, 0CEh, 04Ah, 0CEh, 010h, 0EAh
        db  06Ah, 005h, 002h, 0CEh, 05Eh, 000h, 0FFh, 0E8h, 01Fh, 05Ah, 057h, 080h, 000h, 05Ah, 010h, 0E8h
        db  00Fh, 0D2h, 000h, 02Ah, 05Dh, 02Ah, 016h, 0C0h, 067h, 087h, 055h, 000h, 008h, 0EAh, 06Bh, 0C0h
        db  000h, 0C4h, 06Bh, 082h, 001h, 01Ch, 05Eh, 010h, 000h, 087h, 05Eh, 000h, 008h, 030h, 05Eh, 024h
        db  000h, 031h, 05Eh, 000h, 070h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h, 05Eh, 008h
        db  000h, 038h, 05Eh, 016h, 000h, 039h, 05Eh, 028h, 060h, 041h, 05Eh, 030h, 004h, 05Ch, 000h, 042h
        db  010h, 0A7h, 072h, 0F2h, 00Fh, 0C0h, 070h, 087h, 055h, 000h, 008h, 0EAh, 06Bh, 0C0h, 000h, 0C4h
        db  06Bh, 026h, 002h, 01Ch, 05Eh, 008h, 000h, 087h, 05Eh, 000h, 008h, 030h, 05Eh, 028h, 000h, 031h
        db  05Eh, 000h, 070h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h, 05Eh, 008h, 000h, 038h
        db  05Eh, 01Ah, 000h, 039h, 05Eh, 027h, 060h, 041h, 05Eh, 00Ah, 008h, 0D3h, 05Eh, 05Dh, 042h, 003h
        db  05Bh, 0A7h, 072h, 087h, 055h, 000h, 080h, 0EAh, 06Bh, 0C0h, 000h, 0C4h, 06Bh, 026h, 002h, 01Ch
        db  05Eh, 000h, 001h, 087h, 05Eh, 000h, 080h, 030h, 05Eh, 034h, 000h, 031h, 05Eh, 000h, 070h, 032h
        db  05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h, 05Eh, 010h, 000h, 038h, 05Eh, 026h, 000h, 039h
        db  05Eh, 025h, 040h, 041h, 05Eh, 022h, 014h, 042h, 01Fh, 043h, 01Fh, 044h, 01Fh, 045h, 01Fh, 046h
        db  01Fh, 047h, 01Fh, 048h, 01Fh, 049h, 01Fh, 04Ah, 01Fh, 0A7h, 072h, 087h, 055h, 000h, 008h, 0EAh
        db  06Bh, 0C0h, 000h, 0C4h, 06Bh, 026h, 002h, 01Ch, 05Eh, 001h, 000h, 087h, 05Eh, 000h, 008h, 030h
        db  05Eh, 020h, 000h, 031h, 05Eh, 000h, 070h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h
        db  05Eh, 010h, 000h, 0E8h, 00Fh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 026h, 040h, 0E8h, 01Fh, 01Bh
        db  054h, 000h, 020h, 08Dh, 0C5h, 01Bh, 057h, 000h, 020h, 01Bh, 010h, 029h, 057h, 008h, 000h, 029h
        db  010h, 029h, 057h, 004h, 000h, 029h, 010h, 0A7h, 072h, 01Ch, 055h, 002h, 000h, 0E2h, 06Ah, 0C0h
        db  000h, 01Ch, 05Eh, 002h, 000h, 087h, 05Eh, 000h, 080h, 030h, 05Eh, 012h, 000h, 031h, 05Eh, 001h
        db  070h, 035h, 000h, 032h, 010h, 036h, 000h, 033h, 010h, 037h, 000h, 034h, 010h, 038h, 05Eh, 004h
        db  000h, 039h, 05Eh, 007h, 000h, 0E9h, 05Eh, 020h, 000h, 08Eh, 070h, 0F0h, 00Fh, 00Ch, 054h, 00Fh
        db  000h, 0D1h, 052h, 0B0h, 002h, 0C0h, 06Fh, 0C3h, 072h, 063h, 074h, 0CBh, 072h, 057h, 073h, 069h
        db  073h, 00Bh, 074h, 064h, 073h, 0C6h, 073h, 0C3h, 072h, 0C3h, 072h, 0C3h, 072h, 0C3h, 072h, 0C3h
        db  072h, 0C3h, 072h, 0C3h, 072h, 0C9h, 072h, 00Bh, 05Eh, 001h, 080h, 0C5h, 072h, 00Bh, 05Eh, 000h
        db  080h, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 08Eh, 070h, 0F5h, 00Fh, 0CAh, 072h, 011h, 000h, 0E2h
        db  06Dh, 079h, 000h, 0E8h, 01Fh, 0F2h, 01Fh, 0BEh, 05Eh, 001h, 000h, 074h, 057h, 001h, 000h, 074h
        db  010h, 09Bh, 05Eh, 0FFh, 0FFh, 0A3h, 05Eh, 060h, 0EAh, 061h, 05Eh, 000h, 001h, 0BFh, 06Ah, 0DBh
        db  002h, 0A3h, 06Ah, 0DDh, 002h, 09Bh, 05Eh, 0FFh, 0FFh, 0A3h, 05Eh, 0E8h, 003h, 061h, 05Eh, 000h
        db  001h, 0A3h, 06Ah, 0E5h, 002h, 09Bh, 054h, 018h, 000h, 0E2h, 06Bh, 0C0h, 002h, 0BEh, 05Eh, 002h
        db  000h, 0BFh, 06Ah, 0EDh, 002h, 077h, 01Fh, 074h, 05Eh, 05Dh, 002h, 075h, 01Fh, 011h, 05Eh, 001h
        db  000h, 089h, 05Eh, 000h, 001h, 061h, 05Eh, 001h, 000h, 065h, 05Eh, 07Fh, 0FFh, 09Bh, 05Eh, 0BFh
        db  0FFh, 06Eh, 05Eh, 010h, 010h, 0DCh, 05Eh, 0FFh, 0D1h, 00Bh, 05Eh, 000h, 080h, 08Eh, 05Eh, 010h
        db  000h, 0F0h, 01Fh, 01Bh, 01Fh, 0B0h, 000h, 035h, 010h, 0B1h, 000h, 036h, 010h, 0B2h, 000h, 037h
        db  010h, 0BFh, 06Ah, 00Dh, 003h, 084h, 01Fh, 09Bh, 05Eh, 010h, 000h, 077h, 01Fh, 08Eh, 070h, 011h
        db  000h, 0E2h, 06Dh, 079h, 000h, 0E8h, 01Fh, 074h, 057h, 001h, 000h, 074h, 010h, 0A3h, 05Eh, 040h
        db  09Ch, 061h, 05Eh, 000h, 001h, 0FFh, 060h, 0A3h, 06Ah, 020h, 003h, 09Bh, 05Eh, 0FFh, 0FFh, 0A3h
        db  05Eh, 0E8h, 003h, 061h, 05Eh, 000h, 001h, 0A3h, 06Ah, 028h, 003h, 09Bh, 054h, 018h, 000h, 0E2h
        db  06Bh, 0C0h, 002h, 074h, 05Eh, 03Bh, 002h, 0A3h, 05Eh, 0E8h, 003h, 061h, 05Eh, 000h, 001h, 0A3h
        db  06Ah, 034h, 003h, 074h, 057h, 0C0h, 000h, 074h, 010h, 09Bh, 05Eh, 0FFh, 0FFh, 043h, 073h, 0E8h
        db  01Fh, 074h, 05Eh, 0DBh, 002h, 043h, 073h, 0E8h, 01Fh, 074h, 05Eh, 0CBh, 002h, 0F2h, 01Fh, 0BEh
        db  05Eh, 001h, 000h, 011h, 05Eh, 008h, 000h, 089h, 05Eh, 000h, 007h, 0A5h, 05Eh, 0E8h, 003h, 061h
        db  05Eh, 018h, 000h, 065h, 05Eh, 0CFh, 0FFh, 0D5h, 05Eh, 000h, 081h, 01Ch, 01Fh, 087h, 01Fh, 0BFh
        db  06Ah, 054h, 003h, 0FBh, 072h, 0E8h, 01Fh, 011h, 000h, 05Ch, 0C4h, 089h, 05Eh, 000h, 000h, 00Bh
        db  05Eh, 000h, 080h, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 0C0h, 06Dh, 079h, 000h, 08Eh, 070h, 012h
        db  05Eh, 000h, 0A0h, 0D1h, 05Eh, 02Eh, 000h, 076h, 073h, 00Dh, 000h, 012h, 010h, 0E8h, 01Fh, 00Dh
        db  054h, 0FFh, 000h, 0CBh, 000h, 013h, 010h, 00Ch, 045h, 0E8h, 00Fh, 0E2h, 06Ah, 083h, 003h, 0EAh
        db  06Ah, 083h, 003h, 013h, 010h, 00Dh, 05Eh, 00Ch, 015h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 001h
        db  047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh, 05Eh, 001h, 000h, 0F0h, 01Fh, 08Eh, 070h, 013h, 057h, 000h
        db  015h, 00Dh, 010h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 003h, 047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh
        db  05Eh, 003h, 000h, 0F0h, 01Fh, 08Eh, 070h, 0E8h, 01Fh, 013h, 055h, 00Ch, 000h, 0E8h, 00Fh, 0E2h
        db  06Ah, 0A8h, 003h, 0EAh, 06Ah, 0A8h, 003h, 013h, 010h, 012h, 006h, 0DEh, 05Eh, 015h, 000h, 006h
        db  05Bh, 012h, 016h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 001h, 047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh
        db  05Eh, 001h, 000h, 0F0h, 01Fh, 08Eh, 070h, 013h, 057h, 000h, 015h, 00Dh, 010h, 012h, 006h, 0DEh
        db  05Eh, 015h, 000h, 006h, 05Bh, 012h, 016h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 003h, 047h, 01Bh
        db  010h, 0E8h, 00Fh, 08Eh, 05Eh, 003h, 000h, 0F0h, 01Fh, 08Eh, 070h, 012h, 006h, 0DEh, 05Eh, 015h
        db  000h, 013h, 000h, 0CFh, 019h, 000h, 05Bh, 00Bh, 05Eh, 000h, 080h, 08Eh, 05Eh, 010h, 000h, 0F0h
        db  01Fh, 08Eh, 070h, 0D3h, 05Eh, 054h, 015h, 006h, 05Bh, 0E8h, 01Fh, 029h, 054h, 0FEh, 0FFh, 029h
        db  010h, 00Dh, 05Eh, 00Ch, 015h, 01Bh, 054h, 000h, 0FFh, 004h, 047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh
        db  05Eh, 002h, 000h, 0F0h, 01Fh, 054h, 01Fh, 055h, 01Fh, 056h, 01Fh, 057h, 01Fh, 058h, 01Fh, 059h
        db  01Fh, 08Eh, 070h, 0D3h, 05Eh, 05Ah, 015h, 006h, 05Bh, 0E8h, 01Fh, 029h, 054h, 0FDh, 0FFh, 029h
        db  010h, 00Dh, 05Eh, 00Ch, 015h, 05Ah, 054h, 040h, 000h, 05Ah, 010h, 01Bh, 054h, 000h, 0FFh, 005h
        db  047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh, 05Eh, 002h, 000h, 0F0h, 01Fh, 08Eh, 070h, 0D3h, 05Eh, 03Eh
        db  015h, 003h, 05Bh, 018h, 05Eh, 008h, 005h, 029h, 000h, 019h, 010h, 01Ah, 01Fh, 0E8h, 01Fh, 029h
        db  054h, 0F3h, 0FFh, 029h, 010h, 00Dh, 05Eh, 00Ch, 015h, 01Bh, 054h, 0F0h, 0F7h, 01Bh, 010h, 0E8h
        db  00Fh, 00Bh, 05Eh, 000h, 080h, 08Eh, 05Eh, 012h, 000h, 0F0h, 01Fh, 08Eh, 070h, 00Dh, 000h, 0EAh
        db  054h, 0FFh, 000h, 012h, 010h, 00Dh, 054h, 0FFh, 000h, 0CBh, 000h, 013h, 010h, 00Ch, 045h, 0E2h
        db  06Ah, 042h, 004h, 0EAh, 06Ah, 042h, 004h, 013h, 010h, 0DDh, 05Eh, 000h, 015h, 012h, 000h, 0DEh
        db  010h, 006h, 05Bh, 012h, 016h, 00Dh, 05Eh, 00Ch, 015h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 002h
        db  047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh, 05Eh, 002h, 000h, 0F0h, 01Fh, 08Eh, 070h, 013h, 055h, 00Ch
        db  000h, 0E2h, 06Ah, 042h, 004h, 0EAh, 06Ah, 042h, 004h, 013h, 010h, 0DDh, 05Eh, 000h, 015h, 012h
        db  000h, 0DEh, 010h, 006h, 05Bh, 012h, 016h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 002h, 047h, 01Bh
        db  010h, 0E8h, 00Fh, 08Eh, 05Eh, 002h, 000h, 0F0h, 01Fh, 08Eh, 070h, 0DDh, 05Eh, 000h, 015h, 012h
        db  000h, 0DEh, 010h, 013h, 000h, 0CFh, 019h, 000h, 05Bh, 013h, 057h, 000h, 015h, 00Dh, 010h, 0E8h
        db  01Fh, 01Bh, 054h, 000h, 0FFh, 000h, 047h, 01Bh, 010h, 0E8h, 00Fh, 00Bh, 05Eh, 000h, 080h, 08Eh
        db  05Eh, 012h, 000h, 0F0h, 01Fh, 08Eh, 070h, 01Bh, 000h, 00Fh, 044h, 0D1h, 052h, 05Dh, 004h, 0C0h
        db  06Fh, 09Ch, 070h, 090h, 073h, 02Bh, 074h, 0BAh, 073h, 0DEh, 073h, 0F3h, 073h, 011h, 000h, 0E2h
        db  06Dh, 079h, 000h, 00Dh, 054h, 00Fh, 000h, 0D1h, 052h, 06Bh, 004h, 0C0h, 06Fh, 072h, 074h, 040h
        db  073h, 014h, 073h, 061h, 076h, 03Ch, 073h, 090h, 076h, 0E7h, 076h, 0E8h, 01Fh, 0C0h, 06Dh, 07Ch
        db  004h, 0C0h, 06Dh, 025h, 000h, 0E8h, 00Fh, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 08Eh, 070h, 00Bh
        db  05Eh, 002h, 080h, 0D1h, 05Eh, 0FFh, 0FFh, 001h, 042h, 0E2h, 067h, 0D1h, 05Eh, 000h, 080h, 0D1h
        db  052h, 0FFh, 0FFh, 0E2h, 066h, 0D1h, 053h, 0FFh, 07Fh, 0E2h, 067h, 0D1h, 05Eh, 0A5h, 05Ah, 0EAh
        db  053h, 05Ah, 0A5h, 0E2h, 067h, 0D1h, 05Eh, 05Ah, 0A5h, 0CDh, 053h, 0ADh, 052h, 0E2h, 067h, 0D1h
        db  05Eh, 0ADh, 052h, 0CBh, 053h, 05Ah, 0A5h, 0E2h, 067h, 0C7h, 05Eh, 0A5h, 000h, 065h, 05Eh, 03Ah
        db  009h, 088h, 05Eh, 02Eh, 001h, 0DCh, 05Eh, 0CCh, 033h, 0D5h, 05Eh, 099h, 066h, 0D4h, 05Eh, 034h
        db  012h, 0A0h, 05Eh, 022h, 05Ah, 0A1h, 05Eh, 039h, 000h, 0B0h, 05Eh, 05Ah, 05Ah, 0B1h, 05Eh, 099h
        db  033h, 0B2h, 05Eh, 08Fh, 047h, 0B3h, 05Eh, 034h, 012h, 0B4h, 05Eh, 0A3h, 09Fh, 0B5h, 05Eh, 0A3h
        db  078h, 0B6h, 05Eh, 0A4h, 098h, 0B7h, 05Eh, 020h, 063h, 0B8h, 05Eh, 041h, 056h, 0B9h, 05Eh, 0CDh
        db  0ABh, 0BAh, 05Eh, 0C8h, 0F7h, 0BBh, 05Eh, 0EFh, 013h, 0BCh, 05Eh, 0CFh, 0DCh, 0D4h, 066h, 0E4h
        db  066h, 0C7h, 053h, 0A5h, 000h, 0E2h, 067h, 065h, 053h, 03Ah, 009h, 0E2h, 067h, 088h, 053h, 02Eh
        db  001h, 0E2h, 067h, 0DCh, 053h, 0CCh, 033h, 0E2h, 067h, 0D5h, 053h, 099h, 066h, 0E2h, 067h, 0D4h
        db  053h, 034h, 012h, 0E2h, 067h, 0A0h, 053h, 022h, 05Ah, 0E2h, 067h, 0A1h, 053h, 039h, 000h, 0E2h
        db  067h, 0B0h, 053h, 05Ah, 05Ah, 0E2h, 067h, 0B1h, 053h, 099h, 033h, 0E2h, 067h, 0B2h, 053h, 08Fh
        db  047h, 0E2h, 067h, 0B3h, 053h, 034h, 012h, 0E2h, 067h, 0B4h, 053h, 0A3h, 09Fh, 0E2h, 067h, 0B5h
        db  053h, 0A3h, 078h, 0E2h, 067h, 0B6h, 053h, 0A4h, 098h, 0E2h, 067h, 0B7h, 053h, 020h, 063h, 0E2h
        db  067h, 0B8h, 053h, 041h, 056h, 0E2h, 067h, 0B9h, 053h, 0CDh, 0ABh, 0E2h, 067h, 0BAh, 053h, 0C8h
        db  0F7h, 0E2h, 067h, 0BBh, 053h, 0EFh, 013h, 0E2h, 067h, 0BCh, 053h, 0CFh, 0DCh, 0E2h, 067h, 0C7h
        db  05Eh, 05Ah, 000h, 065h, 05Eh, 0C5h, 006h, 088h, 05Eh, 0D1h, 000h, 0DCh, 05Eh, 033h, 0CCh, 0D5h
        db  05Eh, 066h, 099h, 0D4h, 05Eh, 000h, 000h, 0A0h, 05Eh, 0BBh, 0A5h, 0A1h, 05Eh, 0C6h, 000h, 0B0h
        db  05Eh, 0A5h, 0A5h, 0B1h, 05Eh, 066h, 0CCh, 0B2h, 05Eh, 070h, 0B8h, 0B3h, 05Eh, 0CBh, 0EDh, 0B4h
        db  05Eh, 05Ch, 060h, 0B5h, 05Eh, 05Ch, 087h, 0B6h, 05Eh, 05Bh, 067h, 0B7h, 05Eh, 0DFh, 09Ch, 0B8h
        db  05Eh, 0BEh, 0A9h, 0B9h, 05Eh, 032h, 05Bh, 0BAh, 05Eh, 037h, 008h, 0BBh, 05Eh, 010h, 0ECh, 0BCh
        db  05Eh, 030h, 023h, 0D4h, 067h, 0E4h, 066h, 0C7h, 053h, 05Ah, 000h, 0E2h, 067h, 065h, 053h, 0C5h
        db  006h, 0E2h, 067h, 088h, 053h, 0D1h, 000h, 0E2h, 067h, 0DCh, 053h, 033h, 0CCh, 0E2h, 067h, 0D5h
        db  053h, 066h, 099h, 0E2h, 067h, 0D4h, 000h, 0E2h, 067h, 0D5h, 05Eh, 000h, 000h, 0D4h, 05Eh, 0CBh
        db  0EDh, 0D4h, 066h, 0E4h, 067h, 0D5h, 000h, 0E2h, 067h, 0D4h, 053h, 0CBh, 0EDh, 0E2h, 067h, 0A0h
        db  053h, 0BBh, 0A5h, 0E2h, 067h, 0A1h, 053h, 0C6h, 000h, 0E2h, 067h, 0B0h, 053h, 0A5h, 0A5h, 0E2h
        db  067h, 0B1h, 053h, 066h, 0CCh, 0E2h, 067h, 0B2h, 053h, 070h, 0B8h, 0E2h, 067h, 0B3h, 053h, 0CBh
        db  0EDh, 0E2h, 067h, 0B4h, 053h, 05Ch, 060h, 0E2h, 067h, 0B5h, 053h, 05Ch, 087h, 0E2h, 067h, 0B6h
        db  053h, 05Bh, 067h, 0E2h, 067h, 0B7h, 053h, 0DFh, 09Ch, 0E2h, 067h, 0B8h, 053h, 0BEh, 0A9h, 0E2h
        db  067h, 0B9h, 053h, 032h, 05Bh, 0E2h, 067h, 0BAh, 053h, 037h, 008h, 0E2h, 067h, 0BBh, 053h, 010h
        db  0ECh, 0E2h, 067h, 0BCh, 053h, 030h, 023h, 0E2h, 067h, 031h, 05Eh, 000h, 080h, 0C0h, 06Dh, 0DBh
        db  005h, 0E2h, 067h, 031h, 05Eh, 000h, 040h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h
        db  020h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h, 010h, 0C0h, 06Dh, 0DBh, 005h, 0E2h
        db  067h, 031h, 05Eh, 000h, 008h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h, 008h, 0C0h
        db  06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h, 004h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h
        db  05Eh, 000h, 002h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h, 081h, 0C0h, 06Dh, 0DBh
        db  005h, 0E2h, 067h, 031h, 05Eh, 080h, 000h, 0C0h, 06Dh, 015h, 006h, 0E2h, 067h, 031h, 05Eh, 040h
        db  000h, 0C0h, 06Dh, 015h, 006h, 0E2h, 067h, 031h, 05Eh, 020h, 000h, 0C0h, 06Dh, 015h, 006h, 0E2h
        db  067h, 031h, 05Eh, 010h, 000h, 0C0h, 06Dh, 015h, 006h, 0E2h, 067h, 031h, 05Eh, 008h, 000h, 0C0h
        db  06Dh, 015h, 006h, 0E2h, 067h, 0A4h, 067h, 0A3h, 067h, 031h, 05Eh, 004h, 000h, 0C0h, 06Dh, 015h
        db  006h, 0E2h, 067h, 0ABh, 067h, 0A9h, 067h, 0A5h, 067h, 0A4h, 066h, 0A3h, 066h, 0A7h, 067h, 0A6h
        db  067h, 031h, 05Eh, 002h, 000h, 0C0h, 06Dh, 015h, 006h, 0E2h, 067h, 0ABh, 066h, 0A9h, 066h, 0A5h
        db  066h, 067h, 01Fh, 0A7h, 066h, 0A6h, 066h, 00Bh, 05Eh, 000h, 080h, 0C0h, 067h, 031h, 000h, 0D1h
        db  054h, 000h, 0FFh, 0EAh, 04Fh, 033h, 010h, 033h, 052h, 0FFh, 0FEh, 034h, 010h, 001h, 042h, 032h
        db  010h, 034h, 052h, 000h, 001h, 0D1h, 052h, 0FFh, 0FFh, 036h, 010h, 034h, 000h, 080h, 010h, 081h
        db  010h, 082h, 010h, 083h, 010h, 084h, 010h, 085h, 010h, 032h, 000h, 0C3h, 010h, 033h, 005h, 034h
        db  006h, 067h, 01Fh, 034h, 000h, 00Bh, 010h, 0D2h, 000h, 034h, 04Bh, 0E2h, 067h, 0C3h, 000h, 00Bh
        db  010h, 036h, 04Bh, 0E2h, 067h, 0D3h, 000h, 00Bh, 010h, 033h, 04Bh, 0E2h, 067h, 080h, 000h, 033h
        db  04Bh, 0E2h, 067h, 081h, 000h, 033h, 04Bh, 0E2h, 067h, 082h, 000h, 031h, 04Bh, 0E2h, 067h, 083h
        db  000h, 033h, 04Bh, 0E2h, 067h, 084h, 000h, 033h, 04Bh, 0E2h, 067h, 085h, 000h, 031h, 04Bh, 0E2h
        db  067h, 031h, 000h, 0A2h, 010h, 0A3h, 010h, 0A4h, 010h, 0A5h, 010h, 0A6h, 010h, 0A7h, 010h, 0A8h
        db  010h, 0A9h, 010h, 0AAh, 010h, 0ABh, 010h, 0ACh, 010h, 0ADh, 010h, 0AEh, 010h, 0D1h, 052h, 0FFh
        db  0FFh, 030h, 010h, 0D1h, 054h, 0FEh, 0FFh, 035h, 010h, 031h, 000h, 030h, 000h, 076h, 010h, 061h
        db  05Eh, 0F5h, 0FFh, 064h, 01Fh, 067h, 01Fh, 0AEh, 000h, 035h, 04Bh, 0E2h, 067h, 0ADh, 000h, 035h
        db  04Bh, 0E2h, 067h, 0ACh, 000h, 035h, 04Bh, 0E2h, 067h, 0A4h, 000h, 035h, 04Bh, 0E2h, 067h, 0A3h
        db  000h, 035h, 04Bh, 0E2h, 067h, 0ABh, 000h, 030h, 04Bh, 0E2h, 067h, 0A9h, 000h, 030h, 04Bh, 0E2h
        db  067h, 0A7h, 000h, 030h, 04Bh, 0E2h, 067h, 0A6h, 000h, 030h, 04Bh, 0E2h, 067h, 0A5h, 000h, 030h
        db  04Bh, 0E2h, 067h, 0A2h, 000h, 030h, 04Bh, 0E2h, 067h, 076h, 000h, 031h, 04Bh, 0E2h, 067h, 061h
        db  05Eh, 00Ah, 000h, 067h, 01Fh, 0A8h, 000h, 030h, 04Bh, 0E2h, 067h, 0AAh, 000h, 030h, 04Bh, 0E2h
        db  067h, 0C0h, 067h, 00Bh, 05Eh, 003h, 080h, 088h, 076h, 0F4h, 00Fh, 0F5h, 01Fh, 0CCh, 01Fh, 0CCh
        db  000h, 0E7h, 053h, 001h, 000h, 0E2h, 06Bh, 05Eh, 006h, 0CCh, 000h, 0E7h, 053h, 002h, 000h, 0E2h
        db  06Bh, 05Eh, 006h, 0CCh, 000h, 0E7h, 053h, 003h, 000h, 0E2h, 06Bh, 05Eh, 006h, 0CCh, 01Fh, 0CCh
        db  000h, 0ABh, 05Eh, 080h, 004h, 061h, 05Eh, 040h, 000h, 0D1h, 04Bh, 095h, 01Fh, 0CBh, 000h, 0CCh
        db  04Ah, 0ABh, 06Ah, 07Ah, 006h, 0D1h, 053h, 011h, 011h, 0E2h, 06Ah, 086h, 006h, 00Bh, 05Eh, 001h
        db  080h, 088h, 076h, 00Bh, 05Eh, 000h, 080h, 0CCh, 01Fh, 0CCh, 000h, 0F1h, 01Fh, 08Eh, 05Eh, 010h
        db  000h, 0F0h, 01Fh, 0F4h, 01Fh, 08Eh, 070h, 0E8h, 01Fh, 0C0h, 06Dh, 04Fh, 000h, 074h, 05Eh, 099h
        db  000h, 075h, 01Fh, 061h, 05Eh, 010h, 000h, 00Eh, 000h, 030h, 010h, 00Fh, 000h, 031h, 010h, 00Bh
        db  05Eh, 000h, 080h, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 09Bh, 05Eh, 0FFh, 0FFh, 01Bh, 01Fh, 0B0h
        db  000h, 035h, 010h, 0B1h, 000h, 036h, 010h, 0B2h, 000h, 037h, 010h, 030h, 05Eh, 020h, 000h, 031h
        db  05Eh, 004h, 010h, 032h, 01Fh, 033h, 01Fh, 034h, 01Fh, 0C4h, 06Ah, 0B9h, 006h, 0D8h, 06Bh, 0B8h
        db  006h, 0CAh, 06Bh, 0DBh, 006h, 0B1h, 076h, 088h, 000h, 0A5h, 06Ah, 0B9h, 006h, 09Bh, 05Eh, 0FFh
        db  0FFh, 087h, 05Eh, 000h, 080h, 069h, 05Eh, 00Ch, 000h, 0C5h, 05Eh, 022h, 030h, 0C3h, 06Ah, 0C3h
        db  006h, 0CAh, 06Bh, 0D6h, 006h, 0D8h, 06Bh, 0D4h, 006h, 0D9h, 05Eh, 0FEh, 030h, 0C3h, 06Ah, 0CBh
        db  006h, 0CAh, 06Bh, 0D6h, 006h, 0D8h, 06Bh, 0D4h, 006h, 0D9h, 05Eh, 0FEh, 031h, 0C3h, 076h, 088h
        db  000h, 0B9h, 076h, 0D8h, 05Eh, 0FEh, 030h, 0D8h, 06Ah, 0D8h, 006h, 088h, 000h, 00Ch, 053h, 001h
        db  000h, 0E2h, 06Bh, 0E3h, 006h, 00Dh, 053h, 006h, 000h, 0E2h, 06Ah, 0F2h, 006h, 0C0h, 06Dh, 04Fh
        db  000h, 0E8h, 00Fh, 08Eh, 070h, 0E8h, 01Fh, 0C0h, 06Dh, 04Fh, 000h, 074h, 05Eh, 0B9h, 000h, 075h
        db  01Fh, 061h, 05Eh, 010h, 000h, 0FFh, 060h, 0A5h, 06Ah, 0F0h, 006h, 0E8h, 01Fh, 0F0h, 00Fh, 00Bh
        db  05Eh, 000h, 080h, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 09Bh, 05Eh, 0FFh, 0FFh, 0CAh, 06Bh, 0DBh
        db  006h, 0FFh, 060h, 0FFh, 060h, 09Bh, 054h, 000h, 004h, 005h, 0C5h, 074h, 05Eh, 0B9h, 000h, 0FBh
        db  076h, 074h, 05Eh, 099h, 000h, 09Bh, 05Eh, 0FFh, 0FFh, 0FBh, 076h, 0FFh, 060h, 0FFh, 060h, 0D4h
        db  010h, 0FFh, 060h, 0FFh, 060h, 0FFh, 060h, 0FFh, 060h, 0D7h, 06Bh, 015h, 007h, 0FFh, 060h, 0FFh
        db  060h, 018h, 0C7h, 0FFh, 060h, 0FFh, 060h, 01Bh, 0D1h, 0FFh, 060h, 0FFh, 060h, 01Eh, 099h, 0FFh
        db  060h, 0FFh, 060h, 066h, 06Bh, 022h, 007h, 0FFh, 060h, 0FFh, 060h, 025h, 0BDh, 0FFh, 060h, 0FFh
        db  060h, 028h, 0A7h, 0FFh, 060h, 0FFh, 060h, 02Bh, 0BBh, 0FFh, 060h, 0FFh, 060h, 02Eh, 0A5h, 0FFh
        db  060h, 0FFh, 060h, 031h, 0ADh, 0FFh, 060h, 0FFh, 060h, 034h, 0A1h, 0FFh, 060h, 0FFh, 060h, 037h
        db  0A3h, 0FFh, 060h, 0FFh, 060h, 03Ah, 0C3h, 0FFh, 060h, 0FFh, 060h, 03Dh, 083h, 0FFh, 060h, 0FFh
        db  060h, 040h, 085h, 0FFh, 060h, 0FFh, 060h, 043h, 093h, 0FFh, 060h, 0FFh, 060h, 043h, 077h, 06Ch
        db  01Fh, 0DBh, 05Eh, 018h, 000h, 058h, 079h, 08Ch, 05Eh, 000h, 007h, 0DBh, 05Eh, 000h, 080h, 06Bh
        db  07Bh, 06Eh, 000h, 0E2h, 052h, 051h, 007h, 0C0h, 06Fh, 060h, 077h, 060h, 077h, 060h, 077h, 060h
        db  077h, 063h, 077h, 08Dh, 077h, 060h, 077h, 060h, 077h, 060h, 077h, 060h, 077h, 060h, 077h, 060h
        db  077h, 0A0h, 077h, 060h, 077h, 060h, 077h, 09Bh, 05Eh, 0AFh, 0FFh, 058h, 079h, 09Bh, 05Eh, 010h
        db  000h, 084h, 054h, 0FFh, 000h, 0D1h, 055h, 002h, 000h, 06Bh, 0D5h, 058h, 079h, 074h, 054h, 0F7h
        db  0FFh, 074h, 010h, 0A3h, 05Eh, 0D0h, 007h, 061h, 05Eh, 000h, 001h, 09Bh, 05Eh, 010h, 000h, 084h
        db  01Fh, 0A3h, 06Ah, 075h, 007h, 084h, 054h, 0FFh, 000h, 0D1h, 055h, 064h, 000h, 06Bh, 0D5h, 074h
        db  057h, 008h, 000h, 074h, 010h, 0FFh, 060h, 0FFh, 060h, 09Bh, 05Eh, 010h, 000h, 07Eh, 000h, 04Dh
        db  054h, 001h, 000h, 0E2h, 06Ah, 058h, 009h, 0A3h, 05Eh, 060h, 0EAh, 061h, 05Eh, 000h, 001h, 058h
        db  079h, 09Bh, 05Eh, 020h, 000h, 04Dh, 054h, 001h, 000h, 0E2h, 06Bh, 058h, 009h, 04Dh, 057h, 001h
        db  000h, 04Dh, 010h, 04Fh, 054h, 080h, 0FFh, 04Fh, 010h, 0A3h, 05Eh, 060h, 0EAh, 061h, 05Eh, 000h
        db  001h, 063h, 05Eh, 002h, 000h, 058h, 079h, 09Ah, 05Eh, 0FFh, 0FFh, 09Bh, 05Eh, 0FFh, 0FFh, 06Bh
        db  07Bh, 0C8h, 000h, 0C0h, 069h, 0B0h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0BAh, 077h, 0B8h, 077h, 058h, 079h, 0C8h, 077h, 058h
        db  079h, 058h, 079h, 058h, 079h, 058h, 079h, 077h, 01Fh, 058h, 079h, 0F6h, 00Fh, 058h, 079h, 0DBh
        db  05Eh, 020h, 000h, 030h, 000h, 002h, 042h, 0D1h, 057h, 000h, 030h, 069h, 05Eh, 004h, 000h, 0C4h
        db  010h, 0E5h, 05Eh, 008h, 000h, 058h, 079h, 088h, 000h, 01Ch, 000h, 0E2h, 06Ah, 058h, 009h, 0C0h
        db  069h, 0D0h, 007h, 000h, 000h, 000h, 000h, 0E0h, 077h, 0EDh, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h
        db  077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h
        db  077h, 0FBh, 077h, 0FBh, 077h, 0FBh, 077h, 0C8h, 054h, 002h, 000h, 0E2h, 06Ah, 0E8h, 007h, 077h
        db  01Fh, 01Ch, 01Fh, 087h, 01Fh, 058h, 079h, 01Ch, 000h, 0E6h, 010h, 01Ch, 01Fh, 087h, 01Fh, 058h
        db  079h, 0C8h, 054h, 002h, 000h, 0E4h, 0C5h, 04Bh, 054h, 040h, 000h, 0F5h, 0C5h, 01Ch, 000h, 0E6h
        db  010h, 04Bh, 054h, 0BFh, 0FFh, 04Bh, 010h, 01Ch, 01Fh, 087h, 01Fh, 058h, 079h, 01Ch, 054h, 000h
        db  0E0h, 0E2h, 06Bh, 002h, 008h, 01Ch, 01Fh, 087h, 01Fh, 058h, 079h, 01Ch, 054h, 000h, 080h, 0E2h
        db  06Ah, 058h, 009h, 061h, 05Eh, 000h, 004h, 063h, 05Eh, 000h, 008h, 058h, 079h, 062h, 000h, 0C0h
        db  069h, 010h, 008h, 000h, 000h, 000h, 000h, 058h, 079h, 044h, 078h, 0A3h, 078h, 085h, 078h, 05Bh
        db  078h, 06Ah, 078h, 058h, 079h, 088h, 078h, 058h, 079h, 01Ch, 078h, 058h, 079h, 0BEh, 078h, 0FFh
        db  060h, 022h, 078h, 0FFh, 060h, 0FFh, 060h, 09Bh, 05Eh, 040h, 000h, 0F2h, 01Fh, 0BEh, 05Eh, 001h
        db  000h, 0C8h, 05Eh, 001h, 000h, 0DBh, 05Eh, 080h, 000h, 086h, 05Eh, 000h, 002h, 0A5h, 05Eh, 0E8h
        db  003h, 061h, 05Eh, 010h, 000h, 065h, 05Eh, 0EFh, 0FFh, 04Dh, 054h, 001h, 000h, 036h, 0C4h, 063h
        db  05Eh, 002h, 000h, 011h, 05Eh, 008h, 000h, 089h, 05Eh, 000h, 007h, 0E6h, 05Eh, 0FFh, 0FEh, 0E4h
        db  05Eh, 000h, 080h, 01Ch, 01Fh, 087h, 01Fh, 0BFh, 06Ah, 040h, 008h, 077h, 01Fh, 058h, 079h, 04Dh
        db  054h, 001h, 000h, 058h, 0C4h, 04Dh, 054h, 002h, 000h, 058h, 0C5h, 04Fh, 054h, 07Fh, 000h, 0D1h
        db  053h, 005h, 000h, 055h, 0C4h, 04Fh, 052h, 001h, 000h, 04Fh, 010h, 061h, 05Eh, 000h, 001h, 058h
        db  079h, 04Dh, 057h, 00Eh, 000h, 04Dh, 010h, 062h, 05Eh, 002h, 000h, 058h, 079h, 011h, 052h, 05Eh
        db  008h, 0C0h, 06Fh, 058h, 079h, 058h, 079h, 058h, 079h, 0B9h, 07Bh, 075h, 07Bh, 058h, 079h, 058h
        db  079h, 058h, 079h, 0A1h, 07Ah, 08Ch, 05Eh, 000h, 006h, 06Bh, 07Bh, 011h, 053h, 005h, 000h, 0E2h
        db  06Ah, 067h, 008h, 080h, 000h, 0E2h, 06Bh, 080h, 008h, 081h, 000h, 0E2h, 06Bh, 080h, 008h, 082h
        db  000h, 0E2h, 06Bh, 080h, 008h, 083h, 000h, 0E2h, 06Bh, 080h, 008h, 084h, 000h, 0E2h, 06Bh, 080h
        db  008h, 085h, 000h, 0E2h, 06Ah, 082h, 008h, 0E4h, 05Eh, 020h, 000h, 061h, 05Eh, 008h, 000h, 058h
        db  079h, 08Ch, 05Eh, 000h, 005h, 06Bh, 07Bh, 011h, 052h, 08Bh, 008h, 0C0h, 06Fh, 058h, 079h, 075h
        db  07Bh, 0DBh, 07Ah, 058h, 079h, 075h, 07Bh, 058h, 079h, 094h, 078h, 058h, 079h, 058h, 079h, 061h
        db  05Eh, 002h, 000h, 01Bh, 054h, 000h, 001h, 0E2h, 06Ah, 0A0h, 008h, 01Bh, 054h, 0FFh, 0FEh, 01Bh
        db  010h, 0E4h, 05Eh, 080h, 000h, 058h, 079h, 0E4h, 05Eh, 088h, 000h, 058h, 079h, 011h, 053h, 004h
        db  000h, 0ABh, 0C5h, 062h, 05Eh, 004h, 000h, 0E4h, 05Eh, 040h, 000h, 058h, 079h, 011h, 053h, 003h
        db  000h, 0B4h, 0C5h, 04Bh, 054h, 020h, 000h, 0B4h, 0C5h, 04Bh, 052h, 010h, 000h, 04Bh, 010h, 061h
        db  05Eh, 080h, 000h, 069h, 05Eh, 00Ch, 000h, 030h, 000h, 002h, 042h, 0D1h, 057h, 000h, 030h, 0C4h
        db  010h, 058h, 079h, 062h, 05Eh, 000h, 008h, 011h, 053h, 008h, 000h, 0CFh, 0C5h, 01Ch, 054h, 000h
        db  080h, 0E2h, 06Ah, 058h, 009h, 069h, 05Eh, 00Ch, 000h, 030h, 000h, 002h, 042h, 0D1h, 057h, 000h
        db  030h, 0C4h, 010h, 058h, 079h, 011h, 053h, 007h, 000h, 0E2h, 06Bh, 058h, 009h, 0B8h, 057h, 001h
        db  000h, 0B8h, 010h, 0BEh, 05Eh, 004h, 000h, 0E4h, 05Eh, 080h, 000h, 04Dh, 054h, 010h, 000h, 0E4h
        db  0C5h, 04Dh, 057h, 010h, 000h, 04Dh, 010h, 09Bh, 05Eh, 020h, 000h, 099h, 05Eh, 020h, 000h, 061h
        db  05Eh, 002h, 008h, 065h, 05Eh, 05Fh, 0FDh, 04Dh, 054h, 001h, 000h, 0EDh, 0C4h, 063h, 05Eh, 002h
        db  000h, 0E9h, 05Eh, 080h, 000h, 011h, 05Eh, 006h, 000h, 089h, 05Eh, 000h, 006h, 0E5h, 05Eh, 001h
        db  000h, 0BFh, 06Ah, 0F5h, 008h, 087h, 01Fh, 058h, 079h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h, 04Dh, 054h, 0FCh, 0FFh, 04Dh, 010h, 01Dh, 021h, 01Eh
        db  023h, 01Eh, 023h, 056h, 09Ch, 01Eh, 000h, 03Eh, 04Dh, 00Fh, 0C5h, 01Fh, 000h, 03Fh, 04Dh, 00Fh
        db  0C5h, 020h, 000h, 040h, 04Dh, 0E2h, 01Fh, 0D4h, 017h, 0D1h, 021h, 022h, 021h, 004h, 045h, 040h
        db  0C4h, 053h, 0D4h, 021h, 010h, 0E5h, 06Ah, 017h, 009h, 056h, 08Fh, 0D1h, 021h, 0D1h, 005h, 0CFh
        db  053h, 001h, 000h, 032h, 0C4h, 0CFh, 053h, 002h, 000h, 039h, 0C4h, 0CEh, 000h, 0EAh, 000h, 023h
        db  010h, 021h, 000h, 023h, 04Dh, 040h, 0C4h, 053h, 0D4h, 021h, 010h, 023h, 000h, 0CDh, 055h, 001h
        db  000h, 053h, 097h, 0FFh, 021h, 001h, 045h, 02Eh, 0C5h, 017h, 079h, 027h, 021h, 021h, 000h, 004h
        db  045h, 040h, 0C4h, 053h, 0D4h, 021h, 010h, 017h, 079h, 024h, 023h, 021h, 000h, 008h, 045h, 040h
        db  0C4h, 053h, 0D4h, 021h, 010h, 017h, 079h, 0DEh, 05Eh, 0FFh, 000h, 0E5h, 061h, 0E3h, 01Bh, 0CAh
        db  000h, 058h, 0AFh, 01Dh, 054h, 00Fh, 000h, 0D1h, 052h, 04Bh, 009h, 0C0h, 06Fh, 09Ch, 07Ah, 060h
        db  079h, 062h, 07Ah, 0E6h, 07Ah, 09Ah, 079h, 0B7h, 079h, 020h, 07Ah, 058h, 079h, 0DEh, 05Eh, 0FFh
        db  000h, 0E5h, 061h, 0CAh, 000h, 0CAh, 000h, 0DBh, 000h, 0E2h, 069h, 010h, 000h, 02Fh, 005h, 02Eh
        db  006h, 02Dh, 007h, 02Ch, 000h, 0C0h, 065h, 062h, 05Eh, 008h, 000h, 022h, 000h, 022h, 053h, 007h
        db  000h, 094h, 0C5h, 011h, 053h, 002h, 000h, 058h, 0C5h, 058h, 0A0h, 058h, 0ADh, 058h, 0A5h, 079h
        db  085h, 04Bh, 054h, 001h, 000h, 085h, 0C5h, 04Bh, 057h, 001h, 000h, 04Bh, 010h, 0D5h, 054h, 002h
        db  000h, 081h, 0C5h, 0E4h, 05Eh, 002h, 000h, 058h, 079h, 04Bh, 054h, 002h, 000h, 0E2h, 06Bh, 0E3h
        db  00Ah, 04Bh, 057h, 002h, 000h, 04Bh, 010h, 073h, 079h, 04Bh, 057h, 040h, 000h, 04Bh, 010h, 058h
        db  079h, 0A5h, 05Eh, 028h, 00Ah, 061h, 05Eh, 019h, 000h, 065h, 05Eh, 04Fh, 0FFh, 0E5h, 05Eh, 001h
        db  000h, 011h, 05Eh, 004h, 000h, 089h, 05Eh, 000h, 004h, 0E4h, 05Eh, 040h, 000h, 058h, 079h, 022h
        db  000h, 022h, 053h, 00Bh, 004h, 0E2h, 06Ah, 05Ch, 00Bh, 058h, 079h, 011h, 052h, 09Dh, 009h, 0C0h
        db  06Fh, 058h, 079h, 0A1h, 07Bh, 058h, 079h, 058h, 079h, 0D0h, 07Bh, 058h, 079h, 0B4h, 079h, 058h
        db  079h, 0E1h, 06Ah, 058h, 009h, 061h, 05Eh, 000h, 004h, 065h, 05Eh, 0FFh, 0F7h, 04Dh, 054h, 001h
        db  000h, 0B0h, 0C4h, 063h, 05Eh, 002h, 000h, 01Ch, 01Fh, 011h, 05Eh, 007h, 000h, 058h, 079h, 05Ch
        db  05Eh, 002h, 000h, 0D1h, 079h, 01Eh, 000h, 05Dh, 010h, 01Fh, 000h, 05Eh, 010h, 020h, 000h, 05Fh
        db  010h, 011h, 052h, 0C0h, 009h, 0C0h, 06Fh, 058h, 079h, 0A1h, 07Bh, 058h, 079h, 058h, 079h, 0F8h
        db  079h, 058h, 079h, 0C9h, 079h, 058h, 079h, 058h, 079h, 0B4h, 0C2h, 0C1h, 06Bh, 018h, 00Ah, 0C2h
        db  06Bh, 018h, 00Ah, 0E8h, 06Bh, 040h, 00Ah, 038h, 07Ah, 0D5h, 05Eh, 010h, 000h, 087h, 01Fh, 01Ch
        db  01Fh, 0B8h, 054h, 0FEh, 0FFh, 0B8h, 010h, 0BEh, 05Eh, 003h, 000h, 0FFh, 060h, 0BFh, 06Ah, 0DBh
        db  009h, 077h, 01Fh, 0E5h, 05Eh, 001h, 000h, 074h, 054h, 0FDh, 0FFh, 004h, 047h, 074h, 010h, 0DBh
        db  05Eh, 080h, 000h, 0A5h, 05Eh, 028h, 00Ah, 061h, 05Eh, 019h, 000h, 065h, 05Eh, 04Fh, 0FFh, 04Dh
        db  054h, 001h, 000h, 0F1h, 0C4h, 063h, 05Eh, 002h, 000h, 011h, 05Eh, 004h, 000h, 089h, 05Eh, 000h
        db  004h, 058h, 079h, 087h, 01Fh, 0D3h, 079h, 062h, 05Eh, 008h, 000h, 0C1h, 06Ah, 004h, 00Ah, 0C2h
        db  06Ah, 018h, 00Ah, 061h, 05Eh, 001h, 000h, 01Bh, 054h, 0FFh, 0FDh, 01Bh, 010h, 058h, 079h, 0C2h
        db  06Bh, 018h, 00Ah, 0E8h, 06Bh, 010h, 00Ah, 0E4h, 05Eh, 001h, 000h, 01Eh, 000h, 03Eh, 010h, 01Fh
        db  000h, 03Fh, 010h, 020h, 000h, 040h, 010h, 061h, 05Eh, 081h, 000h, 063h, 05Eh, 084h, 000h, 01Bh
        db  057h, 000h, 002h, 01Bh, 010h, 058h, 079h, 0DBh, 000h, 0E2h, 069h, 010h, 000h, 02Fh, 005h, 02Eh
        db  006h, 02Dh, 007h, 02Ch, 000h, 0C0h, 065h, 01Eh, 000h, 05Dh, 010h, 01Fh, 000h, 05Eh, 010h, 020h
        db  000h, 05Fh, 010h, 011h, 052h, 029h, 00Ah, 0C0h, 06Fh, 058h, 079h, 0A1h, 07Bh, 058h, 079h, 058h
        db  079h, 044h, 07Ah, 058h, 079h, 032h, 07Ah, 058h, 079h, 058h, 079h, 0C1h, 06Bh, 018h, 00Ah, 0C2h
        db  06Bh, 018h, 00Ah, 0E8h, 06Bh, 040h, 00Ah, 0E4h, 05Eh, 001h, 000h, 01Eh, 000h, 03Eh, 010h, 01Fh
        db  000h, 03Fh, 010h, 020h, 000h, 040h, 010h, 01Bh, 057h, 000h, 001h, 01Bh, 010h, 058h, 079h, 062h
        db  05Eh, 008h, 000h, 0C1h, 06Bh, 018h, 00Ah, 0C2h, 06Bh, 018h, 00Ah, 061h, 05Eh, 080h, 000h, 063h
        db  05Eh, 004h, 000h, 01Bh, 054h, 000h, 002h, 054h, 0C4h, 086h, 05Eh, 008h, 000h, 057h, 07Ah, 01Bh
        db  057h, 000h, 002h, 01Bh, 010h, 0E8h, 06Bh, 018h, 00Ah, 0E4h, 05Eh, 001h, 000h, 01Eh, 000h, 03Eh
        db  010h, 01Fh, 000h, 03Fh, 010h, 020h, 000h, 040h, 010h, 058h, 079h, 05Ah, 057h, 000h, 040h, 05Ah
        db  010h, 029h, 057h, 002h, 000h, 029h, 010h, 011h, 052h, 06Bh, 00Ah, 0C0h, 06Fh, 058h, 079h, 09Eh
        db  07Bh, 0DEh, 07Ah, 074h, 07Ah, 077h, 07Ah, 0C2h, 07Ah, 0F6h, 079h, 058h, 079h, 0D1h, 079h, 0E1h
        db  06Ah, 00Bh, 00Bh, 058h, 079h, 0BEh, 05Eh, 002h, 000h, 0E6h, 05Eh, 040h, 000h, 0A5h, 05Eh, 028h
        db  00Ah, 061h, 05Eh, 011h, 000h, 0B0h, 000h, 024h, 04Dh, 0E2h, 06Bh, 09Ch, 00Ah, 0B1h, 000h, 025h
        db  04Dh, 0E2h, 06Bh, 09Ch, 00Ah, 0B2h, 000h, 026h, 04Dh, 0E2h, 06Bh, 09Ch, 00Ah, 027h, 053h, 001h
        db  000h, 0E2h, 06Ah, 09Ch, 00Ah, 0BFh, 06Ah, 08Fh, 00Ah, 065h, 054h, 008h, 000h, 0E2h, 06Ah, 018h
        db  00Ah, 061h, 05Eh, 020h, 000h, 063h, 05Eh, 008h, 000h, 0BFh, 06Ah, 099h, 00Ah, 058h, 079h, 062h
        db  05Eh, 008h, 000h, 0BFh, 06Ah, 09Eh, 00Ah, 058h, 079h, 0A5h, 05Eh, 0E8h, 003h, 061h, 05Eh, 090h
        db  000h, 065h, 05Eh, 0EBh, 0FFh, 04Dh, 054h, 001h, 000h, 0ACh, 0C4h, 063h, 05Eh, 002h, 000h, 04Bh
        db  054h, 0C3h, 0FFh, 04Bh, 010h, 05Ch, 05Eh, 001h, 000h, 0D5h, 05Eh, 010h, 020h, 087h, 01Fh, 01Ch
        db  01Fh, 05Bh, 05Eh, 003h, 000h, 011h, 05Eh, 003h, 000h, 089h, 05Eh, 000h, 003h, 0B8h, 054h, 0FEh
        db  0FFh, 0B8h, 010h, 074h, 054h, 0FDh, 0FFh, 074h, 010h, 058h, 079h, 0E1h, 06Ah, 00Eh, 00Bh, 04Dh
        db  054h, 0FCh, 0FFh, 04Dh, 010h, 0A5h, 05Eh, 0E8h, 003h, 061h, 05Eh, 090h, 000h, 065h, 05Eh, 0EBh
        db  0FFh, 04Bh, 054h, 0C3h, 0FFh, 04Bh, 010h, 0D5h, 05Eh, 000h, 020h, 01Ch, 01Fh, 087h, 01Fh, 05Bh
        db  05Eh, 003h, 000h, 011h, 05Eh, 003h, 000h, 089h, 05Eh, 000h, 003h, 058h, 079h, 08Ch, 05Eh, 000h
        db  003h, 06Bh, 07Bh, 0E6h, 05Eh, 040h, 000h, 08Ch, 05Eh, 000h, 001h, 06Bh, 07Bh, 08Ch, 05Eh, 000h
        db  002h, 06Bh, 07Bh, 05Ah, 057h, 020h, 000h, 05Ah, 010h, 029h, 057h, 002h, 000h, 029h, 010h, 011h
        db  052h, 0EFh, 00Ah, 0C0h, 06Fh, 058h, 079h, 058h, 079h, 0DDh, 07Bh, 0FEh, 07Ah, 0D0h, 07Bh, 058h
        db  079h, 0FBh, 07Ah, 058h, 079h, 0F8h, 07Ah, 0E1h, 06Ah, 0D1h, 009h, 058h, 079h, 05Ch, 05Eh, 001h
        db  000h, 0D1h, 079h, 0E1h, 06Bh, 028h, 00Bh, 0B0h, 000h, 01Eh, 04Dh, 0E2h, 06Bh, 009h, 00Bh, 0B1h
        db  000h, 01Fh, 04Dh, 009h, 0C5h, 0B2h, 000h, 020h, 04Dh, 0EAh, 06Bh, 025h, 00Bh, 074h, 057h, 004h
        db  000h, 074h, 010h, 04Dh, 054h, 0FCh, 0FFh, 04Dh, 010h, 0BEh, 05Eh, 003h, 000h, 0A5h, 05Eh, 028h
        db  00Ah, 061h, 05Eh, 019h, 000h, 065h, 05Eh, 04Fh, 0FFh, 01Ch, 01Fh, 087h, 01Fh, 0D5h, 01Fh, 011h
        db  05Eh, 004h, 000h, 089h, 05Eh, 000h, 004h, 0E5h, 05Eh, 001h, 000h, 0BFh, 06Ah, 022h, 00Bh, 058h
        db  079h, 05Bh, 05Eh, 004h, 000h, 058h, 079h, 03Eh, 000h, 024h, 04Dh, 0E2h, 06Bh, 054h, 00Bh, 03Fh
        db  000h, 025h, 04Dh, 0E2h, 06Bh, 054h, 00Bh, 040h, 000h, 026h, 04Dh, 0E2h, 06Bh, 054h, 00Bh, 04Bh
        db  054h, 020h, 000h, 0E2h, 06Ah, 050h, 00Bh, 04Bh, 054h, 008h, 000h, 0E2h, 06Ah, 050h, 00Bh, 04Dh
        db  054h, 0FCh, 0FFh, 04Dh, 010h, 074h, 057h, 002h, 000h, 074h, 010h, 0A5h, 05Eh, 0E8h, 003h, 061h
        db  05Eh, 018h, 000h, 065h, 05Eh, 0EFh, 0FFh, 01Ch, 01Fh, 0D5h, 05Eh, 000h, 081h, 011h, 05Eh, 008h
        db  000h, 089h, 05Eh, 000h, 007h, 058h, 079h, 04Bh, 052h, 004h, 000h, 04Bh, 010h, 058h, 079h, 05Ch
        db  05Eh, 003h, 000h, 0E4h, 05Eh, 010h, 000h, 01Ch, 01Fh, 087h, 05Eh, 000h, 008h, 00Bh, 07Bh, 08Ch
        db  05Eh, 000h, 004h, 0A0h, 054h, 000h, 080h, 0E2h, 06Bh, 058h, 009h, 074h, 054h, 0BFh, 0FFh, 074h
        db  010h, 05Ah, 057h, 000h, 001h, 05Ah, 010h, 029h, 057h, 002h, 000h, 029h, 010h, 0E8h, 01Fh, 0FFh
        db  060h, 0DCh, 05Eh, 0FFh, 0FFh, 089h, 05Eh, 000h, 000h, 01Bh, 057h, 000h, 080h, 01Bh, 010h, 058h
        db  079h, 0C0h, 06Dh, 078h, 00Bh, 058h, 079h, 074h, 054h, 0FBh, 0FFh, 0D1h, 057h, 080h, 000h, 074h
        db  010h, 0F2h, 01Fh, 0BEh, 05Eh, 001h, 000h, 0C8h, 05Eh, 001h, 000h, 0D5h, 05Eh, 000h, 020h, 05Bh
        db  05Eh, 003h, 000h, 011h, 05Eh, 003h, 000h, 089h, 05Eh, 000h, 003h, 0A5h, 05Eh, 0E8h, 003h, 061h
        db  05Eh, 090h, 000h, 065h, 05Eh, 0EBh, 0FFh, 04Dh, 054h, 001h, 000h, 095h, 0C4h, 063h, 05Eh, 002h
        db  000h, 04Bh, 054h, 0C3h, 0FFh, 04Bh, 010h, 0BFh, 06Ah, 098h, 00Bh, 01Ch, 01Fh, 087h, 01Fh, 077h
        db  01Fh, 0C0h, 067h, 08Ch, 05Eh, 000h, 000h, 06Bh, 07Bh, 074h, 057h, 084h, 000h, 074h, 010h, 04Bh
        db  054h, 0FCh, 0FFh, 04Bh, 010h, 04Dh, 057h, 010h, 000h, 04Dh, 010h, 09Bh, 05Eh, 0FFh, 0FFh, 099h
        db  05Eh, 020h, 000h, 061h, 05Eh, 001h, 000h, 065h, 05Eh, 07Fh, 0FFh, 0E4h, 05Eh, 002h, 000h, 011h
        db  05Eh, 002h, 000h, 089h, 05Eh, 000h, 002h, 058h, 079h, 061h, 05Eh, 084h, 000h, 065h, 05Eh, 0DBh
        db  0FFh, 04Dh, 054h, 001h, 000h, 0C2h, 0C4h, 063h, 05Eh, 002h, 000h, 0D5h, 05Eh, 000h, 040h, 01Ch
        db  01Fh, 087h, 01Fh, 04Dh, 054h, 002h, 000h, 0CBh, 0C4h, 05Bh, 05Eh, 002h, 000h, 011h, 05Eh, 005h
        db  000h, 089h, 05Eh, 000h, 005h, 058h, 079h, 0BEh, 05Eh, 002h, 000h, 0E6h, 05Eh, 040h, 000h, 062h
        db  05Eh, 008h, 000h, 0A5h, 05Eh, 028h, 00Ah, 061h, 05Eh, 010h, 000h, 0BFh, 06Ah, 0DAh, 00Bh, 058h
        db  079h, 0BEh, 05Eh, 002h, 000h, 0E6h, 05Eh, 040h, 000h, 0BFh, 06Ah, 0E1h, 00Bh, 058h, 079h, 04Dh
        db  054h, 0F7h, 0FFh, 04Dh, 010h, 011h, 052h, 0EAh, 00Bh, 0C0h, 06Fh, 09Ch, 070h, 09Ch, 070h, 079h
        db  070h, 09Ch, 070h, 0F3h, 07Bh, 010h, 07Ch, 0F6h, 07Bh, 09Ch, 070h, 0F6h, 07Bh, 0C0h, 06Dh, 078h
        db  00Bh, 09Ch, 070h, 0A5h, 05Eh, 0E8h, 003h, 061h, 05Eh, 090h, 000h, 065h, 05Eh, 0EBh, 0FFh, 04Bh
        db  054h, 0C3h, 0FFh, 04Bh, 010h, 0D5h, 05Eh, 000h, 020h, 087h, 01Fh, 01Ch, 01Fh, 05Bh, 05Eh, 002h
        db  000h, 011h, 05Eh, 003h, 000h, 089h, 05Eh, 000h, 003h, 0B8h, 054h, 0FEh, 0FFh, 0B8h, 010h, 074h
        db  054h, 0FDh, 0FFh, 074h, 010h, 09Ch, 070h, 05Bh, 05Eh, 002h, 000h, 09Ch, 070h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 058h, 045h, 03Bh, 040h, 028h, 023h, 029h, 020h, 053h, 04Dh, 043h, 038h
        db  031h, 030h, 030h, 02Eh, 056h, 058h, 044h, 020h, 076h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 020h
        db  031h, 02Eh, 030h, 039h, 061h, 020h, 031h, 031h, 02Fh, 032h, 033h, 02Fh, 039h, 034h, 00Ah, 043h
        db  06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 028h, 043h, 029h, 020h, 053h, 074h, 061h
        db  06Eh, 064h, 061h, 072h, 064h, 020h, 04Dh, 069h, 063h, 072h, 06Fh, 073h, 079h, 073h, 074h, 065h
        db  06Dh, 073h, 020h, 043h, 06Fh, 072h, 070h, 02Ch, 020h, 031h, 039h, 039h, 034h, 000h, 000h, 040h
        db  028h, 023h, 029h, 020h, 055h, 04Dh, 05Fh, 038h, 031h, 031h, 035h, 06Eh, 064h, 069h, 073h, 033h
        db  05Fh, 073h, 032h, 02Eh, 03Fh, 03Fh, 05Fh, 076h, 031h, 02Eh, 030h, 030h, 000h, 000h, 000h
loc_1_FA3C:
        rol    byte ptr [eax], 0xff
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_FA98:
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
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 0x1000201
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    eax, 0x1020101
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
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
        add    al, byte ptr [0x4020101]
        add    dword ptr [ecx], eax
        add    dl, byte ptr [ebx + 0x4d]
        inc    ebx
        cmp    byte ptr [ecx], dh
        xor    byte ptr [eax], dh
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_FB80:
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [ecx], dh
        xor    byte ptr [eax], dh
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_FB94:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
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
        push   ebx
        add    byte ptr [eax], al
        dec    ecx
        push   edx
        push   ecx
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        add    byte ptr [edx + 0x41], dl
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_FBE8:
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
        inc    edx
        jne    loc_1_FC6A
        push   esp
        jns    loc_1_FC6A
        db  065h, 000h
loc_1_FBFC:
        inc    edx
        jne    0xfc72
        push   esp
        jns    0xfc72
        add    byte ptr gs:[ebp + 0x65], cl
        imul   esp, dword ptr fs:[ecx + 0x54], 0x657079
        add    byte ptr [eax], al
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 0x54], 0x657079
        add    byte ptr [eax], al
        dec    ebp
        popal  
        js     0xfc89
        insd   dword ptr es:[edi], dx
        jne    loc_1_FC90
        push   eax
        popal  
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_FC7D
        imul   edi, dword ptr [edx + 0x65], 0x4d000000
        popal  
        js     loc_1_FC9D
        insd   dword ptr es:[edi], dx
        jne    0xfca4
        push   eax
        popal  
        arpl   word ptr [ebx + 0x65], bp
        je     0xfc91
        imul   edi, dword ptr [edx + 0x65], 0x54000000
        push   edx
        dec    ecx
        dec    esi
        dec    ecx
        push   esp
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [eax], al
        push   esp
        push   edx
        dec    ecx
        dec    esi
        dec    ecx
        push   esp
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [eax], al
loc_1_FC5C:
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0xfcd9
        jb     0xfc96
        add    byte ptr [eax], al
loc_1_FC6A:
        add    byte ptr [eax], al
loc_1_FC6C:
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [edi + ebp*2 + 0x6b], dl
        outsb  dx, byte ptr gs:[esi]
        inc    ebx
        popal  
        jb     0xfcdd
        add    byte ptr cs:[eax], al
        inc    eax
loc_1_FC7D:
        sub    byte ptr [ebx], ah
        sub    dword ptr [eax], esp
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [edi + ebp*2 + 0x6b], dl
        outsb  dx, byte ptr gs:[esi]
        inc    ebx
        popal  
        jb     0xfcf2
        inc    ebp
        insb   byte ptr es:[edi], dx
loc_1_FC90:
        imul   esi, dword ptr [ebp + 0x20], 0x696d6146
        insb   byte ptr es:[edi], dx
        jns    0xfcbb
        dec    esp
        dec    ebp
loc_1_FC9D:
        inc    ecx
        inc    ebx
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    0xfd0a
        jb     0xfcc7
        push   esi
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], esi
        arpl   word ptr [eax], sp
        sub    byte ptr [ecx], bh
        xor    esi, dword ptr [eax]
        cmp    dword ptr [edx], esi
        xor    byte ptr [ecx], ch
        add    byte ptr [eax], al
loc_1_FCB8:
        add    byte ptr [edx], al
        add    eax, dword ptr [eax + 6]
        add    al, byte ptr [ebx]
        add    eax, 0xf0b0a07
loc_1_FCC8:
        add    byte ptr [eax], al
loc_1_FCCA:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x570  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [0xfccc]       ; -> obj1:0xFCCC
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   0x32
        push   eax
        call   sub_1_D7E6
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0030
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  090h
loc_2_0030:
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
sub_2_00D0:
        push   ebp
        mov    ebp, esp
        push   0
        push   0x32
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D7EC
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_00F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x2e], 0
        jne    loc_2_0109
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0109:
        mov    eax, dword ptr [0xfccc]       ; -> obj1:0xFCCC
        push   eax
        push   0
        push   0x1f
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_D7E6
        or     eax, eax
        je     loc_2_0129
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0129:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x2e]
        mov    edi, edi
loc_2_0130:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_0143
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_0130
        jmp    loc_2_0152
loc_2_0143:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x2e], eax
loc_2_0152:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_0168
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_0168:
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
sub_2_01D0:
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
        call   sub_2_04F0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_01F9
        cmp    ebx, 0x103
        jne    loc_2_0211
loc_2_01F9:
        push   0
        push   0xfcc8                        ; -> loc_1_FCC8
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_02C0
        or     eax, eax
        je     loc_2_0211
        mov    ebx, eax
loc_2_0211:
        or     ebx, ebx
        je     loc_2_021D
        cmp    ebx, 0x103
        jne    loc_2_0248
loc_2_021D:
        cmp    byte ptr [edi + 0x1d], 0
        je     loc_2_0248
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [edi + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_2_0248
        push   0
        push   0xfcc8                        ; -> loc_1_FCC8
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0360
        or     eax, eax
        je     loc_2_0248
        mov    ebx, eax
loc_2_0248:
        or     ebx, ebx
        je     loc_2_0254
        cmp    ebx, 0x103
        jne    loc_2_02B5
loc_2_0254:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_2_02B0
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_026A
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_026A:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_0279
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_027F
loc_2_0279:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_2_027F:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_2_028E
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_D748
loc_2_028E:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_D7EC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_02B0:
        mov    ebx, 0xc001000e
loc_2_02B5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_02C0:
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
        je     loc_2_0320
loc_2_0313:
        mov    ecx, dword ptr [eax + 0xc]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0313
loc_2_0320:
        mov    ecx, dword ptr [edi + 4]
        cmp    esi, ecx
        je     loc_2_0354
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
        je     loc_2_0356
        cmp    eax, 0x103
        je     loc_2_0356
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0xc], ecx
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0354:
        sub    eax, eax
loc_2_0356:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
sub_2_0360:
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
        mov    al, byte ptr [0xfcc8]         ; -> loc_1_FCC8
        cmp    byte ptr [ecx], al
        jne    loc_2_0408
        mov    al, byte ptr [0xfcc9]         ; -> obj1:0xFCC9
        cmp    byte ptr [ecx + 1], al
        jne    loc_2_0408
        mov    al, byte ptr [0xfcca]         ; -> loc_1_FCCA
        cmp    byte ptr [ecx + 2], al
        jne    loc_2_0408
        mov    al, byte ptr [0xfccb]         ; -> obj1:0xFCCB
        cmp    byte ptr [ecx + 3], al
        jne    loc_2_0408
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_03E9
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [ebx + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_0483
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_03E9:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_03FD
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_03FD:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0408:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    loc_2_0440
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_0424
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0424:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_0483
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
loc_2_0440:
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 1
        jbe    loc_2_0456
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0456:
        mov    ebx, dword ptr [ebp + 0xc]
        jne    loc_2_0470
        cmp    byte ptr [ebx + 0x1d], 0
        jne    loc_2_0470
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
loc_2_0470:
        cmp    eax, 1
        jne    loc_2_0483
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_0483
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        mov    byte ptr [ebx + 0x1d], 0
loc_2_0483:
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
        je     loc_2_04B6
        cmp    eax, 0x103
        je     loc_2_04B6
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0xc], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_04B6:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_04D0
        mov    byte ptr [ebx + 0x1d], 0
        mov    dword ptr [esi + 0xc], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_04D0:
        mov    byte ptr [ebx + 0x1d], 1
        mov    dword ptr [esi + 0xc], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_04F0:
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
        je     loc_2_052D
        lea    esp, [esp]
loc_2_0520:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0x10], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0520
loc_2_052D:
        mov    ecx, dword ptr [esi + 0x10]
        cmp    ebx, ecx
        je     loc_2_0561
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
        je     loc_2_0563
        cmp    eax, 0x103
        je     loc_2_0563
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0x10], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0561:
        sub    eax, eax
loc_2_0563:
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

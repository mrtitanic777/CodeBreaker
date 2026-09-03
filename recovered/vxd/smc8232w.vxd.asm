; SMC8232W -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 7068 instructions recovered by recursive descent

Declare_Virtual_Device SMC8232W, 3, 10, SMC8232W_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x4e28  R-X
        db  055h, 0B8h, 009h, 000h, 000h, 000h, 08Bh, 0ECh, 083h, 0ECh, 01Ch, 066h, 089h, 045h, 0F0h, 053h
        db  066h, 089h, 045h, 0F2h, 033h, 0DBh, 066h, 089h, 05Dh, 0E6h, 053h, 08Bh, 04Dh, 00Ch, 08Dh, 045h
        db  0E4h, 051h, 08Dh, 055h, 0F8h, 0C7h, 045h, 0F4h
        dd  loc_1_4C88               ; fixup
        db  0C7h, 045h, 0E8h, 04Dh, 0A3h, 080h, 010h, 066h, 0C7h, 045h, 0E4h, 001h, 000h, 050h, 052h, 0E8h
        db  012h, 045h, 000h, 000h, 08Dh, 04Dh, 0E4h, 089h, 00Dh
        dd  obj1:0x4B9E              ; fixup
        db  08Bh, 055h, 0F8h, 089h, 015h
        dd  obj1:0x4B58              ; fixup
        db  088h, 01Dh
        dd  obj1:0x4BA2              ; fixup
        db  089h, 01Dh
        dd  obj1:0x4B92              ; fixup
        db  089h, 01Dh
        dd  obj1:0x4B96              ; fixup
        db  0C6h, 005h
        dd  obj1:0x4B5C              ; fixup
        db  003h, 088h, 01Dh
        dd  obj1:0x4B5D              ; fixup
        db  089h, 01Dh
        dd  obj1:0x4B5E              ; fixup
        db  0C7h, 005h
        dd  obj1:0x4B62              ; fixup
        dd  loc_1_08A0               ; fixup
        db  0C7h, 005h
        dd  obj1:0x4B66              ; fixup
        dd  loc_1_0AC0               ; fixup
        db  0C7h, 005h
        dd  obj1:0x4B6A              ; fixup
        dd  loc_1_1620               ; fixup
        db  0C7h, 005h
        dd  obj1:0x4B6E              ; fixup
        dd  loc_1_21A0               ; fixup
        db  0C7h, 005h
        dd  obj1:0x4B72              ; fixup
        dd  loc_1_1910               ; fixup
        db  08Bh, 045h, 0F0h, 08Bh, 04Dh, 0F4h, 0C7h, 005h
        dd  obj1:0x4B76              ; fixup
        dd  loc_1_0C10               ; fixup
        db  0C7h, 005h
        dd  obj1:0x4B7A              ; fixup
        dd  loc_1_1470               ; fixup
        db  0C7h, 005h
        dd  obj1:0x4B7E              ; fixup
        dd  loc_1_15F0               ; fixup
        db  0C7h, 005h
        dd  obj1:0x4B82              ; fixup
        dd  loc_1_0130               ; fixup
        db  0C7h, 005h
        dd  obj1:0x4B86              ; fixup
        dd  loc_1_1530               ; fixup
        db  06Ah, 036h, 0A3h
        dd  obj1:0x4B8A              ; fixup
        db  068h
        dd  obj1:0x4B5C              ; fixup
        db  08Dh, 045h, 0FCh, 068h
        dd  obj1:0x4B54              ; fixup
        db  089h, 00Dh
        dd  obj1:0x4B8E              ; fixup
        db  08Bh, 055h, 0F8h, 052h, 068h
        dd  obj1:0x4B54              ; fixup
        db  050h, 0E8h, 054h, 044h, 000h, 000h, 039h, 05Dh, 0FCh, 074h, 014h, 053h, 08Bh, 045h, 0F8h, 050h
        db  0E8h, 03Fh, 044h, 000h, 000h, 08Bh, 045h, 0FCh, 05Bh, 08Bh, 0E5h, 05Dh, 0C2h, 008h, 000h, 08Bh
        db  045h, 0FCh, 05Bh, 08Bh, 0E5h, 05Dh, 0C2h, 008h, 000h, 08Dh, 049h, 000h
loc_1_0130:
        push   ebp
        mov    eax, 0xd
        mov    ebp, esp
        sub    esp, 0x1b4
        mov    word ptr [ebp - 0x4c], ax
        push   ebx
        mov    word ptr [ebp - 0x4a], ax
        push   esi
        push   edi
        mov    eax, 4
        mov    dword ptr [ebp - 0x48], 0     ; -> loc_1_4CB4
        mov    dword ptr [ebp - 0x38], 0     ; -> obj1:0x4CE0
        mov    dword ptr [ebp - 0x30], 0     ; -> loc_1_4CF0
        mov    word ptr [ebp - 0x3c], ax
        mov    word ptr [ebp - 0x3a], ax
        mov    eax, 0xb
loc_1_0170:
        mov    word ptr [ebp - 0x34], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 0x32], ax
        mov    byte ptr [ebp - 2], bl
        mov    eax, 8
        mov    byte ptr [ebp - 1], bl
        mov    word ptr [ebp - 0x44], ax
        mov    dword ptr [ebp - 0x18], ebx
        mov    word ptr [ebp - 0x42], ax
        mov    byte ptr [ebp - 0x2c], bl
        lea    eax, [ebp - 0x2b]
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        mov    dword ptr [eax], ebx
        mov    dword ptr [ebp - 0x40], 0     ; -> obj1:0x4D1C
        mov    dword ptr [ebp - 0x1c], 0x10
        mov    byte ptr [eax + 4], bl
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_46BC
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_1_01CD
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_01CD:
        push   0
        lea    eax, [ebp - 0x4c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_46C2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_01F3
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0x1c], ecx
loc_1_01F3:
        push   1
        lea    eax, [ebp - 0x44]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_46C2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_022E
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 3
        jne    loc_1_0221
        mov    byte ptr [ebp - 1], 1
        jmp    loc_1_022E
loc_1_0221:
        sub    eax, 2
        cmp    eax, 1
        sbb    al, al
        and    al, 2
        mov    byte ptr [ebp - 1], al
loc_1_022E:
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 0x24]
        push   eax
        lea    edx, [ebp - 0x20]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_45C4
        cmp    dword ptr [ebp - 0x24], 6
        jne    loc_1_0265
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_0265
        mov    ecx, dword ptr [ebp - 0x20]
        mov    eax, ecx
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 0x2c], edx
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 0x28], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 0x27], cl
loc_1_0265:
        lea    eax, [ebp - 0x1b4]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0xc]
        push   ecx
        lea    eax, [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_45BE
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_028C
        mov    byte ptr [ebp - 2], 1
        mov    ebx, 0xaa000004
loc_1_028C:
        mov    esi, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [0]            ; -> obj1:0x4B50
        shl    si, 0xc
        push   eax
        or     si, 0xc80
        push   0
        push   0x332
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_463E
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_1_02BE
        mov    byte ptr [ebp - 2], 1
        mov    ebx, 0xaa000002
loc_1_02BE:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0xcc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    edi, [ebp - 0x74]
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    byte ptr [ebp - 0x70], 1
        mov    ecx, esi
        mov    dword ptr [ebp - 0x74], eax
        mov    eax, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x52], ecx
        mov    dword ptr [ebp - 0x6a], 2
        shl    eax, 0xc
        lea    edx, [ebp - 0x74]
        push   edx
loc_1_0300:
        mov    dword ptr [ebp - 0x5a], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x4B54
        mov    dword ptr [ebp - 0x56], 0x40
        mov    dword ptr [ebp - 0x66], 0x64
        mov    dword ptr [ebp - 0x62], 0x5ea
        push   edx
        push   eax
        lea    eax, [edx + 0x4e]
        push   eax
        call   sub_1_456A
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_1_0361
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_46C8
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x332
        push   eax
        call   sub_1_4644
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0361:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_03AC
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 0x4e]
        push   ecx
        call   sub_1_4710
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_46C8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 0x4e]
        push   edx
        call   sub_1_4570
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x332
        push   ecx
        call   sub_1_4644
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_03AC:
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 1]
        mov    word ptr [eax + 4], si
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [ebp - 8]
        mov    dl, byte ptr [ebp - 0x18]
        mov    byte ptr [eax + 1], dl
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x52], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0xc3], 0x4000
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2A80
        cmp    ax, 0xffff
        jne    loc_1_042F
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000004
        mov    ecx, dword ptr [eax + 0x4e]
        push   ecx
        call   sub_1_4710
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_46C8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 0x4e]
        push   edx
        call   sub_1_4570
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x332
        push   ecx
        call   sub_1_4644
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_042F:
        cmp    ax, 2
        jne    loc_1_0488
        push   0
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_46C2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_045F
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 0xa], dx
loc_1_045F:
        push   1
        lea    eax, [ebp - 0x34]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_46C2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_0488
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x18], edx
loc_1_0488:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_46C8
        lea    eax, [ebp - 0x2c]
        mov    ecx, dword ptr [ebp - 0x1c]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   2
        push   edx
        call   sub_1_04E0
        test   eax, eax
        je     loc_1_04D2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0x4e]
        push   ecx
        call   sub_1_4570
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x332
        push   ecx
        call   sub_1_4644
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_04D2:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_04E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        cmp    edi, 2
        mov    dword ptr [esi + 0x17a], ebx
        jbe    loc_1_0523
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000002
        push   eax
        call   sub_1_4710
        mov    dword ptr [ebp - 4], 0xc000009a
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0523:
        xor    edx, edx
        mov    eax, 0                        ; -> sub_1_1B70
        mov    dword ptr [esi + 0x172], edx
        mov    dword ptr [esi + 0x21e], eax
        mov    dword ptr [esi + 0x212], edx
        mov    dword ptr [esi + 0x216], eax
        mov    dword ptr [esi + 0x16a], 0    ; -> obj1:0x4B54
        mov    dword ptr [esi + 0x21a], esi
        mov    ecx, dword ptr [0x42]         ; -> obj1:0x4B96
        mov    dword ptr [esi + 0x16e], ecx
        mov    dword ptr [0x42], esi         ; -> obj1:0x4B96
        mov    al, byte ptr [esi + 0xa]
        mov    cl, byte ptr [esi]
        test   cl, cl
        je     loc_1_0577
        cmp    cl, 2
        mov    ecx, 2
        jne    loc_1_057C
loc_1_0577:
        mov    ecx, 1
loc_1_057C:
        movsx  eax, al
        push   ecx
        push   0
        push   eax
        mov    ecx, dword ptr [esi + 0x4e]
        push   eax
        lea    edx, [esi + 0xcb]
        push   0                             ; -> loc_1_1A10
        lea    eax, [ebp - 4]
        push   esi
        push   0                             ; -> loc_1_19F0
        push   ecx
        push   edx
        push   eax
        call   sub_1_4626
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_05C8
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000005
        push   eax
        call   sub_1_4710
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_05C8:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x18]
        mov    ax, word ptr [esi + 0x20]
        mov    edx, dword ptr [esi + 0x4e]
        shl    eax, 0xa
        push   eax
        push   ecx
        push   edx
        lea    eax, [esi + 0x1c]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_470A
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0605
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000001
        push   eax
        call   sub_1_4710
        add    esp, 0xc
        jmp    loc_1_07F5
loc_1_0605:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 0x14]
loc_1_060A:
        mov    al, byte ptr [ecx + edx]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [esi + edx + 0x23], al
        jb     loc_1_060A
        mov    word ptr [esi + 0x3a], di
        lea    eax, [esi + 0x1cb]
        lea    ecx, [esi + 0x1d3]
        lea    edx, [esi + 0x1cf]
        mov    dword ptr [esi + 0x5f], eax
        mov    dword ptr [esi + 0x8f], ecx
        lea    eax, [esi + 0x1c7]
        lea    ecx, [esi + 0x1d7]
        mov    dword ptr [esi + 0x63], edx
        mov    dword ptr [esi + 0x93], eax
        lea    edx, [esi + 0x1db]
        lea    eax, [esi + 0x1df]
        mov    dword ptr [esi + 0x97], ecx
        mov    dword ptr [esi + 0x67], edx
loc_1_0660:
        lea    ecx, [esi + 0x1e3]
        lea    edx, [esi + 0x1e7]
        mov    dword ptr [esi + 0x6b], eax
        mov    dword ptr [esi + 0x6f], ecx
        lea    eax, [esi + 0x1eb]
        lea    ecx, [esi + 0x1ef]
        mov    dword ptr [esi + 0x73], edx
        mov    dword ptr [esi + 0x77], eax
        lea    edx, [esi + 0x1f3]
        lea    eax, [esi + 0x1f7]
        mov    dword ptr [esi + 0x7b], ecx
        mov    dword ptr [esi + 0x7f], edx
        lea    ecx, [esi + 0x1fb]
        lea    edx, [esi + 0x1ff]
        mov    dword ptr [esi + 0x83], eax
        mov    dword ptr [esi + 0x87], ecx
        lea    eax, [esi + 0x1b7]
        lea    ecx, [esi + 0x1ab]
        mov    dword ptr [esi + 0x8b], edx
        mov    dword ptr [esi + 0x9b], eax
        lea    edx, [esi + 0x1af]
        lea    eax, [esi + 0x1b3]
        mov    dword ptr [esi + 0x9f], ecx
        mov    dword ptr [esi + 0xa3], edx
        lea    ecx, [esi + 0x1bb]
        lea    edx, [esi + 0x1bf]
        mov    dword ptr [esi + 0xa7], eax
        mov    dword ptr [esi + 0xab], ecx
        lea    eax, [esi + 0x1c3]
        mov    dword ptr [esi + 0xaf], edx
        lea    edi, [esi + 0x162]
        mov    dword ptr [esi + 0xb7], eax
        mov    dword ptr [esi + 0x15e], 0
        mov    word ptr [esi + 0x36], 0x5ea
        mov    word ptr [esi + 0x38], 0x100
        mov    ecx, dword ptr [0]            ; -> obj1:0x4B50
        push   ecx
        push   2
        push   0x300
        push   edi
        call   sub_1_463E
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_1_0752
        push   2
        mov    eax, dword ptr [edi]
        or     byte ptr [esi + 0x43], 4
        push   0x300
        push   eax
        call   sub_1_4644
loc_1_0752:
        and    word ptr [esi + 0x42], 0xfbff
        push   esi
        call   sub_1_2BD0
        test   ax, ax
        je     loc_1_079B
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        mov    byte ptr [esi + 0x203], 1
        push   0xaa000003
        push   eax
        call   sub_1_4710
        add    esp, 0xc
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0792
        push   2
        mov    eax, dword ptr [edi]
        push   0x300
        push   eax
        call   sub_1_4644
loc_1_0792:
        mov    dword ptr [ebp - 4], 0xc0010006
        jmp    loc_1_07F5
loc_1_079B:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_07B0
        push   2
        mov    eax, dword ptr [edi]
        push   0x300
        push   eax
        call   sub_1_4644
loc_1_07B0:
        lea    eax, [esi + 0x15e]
        lea    ecx, [esi + 0x166]
        push   eax
        lea    edx, [esi + 0x24]
loc_1_07C0:
        push   ecx
        push   edx
        push   0                             ; -> loc_1_19E0
        push   0                             ; -> loc_1_19B0
        push   0                             ; -> loc_1_19A0
        push   ebx
        call   sub_2_0000
        test   al, al
        jne    loc_1_085A
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000002
        push   eax
        call   sub_1_4710
        mov    dword ptr [ebp - 4], 0xc0000001
        add    esp, 0xc
loc_1_07F5:
        lea    eax, [esi + 0xcb]
        push   eax
        call   sub_1_462C
        cmp    esi, dword ptr [0x42]         ; -> obj1:0x4B96
        jne    loc_1_0820
        mov    eax, dword ptr [esi + 0x16e]
        pop    edi
        pop    esi
        mov    dword ptr [0x42], eax         ; -> obj1:0x4B96
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0820:
        mov    ecx, dword ptr [0x42]         ; -> obj1:0x4B96
        cmp    dword ptr [ecx + 0x16e], esi
        je     loc_1_083C
loc_1_082E:
        mov    ecx, dword ptr [ecx + 0x16e]
        cmp    dword ptr [ecx + 0x16e], esi
        jne    loc_1_082E
loc_1_083C:
        mov    eax, dword ptr [ecx + 0x16e]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x16e]
        mov    dword ptr [ecx + 0x16e], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_085A:
        mov    ecx, 0                        ; -> loc_1_22E0
        lea    eax, [esi + 0x325]
        push   0x1388
        mov    dword ptr [esi + 0x321], ecx
        push   eax
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0x329], ecx
        mov    dword ptr [esi + 0x32d], esi
        call   sub_1_461A
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0xffffffff
        lea    eax, [eax + edx*4]
loc_1_08C4:
        test   edx, edx
        je     loc_1_08D3
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_1_08C4
        mov    dword ptr [ecx], edx
loc_1_08D3:
        cmp    dword ptr [ecx], -1
        jne    loc_1_08E5
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_08E5:
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 0x203], 0
        je     loc_1_08FE
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_08FE:
        mov    eax, dword ptr [0]            ; -> obj1:0x4B50
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x75
        push   ecx
        call   sub_1_463E
        test   eax, eax
        je     loc_1_0935
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000002
        push   eax
        call   sub_1_4710
        add    esp, 0xc
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0935:
        inc    dword ptr [esi + 0x176]
        cmp    dword ptr [esi + 0x172], 0
        jne    loc_1_096F
        push   esi
        call   sub_1_2F10
        test   ax, ax
        je     loc_1_096F
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000003
        push   eax
        call   sub_1_4710
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_096F:
        mov    eax, dword ptr [esi + 0x172]
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x172], ecx
        mov    ecx, dword ptr [esi + 0x15e]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x14
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_0170
        test   al, al
        jne    loc_1_09D5
        push   0
        dec    dword ptr [esi + 0x176]
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x172], ecx
        mov    edx, dword ptr [esi + 0x4e]
        push   edx
        call   sub_1_4710
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_09D5:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x16a]
        mov    dword ptr [eax + 0x10], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 0xee
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        mov    dword ptr [esi + 0x20e], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [esi + 0x176]
        dec    eax
        mov    dword ptr [esi + 0x176], eax
        jne    loc_1_0A67
        cmp    byte ptr [esi + 0x204], 0
        je     loc_1_0A42
        lea    eax, [esi + 0x31e]
        cmp    byte ptr [eax], 0
        je     loc_1_0A56
loc_1_0A42:
        cmp    dword ptr [esi + 0x18e], 0
        je     loc_1_0A67
        lea    eax, [esi + 0x31e]
        cmp    byte ptr [eax], 0
        jne    loc_1_0A67
loc_1_0A56:
        push   0
        add    esi, 0x212
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_0A67:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A80:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x172]
        test   edx, edx
        je     loc_1_0AA3
loc_1_0A93:
        mov    esi, dword ptr [edx + 0x18]
        cmp    ecx, esi
        jae    loc_1_0A9C
        mov    ecx, esi
loc_1_0A9C:
        mov    edx, dword ptr [edx + 0xc]
        test   edx, edx
        jne    loc_1_0A93
loc_1_0AA3:
        test   ecx, ecx
        jne    loc_1_0AAC
        mov    ecx, 0xee
loc_1_0AAC:
        push   eax
        mov    dword ptr [eax + 0x20e], ecx
        call   sub_1_2BA0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0AC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x20], 0
        mov    esi, dword ptr [ebx + 8]
        je     loc_1_0ADE
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0ADE:
        push   0
        inc    dword ptr [esi + 0x176]
        inc    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [ebx + 0x14]
        mov    ecx, dword ptr [esi + 0x15e]
        push   eax
        push   ecx
        call   sub_2_0240
        mov    edi, eax
        test   edi, edi
        jne    loc_1_0B1E
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    eax, 2
        je     loc_1_0B16
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        mov    edi, 0x103
        jmp    loc_1_0B44
loc_1_0B16:
        sub    eax, 2
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_1_0B44
loc_1_0B1E:
        cmp    edi, 0x103
        jne    loc_1_0B30
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        jmp    loc_1_0B44
loc_1_0B30:
        cmp    edi, 0xc001000e
        jne    loc_1_0B41
        mov    byte ptr [ebx + 0x20], 1
        mov    edi, 0x103
loc_1_0B41:
        dec    dword ptr [ebx + 0x1c]
loc_1_0B44:
        cmp    dword ptr [ebx + 0x1c], 0
        jne    loc_1_0BB7
        mov    eax, dword ptr [esi + 0x172]
        cmp    ebx, eax
        jne    loc_1_0B5F
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x172], eax
        jmp    loc_1_0B72
loc_1_0B5F:
        cmp    dword ptr [eax + 0xc], ebx
        je     loc_1_0B6C
loc_1_0B64:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], ebx
        jne    loc_1_0B64
loc_1_0B6C:
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0B72:
        mov    eax, dword ptr [ebx + 0x18]
        cmp    dword ptr [esi + 0x20e], eax
        jne    loc_1_0B83
        push   esi
        call   sub_1_0A80
loc_1_0B83:
        cmp    dword ptr [esi + 0x172], 0
        jne    loc_1_0BBC
        push   esi
        lea    eax, [esi + 0xcb]
        push   0                             ; -> loc_1_22C0
        push   eax
        call   sub_1_4632
        test   al, al
        jne    loc_1_0BBC
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000003
        push   eax
        call   sub_1_4710
        add    esp, 0xc
        jmp    loc_1_0BBC
loc_1_0BB7:
        mov    edi, 0x103
loc_1_0BBC:
        mov    eax, dword ptr [esi + 0x176]
        dec    eax
        mov    dword ptr [esi + 0x176], eax
        jne    loc_1_0C04
        cmp    byte ptr [esi + 0x204], 0
        je     loc_1_0BDF
        lea    eax, [esi + 0x31e]
        cmp    byte ptr [eax], 0
        je     loc_1_0BF3
loc_1_0BDF:
        cmp    dword ptr [esi + 0x18e], 0
        je     loc_1_0C04
        lea    eax, [esi + 0x31e]
        cmp    byte ptr [eax], 0
        jne    loc_1_0C04
loc_1_0BF3:
        push   0
        add    esi, 0x212
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_0C04:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [edi + 0x1c]
        inc    dword ptr [esi + 0x176]
        cmp    byte ptr [edi + 0x20], 0
        je     loc_1_0C37
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_1_0C79
loc_1_0C37:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        jne    loc_1_0C4E
        push   eax
        push   edi
        push   esi
        call   sub_1_10E0
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0C79
loc_1_0C4E:
        cmp    ecx, 1
        jne    loc_1_0C72
        cmp    byte ptr [esi + 0x203], 0
        je     loc_1_0C65
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_0C79
loc_1_0C65:
        push   eax
        push   edi
        push   esi
        call   sub_1_1130
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0C79
loc_1_0C72:
        mov    dword ptr [ebp - 4], 0x10001
loc_1_0C79:
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_0CE9
        mov    eax, dword ptr [ebx + 0x172]
        cmp    eax, edi
        jne    loc_1_0C96
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x172], eax
        jmp    loc_1_0CA9
loc_1_0C96:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_0CA3
loc_1_0C9B:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_0C9B
loc_1_0CA3:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0CA9:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x20e], eax
        jne    loc_1_0CBA
        push   ebx
        call   sub_1_0A80
loc_1_0CBA:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   0x75
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 0x172], 0
        jne    loc_1_0CE9
        push   ebx
        push   0                             ; -> loc_1_22C0
        add    ebx, 0xcb
        push   ebx
        call   sub_1_4632
loc_1_0CE9:
        mov    eax, dword ptr [esi + 0x176]
        dec    eax
        mov    dword ptr [esi + 0x176], eax
        jne    loc_1_0D31
        cmp    byte ptr [esi + 0x204], 0
        je     loc_1_0D0C
        lea    eax, [esi + 0x31e]
        cmp    byte ptr [eax], 0
        je     loc_1_0D20
loc_1_0D0C:
        cmp    dword ptr [esi + 0x18e], 0
        je     loc_1_0D31
        lea    eax, [esi + 0x31e]
        cmp    byte ptr [eax], 0
        jne    loc_1_0D31
loc_1_0D20:
        push   0
        add    esi, 0x212
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_0D31:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_0D40:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        mov    dword ptr [ebp - 0x18], eax
        push   esi
        mov    dword ptr [ebp - 0xc], eax
        push   edi
        lea    esi, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x1010101
        ja     loc_1_0D7D
        je     loc_1_0F34
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_0D9E
        jmp    dword ptr [eax*4]             ; -> obj1:0x1090
loc_1_0D7D:
        cmp    eax, 0x1010102
        je     loc_1_0F53
        cmp    eax, 0x1010103
        je     loc_1_0F72
        cmp    eax, 0x1010104
        je     loc_1_1037
loc_1_0D9E:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_1043
loc_1_0DAA:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_0DBF
        mov    esi, 0                        ; -> loc_1_4C20
        mov    edi, 0x5c
        jmp    loc_1_1043
loc_1_0DBF:
        mov    esi, 0                        ; -> loc_1_4BA4
        mov    edi, 0x7c
        jmp    loc_1_1043
loc_1_0DCE:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x14], 4
        cmp    byte ptr [ebx + 0x203], 0
        jne    loc_1_0DE8
        mov    dword ptr [ebp - 0x14], 0
loc_1_0DE8:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_1_1043
loc_1_0DF5:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_1043
loc_1_0E02:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_1_1043
loc_1_0E0E:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_1043
loc_1_0E1A:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_1043
loc_1_0E26:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0x3a]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xc3]
        imul   ecx, eax
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_1043
loc_1_0E43:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0x16]
        xor    edx, edx
        mov    dx, word ptr [ebx + 0x3a]
        xor    eax, eax
        shl    ecx, 0xa
        mov    ax, word ptr [ebx + 0xc3]
        imul   edx, eax
        sub    ecx, edx
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_1043
loc_1_0E6B:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x38]
        jmp    loc_1_1040
loc_1_0E79:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x38]
        jmp    loc_1_1040
loc_1_0E87:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x2a
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        call   sub_1_4A20
        and    dword ptr [ebp - 8], 0xffffff00
        jmp    loc_1_1043
loc_1_0EAD:
        mov    esi, 0                        ; -> obj1:0x4D64
        mov    edi, 0xd
        jmp    loc_1_1043
loc_1_0EBC:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_0ED6
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x15e]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_1043
loc_1_0ED6:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x14]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_1043
loc_1_0EE7:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_0EFB
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x20e]
        jmp    loc_1_1040
loc_1_0EFB:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_1043
loc_1_0F09:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_1043
loc_1_0F1C:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_1043
loc_1_0F28:
        mov    dword ptr [ebp - 8], 0xe
        jmp    loc_1_1043
loc_1_0F34:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x2a
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_4A20
        jmp    loc_1_1043
loc_1_0F53:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x24
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_4A20
        jmp    loc_1_1043
loc_1_0F72:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_0FD2
        mov    ecx, dword ptr [ebx + 0x15e]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    edx, dword ptr [ebp + 0x1c]
        jbe    loc_1_0FA7
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], edx
        jmp    loc_1_1043
loc_1_0FA7:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x10]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0xc]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_0760
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ecx], eax
        je     loc_1_1049
        jmp    loc_1_1043
loc_1_0FD2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebx + 0x15e]
        mov    ecx, dword ptr [eax + 0x14]
        push   ecx
        push   edx
        call   sub_2_0660
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 0x10], eax
        lea    ecx, [edx*2]
        cmp    ecx, dword ptr [ebp + 0x1c]
        jbe    loc_1_1005
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], ecx
        jmp    loc_1_1043
loc_1_1005:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [eax + 0x14]
        push   ecx
        mov    edx, dword ptr [ebx + 0x15e]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_06A0
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_1043
loc_1_1037:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x17a]
loc_1_1040:
        mov    dword ptr [ebp - 8], eax
loc_1_1043:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1082
loc_1_1049:
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_1082
        cmp    edi, dword ptr [ebp + 0x1c]
        jbe    loc_1_1072
        sub    edi, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_1072:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   esi
        push   eax
        call   sub_1_4A20
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_1082:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Bh, 0FFh
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_0DAA
        dd  loc_1_0DCE
        dd  loc_1_0DF5
        dd  loc_1_0DF5
        dd  loc_1_0E02
        dd  loc_1_0E0E
        dd  loc_1_0E1A
        dd  loc_1_0E26
        dd  loc_1_0E43
        dd  loc_1_0E6B
        dd  loc_1_0E79
        dd  loc_1_0E87
        dd  loc_1_0EAD
        dd  loc_1_0EBC
        dd  loc_1_0EE7
        dd  loc_1_0F09
        dd  loc_1_0F1C
        dd  loc_1_0D9E
        dd  loc_1_0F28
        db  08Dh, 064h, 024h, 000h
sub_1_10E0:
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
        call   sub_1_0D40
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1130:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010103
        ja     loc_1_116D
        je     loc_1_127C
        cmp    eax, 0x1010e
        je     loc_1_117F
        cmp    eax, 0x1010f
        je     loc_1_11DA
        cmp    eax, 0x10112
        je     loc_1_1255
loc_1_116D:
        mov    ebx, 0xc00000bb
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_12B7
loc_1_117F:
        cmp    edi, 4
        je     loc_1_1196
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_12B7
loc_1_1196:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_4A20
        test   dword ptr [ebp - 8], 0xf050
        je     loc_1_11C3
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        mov    ebx, 0xc00000bb
        jmp    loc_1_12B7
loc_1_11C3:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_12D0
        jmp    loc_1_12B5
loc_1_11DA:
        cmp    edi, 4
        je     loc_1_11F1
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_12B7
loc_1_11F1:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_4A20
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_1_124E
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x20e]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_1228
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x20e], eax
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_12B7
loc_1_1228:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    eax, dword ptr [ecx + 0x18]
        jne    loc_1_1246
        mov    eax, dword ptr [ecx + 0x18]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_1246
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x18], eax
        call   sub_1_0A80
        jmp    loc_1_12B7
loc_1_1246:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_12B7
loc_1_124E:
        mov    ebx, 0xc0010014
        jmp    loc_1_12B7
loc_1_1255:
        cmp    edi, 4
        je     loc_1_1269
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_12B7
loc_1_1269:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x71
        push   ecx
        call   sub_1_4A20
        jmp    loc_1_12B7
loc_1_127C:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_1_129A
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_12B7
loc_1_129A:
        push   ecx
        mov    eax, edi
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edx
        call   sub_1_1310
loc_1_12B5:
        mov    ebx, eax
loc_1_12B7:
        test   ebx, ebx
        jne    loc_1_12C5
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_1_12C5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_12D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_1305
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [esi + 0x14]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [eax + 0x15e]
        push   ecx
        call   sub_2_05E0
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
loc_1_1305:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
sub_1_1310:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_1349
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x1c]
        mov    edx, dword ptr [ecx + 0x15e]
        push   edx
        call   sub_2_0300
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x14
loc_1_1349:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1360:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 0x10]
        mov    edi, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esi + 0x18]
        push   ecx
        mov    dword ptr [ebp - 0x14], eax
        push   edx
        mov    ecx, dword ptr [esi + 0x14]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        push   1
        push   ecx
        push   0
        push   ebx
        call   sub_1_0D40
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xc00000bb
        jne    loc_1_1440
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_1_13D0
        je     loc_1_140F
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_13DE
        jmp    dword ptr [eax*4]             ; -> obj1:0x1458
loc_1_13D0:
        cmp    eax, 0x1020102
        je     loc_1_1417
        cmp    eax, 0x1020103
        je     loc_1_141F
loc_1_13DE:
        mov    dword ptr [ebp - 8], 0xc00000bb
        jmp    loc_1_1428
loc_1_13E7:
        mov    eax, dword ptr [ebx + 0x1a3]
        jmp    loc_1_1425
loc_1_13EF:
        mov    eax, dword ptr [ebx + 0x1a7]
        jmp    loc_1_1425
loc_1_13F7:
        mov    eax, dword ptr [ebx + 0x1ab]
        jmp    loc_1_1425
loc_1_13FF:
        mov    eax, dword ptr [ebx + 0x1cb]
        jmp    loc_1_1425
loc_1_1407:
        mov    eax, dword ptr [ebx + 0x1cf]
        jmp    loc_1_1425
loc_1_140F:
        mov    eax, dword ptr [ebx + 0x1c7]
        jmp    loc_1_1425
loc_1_1417:
        mov    eax, dword ptr [ebx + 0x1af]
        jmp    loc_1_1425
loc_1_141F:
        mov    eax, dword ptr [ebx + 0x1b3]
loc_1_1425:
        mov    dword ptr [ebp - 4], eax
loc_1_1428:
        cmp    edi, 4
        jb     loc_1_1440
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_4A20
        add    dword ptr [ebp - 0xc], 4
loc_1_1440:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        pop    edi
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x24], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_13E7
        dd  loc_1_13EF
        dd  loc_1_13F7
        dd  loc_1_13FF
        dd  loc_1_1407
        db  08Dh, 064h, 024h, 000h
loc_1_1470:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x10113
        ja     loc_1_1497
        je     loc_1_14C8
        cmp    eax, 0x10101
        jb     loc_1_14C3
        cmp    eax, 0x10111
        jbe    loc_1_14C8
        jmp    loc_1_14C3
loc_1_1497:
        cmp    eax, 0x1010104
        ja     loc_1_14B5
        cmp    eax, 0x1010101
        jae    loc_1_14C8
        cmp    eax, 0x20101
        jb     loc_1_14C3
        cmp    eax, 0x20105
        jbe    loc_1_14C8
        jmp    loc_1_14C3
loc_1_14B5:
        cmp    eax, 0x1020101
        jb     loc_1_14C3
        cmp    eax, 0x1020103
        jbe    loc_1_14C8
loc_1_14C3:
        mov    esi, 0xc00000bb
loc_1_14C8:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x176]
        test   esi, esi
        jne    loc_1_14DE
        push   ecx
        push   edi
        call   sub_1_1360
        mov    esi, eax
loc_1_14DE:
        mov    eax, dword ptr [edi + 0x176]
        dec    eax
        mov    dword ptr [edi + 0x176], eax
        jne    loc_1_1526
        cmp    byte ptr [edi + 0x204], 0
        je     loc_1_1501
        lea    eax, [edi + 0x31e]
        cmp    byte ptr [eax], 0
        je     loc_1_1515
loc_1_1501:
        cmp    dword ptr [edi + 0x18e], 0
        je     loc_1_1526
        lea    eax, [edi + 0x31e]
        cmp    byte ptr [eax], 0
        jne    loc_1_1526
loc_1_1515:
        push   0
        add    edi, 0x212
        push   edi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_1526:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_1530:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_2B90
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x325]
        push   eax
        mov    edi, 0x300
        push   ecx
        call   sub_1_4620
loc_1_1556:
        push   0x2710
        call   sub_1_4638
        dec    edi
        jne    loc_1_1556
        mov    byte ptr [esi + 0x320], 1
        cmp    esi, dword ptr [0x42]         ; -> obj1:0x4B96
        jne    loc_1_157F
        mov    eax, dword ptr [esi + 0x16e]
        mov    dword ptr [0x42], eax         ; -> obj1:0x4B96
        jmp    loc_1_15AD
loc_1_157F:
        mov    ecx, dword ptr [0x42]         ; -> obj1:0x4B96
        cmp    dword ptr [ecx + 0x16e], esi
        je     loc_1_159B
loc_1_158D:
        mov    ecx, dword ptr [ecx + 0x16e]
        cmp    dword ptr [ecx + 0x16e], esi
        jne    loc_1_158D
loc_1_159B:
        mov    eax, dword ptr [ecx + 0x16e]
        mov    edx, dword ptr [eax + 0x16e]
        mov    dword ptr [ecx + 0x16e], edx
loc_1_15AD:
        lea    eax, [esi + 0xcb]
        push   eax
        call   sub_1_462C
        mov    eax, dword ptr [esi + 0x15e]
        push   eax
        call   sub_2_0120
        mov    eax, dword ptr [esi + 0x4e]
        push   eax
        call   sub_1_4570
        push   0
        push   0x332
        push   esi
        call   sub_1_4644
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_15F0:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x4B54
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_4564
        push   0
        mov    ecx, dword ptr [4]            ; -> obj1:0x4B58
        push   ecx
        call   sub_1_4558
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1620:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        lea    ecx, [ebp - 8]
        mov    esi, dword ptr [ebx + 8]
        push   ecx
        lea    eax, [edi + 0x24]
        push   0
        push   0
        mov    dword ptr [ebp - 4], eax
        push   0
        push   edi
        call   sub_1_4668
        cmp    dword ptr [ebp - 8], 0xc
        jb     loc_1_17D6
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_1_17D6
        cmp    byte ptr [esi + 0x203], 0
        je     loc_1_1676
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1676:
        cmp    byte ptr [esi + 0x205], 0
        je     loc_1_168D
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_168D:
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_16A1
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_16A1:
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   ebx
        inc    dword ptr [ebx + 0x1c]
        push   esi
        inc    dword ptr [esi + 0x176]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 0
        call   sub_1_1830
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 9], 0
        je     loc_1_16F5
        cmp    dword ptr [esi + 0x18e], 0
        jne    loc_1_16E1
        mov    dword ptr [esi + 0x192], edi
        mov    dword ptr [esi + 0x18e], edi
        jmp    loc_1_177E
loc_1_16E1:
        mov    eax, dword ptr [esi + 0x192]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x192], edi
        jmp    loc_1_177E
loc_1_16F5:
        cmp    dword ptr [esi + 0x17e], 0
        je     loc_1_170F
        mov    eax, dword ptr [esi + 0x182]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x182], edi
        jmp    loc_1_177E
loc_1_170F:
        cmp    dword ptr [esi + 0x186], 0
        jne    loc_1_1728
        xor    ebx, ebx
        mov    dword ptr [esi + 0x18a], edi
        mov    dword ptr [esi + 0x186], edi
        jmp    loc_1_1737
loc_1_1728:
        mov    ebx, dword ptr [esi + 0x18a]
        mov    dword ptr [ebx + 0x24], edi
        mov    dword ptr [esi + 0x18a], edi
loc_1_1737:
        push   esi
        push   edi
        call   sub_1_3410
        cmp    ax, 6
        jne    loc_1_177E
        test   ebx, ebx
        jne    loc_1_175E
        mov    dword ptr [esi + 0x18a], 0
        mov    dword ptr [esi + 0x186], 0
        jmp    loc_1_176B
loc_1_175E:
        mov    dword ptr [esi + 0x18a], ebx
        mov    dword ptr [ebx + 0x24], 0
loc_1_176B:
        mov    dword ptr [esi + 0x17e], edi
        mov    dword ptr [esi + 0x182], edi
        mov    byte ptr [esi + 0x331], 0
loc_1_177E:
        mov    edi, 0x103
        mov    eax, dword ptr [esi + 0x176]
        dec    eax
        mov    dword ptr [esi + 0x176], eax
        jne    loc_1_17CB
        cmp    byte ptr [esi + 0x204], 0
        je     loc_1_17A6
        lea    eax, [esi + 0x31e]
        cmp    byte ptr [eax], 0
        je     loc_1_17BA
loc_1_17A6:
        cmp    dword ptr [esi + 0x18e], 0
        je     loc_1_17CB
        lea    eax, [esi + 0x31e]
        cmp    byte ptr [eax], 0
        jne    loc_1_17CB
loc_1_17BA:
        push   0
        add    esi, 0x212
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_17CB:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_17D6:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17F0:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        cmp    edx, esi
        je     loc_1_1812
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_1805:
        mov    bl, byte ptr [eax + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    loc_1_181A
        inc    esi
        cmp    esi, edx
        jb     loc_1_1805
loc_1_1812:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_181A:
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1830:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        lea    esi, [eax + 0x24]
        push   6
        push   0
        lea    ecx, [ebp - 8]
        mov    byte ptr [esi + 9], 0
        mov    byte ptr [esi + 8], 0
        push   eax
        push   ecx
        call   sub_1_20C0
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0x15e]
        cmp    byte ptr [ecx + 0x28], dl
        mov    ebx, dword ptr [eax + 0xc]
        jne    loc_1_1883
        push   6
        lea    eax, [ebp - 8]
        push   eax
        add    ecx, 0x24
        push   ecx
        call   sub_1_17F0
        test   eax, eax
        jne    loc_1_1883
        mov    eax, ebx
        and    al, 1
        mov    byte ptr [esi + 9], al
loc_1_1883:
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax + 0x71], 2
        je     loc_1_1898
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_1898:
        test   bl, 0x20
        je     loc_1_18A9
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_18A9:
        mov    al, byte ptr [4]              ; -> obj1:0x4AD4
        cmp    byte ptr [ebp - 4], al
        jne    loc_1_18D5
        push   6
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj1:0x4AD0
        call   sub_1_17F0
        test   eax, eax
        jne    loc_1_18D5
        and    bl, 8
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_18D5:
        test   byte ptr [ebp - 8], 1
        je     loc_1_18E9
        and    bl, 6
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_18E9:
        cmp    byte ptr [esi + 9], 0
        je     loc_1_18FB
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_18FB:
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1910:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [eax + 0x20], 0
        je     loc_1_1929
        mov    eax, 0xc0010002
        pop    ebp
        ret    4
loc_1_1929:
        cmp    byte ptr [ecx + 0x204], 0
        je     loc_1_1938
        xor    eax, eax
        pop    ebp
        ret    4
loc_1_1938:
        inc    dword ptr [eax + 0x1c]
        inc    dword ptr [ecx + 0x176]
        mov    byte ptr [ecx + 0x204], 1
        mov    dword ptr [ecx + 0x206], eax
        mov    eax, dword ptr [ecx + 0x176]
        dec    eax
        mov    dword ptr [ecx + 0x176], eax
        jne    loc_1_1996
        cmp    byte ptr [ecx + 0x204], 0
        je     loc_1_1971
        lea    eax, [ecx + 0x31e]
        cmp    byte ptr [eax], 0
        je     loc_1_1985
loc_1_1971:
        cmp    dword ptr [ecx + 0x18e], 0
        je     loc_1_1996
        lea    eax, [ecx + 0x31e]
        cmp    byte ptr [eax], 0
        jne    loc_1_1996
loc_1_1985:
        push   0
        add    ecx, 0x212
        push   ecx
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_1996:
        mov    eax, 0x103
        pop    ebp
        ret    4
        db  090h
loc_1_19A0:
        xor    eax, eax
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19B0:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_1_2FE0
        cmp    ax, 1
        pop    ebp
        mov    eax, 0
        adc    eax, -1
        and    eax, 0xc0000001
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 0x1c]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_19F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_2DF0
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x176]
        cmp    byte ptr [esi + 0x31e], bl
        jne    loc_1_1A38
loc_1_1A28:
        mov    byte ptr [esi + 0x31e], 1
        push   ebx
        push   ebx
        push   esi
        push   ebx
        call   sub_1_1B70
loc_1_1A38:
        cmp    byte ptr [esi + 0x31f], bl
        jne    loc_1_1A5D
        mov    byte ptr [esi + 0x31f], 1
        push   esi
        call   sub_1_2080
        cmp    dword ptr [esi + 0x18e], ebx
        je     loc_1_1A5D
        cmp    byte ptr [esi + 0x31e], bl
        je     loc_1_1A28
loc_1_1A5D:
        mov    eax, dword ptr [esi + 0x176]
        dec    eax
        mov    dword ptr [esi + 0x176], eax
        jne    loc_1_1AA5
        cmp    byte ptr [esi + 0x204], 0
        je     loc_1_1A7E
        cmp    byte ptr [esi + 0x31e], 0
        je     loc_1_1A90
loc_1_1A7E:
        cmp    dword ptr [esi + 0x18e], 0
        je     loc_1_1AA5
        cmp    byte ptr [esi + 0x31e], 0
        jne    loc_1_1AA5
loc_1_1A90:
        push   0
        add    esi, 0x212
        push   esi
        mov    byte ptr [esi + 0x10c], 1
        call   sub_1_461A
loc_1_1AA5:
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h
sub_1_1AB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    dword ptr [esi + 0x196], ebx
        mov    byte ptr [esi + 0x19a], 1
        push   0
        push   ebx
        call   sub_1_4668
        cmp    dword ptr [ebp - 4], 6
        jb     loc_1_1B5B
        mov    edi, dword ptr [esi + 0x20e]
        mov    eax, dword ptr [ebp - 4]
        add    edi, 0xe
        cmp    edi, eax
        jb     loc_1_1AF4
        mov    edi, eax
loc_1_1AF4:
        push   edi
        lea    eax, [esi + 0x222]
        push   0
        push   ebx
        push   eax
        call   sub_1_20C0
        cmp    dword ptr [ebp - 4], 0xe
        jae    loc_1_1B32
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [esi + 0x222]
        push   0
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 0x15e]
        push   ecx
        push   esi
        push   eax
        call   sub_1_4730
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1B32:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, 0xe
        sub    eax, 0xe
        lea    ecx, [esi + 0x230]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esi + 0x222]
        push   0xe
        mov    ecx, dword ptr [esi + 0x15e]
        push   eax
        push   eax
        push   esi
        push   ecx
        call   sub_1_4730
loc_1_1B5B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        inc    dword ptr [esi + 0x176]
        call   sub_1_2E50
        push   esi
        lea    eax, [esi + 0xcb]
        push   0                             ; -> sub_1_2DC0
        push   eax
        call   sub_1_4632
        cmp    dword ptr [esi + 0x18e], 0
        je     loc_1_1C60
loc_1_1BA7:
        mov    ebx, dword ptr [esi + 0x18e]
        cmp    dword ptr [esi + 0x192], ebx
        jne    loc_1_1BBF
        mov    dword ptr [esi + 0x192], 0
loc_1_1BBF:
        mov    eax, dword ptr [ebx + 0x24]
        push   ebx
        push   esi
        mov    dword ptr [esi + 0x18e], eax
        call   sub_1_1AB0
        push   0
        inc    dword ptr [esi + 0x1a3]
        mov    edi, dword ptr [ebx + 0x28]
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_4582
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_1C53
        mov    eax, dword ptr [ebx + 0x172]
        cmp    edi, eax
        jne    loc_1_1C00
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x172], eax
        jmp    loc_1_1C13
loc_1_1C00:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_1C0D
loc_1_1C05:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_1C05
loc_1_1C0D:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_1C13:
        mov    eax, dword ptr [ebx + 0x20e]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_1C24
        push   ebx
        call   sub_1_0A80
loc_1_1C24:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   0x75
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 0x172], 0
        jne    loc_1_1C53
        push   ebx
        push   0                             ; -> loc_1_22C0
        add    ebx, 0xcb
        push   ebx
        call   sub_1_4632
loc_1_1C53:
        cmp    dword ptr [esi + 0x18e], 0
        jne    loc_1_1BA7
loc_1_1C60:
        push   esi
        call   sub_1_3070
        cmp    byte ptr [esi + 0x19a], 0
        je     loc_1_1C82
        mov    byte ptr [esi + 0x19a], 0
        mov    eax, dword ptr [esi + 0x15e]
        push   eax
        call   sub_1_4910
loc_1_1C82:
        cmp    byte ptr [esi + 0x204], 0
        je     loc_1_2044
        cmp    dword ptr [esi + 0x176], 1
        jne    loc_1_2044
        push   esi
        lea    eax, [esi + 0xcb]
        mov    byte ptr [esi + 0x205], 1
        push   0                             ; -> sub_1_2DF0
        push   eax
        call   sub_1_4632
        mov    edi, dword ptr [esi + 0x172]
        test   edi, edi
        je     loc_1_1D52
loc_1_1CC3:
        push   0
        inc    dword ptr [edi + 0x1c]
        push   0
        mov    eax, dword ptr [edi]
        push   0x40010004
        push   eax
        call   sub_1_45A6
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_1D47
        mov    eax, dword ptr [ebx + 0x172]
        cmp    edi, eax
        jne    loc_1_1CF4
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x172], eax
        jmp    loc_1_1D07
loc_1_1CF4:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_1D01
loc_1_1CF9:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_1CF9
loc_1_1D01:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_1D07:
        mov    eax, dword ptr [ebx + 0x20e]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_1D18
        push   ebx
        call   sub_1_0A80
loc_1_1D18:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   0x75
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 0x172], 0
        jne    loc_1_1D47
        push   ebx
        push   0                             ; -> loc_1_22C0
        add    ebx, 0xcb
        push   ebx
        call   sub_1_4632
loc_1_1D47:
        mov    edi, dword ptr [edi + 0xc]
        test   edi, edi
        jne    loc_1_1CC3
loc_1_1D52:
        push   esi
        call   sub_1_2BD0
        test   ax, ax
        je     loc_1_1D79
        mov    byte ptr [esi + 0x203], 1
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000003
        push   eax
        call   sub_1_4710
        add    esp, 0xc
        jmp    loc_1_1D80
loc_1_1D79:
        mov    byte ptr [esi + 0x203], 0
loc_1_1D80:
        cmp    dword ptr [esi + 0x186], 0
        je     loc_1_1DB8
        mov    eax, dword ptr [esi + 0x17e]
        mov    ecx, dword ptr [esi + 0x18a]
        mov    dword ptr [ecx + 0x24], eax
        mov    edx, dword ptr [esi + 0x186]
        mov    dword ptr [esi + 0x17e], edx
        mov    dword ptr [esi + 0x18a], 0
        mov    dword ptr [esi + 0x186], 0
loc_1_1DB8:
        cmp    dword ptr [esi + 0x18e], 0
        je     loc_1_1DDC
        mov    eax, dword ptr [esi + 0x17e]
        mov    ecx, dword ptr [esi + 0x192]
        mov    dword ptr [ecx + 0x24], eax
        mov    edx, dword ptr [esi + 0x18e]
        mov    dword ptr [esi + 0x17e], edx
loc_1_1DDC:
        mov    dword ptr [esi + 0x192], 0
        mov    dword ptr [esi + 0x18e], 0
        cmp    dword ptr [esi + 0x17e], 0
        je     loc_1_1E9A
loc_1_1DFD:
        mov    ecx, dword ptr [esi + 0x17e]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x17e], eax
        mov    edi, dword ptr [ecx + 0x28]
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_4582
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_1E8D
        mov    eax, dword ptr [ebx + 0x172]
        cmp    edi, eax
        jne    loc_1_1E3A
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x172], eax
        jmp    loc_1_1E4D
loc_1_1E3A:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_1E47
loc_1_1E3F:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_1E3F
loc_1_1E47:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_1E4D:
        mov    eax, dword ptr [ebx + 0x20e]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_1E5E
        push   ebx
        call   sub_1_0A80
loc_1_1E5E:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   0x75
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 0x172], 0
        jne    loc_1_1E8D
        push   ebx
        push   0                             ; -> loc_1_22C0
        add    ebx, 0xcb
        push   ebx
        call   sub_1_4632
loc_1_1E8D:
        cmp    dword ptr [esi + 0x17e], 0
        jne    loc_1_1DFD
loc_1_1E9A:
        mov    dword ptr [esi + 0x182], 0
        cmp    byte ptr [esi + 0x203], 0
        jne    loc_1_1EB3
        push   esi
        call   sub_1_2F10
loc_1_1EB3:
        mov    byte ptr [esi + 0x205], 0
        mov    edi, dword ptr [esi + 0x172]
        test   edi, edi
        je     loc_1_1F92
loc_1_1EC8:
        inc    dword ptr [edi + 0x1c]
        cmp    byte ptr [esi + 0x203], 0
        je     loc_1_1EE5
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   0x40010007
        push   eax
        call   sub_1_45A6
loc_1_1EE5:
        mov    eax, 0
        push   4
        cmp    byte ptr [esi + 0x203], 1
        adc    eax, -1
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   0x40010005
        push   ecx
        call   sub_1_45A6
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_45AC
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_1F87
        mov    eax, dword ptr [ebx + 0x172]
        cmp    edi, eax
        jne    loc_1_1F34
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x172], eax
        jmp    loc_1_1F47
loc_1_1F34:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_1F41
loc_1_1F39:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_1F39
loc_1_1F41:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_1F47:
        mov    eax, dword ptr [ebx + 0x20e]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_1F58
        push   ebx
        call   sub_1_0A80
loc_1_1F58:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   0x75
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 0x172], 0
        jne    loc_1_1F87
        push   ebx
        push   0                             ; -> loc_1_22C0
        add    ebx, 0xcb
        push   ebx
        call   sub_1_4632
loc_1_1F87:
        mov    edi, dword ptr [edi + 0xc]
        test   edi, edi
        jne    loc_1_1EC8
loc_1_1F92:
        mov    eax, 0
        mov    ecx, dword ptr [esi + 0x206]
        cmp    byte ptr [esi + 0x203], 1
        adc    eax, -1
        mov    edx, dword ptr [ecx]
        and    eax, 0xc0000001
        push   eax
        push   edx
        call   sub_1_458E
        mov    ebx, dword ptr [esi + 0x206]
        mov    edi, dword ptr [ebx + 8]
        dec    dword ptr [ebx + 0x1c]
        jne    loc_1_2037
        mov    ecx, dword ptr [edi + 0x172]
        cmp    ecx, ebx
        jne    loc_1_1FD8
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [edi + 0x172], eax
        jmp    loc_1_1FF7
loc_1_1FD8:
        mov    eax, dword ptr [esi + 0x206]
        cmp    dword ptr [ecx + 0xc], eax
        je     loc_1_1FEB
loc_1_1FE3:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], eax
        jne    loc_1_1FE3
loc_1_1FEB:
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    ebx, dword ptr [esi + 0x206]
loc_1_1FF7:
        mov    eax, dword ptr [edi + 0x20e]
        cmp    dword ptr [ebx + 0x18], eax
        jne    loc_1_2008
        push   edi
        call   sub_1_0A80
loc_1_2008:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_457C
        push   0
        push   0x75
        push   ebx
        call   sub_1_4644
        cmp    dword ptr [edi + 0x172], 0
        jne    loc_1_2037
        push   edi
        push   0                             ; -> loc_1_22C0
        add    edi, 0xcb
        push   edi
        call   sub_1_4632
loc_1_2037:
        mov    byte ptr [esi + 0x204], 0
        push   esi
        call   sub_1_2F10
loc_1_2044:
        mov    byte ptr [esi + 0x31e], 0
        push   esi
        call   sub_1_2ED0
        push   esi
        lea    eax, [esi + 0xcb]
        push   0                             ; -> sub_1_2DC0
        push   eax
        call   sub_1_4632
        pop    edi
        dec    dword ptr [esi + 0x176]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2080:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x205], 0
        je     loc_1_209C
        mov    byte ptr [esi + 0x31f], 0
        pop    esi
        pop    ebp
        ret    4
loc_1_209C:
        mov    byte ptr [esi + 0x331], 0
        push   esi
        call   sub_1_3290
        mov    byte ptr [esi + 0x31f], 0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_20C0:
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
        call   sub_1_4668
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 4], esi
        je     loc_1_2114
loc_1_20E5:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_468C
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    eax, ebx
        ja     loc_1_2114
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_20E5
loc_1_2114:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_2125
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_2125:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_2134:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_2143
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_2146
loc_1_2143:
        mov    ebx, dword ptr [ebp - 8]
loc_1_2146:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_4A20
        cmp    esi, edi
        je     loc_1_2182
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_218D
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_468C
        jmp    loc_1_2134
loc_1_2182:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_218D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_21A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebx + 8]
        add    esi, 0xe
        cmp    dword ptr [edi + 0x196], 0
        je     loc_1_222B
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        mov    ebx, esi
        push   0
        push   ecx
        call   sub_1_4668
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2219
loc_1_21D9:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_468C
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edi + 0x196]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   ebx
        push   edx
        push   eax
        call   sub_1_20C0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_2219
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_21D9
loc_1_2219:
        sub    ebx, esi
        mov    ecx, dword ptr [ebp + 0x1c]
        xor    eax, eax
        pop    edi
        pop    esi
        mov    dword ptr [ecx], ebx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_222B:
        cmp    byte ptr [edi + 0x19a], 0
        je     loc_1_22AB
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   esi
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        push   edx
        call   sub_1_34C0
        test   ax, ax
        je     loc_1_22A0
        cmp    ax, 0x13
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], 0
        jne    loc_1_2292
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edi + 0x111], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [edi + 0x115], ecx
        mov    dword ptr [edi + 0x119], esi
        mov    dword ptr [edi + 0x11d], edx
        mov    dword ptr [edi + 0x121], eax
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_2292:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_22A0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_22AB:
        mov    eax, 0xc0010013
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_22C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_2F90
        cmp    ax, 1
        pop    ebp
        sbb    cl, cl
        neg    cl
        mov    al, cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_22E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x331], 0
        je     loc_1_23E9
        cmp    byte ptr [esi + 0xeb], 0
        je     loc_1_23E9
        push   esi
        call   sub_1_2DF0
        mov    byte ptr [esi + 0x331], 0
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000003
        push   eax
        call   sub_1_4710
        add    esp, 0xc
        cmp    dword ptr [esi + 0x186], 0
        je     loc_1_2361
        xor    edi, edi
loc_1_232E:
        mov    edx, dword ptr [esi + 0x186]
        lea    eax, [edx + 0x24]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x186], ecx
        test   ecx, ecx
        jne    loc_1_2349
        mov    dword ptr [esi + 0x18a], edi
loc_1_2349:
        mov    ebx, dword ptr [eax + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_4582
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x186], edi
        jne    loc_1_232E
loc_1_2361:
        cmp    dword ptr [esi + 0x17e], 0
        je     loc_1_239F
        xor    edi, edi
loc_1_236C:
        mov    edx, dword ptr [esi + 0x17e]
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x17e], eax
        test   eax, eax
        jne    loc_1_2387
        mov    dword ptr [esi + 0x182], edi
loc_1_2387:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_4582
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x17e], edi
        jne    loc_1_236C
loc_1_239F:
        mov    byte ptr [esi + 0x331], 0
        mov    byte ptr [esi + 0xeb], 0
        push   esi
        call   sub_1_2BD0
        test   ax, ax
        je     loc_1_23D4
        mov    byte ptr [esi + 0x203], 1
        push   0
        mov    eax, dword ptr [esi + 0x4e]
        push   0xaa000003
        push   eax
        call   sub_1_4710
        add    esp, 0xc
        jmp    loc_1_23DB
loc_1_23D4:
        mov    byte ptr [esi + 0x203], 0
loc_1_23DB:
        push   esi
        call   sub_1_2F10
        push   esi
        call   sub_1_2DC0
        jmp    loc_1_2402
loc_1_23E9:
        cmp    dword ptr [esi + 0x186], 0
        jne    loc_1_23FB
        cmp    dword ptr [esi + 0x17e], 0
        je     loc_1_2402
loc_1_23FB:
        mov    byte ptr [esi + 0x331], 1
loc_1_2402:
        push   0x1388
        add    esi, 0x325
        push   esi
        call   sub_1_461A
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2420:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   1
        mov    dx, word ptr [esi + 4]
        xor    edi, edi
        add    edx, 0x20
        in     al, dx
        mov    bl, al
        or     al, 0x80
        out    dx, al
        call   sub_1_4638
        mov    di, word ptr [esi + 4]
        mov    al, bl
        lea    edx, [edi + 0x20]
        xor    ecx, ecx
        out    dx, al
        mov    eax, dword ptr [esi + 0x18]
        shr    eax, 0xd
        and    al, 0x3f
        or     al, 0x40
        out    dx, al
        xor    edx, edx
        xor    eax, eax
loc_1_245C:
        cmp    byte ptr [edx + esi + 0x24], al
        je     loc_1_2464
        mov    cl, 1
loc_1_2464:
        inc    edx
        cmp    edx, 6
        jb     loc_1_245C
        lea    edx, [edi + 0x24]
        in     al, dx
        mov    bl, al
        and    al, 0x7f
        out    dx, al
        lea    edx, [edi + 0x21]
        mov    al, 0x46
        out    dx, al
loc_1_2479:
        in     al, dx
        test   al, 0x40
        jne    loc_1_2479
        xor    edi, edi
loc_1_2480:
        xor    eax, eax
        inc    edi
        mov    ax, word ptr [esi + 4]
        cmp    edi, 6
        lea    edx, [eax + edi + 0x27]
        in     al, dx
        mov    byte ptr [edi + esi + 0x29], al
        jb     loc_1_2480
        xor    edx, edx
        mov    al, bl
        mov    dx, word ptr [esi + 4]
        add    edx, 0x21
        test   cl, cl
        out    dx, al
        jne    loc_1_24B7
        xor    ecx, ecx
loc_1_24A7:
        mov    dl, byte ptr [ecx + esi + 0x2a]
        lea    eax, [ecx + esi]
        inc    ecx
        mov    byte ptr [eax + 0x24], dl
        cmp    ecx, 6
        jb     loc_1_24A7
loc_1_24B7:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0xa]
        in     al, dx
        or     al, 0x40
        out    dx, al
        lea    edx, [ecx + 0x20]
        in     al, dx
        or     al, 0x40
        out    dx, al
        lea    edx, [ecx + 0x30]
        mov    al, 1
        out    dx, al
        lea    edx, [ecx + 0x3b]
        xor    al, al
        out    dx, al
        out    dx, al
        lea    edx, [ecx + 0x37]
        in     al, dx
        test   al, 0x80
        jne    loc_1_24E7
        push   2
        call   sub_1_4638
loc_1_24E7:
        xor    edx, edx
        mov    al, 0x49
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 0x3e
        xor    edi, edi
        out    dx, al
        call   sub_1_2FE0
        mov    di, word ptr [esi + 4]
        mov    al, 4
        mov    cl, byte ptr [esi + 0x152]
        lea    ebx, [edi + 0x3d]
        mov    edx, ebx
        out    dx, al
        lea    edx, [edi + 0x31]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0x32]
        mov    al, byte ptr [esi + 0x153]
        out    dx, al
        lea    edx, [edi + 0x33]
        mov    al, cl
        out    dx, al
        mov    edx, ebx
        xor    al, al
        out    dx, al
        lea    edx, [edi + 0x37]
        mov    al, 0xff
        out    dx, al
        lea    edx, [edi + 0x3f]
        mov    al, 0x3f
        mov    byte ptr [esi + 0xc9], 0x3f
        out    dx, al
        lea    edx, [edi + 0x30]
        mov    al, 0x41
        out    dx, al
        xor    ebx, ebx
loc_1_2543:
        lea    edx, [ebx + edi + 0x31]
        mov    al, byte ptr [ebx + esi + 0x24]
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jb     loc_1_2543
        lea    eax, [ecx + 1]
        lea    edx, [edi + 0x37]
        out    dx, al
        xor    ecx, ecx
        and    eax, 0xff
        mov    cx, word ptr [esi + 0x38]
        xor    ebx, ebx
        imul   ecx, eax
        add    ecx, dword ptr [esi + 0x1c]
        lea    edi, [esi + 0xcb]
        mov    dword ptr [esi + 0x154], ecx
loc_1_2578:
        push   esi
        mov    byte ptr [esi + 0x13c], bl
        push   0                             ; -> loc_1_37D0
        push   edi
        call   sub_1_4632
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0x20]
        test   eax, 0x3fffff
        jbe    loc_1_25AF
loc_1_2599:
        mov    eax, dword ptr [esi + 0x1c]
        inc    ecx
        mov    byte ptr [eax + ecx - 1], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 0x20]
        shl    eax, 0xa
        cmp    eax, ecx
        ja     loc_1_2599
loc_1_25AF:
        inc    ebx
        cmp    ebx, 3
        jb     loc_1_2578
        mov    byte ptr [esi + 0x13c], 0
        push   esi
        push   0                             ; -> loc_1_37D0
        push   edi
        call   sub_1_4632
        mov    si, word ptr [esi + 4]
        mov    al, 1
        and    esi, 0xffff
        lea    edx, [esi + 0x30]
        lea    ecx, [esi + 0x24]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    bl, al
        or     al, 0x80
        out    dx, al
        lea    edx, [esi + 0x2d]
        in     al, dx
        or     al, 0x4c
        out    dx, al
        lea    edx, [esi + 0x25]
        in     al, dx
        or     al, 0x80
        out    dx, al
        lea    edx, [esi + 0x26]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0x2b]
        xor    al, al
        out    dx, al
        mov    al, bl
        mov    edx, ecx
        and    al, 0x7f
        pop    edi
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_2610:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    si, word ptr [ecx + 0xe7]
        mov    word ptr [ecx + 0xe9], si
        lea    eax, [esi + 1]
        mov    word ptr [ecx + 0xe7], ax
        cmp    word ptr [ecx + 0x3a], ax
        jne    loc_1_2641
        mov    word ptr [ecx + 0xe7], 0
loc_1_2641:
        movsx  eax, word ptr [ecx + 0xe7]
        cmp    dword ptr [ecx + eax*4 + 0xf4], 2
        je     loc_1_265B
        mov    word ptr [ecx + 0xe7], 0xffff
loc_1_265B:
        movsx  eax, si
        mov    bx, word ptr [ecx + eax*4 + 0xec]
        xor    edi, edi
        mov    di, word ptr [ecx + 4]
        mov    ax, word ptr [ecx + 0xc3]
        lea    edx, [edi + 0x34]
        imul   ax, si
        mov    al, ah
        out    dx, al
        lea    edx, [edi + 0x35]
        mov    al, bl
        out    dx, al
        lea    edx, [edi + 0x36]
        mov    al, bh
        out    dx, al
        lea    edx, [edi + 0x30]
        mov    al, 0x26
        mov    byte ptr [ecx + 0xeb], 1
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_26A0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0xc3]
        lea    edx, [ebp - 8]
        movsx  ecx, word ptr [esi + 0xe5]
        imul   eax, ecx
        add    eax, dword ptr [esi + 0x1c]
        lea    ecx, [ebp - 0x20]
        push   ecx
        xor    ebx, ebx
        push   edx
        mov    dword ptr [ebp - 0x18], eax
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        push   ebx
        push   eax
        call   sub_1_4668
        mov    ecx, dword ptr [esi + 0x125]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    dword ptr [ebp - 0x10], ebx
        add    edx, 0x30
        mov    byte ptr [ebp - 1], bl
        in     al, dx
        mov    dword ptr [ebp - 0x14], ecx
        test   al, 4
        jne    loc_1_26FE
        mov    byte ptr [ebp - 1], 1
        mov    al, 0x10
        out    dx, al
loc_1_26FE:
        xor    ebx, ebx
        push   esi
        push   0                             ; -> loc_1_37D0
        lea    eax, [esi + 0xcb]
        push   eax
        mov    byte ptr [esi + 0x13c], bl
        call   sub_1_4632
        cmp    dword ptr [ebp - 8], ebx
        je     loc_1_2816
loc_1_2721:
        xor    edi, edi
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 0x1c]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        push   edx
        call   sub_1_468C
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x1c]
        push   eax
        mov    edx, dword ptr [ebp - 0x18]
        mov    word ptr [esi + 0x4a], 5
        push   ecx
        push   edx
        call   sub_1_4A20
        cmp    dword ptr [esi + 0x100], ebx
        je     loc_1_2798
loc_1_2753:
        mov    eax, edi
        inc    edi
        cmp    eax, 0xffff
        jae    loc_1_2798
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0xb]
        in     al, dx
        test   al, 0x10
        je     loc_1_2790
        lea    edx, [ecx + 0xa]
        mov    dword ptr [esi + 0x100], ebx
        in     al, dx
        and    al, 0x7f
        out    dx, al
        mov    eax, dword ptr [esi + 0x104]
        inc    eax
        mov    dword ptr [esi + 0x104], eax
        cmp    eax, 0x31
        jne    loc_1_2790
        mov    dword ptr [esi + 0x104], ebx
loc_1_2790:
        cmp    dword ptr [esi + 0x100], ebx
        jne    loc_1_2753
loc_1_2798:
        mov    eax, dword ptr [ebp - 0xc]
        add    dword ptr [ebp - 0x18], eax
        add    dword ptr [ebp - 0x10], eax
        cmp    byte ptr [ebp - 1], bl
        je     loc_1_2800
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x14], ecx
        jae    loc_1_2800
        movsx  eax, word ptr [esi + 0xe5]
        mov    dword ptr [esi + eax*4 + 0xf4], 2
        cmp    word ptr [esi + 0xe7], -1
        jne    loc_1_2800
        mov    ax, word ptr [esi + 0xe5]
        cmp    word ptr [esi + 0xe9], -1
        mov    word ptr [esi + 0xe7], ax
        jne    loc_1_2800
        or     byte ptr [esi + 0x42], 0x20
        movsx  eax, ax
        mov    ecx, dword ptr [ebp - 0x20]
        push   esi
        mov    dword ptr [esi + eax*4 + 0xec], ecx
        call   sub_1_2610
        mov    dword ptr [ebp - 0x14], 0xffffffff
loc_1_2800:
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_1_2721
loc_1_2816:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_2828
        xor    edx, edx
        mov    al, 8
        mov    dx, word ptr [esi + 4]
        add    edx, 0x30
        out    dx, al
loc_1_2828:
        cmp    dword ptr [ebp - 0x10], 0x3c
        movsx  eax, word ptr [esi + 0xe5]
        jae    loc_1_2849
        mov    dword ptr [esi + eax*4 + 0xec], 0x3c
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2849:
        mov    ecx, dword ptr [ebp - 0x10]
        pop    edi
        mov    dword ptr [esi + eax*4 + 0xec], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_2860:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 0x38
        mov    al, byte ptr [esi + 0x3c]
        out    dx, al
        push   0                             ; -> loc_1_37A0
        lea    eax, [esi + 0xcb]
        push   eax
        call   sub_1_4632
        mov    cl, byte ptr [esi + 0xe3]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 3], cl
        add    edx, 0x33
        in     al, dx
        lea    ebx, [eax + 1]
        cmp    byte ptr [esi + 0x153], bl
        ja     loc_1_28AF
        mov    bl, byte ptr [esi + 0x152]
loc_1_28AF:
        mov    di, word ptr [ebp - 2]
loc_1_28B3:
        cmp    bl, byte ptr [ebp - 3]
        je     loc_1_29E0
loc_1_28BC:
        mov    al, bl
        push   esi
        shr    al, 5
        push   0                             ; -> loc_1_37D0
        lea    ecx, [esi + 0xcb]
        mov    byte ptr [esi + 0x13d], al
        push   ecx
        mov    byte ptr [esi + 0x13c], al
        call   sub_1_4632
        mov    al, bl
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x38]
        and    eax, 0x1f
        imul   eax, ecx
        add    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebp - 8], 0
        mov    cx, word ptr [eax + 2]
        mov    dl, byte ptr [eax]
        mov    word ptr [ebp - 8], cx
        mov    cl, byte ptr [eax + 1]
        mov    byte ptr [ebp - 2], cl
        cmp    byte ptr [esi + 0x152], cl
        ja     loc_1_2987
        cmp    byte ptr [esi + 0x153], cl
        jb     loc_1_2987
        test   dl, 1
        mov    byte ptr [esi + 0xe4], bl
        je     loc_1_2941
        add    eax, 4
        push   esi
        mov    dword ptr [esi + 0x14c], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 4
        push   eax
        mov    dword ptr [esi + 0x158], eax
        call   sub_1_3A90
        mov    di, ax
loc_1_2941:
        mov    bl, byte ptr [ebp - 2]
        mov    al, byte ptr [esi + 0x153]
        cmp    bl, al
        jbe    loc_1_2956
        sub    bl, al
        add    bl, byte ptr [esi + 0x152]
loc_1_2956:
        cmp    byte ptr [esi + 0x152], bl
        jne    loc_1_2965
        and    eax, 0xff
        jmp    loc_1_2969
loc_1_2965:
        xor    eax, eax
        mov    al, bl
loc_1_2969:
        dec    eax
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x33
        out    dx, al
        cmp    di, 5
        jne    loc_1_28B3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2987:
        mov    bl, byte ptr [ebp - 3]
        mov    cl, byte ptr [esi + 0x153]
        cmp    bl, cl
        jbe    loc_1_299C
        sub    bl, cl
        add    bl, byte ptr [esi + 0x152]
loc_1_299C:
        mov    al, bl
        push   esi
        shr    al, 5
        push   0                             ; -> loc_1_37D0
        lea    ecx, [esi + 0xcb]
        mov    byte ptr [esi + 0x13d], al
        push   ecx
        mov    byte ptr [esi + 0x13c], al
        call   sub_1_4632
        cmp    byte ptr [esi + 0x152], bl
        jne    loc_1_29D1
        xor    eax, eax
        mov    al, byte ptr [esi + 0x153]
        jmp    loc_1_29D5
loc_1_29D1:
        xor    eax, eax
        mov    al, bl
loc_1_29D5:
        dec    eax
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x33
        out    dx, al
loc_1_29E0:
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 0x37
        push   0                             ; -> loc_1_37A0
        out    dx, al
        lea    eax, [esi + 0xcb]
        push   eax
        call   sub_1_4632
        mov    cl, byte ptr [esi + 0xe3]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 3], cl
        add    edx, 0x33
        in     al, dx
        lea    ebx, [eax + 1]
        cmp    byte ptr [esi + 0x153], bl
        ja     loc_1_2A22
        mov    bl, byte ptr [esi + 0x152]
loc_1_2A22:
        mov    al, bl
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x38]
        and    eax, 0x1f
        imul   eax, ecx
        add    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [esi + 0x154], eax
        mov    al, bl
        shr    al, 5
        cmp    bl, byte ptr [ebp - 3]
        mov    byte ptr [esi + 0x13f], al
        jne    loc_1_28BC
        cmp    byte ptr [esi + 0x150], 0
        je     loc_1_2A6E
        xor    ecx, ecx
        mov    al, 0x10
        mov    cx, word ptr [esi + 4]
        mov    byte ptr [esi + 0x150], 0
        lea    edx, [ecx + 0x37]
        out    dx, al
        lea    edx, [ecx + 0x33]
        in     al, dx
        out    dx, al
loc_1_2A6E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        in     al, dx
        mov    edi, edx
        mov    byte ptr [ebp - 2], al
        and    edi, 0xffff
        shl    word ptr [ebp - 2], 8
        lea    edx, [edi + 1]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    word ptr [ebp - 2], 0x4da3
        je     loc_1_2ABD
        mov    ax, 0xffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2ABD:
        lea    edx, [edi + 2]
        in     al, dx
        lea    edx, [edi + 3]
        mov    byte ptr [ebp - 2], al
        in     al, dx
        shl    word ptr [ebp - 2], 8
        mov    byte ptr [ebp - 2], al
        cmp    word ptr [ebp - 2], 0x8010
        je     loc_1_2AE5
        mov    ax, 0xffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2AE5:
        xor    eax, eax
        lea    edx, [edi + 0xa]
        mov    ax, word ptr [ebp - 2]
        mov    ebx, 0xfba75320
        mov    dword ptr [esi + 0xbb], eax
        in     al, dx
        mov    cl, al
        in     al, dx
        and    cl, 7
        shl    cl, 2
        shr    ebx, cl
        and    bx, 0xf
        mov    cl, al
        mov    word ptr [esi + 0xa], bx
        and    cl, 0xf0
        lea    edx, [edi + 8]
        cmp    cl, 1
        in     al, dx
        mov    ecx, 0
        adc    ecx, -1
        and    cx, 0x20
        lea    edx, [edi + 9]
        mov    word ptr [esi + 0xc], cx
        mov    cl, al
        in     al, dx
        and    ecx, 0xfc
        and    eax, 0xf0
        mov    ch, 1
        mov    ah, 6
        mov    word ptr [esi + 0x3a], 2
        mov    word ptr [esi + 0xc3], 0x600
        mov    word ptr [esi + 0x16], 0x20
        mov    word ptr [esi + 0x20], 8
        mov    word ptr [esi + 0x4c], 0x1fff
        shl    ecx, 0xb
        shl    eax, 9
        mov    dword ptr [esi + 0x18], ecx
        pop    edi
        mov    dword ptr [esi + 0xe], eax
        xor    eax, eax
        mov    word ptr [esi + 0x42], ax
        mov    dword ptr [esi + 6], eax
        mov    ax, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B90:
        xor    ax, ax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BA0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 0x3c], 4
        mov    ebp, esp
        jae    loc_1_2BB4
        mov    word ptr [eax + 0x3c], 4
loc_1_2BB4:
        xor    edx, edx
        pop    ebp
        mov    dx, word ptr [eax + 4]
        mov    al, byte ptr [eax + 0x3c]
        add    edx, 0x38
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4380
        push   esi
        call   sub_1_2DF0
        mov    eax, 0xffffffff
        xor    ecx, ecx
        mov    word ptr [esi + 0xe9], ax
        mov    byte ptr [esi + 0xeb], cl
        mov    word ptr [esi + 0xe5], cx
        mov    byte ptr [esi + 0x151], cl
        mov    byte ptr [esi + 0x129], 3
        mov    dword ptr [esi + 0x125], 0x180
        cmp    word ptr [esi + 0x3a], 3
        mov    word ptr [esi + 0xe7], ax
        jbe    loc_1_2C2B
        mov    word ptr [esi + 0x3a], 2
loc_1_2C2B:
        xor    eax, eax
        cmp    word ptr [esi + 0x3a], ax
        jbe    loc_1_2C4E
        lea    ecx, [esi + 0xf4]
        xor    edx, edx
loc_1_2C3B:
        mov    dword ptr [ecx], edx
        add    ecx, 4
        inc    eax
        xor    edi, edi
        mov    dword ptr [ecx - 0xc], edx
        mov    di, word ptr [esi + 0x3a]
        cmp    edi, eax
        ja     loc_1_2C3B
loc_1_2C4E:
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi + 0xc3]
        mov    edi, dword ptr [esi + 0x1c]
        mov    ax, word ptr [esi + 0x3a]
        mov    edx, dword ptr [esi + 0x1c]
        mov    byte ptr [esi + 0xe3], 0
        imul   ecx, eax
        mov    ax, word ptr [esi + 0x16]
        add    edi, ecx
        mov    ecx, eax
        mov    dword ptr [esi + 0x12c], edi
        and    ecx, 0xffff
        mov    dword ptr [esi + 0x138], edx
        mov    ebx, ecx
        shl    ebx, 0xa
        add    edx, ebx
        mov    dword ptr [esi + 0x134], edx
        xor    edx, edx
        mov    dx, word ptr [esi + 0x20]
        shl    edx, 0xa
        add    edx, dword ptr [esi + 0x1c]
        mov    dword ptr [esi + 0x130], edx
        sub    dx, dx
        div    word ptr [esi + 0x20]
        mov    byte ptr [esi + 0x13e], al
        mov    eax, edi
        sub    eax, dword ptr [esi + 0x1c]
        xor    edi, edi
        mov    di, word ptr [esi + 0x38]
        and    ecx, 0x3fffff
        cdq    
        idiv   edi
        mov    byte ptr [esi + 0x152], al
        cmp    ecx, 0x40
        jne    loc_1_2CDB
        mov    byte ptr [esi + 0x153], 0xff
        jmp    loc_1_2CEC
loc_1_2CDB:
        mov    eax, ebx
        cdq    
        and    edx, 0xff
        add    eax, edx
        mov    byte ptr [esi + 0x153], ah
loc_1_2CEC:
        xor    eax, eax
        mov    dword ptr [esi + 0x154], eax
        mov    dword ptr [esi + 0x115], eax
        mov    dword ptr [esi + 0x119], eax
        mov    dword ptr [esi + 0x11d], eax
        mov    dword ptr [esi + 0x121], eax
        mov    dword ptr [esi + 0x10c], eax
        mov    dword ptr [esi + 0x100], eax
        mov    dword ptr [esi + 0xfc], eax
        mov    dword ptr [esi + 0x108], 0x32
        cmp    word ptr [esi + 0x3c], 4
        mov    dword ptr [esi + 0x104], eax
        jae    loc_1_2D3B
        mov    word ptr [esi + 0x3c], 4
loc_1_2D3B:
        xor    edx, edx
        mov    al, byte ptr [esi + 0x3c]
        mov    dx, word ptr [esi + 4]
        push   esi
        mov    word ptr [esi + 0x12a], 0x187
        add    edx, 0x38
        out    dx, al
        call   sub_1_2420
        test   al, al
        jne    loc_1_2D6D
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0xc7], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2D6D:
        cmp    byte ptr [esi + 0x110], 1
        je     loc_1_2D83
        push   esi
        call   sub_1_3B50
        mov    byte ptr [esi + 0x110], 1
loc_1_2D83:
        push   esi
        call   sub_1_3FD0
        push   esi
        call   sub_1_2420
        test   al, al
        jne    loc_1_2DA5
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0xc7], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2DA5:
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0xc7], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2DC0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0xc7], 1
        je     loc_1_2DD7
        xor    ax, ax
        pop    ebp
        ret    4
loc_1_2DD7:
        xor    edx, edx
        pop    ebp
        mov    dx, word ptr [eax + 4]
        add    edx, 0xb
        in     al, dx
        or     al, 0x80
        out    dx, al
        xor    ax, ax
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2DF0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [eax + 4]
        pop    ebp
        add    edx, 0xb
        in     al, dx
        and    al, 0x7f
        out    dx, al
        xor    ax, ax
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 056h, 08Ah, 088h, 0C9h
        db  000h, 000h, 000h, 033h, 0F6h, 066h, 08Bh, 070h, 004h, 080h, 0E1h, 0F5h, 088h, 088h, 0C9h, 000h
        db  000h, 000h, 08Dh, 056h, 030h, 032h, 0C0h, 0EEh, 08Dh, 056h, 03Fh, 08Ah, 0C1h, 0EEh, 066h, 033h
        db  0C0h, 05Eh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h
        db  024h, 000h, 000h, 000h, 000h
sub_1_2E50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    cl, byte ptr [eax + 0xc9]
        xor    esi, esi
        mov    si, word ptr [eax + 4]
        and    cl, 0xca
        mov    byte ptr [eax + 0xc9], cl
        lea    edx, [esi + 0x30]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0x3f]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h
        db  08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 056h, 08Ah, 088h, 0C9h, 000h, 000h, 000h, 033h, 0F6h, 066h
        db  08Bh, 070h, 004h, 080h, 0C9h, 00Ah, 088h, 088h, 0C9h, 000h, 000h, 000h, 08Dh, 056h, 030h, 032h
        db  0C0h, 0EEh, 08Dh, 056h, 03Fh, 08Ah, 0C1h, 0EEh, 066h, 033h, 0C0h, 05Eh, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2ED0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    cl, byte ptr [eax + 0xc9]
        xor    esi, esi
        mov    si, word ptr [eax + 4]
        or     cl, 0x35
        mov    byte ptr [eax + 0xc9], cl
        lea    edx, [esi + 0x30]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0x3f]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2F10:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0xc7]
        cmp    al, 1
        jne    loc_1_2F29
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_1_2F29:
        cmp    al, 3
        jne    loc_1_2F38
        push   esi
        call   sub_1_2BD0
        test   ax, ax
        jne    loc_1_2F86
loc_1_2F38:
        cmp    byte ptr [esi + 0xc7], 2
        je     loc_1_2F4A
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_2F4A:
        xor    ecx, ecx
        mov    al, 2
        mov    cx, word ptr [esi + 4]
        test   byte ptr [esi + 0xbf], 0x10
        lea    edx, [ecx + 0x30]
        out    dx, al
        lea    edx, [ecx + 0x3d]
        je     loc_1_2F66
        mov    al, 1
        jmp    loc_1_2F68
loc_1_2F66:
        xor    al, al
loc_1_2F68:
        out    dx, al
        mov    byte ptr [esi + 0xc7], 1
        push   esi
        call   sub_1_2DC0
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x26
        in     al, dx
        or     al, 1
        out    dx, al
        xor    ax, ax
loc_1_2F86:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2F90:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xc7], 1
        je     loc_1_2FA9
        mov    ax, 0xa
        pop    esi
        pop    ebp
        ret    4
loc_1_2FA9:
        xor    edx, edx
        push   esi
        mov    dx, word ptr [esi + 4]
        add    edx, 0x26
        in     al, dx
        and    al, 0xfe
        out    dx, al
        call   sub_1_2DF0
        xor    edx, edx
        mov    al, 1
        mov    byte ptr [esi + 0xc7], 3
        mov    dx, word ptr [esi + 4]
        pop    esi
        add    edx, 0x30
        pop    ebp
        out    dx, al
        xor    ax, ax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2FE0:
        push   ebp
        xor    cl, cl
        mov    ebp, esp
        xor    edx, edx
        push   esi
        mov    al, 0x20
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        mov    edi, 0
        add    edx, 0x30
        out    dx, al
        mov    eax, dword ptr [esi + 0x15e]
        test   eax, eax
        je     loc_1_3008
        mov    edi, dword ptr [eax + 0xc]
loc_1_3008:
        test   edi, 6
        je     loc_1_3024
        push   esi
        lea    eax, [esi + 0xcb]
        push   0                             ; -> loc_1_3800
        push   eax
        call   sub_1_4632
        mov    cl, 8
loc_1_3024:
        test   edi, 8
        je     loc_1_302F
        or     cl, 4
loc_1_302F:
        test   edi, 0x20
        je     loc_1_303A
        or     cl, 0x1c
loc_1_303A:
        or     cl, 1
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    al, cl
        add    edx, 0x3c
        pop    edi
        out    dx, al
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 066h, 0B8h
        db  080h, 000h, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3070:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    edx, edx
        push   ebx
        push   esi
        mov    ebx, 1
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 1], bl
        add    edx, 0x37
        in     al, dx
        and    al, 0x35
        je     loc_1_3254
        mov    byte ptr [ebp - 1], 0
loc_1_309A:
        test   al, 0x10
        je     loc_1_30AF
        push   esi
        mov    byte ptr [esi + 0x150], bl
        call   sub_1_2860
        jmp    loc_1_3242
loc_1_30AF:
        test   al, 0x20
        je     loc_1_3108
        xor    edx, edx
        mov    ecx, dword ptr [esi + 0x93]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x3d
        in     al, dx
        mov    edi, eax
        xor    edx, edx
        and    edi, 0xff
        add    dword ptr [ecx], edi
        mov    ecx, dword ptr [esi + 0x5f]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x3e
        in     al, dx
        mov    edi, eax
        xor    edx, edx
        and    edi, 0xff
        add    dword ptr [ecx], edi
        mov    ecx, dword ptr [esi + 0x63]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x3f
        in     al, dx
        and    eax, 0xff
        xor    edx, edx
        add    dword ptr [ecx], eax
        mov    al, 0x20
        mov    dx, word ptr [esi + 4]
        add    edx, 0x37
        jmp    loc_1_3241
loc_1_3108:
        test   bl, al
        je     loc_1_3117
        push   esi
        call   sub_1_2860
        jmp    loc_1_3242
loc_1_3117:
        test   al, 0x40
        je     loc_1_31DB
        test   byte ptr [esi + 0x43], 4
        je     loc_1_3135
        xor    edx, edx
        mov    al, 0x40
        mov    dx, word ptr [esi + 4]
        add    edx, 0x37
        jmp    loc_1_3241
loc_1_3135:
        mov    al, byte ptr [esi + 0x13f]
        push   esi
        push   0                             ; -> loc_1_37D0
        lea    ecx, [esi + 0xcb]
        push   ecx
        mov    byte ptr [esi + 0x13c], al
        call   sub_1_4632
        mov    ecx, dword ptr [esi + 0x154]
        mov    dx, word ptr [ecx + 0x10]
        mov    word ptr [ebp - 8], dx
        mov    eax, dword ptr [ebp - 8]
        mov    al, ah
        mov    ah, dl
        mov    ecx, eax
        and    ecx, 0xffff
        mov    dword ptr [ebp - 8], ecx
        cmp    ecx, 0x5dc
        jbe    loc_1_318B
        xor    edx, edx
        mov    al, 0x40
        mov    dx, word ptr [esi + 4]
        add    edx, 0x37
        jmp    loc_1_3241
loc_1_318B:
        add    dword ptr [ebp - 8], 0x12
        cmp    dword ptr [ebp - 8], 0x40
        jae    loc_1_319C
        mov    dword ptr [ebp - 8], 0x40
loc_1_319C:
        cmp    dword ptr [ebp - 8], 0x100
        jae    loc_1_31AA
        mov    cl, byte ptr [esi + 0x3c]
        jmp    loc_1_31B3
loc_1_31AA:
        mov    ecx, dword ptr [ebp - 8]
        lea    ecx, [ecx + ecx*2]
        shr    ecx, 4
loc_1_31B3:
        xor    edi, edi
        mov    al, 0x40
        mov    di, word ptr [esi + 4]
        lea    edx, [edi + 0x37]
        out    dx, al
        lea    edx, [edi + 0x38]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0x37]
        in     al, dx
        test   bl, al
        je     loc_1_31D5
        push   esi
        call   sub_1_2860
        jmp    loc_1_3242
loc_1_31D5:
        test   al, 4
        je     loc_1_3242
        jmp    loc_1_31DF
loc_1_31DB:
        test   al, 4
        je     loc_1_3242
loc_1_31DF:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x3c
        in     al, dx
        test   al, 2
        mov    cl, al
        je     loc_1_31F4
        mov    eax, dword ptr [esi + 0x5f]
        inc    dword ptr [eax]
loc_1_31F4:
        test   cl, 4
        je     loc_1_3210
        xor    edx, edx
        mov    edi, dword ptr [esi + 0x93]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x3d
        in     al, dx
        and    eax, 0xff
        add    dword ptr [edi], eax
loc_1_3210:
        test   cl, 8
        je     loc_1_321D
        mov    eax, dword ptr [esi + 0x97]
        inc    dword ptr [eax]
loc_1_321D:
        test   cl, 0x10
        je     loc_1_3236
        xor    edx, edx
        mov    ecx, dword ptr [esi + 0x63]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x3f
        in     al, dx
        and    eax, 0xff
        add    dword ptr [ecx], eax
loc_1_3236:
        xor    edx, edx
        mov    al, 4
        mov    dx, word ptr [esi + 4]
        add    edx, 0x37
loc_1_3241:
        out    dx, al
loc_1_3242:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x37
        in     al, dx
        and    al, 0x35
        jne    loc_1_309A
loc_1_3254:
        cmp    byte ptr [esi + 0xca], 0
        je     loc_1_3270
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0xca], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3270:
        mov    eax, 0
        pop    edi
        cmp    byte ptr [ebp - 1], 1
        adc    eax, -1
        pop    esi
        and    ax, 3
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3290:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        add    edx, 0x37
        in     al, dx
        and    al, 0x1a
        test   al, 0xa
        je     loc_1_3403
loc_1_32AD:
        test   al, 2
        jne    loc_1_339C
        test   al, 8
        je     loc_1_33EF
        xor    ecx, ecx
        mov    al, 8
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0x37]
        out    dx, al
        lea    edx, [ecx + 0x34]
        in     al, dx
        mov    bl, al
        and    bl, 8
        je     loc_1_32DC
        mov    eax, dword ptr [esi + 0x9f]
        jmp    loc_1_32E6
loc_1_32DC:
        test   al, 0x20
        je     loc_1_32E8
        mov    eax, dword ptr [esi + 0xb7]
loc_1_32E6:
        inc    dword ptr [eax]
loc_1_32E8:
        dec    byte ptr [esi + 0x129]
        jne    loc_1_335E
        mov    byte ptr [esi + 0x129], 3
        cmp    byte ptr [esi + 0xeb], 0
        je     loc_1_33EF
        mov    byte ptr [esi + 0xeb], 0
        mov    byte ptr [esi + 0x331], 0
        movsx  eax, word ptr [esi + 0xe9]
        mov    dword ptr [esi + eax*4 + 0xf4], 0
        mov    word ptr [esi + 0xe9], 0xffff
        cmp    word ptr [esi + 0xe7], -1
        je     loc_1_333D
        push   esi
        call   sub_1_2610
loc_1_333D:
        push   esi
        mov    eax, 0xc
        cmp    bl, 1
        adc    eax, -1
        push   eax
        call   sub_1_38D0
        cmp    ax, 5
        je     loc_1_3403
        jmp    loc_1_33EF
loc_1_335E:
        xor    edi, edi
        xor    eax, eax
        mov    di, word ptr [esi + 4]
        movsx  ebx, word ptr [esi + 0xe9]
        mov    cx, word ptr [esi + ebx*4 + 0xec]
        lea    edx, [edi + 0x34]
        mov    ax, word ptr [esi + 0xc3]
        imul   eax, ebx
        add    eax, dword ptr [esi + 0x18]
        mov    al, ah
        out    dx, al
        lea    edx, [edi + 0x35]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0x36]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0x30]
        mov    al, 0x26
        out    dx, al
        jmp    loc_1_33EF
loc_1_339C:
        xor    edx, edx
        mov    al, 2
        mov    byte ptr [esi + 0x129], 3
        mov    dx, word ptr [esi + 4]
        add    edx, 0x37
        out    dx, al
        movsx  eax, word ptr [esi + 0xe9]
        mov    byte ptr [esi + 0xeb], 0
        mov    dword ptr [esi + eax*4 + 0xf4], 0
        mov    word ptr [esi + 0xe9], 0xffff
        cmp    word ptr [esi + 0xe7], -1
        je     loc_1_33E1
        push   esi
        call   sub_1_2610
loc_1_33E1:
        push   esi
        push   0
        call   sub_1_38D0
        cmp    ax, 5
        je     loc_1_3403
loc_1_33EF:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0x37
        in     al, dx
        and    al, 0x1a
        test   al, 0xa
        jne    loc_1_32AD
loc_1_3403:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3410:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    di, word ptr [esi + 0xe5]
        movsx  eax, di
        lea    ebx, [esi + eax*4 + 0xf4]
        cmp    dword ptr [ebx], 0
        je     loc_1_343A
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_343A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 1
        push   eax
        push   esi
        call   sub_1_26A0
        mov    ax, word ptr [esi + 0xe5]
        inc    ax
        mov    word ptr [esi + 0xe5], ax
        cmp    ax, 2
        jne    loc_1_3469
        mov    word ptr [esi + 0xe5], 0
loc_1_3469:
        mov    ax, word ptr [esi + 0x42]
        test   al, 0x20
        jne    loc_1_34A2
        mov    dword ptr [ebx], 2
        cmp    word ptr [esi + 0xe7], -1
        jne    loc_1_34AA
        cmp    word ptr [esi + 0xe9], -1
        mov    word ptr [esi + 0xe7], di
        jne    loc_1_34AA
        push   esi
        call   sub_1_2610
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_34A2:
        and    ax, 0xffdf
        mov    word ptr [esi + 0x42], ax
loc_1_34AA:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_34C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        push   edi
        mov    esi, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x158]
        add    ecx, edi
        cmp    ecx, eax
        jbe    loc_1_34ED
        sub    eax, dword ptr [ebp + 0x10]
        mov    edi, eax
loc_1_34ED:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   0
        mov    dword ptr [ebp - 4], edi
        push   0
        push   ecx
        call   sub_1_4668
        lea    ecx, [ebp - 0x18]
        lea    edx, [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_468C
        mov    cl, byte ptr [esi + 0x13d]
        push   esi
        push   0                             ; -> loc_1_37D0
        lea    edx, [esi + 0xcb]
        push   edx
        mov    byte ptr [esi + 0x13c], cl
        mov    dword ptr [ebp - 8], 0
        call   sub_1_4632
        mov    ebx, dword ptr [esi + 0x14c]
        xor    eax, eax
        add    ebx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [esi + 0xe4]
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 0x10]
        test   edi, edi
        lea    edx, [eax + ecx + 4]
        mov    dword ptr [ebp - 0x10], edx
        je     loc_1_369C
loc_1_355F:
        mov    eax, dword ptr [ebp - 0x18]
        mov    edi, dword ptr [ebp - 4]
        sub    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jb     loc_1_356F
        mov    edi, eax
loc_1_356F:
        mov    eax, dword ptr [esi + 0x130]
        lea    ecx, [edi + ebx]
        cmp    ecx, eax
        jbe    loc_1_3580
        mov    edi, eax
        sub    edi, ebx
loc_1_3580:
        push   edi
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [esi + 0xfc], 0
        mov    word ptr [esi + 0x4a], 5
        push   ebx
        push   eax
        add    ebx, edi
        call   sub_1_4A20
        mov    eax, dword ptr [ebp + 8]
        add    dword ptr [ebp - 0x10], edi
        add    dword ptr [ebp - 8], edi
        sub    dword ptr [ebp - 4], edi
        add    dword ptr [eax], edi
        xor    edi, edi
        mov    ecx, dword ptr [esi + 0xfc]
loc_1_35B3:
        test   ecx, ecx
        je     loc_1_35E6
        mov    eax, edi
        inc    edi
        cmp    eax, 0xffff
        jae    loc_1_35E6
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0xb
        in     al, dx
        test   al, 0x10
        je     loc_1_35B3
        mov    dword ptr [esi + 0xfc], 0
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0xa
        in     al, dx
        and    al, 0x7f
        out    dx, al
loc_1_35E6:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_369C
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0x14], eax
        cmp    eax, dword ptr [ebp - 0x18]
        jne    loc_1_362A
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_369C
        lea    eax, [ebp - 0x18]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_468C
        mov    dword ptr [ebp - 8], 0
loc_1_362A:
        cmp    dword ptr [esi + 0x130], ebx
        ja     loc_1_3676
        mov    al, byte ptr [esi + 0x13c]
        inc    al
        mov    byte ptr [esi + 0x13c], al
        cmp    byte ptr [esi + 0x13e], al
        jne    loc_1_365E
        mov    byte ptr [esi + 0x13d], 0
        mov    byte ptr [esi + 0x13c], 0
        mov    ebx, dword ptr [esi + 0x12c]
        jmp    loc_1_3664
loc_1_365E:
        mov    ebx, dword ptr [esi + 0x138]
loc_1_3664:
        push   esi
        lea    eax, [esi + 0xcb]
        push   0                             ; -> loc_1_37D0
        push   eax
        call   sub_1_4632
loc_1_3676:
        mov    eax, dword ptr [esi + 0x1c]
        add    eax, dword ptr [ebp - 0x10]
        cmp    eax, dword ptr [esi + 0x134]
        jb     loc_1_3692
        xor    eax, eax
        mov    al, byte ptr [esi + 0x152]
        shl    eax, 8
        mov    dword ptr [ebp - 0x10], eax
loc_1_3692:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_355F
loc_1_369C:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_36B0:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
        push   esi
        mov    dword ptr [ebp - 8], eax
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x158]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    loc_1_36DA
        sub    eax, ecx
        mov    edi, eax
loc_1_36DA:
        mov    ebx, dword ptr [esi + 0x14c]
        mov    al, byte ptr [esi + 0x13d]
        push   esi
        lea    ecx, [esi + 0xcb]
        push   0                             ; -> loc_1_37D0
        mov    byte ptr [esi + 0x13c], al
        push   ecx
        call   sub_1_4632
        test   edi, edi
        je     loc_1_378A
loc_1_3706:
        mov    eax, dword ptr [esi + 0x130]
        lea    ecx, [edi + ebx]
        mov    dword ptr [ebp - 4], edi
        cmp    ecx, eax
        jbe    loc_1_371B
        sub    eax, ebx
        mov    dword ptr [ebp - 4], eax
loc_1_371B:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ebx
        push   ecx
        call   sub_1_4A20
        add    ebx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    dword ptr [ebp + 0x10], ecx
        sub    edi, ecx
        je     loc_1_378A
        cmp    dword ptr [esi + 0x130], ebx
        ja     loc_1_3782
        mov    al, byte ptr [esi + 0x13c]
        inc    al
        mov    byte ptr [esi + 0x13c], al
        cmp    byte ptr [esi + 0x13e], al
        jne    loc_1_376A
        mov    byte ptr [esi + 0x13d], 0
        mov    byte ptr [esi + 0x13c], 0
        mov    ebx, dword ptr [esi + 0x12c]
        jmp    loc_1_3770
loc_1_376A:
        mov    ebx, dword ptr [esi + 0x138]
loc_1_3770:
        push   esi
        lea    eax, [esi + 0xcb]
        push   0                             ; -> loc_1_37D0
        push   eax
        call   sub_1_4632
loc_1_3782:
        test   edi, edi
        jne    loc_1_3706
loc_1_378A:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_37A0:
        push   ebp
        mov    al, 0x60
        mov    ebp, esp
        push   esi
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 8]
        mov    si, word ptr [ecx + 4]
        lea    edx, [esi + 0x30]
        out    dx, al
        lea    edx, [esi + 0x37]
        in     al, dx
        xor    edx, edx
        mov    byte ptr [ecx + 0xe3], al
        mov    dx, word ptr [ecx + 4]
        mov    al, 0x20
        add    edx, 0x30
        pop    esi
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
loc_1_37D0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 4]
        add    edx, 8
        in     al, dx
        mov    bl, al
        mov    al, byte ptr [ecx + 0x13c]
        and    bl, 0xfc
        or     al, bl
        pop    ebx
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3800:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        mov    di, word ptr [eax + 4]
        mov    cl, 0x18
        mov    al, 0x60
        lea    esi, [edi + 0x30]
        mov    edx, esi
        out    dx, al
loc_1_3819:
        xor    eax, eax
        mov    al, cl
        inc    cl
        cmp    cl, 0x1f
        lea    edx, [eax + edi + 0x20]
        mov    al, 0xff
        out    dx, al
        jbe    loc_1_3819
        mov    edx, esi
        mov    al, 0x20
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 056h
        db  057h, 033h, 0FFh, 066h, 08Bh, 078h, 004h, 0B1h, 018h, 0B0h, 060h, 08Dh, 077h, 030h, 08Bh, 0D6h
        db  0EEh, 033h, 0C0h, 08Ah, 0C1h, 0FEh, 0C1h, 08Dh, 054h, 038h, 020h, 032h, 0C0h, 0EEh, 080h, 0F9h
        db  01Fh, 076h, 0EEh, 08Bh, 0D6h, 0B0h, 020h, 0EEh, 0B0h, 001h, 05Fh, 05Eh, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 055h, 08Bh, 04Ch, 024h, 00Ch, 066h, 083h, 07Ch
        db  024h, 008h, 001h, 08Bh, 0ECh, 08Bh, 091h, 015h, 001h, 000h, 000h, 08Bh, 081h, 021h, 001h, 000h
        db  000h, 0C7h, 081h, 015h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 000h, 08Bh, 089h, 011h
        db  001h, 000h, 000h, 050h, 0B8h, 000h, 000h, 000h, 000h, 083h, 0D0h, 0FFh, 025h, 001h, 000h, 000h
        db  0C0h, 050h, 052h, 051h, 0E8h, 0C7h, 00Ch, 000h, 000h, 066h, 033h, 0C0h, 05Dh, 0C2h, 008h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_38D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x186]
        test   ecx, ecx
        jne    loc_1_38ED
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_38ED:
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x186], eax
        test   eax, eax
        jne    loc_1_3904
        mov    dword ptr [esi + 0x18a], 0
loc_1_3904:
        cmp    byte ptr [ecx + 0x2c], 0
        mov    edi, dword ptr [ecx + 0x28]
        je     loc_1_393F
        cmp    dword ptr [esi + 0x18e], 0
        jne    loc_1_3924
        mov    dword ptr [esi + 0x192], ecx
        mov    dword ptr [esi + 0x18e], ecx
        jmp    loc_1_3933
loc_1_3924:
        mov    eax, dword ptr [esi + 0x192]
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [esi + 0x192], ecx
loc_1_3933:
        mov    dword ptr [ecx + 0x24], 0
        jmp    loc_1_39E1
loc_1_393F:
        mov    ax, word ptr [ebp + 8]
        test   ax, ax
        jne    loc_1_3950
        inc    dword ptr [esi + 0x1a3]
        jmp    loc_1_3956
loc_1_3950:
        inc    dword ptr [esi + 0x1ab]
loc_1_3956:
        cmp    ax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0xc0000001
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_4582
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_39E1
        mov    eax, dword ptr [ebx + 0x172]
        cmp    edi, eax
        jne    loc_1_398E
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x172], eax
        jmp    loc_1_39A1
loc_1_398E:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_399B
loc_1_3993:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_3993
loc_1_399B:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_39A1:
        mov    eax, dword ptr [ebx + 0x20e]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_39B2
        push   ebx
        call   sub_1_0A80
loc_1_39B2:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   0x75
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 0x172], 0
        jne    loc_1_39E1
        push   ebx
        push   0                             ; -> loc_1_22C0
        add    ebx, 0xcb
        push   ebx
        call   sub_1_4632
loc_1_39E1:
        mov    edi, dword ptr [esi + 0x17e]
        test   edi, edi
        je     loc_1_3A85
        cmp    byte ptr [esi + 0x204], 0
        jne    loc_1_3A85
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x17e], eax
        test   eax, eax
        jne    loc_1_3A13
        mov    dword ptr [esi + 0x182], 0
loc_1_3A13:
        push   esi
        push   edi
        call   sub_1_3410
        cmp    ax, 6
        jne    loc_1_3A53
        mov    eax, dword ptr [esi + 0x17e]
        test   eax, eax
        je     loc_1_3A3D
        mov    dword ptr [edi + 0x24], eax
        xor    ax, ax
        mov    dword ptr [esi + 0x17e], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3A3D:
        xor    ax, ax
        mov    dword ptr [esi + 0x17e], edi
        mov    dword ptr [esi + 0x182], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3A53:
        test   ax, ax
        jne    loc_1_3A85
        cmp    dword ptr [esi + 0x186], 0
        jne    loc_1_3A6F
        mov    dword ptr [esi + 0x18a], edi
        mov    dword ptr [esi + 0x186], edi
        jmp    loc_1_3A7E
loc_1_3A6F:
        mov    eax, dword ptr [esi + 0x18a]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x18a], edi
loc_1_3A7E:
        mov    dword ptr [edi + 0x24], 0
loc_1_3A85:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_3A90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 0x19a], 1
        mov    dword ptr [edi + 0x196], 0
        mov    ebx, dword ptr [edi + 0x20e]
        add    ebx, 0xe
        cmp    ebx, esi
        jb     loc_1_3ABC
        mov    ebx, esi
loc_1_3ABC:
        push   edi
        lea    eax, [edi + 0x222]
        push   eax
        push   0
        push   ebx
        call   sub_1_36B0
        test   ax, ax
        je     loc_1_3ADB
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3ADB:
        inc    dword ptr [edi + 0x1a7]
        cmp    esi, 0xe
        jae    loc_1_3B11
        cmp    esi, 6
        jb     loc_1_3B37
        push   0
        lea    eax, [edi + 0x222]
        push   0
        mov    edx, dword ptr [edi + 0x15e]
        push   0
        push   esi
        push   eax
        push   eax
        push   edi
        push   edx
        call   sub_1_4730
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B11:
        sub    esi, 0xe
        sub    ebx, 0xe
        push   esi
        lea    eax, [edi + 0x230]
        push   ebx
        lea    ecx, [edi + 0x222]
        push   eax
        push   0xe
        push   ecx
        mov    eax, dword ptr [edi + 0x15e]
        push   ecx
        push   edi
        push   eax
        call   sub_1_4730
loc_1_3B37:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B50:
        push   ebp
        mov    edx, 0xc80
        in     eax, dx
        mov    ebp, esp
        mov    edx, dword ptr [0]            ; -> loc_1_4D84
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    edx, esi
        je     loc_1_3B7E
        mov    ecx, 0                        ; -> loc_1_4D84
loc_1_3B70:
        cmp    edx, eax
        je     loc_1_3B9E
        mov    edx, dword ptr [ecx + 4]
        add    ecx, 4
        test   edx, edx
        jne    loc_1_3B70
loc_1_3B7E:
        mov    edi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [edi + 4]
        add    edx, 0x25
        in     al, dx
        test   al, 4
        je     loc_1_3BB0
        mov    word ptr [edi + 0x4a], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3B9E:
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [edi + 0x4a], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3BB0:
        cmp    dword ptr [edi + 0x162], 0
        jne    loc_1_3BC8
        mov    word ptr [edi + 0x4a], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3BC8:
        push   3
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    ecx, [ebp - 0x24]
        push   ecx
        call   sub_1_466E
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_1_3BEC
        mov    word ptr [edi + 0x4a], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3BEC:
        push   0x100
        mov    eax, dword ptr [edi + 0x162]
        push   eax
        mov    ecx, dword ptr [ebp - 0x1c]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0x24]
        push   eax
        call   sub_1_467A
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_1_3C27
        mov    word ptr [edi + 0x4a], 5
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_4674
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3C27:
        push   0x100
        mov    eax, dword ptr [edi + 0x162]
        add    eax, 0x100
        mov    ecx, dword ptr [ebp - 0x1c]
        push   eax
        lea    edx, [ebp - 0x18]
        push   ecx
        lea    eax, [ebp - 0x24]
        push   edx
        push   eax
        call   sub_1_467A
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_1_3C70
        mov    word ptr [edi + 0x4a], 5
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_4674
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3C70:
        push   0x100
        mov    eax, dword ptr [edi + 0x162]
        add    eax, 0x200
        mov    ecx, dword ptr [ebp - 0x1c]
        push   eax
        lea    edx, [ebp - 0x20]
        push   ecx
        lea    eax, [ebp - 0x24]
        push   edx
        push   eax
        call   sub_1_467A
        cmp    dword ptr [ebp - 0x24], 0
        je     loc_1_3CC2
        mov    word ptr [edi + 0x4a], 5
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_4674
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3CC2:
        lea    eax, [ebp - 0x28]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        push   edx
        call   sub_1_468C
        lea    ecx, [ebp - 0x28]
        lea    edx, [ebp - 0xc]
        push   ecx
        mov    eax, dword ptr [ebp - 0x20]
        push   edx
        push   eax
        call   sub_1_468C
        lea    ecx, [ebp - 0x28]
        lea    edx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 0x18]
        push   edx
        push   eax
        call   sub_1_468C
        xor    ecx, ecx
loc_1_3CF7:
        mov    eax, dword ptr [ebp - 0x10]
        add    ecx, 4
        mov    dword ptr [eax + ecx - 4], 0xa5a5a5a5
        mov    edx, dword ptr [ebp - 0xc]
        cmp    ecx, 0x100
        mov    dword ptr [edx + ecx - 4], 0x5a5a5a5a
        mov    ebx, dword ptr [edi + 0x1c]
        mov    dword ptr [ebx + ecx - 4], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + ecx - 4], 0
        jb     loc_1_3CF7
        xor    ecx, ecx
        xor    al, al
        mov    cx, word ptr [edi + 4]
        lea    edx, [ecx + 0x30]
        out    dx, al
        lea    edx, [ecx + 0x3f]
        out    dx, al
        lea    edx, [ecx + 0x3c]
        mov    al, 4
        out    dx, al
        mov    eax, dword ptr [edi + 0x1c]
        mov    ecx, 0x47474747
        mov    dword ptr [eax + 0x200], 0xffffffff
        mov    eax, dword ptr [edi + 0x1c]
        mov    word ptr [eax + 0x204], 0xffff
        xor    eax, eax
loc_1_3D64:
        mov    edx, dword ptr [edi + 0x1c]
        add    eax, 4
        cmp    eax, 0x400
        mov    dword ptr [edx + eax + 0x20c], ecx
        jb     loc_1_3D64
        mov    dword ptr [ebp - 4], 1
loc_1_3D7F:
        test   esi, esi
        jne    loc_1_3E2D
        test   byte ptr [ebp - 4], 1
        je     loc_1_3D95
        mov    eax, dword ptr [ebp - 0x14]
        mov    ebx, dword ptr [ebp - 0x10]
        jmp    loc_1_3D9B
loc_1_3D95:
        mov    eax, dword ptr [ebp - 0x20]
        mov    ebx, dword ptr [ebp - 0xc]
loc_1_3D9B:
        push   1
        push   eax
        push   edi
        call   sub_1_3E80
        mov    esi, eax
        test   esi, esi
        jne    loc_1_3E15
        mov    eax, dword ptr [edi + 0x1c]
        cmp    word ptr [eax], 0
        jne    loc_1_3E10
        mov    ecx, 1
        add    ebx, 4
        add    eax, 4
loc_1_3DBE:
        mov    edx, dword ptr [ebx]
        cmp    dword ptr [eax], edx
        jne    loc_1_3E28
        add    ebx, 4
        add    eax, 4
        inc    ecx
        cmp    ecx, 0x3e
        jb     loc_1_3DBE
        push   0
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        push   edi
        call   sub_1_3E80
        mov    esi, eax
        test   esi, esi
        jne    loc_1_3E15
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax], 0
        jne    loc_1_3E10
        mov    edx, 1
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 4
        mov    eax, dword ptr [edi + 0x1c]
        add    eax, 4
loc_1_3DFC:
        mov    ebx, dword ptr [ecx]
        cmp    dword ptr [eax], ebx
        jne    loc_1_3E10
        add    ecx, 4
        add    eax, 4
        inc    edx
        cmp    edx, 0x3e
        jb     loc_1_3DFC
        jmp    loc_1_3E15
loc_1_3E10:
        mov    esi, 1
loc_1_3E15:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        cmp    eax, 0x7d0
        jb     loc_1_3D7F
        jmp    loc_1_3E2D
loc_1_3E28:
        mov    esi, 1
loc_1_3E2D:
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_4674
        test   esi, esi
        je     loc_1_3E64
        mov    word ptr [edi + 0x4a], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3E64:
        mov    word ptr [edi + 0x4a], 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3E80:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x84
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        lea    ecx, [ebp - 0x84]
        push   edi
        xor    esi, esi
        push   eax
        mov    edx, dword ptr [ebp + 0xc]
        push   ecx
        push   edx
        call   sub_1_4692
        mov    edi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [edi + 4]
        lea    edx, [ecx + 0xb]
        in     al, dx
        and    al, 0xbf
        out    dx, al
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 1
        jne    loc_1_3EDA
        lea    edx, [ecx + 0x10]
        mov    eax, dword ptr [ebp - 0x84]
        out    dx, eax
        push   edi
        call   sub_1_3F40
        xor    edx, edx
        mov    eax, 0x280fa
        mov    dx, word ptr [edi + 4]
        add    edx, 0x14
        jmp    loc_1_3EFF
loc_1_3EDA:
        test   eax, eax
        jne    loc_1_3F00
        lea    edx, [ecx + 0x10]
        mov    eax, dword ptr [ebp - 0x84]
        add    eax, 2
        push   edi
        out    dx, eax
        call   sub_1_3F40
        xor    edx, edx
        mov    eax, 0x2c0fa
        mov    dx, word ptr [edi + 4]
        add    edx, 0x14
loc_1_3EFF:
        out    dx, eax
loc_1_3F00:
        xor    ebx, ebx
        xor    ecx, ecx
        mov    cx, word ptr [edi + 4]
        lea    edx, [ecx + 0xb]
loc_1_3F0B:
        in     al, dx
        inc    ebx
        cmp    ebx, 0xffff
        ja     loc_1_3F1B
        test   al, 2
        jne    loc_1_3F0B
        jmp    loc_1_3F20
loc_1_3F1B:
        mov    esi, 1
loc_1_3F20:
        lea    edx, [ecx + 0xa]
        pop    edi
        in     al, dx
        and    al, 0x7f
        out    dx, al
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3F40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 4]
        lea    ecx, [edi + 0x37]
        mov    edx, ecx
        in     al, dx
        test   al, 0x75
        mov    bl, al
        je     loc_1_3F8B
        mov    al, 0x75
        out    dx, al
        lea    edx, [edi + 0x33]
        in     al, dx
        inc    al
        cmp    al, byte ptr [esi + 0x153]
        jb     loc_1_3F72
        mov    al, byte ptr [esi + 0x152]
loc_1_3F72:
        cmp    byte ptr [esi + 0x152], al
        jne    loc_1_3F84
        xor    eax, eax
        mov    al, byte ptr [esi + 0x153]
        jmp    loc_1_3F89
loc_1_3F84:
        and    eax, 0xff
loc_1_3F89:
        dec    eax
        out    dx, al
loc_1_3F8B:
        lea    esi, [edi + 0x30]
        mov    edx, esi
        in     al, dx
        test   al, 4
        jne    loc_1_3FBC
        test   bl, 0x1a
        je     loc_1_3F9F
        mov    edx, ecx
        mov    al, 0x1a
        out    dx, al
loc_1_3F9F:
        lea    edx, [edi + 0x3d]
        mov    al, 4
        out    dx, al
        lea    edx, [edi + 0x34]
        mov    al, 2
        out    dx, al
        lea    edx, [edi + 0x35]
        xor    al, al
        out    dx, al
        lea    edx, [edi + 0x36]
        mov    al, 4
        out    dx, al
        mov    edx, esi
        mov    al, 0x26
        out    dx, al
loc_1_3FBC:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3FD0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        mov    si, word ptr [ecx + 4]
        lea    edx, [esi + 9]
        in     al, dx
        test   al, 8
        je     loc_1_4006
        lea    ecx, [esi + 0x24]
        mov    edx, ecx
        in     al, dx
        mov    bl, al
        or     al, 0x80
        out    dx, al
        lea    edx, [esi + 0x2d]
        in     al, dx
        and    al, 0xfc
        pop    esi
        out    dx, al
        mov    edx, ecx
        mov    al, bl
        out    dx, al
        xor    eax, eax
        pop    ebx
        pop    ebp
        ret    4
loc_1_4006:
        push   ecx
        call   sub_1_4020
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4020:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 4]
        mov    ebp, esp
        add    cx, 0x20
        sub    esp, 4
        mov    al, 0x80
        push   ebx
        push   esi
        push   edi
        movsx  edi, cx
        lea    esi, [ecx + 0x10]
        lea    ecx, [edi + 0x1d]
        mov    dx, si
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    dx, si
        mov    byte ptr [ebp - 2], al
        xor    al, al
        lea    ebx, [edi + 4]
        out    dx, al
        mov    edx, ecx
        mov    eax, ecx
        xor    al, al
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    cl, al
        mov    byte ptr [ebp - 1], al
        and    cl, 0xc3
        or     cl, 0x80
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0xd]
        in     al, dx
        and    al, 0xfd
        or     al, 1
        out    dx, al
        mov    edx, ebx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dx, si
        mov    al, 0xc0
        out    dx, al
        lea    edx, [edi + 0x1f]
        in     al, dx
        mov    dx, si
        mov    byte ptr [ebp - 3], al
        xor    al, al
        out    dx, al
        test   byte ptr [ebp - 3], 4
        je     loc_1_40A7
        mov    edx, ebx
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0xd]
        in     al, dx
        or     al, 3
        out    dx, al
        mov    edx, ebx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        jmp    loc_1_40B0
loc_1_40A7:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_40D0
loc_1_40B0:
        mov    dx, si
        xor    al, al
        out    dx, al
        lea    edx, [edi + 0x1d]
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    cx, word ptr [edi + 4]
        add    cx, 0x20
        movsx  esi, cx
        lea    eax, [ecx + 0x10]
        lea    ecx, [esi + 0x1d]
        mov    dx, ax
        mov    word ptr [ebp - 4], ax
        mov    al, 0x80
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    dx, word ptr [ebp - 4]
        xor    al, al
        out    dx, al
        mov    edx, ecx
        out    dx, al
        lea    edx, [esi + 4]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 0xc3
        or     al, 0x80
        out    dx, al
        lea    edx, [esi + 0xd]
        mov    byte ptr [ebp - 8], al
        in     al, dx
        and    al, 0xfd
        or     al, 1
        out    dx, al
        lea    edx, [esi + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    edx, esi
        push   0x66
        in     al, dx
        or     al, 0x40
        push   0                             ; -> loc_1_4DBC
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [edi + 0x15c]
        out    dx, al
        mov    edx, dword ptr [edi + 0x1c]
        push   edx
        call   sub_1_4A20
        lea    edx, [esi + 5]
        mov    al, byte ptr [edi + 0x15d]
        out    dx, al
        lea    edi, [esi + 0x17]
        mov    al, 0xff
        mov    edx, edi
        out    dx, al
        mov    dx, word ptr [ebp - 4]
        in     al, dx
        and    eax, 0xff
        and    al, 0x3b
        out    dx, al
        lea    edx, [esi + 0x14]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0x15]
        out    dx, al
        lea    edx, [esi + 0x16]
        mov    al, 4
        out    dx, al
        mov    dx, word ptr [ebp - 4]
        xor    ebx, ebx
        in     al, dx
        and    eax, 0xff
        or     al, 6
        out    dx, al
loc_1_4181:
        push   0x64
        call   sub_1_4638
        mov    edx, edi
        in     al, dx
        test   al, 0xa
        jne    loc_1_4198
        inc    ebx
        cmp    ebx, 0xbb8
        jl     loc_1_4181
loc_1_4198:
        lea    eax, [esi + 0x14]
        mov    edx, eax
        in     al, dx
        mov    edx, edi
        mov    byte ptr [ebp - 2], al
        mov    al, 0xff
        xor    ebx, ebx
        out    dx, al
        mov    dx, word ptr [ebp - 4]
        mov    al, 0x21
        out    dx, al
loc_1_41AF:
        push   0xa
        call   sub_1_4638
        mov    edx, edi
        in     al, dx
        test   al, 0x80
        jne    loc_1_41C6
        inc    ebx
        cmp    ebx, 0xa0
        jl     loc_1_41AF
loc_1_41C6:
        test   byte ptr [ebp - 2], 0x10
        je     loc_1_41E1
        lea    edx, [esi + 4]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        lea    edx, [esi + 0xd]
        in     al, dx
        or     al, 3
        out    dx, al
        lea    edx, [esi + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_1_41E1:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 4]
        add    di, 0x20
        mov    dx, di
        lea    eax, [edi + 0x10]
        mov    word ptr [ebp - 2], ax
        push   1
        in     al, dx
        mov    bl, al
        and    bl, 0x3f
        mov    al, bl
        or     al, 0x80
        out    dx, al
        call   sub_1_4638
        mov    dx, di
        mov    al, bl
        out    dx, al
        mov    eax, dword ptr [esi + 0x18]
        shr    eax, 0xd
        and    al, 0x3f
        or     al, 0x40
        out    dx, al
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0xa
        in     al, dx
        or     al, 0x40
        out    dx, al
        mov    cl, al
        xor    ebx, ebx
        in     al, dx
        mov    al, cl
        out    dx, al
        movsx  edi, di
        mov    dx, word ptr [ebp - 2]
        mov    al, 0x21
        out    dx, al
loc_1_4250:
        lea    edx, [edi + 0x17]
        in     al, dx
        test   al, 0x80
        jne    loc_1_4268
        push   1
        inc    ebx
        call   sub_1_4638
        cmp    ebx, 0xa0
        jb     loc_1_4250
loc_1_4268:
        cmp    ebx, 0xa0
        jb     loc_1_427E
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_427E:
        lea    edx, [edi + 0x1e]
        mov    al, 0x49
        out    dx, al
        xor    edx, edx
        xor    al, al
        mov    dx, word ptr [esi + 4]
        add    edx, 0x3b
        out    dx, al
        out    dx, al
        lea    edx, [edi + 0x1d]
        mov    al, 4
        out    dx, al
        mov    ax, word ptr [esi + 0x3a]
        cmp    ax, 2
        ja     loc_1_42A6
        test   ax, ax
        jne    loc_1_42AC
loc_1_42A6:
        mov    word ptr [esi + 0x3a], 2
loc_1_42AC:
        mov    cx, word ptr [esi + 0x3a]
        lea    edx, [edi + 0x11]
        imul   cx, cx, 0x600
        mov    al, ch
        out    dx, al
        mov    ax, word ptr [esi + 0x16]
        lea    edx, [edi + 0x12]
        shl    al, 2
        out    dx, al
        lea    edx, [edi + 0x13]
        mov    al, cl
        out    dx, al
        lea    eax, [ecx + 1]
        lea    edx, [edi + 0x17]
        out    dx, al
        mov    eax, edx
        lea    ebx, [edi + 4]
        mov    al, 0xff
        out    dx, al
        lea    edx, [edi + 0x1f]
        mov    al, byte ptr [esi + 0xc9]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    cl, al
        or     cl, 0x80
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0xb]
        xor    al, al
        out    dx, al
        lea    edx, [edi + 0xd]
        in     al, dx
        or     al, 0x6c
        out    dx, al
        lea    edx, [edi + 5]
        in     al, dx
        or     al, 0x80
        out    dx, al
        mov    dx, word ptr [ebp - 2]
        mov    al, 0x21
        out    dx, al
        mov    edx, edi
        mov    al, 0x40
        out    dx, al
        in     al, dx
        mov    al, 0x40
        lea    edi, [esi + 0xcb]
        out    dx, al
        mov    al, cl
        mov    edx, ebx
        and    al, 0x7f
        xor    ebx, ebx
        out    dx, al
loc_1_4323:
        push   esi
        mov    byte ptr [esi + 0x13c], bl
        push   0                             ; -> loc_1_37D0
        push   edi
        call   sub_1_4632
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0x20]
        test   eax, 0x3fffff
        jbe    loc_1_435A
loc_1_4344:
        mov    eax, dword ptr [esi + 0x1c]
        inc    ecx
        mov    byte ptr [eax + ecx - 1], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 0x20]
        shl    eax, 0xa
        cmp    eax, ecx
        ja     loc_1_4344
loc_1_435A:
        inc    ebx
        cmp    ebx, 3
        jb     loc_1_4323
        mov    byte ptr [esi + 0x13c], 0
        push   esi
        push   0                             ; -> loc_1_37D0
        push   edi
        call   sub_1_4632
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_4380:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0x27]
        in     al, dx
        cmp    al, 0x21
        je     loc_1_441C
        lea    edx, [ecx + 0xb]
        mov    edi, 0x12c
        in     al, dx
        mov    bl, al
        or     al, 8
        out    dx, al
loc_1_43A9:
        push   0x3e8
        call   sub_1_4638
        dec    edi
        jne    loc_1_43A9
        xor    ecx, ecx
        mov    al, bl
        mov    cx, word ptr [esi + 4]
        and    al, 0xf7
        xor    edi, edi
        lea    edx, [ecx + 0xb]
        out    dx, al
        lea    edx, [ecx + 0x21]
        in     al, dx
        test   al, 0x40
        je     loc_1_43F2
loc_1_43CE:
        cmp    edi, 0x12c
        jge    loc_1_43F2
        xor    edx, edx
        push   0x3e8
        mov    dx, word ptr [esi + 4]
        inc    edi
        add    edx, 0x21
        in     al, dx
        mov    bl, al
        call   sub_1_4638
        test   bl, 0x40
        jne    loc_1_43CE
loc_1_43F2:
        mov    dx, word ptr [esi + 4]
        mov    esi, edx
        and    esi, 0xffff
        in     al, dx
        lea    edx, [esi + 1]
        lea    ecx, [esi + 0x24]
        in     al, dx
        lea    edx, [esi + 0x25]
        mov    al, 0x84
        out    dx, al
        mov    edx, ecx
        mov    al, 0xa0
        out    dx, al
        lea    edx, [esi + 0x2d]
        mov    al, 0x6d
        out    dx, al
        mov    edx, ecx
        mov    al, 0x20
        out    dx, al
loc_1_441C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4430:
        mov    al, byte ptr [0]              ; -> obj1:0x4ACC
        cmp    al, 0
        jne    loc_1_4455
        inc    byte ptr [0]                  ; -> obj1:0x4ACC
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_4457
        push   0
        push   0
        call   0
        cmp    eax, 0
        jne    loc_1_4457
loc_1_4455:
        clc    
        ret    
loc_1_4457:
        stc    
        ret    
SMC8232W_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_4468
        jmp    dword ptr [eax*4]             ; -> obj1:0x446A
loc_1_4468:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_4430
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4468
        dd  loc_1_4430
        db  0CCh, 0CCh, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh
        db  0F3h, 06Eh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh
        db  054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h
        db  08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h
        db  08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh
        db  0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h
        db  0FCh, 0F3h, 066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h
        db  00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 0CDh, 020h, 031h, 080h
        db  028h, 000h
sub_1_4558:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_4564:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_456A:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_4570:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_457C:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_4582:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_458E:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_459A:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_45A0:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_45A6:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_45AC:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
sub_1_45BE:
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_45C4:
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
        int3   
        int3   
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_461A:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_4620:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_4626:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_462C:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_4632:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_4638:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_463E:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_4644:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_4668:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_466E:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_4674:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_467A:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
sub_1_4686:
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_468C:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
sub_1_4692:
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_46B0:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_46BC:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_46C2:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_46C8:
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
sub_1_470A:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_4710:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
        int3   
sub_1_4730:
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
        call   sub_1_4760
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_4760:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_4800
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_47A0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_4795
        cmp    al, 0xff
        jne    loc_1_4795
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_4795
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_4807
loc_1_4795:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_4807
        db  08Bh, 0FFh
loc_1_47A0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_47F0
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_47C6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_47CD
loc_1_47C6:
        mov    dword ptr [ebp - 0x10], 1
loc_1_47CD:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_47E0
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_480A
        db  08Dh, 064h, 024h, 000h
loc_1_47E0:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_480A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_47F0:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_480A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4800:
        mov    dword ptr [ebp - 4], 0x20
loc_1_4807:
        mov    esi, dword ptr [ebp + 8]
loc_1_480A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_4901
loc_1_4815:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_486D
        test   al, 2
        je     loc_1_4859
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
        or     al, al
        je     loc_1_4859
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_486D
loc_1_4859:
        test   byte ptr [ebp - 4], 2
        je     loc_1_4864
        test   bl, 4
        jne    loc_1_486D
loc_1_4864:
        test   bl, 0x20
        je     loc_1_48F6
loc_1_486D:
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
        call   sub_1_459A
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_48F6
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_48AF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_48AF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_48C0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_48C6
        db  08Bh, 0FFh
loc_1_48C0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_48C6:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_48D5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_45A0
loc_1_48D5:
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
        call   sub_1_4644
        mov    edi, ebx
        jmp    loc_1_48F9
loc_1_48F6:
        mov    edi, dword ptr [edi + 0x10]
loc_1_48F9:
        or     edi, edi
        jne    loc_1_4815
loc_1_4901:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4910:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4920
        pop    ebp
        ret    4
sub_1_4920:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_499E
loc_1_4930:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_4997
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_45A0
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_4997
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_495C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_495C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_4970
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_4976
        db  005h, 000h, 000h, 000h, 000h
loc_1_4970:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_4976:
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
        call   sub_1_4644
        mov    edi, ebx
        jmp    loc_1_499A
loc_1_4997:
        mov    edi, dword ptr [edi + 0x10]
loc_1_499A:
        or     edi, edi
        jne    loc_1_4930
loc_1_499E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_49C5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_49C5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_4A01
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_49F2
        cmp    bl, 0xff
        jne    loc_1_49F2
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_49F2
        test   al, 8
        je     loc_1_49EB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_49EB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_49F2:
        test   al, 4
        jne    loc_1_49FA
        test   al, 2
        je     loc_1_4A19
loc_1_49FA:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4A01:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_4A19
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_4A19
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4A19:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_4A20:
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
sub_1_4A40:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_4A60
        or     ecx, ecx
        je     loc_1_4A76
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_4A74
loc_1_4A60:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_4A74
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_4A74:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_4A76:
        popfd  
        pop    edi
        pop    esi
        ret    0xc
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SMC8232W'                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  SMC8232W_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 0CCh, 0CCh, 0CCh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h, 000h, 040h, 028h, 023h, 029h, 020h, 053h, 04Dh, 043h, 038h, 032h, 033h, 032h
        db  057h, 02Eh, 056h, 058h, 044h, 020h, 076h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 020h, 031h, 02Eh
        db  030h, 036h, 061h, 020h, 030h, 033h, 02Fh, 031h, 036h, 02Fh, 039h, 035h, 00Ah, 043h, 06Fh, 070h
        db  079h, 072h, 069h, 067h, 068h, 074h, 020h, 028h, 043h, 029h, 020h, 053h, 074h, 061h, 06Eh, 064h
        db  061h, 072h, 064h, 020h, 04Dh, 069h, 063h, 072h, 06Fh, 073h, 079h, 073h, 074h, 065h, 06Dh, 073h
        db  020h, 043h, 06Fh, 072h, 070h, 02Ch, 020h, 031h, 039h, 039h, 034h, 000h, 040h, 028h, 023h, 029h
        db  020h, 055h, 04Dh, 05Fh, 038h, 032h, 033h, 032h, 05Fh, 04Eh, 044h, 049h, 053h, 033h, 05Fh, 073h
        db  032h, 02Eh, 032h, 05Fh, 076h, 031h, 02Eh, 030h, 036h, 061h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4BA4:
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
loc_1_4C20:
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
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [edx], dh
        xor    esi, dword ptr [edx]
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4C88:
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [edx], dh
        xor    esi, dword ptr [edx]
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
loc_1_4CB4:
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
loc_1_4CF0:
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
        push   ebx
        dec    esp
        dec    edi
        push   esp
        dec    esi
        push   ebp
        dec    ebp
        inc    edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        push   ebx
        dec    esp
        dec    edi
        push   esp
        dec    esi
        push   ebp
        dec    ebp
        inc    edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        inc    edx
        jne    0x4d8a
        push   esp
        jns    0x4d8a
        add    byte ptr gs:[edx + 0x75], al
        jae    loc_1_4D74
        jns    0x4d92
        add    byte ptr gs:[ebp + 0x65], cl
        imul   esp, dword ptr fs:[ecx + 0x54], 0x657079
        add    byte ptr [eax], al
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 0x54], 0x657079
        add    byte ptr [eax], al
        dec    ebp
        popal  
        js     loc_1_4DA9
        insd   dword ptr es:[edi], dx
        jne    0x4db0
        push   eax
        popal  
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_4D9D
        imul   edi, dword ptr [edx + 0x65], 0x4d000000
        popal  
        js     0x4dbd
        insd   dword ptr es:[edi], dx
        jne    0x4dc4
        push   eax
        popal  
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_4DB1
        imul   edi, dword ptr [edx + 0x65], 0x53000000
        dec    ebp
        inc    ebx
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     loc_1_4DE1
        jb     loc_1_4D9E
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4D74:
        add    byte ptr [edx], al
        add    eax, dword ptr [eax + 6]
        add    al, byte ptr [ebx]
        add    eax, 0xf0b0a07
loc_1_4D84:
        and    eax, 0x10f130d4
        mov    dword ptr [0x23350911], eax
        add    byte ptr [0x4002335], al
        dec    ebp
        leave  
        stosb  byte ptr es:[edi], al
        rcr    dword ptr [edi + 0x33], 0x70
        add    dword ptr [eax], edx
loc_1_4D9D:
        lodsb  al, byte ptr [esi]
loc_1_4D9E:
        add    byte ptr [edx + 0x22], al
        db  0F0h, 0C0h, 081h, 022h, 0F0h, 0C0h, 071h, 00Eh
loc_1_4DA9:
        adc    dword ptr [0x5110e19], edx
        and    dword ptr [eax], edx
loc_1_4DB1:
        lodsb  al, byte ptr [esi]
        add    byte ptr [eax + 5], cl
        xchg   edx, eax
        add    eax, dword ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4DBC:
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4d], dl
        inc    ebx
        and    byte ptr [ecx + 0x75], al
        je     0x4e3f
        and    byte ptr [ebp + 0x65], cl
        imul   esp, dword ptr fs:[ecx + 0x20], 0x65746544
        arpl   word ptr [eax + 0x54], si
        db  065h, 073h
loc_1_4DE1:
        je     0x4e03
        inc    esi
        jb     0x4e47
        insd   dword ptr es:[edi], dx
        and    byte ptr cs:[eax + ebp*2 + 0x61], dl
        outsb  dx, byte ptr [esi]
        imul   esp, dword ptr [eax], 0x79
        outsd  dx, dword ptr [esi]
        jne    loc_1_4E14
        outsw  dx, word ptr [esi]
        jb     loc_1_4E18
        jne    0x4e6d
        imul   ebp, dword ptr [esi + 0x67], 0x434d5320
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jne    0x4e6b
        je     0x4e7d
        and    byte ptr cs:[0x524b5357], ch
        push   esp
        inc    esp
        inc    esp
loc_1_4E14:
        pop    edx
        push   eax
        dec    ebp
        inc    edx
loc_1_4E18:
        inc    edi
        push   ebx
        inc    esi
        dec    ebp
        push   esp
        dec    edx
        inc    edi
        push   esi
        push   eax
        sub    eax, 0xffff0000
        db  0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0]            ; -> obj1:0x4E24
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_463E
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0034
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0034:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_004E
        mov    esi, 2
loc_2_004E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_463E
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_0082
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_0082:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_463E
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_00B1
        push   eax
        call   sub_2_0120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_00B1:
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
sub_2_0120:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_0140
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_4644
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_4644
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_4644
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_0189
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0189:
        mov    eax, dword ptr [0]            ; -> obj1:0x4E24
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_463E
        or     eax, eax
        je     loc_2_01A9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_01A9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_01B0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_01C3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_01B0
        jmp    loc_2_01D2
loc_2_01C3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_01D2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_01E8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_01E8:
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
sub_2_0240:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_05E0
        mov    edi, eax
        or     edi, edi
        je     loc_2_0269
        cmp    edi, 0x103
        jne    loc_2_0280
loc_2_0269:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   ebx
        call   sub_2_0300
        or     eax, eax
        je     loc_2_0280
        mov    edi, eax
loc_2_0280:
        or     edi, edi
        je     loc_2_028C
        cmp    edi, 0x103
        jne    loc_2_02EB
loc_2_028C:
        mov    eax, dword ptr [esi + 0xc]
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        jne    loc_2_02E6
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_02A2
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_02A2:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_02B1
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_02B7
loc_2_02B1:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x10], eax
loc_2_02B7:
        cmp    byte ptr [esi + 0x18], 0
        je     loc_2_02C6
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_45A0
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [esi + 0x19]
        shl    eax, cl
        or     dword ptr [ebx + 0x28], eax
        push   0
        push   0x1a
        push   esi
        call   sub_1_4644
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_02E6:
        mov    edi, 0xc001000e
loc_2_02EB:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0300:
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
        call   sub_1_4A40
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_4A40
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_0384
        sub    eax, eax
        nop    
loc_2_0360:
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
        ja     loc_2_0360
loc_2_0384:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_03EF
loc_2_0391:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_03E4
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
        call   sub_1_4A40
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
        call   sub_1_4A40
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_03E7
loc_2_03E4:
        inc    dword ptr [ebp - 4]
loc_2_03E7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_0391
loc_2_03EF:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_04DA
        mov    edi, dword ptr [ebp + 0x18]
loc_2_0403:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_07C0
        or     al, al
        je     loc_2_043A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_04C8
loc_2_043A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_0514
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
        call   sub_1_4A40
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
        call   sub_1_4A40
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
loc_2_04C8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_0403
loc_2_04DA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        cmp    dword ptr [ebp - 0xc], edx
        je     loc_2_0565
        mov    edi, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_04F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_056B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_0552
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    loc_2_0552
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_0559
loc_2_0514:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4A40
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_4A40
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_0552:
        mov    dword ptr [ebp - 8], 1
loc_2_0559:
        add    edi, 6
        add    ecx, 6
        inc    edx
        cmp    dword ptr [ebp - 0xc], edx
        ja     loc_2_04F0
loc_2_0565:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_05D0
loc_2_056B:
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
        je     loc_2_05D2
        cmp    edi, 0x103
        je     loc_2_05D2
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4A40
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4A40
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_05D0:
        sub    edi, edi
loc_2_05D2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_05E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        mov    dword ptr [edi + 0xc], 0
        je     loc_2_061D
        lea    esp, [esp]
loc_2_0610:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0610
loc_2_061D:
        mov    ecx, dword ptr [edi + 0xc]
        cmp    esi, ecx
        je     loc_2_0651
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 0x18]
        or     eax, eax
        je     loc_2_0653
        cmp    eax, 0x103
        je     loc_2_0653
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [edi + 0xc], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0651:
        sub    eax, eax
loc_2_0653:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_0660:
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
        je     loc_2_0692
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_0687:
        test   dword ptr [esi], edx
        je     loc_2_068C
        inc    eax
loc_2_068C:
        add    esi, 4
        dec    edi
        jne    loc_2_0687
loc_2_0692:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_06A0:
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
        jbe    loc_2_071C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_06E0:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_070A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_0736
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_4A40
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_070A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_06E0
loc_2_071C:
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
loc_2_0736:
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
sub_2_0760:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_0790
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0790:
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
        call   sub_1_4A40
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_07C0:
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
        je     loc_2_0859
        dec    esi
        cmp    esi, edx
        jb     loc_2_0859
        nop    
loc_2_07E0:
        cmp    edx, ecx
        jb     loc_2_0859
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_0803
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0803:
        jbe    loc_2_0810
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_0837
        db  08Bh, 0FFh
loc_2_0810:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_0827
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0837
loc_2_0827:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_0837
        mov    dword ptr [ebp - 4], 0
loc_2_0837:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0869
        jle    loc_2_0848
        or     edx, edx
        je     loc_2_0859
        lea    esi, [edx - 1]
        jmp    loc_2_084B
loc_2_0848:
        lea    ecx, [edx + 1]
loc_2_084B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_07E0
loc_2_0859:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0869:
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

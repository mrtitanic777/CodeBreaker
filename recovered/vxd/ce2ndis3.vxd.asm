; CE2NDIS3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 8310 instructions recovered by recursive descent

Declare_Virtual_Device CE2NDIS3, 3, 10, CE2NDIS3_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x5864  R-X
        db  055h, 08Bh, 054h, 024h, 008h, 0ECh, 08Bh, 0ECh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0010:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        in     ax, dx
        mov    ebp, esp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h
sub_1_0030:
        push   ebp
        mov    eax, 9
        mov    ebp, esp
        sub    esp, 0x4c
        mov    word ptr [ebp - 0x14], ax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ebp - 0x12], ax
        push   0
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0x10], 0     ; -> obj1:0x5370
        push   eax
        call   sub_1_4FFA
        push   -1
        lea    ecx, [ebp - 0xc]
        push   0
        push   8
        push   ecx
        call   sub_1_50E6
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_1_0085
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0085:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x4c], 3
        mov    byte ptr [ebp - 0x4b], 0
        mov    dword ptr [ebp - 0x4a], 0
        mov    dword ptr [ebp - 0x46], 0     ; -> loc_1_09A0
        mov    dword ptr [ebp - 0x42], 0     ; -> loc_1_17E0
        mov    dword ptr [ebp - 0x3e], 0     ; -> loc_1_0C90
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_1_0E20
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_1_16D0
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_1_0EF0
        mov    dword ptr [ebp - 0x2e], 0     ; -> loc_1_1490
        mov    edx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp - 0x10]
        push   0x36
        mov    dword ptr [ebp - 0x1e], edx
        lea    ecx, [ebp - 0x4c]
        mov    dword ptr [ebp - 0x1a], eax
        push   ecx
        mov    edx, dword ptr [ebp - 0xc]
        push   edx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0x2a], 0     ; -> loc_1_18F0
        mov    dword ptr [ebp - 0x26], 0     ; -> loc_1_0130
        mov    dword ptr [ebp - 0x22], 0     ; -> loc_1_1760
        lea    eax, [edx + 4]
        push   eax
        push   ecx
        call   sub_1_5006
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_011A
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5000
loc_1_011A:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0130:
        push   ebp
        mov    eax, 0xc
        mov    ebp, esp
        sub    esp, 0x80
        mov    word ptr [ebp - 0x60], ax
        mov    ecx, 0x10
        mov    word ptr [ebp - 0x5e], ax
        push   esi
        mov    word ptr [ebp - 0x58], cx
        push   edi
        mov    word ptr [ebp - 0x56], cx
        mov    eax, 9
        mov    dword ptr [ebp - 0x5c], 0     ; -> obj1:0x5388
        mov    dword ptr [ebp - 0x54], 0     ; -> obj1:0x53C4
        mov    dword ptr [ebp - 0x4c], 0     ; -> loc_1_53E0
        mov    dword ptr [ebp - 0x44], 0     ; -> loc_1_546C
        mov    word ptr [ebp - 0x50], ax
        mov    word ptr [ebp - 0x4e], ax
        mov    eax, 0x15
        mov    dword ptr [ebp - 0x3c], 0     ; -> obj1:0x5490
        mov    word ptr [ebp - 0x48], ax
        mov    word ptr [ebp - 0x46], ax
        mov    eax, 0xb
        mov    dword ptr [ebp - 0x34], 0     ; -> obj1:0x54A4
        mov    word ptr [ebp - 0x40], ax
        mov    word ptr [ebp - 0x3e], ax
        mov    eax, 8
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj1:0x54BC
        mov    word ptr [ebp - 0x38], ax
        mov    dword ptr [ebp - 0x24], 0     ; -> loc_1_54DC
        mov    word ptr [ebp - 0x36], ax
        push   -1
        mov    word ptr [ebp - 0x28], cx
        mov    eax, 0xe
        mov    word ptr [ebp - 0x30], ax
        push   0
        mov    word ptr [ebp - 0x2e], ax
        push   0x707
        mov    word ptr [ebp - 0x26], cx
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_50E6
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_1_04E4
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0x1c1
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 4]
        push   esi
        lea    eax, [ebp - 0x10]
        push   eax
        xor    edi, edi
        mov    dword ptr [ecx + 0x48], edx
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_5164
        cmp    dword ptr [ebp - 4], edi
        jne    loc_1_04CF
        push   edi
        lea    eax, [ebp - 0x60]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], edi
        jne    loc_1_04C6
        mov    word ptr [ebp - 0x80], di
        lea    eax, [ebp - 0x7e]
        lea    ecx, [ebp - 0x1c]
        lea    edx, [ebp - 0x20]
        push   ecx
        mov    dword ptr [eax], edi
        push   edx
        mov    dword ptr [eax + 4], edi
        push   0x10
        mov    dword ptr [eax + 8], edi
        mov    word ptr [eax + 0xc], di
        lea    ecx, [ebp - 4]
        lea    eax, [ebp - 0x80]
        push   eax
        push   2
        push   esi
        push   ecx
        call   sub_1_50B4
        add    esp, 0x1c
        cmp    dword ptr [ebp - 4], edi
        jne    loc_1_04C6
        mov    ecx, dword ptr [ebp - 0x7c]
        xor    eax, eax
        mov    ax, word ptr [ebp - 0x7a]
        and    ecx, 0xffff
        sub    eax, ecx
        cmp    eax, 0xf
        jne    loc_1_029E
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x6c], ecx
loc_1_029E:
        lea    edi, [ebp - 0x7e]
        xor    eax, eax
        mov    word ptr [ebp - 0x80], 0
        mov    ecx, 7
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    eax, [ebp - 0x1c]
        lea    ecx, [ebp - 0x20]
        push   eax
        lea    edx, [ebp - 0x80]
        push   ecx
        lea    eax, [ebp - 4]
        push   0x20
        push   edx
        push   1
        push   esi
        push   eax
        call   sub_1_50B4
        add    esp, 0x1c
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_04C6
        mov    eax, dword ptr [ebp - 0x7c]
        mov    ecx, dword ptr [ebp - 8]
        push   1
        mov    dword ptr [ecx + 0x74], eax
        mov    edx, dword ptr [ebp - 0x6c]
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [ebp - 0x58]
        mov    dword ptr [ecx + 0x7c], edx
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   edx
        lea    ecx, [ebp - 0xc]
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0329
        push   1
        lea    eax, [ebp - 0x50]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_04C6
loc_1_0329:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        push   0
        mov    edx, dword ptr [eax + 4]
        lea    eax, [ebp - 0x48]
        mov    dword ptr [ecx + 0x84], edx
        push   eax
        mov    edx, dword ptr [ebp - 0x10]
        push   edx
        lea    ecx, [ebp - 0xc]
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0361
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x10], 9
        jmp    loc_1_036D
loc_1_0361:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0x10], edx
loc_1_036D:
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x14]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        call   sub_1_506C
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_03A9
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_1_03A9
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x14]
        add    edx, 0xa
        mov    eax, ecx
        mov    edi, dword ptr [ecx]
        mov    dword ptr [edx], edi
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], cl
loc_1_03A9:
        mov    eax, dword ptr [ebp - 8]
        push   1
        lea    ecx, [ebp - 0x40]
        mov    byte ptr [eax + 0x92], 0
        push   ecx
        mov    edx, dword ptr [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_03E7
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 8]
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 0x92], cl
loc_1_03E7:
        mov    eax, dword ptr [ebp - 8]
        push   1
        lea    ecx, [ebp - 0x38]
        mov    byte ptr [eax + 0x94], 0
        push   ecx
        mov    edx, dword ptr [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0425
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 8]
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 0x94], cl
loc_1_0425:
        mov    eax, dword ptr [ebp - 8]
        push   1
        lea    ecx, [ebp - 0x30]
        mov    byte ptr [eax + 0x95], 1
        push   ecx
        mov    edx, dword ptr [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0463
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 8]
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 0x95], cl
loc_1_0463:
        mov    eax, dword ptr [ebp - 8]
        push   1
        lea    ecx, [ebp - 0x28]
        mov    byte ptr [eax + 0x93], 0
        push   ecx
        mov    edx, dword ptr [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_04A1
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 8]
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 0x93], cl
loc_1_04A1:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_5170
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        call   sub_1_04F0
        test   eax, eax
        jne    loc_1_04CF
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_04C6:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_5170
loc_1_04CF:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x707
        push   eax
        call   sub_1_50EC
        mov    eax, 0xc0000001
loc_1_04E4:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_04F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x600
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   -1
        lea    eax, [ebp - 8]
        push   ebx
        push   0x26
        push   eax
        call   sub_1_50E6
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    loc_1_0633
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x16], ecx
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 0x6c]
        mov    dword ptr [edx + 0x1a], eax
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 0x1e], 0x10
        mov    edx, dword ptr [ebp - 8]
        lea    eax, [esi + 0x70]
        mov    dword ptr [edx + 0x22], eax
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xa], ecx
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x10]
        push   edx
        push   eax
        push   ecx
        mov    edx, dword ptr [esi + 0x48]
        push   esi
        lea    eax, [esi + 0x4c]
        push   edx
        push   eax
        call   sub_1_5012
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   0x26
        mov    dword ptr [ebp - 4], eax
        push   ecx
        call   sub_1_50EC
        cmp    dword ptr [ebp - 4], ebx
        jne    loc_1_0633
        push   esi
        call   sub_1_2260
        mov    dword ptr [esi], eax
        cmp    eax, ebx
        je     loc_1_0633
        mov    ecx, 0                        ; -> loc_1_1D00
        mov    dword ptr [esi + 0x2c], ebx
        mov    dword ptr [esi + 0x30], ecx
        mov    dword ptr [esi + 0x34], esi
        mov    dword ptr [esi + 0x18], ebx
        mov    dword ptr [esi + 0x14], ebx
        push   2
        mov    eax, dword ptr [esi + 0x84]
        push   ebx
        mov    dword ptr [esi + 0x38], ebx
        push   eax
        mov    dword ptr [esi + 0x3c], ecx
        push   eax
        mov    dword ptr [esi + 0x40], esi
        push   ecx
        lea    edx, [esi + 0x50]
        push   esi
        mov    ecx, dword ptr [esi + 0x4c]
        push   0                             ; -> loc_1_0B80
        lea    eax, [ebp - 4]
        push   ecx
        push   edx
        push   eax
        call   sub_1_50CE
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_05FA
        push   ebx
        mov    eax, dword ptr [esi + 0x4c]
        push   0xaa000005
        push   eax
        call   sub_1_51B8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_05FA:
        lea    edi, [esi + 0x78]
        push   0x1000
        mov    eax, dword ptr [esi + 0x74]
        mov    ecx, dword ptr [esi + 0x4c]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   edi
        push   edx
        call   sub_1_51B2
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_063F
loc_1_061A:
        mov    dword ptr [ebp - 4], 0xc0000001
loc_1_0621:
        mov    eax, dword ptr [esi + 0x4c]
        push   eax
        call   sub_1_507E
        mov    eax, dword ptr [esi + 0x4c]
        push   eax
        call   sub_1_5018
loc_1_0633:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_063F:
        push   0x1000
        mov    eax, dword ptr [esi + 0x7c]
        push   eax
        mov    ecx, dword ptr [esi + 0x4c]
        push   ecx
        lea    edx, [esi + 0x80]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_51B2
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_061A
        mov    dword ptr [ebp - 0x304], 0x13
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 0x300], 2
        je     loc_1_06D3
        mov    dword ptr [ebp - 0x304], 0x19
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    dword ptr [ebp - 0x600], 0x16
        cmp    byte ptr [esi + 0x95], 0
        mov    eax, dword ptr [ebp - 0x300]
        je     loc_1_06BB
        or     eax, 2
        jmp    loc_1_06BE
loc_1_06BB:
        and    eax, 0xfffffffd
loc_1_06BE:
        mov    dword ptr [ebp - 0x5fc], eax
        lea    eax, [ebp - 0x600]
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_2820
loc_1_06D3:
        mov    dword ptr [ebp - 0x304], 0x15
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 0x300], 1
        je     loc_1_0744
        mov    dword ptr [ebp - 0x304], 0x1b
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    dword ptr [ebp - 0x600], 0x18
        cmp    byte ptr [esi + 0x94], 0
        mov    eax, dword ptr [ebp - 0x300]
        je     loc_1_072C
        or     eax, 1
        jmp    loc_1_072F
loc_1_072C:
        and    eax, 0xfffffffe
loc_1_072F:
        mov    dword ptr [ebp - 0x5fc], eax
        lea    eax, [ebp - 0x600]
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_2820
loc_1_0744:
        mov    dword ptr [ebp - 0x304], 0x15
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 0x300], 0x10
        je     loc_1_07B5
        mov    dword ptr [ebp - 0x304], 0x1b
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    dword ptr [ebp - 0x600], 0x18
        cmp    byte ptr [esi + 0x92], 0
        mov    eax, dword ptr [ebp - 0x300]
        je     loc_1_079D
        or     eax, 0x10
        jmp    loc_1_07A0
loc_1_079D:
        and    eax, 0xffffffef
loc_1_07A0:
        mov    dword ptr [ebp - 0x5fc], eax
        lea    eax, [ebp - 0x600]
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_2820
loc_1_07B5:
        xor    ebx, ebx
        lea    ecx, [ebp - 0x304]
        mov    dword ptr [ebp - 0x304], 0x2e
        push   ecx
        mov    dword ptr [ebp - 0x300], ebx
        mov    eax, dword ptr [esi + 0x6c]
        mov    dword ptr [ebp - 0x2fc], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        mov    dword ptr [ebp - 0x300], 8
        mov    ecx, dword ptr [esi + 0x84]
        lea    edx, [ebp - 0x304]
        push   edx
        mov    dword ptr [ebp - 0x2fc], ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 0x300], 4
        mov    ecx, dword ptr [edi]
        lea    edx, [ebp - 0x304]
        push   edx
        mov    edi, 0x1000
        mov    dword ptr [ebp - 0x2fc], ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 0x300], 6
        lea    ecx, [ebp - 0x304]
        mov    dword ptr [ebp - 0x2fc], edi
        push   ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 0x300], 5
        mov    ecx, dword ptr [esi + 0x80]
        lea    edx, [ebp - 0x304]
        push   edx
        mov    dword ptr [ebp - 0x2fc], ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 0x300], 7
        lea    ecx, [ebp - 0x304]
        mov    dword ptr [ebp - 0x2fc], edi
        push   ecx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        lea    ecx, [ebp - 0x304]
        mov    dword ptr [ebp - 0x304], ebx
        push   ecx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        test   eax, eax
        je     loc_1_08C5
        cmp    byte ptr [esi + 0x93], bl
        jne    loc_1_08C5
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   eax, eax
        jne    loc_1_061A
loc_1_08C5:
        mov    edi, 1
        mov    dword ptr [ebp - 0x304], edi
loc_1_08D0:
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   eax, eax
        je     loc_1_08F2
        cmp    edi, 5
        jge    loc_1_061A
        inc    edi
        cmp    edi, 5
        jle    loc_1_08D0
loc_1_08F2:
        mov    dword ptr [ebp - 0x304], 0x23
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   eax, eax
        jne    loc_1_0621
        xor    ecx, ecx
loc_1_0915:
        lea    eax, [ecx + esi]
        mov    dl, byte ptr [ebp + ecx - 0x300]
        inc    ecx
        mov    byte ptr [eax + 0xa], dl
        mov    byte ptr [eax + 4], dl
        cmp    ecx, 6
        jl     loc_1_0915
        lea    eax, [esi + 0x68]
        lea    ecx, [esi + 0x44]
        push   eax
        lea    edx, [esi + 0xa]
        push   ecx
        mov    eax, dword ptr [esi + 0x10]
        push   edx
        add    eax, 5
        push   0                             ; -> loc_1_1A00
        push   0                             ; -> loc_1_1A10
        push   0                             ; -> loc_1_1920
        push   eax
        call   sub_2_0000
loc_1_0952:
        test   al, al
        jne    loc_1_0972
        push   3
        mov    eax, dword ptr [esi + 0x4c]
        push   2
        push   2
        push   0xaa000002
        push   eax
        call   sub_1_51B8
        add    esp, 0x14
        jmp    loc_1_061A
loc_1_0972:
        mov    dword ptr [ebp - 0x304], 0x2f
        lea    eax, [ebp - 0x304]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_09A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x300
        mov    dword ptr [eax], 0
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], 0xffffffff
        lea    eax, [eax + ecx*4]
loc_1_09C7:
        test   ecx, ecx
        je     loc_1_09D6
        sub    eax, 4
        dec    ecx
        cmp    dword ptr [eax], 0
        jne    loc_1_09C7
        mov    dword ptr [edx], ecx
loc_1_09D6:
        cmp    dword ptr [edx], -1
        jne    loc_1_09E8
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_09E8:
        push   -1
        lea    eax, [ebp - 4]
        push   0
        push   0x21
        push   eax
        call   sub_1_50E6
        test   eax, eax
        je     loc_1_0A1E
        push   0
        mov    esi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [esi + 0x4c]
        push   eax
        call   sub_1_51B8
        add    esp, 0xc
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0A1E:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0x8c]
        cmp    dword ptr [esi + 0x88], 0
        jne    loc_1_0AB8
        lea    eax, [ebp - 0x300]
        mov    edi, 2
        mov    dword ptr [ebp - 0x300], 4
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_2820
        lea    ecx, [ebp - 0x300]
        mov    dword ptr [ebp - 0x300], edi
        push   ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 0x300], 0xc
        mov    dword ptr [ebp - 0x2fc], 0
        lea    ecx, [ebp - 0x300]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_2820
        mov    dword ptr [ebp - 0x2fc], 1
        lea    ecx, [ebp - 0x300]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_2820
        lea    ecx, [ebp - 0x300]
        mov    dword ptr [ebp - 0x2fc], edi
        push   ecx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
loc_1_0AB8:
        mov    eax, dword ptr [esi + 0x88]
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x88], ecx
        mov    ecx, dword ptr [esi + 0x68]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x10
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_0170
loc_1_0AE5:
        test   al, al
        jne    loc_1_0B28
        push   0
        dec    dword ptr [esi + 0x8c]
        mov    eax, dword ptr [ebp - 4]
        push   0x21
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x88], ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_50EC
        push   0
        mov    ecx, dword ptr [esi + 0x4c]
        push   0xaa000002
        push   ecx
        call   sub_1_51B8
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_0B28:
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0x5dc
        mov    eax, dword ptr [ebp - 4]
        push   0xbb8
        add    esi, 0x38
        mov    dword ptr [eax + 0x18], 1
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x20], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        call   sub_1_50C2
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_1_0B80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0x88], 1
        sbb    al, al
        pop    ebp
        inc    al
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C2h, 00Ch, 000h, 08Dh, 09Bh, 000h
        db  000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BB0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        cmp    byte ptr [0], cl              ; -> loc_1_5358
        push   esi
        push   edi
        je     loc_1_0C13
        mov    dword ptr [0], 0              ; -> obj4:0x0004
        mov    dword ptr [0], 8              ; -> obj4:0x0314
        mov    eax, dword ptr [0]            ; -> obj4:0x0004
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [0]            ; -> obj4:0x0004
        mov    dword ptr [eax + 0xc], ecx
        mov    eax, dword ptr [0]            ; -> obj4:0x0004
        add    eax, 0x10
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> obj4:0x0010
        lea    edi, [esi + 0xc3]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0]            ; -> obj4:0x0010
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [0]            ; -> obj4:0x0010
        mov    dword ptr [eax + 0x18], edi
        mov    byte ptr [0], cl              ; -> loc_1_5358
        jmp    loc_1_0C16
loc_1_0C13:
        mov    esi, dword ptr [ebp + 8]
loc_1_0C16:
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_1_0C86
        mov    eax, dword ptr [0]            ; -> obj4:0x0004
        mov    edi, dword ptr [ebp + 0x14]
        push   0                             ; -> obj4:0x0314
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [0]            ; -> obj4:0x0010
        mov    dword ptr [ecx + 8], edi
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_32F0
        mov    ecx, dword ptr [0]            ; -> obj4:0x0010
        cmp    dword ptr [ecx + 4], 0
        jne    loc_1_0C86
        cmp    edi, 0xe
        jb     loc_1_0C86
        lea    eax, [edi - 0xe]
        lea    ecx, [esi + 0xd1]
        push   eax
        mov    dword ptr [esi + 0xbf], edi
        sub    edi, 0xe
        lea    eax, [esi + 0xc3]
        push   edi
        inc    dword ptr [esi + 0xab]
        push   ecx
        push   0xe
        push   eax
        mov    ecx, dword ptr [esi + 0x68]
        push   eax
        push   esi
        push   ecx
        call   sub_1_4C90
        mov    ecx, dword ptr [esi + 0x68]
        push   ecx
        call   sub_1_4E70
loc_1_0C86:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
loc_1_0C90:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [eax + 8]
        lea    ebx, [esi + 0x24]
        cmp    dword ptr [edi + 0x14], 0
        je     loc_1_0CDA
loc_1_0CAC:
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        push   edi
        call   sub_1_1F30
        test   eax, eax
        jne    loc_1_0CDA
        mov    eax, dword ptr [edi + 0x14]
        push   0
        push   eax
        mov    ecx, dword ptr [eax + 0x24]
        mov    dword ptr [edi + 0x14], ecx
        mov    edx, dword ptr [eax + 0x28]
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_502A
        dec    dword ptr [edi + 0x1c]
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_0CAC
loc_1_0CDA:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_5110
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5134
        mov    ecx, dword ptr [ebp - 4]
        mov    dl, byte ptr [edi + 0xf]
        cmp    byte ptr [ecx + 5], dl
        jne    loc_1_0D3B
        mov    al, byte ptr [edi + 0xe]
        cmp    byte ptr [ecx + 4], al
        jne    loc_1_0D3B
        mov    al, byte ptr [edi + 0xd]
        cmp    byte ptr [ecx + 3], al
        jne    loc_1_0D3B
        mov    al, byte ptr [edi + 0xc]
        cmp    byte ptr [ecx + 2], al
        jne    loc_1_0D3B
        mov    eax, ecx
        mov    al, byte ptr [ecx + 1]
        cmp    byte ptr [edi + 0xb], al
        jne    loc_1_0D3B
        mov    eax, ecx
        mov    al, byte ptr [ecx]
        cmp    byte ptr [edi + 0xa], al
        jne    loc_1_0D3B
        mov    byte ptr [ebx + 9], 1
        mov    byte ptr [ebx + 8], 1
        jmp    loc_1_0D4F
loc_1_0D3B:
        mov    byte ptr [ebx + 9], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [edi + 0x68]
        push   ecx
        push   edx
        call   sub_1_4F10
        mov    byte ptr [ebx + 8], al
loc_1_0D4F:
        cmp    byte ptr [edi + 0x96], 0
        je     loc_1_0D5C
        mov    byte ptr [ebx + 8], 1
loc_1_0D5C:
        cmp    byte ptr [ebx + 8], 0
        je     loc_1_0D9E
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], eax
        cmp    dword ptr [edi + 0x20], 0
        jne    loc_1_0D79
        mov    dword ptr [edi + 0x20], esi
        jmp    loc_1_0D7F
loc_1_0D79:
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [eax + 0x24], esi
loc_1_0D7F:
        mov    dword ptr [edi + 0x24], esi
        push   1
        inc    dword ptr [edi + 0x28]
        add    edi, 0x2c
        push   edi
        call   sub_1_50C2
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0D9E:
        cmp    dword ptr [edi + 0x14], 0
        je     loc_1_0DD5
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 0
        push   1
        add    edi, 0x2c
        push   edi
        mov    dword ptr [ebx + 4], eax
        mov    ecx, dword ptr [edi - 0x14]
        mov    dword ptr [ecx + 0x24], esi
        mov    dword ptr [edi - 0x14], esi
        inc    dword ptr [edi - 0x10]
        call   sub_1_50C2
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0DD5:
        push   esi
        push   edi
        call   sub_1_1F30
        test   eax, eax
        jne    loc_1_0DF1
        xor    eax, eax
        inc    dword ptr [edi + 0xa7]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0DF1:
        mov    eax, dword ptr [ebp + 8]
        push   1
        add    edi, 0x2c
        mov    dword ptr [ebx], 0
        push   edi
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [edi - 0x18], esi
        mov    dword ptr [edi - 0x14], esi
        inc    dword ptr [edi - 0x10]
        call   sub_1_50C2
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_1_0E20:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, dword ptr [eax + 0xbf]
        push   ebx
        push   esi
        sub    eax, 0xe
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 0x10]
        lea    ecx, [edi + esi]
        cmp    eax, ecx
        jae    loc_1_0E5A
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        mov    eax, 0xc0000001
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_0E5A:
        add    edi, 0xe
        push   0
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x18]
        push   eax
        push   0
        push   0
        push   ecx
        call   sub_1_5110
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        push   eax
        call   sub_1_5134
        mov    ecx, dword ptr [ebp + 0x1c]
        test   esi, esi
        mov    dword ptr [ecx], 0
        je     loc_1_0EE1
loc_1_0E8E:
        mov    ebx, dword ptr [ebp - 0xc]
        cmp    ebx, esi
        jb     loc_1_0E97
        mov    ebx, esi
loc_1_0E97:
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, edi
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 0xc3
        add    edi, ebx
        push   eax
        sub    esi, ebx
        push   ecx
        call   sub_1_51CC
        mov    eax, dword ptr [ebp + 0x1c]
        add    dword ptr [eax], ebx
        test   esi, esi
        je     loc_1_0EE1
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5158
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0EE1
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_5134
        test   esi, esi
        jne    loc_1_0E8E
loc_1_0EE1:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h
loc_1_0EF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [esi + 8]
        inc    dword ptr [esi + 0x18]
        inc    dword ptr [edi + 0x8c]
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        jne    loc_1_0F18
        push   eax
        push   esi
        push   edi
        call   sub_1_0F40
        jmp    loc_1_0F2C
loc_1_0F18:
        cmp    ecx, 1
        jne    loc_1_0F27
        push   eax
        push   esi
        push   edi
        call   sub_1_0F90
        jmp    loc_1_0F2C
loc_1_0F27:
        mov    eax, 0x10001
loc_1_0F2C:
        dec    dword ptr [esi + 0x18]
        dec    dword ptr [edi + 0x8c]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_0F40:
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
        call   sub_1_1140
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0F90:
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
        ja     loc_1_0FCD
        je     loc_1_10E9
        cmp    eax, 0x1010e
        je     loc_1_0FDF
        cmp    eax, 0x1010f
        je     loc_1_1040
        cmp    eax, 0x10112
        je     loc_1_10C2
loc_1_0FCD:
        mov    ebx, 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_1124
loc_1_0FDF:
        cmp    edi, 4
        je     loc_1_0FFC
        mov    dword ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x24], 4
        mov    ebx, 0xc0010014
        jmp    loc_1_1124
loc_1_0FFC:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_51CC
        test   dword ptr [ebp - 8], 0xf054
        je     loc_1_1029
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        mov    ebx, 0xc00000bb
        jmp    loc_1_1124
loc_1_1029:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_1630
        jmp    loc_1_1122
loc_1_1040:
        cmp    edi, 4
        je     loc_1_1057
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_1124
loc_1_1057:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_51CC
        inc    dword ptr [ebp - 4]
        and    dword ptr [ebp - 4], 0xfffffffe
        cmp    dword ptr [ebp - 4], 0x5dc
        ja     loc_1_10BB
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x9b]
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_1_1095
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x9b], eax
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_1124
loc_1_1095:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x14], eax
        jne    loc_1_10B3
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_1_10B3
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x14], eax
        call   sub_1_1660
        jmp    loc_1_1124
loc_1_10B3:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        jmp    loc_1_1124
loc_1_10BB:
        mov    ebx, 0xc0010014
        jmp    loc_1_1124
loc_1_10C2:
        cmp    edi, 4
        je     loc_1_10D6
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_1124
loc_1_10D6:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x1c
        push   ecx
        call   sub_1_51CC
        jmp    loc_1_1124
loc_1_10E9:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_1_1107
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_1124
loc_1_1107:
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
        call   sub_1_16A0
loc_1_1122:
        mov    ebx, eax
loc_1_1124:
        test   ebx, ebx
        jne    loc_1_1132
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_1_1132:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_1140:
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
        ja     loc_1_117D
        je     loc_1_12F7
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_119E
        jmp    dword ptr [eax*4]             ; -> obj1:0x1440
loc_1_117D:
        cmp    eax, 0x1010102
        je     loc_1_1316
        cmp    eax, 0x1010103
        je     loc_1_1335
        cmp    eax, 0x1010104
        je     loc_1_13F1
loc_1_119E:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_13F8
loc_1_11AA:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_11BF
        mov    esi, 0                        ; -> loc_1_52FC
        mov    edi, 0x5c
        jmp    loc_1_13F8
loc_1_11BF:
        mov    esi, 0                        ; -> loc_1_5280
        mov    edi, 0x7c
        jmp    loc_1_13F8
loc_1_11CE:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        mov    dword ptr [ebp - 0x14], 0
        jmp    loc_1_13F8
loc_1_11E2:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_13F8
loc_1_11EF:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_13F8
loc_1_11FB:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_13F8
loc_1_1207:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_13F8
loc_1_1213:
        mov    dword ptr [ebp - 8], 0x1388
        jmp    loc_1_13F8
loc_1_121F:
        mov    dword ptr [ebp - 8], 0x6000
        jmp    loc_1_13F8
loc_1_122B:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_13F8
loc_1_1237:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_13F8
loc_1_1243:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 4
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0
        call   sub_1_51CC
        and    dword ptr [ebp - 8], 0xffffff00
        jmp    loc_1_13F8
loc_1_1270:
        mov    esi, 0                        ; -> loc_1_54EC
        mov    edi, 0x31
        jmp    loc_1_13F8
loc_1_127F:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_1296
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x68]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_13F8
loc_1_1296:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_13F8
loc_1_12A7:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_12BE
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x9b]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_13F8
loc_1_12BE:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_13F8
loc_1_12CC:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_13F8
loc_1_12DF:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_13F8
loc_1_12EB:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_1_13F8
loc_1_12F7:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 4
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_51CC
        jmp    loc_1_13F8
loc_1_1316:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0xa
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_51CC
        jmp    loc_1_13F8
loc_1_1335:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_1392
        mov    ecx, dword ptr [ebx + 0x68]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    dword ptr [ebp + 0x1c], edx
        jae    loc_1_1367
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], edx
        jmp    loc_1_13F8
loc_1_1367:
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
loc_1_137D:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ecx], eax
        je     loc_1_13FE
        jmp    loc_1_13F8
loc_1_1392:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebx + 0x68]
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        push   edx
        call   sub_2_0660
loc_1_13A2:
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 0x10], eax
        lea    ecx, [edx*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_13C2
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], ecx
        jmp    loc_1_13F8
loc_1_13C2:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        mov    edx, dword ptr [ebx + 0x68]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_06A0
loc_1_13E2:
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_13F8
loc_1_13F1:
        mov    dword ptr [ebp - 8], 9
loc_1_13F8:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1434
loc_1_13FE:
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_1434
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_1_1424
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
loc_1_1424:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   esi
        push   eax
        call   sub_1_51CC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_1434:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_11AA
        dd  loc_1_11CE
        dd  loc_1_11E2
        dd  loc_1_11E2
        dd  loc_1_11EF
        dd  loc_1_11FB
        dd  loc_1_1207
        dd  loc_1_1213
        dd  loc_1_121F
        dd  loc_1_122B
        dd  loc_1_1237
        dd  loc_1_1243
        dd  loc_1_1270
        dd  loc_1_127F
        dd  loc_1_12A7
        dd  loc_1_12CC
        dd  loc_1_12DF
        dd  loc_1_119E
        dd  loc_1_12EB
        db  08Dh, 064h, 024h, 000h
loc_1_1490:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [edx + 0x14]
        cmp    ecx, 0x10113
        ja     loc_1_14B8
        je     loc_1_14EF
        cmp    ecx, 0x10101
        jb     loc_1_14EA
        cmp    ecx, 0x10111
        jbe    loc_1_14EF
        jmp    loc_1_14EA
loc_1_14B8:
        cmp    ecx, 0x1010104
        ja     loc_1_14DA
        cmp    ecx, 0x1010101
        jae    loc_1_14EF
        cmp    ecx, 0x20101
        jb     loc_1_14EA
        cmp    ecx, 0x20105
        jbe    loc_1_14EF
        jmp    loc_1_14EA
loc_1_14DA:
        cmp    ecx, 0x1020101
        jb     loc_1_14EA
        cmp    ecx, 0x1020103
        jbe    loc_1_14EF
loc_1_14EA:
        mov    eax, 0xc0010017
loc_1_14EF:
        test   eax, eax
        jne    loc_1_14FD
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1510
loc_1_14FD:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1510:
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
        call   sub_1_1140
        cmp    eax, 0xc00000bb
        mov    edi, eax
        jne    loc_1_15F8
        xor    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_1_1583
        je     loc_1_15C1
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_1591
        jmp    dword ptr [eax*4]             ; -> obj1:0x1610
loc_1_1583:
        cmp    eax, 0x1020102
        je     loc_1_15CA
        cmp    eax, 0x1020103
        je     loc_1_15D2
loc_1_1591:
        mov    edi, 0xc0010017
        jmp    loc_1_15DB
loc_1_1598:
        mov    eax, dword ptr [ebx + 0xa7]
        jmp    loc_1_15D8
loc_1_15A0:
        mov    eax, dword ptr [ebx + 0xab]
        jmp    loc_1_15D8
loc_1_15A8:
        mov    eax, dword ptr [ebx + 0xaf]
        jmp    loc_1_15D8
loc_1_15B0:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_15DB
loc_1_15B9:
        mov    eax, dword ptr [ebx + 0xbb]
        jmp    loc_1_15D8
loc_1_15C1:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_15DB
loc_1_15CA:
        mov    eax, dword ptr [ebx + 0xb3]
        jmp    loc_1_15D8
loc_1_15D2:
        mov    eax, dword ptr [ebx + 0xb7]
loc_1_15D8:
        mov    dword ptr [ebp - 4], eax
loc_1_15DB:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_15F8
        test   edi, edi
        jne    loc_1_15F8
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_51CC
        add    dword ptr [ebp - 8], 4
loc_1_15F8:
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
        db  090h
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_1598
        dd  loc_1_15A0
        dd  loc_1_15A8
        dd  loc_1_15B0
        dd  loc_1_15B9
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1630:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x10]
        inc    dword ptr [esi + 0x18]
        push   edx
        mov    ecx, dword ptr [eax + 0x68]
        push   ecx
        call   sub_2_05E0
loc_1_1655:
        dec    dword ptr [esi + 0x18]
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h
sub_1_1660:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x88]
        cmp    edx, ecx
        je     loc_1_1683
loc_1_1673:
        mov    esi, dword ptr [edx + 0x14]
        cmp    ecx, esi
        jae    loc_1_167C
        mov    ecx, esi
loc_1_167C:
        mov    edx, dword ptr [edx + 0xc]
        test   edx, edx
        jne    loc_1_1673
loc_1_1683:
        test   ecx, ecx
        jne    loc_1_168C
        mov    ecx, 0x5dc
loc_1_168C:
        pop    esi
        mov    dword ptr [eax + 0x9b], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16A0:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        push   1
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        mov    eax, dword ptr [edx + 0x68]
        push   eax
        call   sub_2_0300
loc_1_16C5:
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_16D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x300
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [esi + 0x88]
        mov    dword ptr [ebp - 4], 0
        test   edi, edi
        je     loc_1_170D
loc_1_16F3:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   0x40010004
        push   ecx
        call   sub_1_504E
        mov    edi, dword ptr [edi + 0xc]
        test   edi, edi
        jne    loc_1_16F3
loc_1_170D:
        mov    dword ptr [ebp - 0x300], 0xd
        lea    eax, [ebp - 0x300]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    esi, dword ptr [esi + 0x88]
        mov    dword ptr [ebp - 4], 0
        test   esi, esi
        je     loc_1_1751
loc_1_1737:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [esi]
        push   0x40010005
        push   ecx
        call   sub_1_504E
        mov    esi, dword ptr [esi + 0xc]
        test   esi, esi
        jne    loc_1_1737
loc_1_1751:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1760:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x300
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi + 0x2c]
        push   ecx
        call   sub_1_50C8
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x38]
        push   eax
        push   ecx
        call   sub_1_50C8
        mov    dword ptr [ebp - 0x300], 0xe
        lea    ecx, [ebp - 0x300]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_2820
        mov    ecx, dword ptr [esi + 0x68]
        push   ecx
        call   sub_2_0120
loc_1_17A9:
        lea    ecx, [esi + 0x50]
        push   ecx
        call   sub_1_50D4
        mov    ecx, dword ptr [esi + 0x4c]
        push   ecx
        call   sub_1_507E
        mov    ecx, dword ptr [esi + 0x4c]
        push   ecx
        call   sub_1_5018
        push   0
        push   0x707
        push   esi
        call   sub_1_50EC
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_17E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    dword ptr [ecx + 0x14], 0
        je     loc_1_1803
        pop    ebp
        inc    dword ptr [ecx + 0x703]
        mov    byte ptr [eax + 0x20], 1
        mov    eax, 0x103
        ret    4
loc_1_1803:
        push   eax
        call   sub_1_1810
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_1810:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x300
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 0x88]
        cmp    eax, esi
        jne    loc_1_1836
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi + 0x88], eax
        jmp    loc_1_1849
loc_1_1836:
        cmp    dword ptr [eax + 0xc], esi
        je     loc_1_1843
loc_1_183B:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], esi
        jne    loc_1_183B
loc_1_1843:
        mov    ecx, dword ptr [esi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_1849:
        push   0
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        mov    ecx, dword ptr [edi + 0x68]
        push   ecx
        call   sub_2_0240
loc_1_1858:
        cmp    dword ptr [edi + 0x88], 0
        jne    loc_1_18D0
        mov    dword ptr [ebp - 0x300], 0xb
        mov    dword ptr [ebp - 0x2fc], 2
        lea    eax, [ebp - 0x300]
        mov    ecx, dword ptr [edi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    dword ptr [ebp - 0x2fc], 1
        lea    ecx, [ebp - 0x300]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_1_2820
        mov    dword ptr [ebp - 0x2fc], 0
        lea    ecx, [ebp - 0x300]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_1_2820
        lea    ecx, [ebp - 1]
        lea    edx, [edi + 0x2c]
        push   ecx
        push   edx
        call   sub_1_50C8
        lea    ecx, [ebp - 1]
        lea    edx, [edi + 0x38]
        push   ecx
        push   edx
        call   sub_1_50C8
loc_1_18D0:
        push   0
        push   0x21
        push   esi
        call   sub_1_50EC
        xor    eax, eax
        dec    dword ptr [edi + 0x703]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_18F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_1_500C
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_5000
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1920:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 0x300
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 0x300], 0x28
        cmp    dword ptr [ebp + 8], ebx
        je     loc_1_1980
        xor    edi, edi
        mov    esi, dword ptr [ebp + 0xc]
loc_1_194C:
        xor    ecx, ecx
loc_1_194E:
        lea    eax, [esi + ecx]
        inc    ecx
        cmp    ecx, 6
        mov    al, byte ptr [eax + edi]
        mov    byte ptr [ebp + ecx - 0x2fd], al
        jb     loc_1_194E
        lea    eax, [ebp - 0x300]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_2820
        test   eax, eax
        jne    loc_1_19D5
        add    edi, 6
        inc    ebx
        cmp    ebx, dword ptr [ebp + 8]
        jb     loc_1_194C
loc_1_1980:
        xor    esi, esi
        xor    ebx, ebx
        mov    dword ptr [ebp - 0x300], 0x27
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_19CA
        mov    edi, dword ptr [ebp + 0x14]
loc_1_1996:
        xor    ecx, ecx
loc_1_1998:
        lea    eax, [edi + ecx]
        inc    ecx
        cmp    ecx, 6
        mov    al, byte ptr [eax + esi]
        mov    byte ptr [ebp + ecx - 0x2fd], al
        jb     loc_1_1998
        lea    eax, [ebp - 0x300]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_2820
        test   eax, eax
        jne    loc_1_19E3
        add    esi, 6
        inc    ebx
        cmp    ebx, dword ptr [ebp + 0x10]
        jb     loc_1_1996
loc_1_19CA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_19D5:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_19E3:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A00:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A10:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 0x2fc
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, edx
        and    eax, 0x20
        je     loc_1_1AA4
        test   cl, 0x20
        jne    loc_1_1AA4
        mov    byte ptr [esi + 0x96], 1
        mov    dword ptr [ebp - 0x2fc], 0x1a
        lea    eax, [ebp - 0x2fc]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 0x2f8], 4
        je     loc_1_1A65
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1A65:
        mov    eax, dword ptr [ebp - 0x2f8]
        lea    ecx, [ebp - 0x2fc]
        or     eax, 4
        push   ecx
        mov    dword ptr [ebp - 0x2fc], 0x17
        mov    dword ptr [ebp - 0x2f8], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0xc0000001
        ret    0x14
loc_1_1AA4:
        test   eax, eax
        jne    loc_1_1B1E
        test   cl, 0x20
        je     loc_1_1B1E
        mov    byte ptr [esi + 0x96], 0
        mov    dword ptr [ebp - 0x2fc], 0x1a
        lea    eax, [ebp - 0x2fc]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 0x2f8], 4
        jne    loc_1_1ADF
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1ADF:
        mov    eax, dword ptr [ebp - 0x2f8]
        lea    ecx, [ebp - 0x2fc]
        and    eax, 0xfffffffb
        push   ecx
        mov    dword ptr [ebp - 0x2fc], 0x17
        mov    dword ptr [ebp - 0x2f8], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0xc0000001
        ret    0x14
loc_1_1B1E:
        mov    eax, edx
        and    eax, 2
        je     loc_1_1B94
        test   cl, 2
        jne    loc_1_1B94
        mov    dword ptr [ebp - 0x2fc], 0x1a
        lea    eax, [ebp - 0x2fc]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 0x2f8], 0x10
        je     loc_1_1B55
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1B55:
        mov    eax, dword ptr [ebp - 0x2f8]
        lea    ecx, [ebp - 0x2fc]
        or     eax, 0x10
        push   ecx
        mov    dword ptr [ebp - 0x2fc], 0x17
        mov    dword ptr [ebp - 0x2f8], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0xc0000001
        ret    0x14
loc_1_1B94:
        test   eax, eax
        jne    loc_1_1C07
        test   cl, 2
        je     loc_1_1C07
        mov    dword ptr [ebp - 0x2fc], 0x1a
        lea    eax, [ebp - 0x2fc]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 0x2f8], 0x10
        jne    loc_1_1BC8
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1BC8:
        mov    eax, dword ptr [ebp - 0x2f8]
        lea    ecx, [ebp - 0x2fc]
        and    eax, 0xffffffef
        push   ecx
        mov    dword ptr [ebp - 0x2fc], 0x17
        mov    dword ptr [ebp - 0x2f8], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0xc0000001
        ret    0x14
loc_1_1C07:
        and    edx, 8
        je     loc_1_1C7B
        test   cl, 8
        jne    loc_1_1C7B
        mov    dword ptr [ebp - 0x2fc], 0x1a
        lea    eax, [ebp - 0x2fc]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 0x2f8], 8
        je     loc_1_1C3C
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1C3C:
        mov    eax, dword ptr [ebp - 0x2f8]
        lea    ecx, [ebp - 0x2fc]
        or     eax, 8
        push   ecx
        mov    dword ptr [ebp - 0x2fc], 0x17
        mov    dword ptr [ebp - 0x2f8], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0xc0000001
        ret    0x14
loc_1_1C7B:
        test   edx, edx
        jne    loc_1_1CEE
        test   cl, 8
        je     loc_1_1CEE
        mov    dword ptr [ebp - 0x2fc], 0x1a
        lea    eax, [ebp - 0x2fc]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 0x2f8], 8
        jne    loc_1_1CAF
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1CAF:
        mov    eax, dword ptr [ebp - 0x2f8]
        lea    ecx, [ebp - 0x2fc]
        and    eax, 0xfffffff7
        push   ecx
        mov    dword ptr [ebp - 0x2fc], 0x17
        mov    dword ptr [ebp - 0x2f8], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0xc0000001
        ret    0x14
loc_1_1CEE:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1D00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x88], 0
        je     loc_1_1F1B
        cmp    dword ptr [esi + 0x14], 0
        je     loc_1_1D7B
loc_1_1D21:
        test   bl, bl
        jne    loc_1_1D7B
        mov    ecx, dword ptr [esi + 0x14]
        mov    eax, dword ptr [ecx + 0x28]
        lea    edi, [ecx + 0x24]
        cmp    byte ptr [eax + 0x20], 0
        je     loc_1_1D47
        mov    eax, dword ptr [edi]
        push   0xc0000001
        push   ecx
        mov    dword ptr [esi + 0x14], eax
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edx]
        push   ecx
        jmp    loc_1_1D6D
loc_1_1D47:
        push   ecx
        push   esi
        call   sub_1_1F30
        test   eax, eax
        je     loc_1_1D56
        mov    bl, 1
        jmp    loc_1_1D75
loc_1_1D56:
        push   0
        inc    dword ptr [esi + 0xa7]
        mov    eax, dword ptr [esi + 0x14]
        mov    ecx, dword ptr [edi]
        push   eax
        mov    dword ptr [esi + 0x14], ecx
        mov    edx, dword ptr [edi + 4]
        mov    eax, dword ptr [edx]
        push   eax
loc_1_1D6D:
        call   sub_1_502A
        dec    dword ptr [esi + 0x1c]
loc_1_1D75:
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_1D21
loc_1_1D7B:
        mov    byte ptr [ebp - 1], 0
        cmp    dword ptr [esi + 0x20], 0
        je     loc_1_1EA1
loc_1_1D89:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_1EA1
        mov    ecx, dword ptr [esi + 0x20]
        mov    eax, dword ptr [ecx + 0x28]
        lea    edi, [ecx + 0x24]
        cmp    byte ptr [eax + 0x20], 0
        je     loc_1_1DB8
        mov    eax, dword ptr [edi]
        push   0xc0000001
        push   ecx
        mov    dword ptr [esi + 0x20], eax
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edx]
        push   ecx
        jmp    loc_1_1E8F
loc_1_1DB8:
        cmp    byte ptr [edi + 9], 0
        jne    loc_1_1DD8
        push   ecx
        push   esi
        call   sub_1_1F30
        test   eax, eax
        je     loc_1_1DD2
        mov    byte ptr [ebp - 1], 1
        jmp    loc_1_1E97
loc_1_1DD2:
        inc    dword ptr [esi + 0xa7]
loc_1_1DD8:
        cmp    byte ptr [edi + 8], 0
        je     loc_1_1E7E
        xor    ebx, ebx
        lea    eax, [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [esi + 0x20]
        push   eax
        push   ebx
        push   ebx
        push   ecx
        call   sub_1_5110
        cmp    dword ptr [ebp - 8], ebx
        je     loc_1_1E46
loc_1_1DF9:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5134
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_1E1A
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        jmp    loc_1_1E3B
loc_1_1E1A:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        push   ecx
        lea    eax, [ebx + esi]
        add    eax, 0xc3
        push   eax
        call   sub_1_51CC
        add    ebx, dword ptr [ebp - 0xc]
        lea    ecx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   eax
loc_1_1E3B:
        call   sub_1_5158
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1DF9
loc_1_1E46:
        lea    eax, [ebx - 0xe]
        lea    ecx, [esi + 0xd1]
        push   eax
        mov    dword ptr [esi + 0xbf], ebx
        sub    ebx, 0xe
        lea    eax, [esi + 0xc3]
        push   ebx
        inc    dword ptr [esi + 0xab]
        push   ecx
        push   0xe
        push   eax
        mov    ecx, dword ptr [esi + 0x68]
        push   eax
        push   esi
        push   ecx
        call   sub_1_4C90
        mov    ecx, dword ptr [esi + 0x68]
        push   ecx
        call   sub_1_4E70
loc_1_1E7E:
        mov    eax, dword ptr [esi + 0x20]
        mov    ecx, dword ptr [edi]
        push   0
        mov    dword ptr [esi + 0x20], ecx
        push   eax
        mov    edx, dword ptr [edi + 4]
        mov    eax, dword ptr [edx]
        push   eax
loc_1_1E8F:
        call   sub_1_502A
        dec    dword ptr [esi + 0x28]
loc_1_1E97:
        cmp    dword ptr [esi + 0x20], 0
        jne    loc_1_1D89
loc_1_1EA1:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2450
        cmp    dword ptr [esi + 0x20], 0
        jne    loc_1_1EF9
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_1EF9
        cmp    eax, 0x13
        je     loc_1_1EF9
        cmp    dword ptr [esi + 0x703], 0
        jbe    loc_1_1F04
        mov    edi, dword ptr [esi + 0x88]
        test   edi, edi
        je     loc_1_1F04
loc_1_1ECD:
        cmp    byte ptr [edi + 0x20], 0
        je     loc_1_1EF0
        push   edi
        mov    ebx, dword ptr [edi]
        mov    eax, edi
        mov    edi, dword ptr [edi + 0xc]
        call   sub_1_1810
        push   0
        push   ebx
        call   sub_1_5024
        dec    dword ptr [esi + 0x703]
        jmp    loc_1_1EF3
loc_1_1EF0:
        mov    edi, dword ptr [edi + 0xc]
loc_1_1EF3:
        test   edi, edi
        jne    loc_1_1ECD
        jmp    loc_1_1F04
loc_1_1EF9:
        push   1
        lea    eax, [esi + 0x2c]
        push   eax
        call   sub_1_50C2
loc_1_1F04:
        cmp    dword ptr [esi + 0x88], 0
        je     loc_1_1F1B
        push   0xbb8
        add    esi, 0x38
        push   esi
        call   sub_1_50C2
loc_1_1F1B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1F30:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0xc
        cmp    byte ptr [0], cl              ; -> loc_1_535C
        push   ebx
        push   esi
        push   edi
        je     loc_1_1F8D
        mov    dword ptr [0], 0              ; -> obj4:0x0910
        mov    dword ptr [0], 6              ; -> obj4:0x0614
        mov    eax, dword ptr [0]            ; -> obj4:0x0910
        add    eax, 4
        mov    dword ptr [0], eax            ; -> obj4:0x000C
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0]            ; -> obj4:0x000C
        mov    dword ptr [eax + 0x10], 1
        mov    eax, dword ptr [0]            ; -> obj4:0x000C
        mov    dword ptr [eax + 0x14], ecx
        mov    eax, dword ptr [0]            ; -> obj4:0x000C
        add    eax, 0x18
        mov    byte ptr [0], cl              ; -> loc_1_535C
        mov    dword ptr [0], eax            ; -> obj4:0x0000
loc_1_1F8D:
        xor    edi, edi
        xor    esi, esi
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   esi
        xor    ebx, ebx
        push   esi
        push   ecx
        call   sub_1_5110
loc_1_1FA3:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2021
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_5134
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1FCF
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5158
        jmp    loc_1_1FA3
loc_1_1FCF:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [0]            ; -> obj4:0x0000
        mov    dword ptr [ecx + ebx + 0xc], eax
        add    ebx, 0xc
        inc    edi
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> obj4:0x0000
        lea    eax, [ebp - 4]
        mov    dword ptr [ecx + ebx - 4], edx
        add    esi, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_5158
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_2021
        cmp    ebx, 0x2d0
        jl     loc_1_1FA3
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_2050
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2021:
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        push   0                             ; -> obj4:0x0614
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [0]            ; -> obj4:0x000C
        mov    dword ptr [ecx + 0xc], esi
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_32D0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2050:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        cmp    byte ptr [0], bl              ; -> loc_1_5360
        je     loc_1_20A5
        mov    dword ptr [0], 0              ; -> obj4:0x0008
        mov    dword ptr [0], 6              ; -> obj4:0x0014
        mov    eax, dword ptr [0]            ; -> obj4:0x0008
        add    eax, 4
        mov    dword ptr [0], eax            ; -> obj4:0x0610
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [0]            ; -> obj4:0x0610
        mov    dword ptr [eax + 0x10], 1
        mov    eax, dword ptr [0]            ; -> obj4:0x0610
        mov    byte ptr [0], bl              ; -> loc_1_5360
        add    eax, 0x18
        mov    dword ptr [0], eax            ; -> obj4:0x0310
loc_1_20A5:
        xor    esi, esi
        lea    eax, [ebp - 4]
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    dword ptr [ebp - 0x10], esi
        push   esi
        push   esi
        push   ecx
        call   sub_1_5110
        cmp    dword ptr [ebp - 4], esi
        je     loc_1_2125
        xor    edi, edi
loc_1_20C1:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_5134
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_2112
        cmp    edi, 0x2d0
        jge    loc_1_20FC
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [0]            ; -> obj4:0x0310
        mov    dword ptr [ecx + edi + 0xc], eax
        mov    ecx, dword ptr [0]            ; -> obj4:0x0310
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + edi + 8], edx
        jmp    loc_1_2108
loc_1_20FC:
        jne    loc_1_2108
        mov    ebx, dword ptr [ebp - 4]
        mov    eax, dword ptr [0]            ; -> obj4:0x0310
        mov    dword ptr [eax], esi
loc_1_2108:
        mov    ecx, dword ptr [ebp - 8]
        add    edi, 0xc
        inc    esi
        add    dword ptr [ebp - 0x10], ecx
loc_1_2112:
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5158
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_20C1
loc_1_2125:
        mov    eax, dword ptr [0]            ; -> obj4:0x0610
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    esi, 0x3c
        mov    dword ptr [eax + 0xc], ecx
        mov    eax, dword ptr [0]            ; -> obj4:0x0610
        jg     loc_1_2143
        mov    dword ptr [eax + 0x14], 0
        jmp    loc_1_214A
loc_1_2143:
        mov    dword ptr [eax + 0x14], 1
loc_1_214A:
        push   0                             ; -> obj4:0x0014
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_32D0
        test   eax, eax
        je     loc_1_216C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_216C:
        test   ebx, ebx
        jne    loc_1_217B
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_217B:
        mov    dword ptr [ebp - 4], ebx
        test   ebx, ebx
        je     loc_1_2211
        xor    edi, edi
loc_1_2188:
        xor    esi, esi
        cmp    dword ptr [ebp - 4], edi
        je     loc_1_2208
        xor    ebx, ebx
loc_1_2191:
        cmp    ebx, 0x2d0
        jge    loc_1_21DF
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_5134
        cmp    dword ptr [ebp - 8], edi
        je     loc_1_21CD
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [0]            ; -> obj4:0x0310
        mov    dword ptr [ecx + ebx + 0xc], eax
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> obj4:0x0310
        add    ebx, 0xc
        inc    esi
        mov    dword ptr [ecx + ebx - 4], edx
loc_1_21CD:
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5158
        cmp    dword ptr [ebp - 4], edi
        jne    loc_1_2191
loc_1_21DF:
        cmp    dword ptr [ebp - 4], edi
        je     loc_1_2211
        mov    eax, dword ptr [0]            ; -> obj4:0x0610
        push   0                             ; -> obj4:0x0014
        mov    dword ptr [eax + 0x14], 2
        mov    ecx, dword ptr [0]            ; -> obj4:0x0310
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], esi
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_32D0
loc_1_2208:
        cmp    dword ptr [ebp - 4], edi
        jne    loc_1_2188
loc_1_2211:
        mov    eax, dword ptr [0]            ; -> obj4:0x0610
        push   0                             ; -> obj4:0x0014
        mov    dword ptr [eax + 0x14], 3
        mov    ecx, dword ptr [0]            ; -> obj4:0x0310
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], esi
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_32D0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_2240:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        jle    loc_1_2255
loc_1_224B:
        push   0x61
        call   0
        dec    esi
        jne    loc_1_224B
loc_1_2255:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2260:
        cmp    dword ptr [0], 2              ; -> loc_1_5520
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jl     loc_1_2278
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2278:
        mov    eax, dword ptr [0]            ; -> loc_1_5520
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, 1
        lea    eax, [eax + eax*4]
        shl    eax, 6
        xor    edx, edx
        add    eax, 0                        ; -> obj1:0x5528
        mov    dword ptr [eax + 0x134], ecx
        mov    dword ptr [eax + 4], edi
        mov    ecx, 0x100
        mov    dword ptr [eax + 0x44], 0x1f
        mov    dword ptr [eax + 0x48], 5
        mov    dword ptr [eax + 0x4c], edx
        mov    dword ptr [eax + 0x50], edi
        mov    dword ptr [eax + 0x54], 0x1c00
        mov    dword ptr [eax + 0x58], edx
        mov    dword ptr [eax + 0x5c], ecx
        mov    dword ptr [eax + 0x60], ecx
        mov    ecx, 0x5dc
        mov    esi, 0x3c
        mov    dword ptr [eax + 0x64], ecx
        mov    dword ptr [eax + 0x68], 0x3df
        mov    dword ptr [eax + 0x6c], 0x19
        mov    dword ptr [eax + 0x70], edx
        mov    dword ptr [eax + 0x74], 0x6000
        mov    dword ptr [eax + 0x78], esi
        mov    dword ptr [eax + 0x7c], esi
        mov    esi, 9
        mov    dword ptr [eax + 0x80], ecx
        mov    dword ptr [eax + 0x84], edx
        mov    dword ptr [eax + 0x88], esi
        mov    dword ptr [eax + 0x8c], edi
        mov    dword ptr [eax + 0x90], edx
        mov    dword ptr [eax + 0x94], edx
        mov    dword ptr [eax + 0x98], ecx
        mov    ecx, 0xffffffff
        mov    dword ptr [eax + 0x9c], edx
        mov    dword ptr [eax + 0xa4], ecx
        mov    dword ptr [eax + 0xa8], ecx
        mov    dword ptr [eax + 0xa0], edx
        mov    dword ptr [eax + 0xb8], esi
        mov    dword ptr [eax + 0xbc], edx
        mov    ecx, dword ptr [0]            ; -> loc_1_5520
        mov    esi, ecx
        lea    ebx, [ecx + ecx*4]
        lea    ecx, [esi + ebx*2]
        lea    ecx, [esi + ecx*8]
        add    ecx, 0                        ; -> loc_1_57AC
        mov    dword ptr [eax + 0x120], ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_5520
        lea    ebx, [ecx + ecx*4]
        mov    esi, ecx
        mov    edi, 0xe
        lea    ecx, [esi + ebx*2]
        lea    ecx, [esi + ecx*8]
        mov    esi, 0xc
        mov    byte ptr [ecx + 0x20], 0x50   ; -> loc_1_57CC
        mov    dword ptr [ecx + 0x29], edi   ; -> obj1:0x57D5
        mov    byte ptr [ecx + 0x21], 0x51   ; -> obj1:0x57CD
        mov    dword ptr [ecx + 0x2d], esi   ; -> obj1:0x57D9
        mov    byte ptr [ecx + 0x22], 0x52   ; -> loc_1_57CE
        mov    dword ptr [ecx + 0x31], 0xa   ; -> obj1:0x57DD
        mov    byte ptr [ecx + 0x23], 0x53   ; -> obj1:0x57CF
        mov    dword ptr [ecx + 0x35], 8     ; -> obj1:0x57E1
        mov    byte ptr [ecx + 0x24], 0x53   ; -> loc_1_57D0
        mov    dword ptr [ecx + 0x39], edi   ; -> obj1:0x57E5
        mov    byte ptr [ecx + 0x25], 0x54   ; -> obj1:0x57D1
        mov    dword ptr [ecx + 0x3d], esi   ; -> obj1:0x57E9
        mov    byte ptr [ecx + 0x26], 0x55   ; -> loc_1_57D2
        mov    dword ptr [ecx + 0x41], 0xa   ; -> obj1:0x57ED
        mov    byte ptr [ecx + 0x27], 0x56   ; -> obj1:0x57D3
        mov    dword ptr [ecx + 0x45], 8     ; -> obj1:0x57F1
        mov    byte ptr [ecx + 0x28], 0x56   ; -> loc_1_57D4
        mov    dword ptr [ecx + 0x49], edi   ; -> obj1:0x57F5
        mov    dword ptr [ecx + 4], edx      ; -> loc_1_57B0
        mov    dword ptr [ecx + 0x4d], edx   ; -> obj1:0x57F9
        mov    dword ptr [ecx + 0x51], edx   ; -> obj1:0x57FD
        mov    dword ptr [ecx + 0x55], edx   ; -> obj1:0x5801
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 0xc], edx    ; -> loc_1_57B8
        pop    ebx
        mov    dword ptr [ecx + 0x10], edx   ; -> loc_1_57BC
        pop    ebp
        mov    dword ptr [ecx + 0x14], edx   ; -> loc_1_57C0
        mov    dword ptr [ecx + 0x18], edx   ; -> loc_1_57C4
        mov    dword ptr [ecx + 0x1c], edx   ; -> loc_1_57C8
        mov    dword ptr [ecx + 8], edx      ; -> loc_1_57B4
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2450:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        mov    dword ptr [ebp - 0xc], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x128]
        lea    ecx, [esi + 1]
        mov    dword ptr [ebp - 8], ecx
        cmp    eax, 7
        je     loc_1_248B
        cmp    eax, 6
        je     loc_1_248B
        mov    eax, 0xa
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_248B:
        mov    edx, esi
        xor    al, al
        out    dx, al
        lea    eax, [esi + 6]
        push   eax
        call   0
        and    al, 0xf7
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], al
        mov    edi, dword ptr [ecx + 0x120]
        je     loc_1_250B
        test   al, 2
        je     loc_1_24E8
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        lea    eax, [esi + 0xd]
        push   eax
        call   0
        xor    edx, edx
        mov    ecx, dword ptr [edi + 4]
        mov    dl, al
        mov    eax, edx
        cmp    edx, ecx
        jle    loc_1_24CD
        sub    eax, ecx
        jmp    loc_1_24D4
loc_1_24CD:
        sub    eax, ecx
        add    eax, 0x100
loc_1_24D4:
        add    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 0x4d], 0
        mov    dword ptr [edi + 0x51], 0
loc_1_24E8:
        test   byte ptr [ebp - 4], 0x20
        je     loc_1_24F5
        mov    dword ptr [edi + 0xc], 1
loc_1_24F5:
        test   byte ptr [ebp - 4], 0x40
        je     loc_1_250B
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0xc]
        mov    eax, 0x2000
        out    dx, ax
loc_1_250B:
        mov    edx, dword ptr [ebp - 8]
        mov    al, 0x40
        push   5
        lea    ebx, [esi + 0xb]
        out    dx, al
        call   sub_1_2240
        push   ebx
        call   0
        test   al, al
        je     loc_1_25C3
        test   al, 8
        je     loc_1_259A
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x60]
        add    ecx, 0x20
        mov    dword ptr [eax + 0x60], ecx
        cmp    dword ptr [eax + 0x64], ecx
        ja     loc_1_2542
        and    dword ptr [eax + 0x48], 0xfffffffd
loc_1_2542:
        cmp    dword ptr [edi], 0
        jne    loc_1_2560
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4930
        mov    edx, esi
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2560:
        mov    edx, dword ptr [ebp - 8]
        mov    al, 3
        out    dx, al
        lea    edx, [esi + 0xa]
        mov    eax, ecx
        and    eax, 0xffff
        out    dx, ax
        mov    edx, esi
        mov    al, 0x44
loc_1_2576:
        out    dx, al
loc_1_2577:
        cmp    dword ptr [edi + 0x51], 0x1e
        jle    loc_1_27B1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4930
        mov    edx, esi
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_259A:
        test   al, 0x7f
        je     loc_1_25BE
        mov    edx, esi
        mov    al, 0x50
        out    dx, al
        cmp    dword ptr [edi + 0x18], 0
        jne    loc_1_25BE
        mov    eax, dword ptr [edi + 0x1c]
        inc    eax
        mov    dword ptr [edi + 0x1c], eax
        cmp    eax, 0x3e8
        jle    loc_1_25BE
        mov    dword ptr [edi + 0x18], 1
loc_1_25BE:
        mov    edx, ebx
        xor    al, al
        out    dx, al
loc_1_25C3:
        lea    eax, [esi + 0xc]
        push   eax
        call   0
        test   al, 0x30
        mov    cl, al
        je     loc_1_25E4
        lea    eax, [esi + 0xc]
        mov    edx, eax
        xor    al, al
        out    dx, al
        test   cl, 0x10
        je     loc_1_25E4
        mov    edx, esi
        mov    al, 0x50
        out    dx, al
loc_1_25E4:
        lea    ebx, [esi + 9]
        push   ebx
        call   0
        test   al, 0xcf
        mov    cl, al
        je     loc_1_2617
        mov    edx, ebx
        xor    al, al
        out    dx, al
        test   cl, 0x10
        je     loc_1_260B
        mov    dword ptr [edi + 0x10], 1
        mov    dword ptr [edi + 0x14], 1
loc_1_260B:
        test   cl, 0x40
        je     loc_1_2617
        mov    dword ptr [edi + 0x14], 1
loc_1_2617:
        test   cl, 0x20
        jne    loc_1_2623
        mov    dword ptr [edi + 0x14], 1
loc_1_2623:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        push   esi
        call   0
        cmp    dword ptr [edi + 0xc], 0
        jne    loc_1_2640
        test   al, 1
        je     loc_1_2640
        mov    dword ptr [edi + 0xc], 1
loc_1_2640:
        test   al, 4
        je     loc_1_264E
        lea    edx, [esi + 0xc]
        mov    eax, 0x2000
        out    dx, ax
loc_1_264E:
        mov    ecx, dword ptr [edi + 0xc]
        test   ecx, ecx
        jne    loc_1_2699
        cmp    dword ptr [edi + 8], 0
        jne    loc_1_2699
        cmp    dword ptr [edi + 0x10], 0
        jne    loc_1_2699
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_2699
        cmp    dword ptr [edi + 0x4d], 0
        je     loc_1_2686
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4930
        mov    edx, esi
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2686:
        mov    edx, esi
        mov    al, 4
        out    dx, al
        mov    eax, 0xa
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2699:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xa8], 0
        mov    eax, 0
        jge    loc_1_26AF
        mov    eax, 1
loc_1_26AF:
        mov    dword ptr [ebp - 4], eax
        cmp    ecx, 1
        jne    loc_1_2577
loc_1_26BB:
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_26D1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xa8], 0
        jle    loc_1_2577
loc_1_26D1:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        cmp    dword ptr [edi + 0x4d], 0
        je     loc_1_26E0
        inc    dword ptr [edi + 0x51]
loc_1_26E0:
        inc    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], 0x64
        je     loc_1_278F
        lea    edx, [esi + 0xc]
        mov    eax, 0x2000
        out    dx, ax
        lea    ecx, [esi + 0xe]
        push   ecx
        call   sub_1_0010
        mov    ebx, eax
        and    ebx, 0xffff
        test   ebx, ebx
        jne    loc_1_2729
        lea    edx, [esi + 0xc]
        mov    eax, 0x2000
        out    dx, ax
        push   esi
        call   0
        test   al, 1
        jne    loc_1_2780
        mov    dword ptr [edi + 0xc], 0
        jmp    loc_1_2780
loc_1_2729:
        lea    eax, [esi + 0xc]
        push   eax
        call   0
        test   al, 0x80
        je     loc_1_2798
        sub    ebx, 4
        cmp    ebx, 0x5ea
        jbe    loc_1_276C
        lea    edx, [esi + 0xc]
        mov    eax, 0x2000
        out    dx, ax
        push   esi
        call   0
        test   al, 1
        jne    loc_1_275C
        mov    dword ptr [edi + 0xc], 0
loc_1_275C:
        test   al, 4
        je     loc_1_2780
        lea    edx, [esi + 0xc]
        mov    eax, 0x2000
        out    dx, ax
        jmp    loc_1_2780
loc_1_276C:
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   1
        mov    ecx, dword ptr [eax + 0x134]
        push   ecx
        call   sub_1_0BB0
loc_1_2780:
        cmp    dword ptr [edi + 0xc], 1
        je     loc_1_26BB
        jmp    loc_1_2577
loc_1_278F:
        mov    edx, esi
        mov    al, 4
        jmp    loc_1_2576
loc_1_2798:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4930
        mov    edx, esi
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_27B1:
        cmp    dword ptr [edi + 0x10], 1
        jne    loc_1_27C3
        mov    edx, esi
        mov    al, 0x20
        out    dx, al
        mov    dword ptr [edi + 0x10], 0
loc_1_27C3:
        cmp    dword ptr [edi + 0x14], 1
        jne    loc_1_27F9
        mov    edx, dword ptr [ebp - 8]
        mov    al, 0x40
        out    dx, al
        push   5
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 0x80
        out    dx, al
        mov    al, 0x64
        out    dx, al
        lea    eax, [esi + 9]
        push   eax
        call   0
        test   al, 0x20
        je     loc_1_27F9
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        mov    dword ptr [edi + 0x14], 0
loc_1_27F9:
        mov    edx, esi
        mov    al, 4
        out    dx, al
        cmp    dword ptr [edi + 0xc], 1
        mov    eax, 0
        pop    edi
        adc    eax, -1
        pop    esi
        and    eax, 0x13
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2820:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        cmp    ecx, 0x2f
        ja     loc_1_2835
        jmp    dword ptr [ecx*4]             ; -> obj1:0x2AC4
loc_1_2835:
        mov    eax, 3
        pop    ebp
        ret    8
loc_1_283E:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2B90
        pop    ebp
        ret    8
loc_1_284C:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2BC0
        pop    ebp
        ret    8
loc_1_285A:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3310
        pop    ebp
        ret    8
loc_1_2868:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3340
        pop    ebp
        ret    8
loc_1_2876:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4430
        pop    ebp
        ret    8
loc_1_2884:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4440
        pop    ebp
        ret    8
loc_1_2892:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5860
        pop    ebp
        ret    8
loc_1_28A1:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A50
        pop    ebp
        ret    8
loc_1_28AF:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_57A8
        pop    ebp
        ret    8
loc_1_28BE:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E10
        pop    ebp
        ret    8
loc_1_28CC:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E20
        pop    ebp
        ret    8
loc_1_28DA:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3F00
        pop    ebp
        ret    8
loc_1_28E8:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E30
        pop    ebp
        ret    8
loc_1_28F6:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3FD0
        pop    ebp
        ret    8
loc_1_2904:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3FE0
        pop    ebp
        ret    8
loc_1_2912:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3700
        pop    ebp
        ret    8
loc_1_2920:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3720
        pop    ebp
        ret    8
loc_1_292E:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4450
        pop    ebp
        ret    8
loc_1_293C:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3730
        pop    ebp
        ret    8
loc_1_294A:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3750
        pop    ebp
        ret    8
loc_1_2958:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3770
        pop    ebp
        ret    8
loc_1_2966:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3AB0
        pop    ebp
        ret    8
loc_1_2974:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B70
        pop    ebp
        ret    8
loc_1_2982:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4010
        pop    ebp
        ret    8
loc_1_2990:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3AE0
        pop    ebp
        ret    8
loc_1_299E:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CD0
        pop    ebp
        ret    8
loc_1_29AC:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4080
        pop    ebp
        ret    8
loc_1_29BA:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B00
        pop    ebp
        ret    8
loc_1_29C8:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CF0
        pop    ebp
        ret    8
loc_1_29D6:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_40A0
        pop    ebp
        ret    8
loc_1_29E4:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B50
        pop    ebp
        ret    8
loc_1_29F2:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3D40
        pop    ebp
        ret    8
loc_1_2A00:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_40D0
        pop    ebp
        ret    8
loc_1_2A0E:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4460
        pop    ebp
        ret    8
loc_1_2A1C:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3DE0
        pop    ebp
        ret    8
loc_1_2A2A:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3D60
        pop    ebp
        ret    8
loc_1_2A38:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4780
        pop    ebp
        ret    8
loc_1_2A46:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_47A0
        pop    ebp
        ret    8
loc_1_2A54:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4490
        pop    ebp
        ret    8
loc_1_2A62:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4630
        pop    ebp
        ret    8
loc_1_2A70:
        mov    eax, 0xc
        pop    ebp
        ret    8
loc_1_2A79:
        mov    eax, 0xc
        pop    ebp
        ret    8
loc_1_2A82:
        mov    eax, 0xc
        pop    ebp
        ret    8
loc_1_2A8B:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4910
        pop    ebp
        ret    8
loc_1_2A99:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_40F0
        pop    ebp
        ret    8
loc_1_2AA7:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4810
        pop    ebp
        ret    8
loc_1_2AB5:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_47F0
        pop    ebp
        ret    8
        db  090h
        ; ---- dispatch table (48 entries) ----
        dd  loc_1_283E
        dd  loc_1_284C
        dd  loc_1_285A
        dd  loc_1_2868
        dd  loc_1_2876
        dd  loc_1_2884
        dd  loc_1_2892
        dd  loc_1_28A1
        dd  loc_1_28AF
        dd  loc_1_28BE
        dd  loc_1_28CC
        dd  loc_1_28DA
        dd  loc_1_28E8
        dd  loc_1_28F6
        dd  loc_1_2904
        dd  loc_1_2912
        dd  loc_1_2920
        dd  loc_1_292E
        dd  loc_1_2835
        dd  loc_1_293C
        dd  loc_1_294A
        dd  loc_1_2958
        dd  loc_1_2966
        dd  loc_1_2974
        dd  loc_1_2982
        dd  loc_1_2990
        dd  loc_1_299E
        dd  loc_1_29AC
        dd  loc_1_29BA
        dd  loc_1_29C8
        dd  loc_1_29D6
        dd  loc_1_29E4
        dd  loc_1_29F2
        dd  loc_1_2A00
        dd  loc_1_2A0E
        dd  loc_1_2A1C
        dd  loc_1_2A2A
        dd  loc_1_2A38
        dd  loc_1_2A46
        dd  loc_1_2A54
        dd  loc_1_2A62
        dd  loc_1_2A70
        dd  loc_1_2A79
        dd  loc_1_2A82
        dd  loc_1_2A8B
        dd  loc_1_2A99
        dd  loc_1_2AA7
        dd  loc_1_2AB5
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0x3e8
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x20]
        mov    byte ptr [eax + 0x800], 0x41
        call   sub_1_2240
        mov    dword ptr [esi + 0x128], 4
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_2BC0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0x128], 4
        lea    eax, [esi + 1]
        lea    edi, [esi + 4]
        mov    dword ptr [ebp - 0x14], eax
        je     loc_1_2BF0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2BF0:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x14]
        push   1
        mov    eax, dword ptr [ecx + 0x120]
        mov    dword ptr [ebp - 0x1c], eax
        mov    al, 4
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        push   0xffff
        call   sub_1_2240
        push   0xffff
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 4
        out    dx, al
        push   1
        call   sub_1_2240
        lea    edx, [esi + 9]
        mov    al, 5
        out    dx, al
        push   0xffff
        call   sub_1_2240
        push   0xffff
        call   sub_1_2240
        mov    edx, esi
        mov    al, 2
        out    dx, al
        push   0xffff
        call   sub_1_2240
        mov    edx, esi
        xor    al, al
        out    dx, al
        push   0xffff
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 4
        out    dx, al
        lea    edx, [esi + 8]
        push   0xea60
        out    dx, al
        call   sub_1_2240
        push   esi
        call   0
        lea    ecx, [esi + 0xa]
        push   ecx
        call   0
        and    al, 0x30
        xor    edx, edx
        shr    al, 4
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    dl, al
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], 0x67
        mov    dword ptr [ecx], edx
loc_1_2CA2:
        lea    eax, [esi + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        add    ecx, ebx
        mov    ax, word ptr [ebp - 4]
        mov    dword ptr [ebp - 8], ecx
        out    dx, ax
        lea    eax, [esi + 0xe]
        push   eax
        call   0
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0xac], al
        test   byte ptr [edx + 0xac], 1
        jne    loc_1_2EA4
        inc    ebx
        mov    byte ptr [ecx + 0xb2], al
        inc    dword ptr [ebp - 4]
        cmp    ebx, 6
        jl     loc_1_2CA2
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x40
        out    dx, al
        push   5
        call   sub_1_2240
        lea    edx, [esi + 0xd]
        mov    al, 0x80
        out    dx, al
        lea    eax, [esi + 0xe]
        mov    edx, eax
        mov    al, 0x80
        out    dx, al
        lea    edx, [esi + 0xf]
        mov    al, 0x20
        out    dx, al
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0xb]
        out    dx, al
        lea    eax, [esi + 0xc]
        mov    edx, eax
        xor    al, al
        out    dx, al
        mov    edx, dword ptr [ebp - 0x1c]
        cmp    dword ptr [edx], 0
        jne    loc_1_2D35
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 2
        out    dx, al
        lea    edx, [esi + 8]
        xor    al, al
        out    dx, al
        lea    eax, [esi + 9]
        mov    edx, eax
        mov    al, 0x20
        out    dx, al
loc_1_2D35:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x6c]
        mov    dword ptr [ebp - 4], eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        je     loc_1_2D55
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 3
        out    dx, al
        mov    ax, word ptr [ecx + 0x7c]
        lea    edx, [esi + 8]
        out    dx, ax
loc_1_2D55:
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ecx + 0x48]
        and    ebx, 2
        je     loc_1_2D71
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 3
        out    dx, al
        lea    eax, [esi + 0xa]
        mov    edx, eax
        mov    ax, word ptr [ecx + 0x60]
        out    dx, ax
loc_1_2D71:
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0xd]
        out    dx, al
        xor    al, al
        test   ebx, ebx
        je     loc_1_2D83
        mov    al, 1
loc_1_2D83:
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_2D8B
        or     al, 2
loc_1_2D8B:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 0x8c], 8
        je     loc_1_2D99
        or     al, 4
loc_1_2D99:
        lea    edx, [esi + 0xe]
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x42
        out    dx, al
        lea    eax, [esi + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        mov    al, 0x20
        out    dx, al
        cmp    dword ptr [ecx + 0xbc], 0
        mov    al, 0x10
        jle    loc_1_2DC1
        test   byte ptr [ebp - 4], 0x10
        je     loc_1_2DC1
        mov    al, 0x11
loc_1_2DC1:
        test   byte ptr [ebp - 4], 4
        je     loc_1_2DC9
        or     al, 4
loc_1_2DC9:
        test   byte ptr [ebp - 4], 8
        jne    loc_1_2DD1
        or     al, 8
loc_1_2DD1:
        lea    edx, [esi + 9]
        push   esi
        out    dx, al
        xor    ebx, ebx
        call   0
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x50
        out    dx, al
        push   0xc8
        call   sub_1_2240
        mov    dword ptr [ebp - 4], 8
loc_1_2DF4:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        add    edx, esi
        inc    ebx
        inc    dword ptr [ebp - 4]
        mov    al, byte ptr [ebx + ecx + 0xb1]
        out    dx, al
        cmp    ebx, 6
        jl     loc_1_2DF4
        push   esi
        call   0
        mov    dword ptr [ebp - 0x10], 0x50
        mov    ecx, 0xe
loc_1_2E1F:
        cmp    dword ptr [ebp - 0x10], 0x58
        jge    loc_1_2E55
        lea    edx, [ecx + esi]
        xor    al, al
        out    dx, al
        inc    ecx
        cmp    ecx, 0x10
        jl     loc_1_2E4A
        mov    edx, dword ptr [ebp - 0x14]
        push   0xc8
        inc    dword ptr [ebp - 0x10]
        mov    al, byte ptr [ebp - 0x10]
        out    dx, al
        call   sub_1_2240
        mov    ecx, 8
loc_1_2E4A:
        cmp    dword ptr [ebp - 0x10], 0x57
        jne    loc_1_2E1F
        cmp    ecx, 0xc
        jne    loc_1_2E1F
loc_1_2E55:
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        lea    ecx, [esi + 0xd]
        push   ecx
        call   0
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0x1c]
        push   esi
        mov    dword ptr [ecx + 4], eax
        call   0
        push   esi
        call   0
        and    al, 0x20
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x14]
        cmp    al, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 2
        mov    dword ptr [ecx + 0x3c], eax
        cmp    eax, 2
        mov    al, 2
        out    dx, al
        lea    eax, [esi + 0xa]
        mov    edx, eax
        jne    loc_1_2EBE
        mov    al, 0x3b
        jmp    loc_1_2EC0
loc_1_2EA4:
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 4
        out    dx, al
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2EBE:
        mov    al, 0x39
loc_1_2EC0:
        out    dx, al
        push   0x64
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0xc]
        mov    eax, 0x2000
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        test   byte ptr [ecx + 0x8c], 1
        je     loc_1_2F04
        mov    dword ptr [0], 0              ; -> sub_1_5860
        mov    dword ptr [0], 0              ; -> sub_1_57A8
        mov    dword ptr [ebp - 0x18], 0
        jmp    loc_1_30B0
loc_1_2F04:
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x42
        out    dx, al
        push   5
        call   sub_1_2240
        lea    ecx, [esi + 8]
        push   ecx
        call   0
        lea    ecx, [esi + 9]
        mov    byte ptr [ebp - 9], al
        push   ecx
        call   0
        lea    edx, [esi + 8]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 9]
        or     al, 2
        out    dx, al
        mov    al, byte ptr [ebp - 8]
        lea    edx, [esi + 9]
        or     al, 6
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0xe]
        push   edx
        call   0
        lea    edx, [esi + 0xe]
        mov    byte ptr [ebp - 4], al
        and    al, 0xfc
        push   5
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x40
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 0x24
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x500
        mov    ecx, dword ptr [edx + 0x38]
        mov    edx, edi
        out    dx, ax
        mov    edx, 5
loc_1_2F80:
        xor    eax, eax
loc_1_2F82:
        mov    byte ptr [ecx], al
        inc    ecx
        inc    eax
        cmp    eax, 0x100
        jl     loc_1_2F82
        dec    edx
        jne    loc_1_2F80
        mov    edx, esi
        mov    al, 1
        out    dx, al
        mov    dword ptr [ebp - 0x10], 0
loc_1_2F9C:
        push   esi
        call   0
        test   al, 1
        jne    loc_1_2FB2
        inc    dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x10], 0x186a0
        jl     loc_1_2F9C
loc_1_2FB2:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 0x10], 0x186a0
        mov    ebx, dword ptr [ecx + 0x38]
        jne    loc_1_2FCA
        mov    dword ptr [ebp - 0x18], 0
        jmp    loc_1_2FE7
loc_1_2FCA:
        lea    ecx, [esi + 0xe]
        push   ecx
        call   sub_1_0010
        and    eax, 0xffff
        sub    eax, 0x504
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 0x18], eax
loc_1_2FE7:
        xor    eax, eax
        cmp    dword ptr [ebp - 0x18], eax
        je     loc_1_301D
loc_1_2FEE:
        cmp    eax, 5
        jge    loc_1_301D
        xor    edx, edx
loc_1_2FF5:
        mov    ecx, ebx
        inc    ebx
        mov    cl, byte ptr [ecx]
        and    ecx, 0xff
        cmp    ecx, edx
        jne    loc_1_300F
        inc    edx
        cmp    edx, 0x100
        jl     loc_1_2FF5
        jmp    loc_1_3016
loc_1_300F:
        mov    dword ptr [ebp - 0x18], 0
loc_1_3016:
        inc    eax
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_2FEE
loc_1_301D:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_3039
        mov    dword ptr [0], 0              ; -> sub_1_5860
        mov    dword ptr [0], 0              ; -> sub_1_57A8
        jmp    loc_1_3054
loc_1_3039:
        mov    dword ptr [0], 0              ; -> sub_1_5860
        mov    dword ptr [0], 0              ; -> sub_1_57A8
        mov    dword ptr [ebp - 0x18], 0
loc_1_3054:
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0xc]
        mov    eax, 0x8000
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x40
        out    dx, al
        lea    edx, [esi + 8]
        mov    al, 0x88
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 9]
        out    dx, al
        lea    eax, [esi + 0xb]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0xc]
        push   5
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x42
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, byte ptr [ebp - 9]
        out    dx, al
        lea    edx, [esi + 9]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 1
        out    dx, al
        lea    eax, [esi + 0xe]
        mov    edx, eax
        mov    al, byte ptr [ebp - 4]
        out    dx, al
loc_1_30B0:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_32AB
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x42
        out    dx, al
        push   5
        call   sub_1_2240
        lea    ecx, [esi + 8]
        push   ecx
        call   0
        lea    ecx, [esi + 9]
        mov    byte ptr [ebp - 9], al
        push   ecx
        call   0
        lea    edx, [esi + 8]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 9]
        or     al, 2
        out    dx, al
        mov    al, byte ptr [ebp - 8]
        lea    edx, [esi + 9]
        or     al, 6
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0xe]
        push   edx
        call   0
        lea    edx, [esi + 0xe]
        mov    byte ptr [ebp - 4], al
        and    al, 0xfc
        push   5
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x40
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 0x24
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        mov    edx, edi
        mov    eax, 0x500
        out    dx, ax
        mov    ecx, 3
loc_1_3130:
        xor    eax, eax
loc_1_3132:
        mov    edx, edi
        out    dx, al
        inc    eax
        cmp    eax, 0x100
        jl     loc_1_3132
        dec    ecx
        jne    loc_1_3130
        mov    ecx, 2
loc_1_3145:
        xor    eax, eax
loc_1_3147:
        mov    edx, edi
        out    dx, ax
        inc    eax
        cmp    eax, 0x80
        jl     loc_1_3147
        dec    ecx
        jne    loc_1_3145
        mov    edx, esi
        mov    al, 1
        out    dx, al
        xor    ebx, ebx
loc_1_315D:
        push   esi
        call   0
        test   al, 1
        jne    loc_1_3170
        inc    ebx
        cmp    ebx, 0x186a0
        jl     loc_1_315D
loc_1_3170:
        cmp    ebx, 0x186a0
        jne    loc_1_3181
        mov    dword ptr [ebp - 0x18], 0
        jmp    loc_1_319E
loc_1_3181:
        lea    ecx, [esi + 0xe]
        push   ecx
        call   sub_1_0010
        and    eax, 0xffff
        sub    eax, 0x504
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 0x18], eax
loc_1_319E:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_31DD
loc_1_31AB:
        cmp    dword ptr [ebp - 0x10], 3
        jge    loc_1_31DD
        xor    ebx, ebx
loc_1_31B3:
        push   edi
        call   0
        and    eax, 0xff
        cmp    eax, ebx
        jne    loc_1_31CD
        inc    ebx
        cmp    ebx, 0x100
        jl     loc_1_31B3
        jmp    loc_1_31D4
loc_1_31CD:
        mov    dword ptr [ebp - 0x18], 0
loc_1_31D4:
        inc    dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_31AB
loc_1_31DD:
        mov    dword ptr [ebp - 0x10], 0
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    dword ptr [ebp - 0x18], 0
        mov    dword ptr [ecx + 0x55], 0
        je     loc_1_3234
loc_1_31F4:
        cmp    dword ptr [ebp - 0x10], 2
        jge    loc_1_322E
        mov    ecx, dword ptr [ebp - 0x1c]
        cmp    dword ptr [ecx + 0x55], 0
        jne    loc_1_322E
        xor    ebx, ebx
loc_1_3205:
        push   edi
        call   sub_1_0010
        cmp    ax, bx
        jne    loc_1_321B
        inc    ebx
        cmp    ebx, 0x80
        jl     loc_1_3205
        jmp    loc_1_3225
loc_1_321B:
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    dword ptr [ecx + 0x55], 1
loc_1_3225:
        inc    dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_31F4
loc_1_322E:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_324E
loc_1_3234:
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 4
        out    dx, al
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_324E:
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0xc]
        mov    eax, 0x8000
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x40
        out    dx, al
        lea    edx, [esi + 8]
        mov    al, 0x88
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 9]
        out    dx, al
        lea    eax, [esi + 0xb]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0xc]
        push   5
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x42
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, byte ptr [ebp - 9]
        out    dx, al
        lea    edx, [esi + 9]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0xe]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
loc_1_32AB:
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 0x128], 6
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32D0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_5860
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32F0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_57A8
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3310:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0xc]
        push   5
        mov    al, 0x40
        lea    edx, [esi + 1]
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 0x80
        out    dx, al
        mov    al, 4
        pop    esi
        out    dx, al
        mov    al, 0x20
        pop    ebp
        out    dx, al
        xor    eax, eax
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_3340:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0xc]
        push   5
        mov    al, 0x40
        lea    edx, [esi + 1]
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 0x80
        out    dx, al
        mov    al, 8
        pop    esi
        out    dx, al
        mov    al, 0x20
        pop    ebp
        out    dx, al
        xor    eax, eax
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_3370:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    eax, dword ptr [ecx + 0xc]
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 0xc]
        push   edi
        lea    esi, [eax + 4]
        lea    edi, [eax + 1]
        add    ebx, 4
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 8], ebx
        test   byte ptr [ecx + 0x48], 2
        mov    eax, dword ptr [ecx + 0x120]
        mov    dword ptr [ebp - 0x10], eax
        je     loc_1_3414
        cmp    dword ptr [eax + 0x18], 0
        je     loc_1_3414
        mov    edx, edi
        mov    al, 8
        out    dx, al
        mov    ebx, dword ptr [ebp - 0x14]
        add    ebx, 0xc
        push   ebx
        call   sub_1_0010
        cmp    ax, 1
        jne    loc_1_3414
        mov    edx, edi
        mov    al, 0x40
        out    dx, al
        push   5
        call   sub_1_2240
        push   ebx
        call   0
        and    al, 0xf
        mov    edx, edi
        push   ebx
        mov    byte ptr [ebp - 4], al
        mov    al, 8
        out    dx, al
        call   sub_1_0010
        cmp    ax, 1
        jne    loc_1_3414
        mov    edx, edi
        mov    al, 0x40
        out    dx, al
        push   5
        call   sub_1_2240
        push   ebx
        call   0
        and    al, 0xf
        cmp    al, byte ptr [ebp - 4]
        jne    loc_1_3414
        mov    eax, dword ptr [ebp - 0x14]
        lea    edx, [eax + 8]
        mov    al, 0x80
        out    dx, al
        mov    al, 1
        out    dx, al
        mov    al, 0x64
        out    dx, al
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 0x50
        out    dx, al
loc_1_3414:
        mov    edx, dword ptr [ebp - 0x14]
        xor    al, al
        out    dx, al
        mov    edx, dword ptr [ebp - 8]
        cmp    dword ptr [edx + 0x10], 1
        jne    loc_1_3430
        mov    eax, dword ptr [edx + 0x14]
        cmp    eax, 2
        je     loc_1_3485
        cmp    eax, 3
        je     loc_1_3485
loc_1_3430:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        mov    ebx, dword ptr [edx + 0xc]
        mov    edx, edi
        out    dx, al
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 8
        push   eax
        call   sub_1_0010
        and    eax, 0x1fff
        sub    eax, 2
        cmp    eax, ebx
        jae    loc_1_346B
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 0x11
        pop    edi
        pop    esi
        mov    dword ptr [edx + 4], 5
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_346B:
        mov    edx, esi
        cmp    ebx, 0x3c
        jae    loc_1_3480
        mov    eax, 0x3c
        out    dx, ax
        sub    eax, ebx
        mov    dword ptr [ebp - 0x18], eax
        jmp    loc_1_348C
loc_1_3480:
        mov    ax, bx
        out    dx, ax
loc_1_3485:
        mov    dword ptr [ebp - 0x18], 0
loc_1_348C:
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], 0
        cmp    dword ptr [edx + 0x18], 0
        jle    loc_1_34F4
        lea    edi, [edx + 0x20]
loc_1_349F:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x55], 0
        mov    eax, dword ptr [edi]
        je     loc_1_34B7
        push   eax
        push   ebx
        push   esi
        call   sub_1_4F80
        jmp    loc_1_34E3
loc_1_34B7:
        cdq    
        mov    dword ptr [ebp - 4], eax
        sub    eax, edx
        sar    eax, 1
        push   eax
        push   ebx
        push   esi
        call   sub_1_4F94
        mov    eax, dword ptr [ebp - 4]
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 1
        xor    eax, edx
        sub    eax, edx
        je     loc_1_34E3
        mov    edx, esi
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [ebx + eax - 1]
        out    dx, al
loc_1_34E3:
        add    edi, 0xc
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [edx + 0x18], eax
        jg     loc_1_349F
loc_1_34F4:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_352E
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x55], 0
        mov    eax, dword ptr [ebp - 0x18]
        je     loc_1_3514
        push   eax
        push   0                             ; -> obj4:0x0914
        push   esi
        call   sub_1_4F80
        jmp    loc_1_352E
loc_1_3514:
        shr    eax, 1
        push   eax
        push   0                             ; -> obj4:0x0914
        push   esi
        call   sub_1_4F94
        test   byte ptr [ebp - 0x18], 1
        je     loc_1_352E
        mov    edx, esi
        xor    al, al
        out    dx, al
loc_1_352E:
        mov    edx, dword ptr [ebp - 8]
        cmp    dword ptr [edx + 0x10], 1
        jne    loc_1_3554
        mov    eax, dword ptr [edx + 0x14]
        test   eax, eax
        je     loc_1_3543
        cmp    eax, 3
        jne    loc_1_3554
loc_1_3543:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x4d], 1
        mov    dword ptr [eax + 0x51], 0
loc_1_3554:
        mov    edx, dword ptr [ebp - 0x14]
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3570:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        mov    eax, dword ptr [ecx + 0xc]
        push   ebx
        push   esi
        mov    edx, dword ptr [ecx + 0x38]
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edi, [eax + 1]
        add    esi, 4
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0xc], edx
        add    eax, 4
        test   byte ptr [ecx + 0x48], 2
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ecx + 0x120]
        mov    dword ptr [ebp - 0x10], eax
        je     loc_1_361A
        cmp    dword ptr [eax + 0x18], 0
        je     loc_1_361A
        mov    edx, edi
        mov    al, 8
        out    dx, al
        mov    ebx, dword ptr [ebp - 8]
        add    ebx, 0xc
        push   ebx
        call   sub_1_0010
        cmp    ax, 1
        jne    loc_1_361A
        mov    edx, edi
        mov    al, 0x40
        out    dx, al
        push   5
        call   sub_1_2240
        push   ebx
        call   0
        and    al, 0xf
        mov    edx, edi
        push   ebx
        mov    byte ptr [ebp - 4], al
        mov    al, 8
        out    dx, al
        call   sub_1_0010
        cmp    ax, 1
        jne    loc_1_361A
        mov    edx, edi
        mov    al, 0x40
        out    dx, al
        push   5
        call   sub_1_2240
        push   ebx
        call   0
        and    al, 0xf
        cmp    al, byte ptr [ebp - 4]
        jne    loc_1_361A
        mov    eax, dword ptr [ebp - 8]
        lea    edx, [eax + 8]
        mov    al, 0x80
        out    dx, al
        mov    al, 1
        out    dx, al
        mov    al, 0x64
        out    dx, al
        mov    edx, dword ptr [ebp - 8]
        mov    al, 0x50
        out    dx, al
loc_1_361A:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        cmp    dword ptr [esi + 0x10], 1
        jne    loc_1_3633
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 2
        je     loc_1_3683
        cmp    eax, 3
        je     loc_1_3683
loc_1_3633:
        mov    ebx, dword ptr [esi + 0xc]
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        call   sub_1_0010
        and    eax, 0x1fff
        sub    eax, 2
        cmp    eax, ebx
        jae    loc_1_3668
        mov    eax, 0x11
        pop    edi
        mov    dword ptr [esi + 4], 5
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3668:
        mov    edx, dword ptr [ebp - 0x14]
        cmp    ebx, 0x3c
        jae    loc_1_367E
        mov    eax, 0x3c
        out    dx, ax
        sub    eax, ebx
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_368A
loc_1_367E:
        mov    ax, bx
        out    dx, ax
loc_1_3683:
        mov    dword ptr [ebp - 4], 0
loc_1_368A:
        xor    edi, edi
        cmp    dword ptr [esi + 0x18], edi
        jle    loc_1_36AD
        lea    ebx, [esi + 0x20]
loc_1_3694:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        add    ebx, 0xc
        push   edx
        inc    edi
        call   sub_1_51CC
        cmp    dword ptr [esi + 0x18], edi
        jg     loc_1_3694
loc_1_36AD:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_36C5
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   0                             ; -> obj4:0x0950
        push   ecx
        call   sub_1_51CC
loc_1_36C5:
        cmp    dword ptr [esi + 0x10], 1
        jne    loc_1_36E8
        mov    eax, dword ptr [esi + 0x14]
        test   eax, eax
        je     loc_1_36D7
        cmp    eax, 3
        jne    loc_1_36E8
loc_1_36D7:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x4d], 1
        mov    dword ptr [eax + 0x51], 0
loc_1_36E8:
        mov    edx, dword ptr [ebp - 8]
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3700:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x128]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3720:
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3730:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    dword ptr [eax + 4], 0x1f
        xor    eax, eax
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3750:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    dword ptr [eax + 4], 0x3df
        xor    eax, eax
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3770:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    dword ptr [eax + 4], 9
        xor    eax, eax
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3790:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    ecx, dword ptr [edx + 0xc]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 4], ecx
        push   edi
        inc    ecx
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 0x120]
        add    esi, 4
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x10], ecx
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edx + 4]
        add    ecx, 0x10
        add    eax, dword ptr [edx + 8]
        mov    dword ptr [ebp - 0x14], ecx
        mov    dword ptr [ebp - 0xc], eax
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx], 0
        jne    loc_1_380F
        mov    edx, dword ptr [ebp - 0x10]
        mov    al, 5
        out    dx, al
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xa
        push   eax
        call   sub_1_0010
        mov    ecx, eax
        and    ecx, 0xffff
        add    ecx, 3
        cmp    ecx, 0x8000
        jne    loc_1_37FA
        xor    ecx, ecx
loc_1_37FA:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, ecx
        cmp    eax, 0x8000
        jle    loc_1_380F
        mov    edi, 0x8000
        sub    edi, ecx
        jmp    loc_1_3811
loc_1_380F:
        xor    edi, edi
loc_1_3811:
        mov    edx, dword ptr [ebp - 0x10]
        xor    al, al
        mov    ecx, dword ptr [ebp + 0xc]
        out    dx, al
        mov    eax, dword ptr [ecx + 8]
        test   eax, eax
        jle    loc_1_3830
        and    ax, 0x1fff
        mov    edx, dword ptr [ebp - 4]
        add    edx, 0xc
        or     ah, 0x20
        out    dx, ax
loc_1_3830:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        je     loc_1_3842
        cmp    eax, 1
        jmp    loc_1_38CE
loc_1_3842:
        mov    eax, dword ptr [ebp - 0x14]
        test   edi, edi
        mov    ebx, dword ptr [eax + 0x18]
        jle    loc_1_3891
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 0x55], 0
        je     loc_1_385F
        push   edi
        push   ebx
        push   esi
        call   sub_1_4FBD
        jmp    loc_1_3889
loc_1_385F:
        mov    eax, edi
        cdq    
        sub    eax, edx
        sar    eax, 1
        push   eax
        push   ebx
        push   esi
        call   sub_1_4FD1
        mov    eax, edi
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 1
        xor    eax, edx
        sub    eax, edx
        je     loc_1_3889
        push   esi
        call   0
        mov    byte ptr [edi + ebx - 1], al
loc_1_3889:
        push   esi
        add    ebx, edi
        call   0
loc_1_3891:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ecx + 0x55], 0
        je     loc_1_38A9
        sub    eax, edi
        push   eax
        push   ebx
        push   esi
        call   sub_1_4FBD
        jmp    loc_1_38CE
loc_1_38A9:
        sub    eax, edi
        mov    dword ptr [ebp - 0x18], eax
        shr    eax, 1
        push   eax
        push   ebx
        push   esi
        call   sub_1_4FD1
        test   byte ptr [ebp - 0x18], 1
        je     loc_1_38CE
        push   esi
        sub    ebx, edi
        call   0
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ebx + ecx - 1], al
loc_1_38CE:
        xor    edi, edi
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 4], edi
        cmp    dword ptr [ecx + 0xc], edi
        jne    loc_1_3913
        mov    edx, dword ptr [ebp - 0x10]
        xor    al, al
        mov    esi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    esi, 0xc
        push   ecx
        out    dx, al
        mov    edx, esi
        mov    eax, 0x8000
        out    dx, ax
        call   0
        test   al, 1
        jne    loc_1_3906
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xc], edi
loc_1_3906:
        test   al, 4
        je     loc_1_3913
        mov    edx, esi
        mov    eax, 0x2000
        out    dx, ax
loc_1_3913:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3920:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 0xc]
        mov    ebx, dword ptr [ecx + 0x38]
        lea    eax, [esi + 1]
        lea    edx, [esi + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx + 0x120]
        add    eax, 0x10
        mov    dword ptr [ebp - 0x18], edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x14], ebx
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0x10], ecx
        mov    edi, dword ptr [edx + 4]
        add    edi, dword ptr [edx + 8]
        cmp    dword ptr [ecx], 0
        jne    loc_1_3999
        mov    edx, dword ptr [ebp - 4]
        mov    al, 5
        out    dx, al
        lea    eax, [esi + 0xa]
        push   eax
        call   sub_1_0010
        mov    ecx, eax
        and    ecx, 0xffff
        add    ecx, 3
        cmp    ecx, 0x8000
        jne    loc_1_3986
        xor    ecx, ecx
loc_1_3986:
        lea    eax, [edi + ecx]
        cmp    eax, 0x8000
        jle    loc_1_3999
        mov    ebx, 0x8000
        sub    ebx, ecx
        jmp    loc_1_399B
loc_1_3999:
        xor    ebx, ebx
loc_1_399B:
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jle    loc_1_39B7
        and    ax, 0x1fff
        lea    edx, [esi + 0xc]
        or     ah, 0x20
        out    dx, ax
loc_1_39B7:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        je     loc_1_39C6
        cmp    eax, 1
        jmp    loc_1_39FA
loc_1_39C6:
        mov    eax, dword ptr [ebp - 8]
        test   ebx, ebx
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0xc], ecx
        jle    loc_1_39EA
        push   ebx
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   ecx
        call   sub_1_51CC
        mov    ecx, dword ptr [ebp - 0x18]
        push   ecx
        call   0
        add    dword ptr [ebp - 0xc], ebx
loc_1_39EA:
        sub    edi, ebx
        mov    eax, dword ptr [ebp - 0x14]
        push   edi
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_51CC
loc_1_39FA:
        xor    edi, edi
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 4], edi
        cmp    dword ptr [ecx + 0xc], edi
        jne    loc_1_3A39
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        lea    ebx, [esi + 0xc]
        push   esi
        out    dx, al
        mov    edx, ebx
        mov    eax, 0x8000
        out    dx, ax
        call   0
        test   al, 1
        jne    loc_1_3A2C
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0xc], edi
loc_1_3A2C:
        test   al, 4
        je     loc_1_3A39
        mov    edx, ebx
        mov    eax, 0x2000
        out    dx, ax
loc_1_3A39:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A50:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        add    edx, 4
        jne    loc_1_3A62
        xor    eax, eax
        pop    ebp
        ret    8
loc_1_3A62:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x12c], 0
        jne    loc_1_3A76
        mov    dword ptr [ecx + 0x12c], edx
        jmp    loc_1_3A7E
loc_1_3A76:
        mov    eax, dword ptr [ecx + 0x130]
        mov    dword ptr [eax], edx
loc_1_3A7E:
        mov    eax, 2
        mov    dword ptr [edx + 4], eax
        cmp    dword ptr [edx], 0
        je     loc_1_3A95
loc_1_3A8B:
        mov    edx, dword ptr [edx]
        mov    dword ptr [edx + 4], eax
        cmp    dword ptr [edx], 0
        jne    loc_1_3A8B
loc_1_3A95:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 0x130], edx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3AB0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 4]
        test   ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        je     loc_1_3ACF
        test   dword ptr [eax + 0x44], ecx
        jne    loc_1_3ACF
        mov    eax, 0xd
        pop    ebp
        ret    8
loc_1_3ACF:
        pop    ebp
        mov    dword ptr [eax + 0x48], ecx
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3AE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x48]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B00:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 5
        ja     loc_1_3B1C
        jmp    dword ptr [eax*4]             ; -> obj1:0x3B30
loc_1_3B16:
        xor    eax, eax
        pop    ebp
        ret    8
loc_1_3B1C:
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_3B25:
        mov    eax, 0xf
        pop    ebp
        ret    8
        db  08Bh, 0FFh
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_3B16
        dd  loc_1_3B25
        dd  loc_1_3B25
        dd  loc_1_3B25
        dd  loc_1_3B16
        dd  loc_1_3B25
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3B50:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    dword ptr [eax + 4], 5
        jbe    loc_1_3B66
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_3B66:
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_3B70:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax + 4]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 8]
        push   edi
        mov    dword ptr [ebp - 8], ecx
        mov    esi, dword ptr [edx + 0x6c]
        mov    edi, dword ptr [edx + 0xc]
        test   ecx, ecx
        lea    eax, [edi + 1]
        mov    dword ptr [ebp - 4], eax
        je     loc_1_3BA9
        test   dword ptr [edx + 0x68], ecx
        jne    loc_1_3BA9
        mov    eax, 0xd
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3BA9:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, eax
        mov    dword ptr [ecx + 0x6c], eax
        not    ebx
        and    ebx, esi
        test   bl, 4
        je     loc_1_3BDF
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x42
        out    dx, al
        push   0xa
        call   sub_1_2240
        lea    ecx, [edi + 9]
        push   ecx
        call   0
        and    al, 0xfb
        lea    edx, [edi + 9]
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3BDF:
        test   bl, 0x10
        je     loc_1_3C12
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0xbc], 0
        jle    loc_1_3C12
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x42
        out    dx, al
        push   0xa
        call   sub_1_2240
        lea    ecx, [edi + 9]
        push   ecx
        call   0
        and    al, 0xfe
        lea    edx, [edi + 9]
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3C12:
        test   bl, 8
        je     loc_1_3C38
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x42
        push   0xa
        lea    ebx, [edi + 9]
        out    dx, al
        call   sub_1_2240
        push   ebx
        call   0
        or     al, 8
        mov    edx, ebx
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3C38:
        not    esi
        and    esi, dword ptr [ebp - 8]
        test   esi, 4
        je     loc_1_3C66
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x42
        push   0xa
        lea    ebx, [edi + 9]
        out    dx, al
        call   sub_1_2240
        push   ebx
        call   0
        or     al, 4
        mov    edx, ebx
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3C66:
        test   esi, 0x10
        je     loc_1_3C9B
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0xbc], 0
        jle    loc_1_3C9B
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x42
        push   0xa
        lea    ebx, [edi + 9]
        out    dx, al
        call   sub_1_2240
        push   ebx
        call   0
        or     al, 1
        mov    edx, ebx
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3C9B:
        test   esi, 8
        je     loc_1_3CC4
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x42
        push   0xa
        add    edi, 9
        out    dx, al
        call   sub_1_2240
        push   edi
        call   0
        and    al, 0xf7
        mov    edx, edi
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3CC4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_3CD0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x6c]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3CF0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 4
        ja     loc_1_3D06
        jmp    dword ptr [eax*4]             ; -> obj1:0x3D20
loc_1_3D06:
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_3D0F:
        mov    eax, 0xf
        pop    ebp
        ret    8
loc_1_3D18:
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Bh, 0FFh
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_3D0F
        dd  loc_1_3D0F
        dd  loc_1_3D18
        dd  loc_1_3D0F
        dd  loc_1_3D0F
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3D40:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    dword ptr [eax + 4], 4
        jbe    loc_1_3D56
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_3D56:
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_3D60:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 4], eax
        lea    edx, [eax + 1]
        xor    eax, eax
        mov    dword ptr [ebp - 8], eax
loc_1_3D7D:
        mov    cl, byte ptr [edi + eax + 4]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [esi + eax + 0xb1], cl
        jl     loc_1_3D7D
        mov    al, 0x50
        xor    esi, esi
        out    dx, al
        mov    ebx, 8
loc_1_3D98:
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [eax + ebx]
        mov    al, byte ptr [edi + esi + 4]
        out    dx, al
        push   edx
        call   0
        cmp    al, byte ptr [edi + esi + 4]
        jne    loc_1_3DB8
        inc    esi
        inc    ebx
        cmp    esi, 6
        jl     loc_1_3D98
        jmp    loc_1_3DBF
loc_1_3DB8:
        mov    dword ptr [ebp - 8], 1
loc_1_3DBF:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        dec    eax
        pop    esi
        pop    ebx
        cmp    eax, 1
        sbb    eax, eax
        mov    esp, ebp
        inc    eax
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3DE0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_3DEC:
        mov    bl, byte ptr [eax + edx + 0xb2]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [ecx + edx + 3], bl
        jl     loc_1_3DEC
        xor    eax, eax
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3E10:
        mov    eax, 0xc
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3E20:
        mov    eax, 0xc
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3E30:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        je     loc_1_3E51
        cmp    eax, 1
        je     loc_1_3E5E
        cmp    eax, 2
        je     loc_1_3E6B
        mov    eax, 6
        pop    ebp
        ret    8
loc_1_3E51:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E80
        pop    ebp
        ret    8
loc_1_3E5E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3EB0
        pop    ebp
        ret    8
loc_1_3E6B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3EE0
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3E80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0xc]
        mov    al, 1
        add    esi, 0xc
        push   esi
        lea    edx, [esi - 0xb]
        out    dx, al
        call   0
        or     al, 3
        mov    edx, esi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3EB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0xc]
        mov    al, 1
        add    esi, 0xc
        push   esi
        lea    edx, [esi - 0xb]
        out    dx, al
        call   0
        or     al, 0x70
        mov    edx, esi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3EE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 0xc]
        mov    al, 4
        pop    ebp
        out    dx, al
        xor    eax, eax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3F00:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        je     loc_1_3F21
        cmp    eax, 1
        je     loc_1_3F2E
        cmp    eax, 2
        je     loc_1_3F3B
        mov    eax, 6
        pop    ebp
        ret    8
loc_1_3F21:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3F50
        pop    ebp
        ret    8
loc_1_3F2E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3F80
        pop    ebp
        ret    8
loc_1_3F3B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3FB0
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3F50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0xc]
        mov    al, 1
        add    esi, 0xc
        push   esi
        lea    edx, [esi - 0xb]
        out    dx, al
        call   0
        and    al, 0xfc
        mov    edx, esi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3F80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0xc]
        mov    al, 1
        add    esi, 0xc
        push   esi
        lea    edx, [esi - 0xb]
        out    dx, al
        call   0
        and    al, 0x8f
        mov    edx, esi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3FB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 0xc]
        xor    al, al
        pop    ebp
        out    dx, al
        xor    eax, eax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3FD0:
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3FE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0xc]
        push   1
        mov    al, 4
        lea    edx, [esi + 1]
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4010:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 4]
        test   ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x8c]
        je     loc_1_4038
        test   dword ptr [eax + 0x88], ecx
        jne    loc_1_4038
        mov    eax, 4
        pop    ebp
        ret    8
loc_1_4038:
        mov    dword ptr [eax + 0x8c], ecx
        mov    eax, ecx
        not    eax
        and    eax, edx
        test   al, 1
        je     loc_1_405C
        mov    dword ptr [0], 0              ; -> sub_1_5860
        mov    dword ptr [0], 0              ; -> sub_1_57A8
loc_1_405C:
        not    edx
        and    edx, ecx
        test   dl, 1
        je     loc_1_4079
        mov    dword ptr [0], 0              ; -> sub_1_5860
        mov    dword ptr [0], 0              ; -> sub_1_57A8
loc_1_4079:
        xor    eax, eax
        pop    ebp
        ret    8
        db  090h
sub_1_4080:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x8c]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40A0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        je     loc_1_40C1
        cmp    eax, 1
        jl     loc_1_40B8
        cmp    eax, 2
        jle    loc_1_40C7
loc_1_40B8:
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_40C1:
        xor    eax, eax
        pop    ebp
        ret    8
loc_1_40C7:
        mov    eax, 0xf
        pop    ebp
        ret    8
sub_1_40D0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        cmp    dword ptr [eax + 4], 2
        jbe    loc_1_40E6
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_40E6:
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_40F0:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 4]
        dec    eax
        cmp    eax, 0x2e
        ja     loc_1_4107
        jmp    dword ptr [eax*4]             ; -> obj1:0x4368
loc_1_4107:
        mov    eax, 3
        pop    ebp
        ret    8
loc_1_4110:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_2BC0
        ret    8
loc_1_411D:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3310
        ret    8
loc_1_412A:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3340
        ret    8
loc_1_4137:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4430
        ret    8
loc_1_4144:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4440
        ret    8
loc_1_4151:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_32D0
        ret    8
loc_1_415E:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3A50
        ret    8
loc_1_416B:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_32F0
        ret    8
loc_1_4178:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3E10
        ret    8
loc_1_4185:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3E20
        ret    8
loc_1_4192:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3F00
        ret    8
loc_1_419F:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3E30
        ret    8
loc_1_41AC:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3FD0
        ret    8
loc_1_41B9:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3FE0
        ret    8
loc_1_41C6:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3700
        ret    8
loc_1_41D3:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3720
        ret    8
loc_1_41E0:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4450
        ret    8
loc_1_41ED:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3730
        ret    8
loc_1_41FA:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3750
        ret    8
loc_1_4207:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3770
        ret    8
loc_1_4214:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3AB0
        ret    8
loc_1_4221:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3B70
        ret    8
loc_1_422E:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4010
        ret    8
loc_1_423B:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3AE0
        ret    8
loc_1_4248:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3CD0
        ret    8
loc_1_4255:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4080
        ret    8
loc_1_4262:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3B00
        ret    8
loc_1_426F:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3CF0
        ret    8
loc_1_427C:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_40A0
        ret    8
loc_1_4289:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3B50
        ret    8
loc_1_4296:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3D40
        ret    8
loc_1_42A3:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_40D0
        ret    8
loc_1_42B0:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4460
        ret    8
loc_1_42BD:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3DE0
        ret    8
loc_1_42CA:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_3D60
        ret    8
loc_1_42D7:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4780
        ret    8
loc_1_42E4:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_47A0
        ret    8
loc_1_42F1:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4490
        ret    8
loc_1_42FE:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4630
        ret    8
loc_1_430B:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0
        ret    8
loc_1_4318:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0
        ret    8
loc_1_4325:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0
        ret    8
loc_1_4332:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4910
        ret    8
loc_1_433F:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_40F0
        ret    8
loc_1_434C:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_4810
        ret    8
loc_1_4359:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0        ; -> sub_1_47F0
        ret    8
        db  08Bh, 0FFh
        ; ---- dispatch table (47 entries) ----
        dd  loc_1_4110
        dd  loc_1_411D
        dd  loc_1_412A
        dd  loc_1_4137
        dd  loc_1_4144
        dd  loc_1_4151
        dd  loc_1_415E
        dd  loc_1_416B
        dd  loc_1_4178
        dd  loc_1_4185
        dd  loc_1_4192
        dd  loc_1_419F
        dd  loc_1_41AC
        dd  loc_1_41B9
        dd  loc_1_41C6
        dd  loc_1_41D3
        dd  loc_1_41E0
        dd  loc_1_4107
        dd  loc_1_41ED
        dd  loc_1_41FA
        dd  loc_1_4207
        dd  loc_1_4214
        dd  loc_1_4221
        dd  loc_1_422E
        dd  loc_1_423B
        dd  loc_1_4248
        dd  loc_1_4255
        dd  loc_1_4262
        dd  loc_1_426F
        dd  loc_1_427C
        dd  loc_1_4289
        dd  loc_1_4296
        dd  loc_1_42A3
        dd  loc_1_42B0
        dd  loc_1_42BD
        dd  loc_1_42CA
        dd  loc_1_42D7
        dd  loc_1_42E4
        dd  loc_1_42F1
        dd  loc_1_42FE
        dd  loc_1_430B
        dd  loc_1_4318
        dd  loc_1_4325
        dd  loc_1_4332
        dd  loc_1_433F
        dd  loc_1_434C
        dd  loc_1_4359
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4430:
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4440:
        mov    eax, 0xc
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4450:
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4460:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_446C:
        mov    bl, byte ptr [eax + edx + 0xac]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [ecx + edx + 3], bl
        jl     loc_1_446C
        xor    eax, eax
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4490:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0xffffffff
        push   esi
        mov    eax, dword ptr [edx + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        add    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        inc    eax
        test   byte ptr [esi], 1
        mov    dword ptr [ebp - 0x10], eax
        jne    loc_1_44C6
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_44C6:
        xor    ecx, ecx
        mov    edi, dword ptr [ebp + 8]
        lea    eax, [edi + 0xf6]
loc_1_44D1:
        cmp    dword ptr [eax], 0
        jne    loc_1_44E1
        cmp    dword ptr [ebp - 4], -1
        jne    loc_1_450F
        mov    dword ptr [ebp - 4], ecx
        jmp    loc_1_450F
loc_1_44E1:
        mov    dword ptr [ebp - 8], 1
        xor    ebx, ebx
loc_1_44EA:
        mov    dl, byte ptr [esi + ebx]
        cmp    byte ptr [edi + ebx + 0xc0], dl
        jne    loc_1_44FE
        inc    ebx
        cmp    ebx, 6
        jl     loc_1_44EA
        jmp    loc_1_4505
loc_1_44FE:
        mov    dword ptr [ebp - 8], 0
loc_1_4505:
        cmp    dword ptr [ebp - 8], 1
        je     loc_1_45FB
loc_1_450F:
        add    edi, 6
        add    eax, 4
        inc    ecx
        cmp    ecx, 9
        jl     loc_1_44D1
        cmp    dword ptr [ebp - 4], 0
        jl     loc_1_461E
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        lea    ebx, [eax + eax*2]
        lea    eax, [edx + ebx*2]
loc_1_4533:
        mov    dl, byte ptr [esi + ecx]
        inc    ecx
        cmp    ecx, 6
        mov    byte ptr [eax + ecx + 0xbf], dl
        jl     loc_1_4533
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        push   0xc8
        mov    edi, dword ptr [edx + 0x120]
        mov    edx, dword ptr [ebp - 0x10]
        lea    ecx, [eax + edi + 0x20]
        mov    dword ptr [ebp - 8], ecx
        mov    al, byte ptr [ecx]
        out    dx, al
        call   sub_1_2240
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [edi + ecx*4 + 0x29]
        xor    edi, edi
loc_1_456F:
        mov    eax, dword ptr [ebp - 0xc]
        lea    edx, [eax + ebx]
        mov    al, byte ptr [esi + edi]
        out    dx, al
        push   edx
        call   0
        cmp    al, byte ptr [esi + edi]
        jne    loc_1_4610
        inc    ebx
        cmp    ebx, 0x10
        jl     loc_1_45A8
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0x10]
        push   0xc8
        mov    ebx, 8
        mov    al, byte ptr [eax]
        inc    al
        out    dx, al
        call   sub_1_2240
loc_1_45A8:
        inc    edi
        cmp    edi, 6
        jl     loc_1_456F
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [edx + eax*4 + 0xf6]
        mov    eax, dword ptr [edx + 0xbc]
        inc    eax
        mov    dword ptr [edx + 0xbc], eax
        test   eax, eax
        jle    loc_1_45F0
        test   byte ptr [edx + 0x6c], 0x10
        je     loc_1_45F0
        mov    edx, dword ptr [ebp - 0x10]
        mov    al, 0x42
        out    dx, al
        mov    esi, dword ptr [ebp - 0xc]
        add    esi, 9
        push   esi
        call   0
        or     al, 1
        mov    edx, esi
        out    dx, al
        mov    edx, dword ptr [ebp + 8]
        or     dword ptr [edx + 0x6c], 0x10
loc_1_45F0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_45FB:
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [edx + ecx*4 + 0xf6]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4610:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_461E:
        mov    eax, 0x10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_4630:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, dword ptr [ecx + 0xc]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 8], eax
        push   edi
        inc    eax
        cmp    dword ptr [ecx + 0xbc], 0
        mov    dword ptr [ebp - 0xc], eax
        jne    loc_1_465E
        mov    eax, 0xe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_465E:
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax + 4], 1
        jne    loc_1_4675
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4675:
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        lea    ebx, [edi + 0xf6]
loc_1_4680:
        cmp    dword ptr [ebx], 0
        je     loc_1_46B0
        mov    dword ptr [ebp - 4], 1
        xor    ecx, ecx
loc_1_468E:
        mov    dl, byte ptr [eax + ecx + 4]
        cmp    byte ptr [ecx + edi + 0xc0], dl
        jne    loc_1_46A3
        inc    ecx
        cmp    ecx, 6
        jl     loc_1_468E
        jmp    loc_1_46AA
loc_1_46A3:
        mov    dword ptr [ebp - 4], 0
loc_1_46AA:
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_46C7
loc_1_46B0:
        add    edi, 6
        add    ebx, 4
        inc    esi
        cmp    esi, 9
        jl     loc_1_4680
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_46C7:
        mov    ecx, dword ptr [ebp + 8]
        lea    ecx, [ecx + esi*4 + 0xf6]
        mov    eax, dword ptr [ecx]
        dec    eax
        mov    dword ptr [ecx], eax
        jne    loc_1_4770
        lea    eax, [esi + esi*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0xc]
        lea    eax, [ecx + eax*2 + 0xc0]
        push   0xc8
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    ebx, dword ptr [ecx + 0x120]
        lea    edi, [esi + ebx + 0x20]
        mov    al, byte ptr [edi]
        out    dx, al
        call   sub_1_2240
        mov    ecx, dword ptr [ebx + esi*4 + 0x29]
        mov    esi, 6
loc_1_4718:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        add    edx, ecx
        inc    ecx
        out    dx, al
        cmp    ecx, 0x10
        jl     loc_1_473D
        mov    al, byte ptr [edi]
        mov    edx, dword ptr [ebp - 0xc]
        inc    al
        push   0xc8
        out    dx, al
        call   sub_1_2240
        mov    ecx, 8
loc_1_473D:
        dec    esi
        jne    loc_1_4718
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0xbc]
        dec    eax
        mov    dword ptr [ecx + 0xbc], eax
        jne    loc_1_4770
        mov    edx, dword ptr [ebp - 0xc]
        mov    al, 0x42
        out    dx, al
        mov    esi, dword ptr [ebp - 8]
        add    esi, 9
        push   esi
        call   0
        and    al, 0xfe
        mov    edx, esi
        mov    ecx, dword ptr [ebp + 8]
        out    dx, al
        and    dword ptr [ecx + 0x6c], 0xffffffef
loc_1_4770:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_4780:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0xbc]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0xc]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_47A0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 4]
        lea    eax, [edx + 0xf6]
loc_1_47B7:
        cmp    dword ptr [eax], 0
        je     loc_1_47D5
        xor    edi, edi
loc_1_47BE:
        lea    ebx, [ecx + edi]
        inc    edi
        cmp    edi, 6
        mov    bl, byte ptr [ebx + edx + 0xc0]
        mov    byte ptr [esi + edi - 1], bl
        jl     loc_1_47BE
        add    esi, 6
loc_1_47D5:
        add    ecx, 6
        add    eax, 4
        cmp    ecx, 0x24
        jl     loc_1_47B7
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_47F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    dword ptr [eax + 0x128], 7
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4810:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 4]
        cmp    eax, 9
        ja     loc_1_4826
        jmp    dword ptr [eax*4]             ; -> obj1:0x48E0
loc_1_4826:
        mov    eax, 7
        pop    ebp
        ret    8
loc_1_482F:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0xc], eax
        or     dword ptr [ecx + 8], 1
        xor    eax, eax
        ret    8
loc_1_4842:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x24], eax
        or     dword ptr [ecx + 8], 2
        xor    eax, eax
        ret    8
loc_1_4855:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x18], eax
        or     dword ptr [ecx + 8], 4
        xor    eax, eax
        ret    8
loc_1_4868:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x30], eax
        or     dword ptr [ecx + 8], 8
        xor    eax, eax
        ret    8
loc_1_487B:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x20], eax
        xor    eax, eax
        ret    8
loc_1_488A:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x38], eax
        xor    eax, eax
        ret    8
loc_1_4899:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x1c], eax
        xor    eax, eax
        ret    8
loc_1_48A8:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x34], eax
        xor    eax, eax
        ret    8
loc_1_48B7:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x14], eax
        or     dword ptr [ecx + 8], 0x10
        xor    eax, eax
        ret    8
loc_1_48CA:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0x2c], eax
        or     dword ptr [ecx + 8], 0x20
        xor    eax, eax
        ret    8
        db  08Dh, 049h, 000h
        ; ---- dispatch table (10 entries) ----
        dd  loc_1_482F
        dd  loc_1_4842
        dd  loc_1_4855
        dd  loc_1_4868
        dd  loc_1_487B
        dd  loc_1_488A
        dd  loc_1_4899
        dd  loc_1_48A8
        dd  loc_1_48B7
        dd  loc_1_48CA
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4910:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0xa
        xor    eax, eax
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_4930:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        mov    dword ptr [ecx + 0x128], 3
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 0xc]
        mov    edx, esi
        lea    eax, [esi + 1]
        push   0xffff
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ecx + 0x120]
        mov    dword ptr [ebp - 0xc], eax
        mov    al, 2
        out    dx, al
        call   sub_1_2240
        mov    edx, esi
        xor    al, al
        out    dx, al
        push   0xffff
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 4]
        mov    al, 4
        out    dx, al
        lea    edx, [esi + 8]
        push   0xea60
        out    dx, al
        call   sub_1_2240
        push   esi
        call   0
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x40
        out    dx, al
        push   5
        call   sub_1_2240
        lea    edx, [esi + 0xd]
        mov    al, 0x80
        out    dx, al
        lea    edx, [esi + 0xe]
        out    dx, al
        lea    edx, [esi + 0xf]
        mov    al, 0x20
        out    dx, al
        lea    eax, [esi + 9]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0xb]
        out    dx, al
        lea    edx, [esi + 0xc]
        out    dx, al
        mov    edx, dword ptr [ebp - 0xc]
        cmp    dword ptr [edx], 0
        jne    loc_1_49DA
        mov    edx, dword ptr [ebp - 4]
        mov    al, 2
        out    dx, al
        lea    eax, [esi + 8]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [esi + 9]
        mov    al, 0x20
        out    dx, al
loc_1_49DA:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0x6c]
        mov    edi, eax
        mov    dword ptr [ebp - 8], eax
        and    edi, 2
        je     loc_1_49FB
        mov    edx, dword ptr [ebp - 4]
        mov    al, 3
        out    dx, al
        lea    eax, [esi + 8]
        mov    edx, eax
        mov    ax, word ptr [ecx + 0x7c]
        out    dx, ax
loc_1_49FB:
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ecx + 0x48]
        and    ebx, 2
        je     loc_1_4A15
        mov    edx, dword ptr [ebp - 4]
        mov    al, 3
        out    dx, al
        mov    ax, word ptr [ecx + 0x60]
        lea    edx, [esi + 0xa]
        out    dx, ax
loc_1_4A15:
        mov    edx, dword ptr [ebp - 4]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0xd]
        out    dx, al
        xor    al, al
        test   ebx, ebx
        je     loc_1_4A27
        mov    al, 1
loc_1_4A27:
        test   edi, edi
        je     loc_1_4A2D
        or     al, 2
loc_1_4A2D:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 0x8c], 8
        je     loc_1_4A3B
        or     al, 4
loc_1_4A3B:
        lea    edx, [esi + 0xe]
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x42
        out    dx, al
        lea    eax, [esi + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        mov    al, 0x20
        out    dx, al
        cmp    dword ptr [ecx + 0xbc], 0
        mov    al, 0x10
        jle    loc_1_4A63
        test   byte ptr [ebp - 8], 0x10
        je     loc_1_4A63
        mov    al, 0x11
loc_1_4A63:
        test   byte ptr [ebp - 8], 4
        je     loc_1_4A6B
        or     al, 4
loc_1_4A6B:
        test   byte ptr [ebp - 8], 8
        jne    loc_1_4A73
        or     al, 8
loc_1_4A73:
        lea    edx, [esi + 9]
        push   esi
        out    dx, al
        call   0
        mov    edx, dword ptr [ebp - 4]
        mov    al, 1
        out    dx, al
        lea    ecx, [esi + 0xc]
        push   ecx
        call   0
        or     al, 3
        lea    edx, [esi + 0xc]
        or     al, 0x70
        push   0xc8
        xor    edi, edi
        mov    ebx, 8
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x50
        out    dx, al
        call   sub_1_2240
loc_1_4AAB:
        lea    edx, [esi + ebx]
        mov    ecx, dword ptr [ebp + 8]
        inc    edi
        inc    ebx
        cmp    edi, 6
        mov    al, byte ptr [ecx + edi + 0xb1]
        out    dx, al
        jl     loc_1_4AAB
        push   esi
        xor    edi, edi
        call   0
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], edi
        add    ecx, 0x29
        mov    dword ptr [ebp - 0x14], ecx
loc_1_4AD4:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    ebx, dword ptr [ebp - 0x14]
        lea    edx, [ecx + eax + 0x20]
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebx]
        xor    ebx, ebx
        mov    al, byte ptr [eax]
        out    dx, al
loc_1_4AF1:
        lea    edx, [esi + ecx]
        mov    eax, dword ptr [ebp + 8]
        add    eax, ebx
        inc    ecx
        cmp    ecx, 0x10
        mov    al, byte ptr [eax + edi + 0xc0]
        out    dx, al
        jl     loc_1_4B21
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        push   0xc8
        mov    al, byte ptr [eax]
        inc    al
        out    dx, al
        call   sub_1_2240
        mov    ecx, 8
loc_1_4B21:
        inc    ebx
        cmp    ebx, 6
        jl     loc_1_4AF1
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        add    dword ptr [ebp - 0x14], 4
        cmp    edi, 0x36
        jl     loc_1_4AD4
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
        lea    ecx, [esi + 0xd]
        push   ecx
        call   0
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0xc]
        push   esi
        mov    dword ptr [ecx + 4], eax
        call   0
        push   esi
        call   0
        and    al, 0x20
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    al, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 2
        mov    dword ptr [ecx + 0x3c], eax
        cmp    eax, 2
        mov    al, 2
        out    dx, al
        lea    edx, [esi + 0xa]
        jne    loc_1_4B83
        mov    al, 0x3b
        jmp    loc_1_4B85
loc_1_4B83:
        mov    al, 0x39
loc_1_4B85:
        out    dx, al
        push   0x64
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0xc]
        mov    eax, 0x2000
        out    dx, ax
        mov    edx, dword ptr [ebp - 4]
        mov    al, 0x40
        out    dx, al
        lea    edx, [esi + 8]
        mov    al, 0x24
        out    dx, al
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x4d], eax
        mov    dword ptr [ecx + 0x51], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [ecx + 0x10], eax
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 0x14], eax
        pop    ebx
        mov    dword ptr [ecx + 0x18], eax
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x128], 7
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_4BE0:
        mov    al, byte ptr [0x5278]         ; -> obj1:0x5278
        cmp    al, 0
        jne    loc_1_4C05
        inc    byte ptr [0x5278]             ; -> obj1:0x5278
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_4C07
        push   0
        push   0
        call   sub_1_0030
        cmp    eax, 0
        jne    loc_1_4C07
loc_1_4C05:
        clc    
        ret    
loc_1_4C07:
        stc    
        ret    
CE2NDIS3_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_4C18
        jmp    dword ptr [eax*4 + 0x4c1a]    ; -> obj1:0x4C1A
loc_1_4C18:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_4BE0
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4C18
        dd  loc_1_4BE0
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_4C90:
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
        call   sub_1_4CC0
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_4CC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_4D60
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_4D00
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_4CF5
        cmp    al, 0xff
        jne    loc_1_4CF5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_4CF5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_4D67
loc_1_4CF5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_4D67
        db  08Bh, 0FFh
loc_1_4D00:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_4D50
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_4D26
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_4D2D
loc_1_4D26:
        mov    dword ptr [ebp - 0x10], 1
loc_1_4D2D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_4D40
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_4D6A
        db  08Dh, 064h, 024h, 000h
loc_1_4D40:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_4D6A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4D50:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_4D6A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4D60:
        mov    dword ptr [ebp - 4], 0x20
loc_1_4D67:
        mov    esi, dword ptr [ebp + 8]
loc_1_4D6A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_4E61
loc_1_4D75:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    loc_1_4DCD
        test   al, 2
        je     loc_1_4DB9
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
        je     loc_1_4DB9
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0xc]
        test   dword ptr [ecx + eax*4], edx
        jne    loc_1_4DCD
loc_1_4DB9:
        test   byte ptr [ebp - 4], 2
        je     loc_1_4DC4
        test   bl, 4
        jne    loc_1_4DCD
loc_1_4DC4:
        test   bl, 0x20
        je     loc_1_4E56
loc_1_4DCD:
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
        call   sub_1_5042
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_4E56
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_4E0F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_4E0F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_4E20
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_4E26
        db  08Bh, 0FFh
loc_1_4E20:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_4E26:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_4E35
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5048
loc_1_4E35:
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
        call   sub_1_50EC
        mov    edi, ebx
        jmp    loc_1_4E59
loc_1_4E56:
        mov    edi, dword ptr [edi + 0x10]
loc_1_4E59:
        or     edi, edi
        jne    loc_1_4D75
loc_1_4E61:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4E70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4E80
        pop    ebp
        ret    4
sub_1_4E80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_4EFE
loc_1_4E90:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_4EF7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5048
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_4EF7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_4EBC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_4EBC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_4ED0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_4ED6
        db  005h, 000h, 000h, 000h, 000h
loc_1_4ED0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_4ED6:
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
        call   sub_1_50EC
        mov    edi, ebx
        jmp    loc_1_4EFA
loc_1_4EF7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_4EFA:
        or     edi, edi
        jne    loc_1_4E90
loc_1_4EFE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4F10:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_4F25
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F25:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_4F61
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_4F52
        cmp    bl, 0xff
        jne    loc_1_4F52
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_4F52
        test   al, 8
        je     loc_1_4F4B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F4B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F52:
        test   al, 4
        jne    loc_1_4F5A
        test   al, 2
        je     loc_1_4F79
loc_1_4F5A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F61:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_4F79
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_4F79
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F79:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_4F80:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsb dx, byte ptr [esi]
        pop    esi
        ret    0xc
sub_1_4F94:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsw dx, word ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_4FBD:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insb byte ptr es:[edi], dx
        pop    edi
        ret    0xc
sub_1_4FD1:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insw word ptr es:[edi], dx
        pop    edi
        ret    0xc
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_4FFA:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_5000:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_5006:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_500C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_5012:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_5018:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_5024:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_502A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_5042:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_5048:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_504E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_506C:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
sub_1_507E:
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8057h           ; cd2057802800
        VxDCall  VPOWERD.Service_8058h           ; cd2058802800
        VxDCall  VPOWERD.Service_8059h           ; cd2059802800
        VxDCall  VPOWERD.Service_805Ah           ; cd205a802800
        VxDCall  VPOWERD.Service_805Bh           ; cd205b802800
        VxDCall  VPOWERD.Service_805Ch           ; cd205c802800
        VxDCall  VPOWERD.Service_805Dh           ; cd205d802800
        VxDCall  VPOWERD.Service_805Eh           ; cd205e802800
sub_1_50B4:
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_50C2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_50C8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_50CE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_50D4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_50E6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_50EC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_5110:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_5134:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_5158:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_5164:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_516A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_5170:
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
sub_1_51B2:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_51B8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_51CC:
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
sub_1_51EC:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_520C
        or     ecx, ecx
        je     loc_1_5222
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_5220
loc_1_520C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_5220
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_5220:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_5222:
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
        db  'CE2NDIS3'                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  CE2NDIS3_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_5280:
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
loc_1_52FC:
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
loc_1_5358:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_535C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_5360:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        inc    ebx
        inc    ebp
        xor    cl, byte ptr [esi + 0x44]
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x45], al
        xor    cl, byte ptr [esi + 0x44]
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x64], al
        popal  
        jo     0x53f5
        jb     loc_1_53D8
        jns    loc_1_53F6
        add    byte ptr gs:[ecx + 0x64], al
        popal  
        jo     0x5401
        jb     loc_1_53E4
        jns    loc_1_5402
        add    byte ptr gs:[ecx + 0x6f], cl
        inc    edx
        popal  
        jae    loc_1_53FF
        inc    ecx
        jb     loc_1_5404
        jae    0x5414
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6f], cl
        inc    edx
        popal  
        jae    loc_1_540F
        inc    ecx
        jb     0x5414
        jae    loc_1_5424
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], cl
        je     loc_1_541D
        jb     0x542c
        jne    0x542c
        je     0x540c
        jne    0x542d
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ecx + 0x6e], cl
        je     0x542d
        jb     0x543c
        jne    0x543c
        je     0x541c
        jne    loc_1_543D
        bound  esp, qword ptr [ebp + 0x72]
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        dec    esp
loc_1_53D8:
        jbe    0x5440
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_53E0:
        dec    ecx
        push   edx
        push   ecx
        dec    esp
loc_1_53E4:
        jbe    0x544c
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x546b
        inc    edx
        popal  
        jae    loc_1_545B
loc_1_53F6:
        inc    ecx
        jb     loc_1_5460
        jae    loc_1_5470
        add    byte ptr [eax], al
loc_1_53FF:
        add    byte ptr [ebp + 0x65], cl
loc_1_5402:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
loc_1_5404:
        jb     0x547f
        inc    edx
        popal  
        jae    0x546f
        inc    ecx
        jb     loc_1_5474
loc_1_540F:
        jae    loc_1_5484
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        inc    ebx
        inc    ecx
        push   edx
        inc    esp
        inc    ecx
        je     0x5491
loc_1_541D:
        jb     loc_1_5488
        bound  esi, qword ptr [ebp + 0x74]
        dec    ebp
loc_1_5424:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x54a2
        inc    ecx
        jb     loc_1_5493
        jae    loc_1_54A3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        inc    ebx
        inc    ebx
        inc    ecx
        push   edx
        inc    esp
        inc    ecx
        je     0x54b1
loc_1_543D:
        jb     0x54a8
        bound  esi, qword ptr [ebp + 0x74]
        dec    ebp
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_1_54C2
        inc    ecx
        jb     0x54b3
        jae    0x54c3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        popal  
        js     0x54c1
        insd   dword ptr es:[edi], dx
        jne    0x54c8
loc_1_545B:
        dec    ebp
        jne    0x54ca
        je     0x54c9
loc_1_5460:
        arpl   word ptr [ecx + 0x73], sp
        je     0x54b1
        imul   esi, dword ptr [ebx + 0x74], 0
loc_1_546C:
        dec    ebp
        popal  
        js     0x54d9
loc_1_5470:
        insd   dword ptr es:[edi], dx
        jne    loc_1_54E0
        dec    ebp
loc_1_5474:
        jne    loc_1_54E2
        je     0x54e1
        arpl   word ptr [ecx + 0x73], sp
        je     0x54c9
        imul   esi, dword ptr [ebx + 0x74], 0
loc_1_5484:
        inc    esi
        jne    0x54f3
        insb   byte ptr es:[edi], dx
loc_1_5488:
        inc    esp
        jne    loc_1_54FB
        insb   byte ptr es:[edi], dx
        js     loc_1_548F
loc_1_548F:
        add    byte ptr [esi + 0x75], al
        insb   byte ptr es:[edi], dx
loc_1_5493:
        insb   byte ptr es:[edi], dx
        inc    esp
        jne    loc_1_5507
        insb   byte ptr es:[edi], dx
        js     loc_1_549B
loc_1_549B:
        add    byte ptr [ecx + 0x4f], cl
        sub    eax, 0x796c6e4f
loc_1_54A3:
        add    byte ptr [ecx + 0x4f], cl
        sub    eax, 0x796c6e4f
        add    byte ptr [ebp + 0x61], al
        jb     loc_1_551C
        jns    loc_1_5506
        jb     0x5515
        outsb  dx, byte ptr [esi]
        jae    0x5524
        imul   esi, dword ptr [eax + eax], 0x72614500
        insb   byte ptr es:[edi], dx
        jns    0x5516
loc_1_54C2:
        jb     0x5525
        outsb  dx, byte ptr [esi]
        jae    0x5534
        imul   esi, dword ptr [eax + eax], 0x73655200
        outsd  dx, dword ptr [esi]
        jne    0x5544
        arpl   word ptr [ebp + 0x4d], sp
        popal  
        outsb  dx, byte ptr [esi]
        popal  
        jb     loc_1_54DC
loc_1_54DC:
        push   edx
        jae    loc_1_554F
loc_1_54E0:
        jne    0x5554
loc_1_54E2:
        arpl   word ptr [ebp + 0x4d], sp
        popal  
        outsb  dx, byte ptr [esi]
        popal  
        jb     loc_1_54EC
loc_1_54EC:
        pop    eax
        imul   esi, dword ptr [edx + 0x63], 0x50206d6f
        jb     loc_1_555D
        outsd  dx, dword ptr [esi]
        jb     loc_1_5567
        popal  
loc_1_54FB:
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x20], sp
        push   ebx
        jb     0x556c
        jae    0x5526
loc_1_5506:
        inc    ebp
loc_1_5507:
        je     loc_1_5571
        jb     0x557a
        je     loc_1_552F
        inc    ecx
        popal  
        jo     0x5588
        jb     loc_1_5537
        inc    esp
        jb     loc_1_5583
        jbe    loc_1_5581
loc_1_551C:
        jb     loc_1_551E
loc_1_551E:
        add    byte ptr [eax], al
loc_1_5520:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_552F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5537:
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
loc_1_554F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_555D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5567:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5571:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5581:
        add    byte ptr [eax], al
loc_1_5583:
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
        db  000h
sub_1_57A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57CC:
        add    byte ptr [eax], al
loc_1_57CE:
        add    byte ptr [eax], al
loc_1_57D0:
        add    byte ptr [eax], al
loc_1_57D2:
        add    byte ptr [eax], al
loc_1_57D4:
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
sub_1_5860:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x5524]       ; -> obj1:0x5524
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_50E6
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
        call   sub_1_50E6
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
        call   sub_1_50E6
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
        call   sub_1_50EC
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_50EC
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_50EC
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
        mov    eax, dword ptr [0x5524]       ; -> obj1:0x5524
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_50E6
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
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_05E0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_0269
        cmp    ebx, 0x103
        jne    loc_2_0280
loc_2_0269:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0300
        or     eax, eax
        je     loc_2_0280
        mov    ebx, eax
loc_2_0280:
        or     ebx, ebx
        je     loc_2_028C
        cmp    ebx, 0x103
        jne    loc_2_02EB
loc_2_028C:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_02E6
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_02A2
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_02A2:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_02B1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_02B7
loc_2_02B1:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_02B7:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_02C6
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5048
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_50EC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_02E6:
        mov    ebx, 0xc001000e
loc_2_02EB:
        mov    eax, ebx
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
        call   sub_1_51EC
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_51EC
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
        call   sub_1_51EC
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
        call   sub_1_51EC
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
        call   sub_1_51EC
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
        call   sub_1_51EC
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
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0xc], ecx
        je     loc_2_0565
        mov    edi, dword ptr [ebp - 0x20]
        sub    edx, edx
        nop    
loc_2_04F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_056B
        mov    eax, dword ptr [esi + 4]
        add    eax, edx
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
        call   sub_1_51EC
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_51EC
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
        add    edx, 6
        inc    ecx
        cmp    dword ptr [ebp - 0xc], ecx
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
        call   sub_1_51EC
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_51EC
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
        je     loc_2_061D
        lea    esp, [esp]
loc_2_0610:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0610
loc_2_061D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, ebx
        je     loc_2_0651
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
        je     loc_2_0653
        cmp    eax, 0x103
        je     loc_2_0653
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
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
        call   sub_1_51EC
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
        call   sub_1_51EC
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

VxD__LDATA SEGMENT   ; object 4  vsize=0x98c  RW-
        db  04Ch, 001h, 003h, 000h, 056h, 0CDh, 091h, 02Fh, 038h, 043h, 000h, 000h, 05Ah, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 02Eh, 064h, 061h, 074h, 061h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 09Eh, 002h, 000h, 000h, 08Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 030h, 0C0h, 05Fh, 04Ch, 054h, 045h
        db  058h, 054h, 000h, 000h, 09Eh, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 022h, 000h, 000h
        db  02Ah, 003h, 000h, 000h, 06Ah, 025h, 000h, 000h, 01Eh, 030h, 000h, 000h, 012h, 001h, 02Fh, 003h
        db  020h, 000h, 050h, 060h, 02Eh, 062h, 073h, 073h, 000h, 000h, 000h, 000h, 0DEh, 024h, 000h, 000h
        db  000h, 000h, 000h, 000h, 014h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 030h, 0C0h, 001h, 001h, 001h, 000h
        db  002h, 001h, 001h, 000h, 003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h, 005h, 001h, 001h, 000h
        db  006h, 001h, 001h, 000h, 011h, 001h, 001h, 000h, 013h, 001h, 001h, 000h, 012h, 001h, 001h, 000h
        db  007h, 001h, 001h, 000h, 008h, 001h, 001h, 000h, 009h, 001h, 001h, 000h, 00Ah, 001h, 001h, 000h
        db  00Bh, 001h, 001h, 000h, 00Ch, 001h, 001h, 000h, 00Dh, 001h, 001h, 000h, 010h, 001h, 001h, 000h
        db  00Eh, 001h, 001h, 000h, 00Fh, 001h, 001h, 000h, 001h, 001h, 002h, 000h, 002h, 001h, 002h, 000h
        db  003h, 001h, 002h, 000h, 004h, 001h, 002h, 000h, 005h, 001h, 002h, 000h, 001h, 001h, 001h, 001h
        db  002h, 001h, 001h, 001h, 003h, 001h, 001h, 001h, 004h, 001h, 001h, 001h, 001h, 001h, 002h, 001h
        db  002h, 001h, 002h, 001h, 003h, 001h, 002h, 001h, 001h, 001h, 001h, 000h, 002h, 001h, 001h, 000h
        db  003h, 001h, 001h, 000h, 004h, 001h, 001h, 000h, 005h, 001h, 001h, 000h, 006h, 001h, 001h, 000h
        db  011h, 001h, 001h, 000h, 013h, 001h, 001h, 000h, 012h, 001h, 001h, 000h, 007h, 001h, 001h, 000h
        db  008h, 001h, 001h, 000h, 009h, 001h, 001h, 000h, 00Ah, 001h, 001h, 000h, 00Bh, 001h, 001h, 000h
        db  00Ch, 001h, 001h, 000h, 00Dh, 001h, 001h, 000h, 010h, 001h, 001h, 000h, 00Eh, 001h, 001h, 000h
        db  00Fh, 001h, 001h, 000h, 001h, 001h, 001h, 001h, 002h, 001h, 001h, 001h, 003h, 001h, 001h, 001h
        db  004h, 001h, 001h, 001h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  043h, 045h, 032h, 04Eh, 044h, 049h, 053h, 033h, 000h, 000h, 000h, 000h, 043h, 045h, 032h, 04Eh
        db  044h, 049h, 053h, 033h, 000h, 000h, 000h, 000h, 041h, 064h, 061h, 070h, 074h, 065h, 072h, 054h
        db  079h, 070h, 065h, 000h, 041h, 064h, 061h, 070h, 074h, 065h, 072h, 054h, 079h, 070h, 065h, 000h
        db  049h, 06Fh, 042h, 061h, 073h, 065h, 041h, 064h, 064h, 072h, 065h, 073h, 073h, 000h, 000h, 000h
        db  049h, 06Fh, 042h, 061h, 073h, 065h, 041h, 064h, 064h, 072h, 065h, 073h, 073h, 000h, 000h, 000h
        db  049h, 06Eh, 074h, 065h, 072h, 072h, 075h, 070h, 074h, 04Eh, 075h, 06Dh, 062h, 065h, 072h, 000h
        db  049h, 06Eh, 074h, 065h, 072h, 072h, 075h, 070h, 074h, 04Eh, 075h, 06Dh, 062h, 065h, 072h, 000h
        db  049h, 052h, 051h, 04Ch, 065h, 076h, 065h, 06Ch, 000h, 000h, 000h, 000h, 049h, 052h, 051h, 04Ch
        db  065h, 076h, 065h, 06Ch, 000h, 000h, 000h, 000h, 04Dh, 065h, 06Dh, 06Fh, 072h, 079h, 042h, 061h
        db  073h, 065h, 041h, 064h, 064h, 072h, 065h, 073h, 073h, 000h, 000h, 000h, 04Dh, 065h, 06Dh, 06Fh
        db  072h, 079h, 042h, 061h, 073h, 065h, 041h, 064h, 064h, 072h, 065h, 073h, 073h, 000h, 000h, 000h
        db  050h, 043h, 043h, 041h, 052h, 044h, 041h, 074h, 074h, 072h, 069h, 062h, 075h, 074h, 065h, 04Dh
        db  065h, 06Dh, 06Fh, 072h, 079h, 041h, 064h, 064h, 072h, 065h, 073h, 073h, 000h, 000h, 000h, 000h
        db  050h, 043h, 043h, 041h, 052h, 044h, 041h, 074h, 074h, 072h, 069h, 062h, 075h, 074h, 065h, 04Dh
        db  065h, 06Dh, 06Fh, 072h, 079h, 041h, 064h, 064h, 072h, 065h, 073h, 073h, 000h, 000h, 000h, 000h
        db  04Dh, 061h, 078h, 069h, 06Dh, 075h, 06Dh, 04Dh, 075h, 06Ch, 074h, 069h, 063h, 061h, 073h, 074h
        db  04Ch, 069h, 073h, 074h, 000h, 000h, 000h, 000h, 04Dh, 061h, 078h, 069h, 06Dh, 075h, 06Dh, 04Dh
        db  075h, 06Ch, 074h, 069h, 063h, 061h, 073h, 074h, 04Ch, 069h, 073h, 074h, 000h, 000h, 000h, 000h
        db  046h, 075h, 06Ch, 06Ch, 044h, 075h, 070h, 06Ch, 065h, 078h, 000h, 000h, 046h, 075h, 06Ch, 06Ch
        db  044h, 075h, 070h, 06Ch, 065h, 078h, 000h, 000h, 049h, 04Fh, 02Dh, 04Fh, 06Eh, 06Ch, 079h, 000h
        db  049h, 04Fh, 02Dh, 04Fh, 06Eh, 06Ch, 079h, 000h, 045h, 061h, 072h, 06Ch, 079h, 054h, 072h, 061h
        db  06Eh, 073h, 06Dh, 069h, 074h, 000h, 000h, 000h, 045h, 061h, 072h, 06Ch, 079h, 054h, 072h, 061h
        db  06Eh, 073h, 06Dh, 069h, 074h, 000h, 000h, 000h, 052h, 065h, 073h, 06Fh, 075h, 072h, 063h, 065h
        db  04Dh, 061h, 06Eh, 061h, 067h, 065h, 072h, 000h, 052h, 065h, 073h, 06Fh, 075h, 072h, 063h, 065h
        db  04Dh, 061h, 06Eh, 061h, 067h, 065h, 072h, 000h, 058h, 069h, 072h, 063h, 06Fh, 06Dh, 020h, 050h
        db  065h, 072h, 066h, 06Fh, 072h, 06Dh, 061h, 06Eh, 063h, 065h, 020h, 053h, 065h, 072h, 069h, 065h
        db  073h, 020h, 045h, 074h, 068h, 065h, 072h, 06Eh, 065h, 074h, 020h, 041h, 064h, 061h, 070h, 074h
        db  065h, 072h, 020h, 044h, 072h, 069h, 076h, 065h, 072h, 000h, 055h, 08Bh, 054h, 024h, 008h, 0ECh
        db  08Bh, 0ECh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 055h, 08Bh, 054h, 024h, 008h, 066h
        db  0EDh, 08Bh, 0ECh, 05Dh, 0C2h, 004h, 000h, 08Dh, 049h, 000h, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh
        db  064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 0B8h, 009h, 000h, 000h, 000h
        db  08Bh, 0ECh, 083h, 0ECh, 04Ch, 066h, 089h, 045h, 0ECh, 08Bh, 04Dh, 00Ch, 066h, 089h, 045h, 0EEh
        db  06Ah, 000h, 051h, 08Bh, 055h, 008h, 052h, 08Dh, 045h, 0F8h, 0C7h, 045h, 0FCh, 000h, 000h, 000h
        db  000h, 0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h, 050h, 0E8h, 000h, 000h, 000h, 000h, 06Ah, 0FFh
        db  08Dh, 04Dh, 0F4h, 06Ah, 000h, 06Ah, 008h, 051h, 0E8h, 000h, 000h, 000h, 000h, 089h, 045h, 0FCh
        db  085h, 0C0h, 074h, 00Bh, 0B8h, 001h, 000h, 000h, 0C0h, 08Bh, 0E5h, 05Dh, 0C2h, 008h, 000h, 08Bh
        db  045h, 0F8h, 08Bh, 04Dh, 0F4h, 089h, 001h, 0C6h, 045h, 0B4h, 003h, 0C6h, 045h, 0B5h, 000h, 0C7h
        db  045h, 0B6h, 000h, 000h, 000h, 000h, 0C7h, 045h, 0BAh, 000h, 000h, 000h, 000h, 0C7h, 045h, 0BEh
        db  000h, 000h, 000h, 000h, 0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h, 0C7h, 045h, 0C6h, 000h, 000h
        db  000h, 000h, 0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h, 0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h, 08Bh, 055h, 0ECh, 08Bh, 045h, 0F0h, 06Ah, 036h, 089h
        db  055h, 0E2h, 08Dh, 04Dh, 0B4h, 089h, 045h, 0E6h, 051h, 08Bh, 055h, 0F4h, 052h, 08Bh, 045h, 0F8h
        db  050h, 08Dh, 04Dh, 0FCh, 0C7h, 045h, 0D6h, 000h, 000h, 000h, 000h, 0C7h, 045h, 0DAh, 000h, 000h
        db  000h, 000h, 0C7h, 045h, 0DEh, 000h, 000h, 000h, 000h, 08Dh, 042h, 004h, 050h, 051h, 0E8h, 000h
        db  000h, 000h, 000h, 083h, 07Dh, 0FCh, 000h, 074h, 00Bh, 06Ah, 000h, 08Bh, 045h, 0F8h, 050h, 0E8h
        db  000h, 000h, 000h, 000h, 08Bh, 045h, 0FCh, 08Bh, 0E5h, 05Dh, 0C2h, 008h, 000h, 08Dh, 09Bh, 000h
        db  000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 0B8h, 00Ch, 000h, 000h, 000h
        db  08Bh, 0ECh, 081h, 0ECh, 080h, 000h, 000h, 000h, 066h, 089h, 045h, 0A0h, 0B9h, 010h, 000h, 000h
        db  000h, 066h, 089h, 045h, 0A2h, 056h, 066h, 089h, 04Dh, 0A8h, 057h, 066h, 089h, 04Dh, 0AAh, 0B8h
        db  009h, 000h, 000h, 000h, 0C7h, 045h, 0A4h, 000h, 000h, 000h, 000h, 0C7h, 045h, 0ACh, 000h, 000h
        db  000h, 000h, 0C7h, 045h, 0B4h, 000h, 000h, 000h, 000h, 0C7h, 045h, 0BCh, 000h, 000h, 000h, 000h
        db  066h, 089h, 045h, 0B0h, 066h, 089h, 045h, 0B2h, 0B8h, 015h, 000h, 000h, 000h, 0C7h, 045h, 0C4h
        db  000h, 000h, 000h, 000h, 066h, 089h, 045h, 0B8h, 066h, 089h, 045h, 0BAh, 0B8h, 00Bh, 000h, 000h
        db  000h, 0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h, 066h, 089h, 045h, 0C0h, 066h, 089h, 045h, 0C2h
        db  0B8h, 008h, 000h, 000h, 000h, 0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h, 066h, 089h, 045h, 0C8h
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h, 066h, 089h, 045h, 0CAh, 06Ah, 0FFh, 066h, 089h, 04Dh
        db  0D8h, 0B8h, 00Eh, 000h, 000h, 000h, 066h, 089h, 045h, 0D0h, 06Ah, 000h, 066h, 089h, 045h, 0D2h
        db  068h, 007h, 007h, 000h, 000h, 066h, 089h, 04Dh, 0DAh, 08Dh, 04Dh, 0F8h, 051h, 0E8h, 000h, 000h
        db  000h, 000h, 089h, 045h, 0FCh, 085h, 0C0h, 00Fh, 085h, 0F1h, 002h, 000h, 000h, 08Bh, 07Dh, 0F8h
        db  033h, 0C0h, 0B9h, 0C1h, 001h, 000h, 000h, 0F3h, 0ABh, 066h, 0ABh, 0AAh, 08Bh, 045h, 008h, 08Bh
        db  04Dh, 0F8h, 08Bh, 075h, 00Ch, 08Bh, 050h, 004h, 056h, 08Dh, 045h, 0F0h, 050h, 033h, 0FFh, 089h
        db  051h, 048h, 08Dh, 04Dh, 0FCh, 051h, 0E8h, 000h, 000h, 000h, 000h, 039h, 07Dh, 0FCh, 00Fh, 085h
        db  0A5h, 002h, 000h, 000h, 057h, 08Dh, 045h, 0A0h, 050h, 08Bh, 04Dh, 0F0h, 051h, 08Dh, 055h, 0F4h
        db  052h, 08Dh, 045h, 0FCh, 050h, 0E8h, 000h, 000h, 000h, 000h, 039h, 07Dh, 0FCh, 00Fh, 085h, 07Dh
        db  002h, 000h, 000h, 066h, 089h, 07Dh, 080h, 08Dh, 045h, 082h, 08Dh, 04Dh, 0E4h, 08Dh, 055h, 0E0h
        db  051h, 089h, 038h, 052h, 089h, 078h, 004h, 06Ah, 010h, 089h, 078h, 008h, 066h, 089h, 078h, 00Ch
        db  08Dh, 04Dh, 0FCh, 08Dh, 045h, 080h, 050h, 06Ah, 002h, 056h, 051h, 0E8h, 000h, 000h, 000h, 000h
        db  083h, 0C4h, 01Ch, 039h, 07Dh, 0FCh, 00Fh, 085h, 044h, 002h, 000h, 000h, 08Bh, 04Dh, 084h, 033h
        db  0C0h, 066h, 08Bh, 045h, 086h, 081h, 0E1h, 0FFh, 0FFh, 000h, 000h, 02Bh, 0C1h, 083h, 0F8h, 00Fh
        db  075h, 006h, 08Bh, 045h, 0F8h, 089h, 048h, 06Ch, 08Dh, 07Dh, 082h, 033h, 0C0h, 066h, 0C7h, 045h
        db  080h, 000h, 000h, 0B9h, 007h, 000h, 000h, 000h, 0F3h, 0ABh, 066h, 0ABh, 08Dh, 045h, 0E4h, 08Dh
        db  04Dh, 0E0h, 050h, 08Dh, 055h, 080h, 051h, 08Dh, 045h, 0FCh, 06Ah, 020h, 052h, 06Ah, 001h, 056h
        db  050h, 0E8h, 000h, 000h, 000h, 000h, 083h, 0C4h, 01Ch, 083h, 07Dh, 0FCh, 000h, 00Fh, 085h, 0EDh
        db  001h, 000h, 000h, 08Bh, 045h, 084h, 08Bh, 04Dh, 0F8h, 06Ah, 001h, 089h, 041h, 074h, 08Bh, 055h
        db  094h, 08Bh, 04Dh, 0F8h, 08Dh, 045h, 0A8h, 089h, 051h, 07Ch, 050h, 08Bh, 055h, 0F0h, 052h, 08Dh
        db  04Dh, 0F4h, 051h, 08Dh, 045h, 0FCh, 050h, 0E8h, 000h, 000h, 000h, 000h, 083h, 07Dh, 0FCh, 000h
        db  074h, 021h, 06Ah, 001h, 08Dh, 045h, 0B0h, 050h, 08Bh, 04Dh, 0F0h, 051h, 08Dh, 055h, 0F4h, 052h
        db  08Dh, 045h, 0FCh, 050h, 0E8h, 000h, 000h, 000h, 000h, 083h, 07Dh, 0FCh, 000h, 00Fh, 085h, 09Dh
        db  001h, 000h, 000h, 08Bh, 045h, 0F4h, 08Bh, 04Dh, 0F8h, 06Ah, 000h, 08Bh, 050h, 004h, 08Dh, 045h
        db  0B8h, 089h, 091h, 084h, 000h, 000h, 000h, 050h, 08Bh, 055h, 0F0h, 052h, 08Dh, 04Dh, 0F4h, 051h
        db  08Dh, 045h, 0FCh, 050h, 0E8h, 000h, 000h, 000h, 000h, 083h, 07Dh, 0FCh, 000h, 074h, 00Ch, 08Bh
        db  045h, 0F8h, 0C7h, 040h, 010h, 009h, 000h, 000h, 000h, 0EBh, 00Ch, 08Bh, 045h, 0F4h, 08Bh, 04Dh
        db  0F8h, 08Bh, 050h, 004h, 089h, 051h, 010h, 08Bh, 045h, 0F0h, 08Dh, 04Dh, 0E8h, 050h, 08Dh, 055h
        db  0ECh, 051h, 08Dh, 045h, 0FCh, 052h, 050h, 0E8h, 000h, 000h, 000h, 000h, 083h, 07Dh, 0FCh, 000h
        db  075h, 021h, 083h, 07Dh, 0E8h, 006h, 075h, 01Bh, 08Bh, 055h, 0F8h, 08Bh, 04Dh, 0ECh, 083h, 0C2h
        db  00Ah, 08Bh, 0C1h, 08Bh, 039h, 089h, 03Ah, 08Ah, 041h, 004h, 088h, 042h, 004h, 08Ah, 049h, 005h
        db  088h, 04Ah, 005h, 08Bh, 045h, 0F8h, 06Ah, 001h, 08Dh, 04Dh, 0C0h, 0C6h, 080h, 092h, 000h, 000h
        db  000h, 000h, 051h, 08Bh, 055h, 0F0h, 052h, 08Dh, 045h, 0F4h, 050h, 08Dh, 04Dh, 0FCh, 051h, 0E8h
        db  000h, 000h, 000h, 000h, 083h, 07Dh, 0FCh, 000h, 075h, 017h, 08Bh, 045h, 0F4h, 08Bh, 040h, 004h
        db  048h, 083h, 0F8h, 001h, 08Bh, 045h, 0F8h, 01Ah, 0C9h, 0F6h, 0D9h, 088h, 088h, 092h, 000h, 000h
        db  000h, 08Bh, 045h, 0F8h, 06Ah, 001h, 08Dh, 04Dh, 0C8h, 0C6h, 080h, 094h, 000h, 000h, 000h, 000h
        db  051h, 08Bh, 055h, 0F0h, 052h, 08Dh, 045h, 0F4h, 050h, 08Dh, 04Dh, 0FCh, 051h, 0E8h, 000h, 000h
        db  000h, 000h, 083h, 07Dh, 0FCh, 000h, 075h, 017h, 08Bh, 045h, 0F4h, 08Bh, 040h, 004h, 048h, 083h
        db  0F8h, 001h, 08Bh, 045h, 0F8h, 01Ah, 0C9h, 0F6h, 0D9h, 088h, 088h, 094h, 000h, 000h, 000h, 08Bh
        db  045h, 0F8h, 06Ah, 001h, 08Dh, 04Dh, 0D0h, 0C6h, 080h, 095h, 000h, 000h, 000h, 001h, 051h, 08Bh
        db  055h, 0F0h, 052h, 08Dh, 045h, 0F4h, 050h, 08Dh, 04Dh, 0FCh, 051h, 0E8h, 000h, 000h, 000h, 000h
        db  083h, 07Dh, 0FCh, 000h, 075h, 017h, 08Bh, 045h, 0F4h, 08Bh, 040h, 004h, 048h, 083h, 0F8h, 001h
        db  08Bh, 045h, 0F8h, 01Ah, 0C9h, 0F6h, 0D9h, 088h, 088h, 095h, 000h, 000h, 000h, 08Bh, 045h, 0F8h
        db  06Ah, 001h, 08Dh, 04Dh, 0D8h, 0C6h, 080h, 093h, 000h, 000h, 000h, 000h, 051h, 08Bh, 055h, 0F0h
        db  052h, 08Dh, 045h, 0F4h, 050h, 08Dh, 04Dh, 0FCh, 051h, 0E8h, 000h, 000h, 000h, 000h, 083h, 07Dh
        db  0FCh, 000h, 075h, 017h, 08Bh, 045h, 0F4h, 08Bh, 040h, 004h, 048h, 083h, 0F8h, 001h, 08Bh, 045h
        db  0F8h, 01Ah, 0C9h, 0F6h, 0D9h, 088h, 088h, 093h, 000h, 000h, 000h, 08Bh, 045h, 0F0h, 050h, 0E8h
        db  000h, 000h, 000h, 000h, 056h, 08Bh, 045h, 010h, 050h, 08Bh, 04Dh, 0F8h, 051h, 0E8h, 000h, 000h
        db  000h, 000h, 085h, 0C0h, 075h, 013h, 033h, 0C0h, 05Fh, 05Eh, 08Bh, 0E5h, 05Dh, 0C2h, 00Ch, 000h
        db  08Bh, 045h, 0F0h, 050h, 0E8h, 000h, 000h, 000h, 000h, 06Ah, 000h, 08Bh, 045h, 0F8h, 068h, 007h
        db  007h, 000h, 000h, 050h, 0E8h, 000h, 000h, 000h, 000h, 0B8h, 001h, 000h, 000h, 0C0h, 05Fh, 05Eh
        db  08Bh, 0E5h, 05Dh, 0C2h, 00Ch, 000h, 08Dh, 064h, 024h, 000h, 055h, 08Bh, 0ECh, 081h, 0ECh, 000h
        db  006h, 000h, 000h, 053h, 056h, 057h, 033h, 0DBh, 06Ah, 0FFh, 08Dh, 045h, 0F8h, 053h, 06Ah, 026h
        db  050h, 0E8h, 000h, 000h, 000h, 000h, 089h, 045h, 0FCh, 03Bh, 0C3h, 00Fh, 085h, 01Ch, 001h, 000h
        db  000h, 08Bh, 07Dh, 0F8h, 033h, 0C0h, 0B9h, 009h, 000h, 000h, 000h, 0F3h, 0ABh, 066h, 0ABh, 08Bh
        db  045h, 0F8h, 0B9h, 001h, 000h, 000h, 000h, 08Bh, 075h, 008h, 089h, 048h, 016h, 08Bh, 055h, 0F8h
        db  08Bh, 046h, 06Ch, 089h, 042h, 01Ah, 08Bh, 055h, 0F8h, 0C7h, 042h, 01Eh, 010h, 000h, 000h, 000h
        db  08Bh, 055h, 0F8h, 08Dh, 046h, 070h, 089h, 042h, 022h, 08Bh, 055h, 0F8h, 08Bh, 045h, 00Ch, 089h
        db  04Ah, 00Ah, 08Bh, 055h, 0F8h, 08Bh, 04Dh, 010h, 052h, 050h, 051h, 08Bh, 056h, 048h, 056h, 08Dh
        db  046h, 04Ch, 052h, 050h, 0E8h, 000h, 000h, 000h, 000h, 053h, 08Bh, 04Dh, 0F8h, 06Ah, 026h, 089h
        db  045h, 0FCh, 051h, 0E8h, 000h, 000h, 000h, 000h, 039h, 05Dh, 0FCh, 00Fh, 085h, 0ACh, 000h, 000h
        db  000h, 056h, 0E8h, 000h, 000h, 000h, 000h, 089h, 006h, 03Bh, 0C3h, 00Fh, 084h, 09Ch, 000h, 000h
        db  000h, 0B9h, 000h, 000h, 000h, 000h, 089h, 05Eh, 02Ch, 089h, 04Eh, 030h, 089h, 076h, 034h, 089h
        db  05Eh, 018h, 089h, 05Eh, 014h, 06Ah, 002h, 08Bh, 086h, 084h, 000h, 000h, 000h, 053h, 089h, 05Eh
        db  038h, 050h, 089h, 04Eh, 03Ch, 050h, 089h, 076h, 040h, 051h, 08Dh, 056h, 050h, 056h, 08Bh, 04Eh
        db  04Ch, 068h, 000h, 000h, 000h, 000h, 08Dh, 045h, 0FCh, 051h, 052h, 050h, 0E8h, 000h, 000h, 000h
        db  000h, 039h, 05Dh, 0FCh, 074h, 01Eh, 053h, 08Bh, 046h, 04Ch, 068h, 005h, 000h, 000h, 0AAh, 050h
        db  0E8h, 000h, 000h, 000h, 04Ch, 001h, 003h, 000h, 01Dh, 0CDh, 091h, 02Fh, 04Ah, 057h, 000h, 000h
        db  026h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 02Eh, 064h, 061h, 074h, 061h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 08Ch, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 030h, 0C0h
        db  05Fh, 04Ch, 054h, 045h, 058h, 054h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A0h, 029h, 000h, 000h, 090h, 000h, 000h, 000h, 030h, 02Ah, 000h, 000h, 016h, 03Ah, 000h, 000h
        db  097h, 001h, 0DEh, 004h, 020h, 000h, 050h, 060h, 02Eh, 062h, 073h, 073h, 000h, 000h, 000h, 000h
        db  0A4h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 078h, 034h, 045h, 06Eh
VxD__LDATA ENDS

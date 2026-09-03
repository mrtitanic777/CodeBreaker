; SMC80PC -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 5414 instructions recovered by recursive descent

Declare_Virtual_Device SMC80PC, 3, 10, SMC80PC_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x21e4  R-X
loc_1_0000:
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
        call   sub_1_1B48
        cmp    dword ptr [ebp - 8], 0xc
        jb     loc_1_01B7
        cmp    dword ptr [ebp - 8], 0x5ea
        ja     loc_1_01B7
        cmp    byte ptr [esi + 0x172], 0
        je     loc_1_0056
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0056:
        cmp    byte ptr [esi + 0x174], 0
        je     loc_1_006D
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_006D:
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_0081
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0081:
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   ebx
        inc    dword ptr [ebx + 0x1c]
        push   esi
        inc    dword ptr [esi + 0x10d]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 0
        call   sub_1_0210
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 9], 0
        je     loc_1_00D5
        cmp    dword ptr [esi + 0x125], 0
        jne    loc_1_00C1
        mov    dword ptr [esi + 0x129], edi
        mov    dword ptr [esi + 0x125], edi
        jmp    loc_1_015F
loc_1_00C1:
        mov    eax, dword ptr [esi + 0x129]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x129], edi
        jmp    loc_1_015F
loc_1_00D5:
        cmp    dword ptr [esi + 0x115], 0
        je     loc_1_00E9
        mov    eax, dword ptr [esi + 0x119]
        mov    dword ptr [eax + 0x24], edi
        jmp    loc_1_0152
loc_1_00E9:
        cmp    dword ptr [esi + 0x11d], 0
        jne    loc_1_0102
        xor    ebx, ebx
        mov    dword ptr [esi + 0x121], edi
        mov    dword ptr [esi + 0x11d], edi
        jmp    loc_1_0111
loc_1_0102:
        mov    ebx, dword ptr [esi + 0x121]
        mov    dword ptr [ebx + 0x24], edi
        mov    dword ptr [esi + 0x121], edi
loc_1_0111:
        mov    byte ptr [esi + 0x2a1], 0
        push   esi
        push   edi
        call   sub_1_13B0
        cmp    ax, 6
        jne    loc_1_015F
        test   ebx, ebx
        jne    loc_1_013F
        mov    dword ptr [esi + 0x121], 0
        mov    dword ptr [esi + 0x11d], 0
        jmp    loc_1_014C
loc_1_013F:
        mov    dword ptr [esi + 0x121], ebx
        mov    dword ptr [ebx + 0x24], 0
loc_1_014C:
        mov    dword ptr [esi + 0x115], edi
loc_1_0152:
        mov    dword ptr [esi + 0x119], edi
        mov    byte ptr [esi + 0x2a1], 0
loc_1_015F:
        mov    edi, 0x103
        mov    eax, dword ptr [esi + 0x10d]
        dec    eax
        mov    dword ptr [esi + 0x10d], eax
        jne    loc_1_01AC
        cmp    byte ptr [esi + 0x173], 0
        je     loc_1_0187
        lea    eax, [esi + 0x28d]
        cmp    byte ptr [eax], 0
        je     loc_1_019B
loc_1_0187:
        cmp    dword ptr [esi + 0x125], 0
        je     loc_1_01AC
        lea    eax, [esi + 0x28d]
        cmp    byte ptr [eax], 0
        jne    loc_1_01AC
loc_1_019B:
        push   0
        add    esi, 0x181
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1AFA
loc_1_01AC:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_01B7:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_01D0:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        cmp    edx, esi
        je     loc_1_01F2
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
loc_1_01E5:
        mov    bl, byte ptr [eax + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    loc_1_01FA
        inc    esi
        cmp    esi, edx
        jb     loc_1_01E5
loc_1_01F2:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_01FA:
        mov    eax, 0xffffffff
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0210:
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
        call   sub_1_09F0
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0xf9]
        cmp    byte ptr [ecx + 0x3c], dl
        mov    ebx, dword ptr [eax + 0xc]
        jne    loc_1_0263
        push   6
        lea    eax, [ebp - 8]
        push   eax
        add    ecx, 0x38
        push   ecx
        call   sub_1_01D0
        test   eax, eax
        jne    loc_1_0263
        mov    eax, ebx
        and    al, 1
        mov    byte ptr [esi + 9], al
loc_1_0263:
        mov    eax, dword ptr [ebp + 0xc]
        test   byte ptr [eax + 0x71], 2
        je     loc_1_0278
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0278:
        test   bl, 0x20
        je     loc_1_0289
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_0289:
        mov    al, byte ptr [4]              ; -> obj1:0x201C
        cmp    byte ptr [ebp - 4], al
        jne    loc_1_02B5
        push   6
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj1:0x2018
        call   sub_1_01D0
        test   eax, eax
        jne    loc_1_02B5
        and    bl, 8
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_02B5:
        test   byte ptr [ebp - 8], 1
        je     loc_1_02C9
        and    bl, 6
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_02C9:
        cmp    byte ptr [esi + 9], 0
        je     loc_1_02DB
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_02DB:
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_02F0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0x2e]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [ecx + 0x2e]
        mov    byte ptr [eax + 0x10], dl
        mov    eax, dword ptr [ecx + 0x2e]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x10]
        test   ecx, 0xfffffff9
        jne    loc_1_032A
        mov    cl, byte ptr [eax + 0x17]
        test   cl, cl
        je     loc_1_032A
        cmp    cl, 0xff
        je     loc_1_032A
        pop    ebp
        mov    byte ptr [eax + 0x1f], dl
        mov    al, 1
        ret    4
loc_1_032A:
        xor    al, al
        pop    ebp
        ret    4
loc_1_0330:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x10d]
        mov    byte ptr [esi + 0x290], 1
loc_1_0347:
        cmp    byte ptr [esi + 0x28e], bl
        jne    loc_1_035C
        mov    byte ptr [esi + 0x28e], 1
        push   esi
        call   sub_1_09B0
loc_1_035C:
        cmp    byte ptr [esi + 0x28d], bl
        jne    loc_1_0384
        mov    byte ptr [esi + 0x28d], 1
        push   ebx
        push   ebx
        push   esi
        push   ebx
        call   sub_1_04B0
        cmp    dword ptr [esi + 0x125], ebx
        je     loc_1_0384
        cmp    byte ptr [esi + 0x28d], bl
        je     loc_1_0347
loc_1_0384:
        mov    eax, dword ptr [esi + 0x10d]
        dec    eax
        mov    dword ptr [esi + 0x10d], eax
        jne    loc_1_03CC
        cmp    byte ptr [esi + 0x173], 0
        je     loc_1_03A5
        cmp    byte ptr [esi + 0x28d], 0
        je     loc_1_03B7
loc_1_03A5:
        cmp    dword ptr [esi + 0x125], 0
        je     loc_1_03CC
        cmp    byte ptr [esi + 0x28d], 0
        jne    loc_1_03CC
loc_1_03B7:
        push   0
        lea    eax, [esi + 0x181]
        mov    byte ptr [esi + 0x28d], 1
        push   eax
        call   sub_1_1AFA
loc_1_03CC:
        or     byte ptr [esi + 0xae], 0x3f
        push   esi
        call   sub_1_1020
        mov    byte ptr [esi + 0x290], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_03F0:
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
        mov    dword ptr [esi + 0x12d], ebx
        mov    byte ptr [esi + 0x131], 1
        push   0
        push   ebx
        call   sub_1_1B48
        cmp    dword ptr [ebp - 4], 6
        jb     loc_1_049B
        mov    edi, dword ptr [esi + 0x17d]
        mov    eax, dword ptr [ebp - 4]
        add    edi, 0xe
        cmp    edi, eax
        jb     loc_1_0434
        mov    edi, eax
loc_1_0434:
        push   edi
        lea    eax, [esi + 0x191]
        push   0
        push   ebx
        push   eax
        call   sub_1_09F0
        cmp    dword ptr [ebp - 4], 0xe
        jae    loc_1_0472
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [esi + 0x191]
        push   0
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 0xf9]
        push   ecx
        push   esi
        push   eax
        call   sub_1_1C10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0472:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, 0xe
        sub    eax, 0xe
        lea    ecx, [esi + 0x19f]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esi + 0x191]
        push   0xe
        mov    ecx, dword ptr [esi + 0xf9]
        push   eax
        push   eax
        push   esi
        push   ecx
        call   sub_1_1C10
loc_1_049B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x10d]
        cmp    byte ptr [esi + 0x290], 0
        jne    loc_1_04D7
        push   esi
        call   sub_1_10A0
        push   esi
        call   sub_1_1020
loc_1_04D7:
        cmp    dword ptr [esi + 0x125], 0
        je     loc_1_059D
loc_1_04E4:
        mov    ebx, dword ptr [esi + 0x125]
        cmp    dword ptr [esi + 0x129], ebx
        jne    loc_1_04FC
        mov    dword ptr [esi + 0x129], 0
loc_1_04FC:
        mov    eax, dword ptr [ebx + 0x24]
        push   ebx
        push   esi
        mov    dword ptr [esi + 0x125], eax
        call   sub_1_03F0
        push   0
        inc    dword ptr [esi + 0x13a]
        mov    edi, dword ptr [ebx + 0x28]
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A62
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_0590
        mov    eax, dword ptr [ebx + 0x109]
        cmp    edi, eax
        jne    loc_1_053D
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x109], eax
        jmp    loc_1_0550
loc_1_053D:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_054A
loc_1_0542:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_0542
loc_1_054A:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0550:
        mov    eax, dword ptr [ebx + 0x17d]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_0561
        push   ebx
        call   sub_2_09A0
loc_1_0561:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
        push   0x75
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 0x109], 0
        jne    loc_1_0590
        push   ebx
        push   0                             ; -> loc_2_1620
        add    ebx, 0xb0
        push   ebx
        call   sub_1_1B12
loc_1_0590:
        cmp    dword ptr [esi + 0x125], 0
        jne    loc_1_04E4
loc_1_059D:
        push   esi
        call   sub_1_1100
        cmp    byte ptr [esi + 0x131], 0
        je     loc_1_05BF
        mov    byte ptr [esi + 0x131], 0
        mov    eax, dword ptr [esi + 0xf9]
        push   eax
        call   sub_1_1DF0
loc_1_05BF:
        cmp    byte ptr [esi + 0x173], 0
        je     loc_1_0981
        cmp    dword ptr [esi + 0x10d], 1
        jne    loc_1_0981
        push   esi
        lea    eax, [esi + 0xb0]
        mov    byte ptr [esi + 0x174], 1
        push   0                             ; -> sub_1_1060
        push   eax
        call   sub_1_1B12
        mov    edi, dword ptr [esi + 0x109]
        test   edi, edi
        je     loc_1_068F
loc_1_0600:
        push   0
        inc    dword ptr [edi + 0x1c]
        push   0
        mov    eax, dword ptr [edi]
        push   0x40010004
        push   eax
        call   sub_1_1A86
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_0684
        mov    eax, dword ptr [ebx + 0x109]
        cmp    eax, edi
        jne    loc_1_0631
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x109], eax
        jmp    loc_1_0644
loc_1_0631:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_063E
loc_1_0636:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_0636
loc_1_063E:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0644:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x17d], eax
        jne    loc_1_0655
        push   ebx
        call   sub_2_09A0
loc_1_0655:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
        push   0x75
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 0x109], 0
        jne    loc_1_0684
        push   ebx
        push   0                             ; -> loc_2_1620
        add    ebx, 0xb0
        push   ebx
        call   sub_1_1B12
loc_1_0684:
        mov    edi, dword ptr [edi + 0xc]
        test   edi, edi
        jne    loc_1_0600
loc_1_068F:
        push   esi
        call   sub_2_1770
        test   ax, ax
        je     loc_1_06B6
        mov    byte ptr [esi + 0x172], 1
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000003
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
        jmp    loc_1_06BD
loc_1_06B6:
        mov    byte ptr [esi + 0x172], 0
loc_1_06BD:
        cmp    dword ptr [esi + 0x11d], 0
        je     loc_1_06F5
        mov    eax, dword ptr [esi + 0x115]
        mov    ecx, dword ptr [esi + 0x121]
        mov    dword ptr [ecx + 0x24], eax
        mov    edx, dword ptr [esi + 0x11d]
        mov    dword ptr [esi + 0x115], edx
        mov    dword ptr [esi + 0x121], 0
        mov    dword ptr [esi + 0x11d], 0
loc_1_06F5:
        cmp    dword ptr [esi + 0x125], 0
        je     loc_1_0719
        mov    eax, dword ptr [esi + 0x115]
        mov    ecx, dword ptr [esi + 0x129]
        mov    dword ptr [ecx + 0x24], eax
        mov    edx, dword ptr [esi + 0x125]
        mov    dword ptr [esi + 0x115], edx
loc_1_0719:
        mov    dword ptr [esi + 0x129], 0
        mov    dword ptr [esi + 0x125], 0
        cmp    dword ptr [esi + 0x115], 0
        je     loc_1_07D7
loc_1_073A:
        mov    ecx, dword ptr [esi + 0x115]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x115], eax
        mov    edi, dword ptr [ecx + 0x28]
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1A62
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_07CA
        mov    eax, dword ptr [ebx + 0x109]
        cmp    eax, edi
        jne    loc_1_0777
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x109], eax
        jmp    loc_1_078A
loc_1_0777:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_0784
loc_1_077C:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_077C
loc_1_0784:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_078A:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x17d], eax
        jne    loc_1_079B
        push   ebx
        call   sub_2_09A0
loc_1_079B:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
        push   0x75
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 0x109], 0
        jne    loc_1_07CA
        push   ebx
        push   0                             ; -> loc_2_1620
        add    ebx, 0xb0
        push   ebx
        call   sub_1_1B12
loc_1_07CA:
        cmp    dword ptr [esi + 0x115], 0
        jne    loc_1_073A
loc_1_07D7:
        mov    dword ptr [esi + 0x119], 0
        cmp    byte ptr [esi + 0x172], 0
        jne    loc_1_07F0
        push   esi
        call   sub_2_1870
loc_1_07F0:
        mov    byte ptr [esi + 0x174], 0
        mov    edi, dword ptr [esi + 0x109]
        test   edi, edi
        je     loc_1_08CF
loc_1_0805:
        inc    dword ptr [edi + 0x1c]
        cmp    byte ptr [esi + 0x172], 0
        je     loc_1_0822
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   0x40010007
        push   eax
        call   sub_1_1A86
loc_1_0822:
        mov    eax, 0
        push   4
        cmp    byte ptr [esi + 0x172], 1
        adc    eax, -1
        and    eax, 0xc0000001
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [edi]
        push   0x40010005
        push   ecx
        call   sub_1_1A86
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1A8C
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_08C4
        mov    eax, dword ptr [ebx + 0x109]
        cmp    eax, edi
        jne    loc_1_0871
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x109], eax
        jmp    loc_1_0884
loc_1_0871:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_087E
loc_1_0876:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_0876
loc_1_087E:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_0884:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x17d], eax
        jne    loc_1_0895
        push   ebx
        call   sub_2_09A0
loc_1_0895:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
        push   0x75
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 0x109], 0
        jne    loc_1_08C4
        push   ebx
        push   0                             ; -> loc_2_1620
        add    ebx, 0xb0
        push   ebx
        call   sub_1_1B12
loc_1_08C4:
        mov    edi, dword ptr [edi + 0xc]
        test   edi, edi
        jne    loc_1_0805
loc_1_08CF:
        mov    eax, 0
        mov    ecx, dword ptr [esi + 0x175]
        cmp    byte ptr [esi + 0x172], 1
        adc    eax, -1
        mov    edx, dword ptr [ecx]
        and    eax, 0xc0000001
        push   eax
        push   edx
        call   sub_1_1A6E
        mov    ebx, dword ptr [esi + 0x175]
        mov    edi, dword ptr [ebx + 8]
        dec    dword ptr [ebx + 0x1c]
        jne    loc_1_0974
        mov    ecx, dword ptr [edi + 0x109]
        cmp    ebx, ecx
        jne    loc_1_0915
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [edi + 0x109], eax
        jmp    loc_1_0934
loc_1_0915:
        mov    eax, dword ptr [esi + 0x175]
        cmp    dword ptr [ecx + 0xc], eax
        je     loc_1_0928
loc_1_0920:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], eax
        jne    loc_1_0920
loc_1_0928:
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 0xc], eax
        mov    ebx, dword ptr [esi + 0x175]
loc_1_0934:
        mov    eax, dword ptr [ebx + 0x18]
        cmp    dword ptr [edi + 0x17d], eax
        jne    loc_1_0945
        push   edi
        call   sub_2_09A0
loc_1_0945:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1A5C
        push   0
        push   0x75
        push   ebx
        call   sub_1_1B24
        cmp    dword ptr [edi + 0x109], 0
        jne    loc_1_0974
        push   edi
        push   0                             ; -> loc_2_1620
        add    edi, 0xb0
        push   edi
        call   sub_1_1B12
loc_1_0974:
        mov    byte ptr [esi + 0x173], 0
        push   esi
        call   sub_2_1870
loc_1_0981:
        mov    byte ptr [esi + 0x28d], 0
        cmp    byte ptr [esi + 0x290], 0
        jne    loc_1_099D
        push   esi
        call   sub_1_10E0
        push   esi
        call   sub_1_1020
loc_1_099D:
        mov    al, byte ptr [ebp - 4]
loc_1_09A0:
        pop    edi
        dec    dword ptr [esi + 0x10d]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  090h
sub_1_09B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x174], 0
        je     loc_1_09CC
        mov    byte ptr [esi + 0x28e], 0
        pop    esi
        pop    ebp
        ret    4
loc_1_09CC:
        mov    byte ptr [esi + 0x2a1], 0
        push   esi
        call   sub_1_1260
        mov    byte ptr [esi + 0x28e], 0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09F0:
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
        call   sub_1_1B48
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 4], esi
        je     loc_1_0A44
loc_1_0A15:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     loc_1_0A44
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0A15
loc_1_0A44:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0A55
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0A55:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_0A64:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_0A73
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_0A76
loc_1_0A73:
        mov    ebx, dword ptr [ebp - 8]
loc_1_0A76:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_1F00
        cmp    edi, esi
        je     loc_1_0AB2
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0ABD
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        jmp    loc_1_0A64
loc_1_0AB2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_0ABD:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0AD0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [eax + 8]
        add    esi, 0xe
        cmp    dword ptr [edi + 0x12d], 0
        je     loc_1_0B5C
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        mov    ebx, esi
        push   0
        push   ecx
        call   sub_1_1B48
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0B4A
loc_1_0B0A:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edi + 0x12d]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   ebx
        push   edx
        push   eax
        call   sub_1_09F0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_0B4A
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0B0A
loc_1_0B4A:
        sub    ebx, esi
        xor    eax, eax
        mov    esi, dword ptr [ebp + 0x1c]
        pop    edi
        mov    dword ptr [esi], ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_0B5C:
        cmp    byte ptr [edi + 0x131], 0
        je     loc_1_0B9C
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   esi
        push   ecx
        mov    esi, dword ptr [ebp + 0x1c]
        push   esi
        call   sub_1_1440
        test   ax, ax
        je     loc_1_0B91
        mov    eax, 0xc0000001
        pop    edi
        mov    dword ptr [esi], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_0B91:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_0B9C:
        mov    eax, 0xc0010013
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x2a1], 0
        je     loc_1_0CB9
        cmp    byte ptr [esi + 0xcf], 0
        je     loc_1_0CB9
        push   esi
        call   sub_1_1060
        mov    byte ptr [esi + 0x2a1], 0
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000003
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
        cmp    dword ptr [esi + 0x11d], 0
        je     loc_1_0C31
        xor    edi, edi
loc_1_0BFE:
        mov    edx, dword ptr [esi + 0x11d]
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x11d], eax
        test   eax, eax
        jne    loc_1_0C19
        mov    dword ptr [esi + 0x121], edi
loc_1_0C19:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1A62
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x11d], edi
        jne    loc_1_0BFE
loc_1_0C31:
        cmp    dword ptr [esi + 0x115], 0
        je     loc_1_0C6F
        xor    edi, edi
loc_1_0C3C:
        mov    edx, dword ptr [esi + 0x115]
        lea    ecx, [edx + 0x24]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x115], eax
        test   eax, eax
        jne    loc_1_0C57
        mov    dword ptr [esi + 0x119], edi
loc_1_0C57:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1A62
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x115], edi
        jne    loc_1_0C3C
loc_1_0C6F:
        mov    byte ptr [esi + 0x2a1], 0
        mov    byte ptr [esi + 0xcf], 0
        push   esi
        call   sub_2_1770
        test   ax, ax
        je     loc_1_0CAA
        mov    byte ptr [esi + 0x172], 1
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000003
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
        push   esi
        call   sub_1_1020
        jmp    loc_1_0CD2
loc_1_0CAA:
        mov    byte ptr [esi + 0x172], 0
        push   esi
        call   sub_1_1020
        jmp    loc_1_0CD2
loc_1_0CB9:
        cmp    dword ptr [esi + 0x11d], 0
        jne    loc_1_0CCB
        cmp    dword ptr [esi + 0x115], 0
        je     loc_1_0CD2
loc_1_0CCB:
        mov    byte ptr [esi + 0x2a1], 1
loc_1_0CD2:
        push   0x30d40
        add    esi, 0x295
        push   esi
        call   sub_1_1AFA
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0CF0:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [edx + 0x2e]
        or     byte ptr [eax], 2
        mov    cx, word ptr [edx + 0xcb]
        mov    word ptr [edx + 0xcd], cx
        lea    eax, [ecx + 1]
        mov    word ptr [edx + 0xcb], ax
        cmp    word ptr [edx + 0x4e], ax
        jne    loc_1_0D26
        mov    word ptr [edx + 0xcb], 0
loc_1_0D26:
        mov    ax, word ptr [edx + 0xcb]
        movsx  esi, ax
        cmp    dword ptr [edx + esi*4 + 0xd4], 2
        jne    loc_1_0D3F
        cmp    ax, cx
        jne    loc_1_0D48
loc_1_0D3F:
        mov    word ptr [edx + 0xcb], 0xffff
loc_1_0D48:
        movsx  eax, cx
        mov    bx, word ptr [edx + eax*4 + 0xd0]
        mov    al, byte ptr [edx + 0xa8]
        imul   cl
        shr    al, 8
        mov    ecx, dword ptr [edx + 0x2e]
        inc    al
        mov    byte ptr [ecx + 0x14], al
        mov    ecx, dword ptr [edx + 0x2e]
        pop    esi
        mov    byte ptr [ecx + 0x15], bl
        mov    ecx, dword ptr [edx + 0x2e]
        mov    byte ptr [ecx + 0x16], bh
        mov    ecx, dword ptr [edx + 0x2e]
        mov    byte ptr [edx + 0xcf], 1
        mov    byte ptr [ecx + 0x10], 6
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        push   0
        mov    di, word ptr [esi + 0xa8]
        mov    ecx, dword ptr [ebp + 0xc]
        movsx  eax, word ptr [esi + 0xc9]
        imul   edi, eax
        add    edi, dword ptr [esi + 0x2a]
        lea    eax, [ebp - 4]
        push   eax
        xor    ebx, ebx
        push   0
        push   0
        push   ecx
        call   sub_1_1B48
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_0E04
loc_1_0DCC:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        push   edi
        call   sub_1_1F00
        add    edi, dword ptr [ebp - 8]
        add    ebx, dword ptr [ebp - 8]
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0DCC
loc_1_0E04:
        movsx  eax, word ptr [esi + 0xc9]
        cmp    ebx, 0x3c
        jae    loc_1_0E24
        mov    dword ptr [esi + eax*4 + 0xd0], 0x3c
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0E24:
        pop    edi
        mov    dword ptr [esi + eax*4 + 0xd0], ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   0                             ; -> loc_1_15E0
        lea    eax, [esi + 0xb0]
        push   eax
        call   sub_1_1B12
        mov    cl, byte ptr [esi + 0xc8]
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [ebp - 1], cl
        mov    bl, byte ptr [eax + 0x13]
        inc    bl
        cmp    byte ptr [esi + 0xf3], bl
        jne    loc_1_0E7D
        mov    bl, byte ptr [esi + 0xf2]
loc_1_0E7D:
        xor    edi, edi
loc_1_0E7F:
        cmp    bl, byte ptr [ebp - 1]
        je     loc_1_0F50
loc_1_0E88:
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi + 0x4c]
        mov    al, bl
        imul   ecx, eax
        add    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], edi
        mov    ax, word ptr [ecx + 2]
        mov    word ptr [ebp - 8], ax
        cmp    dword ptr [ebp - 8], 0x600
        ja     loc_1_0FB3
        mov    bl, byte ptr [ecx + 1]
        cmp    byte ptr [esi + 0xf2], bl
        ja     loc_1_0F21
        cmp    byte ptr [esi + 0xf3], bl
        jb     loc_1_0F21
        add    ecx, 4
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 4
        push   esi
        push   eax
        mov    dword ptr [esi + 0xec], ecx
        mov    dword ptr [esi + 0xf4], eax
        call   sub_1_1850
        mov    dl, byte ptr [esi + 0xf3]
        cmp    bl, dl
        jb     loc_1_0EF1
        sub    bl, dl
        add    bl, byte ptr [esi + 0xf2]
loc_1_0EF1:
        cmp    byte ptr [esi + 0xf2], bl
        jne    loc_1_0F03
        dec    dl
        mov    ecx, dword ptr [esi + 0x2e]
        mov    byte ptr [ecx + 0x13], dl
        jmp    loc_1_0F0C
loc_1_0F03:
        lea    ecx, [ebx - 1]
        mov    edx, dword ptr [esi + 0x2e]
        mov    byte ptr [edx + 0x13], cl
loc_1_0F0C:
        cmp    ax, 5
        jne    loc_1_0E7F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0F21:
        mov    cl, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + 0xf3]
        cmp    cl, al
        jbe    loc_1_0F36
        sub    cl, al
        add    cl, byte ptr [esi + 0xf2]
loc_1_0F36:
        cmp    byte ptr [esi + 0xf2], cl
        jne    loc_1_0F48
        dec    al
        mov    ecx, dword ptr [esi + 0x2e]
        mov    byte ptr [ecx + 0x13], al
        jmp    loc_1_0F50
loc_1_0F48:
        dec    cl
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x13], cl
loc_1_0F50:
        mov    eax, dword ptr [esi + 0x2e]
        push   esi
        push   0                             ; -> loc_1_15E0
        lea    ecx, [esi + 0xb0]
        mov    byte ptr [eax + 0x17], 1
        push   ecx
        call   sub_1_1B12
        mov    cl, byte ptr [esi + 0xc8]
        mov    edx, dword ptr [esi + 0x2e]
        mov    byte ptr [ebp - 1], cl
        mov    bl, byte ptr [edx + 0x13]
        inc    bl
        cmp    byte ptr [esi + 0xf3], bl
        jne    loc_1_0F88
        mov    bl, byte ptr [esi + 0xf2]
loc_1_0F88:
        cmp    bl, byte ptr [ebp - 1]
        jne    loc_1_0E88
        cmp    byte ptr [esi + 0xf0], 0
        je     loc_1_0FA8
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x17], 0x10
        mov    byte ptr [esi + 0xf0], 0
loc_1_0FA8:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FB3:
        mov    eax, 0xffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0FD0:
        push   ebp
        mov    ecx, 0x10
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 0x20], cx
        mov    byte ptr [eax], 6
        mov    dword ptr [eax + 0x9c], 0x2a3
        mov    dword ptr [eax + 0xa0], 0x330000
        mov    dword ptr [eax + 0x10], 0
        mov    word ptr [eax + 0x32], cx
        pop    ebp
        mov    word ptr [eax + 0x58], 6
        mov    word ptr [eax + 0x5a], 2
        mov    word ptr [eax + 0x56], cx
        mov    ax, 1
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1020:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0xac], 1
        je     loc_1_1037
        xor    ax, ax
        pop    ebp
        ret    4
loc_1_1037:
        mov    ecx, dword ptr [eax + 0x2e]
        pop    ebp
        mov    byte ptr [ecx + 0x10], 0
        mov    dl, byte ptr [eax + 0xae]
        mov    ecx, dword ptr [eax + 0x2e]
        xor    ax, ax
        mov    byte ptr [ecx + 0x1f], dl
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1060:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        pop    ebp
        mov    ecx, dword ptr [eax + 0x2e]
        mov    byte ptr [ecx + 0x10], dl
        mov    ecx, dword ptr [eax + 0x2e]
        xor    ax, ax
        mov    byte ptr [ecx + 0x1f], dl
        ret    4
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 080h, 0A0h, 0AEh, 000h, 000h
        db  000h, 0F5h, 066h, 033h, 0C0h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h
sub_1_10A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        and    byte ptr [eax + 0xae], 0xca
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h
        db  08Bh, 0ECh, 080h, 088h, 0AEh, 000h, 000h, 000h, 00Ah, 066h, 033h, 0C0h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        or     byte ptr [eax + 0xae], 0x35
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1100:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 1
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0
        mov    eax, dword ptr [esi + 0x2e]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [esi + 0x2e]
        mov    cl, byte ptr [eax + 0x17]
        and    cl, 0x35
        je     loc_1_121F
        mov    byte ptr [ebp - 1], bl
loc_1_112C:
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x10], bl
        xor    eax, eax
        mov    edx, dword ptr [esi + 0x2e]
        mov    al, byte ptr [edx + 0x10]
        test   eax, 0xfffffff9
        jne    loc_1_120C
        cmp    byte ptr [edx + 0x17], 0xff
        je     loc_1_120C
        test   cl, 0x10
        je     loc_1_116E
        mov    byte ptr [esi + 0xf0], 1
        push   esi
        call   sub_1_0E40
        test   eax, eax
        je     loc_1_11FB
        jmp    loc_1_121F
loc_1_116E:
        test   cl, 0x20
        je     loc_1_11A0
        mov    eax, dword ptr [esi + 0x74]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 0x1d]
        add    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 0x2e]
        mov    edx, dword ptr [esi + 0x68]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x1e]
        add    dword ptr [edx], ecx
        mov    edx, dword ptr [esi + 0x6c]
        mov    eax, dword ptr [esi + 0x2e]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x1f]
        add    dword ptr [edx], ecx
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x17], 0x20
        jmp    loc_1_11FB
loc_1_11A0:
        test   cl, 1
        je     loc_1_11B1
        push   esi
        call   sub_1_0E40
        test   eax, eax
        je     loc_1_11FB
        jmp    loc_1_121F
loc_1_11B1:
        test   cl, 4
        je     loc_1_11FB
        mov    cl, byte ptr [edx + 0x1c]
        test   cl, 2
        je     loc_1_11C3
        mov    eax, dword ptr [esi + 0x68]
        inc    dword ptr [eax]
loc_1_11C3:
        test   cl, 4
        je     loc_1_11D8
        mov    edx, dword ptr [esi + 0x74]
        mov    eax, dword ptr [esi + 0x2e]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        add    dword ptr [edx], eax
loc_1_11D8:
        test   cl, 8
        je     loc_1_11E2
        mov    eax, dword ptr [esi + 0x78]
        inc    dword ptr [eax]
loc_1_11E2:
        test   cl, 0x10
        je     loc_1_11F4
        mov    ecx, dword ptr [esi + 0x6c]
        mov    eax, dword ptr [esi + 0x2e]
        xor    edx, edx
        mov    dl, byte ptr [eax + 0x1f]
        add    dword ptr [ecx], edx
loc_1_11F4:
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x17], 4
loc_1_11FB:
        mov    eax, dword ptr [esi + 0x2e]
        mov    cl, byte ptr [eax + 0x17]
        and    cl, 0x35
        jne    loc_1_112C
        jmp    loc_1_121F
loc_1_120C:
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000003
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
loc_1_121F:
        cmp    byte ptr [esi + 0xaf], 0
        je     loc_1_123A
        mov    byte ptr [esi + 0xaf], 0
        xor    ax, ax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_123A:
        mov    eax, 0
        pop    esi
        cmp    byte ptr [ebp - 1], 1
        adc    eax, -1
        pop    ebx
        and    ax, 3
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1260:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x2e]
        mov    cl, byte ptr [eax + 0x17]
        and    cl, 0x1a
        test   cl, 0xa
        je     loc_1_139E
        mov    edi, 0xffffffff
loc_1_1280:
        mov    eax, dword ptr [esi + 0x2e]
        xor    edx, edx
        mov    byte ptr [eax + 0x10], 0
        mov    eax, dword ptr [esi + 0x2e]
        mov    dl, byte ptr [eax + 0x10]
        test   edx, 0xfffffff9
        jne    loc_1_138B
        add    eax, 0x17
        cmp    byte ptr [eax], 0xff
        je     loc_1_138B
        test   cl, 2
        jne    loc_1_1328
        test   cl, 8
        je     loc_1_136F
        mov    byte ptr [eax], 8
        mov    eax, dword ptr [esi + 0x2e]
        mov    al, byte ptr [eax + 0x14]
        mov    bl, al
        and    bl, 8
        je     loc_1_12CD
        mov    eax, dword ptr [esi + 0x80]
        jmp    loc_1_12D7
loc_1_12CD:
        test   al, 0x20
        je     loc_1_12D9
        mov    eax, dword ptr [esi + 0x98]
loc_1_12D7:
        inc    dword ptr [eax]
loc_1_12D9:
        mov    byte ptr [esi + 0xcf], 0
        mov    byte ptr [esi + 0x2a1], 0
        movsx  eax, word ptr [esi + 0xcd]
        mov    dword ptr [esi + eax*4 + 0xd4], 0
        mov    word ptr [esi + 0xcd], di
        cmp    word ptr [esi + 0xcb], di
        je     loc_1_130F
        push   esi
        call   sub_1_0CF0
loc_1_130F:
        push   esi
        mov    eax, 0xc
        cmp    bl, 1
        adc    eax, edi
        push   eax
        call   sub_1_1690
        cmp    ax, 5
        je     loc_1_139E
        jmp    loc_1_136F
loc_1_1328:
        mov    byte ptr [eax], 2
        mov    byte ptr [esi + 0xcf], 0
        mov    byte ptr [esi + 0x2a1], 0
        movsx  eax, word ptr [esi + 0xcd]
        mov    dword ptr [esi + eax*4 + 0xd4], 0
        mov    word ptr [esi + 0xcd], di
        cmp    word ptr [esi + 0xcb], di
        je     loc_1_1361
        push   esi
        call   sub_1_0CF0
loc_1_1361:
        push   esi
        push   0
        call   sub_1_1690
        cmp    ax, 5
        je     loc_1_139E
loc_1_136F:
        mov    eax, dword ptr [esi + 0x2e]
        mov    cl, byte ptr [eax + 0x17]
        and    cl, 0x1a
        test   cl, 0xa
        jne    loc_1_1280
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_138B:
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000003
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
loc_1_139E:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_13B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    si, word ptr [edi + 0xc9]
        movsx  eax, si
        lea    ebx, [edi + eax*4 + 0xd4]
        cmp    dword ptr [ebx], 0
        je     loc_1_13DA
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_13DA:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 1
        push   eax
        push   edi
        call   sub_1_0D90
        mov    ax, word ptr [edi + 0xc9]
        inc    ax
        mov    word ptr [edi + 0xc9], ax
        cmp    ax, 1
        jne    loc_1_1409
        mov    word ptr [edi + 0xc9], 0
loc_1_1409:
        mov    dword ptr [ebx], 2
        cmp    word ptr [edi + 0xcb], -1
        jne    loc_1_1430
        cmp    word ptr [edi + 0xcd], -1
        mov    word ptr [edi + 0xcb], si
        jne    loc_1_1430
        push   edi
        call   sub_1_0CF0
loc_1_1430:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1440:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x10
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 0x18]
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ecx + 0xf4]
        mov    esi, dword ptr [ebp + 0xc]
        lea    ecx, [esi + edx]
        cmp    ecx, eax
        jbe    loc_1_146D
        sub    eax, edx
        mov    esi, eax
loc_1_146D:
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [eax + 0xec]
        add    edi, edx
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   ecx
        call   sub_1_1B48
        lea    ecx, [ebp - 0x10]
        lea    edx, [ebp - 0xc]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        push   eax
        call   sub_1_1B6C
        mov    dword ptr [ebp - 8], 0
        test   esi, esi
        je     loc_1_1537
loc_1_14AB:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ebx, esi
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, esi
        ja     loc_1_14B9
        mov    ebx, eax
loc_1_14B9:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebx + edi]
        mov    eax, dword ptr [eax + 0xdc]
        cmp    ecx, eax
        jbe    loc_1_14CD
        mov    ebx, eax
        sub    ebx, edi
loc_1_14CD:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   edi
        sub    esi, ebx
        push   eax
        add    edi, ebx
        call   sub_1_1F00
        mov    eax, dword ptr [ebp + 8]
        add    dword ptr [eax], ebx
        test   esi, esi
        je     loc_1_1537
        mov    eax, dword ptr [ebp + 0x18]
        cmp    dword ptr [eax + 0xdc], edi
        jne    loc_1_14F6
        mov    edi, dword ptr [eax + 0xd8]
loc_1_14F6:
        add    dword ptr [ebp - 0xc], ebx
        add    dword ptr [ebp - 8], ebx
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 0x10]
        jne    loc_1_152F
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1537
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        mov    dword ptr [ebp - 8], 0
loc_1_152F:
        test   esi, esi
        jne    loc_1_14AB
loc_1_1537:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1550:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    dword ptr [ebp - 8], eax
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0xf4]
        lea    edx, [edi + eax]
        cmp    edx, ecx
        jbe    loc_1_157A
        sub    ecx, eax
        mov    edi, ecx
loc_1_157A:
        mov    ebx, dword ptr [esi + 0xec]
        test   edi, edi
        je     loc_1_15C6
loc_1_1584:
        mov    eax, dword ptr [esi + 0xdc]
        lea    ecx, [edi + ebx]
        mov    dword ptr [ebp - 4], edi
        cmp    ecx, eax
        jbe    loc_1_1599
        sub    eax, ebx
        mov    dword ptr [ebp - 4], eax
loc_1_1599:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ebx
        push   ecx
        call   sub_1_1F00
        add    ebx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    dword ptr [ebp + 0x10], ecx
        sub    edi, ecx
        je     loc_1_15C6
        cmp    dword ptr [esi + 0xdc], ebx
        jne    loc_1_15C2
        mov    ebx, dword ptr [esi + 0xd8]
loc_1_15C2:
        test   edi, edi
        jne    loc_1_1584
loc_1_15C6:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_15E0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0x2e]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [ecx + 0x2e]
        mov    byte ptr [eax + 0x10], 0x40
        mov    eax, dword ptr [ecx + 0x2e]
        pop    ebp
        mov    al, byte ptr [eax + 0x17]
        mov    byte ptr [ecx + 0xc8], al
        mov    ecx, dword ptr [ecx + 0x2e]
        mov    al, 1
        mov    byte ptr [ecx + 0x10], 0
        ret    4
        db  08Dh, 049h, 000h, 055h, 08Bh, 054h, 024h, 008h, 08Bh, 0ECh, 053h, 08Bh, 042h, 02Eh, 0B3h, 018h
        db  080h, 008h, 002h, 08Bh, 042h, 02Eh, 0C6h, 040h, 010h, 040h, 033h, 0C0h, 08Bh, 04Ah, 02Eh, 08Ah
        db  0C3h, 0FEh, 0C3h, 080h, 0FBh, 01Fh, 0C6h, 004h, 008h, 0FFh, 076h, 0EEh, 08Bh, 042h, 02Eh, 05Bh
        db  05Dh, 0C6h, 040h, 010h, 000h, 0B0h, 001h, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h, 055h, 08Bh, 054h, 024h, 008h, 08Bh, 0ECh, 053h, 08Bh, 042h, 02Eh, 0B3h, 018h
        db  080h, 008h, 002h, 08Bh, 042h, 02Eh, 0C6h, 040h, 010h, 040h, 033h, 0C0h, 08Bh, 04Ah, 02Eh, 08Ah
        db  0C3h, 0FEh, 0C3h, 080h, 0FBh, 01Fh, 0C6h, 004h, 008h, 000h, 076h, 0EEh, 08Bh, 042h, 02Eh, 05Bh
        db  05Dh, 0C6h, 040h, 010h, 000h, 0B0h, 001h, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h
sub_1_1690:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x11d]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x11d], eax
        test   eax, eax
        jne    loc_1_16B6
        mov    dword ptr [esi + 0x121], 0
loc_1_16B6:
        cmp    byte ptr [ecx + 0x2c], 0
        mov    edi, dword ptr [ecx + 0x28]
        je     loc_1_16F1
        cmp    dword ptr [esi + 0x125], 0
        jne    loc_1_16D6
        mov    dword ptr [esi + 0x129], ecx
        mov    dword ptr [esi + 0x125], ecx
        jmp    loc_1_16E5
loc_1_16D6:
        mov    eax, dword ptr [esi + 0x129]
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [esi + 0x129], ecx
loc_1_16E5:
        mov    dword ptr [ecx + 0x24], 0
        jmp    loc_1_1793
loc_1_16F1:
        mov    ax, word ptr [ebp + 8]
        test   ax, ax
        jne    loc_1_1702
        inc    dword ptr [esi + 0x13a]
        jmp    loc_1_1708
loc_1_1702:
        inc    dword ptr [esi + 0x142]
loc_1_1708:
        cmp    ax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0xc0000001
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1A62
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_1_1793
        mov    eax, dword ptr [ebx + 0x109]
        cmp    edi, eax
        jne    loc_1_1740
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x109], eax
        jmp    loc_1_1753
loc_1_1740:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_1_174D
loc_1_1745:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_1745
loc_1_174D:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_1_1753:
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [ebx + 0x17d], eax
        jne    loc_1_1764
        push   ebx
        call   sub_2_09A0
loc_1_1764:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
loc_1_1770:
        push   0x75
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 0x109], 0
        jne    loc_1_1793
        push   ebx
        push   0                             ; -> loc_2_1620
        add    ebx, 0xb0
        push   ebx
        call   sub_1_1B12
loc_1_1793:
        mov    edi, dword ptr [esi + 0x115]
        test   edi, edi
        je     loc_1_1837
        cmp    byte ptr [esi + 0x173], 0
        jne    loc_1_1837
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x115], eax
        test   eax, eax
        jne    loc_1_17C5
        mov    dword ptr [esi + 0x119], 0
loc_1_17C5:
        push   esi
        push   edi
        call   sub_1_13B0
        cmp    ax, 6
        jne    loc_1_1805
        mov    eax, dword ptr [esi + 0x115]
        test   eax, eax
        je     loc_1_17EF
        mov    dword ptr [edi + 0x24], eax
        xor    ax, ax
        mov    dword ptr [esi + 0x115], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_17EF:
        xor    ax, ax
        mov    dword ptr [esi + 0x115], edi
        mov    dword ptr [esi + 0x119], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_1805:
        test   ax, ax
        jne    loc_1_1837
        cmp    dword ptr [esi + 0x11d], 0
        jne    loc_1_1821
        mov    dword ptr [esi + 0x121], edi
        mov    dword ptr [esi + 0x11d], edi
        jmp    loc_1_1830
loc_1_1821:
        mov    eax, dword ptr [esi + 0x121]
        mov    dword ptr [eax + 0x24], edi
        mov    dword ptr [esi + 0x121], edi
loc_1_1830:
        mov    dword ptr [edi + 0x24], 0
loc_1_1837:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1850:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 0x131], 1
        mov    dword ptr [edi + 0x12d], 0
        mov    ebx, dword ptr [edi + 0x17d]
        add    ebx, 0xe
        cmp    ebx, esi
        jb     loc_1_187C
        mov    ebx, esi
loc_1_187C:
        push   edi
        lea    eax, [edi + 0x191]
        push   eax
        push   0
        push   ebx
        call   sub_1_1550
        test   ax, ax
        je     loc_1_189B
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_189B:
        inc    dword ptr [edi + 0x13e]
        cmp    esi, 0xe
        jae    loc_1_18D1
        cmp    esi, 6
        jb     loc_1_18F7
        push   0
        lea    eax, [edi + 0x191]
        push   0
        mov    edx, dword ptr [edi + 0xf9]
        push   0
        push   esi
        push   eax
        push   eax
        push   edi
        push   edx
        call   sub_1_1C10
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_18D1:
        sub    esi, 0xe
        sub    ebx, 0xe
        push   esi
        lea    eax, [edi + 0x19f]
        push   ebx
        lea    ecx, [edi + 0x191]
        push   eax
        push   0xe
        push   ecx
        mov    eax, dword ptr [edi + 0xf9]
        push   ecx
        push   edi
        push   eax
        call   sub_1_1C10
loc_1_18F7:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1910:
        mov    al, byte ptr [0]              ; -> obj1:0x1FAC
        cmp    al, 0
        jne    loc_1_1935
        inc    byte ptr [0]                  ; -> obj1:0x1FAC
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_1937
        push   0
        push   0
        call   sub_2_0000
        cmp    eax, 0
        jne    loc_1_1937
loc_1_1935:
        clc    
        ret    
loc_1_1937:
        stc    
        ret    
SMC80PC_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_1948
        jmp    dword ptr [eax*4]             ; -> obj1:0x194A
loc_1_1948:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_1910
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1948
        dd  loc_1_1910
        db  0CCh, 0CCh, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh
        db  0F3h, 06Eh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh
        db  054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h
        db  08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h
        db  08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh
        db  0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h
        db  0FCh, 0F3h, 066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h
        db  00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_1A32:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_1A38:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_1A3E:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_1A44:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_1A4A:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_1A50:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_1A5C:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_1A62:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_1A6E:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_1A7A:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_1A80:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_1A86:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_1A8C:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_1AA4:
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
sub_1_1AEC:
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        int3   
        int3   
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_1AFA:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_1B00:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_1B06:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_1B0C:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_1B12:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_1B18:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_1B1E:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_1B24:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_1B48:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_1B6C:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_1B90:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_1B9C:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_1BA2:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_1BA8:
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
sub_1_1BEA:
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_1BF0:
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
sub_1_1C10:
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
        call   sub_1_1C40
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_1C40:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_1CE0
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_1C80
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_1C75
        cmp    al, 0xff
        jne    loc_1_1C75
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_1C75
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_1CE7
loc_1_1C75:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_1CE7
        db  08Bh, 0FFh
loc_1_1C80:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_1CD0
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_1CA6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_1CAD
loc_1_1CA6:
        mov    dword ptr [ebp - 0x10], 1
loc_1_1CAD:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1CC0
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_1CEA
        db  08Dh, 064h, 024h, 000h
loc_1_1CC0:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_1CEA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1CD0:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_1CEA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1CE0:
        mov    dword ptr [ebp - 4], 0x20
loc_1_1CE7:
        mov    esi, dword ptr [ebp + 8]
loc_1_1CEA:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_1DE1
loc_1_1CF5:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    loc_1_1D4D
        test   al, 2
        je     loc_1_1D39
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_2160
        or     al, al
        je     loc_1_1D39
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    loc_1_1D4D
loc_1_1D39:
        test   byte ptr [ebp - 4], 2
        je     loc_1_1D44
        test   bl, 4
        jne    loc_1_1D4D
loc_1_1D44:
        test   bl, 0x20
        je     loc_1_1DD6
loc_1_1D4D:
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
        call   sub_1_1A7A
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_1DD6
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_1D8F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_1D8F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_1DA0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_1DA6
        db  08Bh, 0FFh
loc_1_1DA0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_1DA6:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_1DB5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1A80
loc_1_1DB5:
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
        call   sub_1_1B24
        mov    edi, ebx
        jmp    loc_1_1DD9
loc_1_1DD6:
        mov    edi, dword ptr [edi + 0x10]
loc_1_1DD9:
        or     edi, edi
        jne    loc_1_1CF5
loc_1_1DE1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1DF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1E00
        pop    ebp
        ret    4
sub_1_1E00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_1E7E
loc_1_1E10:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_1E77
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1A80
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_1E77
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_1E3C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_1E3C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_1E50
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_1E56
        db  005h, 000h, 000h, 000h, 000h
loc_1_1E50:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_1E56:
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
        call   sub_1_1B24
        mov    edi, ebx
        jmp    loc_1_1E7A
loc_1_1E77:
        mov    edi, dword ptr [edi + 0x10]
loc_1_1E7A:
        or     edi, edi
        jne    loc_1_1E10
loc_1_1E7E:
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
        je     loc_1_1EA5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1EA5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_1EE1
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_1ED2
        cmp    bl, 0xff
        jne    loc_1_1ED2
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_1ED2
        test   al, 8
        je     loc_1_1ECB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1ECB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_1ED2:
        test   al, 4
        jne    loc_1_1EDA
        test   al, 2
        je     loc_1_1EF9
loc_1_1EDA:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1EE1:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_1EF9
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_1EF9
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1EF9:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_1F00:
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
sub_1_1F20:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_1F40
        or     ecx, ecx
        je     loc_1_1F56
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_1F54
loc_1_1F40:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_1F54
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_1F54:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_1F56:
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
        db  'SMC80PC '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  SMC80PC_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 0CCh, 0CCh, 0CCh, 040h, 028h, 023h, 029h
        db  020h, 053h, 04Dh, 043h, 038h, 030h, 050h, 043h, 02Eh, 056h, 058h, 044h, 020h, 076h, 031h, 02Eh
        db  030h, 030h, 062h, 020h, 031h, 031h, 02Fh, 032h, 031h, 02Fh, 039h, 034h, 00Ah, 020h, 028h, 043h
        db  029h, 020h, 053h, 074h, 061h, 06Eh, 064h, 061h, 072h, 064h, 020h, 04Dh, 069h, 063h, 072h, 06Fh
        db  073h, 079h, 073h, 074h, 065h, 06Dh, 073h, 020h, 043h, 06Fh, 072h, 070h, 02Ch, 020h, 031h, 039h
        db  039h, 034h, 000h, 000h, 040h, 028h, 023h, 029h, 020h, 055h, 04Dh, 05Fh, 038h, 030h, 031h, 036h
        db  077h, 069h, 06Eh, 039h, 035h, 05Fh, 073h, 032h, 02Eh, 03Fh, 03Fh, 05Fh, 076h, 031h, 02Eh, 030h
        db  031h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2074:
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
loc_1_20F0:
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
        cmp    byte ptr [eax], dh
        push   eax
        inc    ebx
        db  000h
loc_1_2154:
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [eax], dh
        push   eax
        inc    ebx
        add    byte ptr [ebp + 0x41], cl
        pop    eax
        dec    ebp
loc_1_2160:
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
loc_1_216C:
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
        add    byte ptr [ebp + 0x45], cl
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    ebp
        inc    ebp
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_21B4:
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x2231
        jb     0x21ee
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        sub    byte ptr [ebx], ah
        sub    dword ptr [eax], esp
        dec    esp
        dec    ebp
        pop    edi
        cmp    byte ptr [eax], dh
        xor    dword ptr [esi], esi
        pop    edi
        jae    0x2205
        aas    
        aas    
        pop    edi
        jbe    0x220a
        xor    byte ptr cs:[ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2220  R-X
sub_2_0000:
        push   ebp
        mov    eax, 8
        mov    ebp, esp
        sub    esp, 0x18
        mov    word ptr [ebp - 0x18], ax
        push   ebx
        mov    word ptr [ebp - 0x16], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 0x10], bx
        push   ebx
        mov    word ptr [ebp - 0xe], bx
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        lea    eax, [ebp - 0x10]
        push   eax
        lea    edx, [ebp - 8]
        mov    dword ptr [ebp - 0x14], 0     ; -> loc_1_2154
        push   edx
        call   sub_1_1A32
        lea    ecx, [ebp - 0x10]
        mov    dword ptr [0x4a], ecx         ; -> obj1:0x206E
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [4], edx            ; -> obj1:0x2028
        mov    byte ptr [0x4e], bl           ; -> obj1:0x2072
        mov    dword ptr [0x3e], ebx         ; -> obj1:0x2062
        mov    dword ptr [0x42], ebx         ; -> obj1:0x2066
        mov    byte ptr [8], 3               ; -> obj1:0x202C
        mov    byte ptr [9], 0xa             ; -> obj1:0x202D
        mov    dword ptr [0xa], ebx          ; -> obj1:0x202E
        mov    dword ptr [0xe], 0            ; -> obj1:0x2032
        mov    dword ptr [0x12], 0           ; -> obj1:0x2036
        mov    dword ptr [0x16], 0           ; -> obj1:0x203A
        mov    dword ptr [0x1a], 0           ; -> obj1:0x203E
        mov    dword ptr [0x1e], 0           ; -> obj1:0x2042
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [0x22], 0           ; -> obj1:0x2046
        mov    dword ptr [0x26], 0           ; -> obj1:0x204A
        mov    dword ptr [0x2a], 0           ; -> obj1:0x204E
        mov    dword ptr [0x2e], 0           ; -> obj1:0x2052
        mov    dword ptr [0x32], 0           ; -> obj1:0x2056
        push   0x36
        mov    dword ptr [0x36], eax         ; -> obj1:0x205A
        push   8                             ; -> obj1:0x202C
        lea    eax, [ebp - 4]
        push   0                             ; -> obj1:0x2024
        mov    dword ptr [0x3a], ecx         ; -> obj1:0x205E
        mov    edx, dword ptr [ebp - 8]
        push   edx
        push   0                             ; -> obj1:0x2024
        push   eax
        call   sub_1_1A3E
        cmp    dword ptr [ebp - 4], ebx
        je     loc_2_011B
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1A38
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_011B:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0130:
        push   ebp
        mov    eax, 0xd
        mov    ebp, esp
        sub    esp, 0x84
        mov    word ptr [ebp - 0x3c], ax
        push   ebx
        mov    word ptr [ebp - 0x3a], ax
        push   esi
        push   edi
        mov    eax, 4
        mov    word ptr [ebp - 0x34], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 0x32], ax
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [ebp - 0x2c], bl
        mov    byte ptr [ebp - 0x2b], bl
        mov    byte ptr [ebp - 0x2a], bl
        mov    byte ptr [ebp - 0x29], bl
        mov    esi, 0x10
        mov    byte ptr [ebp - 0x28], bl
        mov    eax, dword ptr [ebp + 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        mov    byte ptr [ebp - 0x27], bl
        mov    dword ptr [ebp - 0x38], 0     ; -> loc_1_216C
        mov    dword ptr [ebp - 0x30], 0     ; -> obj1:0x2198
        push   edx
        call   sub_1_1B9C
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_2_01A4
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_01A4:
        push   0
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1BA2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_01C7
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [eax + 4]
loc_2_01C7:
        push   0
        lea    eax, [ebp - 0x34]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1BA2
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_01EF
        mov    eax, dword ptr [ebp - 0x14]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 4], ax
loc_2_01EF:
        lea    edi, [ebp - 0x5a]
        xor    eax, eax
        mov    word ptr [ebp - 0x5c], 0
        mov    ecx, 7
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    eax, [ebp - 0x1c]
        lea    ecx, [ebp - 0x20]
        push   eax
        lea    edx, [ebp - 0x5c]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   0x20
        lea    ecx, [ebp - 0xc]
        push   edx
        push   1
        push   eax
        push   ecx
        call   sub_1_1AEC
        add    esp, 0x1c
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_0237
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0237:
        mov    eax, dword ptr [ebp - 0x58]
        mov    ecx, dword ptr [ebp - 0x48]
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x20], eax
        push   edx
        lea    eax, [ebp - 0x24]
        push   eax
        lea    edx, [ebp - 0xc]
        mov    dword ptr [ebp - 0x1c], ecx
        lea    ecx, [ebp - 0x18]
        push   ecx
        push   edx
        call   sub_1_1AA4
        cmp    dword ptr [ebp - 0x24], 6
        jne    loc_2_027A
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_027A
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, ecx
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 0x2c], edx
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 0x28], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 0x27], cl
loc_2_027A:
        xor    edi, edi
loc_2_027C:
        mov    eax, dword ptr [0]            ; -> obj1:0x2020
        lea    ecx, [ebp - 8]
        push   eax
        push   edi
        push   0x2a2
        push   ecx
        call   sub_1_1B1E
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_02A3
        mov    byte ptr [ebp - 1], 1
        mov    ebx, 0xaa000002
        jmp    loc_2_027C
loc_2_02A3:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0xa8
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x20]
        mov    edx, dword ptr [ebp - 0x1c]
        lea    edi, [ebp - 0x84]
        mov    dword ptr [ecx + 0x22], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ebp - 4]
        mov    dword ptr [ecx + 0x26], edx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x14], ax
        xor    eax, eax
        mov    ecx, 9
        lea    edx, [ebp - 0x84]
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        push   edx
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   edx
        mov    ecx, dword ptr [0]            ; -> obj1:0x2024
        push   ecx
        lea    eax, [edx + 0x5e]
        mov    dword ptr [ebp - 0x7a], 1
        push   eax
        call   sub_1_1A4A
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_2_0337
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1BA8
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x2a2
        push   eax
        call   sub_1_1B24
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0337:
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_0382
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 0x5e]
        push   ecx
        call   sub_1_1BF0
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_1BA8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 0x5e]
        push   edx
        call   sub_1_1A50
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x2a2
        push   ecx
        call   sub_1_1B24
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0382:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0FD0
        cmp    ax, 0xffff
        jne    loc_2_03DA
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0xaa000004
        mov    ecx, dword ptr [eax + 0x5e]
        push   ecx
        call   sub_1_1BF0
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_1BA8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 0x5e]
        push   edx
        call   sub_1_1A50
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x2a2
        push   ecx
        call   sub_1_1B24
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_03DA:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1BA8
        lea    eax, [ebp - 0x2c]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   esi
        push   1
        push   ecx
        call   sub_2_0430
        test   eax, eax
        je     loc_2_0421
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0x5e]
        push   ecx
        call   sub_1_1A50
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0x2a2
        push   ecx
        call   sub_1_1B24
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0421:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_2_0430:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x111], eax
        cmp    edi, 1
        jbe    loc_2_0474
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000002
        push   eax
        call   sub_1_1BF0
        mov    dword ptr [ebp - 4], 0xc000009a
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0474:
        xor    ebx, ebx
        mov    eax, 0                        ; -> sub_1_04B0
        mov    dword ptr [esi + 0x109], ebx
        mov    dword ptr [esi + 0x18d], eax
        mov    dword ptr [esi + 0x181], ebx
        mov    dword ptr [esi + 0x185], eax
        push   1
        mov    dword ptr [esi + 0x189], esi
        mov    dword ptr [esi + 0x101], 0    ; -> obj1:0x2024
        mov    ecx, dword ptr [0x42]         ; -> obj1:0x2066
        push   ebx
        mov    dword ptr [esi + 0x105], ecx
        mov    dword ptr [0x42], esi         ; -> obj1:0x2066
        movsx  eax, byte ptr [esi + 0x14]
        push   eax
        mov    edx, dword ptr [esi + 0x5e]
        push   eax
        lea    ecx, [esi + 0xb0]
        push   0                             ; -> loc_1_0330
        lea    eax, [ebp - 4]
        push   esi
        push   0                             ; -> loc_1_02F0
        push   edx
        push   ecx
        push   eax
        call   sub_1_1B06
        cmp    dword ptr [ebp - 4], ebx
        je     loc_2_0500
        push   ebx
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000005
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0500:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x22]
        mov    ax, word ptr [esi + 0x20]
        mov    edx, dword ptr [esi + 0x5e]
        shl    eax, 0xa
        push   eax
        push   ecx
        push   edx
        lea    eax, [esi + 8]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_1BEA
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_053D
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000001
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
        jmp    loc_2_074F
loc_2_053D:
        lea    ebx, [esi + 0xc]
        push   0x1000
        mov    eax, dword ptr [esi + 0x26]
        mov    ecx, dword ptr [esi + 0x5e]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   ebx
        push   edx
        call   sub_1_1BEA
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0575
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000001
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
        jmp    loc_2_074F
loc_2_0575:
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 0x2e], eax
        add    eax, 0x100
        mov    dword ptr [esi + 0x2a], eax
        mov    eax, dword ptr [ebx]
        mov    cl, byte ptr [eax + 0xc6]
        mov    byte ptr [esi + 0x3e], cl
        mov    dl, byte ptr [eax + 0xc8]
        mov    byte ptr [esi + 0x3f], dl
        mov    bl, byte ptr [eax + 0xca]
        mov    byte ptr [esi + 0x40], bl
        mov    cl, byte ptr [eax + 0xcc]
        mov    byte ptr [esi + 0x41], cl
        mov    dl, byte ptr [eax + 0xce]
        mov    byte ptr [esi + 0x42], dl
        mov    edx, dword ptr [ebp + 0x14]
        mov    bl, byte ptr [eax + 0xd0]
        mov    byte ptr [esi + 0x43], bl
        mov    cl, byte ptr [edx]
        test   cl, cl
        jne    loc_2_061C
        cmp    byte ptr [edx + 1], 0
        jne    loc_2_061C
        cmp    byte ptr [edx + 2], 0
        jne    loc_2_061C
        cmp    byte ptr [edx + 3], 0
        jne    loc_2_061C
        cmp    byte ptr [edx + 4], 0
        jne    loc_2_061C
        cmp    byte ptr [edx + 5], 0
        jne    loc_2_061C
        lea    ebx, [esi + 0x38]
        mov    cl, byte ptr [eax + 0xc6]
        mov    byte ptr [ebx], cl
        mov    dl, byte ptr [eax + 0xc8]
        mov    byte ptr [esi + 0x39], dl
        mov    cl, byte ptr [eax + 0xca]
        mov    byte ptr [esi + 0x3a], cl
        mov    dl, byte ptr [eax + 0xcc]
        mov    byte ptr [esi + 0x3b], dl
        mov    cl, byte ptr [eax + 0xce]
        mov    byte ptr [esi + 0x3c], cl
        mov    al, byte ptr [eax + 0xd0]
        mov    byte ptr [esi + 0x3d], al
        jmp    loc_2_063F
loc_2_061C:
        lea    ebx, [esi + 0x38]
        mov    byte ptr [ebx], cl
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [esi + 0x39], al
        mov    cl, byte ptr [edx + 2]
        mov    byte ptr [esi + 0x3a], cl
        mov    al, byte ptr [edx + 3]
        mov    byte ptr [esi + 0x3b], al
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [esi + 0x3c], cl
        mov    dl, byte ptr [edx + 5]
        mov    byte ptr [esi + 0x3d], dl
loc_2_063F:
        mov    word ptr [esi + 0x4e], di
        lea    eax, [esi + 0x162]
        lea    ecx, [esi + 0x16a]
        mov    dword ptr [esi + 0x68], eax
        lea    edx, [esi + 0x166]
        mov    dword ptr [esi + 0x70], ecx
        lea    eax, [esi + 0x15e]
        mov    dword ptr [esi + 0x6c], edx
        lea    ecx, [esi + 0x16e]
        mov    dword ptr [esi + 0x74], eax
        lea    edx, [esi + 0x14e]
        mov    dword ptr [esi + 0x78], ecx
        lea    eax, [esi + 0x142]
        mov    dword ptr [esi + 0x7c], edx
        lea    ecx, [esi + 0x146]
        mov    dword ptr [esi + 0x80], eax
        lea    edx, [esi + 0x14a]
        mov    dword ptr [esi + 0x84], ecx
        lea    eax, [esi + 0x152]
        mov    dword ptr [esi + 0x88], edx
        lea    ecx, [esi + 0x156]
        mov    dword ptr [esi + 0x8c], eax
        lea    edx, [esi + 0x15a]
        mov    dword ptr [esi + 0x90], ecx
        lea    edi, [esi + 0xf9]
        mov    dword ptr [esi + 0x98], edx
        mov    word ptr [esi + 0x4a], 0x5ea
        push   esi
        mov    dword ptr [edi], 0
        mov    word ptr [esi + 0x4c], 0x100
        mov    word ptr [esi + 0xa8], 0x600
        call   sub_2_1770
        test   ax, ax
        je     loc_2_0710
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        mov    byte ptr [esi + 0x172], 1
        push   0xaa000003
        push   eax
        call   sub_1_1BF0
        mov    dword ptr [ebp - 4], 0xc0010006
        add    esp, 0xc
        jmp    loc_2_074F
loc_2_0710:
        push   edi
        lea    eax, [esi + 0xfd]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        push   ebx
        push   0                             ; -> loc_2_1610
        push   0                             ; -> loc_2_15E0
        push   0                             ; -> loc_2_15D0
        push   ecx
        call   sub_2_19A0
        test   al, al
        jne    loc_2_07B4
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000002
        push   eax
        call   sub_1_1BF0
        mov    dword ptr [ebp - 4], 0xc0000001
        add    esp, 0xc
loc_2_074F:
        lea    eax, [esi + 0xb0]
        push   eax
        call   sub_1_1B0C
        cmp    dword ptr [0x42], esi         ; -> obj1:0x2066
        jne    loc_2_077A
        mov    eax, dword ptr [esi + 0x105]
        pop    edi
        pop    esi
        mov    dword ptr [0x42], eax         ; -> obj1:0x2066
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_077A:
        mov    ecx, dword ptr [0x42]         ; -> obj1:0x2066
        cmp    dword ptr [ecx + 0x105], esi
        je     loc_2_0796
loc_2_0788:
        mov    ecx, dword ptr [ecx + 0x105]
        cmp    dword ptr [ecx + 0x105], esi
        jne    loc_2_0788
loc_2_0796:
        mov    eax, dword ptr [ecx + 0x105]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 0x105]
        mov    dword ptr [ecx + 0x105], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_07B4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  090h
loc_2_07C0:
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
loc_2_07E4:
        test   edx, edx
        je     loc_2_07F3
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_2_07E4
        mov    dword ptr [ecx], edx
loc_2_07F3:
        cmp    dword ptr [ecx], -1
        jne    loc_2_0805
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_0805:
        mov    esi, dword ptr [ebp + 0x20]
        cmp    byte ptr [esi + 0x172], 0
        je     loc_2_081E
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_081E:
        mov    eax, dword ptr [0]            ; -> obj1:0x2020
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x75
        push   ecx
        call   sub_1_1B1E
        test   eax, eax
        je     loc_2_0855
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000002
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_0855:
        inc    dword ptr [esi + 0x10d]
        cmp    dword ptr [esi + 0x109], 0
        jne    loc_2_088F
        push   esi
        call   sub_2_1870
        test   ax, ax
        je     loc_2_088F
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000003
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_088F:
        mov    eax, dword ptr [esi + 0x109]
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x109], ecx
        mov    ecx, dword ptr [esi + 0xf9]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x14
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_1B10
        test   al, al
        jne    loc_2_08F5
        push   0
        dec    dword ptr [esi + 0x10d]
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000002
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x109], ecx
        mov    edx, dword ptr [esi + 0x5e]
        push   edx
        call   sub_1_1BF0
        add    esp, 0xc
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_08F5:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x101]
        mov    dword ptr [eax + 0x10], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, 0xee
        mov    byte ptr [eax + 0x20], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x71], ecx
        mov    dword ptr [esi + 0x17d], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi + 0x10d]
        dec    eax
        mov    dword ptr [esi + 0x10d], eax
        jne    loc_2_098C
        cmp    byte ptr [esi + 0x173], cl
        je     loc_2_0967
        lea    eax, [esi + 0x28d]
        cmp    byte ptr [eax], cl
        je     loc_2_097B
loc_2_0967:
        cmp    dword ptr [esi + 0x125], 0
        je     loc_2_098C
        lea    eax, [esi + 0x28d]
        cmp    byte ptr [eax], 0
        jne    loc_2_098C
loc_2_097B:
        push   0
        add    esi, 0x181
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1AFA
loc_2_098C:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_09A0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x109]
        test   edx, edx
        je     loc_2_09C3
loc_2_09B3:
        mov    esi, dword ptr [edx + 0x18]
        cmp    ecx, esi
        jae    loc_2_09BC
        mov    ecx, esi
loc_2_09BC:
        mov    edx, dword ptr [edx + 0xc]
        test   edx, edx
        jne    loc_2_09B3
loc_2_09C3:
        test   ecx, ecx
        jne    loc_2_09CC
        mov    ecx, 0xee
loc_2_09CC:
        pop    esi
        mov    dword ptr [eax + 0x17d], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_09E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x20], 0
        mov    esi, dword ptr [ebx + 8]
        je     loc_2_09FE
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_09FE:
        push   0
        inc    dword ptr [esi + 0x10d]
        inc    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [ebx + 0x14]
        mov    ecx, dword ptr [esi + 0xf9]
        push   eax
        push   ecx
        call   sub_2_1BE0
        mov    edi, eax
        test   edi, edi
        jne    loc_2_0A3E
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    eax, 2
        je     loc_2_0A36
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        mov    edi, 0x103
        jmp    loc_2_0A64
loc_2_0A36:
        sub    eax, 2
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_2_0A64
loc_2_0A3E:
        cmp    edi, 0x103
        jne    loc_2_0A50
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        jmp    loc_2_0A64
loc_2_0A50:
        cmp    edi, 0xc001000e
        jne    loc_2_0A61
        mov    byte ptr [ebx + 0x20], 1
        mov    edi, 0x103
loc_2_0A61:
        dec    dword ptr [ebx + 0x1c]
loc_2_0A64:
        cmp    dword ptr [ebx + 0x1c], 0
        jne    loc_2_0AD7
        mov    eax, dword ptr [esi + 0x109]
        cmp    ebx, eax
        jne    loc_2_0A7F
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x109], eax
        jmp    loc_2_0A92
loc_2_0A7F:
        cmp    dword ptr [eax + 0xc], ebx
        je     loc_2_0A8C
loc_2_0A84:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], ebx
        jne    loc_2_0A84
loc_2_0A8C:
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_2_0A92:
        mov    eax, dword ptr [ebx + 0x18]
        cmp    dword ptr [esi + 0x17d], eax
        jne    loc_2_0AA3
        push   esi
        call   sub_2_09A0
loc_2_0AA3:
        cmp    dword ptr [esi + 0x109], 0
        jne    loc_2_0ADC
        push   esi
        lea    eax, [esi + 0xb0]
        push   0                             ; -> loc_2_1620
        push   eax
        call   sub_1_1B12
        test   al, al
        jne    loc_2_0ADC
        push   0
        mov    eax, dword ptr [esi + 0x5e]
        push   0xaa000003
        push   eax
        call   sub_1_1BF0
        add    esp, 0xc
        jmp    loc_2_0ADC
loc_2_0AD7:
        mov    edi, 0x103
loc_2_0ADC:
        mov    eax, dword ptr [esi + 0x10d]
        dec    eax
        mov    dword ptr [esi + 0x10d], eax
        jne    loc_2_0B24
        cmp    byte ptr [esi + 0x173], 0
        je     loc_2_0AFF
        lea    eax, [esi + 0x28d]
        cmp    byte ptr [eax], 0
        je     loc_2_0B13
loc_2_0AFF:
        cmp    dword ptr [esi + 0x125], 0
        je     loc_2_0B24
        lea    eax, [esi + 0x28d]
        cmp    byte ptr [eax], 0
        jne    loc_2_0B24
loc_2_0B13:
        push   0
        add    esi, 0x181
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1AFA
loc_2_0B24:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [edi + 0x1c]
        inc    dword ptr [esi + 0x10d]
        cmp    byte ptr [edi + 0x20], 0
        je     loc_2_0B57
        mov    dword ptr [ebp - 4], 0xc0010002
        jmp    loc_2_0B99
loc_2_0B57:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        jne    loc_2_0B6E
        push   eax
        push   edi
        push   esi
        call   sub_2_1000
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_0B99
loc_2_0B6E:
        cmp    ecx, 1
        jne    loc_2_0B92
        cmp    byte ptr [esi + 0x172], 0
        je     loc_2_0B85
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_2_0B99
loc_2_0B85:
        push   eax
        push   edi
        push   esi
        call   sub_2_1050
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_0B99
loc_2_0B92:
        mov    dword ptr [ebp - 4], 0x10001
loc_2_0B99:
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 0x1c]
        jne    loc_2_0C09
        mov    eax, dword ptr [ebx + 0x109]
        cmp    edi, eax
        jne    loc_2_0BB6
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0x109], eax
        jmp    loc_2_0BC9
loc_2_0BB6:
        cmp    dword ptr [eax + 0xc], edi
        je     loc_2_0BC3
loc_2_0BBB:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_2_0BBB
loc_2_0BC3:
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ecx
loc_2_0BC9:
        mov    eax, dword ptr [ebx + 0x17d]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_2_0BDA
        push   ebx
        call   sub_2_09A0
loc_2_0BDA:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
        push   0x75
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 0x109], 0
        jne    loc_2_0C09
        push   ebx
        push   0                             ; -> loc_2_1620
        add    ebx, 0xb0
        push   ebx
        call   sub_1_1B12
loc_2_0C09:
        mov    eax, dword ptr [esi + 0x10d]
        dec    eax
        mov    dword ptr [esi + 0x10d], eax
        jne    loc_2_0C51
        cmp    byte ptr [esi + 0x173], 0
        je     loc_2_0C2C
        lea    eax, [esi + 0x28d]
        cmp    byte ptr [eax], 0
        je     loc_2_0C40
loc_2_0C2C:
        cmp    dword ptr [esi + 0x125], 0
        je     loc_2_0C51
        lea    eax, [esi + 0x28d]
        cmp    byte ptr [eax], 0
        jne    loc_2_0C51
loc_2_0C40:
        push   0
        add    esi, 0x181
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1AFA
loc_2_0C51:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_2_0C60:
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
        ja     loc_2_0C9D
        je     loc_2_0E5A
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_2_0CBE
        jmp    dword ptr [eax*4]             ; -> obj2:0x0FB4
loc_2_0C9D:
        cmp    eax, 0x1010102
        je     loc_2_0E79
        cmp    eax, 0x1010103
        je     loc_2_0E98
        cmp    eax, 0x1010104
        je     loc_2_0F5D
loc_2_0CBE:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_2_0F69
loc_2_0CCA:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_2_0CDF
        mov    esi, 0                        ; -> loc_1_20F0
        mov    edi, 0x5c
        jmp    loc_2_0F69
loc_2_0CDF:
        mov    esi, 0                        ; -> loc_1_2074
        mov    edi, 0x7c
        jmp    loc_2_0F69
loc_2_0CEE:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x14], 4
        cmp    byte ptr [ebx + 0x172], 0
        jne    loc_2_0D08
        mov    dword ptr [ebp - 0x14], 0
loc_2_0D08:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_2_0F69
loc_2_0D15:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_2_0F69
loc_2_0D22:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_2_0F69
loc_2_0D2E:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_2_0F69
loc_2_0D3A:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_2_0F69
loc_2_0D46:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0x4e]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xa8]
        imul   ecx, eax
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_2_0F69
loc_2_0D63:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0x20]
        xor    edx, edx
        mov    dx, word ptr [ebx + 0x4e]
        xor    eax, eax
        shl    ecx, 0xa
        mov    ax, word ptr [ebx + 0xa8]
        imul   edx, eax
        sub    ecx, edx
        sub    ecx, 0x100
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_2_0F69
loc_2_0D91:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x4c]
        jmp    loc_2_0F66
loc_2_0D9F:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x4c]
        jmp    loc_2_0F66
loc_2_0DAD:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x3e
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        call   sub_1_1F00
        and    dword ptr [ebp - 8], 0xffffff00
        jmp    loc_2_0F69
loc_2_0DD3:
        mov    esi, 0                        ; -> loc_1_21B4
        mov    edi, 0xd
        jmp    loc_2_0F69
loc_2_0DE2:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0DFC
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xf9]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_2_0F69
loc_2_0DFC:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x14]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_2_0F69
loc_2_0E0D:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_0E21
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x17d]
        jmp    loc_2_0F66
loc_2_0E21:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_2_0F69
loc_2_0E2F:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x30a
        jmp    loc_2_0F69
loc_2_0E42:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_2_0F69
loc_2_0E4E:
        mov    dword ptr [ebp - 8], 0xe
        jmp    loc_2_0F69
loc_2_0E5A:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x3e
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_1F00
        jmp    loc_2_0F69
loc_2_0E79:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x38
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_1F00
        jmp    loc_2_0F69
loc_2_0E98:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_2_0EF8
        mov    ecx, dword ptr [ebx + 0xf9]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    dword ptr [ebp + 0x1c], edx
        jae    loc_2_0ECD
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], edx
        jmp    loc_2_0F69
loc_2_0ECD:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x10]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0xc]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_2100
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ecx], eax
        je     loc_2_0F6F
        jmp    loc_2_0F69
loc_2_0EF8:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebx + 0xf9]
        mov    ecx, dword ptr [eax + 0x14]
        push   ecx
        push   edx
        call   sub_2_2000
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 0x10], eax
        lea    ecx, [edx*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_2_0F2B
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], ecx
        jmp    loc_2_0F69
loc_2_0F2B:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [eax + 0x14]
        push   ecx
        mov    edx, dword ptr [ebx + 0xf9]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_2040
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_2_0F69
loc_2_0F5D:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x111]
loc_2_0F66:
        mov    dword ptr [ebp - 8], eax
loc_2_0F69:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0FA8
loc_2_0F6F:
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_2_0FA8
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_2_0F98
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
loc_2_0F98:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   esi
        push   eax
        call   sub_1_1F00
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_2_0FA8:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        ; ---- dispatch table (19 entries) ----
        dd  loc_2_0CCA
        dd  loc_2_0CEE
        dd  loc_2_0D15
        dd  loc_2_0D15
        dd  loc_2_0D22
        dd  loc_2_0D2E
        dd  loc_2_0D3A
        dd  loc_2_0D46
        dd  loc_2_0D63
        dd  loc_2_0D91
        dd  loc_2_0D9F
        dd  loc_2_0DAD
        dd  loc_2_0DD3
        dd  loc_2_0DE2
        dd  loc_2_0E0D
        dd  loc_2_0E2F
        dd  loc_2_0E42
        dd  loc_2_0CBE
        dd  loc_2_0E4E
sub_2_1000:
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
loc_2_1020:
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x14]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_2_0C60
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1050:
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
        ja     loc_2_108D
        je     loc_2_119C
        cmp    eax, 0x1010e
        je     loc_2_109F
        cmp    eax, 0x1010f
        je     loc_2_10FA
        cmp    eax, 0x10112
        je     loc_2_1175
loc_2_108D:
        mov    ebx, 0xc00000bb
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_2_11D7
loc_2_109F:
        cmp    edi, 4
        je     loc_2_10B6
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_2_11D7
loc_2_10B6:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_1F00
        test   dword ptr [ebp - 8], 0xf050
        je     loc_2_10E3
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        mov    ebx, 0xc00000bb
        jmp    loc_2_11D7
loc_2_10E3:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_2_11F0
        jmp    loc_2_11D5
loc_2_10FA:
        cmp    edi, 4
        je     loc_2_1111
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_2_11D7
loc_2_1111:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_1F00
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_2_116E
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x17d]
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_2_1148
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x17d], eax
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_2_11D7
loc_2_1148:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    eax, dword ptr [ecx + 0x18]
        jne    loc_2_1166
        mov    eax, dword ptr [ecx + 0x18]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_2_1166
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x18], eax
        call   sub_2_09A0
        jmp    loc_2_11D7
loc_2_1166:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_2_11D7
loc_2_116E:
        mov    ebx, 0xc0010014
        jmp    loc_2_11D7
loc_2_1175:
        cmp    edi, 4
        je     loc_2_1189
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_2_11D7
loc_2_1189:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x71
        push   ecx
        call   sub_1_1F00
        jmp    loc_2_11D7
loc_2_119C:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_2_11BA
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_2_11D7
loc_2_11BA:
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
        call   sub_2_1230
loc_2_11D5:
        mov    ebx, eax
loc_2_11D7:
        test   ebx, ebx
        jne    loc_2_11E5
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_2_11E5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_2_11F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_2_1225
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [esi + 0x14]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [eax + 0xf9]
        push   ecx
        call   sub_2_1F80
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
loc_2_1225:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
sub_2_1230:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_2_1269
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
        mov    edx, dword ptr [ecx + 0xf9]
        push   edx
        call   sub_2_1CA0
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x14
loc_2_1269:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1280:
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
        call   sub_2_0C60
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0xc00000bb
        jne    loc_2_1360
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_2_12F0
        je     loc_2_132F
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_2_12FE
        jmp    dword ptr [eax*4]             ; -> obj2:0x1378
loc_2_12F0:
        cmp    eax, 0x1020102
        je     loc_2_1337
        cmp    eax, 0x1020103
        je     loc_2_133F
loc_2_12FE:
        mov    dword ptr [ebp - 8], 0xc00000bb
        jmp    loc_2_1348
loc_2_1307:
        mov    eax, dword ptr [ebx + 0x13a]
        jmp    loc_2_1345
loc_2_130F:
        mov    eax, dword ptr [ebx + 0x13e]
        jmp    loc_2_1345
loc_2_1317:
        mov    eax, dword ptr [ebx + 0x142]
        jmp    loc_2_1345
loc_2_131F:
        mov    eax, dword ptr [ebx + 0x162]
        jmp    loc_2_1345
loc_2_1327:
        mov    eax, dword ptr [ebx + 0x166]
        jmp    loc_2_1345
loc_2_132F:
        mov    eax, dword ptr [ebx + 0x15e]
        jmp    loc_2_1345
loc_2_1337:
        mov    eax, dword ptr [ebx + 0x146]
        jmp    loc_2_1345
loc_2_133F:
        mov    eax, dword ptr [ebx + 0x14a]
loc_2_1345:
        mov    dword ptr [ebp - 4], eax
loc_2_1348:
        cmp    edi, 4
        jb     loc_2_1360
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_1F00
        add    dword ptr [ebp - 0xc], 4
loc_2_1360:
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
        dd  loc_2_1307
        dd  loc_2_130F
        dd  loc_2_1317
        dd  loc_2_131F
        dd  loc_2_1327
        db  08Dh, 064h, 024h, 000h
loc_2_1390:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x10113
        ja     loc_2_13B7
        je     loc_2_13E8
        cmp    eax, 0x10101
        jb     loc_2_13E3
        cmp    eax, 0x10111
        jbe    loc_2_13E8
        jmp    loc_2_13E3
loc_2_13B7:
        cmp    eax, 0x1010104
        ja     loc_2_13D5
        cmp    eax, 0x1010101
        jae    loc_2_13E8
        cmp    eax, 0x20101
        jb     loc_2_13E3
        cmp    eax, 0x20105
        jbe    loc_2_13E8
        jmp    loc_2_13E3
loc_2_13D5:
        cmp    eax, 0x1020101
        jb     loc_2_13E3
        cmp    eax, 0x1020103
        jbe    loc_2_13E8
loc_2_13E3:
        mov    esi, 0xc00000bb
loc_2_13E8:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x10d]
        test   esi, esi
        jne    loc_2_13FE
        push   ecx
        push   edi
        call   sub_2_1280
        mov    esi, eax
loc_2_13FE:
        mov    eax, dword ptr [edi + 0x10d]
        dec    eax
        mov    dword ptr [edi + 0x10d], eax
        jne    loc_2_1446
        cmp    byte ptr [edi + 0x173], 0
        je     loc_2_1421
        lea    eax, [edi + 0x28d]
        cmp    byte ptr [eax], 0
        je     loc_2_1435
loc_2_1421:
        cmp    dword ptr [edi + 0x125], 0
        je     loc_2_1446
        lea    eax, [edi + 0x28d]
        cmp    byte ptr [eax], 0
        jne    loc_2_1446
loc_2_1435:
        push   0
        add    edi, 0x181
        push   edi
        mov    byte ptr [eax], 1
        call   sub_1_1AFA
loc_2_1446:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_2_1450:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1760
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x295]
        push   eax
        mov    edi, 0x300
        push   ecx
        call   sub_1_1B00
loc_2_1476:
        push   0x2710
        call   sub_1_1B18
        dec    edi
        jne    loc_2_1476
        mov    byte ptr [esi + 0x28f], 1
        cmp    dword ptr [0x42], esi         ; -> obj1:0x2066
        jne    loc_2_149F
        mov    eax, dword ptr [esi + 0x105]
        mov    dword ptr [0x42], eax         ; -> obj1:0x2066
        jmp    loc_2_14CD
loc_2_149F:
        mov    ecx, dword ptr [0x42]         ; -> obj1:0x2066
        cmp    dword ptr [ecx + 0x105], esi
        je     loc_2_14BB
loc_2_14AD:
        mov    ecx, dword ptr [ecx + 0x105]
        cmp    dword ptr [ecx + 0x105], esi
        jne    loc_2_14AD
loc_2_14BB:
        mov    eax, dword ptr [ecx + 0x105]
        mov    edx, dword ptr [eax + 0x105]
        mov    dword ptr [ecx + 0x105], edx
loc_2_14CD:
        lea    eax, [esi + 0xb0]
        push   eax
        call   sub_1_1B0C
        mov    eax, dword ptr [esi + 0xf9]
        push   eax
        call   sub_2_1AC0
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_1A50
        push   0
        push   0x2a2
        push   esi
        call   sub_1_1B24
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1510:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x2024
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_1A44
        push   0
        mov    ecx, dword ptr [4]            ; -> obj1:0x2028
        push   ecx
        call   sub_1_1A38
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1540:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [eax + 0x20], 0
        je     loc_2_1559
        mov    eax, 0xc0010002
        pop    ebp
        ret    4
loc_2_1559:
        cmp    byte ptr [ecx + 0x173], 0
        je     loc_2_1568
        xor    eax, eax
        pop    ebp
        ret    4
loc_2_1568:
        inc    dword ptr [eax + 0x1c]
        inc    dword ptr [ecx + 0x10d]
        mov    byte ptr [ecx + 0x173], 1
        mov    dword ptr [ecx + 0x175], eax
        mov    eax, dword ptr [ecx + 0x10d]
        dec    eax
        mov    dword ptr [ecx + 0x10d], eax
        jne    loc_2_15C6
        cmp    byte ptr [ecx + 0x173], 0
        je     loc_2_15A1
        lea    eax, [ecx + 0x28d]
        cmp    byte ptr [eax], 0
        je     loc_2_15B5
loc_2_15A1:
        cmp    dword ptr [ecx + 0x125], 0
        je     loc_2_15C6
        lea    eax, [ecx + 0x28d]
        cmp    byte ptr [eax], 0
        jne    loc_2_15C6
loc_2_15B5:
        push   0
        add    ecx, 0x181
        push   ecx
        mov    byte ptr [eax], 1
        call   sub_1_1AFA
loc_2_15C6:
        mov    eax, 0x103
        pop    ebp
        ret    4
        db  090h
loc_2_15D0:
        xor    eax, eax
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_15E0:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_2_1950
        cmp    ax, 1
        pop    ebp
        mov    eax, 0
        adc    eax, -1
        and    eax, 0xc0000001
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1610:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 0x1c]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_1620:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_2_1900
        cmp    ax, 1
        pop    ebp
        sbb    cl, cl
        neg    cl
        mov    al, cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_1640:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   0xfa0
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x2e]
        mov    bl, byte ptr [eax]
        mov    cl, bl
        and    bl, 0x7f
        or     cl, 0x80
        mov    byte ptr [eax], cl
        call   sub_1_1B18
        mov    eax, dword ptr [esi + 0x2e]
        push   0xfa0
        mov    byte ptr [eax], bl
        call   sub_1_1B18
        mov    eax, dword ptr [esi + 0x2e]
        xor    ecx, ecx
        or     byte ptr [eax], 2
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x10], 1
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x1a], cl
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x1b], cl
        mov    eax, dword ptr [esi + 0x2e]
        test   byte ptr [eax + 0x17], 0x80
        jne    loc_2_169E
        push   0x186a00
        call   sub_1_1B18
loc_2_169E:
        mov    eax, dword ptr [esi + 0x2e]
        push   esi
        mov    byte ptr [eax + 0x1e], 0x49
        call   sub_2_1950
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x1d], 4
        mov    cl, byte ptr [esi + 0xf2]
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x11], cl
        mov    eax, dword ptr [esi + 0x2e]
        mov    dl, byte ptr [esi + 0xf3]
        mov    byte ptr [eax + 0x12], dl
        mov    eax, dword ptr [esi + 0x2e]
        mov    cl, byte ptr [esi + 0xf2]
        mov    byte ptr [eax + 0x13], cl
        mov    eax, dword ptr [esi + 0x2e]
        mov    ecx, 0x3f
        mov    byte ptr [eax + 0x17], 0xff
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [esi + 0xae], cl
        mov    byte ptr [eax + 0x1f], cl
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x10], 0x41
        xor    eax, eax
loc_2_16F7:
        mov    dl, byte ptr [esi + eax + 0x38]
        mov    ecx, dword ptr [esi + 0x2e]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax + 0x10], dl
        jb     loc_2_16F7
        mov    cl, byte ptr [esi + 0xf2]
        mov    eax, dword ptr [esi + 0x2e]
        inc    cl
        mov    byte ptr [eax + 0x17], cl
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0x32]
        shl    eax, 0xa
        sub    eax, 0x100
        je     loc_2_1743
loc_2_1728:
        mov    eax, dword ptr [esi + 0x2a]
        inc    ecx
        mov    byte ptr [eax + ecx - 1], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 0x32]
        shl    eax, 0xa
        sub    eax, 0x100
        cmp    eax, ecx
        ja     loc_2_1728
loc_2_1743:
        mov    eax, dword ptr [esi + 0x2e]
        pop    esi
        pop    ebx
        pop    ebp
        mov    byte ptr [eax + 0x10], 1
        mov    al, 1
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1760:
        xor    ax, ax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1770:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 0x2e]
        or     byte ptr [eax], 2
        mov    eax, 0xffffffff
        mov    word ptr [esi + 0xcd], ax
        mov    byte ptr [esi + 0xcf], cl
        mov    word ptr [esi + 0xc9], cx
        mov    byte ptr [esi + 0xf1], cl
        mov    word ptr [esi + 0xcb], ax
        cmp    word ptr [esi + 0x4e], cx
        jbe    loc_2_17C8
        lea    eax, [esi + 0xd4]
        xor    edx, edx
loc_2_17B5:
        mov    dword ptr [eax], edx
        add    eax, 4
        inc    ecx
        xor    edi, edi
        mov    dword ptr [eax - 8], edx
        mov    di, word ptr [esi + 0x4e]
        cmp    edi, ecx
        ja     loc_2_17B5
loc_2_17C8:
        xor    eax, eax
        xor    ecx, ecx
        mov    ax, word ptr [esi + 0xa8]
        xor    ebx, ebx
        mov    byte ptr [esi + 0xc8], 0
        mov    cx, word ptr [esi + 0x4e]
        imul   eax, ecx
        mov    bx, word ptr [esi + 0x20]
        add    eax, dword ptr [esi + 0x2a]
        mov    edi, ebx
        mov    ecx, dword ptr [esi + 8]
        shl    edi, 0xa
        mov    dword ptr [esi + 0xd8], eax
        sub    eax, ecx
        lea    edx, [ecx + edi]
        xor    ecx, ecx
        and    ebx, 0x3fffff
        mov    cx, word ptr [esi + 0x4c]
        mov    dword ptr [esi + 0xdc], edx
        cdq    
        idiv   ecx
        mov    byte ptr [esi + 0xf2], al
        cmp    ebx, 0x40
        jne    loc_2_1826
        mov    byte ptr [esi + 0xf3], 0xff
        jmp    loc_2_1837
loc_2_1826:
        mov    eax, edi
        cdq    
        and    edx, 0xff
        add    eax, edx
        mov    byte ptr [esi + 0xf3], ah
loc_2_1837:
        push   esi
        call   sub_2_1640
        test   al, al
        jne    loc_2_1853
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0xac], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_1853:
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0xac], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1870:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xac], 1
        jne    loc_2_1888
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_2_1888:
        mov    eax, dword ptr [esi + 0x2e]
        or     byte ptr [eax], 2
        cmp    byte ptr [esi + 0xac], 3
        jne    loc_2_18A2
        push   esi
        call   sub_2_1770
        test   ax, ax
        jne    loc_2_18F4
loc_2_18A2:
        cmp    byte ptr [esi + 0xac], 2
        je     loc_2_18B4
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_2_18B4:
        mov    eax, dword ptr [esi + 0x2e]
        mov    byte ptr [eax + 0x10], 2
        test   byte ptr [esi + 0xa4], 0x10
        mov    eax, dword ptr [esi + 0x2e]
        je     loc_2_18CD
        mov    byte ptr [eax + 0x1d], 1
        jmp    loc_2_18D1
loc_2_18CD:
        mov    byte ptr [eax + 0x1d], 0
loc_2_18D1:
        mov    byte ptr [esi + 0xac], 1
        mov    eax, dword ptr [esi + 0x2e]
        mov    cl, byte ptr [eax + 2]
        add    eax, 2
        cmp    cl, 0x20
        jne    loc_2_18EB
        or     cl, 0x80
        mov    byte ptr [eax], cl
loc_2_18EB:
        push   esi
        call   sub_1_1020
        xor    ax, ax
loc_2_18F4:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1900:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xac], 1
        je     loc_2_1919
        mov    ax, 0xa
        pop    esi
        pop    ebp
        ret    4
loc_2_1919:
        push   esi
        call   sub_1_1060
        mov    eax, dword ptr [esi + 0x2e]
        push   0x186a00
        mov    byte ptr [eax + 0x10], 1
        mov    eax, dword ptr [esi + 0x2e]
        and    byte ptr [eax + 2], 0x7f
        call   sub_1_1B18
        mov    byte ptr [esi + 0xac], 3
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1950:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [edx + 0x2e]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [edx + 0x2e]
        mov    byte ptr [eax + 0x10], cl
        mov    eax, dword ptr [edx + 0xf9]
        test   eax, eax
        je     loc_2_1974
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_2_1976
loc_2_1974:
        xor    eax, eax
loc_2_1976:
        test   al, 8
        je     loc_2_197C
        mov    cl, 4
loc_2_197C:
        test   al, 6
        je     loc_2_1983
        or     cl, 8
loc_2_1983:
        test   al, 0x20
        je     loc_2_198A
        or     cl, 0x1c
loc_2_198A:
        mov    eax, dword ptr [edx + 0x2e]
        pop    ebp
        mov    byte ptr [eax + 0x1c], cl
        xor    ax, ax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_19A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0]            ; -> obj1:0x21E0
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_1B1E
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_19D4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_19D4:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_19EE
        mov    esi, 2
loc_2_19EE:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_1B1E
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_1A22
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1AC0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_1A22:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_1B1E
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
loc_2_1A3E:
        je     loc_2_1A51
        push   eax
        call   sub_2_1AC0
        sub    al, al
        pop    edi
        pop    esi
loc_2_1A4A:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_1A51:
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
sub_2_1AC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_1AE0
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_1B24
loc_2_1AE0:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_1AF6
        push   0
        mov    ecx, dword ptr [esi + 0x20]
loc_2_1AEC:
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_1B24
loc_2_1AF6:
        push   0
        push   0x32
loc_2_1AFA:
        push   esi
        call   sub_1_1B24
loc_2_1B00:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
loc_2_1B1E:
        jne    loc_2_1B29
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_1B29:
        mov    eax, dword ptr [0]            ; -> obj1:0x21E0
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_1B1E
        or     eax, eax
        je     loc_2_1B49
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_1B49:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_1B50:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_1B63
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_1B50
        jmp    loc_2_1B72
loc_2_1B63:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_1B72:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_1B88
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_1B88:
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
sub_2_1BE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
loc_2_1BEA:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_1F80
        mov    edi, eax
        or     edi, edi
        je     loc_2_1C09
        cmp    edi, 0x103
        jne    loc_2_1C20
loc_2_1C09:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   esi
        push   ebx
        call   sub_2_1CA0
        or     eax, eax
        je     loc_2_1C20
        mov    edi, eax
loc_2_1C20:
        or     edi, edi
        je     loc_2_1C2C
        cmp    edi, 0x103
        jne    loc_2_1C8B
loc_2_1C2C:
        mov    eax, dword ptr [esi + 0xc]
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        jne    loc_2_1C86
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_1C42
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_1C42:
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_1C51
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_1C57
loc_2_1C51:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x10], eax
loc_2_1C57:
        cmp    byte ptr [esi + 0x18], 0
        je     loc_2_1C66
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1A80
loc_2_1C66:
        mov    eax, 1
        mov    cl, byte ptr [esi + 0x19]
        shl    eax, cl
        or     dword ptr [ebx + 0x28], eax
        push   0
        push   0x1a
        push   esi
        call   sub_1_1B24
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_1C86:
        mov    edi, 0xc001000e
loc_2_1C8B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1CA0:
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
        call   sub_1_1F20
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_1F20
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_1D24
        sub    eax, eax
        nop    
loc_2_1D00:
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
        ja     loc_2_1D00
loc_2_1D24:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_1D8F
loc_2_1D31:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_1D84
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
        call   sub_1_1F20
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
        call   sub_1_1F20
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_1D87
loc_2_1D84:
        inc    dword ptr [ebp - 4]
loc_2_1D87:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_1D31
loc_2_1D8F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_1E7A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_1DA3:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_2160
        or     al, al
        je     loc_2_1DDA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_1E68
loc_2_1DDA:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_1EB4
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
        call   sub_1_1F20
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
        call   sub_1_1F20
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
loc_2_1E68:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_1DA3
loc_2_1E7A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        cmp    dword ptr [ebp - 0xc], edx
        je     loc_2_1F05
        mov    edi, dword ptr [ebp - 0x20]
        sub    ecx, ecx
        nop    
loc_2_1E90:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_1F0B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_1EF2
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    loc_2_1EF2
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_1EF9
loc_2_1EB4:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1F20
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_1F20
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_1EF2:
        mov    dword ptr [ebp - 8], 1
loc_2_1EF9:
        add    edi, 6
        add    ecx, 6
        inc    edx
loc_2_1F00:
        cmp    dword ptr [ebp - 0xc], edx
        ja     loc_2_1E90
loc_2_1F05:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_1F70
loc_2_1F0B:
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
loc_2_1F20:
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   dword ptr [esi + 0x14]
        mov    edi, eax
        or     edi, edi
        je     loc_2_1F72
        cmp    edi, 0x103
        je     loc_2_1F72
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1F20
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1F20
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_1F70:
        sub    edi, edi
loc_2_1F72:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_1F80:
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
        je     loc_2_1FBD
        lea    esp, [esp]
loc_2_1FB0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_1FB0
loc_2_1FBD:
        mov    ecx, dword ptr [edi + 0xc]
        cmp    esi, ecx
        je     loc_2_1FF1
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
        je     loc_2_1FF3
        cmp    eax, 0x103
        je     loc_2_1FF3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [edi + 0xc], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1FF1:
        sub    eax, eax
loc_2_1FF3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_2000:
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
        je     loc_2_2032
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_2027:
        test   dword ptr [esi], edx
        je     loc_2_202C
        inc    eax
loc_2_202C:
        add    esi, 4
        dec    edi
        jne    loc_2_2027
loc_2_2032:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2040:
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
        jbe    loc_2_20BC
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_2080:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_20AA
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_20D6
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_1F20
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_20AA:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_2080
loc_2_20BC:
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
loc_2_20D6:
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
sub_2_2100:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_2130
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2130:
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
        call   sub_1_1F20
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2160:
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
        je     loc_2_21F9
        dec    esi
        cmp    esi, edx
        jb     loc_2_21F9
        nop    
loc_2_2180:
        cmp    edx, ecx
        jb     loc_2_21F9
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_21A3
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_21D7
loc_2_21A3:
        jbe    loc_2_21B0
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_21D7
        db  08Bh, 0FFh
loc_2_21B0:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_21C7
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_21D7
loc_2_21C7:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_21D7
        mov    dword ptr [ebp - 4], 0
loc_2_21D7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_2209
        jle    loc_2_21E8
        or     edx, edx
        je     loc_2_21F9
        lea    esi, [edx - 1]
        jmp    loc_2_21EB
loc_2_21E8:
        lea    ecx, [edx + 1]
loc_2_21EB:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_2180
loc_2_21F9:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_2209:
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

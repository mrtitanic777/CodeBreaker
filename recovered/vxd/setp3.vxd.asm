; SETP3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 8789 instructions recovered by recursive descent

Declare_Virtual_Device SETP3, 3, 10, SETP3_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x4ac8  R-X
sub_1_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x21
        push   0
        call   sub_1_4AB0
        lea    eax, [ebp - 1]
        push   eax
        push   0
        call   sub_1_4AB8
        mov    al, byte ptr [ebp - 1]
        and    al, 0x21
        cmp    al, 0x21
        je     loc_1_0033
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0033:
        push   0x48
        xor    ebx, ebx
        push   0xe
        call   sub_1_4AB0
        push   ebx
        push   0xb
        call   sub_1_4AB0
        push   ebx
        push   0xa
        call   sub_1_4AB0
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0x15b]
        push   eax
        push   0xc
        call   sub_1_4AB0
        push   2
        push   0xd
        call   sub_1_4AB0
        mov    al, byte ptr [esi + 0xfa]
        push   eax
        push   3
        call   sub_1_4AB0
        mov    al, byte ptr [esi + 0xfa]
        push   eax
        push   1
        call   sub_1_4AB0
        mov    al, byte ptr [esi + 0xfb]
        push   eax
        push   2
        call   sub_1_4AB0
        mov    al, byte ptr [esi + 0xfa]
        push   0xff
        inc    al
        push   7
        mov    byte ptr [esi + 0xfd], al
        mov    byte ptr [esi + 0xfc], al
        mov    byte ptr [esi + 0x118], bl
        call   sub_1_4AB0
        mov    cl, byte ptr [esi + 0x15c]
        push   ecx
        push   0xf
        call   sub_1_4AB0
        push   0x61
        push   ebx
        call   sub_1_4AB0
loc_1_00D9:
        mov    al, byte ptr [esi + ebx + 0x104]
        lea    ecx, [ebx + 1]
        push   eax
        inc    ebx
        push   ecx
        call   sub_1_4AB0
        cmp    ebx, 6
        jb     loc_1_00D9
        mov    eax, dword ptr [esi + 0x14b]
        xor    ebx, ebx
        mov    edi, dword ptr [eax + 0xc]
        and    edi, 4
loc_1_00FF:
        mov    eax, 0xff
        test   edi, edi
        jne    loc_1_0111
        xor    eax, eax
        mov    al, byte ptr [esi + ebx + 0x14f]
loc_1_0111:
        push   eax
        lea    eax, [ebx + 8]
        inc    ebx
        push   eax
        call   sub_1_4AB0
        cmp    ebx, 8
        jb     loc_1_00FF
        mov    al, byte ptr [esi + 0xfd]
        push   eax
        push   7
        call   sub_1_4AB0
        push   0x21
        push   0
        call   sub_1_4AB0
        push   0x22
        push   0
        call   sub_1_4AB0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_0150:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   eax
        call   sub_1_0670
        push   0
        xor    esi, esi
        push   0xb
        mov    ebx, 0x80
        call   sub_1_4AB0
        push   0
        push   0xa
        call   sub_1_4AB0
loc_1_017E:
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4AB8
        test   byte ptr [ebp - 1], bl
        jne    loc_1_01A4
        mov    edi, 0xa
loc_1_0194:
        push   0x64
        call   sub_1_4250
        dec    edi
        jne    loc_1_0194
        inc    esi
        cmp    esi, 4
        jb     loc_1_017E
loc_1_01A4:
        push   2
        push   0xd
        call   sub_1_4AB0
        push   0x22
        push   0
        call   sub_1_4AB0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_01D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    edi, 0x14
        call   sub_1_0150
loc_1_01E3:
        push   0x64
        call   sub_1_4250
        dec    edi
        jne    loc_1_01E3
        call   sub_1_1540
        call   sub_1_0980
        push   esi
        call   0
        test   al, al
        jne    loc_1_0220
        push   1
        mov    eax, dword ptr [esi + 4]
        push   1
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_4328
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0220:
        call   sub_1_1520
        push   0
        push   0xd
        call   sub_1_4AB0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0240:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        lea    eax, [ebp - 0x14]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        push   0
        push   0
        push   edx
        call   sub_1_4280
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_026F
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_026F:
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [ebp - 0x10]
        mov    edx, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 0x4f]
        shl    esi, 8
        add    esi, dword ptr [eax + 0xed]
        lea    eax, [ebp - 0xc]
        push   eax
        push   ecx
        push   edx
        call   sub_1_42A4
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_02BF
        xor    edi, edi
loc_1_0297:
        cmp    dword ptr [ebp - 0xc], edi
        jne    loc_1_02BF
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_42C8
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 0x10]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_42A4
        cmp    dword ptr [ebp - 8], edi
        jne    loc_1_0297
loc_1_02BF:
        pushf  
        cli    
        push   0
        push   0
        call   sub_1_4AB0
        push   0xff
        push   0xa
        call   sub_1_4AB0
        push   0
        push   0xb
        call   sub_1_4AB0
        push   0xa
        push   0
        call   sub_1_4AB0
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        push   0x40
        push   7
        call   sub_1_4AB0
        push   esi
        push   8
        shr    esi, 8
        call   sub_1_4AB0
        push   esi
        push   9
        call   sub_1_4AB0
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   0xa
        call   sub_1_4AB0
        mov    eax, dword ptr [ebp - 0x14]
        shr    eax, 8
        xor    esi, esi
        push   eax
        push   0xb
        call   sub_1_4AB0
        push   0x12
        push   0
        call   sub_1_4AB0
loc_1_0339:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        push   ecx
        call   sub_1_4AC4
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 8], esi
        je     loc_1_039C
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42A4
        cmp    dword ptr [ebp - 8], esi
        je     loc_1_03A1
loc_1_036F:
        cmp    dword ptr [ebp - 0xc], esi
        jne    loc_1_039C
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 8], esi
        je     loc_1_039C
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42A4
        cmp    dword ptr [ebp - 8], esi
        jne    loc_1_036F
loc_1_039C:
        cmp    dword ptr [ebp - 8], esi
        jne    loc_1_0339
loc_1_03A1:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, 0xffff
        mov    ebx, 0x40
        mov    dword ptr [eax], ecx
loc_1_03B3:
        test   esi, esi
        je     loc_1_03C9
        dec    esi
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4AB8
        test   byte ptr [ebp - 1], bl
        je     loc_1_03B3
loc_1_03C9:
        popf   
        pop    edi
        cmp    esi, 1
        sbb    al, al
        pop    esi
        inc    al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_1_03E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        test   esi, esi
        jne    loc_1_03FB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_03FB:
        pushf  
        cli    
        push   0
        push   0
        call   sub_1_4AB0
        push   0xff
        push   0xa
        call   sub_1_4AB0
        push   0
        push   0xb
        call   sub_1_4AB0
        push   0xa
        push   0
        call   sub_1_4AB0
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        push   0x40
        push   7
        call   sub_1_4AB0
        mov    edi, dword ptr [ebp + 8]
        push   edi
        push   8
        shr    edi, 8
        call   sub_1_4AB0
        push   edi
        push   9
        call   sub_1_4AB0
        push   esi
        push   0xa
        call   sub_1_4AB0
        mov    eax, esi
        shr    eax, 8
        push   eax
        push   0xb
        call   sub_1_4AB0
        push   0x12
        push   0
        call   sub_1_4AB0
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    esi, dword ptr [0]            ; -> sub_1_4AC4
        mov    ebx, 0x40
        call   esi
        mov    si, 0xffff
loc_1_0488:
        test   si, si
        je     loc_1_04A0
        dec    si
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4AB8
        test   byte ptr [ebp - 1], bl
        je     loc_1_0488
loc_1_04A0:
        popf   
        cmp    si, 1
        pop    edi
        sbb    al, al
        pop    esi
        inc    al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        test   esi, esi
        jne    loc_1_04DB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_04DB:
        pushf  
        cli    
        push   0
        push   0
        call   sub_1_4AB0
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        push   8
        shr    edi, 8
        call   sub_1_4AB0
        push   edi
        push   9
        call   sub_1_4AB0
        push   esi
        push   0xa
        call   sub_1_4AB0
        mov    eax, esi
        shr    eax, 8
        push   eax
        push   0xb
        call   sub_1_4AB0
        push   0xa
        push   0
        mov    ebx, 0x40
        call   sub_1_4AB0
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    esi, 0xffff
        call   sub_1_4ABC
loc_1_0536:
        test   esi, esi
        je     loc_1_054C
        dec    esi
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4AB8
        test   byte ptr [ebp - 1], bl
        je     loc_1_0536
loc_1_054C:
        popf   
        pop    edi
        cmp    esi, 1
        sbb    al, al
        pop    esi
        inc    al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_0560:
        push   ebp
        mov    eax, 0xffffffff
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_05A8
loc_1_0572:
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [esi + ecx]
        mov    ecx, 8
loc_1_057D:
        mov    edi, eax
        mov    bl, dl
        shr    dl, 1
        and    edi, 0x80000000
        shr    edi, 0x1f
        and    ebx, 1
        xor    edi, ebx
        add    eax, eax
        test   edi, edi
        je     loc_1_059F
        xor    eax, 0x4c11db6
        or     eax, edi
loc_1_059F:
        dec    ecx
        jne    loc_1_057D
        inc    esi
        cmp    dword ptr [ebp + 0xc], esi
        ja     loc_1_0572
loc_1_05A8:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_05B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   6
        push   eax
        call   sub_1_0560
        shr    eax, 0x1a
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, eax
        and    al, 7
        shr    ebx, 3
        mov    cl, al
        mov    byte ptr [edx], bl
        mov    dl, 1
        shl    dl, cl
        mov    ecx, dword ptr [ebp + 0x10]
        pop    ebx
        pop    ebp
        mov    byte ptr [ecx], dl
        ret    0xc
        db  090h
sub_1_05E0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, 8
        sub    esp, 0xc
        mov    dword ptr [ebp - 8], edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_1_05F5:
        mov    ecx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 8]
        mov    byte ptr [ecx + esi + 0x14f], dl
        cmp    dword ptr [ebp - 8], eax
        jb     loc_1_05F5
        push   0                             ; -> obj1:0x43F0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi + 0x14b]
        push   0x60
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_2_1E10
loc_1_0622:
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_1_065C
        xor    edi, edi
loc_1_062A:
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 2]
        push   eax
        dec    dword ptr [ebp - 8]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        lea    eax, [edx + edx*2]
        lea    ecx, [eax*2]                  ; -> obj1:0x43F0
        push   ecx
        call   sub_1_05B0
        xor    edx, edx
        mov    cl, byte ptr [ebp - 1]
        mov    dl, byte ptr [ebp - 2]
        or     byte ptr [edx + esi + 0x14f], cl
        cmp    dword ptr [ebp - 8], edi
        ja     loc_1_062A
loc_1_065C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0670:
        push   0x21
        push   0
        call   sub_1_4AB0
        xor    al, al
        ret    4
        db  090h
sub_1_0680:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [eax + 0x57]
        push   esi
        mov    esi, dword ptr [eax + ecx*4 + 0xbd]
        mov    al, byte ptr [eax + 0xf9]
        add    al, cl
        push   eax
        push   4
        call   sub_1_4AB0
        cmp    esi, 0x3c
        jae    loc_1_06B0
        mov    esi, 0x3c
loc_1_06B0:
        mov    eax, esi
        shr    eax, 8
        push   eax
        push   6
        call   sub_1_4AB0
        push   esi
        push   5
        call   sub_1_4AB0
        lea    ecx, [ebp - 1]
        push   ecx
        push   0
        call   sub_1_4AB8
        push   0x26
        push   0
        call   sub_1_4AB0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_06F0:
        push   ebp
        mov    ebp, esp
        push   0x62
        push   0
        call   sub_1_4AB0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xfd
        push   eax
        push   7
        call   sub_1_4AB8
        push   0x22
        push   0
        call   sub_1_4AB0
        xor    al, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0720:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 0xfe
        push   eax
        push   4
        call   sub_1_4AB8
        xor    al, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0740:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 0xfc]
        cmp    byte ptr [eax + 0xfa], cl
        jne    loc_1_076A
        mov    al, byte ptr [eax + 0xfb]
        dec    al
        push   eax
        push   3
        call   sub_1_4AB0
        pop    ebp
        ret    4
loc_1_076A:
        dec    cl
        push   ecx
        push   3
        call   sub_1_4AB0
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0780:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    al, byte ptr [eax + 0x15b]
        push   eax
        push   0xc
        call   sub_1_4AB0
        xor    al, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_07A0:
        push   esi
        push   0x62
        push   0
        xor    esi, esi
        call   sub_1_4AB0
        mov    eax, dword ptr [0]            ; -> sub_1_4AB0
loc_1_07B2:
        lea    ecx, [esi + 8]
        push   0xff
        push   ecx
        inc    esi
        call   eax
        mov    eax, dword ptr [0]            ; -> sub_1_4AB0
        cmp    esi, 8
        jb     loc_1_07B2
        push   0x22
        push   0
        call   eax
        xor    al, al
        pop    esi
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0x62
        push   0
        xor    edi, edi
        call   sub_1_4AB0
        mov    esi, dword ptr [ebp + 8]
loc_1_07F4:
        mov    al, byte ptr [esi + edi + 0x14f]
        lea    ecx, [edi + 8]
        push   eax
        inc    edi
        push   ecx
        call   sub_1_4AB0
        cmp    edi, 8
        jb     loc_1_07F4
        push   0x22
        push   0
        call   sub_1_4AB0
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0820:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        test   byte ptr [eax + 0x103], 4
        je     loc_1_0836
        push   eax
        call   sub_1_0840
loc_1_0836:
        xor    al, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0840:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        push   0xd
        call   sub_1_4AB8
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        lea    ecx, [ebp - 1]
        push   ecx
        add    dword ptr [esi + 0x132], eax
        push   0xe
        call   sub_1_4AB8
        xor    ecx, ecx
        lea    edx, [ebp - 1]
        mov    cl, byte ptr [ebp - 1]
        push   edx
        push   0xf
        add    dword ptr [esi + 0x136], ecx
        call   sub_1_4AB8
        xor    ecx, ecx
        xor    al, al
        mov    cl, byte ptr [ebp - 1]
        add    dword ptr [esi + 0x13a], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_08A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x14
        push   esi
        call   sub_1_0670
loc_1_08B6:
        push   0x64
        call   sub_1_4250
        dec    edi
        jne    loc_1_08B6
        test   byte ptr [esi + 0x103], 0xa
        jne    loc_1_08E8
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4AB8
        test   byte ptr [ebp - 1], 0xa
        jne    loc_1_08E8
        mov    al, byte ptr [esi + 0x5b]
        mov    byte ptr [esi + 0x5c], al
        mov    byte ptr [esi + 0x1b1], 0
loc_1_08E8:
        mov    byte ptr [esi + 0x5b], 0
        mov    dword ptr [esi + 0x147], 0
        push   0
        push   0xb
        call   sub_1_4AB0
        push   0
        push   0xa
        call   sub_1_4AB0
        push   2
        push   0xd
        call   sub_1_4AB0
        push   0x22
        push   0
        call   sub_1_4AB0
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0930:
        mov    cx, word ptr [0]              ; -> loc_1_4A1C
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [0]              ; -> obj1:0x4664
        sub    esp, 0xc
        mov    byte ptr [ebp - 3], al
        mov    word ptr [ebp - 0xa], cx
        mov    dx, word ptr [ebp - 0xa]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    byte ptr [ebp - 2], 0x1d
        add    cx, 2
        mov    word ptr [ebp - 8], cx
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    byte ptr [ebp - 1], 0x15
        mov    word ptr [ebp - 6], cx
        mov    dx, word ptr [ebp - 6]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0980:
        mov    cx, word ptr [0]              ; -> loc_1_4A1C
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    byte ptr [ebp - 6], 0xaa
        mov    word ptr [ebp - 0x12], cx
        mov    dx, word ptr [ebp - 0x12]
        mov    al, byte ptr [ebp - 6]
        out    dx, al
        mov    byte ptr [ebp - 5], 0x1d
        add    cx, 2
        mov    word ptr [ebp - 0x10], cx
        mov    dx, word ptr [ebp - 0x10]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    byte ptr [ebp - 4], 0x15
        mov    word ptr [ebp - 0xe], cx
        mov    dx, word ptr [ebp - 0xe]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        push   0x14
        call   sub_1_4250
        mov    byte ptr [ebp - 3], 0
        mov    cx, word ptr [0]              ; -> loc_1_4A1C
        mov    word ptr [ebp - 0xc], cx
        mov    dx, word ptr [ebp - 0xc]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    byte ptr [ebp - 2], 0x1d
        add    cx, 2
        mov    word ptr [ebp - 0xa], cx
        mov    dx, word ptr [ebp - 0xa]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    byte ptr [ebp - 1], 0x15
        mov    word ptr [ebp - 8], cx
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        push   0x14
        call   sub_1_4250
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_0A10:
        mov    al, byte ptr [0]              ; -> loc_1_4660
        mov    cl, byte ptr [0]              ; -> obj1:0x4664
        or     al, 8
        and    cl, 0xfb
        or     al, 0x10
        or     cl, byte ptr [0]              ; -> loc_1_466C
        or     cl, byte ptr [0]              ; -> loc_1_4670
        mov    byte ptr [0], al              ; -> loc_1_4660
        mov    byte ptr [0], 0               ; -> loc_1_4674
        mov    byte ptr [0], cl              ; -> obj1:0x4664
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A50:
        mov    al, byte ptr [0]              ; -> loc_1_4660
        mov    cl, byte ptr [0]              ; -> obj1:0x4664
        or     al, 8
        or     cl, 4
        or     al, 0x10
        or     cl, byte ptr [0]              ; -> loc_1_466C
        or     cl, byte ptr [0]              ; -> loc_1_4670
        mov    byte ptr [0], al              ; -> loc_1_4660
        mov    byte ptr [0], 0               ; -> loc_1_4674
        mov    byte ptr [0], cl              ; -> obj1:0x4664
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A90:
        mov    al, byte ptr [0]              ; -> loc_1_4660
        mov    cl, byte ptr [0]              ; -> obj1:0x4664
        and    al, 0xf7
        and    cl, 0xfb
        or     al, 0x10
        or     cl, byte ptr [0]              ; -> loc_1_466C
        or     cl, byte ptr [0]              ; -> loc_1_4670
        mov    byte ptr [0], al              ; -> loc_1_4660
        mov    byte ptr [0], 0               ; -> loc_1_4674
        mov    byte ptr [0], cl              ; -> obj1:0x4664
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0AD0:
        xor    al, al
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0AE0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        or     al, 0x20
        jmp    loc_1_0AF8
loc_1_0AF8:
        jmp    loc_1_0AFD
loc_1_0AFD:
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        jmp    loc_1_0B09
loc_1_0B09:
        jmp    loc_1_0B0E
loc_1_0B0E:
        out    dx, al
        mov    ax, 0x1515
        jmp    loc_1_0B18
loc_1_0B18:
        jmp    loc_1_0B1D
loc_1_0B1D:
        out    dx, al
        mov    al, 0x17
        jmp    loc_1_0B25
loc_1_0B25:
        jmp    loc_1_0B2A
loc_1_0B2A:
        out    dx, al
        dec    dx
        nop    
        nop    
        nop    
        push   ecx
        mov    ecx, 0x1000
        jmp    loc_1_0B3B
loc_1_0B3B:
        jmp    loc_1_0B40
loc_1_0B40:
        in     al, dx
        jmp    loc_1_0B46
loc_1_0B46:
        jmp    loc_1_0B4B
loc_1_0B4B:
        test   al, 0x40
        jne    loc_1_0B7E
        in     al, dx
        jmp    loc_1_0B55
loc_1_0B55:
        jmp    loc_1_0B5A
loc_1_0B5A:
        test   al, 0x40
        jne    loc_1_0B7E
        in     al, dx
        jmp    loc_1_0B64
loc_1_0B64:
        jmp    loc_1_0B69
loc_1_0B69:
        test   al, 0x40
        jne    loc_1_0B7E
loc_1_0B6D:
        in     al, dx
        jmp    loc_1_0B73
loc_1_0B73:
        jmp    loc_1_0B78
loc_1_0B78:
        test   al, 0x40
        jne    loc_1_0B7E
        loop   loc_1_0B6D
loc_1_0B7E:
        pop    ecx
        inc    dx
        xchg   ah, al
        jmp    loc_1_0B88
loc_1_0B88:
        jmp    loc_1_0B8D
loc_1_0B8D:
        out    dx, al
        dec    dx
        jmp    loc_1_0B95
loc_1_0B95:
        jmp    loc_1_0B9A
loc_1_0B9A:
        in     al, dx
        sar    al, 3
        and    ax, 0xb847
        or     al, ah
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        popf   
        pop    esi
        pop    ebp
        ret    8
loc_1_0BB0:
        push   ebp
        mov    ebp, esp
        push   edi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x30
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        jmp    loc_1_0BCC
loc_1_0BCC:
        jmp    loc_1_0BD1
loc_1_0BD1:
        out    dx, al
        mov    al, 0x15
        jmp    loc_1_0BD9
loc_1_0BD9:
        jmp    loc_1_0BDE
loc_1_0BDE:
        out    dx, al
        nop    
        nop    
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
loc_1_0BE7:
        push   ecx
        mov    ecx, 0x10
loc_1_0BED:
        in     al, 0x61
        nop    
        loop   loc_1_0BED
        pop    ecx
        mov    al, 0x16
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        jmp    loc_1_0C03
loc_1_0C03:
        jmp    loc_1_0C08
loc_1_0C08:
        in     al, dx
        jmp    loc_1_0C0E
loc_1_0C0E:
        jmp    loc_1_0C13
loc_1_0C13:
        test   al, 0x40
        jne    loc_1_0C46
        in     al, dx
        jmp    loc_1_0C1D
loc_1_0C1D:
        jmp    loc_1_0C22
loc_1_0C22:
        test   al, 0x40
        jne    loc_1_0C46
        in     al, dx
        jmp    loc_1_0C2C
loc_1_0C2C:
        jmp    loc_1_0C31
loc_1_0C31:
        test   al, 0x40
        jne    loc_1_0C46
loc_1_0C35:
        in     al, dx
        jmp    loc_1_0C3B
loc_1_0C3B:
        jmp    loc_1_0C40
loc_1_0C40:
        test   al, 0x40
        jne    loc_1_0C46
        loop   loc_1_0C35
loc_1_0C46:
        pop    ecx
        mov    ah, al
        inc    dx
        mov    al, 0x14
        jmp    loc_1_0C52
loc_1_0C52:
        jmp    loc_1_0C57
loc_1_0C57:
        out    dx, al
        jmp    loc_1_0C5D
loc_1_0C5D:
        jmp    loc_1_0C62
loc_1_0C62:
        nop    
        dec    dx
        in     al, dx
        inc    dx
        sar    al, 3
        and    ax, 0xb847
        or     al, ah
        mov    byte ptr [edi], al
        inc    edi
        dec    ecx
        jne    loc_1_0BE7
        popf   
        pop    edi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        call   sub_1_0A90
        call   sub_1_0930
        mov    al, byte ptr [0]              ; -> loc_1_4660
        xor    esi, esi
        push   eax
        mov    ebx, 0xaa
        call   sub_1_4AC0
        push   0xaa
        push   8
        call   sub_1_13C0
loc_1_0CC1:
        lea    eax, [ebp - 1]
        push   eax
        push   8
        call   sub_1_0CF0
        cmp    byte ptr [ebp - 1], bl
        jne    loc_1_0CE2
        inc    esi
        cmp    esi, 0x3e8
        jb     loc_1_0CC1
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0CE2:
        mov    al, 0xff
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0CF0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        or     al, 0x20
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    ax, 0x1515
        out    dx, al
        mov    al, 0x17
        out    dx, al
        dec    dx
        nop    
        nop    
        nop    
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x40
        jne    loc_1_0D34
        in     al, dx
        test   al, 0x40
        jne    loc_1_0D34
        in     al, dx
        test   al, 0x40
        jne    loc_1_0D34
loc_1_0D2D:
        in     al, dx
        test   al, 0x40
        jne    loc_1_0D34
        loop   loc_1_0D2D
loc_1_0D34:
        pop    ecx
        inc    dx
        xchg   ah, al
        out    dx, al
        dec    dx
        in     al, dx
        sar    al, 3
        and    ax, 0xb847
        or     al, ah
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        popf   
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D60:
        push   ebp
        mov    ebp, esp
        push   edi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x30
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
loc_1_0D81:
        jmp    loc_1_0D86
loc_1_0D86:
        jmp    loc_1_0D8B
loc_1_0D8B:
        jmp    loc_1_0D90
loc_1_0D90:
        mov    al, 0x16
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x40
        jne    loc_1_0DB1
        in     al, dx
        test   al, 0x40
        jne    loc_1_0DB1
        in     al, dx
        test   al, 0x40
        jne    loc_1_0DB1
loc_1_0DAA:
        in     al, dx
        test   al, 0x40
        jne    loc_1_0DB1
        loop   loc_1_0DAA
loc_1_0DB1:
        pop    ecx
        mov    ah, al
        inc    dx
        mov    al, 0x14
        out    dx, al
        nop    
        dec    dx
        in     al, dx
        inc    dx
        sar    al, 3
        and    ax, 0xb847
        or     al, ah
        mov    byte ptr [edi], al
        inc    edi
        loop   loc_1_0D81
        popf   
        pop    edi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0DE0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        or     al, 0x20
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        sub    dx, 2
        mov    al, 0xff
        out    dx, al
        add    dx, 2
        mov    ax, 0x3537
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x80
        je     loc_1_0E2C
        in     al, dx
        test   al, 0x80
        je     loc_1_0E2C
        in     al, dx
        test   al, 0x80
        je     loc_1_0E2C
loc_1_0E25:
        in     al, dx
        test   al, 0x80
        je     loc_1_0E2C
        loop   loc_1_0E25
loc_1_0E2C:
        pop    ecx
        dec    dx
        in     al, dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        add    dx, 2
        mov    al, ah
        out    dx, al
        popf   
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0E50:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf  
        cli    
        cld    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x30
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        sub    dx, 2
        mov    al, 0xff
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        add    dx, 2
loc_1_0E7E:
        jmp    loc_1_0E83
loc_1_0E83:
        jmp    loc_1_0E88
loc_1_0E88:
        jmp    loc_1_0E8D
loc_1_0E8D:
        mov    ax, 0x3537
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x80
        je     loc_1_0EB0
        in     al, dx
        test   al, 0x80
        je     loc_1_0EB0
        in     al, dx
        test   al, 0x80
        je     loc_1_0EB0
loc_1_0EA9:
        in     al, dx
        test   al, 0x80
        je     loc_1_0EB0
        loop   loc_1_0EA9
loc_1_0EB0:
        pop    ecx
        dec    dx
        insb   byte ptr es:[edi], dx
        add    dx, 2
        mov    al, ah
        out    dx, al
        nop    
        nop    
        loop   loc_1_0E7E
        popf   
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0ED0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        or     al, 0x20
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        sub    dx, 2
        mov    al, 0xff
        out    dx, al
        add    dx, 2
        mov    ax, 0x9597
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x80
        je     loc_1_0F1C
        in     al, dx
        test   al, 0x80
        je     loc_1_0F1C
        in     al, dx
        test   al, 0x80
        je     loc_1_0F1C
loc_1_0F15:
        in     al, dx
        test   al, 0x80
        je     loc_1_0F1C
        loop   loc_1_0F15
loc_1_0F1C:
        pop    ecx
        dec    dx
        in     al, dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        add    dx, 2
        mov    al, ah
        out    dx, al
        popf   
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0F40:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x30
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        sub    dx, 2
        mov    al, 0xff
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        add    dx, 2
loc_1_0F6D:
        jmp    loc_1_0F72
loc_1_0F72:
        jmp    loc_1_0F77
loc_1_0F77:
        jmp    loc_1_0F7C
loc_1_0F7C:
        mov    ax, 0x9597
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x80
        je     loc_1_0F9F
        in     al, dx
        test   al, 0x80
        je     loc_1_0F9F
        in     al, dx
        test   al, 0x80
        je     loc_1_0F9F
loc_1_0F98:
        in     al, dx
        test   al, 0x80
        je     loc_1_0F9F
        loop   loc_1_0F98
loc_1_0F9F:
        pop    ecx
        dec    dx
        insb   byte ptr es:[edi], dx
        add    dx, 2
        mov    al, ah
        out    dx, al
        nop    
        nop    
        loop   loc_1_0F6D
        popf   
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0FC0:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        or     al, 0x20
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        sub    dx, 2
        mov    al, 0xff
        out    dx, al
        add    dx, 2
        mov    ax, 0x1517
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x80
        je     loc_1_100C
        in     al, dx
        test   al, 0x80
        je     loc_1_100C
        in     al, dx
        test   al, 0x80
        je     loc_1_100C
loc_1_1005:
        in     al, dx
        test   al, 0x80
        je     loc_1_100C
        loop   loc_1_1005
loc_1_100C:
        pop    ecx
        dec    dx
        in     al, dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        add    dx, 2
        mov    al, ah
        out    dx, al
        popf   
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1030:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x30
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        sub    dx, 2
        mov    al, 0xff
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        add    dx, 2
loc_1_105D:
        jmp    loc_1_1062
loc_1_1062:
        jmp    loc_1_1067
loc_1_1067:
        jmp    loc_1_106C
loc_1_106C:
        mov    ax, 0x1517
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x80
        je     loc_1_108F
        in     al, dx
        test   al, 0x80
        je     loc_1_108F
        in     al, dx
        test   al, 0x80
        je     loc_1_108F
loc_1_1088:
        in     al, dx
        test   al, 0x80
        je     loc_1_108F
        loop   loc_1_1088
loc_1_108F:
        pop    ecx
        dec    dx
        insb   byte ptr es:[edi], dx
        add    dx, 2
        mov    al, ah
        out    dx, al
        nop    
        nop    
        loop   loc_1_105D
        popf   
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    word ptr [0], 0x378           ; -> loc_1_4A1C
        push   ebx
        je     loc_1_10D4
        cmp    word ptr [0], 0x278           ; -> loc_1_4A1C
        je     loc_1_10D4
        mov    al, 0xff
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_10D4:
        mov    al, byte ptr [0]              ; -> loc_1_4660
        mov    cl, byte ptr [0]              ; -> obj1:0x4664
        or     al, 8
        and    cl, 0xfb
        or     al, 0x10
        or     cl, byte ptr [0]              ; -> loc_1_466C
        or     cl, byte ptr [0]              ; -> loc_1_4670
        mov    byte ptr [0], al              ; -> loc_1_4660
        mov    byte ptr [0], 0               ; -> loc_1_4674
        mov    byte ptr [0], cl              ; -> obj1:0x4664
        call   sub_1_0930
        push   bx
        push   dx
        pushf  
        cli    
        in     ax, 0x22
        mov    bx, ax
        test   ax, 1
        jne    loc_1_1129
        mov    ax, 0x8000
        out    0x23, al
        xchg   ah, al
        out    0x22, al
        out    0x22, ax
loc_1_1129:
        in     al, 0x22
        and    al, 0xf0
        or     al, 3
        out    0x22, al
        mov    dx, 0x30e
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    ax, 0x8000
        out    0x23, al
        xchg   ah, al
        out    0x22, al
        mov    ax, bx
        out    0x22, ax
        test   ax, 1
        jne    loc_1_1158
        or     ax, 0x100
        out    0x22, ax
loc_1_1158:
        popf   
        pop    dx
        pop    bx
        cmp    word ptr [ebp - 2], 0x43
        je     loc_1_1181
        cmp    word ptr [ebp - 2], 0x73
        je     loc_1_1181
        cmp    word ptr [ebp - 2], 0x70
        je     loc_1_1181
        cmp    word ptr [ebp - 2], 0x40
        je     loc_1_1181
        mov    al, 0xff
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1181:
        xor    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1190:
        push   ebx
        pushf  
        cli    
        in     ax, 0x22
        mov    cx, ax
        test   ax, 1
        jne    loc_1_11AD
        mov    ax, 0x8000
        out    0x23, al
        xchg   ah, al
        out    0x22, al
        out    0x22, ax
loc_1_11AD:
        mov    ax, 0x100
        out    0x22, ax
        in     al, 0x23
        test   al, 0x80
        je     loc_1_11CB
        mov    dx, 0xfc23
        in     al, dx
        mov    dx, 0xf023
        in     al, dx
        mov    dx, 0xc023
        in     al, dx
        in     al, 0x23
loc_1_11CB:
        mov    al, 0x61
        out    0x24, al
        in     al, 0x25
        mov    bh, al
        or     al, 8
        out    0x25, al
        xor    al, al
        out    0xfb, al
        mov    al, 2
        out    0xae, al
        in     al, 0xaf
        mov    byte ptr [0], al              ; -> loc_1_4A14
        and    al, 0x8f
        cmp    word ptr [0], 0x378           ; -> loc_1_4A1C
        jne    loc_1_11F7
        mov    ah, 0x90
        jmp    loc_1_11F9
loc_1_11F7:
        mov    ah, 0xa0
loc_1_11F9:
        or     al, ah
        out    0xaf, al
        xor    al, al
        out    0xf9, al
        mov    al, 0x61
        out    0x24, al
        mov    al, bh
        out    0x25, al
        test   bl, 0x80
        je     loc_1_1216
        mov    al, 0xfa
        out    0x24, al
        mov    al, 1
        out    0x25, al
loc_1_1216:
        mov    ax, 0x8000
        out    0x23, al
        xchg   ah, al
        out    0x22, al
        mov    ax, cx
        out    0x22, ax
        test   ax, 1
        jne    loc_1_1233
        or     ax, 0x100
        out    0x22, ax
loc_1_1233:
        popf   
        pop    ebx
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1240:
        push   ebx
        pushf  
        cli    
        in     ax, 0x22
        mov    cx, ax
        test   ax, 1
        jne    loc_1_125D
        mov    ax, 0x8000
        out    0x23, al
        xchg   ah, al
        out    0x22, al
        out    0x22, ax
loc_1_125D:
        mov    ax, 0x100
        out    0x22, ax
        in     al, 0x23
        test   al, 0x80
        je     loc_1_127B
        mov    dx, 0xfc23
        in     al, dx
        mov    dx, 0xf023
        in     al, dx
        mov    dx, 0xc023
        in     al, dx
        in     al, 0x23
loc_1_127B:
        mov    al, 0x61
        out    0x24, al
        in     al, 0x25
        mov    bh, al
        or     al, 8
        out    0x25, al
        xor    al, al
        out    0xfb, al
        mov    al, 2
        out    0xae, al
        mov    al, byte ptr [0]              ; -> loc_1_4A14
        out    0xaf, al
        xor    al, al
        out    0xf9, al
        mov    al, 0x61
        out    0x24, al
        mov    al, bh
        out    0x25, al
        test   bl, 0x80
        je     loc_1_12AF
        mov    al, 0xfa
        out    0x24, al
        mov    al, 1
        out    0x25, al
loc_1_12AF:
        mov    ax, 0x8000
        out    0x23, al
        xchg   ah, al
        out    0x22, al
        mov    ax, cx
        out    0x22, ax
        test   ax, 1
        jne    loc_1_12CC
        or     ax, 0x100
        out    0x22, ax
loc_1_12CC:
        popf   
        pop    ebx
        ret    
loc_1_12D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        or     al, 0x20
        add    dx, 3
        out    dx, al
        inc    dx
        mov    edi, dword ptr [ebp + 0xc]
        insb   byte ptr es:[edi], dx
        popf   
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1300:
        push   ebp
        mov    ebp, esp
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        add    dx, 3
        out    dx, al
        mov    al, byte ptr [ebp + 0xc]
        inc    dx
        out    dx, al
        popf   
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1330:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        pushf  
        cli    
        cld    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x30
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        add    dx, 4
        rep insb byte ptr es:[edi], dx
        popf   
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1370:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf  
        cli    
        cld    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x10
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        add    dx, 4
        rep outsb dx, byte ptr [esi]
        popf   
        pop    esi
        pop    ebp
        ret    8
        db  090h
loc_1_13A0:
        push   ebp
        mov    ebp, esp
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        add    dx, 3
        out    dx, al
        popf   
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_13C0:
        push   ebp
        mov    ebp, esp
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        sub    dx, 2
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        add    dx, 2
        mov    ax, 0x1517
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x80
        je     loc_1_140A
        in     al, dx
        test   al, 0x80
        je     loc_1_140A
        in     al, dx
        test   al, 0x80
        je     loc_1_140A
loc_1_1403:
        in     al, dx
        test   al, 0x80
        je     loc_1_140A
        loop   loc_1_1403
loc_1_140A:
        pop    ecx
        inc    dx
        mov    al, ah
        out    dx, al
        popf   
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1420:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf  
        cli    
        cld    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x10
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
loc_1_1442:
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        outsb  dx, byte ptr [esi]
        add    dx, 2
        mov    ax, 0x1517
        out    dx, al
        dec    dx
        push   ecx
        mov    ecx, 0x1000
        in     al, dx
        test   al, 0x80
        je     loc_1_1471
        in     al, dx
        test   al, 0x80
        je     loc_1_1471
        in     al, dx
        test   al, 0x80
        je     loc_1_1471
loc_1_146A:
        in     al, dx
        test   al, 0x80
        je     loc_1_1471
        loop   loc_1_146A
loc_1_1471:
        pop    ecx
        inc    dx
        mov    al, ah
        out    dx, al
        loop   loc_1_1442
        popf   
        pop    esi
        pop    ebp
        ret    8
loc_1_1480:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1490:
        push   ebp
        mov    ebp, esp
        pushf  
        cli    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        or     al, byte ptr [0]              ; -> loc_1_4674
        out    dx, al
        mov    al, 0x15
        or     al, byte ptr [0]              ; -> loc_1_4674
        out    dx, al
        popf   
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_14C0:
        pushf  
        cli    
        cmp    byte ptr [0], 0               ; -> loc_1_4678
        jne    loc_1_14DA
        mov    al, byte ptr [0]              ; -> loc_1_4660
        and    al, 0xfe
        push   eax
        call   sub_1_4AC0
loc_1_14DA:
        dec    byte ptr [0]                  ; -> loc_1_4678
        popf   
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14F0:
        pushf  
        cli    
        mov    al, byte ptr [0]              ; -> loc_1_4660
        push   eax
        call   sub_1_4AC0
        inc    byte ptr [0]                  ; -> loc_1_4678
        js     loc_1_151C
        mov    byte ptr [0], 0               ; -> loc_1_4678
        mov    al, byte ptr [0]              ; -> loc_1_4660
        or     al, 1
        push   eax
        call   sub_1_4AC0
loc_1_151C:
        popf   
        ret    
        db  090h
sub_1_1520:
        pushf  
        cli    
        mov    byte ptr [0], 0               ; -> loc_1_4678
        mov    al, byte ptr [0]              ; -> loc_1_4660
        or     al, 1
        push   eax
        call   sub_1_4AC0
        popf   
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_1540:
        pushf  
        cli    
        mov    byte ptr [0], 0               ; -> loc_1_4678
        mov    al, byte ptr [0]              ; -> loc_1_4660
        and    al, 0xfe
        push   eax
        call   sub_1_4AC0
        popf   
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_1560:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   eax
        push   edx
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        inc    dx
        in     al, dx
        pop    edx
        pop    eax
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x1b2], 1
        jne    loc_1_1588
        xor    al, al
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1588:
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4AB8
        cmp    byte ptr [ebp - 1], 0xff
        jne    loc_1_15A2
        xor    al, al
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_15A2:
        call   sub_1_14C0
        push   0
        push   0xf
        call   sub_1_4AB0
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_15C0:
        mov    byte ptr [0], 5               ; -> loc_1_4454
        push   eax
        push   edx
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        inc    dx
        in     al, dx
        pop    edx
        pop    eax
        mov    al, byte ptr [0]              ; -> loc_1_4660
        xor    al, 2
        push   eax
        mov    byte ptr [0], al              ; -> loc_1_4660
        call   sub_1_4AC0
        xor    al, al
        ret    4
        db  08Dh, 049h, 000h
loc_1_15F0:
        mov    byte ptr [0], 7               ; -> loc_1_4454
        push   eax
        push   edx
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        inc    dx
        in     al, dx
        pop    edx
        pop    eax
        mov    al, byte ptr [0]              ; -> loc_1_4660
        xor    al, 2
        push   eax
        mov    byte ptr [0], al              ; -> loc_1_4660
        call   sub_1_4AC0
        xor    al, al
        ret    4
        db  08Dh, 049h, 000h
loc_1_1620:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_187E
        lea    edi, [esi + 0x103]
        call   sub_1_14C0
        push   edi
        inc    dword ptr [esi + 0x30]
        push   7
        call   sub_1_4AB8
        mov    al, byte ptr [edi]
        test   al, al
        je     loc_1_165C
        push   eax
        push   7
        call   sub_1_4AB0
loc_1_165C:
        mov    al, byte ptr [edi]
        test   al, 0x20
        je     loc_1_1669
        mov    eax, 8
        jmp    loc_1_169D
loc_1_1669:
        test   al, 0x10
        je     loc_1_167A
        push   esi
        call   sub_1_0840
        mov    eax, 4
        jmp    loc_1_169D
loc_1_167A:
        test   al, 0xa
        je     loc_1_1685
        mov    eax, 2
        jmp    loc_1_169D
loc_1_1685:
        test   al, 1
        jne    loc_1_1698
        test   al, 4
        mov    eax, 0x10
        je     loc_1_169D
        push   esi
        call   sub_1_0840
loc_1_1698:
        mov    eax, 1
loc_1_169D:
        xor    ebx, ebx
loc_1_169F:
        cmp    eax, 0x10
        jne    loc_1_16BC
        cmp    dword ptr [esi + 0x161], ebx
        je     loc_1_17A5
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_1_17A5
loc_1_16BC:
        dec    eax
        cmp    eax, 7
        ja     loc_1_1726
        xor    ecx, ecx
        mov    cl, byte ptr [eax]            ; -> obj1:0x189C
        jmp    dword ptr [ecx*4]             ; -> obj1:0x1888
loc_1_16D1:
        push   esi
        call   sub_1_0720
        mov    byte ptr [esi + 0x1b1], bl
        cmp    byte ptr [esi + 0x11b], bl
        jne    loc_1_1704
        mov    byte ptr [esi + 0x5b], bl
        mov    dword ptr [esi + 0x147], ebx
        test   byte ptr [edi], 8
        je     loc_1_16F9
        push   esi
        call   sub_1_2A20
loc_1_16F9:
        test   byte ptr [edi], 2
        je     loc_1_1704
        push   esi
        call   sub_1_1AE0
loc_1_1704:
        and    byte ptr [edi], 0xf5
        jmp    loc_1_1726
loc_1_1709:
        mov    byte ptr [esi + 0x118], 1
        and    byte ptr [edi], 0xef
loc_1_1713:
        push   esi
        call   sub_1_18B0
        test   al, al
        je     loc_1_1720
        and    byte ptr [edi], 0xfa
loc_1_1720:
        push   esi
        call   sub_1_26C0
loc_1_1726:
        cmp    dword ptr [esi + 0x161], ebx
        je     loc_1_173C
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_1_173C
        push   esi
        call   sub_1_18B0
loc_1_173C:
        mov    al, byte ptr [edi]
        test   al, 0x20
        je     loc_1_1757
        mov    eax, 8
        jmp    loc_1_169F
loc_1_174C:
        push   esi
        call   sub_1_0840
        and    byte ptr [edi], 0xdf
        jmp    loc_1_1726
loc_1_1757:
        test   al, 0x10
        je     loc_1_176B
        push   esi
        call   sub_1_0840
        mov    eax, 4
        jmp    loc_1_169F
loc_1_176B:
        test   al, 0xa
        je     loc_1_1779
        mov    eax, 2
        jmp    loc_1_169F
loc_1_1779:
        test   al, 1
        je     loc_1_1787
        mov    eax, 1
        jmp    loc_1_169F
loc_1_1787:
        test   al, 4
        je     loc_1_179B
        push   esi
        call   sub_1_0840
        mov    eax, 1
        jmp    loc_1_169F
loc_1_179B:
        mov    eax, 0x10
        jmp    loc_1_169F
loc_1_17A5:
        push   edi
        push   7
        call   sub_1_4AB8
        mov    al, byte ptr [edi]
        test   al, al
        je     loc_1_17BD
        push   eax
        push   7
        call   sub_1_4AB0
loc_1_17BD:
        mov    al, byte ptr [edi]
        test   al, 0x20
        je     loc_1_17CA
        mov    eax, 8
        jmp    loc_1_17FE
loc_1_17CA:
        test   al, 0x10
        je     loc_1_17DB
        push   esi
        call   sub_1_0840
        mov    eax, 4
        jmp    loc_1_17FE
loc_1_17DB:
        test   al, 0xa
        je     loc_1_17E6
        mov    eax, 2
        jmp    loc_1_17FE
loc_1_17E6:
        test   al, 1
        jne    loc_1_17F9
        test   al, 4
        mov    eax, 0x10
        je     loc_1_17FE
        push   esi
        call   sub_1_0840
loc_1_17F9:
        mov    eax, 1
loc_1_17FE:
        cmp    eax, 0x10
        jne    loc_1_16BC
        mov    byte ptr [esi + 0x11d], 0
        call   sub_1_14F0
        mov    byte ptr [esi + 0x15c], 0x1b
        push   0x1b
        push   0xf
        call   sub_1_4AB0
        call   sub_1_14F0
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_185F
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_184A
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_184A
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_185F
loc_1_184A:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_4232
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_185F:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_187E
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_187E
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_187E
        push   esi
        call   sub_1_26C0
loc_1_187E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_1713
        dd  loc_1_16D1
        dd  loc_1_1709
        dd  loc_1_174C
        dd  loc_1_1726
        db  000h, 001h, 004h, 002h, 004h, 004h, 004h, 003h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h
sub_1_18B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0x11a], 0
        call   sub_1_06F0
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_18E5
        cmp    byte ptr [esi + 0x118], 0
        je     loc_1_18E5
        push   esi
        call   sub_1_08A0
loc_1_18E5:
        xor    ebx, ebx
loc_1_18E7:
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_1_19D4
        test   byte ptr [esi + 0x103], 4
        jne    loc_1_1A7B
        mov    al, byte ptr [esi + 0xfd]
        cmp    byte ptr [esi + 0xfc], al
        jne    loc_1_1926
        push   esi
        call   sub_1_06F0
        mov    al, byte ptr [esi + 0xfd]
        cmp    byte ptr [esi + 0xfc], al
        je     loc_1_19D4
loc_1_1926:
        push   esi
        inc    dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0x169], ebx
        call   sub_1_1FD0
        test   al, al
        je     loc_1_198F
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 0x1b6]
        mov    al, byte ptr [esi + 0x1b5]
        shl    ecx, 8
        add    ecx, eax
        mov    eax, dword ptr [esi + 0x15d]
        sub    ecx, 4
        cmp    ecx, eax
        jb     loc_1_195D
        mov    ecx, eax
loc_1_195D:
        add    ecx, 0xe
        mov    eax, dword ptr [esi + 0x1b7]
        push   ecx
        lea    edx, [esi + 0x1bb]
        push   eax
        push   edx
        call   sub_1_04C0
        test   al, al
        je     loc_1_198F
        push   esi
        call   sub_1_2180
        cmp    eax, 3
        je     loc_1_1A98
        inc    dword ptr [esi + 0x122]
        jmp    loc_1_1994
loc_1_198F:
        mov    eax, 1
loc_1_1994:
        cmp    eax, 3
        je     loc_1_1A98
        cmp    eax, 1
        jne    loc_1_19B0
        push   esi
        call   sub_1_06F0
        mov    al, byte ptr [esi + 0xfd]
        jmp    loc_1_19B6
loc_1_19B0:
        mov    al, byte ptr [esi + 0x1b4]
loc_1_19B6:
        push   esi
        mov    byte ptr [esi + 0xfc], al
        call   sub_1_0740
        cmp    dword ptr [esi + 0x34], 0xa
        jbe    loc_1_18E7
        xor    eax, eax
        mov    byte ptr [ebp - 1], al
        mov    dword ptr [esi + 0x34], eax
loc_1_19D4:
        cmp    byte ptr [esi + 0x118], 0
        je     loc_1_1A15
        mov    byte ptr [esi + 0x118], 0
        push   esi
        call   sub_1_0820
        push   0
        push   0xd
        call   sub_1_4AB0
        cmp    byte ptr [esi + 0x5c], 0
        je     loc_1_1A15
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_1A15
        mov    byte ptr [esi + 0x5c], 0
        mov    byte ptr [esi + 0x1b1], 0
        mov    byte ptr [esi + 0x5b], 1
        push   esi
        call   sub_1_0680
loc_1_1A15:
        cmp    dword ptr [esi + 0x161], 0
        je     loc_1_1A66
loc_1_1A1E:
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_1A6F
        mov    edi, dword ptr [esi + 0x161]
        push   edi
        push   esi
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x161], eax
        mov    dword ptr [esi + 0x169], edi
        inc    dword ptr [esi + 0x11e]
        mov    ebx, dword ptr [edi + 0x28]
        call   sub_1_1E30
        push   0
        mov    ecx, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [ecx]
        push   eax
        call   sub_1_419A
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x161], 0
        jne    loc_1_1A1E
loc_1_1A66:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_1AAA
loc_1_1A6F:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1A7B:
        push   esi
        call   sub_1_06F0
        mov    al, byte ptr [esi + 0xfd]
        push   esi
        mov    byte ptr [esi + 0xfc], al
        call   sub_1_0740
        jmp    loc_1_19D4
loc_1_1A98:
        mov    byte ptr [esi + 0x15c], 0x1a
        push   esi
        call   sub_1_01D0
        jmp    loc_1_19D4
loc_1_1AAA:
        cmp    byte ptr [esi + 0x11a], 0
        je     loc_1_1AC6
        mov    eax, dword ptr [esi + 0x14b]
        push   eax
        call   sub_1_3FE0
        mov    byte ptr [esi + 0x11a], 0
loc_1_1AC6:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x11b], 1
        cmp    dword ptr [esi + 0x57], -1
        jne    loc_1_1B1E
        push   4
        mov    eax, dword ptr [esi + 4]
        mov    byte ptr [esi + 0x11b], 0
        push   1
        push   0xaa000006
        push   eax
        call   sub_1_4328
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1B1E:
        push   esi
        call   sub_1_0720
        test   byte ptr [esi + 0xfe], 1
        je     loc_1_1B35
        inc    dword ptr [esi + 0x11e]
        jmp    loc_1_1B3B
loc_1_1B35:
        inc    dword ptr [esi + 0x126]
loc_1_1B3B:
        mov    ecx, dword ptr [esi + 0x57]
        mov    eax, dword ptr [esi + ecx*4 + 0xbd]
        lea    edx, [esi + ecx*4]
        add    eax, 0xff
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        add    eax, ecx
        cmp    eax, ecx
        jbe    loc_1_1B70
        add    edx, 0x5d
loc_1_1B5C:
        mov    dword ptr [edx], 0
        add    edx, 4
        inc    ecx
        mov    eax, dword ptr [esi + 0x57]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, ecx
        ja     loc_1_1B5C
loc_1_1B70:
        mov    eax, dword ptr [esi + 0x53]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x53], eax
        cmp    eax, 0xc
        jne    loc_1_1B85
        mov    dword ptr [esi + 0x53], 0
loc_1_1B85:
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_1B9B
        cmp    dword ptr [esi + 0x4f], eax
        je     loc_1_1B9B
        mov    dword ptr [esi + 0x53], 0
loc_1_1B9B:
        mov    eax, dword ptr [esi + 0x57]
        mov    dword ptr [esi + eax*4 + 0x8d], 0
        mov    ecx, dword ptr [esi + 0x53]
        lea    eax, [esi + ecx*4 + 0x5d]
        mov    edx, dword ptr [eax]
        test   edx, edx
        je     loc_1_1BD8
        cmp    edx, 1
        je     loc_1_1DB9
        cmp    edx, 2
        je     loc_1_1DD6
        mov    byte ptr [esi + 0x11b], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BD8:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_1C00
        mov    dword ptr [esi + 0x57], 0xffffffff
        mov    byte ptr [esi + 0x11b], 0
        push   2
        push   esi
        call   sub_1_3930
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1C00:
        mov    ebx, dword ptr [esi + 0x47]
        test   ebx, ebx
        je     loc_1_1DA2
        lea    edi, [ebx + 0x24]
        lea    ecx, [ebp - 4]
        push   ecx
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   0
        mov    dword ptr [esi + 0x47], eax
        push   ebx
        call   sub_1_4280
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xff
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 0x4f]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jbe    loc_1_1C43
        mov    dword ptr [esi + 0x4f], 0
loc_1_1C43:
        mov    ecx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        mov    dword ptr [esi + 0x53], ecx
        cmp    eax, ecx
        jbe    loc_1_1C6A
        lea    eax, [esi + ecx*4 + 0x5d]
loc_1_1C56:
        mov    dword ptr [eax], 2
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [esi + 0x4f]
        add    edx, dword ptr [ebp - 4]
        cmp    edx, ecx
        ja     loc_1_1C56
loc_1_1C6A:
        mov    eax, dword ptr [esi + 0x4f]
        mov    dword ptr [esi + eax*4 + 0x8d], ebx
        mov    eax, dword ptr [esi + 0x4f]
        mov    ecx, dword ptr [esi + 0x53]
        lea    eax, [esi + eax*4]
        add    eax, 0xbd
        mov    dword ptr [esi + 0x57], ecx
        push   eax
        push   ebx
        push   esi
        call   sub_1_0240
        test   al, al
        jne    loc_1_1CE3
        push   0xc0000001
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_419A
        mov    edx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [ebp - 4]
        add    eax, edx
        cmp    eax, edx
        jbe    loc_1_1CC6
        lea    eax, [esi + edx*4 + 0x5d]
loc_1_1CB2:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    edx
        mov    ecx, dword ptr [esi + 0x4f]
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, edx
        ja     loc_1_1CB2
loc_1_1CC6:
        mov    eax, dword ptr [esi + 0x4f]
        push   esi
        mov    dword ptr [esi + eax*4 + 0x8d], 0
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1CE3:
        mov    ecx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [esi + ecx*4 + 0xbd]
        cmp    eax, 0x3c
        jae    loc_1_1D12
        shl    ecx, 8
        mov    edx, 0x3c
        sub    edx, eax
        push   edx
        push   0                             ; -> loc_1_47E4
        mov    edx, dword ptr [esi + 0xed]
        add    edx, ecx
        add    edx, eax
        push   edx
        call   sub_1_03E0
loc_1_1D12:
        mov    eax, dword ptr [esi + 0x4f]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x4f], eax
        cmp    eax, 0xc
        jne    loc_1_1D27
        mov    dword ptr [esi + 0x4f], 0
loc_1_1D27:
        mov    byte ptr [esi + 0x11b], 0
        cmp    byte ptr [esi + 0x118], 0
        je     loc_1_1D3D
        mov    byte ptr [esi + 0x5c], 1
        jmp    loc_1_1D47
loc_1_1D3D:
        mov    byte ptr [esi + 0x5b], 1
        push   esi
        call   sub_1_0680
loc_1_1D47:
        cmp    byte ptr [edi + 8], 0
        jne    loc_1_1D6D
        mov    edi, dword ptr [edi + 4]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_419A
        push   esi
        dec    dword ptr [edi + 0x1c]
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1D6D:
        cmp    dword ptr [esi + 0x161], 0
        jne    loc_1_1D7E
        mov    dword ptr [esi + 0x161], ebx
        jmp    loc_1_1D87
loc_1_1D7E:
        mov    eax, dword ptr [esi + 0x165]
        mov    dword ptr [eax + 0x24], ebx
loc_1_1D87:
        push   esi
        mov    dword ptr [esi + 0x165], ebx
        mov    dword ptr [edi], 0
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1DA2:
        mov    dword ptr [esi + 0x57], 0xffffffff
        mov    byte ptr [esi + 0x11b], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1DB9:
        mov    dword ptr [esi + 0x57], 0xffffffff
        mov    byte ptr [esi + 0x11b], 0
        push   esi
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1DD6:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_1E04
        mov    dword ptr [esi + 0x57], 0xffffffff
        push   2
        mov    dword ptr [eax], 0
        mov    byte ptr [esi + 0x11b], 0
        push   esi
        call   sub_1_3930
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1E04:
        push   esi
        mov    dword ptr [esi + 0x57], ecx
        mov    byte ptr [esi + 0x11b], 0
        mov    byte ptr [esi + 0x5b], 1
        call   sub_1_0680
        push   esi
        call   sub_1_26C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        mov    ebx, dword ptr [ebp + 0xc]
        push   edi
        push   eax
        push   0
        push   0
        push   0
        push   ebx
        call   sub_1_4280
        cmp    dword ptr [ebp - 4], 6
        jae    loc_1_1E60
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1E60:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [esi + 0x15d]
        add    edi, 0xe
        cmp    edi, eax
        jb     loc_1_1E75
        mov    edi, eax
loc_1_1E75:
        push   edi
        lea    eax, [esi + 0x1bb]
        push   0
        push   ebx
        push   eax
        call   sub_1_1EF0
        cmp    edi, 0xe
        jae    loc_1_1EA8
        cmp    edi, 6
        jb     loc_1_1ED1
        push   0
        lea    eax, [esi + 0x1bb]
        push   0
        mov    edx, dword ptr [esi + 0x14b]
        push   0
        push   edi
        push   eax
        push   eax
        push   esi
        push   edx
        jmp    loc_1_1ECC
loc_1_1EA8:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, 0xe
        sub    eax, 0xe
        lea    ecx, [esi + 0x1c9]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esi + 0x1bb]
        push   0xe
        mov    ecx, dword ptr [esi + 0x14b]
        push   eax
        push   eax
        push   esi
        push   ecx
loc_1_1ECC:
        call   sub_1_3E00
loc_1_1ED1:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x11a], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1EF0:
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
        call   sub_1_4280
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 4], esi
        je     loc_1_1F44
loc_1_1F15:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_42A4
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     loc_1_1F44
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1F15
loc_1_1F44:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1F55
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1F55:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_1F64:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_1F73
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_1F76
loc_1_1F73:
        mov    ebx, dword ptr [ebp - 8]
loc_1_1F76:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_433C
        cmp    edi, esi
        je     loc_1_1FB2
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1FBD
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_42A4
        jmp    loc_1_1F64
loc_1_1FB2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1FBD:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1FD0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x18
        xor    ecx, ecx
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   6
        mov    al, byte ptr [esi + 0xfc]
        mov    cl, byte ptr [esi + 0xfa]
        sub    eax, ecx
        lea    ecx, [ebp - 0x18]
        shl    eax, 8
        add    eax, dword ptr [esi + 0xf1]
        push   eax
        mov    dword ptr [ebp - 0xc], eax
        push   ecx
        call   sub_1_04C0
        test   al, al
        jne    loc_1_2015
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2015:
        xor    ecx, ecx
        lea    edx, [ebp - 0x18]
        add    dword ptr [ebp - 0xc], 4
loc_1_201E:
        lea    edi, [edx + 1]
        lea    eax, [edx + 2]
        mov    dword ptr [ebp - 8], edi
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [edx + 3]
        lea    edi, [edx + 4]
        mov    byte ptr [ebp - 1], 0
        test   byte ptr [edx], 0x5e
        mov    bl, byte ptr [eax]
        mov    byte ptr [ebp - 2], bl
        jne    loc_1_20B0
        test   byte ptr [edx], 0x20
        je     loc_1_204C
        test   ecx, ecx
        jne    loc_1_204C
        test   byte ptr [edi], 1
        je     loc_1_20B0
loc_1_204C:
        mov    edi, dword ptr [ebp - 0x10]
        cmp    byte ptr [edi], bl
        jne    loc_1_206E
        mov    edi, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 2], bl
        mov    bl, byte ptr [edi]
        mov    edi, dword ptr [ebp - 0x10]
        sub    bl, byte ptr [esi + 0xfc]
        dec    bl
        mov    byte ptr [eax], bl
        cmp    byte ptr [edi], 0xfc
        jbe    loc_1_206E
        inc    eax
loc_1_206E:
        xor    ebx, ebx
        mov    edi, dword ptr [ebp - 0x10]
        mov    bl, byte ptr [eax]
        shl    ebx, 8
        movzx  edi, byte ptr [edi]
        lea    edi, [ebx + edi - 4]
        cmp    edi, 0x5ea
        ja     loc_1_208C
        cmp    edi, 0x3c
        jae    loc_1_2095
loc_1_208C:
        mov    byte ptr [ebp - 1], 1
        mov    bl, byte ptr [ebp - 2]
        mov    byte ptr [eax], bl
loc_1_2095:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_20BA
        mov    edi, dword ptr [ebp - 8]
        mov    al, byte ptr [edi]
        cmp    byte ptr [esi + 0xfa], al
        ja     loc_1_20B0
        cmp    byte ptr [esi + 0xfb], al
        jae    loc_1_20B4
loc_1_20B0:
        mov    byte ptr [ebp - 1], 1
loc_1_20B4:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_20EC
loc_1_20BA:
        test   ecx, ecx
        je     loc_1_20C5
        cmp    ecx, 1
        je     loc_1_20DA
        jmp    loc_1_20EB
loc_1_20C5:
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0xc]
        cmp    byte ptr [esi + 0x11c], 0
        jne    loc_1_20EB
        inc    edx
        inc    dword ptr [ebp - 0xc]
        jmp    loc_1_20EB
loc_1_20DA:
        dec    edx
        dec    dword ptr [ebp - 0xc]
        cmp    byte ptr [esi + 0x11c], 0
        jne    loc_1_20EB
        dec    edx
        dec    dword ptr [ebp - 0xc]
loc_1_20EB:
        inc    ecx
loc_1_20EC:
        cmp    ecx, 2
        jae    loc_1_20FB
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_201E
loc_1_20FB:
        mov    al, byte ptr [edx]
        inc    edx
        inc    edx
        mov    byte ptr [esi + 0x1b3], al
        mov    cl, byte ptr [edx - 1]
        inc    edx
        mov    edi, dword ptr [ebp - 0xc]
        mov    byte ptr [esi + 0x1b4], cl
        mov    al, byte ptr [edx - 1]
        mov    byte ptr [esi + 0x1b5], al
        and    eax, 0xff
        mov    dl, byte ptr [edx]
        mov    byte ptr [esi + 0x1b6], dl
        mov    dword ptr [esi + 0x1b7], edi
        and    edx, 0xff
        shl    edx, 8
        lea    eax, [edx + eax - 4]
        cmp    eax, 0x5ea
        ja     loc_1_2152
        cmp    eax, 0x3c
        jb     loc_1_2152
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2152:
        cmp    byte ptr [esi + 0xfa], cl
        ja     loc_1_216D
        cmp    byte ptr [esi + 0xfb], cl
        jb     loc_1_216D
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_216D:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2180:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 0x1b6]
        mov    al, byte ptr [esi + 0xfc]
        mov    cl, byte ptr [esi + 0xfb]
        lea    edx, [eax + ebx + 1]
        cmp    dl, cl
        jb     loc_1_21AA
        sub    dl, cl
        add    dl, byte ptr [esi + 0xfa]
loc_1_21AA:
        mov    al, byte ptr [esi + 0x1b4]
        cmp    dl, al
        je     loc_1_21CF
        inc    dl
        cmp    cl, dl
        jne    loc_1_21C0
        mov    dl, byte ptr [esi + 0xfa]
loc_1_21C0:
        cmp    al, dl
        je     loc_1_21CF
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_21CF:
        cmp    byte ptr [esi + 0xfa], al
        ja     loc_1_225C
        cmp    al, cl
        ja     loc_1_225C
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 0x1b5]
        mov    al, bl
        shl    eax, 8
        lea    eax, [ecx + eax - 4]
        cmp    eax, 0x5ea
        jbe    loc_1_2204
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2204:
        mov    ecx, dword ptr [esi + 0x15d]
        add    ecx, 0xe
        cmp    ecx, eax
        jb     loc_1_2213
        mov    ecx, eax
loc_1_2213:
        mov    dword ptr [esi + 0x2b7], eax
        cmp    ecx, 0xe
        jae    loc_1_2227
        push   0
        push   0
        push   0
        push   ecx
        jmp    loc_1_2238
loc_1_2227:
        sub    eax, 0xe
        sub    ecx, 0xe
        push   eax
        lea    edx, [esi + 0x1c9]
        push   ecx
        push   edx
        push   0xe
loc_1_2238:
        lea    eax, [esi + 0x1bb]
        mov    edx, dword ptr [esi + 0x14b]
        push   eax
        push   eax
        push   esi
        push   edx
        call   sub_1_3E00
        mov    byte ptr [esi + 0x11a], 1
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_225C:
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2270:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        call   sub_1_14C0
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        add    edi, 0xe
        cmp    dword ptr [esi + 0x169], 0
        je     loc_1_2321
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        mov    ebx, edi
        push   0
        push   ecx
        call   sub_1_4280
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_22EF
loc_1_22AF:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42A4
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [esi + 0x169]
        push   ecx
        mov    eax, dword ptr [ebp - 0x18]
        push   ebx
        push   edx
        push   eax
        call   sub_1_1EF0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 0xc]
        jb     loc_1_22EF
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_22AF
loc_1_22EF:
        call   sub_1_14F0
        cmp    edi, ebx
        jbe    loc_1_230F
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_230F:
        sub    ebx, edi
        mov    ecx, dword ptr [ebp + 0x1c]
        xor    eax, eax
        pop    edi
        pop    esi
        mov    dword ptr [ecx], ebx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_2321:
        mov    eax, dword ptr [esi + 0x2b7]
        mov    ecx, dword ptr [ebp + 0x14]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    loc_1_2358
        cmp    edi, eax
        jbe    loc_1_2351
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], 0
        call   sub_1_14F0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_2351:
        sub    eax, edi
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_1_235B
loc_1_2358:
        mov    dword ptr [ebp - 0x1c], ecx
loc_1_235B:
        mov    eax, dword ptr [ebp - 0x1c]
        add    edi, dword ptr [esi + 0x1b7]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [esi + 0xf5]
        cmp    edi, eax
        jbe    loc_1_2379
        sub    edi, eax
        add    edi, dword ptr [esi + 0xf1]
loc_1_2379:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        push   0
        push   ecx
        call   sub_1_4280
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_42A4
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_1_243A
loc_1_23AE:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ebx, dword ptr [ebp - 0x10]
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0x10]
        ja     loc_1_23BE
        mov    ebx, eax
loc_1_23BE:
        mov    eax, dword ptr [esi + 0xf5]
        lea    ecx, [ebx + edi]
        cmp    ecx, eax
        jbe    loc_1_23CF
        mov    ebx, eax
        sub    ebx, edi
loc_1_23CF:
        push   ebx
        mov    eax, dword ptr [ebp - 0x14]
        push   edi
        add    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04C0
        test   al, al
        je     loc_1_24AD
        add    edi, ebx
        sub    dword ptr [ebp - 0x10], ebx
        je     loc_1_243A
        cmp    dword ptr [esi + 0xf5], edi
        jne    loc_1_23FA
        mov    edi, dword ptr [esi + 0xf1]
loc_1_23FA:
        add    dword ptr [ebp - 4], ebx
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0xc]
        jne    loc_1_2430
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_42C8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_243A
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_42A4
        mov    dword ptr [ebp - 4], 0
loc_1_2430:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_23AE
loc_1_243A:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx], eax
        cmp    byte ptr [esi + 0x118], 0
        jne    loc_1_2575
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_2575
        lea    eax, [ebp - 4]
        push   eax
        push   7
        call   sub_1_4AB8
        test   byte ptr [ebp - 4], 8
        je     loc_1_2474
        push   esi
        call   sub_1_2A20
loc_1_2474:
        test   byte ptr [ebp - 4], 2
        je     loc_1_2575
        push   2
        push   7
        call   sub_1_4AB0
        push   esi
        call   sub_1_0720
        mov    byte ptr [esi + 0x5b], 0
        mov    dword ptr [esi + 0x147], 0
        test   byte ptr [esi + 0xfe], 1
        je     loc_1_24E0
        inc    dword ptr [esi + 0x11e]
        jmp    loc_1_24E6
loc_1_24AD:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp - 0x10]
        push   2
        push   1
        mov    dword ptr [ecx], eax
        push   0xaa000003
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_1_4328
        add    esp, 0x10
        call   sub_1_14F0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_24E0:
        inc    dword ptr [esi + 0x126]
loc_1_24E6:
        mov    edi, dword ptr [esi + 0x57]
        mov    edx, dword ptr [esi + edi*4 + 0xbd]
        lea    ecx, [esi + edi*4]
        add    edx, 0xff
        shr    edx, 8
        lea    eax, [edx + edi]
        cmp    eax, edi
        jbe    loc_1_2519
        add    ecx, 0x5d
loc_1_2506:
        mov    dword ptr [ecx], 0
        add    ecx, 4
        inc    edi
        mov    eax, dword ptr [esi + 0x57]
        add    eax, edx
        cmp    eax, edi
        ja     loc_1_2506
loc_1_2519:
        mov    eax, dword ptr [esi + 0x53]
        add    eax, edx
        mov    dword ptr [esi + 0x53], eax
        cmp    eax, 0xc
        jne    loc_1_252D
        mov    dword ptr [esi + 0x53], 0
loc_1_252D:
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_2543
        cmp    dword ptr [esi + 0x4f], eax
        je     loc_1_2543
        mov    dword ptr [esi + 0x53], 0
loc_1_2543:
        mov    eax, dword ptr [esi + 0x57]
        xor    ecx, ecx
        mov    dword ptr [esi + eax*4 + 0x8d], ecx
        mov    byte ptr [esi + 0x1b1], cl
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x5d], 2
        jne    loc_1_256E
        push   esi
        mov    dword ptr [esi + 0x57], eax
        mov    byte ptr [esi + 0x5b], 1
        call   sub_1_0680
        jmp    loc_1_2575
loc_1_256E:
        mov    dword ptr [esi + 0x57], 0xffffffff
loc_1_2575:
        call   sub_1_14F0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2590:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebx + 8]
        add    edi, 0x24
        call   sub_1_14C0
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_25C3
        call   sub_1_14F0
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_25C3:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_25DF
        call   sub_1_14F0
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_25DF:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        inc    dword ptr [ebx + 0x1c]
        push   0
        inc    dword ptr [esi + 0x30]
        push   ecx
        mov    dword ptr [edi + 4], ebx
        call   sub_1_4280
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_42A4
        test   byte ptr [ebx + 0x71], 2
        je     loc_1_2618
        mov    byte ptr [edi + 8], 0
        jmp    loc_1_262B
loc_1_2618:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x14b]
        push   eax
        push   ecx
        call   sub_1_4080
        mov    byte ptr [edi + 8], al
loc_1_262B:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_263C
        mov    eax, dword ptr [esi + 0x4b]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], ecx
        jmp    loc_1_2642
loc_1_263C:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x47], ecx
loc_1_2642:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x4b], ecx
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_267F
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_266F
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_266F
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_267F
loc_1_266F:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_4232
        jmp    loc_1_269E
loc_1_267F:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_269E
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_269E
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_269E
        push   esi
        call   sub_1_26C0
loc_1_269E:
        call   sub_1_14F0
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_26C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        call   sub_1_14C0
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_291A
loc_1_26DB:
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_291A
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [esi + 0x47]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_4280
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x57]
        add    eax, 0xff
        shr    eax, 8
        cmp    ecx, -1
        mov    dword ptr [ebp - 4], eax
        jne    loc_1_276F
        mov    edx, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + edx*4 + 0x5d], 0
        jne    loc_1_2730
        mov    eax, dword ptr [esi + 0x4f]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jbe    loc_1_27A4
        mov    dword ptr [esi + 0x4f], 0
        jmp    loc_1_27A4
loc_1_2730:
        mov    ecx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, edx
        jae    loc_1_274E
        add    eax, ecx
        cmp    eax, edx
        jbe    loc_1_27A4
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_274E:
        add    eax, ecx
        cmp    eax, 0xc
        jbe    loc_1_27A4
        mov    dword ptr [esi + 0x4f], 0
        cmp    dword ptr [ebp - 4], edx
        jbe    loc_1_27A4
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_276F:
        mov    edx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, edx
        jbe    loc_1_278D
        add    eax, edx
        cmp    eax, ecx
        jbe    loc_1_27A4
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_278D:
        add    eax, edx
        cmp    eax, 0xc
        jbe    loc_1_27A4
        mov    dword ptr [esi + 0x4f], 0
        cmp    dword ptr [ebp - 4], ecx
        ja     loc_1_2928
loc_1_27A4:
        mov    ecx, dword ptr [esi + 0x4f]
        mov    ebx, dword ptr [esi + 0x47]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esi + 0x47], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        cmp    eax, ecx
        jbe    loc_1_27CA
        lea    edi, [esi + ecx*4 + 0x5d]
        mov    eax, 1
        mov    ecx, dword ptr [ebp - 4]
        rep stosd dword ptr es:[edi], eax
loc_1_27CA:
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [esi + ecx*4 + 0xbd]
        mov    dword ptr [esi + ecx*4 + 0x8d], ebx
        lea    eax, [esi + ecx*4]
        push   edi
        push   ebx
        mov    dword ptr [ebp - 0xc], eax
        push   esi
        call   sub_1_0240
        test   al, al
        jne    loc_1_282E
        push   0xc0000001
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_419A
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_2816
        mov    edi, dword ptr [ebp - 0xc]
        xor    eax, eax
        add    edi, 0x5d
        mov    ecx, dword ptr [ebp - 4]
        rep stosd dword ptr es:[edi], eax
loc_1_2816:
        mov    eax, dword ptr [ebx + 0x28]
        mov    ecx, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x1c]
        mov    dword ptr [ecx + 0x8d], 0
        jmp    loc_1_2910
loc_1_282E:
        mov    edx, dword ptr [edi]
        cmp    edx, 0x3c
        jae    loc_1_2858
        mov    eax, 0x3c
        mov    ecx, dword ptr [ebp - 8]
        shl    ecx, 8
        sub    eax, edx
        push   eax
        push   0                             ; -> loc_1_47E4
        mov    eax, dword ptr [esi + 0xed]
        add    eax, ecx
        add    eax, edx
        push   eax
        call   sub_1_03E0
loc_1_2858:
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_2936
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_2880
        mov    edi, dword ptr [ebp - 0xc]
        mov    eax, 2
        add    edi, 0x5d
        mov    ecx, dword ptr [ebp - 4]
        rep stosd dword ptr es:[edi], eax
loc_1_2880:
        mov    eax, dword ptr [esi + 0x4f]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x4f], eax
        cmp    eax, 0xc
        jne    loc_1_2895
        mov    dword ptr [esi + 0x4f], 0
loc_1_2895:
        cmp    dword ptr [esi + 0x57], -1
        jne    loc_1_28D0
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_28B1
        cmp    dword ptr [esi + 0x4f], eax
        je     loc_1_28B1
        mov    dword ptr [esi + 0x53], 0
loc_1_28B1:
        cmp    byte ptr [esi + 0x118], 0
        mov    eax, dword ptr [esi + 0x53]
        mov    dword ptr [esi + 0x57], eax
        je     loc_1_28C6
        mov    byte ptr [esi + 0x5c], 1
        jmp    loc_1_28D0
loc_1_28C6:
        mov    byte ptr [esi + 0x5b], 1
        push   esi
        call   sub_1_0680
loc_1_28D0:
        cmp    byte ptr [ebx + 0x2c], 0
        jne    loc_1_28E9
        mov    edi, dword ptr [ebx + 0x28]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_419A
        dec    dword ptr [edi + 0x1c]
        jmp    loc_1_2910
loc_1_28E9:
        cmp    dword ptr [esi + 0x161], 0
        jne    loc_1_28FA
        mov    dword ptr [esi + 0x161], ebx
        jmp    loc_1_2903
loc_1_28FA:
        mov    eax, dword ptr [esi + 0x165]
        mov    dword ptr [eax + 0x24], ebx
loc_1_2903:
        mov    dword ptr [esi + 0x165], ebx
        mov    dword ptr [ebx + 0x24], 0
loc_1_2910:
        cmp    dword ptr [esi + 0x47], 0
        jne    loc_1_26DB
loc_1_291A:
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2928:
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2936:
        mov    eax, dword ptr [ebx + 0x28]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_2955
        mov    ecx, dword ptr [ebp - 8]
        xor    eax, eax
        lea    edi, [esi + ecx*4 + 0x5d]
        mov    ecx, dword ptr [ebp - 4]
        rep stosd dword ptr es:[edi], eax
loc_1_2955:
        push   0
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_419A
        push   3
        push   esi
        call   sub_1_3930
        mov    ecx, dword ptr [ebp - 0xc]
        dec    dword ptr [ecx + 0x1c]
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_2980:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf  
        cli    
        call   sub_1_14C0
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x1b1], 0
        je     loc_1_29E7
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_29F4
        mov    byte ptr [esi + 0x1b1], 0
        push   3
        mov    eax, dword ptr [esi + 4]
        push   1
        push   0xaa000003
        push   eax
        call   sub_1_4328
        add    esp, 0x10
        push   esi
        call   sub_1_0150
        cmp    dword ptr [esi + 0x147], 4
        jbe    loc_1_29D9
        push   esi
        call   sub_1_01D0
        mov    dword ptr [esi + 0x147], 0
loc_1_29D9:
        push   esi
        call   sub_1_2A20
        inc    dword ptr [esi + 0x147]
        jmp    loc_1_29F4
loc_1_29E7:
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_29F4
        mov    byte ptr [esi + 0x1b1], 1
loc_1_29F4:
        push   0x7d0
        add    esi, 0x1a5
        push   esi
        call   sub_1_4232
        call   sub_1_14F0
        popf   
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2A20:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   8
        push   7
        mov    edi, 0x14
        call   sub_1_4AB0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0670
loc_1_2A3D:
        push   0x64
        call   sub_1_4250
        dec    edi
        jne    loc_1_2A3D
        push   2
        push   0xd
        call   sub_1_4AB0
        push   0x22
        push   0
        call   sub_1_4AB0
        push   0
        push   0xd
        call   sub_1_4AB0
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_2A75
        mov    byte ptr [esi + 0x5b], 1
        push   esi
        call   sub_1_0680
loc_1_2A75:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2A80:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x28]
        test   edx, edx
        je     loc_1_2AA0
loc_1_2A90:
        mov    esi, dword ptr [edx + 0x18]
        cmp    ecx, esi
        jae    loc_1_2A99
        mov    ecx, esi
loc_1_2A99:
        mov    edx, dword ptr [edx + 0xc]
        test   edx, edx
        jne    loc_1_2A90
loc_1_2AA0:
        test   ecx, ecx
        jne    loc_1_2AA9
        mov    ecx, 0xee
loc_1_2AA9:
        pop    esi
        mov    dword ptr [eax + 0x15d], ecx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x20], 0
        mov    esi, dword ptr [ebx + 8]
        je     loc_1_2AE3
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2AE3:
        call   sub_1_14C0
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [ebx + 0x1c]
        mov    ecx, dword ptr [esi + 0x28]
        cmp    ebx, ecx
        jne    loc_1_2AFD
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x28], eax
        jmp    loc_1_2B10
loc_1_2AFD:
        cmp    dword ptr [ecx + 0xc], ebx
        je     loc_1_2B0A
loc_1_2B02:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], ebx
        jne    loc_1_2B02
loc_1_2B0A:
        mov    edx, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0xc], edx
loc_1_2B10:
        push   0
        mov    eax, dword ptr [ebx + 0x14]
        push   eax
        mov    ecx, dword ptr [esi + 0x14b]
        push   ecx
        call   sub_2_18F0
loc_1_2B22:
        mov    edi, eax
        test   edi, edi
        jne    loc_1_2B47
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    eax, 2
        je     loc_1_2B3F
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        mov    edi, 0x103
        jmp    loc_1_2B6D
loc_1_2B3F:
        sub    eax, 2
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_1_2B6D
loc_1_2B47:
        cmp    edi, 0x103
        jne    loc_1_2B59
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        jmp    loc_1_2B6D
loc_1_2B59:
        cmp    edi, 0xc001000e
        jne    loc_1_2B6A
        mov    byte ptr [ebx + 0x20], 1
        mov    edi, 0x103
loc_1_2B6A:
        dec    dword ptr [ebx + 0x1c]
loc_1_2B6D:
        cmp    dword ptr [ebx + 0x1c], 0
        jne    loc_1_2BA2
        mov    eax, dword ptr [ebx + 0x18]
        cmp    dword ptr [esi + 0x15d], eax
        jne    loc_1_2B84
        push   esi
        call   sub_1_2A80
loc_1_2B84:
        push   0
        push   0x75
        push   ebx
        call   sub_1_425C
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_2BB0
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_1_2BB0
        push   esi
        call   sub_1_0150
        jmp    loc_1_2BB0
loc_1_2BA2:
        mov    eax, dword ptr [esi + 0x2c]
        mov    edi, 0x103
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [esi + 0x2c], ebx
loc_1_2BB0:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_2BE1
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_2BD1
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_2BD1
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_2BE1
loc_1_2BD1:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_4232
        jmp    loc_1_2C00
loc_1_2BE1:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_2C00
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_2C00
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_2C00
        push   esi
        call   sub_1_26C0
loc_1_2C00:
        call   sub_1_14F0
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2C10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        call   sub_1_14C0
        inc    dword ptr [ebx + 0x1c]
        inc    dword ptr [edi + 0x30]
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_2C34
        mov    esi, 0xc0010002
        jmp    loc_1_2C70
loc_1_2C34:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x10]
        test   eax, eax
        jne    loc_1_2C4A
        push   ecx
        push   ebx
        push   edi
        call   sub_1_3060
        mov    esi, eax
        jmp    loc_1_2C70
loc_1_2C4A:
        cmp    eax, 1
        jne    loc_1_2C6B
        cmp    byte ptr [edi + 0x13e], 0
        je     loc_1_2C5F
        mov    esi, 0xc001000d
        jmp    loc_1_2C70
loc_1_2C5F:
        push   ecx
        push   ebx
        push   edi
        call   sub_1_30B0
        mov    esi, eax
        jmp    loc_1_2C70
loc_1_2C6B:
        mov    esi, 0x10001
loc_1_2C70:
        dec    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_1_2CA4
        cmp    byte ptr [edi + 0x13e], 0
        jne    loc_1_2C94
        cmp    dword ptr [edi + 0x16d], 0
        jne    loc_1_2C94
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_1_2CA4
loc_1_2C94:
        push   1
        add    edi, 0x179
        push   edi
        call   sub_1_4232
        jmp    loc_1_2CC3
loc_1_2CA4:
        cmp    dword ptr [edi + 0x47], 0
        je     loc_1_2CC3
        mov    eax, dword ptr [edi + 0x4f]
        cmp    dword ptr [edi + eax*4 + 0x5d], 0
        jne    loc_1_2CC3
        cmp    byte ptr [edi + 0x11d], 0
        jne    loc_1_2CC3
        push   edi
        call   sub_1_26C0
loc_1_2CC3:
        call   sub_1_14F0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2CE0:
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
        ja     loc_1_2D1D
        je     loc_1_2EAF
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_2D3E
        jmp    dword ptr [eax*4]             ; -> obj1:0x300C
loc_1_2D1D:
        cmp    eax, 0x1010102
        je     loc_1_2ED1
        cmp    eax, 0x1010103
        je     loc_1_2EF3
        cmp    eax, 0x1010104
        je     loc_1_2FB8
loc_1_2D3E:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2FC1
loc_1_2D4A:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_2D5F
        mov    esi, 0                        ; -> loc_1_48F0
        mov    edi, 0x5c
        jmp    loc_1_2FC1
loc_1_2D5F:
        mov    esi, 0                        ; -> loc_1_4874
        mov    edi, 0x7c
        jmp    loc_1_2FC1
loc_1_2D6E:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x14], 2
        cmp    byte ptr [ebx + 0x13e], 0
        jne    loc_1_2D88
        mov    dword ptr [ebp - 0x14], 0
loc_1_2D88:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_1_2FC1
loc_1_2D95:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_2FC1
loc_1_2DA2:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_1_2FC1
loc_1_2DAE:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_2FC1
loc_1_2DBA:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_2FC1
loc_1_2DC6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x38]
        shl    eax, 8
        jmp    loc_1_2FBE
loc_1_2DD4:
        mov    eax, 0x20
        mov    ebx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ebx + 0x38]
        shl    eax, 8
        jmp    loc_1_2FBE
loc_1_2DE7:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_2FC1
loc_1_2DF3:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_2FC1
loc_1_2DFF:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x10a
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        call   sub_1_433C
        and    dword ptr [ebp - 8], 0xffffff00
        jmp    loc_1_2FC1
loc_1_2E28:
        mov    esi, 0                        ; -> loc_1_49E8
        mov    edi, 0x21
        jmp    loc_1_2FC1
loc_1_2E37:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2E51
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x14b]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_2FC1
loc_1_2E51:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x14]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_2FC1
loc_1_2E62:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_2E76
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x15d]
        jmp    loc_1_2FBE
loc_1_2E76:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_2FC1
loc_1_2E84:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_2FC1
loc_1_2E97:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_2FC1
loc_1_2EA3:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_1_2FC1
loc_1_2EAF:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x10a
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_433C
        jmp    loc_1_2FC1
loc_1_2ED1:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x104
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_433C
        jmp    loc_1_2FC1
loc_1_2EF3:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_2F53
        mov    ecx, dword ptr [ebx + 0x14b]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    edx, dword ptr [ebp + 0x1c]
        jbe    loc_1_2F28
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], edx
        jmp    loc_1_2FC1
loc_1_2F28:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x10]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0xc]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_1E10
loc_1_2F3E:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ecx], eax
        je     loc_1_2FC7
        jmp    loc_1_2FC1
loc_1_2F53:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebx + 0x14b]
        mov    ecx, dword ptr [eax + 0x14]
        push   ecx
        push   edx
        call   sub_2_1D10
loc_1_2F66:
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 0x10], eax
        lea    ecx, [edx*2]
        cmp    ecx, dword ptr [ebp + 0x1c]
        jbe    loc_1_2F86
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], ecx
        jmp    loc_1_2FC1
loc_1_2F86:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        push   edx
        mov    ecx, dword ptr [eax + 0x14]
        push   ecx
        mov    edx, dword ptr [ebx + 0x14b]
        push   edx
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_1D50
loc_1_2FA9:
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_2FC1
loc_1_2FB8:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x41]
loc_1_2FBE:
        mov    dword ptr [ebp - 8], eax
loc_1_2FC1:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2FFD
loc_1_2FC7:
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_2FFD
        cmp    edi, dword ptr [ebp + 0x1c]
        jbe    loc_1_2FED
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
loc_1_2FED:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   esi
        push   eax
        call   sub_1_433C
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_2FFD:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_2D4A
        dd  loc_1_2D6E
        dd  loc_1_2D95
        dd  loc_1_2D95
        dd  loc_1_2DA2
        dd  loc_1_2DAE
        dd  loc_1_2DBA
        dd  loc_1_2DC6
        dd  loc_1_2DD4
        dd  loc_1_2DE7
        dd  loc_1_2DF3
        dd  loc_1_2DFF
        dd  loc_1_2E28
        dd  loc_1_2E37
        dd  loc_1_2E62
        dd  loc_1_2E84
        dd  loc_1_2E97
        dd  loc_1_2D3E
        dd  loc_1_2EA3
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3060:
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
        call   sub_1_2CE0
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_30B0:
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
        ja     loc_1_30ED
        je     loc_1_31FC
        cmp    eax, 0x1010e
        je     loc_1_30FF
        cmp    eax, 0x1010f
        je     loc_1_315A
        cmp    eax, 0x10112
        je     loc_1_31D5
loc_1_30ED:
        mov    ebx, 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_3237
loc_1_30FF:
        cmp    edi, 4
        je     loc_1_3116
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_3237
loc_1_3116:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_433C
        test   dword ptr [ebp - 8], 0xf050
        je     loc_1_3143
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        mov    ebx, 0xc00000bb
        jmp    loc_1_3237
loc_1_3143:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_3250
        jmp    loc_1_3235
loc_1_315A:
        cmp    edi, 4
        je     loc_1_3171
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_3237
loc_1_3171:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_433C
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_1_31CE
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x15d]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_31A8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x15d], eax
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_3237
loc_1_31A8:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], eax
        jne    loc_1_31C6
        mov    eax, dword ptr [ecx + 0x18]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_31C6
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x18], eax
        call   sub_1_2A80
        jmp    loc_1_3237
loc_1_31C6:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_3237
loc_1_31CE:
        mov    ebx, 0xc0010014
        jmp    loc_1_3237
loc_1_31D5:
        cmp    edi, 4
        je     loc_1_31E9
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_3237
loc_1_31E9:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x71
        push   ecx
        call   sub_1_433C
        jmp    loc_1_3237
loc_1_31FC:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_1_321A
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_3237
loc_1_321A:
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
        call   sub_1_32B0
loc_1_3235:
        mov    ebx, eax
loc_1_3237:
        test   ebx, ebx
        jne    loc_1_3245
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_1_3245:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_3250:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 0x13e], 0
        jne    loc_1_3299
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_328F
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [ecx + 0x14b]
        push   ecx
        call   sub_2_1C90
loc_1_3287:
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
loc_1_328F:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_1_3299:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32B0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 0x13e], 0
        jne    loc_1_32FD
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_32F3
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x14]
        push   edx
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [esi + 0x14]
        push   edx
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [ecx + 0x14b]
        push   ecx
        call   sub_2_19B0
loc_1_32EB:
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x14
loc_1_32F3:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
loc_1_32FD:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3310:
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
        call   sub_1_2CE0
        cmp    eax, 0xc00000bb
        mov    edi, eax
        jne    loc_1_33F6
        xor    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_1_3383
        je     loc_1_33C0
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_3391
        jmp    dword ptr [eax*4]             ; -> obj1:0x3410
loc_1_3383:
        cmp    eax, 0x1020102
        je     loc_1_33C8
        cmp    eax, 0x1020103
        je     loc_1_33D0
loc_1_3391:
        mov    edi, 0xc0010017
        jmp    loc_1_33D9
loc_1_3398:
        mov    eax, dword ptr [ebx + 0x11e]
        jmp    loc_1_33D6
loc_1_33A0:
        mov    eax, dword ptr [ebx + 0x122]
        jmp    loc_1_33D6
loc_1_33A8:
        mov    eax, dword ptr [ebx + 0x126]
        jmp    loc_1_33D6
loc_1_33B0:
        mov    eax, dword ptr [ebx + 0x136]
        jmp    loc_1_33D6
loc_1_33B8:
        mov    eax, dword ptr [ebx + 0x13a]
        jmp    loc_1_33D6
loc_1_33C0:
        mov    eax, dword ptr [ebx + 0x132]
        jmp    loc_1_33D6
loc_1_33C8:
        mov    eax, dword ptr [ebx + 0x12a]
        jmp    loc_1_33D6
loc_1_33D0:
        mov    eax, dword ptr [ebx + 0x12e]
loc_1_33D6:
        mov    dword ptr [ebp - 4], eax
loc_1_33D9:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_33F6
        test   edi, edi
        jne    loc_1_33F6
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_433C
        add    dword ptr [ebp - 8], 4
loc_1_33F6:
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
        db  08Dh, 049h, 000h
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_3398
        dd  loc_1_33A0
        dd  loc_1_33A8
        dd  loc_1_33B0
        dd  loc_1_33B8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3430:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        call   sub_1_14C0
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x10113
        ja     loc_1_345B
        je     loc_1_348C
        cmp    eax, 0x10101
        jb     loc_1_3487
        cmp    eax, 0x10111
        jbe    loc_1_348C
        jmp    loc_1_3487
loc_1_345B:
        cmp    eax, 0x1010104
        ja     loc_1_3479
        cmp    eax, 0x1010101
        jae    loc_1_348C
        cmp    eax, 0x20101
        jb     loc_1_3487
        cmp    eax, 0x20105
        jbe    loc_1_348C
        jmp    loc_1_3487
loc_1_3479:
        cmp    eax, 0x1020101
        jb     loc_1_3487
        cmp    eax, 0x1020103
        jbe    loc_1_348C
loc_1_3487:
        mov    esi, 0xc0010017
loc_1_348C:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x30]
        test   esi, esi
        jne    loc_1_349F
        push   ecx
        push   edi
        call   sub_1_3310
        mov    esi, eax
loc_1_349F:
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_1_34D0
        cmp    byte ptr [edi + 0x13e], 0
        jne    loc_1_34C0
        cmp    dword ptr [edi + 0x16d], 0
        jne    loc_1_34C0
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_1_34D0
loc_1_34C0:
        push   1
        add    edi, 0x179
        push   edi
        call   sub_1_4232
        jmp    loc_1_34EF
loc_1_34D0:
        cmp    dword ptr [edi + 0x47], 0
        je     loc_1_34EF
        mov    eax, dword ptr [edi + 0x4f]
        cmp    dword ptr [edi + eax*4 + 0x5d], 0
        jne    loc_1_34EF
        cmp    byte ptr [edi + 0x11d], 0
        jne    loc_1_34EF
        push   edi
        call   sub_1_26C0
loc_1_34EF:
        call   sub_1_14F0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_3500:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x4828
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_417C
        push   0
        mov    ecx, dword ptr [4]            ; -> obj1:0x482C
        push   ecx
        call   sub_1_4170
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3530:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x1b2], 1
        call   sub_1_14C0
        cmp    dword ptr [0x3e], esi         ; -> obj1:0x4866
        jne    loc_1_3559
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x3e], eax         ; -> obj1:0x4866
        jmp    loc_1_3575
loc_1_3559:
        mov    ecx, dword ptr [0x3e]         ; -> obj1:0x4866
        cmp    dword ptr [ecx + 0x24], esi
        je     loc_1_356C
loc_1_3564:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], esi
        jne    loc_1_3564
loc_1_356C:
        mov    eax, dword ptr [ecx + 0x24]
        mov    edx, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], edx
loc_1_3575:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x1a5]
        push   eax
        mov    edi, 0x61a8
        push   ecx
        call   sub_1_4238
loc_1_358A:
        push   0x64
        call   sub_1_4250
        dec    edi
        jne    loc_1_358A
        mov    byte ptr [0], 0               ; -> loc_1_4820
        push   0x21
        push   0
        call   sub_1_4AB0
        call   sub_1_4A20
        call   sub_2_06F0
loc_1_35B0:
        mov    eax, dword ptr [esi + 0x14b]
        push   eax
        call   sub_2_17D0
loc_1_35BC:
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_4244
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4188
        push   0
        push   0x2eb
        push   esi
        call   sub_1_425C
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x20], 0
        mov    esi, dword ptr [edi + 8]
        je     loc_1_3613
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3613:
        call   sub_1_14C0
        inc    dword ptr [edi + 0x1c]
        inc    dword ptr [esi + 0x30]
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_3640
        dec    dword ptr [edi + 0x1c]
        dec    dword ptr [esi + 0x30]
        call   sub_1_14F0
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3640:
        mov    ebx, dword ptr [esi + 0x28]
        mov    dword ptr [ebp - 4], 0
        test   ebx, ebx
        je     loc_1_3670
loc_1_364E:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [ebx + 0x1c]
        push   0x40010004
        mov    ecx, dword ptr [ebx]
        push   ecx
        call   sub_1_41BE
        mov    eax, dword ptr [ebx + 0xc]
        dec    dword ptr [ebx + 0x1c]
        mov    ebx, eax
        test   eax, eax
        jne    loc_1_364E
loc_1_3670:
        mov    byte ptr [esi + 0x13e], 1
        mov    dword ptr [esi + 0x13f], 0
        push   esi
        mov    dword ptr [esi + 0x143], edi
        call   sub_1_3700
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [edi + 0x1c]
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_36C4
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_36B4
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_36B4
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_36C4
loc_1_36B4:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_4232
        jmp    loc_1_36E3
loc_1_36C4:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_36E3
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_36E3
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_36E3
        push   esi
        call   sub_1_26C0
loc_1_36E3:
        call   sub_1_14F0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3700:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x30]
        cmp    dword ptr [esi + 0x16d], 0
        je     loc_1_374C
loc_1_3714:
        mov    ecx, dword ptr [esi + 0x16d]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x16d], eax
        mov    eax, dword ptr [ecx + 8]
        mov    edi, dword ptr [ecx + 4]
        cmp    eax, 4
        je     loc_1_3740
        cmp    eax, 8
        je     loc_1_3740
        push   0xc001000c
        mov    eax, dword ptr [edi]
        push   ecx
        push   eax
        call   sub_1_41AC
loc_1_3740:
        dec    dword ptr [edi + 0x1c]
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_3714
loc_1_374C:
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_376A
        mov    eax, 0x103
        pop    edi
        mov    dword ptr [esi + 0x13f], 2
        dec    dword ptr [esi + 0x30]
        pop    esi
        pop    ebp
        ret    4
loc_1_376A:
        push   esi
        dec    dword ptr [esi + 0x30]
        call   sub_1_3780
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3780:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        xor    ecx, ecx
        push   esi
        mov    dword ptr [eax + 0x4f], ecx
        push   edi
        mov    esi, dword ptr [eax + 0x30]
        inc    esi
        mov    edx, dword ptr [eax + 0x38]
        mov    dword ptr [eax + 0x30], esi
        mov    dword ptr [eax + 0x53], ecx
        cmp    edx, ecx
        je     loc_1_37AF
        lea    edi, [eax + 0x5d]
loc_1_37A2:
        cmp    dword ptr [edi], 0
        jne    loc_1_37BF
        add    edi, 4
        inc    ecx
        cmp    ecx, edx
        jb     loc_1_37A2
loc_1_37AF:
        dec    esi
        push   eax
        mov    dword ptr [eax + 0x30], esi
        call   sub_1_37E0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_37BF:
        dec    esi
        pop    edi
        mov    dword ptr [eax + 0x13f], 3
        mov    dword ptr [eax + 0x30], esi
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_37E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        inc    dword ptr [esi + 0x30]
        cmp    dword ptr [esi + 0x38], 0
        jbe    loc_1_3830
        lea    edi, [esi + 0x8d]
loc_1_3802:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        je     loc_1_3822
        mov    ebx, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_419A
        dec    dword ptr [ebx + 0x1c]
        mov    dword ptr [edi], 0
loc_1_3822:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0x38], eax
        ja     loc_1_3802
loc_1_3830:
        mov    edx, dword ptr [esi + 0x47]
        test   edx, edx
        je     loc_1_385A
loc_1_3837:
        lea    eax, [edx + 0x24]
        push   0xc001000c
        push   edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x47], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_419A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0x47]
        test   edx, edx
        jne    loc_1_3837
loc_1_385A:
        mov    edx, dword ptr [esi + 0x161]
        test   edx, edx
        je     loc_1_388D
loc_1_3864:
        lea    eax, [edx + 0x24]
        push   0xc001000c
        push   edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x161], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_419A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0x161]
        test   edx, edx
        jne    loc_1_3864
loc_1_388D:
        cmp    byte ptr [esi + 0x11d], 0
        je     loc_1_38AE
loc_1_3896:
        mov    edi, 0x64
loc_1_389B:
        push   0x64
        call   sub_1_4250
        dec    edi
        jne    loc_1_389B
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_3896
loc_1_38AE:
        mov    byte ptr [esi + 0x15c], 0x1b
        push   esi
        call   sub_1_01D0
        mov    edi, dword ptr [esi + 0x28]
        cmp    al, 1
        sbb    eax, eax
        xor    ebx, ebx
        and    eax, 0xc0000001
        mov    byte ptr [esi + 0x13e], bl
        mov    dword ptr [ebp - 4], eax
        test   edi, edi
        je     loc_1_3914
loc_1_38D6:
        inc    dword ptr [edi + 0x1c]
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_38ED
        push   ebx
        mov    eax, dword ptr [edi]
        push   ebx
        push   0x40010007
        push   eax
        call   sub_1_41BE
loc_1_38ED:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   0x40010005
        push   ecx
        call   sub_1_41BE
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_41C4
        mov    eax, dword ptr [edi + 0xc]
        dec    dword ptr [edi + 0x1c]
        mov    edi, eax
        test   eax, eax
        jne    loc_1_38D6
loc_1_3914:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        dec    dword ptr [esi + 0x30]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3930:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x13f], eax
        jne    loc_1_3A1D
        cmp    eax, 1
        je     loc_1_39B3
        cmp    eax, 2
        je     loc_1_39BB
        cmp    eax, 3
        je     loc_1_39C3
        mov    eax, dword ptr [ebp - 4]
loc_1_395D:
        cmp    eax, 0x103
        je     loc_1_397C
        push   eax
        mov    eax, dword ptr [esi + 0x143]
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_41A6
        mov    ecx, dword ptr [esi + 0x143]
        dec    dword ptr [ecx + 0x1c]
loc_1_397C:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_39FE
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_399D
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_399D
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_39FE
loc_1_399D:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_4232
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_39B3:
        push   esi
        call   sub_1_3700
        jmp    loc_1_395D
loc_1_39BB:
        push   esi
        call   sub_1_3780
        jmp    loc_1_395D
loc_1_39C3:
        mov    ecx, dword ptr [esi + 0x30]
        xor    edx, edx
        inc    ecx
        mov    eax, dword ptr [esi + 0x38]
        mov    dword ptr [esi + 0x30], ecx
        cmp    eax, edx
        je     loc_1_39E3
        lea    edi, [esi + 0x5d]
loc_1_39D6:
        cmp    dword ptr [edi], 0
        jne    loc_1_39F2
        add    edi, 4
        inc    edx
        cmp    edx, eax
        jb     loc_1_39D6
loc_1_39E3:
        dec    ecx
        push   esi
        mov    dword ptr [esi + 0x30], ecx
        call   sub_1_37E0
        jmp    loc_1_395D
loc_1_39F2:
        dec    ecx
        pop    edi
        mov    dword ptr [esi + 0x30], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_39FE:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_3A1D
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_3A1D
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_3A1D
        push   esi
        call   sub_1_26C0
loc_1_3A1D:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3A30:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0x1c]
        mov    esi, dword ptr [ebx + 8]
        call   sub_1_14C0
        test   edi, edi
        jne    loc_1_3A4B
        lea    edi, [ebx + 0x49]
loc_1_3A4B:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_3A5B
        mov    esi, 0xc001000d
        jmp    loc_1_3AA0
loc_1_3A5B:
        mov    dword ptr [edi + 4], ebx
        inc    dword ptr [ebx + 0x1c]
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffa
        add    eax, 0xa
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi], 0
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_3A8D
        mov    dword ptr [esi + 0x171], edi
        mov    dword ptr [esi + 0x16d], edi
        jmp    loc_1_3A9B
loc_1_3A8D:
        mov    eax, dword ptr [esi + 0x171]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x171], edi
loc_1_3A9B:
        mov    esi, 0x103
loc_1_3AA0:
        call   sub_1_14F0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_3AB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebx + 8]
        call   sub_1_14C0
        test   edi, edi
        jne    loc_1_3ACB
        lea    edi, [ebx + 0x21]
loc_1_3ACB:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_3ADB
        mov    esi, 0xc001000d
        jmp    loc_1_3B1D
loc_1_3ADB:
        mov    dword ptr [edi + 4], ebx
        inc    dword ptr [ebx + 0x1c]
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi], 0
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_3B0A
        mov    dword ptr [esi + 0x171], edi
        mov    dword ptr [esi + 0x16d], edi
        jmp    loc_1_3B18
loc_1_3B0A:
        mov    eax, dword ptr [esi + 0x171]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x171], edi
loc_1_3B18:
        mov    esi, 0x103
loc_1_3B1D:
        call   sub_1_14F0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_3B30:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 0x1c]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_3B40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_14C0
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x30]
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_3C00
        cmp    dword ptr [esi + 0x175], 0
        jne    loc_1_3C00
loc_1_3B6B:
        mov    edi, dword ptr [esi + 0x16d]
        test   edi, edi
        je     loc_1_3BD6
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x16d], eax
        cmp    dword ptr [esi + 0x171], edi
        jne    loc_1_3B8F
        mov    dword ptr [esi + 0x171], 0
loc_1_3B8F:
        mov    dword ptr [esi + 0x175], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     loc_1_3BAA
        cmp    eax, 0xa
        je     loc_1_3BAA
        push   esi
        call   sub_1_3CC0
        jmp    loc_1_3BB0
loc_1_3BAA:
        push   esi
        call   sub_1_3D30
loc_1_3BB0:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     loc_1_3BCA
        cmp    ecx, 4
        je     loc_1_3BCA
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_41AC
loc_1_3BCA:
        dec    dword ptr [ebx + 0x1c]
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_3B6B
loc_1_3BD6:
        mov    dword ptr [esi + 0x175], 0
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_3C00
        push   1
        dec    dword ptr [esi + 0x30]
        push   esi
        call   sub_1_3930
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_3C00:
        mov    edi, dword ptr [esi + 0x2c]
        test   edi, edi
        je     loc_1_3C66
        xor    ebx, ebx
loc_1_3C09:
        cmp    dword ptr [edi + 0x1c], 0
        jbe    loc_1_3C16
        mov    edi, dword ptr [edi + 0xc]
        mov    ebx, edi
        jmp    loc_1_3C50
loc_1_3C16:
        mov    eax, dword ptr [esi + 0x15d]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_3C27
        push   esi
        call   sub_1_2A80
loc_1_3C27:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_4194
        lea    ecx, [edi + 0xc]
        test   ebx, ebx
        mov    eax, dword ptr [ecx]
        je     loc_1_3C3F
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_3C42
loc_1_3C3F:
        mov    dword ptr [esi + 0x2c], eax
loc_1_3C42:
        mov    eax, edi
        push   0
        mov    edi, dword ptr [ecx]
        push   0x75
        push   eax
        call   sub_1_425C
loc_1_3C50:
        test   edi, edi
        jne    loc_1_3C09
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_1_3C66
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_3C66
        push   esi
        call   sub_1_0150
loc_1_3C66:
        dec    dword ptr [esi + 0x30]
        jne    loc_1_3C93
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_3C83
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_3C83
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_3C93
loc_1_3C83:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_4232
        jmp    loc_1_3CB2
loc_1_3C93:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_3CB2
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_3CB2
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_3CB2
        push   esi
        call   sub_1_26C0
loc_1_3CB2:
        call   sub_1_14F0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_3CC0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0x14b]
        mov    edi, dword ptr [eax + 0xc]
        test   edi, 0x24
        je     loc_1_3CE1
        call   sub_1_07A0
        jmp    loc_1_3CE6
loc_1_3CE1:
        call   sub_1_3D30
loc_1_3CE6:
        test   edi, 0x26
        lea    eax, [esi + 0x15b]
        je     loc_1_3CF9
        or     byte ptr [eax], 8
        jmp    loc_1_3CFC
loc_1_3CF9:
        and    byte ptr [eax], 0xf7
loc_1_3CFC:
        test   edi, 0x20
        je     loc_1_3D09
        or     byte ptr [eax], 0x10
        jmp    loc_1_3D0C
loc_1_3D09:
        and    byte ptr [eax], 0xef
loc_1_3D0C:
        test   edi, 0x28
        je     loc_1_3D19
        or     byte ptr [eax], 4
        jmp    loc_1_3D1C
loc_1_3D19:
        and    byte ptr [eax], 0xfb
loc_1_3D1C:
        push   esi
        call   sub_1_0780
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3D30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_05E0
        push   esi
        call   sub_1_07E0
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3D50:
        mov    al, byte ptr [0x43e8]         ; -> obj1:0x43E8
        cmp    al, 0
        jne    loc_1_3D75
        inc    byte ptr [0x43e8]             ; -> obj1:0x43E8
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_3D77
        push   0
        push   0
        call   sub_2_0D70
        cmp    eax, 0
        jne    loc_1_3D77
loc_1_3D75:
        clc    
        ret    
loc_1_3D77:
        stc    
        ret    
SETP3_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_3D88
        jmp    dword ptr [eax*4 + 0x3d8a]    ; -> obj1:0x3D8A
loc_1_3D88:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_3D50
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D88
        dd  loc_1_3D50
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3E00:
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
        call   sub_1_3E30
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_3E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_3ED0
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_3E70
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_3E65
        cmp    al, 0xff
        jne    loc_1_3E65
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_3E65
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_3ED7
loc_1_3E65:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_3ED7
        db  08Bh, 0FFh
loc_1_3E70:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_3EC0
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_3E96
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_3E9D
loc_1_3E96:
        mov    dword ptr [ebp - 0x10], 1
loc_1_3E9D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_3EB0
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_3EDA
        db  08Dh, 064h, 024h, 000h
loc_1_3EB0:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_3EDA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3EC0:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_3EDA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3ED0:
        mov    dword ptr [ebp - 4], 0x20
loc_1_3ED7:
        mov    esi, dword ptr [ebp + 8]
loc_1_3EDA:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_3FD1
loc_1_3EE5:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    loc_1_3F3D
        test   al, 2
        je     loc_1_3F29
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_1E70
        or     al, al
        je     loc_1_3F29
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0xc]
        test   dword ptr [ecx + eax*4], edx
        jne    loc_1_3F3D
loc_1_3F29:
        test   byte ptr [ebp - 4], 2
        je     loc_1_3F34
        test   bl, 4
        jne    loc_1_3F3D
loc_1_3F34:
        test   bl, 0x20
        je     loc_1_3FC6
loc_1_3F3D:
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
        call   sub_1_41B2
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_3FC6
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_3F7F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_3F7F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_3F90
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_3F96
        db  08Bh, 0FFh
loc_1_3F90:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_3F96:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_3FA5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_41B8
loc_1_3FA5:
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
        call   sub_1_425C
        mov    edi, ebx
        jmp    loc_1_3FC9
loc_1_3FC6:
        mov    edi, dword ptr [edi + 0x10]
loc_1_3FC9:
        or     edi, edi
        jne    loc_1_3EE5
loc_1_3FD1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3FE0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3FF0
        pop    ebp
        ret    4
sub_1_3FF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_406E
loc_1_4000:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_4067
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_41B8
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_4067
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_402C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_402C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_4040
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_4046
        db  005h, 000h, 000h, 000h, 000h
loc_1_4040:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_4046:
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
        call   sub_1_425C
        mov    edi, ebx
        jmp    loc_1_406A
loc_1_4067:
        mov    edi, dword ptr [edi + 0x10]
loc_1_406A:
        or     edi, edi
        jne    loc_1_4000
loc_1_406E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4080:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_4095
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4095:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_40D1
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_40C2
        cmp    bl, 0xff
        jne    loc_1_40C2
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_40C2
        test   al, 8
        je     loc_1_40BB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_40BB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_40C2:
        test   al, 4
        jne    loc_1_40CA
        test   al, 2
        je     loc_1_40E9
loc_1_40CA:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_40D1:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_40E9
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_40E9
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_40E9:
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
sub_1_416A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_4170:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_4176:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_417C:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_4182:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_4188:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_4194:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_419A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_41A6:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_41AC:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_41B2:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_41B8:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_41BE:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_41C4:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_41DC:
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
sub_1_4232:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_4238:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_423E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_4244:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_4250:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_4256:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_425C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_4280:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_42A4:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_42C8:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_42D4:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_42DA:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_42E0:
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
sub_1_4328:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_433C:
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
sub_1_435C:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_437C
        or     ecx, ecx
        je     loc_1_4392
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_4390
loc_1_437C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_4390
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_4390:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_4392:
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
        db  'SETP3   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  SETP3_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4450:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_4454:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4458:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_445C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4460:
        add    byte ptr [eax], al
        rol    eax, 0x81
        rol    dword ptr [eax + 1], 1
        ret    
        db  0C0h, 003h, 080h, 002h, 041h, 0C2h, 001h, 0C6h, 0C0h, 006h, 080h, 007h, 041h, 0C7h, 000h, 005h
        db  0C1h, 0C5h, 081h, 0C4h, 040h, 004h, 001h, 0CCh, 0C0h, 00Ch, 080h, 00Dh, 041h, 0CDh, 000h, 00Fh
        db  0C1h, 0CFh, 081h, 0CEh, 040h, 00Eh, 000h, 00Ah, 0C1h, 0CAh, 081h, 0CBh, 040h, 00Bh, 001h, 0C9h
        db  0C0h, 009h, 080h, 008h, 041h, 0C8h, 001h, 0D8h, 0C0h, 018h, 080h, 019h, 041h, 0D9h, 000h, 01Bh
        db  0C1h, 0DBh, 081h, 0DAh, 040h, 01Ah, 000h, 01Eh, 0C1h, 0DEh, 081h, 0DFh, 040h, 01Fh, 001h, 0DDh
        db  0C0h, 01Dh, 080h, 01Ch, 041h, 0DCh, 000h, 014h, 0C1h, 0D4h, 081h, 0D5h, 040h, 015h, 001h, 0D7h
        db  0C0h, 017h, 080h, 016h, 041h, 0D6h, 001h, 0D2h, 0C0h, 012h, 080h, 013h, 041h, 0D3h, 000h, 011h
        db  0C1h, 0D1h, 081h, 0D0h, 040h, 010h, 001h, 0F0h, 0C0h, 030h, 080h, 031h, 041h, 0F1h, 000h, 033h
        db  0C1h, 0F3h, 081h, 0F2h, 040h, 032h, 000h, 036h, 0C1h, 0F6h, 081h, 0F7h, 040h, 037h, 001h, 0F5h
        db  0C0h, 035h, 080h, 034h, 041h, 0F4h, 000h, 03Ch, 0C1h, 0FCh, 081h, 0FDh, 040h, 03Dh, 001h, 0FFh
        db  0C0h, 03Fh, 080h, 03Eh, 041h, 0FEh, 001h, 0FAh, 0C0h, 03Ah, 080h, 03Bh, 041h, 0FBh, 000h, 039h
        db  0C1h, 0F9h, 081h, 0F8h, 040h, 038h, 000h, 028h, 0C1h, 0E8h, 081h, 0E9h, 040h, 029h, 001h, 0EBh
        db  0C0h, 02Bh, 080h, 02Ah, 041h, 0EAh, 001h, 0EEh, 0C0h, 02Eh, 080h, 02Fh, 041h, 0EFh, 000h, 02Dh
        db  0C1h, 0EDh, 081h, 0ECh, 040h, 02Ch, 001h, 0E4h, 0C0h, 024h, 080h, 025h, 041h, 0E5h, 000h, 027h
        db  0C1h, 0E7h, 081h, 0E6h, 040h, 026h, 000h, 022h, 0C1h, 0E2h, 081h, 0E3h, 040h, 023h, 001h, 0E1h
        db  0C0h, 021h, 080h, 020h, 041h, 0E0h, 001h, 0A0h, 0C0h, 060h, 080h, 061h, 041h, 0A1h, 000h, 063h
        db  0C1h, 0A3h, 081h, 0A2h, 040h, 062h, 000h, 066h, 0C1h, 0A6h, 081h, 0A7h, 040h, 067h, 001h, 0A5h
        db  0C0h, 065h, 080h, 064h, 041h, 0A4h, 000h, 06Ch, 0C1h, 0ACh, 081h, 0ADh, 040h, 06Dh, 001h, 0AFh
        db  0C0h, 06Fh, 080h, 06Eh, 041h, 0AEh, 001h, 0AAh, 0C0h, 06Ah, 080h, 06Bh, 041h, 0ABh, 000h, 069h
        db  0C1h, 0A9h, 081h, 0A8h, 040h, 068h, 000h, 078h, 0C1h, 0B8h, 081h, 0B9h, 040h, 079h, 001h, 0BBh
        db  0C0h, 07Bh, 080h, 07Ah, 041h, 0BAh, 001h, 0BEh, 0C0h, 07Eh, 080h, 07Fh, 041h, 0BFh, 000h, 07Dh
        db  0C1h, 0BDh, 081h, 0BCh, 040h, 07Ch, 001h, 0B4h, 0C0h, 074h, 080h, 075h, 041h, 0B5h, 000h, 077h
        db  0C1h, 0B7h, 081h, 0B6h, 040h, 076h, 000h, 072h, 0C1h, 0B2h, 081h, 0B3h, 040h, 073h, 001h, 0B1h
        db  0C0h, 071h, 080h, 070h, 041h, 0B0h, 000h, 050h, 0C1h, 090h, 081h, 091h, 040h, 051h, 001h, 093h
        db  0C0h, 053h, 080h, 052h, 041h, 092h, 001h, 096h, 0C0h, 056h, 080h, 057h, 041h, 097h, 000h, 055h
        db  0C1h, 095h, 081h, 094h, 040h, 054h, 001h, 09Ch, 0C0h, 05Ch, 080h, 05Dh, 041h, 09Dh, 000h, 05Fh
        db  0C1h, 09Fh, 081h, 09Eh, 040h, 05Eh, 000h, 05Ah, 0C1h, 09Ah, 081h, 09Bh, 040h, 05Bh, 001h, 099h
        db  0C0h, 059h, 080h, 058h, 041h, 098h, 001h, 088h, 0C0h, 048h, 080h, 049h, 041h, 089h, 000h, 04Bh
        db  0C1h, 08Bh, 081h, 08Ah, 040h, 04Ah, 000h, 04Eh, 0C1h, 08Eh, 081h, 08Fh, 040h, 04Fh, 001h, 08Dh
        db  0C0h, 04Dh, 080h, 04Ch, 041h, 08Ch, 000h, 044h, 0C1h, 084h, 081h, 085h, 040h, 045h, 001h, 087h
        db  0C0h, 047h, 080h, 046h, 041h, 086h, 001h, 082h, 0C0h, 042h, 080h, 043h, 041h, 083h, 000h, 041h
        db  0C1h, 081h, 081h, 080h, 040h, 040h
loc_1_4660:
        inc    eax
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_466C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4670:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4674:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4678:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_467C:
        inc    ebp
        outsb  dx, byte ptr [esi]
        push   0x65636e61
        and    byte ptr fs:[eax + 0x61], dl
        jb     loc_1_46EA
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [eax + 0x6f], dl
        jb     loc_1_4706
        add    byte ptr [eax], al
loc_1_4694:
        push   eax
        push   ebx
        das    
        xor    ah, byte ptr [eax]
        inc    edx
        imul   ebp, dword ptr [0x65726964], 0x6f697463
        outsb  dx, byte ptr [esi]
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x52], bl
        pop    ebp
        add    byte ptr [edi + ecx*2 + 0x53], dl
        dec    eax
        dec    ecx
        inc    edx
        inc    ecx
        and    byte ptr [eax], ah
        inc    edx
        imul   ebp, dword ptr [0x65726964], 0x6f697463
        outsb  dx, byte ptr [esi]
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x52], bl
        pop    ebp
        add    byte ptr [edx + 0x69], al
        sub    eax, 0x65726964
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x52], bl
        pop    ebp
        add    byte ptr [eax], al
loc_1_46DC:
        push   ebp
        outsb  dx, byte ptr [esi]
        imul   esp, dword ptr [ecx + ebp*2 + 0x72], 0x69746365
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        popal  
        insb   byte ptr es:[edi], dx
loc_1_46EA:
        and    byte ptr [ebx + 0x52], bl
        pop    ebp
        add    byte ptr [eax], al
loc_1_46F0:
        push   ebp
        outsb  dx, byte ptr [esi]
        imul   esp, dword ptr [ecx + ebp*2 + 0x72], 0x69746365
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x46], bl
        pop    ebp
        add    byte ptr [eax], al
loc_1_4704:
        add    byte ptr [eax], al
loc_1_4706:
        add    byte ptr [eax], al
loc_1_4708:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_470C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4710:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4714:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4718:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_471C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4720:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4724:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_47E4:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_4820:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4874:
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
loc_1_48F0:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        push   esp
        push   eax
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_4958:
        push   ebx
        inc    ebp
        push   esp
        push   eax
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        push   eax
        push   eax
        pop    edi
        dec    ebp
        outsd  dx, dword ptr [esi]
        db  064h, 065h, 000h
loc_1_4968:
        push   eax
        push   eax
        pop    edi
        dec    ebp
        outsd  dx, dword ptr [esi]
        add    byte ptr gs:[ecx + ebp*2 + 0x73], al
        popal  
        bound  ebp, qword ptr [ebp + 0x5f]
        dec    esp
        imul   ebp, dword ptr [esi + 0x6b], 0
loc_1_4980:
        inc    esp
        imul   esi, dword ptr [ebx + 0x61], 0x5f656c62
        dec    esp
        imul   ebp, dword ptr [esi + 0x6b], 0
        dec    ebp
        imul   ebp, dword ptr [esi + 0x69], 0x6f6c425f
        arpl   word ptr [ebx], bp
        add    byte ptr [ebp + 0x69], cl
        outsb  dx, byte ptr [esi]
        imul   ebx, dword ptr [edi + 0x42], 0x6b636f6c
        add    byte ptr [eax], al
        dec    esp
        push   eax
        push   esp
        db  000h
loc_1_49AC:
        dec    esp
        push   eax
        push   esp
        add    byte ptr [ebp + 0x41], cl
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
loc_1_49C0:
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
loc_1_49E8:
        push   ebx
        imul   ebp, dword ptr [ecx + ebp*2 + 0x63], 0x45206d6f
        je     0x4a5b
        jb     loc_1_4A64
        je     0x4a19
        push   eax
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x65], bp
        je     sub_1_4A20
        inc    ecx
        popal  
        jo     0x4a79
        jb     loc_1_4A36
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
sub_1_4A10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4A14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4A18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4A1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4A20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4A24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4A36:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4A64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4AA4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4AA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4AAC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4AB0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4AB4:
        add    byte ptr [eax], al
loc_1_4AB6:
        add    byte ptr [eax], al
sub_1_4AB8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4ABC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4AC0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4AC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1f30  R-X
        db  055h, 08Ah, 00Dh
        dd  obj1:0x4668              ; fixup
        db  08Bh, 0ECh, 08Bh, 044h, 024h, 00Ch, 050h, 051h, 0E8h, 0ECh, 00Ch, 000h, 000h, 05Dh, 03Ch, 001h
        db  01Ah, 0C0h, 0F6h, 0D8h, 0C2h, 008h, 000h, 08Bh, 0FFh
sub_2_0020:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x100
        mov    dword ptr [eax + 0x114], 0x8000
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax + 0x110], esi
loc_2_0041:
        lea    edi, [ebp - 0x100]
        mov    eax, 0xaa55ff00
        mov    ecx, 0x40
        push   0x100
        rep stosd dword ptr es:[edi], eax
        lea    eax, [ebp - 0x100]
        push   eax
        push   esi
        call   sub_1_03E0
loc_2_0065:
        test   al, al
        je     loc_2_00BE
        lea    edi, [ebp - 0x100]
        xor    eax, eax
        mov    ecx, 0x40
        push   0x100
        rep stosd dword ptr es:[edi], eax
        push   eax
        lea    eax, [ebp - 0x100]
        push   eax
        call   sub_1_04C0
loc_2_008A:
        test   al, al
        je     loc_2_00C8
        lea    eax, [ebp - 0x100]
loc_2_0094:
        cmp    dword ptr [eax], 0xaa55ff00
        jne    loc_2_00D2
        add    eax, 4
        lea    ecx, [ebp]
        cmp    eax, ecx
        jb     loc_2_0094
        add    esi, 0x100
        cmp    esi, 0x8000
        jb     loc_2_0041
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_00BE:
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_00C8:
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_00D2:
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0670
loc_2_00F1:
        push   0x50
        push   0xe
        call   sub_1_4AB0
        push   0xa0
        push   4
        call   sub_1_4AB0
        push   0
        push   0xd
        call   sub_1_4AB0
        push   0x20
        push   0xc
        call   sub_1_4AB0
        push   0xc
        push   1
        call   sub_1_4AB0
        push   0x80
        push   2
        call   sub_1_4AB0
        push   0xc
        push   3
        call   sub_1_4AB0
        push   0x3c
        mov    bl, 0x14
        push   5
        call   sub_1_4AB0
        push   0
        push   6
        call   sub_1_4AB0
loc_2_0153:
        push   0x64
        call   sub_1_4250
loc_2_015A:
        dec    bl
        jne    loc_2_0153
        push   0xff
        push   7
        call   sub_1_4AB0
        push   0x41
        push   0
        call   sub_1_4AB0
        push   0xc
        mov    bl, 0x14
        push   7
        call   sub_1_4AB0
        push   1
        push   0
        call   sub_1_4AB0
loc_2_018B:
        push   0x64
        call   sub_1_4250
loc_2_0192:
        dec    bl
        jne    loc_2_018B
        lea    eax, [ebp - 1]
        push   eax
        push   0
        call   sub_1_4AB8
        test   byte ptr [ebp - 1], 1
        jne    loc_2_01B2
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_01B2:
        push   0x37
        push   0xa
        call   sub_1_4AB0
        push   0xa
        push   0
        call   sub_1_4AB0
        mov    byte ptr [esi + 0x11c], 1
        push   0
        push   0xf
        call   sub_1_4AB0
        push   0
        push   0
        call   sub_1_4AB0
        push   0x48
        push   0xe
        call   sub_1_4AB0
        push   0
        push   0xd
        call   sub_1_4AB0
        push   0
        push   0xc
        call   sub_1_4AB0
        push   0xff
        push   7
        call   sub_1_4AB0
        push   0x21
        push   0
        call   sub_1_4AB0
        push   0
        mov    bl, 0xff
        push   0xa
        call   sub_1_4AB0
        push   0
        push   0xb
        call   sub_1_4AB0
loc_2_022C:
        dec    bl
        je     loc_2_0242
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4AB8
        test   byte ptr [ebp - 1], 0x80
        je     loc_2_022C
loc_2_0242:
        push   2
        push   0xd
        call   sub_1_4AB0
        push   0x22
        push   0
        call   sub_1_4AB0
        push   esi
        call   sub_2_0020
        push   esi
        test   al, al
        jne    loc_2_0270
        call   sub_1_0670
loc_2_0266:
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0270:
        call   sub_1_0670
loc_2_0275:
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_0280:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
loc_2_0289:
        mov    dl, byte ptr [eax + 0x27]     ; -> obj1:0x4A4B
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax + 0x109], dl
        jb     loc_2_0289
        cmp    byte ptr [ecx + 0x104], 0
        jne    loc_2_031A
        cmp    byte ptr [ecx + 0x105], 0
        jne    loc_2_031A
        cmp    byte ptr [ecx + 0x106], 0
        jne    loc_2_031A
        cmp    byte ptr [ecx + 0x107], 0
        jne    loc_2_031A
        cmp    byte ptr [ecx + 0x108], 0
        jne    loc_2_031A
        cmp    byte ptr [ecx + 0x109], 0
        jne    loc_2_031A
        mov    al, byte ptr [ecx + 0x10a]
        mov    dl, byte ptr [ecx + 0x10b]
        mov    byte ptr [ecx + 0x104], al
        mov    byte ptr [ecx + 0x105], dl
        mov    al, byte ptr [ecx + 0x10c]
        mov    dl, byte ptr [ecx + 0x10d]
        mov    byte ptr [ecx + 0x106], al
        mov    byte ptr [ecx + 0x107], dl
        mov    al, byte ptr [ecx + 0x10e]
        mov    dl, byte ptr [ecx + 0x10f]
        mov    byte ptr [ecx + 0x108], al
        mov    byte ptr [ecx + 0x109], dl
loc_2_031A:
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_0320:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        push   5
        call   sub_2_0B80
        call   sub_1_4AAC
        mov    si, word ptr [ebp + 8]
loc_2_033C:
        call   sub_2_0AE0
        call   sub_1_0930
loc_2_0346:
        call   sub_1_0980
loc_2_034B:
        call   sub_1_0930
loc_2_0350:
        mov    al, byte ptr [0]              ; -> loc_1_4660
        push   eax
        call   sub_1_4AC0
        push   0x21
        push   0
        call   sub_1_4AB0
        lea    eax, [ebp - 1]
        push   eax
        push   0
        call   sub_1_4AB8
        cmp    byte ptr [ebp - 1], 0x21
        mov    eax, dword ptr [0]            ; -> sub_1_4AB0
        je     loc_2_038D
        cmp    byte ptr [ebp - 1], 0x23
        je     loc_2_038D
        call   sub_2_0A70
        jmp    loc_2_048B
loc_2_038D:
        push   0x61
        push   0
        call   eax
        lea    eax, [ebp - 1]
        push   eax
        push   0
        call   sub_1_4AB8
        cmp    byte ptr [ebp - 1], 0x61
        mov    eax, dword ptr [0]            ; -> sub_1_4AB0
        je     loc_2_03BA
        cmp    byte ptr [ebp - 1], 0x63
        je     loc_2_03BA
        call   sub_2_0A70
        jmp    loc_2_048B
loc_2_03BA:
        push   0x21
        push   0
        call   eax
        push   0x48
        push   0xe
        call   sub_1_4AB0
        push   0
        push   0xa
        call   sub_1_4AB0
        push   0
        push   0xb
        call   sub_1_4AB0
        push   0xc
        push   0xc
        call   sub_1_4AB0
        push   2
        push   0xd
        call   sub_1_4AB0
        push   0xc
        push   3
        call   sub_1_4AB0
        push   0xc
        push   1
        call   sub_1_4AB0
        push   0x80
        push   2
        call   sub_1_4AB0
        push   0xff
        push   7
        mov    di, si
        call   sub_1_4AB0
        push   0
        push   0xf
        call   sub_1_4AB0
        call   sub_2_0910
        cmp    si, 5
        ja     loc_2_048B
loc_2_0438:
        push   edi
        call   sub_2_0B80
        call   sub_1_4A10
        test   al, al
        mov    eax, dword ptr [0]            ; -> sub_1_4AAC
        jne    loc_2_0483
        call   eax
        call   sub_1_0930
loc_2_0454:
        mov    al, byte ptr [0]              ; -> loc_1_4660
        push   eax
        call   sub_1_4AC0
        push   1
        call   sub_2_04D0
        test   al, al
        mov    eax, dword ptr [0]            ; -> sub_1_4A20
        je     loc_2_04A1
        call   eax
        call   sub_1_0930
loc_2_0477:
        mov    al, byte ptr [0]              ; -> loc_1_4660
        push   eax
        call   sub_1_4AC0
loc_2_0483:
        inc    di
        cmp    di, 5
        jbe    loc_2_0438
loc_2_048B:
        inc    bl
        cmp    bl, 3
        jb     loc_2_033C
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_04A1:
        call   sub_2_0620
        test   al, al
        je     loc_2_04B5
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_04B5:
        mov    word ptr [0], di              ; -> loc_1_4AB6
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_04D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x5c
        push   ebx
        push   esi
        pushf  
        cli    
        xor    cx, cx
loc_2_04DE:
        mov    eax, ecx
        inc    cx
        and    eax, 0xffff
        cmp    cx, 6
        mov    dl, byte ptr [eax + 0x27]     ; -> obj1:0x4A4B
        mov    byte ptr [ebp + eax - 0x56], dl
        mov    byte ptr [ebp + eax - 0x5c], dl
        jb     loc_2_04DE
        xor    cx, cx
        lea    eax, [ebp - 0x4e]
        mov    word ptr [ebp - 0x50], 0x40
loc_2_0507:
        mov    byte ptr [eax], cl
        inc    eax
        inc    cx
        cmp    cx, 0x40
        jb     loc_2_0507
        lea    eax, [ebp - 0xc]
        push   0x4e
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        lea    eax, [ebp - 0x5c]
        push   eax
        push   0
        call   sub_1_03E0
loc_2_052F:
        test   al, al
        jne    loc_2_053F
        popf   
        mov    al, 0xff
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_053F:
        push   0x40
        push   0xe
        call   sub_1_4AB0
        push   0x1f
        push   0xc
        call   sub_1_4AB0
        mov    al, byte ptr [ebp + 8]
        and    al, 3
        add    al, al
        or     al, 1
        push   eax
        push   0xd
        call   sub_1_4AB0
        push   0x4e
        push   5
        call   sub_1_4AB0
        push   0
        push   6
        call   sub_1_4AB0
        xor    si, si
        push   0
        push   4
        mov    ebx, 2
        call   sub_1_4AB0
        push   0x26
        push   0
        call   sub_1_4AB0
loc_2_0595:
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4AB8
        test   byte ptr [ebp - 1], bl
        jne    loc_2_05AF
        inc    si
        cmp    si, 0xffff
        jb     loc_2_0595
loc_2_05AF:
        cmp    si, 0xffff
        jne    loc_2_05C2
        popf   
        mov    al, 0xff
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_05C2:
        xor    si, si
loc_2_05C5:
        mov    eax, esi
        inc    si
        and    eax, 0xffff
        lea    ecx, [ebp + eax - 0xc]
        push   ecx
        push   6
        call   sub_1_4AB8
        cmp    si, 8
        jb     loc_2_05C5
        push   0x21
        push   0
        call   sub_1_4AB0
        cmp    word ptr [ebp - 0xc], 0x3b00
        jne    loc_2_0608
        cmp    dword ptr [ebp - 0xa], 0x3f3e3d3c
        jne    loc_2_0608
        popf   
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0608:
        popf   
        mov    al, 0xff
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0620:
        mov    byte ptr [0], 0               ; -> loc_1_4454
        push   esi
        push   edi
        pushf  
        sti    
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        add    dx, 2
        mov    al, 0x15
        out    dx, al
        xor    di, di
loc_2_063D:
        mov    si, 0x1f4
loc_2_0641:
        push   0x64
        call   sub_1_4250
loc_2_0648:
        dec    si
        jne    loc_2_0641
        cmp    byte ptr [0], 0               ; -> loc_1_4454
        jne    loc_2_0685
        xor    byte ptr [0], 2               ; -> loc_1_4660
        mov    al, byte ptr [0]              ; -> loc_1_4660
        push   eax
        call   sub_1_4AC0
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        add    dx, 2
        mov    al, 0x15
        out    dx, al
        inc    di
        cmp    di, 4
        jb     loc_2_063D
        popf   
        mov    al, 0xff
        pop    edi
        pop    esi
        ret    
loc_2_0685:
        popf   
        xor    al, al
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
sub_2_0690:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    al, byte ptr [esp + 8]
        pop    ebp
        lea    ecx, [eax*2 + 0x408]
        mov    dword ptr [0], ecx            ; -> loc_1_4A18
        mov    ax, word ptr [ecx]
        ret    4
        db  08Dh, 049h, 000h
sub_2_06B0:
        push   ebp
        mov    al, byte ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_2_0690
        mov    word ptr [0], ax              ; -> loc_1_4A1C
        test   ax, ax
        je     loc_2_06DF
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        in     al, dx
        mov    byte ptr [0], al              ; -> loc_1_4AB4
        add    dx, 2
        in     al, dx
        mov    byte ptr [0], al              ; -> obj1:0x4AB5
loc_2_06DF:
        mov    ax, word ptr [0]              ; -> loc_1_4A1C
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_06F0:
        cmp    word ptr [0], 0               ; -> loc_1_4A1C
        je     loc_2_073A
        pushfd 
        cli    
        cmp    byte ptr [0], 0               ; -> loc_1_4458
        je     loc_2_070A
        call   sub_2_0760
loc_2_070A:
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [0]              ; -> loc_1_4AB4
        out    dx, al
        cmp    word ptr [0], 1               ; -> loc_1_4AB6
        jne    loc_2_0728
        and    byte ptr [0], 0xdf            ; -> obj1:0x4AB5
loc_2_0728:
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, byte ptr [0]              ; -> obj1:0x4AB5
        add    dx, 2
        out    dx, al
        popfd  
loc_2_073A:
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_2_0740:
        mov    byte ptr [0], 1               ; -> loc_1_4458
        mov    eax, dword ptr [0]            ; -> loc_1_4A18
        mov    word ptr [eax], 1
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0760:
        mov    ax, word ptr [0]              ; -> loc_1_4A1C
        mov    ecx, dword ptr [0]            ; -> loc_1_4A18
        mov    byte ptr [0], 0               ; -> loc_1_4458
        mov    word ptr [ecx], ax
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0780:
        push   ebx
        push   bx
        xor    ebx, ebx
        pop    bx
        xor    bl, al
        sub    bh, bh
        shl    bx, 1
        mov    al, ah
        sub    ah, ah
        xor    ax, word ptr [ebx]            ; -> loc_1_4460
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
sub_2_07A0:
        push   ax
        push   ecx
        mov    ecx, 0x30
loc_2_07A8:
        loop   loc_2_07A8
        pop    ecx
        pop    ax
        ret    
        db  08Bh, 0FFh, 066h, 050h, 051h, 0B9h, 000h, 006h, 000h, 000h, 0E4h, 061h, 0E2h, 0FCh, 059h, 066h
        db  058h, 0C3h
sub_2_07C0:
        push   ax
        call   sub_2_07A0
        mov    al, 3
        out    dx, al
        call   sub_2_07A0
        mov    al, 2
        out    dx, al
        call   sub_2_07A0
        mov    al, 0
        out    dx, al
        pop    ax
        ret    
        db  08Dh, 049h, 000h
sub_2_07E0:
        push   ax
        call   sub_2_07A0
        mov    al, 0
        out    dx, al
        call   sub_2_07A0
        mov    al, 2
        out    dx, al
        call   sub_2_07A0
        mov    al, 3
        out    dx, al
        pop    ax
        ret    
        db  08Dh, 049h, 000h
sub_2_0800:
        push   ax
        call   sub_2_07A0
        mov    al, 1
        out    dx, al
        call   sub_2_07A0
        mov    al, 3
        out    dx, al
        call   sub_2_07A0
        mov    al, 1
        out    dx, al
        pop    ax
        ret    
        db  08Dh, 049h, 000h
sub_2_0820:
        push   ax
        call   sub_2_07A0
        mov    al, 0
        out    dx, al
        call   sub_2_07A0
        mov    al, 2
        out    dx, al
        call   sub_2_07A0
        mov    al, 0
        out    dx, al
        pop    ax
        ret    
        db  08Dh, 049h, 000h
sub_2_0840:
        call   sub_2_07A0
        mov    al, 1
        out    dx, al
        call   sub_2_07A0
        mov    al, 3
        out    dx, al
        call   sub_2_07A0
        inc    dx
        in     al, dx
        dec    dx
        mov    ah, al
        call   sub_2_07A0
        mov    al, 1
        out    dx, al
        call   sub_2_07A0
        and    ah, 8
        ror    ah, 3
        xor    ah, 1
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0880:
        shl    al, 1
        add    al, ah
        call   sub_2_07C0
        push   ecx
        mov    ecx, 8
loc_2_0890:
        shl    al, 1
        jae    loc_2_089C
        call   sub_2_0800
        jmp    loc_2_08A1
loc_2_089C:
        call   sub_2_0820
loc_2_08A1:
        loop   loc_2_0890
        pop    ecx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_08B0:
        push   ax
        push   ecx
        xor    bl, bl
        mov    ecx, 8
loc_2_08BA:
        shl    bl, 1
        call   sub_2_0840
        or     bl, ah
        loop   loc_2_08BA
        pop    ecx
        pop    ax
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_08D0:
        mov    al, 0x11
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        call   sub_2_07A0
        sub    dx, 2
        mov    al, 3
        out    dx, al
        call   sub_2_07A0
        add    dx, 2
        mov    al, 0x17
        out    dx, al
        sub    dx, 2
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0900:
        add    dx, 2
        mov    al, 0x15
        out    dx, al
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0910:
        push   ebx
        push   edi
        push   ax
        push   bx
        push   ecx
        push   dx
        push   edi
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        call   sub_2_08D0
        mov    ax, 0x100
        call   sub_2_0880
        call   sub_2_0840
        mov    edi, 0                        ; -> loc_1_4A24
        mov    ecx, 0x80
        xor    ax, ax
loc_2_0941:
        call   sub_2_08B0
        call   sub_2_0780
        mov    byte ptr [edi], bl
        cmp    ecx, 1
        je     loc_2_0959
        call   sub_2_0820
        jmp    loc_2_095E
loc_2_0959:
        call   sub_2_0800
loc_2_095E:
        inc    edi
        loop   loc_2_0941
        mov    word ptr [0], ax              ; -> loc_1_445C
        call   sub_2_0840
        call   sub_2_07E0
        mov    byte ptr [0], 0               ; -> loc_1_4AA8
        cmp    word ptr [0], 1               ; -> loc_1_445C
        je     loc_2_0989
        mov    byte ptr [0], 1               ; -> loc_1_4AA8
loc_2_0989:
        call   sub_2_0900
        pop    edi
        pop    dx
        pop    ecx
        pop    bx
        pop    ax
        pop    edi
        pop    ebx
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 066h, 053h, 032h, 0E4h, 086h, 0D8h, 0E8h, 0D5h, 0FEh
        db  0FFh, 0FFh, 0E8h, 000h, 0FEh, 0FFh, 0FFh, 0E8h, 08Bh, 0FEh, 0FFh, 0FFh, 0F6h, 0C4h, 001h, 075h
        db  027h, 051h, 0B9h, 008h, 000h, 000h, 000h, 0C0h, 0E3h, 001h, 073h, 007h, 0E8h, 036h, 0FEh, 0FFh
        db  0FFh, 0EBh, 005h, 0E8h, 04Fh, 0FEh, 0FFh, 0FFh, 0E2h, 0EDh, 059h, 0E8h, 067h, 0FEh, 0FFh, 0FFh
        db  0E8h, 002h, 0FEh, 0FFh, 0FFh, 0F8h, 0EBh, 001h, 0F9h, 066h, 05Bh, 0C3h, 08Dh, 064h, 024h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 053h, 056h, 066h, 050h, 066h, 053h, 051h, 066h, 052h
        db  056h, 066h, 08Bh, 015h
        dd  loc_1_4A1C               ; fixup
        db  0BEh
        dd  loc_1_4A24               ; fixup
        db  0FCh, 0B9h, 07Eh, 000h, 000h, 000h, 066h, 033h, 0C0h, 066h, 08Bh, 0D8h, 0ACh, 066h, 093h, 0E8h
        db  066h, 0FDh, 0FFh, 0FFh, 0E2h, 0F3h, 08Bh, 035h
        dd  loc_1_4A24               ; fixup
        db  066h, 089h, 046h, 07Eh, 0B9h, 080h, 000h, 000h, 000h, 032h, 0DBh, 0E8h, 09Eh, 0FEh, 0FFh, 0FFh
        db  0E8h, 079h, 0FDh, 0FFh, 0FFh, 0E8h, 074h, 0FDh, 0FFh, 0FFh, 0ACh, 0E8h, 05Eh, 0FFh, 0FFh, 0FFh
        db  072h, 00Dh, 0FEh, 0C3h, 0E2h, 0EAh, 0C6h, 005h
        dd  loc_1_4AA8               ; fixup
        db  000h, 0EBh, 007h, 0C6h, 005h
        dd  loc_1_4AA8               ; fixup
        db  001h, 0E8h, 0A3h, 0FEh, 0FFh, 0FFh, 05Eh, 066h, 05Ah, 059h, 066h, 05Bh, 066h, 058h, 05Eh, 05Bh
        db  0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0A70:
        mov    cx, word ptr [0]              ; -> loc_1_4A1C
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 0xa], cx
        push   esi
        mov    byte ptr [ebp - 3], 0
        mov    dx, word ptr [ebp - 0xa]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    byte ptr [ebp - 2], 0xb
        add    cx, 2
        mov    word ptr [ebp - 8], cx
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    esi, 0x1388
loc_2_0AA7:
        push   0x64
        call   sub_1_4250
loc_2_0AAE:
        dec    esi
        jne    loc_2_0AA7
        mov    byte ptr [ebp - 1], 0xec
        mov    ax, word ptr [0]              ; -> loc_1_4A1C
        add    ax, 2
        mov    word ptr [ebp - 6], ax
        mov    dx, word ptr [ebp - 6]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    esi, 0x7d0
loc_2_0AD0:
        push   0x64
        call   sub_1_4250
loc_2_0AD7:
        dec    esi
        jne    loc_2_0AD0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
sub_2_0AE0:
        mov    cx, word ptr [0]              ; -> loc_1_4A1C
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        mov    word ptr [ebp - 0x16], cx
        push   esi
        mov    byte ptr [ebp - 7], 0x30
        mov    dx, word ptr [ebp - 0x16]
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        mov    byte ptr [ebp - 6], 0x1d
        add    cx, 2
        mov    word ptr [ebp - 0x14], cx
        mov    dx, word ptr [ebp - 0x14]
        mov    al, byte ptr [ebp - 6]
        out    dx, al
        mov    byte ptr [ebp - 5], 0x15
        mov    word ptr [ebp - 0x12], cx
        mov    dx, word ptr [ebp - 0x12]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    byte ptr [ebp - 4], 0x1d
        mov    word ptr [ebp - 0x10], cx
        mov    dx, word ptr [ebp - 0x10]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    byte ptr [ebp - 3], 0x15
        mov    word ptr [ebp - 0xe], cx
        mov    dx, word ptr [ebp - 0xe]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    esi, 3
        call   sub_1_0980
loc_2_0B4C:
        mov    cx, word ptr [0]              ; -> loc_1_4A1C
        add    cx, 2
loc_2_0B57:
        mov    byte ptr [ebp - 2], 0x17
        mov    word ptr [ebp - 0xc], cx
        mov    dx, word ptr [ebp - 0xc]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    byte ptr [ebp - 1], 0x15
        mov    word ptr [ebp - 0xa], cx
        mov    dx, word ptr [ebp - 0xa]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        dec    esi
        jne    loc_2_0B57
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
sub_2_0B80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        and    eax, 0xffff
        shl    eax, 2
        lea    ecx, [eax + eax*8]
        mov    eax, dword ptr [eax + eax*8]  ; -> loc_1_4704
        mov    dword ptr [0], eax            ; -> loc_1_4AA4
        mov    eax, dword ptr [ecx + 4]      ; -> loc_1_4708
        mov    dword ptr [0], eax            ; -> sub_1_4AB8
        mov    eax, dword ptr [ecx + 8]      ; -> loc_1_470C
        mov    dword ptr [0], eax            ; -> sub_1_4AB0
        mov    eax, dword ptr [ecx + 0xc]    ; -> loc_1_4710
        mov    dword ptr [0], eax            ; -> sub_1_4ABC
        mov    eax, dword ptr [ecx + 0x10]   ; -> loc_1_4714
        mov    dword ptr [0], eax            ; -> sub_1_4AC4
        mov    eax, dword ptr [ecx + 0x14]   ; -> loc_1_4718
        mov    dword ptr [0], eax            ; -> sub_1_4AC0
        mov    eax, dword ptr [ecx + 0x18]   ; -> loc_1_471C
        pop    ebp
        mov    dword ptr [0], eax            ; -> sub_1_4A10
        mov    eax, dword ptr [ecx + 0x1c]   ; -> loc_1_4720
        mov    dword ptr [0], eax            ; -> sub_1_4AAC
        mov    eax, dword ptr [ecx + 0x20]   ; -> loc_1_4724
        mov    dword ptr [0], eax            ; -> sub_1_4A20
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0C00:
        push   dx
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x33
        out    dx, al
        add    dx, 2
        mov    al, 0x34
        out    dx, al
        sub    dx, 2
        nop    
        in     al, dx
        cmp    al, 0x33
        jne    loc_2_0C37
        mov    al, 0xcc
        out    dx, al
        add    dx, 2
        mov    al, 0x35
        out    dx, al
        sub    dx, 2
        nop    
        in     al, dx
        cmp    al, 0xcc
        jne    loc_2_0C37
        mov    ax, 0xffff
        jmp    loc_2_0C3A
loc_2_0C37:
        xor    ax, ax
loc_2_0C3A:
        push   ax
        mov    ax, 0x15
        add    dx, 2
        out    dx, al
        pop    ax
        pop    dx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0C50:
        push   dx
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    al, 0x33
        out    dx, al
        add    dx, 2
        mov    al, 0x94
        out    dx, al
        sub    dx, 2
        nop    
        in     al, dx
        cmp    al, 0x33
        jne    loc_2_0C87
        mov    al, 0xcc
        out    dx, al
        add    dx, 2
        mov    al, 0x95
        out    dx, al
        sub    dx, 2
        nop    
        in     al, dx
        cmp    al, 0xcc
        jne    loc_2_0C87
        mov    ax, 0xffff
        jmp    loc_2_0C8A
loc_2_0C87:
        xor    ax, ax
loc_2_0C8A:
        push   ax
        mov    ax, 0x15
        add    dx, 2
        out    dx, al
        pop    ax
        pop    dx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0CA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    esi, esi
        call   sub_1_0A50
loc_2_0CAF:
        call   sub_1_0930
loc_2_0CB4:
        mov    al, byte ptr [0]              ; -> loc_1_4660
        xor    ebx, ebx
        push   eax
        call   sub_1_4AC0
        push   esi
        push   8
        call   sub_1_13C0
loc_2_0CCA:
        lea    eax, [ebp - 1]
        push   eax
        push   8
        call   sub_1_0FC0
loc_2_0CD5:
        cmp    byte ptr [ebp - 1], bl
        jne    loc_2_0CEB
        inc    esi
        cmp    esi, 0x3e8
        jb     loc_2_0CCA
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0CEB:
        mov    al, 0xff
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pushf  
        cli    
        xor    bl, bl
        cmp    byte ptr [ebp + 8], 1
        sbb    edi, edi
        and    edi, 2
        inc    edi
        test   edi, edi
        jle    loc_2_0D4C
        mov    si, word ptr [ebp + 0xc]
loc_2_0D1D:
        cmp    byte ptr [ebp + 8], 0
        je     loc_2_0D28
        mov    bl, byte ptr [ebp + 8]
        dec    bl
loc_2_0D28:
        push   ebx
        call   sub_2_06B0
        test   ax, ax
        je     loc_2_0D3D
        push   esi
        call   sub_2_0320
        test   al, al
        je     loc_2_0D57
loc_2_0D3D:
        inc    bl
        call   sub_2_06F0
        xor    eax, eax
        mov    al, bl
        cmp    eax, edi
        jl     loc_2_0D1D
loc_2_0D4C:
        popf   
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_0D57:
        call   sub_2_0740
        popf   
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0D70:
        push   ebp
        mov    eax, 6
        mov    ebp, esp
        sub    esp, 0x10
        mov    word ptr [ebp - 0x10], ax
        push   ebx
        mov    word ptr [ebp - 0xe], ax
        push   esi
        xor    ebx, ebx
        mov    ecx, dword ptr [ebp + 0xc]
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        push   ecx
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_4958
        push   esi
        push   eax
        call   sub_1_416A
loc_2_0DA0:
        mov    dword ptr [0x46], esi         ; -> obj1:0x486E
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [4], eax            ; -> obj1:0x482C
        mov    byte ptr [0x4a], bl           ; -> obj1:0x4872
        mov    dword ptr [0x3e], ebx         ; -> obj1:0x4866
        mov    byte ptr [8], 3               ; -> obj1:0x4830
        mov    byte ptr [9], bl              ; -> obj1:0x4831
        mov    dword ptr [0xa], ebx          ; -> obj1:0x4832
        mov    dword ptr [0xe], 0            ; -> obj1:0x4836
        mov    dword ptr [0x12], 0           ; -> obj1:0x483A
        mov    dword ptr [0x16], 0           ; -> obj1:0x483E
        mov    dword ptr [0x1a], 0           ; -> obj1:0x4842
        mov    dword ptr [0x1e], 0           ; -> obj1:0x4846
        mov    edx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [0x22], 0           ; -> obj1:0x484A
        mov    dword ptr [0x26], 0           ; -> obj1:0x484E
        mov    dword ptr [0x2a], 0           ; -> obj1:0x4852
        mov    dword ptr [0x2e], 0           ; -> obj1:0x4856
        mov    dword ptr [0x32], 0           ; -> obj1:0x485A
        push   0x36
        mov    dword ptr [0x36], edx         ; -> obj1:0x485E
        push   8                             ; -> obj1:0x4830
        lea    edx, [ebp - 8]
        push   0                             ; -> obj1:0x4828
        mov    dword ptr [0x3a], ecx         ; -> obj1:0x4862
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0                             ; -> obj1:0x4828
        push   edx
        call   sub_1_4176
loc_2_0E61:
        cmp    dword ptr [ebp - 8], ebx
        je     loc_2_0E7B
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4170
loc_2_0E70:
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0E7B:
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0E90:
        push   ebp
        mov    eax, 8
        mov    ebp, esp
        sub    esp, 0x40
        mov    word ptr [ebp - 0x40], ax
        mov    ecx, 0xd
        mov    word ptr [ebp - 0x3e], ax
        push   esi
        mov    word ptr [ebp - 0x38], cx
        push   edi
        mov    word ptr [ebp - 0x36], cx
        mov    eax, 0xb
        mov    dword ptr [ebp - 0x3c], 0     ; -> loc_1_4968
        mov    dword ptr [ebp - 0x34], 0     ; -> loc_1_4980
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj1:0x499C
        mov    dword ptr [ebp - 0x24], 0     ; -> loc_1_49AC
        mov    word ptr [ebp - 0x30], ax
        mov    dword ptr [ebp - 0x1c], 0     ; -> loc_1_49C0
        mov    word ptr [ebp - 0x2e], ax
        mov    eax, 4
        cmp    byte ptr [0], 0               ; -> loc_1_4820
        mov    word ptr [ebp - 0x28], ax
        mov    word ptr [ebp - 0x26], ax
        mov    word ptr [ebp - 0x20], cx
        mov    word ptr [ebp - 0x1e], cx
        je     loc_2_0F0D
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0F0D:
        mov    esi, 0x10
        mov    eax, dword ptr [0]            ; -> obj1:0x4824
        push   eax
        lea    ecx, [ebp - 0xc]
        push   0
        push   0x2eb
        push   ecx
        call   sub_1_4256
loc_2_0F28:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_2_10CF
        mov    edi, dword ptr [ebp - 0xc]
        xor    eax, eax
        mov    ecx, 0xba
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    edi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x10]
        push   edi
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_42D4
loc_2_0F53:
        cmp    dword ptr [ebp - 4], 0
        push   0
        je     loc_2_0F76
        push   0x2eb
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_425C
loc_2_0F69:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0F76:
        lea    eax, [ebp - 0x40]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        call   sub_1_42DA
loc_2_0F8B:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0F9E
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [0], cx              ; -> loc_1_4450
loc_2_0F9E:
        push   0
        lea    eax, [ebp - 0x38]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_42DA
loc_2_0FB5:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0FC6
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [0], al              ; -> loc_1_466C
loc_2_0FC6:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_42DA
loc_2_0FDD:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0FEE
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [0], al              ; -> loc_1_4670
loc_2_0FEE:
        push   0
        lea    eax, [ebp - 0x28]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_42DA
loc_2_1005:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_1016
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [0], al              ; -> obj1:0x4668
loc_2_1016:
        push   0
        lea    eax, [ebp - 0x20]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_42DA
loc_2_102D:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_1039
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 4]
loc_2_1039:
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x14]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        call   sub_1_41DC
loc_2_104E:
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_2_1075
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_1075
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 0x14]
        add    eax, 0x104
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [eax + 4], cl
        mov    dl, byte ptr [edx + 5]
        mov    byte ptr [eax + 5], dl
loc_2_1075:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_42E0
loc_2_107E:
        mov    eax, dword ptr [ebp - 0xc]
        push   0
        push   0
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax + 0x38], 0xc
        mov    eax, dword ptr [ebp - 0xc]
        push   edi
        push   ecx
        mov    dword ptr [eax + 0x41], esi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_10E0
        test   eax, eax
        je     loc_2_10C1
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   0x2eb
        push   eax
        call   sub_1_425C
loc_2_10B4:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_10C1:
        call   sub_1_1520
loc_2_10C6:
        mov    byte ptr [0], 1               ; -> loc_1_4820
        xor    eax, eax
loc_2_10CF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_10E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], bl
        cmp    dword ptr [esi + 0x38], 0xc
        jbe    loc_2_110A
        mov    dword ptr [ebp - 8], 0xc000009a
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_110A:
        push   1
        push   0
        push   5
        mov    eax, dword ptr [esi + 4]
        push   5
        lea    ecx, [esi + 0x2bb]
        push   0                             ; -> loc_1_1620
        lea    edx, [ebp - 8]
        push   esi
        push   0                             ; -> loc_1_15C0
        push   eax
        push   ecx
        push   edx
        call   sub_1_423E
loc_2_1131:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_1139
        mov    bl, 1
loc_2_1139:
        lea    edi, [esi + 0x2d3]
        push   1
        push   0
        mov    eax, dword ptr [esi + 4]
        push   7
        push   7
        push   0                             ; -> loc_1_1620
        lea    ecx, [ebp - 8]
        push   esi
        push   0                             ; -> loc_1_15F0
        push   eax
        push   edi
        push   ecx
        call   sub_1_423E
loc_2_1160:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_116A
        mov    byte ptr [ebp - 1], 1
loc_2_116A:
        mov    ax, word ptr [0]              ; -> loc_1_4450
        push   eax
        push   esi
        call   0
        test   al, al
        jne    loc_2_11C1
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0xaa000004
        push   eax
        call   sub_1_4328
loc_2_118B:
        mov    dword ptr [ebp - 8], 0xc0010006
        add    esp, 0xc
        test   bl, bl
        je     loc_2_11A5
        lea    eax, [esi + 0x2bb]
        push   eax
        call   sub_1_4244
loc_2_11A5:
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_145C
        push   edi
        call   sub_1_4244
loc_2_11B5:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_11C1:
        test   bl, bl
        je     loc_2_11D1
        lea    eax, [esi + 0x2bb]
        push   eax
        call   sub_1_4244
loc_2_11D1:
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_11DD
        push   edi
        call   sub_1_4244
loc_2_11DD:
        mov    al, byte ptr [0]              ; -> loc_1_4454
        xor    edx, edx
        lea    edi, [ebp - 0x30]
        mov    ecx, 9
        mov    byte ptr [esi + 0x40], al
        xor    eax, eax
        mov    dx, word ptr [0]              ; -> loc_1_4A1C
        mov    dword ptr [esi + 0x3c], edx
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    edi, 1
        lea    eax, [ebp - 0x30]
        push   eax
        mov    dword ptr [ebp - 0x26], edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x1a], edi
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x4828
        lea    ecx, [esi + 4]
        mov    dword ptr [ebp - 0x16], edx
        mov    dword ptr [ebp - 0x12], 8
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        push   esi
        push   eax
        push   ecx
        call   sub_1_4182
loc_2_1230:
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_2_1447
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_125C
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   edi
        push   0xaa00000a
        push   ecx
        call   sub_1_4328
loc_2_1254:
        add    esp, 0x10
        jmp    loc_2_143E
loc_2_125C:
        mov    eax, 0                        ; -> loc_1_3B40
        mov    ecx, dword ptr [esi + 0x3c]
        mov    dword ptr [esi + 0x185], eax
        mov    dword ptr [esi + 0x17d], eax
        push   esi
        mov    dword ptr [esi + 0x181], esi
        mov    dword ptr [esi + 0x179], 0
        mov    dword ptr [esi + 0xff], ecx
        call   sub_2_00E0
        test   al, al
        jne    loc_2_12AF
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0xaa000004
        push   eax
        call   sub_1_4328
loc_2_12A0:
        mov    dword ptr [ebp - 8], 0xc0010006
        add    esp, 0xc
        jmp    loc_2_143E
loc_2_12AF:
        mov    edx, dword ptr [esi + 0x110]
        mov    edi, dword ptr [esi + 0x38]
        mov    cl, dh
        mov    eax, edi
        shl    eax, 8
        mov    dword ptr [esi + 0xed], edx
        add    eax, edx
        lea    ebx, [edi + ecx]
        mov    byte ptr [esi + 0xf9], cl
        mov    dword ptr [esi + 0xf1], eax
        mov    eax, dword ptr [esi + 0x114]
        mov    byte ptr [esi + 0xfa], bl
        add    edx, eax
        shr    eax, 8
        mov    dword ptr [esi + 0xf5], edx
        mov    dword ptr [esi + 0x57], 0xffffffff
        add    al, cl
        xor    ecx, ecx
        mov    byte ptr [esi + 0xfb], al
        mov    byte ptr [esi + 0x15b], cl
        mov    byte ptr [esi + 0x11d], cl
        test   edi, edi
        je     loc_2_1320
        lea    eax, [esi + 0x5d]
loc_2_1311:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        cmp    dword ptr [esi + 0x38], ecx
        ja     loc_2_1311
loc_2_1320:
        xor    ebx, ebx
        push   esi
        lea    edi, [esi + 0x14b]
        mov    byte ptr [esi + 0x1b1], bl
        mov    byte ptr [esi + 0x11b], bl
        call   sub_2_0280
        push   edi
        lea    eax, [esi + 0x104]
        push   esi
        mov    ecx, dword ptr [esi + 0x41]
        push   eax
        push   0                             ; -> loc_1_3B30
        push   0                             ; -> loc_1_3AB0
        push   0                             ; -> loc_1_3A30
        push   ecx
        call   sub_2_16B0
        test   al, al
        jne    loc_2_137D
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   0xaa000002
        push   eax
        call   sub_1_4328
loc_2_136E:
        mov    dword ptr [ebp - 8], 0xc0000001
        add    esp, 0xc
        jmp    loc_2_143E
loc_2_137D:
        mov    byte ptr [esi + 0x15c], 0x1b
        mov    dword ptr [esi + 0x20], 0     ; -> obj1:0x4828
        mov    eax, dword ptr [0x3e]         ; -> obj1:0x4866
        push   esi
        mov    dword ptr [esi + 0x24], eax
        mov    dword ptr [0x3e], esi         ; -> obj1:0x4866
        call   sub_1_0000
loc_2_139F:
        test   al, al
        jne    loc_2_13BF
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0xaa000004
        push   eax
        call   sub_1_4328
loc_2_13B3:
        mov    dword ptr [ebp - 8], 0xc0010006
        add    esp, 0xc
        jmp    loc_2_13FE
loc_2_13BF:
        movsx  eax, byte ptr [esi + 0x40]
        push   1
        push   0
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   eax
        lea    edx, [esi + 8]
        push   0                             ; -> loc_1_1620
        lea    eax, [ebp - 8]
        push   esi
        push   0                             ; -> loc_1_1560
        push   ecx
        push   edx
        push   eax
        call   sub_1_423E
loc_2_13E5:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_1468
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0xaa000005
        push   eax
        call   sub_1_4328
loc_2_13FB:
        add    esp, 0xc
loc_2_13FE:
        cmp    dword ptr [0x3e], esi         ; -> obj1:0x4866
        jne    loc_2_1410
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x3e], eax         ; -> obj1:0x4866
        jmp    loc_2_142C
loc_2_1410:
        mov    ecx, dword ptr [0x3e]         ; -> obj1:0x4866
        cmp    dword ptr [ecx + 0x24], esi
        je     loc_2_1423
loc_2_141B:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], esi
        jne    loc_2_141B
loc_2_1423:
        mov    eax, dword ptr [ecx + 0x24]
        mov    edx, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], edx
loc_2_142C:
        push   1
        push   0
        call   sub_1_4AB0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_2_17D0
loc_2_143E:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4188
loc_2_1447:
        push   0x21
        push   0
        call   sub_1_4AB0
        call   sub_1_4A20
        call   sub_2_06F0
loc_2_145C:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1468:
        xor    ebx, ebx
        mov    eax, 0                        ; -> loc_1_1620
        mov    dword ptr [esi + 0x189], ebx
        mov    dword ptr [esi + 0x18d], eax
        lea    ecx, [esi + 0x1a5]
        mov    dword ptr [esi + 0x191], esi
        mov    dword ptr [esi + 0x195], ebx
        mov    dword ptr [esi + 0x199], eax
        mov    eax, 0                        ; -> loc_1_2980
        push   0x7d0
        mov    dword ptr [esi + 0x19d], esi
        mov    dword ptr [esi + 0x1a1], eax
        push   ecx
        mov    dword ptr [ecx], ebx
        mov    dword ptr [esi + 0x1a9], eax
        mov    dword ptr [esi + 0x1ad], esi
        call   sub_1_4232
loc_2_14BD:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x1b2], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_2_14D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_14C0
loc_2_14DE:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0xffffffff
        lea    eax, [eax + edx*4]
loc_2_14F9:
        test   edx, edx
        je     loc_2_1508
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_2_14F9
        mov    dword ptr [ecx], edx
loc_2_1508:
        cmp    dword ptr [ecx], -1
        jne    loc_2_1520
        call   sub_1_14F0
loc_2_1512:
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_1520:
        mov    eax, dword ptr [0]            ; -> obj1:0x4824
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x75
        push   ecx
        call   sub_1_4256
loc_2_1533:
        test   eax, eax
        je     loc_2_1560
        push   0
        mov    esi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_4328
loc_2_154A:
        add    esp, 0xc
        call   sub_1_14F0
loc_2_1552:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_1560:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0x30]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_157C
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_2_157C
        push   0
        push   0xd
        call   sub_1_4AB0
loc_2_157C:
        mov    eax, dword ptr [esi + 0x28]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x28], ecx
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_2_1665
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        add    eax, 0x14
        mov    ecx, dword ptr [esi + 0x14b]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_1820
        test   al, al
        je     loc_2_1665
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x20]
        mov    dword ptr [eax + 0x10], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x71], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], bl
        mov    eax, 0xee
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        mov    dword ptr [esi + 0x15d], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_2_1636
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_2_1626
        cmp    dword ptr [esi + 0x16d], ebx
        jne    loc_2_1626
        cmp    dword ptr [esi + 0x2c], ebx
        je     loc_2_1636
loc_2_1626:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_4232
loc_2_1634:
        jmp    loc_2_1655
loc_2_1636:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_2_1655
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_2_1655
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_2_1655
        push   esi
        call   sub_1_26C0
loc_2_1655:
        call   sub_1_14F0
loc_2_165A:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_1665:
        push   0
        dec    dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp - 4]
        push   0x75
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x28], ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_425C
loc_2_167E:
        push   0
        mov    ecx, dword ptr [esi + 4]
        push   0xaa000002
        push   ecx
        call   sub_1_4328
loc_2_168E:
        add    esp, 0xc
        call   sub_1_14F0
loc_2_1696:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_16B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x4a0c]       ; -> obj1:0x4A0C
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_4256
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_16E4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_16E4:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_16FE
        mov    esi, 2
loc_2_16FE:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_4256
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_1732
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_17D0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_1732:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_4256
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_1761
        push   eax
        call   sub_2_17D0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_1761:
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
sub_2_17D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_17F0
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_425C
loc_2_17F0:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_1806
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_425C
loc_2_1806:
        push   0
        push   0x32
        push   esi
        call   sub_1_425C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1820:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_1839
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_1839:
        mov    eax, dword ptr [0x4a0c]       ; -> obj1:0x4A0C
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4256
        or     eax, eax
        je     loc_2_1859
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_1859:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_1860:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_1873
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_1860
        jmp    loc_2_1882
loc_2_1873:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_1882:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_1898
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_1898:
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
sub_2_18F0:
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
        call   sub_2_1C90
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_1919
        cmp    ebx, 0x103
        jne    loc_2_1930
loc_2_1919:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_19B0
        or     eax, eax
        je     loc_2_1930
        mov    ebx, eax
loc_2_1930:
        or     ebx, ebx
        je     loc_2_193C
        cmp    ebx, 0x103
        jne    loc_2_199B
loc_2_193C:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_1996
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_1952
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_1952:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_1961
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_1967
loc_2_1961:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_1967:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_1976
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_41B8
loc_2_1976:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_425C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_1996:
        mov    ebx, 0xc001000e
loc_2_199B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_19B0:
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
        call   sub_1_435C
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_435C
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_1A34
        sub    eax, eax
        nop    
loc_2_1A10:
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
        ja     loc_2_1A10
loc_2_1A34:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_1A9F
loc_2_1A41:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_1A94
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
        call   sub_1_435C
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
        call   sub_1_435C
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_1A97
loc_2_1A94:
        inc    dword ptr [ebp - 4]
loc_2_1A97:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_1A41
loc_2_1A9F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_1B8A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_1AB3:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_1E70
        or     al, al
        je     loc_2_1AEA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_1B78
loc_2_1AEA:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_1BC4
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
        call   sub_1_435C
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
        call   sub_1_435C
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
loc_2_1B78:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_1AB3
loc_2_1B8A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0xc], ecx
        je     loc_2_1C15
        mov    edi, dword ptr [ebp - 0x20]
        sub    edx, edx
        nop    
loc_2_1BA0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_1C1B
        mov    eax, dword ptr [esi + 4]
        add    eax, edx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_1C02
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    loc_2_1C02
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_1C09
loc_2_1BC4:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_435C
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_435C
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_1C02:
        mov    dword ptr [ebp - 8], 1
loc_2_1C09:
        add    edi, 6
        add    edx, 6
        inc    ecx
        cmp    dword ptr [ebp - 0xc], ecx
        ja     loc_2_1BA0
loc_2_1C15:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_1C80
loc_2_1C1B:
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
        je     loc_2_1C82
        cmp    edi, 0x103
        je     loc_2_1C82
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_435C
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_435C
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_1C80:
        sub    edi, edi
loc_2_1C82:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_1C90:
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
        je     loc_2_1CCD
        lea    esp, [esp]
loc_2_1CC0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_1CC0
loc_2_1CCD:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, ebx
        je     loc_2_1D01
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
        je     loc_2_1D03
        cmp    eax, 0x103
        je     loc_2_1D03
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1D01:
        sub    eax, eax
loc_2_1D03:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_1D10:
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
        je     loc_2_1D42
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_1D37:
        test   dword ptr [esi], edx
        je     loc_2_1D3C
        inc    eax
loc_2_1D3C:
        add    esi, 4
        dec    edi
        jne    loc_2_1D37
loc_2_1D42:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1D50:
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
        jbe    loc_2_1DCC
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_1D90:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_1DBA
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_1DE6
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_435C
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_1DBA:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_1D90
loc_2_1DCC:
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
loc_2_1DE6:
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
sub_2_1E10:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_1E40
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1E40:
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
        call   sub_1_435C
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1E70:
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
        je     loc_2_1F09
        dec    esi
        cmp    esi, edx
        jb     loc_2_1F09
        nop    
loc_2_1E90:
        cmp    edx, ecx
        jb     loc_2_1F09
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_1EB3
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1EE7
loc_2_1EB3:
        jbe    loc_2_1EC0
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_1EE7
        db  08Bh, 0FFh
loc_2_1EC0:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_1ED7
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1EE7
loc_2_1ED7:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_1EE7
        mov    dword ptr [ebp - 4], 0
loc_2_1EE7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_1F19
        jle    loc_2_1EF8
        or     edx, edx
        je     loc_2_1F09
        lea    esi, [edx - 1]
        jmp    loc_2_1EFB
loc_2_1EF8:
        lea    ecx, [edx + 1]
loc_2_1EFB:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_1E90
loc_2_1F09:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_1F19:
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

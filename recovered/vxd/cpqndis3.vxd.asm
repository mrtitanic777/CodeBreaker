; CPQNDIS3 -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 7994 instructions recovered by recursive descent

Declare_Virtual_Device CPQNDIS3, 3, 10, CPQNDIS3_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x4455  R-X
sub_1_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x21
        push   0
        call   sub_1_4438
        lea    eax, [ebp - 1]
        push   eax
        push   0
        call   sub_1_4440
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
        call   sub_1_4438
        push   ebx
        push   0xb
        call   sub_1_4438
        push   ebx
        push   0xa
        call   sub_1_4438
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0x15b]
        push   eax
        push   0xc
        call   sub_1_4438
        push   2
        push   0xd
        call   sub_1_4438
        mov    al, byte ptr [esi + 0xfa]
        push   eax
        push   3
        call   sub_1_4438
        mov    al, byte ptr [esi + 0xfa]
        push   eax
        push   1
        call   sub_1_4438
        mov    al, byte ptr [esi + 0xfb]
        push   eax
        push   2
        call   sub_1_4438
        mov    al, byte ptr [esi + 0xfa]
        push   0xff
        inc    al
        push   7
        mov    byte ptr [esi + 0xfd], al
        mov    byte ptr [esi + 0xfc], al
        mov    byte ptr [esi + 0x118], bl
        call   sub_1_4438
        mov    cl, byte ptr [esi + 0x15c]
        push   ecx
        push   0xf
        call   sub_1_4438
        push   0x61
        push   ebx
        call   sub_1_4438
loc_1_00D9:
        mov    al, byte ptr [esi + ebx + 0x104]
        lea    ecx, [ebx + 1]
        push   eax
        inc    ebx
        push   ecx
        call   sub_1_4438
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
        call   sub_1_4438
        cmp    ebx, 8
        jb     loc_1_00FF
        mov    al, byte ptr [esi + 0xfd]
        push   eax
        push   7
        call   sub_1_4438
        push   0x21
        push   0
        call   sub_1_4438
        push   0x22
        push   0
        call   sub_1_4438
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
        push   esi
        push   eax
        call   sub_1_0660
        push   0
        push   0xb
        call   sub_1_4438
        push   0
        push   0xa
        call   sub_1_4438
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4440
        test   byte ptr [ebp - 1], 0x80
        jne    loc_1_0196
        mov    esi, 0xa
loc_1_018C:
        push   0x64
        call   sub_1_3CC0
        dec    esi
        jne    loc_1_018C
loc_1_0196:
        push   2
        push   0xd
        call   sub_1_4438
        push   0x22
        push   0
        call   sub_1_4438
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_01C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    edi, 0x14
        call   sub_1_0150
loc_1_01D3:
        push   0x64
        call   sub_1_3CC0
        dec    edi
        jne    loc_1_01D3
        call   sub_1_0F30
        call   sub_1_0A70
        push   esi
        call   0
        test   al, al
        jne    loc_1_0210
        push   1
        mov    eax, dword ptr [esi + 4]
        push   1
        push   2
        push   0xaa000003
        push   eax
        call   sub_1_3D98
        add    esp, 0x14
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0210:
        call   sub_1_0F10
        push   0
        push   0xd
        call   sub_1_4438
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0230:
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
        call   sub_1_3CF0
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_025F
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_025F:
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
        call   sub_1_3D14
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_02AF
        xor    edi, edi
loc_1_0287:
        cmp    dword ptr [ebp - 0xc], edi
        jne    loc_1_02AF
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3D38
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 0x10]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_3D14
        cmp    dword ptr [ebp - 8], edi
        jne    loc_1_0287
loc_1_02AF:
        pushf  
        cli    
        push   0
        push   0
        call   sub_1_4438
        push   0xff
        push   0xa
        call   sub_1_4438
        push   0
        push   0xb
        call   sub_1_4438
        push   0xa
        push   0
        call   sub_1_4438
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        push   0x40
        push   7
        call   sub_1_4438
        push   esi
        push   8
        shr    esi, 8
        call   sub_1_4438
        push   esi
        push   9
        call   sub_1_4438
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        push   0xa
        call   sub_1_4438
        mov    eax, dword ptr [ebp - 0x14]
        shr    eax, 8
        xor    esi, esi
        push   eax
        push   0xb
        call   sub_1_4438
        push   0x12
        push   0
        call   sub_1_4438
loc_1_0329:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        push   ecx
        call   sub_1_4450
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 8], esi
        je     loc_1_038C
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D14
        cmp    dword ptr [ebp - 8], esi
        je     loc_1_0391
loc_1_035F:
        cmp    dword ptr [ebp - 0xc], esi
        jne    loc_1_038C
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 8], esi
        je     loc_1_038C
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D14
        cmp    dword ptr [ebp - 8], esi
        jne    loc_1_035F
loc_1_038C:
        cmp    dword ptr [ebp - 8], esi
        jne    loc_1_0329
loc_1_0391:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp + 0x10]
        mov    esi, 0xffff
        mov    ebx, 0x40
        mov    dword ptr [eax], ecx
loc_1_03A3:
        test   esi, esi
        je     loc_1_03B9
        dec    esi
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4440
        test   byte ptr [ebp - 1], bl
        je     loc_1_03A3
loc_1_03B9:
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
sub_1_03D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        test   esi, esi
        jne    loc_1_03EB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_03EB:
        pushf  
        cli    
        push   0
        push   0
        call   sub_1_4438
        push   0xff
        push   0xa
        call   sub_1_4438
        push   0
        push   0xb
        call   sub_1_4438
        push   0xa
        push   0
        call   sub_1_4438
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        push   0x40
        push   7
        call   sub_1_4438
        mov    edi, dword ptr [ebp + 8]
        push   edi
        push   8
        shr    edi, 8
        call   sub_1_4438
        push   edi
        push   9
        call   sub_1_4438
        push   esi
        push   0xa
        call   sub_1_4438
        mov    eax, esi
        shr    eax, 8
        push   eax
        push   0xb
        call   sub_1_4438
        push   0x12
        push   0
        call   sub_1_4438
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    esi, dword ptr [0]            ; -> sub_1_4450
        mov    ebx, 0x40
        call   esi
        mov    si, 0xffff
loc_1_0478:
        test   si, si
        je     loc_1_0490
        dec    si
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4440
        test   byte ptr [ebp - 1], bl
        je     loc_1_0478
loc_1_0490:
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
sub_1_04B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        test   esi, esi
        jne    loc_1_04CB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_04CB:
        pushf  
        cli    
        push   0
        push   0
        call   sub_1_4438
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        push   8
        shr    edi, 8
        call   sub_1_4438
        push   edi
        push   9
        call   sub_1_4438
        push   esi
        push   0xa
        call   sub_1_4438
        mov    eax, esi
        shr    eax, 8
        push   eax
        push   0xb
        call   sub_1_4438
        push   0xa
        push   0
        mov    ebx, 0x40
        call   sub_1_4438
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    esi, 0xffff
        call   sub_1_4444
loc_1_0526:
        test   esi, esi
        je     loc_1_053C
        dec    esi
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4440
        test   byte ptr [ebp - 1], bl
        je     loc_1_0526
loc_1_053C:
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
sub_1_0550:
        push   ebp
        mov    eax, 0xffffffff
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_0598
loc_1_0562:
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [esi + ecx]
        mov    ecx, 8
loc_1_056D:
        mov    edi, eax
        mov    bl, dl
        shr    dl, 1
        and    edi, 0x80000000
        shr    edi, 0x1f
        and    ebx, 1
        xor    edi, ebx
        add    eax, eax
        test   edi, edi
        je     loc_1_058F
        xor    eax, 0x4c11db6
        or     eax, edi
loc_1_058F:
        dec    ecx
        jne    loc_1_056D
        inc    esi
        cmp    dword ptr [ebp + 0xc], esi
        ja     loc_1_0562
loc_1_0598:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_05A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   6
        push   eax
        call   sub_1_0550
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
sub_1_05D0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, 8
        sub    esp, 0xc
        mov    dword ptr [ebp - 8], edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_1_05E5:
        mov    ecx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 8]
        mov    byte ptr [ecx + esi + 0x14f], dl
        cmp    dword ptr [ebp - 8], eax
        jb     loc_1_05E5
        push   0                             ; -> obj1:0x3E60
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi + 0x14b]
        push   0x60
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_2_1CA0
loc_1_0612:
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_1_064C
        xor    edi, edi
loc_1_061A:
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 2]
        push   eax
        dec    dword ptr [ebp - 8]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        lea    eax, [edx + edx*2]
        lea    ecx, [eax*2]                  ; -> obj1:0x3E60
        push   ecx
        call   sub_1_05A0
        xor    edx, edx
        mov    cl, byte ptr [ebp - 1]
        mov    dl, byte ptr [ebp - 2]
        or     byte ptr [edx + esi + 0x14f], cl
        cmp    dword ptr [ebp - 8], edi
        ja     loc_1_061A
loc_1_064C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0660:
        push   0x21
        push   0
        call   sub_1_4438
        xor    al, al
        ret    4
        db  090h
sub_1_0670:
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
        call   sub_1_4438
        cmp    esi, 0x3c
        jae    loc_1_06A0
        mov    esi, 0x3c
loc_1_06A0:
        mov    eax, esi
        shr    eax, 8
        push   eax
        push   6
        call   sub_1_4438
        push   esi
        push   5
        call   sub_1_4438
        lea    ecx, [ebp - 1]
        push   ecx
        push   0
        call   sub_1_4440
        push   0x26
        push   0
        call   sub_1_4438
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_06E0:
        push   ebp
        mov    ebp, esp
        push   0x62
        push   0
        call   sub_1_4438
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xfd
        push   eax
        push   7
        call   sub_1_4440
        push   0x22
        push   0
        call   sub_1_4438
        xor    al, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0710:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 0xfe
        push   eax
        push   4
        call   sub_1_4440
        xor    al, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0730:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 0xfc]
        cmp    byte ptr [eax + 0xfa], cl
        jne    loc_1_075A
        mov    al, byte ptr [eax + 0xfb]
        dec    al
        push   eax
        push   3
        call   sub_1_4438
        pop    ebp
        ret    4
loc_1_075A:
        dec    cl
        push   ecx
        push   3
        call   sub_1_4438
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0770:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    al, byte ptr [eax + 0x15b]
        push   eax
        push   0xc
        call   sub_1_4438
        xor    al, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0790:
        push   esi
        push   0x62
        push   0
        xor    esi, esi
        call   sub_1_4438
        mov    eax, dword ptr [0]            ; -> sub_1_4438
loc_1_07A2:
        lea    ecx, [esi + 8]
        push   0xff
        push   ecx
        inc    esi
        call   eax
        mov    eax, dword ptr [0]            ; -> sub_1_4438
        cmp    esi, 8
        jb     loc_1_07A2
        push   0x22
        push   0
        call   eax
        xor    al, al
        pop    esi
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0x62
        push   0
        xor    edi, edi
        call   sub_1_4438
        mov    esi, dword ptr [ebp + 8]
loc_1_07E4:
        mov    al, byte ptr [esi + edi + 0x14f]
        lea    ecx, [edi + 8]
        push   eax
        inc    edi
        push   ecx
        call   sub_1_4438
        cmp    edi, 8
        jb     loc_1_07E4
        push   0x22
        push   0
        call   sub_1_4438
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0810:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        test   byte ptr [eax + 0x103], 4
        je     loc_1_0826
        push   eax
        call   sub_1_0830
loc_1_0826:
        xor    al, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0830:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        push   0xd
        call   sub_1_4440
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        lea    ecx, [ebp - 1]
        push   ecx
        add    dword ptr [esi + 0x132], eax
        push   0xe
        call   sub_1_4440
        xor    ecx, ecx
        lea    edx, [ebp - 1]
        mov    cl, byte ptr [ebp - 1]
        push   edx
        push   0xf
        add    dword ptr [esi + 0x136], ecx
        call   sub_1_4440
        xor    ecx, ecx
        xor    al, al
        mov    cl, byte ptr [ebp - 1]
        add    dword ptr [esi + 0x13a], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x14
        push   esi
        call   sub_1_0660
loc_1_08A6:
        push   0x64
        call   sub_1_3CC0
        dec    edi
        jne    loc_1_08A6
        test   byte ptr [esi + 0x103], 0xa
        jne    loc_1_08D8
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4440
        test   byte ptr [ebp - 1], 0xa
        jne    loc_1_08D8
        mov    al, byte ptr [esi + 0x5b]
        mov    byte ptr [esi + 0x5c], al
        mov    byte ptr [esi + 0x1b1], 0
loc_1_08D8:
        mov    byte ptr [esi + 0x5b], 0
        mov    dword ptr [esi + 0x147], 0
        push   0
        push   0xb
        call   sub_1_4438
        push   0
        push   0xa
        call   sub_1_4438
        push   2
        push   0xd
        call   sub_1_4438
        push   0x22
        push   0
        call   sub_1_4438
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0920:
        push   ebp
        mov    ebp, esp
        pushf  
        cli    
        cmp    byte ptr [ebp + 8], 1
        je     loc_1_0938
        cmp    byte ptr [ebp + 8], 2
        je     loc_1_098D
        cmp    byte ptr [ebp + 8], 3
        je     loc_1_0963
loc_1_0938:
        mov    dx, 0xfb
        out    dx, al
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        add    dx, 0x402
        in     al, dx
        and    al, 0x1f
        out    dx, al
        mov    dx, 0xec
        mov    al, 0x5c
        out    dx, al
        mov    dx, 0xed
        in     al, dx
        and    al, 0xdf
        out    dx, al
        mov    dx, 0xfb
        out    dx, al
        jmp    loc_1_09B8
loc_1_0963:
        mov    dx, 0xfb
        out    dx, al
        mov    dx, 0xec
        mov    al, 0x5b
        out    dx, al
        mov    dx, 0xed
        in     al, dx
        and    al, 0x9f
        out    dx, al
        mov    dx, 0xec
        mov    al, 0x5c
        out    dx, al
        mov    dx, 0xed
        in     al, dx
        or     al, 0x20
        out    dx, al
        mov    dx, 0xfb
        out    dx, al
        jmp    loc_1_09B8
loc_1_098D:
        mov    dx, 0xfb
        out    dx, al
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        add    dx, 0x402
        in     al, dx
        and    al, 0x1f
        or     al, 0x20
        out    dx, al
        mov    dx, 0xec
        mov    al, 0x5c
        out    dx, al
        mov    dx, 0xed
        in     al, dx
        and    al, 0xdf
        out    dx, al
        mov    dx, 0xfb
        out    dx, al
loc_1_09B8:
        popf   
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_09C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushf  
        cli    
        mov    dx, 0xfb
        out    dx, al
        mov    dx, 0xec
        mov    al, 0x5c
        out    dx, al
        mov    dx, 0xed
        in     al, dx
        and    al, 0x20
        je     loc_1_09E2
        mov    ah, 3
        jmp    loc_1_0A01
loc_1_09E2:
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        add    dx, 0x402
        in     al, dx
        test   al, 0xe0
        jne    loc_1_09F7
        mov    ah, 1
        jmp    loc_1_0A01
loc_1_09F7:
        test   al, 0xc0
        jne    loc_1_09FF
        mov    ah, 2
        jmp    loc_1_0A01
loc_1_09FF:
        mov    ah, 0xff
loc_1_0A01:
        mov    dx, 0xf9
        out    dx, al
        mov    byte ptr [ebp - 1], ah
        popf   
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A20:
        mov    cx, word ptr [0]              ; -> loc_1_43A4
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [0]              ; -> obj1:0x40CC
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
sub_1_0A70:
        push   0xaa
        call   sub_1_43A0
        push   0x14
        call   sub_1_3CC0
        push   0
        call   sub_1_43A0
        push   0x14
        call   sub_1_3CC0
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0AA0:
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        mov    cl, byte ptr [0]              ; -> obj1:0x40CC
        or     al, 8
        and    cl, 0xfb
        or     al, 0x10
        or     cl, byte ptr [0]              ; -> loc_1_40D0
        or     cl, byte ptr [0]              ; -> loc_1_40D4
        mov    byte ptr [0], al              ; -> loc_1_40C8
        mov    byte ptr [0], 0               ; -> loc_1_40D8
        mov    byte ptr [0], cl              ; -> obj1:0x40CC
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0A0h
        dd  loc_1_40C8               ; fixup
        db  08Ah, 00Dh
        dd  obj1:0x40CC              ; fixup
        db  00Ch, 008h, 080h, 0C9h, 004h, 00Ch, 010h, 00Ah, 00Dh
        dd  loc_1_40D0               ; fixup
        db  00Ah, 00Dh
        dd  loc_1_40D4               ; fixup
        db  0A2h
        dd  loc_1_40C8               ; fixup
        db  0C6h, 005h
        dd  loc_1_40D8               ; fixup
        db  000h, 088h, 00Dh
        dd  obj1:0x40CC              ; fixup
        db  0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0B20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    dx, word ptr [0]              ; -> loc_1_43A4
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
        in     al, dx
        test   al, 0x80
        je     loc_1_0B6A
        in     al, dx
        test   al, 0x80
        je     loc_1_0B6A
        in     al, dx
        test   al, 0x80
        je     loc_1_0B6A
        push   ecx
        mov    ecx, 0x1000
loc_1_0B62:
        in     al, dx
        test   al, 0x80
        je     loc_1_0B6A
        loop   loc_1_0B62
        pop    ecx
loc_1_0B6A:
        dec    dx
        in     al, dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        add    dx, 2
        mov    al, ah
        out    dx, al
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_0B80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    dx, word ptr [0]              ; -> loc_1_43A4
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
        mov    esi, dword ptr [ebp + 8]
        add    dx, 2
loc_1_0BA9:
        mov    ax, 0x3537
        out    dx, al
        dec    dx
        in     al, dx
        test   al, 0x80
        je     loc_1_0BCD
        in     al, dx
        test   al, 0x80
        je     loc_1_0BCD
        in     al, dx
        test   al, 0x80
        je     loc_1_0BCD
        push   ecx
        mov    ecx, 0x1000
loc_1_0BC5:
        in     al, dx
        test   al, 0x80
        je     loc_1_0BCD
        loop   loc_1_0BC5
        pop    ecx
loc_1_0BCD:
        dec    dx
        in     al, dx
        mov    byte ptr [esi], al
        inc    esi
        add    dx, 2
        mov    al, ah
        out    dx, al
        loop   loc_1_0BA9
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0BF0:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C00:
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        or     al, 8
        or     al, 0x10
        mov    byte ptr [0], al              ; -> loc_1_40C8
        mov    al, byte ptr [0]              ; -> obj1:0x40CC
        and    al, 0xfb
        or     al, 8
        or     al, byte ptr [0]              ; -> loc_1_40D0
        or     al, 2
        mov    byte ptr [0], al              ; -> obj1:0x40CC
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C30:
        mov    al, byte ptr [0]              ; -> obj1:0x40CC
        and    al, 0xf7
        mov    byte ptr [0], al              ; -> obj1:0x40CC
        ret    
        db  08Dh, 049h, 000h
loc_1_0C40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        add    dx, 2
        in     al, dx
        and    al, 0xdf
        out    dx, al
        inc    dx
        mov    al, byte ptr [ebp + 8]
        or     al, 0x20
        out    dx, al
        dec    dx
        jmp    loc_1_0C62
loc_1_0C62:
        jmp    loc_1_0C67
loc_1_0C67:
        in     al, dx
        or     al, 0x20
        out    dx, al
        add    dx, 2
        jmp    loc_1_0C74
loc_1_0C74:
        in     al, dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        pop    esi
        pop    ebp
        ret    8
        db  090h
loc_1_0C80:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        add    dx, 2
        in     al, dx
        and    al, 0xdf
        out    dx, al
        inc    dx
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        jmp    loc_1_0C9D
loc_1_0C9D:
        jmp    loc_1_0CA2
loc_1_0CA2:
        jmp    loc_1_0CA7
loc_1_0CA7:
        mov    al, byte ptr [ebp + 0xc]
        inc    dx
        out    dx, al
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0CC0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        add    dx, 2
        in     al, dx
        and    al, 0xdf
        out    dx, al
        inc    dx
        mov    al, 0x30
        out    dx, al
        cld    
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        dec    dx
        in     al, dx
        or     al, 0x20
        out    dx, al
        add    dx, 2
        rep insb byte ptr es:[edi], dx
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        add    dx, 2
        in     al, dx
        and    al, 0xdf
        out    dx, al
        inc    dx
        mov    al, 0x10
        out    dx, al
        cld    
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        inc    dx
        rep outsb dx, byte ptr [esi]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0D30:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        add    dx, 2
        in     al, dx
        and    al, 0xdf
        out    dx, al
        inc    dx
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0D50:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        add    dx, 2
        in     al, dx
        and    al, 0xdf
        out    dx, al
        inc    dx
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0D70:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [0]              ; -> loc_1_43A4
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
        in     al, dx
        test   al, 0x80
        je     loc_1_0DB8
        in     al, dx
        test   al, 0x80
        je     loc_1_0DB8
        in     al, dx
        test   al, 0x80
        je     loc_1_0DB8
        push   ecx
        mov    ecx, 0x1000
loc_1_0DB0:
        in     al, dx
        test   al, 0x80
        je     loc_1_0DB8
        loop   loc_1_0DB0
        pop    ecx
loc_1_0DB8:
        inc    dx
        mov    al, ah
        out    dx, al
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0DD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        mov    al, 0x10
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
loc_1_0DEE:
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        mov    al, byte ptr [esi]
        inc    esi
        out    dx, al
        add    dx, 2
        mov    ax, 0x1517
        out    dx, al
        dec    dx
        in     al, dx
        test   al, 0x80
        je     loc_1_0E21
        in     al, dx
        test   al, 0x80
        je     loc_1_0E21
        in     al, dx
        test   al, 0x80
        je     loc_1_0E21
        push   ecx
        mov    ecx, 0x1000
loc_1_0E19:
        in     al, dx
        test   al, 0x80
        je     loc_1_0E21
        loop   loc_1_0E19
        pop    ecx
loc_1_0E21:
        inc    dx
        mov    al, ah
        out    dx, al
        loop   loc_1_0DEE
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_0E30:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0E40:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        or     al, byte ptr [0]              ; -> loc_1_40D8
        out    dx, al
        mov    al, 0x15
        or     al, byte ptr [0]              ; -> loc_1_40D8
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0E70:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        mov    al, byte ptr [ebp + 8]
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        or     al, byte ptr [0]              ; -> loc_1_40D8
        out    dx, al
        mov    al, 0x15
        or     al, byte ptr [0]              ; -> loc_1_40D8
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0EA0:
        pushf  
        cli    
        cmp    byte ptr [0], 0               ; -> loc_1_40DC
        jne    loc_1_0EBF
        call   sub_1_0F50
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        and    al, 0xfe
        push   eax
        call   sub_1_4448
loc_1_0EBF:
        dec    byte ptr [0]                  ; -> loc_1_40DC
        popf   
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0ED0:
        pushf  
        cli    
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        push   eax
        call   sub_1_4448
        inc    byte ptr [0]                  ; -> loc_1_40DC
        js     loc_1_0F01
        mov    byte ptr [0], 0               ; -> loc_1_40DC
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        or     al, 1
        push   eax
        call   sub_1_4448
        call   sub_1_0FF0
loc_1_0F01:
        popf   
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0F10:
        pushf  
        cli    
        mov    byte ptr [0], 0               ; -> loc_1_40DC
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        or     al, 1
        push   eax
        call   sub_1_4448
        popf   
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_0F30:
        pushf  
        cli    
        mov    byte ptr [0], 0               ; -> loc_1_40DC
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        and    al, 0xfe
        push   eax
        call   sub_1_4448
        popf   
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_0F50:
        call   sub_1_09C0
        mov    byte ptr [0], al              ; -> loc_1_443E
        cmp    al, 2
        je     loc_1_0F65
        push   2
        call   sub_1_0920
loc_1_0F65:
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        in     al, dx
        mov    byte ptr [0], al              ; -> loc_1_444C
        add    dx, 2
        mov    al, 0x14
        out    dx, al
        sub    dx, 2
        mov    al, 0xff
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xdf
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xff
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xfb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xdb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xfb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xdb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xfb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xff
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xdf
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        cmp    byte ptr [0], 0               ; -> loc_1_3EC0
        je     loc_1_0FE8
        push   3
        call   sub_1_0920
        ret    
loc_1_0FE8:
        push   2
        call   sub_1_0920
        ret    
sub_1_0FF0:
        push   2
        call   sub_1_0920
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        mov    al, 0
        out    dx, al
        add    dx, 2
        mov    al, 0x1c
        out    dx, al
        mov    al, 0x14
        out    dx, al
        sub    dx, 2
        mov    al, 0xff
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xdf
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xff
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xfb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xdb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xfb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xdb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xfb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, 0xdb
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, byte ptr [0]              ; -> loc_1_444C
        out    dx, al
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        mov    al, byte ptr [0]              ; -> loc_1_443E
        cmp    al, 2
        je     loc_1_107B
        push   eax
        call   sub_1_0920
loc_1_107B:
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_1080:
        call   sub_1_0EA0
        push   0
        push   0xf
        call   sub_1_4438
        call   sub_1_0ED0
        mov    al, 1
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10A0:
        xor    al, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_1304
        lea    edi, [esi + 0x103]
        call   sub_1_0EA0
        push   edi
        inc    dword ptr [esi + 0x30]
        push   7
        call   sub_1_4440
        mov    al, byte ptr [edi]
        test   al, al
        je     loc_1_10EC
        push   eax
        push   7
        call   sub_1_4438
loc_1_10EC:
        mov    al, byte ptr [edi]
        test   al, 0x20
        je     loc_1_10F9
        mov    eax, 8
        jmp    loc_1_112D
loc_1_10F9:
        test   al, 0x10
        je     loc_1_110A
        push   esi
        call   sub_1_0830
        mov    eax, 4
        jmp    loc_1_112D
loc_1_110A:
        test   al, 0xa
        je     loc_1_1115
        mov    eax, 2
        jmp    loc_1_112D
loc_1_1115:
        test   al, 1
        jne    loc_1_1128
        test   al, 4
        mov    eax, 0x10
        je     loc_1_112D
        push   esi
        call   sub_1_0830
loc_1_1128:
        mov    eax, 1
loc_1_112D:
        xor    ebx, ebx
loc_1_112F:
        cmp    eax, 0x10
        jne    loc_1_114C
        cmp    dword ptr [esi + 0x161], ebx
        je     loc_1_1235
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_1_1235
loc_1_114C:
        dec    eax
        cmp    eax, 7
        ja     loc_1_11B6
        xor    ecx, ecx
        mov    cl, byte ptr [eax]            ; -> obj1:0x1320
        jmp    dword ptr [ecx*4]             ; -> obj1:0x130C
loc_1_1161:
        push   esi
        call   sub_1_0710
        mov    byte ptr [esi + 0x1b1], bl
        cmp    byte ptr [esi + 0x11b], bl
        jne    loc_1_1194
        mov    byte ptr [esi + 0x5b], bl
        mov    dword ptr [esi + 0x147], ebx
        test   byte ptr [edi], 8
        je     loc_1_1189
        push   esi
        call   sub_1_24A0
loc_1_1189:
        test   byte ptr [edi], 2
        je     loc_1_1194
        push   esi
        call   sub_1_1560
loc_1_1194:
        and    byte ptr [edi], 0xf5
        jmp    loc_1_11B6
loc_1_1199:
        mov    byte ptr [esi + 0x118], 1
        and    byte ptr [edi], 0xef
loc_1_11A3:
        push   esi
        call   sub_1_1330
        test   al, al
        je     loc_1_11B0
        and    byte ptr [edi], 0xfa
loc_1_11B0:
        push   esi
        call   sub_1_2140
loc_1_11B6:
        cmp    dword ptr [esi + 0x161], ebx
        je     loc_1_11CC
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_1_11CC
        push   esi
        call   sub_1_1330
loc_1_11CC:
        mov    al, byte ptr [edi]
        test   al, 0x20
        je     loc_1_11E7
        mov    eax, 8
        jmp    loc_1_112F
loc_1_11DC:
        push   esi
        call   sub_1_0830
        and    byte ptr [edi], 0xdf
        jmp    loc_1_11B6
loc_1_11E7:
        test   al, 0x10
        je     loc_1_11FB
        push   esi
        call   sub_1_0830
        mov    eax, 4
        jmp    loc_1_112F
loc_1_11FB:
        test   al, 0xa
        je     loc_1_1209
        mov    eax, 2
        jmp    loc_1_112F
loc_1_1209:
        test   al, 1
        je     loc_1_1217
        mov    eax, 1
        jmp    loc_1_112F
loc_1_1217:
        test   al, 4
        je     loc_1_122B
        push   esi
        call   sub_1_0830
        mov    eax, 1
        jmp    loc_1_112F
loc_1_122B:
        mov    eax, 0x10
        jmp    loc_1_112F
loc_1_1235:
        push   edi
        push   7
        call   sub_1_4440
        mov    al, byte ptr [edi]
        test   al, al
        je     loc_1_124D
        push   eax
        push   7
        call   sub_1_4438
loc_1_124D:
        mov    al, byte ptr [edi]
        test   al, 0x20
        je     loc_1_125A
        mov    eax, 8
        jmp    loc_1_128E
loc_1_125A:
        test   al, 0x10
        je     loc_1_126B
        push   esi
        call   sub_1_0830
        mov    eax, 4
        jmp    loc_1_128E
loc_1_126B:
        test   al, 0xa
        je     loc_1_1276
        mov    eax, 2
        jmp    loc_1_128E
loc_1_1276:
        test   al, 1
        jne    loc_1_1289
        test   al, 4
        mov    eax, 0x10
        je     loc_1_128E
        push   esi
        call   sub_1_0830
loc_1_1289:
        mov    eax, 1
loc_1_128E:
        cmp    eax, 0x10
        jne    loc_1_114C
        mov    byte ptr [esi + 0x11d], 0
        mov    byte ptr [esi + 0x15c], 0x1b
        push   0x1b
        push   0xf
        call   sub_1_4438
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_12E0
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_12D0
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_12D0
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_12E0
loc_1_12D0:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_3CA2
        jmp    loc_1_12FF
loc_1_12E0:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_12FF
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_12FF
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_12FF
        push   esi
        call   sub_1_2140
loc_1_12FF:
        call   sub_1_0ED0
loc_1_1304:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  090h
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_11A3
        dd  loc_1_1161
        dd  loc_1_1199
        dd  loc_1_11DC
        dd  loc_1_11B6
        db  000h, 001h, 004h, 002h, 004h, 004h, 004h, 003h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1330:
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
        call   sub_1_06E0
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_1365
        cmp    byte ptr [esi + 0x118], 0
        je     loc_1_1365
        push   esi
        call   sub_1_0890
loc_1_1365:
        xor    ebx, ebx
loc_1_1367:
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_1_1454
        test   byte ptr [esi + 0x103], 4
        jne    loc_1_14FB
        mov    al, byte ptr [esi + 0xfd]
        cmp    byte ptr [esi + 0xfc], al
        jne    loc_1_13A6
        push   esi
        call   sub_1_06E0
        mov    al, byte ptr [esi + 0xfd]
        cmp    byte ptr [esi + 0xfc], al
        je     loc_1_1454
loc_1_13A6:
        push   esi
        inc    dword ptr [esi + 0x34]
        mov    dword ptr [esi + 0x169], ebx
        call   sub_1_1A50
        test   al, al
        je     loc_1_140F
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 0x1b6]
        mov    al, byte ptr [esi + 0x1b5]
        shl    ecx, 8
        add    ecx, eax
        mov    eax, dword ptr [esi + 0x15d]
        sub    ecx, 4
        cmp    ecx, eax
        jb     loc_1_13DD
        mov    ecx, eax
loc_1_13DD:
        add    ecx, 0xe
        mov    eax, dword ptr [esi + 0x1b7]
        push   ecx
        lea    edx, [esi + 0x1bb]
        push   eax
        push   edx
        call   sub_1_04B0
        test   al, al
        je     loc_1_140F
        push   esi
        call   sub_1_1C00
        cmp    eax, 3
        je     loc_1_1518
        inc    dword ptr [esi + 0x122]
        jmp    loc_1_1414
loc_1_140F:
        mov    eax, 1
loc_1_1414:
        cmp    eax, 3
        je     loc_1_1518
        cmp    eax, 1
        jne    loc_1_1430
        push   esi
        call   sub_1_06E0
        mov    al, byte ptr [esi + 0xfd]
        jmp    loc_1_1436
loc_1_1430:
        mov    al, byte ptr [esi + 0x1b4]
loc_1_1436:
        push   esi
        mov    byte ptr [esi + 0xfc], al
        call   sub_1_0730
        cmp    dword ptr [esi + 0x34], 0xa
        jbe    loc_1_1367
        xor    eax, eax
        mov    byte ptr [ebp - 1], al
        mov    dword ptr [esi + 0x34], eax
loc_1_1454:
        cmp    byte ptr [esi + 0x118], 0
        je     loc_1_1495
        mov    byte ptr [esi + 0x118], 0
        push   esi
        call   sub_1_0810
        push   0
        push   0xd
        call   sub_1_4438
        cmp    byte ptr [esi + 0x5c], 0
        je     loc_1_1495
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_1495
        mov    byte ptr [esi + 0x5c], 0
        mov    byte ptr [esi + 0x1b1], 0
        mov    byte ptr [esi + 0x5b], 1
        push   esi
        call   sub_1_0670
loc_1_1495:
        cmp    dword ptr [esi + 0x161], 0
        je     loc_1_14E6
loc_1_149E:
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_14EF
        mov    edi, dword ptr [esi + 0x161]
        push   edi
        push   esi
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [esi + 0x161], eax
        mov    dword ptr [esi + 0x169], edi
        inc    dword ptr [esi + 0x11e]
        mov    ebx, dword ptr [edi + 0x28]
        call   sub_1_18B0
        push   0
        mov    ecx, dword ptr [edi + 0x28]
        push   edi
        mov    eax, dword ptr [ecx]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi + 0x161], 0
        jne    loc_1_149E
loc_1_14E6:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_152A
loc_1_14EF:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_14FB:
        push   esi
        call   sub_1_06E0
        mov    al, byte ptr [esi + 0xfd]
        push   esi
        mov    byte ptr [esi + 0xfc], al
        call   sub_1_0730
        jmp    loc_1_1454
loc_1_1518:
        mov    byte ptr [esi + 0x15c], 0x1a
        push   esi
        call   sub_1_01C0
        jmp    loc_1_1454
loc_1_152A:
        cmp    byte ptr [esi + 0x11a], 0
        je     loc_1_1546
        mov    eax, dword ptr [esi + 0x14b]
        push   eax
        call   sub_1_3A50
        mov    byte ptr [esi + 0x11a], 0
loc_1_1546:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1560:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x11b], 1
        cmp    dword ptr [esi + 0x57], -1
        jne    loc_1_159E
        push   4
        mov    eax, dword ptr [esi + 4]
        mov    byte ptr [esi + 0x11b], 0
        push   1
        push   0xaa000006
        push   eax
        call   sub_1_3D98
        add    esp, 0x10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_159E:
        push   esi
        call   sub_1_0710
        test   byte ptr [esi + 0xfe], 1
        je     loc_1_15B5
        inc    dword ptr [esi + 0x11e]
        jmp    loc_1_15BB
loc_1_15B5:
        inc    dword ptr [esi + 0x126]
loc_1_15BB:
        mov    ecx, dword ptr [esi + 0x57]
        mov    eax, dword ptr [esi + ecx*4 + 0xbd]
        lea    edx, [esi + ecx*4]
        add    eax, 0xff
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        add    eax, ecx
        cmp    eax, ecx
        jbe    loc_1_15F0
        add    edx, 0x5d
loc_1_15DC:
        mov    dword ptr [edx], 0
        add    edx, 4
        inc    ecx
        mov    eax, dword ptr [esi + 0x57]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, ecx
        ja     loc_1_15DC
loc_1_15F0:
        mov    eax, dword ptr [esi + 0x53]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x53], eax
        cmp    eax, 0xc
        jne    loc_1_1605
        mov    dword ptr [esi + 0x53], 0
loc_1_1605:
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_161B
        cmp    dword ptr [esi + 0x4f], eax
        je     loc_1_161B
        mov    dword ptr [esi + 0x53], 0
loc_1_161B:
        mov    eax, dword ptr [esi + 0x57]
        mov    dword ptr [esi + eax*4 + 0x8d], 0
        mov    ecx, dword ptr [esi + 0x53]
        lea    eax, [esi + ecx*4 + 0x5d]
        mov    edx, dword ptr [eax]
        test   edx, edx
        je     loc_1_1658
        cmp    edx, 1
        je     loc_1_1839
        cmp    edx, 2
        je     loc_1_1856
        mov    byte ptr [esi + 0x11b], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1658:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_1680
        mov    dword ptr [esi + 0x57], 0xffffffff
        mov    byte ptr [esi + 0x11b], 0
        push   2
        push   esi
        call   sub_1_33A0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1680:
        mov    ebx, dword ptr [esi + 0x47]
        test   ebx, ebx
        je     loc_1_1822
        lea    edi, [ebx + 0x24]
        lea    ecx, [ebp - 4]
        push   ecx
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   0
        mov    dword ptr [esi + 0x47], eax
        push   ebx
        call   sub_1_3CF0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xff
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 0x4f]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jbe    loc_1_16C3
        mov    dword ptr [esi + 0x4f], 0
loc_1_16C3:
        mov    ecx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        mov    dword ptr [esi + 0x53], ecx
        cmp    eax, ecx
        jbe    loc_1_16EA
        lea    eax, [esi + ecx*4 + 0x5d]
loc_1_16D6:
        mov    dword ptr [eax], 2
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [esi + 0x4f]
        add    edx, dword ptr [ebp - 4]
        cmp    edx, ecx
        ja     loc_1_16D6
loc_1_16EA:
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
        call   sub_1_0230
        test   al, al
        jne    loc_1_1763
        push   0xc0000001
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_3C0A
        mov    edx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [ebp - 4]
        add    eax, edx
        cmp    eax, edx
        jbe    loc_1_1746
        lea    eax, [esi + edx*4 + 0x5d]
loc_1_1732:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    edx
        mov    ecx, dword ptr [esi + 0x4f]
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, edx
        ja     loc_1_1732
loc_1_1746:
        mov    eax, dword ptr [esi + 0x4f]
        push   esi
        mov    dword ptr [esi + eax*4 + 0x8d], 0
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1763:
        mov    ecx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [esi + ecx*4 + 0xbd]
        cmp    eax, 0x3c
        jae    loc_1_1792
        shl    ecx, 8
        mov    edx, 0x3c
        sub    edx, eax
        push   edx
        push   0                             ; -> loc_1_4174
        mov    edx, dword ptr [esi + 0xed]
        add    edx, ecx
        add    edx, eax
        push   edx
        call   sub_1_03D0
loc_1_1792:
        mov    eax, dword ptr [esi + 0x4f]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x4f], eax
        cmp    eax, 0xc
        jne    loc_1_17A7
        mov    dword ptr [esi + 0x4f], 0
loc_1_17A7:
        mov    byte ptr [esi + 0x11b], 0
        cmp    byte ptr [esi + 0x118], 0
        je     loc_1_17BD
        mov    byte ptr [esi + 0x5c], 1
        jmp    loc_1_17C7
loc_1_17BD:
        mov    byte ptr [esi + 0x5b], 1
        push   esi
        call   sub_1_0670
loc_1_17C7:
        cmp    byte ptr [edi + 8], 0
        jne    loc_1_17ED
        mov    edi, dword ptr [edi + 4]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C0A
        push   esi
        dec    dword ptr [edi + 0x1c]
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_17ED:
        cmp    dword ptr [esi + 0x161], 0
        jne    loc_1_17FE
        mov    dword ptr [esi + 0x161], ebx
        jmp    loc_1_1807
loc_1_17FE:
        mov    eax, dword ptr [esi + 0x165]
        mov    dword ptr [eax + 0x24], ebx
loc_1_1807:
        push   esi
        mov    dword ptr [esi + 0x165], ebx
        mov    dword ptr [edi], 0
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1822:
        mov    dword ptr [esi + 0x57], 0xffffffff
        mov    byte ptr [esi + 0x11b], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1839:
        mov    dword ptr [esi + 0x57], 0xffffffff
        mov    byte ptr [esi + 0x11b], 0
        push   esi
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1856:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_1884
        mov    dword ptr [esi + 0x57], 0xffffffff
        push   2
        mov    dword ptr [eax], 0
        mov    byte ptr [esi + 0x11b], 0
        push   esi
        call   sub_1_33A0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1884:
        push   esi
        mov    dword ptr [esi + 0x57], ecx
        mov    byte ptr [esi + 0x11b], 0
        mov    byte ptr [esi + 0x5b], 1
        call   sub_1_0670
        push   esi
        call   sub_1_2140
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_18B0:
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
        call   sub_1_3CF0
        cmp    dword ptr [ebp - 4], 6
        jae    loc_1_18E0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_18E0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [esi + 0x15d]
        add    edi, 0xe
        cmp    edi, eax
        jb     loc_1_18F5
        mov    edi, eax
loc_1_18F5:
        push   edi
        lea    eax, [esi + 0x1bb]
        push   0
        push   ebx
        push   eax
        call   sub_1_1970
        cmp    edi, 0xe
        jae    loc_1_1928
        cmp    edi, 6
        jb     loc_1_1951
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
        jmp    loc_1_194C
loc_1_1928:
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
loc_1_194C:
        call   sub_1_3870
loc_1_1951:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x11a], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1970:
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
        call   sub_1_3CF0
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebp - 4], esi
        je     loc_1_19C4
loc_1_1995:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_3D14
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     loc_1_19C4
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1995
loc_1_19C4:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_19D5
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_19D5:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 0x14]
loc_1_19E4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    loc_1_19F3
        mov    ebx, edi
        sub    ebx, esi
        jmp    loc_1_19F6
loc_1_19F3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_19F6:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_3DAC
        cmp    esi, edi
        je     loc_1_1A32
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1A3D
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_3D14
        jmp    loc_1_19E4
loc_1_1A32:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_1A3D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1A50:
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
        call   sub_1_04B0
        test   al, al
        jne    loc_1_1A95
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1A95:
        xor    ecx, ecx
        lea    edx, [ebp - 0x18]
        add    dword ptr [ebp - 0xc], 4
loc_1_1A9E:
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
        jne    loc_1_1B30
        test   byte ptr [edx], 0x20
        je     loc_1_1ACC
        test   ecx, ecx
        jne    loc_1_1ACC
        test   byte ptr [edi], 1
        je     loc_1_1B30
loc_1_1ACC:
        mov    edi, dword ptr [ebp - 0x10]
        cmp    byte ptr [edi], bl
        jne    loc_1_1AEE
        mov    edi, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 2], bl
        mov    bl, byte ptr [edi]
        mov    edi, dword ptr [ebp - 0x10]
        sub    bl, byte ptr [esi + 0xfc]
        dec    bl
        mov    byte ptr [eax], bl
        cmp    byte ptr [edi], 0xfc
        jbe    loc_1_1AEE
        inc    eax
loc_1_1AEE:
        xor    ebx, ebx
        mov    edi, dword ptr [ebp - 0x10]
        mov    bl, byte ptr [eax]
        shl    ebx, 8
        movzx  edi, byte ptr [edi]
        lea    edi, [ebx + edi - 4]
        cmp    edi, 0x5ea
        ja     loc_1_1B0C
        cmp    edi, 0x3c
        jae    loc_1_1B15
loc_1_1B0C:
        mov    byte ptr [ebp - 1], 1
        mov    bl, byte ptr [ebp - 2]
        mov    byte ptr [eax], bl
loc_1_1B15:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_1B3A
        mov    edi, dword ptr [ebp - 8]
        mov    al, byte ptr [edi]
        cmp    byte ptr [esi + 0xfa], al
        ja     loc_1_1B30
        cmp    byte ptr [esi + 0xfb], al
        jae    loc_1_1B34
loc_1_1B30:
        mov    byte ptr [ebp - 1], 1
loc_1_1B34:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1B6C
loc_1_1B3A:
        test   ecx, ecx
        je     loc_1_1B45
        cmp    ecx, 1
        je     loc_1_1B5A
        jmp    loc_1_1B6B
loc_1_1B45:
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0xc]
        cmp    byte ptr [esi + 0x11c], 0
        jne    loc_1_1B6B
        inc    edx
        inc    dword ptr [ebp - 0xc]
        jmp    loc_1_1B6B
loc_1_1B5A:
        dec    edx
        dec    dword ptr [ebp - 0xc]
        cmp    byte ptr [esi + 0x11c], 0
        jne    loc_1_1B6B
        dec    edx
        dec    dword ptr [ebp - 0xc]
loc_1_1B6B:
        inc    ecx
loc_1_1B6C:
        cmp    ecx, 2
        jae    loc_1_1B7B
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_1A9E
loc_1_1B7B:
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
        ja     loc_1_1BD2
        cmp    eax, 0x3c
        jb     loc_1_1BD2
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BD2:
        cmp    byte ptr [esi + 0xfa], cl
        ja     loc_1_1BED
        cmp    byte ptr [esi + 0xfb], cl
        jb     loc_1_1BED
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BED:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1C00:
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
        jb     loc_1_1C2A
        sub    dl, cl
        add    dl, byte ptr [esi + 0xfa]
loc_1_1C2A:
        mov    al, byte ptr [esi + 0x1b4]
        cmp    dl, al
        je     loc_1_1C4F
        inc    dl
        cmp    dl, cl
        jne    loc_1_1C40
        mov    dl, byte ptr [esi + 0xfa]
loc_1_1C40:
        cmp    dl, al
        je     loc_1_1C4F
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1C4F:
        cmp    byte ptr [esi + 0xfa], al
        ja     loc_1_1CDC
        cmp    al, cl
        ja     loc_1_1CDC
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 0x1b5]
        mov    al, bl
        shl    eax, 8
        lea    eax, [ecx + eax - 4]
        cmp    eax, 0x5ea
        jbe    loc_1_1C84
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1C84:
        mov    ecx, dword ptr [esi + 0x15d]
        add    ecx, 0xe
        cmp    ecx, eax
        jb     loc_1_1C93
        mov    ecx, eax
loc_1_1C93:
        mov    dword ptr [esi + 0x2b7], eax
        cmp    ecx, 0xe
        jae    loc_1_1CA7
        push   0
        push   0
        push   0
        push   ecx
        jmp    loc_1_1CB8
loc_1_1CA7:
        sub    eax, 0xe
        sub    ecx, 0xe
        push   eax
        lea    edx, [esi + 0x1c9]
        push   ecx
        push   edx
        push   0xe
loc_1_1CB8:
        lea    eax, [esi + 0x1bb]
        mov    edx, dword ptr [esi + 0x14b]
        push   eax
        push   eax
        push   esi
        push   edx
        call   sub_1_3870
        mov    byte ptr [esi + 0x11a], 1
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1CDC:
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1CF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        call   sub_1_0EA0
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        add    edi, 0xe
        cmp    dword ptr [esi + 0x169], 0
        je     loc_1_1DA1
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        mov    ebx, edi
        push   0
        push   ecx
        call   sub_1_3CF0
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1D6F
loc_1_1D2F:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D14
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [esi + 0x169]
        push   ecx
        mov    eax, dword ptr [ebp - 0x18]
        push   ebx
        push   edx
        push   eax
        call   sub_1_1970
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 0xc]
        jb     loc_1_1D6F
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_1D2F
loc_1_1D6F:
        call   sub_1_0ED0
        cmp    edi, ebx
        jbe    loc_1_1D8F
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx], 0
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1D8F:
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
loc_1_1DA1:
        mov    ecx, dword ptr [esi + 0x2b7]
        mov    eax, dword ptr [ebp + 0x14]
        lea    edx, [edi + eax]
        cmp    edx, ecx
        jbe    loc_1_1DD8
        cmp    edi, ecx
        jbe    loc_1_1DD1
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [ecx], 0
        call   sub_1_0ED0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1DD1:
        sub    ecx, edi
        mov    dword ptr [ebp - 0x1c], ecx
        jmp    loc_1_1DDB
loc_1_1DD8:
        mov    dword ptr [ebp - 0x1c], eax
loc_1_1DDB:
        mov    eax, dword ptr [ebp - 0x1c]
        add    edi, dword ptr [esi + 0x1b7]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [esi + 0xf5]
        cmp    edi, eax
        jbe    loc_1_1DF9
        sub    edi, eax
        add    edi, dword ptr [esi + 0xf1]
loc_1_1DF9:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        push   0
        push   ecx
        call   sub_1_3CF0
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_3D14
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_1_1EBA
loc_1_1E2E:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ebx, dword ptr [ebp - 0x10]
        sub    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0x10], eax
        jb     loc_1_1E3E
        mov    ebx, eax
loc_1_1E3E:
        mov    eax, dword ptr [esi + 0xf5]
        lea    ecx, [ebx + edi]
        cmp    ecx, eax
        jbe    loc_1_1E4F
        mov    ebx, eax
        sub    ebx, edi
loc_1_1E4F:
        push   ebx
        mov    eax, dword ptr [ebp - 0x14]
        push   edi
        add    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_04B0
        test   al, al
        je     loc_1_1F2D
        add    edi, ebx
        sub    dword ptr [ebp - 0x10], ebx
        je     loc_1_1EBA
        cmp    dword ptr [esi + 0xf5], edi
        jne    loc_1_1E7A
        mov    edi, dword ptr [esi + 0xf1]
loc_1_1E7A:
        add    dword ptr [ebp - 4], ebx
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0xc]
        jne    loc_1_1EB0
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3D38
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1EBA
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3D14
        mov    dword ptr [ebp - 4], 0
loc_1_1EB0:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1E2E
loc_1_1EBA:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx], eax
        cmp    byte ptr [esi + 0x118], 0
        jne    loc_1_1FF5
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_1FF5
        lea    eax, [ebp - 4]
        push   eax
        push   7
        call   sub_1_4440
        test   byte ptr [ebp - 4], 8
        je     loc_1_1EF4
        push   esi
        call   sub_1_24A0
loc_1_1EF4:
        test   byte ptr [ebp - 4], 2
        je     loc_1_1FF5
        push   2
        push   7
        call   sub_1_4438
        push   esi
        call   sub_1_0710
        mov    byte ptr [esi + 0x5b], 0
        mov    dword ptr [esi + 0x147], 0
        test   byte ptr [esi + 0xfe], 1
        je     loc_1_1F60
        inc    dword ptr [esi + 0x11e]
        jmp    loc_1_1F66
loc_1_1F2D:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp + 0x1c]
        sub    eax, dword ptr [ebp - 0x10]
        push   2
        push   1
        mov    dword ptr [ecx], eax
        push   0xaa000003
        mov    edx, dword ptr [esi + 4]
        push   edx
        call   sub_1_3D98
        add    esp, 0x10
        call   sub_1_0ED0
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_1F60:
        inc    dword ptr [esi + 0x126]
loc_1_1F66:
        mov    edi, dword ptr [esi + 0x57]
        mov    edx, dword ptr [esi + edi*4 + 0xbd]
        lea    ecx, [esi + edi*4]
        add    edx, 0xff
        shr    edx, 8
        lea    eax, [edi + edx]
        cmp    eax, edi
        jbe    loc_1_1F99
        add    ecx, 0x5d
loc_1_1F86:
        mov    dword ptr [ecx], 0
        add    ecx, 4
        inc    edi
        mov    eax, dword ptr [esi + 0x57]
        add    eax, edx
        cmp    eax, edi
        ja     loc_1_1F86
loc_1_1F99:
        mov    eax, dword ptr [esi + 0x53]
        add    eax, edx
        mov    dword ptr [esi + 0x53], eax
        cmp    eax, 0xc
        jne    loc_1_1FAD
        mov    dword ptr [esi + 0x53], 0
loc_1_1FAD:
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_1FC3
        cmp    dword ptr [esi + 0x4f], eax
        je     loc_1_1FC3
        mov    dword ptr [esi + 0x53], 0
loc_1_1FC3:
        mov    eax, dword ptr [esi + 0x57]
        xor    ecx, ecx
        mov    dword ptr [esi + eax*4 + 0x8d], ecx
        mov    byte ptr [esi + 0x1b1], cl
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x5d], 2
        jne    loc_1_1FEE
        push   esi
        mov    dword ptr [esi + 0x57], eax
        mov    byte ptr [esi + 0x5b], 1
        call   sub_1_0670
        jmp    loc_1_1FF5
loc_1_1FEE:
        mov    dword ptr [esi + 0x57], 0xffffffff
loc_1_1FF5:
        call   sub_1_0ED0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2010:
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
        call   sub_1_0EA0
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_2043
        call   sub_1_0ED0
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2043:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_205F
        call   sub_1_0ED0
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_205F:
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
        call   sub_1_3CF0
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_3D14
        test   byte ptr [ebx + 0x71], 2
        je     loc_1_2098
        mov    byte ptr [edi + 8], 0
        jmp    loc_1_20AB
loc_1_2098:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x14b]
        push   eax
        push   ecx
        call   sub_1_3AF0
        mov    byte ptr [edi + 8], al
loc_1_20AB:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_20BC
        mov    eax, dword ptr [esi + 0x4b]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x24], ecx
        jmp    loc_1_20C2
loc_1_20BC:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x47], ecx
loc_1_20C2:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x4b], ecx
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_20FF
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_20EF
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_20EF
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_20FF
loc_1_20EF:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_3CA2
        jmp    loc_1_211E
loc_1_20FF:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_211E
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_211E
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_211E
        push   esi
        call   sub_1_2140
loc_1_211E:
        call   sub_1_0ED0
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2140:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        call   sub_1_0EA0
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_239A
loc_1_215B:
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_239A
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [esi + 0x47]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_3CF0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x57]
        add    eax, 0xff
        shr    eax, 8
        cmp    ecx, -1
        mov    dword ptr [ebp - 4], eax
        jne    loc_1_21EF
        mov    ecx, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + ecx*4 + 0x5d], 0
        jne    loc_1_21B0
        mov    eax, dword ptr [esi + 0x4f]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0xc
        jbe    loc_1_2224
        mov    dword ptr [esi + 0x4f], 0
        jmp    loc_1_2224
loc_1_21B0:
        mov    edx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [ebp - 4]
        cmp    edx, ecx
        jae    loc_1_21CE
        add    eax, edx
        cmp    eax, ecx
        jbe    loc_1_2224
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_21CE:
        add    eax, edx
        cmp    eax, 0xc
        jbe    loc_1_2224
        mov    dword ptr [esi + 0x4f], 0
        cmp    dword ptr [ebp - 4], ecx
        jbe    loc_1_2224
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_21EF:
        mov    edx, dword ptr [esi + 0x4f]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, edx
        jbe    loc_1_220D
        add    eax, edx
        cmp    eax, ecx
        jbe    loc_1_2224
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_220D:
        add    eax, edx
        cmp    eax, 0xc
        jbe    loc_1_2224
        mov    dword ptr [esi + 0x4f], 0
        cmp    dword ptr [ebp - 4], ecx
        ja     loc_1_23A8
loc_1_2224:
        mov    ecx, dword ptr [esi + 0x4f]
        mov    ebx, dword ptr [esi + 0x47]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esi + 0x47], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        cmp    ecx, eax
        jae    loc_1_224A
        lea    edi, [esi + ecx*4 + 0x5d]
        mov    eax, 1
        mov    ecx, dword ptr [ebp - 4]
        rep stosd dword ptr es:[edi], eax
loc_1_224A:
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [esi + ecx*4 + 0xbd]
        mov    dword ptr [esi + ecx*4 + 0x8d], ebx
        lea    eax, [esi + ecx*4]
        push   edi
        push   ebx
        mov    dword ptr [ebp - 0xc], eax
        push   esi
        call   sub_1_0230
        test   al, al
        jne    loc_1_22AE
        push   0xc0000001
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_3C0A
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_2296
        mov    edi, dword ptr [ebp - 0xc]
        xor    eax, eax
        add    edi, 0x5d
        mov    ecx, dword ptr [ebp - 4]
        rep stosd dword ptr es:[edi], eax
loc_1_2296:
        mov    eax, dword ptr [ebx + 0x28]
        mov    ecx, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x1c]
        mov    dword ptr [ecx + 0x8d], 0
        jmp    loc_1_2390
loc_1_22AE:
        mov    edx, dword ptr [edi]
        cmp    edx, 0x3c
        jae    loc_1_22D8
        mov    eax, 0x3c
        mov    ecx, dword ptr [ebp - 8]
        shl    ecx, 8
        sub    eax, edx
        push   eax
        push   0                             ; -> loc_1_4174
        mov    eax, dword ptr [esi + 0xed]
        add    eax, ecx
        add    eax, edx
        push   eax
        call   sub_1_03D0
loc_1_22D8:
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_23B6
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_2300
        mov    edi, dword ptr [ebp - 0xc]
        mov    eax, 2
        add    edi, 0x5d
        mov    ecx, dword ptr [ebp - 4]
        rep stosd dword ptr es:[edi], eax
loc_1_2300:
        mov    eax, dword ptr [esi + 0x4f]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x4f], eax
        cmp    eax, 0xc
        jne    loc_1_2315
        mov    dword ptr [esi + 0x4f], 0
loc_1_2315:
        cmp    dword ptr [esi + 0x57], -1
        jne    loc_1_2350
        mov    eax, dword ptr [esi + 0x53]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_2331
        cmp    dword ptr [esi + 0x4f], eax
        je     loc_1_2331
        mov    dword ptr [esi + 0x53], 0
loc_1_2331:
        cmp    byte ptr [esi + 0x118], 0
        mov    eax, dword ptr [esi + 0x53]
        mov    dword ptr [esi + 0x57], eax
        je     loc_1_2346
        mov    byte ptr [esi + 0x5c], 1
        jmp    loc_1_2350
loc_1_2346:
        mov    byte ptr [esi + 0x5b], 1
        push   esi
        call   sub_1_0670
loc_1_2350:
        cmp    byte ptr [ebx + 0x2c], 0
        jne    loc_1_2369
        mov    edi, dword ptr [ebx + 0x28]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [edi + 0x1c]
        jmp    loc_1_2390
loc_1_2369:
        cmp    dword ptr [esi + 0x161], 0
        jne    loc_1_237A
        mov    dword ptr [esi + 0x161], ebx
        jmp    loc_1_2383
loc_1_237A:
        mov    eax, dword ptr [esi + 0x165]
        mov    dword ptr [eax + 0x24], ebx
loc_1_2383:
        mov    dword ptr [esi + 0x165], ebx
        mov    dword ptr [ebx + 0x24], 0
loc_1_2390:
        cmp    dword ptr [esi + 0x47], 0
        jne    loc_1_215B
loc_1_239A:
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_23A8:
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_23B6:
        mov    eax, dword ptr [ebx + 0x28]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_23D5
        mov    ecx, dword ptr [ebp - 8]
        xor    eax, eax
        lea    edi, [esi + ecx*4 + 0x5d]
        mov    ecx, dword ptr [ebp - 4]
        rep stosd dword ptr es:[edi], eax
loc_1_23D5:
        push   0
        mov    eax, dword ptr [ebx + 0x28]
        push   ebx
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_3C0A
        push   3
        push   esi
        call   sub_1_33A0
        mov    ecx, dword ptr [ebp - 0xc]
        dec    dword ptr [ecx + 0x1c]
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_2400:
        push   ebp
        mov    ebp, esp
        push   esi
        pushf  
        cli    
        call   sub_1_0EA0
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x1b1], 0
        je     loc_1_2467
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_2474
        mov    byte ptr [esi + 0x1b1], 0
        push   3
        mov    eax, dword ptr [esi + 4]
        push   1
        push   0xaa000003
        push   eax
        call   sub_1_3D98
        add    esp, 0x10
        push   esi
        call   sub_1_0150
        cmp    dword ptr [esi + 0x147], 4
        jbe    loc_1_2459
        push   esi
        call   sub_1_01C0
        mov    dword ptr [esi + 0x147], 0
loc_1_2459:
        push   esi
        call   sub_1_24A0
        inc    dword ptr [esi + 0x147]
        jmp    loc_1_2474
loc_1_2467:
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_2474
        mov    byte ptr [esi + 0x1b1], 1
loc_1_2474:
        push   0x7d0
        add    esi, 0x1a5
        push   esi
        call   sub_1_3CA2
        call   sub_1_0ED0
        popf   
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_24A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   8
        push   7
        mov    edi, 0x14
        call   sub_1_4438
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0660
loc_1_24BD:
        push   0x64
        call   sub_1_3CC0
        dec    edi
        jne    loc_1_24BD
        push   2
        push   0xd
        call   sub_1_4438
        push   0x22
        push   0
        call   sub_1_4438
        push   0
        push   0xd
        call   sub_1_4438
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_24F5
        mov    byte ptr [esi + 0x5b], 1
        push   esi
        call   sub_1_0670
loc_1_24F5:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2500:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x28]
        test   edx, edx
        je     loc_1_2520
loc_1_2510:
        mov    esi, dword ptr [edx + 0x18]
        cmp    ecx, esi
        jae    loc_1_2519
        mov    ecx, esi
loc_1_2519:
        mov    edx, dword ptr [edx + 0xc]
        test   edx, edx
        jne    loc_1_2510
loc_1_2520:
        test   ecx, ecx
        jne    loc_1_2529
        mov    ecx, 0xee
loc_1_2529:
        pop    esi
        mov    dword ptr [eax + 0x15d], ecx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2540:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x20], 0
        mov    esi, dword ptr [ebx + 8]
        je     loc_1_2563
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2563:
        call   sub_1_0EA0
        inc    dword ptr [esi + 0x30]
        inc    dword ptr [ebx + 0x1c]
        mov    ecx, dword ptr [esi + 0x28]
        cmp    ecx, ebx
        jne    loc_1_257D
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0x28], eax
        jmp    loc_1_2590
loc_1_257D:
        cmp    dword ptr [ecx + 0xc], ebx
        je     loc_1_258A
loc_1_2582:
        mov    ecx, dword ptr [ecx + 0xc]
        cmp    dword ptr [ecx + 0xc], ebx
        jne    loc_1_2582
loc_1_258A:
        mov    edx, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0xc], edx
loc_1_2590:
        push   0
        mov    eax, dword ptr [ebx + 0x14]
        push   eax
        mov    ecx, dword ptr [esi + 0x14b]
        push   ecx
        call   sub_2_1780
loc_1_25A2:
        mov    edi, eax
        test   edi, edi
        jne    loc_1_25C7
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    eax, 2
        je     loc_1_25BF
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        mov    edi, 0x103
        jmp    loc_1_25ED
loc_1_25BF:
        sub    eax, 2
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_1_25ED
loc_1_25C7:
        cmp    edi, 0x103
        jne    loc_1_25D9
        mov    byte ptr [ebx + 0x20], 1
        sub    dword ptr [ebx + 0x1c], 2
        jmp    loc_1_25ED
loc_1_25D9:
        cmp    edi, 0xc001000e
        jne    loc_1_25EA
        mov    byte ptr [ebx + 0x20], 1
        mov    edi, 0x103
loc_1_25EA:
        dec    dword ptr [ebx + 0x1c]
loc_1_25ED:
        cmp    dword ptr [ebx + 0x1c], 0
        jne    loc_1_2622
        mov    eax, dword ptr [ebx + 0x18]
        cmp    dword ptr [esi + 0x15d], eax
        jne    loc_1_2604
        push   esi
        call   sub_1_2500
loc_1_2604:
        push   0
        push   0x75
        push   ebx
        call   sub_1_3CCC
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_2630
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_1_2630
        push   esi
        call   sub_1_0150
        jmp    loc_1_2630
loc_1_2622:
        mov    eax, dword ptr [esi + 0x2c]
        mov    edi, 0x103
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [esi + 0x2c], ebx
loc_1_2630:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_2661
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_2651
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_2651
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_2661
loc_1_2651:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_3CA2
        jmp    loc_1_2680
loc_1_2661:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_2680
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_2680
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_2680
        push   esi
        call   sub_1_2140
loc_1_2680:
        call   sub_1_0ED0
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2690:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        call   sub_1_0EA0
        inc    dword ptr [ebx + 0x1c]
        inc    dword ptr [edi + 0x30]
        cmp    byte ptr [ebx + 0x20], 0
        je     loc_1_26B4
        mov    esi, 0xc0010002
        jmp    loc_1_26F0
loc_1_26B4:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x10]
        test   eax, eax
        jne    loc_1_26CA
        push   ecx
        push   ebx
        push   edi
        call   sub_1_2AE0
        mov    esi, eax
        jmp    loc_1_26F0
loc_1_26CA:
        cmp    eax, 1
        jne    loc_1_26EB
        cmp    byte ptr [edi + 0x13e], 0
        je     loc_1_26DF
        mov    esi, 0xc001000d
        jmp    loc_1_26F0
loc_1_26DF:
        push   ecx
        push   ebx
        push   edi
        call   sub_1_2B30
        mov    esi, eax
        jmp    loc_1_26F0
loc_1_26EB:
        mov    esi, 0x10001
loc_1_26F0:
        dec    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_1_2724
        cmp    byte ptr [edi + 0x13e], 0
        jne    loc_1_2714
        cmp    dword ptr [edi + 0x16d], 0
        jne    loc_1_2714
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_1_2724
loc_1_2714:
        push   1
        add    edi, 0x179
        push   edi
        call   sub_1_3CA2
        jmp    loc_1_2743
loc_1_2724:
        cmp    dword ptr [edi + 0x47], 0
        je     loc_1_2743
        mov    eax, dword ptr [edi + 0x4f]
        cmp    dword ptr [edi + eax*4 + 0x5d], 0
        jne    loc_1_2743
        cmp    byte ptr [edi + 0x11d], 0
        jne    loc_1_2743
        push   edi
        call   sub_1_2140
loc_1_2743:
        call   sub_1_0ED0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2760:
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
        ja     loc_1_279D
        je     loc_1_292F
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_27BE
        jmp    dword ptr [eax*4]             ; -> obj1:0x2A8C
loc_1_279D:
        cmp    eax, 0x1010102
        je     loc_1_2951
        cmp    eax, 0x1010103
        je     loc_1_2973
        cmp    eax, 0x1010104
        je     loc_1_2A38
loc_1_27BE:
        mov    dword ptr [ebp - 0xc], 0xc00000bb
        jmp    loc_1_2A41
loc_1_27CA:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_27DF
        mov    esi, 0                        ; -> loc_1_427C
        mov    edi, 0x5c
        jmp    loc_1_2A41
loc_1_27DF:
        mov    esi, 0                        ; -> loc_1_4200
        mov    edi, 0x7c
        jmp    loc_1_2A41
loc_1_27EE:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x14], 2
        cmp    byte ptr [ebx + 0x13e], 0
        jne    loc_1_2808
        mov    dword ptr [ebp - 0x14], 0
loc_1_2808:
        lea    esi, [ebp - 0x14]
        mov    edi, 4
        jmp    loc_1_2A41
loc_1_2815:
        lea    esi, [ebp - 0x18]
        mov    edi, 4
        jmp    loc_1_2A41
loc_1_2822:
        mov    dword ptr [ebp - 8], 0xee
        jmp    loc_1_2A41
loc_1_282E:
        mov    dword ptr [ebp - 8], 0x5dc
        jmp    loc_1_2A41
loc_1_283A:
        mov    dword ptr [ebp - 8], 0x186a0
        jmp    loc_1_2A41
loc_1_2846:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x38]
        shl    eax, 8
        jmp    loc_1_2A3E
loc_1_2854:
        mov    eax, 0x20
        mov    ebx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ebx + 0x38]
        shl    eax, 8
        jmp    loc_1_2A3E
loc_1_2867:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_2A41
loc_1_2873:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_2A41
loc_1_287F:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x10a
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        call   sub_1_3DAC
        and    dword ptr [ebp - 8], 0xffffff00
        jmp    loc_1_2A41
loc_1_28A8:
        mov    esi, 0                        ; -> loc_1_437C
        mov    edi, 0x15
        jmp    loc_1_2A41
loc_1_28B7:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_28D1
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x14b]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_2A41
loc_1_28D1:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x14]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_2A41
loc_1_28E2:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_28F6
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x15d]
        jmp    loc_1_2A3E
loc_1_28F6:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_2A41
loc_1_2904:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_2A41
loc_1_2917:
        mov    dword ptr [ebp - 8], 0x5ea
        jmp    loc_1_2A41
loc_1_2923:
        mov    dword ptr [ebp - 8], 0xf
        jmp    loc_1_2A41
loc_1_292F:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x10a
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_3DAC
        jmp    loc_1_2A41
loc_1_2951:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0x104
        lea    eax, [ebp - 0x20]
        push   ebx
        lea    esi, [ebp - 0x20]
        push   eax
        mov    edi, 6
        call   sub_1_3DAC
        jmp    loc_1_2A41
loc_1_2973:
        cmp    byte ptr [ebp + 0x14], 0
        mov    ebx, dword ptr [ebp + 8]
        je     loc_1_29D3
        mov    ecx, dword ptr [ebx + 0x14b]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x10], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    dword ptr [ebp + 0x1c], edx
        jae    loc_1_29A8
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], edx
        jmp    loc_1_2A41
loc_1_29A8:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x10]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0xc]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_1CA0
loc_1_29BE:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ecx], eax
        je     loc_1_2A47
        jmp    loc_1_2A41
loc_1_29D3:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebx + 0x14b]
        mov    ecx, dword ptr [eax + 0x14]
        push   ecx
        push   edx
        call   sub_2_1BA0
loc_1_29E6:
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 0x10], eax
        lea    ecx, [edx*2]
        cmp    dword ptr [ebp + 0x1c], ecx
        jae    loc_1_2A06
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0xc], 0xc0010014
        mov    dword ptr [eax], ecx
        jmp    loc_1_2A41
loc_1_2A06:
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
        call   sub_2_1BE0
loc_1_2A29:
        mov    ecx, dword ptr [ebp - 0x10]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_2A41
loc_1_2A38:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x41]
loc_1_2A3E:
        mov    dword ptr [ebp - 8], eax
loc_1_2A41:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2A7D
loc_1_2A47:
        cmp    dword ptr [ebp + 0x10], 0x1010103
        je     loc_1_2A7D
        cmp    edi, dword ptr [ebp + 0x1c]
        jbe    loc_1_2A6D
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
loc_1_2A6D:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   esi
        push   eax
        call   sub_1_3DAC
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], edi
loc_1_2A7D:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
        ; ---- dispatch table (19 entries) ----
        dd  loc_1_27CA
        dd  loc_1_27EE
        dd  loc_1_2815
        dd  loc_1_2815
        dd  loc_1_2822
        dd  loc_1_282E
        dd  loc_1_283A
        dd  loc_1_2846
        dd  loc_1_2854
        dd  loc_1_2867
        dd  loc_1_2873
        dd  loc_1_287F
        dd  loc_1_28A8
        dd  loc_1_28B7
        dd  loc_1_28E2
        dd  loc_1_2904
        dd  loc_1_2917
        dd  loc_1_27BE
        dd  loc_1_2923
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2AE0:
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
        call   sub_1_2760
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B30:
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
        ja     loc_1_2B6D
        je     loc_1_2C7C
        cmp    eax, 0x1010e
        je     loc_1_2B7F
        cmp    eax, 0x1010f
        je     loc_1_2BDA
        cmp    eax, 0x10112
        je     loc_1_2C55
loc_1_2B6D:
        mov    ebx, 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_2CB7
loc_1_2B7F:
        cmp    edi, 4
        je     loc_1_2B96
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_2CB7
loc_1_2B96:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_3DAC
        test   dword ptr [ebp - 8], 0xf050
        je     loc_1_2BC3
        mov    dword ptr [esi + 0x20], 4
        mov    dword ptr [esi + 0x24], 0
        mov    ebx, 0xc00000bb
        jmp    loc_1_2CB7
loc_1_2BC3:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_2CD0
        jmp    loc_1_2CB5
loc_1_2BDA:
        cmp    edi, 4
        je     loc_1_2BF1
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_2CB7
loc_1_2BF1:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_3DAC
        cmp    dword ptr [ebp - 4], 0xee
        ja     loc_1_2C4E
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x15d]
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_1_2C28
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x15d], eax
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_2CB7
loc_1_2C28:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], eax
        jne    loc_1_2C46
        mov    eax, dword ptr [ecx + 0x18]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_1_2C46
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 0x18], eax
        call   sub_1_2500
        jmp    loc_1_2CB7
loc_1_2C46:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_2CB7
loc_1_2C4E:
        mov    ebx, 0xc0010014
        jmp    loc_1_2CB7
loc_1_2C55:
        cmp    edi, 4
        je     loc_1_2C69
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_2CB7
loc_1_2C69:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x71
        push   ecx
        call   sub_1_3DAC
        jmp    loc_1_2CB7
loc_1_2C7C:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     loc_1_2C9A
        mov    ebx, 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_2CB7
loc_1_2C9A:
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
        call   sub_1_2D30
loc_1_2CB5:
        mov    ebx, eax
loc_1_2CB7:
        test   ebx, ebx
        jne    loc_1_2CC5
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
loc_1_2CC5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_2CD0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 0x13e], 0
        jne    loc_1_2D19
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_2D0F
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
        call   sub_2_1B20
loc_1_2D07:
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x10
loc_1_2D0F:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x10
loc_1_2D19:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D30:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 0x13e], 0
        jne    loc_1_2D7D
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_2D73
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
        call   sub_2_1840
loc_1_2D6B:
        dec    dword ptr [esi + 0x1c]
        pop    esi
        pop    ebp
        ret    0x14
loc_1_2D73:
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    0x14
loc_1_2D7D:
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D90:
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
        call   sub_1_2760
        cmp    eax, 0xc00000bb
        mov    edi, eax
        jne    loc_1_2E76
        xor    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1020101
        ja     loc_1_2E03
        je     loc_1_2E40
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_2E11
        jmp    dword ptr [eax*4]             ; -> obj1:0x2E90
loc_1_2E03:
        cmp    eax, 0x1020102
        je     loc_1_2E48
        cmp    eax, 0x1020103
        je     loc_1_2E50
loc_1_2E11:
        mov    edi, 0xc0010017
        jmp    loc_1_2E59
loc_1_2E18:
        mov    eax, dword ptr [ebx + 0x11e]
        jmp    loc_1_2E56
loc_1_2E20:
        mov    eax, dword ptr [ebx + 0x122]
        jmp    loc_1_2E56
loc_1_2E28:
        mov    eax, dword ptr [ebx + 0x126]
        jmp    loc_1_2E56
loc_1_2E30:
        mov    eax, dword ptr [ebx + 0x136]
        jmp    loc_1_2E56
loc_1_2E38:
        mov    eax, dword ptr [ebx + 0x13a]
        jmp    loc_1_2E56
loc_1_2E40:
        mov    eax, dword ptr [ebx + 0x132]
        jmp    loc_1_2E56
loc_1_2E48:
        mov    eax, dword ptr [ebx + 0x12a]
        jmp    loc_1_2E56
loc_1_2E50:
        mov    eax, dword ptr [ebx + 0x12e]
loc_1_2E56:
        mov    dword ptr [ebp - 4], eax
loc_1_2E59:
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_2E76
        test   edi, edi
        jne    loc_1_2E76
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_3DAC
        add    dword ptr [ebp - 8], 4
loc_1_2E76:
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
        dd  loc_1_2E18
        dd  loc_1_2E20
        dd  loc_1_2E28
        dd  loc_1_2E30
        dd  loc_1_2E38
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2EB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        call   sub_1_0EA0
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ecx + 0x14]
        cmp    eax, 0x10113
        ja     loc_1_2EDB
        je     loc_1_2F0C
        cmp    eax, 0x10101
        jb     loc_1_2F07
        cmp    eax, 0x10111
        jbe    loc_1_2F0C
        jmp    loc_1_2F07
loc_1_2EDB:
        cmp    eax, 0x1010104
        ja     loc_1_2EF9
        cmp    eax, 0x1010101
        jae    loc_1_2F0C
        cmp    eax, 0x20101
        jb     loc_1_2F07
        cmp    eax, 0x20105
        jbe    loc_1_2F0C
        jmp    loc_1_2F07
loc_1_2EF9:
        cmp    eax, 0x1020101
        jb     loc_1_2F07
        cmp    eax, 0x1020103
        jbe    loc_1_2F0C
loc_1_2F07:
        mov    esi, 0xc0010017
loc_1_2F0C:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 0x30]
        test   esi, esi
        jne    loc_1_2F1F
        push   ecx
        push   edi
        call   sub_1_2D90
        mov    esi, eax
loc_1_2F1F:
        mov    eax, dword ptr [edi + 0x30]
        dec    eax
        mov    dword ptr [edi + 0x30], eax
        jne    loc_1_2F50
        cmp    byte ptr [edi + 0x13e], 0
        jne    loc_1_2F40
        cmp    dword ptr [edi + 0x16d], 0
        jne    loc_1_2F40
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_1_2F50
loc_1_2F40:
        push   1
        add    edi, 0x179
        push   edi
        call   sub_1_3CA2
        jmp    loc_1_2F6F
loc_1_2F50:
        cmp    dword ptr [edi + 0x47], 0
        je     loc_1_2F6F
        mov    eax, dword ptr [edi + 0x4f]
        cmp    dword ptr [edi + eax*4 + 0x5d], 0
        jne    loc_1_2F6F
        cmp    byte ptr [edi + 0x11d], 0
        jne    loc_1_2F6F
        push   edi
        call   sub_1_2140
loc_1_2F6F:
        call   sub_1_0ED0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_2F80:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x41B4
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_3BEC
        push   0
        mov    ecx, dword ptr [4]            ; -> obj1:0x41B8
        push   ecx
        call   sub_1_3BE0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x1b2], 1
        call   sub_1_0EA0
        cmp    esi, dword ptr [0x3e]         ; -> obj1:0x41F2
        jne    loc_1_2FD9
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x3e], eax         ; -> obj1:0x41F2
        jmp    loc_1_2FF5
loc_1_2FD9:
        mov    ecx, dword ptr [0x3e]         ; -> obj1:0x41F2
        cmp    dword ptr [ecx + 0x24], esi
        je     loc_1_2FEC
loc_1_2FE4:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], esi
        jne    loc_1_2FE4
loc_1_2FEC:
        mov    eax, dword ptr [ecx + 0x24]
        mov    edx, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], edx
loc_1_2FF5:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x1a5]
        push   eax
        mov    edi, 0x61a8
        push   ecx
        call   sub_1_3CA8
loc_1_300A:
        push   0x64
        call   sub_1_3CC0
        dec    edi
        jne    loc_1_300A
        mov    eax, dword ptr [esi + 0x14b]
        push   eax
        call   sub_2_1660
loc_1_3020:
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_3CB4
        lea    eax, [esi + 0x2bb]
        push   eax
        call   sub_1_3CB4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3BF8
        push   0
        push   0x2d3
        push   esi
        call   sub_1_3CCC
        call   sub_1_0ED0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_3060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x20], 0
        mov    esi, dword ptr [ebx + 8]
        je     loc_1_3083
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3083:
        call   sub_1_0EA0
        inc    dword ptr [ebx + 0x1c]
        inc    dword ptr [esi + 0x30]
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_30B0
        dec    dword ptr [ebx + 0x1c]
        dec    dword ptr [esi + 0x30]
        call   sub_1_0ED0
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_30B0:
        mov    edi, dword ptr [esi + 0x28]
        mov    dword ptr [ebp - 4], 0
        test   edi, edi
        je     loc_1_30E0
loc_1_30BE:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi + 0x1c]
        push   0x40010004
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_3C2E
        mov    eax, dword ptr [edi + 0xc]
        dec    dword ptr [edi + 0x1c]
        mov    edi, eax
        test   eax, eax
        jne    loc_1_30BE
loc_1_30E0:
        mov    byte ptr [esi + 0x13e], 1
        mov    dword ptr [esi + 0x13f], 0
        push   esi
        mov    dword ptr [esi + 0x143], ebx
        call   sub_1_3170
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_3134
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_3124
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_3124
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_3134
loc_1_3124:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_3CA2
        jmp    loc_1_3153
loc_1_3134:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_3153
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_3153
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_3153
        push   esi
        call   sub_1_2140
loc_1_3153:
        call   sub_1_0ED0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3170:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x30]
        cmp    dword ptr [esi + 0x16d], 0
        je     loc_1_31BC
loc_1_3184:
        mov    ecx, dword ptr [esi + 0x16d]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x16d], eax
        mov    eax, dword ptr [ecx + 8]
        mov    edi, dword ptr [ecx + 4]
        cmp    eax, 4
        je     loc_1_31B0
        cmp    eax, 8
        je     loc_1_31B0
        push   0xc001000c
        mov    eax, dword ptr [edi]
        push   ecx
        push   eax
        call   sub_1_3C1C
loc_1_31B0:
        dec    dword ptr [edi + 0x1c]
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_3184
loc_1_31BC:
        cmp    dword ptr [esi + 0x57], -1
        je     loc_1_31DA
        mov    eax, 0x103
        pop    edi
        mov    dword ptr [esi + 0x13f], 2
        dec    dword ptr [esi + 0x30]
        pop    esi
        pop    ebp
        ret    4
loc_1_31DA:
        push   esi
        dec    dword ptr [esi + 0x30]
        call   sub_1_31F0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_31F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    ecx, dword ptr [esi + 0x30]
        inc    ecx
        mov    eax, dword ptr [esi + 0x38]
        mov    dword ptr [esi + 0x30], ecx
        mov    dword ptr [esi + 0x4f], edi
        mov    dword ptr [esi + 0x53], edi
        cmp    eax, edi
        je     loc_1_321E
        lea    edx, [esi + 0x5d]
loc_1_3211:
        cmp    dword ptr [edx], 0
        jne    loc_1_322E
        add    edx, 4
        inc    edi
        cmp    eax, edi
        ja     loc_1_3211
loc_1_321E:
        dec    ecx
        push   esi
        mov    dword ptr [esi + 0x30], ecx
        call   sub_1_3250
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_322E:
        dec    ecx
        mov    eax, 0x103
        pop    edi
        mov    dword ptr [esi + 0x30], ecx
        mov    dword ptr [esi + 0x13f], 3
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3250:
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
        jbe    loc_1_32A0
        lea    edi, [esi + 0x8d]
loc_1_3272:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        je     loc_1_3292
        mov    ebx, dword ptr [ecx + 0x28]
        push   0xc001000c
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [ebx + 0x1c]
        mov    dword ptr [edi], 0
loc_1_3292:
        add    edi, 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0x38], eax
        ja     loc_1_3272
loc_1_32A0:
        mov    edx, dword ptr [esi + 0x47]
        test   edx, edx
        je     loc_1_32CA
loc_1_32A7:
        lea    eax, [edx + 0x24]
        push   0xc001000c
        push   edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x47], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0x47]
        test   edx, edx
        jne    loc_1_32A7
loc_1_32CA:
        mov    edx, dword ptr [esi + 0x161]
        test   edx, edx
        je     loc_1_32FD
loc_1_32D4:
        lea    eax, [edx + 0x24]
        push   0xc001000c
        push   edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x161], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C0A
        dec    dword ptr [edi + 0x1c]
        mov    edx, dword ptr [esi + 0x161]
        test   edx, edx
        jne    loc_1_32D4
loc_1_32FD:
        cmp    byte ptr [esi + 0x11d], 0
        je     loc_1_331E
loc_1_3306:
        mov    edi, 0x64
loc_1_330B:
        push   0x64
        call   sub_1_3CC0
        dec    edi
        jne    loc_1_330B
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_3306
loc_1_331E:
        mov    byte ptr [esi + 0x15c], 0x1b
        push   esi
        call   sub_1_01C0
        mov    edi, dword ptr [esi + 0x28]
        cmp    al, 1
        sbb    eax, eax
        xor    ebx, ebx
        and    eax, 0xc0000001
        mov    byte ptr [esi + 0x13e], bl
        mov    dword ptr [ebp - 4], eax
        test   edi, edi
        je     loc_1_3384
loc_1_3346:
        inc    dword ptr [edi + 0x1c]
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_335D
        push   ebx
        mov    eax, dword ptr [edi]
        push   ebx
        push   0x40010007
        push   eax
        call   sub_1_3C2E
loc_1_335D:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   0x40010005
        push   ecx
        call   sub_1_3C2E
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_3C34
        mov    eax, dword ptr [edi + 0xc]
        dec    dword ptr [edi + 0x1c]
        mov    edi, eax
        test   eax, eax
        jne    loc_1_3346
loc_1_3384:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        dec    dword ptr [esi + 0x30]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_33A0:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x13f], eax
        jne    loc_1_348D
        cmp    eax, 1
        je     loc_1_3423
        cmp    eax, 2
        je     loc_1_342B
        cmp    eax, 3
        je     loc_1_3433
        mov    eax, dword ptr [ebp - 4]
loc_1_33CD:
        cmp    eax, 0x103
        je     loc_1_33EC
        push   eax
        mov    eax, dword ptr [esi + 0x143]
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_3C16
        mov    ecx, dword ptr [esi + 0x143]
        dec    dword ptr [ecx + 0x1c]
loc_1_33EC:
        mov    eax, dword ptr [esi + 0x30]
        dec    eax
        mov    dword ptr [esi + 0x30], eax
        jne    loc_1_346E
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_340D
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_340D
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_346E
loc_1_340D:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_3CA2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3423:
        push   esi
        call   sub_1_3170
        jmp    loc_1_33CD
loc_1_342B:
        push   esi
        call   sub_1_31F0
        jmp    loc_1_33CD
loc_1_3433:
        mov    edx, dword ptr [esi + 0x30]
        xor    eax, eax
        inc    edx
        mov    ecx, dword ptr [esi + 0x38]
        mov    dword ptr [esi + 0x30], edx
        cmp    ecx, eax
        je     loc_1_3453
        lea    edi, [esi + 0x5d]
loc_1_3446:
        cmp    dword ptr [edi], 0
        jne    loc_1_3462
        add    edi, 4
        inc    eax
        cmp    eax, ecx
        jb     loc_1_3446
loc_1_3453:
        dec    edx
        push   esi
        mov    dword ptr [esi + 0x30], edx
        call   sub_1_3250
        jmp    loc_1_33CD
loc_1_3462:
        dec    edx
        pop    edi
        mov    dword ptr [esi + 0x30], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_346E:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_348D
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_348D
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_348D
        push   esi
        call   sub_1_2140
loc_1_348D:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_34A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x18]
        mov    esi, dword ptr [ebp + 0x1c]
        mov    edi, dword ptr [ebx + 8]
        call   sub_1_0EA0
        test   esi, esi
        jne    loc_1_34BB
        lea    esi, [ebx + 0x49]
loc_1_34BB:
        cmp    byte ptr [edi + 0x13e], 0
        je     loc_1_34CB
        mov    esi, 0xc001000d
        jmp    loc_1_3510
loc_1_34CB:
        mov    dword ptr [esi + 4], ebx
        inc    dword ptr [ebx + 0x1c]
        cmp    byte ptr [ebp + 0x20], 1
        sbb    eax, eax
        and    eax, 0xfffffffa
        add    eax, 0xa
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [edi + 0x16d], 0
        jne    loc_1_34FD
        mov    dword ptr [edi + 0x171], esi
        mov    dword ptr [edi + 0x16d], esi
        jmp    loc_1_350B
loc_1_34FD:
        mov    eax, dword ptr [edi + 0x171]
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0x171], esi
loc_1_350B:
        mov    esi, 0x103
loc_1_3510:
        call   sub_1_0ED0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
        db  08Bh, 0FFh
loc_1_3520:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebx + 8]
        call   sub_1_0EA0
        test   edi, edi
        jne    loc_1_353B
        lea    edi, [ebx + 0x21]
loc_1_353B:
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_354B
        mov    esi, 0xc001000d
        jmp    loc_1_358D
loc_1_354B:
        mov    dword ptr [edi + 4], ebx
        inc    dword ptr [ebx + 0x1c]
        cmp    byte ptr [ebp + 0x18], 1
        sbb    eax, eax
        add    eax, 9
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi], 0
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_357A
        mov    dword ptr [esi + 0x171], edi
        mov    dword ptr [esi + 0x16d], edi
        jmp    loc_1_3588
loc_1_357A:
        mov    eax, dword ptr [esi + 0x171]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x171], edi
loc_1_3588:
        mov    esi, 0x103
loc_1_358D:
        call   sub_1_0ED0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h
loc_1_35A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 0x1c]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_35B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_0EA0
        mov    esi, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x30]
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_3670
        cmp    dword ptr [esi + 0x175], 0
        jne    loc_1_3670
loc_1_35DB:
        mov    edi, dword ptr [esi + 0x16d]
        test   edi, edi
        je     loc_1_3646
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x16d], eax
        cmp    dword ptr [esi + 0x171], edi
        jne    loc_1_35FF
        mov    dword ptr [esi + 0x171], 0
loc_1_35FF:
        mov    dword ptr [esi + 0x175], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     loc_1_361A
        cmp    eax, 0xa
        je     loc_1_361A
        push   esi
        call   sub_1_3730
        jmp    loc_1_3620
loc_1_361A:
        push   esi
        call   sub_1_37A0
loc_1_3620:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     loc_1_363A
        cmp    ecx, 4
        je     loc_1_363A
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_3C1C
loc_1_363A:
        dec    dword ptr [ebx + 0x1c]
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_35DB
loc_1_3646:
        mov    dword ptr [esi + 0x175], 0
        cmp    byte ptr [esi + 0x13e], 0
        je     loc_1_3670
        push   1
        dec    dword ptr [esi + 0x30]
        push   esi
        call   sub_1_33A0
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_1_3670:
        mov    edi, dword ptr [esi + 0x2c]
        test   edi, edi
        je     loc_1_36D6
        xor    ebx, ebx
loc_1_3679:
        cmp    dword ptr [edi + 0x1c], 0
        jbe    loc_1_3686
        mov    edi, dword ptr [edi + 0xc]
        mov    ebx, edi
        jmp    loc_1_36C0
loc_1_3686:
        mov    eax, dword ptr [esi + 0x15d]
        cmp    dword ptr [edi + 0x18], eax
        jne    loc_1_3697
        push   esi
        call   sub_1_2500
loc_1_3697:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_3C04
        lea    ecx, [edi + 0xc]
        test   ebx, ebx
        mov    eax, dword ptr [ecx]
        je     loc_1_36AF
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_36B2
loc_1_36AF:
        mov    dword ptr [esi + 0x2c], eax
loc_1_36B2:
        mov    eax, edi
        push   0
        mov    edi, dword ptr [ecx]
        push   0x75
        push   eax
        call   sub_1_3CCC
loc_1_36C0:
        test   edi, edi
        jne    loc_1_3679
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_1_36D6
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_36D6
        push   esi
        call   sub_1_0150
loc_1_36D6:
        dec    dword ptr [esi + 0x30]
        jne    loc_1_3703
        cmp    byte ptr [esi + 0x13e], 0
        jne    loc_1_36F3
        cmp    dword ptr [esi + 0x16d], 0
        jne    loc_1_36F3
        cmp    dword ptr [esi + 0x2c], 0
        je     loc_1_3703
loc_1_36F3:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_3CA2
        jmp    loc_1_3722
loc_1_3703:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_1_3722
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_1_3722
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_1_3722
        push   esi
        call   sub_1_2140
loc_1_3722:
        call   sub_1_0ED0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_3730:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0x14b]
        mov    edi, dword ptr [eax + 0xc]
        test   edi, 0x24
        je     loc_1_3751
        call   sub_1_0790
        jmp    loc_1_3756
loc_1_3751:
        call   sub_1_37A0
loc_1_3756:
        test   edi, 0x26
        lea    eax, [esi + 0x15b]
        je     loc_1_3769
        or     byte ptr [eax], 8
        jmp    loc_1_376C
loc_1_3769:
        and    byte ptr [eax], 0xf7
loc_1_376C:
        test   edi, 0x20
        je     loc_1_3779
        or     byte ptr [eax], 0x10
        jmp    loc_1_377C
loc_1_3779:
        and    byte ptr [eax], 0xef
loc_1_377C:
        test   edi, 0x28
        je     loc_1_3789
        or     byte ptr [eax], 4
        jmp    loc_1_378C
loc_1_3789:
        and    byte ptr [eax], 0xfb
loc_1_378C:
        push   esi
        call   sub_1_0770
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_37A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_05D0
        push   esi
        call   sub_1_07D0
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_37C0:
        mov    al, byte ptr [0x3e58]         ; -> obj1:0x3E58
        cmp    al, 0
        jne    loc_1_37E5
        inc    byte ptr [0x3e58]             ; -> obj1:0x3E58
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_37E7
        push   0
        push   0
        call   sub_2_0CD0
        cmp    eax, 0
        jne    loc_1_37E7
loc_1_37E5:
        clc    
        ret    
loc_1_37E7:
        stc    
        ret    
CPQNDIS3_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_37F8
        jmp    dword ptr [eax*4 + 0x37fa]    ; -> obj1:0x37FA
loc_1_37F8:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_37C0
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37F8
        dd  loc_1_37C0
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3870:
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
        call   sub_1_38A0
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_38A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_3940
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_38E0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_38D5
        cmp    al, 0xff
        jne    loc_1_38D5
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_38D5
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_3947
loc_1_38D5:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_3947
        db  08Bh, 0FFh
loc_1_38E0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_3930
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_3906
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_390D
loc_1_3906:
        mov    dword ptr [ebp - 0x10], 1
loc_1_390D:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_3920
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_394A
        db  08Dh, 064h, 024h, 000h
loc_1_3920:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_394A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3930:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_394A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3940:
        mov    dword ptr [ebp - 4], 0x20
loc_1_3947:
        mov    esi, dword ptr [ebp + 8]
loc_1_394A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_3A41
loc_1_3955:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    loc_1_39AD
        test   al, 2
        je     loc_1_3999
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_1D00
        or     al, al
        je     loc_1_3999
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0xc]
        test   dword ptr [ecx + eax*4], edx
        jne    loc_1_39AD
loc_1_3999:
        test   byte ptr [ebp - 4], 2
        je     loc_1_39A4
        test   bl, 4
        jne    loc_1_39AD
loc_1_39A4:
        test   bl, 0x20
        je     loc_1_3A36
loc_1_39AD:
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
        call   sub_1_3C22
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_3A36
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_39EF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_39EF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_3A00
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_3A06
        db  08Bh, 0FFh
loc_1_3A00:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_3A06:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_3A15
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3C28
loc_1_3A15:
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
        call   sub_1_3CCC
        mov    edi, ebx
        jmp    loc_1_3A39
loc_1_3A36:
        mov    edi, dword ptr [edi + 0x10]
loc_1_3A39:
        or     edi, edi
        jne    loc_1_3955
loc_1_3A41:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3A50:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A60
        pop    ebp
        ret    4
sub_1_3A60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_3ADE
loc_1_3A70:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_3AD7
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3C28
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_3AD7
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_3A9C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_3A9C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_3AB0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_3AB6
        db  005h, 000h, 000h, 000h, 000h
loc_1_3AB0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_3AB6:
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
        call   sub_1_3CCC
        mov    edi, ebx
        jmp    loc_1_3ADA
loc_1_3AD7:
        mov    edi, dword ptr [edi + 0x10]
loc_1_3ADA:
        or     edi, edi
        jne    loc_1_3A70
loc_1_3ADE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3AF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_3B05
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B05:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_3B41
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_3B32
        cmp    bl, 0xff
        jne    loc_1_3B32
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_3B32
        test   al, 8
        je     loc_1_3B2B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B2B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B32:
        test   al, 4
        jne    loc_1_3B3A
        test   al, 2
        je     loc_1_3B59
loc_1_3B3A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B41:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_3B59
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_3B59
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B59:
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
sub_1_3BDA:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_3BE0:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_3BE6:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_3BEC:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_3BF2:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_3BF8:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_3C04:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_3C0A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_3C16:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_3C1C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_3C22:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_3C28:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_3C2E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_3C34:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_3C4C:
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
sub_1_3CA2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_3CA8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_3CAE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_3CB4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_3CC0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_3CC6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_3CCC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_3CF0:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_3D14:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_3D38:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_3D44:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_3D4A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_3D50:
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
sub_1_3D98:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_3DAC:
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
sub_1_3DCC:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_3DEC
        or     ecx, ecx
        je     loc_1_3E02
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_3E00
loc_1_3DEC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_3E00
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_3E00:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_3E02:
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
        db  'CPQNDIS3'                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  CPQNDIS3_Control         ; DDB_Control_Proc
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
loc_1_3EC0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_3EC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3EC8:
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
loc_1_40C8:
        inc    esp
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        db  000h
loc_1_40D0:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_40D4:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_40D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_40DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_40E0:
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
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [ebp + 0x50], al
        push   eax
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al            ; -> loc_1_40E0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0B20
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0D70
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0B80
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0DD0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0E40
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0E70
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_2_0C10
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0AA0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0E30
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x4104
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0C40
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0C80
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0CC0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0D00
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0D30
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0D50
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0BF0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0C00
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0C30
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_4174:
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
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4200:
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
loc_1_427C:
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
        inc    ebx
        push   eax
        push   ecx
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_42E8:
        inc    ebx
        push   eax
        push   ecx
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x6f], dl
        jb     loc_1_436C
        pop    edi
        push   esp
        jns    loc_1_436C
        add    byte ptr gs:[eax], al
        add    byte ptr [eax + 0x6f], dl
        jb     loc_1_4378
        pop    edi
        push   esp
        jns    loc_1_4378
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 0x61], al
        bound  ebp, qword ptr [ebp + 0x5f]
        push   esp
        jns    loc_1_4385
        add    byte ptr gs:[eax], al
loc_1_4318:
        inc    ebx
        popal  
        bound  ebp, qword ptr [ebp + 0x5f]
        push   esp
        jns    0x4391
        add    byte ptr gs:[eax], al
        inc    esp
        imul   esi, dword ptr [ebx + 0x61], 0x5f656c62
        dec    esp
        imul   ebp, dword ptr [esi + 0x6b], 0
loc_1_4334:
        inc    esp
        imul   esi, dword ptr [ebx + 0x61], 0x5f656c62
        dec    esp
        imul   ebp, dword ptr [esi + 0x6b], 0
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
loc_1_4354:
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
loc_1_436C:
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
loc_1_4378:
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_437C:
        inc    ebx
        push   eax
        push   ecx
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    esp, dword ptr [eax]
loc_1_4385:
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 0x64], al
        popal  
        jo     0x4401
        jb     loc_1_43BE
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_43A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_43A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_43A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_43AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_43BE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_442C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4430:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4434:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4438:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_443C:
        add    byte ptr [eax], al
loc_1_443E:
        add    byte ptr [eax], al
sub_1_4440:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4444:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4448:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_444C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_4450:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1dc0  R-X
        db  06Ah, 000h, 06Ah, 000h, 0E8h, 057h, 00Ch, 000h, 000h, 083h, 0F8h, 001h, 01Ah, 0C9h, 0F6h, 0D9h
        db  08Ah, 0C1h, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
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
        call   sub_1_03D0
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
        call   sub_1_04B0
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
        call   sub_1_0660
loc_2_00F1:
        push   0x50
        push   0xe
        call   sub_1_4438
        push   0xa0
        push   4
        call   sub_1_4438
        push   0
        push   0xd
        call   sub_1_4438
        push   0x20
        push   0xc
        call   sub_1_4438
        push   0xc
        push   1
        call   sub_1_4438
        push   0x80
        push   2
        call   sub_1_4438
        push   0xc
        push   3
        call   sub_1_4438
        push   0x3c
        mov    bl, 0x14
        push   5
        call   sub_1_4438
        push   0
        push   6
        call   sub_1_4438
loc_2_0153:
        push   0x64
        call   sub_1_3CC0
loc_2_015A:
        dec    bl
        jne    loc_2_0153
        push   0xff
        push   7
        call   sub_1_4438
        push   0x41
        push   0
        call   sub_1_4438
        push   0xc
        mov    bl, 0x14
        push   7
        call   sub_1_4438
        push   1
        push   0
        call   sub_1_4438
loc_2_018B:
        push   0x64
        call   sub_1_3CC0
loc_2_0192:
        dec    bl
        jne    loc_2_018B
        lea    eax, [ebp - 1]
        push   eax
        push   0
        call   sub_1_4440
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
        call   sub_1_4438
        push   0xa
        push   0
        call   sub_1_4438
        mov    byte ptr [esi + 0x11c], 1
        push   0
        push   0xf
        call   sub_1_4438
        push   0
        push   0
        call   sub_1_4438
        push   0x48
        push   0xe
        call   sub_1_4438
        push   0
        push   0xd
        call   sub_1_4438
        push   0
        push   0xc
        call   sub_1_4438
        push   0xff
        push   7
        call   sub_1_4438
        push   0x21
        push   0
        call   sub_1_4438
        push   0
        mov    bl, 0xff
        push   0xa
        call   sub_1_4438
        push   0
        push   0xb
        call   sub_1_4438
loc_2_022C:
        dec    bl
        je     loc_2_0242
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4440
        test   byte ptr [ebp - 1], 0x80
        je     loc_2_022C
loc_2_0242:
        push   2
        push   0xd
        call   sub_1_4438
        push   0x22
        push   0
        call   sub_1_4438
        push   esi
        call   sub_2_0020
        push   esi
        test   al, al
        jne    loc_2_0270
        call   sub_1_0660
loc_2_0266:
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0270:
        call   sub_1_0660
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
        mov    dl, byte ptr [eax + 0x27]     ; -> obj1:0x43D3
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
        push   2
        xor    bl, bl
        call   sub_1_0920
loc_2_0330:
        push   0
        call   sub_2_0B80
        call   sub_1_4434
loc_2_033D:
        call   sub_2_0AE0
        call   sub_1_0A20
loc_2_0347:
        call   sub_1_0A70
loc_2_034C:
        call   sub_1_0A20
loc_2_0351:
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        push   eax
        call   sub_1_4448
        push   0x21
        push   0
        call   sub_1_4438
        lea    ecx, [ebp - 1]
        push   ecx
        push   0
        call   sub_1_4440
        cmp    byte ptr [ebp - 1], 0x21
        je     loc_2_038D
        cmp    byte ptr [ebp - 1], 0x23
        je     loc_2_038D
        inc    bl
        call   sub_2_0A70
        cmp    bl, 3
        jb     loc_2_033D
        jmp    loc_2_0401
loc_2_038D:
        push   0x21
        push   0
        call   sub_1_4438
        push   0x48
        push   0xe
        call   sub_1_4438
        push   0
        push   0xa
        call   sub_1_4438
        push   0
        push   0xb
        call   sub_1_4438
        push   0xc
        push   0xc
        call   sub_1_4438
        push   2
        push   0xd
        call   sub_1_4438
        push   0xc
        push   3
        call   sub_1_4438
        push   0xc
        push   1
        call   sub_1_4438
        push   0x80
        push   2
        call   sub_1_4438
        push   0xff
        push   7
        call   sub_1_4438
        push   0
        push   0xf
        call   sub_1_4438
loc_2_0401:
        cmp    bl, 3
        jae    loc_2_045E
        call   sub_2_0900
        call   sub_2_0470
        push   1
        call   sub_2_0540
        test   al, al
        je     loc_2_0427
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0427:
        cmp    byte ptr [0], 0               ; -> loc_1_3EC0
        je     loc_2_0455
        push   1
        call   sub_2_0B80
        call   sub_1_4434
        call   sub_1_0A20
loc_2_0442:
        push   3
        call   sub_1_0920
loc_2_0449:
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        push   eax
        call   sub_1_4448
loc_2_0455:
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_045E:
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0470:
        push   ebx
        mov    al, byte ptr [0]              ; -> obj1:0x4454
        push   esi
        test   al, al
        jne    loc_2_0525
        xor    esi, esi
loc_2_0481:
        push   3
        call   sub_2_0540
        mov    bl, al
        call   sub_1_0A70
loc_2_048F:
        push   0x21
        push   0
        call   sub_1_4438
        push   0x48
        push   0xe
        call   sub_1_4438
        push   0
        push   0xa
        call   sub_1_4438
        push   0
        push   0xb
        call   sub_1_4438
        push   0xc
        push   0xc
        call   sub_1_4438
        push   2
        push   0xd
        call   sub_1_4438
        push   0xc
        push   3
        call   sub_1_4438
        push   0xc
        push   1
        call   sub_1_4438
        push   0x80
        push   2
        call   sub_1_4438
        push   0xff
        push   7
        call   sub_1_4438
        push   0
        push   0xf
        call   sub_1_4438
        test   bl, bl
        je     loc_2_0511
        inc    esi
        cmp    esi, 3
        jb     loc_2_0481
loc_2_0511:
        cmp    esi, 3
        jne    loc_2_0535
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        pop    esi
        and    al, 0xfb
        pop    ebx
        mov    byte ptr [0], al              ; -> loc_1_40C8
        ret    
loc_2_0525:
        cmp    al, 2
        jne    loc_2_0535
        mov    al, byte ptr [0]              ; -> loc_1_40C8
        and    al, 0xfb
        mov    byte ptr [0], al              ; -> loc_1_40C8
loc_2_0535:
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0540:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x5c
        push   ebx
        push   esi
        pushf  
        cli    
        xor    ax, ax
loc_2_054E:
        mov    ecx, eax
        inc    ax
        and    ecx, 0xffff
        cmp    ax, 6
        mov    dl, byte ptr [ecx + 0x27]     ; -> obj1:0x43D3
        mov    byte ptr [ebp + ecx - 0x56], dl
        mov    byte ptr [ebp + ecx - 0x5c], dl
        jb     loc_2_054E
        xor    cx, cx
        lea    eax, [ebp - 0x4e]
        mov    word ptr [ebp - 0x50], 0x40
loc_2_0578:
        mov    byte ptr [eax], cl
        inc    eax
        inc    cx
        cmp    cx, 0x40
        jb     loc_2_0578
        lea    eax, [ebp - 0xc]
        push   0x4e
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        lea    eax, [ebp - 0x5c]
        push   eax
        push   0
        call   sub_1_03D0
loc_2_05A0:
        test   al, al
        jne    loc_2_05B0
        popf   
        mov    al, 0xff
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_05B0:
        push   0x40
        push   0xe
        call   sub_1_4438
        push   0x1f
        push   0xc
        call   sub_1_4438
        mov    al, byte ptr [ebp + 8]
        and    al, 3
        add    al, al
        or     al, 1
        push   eax
        push   0xd
        call   sub_1_4438
        push   0x4e
        push   5
        call   sub_1_4438
        push   0
        push   6
        call   sub_1_4438
        xor    si, si
        push   0
        push   4
        mov    ebx, 2
        call   sub_1_4438
        push   0x26
        push   0
        call   sub_1_4438
loc_2_0606:
        lea    eax, [ebp - 1]
        push   eax
        push   7
        call   sub_1_4440
        test   byte ptr [ebp - 1], bl
        jne    loc_2_0620
        inc    si
        cmp    si, 0xffff
        jb     loc_2_0606
loc_2_0620:
        cmp    si, 0xffff
        jne    loc_2_0633
        popf   
        mov    al, 0xff
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0633:
        xor    si, si
loc_2_0636:
        mov    eax, esi
        inc    si
        and    eax, 0xffff
        lea    ecx, [ebp + eax - 0xc]
        push   ecx
        push   6
        call   sub_1_4440
        cmp    si, 8
        jb     loc_2_0636
        push   0x21
        push   0
        call   sub_1_4438
        cmp    word ptr [ebp - 0xc], 0x3b00
        jne    loc_2_0679
        cmp    dword ptr [ebp - 0xa], 0x3f3e3d3c
        jne    loc_2_0679
        popf   
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0679:
        popf   
        mov    al, 0xff
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0690:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    al, byte ptr [esp + 8]
        mov    ax, word ptr [eax*2 + 0x408]
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_06B0:
        push   ebp
        mov    al, byte ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_2_0690
        mov    word ptr [0], ax              ; -> loc_1_43A4
        test   ax, ax
        je     loc_2_06DF
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        in     al, dx
        mov    byte ptr [0], al              ; -> loc_1_443C
        add    dx, 2
        in     al, dx
        mov    byte ptr [0], al              ; -> obj1:0x443D
loc_2_06DF:
        mov    ax, word ptr [0]              ; -> loc_1_43A4
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_06F0:
        cmp    word ptr [0], 0               ; -> loc_1_43A4
        je     loc_2_071A
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        mov    al, byte ptr [0]              ; -> loc_1_443C
        out    dx, al
        mov    al, byte ptr [0]              ; -> obj1:0x443D
        add    dx, 2
        out    dx, al
        mov    word ptr [0], 0               ; -> loc_1_43A4
loc_2_071A:
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_2_0720:
        push   ebx
        push   bx
        xor    ebx, ebx
        pop    bx
        xor    bl, al
        sub    bh, bh
        shl    bx, 1
        mov    al, ah
        sub    ah, ah
        xor    ax, word ptr [ebx]            ; -> loc_1_3EC8
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
sub_2_0740:
        push   ax
        push   ecx
        mov    ecx, 0x30
loc_2_0748:
        loop   loc_2_0748
        pop    ecx
        pop    ax
        ret    
        db  08Bh, 0FFh, 066h, 050h, 051h, 0B9h, 000h, 006h, 000h, 000h, 0E4h, 061h, 0E2h, 0FCh, 059h, 066h
        db  058h, 0C3h
sub_2_0760:
        push   ax
        call   sub_2_0740
        mov    al, 3
        out    dx, al
        call   sub_2_0740
        mov    al, 2
        out    dx, al
        call   sub_2_0740
        mov    al, 0
        out    dx, al
        pop    ax
        ret    
        db  08Dh, 049h, 000h
sub_2_0780:
        push   ax
        call   sub_2_0740
        mov    al, 0
        out    dx, al
        call   sub_2_0740
        mov    al, 2
        out    dx, al
        call   sub_2_0740
        mov    al, 3
        out    dx, al
        pop    ax
        ret    
        db  08Dh, 049h, 000h
sub_2_07A0:
        push   ax
        call   sub_2_0740
        mov    al, 1
        out    dx, al
        call   sub_2_0740
        mov    al, 3
        out    dx, al
        call   sub_2_0740
        mov    al, 1
        out    dx, al
        pop    ax
        ret    
        db  08Dh, 049h, 000h
sub_2_07C0:
        push   ax
        call   sub_2_0740
        mov    al, 0
        out    dx, al
        call   sub_2_0740
        mov    al, 2
        out    dx, al
        call   sub_2_0740
        mov    al, 0
        out    dx, al
        pop    ax
        ret    
        db  08Dh, 049h, 000h
sub_2_07E0:
        call   sub_2_0740
        mov    al, 1
        out    dx, al
        call   sub_2_0740
        mov    al, 3
        out    dx, al
        call   sub_2_0740
        inc    dx
        in     al, dx
        dec    dx
        mov    ah, al
        call   sub_2_0740
        mov    al, 1
        out    dx, al
        call   sub_2_0740
        and    ah, 8
        ror    ah, 3
        xor    ah, 1
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0820:
        push   ax
        xor    al, al
        or     al, 0xa0
        call   sub_2_0760
        push   ecx
        mov    ecx, 8
loc_2_0831:
        shl    al, 1
        jae    loc_2_083D
        call   sub_2_07A0
        jmp    loc_2_0842
loc_2_083D:
        call   sub_2_07C0
loc_2_0842:
        loop   loc_2_0831
        pop    ecx
        call   sub_2_07E0
        pop    ax
        push   ecx
        mov    ecx, 8
loc_2_0852:
        shl    al, 1
        jae    loc_2_085E
        call   sub_2_07A0
        jmp    loc_2_0863
loc_2_085E:
        call   sub_2_07C0
loc_2_0863:
        loop   loc_2_0852
        pop    ecx
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0870:
        xor    al, al
        add    al, 1
        or     al, 0xa0
        call   sub_2_0760
        push   ecx
        mov    ecx, 8
loc_2_0881:
        shl    al, 1
        jae    loc_2_088D
        call   sub_2_07A0
        jmp    loc_2_0892
loc_2_088D:
        call   sub_2_07C0
loc_2_0892:
        loop   loc_2_0881
        pop    ecx
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_08A0:
        push   ax
        push   ecx
        xor    bl, bl
        mov    ecx, 8
loc_2_08AA:
        shl    bl, 1
        call   sub_2_07E0
        or     bl, ah
        loop   loc_2_08AA
        pop    ecx
        pop    ax
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_08C0:
        mov    al, 0x11
        out    dx, al
        add    dx, 2
        mov    al, 0x1d
        out    dx, al
        mov    al, 0x15
        out    dx, al
        call   sub_2_0740
        sub    dx, 2
        mov    al, 3
        out    dx, al
        call   sub_2_0740
        add    dx, 2
        mov    al, 0x17
        out    dx, al
        sub    dx, 2
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_08F0:
        add    dx, 2
        mov    al, 0x15
        out    dx, al
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0900:
        push   ebx
        push   edi
        push   ax
        push   bx
        push   ecx
        push   dx
        push   edi
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        call   sub_2_08C0
        mov    ax, 0x100
        call   sub_2_0820
        call   sub_2_07E0
        call   sub_2_0870
        call   sub_2_07E0
        mov    edi, 0                        ; -> loc_1_43AC
        mov    ecx, 0x80
        xor    ax, ax
loc_2_093B:
        call   sub_2_08A0
        call   sub_2_0720
        mov    byte ptr [edi], bl
        cmp    ecx, 1
        je     loc_2_0953
        call   sub_2_07C0
        jmp    loc_2_0958
loc_2_0953:
        call   sub_2_07A0
loc_2_0958:
        inc    edi
        loop   loc_2_093B
        mov    word ptr [0], ax              ; -> loc_1_3EC4
        call   sub_2_07E0
        call   sub_2_0780
        mov    byte ptr [0], 0               ; -> loc_1_4430
        cmp    word ptr [0], 1               ; -> loc_1_3EC4
        je     loc_2_0983
        mov    byte ptr [0], 1               ; -> loc_1_4430
loc_2_0983:
        call   sub_2_08F0
        pop    edi
        pop    dx
        pop    ecx
        pop    bx
        pop    ax
        pop    edi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 066h, 053h, 032h
        db  0E4h, 086h, 0D8h, 0E8h, 075h, 0FEh, 0FFh, 0FFh, 0E8h, 0A0h, 0FDh, 0FFh, 0FFh, 0E8h, 02Bh, 0FEh
        db  0FFh, 0FFh, 0F6h, 0C4h, 001h, 075h, 027h, 051h, 0B9h, 008h, 000h, 000h, 000h, 0C0h, 0E3h, 001h
        db  073h, 007h, 0E8h, 0D6h, 0FDh, 0FFh, 0FFh, 0EBh, 005h, 0E8h, 0EFh, 0FDh, 0FFh, 0FFh, 0E2h, 0EDh
        db  059h, 0E8h, 007h, 0FEh, 0FFh, 0FFh, 0E8h, 0A2h, 0FDh, 0FFh, 0FFh, 0F8h, 0EBh, 001h, 0F9h, 066h
        db  05Bh, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 053h, 056h, 066h
        db  050h, 066h, 053h, 051h, 066h, 052h, 056h, 066h, 08Bh, 015h
        dd  loc_1_43A4               ; fixup
        db  0BEh
        dd  loc_1_43AC               ; fixup
        db  0FCh, 0B9h, 07Eh, 000h, 000h, 000h, 066h, 033h, 0C0h, 066h, 08Bh, 0D8h, 0ACh, 066h, 093h, 0E8h
        db  006h, 0FDh, 0FFh, 0FFh, 0E2h, 0F3h, 08Bh, 035h
        dd  loc_1_43AC               ; fixup
        db  066h, 089h, 046h, 07Eh, 0B9h, 080h, 000h, 000h, 000h, 032h, 0DBh, 0E8h, 08Eh, 0FEh, 0FFh, 0FFh
        db  0E8h, 019h, 0FDh, 0FFh, 0FFh, 0E8h, 014h, 0FDh, 0FFh, 0FFh, 0ACh, 0E8h, 05Eh, 0FFh, 0FFh, 0FFh
        db  072h, 00Dh, 0FEh, 0C3h, 0E2h, 0EAh, 0C6h, 005h
        dd  loc_1_4430               ; fixup
        db  000h, 0EBh, 007h, 0C6h, 005h
        dd  loc_1_4430               ; fixup
        db  001h, 0E8h, 093h, 0FEh, 0FFh, 0FFh, 05Eh, 066h, 05Ah, 059h, 066h, 05Bh, 066h, 058h, 05Eh, 05Bh
        db  0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0A70:
        mov    cx, word ptr [0]              ; -> loc_1_43A4
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
        call   sub_1_3CC0
loc_2_0AAE:
        dec    esi
        jne    loc_2_0AA7
        mov    byte ptr [ebp - 1], 0xec
        mov    ax, word ptr [0]              ; -> loc_1_43A4
        add    ax, 2
        mov    word ptr [ebp - 6], ax
        mov    dx, word ptr [ebp - 6]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    esi, 0x7d0
loc_2_0AD0:
        push   0x64
        call   sub_1_3CC0
loc_2_0AD7:
        dec    esi
        jne    loc_2_0AD0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
sub_2_0AE0:
        mov    cx, word ptr [0]              ; -> loc_1_43A4
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
        call   sub_1_0A70
loc_2_0B4C:
        mov    cx, word ptr [0]              ; -> loc_1_43A4
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
        xor    eax, eax
        mov    ebp, esp
        mov    al, byte ptr [esp + 8]
        lea    edx, [eax + eax*4]
        mov    eax, dword ptr [edx*8]        ; -> obj1:0x411C
        lea    ecx, [edx*8]
        mov    dword ptr [0], eax            ; -> loc_1_442C
        mov    eax, dword ptr [ecx + 4]      ; -> obj1:0x4120
        mov    dword ptr [0], eax            ; -> sub_1_4440
        mov    eax, dword ptr [ecx + 8]      ; -> obj1:0x4124
        mov    dword ptr [0], eax            ; -> sub_1_4438
        mov    eax, dword ptr [ecx + 0xc]    ; -> obj1:0x4128
        mov    dword ptr [0], eax            ; -> sub_1_4444
        mov    eax, dword ptr [ecx + 0x10]   ; -> obj1:0x412C
        mov    dword ptr [0], eax            ; -> sub_1_4450
        mov    eax, dword ptr [ecx + 0x14]   ; -> obj1:0x4130
        mov    dword ptr [0], eax            ; -> sub_1_4448
        mov    eax, dword ptr [ecx + 0x18]   ; -> obj1:0x4134
        mov    dword ptr [0], eax            ; -> sub_1_43A0
        mov    eax, dword ptr [ecx + 0x1c]   ; -> obj1:0x4138
        pop    ebp
        mov    dword ptr [0], eax            ; -> obj1:0x4398
        mov    eax, dword ptr [ecx + 0x20]   ; -> obj1:0x413C
        mov    dword ptr [0], eax            ; -> sub_1_4434
        mov    eax, dword ptr [ecx + 0x24]   ; -> obj1:0x4140
        mov    dword ptr [0], eax            ; -> loc_1_43A8
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0C10:
        push   dx
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        mov    al, 0x33
        out    dx, al
        add    dx, 2
        mov    al, 0x34
        out    dx, al
        sub    dx, 2
        nop    
        in     al, dx
        cmp    al, 0x33
        jne    loc_2_0C47
        mov    al, 0xcc
        out    dx, al
        add    dx, 2
        mov    al, 0x35
        out    dx, al
        sub    dx, 2
        nop    
        in     al, dx
        cmp    al, 0xcc
        jne    loc_2_0C47
        mov    ax, 0xffff
        jmp    loc_2_0C4A
loc_2_0C47:
        xor    ax, ax
loc_2_0C4A:
        push   ax
        mov    ax, 0x15
        add    dx, 2
        out    dx, al
        pop    ax
        pop    dx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [ebp + 8]
        cmp    bl, 1
        sbb    esi, esi
        xor    eax, eax
        and    esi, 2
        mov    al, bl
        inc    esi
        cmp    eax, esi
        jge    loc_2_0CB6
        mov    di, word ptr [ebp + 0xc]
loc_2_0C7E:
        push   ebx
        call   sub_2_06B0
        test   ax, ax
        je     loc_2_0CA7
        pushf  
        cli    
        call   sub_1_0F50
loc_2_0C91:
        popf   
        push   edi
        call   sub_2_0320
        test   eax, eax
        je     loc_2_0CC2
        pushf  
        cli    
        call   sub_1_0FF0
loc_2_0CA5:
        popf   
loc_2_0CA7:
        inc    bl
        call   sub_2_06F0
        xor    eax, eax
        mov    al, bl
        cmp    eax, esi
        jl     loc_2_0C7E
loc_2_0CB6:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_0CC2:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_2_0CD0:
        push   ebp
        mov    eax, 9
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
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_42E8
        push   esi
        push   eax
        call   sub_1_3BDA
loc_2_0D00:
        mov    dword ptr [0x46], esi         ; -> obj1:0x41FA
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [4], eax            ; -> obj1:0x41B8
        mov    byte ptr [0x4a], bl           ; -> obj1:0x41FE
        mov    dword ptr [0x3e], ebx         ; -> obj1:0x41F2
        mov    byte ptr [8], 3               ; -> obj1:0x41BC
        mov    byte ptr [9], bl              ; -> obj1:0x41BD
        mov    dword ptr [0xa], ebx          ; -> obj1:0x41BE
        mov    dword ptr [0xe], 0            ; -> obj1:0x41C2
        mov    dword ptr [0x12], 0           ; -> obj1:0x41C6
        mov    dword ptr [0x16], 0           ; -> obj1:0x41CA
        mov    dword ptr [0x1a], 0           ; -> obj1:0x41CE
        mov    dword ptr [0x1e], 0           ; -> obj1:0x41D2
        mov    edx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [0x22], 0           ; -> obj1:0x41D6
        mov    dword ptr [0x26], 0           ; -> obj1:0x41DA
        mov    dword ptr [0x2a], 0           ; -> obj1:0x41DE
        mov    dword ptr [0x2e], 0           ; -> obj1:0x41E2
        mov    dword ptr [0x32], 0           ; -> obj1:0x41E6
        push   0x36
        mov    dword ptr [0x36], edx         ; -> obj1:0x41EA
        push   8                             ; -> obj1:0x41BC
        lea    edx, [ebp - 8]
        push   0                             ; -> obj1:0x41B4
        mov    dword ptr [0x3a], ecx         ; -> obj1:0x41EE
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0                             ; -> obj1:0x41B4
        push   edx
        call   sub_1_3BE6
loc_2_0DC1:
        cmp    dword ptr [ebp - 8], ebx
        je     loc_2_0DDB
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3BE0
loc_2_0DD0:
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0DDB:
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0DF0:
        push   ebp
        mov    eax, 0xa
        mov    ebp, esp
        sub    esp, 0x38
        mov    word ptr [ebp - 0x38], ax
        push   esi
        mov    word ptr [ebp - 0x36], ax
        push   edi
        mov    dword ptr [ebp - 0x34], 0     ; -> obj1:0x4300
        mov    dword ptr [ebp - 0x2c], 0     ; -> loc_1_4318
        mov    dword ptr [ebp - 0x24], 0     ; -> loc_1_4334
        mov    eax, 0xb
        mov    word ptr [ebp - 0x30], ax
        mov    dword ptr [ebp - 0x1c], 0     ; -> loc_1_4354
        mov    word ptr [ebp - 0x2e], ax
        mov    esi, 0x10
        mov    eax, 0xd
        mov    ecx, dword ptr [0]            ; -> obj1:0x41B0
        mov    word ptr [ebp - 0x28], ax
        push   ecx
        mov    word ptr [ebp - 0x26], ax
        push   0
        mov    word ptr [ebp - 0x20], ax
        push   0x2d3
        mov    word ptr [ebp - 0x1e], ax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_3CC6
loc_2_0E5F:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_2_0FF1
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0xb4
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    edi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 0x10]
        push   edi
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_3D44
loc_2_0E8A:
        cmp    dword ptr [ebp - 4], 0
        push   0
        je     loc_2_0EAD
        push   0x2d3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3CCC
loc_2_0EA0:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0EAD:
        lea    eax, [ebp - 0x38]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        call   sub_1_3D4A
loc_2_0EC2:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0ED3
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [0], al              ; -> loc_1_3EC0
loc_2_0ED3:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_3D4A
loc_2_0EEA:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0EFB
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [0], al              ; -> obj1:0x4454
loc_2_0EFB:
        push   0
        lea    eax, [ebp - 0x28]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_3D4A
loc_2_0F12:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0F23
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [0], al              ; -> loc_1_40D0
loc_2_0F23:
        push   0
        lea    eax, [ebp - 0x20]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_3D4A
loc_2_0F3A:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0F46
        mov    eax, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [eax + 4]
loc_2_0F46:
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x14]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        call   sub_1_3C4C
loc_2_0F5B:
        cmp    dword ptr [ebp - 0x18], 6
        jne    loc_2_0F83
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0F83
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x14]
        add    edx, 0x104
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    cl, byte ptr [eax + 4]
        mov    byte ptr [edx + 4], cl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [edx + 5], al
loc_2_0F83:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_3D50
loc_2_0F8C:
        mov    eax, dword ptr [ebp - 8]
        push   0
        push   0
        push   edi
        mov    dword ptr [eax + 0x38], 0xc
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    byte ptr [eax + 0x40], 0xf
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        mov    dword ptr [eax + 0x41], esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1000
        test   eax, eax
        je     loc_2_0FE0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0x2d3
        push   eax
        call   sub_1_3CCC
loc_2_0FC9:
        pushf  
        cli    
        call   sub_1_0FF0
loc_2_0FD1:
        popf   
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0FE0:
        pushf  
        cli    
        call   sub_1_0F10
loc_2_0FE8:
        call   sub_1_0FF0
loc_2_0FED:
        popf   
        xor    eax, eax
loc_2_0FF1:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x38], 0xc
        jbe    loc_2_1025
        mov    dword ptr [ebp - 4], 0xc000009a
loc_2_1019:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1025:
        push   esi
        call   0
        test   al, al
        jne    loc_2_1055
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0xaa000004
        push   eax
        call   sub_1_3D98
loc_2_103F:
        mov    dword ptr [ebp - 4], 0xc0010006
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1055:
        xor    edx, edx
        lea    edi, [ebp - 0x2c]
        mov    dx, word ptr [0]              ; -> loc_1_43A4
        xor    eax, eax
        mov    ecx, 9
        mov    dword ptr [esi + 0x3c], edx
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    edi, 1
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    dword ptr [ebp - 0x22], edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x16], edi
        push   ecx
        mov    eax, dword ptr [0]            ; -> obj1:0x41B4
        lea    ecx, [esi + 4]
        mov    dword ptr [ebp - 0x12], edx
        mov    dword ptr [ebp - 0xe], 0x20
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        push   esi
        push   eax
        push   ecx
        call   sub_1_3BF2
loc_2_10A0:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_2_1019
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_2_10CC
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   edi
        push   0xaa00000a
        push   ecx
        call   sub_1_3D98
loc_2_10C4:
        add    esp, 0x10
        jmp    loc_2_12DB
loc_2_10CC:
        mov    eax, 0                        ; -> loc_1_35B0
        mov    ecx, dword ptr [esi + 0x3c]
        mov    dword ptr [esi + 0x185], eax
        mov    dword ptr [esi + 0x17d], eax
        push   esi
        mov    dword ptr [esi + 0x181], esi
        mov    dword ptr [esi + 0x179], 0
        mov    dword ptr [esi + 0xff], ecx
        call   sub_2_00E0
        test   al, al
        jne    loc_2_111F
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0xaa000004
        push   eax
        call   sub_1_3D98
loc_2_1110:
        mov    dword ptr [ebp - 4], 0xc0010006
        add    esp, 0xc
        jmp    loc_2_12DB
loc_2_111F:
        mov    edx, dword ptr [esi + 0x110]
        mov    edi, dword ptr [esi + 0x38]
        mov    cl, dh
        mov    eax, edi
        shl    eax, 8
        mov    dword ptr [esi + 0xed], edx
        add    eax, edx
        lea    ebx, [ecx + edi]
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
        je     loc_2_1190
        lea    eax, [esi + 0x5d]
loc_2_1181:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        cmp    dword ptr [esi + 0x38], ecx
        ja     loc_2_1181
loc_2_1190:
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
        push   0                             ; -> loc_1_35A0
        push   0                             ; -> loc_1_3520
        push   0                             ; -> loc_1_34A0
        push   ecx
        call   sub_2_1540
        test   al, al
        jne    loc_2_11ED
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   0xaa000002
        push   eax
        call   sub_1_3D98
loc_2_11DE:
        mov    dword ptr [ebp - 4], 0xc0000001
        add    esp, 0xc
        jmp    loc_2_12DB
loc_2_11ED:
        mov    byte ptr [esi + 0x15c], 0x1b
        mov    dword ptr [esi + 0x20], 0     ; -> obj1:0x41B4
        mov    eax, dword ptr [0x3e]         ; -> obj1:0x41F2
        push   esi
        mov    dword ptr [esi + 0x24], eax
        mov    dword ptr [0x3e], esi         ; -> obj1:0x41F2
        call   sub_1_0000
loc_2_120F:
        test   al, al
        jne    loc_2_122F
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0xaa000004
        push   eax
        call   sub_1_3D98
loc_2_1223:
        mov    dword ptr [ebp - 4], 0xc0010006
        add    esp, 0xc
        jmp    loc_2_129B
loc_2_122F:
        movsx  eax, byte ptr [esi + 0x40]
        push   1
        push   0
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   eax
        lea    edx, [esi + 8]
        push   0                             ; -> loc_1_10B0
        lea    eax, [ebp - 4]
        push   esi
        push   0                             ; -> loc_1_1080
        push   ecx
        push   edx
        push   eax
        call   sub_1_3CAE
loc_2_1255:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_1288
        push   1
        push   1
        push   7
        mov    eax, dword ptr [esi + 4]
        push   7
        lea    ecx, [esi + 0x2bb]
        push   0                             ; -> loc_1_10B0
        lea    edx, [ebp - 4]
        push   esi
        push   0                             ; -> loc_1_10A0
        push   eax
        push   ecx
        push   edx
        call   sub_1_3CAE
loc_2_1282:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_12F0
loc_2_1288:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0xaa000005
        push   eax
        call   sub_1_3D98
loc_2_1298:
        add    esp, 0xc
loc_2_129B:
        cmp    esi, dword ptr [0x3e]         ; -> obj1:0x41F2
        jne    loc_2_12AD
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x3e], eax         ; -> obj1:0x41F2
        jmp    loc_2_12C9
loc_2_12AD:
        mov    ecx, dword ptr [0x3e]         ; -> obj1:0x41F2
        cmp    dword ptr [ecx + 0x24], esi
        je     loc_2_12C0
loc_2_12B8:
        mov    ecx, dword ptr [ecx + 0x24]
        cmp    dword ptr [ecx + 0x24], esi
        jne    loc_2_12B8
loc_2_12C0:
        mov    eax, dword ptr [ecx + 0x24]
        mov    edx, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x24], edx
loc_2_12C9:
        push   1
        push   0
        call   sub_1_4438
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_2_1660
loc_2_12DB:
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3BF8
loc_2_12E4:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_12F0:
        xor    ebx, ebx
        mov    eax, 0                        ; -> loc_1_10B0
        mov    dword ptr [esi + 0x189], ebx
        mov    dword ptr [esi + 0x18d], eax
        mov    ecx, 0                        ; -> loc_1_2400
        mov    dword ptr [esi + 0x191], esi
        mov    dword ptr [esi + 0x195], ebx
        mov    dword ptr [esi + 0x199], eax
        lea    eax, [esi + 0x1a5]
        mov    dword ptr [esi + 0x19d], esi
        push   0x7d0
        mov    dword ptr [esi + 0x1a1], ecx
        push   eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 0x1a9], ecx
        mov    dword ptr [esi + 0x1ad], esi
        call   sub_1_3CA2
loc_2_1345:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 0x1b2], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1360:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_0EA0
loc_2_136E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0xffffffff
        lea    eax, [eax + edx*4]
loc_2_1389:
        test   edx, edx
        je     loc_2_1398
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_2_1389
        mov    dword ptr [ecx], edx
loc_2_1398:
        cmp    dword ptr [ecx], -1
        jne    loc_2_13B0
        call   sub_1_0ED0
loc_2_13A2:
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_13B0:
        mov    eax, dword ptr [0]            ; -> obj1:0x41B0
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x75
        push   ecx
        call   sub_1_3CC6
loc_2_13C3:
        test   eax, eax
        je     loc_2_13F0
        push   0
        mov    esi, dword ptr [ebp + 0x20]
        push   0xaa000002
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3D98
loc_2_13DA:
        add    esp, 0xc
        call   sub_1_0ED0
loc_2_13E2:
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_13F0:
        mov    esi, dword ptr [ebp + 0x20]
        inc    dword ptr [esi + 0x30]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_140C
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_2_140C
        push   0
        push   0xd
        call   sub_1_4438
loc_2_140C:
        mov    eax, dword ptr [esi + 0x28]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x28], ecx
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_2_14F5
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        add    eax, 0x14
        mov    ecx, dword ptr [esi + 0x14b]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_16B0
        test   al, al
        je     loc_2_14F5
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
        jne    loc_2_14C6
        cmp    byte ptr [esi + 0x13e], bl
        jne    loc_2_14B6
        cmp    dword ptr [esi + 0x16d], ebx
        jne    loc_2_14B6
        cmp    dword ptr [esi + 0x2c], ebx
        je     loc_2_14C6
loc_2_14B6:
        push   1
        add    esi, 0x179
        push   esi
        call   sub_1_3CA2
loc_2_14C4:
        jmp    loc_2_14E5
loc_2_14C6:
        cmp    dword ptr [esi + 0x47], 0
        je     loc_2_14E5
        mov    eax, dword ptr [esi + 0x4f]
        cmp    dword ptr [esi + eax*4 + 0x5d], 0
        jne    loc_2_14E5
        cmp    byte ptr [esi + 0x11d], 0
        jne    loc_2_14E5
        push   esi
        call   sub_1_2140
loc_2_14E5:
        call   sub_1_0ED0
loc_2_14EA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_2_14F5:
        push   0
        dec    dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp - 4]
        push   0x75
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x28], ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3CCC
loc_2_150E:
        push   0
        mov    ecx, dword ptr [esi + 4]
        push   0xaa000002
        push   ecx
        call   sub_1_3D98
loc_2_151E:
        add    esp, 0xc
        call   sub_1_0ED0
loc_2_1526:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1540:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x4394]       ; -> obj1:0x4394
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_3CC6
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_1574
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_1574:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_2_158E
        mov    esi, 2
loc_2_158E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_3CC6
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_2_15C2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1660
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_15C2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   sub_1_3CC6
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_2_15F1
        push   eax
        call   sub_2_1660
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_2_15F1:
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
sub_2_1660:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_2_1680
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   sub_1_3CCC
loc_2_1680:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_1696
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_3CCC
loc_2_1696:
        push   0
        push   0x32
        push   esi
        call   sub_1_3CCC
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_16B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_2_16C9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_16C9:
        mov    eax, dword ptr [0x4394]       ; -> obj1:0x4394
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_3CC6
        or     eax, eax
        je     loc_2_16E9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_16E9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_2_16F0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_1703
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_16F0
        jmp    loc_2_1712
loc_2_1703:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_2_1712:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_2_1728
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_1728:
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
sub_2_1780:
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
        call   sub_2_1B20
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_17A9
        cmp    ebx, 0x103
        jne    loc_2_17C0
loc_2_17A9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_1840
        or     eax, eax
        je     loc_2_17C0
        mov    ebx, eax
loc_2_17C0:
        or     ebx, ebx
        je     loc_2_17CC
        cmp    ebx, 0x103
        jne    loc_2_182B
loc_2_17CC:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_2_1826
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_17E2
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_17E2:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_17F1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_17F7
loc_2_17F1:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_2_17F7:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_2_1806
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3C28
loc_2_1806:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_3CCC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_2_1826:
        mov    ebx, 0xc001000e
loc_2_182B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1840:
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
        call   sub_1_3DCC
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_3DCC
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_2_18C4
        sub    eax, eax
        nop    
loc_2_18A0:
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
        ja     loc_2_18A0
loc_2_18C4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_2_192F
loc_2_18D1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_2_1924
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
        call   sub_1_3DCC
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
        call   sub_1_3DCC
        dec    dword ptr [esi + 0x24]
        jmp    loc_2_1927
loc_2_1924:
        inc    dword ptr [ebp - 4]
loc_2_1927:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_2_18D1
loc_2_192F:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_1A1A
        mov    edi, dword ptr [ebp + 0x18]
loc_2_1943:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_2_1D00
        or     al, al
        je     loc_2_197A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_2_1A08
loc_2_197A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_2_1A54
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
        call   sub_1_3DCC
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
        call   sub_1_3DCC
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
loc_2_1A08:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_2_1943
loc_2_1A1A:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0xc], ecx
        je     loc_2_1AA5
        mov    edi, dword ptr [ebp - 0x20]
        sub    edx, edx
        nop    
loc_2_1A30:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_1AAB
        mov    eax, dword ptr [esi + 4]
        add    eax, edx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_2_1A92
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    loc_2_1A92
        mov    dword ptr [ebp - 8], 0
        jmp    loc_2_1A99
loc_2_1A54:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3DCC
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_3DCC
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_2_1A92:
        mov    dword ptr [ebp - 8], 1
loc_2_1A99:
        add    edi, 6
        add    edx, 6
        inc    ecx
        cmp    dword ptr [ebp - 0xc], ecx
        ja     loc_2_1A30
loc_2_1AA5:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_1B10
loc_2_1AAB:
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
        je     loc_2_1B12
        cmp    edi, 0x103
        je     loc_2_1B12
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3DCC
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3DCC
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_2_1B10:
        sub    edi, edi
loc_2_1B12:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_2_1B20:
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
        je     loc_2_1B5D
        lea    esp, [esp]
loc_2_1B50:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_1B50
loc_2_1B5D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, ebx
        je     loc_2_1B91
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
        je     loc_2_1B93
        cmp    eax, 0x103
        je     loc_2_1B93
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_1B91:
        sub    eax, eax
loc_2_1B93:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
sub_2_1BA0:
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
        je     loc_2_1BD2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_1BC7:
        test   dword ptr [esi], edx
        je     loc_2_1BCC
        inc    eax
loc_2_1BCC:
        add    esi, 4
        dec    edi
        jne    loc_2_1BC7
loc_2_1BD2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1BE0:
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
        jbe    loc_2_1C5C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_2_1C20:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_2_1C4A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_2_1C76
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_3DCC
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_2_1C4A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_2_1C20
loc_2_1C5C:
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
loc_2_1C76:
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
sub_2_1CA0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_2_1CD0
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1CD0:
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
        call   sub_1_3DCC
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1D00:
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
        je     loc_2_1D99
        dec    esi
        cmp    esi, edx
        jb     loc_2_1D99
        nop    
loc_2_1D20:
        cmp    edx, ecx
        jb     loc_2_1D99
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_2_1D43
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1D77
loc_2_1D43:
        jbe    loc_2_1D50
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_2_1D77
        db  08Bh, 0FFh
loc_2_1D50:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_2_1D67
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_1D77
loc_2_1D67:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_2_1D77
        mov    dword ptr [ebp - 4], 0
loc_2_1D77:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_1DA9
        jle    loc_2_1D88
        or     edx, edx
        je     loc_2_1D99
        lea    esi, [edx - 1]
        jmp    loc_2_1D8B
loc_2_1D88:
        lea    ecx, [edx + 1]
loc_2_1D8B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_2_1D20
loc_2_1D99:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_1DA9:
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

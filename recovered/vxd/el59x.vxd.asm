; EL59X -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 14476 instructions recovered by recursive descent

Declare_Virtual_Device EL59X, 3, 10, EL59X_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xbab4  R-X
loc_1_0000:
        mov    al, byte ptr [0x9118]         ; -> obj1:0x9118
        cmp    al, 0
        jne    loc_1_0025
        inc    byte ptr [0x9118]             ; -> obj1:0x9118
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_0027
        push   0
        push   0
        call   sub_1_3C58
        cmp    eax, 0
        jne    loc_1_0027
loc_1_0025:
        clc    
        ret    
loc_1_0027:
        stc    
        ret    
EL59X_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_0038
        jmp    dword ptr [eax*4 + 0x3a]      ; -> obj1:0x003A
loc_1_0038:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_0000
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0038
        dd  loc_1_0000
        db  000h, 000h
sub_1_00A8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_911C
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ecx + 0x17a]
        in     al, dx
        test   al, 1
        je     loc_1_00F2
        mov    edx, dword ptr [ecx + 0x176]
        mov    eax, 0x7000
        out    dx, ax
        mov    eax, 0x6801
        pop    ebp
        out    dx, ax
        mov    al, 1
        ret    4
loc_1_00F2:
        xor    al, al
        pop    ebp
        ret    4
sub_1_00F8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0xc2], 1
        mov    edx, dword ptr [esi + 0x182]
        in     al, dx
        mov    cl, al
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        and    ecx, 0xffff00ff
        mov    eax, dword ptr [0]            ; -> loc_1_9124
        mov    word ptr [ebp - 2], cx
        mov    edi, ecx
        and    edi, 0xffff
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edi        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
loc_1_0159:
        mov    edx, dword ptr [esi + 0x17a]
        in     ax, dx
        and    eax, 0xfe
        je     loc_1_01C7
        mov    ecx, dword ptr [0]            ; -> loc_1_912C
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [edx*4], ecx        ; -> obj1:0x9AB4
        lea    ecx, [edx + 1]
        and    ecx, 0x7ff
        mov    edx, ebx
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    dword ptr [ecx*4], ebx        ; -> obj1:0x9AB4
        inc    ecx
        inc    ebx
        and    ecx, 0x7ff
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        inc    ecx
        and    ecx, 0x7ff
        cmp    edx, 0xa
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        jae    loc_1_01C7
        push   esi
        and    eax, 0xfffffffe
        call   dword ptr [esi + eax*2 + 0x3a6]
        jmp    loc_1_0159
loc_1_01C7:
        imul   edi, edi
        mov    cx, word ptr [ebp - 2]
        add    edi, dword ptr [esi + 0x20a]
        mov    ax, word ptr [esi + 0x206]
        add    cx, word ptr [esi + 0x208]
        mov    dword ptr [esi + 0x20a], edi
        inc    ax
        mov    word ptr [esi + 0x208], cx
        mov    word ptr [esi + 0x206], ax
        cmp    ax, 0x100
        jb     loc_1_021B
        mov    word ptr [esi + 0x206], 0x80
        shr    edi, 1
        shr    cx, 1
        mov    dword ptr [esi + 0x20a], edi
        mov    word ptr [esi + 0x208], cx
loc_1_021B:
        mov    byte ptr [esi + 0xc2], 0
        mov    eax, dword ptr [0]            ; -> loc_1_9134
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        cmp    byte ptr [esi + 0xb7], 0
        je     loc_1_0270
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x70fe
        mov    byte ptr [esi + 0xb7], 0
        out    dx, ax
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_8D10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0270:
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x70fe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0288:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0xb6], 0
        je     loc_1_02A7
        mov    byte ptr [esi + 0xb6], 0
        push   0
        push   esi
        call   sub_1_3668
loc_1_02A7:
        lea    ecx, [esi + 0x3a6]
        xor    eax, eax
loc_1_02AF:
        test   al, 2
        je     loc_1_02BB
        mov    dword ptr [ecx], 0            ; -> loc_1_04E8
        jmp    loc_1_0307
loc_1_02BB:
        test   al, 0x80
        je     loc_1_02C7
        mov    dword ptr [ecx], 0            ; -> loc_1_0328
        jmp    loc_1_0307
loc_1_02C7:
        test   al, 0x20
        je     loc_1_02D3
        mov    dword ptr [ecx], 0            ; -> loc_1_2088
        jmp    loc_1_0307
loc_1_02D3:
        test   al, 0x10
        je     loc_1_02DF
        mov    dword ptr [ecx], 0            ; -> loc_1_2C18
        jmp    loc_1_0307
loc_1_02DF:
        test   al, 4
        je     loc_1_02EB
        mov    dword ptr [ecx], 0            ; -> loc_1_18A8
        jmp    loc_1_0307
loc_1_02EB:
        test   al, 8
        je     loc_1_02F7
        mov    dword ptr [ecx], 0            ; -> loc_1_1A08
        jmp    loc_1_0307
loc_1_02F7:
        test   al, 0x40
        mov    dword ptr [ecx], 0            ; -> loc_1_0468
        jne    loc_1_0307
        mov    dword ptr [ecx], 0            ; -> loc_1_05D8
loc_1_0307:
        add    ecx, 4
        add    eax, 2
        cmp    eax, 0x100
        jl     loc_1_02AF
        mov    dword ptr [esi + 0x377], 0    ; -> loc_1_31F8
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0328:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_0348
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0348:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 0x806
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x176]
        mov    edi, dword ptr [esi + 0x172]
        mov    edx, ecx
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        lea    edx, [edi + 1]
        xor    ebx, ebx
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [esi + 0x31a], edx
        lea    edx, [edi + 2]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 3]
        in     al, dx
        xor    edx, edx
        add    dword ptr [esi + 0x322], ebx
        mov    dl, al
        xor    ebx, ebx
        add    dword ptr [esi + 0x31e], edx
        lea    edx, [edi + 5]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 0xc]
        in     ax, dx
        lea    edx, [edi + 0xa]
        add    dword ptr [esi + 0x34a], ebx
        mov    ebx, eax
        in     ax, dx
        and    ebx, 0xffff
        mov    edx, eax
        and    edx, 0xffff
        add    dword ptr [esi + 0x30e], ebx
        xor    ebx, ebx
        add    dword ptr [esi + 0x312], edx
        mov    edx, edi
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 4]
        in     al, dx
        xor    edx, edx
        add    dword ptr [esi + 0x32a], ebx
        mov    dl, al
        xor    ebx, ebx
        add    dword ptr [esi + 0x32e], edx
        lea    edx, [edi + 8]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 7]
        in     al, dx
        add    dword ptr [esi + 0x332], ebx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [esi + 0x306], edx
        lea    edx, [edi + 6]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 9]
        in     al, dx
        and    eax, 3
        shl    eax, 8
        add    dword ptr [esi + 0x306], eax
        xor    eax, eax
        mov    al, bl
        add    dword ptr [esi + 0x302], eax
        cmp    byte ptr [esi + 0xb8], 0
        je     loc_1_0431
        add    dword ptr [esi + 0x31a], eax
loc_1_0431:
        mov    edx, ecx
        mov    eax, 0x804
        out    dx, ax
        lea    edx, [edi + 0xc]
        in     al, dx
        and    eax, 0xff
        mov    edx, ecx
        pop    edi
        add    dword ptr [esi + 0x34e], eax
        mov    eax, 0xa800
        pop    esi
        out    dx, ax
        mov    eax, 0x807
        pop    ebx
        out    dx, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0468:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_913C
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x6840
        mov    byte ptr [esi + 0xbf], 1
        mov    edx, dword ptr [esi + 0x176]
        cmp    byte ptr [esi + 0xc0], 0
        out    dx, ax
        je     loc_1_04D4
        push   esi
        call   sub_1_3718
        mov    byte ptr [esi + 0xb6], 0
        mov    byte ptr [esi + 0xc0], 0
        push   esi
        call   sub_1_7A98
        mov    ax, word ptr [esi + 0xce]
        mov    edx, dword ptr [esi + 0x176]
        out    dx, ax
loc_1_04D4:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_04E8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_9144
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x804
        mov    edi, dword ptr [esi + 0x176]
        mov    edx, edi
        out    dx, ax
        mov    edx, dword ptr [esi + 0x172]
        add    edx, 4
        in     ax, dx
        mov    cx, ax
        mov    edx, edi
        mov    eax, 0x807
        out    dx, ax
        mov    eax, ecx
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        and    eax, 0xffff
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        test   ch, 4
        mov    dword ptr [0], eax            ; -> loc_1_9418
        je     loc_1_0595
        push   esi
        call   sub_1_1BC8
        cmp    dword ptr [esi + 0x10e], 0
        je     loc_1_05C5
        mov    eax, dword ptr [esi + 0x112]
        mov    edx, dword ptr [esi + 0x176]
        pop    edi
        mov    dword ptr [esi + 0x116], eax
        mov    byte ptr [esi + 0x126], 0
        add    eax, 3
        pop    esi
        shr    eax, 2
        pop    ebp
        sub    ax, 0x7000
        out    dx, ax
        ret    4
loc_1_0595:
        mov    edi, dword ptr [esi + 0x176]
        mov    eax, 0x2800
        mov    edx, edi
        mov    ecx, dword ptr [esi + 0x17a]
        out    dx, ax
        mov    edx, ecx
loc_1_05AC:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_05AC
        mov    edx, edi
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, ecx
loc_1_05BE:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_05BE
loc_1_05C5:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_05D8:
        mov    ecx, dword ptr [0]            ; -> loc_1_914C
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        ret    4
loc_1_05F8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [0]            ; -> loc_1_9154
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        lea    ecx, [ebp - 4]
        push   ecx
        mov    ebx, dword ptr [ebp + 0xc]
        push   0
        mov    dword ptr [0], eax            ; -> loc_1_9418
        push   0
        push   0
        push   ebx
        call   sub_1_8FB0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0x28], edi
        mov    word ptr [ebx + 0x2c], cx
        cmp    ecx, 0xe
        mov    byte ptr [ebx + 0x2e], 0
        jb     loc_1_0812
        cmp    dword ptr [esi + 0x156], ecx
        jb     loc_1_0812
        cmp    byte ptr [edi + 0x20], 0
        je     loc_1_066F
        mov    eax, 0xc0010002
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_066F:
        cmp    byte ptr [esi + 0x83], 0
        je     loc_1_0686
        mov    eax, 0xc001000d
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0686:
        inc    dword ptr [edi + 0x1c]
        test   byte ptr [esi + 0x21], 0xa
        mov    dword ptr [ebx + 0x24], 0
        je     loc_1_06D7
        cmp    dword ptr [esi + 0x15], 0
        jne    loc_1_06A1
        mov    dword ptr [esi + 0x15], ebx
        jmp    loc_1_06A7
loc_1_06A1:
        mov    eax, dword ptr [esi + 0x11]
        mov    dword ptr [eax + 0x24], ebx
loc_1_06A7:
        mov    dword ptr [esi + 0x11], ebx
        mov    eax, dword ptr [0]            ; -> loc_1_915C
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 0x7ff
        pop    ebp
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x103
        ret    8
loc_1_06D7:
        mov    dword ptr [esi + 0x15], ebx
        mov    dword ptr [esi + 0x11], ebx
        push   ebx
        or     byte ptr [esi + 0x21], 8
        mov    eax, dword ptr [esi + 0xce]
        push   esi
        and    eax, 0xfffffffb
        mov    dword ptr [esi + 0xce], eax
        mov    edx, dword ptr [esi + 0x176]
        out    dx, ax
        call   sub_1_08C8
        test   eax, eax
        je     loc_1_0747
        mov    eax, dword ptr [0]            ; -> loc_1_9164
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [esi + 0xce]
        or     eax, 4
        pop    edi
        mov    dword ptr [esi + 0xce], eax
        mov    edx, dword ptr [esi + 0x176]
        pop    esi
        out    dx, ax
        mov    eax, 0x103
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0747:
        mov    eax, dword ptr [esi + 0x15]
        mov    ebx, dword ptr [eax + 0x24]
        mov    dword ptr [esi + 0x15], ebx
        test   ebx, ebx
        je     loc_1_078C
loc_1_0754:
        mov    eax, dword ptr [0]            ; -> loc_1_916C
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   ebx
        push   esi
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        call   sub_1_08C8
        test   eax, eax
        jne    loc_1_07E1
        push   ebx
        push   esi
        call   sub_1_0828
        mov    ebx, dword ptr [esi + 0x15]
        test   ebx, ebx
        jne    loc_1_0754
loc_1_078C:
        and    byte ptr [esi + 0x21], 0xf7
        mov    eax, dword ptr [edi + 0x1c]
        dec    eax
        mov    dword ptr [edi + 0x1c], eax
        jne    loc_1_079F
        push   edi
        call   sub_1_79E8
loc_1_079F:
        mov    eax, dword ptr [esi + 0xce]
        or     eax, 4
        mov    dword ptr [esi + 0xce], eax
        mov    edx, dword ptr [esi + 0x176]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        out    dx, ax
        mov    edx, dword ptr [0]            ; -> loc_1_9174
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        pop    edi
        pop    esi
        mov    dword ptr [ecx*4], edx        ; -> obj1:0x9AB4
        pop    ebx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_07E1:
        mov    eax, dword ptr [edi + 0x1c]
        dec    eax
        mov    dword ptr [edi + 0x1c], eax
        jne    loc_1_07F0
        push   edi
        call   sub_1_79E8
loc_1_07F0:
        mov    eax, dword ptr [esi + 0xce]
        pop    edi
        or     eax, 4
        mov    dword ptr [esi + 0xce], eax
        mov    edx, dword ptr [esi + 0x176]
        pop    esi
        out    dx, ax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0812:
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0828:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_917C
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    al, byte ptr [edx + 0x2e]
        test   al, 2
        jne    loc_1_085A
        xor    edi, edi
        jmp    loc_1_086C
loc_1_085A:
        and    al, 8
        cmp    al, 1
        sbb    edi, edi
        and    edi, 0xfffefff5
        sub    edi, 0x3ffefff4
loc_1_086C:
        mov    ecx, dword ptr [ebp + 8]
        push   edi
        push   edx
        mov    eax, dword ptr [ecx + 0x15]
        mov    esi, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 0x15], esi
        mov    esi, dword ptr [edx + 0x28]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_8ECA
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_1_0895
        push   esi
        call   sub_1_79E8
loc_1_0895:
        mov    eax, dword ptr [0]            ; -> loc_1_9184
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        pop    edi
        pop    esi
        pop    ebp
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_08C8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x20
        mov    ecx, dword ptr [eax + 0x192]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [eax + 0x2ea]
        mov    eax, dword ptr [0]            ; -> obj1:0x918C
        mov    dword ptr [ebp - 0x20], ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    byte ptr [ebp - 1], bl
        xor    edx, edx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp + 0xc]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        inc    eax
        mov    dx, word ptr [ecx + 0x2c]
        push   ebx
        lea    ecx, [ebp - 0x18]
        mov    dword ptr [ebp - 0x1c], edx
        push   ecx
        mov    dword ptr [eax*4 - 4], edx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        push   ebx
        push   ebx
        mov    edx, dword ptr [ebp + 0xc]
        push   edx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        call   sub_1_8FB0
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [ebp - 0x18]
        push   edx
        push   eax
        call   sub_1_8FD4
        cmp    dword ptr [ebp - 0xc], 6
        jl     loc_1_0998
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0x82], bl
        jne    loc_1_096D
        mov    eax, dword ptr [ebp - 0x14]
        test   byte ptr [eax], 1
        jne    loc_1_096D
        mov    ecx, eax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x40]
        cmp    dword ptr [ecx + 2], edx
        jne    loc_1_097E
loc_1_096D:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        xor    ebx, ebx
        push   ecx
        call   sub_1_7BC8
        mov    bl, al
loc_1_097E:
        test   ebx, ebx
        je     loc_1_0998
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        inc    dword ptr [eax + 0x30a]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3358
loc_1_0998:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x186]
        in     ax, dx
        mov    ecx, eax
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        and    ecx, 0xffff
        inc    eax
        mov    dword ptr [ebp - 0x10], ecx
        mov    dword ptr [eax*4 - 4], ecx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        cmp    ecx, 8
        mov    dword ptr [0], eax            ; -> loc_1_9418
        jge    loc_1_0A5C
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x10e], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x112], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x116], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x126], 0
        mov    ebx, dword ptr [0]            ; -> loc_1_9194
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        lea    eax, [ecx + 1]
        mov    dword ptr [ecx*4], ebx        ; -> obj1:0x9AB4
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x9fff
        mov    edx, dword ptr [ecx + 0x176]
        mov    ecx, dword ptr [ebp - 0x1c]
        out    dx, ax
        add    ecx, 3
        pop    edi
        sar    ecx, 2
        pop    esi
        sub    cx, 0x7000
        pop    ebx
        mov    ax, cx
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    8
loc_1_0A5C:
        cmp    esi, dword ptr [ebp - 0x1c]
        jge    loc_1_0A83
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, 4
        cmp    eax, dword ptr [ebp - 0x10]
        jge    loc_1_0A83
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x29e
        cmp    dword ptr [eax], 0
        jne    loc_1_0A83
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [eax], 1
loc_1_0A83:
        mov    ebx, dword ptr [ebp - 0x1c]
        add    ebx, 0x100
        and    ebx, 0x600
        shr    ebx, 9
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_0ABC
        shl    ebx, 4
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [ebx + eax + 0x264]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x176]
        mov    ax, word ptr [ebx + eax + 0x25e]
        jmp    loc_1_0ADB
loc_1_0ABC:
        shl    ebx, 4
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [ebx + eax + 0x224]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x176]
        mov    ax, word ptr [ebx + eax + 0x21e]
loc_1_0ADB:
        cmp    byte ptr [ebp - 1], 0
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [ebp - 0x1c]
        out    dx, eax
        je     loc_1_0BDB
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        push   0
        push   0
        push   eax
        call   sub_1_1E68
loc_1_0AFF:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_0B5E
        mov    eax, dword ptr [ebp - 0x1c]
        mov    edx, dword ptr [0]            ; -> loc_1_91A4
        neg    eax
        and    eax, 3
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebp - 0x10], eax
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [ecx*4], edx        ; -> obj1:0x9AB4
        mov    edx, dword ptr [ebp - 0x10]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        test   edx, edx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        je     loc_1_0B58
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0x10]
        rep outsb dx, byte ptr [esi]
        mov    esi, eax
loc_1_0B58:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_0B6B
loc_1_0B5E:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x29e], 0
loc_1_0B6B:
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x17e]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        test   cl, 0x10
        mov    dword ptr [ebp - 0x10], ecx
        je     loc_1_0D15
        push   5
        call   sub_1_8F80
        mov    eax, dword ptr [0]            ; -> loc_1_91AC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        inc    dword ptr [ecx + 0x316]
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1BC8
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_0CF3
        add    ebx, dword ptr [ebp + 8]
        add    ebx, 0x25e
        jmp    loc_1_0CFC
loc_1_0BDB:
        cli    
        mov    edi, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 0x10], 4
        mov    eax, dword ptr [eax + 0xd6]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, ecx
        jl     loc_1_0BF5
loc_1_0BF3:
        mov    eax, ecx
loc_1_0BF5:
        mov    dword ptr [ebp - 8], eax
loc_1_0BF8:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        jl     loc_1_0C65
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp - 0xc]
        sub    edi, ecx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        sub    dword ptr [ebp - 8], ecx
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        lea    eax, [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x18]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_0AFF
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x14]
        push   eax
        mov    edx, dword ptr [ebp - 0x18]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    loc_1_0BF8
loc_1_0C65:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        push   esi
        mov    esi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 8]
        sub    edi, eax
        sub    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 0x14], eax
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x186]
        in     ax, dx
        and    eax, 0xffff
        sub    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        cli    
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        cmp    ecx, 8
        mov    dword ptr [0], eax            ; -> loc_1_9418
        jl     loc_1_0D3F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd6]
        cmp    eax, ecx
        jl     loc_1_0BF5
        jmp    loc_1_0BF3
loc_1_0CF3:
        add    ebx, dword ptr [ebp + 8]
        add    ebx, 0x21e
loc_1_0CFC:
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1AB8
        push   1
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0DF8
loc_1_0D15:
        mov    eax, dword ptr [0]            ; -> loc_1_91B4
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 0x7ff
        pop    ebp
        mov    dword ptr [0], eax            ; -> loc_1_9418
        xor    eax, eax
        ret    8
loc_1_0D3F:
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + 0x10e], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x112], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x116], edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x11a], edx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x11e], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x122], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x126], 1
        mov    edx, dword ptr [0]            ; -> loc_1_919C
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        lea    eax, [ecx + 1]
        mov    dword ptr [ecx*4], edx        ; -> obj1:0x9AB4
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edi        ; -> obj1:0x9AB4
        inc    eax
        add    edi, 3
        sar    edi, 2
        and    eax, 0x7ff
        sub    di, 0x7000
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ecx + 0x176]
        mov    eax, 0x9fff
        out    dx, ax
        mov    ax, di
        pop    edi
        out    dx, ax
        mov    eax, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DF8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x28
        mov    ecx, dword ptr [eax + 0x2ea]
        mov    edx, dword ptr [eax + 0x192]
        push   ebx
        mov    dword ptr [ebp - 0x24], ecx
        push   esi
        xor    ecx, ecx
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_91BC
        mov    dword ptr [ebp - 0x1c], edx
        mov    byte ptr [ebp - 1], cl
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        xor    ebx, ebx
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [ebp + 8]
        mov    bx, word ptr [edx + 0x2c]
        mov    edx, dword ptr [eax + 0x186]
        in     ax, dx
        mov    edx, eax
        mov    dword ptr [ebp - 0x20], ebx
        and    edx, 0xffff
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        mov    dword ptr [ebp - 0x14], edx
        and    eax, 0x7ff
        cmp    edx, 8
        mov    dword ptr [0], eax            ; -> loc_1_9418
        jge    loc_1_0F0A
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x10e], edx
        mov    edx, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x112], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x116], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x126], cl
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    ebx, dword ptr [0]            ; -> loc_1_91C4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [ecx*4], ebx        ; -> obj1:0x9AB4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x9fff
        mov    edx, dword ptr [ecx + 0x176]
        mov    ecx, dword ptr [ebp - 0x20]
        out    dx, ax
        add    ecx, 3
        pop    edi
        sar    ecx, 2
        pop    esi
        sub    cx, 0x7000
        pop    ebx
        mov    ax, cx
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    0xc
loc_1_0F0A:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   edx
        call   sub_1_8FB0
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 0x18]
        push   ecx
        mov    eax, dword ptr [ebp - 0xc]
        push   edx
        push   eax
        call   sub_1_8FD4
        mov    eax, dword ptr [ebp - 0x20]
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 0x24]
        jle    loc_1_0F5B
        mov    eax, dword ptr [ebp - 0x20]
        add    eax, 4
        cmp    eax, dword ptr [ebp - 0x14]
        jge    loc_1_0F5B
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x29e
        cmp    dword ptr [eax], 0
        jne    loc_1_0F5B
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [eax], 1
loc_1_0F5B:
        cmp    byte ptr [ebp + 0x10], 0
        jne    loc_1_0FC1
        mov    eax, dword ptr [ebp - 0x20]
        add    eax, 0x100
        and    eax, 0x600
        shr    eax, 9
        cmp    byte ptr [ebp - 1], 0
        mov    dword ptr [ebp - 0x28], eax
        je     loc_1_0F9D
        mov    esi, eax
        mov    eax, dword ptr [ebp + 8]
        shl    esi, 4
        inc    word ptr [eax + esi + 0x264]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x176]
        mov    ax, word ptr [eax + esi + 0x25e]
        jmp    loc_1_0FBF
loc_1_0F9D:
        mov    esi, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [ebp + 8]
        shl    esi, 4
        inc    word ptr [eax + esi + 0x224]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x176]
        mov    ax, word ptr [eax + esi + 0x21e]
loc_1_0FBF:
        out    dx, ax
loc_1_0FC1:
        cli    
        mov    edx, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [ebp - 0x20]
        out    dx, eax
        sub    dword ptr [ebp - 0x14], 4
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xd6]
        cmp    eax, ecx
        jl     loc_1_0FE1
        mov    eax, ecx
loc_1_0FE1:
        xor    ebx, ebx
        mov    dword ptr [ebp - 0x10], eax
loc_1_0FE6:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1050
        mov    eax, dword ptr [ebp - 0x24]
        cmp    dword ptr [ebp - 8], eax
        jle    loc_1_1050
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   0
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        push   ebx
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        push   edx
        call   sub_1_1C18
        mov    ebx, dword ptr [ebp - 0xc]
        sub    edi, dword ptr [ebp - 8]
        lea    eax, [ebp - 0xc]
        push   eax
        push   ebx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_115A
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    loc_1_0FE6
loc_1_1050:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0x10], eax
        jl     loc_1_10C0
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp - 8]
        sub    edi, ecx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        sub    dword ptr [ebp - 0x10], ecx
        push   esi
        mov    esi, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        lea    eax, [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_115A
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    loc_1_0FE6
loc_1_10C0:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        push   esi
        mov    esi, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 0x10]
        sub    edi, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 0x18], eax
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x186]
        in     ax, dx
        and    eax, 0xffff
        sub    eax, 4
        mov    dword ptr [ebp - 0x14], eax
        cli    
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        cmp    dword ptr [ebp - 0x14], 8
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [ebp + 8]
        jl     loc_1_128C
        mov    eax, dword ptr [eax + 0xd6]
        mov    ecx, dword ptr [ebp - 0x14]
        cmp    eax, ecx
        jl     loc_1_1152
        mov    eax, ecx
loc_1_1152:
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_0FE6
loc_1_115A:
        mov    eax, dword ptr [ebp - 0x20]
        mov    edx, dword ptr [0]            ; -> loc_1_91D4
        neg    eax
        and    eax, 3
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebp - 0x14], eax
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [ecx*4], edx        ; -> obj1:0x9AB4
        mov    edx, dword ptr [ebp - 0x14]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        test   edx, edx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        je     loc_1_11AD
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x14]
        rep outsb dx, byte ptr [esi]
        mov    esi, eax
loc_1_11AD:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_11CE
        mov    eax, dword ptr [ebp + 8]
        test   ebx, ebx
        mov    dword ptr [eax + 0x29e], 0
        je     loc_1_11CE
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_89E8
loc_1_11CE:
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x17e]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        test   cl, 0x10
        mov    dword ptr [ebp - 0x14], ecx
        je     loc_1_1262
        push   5
        call   sub_1_8F80
        mov    eax, dword ptr [0]            ; -> loc_1_91DC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        inc    dword ptr [ecx + 0x316]
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1BC8
        cmp    byte ptr [ebp + 0x10], 0
        jne    loc_1_1253
        cmp    byte ptr [ebp - 1], 0
        mov    eax, dword ptr [ebp - 0x28]
        je     loc_1_123E
        shl    eax, 4
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x25e
        jmp    loc_1_1249
loc_1_123E:
        shl    eax, 4
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x21e
loc_1_1249:
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1AB8
loc_1_1253:
        push   1
        mov    edx, dword ptr [ebp + 0xc]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0DF8
loc_1_1262:
        mov    eax, dword ptr [0]            ; -> loc_1_91E4
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 0x7ff
        pop    ebp
        mov    dword ptr [0], eax            ; -> loc_1_9418
        xor    eax, eax
        ret    0xc
loc_1_128C:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    dword ptr [eax + 0x10e], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x112], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x116], edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x11a], edx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x11e], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x122], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x126], 1
        mov    edx, dword ptr [0]            ; -> loc_1_91CC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        lea    eax, [ecx + 1]
        mov    dword ptr [ecx*4], edx        ; -> obj1:0x9AB4
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edi        ; -> obj1:0x9AB4
        inc    eax
        add    edi, 3
        sar    edi, 2
        and    eax, 0x7ff
        sub    di, 0x7000
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ecx + 0x176]
        mov    eax, 0x9fff
        out    dx, ax
        mov    ax, di
        out    dx, ax
        sti    
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1348:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x30
        mov    ecx, dword ptr [eax + 0x2ea]
        mov    edx, dword ptr [eax + 0x192]
        push   ebx
        mov    dword ptr [ebp - 0x28], ecx
        push   esi
        mov    ebx, dword ptr [0]            ; -> loc_1_91EC
        push   edi
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        lea    eax, [ecx + 1]
        mov    dword ptr [ebp - 0x24], edx
        and    eax, 0x7ff
        mov    dword ptr [ecx*4], ebx        ; -> obj1:0x9AB4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    byte ptr [ebp - 1], 0
        mov    esi, dword ptr [ecx + 0x10e]
        cli    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x186]
        in     ax, dx
        mov    ecx, eax
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        and    ecx, 0xffff
        inc    eax
        mov    dword ptr [ebp - 0x14], ecx
        mov    dword ptr [eax*4 - 4], ecx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x112
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ecx]
        cmp    byte ptr [eax + 0x126], 0
        jne    loc_1_1474
        cmp    dword ptr [ebp - 0x14], 8
        jge    loc_1_1447
        mov    eax, dword ptr [0]            ; -> loc_1_91F4
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        inc    eax
        mov    ebx, dword ptr [ecx]
        mov    dword ptr [eax*4 - 4], ebx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x9fff
        mov    ecx, dword ptr [ecx]
        mov    edx, dword ptr [ebx + 0x176]
        add    ecx, 3
        out    dx, ax
        shr    ecx, 2
        sub    cx, 0x7000
        mov    ax, cx
        out    dx, ax
        sti    
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1447:
        mov    edx, dword ptr [ebp - 0x24]
        mov    eax, ebx
        out    dx, eax
        push   0
        lea    eax, [ebp - 0xc]
        sub    dword ptr [ebp - 0x14], 4
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_8FB0
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    loc_1_1492
loc_1_1474:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x11a]
        mov    dword ptr [ebp - 0xc], ecx
        mov    edx, dword ptr [eax + 0x11e]
        mov    dword ptr [ebp - 0x18], edx
        mov    ecx, dword ptr [eax + 0x122]
        mov    dword ptr [ebp - 8], ecx
loc_1_1492:
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 0x116]
        mov    ecx, dword ptr [eax + 0xd6]
        mov    eax, dword ptr [ebp - 0x14]
        cmp    ecx, eax
        jl     loc_1_14AA
        mov    ecx, eax
loc_1_14AA:
        mov    dword ptr [ebp - 0x10], ecx
        cmp    dword ptr [ebp - 0x28], edi
        jge    loc_1_14EB
        lea    eax, [edi + 4]
        cmp    eax, dword ptr [ebp - 0x14]
        jge    loc_1_14EB
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x29e
        cmp    dword ptr [eax], 0
        jne    loc_1_14EB
        lea    ecx, [ebp - 0x30]
        mov    dword ptr [eax], 1
        lea    eax, [ebp - 0x2c]
        mov    edx, dword ptr [ebp - 0xc]
        mov    byte ptr [ebp - 1], 1
        push   eax
        push   ecx
        push   edx
        call   sub_1_8FD4
        mov    eax, dword ptr [ebp - 0x2c]
        sub    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x1c], eax
loc_1_14EB:
        add    ebx, 0x100
        and    ebx, 0x600
        shr    ebx, 9
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1521
        shl    ebx, 4
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + ebx + 0x264]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x176]
        mov    ax, word ptr [eax + ebx + 0x25e]
        jmp    loc_1_1540
loc_1_1521:
        shl    ebx, 4
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + ebx + 0x224]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x176]
        mov    ax, word ptr [eax + ebx + 0x21e]
loc_1_1540:
        mov    dword ptr [ebp - 0x20], 0
        out    dx, ax
loc_1_1549:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_15C2
        mov    eax, dword ptr [ebp - 0x28]
        cmp    eax, dword ptr [ebp - 8]
        jge    loc_1_15C2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   0
        mov    edx, dword ptr [ebp - 0x20]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp - 0x1c]
        push   ecx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        push   edx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1C18
        mov    ecx, dword ptr [ebp - 0xc]
        sub    edi, dword ptr [ebp - 8]
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x20], ecx
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_16D3
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_8FD4
        mov    dword ptr [ebp - 0x1c], 0
        jmp    loc_1_1549
loc_1_15C2:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0x10], eax
        jl     loc_1_1639
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        push   esi
        mov    esi, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [ebp - 0xc]
        sub    edi, eax
        push   ecx
        sub    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_16D3
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_8FD4
        mov    dword ptr [ebp - 0x1c], 0
        jmp    loc_1_1549
loc_1_1639:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        push   esi
        mov    esi, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    eax, dword ptr [ebp - 0x10]
        sub    edi, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 0x18], eax
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x186]
        in     ax, dx
        and    eax, 0xffff
        sub    eax, 4
        mov    dword ptr [ebp - 0x14], eax
        cli    
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        cmp    dword ptr [ebp - 0x14], 8
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [ebp + 8]
        jl     loc_1_1805
        mov    eax, dword ptr [eax + 0xd6]
        mov    ecx, dword ptr [ebp - 0x14]
        cmp    eax, ecx
        jl     loc_1_16CB
        mov    eax, ecx
loc_1_16CB:
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_1549
loc_1_16D3:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [0]            ; -> loc_1_9204
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    eax, dword ptr [eax + 0x112]
        neg    eax
        and    eax, 3
        mov    dword ptr [ecx*4], edx        ; -> obj1:0x9AB4
        mov    dword ptr [ebp - 0x14], eax
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    edx, dword ptr [ebp - 0x14]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        test   edx, edx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        je     loc_1_172C
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ebp - 0x14]
        rep outsb dx, byte ptr [esi]
        mov    esi, eax
loc_1_172C:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_1752
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 0x20], 0
        mov    dword ptr [eax + 0x29e], 0
        je     loc_1_1752
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_89E8
loc_1_1752:
        sti    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x17e]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        test   cl, 0x10
        mov    dword ptr [ebp - 0x14], ecx
        je     loc_1_17DB
        push   5
        call   sub_1_8F80
        mov    eax, dword ptr [0]            ; -> loc_1_920C
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        inc    dword ptr [ecx + 0x316]
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1BC8
        cmp    byte ptr [ebp - 1], 0
        mov    eax, dword ptr [ebp + 8]
        je     loc_1_17B8
        add    eax, ebx
        add    eax, 0x25e
        jmp    loc_1_17BF
loc_1_17B8:
        add    eax, ebx
        add    eax, 0x21e
loc_1_17BF:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1AB8
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x10e]
        push   ecx
        push   eax
        call   sub_1_0DF8
loc_1_17DB:
        mov    eax, dword ptr [0]            ; -> loc_1_9214
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 0x7ff
        pop    ebp
        mov    dword ptr [0], eax            ; -> loc_1_9418
        xor    eax, eax
        ret    4
loc_1_1805:
        mov    dword ptr [eax + 0x116], edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0x11a], edx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 0x18]
        mov    dword ptr [eax + 0x11e], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x122], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x126], 1
        mov    edx, dword ptr [0]            ; -> loc_1_91FC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        lea    eax, [ecx + 1]
        mov    dword ptr [ecx*4], edx        ; -> obj1:0x9AB4
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edi        ; -> obj1:0x9AB4
        inc    eax
        add    edi, 3
        sar    edi, 2
        and    eax, 0x7ff
        sub    di, 0x7000
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ecx + 0x176]
        mov    eax, 0x9fff
        out    dx, ax
        mov    ax, di
        out    dx, ax
        sti    
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_18A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [0]            ; -> obj1:0x921C
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x17e]
        in     al, dx
        out    dx, al
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        xor    ebx, ebx
        mov    bl, al
        mov    dword ptr [edx*4], ecx        ; -> obj1:0x9AB4
        lea    ecx, [edx + 1]
        and    ecx, 0x7ff
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    dword ptr [ecx*4], ebx        ; -> obj1:0x9AB4
        inc    ecx
        and    ecx, 0x7ff
        test   al, 0x10
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        je     loc_1_1938
        push   5
        call   sub_1_8F80
        push   esi
        inc    dword ptr [esi + 0x316]
        call   sub_1_1BC8
        mov    eax, dword ptr [esi + 0x10e]
        test   eax, eax
        jne    loc_1_1956
        lea    ebx, [esi + 0x25e]
        mov    edi, 4
loc_1_191F:
        push   ebx
        push   esi
        add    ebx, 0x10
        call   sub_1_1AB8
        lea    eax, [ebx - 0x50]
        push   eax
        push   esi
        call   sub_1_1AB8
        dec    edi
        jne    loc_1_191F
        jmp    loc_1_1976
loc_1_1938:
        test   al, 0x20
        je     loc_1_19CF
        push   esi
        inc    dword ptr [esi + 0x336]
        call   sub_1_1BC8
        mov    eax, dword ptr [esi + 0x10e]
        test   eax, eax
        je     loc_1_1976
loc_1_1956:
        push   1
        push   eax
        push   esi
        call   sub_1_0DF8
        mov    eax, dword ptr [esi + 0x10e]
        push   eax
        push   esi
        call   sub_1_0828
        mov    dword ptr [esi + 0x10e], 0
loc_1_1976:
        mov    edi, dword ptr [esi + 0x15]
        test   edi, edi
        jne    loc_1_1985
        and    byte ptr [esi + 0x21], 0xf7
        test   edi, edi
        je     loc_1_19A9
loc_1_1985:
        push   0
        push   edi
        push   esi
        call   sub_1_0DF8
        cmp    eax, 1
        je     loc_1_19A9
        push   edi
        push   esi
        call   sub_1_0828
        mov    edi, dword ptr [esi + 0x15]
        test   edi, edi
        jne    loc_1_1985
        and    byte ptr [esi + 0x21], 0xf7
        test   edi, edi
        jne    loc_1_1985
loc_1_19A9:
        mov    eax, dword ptr [0]            ; -> loc_1_9224
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        pop    ebp
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        ret    4
loc_1_19CF:
        test   al, 8
        je     loc_1_19ED
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x4800
        pop    edi
        inc    dword ptr [esi + 0x326]
        out    dx, ax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_19ED:
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x4800
        pop    edi
        inc    dword ptr [esi + 0x33a]
        out    dx, ax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_1A08:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x922C
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        push   edi
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x6808
        mov    edx, dword ptr [edi + 0x176]
        out    dx, ax
        call   sub_1_1348
        cmp    eax, 1
        je     loc_1_1AB0
        mov    eax, dword ptr [edi + 0x10e]
        push   eax
        push   edi
        call   sub_1_0828
        mov    dword ptr [edi + 0x10e], 0
        mov    esi, dword ptr [edi + 0x15]
        test   esi, esi
        jne    loc_1_1A6D
        and    byte ptr [edi + 0x21], 0xf7
        test   esi, esi
        je     loc_1_1A91
loc_1_1A6D:
        push   0
        push   esi
        push   edi
        call   sub_1_0DF8
        cmp    eax, 1
        je     loc_1_1A91
        push   esi
        push   edi
        call   sub_1_0828
        mov    esi, dword ptr [edi + 0x15]
        test   esi, esi
        jne    loc_1_1A6D
        and    byte ptr [edi + 0x21], 0xf7
        test   esi, esi
        jne    loc_1_1A6D
loc_1_1A91:
        mov    eax, dword ptr [0]            ; -> loc_1_9234
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
loc_1_1AB0:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1AB8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_923C
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    esi, dword ptr [ebp + 8]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    di, word ptr [ecx + 6]
        mov    word ptr [ecx + 6], 0
        mov    eax, dword ptr [esi + 0x21a]
        mov    edx, edi
        and    edx, 0xffff
        cmp    edx, eax
        jbe    loc_1_1B02
        mov    di, ax
loc_1_1B02:
        mov    dx, word ptr [ecx + 8]
        inc    dx
        mov    ax, word ptr [ecx + 0xa]
        add    ax, di
        mov    word ptr [ecx + 8], dx
        mov    word ptr [ecx + 0xa], ax
        cmp    dx, 4
        jb     loc_1_1B2D
        shr    dx, 1
        shr    ax, 1
        mov    word ptr [ecx + 8], dx
        mov    word ptr [ecx + 0xa], ax
loc_1_1B2D:
        xor    ebx, ebx
        xor    edi, edi
        mov    bx, word ptr [ecx + 4]
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [ecx + 2]
        mov    esi, dword ptr [esi + 0x216]
        mov    ax, word ptr [ecx + 0xa]
        sub    edi, ebx
        sub    dx, dx
        div    word ptr [ecx + 8]
        and    eax, 0xffff
        sub    esi, eax
        imul   esi, edi
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        cdq    
        idiv   dword ptr [esi + 0x216]
        mov    esi, eax
        test   esi, esi
        jl     loc_1_1B75
        cmp    esi, edi
        jle    loc_1_1B71
        mov    esi, edi
loc_1_1B71:
        add    edi, esi
        jmp    loc_1_1B87
loc_1_1B75:
        neg    esi
        mov    eax, edi
        cdq    
        sub    eax, edx
        sar    eax, 1
        cmp    esi, eax
        jle    loc_1_1B85
        mov    esi, eax
loc_1_1B85:
        sub    edi, esi
loc_1_1B87:
        cmp    edi, 8
        jge    loc_1_1B91
        mov    edi, 8
loc_1_1B91:
        add    edi, ebx
        cmp    edi, ebx
        jge    loc_1_1B99
        mov    edi, ebx
loc_1_1B99:
        cmp    edi, 0x5ea
        jle    loc_1_1BA6
        mov    edi, 0x5ea
loc_1_1BA6:
        mov    word ptr [ecx + 2], di
        add    edi, 2
        sar    edi, 2
        sub    di, 0x6800
        mov    word ptr [ecx], di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1BC8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_9244
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   esi
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp + 8]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x5840
        mov    esi, dword ptr [ecx + 0x176]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x17a]
loc_1_1C03:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_1C03
        mov    edx, esi
        mov    eax, 0x4800
        out    dx, ax
        pop    esi
        pop    ebp
        ret    4
sub_1_1C18:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x28
        mov    ecx, dword ptr [eax + 0x192]
        mov    edx, dword ptr [eax + 0x1fe]
        push   ebx
        mov    eax, dword ptr [ebp + 0x10]
        push   esi
        mov    dword ptr [ebp - 0x24], ecx
        push   edi
        mov    dword ptr [ebp - 0x20], edx
        mov    dword ptr [ebp - 0x14], 4
        test   eax, eax
        je     loc_1_1C4F
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_89E8
loc_1_1C4F:
        lea    eax, [ebp - 0x14]
        mov    esi, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a2
        push   eax
        push   1
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8998
        lea    eax, [ebp - 0x28]
        lea    ecx, [ebp - 0x1c]
        push   eax
        push   ecx
        push   esi
        call   sub_1_8FD4
        mov    dword ptr [ebp - 0x10], 0
        mov    edi, dword ptr [ebp + 0x14]
        add    dword ptr [ebp - 0x1c], edi
        cmp    dword ptr [ebp - 0x14], 0
        jbe    loc_1_1E54
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x2a6
        mov    dword ptr [ebp - 0x18], eax
loc_1_1C9B:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ebx, dword ptr [eax]
        cmp    edi, ebx
        jle    loc_1_1CEE
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        sub    edi, ebx
        mov    eax, dword ptr [0]            ; -> loc_1_924C
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edi        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], ebx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        jmp    loc_1_1E41
loc_1_1CEE:
        mov    eax, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [0]            ; -> loc_1_9254
        mov    ecx, dword ptr [eax - 4]
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        mov    dword ptr [ebp - 8], ecx
        and    eax, 0x7ff
        add    dword ptr [ebp - 8], edi
        sub    ebx, edi
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edi        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        xor    edi, edi
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, dword ptr [ebp - 0x14]
        cmp    eax, -1
        jne    loc_1_1D3B
        add    ebx, dword ptr [ebp + 0x18]
loc_1_1D3B:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0x14]
        neg    eax
        and    eax, 0xf
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [ecx*4], edx        ; -> obj1:0x9AB4
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], ebx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        mov    edx, dword ptr [0]            ; -> loc_1_925C
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        mov    edx, dword ptr [ebp - 0x10]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        cmp    dword ptr [ebp - 0xc], 0
        mov    dword ptr [0], eax            ; -> loc_1_9418
        je     loc_1_1DF4
        push   esi
        mov    esi, dword ptr [ebp - 0x1c]
        mov    edx, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        sub    ebx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 0xc]
        add    dword ptr [ebp - 8], eax
loc_1_1DF4:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0x172]
        mov    eax, dword ptr [ebp - 8]
        mov    edx, esi
        out    dx, eax
        mov    ax, bx
        lea    edx, [esi + 6]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 0x20]
        mov    edx, dword ptr [eax + 0x176]
        mov    eax, 0xa001
        out    dx, ax
        lea    edx, [esi + 0xe]
loc_1_1E21:
        mov    dword ptr [ebp - 4], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 4]
loc_1_1E28:
        xor    eax, eax
        dec    ecx
        jne    loc_1_1E28
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    loc_1_1E21
        lea    edx, [esi + 0xc]
        mov    eax, 0x1000
        out    dx, ax
        in     ax, dx
loc_1_1E41:
        add    dword ptr [ebp - 0x18], 8
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x14], eax
        ja     loc_1_1C9B
loc_1_1E54:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E68:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    ecx, dword ptr [ebp + 0x14]
        push   esi
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 0x2c]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, ebx
        sub    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [esi + 0x1fe]
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, ebx
        mov    dword ptr [edx*4], ebx        ; -> obj1:0x9AB4
        neg    eax
        and    eax, 3
        push   0
        mov    dword ptr [ebp - 0x14], eax
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        lea    edx, [ebp - 8]
        push   edx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        push   0
        push   0
        push   ecx
        call   sub_1_8FB0
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 0xc]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_8FD4
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    eax, dword ptr [esi + ecx*4 + 0x2da]
        mov    ecx, dword ptr [0]            ; -> loc_1_9264
        mov    dword ptr [edx*4], ecx        ; -> obj1:0x9AB4
        lea    ecx, [edx + 1]
        and    ecx, 0x7ff
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        add    eax, ecx
        mov    dword ptr [edx*4], ecx        ; -> obj1:0x9AB4
        inc    edx
        and    edx, 0x7ff
        mov    dword ptr [0], edx            ; -> loc_1_9418
        lea    ecx, [edx + 1]
        and    ecx, 0x7ff
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    ecx, dword ptr [ebp - 0x14]
        add    ecx, edi
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [edx*4], ecx        ; -> obj1:0x9AB4
        lea    ecx, [edx + 1]
        and    ecx, 0x7ff
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    edx, dword ptr [esi + 0x172]
        out    dx, eax
        mov    ax, word ptr [ebp - 0x14]
        add    edx, 6
        add    ax, di
        out    dx, ax
        cli    
        mov    edi, dword ptr [esi + 0x39e]
loc_1_1F6C:
        cmp    ebx, dword ptr [ebp - 4]
        jl     loc_1_1FAB
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        push   edi
        call   sub_1_906C
        sub    ebx, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_1FB6
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    loc_1_1F6C
loc_1_1FAB:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   edi
        call   sub_1_906C
loc_1_1FB6:
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0xa001
        mov    edi, dword ptr [ebp - 0x18]
        mov    esi, dword ptr [esi + 0x172]
        out    dx, ax
        lea    edx, [esi + 0xe]
loc_1_1FCF:
        mov    dword ptr [ebp - 0x10], edi
        push   ecx
        mov    ecx, dword ptr [ebp - 0x10]
loc_1_1FD6:
        xor    eax, eax
        dec    ecx
        jne    loc_1_1FD6
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    loc_1_1FCF
        sti    
        lea    edx, [esi + 0xc]
        mov    eax, 0x1000
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        in     ax, dx
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2008:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_926C
        mov    ebp, esp
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    edx, dword ptr [ebp + 0x14]
        push   esi
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    word ptr [esi + 0x2e], dx
        mov    eax, dword ptr [ebp + 0x10]
        add    ax, 0xe
        mov    dword ptr [esi + 0x28], edi
        mov    word ptr [esi + 0x2c], ax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        call   dword ptr [eax + 0xc]
        test   eax, eax
        je     loc_1_2066
        cmp    eax, 1
        je     loc_1_2076
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
loc_1_2066:
        mov    ecx, dword ptr [esi + 0x30]
        mov    eax, dword ptr [ebp + 0x1c]
        pop    edi
        pop    esi
        pop    ebp
        mov    dword ptr [eax], ecx
        xor    eax, eax
        ret    0x18
loc_1_2076:
        mov    eax, 0x103
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2088:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        mov    esi, dword ptr [ebx + 0x1fe]
        mov    eax, dword ptr [ebx + 0x192]
        mov    edx, dword ptr [ebx + 0x176]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, 0x6820
        cmp    byte ptr [ebx + 0xb6], 0
        out    dx, ax
        je     loc_1_251F
        mov    eax, dword ptr [0]            ; -> loc_1_9274
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xe2]
        mov    edx, dword ptr [ebx + 0x176]
        mov    byte ptr [ebx + 0xb6], 0
        out    dx, ax
        mov    ecx, dword ptr [ebx + 0x36f]
        mov    edx, dword ptr [ebx + 0x18a]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], ecx
        mov    ecx, dword ptr [ebx + 0x396]
        mov    dword ptr [ebp - 8], eax
        add    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        inc    eax
        mov    dword ptr [ebp - 0x10], ecx
        mov    ecx, dword ptr [ebp - 8]
        and    dword ptr [ebp - 8], 0x1ffc
        mov    dword ptr [eax*4 - 4], ecx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [ebx + 0x29e]
        test   eax, eax
        jne    loc_1_21B2
        mov    ecx, dword ptr [ebx + 0x2ee]
        cmp    dword ptr [ebx + 0xea], ecx
        jbe    loc_1_21B2
        mov    edx, dword ptr [ebx + 0x172]
        mov    eax, dword ptr [ebx + 0x2d2]
        mov    cx, 0x5ea
        add    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx + 0x29e], 2
        mov    byte ptr [ebp - 1], 1
        out    dx, eax
        sub    cx, word ptr [ebp - 0xc]
        mov    ax, cx
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 0x176]
        mov    eax, 0xa000
        out    dx, ax
        mov    eax, dword ptr [0]            ; -> loc_1_927C
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        jmp    loc_1_21E0
loc_1_21B2:
        cmp    eax, 2
        jne    loc_1_21DC
        mov    eax, dword ptr [0]            ; -> loc_1_9284
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        jmp    loc_1_21E0
loc_1_21DC:
        mov    byte ptr [ebp - 1], 0
loc_1_21E0:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_2318
        mov    eax, dword ptr [0]            ; -> loc_1_928C
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ebx + 0x172]
        add    edx, 0xe
loc_1_2212:
        mov    dword ptr [ebp - 0x28], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 0x28]
loc_1_2219:
        xor    eax, eax
        dec    ecx
        jne    loc_1_2219
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    loc_1_2212
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    edx, dword ptr [0]            ; -> loc_1_9294
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        mov    edx, dword ptr [0]            ; -> obj1:0x929C
        and    eax, 0x7ff
        mov    ecx, 0x5ea
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ebx + 0x172]
        add    edx, 6
        in     ax, dx
        mov    edx, eax
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        and    edx, 0xffff
        inc    eax
        sub    ecx, edx
        mov    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [eax*4 - 4], ecx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x4000
        mov    edx, dword ptr [ebx + 0x172]
        mov    esi, dword ptr [0]            ; -> loc_1_9418
        add    edx, 0xc
        out    dx, ax
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0xffff
        mov    dword ptr [esi*4], ecx        ; -> obj1:0x9AB4
        lea    ecx, [esi + 1]
        and    ecx, 0x7ff
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    dword ptr [ebx + 0x29e], 0
        mov    edx, dword ptr [ebx + 0x18a]
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        jmp    loc_1_23F5
loc_1_2318:
        mov    eax, dword ptr [0]            ; -> loc_1_92A4
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
loc_1_2337:
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        mov    ecx, dword ptr [ebp - 8]
        inc    eax
        add    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [ebp - 0x24], ecx
        mov    dword ptr [eax*4 - 4], ecx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x24]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebx + 0x18a]
        in     ax, dx
        and    eax, 0xffff
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        add    dword ptr [ebp - 0x10], ecx
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        test   byte ptr [ebp - 7], 0x80
        mov    dword ptr [0], eax            ; -> loc_1_9418
        je     loc_1_23A8
        and    dword ptr [ebp - 8], 0x1ffc
        jmp    loc_1_2337
loc_1_23A8:
        test   byte ptr [ebp - 7], 0x40
        jne    loc_1_23FB
        and    dword ptr [ebp - 8], 0x1fff
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        add    edx, 3
        mov    eax, edx
        mov    dword ptr [ebp - 0x34], edx
        and    eax, 0xfffffffc
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x34]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0xc], ecx
loc_1_23F5:
        test   byte ptr [ebp - 7], 0x40
        je     loc_1_2464
loc_1_23FB:
        cmp    dword ptr [ebx + 0x29e], 2
        jne    loc_1_2408
        mov    byte ptr [ebp - 1], 1
loc_1_2408:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_2451
        mov    eax, dword ptr [0]            ; -> loc_1_92AC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x280f
        mov    edx, dword ptr [ebx + 0x176]
        out    dx, ax
        mov    edx, dword ptr [ebx + 0x17a]
loc_1_2440:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2440
        mov    dword ptr [ebx + 0x29e], 0
loc_1_2451:
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        push   ebx
        call   sub_1_34D8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2464:
        mov    byte ptr [ebx + 0xb7], 1
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx + 0x373], ecx
        mov    dword ptr [ebx + 0x36f], ecx
        mov    eax, dword ptr [0]            ; -> loc_1_92B4
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        cmp    byte ptr [ebx + 0x36a], 0
        je     loc_1_24BF
        push   ecx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    loc_1_24ED
loc_1_24BF:
        lea    edx, [ebx + 0x36b]
        mov    eax, dword ptr [ebx + 0x373]
        sub    eax, 0xe
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebx + 0x36f]
        sub    eax, 0xe
        lea    esi, [ecx + 0xe]
        push   eax
        push   esi
        push   0xe
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [ebx + 0x5e]
        push   ecx
        call   sub_1_8B30
loc_1_24ED:
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        push   ebx
        call   sub_1_3668
        mov    eax, dword ptr [0]            ; -> loc_1_92BC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 0x7ff
        pop    ebp
        mov    dword ptr [0], eax            ; -> loc_1_9418
        ret    4
loc_1_251F:
        mov    eax, dword ptr [0]            ; -> obj1:0x92C4
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [ebx + 0x396]
        mov    ecx, dword ptr [ebx + 0xc6]
        mov    edx, dword ptr [ebx + 0x18a]
        mov    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [ebp - 0x14], ecx
        mov    edi, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebp - 0x10], eax
        in     ax, dx
        mov    edx, eax
        lea    eax, [edi + 1]
        and    edx, 0xffff
        and    eax, 0x7ff
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [edi*4], edx        ; -> obj1:0x9AB4
        and    dword ptr [ebp - 8], 0x1ffc
        mov    dword ptr [0], eax            ; -> loc_1_9418
        cmp    dword ptr [ebx + 0x2ee], ecx
        jae    loc_1_25ED
        cmp    dword ptr [ebx + 0x29e], 0
        jne    loc_1_25E9
        mov    edx, dword ptr [ebx + 0x172]
        mov    eax, dword ptr [ebx + 0x2d2]
        mov    dword ptr [ebx + 0x29e], 2
        mov    byte ptr [ebp - 1], 1
        out    dx, eax
        mov    ax, cx
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 0x176]
        mov    eax, 0xa000
        out    dx, ax
        mov    eax, dword ptr [0]            ; -> obj1:0x92CC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        jmp    loc_1_25ED
loc_1_25E9:
        mov    byte ptr [ebp - 1], 0
loc_1_25ED:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_2837
        mov    eax, dword ptr [0]            ; -> loc_1_92D4
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ebx + 0x172]
        add    edx, 0xe
loc_1_261F:
        mov    dword ptr [ebp - 0x20], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 0x20]
loc_1_2626:
        xor    eax, eax
        dec    ecx
        jne    loc_1_2626
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    loc_1_261F
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [0]            ; -> loc_1_92DC
        mov    edx, dword ptr [0]            ; -> obj1:0x92E4
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ebx + 0x172]
        add    edx, 6
        in     ax, dx
        mov    ecx, eax
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        and    ecx, 0xffff
        inc    eax
        sub    dword ptr [ebp - 0xc], ecx
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax*4 - 4], edx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x4000
        mov    edx, dword ptr [ebx + 0x172]
        mov    esi, dword ptr [0]            ; -> loc_1_9418
        add    edx, 0xc
        out    dx, ax
        in     ax, dx
        mov    ecx, eax
        mov    edx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        add    dword ptr [ebp - 0x10], edx
        mov    dword ptr [esi*4], ecx        ; -> obj1:0x9AB4
        lea    ecx, [esi + 1]
        and    ecx, 0x7ff
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    dword ptr [ebx + 0x29e], 0
        mov    edx, dword ptr [ebx + 0x18a]
        in     ax, dx
        and    eax, 0xffff
        test   ah, 0x80
        mov    dword ptr [ebp - 8], eax
        jne    loc_1_28ED
        test   ah, 0x40
        je     loc_1_2729
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        push   ebx
        call   sub_1_34D8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2729:
        and    dword ptr [ebp - 8], 0x1fff
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        add    edx, 3
        mov    eax, edx
        mov    dword ptr [ebp - 0x30], edx
        and    eax, 0xfffffffc
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x30]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    byte ptr [ebx + 0xb7], 1
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0xc], ecx
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx + 0x36f], eax
        mov    dword ptr [ebx + 0x373], eax
        mov    edx, dword ptr [0]            ; -> loc_1_92EC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        lea    eax, [ecx + 1]
        mov    dword ptr [ecx*4], edx        ; -> obj1:0x9AB4
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        cmp    byte ptr [ebx + 0x36a], 0
        je     loc_1_27F6
        push   ecx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    loc_1_2824
        lea    ecx, [ebx + 0x36b]
        mov    eax, dword ptr [ebx + 0x373]
        sub    eax, 0xe
        mov    edx, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ebx + 0x36f]
        sub    eax, 0xe
        lea    esi, [edx + 0xe]
        push   eax
        push   esi
        push   0xe
        push   edx
        push   edx
        push   ecx
        jmp    loc_1_281B
loc_1_27F6:
        lea    edx, [ebx + 0x36b]
        mov    eax, dword ptr [ebx + 0x373]
        sub    eax, 0xe
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebx + 0x36f]
        sub    eax, 0xe
        lea    esi, [ecx + 0xe]
        push   eax
        push   esi
        push   0xe
        push   ecx
        push   ecx
        push   edx
loc_1_281B:
        mov    ecx, dword ptr [ebx + 0x5e]
        push   ecx
        call   sub_1_8B30
loc_1_2824:
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        push   ebx
        call   sub_1_3668
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2837:
        mov    eax, dword ptr [0]            ; -> loc_1_92F4
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        cmp    dword ptr [ebp - 0x14], 0
        mov    dword ptr [0], eax            ; -> loc_1_9418
        je     loc_1_28ED
loc_1_2860:
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        cmp    eax, ecx
        jb     loc_1_286C
        mov    eax, ecx
loc_1_286C:
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [ebp - 0x1c], ecx
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x1c]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebx + 0x18a]
        add    dword ptr [ebp - 0x10], eax
        sub    dword ptr [ebp - 0x14], eax
        in     ax, dx
        mov    ecx, eax
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        and    ecx, 0xffff
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [edx*4], ecx        ; -> obj1:0x9AB4
        mov    dword ptr [0], eax            ; -> loc_1_9418
        test   ch, 0x80
        je     loc_1_2927
        and    dword ptr [ebp - 8], 0x1ffc
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_2860
loc_1_28ED:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebx + 0x396]
        xor    eax, eax
        mov    dword ptr [ebx + 0x36f], ecx
        mov    cx, word ptr [edx + 0xc]
        mov    al, cl
        shl    eax, 8
        shr    cx, 8
        and    ecx, 0xffff
        add    ecx, eax
        cmp    ecx, 0x600
        jae    loc_1_2A2C
        add    ecx, 0xe
        jmp    loc_1_2A9A
loc_1_2927:
        test   byte ptr [ebp - 7], 0x40
        je     loc_1_2943
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, dword ptr [ebp - 0x14]
        push   eax
        push   ebx
        call   sub_1_34D8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2943:
        and    dword ptr [ebp - 8], 0x1fff
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        add    edx, 3
        mov    eax, edx
        mov    dword ptr [ebp - 0x2c], edx
        and    eax, 0xfffffffc
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x2c]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [ebx + 0xb7], 1
        sub    dword ptr [ebp - 0xc], ecx
        add    dword ptr [ebp - 0xc], eax
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx + 0x36f], edx
        mov    dword ptr [ebx + 0x373], edx
        mov    eax, dword ptr [0]            ; -> loc_1_92FC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        cmp    byte ptr [ebx + 0x36a], 0
        je     loc_1_29EB
        push   edx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    loc_1_2A19
loc_1_29EB:
        lea    ecx, [ebx + 0x36b]
        mov    eax, dword ptr [ebx + 0x373]
        sub    eax, 0xe
        mov    edx, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ebx + 0x36f]
        sub    eax, 0xe
        lea    esi, [edx + 0xe]
        push   eax
        push   esi
        push   0xe
        push   edx
        push   edx
        push   ecx
        mov    ecx, dword ptr [ebx + 0x5e]
        push   ecx
        call   sub_1_8B30
loc_1_2A19:
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        push   ebx
        call   sub_1_3668
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A2C:
        jne    loc_1_2A4B
        mov    cx, word ptr [edx + 0x10]
        xor    eax, eax
        mov    al, cl
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0xffff
        shl    eax, 8
        lea    ecx, [eax + edx + 0xe]
        jmp    loc_1_2A9A
loc_1_2A4B:
        cmp    ecx, 0x800
        jne    loc_1_2A70
        mov    cx, word ptr [edx + 0x10]
        xor    eax, eax
        mov    al, cl
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0xffff
        shl    eax, 8
        lea    ecx, [eax + edx + 0xe]
        jmp    loc_1_2A9A
loc_1_2A70:
        cmp    ecx, 0x8137
        jne    loc_1_2A95
        mov    cx, word ptr [edx + 0x10]
        xor    eax, eax
        mov    al, cl
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0xffff
        shl    eax, 8
        lea    ecx, [eax + edx + 0xe]
        jmp    loc_1_2A9A
loc_1_2A95:
        mov    ecx, 0x5ea
loc_1_2A9A:
        cmp    ecx, 0x5ea
        ja     loc_1_2AA7
        cmp    dword ptr [ebp - 0xc], ecx
        jbe    loc_1_2AAC
loc_1_2AA7:
        mov    ecx, 0x5ea
loc_1_2AAC:
        mov    esi, ecx
        sub    esi, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx + 0xea], esi
        mov    eax, dword ptr [0]            ; -> loc_1_9304
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        cmp    dword ptr [ebx + 0x2ee], ecx
        jae    loc_1_2B7C
        cmp    dword ptr [ebx + 0x29e], 0
        jne    loc_1_2B59
        mov    dword ptr [ebx + 0x29e], 2
        mov    eax, dword ptr [0]            ; -> loc_1_930C
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    di, 0x5ea
        mov    dword ptr [0], eax            ; -> loc_1_9418
        sub    di, word ptr [ebp - 0xc]
        mov    edx, dword ptr [ebx + 0x172]
        mov    eax, dword ptr [ebx + 0x2d2]
        add    eax, dword ptr [ebp - 0xc]
        out    dx, eax
        mov    ax, di
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 0x176]
        mov    eax, 0xa000
        out    dx, ax
        jmp    loc_1_2B7C
loc_1_2B59:
        mov    eax, dword ptr [0]            ; -> obj1:0x9314
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
loc_1_2B7C:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_2B9A
        mov    eax, dword ptr [ebx + 0x1c2]
        cmp    eax, 0x20
        je     loc_1_2BB1
        cmp    eax, 0x30
        je     loc_1_2BB1
        sub    ecx, dword ptr [ebx + 0x212]
        jmp    loc_1_2BB1
loc_1_2B9A:
        shr    esi, 1
        mov    eax, dword ptr [ebx + 0xda]
        imul   eax, esi
        shr    eax, 5
        sub    ecx, eax
        sub    ecx, dword ptr [ebx + 0x20e]
loc_1_2BB1:
        test   ecx, ecx
        jge    loc_1_2BB7
        xor    ecx, ecx
loc_1_2BB7:
        lea    eax, [ecx + 3]
        mov    edx, dword ptr [ebx + 0x176]
        mov    byte ptr [ebx + 0xb6], 1
        shr    eax, 2
        and    ax, 0x7ff
        sub    ax, 0x7800
        out    dx, ax
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        lea    eax, [edx + 1]
        mov    dword ptr [edx*4], ecx        ; -> obj1:0x9AB4
        and    eax, 0x7ff
        mov    edx, dword ptr [0]            ; -> obj1:0x931C
        pop    edi
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        pop    esi
        pop    ebx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2C18:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    esi, dword ptr [ebx + 0x1fe]
        mov    eax, dword ptr [ebx + 0x192]
        mov    edx, dword ptr [ebx + 0x18a]
        mov    edi, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebp - 0x18], eax
        in     ax, dx
        mov    edx, eax
        mov    eax, dword ptr [0]            ; -> obj1:0x9324
        and    edx, 0xffff
        mov    dword ptr [edi*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edi + 1]
        mov    dword ptr [ebp - 4], edx
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        test   dh, 0x40
        mov    dword ptr [0], eax            ; -> loc_1_9418
        je     loc_1_2D0A
        xor    esi, esi
        cmp    byte ptr [ebx + 0xb6], cl
        je     loc_1_2C92
        mov    esi, dword ptr [ebx + 0x36f]
loc_1_2C92:
        cmp    dword ptr [ebx + 0x29e], 2
        jne    loc_1_2C9D
        mov    cl, 1
loc_1_2C9D:
        test   cl, cl
        je     loc_1_2CE4
        mov    eax, dword ptr [0]            ; -> obj1:0x932C
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x280f
        mov    edx, dword ptr [ebx + 0x176]
        out    dx, ax
        mov    edx, dword ptr [ebx + 0x17a]
loc_1_2CD3:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_2CD3
        mov    dword ptr [ebx + 0x29e], 0
loc_1_2CE4:
        push   esi
        push   ebx
        call   sub_1_34D8
        mov    ax, word ptr [ebx + 0xe2]
        mov    edx, dword ptr [ebx + 0x176]
        mov    byte ptr [ebx + 0xb6], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
loc_1_2D0A:
        and    dword ptr [ebp - 4], 0x1fff
        cmp    byte ptr [ebx + 0xb6], 0
        je     loc_1_2FC0
        mov    eax, dword ptr [0]            ; -> loc_1_9334
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    ax, word ptr [ebx + 0xe2]
        mov    edx, dword ptr [ebx + 0x176]
        mov    byte ptr [ebx + 0xb6], 0
        out    dx, ax
        mov    ecx, dword ptr [ebx + 0x36f]
        mov    eax, dword ptr [ebx + 0x396]
        add    eax, ecx
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebx + 0x29e]
        test   eax, eax
        jne    loc_1_2DD6
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x2ee], edx
        jae    loc_1_2DD6
        mov    cl, 1
        mov    edx, dword ptr [ebx + 0x172]
        mov    di, 0x5ea
        mov    eax, dword ptr [ebx + 0x2d2]
        sub    di, word ptr [ebp - 8]
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 0x29e], 2
        out    dx, eax
        mov    ax, di
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 0x176]
        mov    eax, 0xa000
        out    dx, ax
        mov    eax, dword ptr [0]            ; -> loc_1_933C
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        jmp    loc_1_2DE0
loc_1_2DD6:
        sub    eax, 2
        cmp    eax, 1
        sbb    cl, cl
        neg    cl
loc_1_2DE0:
        test   cl, cl
        je     loc_1_2EFC
        mov    eax, dword ptr [0]            ; -> loc_1_9344
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [0]            ; -> loc_1_934C
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ebx + 0x172]
        add    edx, 0xe
loc_1_2E28:
        mov    dword ptr [ebp - 0x10], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 0x10]
loc_1_2E2F:
        xor    eax, eax
        dec    ecx
        jne    loc_1_2E2F
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    loc_1_2E28
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    edx, dword ptr [0]            ; -> loc_1_9354
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        mov    edx, dword ptr [0]            ; -> obj1:0x935C
        and    eax, 0x7ff
        mov    ecx, 0x5ea
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ebx + 0x172]
        add    edx, 6
        in     ax, dx
        mov    edx, eax
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        and    edx, 0xffff
        inc    eax
        sub    ecx, edx
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [eax*4 - 4], ecx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x4000
        mov    edx, dword ptr [ebx + 0x172]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        add    edx, 0xc
        out    dx, ax
        in     ax, dx
        and    eax, 0xffff
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [ebx + 0x29e], 0
        jmp    loc_1_2F58
loc_1_2EFC:
        mov    eax, dword ptr [0]            ; -> loc_1_9364
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        add    dword ptr [ebp - 8], edx
        and    eax, 0x7ff
        add    edx, 3
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, edx
        and    eax, 0xfffffffc
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebp - 0x24], edx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x24]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
loc_1_2F58:
        mov    byte ptr [ebx + 0xb7], 1
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 0x373], ecx
        mov    dword ptr [ebx + 0x36f], ecx
        mov    eax, dword ptr [0]            ; -> loc_1_936C
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [edx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [edx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        cmp    byte ptr [ebx + 0x36a], 0
        je     loc_1_3196
        push   ecx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    loc_1_31C4
        jmp    loc_1_3196
loc_1_2FC0:
        mov    eax, dword ptr [0]            ; -> loc_1_9374
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    ecx, 1
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [ebx + 0x29e]
        mov    dword ptr [ebx + 0x373], edx
        mov    dword ptr [ebx + 0x36f], edx
        mov    byte ptr [ebx + 0xb7], cl
        test   eax, eax
        jne    loc_1_303A
        cmp    dword ptr [ebx + 0x2ee], edx
        jae    loc_1_303A
        mov    edx, dword ptr [ebx + 0x172]
        mov    eax, dword ptr [ebx + 0x2d2]
        mov    dword ptr [ebx + 0x29e], 2
        out    dx, eax
        mov    ax, word ptr [ebp - 4]
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 0x176]
        mov    eax, 0xa000
        out    dx, ax
        jmp    loc_1_3057
loc_1_303A:
        cmp    eax, 2
        jne    loc_1_3055
        mov    dword ptr [ebp - 0x20], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 0x20]
loc_1_3046:
        xor    eax, eax
        dec    ecx
        jne    loc_1_3046
        pop    ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3055:
        xor    cl, cl
loc_1_3057:
        test   cl, cl
        je     loc_1_3108
        mov    eax, dword ptr [0]            ; -> loc_1_937C
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [0]            ; -> obj1:0x9384
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], ecx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ebx + 0x172]
        add    edx, 0xe
loc_1_309F:
        mov    dword ptr [ebp - 0xc], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 0xc]
loc_1_30A6:
        xor    eax, eax
        dec    ecx
        jne    loc_1_30A6
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    loc_1_309F
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    edx, dword ptr [0]            ; -> loc_1_938C
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x4000
        mov    dword ptr [ebx + 0x29e], 0
        mov    edx, dword ptr [ebx + 0x172]
        add    edx, 0xc
        out    dx, ax
        in     ax, dx
        jmp    loc_1_314B
loc_1_3108:
        mov    eax, dword ptr [ebx + 0x396]
        mov    edx, dword ptr [ebp - 4]
        add    edx, 3
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, edx
        and    eax, 0xfffffffc
        mov    dword ptr [ebp - 0x1c], edx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x1c]
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        mov    edi, eax
loc_1_314B:
        mov    eax, dword ptr [0]            ; -> loc_1_9394
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    dword ptr [ebp - 8], edx
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        cmp    byte ptr [ebx + 0x36a], 0
        je     loc_1_3196
        push   edx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    loc_1_31C4
loc_1_3196:
        lea    ecx, [ebx + 0x36b]
        mov    eax, dword ptr [ebx + 0x373]
        sub    eax, 0xe
        mov    edx, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ebx + 0x36f]
        sub    eax, 0xe
        lea    esi, [edx + 0xe]
        push   eax
        push   esi
        push   0xe
        push   edx
        push   edx
        push   ecx
        mov    ecx, dword ptr [ebx + 0x5e]
        push   ecx
        call   sub_1_8B30
loc_1_31C4:
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        push   ebx
        call   sub_1_3668
        mov    eax, dword ptr [0]            ; -> loc_1_939C
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 0x7ff
        pop    ebp
        mov    dword ptr [0], eax            ; -> loc_1_9418
        ret    4
        db  08Bh, 0FFh
loc_1_31F8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x93A4
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        sub    esp, 0xc
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        push   ebx
        push   esi
        and    eax, 0x7ff
        push   edi
        xor    edx, edx
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dx, word ptr [esi + 0x2c]
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        xor    edx, edx
        xor    ecx, ecx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dx, word ptr [esi + 0x2e]
        mov    dword ptr [eax*4], edx        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        xor    eax, eax
        mov    cx, word ptr [esi + 0x2c]
        mov    edi, dword ptr [edx + 0x396]
        mov    edx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [esi + 0x2e]
        add    edi, ecx
        mov    ebx, dword ptr [edx + 8]
        sub    ebx, ecx
        cmp    ebx, eax
        jbe    loc_1_3278
        mov    ebx, eax
loc_1_3278:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    dword ptr [esi + 0x30], ebx
        push   0
        push   0
        push   esi
        call   sub_1_8FB0
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_8FD4
        test   ebx, ebx
        je     loc_1_332B
loc_1_32A4:
        cmp    ebx, dword ptr [ebp - 4]
        jbe    loc_1_3304
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp - 4]
        and    eax, 0x7ff
        push   ecx
        push   edi
        mov    dword ptr [0], eax            ; -> loc_1_9418
        push   edx
        call   sub_1_906C
        sub    ebx, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_3328
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_8FD4
        test   ebx, ebx
        jne    loc_1_32A4
        jmp    loc_1_332B
loc_1_3304:
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        push   ebx
        inc    eax
        push   edi
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [eax*4 - 4], ebx    ; -> obj1:0x9AB0
        and    eax, 0x7ff
        push   ecx
        mov    dword ptr [0], eax            ; -> loc_1_9418
        call   sub_1_906C
        jmp    loc_1_332B
loc_1_3328:
        sub    dword ptr [esi + 0x30], ebx
loc_1_332B:
        mov    eax, dword ptr [0]            ; -> loc_1_93AC
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 0x7ff
        pop    ebp
        mov    dword ptr [0], eax            ; -> loc_1_9418
        xor    eax, eax
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_3358:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x93B4
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        sub    esp, 0xc
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        push   ebx
        push   esi
        and    eax, 0x7ff
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [esi]
        mov    di, word ptr [ecx + 0x2c]
        add    eax, 0xf
        and    eax, 0xfffffff0
        mov    dword ptr [esi + 0x387], edi
        cmp    eax, edi
        jae    loc_1_33A0
        mov    edi, eax
loc_1_33A0:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        mov    dword ptr [esi + 0x383], edi
        push   0
        push   ecx
        call   sub_1_8FB0
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 0xc]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_8FD4
        mov    ebx, dword ptr [esi + 0x39a]
loc_1_33D0:
        cmp    edi, dword ptr [ebp - 4]
        jb     loc_1_340F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_906C
        sub    edi, dword ptr [ebp - 4]
        add    ebx, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_341A
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    loc_1_33D0
loc_1_340F:
        push   edi
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   ebx
        call   sub_1_906C
loc_1_341A:
        mov    eax, dword ptr [ebp + 0xc]
        lea    edx, [esi + 0x37f]
        mov    dword ptr [esi + 0x38f], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [esi + 0x387]
        lea    ebx, [ecx + 0xe]
        sub    eax, 0xe
        push   eax
        mov    eax, dword ptr [esi + 0x383]
        sub    eax, 0xe
        push   eax
        push   ebx
        push   0xe
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [esi + 0x5e]
        push   ecx
        call   sub_1_8B30
        cmp    byte ptr [esi + 0xc2], 0
        je     loc_1_346A
        mov    byte ptr [esi + 0xb7], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_346A:
        mov    eax, dword ptr [esi + 0x5e]
        push   eax
        call   sub_1_8D10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3488:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_93BC
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        lea    edx, [ecx + 0x30]
        push   edx
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x2c]
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [edx + 0x10]
        push   eax
        xor    edx, edx
        mov    dx, word ptr [ecx + 0x2e]
        push   edx
        push   0
        push   ecx
        call   sub_1_8FFE
        xor    eax, eax
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_34D8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_93C4
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp + 8]
        and    eax, 0x7ff
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    ebx, dword ptr [ecx + 0x172]
        lea    edx, [ebx + 0x18]
        in     ax, dx
        mov    dx, ax
        and    dx, 0x1fff
        lea    edi, [edx + esi]
        lea    edx, [ebx + 0x14]
        in     al, dx
        xor    edx, edx
        test   al, 1
        mov    dl, al
        je     loc_1_3544
        mov    eax, dword ptr [0]            ; -> obj1:0x93CC
        mov    ebx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ebx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
loc_1_3544:
        test   dl, 2
        je     loc_1_356E
        mov    eax, dword ptr [0]            ; -> loc_1_93D4
        mov    ebx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ebx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        inc    dword ptr [ecx + 0xfe]
loc_1_356E:
        test   dl, 4
        je     loc_1_3598
        mov    eax, dword ptr [0]            ; -> loc_1_93DC
        mov    ebx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ebx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        inc    dword ptr [ecx + 0x33e]
loc_1_3598:
        test   dl, 8
        je     loc_1_35C2
        mov    eax, dword ptr [0]            ; -> loc_1_93E4
        mov    ebx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ebx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        inc    dword ptr [ecx + 0x342]
loc_1_35C2:
        test   dl, 0x10
        je     loc_1_35EC
        mov    eax, dword ptr [0]            ; -> obj1:0x93EC
        mov    ebx, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [ebx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ebx + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        inc    dword ptr [ecx + 0x346]
loc_1_35EC:
        cmp    edx, 1
        je     loc_1_3609
        cmp    edx, 2
        je     loc_1_3609
        mov    eax, edi
        dec    dword ptr [ecx + 0x306]
        and    eax, 0xffff
        sub    dword ptr [ecx + 0x312], eax
loc_1_3609:
        push   esi
        push   ecx
        call   sub_1_3668
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_3618:
        push   ebp
        mov    cl, 0xf5
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 0xc6]
        sub    cl, byte ptr [edx + 0x20e]
        shr    esi, 1
        mov    eax, dword ptr [edx + 0xe6]
        imul   esi, dword ptr [edx + 0xda]
        shr    esi, cl
        shl    eax, 2
        cmp    eax, esi
        jle    loc_1_3648
        mov    esi, eax
loc_1_3648:
        add    esi, 3
        and    esi, 0x1ffc
        shr    esi, 2
        add    esi, 0x8800
        mov    dword ptr [edx + 0xe2], esi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3668:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_93F4
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    edi, 0x1fff
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ecx + 0x172]
        add    edx, 0x18
        in     ax, dx
        and    ax, 0x1fff
        lea    esi, [eax + edi + 3]
        mov    eax, dword ptr [0]            ; -> loc_1_9418
        and    esi, 0xfffc
        nop    
        mov    dword ptr [eax*4], edi        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    dword ptr [eax*4], esi        ; -> obj1:0x9AB4
        inc    eax
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x4000
        mov    edx, dword ptr [ecx + 0x176]
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x17a]
loc_1_36EE:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_36EE
        cmp    byte ptr [ecx + 0x36a], 0
        je     loc_1_3705
        push   esi
        push   ecx
        call   sub_1_3898
loc_1_3705:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3718:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_93FC
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   esi
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp + 8]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, 0x2800
        mov    esi, dword ptr [ecx + 0x176]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x17a]
loc_1_3753:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_3753
        mov    ax, word ptr [ecx + 0x102]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0x2000
        pop    esi
        out    dx, ax
        mov    ax, word ptr [ecx + 0xe2]
        pop    ebp
        out    dx, ax
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3788:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_9404
        mov    ebp, esp
        mov    ecx, dword ptr [0]            ; -> loc_1_9418
        push   esi
        push   edi
        mov    dword ptr [ecx*4], eax        ; -> obj1:0x9AB4
        lea    eax, [ecx + 1]
        and    eax, 0x7ff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    edx, dword ptr [ecx + 0x362]
        cmp    dword ptr [ecx + 0x35e], edx
        je     loc_1_387C
        mov    edx, dword ptr [ebp + 0xc]
loc_1_37C4:
        mov    eax, dword ptr [ecx + 0x35a]
        mov    esi, dword ptr [0]            ; -> loc_1_9418
        add    eax, edx
        mov    dword ptr [esi*4], eax        ; -> obj1:0x9AB4
        lea    eax, [esi + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    esi, dword ptr [ecx + 0x35e]
        mov    edi, dword ptr [ecx + 0x366]
        mov    eax, dword ptr [edi + esi*8]
        mov    esi, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [esi*4], eax        ; -> obj1:0x9AB4
        lea    eax, [esi + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    esi, dword ptr [ecx + 0x35e]
        mov    edi, dword ptr [ecx + 0x366]
        mov    eax, dword ptr [edi + esi*8 + 4]
        mov    esi, dword ptr [0]            ; -> loc_1_9418
        mov    dword ptr [esi*4], eax        ; -> obj1:0x9AB4
        lea    eax, [esi + 1]
        and    eax, 0x7ff
        mov    dword ptr [0], eax            ; -> loc_1_9418
        mov    eax, dword ptr [ecx + 0x35a]
        mov    edi, dword ptr [ecx + 0x35e]
        add    eax, edx
        lea    esi, [edi*8]
        add    esi, dword ptr [ecx + 0x366]
        cmp    dword ptr [esi], eax
        ja     loc_1_3884
        cmp    dword ptr [esi + 4], eax
        jae    loc_1_388C
        inc    edi
        mov    dword ptr [ecx + 0x35e], edi
        and    edi, 0x1ff
        mov    dword ptr [ecx + 0x35e], edi
        cmp    dword ptr [ecx + 0x362], edi
        jne    loc_1_37C4
loc_1_387C:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3884:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_388C:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_3898:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, 0x803
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0x176]
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 0x172]
        mov    ebx, dword ptr [0]            ; -> loc_1_940C
        add    edx, 0xa
        in     ax, dx
        mov    di, ax
        mov    edx, ecx
        mov    eax, 0x807
        and    edi, 0xffff
        out    dx, ax
        mov    edx, dword ptr [0]            ; -> loc_1_9418
        lea    ecx, [edx + 1]
        mov    dword ptr [edx*4], ebx        ; -> obj1:0x9AB4
        and    ecx, 0x7ff
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    dword ptr [ecx*4], edi        ; -> obj1:0x9AB4
        inc    ecx
        and    ecx, 0x7ff
        mov    dword ptr [0], ecx            ; -> loc_1_9418
        mov    edx, dword ptr [esi + 0x172]
        add    edx, 0x18
        in     ax, dx
        test   ah, 0x80
        je     loc_1_3935
        mov    eax, dword ptr [esi + 0x35e]
        pop    edi
        mov    dword ptr [esi + 0x35a], 0
        mov    dword ptr [esi + 0x362], eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3935:
        mov    ecx, dword ptr [ebp + 0xc]
        and    edi, 0xfffffffc
        lea    eax, [ecx + 3]
        and    eax, 0xfffffffc
        add    eax, 4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, edi
        jb     loc_1_39AE
        mov    edx, dword ptr [esi + 0x362]
        lea    eax, [edx + 1]
        and    eax, 0x1ff
        mov    dword ptr [esi + 0x362], eax
        cmp    dword ptr [esi + 0x35e], eax
        jne    loc_1_3980
        push   esi
        call   sub_1_3718
        mov    dword ptr [esi + 0x35a], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3980:
        mov    eax, dword ptr [esi + 0x352]
        mov    ebx, dword ptr [esi + 0x366]
        add    eax, dword ptr [esi + 0x35a]
        sub    eax, 0x14
        mov    dword ptr [ebx + edx*8], eax
        mov    eax, dword ptr [esi + 0x366]
        lea    eax, [eax + edx*8]
        mov    edx, dword ptr [eax]
        sub    edx, edi
        add    edx, dword ptr [ebp - 4]
        add    edx, 0x14
        mov    dword ptr [eax + 4], edx
loc_1_39AE:
        mov    eax, dword ptr [esi + 0x35a]
        lea    edx, [eax + ecx + 4]
        mov    eax, dword ptr [esi + 0x352]
        add    ecx, eax
        mov    dword ptr [esi + 0x35a], edx
        add    ecx, 4
        add    eax, edx
        cmp    ecx, eax
        jbe    loc_1_3A01
        sub    edx, 0x80000000
        xor    ecx, ecx
        mov    dword ptr [esi + 0x35a], edx
        mov    edx, 0x80000000
loc_1_39E2:
        mov    eax, dword ptr [esi + 0x366]
        add    ecx, 8
        add    dword ptr [eax + ecx - 8], edx
        mov    eax, dword ptr [esi + 0x366]
        add    dword ptr [eax + ecx - 4], edx
        cmp    ecx, 0x1000
        jb     loc_1_39E2
loc_1_3A01:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A18:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 0xf6]
        mov    dword ptr [esi + 0xf6], eax
        cmp    eax, 0x3e8
        jbe    loc_1_3A4A
        mov    dword ptr [esi + 0xf6], 0
        push   esi
        call   sub_1_0348
loc_1_3A4A:
        cmp    byte ptr [esi + 0xb8], 0
        jne    loc_1_3A9C
        mov    eax, dword ptr [esi + 0x31a]
        cmp    eax, 0x3e8
        jbe    loc_1_3A9C
        shl    eax, 2
        cmp    eax, dword ptr [esi + 0x302]
        jbe    loc_1_3A9C
        mov    ecx, dword ptr [esi + 0x176]
        mov    eax, 0x804
        mov    byte ptr [esi + 0xb8], 1
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 0x172]
        add    edx, 0xa
        in     ax, dx
        and    eax, 0xfff7
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x807
        out    dx, ax
loc_1_3A9C:
        mov    cx, word ptr [esi + 0x206]
        mov    ax, word ptr [esi + 0x208]
        mov    ebx, ecx
        sub    dx, dx
        and    ebx, 0xffff
        div    cx
        mov    word ptr [ebp - 2], ax
        sub    edx, edx
        mov    cx, word ptr [ebp - 2]
        mov    eax, dword ptr [esi + 0x20a]
        div    ebx
        imul   cx, cx
        sub    ax, cx
        mov    edx, dword ptr [esi + 0x4a]
        mov    cx, ax
        shr    ax, 0xc
        mov    edi, eax
        and    edi, 0xffff
        mov    bl, byte ptr [edi + edx + 0x2e4]
        and    ebx, 0xffff00ff
        test   bx, bx
        jne    loc_1_3B00
        mov    bx, cx
        shr    bx, 5
        add    bx, 2
loc_1_3B00:
        mov    ax, cx
        sub    dx, dx
        div    bx
        mov    edi, eax
        mov    eax, ebx
        sub    dx, dx
        and    edi, 0xffff
        and    eax, 0xffff
        add    edi, eax
        sar    edi, 1
        mov    ax, cx
        div    di
        mov    ecx, eax
        mov    eax, edi
        and    ecx, 0xffff
        and    eax, 0xffff
        add    ecx, eax
        xor    edx, edx
        sar    ecx, 1
        mov    dx, word ptr [ebp - 2]
        mov    eax, ecx
        and    eax, 0xffff
        mov    ecx, dword ptr [esi + 0xde]
        lea    eax, [edx + eax*2]
        shl    eax, 2
        cmp    ecx, 0x40
        mov    dword ptr [esi + 0x20e], eax
        mov    dword ptr [esi + 0x212], eax
        jge    loc_1_3B72
        mov    edx, eax
        imul   edx, ecx
        shl    edx, 6
        mov    dword ptr [esi + 0x212], edx
loc_1_3B72:
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 0x212]
        mov    dword ptr [esi + 0x15a], 0x5f5e100
        add    eax, eax
        mov    dword ptr [esi + 0x20e], eax
        lea    eax, [ecx + ecx*4]
        add    eax, eax
        cmp    dword ptr [esi + 0x1c2], 0x30
        mov    dword ptr [esi + 0x212], eax
        jne    loc_1_3BAB
        mov    dword ptr [esi + 0x212], 0
loc_1_3BAB:
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 0xfa]
        mov    dword ptr [esi + 0xfa], eax
        cmp    eax, 0x3e8
        jbe    loc_1_3BE5
        mov    dword ptr [esi + 0xfa], 0
        cmp    dword ptr [esi + 0xfe], 0x64
        jb     loc_1_3BDB
        add    dword ptr [esi + 0xe6], 2
loc_1_3BDB:
        mov    dword ptr [esi + 0xfe], 0
loc_1_3BE5:
        push   esi
        call   sub_1_3618
        cmp    byte ptr [esi + 0xb6], 0
        jne    loc_1_3C03
        mov    ax, word ptr [esi + 0xe2]
        mov    edx, dword ptr [esi + 0x176]
        out    dx, ax
loc_1_3C03:
        lea    ebx, [esi + 0x21e]
        mov    edi, 4
loc_1_3C0E:
        xor    eax, eax
        mov    ax, word ptr [ebx + 6]
        cmp    eax, dword ptr [esi + 0x21a]
        jl     loc_1_3C23
        push   ebx
        push   esi
        call   sub_1_1AB8
loc_1_3C23:
        lea    eax, [ebx + 0x40]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 6]
        cmp    ecx, dword ptr [esi + 0x21a]
        jl     loc_1_3C3B
        push   eax
        push   esi
        call   sub_1_1AB8
loc_1_3C3B:
        add    ebx, 0x10
        dec    edi
        jne    loc_1_3C0E
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3C58:
        push   ebp
        mov    eax, 6
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 0xc], ax
        push   ebx
        mov    word ptr [ebp - 0xa], ax
        push   edi
        mov    edi, 0                        ; -> loc_1_97C0
        xor    eax, eax
        mov    ecx, 0xbd
        xor    ebx, ebx
        mov    dword ptr [ebp - 8], 0        ; -> loc_1_94BC
        rep stosd dword ptr es:[edi], eax
        mov    word ptr [0], 8               ; -> obj1:0x9794
        push   ebx
        mov    dword ptr [4], ebx            ; -> obj1:0x9798
        mov    word ptr [2], 4               ; -> obj1:0x9796
        push   ebx
        push   0                             ; -> obj1:0x9794
        push   4                             ; -> loc_1_97C4
        call   sub_1_8E9A
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [0x3e], edx         ; -> obj1:0x97FE
        mov    dword ptr [0x42], ecx         ; -> obj1:0x9802
        mov    dword ptr [0x46], ebx         ; -> obj1:0x9806
        mov    byte ptr [8], 3               ; -> loc_1_97C8
        mov    byte ptr [9], bl              ; -> obj1:0x97C9
        mov    dword ptr [0xa], ebx          ; -> loc_1_97CA
        mov    dword ptr [0xe], 0            ; -> loc_1_97CE
        mov    dword ptr [0x12], 0           ; -> loc_1_97D2
        mov    dword ptr [0x16], 0           ; -> loc_1_97D6
        mov    dword ptr [0x1a], 0           ; -> loc_1_97DA
        mov    dword ptr [0x1e], 0           ; -> loc_1_97DE
        mov    dword ptr [0x22], 0           ; -> loc_1_97E2
        mov    dword ptr [0x26], 0           ; -> loc_1_97E6
        mov    dword ptr [0x2a], 0           ; -> loc_1_97EA
        mov    dword ptr [0x2e], 0           ; -> loc_1_97EE
        mov    dword ptr [0x32], 0           ; -> loc_1_97F2
        mov    eax, dword ptr [ebp + 8]
        push   0x36
        push   8                             ; -> loc_1_97C8
        mov    dword ptr [0x36], edx         ; -> obj1:0x97F6
        mov    edx, dword ptr [ebp + 0xc]
        push   0                             ; -> loc_1_97C0
        mov    dword ptr [0x3a], ecx         ; -> obj1:0x97FA
        mov    dword ptr [0x52], eax         ; -> obj1:0x9812
        lea    eax, [ebp - 4]
        mov    dword ptr [0x56], edx         ; -> obj1:0x9816
        mov    ecx, dword ptr [4]            ; -> loc_1_97C4
        push   ecx
        push   0                             ; -> loc_1_97C0
        push   eax
        call   sub_1_8EA6
        cmp    dword ptr [ebp - 4], ebx
        je     loc_1_3DA9
        cmp    dword ptr [0x46], ebx         ; -> obj1:0x9806
        jbe    loc_1_3D91
        xor    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3D91:
        push   0
        mov    eax, dword ptr [4]            ; -> loc_1_97C4
        push   eax
        call   sub_1_8EA0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3DA9:
        xor    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_3DB8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x9674
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   3
        push   0x5d4
        push   ecx
        call   sub_1_8F86
        test   eax, eax
        je     loc_1_3DE7
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3DE7:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0x175
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_42F8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_44E8
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_1_3EB8
        test   eax, eax
        je     loc_1_3E37
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x5d4
        push   eax
        call   sub_1_8F8C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3E37:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7098
        test   eax, eax
        je     loc_1_3E77
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0xaa000004
        mov    ecx, dword ptr [eax + 0x3a]
        push   ecx
        call   sub_1_9058
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 4]
        push   3
        push   0x5d4
        push   ecx
        call   sub_1_8F8C
        mov    eax, 0xc0010006
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3E77:
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   eax
        call   sub_1_3FE8
        mov    esi, eax
        test   esi, esi
        je     loc_1_3EA2
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   0x5d4
        push   eax
        call   sub_1_8F8C
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3EA2:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3EB8:
        push   ebp
        mov    eax, 0xf
        mov    ebp, esp
        sub    esp, 0x24
        mov    word ptr [ebp - 0x24], ax
        push   esi
        mov    word ptr [ebp - 0x22], ax
        push   edi
        mov    eax, 8
        mov    edi, dword ptr [ebp + 0xc]
        mov    word ptr [ebp - 0x1c], ax
        push   edi
        mov    word ptr [ebp - 0x1a], ax
        lea    ecx, [ebp - 8]
        push   ecx
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x20], 0     ; -> obj1:0x9504
        mov    dword ptr [ebp - 0x18], 0     ; -> obj1:0x9534
        push   eax
        call   sub_1_9004
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_3F0C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_3F0C:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [esi + 8], edi
        call   sub_1_8F0C
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3F45
        lea    edx, [esi + 0x3e]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, ecx
        mov    edi, dword ptr [ecx]
        mov    dword ptr [edx], edi
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], cl
loc_1_3F45:
        push   1
        lea    eax, [ebp - 0x24]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_900A
        cmp    dword ptr [ebp - 4], 0
        mov    eax, 8
        jne    loc_1_3F6D
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
loc_1_3F6D:
        cmp    eax, 0xff
        ja     loc_1_3F81
        and    ax, 0xff
        mov    word ptr [esi + 0x5d2], ax
        jmp    loc_1_3F8A
loc_1_3F81:
        mov    word ptr [esi + 0x5d2], 8
loc_1_3F8A:
        push   1
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_900A
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3FC3
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 4]
        cmp    al, 5
        jne    loc_1_3FB5
        xor    eax, eax
        jmp    loc_1_3FC8
loc_1_3FB5:
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xfffffffd
        add    eax, 4
        jmp    loc_1_3FC8
loc_1_3FC3:
        mov    eax, 4
loc_1_3FC8:
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        push   eax
        push   esi
        call   sub_1_6C78
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_9010
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_3FE8:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj1:0x9674
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        lea    ecx, [ebp - 4]
        push   esi
        push   edi
        push   eax
        push   0
        push   0x32
        push   ecx
        call   sub_1_8F86
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_1_4019
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_4019:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x142]
        test   eax, eax
        jne    loc_1_4040
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xa], 5
        jmp    loc_1_4058
loc_1_4040:
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 4]
        jne    loc_1_4051
        mov    dword ptr [eax + 0xa], 2
        jmp    loc_1_4058
loc_1_4051:
        mov    dword ptr [eax + 0xa], 0xff
loc_1_4058:
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        lea    edi, [esi + 0x172]
        mov    edx, 0x20
        mov    dword ptr [eax + 0x16], 1
loc_1_406F:
        mov    eax, dword ptr [esi + 0x16e]
        mov    ebx, dword ptr [ebp - 4]
        add    ecx, 0xc
        cmp    ecx, 0xc
        mov    dword ptr [ebx + ecx + 0xe], eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + ecx + 0x12], edx
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + ecx + 0x16], edi
        jb     loc_1_406F
        mov    eax, 1
        mov    byte ptr [ebp - 0x18], al
        mov    byte ptr [ebp - 0x17], al
        mov    byte ptr [ebp - 0x16], al
        mov    dword ptr [ebp - 0x15], 0
        mov    dword ptr [ebp - 0x11], 4
        mov    ecx, dword ptr [esi + 0x156]
        mov    dword ptr [ebp - 0xd], ecx
        cmp    byte ptr [ebp - 0x18], 0
        je     loc_1_40E9
        mov    cl, byte ptr [ebp - 0x17]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], cl
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x15]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 0x16]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x11]
        mov    dword ptr [eax + 0xe], edx
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 0xd]
        mov    dword ptr [eax + 0x12], ebx
loc_1_40E9:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   ecx
        mov    eax, dword ptr [0]            ; -> loc_1_97C0
        push   edx
        lea    ecx, [esi + 0x3a]
        push   esi
        push   eax
        push   ecx
        call   sub_1_8EB2
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [ecx + 0x22]
        cmp    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [edx]
        mov    byte ptr [esi + 0xad], 1
        mov    dword ptr [edi], eax
        jne    loc_1_4171
        push   esi
        call   sub_1_74C8
        test   eax, eax
        je     loc_1_4148
        push   0
        mov    eax, dword ptr [esi + 0x3a]
        push   0xaa000002
        push   eax
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0xc000009a
        add    esp, 0xc
        jmp    loc_1_429D
loc_1_4148:
        push   esi
        call   sub_1_4738
        test   eax, eax
        je     loc_1_41CE
        cmp    eax, 2
        jne    loc_1_418A
        push   0
        mov    eax, dword ptr [esi + 0x3a]
        push   0xaa000007
        push   eax
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0xc0010004
        add    esp, 0xc
loc_1_4171:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x10
        push   eax
        call   sub_1_8F8C
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_418A:
        cmp    eax, 3
        jne    loc_1_41B2
        mov    eax, dword ptr [esi + 0x16e]
        mov    ecx, dword ptr [esi + 0x3a]
        push   eax
        push   1
        push   0xaa000001
        push   ecx
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0xc001001e
        add    esp, 0x10
        jmp    loc_1_4171
loc_1_41B2:
        push   0
        mov    eax, dword ptr [esi + 0x3a]
        push   0xaa000006
        push   eax
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0xc0000001
        add    esp, 0xc
        jmp    loc_1_4171
loc_1_41CE:
        lea    ebx, [esi + 0x22]
        xor    eax, eax
        mov    edi, ebx
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        mov    eax, 2
        mov    ecx, dword ptr [esi + 0x14a]
        cmp    dword ptr [esi + 0x146], 1
        adc    eax, -1
        push   eax
        mov    al, byte ptr [esi + 0x13e]
        push   eax
        push   ecx
        push   ecx
        mov    eax, dword ptr [esi + 0x3a]
        push   0                             ; -> loc_1_8B08
        lea    edx, [ebp - 8]
        push   esi
        push   0                             ; -> loc_1_8AE8
        push   eax
        push   ebx
        push   edx
        call   sub_1_8F6E
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_4236
        mov    eax, dword ptr [esi + 0x14a]
        mov    ecx, dword ptr [esi + 0x3a]
        push   eax
        push   1
        push   0xaa000005
        push   ecx
        call   sub_1_9058
        add    esp, 0x10
        jmp    loc_1_429D
loc_1_4236:
        push   esi
        call   sub_1_68D8
        test   eax, eax
        je     loc_1_426A
        mov    eax, dword ptr [esi + 0x14a]
        mov    ecx, dword ptr [esi + 0x16e]
        push   eax
        mov    edx, dword ptr [esi + 0x3a]
        push   ecx
        push   2
        push   0xaa000001
        push   edx
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0xc001001e
        add    esp, 0x14
        jmp    loc_1_4297
loc_1_426A:
        lea    eax, [esi + 0x5e]
        lea    ecx, [esi + 0xd]
        push   eax
        lea    edx, [esi + 0x3e]
        push   ecx
        push   edx
        push   0                             ; -> loc_1_7998
        push   0                             ; -> loc_1_7748
        push   0                             ; -> loc_1_7718
        push   0x10
        call   sub_2_0000
loc_1_428C:
        test   al, al
        jne    loc_1_42B1
        mov    dword ptr [ebp - 8], 0xc0000001
loc_1_4297:
        push   ebx
        call   sub_1_8F74
loc_1_429D:
        push   esi
        call   sub_1_7638
        mov    eax, dword ptr [esi + 0x3a]
        push   eax
        call   sub_1_8EB8
        jmp    loc_1_4171
loc_1_42B1:
        xor    ecx, ecx
        mov    dword ptr [esi + 0x56], ecx
        mov    dword ptr [esi + 0x52], ecx
        mov    dword ptr [esi + 0x5a], ecx
        mov    eax, dword ptr [0x4a]         ; -> obj1:0x980A
        mov    dword ptr [esi + 0x4e], eax
        push   ecx
        push   0x32
        mov    dword ptr [0x4a], esi         ; -> obj1:0x980A
        mov    edx, dword ptr [esi + 0x4a]
        mov    dword ptr [esi + 0x62], ecx
        mov    dword ptr [esi + 0x66], 0     ; -> loc_1_76F8
        mov    dword ptr [esi + 0x6a], esi
        inc    dword ptr [edx + 0x46]
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_8F8C
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
sub_1_42F8:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x19], ecx
        mov    dword ptr [eax + 0x1d], ecx
        mov    dword ptr [eax + 0x11], ecx
        mov    dword ptr [eax + 0x15], ecx
        mov    dword ptr [eax + 0x72], ecx
        mov    dword ptr [eax + 0x76], ecx
        mov    dword ptr [eax + 0x7a], ecx
        mov    dword ptr [eax + 0x7e], ecx
        pop    ebp
        mov    dword ptr [eax], 0x4a
        mov    dword ptr [eax + 0x4a], 0     ; -> loc_1_97C0
        mov    dword ptr [eax + 4], 0x64
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 050h, 080h
        db  048h, 021h, 040h, 0E8h, 0EFh, 038h, 000h, 000h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4358:
        push   ebp
        mov    eax, dword ptr [0]            ; -> loc_1_97C0
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_8EAC
        push   0
        mov    ecx, dword ptr [4]            ; -> loc_1_97C4
        push   ecx
        call   sub_1_8EA0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4388:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [0x4a], esi         ; -> obj1:0x980A
        jne    loc_1_43A6
        mov    eax, dword ptr [esi + 0x4e]
        mov    dword ptr [0x4a], eax         ; -> obj1:0x980A
        jmp    loc_1_43C2
loc_1_43A6:
        mov    ecx, dword ptr [0x4a]         ; -> obj1:0x980A
        cmp    dword ptr [ecx + 0x4e], esi
        je     loc_1_43B9
loc_1_43B1:
        mov    ecx, dword ptr [ecx + 0x4e]
        cmp    dword ptr [ecx + 0x4e], esi
        jne    loc_1_43B1
loc_1_43B9:
        mov    eax, dword ptr [ecx + 0x4e]
        mov    edx, dword ptr [eax + 0x4e]
        mov    dword ptr [ecx + 0x4e], edx
loc_1_43C2:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0x62]
        push   eax
        push   ecx
        mov    edi, 1
        call   sub_1_8F68
        lea    ecx, [esi + 0x22]
        push   ecx
        call   sub_1_8F74
        mov    ebx, dword ptr [esi + 0x14a]
        mov    edx, ebx
        and    dl, 7
        mov    cl, dl
        shl    edi, cl
        mov    ecx, 7
        cmp    ecx, ebx
        sbb    edx, edx
        xor    ecx, ecx
        and    edx, 0x80
        add    edx, 0x21
        in     al, dx
        mov    cl, al
        jmp    loc_1_4405
loc_1_4405:
        jmp    loc_1_4407
loc_1_4407:
        mov    eax, edi
        or     al, cl
        mov    ecx, dword ptr [esi + 0x5e]
        out    dx, al
        push   ecx
        call   sub_2_0120
loc_1_4415:
        push   esi
        call   sub_1_7638
        mov    ecx, dword ptr [esi + 0x3a]
        push   ecx
        call   sub_1_9064
        mov    ecx, dword ptr [esi + 0x3a]
        push   ecx
        call   sub_1_8EB8
        push   0
        push   0x5d4
        push   esi
        call   sub_1_8F8C
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 055h, 033h, 0D2h, 08Bh, 0ECh, 08Bh, 044h, 024h, 008h, 08Dh, 088h
        db  0CBh, 002h, 000h, 000h, 088h, 050h, 05Ah, 083h, 0C0h, 05Ah, 089h, 011h, 089h, 051h, 004h, 089h
        db  051h, 008h, 089h, 051h, 00Ch, 088h, 090h, 08Ah, 002h, 000h, 000h, 0C6h, 080h, 08Bh, 002h, 000h
        db  000h, 040h, 0C6h, 080h, 08Ch, 002h, 000h, 000h, 05Bh, 0C6h, 080h, 08Dh, 002h, 000h, 000h, 06Fh
        db  0C6h, 080h, 08Eh, 002h, 000h, 000h, 080h, 0C6h, 080h, 08Fh, 002h, 000h, 000h, 08Fh, 0C6h, 080h
        db  090h, 002h, 000h, 000h, 09Dh, 0C6h, 080h, 091h, 002h, 000h, 000h, 0A9h, 0C6h, 080h, 092h, 002h
        db  000h, 000h, 0B5h, 0C6h, 080h, 093h, 002h, 000h, 000h, 0C0h, 0C6h, 080h, 094h, 002h, 000h, 000h
        db  0CAh, 0C6h, 080h, 095h, 002h, 000h, 000h, 0D4h, 0C6h, 080h, 096h, 002h, 000h, 000h, 0DEh, 0C6h
        db  080h, 097h, 002h, 000h, 000h, 0E7h, 0C6h, 080h, 098h, 002h, 000h, 000h, 0EFh, 0C6h, 080h, 099h
        db  002h, 000h, 000h, 0F8h, 033h, 0C0h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h
        db  024h, 000h, 000h, 000h, 000h
sub_1_44E8:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0xb6], cl
        mov    byte ptr [esi + 0xb7], cl
        mov    byte ptr [esi + 0xb8], cl
        mov    byte ptr [esi + 0xb9], cl
        mov    byte ptr [esi + 0xbb], cl
        mov    dword ptr [esi + 0xc6], 0x3c
        mov    dword ptr [esi + 0xce], 0x79fe
        mov    dword ptr [esi + 0xe6], ecx
        mov    dword ptr [esi + 0xe2], 0x8800
        mov    dword ptr [esi + 0xf6], ecx
        mov    dword ptr [esi + 0xfa], ecx
        mov    dword ptr [esi + 0xfe], ecx
        add    esi, 0xb6
        mov    word ptr [esi + 0x4c], 0x8000
        mov    edi, 4
        mov    dword ptr [esi + 0x80], ecx
        mov    dword ptr [esi + 0x84], ecx
        mov    eax, 1
        mov    dword ptr [esi + 0xb8], ecx
        mov    edx, 0x20
        mov    dword ptr [esi + 0x98], ecx
        mov    dword ptr [esi + 0x9c], 0x5ea
        mov    dword ptr [esi + 0xa0], 0x5ea
        mov    dword ptr [esi + 0xb0], edi
        mov    dword ptr [esi + 0xb4], edi
        mov    word ptr [esi + 0x150], ax
        mov    word ptr [esi + 0x152], dx
        mov    dword ptr [esi + 0x154], 0x400
        mov    dword ptr [esi + 0x158], edx
        mov    dword ptr [esi + 0x15c], edx
        mov    dword ptr [esi + 0x160], 0x1388
        mov    dword ptr [esi + 0x164], 0x1d4c
        mov    byte ptr [esi + 0x88], al
        mov    eax, 0xff
        mov    edx, 0x10
        mov    ebx, 0xface
        mov    dword ptr [esi + 0xa4], 0x5f5e100
        mov    dword ptr [esi + 0x110], 0x1a
        mov    dword ptr [esi + 0x50], eax
        mov    dword ptr [esi + 0x54], eax
        lea    eax, [esi + 0x1a8]
loc_1_4607:
        mov    word ptr [eax], 0x9804
        mov    word ptr [eax + 4], dx
        mov    word ptr [eax + 6], cx
        mov    word ptr [eax + 8], cx
        mov    word ptr [eax + 0xa], cx
        mov    word ptr [eax + 0xc], bx
        mov    word ptr [eax + 0xe], bx
        mov    word ptr [eax - 0x40], 0x9804
        mov    word ptr [eax - 0x3c], dx
        mov    word ptr [eax - 0x3a], cx
        mov    word ptr [eax - 0x38], cx
        mov    word ptr [eax - 0x36], cx
        mov    word ptr [eax - 0x34], bx
        mov    word ptr [eax - 0x32], bx
        add    eax, edx
        dec    edi
        jne    loc_1_4607
        mov    dword ptr [esi + 0x2c1], 0    ; -> loc_1_31F8
        xor    ebx, ebx
        mov    dword ptr [esi + 0x2c5], ebx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    dword ptr [esi + 0x2b9], ebx
        mov    dword ptr [esi + 0x2bd], ebx
        mov    dword ptr [esi + 0x2d5], 0    ; -> loc_1_3488
        mov    dword ptr [esi + 0x2d9], ebx
        mov    dword ptr [esi + 0x2cd], ebx
        mov    dword ptr [esi + 0x2d1], ebx
        call   sub_1_0288
        mov    eax, 0x7fff
        mov    dword ptr [esi + 0x114], eax
        mov    dword ptr [esi + 0x118], eax
        mov    dword ptr [esi + 0x11c], eax
        mov    dword ptr [esi + 0x120], eax
        mov    dword ptr [esi + 0x124], eax
        mov    dword ptr [esi + 0x128], eax
        mov    dword ptr [esi + 0x12c], eax
        mov    dword ptr [esi + 0x130], eax
        mov    dword ptr [esi + 0x134], eax
        mov    dword ptr [esi + 0x138], eax
        mov    dword ptr [esi + 0x13c], eax
        mov    dword ptr [esi + 0x140], eax
        mov    dword ptr [esi + 0x144], eax
        mov    dword ptr [esi + 0x148], eax
        mov    ecx, dword ptr [esi + 0xa0]
        mov    dword ptr [esi + 0x238], ecx
        mov    ecx, dword ptr [esi + 0xa0]
        mov    dword ptr [esi + 0x234], ecx
        mov    byte ptr [esi + 0x512], bl
        xor    eax, eax
        mov    word ptr [esi + 0x513], bx
        pop    edi
        mov    word ptr [esi + 0x515], bx
        mov    word ptr [esi + 0x517], bx
        mov    byte ptr [esi + 0x519], bl
        mov    word ptr [esi + 0x51a], bx
        mov    byte ptr [esi + 0x2b4], 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4738:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x172]
        lea    edi, [esi + 0xb6]
        add    eax, 0xe
        mov    dword ptr [edi + 0xc0], eax
        mov    eax, dword ptr [edi + 0xbc]
        add    eax, 0xe
        mov    dword ptr [edi + 0xc4], eax
        mov    eax, dword ptr [edi + 0xbc]
        add    eax, 0x1b
        mov    dword ptr [edi + 0xc8], eax
        mov    eax, dword ptr [edi + 0xbc]
        add    eax, 0x1a
        mov    dword ptr [edi + 0xcc], eax
        mov    eax, dword ptr [edi + 0xbc]
        add    eax, 0x1c
        mov    dword ptr [edi + 0xd0], eax
        mov    eax, dword ptr [edi + 0xbc]
        add    eax, 0x18
        mov    dword ptr [edi + 0xd4], eax
        mov    eax, dword ptr [edi + 0xbc]
        add    eax, 0xa
        mov    dword ptr [edi + 0xd8], eax
        mov    eax, dword ptr [edi + 0xbc]
        add    eax, 0x10
        mov    dword ptr [edi + 0xdc], eax
        mov    eax, 0x807
        mov    ebx, dword ptr [esi + 0x176]
        mov    ecx, dword ptr [esi + 0x172]
        mov    edx, ebx
        out    dx, ax
        lea    edx, [ecx + 0xe]
        in     ax, dx
        and    ah, 0xe0
        cmp    ah, 0xe0
        je     loc_1_47ED
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_47ED:
        lea    edx, [ecx + 6]
        mov    eax, 0x5555
        out    dx, ax
        in     ax, dx
        cmp    ax, 0x1555
        je     loc_1_480B
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_480B:
        mov    eax, 0xaaaa
        out    dx, ax
        in     ax, dx
        cmp    ax, 0xaaa
        je     loc_1_4826
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_4826:
        xor    eax, eax
        out    dx, ax
        mov    edx, ebx
        mov    eax, 0x7bf
        out    dx, ax
        mov    edx, dword ptr [esi + 0x17a]
loc_1_4839:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4839
        mov    edx, ebx
        mov    eax, 0x800
        out    dx, ax
        mov    eax, dword ptr [esi + 0x142]
        cmp    eax, 1
        je     loc_1_4859
        cmp    eax, 3
        jne    loc_1_4881
loc_1_4859:
        lea    edx, [ecx + 8]
        in     ax, dx
        shr    ax, 0xc
        mov    ecx, eax
        and    ecx, 0xffff
        mov    dword ptr [edi + 0x94], ecx
        mov    dword ptr [edi + 0x90], 0
        mov    byte ptr [edi + 0x88], 1
loc_1_4881:
        push   0xd
        push   esi
        call   sub_1_7698
        cmp    dword ptr [edi + 0x1c], 0
        jne    loc_1_48AD
        mov    cx, ax
        and    cx, 0x3f00
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0xffff
        lea    ebx, [edx + edx*4 + 5]
        lea    ecx, [ebx + ebx*4]
        mov    dword ptr [edi + 0x1c], ecx
loc_1_48AD:
        test   ah, 0x40
        je     loc_1_48B6
        mov    byte ptr [edi + 3], 1
loc_1_48B6:
        test   ah, 0x80
        je     loc_1_48BF
        mov    byte ptr [edi + 4], 1
loc_1_48BF:
        and    eax, 0x30
        push   0xf
        push   esi
        mov    dword ptr [edi + 0x10c], eax
        call   sub_1_7698
        mov    byte ptr [edi + 0x2b4], 0
        test   al, 0x20
        jne    loc_1_48E2
        mov    byte ptr [edi + 0x2b4], 1
loc_1_48E2:
        push   0xa
        lea    ebx, [esi + 0x44]
        push   esi
        call   sub_1_7698
        push   0xb
        mov    byte ptr [ebx], ah
        push   esi
        mov    byte ptr [esi + 0x45], al
        call   sub_1_7698
        push   0xc
        mov    byte ptr [esi + 0x46], ah
        push   esi
        mov    byte ptr [esi + 0x47], al
        call   sub_1_7698
        lea    ecx, [esi + 0x3e]
        mov    byte ptr [esi + 0x48], ah
        mov    byte ptr [esi + 0x49], al
        mov    al, byte ptr [esi + 0x3f]
        or     al, byte ptr [esi + 0x40]
        or     al, byte ptr [esi + 0x41]
        or     al, byte ptr [esi + 0x42]
        or     al, byte ptr [esi + 0x43]
        or     al, byte ptr [ecx]
        jne    loc_1_4930
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    dx, word ptr [ebx + 4]
        mov    word ptr [ecx + 4], dx
loc_1_4930:
        mov    ebx, dword ptr [esi + 0x176]
        mov    eax, 0x5800
        mov    edx, ebx
        mov    ecx, dword ptr [esi + 0x17a]
        out    dx, ax
        mov    edx, ecx
loc_1_4947:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4947
        mov    edx, ebx
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, ecx
loc_1_4959:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4959
        mov    edx, ebx
        mov    eax, 0x69ff
        out    dx, ax
        mov    eax, 0x806
        mov    ecx, dword ptr [esi + 0x172]
        out    dx, ax
        mov    eax, 0xb000
        out    dx, ax
        mov    edx, ecx
        in     al, dx
        lea    edx, [ecx + 4]
        in     al, dx
        lea    edx, [ecx + 8]
        in     al, dx
        lea    edx, [ecx + 1]
        in     al, dx
        lea    edx, [ecx + 2]
        in     al, dx
        lea    edx, [ecx + 3]
        in     al, dx
        lea    edx, [ecx + 5]
        in     al, dx
        lea    edx, [ecx + 6]
        in     al, dx
        lea    edx, [ecx + 7]
        in     al, dx
        lea    edx, [ecx + 0xc]
        in     ax, dx
        lea    edx, [ecx + 0xa]
        in     ax, dx
        mov    edx, dword ptr [esi + 0x186]
        in     ax, dx
        mov    edx, eax
        and    edx, 0xffff
        cmp    ax, 0xbd4
        mov    dword ptr [edi + 0x3c], edx
        ja     loc_1_49C7
        mov    ax, 0xbd4
loc_1_49C7:
        and    eax, 0xffff
        mov    dword ptr [edi + 0xa8], eax
        mov    eax, 0x803
        mov    edx, dword ptr [esi + 0x176]
        out    dx, ax
        mov    edx, dword ptr [esi + 0x18e]
        in     ax, dx
        and    eax, 0xffff
        mov    ecx, eax
        mov    dword ptr [edi + 0x38], eax
        and    ecx, 0xfffffffc
        cmp    ecx, 0xbd4
        jg     loc_1_4A01
        mov    ecx, 0xbd4
loc_1_4A01:
        add    eax, 3
        xor    ebx, ebx
        and    eax, 0xfffffffc
        mov    dword ptr [edi + 0xac], ecx
        add    eax, 6
        mov    dword ptr [edi + 0x29c], eax
        mov    dword ptr [edi + 0x2a8], ebx
        mov    dword ptr [edi + 0x2ac], ebx
        mov    ax, word ptr [edi + 0x18]
        mov    edx, dword ptr [esi + 0x176]
        out    dx, ax
        mov    eax, 0x71fe
        xor    ecx, ecx
        out    dx, ax
        mov    eax, 0x802
        out    dx, ax
loc_1_4A40:
        mov    edx, dword ptr [esi + 0x172]
        mov    eax, ebx
        inc    bx
        add    edx, ecx
        and    eax, 0xffff
        inc    ecx
        cmp    bx, 6
        mov    al, byte ptr [eax + esi + 0x3e]
        out    dx, al
        jb     loc_1_4A40
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0xa800
        out    dx, ax
        mov    eax, 0x5800
        mov    ecx, dword ptr [esi + 0x17a]
        out    dx, ax
        mov    edx, ecx
loc_1_4A79:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4A79
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, ecx
loc_1_4A8F:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4A8F
        mov    ax, word ptr [edi + 0x2c]
        mov    edx, dword ptr [esi + 0x176]
        out    dx, ax
        mov    eax, 0x807
        pop    edi
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4AB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_5478
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x4800
        out    dx, ax
        mov    eax, 0x2000
        test   edi, edi
        out    dx, ax
        jne    loc_1_4AF0
        push   esi
        call   sub_1_4C38
loc_1_4AF0:
        push   0x30d40
        call   sub_1_8F80
        cmp    edi, 4
        je     loc_1_4B04
        cmp    edi, 5
        jne    loc_1_4B11
loc_1_4B04:
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0xa800
        out    dx, ax
loc_1_4B11:
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x804
        out    dx, ax
        cmp    edi, 4
        je     loc_1_4B28
        cmp    edi, 5
        jne    loc_1_4B32
loc_1_4B28:
        mov    edx, dword ptr [esi + 0x172]
        add    edx, 0xc
        in     al, dx
loc_1_4B32:
        mov    edx, dword ptr [esi + 0x172]
        xor    ecx, ecx
        add    edx, 0xa
        in     ax, dx
        test   ah, 8
        je     loc_1_4B97
        cmp    edi, 4
        je     loc_1_4B55
        cmp    edi, 5
        je     loc_1_4B55
        mov    ecx, 1
        jmp    loc_1_4B97
loc_1_4B55:
        push   0xc8
        xor    ebx, ebx
        call   sub_1_8F80
        mov    ecx, 1
        mov    eax, dword ptr [esi + 0x172]
        add    eax, 0xc
        mov    dword ptr [ebp - 4], eax
loc_1_4B72:
        mov    edx, dword ptr [ebp - 4]
        in     al, dx
        test   al, al
        jne    loc_1_4B95
        mov    edx, dword ptr [esi + 0x172]
        add    edx, 0xa
        in     ax, dx
        test   ah, 8
        je     loc_1_4B95
        inc    ebx
        cmp    ebx, 0x3e8
        jb     loc_1_4B72
        jmp    loc_1_4B97
loc_1_4B95:
        xor    ecx, ecx
loc_1_4B97:
        mov    ebx, dword ptr [esi + 0x176]
        mov    eax, 0x5800
        mov    edx, ebx
        mov    esi, dword ptr [esi + 0x17a]
        out    dx, ax
        mov    edx, esi
loc_1_4BAE:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4BAE
        mov    edx, ebx
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, esi
loc_1_4BC0:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4BC0
        mov    edx, ebx
        mov    eax, 0x807
        out    dx, ax
        cmp    edi, 4
        je     loc_1_4BDA
        cmp    edi, 5
        jne    loc_1_4BE3
loc_1_4BDA:
        mov    edx, ebx
        mov    eax, 0xb000
        out    dx, ax
loc_1_4BE3:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4BF8:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        xor    edi, edi
        push   ecx
        push   esi
        call   sub_1_5478
loc_1_4C14:
        push   esi
        call   sub_1_4C38
        test   al, al
        jne    loc_1_4C2E
        inc    edi
        cmp    edi, 3
        jb     loc_1_4C14
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4C2E:
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_4C38:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x192]
        mov    ecx, dword ptr [edi + 0x176]
        mov    edx, ecx
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0x803
        out    dx, ax
        mov    edx, dword ptr [edi + 0x172]
        add    edx, 6
        in     ax, dx
        or     al, 0x20
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x4800
        out    dx, ax
        mov    eax, 0x2000
        lea    ecx, [edi + 0x3e]
        out    dx, ax
        mov    eax, 0x8001
        mov    dword ptr [ebp - 8], ecx
        out    dx, ax
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 0x800e
        out    dx, eax
        push   esi
        mov    esi, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 6
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        push   esi
        mov    esi, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 6
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        out    dx, eax
        mov    esi, 0x3e8
loc_1_4CD4:
        push   esi
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 0x17e]
        in     al, dx
        test   al, 0x80
        jne    loc_1_4CF3
        add    esi, 0x3e8
        cmp    esi, 0x59d8
        jb     loc_1_4CD4
loc_1_4CF3:
        mov    edx, dword ptr [edi + 0x176]
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, dword ptr [edi + 0x17a]
loc_1_4D06:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4D06
        xor    esi, esi
loc_1_4D0F:
        mov    edx, dword ptr [edi + 0x18a]
        in     ax, dx
        test   ah, 0x80
        je     loc_1_4D29
        push   0x64
        inc    esi
        call   sub_1_8F80
        cmp    esi, 0xf
        jb     loc_1_4D0F
loc_1_4D29:
        mov    edx, dword ptr [edi + 0x18a]
        in     ax, dx
        test   ah, 0xc0
        jne    loc_1_4D38
        mov    bl, 1
loc_1_4D38:
        mov    ecx, dword ptr [edi + 0x176]
        mov    eax, 0x2800
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [edi + 0x17a]
loc_1_4D4D:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_4D4D
        mov    edx, ecx
        mov    eax, 0x69ff
        out    dx, ax
        mov    edx, dword ptr [edi + 0x172]
        add    edx, 6
        in     ax, dx
        and    ax, 0xffdf
        pop    edi
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x807
        out    dx, ax
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4D88:
        push   ebp
        mov    eax, 0x804
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x20
        mov    edx, dword ptr [esi + 0x176]
        out    dx, ax
        mov    edx, dword ptr [esi + 0x172]
        xor    eax, eax
        add    edx, 8
        out    dx, ax
loc_1_4DAF:
        push   1
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 0x172]
        mov    eax, 1
        add    edx, 8
        push   eax
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 0x172]
        xor    eax, eax
        add    edx, 8
        dec    edi
        out    dx, ax
        jne    loc_1_4DAF
        mov    edx, dword ptr [esi + 0x172]
        add    edx, 8
        in     ax, dx
        and    al, 2
        mov    edx, dword ptr [esi + 0x176]
        shr    al, 1
        pop    edi
        mov    cl, al
        pop    esi
        mov    eax, 0x801
        pop    ebp
        out    dx, ax
        mov    al, cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4E08:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    bx, 4
        push   esi
        push   edi
        mov    eax, 4
        mov    edi, dword ptr [ebp + 8]
        mov    esi, 0x20
        mov    edx, dword ptr [edi + 0x172]
        add    edx, 8
        out    dx, ax
loc_1_4E2A:
        or     bl, 2
        mov    edx, dword ptr [edi + 0x172]
        mov    ax, bx
        add    edx, 8
        or     bl, 1
        push   1
        out    dx, ax
        mov    ax, bx
        out    dx, ax
        mov    bx, 4
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 0x172]
        mov    eax, 4
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    esi
        jne    loc_1_4E2A
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4E78:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, 0x804
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    edx, dword ptr [esi + 0x176]
        out    dx, ax
        call   sub_1_4E08
        mov    ax, word ptr [ebp + 0xc]
        and    ax, 0x1f
        lea    edi, [ebp - 8]
        shl    ax, 2
        or     ax, 0x5002
        mov    cx, word ptr [ebp + 0x10]
        mov    bx, 4
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], cx
loc_1_4EBA:
        mov    dword ptr [ebp - 4], 0x10
loc_1_4EC1:
        mov    ax, word ptr [edi]
        test   ah, 0x80
        je     loc_1_4ECE
        or     bl, 2
        jmp    loc_1_4ED3
loc_1_4ECE:
        and    bx, 0xfffd
loc_1_4ED3:
        add    ax, ax
        mov    edx, dword ptr [esi + 0x172]
        mov    word ptr [edi], ax
        add    edx, 8
        mov    ax, bx
        push   1
        out    dx, ax
        or     bl, 1
        mov    ax, bx
        out    dx, ax
        mov    bx, 4
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 0x172]
        mov    eax, 4
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    dword ptr [ebp - 4]
        jne    loc_1_4EC1
        add    edi, 2
        lea    eax, [ebp - 4]
        cmp    edi, eax
        jb     loc_1_4EBA
        mov    edx, dword ptr [esi + 0x172]
        xor    eax, eax
        add    edx, 8
        mov    edi, 2
        out    dx, ax
loc_1_4F32:
        mov    edx, dword ptr [esi + 0x172]
        mov    eax, 1
        add    edx, 8
        push   eax
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 0x172]
        xor    eax, eax
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    edi
        jne    loc_1_4F32
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x801
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
sub_1_4F78:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, 0x804
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        mov    edx, dword ptr [edi + 0x176]
        out    dx, ax
        mov    si, 4
        call   sub_1_4E08
        mov    ax, word ptr [ebp + 0xc]
        and    ax, 0x1f
        mov    ah, 0x18
        mov    dword ptr [ebp - 4], 0xe
        lea    ebx, [eax*4]
loc_1_4FB3:
        test   bh, 0x80
        je     loc_1_4FBE
        or     si, 2
        jmp    loc_1_4FC3
loc_1_4FBE:
        and    si, 0xfffd
loc_1_4FC3:
        add    bx, bx
        mov    edx, dword ptr [edi + 0x172]
        mov    ax, si
        add    edx, 8
        or     si, 1
        push   1
        out    dx, ax
        mov    ax, si
        out    dx, ax
        mov    si, 4
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 0x172]
        mov    eax, 4
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    dword ptr [ebp - 4]
        jne    loc_1_4FB3
        mov    edx, dword ptr [edi + 0x172]
        xor    eax, eax
        add    edx, 8
        out    dx, ax
        mov    eax, 1
        out    dx, ax
        push   eax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 0x172]
        xor    eax, eax
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 0x172]
        mov    eax, 1
        add    edx, 8
        push   eax
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 0x172]
        add    edx, 8
        in     ax, dx
        test   al, 2
        je     loc_1_506F
        mov    edx, dword ptr [edi + 0x176]
        mov    eax, 0x807
        out    dx, ax
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_506F:
        xor    eax, eax
        push   1
        out    dx, ax
        mov    esi, 0x10
        call   sub_1_8F80
        mov    ebx, dword ptr [ebp + 0x10]
loc_1_5082:
        shl    word ptr [ebx], 1
        mov    eax, 1
        mov    edx, dword ptr [edi + 0x172]
        push   eax
        add    edx, 8
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 0x172]
        add    edx, 8
        in     ax, dx
        test   al, 2
        je     loc_1_50B0
        or     byte ptr [ebx], 1
        jmp    loc_1_50B5
loc_1_50B0:
        and    word ptr [ebx], 0xfffe
loc_1_50B5:
        mov    edx, dword ptr [edi + 0x172]
        xor    eax, eax
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    esi
        jne    loc_1_5082
        mov    edx, dword ptr [edi + 0x172]
        xor    eax, eax
        add    edx, 8
        mov    esi, 2
        out    dx, ax
loc_1_50DE:
        mov    edx, dword ptr [edi + 0x172]
        mov    eax, 1
        add    edx, 8
        push   eax
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 0x172]
        xor    eax, eax
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    esi
        jne    loc_1_50DE
        mov    edx, dword ptr [edi + 0x176]
        mov    eax, 0x801
        out    dx, ax
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
sub_1_5128:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    loc_1_514B
        xor    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_514B:
        lea    eax, [ebp - 2]
        push   eax
        push   1
        push   esi
        call   sub_1_4F78
        test   al, al
        je     loc_1_516A
        test   byte ptr [ebp - 2], 4
        je     loc_1_516A
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_516A:
        xor    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_5178:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], bl
        push   esi
        call   sub_1_4D88
        test   al, al
        jne    loc_1_519E
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_519E:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    loc_1_51B9
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_51B9:
        or     byte ptr [ebp - 3], 0x80
        mov    eax, dword ptr [ebp - 4]
        xor    edi, edi
        push   eax
        push   0
        push   esi
        call   sub_1_4E78
loc_1_51CB:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4F78
        test   al, al
        je     loc_1_520C
        test   byte ptr [ebp - 3], 0x80
        je     loc_1_51F1
        push   0x30d40
        inc    edi
        call   sub_1_8F80
        cmp    edi, 0xa
        jb     loc_1_51CB
loc_1_51F1:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    loc_1_5217
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_520C:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5217:
        lea    eax, [ebp - 6]
        push   eax
        push   1
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    loc_1_5232
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5232:
        test   byte ptr [ebp - 6], 1
        je     loc_1_5274
        lea    eax, [esi + 0x19a]
        push   eax
        push   2
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    loc_1_5256
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5256:
        lea    eax, [esi + 0x19c]
        push   eax
        push   3
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    loc_1_5274
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5274:
        test   byte ptr [ebp - 5], 0xe0
        je     loc_1_527C
        mov    bl, 1
loc_1_527C:
        test   byte ptr [ebp - 5], 0x18
        je     loc_1_5286
        mov    byte ptr [ebp - 1], 1
loc_1_5286:
        test   bl, bl
        je     loc_1_541B
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_53F8
        test   byte ptr [ebp - 6], 8
        je     loc_1_5355
        test   byte ptr [ebp - 3], 0x10
        je     loc_1_52AE
        test   byte ptr [ebp - 6], 0x20
        jne    loc_1_5311
loc_1_52AE:
        or     byte ptr [ebp - 3], 0x12
        mov    eax, dword ptr [ebp - 4]
        xor    edi, edi
        push   eax
        mov    ebx, 0x20
        push   0
        push   esi
        call   sub_1_4E78
loc_1_52C5:
        lea    eax, [ebp - 6]
        push   eax
        push   1
        push   esi
        call   sub_1_4F78
        test   al, al
        je     loc_1_5306
        test   word ptr [ebp - 6], bx
        jne    loc_1_52EB
        push   0x30d40
        inc    edi
        call   sub_1_8F80
        cmp    edi, 0xa
        jb     loc_1_52C5
loc_1_52EB:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    loc_1_5311
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5306:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5311:
        test   byte ptr [ebp - 3], 0x20
        je     loc_1_5336
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 0x15a], 0x5f5e100
        mov    dword ptr [esi + 0x1c6], 0x1a
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5336:
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 0x15a], 0x989680
        mov    dword ptr [esi + 0x1c6], 0x100
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5355:
        or     byte ptr [ebp - 3], 0x20
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4E78
        push   esi
        call   sub_1_5128
        test   al, al
        je     loc_1_538E
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 0x15a], 0x5f5e100
        mov    dword ptr [esi + 0x1c6], 0x1a
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_538E:
        and    word ptr [ebp - 4], 0xdfff
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4E78
        push   esi
        call   sub_1_5128
        test   al, al
        je     loc_1_53C9
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 0x15a], 0x989680
        mov    dword ptr [esi + 0x1c6], 0x100
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_53C9:
        or     byte ptr [ebp - 3], 0x20
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4E78
        xor    al, al
        pop    edi
        mov    dword ptr [esi + 0x15a], 0x5f5e100
        mov    dword ptr [esi + 0x1c6], 0x1a
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_53F8:
        test   bl, bl
        je     loc_1_541B
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 0x15a], 0x5f5e100
        mov    dword ptr [esi + 0x1c6], 0x1a
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_541B:
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 0x15a], 0x989680
        mov    dword ptr [esi + 0x1c6], 0x100
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   esi
        call   sub_1_4F78
        and    word ptr [ebp - 2], 0xfbff
        mov    ecx, dword ptr [ebp - 2]
        push   ecx
        push   0
        push   esi
        call   sub_1_4E78
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_5478:
        push   ebp
        mov    eax, 0x803
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0x176]
        mov    ecx, dword ptr [ebp + 0x10]
        out    dx, ax
        mov    edx, dword ptr [esi + 0x172]
        mov    edi, dword ptr [ebp + 0xc]
        in     eax, dx
        mov    edx, eax
        and    eax, 0xff8fffff
        and    edx, 0x700000
        shr    edx, 0x14
        mov    dword ptr [ecx], edx
        mov    ecx, edi
        shl    ecx, 0x14
        mov    ebx, dword ptr [esi + 0x172]
        or     eax, ecx
        mov    edx, ebx
        out    dx, eax
        mov    ecx, dword ptr [esi + 0x176]
        mov    eax, 0x804
        mov    edx, ecx
        out    dx, ax
        lea    edx, [ebx + 0xa]
        in     ax, dx
        and    ax, 0xff37
        cmp    edi, 1
        jne    loc_1_54DC
        or     al, 8
loc_1_54DC:
        test   edi, edi
        je     loc_1_54EA
        cmp    edi, 4
        je     loc_1_54EA
        cmp    edi, 5
        jne    loc_1_54F7
loc_1_54EA:
        or     al, 0x40
        cmp    byte ptr [esi + 0xb9], 0
        jne    loc_1_54F7
        or     al, 0x80
loc_1_54F7:
        out    dx, ax
        mov    edx, ecx
        cmp    edi, 3
        jne    loc_1_5507
        mov    eax, 0x1000
        jmp    loc_1_550C
loc_1_5507:
        mov    eax, 0xb800
loc_1_550C:
        out    dx, ax
        push   0xbb8
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x807
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5538:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0xb8
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        add    edx, 0xb6
        push   edi
        mov    dword ptr [ebp - 0x14], eax
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [edx + 0x2e0]
        mov    dword ptr [ebp - 0xc], edx
        mov    dword ptr [ebp - 0x60], eax
        mov    eax, dword ptr [ebx + 0x192]
        mov    ecx, dword ptr [edx + 0x2e4]
        mov    dword ptr [ebp - 0x64], ecx
        mov    dword ptr [ebp - 0x30], eax
        mov    ecx, dword ptr [edx + 0x1c]
        lea    eax, [ecx + ecx*4]
        add    eax, eax
        shr    eax, 5
        cmp    eax, 0xff
        jb     loc_1_558D
        mov    eax, 0xff
loc_1_558D:
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xb4], eax
        mov    eax, 0x805
        mov    esi, dword ptr [edx + 0x176]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x172]
        lea    edx, [ecx + 0xa]
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        lea    edx, [ecx + 0xc]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    edx, esi
        mov    eax, 0x807
        mov    esi, dword ptr [ebp - 0xc]
        out    dx, ax
        mov    eax, 0x7840
        out    dx, ax
        mov    eax, 0x7040
        out    dx, ax
        xor    eax, eax
        mov    dword ptr [esi + 0x118], eax
        mov    dword ptr [ebp - 0x9c], eax
        mov    dword ptr [esi + 0x114], eax
        mov    dword ptr [ebp - 0x5c], eax
        mov    dword ptr [ebp - 0x18], 0x400
        mov    byte ptr [ebp - 2], al
loc_1_55FB:
        cmp    dword ptr [ebp - 0x18], 0x20
        jb     loc_1_571A
        mov    byte ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0xa0], 3
loc_1_5613:
        cli    
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x7040
        mov    edx, dword ptr [edx + 0x176]
        out    dx, ax
        mov    eax, 0x6841
        mov    esi, dword ptr [ebp - 0xc]
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x90], eax
        mov    eax, dword ptr [esi + 0x2e0]
        inc    eax
        mov    dword ptr [ebp - 0x94], eax
        mov    eax, dword ptr [esi + 0x2e4]
        inc    eax
        mov    dword ptr [ebp - 0x98], eax
        pushaw 
        mov    edi, dword ptr [ebp - 0x98]
        mov    esi, dword ptr [ebp - 0x94]
        mov    edx, dword ptr [ebp - 0x90]
loc_1_566C:
        test   edi, 3
        je     loc_1_567A
        movsb  byte ptr es:[edi], byte ptr [esi]
        dec    edx
        jne    loc_1_566C
        jmp    loc_1_568C
loc_1_567A:
        mov    ecx, edx
        shr    ecx, 2
        je     loc_1_568C
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        je     loc_1_568C
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_568C:
        popaw  
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0x1a
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_56AD
        inc    dword ptr [ebp - 0x5c]
        jmp    loc_1_56C7
loc_1_56AD:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x13c], ecx
        jbe    loc_1_56C7
        mov    dword ptr [esi + 0x13c], ecx
loc_1_56C7:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_56E7
        mov    dword ptr [esi + 0x140], eax
loc_1_56E7:
        dec    dword ptr [ebp - 0xa0]
        jne    loc_1_5613
        cmp    dword ptr [ebp - 0x5c], 3
        je     loc_1_56FF
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_5710
loc_1_56FF:
        shr    dword ptr [ebp - 0x18], 1
        mov    dword ptr [ebp - 0x5c], 0
        inc    dword ptr [ebp - 0x9c]
loc_1_5710:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_55FB
loc_1_571A:
        mov    cl, byte ptr [ebp - 0x9c]
        mov    esi, dword ptr [ebp - 0xc]
        xor    edi, edi
        shr    dword ptr [esi + 0x13c], cl
        mov    dword ptr [ebp - 0x18], 0x100
        xor    ecx, ecx
        mov    byte ptr [ebp - 2], cl
loc_1_5737:
        cmp    dword ptr [ebp - 0x18], 8
        jb     loc_1_580C
        mov    byte ptr [ebp - 0x14], 0
        mov    ebx, 3
loc_1_574A:
        cli    
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x7040
        mov    esi, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [edx + 0x176]
        out    dx, ax
        shr    esi, 3
        mov    eax, 0x6841
        out    dx, ax
        mov    eax, 0x6000
        test   esi, esi
        out    dx, ax
        je     loc_1_5792
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0xe
loc_1_577F:
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        in     ax, dx
        dec    esi
        jne    loc_1_577F
loc_1_5792:
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0x1a
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_57AF
        inc    edi
        jmp    loc_1_57C9
loc_1_57AF:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 0x10], eax
        mov    edx, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x134], edx
        jbe    loc_1_57C9
        mov    dword ptr [esi + 0x134], edx
loc_1_57C9:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_57E9
        mov    dword ptr [esi + 0x140], eax
loc_1_57E9:
        dec    ebx
        jne    loc_1_574A
        cmp    edi, 3
        je     loc_1_57FB
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_5802
loc_1_57FB:
        inc    ecx
        xor    edi, edi
        shr    dword ptr [ebp - 0x18], 1
loc_1_5802:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_5737
loc_1_580C:
        mov    esi, dword ptr [ebp - 0xc]
        xor    edi, edi
        shr    dword ptr [esi + 0x134], cl
        mov    dword ptr [ebp - 0x18], 0x100
        xor    ecx, ecx
        mov    byte ptr [ebp - 2], cl
loc_1_5823:
        cmp    dword ptr [ebp - 0x18], 8
        jb     loc_1_58FA
        mov    byte ptr [ebp - 0x14], 0
        mov    ebx, 3
loc_1_5836:
        cli    
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x7040
        mov    esi, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [edx + 0x176]
        out    dx, ax
        shr    esi, 3
        mov    eax, 0x6841
        out    dx, ax
        mov    eax, 0x6000
        test   esi, esi
        out    dx, ax
        je     loc_1_5880
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 6
loc_1_586B:
        xor    eax, eax
        dec    esi
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        jne    loc_1_586B
loc_1_5880:
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0x1a
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_589D
        inc    edi
        jmp    loc_1_58B7
loc_1_589D:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 0x10], eax
        mov    edx, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x138], edx
        jbe    loc_1_58B7
        mov    dword ptr [esi + 0x138], edx
loc_1_58B7:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_58D7
        mov    dword ptr [esi + 0x140], eax
loc_1_58D7:
        dec    ebx
        jne    loc_1_5836
        cmp    edi, 3
        je     loc_1_58E9
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_58F0
loc_1_58E9:
        inc    ecx
        xor    edi, edi
        shr    dword ptr [ebp - 0x18], 1
loc_1_58F0:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_5823
loc_1_58FA:
        mov    esi, dword ptr [ebp - 0xc]
        xor    eax, eax
        mov    ebx, dword ptr [ebp - 0xb8]
        mov    dword ptr [ebp - 0x5c], eax
        shr    dword ptr [esi + 0x134], cl
        mov    dword ptr [ebp - 0x18], 0x8000
        mov    byte ptr [ebp - 2], al
loc_1_5918:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_59F1
        mov    byte ptr [ebp - 0x14], 0
        mov    edi, 3
loc_1_592B:
        cli    
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x7040
        mov    edx, dword ptr [edx + 0x176]
        out    dx, ax
        mov    eax, 0x6841
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x8c], eax
        push   ecx
        mov    ecx, dword ptr [ebp - 0x8c]
loc_1_595A:
        xor    eax, eax
        dec    ecx
        jne    loc_1_595A
        pop    ecx
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0x1a
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_597F
        inc    dword ptr [ebp - 0x5c]
        jmp    loc_1_59A9
loc_1_597F:
        mov    eax, dword ptr [ebp - 0x14]
        sub    dword ptr [ebp - 0x10], eax
        jne    loc_1_598E
        mov    dword ptr [ebp - 0x10], 1
loc_1_598E:
        mov    eax, dword ptr [ebp - 0x18]
        sub    edx, edx
        div    dword ptr [ebp - 0x10]
        mov    ebx, eax
        mov    esi, dword ptr [ebp - 0xc]
        cmp    dword ptr [esi + 0x148], ebx
        jbe    loc_1_59A9
        mov    dword ptr [esi + 0x148], ebx
loc_1_59A9:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_59C9
        mov    dword ptr [esi + 0x140], eax
loc_1_59C9:
        dec    edi
        jne    loc_1_592B
        cmp    dword ptr [ebp - 0x5c], 3
        je     loc_1_59DC
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_59E7
loc_1_59DC:
        shr    dword ptr [ebp - 0x18], 1
        mov    dword ptr [ebp - 0x5c], 0
loc_1_59E7:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_5918
loc_1_59F1:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x804
        mov    esi, dword ptr [edx + 0x176]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 6
        in     ax, dx
        or     ah, 0x10
        out    dx, ax
        mov    edx, esi
        mov    eax, 0x807
        out    dx, ax
        mov    eax, 0x997a
        out    dx, ax
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x17a]
        mov    edx, ecx
loc_1_5A38:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_5A38
        mov    edx, esi
        mov    eax, 0x2000
        out    dx, ax
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, ecx
loc_1_5A51:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_5A51
        mov    edx, esi
        mov    eax, 0x4800
        mov    dword ptr [ebp - 0x18], 0x400
        out    dx, ax
        xor    eax, eax
        mov    dword ptr [ebp - 0x9c], eax
        mov    dword ptr [ebp - 0x5c], eax
        mov    dword ptr [ebp - 0x6c], eax
        mov    byte ptr [ebp - 2], al
loc_1_5A79:
        cmp    dword ptr [ebp - 0x18], 0x20
        jb     loc_1_5C42
        mov    dword ptr [ebp - 0x58], 0
loc_1_5A8A:
        cli    
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x7040
        mov    byte ptr [ebp - 0x14], 0
        mov    edx, dword ptr [edx + 0x176]
        out    dx, ax
        mov    eax, 0x6841
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 0x172]
        mov    eax, dword ptr [ebp - 0x18]
        lea    edx, [edi + 0x10]
        out    dx, eax
        push   esi
        mov    esi, dword ptr [ebp - 0x64]
        mov    edx, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        lea    edx, [edi + 0x1a]
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_5AEC
        inc    dword ptr [ebp - 0x5c]
        jmp    loc_1_5B06
loc_1_5AEC:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x11c], ecx
        jbe    loc_1_5B06
        mov    dword ptr [esi + 0x11c], ecx
loc_1_5B06:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_5B26
        mov    dword ptr [esi + 0x140], eax
loc_1_5B26:
        mov    ecx, 0x4e20
loc_1_5B2B:
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     loc_1_5B4A
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0x18
        in     ax, dx
        and    eax, 0x1fff
        cmp    eax, dword ptr [ebp - 0x18]
        jne    loc_1_5B2B
loc_1_5B4A:
        test   ecx, ecx
        je     loc_1_5D20
        cli    
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x7040
        mov    byte ptr [ebp - 0x14], 0
        mov    edx, dword ptr [edx + 0x176]
        out    dx, ax
        mov    eax, 0x6841
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        push   edi
        mov    edi, dword ptr [ebp - 0x60]
        mov    edx, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, eax
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        and    eax, 3
        mov    ecx, eax
        rep insb byte ptr es:[edi], dx
        pop    edi
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0x1a
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_5BAD
        inc    dword ptr [ebp - 0x6c]
        jmp    loc_1_5BC7
loc_1_5BAD:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x120], ecx
        jbe    loc_1_5BC7
        mov    dword ptr [esi + 0x120], ecx
loc_1_5BC7:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_5BE7
        mov    dword ptr [esi + 0x140], eax
loc_1_5BE7:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x4000
        mov    edx, dword ptr [edx + 0x176]
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x17a]
loc_1_5C00:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_5C00
        inc    dword ptr [ebp - 0x58]
        cmp    dword ptr [ebp - 0x58], 3
        jb     loc_1_5A8A
        cmp    dword ptr [ebp - 0x5c], 3
        je     loc_1_5C26
        cmp    dword ptr [ebp - 0x6c], 3
        je     loc_1_5C26
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_5C38
loc_1_5C26:
        xor    eax, eax
        inc    dword ptr [ebp - 0x9c]
        mov    dword ptr [ebp - 0x5c], eax
        mov    dword ptr [ebp - 0x6c], eax
        shr    dword ptr [ebp - 0x18], 1
loc_1_5C38:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_5A79
loc_1_5C42:
        mov    cl, byte ptr [ebp - 0x9c]
        mov    esi, dword ptr [ebp - 0xc]
        xor    eax, eax
        mov    dword ptr [ebp - 0x9c], eax
        mov    dword ptr [ebp - 0x5c], eax
        mov    dword ptr [ebp - 0x6c], eax
        mov    byte ptr [ebp - 2], al
        shl    dword ptr [esi + 0x11c], cl
        shl    dword ptr [esi + 0x120], cl
        mov    dword ptr [ebp - 0x18], 0x400
loc_1_5C6F:
        mov    esi, dword ptr [ebp - 0xc]
        cmp    dword ptr [esi + 0x1e8], 3
        je     loc_1_5ED0
        cmp    dword ptr [ebp - 0x18], 0x20
        jb     loc_1_5ED0
        mov    dword ptr [ebp - 0x58], 0
loc_1_5C90:
        cli    
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x7040
        mov    ecx, dword ptr [edx + 0x176]
        mov    byte ptr [ebp - 0x14], 0
        mov    edx, ecx
        out    dx, ax
        mov    eax, 0x6841
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 0x18]
        mov    esi, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [edx + 0x172]
        lea    edx, [edi + 0x10]
        out    dx, eax
        mov    edx, edi
        mov    eax, dword ptr [esi + 0x220]
        out    dx, eax
        mov    ax, word ptr [ebp - 0x18]
        lea    edx, [edi + 6]
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0xa001
        out    dx, ax
        mov    esi, 0x64000
        lea    edx, [edi + 0xe]
loc_1_5CEB:
        mov    dword ptr [ebp - 0x54], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 0x54]
loc_1_5CF2:
        xor    eax, eax
        dec    ecx
        jne    loc_1_5CF2
        pop    ecx
        in     ax, dx
        test   ah, 8
        je     loc_1_5D02
        dec    esi
        jne    loc_1_5CEB
loc_1_5D02:
        test   esi, esi
        je     loc_1_5E95
        lea    edx, [edi + 0x1a]
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_5D2E
        inc    dword ptr [ebp - 0x5c]
        jmp    loc_1_5D48
loc_1_5D20:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_5D2E:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x124], ecx
        jbe    loc_1_5D48
        mov    dword ptr [esi + 0x124], ecx
loc_1_5D48:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_5D68
        mov    dword ptr [esi + 0x140], eax
loc_1_5D68:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x1000
        mov    ecx, 0x4e20
        mov    edi, dword ptr [edx + 0x172]
        lea    edx, [edi + 0xc]
        out    dx, ax
loc_1_5D80:
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     loc_1_5D96
        lea    edx, [edi + 0x18]
        in     ax, dx
        and    eax, 0x1fff
        cmp    eax, dword ptr [ebp - 0x18]
        jne    loc_1_5D80
loc_1_5D96:
        test   ecx, ecx
        je     loc_1_60CB
        cli    
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x7040
        mov    ecx, dword ptr [edx + 0x176]
        mov    byte ptr [ebp - 0x14], 0
        mov    edx, ecx
        mov    esi, dword ptr [ebp - 0xc]
        out    dx, ax
        mov    eax, 0x6841
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        mov    edx, edi
        mov    eax, dword ptr [esi + 0x21c]
        out    dx, eax
        mov    ax, word ptr [ebp - 0x18]
        lea    edx, [edi + 6]
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0xa000
        out    dx, ax
        mov    esi, 0x64000
        lea    edx, [edi + 0xe]
loc_1_5DE9:
        mov    dword ptr [ebp - 0x50], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 0x50]
loc_1_5DF0:
        xor    eax, eax
        dec    ecx
        jne    loc_1_5DF0
        pop    ecx
        in     ax, dx
        test   ah, 8
        je     loc_1_5E00
        dec    esi
        jne    loc_1_5DE9
loc_1_5E00:
        test   esi, esi
        je     loc_1_5E95
        lea    edx, [edi + 0x1a]
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_5E1E
        inc    dword ptr [ebp - 0x6c]
        jmp    loc_1_5E38
loc_1_5E1E:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x128], ecx
        jbe    loc_1_5E38
        mov    dword ptr [esi + 0x128], ecx
loc_1_5E38:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_5E58
        mov    dword ptr [esi + 0x140], eax
loc_1_5E58:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x4000
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0xc
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x176]
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x17a]
loc_1_5E7F:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_5E7F
        inc    dword ptr [ebp - 0x58]
        cmp    dword ptr [ebp - 0x58], 3
        jb     loc_1_5C90
        jmp    loc_1_5EA2
loc_1_5E95:
        mov    esi, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x1e8], 3
loc_1_5EA2:
        cmp    dword ptr [ebp - 0x5c], 3
        je     loc_1_5EB4
        cmp    dword ptr [ebp - 0x6c], 3
        je     loc_1_5EB4
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_5EC6
loc_1_5EB4:
        xor    eax, eax
        inc    dword ptr [ebp - 0x9c]
        mov    dword ptr [ebp - 0x5c], eax
        mov    dword ptr [ebp - 0x6c], eax
        shr    dword ptr [ebp - 0x18], 1
loc_1_5EC6:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_5C6F
loc_1_5ED0:
        mov    cl, byte ptr [ebp - 0x9c]
        mov    esi, dword ptr [ebp - 0xc]
        xor    eax, eax
        mov    dword ptr [ebp - 0x9c], eax
        mov    dword ptr [ebp - 0x6c], eax
        shl    dword ptr [esi + 0x124], cl
        shl    dword ptr [esi + 0x128], cl
        mov    dword ptr [ebp - 0x18], 0x400
        mov    byte ptr [ebp - 2], al
loc_1_5EFA:
        mov    esi, dword ptr [ebp - 0xc]
        cmp    dword ptr [esi + 0x1e8], 3
        je     loc_1_619A
        cmp    dword ptr [ebp - 0x18], 0x20
        jb     loc_1_619A
        mov    dword ptr [ebp - 0x58], 0
loc_1_5F1B:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x172]
        mov    dword ptr [ebp - 0x24], eax
        add    eax, 0x10
        mov    edx, eax
        mov    esi, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x80], eax
        mov    eax, dword ptr [ebp - 0x18]
        out    dx, eax
        mov    edx, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [esi + 0x220]
        out    dx, eax
        lea    eax, [edx + 6]
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        mov    edx, eax
        mov    dword ptr [ebp - 0x88], eax
        mov    dword ptr [ebp - 0x84], ecx
        mov    eax, ecx
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 0x64000
        mov    eax, dword ptr [edx + 0x176]
        mov    edx, eax
        mov    dword ptr [ebp - 0x68], eax
        mov    eax, 0xa001
        out    dx, ax
        mov    eax, dword ptr [ebp - 0x24]
        add    eax, 0xe
        mov    dword ptr [ebp - 0x2c], eax
loc_1_5F81:
        mov    dword ptr [ebp - 0x4c], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 0x4c]
loc_1_5F88:
        xor    eax, eax
        dec    ecx
        jne    loc_1_5F88
        pop    ecx
        mov    edx, dword ptr [ebp - 0x2c]
        in     ax, dx
        test   ax, 0x800
        mov    di, ax
        je     loc_1_5F9F
        dec    esi
        jne    loc_1_5F81
loc_1_5F9F:
        test   esi, esi
        je     loc_1_6166
        mov    edx, dword ptr [ebp - 0x24]
        mov    eax, 0x1000
        add    edx, 0xc
        mov    ecx, 0x4e20
        out    dx, ax
loc_1_5FB9:
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     loc_1_5FD2
        mov    edx, dword ptr [ebp - 0x24]
        add    edx, 0x18
        in     ax, dx
        and    eax, 0x1fff
        cmp    eax, dword ptr [ebp - 0x18]
        jne    loc_1_5FB9
loc_1_5FD2:
        test   ecx, ecx
        je     loc_1_6380
        cli    
        mov    edx, dword ptr [ebp - 0x68]
        mov    eax, 0x7040
        mov    byte ptr [ebp - 0x14], 0
        out    dx, ax
        mov    eax, 0x6841
        mov    esi, dword ptr [ebp - 0xc]
        out    dx, ax
        mov    eax, 0x6000
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [esi + 0x21c]
        out    dx, eax
        mov    edx, dword ptr [ebp - 0x88]
        mov    eax, dword ptr [ebp - 0x84]
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x68]
        mov    eax, 0xa000
        mov    byte ptr [ebp - 1], 0
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x80]
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x34], eax
        test   eax, eax
        out    dx, eax
        je     loc_1_607F
        shr    eax, 3
        mov    dword ptr [ebp - 0x3c], eax
loc_1_6034:
        mov    eax, dword ptr [ebp - 0x3c]
        mov    dword ptr [ebp - 0x1c], eax
        push   esi
        mov    esi, dword ptr [ebp - 0x64]
        mov    edx, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd dx, dword ptr [esi]
        and    eax, 3
        mov    ecx, eax
        rep outsb dx, byte ptr [esi]
        pop    esi
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_6077
        mov    edx, dword ptr [ebp - 0x2c]
        in     ax, dx
        test   ax, 0x800
        mov    di, ax
        jne    loc_1_6077
        mov    byte ptr [ebp - 1], 1
        mov    eax, dword ptr [ebp - 0x24]
        add    eax, 0x1a
        mov    edx, eax
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
loc_1_6077:
        mov    eax, dword ptr [ebp - 0x3c]
        sub    dword ptr [ebp - 0x34], eax
        jne    loc_1_6034
loc_1_607F:
        mov    eax, dword ptr [ebp - 0x24]
        test   di, 0x800
        lea    esi, [eax + 0x1a]
        mov    edx, esi
        in     al, dx
        je     loc_1_60BC
        mov    edi, 0x64000
loc_1_6094:
        mov    dword ptr [ebp - 0x48], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 0x48]
loc_1_609B:
        xor    eax, eax
        dec    ecx
        jne    loc_1_609B
        pop    ecx
        mov    edx, dword ptr [ebp - 0x2c]
        in     ax, dx
        test   ah, 8
        je     loc_1_60AE
        dec    edi
        jne    loc_1_6094
loc_1_60AE:
        test   edi, edi
        je     loc_1_6166
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
loc_1_60BC:
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_60D9
        inc    dword ptr [ebp - 0x6c]
        jmp    loc_1_60F3
loc_1_60CB:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_60D9:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x130], ecx
        jbe    loc_1_60F3
        mov    dword ptr [esi + 0x130], ecx
loc_1_60F3:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_6113
        mov    dword ptr [esi + 0x140], eax
loc_1_6113:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x4000
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0xc
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 0x176]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x17a]
        mov    edx, ecx
loc_1_613E:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_613E
        mov    edx, esi
        mov    eax, 0x4000
        out    dx, ax
        mov    edx, ecx
loc_1_6150:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_6150
        inc    dword ptr [ebp - 0x58]
        cmp    dword ptr [ebp - 0x58], 3
        jb     loc_1_5F1B
        jmp    loc_1_6173
loc_1_6166:
        mov    esi, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x1e8], 3
loc_1_6173:
        cmp    dword ptr [ebp - 0x6c], 3
        je     loc_1_617F
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_6190
loc_1_617F:
        shr    dword ptr [ebp - 0x18], 1
        mov    dword ptr [ebp - 0x6c], 0
        inc    dword ptr [ebp - 0x9c]
loc_1_6190:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_5EFA
loc_1_619A:
        mov    cl, byte ptr [ebp - 0x9c]
        mov    esi, dword ptr [ebp - 0xc]
        xor    eax, eax
        mov    dword ptr [ebp - 0x9c], eax
        mov    dword ptr [ebp - 0x5c], eax
        shl    dword ptr [esi + 0x130], cl
        mov    dword ptr [ebp - 0x18], 0x400
        mov    byte ptr [ebp - 2], al
loc_1_61BE:
        mov    esi, dword ptr [ebp - 0xc]
        cmp    dword ptr [esi + 0x1e8], 3
        je     loc_1_6454
        cmp    dword ptr [ebp - 0x18], 0x20
        jb     loc_1_6454
        mov    dword ptr [ebp - 0x58], 0
loc_1_61DF:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x172]
        mov    dword ptr [ebp - 0x20], eax
        add    eax, 0x10
        mov    edx, eax
        mov    esi, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x7c], eax
        mov    eax, dword ptr [ebp - 0x18]
        out    dx, eax
        mov    edx, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [esi + 0x220]
        out    dx, eax
        lea    eax, [edx + 6]
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        mov    edx, eax
        mov    dword ptr [ebp - 0x78], eax
        mov    dword ptr [ebp - 0x74], ecx
        mov    eax, ecx
        mov    esi, 0x64000
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0xa001
        mov    edi, dword ptr [edx + 0x176]
        mov    edx, edi
        out    dx, ax
        mov    eax, dword ptr [ebp - 0x20]
        add    eax, 0xe
        mov    dword ptr [ebp - 0x28], eax
loc_1_623C:
        mov    dword ptr [ebp - 0x44], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 0x44]
loc_1_6243:
        xor    eax, eax
        dec    ecx
        jne    loc_1_6243
        pop    ecx
        mov    edx, dword ptr [ebp - 0x28]
        in     ax, dx
        test   ah, 8
        mov    word ptr [ebp - 4], ax
        je     loc_1_625A
        dec    esi
        jne    loc_1_623C
loc_1_625A:
        test   esi, esi
        je     loc_1_6420
        mov    edx, dword ptr [ebp - 0x20]
        mov    eax, 0x1000
        add    edx, 0xc
        mov    ecx, 0x4e20
        out    dx, ax
loc_1_6274:
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     loc_1_628D
        mov    edx, dword ptr [ebp - 0x20]
        add    edx, 0x18
        in     ax, dx
        and    eax, 0x1fff
        cmp    eax, dword ptr [ebp - 0x18]
        jne    loc_1_6274
loc_1_628D:
        test   ecx, ecx
        je     loc_1_6514
        cli    
        mov    edx, edi
        mov    eax, 0x7040
        mov    byte ptr [ebp - 0x14], 0
        out    dx, ax
        mov    eax, 0x6841
        out    dx, ax
        mov    eax, 0x6000
        mov    esi, dword ptr [ebp - 0xc]
        out    dx, ax
        mov    edx, dword ptr [ebp - 0x7c]
        mov    eax, dword ptr [ebp - 0x18]
        out    dx, eax
        mov    edx, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [esi + 0x220]
        out    dx, eax
        mov    edx, dword ptr [ebp - 0x78]
        mov    eax, dword ptr [ebp - 0x74]
        mov    byte ptr [ebp - 1], 0
        out    dx, ax
        mov    edx, edi
        mov    eax, 0xa001
        out    dx, ax
        mov    eax, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0x34], eax
        test   eax, eax
        je     loc_1_6335
        shr    eax, 3
        mov    dword ptr [ebp - 0x38], eax
loc_1_62EA:
        mov    eax, dword ptr [ebp - 0x38]
        mov    dword ptr [ebp - 0x1c], eax
        push   edi
        mov    edi, dword ptr [ebp - 0x60]
        mov    edx, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, eax
        shr    ecx, 2
        rep insd dword ptr es:[edi], dx
        and    eax, 3
        mov    ecx, eax
        rep insb byte ptr es:[edi], dx
        pop    edi
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_632D
        mov    edx, dword ptr [ebp - 0x28]
        in     ax, dx
        test   ah, 8
        mov    word ptr [ebp - 4], ax
        jne    loc_1_632D
        mov    byte ptr [ebp - 1], 1
        mov    eax, dword ptr [ebp - 0x20]
        add    eax, 0x1a
        mov    edx, eax
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
loc_1_632D:
        mov    eax, dword ptr [ebp - 0x38]
        sub    dword ptr [ebp - 0x34], eax
        jne    loc_1_62EA
loc_1_6335:
        mov    eax, dword ptr [ebp - 0x20]
        test   byte ptr [ebp - 3], 8
        lea    esi, [eax + 0x1a]
        mov    edx, esi
        in     al, dx
        je     loc_1_6371
        mov    edi, 0x64000
loc_1_6349:
        mov    dword ptr [ebp - 0x40], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 0x40]
loc_1_6350:
        xor    eax, eax
        dec    ecx
        jne    loc_1_6350
        pop    ecx
        mov    edx, dword ptr [ebp - 0x28]
        in     ax, dx
        test   ah, 8
        je     loc_1_6363
        dec    edi
        jne    loc_1_6349
loc_1_6363:
        test   edi, edi
        je     loc_1_6420
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ebp - 0x10], al
loc_1_6371:
        sti    
        cmp    dword ptr [ebp - 0x10], 0xff
        jne    loc_1_638E
        inc    dword ptr [ebp - 0x5c]
        jmp    loc_1_63A8
loc_1_6380:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_638E:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        sub    dword ptr [ebp - 0x10], eax
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x12c], ecx
        jbe    loc_1_63A8
        mov    dword ptr [esi + 0x12c], ecx
loc_1_63A8:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0xc]
        add    dword ptr [esi + 0x144], eax
        inc    dword ptr [esi + 0x14c]
        cmp    dword ptr [esi + 0x140], eax
        jbe    loc_1_63C8
        mov    dword ptr [esi + 0x140], eax
loc_1_63C8:
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x1000
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 0xc
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x4000
        mov    esi, dword ptr [edx + 0x176]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x17a]
        mov    edx, ecx
loc_1_63F8:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_63F8
        mov    edx, esi
        mov    eax, 0x4000
        out    dx, ax
        mov    edx, ecx
loc_1_640A:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_640A
        inc    dword ptr [ebp - 0x58]
        cmp    dword ptr [ebp - 0x58], 3
        jb     loc_1_61DF
        jmp    loc_1_642D
loc_1_6420:
        mov    esi, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x1e8], 3
loc_1_642D:
        cmp    dword ptr [ebp - 0x5c], 3
        je     loc_1_6439
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_644A
loc_1_6439:
        shr    dword ptr [ebp - 0x18], 1
        mov    dword ptr [ebp - 0x5c], 0
        inc    dword ptr [ebp - 0x9c]
loc_1_644A:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_61BE
loc_1_6454:
        mov    cl, byte ptr [ebp - 0x9c]
        mov    esi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 0x2800
        shl    dword ptr [esi + 0x12c], cl
        mov    edi, dword ptr [edx + 0x176]
        mov    edx, edi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x17a]
        mov    edx, ecx
loc_1_6480:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_6480
        mov    edx, edi
        mov    eax, 0x2000
        out    dx, ax
        mov    esi, dword ptr [ebp - 0xc]
        mov    ax, word ptr [esi + 0x4c]
        out    dx, ax
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, ecx
loc_1_64A2:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_64A2
        mov    edx, edi
        mov    eax, 0x4800
        out    dx, ax
        mov    eax, 0x804
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 6
        mov    esi, dword ptr [ebp - 0xc]
        in     ax, dx
        and    ax, 0xefff
        mov    ecx, dword ptr [esi + 0x11c]
        out    dx, ax
        mov    edx, edi
        mov    eax, 0x807
        out    dx, ax
        mov    eax, dword ptr [ebp - 0xb4]
        sub    edx, edx
        shl    eax, 0xa
        mov    dword ptr [ebp - 0x70], ecx
        div    ecx
        and    eax, 0xfffffffc
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [esi + 0x124]
        cmp    dword ptr [esi + 0x11c], eax
        ja     loc_1_6522
        lea    edi, [esi + 0x234]
        mov    dword ptr [edi], 0x7fff
        jmp    loc_1_65B3
loc_1_6514:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_6522:
        mov    edi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x234], 0x80
        add    edi, 0x234
        cmp    dword ptr [edx + 0x142], 0
        jne    loc_1_65B3
        mov    ebx, dword ptr [edx + 0x196]
        cmp    ebx, 0xff
        je     loc_1_65B3
        mov    esi, dword ptr [edx + 0x176]
        mov    eax, 0x804
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x172]
        add    edx, 4
        in     ax, dx
        mov    ecx, eax
        mov    edx, esi
        and    ecx, 0xffff
        mov    eax, 0x807
        mov    esi, dword ptr [ebp - 0xc]
        out    dx, ax
        mov    eax, dword ptr [esi + 0x124]
        shl    eax, 3
        mov    dword ptr [ebp - 0x70], eax
        mov    eax, 0xf8
        cmp    dword ptr [ebp - 0x70], eax
        jbe    loc_1_6599
        mov    dword ptr [ebp - 0x70], eax
loc_1_6599:
        and    cl, 0x3e
        cmp    cl, 1
        ja     loc_1_65B3
        push   1
        lea    eax, [ebp - 0x70]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   0xd
        push   ebx
        push   edx
        call   sub_1_8A78
loc_1_65B3:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x100]
        cmp    eax, 0xffff
        je     loc_1_65C5
        mov    dword ptr [edi], eax
loc_1_65C5:
        cmp    dword ptr [edi], 0x20
        jae    loc_1_65D0
        mov    dword ptr [edi], 0x20
loc_1_65D0:
        xor    ecx, ecx
        mov    edi, dword ptr [ebp - 0xc]
        add    edi, 0x16c
loc_1_65DB:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0xf8]
        cmp    eax, 0xffff
        je     loc_1_65EE
        mov    word ptr [edi], ax
loc_1_65EE:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x11c]
        mov    dword ptr [ebp - 0xb0], eax
        mov    eax, dword ptr [esi + 0x110]
        cmp    dword ptr [ebp - 0xb0], eax
        jbe    loc_1_6691
        mov    ebx, ecx
        shl    ebx, 9
        lea    esi, [ebx - 0x100]
        add    ebx, 0x100
        test   esi, esi
        jge    loc_1_6626
        xor    esi, esi
loc_1_6626:
        sub    edx, edx
        mov    dword ptr [ebp - 0xac], esi
        imul   eax, esi
        div    dword ptr [ebp - 0xb0]
        sub    dword ptr [ebp - 0xac], eax
        xor    eax, eax
        mov    edx, dword ptr [ebp - 0xac]
        mov    dword ptr [ebp - 0x70], edx
        mov    ax, word ptr [edi]
        cmp    eax, edx
        ja     loc_1_6651
        mov    eax, edx
loc_1_6651:
        mov    word ptr [edi], ax
        lea    eax, [esi + ebx]
        cdq    
        sub    eax, edx
        mov    esi, dword ptr [ebp - 0xc]
        sar    eax, 1
        sub    edx, edx
        mov    dword ptr [ebp - 0x70], eax
        mov    eax, dword ptr [esi + 0x110]
        imul   eax, dword ptr [ebp - 0x70]
        div    dword ptr [esi + 0x11c]
        sub    dword ptr [ebp - 0x70], eax
        mov    eax, dword ptr [ebp - 0x70]
        shr    eax, 2
        add    dword ptr [ebp - 0x70], eax
        xor    eax, eax
        mov    ax, word ptr [edi]
        mov    edx, dword ptr [ebp - 0x70]
        cmp    eax, edx
        ja     loc_1_669E
        mov    eax, edx
        jmp    loc_1_669E
loc_1_6691:
        mov    ax, word ptr [edi]
        cmp    ax, 0x20
        ja     loc_1_669E
        mov    ax, 0x20
loc_1_669E:
        mov    word ptr [edi - 2], ax
        mov    ax, word ptr [edi - 2]
        mov    dx, word ptr [edi]
        cmp    ax, dx
        ja     loc_1_66B1
        mov    ax, dx
loc_1_66B1:
        mov    word ptr [edi - 2], ax
        add    edi, 0x10
        shr    ax, 2
        inc    ecx
        sub    ax, 0x6800
        cmp    ecx, 4
        mov    word ptr [edi - 0x14], ax
        jb     loc_1_65DB
        mov    dword ptr [ebp - 0xa4], 0
        mov    ecx, dword ptr [ebp - 0xc]
        add    ecx, 0x1ac
loc_1_66E1:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0xf8]
        cmp    eax, 0xffff
        je     loc_1_66F4
        mov    word ptr [ecx], ax
loc_1_66F4:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x110]
        mov    edx, dword ptr [esi + 0x124]
        mov    dword ptr [ebp - 0xa8], edx
        cmp    eax, edx
        jae    loc_1_6787
        mov    edi, dword ptr [ebp - 0xa4]
        shl    edi, 9
        lea    esi, [edi - 0x100]
        add    edi, 0x100
        test   esi, esi
        jge    loc_1_6728
        xor    esi, esi
loc_1_6728:
        mov    ebx, esi
        sub    edx, edx
        imul   eax, ebx
        div    dword ptr [ebp - 0xa8]
        sub    ebx, eax
        xor    eax, eax
        mov    edx, ebx
        mov    dword ptr [ebp - 0x70], ebx
        mov    ax, word ptr [ecx]
        cmp    eax, edx
        ja     loc_1_6747
        mov    eax, edx
loc_1_6747:
        mov    word ptr [ecx], ax
        lea    eax, [esi + edi]
        cdq    
        sub    eax, edx
        mov    esi, dword ptr [ebp - 0xc]
        sar    eax, 1
        sub    edx, edx
        mov    dword ptr [ebp - 0x70], eax
        mov    eax, dword ptr [esi + 0x110]
        imul   eax, dword ptr [ebp - 0x70]
        div    dword ptr [esi + 0x124]
        sub    dword ptr [ebp - 0x70], eax
        mov    eax, dword ptr [ebp - 0x70]
        shr    eax, 2
        add    dword ptr [ebp - 0x70], eax
        xor    eax, eax
        mov    ax, word ptr [ecx]
        mov    edx, dword ptr [ebp - 0x70]
        cmp    eax, edx
        ja     loc_1_679B
        mov    eax, edx
        jmp    loc_1_679B
loc_1_6787:
        mov    dword ptr [ebp - 0x70], 0x20
        mov    ax, word ptr [ecx]
        cmp    ax, 0x20
        ja     loc_1_679B
        mov    ax, 0x20
loc_1_679B:
        mov    word ptr [ecx - 2], ax
        mov    ax, word ptr [ecx - 2]
        mov    dx, word ptr [ecx]
        cmp    ax, dx
        ja     loc_1_67AE
        mov    ax, dx
loc_1_67AE:
        mov    word ptr [ecx - 2], ax
        add    ecx, 0x10
        shr    ax, 2
        inc    dword ptr [ebp - 0xa4]
        sub    ax, 0x6800
        cmp    dword ptr [ebp - 0xa4], 4
        mov    word ptr [ecx - 0x14], ax
        jb     loc_1_66E1
        mov    esi, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [esi + 0x128]
        mov    edi, dword ptr [esi + 0x120]
        cmp    edx, edi
        jb     loc_1_67F5
        lea    ecx, [esi + 0x238]
        mov    dword ptr [ecx], 0x7fff
        jmp    loc_1_6815
loc_1_67F5:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [ebp - 0xc]
        add    ecx, 0x238
        sub    edi, edx
        mov    eax, dword ptr [esi + 0x138]
        sub    edx, edx
        lea    eax, [eax + eax*2]
        shl    eax, 2
        div    edi
        mov    dword ptr [ecx], eax
loc_1_6815:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x120]
        mov    dword ptr [ebp - 0x70], eax
        cmp    dword ptr [esi + 0xa4], 0x5f5e100
        jne    loc_1_684B
        mov    eax, dword ptr [esi + 0x120]
        lea    eax, [eax + eax*4]
        add    eax, eax
        shr    eax, 2
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [esi + 0x128]
        lea    eax, [eax + eax*4]
        add    eax, eax
        jmp    loc_1_6860
loc_1_684B:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x120]
        shr    eax, 2
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [esi + 0x128]
loc_1_6860:
        shr    eax, 2
        mov    dword ptr [esi + 0x28], eax
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x104]
        cmp    eax, 0xffff
        je     loc_1_6878
        mov    dword ptr [ecx], eax
loc_1_6878:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0xfc]
        cmp    eax, 0xffff
        je     loc_1_688B
        mov    dword ptr [esi + 0x30], eax
loc_1_688B:
        mov    esi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [esi + 0x108]
        test   eax, eax
        je     loc_1_68AA
        mov    dword ptr [esi + 0x160], eax
        lea    eax, [eax + eax*2]
        shr    eax, 1
        mov    dword ptr [esi + 0x164], eax
loc_1_68AA:
        mov    ax, word ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        add    ax, 0x7800
        mov    edx, dword ptr [edx + 0x176]
        out    dx, ax
        mov    ax, word ptr [ebp - 6]
        pop    edi
        add    ax, 0x7000
        pop    esi
        out    dx, ax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_68D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, 0x7840
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 0xb6]
        mov    edx, dword ptr [edi + 0x176]
        out    dx, ax
        mov    eax, 0x6000
        xor    ebx, ebx
        mov    byte ptr [esi + 9], 0
        mov    edx, dword ptr [edi + 0x176]
        out    dx, ax
loc_1_690A:
        cmp    byte ptr [esi + 9], 0
        jne    loc_1_693D
        mov    edx, dword ptr [edi + 0x17a]
        in     ax, dx
        test   al, 1
        je     loc_1_6939
        mov    edx, dword ptr [edi + 0x182]
        in     al, dx
        cmp    al, 0xff
        je     loc_1_693D
        push   1
        inc    ebx
        call   sub_1_8F80
        cmp    ebx, 0x1388
        jb     loc_1_690A
        jmp    loc_1_693D
loc_1_6939:
        mov    byte ptr [esi + 9], 1
loc_1_693D:
        cmp    ebx, 0x320
        jae    loc_1_694A
        mov    ebx, 0x320
loc_1_694A:
        lea    eax, [ebx*4]
        mov    dword ptr [ebp - 4], 0
        lea    ecx, [eax + eax*4]
        lea    edx, [ecx + ecx*2]
        lea    ebx, [edx + edx*4]
        test   ebx, ebx
        je     loc_1_6993
loc_1_6965:
        cmp    byte ptr [esi + 9], 0
        jne    loc_1_69A7
        mov    edx, dword ptr [edi + 0x17a]
        in     ax, dx
        test   al, 1
        je     loc_1_698F
        mov    edx, dword ptr [edi + 0x182]
        push   1
        in     al, dx
        call   sub_1_8F80
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], ebx
        jb     loc_1_6965
        jmp    loc_1_6993
loc_1_698F:
        mov    byte ptr [esi + 9], 1
loc_1_6993:
        cmp    byte ptr [esi + 9], 0
        jne    loc_1_69A7
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_69A7:
        xor    bx, bx
        mov    edx, dword ptr [edi + 0x176]
        mov    eax, 0x802
        xor    ecx, ecx
        out    dx, ax
        mov    eax, dword ptr [edi + 0x172]
        mov    dword ptr [ebp - 4], eax
loc_1_69C2:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, ebx
        inc    bx
        add    edx, ecx
        and    eax, 0xffff
        inc    ecx
        cmp    bx, 6
        mov    al, byte ptr [eax + edi + 0x3e]
        out    dx, al
        jb     loc_1_69C2
        mov    ebx, 6
        mov    ecx, ebx
loc_1_69E3:
        mov    edx, dword ptr [edi + 0x172]
        xor    al, al
        add    edx, ecx
        inc    ecx
        out    dx, al
        dec    bx
        jne    loc_1_69E3
        mov    edx, dword ptr [edi + 0x176]
        mov    eax, 0x807
        out    dx, ax
        mov    eax, 0x803
        out    dx, ax
        mov    edx, dword ptr [edi + 0x172]
        add    edx, 8
        in     ax, dx
        mov    bx, ax
        mov    edx, dword ptr [edi + 0x172]
        in     eax, dx
        mov    ecx, eax
        mov    edx, dword ptr [edi + 0x176]
        mov    eax, 0x807
        cmp    dword ptr [esi + 0x54], 0xff
        out    dx, ax
        jne    loc_1_6A50
        mov    eax, ecx
        and    eax, 0x700000
        shr    eax, 0x14
        test   ecx, 0x1000000
        mov    dword ptr [esi + 0x54], eax
        mov    dword ptr [esi + 0x50], eax
        je     loc_1_6A50
        mov    byte ptr [esi + 5], 1
loc_1_6A50:
        cmp    byte ptr [esi + 5], 0
        je     loc_1_6BA5
        mov    edx, dword ptr [edi + 0x176]
        mov    eax, 0xb000
        mov    dword ptr [esi + 0x50], 0xff
        test   bl, 2
        out    dx, ax
        je     loc_1_6A9A
        push   4
        push   edi
        call   sub_1_4AB8
        test   al, al
        je     loc_1_6A9A
        mov    dword ptr [esi + 0x50], 4
        mov    dword ptr [esi + 0xa4], 0x5f5e100
        mov    dword ptr [esi + 0x110], 0x1a
loc_1_6A9A:
        cmp    dword ptr [esi + 0x50], 0xff
        jne    loc_1_6AF0
        test   bl, 4
        je     loc_1_6ACF
        push   5
        push   edi
        call   sub_1_4AB8
        test   al, al
        je     loc_1_6ACF
        mov    dword ptr [esi + 0x50], 5
        mov    dword ptr [esi + 0xa4], 0x5f5e100
        mov    dword ptr [esi + 0x110], 0x1a
loc_1_6ACF:
        cmp    dword ptr [esi + 0x50], 0xff
        jne    loc_1_6B25
        test   bl, 0x41
        je     loc_1_6AF0
        push   6
        push   edi
        call   sub_1_5178
        test   al, al
        je     loc_1_6AF0
        mov    dword ptr [esi + 0x50], 6
loc_1_6AF0:
        cmp    dword ptr [esi + 0x50], 0xff
        jne    loc_1_6B5A
        test   bl, 8
        je     loc_1_6B25
        push   0
        push   edi
        call   sub_1_4AB8
        test   al, al
        je     loc_1_6B25
        mov    dword ptr [esi + 0x50], 0
        mov    dword ptr [esi + 0xa4], 0x989680
        mov    dword ptr [esi + 0x110], 0x100
loc_1_6B25:
        cmp    dword ptr [esi + 0x50], 0xff
        jne    loc_1_6B8F
        test   bl, 0x20
        je     loc_1_6B5A
        push   1
        push   edi
        call   sub_1_4BF8
        test   al, al
        je     loc_1_6B5A
        mov    dword ptr [esi + 0x50], 1
        mov    dword ptr [esi + 0xa4], 0x989680
        mov    dword ptr [esi + 0x110], 0x100
loc_1_6B5A:
        cmp    dword ptr [esi + 0x50], 0xff
        jne    loc_1_6BB3
        test   bl, 0x10
        je     loc_1_6B8F
        push   3
        push   edi
        call   sub_1_4BF8
        test   al, al
        je     loc_1_6B8F
        mov    dword ptr [esi + 0x50], 3
        mov    dword ptr [esi + 0xa4], 0x989680
        mov    dword ptr [esi + 0x110], 0x100
loc_1_6B8F:
        cmp    dword ptr [esi + 0x50], 0xff
        jne    loc_1_6BB3
        mov    eax, dword ptr [esi + 0x54]
        mov    dword ptr [esi + 0x50], eax
        cmp    eax, 6
        jne    loc_1_6BB3
        jmp    loc_1_6BAB
loc_1_6BA5:
        cmp    dword ptr [esi + 0x50], 6
        jne    loc_1_6BB3
loc_1_6BAB:
        push   6
        push   edi
        call   sub_1_5178
loc_1_6BB3:
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [esi + 0x50]
        push   eax
        push   ecx
        push   edi
        call   sub_1_5478
        push   0x1e8480
        call   sub_1_8F80
        push   edi
        call   sub_1_5538
        test   eax, eax
        jne    loc_1_6C6D
        cmp    dword ptr [esi + 0xa4], 0x989680
        jne    loc_1_6C1F
        mov    eax, dword ptr [esi + 0x50]
        cmp    eax, 3
        je     loc_1_6C1F
        cmp    eax, 1
        je     loc_1_6C1F
        cmp    byte ptr [esi + 4], 0
        je     loc_1_6C1F
        mov    ecx, dword ptr [edi + 0x176]
        mov    eax, 0x803
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [edi + 0x172]
        add    edx, 6
        in     ax, dx
        or     al, 0x20
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0x807
        out    dx, ax
loc_1_6C1F:
        mov    ax, word ptr [esi + 0x18]
        mov    ebx, dword ptr [edi + 0x176]
        mov    edx, ebx
        out    dx, ax
        mov    eax, 0x71fe
        mov    ecx, dword ptr [edi + 0x17a]
        out    dx, ax
        mov    eax, 0xa800
        out    dx, ax
        mov    eax, 0x5800
        out    dx, ax
        mov    edx, ecx
loc_1_6C4A:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_6C4A
        mov    edx, ebx
        mov    eax, 0x2800
        out    dx, ax
        mov    edx, ecx
loc_1_6C5C:
        in     ax, dx
        test   ah, 0x10
        jne    loc_1_6C5C
        mov    ax, word ptr [esi + 0x2c]
        mov    edx, ebx
        out    dx, ax
        xor    eax, eax
loc_1_6C6D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_6C78:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x88
        mov    dword ptr [ebp - 0x84], 0     ; -> loc_1_9540
        push   ebx
        mov    eax, 6
        mov    word ptr [ebp - 0x68], ax
        push   esi
        mov    word ptr [ebp - 0x66], ax
        push   edi
        mov    dword ptr [ebp - 0x64], 0     ; -> loc_1_954C
        mov    esi, 4
        mov    ecx, 0xa
        mov    dword ptr [ebp - 0x5c], 0     ; -> loc_1_955C
        mov    word ptr [ebp - 0x60], 5
        mov    word ptr [ebp - 0x5e], 5
        mov    dword ptr [ebp - 0x54], 0     ; -> obj1:0x9590
        mov    word ptr [ebp - 0x88], si
        mov    eax, 0xb
        mov    word ptr [ebp - 0x86], si
        mov    dword ptr [ebp - 0x3c], 0     ; -> loc_1_95A8
        mov    word ptr [ebp - 0x58], cx
        mov    word ptr [ebp - 0x56], cx
        mov    dword ptr [ebp - 0x34], 0     ; -> loc_1_95C0
        mov    word ptr [ebp - 0x40], ax
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj1:0x95D8
        mov    word ptr [ebp - 0x3e], ax
        mov    word ptr [ebp - 0x38], ax
        mov    dword ptr [ebp - 0x24], 0     ; -> loc_1_95F0
        mov    word ptr [ebp - 0x36], ax
        mov    eax, 0xc
        mov    word ptr [ebp - 0x30], ax
        mov    word ptr [ebp - 0x2e], ax
        mov    dword ptr [ebp - 0x1c], 0     ; -> obj1:0x9608
        mov    word ptr [ebp - 0x28], ax
        mov    word ptr [ebp - 0x26], ax
        mov    dword ptr [ebp - 0x7c], 0     ; -> obj1:0x9618
        mov    word ptr [ebp - 0x78], 5
        mov    word ptr [ebp - 0x76], 5
        mov    dword ptr [ebp - 0x74], 0     ; -> loc_1_9624
        mov    word ptr [ebp - 0x20], cx
        mov    word ptr [ebp - 0x1e], cx
        mov    dword ptr [ebp - 0x6c], 0     ; -> loc_1_9630
        mov    word ptr [ebp - 0x80], si
        mov    word ptr [ebp - 0x7e], si
        mov    dword ptr [ebp - 0x44], 0     ; -> loc_1_9638
        mov    word ptr [ebp - 0x70], si
        mov    eax, 3
        mov    word ptr [ebp - 0x6e], si
        mov    dword ptr [ebp - 0x4c], 0     ; -> loc_1_9640
        mov    word ptr [ebp - 0x48], si
        mov    word ptr [ebp - 0x46], si
        mov    dword ptr [ebp - 0x14], 0     ; -> obj1:0x9650
        mov    word ptr [ebp - 0x50], ax
        mov    word ptr [ebp - 0x4e], ax
        mov    dword ptr [ebp - 0xc], 0      ; -> obj1:0x9668
        mov    word ptr [ebp - 0x18], cx
        mov    word ptr [ebp - 0x16], cx
        mov    word ptr [ebp - 0x10], cx
        mov    word ptr [ebp - 0xe], cx
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x19e], 0xff
        mov    dword ptr [edi + 0x1a2], 0xffffffff
        mov    dword ptr [edi + 0x1a6], 0xffffffff
        mov    dword ptr [edi + 0x1aa], 1
        mov    dword ptr [edi + 0x1ae], 0xffff
        mov    dword ptr [edi + 0x1b2], 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        push   2
        lea    eax, [ebp - 0x88]
        mov    dword ptr [edi + 0x142], ecx
        push   eax
        mov    ebx, dword ptr [ebp + 0x10]
        push   ebx
        lea    ecx, [ebp - 4]
        push   ecx
        lea    eax, [ebp - 8]
        mov    dword ptr [edi + 0x1b6], 0xffff
        mov    dword ptr [edi + 0x1ba], 0xffff
        mov    dword ptr [edi + 0x1be], 0
        push   eax
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6E88
        push   1
        lea    eax, [ebp - 0x80]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, esi
        push   eax
        call   sub_1_9016
        test   al, al
        je     loc_1_6E48
        mov    dword ptr [edi + 0x19e], 5
loc_1_6E48:
        push   1
        lea    eax, [ebp - 0x78]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_9016
        test   al, al
        je     loc_1_6E68
        mov    dword ptr [edi + 0x19e], 2
loc_1_6E68:
        push   1
        lea    eax, [ebp - 0x70]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_9016
        test   al, al
        je     loc_1_6E88
        mov    dword ptr [edi + 0x19e], 1
loc_1_6E88:
        push   1
        lea    eax, [ebp - 0x68]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6EAE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 0x1a2], ecx
loc_1_6EAE:
        push   1
        lea    eax, [ebp - 0x60]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6ED9
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0x1f
        ja     loc_1_6ED9
        mov    dword ptr [edi + 0x1a6], eax
loc_1_6ED9:
        push   2
        lea    eax, [ebp - 0x58]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        mov    dword ptr [edi + 0x1aa], 0
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6F3F
        push   1
        lea    eax, [ebp - 0x50]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_9016
        test   al, al
        je     loc_1_6F1F
        mov    dword ptr [edi + 0x1aa], 2
        jmp    loc_1_6F3F
loc_1_6F1F:
        push   1
        lea    eax, [ebp - 0x48]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_9016
        test   al, al
        je     loc_1_6F3F
        mov    dword ptr [edi + 0x1aa], 1
loc_1_6F3F:
        push   0
        lea    eax, [ebp - 0x40]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6F65
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 0x1ae], ecx
loc_1_6F65:
        push   0
        lea    eax, [ebp - 0x38]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6F8B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 0x1b2], ecx
loc_1_6F8B:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6FB1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 0x1b6], ecx
loc_1_6FB1:
        push   0
        lea    eax, [ebp - 0x28]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6FD7
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 0x1ba], ecx
loc_1_6FD7:
        push   0
        lea    eax, [ebp - 0x20]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_6FFD
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 0x1be], ecx
loc_1_6FFD:
        push   1
        lea    eax, [ebp - 0x18]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        mov    dword ptr [edi + 0x16e], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_702D
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 0x16e], ecx
loc_1_702D:
        push   1
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_900A
        mov    dword ptr [edi + 0x14a], 0
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_7085
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 3
        je     loc_1_707F
        cmp    eax, 5
        je     loc_1_707F
        cmp    eax, 7
        je     loc_1_707F
        cmp    eax, 9
        je     loc_1_707F
        cmp    eax, 0xa
        je     loc_1_707F
        cmp    eax, 0xb
        je     loc_1_707F
        cmp    eax, 0xc
        je     loc_1_707F
        cmp    eax, 0xf
        jne    loc_1_7085
loc_1_707F:
        mov    dword ptr [edi + 0x14a], eax
loc_1_7085:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7098:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x104
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x142], 4
        mov    eax, dword ptr [esi + 0x16e]
        test   eax, eax
        je     loc_1_70D1
        cmp    dword ptr [esi + 0x14a], 0
        je     loc_1_70D1
        lea    ecx, [ebp - 0x104]
        push   ecx
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        jmp    loc_1_70DE
loc_1_70D1:
        lea    eax, [ebp - 0x104]
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   ecx
loc_1_70DE:
        push   esi
        call   sub_1_7278
        cmp    eax, 1
        je     loc_1_7191
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0xf4]
        and    ecx, 0x1f
        and    eax, 0xfffffffe
        xor    edx, edx
        mov    dword ptr [esi + 0x196], ecx
        mov    dl, byte ptr [ebp - 0xc8]
        mov    dword ptr [esi + 0x16e], eax
        mov    eax, dword ptr [esi + 0x1aa]
        mov    dword ptr [esi + 0x14a], edx
        mov    dword ptr [esi + 0x142], 0
        mov    dword ptr [esi + 0x146], 2
        test   eax, eax
        jne    loc_1_7149
        test   byte ptr [ebp - 0x100], 4
        je     loc_1_716A
        mov    dword ptr [esi + 0x29e], 0
        jmp    loc_1_7174
loc_1_7149:
        cmp    eax, 1
        jne    loc_1_7161
        or     byte ptr [ebp - 0x100], 4
        mov    dword ptr [esi + 0x29e], 0
        jmp    loc_1_7174
loc_1_7161:
        and    word ptr [ebp - 0x100], 0xfffb
loc_1_716A:
        mov    dword ptr [esi + 0x29e], 3
loc_1_7174:
        push   2
        lea    eax, [ebp - 0x100]
        and    word ptr [ebp - 0x100], 0xffbf
        push   eax
        push   4
        push   ecx
        push   esi
        call   sub_1_8A78
        xor    eax, eax
loc_1_7191:
        mov    edx, dword ptr [esi + 0x142]
        lea    ecx, [edx - 3]
        cmp    ecx, 1
        lea    ecx, [edx - 1]
        sbb    edi, edi
        neg    edi
        cmp    ecx, 1
        sbb    ecx, ecx
        neg    ecx
        or     edi, ecx
        jne    loc_1_71B4
        cmp    edx, 4
        jne    loc_1_71E8
loc_1_71B4:
        mov    eax, dword ptr [esi + 0x4a]
        push   eax
        push   esi
        call   sub_1_73B8
        cmp    eax, 1
        jne    loc_1_71D0
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_71D0:
        mov    dword ptr [esi + 0x142], 1
        mov    eax, dword ptr [esi + 0x13a]
        mov    dword ptr [esi + 0x196], eax
        xor    eax, eax
loc_1_71E8:
        mov    ecx, dword ptr [esi + 0x196]
        mov    edx, dword ptr [esi + 0x16e]
        mov    word ptr [esi + 0x5aa], 0x20
        mov    word ptr [esi + 0x5a6], cx
        mov    word ptr [esi + 0x5a8], dx
        xor    edx, edx
        mov    word ptr [esi + 0x5ac], dx
        mov    dword ptr [esi + 0x5b0], edx
        mov    word ptr [esi + 0x5ae], dx
        mov    cl, byte ptr [esi + 0x14a]
        mov    word ptr [esi + 0x5b4], dx
        mov    dword ptr [esi + 0x5b6], edx
        mov    word ptr [esi + 0x5ba], dx
        mov    edi, dword ptr [esi + 0x142]
        mov    word ptr [esi + 0x5c0], di
        mov    byte ptr [esi + 0x5bc], cl
        mov    word ptr [esi + 0x5c2], dx
        pop    edi
        mov    byte ptr [esi + 0x5be], dl
        mov    byte ptr [esi + 0x5bf], dl
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7278:
        push   ebp
        mov    eax, 1
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        mov    dword ptr [ebp - 0xc], eax
        push   esi
        mov    dword ptr [ebp - 0x10], eax
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x1a6]
        cmp    ecx, -1
        jne    loc_1_72A2
        and    dword ptr [edi], 0xffffffe0
        jmp    loc_1_72AF
loc_1_72A2:
        mov    eax, dword ptr [edi]
        mov    edx, eax
        xor    edx, ecx
        and    edx, 0x1f
        xor    edx, eax
        mov    dword ptr [edi], edx
loc_1_72AF:
        mov    eax, dword ptr [edi]
        and    eax, 0x1f
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 0x20
        jae    loc_1_739D
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp + 0x14]
        lea    edx, [ecx + eax*4]
        mov    dword ptr [ebp - 8], edx
loc_1_72CF:
        mov    ecx, dword ptr [edi]
        mov    edx, dword ptr [ebp - 8]
        mov    eax, ecx
        xor    eax, dword ptr [ebp - 4]
        and    eax, 0x1f
        xor    eax, ecx
        mov    dword ptr [edi], eax
        and    eax, 0xffffff1f
        mov    dword ptr [edi], eax
        and    eax, 0xff
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [esi + 0x4a]
        cmp    byte ptr [ecx + edx + 0x12c], 0
        jne    loc_1_7352
        push   ebx
        lea    eax, [ebp - 0xc]
        push   edi
        push   esi
        push   eax
        call   sub_1_89F8
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_7352
        mov    ax, word ptr [ebx]
        cmp    ax, 0xffff
        je     loc_1_7352
        cmp    ax, 0x10b7
        jne    loc_1_7352
        cmp    byte ptr [ebx + 3], 0x59
        jne    loc_1_7352
        lea    eax, [ebx + 0xd]
        push   1
        push   eax
        push   0xd
        mov    byte ptr [eax], 0xf8
        mov    eax, dword ptr [edi]
        and    eax, 0x1f
        push   eax
        push   esi
        call   sub_1_8A78
        push   ebx
        push   edi
        push   esi
        call   sub_1_8A69
        mov    eax, dword ptr [ebx + 0x10]
        and    eax, 0xfffffffe
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_7372
        cmp    dword ptr [ebp + 0x10], eax
        je     loc_1_7372
loc_1_7352:
        add    dword ptr [ebp - 8], 0xd
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], 0x1a0
        jb     loc_1_72CF
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_7372:
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ecx + ecx*2]
        lea    edx, [ecx + eax*4]
        mov    eax, dword ptr [esi + 0x4a]
        mov    dword ptr [ebp - 0x10], 0
        mov    byte ptr [eax + edx + 0x12c], 1
        mov    ecx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [esi + 0x4a]
        and    ecx, 0xfffffffe
        mov    dword ptr [eax + edx + 0x12e], ecx
loc_1_739D:
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_73B8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0x1a6]
        test   esi, esi
        je     loc_1_7414
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    esi, -1
        je     loc_1_7417
        cmp    byte ptr [ebx + esi + 0x2cb], 0
        jne    loc_1_7417
        push   esi
        push   edi
        call   sub_1_7478
        test   al, al
        je     loc_1_7417
        mov    eax, esi
        mov    dword ptr [edi + 0x13a], esi
        mov    dword ptr [edi + 0x14a], 0
        shl    eax, 0xc
        mov    dword ptr [edi + 0x16e], eax
        xor    eax, eax
        mov    byte ptr [ebx + esi + 0x2cb], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_7414:
        mov    ebx, dword ptr [ebp + 0xc]
loc_1_7417:
        mov    esi, 1
loc_1_741C:
        cmp    byte ptr [ebx + esi + 0x2cb], 0
        jne    loc_1_7431
        push   esi
        push   edi
        call   sub_1_7478
        test   al, al
        jne    loc_1_7443
loc_1_7431:
        inc    esi
        cmp    esi, 0xf
        jbe    loc_1_741C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_7443:
        mov    eax, esi
        mov    dword ptr [edi + 0x13a], esi
        mov    dword ptr [edi + 0x14a], 0
        shl    eax, 0xc
        mov    dword ptr [edi + 0x16e], eax
        xor    eax, eax
        mov    byte ptr [ebx + esi + 0x2cb], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7478:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        shl    edx, 0xc
        mov    ebp, esp
        add    edx, 0xc80
        in     ax, dx
        cmp    ax, 0x6d50
        je     loc_1_7496
        xor    al, al
        pop    ebp
        ret    8
loc_1_7496:
        add    edx, 2
        in     ax, dx
        and    eax, 0xf0ff
        cmp    eax, 0x2059
        je     loc_1_74B4
        cmp    eax, 0x7059
        je     loc_1_74B4
        xor    al, al
        pop    ebp
        ret    8
loc_1_74B4:
        mov    al, 1
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_74C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    ecx, [esi + 0x396]
        lea    ebx, [esi + 0xb6]
        push   ecx
        mov    edx, dword ptr [esi + 0x3a]
        push   0
        push   0xc00
        push   edx
        call   sub_1_8F12
        mov    edi, dword ptr [ebx + 0x2e0]
        test   edi, edi
        jne    loc_1_750D
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_750D:
        xor    eax, eax
        mov    ecx, 0x300
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebx + 0x2e0]
        add    eax, 0x600
        mov    dword ptr [ebx + 0x2e4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 0x21c], eax
        add    eax, 0x600
        push   0
        mov    dword ptr [ebx + 0x220], eax
        mov    ecx, dword ptr [ebx + 0x2e0]
        lea    eax, [ebp - 0x10]
        push   eax
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebx + 0x2e4]
        push   2
        push   0x600
        lea    ecx, [ebp - 0x18]
        push   ecx
        mov    dword ptr [ebp - 0x14], edx
        push   esi
        call   sub_1_8928
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebx + 0x23c], ecx
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx + 0x240], edx
        lea    edx, [ebp - 4]
        mov    eax, dword ptr [ebx + 0x2e0]
        push   edx
        mov    dword ptr [ebx + 0x2b5], eax
        lea    eax, [ebx + 0x2e8]
        mov    ecx, dword ptr [ebx + 0x2e4]
        push   eax
        push   0
        mov    dword ptr [ebx + 0x2c9], ecx
        push   0xc00
        mov    ecx, dword ptr [esi + 0x3a]
        push   ecx
        call   sub_1_8F12
        cmp    ebx, 0xfffffd18
        jne    loc_1_75BF
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_75BF:
        mov    edi, dword ptr [ebx + 0x2e8]
        xor    eax, eax
        mov    ecx, 0x300
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebx + 0x2e8]
        push   1
        add    eax, 0x600
        lea    ecx, [ebx + 0x244]
        push   ecx
        mov    dword ptr [ebx + 0x2ec], eax
        mov    eax, dword ptr [ebp - 4]
        push   2
        mov    dword ptr [ebx + 0x224], eax
        push   0x600
        add    eax, 0x600
        add    ebx, 0x2e8
        push   ebx
        push   esi
        add    esi, 0x366
        mov    dword ptr [ebx - 0xc0], eax
        call   sub_1_8928
        mov    ecx, dword ptr [0]            ; -> obj1:0x9674
        push   ecx
        push   0
        push   0x1000
        push   esi
        call   sub_1_8F86
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_7638:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x2d2]
        mov    esi, dword ptr [ebx + 0x2da]
        lea    ecx, [ebx + 0x396]
        lea    edi, [ebx + 0xb6]
        cmp    dword ptr [ecx], 0
        je     loc_1_7670
        push   eax
        push   ecx
        push   0
        mov    eax, dword ptr [ebx + 0x3a]
        push   0xc00
        push   eax
        call   sub_1_8F18
loc_1_7670:
        add    edi, 0x2e8
        cmp    dword ptr [edi], 0
        je     loc_1_768D
        push   esi
        mov    eax, dword ptr [ebx + 0x3a]
        push   edi
        push   0
        push   0xc00
        push   eax
        call   sub_1_8F18
loc_1_768D:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_7698:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [eax + 0x172]
        mov    si, word ptr [ebp + 0xc]
        mov    eax, 0x800
        lea    edi, [ecx + 0xe]
        and    esi, 0xffff00ff
        add    si, 0x80
        mov    edx, edi
        out    dx, ax
        mov    ax, si
        lea    edx, [ecx + 0xa]
        out    dx, ax
loc_1_76CA:
        in     ax, dx
        test   ah, 0x80
        jne    loc_1_76CA
        lea    edx, [ecx + 0xc]
        in     ax, dx
        mov    cx, ax
        mov    edx, edi
        mov    eax, 0x807
        pop    edi
        out    dx, ax
        mov    ax, cx
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_76F8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_3A18
        mov    eax, dword ptr [esi + 4]
        add    esi, 0x62
        push   eax
        push   esi
        call   sub_1_8F62
        pop    esi
        pop    ebp
        ret    0x10
        db  090h
loc_1_7718:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        mov    eax, 0
        pop    ebp
        cmp    byte ptr [ecx + 0x83], 1
        adc    eax, -1
        and    eax, 0xc001000d
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7748:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        xor    eax, eax
        cmp    byte ptr [ecx + 0x83], al
        je     loc_1_7764
        mov    eax, 0xc001000d
        pop    ebp
        ret    0x14
loc_1_7764:
        mov    edx, dword ptr [ebp + 0xc]
        test   edx, 0xf010
        je     loc_1_7778
        mov    eax, 0xc0000001
        pop    ebp
        ret    0x14
loc_1_7778:
        test   dl, 1
        je     loc_1_7782
        mov    eax, 1
loc_1_7782:
        test   dl, 2
        je     loc_1_778A
        or     eax, 2
loc_1_778A:
        test   dl, 4
        je     loc_1_7792
        or     eax, 2
loc_1_7792:
        test   dl, 8
        je     loc_1_779A
        or     eax, 4
loc_1_779A:
        mov    byte ptr [ecx + 0x82], 0
        test   dl, 0x20
        je     loc_1_77B0
        mov    byte ptr [ecx + 0x82], 1
        or     eax, 8
loc_1_77B0:
        add    ax, 0x8000
        pop    ebp
        mov    word ptr [ecx + 0x102], ax
        mov    edx, dword ptr [ecx + 0x176]
        out    dx, ax
        xor    eax, eax
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_77D8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        push   edi
        mov    edx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0xffffffff
        lea    eax, [eax + edx*4]
loc_1_77FD:
        test   edx, edx
        je     loc_1_780C
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    loc_1_77FD
        mov    dword ptr [ecx], edx
loc_1_780C:
        cmp    dword ptr [ecx], -1
        jne    loc_1_781F
        mov    eax, 0xc0010019
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_781F:
        mov    eax, dword ptr [0]            ; -> obj1:0x9674
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   0x49
        push   ecx
        call   sub_1_8F86
        test   eax, eax
        je     loc_1_7844
        mov    eax, 0xc000009a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_7844:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0x1c]
        add    eax, 0x14
        mov    esi, dword ptr [ebp + 0x20]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x5e]
        push   eax
        push   ecx
        call   sub_2_0170
loc_1_785F:
        test   al, al
        jne    loc_1_787E
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x49
        push   eax
        call   sub_1_8F8C
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
loc_1_787E:
        cmp    byte ptr [esi + 0x83], 0
        je     loc_1_789D
        mov    eax, dword ptr [esi + 0x5a]
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, 0x103
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x5a], ecx
        jmp    loc_1_78AE
loc_1_789D:
        mov    eax, dword ptr [esi + 0x52]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0xc], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x52], ecx
loc_1_78AE:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0x4a]
        mov    dword ptr [eax + 0x10], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x2a], cl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x20], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 1
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx + 0x18], eax
        cmp    dword ptr [esi + 0x56], ecx
        jne    loc_1_7902
        mov    eax, dword ptr [esi + 4]
        lea    ecx, [esi + 0x62]
        push   eax
        push   ecx
        call   sub_1_8F62
loc_1_7902:
        push   esi
        inc    dword ptr [esi + 0x56]
        call   sub_1_7BE8
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x24
        db  08Bh, 0FFh
loc_1_7918:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x20], 0
        mov    ecx, dword ptr [esi]
        je     loc_1_7931
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    4
loc_1_7931:
        mov    byte ptr [esi + 0x20], 1
        inc    dword ptr [esi + 0x1c]
        mov    eax, dword ptr [ecx + 0x52]
        cmp    esi, eax
        jne    loc_1_7947
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ecx + 0x52], eax
        jmp    loc_1_795A
loc_1_7947:
        cmp    dword ptr [eax + 0xc], esi
        je     loc_1_7954
loc_1_794C:
        mov    eax, dword ptr [eax + 0xc]
        cmp    dword ptr [eax + 0xc], esi
        jne    loc_1_794C
loc_1_7954:
        mov    edx, dword ptr [esi + 0xc]
        mov    dword ptr [eax + 0xc], edx
loc_1_795A:
        lea    eax, [esi + 0x21]
        mov    edx, dword ptr [esi + 0x14]
        push   eax
        mov    ecx, dword ptr [ecx + 0x5e]
        push   edx
        push   ecx
        call   sub_2_0240
loc_1_796B:
        test   eax, eax
        je     loc_1_7976
        cmp    eax, 0x103
        jne    loc_1_797C
loc_1_7976:
        push   esi
        call   sub_1_79B8
loc_1_797C:
        push   esi
        call   sub_1_79B8
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7998:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_79B8
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_79B8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0x1c]
        dec    eax
        mov    dword ptr [ecx + 0x1c], eax
        jne    loc_1_79D4
        push   ecx
        call   sub_1_79E8
        xor    eax, eax
        pop    ebp
        ret    4
loc_1_79D4:
        mov    eax, 0x103
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_79E8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        push   edi
        call   sub_1_88F8
        mov    eax, dword ptr [edi + 0x56]
        mov    ecx, 1
        dec    eax
        push   edi
        mov    dword ptr [edi + 0x56], eax
        cmp    ecx, eax
        mov    al, cl
        adc    al, 0xff
        mov    byte ptr [edi + 0xc], al
        call   sub_1_7C18
        push   0
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_1_8EC4
        push   0
        push   0x49
        push   esi
        call   sub_1_8F8C
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7A38:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [eax + 0x20], 0
        mov    esi, dword ptr [eax]
        je     loc_1_7A52
        mov    eax, 0xc0010002
        pop    esi
        pop    ebp
        ret    4
loc_1_7A52:
        mov    cl, byte ptr [esi + 0x21]
        test   cl, 2
        je     loc_1_7A64
        mov    eax, 0xc001000d
        pop    esi
        pop    ebp
        ret    4
loc_1_7A64:
        or     cl, 2
        push   0x40010004
        push   esi
        mov    byte ptr [esi + 0x21], cl
        inc    dword ptr [eax + 0x1c]
        mov    dword ptr [esi + 0x6e], eax
        call   sub_1_7B68
        mov    byte ptr [esi + 0x83], 1
        push   esi
        call   sub_1_7CB8
        mov    eax, 0x103
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_7A98:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0x11]
loc_1_7AA6:
        push   edi
        call   sub_1_7B48
        test   eax, eax
        je     loc_1_7ACF
        or     byte ptr [eax + 0x2e], 0xa
        mov    dword ptr [eax + 0x24], ebx
        cmp    dword ptr [esi + 0x1d], ebx
        jne    loc_1_7AC4
        mov    dword ptr [esi + 0x1d], eax
        mov    dword ptr [esi + 0x19], eax
        jmp    loc_1_7AA6
loc_1_7AC4:
        mov    ecx, dword ptr [esi + 0x19]
        mov    dword ptr [ecx + 0x24], eax
        mov    dword ptr [esi + 0x19], eax
        jmp    loc_1_7AA6
loc_1_7ACF:
        lea    edi, [esi + 0x19]
loc_1_7AD2:
        push   edi
        call   sub_1_7B48
        test   eax, eax
        je     loc_1_7AE5
        push   eax
        push   esi
        call   sub_1_0828
        jmp    loc_1_7AD2
loc_1_7AE5:
        mov    edi, dword ptr [esi + 0x6e]
        push   0x40010005
        and    byte ptr [esi + 0x21], 0xfd
        mov    byte ptr [esi + 0x83], 0
        push   esi
        call   sub_1_7B68
        push   0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_8ED6
        push   edi
        call   sub_1_79B8
        mov    ecx, dword ptr [esi + 0x5a]
        test   ecx, ecx
        je     loc_1_7B3C
loc_1_7B16:
        mov    eax, dword ptr [esi + 0x5a]
        mov    ebx, dword ptr [esi + 0x52]
        push   0
        push   0
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x5a], edx
        mov    dword ptr [ecx + 0xc], ebx
        mov    dword ptr [esi + 0x52], ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        call   sub_1_8EBE
        mov    ecx, dword ptr [esi + 0x5a]
        test   ecx, ecx
        jne    loc_1_7B16
loc_1_7B3C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_7B48:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 4]
        test   eax, eax
        je     loc_1_7B5C
        mov    edx, dword ptr [eax + 0x24]
        mov    dword ptr [ecx + 4], edx
loc_1_7B5C:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_7B68:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 0x52]
        test   edi, edi
        je     loc_1_7BB1
        mov    esi, dword ptr [ebp + 0xc]
loc_1_7B7F:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi + 0x1c]
        mov    ebx, dword ptr [edi + 0xc]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_8EEE
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_8EF4
        push   edi
        call   sub_1_79B8
        mov    edi, ebx
        test   ebx, ebx
        jne    loc_1_7B7F
loc_1_7BB1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7BC8:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x5e]
        push   edx
        call   sub_1_8DB0
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_7BE8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0x56], 1
        jne    loc_1_7C09
        mov    edx, dword ptr [eax + 0x176]
        mov    eax, 0x2000
        out    dx, ax
        mov    eax, 0x4800
        out    dx, ax
loc_1_7C09:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7C18:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0x56], 0
        jne    loc_1_7C32
        mov    edx, dword ptr [eax + 0x176]
        mov    eax, 0x1800
        out    dx, ax
loc_1_7C32:
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 055h, 0B8h, 000h, 058h, 000h, 000h, 08Bh, 0ECh, 056h, 08Bh, 04Dh, 008h, 08Bh, 0B1h
        db  076h, 001h, 000h, 000h, 08Bh, 089h, 07Ah, 001h, 000h, 000h, 08Bh, 0D6h, 066h, 0EFh, 08Bh, 0D1h
        db  066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 028h, 000h, 000h, 066h, 0EFh
        db  08Bh, 0D1h, 066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 078h, 000h, 000h
        db  066h, 0EFh, 0B8h, 0FFh, 069h, 000h, 000h, 05Eh, 066h, 0EFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h
        db  024h, 000h
sub_1_7C88:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_0348
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7CA8:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7CB8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0xc0], 1
        call   sub_1_1BC8
        mov    edx, dword ptr [esi + 0x176]
        mov    eax, 0x1800
        mov    dword ptr [esi + 0x10e], 0
        out    dx, ax
        mov    eax, 0x7840
        pop    esi
        out    dx, ax
        mov    eax, 0x6000
        pop    ebp
        out    dx, ax
        ret    4
        db  08Bh, 0FFh
loc_1_7CF8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi]
        inc    dword ptr [esi + 0x1c]
        cmp    byte ptr [esi + 0x20], 0
        je     loc_1_7D12
        mov    edi, 0xc0010002
        jmp    loc_1_7D4E
loc_1_7D12:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 0x10]
        test   edx, edx
        jne    loc_1_7D28
        push   eax
        push   esi
        push   ecx
        call   sub_1_82B8
        mov    edi, eax
        jmp    loc_1_7D4E
loc_1_7D28:
        cmp    edx, 1
        jne    loc_1_7D49
        cmp    byte ptr [ecx + 0x83], 0
        je     loc_1_7D3D
        mov    edi, 0xc001000d
        jmp    loc_1_7D4E
loc_1_7D3D:
        push   eax
        push   esi
        push   ecx
        call   sub_1_8308
        mov    edi, eax
        jmp    loc_1_7D4E
loc_1_7D49:
        mov    edi, 0x10001
loc_1_7D4E:
        push   esi
        call   sub_1_79B8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7D68:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 0x14e]
        mov    word ptr [ebp - 6], bx
        mov    dword ptr [ebp - 0x20], ecx
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], ebx
        cmp    edx, 0x1010101
        mov    eax, edx
        mov    byte ptr [ebp - 4], bl
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [ebp - 0x1c], ebx
        ja     loc_1_7DB3
        je     loc_1_80F9
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_7DD4
        jmp    dword ptr [eax*4]             ; -> obj1:0x823C
loc_1_7DB3:
        cmp    eax, 0x1010102
        je     loc_1_8115
        cmp    eax, 0x1010103
        je     loc_1_8131
        cmp    eax, 0x1010104
        je     loc_1_81DF
loc_1_7DD4:
        mov    eax, edx
        and    eax, 0xff000000
        cmp    eax, 0xff000000
        je     loc_1_7DEE
        mov    dword ptr [ebp - 0x10], 0xc00000bb
        jmp    loc_1_81F0
loc_1_7DEE:
        add    edx, 0xfefeff
        cmp    edx, 8
        ja     loc_1_7E00
        jmp    dword ptr [edx*4]             ; -> obj1:0x8288
loc_1_7E00:
        mov    dword ptr [ebp - 0x10], 0xc00000bb
        jmp    loc_1_81F0
loc_1_7E0C:
        mov    al, byte ptr [esi + 0x5c8]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    loc_1_81F6
loc_1_7E22:
        mov    ax, word ptr [esi + 0x5c9]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_81F6
loc_1_7E3A:
        mov    ax, word ptr [esi + 0x5cb]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_81F6
loc_1_7E52:
        mov    ax, word ptr [esi + 0x5cd]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_81F6
loc_1_7E6A:
        mov    al, byte ptr [esi + 0x5cf]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    loc_1_81F6
loc_1_7E80:
        mov    ax, word ptr [esi + 0x5d2]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_81F6
loc_1_7E98:
        mov    ax, word ptr [esi + 0x5d0]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    loc_1_81F6
loc_1_7EB0:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x5d2]
        lea    ecx, [eax + eax*4]
        lea    edi, [ecx*2]
        cmp    dword ptr [ebp + 0x1c], edi
        jae    loc_1_7ED9
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], edi
        jmp    loc_1_81F0
loc_1_7ED9:
        push   edi
        mov    eax, dword ptr [esi + 0x5c4]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        mov    bl, 1
        call   sub_1_906C
        mov    ecx, dword ptr [ebp + 0x24]
        mov    dword ptr [ecx], edi
        jmp    loc_1_81F0
loc_1_7EF6:
        cmp    dword ptr [ebp + 0x1c], 0x1e
        jae    loc_1_7F11
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], 0x1e
        jmp    loc_1_81F0
loc_1_7F11:
        push   0x1e
        mov    eax, dword ptr [esi + 0x5c4]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        mov    bl, 1
        call   sub_1_906C
        mov    ecx, dword ptr [ebp + 0x24]
        mov    dword ptr [ecx], 0x1e
        jmp    loc_1_81F0
loc_1_7F33:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_7F48
        mov    eax, 0                        ; -> loc_1_9714
        mov    esi, 0x5c
        jmp    loc_1_81F6
loc_1_7F48:
        mov    eax, 0                        ; -> loc_1_9678
        mov    esi, 0x9c
        jmp    loc_1_81F6
loc_1_7F57:
        cmp    byte ptr [esi + 0x83], 0
        mov    dword ptr [ebp - 0x1c], 2
        jne    loc_1_7F6E
        mov    dword ptr [ebp - 0x1c], 0
loc_1_7F6E:
        lea    eax, [ebp - 0x1c]
        mov    esi, 4
        jmp    loc_1_81F6
loc_1_7F7B:
        lea    eax, [ebp - 0x20]
        mov    esi, 4
        jmp    loc_1_81F6
loc_1_7F88:
        mov    eax, dword ptr [esi + 0x152]
        mov    esi, 4
        sub    eax, 0xe
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_7FA1:
        mov    eax, dword ptr [esi + 0x156]
        mov    esi, 4
        sub    eax, 0xe
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_7FBA:
        mov    ecx, 0x64
        mov    eax, dword ptr [esi + 0x15a]
        sub    edx, edx
        mov    esi, 4
        div    ecx
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_7FD9:
        mov    eax, dword ptr [esi + 0x15e]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_7FEF:
        mov    eax, dword ptr [esi + 0x162]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_8005:
        mov    eax, dword ptr [esi + 0x166]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_801B:
        mov    eax, dword ptr [esi + 0x16a]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_8031:
        push   3
        add    esi, 0x44
        push   esi
        lea    eax, [ebp - 0xc]
        push   eax
        mov    esi, 4
        call   sub_1_906C
        and    dword ptr [ebp - 0xc], 0xffffff00
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_8054:
        mov    eax, 0                        ; -> loc_1_9770
        mov    esi, 0x18
        jmp    loc_1_81F6
loc_1_8063:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_807A
        test   ecx, ecx
        jne    loc_1_8075
        mov    eax, dword ptr [esi + 0x5e]
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_1_8089
loc_1_8075:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_8089
loc_1_807A:
        xor    eax, eax
        test   ecx, ecx
        jne    loc_1_8089
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x14]
        mov    eax, dword ptr [eax + 8]
loc_1_8089:
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_8099:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_80A6
        mov    eax, dword ptr [esi]
        sub    eax, 0xe
        jmp    loc_1_80AC
loc_1_80A6:
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x18]
loc_1_80AC:
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_80BC:
        lea    eax, [ebp - 6]
        mov    esi, 2
        mov    word ptr [ebp - 6], 0x300
        jmp    loc_1_81F6
loc_1_80CF:
        mov    eax, dword ptr [esi + 0x156]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [ebp - 0xc]
        jmp    loc_1_81F6
loc_1_80E5:
        lea    eax, [ebp - 0xc]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], 7
        jmp    loc_1_81F6
loc_1_80F9:
        push   6
        add    esi, 0x44
        push   esi
        lea    eax, [ebp - 0x28]
        push   eax
        mov    esi, 6
        call   sub_1_906C
        lea    eax, [ebp - 0x28]
        jmp    loc_1_81F6
loc_1_8115:
        push   6
        add    esi, 0x3e
        push   esi
        lea    eax, [ebp - 0x28]
        push   eax
        mov    esi, 6
        call   sub_1_906C
        lea    eax, [ebp - 0x28]
        jmp    loc_1_81F6
loc_1_8131:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_8186
        mov    ecx, dword ptr [esi + 0x5e]
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_1_815B
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_1_81F0
loc_1_815B:
        mov    eax, dword ptr [ebp + 0x18]
        lea    edx, [ebp - 0x18]
        push   eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        lea    eax, [ebp - 0x10]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_0760
loc_1_8171:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 0x24]
        cmp    dword ptr [ebp - 0x10], 0
        lea    eax, [eax + eax*2]
        mov    bl, 1
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_81F0
loc_1_8186:
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x5e]
        mov    eax, dword ptr [edi + 0x14]
        push   eax
        push   ecx
        call   sub_2_0660
loc_1_8196:
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 0x1c], eax
        jae    loc_1_81B1
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [ecx], eax
        jmp    loc_1_81F0
loc_1_81B1:
        mov    eax, dword ptr [ebp + 0x18]
        lea    ecx, [ebp - 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x1c]
        push   ecx
        mov    eax, dword ptr [edi + 0x14]
        push   edx
        mov    ecx, dword ptr [esi + 0x5e]
        push   eax
        lea    edx, [ebp - 0x10]
        push   ecx
        push   edx
        mov    bl, 1
        call   sub_2_06A0
loc_1_81D0:
        mov    ecx, dword ptr [ebp - 0x18]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 0x24]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    loc_1_81F0
loc_1_81DF:
        lea    eax, [ebp - 0xc]
        mov    esi, 4
        mov    dword ptr [ebp - 0xc], 0x10
        jmp    loc_1_81F6
loc_1_81F0:
        mov    eax, dword ptr [ebp - 0x14]
        mov    esi, dword ptr [ebp - 0x14]
loc_1_81F6:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_822D
        test   bl, bl
        jne    loc_1_822D
        cmp    dword ptr [ebp + 0x1c], esi
        jae    loc_1_821D
        mov    ecx, dword ptr [ebp + 0x20]
        pop    edi
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx], esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_821D:
        push   esi
        push   eax
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        call   sub_1_906C
        mov    ecx, dword ptr [ebp + 0x24]
        add    dword ptr [ecx], esi
loc_1_822D:
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 049h, 000h
        ; ---- dispatch table (28 entries) ----
        dd  loc_1_7F33
        dd  loc_1_7F57
        dd  loc_1_7F7B
        dd  loc_1_7F7B
        dd  loc_1_7F88
        dd  loc_1_7FA1
        dd  loc_1_7FBA
        dd  loc_1_7FD9
        dd  loc_1_7FEF
        dd  loc_1_8005
        dd  loc_1_801B
        dd  loc_1_8031
        dd  loc_1_8054
        dd  loc_1_8063
        dd  loc_1_8099
        dd  loc_1_80BC
        dd  loc_1_80CF
        dd  loc_1_7DD4
        dd  loc_1_80E5
        dd  loc_1_7E0C
        dd  loc_1_7E22
        dd  loc_1_7E3A
        dd  loc_1_7E52
        dd  loc_1_7E6A
        dd  loc_1_7E80
        dd  loc_1_7E98
        dd  loc_1_7EB0
        dd  loc_1_7EF6
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_82B8:
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
        call   sub_1_7D68
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8308:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x1010103
        ja     loc_1_8352
        je     loc_1_8527
        cmp    eax, 0x1010e
        je     loc_1_8495
        cmp    eax, 0x1010f
        je     loc_1_84D1
        cmp    eax, 0x10112
        je     loc_1_851E
loc_1_8352:
        mov    edx, eax
        and    edx, 0xff000000
        cmp    edx, 0xff000000
        je     loc_1_8376
        mov    dword ptr [ebp - 4], 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_8565
loc_1_8376:
        add    eax, 0xfefefe
        cmp    eax, 3
        ja     loc_1_8387
        jmp    dword ptr [eax*4]             ; -> obj1:0x8584
loc_1_8387:
        mov    dword ptr [ebp - 4], 0xc0010017
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_8565
loc_1_839B:
        cmp    ebx, 2
        je     loc_1_83B4
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_8565
loc_1_83B4:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 0x5c9
        push   edi
        call   sub_1_906C
        jmp    loc_1_8565
loc_1_83CB:
        cmp    ebx, 2
        je     loc_1_83E4
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_8565
loc_1_83E4:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 0x5c9
        push   edi
        call   sub_1_906C
        jmp    loc_1_8565
loc_1_83FB:
        cmp    ebx, 2
        je     loc_1_8414
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_8565
loc_1_8414:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 0x5c9
        push   edi
        call   sub_1_906C
        jmp    loc_1_8565
loc_1_842B:
        cmp    ebx, 1
        je     loc_1_8444
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_8565
loc_1_8444:
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx]
        cmp    byte ptr [edi + 0x5cf], al
        je     loc_1_8565
        mov    byte ptr [edi + 0x5cf], al
        test   al, al
        jne    loc_1_8565
        xor    edx, edx
        xor    eax, eax
        mov    word ptr [edi + 0x5d0], 0
        mov    dx, word ptr [edi + 0x5d2]
        mov    edi, dword ptr [edi + 0x5c4]
        lea    edx, [edx + edx*4]
        add    edx, edx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        jmp    loc_1_8565
loc_1_8495:
        cmp    ebx, 4
        je     loc_1_84AE
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_8565
loc_1_84AE:
        push   4
        lea    eax, [ebp - 0xc]
        push   ecx
        push   eax
        call   sub_1_906C
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edi
        call   sub_1_8888
        jmp    loc_1_8562
loc_1_84D1:
        cmp    ebx, 4
        je     loc_1_84E7
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_8565
loc_1_84E7:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_906C
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x152]
        sub    eax, 0xe
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_1_8515
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   edi
        mov    dword ptr [eax + 0x18], ecx
        call   sub_1_88F8
        jmp    loc_1_8565
loc_1_8515:
        mov    dword ptr [ebp - 4], 0xc0010014
        jmp    loc_1_8565
loc_1_851E:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_8565
loc_1_8527:
        mov    edi, 6
        mov    eax, ebx
        sub    edx, edx
        div    edi
        test   edx, edx
        je     loc_1_8547
        mov    dword ptr [ebp - 4], 0xc0010014
        xor    eax, eax
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        jmp    loc_1_8565
loc_1_8547:
        push   ecx
        mov    eax, ebx
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   edi
        call   sub_1_8818
loc_1_8562:
        mov    dword ptr [ebp - 4], eax
loc_1_8565:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_8575
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], ebx
loc_1_8575:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
        ; ---- dispatch table (4 entries) ----
        dd  loc_1_839B
        dd  loc_1_83CB
        dd  loc_1_83FB
        dd  loc_1_842B
        db  08Dh, 064h, 024h, 000h
sub_1_8598:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    dword ptr [ebp - 4], 0
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
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], ecx
        lea    eax, [ebp - 8]
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
        call   sub_1_7D68
        cmp    eax, 0xc00000bb
        mov    edi, eax
        jne    loc_1_873E
        xor    edi, edi
        push   ebx
        call   sub_1_7C88
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x2020d
        ja     loc_1_861C
        je     loc_1_86B7
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_864A
        jmp    dword ptr [eax*4]             ; -> obj1:0x8758
loc_1_861C:
        cmp    eax, 0x1020201
        ja     loc_1_8654
        je     loc_1_86D7
        cmp    eax, 0x1020101
        je     loc_1_86BF
        cmp    eax, 0x1020102
        je     loc_1_86C7
        cmp    eax, 0x1020103
        je     loc_1_86CF
loc_1_864A:
        mov    edi, 0xc0010017
        jmp    loc_1_8710
loc_1_8654:
        sub    eax, 0x1020202
        cmp    eax, 5
        ja     loc_1_864A
        jmp    dword ptr [eax*4]             ; -> obj1:0x876C
loc_1_8665:
        mov    eax, dword ptr [ebx + 0x302]
        jmp    loc_1_870D
loc_1_8670:
        mov    eax, dword ptr [ebx + 0x306]
        jmp    loc_1_870D
loc_1_867B:
        mov    eax, dword ptr [ebx + 0x316]
        add    eax, dword ptr [ebx + 0x33a]
        add    eax, dword ptr [ebx + 0x336]
        add    eax, dword ptr [ebx + 0x326]
        jmp    loc_1_870D
loc_1_8695:
        mov    eax, dword ptr [ebx + 0x34a]
        add    eax, dword ptr [ebx + 0x33e]
        add    eax, dword ptr [ebx + 0x342]
        add    eax, dword ptr [ebx + 0x346]
        jmp    loc_1_870D
loc_1_86AF:
        mov    eax, dword ptr [ebx + 0x34a]
        jmp    loc_1_870D
loc_1_86B7:
        mov    eax, dword ptr [ebx + 0x342]
        jmp    loc_1_870D
loc_1_86BF:
        mov    eax, dword ptr [ebx + 0x33e]
        jmp    loc_1_870D
loc_1_86C7:
        mov    eax, dword ptr [ebx + 0x31e]
        jmp    loc_1_870D
loc_1_86CF:
        mov    eax, dword ptr [ebx + 0x322]
        jmp    loc_1_870D
loc_1_86D7:
        mov    eax, dword ptr [ebx + 0x332]
        jmp    loc_1_870D
loc_1_86DF:
        mov    eax, dword ptr [ebx + 0x326]
        jmp    loc_1_870D
loc_1_86E7:
        mov    eax, dword ptr [ebx + 0x34a]
        jmp    loc_1_870D
loc_1_86EF:
        mov    eax, dword ptr [ebx + 0x316]
        jmp    loc_1_870D
loc_1_86F7:
        mov    eax, dword ptr [ebx + 0x31a]
        jmp    loc_1_870D
loc_1_86FF:
        mov    eax, dword ptr [ebx + 0x32a]
        jmp    loc_1_870D
loc_1_8707:
        mov    eax, dword ptr [ebx + 0x32e]
loc_1_870D:
        mov    dword ptr [ebp - 4], eax
loc_1_8710:
        push   ebx
        call   sub_1_7CA8
        test   edi, edi
        jne    loc_1_873E
        cmp    dword ptr [ebp - 0x14], 4
        jb     loc_1_8735
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x10]
        push   ecx
        call   sub_1_906C
        add    dword ptr [ebp - 0xc], 4
        jmp    loc_1_873E
loc_1_8735:
        add    dword ptr [ebp - 8], 4
        mov    edi, 0xc0010014
loc_1_873E:
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
        db  08Dh, 049h, 000h
        ; ---- dispatch table (11 entries) ----
        dd  loc_1_8665
        dd  loc_1_8670
        dd  loc_1_867B
        dd  loc_1_8695
        dd  loc_1_86AF
        dd  loc_1_86DF
        dd  loc_1_86E7
        dd  loc_1_86EF
        dd  loc_1_86F7
        dd  loc_1_86FF
        dd  loc_1_8707
        db  08Dh, 064h, 024h, 000h
loc_1_8788:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x14]
        cmp    ecx, 0x20105
        ja     loc_1_87BB
        cmp    ecx, 0x20101
        jae    loc_1_8805
        cmp    ecx, 0x10101
        jb     loc_1_87FC
        cmp    ecx, 0x10111
        jbe    loc_1_8805
        mov    eax, 0xc0010017
        pop    ebp
        ret    8
loc_1_87BB:
        cmp    ecx, 0x1010104
        ja     loc_1_87DC
        cmp    ecx, 0x1010101
        jae    loc_1_8805
        cmp    ecx, 0x2020d
        je     loc_1_8805
        mov    eax, 0xc0010017
        pop    ebp
        ret    8
loc_1_87DC:
        cmp    ecx, 0x1020207
        ja     loc_1_87FC
        cmp    ecx, 0x1020201
        jae    loc_1_8805
        cmp    ecx, 0x1020101
        jb     loc_1_87FC
        cmp    ecx, 0x1020103
        jbe    loc_1_8805
loc_1_87FC:
        mov    eax, 0xc0010017
        pop    ebp
        ret    8
loc_1_8805:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8598
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_8818:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ecx + 0x83], 0
        push   edi
        jne    loc_1_886E
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_8861
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x14]
        push   edx
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        inc    dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [ecx + 0x5e]
        push   ecx
        call   sub_2_0300
loc_1_8851:
        mov    edi, eax
        push   esi
        call   sub_1_79B8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
loc_1_8861:
        mov    edi, 0xc0010002
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
loc_1_886E:
        mov    edi, 0xc001000d
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x14
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8888:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        cmp    byte ptr [ecx + 0x83], 0
        jne    loc_1_88E4
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x20], 0
        jne    loc_1_88D7
        inc    dword ptr [esi + 0x1c]
        cmp    dword ptr [ecx + 0x14e], edi
        jne    loc_1_88C9
        push   1
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    edi, dword ptr [esi + 0x14]
        push   edi
        mov    eax, dword ptr [ecx + 0x5e]
        push   eax
        call   sub_2_05E0
loc_1_88C7:
        mov    edi, eax
loc_1_88C9:
        push   esi
        call   sub_1_79B8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_1_88D7:
        mov    edi, 0xc0010002
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_1_88E4:
        mov    edi, 0xc001000d
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_88F8:
        push   ebp
        mov    ecx, 0x3c
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x52]
        test   edx, edx
        je     loc_1_891B
loc_1_890B:
        mov    esi, dword ptr [edx + 0x18]
        cmp    ecx, esi
        jae    loc_1_8914
        mov    ecx, esi
loc_1_8914:
        mov    edx, dword ptr [edx + 0xc]
        test   edx, edx
        jne    loc_1_890B
loc_1_891B:
        add    ecx, 0xe
        pop    esi
        pop    ebp
        mov    dword ptr [eax], ecx
        ret    4
        db  08Dh, 049h, 000h
sub_1_8928:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        push   esi
        push   eax
        push   ecx
        call   sub_1_8FB6
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ebp + 0x1c], 0
        je     loc_1_8956
        mov    dword ptr [ecx + 0xb2], eax
        jmp    loc_1_895C
loc_1_8956:
        mov    dword ptr [ecx + 0xae], eax
loc_1_895C:
        test   esi, esi
        je     loc_1_8984
        mov    ebx, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0xc]
loc_1_8966:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        add    edi, 4
        push   ebx
        push   eax
        add    ebx, 4
        call   sub_1_8FC2
        dec    esi
        jne    loc_1_8966
loc_1_8984:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8998:
        push   ebp
        mov    ecx, 0
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0xc]
        test   eax, eax
        je     loc_1_89BB
        mov    edx, dword ptr [esi + 4]
        lea    ecx, [eax + edx]
        shr    edx, 0xc
        dec    ecx
        shr    ecx, 0xc
        sub    ecx, edx
        inc    ecx
loc_1_89BB:
        mov    eax, dword ptr [ebp + 0x18]
        cmp    dword ptr [eax], ecx
        jb     loc_1_89D2
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   esi
        call   sub_1_8FDA
        pop    esi
        pop    ebp
        ret    0x14
loc_1_89D2:
        pop    esi
        mov    dword ptr [eax], ecx
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_89E8:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_89F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xad]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_8A39
        push   0x100
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax]
        and    eax, 0x1f
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x3a]
        push   eax
        call   sub_1_8F42
        jmp    loc_1_8A59
loc_1_8A39:
        push   0x100
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax]
        and    eax, 0x1f
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_8F36
loc_1_8A59:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_8A69:
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8A78:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 0x10]
        push   ecx
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0xad], 0
        push   edi
        push   esi
        je     loc_1_8ABE
        mov    eax, dword ptr [ebx + 0x3a]
        push   eax
        call   sub_1_8F48
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebx + 0x3a]
        push   ecx
        push   edi
        push   esi
        push   edx
        call   sub_1_8F42
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
loc_1_8ABE:
        mov    eax, dword ptr [ebx + 8]
        push   eax
        call   sub_1_8F3C
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebx + 8]
        push   ecx
        push   edi
        push   esi
        push   edx
        call   sub_1_8F36
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8AE8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_00A8
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8B08:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   eax
        call   sub_1_00F8
        pop    ebp
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_8B30:
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
        call   sub_1_8B60
        pop    ebp
        ret    0x20
        db  08Dh, 064h, 024h, 000h
sub_1_8B60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0xe
        jb     loc_1_8C00
        mov    edx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [edx]
        test   al, 1
        je     loc_1_8BA0
        cmp    dword ptr [edx + 2], -1
        jne    loc_1_8B95
        cmp    al, 0xff
        jne    loc_1_8B95
        cmp    byte ptr [edx + 1], 0xff
        jne    loc_1_8B95
        mov    dword ptr [ebp - 4], 8
        jmp    loc_1_8C07
loc_1_8B95:
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_8C07
        db  08Bh, 0FFh
loc_1_8BA0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xc], 0x20
        je     loc_1_8BF0
        mov    edx, dword ptr [ebp + 0x10]
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_8BC6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_8BCD
loc_1_8BC6:
        mov    dword ptr [ebp - 0x10], 1
loc_1_8BCD:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_8BE0
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_8C0A
        db  08Dh, 064h, 024h, 000h
loc_1_8BE0:
        mov    dword ptr [ebp - 4], 0x20
        jmp    loc_1_8C0A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8BF0:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_8C0A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8C00:
        mov    dword ptr [ebp - 4], 0x20
loc_1_8C07:
        mov    esi, dword ptr [ebp + 8]
loc_1_8C0A:
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_8D01
loc_1_8C15:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    loc_1_8C6D
        test   al, 2
        je     loc_1_8C59
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
        je     loc_1_8C59
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0xc]
        test   dword ptr [ecx + eax*4], edx
        jne    loc_1_8C6D
loc_1_8C59:
        test   byte ptr [ebp - 4], 2
        je     loc_1_8C64
        test   bl, 4
        jne    loc_1_8C6D
loc_1_8C64:
        test   bl, 0x20
        je     loc_1_8CF6
loc_1_8C6D:
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
        call   sub_1_8EE2
        mov    byte ptr [edi + 0x18], 1
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_8CF6
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_8CAF
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_8CAF:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_8CC0
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_8CC6
        db  08Bh, 0FFh
loc_1_8CC0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_8CC6:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_8CD5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_8EE8
loc_1_8CD5:
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
        call   sub_1_8F8C
        mov    edi, ebx
        jmp    loc_1_8CF9
loc_1_8CF6:
        mov    edi, dword ptr [edi + 0x10]
loc_1_8CF9:
        or     edi, edi
        jne    loc_1_8C15
loc_1_8D01:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_8D10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8D20
        pop    ebp
        ret    4
sub_1_8D20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_1_8D9E
loc_1_8D30:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_8D97
        mov    byte ptr [edi + 0x18], 0
        inc    dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_8EE8
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_8D97
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_8D5C
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_8D5C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_8D70
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_8D76
        db  005h, 000h, 000h, 000h, 000h
loc_1_8D70:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_8D76:
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
        call   sub_1_8F8C
        mov    edi, ebx
        jmp    loc_1_8D9A
loc_1_8D97:
        mov    edi, dword ptr [edi + 0x10]
loc_1_8D9A:
        or     edi, edi
        jne    loc_1_8D30
loc_1_8D9E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8DB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0xc]
        test   al, 0x20
        je     loc_1_8DC5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_8DC5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     loc_1_8E01
        cmp    dword ptr [ecx + 2], -1
        jne    loc_1_8DF2
        cmp    bl, 0xff
        jne    loc_1_8DF2
        cmp    byte ptr [ecx + 1], 0xff
        jne    loc_1_8DF2
        test   al, 8
        je     loc_1_8DEB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_8DEB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_8DF2:
        test   al, 4
        jne    loc_1_8DFA
        test   al, 2
        je     loc_1_8E19
loc_1_8DFA:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_8E01:
        mov    eax, dword ptr [edx + 0x2e]
        cmp    dword ptr [ecx + 2], eax
        jne    loc_1_8E19
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 0x2c], ax
        jne    loc_1_8E19
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_8E19:
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
sub_1_8E9A:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_8EA0:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_8EA6:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_8EAC:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_8EB2:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_8EB8:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_8EBE:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_8EC4:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_8ECA:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_8ED6:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_8EE2:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_8EE8:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_8EEE:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_8EF4:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_8F0C:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
sub_1_8F12:
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
sub_1_8F18:
        VxDCall  VPOWERD.Service_8049h           ; cd2049802800
        VxDCall  VPOWERD.Service_804Fh           ; cd204f802800
        VxDCall  VPOWERD.Service_8057h           ; cd2057802800
        VxDCall  VPOWERD.Service_8058h           ; cd2058802800
        VxDCall  VPOWERD.Service_8059h           ; cd2059802800
sub_1_8F36:
        VxDCall  VPOWERD.Service_805Ah           ; cd205a802800
sub_1_8F3C:
        VxDCall  VPOWERD.Service_805Bh           ; cd205b802800
sub_1_8F42:
        VxDCall  VPOWERD.Service_805Ch           ; cd205c802800
sub_1_8F48:
        VxDCall  VPOWERD.Service_805Dh           ; cd205d802800
        VxDCall  VPOWERD.Service_805Eh           ; cd205e802800
        VxDCall  VPOWERD.Service_8056h           ; cd2056802800
        add    byte ptr [eax], al
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000802800
sub_1_8F62:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_8F68:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_8F6E:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_8F74:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_8F80:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_8F86:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_8F8C:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_8FB0:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_8FB6:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_8FC2:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_8FD4:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
sub_1_8FDA:
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_8FF8:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
sub_1_8FFE:
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_9004:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_900A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_9010:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
sub_1_9016:
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
sub_1_9058:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
sub_1_9064:
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_906C:
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
sub_1_908C:
        push   esi
        push   edi
        pushfd 
        mov    ecx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        cmp    esi, edi
        jae    loc_1_90AC
        or     ecx, ecx
        je     loc_1_90C2
        std    
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    loc_1_90C0
loc_1_90AC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     loc_1_90C0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
loc_1_90C0:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_90C2:
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
        db  'EL59X   '                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  EL59X_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_911C:
        dec    ecx
        push   ebx
        push   edx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9124:
        inc    esp
        push   eax
        inc    ebx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_912C:
        inc    esp
        push   eax
        inc    ebx
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9134:
        inc    esp
        push   eax
        inc    ebx
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_913C:
        dec    ecx
        push   edx
        inc    ebp
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9144:
        inc    esi
        inc    ecx
        dec    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_914C:
        dec    ecx
        inc    esp
        dec    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9154:
        push   ebp
        push   ebx
        inc    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_915C:
        push   ebp
        push   ebx
        inc    esp
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9164:
        push   ebp
        push   ebx
        inc    esp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_916C:
        push   ebp
        push   ebx
        inc    esp
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9174:
        push   ebp
        push   ebx
        inc    esp
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_917C:
        push   ebp
        dec    ecx
        inc    ebx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9184:
        push   ebp
        dec    ecx
        inc    ebx
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [edi + edx*2 + 0x4e], al
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9194:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_919C:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91A4:
        push   eax
        inc    ecx
        inc    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91AC:
        push   ebp
        dec    esi
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_91B4:
        inc    esp
        push   edi
        dec    esi
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_91BC:
        dec    ecx
        inc    esp
        dec    esi
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91C4:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91CC:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91D4:
        push   eax
        inc    ecx
        inc    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91DC:
        push   ebp
        dec    esi
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_91E4:
        dec    ecx
        inc    esp
        dec    esi
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_91EC:
        push   edx
        inc    esp
        dec    esi
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91F4:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_91FC:
        inc    ecx
        push   esi
        inc    ecx
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9204:
        push   eax
        inc    ecx
        inc    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_920C:
        push   ebp
        dec    esi
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9214:
        push   edx
        inc    esp
        dec    esi
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax + ebx*2 + 0x43], dl
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9224:
        push   esp
        pop    eax
        inc    ebx
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax + ebx*2 + 0x41], dl
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_9234:
        push   esp
        pop    eax
        inc    ecx
        cmp    al, 0
        add    byte ptr [eax], al
        db  000h
loc_1_923C:
        inc    ecx
        inc    esp
        dec    edx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9244:
        push   esp
        push   edx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_924C:
        inc    esp
        dec    edi
        inc    esi
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9254:
        push   esp
        inc    esp
        dec    ebp
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_925C:
        dec    ecx
        push   esp
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9264:
        dec    esi
        inc    esp
        dec    ebp
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_926C:
        push   ebp
        pop    eax
        inc    esi
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9274:
        push   edx
        pop    eax
        inc    ebp
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_927C:
        inc    ebp
        xor    cl, byte ptr [ebp + 0x53]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9284:
        inc    ebp
        xor    cl, byte ptr [ebp + 0x50]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_928C:
        inc    esp
        dec    esp
        xor    dword ptr [esi], edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9294:
        inc    esp
        dec    esp
        xor    dword ptr [eax + eax], edi
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 0x41], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92A4:
        inc    ebp
        xor    dl, byte ptr [eax + 0x49]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92AC:
        push   edx
        pop    eax
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92B4:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92BC:
        push   edx
        pop    eax
        inc    ebp
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x58], dl
        inc    ebp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x31], al
        inc    esp
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92D4:
        inc    esp
        dec    esp
        xor    bh, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92DC:
        inc    esp
        dec    esp
        xor    bh, byte ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 0x41], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92EC:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92F4:
        inc    ebp
        xor    dword ptr [eax + 0x4c], edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_92FC:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9304:
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_930C:
        push   edx
        inc    esp
        inc    ebp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x50], dl
        inc    ebp
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x58], dl
        inc    ebp
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x58], dl
        inc    ebx
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x58], dl
        inc    esp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9334:
        push   edx
        pop    eax
        inc    ebx
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_933C:
        push   edx
        inc    ebx
        inc    esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9344:
        push   edx
        inc    ebx
        inc    esp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_934C:
        inc    esp
        dec    esp
        xor    edi, dword ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9354:
        inc    esp
        dec    esp
        xor    edi, dword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 0x41], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9364:
        push   edx
        inc    ebx
        push   eax
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_936C:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9374:
        push   edx
        pop    eax
        inc    ebx
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  000h
loc_1_937C:
        push   edx
        inc    ebx
        inc    esp
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [esp + ecx*2 + 0x34], al
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_938C:
        inc    esp
        dec    esp
        xor    al, 0x3c
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9394:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_939C:
        push   edx
        pop    eax
        inc    ebx
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x50], dl
        dec    esp
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_93AC:
        push   ebp
        push   eax
        dec    esp
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax + edx*2 + 0x49], cl
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93BC:
        dec    esp
        push   eax
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93C4:
        push   edx
        inc    ebp
        dec    edx
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x52], cl
        push   ebp
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93D4:
        push   edx
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93DC:
        inc    ecx
        dec    esp
        inc    edi
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93E4:
        inc    ebx
        push   edx
        inc    ebx
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x49], dl
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93F4:
        inc    esp
        dec    ecx
        push   ebx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_93FC:
        push   edx
        push   edx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9404:
        dec    eax
        inc    ecx
        dec    ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_940C:
        dec    eax
        inc    ecx
        dec    ebx
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
loc_1_9418:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr ds:[eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebp
        dec    esp
        xor    eax, 0x5839
        db  000h
loc_1_94BC:
        inc    ebp
        dec    esp
        xor    eax, 0x5839
        add    byte ptr [ebp + 0x61], cl
        js     0x9531
        insd   dword ptr es:[edi], dx
        jne    loc_1_9538
        dec    ebp
        jne    loc_1_953A
        je     0x9539
        arpl   word ptr [ecx + 0x73], sp
        je     loc_1_9521
        imul   esi, dword ptr [ebx + 0x74], 0
        dec    ebp
        popal  
        js     loc_1_9549
        insd   dword ptr es:[edi], dx
        jne    loc_1_9550
        dec    ebp
        jne    0x9552
        je     loc_1_9551
        arpl   word ptr [ecx + 0x73], sp
        je     0x9539
        imul   esi, dword ptr [ebx + 0x74], 0
        inc    esp
        push   esp
        inc    ecx
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    loc_1_956D
        arpl   word ptr [ebp + 0x43], sp
        outsd  dx, dword ptr [esi]
        jne    loc_1_956F
        je     loc_1_9503
loc_1_9503:
        add    byte ptr [esp + edx*2 + 0x41], al
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    loc_1_957D
        arpl   word ptr [ebp + 0x43], sp
        outsd  dx, dword ptr [esi]
        jne    loc_1_957F
        je     loc_1_9513
loc_1_9513:
        add    byte ptr [esi + 0x45], cl
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
loc_1_9521:
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
        inc    edx
        jne    loc_1_95A2
        push   esp
        jns    loc_1_95A2
        add    byte ptr gs:[edx + 0x75], al
        jae    loc_1_958C
loc_1_9538:
        jns    loc_1_95AA
loc_1_953A:
        add    byte ptr gs:[edx + 0x55], al
        push   ebx
        db  000h
loc_1_9540:
        inc    edx
        push   ebp
        push   ebx
        add    byte ptr [edx + 0x55], al
        push   ebx
        dec    esi
        dec    edi
loc_1_9549:
        add    byte ptr [eax], al
        db  000h
loc_1_954C:
        inc    edx
        push   ebp
        push   ebx
        dec    esi
loc_1_9550:
        dec    edi
loc_1_9551:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x4c], dl
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_955C:
        push   ebx
        dec    esp
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        inc    ecx
        push   esp
        inc    ebx
        dec    eax
        inc    ecx
        inc    esp
        inc    esp
        push   edx
loc_1_956D:
        inc    ebp
        push   ebx
loc_1_956F:
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        inc    ecx
        push   esp
        inc    ebx
        dec    eax
        inc    ecx
        inc    esp
        inc    esp
        push   edx
loc_1_957D:
        inc    ebp
        push   ebx
loc_1_957F:
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        push   ebp
        push   ebx
        dec    ebp
        inc    ecx
        push   ebx
        push   esp
        inc    ebp
loc_1_958C:
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x55], al
        push   ebx
        dec    ebp
        inc    ecx
        push   ebx
        push   esp
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x49], cl
        dec    esi
        push   esp
        pop    eax
        push   ebx
loc_1_95A2:
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
loc_1_95A8:
        dec    ebp
        dec    ecx
loc_1_95AA:
        dec    esi
        push   esp
        pop    eax
        push   ebx
        push   esp
        inc    ecx
        push   edx
        push   esp
        add    byte ptr [eax], al
        dec    ebp
        dec    ecx
        dec    esi
        push   edx
        pop    eax
        inc    ebp
        inc    ecx
        push   edx
        dec    esp
        pop    ecx
        add    byte ptr [eax], al
loc_1_95C0:
        dec    ebp
        dec    ecx
        dec    esi
        push   edx
        pop    eax
        inc    ebp
        inc    ecx
        push   edx
        dec    esp
        pop    ecx
        add    byte ptr [eax], al
        push   esp
        pop    eax
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        push   ebx
        dec    edi
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [eax + ebx*2 + 0x43], dl
        push   edx
        dec    edi
        push   ebx
        push   ebx
        dec    edi
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [edx + 0x58], dl
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        push   ebx
        dec    edi
        push   esi
        inc    ebp
        push   edx
        db  000h
loc_1_95F0:
        push   edx
        pop    eax
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        push   ebx
        dec    edi
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ebp + 0x4e], dl
        inc    esp
        inc    ebp
        push   edx
        inc    edi
        dec    edi
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x4e], dl
        inc    esp
        inc    ebp
        push   edx
        inc    edi
        dec    edi
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        add    byte ptr [ebp + 0x49], al
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_9624:
        inc    ebp
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        push   ebx
        inc    ecx
        db  000h
loc_1_9630:
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [ecx + 0x45], bl
        push   ebx
        db  000h
loc_1_9638:
        pop    ecx
        inc    ebp
        push   ebx
        add    byte ptr [esi + 0x4f], cl
        add    byte ptr [eax], al
loc_1_9640:
        dec    esi
        dec    edi
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
        add    byte ptr [ecx + 0x4e], cl
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
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_9678:
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
        add    eax, 0xd000201
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
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
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, 0x6010202
        add    al, byte ptr [edx]
        add    dword ptr [edi], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx + eax], eax
        add    al, byte ptr [ecx]
loc_1_9714:
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
loc_1_9770:
        inc    esi
        popal  
        jae    loc_1_97E8
        and    byte ptr [ebp + 0x74], al
        push   0x656e7265
        je     loc_1_979E
        inc    ecx
        popal  
        jo     loc_1_97F7
        jb     loc_1_9786
loc_1_9786:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_979E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97C8:
        add    byte ptr [eax], al
loc_1_97CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97CE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97DE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97E2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97E6:
        add    byte ptr [eax], al
loc_1_97E8:
        add    byte ptr [eax], al
loc_1_97EA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_97F2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_97F7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x9790]
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   0xffff2f86
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_1_A034
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_A034:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0xc
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    loc_1_A04E
        mov    esi, 2
loc_1_A04E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   0xffff2f86
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     loc_1_A082
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_A120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_A082:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        call   0xffff2f86
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     loc_1_A0B1
        push   eax
        call   sub_1_A120
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_1_A0B1:
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
sub_1_A120:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_A140
        mov    ecx, dword ptr [esi + 0x20]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        call   0xffff2f8c
loc_1_A140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_1_A156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   0xffff2f8c
loc_1_A156:
        push   0
        push   0x32
        push   esi
        call   0xffff2f8c
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_1_A189
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_A189:
        mov    eax, dword ptr [0x9790]
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   0xffff2f86
        or     eax, eax
        je     loc_1_A1A9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_A1A9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x28]
        mov    edi, edi
loc_1_A1B0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_1_A1C3
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_1_A1B0
        jmp    loc_1_A1D2
loc_1_A1C3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x28], eax
loc_1_A1D2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_A1E8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_1_A1E8:
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
        call   sub_1_A5E0
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_A269
        cmp    ebx, 0x103
        jne    loc_1_A280
loc_1_A269:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_1_A300
        or     eax, eax
        je     loc_1_A280
        mov    ebx, eax
loc_1_A280:
        or     ebx, ebx
        je     loc_1_A28C
        cmp    ebx, 0x103
        jne    loc_1_A2EB
loc_1_A28C:
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        mov    dword ptr [edi + 0xc], eax
        jne    loc_1_A2E6
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_1_A2A2
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_1_A2A2:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_A2B1
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_A2B7
loc_1_A2B1:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_A2B7:
        cmp    byte ptr [edi + 0x18], 0
        je     loc_1_A2C6
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   0xffff2ee8
loc_1_A2C6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   0xffff2f8c
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_A2E6:
        mov    ebx, 0xc001000e
loc_1_A2EB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_A300:
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
        call   0xffff308c
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   0xffff308c
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, edi
        je     loc_1_A384
        sub    eax, eax
        nop    
loc_1_A360:
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
        ja     loc_1_A360
loc_1_A384:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_1_A3EF
loc_1_A391:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_1_A3E4
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
        call   0xffff308c
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
        call   0xffff308c
        dec    dword ptr [esi + 0x24]
        jmp    loc_1_A3E7
loc_1_A3E4:
        inc    dword ptr [ebp - 4]
loc_1_A3E7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 0x24], eax
        ja     loc_1_A391
loc_1_A3EF:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_A4DA
        mov    edi, dword ptr [ebp + 0x18]
loc_1_A403:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 0x24]
        push   eax
        call   sub_1_A7C0
        or     al, al
        je     loc_1_A43A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0x18], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 0x19]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 0x18]
        or     dword ptr [ecx + eax*4], ebx
        jmp    loc_1_A4C8
loc_1_A43A:
        mov    ecx, dword ptr [esi + 0x24]
        cmp    dword ptr [esi + 0x20], ecx
        jbe    loc_1_A514
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
        call   0xffff308c
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
        call   0xffff308c
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
loc_1_A4C8:
        add    edi, 6
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x14], eax
        ja     loc_1_A403
loc_1_A4DA:
        mov    eax, dword ptr [esi + 0x24]
        sub    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0xc], ecx
        je     loc_1_A565
        mov    edi, dword ptr [ebp - 0x20]
        sub    edx, edx
        nop    
loc_1_A4F0:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_A56B
        mov    eax, dword ptr [esi + 4]
        add    eax, edx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    loc_1_A552
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    loc_1_A552
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_A559
loc_1_A514:
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0xffff308c
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   0xffff308c
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, 0xc0010009
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
loc_1_A552:
        mov    dword ptr [ebp - 8], 1
loc_1_A559:
        add    edi, 6
        add    edx, 6
        inc    ecx
        cmp    dword ptr [ebp - 0xc], ecx
        ja     loc_1_A4F0
loc_1_A565:
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_A5D0
loc_1_A56B:
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
        je     loc_1_A5D2
        cmp    edi, 0x103
        je     loc_1_A5D2
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0xffff308c
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   0xffff308c
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh
loc_1_A5D0:
        sub    edi, edi
loc_1_A5D2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Dh, 049h, 000h
sub_1_A5E0:
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
        je     loc_1_A61D
        lea    esp, [esp]
loc_1_A610:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0xc], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_1_A610
loc_1_A61D:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, ebx
        je     loc_1_A651
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
        je     loc_1_A653
        cmp    eax, 0x103
        je     loc_1_A653
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0xc], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_A651:
        sub    eax, eax
loc_1_A653:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h
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
        je     loc_1_A692
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_1_A687:
        test   dword ptr [esi], edx
        je     loc_1_A68C
        inc    eax
loc_1_A68C:
        add    esi, 4
        dec    edi
        jne    loc_1_A687
loc_1_A692:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
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
        jbe    loc_1_A71C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, edi
loc_1_A6E0:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 0x10]
        test   dword ptr [ecx + esi], eax
        je     loc_1_A70A
        cmp    dword ptr [ebp + 0x14], 6
        jb     loc_1_A736
        sub    dword ptr [ebp + 0x14], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   0xffff308c
        add    edi, 6
        inc    dword ptr [ebp - 0xc]
loc_1_A70A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 0x24], eax
        ja     loc_1_A6E0
loc_1_A71C:
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
loc_1_A736:
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
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x24]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 0x10]
        jbe    loc_1_A790
        mov    dword ptr [eax], 0xc0000001
        mov    eax, dword ptr [ebp + 0x14]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_A790:
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
        call   0xffff308c
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_A7C0:
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
        je     loc_1_A859
        dec    esi
        cmp    esi, edx
        jb     loc_1_A859
        nop    
loc_1_A7E0:
        cmp    edx, ecx
        jb     loc_1_A859
        mov    eax, dword ptr [ebp + 0xc]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    loc_1_A803
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_A837
loc_1_A803:
        jbe    loc_1_A810
        mov    dword ptr [ebp - 4], 0xffffffff
        jmp    loc_1_A837
        db  08Bh, 0FFh
loc_1_A810:
        mov    eax, dword ptr [ebp + 0x10]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    loc_1_A827
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_A837
loc_1_A827:
        mov    dword ptr [ebp - 4], 0xffffffff
        ja     loc_1_A837
        mov    dword ptr [ebp - 4], 0
loc_1_A837:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_A869
        jle    loc_1_A848
        or     edx, edx
        je     loc_1_A859
        lea    esi, [edx - 1]
        jmp    loc_1_A84B
loc_1_A848:
        lea    ecx, [edx + 1]
loc_1_A84B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    loc_1_A7E0
loc_1_A859:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_1_A869:
        mov    eax, dword ptr [ebp + 0x14]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h
        db  0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 000h, 0C3h, 020h, 045h, 04Ch
        db  035h, 039h, 078h, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 020h, 064h, 072h, 069h, 076h, 065h
        db  072h, 02Ch, 020h, 025h, 056h, 045h, 052h, 020h, 031h, 02Eh, 030h, 02Eh, 030h, 039h, 000h, 000h
        db  009h, 045h, 04Ch, 035h, 039h, 058h, 05Fh, 044h, 044h, 042h, 001h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x880  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x9790]       ; -> obj1:0x9790
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   0x32
        push   eax
        call   sub_1_8F86
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
        call   sub_1_8F86
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
        call   sub_1_8F86
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
        call   sub_1_8F8C
loc_2_0140:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_2_0156
        push   0
        mov    ecx, dword ptr [esi + 0x20]
        shl    ecx, 3
        push   ecx
        push   eax
        call   sub_1_8F8C
loc_2_0156:
        push   0
        push   0x32
        push   esi
        call   sub_1_8F8C
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
        mov    eax, dword ptr [0x9790]       ; -> obj1:0x9790
        push   eax
        push   0
        push   0x1a
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_8F86
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
        call   sub_1_8EE8
loc_2_02C6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x19]
        shl    eax, cl
        or     dword ptr [esi + 0x28], eax
        push   0
        push   0x1a
        push   edi
        call   sub_1_8F8C
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
        call   sub_1_908C
        mov    eax, dword ptr [esi + 0x24]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        sub    edi, edi
        push   eax
        call   sub_1_908C
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
        call   sub_1_908C
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
        call   sub_1_908C
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
        call   sub_1_908C
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
        call   sub_1_908C
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
        call   sub_1_908C
        mov    eax, dword ptr [ebp - 0xc]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_908C
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
        call   sub_1_908C
        push   ebx
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_908C
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
        call   sub_1_908C
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
        call   sub_1_908C
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

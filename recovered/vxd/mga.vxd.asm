; MGA -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 833 instructions recovered by recursive descent

Declare_Virtual_Device MGA, 3, 1, MGA_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xb98  R-X
MGA_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_2_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  sub_1_007D
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_2_0000
sub_1_007D:
        push   edi
        push   esi
        push   eax
        push   ebx
        call   sub_1_06AB
        je     loc_1_0091
        mov    eax, dword ptr [0xb78]        ; -> obj1:0x0B78
        or     eax, eax
        jne    loc_1_0097
loc_1_0091:
        pop    ebx
        pop    eax
        pop    esi
        pop    edi
        stc    
        ret    
loc_1_0097:
        mov    edi, eax
        pushal 
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    eax, eax
        xor    edi, edi
        xor    ecx, ecx
        lea    edx, [0xb7c]                  ; -> obj1:0x0B7C
        mov    esi, 0x667                    ; -> loc_1_0667
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        popal  
        mov    byte ptr [edi + 0x1fde], 0xa
        mov    byte ptr [edi + 0x1fdf], 0x80
        bt     dword ptr [0xb74], 0x17       ; -> obj1:0x0B74
        jae    loc_1_00EB
        mov    dx, 0x46e8
        xor    al, al
        out    dx, al
        bt     dword ptr [0xb74], 8          ; -> obj1:0x0B74
        jae    loc_1_00EB
        mov    dword ptr [edi + 0x2010], 0xac000
loc_1_00EB:
        mov    al, 0x67
        mov    byte ptr [edi + 0x1fc2], al
        mov    eax, dword ptr [0xb74]        ; -> obj1:0x0B74
        and    eax, 0xff
        cmp    eax, 9
        jne    loc_1_0158
        mov    byte ptr [edi + 0x3c00], 0x2c
        mov    byte ptr [edi + 0x3c28], 0xef
        mov    byte ptr [edi + 0x3c00], 0x2f
        mov    byte ptr [edi + 0x3c28], 0
        mov    byte ptr [edi + 0x3c00], 0x2c
        mov    byte ptr [edi + 0x3c28], 0
        mov    byte ptr [edi + 0x3c00], 0x2d
        mov    byte ptr [edi + 0x3c28], 0xb0
        mov    byte ptr [edi + 0x3c28], 0x29
        mov    byte ptr [edi + 0x3c28], 0xf2
        mov    ah, 0xff
loc_1_014A:
        dec    ah
        je     loc_1_0158
        mov    al, byte ptr [edi + 0x3c28]
        and    al, 4
        je     loc_1_014A
loc_1_0158:
        mov    byte ptr [edi + 0x1fd4], 0x11
        mov    al, byte ptr [edi + 0x1fd5]
        and    al, 0x7f
        mov    byte ptr [edi + 0x1fd5], al
        xor    al, al
        mov    byte ptr [edi + 0x1fd4], 1
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 0
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 6
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 0x11
        mov    al, 0x80
        or     byte ptr [edi + 0x1fd5], al
        mov    al, 1
        or     byte ptr [edi + 0x1e52], al
        xor    al, al
        mov    byte ptr [edi + 0x3e00], al
        bt     dword ptr [0xb74], 0x1f       ; -> obj1:0x0B74
        jae    loc_1_023E
        mov    al, byte ptr [edi + 0x3c80]
        mov    dword ptr [0xb88], eax        ; -> obj1:0x0B88
        mov    al, 0x40
        mov    byte ptr [edi + 0x3c80], al
        mov    eax, 1
        mov    dword ptr [edi + 0x1e40], eax
        mov    al, 0
        mov    byte ptr [edi + 0x3c84], al
        mov    bl, byte ptr [edi + 0x3c88]
        mov    bh, byte ptr [edi + 0x3c88]
        mov    al, byte ptr [edi + 0x3c88]
        mov    ah, byte ptr [edi + 0x3c88]
        shl    eax, 0x10
        mov    ax, bx
        or     eax, 0x81000000
        mov    al, 0
        mov    byte ptr [edi + 0x3c84], al
        mov    byte ptr [edi + 0x3c88], al
        mov    byte ptr [edi + 0x3c88], ah
        shr    eax, 0x10
        mov    byte ptr [edi + 0x3c88], al
        mov    byte ptr [edi + 0x3c88], ah
        mov    al, 8
        mov    byte ptr [edi + 0x3c84], al
        mov    eax, 0
        mov    dword ptr [edi + 0x1e40], eax
loc_1_023E:
        mov    eax, dword ptr [0xb74]        ; -> obj1:0x0B74
        and    eax, 0xff
        cmp    eax, 0
        je     loc_1_0360
        cmp    eax, 2
        je     loc_1_0334
        cmp    eax, 9
        je     loc_1_02AD
        mov    byte ptr [edi + 0x3c18], 0xff
        mov    byte ptr [edi + 0x3c1c], 0
        mov    byte ptr [edi + 0x3c18], 0x1d
        mov    byte ptr [edi + 0x3c1c], 0
        bt     dword ptr [0xb74], 0x1f       ; -> obj1:0x0B74
        jae    loc_1_02A8
        mov    byte ptr [edi + 0x3c84], 0xa
        mov    al, byte ptr [edi + 0x3c88]
        and    al, 0xb
        mov    byte ptr [edi + 0x3c84], 0xa
        mov    byte ptr [edi + 0x3c88], al
        mov    byte ptr [edi + 0x3c84], 8
loc_1_02A8:
        jmp    loc_1_0390
loc_1_02AD:
        mov    byte ptr [edi + 0x3c00], 0x2a
        mov    byte ptr [edi + 0x3c28], 1
        mov    byte ptr [edi + 0x3c00], 0x2b
        mov    byte ptr [edi + 0x3c28], 1
        mov    byte ptr [edi + 0x3c00], 0x1e
        mov    byte ptr [edi + 0x3c28], 4
        mov    byte ptr [edi + 0x3c00], 0x1d
        mov    byte ptr [edi + 0x3c28], 0
        mov    byte ptr [edi + 0x3c00], 0x1a
        mov    byte ptr [edi + 0x3c28], 0
        mov    byte ptr [edi + 0x3c00], 0x18
        mov    byte ptr [edi + 0x3c28], 0x80
        mov    byte ptr [edi + 0x3c00], 0x19
        mov    byte ptr [edi + 0x3c28], 0x98
        mov    byte ptr [edi + 0x3c00], 0x39
        mov    byte ptr [edi + 0x3c28], 0x18
        mov    byte ptr [edi + 0x3c00], 6
        mov    al, byte ptr [edi + 0x3c28]
        and    al, 0xfc
        mov    byte ptr [edi + 0x3c28], al
        jmp    loc_1_0390
loc_1_0334:
        mov    al, 0x80
        mov    byte ptr [edi + 0x3c18], al
        mov    al, 1
        mov    byte ptr [edi + 0x3c00], al
        xor    al, al
        mov    byte ptr [edi + 0x3c20], al
        mov    byte ptr [edi + 0x3c24], al
        mov    byte ptr [edi + 0x3c28], al
        mov    byte ptr [edi + 0x3c18], al
        jmp    loc_1_0390
loc_1_0360:
        mov    al, 1
        mov    byte ptr [edi + 0x3c18], al
        mov    al, 2
        mov    byte ptr [edi + 0x3c00], al
        mov    al, 0
        mov    byte ptr [edi + 0x3c08], al
        mov    al, 3
        mov    byte ptr [edi + 0x3c00], al
        mov    al, 0x10
        mov    byte ptr [edi + 0x3c08], al
        mov    al, 0
        mov    byte ptr [edi + 0x3c18], al
loc_1_0390:
        push   eax
        mov    eax, 0x10000
loc_1_0396:
        dec    eax
        jne    loc_1_0396
        pop    eax
        mov    al, 0
        mov    byte ptr [edi + 0x1fc4], al
        mov    al, 3
        mov    byte ptr [edi + 0x1fc5], al
        mov    dword ptr [edi + 0x1e40], 1
        push   ecx
        mov    ecx, 0x32
loc_1_03BA:
        dec    ecx
        jne    loc_1_03BA
        pop    ecx
        bt     dword ptr [0xb74], 0x10       ; -> obj1:0x0B74
        jb     loc_1_0438
        bt     dword ptr [0xb74], 0x11       ; -> obj1:0x0B74
        jae    loc_1_0438
        bt     dword ptr [0xb74], 0x17       ; -> obj1:0x0B74
        jb     loc_1_03E6
        mov    dword ptr [edi + 0x1e40], 0
loc_1_03E6:
        mov    al, 0x8d
        mov    byte ptr [edi + 0x1e47], al
        mov    dword ptr [edi + 0x1e40], 0
        bt     dword ptr [0xb74], 8          ; -> obj1:0x0B74
        jb     loc_1_0424
        mov    eax, dword ptr [edi + 0x1e50]
        or     eax, 0x10000000
        or     eax, 0x400
        or     eax, 0x10000000
        or     eax, 0x200
        mov    dword ptr [edi + 0x1e50], eax
        jmp    loc_1_042E
loc_1_0424:
        mov    dword ptr [edi + 0x1e50], 0x11000700
loc_1_042E:
        xor    al, al
        mov    byte ptr [edi + 0x1e47], al
        jmp    loc_1_0459
loc_1_0438:
        mov    al, 0x8d
        mov    byte ptr [edi + 0x1e47], al
        mov    eax, dword ptr [edi + 0x1e50]
        or     eax, 0x400
        mov    dword ptr [edi + 0x1e50], eax
        xor    al, al
        mov    byte ptr [edi + 0x1e47], al
loc_1_0459:
        bt     dword ptr [0xb74], 0x1f       ; -> obj1:0x0B74
        jae    loc_1_04C1
        mov    al, 0
        mov    byte ptr [edi + 0x3c84], al
        mov    al, byte ptr [edi + 0x3c88]
        mov    ah, byte ptr [edi + 0x3c88]
        shl    eax, 0x10
        mov    al, byte ptr [edi + 0x3c88]
        mov    ah, byte ptr [edi + 0x3c88]
        or     ah, 1
        rol    eax, 0x10
        mov    byte ptr [edi + 0x3c84], 0
        mov    byte ptr [edi + 0x3c88], al
        mov    byte ptr [edi + 0x3c88], ah
        shr    eax, 0x10
        mov    byte ptr [edi + 0x3c88], al
        mov    byte ptr [edi + 0x3c88], ah
        mov    al, 8
        mov    byte ptr [edi + 0x3c84], al
        mov    eax, dword ptr [0xb88]        ; -> obj1:0x0B88
        mov    byte ptr [edi + 0x3c80], al
loc_1_04C1:
        bt     dword ptr [0xb74], 0x10       ; -> obj1:0x0B74
        jb     loc_1_0507
        bt     dword ptr [0xb74], 0x11       ; -> obj1:0x0B74
        jae    loc_1_0507
        bt     dword ptr [0xb74], 8          ; -> obj1:0x0B74
        jae    loc_1_0507
        mov    al, 0x8d
        mov    byte ptr [edi + 0x1e47], al
        mov    byte ptr [edi + 0x1e50], 0
        mov    eax, dword ptr [edi + 0x1e50]
        or     eax, 0x200
        mov    dword ptr [edi + 0x1e50], eax
        xor    al, al
        mov    byte ptr [edi + 0x1e47], al
loc_1_0507:
        mov    dword ptr [edi + 0x1e40], 0
        bt     dword ptr [0xb74], 0x17       ; -> obj1:0x0B74
        jae    loc_1_0536
        bt     dword ptr [0xb74], 8          ; -> obj1:0x0B74
        jae    loc_1_052F
        mov    dword ptr [edi + 0x2010], 0x800ac000
loc_1_052F:
        mov    dx, 0x46e8
        mov    al, 8
        out    dx, al
loc_1_0536:
        mov    dx, 0x3ce
        mov    al, 6
        out    dx, al
        inc    dx
        mov    al, 5
        out    dx, al
        pop    ebx
        pop    eax
        pop    esi
        pop    edi
        clc    
        ret    
loc_1_0548:
        pushal 
        mov    eax, dword ptr [ebp + 0x18]
        cmp    eax, 0xac53962b
        jne    loc_1_0641
        mov    eax, dword ptr [0xb78]        ; -> obj1:0x0B78
        or     eax, eax
        jne    loc_1_063F
        mov    ax, word ptr [ebp + 0x3c]
        shl    eax, 0x10
        mov    ax, word ptr [ebp]
        mov    dword ptr [0xb7c], eax        ; -> obj1:0x0B7C
        movzx  eax, word ptr [ebp + 0x40]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        inc    eax
        je     loc_1_0635
        dec    eax
        movzx  esi, word ptr [ebp + 4]
        add    esi, eax
        mov    al, byte ptr [esi + 0x1c]
        and    eax, 0xff
        mov    dword ptr [0xb74], eax        ; -> obj1:0x0B74
        mov    eax, dword ptr [esi + 4]
        and    eax, 0xe
        cmp    al, 0xa
        jne    loc_1_05E4
        bts    dword ptr [0xb74], 0x11       ; -> obj1:0x0B74
        push   0
        push   0x1000
        push   0xc0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        inc    eax
        je     loc_1_05E4
        dec    eax
        add    eax, 0x78
        mov    eax, dword ptr [eax]
        and    eax, 0xffffff
        cmp    eax, 0x42565f
        jne    loc_1_05E4
        bts    dword ptr [0xb74], 0x17       ; -> obj1:0x0B74
loc_1_05E4:
        mov    eax, dword ptr [esi]
        cmp    eax, 0xac000
        jne    loc_1_05F5
        bts    dword ptr [0xb74], 8          ; -> obj1:0x0B74
loc_1_05F5:
        push   0
        push   0x4000
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        inc    eax
        je     loc_1_0635
        dec    eax
        mov    dword ptr [0xb78], eax        ; -> obj1:0x0B78
        mov    esi, eax
        mov    eax, dword ptr [esi + 0x1e50]
        bt     eax, 4
        jb     loc_1_0625
        bts    dword ptr [0xb74], 0x1f       ; -> obj1:0x0B74
loc_1_0625:
        bt     eax, 0x1b
        jae    loc_1_0633
        bts    dword ptr [0xb74], 0x10       ; -> obj1:0x0B74
loc_1_0633:
        jmp    loc_1_063F
loc_1_0635:
        mov    dword ptr [0xb78], 0          ; -> obj1:0x0B78
loc_1_063F:
        jmp    loc_1_0659
loc_1_0641:
        push   0
        push   0x4000
        push   0xac000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        mov    dword ptr [ebp + 0x14], eax
loc_1_0659:
        popal  
        ret    
loc_1_065B:
        call   sub_1_007D
        ret    
loc_1_0661:
        call   sub_1_007D
        ret    
loc_1_0667:
        pushal 
        mov    eax, dword ptr [edx]
        mov    esi, dword ptr [edx + 8]
        and    esi, 0xffff
        mov    edi, dword ptr [edx + 4]
        and    edi, 0xffff
        mov    edx, eax
        shr    eax, 0x10
        push   esi
        push   edi
        push   edx
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        pop    edi
        pop    esi
        inc    eax
        je     loc_1_06A9
        dec    eax
        and    edx, 0xffff
        add    edx, eax
        mov    byte ptr [edx], 0
        add    esi, eax
        add    edi, eax
        xor    dl, dl
loc_1_06A9:
        popal  
        ret    
sub_1_06AB:
        push   edx
        push   eax
        push   ebx
        mov    dx, 0x3c4
        in     al, dx
        jmp    loc_1_06B5
loc_1_06B5:
        mov    ah, al
        xor    al, 7
        out    dx, al
        jmp    loc_1_06BC
loc_1_06BC:
        mov    bl, al
        in     al, dx
        jmp    loc_1_06C1
loc_1_06C1:
        cmp    al, bl
        pushfd 
        mov    al, ah
        popfd  
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        ret    
loc_1_06CC:
        pushal 
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ax, 0x4f10
        jne    loc_1_06EE
        mov    ax, word ptr [ebp + 0x1c]
        mov    bx, word ptr [ebp + 0x10]
        call   sub_1_06F4
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x10], bx
        popal  
        stc    
        ret    
loc_1_06EE:
        popal  
        clc    
        ret    
        db  000h, 000h, 000h
sub_1_06F4:
        cmp    bl, 0
        jne    loc_1_0700
        call   sub_1_071D
        jmp    loc_1_071C
loc_1_0700:
        cmp    bl, 1
        jne    loc_1_070C
        call   sub_1_07AD
        jmp    loc_1_071C
loc_1_070C:
        cmp    bl, 2
        jne    loc_1_0718
        call   sub_1_0726
        jmp    loc_1_071C
loc_1_0718:
        mov    ax, 0
loc_1_071C:
        ret    
sub_1_071D:
        mov    ax, 0x4f
        mov    bh, 6
        mov    bl, 0x10
        ret    
sub_1_0726:
        push   edx
        call   sub_1_0732
        pop    edx
        mov    ax, 0x4f
        ret    
sub_1_0732:
        push   edi
        call   sub_1_06AB
        je     loc_1_0785
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        and    edi, 0xfffff
        cmp    edi, 0xac000
        jne    loc_1_074E
loc_1_074E:
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        mov    eax, dword ptr [edi + 0x1e50]
        and    eax, 0x400
        jne    loc_1_0785
        mov    byte ptr [edi + 0x1fd4], 0
        mov    al, byte ptr [edi + 0x1fd5]
        or     al, al
        je     loc_1_07A9
        mov    byte ptr [edi + 0x1fd4], 6
        mov    al, byte ptr [edi + 0x1fd5]
        or     al, al
        je     loc_1_07A5
        jmp    loc_1_07A1
loc_1_0785:
        mov    dx, 0x3d4
        mov    al, 0
        out    dx, al
        inc    dx
        in     al, dx
        or     al, al
        je     loc_1_07A9
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        or     al, al
        je     loc_1_07A5
loc_1_07A1:
        mov    bh, 0
        pop    edi
        ret    
loc_1_07A5:
        mov    bh, 2
        pop    edi
        ret    
loc_1_07A9:
        mov    bh, 4
        pop    edi
        ret    
sub_1_07AD:
        push   edi
        mov    al, byte ptr [0xb94]          ; -> obj1:0x0B94
        or     al, al
        jne    loc_1_07BC
        call   sub_1_0812
loc_1_07BC:
        mov    al, bh
        or     al, al
        je     loc_1_07C6
        and    al, 6
        je     loc_1_080C
loc_1_07C6:
        mov    byte ptr [0xb94], bh          ; -> obj1:0x0B94
        cmp    bh, 0
        jne    loc_1_07DC
        call   sub_1_0899
        mov    ax, 0x4f
        jmp    loc_1_0810
loc_1_07DC:
        cmp    bh, 1
        jne    loc_1_07EC
        call   sub_1_0B21
        mov    ax, 0x4f
        jmp    loc_1_0810
loc_1_07EC:
        cmp    bh, 2
        jne    loc_1_07FC
        call   sub_1_097D
        mov    ax, 0x4f
        jmp    loc_1_0810
loc_1_07FC:
        cmp    bh, 4
        jne    loc_1_080C
        call   sub_1_0A5B
        mov    ax, 0x4f
        jmp    loc_1_0810
loc_1_080C:
        mov    ax, 0
loc_1_0810:
        pop    edi
        ret    
sub_1_0812:
        push   edi
        call   sub_1_06AB
        je     loc_1_086C
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        and    edi, 0xfffff
        cmp    edi, 0xac000
        jne    loc_1_082E
loc_1_082E:
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        mov    byte ptr [edi + 0x1fd4], 0
        mov    al, byte ptr [edi + 0x1fd5]
        mov    byte ptr [0xb95], al          ; -> obj1:0x0B95
        mov    byte ptr [edi + 0x1fd4], 1
        mov    al, byte ptr [edi + 0x1fd5]
        mov    byte ptr [0xb96], al          ; -> obj1:0x0B96
        mov    byte ptr [edi + 0x1fd4], 6
        mov    al, byte ptr [edi + 0x1fd5]
        mov    byte ptr [0xb97], al          ; -> obj1:0x0B97
        pop    edi
        ret    
loc_1_086C:
        mov    dx, 0x3d4
        mov    al, 0
        out    dx, al
        inc    dx
        in     al, dx
        mov    byte ptr [0xb95], al          ; -> obj1:0x0B95
        dec    dx
        mov    al, 1
        out    dx, al
        inc    dx
        in     al, dx
        mov    byte ptr [0xb96], al          ; -> obj1:0x0B96
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        in     al, dx
        mov    byte ptr [0xb97], al          ; -> obj1:0x0B97
        dec    dx
        pop    edi
        ret    
sub_1_0899:
        push   edi
        call   sub_1_06AB
        je     loc_1_0929
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        and    edi, 0xfffff
        cmp    edi, 0xac000
        jne    loc_1_08B9
loc_1_08B9:
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        mov    byte ptr [edi + 0x1fd4], 0x11
        mov    al, byte ptr [edi + 0x1fd5]
        and    al, 0x7f
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 0
        mov    al, byte ptr [0xb95]          ; -> obj1:0x0B95
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 1
        mov    al, byte ptr [0xb96]          ; -> obj1:0x0B96
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 6
        mov    al, byte ptr [0xb97]          ; -> obj1:0x0B97
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 0x11
        mov    al, 0x80
        or     byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fc4], 1
        and    byte ptr [edi + 0x1fc5], 0xdf
        jmp    loc_1_097B
loc_1_0929:
        mov    dx, 0x3d4
        mov    al, 0x11
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0x7f
        out    dx, al
        mov    dx, 0x3d4
        mov    al, 0
        out    dx, al
        inc    dx
        mov    al, byte ptr [0xb95]          ; -> obj1:0x0B95
        out    dx, al
        dec    dx
        mov    al, 1
        out    dx, al
        inc    dx
        mov    al, byte ptr [0xb96]          ; -> obj1:0x0B96
        out    dx, al
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        mov    al, byte ptr [0xb97]          ; -> obj1:0x0B97
        out    dx, al
        dec    dx
        mov    dx, 0x3d4
        mov    al, 0x11
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 0x80
        out    dx, al
        mov    dx, 0x3c4
        mov    al, 1
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0xdf
        out    dx, al
loc_1_097B:
        pop    edi
        ret    
sub_1_097D:
        push   edi
        call   sub_1_06AB
        je     loc_1_0A0A
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        and    edi, 0xfffff
        cmp    edi, 0xac000
        jne    loc_1_099D
loc_1_099D:
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        mov    byte ptr [edi + 0x1fc4], 1
        or     byte ptr [edi + 0x1fc5], 0x20
        mov    byte ptr [edi + 0x1fd4], 0x11
        mov    al, byte ptr [edi + 0x1fd5]
        and    al, 0x7f
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 0
        mov    al, byte ptr [0xb95]          ; -> obj1:0x0B95
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 1
        mov    al, byte ptr [0xb96]          ; -> obj1:0x0B96
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 6
        xor    al, al
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 0x11
        mov    al, 0x80
        or     byte ptr [edi + 0x1fd5], al
        jmp    loc_1_0A59
loc_1_0A0A:
        mov    dx, 0x3c4
        mov    al, 1
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 0x20
        out    dx, al
        mov    dx, 0x3d4
        mov    al, 0x11
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0x7f
        out    dx, al
        mov    dx, 0x3d4
        mov    al, 0
        out    dx, al
        inc    dx
        mov    al, byte ptr [0xb95]          ; -> obj1:0x0B95
        out    dx, al
        dec    dx
        mov    al, 1
        out    dx, al
        inc    dx
        mov    al, byte ptr [0xb96]          ; -> obj1:0x0B96
        out    dx, al
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        mov    al, 0
        out    dx, al
        dec    dx
        mov    dx, 0x3d4
        mov    al, 0x11
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 0x80
        out    dx, al
loc_1_0A59:
        pop    edi
        ret    
sub_1_0A5B:
        push   edi
        call   sub_1_06AB
        je     loc_1_0ADA
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        and    edi, 0xfffff
        cmp    edi, 0xac000
        jne    loc_1_0A77
loc_1_0A77:
        mov    edi, dword ptr [0xb78]        ; -> obj1:0x0B78
        mov    byte ptr [edi + 0x1fc4], 1
        or     byte ptr [edi + 0x1fc5], 0x20
        mov    byte ptr [edi + 0x1fd4], 0x11
        mov    al, byte ptr [edi + 0x1fd5]
        and    al, 0x7f
        mov    byte ptr [edi + 0x1fd5], al
        xor    al, al
        mov    byte ptr [edi + 0x1fd4], 1
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 0
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 6
        mov    byte ptr [edi + 0x1fd5], al
        mov    byte ptr [edi + 0x1fd4], 0x11
        mov    al, 0x80
        or     byte ptr [edi + 0x1fd5], al
        jmp    loc_1_0B1F
loc_1_0ADA:
        mov    dx, 0x3c4
        mov    al, 1
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 0x20
        out    dx, al
        mov    dx, 0x3d4
        mov    al, 0x11
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0x7f
        out    dx, al
        mov    dx, 0x3d4
        mov    al, 1
        out    dx, al
        inc    dx
        xor    al, al
        out    dx, al
        dec    dx
        out    dx, al
        inc    dx
        out    dx, al
        dec    dx
        mov    al, 6
        out    dx, al
        inc    dx
        xor    al, al
        out    dx, al
        dec    dx
        mov    dx, 0x3d4
        mov    al, 0x11
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 0x80
        out    dx, al
loc_1_0B1F:
        pop    edi
        ret    
sub_1_0B21:
        ret    
        db  000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'MGA     '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  MGA_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x88  R-X
loc_2_0000:
        mov    dword ptr [0xb8c], ebx        ; -> obj1:0x0B8C
        mov    dword ptr [0xb78], 0          ; -> obj1:0x0B78
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x30
        jb     loc_2_0086
        mov    dword ptr [edi], 0x548        ; -> loc_1_0548
        mov    dword ptr [edi + 0x10], 0x65b ; -> loc_1_065B
        mov    dword ptr [edi + 0x68], 0x661 ; -> loc_1_0661
        mov    dword ptr [edi + 0xa4], 0x6cc ; -> loc_1_06CC
        mov    ecx, 0                        ; -> loc_3_0000
        mov    edx, 0x3de
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        jb     loc_2_0083
        mov    ecx, 0                        ; -> loc_3_0000
        mov    edx, 0x3df
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        jb     loc_2_0083
        push   edx
        mov    dx, 0x3de
        in     al, dx
        jmp    loc_2_0065
loc_2_0065:
        mov    ah, al
        mov    al, 7
        out    dx, al
        jmp    loc_2_006C
loc_2_006C:
        inc    dx
        in     al, dx
        jmp    loc_2_0071
loc_2_0071:
        and    eax, 0xffff
        mov    dword ptr [0xb90], eax        ; -> obj1:0x0B90
        mov    al, ah
        dec    dx
        out    dx, al
        jmp    loc_2_0082
loc_2_0082:
        pop    edx
loc_2_0083:
        clc    
        jmp    loc_2_0087
loc_2_0086:
        stc    
loc_2_0087:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x40  R-X
loc_3_0000:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0xb8c]        ; -> obj1:0x0B8C
        jne    loc_3_001B
        cmp    ebx, edi
        je     loc_3_001B
        mov    eax, dword ptr [0xb90]        ; -> obj1:0x0B90
        jmp    loc_3_003D
loc_3_001B:
        cmp    ecx, 4
        je     loc_3_0033
        cmp    ecx, 0
        je     loc_3_003A
        cmp    ecx, 0xc
        je     loc_3_002F
        cmp    ecx, 8
        je     loc_3_0036
loc_3_002F:
        out    dx, ax
        jmp    loc_3_003D
loc_3_0033:
        out    dx, al
        jmp    loc_3_003D
loc_3_0036:
        in     ax, dx
        jmp    loc_3_003D
loc_3_003A:
        in     al, dx
        jmp    loc_3_003D
loc_3_003D:
        pop    esi
        pop    edi
        ret    
VxD__LTEXT ENDS

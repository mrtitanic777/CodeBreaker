; DBKVSSD -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 5190 instructions recovered by recursive descent

Declare_Virtual_Device DBKVSSD, 1, 0, DBKVSSD_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 016810009h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x37b8  R-X
DBKVSSD_Control:
        cmp    eax, 0x22
        jne    loc_1_000E
        push   edx
        push   ebx
        call   sub_1_1A00
        stc    
        ret    
loc_1_000E:
        clc    
        ret    
sub_1_0010:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    esi, dword ptr [ebp + 8]
        pushfd 
        inc    byte ptr [esi + 6]
        push   ebx
        push   ebp
        mov    ebp, esp
        push   esi
        push   eax
        push   ecx
        push   edx
        push   edi
        lea    esp, [ebp - 0x24]
        mov    byte ptr [ebp - 0x24], 0
        cld    
        mov    dword ptr [ebp - 0x1c], 0
        mov    dx, word ptr [esi + 4]
        mov    word ptr [0x3238], dx         ; -> obj1:0x3238
        push   esi
        call   sub_1_0B37
        movzx  eax, ax
        mov    dword ptr [0x323a], eax       ; -> obj1:0x323A
        pop    ebx
        call   sub_1_113E
        pushf  
        call   sub_1_0955
        mov    esi, dword ptr [ebp - 4]
        dec    byte ptr [esi + 6]
        popf   
        lea    esp, [ebp - 0x14]
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        pop    ebp
        pop    ebx
        popfd  
        popal  
        mov    eax, dword ptr [0x323a]       ; -> obj1:0x323A
        leave  
        ret    
sub_1_0074:
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    eax, 0xd6d8
        movzx  ecx, cx
        mul    ecx
        push   0
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Yield        ; cd2015003300
        pop    ecx
        pop    ecx
        pop    edx
        pop    ebx
        pop    ecx
        pop    eax
        ret    
sub_1_0092:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        sub    esp, 8
        mov    ebx, esp
        mov    word ptr [ebx], 3
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xa], 0x44425353
        mov    dword ptr [ebx + 2], esi
        lea    esi, [0x107c]                 ; -> loc_1_107C
        xor    edx, edx
        mov    ecx, 6
        mov    eax, 0x32
        VxDCall  Dev_097C.Service_0001h          ; cd2001007c09
        add    esp, 8
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
        db  000h, 000h, 000h, 000h
loc_1_00D3:
        push   eax
        mov    eax, dword ptr [0xcf]         ; -> obj1:0x00CF
        cmp    eax, 8
        jb     loc_1_00E7
        sub    eax, 8
        or     al, 0x60
        out    0xa0, al
        mov    al, 2
loc_1_00E7:
        or     al, 0x60
        out    0x20, al
        mov    al, 0xff
        out    0xa1, al
        out    0x21, al
        pop    eax
        iretd  
sub_1_00F3:
        push   ebx
        push   esi
        sub    esp, 8
        sidt   [esp + 2]
        pop    ebx
        pop    ebx
        mov    dword ptr [0xcf], eax         ; -> obj1:0x00CF
        shl    eax, 3
        add    ebx, eax
        add    ebx, 0x280
        mov    esi, 0xd3                     ; -> loc_1_00D3
        xchg   word ptr [ebx], si
        ror    esi, 0x10
        xchg   word ptr [ebx + 6], si
        ror    esi, 0x10
        sti    
        nop    
        nop    
        nop    
        cli    
        mov    word ptr [ebx], si
        ror    esi, 0x10
        mov    word ptr [ebx + 6], si
        pop    esi
        pop    ebx
        ret    
sub_1_0133:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        mov    bx, word ptr [ebp + 8]
        or     bx, bx
        je     loc_1_0189
        mov    ecx, 1
loc_1_0147:
        test   cl, bl
        je     loc_1_015D
        mov    al, cl
        not    al
        out    0x21, al
        bsf    eax, ecx
        call   sub_1_00F3
        mov    al, 0xff
        out    0x21, al
loc_1_015D:
        shl    cl, 1
        jne    loc_1_0147
        mov    bl, bh
        mov    cl, 1
loc_1_0165:
        test   cl, bl
        je     loc_1_0185
        mov    al, 0xfb
        out    0x21, al
        mov    al, cl
        not    al
        out    0xa1, al
        xchg   ch, cl
        bsf    eax, ecx
        call   sub_1_00F3
        xchg   ch, cl
        mov    al, 0xff
        out    0xa1, al
        out    0x21, al
loc_1_0185:
        shl    cl, 1
        jne    loc_1_0165
loc_1_0189:
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
        db  000h, 000h
sub_1_0190:
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        mov    al, 0x80
        add    dx, 6
        out    dx, al
        xor    ax, ax
        out    dx, al
        add    dx, -6
        mov    al, 0xa0
        call   sub_1_0E4E
        mov    ax, word ptr [ebx + 0x4c]
        and    ax, 0xfff0
        add    dx, 0xe
        out    dx, al
        out    dx, ax
        add    dx, -0xe
        xor    ax, ax
        mov    al, byte ptr [ebx + 0x2c]
        call   sub_1_0F2C
        call   sub_1_042C
        mov    ax, 0x1000
        call   sub_1_098C
        xor    ax, ax
        call   sub_1_098C
        xor    ax, ax
        call   sub_1_082C
        mov    al, 0x60
        call   sub_1_0E4E
        add    dx, 0xe
        mov    ax, 0
        out    dx, ax
        mov    ax, word ptr [ebx + 0x52]
        mov    ah, al
        mov    al, 1
        test   ah, 0x10
        jne    loc_1_0209
        or     al, 3
loc_1_0209:
        dec    dx
        out    dx, al
        ret    
sub_1_020D:
        call   sub_1_0B2D
        push   dx
        add    dx, 4
        in     ax, dx
        and    ax, 0x7000
        mov    cl, 0xc
        shr    ax, cl
        mov    cx, ax
        pop    dx
        ret    
sub_1_0229:
        call   sub_1_0190
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        add    dx, 8
        mov    al, 0xe0
        out    dx, al
        add    dx, 6
        mov    ax, 0x8000
        out    dx, ax
        add    dx, -6
        mov    al, 0xc0
        out    dx, al
        add    dx, 6
        in     ax, dx
        push   ax
        add    dx, -6
        mov    al, 0xe0
        out    dx, al
        xor    ax, ax
        add    dx, 6
        out    dx, ax
        add    dx, -0xe
        pop    ax
        and    ax, 0x7f00
        shr    ax, 8
        cmp    ax, word ptr [ebx + 8]
        jne    loc_1_0287
        cmp    al, 4
        je     loc_1_0290
        cmp    al, 8
        je     loc_1_0290
        cmp    al, 0x14
        je     loc_1_0290
        jmp    loc_1_02E3
loc_1_0287:
        mov    ax, 0x8080
        jmp    loc_1_0376
loc_1_0290:
        inc    dx
        inc    dx
        mov    ax, 0x300
        out    dx, ax
        inc    dx
        inc    dx
        in     ax, dx
        and    ax, 0x7000
        or     ax, 0x800
        out    dx, ax
        add    dx, -4
        in     ax, dx
        test   ax, 0x4000
        je     loc_1_02B8
        jmp    loc_1_0287
loc_1_02B8:
        lea    esi, [ebx + 0x38]
        test   ax, 0x100
        je     loc_1_02C5
        mov    byte ptr [esi + 0xf], 0xf
loc_1_02C5:
        test   ax, 0x80
        je     loc_1_02CF
        mov    byte ptr [esi + 0xc], 0xc
loc_1_02CF:
        test   ax, 0x40
        je     loc_1_02D9
        mov    byte ptr [esi + 9], 9
loc_1_02D9:
        test   ax, 0x20
        je     loc_1_02E3
        mov    byte ptr [esi + 6], 6
loc_1_02E3:
        xor    ax, ax
        inc    dx
        inc    dx
        out    dx, ax
        inc    dx
        inc    dx
        push   bx
        mov    bx, ax
        in     ax, dx
        and    ax, 0x7000
        or     ax, bx
        pop    bx
        out    dx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x48]
        add    dx, 5
        out    dx, al
        or     al, 0x20
        out    dx, al
        mov    cx, 2
        call   sub_1_0074
        dec    dx
        in     al, dx
        and    al, 0x1f
        or     al, 0xe0
        out    dx, al
        xor    ax, ax
        add    dx, 6
        out    dx, ax
        mov    al, 0x18
        add    dx, -8
        out    dx, al
        xor    ax, ax
        mov    cx, ax
        add    dx, -4
        out    dx, ax
        inc    dx
        inc    dx
        push   bx
        mov    bx, ax
        in     ax, dx
        and    ax, 0x7000
        or     ax, bx
        pop    bx
        out    dx, ax
        dec    dx
        dec    dx
loc_1_0357:
        dec    dx
        dec    dx
        out    dx, al
        inc    cx
        inc    dx
        inc    dx
        in     al, dx
        cmp    al, cl
        jne    loc_1_036F
        cmp    cx, 0xa
        jb     loc_1_0357
        jmp    loc_1_0373
loc_1_036F:
        mov    ah, 3
        jmp    loc_1_0376
loc_1_0373:
        xor    ax, ax
loc_1_0376:
        push   ax
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        call   sub_1_0190
        pop    ax
        ret    
sub_1_0387:
        push   cx
        mov    byte ptr [ebx + 0x11], 0
        mov    byte ptr [ebx + 0xd], 1
        mov    byte ptr [ebx + 0xc], 0x40
        add    byte ptr [ebx + 0xc], 0
        mov    cl, byte ptr [ebx + 0xe]
        mov    ax, 8
        mul    cl
        mov    word ptr [ebx + 4], ax
        push   esi
        mov    esi, dword ptr [ebp - 4]
        mov    ax, word ptr [esi + 0x52]
        pop    esi
        mov    word ptr [ebx + 6], ax
        pop    cx
        ret    
sub_1_03B6:
        push   esi
        push   ecx
        xor    ecx, ecx
        mov    cl, byte ptr [ebx + 0x1a]
        mov    esi, dword ptr [ebp - 4]
        cmp    cl, byte ptr [esi + 0x2f]
        jb     loc_1_0400
        sub    cl, byte ptr [esi + 0x2f]
        mov    ax, 8
        mul    cl
        add    ax, 0x100
        mov    word ptr [ebx + 0x10], ax
        mov    ax, word ptr [esi + 0x50]
        and    ax, 0x1f
        mov    byte ptr [ebx + 0x1b], 0x32
        mov    byte ptr [ebx + 0x1c], 6
        mov    word ptr [ebx + 0xa], ax
        mov    word ptr [ebx], 1
        mov    eax, 0x3272                   ; -> loc_1_3272
        add    eax, 2
        mov    dword ptr [ebx + 0x12], eax
        mov    byte ptr [ebx + 6], 4
        jmp    loc_1_0429
loc_1_0400:
        mov    ax, 4
        mul    cl
        add    ax, 0x200
        mov    word ptr [ebx + 0xe], ax
        mov    ax, word ptr [esi + 0x50]
        and    ax, 0x1f
        mov    word ptr [ebx + 0xa], ax
        mov    eax, 0x326a                   ; -> loc_1_326A
        add    eax, 2
        mov    dword ptr [ebx + 0x12], eax
        mov    byte ptr [ebx + 6], 5
loc_1_0429:
        pop    ecx
        pop    esi
        ret    
sub_1_042C:
        push   ebx
        push   edx
        push   ecx
        push   esi
        mov    esi, dword ptr [ebp - 4]
        mov    dx, word ptr [esi + 4]
        mov    ax, 0
        call   sub_1_082C
        mov    bx, 0x78
        mov    ax, word ptr [esi + 0x52]
        out    dx, ax
        xchg   bx, ax
        out    dx, ax
        xchg   bx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        xchg   bx, ax
        out    dx, ax
        inc    dx
        inc    dx
        mov    ax, 0x100
        out    dx, ax
        mov    ecx, 8
        dec    dx
        dec    dx
        mov    bx, word ptr [esi + 0x50]
        xor    ax, ax
loc_1_0474:
        out    dx, ax
        out    dx, ax
        out    dx, ax
        xchg   bx, ax
        out    dx, ax
        xchg   bx, ax
        loop   loc_1_0474
        inc    dx
        inc    dx
        mov    ax, 0x200
        out    dx, ax
        mov    ecx, 4
        shl    cx, 1
        dec    dx
        dec    dx
        xor    ax, ax
loc_1_049B:
        out    dx, ax
        loop   loc_1_049B
        pop    esi
        pop    ecx
        pop    edx
        pop    ebx
        ret    
loc_1_04A4:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 0x18]
        mov    dx, word ptr [ebx + 2]
        mov    cx, word ptr [ebx]
        or     al, al
        jne    loc_1_04C7
        xor    cx, cx
        mov    dx, cx
        jmp    loc_1_053C
        db  0E9h, 0A8h, 000h, 000h, 000h
loc_1_04C7:
        shr    cx, 1
        or     cx, dx
        je     loc_1_053C
        xor    dx, dx
        jcxz   loc_1_04E4
        mov    dx, 0x840
        test   byte ptr [ebx + 6], 4
        je     loc_1_04E4
        and    dx, 0xf7bf
loc_1_04E4:
        push   eax
        push   ebx
        push   ecx
        mov    ecx, edx
        mov    ebx, dword ptr [ebp - 0x1c]
        mov    dx, word ptr [esi + 4]
        movzx  eax, word ptr [ebx + 4]
        add    eax, 2
        call   sub_1_082C
        movzx  eax, word ptr [ebx + 8]
        and    eax, 0xfffffff9
        test   ecx, 0x800
        jne    loc_1_050E
        or     eax, 2
loc_1_050E:
        mov    word ptr [ebx + 8], ax
        out    dx, ax
        mov    edx, ecx
        pop    ecx
        pop    ebx
        pop    eax
        test   byte ptr [ebx + 6], 2
        je     loc_1_0524
        or     dx, 0x8000
loc_1_0524:
        cmp    word ptr [ebx], 1
        jne    loc_1_052F
        or     dx, 0x200
loc_1_052F:
        test   word ptr [ebx + 0x18], 1
        je     loc_1_053C
        or     dx, 0x1000
loc_1_053C:
        mov    word ptr [ebx + 8], cx
        mov    ax, dx
        mov    dx, word ptr [esi + 4]
        mov    si, bx
        mov    bx, ax
        jcxz   loc_1_0558
        call   sub_1_0576
        or     bx, ax
loc_1_0558:
        mov    word ptr [esi + 0xa], bx
        mov    ax, word ptr [esi + 0xe]
        call   sub_1_082C
        mov    ax, cx
        out    dx, ax
        mov    ax, bx
        out    dx, ax
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        clc    
        ret    
sub_1_0576:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        movzx  ebx, word ptr [ebx + 0x54]
        mov    eax, 4
        sub    eax, ebx
        pop    ebx
        ret    
sub_1_0587:
        push   eax
        push   edi
        mov    edi, dword ptr [ebp - 4]
        mov    dx, word ptr [edi + 4]
        call   sub_1_0801
        cmp    byte ptr [ebp - 0x24], 0
        jne    loc_1_05AA
        mov    edi, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [edi + 0xf]
        add    dx, 6
        out    dx, al
        add    dx, -6
loc_1_05AA:
        pop    edi
        pop    eax
        ret    
sub_1_05AD:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 0x18]
        mov    dx, word ptr [ebx + 2]
        mov    cx, word ptr [ebx]
        or     al, al
        jne    loc_1_05D1
        xor    cx, cx
        mov    dx, cx
        jmp    loc_1_0610
        db  0E9h, 031h, 001h, 000h, 000h
loc_1_05D1:
        xor    ax, ax
        shr    cx, 1
        rcr    ax, 1
        shr    ax, 1
        or     cx, ax
        or     cx, dx
        xor    dx, dx
        jcxz   loc_1_0610
        test   byte ptr [ebx + 6], 4
        jne    loc_1_05F4
        or     dx, 0x800
loc_1_05F4:
        or     dx, 0x40
        test   byte ptr [ebx + 6], 2
        je     loc_1_0603
        or     dx, 0x8000
loc_1_0603:
        test   word ptr [ebx + 0x18], 1
        je     loc_1_0610
        or     dx, 0x1000
loc_1_0610:
        mov    word ptr [ebx + 8], cx
        mov    ax, dx
        mov    dx, word ptr [esi + 4]
        mov    esi, ebx
        mov    bx, ax
        xor    ax, ax
        jcxz   loc_1_062B
        mov    al, byte ptr [esi + 0x1c]
        xor    ah, ah
loc_1_062B:
        or     bx, ax
        mov    word ptr [esi + 0xa], bx
        mov    cx, word ptr [esi + 2]
        mov    ax, word ptr [esi + 4]
        and    cx, 0xfff
        sub    ax, cx
        and    ax, 0x3fff
        mov    cl, byte ptr [esi + 7]
        shl    ax, 1
        shr    cl, 1
        rcr    ax, 1
        push   esi
        mov    esi, dword ptr [ebp - 4]
        cmp    word ptr [esi + 8], 2
        pop    esi
        jne    loc_1_067E
        test   word ptr [esi + 0xa], 0x8000
        je     loc_1_067E
        push   eax
        add    dx, 8
        mov    al, 0xa0
        out    dx, al
        add    dx, 6
        in     ax, dx
        or     ax, 0x10
        out    dx, ax
        add    dx, -0xe
        pop    eax
loc_1_067E:
        mov    word ptr [esi + 0xc], ax
        mov    cx, word ptr [esi + 8]
        push   eax
        mov    ax, word ptr [esi + 0x10]
        call   sub_1_082C
        xor    ax, ax
        out    dx, ax
        mov    ax, cx
        out    dx, ax
        pop    eax
        out    dx, ax
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        cmp    word ptr [ebx + 8], 2
        pop    ebx
        jne    loc_1_06F1
        push   bx
        and    bx, 0xffe0
        add    dx, 6
        mov    al, 1
        out    dx, al
        add    dx, -6
        in     ax, dx
        and    ax, 0x1f
        or     ax, bx
        out    dx, ax
        movzx  eax, byte ptr [esi + 0x1a]
        sub    al, 4
        not    al
        and    al, 7
        add    al, 4
        mov    cl, 0x1e
        mul    cl
        mov    esi, dword ptr [ebp - 4]
        mov    esi, dword ptr [esi + 0x1a]
        add    esi, eax
        mov    ax, word ptr [esi + 0x10]
        add    ax, 6
        inc    dx
        inc    dx
        out    dx, ax
        pop    bx
        dec    dx
        dec    dx
loc_1_06F1:
        and    bx, 0x1f
        mov    ax, word ptr [esi + 0xa]
        and    ax, 0xffe0
        or     ax, bx
        out    dx, ax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        clc    
        ret    
loc_1_070A:
        clc    
        or     al, al
        je     loc_1_0712
        mov    ah, 2
        stc    
loc_1_0712:
        ret    
loc_1_0713:
        mov    ah, 2
        stc    
        ret    
loc_1_0717:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        and    byte ptr [ebx + 0x2a], 0xfe
        or     al, al
        je     loc_1_0727
        or     byte ptr [ebx + 0x2a], 1
loc_1_0727:
        pop    ebx
        clc    
        ret    
loc_1_072A:
        stc    
        ret    
loc_1_072C:
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    ch, al
        call   sub_1_0B2D
        mov    ebx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebx + 0x52]
        mov    ah, 1
        test   al, 0x10
        jne    loc_1_0747
        or     ah, 3
loc_1_0747:
        add    dx, 0xd
        in     al, dx
        or     al, ah
        mov    cl, al
        and    cl, 3
        mov    ah, byte ptr [ebx + 0x2c]
        and    ah, 0x7f
        test   ch, ch
        je     loc_1_0763
        or     ah, 0x80
        or     cl, 0x60
loc_1_0763:
        mov    byte ptr [ebx + 0x2c], ah
        test   al, 0x60
        jne    loc_1_0779
        test   cl, 0x60
        je     loc_1_0779
        dec    dx
        in     al, dx
        and    al, 0x9f
        or     al, 1
        out    dx, al
        inc    dx
loc_1_0779:
        push   cx
        mov    cl, 1
        dec    dx
        mov    al, cl
        out    dx, al
        add    dx, -8
        in     ax, dx
        mov    bx, ax
        xor    bx, 0x1000
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
        add    dx, 8
        mov    al, cl
        out    dx, al
        xor    bx, 0x1000
        add    dx, -8
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
        pop    cx
        add    dx, 9
        mov    al, cl
        out    dx, al
        mov    ebx, dword ptr [ebp - 4]
        cmp    byte ptr [ebx + 0x26], 1
        je     loc_1_07FC
        add    dx, -9
        in     ax, dx
        mov    bx, ax
        xor    bx, 0x1000
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
        add    dx, 9
        mov    al, cl
        out    dx, al
        xor    bx, 0x1000
        add    dx, -9
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
loc_1_07FC:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_1_0801:
        push   edx
        push   ebx
        xor    ax, ax
        mov    ebx, dword ptr [ebp - 0x1c]
        cmp    byte ptr [ebx + 0xe], 0
        je     loc_1_0813
        mov    ax, 0x1000
loc_1_0813:
        add    dx, 4
        push   bx
        mov    bx, ax
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        pop    bx
        out    dx, ax
        pop    ebx
        pop    edx
        ret    
sub_1_082C:
        push   edx
        push   eax
        mov    ax, 1
        call   sub_1_0E5D
        add    dx, 4
        in     ax, dx
        and    ax, 0x7000
        or     ax, 0x800
        out    dx, ax
        pop    eax
        dec    dx
        dec    dx
        out    dx, ax
        pop    edx
        ret    
        db  056h
sub_1_0851:
        push   edx
        push   ecx
        push   ebx
        call   sub_1_0B2D
        mov    cx, ax
        and    ax, 0xf
        mov    ebx, dword ptr [ebp - 4]
        lea    ebx, [ebx + 0x38]
        xlatb  
        test   al, 0xf0
        je     loc_1_087D
        mov    al, 1
        or     al, al
        je     loc_1_0878
        xor    al, al
        test   cl, 0x80
        je     loc_1_087D
loc_1_0878:
        mov    ah, 6
        stc    
        jmp    loc_1_08A7
loc_1_087D:
        mov    ebx, dword ptr [ebp - 0x1c]
        and    word ptr [ebx + 6], 0xffb0
        test   cl, 0x40
        jne    loc_1_088E
        or     ax, 0x40
loc_1_088E:
        test   cl, 0x80
        je     loc_1_0897
        or     word ptr [ebx + 6], ax
loc_1_0897:
        mov    ax, word ptr [ebx + 4]
        call   sub_1_082C
        mov    ax, word ptr [ebx + 6]
        out    dx, ax
        clc    
loc_1_08A7:
        pop    ebx
        pop    ecx
        pop    edx
        ret    
sub_1_08AB:
        push   ebx
        push   edx
        push   eax
        call   sub_1_0B2D
        mov    ebx, dword ptr [ebp - 0x1c]
        cmp    ax, 2
        je     loc_1_08D8
        cmp    ax, 1
        je     loc_1_08C9
        pop    ax
        mov    ah, 0xd
        stc    
        jmp    loc_1_08F5
loc_1_08C9:
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x52]
        mov    word ptr [ebx + 6], ax
        pop    eax
        jmp    loc_1_08DE
loc_1_08D8:
        or     word ptr [ebx + 6], 0x3800
loc_1_08DE:
        mov    ax, word ptr [ebx + 4]
        call   sub_1_082C
        mov    ax, word ptr [ebx + 6]
        out    dx, ax
        mov    ax, word ptr [ebx + 8]
        out    dx, ax
        clc    
        pop    eax
loc_1_08F5:
        pop    edx
        pop    ebx
        ret    
        db  053h, 052h, 08Bh, 05Dh, 0E4h, 066h, 0C7h, 043h, 008h, 000h, 000h, 066h, 08Bh, 043h, 004h, 0E8h
        db  020h, 0FFh, 0FFh, 0FFh, 066h, 08Bh, 043h, 006h, 066h, 0EFh, 066h, 08Bh, 043h, 008h, 066h, 0EFh
        db  05Ah, 05Bh, 0F8h, 0C3h
sub_1_091C:
        push   ecx
        push   edx
        push   eax
        call   sub_1_0B2D
        call   sub_1_020D
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 8]
        mov    ch, al
        pop    eax
        cmp    ch, 4
        je     loc_1_0946
        cmp    ch, 8
        je     loc_1_0946
        cmp    ch, 0x14
        je     loc_1_0946
        shl    al, cl
        jmp    loc_1_094C
loc_1_0946:
        or     al, al
        je     loc_1_094C
        or     al, 0x20
loc_1_094C:
        add    dx, 9
        out    dx, al
        pop    edx
        pop    ecx
        clc    
        ret    
sub_1_0955:
        push   ebx
        push   eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        mov    ebx, dword ptr [ebp - 0x1c]
        or     ebx, ebx
        je     loc_1_0989
        mov    ebx, dword ptr [ebp - 4]
        cmp    byte ptr [ebx + 6], 1
        ja     loc_1_0989
        add    dx, 7
        in     al, dx
        test   al, 0x80
        jne    loc_1_0989
        add    dx, -7
        xor    eax, eax
        call   sub_1_082C
        add    dx, 6
        mov    al, 0
        out    dx, al
loc_1_0989:
        pop    eax
        pop    ebx
        ret    
sub_1_098C:
        push   edx
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        push   eax
        mov    al, 0x40
        call   sub_1_0E4E
        pop    eax
        add    dx, 4
        push   bx
        mov    bx, ax
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        pop    bx
        out    dx, ax
        mov    ax, word ptr [ebx + 0x4a]
        or     eax, 7
        add    dx, 0xa
        out    dx, ax
        add    dx, -6
        mov    al, 0xc0
        out    dx, al
        mov    ax, word ptr [ebx + 0x4e]
        add    dx, 6
        out    dx, ax
        pop    ebx
        pop    edx
        ret    
sub_1_09D1:
        push   ebx
        push   esi
        push   edx
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 0x1c]
        mov    byte ptr [ebx + 0xd], 1
        and    byte ptr [ebx + 0xc], 0x7f
        mov    word ptr [ebx + 0xa], 0
        and    byte ptr [ebx + 0xf], 0x20
        call   sub_1_0B2D
        mov    al, 0xc0
        call   sub_1_0E4E
        mov    ax, word ptr [esi + 0x4e]
        add    dx, 0xe
        out    dx, ax
        add    dx, -0xe
        mov    ebx, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [ebx + 4]
        add    ax, 2
        call   sub_1_082C
        in     ax, dx
        and    ax, 0xf8
        push   eax
        mov    si, word ptr [esi + 0x52]
        mov    word ptr [ebx + 6], si
        push   esi
        call   sub_1_0A4F
        mov    ax, word ptr [ebx + 4]
        call   sub_1_082C
        pop    esi
        mov    ax, si
        out    dx, ax
        pop    eax
        out    dx, ax
        mov    word ptr [ebx + 0x18], 0
        add    dx, 9
        mov    al, 0
        out    dx, al
        pop    edx
        pop    esi
        pop    ebx
        clc    
        ret    
sub_1_0A4F:
        push   ebx
        push   esi
        push   edi
        push   cx
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 0x1c]
        mov    bl, byte ptr [ebx + 0xe]
        xor    bh, bh
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x27]
        mov    esi, dword ptr [esi + 0x1a]
loc_1_0A67:
        push   ecx
        cmp    word ptr [esi + 0x18], bx
        jne    loc_1_0AAC
        test   word ptr [esi + 0xa], 0x8000
        je     loc_1_0AAC
        mov    dword ptr [ebp - 0x18], esi
        mov    word ptr [esi], 1
        mov    word ptr [esi + 2], 0
        mov    word ptr [esi + 4], 0
        and    byte ptr [esi + 6], 0xfd
        and    byte ptr [esi + 7], 0xfd
        mov    edi, 0x4a4                    ; -> loc_1_04A4
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 0x2f]
        cmp    byte ptr [esi + 0x1a], al
        jb     loc_1_0AA8
        mov    edi, 0x5ad                    ; -> sub_1_05AD
loc_1_0AA8:
        xor    eax, eax
        call   edi
loc_1_0AAC:
        pop    ecx
        add    esi, 0x1e
        loop   loc_1_0A67
        pop    cx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0AB8:
        push   ebx
        mov    ebx, 0xff19
        call   sub_1_0C9B
        mov    ebx, dword ptr [ebp - 0x1c]
        or     byte ptr [ebx + 0xf], 1
        pop    ebx
        ret    
loc_1_0ACC:
        call   sub_1_1276
        jb     loc_1_0B2C
        mov    ebx, dword ptr [ebp - 0x18]
        mov    ebx, dword ptr [ebx + 0x12]
        mov    dx, word ptr [ebx]
        mov    byte ptr [ebp + 4], dl
        mov    esi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x26]
        cmp    eax, 1
        je     loc_1_0AEF
        or     eax, 1
loc_1_0AEF:
        mov    word ptr [ebp - 0xc], ax
        mov    al, dl
        mov    edi, dword ptr [ebp - 0x14]
        movzx  ecx, word ptr [edi]
        xor    dx, dx
        jcxz   loc_1_0B26
        test   al, 4
        mov    ax, 0x12
        je     loc_1_0B0D
        mov    ax, 0xe
loc_1_0B0D:
        mov    esi, dword ptr [ebx + 2]
        add    esi, 2
        cmp    cx, ax
        jbe    loc_1_0B1B
        movzx  ecx, ax
loc_1_0B1B:
        mov    dx, cx
        push   edi
        add    edi, 4
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
loc_1_0B26:
        mov    word ptr [edi + 2], dx
        mov    ah, 0
loc_1_0B2C:
        ret    
sub_1_0B2D:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        pop    ebx
        ret    
sub_1_0B37:
        call   sub_1_0229
        cmp    ah, 0
        je     loc_1_0B47
        mov    ax, 0xffff
        jmp    loc_1_0B4A
loc_1_0B47:
        xor    ax, ax
loc_1_0B4A:
        ret    
loc_1_0B4B:
        call   sub_1_1276
        jb     loc_1_0B79
        mov    ebx, dword ptr [ebp - 0x18]
        mov    ax, word ptr [ebx + 0x18]
        mov    byte ptr [ebp + 4], al
        mov    ax, word ptr [ebx]
        mov    word ptr [ebp - 0xc], ax
        mov    al, byte ptr [ebx + 6]
        mov    byte ptr [ebp - 0xf], al
        mov    ax, word ptr [ebx + 2]
        mov    word ptr [ebp - 0x14], ax
        mov    al, byte ptr [ebx + 0x1b]
        mov    byte ptr [ebp - 0x10], al
        mov    ah, 0
loc_1_0B79:
        ret    
loc_1_0B7A:
        call   sub_1_1276
        jb     loc_1_0BC1
        mov    esi, dword ptr [ebp - 0x18]
        mov    dh, byte ptr [ebp - 0xf]
        test   dh, 0xf8
        jne    loc_1_0BBF
        mov    dx, word ptr [ebp - 0x14]
        mov    cx, word ptr [ebp - 0xc]
        call   sub_1_0FED
        jae    loc_1_0BA0
        jmp    loc_1_0C9A
loc_1_0BA0:
        mov    dh, byte ptr [ebp - 0xf]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 0x2f]
        cmp    byte ptr [esi + 0x1a], al
        pop    eax
        jae    loc_1_0BB3
        not    dh
loc_1_0BB3:
        test   dh, 1
        je     loc_1_0BC6
        mov    ah, 0xd
        jmp    loc_1_0C9A
loc_1_0BBF:
        mov    ah, 2
loc_1_0BC1:
        jmp    loc_1_0C9A
loc_1_0BC6:
        mov    al, byte ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 0x2f]
        cmp    byte ptr [esi + 0x1a], al
        pop    eax
        jae    loc_1_0BD9
        mov    al, 0xa
loc_1_0BD9:
        call   sub_1_0D67
        jb     loc_1_0BC1
        mov    byte ptr [esi + 0x1b], al
        mov    byte ptr [esi + 0x1c], ah
        xor    ah, ah
        mov    al, byte ptr [ebp + 4]
        cmp    ax, word ptr [esi + 0x18]
        jne    loc_1_0C16
        mov    cx, word ptr [ebp - 0xc]
        cmp    cx, word ptr [esi]
        jne    loc_1_0C16
        mov    al, byte ptr [ebp - 0xf]
        cmp    al, byte ptr [esi + 6]
        jne    loc_1_0C16
        mov    di, word ptr [ebp - 0x14]
        cmp    di, word ptr [esi + 2]
        jne    loc_1_0C16
        test   byte ptr [esi + 6], 2
        je     loc_1_0C98
loc_1_0C16:
        and    byte ptr [esi + 7], 0xfd
        mov    al, byte ptr [ebp - 0xf]
        test   al, 2
        je     loc_1_0C25
        or     byte ptr [esi + 7], 2
loc_1_0C25:
        mov    al, byte ptr [ebp - 0xf]
        mov    byte ptr [esi + 6], al
        mov    cx, word ptr [ebp - 0xc]
        mov    word ptr [esi], cx
        mov    di, word ptr [ebp - 0x14]
        mov    word ptr [esi + 2], di
        mov    bl, byte ptr [ebp + 4]
        mov    bh, 0
        mov    word ptr [esi + 0x18], bx
        mov    ah, byte ptr [ebp + 5]
        mov    al, 1
        mov    ebx, esi
        mov    esi, 0x4a4                    ; -> loc_1_04A4
        push   eax
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 0x2f]
        cmp    byte ptr [ebx + 0x1a], al
        pop    eax
        jb     loc_1_0C62
        mov    esi, 0x5ad                    ; -> sub_1_05AD
loc_1_0C62:
        call   esi
        mov    ax, 1
        mov    cl, byte ptr [ebx + 0x1a]
        shl    ax, cl
        mov    dx, ax
        not    dx
        test   byte ptr [ebx + 6], 2
        jne    loc_1_0C7D
        xor    ax, ax
loc_1_0C7D:
        mov    esi, dword ptr [ebp - 4]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x26]
        mov    esi, dword ptr [esi + 0x16]
loc_1_0C88:
        and    word ptr [esi + 0xa], dx
        add    esi, 0x1a
        loop   loc_1_0C88
        mov    esi, dword ptr [ebp - 0x1c]
        or     word ptr [esi + 0xa], ax
loc_1_0C98:
        mov    ah, 0
loc_1_0C9A:
        ret    
sub_1_0C9B:
        push   edx
        add    dx, 6
        in     al, dx
        and    al, bh
        or     al, bl
        out    dx, al
        pop    edx
        ret    
sub_1_0CA8:
        push   edx
        push   ebx
        call   sub_1_0B2D
        mov    ebx, dword ptr [ebp - 0x1c]
        add    dx, 7
        in     al, dx
        dec    dx
        call   sub_1_0D14
        test   eax, 0x80
        jne    loc_1_0CCA
        xor    eax, eax
        out    dx, al
        jmp    loc_1_0CDB
loc_1_0CCA:
        mov    ah, al
        add    dx, 3
        in     al, dx
        and    al, 0x1b
        jne    loc_1_0CDB
        and    ah, 0x80
        or     ah, 0x40
loc_1_0CDB:
        mov    al, ah
        pop    ebx
        pop    edx
        ret    
sub_1_0CE0:
        call   sub_1_0CA8
        push   edx
        push   esi
        push   ax
        call   sub_1_0B2D
        add    dx, 0xc
        in     ax, dx
        test   ah, 3
        je     loc_1_0CFE
        test   ah, 0x40
        je     loc_1_0CFE
loc_1_0CFE:
        mov    esi, dword ptr [ebp - 0x1c]
        mov    al, 0xbd
        out    dx, al
        pop    ax
        mov    ah, byte ptr [esi + 0x14]
        mov    byte ptr [esi + 0x14], al
        xor    ah, al
        or     byte ptr [esi + 0x13], ah
        pop    esi
        pop    edx
        ret    
sub_1_0D14:
        xor    bl, bl
        shl    ax, 1
        rcl    al, 1
        rcl    bl, 1
        shr    ax, 1
        and    al, 0xf0
        or     al, bl
        ret    
sub_1_0D25:
        mov    al, 0xff
        call   sub_1_0DFD
        mov    cx, 2
        call   sub_1_0074
        mov    al, 0
        call   sub_1_0DFD
        clc    
        ret    
        db  053h, 051h, 052h, 0E8h, 021h, 000h, 000h, 000h, 072h, 01Bh, 08Bh, 05Dh, 0E8h, 088h, 043h, 01Bh
        db  088h, 063h, 01Ch, 0E8h, 0D7h, 0FDh, 0FFh, 0FFh, 0E8h, 077h, 000h, 000h, 000h, 0B0h, 0FFh, 0E8h
        db  0BAh, 000h, 000h, 000h, 0F8h, 05Ah, 059h, 05Bh, 0C3h
sub_1_0D67:
        push   ebx
        push   ecx
        mov    cl, al
        mov    ah, al
        test   ah, 0x80
        jne    loc_1_0DCA
        test   ah, 0xf8
        jne    loc_1_0D8B
        movzx  ebx, al
        sub    ebx, 1
        jb     loc_1_0DCA
        cmp    al, 4
        ja     loc_1_0DCA
        mov    ah, byte ptr [ebx + 0x33bb]   ; -> loc_1_33BB
        jmp    loc_1_0DBC
loc_1_0D8B:
        and    ah, 7
        xor    ah, 2
        jne    loc_1_0DA2
        shr    al, 3
        movzx  ebx, ax
        dec    ebx
        mov    ah, byte ptr [ebx + 0x33ac]   ; -> obj1:0x33AC
        jmp    loc_1_0DBC
loc_1_0DA2:
        mov    ah, 1
        cmp    cl, 0x79
        je     loc_1_0DBC
        mov    ah, 0x1c
        cmp    cl, 0xb
        je     loc_1_0DBC
        and    cl, 7
        cmp    cl, 2
        jae    loc_1_0DCA
        mov    ah, 1
        mov    cl, 0x79
loc_1_0DBC:
        mov    ebx, dword ptr [ebp - 4]
        push   ecx
        mov    cx, word ptr [ebx + 0x54]
        shr    ah, cl
        pop    ecx
        clc    
        jmp    loc_1_0DCD
loc_1_0DCA:
        mov    ah, 0x17
        stc    
loc_1_0DCD:
        mov    al, cl
        pop    ecx
        pop    ebx
        ret    
        db  053h, 08Ah, 0D8h, 066h, 083h, 0C2h, 008h, 0ECh, 024h, 01Fh, 00Ch, 040h, 0EEh, 066h, 083h, 0C2h
        db  006h, 066h, 0EDh, 066h, 083h, 0E0h, 0E0h, 00Ah, 0C3h, 053h, 08Bh, 05Dh, 0FCh, 066h, 00Bh, 043h
        db  04Ah, 05Bh, 066h, 0EFh, 066h, 083h, 0C2h, 0F2h, 0F8h, 05Bh, 0C3h
sub_1_0DFD:
        push   edx
        and    al, 4
        push   ebx
        xchg   bx, ax
        call   sub_1_0E3F
        and    al, 0xf3
        or     al, bl
        or     al, 0x18
        mov    ebx, dword ptr [ebp - 4]
        or     al, byte ptr [ebx + 0x4e]
        add    dx, 0xe
        out    dx, al
        pop    ebx
        pop    edx
        ret    
        db  052h, 024h, 010h, 053h, 066h, 093h, 0E8h, 018h, 000h, 000h, 000h, 0A8h, 020h, 074h, 002h, 033h
        db  0DBh, 024h, 0EFh, 00Ah, 0C3h, 08Bh, 05Dh, 0FCh, 00Ah, 043h, 04Eh, 066h, 083h, 0C2h, 00Eh, 0EEh
        db  05Bh, 05Ah, 0C3h
sub_1_0E3F:
        push   edx
        mov    al, 0xc0
        call   sub_1_0E4E
        add    dx, 0xe
        in     al, dx
        pop    edx
        ret    
sub_1_0E4E:
        push   edx
        mov    ah, al
        add    dx, 8
        in     al, dx
        and    al, 0x1f
        or     al, ah
        out    dx, al
        pop    edx
        ret    
sub_1_0E5D:
        push   eax
        push   ebx
        xor    bl, bl
        mov    bh, 0xe7
        shr    al, 1
        jae    loc_1_0E69
        mov    bl, 0x18
loc_1_0E69:
        call   sub_1_0C9B
        pop    ebx
        pop    eax
        ret    
sub_1_0E71:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   edi
        call   sub_1_0B2D
        not    al
        shl    ax, 1
        shr    al, 2
        rcl    ah, 1
        shl    al, 2
        shr    ax, 5
        shl    al, 3
        mov    cx, ax
        and    cx, 0xf8
        mov    ebx, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [ebx + 4]
        inc    ax
        inc    ax
        call   sub_1_082C
        mov    ax, 1
        add    dx, 6
        out    dx, al
        add    dx, 7
        in     al, dx
        mov    edi, eax
        and    al, 3
        out    dx, al
        add    dx, -0xd
        in     ax, dx
        and    ax, 0xff07
        or     ax, cx
        out    dx, ax
        mov    word ptr [ebx + 8], ax
        push   ebx
        mov    cl, 0xbd
        add    dx, 0xc
        mov    al, cl
        out    dx, al
        add    dx, -8
        in     ax, dx
        mov    bx, ax
        xor    bx, 0x1000
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
        add    dx, 8
        mov    al, cl
        out    dx, al
        xor    bx, 0x1000
        add    dx, -8
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
        mov    eax, edi
        add    dx, 9
        out    dx, al
        pop    ebx
        add    dx, -6
        in     al, dx
        mov    byte ptr [ebx + 0x15], al
        push   ebx
        call   sub_1_0D14
        pop    ebx
        mov    byte ptr [ebx + 0x14], al
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        clc    
        ret    
sub_1_0F2C:
        push   eax
        push   ebx
        push   ecx
        push   edx
        pushfd 
        cli    
        and    ax, 0xf
        mov    ebx, dword ptr [ebp - 4]
        lea    ebx, [ebx + 0x38]
        xlatb  
        test   al, 0xf0
        jne    loc_1_0FDE
        push   eax
        call   sub_1_0B2D
        mov    cl, 0
        add    dx, 0xd
        mov    al, cl
        out    dx, al
        add    dx, -9
        in     ax, dx
        mov    bx, ax
        xor    bx, 0x1000
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
        add    dx, 9
        mov    al, cl
        out    dx, al
        xor    bx, 0x1000
        add    dx, -9
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
        pop    ebx
        add    dx, 4
        mov    al, 0xa0
        out    dx, al
        add    dx, 6
        in     ax, dx
        and    ax, 0xfff0
        or     al, bl
        out    dx, ax
        mov    cl, 1
        dec    dx
        dec    dx
        mov    al, cl
        out    dx, al
        add    dx, -8
        in     ax, dx
        mov    bx, ax
        xor    bx, 0x1000
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
        add    dx, 8
        mov    al, cl
        out    dx, al
        xor    bx, 0x1000
        add    dx, -8
        in     ax, dx
        and    ax, 0x8fff
        or     ax, bx
        out    dx, ax
        clc    
        jmp    loc_1_0FE1
loc_1_0FDE:
        mov    ah, 6
        stc    
loc_1_0FE1:
        popfd  
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
loc_1_0FE7:
        clc    
        ret    
sub_1_0FE9:
        clc    
        ret    
sub_1_0FEB:
        clc    
        ret    
sub_1_0FED:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    edi, dword ptr [esi + 0x12]
        mov    edi, dword ptr cs:[edi + 2]
        or     cx, cx
        je     loc_1_1008
        mov    ax, cx
        call   sub_1_1063
        jae    loc_1_100D
loc_1_1008:
        mov    ah, 0xa
        stc    
        jmp    loc_1_105D
loc_1_100D:
        jcxz   loc_1_1027
        push   dx
        mov    ax, dx
        xor    dx, dx
        div    cx
        or     dx, dx
        pop    dx
        je     loc_1_1027
loc_1_1022:
        mov    ah, 3
        stc    
        jmp    loc_1_105D
loc_1_1027:
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 0x2f]
        cmp    byte ptr [esi + 0x1a], al
        jae    loc_1_103F
        mov    ax, dx
        add    ax, cx
        je     loc_1_105D
        jb     loc_1_1008
        jmp    loc_1_105D
loc_1_103F:
        jcxz   loc_1_1008
        cmp    cx, 0x1000
        ja     loc_1_1008
        cmp    dx, 0xfff
        ja     loc_1_1022
        mov    ax, dx
        add    ax, cx
        cmp    ax, 0x1000
        ja     loc_1_1008
        clc    
loc_1_105D:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
sub_1_1063:
        push   eax
        push   ecx
        mov    cx, ax
        not    ax
        dec    cx
        xor    ax, cx
        and    ax, cx
        je     loc_1_1076
        stc    
loc_1_1076:
        pop    ecx
        pop    eax
        ret    
        db  000h, 000h, 000h
loc_1_107C:
        pushfd 
        inc    byte ptr [esi + 6]
        push   ebx
        push   ebp
        mov    ebp, esp
        push   esi
        push   eax
        push   ecx
        push   edx
        push   edi
        lea    esp, [ebp - 0x24]
        mov    byte ptr [ebp - 0x24], 0
        cld    
        cmp    ah, 0x9e
        je     loc_1_10DF
        sub    ah, 0x80
        jb     loc_1_10FF
        cmp    ah, 0x30
        ja     loc_1_10FF
        movzx  eax, ah
        mov    eax, dword ptr [eax*4 + 0x3431] ; -> obj1:0x3431
        cmp    dword ptr [esi + 0xa], 0x44425353
        jne    loc_1_1103
        call   eax
loc_1_10B5:
        mov    byte ptr [ebp - 7], ah
        and    dword ptr [ebp + 8], 0xfffffffe
        or     ah, ah
        je     loc_1_10C4
        or     dword ptr [ebp + 8], 1
loc_1_10C4:
        pushf  
        call   sub_1_0955
        mov    esi, dword ptr [ebp - 4]
        dec    byte ptr [esi + 6]
        popf   
        lea    esp, [ebp - 0x14]
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        pop    ebp
        pop    ebx
        popfd  
        ret    
loc_1_10DF:
        call   sub_1_1804
        mov    byte ptr [ebp - 7], ah
        and    dword ptr [ebp + 8], 0xfffffffe
        or     ah, ah
        je     loc_1_10F3
        or     dword ptr [ebp + 8], 1
loc_1_10F3:
        jmp    loc_1_10C4
        db  0C6h, 045h, 0F9h, 018h, 083h, 04Dh, 008h, 001h, 0EBh, 0C5h
loc_1_10FF:
        mov    ah, 0x15
        jmp    loc_1_10B5
loc_1_1103:
        mov    ah, 1
        jmp    loc_1_10B5
        db  0F8h, 00Ah, 0C0h, 074h, 003h, 0B4h, 002h, 0F9h, 0C3h
loc_1_1110:
        mov    ah, 0x15
        ret    
sub_1_1113:
        mov    edi, dword ptr [ebp + 4]
        mov    esi, 0x328e                   ; -> obj1:0x328E
        mov    ecx, 0x8f
        cld    
        push   edi
        rep movsw word ptr es:[edi], word ptr [esi]
        pop    edi
        mov    esi, edi
        mov    ecx, 0x200
        xor    ax, ax
loc_1_1130:
        lodsb  al, byte ptr [esi]
        add    ah, al
        loop   loc_1_1130
        neg    ah
        mov    byte ptr [edi + 0x2b], ah
        xor    ax, ax
        ret    
sub_1_113E:
        push   ebx
        push   edi
        push   esi
        mov    edx, ebx
        add    edx, 0x56
        lea    esi, [0x327c]                 ; -> obj1:0x327C
        mov    dword ptr [ebx + 0x12], esi
        mov    byte ptr [ebx + 0x28], 0
        mov    byte ptr [ebx + 0x29], 0
        mov    byte ptr [ebx + 0x2a], 0
        or     word ptr [ebx + 0x2c], 0x40
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 0x16]
        mov    cl, byte ptr [edi + 0x26]
        xor    ch, ch
        mov    edx, dword ptr [esi + 0xe]
        add    edx, 2
loc_1_1170:
        mov    dword ptr [ebx], edx
        mov    byte ptr [ebx + 0xe], ch
        mov    word ptr [ebx + 0xa], 0
        call   sub_1_0387
        add    ebx, 0x1a
        inc    ch
        cmp    ch, cl
        jne    loc_1_1170
        mov    ebx, dword ptr [edi + 0x1a]
        mov    cl, byte ptr [edi + 0x27]
        xor    ch, ch
loc_1_1191:
        mov    byte ptr [ebx + 0x1a], ch
        call   sub_1_03B6
        add    ebx, 0x1e
        inc    ch
        cmp    ch, cl
        jne    loc_1_1191
        pop    esi
        pop    edi
        pop    ebx
        ret    
sub_1_11A6:
        call   sub_1_1559
        jb     loc_1_11D7
        mov    bl, byte ptr [ebp + 4]
sub_1_11B0:
        push   ecx
        push   esi
        mov    esi, dword ptr [ebp - 4]
        cmp    bl, byte ptr [esi + 0x26]
        cmc    
        jae    loc_1_11BF
        mov    ah, 0xb
        jmp    loc_1_11D5
loc_1_11BF:
        xor    eax, eax
        mov    al, bl
        mov    cl, 0x1a
        mul    cl
        mov    esi, dword ptr [esi + 0x16]
        add    esi, eax
        mov    dword ptr [ebp - 0x1c], esi
        call   sub_1_0587
        clc    
loc_1_11D5:
        pop    esi
        pop    ecx
loc_1_11D7:
        ret    
loc_1_11D8:
        call   sub_1_1559
        jae    loc_1_11E4
        jmp    loc_1_1275
loc_1_11E4:
        cld    
        mov    ebx, dword ptr [ebp - 0x20]
        mov    edi, dword ptr [ebp - 4]
        mov    ah, byte ptr [edi + 0x27]
        mov    al, byte ptr [edi + 0x26]
        mov    word ptr [ebp + 4], ax
        mov    esi, dword ptr [edi + 0x30]
        xor    ax, ax
        mov    edi, dword ptr [ebp - 0x14]
        movzx  ecx, word ptr [edi]
        add    cx, 4
        xor    dx, dx
        cmp    cx, 0x10
        je     loc_1_1231
        jb     loc_1_123B
        push   edi
        push   esi
        push   cx
        lea    edi, [edi + 0x10]
        sub    cx, 0x10
        lodsw  ax, word ptr [esi]
        shl    ax, 1
        cmp    cx, ax
        jbe    loc_1_1228
        mov    cx, ax
loc_1_1228:
        add    dx, cx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    cx
        pop    esi
        pop    edi
loc_1_1231:
        lodsw  ax, word ptr [esi]
        inc    dx
        inc    dx
        mov    word ptr [edi + 0xe], ax
loc_1_123B:
        call   sub_1_17CD
        cmp    cx, 0xe
        jb     loc_1_124D
        add    dx, 4
        mov    dword ptr [edi + 0xa], eax
loc_1_124D:
        call   sub_1_17DB
        cmp    cx, 0xa
        jb     loc_1_125F
        add    dx, 4
        mov    dword ptr [edi + 6], eax
loc_1_125F:
        dec    cx
        jcxz   loc_1_126F
        inc    dx
        inc    dx
        mov    ax, word ptr [ebx]
        mov    word ptr [edi + 4], ax
loc_1_126F:
        mov    word ptr [edi + 2], dx
        mov    ah, 0
loc_1_1275:
        ret    
sub_1_1276:
        call   sub_1_1559
        jb     loc_1_12B6
        push   ecx
        push   esi
        mov    esi, dword ptr [ebp - 4]
        mov    bh, byte ptr [ebp + 5]
        cmp    bh, byte ptr [esi + 0x27]
        cmc    
        jae    loc_1_128F
        mov    ah, 0x11
        jmp    loc_1_12B4
loc_1_128F:
        xor    eax, eax
        mov    al, bh
        mov    cl, 0x1e
        mul    cl
        mov    esi, dword ptr [esi + 0x1a]
        add    esi, eax
        mov    dword ptr [ebp - 0x18], esi
        mov    bx, word ptr [esi + 0x18]
        cmp    byte ptr [ebp - 7], 0x89
        jne    loc_1_12AC
        mov    bl, byte ptr [ebp + 4]
loc_1_12AC:
        call   sub_1_11B0
        jb     loc_1_12B4
        clc    
loc_1_12B4:
        pop    esi
        pop    ecx
loc_1_12B6:
        ret    
loc_1_12B7:
        mov    bl, 0
        call   sub_1_1276
        jb     loc_1_12ED
        mov    ebx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 0x2f]
        cmp    byte ptr [ebx + 0x1a], al
        jae    loc_1_12D3
        mov    ah, 0x11
        jmp    loc_1_12ED
loc_1_12D3:
        cmp    dword ptr [ebp + 4], 0
        je     loc_1_12DD
        mov    ah, 8
        jmp    loc_1_12ED
loc_1_12DD:
        mov    al, byte ptr [ebx + 7]
        mov    byte ptr [ebp - 0x10], al
        mov    ax, word ptr [ebx + 4]
        mov    word ptr [ebp - 0x14], ax
        mov    ah, 0
loc_1_12ED:
        ret    
loc_1_12EE:
        call   sub_1_1559
        jb     loc_1_1313
        mov    word ptr [ebp + 4], 0x210
        mov    bx, 1
        cmp    bx, 0
        je     loc_1_130B
        mov    word ptr [ebp + 4], 0x201
loc_1_130B:
        mov    word ptr [ebp - 0xc], 0x101
        mov    ah, 0
loc_1_1313:
        ret    
loc_1_1314:
        call   sub_1_11A6
        jb     loc_1_1378
        mov    esi, dword ptr [ebp - 0x1c]
        mov    esi, dword ptr [esi]
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp + 5], al
        mov    ah, byte ptr [esi + 1]
        mov    al, byte ptr [esi + 2]
        mov    word ptr [ebp - 0x10], ax
        mov    edi, dword ptr [ebp - 0x14]
        movzx  ecx, word ptr [edi]
        add    cx, 4
        xor    dx, dx
        cmp    cx, 0xe
        jb     loc_1_134E
        call   sub_1_17CD
        mov    dword ptr [edi + 0xa], eax
        add    dx, 4
loc_1_134E:
        cmp    cx, 0xa
        jb     loc_1_1360
        call   sub_1_17DB
        mov    dword ptr [edi + 6], eax
        add    dx, 4
loc_1_1360:
        dec    cx
        jcxz   loc_1_1372
        xor    ah, ah
        mov    al, byte ptr [esi + 3]
        mov    word ptr [edi + 4], ax
        inc    dx
        inc    dx
loc_1_1372:
        mov    word ptr [edi + 2], dx
        mov    ah, 0
loc_1_1378:
        ret    
sub_1_1379:
        push   ebx
        push   ecx
        cmp    dword ptr [ebx], 0
        jne    loc_1_1383
        clc    
        jmp    loc_1_13AD
loc_1_1383:
        mov    ebx, dword ptr [ebx]
        push   eax
        mov    ch, cl
        not    ch
        and    ah, cl
        and    al, cl
        sub    al, ah
        jne    loc_1_1395
        pop    eax
        jmp    loc_1_13AD
loc_1_1395:
        mov    ax, 1
        jb     loc_1_13A1
        mov    ax, 0
        xor    cl, cl
loc_1_13A1:
        call   ebx
        jae    loc_1_13A8
        pop    ecx
        jmp    loc_1_13AD
loc_1_13A8:
        pop    eax
        and    al, ch
        or     al, cl
loc_1_13AD:
        pop    ecx
        pop    ebx
        ret    
loc_1_13B0:
        call   sub_1_11A6
        jae    loc_1_13BC
        jmp    loc_1_14D1
loc_1_13BC:
        mov    edi, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 0x1c]
        mov    cx, word ptr [ebp - 0xc]
        cmp    word ptr [esi + 0x18], cx
        je     loc_1_13E1
        or     cx, cx
        jne    loc_1_13D5
        and    byte ptr [esi + 0xf], 0xfe
loc_1_13D5:
        call   sub_1_15CE
        jae    loc_1_13E1
        jmp    loc_1_14D1
loc_1_13E1:
        mov    word ptr [esi + 0x18], cx
        mov    al, byte ptr [ebp + 5]
        or     al, al
        je     loc_1_140D
        push   eax
        push   esi
        mov    esi, dword ptr [esi]
        mov    ah, byte ptr [esi]
        mov    si, 1
        cmp    si, 0
        je     loc_1_13FE
        and    al, ah
loc_1_13FE:
        not    ah
        test   ah, al
        pop    esi
        pop    eax
        je     loc_1_140D
        mov    ah, 6
        jmp    loc_1_14D1
loc_1_140D:
        cmp    byte ptr [esi + 0x12], al
        je     loc_1_1421
        call   sub_1_0E71
        jae    loc_1_141E
        jmp    loc_1_14D1
loc_1_141E:
        mov    byte ptr [esi + 0x12], al
loc_1_1421:
        mov    al, byte ptr [ebp - 0xf]
        or     al, al
        je     loc_1_1432
        mov    ebx, dword ptr [esi]
        and    al, byte ptr [ebx + 1]
        not    al
        and    byte ptr [esi + 0x13], al
loc_1_1432:
        mov    ah, byte ptr [ebp - 0x10]
        mov    al, byte ptr [esi + 0x17]
        mov    ebx, 0x34f8                   ; -> loc_1_34F8
        mov    cx, 0x10
loc_1_1441:
        call   sub_1_1379
        jb     loc_1_14D1
        or     byte ptr [esi + 0x17], al
        add    ebx, 4
        shr    cl, 1
        jae    loc_1_1441
        mov    ah, byte ptr [ebp - 0x10]
        mov    al, byte ptr [edi + 0x29]
        mov    ebx, 0x350c                   ; -> loc_1_350C
        mov    cx, 0x80
loc_1_1465:
        call   sub_1_1379
        jb     loc_1_14D1
        or     byte ptr [esi + 0x17], al
        inc    bx
        inc    bx
        shr    cl, 1
        jae    loc_1_1465
        xor    ah, ah
        mov    al, byte ptr [ebp - 0x13]
        mov    cl, al
        cmp    al, byte ptr [esi + 0xd]
        je     loc_1_148D
        call   sub_1_08AB
        jb     loc_1_14D1
        mov    byte ptr [esi + 0xd], cl
loc_1_148D:
        test   byte ptr [ebp - 0x13], 1
        jne    loc_1_14BF
        test   word ptr [ebp - 0x14], 0x80
        je     loc_1_14A7
        test   word ptr [ebp - 0x14], 0x40
        jne    loc_1_14A7
        mov    ah, 6
        jmp    loc_1_14D1
loc_1_14A7:
        xor    ah, ah
        mov    al, byte ptr [ebp - 0x14]
        mov    cl, al
        cmp    al, byte ptr [esi + 0xc]
        je     loc_1_14BD
        call   sub_1_0851
        jb     loc_1_14D1
        mov    byte ptr [esi + 0xc], cl
loc_1_14BD:
        jmp    loc_1_14CE
loc_1_14BF:
        mov    al, byte ptr [ebp - 0x14]
        and    al, 0x7f
        mov    cl, al
        call   sub_1_0851
        mov    byte ptr [esi + 0xc], cl
loc_1_14CE:
        mov    ah, 0
        clc    
loc_1_14D1:
        ret    
loc_1_14D2:
        call   sub_1_11A6
        jb     loc_1_1512
        mov    esi, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp - 0x1c]
        call   sub_1_0CE0
        cmp    byte ptr [ebp - 7], 0x8f
        je     loc_1_14F6
        call   sub_1_1657
        mov    word ptr [ebp - 0xc], cx
        mov    al, byte ptr [edi + 0x12]
loc_1_14F6:
        mov    byte ptr [ebp + 5], al
        mov    ah, byte ptr [edi + 0x13]
        mov    al, byte ptr [edi + 0x17]
        or     al, byte ptr [esi + 0x29]
        mov    word ptr [ebp - 0x10], ax
        mov    al, byte ptr [edi + 0xc]
        mov    ah, byte ptr [edi + 0xd]
        mov    word ptr [ebp - 0x14], ax
        mov    ah, 0
loc_1_1512:
        ret    
loc_1_1513:
        call   sub_1_1559
        jb     loc_1_1558
        mov    ah, 0x15
        cmp    byte ptr [ebp + 4], 0
        jne    loc_1_1558
        mov    word ptr [ebp - 0x10], 0x100
        mov    edi, dword ptr [ebp - 0x14]
        movzx  ecx, word ptr [edi]
        lea    ebx, [edi + 2]
        mov    edx, ecx
        jecxz  loc_1_1553
        add    edi, 4
        xor    edx, edx
        mov    esi, dword ptr [ebp - 0x20]
        mov    esi, dword ptr [esi + 2]
        add    esi, 8
        xor    edx, edx
        cld    
loc_1_1546:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        inc    edx
        or     al, al
        je     loc_1_1553
        loop   loc_1_1546
        mov    byte ptr [edi - 1], 0
loc_1_1553:
        mov    word ptr [ebx], dx
        mov    ah, 0
loc_1_1558:
        ret    
sub_1_1559:
        push   ecx
        push   esi
        mov    dword ptr [ebp - 0x1c], 0
        mov    esi, dword ptr [ebp - 4]
        cmp    esi, 0
        je     loc_1_1579
        mov    esi, 0x327a                   ; -> loc_1_327A
        lea    esi, [esi + 2]
        mov    dword ptr [ebp - 0x20], esi
        clc    
loc_1_1576:
        pop    esi
        pop    ecx
        ret    
loc_1_1579:
        mov    ah, 1
        stc    
        jmp    loc_1_1576
loc_1_157E:
        call   sub_1_11A6
        jb     loc_1_15BB
        mov    ebx, dword ptr [ebp - 0x1c]
        call   sub_1_0B2D
        call   sub_1_0CA8
        mov    ah, 0x14
        cmp    al, 0x80
        jb     loc_1_15BB
        call   sub_1_09D1
        pushfd 
        cli    
        call   sub_1_1672
        call   sub_1_0AB8
        popfd  
        mov    cx, 2
        call   sub_1_0074
        call   sub_1_0D25
        clc    
        mov    ah, 0
loc_1_15BB:
        ret    
loc_1_15BC:
        mov    cl, byte ptr [ebp - 0xc]
        cmp    cl, 0xb
        je     loc_1_15C8
        mov    ah, 0x15
        jmp    loc_1_15CD
loc_1_15C8:
        call   sub_1_1113
loc_1_15CD:
        ret    
sub_1_15CE:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        mov    ax, cx
        mov    ah, al
        shr    ah, 4
        xor    ah, al
        test   ah, 0xf
        je     loc_1_15EA
        mov    ah, 0xf
        stc    
        jmp    loc_1_1651
loc_1_15EA:
        push   ebx
        push   edi
        mov    edi, dword ptr [ebx + 0x34]
        add    edi, 2
        mov    esi, dword ptr [ebx + 0x30]
        xor    ax, ax
        xor    ebx, ebx
        mov    bl, ch
        cmp    bl, 2
        jb     loc_1_1606
        mov    ah, 0xe
        stc    
        jmp    loc_1_164F
loc_1_1606:
        shl    bl, 1
        mov    al, byte ptr [ebx + edi]
        mov    ah, 1
        cmp    ah, 0
        jne    loc_1_161A
        or     ch, ch
        je     loc_1_161A
        or     cl, cl
        je     loc_1_1628
loc_1_161A:
        xor    bx, bx
        mov    ch, cl
        shr    ch, 4
        mov    bl, ch
        cmp    bl, byte ptr [esi]
        jb     loc_1_162D
loc_1_1628:
        mov    ah, 0xf
        stc    
        jmp    loc_1_164F
loc_1_162D:
        shl    bl, 1
        xor    al, byte ptr [ebx + edi + 1]
        pop    edi
        pop    ebx
        call   sub_1_091C
        mov    cx, 9
        call   sub_1_0074
        pop    edx
        pop    ecx
        mov    ebx, dword ptr [ebp - 0x1c]
        mov    word ptr [ebx + 0x18], cx
        clc    
        jmp    loc_1_1653
loc_1_164F:
        pop    edi
        pop    ebx
loc_1_1651:
        pop    edx
        pop    ecx
loc_1_1653:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1657:
        push   ebx
        push   esi
        push   edi
        push   edx
        mov    ebx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 0x20]
        mov    edi, dword ptr [ebp - 0x1c]
        call   sub_1_0FE9
        mov    cx, word ptr [edi + 0x18]
        pop    edx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1672:
        push   eax
        push   ecx
        push   edx
        push   ebx
        mov    ebx, dword ptr [ebp - 0x1c]
        mov    word ptr [ebx + 0x18], 0x111
        mov    ebx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebx + 0x48]
        pop    ebx
        jmp    loc_1_1699
        db  050h, 051h, 052h, 053h, 08Bh, 05Dh, 0E4h, 066h, 0C7h, 043h, 018h, 000h, 000h, 05Bh, 033h, 0C0h
loc_1_1699:
        push   eax
        call   sub_1_0B2D
        pop    eax
        call   sub_1_091C
        pop    edx
        pop    ecx
        pop    eax
        ret    
loc_1_16A9:
        call   sub_1_1559
        jae    loc_1_16B5
        jmp    loc_1_1770
loc_1_16B5:
        mov    esi, dword ptr [ebp - 4]
        mov    al, byte ptr [esi + 0x2a]
        cmp    byte ptr [ebp - 7], 0x85
        jne    loc_1_16F0
        mov    byte ptr [ebp - 0xf], al
        mov    ax, word ptr [esi + 0x2c]
        mov    bx, word ptr [esi + 8]
        cmp    bl, 4
        je     loc_1_16E5
        cmp    bl, 8
        je     loc_1_16E5
        cmp    bl, 0x14
        je     loc_1_16E5
        cmp    ax, 1
        jne    loc_1_16E5
        mov    ax, 0xb
loc_1_16E5:
        mov    word ptr [ebp - 0x14], ax
        mov    ah, 0
        jmp    loc_1_1770
loc_1_16F0:
        mov    di, word ptr [ebp - 0x14]
        mov    dx, word ptr [ebp - 0x10]
        mov    ebx, 0x354c                   ; -> loc_1_354C
        mov    ah, dh
        mov    ecx, 2
loc_1_1704:
        call   sub_1_1379
        jb     loc_1_176A
        mov    byte ptr [esi + 0x2a], al
        add    ebx, 4
        shr    cl, 1
        jae    loc_1_1704
        test   di, 0x40
        jne    loc_1_1721
        mov    ah, 6
        stc    
        jmp    loc_1_176E
loc_1_1721:
        mov    ax, di
        and    ax, 0x1f
        call   sub_1_0F2C
        jb     loc_1_176E
        and    word ptr [esi + 0x2c], 0xff7f
        and    word ptr [esi + 0x2c], 0xffe0
        or     word ptr [esi + 0x2c], ax
        mov    ebx, 0x352c                   ; -> loc_1_352C
        mov    ax, word ptr [esi + 0x2c]
        mov    dx, di
        mov    ah, dl
        mov    ecx, 0x80
loc_1_1751:
        call   sub_1_1379
        jb     loc_1_176E
        mov    byte ptr [esi + 0x2c], al
        add    ebx, 4
        shr    cl, 1
        jae    loc_1_1751
        mov    byte ptr [esi + 0x2d], 0
        mov    ah, 0
        jmp    loc_1_1770
loc_1_176A:
        mov    ah, 1
        jmp    loc_1_1770
loc_1_176E:
        mov    ah, 6
loc_1_1770:
        ret    
loc_1_1771:
        mov    bl, 0
        call   sub_1_1276
        jb     loc_1_17CC
        mov    ebx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x2f]
        cmp    byte ptr [ebx + 0x1a], al
        jae    loc_1_178D
        mov    ah, 0x11
        jmp    loc_1_17CC
loc_1_178D:
        cmp    byte ptr [ebp + 4], 0
        je     loc_1_1797
        mov    ah, 8
        jmp    loc_1_17CC
loc_1_1797:
        mov    esi, dword ptr [ebp - 0x18]
        mov    cx, word ptr [ebp - 0x14]
        call   sub_1_0FEB
        jae    loc_1_17A7
        jmp    loc_1_17CC
loc_1_17A7:
        mov    al, byte ptr [ebp - 0x10]
        test   al, 0xf8
        je     loc_1_17B2
        mov    ah, 2
        jmp    loc_1_17CC
loc_1_17B2:
        and    byte ptr [ebx + 7], 2
        and    al, 0xfd
        or     byte ptr [ebx + 7], al
        mov    ax, word ptr [ebp - 0x14]
        mov    word ptr [ebx + 4], ax
        mov    al, 1
        call   sub_1_05AD
        mov    ah, 0
loc_1_17CC:
        ret    
sub_1_17CD:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        xor    eax, eax
        cmp    byte ptr [ebx + 0x2b], 0
        jne    loc_1_1802
        jmp    loc_1_17E7
sub_1_17DB:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        xor    eax, eax
        cmp    byte ptr [ebx + 0x2b], 1
        jne    loc_1_1802
loc_1_17E7:
        push   ecx
        mov    ecx, 0x10
        lea    ebx, [ebx + 0x38]
        jmp    loc_1_17F5
loc_1_17F2:
        shr    eax, 1
        inc    ebx
loc_1_17F5:
        cmp    byte ptr [ebx], 0
        je     loc_1_17FF
        or     eax, 0x8000
loc_1_17FF:
        loop   loc_1_17F2
        pop    ecx
loc_1_1802:
        pop    ebx
        ret    
sub_1_1804:
        cld    
        push   word ptr [ebp + 4]
        mov    edi, esp
        sub    esp, 0xa
        call   sub_1_1559
        jae    loc_1_181A
        jmp    loc_1_18F0
loc_1_181A:
        xor    ax, ax
        mov    word ptr [ebp - 0xc], ax
        mov    ebx, dword ptr [ebp - 4]
        mov    al, byte ptr [ebx + 0x26]
        mov    byte ptr ss:[edi - 8], al
        call   sub_1_0B2D
        mov    word ptr ss:[edi - 0xa], dx
        add    dx, 5
        in     al, dx
        mov    byte ptr ss:[edi - 6], al
        and    al, 0x8f
        mov    byte ptr ss:[edi - 5], al
        mov    byte ptr [ebp - 0x24], 1
loc_1_1848:
        xor    ax, ax
        mov    byte ptr ss:[edi - 3], al
        mov    word ptr [ebp + 4], ax
        inc    ax
        mov    word ptr ss:[edi - 2], ax
loc_1_185A:
        mov    bl, byte ptr [ebp + 4]
        call   sub_1_11B0
        mov    dx, word ptr ss:[edi - 0xa]
        add    dx, 5
        mov    al, byte ptr [ebp + 4]
        shl    al, 4
        or     al, byte ptr ss:[edi - 5]
        out    dx, al
        add    dx, 7
        in     ax, dx
        and    al, ah
        and    al, 0x60
        je     loc_1_18BC
        mov    byte ptr ss:[edi - 3], al
        test   al, 0x20
        je     loc_1_18AE
        push   ax
        add    dx, -3
        in     al, dx
        and    al, 0xc0
        cmp    al, 0xc0
        jne    loc_1_189C
        call   sub_1_1960
loc_1_189C:
        mov    dx, word ptr ss:[edi - 0xa]
        add    dx, 0xc
        mov    al, 0xdd
        out    dx, al
        pop    ax
        test   al, 0x40
        je     loc_1_18BC
loc_1_18AE:
        call   sub_1_18F7
        mov    ax, word ptr ss:[edi - 2]
        or     word ptr [ebp - 0xc], ax
loc_1_18BC:
        shl    word ptr ss:[edi - 2], 1
        inc    byte ptr [ebp + 4]
        mov    al, byte ptr [ebp + 4]
        cmp    al, byte ptr ss:[edi - 8]
        jb     loc_1_185A
        mov    al, byte ptr ss:[edi - 3]
        or     al, al
        jne    loc_1_1848
        mov    dx, word ptr ss:[edi - 0xa]
        add    dx, 5
        mov    al, byte ptr ss:[edi - 6]
        out    dx, al
        mov    dword ptr [ebp - 0x1c], 0
        mov    ah, 0
loc_1_18F0:
        mov    esp, edi
        pop    word ptr [ebp + 4]
        ret    
sub_1_18F7:
        mov    dx, word ptr ss:[edi - 0xa]
        mov    esi, dword ptr [ebp - 0x1c]
        xor    bl, bl
        add    dx, 8
        in     al, dx
        push   ax
        and    al, 0x1f
        or     al, 0x40
        out    dx, al
        add    dx, 6
loc_1_1911:
        in     ax, dx
        mov    al, 0xbd
        dec    dx
        dec    dx
        out    dx, al
        mov    al, byte ptr [esi + 0x15]
        mov    byte ptr [esi + 0x15], ah
        xor    al, ah
        or     bl, al
        test   ah, 0x80
        jne    loc_1_1945
        add    dx, -4
        in     al, dx
        add    al, 0x80
        out    dx, al
        add    dx, 6
        in     al, dx
        and    al, 0xef
        out    dx, al
        add    dx, -6
        in     al, dx
        add    al, 0x80
        out    dx, al
        add    dx, 4
loc_1_1945:
        in     al, dx
        inc    dx
        inc    dx
        test   al, 0x40
        jne    loc_1_1911
        pop    ax
        add    dx, -6
        out    dx, al
        mov    al, bl
        call   sub_1_0D14
        or     byte ptr [esi + 0x13], al
        ret    
sub_1_1960:
        push   word ptr [ebp + 4]
        mov    word ptr [ebp + 4], 0
loc_1_196A:
        mov    bl, byte ptr [ebp + 4]
        call   sub_1_11B0
        mov    dx, word ptr ss:[edi - 0xa]
        add    dx, 5
        mov    al, byte ptr [ebp + 4]
        shl    al, 4
        or     al, byte ptr ss:[edi - 5]
        out    dx, al
        add    dx, 4
        mov    al, 0x40
        out    dx, al
        add    dx, -3
        in     al, dx
        and    al, 0xfe
        out    dx, al
        inc    dx
        inc    dx
        in     al, dx
        push   ax
        and    al, 0x1f
        add    al, 0xc0
        out    dx, al
        add    dx, 6
        in     ax, dx
        and    al, 0xef
        out    dx, al
        pop    ax
        add    dx, -6
        out    dx, al
        inc    word ptr [ebp + 4]
        mov    ax, word ptr [ebp + 4]
        cmp    al, byte ptr ss:[edi - 8]
        jb     loc_1_196A
        xor    al, al
        inc    dx
        out    dx, al
        pop    word ptr [ebp + 4]
        mov    bl, byte ptr [ebp + 4]
        call   sub_1_11B0
        ret    
        db  053h, 050h, 066h, 036h, 08Bh, 057h, 0F6h, 08Bh, 05Dh, 0E4h, 00Bh, 0DBh, 074h, 01Bh, 08Bh, 05Dh
        db  0FCh, 080h, 07Bh, 006h, 001h, 077h, 012h, 066h, 083h, 0C2h, 007h, 0ECh, 0A8h, 080h, 075h, 009h
        db  0B7h, 0FEh, 0B3h, 000h, 0E8h, 0A1h, 0F2h, 0FFh, 0FFh, 058h, 05Bh, 0C3h, 000h, 000h, 000h
sub_1_1A00:
        push   esi
        cmp    dword ptr [esp + 0xc], 2
        mov    esi, 0x13
        jne    loc_1_1A43
        mov    dword ptr [0x37b4], 0         ; -> obj1:0x37B4
        call   sub_1_2EA0
        push   3
        push   0
        push   0x1a49                        ; -> loc_1_1A49
        push   dword ptr [esp + 0x14]
        call   sub_2_005C
        add    esp, 0x10
        mov    esi, eax
        push   0
        push   dword ptr [0x37b0]            ; -> obj1:0x37B0
        call   sub_2_0054
        add    esp, 8
loc_1_1A43:
        mov    eax, esi
        pop    esi
        ret    8
loc_1_1A49:
        sub    esp, 0xd8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    ebx, dword ptr [esp + 0xec]
        cmp    ebx, 0xa
        ja     loc_1_1BA7
        jmp    loc_1_1BB9
loc_1_1A6C:
        mov    ebx, dword ptr [esp + 0xf0]
        cmp    ebx, 1
        jne    loc_1_1AEC
        mov    ebp, dword ptr [esp + 0xf4]
        push   ebp
        call   sub_1_1C2F
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_1A93
        mov    edi, 0x13
loc_1_1A93:
        or     edi, edi
        jne    loc_1_1B38
        push   esi
        call   sub_1_0010
        add    esp, 4
        or     eax, eax
        je     loc_1_1AB8
        push   0
        push   esi
        call   sub_2_0010
        add    esp, 8
        mov    edi, 0x13
loc_1_1AB8:
        or     edi, edi
        jne    loc_1_1BAC
        cmp    ebx, 1
        jne    loc_1_1B38
        push   esi
        call   sub_1_0092
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        push   0
        push   esi
        jne    loc_1_1AE2
        push   ebp
        call   sub_2_00A4
        add    esp, 0xc
        jmp    loc_1_1B38
loc_1_1AE2:
        call   sub_2_0010
        add    esp, 8
        jmp    loc_1_1B38
loc_1_1AEC:
        mov    esi, dword ptr [esp + 0xf8]
        mov    ebp, dword ptr [esp + 0xf4]
        push   0
        lea    eax, [esp + 0x14]
        push   ebp
        push   eax
        call   sub_2_0094
        add    esp, 0xc
        or     eax, eax
        je     loc_1_1B10
        jmp    loc_1_1B33
loc_1_1B10:
        mov    ax, word ptr [esp + 0x6e]
        mov    word ptr [esi + 4], ax
        movzx  ax, byte ptr [esp + 0xc0]
        mov    word ptr [esi + 0x2c], ax
        push   esi
        call   sub_1_0010
        add    esp, 4
        or     eax, eax
        je     loc_1_1B38
loc_1_1B33:
        mov    edi, 0x13
loc_1_1B38:
        or     edi, edi
        jne    loc_1_1BAC
        push   esi
        call   sub_1_319C
        add    esp, 4
        jmp    loc_1_1BAC
loc_1_1B47:
        mov    esi, dword ptr [esp + 0xf8]
        or     esi, esi
        je     loc_1_1BAC
        push   esi
        call   sub_1_31B6
        add    esp, 4
        push   esi
        call   sub_1_1C0B
        add    esp, 4
        cmp    ebx, 4
        jne    loc_1_1BAC
        push   0
        push   esi
        call   sub_2_0010
        add    esp, 8
        jmp    loc_1_1BAC
loc_1_1B76:
        push   dword ptr [esp + 0xf8]
        call   sub_1_1BE5
        add    esp, 4
        or     eax, eax
        je     loc_1_1BAC
        mov    edi, 0x13
        jmp    loc_1_1BAC
loc_1_1B90:
        mov    ebx, dword ptr [esp + 0xf0]
        and    ebx, 0x7fffffff
        cmp    ebx, 4
        jb     loc_1_1BA7
        cmp    ebx, 8
        jbe    loc_1_1BAC
loc_1_1BA7:
        mov    edi, 1
loc_1_1BAC:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xd8
        ret    
loc_1_1BB9:
        lodsb  al, byte ptr [esi]
        sbb    eax, dword ptr [eax]
        add    byte ptr [edx + ebx], ch      ; -> loc_1_1A6C
        add    byte ptr [edi + 0x1b], al     ; -> loc_1_1B47
        add    byte ptr [eax], al
        jbe    0x1be2
        add    byte ptr [eax], al
        inc    edi
        sbb    eax, dword ptr [eax]
        add    byte ptr [edi - 0x58ffffe5], ah ; -> loc_1_1BA7
        sbb    eax, dword ptr [eax]
        add    byte ptr [edi - 0x6fffffe5], ah ; -> loc_1_1BA7
        sbb    eax, dword ptr [eax]
        add    byte ptr [ebx + ebx + 0x1bac0000], ch ; -> loc_1_1BAC
        add    byte ptr [eax], al
sub_1_1BE5:
        push   ebx
        mov    ecx, dword ptr [esp + 8]
        sub    eax, eax
        mov    bl, byte ptr [ecx + 0x26]
        cmp    bl, al
        je     loc_1_1C09
        mov    edx, dword ptr [ecx + 0x16]
        add    edx, 0xf
        movzx  ecx, bl
loc_1_1BFC:
        mov    bl, byte ptr [edx]
        and    ebx, 1
        or     eax, ebx
        add    edx, 0x1a
        dec    ecx
        jne    loc_1_1BFC
loc_1_1C09:
        pop    ebx
        ret    
sub_1_1C0B:
        push   esi
        push   0x80
        mov    esi, dword ptr [esp + 0xc]
        push   6
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        push   0
        push   6
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        pop    esi
        ret    
sub_1_1C2F:
        sub    esp, 0x130
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x1c], 0xa
        mov    dword ptr [esp + 0x18], 0x32
        mov    dword ptr [esp + 0x14], 0
        mov    edi, dword ptr [esp + 0x144]
        push   0
        lea    eax, [esp + 0x6c]
        push   edi
        push   eax
        call   sub_2_0094
        add    esp, 0xc
        or     eax, eax
        je     loc_1_1C73
        sub    eax, eax
        jmp    loc_1_2085
loc_1_1C73:
        mov    ax, word ptr [esp + 0xc6]
        mov    word ptr [esp + 0x24], ax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_1C0B
        lea    eax, [esp + 0x24]
        add    esp, 4
        push   eax
        call   sub_1_2130
        lea    eax, [esp + 0x24]
        add    esp, 4
        push   eax
        call   sub_1_2871
        add    esp, 4
        or     eax, eax
        jne    loc_1_1CB2
        sub    eax, eax
        jmp    loc_1_2085
loc_1_1CB2:
        push   0x3558                        ; -> loc_1_3558
        push   edi
        call   sub_1_2090
        add    esp, 8
        cmp    ax, word ptr [esp + 0x28]
        je     loc_1_1CCF
        mov    dword ptr [esp + 0x14], 1
loc_1_1CCF:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_1CEC
        push   0x3560                        ; -> loc_1_3560
        push   edi
        call   sub_1_2090
        add    esp, 8
        mov    bx, ax
        or     bx, bx
        jne    loc_1_1D08
loc_1_1CEC:
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_2837
        add    esp, 4
        mov    bx, ax
        or     bx, bx
        jne    loc_1_1D08
        sub    eax, eax
        jmp    loc_1_2085
loc_1_1D08:
        push   0x356c                        ; -> loc_1_356C
        push   edi
        call   sub_1_2090
        mov    word ptr [esp + 0x18], ax
        add    esp, 8
        push   0x3574                        ; -> obj1:0x3574
        push   edi
        call   sub_1_2090
        mov    word ptr [esp + 0x1a], ax
        add    esp, 8
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_1D3D
        cmp    word ptr [esp + 0x10], 0
        jne    loc_1_1D4F
loc_1_1D3D:
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_2884
        mov    word ptr [esp + 0x14], ax
        add    esp, 4
loc_1_1D4F:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_1D5E
        cmp    word ptr [esp + 0x12], 0
        jne    loc_1_1D70
loc_1_1D5E:
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_289E
        mov    word ptr [esp + 0x16], ax
        add    esp, 4
loc_1_1D70:
        lea    eax, [esp + 0x18]
        push   1
        push   eax
        lea    eax, [esp + 0x34]
        push   eax
        push   3
        push   0x3580                        ; -> loc_1_3580
        push   0
        push   edi
        call   sub_2_009C
        add    esp, 0x1c
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_1D9C
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_1DB4
loc_1_1D9C:
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x28]
        push   ebx
        push   eax
        call   sub_1_28F0
        add    esp, 0x10
loc_1_1DB4:
        movzx  ebp, bx
        imul   ebp, ebp, 0x1a
        movzx  eax, word ptr [esp + 0x12]
        movzx  ecx, word ptr [esp + 0x10]
        add    eax, ecx
        imul   eax, eax, 0x1e
        add    eax, dword ptr [esp + 0x18]
        lea    eax, [ebp + eax + 0x56]
        push   1
        push   eax
        call   sub_2_0008
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    loc_1_1DE7
        jmp    loc_1_2083
loc_1_1DE7:
        mov    dword ptr [esi], edi
        mov    ax, word ptr [esp + 0x28]
        mov    word ptr [esi + 8], ax
        mov    ax, word ptr [esp + 0xc6]
        mov    word ptr [esi + 4], ax
        movzx  ax, byte ptr [esp + 0x118]
        mov    word ptr [esi + 0x2c], ax
        push   esi
        call   sub_1_2D8F
        add    esp, 4
        dec    ax
        mov    word ptr [esi + 0x54], ax
        mov    dword ptr [esp + 0x1c], 0x10
        lea    eax, [esp + 0x1c]
        push   1
        push   eax
        lea    eax, [esi + 0x38]
        push   eax
        push   3
        push   0x358c                        ; -> loc_1_358C
        push   0
        push   edi
        call   sub_2_009C
        add    esp, 0x1c
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_1E4D
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_1E56
loc_1_1E4D:
        push   esi
        call   sub_1_2984
        add    esp, 4
loc_1_1E56:
        push   esi
        call   sub_1_28B8
        add    esp, 4
        cmp    eax, 1
        jne    loc_1_1E6E
        cmp    byte ptr [esi + 0x43], 0xb
        jne    loc_1_1E6E
        mov    byte ptr [esi + 0x43], 1
loc_1_1E6E:
        mov    byte ptr [esi + 0x26], bl
        mov    al, byte ptr [esp + 0x12]
        mov    byte ptr [esi + 0x2e], al
        mov    al, byte ptr [esp + 0x10]
        lea    ecx, [esi + 0x56]
        mov    byte ptr [esi + 0x2f], al
        mov    al, byte ptr [esp + 0x12]
        add    al, byte ptr [esp + 0x10]
        mov    byte ptr [esi + 0x27], al
        mov    dword ptr [esi + 0x30], ecx
        movzx  eax, word ptr [esp + 0x2c]
        lea    ecx, [ecx + eax*2 + 2]
        mov    dword ptr [esi + 0x34], ecx
        movzx  eax, word ptr [esp + 0x2c]
        movzx  eax, word ptr [esp + eax*2 + 0x2e]
        lea    ecx, [ecx + eax*2 + 2]
        mov    dword ptr [esi + 0x16], ecx
        add    ecx, ebp
        mov    dword ptr [esi + 0x1a], ecx
        lea    ecx, [esp + 0x2c]
        sub    ebx, ebx
        cmp    word ptr [esp + 0x2c], bx
        jb     loc_1_1EDB
        sub    edx, edx
loc_1_1EC1:
        mov    ax, word ptr [ecx]
        mov    ebp, dword ptr [esi + 0x30]
        mov    word ptr [edx + ebp], ax
        add    ecx, 2
        add    edx, 2
        inc    ebx
        movzx  eax, word ptr [esp + 0x2c]
        cmp    eax, ebx
        jge    loc_1_1EC1
loc_1_1EDB:
        mov    edx, dword ptr [esi + 0x34]
        mov    ax, word ptr [ecx]
        mov    word ptr [edx], ax
        add    ecx, 2
        mov    edx, 1
        cmp    ax, dx
        jb     loc_1_1F11
        mov    ebx, 2
loc_1_1EF6:
        mov    ax, word ptr [ecx]
        mov    ebp, dword ptr [esi + 0x34]
        mov    word ptr [ebx + ebp], ax
        add    ecx, 2
        add    ebx, 2
        mov    eax, dword ptr [esi + 0x34]
        inc    edx
        movzx  eax, word ptr [eax]
        cmp    eax, edx
        jge    loc_1_1EF6
loc_1_1F11:
        mov    ax, word ptr [ecx]
        cmp    dword ptr [esp + 0x14], 0
        mov    word ptr [esi + 0x48], ax
        jne    loc_1_203E
        push   0x35ac                        ; -> loc_1_35AC
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax*2]
        push   0x35a0                        ; -> loc_1_35A0
        push   edi
        call   sub_1_20DB
        add    esp, 8
        add    bx, ax
        add    bx, bx
        push   0x3594                        ; -> obj1:0x3594
        push   edi
        call   sub_1_20DB
        add    esp, 8
        sub    bx, ax
        inc    bx
        shl    bx, 5
        mov    word ptr [esi + 0x4a], bx
        push   0x35b8                        ; -> loc_1_35B8
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax*2 + 0x18]
        push   0x35c0                        ; -> loc_1_35C0
        push   edi
        call   sub_1_20DB
        add    esp, 8
        add    bx, ax
        push   0x35c8                        ; -> obj1:0x35C8
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax + ebx*2]
        push   0x35dc                        ; -> loc_1_35DC
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax + ebx*2]
        shl    bx, 4
        mov    word ptr [esi + 0x4c], bx
        push   0x35f0                        ; -> obj1:0x35F0
        push   edi
        call   sub_1_20DB
        add    esp, 8
        imul   ax, ax, 0xc0
        mov    word ptr [esi + 0x4e], ax
        push   0x3608                        ; -> loc_1_3608
        push   edi
        call   sub_1_20DB
        add    esp, 8
        mov    bx, ax
        shl    bx, 4
        push   0x35fc                        ; -> loc_1_35FC
        push   edi
        call   sub_1_20DB
        add    esp, 8
        add    bx, ax
        shl    bx, 5
        mov    word ptr [esi + 0x50], bx
        push   0x3624                        ; -> obj1:0x3624
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax*2 + 0x80]
        push   0x3614                        ; -> obj1:0x3614
        push   edi
        call   sub_1_20DB
        add    esp, 8
        add    bx, ax
        shl    bx, 4
        mov    word ptr [esi + 0x52], bx
        mov    ax, bx
        and    ax, 0x10
        cmp    ax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [esi + 0x2b], al
        jmp    loc_1_2083
loc_1_203E:
        cmp    word ptr [esi + 0x54], 1
        sbb    eax, eax
        and    eax, 0xffffff80
        add    eax, 0xe0
        mov    word ptr [esi + 0x4a], ax
        push   esi
        call   sub_1_239D
        add    esp, 4
        cmp    eax, 1
        cmc    
        sbb    eax, eax
        and    eax, 0xc0
        or     ax, 0x610
        mov    word ptr [esi + 0x4c], ax
        mov    word ptr [esi + 0x4e], 0xc0
        mov    word ptr [esi + 0x50], 0
        mov    word ptr [esi + 0x52], 0x800
        mov    byte ptr [esi + 0x2b], 1
loc_1_2083:
        mov    eax, esi
loc_1_2085:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x130
        ret    
sub_1_2090:
        sub    esp, 0x2c
        push   esi
        mov    dword ptr [esp + 4], 0x28
        lea    eax, [esp + 4]
        push   1
        push   eax
        sub    si, si
        lea    eax, [esp + 0x10]
        push   eax
        push   1
        push   dword ptr [esp + 0x48]
        push   0
        push   dword ptr [esp + 0x4c]
        call   sub_2_009C
        add    esp, 0x1c
        or     eax, eax
        jne    loc_1_20D3
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_20F8
        add    esp, 4
        mov    si, ax
loc_1_20D3:
        mov    ax, si
        pop    esi
        add    esp, 0x2c
        ret    
sub_1_20DB:
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_1_2090
        add    esp, 8
        or     ax, ax
        mov    ax, 1
        jne    loc_1_20F7
        sub    ax, ax
loc_1_20F7:
        ret    
sub_1_20F8:
        mov    ecx, dword ptr [esp + 4]
        sub    ax, ax
        cmp    byte ptr [ecx], al
        je     loc_1_2121
loc_1_2103:
        mov    dl, byte ptr [ecx]
        cmp    dl, 0x30
        jl     loc_1_2121
        cmp    dl, 0x39
        jg     loc_1_2121
        imul   ax, ax, 0xa
        movsx  dx, dl
        inc    ecx
        lea    eax, [eax + edx - 0x30]
        cmp    byte ptr [ecx], 0
        jne    loc_1_2103
loc_1_2121:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2130:
        push   esi
        push   edi
        push   0x8000
        mov    esi, dword ptr [esp + 0x10]
        push   0xe0
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   0xc0
        push   esi
        call   sub_1_26A2
        add    esp, 8
        mov    di, ax
        and    di, 0xff00
        shr    di, 8
        push   0
        push   0xe0
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        mov    word ptr [esi + 8], di
        mov    ax, di
        pop    edi
        pop    esi
        ret    
sub_1_217D:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x1c]
        push   2
        push   esi
        call   sub_1_2638
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 8
        or     eax, 0x10000000
        push   eax
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0xc
        push   8
        push   esi
        call   sub_1_260A
        mov    byte ptr [esp + 0x1b], al
        add    esp, 8
        push   9
        push   esi
        call   sub_1_260A
        add    esp, 8
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 0x80
        je     loc_1_21DD
        mov    edi, 1
        jmp    loc_1_2305
loc_1_21DD:
        push   9
        push   esi
        call   sub_1_260A
        add    esp, 8
        mov    bl, al
        test   bl, 0x27
        je     loc_1_21FF
        mov    al, bl
        and    al, 0xd8
        push   eax
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
loc_1_21FF:
        push   0x8200
        push   0xe0
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 0x20
        jne    loc_1_2293
        push   0xc0
        push   esi
        call   sub_1_26A2
        add    esp, 8
        test   al, 0x20
        je     loc_1_2293
        push   esi
        call   sub_1_28D4
        add    esp, 4
        or     eax, eax
        mov    al, bl
        je     loc_1_2246
        or     al, 0x27
        jmp    loc_1_2248
loc_1_2246:
        or     al, 2
loc_1_2248:
        push   eax
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        sub    ebp, ebp
loc_1_2256:
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 0x20
        je     loc_1_2277
        push   0xc0
        push   esi
        call   sub_1_26A2
        add    esp, 8
        test   al, 0x20
        je     loc_1_228E
loc_1_2277:
        push   0
        push   0xd6d8
        call   sub_2_006C
        add    esp, 8
        inc    ebp
        cmp    ebp, 0x1e
        jl     loc_1_2256
        jmp    loc_1_2293
loc_1_228E:
        mov    edi, 1
loc_1_2293:
        push   0
        push   0xe0
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   dword ptr [esp + 0x13]
        push   8
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        push   ebx
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        sub    ebx, ebx
        push   dword ptr [esp + 0x14]
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0xc
loc_1_22CF:
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 0x20
        jne    loc_1_22F0
        push   0xc0
        push   esi
        call   sub_1_26A2
        add    esp, 8
        test   al, 0x20
        jne    loc_1_2305
loc_1_22F0:
        push   0
        push   0xd6d8
        call   sub_2_006C
        add    esp, 8
        inc    ebx
        cmp    ebx, 0x2d
        jl     loc_1_22CF
loc_1_2305:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  083h, 0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 07Ch, 024h, 018h, 02Bh, 0F6h, 066h, 08Bh, 05Fh
        db  004h, 08Dh, 044h, 073h, 010h, 050h, 0E8h, 029h, 00Ah, 000h, 000h, 083h, 0C4h, 004h, 066h, 08Bh
        db  0E8h, 08Dh, 004h, 073h, 050h, 0E8h, 01Ah, 00Ah, 000h, 000h, 083h, 0C4h, 004h, 066h, 03Bh, 0E8h
        db  075h, 058h, 046h, 083h, 0FEh, 008h, 07Ch, 0D9h, 06Ah, 008h, 057h, 0E8h, 0BBh, 002h, 000h, 000h
        db  083h, 0C4h, 008h, 034h, 0E0h, 088h, 044h, 024h, 013h, 0FFh, 074h, 024h, 013h, 06Ah, 008h, 057h
        db  0E8h, 08Bh, 002h, 000h, 000h, 083h, 0C4h, 00Ch, 06Ah, 018h, 057h, 0E8h, 09Bh, 002h, 000h, 000h
        db  083h, 0C4h, 008h, 08Ah, 0D8h, 08Ah, 044h, 024h, 013h, 034h, 0E0h, 050h, 06Ah, 008h, 057h, 0E8h
        db  06Ch, 002h, 000h, 000h, 02Ah, 05Ch, 024h, 01Fh, 083h, 0C4h, 00Ch, 080h, 0FBh, 001h, 01Bh, 0C0h
        db  0F7h, 0D8h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h, 02Bh, 0C0h, 0EBh, 0F4h
sub_1_239D:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x1c]
        push   esi
        call   sub_1_2802
        add    esp, 4
        cmp    eax, edi
        je     loc_1_23C4
        test   byte ptr [eax + 0x10], 2
        jne    loc_1_23C4
        sub    eax, eax
        jmp    loc_1_259B
loc_1_23C4:
        call   sub_1_2EA0
        cmp    dword ptr [0x37b4], 0         ; -> obj1:0x37B4
        je     loc_1_2599
        push   0
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0xc
        push   8
        push   esi
        call   sub_1_260A
        mov    byte ptr [esp + 0x1d], al
        add    esp, 8
        push   6
        push   esi
        call   sub_1_260A
        mov    byte ptr [esp + 0x1c], al
        add    esp, 8
        push   0xa0
        push   esi
        call   sub_1_26A2
        mov    word ptr [esp + 0x1e], ax
        add    esp, 8
        push   0x80
        push   esi
        call   sub_1_26A2
        add    esp, 8
        mov    bx, ax
        push   0x10
        push   0xa0
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   0x5678
        push   0x80
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   9
        push   esi
        call   sub_1_260A
        mov    byte ptr [esp + 0x1b], al
        add    esp, 8
        push   0x25
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        push   0
        push   0xd6d8
        call   sub_2_006C
        add    esp, 8
        push   0xc0
        push   esi
        call   sub_1_26A2
        add    esp, 8
        and    ax, 0xffef
        push   eax
        push   0xc0
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   0x808
        push   0xe0
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   0x18
        push   8
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        push   0x19
        push   6
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        mov    ebp, dword ptr [0x3780]       ; -> loc_1_3780
        shr    ebp, 0xc
        push   0x8000
        push   1
        push   ebp
        push   0
        inc    ebp
        push   esi
        call   sub_1_26D1
        add    esp, 0x14
        push   0x8800
        push   1
        push   ebp
        push   1
        push   esi
        call   sub_1_26D1
        add    esp, 0x14
        mov    eax, dword ptr [0x37b4]       ; -> obj1:0x37B4
        add    eax, 0x1000
        sub    ecx, ecx
        mov    ax, word ptr [eax]
loc_1_2500:
        cmp    ax, 0x5678
        je     loc_1_250C
        cmp    ax, 0x7878
        jne    loc_1_2514
loc_1_250C:
        inc    ecx
        cmp    ecx, 0x64
        jl     loc_1_2500
        jmp    loc_1_2519
loc_1_2514:
        mov    edi, 1
loc_1_2519:
        push   0
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_26D1
        add    esp, 0x14
        push   0
        push   0
        push   0
        push   1
        push   esi
        call   sub_1_26D1
        add    esp, 0x14
        push   dword ptr [esp + 0x16]
        push   0xa0
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   ebx
        push   0x80
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   0
        push   0xe0
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   dword ptr [esp + 0x15]
        push   8
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        push   dword ptr [esp + 0x14]
        push   6
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        push   dword ptr [esp + 0x13]
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
loc_1_2599:
        mov    eax, edi
loc_1_259B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_25A3:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        mov    di, word ptr [esp + 0x10]
        add    di, word ptr [eax + 4]
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        shr    esi, 0x10
        push   edi
        call   sub_1_2D31
        add    esp, 8
        push   esi
        add    di, 2
        push   edi
        call   sub_1_2D31
        add    esp, 8
        pop    edi
        pop    esi
        ret    
sub_1_25D4:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 0xc]
        mov    cx, word ptr [eax + 4]
        add    cx, word ptr [esp + 0xc]
        push   ecx
        call   sub_1_2D31
        add    esp, 8
        ret    
sub_1_25EF:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 0xc]
        mov    cx, word ptr [eax + 4]
        add    cx, word ptr [esp + 0xc]
        push   ecx
        call   sub_1_2D26
        add    esp, 8
        ret    
sub_1_260A:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 4]
        add    cx, word ptr [esp + 8]
        push   ecx
        call   sub_1_2D3E
        add    esp, 4
        ret    
sub_1_2621:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 4]
        add    cx, word ptr [esp + 8]
        push   ecx
        call   sub_1_2D53
        add    esp, 4
        ret    
sub_1_2638:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        mov    si, word ptr [esp + 0x10]
        add    si, word ptr [eax + 4]
        push   esi
        call   sub_1_2D53
        add    esp, 4
        movzx  edi, ax
        add    si, 2
        push   esi
        call   sub_1_2D53
        add    esp, 4
        movzx  eax, ax
        shl    eax, 0x10
        or     edi, eax
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_1_266D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   8
        push   esi
        call   sub_1_260A
        add    esp, 8
        and    al, 0x1f
        or     al, byte ptr [esp + 0xc]
        push   eax
        push   8
        push   esi
        call   sub_1_25EF
        mov    ax, word ptr [esp + 0x1c]
        add    esp, 0xc
        push   eax
        push   0xe
        push   esi
        call   sub_1_25D4
        add    esp, 0xc
        pop    esi
        ret    
sub_1_26A2:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   8
        push   esi
        call   sub_1_260A
        add    esp, 8
        and    al, 0x1f
        or     al, byte ptr [esp + 0xc]
        push   eax
        push   8
        push   esi
        call   sub_1_25EF
        add    esp, 0xc
        push   0xe
        push   esi
        call   sub_1_2621
        add    esp, 8
        pop    esi
        ret    
sub_1_26D1:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x18]
        mov    edi, esi
        imul   di, di, 0xffff
        and    di, 0x3fff
        cmp    word ptr [esp + 0x1c], 1
        jne    loc_1_26F2
        or     esi, 0x4000
loc_1_26F2:
        movzx  eax, word ptr [esp + 0x14]
        shl    eax, 3
        add    eax, 0x102
        or     eax, 0x8000000
        push   eax
        mov    ebx, dword ptr [esp + 0x14]
        push   2
        push   ebx
        call   sub_1_25A3
        add    esp, 0xc
        push   esi
        push   0
        push   ebx
        call   sub_1_25D4
        add    esp, 0xc
        push   edi
        push   0
        push   ebx
        call   sub_1_25D4
        mov    ax, word ptr [esp + 0x2c]
        add    esp, 0xc
        push   eax
        push   0
        push   ebx
        call   sub_1_25D4
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2741:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        mov    edi, dword ptr [esp + 0x10]
        push   edi
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     loc_1_27FE
        mov    ecx, dword ptr [eax + 6]
        sub    edx, edx
        cmp    byte ptr [ecx], dl
        je     loc_1_2783
loc_1_2771:
        movzx  bx, byte ptr [ecx + edx]
        mov    word ptr [esi], bx
        add    esi, 2
        inc    edx
        cmp    byte ptr [ecx + edx], 0
        jne    loc_1_2771
loc_1_2783:
        test   byte ptr [eax + 0x10], 1
        je     loc_1_27FE
        push   0x8000300
        push   2
        push   edi
        call   sub_1_25A3
        add    esp, 0xc
        push   0
        push   edi
        call   sub_1_2621
        add    esp, 8
        test   al, 0x20
        je     loc_1_27B7
        mov    word ptr [esi], 6
        add    esi, 2
        mov    byte ptr [0x3676], 6          ; -> obj1:0x3676
loc_1_27B7:
        test   al, 0x40
        je     loc_1_27CA
        mov    word ptr [esi], 9
        add    esi, 2
        mov    byte ptr [0x3679], 9          ; -> obj1:0x3679
loc_1_27CA:
        test   al, 0x80
        je     loc_1_27DD
        mov    word ptr [esi], 0xc
        add    esi, 2
        mov    byte ptr [0x367c], 0xc        ; -> loc_1_367C
loc_1_27DD:
        test   ah, 1
        je     loc_1_27EE
        mov    word ptr [esi], 0xf
        mov    byte ptr [0x367f], 0xf        ; -> loc_1_367F
loc_1_27EE:
        push   0x8000000
        push   2
        push   edi
        call   sub_1_25A3
        add    esp, 0xc
loc_1_27FE:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2802:
        sub    ecx, ecx
        cmp    word ptr [0x36cc], cx         ; -> obj1:0x36CC
        je     loc_1_2829
        mov    eax, dword ptr [esp + 4]
        mov    edx, 0x36cc                   ; -> obj1:0x36CC
        mov    ax, word ptr [eax + 8]
loc_1_281A:
        cmp    word ptr [edx], ax
        je     loc_1_282C
        add    edx, 0x12
        inc    ecx
        cmp    word ptr [edx], 0
        jne    loc_1_281A
loc_1_2829:
        sub    eax, eax
loc_1_282B:
        ret    
loc_1_282C:
        imul   ecx, ecx, 0x12
        lea    eax, [ecx + 0x36cc]           ; -> obj1:0x36CC
        jmp    loc_1_282B
sub_1_2837:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        jne    loc_1_284E
        sub    ax, ax
        jmp    loc_1_286F
loc_1_284E:
        cmp    word ptr [eax + 0xa], 2
        jae    loc_1_285B
        mov    ax, 1
        jmp    loc_1_286F
loc_1_285B:
        push   esi
        call   sub_1_217D
        add    esp, 4
        cmp    eax, 1
        sbb    ecx, ecx
        add    ecx, 2
        mov    ax, cx
loc_1_286F:
        pop    esi
        ret    
sub_1_2871:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret    
sub_1_2884:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     loc_1_289A
        mov    ax, word ptr [eax + 0xc]
        jmp    loc_1_289D
loc_1_289A:
        sub    ax, ax
loc_1_289D:
        ret    
sub_1_289E:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     loc_1_28B4
        mov    ax, word ptr [eax + 0xe]
        jmp    loc_1_28B7
loc_1_28B4:
        sub    ax, ax
loc_1_28B7:
        ret    
sub_1_28B8:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     loc_1_28D1
        mov    ax, word ptr [eax + 0x10]
        and    eax, 4
        jmp    loc_1_28D3
loc_1_28D1:
        sub    eax, eax
loc_1_28D3:
        ret    
sub_1_28D4:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     loc_1_28ED
        mov    ax, word ptr [eax + 0x10]
        and    eax, 1
        jmp    loc_1_28EF
loc_1_28ED:
        sub    eax, eax
loc_1_28EF:
        ret    
sub_1_28F0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   esi
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     loc_1_290B
        mov    eax, dword ptr [eax + 2]
        or     eax, eax
        jne    loc_1_2918
loc_1_290B:
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_2C29
        add    esp, 8
loc_1_2918:
        cmp    word ptr [0x36a0], 0          ; -> obj1:0x36A0
        jb     loc_1_2940
        movzx  ecx, word ptr [0x36a0]        ; -> obj1:0x36A0
        inc    ecx
        mov    esi, 0x36a0                   ; -> obj1:0x36A0
        mov    ebx, dword ptr [esp + 0x18]
        mov    edi, ebx
        shr    ecx, 1
        rep movsd dword ptr es:[edi], dword ptr [esi]
        adc    ecx, ecx
        rep movsw word ptr es:[edi], word ptr [esi]
        jmp    loc_1_2944
loc_1_2940:
        mov    ebx, dword ptr [esp + 0x18]
loc_1_2944:
        movzx  ecx, word ptr [eax]
        lea    edx, [ecx + 1]
        or     edx, edx
        jl     loc_1_2967
        movzx  edx, word ptr [0x36a0]        ; -> obj1:0x36A0
        lea    edi, [ebx + edx*2 + 2]
        add    ecx, 2
        mov    esi, eax
        shr    ecx, 1
        rep movsd dword ptr es:[edi], dword ptr [esi]
        adc    ecx, ecx
        rep movsw word ptr es:[edi], word ptr [esi]
loc_1_2967:
        movzx  ecx, word ptr [eax]
        movzx  eax, word ptr [0x36a0]        ; -> obj1:0x36A0
        add    ecx, eax
        mov    eax, dword ptr [esp + 0x1c]
        lea    ecx, [ecx*2 + 6]
        mov    dword ptr [eax], ecx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2984:
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 0x18], 0
        cli    
        push   0x21
        call   sub_1_2D3E
        add    esp, 4
        mov    byte ptr [esp + 0x13], al
        push   0xa1
        call   sub_1_2D3E
        add    esp, 4
        mov    byte ptr [esp + 0x12], al
        push   0xff
        push   0x21
        call   sub_1_2D26
        add    esp, 8
        push   0xff
        push   0xa1
        call   sub_1_2D26
        add    esp, 8
        push   0x8000000
        push   2
        mov    esi, dword ptr [esp + 0x48]
        push   esi
        call   sub_1_25A3
        add    esp, 0xc
        push   0
        push   esi
        call   sub_1_2621
        add    esp, 8
        mov    word ptr [esp + 0x1a], ax
        lea    eax, [esp + 0x1c]
        push   eax
        push   esi
        call   sub_1_2741
        add    esp, 8
        lea    edi, [esi + 0x38]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        cmp    word ptr [esp + 0x1c], ax
        je     loc_1_2B8F
        lea    edi, [esp + 0x1c]
loc_1_2A1D:
        push   0x8000000
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0xc
        mov    ax, word ptr [edi]
        push   eax
        push   0
        push   esi
        call   sub_1_25D4
        add    esp, 0xc
        push   0xa
        push   0x20
        call   sub_1_2D26
        add    esp, 8
        push   0x20
        call   sub_1_2D3E
        add    esp, 4
        movzx  ax, al
        mov    word ptr [esp + 0x14], ax
        push   0xa
        push   0xa0
        call   sub_1_2D26
        add    esp, 8
        push   0xa0
        call   sub_1_2D3E
        add    esp, 4
        movzx  bp, al
        shl    bp, 8
        push   0x8000000
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0xc
        push   0
        push   0
        push   esi
        call   sub_1_25D4
        add    esp, 0xc
        push   0xa
        push   0x20
        call   sub_1_2D26
        add    esp, 8
        push   0x20
        call   sub_1_2D3E
        add    esp, 4
        movzx  bx, al
        push   0xa
        push   0xa0
        call   sub_1_2D26
        add    esp, 8
        push   0xa0
        call   sub_1_2D3E
        add    esp, 4
        movzx  ax, al
        shl    ax, 8
        add    bx, ax
        push   0x8000000
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0xc
        mov    ax, word ptr [edi]
        push   eax
        push   0
        push   esi
        call   sub_1_25D4
        add    esp, 0xc
        push   0xa
        push   0x20
        call   sub_1_2D26
        add    esp, 8
        push   0x20
        call   sub_1_2D3E
        add    esp, 4
        movzx  ax, al
        mov    word ptr [esp + 0x16], ax
        push   0xa
        push   0xa0
        call   sub_1_2D26
        add    esp, 8
        push   0xa0
        call   sub_1_2D3E
        add    esp, 4
        movzx  ax, al
        shl    ax, 8
        mov    cx, word ptr [esp + 0x16]
        add    cx, ax
        xor    cx, bx
        mov    ax, word ptr [esp + 0x14]
        add    ax, bp
        xor    bx, ax
        and    cx, bx
        movzx  edx, cx
        lea    eax, [edx - 1]
        test   edx, eax
        jne    loc_1_2B82
        or     word ptr [esp + 0x18], cx
        sub    ecx, ecx
loc_1_2B69:
        mov    eax, 1
        shl    eax, cl
        test   edx, eax
        jne    loc_1_2B7C
        inc    ecx
        cmp    ecx, 0x10
        jl     loc_1_2B69
        jmp    loc_1_2B82
loc_1_2B7C:
        mov    al, byte ptr [edi]
        mov    byte ptr [ecx + esi + 0x38], al
loc_1_2B82:
        add    edi, 2
        cmp    word ptr [edi], 0
        jne    loc_1_2A1D
loc_1_2B8F:
        push   0x8000000
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0xc
        push   dword ptr [esp + 0x1a]
        push   0
        push   esi
        call   sub_1_25D4
        add    esp, 0xc
        push   dword ptr [esp + 0x18]
        call   sub_1_0133
        add    esp, 4
        push   dword ptr [esp + 0x13]
        push   0x21
        call   sub_1_2D26
        add    esp, 8
        push   dword ptr [esp + 0x12]
        push   0xa1
        call   sub_1_2D26
        add    esp, 8
        sti    
        cmp    word ptr [esp + 0x18], 0
        jne    loc_1_2BF4
        sub    eax, eax
loc_1_2BE4:
        mov    cl, byte ptr [eax + 0x3690]   ; -> obj1:0x3690
        mov    byte ptr [eax + esi + 0x38], cl
        inc    eax
        cmp    eax, 0x10
        jl     loc_1_2BE4
loc_1_2BF4:
        mov    bx, word ptr [esi + 0x2c]
        or     bx, bx
        je     loc_1_2C21
        and    ebx, 0xf
        lea    edi, [esi + ebx + 0x38]
        cmp    byte ptr [edi], 0
        jne    loc_1_2C21
        push   esi
        call   sub_1_28B8
        add    esp, 4
        or     eax, eax
        je     loc_1_2C1F
        cmp    bx, 0xb
        mov    byte ptr [edi], 1
        je     loc_1_2C21
loc_1_2C1F:
        mov    byte ptr [edi], bl
loc_1_2C21:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    
sub_1_2C29:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        mov    ebx, 0x36a8                   ; -> loc_1_36A8
        mov    edi, dword ptr [esp + 0x10]
        push   7
        push   edi
        call   sub_1_260A
        add    esp, 8
        test   al, 0x80
        je     loc_1_2C8B
        cmp    word ptr [esp + 0x14], 1
        jbe    loc_1_2C81
        push   2
        push   edi
        call   sub_1_2638
        add    esp, 8
        mov    esi, eax
        xor    esi, 0x10000000
        push   esi
        push   2
        push   edi
        call   sub_1_25A3
        add    esp, 0xc
        push   7
        push   edi
        call   sub_1_260A
        add    esp, 8
        test   al, 0x80
        je     loc_1_2C8B
        jmp    loc_1_2D20
loc_1_2C81:
        mov    eax, 0x36a8                   ; -> loc_1_36A8
        jmp    loc_1_2D22
loc_1_2C8B:
        push   0
        push   9
        push   edi
        call   sub_1_25EF
        add    esp, 0xc
        push   0x8200
        push   0xe0
        push   edi
        call   sub_1_266D
        add    esp, 0xc
        test   esi, 0x10000000
        je     loc_1_2CB7
        push   0x10
        jmp    loc_1_2CB9
loc_1_2CB7:
        push   8
loc_1_2CB9:
        push   9
        push   edi
        call   sub_1_25EF
        add    esp, 0xc
        sub    esi, esi
loc_1_2CC6:
        push   7
        push   edi
        call   sub_1_260A
        add    esp, 8
        test   al, 0x20
        je     loc_1_2CE7
        push   0xc0
        push   edi
        call   sub_1_26A2
        add    esp, 8
        test   al, 0x20
        je     loc_1_2CFE
loc_1_2CE7:
        push   0
        push   0xd6d8
        call   sub_2_006C
        add    esp, 8
        inc    esi
        cmp    esi, 0x1e
        jl     loc_1_2CC6
        jmp    loc_1_2D03
loc_1_2CFE:
        mov    ebx, 0x36b4                   ; -> loc_1_36B4
loc_1_2D03:
        push   0
        push   9
        push   edi
        call   sub_1_25EF
        add    esp, 0xc
        push   0
        push   0xe0
        push   edi
        call   sub_1_266D
        add    esp, 0xc
loc_1_2D20:
        mov    eax, ebx
loc_1_2D22:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2D26:
        mov    dx, word ptr [esp + 4]
        mov    al, byte ptr [esp + 8]
        out    dx, al
        ret    
sub_1_2D31:
        mov    dx, word ptr [esp + 4]
        mov    ax, word ptr [esp + 8]
        out    dx, ax
        ret    
sub_1_2D3E:
        sub    esp, 4
        mov    dx, word ptr [esp + 8]
        in     al, dx
        mov    byte ptr [esp + 3], al
        mov    al, byte ptr [esp + 3]
        add    esp, 4
        ret    
sub_1_2D53:
        sub    esp, 4
        mov    dx, word ptr [esp + 8]
        in     ax, dx
        mov    word ptr [esp + 2], ax
        mov    ax, word ptr [esp + 2]
        add    esp, 4
        ret    
sub_1_2D6B:
        push   esi
        push   0x42
        call   sub_1_2D3E
        add    esp, 4
        movzx  esi, al
        push   0x42
        call   sub_1_2D3E
        add    esp, 4
        movzx  eax, al
        shl    eax, 8
        or     esi, eax
        mov    eax, esi
        pop    esi
        ret    
sub_1_2D8F:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        push   0x61
        call   sub_1_2D3E
        mov    byte ptr [esp + 0x17], al
        add    esp, 4
        and    al, 0xfd
        or     al, 1
        push   eax
        push   0x61
        call   sub_1_2D26
        mov    dword ptr [esp + 0x1c], 0xa
        mov    esi, dword ptr [esp + 0x24]
        add    esp, 8
loc_1_2DC1:
        push   0xb6
        push   0x43
        call   sub_1_2D26
        add    esp, 8
        push   0xff
        push   0x42
        call   sub_1_2D26
        add    esp, 8
        push   0xff
        push   0x42
        call   sub_1_2D26
        add    esp, 8
        push   0x4000
        push   0x20
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        call   sub_1_2D6B
        mov    ebp, eax
loc_1_2E05:
        call   sub_1_2D6B
        mov    ebx, eax
        cmp    ebp, ebx
        ja     loc_1_2E05
        push   0x40
        push   esi
        call   sub_1_26A2
        add    esp, 8
        and    ax, 0xff
        push   eax
        push   0x40
        push   esi
        call   sub_1_266D
        add    esp, 0xc
loc_1_2E2B:
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 4
        je     loc_1_2E2B
        call   sub_1_2D6B
        sub    ebx, eax
        mov    eax, 0x400000
        sub    edx, edx
        div    ebx
        add    edi, eax
        dec    dword ptr [esp + 0x14]
        jne    loc_1_2DC1
        push   0
        push   0x20
        push   esi
        call   sub_1_266D
        add    esp, 0xc
        push   dword ptr [esp + 0x13]
        push   0x61
        call   sub_1_2D26
        add    esp, 8
        mov    ecx, 0xa
        mov    eax, edi
        sub    edx, edx
        div    ecx
        cmp    eax, 0x400
        mov    cx, 0
        setbe  cl
        inc    cx
        mov    ax, cx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2EA0:
        sub    esp, 0xc
        cmp    dword ptr [0x3780], 0         ; -> loc_1_3780
        jne    loc_1_2F75
        mov    eax, 1
        mov    dword ptr [0x3798], 0xffffffff ; -> loc_1_3798
        cmp    dword ptr [0x379c], eax       ; -> loc_1_379C
        jbe    loc_1_2EDB
loc_1_2EC7:
        or     eax, eax
        je     loc_1_2EDB
        add    eax, eax
        shl    dword ptr [0x3798], 1         ; -> loc_1_3798
        cmp    dword ptr [0x379c], eax       ; -> loc_1_379C
        ja     loc_1_2EC7
loc_1_2EDB:
        push   0
        lea    eax, [esp + 8]
        push   0
        push   eax
        call   sub_2_0044
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_2F75
        push   0
        push   dword ptr [esp + 8]
        push   0
        push   0x37b0                        ; -> obj1:0x37B0
        call   sub_2_004C
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_2F75
        push   0
        push   0
        lea    eax, [esp + 8]
        push   dword ptr [0x37b0]            ; -> obj1:0x37B0
        push   eax
        call   sub_2_0074
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_2F75
        push   0
        push   0x24
        push   0x3788                        ; -> loc_1_3788
        push   1
        lea    eax, [esp + 0x18]
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_2_0084
        add    esp, 0x18
        or     eax, eax
        jne    loc_1_2F75
        push   0
        push   0
        push   dword ptr [0x37b0]            ; -> obj1:0x37B0
        call   sub_2_0064
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_2F75
        push   3
        push   0
        push   0x2f79                        ; -> loc_1_2F79
        push   dword ptr [0x37b0]            ; -> obj1:0x37B0
        call   sub_2_005C
        add    esp, 0x10
loc_1_2F75:
        add    esp, 0xc
        ret    
loc_1_2F79:
        sub    esp, 0xc
        cmp    dword ptr [esp + 0x10], 1
        je     loc_1_2F8A
        mov    eax, 1
        jmp    loc_1_2FFE
loc_1_2F8A:
        push   2
        lea    eax, [esp + 8]
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_2_007C
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_2FFE
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [esp], eax
        lea    eax, [esp + 8]
        push   0
        push   eax
        push   1
        lea    eax, [esp + 0xc]
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_2_008C
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_2FFC
        mov    eax, dword ptr [esp]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [0x3780], ecx       ; -> loc_1_3780
        mov    eax, dword ptr [eax + 8]
        sub    eax, ecx
        inc    eax
        mov    dword ptr [0x3784], eax       ; -> loc_1_3784
        push   eax
        push   ecx
        call   sub_2_0000
        add    esp, 0xc
        cmp    eax, -1
        jne    loc_1_2FF7
        mov    eax, 0x13
        jmp    loc_1_2FFE
loc_1_2FF7:
        mov    dword ptr [0x37b4], eax       ; -> obj1:0x37B4
loc_1_2FFC:
        sub    eax, eax
loc_1_2FFE:
        add    esp, 0xc
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3010:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        push   edx
        push   ecx
        push   ebx
        push   edi
        push   esi
        pushfd 
        cli    
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_260A
        add    esp, 8
        mov    bl, al
        push   0xa0
        push   dword ptr [ebp - 4]
        call   sub_1_26A2
        add    esp, 8
        and    ax, 0x3fff
        push   eax
        push   0xa0
        push   dword ptr [ebp - 4]
        call   sub_1_266D
        add    esp, 0xc
        push   ebx
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_25EF
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x22], 0
        popfd  
        pop    esi
        pop    edi
        pop    ebx
        pop    ecx
        pop    edx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_3076:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        push   edx
        push   ecx
        push   ebx
        push   edi
        push   esi
        pushfd 
        cli    
        sub    si, si
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_260A
        add    esp, 8
        mov    bl, al
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 0x27]
        or     cl, cl
        je     loc_1_30B4
        mov    edx, dword ptr [eax + 0x16]
        add    edx, 0xa
        movzx  eax, cl
loc_1_30AB:
        or     si, word ptr [edx]
        add    edx, 0x1a
        dec    eax
        jne    loc_1_30AB
loc_1_30B4:
        push   0xa0
        push   dword ptr [ebp - 4]
        call   sub_1_26A2
        add    esp, 8
        or     si, si
        je     loc_1_3146
        test   ah, 0x80
        je     loc_1_311A
        and    ax, 0x3fff
        push   eax
        push   0xa0
        push   dword ptr [ebp - 4]
        call   sub_1_266D
        add    esp, 0xc
        push   ebx
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_25EF
        add    esp, 0xc
        sti    
        mov    esi, dword ptr [ebp - 4]
        add    esi, 0x22
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_3104
        push   eax
        call   sub_2_0034
loc_1_3104:
        push   dword ptr [ebp - 4]
        push   0xfa0
        push   0x3010                        ; -> loc_1_3010
        call   sub_2_0018
        mov    dword ptr [esi], eax
        jmp    loc_1_3186
loc_1_311A:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 0x22], 0
        jne    loc_1_3186
        or     ax, 0x4000
        push   eax
        push   0xa0
        push   ecx
        call   sub_1_266D
        add    esp, 0xc
        push   ebx
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_25EF
        add    esp, 0xc
        jmp    loc_1_3186
loc_1_3146:
        and    ax, 0x7fff
        or     ax, 0x4000
        push   eax
        push   0xa0
        push   dword ptr [ebp - 4]
        call   sub_1_266D
        add    esp, 0xc
        push   ebx
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_25EF
        add    esp, 0xc
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0x22]
        add    esi, 0x22
        or     eax, eax
        je     loc_1_3186
        push   eax
        call   sub_2_0034
        mov    dword ptr [esi], 0
loc_1_3186:
        sti    
        push   dword ptr [ebp - 4]
        call   sub_1_319C
        add    esp, 4
        popfd  
        pop    esi
        pop    edi
        pop    ebx
        pop    ecx
        pop    edx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_319C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        push   0x1f4
        push   0x3076                        ; -> loc_1_3076
        call   sub_2_0018
        mov    dword ptr [esi + 0x1e], eax
        pop    esi
        ret    
sub_1_31B6:
        push   esi
        cli    
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x1e]
        or     eax, eax
        je     loc_1_31D0
        push   eax
        call   sub_2_0034
        mov    dword ptr [esi + 0x1e], 0
loc_1_31D0:
        mov    eax, dword ptr [esi + 0x22]
        or     eax, eax
        je     loc_1_31E4
        push   eax
        call   sub_2_0034
        mov    dword ptr [esi + 0x22], 0
loc_1_31E4:
        sti    
        pop    esi
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DBKVSSD '                  ; DDB_Name
        dd  016810009h                  ; DDB_Init_Order
        dd  DBKVSSD_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3240:
        add    al, 0xae
        int1   
        int1   
        inc    eax
        db  003h
loc_1_3246:
        pop    es
        scasb  al, byte ptr es:[edi]
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 001h, 000h, 000h, 000h, 001h, 000h, 010h, 000h
loc_1_3256:
        push   es
        scasb  al, byte ptr es:[edi]
        jg     loc_1_3262
        add    byte ptr [eax], al
        dec    dword ptr [edi]
        add    dword ptr [eax], eax
        add    byte ptr [eax], dl
loc_1_3262:
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        db  00Bh, 079h
loc_1_326A:
        add    dword ptr [esi + 0x32460084], ebp ; -> loc_1_3246
        add    byte ptr [eax], al
loc_1_3272:
        add    dword ptr [esi + 0x32560083], ebp ; -> loc_1_3256
        add    byte ptr [eax], al
loc_1_327A:
        add    ebp, dword ptr [esi + 0x33c00001] ; -> obj1:0x33C0
        add    byte ptr [eax], al
        push   0x32
        add    byte ptr [eax], al
        jb     0x32ba
        add    byte ptr [eax], al
        inc    eax
        xor    al, byte ptr [eax]
        add    bl, bl
        inc    ebx
        push   ebx
        push   esi
        add    byte ptr [edx], al
        inc    eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + edx], bh      ; -> loc_1_107C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x6c], al
        popal  
        jae    loc_1_3331
        sub    eax, 0x534f4942
        cmp    eax, 0x61480a31
        jb     0x3339
        ja     0x3338
        jb     loc_1_333E
        sub    eax, 0x473d4449
        outsb  dx, byte ptr gs:[esi]
        jb     0x334c
        arpl   word ptr [eax], sp
        push   ebx
        jne    0x3358
        jo     0x3359
        jb     loc_1_3360
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        xor    al, 0x20
        push   ebx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x65], bp
        je     loc_1_3319
        push   ebx
        jb     0x3373
        imul   esp, dword ptr [ebx + 0x65], 0x480a2073
        popal  
        jb     loc_1_336B
        ja     0x336a
        jb     loc_1_3370
        sub    eax, 0x69766552
        jae    0x337b
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    eax, 0x302e3156
loc_1_3319:
        xor    byte ptr [edx], cl
        dec    eax
        popal  
        jb     0x3383
        ja     loc_1_3382
        jb     0x3388
        sub    eax, 0x65746144
        cmp    eax, 0x38302f32
        das    
        cmp    dword ptr [edx + ecx], esi
loc_1_3331:
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
        sub    eax, 0x69766572
        jae    loc_1_33A5
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_1_333E:
        cmp    eax, 0x302f3230
        cmp    byte ptr [edi], ch
        cmp    dword ptr [edx + ecx], esi
        push   ebx
        outsd  dx, dword ptr [esi]
        je     0x33c4
        popal  
        jb     loc_1_33B5
        sub    eax, 0x65746144
        cmp    eax, 0x302f3230
        cmp    byte ptr [edi], ch
        cmp    dword ptr [edx + ecx], esi
        inc    ebx
loc_1_3360:
        outsd  dx, dword ptr [esi]
        jo     0x33dc
        jb     0x33ce
        push   0x6f4e2d74
loc_1_336B:
        je     loc_1_33D6
        arpl   word ptr [ebp + 0x3d], sp
loc_1_3370:
        inc    ebx
        outsd  dx, dword ptr [esi]
        jo     loc_1_33ED
        jb     0x33df
        push   0x63282074
        sub    dword ptr [eax], esp
        xor    dword ptr [ecx], edi
        cmp    dword ptr [ebx], esi
loc_1_3382:
        and    byte ptr [ecx + 0x74], al
        popal  
        bound  ebp, qword ptr [edi + 0x6f]
        imul   esp, dword ptr [eax], 0x49
        outsb  dx, byte ptr [esi]
        arpl   word ptr [esi], bp
        or     al, byte ptr [ebx + 0x61]
        jb     0x33f9
        sub    eax, 0x746e6f43
        jb     0x340b
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        jb     0x33de
        inc    esp
        inc    edx
        cmp    byte ptr [esi], dh
loc_1_33A5:
        xor    byte ptr [eax], bh
        xor    cl, byte ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    eax, dword ptr [ebx]
        add    eax, dword ptr [0xb090806]
loc_1_33B5:
        or     al, 0xe
        movups xmm2, xmmword ptr [edi + edx]
loc_1_33BB:
        push   es
        add    eax, 0x203
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [ebp + 0x78], al
        inc    ebx
        inc    ecx
        and    byte ptr [esi + 0x31], dl
        xor    eax, 0x50202030
        inc    ebx
loc_1_33D6:
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        and    byte ptr [esi + 0x32], dl
        xor    dword ptr cs:[eax], esi
        and    byte ptr [eax], ah
        push   ebx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x65], bp
        je     0x3409
        push   ebx
        jb     loc_1_3463
loc_1_33ED:
        imul   esp, dword ptr [ebx + 0x65], 0x560a0d73
        xor    dword ptr [esi], ebp
        xor    byte ptr [edx], dh
        and    byte ptr [eax], dh
        aaa    
        das    
        xor    dh, byte ptr [esi]
        das    
        cmp    dword ptr [ecx + 0x706f430a], esi
        jns    0x347a
        imul   esp, dword ptr [edi + 0x68], 0x43282074
        sub    dword ptr [eax], esp
        inc    esp
        popal  
        je     0x3476
        bound  ebp, qword ptr [edi + 0x6f]
        imul   esp, dword ptr [eax], 0x49
        outsb  dx, byte ptr [esi]
        arpl   word ptr [edi + 0x72], bp
        jo     0x3490
        jb     0x3484
        je     0x348a
        and    byte ptr fs:[ecx], dh
        cmp    dword ptr [ecx], edi
        xor    byte ptr [0x202e3439], ch
        add    byte ptr [eax], dl            ; -> loc_1_1110
        adc    dword ptr [eax], eax
        add    byte ptr [eax], dl            ; -> loc_1_1110
        adc    dword ptr [eax], eax
        add    byte ptr [eax], dl            ; -> loc_1_1110
        adc    dword ptr [eax], eax
        add    dh, ch                        ; -> loc_1_12EE
        adc    al, byte ptr [eax]
        add    al, bl                        ; -> loc_1_11D8
        adc    dword ptr [eax], eax
        add    byte ptr [ecx - 0x56ffffea], ch ; -> loc_1_16A9
        push   ss
        add    byte ptr [eax], al
        int3   
        or     al, byte ptr [eax]
        add    byte ptr [ebx + 0xb], cl      ; -> loc_1_0B4B
        add    byte ptr [eax], al
        jp     0x3462
        add    byte ptr [eax], al
        mov    bh, 0x12
        add    byte ptr [eax], al
        jno    0x3476
        add    byte ptr [eax], al
        adc    al, 0x13
loc_1_3463:
        add    byte ptr [eax], al
        rcl    byte ptr [eax + eax], cl
        add    byte ptr [eax - 0x2dffffed], dh ; -> loc_1_13B0
        adc    al, 0
        add    byte ptr [esi + 0x15], bh     ; -> loc_1_157E
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    edx, dword ptr [0x18040000]   ; -> sub_1_1804
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        mov    esp, 0x10000015               ; -> loc_1_1110
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34F8:
        or     al, byte ptr [edi]
        add    byte ptr [eax], al
        or     al, byte ptr [edi]
        add    byte ptr [eax], al
        or     al, byte ptr [edi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_350C:
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
loc_1_352C:
        sub    al, 7
        add    byte ptr [eax], al
        out    0xf, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_354C:
        sub    al, byte ptr [edi]
        add    byte ptr [eax], al
        pop    ss
        pop    es
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_3558:
        inc    ebx
        dec    eax
        dec    ecx
        push   eax
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_1_3560:
        dec    esi
        push   ebx
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_356C:
        dec    esi
        dec    ecx
        dec    edi
        push   edi
        dec    ecx
        dec    esi
        push   ebx
        add    byte ptr [esi + 0x4d], cl
        inc    ebp
        dec    ebp
        push   edi
        dec    ecx
        dec    esi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3580:
        push   eax
        push   edi
        push   edx
        push   esp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_358C:
        dec    ecx
        push   edx
        push   ecx
        pop    edi
        dec    ebp
        inc    ecx
        push   eax
        add    byte ptr [ebx + 0x4c], al
        dec    ebx
        pop    edi
        push   ebx
        pop    ecx
        dec    esi
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35A0:
        inc    ebx
        dec    esp
        dec    ebx
        pop    edi
        push   edx
        dec    ecx
        push   ebx
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [eax], al
loc_1_35AC:
        inc    edx
        inc    ebx
        dec    esp
        dec    ebx
        pop    edi
        push   ebx
        push   edx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35B8:
        dec    ecx
        dec    edi
        xor    dword ptr [esi], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35C0:
        dec    ebp
        inc    ebp
        dec    ebp
        xor    dword ptr [esi], esi
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x4f], cl
        inc    esp
        inc    ebp
        inc    ebx
        dec    edi
        inc    esp
        inc    ebp
        pop    edi
        dec    eax
        inc    ecx
        xor    dh, byte ptr [ebx]
        pop    edi
        xor    dword ptr [edi], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35DC:
        dec    ebp
        inc    ebp
        dec    ebp
        inc    esp
        inc    ebp
        inc    ebx
        dec    edi
        inc    esp
        inc    ebp
        pop    edi
        dec    eax
        inc    ecx
        xor    dh, byte ptr [ebx]
        pop    edi
        xor    dword ptr [edx], esi
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x43], al
        dec    esp
        dec    ebx
        pop    edi
        push   ebx
        push   edx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35FC:
        inc    edx
        inc    ebx
        dec    esp
        dec    ebx
        pop    edi
        push   ebx
        push   edx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3608:
        inc    ebx
        inc    ecx
        dec    esi
        push   ebp
        push   ebx
        inc    ebp
        pop    edi
        dec    eax
        dec    ebx
        inc    ebp
        dec    esi
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        pop    edi
        inc    ecx
        inc    ebx
        push   esp
        dec    ecx
        push   esi
        inc    ebp
        pop    edi
        dec    esp
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        pop    edi
        push   ebx
        dec    esi
        push   ebx
        pop    edi
        dec    esp
        inc    ebp
        push   esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        db  000h
loc_1_3634:
        add    dword ptr [ebx], eax
        add    al, 5
        push   es
        pop    es
        or     cl, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3640:
        add    eax, dword ptr [eax + 0xe0b0a07]
        add    byte ptr [ecx], al
        add    eax, dword ptr [eax + 0xe0a07]
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    al, 5
        push   es
        pop    es
        add    byte ptr [eax], al
        or     al, byte ptr [ecx]
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax + 0x6000700]
        or     al, byte ptr [ecx]
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax + 0x700]
        or     cl, byte ptr [ebx]
loc_1_367C:
        add    byte ptr [eax], al
        push   cs
loc_1_367F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax + 0x700]
        or     cl, byte ptr [ebx]
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax + 0x700]
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        loopne 0x36d7
        loopne 0x371f
        pushal 
loc_1_36A8:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], cl
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_36B4:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     dword ptr [eax], ecx
        add    byte ptr [ecx], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_36C0:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        pop    es
        add    al, byte ptr [eax]
        add    dword ptr [edi], eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + esi], dh      ; -> loc_1_3634
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     byte ptr [eax], al
        push   es
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + esi], dh      ; -> loc_1_3634
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        push   es
        add    byte ptr [edi], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + esi], dh      ; -> loc_1_3634
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        push   es
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + esi], dh      ; -> loc_1_3634
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        push   es
        add    byte ptr [eax + eax], al
        sal    byte ptr [esi], 0
        add    byte ptr [eax + 0x36], al     ; -> loc_1_3640
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    dword ptr [eax], eax
        or     byte ptr [eax], al
        sal    byte ptr [esi], 0
        add    byte ptr [eax + 0x36], al     ; -> loc_1_3640
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    dword ptr [eax], eax
        adc    al, 0
        sal    byte ptr [esi], 0
        add    byte ptr [eax + 0x36], al     ; -> loc_1_3640
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    dword ptr [eax], eax
        adc    eax, 0x36b400                 ; -> loc_1_36B4
        add    byte ptr [eax + 0x36], cl     ; -> obj1:0x3648
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    al, 0
        add    eax, 0x36b400                 ; -> loc_1_36B4
        add    byte ptr [eax + 0x36], cl     ; -> obj1:0x3648
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3780:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3784:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3788:
        add    dword ptr [eax], eax
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3798:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_379C:
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xaa  R-X
sub_2_0000:
        VMMCall  VMM._MapPhysToLinear            ; cd206c800100
        add    byte ptr [eax], al
sub_2_0008:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_2_0010:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_2_0018:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_2_0034:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_2_0044:
        VxDCall  CONFIGMG._CONFIGMG_Locate_DevNode ; cd2002803300
        add    byte ptr [eax], al
sub_2_004C:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        add    byte ptr [eax], al
sub_2_0054:
        VxDCall  CONFIGMG._CONFIGMG_Remove_SubTree ; cd200d803300
        add    byte ptr [eax], al
sub_2_005C:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        add    byte ptr [eax], al
sub_2_0064:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        add    byte ptr [eax], al
sub_2_006C:
        VxDCall  CONFIGMG._CONFIGMG_Yield        ; cd2015803300
        add    byte ptr [eax], al
sub_2_0074:
        VxDCall  CONFIGMG._CONFIGMG_Add_Empty_Log_Conf ; cd2018803300
        add    byte ptr [eax], al
sub_2_007C:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        add    byte ptr [eax], al
sub_2_0084:
        VxDCall  CONFIGMG._CONFIGMG_Add_Res_Des  ; cd201c803300
        add    byte ptr [eax], al
sub_2_008C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        add    byte ptr [eax], al
sub_2_0094:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        add    byte ptr [eax], al
sub_2_009C:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        add    byte ptr [eax], al
sub_2_00A4:
        VxDCall  Dev_2433.Service_8049h          ; cd2049803324
VxD__LTEXT ENDS

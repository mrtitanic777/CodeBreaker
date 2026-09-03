; PARALINK -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 3960 instructions recovered by recursive descent

Declare_Virtual_Device PARALINK, 1, 0, PARALINK_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x27c0  R-X
PARALINK_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
sub_1_000B:
        cld    
        push   esi
        push   edi
        lea    edi, [esi + 0x94]
        mov    esi, ebx
        mov    ecx, 0x30
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xor    eax, eax
        pop    edi
        pop    esi
        ret    
sub_1_0022:
        xor    eax, eax
        or     ebx, ebx
        je     loc_1_0033
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0x58]
        mov    dword ptr [ebx + 8], eax
loc_1_0033:
        ret    
loc_1_0034:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cld    
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_1_000B
        mov    dword ptr [esi + 0xc], eax
        or     al, 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0050:
        push   ebp
        mov    ebp, esp
        push   edi
        or     al, 1
        pop    edi
        leave  
        ret    
loc_1_0059:
        push   ebp
        mov    ebp, esp
        push   esi
        pop    esi
        leave  
        ret    
loc_1_0060:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_1_0022
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        xchg   dword ptr [esi + 0x34], eax
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx], eax
        or     al, 1
        pop    esi
        pop    edx
        pop    ebx
        leave  
        ret    
loc_1_0085:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [esi + 0xc], 0xffffffec
        cmp    bl, 5
        jne    loc_1_00BA
        test   byte ptr [esi + 0x12a], 1
        jne    loc_1_011D
        or     byte ptr [esi + 0x12a], 1
        call   sub_1_1B87
        xor    eax, eax
        jmp    loc_1_011D
loc_1_00BA:
        cmp    bl, 6
        jne    loc_1_00D8
        test   byte ptr [esi + 0x12a], 1
        je     loc_1_011D
        and    byte ptr [esi + 0x12a], 0xfe
        call   sub_1_1BB4
        xor    eax, eax
        jmp    loc_1_011D
loc_1_00D8:
        cmp    bl, 0x13
        jne    loc_1_00F3
        mov    edi, dword ptr [ebp + 0x10]
        or     edi, edi
        je     loc_1_011E
        cmp    edi, -1
        je     loc_1_011E
        mov    dword ptr [esi + 0x130], edi
        xor    eax, eax
        jmp    loc_1_011D
loc_1_00F3:
        cmp    bl, 0x10
        jne    loc_1_0101
        or     byte ptr [esi + 0x129], 0x20
        jmp    loc_1_011D
loc_1_0101:
        cmp    bl, 0x15
        jne    loc_1_010F
        and    byte ptr [esi + 0x129], 0xdf
        jmp    loc_1_011D
loc_1_010F:
        cmp    bl, 0x14
        jne    loc_1_011E
        and    byte ptr [esi + 0x129], 0xbf
        jmp    loc_1_011D
loc_1_011D:
        inc    eax
loc_1_011E:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
loc_1_0125:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0xc], 0
        mov    edx, dword ptr [edx + 0xe4]
        mov    eax, dword ptr [ebp + 0xc]
        not    eax
        mov    ecx, dword ptr [edx]
        and    eax, ecx
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx], ecx
        or     al, 1
        pop    edx
        pop    ecx
        leave  
        ret    
loc_1_0150:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        push   esi
        mov    eax, 0x13
        call   sub_1_03A8
        pop    esi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], ecx
        cmp    dword ptr [esi + 0x104], 0
        je     loc_1_0191
        cmp    dword ptr [esi + 0x120], 0
        je     loc_1_0191
        mov    eax, 2
        call   sub_1_022B
loc_1_0191:
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        mov    dword ptr [esi + 0x34], eax
        inc    eax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
loc_1_01A1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], 0
        mov    edx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edx + 0x4c]
        or     ebx, ebx
        je     loc_1_0207
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        cmp    ecx, ebx
        jle    loc_1_01C8
        xchg   ebx, ecx
loc_1_01C8:
        push   ecx
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx], ecx
        mov    esi, dword ptr [edx + 0x50]
        mov    eax, dword ptr [edx + 0x3c]
        add    eax, dword ptr [edx + 0x40]
        sub    eax, esi
        sub    eax, ecx
        mov    ebx, eax
        jge    loc_1_01E1
        add    ecx, eax
loc_1_01E1:
        cld    
        shr    cx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        jae    loc_1_01EB
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_01EB:
        cmp    ebx, 0
        jg     loc_1_0200
        mov    ecx, ebx
        neg    ecx
        mov    esi, dword ptr [edx + 0x3c]
        shr    cx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        jae    loc_1_0200
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_0200:
        pop    ecx
        sub    dword ptr [edx + 0x4c], ecx
        mov    dword ptr [edx + 0x50], esi
loc_1_0207:
        mov    eax, dword ptr [edx + 0x4c]
        cmp    eax, dword ptr [edx + 0x11c]
        jae    loc_1_0219
        and    byte ptr [edx + 0xe9], 0xfe
loc_1_0219:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
        db  000h, 000h
loc_1_0228:
        xor    eax, eax
        ret    
sub_1_022B:
        pushal 
        or     byte ptr [esi + 0xe9], al
        cmp    eax, 2
        jne    loc_1_0259
        mov    ebx, dword ptr [esi + 0x104]
        or     ebx, ebx
        je     loc_1_02AC
        inc    ebx
        je     loc_1_02AC
        push   0
        push   eax
        push   dword ptr [esi + 0x108]
        push   esi
        call   dword ptr [esi + 0x104]
        add    esp, 0x10
        jmp    loc_1_02AC
loc_1_0259:
        cmp    eax, 1
        jne    loc_1_0280
        mov    ebx, dword ptr [esi + 0xfc]
        or     ebx, ebx
        je     loc_1_02AC
        inc    ebx
        je     loc_1_02AC
        push   0
        push   eax
        push   dword ptr [esi + 0x100]
        push   esi
        call   dword ptr [esi + 0xfc]
        add    esp, 0x10
        jmp    loc_1_02AC
loc_1_0280:
        cmp    eax, 4
        jne    loc_1_02AC
        mov    ebx, dword ptr [esi + 0xf4]
        or     ebx, ebx
        je     loc_1_02AC
        inc    ebx
        je     loc_1_02AC
        mov    ebx, dword ptr [esi + 0xe4]
        mov    ebx, dword ptr [ebx]
        push   ebx
        push   eax
        push   dword ptr [esi + 0xf8]
        push   esi
        call   dword ptr [esi + 0xf4]
        add    esp, 0x10
loc_1_02AC:
        popal  
        ret    
sub_1_02AE:
        push   edi
        mov    edx, dword ptr [eax + 0x144]
        xor    ecx, ecx
loc_1_02B7:
        mov    cl, byte ptr [edx]
        mov    ch, byte ptr [edi]
        or     ecx, ecx
        je     loc_1_02D3
        call   sub_1_02D5
        xchg   ch, cl
        call   sub_1_02D5
        inc    edx
        inc    edi
        cmp    cl, ch
        je     loc_1_02B7
        or     esi, esi
loc_1_02D3:
        pop    edi
        ret    
sub_1_02D5:
        cmp    cl, 0x41
        jb     loc_1_02E2
        cmp    cl, 0x5a
        ja     loc_1_02E2
        or     cl, 0x20
loc_1_02E2:
        ret    
        db  000h
sub_1_02E4:
        push   eax
        push   ebx
        push   edx
        movzx  eax, byte ptr [esi + 0x151]
        mov    eax, dword ptr [eax*4 + 0x2550] ; -> loc_1_2550
        movzx  ebx, byte ptr [esi + 0x152]
        mov    edx, dword ptr [esi + 0xc8]
        call   dword ptr [eax + ebx*4]
        pop    edx
        pop    ebx
        pop    eax
        ret    
loc_1_0309:
        stc    
        ret    
loc_1_030B:
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0x10
        jb     loc_1_0319
        in     al, dx
        and    al, 0x10
loc_1_0319:
        or     al, 0xc
        out    dx, al
        dec    dx
        dec    dx
        clc    
        ret    
loc_1_0322:
        push   edx
        inc    dx
        inc    dx
        add    dx, 0x400
        mov    al, 0x35
        out    dx, al
        pop    edx
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0x10
        jb     loc_1_033E
        in     al, dx
        and    al, 0x10
loc_1_033E:
        or     al, 0xf
        out    dx, al
        dec    dx
        dec    dx
        clc    
        ret    
loc_1_0347:
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0x10
        jb     loc_1_0355
        in     al, dx
        and    al, 0x10
loc_1_0355:
        or     al, 0xf
        out    dx, al
        dec    dx
        dec    dx
        clc    
        ret    
sub_1_035E:
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0x10
        jb     loc_1_036C
        in     al, dx
        and    al, 0x10
loc_1_036C:
        mov    ah, al
        or     al, 0xc
        out    dx, al
        push   ax
        in     al, 0x61
        pop    ax
        mov    al, 0xe
        or     al, ah
        out    dx, al
        dec    dx
        dec    dx
        mov    al, 0
        out    dx, al
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xc
        or     al, ah
        out    dx, al
        dec    dx
        dec    dx
        clc    
        ret    
loc_1_0395:
        add    dx, 0x402
        mov    al, 0x35
        out    dx, al
        sub    dx, 0x402
        call   sub_1_035E
        ret    
sub_1_03A8:
        mov    dword ptr [0x2774], eax       ; -> loc_1_2774
        mov    dword ptr [0x276c], ecx       ; -> loc_1_276C
        mov    dword ptr [0x2770], edi       ; -> loc_1_2770
        mov    dword ptr [0x2768], esi       ; -> loc_1_2768
        mov    dword ptr [0x2788], 3         ; -> loc_1_2788
        inc    dword ptr [esi + 0x188]
loc_1_03CF:
        cmp    byte ptr [esi + 0x128], 0
        je     loc_1_0498
        mov    edx, dword ptr [esi + 0xc8]
        mov    dword ptr [0x2799], 0x54786d74 ; -> obj1:0x2799
        cld    
        cmp    byte ptr [esi + 0x151], 0
        jne    loc_1_03FB
        call   sub_1_22CA
loc_1_03FB:
        inc    edx
        call   sub_1_1046
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_04EF
        dec    edx
        movzx  eax, byte ptr [esi + 0x151]
        mov    edx, dword ptr [esi + 0xc8]
        call   sub_1_2560
        jb     loc_1_0498
        movzx  eax, byte ptr [esi + 0x150]
        shl    al, 2
        or     al, byte ptr [esi + 0x1fc]
        cmp    ax, 0xf
        jg     loc_1_0492
        movzx  ebx, byte ptr [esi + 0x151]
        mov    ebx, dword ptr [ebx*4 + 0x2580] ; -> loc_1_2580
        mov    edx, dword ptr [esi + 0xc8]
        call   dword ptr [ebx + eax*4]
        jb     loc_1_049F
        inc    dword ptr [esi + 0x18c]
        inc    dword ptr [esi + 0x190]
        cmp    dword ptr [esi + 0x190], 2
        jl     loc_1_0476
        cmp    byte ptr [esi + 0x151], 0
        jne    loc_1_0476
        mov    byte ptr [esi + 0x151], 1
loc_1_0476:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi + 0x194], eax
        mov    dword ptr [0x276c], eax       ; -> loc_1_276C
        push   ecx
        call   sub_1_1DC8
        pop    ecx
        xor    eax, eax
        clc    
        ret    
loc_1_0492:
        inc    dword ptr [esi + 0x1ec]
loc_1_0498:
        mov    byte ptr [esi + 0x151], 0
loc_1_049F:
        dec    dword ptr [0x2788]            ; -> loc_1_2788
        jge    loc_1_04C2
        xor    eax, eax
        mov    dword ptr [esi + 0x190], eax
        mov    dword ptr [0x276c], eax       ; -> loc_1_276C
        push   ecx
        call   sub_1_1DC8
        pop    ecx
        mov    eax, 2
        stc    
        ret    
loc_1_04C2:
        mov    edx, dword ptr [esi + 0xc8]
        inc    edx
        mov    ecx, dword ptr [esi + 0x170]
        shl    ecx, 1
loc_1_04D1:
        call   sub_1_1046
        and    al, 0xf0
        cmp    al, 0xe0
        je     loc_1_04E4
        cmp    al, 0xa0
        je     loc_1_04E4
        loop   loc_1_04D1
        jmp    loc_1_049F
loc_1_04E4:
        inc    dword ptr [esi + 0x1e4]
        jmp    loc_1_03CF
loc_1_04EF:
        inc    dword ptr [esi + 0x1e8]
        pushal 
        call   sub_1_1E2C
        popal  
        jmp    loc_1_03CF
loc_1_0501:
        stc    
        ret    
sub_1_0503:
        mov    dword ptr [0x2768], esi       ; -> loc_1_2768
        mov    dword ptr [0x2778], edi       ; -> loc_1_2778
        mov    edx, dword ptr [esi + 0xc8]
        mov    dword ptr [0x2799], 0x52656376 ; -> obj1:0x2799
        cld    
        inc    dword ptr [esi + 0x180]
        cmp    byte ptr [esi + 0x128], 0
        je     loc_1_05C8
        cmp    byte ptr [esi + 0x151], 0
        jne    loc_1_0541
        call   sub_1_22CA
loc_1_0541:
        movzx  eax, byte ptr [esi + 0x151]
        mov    edx, dword ptr [esi + 0xc8]
        call   sub_1_2570
        jb     loc_1_05C8
        movzx  eax, byte ptr [esi + 0x150]
        shl    al, 2
        or     al, byte ptr [esi + 0x1fc]
        cmp    ax, 0xf
        jg     loc_1_05C2
        movzx  ebx, byte ptr [esi + 0x151]
        mov    ebx, dword ptr [ebx*4 + 0x2590] ; -> obj1:0x2590
        mov    edx, dword ptr [esi + 0xc8]
        call   dword ptr [ebx + eax*4]
        jb     loc_1_05CF
        push   eax
        push   ecx
        inc    dword ptr [esi + 0x184]
        inc    dword ptr [esi + 0x190]
        cmp    dword ptr [esi + 0x190], 2
        jl     loc_1_05AD
        cmp    byte ptr [esi + 0x151], 0
        jne    loc_1_05AD
        mov    byte ptr [esi + 0x151], 1
loc_1_05AD:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi + 0x194], eax
        call   sub_1_1DC8
        pop    ecx
        pop    eax
        clc    
        ret    
loc_1_05C2:
        inc    dword ptr [esi + 0x1ec]
loc_1_05C8:
        mov    byte ptr [esi + 0x151], 0
loc_1_05CF:
        xor    eax, eax
        mov    dword ptr [esi + 0x190], eax
        push   ecx
        call   sub_1_1DC8
        pop    ecx
        mov    eax, 2
        stc    
        ret    
loc_1_05E5:
        stc    
        ret    
loc_1_05E7:
        mov    dword ptr [0x2799], 0x54486434 ; -> obj1:0x2799
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, 0xc
        inc    dx
        inc    dx
        out    dx, al
        sti    
        dec    dx
        dec    dx
        mov    al, 0xe5
        out    dx, al
        mov    al, 0xed
        out    dx, al
        mov    ecx, dword ptr [esi + 0x170]
        inc    dx
loc_1_0611:
        call   sub_1_1046
        cmp    al, 0xe8
        je     loc_1_0745
        cmp    al, 0xd8
        je     loc_1_0629
        loop   loc_1_0611
        jmp    loc_1_06C5
loc_1_0629:
        dec    dx
        mov    byte ptr [0x2796], 0          ; -> loc_1_2796
        mov    al, byte ptr [esi + 0x1ff]
        mov    ah, byte ptr [esi + 0x153]
        call   sub_1_076F
        jb     loc_1_06D1
        xor    ah, 0xff
        cmp    al, ah
        jne    loc_1_0701
        cmp    al, 3
        jg     loc_1_070D
        mov    byte ptr [esi + 0x1fc], al
        mov    eax, dword ptr [0x2774]       ; -> loc_1_2774
        mov    ah, byte ptr [esi + 0x150]
        cmp    ah, 3
        jg     loc_1_0719
        call   sub_1_076F
        jb     loc_1_06DD
        cmp    ax, word ptr [0x278c]         ; -> loc_1_278C
        jne    loc_1_0725
        mov    eax, dword ptr [0x276c]       ; -> loc_1_276C
        call   sub_1_076F
        jb     loc_1_06E9
        cmp    ax, word ptr [0x278c]         ; -> loc_1_278C
        jne    loc_1_0731
        mov    al, 0xea
        out    dx, al
        mov    al, 0xc
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        mov    ecx, dword ptr [esi + 0x170]
loc_1_06B5:
        call   sub_1_1046
        cmp    al, 0xd0
        loopne loc_1_06B5
        jcxz   loc_1_06F5
        dec    dx
        clc    
        ret    
loc_1_06C5:
        add    dword ptr [esi + 0x1b0], 0x1000
        jmp    loc_1_073D
loc_1_06D1:
        add    dword ptr [esi + 0x1b4], 0x1000
        jmp    loc_1_073D
loc_1_06DD:
        add    dword ptr [esi + 0x1b8], 0x1000
        jmp    loc_1_073D
loc_1_06E9:
        add    dword ptr [esi + 0x1bc], 0x1000
        jmp    loc_1_073D
loc_1_06F5:
        add    dword ptr [esi + 0x1c0], 0x1000
        jmp    loc_1_073D
loc_1_0701:
        add    dword ptr [esi + 0x1b0], 0x100000
        jmp    loc_1_073D
loc_1_070D:
        add    dword ptr [esi + 0x1b4], 0x100000
        jmp    loc_1_073D
loc_1_0719:
        add    dword ptr [esi + 0x1b8], 0x100000
        jmp    loc_1_073D
loc_1_0725:
        add    dword ptr [esi + 0x1bc], 0x100000
        jmp    loc_1_073D
loc_1_0731:
        add    dword ptr [esi + 0x1c0], 0x100000
        jmp    loc_1_073D
loc_1_073D:
        inc    dword ptr [esi + 0x1a0]
        jmp    loc_1_0753
loc_1_0745:
        inc    dword ptr [esi + 0x1c4]
        jmp    loc_1_0753
        db  0FFh, 086h, 098h, 001h, 000h, 000h
loc_1_0753:
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, 0xe5
        out    dx, al
        mov    al, 0xc
        or     al, byte ptr [esi + 0x136]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        stc    
        ret    
sub_1_076F:
        push   edx
        mov    bx, ax
        and    al, 0xf
        mov    byte ptr [0x278e], al         ; -> loc_1_278E
        add    al, byte ptr [0x2796]         ; -> loc_1_2796
        and    al, 0xf
        mov    byte ptr [0x2796], al         ; -> loc_1_2796
        xchg   al, bl
        shr    al, 4
        mov    byte ptr [0x278f], al         ; -> obj1:0x278F
        add    al, byte ptr [0x2796]         ; -> loc_1_2796
        and    al, 0xf
        mov    byte ptr [0x2796], al         ; -> loc_1_2796
        xchg   al, bh
        and    al, 0xf
        mov    byte ptr [0x2790], al         ; -> loc_1_2790
        add    al, byte ptr [0x2796]         ; -> loc_1_2796
        and    al, 0xf
        mov    byte ptr [0x2796], al         ; -> loc_1_2796
        xchg   al, ah
        shr    al, 4
        mov    byte ptr [0x2791], al         ; -> obj1:0x2791
        add    al, byte ptr [0x2796]         ; -> loc_1_2796
        and    al, 0xf
        mov    byte ptr [0x2796], al         ; -> loc_1_2796
        shl    al, 4
        or     al, ah
        shl    bh, 4
        or     bl, bh
        mov    ah, bl
        xchg   ah, al
        mov    word ptr [0x278c], ax         ; -> loc_1_278C
        mov    al, byte ptr [0x278e]         ; -> loc_1_278E
        shl    al, 1
        xor    al, 0xf0
        out    dx, al
        mov    ecx, dword ptr [esi + 0x16c]
        inc    dx
loc_1_07F1:
        in     al, dx
        test   al, 8
        loopne loc_1_07F1
        jcxz   loc_1_0818
        in     al, dx
        mov    byte ptr [0x278e], al         ; -> loc_1_278E
        dec    dx
        mov    al, byte ptr [0x278f]         ; -> obj1:0x278F
        shl    al, 1
        xor    al, 0xf1
        out    dx, al
        mov    ecx, dword ptr [esi + 0x16c]
        inc    dx
loc_1_0813:
        in     al, dx
        test   al, 8
        loope  loc_1_0813
loc_1_0818:
        jcxz   loc_1_0889
        in     al, dx
        mov    byte ptr [0x278f], al         ; -> obj1:0x278F
        dec    dx
        mov    al, byte ptr [0x2790]         ; -> loc_1_2790
        shl    al, 1
        xor    al, 0xf0
        out    dx, al
        mov    ecx, dword ptr [esi + 0x16c]
        inc    dx
loc_1_0835:
        in     al, dx
        test   al, 8
        loopne loc_1_0835
        jcxz   loc_1_0889
        in     al, dx
        mov    byte ptr [0x2790], al         ; -> loc_1_2790
        dec    dx
        mov    al, byte ptr [0x2791]         ; -> obj1:0x2791
        shl    al, 1
        xor    al, 0xf1
        out    dx, al
        mov    ecx, dword ptr [esi + 0x16c]
        inc    dx
loc_1_0857:
        in     al, dx
        test   al, 8
        loope  loc_1_0857
        jcxz   loc_1_0889
        in     al, dx
        mov    byte ptr [0x2791], al         ; -> obj1:0x2791
        dec    dx
        mov    ax, word ptr [0x278e]         ; -> loc_1_278E
        mov    bx, word ptr [0x2790]         ; -> loc_1_2790
        xchg   bl, ah
        and    ax, 0xf0f0
        and    bx, 0xf0f0
        shr    ax, 4
        or     ax, bx
        pop    edx
        clc    
        ret    
loc_1_0889:
        pop    edx
        stc    
        ret    
loc_1_088C:
        mov    dword ptr [0x2799], 0x52486434 ; -> obj1:0x2799
        mov    al, 0xc
        inc    dx
        inc    dx
        out    dx, al
        sti    
        dec    dx
        mov    ecx, dword ptr [esi + 0x174]
loc_1_08A6:
        call   sub_1_1046
        cmp    al, 0xe8
        je     loc_1_08B6
        loop   loc_1_08A6
        jmp    loc_1_0985
loc_1_08B6:
        mov    byte ptr [0x2796], 0          ; -> loc_1_2796
        dec    dx
        mov    al, 0xeb
        out    dx, al
        mov    al, byte ptr [esi + 0x150]
        cmp    al, 3
        jg     loc_1_09C1
        mov    ah, al
        and    al, 0xf
        mov    byte ptr [0x278e], al         ; -> loc_1_278E
        mov    al, ah
        shr    al, 4
        mov    byte ptr [0x278f], al         ; -> obj1:0x278F
        xor    ah, 0xff
        mov    al, ah
        and    al, 0xf
        mov    byte ptr [0x2790], al         ; -> loc_1_2790
        shr    ah, 4
        mov    byte ptr [0x2791], ah         ; -> obj1:0x2791
        call   sub_1_0A2F
        jb     loc_1_0991
        mov    byte ptr [esi + 0x1ff], al
        call   sub_1_0A36
        jb     loc_1_099D
        cmp    ah, 3
        jg     loc_1_09CD
        mov    byte ptr [esi + 0x1fc], ah
        movzx  eax, al
        mov    dword ptr [0x2780], eax       ; -> loc_1_2780
        call   sub_1_0A36
        jb     loc_1_09A9
        movzx  ebx, ax
        mov    dword ptr [0x277c], ebx       ; -> loc_1_277C
        cmp    bx, 0x1000
        ja     loc_1_0A0D
        cmp    bx, 1
        jl     loc_1_0A0D
        mov    eax, dword ptr [esi + 0x40]
        sub    eax, dword ptr [esi + 0x4c]
        cmp    bx, ax
        jg     loc_1_0A0D
        inc    dx
        mov    ecx, dword ptr [esi + 0x174]
loc_1_0967:
        call   sub_1_1046
        cmp    al, 0xd0
        loopne loc_1_0967
        jcxz   loc_1_09B5
        dec    dx
        mov    al, 0xea
        out    dx, al
        mov    al, 0xc
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        clc    
        ret    
loc_1_0985:
        add    dword ptr [esi + 0x1c8], 0x1000
        jmp    loc_1_09FD
loc_1_0991:
        add    dword ptr [esi + 0x1cc], 0x1000
        jmp    loc_1_09FD
loc_1_099D:
        add    dword ptr [esi + 0x1d0], 0x1000
        jmp    loc_1_09FD
loc_1_09A9:
        add    dword ptr [esi + 0x1d4], 0x1000
        jmp    loc_1_09FD
loc_1_09B5:
        add    dword ptr [esi + 0x1d8], 0x1000
        jmp    loc_1_09FD
loc_1_09C1:
        add    dword ptr [esi + 0x1c8], 0x100000
        jmp    loc_1_09FD
loc_1_09CD:
        add    dword ptr [esi + 0x1cc], 0x100000
        jmp    loc_1_09FD
        db  081h, 086h, 0D0h, 001h, 000h, 000h, 000h, 000h, 010h, 000h, 0EBh, 018h, 081h, 086h, 0D4h, 001h
        db  000h, 000h, 000h, 000h, 010h, 000h, 0EBh, 00Ch, 081h, 086h, 0D8h, 001h, 000h, 000h, 000h, 000h
        db  010h, 000h, 0EBh, 000h
loc_1_09FD:
        inc    dword ptr [esi + 0x1a4]
        jmp    loc_1_0A13
        db  0FFh, 086h, 09Ch, 001h, 000h, 000h, 0EBh, 006h
loc_1_0A0D:
        inc    dword ptr [esi + 0x1dc]
loc_1_0A13:
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, 0xe5
        out    dx, al
        mov    al, 0xc
        or     al, byte ptr [esi + 0x136]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        stc    
        ret    
sub_1_0A2F:
        push   edi
        mov    di, 0xffff
        jmp    loc_1_0A3A
sub_1_0A36:
        push   edi
        xor    di, di
loc_1_0A3A:
        push   edx
        mov    ecx, dword ptr [esi + 0x16c]
        inc    dx
loc_1_0A43:
        in     al, dx
        test   al, 8
        loopne loc_1_0A43
        jcxz   loc_1_0A83
        in     al, dx
        shr    al, 4
        mov    bl, al
        add    al, byte ptr [0x2796]         ; -> loc_1_2796
        and    al, 0xf
        mov    byte ptr [0x2796], al         ; -> loc_1_2796
        mov    byte ptr [0x2792], al         ; -> loc_1_2792
        dec    dx
        test   di, 0xffff
        je     loc_1_0A71
        mov    al, byte ptr [0x278e]         ; -> loc_1_278E
loc_1_0A71:
        shl    al, 1
        xor    al, 0xf0
        out    dx, al
        mov    ecx, dword ptr [esi + 0x16c]
        inc    dx
loc_1_0A7E:
        in     al, dx
        test   al, 8
        loope  loc_1_0A7E
loc_1_0A83:
        jcxz   loc_1_0AC2
        in     al, dx
        and    al, 0xf0
        mov    bh, al
        shr    al, 4
        add    al, byte ptr [0x2796]         ; -> loc_1_2796
        and    al, 0xf
        mov    byte ptr [0x2796], al         ; -> loc_1_2796
        mov    byte ptr [0x2793], al         ; -> obj1:0x2793
        dec    dx
        test   di, 0xffff
        je     loc_1_0AAE
        mov    al, byte ptr [0x278f]         ; -> obj1:0x278F
loc_1_0AAE:
        shl    al, 1
        xor    al, 0xf1
        out    dx, al
        or     bl, bh
        mov    ecx, dword ptr [esi + 0x16c]
        inc    dx
loc_1_0ABD:
        in     al, dx
        test   al, 8
        loopne loc_1_0ABD
loc_1_0AC2:
        jcxz   loc_1_0B35
        in     al, dx
        shr    al, 4
        mov    ah, al
        add    al, byte ptr [0x2796]         ; -> loc_1_2796
        and    al, 0xf
        mov    byte ptr [0x2796], al         ; -> loc_1_2796
        mov    byte ptr [0x2794], al         ; -> loc_1_2794
        dec    dx
        test   di, 0xffff
        je     loc_1_0AEB
        mov    al, byte ptr [0x2790]         ; -> loc_1_2790
loc_1_0AEB:
        shl    al, 1
        xor    al, 0xf0
        out    dx, al
        mov    ecx, dword ptr [esi + 0x16c]
        inc    dx
loc_1_0AF8:
        in     al, dx
        test   al, 8
        loope  loc_1_0AF8
        jcxz   loc_1_0B35
        in     al, dx
        and    al, 0xf0
        mov    bh, al
        shr    al, 4
        add    al, byte ptr [0x2796]         ; -> loc_1_2796
        and    al, 0xf
        mov    byte ptr [0x2796], al         ; -> loc_1_2796
        mov    byte ptr [0x2795], al         ; -> obj1:0x2795
        dec    dx
        test   di, 0xffff
        je     loc_1_0B28
        mov    al, byte ptr [0x2791]         ; -> obj1:0x2791
loc_1_0B28:
        shl    al, 1
        xor    al, 0xf1
        out    dx, al
        or     ah, bh
        mov    al, bl
        pop    edx
        pop    edi
        clc    
        ret    
loc_1_0B35:
        pop    edx
        pop    edi
        stc    
        ret    
loc_1_0B39:
        mov    dword ptr [0x2799], 0x54486438 ; -> obj1:0x2799
        mov    al, 0xed
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        out    dx, al
        mov    al, 0xe
        out    dx, al
        dec    dx
        mov    ecx, dword ptr [esi + 0x170]
loc_1_0B58:
        call   sub_1_1046
        and    al, 0xf0
        cmp    al, 0xe0
        jne    loc_1_0B7A
        dec    dx
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        or     al, byte ptr [esi + 0x136]
        out    dx, al
        jmp    loc_1_0CF6
loc_1_0B7A:
        cmp    al, 0x80
        je     loc_1_0B85
        loop   loc_1_0B58
        jmp    loc_1_0CFE
loc_1_0B85:
        mov    edx, dword ptr [esi + 0xc8]
        mov    byte ptr [0x2796], 0          ; -> loc_1_2796
        mov    al, byte ptr [esi + 0x1ff]
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xa0
        call   sub_1_1676
        jae    loc_1_0BB8
        jmp    loc_1_0CFE
loc_1_0BB8:
        mov    al, byte ptr [esi + 0x153]
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xe
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xe0
        call   sub_1_1676
        jae    loc_1_0BDE
        jmp    loc_1_0CFE
loc_1_0BDE:
        inc    dx
        inc    dx
        mov    al, 0xee
        out    dx, al
        dec    dx
        dec    dx
        in     al, dx
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        cmp    al, 3
        jg     loc_1_0D06
        mov    byte ptr [esi + 0x1fc], al
        inc    dx
        inc    dx
        mov    al, 0xed
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x20
        call   sub_1_1676
        jae    loc_1_0C17
        jmp    loc_1_0CFE
loc_1_0C17:
        in     al, dx
        cmp    al, byte ptr [0x2796]         ; -> loc_1_2796
        jne    loc_1_0D06
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        mov    al, 0xa5
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xc
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x60
        call   sub_1_1676
        jae    loc_1_0C4C
        jmp    loc_1_0CFE
loc_1_0C4C:
        mov    eax, dword ptr [0x2774]       ; -> loc_1_2774
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xa0
        call   sub_1_1676
        jae    loc_1_0C71
        jmp    loc_1_0CFE
loc_1_0C71:
        mov    al, byte ptr [esi + 0x150]
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xe
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xe0
        call   sub_1_1676
        jae    loc_1_0C94
        jmp    loc_1_0CFE
loc_1_0C94:
        mov    eax, dword ptr [0x276c]       ; -> loc_1_276C
        mov    bx, ax
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xd
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x20
        call   sub_1_1676
        jae    loc_1_0CB9
        jmp    loc_1_0CFE
loc_1_0CB9:
        mov    al, bh
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xc
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x60
        call   sub_1_1676
        jae    loc_1_0CD8
        jmp    loc_1_0CFE
loc_1_0CD8:
        mov    al, byte ptr [0x2796]         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xa0
        call   sub_1_1676
        jae    loc_1_0CF4
        jmp    loc_1_0CFE
loc_1_0CF4:
        clc    
        ret    
loc_1_0CF6:
        inc    dword ptr [esi + 0x1c4]
        jmp    loc_1_0D0C
loc_1_0CFE:
        inc    dword ptr [esi + 0x1a0]
        jmp    loc_1_0D0C
loc_1_0D06:
        inc    dword ptr [esi + 0x198]
loc_1_0D0C:
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, 0xe5
        out    dx, al
        mov    al, 0xf
        or     al, byte ptr [esi + 0x136]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        stc    
        ret    
loc_1_0D28:
        mov    dword ptr [0x2799], 0x52486438 ; -> obj1:0x2799
        inc    dx
        inc    dx
        mov    al, 0xef
        out    dx, al
        dec    dx
        dec    dx
        mov    ecx, dword ptr [esi + 0x174]
loc_1_0D43:
        inc    dx
        call   sub_1_1046
        and    al, 0xf0
        mov    ah, al
        dec    dx
        in     al, dx
        xor    ax, 0xe0ed
        loopne loc_1_0D43
        jcxz   loc_1_0D5C
        jmp    loc_1_0D61
loc_1_0D5C:
        jmp    loc_1_0EFB
loc_1_0D61:
        mov    edx, dword ptr [esi + 0xc8]
        inc    dx
        inc    dx
        mov    al, 0xeb
        out    dx, al
        dec    dx
        dec    dx
        mov    byte ptr [0x2796], 0          ; -> loc_1_2796
        mov    ah, 0xa0
        call   sub_1_1676
        jae    loc_1_0D87
        jmp    loc_1_0EFB
loc_1_0D87:
        in     al, dx
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        inc    dx
        inc    dx
        mov    al, 0xef
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xe0
        call   sub_1_1676
        jae    loc_1_0DA7
        jmp    loc_1_0EFB
loc_1_0DA7:
        in     al, dx
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        mov    byte ptr [esi + 0x1ff], al
        mov    al, byte ptr [esi + 0x150]
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xe
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x20
        call   sub_1_1676
        jae    loc_1_0DDA
        jmp    loc_1_0EFB
loc_1_0DDA:
        mov    al, byte ptr [0x2796]         ; -> loc_1_2796
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xd
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x60
        call   sub_1_1676
        jae    loc_1_0DFF
        jmp    loc_1_0EFB
loc_1_0DFF:
        inc    dx
        inc    dx
        mov    al, 0xed
        out    dx, al
        dec    dx
        dec    dx
        in     al, dx
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        cmp    al, 0xa5
        je     loc_1_0E1A
        jmp    loc_1_0F0B
loc_1_0E1A:
        inc    dx
        inc    dx
        mov    al, 0xec
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xa0
        call   sub_1_1676
        jae    loc_1_0E33
        jmp    loc_1_0EFB
loc_1_0E33:
        in     al, dx
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        movzx  eax, al
        mov    dword ptr [0x2780], eax       ; -> loc_1_2780
        inc    dx
        inc    dx
        mov    al, 0xef
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xe0
        call   sub_1_1676
        jae    loc_1_0E5B
        jmp    loc_1_0EFB
loc_1_0E5B:
        in     al, dx
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        cmp    al, 3
        jg     loc_1_0F0B
        mov    byte ptr [esi + 0x1fc], al
        inc    dx
        inc    dx
        mov    al, 0xee
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x20
        call   sub_1_1676
        jae    loc_1_0E86
        jmp    loc_1_0EFB
loc_1_0E86:
        in     al, dx
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        mov    bl, al
        inc    dx
        inc    dx
        mov    al, 0xed
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x60
        call   sub_1_1676
        jae    loc_1_0EA5
        jmp    loc_1_0EFB
loc_1_0EA5:
        in     al, dx
        add    byte ptr [0x2796], al         ; -> loc_1_2796
        mov    bh, al
        cmp    bx, 0x1000
        ja     loc_1_0F03
        cmp    bx, 1
        jl     loc_1_0F03
        mov    eax, dword ptr [esi + 0x40]
        sub    eax, dword ptr [esi + 0x4c]
        cmp    bx, ax
        jg     loc_1_0F03
        movzx  ebx, bx
        mov    dword ptr [0x277c], ebx       ; -> loc_1_277C
        inc    dx
        inc    dx
        mov    al, 0xec
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xa0
        call   sub_1_1676
        jae    loc_1_0EE5
        jmp    loc_1_0EFB
loc_1_0EE5:
        in     al, dx
        cmp    al, byte ptr [0x2796]         ; -> loc_1_2796
        jne    loc_1_0F0B
        inc    dx
        inc    dx
        mov    al, 0xef
        out    dx, al
        dec    dx
        dec    dx
        clc    
        ret    
loc_1_0EFB:
        inc    dword ptr [esi + 0x1a4]
        jmp    loc_1_0F11
loc_1_0F03:
        inc    dword ptr [esi + 0x1dc]
        jmp    loc_1_0F11
loc_1_0F0B:
        inc    dword ptr [esi + 0x19c]
loc_1_0F11:
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        or     al, byte ptr [esi + 0x136]
        out    dx, al
        pop    eax
        clc    
        ret    
loc_1_0F31:
        mov    dword ptr [0x2799], 0x5434    ; -> obj1:0x2799
        mov    eax, 0xc
        inc    dx
        inc    dx
        out    dx, al
        sti    
        dec    dx
        dec    dx
        mov    al, 0xe3
        out    dx, al
        inc    dx
        mov    ecx, dword ptr [esi + 0x170]
loc_1_0F55:
        call   sub_1_1046
        cmp    al, 0x98
        je     loc_1_0F65
        loop   loc_1_0F55
        jmp    loc_1_1037
loc_1_0F65:
        dec    dx
        mov    ecx, dword ptr [esi + 0x16c]
        shl    ecx, 2
        mov    dword ptr [0x2784], ecx       ; -> loc_1_2784
        mov    esi, dword ptr [0x2770]       ; -> loc_1_2770
        mov    edi, dword ptr [0x276c]       ; -> loc_1_276C
        shr    edi, 1
        jae    loc_1_0F87
        inc    edi
loc_1_0F87:
        lodsw  ax, word ptr [esi]
        xor    ax, 0x8888
        mov    bx, ax
        shl    al, 1
        out    dx, al
        mov    ah, bl
        shr    ah, 3
        or     ah, 1
        inc    dx
loc_1_0F9D:
        in     al, dx
        test   al, 8
        loopne loc_1_0F9D
        jcxz   loc_1_0FCC
        mov    al, ah
        dec    dx
        out    dx, al
        mov    ah, bh
        shl    ah, 1
        inc    dx
loc_1_0FB0:
        in     al, dx
        test   al, 8
        loope  loc_1_0FB0
        jcxz   loc_1_0FE1
        mov    al, ah
        dec    dx
        out    dx, al
        mov    ah, bh
        shr    ah, 3
        or     ah, 1
        inc    dx
loc_1_0FC7:
        in     al, dx
        test   al, 8
        loopne loc_1_0FC7
loc_1_0FCC:
        jcxz   loc_1_1019
        mov    al, ah
        dec    dx
        out    dx, al
        mov    ecx, dword ptr [0x2784]       ; -> loc_1_2784
        inc    dx
loc_1_0FDC:
        in     al, dx
        test   al, 8
        loope  loc_1_0FDC
loc_1_0FE1:
        jcxz   loc_1_1028
        dec    dx
        dec    di
        jne    loc_1_0F87
        mov    al, 0xe5
        out    dx, al
        xor    eax, eax
        xchg   dword ptr [0x276c], ecx       ; -> loc_1_276C
        clc    
loc_1_0FF6:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        push   eax
        mov    al, 0xe5
        out    dx, al
        mov    al, 0xc
        or     al, byte ptr [esi + 0x136]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        pop    eax
        ret    
loc_1_1019:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1b0]
        stc    
        jmp    loc_1_0FF6
loc_1_1028:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1b4]
        stc    
        jmp    loc_1_0FF6
loc_1_1037:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1a8]
        stc    
        jmp    loc_1_0FF6
sub_1_1046:
        push   ecx
        mov    ecx, 5
loc_1_104C:
        in     al, dx
        mov    ah, al
        in     al, dx
        and    ax, 0xf8f8
        cmp    ah, al
        loopne loc_1_104C
        pop    ecx
        ret    
loc_1_105A:
        mov    dword ptr [0x2799], 0x5234    ; -> obj1:0x2799
        mov    al, 0xc
        inc    dx
        inc    dx
        out    dx, al
        sti    
        mov    ecx, dword ptr [esi + 0x174]
        dec    dx
loc_1_1074:
        call   sub_1_1046
        cmp    al, 0x98
        loopne loc_1_1074
        jcxz   loc_1_10A3
        dec    dx
        mov    al, 0xe3
        out    dx, al
        mov    ecx, dword ptr [esi + 0x16c]
        shl    ecx, 2
        mov    dword ptr [0x2784], ecx       ; -> loc_1_2784
        mov    esi, dword ptr [0x277c]       ; -> loc_1_277C
        shr    si, 1
        jae    loc_1_10A1
        inc    si
loc_1_10A1:
        jmp    loc_1_10A8
loc_1_10A3:
        jmp    loc_1_114C
loc_1_10A8:
        mov    edi, dword ptr [0x2778]       ; -> loc_1_2778
loc_1_10AE:
        mov    ecx, dword ptr [0x2784]       ; -> loc_1_2784
        inc    dx
loc_1_10B6:
        in     al, dx
        test   al, 8
        loopne loc_1_10B6
        jcxz   loc_1_10E8
        in     al, dx
        shl    ax, 4
        dec    dx
        out    dx, al
        mov    bl, ah
        inc    dx
loc_1_10CA:
        in     al, dx
        test   al, 8
        loope  loc_1_10CA
        jcxz   loc_1_10FA
        in     al, dx
        mov    bh, al
        or     al, 1
        dec    dx
        out    dx, al
        and    bx, 0xf00f
        or     bl, bh
        inc    dx
loc_1_10E3:
        in     al, dx
        test   al, 8
        loopne loc_1_10E3
loc_1_10E8:
        jcxz   loc_1_113D
        in     al, dx
        shl    ax, 4
        dec    dx
        out    dx, al
        inc    dx
loc_1_10F5:
        in     al, dx
        test   al, 8
        loope  loc_1_10F5
loc_1_10FA:
        jcxz   loc_1_112E
        in     al, dx
        and    ax, 0xff0
        or     ah, al
        or     al, 1
        dec    dx
        out    dx, al
        mov    al, bl
        stosw  word ptr es:[edi], ax
        dec    si
        jne    loc_1_10AE
        inc    dx
loc_1_1113:
        call   sub_1_1046
        cmp    al, 0xa8
        je     loc_1_1120
        loop   loc_1_1113
        jmp    loc_1_114C
loc_1_1120:
        mov    ecx, dword ptr [0x277c]       ; -> loc_1_277C
        mov    eax, dword ptr [0x2780]       ; -> loc_1_2780
        clc    
        jmp    loc_1_1159
loc_1_112E:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1cc]
        stc    
        jmp    loc_1_1159
loc_1_113D:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1c8]
        stc    
        jmp    loc_1_1159
loc_1_114C:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1ac]
        stc    
loc_1_1159:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        push   eax
        mov    al, 0xe5
        out    dx, al
        mov    al, 0xc
        or     al, byte ptr [esi + 0x136]
        inc    dx
        inc    dx
        out    dx, al
        dec    dx
        dec    dx
        pop    eax
        ret    
loc_1_117C:
        mov    dword ptr [0x2799], 0x55547838 ; -> obj1:0x2799
        mov    al, 0xe3
        out    dx, al
        inc    dx
        mov    ecx, dword ptr [esi + 0x170]
loc_1_1191:
        call   sub_1_1046
        cmp    al, 0x98
        je     loc_1_11C0
        loop   loc_1_1191
        jmp    loc_1_11AF
loc_1_119E:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1b0]
        jmp    loc_1_127F
loc_1_11AF:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1a8]
        jmp    loc_1_127F
loc_1_11C0:
        dec    dx
        mov    al, 0xe8
        mov    ah, 0xea
        mov    bl, 0x80
        mov    bh, 0x90
        stc    
        call   sub_1_1AC6
        jb     loc_1_119E
        mov    al, 0xf1
        call   sub_1_1B44
        inc    dx
        mov    ecx, 0x4000
loc_1_11E0:
        in     al, dx
        test   al, 8
        loopne loc_1_11E0
        jcxz   loc_1_11F9
        inc    dx
        mov    al, 4
        out    dx, al
        dec    dx
        mov    ecx, 0x4000
loc_1_11F4:
        in     al, dx
        test   al, 0x10
        loope  loc_1_11F4
loc_1_11F9:
        jcxz   loc_1_1263
        mov    ecx, dword ptr [esi + 0x16c]
        mov    edi, ecx
        mov    esi, dword ptr [0x2770]       ; -> loc_1_2770
        mov    ebx, dword ptr [0x276c]       ; -> loc_1_276C
        shr    bx, 1
        jae    loc_1_1217
        inc    bx
loc_1_1217:
        dec    dx
        outsb  dx, byte ptr [esi]
        inc    dx
        inc    dx
        mov    al, 6
        out    dx, al
        dec    dx
        mov    ecx, edi
loc_1_1225:
        in     al, dx
        test   al, 0x80
        loope  loc_1_1225
        jcxz   loc_1_1271
        dec    dx
        outsb  dx, byte ptr [esi]
        inc    dx
        inc    dx
        mov    al, 4
        out    dx, al
        dec    dx
        mov    ecx, edi
loc_1_123B:
        in     al, dx
        test   al, 0x80
        loopne loc_1_123B
        jcxz   loc_1_1271
        dec    bx
        jne    loc_1_1217
        xchg   dword ptr [0x276c], ecx       ; -> loc_1_276C
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        xor    eax, eax
        clc    
        ret    
loc_1_1263:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1b4]
        jmp    loc_1_127F
loc_1_1271:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1b8]
        jmp    loc_1_127F
loc_1_127F:
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        pop    eax
        stc    
        ret    
loc_1_1295:
        mov    dword ptr [0x2799], 0x55527845 ; -> obj1:0x2799
        mov    ecx, dword ptr [esi + 0x174]
        inc    dx
loc_1_12A7:
        call   sub_1_1046
        cmp    al, 0x98
        loopne loc_1_12A7
        jcxz   loc_1_12F1
        dec    dx
        mov    al, 0xe3
        out    dx, al
        mov    al, 0xe0
        mov    ah, 0xe2
        mov    bl, 0xc0
        mov    bh, 0xd0
        clc    
        call   sub_1_1AC6
        jb     loc_1_12FF
        inc    dx
        mov    ecx, 0x4000
loc_1_12CF:
        in     al, dx
        test   al, 8
        loope  loc_1_12CF
        jcxz   loc_1_130D
        dec    dx
        mov    al, 0xe1
        call   sub_1_1B44
        inc    dx
        mov    ecx, 0x4000
loc_1_12E7:
        in     al, dx
        test   al, 8
        loopne loc_1_12E7
        jcxz   loc_1_130D
        jmp    loc_1_133F
loc_1_12F1:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1ac]
        jmp    loc_1_1329
loc_1_12FF:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1c8]
        jmp    loc_1_1329
loc_1_130D:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1cc]
        jmp    loc_1_1329
loc_1_131B:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1d0]
        jmp    loc_1_1329
loc_1_1329:
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        pop    eax
        stc    
        ret    
loc_1_133F:
        mov    ecx, 0x4000
loc_1_1344:
        in     al, dx
        test   al, 0x10
        loope  loc_1_1344
        jcxz   loc_1_130D
        inc    dx
        mov    al, 0xe4
        out    dx, al
        mov    ecx, dword ptr [esi + 0x16c]
        mov    esi, ecx
        mov    edi, dword ptr [0x2778]       ; -> loc_1_2778
        mov    ebx, dword ptr [0x277c]       ; -> loc_1_277C
        shr    bx, 1
        jae    loc_1_136C
        inc    bx
loc_1_136C:
        dec    dx
loc_1_136E:
        in     al, dx
        test   al, 0x80
        loope  loc_1_136E
        jcxz   loc_1_131B
        dec    dx
        insb   byte ptr es:[edi], dx
        inc    dx
        inc    dx
        mov    al, 0xe6
        out    dx, al
        dec    dx
        mov    ecx, esi
loc_1_1384:
        in     al, dx
        test   al, 0x80
        loopne loc_1_1384
        jcxz   loc_1_131B
        dec    dx
        insb   byte ptr es:[edi], dx
        inc    dx
        inc    dx
        mov    al, 0xe4
        out    dx, al
        dec    dx
        mov    ecx, esi
        dec    bx
        jne    loc_1_136E
        mov    ecx, dword ptr [0x277c]       ; -> loc_1_277C
        mov    eax, dword ptr [0x2780]       ; -> loc_1_2780
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        pop    eax
        clc    
        ret    
loc_1_13BF:
        mov    dword ptr [0x2799], 0x54387845 ; -> obj1:0x2799
        pushfd 
        cli    
        mov    al, 0x5a
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xd
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x20
        call   sub_1_1676
        jae    loc_1_13E7
        jmp    loc_1_14A4
loc_1_13E7:
        inc    dx
        inc    dx
        add    dx, 0x400
        mov    al, 0x75
        out    dx, al
        sub    dx, 0x400
        mov    al, 4
        out    dx, al
        dec    dx
        mov    ecx, 0x4000
loc_1_1402:
        in     al, dx
        and    al, 0xf0
        cmp    al, 0xd0
        loopne loc_1_1402
        jcxz   loc_1_147A
        add    dx, 0x3ff
        mov    esi, dword ptr [0x2770]       ; -> loc_1_2770
        mov    edi, dword ptr [0x276c]       ; -> loc_1_276C
        mov    bx, di
        and    bx, 0xf
        shr    di, 4
        inc    di
loc_1_142A:
        dec    di
        je     loc_1_144C
        mov    ecx, 0x10
        rep outsb dx, byte ptr [esi]
        inc    dx
        inc    dx
        mov    ecx, 0x4000
loc_1_143E:
        in     al, dx
        test   al, 1
        loope  loc_1_143E
        jcxz   loc_1_1488
        dec    dx
        dec    dx
        jmp    loc_1_142A
loc_1_144C:
        inc    dx
        inc    dx
        or     bx, bx
        je     loc_1_1462
        movzx  ecx, bx
        dec    dx
        dec    dx
        rep outsb dx, byte ptr [esi]
        inc    dx
        inc    dx
loc_1_1462:
        mov    ecx, 0x4000
loc_1_1467:
        in     al, dx
        test   al, 1
        loope  loc_1_1467
        mov    al, 0x35
        out    dx, al
        jcxz   loc_1_14B2
        xchg   dword ptr [0x276c], ecx       ; -> loc_1_276C
        jmp    loc_1_14F2
loc_1_147A:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1b0]
        jmp    loc_1_14C0
loc_1_1488:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1b4]
        jmp    loc_1_14C0
        db  08Bh, 035h
        dd  loc_1_2768               ; fixup
        db  0FFh, 086h, 0B8h, 001h, 000h, 000h, 0EBh, 01Ch
loc_1_14A4:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1a8]
        jmp    loc_1_14C0
loc_1_14B2:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1dc]
        jmp    loc_1_14C0
loc_1_14C0:
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        add    dx, 0x402
        mov    al, 0x35
        out    dx, al
        sub    dx, 0x402
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        or     al, byte ptr [esi + 0x136]
        out    dx, al
        dec    dx
        dec    dx
        pop    eax
        popfd  
        stc    
        ret    
loc_1_14F2:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        or     al, byte ptr [esi + 0x136]
        out    dx, al
        dec    dx
        dec    dx
        popfd  
        xor    eax, eax
        clc    
        ret    
loc_1_1517:
        mov    dword ptr [0x2799], 0x54387845 ; -> obj1:0x2799
        pushfd 
        cli    
        mov    ah, 0x20
        call   sub_1_1676
        jae    loc_1_1531
        jmp    loc_1_1607
loc_1_1531:
        in     al, dx
        cmp    al, 0x5a
        jne    loc_1_1607
        inc    dx
        inc    dx
        mov    al, 0xed
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0x70
        call   sub_1_1676
        jae    loc_1_1553
        jmp    loc_1_15F9
loc_1_1553:
        add    dx, 0x402
        mov    al, 0x75
        out    dx, al
        sub    dx, 0x400
        mov    al, 0xe0
        out    dx, al
        mov    ecx, 0x4000
        add    dx, 0x400
        mov    ebx, dword ptr [0x277c]       ; -> loc_1_277C
        mov    edi, dword ptr [0x2778]       ; -> loc_1_2778
loc_1_1579:
        in     al, dx
        test   al, 2
        jne    loc_1_1594
        test   al, 1
        loopne loc_1_1579
        jcxz   loc_1_15EB
        dec    dx
        dec    dx
        insb   byte ptr es:[edi], dx
        inc    dx
        inc    dx
        dec    bx
        je     loc_1_15C6
        jmp    loc_1_1579
loc_1_1594:
        mov    ecx, 0x10
        dec    dx
        dec    dx
        rep insb byte ptr es:[edi], dx
        inc    dx
        inc    dx
        sub    bx, 0x10
        js     loc_1_15AD
        je     loc_1_15C6
        jmp    loc_1_1579
loc_1_15AD:
        jmp    loc_1_15C6
        db  0B9h, 000h, 010h, 000h, 000h, 0ECh, 0A8h, 001h, 075h, 05Ch, 066h, 04Ah, 066h, 04Ah, 0ECh, 066h
        db  042h, 066h, 042h, 0E2h, 0F0h, 0EBh, 04Fh
loc_1_15C6:
        mov    al, 0x35
        out    dx, al
        sub    dx, 0x402
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        or     al, byte ptr [esi + 0x136]
        out    dx, al
        mov    ecx, dword ptr [0x277c]       ; -> loc_1_277C
        mov    eax, dword ptr [0x2780]       ; -> loc_1_2780
        jmp    loc_1_1623
loc_1_15EB:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1cc]
        jmp    loc_1_1648
loc_1_15F9:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1c8]
        jmp    loc_1_1648
loc_1_1607:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1ac]
        jmp    loc_1_1648
        db  08Bh, 035h
        dd  loc_1_2768               ; fixup
        db  0FFh, 086h, 0DCh, 001h, 000h, 000h, 0EBh, 025h
loc_1_1623:
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        or     al, byte ptr [esi + 0x136]
        out    dx, al
        dec    dx
        dec    dx
        pop    eax
        popfd  
        clc    
        ret    
loc_1_1648:
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        mov    al, 0xf
        or     al, byte ptr [esi + 0x136]
        out    dx, al
        add    dx, 0x400
        mov    al, 0x35
        out    dx, al
        sub    dx, 0x402
        pop    eax
        popfd  
        stc    
        ret    
sub_1_1676:
        inc    dx
        mov    ecx, 0x4000
loc_1_167D:
        in     al, dx
        and    al, 0xf0
        cmp    al, ah
        loopne loc_1_167D
        jcxz   loc_1_1693
        in     al, dx
        cmp    al, ah
        loopne loc_1_167D
        jcxz   loc_1_1693
        dec    dx
        clc    
        ret    
loc_1_1693:
        dec    dx
        stc    
        ret    
loc_1_1697:
        pushfd 
        mov    dword ptr [0x2799], 0x55547845 ; -> obj1:0x2799
        mov    al, 0xe3
        out    dx, al
        inc    dx
        mov    ecx, dword ptr [esi + 0x170]
loc_1_16AD:
        call   sub_1_1046
        cmp    al, 0x98
        je     loc_1_16F4
        loop   loc_1_16AD
        jmp    loc_1_16D1
loc_1_16BA:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        xor    eax, eax
        popfd  
        clc    
        ret    
loc_1_16D1:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1a8]
loc_1_16DD:
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        pop    eax
        popfd  
        stc    
        ret    
loc_1_16F4:
        dec    dx
        mov    al, 0xe8
        mov    ah, 0xea
        mov    bl, 0x80
        mov    bh, 0x90
        cli    
        stc    
        call   sub_1_1AC6
        jb     loc_1_17B8
        mov    al, 0xf3
        call   sub_1_1B44
        inc    dx
        mov    ecx, 0x4000
loc_1_1719:
        in     al, dx
        test   al, 8
        loopne loc_1_1719
        jcxz   loc_1_1758
        inc    dx
        add    dx, 0x400
        mov    al, 0x75
        out    dx, al
        mov    esi, dword ptr [0x2770]       ; -> loc_1_2770
        mov    edi, dword ptr [0x276c]       ; -> loc_1_276C
        mov    bx, di
        and    bx, 0xf
        shr    di, 4
        inc    di
        sub    dx, 0x400
        mov    al, 0x14
        out    dx, al
        dec    dx
        mov    ecx, 0x4000
loc_1_1753:
        in     al, dx
        test   al, 0x10
        loope  loc_1_1753
loc_1_1758:
        jcxz   loc_1_17BA
        add    dx, 0x3ff
loc_1_1760:
        dec    di
        je     loc_1_1782
        mov    ecx, 0x10
        rep outsb dx, byte ptr [esi]
        inc    dx
        inc    dx
        mov    ecx, 0x4000
loc_1_1774:
        in     al, dx
        test   al, 1
        loope  loc_1_1774
        jcxz   loc_1_17CC
        dec    dx
        dec    dx
        jmp    loc_1_1760
loc_1_1782:
        inc    dx
        inc    dx
        or     bx, bx
        je     loc_1_1798
        movzx  ecx, bx
        dec    dx
        dec    dx
        rep outsb dx, byte ptr [esi]
        inc    dx
        inc    dx
loc_1_1798:
        mov    ecx, 0x4000
loc_1_179D:
        in     al, dx
        test   al, 1
        loope  loc_1_179D
        in     al, dx
        and    al, 0x1f
        or     al, 0x35
        out    dx, al
        jcxz   loc_1_17CC
        xor    ecx, ecx
        xchg   dword ptr [0x276c], ecx       ; -> loc_1_276C
        jmp    loc_1_16BA
loc_1_17B8:
        nop    
        nop    
loc_1_17BA:
        nop    
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1b8]
        jmp    loc_1_16DD
loc_1_17CC:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1bc]
        jmp    loc_1_16DD
loc_1_17DD:
        pushfd 
        mov    dword ptr [0x2799], 0x55527845 ; -> obj1:0x2799
        mov    ecx, dword ptr [esi + 0x174]
        inc    dx
loc_1_17F0:
        call   sub_1_1046
        cmp    al, 0x98
        loopne loc_1_17F0
        jcxz   loc_1_182C
        dec    dx
        mov    al, 0xe3
        out    dx, al
        mov    al, 0xe0
        mov    ah, 0xe2
        mov    bl, 0xc0
        mov    bh, 0xd0
        cli    
        clc    
        call   sub_1_1AC6
        jb     loc_1_185F
        inc    dx
        mov    ecx, 0x4000
loc_1_1819:
        in     al, dx
        test   al, 8
        loope  loc_1_1819
        jcxz   loc_1_185F
        dec    dx
        mov    al, 0xe3
        call   sub_1_1B44
        jmp    loc_1_1870
loc_1_182C:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1ac]
        jmp    loc_1_1925
        db  08Bh, 035h
        dd  loc_1_2768               ; fixup
        db  0FFh, 086h, 0C8h, 001h, 000h, 000h, 0E9h, 0D7h, 000h, 000h, 000h, 08Bh, 035h
        dd  loc_1_2768               ; fixup
        db  0FFh, 086h, 0CCh, 001h, 000h, 000h, 0E9h, 0C6h, 000h, 000h, 000h
loc_1_185F:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1d0]
        jmp    loc_1_1925
loc_1_1870:
        mov    ebx, dword ptr [0x277c]       ; -> loc_1_277C
        mov    edi, dword ptr [0x2778]       ; -> loc_1_2778
        inc    dx
        mov    ecx, 0x4000
loc_1_1883:
        in     al, dx
        and    al, 0x18
        xor    al, 8
        cmp    al, 0x18
        loopne loc_1_1883
        jcxz   loc_1_185F
        inc    dx
        mov    al, 0x14
        out    dx, al
        add    dx, 0x400
        in     al, dx
        and    al, 0x1f
        or     al, 0x75
        out    dx, al
        sub    dx, 0x400
        mov    al, 0x20
        out    dx, al
        mov    ecx, 0x4000
        add    dx, 0x400
loc_1_18B1:
        in     al, dx
        test   al, 2
        jne    loc_1_18CC
        test   al, 1
        loopne loc_1_18B1
        jcxz   loc_1_1900
        dec    dx
        dec    dx
        insb   byte ptr es:[edi], dx
        inc    dx
        inc    dx
        dec    bx
        je     loc_1_18E5
        jmp    loc_1_18B1
loc_1_18CC:
        mov    ecx, 0x10
        dec    dx
        dec    dx
        rep insb byte ptr es:[edi], dx
        inc    dx
        inc    dx
        sub    bx, 0x10
        jns    loc_1_18E3
        jmp    loc_1_18E5
loc_1_18E3:
        jne    loc_1_18B1
loc_1_18E5:
        in     al, dx
        and    al, 0x1f
        or     al, 0x35
        out    dx, al
        sub    dx, 0x400
        mov    al, 0x14
        out    dx, al
        mov    ecx, dword ptr [0x277c]       ; -> loc_1_277C
        mov    eax, dword ptr [0x2780]       ; -> loc_1_2780
        jmp    loc_1_190E
loc_1_1900:
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1d4]
        jmp    loc_1_1925
loc_1_190E:
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        pop    eax
        popfd  
        clc    
        ret    
loc_1_1925:
        push   eax
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        pop    eax
        popfd  
        stc    
        ret    
loc_1_193C:
        pushfd 
        mov    dword ptr [0x2799], 0x55547850 ; -> obj1:0x2799
        mov    al, 0xe3
        out    dx, al
        inc    dx
        mov    ecx, dword ptr [esi + 0x170]
loc_1_1952:
        call   sub_1_1046
        cmp    al, 0x98
        loopne loc_1_1952
        jcxz   loc_1_1960
        jmp    loc_1_1962
loc_1_1960:
        jmp    loc_1_19CE
loc_1_1962:
        dec    dx
        mov    al, 0xe8
        mov    ah, 0xea
        mov    bl, 0x80
        mov    bh, 0x90
        cli    
        stc    
        call   sub_1_1AC6
        jb     loc_1_19CF
        mov    al, 0xf2
        call   sub_1_1B44
        inc    dx
        mov    ecx, 0x4000
loc_1_1983:
        in     al, dx
        test   al, 8
        loopne loc_1_1983
        jcxz   loc_1_19D0
        inc    dx
        mov    al, 0x14
        out    dx, al
        dec    dx
        mov    ecx, 0x4000
loc_1_1997:
        in     al, dx
        test   al, 0x10
        loope  loc_1_1997
        jcxz   loc_1_19D1
        mov    esi, dword ptr [0x2770]       ; -> loc_1_2770
        mov    ecx, dword ptr [0x276c]       ; -> loc_1_276C
        add    dx, 3
        rep outsb dx, byte ptr [esi]
        mov    ecx, dword ptr [0x276c]       ; -> loc_1_276C
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        xor    eax, eax
        popfd  
        clc    
        ret    
loc_1_19CE:
        nop    
loc_1_19CF:
        nop    
loc_1_19D0:
        nop    
loc_1_19D1:
        nop    
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1a8]
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        popfd  
        stc    
        ret    
loc_1_19F3:
        pushfd 
        mov    dword ptr [0x2799], 0x55527850 ; -> obj1:0x2799
        mov    ecx, dword ptr [esi + 0x174]
        inc    dx
loc_1_1A06:
        call   sub_1_1046
        cmp    al, 0x98
        loopne loc_1_1A06
        jcxz   loc_1_1A14
        jmp    loc_1_1A19
loc_1_1A14:
        jmp    loc_1_1AA1
loc_1_1A19:
        dec    dx
        mov    al, 0xe3
        out    dx, al
        mov    al, 0xe0
        mov    ah, 0xe2
        mov    bl, 0xc0
        mov    bh, 0xd0
        cli    
        clc    
        call   sub_1_1AC6
        jb     loc_1_1AA2
        inc    dx
        mov    ecx, 0x4000
loc_1_1A36:
        in     al, dx
        test   al, 8
        loope  loc_1_1A36
        jcxz   loc_1_1AA3
        mov    al, 0xe2
        dec    dx
        call   sub_1_1B44
        inc    dx
        mov    ecx, 0x4000
loc_1_1A4E:
        in     al, dx
        and    al, 0x18
        xor    al, 8
        cmp    al, 0x18
        loopne loc_1_1A4E
        jcxz   loc_1_1AA4
        inc    dx
        mov    al, 0xf4
        out    dx, al
        inc    dx
        inc    dx
        mov    edi, dword ptr [0x2778]       ; -> loc_1_2778
        mov    ecx, dword ptr [0x277c]       ; -> loc_1_277C
        rep insb byte ptr es:[edi], dx
        dec    dx
        dec    dx
        mov    al, 0xe4
        out    dx, al
        mov    al, 0xe6
        out    dx, al
        mov    al, 0xe4
        out    dx, al
        mov    al, 0xec
        out    dx, al
        mov    ecx, dword ptr [0x277c]       ; -> loc_1_277C
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        mov    eax, dword ptr [0x2780]       ; -> loc_1_2780
        popfd  
        clc    
        ret    
loc_1_1AA1:
        nop    
loc_1_1AA2:
        nop    
loc_1_1AA3:
        nop    
loc_1_1AA4:
        nop    
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        inc    dword ptr [esi + 0x1d4]
        mov    esi, dword ptr [0x2768]       ; -> loc_1_2768
        mov    edx, dword ptr [esi + 0xc8]
        stc    
        call   sub_1_035E
        popfd  
        stc    
        ret    
sub_1_1AC6:
        push   edx
        pushfd 
        jb     loc_1_1AE4
        push   eax
        inc    dx
        mov    ecx, 0x4000
loc_1_1AD2:
        in     al, dx
        and    al, 0xf8
        cmp    al, bl
        loopne loc_1_1AD2
        pop    eax
        jcxz   loc_1_1B30
        dec    dx
        out    dx, al
        inc    dx
        jmp    loc_1_1AF6
loc_1_1AE4:
        out    dx, al
        mov    ecx, 0x4000
        inc    dx
loc_1_1AEC:
        in     al, dx
        and    al, 0xf8
        cmp    al, bl
        loopne loc_1_1AEC
        jcxz   loc_1_1B34
loc_1_1AF6:
        popfd  
        pushfd 
        jb     loc_1_1B14
        mov    ecx, 0x4000
loc_1_1AFF:
        in     al, dx
        and    al, 0xf8
        cmp    al, bh
        loopne loc_1_1AFF
        jcxz   loc_1_1B38
        dec    dx
        mov    al, ah
        out    dx, al
        xor    al, al
        popfd  
        pop    edx
        clc    
        ret    
loc_1_1B14:
        dec    dx
        mov    al, ah
        out    dx, al
        inc    dx
        mov    ecx, 0x4000
loc_1_1B20:
        in     al, dx
        and    al, 0xf8
        cmp    al, bh
        loopne loc_1_1B20
        jcxz   loc_1_1B38
        xor    al, al
        popfd  
        clc    
        pop    edx
        ret    
loc_1_1B30:
        mov    al, 1
        jmp    loc_1_1B40
loc_1_1B34:
        mov    al, 2
        jmp    loc_1_1B40
loc_1_1B38:
        mov    al, 3
        jmp    loc_1_1B40
        db  0B0h, 004h, 0EBh, 000h
loc_1_1B40:
        popfd  
        stc    
        pop    edx
        ret    
sub_1_1B44:
        clc    
        mov    ah, al
        and    ah, 0xf
        je     loc_1_1B51
        test   al, 0x10
        jne    loc_1_1B51
        stc    
loc_1_1B51:
        out    dx, al
        push   ax
        in     al, 0x61
        pop    ax
        inc    dx
        inc    dx
        mov    al, 0xe
        out    dx, al
        push   ax
        in     al, 0x61
        pop    ax
        mov    al, 0xa
        out    dx, al
        mov    al, 0xe
        out    dx, al
        push   ax
        in     al, 0x61
        pop    ax
        mov    al, 0xc
        jae    loc_1_1B77
        or     al, 0xe0
loc_1_1B77:
        out    dx, al
        push   ax
        in     al, 0x61
        pop    ax
        dec    dx
        dec    dx
        mov    al, 0xe5
        out    dx, al
        ret    
        db  0C3h
sub_1_1B87:
        pushal 
        mov    byte ptr [esi + 0x128], 1
        mov    byte ptr [esi + 0x148], 1
        mov    eax, 0x11
        mov    ecx, 0xac
        lea    edi, [esi + 0x148]
        call   sub_1_03A8
        popal  
        mov    byte ptr [esi + 0x128], 1
        ret    
sub_1_1BB4:
        pushal 
        mov    byte ptr [esi + 0x148], 4
        mov    eax, 0x11
        mov    ecx, 0xac
        lea    edi, [esi + 0x148]
        call   sub_1_03A8
        popal  
        call   sub_1_1E13
        ret    
sub_1_1BD8:
        pushal 
        cmp    word ptr [0x2797], 0          ; -> obj1:0x2797
        jne    loc_1_1CBB
        cmp    dword ptr [0x2799], 0         ; -> obj1:0x2799
        jne    loc_1_1CBB
        mov    esi, dword ptr [0x2508]       ; -> obj1:0x2508
        or     esi, esi
        je     loc_1_1CBB
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_1CBB
loc_1_1C0E:
        mov    esi, eax
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, byte ptr [esi + 0x128]
        cmp    al, 0
        je     loc_1_1CA6
        mov    ebx, dword ptr [esi + 0x178]
        or     ebx, ebx
        je     loc_1_1CA6
        mov    eax, dword ptr [0x27a5]       ; -> obj1:0x27A5
        sub    eax, ebx
        cmp    eax, 0x7530
        jle    loc_1_1C8A
        pushal 
        mov    ebx, dword ptr [esi + 0x12c]
        mov    edx, dword ptr [esi + 0xe4]
        mov    ecx, dword ptr [edx]
        push   ecx
        or     ecx, 0x30
        mov    eax, dword ptr [esi + 0xe0]
        and    eax, ecx
        mov    dword ptr [edx], eax
        mov    ecx, 0xffffff5f
        and    byte ptr [ebx], cl
        call   sub_1_1E13
        xor    ebx, ebx
        mov    dword ptr [esi + 0x178], ebx
        pop    ebx
        cmp    dword ptr [esi + 0xf4], -1
        je     loc_1_1C87
        not    ebx
        and    eax, ebx
        je     loc_1_1C87
        mov    eax, 4
        call   sub_1_022B
loc_1_1C87:
        popal  
        jmp    loc_1_1CA6
loc_1_1C8A:
        mov    byte ptr [esi + 0x148], 3
        mov    eax, 0x11
        mov    ecx, 0xac
        lea    edi, [esi + 0x148]
        call   sub_1_03A8
loc_1_1CA6:
        mov    eax, esi
        mov    esi, dword ptr [0x2508]       ; -> obj1:0x2508
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_1CBB
        jmp    loc_1_1C0E
loc_1_1CBB:
        popal  
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_1CC0:
        pushal 
        pushfd 
        inc    dword ptr [0x279d]            ; -> obj1:0x279D
        call   sub_1_1D18
        popfd  
        popal  
        ret    
loc_1_1CD0:
        xor    eax, eax
        call   sub_1_1D18
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [0x27a5], eax       ; -> obj1:0x27A5
        sub    eax, dword ptr [0x27a9]       ; -> obj1:0x27A9
        cmp    eax, 0xbb8
        jl     loc_1_1CFE
        call   sub_1_1BD8
        mov    eax, dword ptr [0x27a5]       ; -> obj1:0x27A5
        mov    dword ptr [0x27a9], eax       ; -> obj1:0x27A9
loc_1_1CFE:
        mov    eax, 0x14
        mov    edx, esi
        mov    esi, 0x1cd0                   ; -> loc_1_1CD0
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x2510], esi       ; -> obj1:0x2510
        ret    
        db  090h
sub_1_1D18:
        cli    
        or     eax, eax
        je     loc_1_1D49
        mov    edi, eax
        mov    esi, dword ptr [0x2508]       ; -> obj1:0x2508
        or     esi, esi
        je     loc_1_1D49
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_1D49
loc_1_1D31:
        cmp    edi, dword ptr [eax + 0x13c]
        je     loc_1_1D43
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_1D49
        jmp    loc_1_1D31
loc_1_1D43:
        inc    byte ptr [eax + 0x135]
loc_1_1D49:
        cmp    dword ptr [0x2799], 0         ; -> obj1:0x2799
        jne    loc_1_1DC4
        cmp    word ptr [0x2797], 0          ; -> obj1:0x2797
        jne    loc_1_1DC4
        inc    word ptr [0x2797]             ; -> obj1:0x2797
        mov    esi, dword ptr [0x2508]       ; -> obj1:0x2508
        or     esi, esi
        je     loc_1_1DBD
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_1DBD
        sti    
loc_1_1D76:
        mov    esi, eax
        mov    edx, dword ptr [esi + 0xc8]
        mov    al, byte ptr [esi + 0x128]
        cmp    al, 0
        je     loc_1_1D9B
        inc    edx
        call   sub_1_1046
        and    al, 0xf0
        cmp    al, 0xe0
        jne    loc_1_1D9B
        call   sub_1_1E2C
        jmp    loc_1_1DAD
loc_1_1D9B:
        mov    eax, esi
        mov    esi, dword ptr [0x2508]       ; -> obj1:0x2508
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_1DAD
        jmp    loc_1_1D76
loc_1_1DAD:
        cli    
        dec    word ptr [0x2797]             ; -> obj1:0x2797
        call   sub_1_1DC8
        sti    
        clc    
        ret    
loc_1_1DBD:
        dec    word ptr [0x2797]             ; -> obj1:0x2797
loc_1_1DC4:
        sti    
        stc    
        ret    
        db  090h
sub_1_1DC8:
        cli    
        push   esi
        push   edi
        mov    dword ptr [0x2799], 0         ; -> obj1:0x2799
        mov    esi, dword ptr [0x2508]       ; -> obj1:0x2508
        or     esi, esi
        je     loc_1_1E0F
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_1E0F
loc_1_1DE7:
        cmp    byte ptr [eax + 0x135], 0
        je     loc_1_1E07
        mov    edi, eax
        mov    eax, dword ptr [edi + 0x13c]
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        mov    byte ptr [edi + 0x135], 0
        mov    eax, edi
loc_1_1E07:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_1_1DE7
loc_1_1E0F:
        pop    edi
        pop    esi
        sti    
        ret    
sub_1_1E13:
        push   eax
        push   edx
        mov    edx, dword ptr [esi + 0xc8]
        inc    dx
        inc    dx
        mov    al, 0xc
        out    dx, al
        mov    byte ptr [esi + 0x270], 0
        pop    edx
        pop    eax
        ret    
sub_1_1E2C:
        inc    dword ptr [0x27a1]            ; -> obj1:0x27A1
        mov    edx, dword ptr [esi + 0xc8]
        mov    edi, dword ptr [esi + 0x114]
        call   sub_1_0503
        jb     loc_1_1E72
        mov    dword ptr [0x2799], 0x446f506b ; -> obj1:0x2799
        sti    
        cmp    al, 0x13
        jne    loc_1_1E61
        mov    edi, dword ptr [esi + 0x114]
        call   sub_1_1E73
        jmp    loc_1_1E72
loc_1_1E61:
        cmp    al, 0x11
        jne    loc_1_1E72
        mov    edi, dword ptr [esi + 0x114]
        call   sub_1_1F31
        jmp    loc_1_1E72
loc_1_1E72:
        ret    
sub_1_1E73:
        push   eax
        push   ebx
        push   edx
        push   esi
        push   ecx
        mov    edx, esi
        mov    eax, dword ptr [edx + 0x40]
        sub    eax, dword ptr [edx + 0x4c]
        cmp    ecx, eax
        jg     loc_1_1F28
        mov    esi, edi
        mov    edi, dword ptr [edx + 0x54]
        mov    eax, dword ptr [edx + 0x3c]
        add    eax, dword ptr [edx + 0x40]
        sub    eax, edi
        sub    eax, ecx
        mov    ebx, eax
        jns    loc_1_1E9D
        add    ecx, ebx
loc_1_1E9D:
        mov    edi, dword ptr [edx + 0x54]
        cld    
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        jae    loc_1_1EA9
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_1EA9:
        cmp    ebx, 0
        jg     loc_1_1EBD
        mov    ecx, ebx
        neg    ecx
        mov    edi, dword ptr [edx + 0x3c]
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        jae    loc_1_1EBD
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_1EBD:
        mov    dword ptr [edx + 0x54], edi
        pop    ecx
        add    dword ptr [edx + 0x4c], ecx
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [edx + 0x178], eax
        mov    edi, dword ptr [edx + 0x130]
        mov    dword ptr [edi], eax
        and    byte ptr [edx + 0xe9], 0x7f
        cmp    dword ptr [edx + 0xfc], 0
        je     loc_1_1F23
        cmp    dword ptr [edx + 0x11c], -1
        je     loc_1_1F23
        test   byte ptr [edx + 0x129], 0x20
        je     loc_1_1F10
        test   byte ptr [edx + 0xe9], 1
        je     loc_1_1F05
        jmp    loc_1_1F23
loc_1_1F05:
        mov    ecx, dword ptr [edx + 0x4c]
        cmp    ecx, dword ptr [edx + 0x11c]
        jb     loc_1_1F23
loc_1_1F10:
        and    byte ptr [edx + 0xe9], 1
        mov    eax, 1
        mov    esi, edx
        call   sub_1_022B
loc_1_1F23:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        ret    
loc_1_1F28:
        inc    dword ptr [esi + 0x1dc]
        pop    ecx
        jmp    loc_1_1F23
sub_1_1F31:
        push   ebx
        push   edx
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi + 0x178], eax
        mov    ebx, dword ptr [esi + 0x12c]
        mov    edx, dword ptr [esi + 0xe4]
        mov    al, byte ptr [edi]
        cmp    al, 4
        jne    loc_1_1F91
        mov    ecx, dword ptr [edx]
        push   ecx
        or     ecx, 0x30
        mov    eax, dword ptr [esi + 0xe0]
        and    eax, ecx
        mov    dword ptr [edx], eax
        mov    ecx, 0xffffff5f
        and    byte ptr [ebx], cl
        call   sub_1_1E13
        pop    ebx
        cmp    dword ptr [esi + 0xf4], -1
        je     loc_1_2005
        not    ebx
        and    eax, ebx
        je     loc_1_2005
        mov    eax, 4
        call   sub_1_022B
        jmp    loc_1_2005
loc_1_1F91:
        cmp    al, 2
        jne    loc_1_1FCF
        mov    ecx, dword ptr [edx]
        push   ecx
        or     ecx, 0x30
        mov    ecx, 0xa0
        and    byte ptr [ebx], cl
        mov    dword ptr [edx], ecx
        mov    byte ptr [esi + 0x270], 2
        mov    eax, dword ptr [esi + 0xe0]
        and    eax, ecx
        pop    ebx
        cmp    dword ptr [esi + 0xf4], -1
        je     loc_1_2005
        not    ebx
        and    eax, ebx
        je     loc_1_2005
        mov    eax, 4
        call   sub_1_022B
        jmp    loc_1_2005
loc_1_1FCF:
        cmp    al, 1
        jne    loc_1_2001
        pushal 
        mov    byte ptr [esi + 0x128], 1
        mov    byte ptr [esi + 0x148], 2
        mov    eax, 0x11
        mov    ecx, 0xac
        lea    edi, [esi + 0x148]
        call   sub_1_03A8
        popal  
        mov    byte ptr [esi + 0x128], 2
        jmp    loc_1_2005
loc_1_2001:
        cmp    al, 3
        jne    loc_1_2005
loc_1_2005:
        pop    edx
        pop    ebx
        ret    
sub_1_2008:
        pushal 
        mov    edx, dword ptr [esi + 0xc8]
        call   sub_1_2037
        or     ax, ax
        je     loc_1_2035
        mov    al, 5
        mov    byte ptr [esi + 0x152], al
        mov    edx, dword ptr [esi + 0xc8]
        inc    dx
        inc    dx
        add    dx, 0x400
        mov    al, 0x35
        out    dx, al
        jmp    loc_1_2035
loc_1_2035:
        popal  
        ret    
sub_1_2037:
        push   ecx
        push   edx
        add    dx, 0x402
        in     al, dx
        push   eax
        mov    al, 0xc0
        mov    ah, al
        out    dx, al
        push   ax
        in     al, 0x61
        pop    ax
        in     al, dx
        and    al, 0xe0
        cmp    al, ah
        jne    loc_1_209F
        sub    cx, cx
loc_1_2055:
        in     al, dx
        test   al, 2
        jne    loc_1_206E
        dec    dx
        dec    dx
        out    dx, al
        inc    dx
        inc    dx
        inc    cx
        cmp    cx, 0x800
        jae    loc_1_209F
        jmp    loc_1_2055
loc_1_206E:
        mov    word ptr [esi + 0x154], cx
        sub    cx, cx
loc_1_2078:
        in     al, dx
        test   al, 1
        jne    loc_1_2091
        dec    dx
        dec    dx
        in     al, dx
        inc    dx
        inc    dx
        inc    cx
        cmp    cx, 0x800
        jae    loc_1_209F
        jmp    loc_1_2078
loc_1_2091:
        mov    word ptr [esi + 0x156], cx
        pop    eax
        out    dx, al
        mov    ax, cx
        jmp    loc_1_20B2
loc_1_209F:
        pop    eax
        out    dx, al
        sub    ax, ax
        mov    word ptr [esi + 0x154], ax
        mov    word ptr [esi + 0x156], ax
loc_1_20B2:
        pop    edx
        pop    ecx
        ret    
        db  0FAh, 053h, 051h, 052h, 066h, 033h, 0D2h, 066h, 0C7h, 005h
        dd  loc_1_27B0               ; fixup
        db  00Eh, 003h, 066h, 0C7h, 005h
        dd  loc_1_27B2               ; fixup
        db  000h, 043h, 0E8h, 0F7h, 000h, 000h, 000h, 066h, 03Dh, 086h, 004h, 075h, 012h, 066h, 0C7h, 005h
        dd  loc_1_27B0               ; fixup
        db  00Ah, 007h, 066h, 0C7h, 005h
        dd  loc_1_27B2               ; fixup
        db  000h, 044h, 066h, 03Dh, 086h, 003h, 074h, 00Bh, 066h, 03Dh, 086h, 004h, 074h, 005h, 0E9h, 0C6h
        db  000h, 000h, 000h, 0E4h, 070h, 050h, 0E4h, 074h, 050h, 0E4h, 076h, 050h, 0B0h, 010h, 0E6h, 070h
        db  0E4h, 074h, 03Ch, 010h, 075h, 005h, 0E9h, 0A5h, 000h, 000h, 000h, 0B0h, 05Ah, 0E6h, 074h, 0E4h
        db  074h, 024h, 07Fh, 03Ch, 05Ah, 074h, 005h, 0E9h, 094h, 000h, 000h, 000h, 0B0h, 025h, 0E6h, 074h
        db  0E4h, 074h, 024h, 07Fh, 03Ch, 025h, 074h, 005h, 0E9h, 083h, 000h, 000h, 000h, 0B0h, 000h, 0E6h
        db  074h, 0E4h, 076h, 050h, 0F6h, 0D0h, 08Ah, 0E0h, 0E6h, 076h, 0E4h, 076h, 038h, 0C4h, 075h, 070h
        db  058h, 0E6h, 076h, 0B0h, 001h, 0E6h, 074h, 0E4h, 076h, 050h, 0F6h, 0D0h, 08Ah, 0E0h, 0E6h, 076h
        db  0E4h, 076h, 038h, 0C4h, 075h, 05Ah, 058h, 0E6h, 076h, 066h, 0E5h, 022h, 066h, 08Bh, 0C8h, 066h
        db  0A9h, 001h, 000h, 075h, 017h, 066h, 0B8h, 000h, 080h, 0E6h, 023h, 066h, 050h, 0E4h, 061h, 0E4h
        db  061h, 0E4h, 061h, 066h, 058h, 086h, 0C4h, 0E6h, 022h, 066h, 0E7h, 022h, 0E4h, 022h, 024h, 0F0h
        db  00Ch, 003h, 0E6h, 022h, 066h, 08Bh, 015h
        dd  loc_1_27B0               ; fixup
        db  066h, 0EDh, 066h, 08Bh, 0D0h, 066h, 0B8h, 000h, 080h, 0E6h, 023h, 086h, 0C4h, 0E6h, 022h, 066h
        db  0E7h, 022h, 066h, 08Bh, 0C1h, 066h, 0E7h, 022h, 066h, 0A9h, 001h, 000h, 075h, 007h, 066h, 00Dh
        db  000h, 001h, 066h, 0E7h, 022h, 058h, 0E6h, 074h, 058h, 0E6h, 076h, 058h, 0E6h, 070h, 066h, 08Bh
        db  0C2h, 0FBh, 05Ah, 059h, 05Bh, 0C3h, 09Ch, 051h, 052h, 08Bh, 0D4h, 066h, 083h, 0E4h, 0FCh, 09Ch
        db  058h, 08Bh, 0C8h, 035h, 000h, 000h, 004h, 000h, 050h, 09Dh, 09Ch, 058h, 08Bh, 0E2h, 066h, 0BAh
        db  086h, 003h, 033h, 0C1h, 074h, 017h, 066h, 0BAh, 086h, 004h, 08Bh, 0C1h, 035h, 000h, 000h, 020h
        db  000h, 050h, 09Dh, 09Ch, 058h, 033h, 0C1h, 074h, 004h, 066h, 0BAh, 086h, 005h, 066h, 08Bh, 0C2h
        db  05Ah, 059h, 09Dh, 0C3h, 0B4h, 0E2h, 0CDh, 017h, 080h, 0FCh, 0E2h, 075h, 003h, 066h, 033h, 0C0h
        db  0C3h, 053h, 051h, 052h, 066h, 0E5h, 022h, 066h, 08Bh, 0C8h, 066h, 0A9h, 001h, 000h, 075h, 00Dh
        db  066h, 0B8h, 000h, 080h, 0E6h, 023h, 086h, 0C4h, 0E6h, 022h, 066h, 0E7h, 022h, 0E4h, 022h, 024h
        db  0F0h, 00Ch, 003h, 0E6h, 022h, 066h, 08Bh, 015h
        dd  loc_1_27B0               ; fixup
        db  066h, 0EDh, 066h, 08Bh, 01Dh
        dd  loc_1_27B2               ; fixup
        db  066h, 033h, 0D2h, 038h, 0FCh, 075h, 053h, 066h, 0B8h, 000h, 001h, 066h, 0E7h, 022h, 0E4h, 023h
        db  08Ah, 0D8h, 0A8h, 080h, 074h, 014h, 066h, 0BAh, 023h, 0FCh, 0ECh, 066h, 0BAh, 023h, 0F0h, 0ECh
        db  066h, 0BAh, 023h, 0C0h, 0ECh, 066h, 0BAh, 023h, 000h, 0ECh, 0B0h, 061h, 0E6h, 024h, 0E4h, 025h
        db  08Ah, 0F8h, 00Ch, 008h, 0E6h, 025h, 0E6h, 0FBh, 0B0h, 002h, 0E6h, 0AEh, 0E4h, 0AFh, 066h, 08Bh
        db  0D0h, 0E6h, 0AFh, 0E6h, 0F9h, 0B0h, 061h, 0E6h, 024h, 08Ah, 0C7h, 0E6h, 025h, 0F6h, 0C3h, 080h
        db  074h, 008h, 0B0h, 0FAh, 0E6h, 024h, 0B0h, 001h, 0E6h, 025h, 066h, 0B8h, 000h, 080h, 0E6h, 023h
        db  086h, 0C4h, 0E6h, 022h, 066h, 0E7h, 022h, 066h, 08Bh, 0C1h, 066h, 0E7h, 022h, 066h, 0A9h, 001h
        db  000h, 075h, 007h, 066h, 00Dh, 000h, 001h, 066h, 0E7h, 022h, 066h, 08Bh, 0C2h, 05Ah, 059h, 05Bh
        db  0C3h
sub_1_22CA:
        push   ebx
        push   ecx
        push   edx
        pushfd 
        cli    
        mov    edx, dword ptr [esi + 0xc8]
        inc    dword ptr [esi + 0x1e0]
        mov    al, 0xe5
        out    dx, al
        inc    dx
        inc    dx
        in     al, dx
        mov    ch, al
        and    ch, 0x10
        mov    al, 0xc
        or     al, ch
        out    dx, al
        push   ax
        in     al, 0x61
        pop    ax
        mov    al, 0xe
        or     al, ch
        out    dx, al
        dec    dx
        dec    dx
        mov    al, 0
        out    dx, al
        mov    al, 0xe0
        out    dx, al
        inc    dx
        in     al, dx
        mov    bh, al
        dec    dx
        mov    al, 0xe1
        out    dx, al
        inc    dx
        in     al, dx
        mov    bl, al
        dec    dx
        mov    al, 0xe2
        out    dx, al
        inc    dx
        in     al, dx
        mov    ah, al
        dec    dx
        mov    al, 0xe3
        out    dx, al
        inc    dx
        in     al, dx
        push   eax
        inc    dx
        mov    al, 0xc
        or     al, ch
        out    dx, al
        dec    dx
        dec    dx
        mov    al, 0xe5
        out    dx, al
        pop    eax
        and    bx, 0x7878
        and    ax, 0x7878
        shl    bh, 1
        shr    bl, 3
        or     bl, bh
        shl    ah, 1
        shr    al, 3
        or     al, ah
        cmp    bl, 0xca
        jne    loc_1_23BA
        cmp    al, 2
        je     loc_1_2359
        cmp    al, 1
        je     loc_1_2359
        jmp    loc_1_23BA
loc_1_2359:
        cmp    byte ptr [esi + 0x152], 0
        jne    loc_1_23A4
        mov    byte ptr [esi + 0x152], 2
        inc    dx
        inc    dx
        in     al, dx
        mov    ch, al
        and    ch, 0x10
        mov    al, 0xec
        or     al, ch
        out    dx, al
        dec    dx
        dec    dx
        mov    ah, 0xa5
        mov    al, ah
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        in     al, dx
        cmp    al, ah
        jne    loc_1_23A4
        xor    ah, 0xff
        mov    al, ah
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        in     al, dx
        cmp    al, ah
        jne    loc_1_23A4
        mov    byte ptr [esi + 0x152], 1
        jne    loc_1_23A4
loc_1_23A4:
        mov    byte ptr [esi + 0x151], 3
        inc    dx
        inc    dx
        mov    al, 0xc
        or     al, ch
        out    dx, al
        dec    dx
        dec    dx
        jmp    loc_1_2432
loc_1_23BA:
        inc    dx
        inc    dx
        mov    ebx, 3
loc_1_23C3:
        in     al, dx
        mov    ah, al
        and    ah, 0x10
        mov    al, 4
        or     al, ah
        out    dx, al
        push   ax
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        pop    ax
        dec    dx
        mov    ecx, 5
loc_1_23E3:
        in     al, dx
        test   al, 8
        je     loc_1_2420
        loop   loc_1_23E3
        inc    dx
        mov    al, 0xc
        or     al, ah
        out    dx, al
        push   ax
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        in     al, 0x61
        pop    ax
        dec    dx
        mov    ecx, 5
loc_1_2406:
        in     al, dx
        test   al, 8
        jne    loc_1_2420
        loop   loc_1_2406
        inc    dx
        dec    bx
        jg     loc_1_23C3
        dec    dx
        dec    dx
        mov    byte ptr [esi + 0x151], 2
        jmp    loc_1_2432
loc_1_2420:
        inc    dx
        mov    al, 0xc
        or     al, ah
        out    dx, al
        dec    dx
        dec    dx
        mov    byte ptr [esi + 0x151], 0
loc_1_2432:
        movzx  eax, byte ptr [esi + 0x152]
        movzx  ebx, byte ptr [esi + 0x151]
        imul   eax, eax, 4
        mov    al, byte ptr [ebx + eax + 0x2538] ; -> obj1:0x2538
        mov    byte ptr [esi + 0x150], al
        clc    
        call   sub_1_02E4
        popfd  
        pop    edx
        pop    ecx
        pop    ebx
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PARALINK'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  PARALINK_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  loc_1_0034               ; fixup
        dd  loc_3_0304               ; fixup
        dd  loc_1_0050               ; fixup
        dd  loc_1_0059               ; fixup
        dd  loc_3_032C               ; fixup
        dd  loc_3_03F9               ; fixup
        dd  loc_1_0060               ; fixup
        dd  loc_3_0422               ; fixup
        dd  loc_3_0442               ; fixup
        dd  loc_1_0085               ; fixup
        dd  loc_3_047D               ; fixup
        dd  loc_3_04A3               ; fixup
        dd  loc_1_0125               ; fixup
        dd  loc_1_0150               ; fixup
        dd  loc_1_01A1               ; fixup
        dd  loc_3_0578               ; fixup
        dd  loc_3_0521               ; fixup
        dd  loc_3_04C9               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_3_05D0               ; fixup
        dd  loc_3_05F3               ; fixup
        dd  loc_3_061D               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F4h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 001h
        db  000h, 000h, 001h, 002h, 000h, 000h, 001h, 002h, 000h, 000h, 003h, 003h
loc_1_2550:
        mov    al, byte ptr [0xa0000025]     ; -> loc_1_25A0
        and    eax, 0x25b80000               ; -> loc_1_25B8
        add    byte ptr [eax], al
        dd  loc_1_25D0               ; fixup
sub_1_2560:
        out    5, eax
        add    byte ptr [eax], al
        out    5, eax
        add    byte ptr [eax], al
        cmp    dword ptr [ebx], ecx
        add    byte ptr [eax], al
        out    5, eax
        add    byte ptr [eax], al
sub_1_2570:
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        mov    word ptr [eax], cs
        add    byte ptr [eax], al
        sub    byte ptr [0x88c0000], cl      ; -> loc_1_088C
        add    byte ptr [eax], al
loc_1_2580:
        call   0xe80025aa                    ; -> obj1:0x25E8
        and    eax, 0x26280000               ; -> loc_1_2628
        add    byte ptr [eax], al
        push   0xa8000026                    ; -> loc_1_26A8
        add    byte ptr es:[eax], al
        test   al, 0x26
        add    byte ptr [eax], al
        call   0x280025c3                    ; -> obj1:0x2728
        daa    
        add    byte ptr [eax], al
loc_1_25A0:
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
        or     eax, dword ptr [ebx]
        add    byte ptr [eax], al
loc_1_25B8:
        inc    edi
        add    eax, dword ptr [eax]
        add    byte ptr [ecx], cl            ; -> loc_1_0309
        add    eax, dword ptr [eax]
        add    byte ptr [edi + 3], al        ; -> loc_1_0347
        add    byte ptr [eax], al
        inc    edi
        add    eax, dword ptr [eax]
        add    byte ptr [edi + 3], al        ; -> loc_1_0347
        add    byte ptr [eax], al
        and    al, byte ptr [ebx]
        add    byte ptr [eax], al
loc_1_25D0:
        pop    esi
        add    eax, dword ptr [eax]
        add    byte ptr [esi + 3], bl        ; -> sub_1_035E
        add    byte ptr [eax], al
        pop    esi
        add    eax, dword ptr [eax]
        add    byte ptr [esi + 3], bl        ; -> sub_1_035E
        add    byte ptr [eax], al
        pop    esi
        add    eax, dword ptr [eax]
        add    byte ptr [ebp + 0x31000003], dl ; -> loc_1_0395
        sldt   word ptr [eax]
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
loc_1_2628:
        add    dword ptr [0x5010000], eax    ; -> loc_1_0501
        add    byte ptr [eax], al
        add    dword ptr [0x5010000], eax    ; -> loc_1_0501
        add    byte ptr [eax], al
        add    dword ptr [0x5010000], eax    ; -> loc_1_0501
        add    byte ptr [eax], al
        add    dword ptr [0x5010000], eax    ; -> loc_1_0501
        add    byte ptr [eax], al
        add    dword ptr [0x5010000], eax    ; -> loc_1_0501
        add    byte ptr [eax], al
        add    dword ptr [0x5010000], eax    ; -> loc_1_0501
        add    byte ptr [eax], al
        add    dword ptr [0x5010000], eax    ; -> loc_1_0501
        add    byte ptr [eax], al
        add    dword ptr [0x13bf0000], eax   ; -> loc_1_13BF
        add    byte ptr [eax], al
loc_1_2668:
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        jl     0x268f
        add    byte ptr [eax], al
        jl     0x2693
        add    byte ptr [eax], al
        jl     0x2697
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        jl     0x269f
        add    byte ptr [eax], al
        cmp    al, 0x19
        add    byte ptr [eax], al
        cmp    al, 0x19
        add    byte ptr [eax], al
        xor    dword ptr [edi], ecx
        add    byte ptr [eax], al
        jl     0x26af
        add    byte ptr [eax], al
        xchg   edi, eax
        push   ss
        add    byte ptr [eax], al
        xchg   edi, eax
        push   ss
        add    byte ptr [eax], al
loc_1_26A8:
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
loc_1_26E8:
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        in     eax, 5
        add    byte ptr [eax], al
        pop    ss
        adc    eax, 0x105a0000               ; -> loc_1_105A
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        pop    edx
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        xchg   ebp, eax
        adc    al, byte ptr [eax]
        add    byte ptr [ebp - 0x6affffee], dl ; -> loc_1_1295
        adc    al, byte ptr [eax]
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        xchg   ebp, eax
        adc    al, byte ptr [eax]
        add    bl, dh                        ; -> loc_1_19F3
        sbb    dword ptr [eax], eax
        add    bl, dh                        ; -> loc_1_19F3
        sbb    dword ptr [eax], eax
        add    byte ptr [edx + 0x10], bl     ; -> loc_1_105A
        add    byte ptr [eax], al
        xchg   ebp, eax
        adc    al, byte ptr [eax]
        add    ch, bl                        ; -> loc_1_17DD
        pop    ss
        add    byte ptr [eax], al
        fst    qword ptr [edi]
        add    byte ptr [eax], al
loc_1_2768:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_276C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2770:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2774:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2778:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_277C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2780:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2784:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2788:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_278C:
        add    byte ptr [eax], al
loc_1_278E:
        add    byte ptr [eax], al
loc_1_2790:
        add    byte ptr [eax], al
loc_1_2792:
        add    byte ptr [eax], al
loc_1_2794:
        add    byte ptr [eax], al
loc_1_2796:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_27B0:
        add    byte ptr [eax], al
loc_1_27B2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x48  R-X
loc_2_0000:
        cmp    dword ptr [0x250c], 0         ; -> obj1:0x250C
        jne    loc_2_0043
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [0x250c], ebx       ; -> obj1:0x250C
        VxDCall  BIOS.Service_0000h              ; cd2000002b00
        jb     loc_2_0044
        mov    eax, 4
        mov    ecx, 0x2a0
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_2_0044
        mov    dword ptr [0x2508], esi       ; -> obj1:0x2508
        push   0x634                         ; -> loc_3_0634
        VxDCall  BIOS.Service_0001h              ; cd2001002b00
        add    esp, 4
loc_2_0043:
        clc    
loc_2_0044:
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x7b0  R-X
loc_3_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    dword ptr [0x2799], 0x4f      ; -> obj1:0x2799
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [0x2508]       ; -> obj1:0x2508
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_3_0021:
        je     loc_3_004E
        call   sub_1_02AE
        je     loc_3_0032
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jmp    loc_3_0021
loc_3_0032:
        mov    esi, eax
        cmp    dword ptr [esi + 0x64], 0x534d5446
        je     loc_3_0058
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_3_0764
        jae    loc_3_006C
        jb     loc_3_0058
        call   sub_3_0794
loc_3_004E:
        mov    eax, 0xfffffff6
        jmp    loc_3_02D9
loc_3_0058:
        mov    eax, 0xfffffffe
        cmp    dword ptr [esi + 0xc8], -1
        jne    loc_3_02D9
        jmp    loc_3_004E
loc_3_006C:
        xor    eax, eax
        mov    dword ptr [esi + 0x13c], eax
        mov    byte ptr [esi + 0x136], al
        mov    eax, dword ptr [esi + 0x138]
        or     eax, eax
        jne    loc_3_00AD
        mov    edi, dword ptr [ebp + 8]
        mov    edi, dword ptr [edi]
        lea    edx, [5]                      ; -> obj4:0x0005
        mov    edx, dword ptr [edx]
        cmp    edx, edi
        jne    loc_3_009C
        mov    eax, 5
        jmp    loc_3_00AD
loc_3_009C:
        lea    edx, [0]                      ; -> obj4:0x0000
        mov    edx, dword ptr [edx]
        cmp    edx, edi
        jne    loc_3_010C
        mov    eax, 7
loc_3_00AD:
        mov    edi, 0x2518                   ; -> obj1:0x2518
        mov    word ptr [edi], ax
        mov    dword ptr [edi + 4], 0x1cc0   ; -> loc_1_1CC0
        mov    word ptr [edi + 2], 4
        mov    dword ptr [edi + 0x1c], esi
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_3_010C
        mov    dword ptr [esi + 0x13c], eax
        mov    byte ptr [esi + 0x136], 0x10
        mov    ebx, dword ptr [0x250c]       ; -> obj1:0x250C
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        and    cl, 4
        mov    byte ptr [esi + 0x137], cl
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        mov    ebx, dword ptr [0x250c]       ; -> obj1:0x250C
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   cl, 8
        je     loc_3_010C
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_3_010C:
        cmp    dword ptr [0x2510], 0         ; -> obj1:0x2510
        jne    loc_3_012D
        push   esi
        mov    esi, 0x1cd0                   ; -> loc_1_1CD0
        mov    eax, 0x64
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x2510], esi       ; -> obj1:0x2510
        pop    esi
loc_3_012D:
        inc    dword ptr [0x2514]            ; -> obj1:0x2514
        lea    edi, [esi + 0x4c]
        xor    eax, eax
        mov    ecx, 6
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [esi + 0x129], al
        mov    byte ptr [esi + 0x3b], al
        mov    dword ptr [esi + 0x120], eax
        mov    dword ptr [esi + 0xe8], eax
        dec    eax
        mov    dword ptr [esi + 0x11c], eax
        mov    dword ptr [esi + 0xa0], eax
        mov    dword ptr [esi + 0xa4], eax
        mov    dword ptr [esi + 0x9c], 1
        lea    edi, [esi + 0x30]
        mov    dword ptr [esi + 0xe4], edi
        lea    edi, [esi + 0x38]
        mov    dword ptr [esi + 0x12c], edi
        lea    edi, [esi + 0x88]
        mov    dword ptr [esi + 0x130], edi
        xor    eax, eax
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [esi + 0xfc], eax
        mov    dword ptr [esi + 0x104], eax
        mov    byte ptr [esi + 0x38], 0xb0
        mov    byte ptr [esi + 0x12a], 0
        mov    byte ptr [esi + 0x128], 0
        mov    bl, 0
        xor    ah, ah
        xor    ecx, ecx
        cmp    ecx, dword ptr [esi + 0x13c]
        call   sub_1_02E4
        jb     loc_3_02F4
        mov    dword ptr [esi + 0x16c], 0x1000
        mov    dword ptr [esi + 0x170], 0x7000
        mov    dword ptr [esi + 0x174], 0x1000
        xor    eax, eax
        mov    dword ptr [esi + 0x1a0], eax
        mov    dword ptr [esi + 0x1a4], eax
        mov    dword ptr [esi + 0x198], eax
        mov    dword ptr [esi + 0x19c], eax
        mov    dword ptr [esi + 0x1a8], eax
        mov    dword ptr [esi + 0x1ac], eax
        mov    dword ptr [esi + 0x1b0], eax
        mov    dword ptr [esi + 0x1b4], eax
        mov    dword ptr [esi + 0x1b8], eax
        mov    dword ptr [esi + 0x1bc], eax
        mov    dword ptr [esi + 0x1c0], eax
        mov    dword ptr [esi + 0x1c4], eax
        mov    dword ptr [esi + 0x1c8], eax
        mov    dword ptr [esi + 0x1cc], eax
        mov    dword ptr [esi + 0x1d0], eax
        mov    dword ptr [esi + 0x1d4], eax
        mov    dword ptr [esi + 0x1d8], eax
        mov    dword ptr [esi + 0x1dc], eax
        mov    dword ptr [esi + 0x1e0], eax
        mov    dword ptr [esi + 0x1e4], eax
        mov    dword ptr [esi + 0x1e8], eax
        mov    dword ptr [esi + 0x1ec], eax
        push   0x81
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   1
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        mov    dword ptr [esi + 0x10c], eax
        mov    dword ptr [esi + 0x114], edx
        mov    dword ptr [esi + 0x118], 0x1000
        push   0x81
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   1
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        xor    ebx, ebx
        mov    dword ptr [esi + 0x110], eax
        mov    dword ptr [esi + 0x3c], edx
        mov    dword ptr [esi + 0x40], 0x1000
        mov    dword ptr [esi + 0x4c], ebx
        mov    dword ptr [esi + 0x50], edx
        mov    dword ptr [esi + 0x54], edx
        xor    eax, eax
loc_3_02D9:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx], eax
        or     esi, esi
        je     loc_3_02F4
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x140], ebx
        mov    dword ptr [esi + 0xc], eax
        or     eax, eax
        mov    eax, esi
        je     loc_3_02F6
loc_3_02F4:
        xor    eax, eax
loc_3_02F6:
        push   eax
        call   sub_1_1DC8
        pop    eax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
loc_3_0304:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xc], 0
        lea    esi, [esi + 0x94]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, 0xc
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        or     al, 1
        pop    edi
        pop    esi
        pop    ecx
        leave  
        ret    
loc_3_032C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    dword ptr [0x2799], 0x43      ; -> obj1:0x2799
        mov    esi, dword ptr [ebp + 8]
        call   sub_3_0794
        mov    dword ptr [esi + 0xc], 0
        mov    edx, dword ptr [esi + 0xc8]
        inc    dx
        inc    dx
        mov    al, 0xc
        out    dx, al
        mov    eax, dword ptr [0x2510]       ; -> obj1:0x2510
        or     eax, eax
        je     loc_3_037C
        dec    dword ptr [0x2514]            ; -> obj1:0x2514
        jne    loc_3_037C
        push   esi
        mov    esi, eax
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        xor    eax, eax
        mov    dword ptr [0x2510], eax       ; -> obj1:0x2510
loc_3_037C:
        mov    eax, dword ptr [esi + 0x13c]
        or     eax, eax
        je     loc_3_03A3
        cmp    byte ptr [esi + 0x137], 0
        je     loc_3_0395
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
loc_3_0395:
        xor    eax, eax
        xchg   dword ptr [esi + 0x13c], eax
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_3_03A3:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x10c]
        or     eax, eax
        je     loc_3_03C4
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 0x10c], eax
loc_3_03C4:
        mov    eax, dword ptr [esi + 0x110]
        or     eax, eax
        je     loc_3_03E2
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 0x110], eax
loc_3_03E2:
        mov    ebx, esi
        call   sub_3_0730
        push   eax
        call   sub_1_1DC8
        pop    eax
        or     al, 1
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
loc_3_03F9:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        jecxz  loc_3_0418
        mov    eax, dword ptr [edx + 0x4c]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 0x58]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx], 0
loc_3_0418:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        dec    eax
        pop    edx
        pop    ecx
        leave  
        ret    
loc_3_0422:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x12c], ecx
        mov    al, byte ptr [edx + 0x38]
        mov    byte ptr [ecx], al
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        pop    edx
        pop    ecx
        leave  
        ret    
loc_3_0442:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    edx, edi
        mov    ecx, 0x40
        xor    eax, eax
        rep stosb byte ptr es:[edi], al
        mov    word ptr [edx], 0x40
        mov    word ptr [edx + 2], 1
        mov    dword ptr [edx + 4], 1
        mov    dword ptr [edx + 0x18], 2
        xor    eax, eax
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        pop    edi
        pop    edx
        pop    ecx
        leave  
        ret    
loc_3_047D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    edx, dword ptr [esi + 0x3c]
        mov    dword ptr [esi + 0x4c], ebx
        mov    dword ptr [esi + 0x50], edx
        mov    dword ptr [esi + 0x54], edx
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
loc_3_04A3:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xe0], eax
        mov    ecx, dword ptr [ebp + 0x10]
        jecxz  loc_3_04BF
        mov    dword ptr [edx + 0xe4], ecx
loc_3_04BF:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        pop    edx
        pop    ecx
        leave  
        ret    
loc_3_04C9:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx + 0x108], ecx
        mov    ecx, dword ptr [ebp + 0x10]
        inc    eax
        je     loc_3_04F8
        dec    eax
        test   byte ptr [edx + 0xf0], 2
        je     loc_3_04F8
        cmp    eax, dword ptr [edx + 0x48]
        jb     loc_3_04F8
        mov    eax, dword ptr [edx + 0x48]
        sub    eax, 0xa
loc_3_04F8:
        mov    dword ptr [edx + 0x104], ecx
        or     ecx, ecx
        jne    loc_3_0504
        xor    eax, eax
loc_3_0504:
        mov    dword ptr [edx + 0xdc], eax
        mov    dword ptr [edx + 0x120], eax
        or     byte ptr [edx + 0xe9], 2
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        pop    edx
        pop    ecx
        leave  
        ret    
loc_3_0521:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, ecx
        inc    eax
        je     loc_3_0541
        cmp    ecx, dword ptr [esi + 0x40]
        jb     loc_3_0541
        mov    ecx, dword ptr [esi + 0x40]
        sub    ecx, 0xa
loc_3_0541:
        mov    ebx, dword ptr [ebp + 0x10]
        or     ebx, ebx
        jne    loc_3_054B
        or     ecx, 0xffffffff
loc_3_054B:
        mov    dword ptr [esi + 0xfc], ebx
        mov    dword ptr [esi + 0x11c], ecx
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0x100], ecx
        cmp    dword ptr [esi + 0x4c], 0
        jne    loc_3_0570
        xor    eax, eax
        mov    edi, dword ptr [esi + 0x130]
        mov    dword ptr [edi], eax
loc_3_0570:
        or     al, 1
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave  
        ret    
loc_3_0578:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [edx + 0xf8], eax
        mov    dword ptr [edx + 0xf4], ecx
        and    byte ptr [edx + 0xe9], 0xbf
        jecxz  loc_3_05C5
        mov    ecx, dword ptr [edx + 0xe4]
        mov    eax, dword ptr [edx + 0xe0]
        cli    
        or     byte ptr [edx + 0xe9], 0x40
        and    eax, dword ptr [ecx]
        sti    
        or     eax, eax
        je     loc_3_05C5
        push   esi
        mov    esi, edx
        mov    eax, 4
        call   sub_1_022B
        pop    esi
loc_3_05C5:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        pop    esi
        pop    edx
        pop    ecx
        leave  
        ret    
loc_3_05D0:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], 0x30
        or     ecx, ecx
        jne    loc_3_0304
        mov    dword ptr [edx + 0xc], ecx
        pop    edx
        pop    ecx
        leave  
        ret    
loc_3_05F3:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0x10], 0x30
        jne    loc_3_0610
        mov    dword ptr [ebp + 0x10], 0xffffffff
        jmp    loc_1_0034
loc_3_0610:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], 0xffffffeb
        pop    edx
        pop    ecx
        leave  
        ret    
loc_3_061D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ecx + 0x34]
        mov    dword ptr [eax], ebx
        or     al, 1
        pop    ecx
        pop    ebx
        leave  
        ret    
        db  000h
loc_3_0634:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_3_063D
        ret    
loc_3_063D:
        push   esi
        push   edi
        push   ebx
        push   edx
        mov    esi, dword ptr [0x2508]       ; -> obj1:0x2508
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_3_0667
        mov    ecx, dword ptr [esp + 0x20]
loc_3_0653:
        cmp    dword ptr [eax + 0xc8], ecx
        je     loc_3_0724
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_3_0653
loc_3_0667:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_3_0724
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    edi, eax
        mov    ebx, eax
        xor    eax, eax
        mov    ecx, 0xa8
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [ebx], 0x10a0094
        mov    dword ptr [ebx + 4], 0x24ac   ; -> obj1:0x24AC
        mov    dword ptr [ebx + 8], 0x17
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [ebx + 0xc8], ecx
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ebx + 0x138], ecx
        mov    edi, dword ptr [esp + 0x28]
        mov    esi, edi
        xor    eax, eax
        xor    ecx, ecx
        dec    ecx
        repne scasb al, byte ptr es:[edi]
        neg    ecx
        dec    ecx
        push   ecx
        push   0
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    edi, eax
        pop    ecx
        or     eax, eax
        je     loc_3_0729
        mov    dword ptr [ebx + 0x144], eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   eax
        push   0                             ; -> loc_3_0000
        push   dword ptr [esp + 0x24]
        VxDCall  BIOS.Service_0019h              ; cd2019002b00
        add    esp, 0xc
        push   dword ptr [esp + 0x28]
        VxDCall  BIOS.Service_0021h              ; cd2021002b00
        add    esp, 4
        mov    dword ptr [ebx + 0xd0], eax
        test   eax, eax
        je     loc_3_0717
        push   dword ptr [esp + 0x28]
        VxDCall  BIOS.Service_0022h              ; cd2022002b00
        add    esp, 4
        mov    dword ptr [ebx + 0xd8], eax
loc_3_0717:
        mov    esi, ebx
        mov    edx, dword ptr [esi + 0xc8]
        call   sub_1_2008
loc_3_0724:
        pop    edx
        pop    ebx
        pop    edi
        pop    esi
        ret    
loc_3_0729:
        call   sub_3_0730
        jmp    loc_3_0724
sub_3_0730:
        push   esi
        cmp    dword ptr [ebx + 0x144], 0
        je     loc_3_074B
        push   0
        push   dword ptr [ebx + 0x144]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_3_074B:
        mov    esi, dword ptr [0x2508]       ; -> obj1:0x2508
        mov    eax, ebx
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        pop    esi
        ret    
        db  000h, 000h, 000h
sub_3_0764:
        push   edi
        mov    edi, dword ptr [esi + 0xd0]
        test   edi, edi
        je     loc_3_0792
        mov    eax, dword ptr [esi + 0xd8]
        test   eax, eax
        je     loc_3_0792
        push   1
        push   esi
        push   0x228                         ; -> loc_1_0228
        push   eax
        push   1
        call   edi
        add    esp, 0x14
        mov    dword ptr [esi + 0xd4], eax
        cmp    eax, 1
loc_3_0792:
        pop    edi
        ret    
sub_3_0794:
        mov    ecx, dword ptr [esi + 0xd0]
        jecxz  loc_3_07AE
        push   0x228                         ; -> loc_1_0228
        push   dword ptr [esi + 0xd4]
        push   3
        call   ecx
        add    esp, 0xc
loc_3_07AE:
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0xc  RW-
        db  04Ch, 050h, 054h, 031h, 000h, 04Ch, 050h, 054h, 032h, 025h, 050h, 041h
VxD__LDATA ENDS

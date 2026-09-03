; IRMATRAC -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 8772 instructions recovered by recursive descent

Declare_Virtual_Device IRMATRAC, 3, 10, IRMATRAC_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 054004000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x5dbd  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23f]
        mov    eax, 1
        out    dx, ax
        mov    word ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xaf]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x225]
        and    eax, 0xffff
        add    eax, 0xe
        mov    word ptr [ebp - 0x18], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbf]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xbb]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_0084
loc_1_005F:
        inc    word ptr [ebp - 0x10]
        add    dword ptr [ebp - 0xc], 0x1b
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        add    eax, 0x12
        mov    word ptr [ebp - 0x18], ax
        add    dword ptr [ebp - 4], 0x100
        add    dword ptr [ebp - 8], 0x100
loc_1_0084:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xa7]
        and    eax, 0xffff
        dec    eax
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        cmp    eax, ecx
        jl     loc_1_01AC
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    ax, word ptr [ebp - 0x18]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0101
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    dx, word ptr [eax + 0x233]
        mov    eax, ecx
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 4]
        shr    ebx, 0x10
        mov    dx, word ptr [ecx + 0x233]
        mov    ax, bx
        out    dx, ax
        jmp    loc_1_0126
loc_1_0101:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ebp - 8]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        shr    ecx, 0x10
        mov    dx, word ptr [eax + 0x233]
        mov    ax, cx
        out    dx, ax
loc_1_0126:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xa7]
        and    eax, 0xffff
        dec    eax
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_0165
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x1b
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], eax
        jmp    loc_1_0173
loc_1_0165:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xaf]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], eax
loc_1_0173:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x225]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        add    ecx, ecx
        lea    ecx, [ecx + ecx*8]
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 0xc]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 8], eax
        jmp    loc_1_005F
loc_1_01AC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xaf]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1ef], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xaf]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1f3], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1eb], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x212]
        and    ecx, 0xffff
        add    ecx, 4
        mov    dx, word ptr [eax + 0x23d]
        mov    ax, cx
        out    dx, ax
        mov    word ptr [ebp - 0x10], 0
        jmp    loc_1_020D
loc_1_0209:
        inc    word ptr [ebp - 0x10]
loc_1_020D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xab]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_0336
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ecx + 0x247]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_02AF
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x20e]
        mov    ecx, dword ptr [edx + ecx*4]
        shr    ecx, 0x10
        mov    dx, word ptr [eax + 0x233]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0x20e]
        mov    cx, word ptr [edx + ecx*4]
        and    ecx, 0xffff
        mov    dx, word ptr [eax + 0x233]
        mov    eax, ecx
        out    dx, ax
        jmp    loc_1_0300
loc_1_02AF:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        shl    ecx, 2
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edx + 0x206]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ecx + ebx]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        and    ecx, 0xffff
        shl    ecx, 2
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edx + 0x206]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ecx + ebx + 2]
        out    dx, ax
loc_1_0300:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    eax, 0x200
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x233]
        in     ax, dx
        mov    word ptr [ebp - 0x14], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x233]
        in     ax, dx
        mov    word ptr [ebp - 0x14], ax
        jmp    loc_1_0209
loc_1_0336:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_033D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   cx
        push   ax
        call   sub_1_04C8
        and    al, 4
        pop    cx
        and    cl, 1
        or     al, cl
        call   sub_1_0536
        or     al, 2
        call   sub_1_0536
        xchg   ah, al
        in     al, dx
        nop    
        nop    
        nop    
        nop    
        xchg   ah, al
        call   sub_1_0536
        and    al, 5
        call   sub_1_0536
        xchg   ah, al
        and    al, 1
        pop    cx
        jmp    loc_1_0381
loc_1_0381:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0386:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        lea    esi, [0]                      ; -> obj1:0x5DB8
        mov    byte ptr [esi + 1], 0x40
        mov    byte ptr [esi], 0x40
        mov    word ptr [esi + 2], 0x50
        mov    word ptr [esi + 4], 0x28
        jmp    loc_1_03AA
loc_1_03AA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_03AF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        or     al, 4
        call   sub_1_0536
        xor    ebx, ebx
        mov    cx, 8
loc_1_03CF:
        shl    bl, 1
        call   sub_1_033D
        or     bl, al
        loop   loc_1_03CF
        mov    al, bl
        pop    ebx
        jmp    loc_1_03E3
loc_1_03E3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_03E8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ax
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        or     al, 0
        call   sub_1_0536
        pop    ax
        mov    ecx, 8
loc_1_040A:
        rol    al, 1
        push   ax
        call   sub_1_033D
        pop    ax
        loop   loc_1_040A
        jmp    loc_1_041D
loc_1_041D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0422:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        mov    al, 1
        call   sub_1_0536
        or     al, 2
        call   sub_1_0536
        mov    al, 2
        call   sub_1_0536
        mov    al, 0
        call   sub_1_0536
        jmp    loc_1_0455
loc_1_0455:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_045A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        mov    al, 0
        call   sub_1_0536
        or     al, 2
        call   sub_1_0536
        or     al, 1
        call   sub_1_0536
        mov    al, 1
        call   sub_1_0536
        jmp    loc_1_048D
loc_1_048D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0492:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        mov    al, 1
        call   sub_1_0536
        or     al, 2
        call   sub_1_0536
        call   sub_1_0536
        mov    al, 1
        call   sub_1_0536
        jmp    loc_1_04C3
loc_1_04C3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_04C8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        in     al, dx
        nop    
        nop    
        nop    
        nop    
        mov    ah, al
        and    ah, 6
        mov    ecx, 3
        sar    al, cl
        and    al, 1
        or     al, ah
        jmp    loc_1_04E8
loc_1_04E8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_04ED:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        or     al, 4
        call   sub_1_0536
        call   sub_1_033D
        and    al, 1
        cmp    al, 0
        je     loc_1_0519
        or     byte ptr [esi + 6], 1
loc_1_0519:
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        and    al, 1
        call   sub_1_0536
        jmp    loc_1_0531
loc_1_0531:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0536:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        out    dx, al
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        call   sub_1_0552
        jmp    loc_1_054D
loc_1_054D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0552:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_0558:
        loop   loc_1_0558
        jmp    loc_1_055F
loc_1_055F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0564:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x74
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x214], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x19]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_0655
        mov    dword ptr [ebp - 0x20], 0xe
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        test   al, 8
        jne    loc_1_05B6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x19], 0xe
        jmp    loc_1_0655
loc_1_05B6:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22f]
        mov    word ptr [ebp - 0x64], ax
        call   sub_1_0386
        lea    esi, [0]                      ; -> obj1:0x5DB8
        lea    edi, [ebp - 0x60]
        movzx  edx, word ptr [ebp - 0x64]
        add    edx, 0x1c
loc_1_05D9:
        call   sub_1_0422
        movzx  eax, byte ptr [esi + 1]
        shl    ax, 1
        sub    al, byte ptr [esi]
        rol    al, 1
        or     al, 1
        call   sub_1_03E8
        call   sub_1_04ED
        call   sub_1_03AF
        stosb  byte ptr es:[edi], al
        call   sub_1_0492
        call   sub_1_045A
        dec    byte ptr [esi]
        jne    loc_1_05D9
        lea    esi, [ebp - 0x60]
        cmp    word ptr [esi], 0x55aa
        jne    loc_1_064B
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 2]
        sub    cl, 2
        lea    esi, [esi + 3]
loc_1_0628:
        movzx  eax, word ptr [esi]
        cmp    al, 1
        je     loc_1_0643
        and    al, 0xf
        add    al, 1
        sub    cl, al
        jbe    loc_1_064B
        add    esi, eax
        jmp    loc_1_0628
loc_1_0643:
        xor    eax, eax
        inc    esi
        mov    al, byte ptr [esi]
        mov    dword ptr [ebp - 0x20], eax
loc_1_064B:
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x19], ax
loc_1_0655:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x237]
        mov    eax, 0xffff
        out    dx, ax
        push   0x4e20
        call   sub_1_58D0
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x239]
        in     al, dx
        mov    byte ptr [ebp - 0x1c], al
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x1c]
        or     cl, 8
        mov    dx, word ptr [eax + 0x239]
        mov    al, cl
        out    dx, al
        mov    word ptr [ebp - 4], 0x2710
loc_1_0699:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_06FC
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x237]
        in     al, dx
        mov    byte ptr [ebp - 0x1c], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1c]
        cmp    eax, 0x40
        jne    loc_1_06CA
        jmp    loc_1_06FC
loc_1_06CA:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1c]
        test   al, 0x10
        jne    loc_1_06E4
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1c]
        test   al, 0x20
        jne    loc_1_06E9
loc_1_06E4:
        jmp    loc_1_0CF5
loc_1_06E9:
        push   0x2710
        call   sub_1_58D0
        dec    word ptr [ebp - 4]
        jmp    loc_1_0699
loc_1_06FC:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_0711
        jmp    loc_1_0CF5
loc_1_0711:
        push   0x3e8
        call   sub_1_58D0
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23f]
        mov    eax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa00
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    eax, 0x8000
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        xor    eax, eax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        xor    eax, eax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        xor    eax, eax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        xor    eax, eax
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 0x19]
        and    edx, 0xffff
        shl    edx, 8
        mov    ebx, edx
        and    ebx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 0x19]
        and    edx, 0xffff
        or     ebx, edx
        mov    dx, word ptr [ecx + 0x233]
        mov    ax, bx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    eax, 0x505
        out    dx, ax
        mov    byte ptr [ebp - 0x18], 0
        jmp    loc_1_07D9
loc_1_07D6:
        inc    byte ptr [ebp - 0x18]
loc_1_07D9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        cmp    eax, 0xa
        jge    loc_1_07FA
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        xor    eax, eax
        out    dx, ax
        jmp    loc_1_07D6
loc_1_07FA:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    eax, 0x400
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    eax, 0x20
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x233]
        mov    ax, word ptr [ebx + 0xa7]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ecx + 0xab]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x237]
        mov    eax, 0x9080
        out    dx, ax
        push   0x4e20
        call   sub_1_58D0
        mov    word ptr [ebp - 4], 0x4e20
loc_1_0869:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_08BF
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x237]
        in     al, dx
        mov    byte ptr [ebp - 0x1c], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1c]
        test   al, 0x70
        jne    loc_1_0899
        jmp    loc_1_08BF
loc_1_0899:
        mov    al, byte ptr [ebp - 0x1c]
        and    eax, 0x70
        cmp    al, 0x10
        jne    loc_1_08AC
        jmp    loc_1_0CF5
loc_1_08AC:
        push   0x2710
        call   sub_1_58D0
        dec    word ptr [ebp - 4]
        jmp    loc_1_0869
loc_1_08BF:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_08D4
        jmp    loc_1_0CF5
loc_1_08D4:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23f]
        mov    eax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa30
        out    dx, ax
        push   0x4e20
        call   sub_1_58D0
        mov    word ptr [ebp - 4], 0xfa0
loc_1_0906:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_0965
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x231]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   ah, 0x40
        je     loc_1_093C
        jmp    loc_1_0965
loc_1_093C:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   ah, 0x20
        je     loc_1_0952
        jmp    loc_1_0CF5
loc_1_0952:
        push   0x3e8
        call   sub_1_58D0
        dec    word ptr [ebp - 4]
        jmp    loc_1_0906
loc_1_0965:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_097A
        jmp    loc_1_0CF5
loc_1_097A:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffdf
        mov    dx, word ptr [eax + 0x231]
        mov    ax, cx
        out    dx, ax
        mov    word ptr [ebp - 4], 0x1770
loc_1_0998:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_09E1
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x231]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   ah, 0x20
        jne    loc_1_09CE
        jmp    loc_1_09E1
loc_1_09CE:
        push   0x3e8
        call   sub_1_58D0
        dec    word ptr [ebp - 4]
        jmp    loc_1_0998
loc_1_09E1:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_09F6
        jmp    loc_1_0CF5
loc_1_09F6:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23f]
        mov    eax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa02
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x231]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    ax, word ptr [ebp - 0xc]
        out    dx, ax
        mov    byte ptr [ebp - 0x18], 0
        jmp    loc_1_0A44
loc_1_0A41:
        inc    byte ptr [ebp - 0x18]
loc_1_0A44:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18]
        cmp    eax, 3
        jge    loc_1_0A6D
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        in     ax, dx
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x18]
        mov    word ptr [ebp + ecx*2 - 0x14], ax
        jmp    loc_1_0A41
loc_1_0A6D:
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        cmp    eax, 3
        jne    loc_1_0A8D
        xor    eax, eax
        mov    ax, word ptr [ebp - 0x12]
        cmp    eax, 3
        je     loc_1_0A92
loc_1_0A8D:
        jmp    loc_1_0CF5
loc_1_0A92:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0AE6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x21d]
        shr    eax, 0x10
        and    eax, 0xffff
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x21d]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
        jmp    loc_1_0B1D
loc_1_0AE6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x221]
        and    eax, 0xff0000
        shr    eax, 0x10
        and    ax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x221]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
loc_1_0B1D:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x215], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x21b], 0x8000
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0B52
        jmp    loc_1_0CF5
loc_1_0B52:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3f
        mov    dword ptr [ebp - 0x68], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x21d]
        mov    dword ptr [ebp - 0x6c], eax
        mov    eax, dword ptr [ebp - 0x6c]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x68]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x6c]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x68]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x6c]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x68]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x6c]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x68]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x6c]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x68]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x6c]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x68]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x3f]
        test   eax, eax
        je     loc_1_0BD1
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x3f]
        cmp    eax, 0x10
        je     loc_1_0BD1
        jmp    loc_1_0CF5
loc_1_0BD1:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x45]
        test   eax, eax
        jne    loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x46]
        test   eax, eax
        jne    loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x47]
        test   eax, eax
        jne    loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x48]
        test   eax, eax
        jne    loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x49]
        test   eax, eax
        jne    loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 0x4a]
        test   eax, eax
        jne    loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x45
        mov    dword ptr [ebp - 0x70], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x3f
        mov    dword ptr [ebp - 0x74], eax
        mov    eax, dword ptr [ebp - 0x74]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x70]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x74]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x70]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x74]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x70]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x74]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x70]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x74]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x70]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x74]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x70]
        mov    byte ptr [ecx + 5], al
loc_1_0C83:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23f]
        mov    eax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa32
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x231]
        in     ax, dx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x225], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa34
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x231]
        in     ax, dx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x212], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x2f], 0
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_0D19
loc_1_0CF5:
        push   7
        push   0
        push   2
        push   0xaa000008
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
        add    esp, 0x14
        mov    dword ptr [ebp - 8], 0xc0000001
loc_1_0D19:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_0D21
loc_1_0D21:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_0D28:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0xc], 0x3b
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x3e]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0D6B
        mov    eax, 0xc0000001
        jmp    loc_1_0F87
loc_1_0D6B:
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_1_0D7A
loc_1_0D77:
        inc    dword ptr [ebp - 0x10]
loc_1_0D7A:
        mov    eax, dword ptr [ebp + 0x18]
        cmp    dword ptr [ebp - 0x10], eax
        jae    loc_1_0DA0
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    dword ptr [ecx + eax*4], 1
        jne    loc_1_0D9B
        jmp    loc_1_0DA0
loc_1_0D9B:
        jmp    loc_1_0D77
loc_1_0DA0:
        mov    eax, dword ptr [ebp + 0x18]
        cmp    dword ptr [ebp - 0x10], eax
        jne    loc_1_0DB6
        mov    eax, 0xc0010019
        jmp    loc_1_0F87
loc_1_0DB6:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x18], 0xffffffff
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        push   0
        push   0x96
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_58D6
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_0E13
        push   5
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
        add    esp, 0x14
        mov    eax, 0xc000009a
        jmp    loc_1_0F87
loc_1_0E13:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1d], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xc
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x85]
        push   eax
        call   sub_2_0A10
loc_1_0E7A:
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0EBE
        push   8
        push   1
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
        add    esp, 0x14
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_58DC
        mov    eax, 0xc0000001
        jmp    loc_1_0F87
loc_1_0EBE:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0x100
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x264], 0x100
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x61
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x65]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x65]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x65], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x30]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_0F7F
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1c], 1
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x31]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0F7F
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x39], 3
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x22d], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2A07
        test   eax, eax
        je     loc_1_0F70
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 0x14], 0xc0000001
        mov    eax, dword ptr [ebp - 0x14]
        jmp    loc_1_0F87
loc_1_0F70:
        mov    dword ptr [ebp - 0x14], 0x103
        mov    eax, dword ptr [ebp - 0x14]
        jmp    loc_1_0F87
loc_1_0F7F:
        mov    eax, dword ptr [ebp - 0x14]
        jmp    loc_1_0F87
loc_1_0F87:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x24
sub_1_0F8E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x61]
        mov    dword ptr [ebp - 4], eax
loc_1_0FA7:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x61
        cmp    eax, dword ptr [ebp - 4]
        je     loc_1_0FE1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x18], ecx
        jbe    loc_1_0FD4
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], eax
loc_1_0FD4:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0FA7
loc_1_0FE1:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0FF2
        mov    dword ptr [ebp - 8], 0x100
loc_1_0FF2:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x264], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_1005:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x5d]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_1048
        mov    eax, 0xc0010002
        jmp    loc_1_12D0
loc_1_1048:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0x14]
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x85]
        push   eax
        call   sub_2_0AF0
loc_1_1066:
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_115B
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x14], 2
        jne    loc_1_10F5
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x264]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_1_10E0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0F8E
loc_1_10E0:
        push   0
        push   0x96
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_58DC
        jmp    loc_1_1156
loc_1_10F5:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x69
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x6d]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x6d]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x6d], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 0x14], 2
        mov    dword ptr [ebp - 0xc], 0x103
loc_1_1156:
        jmp    loc_1_12C8
loc_1_115B:
        cmp    dword ptr [ebp - 0xc], 0x103
        jne    loc_1_11C7
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 0x14], 2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x264]
        cmp    dword ptr [eax + 0x18], ecx
        jne    loc_1_11B5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0F8E
loc_1_11B5:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x5d]
        xor    eax, eax
        jmp    loc_1_12D0
        db  0E9h, 001h, 001h, 000h, 000h
loc_1_11C7:
        cmp    dword ptr [ebp - 0xc], 0xc001000e
        jne    loc_1_1239
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    dword ptr [ebp - 0xc], 0x103
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x20]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x69
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x6d]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x6d]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x6d], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_12C8
loc_1_1239:
        cmp    dword ptr [ebp - 0xc], 0xc001000d
        jne    loc_1_12AB
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x1d], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x71
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x75]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x75]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x75], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 0xc], 0x103
        jmp    loc_1_12C8
loc_1_12AB:
        push   1
        push   0xa
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
        add    esp, 0x14
loc_1_12C8:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_12D0
loc_1_12D0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_12D7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_57FC
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_57F0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_1304:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1e8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x48], 0
        mov    word ptr [ebp - 0x19c], 6
        mov    word ptr [ebp - 0x19a], 6
        mov    dword ptr [ebp - 0x198], 0    ; -> obj1:0x5A94
        mov    word ptr [ebp - 0x20], 8
        mov    word ptr [ebp - 0x1e], 8
        mov    dword ptr [ebp - 0x1c], 0     ; -> obj1:0x5AA4
        mov    word ptr [ebp - 0x44], 0xa
        mov    word ptr [ebp - 0x42], 0xa
        mov    dword ptr [ebp - 0x40], 0     ; -> obj1:0x5AB8
        mov    word ptr [ebp - 0x1d0], 0xa
        mov    word ptr [ebp - 0x1ce], 0xa
        mov    dword ptr [ebp - 0x1cc], 0    ; -> obj1:0x5AD0
        mov    word ptr [ebp - 0xc], 0xb
        mov    word ptr [ebp - 0xa], 0xb
        mov    dword ptr [ebp - 8], 0        ; -> loc_1_5AE8
        mov    word ptr [ebp - 0x1bc], 9
        mov    word ptr [ebp - 0x1ba], 9
        mov    dword ptr [ebp - 0x1b8], 0    ; -> loc_1_5B00
        mov    word ptr [ebp - 0x38], 0xd
        mov    word ptr [ebp - 0x36], 0xd
        mov    dword ptr [ebp - 0x34], 0     ; -> loc_1_5B1C
        mov    word ptr [ebp - 0x1ac], 0xa
        mov    word ptr [ebp - 0x1aa], 0xa
        mov    dword ptr [ebp - 0x1a8], 0    ; -> obj1:0x5B38
        mov    word ptr [ebp - 0x30], 0xd
        mov    word ptr [ebp - 0x2e], 0xd
        mov    dword ptr [ebp - 0x2c], 0     ; -> loc_1_5B54
        mov    word ptr [ebp - 0x1b4], 0x12
        mov    word ptr [ebp - 0x1b2], 0x12
        mov    dword ptr [ebp - 0x1b0], 0    ; -> obj1:0x5B78
        mov    word ptr [ebp - 0x1a4], 0x12
        mov    word ptr [ebp - 0x1a2], 0x12
        mov    dword ptr [ebp - 0x1a0], 0    ; -> obj1:0x5BA0
        mov    word ptr [ebp - 0x194], 0x11
        mov    word ptr [ebp - 0x192], 0x11
        mov    dword ptr [ebp - 0x190], 0    ; -> loc_1_5BC8
        mov    word ptr [ebp - 0x18], 0xb
        mov    word ptr [ebp - 0x16], 0xb
        mov    dword ptr [ebp - 0x14], 0     ; -> loc_1_5BE8
        mov    dword ptr [ebp - 0x3c], 0
        mov    byte ptr [ebp - 0x18c], 0
        mov    byte ptr [ebp - 0x1c8], 2
        mov    byte ptr [ebp - 0x1c7], 3
        mov    byte ptr [ebp - 0x1c6], 0xa
        mov    byte ptr [ebp - 0x1c5], 0xb
        mov    byte ptr [ebp - 0x1c4], 4
        mov    byte ptr [ebp - 0x1c3], 6
        mov    byte ptr [ebp - 0x1c2], 5
        mov    byte ptr [ebp - 0x1c1], 7
        jmp    loc_1_1498
loc_1_1498:
        jmp    loc_1_149D
loc_1_149D:
        mov    dword ptr [ebp - 0x1dc], 0xffffffff
        mov    eax, dword ptr [ebp - 0x1dc]
        push   eax
        push   0
        push   0x268
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_1_58D6
        mov    dword ptr [ebp - 0x48], eax
        cmp    dword ptr [ebp - 0x48], 0
        je     loc_1_14D5
        mov    eax, 0xc000009a
        jmp    loc_1_1E5D
loc_1_14D5:
        mov    edi, dword ptr [ebp - 0x10]
        xor    eax, eax
        mov    ecx, 0x9a
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x7d], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0xa7], 0x40
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0xab], 0x20
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x245], 0x1194
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x247], 0x1194
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x39], 2
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_5954
        cmp    dword ptr [ebp - 0x48], 0
        je     loc_1_1560
        push   0
        push   0x268
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_58DC
        mov    eax, 0xc0000001
        jmp    loc_1_1E5D
loc_1_1560:
        push   1
        lea    eax, [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_15D7
        mov    eax, dword ptr [ebp - 0x28]
        cmp    dword ptr [eax + 4], 3
        jne    loc_1_15B1
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        or     al, 0x10
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
        mov    byte ptr [ebp - 0x18c], 1
loc_1_15B1:
        mov    eax, dword ptr [ebp - 0x28]
        cmp    dword ptr [eax + 4], 2
        jne    loc_1_15D7
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        or     al, 0x30
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
loc_1_15D7:
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_585C
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_16FE
        cmp    dword ptr [ebp - 4], 6
        jne    loc_1_16FE
        mov    eax, dword ptr [ebp - 0x24]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0x24]
        mov    cl, byte ptr [ecx + 3]
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x24]
        mov    cl, byte ptr [ecx + 4]
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x24]
        mov    cl, byte ptr [ecx + 5]
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x24]
        mov    cl, byte ptr [ecx]
        and    ecx, 0xff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x24]
        mov    cl, byte ptr [ecx + 1]
        and    ecx, 0xff
        or     eax, ecx
        je     loc_1_16FE
        mov    eax, dword ptr [ebp - 0x24]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        cmp    eax, 0x4f
        jg     loc_1_16FE
        mov    eax, dword ptr [ebp - 0x24]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        cmp    eax, 0x40
        jl     loc_1_16FE
        mov    eax, dword ptr [ebp - 0x10]
        add    eax, 0x45
        mov    dword ptr [ebp - 0x1e0], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    dword ptr [ebp - 0x1e4], eax
        mov    eax, dword ptr [ebp - 0x1e4]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x1e0]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x1e4]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x1e0]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x1e4]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x1e0]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x1e4]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x1e0]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x1e4]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x1e0]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x1e4]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x1e0]
        mov    byte ptr [ecx + 5], al
loc_1_16FE:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x18c]
        cmp    eax, 1
        jne    loc_1_1A0A
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1741
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x3c], eax
loc_1_1741:
        lea    eax, [ebp - 0x1d8]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_5850
        cmp    dword ptr [ebp - 0x48], 0
        je     loc_1_177B
        push   0
        push   0x268
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_58DC
        mov    eax, dword ptr [ebp - 0x48]
        jmp    loc_1_1E5D
loc_1_177B:
        mov    eax, dword ptr [ebp - 0x1d8]
        and    eax, 0xffff
        cmp    eax, 0x5c1c
        je     loc_1_17C1
        mov    eax, dword ptr [ebp - 0x1d8]
        and    eax, 0xffff
        cmp    eax, 0x5c1d
        je     loc_1_17C1
        push   0
        push   0x268
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_58DC
        mov    eax, 0xc0000001
        jmp    loc_1_1E5D
loc_1_17C1:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1d6]
        test   al, 2
        jne    loc_1_17EA
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        or     al, 1
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
loc_1_17EA:
        mov    al, byte ptr [ebp - 0x1d3]
        and    eax, 7
        mov    dword ptr [ebp - 0x1e8], eax
        jmp    loc_1_188B
loc_1_17FE:
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0xa00
        jmp    loc_1_18C5
loc_1_180F:
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0xa20
        jmp    loc_1_18C5
loc_1_1820:
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0xa40
        jmp    loc_1_18C5
loc_1_1831:
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0xa60
        jmp    loc_1_18C5
loc_1_1842:
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0x1a00
        jmp    loc_1_18C5
loc_1_1853:
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0x1a20
        jmp    loc_1_18C5
loc_1_1864:
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0x1a40
        jmp    loc_1_18C5
loc_1_1875:
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0x1a60
        jmp    loc_1_18C5
        db  0E9h, 03Ah, 000h, 000h, 000h
loc_1_188B:
        cmp    dword ptr [ebp - 0x1e8], 7
        ja     loc_1_18C5
        mov    eax, dword ptr [ebp - 0x1e8]
        jmp    dword ptr [eax*4]             ; -> obj1:0x18A5
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_17FE
        dd  loc_1_180F
        dd  loc_1_1820
        dd  loc_1_1831
        dd  loc_1_1842
        dd  loc_1_1853
        dd  loc_1_1864
        dd  loc_1_1875
loc_1_18C5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1d5]
        test   al, 0x10
        jne    loc_1_196C
        mov    al, byte ptr [ebp - 0x1d5]
        and    eax, 7
        mov    al, byte ptr [ebp + eax - 0x1c8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x18], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1d6]
        test   al, 0x40
        je     loc_1_1934
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1d4]
        sar    eax, 4
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x243], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_192F
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x243], 0x10
loc_1_192F:
        jmp    loc_1_193E
loc_1_1934:
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x243], 0
loc_1_193E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1d6]
        test   al, 4
        je     loc_1_1967
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        or     al, 4
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
loc_1_1967:
        jmp    loc_1_1A05
loc_1_196C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1d3]
        test   al, 1
        je     loc_1_198D
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0x1a20
        jmp    loc_1_1999
loc_1_198D:
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x22f], 0xa20
loc_1_1999:
        mov    al, byte ptr [ebp - 0x1d5]
        and    eax, 3
        mov    al, byte ptr [ebp + eax - 0x1c8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x18], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1d6]
        test   al, 0x40
        je     loc_1_19FB
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x1d4]
        sar    eax, 6
        add    eax, 4
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x243], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_19F6
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x243], 0x10
loc_1_19F6:
        jmp    loc_1_1A05
loc_1_19FB:
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x243], 0
loc_1_1A05:
        jmp    loc_1_1C73
loc_1_1A0A:
        lea    eax, [ebp - 0x188]
        push   eax
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_5856
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1A9E
        cmp    dword ptr [ebp - 0x188], 0x10026110
        je     loc_1_1A46
        mov    eax, 0xc0000001
        jmp    loc_1_1E5D
loc_1_1A46:
        mov    al, byte ptr [ebp - 0xd6]
        and    al, 0xf
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x18], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x243], 0x10
        mov    ax, word ptr [ebp - 0xbf]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0x22f], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x15d]
        cmp    eax, 0x31
        jne    loc_1_1A99
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        or     al, 1
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
loc_1_1A99:
        jmp    loc_1_1C73
loc_1_1A9E:
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xcf
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
        push   1
        lea    eax, [ebp - 0x44]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1AEE
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0x22f], ax
        jmp    loc_1_1AF8
loc_1_1AEE:
        mov    eax, 0xc0000001
        jmp    loc_1_1E5D
loc_1_1AF8:
        push   0
        lea    eax, [ebp - 0x1d0]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1B2B
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x18], al
loc_1_1B2B:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1B63
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x243], al
        jmp    loc_1_1B6D
loc_1_1B63:
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x243], 0
loc_1_1B6D:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1BAF
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_1BAF
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x243], 0
loc_1_1BAF:
        push   0
        lea    eax, [ebp - 0x1bc]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1C03
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 0x10
        jne    loc_1_1C03
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        or     al, 1
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
loc_1_1C03:
        push   0
        lea    eax, [ebp - 0x1ac]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1C73
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_1C5C
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        or     al, 4
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
        jmp    loc_1_1C73
loc_1_1C5C:
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xfb
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
loc_1_1C73:
        push   0
        lea    eax, [ebp - 0x38]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1CE0
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_1CC9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        or     al, 2
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
        jmp    loc_1_1CE0
loc_1_1CC9:
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xfd
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x244], al
loc_1_1CE0:
        push   0
        lea    eax, [ebp - 0x19c]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1D14
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0x19], ax
loc_1_1D14:
        push   0
        lea    eax, [ebp - 0x1b4]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1D50
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0xab], ax
loc_1_1D50:
        push   0
        lea    eax, [ebp - 0x1a4]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1D8C
        mov    eax, dword ptr [ebp - 0x28]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0xa7], ax
loc_1_1D8C:
        push   0
        lea    eax, [ebp - 0x194]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        lea    eax, [ebp - 0x28]
        push   eax
        lea    eax, [ebp - 0x48]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 0x48], 0
        jne    loc_1_1E1B
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0x247], eax
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0x245], ax
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        test   al, 1
        jne    loc_1_1E1B
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax + 0x245]
        and    eax, 0xffff
        cmp    eax, 0x1194
        jle    loc_1_1E1B
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 0x245], 0x1194
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax + 0x247], 0x1194
loc_1_1E1B:
        mov    eax, dword ptr [ebp - 0x1c0]
        push   eax
        call   sub_1_5960
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_2_012B
loc_1_1E38:
        mov    dword ptr [ebp - 0x48], eax
        cmp    dword ptr [ebp - 0x48], 0
        je     loc_1_1E55
        push   0
        push   0x268
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_58DC
loc_1_1E55:
        mov    eax, dword ptr [ebp - 0x48]
        jmp    loc_1_1E5D
loc_1_1E5D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_1_1E64:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x1f
        push   eax
        call   sub_1_58B8
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x3e], 1
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x21b], 0x200
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x221]
        and    eax, 0xff0000
        shr    eax, 0x10
        and    ax, 0xff
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x221]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x219], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x215], 4
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2C60
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_1FA5
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x61]
        mov    dword ptr [ebp - 0xc], eax
loc_1_1EFA:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x61
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_1_1F49
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        inc    dword ptr [eax + 0x14]
        push   0
        push   0
        push   0x40010007
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_583E
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_5844
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_1EFA
loc_1_1F49:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1f7], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x1fb], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x31], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x30], 1
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x3d], 1
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x22b], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x227]
        dec    dword ptr [eax + 0x14]
        push   0xc0000001
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x227]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_5826
loc_1_1FA5:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_58C4
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x85]
        push   eax
        call   sub_2_09F0
loc_1_1FBD:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_0580
loc_1_1FC6:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_5808
        push   0
        push   0x268
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_58DC
        jmp    loc_1_1FEA
loc_1_1FEA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_1FF1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 0x5d]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x3d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_208D
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_2081
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2315
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_20FA
        test   eax, eax
        je     loc_1_206F
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x5d]
        mov    eax, 0xc0000001
        jmp    loc_1_20F3
loc_1_206F:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x5d]
        mov    dword ptr [ebp - 8], 0x103
        jmp    loc_1_2088
loc_1_2081:
        mov    dword ptr [ebp - 8], 0xc0010002
loc_1_2088:
        jmp    loc_1_20EB
loc_1_208D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x22b]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_20AF
        mov    dword ptr [ebp - 8], 0xc001000d
        jmp    loc_1_20EB
loc_1_20AF:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x30]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_20CE
        mov    dword ptr [ebp - 8], 0xc0000001
        jmp    loc_1_20EB
loc_1_20CE:
        push   4
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
        add    esp, 0x14
loc_1_20EB:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_20F3
loc_1_20F3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_20FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x30], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0564
        test   eax, eax
        je     loc_1_21BF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x61]
        mov    dword ptr [ebp - 0x10], eax
loc_1_2124:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x61
        cmp    eax, dword ptr [ebp - 0x10]
        je     loc_1_2173
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        inc    dword ptr [eax + 0x14]
        push   0
        push   0
        push   0x40010007
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_583E
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_5844
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x14]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_2124
loc_1_2173:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x31], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x30], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x3d], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x22b], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x227]
        dec    dword ptr [eax + 0x14]
        push   0xc0000001
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x227]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_5826
        mov    eax, 0xc0000001
        jmp    loc_1_230E
loc_1_21BF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f3]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_225D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f3]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f3]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f3]
        mov    al, byte ptr [eax + 0x1a]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_2217
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f3]
        mov    byte ptr [eax + 0x1a], 0
loc_1_2217:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f3]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1f3], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x24
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0xc], eax
        push   0xc001000c
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_581A
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_21BF
loc_1_225D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xaf]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1f3], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xaf]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1ef], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xa7]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x200], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1eb], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1ed], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ff], 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1f7], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1fb], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x5b], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xaf]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x202], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A07
        test   eax, eax
        je     loc_1_2307
        mov    eax, 0xc0000001
        jmp    loc_1_230E
loc_1_2307:
        xor    eax, eax
        jmp    loc_1_230E
loc_1_230E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_2315:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x61]
        mov    dword ptr [ebp - 4], eax
loc_1_2327:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x61
        cmp    eax, dword ptr [ebp - 4]
        je     loc_1_235E
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   0x40010004
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_583E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_2327
loc_1_235E:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x22b], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x3d], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x227], eax
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_238B
        mov    eax, dword ptr [ebp + 0xc]
        inc    dword ptr [eax + 0x14]
loc_1_238B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_2392:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x61
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x65], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x65]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x61], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x69
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x6d], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x6d]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x69], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x71
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x75], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x75]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x71], eax
        jmp    loc_1_23E5
loc_1_23E5:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x31], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x30], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x3d], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x2f], 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_2408:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_243D
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x1e
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax + 0x10], 4
loc_1_243D:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x22b]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_245F
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_250B
loc_1_245F:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], eax
        jne    loc_1_246B
loc_1_246B:
        test   byte ptr [ebp + 0xc], 0x10
        je     loc_1_2481
        mov    dword ptr [ebp - 4], 0xc0000001
        jmp    loc_1_250B
loc_1_2481:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax + 0x10], 4
        jne    loc_1_24AB
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 8], eax
        jmp    loc_1_24B4
loc_1_24AB:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 8], eax
loc_1_24B4:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x89], 0
        jne    loc_1_24F0
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x8d], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x8d]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x89], eax
        jmp    loc_1_24FE
loc_1_24F0:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x8d]
        mov    dword ptr [ecx], eax
loc_1_24FE:
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 4], 0x103
loc_1_250B:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_2513
loc_1_2513:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_1_251A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_254F
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x46
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax + 0x10], 9
loc_1_254F:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x22b]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_2571
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_25E0
loc_1_2571:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x89], 0
        jne    loc_1_25C5
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x8d], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x8d]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x89], eax
        jmp    loc_1_25D3
loc_1_25C5:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x8d]
        mov    dword ptr [ecx], eax
loc_1_25D3:
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 4], 0x103
loc_1_25E0:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_25E8
loc_1_25E8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
loc_1_25EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_2624
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 0x6e
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax + 0x10], 0xa
loc_1_2624:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x22b]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_2646
        mov    dword ptr [ebp - 4], 0xc001000d
        jmp    loc_1_26B5
loc_1_2646:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x89], 0
        jne    loc_1_269A
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x8d], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x8d]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x89], eax
        jmp    loc_1_26A8
loc_1_269A:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0x8d]
        mov    dword ptr [ecx], eax
loc_1_26A8:
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 4], 0x103
loc_1_26B5:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_26BD
loc_1_26BD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x14
sub_1_26C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
loc_1_26CD:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x89], 0
        je     loc_1_28BF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x89]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x89]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x89], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x89], 0
        jne    loc_1_271A
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x8d], 0
loc_1_271A:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x10], 8
        jne    loc_1_274E
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     loc_1_2742
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2F48
        jmp    loc_1_2742
loc_1_2742:
        mov    dword ptr [ebp - 8], 0x103
        jmp    loc_1_28BA
loc_1_274E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_2876
loc_1_275C:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 0x1010e
        jne    loc_1_2784
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3404
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_27CF
loc_1_2784:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x14], 0x2010103
        jne    loc_1_27B4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x99], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30AF
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_27CF
loc_1_27B4:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xa1], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
        mov    dword ptr [ebp - 8], eax
loc_1_27CF:
        jmp    loc_1_28BA
loc_1_27D4:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3404
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_28BA
loc_1_27EC:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x99], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30AF
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_28BA
loc_1_280C:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xa1], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_28BA
loc_1_282C:
        mov    dword ptr [ebp - 8], 0x103
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2F48
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 1
        jmp    loc_1_28BA
loc_1_2848:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        push   1
        push   9
        push   3
        push   0xaa000006
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
        add    esp, 0x18
        jmp    loc_1_28BA
        db  0E9h, 044h, 000h, 000h, 000h
loc_1_2876:
        dec    dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x10], 9
        ja     loc_1_2848
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]            ; -> obj1:0x28B0
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x2898
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_275C
        dd  loc_1_282C
        dd  loc_1_27D4
        dd  loc_1_27EC
        dd  loc_1_280C
        dd  loc_1_2848
        db  000h, 001h, 005h, 002h, 005h, 005h, 005h, 005h, 003h, 004h
loc_1_28BA:
        jmp    loc_1_26CD
loc_1_28BF:
        cmp    dword ptr [ebp - 8], 0x103
        jne    loc_1_28E7
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x91], eax
        mov    eax, 0x103
        jmp    loc_1_29ED
        db  0E9h, 0FFh, 000h, 000h, 000h
loc_1_28E7:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_29C2
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_297D
loc_1_28FF:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_29BD
loc_1_2913:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_29BD
loc_1_2921:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 0x14]
        jmp    loc_1_29BD
loc_1_292F:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x7d]
        push   eax
        call   sub_1_584A
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x5d]
        jmp    loc_1_29BD
loc_1_294C:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        push   9
        push   0                             ; -> sub_1_26C4
        push   3
        push   0xaa000006
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
        add    esp, 0x18
        jmp    loc_1_29BD
        db  0E9h, 040h, 000h, 000h, 000h
loc_1_297D:
        dec    dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x14], 9
        ja     loc_1_294C
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax]            ; -> obj1:0x29B3
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x299F
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_28FF
        dd  loc_1_292F
        dd  loc_1_2913
        dd  loc_1_2921
        dd  loc_1_294C
        db  000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h
loc_1_29BD:
        jmp    loc_1_29E6
loc_1_29C2:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0xa
        push   0                             ; -> sub_1_26C4
        push   3
        push   0xaa000006
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
        add    esp, 0x18
loc_1_29E6:
        xor    eax, eax
        jmp    loc_1_29ED
loc_1_29ED:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
loc_1_29F4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0x14]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_2A07:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    eax, 0                        ; -> obj1:0x5C08
        lea    ecx, [ebp - 0x14]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    edx, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 0xc], edx
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [ecx + 0x10], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        test   al, 2
        jne    loc_1_2A73
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22d]
        and    eax, 0xffff
        or     eax, 0x1000
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x22d], ax
loc_1_2A73:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x21b], 0x100
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_2AD1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xb7]
        and    eax, 0xff0000
        shr    eax, 0x10
        and    ax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xb7]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
        jmp    loc_1_2B00
loc_1_2AD1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xb3]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xb3]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
loc_1_2B00:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x215], 0x1a
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 0xb3]
        xor    eax, eax
        mov    ecx, 6
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22d]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xb3]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xb3]
        movsx  eax, word ptr [eax]
        and    eax, 0x97fd
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xb3]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xb3]
        add    eax, 2
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x45
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ecx + 5], al
        push   0x12
        lea    eax, [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xb3]
        add    eax, 8
        push   eax
        call   sub_1_59BC
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x31], 1
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_2BF2
        mov    eax, 0xc0000001
        jmp    loc_1_2C17
loc_1_2BF2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x33]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_2C10
        mov    eax, 0xc0000001
        jmp    loc_1_2C17
loc_1_2C10:
        xor    eax, eax
        jmp    loc_1_2C17
loc_1_2C17:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_2C1E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x89], 0
        je     loc_1_2C52
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_26C4
        cmp    eax, 0x103
        jne    loc_1_2C52
        mov    eax, 0x103
        jmp    loc_1_2C59
loc_1_2C52:
        xor    eax, eax
        jmp    loc_1_2C59
loc_1_2C59:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_2C60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x214]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_2C95
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x1ff]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_2C9C
loc_1_2C95:
        xor    al, al
        jmp    loc_1_2F41
loc_1_2C9C:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x214], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa30
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x231]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        or     ecx, 0x20
        mov    dx, word ptr [eax + 0x231]
        mov    ax, cx
        out    dx, ax
        mov    word ptr [ebp - 8], 0xea60
loc_1_2CE8:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_2D31
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x231]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   ah, 0x20
        je     loc_1_2D1E
        jmp    loc_1_2D31
loc_1_2D1E:
        push   0xbb8
        call   sub_1_58D0
        dec    word ptr [ebp - 8]
        jmp    loc_1_2CE8
loc_1_2D31:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_2D52
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x214], 1
        xor    al, al
        jmp    loc_1_2F41
loc_1_2D52:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa3c
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x233]
        mov    ax, word ptr [ebx + 0x215]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ecx + 0x217]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ecx + 0x219]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ecx + 0x21b]
        out    dx, ax
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_1_2E87
        push   0x186a0
        call   sub_1_58D0
        mov    word ptr [ebp - 8], 0x7530
loc_1_2DD8:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_2E31
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa42
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x231]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   al, 8
        je     loc_1_2E1E
        jmp    loc_1_2E31
loc_1_2E1E:
        push   0x3e8
        call   sub_1_58D0
        dec    word ptr [ebp - 8]
        jmp    loc_1_2DD8
loc_1_2E31:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_2E52
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x214], 1
        xor    al, al
        jmp    loc_1_2F41
loc_1_2E52:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x231]
        xor    eax, eax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa3c
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x231]
        in     ax, dx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x215], ax
loc_1_2E87:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x23d]
        mov    eax, 0xa30
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 0x231]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffdf
        mov    dx, word ptr [eax + 0x231]
        mov    ax, cx
        out    dx, ax
        mov    word ptr [ebp - 8], 0xea60
loc_1_2EC6:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_2F0F
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x231]
        in     ax, dx
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        test   ah, 0x20
        jne    loc_1_2EFC
        jmp    loc_1_2F0F
loc_1_2EFC:
        push   0x3e8
        call   sub_1_58D0
        dec    word ptr [ebp - 8]
        jmp    loc_1_2EC6
loc_1_2F0F:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_2F30
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x214], 1
        xor    al, al
        jmp    loc_1_2F41
loc_1_2F30:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x214], 1
        mov    al, 1
        jmp    loc_1_2F41
loc_1_2F41:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_2F48:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x21b], 0x800
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_2FA4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x25b]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x25b]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
        jmp    loc_1_2FDB
loc_1_2FA4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x25f]
        and    eax, 0xff0000
        shr    eax, 0x10
        and    ax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x25f]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
loc_1_2FDB:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x215], 0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_2FF9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x21b], 0x400
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3055
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x21d]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x21d]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
        jmp    loc_1_308C
loc_1_3055:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x221]
        and    eax, 0xff0000
        shr    eax, 0x10
        and    ax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x221]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
loc_1_308C:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x215], 6
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        jmp    loc_1_30A8
loc_1_30A8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_30AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0x7ffffffe
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x95]
        and    eax, 0xffff
        test   ah, 0x20
        je     loc_1_30E2
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_3112
loc_1_30E2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x95]
        and    eax, 0xffff
        test   ah, 0x40
        je     loc_1_310B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x99]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_3112
loc_1_310B:
        mov    dword ptr [ebp - 4], 0
loc_1_3112:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x9d], ecx
        je     loc_1_3193
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x21d]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0xc0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 0x18
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 4], ah
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2FF9
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3187
        mov    eax, 0xc0000001
        jmp    loc_1_319A
loc_1_3187:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x9d], eax
loc_1_3193:
        xor    eax, eax
        jmp    loc_1_319A
loc_1_319A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_31A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x21d]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0xc0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa1]
        shr    eax, 0x18
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa1]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xa1]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 4], ah
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xa1]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2FF9
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_322A
        mov    eax, 0xc0000001
        jmp    loc_1_3231
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_322A:
        xor    eax, eax
        jmp    loc_1_3231
loc_1_3231:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_3238:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0x8618
        mov    word ptr [ebp - 4], 0x8418
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x95]
        and    eax, 0xffff
        test   al, 0x20
        je     loc_1_3287
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22d]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     eax, ecx
        mov    word ptr [ebp - 0xc], ax
        jmp    loc_1_32E2
loc_1_3287:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x95]
        and    eax, 0xffff
        test   ah, 0x80
        je     loc_1_32C2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22d]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        or     eax, ecx
        mov    word ptr [ebp - 0xc], ax
        jmp    loc_1_32E2
loc_1_32C2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22d]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        not    ecx
        and    eax, ecx
        mov    word ptr [ebp - 0xc], ax
loc_1_32E2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22d]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_3309
        xor    eax, eax
        jmp    loc_1_33FD
loc_1_3309:
        mov    ax, word ptr [ebp - 0xc]
        mov    word ptr [ebp - 0x10], ax
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0x97fd
        mov    word ptr [ebp - 0x10], ax
        mov    ax, word ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x21d]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x21b], 0x2000
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3383
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x21d]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x21d]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
        jmp    loc_1_33BA
loc_1_3383:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x221]
        and    eax, 0xff0000
        shr    eax, 0x10
        and    ax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x217], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x221]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x219], ax
loc_1_33BA:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x215], 2
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_33E8
        mov    eax, 0xc0000001
        jmp    loc_1_33FD
loc_1_33E8:
        mov    ax, word ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x22d], ax
        xor    eax, eax
        jmp    loc_1_33FD
loc_1_33FD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_3404:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x97]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x95]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x97], ax
        mov    ax, word ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x95], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30AF
        test   eax, eax
        jne    loc_1_3466
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3238
        test   eax, eax
        jne    loc_1_3466
        xor    eax, eax
        jmp    loc_1_34A4
loc_1_3466:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x97]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x95], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x97], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30AF
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3238
        mov    eax, 0xc0000001
        jmp    loc_1_34A4
loc_1_34A4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        db  000h
loc_1_34AC:
        mov    al, byte ptr [0x5a68]         ; -> obj1:0x5A68
        cmp    al, 0
        jne    loc_1_34D1
        inc    byte ptr [0x5a68]             ; -> obj1:0x5A68
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_1_34D3
        push   0
        push   0
        call   sub_2_0000
        cmp    eax, 0
        jne    loc_1_34D3
loc_1_34D1:
        clc    
        ret    
loc_1_34D3:
        stc    
        ret    
IRMATRAC_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_34E4
        jmp    dword ptr [eax*4 + 0x34e6]    ; -> obj1:0x34E6
loc_1_34E4:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_1_34AC
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34E4
        dd  loc_1_34AC
        db  000h, 000h
loc_1_3554:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0x103
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x200]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_35A2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x1eb]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ecx + 0x187]
        jb     loc_1_360E
loc_1_35A2:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x214]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_360E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1ff]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_360E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x5a]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_360E
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x5a], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1ff], 0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4F2C
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1ff], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x5a], 0
loc_1_360E:
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_5900
        cmp    dword ptr [ebp - 8], 0xe
        jb     loc_1_3643
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0x245]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jae    loc_1_364D
loc_1_3643:
        mov    eax, 0xc001000f
        jmp    loc_1_37C8
loc_1_364D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x3d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3731
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + 0x1d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3725
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x24
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x10]
        inc    dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x1f7], 0
        jne    loc_1_36BB
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1f7], eax
        jmp    loc_1_36CA
loc_1_36BB:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x1fb]
        mov    dword ptr [ecx + 0x24], eax
loc_1_36CA:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1fb], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x214]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3720
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x1ff]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3720
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x1ff], 0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_37CF
loc_1_3720:
        jmp    loc_1_372C
loc_1_3725:
        mov    dword ptr [ebp - 0xc], 0xc0010002
loc_1_372C:
        jmp    loc_1_378F
loc_1_3731:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x22b]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3753
        mov    dword ptr [ebp - 0xc], 0xc001000d
        jmp    loc_1_378F
loc_1_3753:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x30]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3772
        mov    dword ptr [ebp - 0xc], 0xc0000001
        jmp    loc_1_378F
loc_1_3772:
        push   2
        push   0xb
        push   2
        push   0xaa000006
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
        add    esp, 0x14
loc_1_378F:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x22b]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_37C0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 0x69
        cmp    dword ptr [eax + 0x69], ecx
        je     loc_1_37C0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_502C
loc_1_37C0:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_37C8
loc_1_37C8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_37CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 0x1c], 0
        mov    word ptr [ebp - 0x18], 0
loc_1_37EC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1f7], 0
        je     loc_1_3B66
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1f7]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp - 0x24]
        add    eax, 0x24
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x200]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_3846
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1eb]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x187]
        jb     loc_1_384B
loc_1_3846:
        jmp    loc_1_3B66
loc_1_384B:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   eax, eax
        je     loc_1_385D
        jmp    loc_1_385D
loc_1_385D:
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_5900
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   eax, eax
        je     loc_1_3886
        jmp    loc_1_3886
loc_1_3886:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 0x200]
        cmp    dword ptr [ebp - 0x10], 0x100
        jae    loc_1_3977
        mov    word ptr [ebp - 0x18], 0
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1f7], eax
loc_1_38B1:
        cmp    dword ptr [ebp - 0x20], 0
        je     loc_1_3912
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        call   sub_1_5924
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ef]
        mov    eax, dword ptr [eax + 0x14]
        mov    ecx, dword ptr [ebp - 0x18]
        and    ecx, 0xffff
        add    eax, ecx
        push   eax
        call   sub_1_59BC
        lea    eax, [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        call   sub_1_5948
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        add    eax, dword ptr [ebp - 8]
        mov    word ptr [ebp - 0x18], ax
        jmp    loc_1_38B1
loc_1_3912:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1ef]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0xffff
        add    ecx, 0xc
        mov    dx, word ptr [eax + 0x23d]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1ef]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ef]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1ef], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0x10]
        or     ecx, 0x2100
        mov    dx, word ptr [eax + 0x231]
        mov    ax, cx
        out    dx, ax
        jmp    loc_1_3B61
loc_1_3977:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ef]
        mov    byte ptr [eax + 0x1a], 1
        mov    eax, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1f7], eax
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1eb]
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_1_39AE
        jmp    loc_1_39AE
loc_1_39AE:
        lea    eax, [ebp - 0x1c]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1ed]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x18b]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_3BFC
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   eax, eax
        jne    loc_1_39ED
        jmp    loc_1_39ED
loc_1_39ED:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1ef]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0xffff
        add    ecx, 6
        mov    dx, word ptr [eax + 0x23d]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ebp - 0x10]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3A9E
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1ed]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0xc7]
        mov    dword ptr [ebp - 0x30], eax
        lea    eax, [ebp - 0x14]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    ecx, dword ptr [ebp - 0x2c]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp - 0x30]
        and    eax, 0xff0000
        shr    eax, 0x10
        and    ax, 0xff
        mov    ecx, dword ptr [ebp - 0x2c]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ebp - 0x14]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x233]
        mov    ax, word ptr [ebp - 0x12]
        out    dx, ax
        jmp    loc_1_3AF9
loc_1_3A9E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x1ed]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0x18b]
        shr    ecx, 0x10
        mov    dx, word ptr [eax + 0x233]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0x1ed]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [edx + ecx*4 + 0x18b]
        and    ecx, 0xffff
        mov    dx, word ptr [eax + 0x233]
        mov    eax, ecx
        out    dx, ax
loc_1_3AF9:
        mov    eax, dword ptr [ebp - 0x24]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1ef]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0x1ed]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1ed]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x187]
        jne    loc_1_3B3C
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x1ed], 0
loc_1_3B3C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1ef]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1ef], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 0x231]
        mov    eax, 0x100
        out    dx, ax
loc_1_3B61:
        jmp    loc_1_37EC
loc_1_3B66:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ff], 1
        jmp    loc_1_3B75
loc_1_3B75:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
loc_1_3B7C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 0x5d]
        cmp    byte ptr [esi + 0x3d], 0
        jne    loc_1_3BC1
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_1_3BBA
        mov    eax, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x18]
        push   eax
        mov    edx, dword ptr [ebp + 0x14]
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   ecx
        add    eax, dword ptr [ebp + 0x10]
        push   edx
        inc    dword ptr [edi + 0x14]
        push   eax
        call   sub_1_3CDC
        dec    dword ptr [edi + 0x14]
        xor    edi, edi
        jmp    loc_1_3BC6
loc_1_3BBA:
        mov    edi, 0xc001000c
        jmp    loc_1_3BC6
loc_1_3BC1:
        mov    edi, 0xc001000d
loc_1_3BC6:
        mov    eax, dword ptr [esi + 0x5d]
        dec    eax
        mov    dword ptr [esi + 0x5d], eax
        jne    loc_1_3BE8
        cmp    byte ptr [esi + 0x22b], 0
        jne    loc_1_3BE2
        mov    eax, dword ptr [esi + 0x69]
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_3BE8
loc_1_3BE2:
        push   esi
        call   sub_1_502C
loc_1_3BE8:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    0x18
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3BFC:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_3CC5
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x10]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_5900
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_1_3CC5
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5924
        cmp    dword ptr [ebp + 0x10], esi
        je     loc_1_3CC0
        mov    edi, dword ptr [ebp + 0xc]
loc_1_3C4B:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3C77
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_5948
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_3CC0
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5924
        jmp    loc_1_3CBB
loc_1_3C77:
        test   edi, edi
        je     loc_1_3C94
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_3C8C
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_3CBB
loc_1_3C8C:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_3C94:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     loc_1_3CA2
        mov    ebx, eax
loc_1_3CA2:
        push   ebx
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        add    esi, ebx
        call   sub_1_59BC
        add    dword ptr [ebp + 0x14], ebx
        add    dword ptr [ebp - 0xc], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_3CBB:
        cmp    esi, dword ptr [ebp + 0x10]
        jb     loc_1_3C4B
loc_1_3CC0:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_3CC5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3CDC:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_3DA8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    edx, dword ptr [ebp + 0x10]
        push   esi
        push   edx
        call   sub_1_5900
        cmp    dword ptr [ebp - 0x14], esi
        je     loc_1_3DA8
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5924
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], esi
        mov    dword ptr [ebp - 0x10], ecx
        je     loc_1_3DA3
        mov    edi, dword ptr [ebp + 0x14]
loc_1_3D31:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3D5D
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_5948
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_3DA3
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5924
        jmp    loc_1_3D9E
loc_1_3D5D:
        test   edi, edi
        je     loc_1_3D7A
        cmp    dword ptr [ebp - 4], edi
        jae    loc_1_3D72
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_3D9E
loc_1_3D72:
        add    dword ptr [ebp - 0xc], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_3D7A:
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     loc_1_3D88
        mov    ebx, eax
loc_1_3D88:
        push   ebx
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    ecx, dword ptr [ebp - 0xc]
        push   ecx
        add    esi, ebx
        call   sub_1_59BC
        add    dword ptr [ebp - 0x10], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_3D9E:
        cmp    esi, dword ptr [ebp + 0xc]
        jb     loc_1_3D31
loc_1_3DA3:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], esi
loc_1_3DA8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 01Ch
        db  08Bh, 0ECh, 083h, 0ECh, 024h, 0C7h, 000h, 000h, 000h, 000h, 000h, 053h, 0C7h, 045h, 0E4h, 000h
        db  000h, 000h, 000h, 083h, 07Dh, 010h, 000h, 056h, 057h, 00Fh, 084h, 056h, 001h, 000h, 000h, 06Ah
        db  000h, 08Dh, 045h, 0F0h, 050h, 08Dh, 04Dh, 0DCh, 051h, 08Bh, 055h, 008h, 06Ah, 000h, 052h, 0E8h
        db  00Bh, 01Bh, 000h, 000h, 083h, 07Dh, 0DCh, 000h, 00Fh, 084h, 037h, 001h, 000h, 000h, 08Dh, 045h
        db  0FCh, 08Dh, 04Dh, 0E8h, 050h, 08Bh, 055h, 0F0h, 051h, 052h, 0E8h, 014h, 01Bh, 000h, 000h, 06Ah
        db  000h, 08Dh, 04Dh, 0F4h, 051h, 08Dh, 055h, 0E0h, 052h, 08Bh, 045h, 014h, 06Ah, 000h, 050h, 0E8h
        db  0DBh, 01Ah, 000h, 000h, 083h, 07Dh, 0E0h, 000h, 00Fh, 084h, 007h, 001h, 000h, 000h, 08Dh, 045h
        db  0F8h, 08Dh, 04Dh, 0ECh, 050h, 08Bh, 055h, 0F4h, 051h, 052h, 0E8h, 0E4h, 01Ah, 000h, 000h, 083h
        db  07Dh, 010h, 000h, 00Fh, 084h, 0E4h, 000h, 000h, 000h, 08Bh, 075h, 00Ch, 08Bh, 07Dh, 018h, 083h
        db  07Dh, 0FCh, 000h, 075h, 02Dh, 08Dh, 045h, 0F0h, 08Bh, 04Dh, 0F0h, 050h, 051h, 0E8h, 0E5h, 01Ah
        db  000h, 000h, 083h, 07Dh, 0F0h, 000h, 00Fh, 084h, 0C1h, 000h, 000h, 000h, 08Dh, 045h, 0FCh, 08Dh
        db  04Dh, 0E8h, 050h, 08Bh, 055h, 0F0h, 051h, 052h, 0E8h, 0A6h, 01Ah, 000h, 000h, 0E9h, 09Fh, 000h
        db  000h, 000h, 083h, 07Dh, 0F8h, 000h, 075h, 02Ah, 08Dh, 045h, 0F4h, 08Bh, 04Dh, 0F4h, 050h, 051h
        db  0E8h, 0B2h, 01Ah, 000h, 000h, 083h, 07Dh, 0F4h, 000h, 00Fh, 084h, 08Eh, 000h, 000h, 000h, 08Dh
        db  045h, 0F8h, 08Dh, 04Dh, 0ECh, 050h, 08Bh, 055h, 0F4h, 051h, 052h, 0E8h, 073h, 01Ah, 000h, 000h
        db  0EBh, 06Fh, 085h, 0F6h, 074h, 019h, 03Bh, 075h, 0FCh, 076h, 00Ch, 02Bh, 075h, 0FCh, 0C7h, 045h
        db  0FCh, 000h, 000h, 000h, 000h, 0EBh, 05Ah, 001h, 075h, 0E8h, 029h, 075h, 0FCh, 033h, 0F6h, 085h
        db  0FFh, 074h, 019h, 03Bh, 07Dh, 0F8h, 076h, 00Ch, 02Bh, 07Dh, 0F8h, 0C7h, 045h, 0F8h, 000h, 000h
        db  000h, 000h, 0EBh, 03Dh, 001h, 07Dh, 0ECh, 029h, 07Dh, 0F8h, 033h, 0FFh, 08Bh, 05Dh, 0F8h, 08Bh
        db  045h, 0FCh, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 08Bh, 045h, 010h, 02Bh, 045h, 0E4h, 03Bh, 0D8h
        db  072h, 002h, 08Bh, 0D8h, 053h, 08Bh, 045h, 0ECh, 050h, 08Bh, 04Dh, 0E8h, 051h, 0E8h, 0A9h, 01Ah
        db  000h, 000h, 001h, 05Dh, 0E8h, 001h, 05Dh, 0ECh, 001h, 05Dh, 0E4h, 029h, 05Dh, 0F8h, 029h, 05Dh
        db  0FCh, 08Bh, 04Dh, 0E4h, 039h, 04Dh, 010h, 00Fh, 087h, 022h, 0FFh, 0FFh, 0FFh, 08Bh, 045h, 01Ch
        db  08Bh, 04Dh, 0E4h, 089h, 008h, 05Fh, 05Eh, 05Bh, 08Bh, 0E5h, 05Dh, 0C2h, 018h, 000h, 08Dh, 09Bh
        db  000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3F4C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        inc    dword ptr [edi + 0x5d]
        mov    edx, dword ptr [ecx + 0x10]
        test   edx, edx
        jne    loc_1_3F73
        push   ecx
        push   eax
        push   edi
        call   sub_1_443C
        mov    esi, eax
        jmp    loc_1_3FD5
loc_1_3F73:
        cmp    edx, 1
        jne    loc_1_3FD0
        cmp    byte ptr [edi + 0x3d], 0
        jne    loc_1_3F97
        cmp    byte ptr [eax + 0x1d], 0
        jne    loc_1_3F90
        push   ecx
        push   eax
        push   edi
        call   sub_1_448C
        mov    esi, eax
        jmp    loc_1_3FD5
loc_1_3F90:
        mov    esi, 0xc0010002
        jmp    loc_1_3FD5
loc_1_3F97:
        cmp    byte ptr [edi + 0x22b], 1
        jne    loc_1_3FA7
        mov    esi, 0xc001000d
        jmp    loc_1_3FD5
loc_1_3FA7:
        cmp    byte ptr [edi + 0x30], 1
        jne    loc_1_3FB4
        mov    esi, 0xc0000001
        jmp    loc_1_3FD5
loc_1_3FB4:
        push   3
        mov    eax, dword ptr [edi + 0x81]
        push   0xb
        push   2
        push   0xaa000006
        push   eax
        call   sub_1_59A8
        add    esp, 0x14
        jmp    loc_1_3FD5
loc_1_3FD0:
        mov    esi, 0x10001
loc_1_3FD5:
        mov    eax, dword ptr [edi + 0x5d]
        dec    eax
        mov    dword ptr [edi + 0x5d], eax
        jne    loc_1_3FF7
        cmp    byte ptr [edi + 0x22b], 0
        jne    loc_1_3FF1
        mov    eax, dword ptr [edi + 0x69]
        sub    eax, edi
        cmp    eax, 0x69
        je     loc_1_3FF7
loc_1_3FF1:
        push   edi
        call   sub_1_502C
loc_1_3FF7:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_400C:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        mov    dword ptr [ebp - 0x10], eax
        push   esi
        mov    ecx, dword ptr [ebp + 0x10]
        push   edi
        mov    esi, 4
        lea    edi, [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        cmp    ecx, 0x2010101
        mov    eax, ecx
        mov    dword ptr [ebp - 0x14], 1
        ja     loc_1_4050
        je     loc_1_4286
        sub    eax, 0x10101
        cmp    eax, 0x12
        ja     loc_1_4061
        jmp    dword ptr [eax*4]             ; -> obj1:0x43D8
loc_1_4050:
        sub    eax, 0x2010102
        cmp    eax, 5
        ja     loc_1_4061
        jmp    dword ptr [eax*4]             ; -> obj1:0x4424
loc_1_4061:
        mov    dword ptr [ebp - 0x10], 0xc00000bb
        jmp    loc_1_4399
loc_1_406D:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_4082
        mov    edi, 0                        ; -> obj1:0x5CE4
        mov    esi, 0x68
        jmp    loc_1_4399
loc_1_4082:
        mov    edi, 0                        ; -> loc_1_5C44
        mov    esi, 0xa0
        jmp    loc_1_4399
loc_1_4091:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0x22b], 0
        je     loc_1_40A6
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_1_40D1
loc_1_40A6:
        cmp    byte ptr [ebx + 0x2f], 0
        jne    loc_1_40CA
        cmp    byte ptr [ebx + 0x31], 0
        jne    loc_1_40CA
        cmp    byte ptr [ebx + 0x3d], 0
        je     loc_1_40C1
        mov    dword ptr [ebp - 0xc], 4
        jmp    loc_1_40D1
loc_1_40C1:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_40D1
loc_1_40CA:
        mov    dword ptr [ebp - 0xc], 1
loc_1_40D1:
        lea    edi, [ebp - 0xc]
        mov    esi, 4
        jmp    loc_1_4399
loc_1_40DE:
        lea    edi, [ebp - 0x14]
        mov    esi, 4
        jmp    loc_1_4399
loc_1_40EB:
        mov    dword ptr [ebp - 8], 0x100
        jmp    loc_1_4399
loc_1_40F7:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x245]
        cmp    ecx, 0x10106
        mov    dword ptr [ebp - 8], eax
        jne    loc_1_4399
        sub    dword ptr [ebp - 8], 0xe
        jmp    loc_1_4399
loc_1_411B:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x244]
        and    al, 1
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0xfffe2b40
        add    eax, 0x27100
        jmp    loc_1_4396
loc_1_4139:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0xa7]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x245]
        imul   ecx, eax
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_4399
loc_1_4159:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0xab]
        imul   eax, dword ptr [ebx + 0x247]
        jmp    loc_1_4396
loc_1_4171:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x245]
        jmp    loc_1_4396
loc_1_4182:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x247]
        jmp    loc_1_4396
loc_1_4190:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 8]
        lea    eax, [ebx + 0x3f]
        push   eax
        push   ecx
        call   sub_1_59BC
        and    dword ptr [ebp - 8], 0xffffff00
        test   byte ptr [ebx + 0x244], 0x10
        jne    loc_1_4399
        or     dword ptr [ebp - 8], 1
        jmp    loc_1_4399
loc_1_41BF:
        mov    ebx, dword ptr [ebp + 8]
        test   byte ptr [ebx + 0x244], 0x10
        jne    loc_1_41DA
        mov    edi, 0                        ; -> obj1:0x5D4C
        mov    esi, 0x31
        jmp    loc_1_4399
loc_1_41DA:
        mov    edi, 0                        ; -> loc_1_5D80
        mov    esi, 0x2e
        jmp    loc_1_4399
loc_1_41E9:
        cmp    byte ptr [ebp + 0x14], 0
        je     loc_1_4200
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x95]
        jmp    loc_1_4396
loc_1_4200:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_4399
loc_1_4211:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_4225
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_4399
loc_1_4225:
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, ebx
        mov    ecx, dword ptr [ebx + 0x61]
        sub    eax, ecx
        cmp    eax, -0x61
        je     loc_1_4399
        mov    edx, 0x100
loc_1_4244:
        mov    eax, dword ptr [ecx + 0x18]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_4257
        mov    dword ptr [ebp - 8], eax
        cmp    eax, edx
        je     loc_1_4399
loc_1_4257:
        mov    ecx, dword ptr [ecx]
        mov    eax, ebx
        sub    eax, ecx
        cmp    eax, -0x61
        jne    loc_1_4244
        jmp    loc_1_4399
loc_1_4267:
        lea    edi, [ebp - 2]
        mov    esi, 2
        mov    word ptr [ebp - 2], 0x300
        jmp    loc_1_4399
loc_1_427A:
        mov    dword ptr [ebp - 8], 6
        jmp    loc_1_4399
loc_1_4286:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x3f]
        add    ebx, 0x3f
        mov    byte ptr [ebp - 0x1c], al
        mov    cl, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 0x1b], cl
        mov    dl, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 0x1a], dl
        lea    edi, [ebp - 0x1c]
        mov    al, byte ptr [ebx + 3]
        mov    esi, 6
        mov    byte ptr [ebp - 0x19], al
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 0x18], cl
        mov    dl, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 0x17], dl
        jmp    loc_1_4399
loc_1_42BD:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x45]
        add    ebx, 0x45
        mov    byte ptr [ebp - 0x1c], al
        mov    cl, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 0x1b], cl
        mov    dl, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 0x1a], dl
        lea    edi, [ebp - 0x1c]
        mov    al, byte ptr [ebx + 3]
        mov    esi, 6
        mov    byte ptr [ebp - 0x19], al
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 0x18], cl
        mov    dl, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 0x17], dl
        jmp    loc_1_4399
loc_1_42F4:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_1_4308
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 0xc]
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_4314
loc_1_4308:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x9d]
        mov    dword ptr [ebp - 8], eax
loc_1_4314:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 0x10
        and    edx, 0xff00
        or     edx, eax
        mov    ecx, dword ptr [ebp - 8]
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        shr    ecx, 0x10
        and    eax, 0xff0000
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_4399
loc_1_4342:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0xa1]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        shl    eax, 0x10
        and    edx, 0xff00
        or     edx, eax
        mov    ecx, dword ptr [ebp - 8]
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        shr    ecx, 0x10
        and    eax, 0xff0000
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    loc_1_4399
loc_1_4378:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x33]
        or     eax, 0xc0011000
        jmp    loc_1_4396
loc_1_4388:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x35]
        jmp    loc_1_4396
loc_1_4390:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x39]
loc_1_4396:
        mov    dword ptr [ebp - 8], eax
loc_1_4399:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_43CC
        cmp    esi, dword ptr [ebp + 0x1c]
        jbe    loc_1_43BC
        mov    eax, dword ptr [ebp + 0x20]
        pop    edi
        mov    dword ptr [ebp - 0x10], 0xc0010014
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 0x10]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
loc_1_43BC:
        push   esi
        mov    eax, dword ptr [ebp + 0x18]
        push   edi
        push   eax
        call   sub_1_59BC
        mov    eax, dword ptr [ebp + 0x24]
        add    dword ptr [eax], esi
loc_1_43CC:
        mov    eax, dword ptr [ebp - 0x10]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x20
        ; ---- dispatch table (25 entries) ----
        dd  loc_1_406D
        dd  loc_1_4091
        dd  loc_1_40DE
        dd  loc_1_40DE
        dd  loc_1_40EB
        dd  loc_1_40F7
        dd  loc_1_411B
        dd  loc_1_4139
        dd  loc_1_4159
        dd  loc_1_4171
        dd  loc_1_4182
        dd  loc_1_4190
        dd  loc_1_41BF
        dd  loc_1_41E9
        dd  loc_1_4211
        dd  loc_1_4267
        dd  loc_1_40F7
        dd  loc_1_4061
        dd  loc_1_427A
        dd  loc_1_42BD
        dd  loc_1_42F4
        dd  loc_1_4342
        dd  loc_1_4378
        dd  loc_1_4388
        dd  loc_1_4390
sub_1_443C:
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
        call   sub_1_400C
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_448C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        mov    edi, dword ptr [esi + 0x1c]
        cmp    edi, 4
        je     loc_1_44B2
        mov    ebx, 0xc0010014
        jmp    loc_1_45A6
loc_1_44B2:
        cmp    eax, 0x2010103
        ja     loc_1_44E2
        je     loc_1_4584
        cmp    eax, 0x1010e
        je     loc_1_44F7
        cmp    eax, 0x1010f
        je     loc_1_451A
        cmp    eax, 0x10112
        je     loc_1_4580
        mov    ebx, 0xc0010017
        jmp    loc_1_45A6
loc_1_44E2:
        cmp    eax, 0x2010104
        je     loc_1_4595
        mov    ebx, 0xc0010017
        jmp    loc_1_45A6
loc_1_44F7:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_59BC
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_45DC
        jmp    loc_1_45A4
loc_1_451A:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_59BC
        cmp    dword ptr [ebp - 4], 0x100
        jbe    loc_1_4536
        mov    ebx, 0xc0010014
        jmp    loc_1_45A6
loc_1_4536:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0x264]
        cmp    dword ptr [ebp - 4], edx
        jbe    loc_1_4555
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], edx
        mov    dword ptr [eax + 0x264], edx
        jmp    loc_1_45A6
loc_1_4555:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ecx + 0x18]
        cmp    ecx, edx
        jne    loc_1_4575
        cmp    dword ptr [ebp - 4], ecx
        jae    loc_1_4575
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        push   eax
        mov    dword ptr [ecx + 0x18], edx
        call   sub_1_0F8E
        jmp    loc_1_45A6
loc_1_4575:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x18], eax
        jmp    loc_1_45A6
loc_1_4580:
        xor    ebx, ebx
        jmp    loc_1_45A6
loc_1_4584:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   eax
        call   sub_1_48BC
        jmp    loc_1_45A4
loc_1_4595:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   eax
        call   sub_1_48FC
loc_1_45A4:
        mov    ebx, eax
loc_1_45A6:
        cmp    ebx, 0x103
        je     loc_1_45CB
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, 0x103
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x20], edi
        mov    eax, dword ptr [ecx + 0x10]
        push   eax
        call   sub_1_582C
loc_1_45CB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_45DC:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        test   al, 0x56
        push   esi
        push   edi
        je     loc_1_45F4
        mov    eax, 0xc00000bb
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
loc_1_45F4:
        mov    esi, dword ptr [ebp + 0xc]
        push   1
        push   eax
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi + 0xc]
        push   eax
        inc    dword ptr [esi + 0x14]
        push   ecx
        mov    eax, dword ptr [edi + 0x85]
        push   eax
        call   sub_2_0E10
loc_1_4614:
        dec    dword ptr [esi + 0x14]
        cmp    eax, 0x103
        jne    loc_1_4624
        push   edi
        call   sub_1_2C1E
loc_1_4624:
        pop    edi
        pop    esi
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh
sub_1_462C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
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
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 0x14], ecx
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 0x14]
        push   ecx
        mov    eax, dword ptr [esi + 0x14]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_400C
        cmp    eax, 0xc00000bb
        mov    edi, eax
        jne    loc_1_477C
        xor    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 0x2020101
        ja     loc_1_469F
        je     loc_1_4703
        sub    eax, 0x20101
        cmp    eax, 4
        ja     loc_1_46AF
        jmp    dword ptr [eax*4]             ; -> obj1:0x4794
loc_1_469F:
        cmp    eax, 0x2020201
        ja     loc_1_46B9
        je     loc_1_471C
        cmp    eax, 0x2020102
        je     loc_1_470F
loc_1_46AF:
        mov    edi, 0xc0010017
        jmp    loc_1_474D
loc_1_46B9:
        sub    eax, 0x2020202
        cmp    eax, 5
        ja     loc_1_46AF
        jmp    dword ptr [eax*4]             ; -> obj1:0x47A8
loc_1_46CA:
        mov    eax, dword ptr [ebx + 0x24b]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_474D
loc_1_46D5:
        mov    eax, dword ptr [ebx + 0x24f]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_474D
loc_1_46E0:
        mov    eax, dword ptr [ebx + 0x253]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_474D
loc_1_46EB:
        mov    eax, dword ptr [ebx + 0x257]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_474D
loc_1_46F6:
        mov    eax, dword ptr [ebx + 0x25b]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 6]
        jmp    loc_1_474A
loc_1_4703:
        mov    eax, dword ptr [ebx + 0x25b]
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        jmp    loc_1_474A
loc_1_470F:
        mov    eax, dword ptr [ebx + 0x25b]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 5]
        jmp    loc_1_474A
loc_1_471C:
        mov    eax, dword ptr [ebx + 0x25b]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        jmp    loc_1_474A
loc_1_4729:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_474D
loc_1_4732:
        mov    eax, dword ptr [ebx + 0x25b]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 7]
        jmp    loc_1_474A
loc_1_473F:
        mov    eax, dword ptr [ebx + 0x25b]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 9]
loc_1_474A:
        mov    dword ptr [ebp - 4], ecx
loc_1_474D:
        test   edi, edi
        jne    loc_1_477C
        cmp    dword ptr [ebp - 0x10], 4
        jb     loc_1_476C
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_59BC
        add    dword ptr [ebp - 0xc], 4
        jmp    loc_1_477C
loc_1_476C:
        mov    eax, 4
        mov    edi, 0xc0010014
        sub    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 8], eax
loc_1_477C:
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
        db  090h
        ; ---- dispatch table (11 entries) ----
        dd  loc_1_46CA
        dd  loc_1_46D5
        dd  loc_1_46E0
        dd  loc_1_46EB
        dd  loc_1_46F6
        dd  loc_1_4729
        dd  loc_1_4729
        dd  loc_1_4732
        dd  loc_1_4729
        dd  loc_1_473F
        dd  loc_1_4729
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_47CC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        inc    dword ptr [esi + 0x5d]
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x10111
        ja     loc_1_4803
        cmp    eax, 0x1010e
        jae    loc_1_4859
        cmp    eax, 0x10101
        jb     loc_1_47FC
        cmp    eax, 0x1010c
        jbe    loc_1_4859
loc_1_47FC:
        mov    edi, 0xc0010017
        jmp    loc_1_4859
loc_1_4803:
        cmp    eax, 0x20105
        ja     loc_1_481F
        cmp    eax, 0x20101
        jae    loc_1_4853
        cmp    eax, 0x10113
        je     loc_1_4859
        mov    edi, 0xc0010017
        jmp    loc_1_4859
loc_1_481F:
        cmp    eax, 0x2020102
        ja     loc_1_4842
        cmp    eax, 0x2020101
        jae    loc_1_4853
        cmp    eax, 0x2010101
        jb     loc_1_47FC
        cmp    eax, 0x2010107
        jbe    loc_1_4859
        mov    edi, 0xc0010017
        jmp    loc_1_4859
loc_1_4842:
        sub    eax, 0x2020201
        cmp    eax, 6
        ja     loc_1_47FC
        jmp    dword ptr [eax*4]             ; -> obj1:0x4894
loc_1_4853:
        push   esi
        call   sub_1_2F48
loc_1_4859:
        test   edi, edi
        jne    loc_1_4866
        push   ebx
        push   esi
        call   sub_1_462C
        mov    edi, eax
loc_1_4866:
        mov    eax, dword ptr [esi + 0x5d]
        dec    eax
        mov    dword ptr [esi + 0x5d], eax
        jne    loc_1_4888
        cmp    byte ptr [esi + 0x22b], 0
        jne    loc_1_4882
        mov    eax, dword ptr [esi + 0x69]
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_4888
loc_1_4882:
        push   esi
        call   sub_1_502C
loc_1_4888:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_4853
        dd  loc_1_4859
        dd  loc_1_4859
        dd  loc_1_4853
        dd  loc_1_4859
        dd  loc_1_4853
        dd  loc_1_4859
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_48BC:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [esi + 0xc]
        push   edx
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 0x14]
        mov    ecx, dword ptr [eax + 0x85]
        push   ecx
        call   sub_2_0BE0
loc_1_48E4:
        dec    dword ptr [esi + 0x14]
        cmp    eax, 0x103
        jne    loc_1_48F7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C1E
loc_1_48F7:
        pop    esi
        pop    ebp
        ret    0x10
sub_1_48FC:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        mov    edx, dword ptr [esi + 0xc]
        push   edx
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 0x14]
        mov    ecx, dword ptr [eax + 0x85]
        push   ecx
        call   sub_2_0C80
loc_1_4924:
        dec    dword ptr [esi + 0x14]
        cmp    eax, 0x103
        jne    loc_1_4937
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C1E
loc_1_4937:
        pop    esi
        pop    ebp
        ret    0x10
loc_1_493C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    si, word ptr [ecx + 8]
        mov    edx, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx]
        mov    ax, word ptr [eax + 0x5b]
        and    ax, si
        not    si
        or     word ptr [edx], ax
        mov    al, 1
        mov    ecx, dword ptr [ecx]
        and    word ptr [ecx + 0x5b], si
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_496C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi + 0x23b]
        in     al, dx
        and    al, 3
        cmp    al, 3
        jne    loc_1_49DB
        mov    dx, word ptr [edi + 0x23f]
        push   edi
        in     ax, dx
        mov    bx, ax
        mov    dx, word ptr [edi + 0x23d]
        in     ax, dx
        mov    si, ax
        call   sub_1_52AC
        mov    dx, word ptr [edi + 0x23f]
        mov    ax, bx
        out    dx, ax
        mov    dx, word ptr [edi + 0x23d]
        mov    ax, si
        out    dx, ax
loc_1_49B9:
        test   byte ptr [edi + 0x244], 0x10
        jne    loc_1_49CC
        mov    dx, word ptr [edi + 0x241]
        mov    al, 0xff
        out    dx, al
loc_1_49CC:
        cmp    byte ptr [edi + 0x30], 1
        sbb    al, al
        pop    edi
        neg    al
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_49DB:
        mov    dx, word ptr [edi + 0x237]
        in     al, dx
        and    al, 0x7f
        out    dx, al
        cmp    byte ptr [edi + 0x31], 0
        je     loc_1_49B9
        mov    bx, word ptr [edi + 0x23d]
        mov    dx, bx
        in     ax, dx
        mov    cx, ax
        mov    eax, 0xa30
        out    dx, ax
        mov    si, word ptr [edi + 0x231]
        mov    dx, si
        in     ax, dx
        test   ah, 0x10
        je     loc_1_4A21
        mov    word ptr [edi + 0x5b], 4
        mov    word ptr [edi + 0x33], 0
        jmp    loc_1_4A5B
loc_1_4A21:
        mov    dx, bx
        mov    eax, 0xa42
        out    dx, ax
        mov    dx, si
        in     ax, dx
        test   al, 8
        je     loc_1_4A5B
        test   ah, 1
        je     loc_1_4A5B
        xor    eax, eax
        out    dx, ax
        mov    dx, bx
        mov    eax, 0xa3c
        out    dx, ax
        mov    dx, si
        in     ax, dx
        mov    word ptr [edi + 0x33], ax
        test   ax, ax
        je     loc_1_4A5B
        mov    word ptr [edi + 0x5b], 4
loc_1_4A5B:
        mov    dx, word ptr [edi + 0x23d]
        test   byte ptr [edi + 0x244], 0x10
        mov    ax, cx
        out    dx, ax
        jne    loc_1_4A7A
        mov    dx, word ptr [edi + 0x241]
        mov    al, 0xff
        out    dx, al
loc_1_4A7A:
        mov    al, byte ptr [edi + 0x5b]
        pop    edi
        and    al, 4
        pop    esi
        shr    al, 2
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_4A8C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 2], 0
        push   ebx
        push   esi
        push   edi
        mov    bl, 1
        mov    esi, dword ptr [ebp + 0xc]
        mov    dx, word ptr [esi + 0x23d]
        in     ax, dx
        mov    di, ax
loc_1_4AAC:
        cmp    byte ptr [esi + 0x59], 1
        je     loc_1_4AE3
        mov    byte ptr [esi + 0x59], 1
        push   esi
        call   sub_1_4BAC
        mov    byte ptr [esi + 0x59], 0
        test   al, al
        jne    loc_1_4AC8
        test   bl, bl
        je     loc_1_4AE3
loc_1_4AC8:
        xor    bl, bl
        cmp    byte ptr [esi + 0x5a], 1
        je     loc_1_4AE3
        mov    byte ptr [esi + 0x5a], 1
        push   1
        push   esi
        call   sub_1_4F2C
        mov    byte ptr [esi + 0x5a], bl
        test   al, al
        jne    loc_1_4AAC
loc_1_4AE3:
        cmp    byte ptr [esi + 0x31], 1
        jne    loc_1_4B23
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 0xc]
        push   ecx
        mov    dword ptr [ebp - 0xc], esi
        push   0                             ; -> loc_1_493C
        mov    dword ptr [ebp - 8], eax
        mov    word ptr [ebp - 4], 4
        push   esi
        call   sub_1_58CA
        test   byte ptr [ebp - 2], 4
        je     loc_1_4B23
        push   esi
        inc    dword ptr [esi + 0x5d]
        and    word ptr [ebp - 2], 0xfffb
        mov    byte ptr [esi + 0x31], 0
        call   sub_1_4CDC
        dec    dword ptr [esi + 0x5d]
loc_1_4B23:
        cmp    byte ptr [esi + 0x30], 0
        jne    loc_1_4B75
        cmp    byte ptr [esi + 0x214], 1
        jne    loc_1_4B75
        mov    bx, word ptr [esi + 0x23d]
        mov    eax, 0xa30
        mov    dx, bx
        out    dx, ax
        mov    cx, word ptr [esi + 0x231]
        mov    dx, cx
        in     ax, dx
        test   al, 1
        je     loc_1_4B75
        and    eax, 0xfffe
        out    dx, ax
        mov    dx, bx
        mov    eax, 0xa3a
        out    dx, ax
        mov    dx, cx
        in     ax, dx
        test   ax, ax
        je     loc_1_4B75
        push   eax
        push   esi
        call   sub_1_4E2C
loc_1_4B75:
        cmp    byte ptr [esi + 0x22b], 0
        jne    loc_1_4B8E
        mov    eax, dword ptr [esi + 0x69]
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_4B8E
        push   esi
        call   sub_1_502C
loc_1_4B8E:
        mov    dx, word ptr [esi + 0x23d]
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    0x10
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4BAC:
        push   ebp
        xor    cl, cl
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x22b], 1
        jne    loc_1_4C29
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_4BC9:
        mov    eax, 0xe
loc_1_4BCE:
        cmp    eax, ecx
        jbe    loc_1_4BEB
        cmp    ecx, 6
        jb     loc_1_4C02
        add    eax, edx
        push   0
        push   0
        push   0
        push   ecx
        push   edx
        push   eax
        mov    ecx, dword ptr [esi + 0x85]
        push   ecx
        jmp    loc_1_4BFD
loc_1_4BEB:
        sub    ecx, eax
        lea    ebx, [eax + edx]
        push   ecx
        push   ecx
        push   ebx
        push   eax
        push   edx
        mov    eax, dword ptr [esi + 0x85]
        push   ebx
        push   eax
loc_1_4BFD:
        call   sub_1_5350
loc_1_4C02:
        mov    eax, dword ptr [esi + 0x85]
        push   eax
        call   sub_1_55A0
        mov    dx, word ptr [esi + 0x23d]
        mov    eax, edi
        out    dx, ax
        mov    dx, word ptr [esi + 0x231]
        mov    eax, 0x200
        out    dx, ax
        mov    cl, 1
loc_1_4C29:
        mov    bx, word ptr [esi + 0x212]
        mov    ax, bx
        add    ax, 0xa
        mov    dx, word ptr [esi + 0x23d]
        mov    edi, eax
        and    edi, 0xffff
        mov    eax, edi
        out    dx, ax
        mov    dx, word ptr [esi + 0x231]
        in     ax, dx
        cmp    ax, 8
        jne    loc_1_4CCB
        mov    dx, word ptr [esi + 0x23d]
        mov    eax, ebx
        and    eax, 0xffff
        inc    dword ptr [esi + 0x24f]
        out    dx, ax
        mov    dx, word ptr [esi + 0x233]
        in     ax, dx
        mov    word ptr [esi + 0x212], ax
        in     ax, dx
        mov    bx, ax
        shr    bx, 8
        mov    dx, word ptr [esi + 0x231]
        in     ax, dx
        mov    ecx, eax
        mov    ax, word ptr [esi + 0x247]
        and    ecx, 0xffff
        out    dx, ax
        mov    edx, ebx
        mov    eax, dword ptr [esi + 0x20e]
        and    edx, 0xffff
        mov    edx, dword ptr [eax + edx*4]
        test   byte ptr [edx + 8], 0x80
        je     loc_1_4BC9
        mov    al, byte ptr [edx + 0xe]
        and    eax, 0x1f
        add    eax, 0xe
        jmp    loc_1_4BCE
loc_1_4CCB:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4CDC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 0x33], 0
        je     loc_1_4DC5
        mov    edi, dword ptr [esi + 0x61]
        mov    eax, esi
        mov    byte ptr [esi + 0x3d], 1
        mov    byte ptr [esi + 0x30], 1
        mov    dword ptr [esi + 0x39], 5
        sub    eax, edi
        cmp    eax, -0x61
        je     loc_1_4E19
loc_1_4D12:
        cmp    byte ptr [edi + 0x1c], 0
        mov    ebx, edi
        je     loc_1_4D45
        mov    byte ptr [edi + 0x1c], 0
        mov    word ptr [esi + 0x33], 0
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [ebx + 4]
        push   0
        push   0x96
        push   ebx
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [ecx + 4], eax
        call   sub_1_58DC
        dec    dword ptr [esi + 0x5d]
        jmp    loc_1_4DAD
loc_1_4D45:
        xor    eax, eax
        push   4
        mov    ax, word ptr [esi + 0x33]
        or     eax, 0xc0011000
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [ebx + 0x14]
        mov    word ptr [esi + 0x33], 0
        push   0x40010007
        mov    ecx, dword ptr [ebx + 0x10]
        push   ecx
        call   sub_1_583E
        mov    ecx, dword ptr [ebx + 0x10]
        push   ecx
        call   sub_1_5844
        dec    dword ptr [ebx + 0x14]
        cmp    byte ptr [esi + 0x22b], 0
        je     loc_1_4DAD
        mov    eax, dword ptr [esi + 0x227]
        push   0xc0000001
        mov    byte ptr [esi + 0x22b], 0
        mov    byte ptr [esi + 0x3d], 0
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [esi + 0x227]
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        call   sub_1_5826
loc_1_4DAD:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        jne    loc_1_4D12
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4DC5:
        xor    eax, eax
        mov    byte ptr [esi + 0x3d], al
        mov    byte ptr [esi + 0x30], al
        cmp    byte ptr [esi + 0x22b], al
        jne    loc_1_4E13
        mov    edi, dword ptr [esi + 0x61]
        mov    eax, esi
        mov    dword ptr [esi + 0x39], 1
        sub    eax, edi
        cmp    eax, -0x61
        je     loc_1_4E19
loc_1_4DE8:
        cmp    byte ptr [edi + 0x1c], 0
        je     loc_1_4DFF
        mov    byte ptr [edi + 0x1c], 0
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   0
        push   eax
        call   sub_1_580E
loc_1_4DFF:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        jne    loc_1_4DE8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4E13:
        push   esi
        call   sub_1_517C
loc_1_4E19:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4E2C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        xor    eax, eax
        push   ebx
        mov    cx, word ptr [ebp + 0xc]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        test   cl, 0x20
        mov    dx, word ptr [esi + 0x231]
        out    dx, ax
        je     loc_1_4E5A
        mov    dword ptr [ebp - 4], 0x40
loc_1_4E5A:
        test   cl, 0x40
        je     loc_1_4E66
        or     dword ptr [ebp - 4], 0x80
loc_1_4E66:
        test   ch, 8
        je     loc_1_4E72
        or     dword ptr [ebp - 4], 0x800
loc_1_4E72:
        test   cl, 0x80
        je     loc_1_4E7E
        or     dword ptr [ebp - 4], 0x100
loc_1_4E7E:
        test   ch, 1
        je     loc_1_4E8A
        or     dword ptr [ebp - 4], 0x200
loc_1_4E8A:
        test   ch, 4
        je     loc_1_4E96
        or     dword ptr [ebp - 4], 0x400
loc_1_4E96:
        test   ch, 0x10
        je     loc_1_4EA2
        or     dword ptr [ebp - 4], 0x1000
loc_1_4EA2:
        test   ch, 0x20
        je     loc_1_4EAE
        or     dword ptr [ebp - 4], 0x2000
loc_1_4EAE:
        test   ch, 0x40
        je     loc_1_4EBA
        or     dword ptr [ebp - 4], 0x4000
loc_1_4EBA:
        test   ch, 0x80
        je     loc_1_4EC6
        or     dword ptr [ebp - 4], 0x8000
loc_1_4EC6:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_4F0D
        mov    edi, dword ptr [esi + 0x61]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        je     loc_1_4F07
loc_1_4ED8:
        lea    ebx, [edi + 0x14]
        push   4
        lea    eax, [ebp - 4]
        inc    dword ptr [ebx]
        push   eax
        mov    ecx, dword ptr [edi + 0x10]
        push   0x40010006
        push   ecx
        call   sub_1_583E
        mov    ecx, dword ptr [edi + 0x10]
        push   ecx
        call   sub_1_5844
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        dec    dword ptr [ebx]
        cmp    eax, -0x61
        jne    loc_1_4ED8
loc_1_4F07:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x35], eax
loc_1_4F0D:
        test   byte ptr [ebp - 3], 1
        je     loc_1_4F19
        push   esi
        call   sub_1_2F48
loc_1_4F19:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4F2C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x30], 1
        jne    loc_1_4F44
        xor    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F44:
        mov    eax, dword ptr [esi + 0x1f3]
        cmp    dword ptr [esi + 0x1ef], eax
        jne    loc_1_4F60
        cmp    word ptr [esi + 0x200], 0
        jne    loc_1_4FF5
loc_1_4F60:
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        je     loc_1_4FF5
        mov    ax, word ptr [eax + 4]
        add    ax, 0xc
        mov    dx, word ptr [esi + 0x23d]
        and    eax, 0xffff
        out    dx, ax
        mov    dx, word ptr [esi + 0x231]
        in     ax, dx
        cmp    ax, 8
        jne    loc_1_4FED
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [ecx + 0x28]
        dec    dword ptr [edx + 0x14]
        inc    word ptr [esi + 0x200]
        mov    eax, dword ptr [esi + 0x1f3]
        add    eax, 0x1a
        inc    dword ptr [esi + 0x24b]
        cmp    byte ptr [eax], 0
        je     loc_1_4FBF
        mov    byte ptr [eax], 0
        dec    word ptr [esi + 0x1eb]
loc_1_4FBF:
        mov    eax, dword ptr [esi + 0x1f3]
        push   0
        push   ecx
        mov    bl, 1
        mov    dword ptr [eax + 0x10], 0
        mov    ecx, dword ptr [edx + 0x10]
        push   ecx
        call   sub_1_581A
        mov    ecx, dword ptr [esi + 0x1f3]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [esi + 0x1f3], edx
        jmp    loc_1_4F44
loc_1_4FED:
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_4FF5:
        cmp    byte ptr [esi + 0x214], 0
        je     loc_1_5016
        cmp    byte ptr [esi + 0x1ff], 0
        je     loc_1_5016
        mov    byte ptr [esi + 0x1ff], 0
        push   1
        push   esi
        call   sub_1_37CF
loc_1_5016:
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_502C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 0x5d]
        cmp    byte ptr [esi + 0x22b], 0
        jne    loc_1_5123
        mov    edi, dword ptr [esi + 0x71]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0x71
        je     loc_1_5123
        push   0
        inc    dword ptr [edi + 0x14]
        mov    eax, dword ptr [edi + 0xc]
        mov    ecx, dword ptr [esi + 0x85]
        push   eax
        push   ecx
        call   sub_2_0AF0
loc_1_5069:
        test   eax, eax
        jne    loc_1_50D5
        cmp    dword ptr [edi + 0x14], 2
        jne    loc_1_50AD
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x264], ecx
        jne    loc_1_5093
        push   esi
        call   sub_1_0F8E
loc_1_5093:
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_5814
        push   0
        push   0x96
        push   edi
        call   sub_1_58DC
        jmp    loc_1_5123
loc_1_50AD:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        lea    edx, [esi + 0x69]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi + 0x6d]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 0x6d]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x6d], edi
        sub    dword ptr [edi + 0x14], 2
        jmp    loc_1_5123
loc_1_50D5:
        cmp    eax, 0x103
        jne    loc_1_5108
        mov    byte ptr [edi + 0x1d], 1
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        lea    edx, [esi + 0x69]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi + 0x6d]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 0x6d]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 0x6d], edi
        sub    dword ptr [edi + 0x14], 2
        jmp    loc_1_5123
loc_1_5108:
        push   eax
        push   0xa
        push   3
        mov    eax, dword ptr [esi + 0x81]
        push   3
        push   0xaa000006
        push   eax
        call   sub_1_59A8
        add    esp, 0x18
loc_1_5123:
        mov    edi, dword ptr [esi + 0x69]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0x69
        je     loc_1_516D
        cmp    dword ptr [edi + 0x14], 0
        jne    loc_1_516D
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [esi + 0x264], eax
        jne    loc_1_5146
        push   esi
        call   sub_1_0F8E
loc_1_5146:
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_5814
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        push   0
        push   0x96
        push   edi
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 4], edx
        call   sub_1_58DC
loc_1_516D:
        pop    edi
        dec    dword ptr [esi + 0x5d]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_517C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    dword ptr [esi + 0x9d], 0
        call   sub_1_30AF
        test   eax, eax
        jne    loc_1_51A7
        push   esi
        call   sub_1_31A1
        test   eax, eax
        jne    loc_1_51A7
        push   esi
        call   sub_1_51AC
loc_1_51A7:
        pop    esi
        pop    ebp
        ret    4
sub_1_51AC:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x61]
        mov    byte ptr [esi + 0x22b], al
        mov    byte ptr [esi + 0x3d], al
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        je     loc_1_51F1
loc_1_51CB:
        push   0
        mov    eax, dword ptr [edi + 0x10]
        push   0
        push   0x40010005
        push   eax
        call   sub_1_583E
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_1_5844
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0x61
        jne    loc_1_51CB
loc_1_51F1:
        mov    eax, dword ptr [esi + 0x227]
        push   0
        dec    dword ptr [eax + 0x14]
        mov    eax, dword ptr [esi + 0x227]
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        call   sub_1_5826
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 0x214], 0
        je     loc_1_528B
        cmp    byte ptr [esi + 0x1ff], 0
        je     loc_1_528B
        mov    ax, word ptr [esi + 0x200]
        cmp    word ptr [esi + 0xa7], ax
        je     loc_1_5269
        cmp    byte ptr [esi + 0x5a], 0
        jne    loc_1_5269
        mov    byte ptr [esi + 0x5a], 1
        mov    byte ptr [esi + 0x1ff], 0
        push   1
        push   esi
        call   sub_1_4F2C
        mov    byte ptr [esi + 0x1ff], 1
        mov    byte ptr [esi + 0x5a], 0
loc_1_5269:
        cmp    byte ptr [esi + 0x59], 0
        jne    loc_1_528B
        mov    byte ptr [esi + 0x59], 1
        mov    byte ptr [esi + 0x1ff], 0
        push   esi
        call   sub_1_4BAC
        mov    byte ptr [esi + 0x1ff], 1
        mov    byte ptr [esi + 0x59], 0
loc_1_528B:
        push   0x1388
        add    esi, 0x1f
        push   esi
        call   sub_1_58B2
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_52AC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 0x23b]
        in     al, dx
        mov    bl, al
        or     al, 8
        out    dx, al
        mov    dx, word ptr [esi + 0x233]
        in     ax, dx
        mov    cx, ax
        test   cx, cx
        je     loc_1_5336
        mov    dx, word ptr [esi + 0x235]
        in     ax, dx
        mov    di, ax
        mov    dx, word ptr [esi + 0x237]
        in     ax, dx
        mov    edx, eax
        mov    eax, edi
        and    edx, 0xffff
        and    eax, 0xffff
        shl    edx, 0x10
        or     edx, eax
        mov    eax, ecx
        test   bl, 4
        je     loc_1_5320
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x231]
        push   eax
        push   edx
        push   ecx
        call   sub_1_57AD
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_5320:
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x231]
        push   eax
        push   edx
        push   ecx
        call   sub_1_5770
loc_1_5336:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
sub_1_5350:
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
        call   sub_1_5380
        pop    ebp
        ret    0x1c
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_5380:
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
        jb     loc_1_54A0
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     loc_1_5444
        mov    al, byte ptr [ecx]
        cmp    al, 0xff
        jne    loc_1_53C2
        cmp    byte ptr [ecx + 1], 0xff
        je     loc_1_53CC
loc_1_53C2:
        cmp    al, 0xc0
        jne    loc_1_53E8
        cmp    byte ptr [ecx + 1], 0
        jne    loc_1_53E8
loc_1_53CC:
        cmp    byte ptr [ecx + 2], 0xff
        jne    loc_1_53E8
        cmp    byte ptr [ecx + 3], 0xff
        jne    loc_1_53E8
        cmp    byte ptr [ecx + 4], 0xff
        jne    loc_1_53E8
        cmp    byte ptr [ecx + 5], 0xff
        mov    byte ptr [ebp - 2], 1
        je     loc_1_53EC
loc_1_53E8:
        mov    byte ptr [ebp - 2], 0
loc_1_53EC:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_5400
        mov    dword ptr [ebp - 8], 8
        jmp    loc_1_54AB
        db  08Bh, 0FFh
loc_1_5400:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 0x80
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 0x1000
        jne    loc_1_541C
        mov    dword ptr [ebp - 8], 0x4000
loc_1_541C:
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
        jmp    loc_1_54AB
loc_1_5444:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x10], 0x30
        je     loc_1_5490
        mov    edx, dword ptr [esi + 0x1a]
        lea    eax, [esi + 0x18]
        cmp    dword ptr [ecx + 2], edx
        jne    loc_1_5467
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 0x10], 0
        je     loc_1_546E
loc_1_5467:
        mov    dword ptr [ebp - 0x10], 1
loc_1_546E:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_5480
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_54AE
        db  08Dh, 049h, 000h
loc_1_5480:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_54AE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5490:
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_54AE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_54A0:
        mov    dword ptr [ebp - 8], 0x20
        mov    byte ptr [ebp - 1], 0
loc_1_54AB:
        mov    esi, dword ptr [ebp + 8]
loc_1_54AE:
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_5591
loc_1_54B9:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    loc_1_54FD
        test   ah, 0x40
        je     loc_1_54D2
        mov    eax, dword ptr [edi + 0xc]
        test   dword ptr [ebp - 0xc], eax
        jne    loc_1_54FD
loc_1_54D2:
        test   byte ptr [ebp - 7], 0x40
        je     loc_1_54DD
        test   ch, 0x20
        jne    loc_1_54FD
loc_1_54DD:
        test   byte ptr [ebp - 7], 0x10
        je     loc_1_54E9
        cmp    byte ptr [edi + 0x1d], 0
        jne    loc_1_54FD
loc_1_54E9:
        test   cl, 0x10
        je     loc_1_54F4
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_54FD
loc_1_54F4:
        test   cl, 0x20
        je     loc_1_5586
loc_1_54FD:
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
        call   sub_1_5832
        mov    byte ptr [edi + 0x1e], 1
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_5586
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_553F
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_553F:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_5550
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_5556
        db  08Bh, 0FFh
loc_1_5550:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_5556:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_5565
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5838
loc_1_5565:
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
        call   sub_1_58DC
        mov    edi, ebx
        jmp    loc_1_5589
loc_1_5586:
        mov    edi, dword ptr [edi + 0x10]
loc_1_5589:
        or     edi, edi
        jne    loc_1_54B9
loc_1_5591:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_55A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_55B0
        pop    ebp
        ret    4
sub_1_55B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x14]
        or     edi, edi
        je     loc_1_562E
loc_1_55C0:
        inc    dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_1_55D6
        mov    byte ptr [edi + 0x1e], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5838
loc_1_55D6:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_1_5627
        mov    ebx, dword ptr [edi + 0x10]
        or     ebx, ebx
        je     loc_1_55EC
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 0x14], eax
loc_1_55EC:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_1_5600
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_1_5606
        db  005h, 000h, 000h, 000h, 000h
loc_1_5600:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_1_5606:
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
        call   sub_1_58DC
        mov    edi, ebx
        jmp    loc_1_562A
loc_1_5627:
        mov    edi, dword ptr [edi + 0x10]
loc_1_562A:
        or     edi, edi
        jne    loc_1_55C0
loc_1_562E:
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
        je     loc_1_5692
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_5692
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_5692
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_5692:
        mov    esi, dword ptr [eax + 0x10]
        or     esi, esi
        jne    loc_1_56A2
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_56A2:
        test   esi, 0x20
        je     loc_1_56B3
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_56B3:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     loc_1_56D0
        test   esi, 0x10
        je     loc_1_56D0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_56D0:
        mov    eax, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [eax]
        and    cl, 0x80
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     loc_1_575A
        mov    cl, byte ptr [eax]
        cmp    cl, 0xff
        jne    loc_1_56EC
        cmp    byte ptr [eax + 1], 0xff
        je     loc_1_56F7
loc_1_56EC:
        cmp    cl, 0xc0
        jne    loc_1_5713
        cmp    byte ptr [eax + 1], 0
        jne    loc_1_5713
loc_1_56F7:
        cmp    byte ptr [eax + 2], 0xff
        jne    loc_1_5713
        cmp    byte ptr [eax + 3], 0xff
        jne    loc_1_5713
        cmp    byte ptr [eax + 4], 0xff
        jne    loc_1_5713
        cmp    byte ptr [eax + 5], 0xff
        mov    byte ptr [ebp - 1], 1
        je     loc_1_5717
loc_1_5713:
        mov    byte ptr [ebp - 1], 0
loc_1_5717:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_5740
        test   esi, 8
        je     loc_1_5730
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_1_5730:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5740:
        test   esi, 0x6000
        je     loc_1_5751
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_5751:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_575A:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5770:
        push   esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep outsb dx, byte ptr [esi]
        pop    esi
        ret    0xc
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h
sub_1_57AD:
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 8]
        cld    
        rep insb byte ptr es:[edi], dx
        pop    edi
        ret    0xc
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_57EA:
        VxDCall  VPOWERD.Service_8031h           ; cd2031802800
sub_1_57F0:
        VxDCall  VPOWERD.Service_8032h           ; cd2032802800
sub_1_57F6:
        VxDCall  VPOWERD.Service_8033h           ; cd2033802800
sub_1_57FC:
        VxDCall  VPOWERD.Service_8034h           ; cd2034802800
sub_1_5802:
        VxDCall  VPOWERD.Service_8035h           ; cd2035802800
sub_1_5808:
        VxDCall  VPOWERD.Service_8036h           ; cd2036802800
sub_1_580E:
        VxDCall  VPOWERD.Service_8037h           ; cd2037802800
sub_1_5814:
        VxDCall  VPOWERD.Service_8038h           ; cd2038802800
sub_1_581A:
        VxDCall  VPOWERD.Service_8039h           ; cd2039802800
        VxDCall  VPOWERD.Service_803Ah           ; cd203a802800
sub_1_5826:
        VxDCall  VPOWERD.Service_803Bh           ; cd203b802800
sub_1_582C:
        VxDCall  VPOWERD.Service_803Ch           ; cd203c802800
sub_1_5832:
        VxDCall  VPOWERD.Service_803Dh           ; cd203d802800
sub_1_5838:
        VxDCall  VPOWERD.Service_803Eh           ; cd203e802800
sub_1_583E:
        VxDCall  VPOWERD.Service_803Fh           ; cd203f802800
sub_1_5844:
        VxDCall  VPOWERD.Service_8040h           ; cd2040802800
sub_1_584A:
        VxDCall  VPOWERD.Service_8041h           ; cd2041802800
sub_1_5850:
        VxDCall  VPOWERD._VPOWERD_OEM_APM_Function ; cd2009802800
sub_1_5856:
        VxDCall  VPOWERD._VPOWERD_Get_Power_State ; cd2008802800
sub_1_585C:
        VxDCall  VPOWERD.Service_8044h           ; cd2044802800
sub_1_5862:
        VxDCall  VPOWERD.Service_8048h           ; cd2048802800
sub_1_5868:
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
sub_1_58B2:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_58B8:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
sub_1_58BE:
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
sub_1_58C4:
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
sub_1_58CA:
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
sub_1_58D0:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_58D6:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_58DC:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
sub_1_5900:
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
sub_1_5906:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_590C:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
loc_1_5912:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
sub_1_591E:
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
sub_1_5924:
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
sub_1_5948:
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
sub_1_5954:
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
sub_1_595A:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_5960:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
sub_1_596C:
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
sub_1_5972:
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
sub_1_5978:
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
sub_1_597E:
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
sub_1_59A8:
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_59BC:
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
        db  'IRMATRAC'                  ; DDB_Name
        dd  054004000h                  ; DDB_Init_Order
        dd  IRMATRAC_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 049h, 052h, 04Dh, 041h, 054h, 052h, 041h, 043h, 000h, 000h, 000h, 000h
loc_1_5A80:
        dec    ecx
        push   edx
        dec    ebp
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        push   ebp
        push   edx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x55], al
        push   edx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x55], al
        push   ebx
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [edx + 0x55], al
        push   ebx
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [ecx + 0x4f], cl
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
        add    byte ptr [ebp + ecx*2 + 0x41], al
        inc    ebx
        dec    eax
        inc    ecx
        dec    esi
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
loc_1_5AE8:
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        dec    eax
        inc    ecx
        dec    esi
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        inc    esp
        inc    ecx
        push   esp
        inc    ecx
        push   edx
        inc    ecx
        push   esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5B00:
        inc    esp
        inc    ecx
        push   esp
        inc    ecx
        push   edx
        inc    ecx
        push   esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebp
        inc    ecx
        push   edx
        dec    esp
        pop    ecx
        push   edx
        inc    ebp
        dec    esp
        inc    ebp
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5B1C:
        inc    ebp
        inc    ecx
        push   edx
        dec    esp
        pop    ecx
        push   edx
        inc    ebp
        dec    esp
        inc    ebp
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebx
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x41], al
        inc    edx
        dec    esp
        inc    ebp
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax*2 + 0x54], al
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        inc    esi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5B54:
        inc    esp
        inc    ecx
        push   esp
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        inc    esi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    edx, dword ptr [edx + 0x45]
        inc    ebx
        push   esi
        inc    edx
        push   ebp
        inc    esi
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x44], cl
        dec    ecx
        push   ebx
        xor    edx, dword ptr [edx + 0x45]
        inc    ebx
        push   esi
        inc    edx
        push   ebp
        inc    esi
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x44], cl
        dec    ecx
        push   ebx
        xor    ecx, dword ptr [ebp + 0x41]
        pop    eax
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        dec    ebp
        dec    ecx
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x44], cl
        dec    ecx
        push   ebx
        xor    ecx, dword ptr [ebp + 0x41]
        pop    eax
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        dec    ebp
        dec    ecx
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x44], cl
        dec    ecx
        push   ebx
        xor    ecx, dword ptr [ebp + 0x41]
        pop    eax
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        dec    esp
        inc    ebp
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5BC8:
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        xor    ecx, dword ptr [ebp + 0x41]
        pop    eax
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        dec    esp
        inc    ebp
        dec    esi
        add    byte ptr [eax], al
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
loc_1_5BE8:
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
        dec    ecx
        push   edx
        dec    ebp
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        inc    ebx
        and    byte ptr [esi + 0x44], cl
        dec    ecx
        push   ebx
        and    byte ptr [ebx], dh
        pop    eax
        and    byte ptr [eax], al
        add    byte ptr [ecx + 0x52], cl
        dec    ebp
        inc    ecx
        push   esp
        push   edx
        inc    ecx
        inc    ebx
        and    byte ptr [esi + 0x44], cl
        dec    ecx
        push   ebx
        and    byte ptr [ebx], dh
        pop    eax
        and    byte ptr [eax], al
        add    byte ptr [edx], al
        add    ecx, dword ptr [edx]
        or     eax, dword ptr [esi + eax]
        add    eax, 7
        add    dword ptr [edx + eax], eax
        add    eax, 0x6000003
        pop    es
loc_1_5C30:
        inc    esp
        inc    ebx
        inc    ecx
        dec    ebp
        inc    ecx
        inc    ebx
        inc    edx
        dec    ecx
        dec    esi
        add    byte ptr [eax], al
loc_1_5C3C:
        or     eax, dword ptr [eax]
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5C44:
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
        add    eax, 0x6020101
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx + eax]
        add    al, byte ptr [edx]
        add    eax, 0x6020202
        add    al, byte ptr [edx]
        add    al, byte ptr [edi]
        add    al, byte ptr [edx]
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
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    eax, 0x6020101
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    cl, byte ptr [ecx + 0x72]
        insd   dword ptr es:[edi], dx
        popal  
        je     0x5dc4
        popal  
        arpl   word ptr [eax], sp
        push   esp
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x74654e20
        ja     0x5dd4
        jb     0x5dd2
        and    byte ptr [ebx + 0x61], al
        jb     0x5dd0
        and    byte ptr [esi + 0x6f], ah
        jb     0x5d91
        push   eax
        inc    ebx
        and    byte ptr [ecx + 0x2f], cl
        dec    edi
        and    byte ptr [edx + 0x75], ah
        jae    0x5daa
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5D80:
        dec    ecx
        jb     0x5df0
        popal  
        je     0x5df8
        popal  
        arpl   word ptr [eax], sp
        push   esp
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebp + 0x6e], 0x20
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x74654e20
        ja     0x5e08
        jb     0x5e06
        and    byte ptr [ebx + 0x61], al
        jb     0x5e04
        and    byte ptr [esi + 0x6f], ah
        jb     0x5dc5
        dec    ebp
        inc    ebx
        inc    ecx
        and    byte ptr [edx + 0x75], ah
        jae    0x5ddb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5DB1:
        add    byte ptr [eax], al
loc_1_5DB3:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xe90  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x5c
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x1c], 9
        mov    word ptr [ebp - 0x1a], 9
        mov    dword ptr [ebp - 0x18], 0     ; -> loc_1_5A80
        mov    word ptr [ebp - 0x14], 0
        mov    word ptr [ebp - 0x12], 2
        mov    word ptr [ebp - 0x10], 0x5c1c
        mov    word ptr [ebp - 0xe], 0x5c1d
        lea    eax, [ebp - 0x14]
        mov    dword ptr [ebp + 8], eax
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0x20]
        push   eax
        call   sub_1_57EA
loc_2_004D:
        mov    dword ptr [ebp - 0x5c], 0xffffffff
        mov    eax, dword ptr [ebp - 0x5c]
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_58D6
loc_2_0065:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_007C
        mov    eax, 0xc000009a
        jmp    loc_2_0124
loc_2_007C:
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 0x58], 3
        mov    byte ptr [ebp - 0x57], 0
        mov    dword ptr [ebp - 0x52], 0     ; -> loc_1_0D28
        mov    dword ptr [ebp - 0x4e], 0     ; -> loc_1_1005
        mov    dword ptr [ebp - 0x4a], 0     ; -> loc_1_3554
        mov    dword ptr [ebp - 0x46], 0     ; -> loc_1_3B7C
        mov    dword ptr [ebp - 0x42], 0     ; -> loc_1_1FF1
        mov    dword ptr [ebp - 0x3e], 0     ; -> loc_1_3F4C
        mov    dword ptr [ebp - 0x3a], 0     ; -> loc_1_47CC
        mov    dword ptr [ebp - 0x36], 0     ; -> loc_1_12D7
        mov    dword ptr [ebp - 0x32], 0     ; -> loc_1_1304
        mov    dword ptr [ebp - 0x2e], 0     ; -> loc_1_1E64
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 0x18]
        lea    edx, [ebp - 0x2a]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   0x36
        lea    eax, [ebp - 0x58]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_57F6
loc_2_00FE:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_010F
        xor    eax, eax
        jmp    loc_2_0124
loc_2_010F:
        push   0
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        call   sub_1_57F0
loc_2_011A:
        mov    eax, 0xc0000001
        jmp    loc_2_0124
loc_2_0124:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_2_012B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 0x2c]
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        test   eax, eax
        je     loc_2_0164
        mov    dword ptr [ebp - 0x1e], 1
        jmp    loc_2_016B
loc_2_0164:
        mov    dword ptr [ebp - 0x1e], 0
loc_2_016B:
        mov    dword ptr [ebp - 0x26], 0     ; -> loc_2_04A8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0x30
        cmp    al, 0x30
        jne    loc_2_0192
        mov    dword ptr [ebp - 0x22], 2
        jmp    loc_2_01BB
loc_2_0192:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0xff
        test   al, 0x10
        je     loc_2_01B4
        mov    dword ptr [ebp - 0x22], 3
        jmp    loc_2_01BB
loc_2_01B4:
        mov    dword ptr [ebp - 0x22], 1
loc_2_01BB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x243]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x2c], eax
        cmp    dword ptr [ebp - 0x2c], 0
        je     loc_2_0211
        cmp    dword ptr [ebp - 0x22], 2
        jne    loc_2_01EE
        mov    dword ptr [ebp - 0x2c], 0
        mov    dword ptr [ebp - 0x1e], 0
loc_2_01EE:
        mov    byte ptr [ebp - 0x28], 1
        mov    eax, dword ptr [ebp - 0x1e]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xa9], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x245]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x1a], eax
loc_2_0211:
        mov    dword ptr [ebp - 0x16], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x22f]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x12], eax
        mov    dword ptr [ebp - 0xe], 0x20
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x7d]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x81
        push   eax
        call   sub_1_5802
loc_2_0256:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_026B
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_04A1
loc_2_026B:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xab]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0xad], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0040
loc_2_0288:
        and    eax, 0xff
        test   eax, eax
        je     loc_2_0462
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2392
loc_2_029E:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x244]
        and    eax, 0x10
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 2
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x18]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x18]
        and    eax, 0xff
        push   eax
        push   0                             ; -> loc_1_4A8C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0                             ; -> loc_1_496C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_58BE
loc_2_02F2:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_0330
        push   1
        push   0
        push   2
        push   0xaa000005
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
loc_2_0316:
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_5808
loc_2_0328:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_2_04A1
loc_2_0330:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x3e], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x69
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x6d], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x6d]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x69], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xa7]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x200], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xaf]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x202], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x59], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x1ff], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x214], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0564
loc_2_0399:
        test   eax, eax
        je     loc_2_03CC
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_58C4
loc_2_03AA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0580
loc_2_03B3:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_5808
loc_2_03C2:
        mov    eax, 0xc0000001
        jmp    loc_2_04A1
loc_2_03CC:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x85
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x79
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x45
        push   eax
        push   0                             ; -> loc_1_29F4
        push   0                             ; -> loc_1_2408
        push   0                             ; -> loc_1_25EF
        push   0                             ; -> loc_1_251A
        call   sub_2_0920
        and    eax, 0xff
        test   eax, eax
        jne    loc_2_0456
        push   2
        push   0
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
loc_2_0423:
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_58C4
loc_2_042F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0580
loc_2_0438:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_5808
loc_2_0447:
        mov    eax, 0xc000009a
        jmp    loc_2_04A1
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_0456:
        xor    eax, eax
        jmp    loc_2_04A1
        db  0E9h, 03Fh, 000h, 000h, 000h
loc_2_0462:
        push   3
        push   0
        push   2
        push   0xaa000002
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_59A8
loc_2_047C:
        add    esp, 0x14
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_0580
loc_2_0488:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x81]
        push   eax
        call   sub_1_5808
loc_2_0497:
        mov    eax, 0xc000009a
        jmp    loc_2_04A1
loc_2_04A1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
loc_2_04A8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        xor    ecx, ecx
        push   ebx
        mov    dword ptr [ebp - 0xc], 0
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x81], eax
        mov    cx, word ptr [esi + 0x22f]
        lea    edx, [ecx + 0x11]
        in     al, dx
        cmp    al, 0x5c
        je     loc_2_04E4
        mov    eax, 0xaa000004
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_04E4:
        lea    edx, [ecx + 0x10]
        in     al, dx
        cmp    al, 0x1d
        jne    loc_2_056B
        or     byte ptr [esi + 0x244], 8
loc_2_04F3:
        xor    edx, edx
        mov    dx, word ptr [esi + 0x22f]
        add    edx, 0x13
        in     al, dx
        test   al, 0x80
        jne    loc_2_0623
        xor    edi, edi
        and    byte ptr [esi + 0x244], 0xcf
        mov    di, word ptr [esi + 0x22f]
        lea    edx, [edi + 0x12]
        in     al, dx
        mov    cl, al
        and    al, 0x41
        cmp    al, 0x41
        jne    loc_2_0529
        mov    al, cl
        and    al, 0xbf
        out    dx, al
loc_2_0529:
        and    cl, 0xfe
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0x13]
        in     al, dx
        test   al, 0x40
        jne    loc_2_053E
        mov    byte ptr [esi + 0x243], 0
loc_2_053E:
        mov    cl, byte ptr [esi + 0x244]
        mov    dl, byte ptr [esi + 0x18]
        and    cl, 8
        jne    loc_2_0592
        cmp    dl, 7
        ja     loc_2_0584
        cmp    dl, 2
        jb     loc_2_0584
        cmp    dl, 4
        je     loc_2_0584
        and    al, 0xfc
        or     al, 4
        cmp    dl, 3
        jbe    loc_2_057D
        shr    dl, 1
        or     al, dl
        jmp    loc_2_05C4
loc_2_056B:
        cmp    al, 0x1c
        je     loc_2_04F3
        mov    eax, 0xaa000004
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_057D:
        sub    dl, 2
        or     al, dl
        jmp    loc_2_05C4
loc_2_0584:
        mov    eax, 0xaa00000a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0592:
        cmp    dl, 0xb
        ja     loc_2_0770
        cmp    dl, 2
        jb     loc_2_0770
        cmp    dl, 9
        je     loc_2_0770
        cmp    dl, 8
        je     loc_2_0770
        and    al, 0xf8
        and    edx, 0xff
        or     al, byte ptr [edx]            ; -> obj1:0x5C24
loc_2_05C4:
        xor    edi, edi
        mov    bl, byte ptr [esi + 0x243]
        mov    di, word ptr [esi + 0x22f]
        test   bl, bl
        lea    edx, [edi + 0x13]
        out    dx, al
        je     loc_2_0600
        test   cl, cl
        jne    loc_2_073D
        cmp    bl, 5
        jb     loc_2_072F
        cmp    bl, 6
        ja     loc_2_072F
        lea    edx, [edi + 0x14]
        mov    al, bl
        shl    al, 6
        or     al, 0x1c
loc_2_05FF:
        out    dx, al
loc_2_0600:
        cmp    bl, 1
        sbb    al, al
        and    al, 0xc0
        add    al, 0x79
        test   byte ptr [esi + 0x244], 1
        jne    loc_2_0614
        or     al, 2
loc_2_0614:
        test   byte ptr [esi + 0x244], 4
        je     loc_2_061F
        or     al, 4
loc_2_061F:
        lea    edx, [edi + 0x12]
        out    dx, al
loc_2_0623:
        mov    ax, word ptr [esi + 0x22f]
        mov    word ptr [esi + 0x231], ax
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 2
        mov    word ptr [esi + 0x233], ax
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 4
        mov    word ptr [esi + 0x235], ax
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 6
        mov    word ptr [esi + 0x237], ax
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 8
        mov    word ptr [esi + 0x239], ax
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 9
        mov    word ptr [esi + 0x23b], ax
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 0xa
        mov    word ptr [esi + 0x23d], ax
        push   esi
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 0xc
        mov    word ptr [esi + 0x23f], ax
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 0xe
        mov    word ptr [esi + 0x241], ax
        call   sub_2_08C8
        test   al, al
        jne    loc_2_077E
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 4
        mov    word ptr [esi + 0x233], ax
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 2
        mov    word ptr [esi + 0x235], ax
        push   esi
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 0xc
        mov    word ptr [esi + 0x23d], ax
        mov    ax, word ptr [esi + 0x22f]
        add    ax, 0xa
        mov    word ptr [esi + 0x23f], ax
        call   sub_2_08C8
        test   al, al
        jne    loc_2_077E
        mov    eax, 0xaa000004
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_072F:
        mov    eax, 0xaa00000a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_073D:
        cmp    bl, 3
        jb     loc_2_0762
        cmp    bl, 7
        ja     loc_2_0762
        cmp    bl, 4
        je     loc_2_0762
        mov    al, bl
        and    al, 3
        shl    al, 4
        cmp    bl, 3
        jne    loc_2_075A
        xor    al, al
loc_2_075A:
        lea    edx, [edi + 0x14]
        jmp    loc_2_05FF
loc_2_0762:
        mov    eax, 0xaa00000a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_0770:
        mov    eax, 0xaa00000a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_077E:
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        lea    ecx, [ebp - 0x14]
        push   eax
        lea    edx, [ebp - 0x10]
        push   0                             ; -> loc_1_5C3C
        lea    eax, [ebp - 0xc]
        push   ecx
        push   edx
        push   eax
        call   sub_1_596C
loc_2_079A:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_07AE
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_07AE:
        mov    eax, dword ptr [ebp - 0x10]
        lea    ecx, [ebp - 8]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_5972
loc_2_07BF:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_07D3
        mov    eax, 0xc0000001
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_07D3:
        mov    dx, word ptr [esi + 0x239]
        mov    al, 0x80
        out    dx, al
        push   0xa
        call   sub_1_58D0
loc_2_07E4:
        mov    cx, word ptr [esi + 0x239]
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    word ptr [ebp - 2], 0x7f
        cmp    byte ptr [esi + 0x243], 0
        jne    loc_2_0806
        or     byte ptr [ebp - 1], 1
loc_2_0806:
        mov    dx, cx
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
        mov    eax, 0x80
        out    dx, ax
        mov    eax, 0x6a
        out    dx, ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        add    dword ptr [ebp - 8], 2
        test   ax, ax
        je     loc_2_0888
        mov    edi, eax
        mov    ecx, 2
        and    edi, 0xffff
loc_2_0839:
        mov    eax, dword ptr [ebp - 8]
        mov    bx, word ptr [eax]
        shr    bx, 1
        add    dword ptr [ebp - 8], ecx
        mov    dx, word ptr [esi + 0x23f]
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        out    dx, ax
        add    dword ptr [ebp - 8], ecx
        mov    dx, word ptr [esi + 0x235]
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        out    dx, ax
        add    dword ptr [ebp - 8], ecx
        test   bx, bx
        je     loc_2_0885
loc_2_086F:
        mov    dx, word ptr [esi + 0x233]
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        out    dx, ax
        add    dword ptr [ebp - 8], ecx
        dec    bx
        jne    loc_2_086F
loc_2_0885:
        dec    edi
        jne    loc_2_0839
loc_2_0888:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_5978
loc_2_0891:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_597E
loc_2_089A:
        mov    cx, word ptr [ebp - 2]
        pop    edi
        and    cx, 0xff2a
        mov    dx, word ptr [esi + 0x239]
        or     cl, 0x2a
        mov    ax, cx
        pop    esi
        out    dx, ax
        mov    eax, dword ptr [ebp - 0xc]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_08C8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        mov    si, word ptr [ecx + 0x235]
        mov    cx, word ptr [ecx + 0x23d]
        inc    esi
        mov    edx, esi
        and    ecx, 0xffff
        in     al, dx
        mov    bl, al
        inc    ecx
        mov    edx, ecx
        in     al, dx
        cmp    al, bl
        jne    loc_2_090C
        mov    edx, esi
        mov    al, 0x80
        out    dx, al
        in     al, dx
        mov    bl, al
        mov    edx, ecx
        in     al, dx
        cmp    al, bl
        jne    loc_2_090C
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_090C:
        xor    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0920:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [0x5db4]       ; -> obj1:0x5DB4
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   0x32
        push   eax
        call   sub_1_58D6
        mov    ecx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     loc_2_0950
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    0x1c
        db  090h
loc_2_0950:
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
sub_2_09F0:
        push   ebp
        mov    ebp, esp
        push   0
        push   0x32
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_58DC
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x2e], 0
        jne    loc_2_0A29
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0A29:
        mov    eax, dword ptr [0x5db4]       ; -> obj1:0x5DB4
        push   eax
        push   0
        push   0x1f
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_58D6
        or     eax, eax
        je     loc_2_0A49
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_2_0A49:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 0x2e]
        mov    edi, edi
loc_2_0A50:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    loc_2_0A63
        inc    ecx
        cmp    ecx, 0x20
        jb     loc_2_0A50
        jmp    loc_2_0A72
loc_2_0A63:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 0x2e], eax
loc_2_0A72:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [eax + 0x10], ecx
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_2_0A88
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x14], ecx
loc_2_0A88:
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
sub_2_0AF0:
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
        call   sub_2_0E10
        mov    ebx, eax
        or     ebx, ebx
        je     loc_2_0B19
        cmp    ebx, 0x103
        jne    loc_2_0B31
loc_2_0B19:
        push   0
        push   0x5db0                        ; -> obj1:0x5DB0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0BE0
        or     eax, eax
        je     loc_2_0B31
        mov    ebx, eax
loc_2_0B31:
        or     ebx, ebx
        je     loc_2_0B3D
        cmp    ebx, 0x103
        jne    loc_2_0B68
loc_2_0B3D:
        cmp    byte ptr [edi + 0x1d], 0
        je     loc_2_0B68
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [edi + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_2_0B68
        push   0
        push   0x5db0                        ; -> obj1:0x5DB0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        push   esi
        call   sub_2_0C80
        or     eax, eax
        je     loc_2_0B68
        mov    ebx, eax
loc_2_0B68:
        or     ebx, ebx
        je     loc_2_0B74
        cmp    ebx, 0x103
        jne    loc_2_0BD5
loc_2_0B74:
        mov    eax, dword ptr [edi + 0x18]
        dec    eax
        mov    dword ptr [edi + 0x18], eax
        jne    loc_2_0BD0
        mov    eax, dword ptr [edi + 0x10]
        or     eax, eax
        je     loc_2_0B8A
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x14], ecx
loc_2_0B8A:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_2_0B99
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [eax + 0x10], ecx
        jmp    loc_2_0B9F
loc_2_0B99:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x14], eax
loc_2_0B9F:
        cmp    byte ptr [edi + 0x1e], 0
        je     loc_2_0BAE
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5838
loc_2_0BAE:
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x1c]
        shl    eax, cl
        or     dword ptr [esi + 0x2e], eax
        push   0
        push   0x1f
        push   edi
        call   sub_1_58DC
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
loc_2_0BD0:
        mov    ebx, 0xc001000e
loc_2_0BD5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_2_0BE0:
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
        je     loc_2_0C40
loc_2_0C33:
        mov    ecx, dword ptr [eax + 0xc]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0C33
loc_2_0C40:
        mov    ecx, dword ptr [edi + 4]
        cmp    esi, ecx
        je     loc_2_0C74
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
        je     loc_2_0C76
        cmp    eax, 0x103
        je     loc_2_0C76
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0xc], ecx
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0C74:
        sub    eax, eax
loc_2_0C76:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
sub_2_0C80:
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
        mov    al, byte ptr [0x5db0]         ; -> obj1:0x5DB0
        cmp    byte ptr [ecx], al
        jne    loc_2_0D28
        mov    al, byte ptr [0x5db1]         ; -> loc_1_5DB1
        cmp    byte ptr [ecx + 1], al
        jne    loc_2_0D28
        mov    al, byte ptr [0x5db2]         ; -> obj1:0x5DB2
        cmp    byte ptr [ecx + 2], al
        jne    loc_2_0D28
        mov    al, byte ptr [0x5db3]         ; -> loc_1_5DB3
        cmp    byte ptr [ecx + 3], al
        jne    loc_2_0D28
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_0D09
        dec    dword ptr [esi + 0xc]
        mov    byte ptr [ebx + 0x1d], 0
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_0DA3
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0D09:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_0D1D
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0D1D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0D28:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    loc_2_0D60
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_0D44
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0D44:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_2_0DA3
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
loc_2_0D60:
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 1
        jbe    loc_2_0D76
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0D76:
        mov    ebx, dword ptr [ebp + 0xc]
        jne    loc_2_0D90
        cmp    byte ptr [ebx + 0x1d], 0
        jne    loc_2_0D90
        mov    eax, 0xc001001a
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  090h
loc_2_0D90:
        cmp    eax, 1
        jne    loc_2_0DA3
        cmp    byte ptr [ebx + 0x1d], 0
        je     loc_2_0DA3
        dec    eax
        mov    dword ptr [esi + 0xc], eax
        mov    byte ptr [ebx + 0x1d], 0
loc_2_0DA3:
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
        je     loc_2_0DD6
        cmp    eax, 0x103
        je     loc_2_0DD6
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0xc], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0DD6:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_0DF0
        mov    byte ptr [ebx + 0x1d], 0
        mov    dword ptr [esi + 0xc], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0DF0:
        mov    byte ptr [ebx + 0x1d], 1
        mov    dword ptr [esi + 0xc], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0E10:
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
        je     loc_2_0E4D
        lea    esp, [esp]
loc_2_0E40:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0x10], ecx
        mov    eax, dword ptr [eax + 0x10]
        or     eax, eax
        jne    loc_2_0E40
loc_2_0E4D:
        mov    ecx, dword ptr [esi + 0x10]
        cmp    ebx, ecx
        je     loc_2_0E81
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
        je     loc_2_0E83
        cmp    eax, 0x103
        je     loc_2_0E83
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0x10], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_2_0E81:
        sub    eax, eax
loc_2_0E83:
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

VxD__LDATA SEGMENT   ; object 4  vsize=0x4  RW-
        db  0FFh, 0FFh, 0FFh, 0FFh
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x830  R-X
        db  055h, 08Bh, 044h, 024h, 010h, 08Bh, 0ECh, 083h, 0ECh, 008h, 050h, 08Bh, 04Dh, 00Ch, 051h, 08Bh
        db  055h, 008h, 08Dh, 04Dh, 0FCh, 08Bh, 042h, 055h, 050h, 08Dh, 055h, 0F8h, 051h, 052h, 0E8h
        dd  loc_1_5912               ; fixup
        db  083h, 07Dh, 0F8h, 000h, 074h, 007h, 0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h, 08Bh, 045h, 0FCh
        db  08Bh, 0E5h, 05Dh, 0C2h, 00Ch, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0040:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 0xa7]
        cmp    cx, 0x18
        jbe    loc_5_0065
        mov    dword ptr [esi + 0x187], 0x18
        jmp    loc_5_0071
loc_5_0065:
        and    ecx, 0xffff
        mov    dword ptr [esi + 0x187], ecx
loc_5_0071:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x187]
        mov    ax, word ptr [esi + 0xab]
        lea    edx, [eax + ecx + 1]
        lea    eax, [esi + 0x55]
        push   edx
        lea    ecx, [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_1_5906
loc_5_0092:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_00A3
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_00A3:
        lea    edi, [esi + 0xaf]
        push   -1
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0xa7]
        lea    ecx, [eax + eax*2]
        lea    edx, [ecx + ecx*8]
        push   edx
        push   edi
        call   sub_1_58D6
loc_5_00C3:
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_5_00D5
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_00D5:
        xor    edx, edx
        mov    edi, dword ptr [edi]
        mov    dx, word ptr [esi + 0xa7]
        lea    eax, [edx + edx*2]
        lea    edx, [eax + eax*8]
        xor    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        push   -1
        and    ecx, 3
        push   eax
        rep stosb byte ptr es:[edi], al
        xor    eax, eax
        lea    edx, [esi + 0x206]
        mov    ax, word ptr [esi + 0xab]
        shl    eax, 2
        lea    ecx, [eax + eax*4]
        push   ecx
        push   edx
        call   sub_1_58D6
loc_5_0115:
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_5_0127
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0127:
        xor    eax, eax
        mov    edi, dword ptr [esi + 0x206]
        mov    ax, word ptr [esi + 0xab]
        push   -1
        shl    eax, 2
        lea    ecx, [eax + eax*4]
        xor    eax, eax
        shr    ecx, 2
        push   eax
        rep stosd dword ptr es:[edi], eax
        xor    eax, eax
        lea    ecx, [esi + 0x20e]
        mov    ax, word ptr [esi + 0xab]
        shl    eax, 2
        push   eax
        push   ecx
        call   sub_1_58D6
loc_5_015F:
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_5_0171
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0171:
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_02A6
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        push   1
        xor    eax, eax
        mov    ax, word ptr [esi + 0xab]
        mov    ecx, dword ptr [esi + 0x81]
        imul   eax, dword ptr [esi + 0x247]
        push   eax
        push   ecx
        call   sub_1_5862
loc_5_01A5:
        test   byte ptr [ebp - 5], 0xff
        je     loc_5_01BF
        xor    al, al
        pop    edi
        mov    word ptr [esi + 0xab], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_01BF:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_01D9
        xor    al, al
        pop    edi
        mov    word ptr [esi + 0xab], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_01D9:
        xor    ebx, ebx
        cmp    word ptr [esi + 0xab], bx
        jbe    loc_5_0304
        xor    edi, edi
        mov    dword ptr [ebp - 0x10], ebx
loc_5_01ED:
        mov    eax, dword ptr [esi + 0x247]
        mov    edx, dword ptr [esi + 0x20e]
        imul   eax, ebx
        add    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [edx + ecx], eax
        mov    eax, dword ptr [esi + 0x247]
        imul   eax, ebx
        add    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 0x206]
        mov    dword ptr [ecx + edi + 8], eax
        mov    eax, dword ptr [esi + 0x247]
        imul   eax, ebx
        add    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 0x206]
        shr    eax, 0x10
        and    ax, 0xff
        mov    word ptr [ecx + edi], ax
        mov    eax, dword ptr [esi + 0x247]
        imul   ax, bx
        add    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 0x206]
        mov    word ptr [ecx + edi + 2], ax
        mov    eax, dword ptr [esi + 0x247]
        mov    ecx, dword ptr [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [esi + 0x20e]
        mov    eax, dword ptr [edx + ecx]
        push   eax
        push   esi
        call   0
        mov    ecx, dword ptr [esi + 0x206]
        mov    dword ptr [ecx + edi + 4], eax
        mov    ecx, dword ptr [esi + 0x206]
        cmp    dword ptr [ecx + edi + 4], 0
        je     loc_5_029D
        add    edi, 0x14
        inc    ebx
        xor    eax, eax
        add    dword ptr [ebp - 0x10], 4
        mov    ax, word ptr [esi + 0xab]
        cmp    eax, ebx
        ja     loc_5_01ED
        jmp    loc_5_0304
loc_5_029D:
        mov    word ptr [esi + 0xab], bx
        jmp    loc_5_0304
loc_5_02A6:
        push   -1
        xor    eax, eax
        mov    ax, word ptr [esi + 0xab]
        push   0
        imul   eax, dword ptr [esi + 0x247]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_58D6
loc_5_02C4:
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_5_02D6
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_02D6:
        xor    edx, edx
        cmp    word ptr [esi + 0xab], dx
        jbe    loc_5_0304
loc_5_02E1:
        mov    eax, dword ptr [esi + 0x247]
        mov    ecx, dword ptr [esi + 0x20e]
        imul   eax, edx
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + edx*4], eax
        inc    edx
        xor    eax, eax
        mov    ax, word ptr [esi + 0xab]
        cmp    eax, edx
        ja     loc_5_02E1
loc_5_0304:
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_0370
        lea    edi, [esi + 0xbf]
        lea    eax, [esi + 0xbb]
        push   eax
        push   edi
        push   1
        xor    eax, eax
        mov    ax, word ptr [esi + 0xa7]
        mov    ecx, dword ptr [esi + 0x81]
        shl    eax, 8
        push   eax
        push   ecx
        call   sub_1_5862
loc_5_0336:
        mov    eax, dword ptr [edi]
        test   eax, eax
        jne    loc_5_0347
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0347:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0xa7]
        shl    ecx, 8
        push   ecx
        push   eax
        push   esi
        call   0
        mov    dword ptr [esi + 0xc3], eax
        test   eax, eax
        jne    loc_5_039F
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0370:
        push   -1
        xor    eax, eax
        mov    ax, word ptr [esi + 0xa7]
        push   0
        shl    eax, 8
        lea    ecx, [esi + 0xbf]
        push   eax
        push   ecx
        call   sub_1_58D6
loc_5_038D:
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_5_039F
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_039F:
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_0468
        xor    ebx, ebx
        cmp    dword ptr [esi + 0x187], ebx
        jbe    loc_5_041D
        lea    edi, [esi + 0x18b]
loc_5_03BC:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        xor    edx, edx
        mov    dx, word ptr [esi + 0x245]
        push   ecx
        push   1
        mov    eax, dword ptr [esi + 0x81]
        push   edx
        push   eax
        call   sub_1_5862
loc_5_03DC:
        test   byte ptr [ebp - 5], 0xff
        jne    loc_5_044A
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0455
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        xor    edx, edx
        mov    dword ptr [edi], eax
        mov    dword ptr [edi - 0xc4], ecx
        mov    dx, word ptr [esi + 0x245]
        mov    eax, dword ptr [edi]
        push   edx
        push   eax
        push   esi
        call   0
        mov    dword ptr [edi - 0x64], eax
        cmp    dword ptr [edi], 0
        je     loc_5_045D
        add    edi, 4
        inc    ebx
        cmp    dword ptr [esi + 0x187], ebx
        ja     loc_5_03BC
loc_5_041D:
        cmp    byte ptr [esi + 0x243], 0
        lea    edi, [esi + 0x21d]
        je     loc_5_04B8
        lea    eax, [esi + 0x221]
        mov    ecx, dword ptr [esi + 0x81]
        push   eax
        push   edi
        push   0
        push   6
        push   ecx
        call   sub_1_5862
loc_5_0448:
        jmp    loc_5_04C7
loc_5_044A:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0455:
        mov    dword ptr [esi + 0x187], ebx
        jmp    loc_5_041D
loc_5_045D:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0468:
        xor    edi, edi
        cmp    dword ptr [esi + 0x187], edi
        jbe    loc_5_041D
        lea    ebx, [esi + 0x18b]
loc_5_0478:
        push   -1
        xor    eax, eax
        mov    ax, word ptr [esi + 0x245]
        push   0
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_58D6
loc_5_048F:
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        jne    loc_5_04AD
        mov    eax, dword ptr [ebp - 4]
        add    ebx, 4
        inc    edi
        mov    dword ptr [ebx - 4], eax
        cmp    dword ptr [esi + 0x187], edi
        ja     loc_5_0478
        jmp    loc_5_041D
loc_5_04AD:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_04B8:
        push   -1
        push   0
        push   6
        push   edi
        call   sub_1_58D6
loc_5_04C4:
        mov    dword ptr [ebp - 0xc], eax
loc_5_04C7:
        cmp    dword ptr [edi], 0
        jne    loc_5_04D7
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_04D7:
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_050E
        lea    edi, [esi + 0xb3]
        lea    eax, [esi + 0xb7]
        push   eax
        mov    ecx, dword ptr [esi + 0x81]
        push   edi
        push   1
        push   0x1a
        push   ecx
        call   sub_1_5862
loc_5_04FE:
        cmp    dword ptr [edi], 0
        jne    loc_5_0532
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_050E:
        push   -1
        lea    eax, [esi + 0xb3]
        push   0
        push   0x1a
        push   eax
        call   sub_1_58D6
loc_5_0520:
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        je     loc_5_0532
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0532:
        cmp    byte ptr [esi + 0x243], 0
        lea    edi, [esi + 0x25b]
        je     loc_5_055B
        lea    eax, [esi + 0x25f]
        mov    ecx, dword ptr [esi + 0x81]
        push   eax
        push   edi
        push   0
        push   0xd
        push   ecx
        call   sub_1_5862
loc_5_0559:
        jmp    loc_5_0567
loc_5_055B:
        push   -1
        push   0
        push   0xd
        push   edi
        call   sub_1_58D6
loc_5_0567:
        cmp    dword ptr [edi], 1
        pop    edi
        sbb    al, al
        pop    esi
        inc    al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0580:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x55]
        test   eax, eax
        je     loc_5_0599
        push   eax
        call   sub_1_590C
loc_5_0599:
        mov    eax, dword ptr [esi + 0xaf]
        test   eax, eax
        je     loc_5_05BB
        push   0
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0xa7]
        lea    edx, [ecx + ecx*2]
        lea    ebx, [edx + edx*8]
        push   ebx
        push   eax
        call   sub_1_58DC
loc_5_05BB:
        mov    ecx, dword ptr [esi + 0x18b]
        test   ecx, ecx
        je     loc_5_0634
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_05F7
        mov    eax, dword ptr [esi + 0xc7]
        push   eax
        push   ecx
        push   1
        xor    eax, eax
        mov    ax, word ptr [esi + 0x245]
        mov    ecx, dword ptr [esi + 0x81]
        imul   eax, dword ptr [esi + 0x187]
        push   eax
        push   ecx
        call   sub_1_5868
loc_5_05F5:
        jmp    loc_5_0610
loc_5_05F7:
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0x245]
        imul   eax, dword ptr [esi + 0x187]
        push   eax
        push   ecx
        call   sub_1_58DC
loc_5_0610:
        xor    edi, edi
        cmp    dword ptr [esi + 0x187], edi
        jbe    loc_5_0634
        lea    ebx, [esi + 0x127]
loc_5_0620:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        push   eax
        inc    edi
        call   sub_1_591E
loc_5_062C:
        cmp    dword ptr [esi + 0x187], edi
        ja     loc_5_0620
loc_5_0634:
        mov    ecx, dword ptr [esi + 0xbf]
        test   ecx, ecx
        je     loc_5_0681
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_066C
        mov    eax, dword ptr [esi + 0xbb]
        push   eax
        push   ecx
        push   1
        xor    eax, eax
        mov    ax, word ptr [esi + 0xa7]
        mov    ecx, dword ptr [esi + 0x81]
        shl    eax, 8
        push   eax
        push   ecx
        call   sub_1_5868
loc_5_066A:
        jmp    loc_5_0681
loc_5_066C:
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0xa7]
        shl    eax, 8
        push   eax
        push   ecx
        call   sub_1_58DC
loc_5_0681:
        mov    eax, dword ptr [esi + 0xc3]
        test   eax, eax
        je     loc_5_0691
        push   eax
        call   sub_1_591E
loc_5_0691:
        xor    edi, edi
        cmp    dword ptr [esi + 0x206], edi
        je     loc_5_076F
        mov    ax, word ptr [esi + 0xab]
        test   ax, ax
        je     loc_5_076F
        jbe    loc_5_06D8
        xor    ebx, ebx
loc_5_06B3:
        mov    eax, dword ptr [esi + 0x206]
        mov    eax, dword ptr [eax + ebx + 4]
        test   eax, eax
        je     loc_5_06C7
        push   eax
        call   sub_1_591E
loc_5_06C7:
        add    ebx, 0x14
        inc    edi
        xor    eax, eax
        mov    ax, word ptr [esi + 0xab]
        cmp    eax, edi
        ja     loc_5_06B3
loc_5_06D8:
        mov    eax, dword ptr [esi + 0x20e]
        mov    edx, dword ptr [eax]
        test   edx, edx
        je     loc_5_0736
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_071B
        mov    eax, dword ptr [esi + 0x206]
        mov    ecx, dword ptr [eax + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0xab]
        push   ecx
        push   edx
        mov    ecx, dword ptr [esi + 0x247]
        push   1
        mov    edx, dword ptr [esi + 0x81]
        imul   ecx, eax
        push   ecx
        push   edx
        call   sub_1_5868
loc_5_0719:
        jmp    loc_5_0736
loc_5_071B:
        push   0
        mov    eax, dword ptr [esi + 0x247]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0xab]
        imul   eax, ecx
        push   eax
        push   edx
        call   sub_1_58DC
loc_5_0736:
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0xab]
        mov    ecx, dword ptr [esi + 0x20e]
        shl    eax, 2
        push   eax
        push   ecx
        call   sub_1_58DC
loc_5_0751:
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 0xab]
        mov    edx, dword ptr [esi + 0x206]
        shl    eax, 2
        lea    ecx, [eax + eax*4]
        push   ecx
        push   edx
        call   sub_1_58DC
loc_5_076F:
        mov    eax, dword ptr [esi + 0xb3]
        test   eax, eax
        je     loc_5_07A6
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_079C
        mov    ecx, dword ptr [esi + 0xb7]
        push   ecx
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x81]
        push   0x1a
        push   eax
        call   sub_1_5868
loc_5_079A:
        jmp    loc_5_07A6
loc_5_079C:
        push   0
        push   0x1a
        push   eax
        call   sub_1_58DC
loc_5_07A6:
        mov    eax, dword ptr [esi + 0x21d]
        test   eax, eax
        je     loc_5_07DD
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_07D3
        mov    ecx, dword ptr [esi + 0x221]
        push   ecx
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x81]
        push   6
        push   eax
        call   sub_1_5868
loc_5_07D1:
        jmp    loc_5_07DD
loc_5_07D3:
        push   0
        push   6
        push   eax
        call   sub_1_58DC
loc_5_07DD:
        mov    eax, dword ptr [esi + 0x25b]
        test   eax, eax
        je     loc_5_081B
        cmp    byte ptr [esi + 0x243], 0
        je     loc_5_0811
        mov    ecx, dword ptr [esi + 0x25f]
        push   ecx
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x81]
        push   0xd
        push   eax
        call   sub_1_5868
loc_5_0808:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0811:
        push   0
        push   0xd
        push   eax
        call   sub_1_58DC
loc_5_081B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 034h, 045h, 06Eh, 068h
VxD__LTEXT ENDS

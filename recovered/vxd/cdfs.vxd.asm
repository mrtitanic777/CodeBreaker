; CDFS -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 14163 instructions recovered by recursive descent

Declare_Virtual_Device CDFS, 3, 0, CDFS_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A0010100h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xaec  R-X
loc_1_0000:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    ah, 0x15
        jne    loc_1_004B
        movzx  eax, al
        cmp    eax, 0x11
        jge    loc_1_003E
        push   dword ptr [ebp + 0x2c]
        or     dword ptr [ebp + 0x2c], 0x200
        push   ebx
        push   ebp
        call   dword ptr [eax*4 + 0x9de0]    ; -> obj3:0x9DE0
        lea    esp, [esp + 8]
        pop    ecx
        and    dword ptr [ebp + 0x2c], 0xfffffdff
        and    ecx, 0x200
        or     dword ptr [ebp + 0x2c], ecx
        cmp    eax, 1
        cmc    
        ret    
loc_1_003E:
        or     word ptr [ebp + 0x2c], 1
        mov    word ptr [ebp + 0x1c], 1
        clc    
        ret    
loc_1_004B:
        stc    
        ret    
CDFS_Control:
        test   byte ptr [0x87c], 1           ; -> obj1:0x087C
        jne    loc_1_0062
        cmp    eax, 0x1b
        je     loc_2_00AD
        clc    
        ret    
loc_1_0061:
        clc    
loc_1_0062:
        ret    
sub_1_0063:
        VMMCall  VMM._BlockOnID                  ; cd20fa800100
sub_1_0069:
        VMMCall  VMM._SignalID                   ; cd2027810100
loc_1_006F:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 2
        ja     loc_1_007B
        xor    eax, eax
        ret    
loc_1_007B:
        mov    eax, 1
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0090:
        push   0x50
        mov    eax, dword ptr [esp + 8]
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_00B0:
        test   byte ptr [0x884], 1           ; -> obj1:0x0884
        je     loc_1_00C6
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0
        sub    eax, eax
        ret    
loc_1_00C6:
        push   0x3b
        mov    eax, dword ptr [esp + 8]
        push   1
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_00E0:
        test   byte ptr [0x884], 1           ; -> obj1:0x0884
        mov    eax, dword ptr [esp + 4]
        jne    loc_1_0101
        cmp    byte ptr [eax + 4], 2
        jne    loc_1_0101
        push   0x34
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
loc_1_0101:
        mov    word ptr [eax + 0x1a], 0
        sub    eax, eax
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0110:
        test   byte ptr [0x884], 1           ; -> obj1:0x0884
        jne    loc_1_0130
        push   0x34
        mov    eax, dword ptr [esp + 8]
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_0130:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0
        sub    eax, eax
        ret    
        db  08Dh, 049h, 000h
sub_1_0140:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        sub    esi, esi
        cmp    word ptr [0x89c], si          ; -> obj1:0x089C
        jne    loc_1_0165
        push   1
        mov    eax, dword ptr [0x898]        ; -> obj1:0x0898
        push   eax
        call   sub_3_8710
        add    esp, 8
        mov    esi, 1
loc_1_0165:
        cmp    word ptr [0x89c], 0           ; -> obj1:0x089C
        jne    loc_1_0309
        test   byte ptr [0x8a4], 0x80        ; -> obj1:0x08A4
        je     loc_1_01A4
        mov    ebx, 0x80
loc_1_0181:
        inc    byte ptr [0x8a0]              ; -> obj1:0x08A0
        push   1
        push   0x3d0                         ; -> sub_1_03D0
        call   sub_1_0063
        add    esp, 8
        dec    byte ptr [0x8a0]              ; -> obj1:0x08A0
        test   byte ptr [0x8a4], bl          ; -> obj1:0x08A4
        jne    loc_1_0181
loc_1_01A4:
        or     byte ptr [0x8a4], 0x10        ; -> obj1:0x08A4
        test   byte ptr [0x8a4], 4           ; -> obj1:0x08A4
        jne    loc_1_0309
        test   byte ptr [0x8a4], 1           ; -> obj1:0x08A4
        jne    loc_1_0287
        mov    eax, 0x9bc0                   ; -> obj3:0x9BC0
        cmp    eax, 0x57e0                   ; -> sub_3_57E0
        je     loc_1_0287
        mov    eax, 0x57e0                   ; -> sub_3_57E0
        shr    eax, 0xc
        push   0
        mov    ecx, 0x9bc0                   ; -> obj3:0x9BC0
        add    ecx, 0x1000
        shr    ecx, 0xc
        sub    ecx, eax
        push   ecx
        push   eax
        call   sub_3_5150
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_0280
loc_1_0201:
        mov    edi, 8
loc_1_0206:
        test   byte ptr [0x8a4], 1           ; -> obj1:0x08A4
        je     loc_1_0233
        mov    eax, 0x57e0                   ; -> sub_3_57E0
        shr    eax, 0xc
        push   0
        mov    ecx, 0x9bc0                   ; -> obj3:0x9BC0
        add    ecx, 0x1000
        shr    ecx, 0xc
        sub    ecx, eax
        push   ecx
        push   eax
        call   sub_3_5180
        add    esp, 0xc
loc_1_0233:
        test   byte ptr [0x8a4], 2           ; -> obj1:0x08A4
        je     loc_1_0260
        mov    eax, 0x9bc0                   ; -> obj3:0x9BC0
        shr    eax, 0xc
        push   0
        mov    ecx, 0xa334                   ; -> loc_3_A334
        add    ecx, 0x1000
        shr    ecx, 0xc
        sub    ecx, eax
        push   ecx
        push   eax
        call   sub_3_5180
        add    esp, 0xc
loc_1_0260:
        and    byte ptr [0x8a4], 0xfc        ; -> obj1:0x08A4
loc_1_0267:
        or     esi, esi
        je     loc_1_0279
        mov    eax, dword ptr [0x898]        ; -> obj1:0x0898
        push   eax
        call   sub_3_8700
        add    esp, 4
loc_1_0279:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_1_0280:
        or     byte ptr [0x8a4], 1           ; -> obj1:0x08A4
loc_1_0287:
        test   byte ptr [0x8a4], 2           ; -> obj1:0x08A4
        jne    loc_1_02CB
        mov    eax, 0x9bc0                   ; -> obj3:0x9BC0
        cmp    eax, 0xa334                   ; -> loc_3_A334
        je     loc_1_02CB
        shr    eax, 0xc
        push   0
        mov    ecx, 0xa334                   ; -> loc_3_A334
        add    ecx, 0x1000
        shr    ecx, 0xc
        sub    ecx, eax
        push   ecx
        push   eax
        call   sub_3_5150
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_02C4
        jmp    loc_1_0201
loc_1_02C4:
        or     byte ptr [0x8a4], 2           ; -> obj1:0x08A4
loc_1_02CB:
        mov    eax, 1
        mov    ecx, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        or     ecx, ecx
        je     loc_1_02F4
        push   0
        mov    eax, dword ptr [ecx + 0x44]
        add    eax, 0xfff
        shr    eax, 0xc
        push   eax
        shr    ecx, 0xc
        push   ecx
        call   sub_3_5150
        add    esp, 0xc
loc_1_02F4:
        or     eax, eax
        jne    loc_1_0300
        jmp    loc_1_0201
        db  08Dh, 049h, 000h
loc_1_0300:
        or     byte ptr [0x8a4], 4           ; -> obj1:0x08A4
        sub    edi, edi
loc_1_0309:
        or     edi, edi
        jne    loc_1_0206
        inc    word ptr [0x89c]              ; -> obj1:0x089C
        jmp    loc_1_0267
        db  08Dh, 049h, 000h
sub_1_0320:
        mov    ax, word ptr [0x89c]          ; -> obj1:0x089C
        dec    ax
        mov    word ptr [0x89c], ax          ; -> obj1:0x089C
        jne    loc_1_0337
        or     byte ptr [0x8a4], 0x20        ; -> obj1:0x08A4
loc_1_0337:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0340:
        inc    word ptr [0x89c]              ; -> obj1:0x089C
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0350:
        sub    eax, eax
        cmp    word ptr [0x89c], ax          ; -> obj1:0x089C
        jne    loc_1_03CC
        mov    cl, byte ptr [0x8a4]          ; -> obj1:0x08A4
        and    cl, 0x70
        cmp    cl, 0x20
        jne    loc_1_03A2
        or     byte ptr [0x8a4], 0x40        ; -> obj1:0x08A4
        and    dword ptr [4], 0xffffefff     ; -> obj4:0x0004
        test   byte ptr [5], 0x20            ; -> obj4:0x0005
        jne    loc_1_039B
        or     dword ptr [4], 0x2000         ; -> obj4:0x0004
        push   eax
        push   0x78                          ; -> loc_4_0078
        call   sub_1_07F0
        add    esp, 8
loc_1_039B:
        mov    eax, 1
        jmp    loc_1_03B2
loc_1_03A2:
        test   byte ptr [0x8a4], 0x10        ; -> obj1:0x08A4
        je     loc_1_03B2
        and    byte ptr [0x8a4], 0xef        ; -> obj1:0x08A4
loc_1_03B2:
        cmp    byte ptr [0x8a0], 0           ; -> obj1:0x08A0
        je     loc_1_03CC
        or     eax, eax
        je     loc_1_03CC
        push   0x3d0                         ; -> sub_1_03D0
        call   sub_1_0069
        add    esp, 4
loc_1_03CC:
        ret    
        db  08Dh, 049h, 000h
sub_1_03D0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        cmp    word ptr [0x89c], 0           ; -> obj1:0x089C
        jne    loc_1_052F
        test   byte ptr [0x884], 1           ; -> obj1:0x0884
        jne    loc_1_052F
        mov    al, byte ptr [0x8a4]          ; -> obj1:0x08A4
        and    al, 0x14
        cmp    al, 4
        jne    loc_1_052F
        or     byte ptr [0x8a4], 0x80        ; -> obj1:0x08A4
        mov    eax, 0x57e0                   ; -> sub_3_57E0
        shr    eax, 0xc
        push   0
        mov    ecx, 0x9bc0                   ; -> obj3:0x9BC0
        add    ecx, 0x1000
        shr    ecx, 0xc
        sub    ecx, eax
        push   ecx
        push   eax
        call   sub_3_5180
        add    esp, 0xc
        mov    eax, 0x9bc0                   ; -> obj3:0x9BC0
        shr    eax, 0xc
        push   0
        mov    ecx, 0xa334                   ; -> loc_3_A334
        add    ecx, 0x1000
        shr    ecx, 0xc
        sub    ecx, eax
        push   ecx
        push   eax
        call   sub_3_5180
        add    esp, 0xc
        and    byte ptr [0x8a4], 0xfc        ; -> obj1:0x08A4
        or     esi, esi
        je     loc_1_04C9
        sub    ebx, ebx
        cmp    dword ptr [esi + 0x64], ebx
        jbe    loc_1_04AF
        sub    edi, edi
        sub    ebp, ebp
loc_1_0469:
        mov    eax, dword ptr [esi + 4]
        add    eax, edi
        cmp    word ptr [eax + 4], bp
        jne    loc_1_04A6
        cmp    word ptr [eax + 6], bp
        je     loc_1_04A0
loc_1_047A:
        push   ebp
        push   1
        mov    eax, dword ptr [esi + 0x68]
        shr    eax, 0xc
        add    eax, ebx
        push   eax
        call   sub_3_5180
        add    esp, 0xc
        mov    eax, dword ptr [esi + 4]
        dec    word ptr [edi + eax + 6]
        mov    eax, dword ptr [esi + 4]
        cmp    word ptr [edi + eax + 6], bp
        jne    loc_1_047A
loc_1_04A0:
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + eax], ebp
loc_1_04A6:
        add    edi, 8
        inc    ebx
        cmp    dword ptr [esi + 0x64], ebx
        ja     loc_1_0469
loc_1_04AF:
        push   0
        mov    eax, dword ptr [esi + 0x44]
        add    eax, 0xfff
        shr    eax, 0xc
        push   eax
        shr    esi, 0xc
        push   esi
        call   sub_3_5180
        add    esp, 0xc
loc_1_04C9:
        mov    eax, dword ptr [0x9bdc]       ; -> obj3:0x9BDC
        or     eax, eax
        je     loc_1_04FB
        push   0
        mov    ecx, dword ptr [eax + 0x64]
        push   ecx
        mov    eax, dword ptr [eax + 0x68]
        shr    eax, 0xc
        push   eax
        call   sub_3_5180
        add    esp, 0xc
        mov    eax, dword ptr [0x9bdc]       ; -> obj3:0x9BDC
        mov    dword ptr [0x9be0], eax       ; -> obj3:0x9BE0
        mov    dword ptr [0x9bdc], 0         ; -> obj3:0x9BDC
loc_1_04FB:
        push   0x9c8c                        ; -> obj3:0x9C8C
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [0x9c14]       ; -> loc_3_9C14
        mov    dword ptr [0x9c14], 0         ; -> loc_3_9C14
        or     eax, eax
        je     loc_1_0528
        push   8
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0xc
loc_1_0528:
        and    byte ptr [0x8a4], 0x5b        ; -> obj1:0x08A4
loc_1_052F:
        mov    eax, dword ptr [0x9bf8]       ; -> loc_3_9BF8
        and    eax, 0xfffffffc
        mov    dword ptr [0x9bf8], eax       ; -> loc_3_9BF8
        and    byte ptr [0x8a4], 0xbf        ; -> obj1:0x08A4
        cmp    byte ptr [0x8a0], 0           ; -> obj1:0x08A0
        je     loc_1_0559
        push   0x3d0                         ; -> sub_1_03D0
        call   sub_1_0069
        add    esp, 4
loc_1_0559:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_0560:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        xor    eax, eax
        mov    word ptr [esi + 2], 0xffff
        mov    ax, word ptr [esi]
        cmp    eax, 0xe
        ja     loc_1_0769
        jmp    dword ptr [eax*4 + 0x774]     ; -> obj1:0x0774
loc_1_0586:
        mov    word ptr [esi + 2], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0594:
        cmp    word ptr [0x89c], 0           ; -> obj1:0x089C
        jne    loc_1_05F9
        cmp    byte ptr [0x8a0], 0           ; -> obj1:0x08A0
        jne    loc_1_05F9
        mov    edx, dword ptr [0x890]        ; -> obj1:0x0890
        add    edx, 0x14
        sub    eax, eax
        inc    edx
loc_1_05B3:
        mov    cl, byte ptr [edx]
        and    cl, 0x18
        cmp    cl, 0x18
        je     loc_1_05C8
        add    edx, 6
        inc    eax
        cmp    eax, 0x1a
        jb     loc_1_05B3
        jmp    loc_1_05CE
loc_1_05C8:
        mov    word ptr [esi + 2], 0
loc_1_05CE:
        cmp    word ptr [esi + 2], 0
        je     loc_1_0769
        and    byte ptr [0x87c], 0x7f        ; -> obj1:0x087C
        call   sub_3_0000
        mov    al, byte ptr [0x87c]          ; -> obj1:0x087C
        and    al, 0xef
        mov    byte ptr [0x87c], al          ; -> obj1:0x087C
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_05F9:
        mov    word ptr [esi + 2], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0607:
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [esp + 0x10], eax
        test   byte ptr [0x87c], 0x10        ; -> obj1:0x087C
        je     loc_1_06BC
        cmp    byte ptr [eax + 0x40], 5
        jne    loc_1_06BC
        test   byte ptr [eax + 0x21], 0x40
        je     loc_1_06BC
        mov    edx, dword ptr [0x890]        ; -> obj1:0x0890
        sub    ebx, ebx
        lea    edi, [edx + 0x14]
        mov    ecx, dword ptr [0x874]        ; -> obj1:0x0874
loc_1_0640:
        lea    eax, [ebx + ebx*2]
        lea    ebp, [edi + eax*2]
        mov    al, byte ptr [ebp + 1]
        and    al, 0x18
        cmp    al, 0x18
        jne    loc_1_0660
        xor    eax, eax
        mov    al, byte ptr [ebp]
        cmp    eax, ecx
        jne    loc_1_0660
        inc    ecx
        sub    ebx, ebx
        jmp    loc_1_0661
        db  08Dh, 049h, 000h
loc_1_0660:
        inc    ebx
loc_1_0661:
        cmp    ebx, 0x1a
        jb     loc_1_0640
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x874], ecx        ; -> obj1:0x0874
        mov    al, byte ptr [eax + 0x14]
        and    eax, 0xff
        lea    eax, [eax + eax*2]
        lea    edx, [edx + eax*2 + 0x14]
        mov    eax, dword ptr [0x88c]        ; -> obj1:0x088C
        mov    dword ptr [edx + 2], eax
        mov    byte ptr [edx], cl
        mov    al, byte ptr [0x878]          ; -> obj1:0x0878
        or     al, 0x38
        mov    byte ptr [edx + 1], al
        or     byte ptr [0x87c], 0x80        ; -> obj1:0x087C
        mov    eax, dword ptr [0x888]        ; -> obj1:0x0888
        or     eax, eax
        je     loc_1_06B7
        mov    dword ptr [0x874], ecx        ; -> obj1:0x0874
        xor    edx, edx
        mov    dl, byte ptr [eax + 0x15]
        cmp    edx, ecx
        ja     loc_1_06B7
        inc    cl
        mov    byte ptr [eax + 0x15], cl
loc_1_06B7:
        call   sub_1_07B0
loc_1_06BC:
        mov    word ptr [esi + 2], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_06D0:
        mov    eax, dword ptr [esi + 0xc]
        cmp    byte ptr [eax + 0x40], 5
        jne    loc_1_0769
        test   byte ptr [eax + 0x21], 0x40
        je     loc_1_0769
        mov    al, byte ptr [eax + 0x14]
        and    eax, 0xff
        mov    edx, dword ptr [0x890]        ; -> obj1:0x0890
        lea    eax, [eax + eax*2]
        lea    eax, [edx + eax*2 + 0x14]
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [eax], 0
        mov    dword ptr [eax + 2], 0
        mov    dword ptr [0x874], 0          ; -> obj1:0x0874
        mov    word ptr [esi + 2], 0
        call   sub_1_07B0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0727:
        or     byte ptr [0x884], 1           ; -> obj1:0x0884
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0736:
        mov    ecx, dword ptr [0x870]        ; -> obj1:0x0870
        or     ecx, ecx
        je     loc_1_075B
loc_1_0740:
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [0x870], eax        ; -> obj1:0x0870
        push   ecx
        call   sub_3_8580
        add    esp, 4
        mov    ecx, dword ptr [0x870]        ; -> obj1:0x0870
        or     ecx, ecx
        jne    loc_1_0740
loc_1_075B:
        test   byte ptr [0x87c], 0x80        ; -> obj1:0x087C
        je     loc_1_0769
        call   sub_1_0350
loc_1_0769:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
        ; ---- dispatch table (15 entries) ----
        dd  loc_1_0586
        dd  loc_1_0769
        dd  loc_1_0594
        dd  loc_1_0607
        dd  loc_1_06D0
        dd  loc_1_0769
        dd  loc_1_0769
        dd  loc_1_0769
        dd  loc_1_0736
        dd  loc_1_0769
        dd  loc_1_0769
        dd  loc_1_0769
        dd  loc_1_0769
        dd  loc_1_0769
        dd  loc_1_0727
sub_1_07B0:
        push   ebx
        sub    edx, edx
        mov    ecx, dword ptr [0x890]        ; -> obj1:0x0890
        add    ecx, 0x14
        sub    eax, eax
        inc    ecx
        nop    
loc_1_07C0:
        mov    bl, byte ptr [ecx]
        and    bl, 0x18
        cmp    bl, 0x18
        je     loc_1_07D5
        add    ecx, 6
        inc    eax
        cmp    eax, 0x1a
        jb     loc_1_07C0
        jmp    loc_1_07D7
loc_1_07D5:
        mov    edx, eax
loc_1_07D7:
        mov    eax, dword ptr [0x888]        ; -> obj1:0x0888
        or     eax, eax
        je     loc_1_07E5
        inc    dl
        mov    byte ptr [eax + 0x14], dl
loc_1_07E5:
        pop    ebx
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'CDFS    '                  ; DDB_Name
        dd  0A0010100h                  ; DDB_Init_Order
        dd  CDFS_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_08A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_08B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    al, byte ptr [eax]
        outsd  dx, dword ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], dh            ; -> sub_3_1930
        sbb    dword ptr [eax], eax
        add    byte ptr [ecx], ah            ; -> loc_1_0061
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    esp, 0xc7000031               ; -> loc_3_31C7
        xor    dword ptr [eax], eax
        add    byte ptr [edx], cl
        add    edx, dword ptr [eax]
        or     byte ptr [eax - 0x1fffffc5], al ; -> loc_3_3B80
        add    byte ptr [eax], al
        add    byte ptr [eax - 0x4fffffe6], ah ; -> loc_3_1AA0
        sbb    al, byte ptr [eax]
        add    byte ptr [eax + 0x4000003b], ah ; -> loc_3_3BA0
        cmp    al, 0
        add    byte ptr [eax + 0x3c], al     ; -> sub_3_3C40
        add    byte ptr [eax], al
        adc    byte ptr [eax + eax], bh
        add    byte ptr [edx], cl
        add    edx, dword ptr [eax]
        or     byte ptr [eax + 0x3c], al     ; -> sub_3_3C40
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        add    byte ptr [eax + 0x3c], al     ; -> sub_3_3C40
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        add    byte ptr [eax + 0x3c], al     ; -> sub_3_3C40
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        add    byte ptr [eax + 0x3c], al     ; -> sub_3_3C40
        add    byte ptr [eax], al
loc_1_0944:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        sbb    byte ptr [edx], 0
        add    byte ptr [eax - 0x2fffffc6], ah ; -> loc_3_3AA0
        cmp    al, byte ptr [eax]
        add    byte ptr [eax - 0x70000000], dl ; -> loc_1_0090
        dec    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebx], bh
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        add    byte ptr [eax], al            ; -> loc_3_4A00
        dec    edx
        add    byte ptr [eax], al
        and    byte ptr [0xb00000], al       ; -> loc_1_00B0
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        add    byte ptr [eax + 0x4000001a], dl ; -> loc_3_1A90
        cmp    al, 0
        add    byte ptr [eax], ah            ; -> loc_3_4A20
        dec    edx
        add    byte ptr [eax], al
        inc    eax
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x57], dl
        mov    esi, dword ptr [esp + 0xc]
        sub    edi, edi
        mov    eax, dword ptr [esi + 0x14]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        cmp    eax, 0x14
        jg     loc_1_09C3
        je     loc_1_0A9C
        sub    eax, 3
        cmp    eax, 0xd
        ja     loc_1_0A9C
        shl    eax, 2
        jmp    loc_1_0AA4
loc_1_09C3:
        cmp    eax, 0x18
        je     loc_1_0A9C
        cmp    eax, 0x80
        jl     loc_1_0A9C
        cmp    eax, 0x88
        jle    loc_1_0A8A
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
loc_1_09E7:
        mov    eax, dword ptr [esi + 0x14]
        mov    eax, dword ptr [eax + 0xe]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_3_5958
        add    esp, 8
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    ax, word ptr [eax + 0x12]
        and    eax, 0xffff
        dec    eax
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [esi + 0x14]
        cmp    byte ptr [eax + 2], 3
        mov    eax, dword ptr [esi + 0x18]
        mov    al, byte ptr [eax]
        jne    loc_1_0A42
        and    eax, 0xff
        or     eax, eax
        jl     loc_1_0A9C
        cmp    eax, 0xf
        jg     loc_1_0A9C
        mov    eax, dword ptr [esi + 0x18]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    edi, dword ptr [eax*4 + 0x9e24] ; -> loc_3_9E24
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
loc_1_0A42:
        and    eax, 0xff
        or     eax, eax
        jl     loc_1_0A9C
        cmp    eax, 5
        jg     loc_1_0A9C
        mov    eax, dword ptr [esi + 0x18]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    edi, dword ptr [eax*4 + 0x9e64] ; -> loc_3_9E64
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
loc_1_0A66:
        mov    edi, 0xb80                    ; -> loc_3_0B80
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
loc_1_0A70:
        mov    edi, 0xbb0                    ; -> loc_3_0BB0
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0A80:
        mov    edi, 0xbb0                    ; -> loc_3_0BB0
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
loc_1_0A8A:
        mov    eax, dword ptr [esi + 0x14]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        mov    edi, dword ptr [eax*4 + 0x9c7c] ; -> obj3:0x9C7C
loc_1_0A9C:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_1_0AA4:
        out    9, eax
        add    byte ptr [eax], al
        pushfd 
        or     al, byte ptr [eax]
        add    byte ptr [edx + ecx + 0xa9c0000], bl ; -> loc_1_0A9C
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [edx + ecx + 0xa9c0000], bl ; -> loc_1_0A9C
        add    byte ptr [eax], al
        pushfd 
        or     al, byte ptr [eax]
        add    byte ptr [edx + ecx + 0x9e70000], bl ; -> loc_1_0A9C
        add    byte ptr [eax], al
        jo     0xad8
        add    byte ptr [eax], al
        or     byte ptr [edx], 0
        add    byte ptr [edx + ecx + 0xa9c0000], bl ; -> loc_1_0A9C
        add    byte ptr [eax], al
        lea    esp, [esp]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x758  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0010:
        push   ebx
        jns    0x86
        je     loc_2_007A
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x8c
        jb     loc_2_0081
        outsb  dx, byte ptr [esi]
        je     loc_2_0062
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_2_0095
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_2_0085
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0xa0
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        inc    esi
        imul   ebp, dword ptr [ebp + 0x53], 0x65747379
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        inc    esp
        inc    esi
        push   ebx
        add    byte ptr [ebx + 0x61], al
        arpl   word ptr [eax + 0x65], bp
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x65725000
        je     0xb5
        db  068h, 000h
loc_2_0054:
        push   eax
        jb     0xbc
        je     loc_2_00BE
        push   0x6c696154
        db  000h
loc_2_0061:
        push   edx
loc_2_0062:
        popal  
        ja     loc_2_00A8
        popal  
        arpl   word ptr [eax + 0x65], bp
        db  000h
loc_2_006A:
        inc    ebp
        js     0xe1
        outsb  dx, byte ptr gs:[esi]
        inc    ebp
        jb     0xe7
        outsd  dx, dword ptr [esi]
        jb     loc_2_00EB
        db  000h
loc_2_0079:
        push   ebx
loc_2_007A:
        push   esi
        inc    esp
        push   ebx
        outsb  dx, byte ptr gs:[esi]
        jae    0xe6
loc_2_0081:
        add    byte ptr [ebx + 0x68], dl
        outsd  dx, dword ptr [esi]
loc_2_0085:
        ja     0xdd
        jb     0xfd
        imul   ebp, dword ptr [edi + 0x6e], 0x534d0073
        inc    ebx
        inc    esp
        inc    ebp
        pop    eax
loc_2_0095:
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0xfb
        je     0xea
        popal  
        insd   dword ptr es:[edi], dx
        jae    loc_2_00A1
loc_2_00A1:
        dec    esi
        outsd  dx, dword ptr [esi]
        push   edx
        popal  
        inc    ecx
loc_2_00A8:
        push   0x646165
loc_2_00AD:
        or     byte ptr [0x87c], 1           ; -> obj1:0x087C
        mov    dword ptr [0x18], 0x7050      ; -> sub_4_0018
loc_2_00BE:
        mov    dword ptr [0x1c], 0x3d0       ; -> sub_4_001C
        VMMCall  VMM.Get_System_Time_Address     ; cd2044010100
        mov    dword ptr [0x9c1c], eax       ; -> loc_3_9C1C
        VxDCall  BLOCKDEV.BlockDev_Get_Version   ; cd2000001000
        jb     loc_2_020A
        VxDCall  VCDFSD.Service_0000h            ; cd2000004100
        jb     loc_2_020A
loc_2_00EB:
        cmp    eax, 0x400
        jb     loc_2_020A
        VxDCall  VCDFSD.Service_0001h            ; cd2001004100
        cmp    word ptr [eax], 4
        jne    loc_2_020A
        mov    dword ptr [0x890], eax        ; -> obj1:0x0890
        movzx  ecx, word ptr [eax + 4]
        and    ecx, 3
        cmp    ecx, 1
        jne    loc_2_020A
        cmp    word ptr [eax + 6], 0x22
        jne    loc_2_020A
        or     word ptr [eax + 4], 2
        or     byte ptr [0x87c], 2           ; -> obj1:0x087C
        movzx  ecx, word ptr [eax + 8]
        mov    dword ptr [0x9bfc], ecx       ; -> loc_3_9BFC
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [0x88c], ecx        ; -> obj1:0x088C
        shr    ecx, 0xc
        mov    dword ptr [0x888], ecx        ; -> obj1:0x0888
        test   dword ptr [4], 2              ; -> obj4:0x0004
        jne    loc_2_016E
        mov    esi, 8                        ; -> obj4:0x0008
        VMMCall  VMM.Install_Exception_Handler   ; cd20ef000100
        jb     loc_2_016E
        or     dword ptr [4], 2              ; -> obj4:0x0004
loc_2_016E:
        push   1
        call   sub_3_86D0
        lea    esp, [esp + 4]
        mov    dword ptr [0x898], eax        ; -> obj1:0x0898
        or     eax, eax
        je     loc_2_01F8
        or     byte ptr [0x87c], 4           ; -> obj1:0x087C
        call   sub_2_020C
        or     byte ptr [0x87c], 0x10        ; -> obj1:0x087C
        push   0x720                         ; -> loc_2_0720
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        lea    esp, [esp + 4]
        push   0x8dc                         ; -> loc_1_08DC
        VxDCall  VCDFSD.Service_0002h            ; cd2002004100
        lea    esp, [esp + 4]
        or     eax, eax
        jne    loc_2_01F8
        or     byte ptr [0x87c], 0x20        ; -> obj1:0x087C
        or     dword ptr [4], 1              ; -> obj4:0x0004
        test   dword ptr [4], 0x20           ; -> obj4:0x0004
        je     loc_2_01DB
        or     dword ptr [4], 0x10           ; -> obj4:0x0004
loc_2_01DB:
        test   dword ptr [4], 0x2000         ; -> obj4:0x0004
        je     loc_2_01F1
        or     dword ptr [4], 0x1000         ; -> obj4:0x0004
loc_2_01F1:
        call   sub_2_06D0
        clc    
        ret    
loc_2_01F8:
        call   sub_3_0000
        xor    eax, eax
        xchg   byte ptr [0x87c], al          ; -> obj1:0x087C
        shr    al, 4
        cmc    
        ret    
loc_2_020A:
        stc    
        ret    
sub_2_020C:
        lea    eax, [0]                      ; -> obj2:0x0000
        push   eax
        push   0x10                          ; -> loc_2_0010
        push   0x80000002
        VMMCall  VMM._RegCreateKey               ; cd204a010100
        lea    esp, [esp + 0xc]
        or     eax, eax
        jne    loc_2_05D5
        mov    dword ptr [4], 4              ; -> obj2:0x0004
        mov    dword ptr [0xc], 3            ; -> obj2:0x000C
        lea    eax, [4]                      ; -> obj2:0x0004
        push   eax
        lea    eax, [8]                      ; -> obj2:0x0008
        push   eax
        lea    eax, [0xc]                    ; -> obj2:0x000C
        push   eax
        push   0
        push   0x41                          ; -> obj2:0x0041
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        lea    esp, [esp + 0x18]
        or     eax, eax
        jne    loc_2_05D6
        cmp    dword ptr [0xc], 4            ; -> obj2:0x000C
        je     loc_2_029A
        cmp    dword ptr [0xc], 3            ; -> obj2:0x000C
        jne    loc_2_05D6
        cmp    dword ptr [4], 4              ; -> obj2:0x0004
        jne    loc_2_05D6
loc_2_029A:
        mov    eax, dword ptr [8]            ; -> obj2:0x0008
        cmp    eax, 8
        jb     loc_2_05D6
        mov    dword ptr [0x9c00], eax       ; -> loc_3_9C00
        mov    dword ptr [4], 4              ; -> obj2:0x0004
        mov    dword ptr [0xc], 3            ; -> obj2:0x000C
        lea    eax, [4]                      ; -> obj2:0x0004
        push   eax
        lea    eax, [8]                      ; -> obj2:0x0008
        push   eax
        lea    eax, [0xc]                    ; -> obj2:0x000C
        push   eax
        push   0
        push   0x4b                          ; -> obj2:0x004B
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        lea    esp, [esp + 0x18]
        or     eax, eax
        jne    loc_2_05D6
        cmp    dword ptr [0xc], 4            ; -> obj2:0x000C
        je     loc_2_0318
        cmp    dword ptr [0xc], 3            ; -> obj2:0x000C
        jne    loc_2_05D6
        cmp    dword ptr [4], 4              ; -> obj2:0x0004
        jne    loc_2_05D6
loc_2_0318:
        mov    eax, dword ptr [8]            ; -> obj2:0x0008
        mov    dword ptr [0x9c04], eax       ; -> loc_3_9C04
        mov    dword ptr [4], 4              ; -> obj2:0x0004
        mov    dword ptr [0xc], 3            ; -> obj2:0x000C
        lea    eax, [4]                      ; -> obj2:0x0004
        push   eax
        lea    eax, [8]                      ; -> obj2:0x0008
        push   eax
        lea    eax, [0xc]                    ; -> obj2:0x000C
        push   eax
        push   0
        push   0x54                          ; -> loc_2_0054
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        lea    esp, [esp + 0x18]
        or     eax, eax
        jne    loc_2_05D6
        cmp    dword ptr [0xc], 4            ; -> obj2:0x000C
        je     loc_2_038D
        cmp    dword ptr [0xc], 3            ; -> obj2:0x000C
        jne    loc_2_05D6
        cmp    dword ptr [4], 4              ; -> obj2:0x0004
        jne    loc_2_05D6
loc_2_038D:
        mov    eax, dword ptr [8]            ; -> obj2:0x0008
        mov    dword ptr [0x9c08], eax       ; -> loc_3_9C08
loc_2_0397:
        mov    dword ptr [4], 4              ; -> obj2:0x0004
        mov    dword ptr [0xc], 3            ; -> obj2:0x000C
        lea    eax, [4]                      ; -> obj2:0x0004
        push   eax
        lea    eax, [8]                      ; -> obj2:0x0008
        push   eax
        lea    eax, [0xc]                    ; -> obj2:0x000C
        push   eax
        push   0
        push   0x61                          ; -> loc_2_0061
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        lea    esp, [esp + 0x18]
        or     eax, eax
        jne    loc_2_0401
        cmp    dword ptr [0xc], 4            ; -> obj2:0x000C
        je     loc_2_03ED
        cmp    dword ptr [0xc], 3            ; -> obj2:0x000C
        jne    loc_2_0401
loc_2_03ED:
        mov    eax, dword ptr [8]            ; -> obj2:0x0008
        cmp    eax, 4
        ja     loc_2_03FC
        mov    eax, 4
loc_2_03FC:
        mov    dword ptr [0x9c0c], eax       ; -> loc_3_9C0C
loc_2_0401:
        mov    dword ptr [4], 1              ; -> obj2:0x0004
        mov    dword ptr [0xc], 3            ; -> obj2:0x000C
        lea    eax, [4]                      ; -> obj2:0x0004
        push   eax
        lea    eax, [8]                      ; -> obj2:0x0008
        push   eax
        lea    eax, [0xc]                    ; -> obj2:0x000C
        push   eax
        push   0
        push   0x6a                          ; -> loc_2_006A
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        lea    esp, [esp + 0x18]
        or     eax, eax
        jne    loc_2_0458
        cmp    dword ptr [0xc], 3            ; -> obj2:0x000C
        jne    loc_2_0458
        mov    al, byte ptr [8]              ; -> obj2:0x0008
        mov    byte ptr [0x9be8], al         ; -> loc_3_9BE8
loc_2_0458:
        mov    dword ptr [4], 1              ; -> obj2:0x0004
        mov    dword ptr [0xc], 3            ; -> obj2:0x000C
        lea    eax, [4]                      ; -> obj2:0x0004
        push   eax
        lea    eax, [8]                      ; -> obj2:0x0008
        push   eax
        lea    eax, [0xc]                    ; -> obj2:0x000C
        push   eax
        push   0
        push   0x79                          ; -> loc_2_0079
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        lea    esp, [esp + 0x18]
        or     eax, eax
        jne    loc_2_04B5
        cmp    dword ptr [0xc], 3            ; -> obj2:0x000C
        jne    loc_2_04B5
        movzx  eax, byte ptr [8]             ; -> obj2:0x0008
        cmp    al, 2
        ja     loc_2_04B5
        mov    byte ptr [0x878], al          ; -> obj1:0x0878
loc_2_04B5:
        mov    dword ptr [4], 1              ; -> obj2:0x0004
        mov    dword ptr [0xc], 3            ; -> obj2:0x000C
        lea    eax, [4]                      ; -> obj2:0x0004
        push   eax
        lea    eax, [8]                      ; -> obj2:0x0008
        push   eax
        lea    eax, [0xc]                    ; -> obj2:0x000C
        push   eax
        push   0
        push   0x82                          ; -> obj2:0x0082
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        lea    esp, [esp + 0x18]
        or     eax, eax
        jne    loc_2_050C
        cmp    dword ptr [0xc], 3            ; -> obj2:0x000C
        jne    loc_2_050C
        mov    al, byte ptr [8]              ; -> obj2:0x0008
        mov    byte ptr [0x880], al          ; -> obj1:0x0880
loc_2_050C:
        mov    dword ptr [4], 1              ; -> obj2:0x0004
        mov    dword ptr [0xc], 3            ; -> obj2:0x000C
        lea    eax, [4]                      ; -> obj2:0x0004
        push   eax
        lea    eax, [8]                      ; -> obj2:0x0008
        push   eax
        lea    eax, [0xc]                    ; -> obj2:0x000C
        push   eax
        push   0
        push   0x8f                          ; -> obj2:0x008F
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        lea    esp, [esp + 0x18]
        or     eax, eax
        jne    loc_2_0563
        cmp    dword ptr [0xc], 3            ; -> obj2:0x000C
        jne    loc_2_0563
        mov    al, byte ptr [8]              ; -> obj2:0x0008
        mov    byte ptr [0x9bec], al         ; -> loc_3_9BEC
loc_2_0563:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   0xa1                          ; -> loc_2_00A1
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        lea    esp, [esp + 0x18]
        or     eax, eax
        jne    loc_2_05C5
        mov    eax, dword ptr [0x9c08]       ; -> loc_3_9C08
        mov    dword ptr [0x9c04], eax       ; -> loc_3_9C04
        lea    ecx, [0x9c04]                 ; -> loc_3_9C04
        lea    edx, [0x4b]                   ; -> obj2:0x004B
        push   4
        push   ecx
        push   4
        push   0
        push   edx
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        lea    edx, [0xa1]                   ; -> loc_2_00A1
        push   edx
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegDeleteValue             ; cd204f010100
        add    esp, 8
loc_2_05C5:
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        lea    esp, [esp + 4]
loc_2_05D5:
        ret    
loc_2_05D6:
        call   sub_2_05E0
        jmp    loc_2_0397
sub_2_05E0:
        push   ebx
        sub    esp, 0x40
        mov    ebx, esp
        push   0
        push   ebx
        VMMCall  VMM._GetDemandPageInfo          ; cd206f000100
        add    esp, 8
        mov    eax, dword ptr [ebx + 4]
        add    esp, 0x40
        pop    ebx
        cmp    eax, 0xb40
        jae    loc_2_0647
        cmp    eax, 0x740
        jae    loc_2_0627
        mov    dword ptr [0x9c00], 0x100     ; -> loc_3_9C00
        mov    dword ptr [0x9c04], 0x84      ; -> loc_3_9C04
        mov    dword ptr [0x9c08], 0x20      ; -> loc_3_9C08
        jmp    loc_2_0665
loc_2_0627:
        mov    dword ptr [0x9c00], 0x139     ; -> loc_3_9C00
        mov    dword ptr [0x9c04], 0x72      ; -> loc_3_9C04
        mov    dword ptr [0x9c08], 0x40      ; -> loc_3_9C08
        jmp    loc_2_0665
loc_2_0647:
        mov    dword ptr [0x9c00], 0x26b     ; -> loc_3_9C00
        mov    dword ptr [0x9c04], 0xe4      ; -> loc_3_9C04
        mov    dword ptr [0x9c08], 0x80      ; -> loc_3_9C08
loc_2_0665:
        lea    ecx, [0x9c00]                 ; -> loc_3_9C00
        lea    edx, [0x41]                   ; -> obj2:0x0041
        push   4
        push   ecx
        push   4
        push   0
        push   edx
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        lea    ecx, [0x9c04]                 ; -> loc_3_9C04
        lea    edx, [0x4b]                   ; -> obj2:0x004B
        push   4
        push   ecx
        push   4
        push   0
        push   edx
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        lea    ecx, [0x9c08]                 ; -> loc_3_9C08
        lea    edx, [0x54]                   ; -> loc_2_0054
        push   4
        push   ecx
        push   4
        push   0
        push   edx
        push   dword ptr [0]                 ; -> obj2:0x0000
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        ret    
        db  000h
sub_2_06D0:
        mov    dword ptr [0x9cb4], 0x53464443 ; -> obj3:0x9CB4
        mov    word ptr [0x9cba], 0x48       ; -> obj3:0x9CBA
        mov    dword ptr [0x9cac], 0x1000    ; -> obj3:0x9CAC
        mov    byte ptr [0x9cef], 4          ; -> loc_3_9CEF
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        mov    dword ptr [0x9d40], 0x9d50    ; -> obj3:0x9D40
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x9c78], eax       ; -> obj3:0x9C78
        push   0x9c8c                        ; -> obj3:0x9C8C
        call   sub_3_84F0
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0720:
        inc    ebx
        inc    esp
        inc    esi
        push   ebx
        and    byte ptr [edx + edx*2 + 0x50], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        pushal 
        add    eax, 0x8a80000                ; -> sub_1_08A8
        add    byte ptr [eax], al
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0x79a
        arpl   word ptr [eax + 0x44], si
        imul   esi, dword ptr [ebx + 0x63], 0x44534620
        add    dword ptr [eax], eax
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xa364  R-X
sub_3_0000:
        test   byte ptr [0x87c], 0x80        ; -> obj1:0x087C
        jne    loc_3_00A2
        or     byte ptr [0x87c], 0x40        ; -> obj1:0x087C
        test   byte ptr [0x87c], 0x20        ; -> obj1:0x087C
        je     loc_3_002C
        push   0x8dc                         ; -> loc_1_08DC
        VxDCall  VCDFSD.Service_0003h            ; cd2003004100
        lea    esp, [esp + 4]
loc_3_002C:
        and    dword ptr [4], 0xfffffffe     ; -> obj4:0x0004
        test   dword ptr [4], 0x20           ; -> obj4:0x0004
        je     loc_3_0050
        test   dword ptr [4], 0x40           ; -> obj4:0x0004
        je     loc_3_0050
        call   sub_1_0320
loc_3_0050:
        test   dword ptr [4], 0x2000         ; -> obj4:0x0004
        je     loc_3_006B
        or     dword ptr [4], 0x1000         ; -> obj4:0x0004
        call   sub_1_03D0
loc_3_006B:
        call   sub_3_0450
        test   byte ptr [0x87c], 4           ; -> obj1:0x087C
        je     loc_3_0088
        push   dword ptr [0x898]             ; -> obj1:0x0898
        call   sub_3_86F0
        lea    esp, [esp + 4]
loc_3_0088:
        test   byte ptr [0x87c], 2           ; -> obj1:0x087C
        je     loc_3_009B
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        and    word ptr [eax + 4], 0xfffd
loc_3_009B:
        and    byte ptr [0x87c], 0x10        ; -> obj1:0x087C
loc_3_00A2:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_00B0:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x28], 0
        push   1
        mov    eax, dword ptr [0x898]        ; -> obj1:0x0898
        push   eax
        call   sub_3_8710
        mov    edi, dword ptr [esp + 0x38]
        add    esp, 8
loc_3_00D3:
        mov    ecx, dword ptr [0x9be0]       ; -> obj3:0x9BE0
        lea    esp, [esp]
loc_3_00E0:
        mov    eax, dword ptr [esp + 0x28]
        inc    dword ptr [esp + 0x28]
        or     eax, eax
        je     loc_3_0123
        cmp    eax, 1
        jne    loc_3_02E0
        or     edi, edi
        je     loc_3_0105
        mov    dword ptr [0x9c0c], 0x1b      ; -> loc_3_9C0C
        jmp    loc_3_0123
loc_3_0105:
        mov    dword ptr [0x9c00], 0x20      ; -> loc_3_9C00
        mov    dword ptr [0x9c04], 0x1b      ; -> loc_3_9C04
        mov    dword ptr [0x9c08], 2         ; -> loc_3_9C08
loc_3_0123:
        or     edi, edi
        je     loc_3_0145
        mov    dword ptr [esp + 0x10], 0x930
        mov    ebx, dword ptr [0x9c0c]       ; -> loc_3_9C0C
        mov    edx, ebx
        mov    dword ptr [esp + 0x1c], edx
        mov    dword ptr [esp + 0x24], 2
        jmp    loc_3_0167
loc_3_0145:
        mov    dword ptr [esp + 0x10], 0x800
        mov    ebx, dword ptr [0x9c00]       ; -> loc_3_9C00
        mov    edx, dword ptr [0x9c04]       ; -> loc_3_9C04
        mov    dword ptr [esp + 0x1c], edx
        mov    edx, dword ptr [0x9c08]       ; -> loc_3_9C08
        mov    dword ptr [esp + 0x24], edx
loc_3_0167:
        or     edi, edi
        je     loc_3_0180
        or     ecx, ecx
        jne    loc_3_02FB
        mov    dword ptr [esp + 0x20], 0x9bdc ; -> obj3:0x9BDC
        jmp    loc_3_0188
        db  08Dh, 049h, 000h
loc_3_0180:
        mov    dword ptr [esp + 0x20], 0x9bd8 ; -> obj3:0x9BD8
loc_3_0188:
        imul   ebx, dword ptr [esp + 0x10]
        add    ebx, 0xfff
        shr    ebx, 0xc
        mov    eax, ebx
        shl    eax, 0xc
        sub    edx, edx
        div    dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x14], eax
        mov    ebp, 1
        shr    eax, 2
        push   eax
        call   sub_3_6860
        add    esp, 4
        mov    cl, al
        shl    ebp, cl
        cmp    ebp, 0x10
        jae    loc_3_01C4
        mov    ebp, 0x10
loc_3_01C4:
        mov    ecx, dword ptr [esp + 0x14]
        lea    eax, [ecx + ecx*8]
        lea    eax, [eax + ebx*2]
        add    eax, ebp
        lea    eax, [eax*4 + 0xa4]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x20]
        sub    eax, 0x9bdc                   ; -> obj3:0x9BDC
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xffffff00
        add    eax, 0x101
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        call   sub_3_43B0
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        mov    ecx, dword ptr [0x9be0]       ; -> obj3:0x9BE0
        je     loc_3_00E0
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x44], eax
        mov    ax, word ptr [esp + 0x10]
        mov    word ptr [esi + 0x40], ax
        lea    eax, [ebp - 1]
        mov    dword ptr [esi + 0x6c], eax
        lea    eax, [esi + ebp*4 + 0x80]
        mov    dword ptr [esi + 4], eax
        lea    eax, [eax + ebx*8]
        mov    dword ptr [esi], eax
        or     edi, edi
        jne    loc_3_025B
        mov    eax, dword ptr [esp + 0x18]
        push   0
        add    eax, 0xfff
        shr    eax, 0xc
        push   eax
        mov    eax, esi
        shr    eax, 0xc
        push   eax
        call   sub_3_5150
        add    esp, 0xc
        or     eax, eax
        je     loc_3_02CD
loc_3_025B:
        mov    dword ptr [esi + 0x64], ebx
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   ebx
        call   sub_3_04E0
        add    esp, 0x20
        mov    dword ptr [esi + 0x68], eax
        or     eax, eax
        jne    loc_3_02A0
        or     edi, edi
        jne    loc_3_02CD
        mov    eax, dword ptr [esp + 0x18]
        push   0
        add    eax, 0xfff
        shr    eax, 0xc
        push   eax
        mov    eax, esi
        shr    eax, 0xc
        push   eax
        call   sub_3_5180
        add    esp, 0xc
        jmp    loc_3_02CD
        db  090h
loc_3_02A0:
        or     edi, edi
        je     loc_3_0360
        push   0
        push   ebx
        shr    eax, 0xc
        push   eax
        call   sub_3_5150
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_0347
        push   0
        mov    eax, dword ptr [esi + 0x68]
        push   eax
        call   sub_3_5809
        add    esp, 8
loc_3_02CD:
        push   0
        push   0
        push   esi
        call   sub_3_8750
        add    esp, 0xc
        jmp    loc_3_00D3
        db  090h
loc_3_02E0:
        mov    eax, dword ptr [0x898]        ; -> obj1:0x0898
        push   eax
        call   sub_3_8700
        add    esp, 4
        mov    eax, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_3_02FB:
        push   0
        mov    eax, dword ptr [ecx + 0x64]
        push   eax
        mov    eax, dword ptr [ecx + 0x68]
        shr    eax, 0xc
        push   eax
        call   sub_3_5150
        add    esp, 0xc
        or     eax, eax
        mov    esi, 8
        je     loc_3_032F
        mov    eax, dword ptr [0x9be0]       ; -> obj3:0x9BE0
        mov    dword ptr [0x9bdc], eax       ; -> obj3:0x9BDC
        mov    dword ptr [0x9be0], 0         ; -> obj3:0x9BE0
        sub    esi, esi
loc_3_032F:
        mov    eax, dword ptr [0x898]        ; -> obj1:0x0898
        push   eax
        call   sub_3_8700
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
loc_3_0347:
        or     ebx, ebx
        je     loc_3_0360
        sub    eax, eax
        mov    ecx, 1
loc_3_0352:
        mov    edx, dword ptr [esi + 4]
        mov    word ptr [eax + edx + 6], cx
        add    eax, 8
        dec    ebx
        jne    loc_3_0352
loc_3_0360:
        mov    ecx, dword ptr [esp + 0x14]
        cmp    dword ptr [esp + 0x1c], ecx
        jbe    loc_3_036E
        mov    dword ptr [esp + 0x1c], ecx
loc_3_036E:
        mov    edx, dword ptr [esp + 0x1c]
        cmp    dword ptr [esp + 0x24], edx
        jbe    loc_3_037C
        mov    dword ptr [esp + 0x24], edx
loc_3_037C:
        mov    edx, dword ptr [esi]
        mov    edi, dword ptr [esi + 0x68]
        sub    ecx, ecx
        lea    ebx, [edx + 0x1c]
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, dword ptr [esp + 0x1c]
        mov    edi, edi
loc_3_0390:
        lea    ebp, [ebx + 8]
        mov    dword ptr [ebx - 0x1c], ebp
        lea    ebp, [ebx - 0x40]
        mov    dword ptr [ebx - 0x18], ebp
        mov    dword ptr [ebx + 4], edi
        cmp    ecx, eax
        mov    byte ptr [ebx], 5
        jb     loc_3_03A9
        mov    byte ptr [ebx], 1
loc_3_03A9:
        add    edi, dword ptr [esp + 0x10]
        add    ebx, 0x24
        inc    ecx
        cmp    dword ptr [esp + 0x14], ecx
        jae    loc_3_0390
        mov    ecx, dword ptr [esp + 0x14]
        lea    eax, [ecx*4]
        mov    dword ptr [edx + 4], 0
        lea    eax, [eax + eax*8]
        add    eax, edx
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 0x1c], 0
        mov    dword ptr [eax + 0x20], 0xffffffff
        mov    dword ptr [esi + 8], eax
        mov    eax, ecx
        sub    eax, dword ptr [esp + 0x1c]
        lea    ecx, [eax*4]
        mov    dword ptr [esi + 0x38], edx
        lea    ecx, [ecx + ecx*8]
        add    ecx, edx
        mov    dword ptr [esi + 0x3c], ecx
        mov    dword ptr [esi + 0x34], edx
        mov    dword ptr [esi + 0x30], edx
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0xc], ecx
        mov    edx, dword ptr [esp + 0x1c]
        shr    eax, 2
        mov    dword ptr [esi + 0x28], edx
        mov    dword ptr [esi + 0x1c], edx
        mov    edx, dword ptr [esp + 0x24]
        mov    dword ptr [esi + 0x2c], edx
        mov    dword ptr [esi + 0x60], ecx
        mov    dword ptr [esi + 0x5c], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x58], 0x1f4
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [0x898]        ; -> obj1:0x0898
        push   eax
        call   sub_3_8700
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0450:
        sub    esp, 8
        xor    eax, eax
        xchg   dword ptr [0x9bdc], eax       ; -> obj3:0x9BDC
        mov    dword ptr [esp], eax
        or     eax, eax
        jne    loc_3_046F
        xor    eax, eax
        xchg   dword ptr [0x9be0], eax       ; -> obj3:0x9BE0
        mov    dword ptr [esp], eax
loc_3_046F:
        xor    eax, eax
        xchg   dword ptr [0x9bd8], eax       ; -> obj3:0x9BD8
        mov    dword ptr [esp + 4], eax
        cmp    dword ptr [esp], 0
        je     loc_3_04A9
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x68]
        or     eax, eax
        je     loc_3_0498
        push   0
        push   eax
        call   sub_3_5809
        add    esp, 8
loc_3_0498:
        push   0
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0xc
loc_3_04A9:
        cmp    dword ptr [esp + 4], 0
        je     loc_3_04D7
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x68]
        or     eax, eax
        je     loc_3_04C6
        push   0
        push   eax
        call   sub_3_5809
        add    esp, 8
loc_3_04C6:
        push   0
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0xc
loc_3_04D7:
        add    esp, 8
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_3_04E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0520:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0x53464443
        mov    ecx, dword ptr [esp + 0x10]
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x18]
        add    ecx, 0x18
        or     eax, eax
        je     loc_3_0556
        cmp    eax, 2
        je     loc_3_0560
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_3_3C40
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_3_0556:
        mov    word ptr [ecx], 0
        jmp    loc_3_058A
        db  08Dh, 049h, 000h
loc_3_0560:
        xor    eax, eax
        mov    edx, dword ptr [esp + 0x10]
        lea    esi, [esp + 8]
        mov    edi, dword ptr [edx + 0x14]
        mov    ecx, 5
        cmp    ecx, dword ptr [edx]
        jbe    loc_3_057A
        mov    ecx, dword ptr [edx]
        jecxz  loc_3_057E
loc_3_057A:
        dec    ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        stosb  byte ptr es:[edi], al
loc_3_057E:
        mov    dword ptr [edx], 0x10400ff
        mov    word ptr [edx + 0x18], 0x4002
loc_3_058A:
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x1a], 0
        mov    eax, dword ptr [esp + 0x10]
        pop    edi
        movsx  eax, word ptr [eax + 0x1a]
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_05B0:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [0x88c]        ; -> obj1:0x088C
        mov    dword ptr [edx + 1], ecx
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax + 3], 0x100
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_05D0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 0x30], 0x4000
        mov    word ptr [eax + 0x28], 0xd
        mov    word ptr [eax + 0x2a], 0xcd17
        mov    ecx, dword ptr [eax + 0x18]
        inc    ecx
        mov    dword ptr [eax + 0x20], ecx
        call   sub_3_37B0
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0600:
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        lea    eax, [esp + 4]
        or     dword ptr [esi + 0x30], 0x4000
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd09
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0xc
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_068E
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 8]
        mov    byte ptr [eax + 1], cl
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 9]
        mov    byte ptr [eax + 2], cl
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 0xa]
        mov    byte ptr [eax + 3], cl
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 0xb]
        mov    byte ptr [eax + 4], cl
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 0xc]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 0xd]
        mov    byte ptr [eax + 6], cl
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 0xe]
        mov    byte ptr [eax + 7], cl
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 0xf]
        mov    byte ptr [eax + 8], cl
loc_3_068E:
        pop    esi
        add    esp, 0xc
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_06A0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 0x30], 0x4000
        mov    word ptr [eax + 0x28], 0xd
        mov    word ptr [eax + 0x2a], 0xcd16
        mov    ecx, dword ptr [eax + 0x18]
        inc    ecx
        mov    dword ptr [eax + 0x20], ecx
        call   sub_3_37B0
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_06D0:
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        lea    eax, [esp + 4]
        or     dword ptr [esi + 0x30], 0x4000
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd03
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0xc
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esi + 0x18]
        push   esi
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        mov    dword ptr [esp + 0xc], eax
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_0728
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esp + 0xc]
        mov    word ptr [eax + 2], cx
loc_3_0728:
        pop    esi
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_3_0730:
        sub    esp, 0x20
        push   esi
        mov    esi, dword ptr [esp + 0x28]
        lea    eax, [esp + 0xc]
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd0b
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0x18
        mov    dword ptr [esp + 0xc], 0
        mov    byte ptr [esp + 0x1e], 0xff
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_07B2
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 8], eax
        xor    ecx, ecx
        mov    cl, byte ptr [esp + 0xa]
        shl    ecx, 2
        xor    eax, eax
        lea    ecx, [ecx + ecx*2]
        mov    al, byte ptr [esp + 9]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        xor    eax, eax
        lea    ecx, [ecx + ecx*4]
        mov    al, byte ptr [esp + 8]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        sub    ecx, 0x96
        mov    dword ptr [esp + 8], ecx
        mov    eax, dword ptr [esi + 0x18]
        add    ecx, 0x96
        mov    dword ptr [eax + 1], ecx
loc_3_07B2:
        pop    esi
        add    esp, 0x20
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_07C0:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        jne    loc_3_0821
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0x10]
        btr    dword ptr [0x9bf4], eax       ; -> loc_3_9BF4
        mov    al, 0xff
        jb     loc_3_07E8
        mov    al, 1
loc_3_07E8:
        mov    byte ptr [esp + 3], al
        mov    cl, byte ptr [esp + 3]
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0x18]
        mov    byte ptr [eax + 1], cl
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax + 3], 0x100
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_3_3160
        add    esp, 4
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_3_36C0
        add    esp, 4
loc_3_0821:
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0830:
        sub    esp, 0x18
        push   esi
        mov    esi, dword ptr [esp + 0x20]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd0b
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0x18
        mov    dword ptr [esp + 4], 0
        mov    byte ptr [esp + 0x16], 0xff
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_088F
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 8]
        mov    byte ptr [eax + 1], cl
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 0xc]
        mov    byte ptr [eax + 2], cl
        mov    byte ptr [esp + 0x13], 0
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 3], ecx
loc_3_088F:
        pop    esi
        add    esp, 0x18
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_08A0:
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd0c
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0x10
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esi + 0x18]
        push   esi
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        mov    dword ptr [esp + 0xc], eax
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_08FF
        mov    byte ptr [esp + 0xf], 0
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    cl, byte ptr [esp + 0x10]
        mov    byte ptr [eax + 6], cl
loc_3_08FF:
        pop    esi
        add    esp, 0x10
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0910:
        sub    esp, 0x14
        push   esi
        mov    esi, dword ptr [esp + 0x1c]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd0d
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0x14
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esi + 0x18]
        mov    eax, dword ptr [eax + 1]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    eax, dword ptr [eax + 9]
        mov    dword ptr [esp + 0xc], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    eax, dword ptr [eax + 5]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_3_5958
        mov    dword ptr [esp + 0x18], eax
        mov    byte ptr [esp + 0x1c], 0
        mov    byte ptr [esp + 0x1d], 0
        mov    byte ptr [esp + 0x1e], 0
        mov    byte ptr [esp + 0x1f], 0
        add    esp, 8
        push   esi
        call   sub_3_37B0
        add    esp, 4
        pop    esi
        add    esp, 0x14
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0990:
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd05
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0x10
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_09E1
        mov    edx, dword ptr [esi + 0x18]
        lea    ecx, [esp + 8]
        inc    edx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [edx + 8], ax
loc_3_09E1:
        pop    esi
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_09F0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 0x30], 0x4000
        mov    word ptr [eax + 0x28], 8
        mov    word ptr [eax + 0x2a], 0
        mov    dword ptr [eax + 0x20], 0
        call   sub_3_37B0
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0A20:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x1c]
        cmp    byte ptr [ecx + 1], 0
        je     loc_3_0A35
        mov    word ptr [eax + 0x28], 9
        jmp    loc_3_0A42
loc_3_0A35:
        or     dword ptr [eax + 0x30], 0x4000
        mov    word ptr [eax + 0x28], 0xa
loc_3_0A42:
        mov    word ptr [eax + 0x2a], 0
        mov    dword ptr [eax + 0x20], 0
        push   eax
        call   sub_3_37B0
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0A60:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 0x30], 0x4000
        mov    word ptr [eax + 0x28], 0xd
        mov    word ptr [eax + 0x2a], 0xcd00
        mov    dword ptr [eax + 0x20], 0
        mov    dword ptr [eax + 0x24], 0
        call   sub_3_37B0
        add    esp, 4
        ret    
        db  090h
loc_3_0A90:
        sub    esp, 0xc
        mov    ecx, dword ptr [esp + 0x10]
        lea    eax, [esp]
        or     dword ptr [ecx + 0x30], 0x4000
        mov    word ptr [ecx + 0x28], 0xd
        mov    word ptr [ecx + 0x2a], 0xcd0a
        mov    dword ptr [ecx + 0x20], eax
        mov    dword ptr [ecx + 0x24], 0xc
        mov    dword ptr [esp], 0
        mov    eax, dword ptr [ecx + 0x1c]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [esp + 4], al
        mov    eax, dword ptr [ecx + 0x1c]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [esp + 5], al
        mov    eax, dword ptr [ecx + 0x1c]
        mov    al, byte ptr [eax + 3]
        mov    byte ptr [esp + 6], al
        mov    eax, dword ptr [ecx + 0x1c]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [esp + 7], al
        mov    eax, dword ptr [ecx + 0x1c]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [esp + 8], al
        mov    eax, dword ptr [ecx + 0x1c]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [esp + 9], al
        mov    eax, dword ptr [ecx + 0x1c]
        push   ecx
        mov    al, byte ptr [eax + 7]
        mov    byte ptr [esp + 0xe], al
        mov    eax, dword ptr [ecx + 0x1c]
        mov    al, byte ptr [eax + 8]
        mov    byte ptr [esp + 0xf], al
        call   sub_3_37B0
        add    esp, 0x10
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0B20:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 0x30], 0x4000
        mov    word ptr [eax + 0x28], 0xd
        mov    word ptr [eax + 0x2a], 0xcd18
        mov    ecx, dword ptr [eax + 0x1c]
        inc    ecx
        mov    dword ptr [eax + 0x20], ecx
        call   sub_3_37B0
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0B50:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 0x30], 0x4000
        mov    word ptr [eax + 0x28], 7
        mov    word ptr [eax + 0x2a], 0
        mov    dword ptr [eax + 0x20], 0
        call   sub_3_37B0
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0B80:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        jne    loc_3_0BAD
        mov    eax, dword ptr [esi + 0x14]
        push   esi
        mov    word ptr [eax + 3], 0x8103
        call   sub_3_3160
        add    esp, 4
        push   esi
        call   sub_3_36C0
        add    esp, 4
loc_3_0BAD:
        pop    esi
        ret    
        db  090h
loc_3_0BB0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        jne    loc_3_0BDD
        mov    eax, dword ptr [esi + 0x14]
        push   esi
        mov    word ptr [eax + 3], 0x100
        call   sub_3_3160
        add    esp, 4
        push   esi
        call   sub_3_36C0
        add    esp, 4
loc_3_0BDD:
        pop    esi
        ret    
        db  090h
loc_3_0BE0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x14]
        mov    word ptr [eax + 3], 0x100
        ret    
        db  08Bh, 0FFh
loc_3_0BF0:
        sub    esp, 0xc
        mov    ecx, dword ptr [esp + 0x10]
        lea    eax, [esp]
        mov    word ptr [ecx + 0x28], 0xd
        mov    word ptr [ecx + 0x2a], 0xcd01
        mov    dword ptr [ecx + 0x20], eax
        mov    dword ptr [ecx + 0x24], 0xc
        mov    dword ptr [esp], 0
        mov    eax, dword ptr [ecx + 0x14]
        push   ecx
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [ecx + 0x14]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [esp + 0xc], eax
        call   sub_3_37B0
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_0C40:
        sub    esp, 0x20
        push   esi
        mov    esi, dword ptr [esp + 0x28]
        push   esi
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        jne    loc_3_0DC4
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd0e
        lea    eax, [esp + 0x10]
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0x14
        mov    dword ptr [esp + 0x10], 0
        mov    dword ptr [esp + 0x20], 0
        mov    eax, dword ptr [esi + 0x14]
        mov    al, byte ptr [eax + 0xd]
        and    eax, 0xff
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    eax, dword ptr [eax + 0xe]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0xc], eax
        cmp    dword ptr [esp + 8], 0
        je     loc_3_0CED
        xor    ecx, ecx
        mov    cl, byte ptr [esp + 0xe]
        shl    ecx, 2
        mov    dword ptr [esp + 8], 0
        lea    ecx, [ecx + ecx*2]
        xor    eax, eax
        mov    al, byte ptr [esp + 0xd]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        xor    eax, eax
        lea    ecx, [ecx + ecx*4]
        mov    al, byte ptr [esp + 0xc]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        sub    ecx, 0x96
        mov    dword ptr [esp + 0xc], ecx
loc_3_0CED:
        mov    eax, dword ptr [esi + 0x14]
        mov    eax, dword ptr [eax + 0x12]
        dec    eax
        add    dword ptr [esp + 0xc], eax
        cmp    dword ptr [esp + 0x14], 1
        jne    loc_3_0D5E
        mov    dword ptr [esp + 8], 1
        add    dword ptr [esp + 0xc], 0x96
        mov    ecx, 0x4b
        mov    eax, dword ptr [esp + 0xc]
        sub    edx, edx
        div    ecx
        mov    byte ptr [esp + 4], dl
        mov    eax, dword ptr [esp + 0xc]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 0xc], eax
        mov    ecx, 0x3c
        sub    edx, edx
        div    ecx
        mov    byte ptr [esp + 5], dl
        mov    eax, dword ptr [esp + 0xc]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 0xc], eax
        mov    byte ptr [esp + 0xf], 0
        mov    byte ptr [esp + 0xe], al
        mov    al, byte ptr [esp + 5]
        mov    byte ptr [esp + 0xd], al
        mov    al, byte ptr [esp + 4]
        mov    byte ptr [esp + 0xc], al
loc_3_0D5E:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [eax + 0x12], 0
        je     loc_3_0D80
        push   esi
        call   sub_3_30C0
        add    esp, 4
        jmp    loc_3_0D87
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0D80:
        mov    dword ptr [esi + 0x2c], 0
loc_3_0D87:
        cmp    dword ptr [esi + 0x2c], 0
        jne    loc_3_0D98
        mov    eax, dword ptr [esi + 0x14]
        mov    word ptr [eax + 3], 0x100
        jmp    loc_3_0DB2
loc_3_0D98:
        cmp    dword ptr [esi + 0x2c], 0x57
        jne    loc_3_0DA9
        mov    eax, dword ptr [esi + 0x14]
        mov    word ptr [eax + 3], 0x810d
        jmp    loc_3_0DB2
loc_3_0DA9:
        push   esi
        call   sub_3_3820
        add    esp, 4
loc_3_0DB2:
        push   esi
        call   sub_3_3160
        add    esp, 4
        push   esi
        call   sub_3_36C0
        add    esp, 4
loc_3_0DC4:
        pop    esi
        add    esp, 0x20
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0DD0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    word ptr [eax + 0x28], 0xd
        mov    word ptr [eax + 0x2a], 0xcd0f
        mov    dword ptr [eax + 0x20], 0
        mov    dword ptr [eax + 0x24], 0
        call   sub_3_37B0
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0E00:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    word ptr [eax + 0x28], 0xd
        mov    word ptr [eax + 0x2a], 0xcd10
        mov    dword ptr [eax + 0x20], 0
        mov    dword ptr [eax + 0x24], 0
        call   sub_3_37B0
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0E30:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi]
        cmp    dword ptr [edi + 0x28], 1
        jne    loc_3_0E57
        or     dword ptr [esi + 0x20], 0x80000
        mov    eax, dword ptr [edi]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        lea    eax, [ecx + eax*2]
        shl    eax, 4
        jmp    loc_3_0E5C
loc_3_0E57:
        mov    eax, dword ptr [edi]
        shl    eax, 0xb
loc_3_0E5C:
        mov    dword ptr [esi + 0xa0], eax
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [esi + 0x98], eax
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0x1c]
        or     eax, eax
        mov    dword ptr [esi + 0x14], eax
        mov    ecx, dword ptr [edi + 0x14]
        mov    dword ptr [esi + 0x9c], ecx
        mov    dword ptr [esi + 0xa4], 0
        mov    dword ptr [esi + 0x24], 0
        je     loc_3_0EA0
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x44], eax
        jmp    loc_3_0ECE
        db  090h
loc_3_0EA0:
        add    edi, 0x40
        mov    dword ptr [esi + 0x44], edi
        mov    dword ptr [edi], 0x10
        mov    byte ptr [edi + 4], 1
        mov    byte ptr [edi + 5], 1
        mov    byte ptr [edi + 6], 1
        mov    byte ptr [edi + 7], 1
        call   sub_3_7720
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x44]
        mov    dword ptr [eax + 0xc], 0xb
loc_3_0ECE:
        push   0x42
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_0EE0:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        cmp    dword ptr [ecx + 0x24], 0
        jne    loc_3_0F88
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [eax + 0x3c], 5
        jne    loc_3_0F8F
        mov    edi, dword ptr [ecx + 0x4c]
        mov    eax, 0xa334                   ; -> loc_3_A334
        lea    edx, [edi + 9]
        mov    esi, dword ptr [eax]
        cmp    esi, dword ptr [edx]
        jne    loc_3_0F14
        mov    al, byte ptr [eax + 4]
        cmp    al, byte ptr [edx + 4]
loc_3_0F14:
        mov    edx, 0
        sete   dl
        mov    eax, 0xa33c                   ; -> loc_3_A33C
        lea    ebx, [edi + 1]
        mov    esi, dword ptr [eax]
        cmp    esi, dword ptr [ebx]
        jne    loc_3_0F30
        mov    al, byte ptr [eax + 4]
        cmp    al, byte ptr [ebx + 4]
loc_3_0F30:
        mov    eax, 0
        sete   al
        or     eax, eax
        jne    loc_3_0F4D
        or     edx, edx
        jne    loc_3_0F49
        mov    dword ptr [ecx + 0x24], 0xb
        jmp    loc_3_0F8F
loc_3_0F49:
        or     eax, eax
        je     loc_3_0F53
loc_3_0F4D:
        xor    eax, eax
        mov    al, byte ptr [edi]
        jmp    loc_3_0F58
loc_3_0F53:
        xor    eax, eax
        mov    al, byte ptr [edi + 8]
loc_3_0F58:
        cmp    eax, 1
        je     loc_3_0F71
        cmp    eax, 0xff
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 0x38]
        je     loc_3_0F80
        mov    word ptr [eax + 0x1c], 0
        jmp    loc_3_0F8F
loc_3_0F71:
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 0x38]
        mov    word ptr [eax + 0x1c], 1
        jmp    loc_3_0F8F
        db  08Bh, 0FFh
loc_3_0F80:
        mov    word ptr [eax + 0x1c], 0xff
        jmp    loc_3_0F8F
loc_3_0F88:
        mov    dword ptr [ecx + 0x24], 0x15
loc_3_0F8F:
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 0x24]
        mov    word ptr [edx + 0x1a], ax
        cmp    dword ptr [ecx + 0x14], 0
        jne    loc_3_0FAD
        mov    eax, dword ptr [ecx + 0x44]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
loc_3_0FAD:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0FC0:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax], 0
        je     loc_3_0FE0
loc_3_0FC9:
        mov    cl, byte ptr [eax]
        cmp    cl, 0x61
        jb     loc_3_0FDA
        cmp    cl, 0x7a
        ja     loc_3_0FDA
        sub    cl, 0x20
        mov    byte ptr [eax], cl
loc_3_0FDA:
        inc    eax
        cmp    byte ptr [eax], 0
        jne    loc_3_0FC9
loc_3_0FE0:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0FF0:
        sub    esp, 0x78
        push   esi
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [esp + 0x80]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [esp + 0x88]
        push   eax
        mov    eax, dword ptr [esp + 0xc]
        push   esi
        push   eax
        call   sub_3_9AC0
        add    esp, 0x10
        or     eax, eax
        je     loc_3_1030
        mov    eax, 1
        pop    esi
        add    esp, 0x78
        ret    
        db  08Dh, 049h, 000h
loc_3_1030:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0x1a], 0
        jne    loc_3_10A0
        push   0x3810
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 0x14], eax
        push   0x3e
        mov    eax, dword ptr [esp + 8]
        push   5
        push   eax
        call   sub_3_55E0
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 0xc
        push   eax
        call   sub_3_9BA0
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        cmp    word ptr [eax + 0x1a], 0
        jne    loc_3_1083
        and    dword ptr [esi + 0x2c], 0xfffffffe
        sub    eax, eax
        pop    esi
        add    esp, 0x78
        ret    
loc_3_1083:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x1a]
        or     dword ptr [esi + 0x2c], 1
        mov    word ptr [esi + 0x1c], ax
        sub    eax, eax
        pop    esi
        add    esp, 0x78
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_10A0:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_3_10B7
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_9BA0
        add    esp, 4
loc_3_10B7:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x1a]
        or     dword ptr [esi + 0x2c], 1
        mov    word ptr [esi + 0x1c], ax
        sub    eax, eax
        pop    esi
        add    esp, 0x78
        ret    
        db  08Bh, 0FFh
loc_3_10D0:
        sub    esp, 0x78
        push   esi
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [esp + 0x80]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [esp + 0x88]
        push   eax
        mov    eax, dword ptr [esp + 0xc]
        push   esi
        push   eax
        call   sub_3_9AC0
        add    esp, 0x10
        or     eax, eax
        je     loc_3_1110
        mov    eax, 1
        pop    esi
        add    esp, 0x78
        ret    
        db  08Dh, 049h, 000h
loc_3_1110:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0x1a], 0
        jne    loc_3_11B0
        push   0x3810
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax], 1
        xor    eax, eax
        mov    ax, word ptr [esi + 0x14]
        add    eax, 0x10
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [esp + 4]
        push   0x40
        mov    dword ptr [eax + 0x28], 0
        mov    eax, dword ptr [esp + 8]
        push   5
        mov    dword ptr [eax + 0x3c], 5
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        call   sub_3_55E0
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 0xc
        push   eax
        call   sub_3_9BA0
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        cmp    word ptr [eax + 0x1a], 0
        jne    loc_3_1197
        and    dword ptr [esi + 0x2c], 0xfffffffe
        sub    eax, eax
        pop    esi
        add    esp, 0x78
        ret    
loc_3_1197:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x1a]
        or     dword ptr [esi + 0x2c], 1
        mov    word ptr [esi + 0x1c], ax
        sub    eax, eax
        pop    esi
        add    esp, 0x78
        ret    
        db  08Bh, 0FFh
loc_3_11B0:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_3_11C7
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_9BA0
        add    esp, 4
loc_3_11C7:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0x1a], 0xf
        je     loc_3_11DC
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0x15
loc_3_11DC:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x1a]
        or     dword ptr [esi + 0x2c], 1
        mov    word ptr [esi + 0x1c], ax
        sub    eax, eax
        pop    esi
        add    esp, 0x78
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1200:
        mov    eax, dword ptr [esp + 4]
        and    dword ptr [eax + 0x2c], 0xfffffffe
        sub    eax, eax
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_1210:
        sub    esp, 0x78
        push   esi
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [esp + 0x80]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [esp + 0x88]
        push   eax
        mov    eax, dword ptr [esp + 0xc]
        push   esi
        push   eax
        call   sub_3_9AC0
        add    esp, 0x10
        or     eax, eax
        je     loc_3_1250
        mov    eax, 1
        pop    esi
        add    esp, 0x78
        ret    
        db  08Dh, 049h, 000h
loc_3_1250:
        push   0x3810
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 0x14], eax
        xor    eax, eax
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [esi + 0x14]
        mov    dword ptr [ecx], eax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        xor    eax, eax
        mov    ax, word ptr [esi]
        shl    ecx, 0x10
        add    ecx, eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x24], ecx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x28], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x3c], 0
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_3_12F4
        push   0x40
        mov    eax, dword ptr [esp + 8]
        push   5
        push   eax
        call   sub_3_55E0
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 0xc
        cmp    word ptr [eax + 0x1a], 0
        jne    loc_3_12D0
        mov    word ptr [esi + 0x1c], 0
        and    dword ptr [esi + 0x2c], 0xfffffffe
        jmp    loc_3_12E0
        db  090h
loc_3_12D0:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x1a]
        or     dword ptr [esi + 0x2c], 1
        mov    word ptr [esi + 0x1c], ax
loc_3_12E0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_9BA0
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 0x78
        ret    
loc_3_12F4:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0x1a], 0xf
        je     loc_3_1309
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0x15
loc_3_1309:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x1a]
        or     dword ptr [esi + 0x2c], 1
        mov    word ptr [esi + 0x1c], ax
        sub    eax, eax
        pop    esi
        add    esp, 0x78
        ret    
loc_3_1320:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 0x2c], 1
        mov    word ptr [eax + 0x1c], 1
        sub    eax, eax
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1340:
        mov    eax, dword ptr [esp + 4]
        and    dword ptr [eax + 0x2c], 0xfffffffe
        sub    eax, eax
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_1350:
        push   esi
        mov    esi, dword ptr [esp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x18]
        mov    ecx, dword ptr [0x890]        ; -> obj1:0x0890
        lea    edx, [eax + eax*2]
        lea    edx, [ecx + edx*2 + 0x14]
        cmp    eax, 0x1a
        jae    loc_3_1431
        mov    cl, byte ptr [edx + 1]
        test   cl, 8
        je     loc_3_1431
        test   cl, 0x10
        jne    loc_3_1388
        mov    eax, 1
        pop    esi
        ret    
loc_3_1388:
        xor    eax, eax
        mov    ax, word ptr [esi + 0x10]
        or     eax, eax
        je     loc_3_13A5
        cmp    eax, 1
        je     loc_3_13DA
        mov    word ptr [esi + 0x1c], 1
        or     dword ptr [esi + 0x2c], 1
        sub    eax, eax
        pop    esi
        ret    
loc_3_13A5:
        and    ecx, 7
        cmp    ecx, 1
        je     loc_3_13C0
        cmp    ecx, 2
        je     loc_3_13D0
        mov    word ptr [esi + 0x14], 0x100
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_13C0:
        mov    word ptr [esi + 0x14], 0x201
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_13D0:
        mov    word ptr [esi + 0x14], 0x202
        sub    eax, eax
        pop    esi
        ret    
loc_3_13DA:
        xor    eax, eax
        and    dword ptr [esi + 0x2c], 0xfffffffe
        mov    ax, word ptr [esi + 0x14]
        cmp    eax, 0x100
        je     loc_3_1407
        cmp    eax, 0x201
        je     loc_3_1411
        cmp    eax, 0x202
        je     loc_3_1421
        mov    word ptr [esi + 0x1c], 1
        or     dword ptr [esi + 0x2c], 1
        sub    eax, eax
        pop    esi
        ret    
loc_3_1407:
        and    cl, 0xf8
        mov    byte ptr [edx + 1], cl
        sub    eax, eax
        pop    esi
        ret    
loc_3_1411:
        and    cl, 0xf8
        mov    byte ptr [edx + 1], cl
        or     cl, 1
        mov    byte ptr [edx + 1], cl
        sub    eax, eax
        pop    esi
        ret    
loc_3_1421:
        and    cl, 0xf8
        mov    byte ptr [edx + 1], cl
        or     cl, 2
        mov    byte ptr [edx + 1], cl
        sub    eax, eax
        pop    esi
        ret    
loc_3_1431:
        mov    word ptr [esi + 0x1c], 0xf
        or     dword ptr [esi + 0x2c], 1
        sub    eax, eax
        pop    esi
        ret    
        db  090h
loc_3_1440:
        sub    esp, 0x78
        push   esi
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [esp + 0x80]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x18]
        push   eax
        mov    eax, dword ptr [esp + 0x88]
        push   eax
        mov    eax, dword ptr [esp + 0xc]
        push   esi
        push   eax
        call   sub_3_9AC0
        add    esp, 0x10
        or     eax, eax
        je     loc_3_1480
        mov    eax, 1
        pop    esi
        add    esp, 0x78
        ret    
        db  08Dh, 049h, 000h
loc_3_1480:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0x1a], 0
        jne    loc_3_1550
        push   0x3810
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 0xc], eax
        push   0x400
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 0x14], eax
        mov    al, byte ptr [esi + 0x19]
        mov    ecx, dword ptr [esp + 4]
        and    eax, 0xffff00ff
        mov    word ptr [ecx + 0x18], ax
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_3_0FC0
        add    esp, 4
        push   0x44
        mov    eax, dword ptr [esp + 8]
        push   5
        push   eax
        call   sub_3_55E0
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 0xc
        push   eax
        call   sub_3_9BA0
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        cmp    word ptr [eax + 0x1a], 0
        mov    eax, dword ptr [esp + 4]
        jne    loc_3_1520
        mov    ax, word ptr [eax + 0x18]
        and    dword ptr [esi + 0x2c], 0xfffffffe
        mov    word ptr [esi + 0x1c], ax
        mov    byte ptr [esi + 0x19], 0
        sub    eax, eax
        pop    esi
        add    esp, 0x78
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_1520:
        cmp    word ptr [eax + 0x1a], 3
        jne    loc_3_1531
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 2
loc_3_1531:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x1a]
        or     dword ptr [esi + 0x2c], 1
        mov    word ptr [esi + 0x1c], ax
        sub    eax, eax
        mov    byte ptr [esi + 0x19], 0
        pop    esi
        add    esp, 0x78
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_1550:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_3_1567
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_9BA0
        add    esp, 4
loc_3_1567:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0x1a], 0xf
        je     loc_3_157C
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0x15
loc_3_157C:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x1a]
        or     dword ptr [esi + 0x2c], 1
        mov    word ptr [esi + 0x1c], ax
        sub    eax, eax
        mov    byte ptr [esi + 0x19], 0
        pop    esi
        add    esp, 0x78
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_15A0:
        mov    ecx, dword ptr [esp + 4]
        push   0xa
        mov    eax, dword ptr [ecx + 0x14]
        push   ecx
        mov    eax, dword ptr [eax + 0x38]
        mov    dword ptr [ecx + 0x48], eax
        call   sub_3_8380
        add    esp, 8
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_15C0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        cmp    dword ptr [ebx + 0x24], 0
        je     loc_3_15D9
        mov    eax, dword ptr [ebx]
        pop    edi
        mov    word ptr [eax + 0x1a], 0x1e
        pop    esi
        pop    ebx
        ret    
loc_3_15D9:
        mov    eax, dword ptr [ebx]
        mov    esi, dword ptr [ebx + 0x4c]
        mov    edx, dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebx + 0x14]
        movzx  edi, byte ptr [eax + 0x1e]
        cmp    edi, 1
        sbb    ecx, ecx
        and    ecx, 0xfffffffb
        mov    eax, dword ptr [ebx]
        add    ecx, 0x25
        mov    eax, dword ptr [eax + 0x38]
        mov    al, byte ptr [eax + 0x1c]
        and    eax, 0xff
        cmp    eax, 2
        je     loc_3_1630
        cmp    eax, 3
        je     loc_3_1648
        cmp    eax, 4
        je     loc_3_165C
        or     edi, edi
        je     loc_3_1617
        mov    al, byte ptr [esi]
        jmp    loc_3_161A
loc_3_1617:
        mov    al, byte ptr [esi + 8]
loc_3_161A:
        mov    byte ptr [edx], al
        or     edi, edi
        je     loc_3_1627
        add    esi, 0x28
        inc    edx
        dec    ecx
        jmp    loc_3_166D
loc_3_1627:
        add    esi, 0x30
        inc    edx
        dec    ecx
        jmp    loc_3_166D
        db  08Bh, 0FFh
loc_3_1630:
        or     edi, edi
        je     loc_3_1640
        add    esi, 0x2be
        jmp    loc_3_166D
        db  08Dh, 064h, 024h, 000h
loc_3_1640:
        add    esi, 0x2d6
        jmp    loc_3_166D
loc_3_1648:
        or     edi, edi
        je     loc_3_1654
        add    esi, 0x2e3
        jmp    loc_3_166D
loc_3_1654:
        add    esi, 0x2f6
        jmp    loc_3_166D
loc_3_165C:
        or     edi, edi
        je     loc_3_1668
        add    esi, 0x308
        jmp    loc_3_166D
loc_3_1668:
        mov    esi, 0xa360                   ; -> obj3:0xA360
loc_3_166D:
        sub    ebx, ebx
        cmp    ecx, ebx
        je     loc_3_1683
loc_3_1673:
        mov    al, byte ptr [esi + ebx]
        or     al, al
        je     loc_3_1683
        cmp    al, 0x20
        je     loc_3_1683
        inc    ebx
        cmp    ecx, ebx
        ja     loc_3_1673
loc_3_1683:
        or     ebx, ebx
        je     loc_3_1697
        mov    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_1697:
        mov    byte ptr [edx + ebx], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_3_16A0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax + 0x70], 0x10
        mov    eax, dword ptr [esp + 0x14]
        or     dword ptr [eax + 0x20], 0x2000000
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_54A0
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 4
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_3_1795
        mov    eax, dword ptr [esp + 0x14]
        mov    esi, dword ptr [eax]
        mov    ecx, 0x104
        mov    edx, dword ptr [eax + 0x10]
        mov    esi, dword ptr [esi + 0xc]
        lea    edi, [edx + 0x4c]
        mov    dword ptr [esp + 0xc], edx
        xor    eax, eax
loc_3_16F0:
        lodsb  al, byte ptr [esi]
        stosw  word ptr es:[edi], ax
        or     eax, eax
        je     loc_3_1707
        dec    ecx
        jne    loc_3_16F0
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x24], 3
        jmp    loc_3_175C
loc_3_1707:
        lea    esi, [edx + 0x4c]
        lodsw  ax, word ptr [esi]
        mov    edi, esi
        jmp    loc_3_171E
loc_3_1710:
        mov    edi, esi
loc_3_1712:
        lodsw  ax, word ptr [esi]
        or     eax, eax
        je     loc_3_171E
        cmp    ax, 0x5c
        jne    loc_3_1712
loc_3_171E:
        mov    ecx, esi
        sub    ecx, edi
        mov    word ptr [edi - 2], cx
        or     eax, eax
        jne    loc_3_1710
        lea    eax, [edx + 0x4e]
        mov    ecx, esi
        sub    edi, eax
        sub    ecx, eax
        mov    word ptr [edx + 0x4a], di
        mov    word ptr [edx + 0x48], cx
        mov    ebx, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [ebx]
        and    dword ptr [ebx], 0x7fffffff
        shr    ecx, 1
        lea    edi, [edx + 0x4c]
        mov    eax, 0x3f
        repne scasw ax, word ptr es:[edi]
        jne    loc_3_175C
        or     dword ptr [ebx], 0x80000000
loc_3_175C:
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_3_1795
        mov    eax, dword ptr [esp + 0xc]
        push   0x25
        mov    ax, word ptr [eax + 0x48]
        and    eax, 0xfffe
        mov    ecx, dword ptr [esp + 0x10]
        mov    word ptr [eax + ecx + 0x4c], 0
        mov    eax, dword ptr [esp + 0x18]
        mov    byte ptr [eax + 0x6d], 0
        mov    eax, dword ptr [esp + 0x18]
        push   eax
        call   sub_3_8380
        add    esp, 8
loc_3_1795:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_17A0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x1c]
        cmp    dword ptr [ebp + 0x24], 0
        je     loc_3_17C3
        mov    eax, dword ptr [ebp]
        mov    ecx, dword ptr [ebp + 0x24]
        mov    word ptr [eax + 0x1a], cx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_17C3:
        mov    ecx, dword ptr [ebp + 0x14]
        xor    eax, eax
        mov    al, byte ptr [ecx + 0x1e]
        mov    ebx, dword ptr [ebp + 0x90]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [ebp]
        mov    edx, dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp]
        test   byte ptr [eax + 0x18], 1
        je     loc_3_18EA
        mov    al, byte ptr [ebx]
        mov    byte ptr [edx], al
        mov    eax, dword ptr [ebx + 2]
        mov    dword ptr [edx + 1], eax
        mov    ax, 1
        mov    cl, byte ptr [ecx + 0x44]
        shl    ax, cl
        mov    word ptr [edx + 5], ax
        mov    eax, dword ptr [ebx + 0xa]
        mov    dword ptr [edx + 7], eax
        lea    ecx, [edx + 0xb]
        mov    eax, dword ptr [ebx + 0x12]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [esp + 0x10], 0
        mov    ax, word ptr [ebx + 0x16]
        lea    esi, [ebx + 0x12]
        mov    word ptr [ecx + 4], ax
        mov    al, byte ptr [esi + 6]
        mov    byte ptr [ecx + 6], al
        jne    loc_3_1831
        mov    byte ptr [edx + 0x12], 0
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_1836
loc_3_1831:
        mov    al, byte ptr [ebx + 0x19]
        jmp    loc_3_1839
loc_3_1836:
        mov    al, byte ptr [ebx + 0x18]
loc_3_1839:
        mov    byte ptr [edx + 0x12], al
        mov    al, byte ptr [ebx + 0x1a]
        mov    byte ptr [edx + 0x13], al
        mov    al, byte ptr [ebx + 0x1b]
        mov    byte ptr [edx + 0x14], al
        mov    ax, word ptr [ebx + 0x1c]
        mov    word ptr [edx + 0x15], ax
        mov    al, byte ptr [ebx + 0x20]
        mov    ecx, eax
        mov    byte ptr [edx + 0x17], al
        and    ecx, 0xfe
        inc    ecx
        mov    eax, 0xffffffdf
        sub    eax, ebx
        sub    al, cl
        add    al, byte ptr [ebx]
        add    al, bl
        mov    byte ptr [edx + 0x40], al
        and    eax, 0xff
        lea    edi, [edx + 0x41]
        lea    esi, [ecx + ebx + 0x21]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xor    ecx, ecx
        mov    cl, byte ptr [ebx + 0x20]
        lea    eax, [edx + 0x18]
        mov    dword ptr [esp + 0x14], ecx
        lea    esi, [ebx + 0x21]
        mov    edi, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x14]
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xor    ecx, ecx
        mov    cl, byte ptr [ebx + 0x20]
        or     ecx, ecx
        je     loc_3_18BA
loc_3_18B1:
        cmp    byte ptr [eax], 0x3b
        je     loc_3_18BA
        inc    eax
        dec    ecx
        jne    loc_3_18B1
loc_3_18BA:
        mov    byte ptr [eax], 0
        sub    di, di
        or     ecx, ecx
        je     loc_3_1900
        sub    byte ptr [edx + 0x17], cl
        dec    ecx
        mov    esi, ecx
        dec    ecx
        or     esi, esi
        je     loc_3_18E4
        nop    
loc_3_18D0:
        inc    eax
        imul   di, di, 0xa
        movzx  si, byte ptr [eax]
        lea    edi, [edi + esi - 0x30]
        mov    esi, ecx
        dec    ecx
        or     esi, esi
        jne    loc_3_18D0
loc_3_18E4:
        mov    word ptr [edx + 0x3e], di
        jmp    loc_3_1900
loc_3_18EA:
        xor    eax, eax
        mov    al, byte ptr [ebx]
        mov    edi, edx
        mov    esi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_1900:
        cmp    dword ptr [esp + 0x10], 0
        mov    eax, dword ptr [ebp]
        je     loc_3_1912
        mov    word ptr [eax + 0x18], 1
        jmp    loc_3_1918
loc_3_1912:
        mov    word ptr [eax + 0x18], 0
loc_3_1918:
        mov    eax, dword ptr [ebp]
        pop    ebp
        mov    word ptr [eax + 0x1a], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1930:
        sub    esp, 0x14
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x20]
        xor    eax, eax
        mov    al, byte ptr [edi + 4]
        or     eax, eax
        je     loc_3_1963
        cmp    eax, 2
        je     loc_3_1982
        cmp    eax, 3
        je     loc_3_19C0
        mov    si, word ptr [esp + 0xa]
loc_3_1951:
        push   esi
        push   1
        push   edi
        call   sub_3_55E0
        add    esp, 0xc
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
loc_3_1963:
        mov    si, 0xc
        push   edi
        call   sub_3_19D0
        add    esp, 4
        cmp    eax, 0x11c
        jne    loc_3_1951
        mov    eax, 0x11c
        pop    edi
        pop    esi
        add    esp, 0x14
        ret    
loc_3_1982:
        mov    dword ptr [esp + 0xc], 0xbaadf00d
        mov    byte ptr [esp + 0x10], 0xd
        mov    byte ptr [esp + 0x11], 0xf0
        mov    byte ptr [esp + 0x12], 0xad
        mov    si, 0x3c
        mov    byte ptr [esp + 0x13], 0xba
        mov    dword ptr [esp + 0x14], 0xbaadf00d
        mov    dword ptr [esp + 0x18], 0xbaadf00d
        lea    eax, [esp + 0xc]
        mov    dword ptr [edi + 0x24], eax
        jmp    loc_3_1951
        db  005h, 000h, 000h, 000h, 000h
loc_3_19C0:
        mov    si, 3
        jmp    loc_3_1951
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_19D0:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [0x9bd0]       ; -> obj3:0x9BD0
        or     edi, edi
        je     loc_3_19FD
        mov    esi, dword ptr [esp + 0x20]
        mov    ecx, 1
loc_3_19EA:
        test   byte ptr [edi + 0x1f], cl
        je     loc_3_19F7
        mov    eax, dword ptr [esi + 0x28]
        cmp    dword ptr [edi + 8], eax
        je     loc_3_1A0A
loc_3_19F7:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_3_19EA
loc_3_19FD:
        mov    eax, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_3_1A0A:
        mov    ebx, dword ptr [esi + 0x14]
        mov    cl, byte ptr [esi + 4]
        mov    byte ptr [esp + 0x18], cl
        mov    ebp, dword ptr [esi + 0x28]
        mov    cx, word ptr [esi + 0x1a]
        mov    word ptr [esp + 0x14], cx
        mov    ecx, dword ptr [esi + 0x24]
        mov    dword ptr [esp + 0x10], ecx
        mov    byte ptr [esi + 4], 2
        mov    dword ptr [esi + 0x28], edi
        mov    dword ptr [esi + 0x14], 0
        push   esi
        call   sub_3_1930
        add    esp, 4
        mov    dword ptr [esi + 0x14], ebx
        mov    cl, byte ptr [esp + 0x18]
        mov    byte ptr [esi + 4], cl
        mov    dword ptr [esi + 0x28], ebp
        or     eax, eax
        mov    cx, word ptr [esp + 0x14]
        mov    word ptr [esi + 0x1a], cx
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x24], ecx
        jne    loc_3_19FD
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 0x1c], eax
        mov    word ptr [esi + 0x1a], 0x11c
        mov    dword ptr [esi + 0x10], 0x944 ; -> loc_1_0944
        mov    eax, 0x11c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  090h
loc_3_1A80:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0x41
        mov    eax, 0x41
        ret    
loc_3_1A90:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_5825
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_3_1AA0:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0
        sub    eax, eax
        ret    
        db  08Dh, 049h, 000h
loc_3_1AB0:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0
        sub    eax, eax
        ret    
        db  08Dh, 049h, 000h
loc_3_1AC0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        sub    esi, esi
        cmp    dword ptr [edi + 0x4c], esi
        je     loc_3_1AF0
        mov    eax, dword ptr [edi + 0x20]
        push   eax
        push   esi
        mov    eax, dword ptr [edi + 0x4c]
        push   eax
        call   sub_3_8750
        add    esp, 0xc
        mov    dword ptr [edi + 0x4c], esi
        cmp    dword ptr [edi + 0x24], esi
        je     loc_3_1AF5
        mov    esi, 5
        jmp    loc_3_1AF5
        db  08Dh, 049h, 000h
loc_3_1AF0:
        mov    esi, 8
loc_3_1AF5:
        or     esi, esi
        je     loc_3_1B11
        mov    eax, dword ptr [edi + 0x1c]
        push   edi
        mov    cl, byte ptr [eax + 0xb]
        mov    eax, dword ptr [edi + 0x1c]
        or     cl, 1
        mov    byte ptr [eax + 0xb], cl
        call   sub_3_47C0
        add    esp, 4
loc_3_1B11:
        mov    eax, dword ptr [edi]
        mov    dword ptr [edi + 0x50], 0
        mov    word ptr [eax + 0x1a], si
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1B30:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [esi + 0x14], eax
        cmp    eax, dword ptr [0x9c80]       ; -> obj3:0x9C80
        jne    loc_3_1B4F
        mov    dword ptr [0x9c80], 0         ; -> obj3:0x9C80
loc_3_1B4F:
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x44], ecx
        or     byte ptr [eax + 0x1f], 8
        push   eax
        call   sub_3_7800
        add    esp, 4
        push   esi
        call   sub_3_4660
        add    esp, 4
        mov    eax, dword ptr [esi]
        pop    esi
        mov    word ptr [eax + 0x1a], 0
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1B80:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 0x14], 0
        je     loc_3_1B94
        or     dword ptr [esi + 0x20], 0x80
loc_3_1B94:
        mov    eax, dword ptr [esi]
        mov    byte ptr [esi + 0x63], 1
        mov    eax, dword ptr [eax + 0x28]
        mov    edx, eax
        mov    dword ptr [esi + 0x14], eax
        mov    ecx, dword ptr [edx + 0xc]
        mov    dword ptr [esi + 0x44], ecx
        mov    eax, dword ptr [edx + 0x38]
        mov    dword ptr [esi + 0x48], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 0x40], eax
        inc    word ptr [edx + 0x1c]
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [eax + 0x24]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [esi + 0x44], eax
        call   sub_3_7720
        mov    ecx, dword ptr [esi + 0x44]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esi + 0x14]
        test   byte ptr [eax + 0x1f], 2
        jne    loc_3_1C10
        mov    ecx, dword ptr [esi + 0x40]
        cmp    dword ptr [eax + 8], ecx
        jne    loc_3_1C10
        push   0xa
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_1C10:
        mov    dword ptr [esi + 0x24], 0xc
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1C20:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        cmp    dword ptr [ebx + 0x24], 0
        jne    loc_3_1D36
        mov    esi, dword ptr [ebx + 0x14]
        mov    edx, 1
        mov    eax, dword ptr [esi + 0xc]
        mov    cl, byte ptr [eax + 0xc]
        shl    edx, cl
        push   edx
        mov    eax, dword ptr [ebx + 0x4c]
        push   eax
        call   sub_3_6890
        add    esp, 8
        cmp    eax, dword ptr [esi + 0x3c]
        jne    loc_3_1D36
        mov    esi, dword ptr [ebx + 0x14]
        mov    edx, dword ptr [ebx + 0x4c]
        cmp    byte ptr [esi + 0x1e], 0
        lea    esi, [esi + 0x58]
        je     loc_3_1CD6
        lea    edi, [edx + 0x28]
        mov    ecx, 8
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_1D36
        mov    esi, dword ptr [ebx + 0x14]
        lea    edi, [edx + 0xbe]
        mov    ecx, 0x20
        lea    esi, [esi + 0x78]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_1D36
        mov    ecx, dword ptr [ebx + 0x14]
        mov    ax, word ptr [edx + 0x78]
        cmp    word ptr [ecx + 0xf8], ax
        jne    loc_3_1D36
        mov    ax, word ptr [ecx + 0xfa]
        cmp    word ptr [edx + 0x7c], ax
        jne    loc_3_1D36
        mov    eax, dword ptr [ecx + 0xfc]
        cmp    dword ptr [edx + 0x8c], eax
        jne    loc_3_1D36
        mov    eax, dword ptr [ecx + 0x100]
        cmp    dword ptr [edx + 0x84], eax
        je     loc_3_1D68
        jmp    loc_3_1D36
loc_3_1CD6:
        lea    edi, [edx + 0x30]
        mov    ecx, 8
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_1D36
        mov    esi, dword ptr [ebx + 0x14]
        lea    edi, [edx + 0xd6]
        mov    ecx, 0x20
        lea    esi, [esi + 0x78]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_1D36
        mov    ecx, dword ptr [ebx + 0x14]
        mov    ax, word ptr [ecx + 0xf8]
        cmp    word ptr [edx + 0x80], ax
        jne    loc_3_1D36
        mov    ax, word ptr [ecx + 0xfa]
        cmp    word ptr [edx + 0x84], ax
        jne    loc_3_1D36
        mov    eax, dword ptr [ecx + 0xfc]
        cmp    dword ptr [edx + 0x94], eax
        jne    loc_3_1D36
        mov    eax, dword ptr [ecx + 0x100]
        cmp    dword ptr [edx + 0x8c], eax
        je     loc_3_1D68
loc_3_1D36:
        mov    eax, dword ptr [ebx]
        cmp    dword ptr [eax + 0x14], 0
        je     loc_3_1D61
        mov    esi, dword ptr [ebx + 0x14]
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [esi + 0x3c]
        mov    dword ptr [ecx + 0x28], eax
        mov    ecx, 8
        mov    eax, dword ptr [ebx]
        add    esi, 0x58
        mov    edi, dword ptr [eax + 0x14]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 0x14]
        mov    byte ptr [eax + 0x20], 0
loc_3_1D61:
        mov    dword ptr [ebx + 0x24], 0xffffffff
loc_3_1D68:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 0x24]
        mov    word ptr [ecx + 0x1a], ax
        push   ebx
        call   sub_3_75B0
        add    esp, 4
        mov    eax, dword ptr [ebx + 0x44]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
        push   ebx
        call   sub_3_4660
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1DA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x44]
        or     eax, eax
        je     loc_3_1DD1
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 0x44]
        push   eax
        call   sub_3_8750
        add    esp, 0xc
        mov    dword ptr [esi + 0x44], 0
loc_3_1DD1:
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1DE0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        and    eax, 0xffffff
        cmp    eax, 0x3a2b24
        je     loc_3_1E10
        cmp    eax, 0x402f25
        je     loc_3_1E22
        cmp    eax, 0x432f25
        je     loc_3_1E22
        cmp    eax, 0x452f25
        je     loc_3_1E22
loc_3_1E07:
        mov    eax, 0xffff
        ret    
        db  08Dh, 049h, 000h
loc_3_1E10:
        cmp    dword ptr [0x9c10], 0x3ae     ; -> loc_3_9C10
        jne    loc_3_1E07
        mov    eax, 0x3ae
        ret    
loc_3_1E22:
        mov    eax, 0x4b0
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_1E30:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, 1
        mov    ecx, dword ptr [esi + 0x40]
        shl    edx, cl
        mov    eax, dword ptr [0x9bf4]       ; -> loc_3_9BF4
        or     eax, edx
        mov    dword ptr [0x9bf4], eax       ; -> loc_3_9BF4
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 0x10], 0x800
        jne    loc_3_1F30
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        lea    edx, [ecx + ecx*2]
        test   byte ptr [eax + edx*2 + 0x15], 8
        je     loc_3_1F30
        or     dword ptr [esi + 0x20], 0x4000
        mov    al, byte ptr [0x1f40]         ; -> loc_3_1F40
        mov    eax, dword ptr [0x9bd0]       ; -> obj3:0x9BD0
        or     eax, eax
        je     loc_3_1EA0
loc_3_1E84:
        cmp    dword ptr [eax + 8], ecx
        je     loc_3_1E91
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_3_1E84
        jmp    loc_3_1EA0
loc_3_1E91:
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
loc_3_1EA0:
        push   1
        push   0x10
        call   sub_3_43B0
        add    esp, 8
        mov    dword ptr [esi + 0x44], eax
        cmp    dword ptr [esi + 8], 0
        jne    loc_3_1EC4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_3_36E0
        add    esp, 4
        mov    dword ptr [esi + 8], eax
loc_3_1EC4:
        mov    edi, dword ptr [esi + 0x44]
        or     edi, edi
        je     loc_3_1F20
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_3_1F20
        mov    dword ptr [eax + 0x40], esi
        call   sub_3_7720
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x44]
        mov    dword ptr [eax + 0xc], 0xb
        mov    byte ptr [eax + 4], 0xff
        mov    byte ptr [eax + 5], 0xff
        mov    byte ptr [eax + 6], 0xff
        mov    byte ptr [eax + 7], 0xff
        mov    word ptr [esi + 0x70], 0xff
        mov    dword ptr [esi + 0xa0], 0xff
        mov    dword ptr [esi + 0x98], 0xff
        push   0x16
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  090h
loc_3_1F20:
        mov    dword ptr [esi + 0x24], 8
        mov    word ptr [esi + 0x2e], 0x15
        pop    edi
        pop    esi
        ret    
loc_3_1F30:
        mov    dword ptr [esi + 0x24], 0xb
        mov    word ptr [esi + 0x2e], 0x15
        pop    edi
        pop    esi
        ret    
loc_3_1F40:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1F50:
        sub    esp, 0x34
        push   ebx
        mov    ecx, dword ptr [esp + 0x3c]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp + 8], 0
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esp + 0xc], eax
        add    edx, 0x3c
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [ecx + 0x40]
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0
        mov    dword ptr [esp + 0x24], edx
        mov    dword ptr [esp + 0x28], 0x18
        mov    word ptr [esp + 0x2c], 0xd
        mov    word ptr [esp + 0x2e], 0xcd0b
        mov    dword ptr [esp + 0x30], 0
        mov    dword ptr [esp + 0x34], 0x400
        mov    dword ptr [edx], 0
        mov    al, byte ptr [ecx + 0x70]
        mov    byte ptr [edx + 0x12], al
        mov    eax, dword ptr [ecx + 0x44]
        push   ecx
        mov    bl, byte ptr [eax + 4]
        mov    byte ptr [edx + 0x10], bl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [edx + 0x11], al
        mov    byte ptr [edx + 0x13], 0xff
        call   sub_3_84F0
        lea    eax, [esp + 8]
        add    esp, 4
        push   eax
        call   sub_3_30C0
        add    esp, 4
        pop    ebx
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_3_2000:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x18]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 8]
        add    edx, 0x3c
        cmp    dword ptr [eax + 0x3c], 0
        jne    loc_3_20E0
        mov    esi, dword ptr [ecx + 0x44]
        xor    edi, edi
        mov    di, word ptr [ecx + 0x70]
        xor    eax, eax
        mov    al, byte ptr [esi + 4]
        cmp    eax, edi
        jne    loc_3_203A
        mov    eax, dword ptr [edx + 4]
        mov    byte ptr [esi + 6], al
        mov    dword ptr [ecx + 0xa0], eax
loc_3_203A:
        xor    eax, eax
        mov    al, byte ptr [esi + 5]
        cmp    eax, edi
        jne    loc_3_209A
        mov    eax, dword ptr [edx + 0x14]
        add    eax, 0x10
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [esp + 0x10], eax
        movzx  esi, byte ptr [esp + 0x12]
        shl    esi, 2
        xor    eax, eax
        lea    esi, [esi + esi*2]
        mov    al, byte ptr [esp + 0x11]
        lea    esi, [esi + esi*4]
        add    esi, eax
        xor    eax, eax
        lea    esi, [esi + esi*4]
        mov    al, byte ptr [esp + 0x10]
        lea    esi, [esi + esi*2]
        lea    esi, [esi + esi*4]
        add    esi, eax
        sub    esi, 0x96
        mov    dword ptr [esp + 0x10], esi
        mov    eax, esi
        mov    dword ptr [ecx + 0xac], eax
        mov    eax, dword ptr [ecx + 0x44]
        mov    ebx, dword ptr [edx + 8]
        mov    byte ptr [eax + 7], bl
        mov    dword ptr [ecx + 0x98], ebx
loc_3_209A:
        mov    eax, dword ptr [ecx + 0x44]
        mov    bl, byte ptr [edx + 0x10]
        mov    byte ptr [eax + 4], bl
        mov    dl, byte ptr [edx + 0x11]
        mov    byte ptr [eax + 5], dl
        cmp    word ptr [ecx + 0x70], 0xff
        jne    loc_3_20C7
        cmp    dl, bl
        je     loc_3_20C7
        mov    al, bl
        and    eax, 0xffff00ff
        mov    word ptr [ecx + 0x70], ax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_20C7:
        mov    dword ptr [ecx + 0x24], 0
        push   ecx
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_3_20E0:
        mov    eax, dword ptr [ecx + 0x44]
        cmp    dword ptr [eax], 0
        jne    loc_3_20F1
        mov    dword ptr [ecx + 0x24], 0xb
        jmp    loc_3_20F8
loc_3_20F1:
        mov    dword ptr [ecx + 0x24], 0
loc_3_20F8:
        push   ecx
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_2110:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x24], 0
        je     loc_3_2123
        mov    word ptr [esi + 0x2e], 0x15
        pop    esi
        ret    
loc_3_2123:
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi + 0x44]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0x48], eax
        mov    dword ptr [esi + 0x58], 0
        mov    dword ptr [esi + 0x5c], 0
        mov    dword ptr [esi + 0x68], 1
        mov    byte ptr [esi + 0x60], 0xff
        mov    byte ptr [esi + 0x61], 0
        mov    byte ptr [esi + 0x62], 0xff
        push   0x18
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_2170:
        mov    eax, dword ptr [esp + 4]
        push   0xa
        push   eax
        call   sub_3_8380
        add    esp, 8
        ret    
loc_3_2180:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 0x24], 0
        je     loc_3_21C5
        xor    eax, eax
        mov    al, byte ptr [esi + 0x61]
        or     eax, eax
        je     loc_3_21A4
        cmp    eax, 4
        jl     loc_3_21A0
        cmp    eax, 5
        jle    loc_3_21A8
loc_3_21A0:
        or     byte ptr [esi + 0x6f], 8
loc_3_21A4:
        mov    byte ptr [esi + 0x61], 3
loc_3_21A8:
        mov    dword ptr [esi + 0x24], 0
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_21C5:
        mov    edi, dword ptr [esi + 0x4c]
        mov    eax, 0xa344                   ; -> loc_3_A344
        lea    ecx, [edi + 9]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    loc_3_21DC
        mov    al, byte ptr [eax + 4]
        cmp    al, byte ptr [ecx + 4]
loc_3_21DC:
        mov    eax, 0
        sete   al
        mov    ecx, 0xa34c                   ; -> loc_3_A34C
        lea    ebx, [edi + 1]
        mov    edx, dword ptr [ecx]
        cmp    edx, dword ptr [ebx]
        jne    loc_3_21F8
        mov    cl, byte ptr [ecx + 4]
        cmp    cl, byte ptr [ebx + 4]
loc_3_21F8:
        mov    ecx, 0
        sete   cl
        or     eax, eax
        jne    loc_3_224A
        or     ecx, ecx
        jne    loc_3_224A
        test   byte ptr [esi + 0x23], 4
        jne    loc_3_2220
        mov    al, byte ptr [esi + 0x61]
        cmp    al, 4
        je     loc_3_2219
        cmp    al, 5
        jne    loc_3_2220
loc_3_2219:
        or     dword ptr [esi + 0x20], 0x4000000
loc_3_2220:
        cmp    byte ptr [esi + 0x61], 0
        je     loc_3_222C
        test   byte ptr [esi + 0x23], 4
        jne    loc_3_2234
loc_3_222C:
        or     byte ptr [esi + 0x6f], 8
        mov    byte ptr [esi + 0x61], 3
loc_3_2234:
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_224A:
        test   byte ptr [esi + 0x23], 4
        je     loc_3_2266
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_2266:
        mov    byte ptr [esi + 0x62], cl
        or     ecx, ecx
        je     loc_3_2273
        xor    eax, eax
        mov    al, byte ptr [edi]
        jmp    loc_3_2278
loc_3_2273:
        xor    eax, eax
        mov    al, byte ptr [edi + 8]
loc_3_2278:
        or     ecx, ecx
        je     loc_3_2283
        xor    edx, edx
        mov    dl, byte ptr [edi + 6]
        jmp    loc_3_2288
loc_3_2283:
        xor    edx, edx
        mov    dl, byte ptr [edi + 0xe]
loc_3_2288:
        cmp    byte ptr [esi + 0x60], 0xff
        jne    loc_3_2291
        mov    byte ptr [esi + 0x60], cl
loc_3_2291:
        cmp    eax, 1
        je     loc_3_22B0
        cmp    eax, 2
        je     loc_3_22C9
        cmp    eax, 0xff
        je     loc_3_2332
        jmp    loc_3_233D
        db  005h, 000h, 000h, 000h, 000h
loc_3_22B0:
        cmp    edx, 1
        jne    loc_3_233D
        cmp    byte ptr [esi + 0x61], 0
        jne    loc_3_233D
        mov    byte ptr [esi + 0x61], 4
        jmp    loc_3_22FF
loc_3_22C9:
        cmp    edx, 1
        jne    loc_3_233D
        or     ecx, ecx
        lea    eax, [edi + 0x58]
        jne    loc_3_22D8
        lea    eax, [edi + 0x60]
loc_3_22D8:
        push   eax
        call   sub_3_1DE0
        add    esp, 4
        cmp    eax, 0xffff
        je     loc_3_233D
        cmp    eax, 0x4b0
        je     loc_3_22F8
        cmp    dword ptr [esi + 0x68], 0x4b0
        je     loc_3_233D
loc_3_22F8:
        mov    dword ptr [esi + 0x68], eax
        mov    byte ptr [esi + 0x61], 5
loc_3_22FF:
        mov    eax, dword ptr [esi + 0x58]
        or     eax, eax
        je     loc_3_2319
        mov    dword ptr [esi + 0x4c], eax
        mov    dword ptr [esi + 0x50], 4
        push   esi
        call   sub_3_75B0
        add    esp, 4
loc_3_2319:
        mov    eax, dword ptr [esi + 0x48]
        mov    dword ptr [esi + 0x5c], eax
        mov    dword ptr [esi + 0x58], edi
        mov    dword ptr [esi + 0x4c], 0
        mov    dword ptr [esi + 0x50], 0
        jmp    loc_3_233D
loc_3_2332:
        or     dword ptr [esi + 0x20], 0x4000000
        mov    byte ptr [esi + 0x60], 0xff
loc_3_233D:
        push   esi
        call   sub_3_75B0
        add    esp, 4
        inc    dword ptr [esi + 0x48]
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_3_2350:
        mov    ecx, dword ptr [esp + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 0x61]
        cmp    eax, 3
        je     loc_3_2391
        cmp    eax, 4
        jl     loc_3_2368
        cmp    eax, 5
        jle    loc_3_2370
loc_3_2368:
        mov    word ptr [ecx + 0x2e], 0x15
        ret    
        db  090h
loc_3_2370:
        mov    eax, dword ptr [ecx + 0x5c]
        mov    dword ptr [ecx + 0x48], eax
        mov    eax, dword ptr [ecx + 0x58]
        mov    dword ptr [ecx + 0x4c], eax
        mov    dword ptr [ecx + 0x54], 0
        mov    dword ptr [ecx + 0x50], 4
        mov    dword ptr [ecx + 0x58], 0
loc_3_2391:
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_23A0:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x28]
        mov    al, byte ptr [edx + 0x61]
        cmp    al, 4
        je     loc_3_23D0
        cmp    al, 5
        je     loc_3_23D0
        cmp    al, 3
        je     loc_3_2490
        mov    word ptr [edx + 0x2e], 0x15
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_23D0:
        cmp    al, 3
        je     loc_3_2490
        mov    edx, dword ptr [esp + 0x28]
        xor    eax, eax
        mov    al, byte ptr [edx + 0x62]
        mov    ebx, dword ptr [edx + 0x4c]
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_3_23F4
        mov    esi, dword ptr [ebx + 0x84]
        jmp    loc_3_23FA
loc_3_23F4:
        mov    esi, dword ptr [ebx + 0x8c]
loc_3_23FA:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_2410
        mov    edi, dword ptr [ebx + 0x8c]
        jmp    loc_3_2416
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2410:
        mov    edi, dword ptr [ebx + 0x94]
loc_3_2416:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_2428
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x80]
        jmp    loc_3_2431
loc_3_2428:
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x88]
loc_3_2431:
        push   eax
        call   sub_3_6860
        mov    dword ptr [esp + 0x18], eax
        mov    edx, dword ptr [esp + 0x2c]
        add    esp, 4
        mov    eax, dword ptr [edx + 0x44]
        mov    ebp, edi
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esp + 0x1c], eax
        mov    dl, al
        mov    al, dl
        sub    al, byte ptr [esp + 0x14]
        mov    cl, al
        shr    ebp, cl
        shl    ebp, cl
        sub    edi, ebp
        mov    cl, byte ptr [esp + 0x14]
        shl    edi, cl
        mov    ebp, 1
        mov    cl, dl
        shl    ebp, cl
        add    edi, ebp
        add    edi, esi
        dec    edi
        shr    edi, cl
        or     edi, edi
        je     loc_3_2760
        lea    eax, [edi*2 - 1]
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [eax + eax*2 + 0x51]
        shl    eax, 2
        jmp    loc_3_24B3
loc_3_2490:
        mov    edx, dword ptr [esp + 0x28]
        mov    dword ptr [esp + 0x14], 0xb
        mov    eax, dword ptr [edx + 0x98]
        sub    eax, dword ptr [edx + 0xa0]
        lea    eax, [eax*8 + 0x14c]
        mov    ebx, dword ptr [esp + 0x20]
loc_3_24B3:
        push   1
        push   eax
        call   sub_3_43B0
        mov    ebp, eax
        mov    edx, dword ptr [esp + 0x30]
        add    esp, 8
        mov    dword ptr [edx + 0x14], ebp
        or     ebp, ebp
        je     loc_3_2760
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [edx + 0x40]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [edx + 0x44]
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp + 0x1c], 1
        mov    eax, dword ptr [edx + 0x48]
        mov    dword ptr [ebp + 0x38], eax
        mov    eax, dword ptr [edx + 0x68]
        mov    dword ptr [ebp + 0x4c], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebp + 0x44], eax
        cmp    byte ptr [edx + 0x61], 3
        jne    loc_3_25E5
        mov    cl, byte ptr [ebp + 0x1f]
        or     cl, 2
        mov    byte ptr [ebp + 0x1f], cl
        mov    byte ptr [ebp + 0x1e], 1
        mov    edi, dword ptr [edx + 0xa0]
        mov    dword ptr [ebp + 0x138], edi
        mov    esi, dword ptr [edx + 0x98]
        mov    dword ptr [ebp + 0x13c], esi
        sub    esi, edi
        mov    eax, dword ptr [edx + 0xac]
        mov    dword ptr [ebp + 0x140], eax
        mov    eax, dword ptr [edx + 0xac]
        mov    dword ptr [ebp + 0x48], eax
        inc    esi
        mov    dword ptr [ebp + 0x130], esi
        lea    eax, [ebp + 0x144]
        mov    dword ptr [ebp + 0x134], eax
        mov    dword ptr [ebp + 0x104], 3
        test   byte ptr [edx + 0x6f], 8
        je     loc_3_2580
        or     cl, 0x20
        mov    byte ptr [ebp + 0x1f], cl
        mov    dword ptr [ebp + 0x108], 0xa00c ; -> obj3:0xA00C
        mov    esi, 0x9ee0                   ; -> loc_3_9EE0
        jmp    loc_3_258F
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_2580:
        mov    dword ptr [ebp + 0x108], 0xa030 ; -> loc_3_A030
        mov    esi, 0x9f00                   ; -> loc_3_9F00
loc_3_258F:
        lea    edi, [ebp + 0x58]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x9f20                   ; -> loc_3_9F20
        lea    edi, [ebp + 0x10c]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    eax, [ebp + 0x50]
        push   eax
        push   0x9f56                        ; -> loc_3_9F56
        call   sub_3_5300
        add    esp, 8
        cmp    dword ptr [ebp + 0x130], 2
        jbe    loc_3_25D3
        mov    dword ptr [ebp + 0x3c], 0
        jmp    loc_3_274A
loc_3_25D3:
        mov    edx, dword ptr [esp + 0x28]
        mov    eax, dword ptr [edx + 0xac]
        mov    dword ptr [ebp + 0x3c], eax
        jmp    loc_3_274A
loc_3_25E5:
        mov    eax, 1
        mov    cl, byte ptr [esp + 0x1c]
        shl    eax, cl
        push   eax
        push   ebx
        call   sub_3_6890
        add    esp, 8
        mov    dword ptr [ebp + 0x3c], eax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_260B
        xor    eax, eax
        mov    al, byte ptr [ebx + 7]
        jmp    loc_3_2610
loc_3_260B:
        xor    eax, eax
        mov    al, byte ptr [ebx + 0xf]
loc_3_2610:
        mov    dword ptr [ebp + 0x40], eax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_2620
        mov    eax, dword ptr [ebx + 0x50]
        jmp    loc_3_2623
        db  090h
loc_3_2620:
        mov    eax, dword ptr [ebx + 0x58]
loc_3_2623:
        mov    dword ptr [ebp + 0x48], eax
        cmp    dword ptr [esp + 0x10], 0
        lea    eax, [ebx + 0x32d]
        jne    loc_3_2639
        lea    eax, [ebx + 0x316]
loc_3_2639:
        lea    ecx, [ebp + 0x50]
        push   ecx
        push   eax
        call   sub_3_5220
        add    esp, 8
        lea    esi, [ebx + 0x28]
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_3_2653
        lea    esi, [ebx + 0x30]
loc_3_2653:
        lea    edi, [ebp + 0x58]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        cmp    dword ptr [esp + 0x10], 0
        lea    esi, [ebx + 0xbe]
        jne    loc_3_2670
        lea    esi, [ebx + 0xd6]
loc_3_2670:
        lea    edi, [ebp + 0x78]
        mov    ecx, 0x20
        rep movsd dword ptr es:[edi], dword ptr [esi]
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_2687
        mov    ax, word ptr [ebx + 0x78]
        jmp    loc_3_268E
loc_3_2687:
        mov    ax, word ptr [ebx + 0x80]
loc_3_268E:
        mov    word ptr [ebp + 0xf8], ax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_26A2
        mov    ax, word ptr [ebx + 0x7c]
        jmp    loc_3_26A9
loc_3_26A2:
        mov    ax, word ptr [ebx + 0x84]
loc_3_26A9:
        mov    word ptr [ebp + 0xfa], ax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_26C0
        mov    eax, dword ptr [ebx + 0x8c]
        jmp    loc_3_26C6
        db  090h
loc_3_26C0:
        mov    eax, dword ptr [ebx + 0x94]
loc_3_26C6:
        mov    dword ptr [ebp + 0xfc], eax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_26E0
        mov    eax, dword ptr [ebx + 0x84]
        jmp    loc_3_26E6
        db  005h, 000h, 000h, 000h, 000h
loc_3_26E0:
        mov    eax, dword ptr [ebx + 0x8c]
loc_3_26E6:
        mov    dword ptr [ebp + 0x100], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ebp + 0x104], eax
        lea    eax, [ebp + 0x144]
        mov    dword ptr [ebp + 0x108], eax
        cmp    dword ptr [esp + 0x10], 0
        lea    esi, [ebx + 0x9c]
        jne    loc_3_2715
        lea    esi, [ebx + 0xb4]
loc_3_2715:
        lea    edi, [ebp + 0x10c]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    al, byte ptr [esp + 0x10]
        mov    byte ptr [ebp + 0x1e], al
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_274A
        lea    esi, [ebx + 0x400]
        mov    edi, 0xa354                   ; -> loc_3_A354
        mov    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_274A
        or     byte ptr [ebp + 0x1f], 4
loc_3_274A:
        mov    edx, dword ptr [esp + 0x28]
        push   edx
        call   sub_3_75B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  090h
loc_3_2760:
        mov    edx, dword ptr [esp + 0x28]
        pop    ebp
        mov    dword ptr [edx + 0x24], 8
        mov    word ptr [edx + 0x2e], 0x15
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2780:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esi + 0x14]
        test   byte ptr [ebx + 0x1f], 2
        je     loc_3_27D0
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_3_36E0
        add    esp, 4
        mov    dword ptr [esi + 8], eax
        or     eax, eax
        jne    loc_3_27C0
        mov    dword ptr [esi + 0x24], 8
        mov    word ptr [esi + 0x2e], 0x15
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_3_27C0:
        mov    dword ptr [eax + 0x40], esi
        mov    dword ptr [esi + 0xa4], 0
        jmp    loc_3_2809
        db  090h
loc_3_27D0:
        mov    word ptr [esi + 0x74], 1
        mov    word ptr [esi + 0x64], 0
        mov    edi, dword ptr [ebx + 0xfc]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dl, byte ptr [ebx + 0x44]
        mov    ecx, dword ptr [eax + 0xc]
        sub    cl, dl
        mov    eax, edi
        shr    edi, cl
        shl    edi, cl
        sub    eax, edi
        mov    cl, dl
        shl    eax, cl
        add    eax, dword ptr [ebx + 0x100]
        mov    dword ptr [esi + 0x98], eax
        mov    byte ptr [esi + 0x63], 7
loc_3_2809:
        push   0x1a
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_2820:
        sub    esp, 0x34
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x40]
        mov    esi, dword ptr [edx + 0x14]
        test   byte ptr [esi + 0x1f], 2
        je     loc_3_28D3
        mov    ecx, dword ptr [edx]
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 0xc], 0
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esp + 0x10], eax
        add    ecx, 0x3c
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [edx + 0x40]
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x20], 0
        mov    dword ptr [esp + 0x24], 0
        mov    dword ptr [esp + 0x28], ecx
        mov    dword ptr [esp + 0x2c], 0x10
        mov    word ptr [esp + 0x30], 0xd
        mov    word ptr [esp + 0x32], 0xcd0c
        mov    dword ptr [esp + 0x34], 0
        mov    dword ptr [esp + 0x38], 0x400
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esi + 0x138]
        add    eax, dword ptr [edx + 0xa4]
        mov    dword ptr [ecx + 4], eax
        push   edx
        call   sub_3_84F0
        lea    eax, [esp + 0xc]
        add    esp, 4
        push   eax
        call   sub_3_30C0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 0x34
        ret    
loc_3_28D3:
        mov    byte ptr [edx + 0x63], 7
        mov    eax, dword ptr [esi + 0xc]
        mov    edi, dword ptr [esi + 0xfc]
        mov    ecx, dword ptr [eax + 0xc]
        xor    eax, eax
        sub    cl, byte ptr [esi + 0x44]
        mov    ax, word ptr [edx + 0x64]
        shr    edi, cl
        inc    eax
        sar    eax, 1
        add    edi, eax
        mov    dword ptr [edx + 0x48], edi
        push   0xa
        push   edx
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2910:
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x48]
        mov    edx, dword ptr [ebx + 0x14]
        mov    cl, byte ptr [edx + 0x1f]
        and    cl, 2
        je     loc_3_292F
        mov    eax, dword ptr [ebx + 8]
        mov    eax, dword ptr [eax + 0x3c]
        mov    dword ptr [ebx + 0x24], eax
loc_3_292F:
        cmp    dword ptr [ebx + 0x24], 0
        je     loc_3_2946
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
loc_3_2946:
        or     cl, cl
        je     loc_3_29F5
        mov    ecx, dword ptr [edx + 0x134]
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 0xa4]
        mov    eax, dword ptr [eax + 0x44]
        mov    dword ptr [esp + 0x40], eax
        mov    eax, dword ptr [ebx + 0x14]
        mov    esi, dword ptr [esp + 0x40]
        add    dword ptr [eax + 0x3c], esi
        xor    eax, eax
        movzx  esi, byte ptr [esp + 0x42]
        shl    esi, 2
        mov    al, byte ptr [esp + 0x41]
        lea    esi, [esi + esi*2]
        lea    esi, [esi + esi*4]
        add    esi, eax
        xor    eax, eax
        lea    esi, [esi + esi*4]
        mov    al, byte ptr [esp + 0x40]
        lea    esi, [esi + esi*2]
        lea    esi, [esi + esi*4]
        add    esi, eax
        sub    esi, 0x96
        mov    dword ptr [esp + 0x40], esi
        mov    eax, esi
        mov    dword ptr [ecx + edx*8], eax
        mov    edi, dword ptr [ebx + 0x14]
        inc    dword ptr [ebx + 0xa4]
        mov    edx, dword ptr [edi + 0x130]
        cmp    dword ptr [ebx + 0xa4], edx
        jb     loc_3_2D80
        sub    esi, esi
        dec    edx
        je     loc_3_29D6
        lea    eax, [ecx + 4]
        mov    esi, edx
loc_3_29C8:
        mov    ebp, dword ptr [eax + 4]
        sub    ebp, dword ptr [eax - 4]
        mov    dword ptr [eax], ebp
        add    eax, 8
        dec    edx
        jne    loc_3_29C8
loc_3_29D6:
        lea    eax, [ecx + esi*8]
        mov    ecx, dword ptr [edi + 0x140]
        sub    ecx, dword ptr [eax]
        mov    dword ptr [eax + 4], ecx
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
loc_3_29F5:
        xor    eax, eax
        mov    al, byte ptr [edx + 0x1e]
        mov    ecx, dword ptr [edx + 0xc]
        mov    dword ptr [esp + 0x14], eax
        mov    ecx, dword ptr [ecx + 0xc]
        mov    dword ptr [esp + 0x34], ecx
        mov    ecx, dword ptr [edx + 0x108]
        mov    dword ptr [esp + 0x30], ecx
        mov    cx, word ptr [ebx + 0x64]
        mov    eax, dword ptr [edx + 0x44]
        mov    esi, ecx
        and    esi, 0xffff
        mov    dword ptr [esp + 0x24], esi
        mov    ebp, dword ptr [ebx + 0x4c]
        or     cx, cx
        je     loc_3_2BA2
        mov    ecx, dword ptr [esp + 0x30]
        lea    eax, [esi + esi*2]
        lea    edx, [ecx + eax*4]
        mov    dword ptr [esp + 0x2c], edx
        mov    ax, word ptr [ebx + 0x74]
        mov    word ptr [edx], ax
        mov    ax, word ptr [edx + 4]
        or     ax, ax
        je     loc_3_2B80
        mov    dx, 1
        mov    cl, byte ptr [esp + 0x34]
        shl    dx, cl
        sub    dx, ax
        mov    word ptr [esp + 0x10], dx
        mov    edx, dword ptr [esp + 0x2c]
        cmp    word ptr [esp + 0x10], 8
        mov    eax, dword ptr [edx + 8]
        jae    loc_3_2B10
        mov    eax, dword ptr [esp + 0x10]
        and    eax, 0xffff
        mov    dword ptr [esp + 0x38], eax
        mov    ecx, dword ptr [ebx]
        lea    edi, [eax + ecx + 0x3c]
        mov    eax, 8
        sub    eax, dword ptr [esp + 0x38]
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    dword ptr [esp + 0x14], 0
        mov    eax, dword ptr [ebx]
        je     loc_3_2AB5
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x3c]
        jmp    loc_3_2ABA
loc_3_2AB5:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x41]
loc_3_2ABA:
        cmp    dword ptr [esp + 0x14], 0
        mov    eax, dword ptr [ebx]
        je     loc_3_2AC8
        mov    al, byte ptr [eax + 0x3c]
        jmp    loc_3_2ACB
loc_3_2AC8:
        mov    al, byte ptr [eax + 0x41]
loc_3_2ACB:
        and    eax, 0xff
        and    eax, 1
        lea    ecx, [ecx + eax + 8]
        mov    dword ptr [esp + 0x1c], ecx
        push   0
        push   ecx
        call   sub_3_43B0
        mov    edx, dword ptr [esp + 0x34]
        add    esp, 8
        mov    dword ptr [edx + 8], eax
        or     eax, eax
        je     loc_3_2E66
        mov    edx, dword ptr [esp + 0x38]
        mov    esi, dword ptr [ebx]
        mov    edi, eax
        add    esi, 0x3c
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_3_2B42
loc_3_2B10:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_2B20
        xor    edx, edx
        mov    dl, byte ptr [eax]
        jmp    loc_3_2B25
        db  08Dh, 049h, 000h
loc_3_2B20:
        xor    edx, edx
        mov    dl, byte ptr [eax + 5]
loc_3_2B25:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_2B32
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        jmp    loc_3_2B37
loc_3_2B32:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 5]
loc_3_2B37:
        and    ecx, 1
        lea    ecx, [edx + ecx + 8]
        mov    dword ptr [esp + 0x1c], ecx
loc_3_2B42:
        mov    ecx, dword ptr [esp + 0x10]
        and    ecx, 0xffff
        mov    edx, dword ptr [esp + 0x1c]
        sub    edx, ecx
        lea    edi, [eax + ecx]
        mov    esi, ebp
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x1c]
        inc    word ptr [ebx + 0x74]
        sub    ax, word ptr [esp + 0x10]
        mov    word ptr [esp + 0x10], ax
        jmp    loc_3_2B8F
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2B80:
        mov    word ptr [esp + 0x10], 0
        mov    dword ptr [esp + 0x1c], 0
loc_3_2B8F:
        mov    edx, dword ptr [esp + 0x2c]
        mov    cx, word ptr [esp + 0x1c]
        inc    dword ptr [esp + 0x24]
        mov    word ptr [edx + 2], cx
        jmp    loc_3_2BC2
loc_3_2BA2:
        mov    edx, dword ptr [edx + 0xfc]
        mov    ecx, dword ptr [esp + 0x34]
        sub    cl, al
        mov    esi, edx
        shr    edx, cl
        shl    dx, cl
        sub    si, dx
        mov    cl, al
        shl    si, cl
        mov    word ptr [esp + 0x10], si
loc_3_2BC2:
        mov    eax, dword ptr [esp + 0x10]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebx + 0x98]
        jbe    loc_3_2C05
        cmp    dword ptr [esp + 0x24], 0
        je     loc_3_2E66
        mov    esi, dword ptr [esp + 0x24]
        dec    word ptr [ebx + 0x74]
        mov    ecx, dword ptr [esp + 0x30]
        lea    eax, [esi + esi*2]
        mov    word ptr [ecx + eax*4 - 0xa], 0
        mov    dword ptr [ebx + 0x98], 0
        mov    word ptr [esp + 0x10], 0
loc_3_2C05:
        mov    eax, dword ptr [esp + 0x10]
        and    eax, 0xffff
        mov    edx, dword ptr [ebx + 0x98]
        sub    edx, eax
        mov    dword ptr [ebx + 0x98], edx
        add    ebp, eax
        mov    ax, word ptr [ebx + 0x74]
        mov    esi, dword ptr [esp + 0x24]
        mov    word ptr [esp + 0x12], ax
        mov    ecx, dword ptr [esp + 0x30]
        lea    eax, [esi + esi*2]
        lea    esi, [ecx + eax*4]
        mov    dword ptr [esp + 0x18], esi
        mov    ax, word ptr [esp + 0x12]
        mov    word ptr [esi], ax
        mov    ax, word ptr [esp + 0x10]
        or     edx, edx
        mov    word ptr [esi + 4], ax
        je     loc_3_2D18
        mov    eax, 1
        mov    cl, byte ptr [esp + 0x34]
        shl    eax, cl
        mov    dword ptr [esp + 0x28], eax
loc_3_2C61:
        mov    esi, dword ptr [esp + 0x10]
        and    esi, 0xffff
        mov    eax, dword ptr [esp + 0x28]
        sub    eax, esi
        cmp    eax, 8
        jl     loc_3_2D18
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_2C88
        xor    edx, edx
        mov    dl, byte ptr [ebp]
        jmp    loc_3_2C8D
loc_3_2C88:
        xor    edx, edx
        mov    dl, byte ptr [ebp + 5]
loc_3_2C8D:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_2C9B
        xor    eax, eax
        mov    al, byte ptr [ebp]
        jmp    loc_3_2CA0
loc_3_2C9B:
        xor    eax, eax
        mov    al, byte ptr [ebp + 5]
loc_3_2CA0:
        and    eax, 1
        lea    edx, [edx + eax + 8]
        mov    eax, 1
        shl    eax, cl
        sub    eax, esi
        cmp    eax, edx
        jb     loc_3_2D18
        mov    edi, dword ptr [ebx + 0x98]
        cmp    edx, edi
        ja     loc_3_2D03
        mov    ax, word ptr [ebx + 0x74]
        inc    ax
        mov    word ptr [ebx + 0x74], ax
        xor    esi, esi
        mov    si, word ptr [esp + 0x12]
        and    eax, 0xffff
        sub    eax, esi
        cmp    eax, 0x40
        jne    loc_3_2CEC
        mov    eax, ebp
        mov    esi, dword ptr [esp + 0x18]
        sub    eax, dword ptr [ebx + 0x4c]
        sub    ax, word ptr [esi + 4]
        mov    word ptr [esi + 6], ax
loc_3_2CEC:
        add    word ptr [esp + 0x10], dx
        add    ebp, edx
        sub    edi, edx
        mov    dword ptr [ebx + 0x98], edi
        jne    loc_3_2C61
        jmp    loc_3_2D18
loc_3_2D03:
        cmp    word ptr [ebx + 0x74], 1
        jbe    loc_3_2E66
        mov    dword ptr [ebx + 0x98], 0
loc_3_2D18:
        mov    esi, dword ptr [esp + 0x18]
        mov    ax, word ptr [esp + 0x10]
        sub    ax, word ptr [esi + 4]
        mov    word ptr [esi + 2], ax
        mov    esi, dword ptr [ebx + 0x98]
        or     esi, esi
        je     loc_3_2D6E
        mov    eax, 1
        mov    cl, byte ptr [esp + 0x34]
        shl    eax, cl
        mov    edx, dword ptr [esp + 0x10]
        and    edx, 0xffff
        sub    eax, edx
        mov    dword ptr [esp + 0x1c], eax
        mov    ax, 1
        shl    ax, cl
        dec    ax
        and    word ptr [esp + 0x10], ax
        je     loc_3_2D88
        cmp    dword ptr [esp + 0x1c], esi
        jb     loc_3_2D88
loc_3_2D64:
        mov    dword ptr [ebx + 0x98], 0
loc_3_2D6E:
        push   ebx
        call   sub_3_83B0
        add    esp, 4
loc_3_2D77:
        push   ebx
        call   sub_3_75B0
        add    esp, 4
loc_3_2D80:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
loc_3_2D88:
        inc    dword ptr [esp + 0x24]
        mov    esi, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp + 0x30]
        lea    eax, [esi + esi*2]
        lea    esi, [ecx + eax*4]
        mov    ax, word ptr [esp + 0x10]
        mov    word ptr [esi + 4], ax
        or     ax, ax
        je     loc_3_2E49
        mov    ax, word ptr [esp + 0x1c]
        mov    word ptr [esp + 0x10], ax
        cmp    ax, 8
        jae    loc_3_2DC3
        mov    edi, dword ptr [ebx]
        add    edi, 0x3c
        jmp    loc_3_2E08
loc_3_2DC3:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_2DD1
        xor    eax, eax
        mov    al, byte ptr [ebp]
        jmp    loc_3_2DD6
loc_3_2DD1:
        xor    eax, eax
        mov    al, byte ptr [ebp + 5]
loc_3_2DD6:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_2DE4
        xor    ecx, ecx
        mov    cl, byte ptr [ebp]
        jmp    loc_3_2DE9
loc_3_2DE4:
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 5]
loc_3_2DE9:
        and    ecx, 1
        lea    eax, [eax + ecx + 8]
        mov    dword ptr [esp + 0x1c], eax
        push   0
        push   eax
        call   sub_3_43B0
        add    esp, 8
        mov    edi, eax
        mov    dword ptr [esi + 8], edi
        or     edi, edi
        je     loc_3_2E66
loc_3_2E08:
        mov    eax, dword ptr [esp + 0x1c]
        cmp    dword ptr [ebx + 0x98], eax
        jae    loc_3_2E30
        mov    word ptr [esi + 2], 0
        mov    ax, word ptr [ebx + 0x74]
        mov    word ptr [esi + 0xc], ax
        mov    word ptr [esi], ax
        jmp    loc_3_2D64
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_2E30:
        mov    eax, dword ptr [esp + 0x10]
        and    eax, 0xffff
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_2E49:
        mov    eax, dword ptr [esp + 0x10]
        and    eax, 0xffff
        mov    si, word ptr [esp + 0x24]
        sub    dword ptr [ebx + 0x98], eax
        mov    word ptr [ebx + 0x64], si
        jmp    loc_3_2D77
loc_3_2E66:
        mov    dword ptr [ebx + 0x24], 8
        jmp    loc_3_2D6E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2E80:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x24], 0
        je     loc_3_2E90
        mov    word ptr [eax + 0x2e], 0x15
loc_3_2E90:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2EA0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x14]
        test   byte ptr [ecx + 0x1f], 2
        jne    loc_3_2EB8
        push   0x1c
        push   eax
        call   sub_3_8380
        add    esp, 8
loc_3_2EB8:
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2EC0:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x24], 0
        mov    ecx, dword ptr [eax + 0x14]
        test   byte ptr [ecx + 0x1f], 2
        je     loc_3_2EDD
        push   eax
        call   sub_3_83B0
        add    esp, 4
loc_3_2EDD:
        ret    
        db  08Bh, 0FFh
loc_3_2EE0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        push   eax
        mov    word ptr [eax + 0x20], 0
        mov    dword ptr [eax + 0x28], 1
        call   sub_3_7840
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2F10:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x24], 0
        je     loc_3_2F20
        mov    word ptr [eax + 0x2e], 0x15
loc_3_2F20:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2F30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    al, byte ptr [0x3050]         ; -> loc_3_3050
        mov    ebp, dword ptr [0x9bd0]       ; -> obj3:0x9BD0
        or     ebp, ebp
        mov    ebx, dword ptr [esp + 0x14]
        je     loc_3_2FDB
        mov    edx, 2
loc_3_2F50:
        mov    edi, dword ptr [ebx + 0x14]
        test   byte ptr [edi + 0x1f], dl
        mov    eax, dword ptr [edi + 8]
        jne    loc_3_2F79
        cmp    dword ptr [ebp + 8], eax
        jne    loc_3_2FD0
        mov    eax, dword ptr [edi + 0x3c]
        cmp    dword ptr [ebp + 0x3c], eax
        jne    loc_3_2FD0
        lea    esi, [ebp + 0x58]
        lea    edi, [edi + 0x58]
        mov    ecx, 0x2b
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_2FD0
        jmp    loc_3_2FDB
loc_3_2F79:
        cmp    dword ptr [ebp + 8], eax
        jne    loc_3_2FD0
        mov    eax, dword ptr [edi + 0x138]
        cmp    dword ptr [ebp + 0x138], eax
        jne    loc_3_2FD0
        mov    eax, dword ptr [edi + 0x13c]
        cmp    dword ptr [ebp + 0x13c], eax
        jne    loc_3_2FD0
        mov    eax, dword ptr [edi + 0x140]
        cmp    dword ptr [ebp + 0x140], eax
        jne    loc_3_2FD0
        mov    eax, dword ptr [edi + 0x3c]
        cmp    dword ptr [ebp + 0x3c], eax
        jne    loc_3_2FD0
        mov    eax, dword ptr [ebp + 0x130]
        lea    ecx, [eax*8]
        shr    ecx, 2
        mov    esi, dword ptr [ebp + 0x134]
        mov    edi, dword ptr [edi + 0x134]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_3_2FDB
loc_3_2FD0:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    loc_3_2F50
loc_3_2FDB:
        or     ebp, ebp
        jne    loc_3_3020
        mov    esi, dword ptr [ebx + 0x14]
        mov    eax, dword ptr [0x9bd0]       ; -> obj3:0x9BD0
        mov    dword ptr [esi], eax
        mov    dword ptr [0x9bd0], esi       ; -> obj3:0x9BD0
        push   esi
        call   sub_3_77B0
        add    esp, 4
        push   ebx
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 0x1c], esi
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax + 0x14], esi
        mov    dword ptr [eax + 0x18], 0x1930 ; -> sub_3_1930
        mov    dword ptr [ebx + 0x24], 0
        jmp    loc_3_3035
        db  08Dh, 064h, 024h, 000h
loc_3_3020:
        mov    word ptr [ebx + 0x2e], 0x15
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ecx + 0x1c], eax
        mov    dword ptr [ebx + 0x24], 0x11c
loc_3_3035:
        mov    eax, dword ptr [ebx]
        pop    ebp
        mov    dword ptr [eax + 0x10], 0x944 ; -> loc_1_0944
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 0x24]
        mov    word ptr [eax + 0x1a], cx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_3050:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3060:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_75B0
        add    esp, 4
        cmp    dword ptr [esi + 0x14], 0
        je     loc_3_3084
        push   esi
        call   sub_3_4660
        add    esp, 4
        mov    dword ptr [esi + 0x44], 0
loc_3_3084:
        mov    eax, dword ptr [esi + 0x44]
        or     eax, eax
        je     loc_3_30A1
        mov    ecx, dword ptr [esi + 0x20]
        push   ecx
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0xc
        mov    dword ptr [esi + 0x44], 0
loc_3_30A1:
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 0x24]
        mov    word ptr [eax + 0x1a], cx
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_30C0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 0xc]
        mov    ax, word ptr [esi + 0x28]
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 6], 0
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 0x44], eax
        or     dword ptr [edi + 8], 8
        mov    al, byte ptr [esi + 0x10]
        mov    byte ptr [edi + 0x4d], al
        cmp    word ptr [esi + 0x28], 0xd
        jne    loc_3_311D
        mov    eax, dword ptr [esi + 0x10]
        inc    ax
        mov    word ptr [edi + 0x2c], ax
        mov    ax, word ptr [esi + 0x2a]
        mov    word ptr [edi + 0x2e], ax
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [edi + 0x30], eax
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [edi + 0x34], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 0x38], eax
        mov    dword ptr [edi + 0x3c], 1
loc_3_311D:
        push   edi
        call   sub_3_57E0
        add    esp, 4
        cmp    word ptr [esi + 0x28], 0xd
        jne    loc_3_3136
        mov    eax, dword ptr [edi + 0x3c]
        mov    dword ptr [esi + 0x2c], eax
        pop    edi
        pop    esi
        ret    
loc_3_3136:
        mov    ax, word ptr [edi + 6]
        cmp    ax, 0x10
        jb     loc_3_3154
        and    eax, 0xffff
        push   eax
        call   sub_3_57F5
        add    esp, 4
        mov    dword ptr [esi + 0x2c], eax
        pop    edi
        pop    esi
        ret    
loc_3_3154:
        mov    dword ptr [esi + 0x2c], 0
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_3_3160:
        mov    ecx, dword ptr [esp + 4]
        cmp    dword ptr [ecx + 8], 0
        je     loc_3_3197
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 0x1c]
        sub    eax, 0xc3
        cmp    byte ptr [eax + 0x40], 5
        jne    loc_3_3197
        test   byte ptr [eax + 0x47e], 1
        je     loc_3_3197
        mov    eax, dword ptr [ecx + 0x14]
        mov    ecx, dword ptr [ecx + 0x14]
        mov    ax, word ptr [eax + 3]
        or     ax, 0x200
        mov    word ptr [ecx + 3], ax
        ret    
loc_3_3197:
        mov    eax, dword ptr [ecx + 0x14]
        mov    ecx, dword ptr [ecx + 0x14]
        mov    ax, word ptr [eax + 3]
        and    ax, 0xfdff
        mov    word ptr [ecx + 3], ax
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_31B0:
        VxDCall  IFSMgr.IFSMgr_CDROM_Attach      ; cd202f804000
sub_3_31B6:
        VxDCall  IFSMgr.IFSMgr_CDROM_Detach      ; cd2030804000
loc_3_31BC:
        push   ebx
        push   ebp
        call   sub_3_3240
        lea    esp, [esp + 8]
loc_3_31C7:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
        db  000h, 000h
loc_3_31D0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x18]
        mov    ecx, dword ptr [0x890]        ; -> obj1:0x0890
        lea    edx, [eax + eax*2]
        lea    edi, [ecx + edx*2 + 0x14]
        cmp    eax, 0x1a
        jae    loc_3_3225
        mov    al, byte ptr [edi + 1]
        test   al, 8
        je     loc_3_3225
        test   al, 0x10
        jne    loc_3_3200
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
loc_3_3200:
        push   0x3810
        call   sub_3_57FF
        add    esp, 4
        mov    cl, byte ptr [edi]
        mov    byte ptr [eax + 1], cl
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        push   esi
        call   sub_3_3240
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_3_3225:
        mov    word ptr [esi + 0x1c], 0xf
        or     dword ptr [esi + 0x2c], 1
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3240:
        sub    esp, 0x34
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [esp], eax
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [esp + 4], eax
        push   0x3810
        call   sub_3_57FF
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esp + 0x28], 0
        mov    dword ptr [esp + 0x34], 0x500
        lea    eax, [esp + 4]
        add    esp, 4
        push   eax
        call   0xfffd0990                    ; -> obj1:0x0990
        add    esp, 4
        or     eax, eax
        je     loc_3_3290
        lea    ecx, [esp]
        push   ecx
        call   eax
        add    esp, 0x38
        ret    
loc_3_3290:
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax + 3], 0x8103
        add    esp, 0x34
        ret    
        db  08Bh, 0FFh
loc_3_32A0:
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd04
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0xc
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esi + 0x18]
        push   esi
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        mov    dword ptr [esp + 0xc], eax
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_32F0
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 2], ecx
loc_3_32F0:
        pop    esi
        add    esp, 0xc
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3300:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd02
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 8
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_3341
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 1], eax
loc_3_3341:
        pop    esi
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3350:
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd07
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0x10
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_33A2
        mov    al, byte ptr [esp + 8]
        and    al, 0xf
        cmp    al, 1
        jne    loc_3_33C2
        lea    ecx, [esp + 9]
        mov    al, byte ptr [ecx]
        aam    0xa
        shl    ah, 4
        or     al, ah
        mov    byte ptr [ecx], al
        jmp    loc_3_33C2
loc_3_33A2:
        lea    eax, [esp + 8]
        push   esi
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    word ptr [eax + 8], 0
        call   sub_3_3900
        add    esp, 4
loc_3_33C2:
        mov    edx, dword ptr [esi + 0x18]
        lea    ecx, [esp + 8]
        inc    edx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [edx + 8], ax
        pop    esi
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_33F0:
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 0x28], 0xd
        mov    word ptr [esi + 0x2a], 0xcd08
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], 0x10
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_3446
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [ecx + 1], ax
        mov    ecx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 3], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 7], ecx
loc_3_3446:
        pop    esi
        add    esp, 0x10
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_3450:
        sub    esp, 0x80
        push   esi
        push   edi
        lea    eax, [esp + 0x14]
        mov    esi, dword ptr [esp + 0x8c]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    al, byte ptr [eax + 1]
        push   eax
        call   sub_3_3920
        add    esp, 4
        mov    dword ptr [esi + 0x10], eax
        cmp    dword ptr [esi + 0x10], -1
        je     loc_3_35D1
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_9AC0
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 0x10
        cmp    dword ptr [eax + 0x10], 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        jne    loc_3_35D1
        mov    eax, dword ptr [esp + 8]
        mov    edi, dword ptr [esi + 0x14]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 0xe]
        add    edi, 0xd
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_3_5958
        mov    ecx, dword ptr [esp + 0x10]
        add    esp, 8
        mov    dword ptr [ecx + 0x14], eax
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [edi + 5]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        mov    al, byte ptr [edi]
        mov    dword ptr [esp + 0xc], eax
        mov    eax, dword ptr [edi + 7]
        mov    dword ptr [esp + 0x10], eax
        cmp    dword ptr [esp + 0xc], 2
        jae    loc_3_3590
        cmp    byte ptr [edi + 0xb], 2
        jae    loc_3_3590
        cmp    dword ptr [esp + 0xc], 0
        je     loc_3_3547
        xor    ecx, ecx
        mov    cl, byte ptr [esp + 0x12]
        shl    ecx, 2
        mov    dword ptr [esp + 0xc], 0
        lea    ecx, [ecx + ecx*2]
        xor    eax, eax
        mov    al, byte ptr [esp + 0x11]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        xor    eax, eax
        lea    ecx, [ecx + ecx*4]
        mov    al, byte ptr [esp + 0x10]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        sub    ecx, 0x96
        mov    dword ptr [esp + 0x10], ecx
loc_3_3547:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x24], eax
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        mov    al, byte ptr [edi + 0xb]
        push   0x40
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [esp + 0xc]
        push   5
        mov    dword ptr [eax + 0x3c], 0
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        lea    ecx, [esi + 0x2c]
        mov    eax, dword ptr [esp + 8]
        movsx  eax, word ptr [eax + 0x1a]
        mov    dword ptr [ecx], eax
        jmp    loc_3_3599
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3590:
        lea    ecx, [esi + 0x2c]
        mov    dword ptr [ecx], 0x16
loc_3_3599:
        cmp    dword ptr [ecx], 0
        jne    loc_3_35A9
        mov    eax, dword ptr [esi + 0x14]
        mov    word ptr [eax + 3], 0x100
        jmp    loc_3_35B2
loc_3_35A9:
        push   esi
        call   sub_3_3820
        add    esp, 4
loc_3_35B2:
        push   esi
        call   sub_3_3160
        mov    eax, dword ptr [esp + 0xc]
        add    esp, 4
        push   eax
        call   sub_3_9BA0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 0x80
        ret    
loc_3_35D1:
        mov    eax, dword ptr [esi + 0x14]
        pop    edi
        mov    word ptr [eax + 3], 0x8101
        pop    esi
        add    esp, 0x80
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_35F0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        push   1
        lea    esi, [eax + 8]
        push   esi
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_3_31B0
        add    esp, 0xc
        or     eax, eax
        je     loc_3_3611
        mov    dword ptr [esi], 0
loc_3_3611:
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3620:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_3_31B6
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3640:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        mov    al, byte ptr [eax + 1]
        push   eax
        call   sub_3_3920
        add    esp, 4
        mov    dword ptr [esi + 0x10], eax
        cmp    dword ptr [esi + 0x10], -1
        je     loc_3_36A4
        push   esi
        call   sub_3_35F0
        add    esp, 4
        cmp    dword ptr [esi + 8], 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        jne    loc_3_36A4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_3_36E0
        add    esp, 4
        mov    dword ptr [esi + 0xc], eax
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_3_36A0
        push   esi
        call   sub_3_3620
        add    esp, 4
        mov    eax, dword ptr [esi + 0x14]
        pop    esi
        mov    word ptr [eax + 3], 0x810c
        mov    eax, 1
        ret    
loc_3_36A0:
        sub    eax, eax
        pop    esi
        ret    
loc_3_36A4:
        mov    eax, dword ptr [esi + 0x14]
        pop    esi
        mov    word ptr [eax + 3], 0x8101
        mov    eax, 1
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_36C0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0xc]
        push   eax
        call   sub_3_3780
        add    esp, 4
        push   esi
        call   sub_3_3620
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
sub_3_36E0:
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        mov    word ptr [esp + 4], 2
        mov    ax, word ptr [esi + 8]
        shl    ax, 3
        add    ax, word ptr [esi + 0xa]
        mov    word ptr [esp + 8], ax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [esp + 0xa], eax
        lea    eax, [esp + 4]
        mov    byte ptr [esp + 0x12], 2
        push   eax
        call   sub_1_08A8
        mov    eax, dword ptr [esp + 0xe]
        add    eax, dword ptr [esp + 0x12]
        add    esp, 4
        lea    ecx, [eax + 0x58]
        mov    dword ptr [eax + 0x48], ecx
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0xd
        mov    word ptr [eax + 6], 0
        mov    dword ptr [eax + 8], 0x500
        mov    dword ptr [eax + 0xc], 0x8580 ; -> sub_3_8580
        mov    dword ptr [eax + 0x40], 0
        mov    dword ptr [eax + 0x10], 0
        mov    dword ptr [eax + 0x14], 0
        mov    dword ptr [eax + 0x18], 0
        mov    byte ptr [eax + 0x4c], 0
        mov    ecx, dword ptr [esp + 0xa]
        mov    dword ptr [eax + 0x20], ecx
        mov    dword ptr [eax + 0x44], esi
        pop    esi
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3780:
        sub    esp, 8
        mov    eax, dword ptr [esp + 0xc]
        mov    word ptr [esp], 4
        sub    eax, dword ptr [eax + 0x20]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp]
        push   eax
        call   sub_1_08A8
        add    esp, 0xc
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_37B0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   edi
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        je     loc_3_37D0
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_37D0:
        push   edi
        call   sub_3_30C0
        add    esp, 4
        cmp    dword ptr [edi + 0x2c], 0
        jne    loc_3_37F0
        mov    eax, dword ptr [edi + 0x14]
        sub    esi, esi
        mov    word ptr [eax + 3], 0x100
        jmp    loc_3_37FE
        db  08Dh, 064h, 024h, 000h
loc_3_37F0:
        push   edi
        call   sub_3_3820
        add    esp, 4
        mov    esi, 1
loc_3_37FE:
        push   edi
        call   sub_3_3160
        add    esp, 4
        push   edi
        call   sub_3_36C0
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3820:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 0x14]
        mov    word ptr [eax + 3], 0x8100
        mov    eax, dword ptr [edx + 0x2c]
        sub    eax, 0x13
        cmp    eax, 0xf
        ja     loc_3_3840
        jmp    dword ptr [eax*4 + 0x38c0]    ; -> obj3:0x38C0
        db  090h
loc_3_3840:
        mov    eax, 2
        jmp    loc_3_38AC
loc_3_3847:
        sub    eax, eax
        jmp    loc_3_38AC
        db  005h, 000h, 000h, 000h, 000h
loc_3_3850:
        mov    eax, 1
        jmp    loc_3_38AC
loc_3_3857:
        mov    eax, 3
        jmp    loc_3_38AC
        db  08Bh, 0FFh
loc_3_3860:
        mov    eax, 4
        jmp    loc_3_38AC
loc_3_3867:
        mov    eax, 5
        jmp    loc_3_38AC
        db  08Bh, 0FFh
loc_3_3870:
        mov    eax, 6
        jmp    loc_3_38AC
loc_3_3877:
        mov    eax, 7
        jmp    loc_3_38AC
        db  08Bh, 0FFh
loc_3_3880:
        mov    eax, 8
        jmp    loc_3_38AC
loc_3_3887:
        mov    eax, 9
        jmp    loc_3_38AC
        db  08Bh, 0FFh
loc_3_3890:
        mov    eax, 0xa
        jmp    loc_3_38AC
loc_3_3897:
        mov    eax, 0xb
        jmp    loc_3_38AC
        db  08Bh, 0FFh
loc_3_38A0:
        mov    eax, 0xc
        jmp    loc_3_38AC
loc_3_38A7:
        mov    eax, 0xf
loc_3_38AC:
        mov    ecx, dword ptr [edx + 0x14]
        mov    cx, word ptr [ecx + 3]
        or     cx, ax
        mov    eax, dword ptr [edx + 0x14]
        mov    word ptr [eax + 3], cx
        ret    
        db  08Bh, 0FFh
        ; ---- dispatch table (16 entries) ----
        dd  loc_3_3847
        dd  loc_3_3850
        dd  loc_3_3840
        dd  loc_3_3857
        dd  loc_3_3860
        dd  loc_3_3867
        dd  loc_3_3870
        dd  loc_3_3877
        dd  loc_3_3880
        dd  loc_3_3887
        dd  loc_3_3890
        dd  loc_3_3897
        dd  loc_3_38A0
        dd  loc_3_3840
        dd  loc_3_3840
        dd  loc_3_38A7
sub_3_3900:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x14]
        add    eax, 3
        cmp    byte ptr [eax], 0xc
        jne    loc_3_3912
        mov    byte ptr [eax], 2
loc_3_3912:
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3920:
        push   ebx
        mov    ecx, dword ptr [0x890]        ; -> obj1:0x0890
        add    ecx, 0x14
        sub    eax, eax
        inc    ecx
        mov    dl, byte ptr [esp + 8]
loc_3_3931:
        mov    bl, byte ptr [ecx]
        and    bl, 0x18
        cmp    bl, 0x18
        jne    loc_3_3940
        cmp    byte ptr [ecx - 1], dl
        je     loc_3_394E
loc_3_3940:
        add    ecx, 6
        inc    eax
        cmp    eax, 0x1a
        jb     loc_3_3931
        mov    eax, 0xffffffff
loc_3_394E:
        pop    ebx
        ret    
loc_3_3950:
        push   esi
        mov    ecx, dword ptr [0x890]        ; -> obj1:0x0890
        add    ecx, 0x14
        sub    dx, dx
        sub    si, si
        sub    eax, eax
        inc    ecx
loc_3_3963:
        test   byte ptr [ecx], 8
        je     loc_3_3972
        or     dx, dx
        jne    loc_3_3970
        mov    si, ax
loc_3_3970:
        inc    dx
loc_3_3972:
        add    ecx, 6
        inc    eax
        cmp    eax, 0x1a
        jb     loc_3_3963
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax + 0x1c], 0xff
        and    dword ptr [eax + 0x2c], 0xfffffffe
        mov    word ptr [eax + 0x10], dx
        mov    word ptr [eax + 0x18], si
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_39A0:
        push   esi
        mov    esi, dword ptr [0x890]        ; -> obj1:0x0890
        add    esi, 0x14
        push   0x3810
        call   sub_3_57FF
        add    esp, 4
        inc    esi
        mov    ecx, 0x1a
        lea    ecx, [ecx]
loc_3_39C0:
        test   byte ptr [esi], 8
        je     loc_3_39D3
        mov    dl, byte ptr [esi - 1]
        mov    byte ptr [eax], dl
        inc    eax
        mov    edx, dword ptr [esi + 1]
        mov    dword ptr [eax], edx
        add    eax, 4
loc_3_39D3:
        add    esi, 6
        dec    ecx
        jne    loc_3_39C0
        mov    eax, dword ptr [esp + 8]
        pop    esi
        and    dword ptr [eax + 0x2c], 0xfffffffe
        sub    eax, eax
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_39F0:
        mov    edx, dword ptr [esp + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 0x18]
        cmp    eax, 0x1a
        jae    loc_3_3A13
        lea    ecx, [eax + eax*2]
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        test   byte ptr [eax + ecx*2 + 0x15], 8
        mov    word ptr [edx + 0x1c], 1
        jne    loc_3_3A19
loc_3_3A13:
        mov    word ptr [edx + 0x1c], 0
loc_3_3A19:
        mov    word ptr [edx + 0x10], 0xadad
        and    dword ptr [edx + 0x2c], 0xfffffffe
        sub    eax, eax
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3A30:
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        mov    cx, word ptr [eax + 0xa]
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x10], cx
        and    dword ptr [eax + 0x2c], 0xfffffffe
        sub    eax, eax
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_3A50:
        push   ebx
        push   esi
        mov    esi, dword ptr [0x890]        ; -> obj1:0x0890
        add    esi, 0x14
        push   0x3810
        call   sub_3_57FF
        add    esp, 4
        sub    dl, dl
        mov    ebx, 8
        nop    
loc_3_3A70:
        xor    ecx, ecx
        mov    cl, dl
        lea    ecx, [ecx + ecx*2]
        test   byte ptr [esi + ecx*2 + 1], bl
        je     loc_3_3A80
        mov    byte ptr [eax], dl
        inc    eax
loc_3_3A80:
        inc    dl
        cmp    dl, 0x1a
        jb     loc_3_3A70
        mov    eax, dword ptr [esp + 0xc]
        pop    esi
        and    dword ptr [eax + 0x2c], 0xfffffffe
        sub    eax, eax
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3AA0:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 4]
        cmp    ecx, 2
        jl     loc_3_3AB3
        cmp    ecx, 4
        jle    loc_3_3AC0
loc_3_3AB3:
        mov    word ptr [eax + 0x1a], 5
        mov    eax, 5
        ret    
        db  090h
loc_3_3AC0:
        push   0x29
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh
loc_3_3AD0:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax + 4], 0
        je     loc_3_3AE6
        mov    word ptr [eax + 0x1a], 5
        mov    eax, 5
        ret    
loc_3_3AE6:
        push   0x2b
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3B00:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [eax + 4]
        and    cl, 7
        cmp    cl, 1
        je     loc_3_3B38
        mov    cx, word ptr [eax + 0x18]
        and    cx, 0xff
        cmp    cx, 0x10
        je     loc_3_3B38
        cmp    cx, 0x12
        je     loc_3_3B38
        cmp    cx, 2
        je     loc_3_3B38
        push   0x2d
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
loc_3_3B38:
        mov    word ptr [eax + 0x1a], 5
        mov    eax, 5
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3B50:
        sub    esp, 0x90
        mov    eax, dword ptr [esp + 0x94]
        lea    ecx, [esp]
        mov    dword ptr [eax + 0x3c], ecx
        push   0x31
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0x9c
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3B80:
        push   0x33
        mov    eax, dword ptr [esp + 8]
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3BA0:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 4]
        cmp    ecx, 7
        ja     loc_3_3BB5
        jmp    loc_3_3BEC
loc_3_3BB5:
        mov    word ptr [eax + 0x1a], 1
        mov    eax, 1
        ret    
loc_3_3BC1:
        push   0x35
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  090h
loc_3_3BD0:
        mov    word ptr [eax + 0x1a], 0
        sub    eax, eax
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3BE0:
        mov    word ptr [eax + 0x1a], 5
        mov    eax, 5
        ret    
loc_3_3BEC:
        sar    dword ptr [ebx], 0
        add    al, dl                        ; -> loc_3_3BD0
        cmp    eax, dword ptr [eax]
        add    byte ptr [ebp - 0x4affffc5], dh ; -> loc_3_3BB5
        cmp    eax, dword ptr [eax]
        add    cl, al                        ; -> loc_3_3BC1
        cmp    eax, dword ptr [eax]
        add    al, ah                        ; -> loc_3_3BE0
        cmp    eax, dword ptr [eax]
        add    cl, al                        ; -> loc_3_3BC1
        cmp    eax, dword ptr [eax]
        add    al, ah                        ; -> loc_3_3BE0
        cmp    eax, dword ptr [eax]
        add    byte ptr [ebp - 0x74ffdb9c], cl
        inc    esp
        and    al, 4
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 4]
        or     ecx, ecx
        je     loc_3_3C29
        mov    word ptr [eax + 0x1a], 1
        mov    eax, 1
        ret    
loc_3_3C29:
        push   0x4d
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3C40:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 5
        mov    eax, 5
        ret    
loc_3_3C50:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 3], 0x24
        je     loc_3_3C64
        or     dword ptr [esi + 0x20], 0x8000
loc_3_3C64:
        push   esi
        call   sub_3_54A0
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0
        je     loc_3_3C82
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 0x24]
        mov    word ptr [ecx + 0x1a], ax
        or     dword ptr [esi + 0x20], 0x10
        pop    esi
        ret    
loc_3_3C82:
        mov    byte ptr [esi + 0x6d], 1
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 0x70], 0x16
        cmp    byte ptr [eax + 4], 2
        jne    loc_3_3CAC
        mov    eax, dword ptr [esi + 0x10]
        push   0x22
        mov    cx, word ptr [eax + 0x48]
        mov    word ptr [eax + 0x4a], cx
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
loc_3_3CAC:
        push   0x25
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3CC0:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [edx + 0x10]
        cmp    dword ptr [edx + 0x24], 0
        jne    loc_3_3D70
        mov    eax, dword ptr [edx]
        cmp    byte ptr [eax + 4], 2
        jne    loc_3_3CF2
        xor    eax, eax
        mov    ax, word ptr [esi + 0x48]
        sub    eax, dword ptr [esi]
        add    eax, esi
        cmp    eax, -0x4c
        je     loc_3_3CF2
        mov    dword ptr [edx + 0x24], 3
loc_3_3CF2:
        cmp    dword ptr [edx + 0x24], 0
        jne    loc_3_3D70
        mov    eax, dword ptr [edx]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 3
        je     loc_3_3D10
        cmp    eax, 4
        je     loc_3_3D25
        jmp    loc_3_3D70
        db  08Bh, 0FFh
loc_3_3D10:
        mov    eax, dword ptr [esi + 8]
        mov    cx, word ptr [esi + 0x2e]
        mov    bx, word ptr [eax]
        add    bx, cx
        add    esi, 0x4dc
        jmp    loc_3_3D38
loc_3_3D25:
        mov    eax, dword ptr [esi + 4]
        mov    cx, word ptr [esi + 0x2a]
        mov    bx, word ptr [eax]
        add    bx, cx
        add    esi, 0x254
loc_3_3D38:
        mov    eax, dword ptr [edx]
        mov    edi, dword ptr [eax + 0x10]
        add    cx, 4
        mov    word ptr [edi + 2], cx
        lea    eax, [ebx + 4]
        mov    word ptr [edi], ax
        and    ebx, 0xffff
        mov    eax, ebx
        and    eax, 0xfffffffe
        mov    word ptr [esi + eax], 0
        add    ebx, 2
        add    edi, 4
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_3D70:
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 0x24]
        mov    word ptr [eax + 0x1a], cx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_3_3D80:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi + 0x70], 0x16
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 3], 0x24
        je     loc_3_3D9A
        or     dword ptr [esi + 0x20], 0x8000
loc_3_3D9A:
        push   esi
        call   sub_3_54A0
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0
        je     loc_3_3DB8
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 0x24]
        mov    word ptr [ecx + 0x1a], ax
        or     dword ptr [esi + 0x20], 0x10
        pop    esi
        ret    
loc_3_3DB8:
        mov    byte ptr [esi + 0x6d], 0
        push   0x25
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3DD0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi]
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_3DEE
        push   esi
        call   sub_3_94D0
        add    esp, 4
        and    eax, 0xff
        mov    dword ptr [edi], eax
loc_3_3DEE:
        test   byte ptr [esi + 0x21], 8
        je     loc_3_3DFB
        mov    dword ptr [esi + 0x24], 5
loc_3_3DFB:
        mov    eax, dword ptr [esi + 0x24]
        mov    word ptr [edi + 0x1a], ax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3E10:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [0x9c80]       ; -> obj3:0x9C80
        cmp    dword ptr [esi + 0x14], eax
        jne    loc_3_3E29
        mov    dword ptr [0x9c80], 0         ; -> obj3:0x9C80
loc_3_3E29:
        mov    eax, dword ptr [esi]
        mov    ax, word ptr [eax + 0x18]
        and    eax, 0xffff
        cmp    eax, 1
        jl     loc_3_3E4D
        cmp    eax, 2
        jg     loc_3_3E4D
        mov    eax, dword ptr [esi + 0x44]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
loc_3_3E4D:
        mov    eax, dword ptr [esi]
        pop    esi
        mov    word ptr [eax + 0x1a], 0
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3E60:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi + 0x70], 6
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 3], 0x24
        je     loc_3_3E7A
        or     dword ptr [esi + 0x20], 0x8000
loc_3_3E7A:
        push   esi
        call   sub_3_54A0
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0
        je     loc_3_3E98
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 0x24]
        mov    word ptr [ecx + 0x1a], ax
        or     dword ptr [esi + 0x20], 0x10
        pop    esi
        ret    
loc_3_3E98:
        mov    byte ptr [esi + 0x6d], 1
        push   0x25
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3EB0:
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 0x50]
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 0x1c]
        mov    ecx, dword ptr [ebx + 0x1c]
        mov    ax, word ptr [eax + 8]
        inc    ax
        mov    word ptr [ecx + 8], ax
        mov    esi, dword ptr [edx + 0x24]
        mov    dword ptr [ebx + 0x98], esi
        mov    eax, dword ptr [edx + 0x14]
        mov    dword ptr [ebx + 0x9c], eax
        mov    eax, dword ptr [edx]
        or     eax, eax
        mov    dword ptr [ebx + 0xa0], eax
        mov    dword ptr [ebx + 0xa4], 0
        mov    dword ptr [ebx + 0x24], 0
        mov    byte ptr [ebx + 0x63], 5
        mov    ecx, dword ptr [edx + 0x3c]
        mov    dword ptr [ebx + 0xb4], ecx
        je     loc_3_4177
        mov    eax, dword ptr [ebx + 0x18]
        test   word ptr [eax + 0x12], 0x4050
        je     loc_3_416C
        cmp    esi, 0x2c
        jae    loc_3_416C
        mov    eax, dword ptr [ebx + 0x18]
        test   byte ptr [eax + 0x13], 0x40
        je     loc_3_3F37
        mov    dword ptr [esp + 0x10], 0x9fa8 ; -> loc_3_9FA8
        jmp    loc_3_3F50
loc_3_3F37:
        mov    eax, dword ptr [ebx + 0x18]
        test   byte ptr [eax + 0x12], 0x40
        mov    dword ptr [esp + 0x10], 0x9f7c ; -> loc_3_9F7C
        jne    loc_3_3F50
        mov    dword ptr [esp + 0x10], 0x9fd4 ; -> loc_3_9FD4
loc_3_3F50:
        lea    edi, [esp + 0x20]
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, 0xb
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebx + 0x18]
        test   byte ptr [eax + 0x13], 0x40
        mov    eax, dword ptr [ebx + 0x18]
        jne    loc_3_401A
        mov    eax, dword ptr [eax + 0x34]
        sub    eax, 0x2c
        add    dword ptr [esp + 0x24], eax
        add    dword ptr [esp + 0x48], eax
        mov    eax, dword ptr [ebx + 0x18]
        test   byte ptr [eax + 0x12], 0x40
        mov    eax, dword ptr [ebx + 0x18]
        je     loc_3_3FE5
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        mov    cl, byte ptr [eax + eax*4 + 0xa054] ; -> loc_3_A054
        mov    al, cl
        and    eax, 1
        mov    dl, cl
        and    dl, 2
        shr    dl, 1
        and    edx, 0xff
        and    cl, 0xc
        shr    cl, 2
        and    ecx, 0xff
        mov    dword ptr [esp + 0x14], ecx
        mov    cl, al
        shr    word ptr [esp + 0x36], cl
        mov    cl, dl
        shr    word ptr [esp + 0x42], cl
        lea    ecx, [edx + eax]
        shr    word ptr [esp + 0x40], cl
        mov    cl, byte ptr [esp + 0x14]
        shr    dword ptr [esp + 0x38], cl
        add    cl, al
        add    cl, dl
        shr    dword ptr [esp + 0x3c], cl
        jmp    loc_3_4135
loc_3_3FE5:
        test   byte ptr [eax + 0x12], 0x10
        je     loc_3_4135
        mov    eax, dword ptr [ebx + 0x18]
        add    eax, 0x3c
        mov    dword ptr [esp + 0x10], eax
        mov    ecx, eax
        lea    edx, [esp + 0x34]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], eax
        jmp    loc_3_4135
loc_3_401A:
        xor    ecx, ecx
        mov    cx, word ptr [eax + 0x1a]
        lea    eax, [ecx - 1]
        sub    ecx, 2
        mov    word ptr [esp + 0x36], ax
        mov    eax, dword ptr [ebx + 0x14]
        mov    eax, dword ptr [eax + 0x3c]
        mov    dword ptr [esp + 0x38], eax
        mov    esi, 0x4b
        mov    eax, dword ptr [ebx + 0x14]
        sub    edx, edx
        mov    eax, dword ptr [eax + 0x134]
        mov    eax, dword ptr [eax + ecx*8]
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x3c], eax
        add    dword ptr [esp + 0x1c], 0x96
        mov    eax, dword ptr [esp + 0x1c]
        div    esi
        mov    byte ptr [esp + 0xc], dl
        mov    eax, dword ptr [esp + 0x1c]
        sub    edx, edx
        div    esi
        mov    dword ptr [esp + 0x1c], eax
        mov    esi, 0x3c
        sub    edx, edx
        div    esi
        mov    byte ptr [esp + 0xd], dl
        mov    eax, dword ptr [esp + 0x1c]
        sub    edx, edx
        div    esi
        mov    dword ptr [esp + 0x1c], eax
        mov    byte ptr [esp + 0xe], al
        mov    byte ptr [esp + 0xf], 0
        mov    byte ptr [esp + 0x1f], 0
        mov    byte ptr [esp + 0x1e], al
        mov    al, byte ptr [esp + 0xd]
        mov    byte ptr [esp + 0x1d], al
        mov    al, byte ptr [esp + 0xc]
        mov    byte ptr [esp + 0x1c], al
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esp + 0x44], eax
        mov    eax, dword ptr [ebx + 0x14]
        mov    eax, dword ptr [eax + 0x134]
        mov    eax, dword ptr [eax + ecx*8 + 4]
        mov    ecx, 0x4b
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x40], eax
        sub    dword ptr [esp + 0x1c], 0x96
        add    dword ptr [esp + 0x1c], 0x96
        sub    edx, edx
        mov    eax, dword ptr [esp + 0x1c]
        div    ecx
        mov    byte ptr [esp + 0xc], dl
        mov    eax, dword ptr [esp + 0x1c]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 0x1c], eax
        mov    ecx, esi
        sub    edx, edx
        div    ecx
        mov    byte ptr [esp + 0xd], dl
        mov    ecx, esi
        mov    eax, dword ptr [esp + 0x1c]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 0x1c], eax
        mov    byte ptr [esp + 0xe], al
        mov    byte ptr [esp + 0xf], 0
        mov    byte ptr [esp + 0x1f], 0
        mov    byte ptr [esp + 0x1e], al
        mov    al, byte ptr [esp + 0xd]
        mov    byte ptr [esp + 0x1d], al
        mov    al, byte ptr [esp + 0xc]
        mov    byte ptr [esp + 0x1c], al
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esp + 0x48], eax
loc_3_4135:
        mov    edx, dword ptr [ebx + 0xa0]
        mov    ecx, dword ptr [ebx + 0x98]
        mov    eax, 0x2c
        sub    eax, ecx
        cmp    edx, eax
        jbe    loc_3_414E
        mov    edx, eax
loc_3_414E:
        lea    esi, [esp + ecx + 0x20]
        mov    edi, dword ptr [ebx + 0x9c]
        mov    ecx, edx
        shr    ecx, 2
        add    dword ptr [ebx + 0xa4], edx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_416C:
        push   0x38
        push   ebx
        call   sub_3_8380
        add    esp, 8
loc_3_4177:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x40
        ret    
        db  08Bh, 0FFh
loc_3_4180:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi]
        cmp    dword ptr [esi + 0xa4], 0
        jne    loc_3_419A
        lea    ebx, [esi + 0x24]
        cmp    dword ptr [ebx], 0
        jne    loc_3_41C5
loc_3_419A:
        mov    eax, dword ptr [esi + 0xa4]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0xa4]
        add    dword ptr [edi + 0x24], eax
        lea    ebx, [esi + 0x24]
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        mov    ecx, dword ptr [esi + 0xa4]
        add    dword ptr [eax + 0xb4], ecx
        mov    dword ptr [ebx], 0
loc_3_41C5:
        push   esi
        call   sub_3_47C0
        add    esp, 4
        mov    eax, dword ptr [ebx]
        mov    word ptr [edi + 0x1a], ax
        or     dword ptr [esi + 0x20], 0x10
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_41E0:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 0x1c]
        mov    ecx, dword ptr [edx + 0x1c]
        mov    ax, word ptr [eax + 8]
        inc    ax
        mov    word ptr [ecx + 8], ax
        xor    eax, eax
        mov    al, byte ptr [esi + 4]
        or     eax, eax
        je     loc_3_422C
        cmp    eax, 2
        je     loc_3_4216
        mov    word ptr [esi + 0x1a], 1
        push   edx
        call   sub_3_47C0
        add    esp, 4
        pop    esi
        ret    
loc_3_4216:
        mov    eax, dword ptr [edx + 0x18]
        add    eax, 0x28
        je     loc_3_422C
        mov    edi, edi
loc_3_4220:
        mov    ecx, dword ptr [eax + 0xc]
        add    dword ptr [esi + 0x24], ecx
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_3_4220
loc_3_422C:
        mov    word ptr [esi + 0x1a], 0
        push   edx
        call   sub_3_47C0
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_3_4240:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        mov    eax, dword ptr [esi + 0x1c]
        mov    cl, byte ptr [eax + 0xb]
        mov    eax, dword ptr [esi + 0x1c]
        or     cl, 1
        mov    byte ptr [eax + 0xb], cl
        call   sub_3_47C0
        add    esp, 4
        mov    eax, dword ptr [esi]
        pop    esi
        mov    word ptr [eax + 0x1a], 0
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4270:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x1c]
        mov    ax, word ptr [eax + 8]
        inc    ax
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 4], 4
        je     loc_3_42D0
        mov    eax, dword ptr [esi + 0x18]
        add    eax, 0x1c
        push   eax
        call   sub_3_4850
        add    esp, 4
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 4], 6
        jne    loc_3_42D9
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, 0x989680
        mov    eax, dword ptr [eax + 0x1c]
        sub    edx, edx
        div    ecx
        mov    ecx, 0xc8
        sub    edx, edx
        div    ecx
        mov    eax, dword ptr [esi]
        mov    word ptr [eax + 0x18], dx
        jmp    loc_3_42D9
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_42D0:
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 0x28], 0
loc_3_42D9:
        mov    eax, dword ptr [esi]
        push   esi
        mov    word ptr [eax + 0x1a], 0
        call   sub_3_47C0
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_42F0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esi + 0x1c]
        mov    ax, word ptr [eax + 8]
        inc    ax
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax + 0x14]
        mov    ebx, dword ptr [esi + 0x18]
        lea    ebp, [edi + 4]
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [edi + 8], eax
        push   ebp
        call   sub_3_486B
        add    esp, 4
        mov    eax, dword ptr [ebp]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [edi + 0x18], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x18]
        mov    dword ptr [edi + 0x2c], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x1a]
        mov    dword ptr [edi + 0x30], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    eax, dword ptr [eax + 0x3c]
        mov    dword ptr [edi + 0x1c], eax
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [edi + 0x10], 0
        mov    dword ptr [edi + 0x20], 0
        mov    dword ptr [edi + 0x24], 0
        mov    dword ptr [edi + 0x28], 1
        mov    al, byte ptr [ebx + 0x27]
        push   eax
        call   sub_3_9500
        add    esp, 4
        and    eax, 0xff
        mov    dword ptr [edi], eax
        add    ebx, 0x28
        je     loc_3_4396
loc_3_438A:
        mov    eax, dword ptr [ebx + 0xc]
        add    dword ptr [edi + 0x24], eax
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_3_438A
loc_3_4396:
        push   esi
        call   sub_3_47C0
        add    esp, 4
        mov    eax, dword ptr [esi]
        pop    ebp
        mov    word ptr [eax + 0x1a], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
sub_3_43B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
sub_3_43D0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, 0x3c
        mov    edi, dword ptr [esp + 0x14]
        push   edi
        call   sub_3_9620
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_3_43F1
        mov    esi, 0x4c
loc_3_43F1:
        push   1
        push   esi
        call   sub_3_43B0
        add    esp, 8
        mov    esi, eax
        mov    dword ptr [edi + 0x18], esi
        or     esi, esi
        jne    loc_3_4411
        mov    dword ptr [edi + 0x24], 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_4411:
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [esi + 4], eax
        mov    ax, word ptr [edi + 0x66]
        mov    word ptr [esi + 0x18], ax
        mov    eax, dword ptr [edi + 0x84]
        mov    word ptr [esi + 0x1a], ax
        lea    eax, [esi + 0x1c]
        push   eax
        mov    eax, dword ptr [edi + 0x90]
        add    eax, 0x12
        push   eax
        call   sub_3_5300
        add    esp, 8
        mov    ecx, dword ptr [edi + 0x14]
        lea    ebp, [esi + 0x27]
        cmp    byte ptr [ecx + 0x1e], 0
        mov    eax, dword ptr [edi + 0x90]
        je     loc_3_4456
        mov    al, byte ptr [eax + 0x19]
        jmp    loc_3_4459
loc_3_4456:
        mov    al, byte ptr [eax + 0x18]
loc_3_4459:
        mov    byte ptr [ebp], al
        or     ebx, ebx
        je     loc_3_44B1
        mov    edx, dword ptr [ebx]
        mov    dword ptr [esi + 0x3c], edx
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0x40], edx
        lea    eax, [esi + 0x3c]
        mov    edx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 8], edx
        mov    dx, word ptr [ebx + 0xc]
        mov    word ptr [eax + 0xc], dx
        mov    al, byte ptr [ecx + 0x1f]
        and    al, 2
        cmp    al, 1
        cmc    
        sbb    eax, eax
        and    eax, 0x4000
        mov    cx, word ptr [ebx + 4]
        and    cx, 0xf8
        or     ax, cx
        or     ax, word ptr [esi + 0x12]
        or     ax, 0x2200
        mov    word ptr [esi + 0x12], ax
        test   al, 0x40
        je     loc_3_44B1
        cmp    byte ptr [esi + 0x44], 0x10
        jb     loc_3_44B1
        mov    byte ptr [esi + 0x44], 0
loc_3_44B1:
        mov    ecx, dword ptr [edi + 0x90]
        push   1
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [esi + 0x26], al
        and    eax, 0xff
        mov    ecx, dword ptr [ecx + 2]
        add    ecx, eax
        mov    dword ptr [esi + 0x30], ecx
        push   ebx
        push   edi
        call   sub_3_9650
        add    esp, 0xc
        mov    dword ptr [esi + 0x34], eax
        mov    ecx, dword ptr [edi + 0x90]
        mov    al, byte ptr [ecx + 0x1a]
        mov    byte ptr [esi + 0x3a], al
        mov    al, byte ptr [ecx + 0x1b]
        mov    byte ptr [esi + 0x3b], al
        mov    ax, word ptr [ecx + 0x1c]
        mov    word ptr [esi + 0x38], ax
        lea    eax, [esi + 0x28]
        push   eax
        push   edi
        call   sub_3_96B0
        add    esp, 8
        test   byte ptr [ebp], 0x80
        je     loc_3_450C
        or     word ptr [esi + 0x12], 0x400
loc_3_450C:
        mov    dword ptr [edi + 0x24], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_4520:
        push   esi
        mov    al, byte ptr [0x4550]         ; -> loc_3_4550
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0x14]
        mov    ecx, dword ptr [esi + 0x10]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [esi + 0x10], eax
        inc    word ptr [esi + 0x1c]
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4550:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4560:
        push   esi
        mov    al, byte ptr [0x45b0]         ; -> loc_3_45B0
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        mov    ecx, dword ptr [eax + 0x10]
        or     ecx, ecx
        je     loc_3_45AB
        sub    edx, edx
loc_3_4576:
        cmp    word ptr [ecx + 0x10], dx
        je     loc_3_4594
        mov    ax, word ptr [esi + 0x66]
        cmp    word ptr [ecx + 0x18], ax
        jne    loc_3_4594
        xor    eax, eax
        mov    ax, word ptr [ecx + 0x1a]
        cmp    eax, dword ptr [esi + 0x84]
        je     loc_3_45A0
loc_3_4594:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, edx
        jne    loc_3_4576
        mov    dword ptr [esi + 0x18], ecx
        pop    esi
        ret    
        db  090h
loc_3_45A0:
        inc    word ptr [ecx + 0x10]
        or     dword ptr [esi + 0x20], 0x10000000
loc_3_45AB:
        mov    dword ptr [esi + 0x18], ecx
        pop    esi
        ret    
loc_3_45B0:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_45C0:
        push   1
        push   0x10
        call   sub_3_43B0
        mov    edx, dword ptr [esp + 0xc]
        add    esp, 8
        mov    dword ptr [edx + 0x1c], eax
        or     eax, eax
        jne    loc_3_45E0
        mov    dword ptr [edx + 0x24], 8
        ret    
        db  090h
loc_3_45E0:
        mov    ecx, dword ptr [edx + 0x18]
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 8], 1
        mov    ecx, dword ptr [edx]
        mov    cl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 0xa], cl
        mov    ecx, dword ptr [edx]
        mov    ecx, dword ptr [ecx + 8]
        mov    word ptr [eax + 0xc], cx
        mov    ecx, dword ptr [edx]
        mov    cl, byte ptr [ecx + 5]
        and    ecx, 0xffff00ff
        mov    word ptr [eax + 0xe], cx
        mov    dword ptr [edx + 0x24], 0
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4620:
        mov    al, byte ptr [0x4650]         ; -> loc_3_4650
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 0x18]
        mov    eax, dword ptr [eax + 0x14]
        mov    ecx, dword ptr [edx + 0x1c]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 0x1c]
        mov    ecx, dword ptr [edx + 0x18]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [edx + 0x18]
        mov    cx, word ptr [eax + 0x10]
        inc    cx
        mov    eax, dword ptr [edx + 0x18]
        mov    word ptr [eax + 0x10], cx
        ret    
        db  08Bh, 0FFh
loc_3_4650:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4660:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    al, byte ptr [0x4720]         ; -> loc_3_4720
        mov    dword ptr [esp + 0x10], 0
        mov    edi, dword ptr [esp + 0x18]
        mov    esi, dword ptr [edi + 0x14]
        dec    word ptr [esi + 0x1c]
        jne    loc_3_4712
        mov    ecx, 0x9bd0                   ; -> obj3:0x9BD0
        cmp    dword ptr [0x9bd0], 0         ; -> obj3:0x9BD0
        je     loc_3_46A0
loc_3_4693:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        je     loc_3_46A0
        mov    ecx, eax
        cmp    dword ptr [ecx], 0
        jne    loc_3_4693
loc_3_46A0:
        cmp    dword ptr [ecx], 0
        je     loc_3_46B1
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [esp + 0x10], 1
loc_3_46B1:
        test   byte ptr [esi + 0x1f], 2
        jne    loc_3_46EC
        sub    ebx, ebx
        cmp    dword ptr [esi + 0x104], ebx
        jbe    loc_3_46EC
        sub    ebp, ebp
loc_3_46C3:
        mov    eax, dword ptr [esi + 0x108]
        mov    eax, dword ptr [ebp + eax + 8]
        or     eax, eax
        je     loc_3_46E0
        mov    ecx, dword ptr [edi + 0x20]
        push   ecx
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0xc
loc_3_46E0:
        add    ebp, 0xc
        inc    ebx
        cmp    dword ptr [esi + 0x104], ebx
        ja     loc_3_46C3
loc_3_46EC:
        mov    eax, dword ptr [edi + 0x20]
        push   eax
        push   0
        push   esi
        call   sub_3_8750
        add    esp, 0xc
        mov    dword ptr [edi + 0x14], 0
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_4712
        push   edi
        call   sub_3_1DA0
        add    esp, 4
loc_3_4712:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_4720:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4730:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    al, byte ptr [0x47b0]         ; -> loc_3_47B0
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi + 0x18]
        dec    word ptr [edi + 0x10]
        jne    loc_3_47A8
        mov    ecx, dword ptr [esi + 0x14]
        add    ecx, 0x10
        cmp    dword ptr [ecx], 0
        je     loc_3_475E
loc_3_4751:
        mov    eax, dword ptr [ecx]
        cmp    edi, eax
        je     loc_3_475E
        mov    ecx, eax
        cmp    dword ptr [ecx], 0
        jne    loc_3_4751
loc_3_475E:
        sub    ebx, ebx
        cmp    dword ptr [ecx], ebx
        je     loc_3_476D
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ebx, 1
loc_3_476D:
        mov    ebp, dword ptr [edi + 0x28]
        or     ebp, ebp
        je     loc_3_478C
loc_3_4774:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        mov    ecx, dword ptr [esi + 0x20]
        push   ecx
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0xc
        or     ebp, ebp
        jne    loc_3_4774
loc_3_478C:
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        push   0
        push   edi
        call   sub_3_8750
        add    esp, 0xc
        or     ebx, ebx
        je     loc_3_47A8
        push   esi
        call   sub_3_4660
        add    esp, 4
loc_3_47A8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_3_47B0:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_47C0:
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [0x4840]         ; -> loc_3_4840
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esi + 0x1c]
        dec    word ptr [ecx + 8]
        jne    loc_3_4834
        mov    eax, dword ptr [esi + 0x1c]
        sub    edi, edi
        mov    bl, byte ptr [eax + 0xb]
        and    ebx, 2
        jne    loc_3_4807
        mov    edx, dword ptr [esi + 0x18]
        add    edx, 0x14
        cmp    dword ptr [edx], edi
        je     loc_3_47F9
loc_3_47EC:
        mov    eax, dword ptr [edx]
        cmp    eax, ecx
        je     loc_3_47F9
        mov    edx, eax
        cmp    dword ptr [edx], 0
        jne    loc_3_47EC
loc_3_47F9:
        cmp    dword ptr [edx], 0
        je     loc_3_4807
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    edi, 1
loc_3_4807:
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        push   0
        push   ecx
        call   sub_3_8750
        add    esp, 0xc
        or     edi, edi
        je     loc_3_4827
        push   esi
        call   sub_3_4730
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_4827:
        or     ebx, ebx
        je     loc_3_4834
        push   esi
        call   sub_3_4660
        add    esp, 4
loc_3_4834:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_4840:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4850:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   esp
        call   sub_3_486B
        pop    eax
        VxDCall  IFSMgr.IFSMgr_Win32ToDosTime    ; cd203a004000
        lea    esp, [esp + 8]
        ret    
sub_3_486B:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 4]
        or     ecx, dword ptr [eax]
        je     loc_3_488C
        VxDCall  IFSMgr.IFSMgr_GetTimeZoneBias   ; cd2060004000
        mov    edx, 0xdc3cba00
        imul   edx
        mov    ecx, dword ptr [esp + 4]
        sub    dword ptr [ecx], eax
        sbb    dword ptr [ecx + 4], edx
loc_3_488C:
        ret    
sub_3_488D:
        mov    ecx, dword ptr [esp + 4]
        push   eax
        push   eax
        push   esp
        push   ecx
        call   sub_3_5300
        lea    esp, [esp + 8]
        push   esp
        call   sub_3_4850
        lea    esp, [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        ret    
sub_3_48AF:
        cmp    dword ptr [esp + 0xc], 4
        ja     loc_3_48F8
        mov    eax, dword ptr [esp + 8]
        mov    edx, 0x2e002e
        mov    eax, dword ptr [eax]
        jb     loc_3_48C9
        cmp    eax, edx
        jne    loc_3_48F8
        jmp    loc_3_48DA
loc_3_48C9:
        cmp    dword ptr [esp + 0xc], 2
        jne    loc_3_48F8
        cmp    ax, dx
        jne    loc_3_48F8
        mov    edx, 0x20002e
loc_3_48DA:
        mov    eax, 0x200020
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [ecx + 0x10], eax
        mov    word ptr [ecx + 0x14], ax
        xor    eax, eax
        ret    
loc_3_48F8:
        VxDCall  IFSMgr.CreateBasis              ; cd2045804000
sub_3_48FE:
        VxDCall  IFSMgr.AppendBasisTail          ; cd2047804000
sub_3_4904:
        VxDCall  IFSMgr.FcbToShort               ; cd2048804000
sub_3_490A:
        VxDCall  IFSMgr.IFSMgr_MetaMatch         ; cd203d804000
sub_3_4910:
        VxDCall  IFSMgr.BCSToUni                 ; cd2042804000
sub_3_4916:
        VxDCall  IFSMgr.UniToBCS                 ; cd2040804000
sub_3_491C:
        cmp    dword ptr [esp + 0xc], 4
        ja     loc_3_4968
        mov    eax, dword ptr [esp + 8]
        mov    edx, 0x2e002e
        mov    eax, dword ptr [eax]
        jb     loc_3_4936
        cmp    eax, edx
        jne    loc_3_4968
        jmp    loc_3_4947
loc_3_4936:
        cmp    dword ptr [esp + 0xc], 2
        jne    loc_3_4968
        cmp    ax, dx
        jne    loc_3_4968
        mov    edx, 0x20002e
loc_3_4947:
        mov    eax, 0x200020
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [ecx + 0x10], eax
        mov    word ptr [ecx + 0x14], ax
        mov    eax, 0x16
        ret    
loc_3_4968:
        VxDCall  IFSMgr.ShortToFcb               ; cd2049804000
sub_3_496E:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 0x70]
        mov    al, byte ptr [eax + 0x6e]
        not    cl
        mov    ah, al
        not    ah
        and    ax, cx
        and    ax, 0x3f1e
        sete   al
        movzx  eax, al
        ret    
        db  000h, 000h, 000h
loc_3_4990:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esi], 0x800
        mov    word ptr [esi + 0x18], 0x10
        mov    eax, dword ptr [esp + 8]
        xor    edx, edx
        mov    ecx, dword ptr [eax + 0x44]
        mov    eax, dword ptr [eax + 0x48]
        shld   edx, eax, cl
        shl    eax, cl
        shrd   eax, edx, 0xf
        shr    edx, 0xf
        jne    loc_3_49CC
        test   eax, 0xffff0000
        je     loc_3_49D0
loc_3_49CC:
        mov    ax, 0xffff
loc_3_49D0:
        mov    word ptr [esp + 6], ax
        xor    eax, eax
        mov    ax, word ptr [esp + 6]
        mov    dword ptr [esi + 0x24], eax
        mov    dword ptr [esi + 0x28], 0
        mov    dword ptr [esi + 0x14], 0
        mov    word ptr [esi + 0x1a], 0
        sub    eax, eax
        pop    esi
        add    esp, 8
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_4A00:
        push   0x2f
        mov    eax, dword ptr [esp + 8]
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4A20:
        push   0x4e
        mov    eax, dword ptr [esp + 8]
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4A40:
        push   0x36
        mov    eax, dword ptr [esp + 8]
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0xc
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4A60:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    ecx, dword ptr [eax]
        mov    byte ptr [ecx + 4], 0
        call   sub_3_4A80
        add    esp, 4
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4A80:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        mov    edi, dword ptr [ebx]
        mov    eax, dword ptr [ebx]
        test   byte ptr [eax + 3], 0x34
        je     loc_3_4A99
        or     dword ptr [ebx + 0x20], 0x8000
loc_3_4A99:
        cmp    byte ptr [edi + 4], 0
        jne    loc_3_4B30
        push   ebx
        call   sub_3_54A0
        add    esp, 4
        mov    ecx, dword ptr [edi]
        mov    eax, ecx
        and    ax, 0xff9e
        lea    esi, [ebx + 0x24]
        mov    word ptr [ebx + 0x70], ax
        cmp    dword ptr [esi], 0
        jne    loc_3_4B26
        mov    edx, dword ptr [ebx + 0x10]
        cmp    word ptr [edx + 0x48], 2
        ja     loc_3_4B26
        test   al, 8
        je     loc_3_4B20
        test   byte ptr [ebx + 0x21], 0x80
        jne    loc_3_4B20
        and    ecx, 0xffff0000
        mov    dword ptr [edi], ecx
        or     ecx, 0x80000808
        mov    dword ptr [edi], ecx
        mov    eax, dword ptr [ebx]
        xor    edx, edx
        mov    dx, word ptr [0x9ea0]         ; -> loc_3_9EA0
        add    edx, 2
        mov    esi, 0x9ea0                   ; -> loc_3_9EA0
        mov    edi, dword ptr [eax + 0xc]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   ebx
        call   sub_3_5590
        add    esp, 4
        mov    ax, word ptr [ebx + 0x2c]
        mov    word ptr [ebx + 0x2e], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_3_4B20:
        mov    dword ptr [esi], 0x12
loc_3_4B26:
        mov    byte ptr [ebx + 0x6d], 0
        jmp    loc_3_4BA8
        db  090h
loc_3_4B30:
        or     dword ptr [ebx + 0x20], 0x2000000
        push   ebx
        call   sub_3_54A0
        add    esp, 4
        lea    esi, [ebx + 0x24]
        cmp    dword ptr [esi], 0
        jne    loc_3_4BA2
        mov    eax, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [eax + 0x14]
        push   1
        lea    eax, [edi + 1]
        push   0xb
        push   eax
        lea    eax, [ebp + 0x30]
        push   eax
        call   sub_3_4910
        add    esp, 0x10
        or     word ptr [ebp + 0xe], 2
        mov    dl, byte ptr [edi + 0xc]
        and    edx, 0xffff00ff
        mov    word ptr [ebx + 0x70], dx
        mov    ax, word ptr [edi + 0xd]
        mov    word ptr [ebx + 0x66], ax
        xor    ecx, ecx
        mov    cx, word ptr [edi + 0xf]
        inc    ecx
        mov    dword ptr [ebx + 0x80], ecx
        mov    byte ptr [ebx + 0x6d], 0x12
        and    dx, 0xff9e
        mov    word ptr [ebx + 0x70], dx
        cmp    dword ptr [esi], 0
        jne    loc_3_4BB8
        or     ax, ax
        jne    loc_3_4BA8
loc_3_4BA2:
        mov    dword ptr [esi], 0x12
loc_3_4BA8:
        cmp    dword ptr [esi], 0
        jne    loc_3_4BB8
        push   0x25
        push   ebx
        call   sub_3_8380
        add    esp, 8
loc_3_4BB8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_3_4BC0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 0x24], 0
        je     loc_3_4BE7
        cmp    dword ptr [esi + 0x24], 2
        jne    loc_3_4BDA
        mov    dword ptr [esi + 0x24], 0x12
loc_3_4BDA:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 0x24]
        mov    word ptr [eax + 0x1a], cx
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_4BE7:
        test   byte ptr [esi + 0x70], 8
        jne    loc_3_4BFE
        push   esi
        call   sub_3_9520
        add    esp, 4
        or     eax, eax
        jne    loc_3_4CED
loc_3_4BFE:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [esi + 0x10]
        mov    edi, dword ptr [eax + 0x14]
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 4], 0
        jne    loc_3_4C37
        push   1
        push   0xb
        push   0x16
        lea    eax, [ebx + 0x30]
        push   eax
        lea    eax, [edi + 1]
        push   eax
        call   sub_3_4916
        add    esp, 0x14
        mov    ax, word ptr [esi + 0x70]
        test   al, 8
        je     loc_3_4C30
        and    al, 0xf7
        jmp    loc_3_4C34
loc_3_4C30:
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax]
loc_3_4C34:
        mov    byte ptr [edi + 0xc], al
loc_3_4C37:
        test   byte ptr [esi + 0x70], 8
        je     loc_3_4C66
        mov    dword ptr [esi + 0x84], 0
        mov    eax, dword ptr [esi + 0x14]
        add    eax, 0x50
        push   eax
        call   sub_3_4850
        add    esp, 4
        mov    dword ptr [edi + 0x16], eax
        mov    byte ptr [edi + 0x15], 9
        mov    dword ptr [edi + 0x1a], 0
        jmp    loc_3_4C91
loc_3_4C66:
        lea    eax, [edi + 0x16]
        push   eax
        mov    eax, dword ptr [esi + 0x90]
        add    eax, 0x12
        push   eax
        call   sub_3_488D
        add    esp, 8
        push   esi
        call   sub_3_94D0
        add    esp, 4
        mov    byte ptr [edi + 0x15], al
        mov    eax, dword ptr [esi + 0xc0]
        mov    dword ptr [edi + 0x1a], eax
loc_3_4C91:
        mov    ax, word ptr [esi + 0x66]
        mov    word ptr [edi + 0xd], ax
        mov    eax, dword ptr [esi + 0x84]
        mov    word ptr [edi + 0xf], ax
        xor    ecx, ecx
        mov    eax, dword ptr [ebx + 8]
        push   1
        push   0xc
        mov    cx, word ptr [eax]
        sub    ecx, 2
        push   ecx
        add    eax, 2
        push   eax
        lea    eax, [edi + 0x1e]
        push   eax
        call   sub_3_4916
        add    esp, 0x14
        cmp    eax, 8
        jne    loc_3_4CE0
        test   byte ptr [esi + 0x70], 8
        je     loc_3_4CE0
        mov    ecx, dword ptr [esi + 0x14]
        cmp    dword ptr [ecx + 0x4c], 0x4b0
        je     loc_3_4CE0
        mov    byte ptr [eax + edi + 0x1e], 0x2e
        inc    eax
loc_3_4CE0:
        mov    byte ptr [eax + edi + 0x1e], 0
        mov    eax, dword ptr [esi]
        mov    word ptr [eax + 0x1a], 0
loc_3_4CED:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4D00:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [ebx + 0x1c]
        test   byte ptr [ebx + 0x22], 0x40
        jne    loc_3_4D50
        or     dword ptr [ebx + 0x20], 0x400100
        inc    word ptr [ebp + 8]
        lea    eax, [ebp + 0x14]
        mov    dword ptr [ebx + 0xb0], eax
        push   ebx
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     loc_3_4D50
        mov    ax, word ptr [ebx + 0x2c]
        mov    word ptr [ebx + 0x2e], ax
        push   0x46
        push   ebx
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_4D50:
        or     dword ptr [ebx + 0x20], 0x3008000
        push   ebx
        call   sub_3_54A0
        add    esp, 4
        cmp    dword ptr [ebx + 0x24], 0
        jne    loc_3_4E7F
        mov    ax, word ptr [ebp + 0x1c]
        mov    word ptr [ebx + 0x66], ax
        or     ax, ax
        jne    loc_3_4D83
        mov    dword ptr [ebx + 0x24], 0x12
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_4D83:
        mov    eax, dword ptr [ebp + 0x24]
        mov    dword ptr [ebx + 0x8c], eax
        mov    cx, word ptr [ebp + 0x1e]
        mov    word ptr [ebx + 0x76], cx
        mov    edx, dword ptr [ebp + 0x28]
        mov    dword ptr [ebx + 0x84], edx
        lea    eax, [edx + 1]
        mov    dword ptr [ebx + 0x80], eax
        mov    eax, dword ptr [ebp + 0x2c]
        mov    dword ptr [ebx + 0x88], eax
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebx + 0x78], eax
        mov    ax, word ptr [ebp + 0x36]
        mov    word ptr [ebx + 0x70], ax
        mov    eax, dword ptr [ebp + 0x30]
        mov    dword ptr [ebx + 0x7c], eax
        mov    eax, dword ptr [ebx]
        test   byte ptr [eax + 0x18], 0x40
        je     loc_3_4E02
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 0x24]
        inc    eax
        mov    dword ptr [ebx + 0x80], eax
        cmp    eax, edx
        jae    loc_3_4E02
        mov    dword ptr [ebx + 0x84], 0
        mov    edx, ecx
        mov    eax, dword ptr [ebx + 0x14]
        and    edx, 0xffff
        mov    cl, byte ptr [eax + 0x44]
        shl    edx, cl
        mov    dword ptr [ebx + 0x88], edx
        mov    dword ptr [ebx + 0x7c], 0
loc_3_4E02:
        mov    edx, dword ptr [ebx + 0x10]
        mov    al, byte ptr [ebp + 0x34]
        and    ax, 6
        or     word ptr [edx + 0xe], ax
        mov    cx, word ptr [ebp + 0x38]
        mov    word ptr [edx + 0x4a], cx
        mov    eax, ecx
        and    eax, 0xffff
        lea    edi, [edx + eax + 0x4c]
        mov    dword ptr [edx], edi
        mov    word ptr [edx + 0x4c], cx
        mov    ax, word ptr [ebp + 0x3a]
        lea    esi, [ebp + 0x3a]
        add    cx, ax
        mov    word ptr [edx + 0x48], cx
        and    eax, 0xffff
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ax, word ptr [edx + 0x48]
        and    eax, 0xfffe
        mov    word ptr [edx + eax + 0x4c], 0
        test   byte ptr [ebp + 0x35], 8
        jne    loc_3_4E70
        and    dword ptr [ebx + 0x20], 0xffff7fff
        push   ebx
        call   sub_3_8D90
        add    esp, 4
loc_3_4E70:
        mov    byte ptr [ebx + 0x6d], 0x22
        push   0x27
        push   ebx
        call   sub_3_8380
        add    esp, 8
loc_3_4E7F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4E90:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        cmp    dword ptr [ebx + 0x24], 0
        je     loc_3_4EBA
        mov    ebp, dword ptr [ebx + 0x1c]
        cmp    dword ptr [ebx + 0x24], 2
        jne    loc_3_511C
        mov    dword ptr [ebx + 0x24], 0x12
        jmp    loc_3_511C
loc_3_4EBA:
        test   byte ptr [ebx + 0x70], 8
        jne    loc_3_4ED1
        push   ebx
        call   sub_3_9520
        add    esp, 4
        or     eax, eax
        jne    loc_3_5146
loc_3_4ED1:
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [esp + 0x14], eax
        test   byte ptr [ebx + 0x23], 1
        jne    loc_3_4F93
        mov    eax, dword ptr [eax]
        push   1
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        add    eax, 0x3c
        push   eax
        call   sub_3_43B0
        add    esp, 8
        mov    ebp, eax
        mov    dword ptr [ebx + 0x1c], ebp
        or     ebp, ebp
        jne    loc_3_4F10
        mov    dword ptr [ebx + 0x24], 8
        jmp    loc_3_511C
        db  090h
loc_3_4F10:
        lea    eax, [ebp + 0xc]
        mov    dword ptr [ebp + 4], eax
        mov    word ptr [ebp + 8], 1
        mov    byte ptr [ebp + 0xb], 2
        mov    edx, dword ptr [ebx + 0x14]
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebp + 0x10], edx
        mov    ax, word ptr [eax + 0xe]
        and    al, 6
        mov    byte ptr [ebp + 0x34], al
        test   byte ptr [ebx + 0x21], 0x80
        je     loc_3_4F3D
        or     byte ptr [ebp + 0x35], 8
loc_3_4F3D:
        mov    ax, word ptr [ebx + 0x70]
        and    ax, 0xfff7
        mov    word ptr [ebp + 0x36], ax
        mov    eax, dword ptr [esp + 0x14]
        lea    edi, [ebp + 0x3a]
        mov    ax, word ptr [eax + 0x4a]
        mov    word ptr [ebp + 0x38], ax
        mov    eax, dword ptr [esp + 0x14]
        mov    esi, dword ptr [eax]
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        inc    word ptr [edx + 0x1c]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 0x20], ebp
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [eax], 0x4a40       ; -> loc_3_4A40
        mov    dword ptr [eax + 4], 0x3c40   ; -> sub_3_3C40
        mov    dword ptr [eax + 8], 0x920    ; -> obj1:0x0920
loc_3_4F93:
        mov    eax, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 0x1c]
        mov    ecx, dword ptr [eax + 0x14]
        mov    dword ptr [esp + 0x10], ecx
        test   byte ptr [ebx + 0x70], 8
        je     loc_3_4FE9
        mov    dword ptr [ebx + 0x84], 0
        mov    edx, dword ptr [ebx + 0x14]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0x76]
        mov    cl, byte ptr [edx + 0x44]
        shl    eax, cl
        mov    dword ptr [ebx + 0x88], eax
        mov    dword ptr [ebx + 0x7c], 0
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edx + 0x54]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 0x50]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], 9
        mov    dword ptr [ecx + 0x20], 0
        jmp    loc_3_5020
loc_3_4FE9:
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebx + 0x90]
        add    eax, 0x12
        push   eax
        call   sub_3_5300
        add    esp, 8
        push   ebx
        call   sub_3_94D0
        and    eax, 0xff
        mov    ecx, dword ptr [esp + 0x14]
        add    esp, 4
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 0xc0]
        mov    dword ptr [ecx + 0x20], eax
loc_3_5020:
        mov    esi, dword ptr [esp + 0x10]
        add    esi, 4
        push   esi
        call   sub_3_486B
        mov    ecx, dword ptr [esp + 0x14]
        add    esp, 4
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 0x14], eax
        mov    dword ptr [ecx + 0x10], 0
        mov    dword ptr [ecx + 0xc], 0
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 0x1c], 0
        mov    eax, dword ptr [eax + 4]
        mov    cx, word ptr [eax]
        and    ecx, 0xfffe
        mov    word ptr [eax + ecx], 0
        mov    eax, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 4]
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    esi, 2
        add    edi, 0x2c
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x14]
        test   byte ptr [eax + 0xe], 0x48
        je     loc_3_50D3
        mov    eax, dword ptr [eax + 8]
        mov    cx, word ptr [eax]
        and    ecx, 0xfffe
        mov    word ptr [eax + ecx], 0
        mov    eax, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 8]
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    esi, 2
        add    edi, 0x234
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_3_50E0
loc_3_50D3:
        mov    ecx, dword ptr [esp + 0x10]
        mov    word ptr [ecx + 0x234], 0
loc_3_50E0:
        mov    ax, word ptr [ebx + 0x66]
        mov    word ptr [ebp + 0x1c], ax
        mov    ax, word ptr [ebx + 0x76]
        mov    word ptr [ebp + 0x1e], ax
        mov    eax, dword ptr [ebx + 0x78]
        mov    dword ptr [ebp + 0x20], eax
        mov    eax, dword ptr [ebx + 0x8c]
        mov    dword ptr [ebp + 0x24], eax
        mov    ecx, dword ptr [ebx + 0x84]
        mov    dword ptr [ebp + 0x28], ecx
        mov    eax, dword ptr [ebx + 0x88]
        mov    dword ptr [ebp + 0x2c], eax
        mov    eax, dword ptr [ebx + 0x7c]
        mov    dword ptr [ebp + 0x30], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 0x24], ecx
loc_3_511C:
        test   byte ptr [ebx + 0x23], 1
        je     loc_3_513D
        add    ebp, 0x14
        mov    dword ptr [ebx + 0xb0], ebp
        push   ebx
        call   sub_3_59D8
        add    esp, 4
        push   ebx
        call   sub_3_47C0
        add    esp, 4
loc_3_513D:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 0x24]
        mov    word ptr [eax + 0x1a], cx
loc_3_5146:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
sub_3_5150:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_5180:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_51B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    eax, eax
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        inc    eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0xc
        push   eax
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_3_51F0
        test   byte ptr [ebp - 4], 1
        je     loc_3_51F0
        mov    eax, dword ptr [ebp - 4]
        xor    eax, dword ptr [ebp + 8]
        and    eax, 0xfff
        xor    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_51F0:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_5200:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        lea    ecx, [ecx + ecx*4]
        lea    eax, [eax + ecx*2 - 0x210]
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_3_5220:
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, 0x30
        mov    ecx, 0x11
        repe scasb al, byte ptr es:[edi]
        sete   byte ptr [esp + 0xb]
        cmp    byte ptr [esp + 0xb], 0
        je     loc_3_5258
        mov    eax, dword ptr [esp + 0x14]
        pop    edi
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        pop    esi
        add    esp, 4
        ret    
loc_3_5258:
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0xe
        push   eax
        call   sub_3_5200
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        add    eax, 0xc
        push   eax
        call   sub_3_5200
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0xa
        push   eax
        call   sub_3_5200
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        add    eax, 8
        push   eax
        call   sub_3_5200
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        add    eax, 6
        push   eax
        call   sub_3_5200
        add    esp, 4
        dec    eax
        push   eax
        mov    eax, dword ptr [esp + 0x24]
        add    eax, 4
        push   eax
        call   sub_3_5200
        add    esp, 4
        dec    eax
        push   eax
        mov    eax, dword ptr [esp + 0x28]
        add    eax, 2
        push   eax
        call   sub_3_5200
        add    esp, 4
        mov    esi, eax
        mov    eax, dword ptr [esp + 0x28]
        push   eax
        call   sub_3_5200
        shl    eax, 2
        add    esp, 4
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    esi, eax
        mov    eax, dword ptr [esp + 0x2c]
        push   esi
        push   eax
        call   sub_3_53A0
        add    esp, 0x20
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_3_5300:
        sub    esp, 4
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, 0x30
        mov    ecx, 7
        repe scasb al, byte ptr es:[edi]
        sete   byte ptr [esp + 7]
        cmp    byte ptr [esp + 7], 0
        je     loc_3_5336
        mov    eax, dword ptr [esp + 0x10]
        pop    edi
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        add    esp, 4
        ret    
loc_3_5336:
        mov    eax, dword ptr [esp + 0xc]
        push   0
        mov    al, byte ptr [eax + 5]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        mov    al, byte ptr [eax + 3]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        dec    eax
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        dec    eax
        push   eax
        mov    eax, dword ptr [esp + 0x24]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    eax, 0x76c
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        push   eax
        call   sub_3_53A0
        add    esp, 0x20
        pop    edi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
sub_3_53A0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x1c]
        cmp    ecx, 0x641
        jb     loc_3_5483
        mov    esi, dword ptr [esp + 0x20]
        cmp    esi, 0xc
        jae    loc_3_5483
        sub    ecx, 0x641
        xor    edi, edi
        mov    di, word ptr [esi*2 + 0x9ec8] ; -> loc_3_9EC8
        mov    ebx, 0x64
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        sub    edi, eax
        lea    eax, [ecx + ecx*8]
        lea    eax, [ecx + eax*8]
        mov    ebx, 0x190
        lea    eax, [eax + eax*4]
        add    edi, eax
        mov    eax, ecx
        shr    eax, 2
        add    edi, eax
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        add    edi, eax
        add    edi, dword ptr [esp + 0x24]
        mov    dword ptr [esp + 0xc], edi
        inc    ecx
        test   cl, 3
        jne    loc_3_5416
        cmp    esi, 2
        jb     loc_3_5416
        inc    dword ptr [esp + 0xc]
loc_3_5416:
        mov    eax, dword ptr [esp + 0x28]
        lea    ecx, [eax*4]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, dword ptr [esp + 0x2c]
        shl    ecx, 2
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, dword ptr [esp + 0x30]
        shl    ecx, 2
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        add    ecx, dword ptr [esp + 0x34]
        mov    dword ptr [esp + 0x10], ecx
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, 0x83d600
        mul    edx
        add    eax, dword ptr [esp + 0x10]
        adc    edx, 0
        mov    ebx, eax
        mov    eax, edx
        mov    edx, 0x186a0
        mul    edx
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, eax
        mov    eax, ebx
        mov    edx, 0x186a0
        mul    edx
        add    edx, ecx
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_5483:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esp + 0x18]
        pop    edi
        mov    dword ptr [eax], 0
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  090h
sub_3_54A0:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x24], 0
        xor    eax, eax
        xchg   dword ptr [0x9c14], eax       ; -> loc_3_9C14
        mov    dword ptr [esp + 8], eax
        cmp    dword ptr [esp + 8], 0
        jne    loc_3_54D6
        push   0
        push   0x596
        call   sub_3_43B0
        mov    dword ptr [esp + 0x10], eax
        add    esp, 8
loc_3_54D6:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 0x10], eax
        or     eax, eax
        jne    loc_3_54F0
        mov    dword ptr [esi + 0x24], 8
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_3_54F0:
        mov    eax, dword ptr [esp + 8]
        lea    ecx, [eax + 0x4c]
        mov    dword ptr [eax], ecx
        lea    ecx, [eax + 0x254]
        mov    dword ptr [eax + 4], ecx
        lea    ecx, [eax + 0x4dc]
        mov    dword ptr [eax + 8], ecx
        xor    ecx, ecx
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [eax + 0x28], ecx
        mov    dword ptr [eax + 0x2c], ecx
        mov    dword ptr [eax + 0x48], ecx
        test   byte ptr [esi + 0x23], 2
        jne    loc_3_5568
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0xc]
        mov    ax, word ptr [esi]
        sub    ax, 4
        mov    word ptr [ecx + 0x48], ax
        mov    ax, word ptr [esi + 2]
        mov    ecx, dword ptr [esp + 8]
        sub    ax, 4
        mov    word ptr [ecx + 0x4a], ax
        mov    eax, dword ptr [esp + 8]
        add    esi, 4
        mov    ax, word ptr [eax + 0x48]
        and    eax, 0xffff
        mov    edi, dword ptr [esp + 8]
        add    edi, 0x4c
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_5568:
        mov    eax, dword ptr [esp + 8]
        mov    ax, word ptr [eax + 0x48]
        and    eax, 0xfffe
        mov    ecx, dword ptr [esp + 8]
        mov    word ptr [eax + ecx + 0x4c], 0
        pop    edi
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_5590:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_3_55D2
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esp + 4]
        xchg   dword ptr [0x9c14], eax       ; -> loc_3_9C14
        mov    dword ptr [esp + 4], eax
        cmp    dword ptr [esp + 4], 0
        je     loc_3_55CB
        mov    eax, dword ptr [esi + 0x20]
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0xc
loc_3_55CB:
        mov    dword ptr [esi + 0x10], 0
loc_3_55D2:
        pop    esi
        add    esp, 4
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_55E0:
        sub    esp, 0xc4
        mov    eax, dword ptr [esp + 0xcc]
        push   eax
        mov    eax, dword ptr [esp + 0xcc]
        push   eax
        lea    eax, [esp + 8]
        push   eax
        call   sub_3_5680
        mov    eax, dword ptr [esp + 0xd4]
        add    esp, 0xc
        cmp    word ptr [eax + 0x1a], 0
        je     loc_3_5623
        mov    eax, dword ptr [esp + 0xc8]
        movsx  eax, word ptr [eax + 0x1a]
        add    esp, 0xc4
        ret    
loc_3_5623:
        mov    ax, word ptr [esp + 0xd0]
        mov    word ptr [esp + 0x2e], ax
        lea    eax, [esp]
        push   eax
        call   sub_3_83D0
        lea    eax, [esp + 4]
        add    esp, 4
        push   eax
        call   sub_3_75B0
        lea    eax, [esp + 4]
        add    esp, 4
        push   eax
        call   sub_3_5590
        lea    eax, [esp + 4]
        add    esp, 4
        push   eax
        call   sub_3_6820
        add    esp, 4
        call   sub_1_0320
        mov    eax, dword ptr [esp + 0xc8]
        movsx  eax, word ptr [eax + 0x1a]
        add    esp, 0xc4
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_3_5680:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x31
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esi + 0x28], 0x53464443
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [esi], edi
        mov    eax, dword ptr [0x9c10]       ; -> loc_3_9C10
        mov    dword ptr [esi + 0x68], eax
        test   byte ptr [0x884], 1           ; -> obj1:0x0884
        je     loc_3_56B8
        mov    word ptr [edi + 0x1a], 5
        pop    edi
        pop    esi
        ret    
loc_3_56B8:
        call   sub_1_0140
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx + 0x1a], ax
        mov    eax, dword ptr [esi]
        cmp    word ptr [eax + 0x1a], 0
        jne    loc_3_5766
        mov    byte ptr [esi + 0x63], 5
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, 1
        je     loc_3_56F0
        cmp    eax, 2
        je     loc_3_5703
        cmp    eax, 3
        je     loc_3_5722
        cmp    eax, 5
        je     loc_3_5703
        pop    edi
        pop    esi
        ret    
        db  090h
loc_3_56F0:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0x28]
        mov    dword ptr [esi + 0x40], eax
        or     byte ptr [esi + 0x6f], 5
        pop    edi
        pop    esi
        ret    
loc_3_5703:
        mov    ecx, dword ptr [edi + 0x1c]
        mov    dword ptr [esi + 0x14], ecx
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esi + 0x44], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esi + 0x40], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
        or     byte ptr [esi + 0x6f], 2
        pop    edi
        pop    esi
        ret    
loc_3_5722:
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [esi + 0x1c], eax
        mov    eax, dword ptr [esi + 0x1c]
        test   byte ptr [eax + 0xb], 1
        je     loc_3_5740
        mov    eax, dword ptr [esi]
        pop    edi
        mov    word ptr [eax + 0x1a], 6
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_5740:
        mov    eax, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, eax
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [esi + 0x44], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esi + 0x40], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
loc_3_5766:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5770:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   0
        push   esi
        call   sub_3_71C0
        add    esp, 8
        mov    dword ptr [esi + 0x50], eax
        mov    eax, dword ptr [esi + 0x50]
        cmp    eax, 2
        je     loc_3_57A0
        cmp    eax, 3
        je     loc_3_57B0
        cmp    eax, 4
        je     loc_3_57C5
        mov    dword ptr [esi + 0x24], 8
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_3_57A0:
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        pop    esi
        inc    dword ptr [eax + 0xbc]
        ret    
        db  08Dh, 049h, 000h
loc_3_57B0:
        mov    ax, word ptr [esi + 0x2c]
        mov    word ptr [esi + 0x2e], ax
        push   0x46
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
loc_3_57C5:
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        pop    esi
        inc    dword ptr [eax + 0xb8]
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_57E0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 0x44]
        mov    edi, dword ptr [edi + 0x1c]
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        pop    edi
        pop    esi
        ret    
sub_3_57F5:
        mov    ecx, dword ptr [esp + 4]
        VxDCall  BLOCKDEV.Service_800Fh          ; cd200f801000
sub_3_57FF:
        mov    eax, dword ptr [esp + 4]
        VMMCall  VMM.Map_Flat                    ; cd201c800100
sub_3_5809:
        VMMCall  VMM._PageFree                   ; cd2055800100
        movzx  edx, word ptr [esp + 4]
        movzx  ecx, word ptr [esp + 6]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048800100
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a800100
sub_3_5825:
        push   ebx
        push   edi
        push   esi
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [edi + 0x1c]
        mov    ecx, dword ptr [esi + 4]
        mov    ebx, dword ptr [edi + 0x28]
        mov    eax, dword ptr [ebx + 0x1a]
        mov    ax, word ptr [ebx + 0x18]
        and    ax, 0xbfff
        cmp    eax, 0x440d0866
        je     loc_3_58EE
        push   ecx
        call   sub_3_36E0
        lea    esp, [esp + 4]
        mov    edx, eax
        mov    ecx, dword ptr [esi + 4]
        mov    word ptr [edx + 4], 0xd
        mov    dword ptr [edx + 8], 0x510
        mov    dword ptr [edx + 0x3c], 1
        mov    dword ptr [edx + 0x38], ebx
        movzx  eax, word ptr [ebx + 0x10]
        mov    word ptr [edx + 0x2c], ax
        dec    al
        mov    byte ptr [edx + 0x4d], al
        movzx  eax, word ptr [ebx + 0x18]
        mov    dword ptr [edx + 0x30], eax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [edx + 0x34], eax
        movzx  eax, word ptr [ebx + 0x1c]
        mov    word ptr [edx + 0x2e], ax
        xor    ecx, ecx
        mov    dword ptr [edx + 0x44], ecx
        push   edx
        push   edx
        call   sub_3_57E0
        pop    esi
        pop    esi
        mov    eax, dword ptr [esi + 0x3c]
        mov    word ptr [ebx + 0x1c], ax
        cmp    word ptr [esi + 6], 0x10
        jae    loc_3_58B8
        xor    eax, eax
loc_3_58B8:
        mov    word ptr [edi + 0x1a], ax
        push   esi
        call   sub_3_3780
        lea    esp, [esp + 4]
        movzx  eax, word ptr [edi + 0x1a]
loc_3_58CA:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_3_58CE:
        inc    ebx
        inc    esp
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [eax], ah
        db  020h
loc_3_58D6:
        inc    ebx
        inc    esp
        xor    byte ptr [eax], dh
        xor    dword ptr [eax], esp
        and    byte ptr [eax], ah
loc_3_58DE:
        inc    ebx
        inc    esp
        inc    ecx
        push   ebp
        inc    esp
        dec    ecx
        dec    edi
        db  020h
loc_3_58E6:
        inc    esi
        dec    edi
        push   edx
        inc    ebp
        dec    ecx
        inc    edi
        dec    esi
        db  020h
loc_3_58EE:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    word ptr [eax], 0
        mov    ecx, dword ptr [esi + 0x3c]
        mov    dword ptr [eax + 2], ecx
        mov    ecx, dword ptr [esi + 0x58]
        mov    dword ptr [eax + 6], ecx
        mov    ecx, dword ptr [esi + 0x5c]
        mov    dword ptr [eax + 0xa], ecx
        mov    ecx, dword ptr [esi + 0x60]
        mov    dword ptr [eax + 0xe], ecx
        test   byte ptr [esi + 0x1f], 2
        je     loc_3_592D
        mov    edx, 0x58e6                   ; -> loc_3_58E6
        test   byte ptr [esi + 0x1f], 0x20
        jne    loc_3_593D
        mov    edx, 0x58de                   ; -> loc_3_58DE
        jmp    loc_3_593D
loc_3_592D:
        mov    edx, 0x58ce                   ; -> loc_3_58CE
        cmp    byte ptr [esi + 0x1e], 0
        je     loc_3_593D
        mov    edx, 0x58d6                   ; -> loc_3_58D6
loc_3_593D:
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax + 0x11], ecx
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [eax + 0x15], ecx
        and    word ptr [ebx + 0x2c], 0xfffe
        xor    eax, eax
        mov    word ptr [edi + 0x1a], ax
        jmp    loc_3_58CA
sub_3_5958:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, ecx
        shr    eax, 0x10
        shl    eax, 4
        movzx  ecx, cx
        add    eax, ecx
        mov    ecx, dword ptr [esp + 4]
        add    eax, dword ptr [ecx + 4]
        ret    
sub_3_5971:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0xb0]
        mov    ecx, dword ptr [eax + 0x20]
        and    dword ptr [eax + 0x20], 0xfffff9ff
        test   ecx, 0x100
        jne    loc_3_59B3
        cmp    dword ptr [edx], 0
        jne    loc_3_59AC
        test   ecx, 0x400
        jne    loc_3_59A0
        cmp    byte ptr [edx + 4], 0
        jne    loc_3_59AC
loc_3_59A0:
        inc    byte ptr [edx + 6]
        jmp    loc_3_59D5
loc_3_59A5:
        or     dword ptr [eax + 0x20], 0x200
loc_3_59AC:
        mov    eax, 1
        jmp    loc_3_59D7
loc_3_59B3:
        cmp    dword ptr [edx], eax
        je     loc_3_59D2
        cmp    dword ptr [edx], 0
        jne    loc_3_59A5
        cmp    byte ptr [edx + 6], 0
        jne    loc_3_59A5
        test   ecx, 0x200
        jne    loc_3_59D0
        cmp    byte ptr [edx + 4], 0
        jne    loc_3_59A5
loc_3_59D0:
        mov    dword ptr [edx], eax
loc_3_59D2:
        inc    byte ptr [edx + 7]
loc_3_59D5:
        xor    eax, eax
loc_3_59D7:
        ret    
sub_3_59D8:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0xb0]
        cmp    dword ptr [edx], 0
        je     loc_3_59FD
        dec    byte ptr [edx + 7]
        jne    loc_3_5A09
        mov    dword ptr [edx], 0
loc_3_59F2:
        movzx  ecx, byte ptr [edx + 4]
        or     cl, byte ptr [edx + 5]
        je     loc_3_5A09
        jmp    loc_3_5A04
loc_3_59FD:
        dec    byte ptr [edx + 6]
        jne    loc_3_5A09
        jmp    loc_3_59F2
loc_3_5A04:
        call   sub_3_8690
loc_3_5A09:
        ret    
        db  056h, 057h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 074h, 024h, 010h, 08Bh, 04Ch, 024h, 014h, 0E8h
        dd  sub_4_00A1               ; fixup
        db  05Fh, 05Eh, 0C3h
loc_3_5A20:
        add    esi, 8
loc_3_5A23:
        add    esi, 4
loc_3_5A26:
        add    esi, 4
        ret    
loc_3_5A2A:
        add    esi, 8
loc_3_5A2D:
        add    esi, 4
loc_3_5A30:
        add    esi, 4
        shld   edx, eax, 0x10
        add    ax, 0x8000
        add    dx, 0x8000
        mov    al, ah
        mov    ah, dh
        ret    
loc_3_5A45:
        add    esi, 8
loc_3_5A48:
        add    esi, 4
loc_3_5A4B:
        add    esi, 4
        shld   edx, eax, 0x10
        movsx  edx, dx
        movsx  eax, ax
        add    eax, edx
        sar    eax, 1
        ret    
loc_3_5A5D:
        add    esi, 8
loc_3_5A60:
        add    esi, 4
loc_3_5A63:
        add    esi, 4
        shld   edx, eax, 0x10
        movsx  edx, dx
        movsx  eax, ax
        add    eax, edx
        sar    eax, 1
        add    ax, 0x8000
        mov    al, ah
        ret    
        db  000h, 000h, 000h, 000h, 000h
loc_3_5A80:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   4
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_5AA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x50]
        or     eax, eax
        je     loc_3_5AB6
        cmp    eax, 2
        je     loc_3_5AD1
        cmp    eax, 4
        jne    loc_3_5AD8
loc_3_5AB6:
        and    dword ptr [esi + 0x20], 0xfffeffff
        push   esi
        call   sub_3_69B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
loc_3_5AD1:
        or     dword ptr [esi + 0x20], 0x10000
loc_3_5AD8:
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_5AE0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   1
        push   esi
        call   sub_3_71C0
        add    esp, 8
        mov    dword ptr [esi + 0x50], eax
        mov    eax, dword ptr [esi + 0x50]
        or     eax, eax
        je     loc_3_5B10
        cmp    eax, 1
        je     loc_3_5B21
        cmp    eax, 2
        je     loc_3_5B40
        cmp    eax, 3
        je     loc_3_5B56
        cmp    eax, 4
        je     loc_3_5B70
        pop    esi
        ret    
loc_3_5B10:
        test   byte ptr [esi + 0x21], 0x10
        je     loc_3_5B84
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
loc_3_5B21:
        test   byte ptr [esi + 0x21], 0x10
        je     loc_3_5B84
        mov    dword ptr [esi + 0x24], 8
        mov    dword ptr [esi + 0x50], 0
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
loc_3_5B40:
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        push   esi
        inc    dword ptr [eax + 0xbc]
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
loc_3_5B56:
        mov    ax, word ptr [esi + 0x2c]
        mov    word ptr [esi + 0x2e], ax
        push   0x46
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_5B70:
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        push   esi
        inc    dword ptr [eax + 0xb8]
        call   sub_3_83B0
        add    esp, 4
loc_3_5B84:
        pop    esi
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5B90:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_84F0
        add    esp, 4
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x48]
        mov    dword ptr [edx + 0x10], eax
        mov    dword ptr [edx + 0x14], 0
        test   byte ptr [esi + 0x22], 8
        je     loc_3_5BE2
        mov    eax, dword ptr [edx + 8]
        and    eax, 0xfffffffd
        mov    dword ptr [edx + 8], eax
        mov    ecx, dword ptr [esi + 0x4c]
        mov    dword ptr [edx + 0x1c], ecx
        mov    dword ptr [edx + 0x18], 0x930
        or     eax, 0x200
        mov    dword ptr [edx + 8], eax
        test   byte ptr [esi + 0x22], 4
        je     loc_3_5C12
        or     eax, 0x40000
        mov    dword ptr [edx + 8], eax
        jmp    loc_3_5C12
loc_3_5BE2:
        mov    ecx, dword ptr [esi + 0xb4]
        or     ecx, ecx
        je     loc_3_5C01
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_3_5C01
        or     dword ptr [edx + 8], 2
        mov    dword ptr [edx + 0x18], eax
        add    ecx, 8
        mov    dword ptr [edx + 0x1c], ecx
        jmp    loc_3_5C12
loc_3_5C01:
        and    dword ptr [edx + 8], 0xfffffffd
        mov    dword ptr [edx + 0x18], 1
        mov    eax, dword ptr [esi + 0x4c]
        mov    dword ptr [edx + 0x1c], eax
loc_3_5C12:
        or     dword ptr [esi + 0x20], 0x10000
        push   esi
        call   sub_3_69B0
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_1_08B8
        add    esp, 4
        or     eax, eax
        je     loc_3_5C3A
        mov    eax, dword ptr [esi + 8]
        or     dword ptr [eax + 8], 0x40
loc_3_5C3A:
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_3_57E0
        add    esp, 4
        push   esi
        call   sub_3_68D0
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5C60:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 8]
        mov    ax, word ptr [eax + 6]
        cmp    ax, 0x10
        jb     loc_3_5C99
        cmp    byte ptr [0x9be8], 0          ; -> loc_3_9BE8
        je     loc_3_5C90
        and    eax, 0xffff
        push   eax
        call   sub_3_57F5
        add    esp, 4
        mov    dword ptr [esi + 0x24], eax
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_3_5C90:
        mov    dword ptr [esi + 0x24], 0x65
        pop    esi
        ret    
loc_3_5C99:
        test   byte ptr [esi + 0x23], 8
        jne    loc_3_5CC4
        test   byte ptr [esi + 0x22], 8
        jne    loc_3_5CB4
        mov    eax, dword ptr [esi + 0xb4]
        or     eax, eax
        je     loc_3_5CB4
        cmp    dword ptr [eax], 0
        jne    loc_3_5CC4
loc_3_5CB4:
        mov    eax, dword ptr [esi + 0x4c]
        push   eax
        push   esi
        call   sub_3_7610
        add    esp, 8
        mov    dword ptr [esi + 0x50], eax
loc_3_5CC4:
        mov    dword ptr [esi + 0x24], 0
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_3_5CD0:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax + 0x14]
        mov    eax, dword ptr [eax + 0x44]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x20]
        sub    ebx, ebx
        mov    esi, dword ptr [eax + 0x18]
        lea    eax, [esi + 0x28]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    edi, dword ptr [eax + 0xa4]
        add    edi, dword ptr [eax + 0x98]
        cmp    dword ptr [esp + 0x10], ebx
        je     loc_3_5D2D
        sub    ecx, ecx
        lea    ecx, [ecx]
loc_3_5D10:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        add    eax, ebx
        cmp    edi, eax
        jb     loc_3_5D2D
        mov    ebx, eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, ecx
        jne    loc_3_5D10
loc_3_5D2D:
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_3_5D50
        mov    eax, dword ptr [esp + 0x20]
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5D50:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 4], 0
        jne    loc_3_5D90
        push   eax
        mov    eax, dword ptr [esp + 0x24]
        push   eax
        call   sub_3_96B0
        add    esp, 8
        or     eax, eax
        je     loc_3_5D90
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0x24], 0x15
        mov    eax, dword ptr [esp + 0x20]
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_5D90:
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        cmp    dword ptr [ecx + 4], eax
        je     loc_3_5DC0
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0x24], 0x15
        mov    eax, dword ptr [esp + 0x20]
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_3_5DC0:
        sub    edi, ebx
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x14], edi
        mov    edi, dword ptr [eax + 0xa0]
        sub    edi, dword ptr [eax + 0xa4]
        jne    loc_3_5DF0
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5DF0:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax + 0xc]
        add    ecx, edi
        cmp    ecx, eax
        jbe    loc_3_5E07
        sub    eax, dword ptr [esp + 0x14]
        mov    edi, eax
loc_3_5E07:
        mov    dx, word ptr [esi + 0x12]
        test   dx, 0x850
        jne    loc_3_5EC8
        mov    eax, dword ptr [esp + 0x10]
        mov    dl, byte ptr [eax + 0x13]
        or     dl, dl
        je     loc_3_5E91
        mov    al, byte ptr [eax + 0x12]
        or     al, al
        je     loc_3_5E91
        xor    ebx, ebx
        mov    bl, al
        mov    cl, byte ptr [esp + 0x18]
        shl    ebx, cl
        mov    ebp, edx
        and    ebp, 0xff
        shl    ebp, cl
        mov    eax, esi
        sub    eax, dword ptr [esp + 0x10]
        cmp    eax, -0x28
        je     loc_3_5E50
        sub    esi, esi
        jmp    loc_3_5E56
        db  005h, 000h, 000h, 000h, 000h
loc_3_5E50:
        movzx  esi, byte ptr [esi + 0x26]
        shl    esi, cl
loc_3_5E56:
        add    dword ptr [esp + 0x14], esi
        mov    eax, dword ptr [esp + 0x14]
        sub    edx, edx
        div    ebx
        mov    ecx, eax
        mov    eax, dword ptr [esp + 0x14]
        add    eax, edi
        sub    edx, edx
        div    ebx
        cmp    eax, ecx
        je     loc_3_5E7C
        lea    edi, [ecx + 1]
        imul   edi, ebx
        sub    edi, dword ptr [esp + 0x14]
loc_3_5E7C:
        add    ebp, ebx
        imul   ebp, ecx
        mov    eax, dword ptr [esp + 0x14]
        sub    edx, edx
        div    ebx
        add    ebp, edx
        sub    ebp, esi
        mov    dword ptr [esp + 0x14], ebp
loc_3_5E91:
        mov    eax, dword ptr [esp + 0x10]
        xor    edx, edx
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [esp + 0x18]
        shld   edx, eax, cl
        shl    eax, cl
        add    eax, dword ptr [esp + 0x14]
        adc    edx, 0
        mov    ecx, eax
        shrd   eax, edx, 0xb
        and    ecx, 0x7ff
        mov    edx, dword ptr [esp + 0x20]
        mov    dword ptr [edx + 0x48], eax
        mov    dword ptr [edx + 0xac], ecx
        jmp    loc_3_5F60
loc_3_5EC8:
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 0xac], eax
        mov    ebx, 0x930
        test   dl, 0x40
        je     loc_3_5F00
        xor    eax, eax
        mov    al, byte ptr [esi + 0x44]
        mov    cl, byte ptr [eax + eax*4 + 0xa054] ; -> loc_3_A054
        shr    cl, 4
        mov    eax, dword ptr [esp + 0x20]
        shr    ebx, cl
        or     dword ptr [eax + 0x20], 0x40000
        jmp    loc_3_5F05
        db  08Bh, 0FFh
loc_3_5F00:
        test   dl, 0x10
        je     loc_3_5F10
loc_3_5F05:
        mov    eax, dword ptr [esp + 0x20]
        sub    dword ptr [eax + 0xac], 0x2c
loc_3_5F10:
        mov    eax, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax + 0xac]
        mov    dword ptr [ecx + 0x48], eax
        sub    edx, edx
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [ecx + 0xac]
        add    ecx, 0xac
        div    ebx
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [ecx + 0x48]
        add    ecx, 0x48
        sub    edx, edx
        div    ebx
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax + 8]
        add    dword ptr [ecx + 0x48], eax
        mov    eax, dword ptr [esp + 0x20]
        or     dword ptr [eax + 0x20], 0x80000
loc_3_5F60:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [eax + 0xa8], edi
        mov    eax, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax + 0xb4]
        or     eax, eax
        je     loc_3_5F7E
        mov    dword ptr [eax], 0
loc_3_5F7E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_5F90:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
loc_3_5F97:
        push   esi
        call   sub_3_7170
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_3_60D0
        lea    eax, [ebx + 0x14]
        mov    dword ptr [esi + 0xb0], eax
        and    dword ptr [esi + 0x20], 0xfffffeff
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        jne    loc_3_6138
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        mov    al, byte ptr [ebx + 0x1e]
        or     al, al
        lea    ecx, [eax + 1]
        mov    byte ptr [ebx + 0x1e], cl
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        jne    loc_3_5FF7
        push   ebx
        push   edi
        call   sub_3_7A20
        add    esp, 8
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
loc_3_5FF7:
        inc    dword ptr [eax + 0xb8]
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [esi + 0x4c], eax
        push   esi
        call   sub_3_6430
        add    esp, 4
        mov    dword ptr [esi + 0x4c], 0
        mov    al, byte ptr [ebx + 0x1e]
        dec    al
        mov    byte ptr [ebx + 0x1e], al
        jne    loc_3_607A
        mov    al, byte ptr [ebx + 0x1c]
        cmp    al, 3
        jne    loc_3_6062
        test   byte ptr [ebx + 0x1f], 1
        jne    loc_3_6062
        cmp    al, 3
        jne    loc_3_6048
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [edi + 0x50]
        cmp    eax, dword ptr [edi + 0x58]
        ja     loc_3_6048
        mov    eax, dword ptr [ebx + 0x10]
        sub    eax, dword ptr [edi + 0x54]
        cmp    eax, dword ptr [edi + 0x5c]
        jbe    loc_3_6062
loc_3_6048:
        push   ebx
        push   edi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [ebx + 0x1c], 2
        push   ebx
        push   edi
        call   sub_3_7D70
        add    esp, 8
        jmp    loc_3_607A
loc_3_6062:
        mov    al, byte ptr [ebx + 0x1f]
        and    al, 1
        cmp    al, 1
        sbb    eax, eax
        and    eax, 4
        inc    eax
        push   eax
        push   ebx
        push   edi
        call   sub_3_7AB0
        add    esp, 0xc
loc_3_607A:
        push   esi
        call   sub_3_59D8
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_6172
        mov    eax, dword ptr [esi + 0xa4]
        cmp    dword ptr [esi + 0xa0], eax
        je     loc_3_6172
        cmp    dword ptr [esi + 0xa8], 0
        je     loc_3_6147
        and    dword ptr [esi + 0x20], 0xfffeffff
        push   esi
        call   sub_3_69B0
        add    esp, 4
        or     eax, eax
        jne    loc_3_6151
        inc    dword ptr [esi + 0x48]
        jmp    loc_3_5F97
        db  08Dh, 064h, 024h, 000h
loc_3_60D0:
        mov    ecx, dword ptr [esi + 0xb4]
        or     ecx, ecx
        je     loc_3_6129
        test   byte ptr [esi + 0x22], 8
        jne    loc_3_6129
        cmp    dword ptr [esi + 0xa8], 0x800
        jb     loc_3_6129
        mov    edx, dword ptr [esi + 0xa8]
        shr    edx, 0xb
        cmp    edx, 0x10
        jbe    loc_3_60FF
        mov    edx, 0x10
loc_3_60FF:
        mov    eax, dword ptr [esi + 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        cmp    eax, edx
        jae    loc_3_6111
        mov    edx, eax
loc_3_6111:
        mov    eax, dword ptr [esi + 0x48]
        add    eax, edx
        dec    eax
        mov    dword ptr [ecx + 4], eax
        push   0x4a
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_6129:
        push   0xa
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_6138:
        push   0xa
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_6147:
        mov    word ptr [esi + 0x2e], 0x38
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_6151:
        inc    dword ptr [esi + 0x48]
        push   esi
        call   sub_3_84F0
        add    esp, 4
        push   esi
        call   sub_3_8580
        add    esp, 4
        mov    ax, word ptr [esi + 0x2c]
        mov    word ptr [esi + 0x2e], ax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_6172:
        and    dword ptr [esi + 0x20], 0xfffeffff
        push   esi
        call   sub_3_69B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_3_6190:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0xb4]
        or     eax, eax
        je     loc_3_61A5
        mov    dword ptr [eax], 0
loc_3_61A5:
        cmp    dword ptr [esi + 0x24], 0
        push   esi
        je     loc_3_61B6
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
loc_3_61B6:
        call   sub_3_6430
        add    esp, 4
        push   esi
        call   sub_3_75B0
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_61F1
        mov    eax, dword ptr [esi + 0xa4]
        cmp    dword ptr [esi + 0xa0], eax
        je     loc_3_61F1
        cmp    dword ptr [esi + 0xa8], 0
        jne    loc_3_61EC
        mov    word ptr [esi + 0x2e], 0x38
        pop    esi
        ret    
loc_3_61EC:
        inc    dword ptr [esi + 0x48]
        pop    esi
        ret    
loc_3_61F1:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_6200:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    ebx, eax
        mov    eax, dword ptr [esi + 0xb4]
        mov    ebp, dword ptr [eax + 4]
        sub    ebp, dword ptr [esi + 0x48]
        inc    ebp
        cmp    ebp, 0x10
        jbe    loc_3_622A
        mov    ebp, 0x10
loc_3_622A:
        mov    eax, dword ptr [esi + 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        cmp    ebp, eax
        jbe    loc_3_623C
        mov    ebp, eax
loc_3_623C:
        mov    dword ptr [esi + 0x24], 0
        mov    dword ptr [esi + 0x50], 0
        sub    edi, edi
        cmp    ebp, edi
        je     loc_3_62B7
loc_3_6250:
        push   esi
        call   sub_3_7170
        add    esp, 4
        or     eax, eax
        jne    loc_3_62B7
        test   byte ptr [esi + 0x21], 0x10
        je     loc_3_6275
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    dword ptr [ebx + 0x2c], eax
        ja     loc_3_62B7
        mov    eax, dword ptr [ebx + 0x18]
        add    eax, edi
        cmp    eax, dword ptr [ebx + 0x78]
        jae    loc_3_62B7
loc_3_6275:
        mov    eax, dword ptr [esi + 0x20]
        and    eax, 8
        push   eax
        push   esi
        call   sub_3_71C0
        add    esp, 8
        mov    dword ptr [esi + 0x50], eax
        mov    eax, dword ptr [esi + 0x50]
        or     eax, eax
        mov    ecx, dword ptr [0x890]        ; -> obj1:0x0890
        je     loc_3_62B0
        cmp    eax, 1
        je     loc_3_6340
        cmp    eax, 2
        je     loc_3_6300
        cmp    eax, 4
        je     loc_3_6357
loc_3_62AC:
        and    dword ptr [esi + 0x20], 0xffffffbf
loc_3_62B0:
        mov    dword ptr [esi + 0x50], 0
loc_3_62B7:
        or     edi, edi
        je     loc_3_6365
        mov    eax, dword ptr [esi + 0xb4]
        mov    ecx, dword ptr [esi + 0x48]
        dec    ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], edi
        sub    dword ptr [esi + 0x48], edi
        mov    dword ptr [eax + edi*8 + 0xc], 0
        mov    eax, dword ptr [esi + 0xb4]
        push   6
        mov    dword ptr [eax + edi*8 + 8], 0
        mov    dword ptr [esi + 0x24], 0
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_3_6300:
        inc    dword ptr [ecx + 0xbc]
        mov    ecx, dword ptr [esi + 0xb4]
        mov    eax, dword ptr [esi + 0x4c]
        inc    edi
        mov    dword ptr [ecx + edi*8 + 4], eax
        mov    eax, dword ptr [esi + 0xb4]
        mov    dword ptr [eax + edi*8], 1
        mov    dword ptr [esi + 0x50], 0
        mov    dword ptr [esi + 0x4c], 0
        inc    dword ptr [esi + 0x48]
        cmp    ebp, edi
        ja     loc_3_6250
        jmp    loc_3_62B7
        db  090h
loc_3_6340:
        or     edi, edi
        jne    loc_3_62AC
        test   byte ptr [esi + 0x21], 0x10
        jne    loc_3_62B7
        jmp    loc_3_62AC
loc_3_6357:
        push   esi
        call   sub_3_75B0
        add    esp, 4
        jmp    loc_3_62B7
loc_3_6365:
        test   byte ptr [esi + 0x21], 0x10
        je     loc_3_6380
        mov    dword ptr [esi + 0x24], 5
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_6380:
        mov    dword ptr [esi + 0x24], 0
        push   esi
        call   sub_3_83B0
        add    esp, 4
        push   0xa
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_63A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, 0x7670                   ; -> sub_3_7670
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_63C1
        mov    edi, 0x7610                   ; -> sub_3_7610
loc_3_63C1:
        mov    dword ptr [esi + 0x54], 0
        mov    eax, dword ptr [esi + 0xb4]
        sub    ebx, ebx
        cmp    dword ptr [eax], ebx
        jbe    loc_3_6404
        sub    ebp, ebp
loc_3_63D6:
        mov    eax, dword ptr [esi + 0xb4]
        mov    eax, dword ptr [ebp + eax + 0xc]
        mov    dword ptr [esi + 0x4c], eax
        push   eax
        push   esi
        call   edi
        add    esp, 8
        mov    dword ptr [esi + 0x50], eax
        push   esi
        call   sub_3_75B0
        add    esp, 4
        add    ebp, 8
        mov    eax, dword ptr [esi + 0xb4]
        inc    ebx
        cmp    dword ptr [eax], ebx
        ja     loc_3_63D6
loc_3_6404:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_642B
        test   byte ptr [esi + 0x21], 0x10
        jne    loc_3_642B
        mov    eax, dword ptr [esi + 0xb4]
        mov    word ptr [esi + 0x2e], 0x39
        mov    dword ptr [eax], 0
loc_3_642B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_6430:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x2c]
        mov    esi, 1
        mov    eax, dword ptr [eax + 0x14]
        sub    edx, edx
        mov    eax, dword ptr [eax + 0xc]
        mov    cl, byte ptr [eax + 0xc]
        mov    eax, dword ptr [esp + 0x2c]
        shl    esi, cl
        mov    eax, dword ptr [eax + 0xa8]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x2c]
        test   byte ptr [eax + 0x22], 8
        je     loc_3_64A3
        mov    esi, 0x930
        mov    ecx, eax
        mov    eax, dword ptr [ecx + 0x18]
        add    ecx, 0x18
        test   byte ptr [eax + 0x12], 0x40
        je     loc_3_64A3
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        test   byte ptr [eax + eax*4 + 0xa054], 0xf0 ; -> loc_3_A054
        je     loc_3_64A3
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        lea    edx, [eax + eax*4 + 0xa054]   ; -> loc_3_A054
        mov    cl, byte ptr [edx]
        shr    cl, 4
        shr    esi, cl
loc_3_64A3:
        mov    eax, dword ptr [esp + 0x2c]
        mov    ecx, dword ptr [eax + 0xac]
        add    eax, 0xac
        add    ecx, dword ptr [esp + 0x10]
        cmp    ecx, esi
        jbe    loc_3_64C0
        sub    esi, dword ptr [eax]
        mov    dword ptr [esp + 0x10], esi
loc_3_64C0:
        or     edx, edx
        jne    loc_3_650C
        mov    eax, dword ptr [esp + 0x2c]
        mov    esi, dword ptr [eax + 0x4c]
        mov    edi, dword ptr [eax + 0x9c]
        mov    ecx, dword ptr [esp + 0x10]
        add    esi, dword ptr [eax + 0xac]
        add    edi, dword ptr [eax + 0xa4]
        call   sub_4_00A1
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [esp + 0x10], eax
        je     loc_3_65D7
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [eax + 0x24], 8
        jmp    loc_3_65D7
loc_3_650C:
        mov    cl, byte ptr [edx]
        mov    al, cl
        and    al, 2
        shr    al, 1
        and    eax, 0xff
        and    ecx, 1
        add    ecx, eax
        mov    eax, 4
        sar    eax, cl
        mov    dword ptr [esp + 0xc], eax
        dec    eax
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, 4
        mov    cl, byte ptr [edx]
        and    cl, 0xc
        shr    cl, 2
        shl    eax, cl
        dec    eax
        not    eax
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [edx + 1]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    esi, dword ptr [eax + 0xac]
        and    esi, dword ptr [esp + 0x24]
        add    esi, dword ptr [eax + 0x4c]
        mov    edi, dword ptr [eax + 0xa4]
        mov    ebx, edi
        and    ebx, dword ptr [esp + 0x1c]
        add    edi, dword ptr [eax + 0x9c]
        mov    ecx, dword ptr [esp + 0x10]
        jecxz  loc_3_65D7
        or     ebx, ebx
        mov    ebx, dword ptr [esp + 0x20]
        je     loc_3_65A9
        mov    eax, dword ptr [esi]
        call   ebx
        mov    dword ptr [esp + 0x18], eax
        mov    eax, esi
        lea    esi, [esp + 0x18]
        mov    edx, ecx
        and    ecx, dword ptr [esp + 0x1c]
        sub    edx, ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        mov    esi, eax
        jmp    loc_3_65A9
loc_3_65A2:
        mov    dword ptr [edi], eax
        add    edi, 2
loc_3_65A7:
        inc    edi
loc_3_65A8:
        inc    edi
loc_3_65A9:
        sub    ecx, dword ptr [esp + 0xc]
        jb     loc_3_65C3
        mov    eax, dword ptr [esi]
        call   ebx
        cmp    dword ptr [esp + 0xc], 2
        jg     loc_3_65A2
        mov    byte ptr [edi], al
        jl     loc_3_65A8
        mov    word ptr [edi], ax
        jmp    loc_3_65A7
loc_3_65C3:
        add    ecx, dword ptr [esp + 0xc]
        je     loc_3_65D7
        mov    eax, dword ptr [esi]
        call   ebx
        mov    dword ptr [esp + 0x18], eax
        lea    esi, [esp + 0x18]
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_65D7:
        mov    eax, dword ptr [esp + 0x2c]
        mov    ecx, dword ptr [esp + 0x14]
        sub    dword ptr [eax + 0xa8], ecx
        mov    ecx, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [esp + 0x14]
        add    dword ptr [ecx + 0xa4], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [eax + 0xac], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_6610:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0xa4]
        cmp    dword ptr [esi + 0xa0], eax
        ja     loc_3_6630
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_3_6630:
        mov    eax, dword ptr [esi + 0xa4]
        mov    dword ptr [esi + 0x48], eax
        test   byte ptr [esi + 0x22], 8
        jne    loc_3_6645
        shr    dword ptr [esi + 0x48], 0xb
        jmp    loc_3_6654
loc_3_6645:
        mov    ecx, 0x930
        mov    eax, dword ptr [esi + 0x48]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esi + 0x48], eax
loc_3_6654:
        mov    eax, dword ptr [esi + 0x98]
        add    dword ptr [esi + 0x48], eax
        push   0xa
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_6670:
        sub    esp, 8
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 8], 0x800
        cmp    dword ptr [eax + 0x24], 0
        je     loc_3_6696
        push   eax
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 8
        ret    
loc_3_6696:
        mov    eax, dword ptr [esp + 0x14]
        test   byte ptr [eax + 0x22], 8
        je     loc_3_66A8
        mov    dword ptr [esp + 8], 0x930
loc_3_66A8:
        mov    eax, dword ptr [esp + 0x14]
        mov    esi, dword ptr [eax + 0x4c]
        mov    edi, dword ptr [eax + 0x9c]
        add    edi, dword ptr [eax + 0xa4]
        mov    ecx, dword ptr [esp + 8]
        call   sub_4_00A1
        mov    dword ptr [esp + 0xc], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0xc]
        add    dword ptr [eax + 0xa4], ecx
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [esp + 0xc], eax
        jae    loc_3_66F8
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x24], 8
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_83B0
        add    esp, 4
loc_3_66F8:
        pop    edi
        pop    esi
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_3_6700:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 8], 0
        je     loc_3_671B
        xor    ecx, ecx
        mov    cx, word ptr [eax + 0x2c]
        cmp    ecx, 6
        jne    loc_3_671B
        mov    word ptr [eax + 0x2e], 8
loc_3_671B:
        ret    
        db  08Dh, 064h, 024h, 000h
sub_3_6720:
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        mov    word ptr [esp + 4], 2
        mov    eax, dword ptr [esi + 4]
        mov    cx, word ptr [eax + 8]
        shl    cx, 3
        add    cx, word ptr [eax + 0xa]
        mov    word ptr [esp + 8], cx
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esp + 0xa], eax
        mov    eax, dword ptr [esi + 0x20]
        and    eax, 0x1080
        cmp    eax, 1
        sbb    al, al
        and    al, 1
        inc    al
        mov    byte ptr [esp + 0x12], al
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_08A8
        add    esp, 4
        cmp    dword ptr [esp + 0xe], 0
        jne    loc_3_6796
        mov    dword ptr [esi + 0x24], 8
        test   byte ptr [esi + 0x21], 0x10
        jne    loc_3_6819
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        add    esp, 0x10
        ret    
loc_3_6796:
        mov    ecx, dword ptr [esp + 0xa]
        add    ecx, dword ptr [esp + 0xe]
        mov    dword ptr [esi + 8], ecx
        lea    eax, [ecx + 0x58]
        mov    dword ptr [ecx + 0x48], eax
        mov    dword ptr [ecx], 0
        mov    word ptr [ecx + 4], 0
        mov    word ptr [ecx + 6], 0
        mov    dword ptr [ecx + 8], 0x401
        mov    dword ptr [ecx + 0xc], 0x8580 ; -> sub_3_8580
        mov    dword ptr [ecx + 0x40], esi
        mov    dword ptr [ecx + 0x10], 0
        mov    dword ptr [ecx + 0x14], 0
        mov    dword ptr [ecx + 0x18], 0
        mov    byte ptr [ecx + 0x4c], 0
        mov    eax, dword ptr [esp + 0xa]
        mov    dword ptr [ecx + 0x20], eax
        mov    al, byte ptr [esi + 0x40]
        mov    byte ptr [ecx + 0x4d], al
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 0x44], eax
        mov    al, byte ptr [esi + 0x6f]
        test   al, 1
        je     loc_3_6801
        or     dword ptr [ecx + 8], 8
loc_3_6801:
        test   al, 2
        je     loc_3_680C
        or     dword ptr [ecx + 8], 0x20000
loc_3_680C:
        test   byte ptr [esi + 0x20], 0x80
        je     loc_3_6819
        or     dword ptr [ecx + 8], 0x800
loc_3_6819:
        pop    esi
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
sub_3_6820:
        sub    esp, 8
        mov    eax, dword ptr [esp + 0xc]
        cmp    dword ptr [eax + 8], 0
        je     loc_3_6856
        mov    word ptr [esp], 4
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 8]
        sub    ecx, dword ptr [edx + 0xc]
        mov    dword ptr [esp + 4], ecx
        mov    dword ptr [eax + 8], 0
        lea    eax, [esp]
        push   eax
        call   sub_1_08A8
        add    esp, 4
loc_3_6856:
        add    esp, 8
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_6860:
        sub    esp, 4
        cmp    dword ptr [esp + 8], 0
        jne    loc_3_6870
        sub    eax, eax
        add    esp, 4
        ret    
loc_3_6870:
        mov    eax, 0x20
        mov    edx, dword ptr [esp + 8]
loc_3_6879:
        dec    eax
        shl    edx, 1
        jae    loc_3_6879
        mov    dword ptr [esp], eax
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_6890:
        sub    esp, 4
        push   ebx
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        shr    ecx, 2
loc_3_68A3:
        add    bl, byte ptr [edx + ecx*4 - 4]
        add    bh, byte ptr [edx + ecx*4 - 3]
        add    al, byte ptr [edx + ecx*4 - 2]
        add    ah, byte ptr [edx + ecx*4 - 1]
        dec    ecx
        jne    loc_3_68A3
        shl    eax, 0x10
        mov    ax, bx
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esp + 4]
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_68D0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        test   dword ptr [esi + 0x20], 0x40008
        jne    loc_3_6996
        cmp    dword ptr [0x9bf8], 0         ; -> loc_3_9BF8
        jne    loc_3_6996
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_3_6996
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_3_6989
        sub    ebx, ebx
loc_3_6918:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [edi + 0x3c], eax
        je     loc_3_6989
        mov    eax, dword ptr [eax + 0x20]
        mov    ebp, eax
        sub    ebp, dword ptr [edi + 0x68]
        and    ebp, 0xfffff1ff
        shr    ebp, 9
        add    ebp, dword ptr [edi + 4]
        cmp    word ptr [ebp + 6], bx
        jne    loc_3_697A
        push   ebx
        push   1
        shr    eax, 0xc
        push   eax
        call   sub_3_5150
        add    esp, 0xc
        cmp    eax, ebx
        je     loc_3_69A0
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x20]
        and    eax, 0xfffff000
        push   eax
        call   sub_3_51B0
        add    esp, 4
        mov    dword ptr [ebp], eax
        inc    word ptr [ebp + 6]
        test   byte ptr [esi + 0x20], 2
        jne    loc_3_6996
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 0x10], eax
loc_3_697A:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, ebx
        jne    loc_3_6918
loc_3_6989:
        mov    eax, dword ptr [0x9bf8]       ; -> loc_3_9BF8
        or     eax, 1
        mov    dword ptr [0x9bf8], eax       ; -> loc_3_9BF8
loc_3_6996:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_3_69A0:
        call   sub_3_7F30
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_3_69B0:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x10], 0
        test   byte ptr [esi + 0x21], 0x50
        jne    loc_3_6C61
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_3_69E6
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_3_69E6:
        mov    edx, dword ptr [edi + 0x2c]
        cmp    dword ptr [edi + 0x28], edx
        ja     loc_3_69F8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_3_69F8:
        test   byte ptr [esi + 0x22], 1
        jne    loc_3_6AF0
        cmp    dword ptr [0x9c80], 0         ; -> obj3:0x9C80
        je     loc_3_6AF0
        mov    dword ptr [esp + 0x18], 0x9c8c ; -> obj3:0x9C8C
        test   byte ptr [0x9cad], 0x20       ; -> loc_3_9CAD
        jne    loc_3_6AB0
        mov    eax, dword ptr [0x9ce8]       ; -> obj3:0x9CE8
        cmp    dword ptr [0x9cd4], eax       ; -> obj3:0x9CD4
        jae    loc_3_6AB0
        mov    ecx, dword ptr [edi + 0x1c]
        cmp    edx, ecx
        ja     loc_3_6AB0
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [edi + 0x78], eax
        jbe    loc_3_6AB0
        mov    ebx, dword ptr [0x9c74]       ; -> loc_3_9C74
        cmp    dword ptr [ebx + 0x10], 0
        je     loc_3_6A78
        cmp    dword ptr [ebx + 0x14], eax
        jae    loc_3_6A78
        test   byte ptr [esi + 0x22], 8
        jne    loc_3_6A78
        mov    eax, dword ptr [esi + 0x14]
        mov    edi, dword ptr [ebx + 0x18]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0xffff
        cmp    eax, edi
        jb     loc_3_6A78
        lea    eax, [edx + edi]
        cmp    eax, ecx
        ja     loc_3_6C9F
loc_3_6A78:
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        or     dword ptr [0x9cac], 0x2000    ; -> obj3:0x9CAC
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x9c78], eax       ; -> obj3:0x9C78
        call   sub_1_0340
        push   0x9c8c                        ; -> obj3:0x9C8C
        call   sub_3_8580
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_3_6AB0:
        mov    eax, dword ptr [edi + 0x4c]
        cmp    dword ptr [esi + 0x48], eax
        jne    loc_3_6C9F
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        mov    ebx, dword ptr [0x9c74]       ; -> loc_3_9C74
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [edi + 0x48]
        cmp    eax, dword ptr [ebx + 0x20]
        jae    loc_3_6C9F
        cmp    byte ptr [esi + 0x63], 5
        jne    loc_3_6C9F
        mov    dword ptr [edi + 0x4c], 0xffffffff
        mov    dword ptr [esp + 0x14], 1
        jmp    loc_3_6AF8
loc_3_6AF0:
        mov    dword ptr [esp + 0x14], 0
loc_3_6AF8:
        mov    eax, dword ptr [esi + 0xb4]
        or     eax, eax
        je     loc_3_6B08
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        jne    loc_3_6B0D
loc_3_6B08:
        mov    ebx, 1
loc_3_6B0D:
        add    ebx, dword ptr [esi + 0x48]
        xor    eax, eax
        mov    al, byte ptr [esi + 0x63]
        cmp    eax, 5
        je     loc_3_6B24
        cmp    eax, 6
        je     loc_3_6B70
        jmp    loc_3_6BC0
loc_3_6B24:
        cmp    dword ptr [esi + 0x18], 0
        je     loc_3_6BC0
        mov    edx, dword ptr [esi + 0x18]
        mov    ebp, dword ptr [esi + 0x14]
        add    edx, 0x28
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0xc]
        sub    cl, byte ptr [ebp + 0x44]
loc_3_6B40:
        mov    eax, dword ptr [edx + 8]
        shr    eax, cl
        cmp    eax, ebx
        ja     loc_3_6B61
        mov    ebp, dword ptr [edx + 0xc]
        shr    ebp, 0xb
        add    eax, ebp
        cmp    eax, ebx
        jb     loc_3_6B61
        cmp    byte ptr [edx + 0x12], 0
        je     loc_3_6B69
        cmp    byte ptr [edx + 0x13], 0
        je     loc_3_6B69
loc_3_6B61:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_3_6B40
        jmp    loc_3_6BC0
loc_3_6B69:
        mov    dword ptr [esp + 0x10], eax
        jmp    loc_3_6BC0
        db  090h
loc_3_6B70:
        test   byte ptr [esi + 0x23], 1
        je     loc_3_6B90
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [eax + 0x24]
        mov    eax, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [eax + 0x20]
        lea    eax, [ecx + eax - 1]
        mov    dword ptr [esp + 0x10], eax
        jmp    loc_3_6BC0
        db  08Dh, 064h, 024h, 000h
loc_3_6B90:
        mov    ecx, dword ptr [esi + 0x14]
        or     ecx, ecx
        je     loc_3_6BC0
        cmp    dword ptr [ecx + 0x24], 0
        je     loc_3_6BC0
        cmp    dword ptr [ecx + 0x28], 0
        je     loc_3_6BC0
        mov    eax, dword ptr [ecx + 0x24]
        mov    edx, dword ptr [ecx + 0x28]
        lea    eax, [eax + edx - 1]
        mov    dword ptr [esp + 0x10], eax
        sub    eax, ebx
        cmp    eax, dword ptr [ecx + 0x28]
        jbe    loc_3_6BC0
        mov    dword ptr [esp + 0x10], 0
loc_3_6BC0:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_6C9F
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_6BDC
        mov    eax, dword ptr [esp + 0x18]
        test   byte ptr [eax + 0x21], 0x20
        jne    loc_3_6C54
loc_3_6BDC:
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, dword ptr [0x9c84]       ; -> obj3:0x9C84
        jne    loc_3_6C16
        cmp    dword ptr [0x9c7c], ebx       ; -> obj3:0x9C7C
        ja     loc_3_6C16
        mov    eax, dword ptr [0x9c7c]       ; -> obj3:0x9C7C
        add    eax, 0x10
        cmp    eax, ebx
        jb     loc_3_6C16
        mov    eax, dword ptr [0x9c80]       ; -> obj3:0x9C80
        cmp    dword ptr [esi + 0x14], eax
        jne    loc_3_6C16
        mov    eax, dword ptr [esi + 0x20]
        xor    eax, dword ptr [0x9c88]       ; -> obj3:0x9C88
        test   eax, 0xc0000
        je     loc_3_6C25
loc_3_6C16:
        cmp    dword ptr [esp + 0x14], 0
        mov    byte ptr [edi + 0x43], 4
        je     loc_3_6C25
        mov    byte ptr [edi + 0x43], 5
loc_3_6C25:
        push   edi
        call   sub_3_7E10
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 4
        push   eax
        push   ebx
        push   esi
        call   sub_3_6CB0
        add    esp, 0xc
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_3_6C54
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 0x50], eax
        mov    eax, dword ptr [esi + 0x48]
        mov    dword ptr [edi + 0x54], eax
loc_3_6C54:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_3_6C61:
        test   byte ptr [esi + 0x21], 0x10
        je     loc_3_6C9F
        mov    eax, dword ptr [0x9c80]       ; -> obj3:0x9C80
        cmp    dword ptr [esi + 0x14], eax
        jne    loc_3_6C9F
        mov    eax, dword ptr [0x9c7c]       ; -> obj3:0x9C7C
        cmp    dword ptr [esi + 0x48], eax
        jne    loc_3_6C9F
        test   byte ptr [esi + 0x22], 8
        je     loc_3_6C91
        inc    dword ptr [0x9c7c]            ; -> obj3:0x9C7C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_3_6C91:
        mov    eax, dword ptr [esi + 0xb4]
        mov    eax, dword ptr [eax]
        add    dword ptr [0x9c7c], eax       ; -> obj3:0x9C7C
loc_3_6C9F:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_6CB0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x14]
        mov    dword ptr [0x9c80], eax       ; -> obj3:0x9C80
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0x9c7c], eax       ; -> obj3:0x9C7C
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [0x9c84], eax       ; -> obj3:0x9C84
        mov    eax, dword ptr [ecx + 0x20]
        mov    dword ptr [0x9c88], eax       ; -> obj3:0x9C88
        push   ecx
        call   sub_3_78F0
        add    esp, 4
        test   byte ptr [0x9cad], 0x20       ; -> loc_3_9CAD
        jne    loc_3_6D10
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        or     dword ptr [0x9cac], 0x2000    ; -> obj3:0x9CAC
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x9c78], eax       ; -> obj3:0x9C78
        call   sub_1_0340
        push   0x9c8c                        ; -> obj3:0x9C8C
        call   sub_3_8580
        add    esp, 4
loc_3_6D10:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_6D20:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0x24], 0
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x9c78], eax       ; -> obj3:0x9C78
        mov    eax, dword ptr [0x9c80]       ; -> obj3:0x9C80
        cmp    dword ptr [esi + 0x14], eax
        jne    loc_3_6D53
        cmp    dword ptr [esi + 8], 0
        je     loc_3_6D53
        cmp    dword ptr [esi + 4], 0
        jne    loc_3_6E38
loc_3_6D53:
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi + 4]
        lea    edi, [esi + 4]
        or     eax, eax
        je     loc_3_6D79
        push   eax
        mov    eax, dword ptr [esi + 0x40]
        push   eax
        call   sub_3_31B6
        add    esp, 8
        mov    dword ptr [edi], 0
loc_3_6D79:
        mov    eax, dword ptr [0x9c80]       ; -> obj3:0x9C80
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [0x9c7c]       ; -> obj3:0x9C7C
        mov    dword ptr [esi + 0x48], eax
        and    dword ptr [esi + 0x20], 0xfff3ffff
        mov    eax, dword ptr [0x9c88]       ; -> obj3:0x9C88
        and    eax, 0xc0000
        or     dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [esi + 0x14]
        or     eax, eax
        je     loc_3_6E0F
        inc    word ptr [eax + 0x1c]
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [esi + 0x40], edx
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x44], ecx
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 0x1c]
        sub    ecx, 0xc3
        test   byte ptr [ecx + 0x21], 0x10
        jne    loc_3_6DF2
        cmp    dword ptr [ecx + 0x18], eax
        jne    loc_3_6DF2
        push   1
        push   edi
        push   edx
        call   sub_3_31B0
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_6DF2
        mov    ecx, dword ptr [esi + 0x14]
        mov    eax, dword ptr [edi]
        cmp    dword ptr [ecx + 4], eax
        je     loc_3_6DF8
        push   eax
        mov    eax, dword ptr [esi + 0x40]
        push   eax
        call   sub_3_31B6
        add    esp, 8
loc_3_6DF2:
        mov    dword ptr [edi], 0
loc_3_6DF8:
        push   esi
        call   sub_3_4660
        add    esp, 4
        cmp    dword ptr [edi], 0
        je     loc_3_6E0F
        push   esi
        call   sub_3_6720
        add    esp, 4
loc_3_6E0F:
        mov    ecx, dword ptr [esi + 0x14]
        or     ecx, ecx
        je     loc_3_6F30
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_3_6F30
        or     dword ptr [eax + 8], 0x20000
        cmp    ecx, dword ptr [0x9c80]       ; -> obj3:0x9C80
        jne    loc_3_6F37
loc_3_6E38:
        mov    eax, dword ptr [0x9c7c]       ; -> obj3:0x9C7C
        mov    dword ptr [esi + 0x48], eax
        mov    eax, dword ptr [0x9c84]       ; -> obj3:0x9C84
        mov    dword ptr [esi + 0x5c], eax
        and    dword ptr [esi + 0x20], 0xfff3ffff
        mov    eax, dword ptr [0x9c88]       ; -> obj3:0x9C88
        and    eax, 0xc0000
        or     dword ptr [esi + 0x20], eax
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_3_6E75
        mov    dword ptr [esi + 0x24], 5
        pop    edi
        pop    esi
        ret    
loc_3_6E75:
        mov    eax, dword ptr [esi + 0x5c]
        cmp    dword ptr [esi + 0x48], eax
        ja     loc_3_6EB1
loc_3_6E7D:
        push   esi
        call   sub_3_7170
        add    esp, 4
        or     eax, eax
        je     loc_3_6EB1
        inc    dword ptr [esi + 0x48]
        mov    ecx, dword ptr [esi + 0x5c]
        cmp    dword ptr [esi + 0x48], ecx
        ja     loc_3_6EB1
loc_3_6E95:
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_3_6EAC
        mov    edx, dword ptr [esi + 0x48]
        cmp    dword ptr [eax + 0x10], edx
        jne    loc_3_6EAC
        inc    dword ptr [esi + 0x48]
        cmp    dword ptr [esi + 0x48], ecx
        jbe    loc_3_6E95
loc_3_6EAC:
        cmp    dword ptr [esi + 0x48], ecx
        jbe    loc_3_6E7D
loc_3_6EB1:
        mov    eax, dword ptr [esi + 0x48]
        mov    dword ptr [0x9c7c], eax       ; -> obj3:0x9C7C
        mov    ecx, dword ptr [esi + 0x5c]
        cmp    dword ptr [esi + 0x48], ecx
        ja     loc_3_6F25
        mov    eax, dword ptr [edi + 0x1c]
        cmp    dword ptr [edi + 0x2c], eax
        ja     loc_3_6F25
        mov    eax, dword ptr [edi + 0x78]
        cmp    dword ptr [edi + 0x18], eax
        jae    loc_3_6F25
        sub    ecx, dword ptr [esi + 0x48]
        mov    eax, dword ptr [0x9c74]       ; -> loc_3_9C74
        inc    ecx
        mov    eax, dword ptr [eax + 0x2c]
        cmp    ecx, eax
        jbe    loc_3_6EE3
        mov    ecx, eax
loc_3_6EE3:
        test   byte ptr [esi + 0x22], 8
        je     loc_3_6EEE
        mov    ecx, 1
loc_3_6EEE:
        mov    eax, dword ptr [esi + 0xb4]
        mov    edx, dword ptr [esi + 0x48]
        add    edx, ecx
        dec    edx
        mov    dword ptr [eax + 4], edx
        test   byte ptr [esi + 0x22], 8
        je     loc_3_6F11
        push   0xa
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_3_6F11:
        mov    dword ptr [eax], 0
        push   0x4a
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_3_6F25:
        mov    dword ptr [esi + 0x24], 5
        pop    edi
        pop    esi
        ret    
        db  090h
loc_3_6F30:
        mov    dword ptr [esi + 0x24], 5
loc_3_6F37:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_6F40:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 0x22], 8
        je     loc_3_6F54
        push   esi
        call   sub_3_75B0
        add    esp, 4
loc_3_6F54:
        cmp    dword ptr [esi + 0x24], 0
        je     loc_3_7015
        mov    dword ptr [esi + 0x24], 0
        cmp    dword ptr [esi + 0x50], 1
        jne    loc_3_6FDA
        mov    dword ptr [esi + 0x50], 0
        test   byte ptr [0x884], 2           ; -> obj1:0x0884
        jne    loc_3_6FDA
        or     byte ptr [0x884], 2           ; -> obj1:0x0884
        and    byte ptr [0x884], 0xfb        ; -> obj1:0x0884
        and    dword ptr [4], 0xffffffef     ; -> obj4:0x0004
        or     dword ptr [4], 0x40           ; -> obj4:0x0004
        call   sub_1_0340
        test   byte ptr [4], 0x20            ; -> obj4:0x0004
        jne    loc_3_6FC8
        or     dword ptr [4], 0x20           ; -> obj4:0x0004
        or     byte ptr [0x884], 8           ; -> obj1:0x0884
        push   esi
        push   0x20                          ; -> loc_4_0020
        call   sub_1_07F0
        add    esp, 8
        and    byte ptr [0x884], 0xf7        ; -> obj1:0x0884
loc_3_6FC8:
        test   byte ptr [0x884], 4           ; -> obj1:0x0884
        je     loc_3_6FDA
        and    byte ptr [0x884], 0xfb        ; -> obj1:0x0884
        pop    esi
        ret    
loc_3_6FDA:
        call   sub_1_0320
        push   esi
        call   sub_3_84F0
        add    esp, 4
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_3_700C
        push   eax
        mov    eax, dword ptr [esi + 0x40]
        push   eax
        call   sub_3_31B6
        add    esp, 8
        mov    dword ptr [esi + 4], 0
loc_3_700C:
        and    dword ptr [esi + 0x20], 0xffffdfff
        pop    esi
        ret    
loc_3_7015:
        mov    eax, dword ptr [0x9c80]       ; -> obj3:0x9C80
        cmp    dword ptr [esi + 0x14], eax
        jne    loc_3_7045
        mov    eax, dword ptr [0x9c7c]       ; -> obj3:0x9C7C
        cmp    dword ptr [esi + 0x48], eax
        jne    loc_3_7045
        test   byte ptr [esi + 0x22], 8
        je     loc_3_7037
        inc    dword ptr [0x9c7c]            ; -> obj3:0x9C7C
        pop    esi
        ret    
loc_3_7037:
        mov    eax, dword ptr [esi + 0xb4]
        mov    eax, dword ptr [eax]
        add    dword ptr [0x9c7c], eax       ; -> obj3:0x9C7C
loc_3_7045:
        pop    esi
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_7050:
        push   ebx
        push   esi
        push   edi
        test   byte ptr [0x884], 8           ; -> obj1:0x0884
        jne    loc_3_7083
        test   byte ptr [0x884], 1           ; -> obj1:0x0884
        jne    loc_3_714A
        test   byte ptr [0x9bf8], 2          ; -> loc_3_9BF8
        jne    loc_3_714A
        test   byte ptr [0x9cad], 0x20       ; -> loc_3_9CAD
        jne    loc_3_714A
loc_3_7083:
        push   0x9c8c                        ; -> obj3:0x9C8C
        call   sub_3_7EF0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_3_714A
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 4]
        or     ecx, ecx
        je     loc_3_714A
        sub    eax, eax
loc_3_70AA:
        cmp    byte ptr [ecx + 0x1d], al
        jne    loc_3_70CA
        mov    ebx, dword ptr [ecx + 0x20]
        and    ebx, 0xfffff000
        mov    edi, ebx
        sub    edi, dword ptr [esi + 0x68]
        mov    edx, dword ptr [esi + 4]
        shr    edi, 0xc
        cmp    word ptr [edx + edi*8 + 6], ax
        je     loc_3_70D3
loc_3_70CA:
        mov    ecx, dword ptr [ecx + 4]
        cmp    ecx, eax
        jne    loc_3_70AA
        jmp    loc_3_714A
loc_3_70D3:
        push   0
        push   1
        mov    eax, ebx
        shr    eax, 0xc
        push   eax
        call   sub_3_5150
        add    esp, 0xc
        or     eax, eax
        je     loc_3_714A
        push   ebx
        call   sub_3_51B0
        add    esp, 4
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ecx + edi*8], eax
        mov    eax, dword ptr [esi + 4]
        inc    word ptr [eax + edi*8 + 6]
        or     byte ptr [0x884], 4           ; -> obj1:0x0884
        test   byte ptr [0x884], 8           ; -> obj1:0x0884
        jne    loc_3_714A
        test   byte ptr [0x9cad], 0x20       ; -> loc_3_9CAD
        jne    loc_3_714A
        mov    eax, dword ptr [0x9ce8]       ; -> obj3:0x9CE8
        cmp    dword ptr [0x9cd4], eax       ; -> obj3:0x9CD4
        jae    loc_3_714A
        mov    eax, dword ptr [esi + 0x1c]
        cmp    dword ptr [esi + 0x2c], eax
        ja     loc_3_714A
        or     dword ptr [0x9cac], 0x2000    ; -> obj3:0x9CAC
        call   sub_1_0340
        push   0x9c8c                        ; -> obj3:0x9C8C
        call   sub_3_8580
        add    esp, 4
loc_3_714A:
        call   sub_1_0320
        and    dword ptr [4], 0xffffffbf     ; -> obj4:0x0004
        and    byte ptr [0x884], 0xfd        ; -> obj1:0x0884
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7170:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        or     eax, eax
        jne    loc_3_7186
        sub    eax, eax
        pop    esi
        ret    
loc_3_7186:
        mov    ecx, dword ptr [esi + 0x44]
        mov    ecx, dword ptr [ecx + 8]
        mov    edx, dword ptr [esi + 0x48]
        lea    esi, [ecx + edx]
        and    esi, dword ptr [eax + 0x6c]
        mov    eax, dword ptr [eax + esi*4 + 0x80]
        or     eax, eax
        je     loc_3_71B1
loc_3_71A0:
        cmp    dword ptr [eax + 0x10], edx
        jne    loc_3_71AA
        cmp    dword ptr [eax + 0xc], ecx
        je     loc_3_71B1
loc_3_71AA:
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    loc_3_71A0
loc_3_71B1:
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_71C0:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x24]
        add    eax, 0
loc_3_71D0:
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_3_7220
        cmp    dword ptr [esp + 0x28], 0
        jne    loc_3_7210
        mov    eax, dword ptr [esi + 0x20]
        and    eax, 0x80000
        push   eax
        call   sub_3_00B0
        add    esp, 4
        or     eax, eax
        je     loc_3_71D0
        mov    dword ptr [esi + 0x24], 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_7210:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_3_7220:
        cmp    dword ptr [esi + 0x50], 3
        jne    loc_3_7249
        mov    eax, edi
        sub    eax, dword ptr [esi + 0xb0]
        cmp    eax, -0x70
        je     loc_3_7249
        test   byte ptr [esi + 0x21], 1
        je     loc_3_7240
        or     dword ptr [esi + 0x20], 0x600
loc_3_7240:
        push   esi
        call   sub_3_59D8
        add    esp, 4
loc_3_7249:
        mov    eax, dword ptr [esi + 0x44]
        mov    ebp, dword ptr [eax + 8]
        mov    eax, dword ptr [esi + 0x48]
        mov    dword ptr [esp + 0x10], eax
        add    eax, ebp
        mov    dword ptr [esi + 0x4c], 0
        mov    dword ptr [esi + 0x54], 0
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [edi + 0x6c]
        and    eax, dword ptr [esp + 0x1c]
        mov    ebx, dword ptr [edi + eax*4 + 0x80]
        or     ebx, ebx
        je     loc_3_7463
loc_3_7280:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [ebx + 0x10], eax
        jne    loc_3_728E
        cmp    dword ptr [ebx + 0xc], ebp
        je     loc_3_7295
loc_3_728E:
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    loc_3_7280
loc_3_7295:
        or     ebx, ebx
        je     loc_3_7463
        test   byte ptr [esi + 0x21], 0x10
        je     loc_3_72B4
        mov    dword ptr [esi + 0x24], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_72B4:
        lea    eax, [ebx + 0x14]
        mov    dword ptr [esi + 0xb0], eax
        and    dword ptr [esi + 0x20], 0xfffffeff
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     loc_3_72E0
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_3_72E0:
        mov    ecx, dword ptr [ebx + 0x20]
        mov    eax, ecx
        and    eax, 0xfffff000
        mov    dword ptr [esp + 0x14], eax
        mov    ebp, eax
        sub    ebp, dword ptr [edi + 0x68]
        shr    ebp, 0xc
        mov    al, byte ptr [ebx + 0x1d]
        or     al, al
        mov    edx, dword ptr [edi + 4]
        jne    loc_3_7446
        lea    edx, [edx + ebp*8]
        cmp    word ptr [edx + 6], 0
        je     loc_3_7340
        inc    word ptr [edx + 4]
        inc    al
        mov    byte ptr [ebx + 0x1d], al
        mov    dword ptr [esi + 0x4c], ecx
        mov    al, byte ptr [ebx + 0x1e]
        or     al, al
        lea    ecx, [eax + 1]
        mov    byte ptr [ebx + 0x1e], cl
        jne    loc_3_7331
        push   ebx
        push   edi
        call   sub_3_7A20
        add    esp, 8
loc_3_7331:
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
loc_3_7340:
        cmp    dword ptr [esp + 0x28], 0
        push   esi
        je     loc_3_7360
        call   sub_3_59D8
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_3_7360:
        call   sub_3_59D8
        add    esp, 4
        or     dword ptr [esi + 0x20], 0x100
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     loc_3_7389
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_7389:
        push   0
        mov    eax, dword ptr [esp + 0x18]
        push   1
        shr    eax, 0xc
        push   eax
        call   sub_3_5150
        add    esp, 0xc
        or     eax, eax
        je     loc_3_7430
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_51B0
        add    esp, 4
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx + ebp*8], eax
        mov    eax, dword ptr [edi + 4]
        inc    word ptr [eax + ebp*8 + 6]
        mov    eax, dword ptr [edi + 4]
        push   esi
        inc    word ptr [eax + ebp*8 + 4]
        inc    byte ptr [ebx + 0x1d]
        inc    byte ptr [ebx + 0x1e]
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [esi + 0x4c], eax
        call   sub_3_59D8
        add    esp, 4
        and    dword ptr [esi + 0x20], 0xfffffeff
        or     dword ptr [esi + 0x20], 0x400
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        jne    loc_3_7415
        cmp    byte ptr [ebx + 0x1e], 1
        jne    loc_3_7408
        push   ebx
        push   edi
        call   sub_3_7A20
        add    esp, 8
loc_3_7408:
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_7415:
        dec    byte ptr [ebx + 0x1d]
        dec    byte ptr [ebx + 0x1e]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, 3
        dec    word ptr [ecx + ebp*8 + 4]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_7430:
        push   esi
        call   sub_3_59D8
        add    esp, 4
        call   sub_3_7F30
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_7446:
        inc    word ptr [edx + ebp*8 + 4]
        inc    al
        mov    byte ptr [ebx + 0x1d], al
        inc    byte ptr [ebx + 0x1e]
        mov    dword ptr [esi + 0x4c], ecx
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_7463:
        push   edi
        push   esi
        call   sub_3_79A0
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_7490
        add    edi, 0x70
        mov    dword ptr [esi + 0xb0], edi
        or     dword ptr [esi + 0x20], 0x40
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
        db  08Dh, 049h, 000h
loc_3_7490:
        lea    eax, [ebx + 0x14]
        mov    dword ptr [esi + 0xb0], eax
        or     dword ptr [esi + 0x20], 0x100
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     loc_3_74B4
        mov    ebp, 3
        jmp    loc_3_7522
loc_3_74B4:
        push   ebx
        push   edi
        call   sub_3_78A0
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x20]
        and    eax, 0xfffff000
        mov    dword ptr [esp + 0x14], eax
        sub    eax, dword ptr [edi + 0x68]
        shr    eax, 0xc
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 0x18]
        cmp    word ptr [eax + ecx*8 + 6], 0
        jne    loc_3_7557
        cmp    dword ptr [esp + 0x28], 0
        je     loc_3_74FA
        mov    ebp, 1
        push   esi
        call   sub_3_59D8
        add    esp, 4
        jmp    loc_3_7522
loc_3_74FA:
        push   0
        mov    eax, dword ptr [esp + 0x18]
        push   1
        shr    eax, 0xc
        push   eax
        call   sub_3_5150
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_7538
        push   esi
        call   sub_3_59D8
        add    esp, 4
        call   sub_3_7F30
        mov    ebp, eax
loc_3_7522:
        push   1
        push   ebx
        push   edi
        call   sub_3_7AB0
        add    esp, 0xc
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_7538:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_51B0
        add    esp, 4
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [edx + ecx*8], eax
        mov    eax, dword ptr [edi + 4]
        inc    word ptr [eax + ecx*8 + 6]
loc_3_7557:
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 0x18]
        inc    word ptr [eax + ecx*8 + 4]
        inc    byte ptr [ebx + 0x1d]
        inc    byte ptr [ebx + 0x1e]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x10], eax
        mov    dword ptr [ebx + 0xc], ebp
        mov    eax, dword ptr [edi + 0x6c]
        and    eax, dword ptr [esp + 0x1c]
        lea    eax, [edi + eax*4 + 0x80]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [eax], ebx
        mov    edx, dword ptr [ebx + 0x20]
        mov    dword ptr [esi + 0x4c], edx
        and    edx, 0xfff
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + ecx*8]
        add    eax, edx
        mov    dword ptr [esi + 0x54], eax
        mov    eax, 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_3_75B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x50]
        cmp    eax, 1
        je     loc_3_75FA
        cmp    eax, 2
        je     loc_3_75D0
        cmp    eax, 3
        je     loc_3_75F3
        cmp    eax, 4
        je     loc_3_75E0
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_3_75D0:
        mov    eax, dword ptr [esi + 0x4c]
        push   eax
        push   esi
        call   sub_3_7670
        add    esp, 8
        jmp    loc_3_75F3
        db  090h
loc_3_75E0:
        xor    eax, eax
        mov    al, byte ptr [esi + 0x63]
        push   eax
        mov    eax, dword ptr [esi + 0x4c]
        push   eax
        push   esi
        call   sub_3_7690
        add    esp, 0xc
loc_3_75F3:
        mov    dword ptr [esi + 0x4c], 0
loc_3_75FA:
        mov    dword ptr [esi + 0x50], 0
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7610:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_7EF0
        add    esp, 4
        mov    ecx, dword ptr [esp + 0xc]
        push   ecx
        push   eax
        call   sub_3_7F10
        add    esp, 8
        add    eax, 0x14
        mov    dword ptr [esi + 0xb0], eax
        push   esi
        call   sub_3_59D8
        add    esp, 4
        and    dword ptr [esi + 0x20], 0xfffffeff
        or     dword ptr [esi + 0x20], 0x400
        push   esi
        call   sub_3_5971
        add    esp, 4
        mov    eax, dword ptr [esp + 0xc]
        and    byte ptr [eax], 0xff
        mov    eax, 4
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7670:
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        push   1
        mov    esi, dword ptr [esp + 0xc]
        push   eax
        push   esi
        call   sub_3_7690
        add    esp, 0xc
        mov    dword ptr [esi + 0x4c], 0
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_3_7690:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        push   ebx
        call   sub_3_7EF0
        add    esp, 4
        mov    esi, eax
        mov    ebp, dword ptr [esp + 0x18]
        push   ebp
        push   esi
        call   sub_3_7F10
        add    esp, 8
        mov    edi, eax
        sub    ebp, dword ptr [esi + 0x68]
        dec    byte ptr [edi + 0x1d]
        dec    byte ptr [edi + 0x1e]
        mov    eax, dword ptr [esi + 4]
        shr    ebp, 0xc
        dec    word ptr [eax + ebp*8 + 4]
        lea    eax, [edi + 0x14]
        mov    dword ptr [ebx + 0xb0], eax
        push   ebx
        call   sub_3_59D8
        add    esp, 4
        cmp    byte ptr [edi + 0x1e], 0
        jne    loc_3_7703
        mov    cl, byte ptr [edi + 0x1f]
        test   cl, 1
        je     loc_3_76F4
        mov    eax, 1
        and    cl, 0xfe
        mov    byte ptr [edi + 0x1f], cl
        jmp    loc_3_76F8
loc_3_76F4:
        mov    eax, dword ptr [esp + 0x1c]
loc_3_76F8:
        push   eax
        push   edi
        push   esi
        call   sub_3_7AB0
        add    esp, 0xc
loc_3_7703:
        cmp    byte ptr [esi + 0x75], 0
        je     loc_3_770E
        call   sub_3_8690
loc_3_770E:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7720:
        mov    eax, dword ptr [0x9be4]       ; -> loc_3_9BE4
        inc    eax
        mov    dword ptr [0x9be4], eax       ; -> loc_3_9BE4
        ret    
        db  08Dh, 064h, 024h, 000h
sub_3_7730:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        or     esi, esi
        mov    ebp, dword ptr [esp + 0x14]
        je     loc_3_7789
loc_3_7742:
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 0x60]
        or     ebx, ebx
        je     loc_3_7789
loc_3_774B:
        cmp    dword ptr [edi + 0xc], ebp
        jne    loc_3_7781
        mov    cl, byte ptr [edi + 0x1e]
        or     cl, cl
        lea    eax, [ecx + 1]
        mov    byte ptr [edi + 0x1e], al
        jne    loc_3_7778
        push   edi
        push   esi
        call   sub_3_7A20
        add    esp, 8
        dec    byte ptr [edi + 0x1e]
        push   1
        push   edi
        push   esi
        call   sub_3_7AB0
        add    esp, 0xc
        jmp    loc_3_7781
loc_3_7778:
        dec    al
        mov    byte ptr [edi + 0x1e], al
        or     byte ptr [edi + 0x1f], 1
loc_3_7781:
        dec    ebx
        add    edi, 0x24
        or     ebx, ebx
        jne    loc_3_774B
loc_3_7789:
        cmp    dword ptr [0x9bd8], esi       ; -> obj3:0x9BD8
        jne    loc_3_779B
        mov    esi, dword ptr [0x9bdc]       ; -> obj3:0x9BDC
        or     esi, esi
        jne    loc_3_7742
loc_3_779B:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_77B0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [0x9bd0]       ; -> obj3:0x9BD0
        or     esi, esi
        mov    ebx, dword ptr [edi + 8]
        je     loc_3_77D8
loc_3_77C4:
        cmp    dword ptr [esi + 8], ebx
        jne    loc_3_77D2
        push   esi
        call   sub_3_7800
        add    esp, 4
loc_3_77D2:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_3_77C4
loc_3_77D8:
        or     byte ptr [edi + 0x1f], 1
        mov    eax, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        mov    ecx, dword ptr [edi + 0x104]
        inc    ecx
        shr    ecx, 1
        add    dword ptr [eax + 0x20], ecx
        push   edi
        call   sub_3_7840
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_7800:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    al, byte ptr [esi + 0x1f]
        and    al, 3
        cmp    al, 1
        jne    loc_3_7839
        mov    word ptr [esi + 0x20], 0
        mov    dword ptr [esi + 0x28], 0
        push   esi
        call   sub_3_7840
        add    esp, 4
        mov    eax, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        mov    ecx, dword ptr [esi + 0x104]
        inc    ecx
        shr    ecx, 1
        sub    dword ptr [eax + 0x20], ecx
        and    byte ptr [esi + 0x1f], 0xfe
loc_3_7839:
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_3_7840:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    al, byte ptr [edx + 0x1f]
        and    al, 3
        cmp    al, 1
        jne    loc_3_789C
        mov    eax, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0x12e]
        sub    dword ptr [eax + 0x24], ecx
        mov    eax, dword ptr [edx + 0x28]
        mov    word ptr [edx + 0x12e], ax
        mov    si, word ptr [edx + 0x12e]
        mov    eax, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        mov    ecx, esi
        and    ecx, 0xffff
        add    dword ptr [eax + 0x24], ecx
        mov    eax, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_3_789C
        inc    si
        mov    word ptr [edx + 0x12e], si
        mov    eax, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        inc    dword ptr [eax + 0x24]
loc_3_789C:
        pop    esi
        ret    
        db  08Bh, 0FFh
sub_3_78A0:
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 0x10]
        add    ecx, dword ptr [edx + 0xc]
        and    ecx, dword ptr [eax + 0x6c]
        lea    ecx, [eax + ecx*4 + 0x80]
        mov    dword ptr [edx + 0x10], 0
        mov    dword ptr [edx + 0xc], 0
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_3_78D9
loc_3_78CC:
        cmp    eax, edx
        je     loc_3_78DD
        lea    ecx, [eax + 8]
        mov    eax, dword ptr [ecx]
        or     eax, eax
        jne    loc_3_78CC
loc_3_78D9:
        cmp    eax, edx
        jne    loc_3_78E2
loc_3_78DD:
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx], eax
loc_3_78E2:
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_78F0:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        call   sub_3_7EF0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_3_7997
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 4]
        or     edi, edi
        je     loc_3_7938
loc_3_7913:
        cmp    byte ptr [edi + 0x1c], 4
        jne    loc_3_7931
        push   edi
        push   esi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [edi + 0x1c], 2
        push   edi
        push   esi
        call   sub_3_7D70
        add    esp, 8
loc_3_7931:
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        jne    loc_3_7913
loc_3_7938:
        mov    eax, dword ptr [esi + 0x1c]
        cmp    dword ptr [esi + 0x2c], eax
        jb     loc_3_7997
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 4]
        or     edi, edi
        je     loc_3_7997
        sub    ecx, ecx
        lea    esp, [esp]
loc_3_7950:
        cmp    byte ptr [edi + 0x1e], cl
        jne    loc_3_7960
        mov    al, byte ptr [edi + 0x1c]
        cmp    al, 1
        je     loc_3_7960
        cmp    al, 3
        jne    loc_3_7967
loc_3_7960:
        mov    edi, dword ptr [edi + 4]
        cmp    edi, ecx
        jne    loc_3_7950
loc_3_7967:
        or     edi, edi
        je     loc_3_7997
        push   edi
        push   esi
        call   sub_3_7A20
        add    esp, 8
        push   4
        push   edi
        push   esi
        call   sub_3_7AB0
        add    esp, 0xc
        push   edi
        push   esi
        call   sub_3_7A20
        add    esp, 8
        push   1
        push   edi
        push   esi
        call   sub_3_7AB0
        add    esp, 0xc
loc_3_7997:
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_79A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [esp + 0x14]
        lea    esp, [esp]
loc_3_79B0:
        mov    eax, dword ptr [esi + 8]
        sub    ebx, ebx
        mov    edi, dword ptr [eax + 4]
        cmp    edi, ebx
        je     loc_3_7A0C
        test   byte ptr [ebp + 0x6f], 4
        jne    loc_3_79C8
        cmp    dword ptr [esi + 0xc], 2
        jbe    loc_3_7A17
loc_3_79C8:
        mov    al, byte ptr [edi + 0x1c]
        cmp    al, 4
        je     loc_3_79D7
        cmp    al, 3
        je     loc_3_79D7
        cmp    al, 2
        jne    loc_3_79F4
loc_3_79D7:
        xor    ebx, ebx
        mov    bl, byte ptr [edi + 0x1e]
        push   edi
        push   esi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [edi + 0x1c], 1
        push   edi
        push   esi
        call   sub_3_7D70
        add    esp, 8
loc_3_79F4:
        inc    byte ptr [edi + 0x1e]
        push   edi
        push   esi
        call   sub_3_7A20
        add    esp, 8
        dec    byte ptr [edi + 0x1e]
        mov    byte ptr [edi + 0x1c], 1
        and    byte ptr [edi + 0x1f], 0xfb
loc_3_7A0C:
        or     ebx, ebx
        jne    loc_3_79B0
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_7A17:
        sub    edi, edi
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_7A20:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    al, byte ptr [edi + 0x1c]
        cmp    al, 4
        je     loc_3_7A8A
        cmp    al, 2
        je     loc_3_7A8A
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x30], edi
        jne    loc_3_7A3F
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x30], eax
loc_3_7A3F:
        cmp    dword ptr [esi + 0x34], edi
        jne    loc_3_7A49
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x34], eax
loc_3_7A49:
        cmp    dword ptr [esi + 0x38], edi
        jne    loc_3_7A53
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x38], eax
loc_3_7A53:
        cmp    dword ptr [esi + 0x3c], edi
        jne    loc_3_7A5D
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x3c], eax
loc_3_7A5D:
        mov    ecx, dword ptr [edi + 4]
        or     ecx, ecx
        je     loc_3_7A68
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
loc_3_7A68:
        mov    eax, dword ptr [edi]
        push   edi
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], 0
        dec    dword ptr [esi + 0xc]
        push   esi
        call   sub_3_7D10
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_3_7A8A:
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   esi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [edi + 0x1c], 3
        push   edi
        push   esi
        call   sub_3_7D70
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7AB0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        test   byte ptr [esp + 0x1f], 0x80
        je     loc_3_7AD0
        and    dword ptr [esp + 0x1c], 0x7fffffff
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x18]
        jmp    loc_3_7C8B
loc_3_7AD0:
        cmp    dword ptr [esp + 0x1c], 5
        jne    loc_3_7B40
        mov    esi, dword ptr [esp + 0x18]
        cmp    byte ptr [esi + 0x1c], 3
        jne    loc_3_7B09
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [edi + 0x50]
        cmp    eax, dword ptr [edi + 0x58]
        ja     loc_3_7AFF
        mov    eax, dword ptr [esi + 0x10]
        sub    eax, dword ptr [edi + 0x54]
        cmp    eax, dword ptr [edi + 0x5c]
        jbe    loc_3_7B0D
loc_3_7AFF:
        mov    dword ptr [esp + 0x1c], 2
        jmp    loc_3_7B0D
loc_3_7B09:
        mov    edi, dword ptr [esp + 0x14]
loc_3_7B0D:
        mov    cl, byte ptr [esi + 0x1f]
        test   cl, 4
        je     loc_3_7B2D
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 0x48], eax
        mov    eax, dword ptr [0x9c74]       ; -> loc_3_9C74
        mov    eax, dword ptr [eax + 0x1c]
        add    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edi + 0x4c], eax
loc_3_7B2D:
        mov    eax, dword ptr [esi + 0x10]
        cmp    dword ptr [edi + 0x54], eax
        jne    loc_3_7B48
        or     cl, 4
        mov    byte ptr [esi + 0x1f], cl
        jmp    loc_3_7B48
        db  08Dh, 049h, 000h
loc_3_7B40:
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x18]
loc_3_7B48:
        cmp    dword ptr [esi], 0
        je     loc_3_7B81
        mov    al, byte ptr [esi + 0x1c]
        cmp    al, 4
        je     loc_3_7B58
        cmp    al, 3
        jne    loc_3_7B81
loc_3_7B58:
        cmp    dword ptr [esp + 0x1c], 2
        je     loc_3_7B81
        push   esi
        push   edi
        call   sub_3_7D10
        add    esp, 8
        mov    byte ptr [esi + 0x1c], 1
        push   esi
        push   edi
        call   sub_3_7D70
        add    esp, 8
        push   esi
        push   edi
        call   sub_3_7A20
        add    esp, 8
loc_3_7B81:
        mov    eax, dword ptr [edi + 0x10]
        cmp    dword ptr [edi + 0x20], eax
        jae    loc_3_7BBB
        mov    ebx, 6
        mov    edi, edi
loc_3_7B90:
        mov    eax, dword ptr [edi + 0x34]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edi + 0x34], eax
        push   eax
        push   edi
        call   sub_3_7D10
        add    esp, 8
        mov    eax, dword ptr [edi + 0x34]
        push   eax
        mov    byte ptr [eax + 0x1c], bl
        push   edi
        call   sub_3_7D70
        add    esp, 8
        mov    eax, dword ptr [edi + 0x10]
        cmp    dword ptr [edi + 0x20], eax
        jb     loc_3_7B90
loc_3_7BBB:
        mov    eax, dword ptr [edi + 0x14]
        cmp    dword ptr [edi + 0x24], eax
        jae    loc_3_7BF3
        mov    ebx, 5
loc_3_7BC8:
        mov    eax, dword ptr [edi + 0x38]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edi + 0x38], eax
        push   eax
        push   edi
        call   sub_3_7D10
        add    esp, 8
        mov    eax, dword ptr [edi + 0x38]
        push   eax
        mov    byte ptr [eax + 0x1c], bl
        push   edi
        call   sub_3_7D70
        add    esp, 8
        mov    eax, dword ptr [edi + 0x14]
        cmp    dword ptr [edi + 0x24], eax
        jb     loc_3_7BC8
loc_3_7BF3:
        cmp    dword ptr [esp + 0x1c], 4
        jne    loc_3_7C7A
        mov    ebx, dword ptr [edi + 0xc]
        sub    ebx, dword ptr [edi + 0x14]
        sub    ebx, dword ptr [edi + 0x10]
        mov    eax, dword ptr [edi + 0x28]
        cmp    ebx, eax
        jbe    loc_3_7C10
        mov    ebx, eax
loc_3_7C10:
        mov    eax, dword ptr [edi + 0x18]
        sub    eax, dword ptr [edi + 0xc]
        add    eax, dword ptr [edi + 0x1c]
        add    eax, dword ptr [edi + 0x60]
        cmp    eax, ebx
        jae    loc_3_7C7A
loc_3_7C20:
        mov    eax, dword ptr [edi + 0x3c]
        cmp    dword ptr [edi + 0x38], eax
        je     loc_3_7C7A
        mov    ebp, dword ptr [eax + 4]
        push   ebp
        push   edi
        call   sub_3_7A20
        add    esp, 8
        push   0x80000001
        push   ebp
        push   edi
        call   sub_3_7AB0
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x20]
        sub    eax, dword ptr [edi + 0x68]
        and    eax, 0xfffff1ff
        shr    eax, 9
        mov    ecx, dword ptr [edi + 4]
        cmp    word ptr [eax + ecx + 6], 0
        mov    eax, dword ptr [0x9bf8]       ; -> loc_3_9BF8
        jne    loc_3_7C6A
        and    eax, 0xfffffffe
        mov    dword ptr [0x9bf8], eax       ; -> loc_3_9BF8
loc_3_7C6A:
        mov    eax, dword ptr [edi + 0x18]
        sub    eax, dword ptr [edi + 0xc]
        add    eax, dword ptr [edi + 0x1c]
        add    eax, dword ptr [edi + 0x60]
        cmp    eax, ebx
        jb     loc_3_7C20
loc_3_7C7A:
        cmp    dword ptr [esp + 0x1c], 1
        jne    loc_3_7C8B
        push   esi
        push   edi
        call   sub_3_78A0
        add    esp, 8
loc_3_7C8B:
        mov    al, byte ptr [esp + 0x1c]
        mov    byte ptr [esi + 0x1c], al
        push   esi
        push   edi
        call   sub_3_7D70
        add    esp, 8
        or     eax, eax
        je     loc_3_7CEF
        inc    dword ptr [edi + 0xc]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    eax, dword ptr [ecx + 4]
        or     eax, eax
        mov    dword ptr [esi + 4], eax
        je     loc_3_7CB3
        mov    dword ptr [eax], esi
loc_3_7CB3:
        mov    dword ptr [ecx + 4], esi
        xor    eax, eax
        mov    al, byte ptr [esi + 0x1c]
        cmp    eax, 4
        je     loc_3_7CD4
        cmp    eax, 5
        je     loc_3_7CDC
        cmp    eax, 6
        je     loc_3_7CE4
        cmp    eax, 7
        je     loc_3_7CEC
        cmp    dword ptr [edi + 0x3c], ecx
        jne    loc_3_7CD7
loc_3_7CD4:
        mov    dword ptr [edi + 0x3c], esi
loc_3_7CD7:
        cmp    dword ptr [edi + 0x38], ecx
        jne    loc_3_7CDF
loc_3_7CDC:
        mov    dword ptr [edi + 0x38], esi
loc_3_7CDF:
        cmp    dword ptr [edi + 0x34], ecx
        jne    loc_3_7CE7
loc_3_7CE4:
        mov    dword ptr [edi + 0x34], esi
loc_3_7CE7:
        cmp    dword ptr [edi + 0x30], ecx
        jne    loc_3_7CEF
loc_3_7CEC:
        mov    dword ptr [edi + 0x30], esi
loc_3_7CEF:
        cmp    byte ptr [esi + 0x1c], 5
        jne    loc_3_7D02
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x10]
        sub    eax, dword ptr [esi + 0x10]
        cmp    eax, -1
        jne    loc_3_7D06
loc_3_7D02:
        and    byte ptr [esi + 0x1f], 0xfb
loc_3_7D06:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_3_7D10:
        mov    eax, dword ptr [esp + 8]
        mov    al, byte ptr [eax + 0x1c]
        and    eax, 0xff
        cmp    eax, 1
        jl     loc_3_7D6F
        cmp    eax, 2
        jle    loc_3_7D36
        cmp    eax, 4
        je     loc_3_7D40
        cmp    eax, 6
        je     loc_3_7D60
        cmp    eax, 7
        je     loc_3_7D68
        ret    
loc_3_7D36:
        mov    ecx, dword ptr [esp + 4]
        dec    dword ptr [ecx + 0x1c]
        ret    
        db  08Bh, 0FFh
loc_3_7D40:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x18]
        dec    eax
        mov    dword ptr [ecx + 0x18], eax
        test   byte ptr [ecx + 0x42], 1
        je     loc_3_7D6F
        cmp    dword ptr [ecx + 0x7c], eax
        jb     loc_3_7D6F
        push   ecx
        call   sub_3_7E10
        add    esp, 4
        ret    
loc_3_7D60:
        mov    ecx, dword ptr [esp + 4]
        dec    dword ptr [ecx + 0x14]
        ret    
loc_3_7D68:
        mov    ecx, dword ptr [esp + 4]
        dec    dword ptr [ecx + 0x10]
loc_3_7D6F:
        ret    
sub_3_7D70:
        mov    ecx, dword ptr [esp + 8]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 0x1c]
        and    ecx, 0xff
        dec    ecx
        cmp    ecx, 6
        ja     loc_3_7DE7
        jmp    dword ptr [ecx*4 + 0x7de8]    ; -> obj3:0x7DE8
        db  08Dh, 064h, 024h, 000h
loc_3_7D90:
        mov    ecx, dword ptr [esp + 4]
        lea    eax, [ecx + 8]
        inc    dword ptr [ecx + 0x1c]
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_7DA0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x18]
        inc    eax
        mov    dword ptr [ecx + 0x18], eax
        cmp    dword ptr [ecx + 0x78], eax
        ja     loc_3_7DB4
        or     byte ptr [ecx + 0x42], 1
loc_3_7DB4:
        lea    eax, [ecx + 0x3c]
        ret    
loc_3_7DB8:
        mov    eax, dword ptr [esp + 4]
        add    eax, 0x38
        ret    
loc_3_7DC0:
        mov    ecx, dword ptr [esp + 4]
        inc    dword ptr [ecx + 0x14]
        lea    eax, [ecx + 0x34]
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_7DD0:
        mov    ecx, dword ptr [esp + 4]
        inc    dword ptr [ecx + 0x10]
        lea    eax, [ecx + 0x30]
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_7DE0:
        mov    ecx, dword ptr [esp + 4]
        inc    dword ptr [ecx + 0x1c]
loc_3_7DE7:
        ret    
        ; ---- dispatch table (7 entries) ----
        dd  loc_3_7D90
        dd  loc_3_7DE0
        dd  loc_3_7DE7
        dd  loc_3_7DA0
        dd  loc_3_7DB8
        dd  loc_3_7DC0
        dd  loc_3_7DD0
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7E10:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        xor    eax, eax
        mov    dl, byte ptr [ecx + 0x42]
        and    dl, 0xfe
        mov    byte ptr [ecx + 0x42], dl
        mov    al, byte ptr [ecx + 0x43]
        cmp    eax, 5
        ja     loc_3_7EC6
        jmp    dword ptr [eax*4 + 0x7ed4]    ; -> obj3:0x7ED4
loc_3_7E33:
        mov    byte ptr [ecx + 0x43], 1
        mov    esi, dword ptr [0x9c74]       ; -> loc_3_9C74
        mov    eax, dword ptr [esi + 0x34]
        add    dword ptr [ecx + 0x78], eax
        mov    eax, dword ptr [ecx + 0x78]
        dec    eax
        mov    dword ptr [ecx + 0x7c], eax
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_7E50:
        mov    byte ptr [ecx + 0x43], 2
        mov    esi, dword ptr [0x9c74]       ; -> loc_3_9C74
        mov    eax, dword ptr [esi + 0x38]
        add    dword ptr [ecx + 0x78], eax
        mov    eax, dword ptr [ecx + 0x78]
        dec    eax
        mov    dword ptr [ecx + 0x7c], eax
        pop    esi
        ret    
loc_3_7E69:
        mov    eax, dword ptr [ecx + 0x78]
        cmp    dword ptr [ecx + 0x28], eax
        jb     loc_3_7EC6
        mov    esi, dword ptr [0x9c74]       ; -> loc_3_9C74
        mov    edx, dword ptr [esi + 0x40]
        add    edx, eax
        mov    dword ptr [ecx + 0x78], edx
        mov    eax, dword ptr [ecx + 0x78]
        dec    eax
        mov    dword ptr [ecx + 0x7c], eax
        pop    esi
        ret    
loc_3_7E88:
        mov    byte ptr [ecx + 0x43], 0
        mov    esi, dword ptr [0x9c74]       ; -> loc_3_9C74
        or     dl, 1
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [ecx + 0x78], eax
        mov    byte ptr [ecx + 0x42], dl
        mov    eax, dword ptr [ecx + 0x78]
        dec    eax
        mov    dword ptr [ecx + 0x7c], eax
        pop    esi
        ret    
loc_3_7EA7:
        mov    byte ptr [ecx + 0x43], 2
        mov    esi, dword ptr [0x9c74]       ; -> loc_3_9C74
        or     dl, 1
        mov    eax, dword ptr [esi + 0x3c]
        mov    dword ptr [ecx + 0x78], eax
        mov    byte ptr [ecx + 0x42], dl
        mov    eax, dword ptr [ecx + 0x78]
        dec    eax
        mov    dword ptr [ecx + 0x7c], eax
        pop    esi
        ret    
loc_3_7EC6:
        mov    byte ptr [ecx + 0x43], 3
loc_3_7ECA:
        mov    eax, dword ptr [ecx + 0x78]
        dec    eax
        mov    dword ptr [ecx + 0x7c], eax
        pop    esi
        ret    
        db  090h
        ; ---- dispatch table (6 entries) ----
        dd  loc_3_7E33
        dd  loc_3_7E50
        dd  loc_3_7E69
        dd  loc_3_7ECA
        dd  loc_3_7E88
        dd  loc_3_7EA7
        db  08Dh, 064h, 024h, 000h
sub_3_7EF0:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 0x22], 8
        je     loc_3_7F00
        mov    eax, dword ptr [0x9bdc]       ; -> obj3:0x9BDC
        ret    
loc_3_7F00:
        mov    eax, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_7F10:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        xor    esi, esi
        sub    eax, dword ptr [ecx + 0x68]
        mov    si, word ptr [ecx + 0x40]
        sub    edx, edx
        div    esi
        shl    eax, 2
        pop    esi
        lea    eax, [eax + eax*8]
        add    eax, dword ptr [ecx]
        ret    
sub_3_7F30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [0x9bf8]       ; -> loc_3_9BF8
        or     ecx, 2
        mov    dword ptr [0x9bf8], ecx       ; -> loc_3_9BF8
        mov    ebx, dword ptr [0x9bd8]       ; -> obj3:0x9BD8
        mov    ecx, ebx
        sub    eax, eax
        mov    edi, dword ptr [ecx + 4]
        mov    esi, ebx
        add    esi, 0x64
        sub    ebp, ebp
        cmp    dword ptr [esi], eax
        jbe    loc_3_7FA3
loc_3_7F5B:
        cmp    word ptr [edi + 4], 0
        jne    loc_3_7F9B
loc_3_7F62:
        cmp    word ptr [edi + 6], 0
        je     loc_3_7F9B
        dec    word ptr [edi + 6]
        push   0
        push   1
        mov    eax, dword ptr [ebx + 0x68]
        shr    eax, 0xc
        add    eax, ebp
        push   eax
        call   sub_3_5180
        add    esp, 0xc
        cmp    word ptr [edi + 6], 0
        jne    loc_3_7F94
        mov    dword ptr [edi], 0
        mov    eax, 3
loc_3_7F94:
        cmp    word ptr [edi + 4], 0
        je     loc_3_7F62
loc_3_7F9B:
        inc    ebp
        add    edi, 8
        cmp    dword ptr [esi], ebp
        ja     loc_3_7F5B
loc_3_7FA3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_7FB0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esi]
        cmp    dword ptr [esi + 0x24], 0
        je     loc_3_7FD9
        test   byte ptr [esi + 0x21], 8
        je     loc_3_8226
        mov    dword ptr [esi + 0x24], 5
        jmp    loc_3_8226
loc_3_7FD9:
        lea    eax, [esi + 0x20]
        test   byte ptr [esi + 0x22], 0x20
        jne    loc_3_8021
        mov    eax, dword ptr [esi + 0x14]
        or     dword ptr [esi + 0x20], 0x200100
        inc    word ptr [eax + 0x1c]
        add    eax, 0x14
        mov    dword ptr [esi + 0xb0], eax
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     loc_3_8021
        mov    ax, word ptr [esi + 0x2c]
        mov    word ptr [esi + 0x2e], ax
        push   0x46
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_8021:
        sub    ebp, ebp
        cmp    dword ptr [esi + 0x18], ebp
        jne    loc_3_8031
        push   esi
        call   sub_3_4560
        add    esp, 4
loc_3_8031:
        cmp    dword ptr [esi + 0x18], 0
        jne    loc_3_8056
        push   esi
        call   sub_3_43D0
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_8226
        mov    ebp, 1
        or     dword ptr [esi + 0x20], 0x800000
loc_3_8056:
        test   byte ptr [esi + 0x22], 0x40
        jne    loc_3_80A7
        mov    eax, dword ptr [esi + 0x18]
        or     dword ptr [esi + 0x20], 0x400100
        mov    ax, word ptr [eax + 0x10]
        mov    ecx, dword ptr [esi + 0x18]
        inc    ax
        mov    word ptr [ecx + 0x10], ax
        mov    eax, dword ptr [esi + 0x18]
        add    eax, 8
        mov    dword ptr [esi + 0xb0], eax
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     loc_3_80A7
        mov    ax, word ptr [esi + 0x2c]
        mov    word ptr [esi + 0x2e], ax
        push   0x46
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_80A7:
        mov    ebx, dword ptr [esi + 0x18]
        test   byte ptr [esi + 0x23], 0x10
        je     loc_3_80BB
        and    dword ptr [esi + 0x20], 0xefffffff
        dec    word ptr [ebx + 0x10]
loc_3_80BB:
        test   byte ptr [ebx + 0x13], 4
        je     loc_3_819C
        or     ebp, ebp
        jne    loc_3_8159
        push   0
        push   0x14
        call   sub_3_43B0
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_3_80F0
        mov    dword ptr [esi + 0x24], 8
        jmp    loc_3_822A
        db  005h, 000h, 000h, 000h, 000h
loc_3_80F0:
        mov    dword ptr [ebp], 0
        mov    dword ptr [ebp + 4], 0
        mov    eax, dword ptr [esi + 0x90]
        push   0
        mov    eax, dword ptr [eax + 2]
        mov    dword ptr [ebp + 8], eax
        push   esi
        call   sub_3_9620
        add    esp, 4
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0xc
        mov    dword ptr [ebp + 0xc], eax
        mov    ecx, dword ptr [esi + 0x90]
        push   ebp
        mov    al, byte ptr [ecx + 0x1a]
        mov    byte ptr [ebp + 0x12], al
        mov    al, byte ptr [ecx + 0x1b]
        mov    byte ptr [ebp + 0x13], al
        mov    ax, word ptr [ecx + 0x1c]
        mov    word ptr [ebp + 0x10], ax
        push   esi
        call   sub_3_96B0
        add    esp, 8
        lea    eax, [ebx + 0x28]
        cmp    dword ptr [eax], 0
        je     loc_3_8157
        mov    edi, edi
loc_3_8150:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    loc_3_8150
loc_3_8157:
        mov    dword ptr [eax], ebp
loc_3_8159:
        test   byte ptr [esi + 0x23], 0x20
        je     loc_3_8196
        mov    byte ptr [esi + 0x6d], 0x24
        mov    eax, dword ptr [esi + 0x84]
        inc    eax
        mov    dword ptr [esi + 0x80], eax
        mov    ax, word ptr [esi + 0x2c]
        mov    word ptr [esi + 0x2e], ax
        push   0x24
        push   esi
        call   sub_3_8380
        add    esp, 8
        push   0x20
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_8196:
        and    word ptr [ebx + 0x12], 0xfbff
loc_3_819C:
        push   esi
        call   sub_3_45C0
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_822A
        test   byte ptr [esi + 0x22], 0x80
        je     loc_3_81BE
        push   esi
        call   sub_3_4520
        add    esp, 4
loc_3_81BE:
        push   esi
        call   sub_3_4620
        add    esp, 4
        mov    byte ptr [edi + 4], 0
        mov    word ptr [edi + 0x18], 1
        mov    dword ptr [edi], 1
        lea    eax, [ebx + 0x28]
        mov    dword ptr [edi + 0x24], 0
        or     eax, eax
        je     loc_3_81F1
loc_3_81E5:
        mov    ecx, dword ptr [eax + 0xc]
        add    dword ptr [edi + 0x24], ecx
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_3_81E5
loc_3_81F1:
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [edi + 0x20], eax
        lea    eax, [ebx + 0x1c]
        push   eax
        call   sub_3_4850
        add    esp, 4
        mov    dword ptr [edi + 0x28], eax
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [eax], 0x3b50       ; -> loc_3_3B50
        mov    dword ptr [eax + 4], 0x3c40   ; -> sub_3_3C40
        mov    dword ptr [eax + 8], 0x8fc    ; -> obj1:0x08FC
        mov    dword ptr [esi + 0x24], 0
        jmp    loc_3_822A
loc_3_8226:
        mov    ebx, dword ptr [esp + 0x10]
loc_3_822A:
        test   byte ptr [esi + 0x22], 0x20
        je     loc_3_824E
        mov    eax, dword ptr [esi + 0x14]
        add    eax, 0x14
        mov    dword ptr [esi + 0xb0], eax
        push   esi
        call   sub_3_59D8
        add    esp, 4
        push   esi
        call   sub_3_4660
        add    esp, 4
loc_3_824E:
        test   byte ptr [esi + 0x22], 0x40
        je     loc_3_8272
        mov    eax, dword ptr [esi + 0x18]
        add    eax, 8
        mov    dword ptr [esi + 0xb0], eax
        push   esi
        call   sub_3_59D8
        add    esp, 4
        push   esi
        call   sub_3_4730
        add    esp, 4
loc_3_8272:
        and    dword ptr [esi + 0x20], 0xff9fffff
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_8360
        mov    eax, dword ptr [esi + 0x18]
        mov    ax, word ptr [eax + 0x12]
        and    ax, 0x4040
        cmp    ax, 0x40
        jne    loc_3_82F6
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   0
        push   0x930
        call   sub_3_43B0
        add    esp, 8
        mov    dword ptr [esi + 0x4c], eax
        cmp    dword ptr [esi + 0x4c], 0
        jne    loc_3_82BC
        mov    dword ptr [esi + 0x24], 8
loc_3_82BC:
        mov    eax, dword ptr [esi + 0x18]
        mov    eax, dword ptr [eax + 0x30]
        mov    dword ptr [esi + 0x48], eax
        mov    dword ptr [esi + 0x50], 2
        or     dword ptr [esi + 0x20], 0x80c4000
        mov    word ptr [esi + 0x2e], 0x4c
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_836B
        push   6
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_82F6:
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_8360
        cmp    dword ptr [edi + 0x24], 0
        je     loc_3_8360
        test   word ptr [ebx + 0x12], 0x5ff0
        jne    loc_3_8360
        test   byte ptr [0x9cad], 0x20       ; -> loc_3_9CAD
        jne    loc_3_8360
        mov    eax, dword ptr [0x9c1c]       ; -> loc_3_9C1C
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [0x9c78]       ; -> obj3:0x9C78
        cmp    eax, 0x3e8
        jbe    loc_3_8360
        push   esi
        call   sub_3_75B0
        add    esp, 4
        mov    byte ptr [esi + 0x63], 5
        mov    edx, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [ebx + 0x30]
        mov    eax, dword ptr [edx + 0xc]
        push   0xa
        mov    eax, dword ptr [eax + 0xc]
        sub    al, byte ptr [edx + 0x44]
        mov    cl, al
        mov    word ptr [esi + 0x2e], 1
        shr    ebx, cl
        mov    dword ptr [esi + 0x48], ebx
        push   esi
        call   sub_3_8380
        add    esp, 8
        jmp    loc_3_8364
        db  08Dh, 049h, 000h
loc_3_8360:
        or     dword ptr [esi + 0x20], 0x10
loc_3_8364:
        mov    eax, dword ptr [esi + 0x24]
        mov    word ptr [edi + 0x1a], ax
loc_3_836B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8380:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ax, word ptr [esi + 0x30]
        mov    cx, word ptr [esi + 0x2e]
        mov    edx, eax
        and    edx, 0xffff
        mov    word ptr [esi + edx*2 + 0x32], cx
        inc    ax
        mov    word ptr [esi + 0x30], ax
        mov    ax, word ptr [esp + 0xc]
        mov    word ptr [esi + 0x2e], ax
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_3_83B0:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x30]
        dec    ax
        mov    word ptr [ecx + 0x30], ax
        and    eax, 0xffff
        mov    ax, word ptr [ecx + eax*2 + 0x32]
        mov    word ptr [ecx + 0x2e], ax
        ret    
        db  08Dh, 049h, 000h
sub_3_83D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
loc_3_83D5:
        mov    ax, word ptr [esi + 0x2e]
        mov    word ptr [esi + 0x2c], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 0x2c]
        mov    ax, word ptr [eax*8 + 0xa0ae] ; -> obj3:0xA0AE
        and    dword ptr [esi + 0x20], 0x7fffffff
        mov    word ptr [esi + 0x2e], ax
        xor    eax, eax
        push   esi
        mov    ax, word ptr [esi + 0x2c]
        call   dword ptr [eax*8 + 0xa0b0]    ; -> obj3:0xA0B0
        add    esp, 4
        test   byte ptr [esi + 0x20], 1
        je     loc_3_8432
        or     dword ptr [esi + 0x20], 0x40000000
        test   byte ptr [esi + 0x23], 0x80
        jne    loc_3_842B
loc_3_841A:
        push   1
        push   esi
        call   sub_1_0063
        add    esp, 8
        test   byte ptr [esi + 0x23], 0x80
        je     loc_3_841A
loc_3_842B:
        and    dword ptr [esi + 0x20], 0x3ffffffc
loc_3_8432:
        test   byte ptr [esi + 0x20], 0x10
        je     loc_3_83D5
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_8440:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        xor    eax, eax
        or     dword ptr [esi + 0x20], 8
        mov    ax, word ptr [esi + 0x2e]
        test   byte ptr [eax*8 + 0xa0ad], 0x80 ; -> obj3:0xA0AD
        je     loc_3_84A1
        mov    edi, 0x8000
        nop    
loc_3_8460:
        test   byte ptr [esi + 0x20], 0x16
        jne    loc_3_84A1
        mov    ax, word ptr [esi + 0x2e]
        mov    word ptr [esi + 0x2c], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 0x2c]
        mov    ax, word ptr [eax*8 + 0xa0ae] ; -> obj3:0xA0AE
        mov    word ptr [esi + 0x2e], ax
        push   esi
        xor    eax, eax
        mov    ax, word ptr [esi + 0x2c]
        call   dword ptr [eax*8 + 0xa0b0]    ; -> obj3:0xA0B0
        add    esp, 4
        xor    eax, eax
        mov    ax, word ptr [esi + 0x2e]
        test   word ptr [eax*8 + 0xa0ac], di ; -> loc_3_A0AC
        jne    loc_3_8460
loc_3_84A1:
        test   byte ptr [esi + 0x20], 4
        je     loc_3_84B2
        and    dword ptr [esi + 0x20], 0xfffffffb
        and    dword ptr [esi + 0x20], 0xfffffff7
        pop    edi
        pop    esi
        ret    
loc_3_84B2:
        test   byte ptr [esi + 0x21], 0x10
        jne    loc_3_84E0
        or     dword ptr [esi + 0x20], 0x80000000
        test   byte ptr [esi + 0x23], 0x40
        je     loc_3_84E4
        push   esi
        call   sub_1_0069
        add    esp, 4
        and    dword ptr [esi + 0x20], 0xffffffdf
        and    dword ptr [esi + 0x20], 0xfffffff7
        pop    edi
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_84E0:
        and    dword ptr [esi + 0x20], 0xfffffffc
loc_3_84E4:
        and    dword ptr [esi + 0x20], 0xffffffdf
        and    dword ptr [esi + 0x20], 0xfffffff7
        pop    edi
        pop    esi
        ret    
        db  090h
sub_3_84F0:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 0x20], 0x21
        test   byte ptr [eax + 0x20], 8
        je     loc_3_8502
        or     dword ptr [eax + 0x20], 4
loc_3_8502:
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8510:
        sub    esp, 4
        cli    
        mov    eax, dword ptr [0x9bc0]       ; -> obj3:0x9BC0
        or     eax, eax
        je     loc_3_8536
        lea    ecx, [0x9bc0]                 ; -> obj3:0x9BC0
loc_3_8523:
        cmp    dword ptr [eax + 0xc], 0
        je     loc_3_8530
        lea    ecx, [eax + 0xc]
        mov    eax, dword ptr [ecx]
        jmp    loc_3_8523
loc_3_8530:
        mov    dword ptr [ecx], 0
loc_3_8536:
        mov    dword ptr [esp], eax
        sti    
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_8550:
        mov    dword ptr [0x9bd4], 0         ; -> obj3:0x9BD4
        lea    ebx, [ebx]
loc_3_8560:
        call   sub_3_8510
        or     eax, eax
        je     loc_3_8574
        push   eax
        call   sub_3_8440
        add    esp, 4
        jmp    loc_3_8560
loc_3_8574:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8580:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    ecx, dword ptr [0x9bc0]       ; -> obj3:0x9BC0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [0x9bc0], eax       ; -> obj3:0x9BC0
        cmp    dword ptr [0x9bd4], 0         ; -> obj3:0x9BD4
        jne    loc_3_85B9
        push   0
        push   0x8550                        ; -> loc_3_8550
        call   sub_3_8730
        add    esp, 8
        mov    dword ptr [0x9bd4], eax       ; -> obj3:0x9BD4
loc_3_85B9:
        push   dword ptr [ebp - 4]
        popfd  
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_85D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 0x20], 0x40
        je     loc_3_85F0
        mov    eax, dword ptr [esi + 0xb0]
        push   esi
        inc    byte ptr [eax + 5]
        call   sub_3_8670
        add    esp, 4
        pop    esi
        ret    
        db  090h
loc_3_85F0:
        push   esi
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     loc_3_8620
        test   byte ptr [esi + 0x21], 1
        mov    eax, dword ptr [esi + 0xb0]
        je     loc_3_8610
        inc    byte ptr [eax + 4]
        jmp    loc_3_8613
        db  08Bh, 0FFh
loc_3_8610:
        inc    byte ptr [eax + 5]
loc_3_8613:
        push   esi
        call   sub_3_8670
        add    esp, 4
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_3_8620:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_8630:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 0x20], 0x40
        je     loc_3_8651
        mov    ecx, dword ptr [eax + 0xb0]
        and    dword ptr [eax + 0x20], 0xffffffbf
        dec    byte ptr [ecx + 5]
        push   eax
        call   sub_3_83B0
        add    esp, 4
        ret    
loc_3_8651:
        test   byte ptr [eax + 0x21], 1
        mov    eax, dword ptr [eax + 0xb0]
        je     loc_3_8661
        dec    byte ptr [eax + 4]
        ret    
loc_3_8661:
        dec    byte ptr [eax + 5]
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8670:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x9bc4]       ; -> obj3:0x9BC4
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [0x9bc4], ecx       ; -> obj3:0x9BC4
        push   ecx
        call   sub_3_84F0
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h
sub_3_8690:
        push   esi
        mov    edx, dword ptr [0x9bc4]       ; -> obj3:0x9BC4
        mov    dword ptr [0x9bc4], 0         ; -> obj3:0x9BC4
        sub    ecx, ecx
        cmp    edx, ecx
        je     loc_3_86B5
loc_3_86A7:
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [edx + 0xc], ecx
        mov    ecx, edx
        mov    edx, eax
        or     eax, eax
        jne    loc_3_86A7
loc_3_86B5:
        or     ecx, ecx
        je     loc_3_86CB
loc_3_86B9:
        mov    esi, dword ptr [ecx + 0xc]
        push   ecx
        call   sub_3_8580
        add    esp, 4
        mov    ecx, esi
        or     esi, esi
        jne    loc_3_86B9
loc_3_86CB:
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_3_86D0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_3_86E0
        xor    eax, eax
loc_3_86E0:
        nop    
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_86F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        pop    ebp
        ret    
        db  08Bh, 0FFh
sub_3_8700:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Signal_Semaphore_No_Switch  ; cd2031010100
        pop    ebp
        ret    
        db  08Bh, 0FFh
sub_3_8710:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8730:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8750:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        test   byte ptr [ebp + 0x10], 8
        je     loc_3_87A2
        mov    eax, dword ptr [0]            ; -> obj4:0x0000
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> obj4:0x0000
        test   byte ptr [5], 1               ; -> obj4:0x0005
        jne    loc_3_879C
        or     dword ptr [4], 0x100          ; -> obj4:0x0004
        push   0
        push   0x46                          ; -> loc_4_0046
        call   sub_1_07F0
        add    esp, 8
loc_3_879C:
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_87A2:
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_87C0:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 0x20], 0x10
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_87D0:
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_87E0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + 0x1a], 0x78
        ret    
        db  08Dh, 049h, 000h
loc_3_87F0:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x24], 0
        je     loc_3_8803
        push   eax
        call   sub_3_83B0
        add    esp, 4
loc_3_8803:
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8810:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 0x14]
        lea    ecx, [esi + 0x90]
        cmp    byte ptr [eax + 0x1e], 0
        mov    eax, dword ptr [ecx]
        je     loc_3_8830
        movzx  edi, byte ptr [eax + 0x19]
        jmp    loc_3_8834
        db  08Dh, 049h, 000h
loc_3_8830:
        movzx  edi, byte ptr [eax + 0x18]
loc_3_8834:
        and    edi, 6
        or     edi, 1
        mov    eax, dword ptr [ecx]
        push   edi
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 0x20]
        push   ecx
        add    eax, 0x21
        push   eax
        push   esi
        call   sub_3_88E0
        add    esp, 0x10
        push   edi
        push   esi
        call   sub_3_8AB0
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  08Dh, 049h, 000h
sub_3_8860:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        lea    ecx, [esi + 0x94]
        cmp    byte ptr [eax + 0x1e], 0
        mov    eax, dword ptr [ecx]
        je     loc_3_8880
        mov    al, byte ptr [eax]
        jmp    loc_3_8883
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_8880:
        mov    al, byte ptr [eax + 5]
loc_3_8883:
        and    eax, 0xff
        push   2
        push   eax
        mov    eax, dword ptr [ecx]
        add    eax, 8
        push   eax
        push   esi
        call   sub_3_88E0
        add    esp, 0x10
        push   2
        push   esi
        call   sub_3_8AB0
        add    esp, 8
        pop    esi
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_88B0:
        push   esi
        push   8
        mov    esi, dword ptr [esp + 0xc]
        push   0x20
        mov    eax, dword ptr [esi + 0x14]
        add    eax, 0x58
        push   eax
        push   esi
        call   sub_3_88E0
        add    esp, 0x10
        push   8
        push   esi
        call   sub_3_8AB0
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_88E0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + 0x14]
        mov    ecx, dword ptr [eax + 0x4c]
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0xc], eax
        and    word ptr [eax + 0xe], 0xfecf
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0xc]
        add    esi, 0x254
        sub    esi, dword ptr [eax + 4]
        add    esi, 0x288
        mov    ebx, dword ptr [esp + 0x24]
        test   bl, 2
        mov    eax, dword ptr [esp + 0x20]
        je     loc_3_895F
        cmp    eax, 1
        jne    loc_3_895F
        mov    edx, dword ptr [esp + 0x1c]
        mov    dl, byte ptr [edx]
        or     dl, dl
        jne    loc_3_8948
        mov    dword ptr [esp + 0x1c], 0x9ec0 ; -> obj3:0x9EC0
        mov    eax, 2
        mov    ecx, 0x4b0
        jmp    loc_3_895F
loc_3_8948:
        cmp    dl, 1
        jne    loc_3_895F
        mov    dword ptr [esp + 0x1c], 0x9ec4 ; -> loc_3_9EC4
        mov    eax, 4
        mov    ecx, 0x4b0
loc_3_895F:
        mov    dword ptr [esp + 0x10], eax
        cmp    ecx, 0x4b0
        jne    loc_3_89B0
        cmp    esi, eax
        jae    loc_3_897C
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esp + 0x10], esi
        or     word ptr [eax + 0xe], 0x10
loc_3_897C:
        mov    ecx, dword ptr [esp + 0x10]
        add    cx, 2
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [eax], cx
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 0x10]
        lea    edi, [edi + 2]
loc_3_89A0:
        lodsw  ax, word ptr [esi]
        sub    ecx, 2
        xchg   ah, al
        stosw  word ptr es:[edi], ax
        ja     loc_3_89A0
        jmp    loc_3_89E9
        db  08Dh, 049h, 000h
loc_3_89B0:
        shr    esi, 1
        cmp    esi, eax
        jae    loc_3_89C3
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esp + 0x10], esi
        or     word ptr [eax + 0xe], 0x10
loc_3_89C3:
        mov    eax, dword ptr [esp + 0xc]
        push   1
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x24]
        push   eax
        lea    eax, [esi + 2]
        push   eax
        call   sub_3_4910
        add    esp, 0x10
        add    ax, 2
        mov    word ptr [esi], ax
loc_3_89E9:
        test   bl, 8
        je     loc_3_8A28
        mov    eax, dword ptr [esp + 0x18]
        mov    edx, dword ptr [eax + 0x10]
        mov    ebx, dword ptr [edx + 4]
        mov    eax, 0x20
        movzx  ecx, word ptr [ebx]
        lea    edi, [ebx + 2]
        shr    ecx, 1
        je     loc_3_8A1D
        dec    ecx
        je     loc_3_8A1D
        lea    edi, [edi + ecx*2 - 2]
        std    
        repe scasw ax, word ptr es:[edi]
        cld    
        lea    edi, [edi + 2]
        je     loc_3_8A1D
        lea    edi, [edi + 2]
        jmp    loc_3_8A23
loc_3_8A1D:
        or     word ptr [edx + 0xe], 0x100
loc_3_8A23:
        sub    edi, ebx
        mov    word ptr [ebx], di
loc_3_8A28:
        mov    eax, dword ptr [esp + 0x18]
        mov    edx, dword ptr [eax + 0x10]
        mov    edi, dword ptr [edx + 4]
        movzx  ecx, word ptr [edi]
        mov    eax, 0x3b
        shr    ecx, 1
        lea    edi, [edi + 2]
        dec    ecx
        mov    ebx, edi
        je     loc_3_8A49
        repne scasw ax, word ptr es:[edi]
        je     loc_3_8A4C
loc_3_8A49:
        add    edi, 2
loc_3_8A4C:
        cmp    dword ptr [esp + 0x1c], 0x9ec4 ; -> loc_3_9EC4
        je     loc_3_8A6E
loc_3_8A56:
        lea    eax, [edi - 4]
        cmp    ebx, eax
        jae    loc_3_8A6E
        cmp    word ptr [eax], 0x2e
        je     loc_3_8A69
        cmp    word ptr [eax], 0x20
        jne    loc_3_8A6E
loc_3_8A69:
        sub    edi, 2
        jmp    loc_3_8A56
loc_3_8A6E:
        sub    edi, ebx
        mov    word ptr [edx + 0xc], di
        cmp    byte ptr [0x880], 0           ; -> obj1:0x0880
        jne    loc_3_8A83
        mov    edx, dword ptr [edx + 4]
        mov    word ptr [edx], di
loc_3_8A83:
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 4]
        xor    ecx, ecx
        mov    cx, word ptr [eax]
        add    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        add    eax, 0x45c
        cmp    ecx, eax
        jbe    loc_3_8AA7
        mov    eax, dword ptr [esp + 0xc]
        or     word ptr [eax + 0xe], 0x20
loc_3_8AA7:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
sub_3_8AB0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [esp + 0xc], eax
        and    word ptr [eax + 0xe], 0xfd3f
        cmp    edi, 8
        jne    loc_3_8B20
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, 6
        lea    edi, [edx + 0x10]
        mov    eax, 0x200020
        mov    esi, dword ptr [edx + 4]
        rep stosd dword ptr es:[edi], eax
        sub    edi, 0x18
        lodsw  ax, word ptr [esi]
        movzx  ecx, ax
        jecxz  loc_3_8B06
        sub    ecx, 2
        jbe    loc_3_8B06
        cmp    ecx, 0x16
        jbe    loc_3_8AFF
        mov    ecx, 0x16
loc_3_8AFF:
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        jmp    loc_3_8B0C
loc_3_8B06:
        or     word ptr [edx + 0xe], 0x200
loc_3_8B0C:
        mov    eax, dword ptr [esp + 0xc]
        or     word ptr [eax + 0xe], 8
        jmp    loc_3_8BF3
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_8B20:
        mov    eax, dword ptr [esp + 0xc]
        mov    cx, word ptr [eax + 0xc]
        and    ecx, 0xfffe
        add    ecx, dword ptr [eax + 4]
        mov    bx, word ptr [ecx]
        mov    word ptr [ecx], 0
        mov    eax, dword ptr [esp + 0xc]
        mov    ax, word ptr [eax + 0xc]
        and    eax, 0xffff
        sub    eax, 2
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 4]
        add    eax, 2
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        add    eax, 0x10
        push   eax
        call   sub_3_48AF
        mov    ecx, dword ptr [esp + 0x18]
        add    esp, 0xc
        mov    dx, word ptr [ecx + 0xc]
        and    edx, 0xfffe
        mov    ecx, dword ptr [ecx + 4]
        mov    word ptr [edx + ecx], bx
        test   edi, 4
        jne    loc_3_8BA0
        test   edi, 3
        je     loc_3_8B8E
        test   al, 3
        jne    loc_3_8BA0
loc_3_8B8E:
        test   al, 4
        je     loc_3_8BF3
        mov    eax, dword ptr [esp + 0xc]
        or     word ptr [eax + 0xe], 8
        jmp    loc_3_8BF3
        db  08Dh, 049h, 000h
loc_3_8BA0:
        mov    eax, dword ptr [esp + 0xc]
        or     word ptr [eax + 0xe], 0x48
        test   edi, 2
        je     loc_3_8BC0
        mov    eax, dword ptr [esi + 0x7c]
        lea    ecx, [eax*2 + 1]
        jmp    loc_3_8BCD
        db  08Dh, 049h, 000h
loc_3_8BC0:
        mov    eax, dword ptr [esi + 0x84]
        lea    ecx, [eax*2 + 2]
loc_3_8BCD:
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [eax + 0x4c], 0x4b0
        je     loc_3_8BE2
        cmp    byte ptr [0x9bec], 0          ; -> loc_3_9BEC
        jne    loc_3_8BF3
loc_3_8BE2:
        mov    eax, dword ptr [esp + 0xc]
        push   ecx
        add    eax, 0x10
        push   eax
        call   sub_3_48FE
        add    esp, 8
loc_3_8BF3:
        mov    eax, dword ptr [esp + 0xc]
        push   0
        mov    word ptr [eax + 0x26], 0
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 8]
        add    eax, 0x10
        push   eax
        lea    eax, [esi + 2]
        push   eax
        call   sub_3_4904
        add    esp, 0xc
        add    ax, 2
        mov    word ptr [esi], ax
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 8]
        xor    ecx, ecx
        mov    cx, word ptr [eax]
        add    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        add    eax, 0x57c
        cmp    ecx, eax
        jbe    loc_3_8C42
        mov    eax, dword ptr [esp + 0xc]
        mov    word ptr [eax + 0xe], 0x80
loc_3_8C42:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_8C50:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    dword ptr [esp + 0x14], 0
        mov    edi, dword ptr [esi + 0x10]
        mov    byte ptr [esi + 0x6c], 0
        mov    ax, word ptr [edi + 0xe]
        test   al, 2
        je     loc_3_8C84
        test   byte ptr [esi + 0x6d], 1
        jne    loc_3_8D84
        mov    dword ptr [esp + 0x14], 1
loc_3_8C84:
        test   byte ptr [esi + 0x21], 0x80
        je     loc_3_8D49
        test   al, 0x10
        jne    loc_3_8D84
        test   al, 6
        jne    loc_3_8CA0
        mov    ax, word ptr [edi + 0xc]
        jmp    loc_3_8CA6
loc_3_8CA0:
        mov    eax, dword ptr [edi + 4]
        mov    ax, word ptr [eax]
loc_3_8CA6:
        mov    ecx, dword ptr [edi + 4]
        and    eax, 0xffff
        lea    ebp, [ecx + eax]
        mov    dx, word ptr [ebp]
        mov    word ptr [esp + 0x10], dx
        mov    word ptr [ebp], 0
        mov    eax, dword ptr [edi]
        xor    edx, edx
        mov    dx, word ptr [eax]
        lea    ebx, [eax + edx]
        add    ecx, 2
        mov    dx, word ptr [ebx]
        mov    word ptr [esp + 0x12], dx
        mov    word ptr [ebx], 0
        mov    edx, dword ptr [esp + 0x14]
        or     edx, 2
        push   edx
        push   ecx
        add    eax, 2
        push   eax
        call   sub_3_490A
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_8D36
        test   byte ptr [edi + 0xe], 0x80
        jne    loc_3_8D36
        mov    dx, word ptr [esp + 0x10]
        mov    word ptr [ebp], dx
        mov    ecx, dword ptr [edi + 8]
        xor    eax, eax
        mov    ax, word ptr [ecx]
        lea    ebp, [ecx + eax]
        mov    eax, dword ptr [esp + 0x14]
        mov    dx, word ptr [ebp]
        mov    word ptr [esp + 0x10], dx
        mov    word ptr [ebp], 0
        or     eax, 4
        push   eax
        add    ecx, 2
        push   ecx
        mov    eax, dword ptr [edi]
        add    eax, 2
        push   eax
        call   sub_3_490A
        add    esp, 0xc
loc_3_8D36:
        mov    dx, word ptr [esp + 0x12]
        mov    word ptr [ebx], dx
        mov    dx, word ptr [esp + 0x10]
        mov    word ptr [ebp], dx
        jmp    loc_3_8D67
loc_3_8D49:
        test   al, 0x80
        mov    eax, 0
        jne    loc_3_8D67
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        lea    eax, [edi + 0x10]
        push   eax
        add    edi, 0x30
        push   edi
        call   sub_3_490A
        add    esp, 0xc
loc_3_8D67:
        or     eax, eax
        je     loc_3_8D84
        test   byte ptr [esp + 0x14], 1
        je     loc_3_8D80
        mov    byte ptr [esi + 0x6c], 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_3_8D80:
        mov    byte ptr [esi + 0x6c], 1
loc_3_8D84:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h
sub_3_8D90:
        sub    esp, 8
        push   edi
        mov    edx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [edx + 0x10]
        mov    dword ptr [esp + 8], edx
        mov    eax, 0x3b
        mov    edi, dword ptr [edx]
        movzx  ecx, word ptr [edi]
        lea    edi, [edi + ecx - 2]
        and    word ptr [edx + 0xe], 0xfffb
        shr    ecx, 1
        je     loc_3_8DC5
        dec    ecx
        je     loc_3_8DC5
        std    
        repne scasw ax, word ptr es:[edi]
        cld    
        jne    loc_3_8DC5
        or     word ptr [edx + 0xe], 4
loc_3_8DC5:
        mov    eax, dword ptr [esp + 0x10]
        test   byte ptr [eax + 0x21], 0x80
        jne    loc_3_8E18
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [ecx]
        cmp    ax, 2
        jbe    loc_3_8DF0
        and    eax, 0xffff
        sub    eax, 2
        push   eax
        add    ecx, 2
        push   ecx
        jmp    loc_3_8DFE
        db  08Dh, 049h, 000h
loc_3_8DF0:
        mov    word ptr [esp + 6], 0x20
        lea    eax, [esp + 6]
        push   2
        push   eax
loc_3_8DFE:
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0x30
        push   eax
        call   sub_3_491C
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 0xc
        mov    word ptr [eax + 0x46], 0
loc_3_8E18:
        pop    edi
        add    esp, 8
        ret    
        db  08Dh, 049h, 000h
loc_3_8E20:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_3_75B0
        add    esp, 4
        mov    byte ptr [esi + 0x63], 6
        mov    eax, dword ptr [esi + 0x10]
        mov    cl, byte ptr [esi + 0x6d]
        and    ecx, 0xf
        cmp    ecx, 2
        jl     loc_3_8E53
        cmp    ecx, 3
        jle    loc_3_8F0F
        cmp    ecx, 4
        je     loc_3_8F6B
loc_3_8E53:
        mov    dword ptr [esi + 0x80], 0
        mov    dword ptr [esi + 0x7c], 0
        mov    edx, 0xffffffb4
        sub    edx, eax
        xor    ecx, ecx
        add    edx, dword ptr [eax]
        mov    cx, word ptr [eax + 0x4a]
        cmp    edx, ecx
        jb     loc_3_8F41
        cmp    word ptr [eax + 0x48], 2
        ja     loc_3_8EF6
        mov    edi, dword ptr [esi + 0x14]
        add    edi, 0x10c
        mov    dword ptr [esi + 0x90], edi
        push   esi
        call   sub_3_94D0
        add    esp, 4
        mov    byte ptr [esi + 0x6e], al
        push   esi
        call   sub_3_496E
        add    esp, 4
        or     eax, eax
        jne    loc_3_8EB4
        mov    dword ptr [esi + 0x24], 2
        pop    edi
        pop    esi
        ret    
loc_3_8EB4:
        mov    word ptr [esi + 0x66], 1
        mov    al, byte ptr [edi + 1]
        and    eax, 0xffff00ff
        mov    word ptr [esi + 0x76], ax
        mov    ecx, dword ptr [edi + 2]
        mov    dword ptr [esi + 0x78], ecx
        mov    dword ptr [esi + 0x84], 0
        mov    edx, eax
        mov    eax, dword ptr [esi + 0x14]
        and    edx, 0xffff
        mov    cl, byte ptr [eax + 0x44]
        shl    edx, cl
        mov    dword ptr [esi + 0x88], edx
        mov    dword ptr [esi + 0x24], 0
        pop    edi
        pop    esi
        ret    
loc_3_8EF6:
        push   esi
        call   sub_3_8D90
        add    esp, 4
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 3], 0x80
        je     loc_3_8F0F
        mov    eax, dword ptr [esi + 0x10]
        or     word ptr [eax + 0xe], 2
loc_3_8F0F:
        mov    ax, word ptr [esi + 0x70]
        test   al, 8
        je     loc_3_8F6B
        cmp    al, 8
        je     loc_3_8F22
        cmp    word ptr [esi + 0x66], 1
        jne    loc_3_8F65
loc_3_8F22:
        push   esi
        call   sub_3_88B0
        add    esp, 4
        mov    byte ptr [esi + 0x6e], 8
        test   byte ptr [esi + 0x21], 0x80
        jne    loc_3_8F50
        cmp    byte ptr [esi + 0x70], 8
        jne    loc_3_8F50
        mov    byte ptr [esi + 0x6c], 2
        jmp    loc_3_8F59
loc_3_8F41:
        mov    dword ptr [esi + 0x24], 3
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_8F50:
        push   esi
        call   sub_3_8C50
        add    esp, 4
loc_3_8F59:
        cmp    byte ptr [esi + 0x6c], 0
        jne    loc_3_8FB8
        cmp    byte ptr [esi + 0x70], 8
        je     loc_3_8FBE
loc_3_8F65:
        and    word ptr [esi + 0x70], 0xfff7
loc_3_8F6B:
        test   byte ptr [esi + 0x23], 1
        jne    loc_3_9013
        mov    ecx, dword ptr [esi + 0x14]
        mov    ax, word ptr [ecx + 0x20]
        cmp    word ptr [esi + 0x66], ax
        jne    loc_3_8FF1
        mov    eax, dword ptr [ecx + 0x2c]
        cmp    dword ptr [esi + 0x80], eax
        jb     loc_3_8FF1
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [esi + 0x8c], eax
        mov    ax, word ptr [ecx + 0x22]
        mov    word ptr [esi + 0x76], ax
        mov    eax, dword ptr [ecx + 0x2c]
        mov    dword ptr [esi + 0x84], eax
        mov    eax, dword ptr [ecx + 0x30]
        mov    dword ptr [esi + 0x88], eax
        mov    eax, dword ptr [ecx + 0x34]
        mov    dword ptr [esi + 0x7c], eax
        jmp    loc_3_9013
loc_3_8FB8:
        cmp    byte ptr [esi + 0x70], 8
        jne    loc_3_8FC4
loc_3_8FBE:
        mov    word ptr [esi + 0x66], 0
loc_3_8FC4:
        push   esi
        call   sub_3_496E
        add    esp, 4
        or     eax, eax
        jne    loc_3_8FD8
        mov    dword ptr [esi + 0x24], 0x12
loc_3_8FD8:
        cmp    byte ptr [esi + 0x6c], 0
        jne    loc_3_8FE5
        mov    dword ptr [esi + 0x24], 0x12
loc_3_8FE5:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        ret    
loc_3_8FF1:
        mov    dword ptr [esi + 0x84], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 0x76]
        mov    cl, byte ptr [ecx + 0x44]
        shl    eax, cl
        mov    dword ptr [esi + 0x88], eax
        mov    dword ptr [esi + 0x7c], 0
loc_3_9013:
        push   0x20
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_9030:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x14]
        test   byte ptr [eax + 0x1f], 2
        jne    loc_3_9070
        mov    edx, dword ptr [eax + 0x44]
        mov    eax, dword ptr [eax + 0xc]
        mov    cl, dl
        mov    esi, dword ptr [edi + 0x88]
        shr    esi, cl
        mov    ecx, dword ptr [eax + 0xc]
        sub    cl, dl
        add    esi, dword ptr [edi + 0x78]
        shr    esi, cl
        mov    dword ptr [edi + 0x48], esi
        push   0xa
        push   edi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_9070:
        mov    dword ptr [edi + 0x24], 0
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_9080:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        cmp    dword ptr [ebx + 0x24], 0
        je     loc_3_90A2
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_90A2:
        mov    eax, dword ptr [ebx + 0x14]
        test   byte ptr [eax + 0x1f], 2
        jne    loc_3_9153
        mov    eax, dword ptr [eax + 0xc]
        mov    edi, 1
        mov    eax, dword ptr [eax + 0xc]
        mov    cl, al
        shl    edi, cl
        mov    dword ptr [esp + 0x14], eax
        lea    eax, [edi - 1]
        and    eax, dword ptr [ebx + 0x88]
        add    eax, dword ptr [ebx + 0x4c]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, 1
        shl    eax, cl
        mov    edx, dword ptr [ebx + 0x4c]
        lea    esi, [eax + edx - 0x21]
        cmp    dword ptr [ebx + 0x84], 0
        jne    loc_3_9271
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, dword ptr [ebx + 0x88]
        mov    edx, dword ptr [eax + 0xa]
        add    edx, ebp
        add    edx, edi
        dec    edx
        shr    edx, cl
        mov    dword ptr [ebx + 0x8c], edx
        test   byte ptr [ebx + 0x23], 1
        je     loc_3_9118
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [eax + 0x24], edx
        jmp    loc_3_9271
loc_3_9118:
        mov    ecx, dword ptr [ebx + 0x14]
        mov    ax, word ptr [ebx + 0x66]
        mov    word ptr [ecx + 0x20], ax
        mov    ax, word ptr [ebx + 0x76]
        mov    word ptr [ecx + 0x22], ax
        mov    dword ptr [ecx + 0x2c], 0
        mov    dword ptr [ecx + 0x28], edx
        mov    dword ptr [ecx + 0x30], ebp
        mov    eax, dword ptr [ebx + 0x78]
        mov    dword ptr [ecx + 0x24], eax
        mov    dword ptr [ecx + 0x34], 0
        push   ecx
        call   sub_3_7840
        add    esp, 4
        jmp    loc_3_9271
loc_3_9153:
        mov    eax, dword ptr [ebx]
        add    eax, 0x3c
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [ebx + 0x14]
        mov    esi, 0x9f20                   ; -> loc_3_9F20
        mov    edx, dword ptr [eax + 0x134]
        xor    eax, eax
        mov    al, byte ptr [0x9f20]         ; -> loc_3_9F20
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebx + 0x14]
        test   byte ptr [eax + 0x1f], 0x20
        jne    loc_3_9268
        mov    eax, dword ptr [ebx + 0x84]
        or     eax, eax
        je     loc_3_9268
        cmp    eax, 1
        je     loc_3_9260
        xor    eax, eax
        mov    al, byte ptr [0x9f44]         ; -> obj3:0x9F44
        sub    eax, 5
        mov    esi, 0x9f44                   ; -> obj3:0x9F44
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, 0x4b
        and    byte ptr [eax + 0x19], 0xfd
        mov    ecx, dword ptr [ebx + 0x84]
        mov    eax, dword ptr [edx + ecx*8 - 0x10]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, 2
        mov    dword ptr [edx + 2], eax
        sub    edx, edx
        div    esi
        mov    edi, eax
        mov    esi, 0x3c
        sub    edx, edx
        div    esi
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax + 0x17], dl
        mov    eax, edi
        sub    edx, edx
        div    esi
        mov    edi, eax
        sub    edx, edx
        div    esi
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax + 0x16], dl
        mov    eax, edi
        sub    edx, edx
        div    esi
        mov    esi, 0x18
        sub    edx, edx
        div    esi
        mov    eax, dword ptr [esp + 0x10]
        mov    edi, 0xa
        mov    byte ptr [eax + 0x15], dl
        mov    eax, dword ptr [ebx + 0x14]
        mov    esi, dword ptr [esp + 0x10]
        add    ecx, dword ptr [eax + 0x138]
        mov    eax, ecx
        sub    edx, edx
        div    edi
        add    byte ptr [esi + 0x27], dl
        mov    esi, edi
        mov    eax, ecx
        sub    edx, edx
        div    esi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, edi
        sub    edx, edx
        div    esi
        add    byte ptr [ecx + 0x26], dl
        jmp    loc_3_9268
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_9260:
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax + 0x21], 1
loc_3_9268:
        mov    eax, dword ptr [esp + 0x10]
        movzx  esi, byte ptr [eax]
        add    esi, eax
loc_3_9271:
        mov    byte ptr [ebx + 0x6c], 0
        cmp    esi, dword ptr [esp + 0x10]
        jbe    loc_3_939B
        mov    ebp, 2
loc_3_9284:
        mov    eax, dword ptr [esp + 0x10]
        cmp    byte ptr [eax], 0
        je     loc_3_939B
        mov    dword ptr [ebx + 0x90], eax
        mov    edi, dword ptr [ebx + 0x20]
        mov    eax, dword ptr [ebx + 0x14]
        and    dword ptr [ebx + 0x20], 0xdfffffff
        cmp    byte ptr [eax + 0x1e], 0
        mov    eax, dword ptr [esp + 0x10]
        je     loc_3_92B3
        mov    al, byte ptr [eax + 0x19]
        jmp    loc_3_92B6
loc_3_92B3:
        mov    al, byte ptr [eax + 0x18]
loc_3_92B6:
        and    eax, 0xff
        test   al, 0x80
        je     loc_3_92C6
        or     dword ptr [ebx + 0x20], 0x20000000
loc_3_92C6:
        push   ebx
        call   sub_3_8810
        add    esp, 4
        push   ebx
        call   sub_3_94D0
        add    esp, 4
        mov    byte ptr [ebx + 0x6e], al
        mov    eax, dword ptr [ebx + 0x84]
        cmp    dword ptr [ebx + 0x80], eax
        ja     loc_3_9356
        test   byte ptr [ebx + 0x6e], 4
        jne    loc_3_9356
        cmp    word ptr [ebx + 0x66], 1
        jne    loc_3_9306
        mov    eax, dword ptr [esp + 0x10]
        cmp    byte ptr [eax + 0x20], 1
        jne    loc_3_9306
        cmp    byte ptr [eax + 0x21], 1
        jbe    loc_3_9356
loc_3_9306:
        mov    al, byte ptr [ebx + 0x6d]
        and    al, 0xf
        cmp    al, 4
        je     loc_3_9397
        test   edi, 0x20000000
        je     loc_3_9320
        jmp    loc_3_9352
        db  08Dh, 049h, 000h
loc_3_9320:
        push   ebx
        call   sub_3_8C50
        add    esp, 4
        cmp    byte ptr [ebx + 0x6c], 0
        je     loc_3_9356
        push   ebx
        call   sub_3_496E
        add    esp, 4
        or     eax, eax
        jne    loc_3_939B
        mov    eax, dword ptr [ebx + 0x10]
        or     dword ptr [ebx + 0x20], 0x800
        test   word ptr [eax + 0xe], bp
        jne    loc_3_9352
        test   byte ptr [ebx + 0x6e], 6
        je     loc_3_9391
loc_3_9352:
        mov    byte ptr [ebx + 0x6c], 0
loc_3_9356:
        test   byte ptr [ebx + 0x6e], 0x10
        je     loc_3_935F
        inc    dword ptr [ebx + 0x7c]
loc_3_935F:
        inc    dword ptr [ebx + 0x84]
        mov    eax, dword ptr [esp + 0x10]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    dword ptr [ebx + 0x88], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        add    dword ptr [esp + 0x10], eax
        cmp    esi, dword ptr [esp + 0x10]
        ja     loc_3_9284
        jmp    loc_3_939B
loc_3_9391:
        mov    byte ptr [ebx + 0x6c], 3
        jmp    loc_3_939B
loc_3_9397:
        mov    byte ptr [ebx + 0x6c], 1
loc_3_939B:
        xor    eax, eax
        mov    al, byte ptr [ebx + 0x6c]
        or     eax, eax
        je     loc_3_93C3
        cmp    eax, 1
        jl     loc_3_942F
        cmp    eax, 2
        jle    loc_3_9437
        cmp    eax, 3
        je     loc_3_941F
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_93C3:
        mov    eax, dword ptr [ebx + 0x14]
        mov    cl, byte ptr [eax + 0x1f]
        mov    dl, cl
        and    dl, 0x22
        cmp    dl, 0x22
        je     loc_3_941B
        test   cl, 2
        je     loc_3_93F1
        mov    eax, dword ptr [eax + 0x130]
        add    eax, 2
        cmp    eax, dword ptr [ebx + 0x84]
        jbe    loc_3_941B
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_93F1:
        mov    eax, dword ptr [ebx + 0x4c]
        sub    eax, dword ptr [esp + 0x10]
        mov    edx, 1
        mov    cl, byte ptr [esp + 0x14]
        shl    edx, cl
        add    eax, edx
        add    dword ptr [ebx + 0x88], eax
        mov    eax, dword ptr [ebx + 0x8c]
        shl    eax, cl
        cmp    eax, dword ptr [ebx + 0x88]
        ja     loc_3_942F
loc_3_941B:
        mov    byte ptr [ebx + 0x6c], 3
loc_3_941F:
        mov    dword ptr [ebx + 0x24], 2
        push   ebx
        call   sub_3_83B0
        add    esp, 4
loc_3_942F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_9437:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x90], eax
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        test   byte ptr [ebx + 0x23], 1
        je     loc_3_942F
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_9470:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_3_94C4
        test   byte ptr [esi + 0x22], 2
        jne    loc_3_94C4
        mov    ecx, dword ptr [esi + 0x14]
        mov    ax, word ptr [esi + 0x66]
        mov    word ptr [ecx + 0x20], ax
        mov    ax, word ptr [esi + 0x76]
        mov    word ptr [ecx + 0x22], ax
        mov    eax, dword ptr [esi + 0x78]
        mov    dword ptr [ecx + 0x24], eax
        mov    eax, dword ptr [esi + 0x8c]
        mov    dword ptr [ecx + 0x28], eax
        mov    eax, dword ptr [esi + 0x84]
        mov    dword ptr [ecx + 0x2c], eax
        mov    eax, dword ptr [esi + 0x88]
        mov    dword ptr [ecx + 0x30], eax
        mov    eax, dword ptr [esi + 0x7c]
        mov    dword ptr [ecx + 0x34], eax
        push   ecx
        call   sub_3_7840
        add    esp, 4
loc_3_94C4:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret    
        db  090h
sub_3_94D0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x14]
        mov    eax, dword ptr [eax + 0x90]
        cmp    byte ptr [ecx + 0x1e], 0
        je     loc_3_94F0
        mov    al, byte ptr [eax + 0x19]
        push   eax
        call   sub_3_9500
        add    esp, 4
        ret    
loc_3_94F0:
        mov    al, byte ptr [eax + 0x18]
        push   eax
        call   sub_3_9500
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_3_9500:
        mov    al, 1
        mov    cl, byte ptr [esp + 4]
        test   al, cl
        je     loc_3_950C
        mov    al, 3
loc_3_950C:
        test   cl, 2
        je     loc_3_9515
        or     al, 0x10
        and    al, 0xfe
loc_3_9515:
        test   cl, 4
        je     loc_3_951C
        or     al, 6
loc_3_951C:
        ret    
        db  08Dh, 049h, 000h
sub_3_9520:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_9620
        add    esp, 4
        test   dword ptr [esi + 0x20], 0x20020000
        jne    loc_3_9550
        push   1
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0xc
        mov    dword ptr [esi + 0xc0], eax
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_3_9550:
        test   byte ptr [esi + 0x23], 0x20
        jne    loc_3_958A
        push   0
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0xc
        add    dword ptr [esi + 0xc0], eax
        and    dword ptr [esi + 0x20], 0xfffdffff
        mov    eax, dword ptr [esi + 0xbc]
        mov    dword ptr [esi + 0x88], eax
        mov    eax, dword ptr [esi + 0xb8]
        dec    eax
        mov    dword ptr [esi + 0x84], eax
        jmp    loc_3_95ED
loc_3_958A:
        test   byte ptr [esi + 0x22], 2
        jne    loc_3_95D4
        cmp    dword ptr [esi + 0xb8], 0
        je     loc_3_95A0
        sub    eax, eax
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_3_95A0:
        mov    ecx, dword ptr [esi + 0x84]
        inc    ecx
        mov    dword ptr [esi + 0xb8], ecx
        mov    ecx, dword ptr [esi + 0x88]
        mov    dword ptr [esi + 0xbc], ecx
        or     dword ptr [esi + 0x20], 0x20000
        push   1
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0xc
        mov    dword ptr [esi + 0xc0], eax
        jmp    loc_3_95E6
loc_3_95D4:
        push   0
        push   eax
        push   esi
        call   sub_3_9650
        add    esp, 0xc
        add    dword ptr [esi + 0xc0], eax
loc_3_95E6:
        mov    eax, dword ptr [esi + 0x84]
        inc    eax
loc_3_95ED:
        mov    dword ptr [esi + 0x80], eax
        mov    byte ptr [esi + 0x6d], 0x24
        mov    ax, word ptr [esi + 0x2c]
        mov    word ptr [esi + 0x2e], ax
        push   0x24
        push   esi
        call   sub_3_8380
        add    esp, 8
        push   0x20
        push   esi
        call   sub_3_8380
        add    esp, 8
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
sub_3_9620:
        mov    eax, dword ptr [esp + 4]
        xor    edx, edx
        mov    ecx, dword ptr [eax + 0x90]
        mov    al, byte ptr [ecx + 0x20]
        or     al, 1
        and    eax, 0xff
        lea    eax, [ecx + eax + 0x21]
        mov    dl, byte ptr [ecx]
        add    edx, ecx
        lea    ecx, [eax + 0xe]
        cmp    edx, ecx
        jb     loc_3_964D
        cmp    word ptr [eax + 6], 0x4158
        je     loc_3_964F
loc_3_964D:
        sub    eax, eax
loc_3_964F:
        ret    
sub_3_9650:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 0x90]
        or     ecx, ecx
        mov    eax, dword ptr [eax + 0xa]
        je     loc_3_96AE
        mov    dx, word ptr [ecx + 4]
        test   dl, 0x50
        je     loc_3_96AE
        add    eax, 0x7ff
        shr    eax, 0xb
        mov    esi, eax
        lea    eax, [eax + eax*8]
        lea    eax, [esi + eax*8]
        lea    eax, [esi + eax*2]
        shl    eax, 4
        test   dl, 0x40
        je     loc_3_96A4
        mov    cl, byte ptr [ecx + 8]
        cmp    cl, 0x10
        jae    loc_3_96A4
        and    ecx, 0xff
        mov    cl, byte ptr [ecx + ecx*4 + 0xa054] ; -> loc_3_A054
        shr    cl, 4
        shr    eax, cl
loc_3_96A4:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_96AE
        add    eax, 0x2c
loc_3_96AE:
        pop    esi
        ret    
sub_3_96B0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [eax + 4]
        cmp    word ptr [ecx + 0xf8], 1
        je     loc_3_96D8
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [ecx + 0xfa]
        cmp    word ptr [eax + 0x10], dx
        je     loc_3_96DC
        sub    eax, eax
        ret    
loc_3_96D8:
        mov    eax, dword ptr [esp + 8]
loc_3_96DC:
        mov    dword ptr [eax + 4], ecx
        mov    eax, 1
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_96F0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_3_75B0
        add    esp, 4
        mov    byte ptr [esi + 0x63], 7
        mov    dl, byte ptr [esi + 0x6d]
        mov    al, dl
        and    eax, 0xf0
        cmp    eax, 0x10
        je     loc_3_9746
        cmp    eax, 0x20
        je     loc_3_977A
        push   esi
        call   sub_3_8D90
        add    esp, 4
        mov    word ptr [esi + 0x66], 1
        mov    eax, dword ptr [esi + 0x14]
        mov    word ptr [esi + 0x74], 1
        mov    al, byte ptr [eax + 0x1f]
        and    al, 2
        cmp    al, 1
        mov    dword ptr [esi + 0x7c], 1
        sbb    eax, eax
        inc    eax
        mov    word ptr [esi + 0x64], ax
        jmp    loc_3_97B0
loc_3_9746:
        mov    ecx, dword ptr [esi + 0x14]
        mov    ax, word ptr [esi + 0x66]
        cmp    word ptr [ecx + 0x20], ax
        jne    loc_3_9771
        mov    word ptr [esi + 0x74], ax
        mov    ax, word ptr [ecx + 0x22]
        mov    word ptr [esi + 0x76], ax
        mov    eax, dword ptr [ecx + 0x24]
        mov    dword ptr [esi + 0x78], eax
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [esi + 0x8c], eax
        pop    edi
        pop    esi
        ret    
loc_3_9771:
        and    dl, 0xf
        or     dl, 0x20
        mov    byte ptr [esi + 0x6d], dl
loc_3_977A:
        mov    eax, dword ptr [esi + 0x14]
        sub    edx, edx
        mov    edi, dword ptr [eax + 0x108]
        mov    ecx, dword ptr [eax + 0x104]
        dec    ecx
        je     loc_3_97A5
        mov    ax, word ptr [esi + 0x66]
loc_3_9792:
        cmp    word ptr [edi], ax
        ja     loc_3_979D
        cmp    word ptr [edi + 0xc], ax
        ja     loc_3_97A5
loc_3_979D:
        inc    edx
        add    edi, 0xc
        cmp    edx, ecx
        jb     loc_3_9792
loc_3_97A5:
        mov    ax, word ptr [edi]
        mov    word ptr [esi + 0x74], ax
        mov    word ptr [esi + 0x64], dx
loc_3_97B0:
        push   0x1e
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_3_97C0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    si, word ptr [edi + 0x64]
        test   si, 1
        jne    loc_3_9806
        mov    edx, dword ptr [edi + 0x14]
        test   byte ptr [edx + 0x1f], 2
        jne    loc_3_9806
        mov    eax, dword ptr [edx + 0xc]
        mov    ebx, dword ptr [edx + 0xfc]
        mov    ecx, dword ptr [eax + 0xc]
        shr    si, 1
        sub    cl, byte ptr [edx + 0x44]
        mov    eax, esi
        shr    ebx, cl
        and    eax, 0xffff
        add    ebx, eax
        mov    dword ptr [edi + 0x48], ebx
        push   0xa
        push   edi
        call   sub_3_8380
        add    esp, 8
loc_3_9806:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_9810:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x14], 0
        cmp    dword ptr [esi + 0x24], 0
        je     loc_3_983A
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_3_983A:
        mov    edi, dword ptr [esi + 0x10]
        mov    eax, dword ptr [esi + 0x14]
        mov    dx, word ptr [esi + 0x64]
        mov    ecx, edx
        and    ecx, 0xffff
        mov    eax, dword ptr [eax + 0x108]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [eax + ecx*4]
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 2]
        test   dl, 1
        je     loc_3_9868
        mov    eax, dword ptr [ecx + 8]
        jmp    loc_3_9871
loc_3_9868:
        xor    eax, eax
        mov    ax, word ptr [ecx + 4]
        add    eax, dword ptr [esi + 0x4c]
loc_3_9871:
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esi + 0x14]
        test   byte ptr [esi + 0x6d], 0xf0
        movzx  ebp, byte ptr [eax + 0x1e]
        je     loc_3_98BA
        cmp    word ptr [ecx + 6], 0
        je     loc_3_98BA
        xor    eax, eax
        xor    edx, edx
        mov    dx, word ptr [esi + 0x74]
        mov    ax, word ptr [esi + 0x66]
        sub    eax, edx
        cmp    eax, 0x40
        jl     loc_3_98BA
        mov    ax, word ptr [esi + 0x74]
        add    ax, 0x3f
        mov    word ptr [esi + 0x74], ax
        xor    eax, eax
        mov    ax, word ptr [ecx + 6]
        add    dword ptr [esp + 0x10], eax
        xor    eax, eax
        mov    ax, word ptr [ecx + 6]
        sub    ebx, eax
loc_3_98BA:
        or     ebx, ebx
        jle    loc_3_9A58
loc_3_98C2:
        test   byte ptr [esi + 0x6d], 0xf0
        jne    loc_3_9960
        cmp    word ptr [esi + 0x74], 1
        je     loc_3_996D
        mov    ecx, dword ptr [esp + 0x10]
        mov    ax, word ptr [esi + 0x66]
        cmp    word ptr [ecx + 6], ax
        jne    loc_3_99D9
        inc    dword ptr [esi + 0x7c]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x94], eax
        push   esi
        call   sub_3_8860
        add    esp, 4
        push   esi
        call   sub_3_8C50
        add    esp, 4
        cmp    byte ptr [esi + 0x6c], 0
        je     loc_3_99D9
        mov    eax, dword ptr [edi]
        xor    ecx, ecx
        mov    cx, word ptr [eax]
        add    ecx, eax
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    ax, word ptr [ecx]
        add    word ptr [edi + 0x2a], ax
        add    word ptr [edi + 0x28], ax
        and    eax, 0xffff
        add    eax, ecx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [edi + 4], eax
        mov    ax, word ptr [ecx]
        add    word ptr [edi + 0x2e], ax
        add    word ptr [edi + 0x2c], ax
        and    eax, 0xffff
        add    eax, ecx
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esi + 0x7c], 1
        push   esi
        call   sub_3_8D90
        add    esp, 4
        jmp    loc_3_996D
        db  090h
loc_3_9960:
        lea    eax, [esi + 0x74]
        mov    ax, word ptr [esi + 0x74]
        cmp    word ptr [esi + 0x66], ax
        jne    loc_3_99D9
loc_3_996D:
        mov    ax, word ptr [esi + 0x76]
        mov    word ptr [esi + 0x5c], ax
        mov    ax, word ptr [esi + 0x66]
        mov    word ptr [esi + 0x5e], ax
        or     ebp, ebp
        mov    eax, dword ptr [esi + 0x78]
        mov    dword ptr [esi + 0x98], eax
        mov    ax, word ptr [esi + 0x74]
        mov    word ptr [esi + 0x66], ax
        mov    eax, dword ptr [esp + 0x10]
        je     loc_3_999B
        mov    eax, dword ptr [eax + 2]
        jmp    loc_3_999D
loc_3_999B:
        mov    eax, dword ptr [eax]
loc_3_999D:
        mov    dword ptr [esi + 0x78], eax
        or     ebp, ebp
        mov    eax, dword ptr [esp + 0x10]
        je     loc_3_99B0
        mov    al, byte ptr [eax + 1]
        jmp    loc_3_99B3
        db  08Dh, 049h, 000h
loc_3_99B0:
        mov    al, byte ptr [eax + 4]
loc_3_99B3:
        and    eax, 0xffff00ff
        test   byte ptr [esi + 0x6d], 0xf0
        mov    word ptr [esi + 0x76], ax
        jne    loc_3_9A50
        mov    eax, 0xffffffb4
        sub    eax, edi
        xor    ecx, ecx
        add    eax, dword ptr [edi]
        mov    cx, word ptr [edi + 0x4a]
        cmp    eax, ecx
        jae    loc_3_9A50
loc_3_99D9:
        inc    word ptr [esi + 0x74]
        or     ebp, ebp
        mov    eax, dword ptr [esp + 0x10]
        je     loc_3_99EB
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        jmp    loc_3_99F0
loc_3_99EB:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 5]
loc_3_99F0:
        or     ebp, ebp
        mov    eax, dword ptr [esp + 0x10]
        je     loc_3_9A00
        mov    al, byte ptr [eax]
        jmp    loc_3_9A03
        db  08Dh, 064h, 024h, 000h
loc_3_9A00:
        mov    al, byte ptr [eax + 5]
loc_3_9A03:
        and    eax, 0xff
        and    eax, 1
        add    eax, ecx
        add    eax, 8
        sub    ebx, eax
        or     ebp, ebp
        mov    eax, dword ptr [esp + 0x10]
        je     loc_3_9A20
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        jmp    loc_3_9A25
loc_3_9A20:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 5]
loc_3_9A25:
        or     ebp, ebp
        mov    eax, dword ptr [esp + 0x10]
        je     loc_3_9A31
        mov    al, byte ptr [eax]
        jmp    loc_3_9A34
loc_3_9A31:
        mov    al, byte ptr [eax + 5]
loc_3_9A34:
        and    eax, 0xff
        and    eax, 1
        add    eax, ecx
        add    eax, 8
        add    dword ptr [esp + 0x10], eax
        or     ebx, ebx
        jg     loc_3_98C2
        jmp    loc_3_9A58
        db  090h
loc_3_9A50:
        mov    dword ptr [esp + 0x14], 1
loc_3_9A58:
        test   byte ptr [esi + 0x6d], 0xf0
        je     loc_3_9A80
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_3_9A6C
        mov    dword ptr [esi + 0x24], 2
loc_3_9A6C:
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h
loc_3_9A80:
        mov    ax, word ptr [esi + 0x64]
        inc    ax
        cmp    dword ptr [esp + 0x14], 0
        mov    word ptr [esi + 0x64], ax
        mov    dword ptr [esi + 0x24], 0
        jne    loc_3_9AA8
        mov    ecx, dword ptr [esi + 0x14]
        and    eax, 0xffff
        cmp    dword ptr [ecx + 0x104], eax
        ja     loc_3_9AB1
loc_3_9AA8:
        push   esi
        call   sub_3_83B0
        add    esp, 4
loc_3_9AB1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_9AC0:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [0x890]        ; -> obj1:0x0890
        lea    ecx, [edx + edx*2]
        lea    ebx, [eax + ecx*2 + 0x14]
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0x1d
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x34], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0x38], eax
        cmp    edx, 0x1a
        mov    dword ptr [esi + 0x28], edx
        jae    loc_3_9B30
        mov    al, byte ptr [ebx + 1]
        test   al, 8
        je     loc_3_9B30
        test   al, 0x10
        jne    loc_3_9B10
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_9B10:
        push   esi
        call   sub_3_9B40
        add    esp, 4
        cmp    word ptr [esi + 0x1a], 0
        je     loc_3_9B36
        mov    word ptr [esi + 0x1a], 0x1e
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_9B30:
        mov    word ptr [esi + 0x1a], 0xf
loc_3_9B36:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
sub_3_9B40:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        sub    edi, edi
        lea    ebx, [esi + 0x10]
        push   1
        push   ebx
        mov    eax, dword ptr [esi + 0x28]
        push   eax
        call   sub_3_31B0
        add    esp, 0xc
        cmp    eax, edi
        je     loc_3_9B63
        mov    dword ptr [ebx], edi
        jmp    loc_3_9B7B
loc_3_9B63:
        mov    edi, dword ptr [0x9bd0]       ; -> obj3:0x9BD0
        or     edi, edi
        je     loc_3_9B7B
        mov    eax, dword ptr [ebx]
loc_3_9B70:
        cmp    dword ptr [edi + 4], eax
        je     loc_3_9B7B
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_3_9B70
loc_3_9B7B:
        mov    dword ptr [esi + 0x1c], edi
        or     edi, edi
        jne    loc_3_9B90
        mov    word ptr [esi + 0x1a], 0x1e
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_9B90:
        mov    word ptr [esi + 0x1a], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_9BA0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x10]
        push   ecx
        mov    eax, dword ptr [eax + 0x28]
        push   eax
        call   sub_3_31B6
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_9BE4:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_9BE8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_9BEC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9BF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9BF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9BFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9C00:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9C04:
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_3_9C08:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_3_9C0C:
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_3_9C10:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_9C14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9C1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9C20:
        inc    ebx
        inc    esp
        inc    esi
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_9C74:
        and    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9CAD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9CEF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x39], dl     ; -> loc_3_3950
        add    byte ptr [eax], al
        mov    al, byte ptr [0xf0000039]     ; -> loc_3_0FF0
        sldt   word ptr [eax]
        dd  loc_3_0FF0               ; fixup
        dd  loc_3_0FF0               ; fixup
        dd  loc_3_10D0               ; fixup
        dd  loc_3_1200               ; fixup
        dd  loc_3_1200               ; fixup
        dd  loc_3_1210               ; fixup
        dd  loc_3_1320               ; fixup
        dd  loc_3_1340               ; fixup
        dd  loc_3_39F0               ; fixup
        dd  loc_3_3A30               ; fixup
        dd  loc_3_3A50               ; fixup
        dd  loc_3_1350               ; fixup
        dd  loc_3_1440               ; fixup
        dd  loc_3_31D0               ; fixup
loc_3_9E24:
        mov    al, 5
        add    byte ptr [eax], al
        mov    al, byte ptr [0x32]
        add    byte ptr [eax], al
        add    al, dl                        ; -> loc_3_05D0
        add    eax, 0x6000000                ; -> loc_3_0600
        add    byte ptr [eax], al
        mov    al, byte ptr [6]              ; -> loc_3_3300
        xor    eax, dword ptr [eax]
        add    al, dl                        ; -> loc_3_06D0
        push   es
        add    byte ptr [eax], al
        xor    byte ptr [edi], al
        add    byte ptr [eax], al
        rol    byte ptr [edi], 0
        add    byte ptr [eax], dh            ; -> loc_3_0830
        or     byte ptr [eax], al
        add    byte ptr [eax + 0x50000008], ah ; -> loc_3_08A0
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], dl            ; -> loc_3_0910
        or     dword ptr [eax], eax
        add    byte ptr [eax - 0xffffff7], dl ; -> loc_3_0990
        xor    eax, dword ptr [eax]
        db  000h
loc_3_9E64:
        lock or dword ptr [eax], eax
        add    byte ptr [eax], ah            ; -> loc_3_0A20
        or     al, byte ptr [eax]
        add    byte ptr [eax + 0xa], ah      ; -> loc_3_0A60
        add    byte ptr [eax], al
        nop    
        or     al, byte ptr [eax]
        add    byte ptr [eax], ah            ; -> loc_3_0B20
        or     eax, dword ptr [eax]
        add    byte ptr [eax + 0xb], dl      ; -> loc_3_0B50
        add    byte ptr [eax], al
        push   eax
        xor    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ah                        ; -> loc_3_0BE0
        or     eax, dword ptr [eax]
        add    al, dh                        ; -> loc_3_0BF0
        or     eax, dword ptr [eax]
        add    byte ptr [eax + 0xc], al      ; -> loc_3_0C40
        add    byte ptr [eax], al
        ror    byte ptr [0], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
loc_3_9EA0:
        push   ds
        add    byte ptr [eax + eax], al
        sbb    al, byte ptr [eax]
        aas    
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [esi], ch
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr cs:[eax], al
loc_3_9EC4:
        add    byte ptr [esi], ch
        add    byte ptr [esi], ch
loc_3_9EC8:
        add    byte ptr [eax], al
        pop    ds
        add    byte ptr [ebx], bh
        add    byte ptr [edx], bl
        js     loc_3_9ED2
loc_3_9ED2:
        xchg   edi, eax
        add    byte ptr [ebp - 0xcff2c00], dh
        add    byte ptr [ecx], dl
        add    dword ptr [eax], esi
        add    dword ptr [esi + 1], ecx
loc_3_9EE0:
        inc    ebx
        inc    esp
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
loc_3_9F00:
        inc    ecx
        jne    0x9f67
        imul   ebp, dword ptr [edi + 0x20], 0x20204443
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
loc_3_9F20:
        and    al, byte ptr [eax]
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   esi, eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        pop    edi
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        add    byte ptr [esi], dl
        add    byte ptr [eax], al
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9F56:
        pop    edi
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        push   esp
        jb     0x9fc9
        arpl   word ptr [ebx + 0x30], bp
        xor    byte ptr [esi], ch
        arpl   word ptr [ecx], sp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    eax
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9F7C:
        push   edx
        dec    ecx
        inc    esi
        inc    esi
        and    al, 0
        add    byte ptr [eax], al
        push   edi
        inc    ecx
        push   esi
        inc    ebp
        insw   word ptr es:[edi], dx
        je     loc_3_9FAC
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        inc    esp
        lodsb  al, byte ptr [esi]
        add    byte ptr [eax], al
        adc    byte ptr [ecx + 0x40002], dh
        adc    byte ptr [eax], al
        popal  
        je     loc_3_A005
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_9FA8:
        push   edx
        dec    ecx
        inc    esi
        inc    esi
loc_3_9FAC:
        and    al, 0
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        inc    esp
        inc    ecx
        insw   word ptr es:[edi], dx
        je     loc_3_9FD8
        sbb    byte ptr [eax], al
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
loc_3_9FD4:
        push   edx
        dec    ecx
        inc    esi
        inc    esi
loc_3_9FD8:
        and    al, 0
        add    byte ptr [eax], al
        inc    ebx
        inc    esp
        pop    eax
        inc    ecx
        insw   word ptr es:[edi], dx
        je     0xa004
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    eax
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        popal  
        je     0xa05d
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_A000:
        add    dword ptr [eax], eax
        xchg   esi, eax
        add    byte ptr [eax], al
loc_3_A005:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_3_A000
        mov    al, byte ptr [0x20000]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_A030:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x20000], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_A054:
        add    byte ptr [esi], ah            ; -> loc_3_5A26
        pop    edx
        add    byte ptr [eax], al
        adc    al, 0x23                      ; -> loc_3_5A23
        pop    edx
        add    byte ptr [eax], al
        sub    byte ptr [eax], ah            ; -> loc_3_5A20
        pop    edx
        add    byte ptr [eax], al
        add    byte ptr [esi], ah            ; -> loc_3_5A26
        pop    edx
        add    byte ptr [eax], al
        adc    dh, byte ptr [eax]            ; -> loc_3_5A30
        pop    edx
        add    byte ptr [eax], al
        sub    eax, 0x3a00005a               ; -> loc_3_5A2D
        sub    bl, byte ptr [edx]
        add    byte ptr [eax], al
        pop    edx
        add    byte ptr [eax], al
        adc    dword ptr [ebx + 0x5a], ecx   ; -> loc_3_5A4B
        add    byte ptr [eax], al
        and    eax, 0x5a48                   ; -> loc_3_5A48
        cmp    dword ptr [ebp + 0x5a], eax   ; -> loc_3_5A45
        add    byte ptr [eax], al
        add    byte ptr [esi], ah            ; -> loc_3_5A26
        pop    edx
        add    byte ptr [eax], al
        and    esp, dword ptr [ebx + 0x5a]   ; -> loc_3_5A63
        add    byte ptr [eax], al
        aaa    
        pushal 
        pop    edx
        add    byte ptr [eax], al
        dec    ebx
        pop    ebp
        pop    edx
        add    byte ptr [eax], al
        add    byte ptr [esi], ah            ; -> loc_3_5A26
        pop    edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_A0AC:
        add    byte ptr [eax - 0x782fffff], al ; -> loc_3_87D0
        add    byte ptr [eax], al
        add    dword ptr [eax - 0x78400000], eax ; -> loc_3_87C0
        add    byte ptr [eax], al
        add    al, byte ptr [eax - 0x7c500000] ; -> sub_3_83B0
        add    byte ptr [eax], al
        add    eax, dword ptr [eax - 0x781fffff] ; -> loc_3_87E0
        add    byte ptr [eax], al
        add    al, 0x80
        add    eax, 0x5ae000                 ; -> loc_3_5AE0
        add    byte ptr [0x70000200], al     ; -> loc_3_5770
        push   edi
        add    byte ptr [eax], al
        push   es
        add    byte ptr [edi], 0
        add    byte ptr [edi], ah
        add    byte ptr [edi], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], ah            ; -> sub_3_6720
        add    byte ptr [bx + si], al
        or     byte ptr [eax + 0x5b900009], al ; -> loc_3_5B90
        add    byte ptr [eax], al
        or     dword ptr [eax + 0x5c600002], eax ; -> loc_3_5C60
        add    byte ptr [eax], al
        or     al, byte ptr [eax + 0x5a80000b] ; -> loc_3_5A80
        add    byte ptr [eax], al
        or     eax, dword ptr [eax + 0x5aa00006] ; -> loc_3_5AA0
        add    byte ptr [eax], al
        or     al, 0
        or     eax, 0x1e3000                 ; -> loc_3_1E30
        add    byte ptr [0x10000e00], cl     ; -> loc_3_2110
        and    dword ptr [eax], eax
        add    byte ptr [esi], cl
        add    byte ptr [edi], cl
        add    byte ptr [eax + 0x23], dl     ; -> loc_3_2350
        add    byte ptr [eax], al
        lldt   word ptr [eax]
        add    byte ptr [eax + 0x10000023], ah ; -> loc_3_23A0
        add    byte ptr [ecx], dl
        add    byte ptr [eax + 0x11000027], al ; -> loc_3_2780
        add    byte ptr [edx], dl
        add    byte ptr [eax + 0x1200002e], al ; -> loc_3_2E80
        add    byte ptr [ebx], dl
        add    byte ptr [eax + 0x1300002e], ah ; -> loc_3_2EA0
        add    byte ptr [eax + eax], dl
        adc    byte ptr [edi], ch
        add    byte ptr [eax], al
        adc    al, 0
        add    dword ptr [eax], eax
        xor    byte ptr [edi], ch
        add    byte ptr [eax], al
        adc    eax, 0x60000100               ; -> loc_3_3060
        xor    byte ptr [eax], al
        add    byte ptr [esi], dl
        add    byte ptr [edi], dl
        add    byte ptr [eax + 0x1f], dl     ; -> loc_3_1F50
        add    byte ptr [eax], al
        pop    ss
        add    byte ptr [esi], dl
        add    byte ptr [eax], al            ; -> loc_3_2000
        and    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [ecx], bl
        add    byte ptr [eax + 0x21], dh     ; -> loc_3_2170
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        sbb    byte ptr [eax], al
        and    byte ptr [ecx], 0
        add    byte ptr [edx], bl
        add    byte ptr [ebx], bl
        add    byte ptr [eax], ah            ; -> loc_3_2820
        sub    byte ptr [eax], al
        add    byte ptr [ebx], bl
        add    byte ptr [edx], bl
        add    byte ptr [eax], dl            ; -> loc_3_2910
        sub    dword ptr [eax], eax
        add    byte ptr [eax + eax], bl
        sbb    eax, 0x2ec000                 ; -> loc_3_2EC0
        add    byte ptr [0xe0001c00], bl     ; -> loc_3_2EE0
        add    byte ptr cs:[eax], al
        push   ds
        sbb    byte ptr [edi], 0
        rcl    byte ptr [edi - 0x7fe10000], 0x1e
        add    byte ptr [eax], dl            ; -> loc_3_9810
        cwde   
        add    byte ptr [eax], al
        and    byte ptr [eax - 0x6fcfffdf], al ; -> loc_3_9030
        add    byte ptr [eax], al
        and    dword ptr [eax - 0x6f7fffe0], eax ; -> loc_3_9080
        add    byte ptr [eax], al
        and    al, byte ptr [eax - 0x690ffffe] ; -> loc_3_96F0
        add    byte ptr [eax], al
        and    eax, dword ptr [eax - 0x71dfffdc] ; -> loc_3_8E20
        add    byte ptr [eax], al
        and    al, 0x80
        add    al, byte ptr [eax]
        jo     0xa166
        add    byte ptr [eax], al
        and    eax, 0xf0002680               ; -> loc_3_96F0
        xchg   esi, eax
        add    byte ptr [eax], al
        and    byte ptr es:[edi], 0
        lock xchg dword ptr [eax], eax
        add    byte ptr [edi], ah
        sub    byte ptr [eax], 0
        and    byte ptr [esi - 0x7fd80000], cl
        add    al, byte ptr [eax]
        jo     0xa186
        add    byte ptr [eax], al
        sub    dword ptr [eax], eax
        sub    al, byte ptr [eax]
        push   eax
        cmp    al, 0
        add    byte ptr [edx], ch
        add    byte ptr [ecx], al
        add    al, al                        ; -> loc_3_3CC0
        cmp    al, 0
        add    byte ptr [ebx], ch
        add    byte ptr [eax + eax], ch
        cmp    byte ptr [0x2c0000], 1
        add    al, dl                        ; -> loc_3_3DD0
        cmp    eax, 0x2d0000
        add    byte ptr cs:[eax + 0x3e], ah  ; -> loc_3_3E60
        add    byte ptr [eax], al
        add    byte ptr cs:[ecx], al
        add    byte ptr [eax + 0x2f00007f], dh ; -> loc_3_7FB0
        add    byte ptr [eax], dh
        add    byte ptr [eax + 0x3000004a], al ; -> sub_3_4A80
        add    byte ptr [ecx], al
        add    al, al                        ; -> loc_3_4BC0
        dec    ebx
        add    byte ptr [eax], al
        xor    dword ptr [eax], eax
        xor    al, byte ptr [eax]
        mov    al, 0x3e
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    dword ptr [eax], eax
        add    byte ptr [ecx], 0
        xor    eax, dword ptr [eax]
        add    dword ptr [eax], eax
        loopne 0xa28b
        add    byte ptr [eax], al
        xor    al, 0
        add    dword ptr [eax], eax
        inc    eax
        inc    edx
        add    byte ptr [eax], al
        xor    eax, 0x70000100               ; -> loc_3_4270
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr ss:[edi], dh
        add    byte ptr [eax], al            ; -> loc_3_4D00
        dec    ebp
        add    byte ptr [eax], al
        aaa    
        add    byte ptr [ecx], al
        add    byte ptr [eax + 0x3800004e], dl ; -> loc_3_4E90
        add    byte ptr [ecx], bh
        add    al, dl                        ; -> loc_3_5CD0
        pop    esp
        add    byte ptr [eax], al
        cmp    dword ptr [eax], eax
        cmp    al, byte ptr [eax]
        nop    
        pop    edi
        add    byte ptr [eax], al
        cmp    al, byte ptr [eax]
        cmp    dword ptr [eax], eax
        nop    
        popal  
        add    byte ptr [eax], al
        cmp    eax, dword ptr [eax]
        add    dword ptr [eax], eax
        xor    byte ptr [ebx], bl
        add    byte ptr [eax], al
        cmp    al, 0
        cmp    eax, 0x1b8000                 ; -> loc_3_1B80
        add    byte ptr [0x20000100], bh     ; -> loc_3_1C20
        sbb    al, 0
        add    byte ptr [esi], bh
        add    byte ptr [edi], bh
        add    byte ptr [eax + 0x3f000015], ah ; -> loc_3_15A0
        add    byte ptr [ecx], al
        add    al, al                        ; -> loc_3_15C0
        adc    eax, 0x400000
        inc    ecx
        add    byte ptr [eax], dh            ; -> loc_3_0E30
        push   cs
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [ecx], al
        add    al, ah                        ; -> loc_3_0EE0
        push   cs
        add    byte ptr [eax], al
        inc    edx
        add    byte ptr [ebx], al
        adc    byte ptr [esi], ah
        add    byte ptr [ebx], al
        inc    edx
        add    byte ptr [eax + 0x66], dh     ; -> loc_3_6670
        add    byte ptr [eax], al
        inc    esp
        add    byte ptr [ebp], al
        mov    al, byte ptr [0x45000016]
        add    byte ptr [ecx], al
        add    byte ptr [eax + 0x46000017], ah ; -> loc_3_17A0
        add    byte ptr [edi], 0xd0          ; -> loc_3_85D0
        test   dword ptr [eax], eax
        add    byte ptr [edi - 0x80], al
        inc    esi
        add    byte ptr [eax], dh            ; -> loc_3_8630
        xchg   byte ptr [eax], al
        add    byte ptr [eax - 0x80], cl
        dec    ecx
        add    byte ptr [eax], ah            ; -> loc_3_6D20
        insd   dword ptr es:[edi], dx
        add    byte ptr [eax], al
        dec    ecx
        or     byte ptr [eax], 0x40          ; -> loc_3_6F40
        outsd  dx, dword ptr [esi]
        add    byte ptr [eax], al
        dec    edx
        or     byte ptr [ebx], 0             ; -> loc_3_6200
        bound  eax, qword ptr [eax]
        add    byte ptr [ebx - 0x80], cl
        add    al, byte ptr [eax]
        mov    al, byte ptr [0x4c000063]
        add    byte ptr [ecx], al
        add    al, al                        ; -> loc_3_1AC0
        sbb    al, byte ptr [eax]
        add    byte ptr [ebp], cl
        add    dword ptr [eax], eax
        dd  loc_3_42F0               ; fixup
        db  04Eh, 000h, 04Fh, 000h
        dd  loc_3_4A60               ; fixup
        db  04Fh, 000h, 001h, 000h
        dd  loc_3_4E90               ; fixup
        db  050h, 000h, 001h, 000h
        dd  loc_3_3E10               ; fixup
loc_3_A334:
        inc    ebx
        inc    esp
        push   edx
        dec    edi
        dec    ebp
        add    byte ptr [eax], al
        db  000h
loc_3_A33C:
        inc    ebx
        inc    esp
        xor    byte ptr [eax], dh
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_A344:
        inc    ebx
        inc    esp
        push   edx
        dec    edi
        dec    ebp
        add    byte ptr [eax], al
        db  000h
loc_3_A34C:
        inc    ebx
        inc    esp
        xor    byte ptr [eax], dh
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_3_A354:
        inc    ebx
        inc    esp
        sub    eax, 0x30304158
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xdc  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  sub_4_00A1               ; fixup
        dd  loc_4_00BD               ; fixup
        dd  loc_4_00D6               ; fixup
sub_4_0018:
        push   eax
        jo     0x1b
        db  000h
sub_4_001C:
        rol    byte ptr [ebx], 1
        add    byte ptr [eax], al
loc_4_0020:
        test   dword ptr [4], 1              ; -> obj4:0x0004
        je     loc_4_003E
        test   dword ptr [4], 0x10           ; -> obj4:0x0004
        jne    loc_4_003E
        call   sub_4_0018
loc_4_003E:
        and    dword ptr [4], 0xffffffcf     ; -> obj4:0x0004
        ret    
loc_4_0046:
        mov    ecx, dword ptr [0]            ; -> obj4:0x0000
        jecxz  loc_4_006D
        mov    eax, dword ptr [ecx]
        cmp    ecx, dword ptr [0]            ; -> obj4:0x0000
        jne    loc_4_0046
        mov    dword ptr [0], eax            ; -> obj4:0x0000
        push   dword ptr [ecx + 4]
        push   ecx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        lea    esp, [esp + 8]
        jmp    loc_4_0046
loc_4_006D:
        and    dword ptr [4], 0xfffffeff     ; -> obj4:0x0004
        ret    
loc_4_0078:
        test   dword ptr [4], 1              ; -> obj4:0x0004
        je     loc_4_0096
        test   dword ptr [4], 0x1000         ; -> obj4:0x0004
        jne    loc_4_0096
        call   sub_4_001C
loc_4_0096:
        and    dword ptr [4], 0xffffcfff     ; -> obj4:0x0004
        ret    
sub_4_00A1:
        mov    eax, ecx
        shr    ecx, 2
        stc    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        ret    
loc_4_00B1:
        xor    ecx, ecx
        movsb  byte ptr es:[edi], byte ptr [esi]
        inc    eax
loc_4_00B5:
        xor    ecx, ecx
        movsb  byte ptr es:[edi], byte ptr [esi]
        inc    eax
loc_4_00B9:
        xor    ecx, ecx
        movsb  byte ptr es:[edi], byte ptr [esi]
        inc    eax
loc_4_00BD:
        stc    
        ret    
loc_4_00BF:
        push   eax
        shr    eax, 2
        sub    eax, ecx
        shl    eax, 2
        pop    ecx
        sub    ecx, eax
        cmp    ecx, 2
        jecxz  loc_4_00BD
        jb     loc_4_00B9
        je     loc_4_00B5
        jmp    loc_4_00B1
loc_4_00D6:
        jb     loc_4_00BF
        sub    eax, ecx
        stc    
        ret    
VxD__LTEXT ENDS

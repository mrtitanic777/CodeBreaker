; DECLICL -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 7074 instructions recovered by recursive descent

Declare_Virtual_Device DECLICL, 3, 10, DECLICL_Control, 033B4h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x84  R-X
loc_1_0000:
        cmp    dword ptr [0x23608], 0        ; -> obj2:0x3608
        jne    loc_1_0075
        mov    dword ptr [0x23604], 0        ; -> obj2:0x3604
        cmp    eax, 0x1b
        jne    loc_1_0022
        mov    dword ptr [0x23604], 1        ; -> obj2:0x3604
loc_1_0022:
        mov    dword ptr [0x23600], ebx      ; -> obj2:0x3600
        mov    edx, 0x235a4                  ; -> obj2:0x35A4
        mov    dword ptr [0x235fc], edx      ; -> obj2:0x35FC
        mov    edx, dword ptr [edx + 0x2c]
        mov    dword ptr [0x235f8], edx      ; -> obj2:0x35F8
        xor    ecx, ecx
        VxDCall  Dev_33AA.Service_0000h          ; cd200000aa33
        or     ecx, ecx
        jne    loc_1_0050
        VxDCall  Dev_6900.Service_0000h          ; cd2000000069
        jecxz  loc_1_0082
loc_1_0050:
        mov    dword ptr [0x235f4], ecx      ; -> obj2:0x35F4
        push   dword ptr [0x23604]           ; -> obj2:0x3604
        push   ebx
        push   dword ptr [0x235f8]           ; -> obj2:0x35F8
        push   dword ptr [0x235f4]           ; -> obj2:0x35F4
        call   sub_2_0190
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0082
loc_1_0075:
        mov    dword ptr [0x23608], 1        ; -> obj2:0x3608
        xor    eax, eax
        ret    
loc_1_0082:
        stc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x3610  R-X
DECLICL_Control:
        cmp    eax, 1
        je     loc_1_0000
        cmp    eax, 0x1b
        je     loc_1_0000
        cmp    eax, 0x16
        je     loc_2_0023
        cmp    eax, 0x1c
        je     loc_2_0023
        cmp    eax, 0x14
        je     loc_2_0036
        clc    
        ret    
loc_2_0023:
        cmp    ebx, dword ptr [0x23600]      ; -> obj2:0x3600
        jne    loc_2_0034
        push   ebx
        call   sub_2_01B0
        add    esp, 4
loc_2_0034:
        clc    
        ret    
loc_2_0036:
        call   sub_2_01C0
        clc    
        ret    
        db  000h, 000h, 000h
sub_2_0040:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        cld    
        cmp    ecx, 8
        jb     loc_2_007F
        mov    eax, esi
        and    eax, 3
        jmp    dword ptr [eax*4 + 0x20091]   ; -> obj2:0x0091
loc_2_0060:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_0061:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_0062:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_0063:
        sub    ecx, eax
        xor    eax, eax
        shr    ecx, 1
        rcl    eax, 1
        shr    ecx, 1
        rcl    eax, 1
        rep movsd dword ptr es:[edi], dword ptr [esi]
        jmp    dword ptr [eax*4 + 0x200a1]   ; -> obj2:0x00A1
loc_2_0078:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_0079:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_007A:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_007B:
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_2_007F:
        jmp    dword ptr [ecx*4 + 0x200b1]   ; -> obj2:0x00B1
loc_2_0086:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_0087:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_0088:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_0089:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_008A:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_008B:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_008C:
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_2_008D:
        pop    edi
        pop    esi
        pop    ebp
        ret    
        ; ---- dispatch table (16 entries) ----
        dd  loc_2_0063
        dd  loc_2_0062
        dd  loc_2_0061
        dd  loc_2_0060
        dd  loc_2_007B
        dd  loc_2_0079
        dd  loc_2_007A
        dd  loc_2_0078
        dd  loc_2_008D
        dd  loc_2_008C
        dd  loc_2_008B
        dd  loc_2_008A
        dd  loc_2_0089
        dd  loc_2_0088
        dd  loc_2_0087
        dd  loc_2_0086
sub_2_00D1:
        push   ebp
        mov    ebp, esp
        push   edx
        push   edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ah, cl
        mov    al, cl
        shl    eax, 0x10
        mov    al, cl
        mov    ah, cl
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        cld    
        cmp    ecx, 8
        jb     loc_2_011B
        mov    edx, edi
        and    edx, 3
        jmp    dword ptr [edx*4 + 0x2012d]   ; -> obj2:0x012D
loc_2_00FC:
        stosb  byte ptr es:[edi], al
loc_2_00FD:
        stosb  byte ptr es:[edi], al
loc_2_00FE:
        stosb  byte ptr es:[edi], al
loc_2_00FF:
        sub    ecx, edx
        xor    edx, edx
        shr    ecx, 1
        rcl    edx, 1
        shr    ecx, 1
        rcl    edx, 1
        rep stosd dword ptr es:[edi], eax
        jmp    dword ptr [edx*4 + 0x2013d]   ; -> obj2:0x013D
loc_2_0114:
        stosb  byte ptr es:[edi], al
loc_2_0115:
        stosb  byte ptr es:[edi], al
loc_2_0116:
        stosb  byte ptr es:[edi], al
loc_2_0117:
        pop    edi
        pop    edx
        pop    ebp
        ret    
loc_2_011B:
        jmp    dword ptr [ecx*4 + 0x2014d]   ; -> obj2:0x014D
loc_2_0122:
        stosb  byte ptr es:[edi], al
loc_2_0123:
        stosb  byte ptr es:[edi], al
loc_2_0124:
        stosb  byte ptr es:[edi], al
loc_2_0125:
        stosb  byte ptr es:[edi], al
loc_2_0126:
        stosb  byte ptr es:[edi], al
loc_2_0127:
        stosb  byte ptr es:[edi], al
loc_2_0128:
        stosb  byte ptr es:[edi], al
loc_2_0129:
        pop    edi
        pop    edx
        pop    ebp
        ret    
        ; ---- dispatch table (16 entries) ----
        dd  loc_2_00FF
        dd  loc_2_00FE
        dd  loc_2_00FD
        dd  loc_2_00FC
        dd  loc_2_0117
        dd  loc_2_0115
        dd  loc_2_0116
        dd  loc_2_0114
        dd  loc_2_0129
        dd  loc_2_0128
        dd  loc_2_0127
        dd  loc_2_0126
        dd  loc_2_0125
        dd  loc_2_0124
        dd  loc_2_0123
        dd  loc_2_0122
sub_2_016D:
        enter  0, 0
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        sete   al
        movzx  eax, al
        pop    edi
        pop    esi
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0190:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x404f4], eax      ; -> sub_4_04F4
        call   sub_2_0EC0
        xor    eax, eax
        pop    ebp
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h
sub_2_01B0:
        call   sub_2_0510
        xor    eax, eax
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
sub_2_01C0:
        push   0x40000                       ; -> obj4:0x0000
        push   0x300
        call   sub_4_04F4
        add    esp, 8
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_01E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 4], 1
        mov    dl, byte ptr [ebp + 8]
        mov    byte ptr [eax + 5], 1
        mov    byte ptr [eax + 6], dl
        mov    dl, byte ptr [ebp + 0xc]
        mov    byte ptr [eax + 7], dl
        mov    dl, byte ptr [ebp + 0x1c]
        mov    byte ptr [eax + 8], dl
        mov    dl, byte ptr [0x41f18]        ; -> loc_4_1F18
        mov    byte ptr [eax + 9], dl
        mov    dl, byte ptr [ebp + 0x14]
        mov    byte ptr [eax + 0xa], dl
        mov    dl, byte ptr [ebp + 0x10]
        mov    byte ptr [eax + 0xb], dl
        pop    ebp
        ret    
        db  08Dh, 040h, 000h
sub_2_0220:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_2_0245
        mov    edx, dword ptr [0x41e6c]      ; -> obj4:0x1E6C
        push   edx
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        add    eax, 0xc
        push   eax
        call   sub_2_0040
        add    esp, 0xc
loc_2_0245:
        mov    ebx, dword ptr [ebp + 0x1c]
        xor    eax, eax
        push   ebx
        mov    al, byte ptr [ebp + 0x18]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        push   eax
        push   0
        xor    eax, eax
        push   0
        mov    al, byte ptr [ebp + 0xc]
        push   eax
        push   1
        call   sub_2_01E0
        add    esp, 0x1c
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 044h, 020h, 000h
sub_2_0270:
        push   ebp
        mov    ebp, esp
        mov    dh, byte ptr [ebp + 8]
        cmp    dh, byte ptr [0x41f18]        ; -> loc_4_1F18
        jae    loc_2_029B
        mov    eax, dword ptr [0x41f2c]      ; -> loc_4_1F2C
        xor    dl, dl
        test   dh, dh
        ja     loc_2_0297
        pop    ebp
        ret    
loc_2_028B:
        mov    eax, dword ptr [eax]
        test   eax, eax
        je     loc_2_029B
        inc    dl
        cmp    dl, dh
        jae    loc_2_029B
loc_2_0297:
        test   eax, eax
        jne    loc_2_028B
loc_2_029B:
        pop    ebp
        ret    
        db  08Dh, 040h, 000h
sub_2_02A0:
        push   esi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 0xc]
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        call   sub_2_0270
        add    esp, 4
        mov    edx, dword ptr [ebx + 4]
        push   edx
        xor    edx, edx
        mov    dl, byte ptr [esi + 8]
        push   edx
        xor    edx, edx
        mov    dl, byte ptr [0x41e69]        ; -> obj4:0x1E69
        push   edx
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 7]
        push   eax
        call   sub_2_0220
        add    esp, 0x14
        push   0xa6
        push   ebx
        call   sub_2_05C0
        add    esp, 8
        pop    ebp
        pop    esi
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
sub_2_02F0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        mov    dl, byte ptr [eax + 4]
        cmp    dl, 1
        jne    loc_2_031B
        cmp    dl, byte ptr [eax + 5]
        jne    loc_2_031B
        cmp    byte ptr [eax + 6], 0
        jne    loc_2_031B
        push   eax
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_2_02A0
        add    esp, 4
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret    
loc_2_031B:
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0330:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x18]
        mov    edx, esi
        xor    ebx, ebx
        shr    edx, 2
        and    esi, 3
loc_2_0346:
        mov    ecx, edx
        dec    edx
        test   ecx, ecx
        je     loc_2_0358
        mov    ecx, eax
        mov    edi, dword ptr [ecx]
        add    eax, 4
        add    ebx, edi
        jmp    loc_2_0346
loc_2_0358:
        cmp    esi, 2
        jb     loc_2_0366
        jbe    loc_2_0374
        cmp    esi, 3
        je     loc_2_037D
        jmp    loc_2_0386
loc_2_0366:
        cmp    esi, 1
        jne    loc_2_0386
        mov    eax, dword ptr [eax]
        and    eax, 0xff
        jmp    loc_2_0384
loc_2_0374:
        mov    eax, dword ptr [eax]
        and    eax, 0xffff
        jmp    loc_2_0384
loc_2_037D:
        mov    eax, dword ptr [eax]
        and    eax, 0xffffff
loc_2_0384:
        add    ebx, eax
loc_2_0386:
        mov    eax, ebx
        neg    eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
sub_2_0390:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    edi, dword ptr [ebp + 0x20]
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x1c]
        push   eax
        mov    edx, dword ptr [ebp + 0x18]
        push   edx
        mov    ebx, dword ptr [ebp + 0x14]
        push   ebx
        xor    esi, esi
        call   sub_2_0040
        add    esp, 0xc
        mov    ecx, ebx
        jmp    loc_2_03C0
loc_2_03B5:
        mov    eax, dword ptr [ecx]
        add    ecx, 4
        xor    eax, edi
        inc    esi
        mov    dword ptr [ecx - 4], eax
loc_2_03C0:
        xor    ebx, ebx
        mov    bl, byte ptr [ebp + 0x1c]
        mov    edx, ebx
        mov    eax, ebx
        sar    edx, 0x1f
        shl    edx, 2
        sbb    eax, edx
        sar    eax, 2
        cmp    esi, eax
        jl     loc_2_03B5
        mov    edx, dword ptr [ebp + 0x14]
        mov    al, byte ptr [ebp + 0x1c]
        add    ebx, edx
        add    al, 4
        mov    dword ptr [ebx], edi
        mov    byte ptr [edx + 0x36], al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 044h, 020h, 000h
sub_2_03F0:
        push   ebx
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    edi, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0x1c]
        add    edx, 4
        sub    eax, 4
        je     loc_2_0440
loc_2_0409:
        xor    bl, bl
        mov    bh, byte ptr [ebp + 0x14]
        mov    byte ptr [ebp - 4], bl
        test   bh, bh
        ja     loc_2_0438
        jmp    loc_2_043C
loc_2_0417:
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    ebx, edx
        mov    cl, byte ptr [ecx + edi]
        mov    ch, byte ptr [ebx]
        xor    ch, cl
        dec    eax
        mov    byte ptr [ebx], ch
        mov    bl, byte ptr [ebp - 4]
        inc    edx
        inc    bl
        mov    bh, byte ptr [ebp + 0x14]
        mov    byte ptr [ebp - 4], bl
        cmp    bl, bh
        jae    loc_2_043C
loc_2_0438:
        test   eax, eax
        jne    loc_2_0417
loc_2_043C:
        test   eax, eax
        jne    loc_2_0409
loc_2_0440:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_0450:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [ebp + 0x24]
        push   edx
        push   ebx
        mov    dword ptr [ebx], 0
        call   sub_2_0330
        add    esp, 8
        mov    dword ptr [ebx], eax
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x1c]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        push   esi
        call   sub_2_0390
        add    esp, 0x10
        mov    edi, dword ptr [ebp + 0x24]
        push   edi
        xor    eax, eax
        push   ebx
        mov    al, byte ptr [esi + 0x36]
        push   eax
        push   esi
        call   sub_2_03F0
        add    esp, 0x10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0C0h
sub_2_04A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ebx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x20]
        push   4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_0040
        add    esp, 0xc
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        push   edx
        mov    al, byte ptr [ebp + 0x1c]
        push   eax
        mov    ecx, dword ptr [ebp + 0x18]
        push   ecx
        push   ebx
        call   sub_2_0390
        add    esp, 0x10
        mov    edi, dword ptr [ebp + 0x24]
        push   edi
        xor    eax, eax
        push   esi
        mov    al, byte ptr [ebx + 0x36]
        push   eax
        push   ebx
        call   sub_2_03F0
        add    esp, 0x10
        push   edi
        push   esi
        call   sub_2_0330
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        setne  al
        and    eax, 0xff
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0510:
        push   ebx
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ebx, dword ptr [0x42478]      ; -> obj4:0x2478
        xor    ah, ah
        mov    dl, byte ptr [0x4240c]        ; -> obj4:0x240C
        mov    byte ptr [ebp - 4], ah
        test   dl, dl
        jbe    loc_2_0577
loc_2_052C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        shl    eax, 6
        mov    dh, 0x35
        add    eax, ebx
        mov    byte ptr [0x42420], dh        ; -> obj4:0x2420
        mov    dl, byte ptr [eax + 0x30]
        push   0x42420                       ; -> obj4:0x2420
        mov    byte ptr [0x42450], dl        ; -> obj4:0x2450
        mov    dword ptr [0x42424], eax      ; -> obj4:0x2424
        call   sub_2_0990
        add    esp, 4
        push   ebx
        mov    byte ptr [ebx], 0x31
        call   sub_2_0990
        mov    cl, byte ptr [ebp - 4]
        mov    ch, byte ptr [0x4240c]        ; -> obj4:0x240C
        inc    cl
        add    esp, 4
        mov    byte ptr [ebp - 4], cl
        cmp    cl, ch
        jb     loc_2_052C
loc_2_0577:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    ebx
        ret    
        db  08Bh, 0C0h
loc_2_0580:
        mov    al, byte ptr [ebx + 1]
        cmp    al, 0xb
        jb     loc_2_058F
        jbe    loc_2_05BD
        cmp    al, 0x17
        je     loc_2_05BD
        jmp    loc_2_05AA
loc_2_058F:
        test   al, al
        jne    loc_2_05AA
        mov    byte ptr [ebx], 0x95
        mov    dword ptr [ebx + 0x2c], 0x20600 ; -> loc_2_0600
        mov    word ptr [ebx + 8], 0xa6
        push   ebx
        mov    byte ptr [ebx + 0x2a], 0x14
        jmp    loc_2_05B5
loc_2_05AA:
        mov    byte ptr [ebx], 0x92
        push   ebx
        mov    dword ptr [ebx + 0x2c], 0x206b0 ; -> loc_2_06B0
loc_2_05B5:
        call   sub_2_0990
        add    esp, 4
loc_2_05BD:
        ret    
        db  08Bh, 0C0h
sub_2_05C0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        push   0x10
        lea    eax, [ebx + 0x1a]
        push   eax
        push   0x423d2                       ; -> obj4:0x23D2
        call   sub_2_0450
        add    esp, 0x14
        mov    dword ptr [ebx + 0x2c], 0x20670 ; -> loc_2_0670
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [ebx], 0x94
        push   ebx
        mov    word ptr [ebx + 8], ax
        call   sub_2_0990
        add    esp, 4
        pop    ebp
        pop    ebx
        ret    
        db  090h
loc_2_0600:
        mov    al, byte ptr [ebx + 1]
        cmp    al, 6
        jb     loc_2_0615
        jbe    loc_2_0649
        cmp    al, 0xb
        jb     loc_2_0655
        jbe    loc_2_066C
        cmp    al, 0xff
        je     loc_2_0649
        jmp    loc_2_0655
loc_2_0615:
        test   al, al
        jne    loc_2_0655
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        push   eax
        mov    edx, dword ptr [ebx + 4]
        push   edx
        push   0x10
        lea    eax, [ebx + 0x1a]
        push   eax
        push   0x423d2                       ; -> obj4:0x23D2
        call   sub_2_04A0
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0649
        mov    ecx, dword ptr [ebx + 4]
        push   ecx
        push   ebx
        call   sub_2_02F0
        add    esp, 8
        ret    
loc_2_0649:
        mov    byte ptr [ebx], 0x95
        push   ebx
        mov    word ptr [ebx + 8], 0xa6
        jmp    loc_2_0664
loc_2_0655:
        mov    byte ptr [ebx], 0x92
        mov    dword ptr [ebx + 0x2c], 0x206b0 ; -> loc_2_06B0
        push   ebx
        mov    byte ptr [ebx + 0xa], 0x2a
loc_2_0664:
        call   sub_2_0990
        add    esp, 4
loc_2_066C:
        ret    
        db  08Dh, 040h, 000h
loc_2_0670:
        mov    al, byte ptr [ebx + 1]
        cmp    al, 0xa
        jb     loc_2_0690
        jbe    loc_2_067F
        cmp    al, 0xb
        je     loc_2_06A9
        jmp    loc_2_0690
loc_2_067F:
        mov    byte ptr [ebx], 0x92
        mov    dword ptr [ebx + 0x2c], 0x206b0 ; -> loc_2_06B0
        push   ebx
        mov    byte ptr [ebx + 0xa], 0x2a
        jmp    loc_2_06A1
loc_2_0690:
        mov    dword ptr [ebx + 0x2c], 0x20600 ; -> loc_2_0600
        mov    byte ptr [ebx], 0x95
        push   ebx
        mov    word ptr [ebx + 8], 0x156
loc_2_06A1:
        call   sub_2_0990
        add    esp, 4
loc_2_06A9:
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h
loc_2_06B0:
        mov    byte ptr [ebx], 0x91
        mov    dword ptr [ebx + 0x2c], 0x20580 ; -> loc_2_0580
        push   ebx
        mov    byte ptr [ebx + 0xa], 0x2a
        call   sub_2_0990
        add    esp, 4
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
sub_2_06D0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x42478]      ; -> obj4:0x2478
        test   ebx, ebx
        jne    loc_2_06E6
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_06E6:
        push   0x42410                       ; -> obj4:0x2410
        push   0x52
        xor    esi, esi
        call   sub_2_0BB0
        mov    edx, dword ptr [0x42610]      ; -> obj4:0x2610
        add    esp, 8
        test   edx, edx
        jle    loc_2_0781
        lea    edi, [ebx + 0x1a]
loc_2_0708:
        push   0x40
        push   0
        push   ebx
        call   sub_2_00D1
        add    esp, 0xc
        push   0x10
        push   0x42410                       ; -> obj4:0x2410
        mov    al, byte ptr [esi + 0x42614]  ; -> obj4:0x2614
        push   edi
        mov    byte ptr [ebx + 0x30], al
        call   sub_2_0040
        add    esp, 0xc
        push   ebx
        mov    byte ptr [ebx], 0x30
        call   sub_2_0990
        mov    ah, byte ptr [ebx + 3]
        add    esp, 4
        test   ah, ah
        jne    loc_2_074A
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_074A:
        mov    dword ptr [ebx + 4], 0x4232c  ; -> obj4:0x232C
        mov    byte ptr [ebx + 0xa], 0x2a
        mov    byte ptr [ebx + 0x2a], 0
        mov    byte ptr [ebx + 0x2b], 0
        mov    byte ptr [ebx], 0x91
        push   ebx
        mov    dword ptr [ebx + 0x2c], 0x20580 ; -> loc_2_0580
        call   sub_2_0990
        add    edi, 0x40
        inc    esi
        mov    ecx, dword ptr [0x42610]      ; -> obj4:0x2610
        add    esp, 4
        add    ebx, 0x40
        cmp    esi, ecx
        jl     loc_2_0708
loc_2_0781:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0790:
        enter  0x207, 0
        aad    7
        add    al, byte ptr [eax]
        loop   loc_2_07A1
        add    al, byte ptr [eax]
        out    dx, eax
        pop    es
        add    al, byte ptr [eax]
        push   ebp
loc_2_07A1:
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0
        mov    byte ptr [eax + 1], 1
        mov    byte ptr [eax + 2], 1
        cmp    edx, 3
        jne    loc_2_080C
        test   edx, edx
        jne    loc_2_07FC
        dec    edx
        cmp    edx, 3
        ja     loc_2_080C
        jmp    loc_2_0790
loc_2_07C8:
        push   eax
        call   sub_2_0820
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret    
loc_2_07D5:
        push   eax
        call   sub_2_0810
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret    
loc_2_07E2:
        push   eax
        call   sub_2_0830
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret    
loc_2_07EF:
        push   eax
        call   sub_2_0840
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret    
loc_2_07FC:
        push   eax
        call   sub_2_0810
        mov    eax, 1
        add    esp, 4
        pop    ebp
        ret    
loc_2_080C:
        xor    eax, eax
        pop    ebp
        ret    
sub_2_0810:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3], 1
        pop    ebp
        ret    
        db  08Dh, 044h, 020h, 000h
sub_2_0820:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3], 0
        pop    ebp
        ret    
        db  08Dh, 044h, 020h, 000h
sub_2_0830:
        push   ebp
        mov    ebp, esp
        call   sub_2_09C0
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 3], al
        pop    ebp
        ret    
sub_2_0840:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0xc]
        xor    eax, eax
        mov    al, byte ptr [ebx + 0xc]
        push   eax
        call   sub_2_0270
        add    esp, 4
        mov    edx, eax
        test   eax, eax
        sete   al
        mov    byte ptr [ebx + 3], al
        lea    eax, [ebx + 4]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 0xc]
        push   eax
        push   2
        xor    eax, eax
        push   edx
        mov    al, byte ptr [ebx + 0xb]
        push   eax
        call   sub_2_0220
        add    esp, 0x14
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 040h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Ah, 050h, 001h, 080h, 0FAh, 001h
        db  075h, 006h, 080h, 078h, 002h, 000h, 074h, 007h, 0C6h, 040h, 003h, 001h, 05Dh, 05Bh, 0C3h, 08Ah
        db  058h, 008h, 038h, 0D3h, 075h, 005h, 03Ah, 050h, 009h, 074h, 007h, 0C6h, 040h, 003h, 001h, 05Dh
        db  05Bh, 0C3h, 08Ah, 010h, 080h, 0FAh, 006h, 072h, 006h, 088h, 058h, 003h, 05Dh, 05Bh, 0C3h, 081h
        db  0E2h, 0FFh, 000h, 000h, 000h, 050h, 0FFh, 014h, 095h
        dd  obj4:0x1EE8              ; fixup
        db  083h, 0C4h, 004h, 05Dh, 05Bh, 0C3h
sub_2_08D0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    edi, dword ptr [ebp + 0x14]
        lea    esi, [edi*4]
        mov    ebx, dword ptr [ebp + 0x18]
        add    esi, edi
        lea    eax, [ebx*8]
        shl    esi, 4
        add    eax, ebx
        mov    ebx, dword ptr [ebp + 0x1c]
        add    esi, edi
        lea    eax, [eax*8]
        add    esi, esi
        shl    ebx, 6
        add    eax, esi
        add    eax, ebx
        cmp    eax, 0xffff
        jle    loc_2_0917
        mov    eax, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_0917:
        push   eax
        call   sub_2_2BE0
        add    esp, 4
        mov    ebx, eax
        test   eax, eax
        jne    loc_2_0930
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_0930:
        mov    eax, edi
        mov    esi, eax
        dec    eax
        test   esi, esi
        jne    loc_2_093E
        mov    esi, dword ptr [ebp + 0x18]
        jmp    loc_2_097E
loc_2_093E:
        mov    esi, dword ptr [0x41f38]      ; -> loc_4_1F38
        mov    dword ptr [ebx], esi
        mov    dword ptr [0x41f38], ebx      ; -> loc_4_1F38
        add    ebx, 0xa2
        mov    esi, eax
        dec    eax
        test   esi, esi
        jne    loc_2_093E
        mov    esi, dword ptr [ebp + 0x18]
        jmp    loc_2_097E
loc_2_095E:
        push   0x48
        push   0
        push   ebx
        mov    edi, ebx
        call   sub_2_00D1
        mov    eax, dword ptr [0x41f40]      ; -> loc_4_1F40
        add    ebx, 0x48
        mov    dword ptr [edi + 0x44], eax
        add    esp, 0xc
        mov    dword ptr [0x41f40], edi      ; -> loc_4_1F40
loc_2_097E:
        mov    eax, esi
        dec    esi
        test   eax, eax
        jne    loc_2_095E
        mov    dword ptr [0x42478], ebx      ; -> obj4:0x2478
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_0990:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_4_2004
        mov    ah, byte ptr [esi + 1]
        add    esp, 4
        cmp    ah, 0x23
        jne    loc_2_09AF
        cmp    ah, byte ptr [esi + 0x31]
        je     loc_2_09B5
loc_2_09AF:
        xor    al, al
        pop    ebp
        pop    esi
        pop    ebx
        ret    
loc_2_09B5:
        mov    al, 1
        pop    ebp
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 040h, 000h, 08Bh, 0C9h
sub_2_09C0:
        push   ebx
        mov    edx, dword ptr [0x41f2c]      ; -> loc_4_1F2C
        mov    eax, edx
        test   edx, edx
        je     loc_2_09E2
loc_2_09CD:
        mov    ebx, dword ptr [0x41f38]      ; -> loc_4_1F38
        mov    edx, dword ptr [eax]
        mov    dword ptr [eax], ebx
        mov    dword ptr [0x41f38], eax      ; -> loc_4_1F38
        mov    eax, edx
        test   edx, edx
        jne    loc_2_09CD
loc_2_09E2:
        xor    edx, edx
        xor    ah, ah
        xor    al, al
        mov    dword ptr [0x41f2c], edx      ; -> loc_4_1F2C
        mov    byte ptr [0x41f18], ah        ; -> loc_4_1F18
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_0A00:
        push   ebp
        mov    ebp, esp
        push   8
        call   sub_2_0E50
        add    esp, 4
        mov    byte ptr [0x4240c], al        ; -> obj4:0x240C
        xor    al, al
        pop    ebp
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0A20:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        mov    esi, dword ptr [ebp + 0x14]
        push   0xa
        lea    eax, [ebp - 8]
        xor    edx, edx
        push   eax
        mov    edi, 0x10
        mov    dword ptr [ebp - 8], edx
        push   edx
        mov    dword ptr [ebp - 0xc], edx
        mov    dword ptr [ebp - 0x10], edi
        call   sub_4_04F4
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, 0x40018                  ; -> obj4:0x0018
        mov    dx, word ptr [eax + 2]
        add    esp, 0xc
        cmp    dx, 0x30b
        ja     loc_2_0AB9
        test   byte ptr [eax + 4], 8
        je     loc_2_0AB9
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   0x40054                       ; -> obj4:0x0054
        push   0x40064                       ; -> obj4:0x0064
        push   0x10000001
        call   sub_4_04F4
        add    esp, 0x14
        test   eax, eax
        je     loc_2_0A91
        mov    eax, 1
        jmp    loc_2_0B36
loc_2_0A91:
        xor    bl, bl
        lea    eax, [eax]
        lea    edx, [edx]
        nop    
loc_2_0AA0:
        xor    eax, eax
        mov    edx, dword ptr [ebp - 4]
        mov    al, bl
        mov    dl, byte ptr [edx + eax]
        inc    bl
        mov    byte ptr [esi + eax], dl
        cmp    bl, 0x10
        jb     loc_2_0AA0
        jmp    loc_2_0B34
loc_2_0AB9:
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax + 2], 0x400
        jb     loc_2_0B34
        lea    eax, [ebp - 0xc]
        push   eax
        push   ebx
        push   0x80000002
        push   0x40000900
        call   sub_4_04F4
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_0AE6
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_2_0AF2
loc_2_0AE6:
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_0AF2:
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        push   0
        push   0
        push   0x40054                       ; -> obj4:0x0054
        mov    edx, dword ptr [ebp - 0xc]
        push   edx
        push   0x40000901
        call   sub_4_04F4
        add    esp, 0x1c
        test   eax, eax
        je     loc_2_0B22
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_0B22:
        mov    ebx, dword ptr [ebp - 0xc]
        push   ebx
        push   0x40000902
        call   sub_4_04F4
        add    esp, 8
loc_2_0B34:
        xor    eax, eax
loc_2_0B36:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 040h, 000h
sub_2_0B40:
        push   ebx
        push   ebp
        mov    ebp, esp
        push   0xa
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_2_016D
        add    esp, 0xc
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 090h
sub_2_0B60:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0x10]
        mov    edx, ebx
        xor    eax, eax
loc_2_0B6B:
        mov    cl, byte ptr [edx]
        cmp    cl, 0x20
        je     loc_2_0B7D
        test   cl, cl
        je     loc_2_0B7D
        inc    eax
        inc    edx
        cmp    eax, 0x10
        jle    loc_2_0B6B
loc_2_0B7D:
        lea    edx, [eax + ebx]
        cmp    eax, 0x10
        jl     loc_2_0B90
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [ebx + 0xf], al
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret    
loc_2_0B90:
        inc    eax
        mov    byte ptr [edx], 0x20
        inc    edx
        cmp    eax, 0x10
        jl     loc_2_0B90
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [ebx + 0xf], al
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_0BB0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        call   sub_2_0A20
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x10]
        xor    eax, eax
        push   ebx
        mov    al, byte ptr [ebp + 0xc]
        push   eax
        call   sub_2_0B60
        add    esp, 8
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_0BE0:
        push   ebx
        push   ebp
        mov    ebp, esp
        push   4
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_2_2820
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0C13
        mov    eax, dword ptr [ebp + 0x10]
        push   9
        add    eax, 4
        push   eax
        lea    eax, [ebx + 4]
        push   eax
        call   sub_2_2820
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0C18
loc_2_0C13:
        mov    eax, 2
loc_2_0C18:
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 040h, 000h, 08Bh, 0C9h
sub_2_0C20:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    esi, eax
        mov    ebx, dword ptr [0x41f2c]      ; -> loc_4_1F2C
        xor    edx, edx
        xor    edi, edi
        mov    dword ptr [ebp - 4], edx
        test   ebx, ebx
        je     loc_2_0C8B
loc_2_0C3C:
        push   esi
        lea    eax, [ebx + 4]
        push   eax
        call   sub_2_0BE0
        add    esp, 8
        test   eax, eax
        jne    loc_2_0C83
        lea    eax, [ebx + 0x6a]
        push   eax
        lea    eax, [esi + 0x66]
        push   eax
        call   sub_2_0B40
        add    esp, 8
        test   eax, eax
        jne    loc_2_0C7A
        test   byte ptr [ebp + 0x14], 4
        je     loc_2_0D21
        mov    dword ptr [0x41ff4], 1        ; -> loc_4_1FF4
        jmp    loc_2_0D21
loc_2_0C7A:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_0C8B
loc_2_0C83:
        mov    edi, ebx
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    loc_2_0C3C
loc_2_0C8B:
        mov    ecx, dword ptr [0x41f38]      ; -> loc_4_1F38
        test   ecx, ecx
        jne    loc_2_0CAB
        mov    eax, dword ptr [0x41f24]      ; -> loc_4_1F24
        inc    eax
        xor    ebx, ebx
        mov    dword ptr [0x41f24], eax      ; -> loc_4_1F24
        mov    eax, ebx
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_0CAB:
        push   0x9a
        mov    eax, dword ptr [ecx]
        mov    ebx, ecx
        mov    dword ptr [0x41f38], eax      ; -> loc_4_1F38
        push   esi
        lea    eax, [ecx + 4]
        mov    dword ptr [ecx], 0
        push   eax
        mov    esi, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x9e], 0
        call   sub_2_0040
        add    esp, 0xc
        test   esi, esi
        jne    loc_2_0D0D
        test   edi, edi
        jne    loc_2_0CE9
        mov    dword ptr [0x41f2c], ebx      ; -> loc_4_1F2C
        jmp    loc_2_0CEB
loc_2_0CE9:
        mov    dword ptr [edi], ebx
loc_2_0CEB:
        mov    dl, byte ptr [0x41f18]        ; -> loc_4_1F18
        mov    edi, 1
        inc    dl
        mov    dword ptr [0x41ff4], edi      ; -> loc_4_1FF4
        mov    byte ptr [0x41f18], dl        ; -> loc_4_1F18
        mov    eax, ebx
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_0D0D:
        or     byte ptr [ebx + 0x9e], 4
        mov    eax, dword ptr [0x41f34]      ; -> loc_4_1F34
        mov    dword ptr [ebx], eax
        mov    dword ptr [0x41f34], ebx      ; -> loc_4_1F34
loc_2_0D21:
        mov    eax, ebx
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_2_0D30:
        xor    eax, eax
        ret    
        db  090h
        ; ---- dispatch table (6 entries) ----
        dd  loc_2_0D84
        dd  loc_2_0E2D
        dd  loc_2_0DD5
        dd  loc_2_0DEB
        dd  loc_2_0E17
        dd  loc_2_0E01
        db  08Dh, 044h, 020h, 000h
loc_2_0D50:
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0x10]
        cmp    dword ptr [edx + 0x28], 0
        je     loc_2_0E49
        mov    eax, dword ptr [edx + 0x2c]
        mov    edx, dword ptr [edx + 0x40]
        mov    dl, byte ptr [edx]
        and    edx, 0xff
        sub    edx, 3
        cmp    edx, 5
        ja     loc_2_0E49
        jmp    dword ptr cs:[edx*4 + 0x20d34] ; -> obj2:0x0D34
loc_2_0D84:
        push   0
        add    eax, 4
        push   eax
        call   sub_2_0C20
        add    esp, 8
        test   eax, eax
        jne    loc_2_0E49
        mov    edx, 0x2045
        push   0x42624                       ; -> obj4:0x2624
        mov    esi, 0x400cc                  ; -> obj4:0x00CC
        mov    edi, 0xc8
        push   0x200
        mov    word ptr [0x4262c], dx        ; -> obj4:0x262C
        mov    dword ptr [0x42634], esi      ; -> obj4:0x2634
        mov    dword ptr [0x42630], edi      ; -> obj4:0x2630
        call   sub_4_04F4
        add    esp, 8
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret    
loc_2_0DD5:
        push   0x10
        push   0x4200c                       ; -> loc_4_200C
        push   eax
        call   sub_2_0040
        add    esp, 0xc
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret    
loc_2_0DEB:
        push   0x16
        push   0x4201c                       ; -> loc_4_201C
        push   eax
        call   sub_2_0040
        add    esp, 0xc
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret    
loc_2_0E01:
        push   0x21
        push   0x42032                       ; -> loc_4_2032
        push   eax
        call   sub_2_0040
        add    esp, 0xc
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret    
loc_2_0E17:
        push   2
        push   0x42638                       ; -> obj4:0x2638
        push   eax
        call   sub_2_0040
        add    esp, 0xc
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret    
loc_2_0E2D:
        mov    ecx, dword ptr [0x41f2c]      ; -> loc_4_1F2C
        test   ecx, ecx
        je     loc_2_0E49
        push   0x9a
        lea    edx, [ecx + 4]
        push   edx
        push   eax
        call   sub_2_0040
        add    esp, 0xc
loc_2_0E49:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        ret    
        db  090h
sub_2_0E50:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        mov    esi, dword ptr [ebp + 0x10]
        xor    edx, edx
        xor    ebx, ebx
        mov    dword ptr [0x42610], edx      ; -> obj4:0x2610
        test   esi, esi
        jle    loc_2_0EA7
loc_2_0E69:
        mov    byte ptr [ebp - 0x40], 0x7f
        lea    eax, [ebp - 0x40]
        mov    dl, 0x23
        push   eax
        mov    byte ptr [ebp - 0x10], bl
        mov    byte ptr [ebp - 0x3f], dl
        call   sub_2_0990
        mov    dh, byte ptr [ebp - 0x3f]
        add    esp, 4
        cmp    dh, 0x23
        jne    loc_2_0E8E
        cmp    dh, byte ptr [ebp - 0xf]
        je     loc_2_0EA2
loc_2_0E8E:
        mov    eax, dword ptr [0x42610]      ; -> obj4:0x2610
        lea    edx, [eax + 1]
        mov    dword ptr [0x42610], edx      ; -> obj4:0x2610
        mov    byte ptr [eax + 0x42614], bl  ; -> obj4:0x2614
loc_2_0EA2:
        inc    ebx
        cmp    ebx, esi
        jl     loc_2_0E69
loc_2_0EA7:
        mov    al, byte ptr [0x42610]        ; -> obj4:0x2610
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh, 0C0h
sub_2_0EC0:
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0xb0
        sub    ebp, 0x80
        mov    bl, 8
        mov    ecx, 1
        mov    esi, 0x400f4                  ; -> obj4:0x00F4
        mov    edi, 0x41d61                  ; -> obj4:0x1D61
        push   0x42624                       ; -> obj4:0x2624
        xor    edx, edx
        xor    ah, ah
        push   0x200
        mov    dword ptr [ebp + 0x74], edx
        mov    dword ptr [ebp + 0x78], edx
        mov    byte ptr [0x4200c], ah        ; -> loc_4_200C
        mov    byte ptr [0x4201c], ah        ; -> loc_4_201C
        mov    byte ptr [0x42032], ah        ; -> loc_4_2032
        mov    byte ptr [0x42638], bl        ; -> obj4:0x2638
        mov    dword ptr [0x42624], ecx      ; -> obj4:0x2624
        mov    dword ptr [0x42628], esi      ; -> obj4:0x2628
        mov    dword ptr [0x42634], edi      ; -> obj4:0x2634
        mov    ebx, 0x2053
        xor    dl, dl
        xor    dh, dh
        mov    eax, 0xc8
        mov    word ptr [0x4262e], dx        ; -> obj4:0x262E
        mov    word ptr [0x4262c], bx        ; -> obj4:0x262C
        mov    dword ptr [0x42630], eax      ; -> obj4:0x2630
        call   sub_4_04F4
        add    esp, 8
        push   0x42624                       ; -> obj4:0x2624
        mov    edx, 0x41d77                  ; -> obj4:0x1D77
        mov    ebx, 0xc8
        push   0x200
        mov    dword ptr [0x42634], edx      ; -> obj4:0x2634
        mov    dword ptr [0x42630], ebx      ; -> obj4:0x2630
        call   sub_4_04F4
        add    esp, 8
        push   0x42624                       ; -> obj4:0x2624
        mov    esi, 0xc9
        mov    ecx, 0x41da1                  ; -> obj4:0x1DA1
        push   0x200
        mov    dword ptr [0x42630], esi      ; -> obj4:0x2630
        mov    dword ptr [0x42634], ecx      ; -> obj4:0x2634
        call   sub_4_04F4
        add    esp, 8
        push   8
        push   0x400fc                       ; -> obj4:0x00FC
        lea    eax, [ebp + 0x58]
        push   eax
        call   sub_2_0040
        add    esp, 0xc
        lea    eax, [ebp + 0x58]
        push   eax
        push   0x101
        call   sub_4_04F4
        add    esp, 8
        test   eax, eax
        je     loc_2_1004
        push   eax
        push   0x40108                       ; -> obj4:0x0108
        push   0x422c4                       ; -> obj4:0x22C4
        call   sub_2_16C0
        add    esp, 0xc
        mov    edi, 0x2045
        push   0x42624                       ; -> obj4:0x2624
        mov    word ptr [0x4262c], di        ; -> obj4:0x262C
        mov    edi, 0x422c4                  ; -> obj4:0x22C4
        push   0x200
        mov    dword ptr [0x42630], ebx      ; -> obj4:0x2630
        mov    dword ptr [0x42634], edi      ; -> obj4:0x2634
        jmp    loc_2_12CB
loc_2_1004:
        push   0x42610                       ; -> obj4:0x2610
        mov    eax, dword ptr [ebp + 0x6c]
        push   0x42420                       ; -> obj4:0x2420
        mov    dword ptr [0x42004], eax      ; -> sub_4_2004
        call   sub_2_0A00
        add    esp, 8
        mov    edi, dword ptr [0x42610]      ; -> obj4:0x2610
        push   edi
        push   0
        push   1
        call   sub_2_08D0
        add    esp, 0xc
        test   eax, eax
        je     loc_2_105A
        push   0x42624                       ; -> obj4:0x2624
        mov    esi, 0x2045
        mov    ecx, 0x41e38                  ; -> obj4:0x1E38
        push   0x200
        mov    word ptr [0x4262c], si        ; -> obj4:0x262C
        mov    esi, 0x24
        jmp    loc_2_12BF
loc_2_105A:
        lea    eax, [ebp - 0x30]
        push   eax
        push   0x40130                       ; -> obj4:0x0130
        push   0x4220d                       ; -> obj4:0x220D
        call   sub_2_2A10
        add    esp, 0xc
        lea    eax, [ebp - 0x30]
        push   eax
        push   0x420b8                       ; -> loc_4_20B8
        call   sub_2_23E0
        add    esp, 8
        test   eax, eax
        je     loc_2_10CA
        lea    eax, [ebp - 0x30]
        push   eax
        push   0x4013c                       ; -> obj4:0x013C
        push   0x422c4                       ; -> obj4:0x22C4
        call   sub_2_16C0
        add    esp, 0xc
        mov    ecx, 0x2045
        push   0x42624                       ; -> obj4:0x2624
        mov    eax, 0x422c4                  ; -> obj4:0x22C4
        mov    dword ptr [0x42630], ebx      ; -> obj4:0x2630
        push   0x200
        mov    word ptr [0x4262c], cx        ; -> obj4:0x262C
        mov    dword ptr [0x42634], eax      ; -> obj4:0x2634
        call   sub_4_04F4
        add    esp, 8
loc_2_10CA:
        call   sub_2_06D0
        test   eax, eax
        je     loc_2_1112
        mov    ecx, 0x2045
        push   0x42624                       ; -> obj4:0x2624
        mov    edx, 0x41dfa                  ; -> obj4:0x1DFA
        mov    ebx, 8
        push   0x200
        mov    word ptr [0x4262c], cx        ; -> obj4:0x262C
        mov    dword ptr [0x42634], edx      ; -> obj4:0x2634
        mov    dword ptr [0x42630], ebx      ; -> obj4:0x2630
        call   sub_4_04F4
        add    esp, 8
        lea    esp, [ebp + 0x80]
        pop    ebp
        pop    edi
        pop    esi
        ret    
loc_2_1112:
        mov    esi, 0x2053
        mov    ebx, 0x40150                  ; -> obj4:0x0150
        push   0x42624                       ; -> obj4:0x2624
        mov    ecx, 0x12c
        xor    edi, edi
        push   0x200
        mov    word ptr [0x4262c], si        ; -> obj4:0x262C
        mov    dword ptr [0x42634], ebx      ; -> obj4:0x2634
        mov    dword ptr [0x42630], ecx      ; -> obj4:0x2630
        call   sub_4_04F4
        mov    esi, 0x200000
        mov    bh, 5
        add    esp, 8
        mov    cl, 0x14
        mov    eax, dword ptr [0x42008]      ; -> obj4:0x2008
        push   4
        mov    dword ptr [ebp + 0x48], eax
        lea    eax, [ebp + 0x7c]
        mov    word ptr [ebp + 0x7e], di
        push   eax
        lea    eax, [ebp + 0x44]
        mov    dword ptr [ebp + 0x4c], esi
        push   eax
        mov    byte ptr [ebp + 0x7c], bh
        mov    byte ptr [ebp + 0x7d], cl
        call   sub_2_0040
        add    esp, 0xc
        push   8
        push   0x40170                       ; -> obj4:0x0170
        lea    eax, [ebp + 0x34]
        push   eax
        call   sub_2_0040
        add    esp, 0xc
        push   8
        push   0x4017c                       ; -> obj4:0x017C
        lea    eax, [ebp + 0x3c]
        push   eax
        call   sub_2_0040
        add    esp, 0xc
        lea    eax, [ebp + 0x34]
        push   eax
        xor    edi, edi
        push   0x100
        mov    dword ptr [ebp + 0x54], edi
        call   sub_4_04F4
        add    esp, 8
        test   eax, eax
        je     loc_2_120B
        push   eax
        push   0x40188                       ; -> obj4:0x0188
        push   0x422c4                       ; -> obj4:0x22C4
        call   sub_2_16C0
        add    esp, 0xc
        mov    ebx, 0x2045
        mov    edi, 0x422c4                  ; -> obj4:0x22C4
        push   0x42624                       ; -> obj4:0x2624
        mov    eax, 0xc8
        mov    word ptr [0x4262c], bx        ; -> obj4:0x262C
        push   0x200
        mov    dword ptr [0x42634], edi      ; -> obj4:0x2634
        mov    dword ptr [0x42630], eax      ; -> obj4:0x2630
        call   sub_4_04F4
        add    esp, 8
        lea    esp, [ebp + 0x80]
        pop    ebp
        pop    edi
        pop    esi
        ret    
loc_2_120B:
        push   0x1c
        push   edi
        lea    eax, [ebp + 0x34]
        push   eax
        call   sub_2_00D1
        add    esp, 0xc
        push   8
        push   0x40170                       ; -> obj4:0x0170
        lea    eax, [ebp + 0x34]
        push   eax
        call   sub_2_0040
        add    esp, 0xc
        lea    eax, [ebp + 0x34]
        push   eax
        push   0x101
        call   sub_4_04F4
        add    esp, 8
        test   eax, eax
        je     loc_2_128B
        push   eax
        push   0x401b0                       ; -> obj4:0x01B0
        push   0x422c4                       ; -> obj4:0x22C4
        call   sub_2_16C0
        add    esp, 0xc
        mov    eax, 0x2045
        mov    edx, 0xc8
        push   0x42624                       ; -> obj4:0x2624
        mov    word ptr [0x4262c], ax        ; -> obj4:0x262C
        mov    eax, 0x422c4                  ; -> obj4:0x22C4
        push   0x200
        mov    dword ptr [0x42630], edx      ; -> obj4:0x2630
        mov    dword ptr [0x42634], eax      ; -> obj4:0x2634
        call   sub_4_04F4
        add    esp, 8
        jmp    loc_2_1293
loc_2_128B:
        mov    eax, dword ptr [ebp + 0x48]
        mov    dword ptr [0x42008], eax      ; -> obj4:0x2008
loc_2_1293:
        cmp    dword ptr [0x42008], 0x20d50  ; -> obj4:0x2008
        je     loc_2_12D4
        mov    edx, 0x2045
        push   0x42624                       ; -> obj4:0x2624
        mov    ecx, 0x401d8                  ; -> obj4:0x01D8
        mov    esi, 0xc8
        push   0x200
        mov    word ptr [0x4262c], dx        ; -> obj4:0x262C
loc_2_12BF:
        mov    dword ptr [0x42634], ecx      ; -> obj4:0x2634
        mov    dword ptr [0x42630], esi      ; -> obj4:0x2630
loc_2_12CB:
        call   sub_4_04F4
        add    esp, 8
loc_2_12D4:
        lea    esp, [ebp + 0x80]
        pop    ebp
        pop    edi
        pop    esi
        ret    
        db  000h, 000h
sub_2_12E0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        mov    ecx, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0x20]
        lea    ebx, [ebp - 0x10]
        xor    esi, esi
loc_2_12F4:
        mov    eax, ecx
        xor    edx, edx
        div    dword ptr [ebp + 0x1c]
        mov    eax, ecx
        mov    dl, byte ptr [edx + 0x420c8]  ; -> obj4:0x20C8
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    byte ptr [ebx], dl
        xor    edx, edx
        div    ecx
        inc    ebx
        inc    esi
        mov    ecx, eax
        test   eax, eax
        jne    loc_2_12F4
        test   edi, edi
        je     loc_2_1328
        cmp    edi, esi
        jle    loc_2_1328
        sub    edi, esi
        je     loc_2_1328
loc_2_1320:
        inc    esi
        mov    byte ptr [ebx], 0x30
        inc    ebx
        dec    edi
        jne    loc_2_1320
loc_2_1328:
        lea    edx, [ebp - 0x10]
        cmp    ebx, edx
        je     loc_2_1340
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dl, byte ptr [ebx - 1]
        dec    ebx
        lea    edi, [ecx + 1]
        mov    byte ptr [ecx], dl
        mov    dword ptr [ebp + 0x14], edi
        jmp    loc_2_1328
loc_2_1340:
        mov    eax, esi
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 090h
sub_2_1350:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, dword ptr [ebp + 0x18]
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dl, byte ptr [eax]
        mov    edi, eax
        test   dl, dl
        je     loc_2_16A4
loc_2_136B:
        mov    cl, byte ptr [edi]
        cmp    cl, 0x25
        je     loc_2_137C
        inc    ebx
        inc    edi
        mov    byte ptr [ebx - 1], cl
        jmp    loc_2_169B
loc_2_137C:
        mov    edx, dword ptr [ebp + 0x1c]
        mov    al, byte ptr [edi + 1]
        inc    edi
        add    edx, 4
        cmp    al, 0x6e
        jb     loc_2_13D5
        jbe    loc_2_1583
        cmp    al, 0x75
        jb     loc_2_13BA
        jbe    loc_2_15D8
        cmp    al, 0x78
        jb     loc_2_13AD
        jbe    loc_2_15F5
        cmp    al, 0x79
        je     loc_2_140B
        jmp    loc_2_1694
loc_2_13AD:
        cmp    al, 0x77
        je     loc_2_1654
        jmp    loc_2_1694
loc_2_13BA:
        cmp    al, 0x70
        jb     loc_2_1694
        jbe    loc_2_1612
        cmp    al, 0x73
        je     loc_2_14C5
        jmp    loc_2_1694
loc_2_13D5:
        cmp    al, 0x62
        jb     loc_2_13F0
        jbe    loc_2_1674
        cmp    al, 0x63
        jbe    loc_2_143D
        cmp    al, 0x69
        je     loc_2_1537
        jmp    loc_2_1694
loc_2_13F0:
        cmp    al, 0x4e
        jb     loc_2_1694
        jbe    loc_2_146A
        cmp    al, 0x61
        je     loc_2_14E7
        jmp    loc_2_1694
loc_2_140B:
        mov    eax, dword ptr [ebp + 0x1c]
        xor    esi, esi
        mov    dword ptr [ebp + 0x1c], edx
        mov    eax, dword ptr [eax]
loc_2_1415:
        mov    ch, byte ptr [eax]
        cmp    ch, 0x20
        jb     loc_2_1427
        cmp    ch, 0x7e
        ja     loc_2_1427
        xor    edx, edx
        mov    dl, ch
        jmp    loc_2_142C
loc_2_1427:
        mov    edx, 0x2e
loc_2_142C:
        inc    ebx
        inc    eax
        inc    esi
        mov    byte ptr [ebx - 1], dl
        cmp    esi, 0x10
        jae    loc_2_169A
        jmp    loc_2_1415
loc_2_143D:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 0x1c], edx
        test   eax, eax
        jne    loc_2_144E
        mov    eax, 0x40208                  ; -> obj4:0x0208
loc_2_144E:
        movzx  esi, byte ptr [eax]
        inc    eax
        test   esi, esi
        je     loc_2_169A
loc_2_145A:
        inc    ebx
        mov    dl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ebx - 1], dl
        dec    esi
        je     loc_2_169A
        jmp    loc_2_145A
loc_2_146A:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    edx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [eax]
        add    edx, 4
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp + 0x1c], edx
        test   eax, eax
        jne    loc_2_1486
        mov    dword ptr [ebp - 8], 0x40208  ; -> obj4:0x0208
loc_2_1486:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 8]
        inc    eax
        movzx  esi, byte ptr [esi]
        mov    dword ptr [ebp - 8], eax
        cmp    esi, 0x20
        jbe    loc_2_149D
        mov    esi, 0x20
loc_2_149D:
        test   esi, esi
        je     loc_2_169A
        push   2
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        push   0x10
        mov    al, byte ptr [edx]
        push   eax
        push   ebx
        lea    eax, [edx + 1]
        dec    esi
        mov    dword ptr [ebp - 8], eax
        call   sub_2_12E0
        add    esp, 0x10
        add    ebx, eax
        jmp    loc_2_149D
loc_2_14C5:
        mov    esi, dword ptr [ebp + 0x1c]
        mov    esi, dword ptr [esi]
        mov    dword ptr [ebp + 0x1c], edx
        test   esi, esi
        jne    loc_2_14D6
        mov    esi, 0x40208                  ; -> obj4:0x0208
loc_2_14D6:
        mov    dh, byte ptr [esi]
        test   dh, dh
        je     loc_2_169A
        inc    ebx
        inc    esi
        mov    byte ptr [ebx - 1], dh
        jmp    loc_2_14D6
loc_2_14E7:
        mov    eax, dword ptr [ebp + 0x1c]
        xor    esi, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 0x1c], edx
        mov    dword ptr [ebp - 4], eax
loc_2_14F4:
        push   2
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        push   0x10
        mov    al, byte ptr [edx]
        push   eax
        lea    ecx, [edx + 1]
        push   ebx
        mov    dword ptr [ebp - 4], ecx
        call   sub_2_12E0
        add    ebx, eax
        inc    esi
        add    esp, 0x10
        mov    byte ptr [ebx], 0x2d
        inc    ebx
        cmp    esi, 5
        jb     loc_2_14F4
        push   2
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        push   0x10
        mov    al, byte ptr [edx]
        push   eax
        push   ebx
        call   sub_2_12E0
        add    esp, 0x10
        add    ebx, eax
        jmp    loc_2_169A
loc_2_1537:
        mov    eax, dword ptr [ebp + 0x1c]
        xor    esi, esi
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp + 0x1c], edx
loc_2_1542:
        push   0
        mov    edx, dword ptr [ebp - 0xc]
        xor    eax, eax
        push   0xa
        mov    al, byte ptr [edx]
        push   eax
        inc    edx
        push   ebx
        mov    dword ptr [ebp - 0xc], edx
        call   sub_2_12E0
        add    ebx, eax
        inc    esi
        add    esp, 0x10
        mov    byte ptr [ebx], 0x2e
        inc    ebx
        cmp    esi, 3
        jb     loc_2_1542
        push   0
        mov    edx, dword ptr [ebp - 0xc]
        xor    eax, eax
        push   0xa
        mov    al, byte ptr [edx]
        push   eax
        push   ebx
        call   sub_2_12E0
        add    esp, 0x10
        add    ebx, eax
        jmp    loc_2_169A
loc_2_1583:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [eax]
        movzx  si, byte ptr [eax + 1]
        and    esi, 0xffff
        mov    al, byte ptr [eax]
        shl    esi, 8
        xor    ah, ah
        and    eax, 0xffff
        add    esi, eax
        push   0
        mov    eax, esi
        push   0xa
        shr    eax, 0xa
        push   eax
        push   ebx
        mov    dword ptr [ebp + 0x1c], edx
        call   sub_2_12E0
        add    esp, 0x10
        push   0
        push   0xa
        and    esi, 0x3ff
        add    ebx, eax
        push   esi
        mov    byte ptr [ebx], 0x2e
        inc    ebx
        push   ebx
        call   sub_2_12E0
        add    esp, 0x10
        add    ebx, eax
        jmp    loc_2_169A
loc_2_15D8:
        push   0
        mov    esi, dword ptr [ebp + 0x1c]
        push   0xa
        mov    esi, dword ptr [esi]
        push   esi
        push   ebx
        mov    dword ptr [ebp + 0x1c], edx
        call   sub_2_12E0
        add    esp, 0x10
        add    ebx, eax
        jmp    loc_2_169A
loc_2_15F5:
        push   8
        mov    esi, dword ptr [ebp + 0x1c]
        push   0x10
        mov    esi, dword ptr [esi]
        push   esi
        push   ebx
        mov    dword ptr [ebp + 0x1c], edx
        call   sub_2_12E0
        add    esp, 0x10
        add    ebx, eax
        jmp    loc_2_169A
loc_2_1612:
        mov    esi, dword ptr [ebp + 0x1c]
        mov    esi, dword ptr [esi]
        mov    eax, esi
        push   4
        shr    eax, 0x10
        push   0x10
        and    eax, 0xffff
        and    eax, 0xffff
        push   eax
        push   ebx
        mov    dword ptr [ebp + 0x1c], edx
        call   sub_2_12E0
        add    esp, 0x10
        push   4
        add    ebx, eax
        xor    eax, eax
        push   0x10
        mov    ax, si
        push   eax
        mov    byte ptr [ebx], 0x3a
        inc    ebx
        push   ebx
        call   sub_2_12E0
        add    esp, 0x10
        add    ebx, eax
        jmp    loc_2_169A
loc_2_1654:
        mov    eax, dword ptr [ebp + 0x1c]
        push   4
        mov    esi, dword ptr [eax]
        push   0x10
        and    esi, 0xffff
        push   esi
        push   ebx
        mov    dword ptr [ebp + 0x1c], edx
        call   sub_2_12E0
        add    esp, 0x10
        add    ebx, eax
        jmp    loc_2_169A
loc_2_1674:
        mov    eax, dword ptr [ebp + 0x1c]
        push   2
        mov    esi, dword ptr [eax]
        push   0x10
        and    esi, 0xff
        push   esi
        push   ebx
        mov    dword ptr [ebp + 0x1c], edx
        call   sub_2_12E0
        add    esp, 0x10
        add    ebx, eax
        jmp    loc_2_169A
loc_2_1694:
        inc    ebx
        mov    al, byte ptr [edi]
        mov    byte ptr [ebx - 1], al
loc_2_169A:
        inc    edi
loc_2_169B:
        cmp    byte ptr [edi], 0
        jne    loc_2_136B
loc_2_16A4:
        mov    edx, dword ptr [ebp + 0x14]
        mov    eax, ebx
        mov    byte ptr [ebx], 0
        sub    eax, edx
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_16C0:
        push   ebx
        push   ebp
        mov    ebp, esp
        lea    eax, [ebp + 0x14]
        push   eax
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_2_1350
        add    esp, 0xc
        pop    ebp
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h
sub_2_16E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dl, byte ptr [eax]
        test   dl, dl
        jne    loc_2_16F3
        mov    eax, 1
        pop    ebp
        ret    
loc_2_16F3:
        cmp    dl, 0x41
        jne    loc_2_16FF
        mov    eax, 1
        pop    ebp
        ret    
loc_2_16FF:
        cmp    dl, 0x61
        jne    loc_2_170B
        mov    eax, 1
        pop    ebp
        ret    
loc_2_170B:
        xor    eax, eax
        pop    ebp
        ret    
        db  090h, 053h, 056h, 057h, 055h, 089h, 0E5h, 083h, 0ECh, 010h, 08Bh, 04Dh, 018h, 08Bh, 07Dh, 020h
        db  08Dh, 05Dh, 0F0h, 031h, 0F6h, 089h, 0C8h, 031h, 0D2h, 0F7h, 075h, 01Ch, 0A1h
        dd  loc_4_21F8               ; fixup
        db  001h, 0C2h, 089h, 0C8h, 08Ah, 012h, 08Bh, 04Dh, 01Ch, 088h, 013h, 031h, 0D2h, 0F7h, 0F1h, 043h
        db  046h, 089h, 0C1h, 085h, 0C0h, 075h, 0DDh, 085h, 0FFh, 074h, 010h, 039h, 0F7h, 07Eh, 00Ch, 029h
        db  0F7h, 074h, 008h, 046h, 0C6h, 003h, 030h, 043h, 04Fh, 075h, 0F8h, 08Dh, 055h, 0F0h, 039h, 0D3h
        db  074h, 011h, 08Bh, 045h, 014h, 08Ah, 053h, 0FFh, 04Bh, 08Dh, 078h, 001h, 088h, 010h, 089h, 07Dh
        db  014h, 0EBh, 0E8h, 089h, 0F0h, 089h, 0ECh, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 044h, 020h, 000h
sub_2_1780:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x1c]
        test   ebx, ebx
        je     loc_2_179D
        test   byte ptr [ebx + 5], 0x80
        jne    loc_2_1813
loc_2_179D:
        mov    ebx, dword ptr [0x427d4]      ; -> obj4:0x27D4
        mov    dword ptr [0x427c0], eax      ; -> obj4:0x27C0
        test   ebx, ebx
        je     loc_2_17E9
        test   edx, edx
        jne    loc_2_17B5
        mov    edx, 0x40490                  ; -> obj4:0x0490
loc_2_17B5:
        push   edx
        mov    esi, dword ptr [eax*4 + 0x4217c] ; -> obj4:0x217C
        push   esi
        mov    edi, dword ptr [0x427d8]      ; -> obj4:0x27D8
        push   edi
        mov    edx, dword ptr [0x427d4]      ; -> obj4:0x27D4
        push   edx
        push   0x40494                       ; -> obj4:0x0494
        add    eax, 0x3e8
        push   eax
        push   0x2045
        call   sub_2_30F0
        add    esp, 0x1c
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_17E9:
        test   edx, edx
        jne    loc_2_17F2
        mov    edx, 0x40490                  ; -> obj4:0x0490
loc_2_17F2:
        push   edx
        mov    ecx, dword ptr [eax*4 + 0x4217c] ; -> obj4:0x217C
        push   ecx
        push   0x404b0                       ; -> obj4:0x04B0
        add    eax, 0x3e8
        push   eax
        push   0x2045
        call   sub_2_30F0
        add    esp, 0x14
loc_2_1813:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
sub_2_1820:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 0x61
        jb     loc_2_1830
        cmp    al, 0x7a
        ja     loc_2_1830
        sub    al, 0x20
loc_2_1830:
        pop    ebp
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh, 0C0h
sub_2_1840:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ebx, dword ptr [ebp + 0x10]
        mov    ah, byte ptr [ebx]
        mov    esi, dword ptr [ebp + 0x14]
        test   ah, ah
        je     loc_2_188A
loc_2_1854:
        cmp    byte ptr [esi], 0
        je     loc_2_188A
        xor    eax, eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_1820
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        add    esp, 4
        mov    al, byte ptr [esi]
        push   eax
        call   sub_2_1820
        mov    ah, byte ptr [ebp - 4]
        add    esp, 4
        cmp    al, ah
        je     loc_2_1881
        xor    eax, eax
        jmp    loc_2_18A1
loc_2_1881:
        mov    dl, byte ptr [ebx + 1]
        inc    ebx
        inc    esi
        test   dl, dl
        jne    loc_2_1854
loc_2_188A:
        cmp    byte ptr [ebx], 0
        jne    loc_2_1894
        cmp    byte ptr [esi], 0
        je     loc_2_189C
loc_2_1894:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret    
loc_2_189C:
        mov    eax, 1
loc_2_18A1:
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_18B0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebx]
        mov    esi, dword ptr [ebp + 0x18]
        test   edx, edx
        je     loc_2_18E4
loc_2_18C2:
        mov    edi, dword ptr [ebx]
        push   edi
        push   esi
        call   sub_2_1840
        add    esp, 8
        test   eax, eax
        je     loc_2_18DA
        mov    eax, dword ptr [ebx + 4]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_18DA:
        mov    ecx, dword ptr [ebx + 8]
        add    ebx, 8
        test   ecx, ecx
        jne    loc_2_18C2
loc_2_18E4:
        mov    eax, 0xffffffff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0C0h
sub_2_18F0:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 0x20
        je     loc_2_1906
        cmp    al, 9
        je     loc_2_1906
        cmp    al, 0xd
        je     loc_2_1906
        cmp    al, 0xa
        jne    loc_2_190D
loc_2_1906:
        mov    eax, 1
        pop    ebp
        ret    
loc_2_190D:
        xor    eax, eax
        pop    ebp
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Dh, 040h, 000h
sub_2_1920:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 0x30
        jb     loc_2_1935
        cmp    al, 0x39
        ja     loc_2_1935
        mov    eax, 1
        pop    ebp
        ret    
loc_2_1935:
        cmp    dword ptr [ebp + 0xc], 0xa
        je     loc_2_194A
        cmp    al, 0x41
        jb     loc_2_194A
        cmp    al, 0x46
        ja     loc_2_194A
        mov    eax, 1
        pop    ebp
        ret    
loc_2_194A:
        xor    eax, eax
        pop    ebp
        ret    
        db  08Bh, 0C0h
sub_2_1950:
        push   ebp
        mov    ebp, esp
        mov    ah, byte ptr [ebp + 8]
        mov    al, ah
        and    eax, 0xff
        push   eax
        call   sub_2_1820
        add    esp, 4
        mov    ah, al
        cmp    al, 0x30
        jb     loc_2_197A
        cmp    al, 0x39
        ja     loc_2_197A
        and    eax, 0xff
        sub    eax, 0x30
        pop    ebp
        ret    
loc_2_197A:
        cmp    dword ptr [ebp + 0xc], 0x10
        jne    loc_2_1996
        cmp    ah, 0x41
        jb     loc_2_1996
        cmp    ah, 0x46
        ja     loc_2_1996
        mov    al, ah
        and    eax, 0xff
        sub    eax, 0x37
        pop    ebp
        ret    
loc_2_1996:
        xor    eax, eax
        pop    ebp
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h
sub_2_19A0:
        push   ebx
        push   esi
        mov    edx, 0x426bc                  ; -> obj4:0x26BC
        push   edx
        push   0x100
        mov    ebx, dword ptr [0x420e8]      ; -> obj4:0x20E8
        push   ebx
        push   0x601
        mov    dword ptr [0x427c4], edx      ; -> obj4:0x27C4
        call   sub_4_04F4
        add    esp, 0x10
        test   eax, eax
        jg     loc_2_19E5
        mov    ecx, 0x426bc                  ; -> obj4:0x26BC
        mov    esi, 1
        mov    dword ptr [0x427d0], ecx      ; -> obj4:0x27D0
        mov    dword ptr [0x427c8], esi      ; -> obj4:0x27C8
        pop    esi
        pop    ebx
        ret    
loc_2_19E5:
        mov    edx, 0x426bc                  ; -> obj4:0x26BC
        add    edx, eax
        mov    dword ptr [0x427d0], edx      ; -> obj4:0x27D0
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_1A00:
        push   ebx
        mov    eax, dword ptr [0x427c4]      ; -> obj4:0x27C4
        cmp    eax, dword ptr [0x427d0]      ; -> obj4:0x27D0
        jb     loc_2_1A31
        cmp    dword ptr [0x427c8], 0        ; -> obj4:0x27C8
        je     loc_2_1A1B
        xor    al, al
        pop    ebx
        ret    
loc_2_1A1B:
        call   sub_2_19A0
        mov    eax, dword ptr [0x427c4]      ; -> obj4:0x27C4
        cmp    eax, dword ptr [0x427d0]      ; -> obj4:0x27D0
        jb     loc_2_1A31
        xor    al, al
        pop    ebx
        ret    
loc_2_1A31:
        mov    eax, dword ptr [0x427c4]      ; -> obj4:0x27C4
        mov    bl, byte ptr [eax]
        lea    edx, [eax + 1]
        test   bl, bl
        jne    loc_2_1A49
        mov    al, 0x20
        mov    dword ptr [0x427c4], edx      ; -> obj4:0x27C4
        pop    ebx
        ret    
loc_2_1A49:
        mov    dword ptr [0x427c4], edx      ; -> obj4:0x27C4
        mov    al, byte ptr [eax]
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 090h
sub_2_1A60:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [0x427c8]      ; -> obj4:0x27C8
        dec    esi
        test   edx, edx
        je     loc_2_1A8C
        mov    eax, dword ptr [0x427c4]      ; -> obj4:0x27C4
        cmp    eax, dword ptr [0x427d0]      ; -> obj4:0x27D0
        jb     loc_2_1A8C
        mov    eax, 1
        pop    ebp
        pop    esi
        pop    ebx
        ret    
loc_2_1A8C:
        test   esi, esi
        je     loc_2_1AAC
loc_2_1A90:
        call   sub_2_1A00
        mov    byte ptr [ebx], al
        cmp    al, 0xd
        je     loc_2_1A90
        test   al, al
        je     loc_2_1AA3
        cmp    al, 0xa
        jne    loc_2_1AA8
loc_2_1AA3:
        mov    byte ptr [ebx], 0
        jmp    loc_2_1AAC
loc_2_1AA8:
        inc    ebx
        dec    esi
        jmp    loc_2_1A8C
loc_2_1AAC:
        mov    byte ptr [ebx], 0
        mov    esi, dword ptr [0x427d8]      ; -> obj4:0x27D8
        inc    esi
        xor    eax, eax
        mov    dword ptr [0x427d8], esi      ; -> obj4:0x27D8
        pop    ebp
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh, 0C0h
sub_2_1AD0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx], 0
        jne    loc_2_1AE4
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret    
loc_2_1AE4:
        mov    dl, byte ptr [ebx]
        test   dl, dl
        je     loc_2_1B0B
        xor    eax, eax
        mov    al, dl
        push   eax
        call   sub_2_18F0
        add    esp, 4
        test   eax, eax
        jne    loc_2_1B08
        cmp    byte ptr [ebx], 0x3b
        jne    loc_2_1B0B
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret    
loc_2_1B08:
        inc    ebx
        jmp    loc_2_1AE4
loc_2_1B0B:
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret    
sub_2_1B10:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    byte ptr [ebx], 0
        je     loc_2_1B42
loc_2_1B1C:
        xor    eax, eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_18F0
        add    esp, 4
        test   eax, eax
        jne    loc_2_1B3A
        cmp    byte ptr [ebx], 0x5b
        jne    loc_2_1B42
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret    
loc_2_1B3A:
        mov    dl, byte ptr [ebx + 1]
        inc    ebx
        test   dl, dl
        jne    loc_2_1B1C
loc_2_1B42:
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_1B50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 0x18]
        mov    ebx, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebx]
        mov    ah, byte ptr [ebx]
        mov    edi, 0x1f
        test   ah, ah
        je     loc_2_1B82
loc_2_1B69:
        xor    eax, eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_18F0
        add    esp, 4
        test   eax, eax
        je     loc_2_1B82
        mov    dl, byte ptr [ebx + 1]
        inc    ebx
        test   dl, dl
        jne    loc_2_1B69
loc_2_1B82:
        mov    dh, byte ptr [ebx]
        test   dh, dh
        jne    loc_2_1B8F
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_1B8F:
        lea    eax, [ebx + 1]
        cmp    dh, 0x3d
        jne    loc_2_1BA2
        inc    esi
        mov    ebx, eax
        mov    byte ptr [esi - 1], dh
        jmp    loc_2_1C05
loc_2_1BA2:
        cmp    dh, 0x22
        jne    loc_2_1BDC
        mov    ebx, eax
        test   edi, edi
        jle    loc_2_1BC4
loc_2_1BAD:
        mov    dl, byte ptr [ebx]
        test   dl, dl
        je     loc_2_1BC4
        cmp    dl, 0x22
        je     loc_2_1BC4
        inc    esi
        mov    al, byte ptr [ebx]
        inc    ebx
        dec    edi
        mov    byte ptr [esi - 1], al
        test   edi, edi
        jg     loc_2_1BAD
loc_2_1BC4:
        cmp    byte ptr [ebx], 0x22
        jne    loc_2_1C05
        inc    ebx
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [esi], 0
        mov    dword ptr [eax], ebx
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_1BDC:
        test   edi, edi
        jle    loc_2_1C05
        cmp    byte ptr [ebx], 0
        je     loc_2_1C05
        xor    eax, eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_18F0
        add    esp, 4
        test   eax, eax
        jne    loc_2_1C05
        mov    ah, byte ptr [ebx]
        cmp    ah, 0x3d
        je     loc_2_1C05
        inc    esi
        inc    ebx
        dec    edi
        mov    byte ptr [esi - 1], ah
        jmp    loc_2_1BDC
loc_2_1C05:
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [esi], 0
        mov    dword ptr [eax], ebx
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_1C20:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp + 0x10]
        push   eax
        call   sub_2_1B50
        add    esp, 8
        test   eax, eax
        je     loc_2_1C6A
        mov    ebx, dword ptr [0x427cc]      ; -> obj4:0x27CC
        cmp    dword ptr [ebx], 0
        je     loc_2_1C68
loc_2_1C47:
        lea    eax, [ebp - 0x20]
        push   eax
        mov    esi, dword ptr [ebx]
        push   esi
        call   sub_2_1840
        add    esp, 8
        test   eax, eax
        je     loc_2_1C5E
        mov    eax, ebx
        jmp    loc_2_1C6A
loc_2_1C5E:
        mov    ecx, dword ptr [ebx + 8]
        add    ebx, 8
        test   ecx, ecx
        jne    loc_2_1C47
loc_2_1C68:
        xor    eax, eax
loc_2_1C6A:
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret    
sub_2_1C70:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x14]
        push   edx
        push   esi
        call   sub_2_1B50
        add    esp, 8
        test   eax, eax
        je     loc_2_1CB8
        push   ebx
        push   esi
        call   sub_2_1B50
        add    esp, 8
        test   eax, eax
        je     loc_2_1CB8
        cmp    byte ptr [ebx], 0x3d
        je     loc_2_1CA5
        xor    eax, eax
        pop    ebp
        pop    esi
        pop    ebx
        ret    
loc_2_1CA5:
        push   ebx
        push   esi
        call   sub_2_1B50
        add    esp, 8
        test   eax, eax
        je     loc_2_1CB8
        mov    eax, 1
loc_2_1CB8:
        pop    ebp
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 044h, 020h, 000h
sub_2_1CC0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 0x1c]
        mov    ebx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x18]
        xor    edi, edi
        mov    ebx, dword ptr [ebx]
        mov    dword ptr [eax], edi
        cmp    byte ptr [ebx], 0x30
        jne    loc_2_1CE8
        cmp    byte ptr [ebx + 1], 0x78
        jne    loc_2_1CE8
        mov    esi, 0x10
        add    ebx, 2
loc_2_1CE8:
        mov    dh, byte ptr [ebx]
        test   dh, dh
        je     loc_2_1D27
        cmp    dh, 0x2e
        je     loc_2_1D27
        xor    eax, eax
        push   esi
        mov    al, dh
        push   eax
        call   sub_2_1920
        add    esp, 8
        test   eax, eax
        jne    loc_2_1D11
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], ebx
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_1D11:
        imul   edi, esi
        xor    eax, eax
        push   esi
        mov    al, byte ptr [ebx]
        push   eax
        inc    ebx
        call   sub_2_1950
        add    esp, 8
        add    edi, eax
        jmp    loc_2_1CE8
loc_2_1D27:
        cmp    byte ptr [ebx], 0x2e
        jne    loc_2_1D2D
        inc    ebx
loc_2_1D2D:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [eax], edi
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 040h, 000h
        ; ---- dispatch table (9 entries) ----
        dd  loc_2_1D95
        dd  loc_2_1DCA
        dd  loc_2_1E0B
        dd  loc_2_1E0B
        dd  loc_2_1E0B
        dd  loc_2_1E0B
        dd  loc_2_1E0B
        dd  loc_2_1DCA
        dd  loc_2_1D95
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h
loc_2_1D70:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    ebx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 8], esi
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, 8
        ja     loc_2_1E0B
        jmp    dword ptr cs:[eax*4 + 0x21d44] ; -> obj2:0x1D44
loc_2_1D95:
        push   esi
        call   sub_2_16E0
        add    esp, 4
        test   eax, eax
        jne    loc_2_1E35
        push   0xa
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_1CC0
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1DEC
        push   esi
        push   ebx
        push   1
        call   sub_2_1780
        add    esp, 0xc
        jmp    loc_2_1DEC
loc_2_1DCA:
        push   0x10
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_1CC0
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1DEC
        push   esi
        push   ebx
        push   2
        call   sub_2_1780
        add    esp, 0xc
loc_2_1DEC:
        mov    edx, dword ptr [ebx + 0x10]
        test   edx, edx
        je     loc_2_1E0F
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebx + 0xc]
        jb     loc_2_1DFF
        cmp    eax, edx
        jbe    loc_2_1E0F
loc_2_1DFF:
        push   esi
        push   ebx
        push   3
        call   sub_2_1780
        add    esp, 0xc
loc_2_1E0B:
        xor    eax, eax
        jmp    loc_2_1E3A
loc_2_1E0F:
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, 1
        jbe    loc_2_1E23
        cmp    eax, 7
        jb     loc_2_1E35
        cmp    eax, 8
        jbe    loc_2_1E2D
        jmp    loc_2_1E35
loc_2_1E23:
        mov    ebx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx], eax
        jmp    loc_2_1E35
loc_2_1E2D:
        mov    ebx, dword ptr [ebx + 8]
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [ebx], al
loc_2_1E35:
        mov    eax, 1
loc_2_1E3A:
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret    
loc_2_1E40:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    esi, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [esi + 8]
        mov    ah, byte ptr [ebx]
        mov    esi, dword ptr [esi + 0x10]
        test   ah, ah
        je     loc_2_1E9B
loc_2_1E5B:
        test   esi, esi
        je     loc_2_1E9B
        xor    eax, eax
        push   0x10
        mov    al, byte ptr [ebx]
        push   eax
        inc    ebx
        call   sub_2_1950
        add    esp, 8
        mov    dh, byte ptr [ebx]
        mov    dword ptr [ebp - 4], eax
        test   dh, dh
        je     loc_2_1E9B
        xor    eax, eax
        push   0x10
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_2_1950
        mov    ah, byte ptr [ebp - 4]
        shl    ah, 4
        inc    edi
        add    al, ah
        inc    ebx
        mov    byte ptr [edi - 1], al
        dec    esi
        mov    dl, byte ptr [ebx]
        add    esp, 8
        test   dl, dl
        jne    loc_2_1E5B
loc_2_1E9B:
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
loc_2_1EB0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    esi, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x18]
        push   0xa
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        mov    dword ptr [ebp - 0xc], ebx
        call   sub_2_1CC0
        add    esp, 0xc
        test   eax, eax
        je     loc_2_1EEE
        push   0xa
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_1CC0
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1EFE
loc_2_1EEE:
        push   ebx
        push   esi
        push   1
        call   sub_2_1780
        add    esp, 0xc
        xor    eax, eax
        jmp    loc_2_1F47
loc_2_1EFE:
        mov    edx, dword ptr [ebp - 4]
        test   edx, edx
        je     loc_2_1F19
        cmp    edx, 0x3f
        ja     loc_2_1F19
        mov    edi, dword ptr [ebp - 8]
        test   edi, edi
        je     loc_2_1F19
        cmp    edi, 0x3ff
        jbe    loc_2_1F2E
loc_2_1F19:
        push   ebx
        push   esi
        push   4
        call   sub_2_1780
        add    esp, 0xc
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_1F2E:
        mov    eax, edx
        shl    eax, 0xa
        lea    edx, [edi + eax]
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [edx], ax
        mov    eax, 1
loc_2_1F47:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0C0h
loc_2_1F50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ebx, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x14]
        xor    edx, edx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 4], edx
        mov    edi, dword ptr [edi + 0x10]
        mov    esi, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 4]
        dec    edi
        cmp    ecx, 9
        je     loc_2_1F7E
        cmp    ecx, 0xa
        jne    loc_2_1F7F
loc_2_1F7E:
        inc    esi
loc_2_1F7F:
        test   edi, edi
        je     loc_2_1FB7
        mov    ah, byte ptr [ebx]
        test   ah, ah
        je     loc_2_1FB7
        mov    edx, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edx + 4]
        mov    al, ah
        cmp    ecx, 9
        je     loc_2_1F9B
        cmp    ecx, 5
        jne    loc_2_1FA9
loc_2_1F9B:
        and    eax, 0xff
        push   eax
        call   sub_2_1820
        add    esp, 4
loc_2_1FA9:
        mov    byte ptr [esi], al
        inc    esi
        mov    eax, dword ptr [ebp - 4]
        dec    edi
        inc    eax
        inc    ebx
        mov    dword ptr [ebp - 4], eax
        jmp    loc_2_1F7F
loc_2_1FB7:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [eax + 4]
        cmp    ebx, 9
        je     loc_2_1FC7
        cmp    ebx, 0xa
        jne    loc_2_1FD4
loc_2_1FC7:
        mov    eax, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [eax + 8]
        mov    bl, byte ptr [ebp - 4]
        mov    byte ptr [eax], bl
        jmp    loc_2_1FD7
loc_2_1FD4:
        mov    byte ptr [esi], 0
loc_2_1FD7:
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 090h
loc_2_1FF0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0x10]
        push   ebx
        call   sub_2_16E0
        add    esp, 4
        test   eax, eax
        jne    loc_2_2035
        push   ebx
        push   0x420ec                       ; -> obj4:0x20EC
        call   sub_2_18B0
        add    esp, 8
        mov    edx, eax
        cmp    eax, -1
        jne    loc_2_202D
        push   ebx
        mov    edx, dword ptr [ebp + 0xc]
        push   edx
        push   5
        call   sub_2_1780
        add    esp, 0xc
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret    
loc_2_202D:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax], dl
loc_2_2035:
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 040h, 000h
loc_2_2040:
        push   ebp
        mov    ebp, esp
        mov    eax, 1
        pop    ebp
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h
loc_2_2050:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        push   0x42154                       ; -> obj4:0x2154
        call   sub_2_18B0
        add    esp, 8
        mov    edx, eax
        cmp    eax, -1
        jne    loc_2_2083
        mov    ebx, dword ptr [ebp + 0x10]
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
        push   ecx
        push   6
        call   sub_2_1780
        add    esp, 0xc
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret    
loc_2_2083:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    byte ptr [eax], dl
        mov    eax, 1
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 090h
loc_2_20A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    eax, dword ptr [ebp + 0x18]
        lea    esi, [ebp - 0x14]
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], eax
loc_2_20B4:
        push   0xa
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        lea    edi, [ebx*4]
        call   sub_2_1CC0
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_20E4
        mov    esi, dword ptr [ebp + 0x18]
        push   esi
        mov    edi, dword ptr [ebp + 0x14]
        push   edi
        push   7
        call   sub_2_1780
        add    esp, 0xc
        xor    eax, eax
        jmp    loc_2_2134
loc_2_20E4:
        cmp    dword ptr [ebp + edi - 0x14], 0xff
        jbe    loc_2_2109
        mov    ebx, dword ptr [ebp + 0x18]
        push   ebx
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        push   7
        call   sub_2_1780
        add    esp, 0xc
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_2109:
        inc    ebx
        add    esi, 4
        cmp    ebx, 4
        jl     loc_2_20B4
        mov    ebx, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebx + 8]
        mov    al, byte ptr [ebp - 0x14]
        mov    byte ptr [ebx], al
        inc    ebx
        mov    al, byte ptr [ebp - 0x10]
        mov    byte ptr [ebx], al
        inc    ebx
        mov    al, byte ptr [ebp - 0xc]
        mov    byte ptr [ebx], al
        inc    ebx
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ebx], al
        mov    eax, 1
loc_2_2134:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 040h, 000h, 08Bh, 0C9h
sub_2_2140:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        cmp    dword ptr [0x427bc], 0        ; -> obj4:0x27BC
        jne    loc_2_215C
        mov    eax, 1
        jmp    loc_2_2200
loc_2_215C:
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 0x40]
        push   eax
        lea    eax, [ebp + 0x14]
        push   eax
        call   sub_2_1C70
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2189
        push   eax
        push   eax
        push   8
        call   sub_2_1780
        add    esp, 0xc
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_2189:
        mov    ebx, dword ptr [0x427bc]      ; -> obj4:0x27BC
        mov    ebx, dword ptr [ebx + 4]
        cmp    dword ptr [ebx], 0
        je     loc_2_21B4
loc_2_2197:
        lea    eax, [ebp - 0x40]
        push   eax
        mov    edi, dword ptr [ebx]
        push   edi
        call   sub_2_1840
        add    esp, 8
        test   eax, eax
        jne    loc_2_21B4
        mov    esi, dword ptr [ebx + 0x14]
        add    ebx, 0x14
        test   esi, esi
        jne    loc_2_2197
loc_2_21B4:
        cmp    dword ptr [ebx], 0
        jne    loc_2_21E3
        test   byte ptr [0x420e4], 2         ; -> loc_4_20E4
        jne    loc_2_21D2
        lea    eax, [ebp - 0x40]
        push   eax
        push   0
        push   9
        call   sub_2_1780
        add    esp, 0xc
loc_2_21D2:
        xor    edx, edx
        xor    eax, eax
        mov    dword ptr [0x427c0], edx      ; -> obj4:0x27C0
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_21E3:
        mov    eax, dword ptr [ebx + 4]
        and    eax, 0xff
        lea    edx, [eax*4]
        lea    eax, [ebp - 0x20]
        push   eax
        push   ebx
        call   sub_4_21BC
        add    esp, 8
loc_2_2200:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_2210:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, 0x426bc                  ; -> obj4:0x26BC
        mov    dword ptr [0x427d4], eax      ; -> obj4:0x27D4
        xor    ecx, ecx
        mov    dword ptr [0x427c4], edx      ; -> obj4:0x27C4
        mov    dword ptr [0x427d0], edx      ; -> obj4:0x27D0
        mov    dword ptr [0x427c8], ecx      ; -> obj4:0x27C8
        mov    dword ptr [0x427d8], ecx      ; -> obj4:0x27D8
        mov    edx, dword ptr [0x420e8]      ; -> obj4:0x20E8
        mov    dword ptr [0x427c0], ecx      ; -> obj4:0x27C0
        test   edx, edx
        jl     loc_2_2265
        test   byte ptr [ebp + 0x10], 1
        jne    loc_2_225D
        push   eax
        push   ecx
        push   0xa
        call   sub_2_1780
        add    esp, 0xc
loc_2_225D:
        mov    eax, dword ptr [0x427c0]      ; -> obj4:0x27C0
        pop    ebp
        pop    ebx
        ret    
loc_2_2265:
        push   ecx
        push   eax
        call   sub_2_2F70
        add    esp, 8
        mov    dword ptr [0x420e8], eax      ; -> obj4:0x20E8
        test   eax, eax
        jge    loc_2_2299
        test   byte ptr [ebp + 0x10], 1
        jne    loc_2_2291
        mov    ebx, dword ptr [0x427d4]      ; -> obj4:0x27D4
        push   ebx
        push   0
        push   0xa
        call   sub_2_1780
        add    esp, 0xc
loc_2_2291:
        mov    eax, dword ptr [0x427c0]      ; -> obj4:0x27C0
        pop    ebp
        pop    ebx
        ret    
loc_2_2299:
        call   sub_2_19A0
        xor    eax, eax
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 090h
sub_2_22B0:
        mov    edx, dword ptr [0x420e8]      ; -> obj4:0x20E8
        test   edx, edx
        jl     loc_2_22CE
        push   edx
        call   sub_2_2FB0
        mov    ecx, 0xffffffff
        add    esp, 4
        mov    dword ptr [0x420e8], ecx      ; -> obj4:0x20E8
loc_2_22CE:
        ret    
        db  090h, 053h, 057h, 055h, 089h, 0E5h, 081h, 0ECh, 080h, 000h, 000h, 000h, 083h, 0EDh, 07Eh, 08Bh
        db  09Dh, 08Eh, 000h, 000h, 000h, 08Bh, 015h
        dd  obj4:0x20E8              ; fixup
        db  085h, 0D2h, 00Fh, 08Ch, 08Ch, 000h, 000h, 000h, 031h, 0FFh, 057h, 0B9h
        dd  obj4:0x26BC              ; fixup
        db  057h, 089h, 00Dh
        dd  obj4:0x27C4              ; fixup
        db  089h, 00Dh
        dd  obj4:0x27D0              ; fixup
        db  052h, 089h, 03Dh
        dd  obj4:0x27C8              ; fixup
        db  089h, 03Dh
        dd  obj4:0x27D8              ; fixup
        db  0E8h, 017h, 00Dh, 000h, 000h, 083h, 0C4h, 00Ch, 0E8h, 07Fh, 0F6h, 0FFh, 0FFh, 068h, 080h, 000h
        db  000h, 000h, 08Dh, 045h, 0FEh, 050h, 0E8h, 031h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 008h, 085h, 0C0h
        db  075h, 048h, 08Dh, 045h, 0FEh, 050h, 0E8h, 091h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 085h, 0C0h
        db  075h, 0DBh, 08Dh, 045h, 0FEh, 050h, 0E8h, 0C1h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 085h, 0C0h
        db  074h, 0CBh, 06Ah, 05Dh, 08Dh, 045h, 0FEh, 050h, 0E8h, 04Fh, 006h, 000h, 000h, 083h, 0C4h, 008h
        db  085h, 0C0h, 074h, 003h, 0C6h, 000h, 000h, 053h, 08Dh, 045h, 0FFh, 050h, 0E8h, 0CBh, 003h, 000h
        db  000h, 083h, 0C4h, 008h, 085h, 0C0h, 075h, 0A5h, 0EBh, 005h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 08Dh
        db  065h, 07Eh, 05Dh, 05Fh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 053h, 056h, 055h, 089h
        db  0E5h, 08Bh, 05Dh, 010h, 08Bh, 075h, 014h, 056h, 053h, 0E8h, 0BEh, 0F6h, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 083h, 0F8h, 001h, 075h, 009h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Eh, 05Bh, 0C3h, 080h
        db  03Bh, 05Bh, 075h, 009h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Eh, 05Bh, 0C3h, 053h, 0E8h, 009h
        db  0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 085h, 0C0h, 075h, 0CDh, 05Dh, 05Eh, 05Bh, 0C3h, 08Dh, 080h
        db  000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh, 0C0h
sub_2_23E0:
        push   ebx
        push   ebp
        mov    ebp, esp
        push   0
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_2_2400
        add    esp, 0xc
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 090h
sub_2_2400:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0x10]
        xor    edx, edx
        mov    dword ptr [0x427cc], eax      ; -> obj4:0x27CC
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [0x427bc], edx      ; -> obj4:0x27BC
        mov    dword ptr [0x420e4], eax      ; -> loc_4_20E4
        call   sub_2_2A70
        call   sub_2_2B90
        cmp    ax, 0x400
        jb     loc_2_243D
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_2_2550
        add    esp, 4
        pop    ebp
        pop    esi
        pop    ebx
        ret    
loc_2_243D:
        mov    ebx, dword ptr [ebp + 0x18]
        push   ebx
        mov    ecx, dword ptr [ebp + 0x14]
        push   ecx
        call   sub_2_2210
        add    esp, 8
        test   eax, eax
        jne    loc_2_24B1
        mov    ebx, 0x4263c                  ; -> obj4:0x263C
        xor    esi, esi
loc_2_2458:
        push   0x80
        push   ebx
        call   sub_2_1A60
        add    esp, 8
        test   eax, eax
        jne    loc_2_24A7
        push   ebx
        call   sub_2_1AD0
        add    esp, 4
        test   eax, eax
        jne    loc_2_2458
        push   ebx
        call   sub_2_1B10
        add    esp, 4
        test   eax, eax
        je     loc_2_2494
        push   ebx
        call   sub_2_1C20
        add    esp, 4
        mov    dword ptr [0x427bc], eax      ; -> obj4:0x27BC
        jmp    loc_2_2458
loc_2_2494:
        cmp    esi, dword ptr [0x427bc]      ; -> obj4:0x27BC
        je     loc_2_2458
        push   ebx
        call   sub_2_2140
        add    esp, 4
        jmp    loc_2_2458
loc_2_24A7:
        call   sub_2_22B0
        mov    eax, dword ptr [0x427c0]      ; -> obj4:0x27C0
loc_2_24B1:
        pop    ebp
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_24C0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0x108
        mov    ebx, dword ptr [ebp + 0x14]
        cmp    dword ptr [ebx], 0
        je     loc_2_2543
        mov    esi, 0x100
loc_2_24D9:
        push   0x100
        push   0
        lea    eax, [ebp - 0x108]
        push   eax
        call   sub_2_00D1
        add    esp, 0xc
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0x108]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 8], esi
        push   0
        mov    edi, dword ptr [ebx]
        push   edi
        mov    eax, dword ptr [0x420e0]      ; -> loc_4_20E0
        push   eax
        push   0x40000901
        call   sub_4_04F4
        add    esp, 0x1c
        test   eax, eax
        jne    loc_2_2539
        lea    eax, [ebp - 0x108]
        mov    edx, dword ptr [ebx + 4]
        push   eax
        and    edx, 0xff
        push   ebx
        call   sub_4_21BC
        add    esp, 8
loc_2_2539:
        mov    ecx, dword ptr [ebx + 0x14]
        add    ebx, 0x14
        test   ecx, ecx
        jne    loc_2_24D9
loc_2_2543:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h
sub_2_2550:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 0x140
        mov    ebx, dword ptr [ebp + 0x14]
        xor    edx, edx
        mov    dword ptr [0x427d4], edx      ; -> obj4:0x27D4
        mov    dword ptr [0x427d8], edx      ; -> obj4:0x27D8
        cmp    dword ptr [ebx], 0
        je     loc_2_263F
        lea    edi, [ebp - 0x40]
        inc    edi
loc_2_257A:
        mov    edx, dword ptr [ebx]
        push   edx
        lea    eax, [ebp - 0x40]
        push   eax
        lea    esi, [ebp - 0x40]
        call   sub_2_2650
        mov    ah, byte ptr [ebp - 0x40]
        add    esp, 8
        cmp    ah, 0x5b
        jne    loc_2_2596
        mov    esi, edi
loc_2_2596:
        push   0x5d
        push   esi
        call   sub_2_29B0
        add    esp, 8
        test   eax, eax
        je     loc_2_25A8
        mov    byte ptr [eax], 0
loc_2_25A8:
        push   esi
        call   sub_2_29E0
        add    esp, 4
        push   esi
        mov    ecx, dword ptr [0x420dc]      ; -> loc_4_20DC
        push   ecx
        lea    eax, [ebp - 0x140]
        push   eax
        call   sub_2_16C0
        add    esp, 0xc
        push   0x420e0                       ; -> loc_4_20E0
        lea    eax, [ebp - 0x140]
        push   eax
        push   0x80000002
        push   0x40000900
        call   sub_4_04F4
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_2615
        mov    esi, dword ptr [ebx + 4]
        push   esi
        call   sub_2_24C0
        add    esp, 4
        mov    eax, dword ptr [0x420e0]      ; -> loc_4_20E0
        push   eax
        push   0x40000902
        call   sub_4_04F4
        xor    edx, edx
        add    esp, 8
        mov    dword ptr [0x420e0], edx      ; -> loc_4_20E0
        jmp    loc_2_2631
loc_2_2615:
        test   byte ptr [0x420e4], 1         ; -> loc_4_20E4
        jne    loc_2_2631
        lea    eax, [ebp - 0x140]
        push   eax
        push   0
        push   0xc
        call   sub_2_1780
        add    esp, 0xc
loc_2_2631:
        mov    eax, dword ptr [ebx + 8]
        add    ebx, 8
        test   eax, eax
        jne    loc_2_257A
loc_2_263F:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_2650:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        mov    bl, byte ptr [eax]
        mov    ecx, edx
        test   bl, bl
        je     loc_2_266E
loc_2_2662:
        mov    bl, byte ptr [eax]
        inc    eax
        mov    byte ptr [edx], bl
        mov    bh, byte ptr [eax]
        inc    edx
        test   bh, bh
        jne    loc_2_2662
loc_2_266E:
        mov    eax, ecx
        mov    byte ptr [edx], 0
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_2680:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        mov    bl, byte ptr [eax]
        mov    ecx, eax
        test   bl, bl
        je     loc_2_269A
loc_2_2692:
        mov    bh, byte ptr [eax + 1]
        inc    eax
        test   bh, bh
        jne    loc_2_2692
loc_2_269A:
        mov    bl, byte ptr [edx]
        test   bl, bl
        je     loc_2_26A7
        inc    eax
        inc    edx
        mov    byte ptr [eax - 1], bl
        jmp    loc_2_269A
loc_2_26A7:
        mov    byte ptr [eax], bl
        mov    eax, ecx
        pop    ebp
        pop    ebx
        ret    
        db  08Bh, 0C0h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Bh, 055h, 010h, 08Ah, 008h, 08Bh, 05Dh
        db  014h, 084h, 0C9h, 074h, 008h, 08Ah, 068h, 001h, 040h, 084h, 0EDh, 075h, 0F8h, 089h, 0D9h, 04Bh
        db  085h, 0C9h, 074h, 00Dh, 08Ah, 00Ah, 084h, 0C9h, 074h, 007h, 040h, 042h, 088h, 048h, 0FFh, 0EBh
        db  0ECh, 0C6h, 000h, 000h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
        db  08Bh, 0DBh
sub_2_26F0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [eax]
        xor    edx, edx
        test   bl, bl
        je     loc_2_2708
loc_2_26FF:
        mov    bh, byte ptr [eax + 1]
        inc    eax
        inc    edx
        test   bh, bh
        jne    loc_2_26FF
loc_2_2708:
        mov    eax, edx
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 040h, 000h
sub_2_2710:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ebx, dword ptr [ebp + 0x18]
        mov    esi, edx
        test   ebx, ebx
        jle    loc_2_2735
loc_2_2724:
        cmp    byte ptr [eax], 0
        je     loc_2_2735
        inc    edx
        mov    cl, byte ptr [eax]
        inc    eax
        dec    ebx
        mov    byte ptr [edx - 1], cl
        test   ebx, ebx
        jg     loc_2_2724
loc_2_2735:
        mov    eax, esi
        mov    byte ptr [edx], 0
        pop    ebp
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0C0h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Ah, 018h, 08Bh, 055h, 010h, 084h, 0DBh
        db  074h, 043h, 08Ah, 00Ah, 084h, 0C9h, 074h, 03Dh, 08Ah, 038h, 088h, 0CBh, 080h, 0FFh, 061h, 072h
        db  008h, 080h, 0FFh, 07Ah, 077h, 003h, 080h, 0EFh, 020h, 080h, 0FBh, 061h, 072h, 008h, 080h, 0FBh
        db  07Ah, 077h, 003h, 080h, 0EBh, 020h, 038h, 0DFh, 073h, 008h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh
        db  05Bh, 0C3h, 076h, 008h, 0B8h, 001h, 000h, 000h, 000h, 05Dh, 05Bh, 0C3h, 08Ah, 078h, 001h, 040h
        db  042h, 084h, 0FFh, 075h, 0BDh, 080h, 038h, 000h, 074h, 008h, 0B8h, 001h, 000h, 000h, 000h, 05Dh
        db  05Bh, 0C3h, 080h, 03Ah, 000h, 074h, 008h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Bh, 0C3h, 031h
        db  0C0h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h
        db  08Bh, 0C0h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Ah, 018h, 08Bh, 055h, 010h, 084h, 0DBh
        db  074h, 027h, 08Ah, 00Ah, 084h, 0C9h, 074h, 021h, 08Ah, 018h, 038h, 0CBh, 073h, 008h, 0B8h, 0FFh
        db  0FFh, 0FFh, 0FFh, 05Dh, 05Bh, 0C3h, 076h, 008h, 0B8h, 001h, 000h, 000h, 000h, 05Dh, 05Bh, 0C3h
        db  08Ah, 078h, 001h, 040h, 042h, 084h, 0FFh, 075h, 0D9h, 080h, 038h, 000h, 074h, 008h, 0B8h, 001h
        db  000h, 000h, 000h, 05Dh, 05Bh, 0C3h, 080h, 03Ah, 000h, 074h, 008h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh
        db  05Dh, 05Bh, 0C3h, 031h, 0C0h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h
        db  022h, 000h
sub_2_2820:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x18]
        mov    eax, 1
        cmp    esi, eax
        jl     loc_2_287F
loc_2_283A:
        mov    ch, byte ptr [ebx]
        mov    cl, byte ptr [edx]
        mov    byte ptr [ebp - 4], ch
        cmp    cl, 0x61
        jb     loc_2_284E
        cmp    cl, 0x7a
        ja     loc_2_284E
        sub    cl, 0x20
loc_2_284E:
        mov    ch, byte ptr [ebp - 4]
        cmp    ch, 0x61
        jb     loc_2_2861
        cmp    ch, 0x7a
        ja     loc_2_2861
        sub    ch, 0x20
        mov    byte ptr [ebp - 4], ch
loc_2_2861:
        mov    ch, byte ptr [ebp - 4]
        cmp    cl, ch
        jae    loc_2_286F
        mov    eax, 0xffffffff
        jmp    loc_2_2881
loc_2_286F:
        jbe    loc_2_2878
        mov    eax, 1
        jmp    loc_2_2881
loc_2_2878:
        inc    edx
        inc    eax
        inc    ebx
        cmp    eax, esi
        jle    loc_2_283A
loc_2_287F:
        xor    eax, eax
loc_2_2881:
        mov    esp, ebp
        pop    ebp
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 053h, 056h, 055h, 089h, 0E5h, 08Bh, 045h
        db  010h, 08Bh, 055h, 014h, 08Bh, 075h, 018h, 0BBh, 001h, 000h, 000h, 000h, 039h, 0DEh, 07Ch, 023h
        db  08Ah, 008h, 08Ah, 02Ah, 038h, 0E9h, 073h, 009h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Eh, 05Bh
        db  0C3h, 076h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Dh, 05Eh, 05Bh, 0C3h, 040h, 043h, 042h, 039h
        db  0F3h, 07Eh, 0DDh, 031h, 0C0h, 05Dh, 05Eh, 05Bh, 0C3h, 053h, 056h, 057h, 055h, 089h, 0E5h, 083h
        db  0ECh, 00Ch, 08Bh, 05Dh, 014h, 08Bh, 075h, 018h, 0BFh, 00Ah, 000h, 000h, 000h, 031h, 0C9h, 089h
        db  0D8h, 031h, 0D2h, 0F7h, 0F7h, 081h, 0E2h, 0FFh, 000h, 000h, 000h, 083h, 0C2h, 030h, 088h, 0D0h
        db  00Fh, 0BFh, 0D1h, 088h, 044h, 015h, 0F4h, 089h, 0D8h, 031h, 0D2h, 0F7h, 0F7h, 041h, 089h, 0C3h
        db  085h, 0C0h, 075h, 0DBh, 089h, 0C8h, 066h, 085h, 0C9h, 074h, 011h, 049h, 00Fh, 0BFh, 0D1h, 046h
        db  08Ah, 054h, 015h, 0F4h, 088h, 056h, 0FFh, 066h, 085h, 0C9h, 075h, 0EFh, 098h, 0C6h, 006h, 000h
        db  089h, 0ECh, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Bh, 0C0h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch
        db  08Ah, 018h, 031h, 0D2h, 084h, 0DBh, 074h, 020h, 08Ah, 008h, 080h, 0F9h, 030h, 072h, 019h, 080h
        db  0F9h, 039h, 077h, 014h, 06Bh, 0D2h, 00Ah, 08Ah, 018h, 030h, 0FFh, 083h, 0EBh, 030h, 001h, 0DAh
        db  08Ah, 078h, 001h, 040h, 084h, 0FFh, 075h, 0E0h, 031h, 0C0h, 066h, 089h, 0D0h, 05Dh, 05Bh, 0C3h
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 055h, 00Ch
        db  08Bh, 04Dh, 010h, 089h, 0D3h, 089h, 0C8h, 080h, 03Ah, 000h, 074h, 025h, 08Ah, 010h, 08Ah, 033h
        db  084h, 0D2h, 074h, 013h, 08Ah, 010h, 040h, 038h, 0D6h, 074h, 007h, 080h, 038h, 000h, 075h, 0F4h
        db  0EBh, 005h, 089h, 0D8h, 05Dh, 05Bh, 0C3h, 089h, 0C8h, 08Ah, 053h, 001h, 043h, 084h, 0D2h, 075h
        db  0DBh, 031h, 0C0h, 05Dh, 05Bh, 0C3h, 08Dh, 040h, 000h
sub_2_29B0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    dh, byte ptr [eax]
        mov    dl, byte ptr [ebp + 0x10]
        test   dh, dh
        je     loc_2_29CC
loc_2_29C0:
        cmp    dl, byte ptr [eax]
        je     loc_2_29CC
        mov    bl, byte ptr [eax + 1]
        inc    eax
        test   bl, bl
        jne    loc_2_29C0
loc_2_29CC:
        cmp    byte ptr [eax], 0
        jne    loc_2_29D3
        xor    eax, eax
loc_2_29D3:
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_29E0:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [eax]
        mov    edx, eax
        test   bl, bl
        je     loc_2_2A0A
loc_2_29EF:
        mov    cl, byte ptr [eax]
        cmp    cl, 0x61
        jb     loc_2_2A02
        cmp    cl, 0x7a
        ja     loc_2_2A02
        mov    bl, cl
        sub    bl, 0x20
        mov    byte ptr [eax], bl
loc_2_2A02:
        mov    bh, byte ptr [eax + 1]
        inc    eax
        test   bh, bh
        jne    loc_2_29EF
loc_2_2A0A:
        mov    eax, edx
        pop    ebp
        pop    ebx
        ret    
        db  090h
sub_2_2A10:
        push   ebx
        push   esi
        push   ebp
        mov    ebp, esp
        mov    ebx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        push   ebx
        call   sub_2_2650
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0x10]
        push   ecx
        call   sub_2_26F0
        add    eax, ebx
        mov    dl, byte ptr [eax - 1]
        add    esp, 4
        cmp    dl, 0x5c
        jne    loc_2_2A3F
        mov    byte ptr [eax - 1], 0
loc_2_2A3F:
        mov    eax, dword ptr [ebp + 0x14]
        cmp    byte ptr [eax], 0x5c
        je     loc_2_2A55
        push   0x404c0                       ; -> obj4:0x04C0
        push   ebx
        call   sub_2_2680
        add    esp, 8
loc_2_2A55:
        mov    esi, dword ptr [ebp + 0x14]
        push   esi
        push   ebx
        call   sub_2_2680
        add    esp, 8
        pop    ebp
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_2A70:
        push   ebx
        cmp    dword ptr [0x421fc], 0        ; -> loc_4_21FC
        jne    loc_2_2B3B
        mov    dword ptr [0x421fc], 1        ; -> loc_4_21FC
        call   sub_2_33D0
        mov    ebx, dword ptr [0x235fc]      ; -> obj2:0x35FC
        add    ebx, 0xc
        xor    edx, edx
        mov    cl, byte ptr [ebx]
        mov    eax, ebx
        test   cl, cl
        je     loc_2_2AB9
loc_2_2AA0:
        mov    bl, byte ptr [eax]
        cmp    bl, 0x20
        je     loc_2_2AB9
        inc    edx
        inc    eax
        mov    byte ptr [edx + 0x42203], bl  ; -> obj4:0x2203
        cmp    edx, 8
        jge    loc_2_2AB9
        cmp    byte ptr [eax], 0
        jne    loc_2_2AA0
loc_2_2AB9:
        push   0xa
        push   0x42200                       ; -> loc_4_2200
        xor    bh, bh
        push   0
        mov    byte ptr [edx + 0x42204], bh  ; -> loc_4_2204
        call   sub_4_04F4
        add    esp, 0xc
        test   eax, eax
        je     loc_2_2AE1
        mov    eax, dword ptr [0x404c4]      ; -> obj4:0x04C4
        mov    dword ptr [0x4220d], eax      ; -> obj4:0x220D
loc_2_2AE1:
        mov    ecx, dword ptr [0x42200]      ; -> loc_4_2200
        test   ecx, ecx
        je     loc_2_2B2D
        mov    edx, dword ptr [ecx + 0x14]
        xor    eax, eax
loc_2_2AF0:
        mov    bl, byte ptr [edx]
        mov    byte ptr [eax + 0x4220d], bl  ; -> obj4:0x220D
        test   bl, bl
        je     loc_2_2B03
        inc    eax
        inc    edx
        cmp    eax, 0x40
        jb     loc_2_2AF0
loc_2_2B03:
        mov    eax, dword ptr [0x42200]      ; -> loc_4_2200
        test   byte ptr [eax + 4], 1
        je     loc_2_2B18
        mov    dword ptr [0x42250], 1        ; -> loc_4_2250
loc_2_2B18:
        mov    eax, dword ptr [0x42200]      ; -> loc_4_2200
        test   byte ptr [eax + 4], 4
        je     loc_2_2B2D
        mov    dword ptr [0x42254], 1        ; -> loc_4_2254
loc_2_2B2D:
        mov    eax, dword ptr [0x422b4]      ; -> loc_4_22B4
        push   eax
        call   sub_2_32C0
        add    esp, 4
loc_2_2B3B:
        pop    ebx
        ret    
        db  08Dh, 040h, 000h, 053h, 083h, 03Dh
        dd  loc_4_21FC               ; fixup
        db  000h, 074h, 00Dh, 031h, 0DBh, 0E8h, 08Fh, 008h, 000h, 000h, 089h, 01Dh
        dd  loc_4_21FC               ; fixup
        db  05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 090h
sub_2_2B60:
        push   ebp
        mov    ebp, esp
        call   sub_2_2A70
        mov    eax, dword ptr [0x235fc]      ; -> obj2:0x35FC
        push   8
        add    eax, 0xc
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_2_0040
        add    esp, 0xc
        pop    ebp
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Dh, 040h, 000h
sub_2_2B90:
        mov    edx, dword ptr [0x42200]      ; -> loc_4_2200
        test   edx, edx
        jne    loc_2_2B9D
        xor    eax, eax
        ret    
loc_2_2B9D:
        mov    ax, word ptr [edx + 2]
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 08Bh, 0C0h, 055h, 089h
        db  0E5h, 083h, 0ECh, 004h, 08Dh, 045h, 0FCh, 050h, 031h, 0D2h, 068h, 001h, 007h, 000h, 000h, 089h
        db  055h, 0FCh, 0FFh, 015h
        dd  sub_4_04F4               ; fixup
        db  08Bh, 045h, 0FCh, 083h, 0C4h, 008h, 089h, 0ECh, 05Dh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_2_2BE0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ebx, dword ptr [ebp + 0x14]
        add    ebx, 3
        shr    ebx, 2
        lea    ebx, [ebx*4]
        call   sub_2_2A70
        test   ebx, ebx
        jne    loc_2_2C09
        xor    eax, eax
        jmp    loc_2_2C99
loc_2_2C09:
        lea    eax, [ebp - 4]
        push   eax
        add    ebx, 8
        push   ebx
        push   0x400
        call   sub_4_04F4
        add    esp, 0xc
        test   eax, eax
        je     loc_2_2C2C
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_2_2C2C:
        cmp    dword ptr [0x4229c], 0        ; -> loc_4_229C
        jne    loc_2_2C64
        push   eax
        push   0x42298                       ; -> loc_4_2298
        push   4
        push   1
        push   0x404c8                       ; -> obj4:0x04C8
        call   sub_2_3250
        add    esp, 0x14
        push   0
        push   0x4229c                       ; -> loc_4_229C
        push   4
        push   1
        push   0x404d4                       ; -> obj4:0x04D4
        call   sub_2_3250
        add    esp, 0x14
loc_2_2C64:
        mov    esi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0x42298]      ; -> loc_4_2298
        mov    edi, dword ptr [0x4229c]      ; -> loc_4_229C
        push   esi
        inc    edi
        add    ecx, ebx
        push   0x422a0                       ; -> loc_4_22A0
        mov    dword ptr [0x42298], ecx      ; -> loc_4_2298
        mov    dword ptr [0x4229c], edi      ; -> loc_4_229C
        call   sub_2_3490
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_2_2C99:
        mov    esp, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  055h, 089h, 0E5h, 08Bh, 055h, 008h, 083h, 0EAh, 008h, 052h, 089h, 055h, 008h, 0E8h, 0FEh, 007h
        db  000h, 000h, 083h, 0C4h, 004h, 08Dh, 045h, 008h, 050h, 068h, 001h, 004h, 000h, 000h, 0FFh, 015h
        dd  sub_4_04F4               ; fixup
        db  083h, 0C4h, 008h, 05Dh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 090h, 053h, 056h, 057h, 055h
        db  089h, 0E5h, 083h, 0ECh, 008h, 08Bh, 07Dh, 014h, 08Bh, 05Dh, 018h, 08Bh, 045h, 01Ch, 089h, 0DEh
        db  083h, 0C0h, 00Ch, 00Fh, 0AFh, 0F0h, 0C7h, 047h, 028h, 000h, 000h, 000h, 000h, 0C7h, 047h, 02Ch
        db  000h, 000h, 000h, 000h, 0C7h, 047h, 00Ch, 000h, 000h, 000h, 000h, 0C7h, 047h, 034h, 000h, 000h
        db  000h, 000h, 089h, 05Fh, 020h, 089h, 05Fh, 024h, 089h, 047h, 01Ch, 08Bh, 045h, 020h, 089h, 077h
        db  018h, 089h, 047h, 008h, 08Dh, 047h, 010h, 08Bh, 057h, 018h, 089h, 047h, 014h, 089h, 045h, 0F8h
        db  089h, 047h, 010h, 085h, 0D2h, 074h, 059h, 052h, 0E8h, 0AFh, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 004h
        db  089h, 047h, 00Ch, 085h, 0C0h, 075h, 007h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 0EBh, 044h, 089h, 0C6h
        db  085h, 0DBh, 074h, 01Bh, 08Bh, 045h, 0F8h, 089h, 045h, 0FCh, 056h, 08Bh, 045h, 0FCh, 050h, 0E8h
        db  038h, 007h, 000h, 000h, 08Bh, 057h, 01Ch, 083h, 0C4h, 008h, 001h, 0D6h, 04Bh, 075h, 0EBh, 057h
        db  068h
        dd  loc_4_2258               ; fixup
        db  0E8h, 022h, 007h, 000h, 000h, 083h, 0C4h, 008h, 06Ah, 000h, 057h, 06Ah, 038h, 06Ah, 002h, 08Bh
        db  05Fh, 008h, 053h, 0E8h, 0CFh, 004h, 000h, 000h, 083h, 0C4h, 014h, 031h, 0C0h, 089h, 0ECh, 05Dh
        db  05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 040h, 000h, 053h, 056h, 055h, 089h, 0E5h, 09Ch, 0FAh, 058h, 025h
        db  000h, 002h, 000h, 000h, 089h, 0C6h, 089h, 0C2h, 08Bh, 045h, 010h, 08Bh, 058h, 0FCh, 08Bh, 048h
        db  0F4h, 083h, 0E8h, 00Ch, 085h, 0C9h, 075h, 006h, 083h, 078h, 004h, 000h, 074h, 004h, 089h, 0D0h
        db  0EBh, 016h, 050h, 08Dh, 043h, 010h, 050h, 0E8h, 0CBh, 006h, 000h, 000h, 08Bh, 043h, 024h, 040h
        db  083h, 0C4h, 008h, 089h, 043h, 024h, 089h, 0F0h, 09Ch, 009h, 004h, 024h, 09Dh, 05Dh, 05Eh, 05Bh
        db  0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 053h, 056h, 055h, 089h, 0E5h, 08Bh, 04Dh, 010h, 08Bh
        db  05Dh, 014h, 08Bh, 051h, 00Ch, 039h, 0D3h, 072h, 00Bh, 089h, 0D0h, 08Bh, 051h, 018h, 001h, 0C2h
        db  039h, 0D3h, 072h, 006h, 031h, 0C0h, 05Dh, 05Eh, 05Bh, 0C3h, 089h, 0DAh, 029h, 0C2h, 08Bh, 071h
        db  01Ch, 089h, 0D0h, 031h, 0D2h, 0F7h, 0F6h, 089h, 0D8h, 029h, 0D0h, 083h, 0C0h, 00Ch, 05Dh, 05Eh
        db  05Bh, 0C3h, 08Dh, 040h, 000h, 08Bh, 0C9h, 053h, 056h, 057h, 055h, 089h, 0E5h, 08Bh, 05Dh, 014h
        db  09Ch, 0FAh, 058h, 025h, 000h, 002h, 000h, 000h, 089h, 0C6h, 08Bh, 053h, 030h, 08Dh, 043h, 010h
        db  042h, 050h, 089h, 053h, 030h, 0E8h, 00Dh, 006h, 000h, 000h, 083h, 0C4h, 004h, 085h, 0C0h, 075h
        db  011h, 0FFh, 043h, 02Ch, 089h, 0F0h, 09Ch, 009h, 004h, 024h, 09Dh, 031h, 0C0h, 05Dh, 05Fh, 05Eh
        db  05Bh, 0C3h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C7h, 040h, 004h, 000h, 000h, 000h, 000h, 089h
        db  058h, 008h, 08Bh, 04Bh, 024h, 08Dh, 078h, 00Ch, 049h, 08Bh, 043h, 020h, 089h, 04Bh, 024h, 029h
        db  0C8h, 03Bh, 043h, 028h, 076h, 003h, 089h, 043h, 028h, 089h, 0F0h, 09Ch, 009h, 004h, 024h, 09Dh
        db  089h, 0F8h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 053h, 0B8h
        dd  loc_4_2258               ; fixup
        db  03Bh, 005h
        dd  loc_4_2258               ; fixup
        db  074h, 01Ah, 08Bh, 000h, 0C7h, 040h, 028h, 000h, 000h, 000h, 000h, 08Bh, 018h, 0C7h, 040h, 02Ch
        db  000h, 000h, 000h, 000h, 081h, 0FBh
        dd  loc_4_2258               ; fixup
        db  075h, 0E6h, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 056h, 055h, 089h, 0E5h, 083h, 03Dh
        dd  loc_4_22A8               ; fixup
        db  000h, 074h, 005h, 031h, 0C0h, 05Dh, 05Eh, 0C3h, 0C7h, 005h
        dd  loc_4_22A8               ; fixup
        db  001h, 000h, 000h, 000h, 0E8h, 08Fh, 0FBh, 0FFh, 0FFh, 08Bh, 04Dh, 010h, 068h
        dd  obj4:0x04E0              ; fixup
        db  083h, 0F9h, 034h, 076h, 004h, 089h, 0C8h, 0EBh, 005h, 0B8h, 034h, 000h, 000h, 000h, 050h, 08Bh
        db  075h, 00Ch, 056h, 068h
        dd  loc_4_2260               ; fixup
        db  0E8h, 0CAh, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 010h, 085h, 0C0h, 074h, 005h, 0B8h, 0FFh, 0FFh, 0FFh
        db  0FFh, 05Dh, 05Eh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh, 053h
        db  056h, 083h, 03Dh
        dd  loc_4_22A8               ; fixup
        db  000h, 074h, 040h, 08Bh, 01Dh
        dd  loc_4_22A0               ; fixup
        db  081h, 0FBh
        dd  loc_4_22A0               ; fixup
        db  074h, 00Eh, 08Dh, 043h, 008h, 050h, 0E8h, 05Eh, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 0E4h
        db  0B9h
        dd  loc_4_2258               ; fixup
        db  0BEh
        dd  loc_4_2258               ; fixup
        db  089h, 01Dh
        dd  obj4:0x22A4              ; fixup
        db  031h, 0D2h, 089h, 00Dh
        dd  loc_4_225C               ; fixup
        db  089h, 035h
        dd  loc_4_2258               ; fixup
        db  089h, 015h
        dd  loc_4_22A8               ; fixup
        db  05Eh, 05Bh, 0C3h, 000h, 000h
sub_2_2F70:
        push   ebx
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [ebp + 0x10]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        push   0x600
        call   sub_4_04F4
        add    esp, 0x10
        test   eax, eax
        je     loc_2_2F9F
        mov    eax, 0xffffffff
        mov    esp, ebp
        pop    ebp
        pop    ebx
        ret    
loc_2_2F9F:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_2FB0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        push   edx
        push   0x604
        call   sub_4_04F4
        add    esp, 8
        pop    ebp
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 055h, 010h
        db  052h, 08Bh, 05Dh, 014h, 053h, 08Bh, 04Dh, 00Ch, 051h, 068h, 001h, 006h, 000h, 000h, 0FFh, 015h
        dd  sub_4_04F4               ; fixup
        db  083h, 0C4h, 010h, 085h, 0C0h, 07Dh, 005h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Bh, 0C3h, 08Dh
        db  080h, 000h, 000h, 000h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 055h, 010h, 052h, 08Bh, 05Dh, 014h
        db  053h, 08Bh, 04Dh, 00Ch, 051h, 068h, 001h, 006h, 000h, 000h, 0FFh, 015h
        dd  sub_4_04F4               ; fixup
        db  083h, 0C4h, 010h, 085h, 0C0h, 07Dh, 005h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Bh, 0C3h, 08Dh
        db  080h, 000h, 000h, 000h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 055h, 014h, 052h, 08Bh, 05Dh, 010h
        db  053h, 08Bh, 04Dh, 00Ch, 051h, 068h, 002h, 006h, 000h, 000h, 0FFh, 015h
        dd  sub_4_04F4               ; fixup
        db  083h, 0C4h, 010h, 085h, 0C0h, 07Dh, 005h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Dh, 05Bh, 0C3h, 000h
        db  000h, 000h, 000h, 000h, 000h
sub_2_3060:
        push   esi
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        call   sub_2_2A70
        push   3
        push   0x404e4                       ; -> obj4:0x04E4
        mov    edx, 1
        push   0x42204                       ; -> loc_4_2204
        mov    dword ptr [ebp - 0x14], edx
        call   sub_2_016D
        add    esp, 0xc
        cmp    eax, 1
        jne    loc_2_3096
        mov    dword ptr [ebp - 0x10], 0x42207 ; -> obj4:0x2207
        jmp    loc_2_309D
loc_2_3096:
        mov    dword ptr [ebp - 0x10], 0x42204 ; -> loc_4_2204
loc_2_309D:
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [0x42250]      ; -> loc_4_2250
        mov    word ptr [ebp - 0xc], ax
        mov    eax, dword ptr [ebp + 0x10]
        xor    edx, edx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [ebp - 0xa], dx
        mov    dword ptr [ebp - 4], eax
        test   esi, esi
        je     loc_2_30CF
        push   eax
        push   0x300
        call   sub_4_04F4
        add    esp, 8
loc_2_30CF:
        lea    eax, [ebp - 0x14]
        push   eax
        push   0x200
        call   sub_4_04F4
        add    esp, 8
        mov    esp, ebp
        pop    ebp
        pop    esi
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 054h, 022h, 000h
sub_2_30F0:
        push   ebx
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc8
        sub    ebp, 0x72
        lea    eax, [ebp + 0x8a]
        push   eax
        mov    edx, dword ptr [ebp + 0x86]
        push   edx
        lea    eax, [ebp - 0x56]
        push   eax
        call   sub_2_1350
        add    esp, 0xc
        lea    eax, [ebp - 0x56]
        push   eax
        mov    ebx, dword ptr [ebp + 0x82]
        push   ebx
        mov    ecx, dword ptr [ebp + 0x7e]
        push   ecx
        call   sub_2_3060
        add    esp, 0xc
        lea    esp, [ebp + 0x72]
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 053h, 055h, 089h, 0E5h
        db  081h, 0ECh, 0C8h, 000h, 000h, 000h, 083h, 0EDh, 076h, 08Dh, 085h, 08Ah, 000h, 000h, 000h, 050h
        db  08Bh, 095h, 086h, 000h, 000h, 000h, 052h, 08Dh, 045h, 0AEh, 050h, 0E8h, 0ECh, 0E1h, 0FFh, 0FFh
        db  083h, 0C4h, 00Ch, 08Dh, 045h, 0AEh, 050h, 068h, 028h, 023h, 000h, 000h, 08Bh, 09Dh, 082h, 000h
        db  000h, 000h, 053h, 0E8h, 0E4h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 08Dh, 065h, 076h, 05Dh, 05Bh
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_3190:
        push   ebx
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [0x422ac], 0        ; -> sub_4_22AC
        jne    loc_2_31A5
        mov    eax, 0xb
        pop    ebp
        pop    ebx
        ret    
loc_2_31A5:
        mov    ebx, dword ptr [ebp + 0xc]
        push   ebx
        call   sub_4_22AC
        add    esp, 4
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 08Bh, 0DBh
sub_2_31C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   0x2a
        push   0
        lea    eax, [ebp - 0x2c]
        push   eax
        call   sub_2_00D1
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        push   9
        mov    dword ptr [ebp - 0x20], eax
        push   0x42204                       ; -> loc_4_2204
        lea    eax, [ebp - 0x1c]
        mov    edx, 3
        push   eax
        mov    word ptr [ebp - 0x2c], dx
        call   sub_2_2710
        add    esp, 0xc
        lea    eax, [ebp - 0x2c]
        push   eax
        call   sub_2_3190
        add    esp, 4
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Bh, 0D2h, 055h, 089h, 0E5h, 083h, 0ECh, 02Ch, 06Ah, 02Ah
        db  06Ah, 000h, 08Dh, 045h, 0D4h, 050h, 0E8h, 0AEh, 0CEh, 0FFh, 0FFh, 0BAh, 008h, 000h, 000h, 000h
        db  08Dh, 045h, 0D4h, 083h, 0C4h, 00Ch, 066h, 089h, 055h, 0D4h, 0BAh
        dd  loc_4_2258               ; fixup
        db  050h, 089h, 055h, 0D8h, 0E8h, 050h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 004h, 089h, 0ECh, 05Dh, 0C3h
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h
sub_2_3250:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   0x2a
        push   0
        lea    eax, [ebp - 0x2c]
        push   eax
        call   sub_2_00D1
        add    esp, 0xc
        push   9
        push   0x42204                       ; -> loc_4_2204
        lea    eax, [ebp - 0x1c]
        mov    edx, 4
        push   eax
        mov    word ptr [ebp - 0x2c], dx
        call   sub_2_2710
        add    esp, 0xc
        push   0x11
        mov    edx, dword ptr [ebp + 8]
        push   edx
        lea    eax, [ebp - 0x13]
        push   eax
        call   sub_2_2710
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [ebp - 0x24], ax
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [ebp - 0x22], ax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp + 0x18]
        add    esp, 0xc
        mov    dword ptr [ebp - 0x20], eax
        lea    eax, [ebp - 0x2c]
        push   eax
        call   sub_2_3190
        add    esp, 4
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0C0h
sub_2_32C0:
        push   ebx
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        cmp    dword ptr [0x422b0], 0        ; -> loc_4_22B0
        jne    loc_2_3342
        push   8
        mov    eax, dword ptr [ebp + 0xc]
        push   0x404e8                       ; -> obj4:0x04E8
        mov    dword ptr [0x422b4], eax      ; -> loc_4_22B4
        lea    eax, [ebp - 0x24]
        mov    ebx, 1
        push   eax
        mov    dword ptr [0x422b0], ebx      ; -> loc_4_22B0
        call   sub_2_0040
        add    esp, 0xc
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_2_2B60
        mov    eax, dword ptr [0x2360c]      ; -> obj2:0x360C
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 0x24]
        xor    ecx, ecx
        push   eax
        mov    dword ptr [ebp - 0x14], ecx
        mov    dword ptr [ebp - 0x10], ecx
        push   0x101
        mov    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [ebp - 4], ecx
        call   sub_4_04F4
        add    esp, 8
        test   eax, eax
        jne    loc_2_3342
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 0x10]
        push   edx
        mov    dword ptr [0x422ac], eax      ; -> sub_4_22AC
        call   sub_2_31C0
        add    esp, 4
loc_2_3342:
        mov    esp, ebp
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 052h, 000h, 055h, 089h, 0E5h, 083h, 0ECh, 02Ch, 083h
        db  03Dh
        dd  loc_4_22B0               ; fixup
        db  000h, 074h, 038h, 06Ah, 02Ah, 06Ah, 000h, 08Dh, 045h, 0D4h, 050h, 0E8h, 065h, 0CDh, 0FFh, 0FFh
        db  083h, 0C4h, 00Ch, 06Ah, 009h, 068h
        dd  loc_4_2204               ; fixup
        db  08Dh, 045h, 0E4h, 0BAh, 009h, 000h, 000h, 000h, 050h, 066h, 089h, 055h, 0D4h, 0E8h, 088h, 0F3h
        db  0FFh, 0FFh, 083h, 0C4h, 00Ch, 08Dh, 045h, 0D4h, 050h, 0E8h, 0FCh, 0FDh, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 089h, 0ECh, 05Dh, 0C3h, 08Dh, 040h, 000h, 08Bh, 0C9h, 053h, 083h, 03Dh
        dd  loc_4_22B0               ; fixup
        db  000h, 074h, 017h, 08Bh, 00Dh
        dd  loc_4_22B4               ; fixup
        db  031h, 0DBh, 051h, 089h, 01Dh
        dd  loc_4_22B0               ; fixup
        db  0E8h, 002h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 004h, 05Bh, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_33D0:
        xor    edx, edx
        mov    dword ptr [0x422c0], edx      ; -> obj4:0x22C0
        ret    
        db  08Dh, 080h, 000h, 000h, 000h, 000h, 090h, 0C7h, 005h
        dd  obj4:0x22C0              ; fixup
        db  0FFh, 0FFh, 0FFh, 0FFh, 0C3h, 08Dh, 040h, 000h, 08Bh, 0C9h, 053h, 056h, 055h, 089h, 0E5h, 08Bh
        db  05Dh, 010h, 08Bh, 075h, 014h, 09Ch, 0FAh, 058h, 025h, 000h, 002h, 000h, 000h, 08Bh, 053h, 004h
        db  089h, 032h, 089h, 01Eh, 08Bh, 053h, 004h, 089h, 056h, 004h, 089h, 073h, 004h, 09Ch, 009h, 004h
        db  024h, 09Dh, 05Dh, 05Eh, 05Bh, 0C3h, 08Dh, 044h, 020h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 05Dh
        db  00Ch, 09Ch, 0FAh, 058h, 025h, 000h, 002h, 000h, 000h, 08Bh, 04Bh, 004h, 08Bh, 013h, 089h, 011h
        db  08Bh, 05Bh, 004h, 089h, 05Ah, 004h, 09Ch, 009h, 004h, 024h, 09Dh, 05Dh, 05Bh, 0C3h, 08Dh, 080h
        db  000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 05Dh
        db  00Ch, 09Ch, 0FAh, 058h, 025h, 000h, 002h, 000h, 000h, 08Bh, 013h, 039h, 0D3h, 075h, 00Ah, 09Ch
        db  009h, 004h, 024h, 09Dh, 031h, 0C0h, 05Dh, 05Bh, 0C3h, 08Bh, 04Ah, 004h, 089h, 0D3h, 08Bh, 012h
        db  089h, 011h, 08Bh, 00Bh, 08Bh, 053h, 004h, 089h, 051h, 004h, 09Ch, 009h, 004h, 024h, 09Dh, 089h
        db  0D8h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h
sub_2_3490:
        push   ebx
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [ebx], edx
        mov    dword ptr [edx], eax
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], ebx
        mov    dword ptr [eax + 4], edx
        pop    ebp
        pop    ebx
        ret    
        db  08Dh, 040h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Bh, 058h, 004h, 08Bh, 010h, 089h
        db  013h, 08Bh, 040h, 004h, 089h, 042h, 004h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h
        db  08Dh, 052h, 000h, 053h, 055h, 089h, 0E5h, 08Bh, 045h, 00Ch, 08Bh, 010h, 039h, 0D0h, 075h, 005h
        db  031h, 0D0h, 05Dh, 05Bh, 0C3h, 08Bh, 05Ah, 004h, 089h, 0D0h, 08Bh, 012h, 089h, 013h, 08Bh, 018h
        db  08Bh, 050h, 004h, 089h, 053h, 004h, 05Dh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh
        db  054h, 022h, 000h, 053h, 056h, 057h, 055h, 089h, 0E5h, 08Bh, 075h, 014h, 08Bh, 07Dh, 01Ch, 08Bh
        db  006h, 039h, 0F0h, 074h, 018h, 08Bh, 055h, 020h, 052h, 057h, 050h, 08Bh, 018h, 0FFh, 055h, 018h
        db  083h, 0C4h, 00Ch, 085h, 0C0h, 075h, 008h, 089h, 0D8h, 039h, 0F3h, 075h, 0E8h, 031h, 0C0h, 05Dh
        db  05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h, 000h, 08Dh, 092h, 000h, 000h, 000h, 000h
        db  08Dh, 040h, 000h, 053h, 056h, 057h, 055h, 089h, 0E5h, 09Ch, 0FAh, 058h, 025h, 000h, 002h, 000h
        db  000h, 08Bh, 055h, 020h, 052h, 08Bh, 04Dh, 01Ch, 051h, 08Bh, 075h, 018h, 056h, 08Bh, 07Dh, 014h
        db  057h, 089h, 0C3h, 0E8h, 09Bh, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 010h, 089h, 0C6h, 089h, 0D8h, 09Ch
        db  009h, 004h, 024h, 09Dh, 089h, 0F0h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 080h, 000h, 000h, 000h
        db  000h, 08Bh, 0D2h, 053h, 056h, 055h, 089h, 0E5h, 08Bh, 05Dh, 010h, 089h, 0D8h, 08Bh, 00Bh, 031h
        db  0D2h, 039h, 0CBh, 074h, 009h, 08Bh, 000h, 08Bh, 030h, 042h, 039h, 0F3h, 075h, 0F7h, 089h, 0D0h
        db  05Dh, 05Eh, 05Bh, 0C3h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  033B4h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DECLICL '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  DECLICL_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x22  R-X
        db  03Dh, 00Ah, 003h, 072h, 011h, 0F7h, 0C3h, 003h, 000h, 075h, 00Bh, 033h, 0DBh, 033h, 0F6h, 066h
        db  033h, 0D2h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh, 033h, 0F6h, 066h, 033h, 0D2h, 0B8h, 001h, 080h
        db  0C3h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x27dc  RW-
        db  044h, 065h, 062h, 075h, 067h, 05Fh, 051h, 075h, 065h, 072h, 079h, 020h, 063h, 061h, 06Ch, 06Ch
        db  065h, 064h, 00Dh, 00Ah, 000h, 000h, 000h, 000h, 053h, 059h, 053h, 054h, 045h, 04Dh, 05Ch, 043h
        db  075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h
        db  05Ch, 063h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 05Ch, 043h, 06Fh, 06Dh, 070h, 075h, 074h, 065h
        db  072h, 04Eh, 061h, 06Dh, 065h, 05Ch, 043h, 06Fh, 06Dh, 070h, 075h, 074h, 065h, 072h, 04Eh, 061h
        db  06Dh, 065h, 000h, 020h, 043h, 06Fh, 06Dh, 070h, 075h, 074h, 065h, 072h, 04Eh, 061h, 06Dh, 065h
        db  000h, 020h, 072h, 065h, 04Eh, 065h, 074h, 077h, 06Fh, 072h, 06Bh, 000h, 05Ch, 05Ch, 02Eh, 05Ch
        db  070h, 069h, 070h, 065h, 05Ch, 050h, 041h, 054h, 048h, 057h, 04Fh, 052h, 04Bh, 053h, 05Fh, 04Ch
        db  049h, 043h, 045h, 04Eh, 053h, 045h, 05Fh, 050h, 049h, 050h, 045h, 000h, 04Ch, 049h, 043h, 045h
        db  04Eh, 053h, 045h, 053h, 045h, 052h, 056h, 045h, 052h, 000h, 02Ah, 02Ah, 04Ch, 049h, 043h, 045h
        db  04Eh, 053h, 045h, 047h, 052h, 04Fh, 055h, 050h, 000h, 02Ah, 02Ah, 02Ah, 04Ch, 049h, 043h, 048h
        db  049h, 04Eh, 054h, 000h, 04Ch, 049h, 043h, 045h, 04Eh, 053h, 045h, 030h, 030h, 000h, 065h, 066h
        db  05Bh, 044h, 045h, 043h, 04Ch, 049h, 043h, 04Ch, 05Dh, 000h, 052h, 053h, 04Fh, 075h, 074h, 020h
        db  06Fh, 066h, 020h, 06Dh, 065h, 06Dh, 06Fh, 072h, 079h, 020h, 066h, 06Fh, 072h, 020h, 06Ch, 069h
        db  063h, 065h, 06Eh, 073h, 065h, 020h, 064h, 061h, 074h, 061h, 020h, 062h, 061h, 073h, 065h, 02Eh
        db  00Dh, 00Ah, 000h, 069h, 04Ch, 049h, 043h, 04Ch, 000h, 072h, 069h, 06Eh, 04Eh, 045h, 054h, 042h
        db  049h, 04Fh, 053h, 030h, 000h, 069h, 06Eh, 065h, 04Eh, 045h, 054h, 042h, 049h, 04Fh, 053h, 03Ah
        db  020h, 047h, 065h, 074h, 045h, 06Eh, 074h, 072h, 079h, 050h, 06Fh, 069h, 06Eh, 074h, 020h, 065h
        db  072h, 072h, 06Fh, 072h, 02Ch, 020h, 072h, 063h, 03Dh, 025h, 078h, 00Dh, 00Ah, 000h, 065h, 020h
        db  053h, 059h, 053h, 054h, 045h, 04Dh, 02Eh, 049h, 04Eh, 049h, 000h, 000h, 045h, 072h, 072h, 06Fh
        db  072h, 020h, 072h, 065h, 061h, 064h, 069h, 06Eh, 067h, 020h, 025h, 073h, 00Dh, 00Ah, 000h, 045h
        db  043h, 06Ch, 069h, 065h, 06Eh, 074h, 020h, 04Ch, 069h, 063h, 065h, 06Eh, 073h, 069h, 06Eh, 067h
        db  020h, 056h, 058h, 044h, 020h, 069h, 06Eh, 073h, 074h, 061h, 06Ch, 06Ch, 065h, 064h, 000h, 066h
        db  044h, 045h, 043h, 04Ch, 049h, 043h, 04Ch, 030h, 000h, 000h, 052h, 053h, 044h, 045h, 043h, 04Ch
        db  049h, 043h, 04Ch, 020h, 000h, 06Dh, 06Fh, 072h, 044h, 045h, 043h, 04Ch, 049h, 043h, 04Ch, 03Ah
        db  020h, 041h, 064h, 064h, 045h, 06Eh, 074h, 072h, 079h, 050h, 06Fh, 069h, 06Eh, 074h, 020h, 065h
        db  072h, 072h, 06Fh, 072h, 02Ch, 020h, 072h, 063h, 03Dh, 025h, 078h, 00Dh, 00Ah, 000h, 054h, 042h
        db  044h, 045h, 043h, 04Ch, 049h, 043h, 04Ch, 03Ah, 020h, 047h, 065h, 074h, 045h, 06Eh, 074h, 072h
        db  079h, 050h, 06Fh, 069h, 06Eh, 074h, 020h, 065h, 072h, 072h, 06Fh, 072h, 02Ch, 020h, 072h, 063h
        db  03Dh, 025h, 078h, 00Dh, 00Ah, 000h, 072h, 063h, 044h, 045h, 043h, 04Ch, 049h, 043h, 04Ch, 03Ah
        db  020h, 043h, 06Fh, 072h, 072h, 065h, 063h, 074h, 020h, 045h, 06Eh, 074h, 072h, 079h, 020h, 050h
        db  06Fh, 069h, 06Eh, 074h, 020h, 04Eh, 06Fh, 074h, 020h, 052h, 065h, 074h, 075h, 072h, 06Eh, 065h
        db  064h, 02Eh, 00Dh, 00Ah, 000h, 000h, 000h, 000h, 028h, 06Eh, 075h, 06Ch, 06Ch, 029h, 000h, 000h
        db  053h, 059h, 053h, 054h, 045h, 04Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh
        db  06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h
        db  073h, 05Ch, 056h, 078h, 044h, 05Ch, 025h, 073h, 000h, 000h, 000h, 000h, 059h, 045h, 053h, 000h
        db  054h, 052h, 055h, 045h, 000h, 000h, 000h, 000h, 059h, 000h, 000h, 000h, 04Fh, 04Eh, 000h, 000h
        db  045h, 04Eh, 041h, 042h, 04Ch, 045h, 000h, 000h, 045h, 04Eh, 041h, 042h, 04Ch, 045h, 044h, 000h
        db  04Eh, 04Fh, 000h, 000h, 046h, 041h, 04Ch, 053h, 045h, 000h, 000h, 000h, 04Eh, 000h, 000h, 000h
        db  04Fh, 046h, 046h, 000h, 044h, 049h, 053h, 041h, 042h, 04Ch, 045h, 000h, 044h, 049h, 053h, 041h
        db  042h, 04Ch, 045h, 044h, 000h, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 031h, 000h, 000h, 000h, 000h
        db  043h, 04Fh, 04Dh, 032h, 000h, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 033h, 000h, 000h, 000h, 000h
        db  043h, 04Fh, 04Dh, 034h, 000h, 000h, 000h, 000h, 06Eh, 06Fh, 020h, 065h, 072h, 072h, 06Fh, 072h
        db  000h, 000h, 000h, 000h, 049h, 06Eh, 076h, 061h, 06Ch, 069h, 064h, 020h, 064h, 065h, 063h, 069h
        db  06Dh, 061h, 06Ch, 020h, 06Eh, 075h, 06Dh, 062h, 065h, 072h, 03Ah, 020h, 000h, 053h, 054h, 045h
        db  049h, 06Eh, 076h, 061h, 06Ch, 069h, 064h, 020h, 068h, 065h, 078h, 061h, 064h, 065h, 063h, 069h
        db  06Dh, 061h, 06Ch, 020h, 06Eh, 075h, 06Dh, 062h, 065h, 072h, 03Ah, 020h, 000h, 056h, 078h, 044h
        db  056h, 061h, 06Ch, 075h, 065h, 020h, 070h, 061h, 072h, 061h, 06Dh, 065h, 074h, 065h, 072h, 020h
        db  06Fh, 075h, 074h, 020h, 06Fh, 066h, 020h, 072h, 061h, 06Eh, 067h, 065h, 03Ah, 020h, 000h, 04Ch
        db  049h, 06Eh, 076h, 061h, 06Ch, 069h, 064h, 020h, 044h, 045h, 043h, 06Eh, 065h, 074h, 020h, 06Eh
        db  06Fh, 064h, 065h, 020h, 06Eh, 075h, 06Dh, 062h, 065h, 072h, 03Ah, 020h, 000h, 046h, 000h, 044h
        db  049h, 06Eh, 076h, 061h, 06Ch, 069h, 064h, 020h, 062h, 06Fh, 06Fh, 06Ch, 065h, 061h, 06Eh, 020h
        db  076h, 061h, 06Ch, 075h, 065h, 02Ch, 020h, 075h, 073h, 065h, 020h, 027h, 059h, 045h, 053h, 027h
        db  020h, 06Fh, 072h, 020h, 027h, 04Eh, 04Fh, 027h, 03Ah, 020h, 000h, 043h, 049h, 06Eh, 076h, 061h
        db  06Ch, 069h, 064h, 020h, 063h, 06Fh, 06Dh, 06Dh, 020h, 070h, 06Fh, 072h, 074h, 020h, 06Eh, 061h
        db  06Dh, 065h, 02Ch, 020h, 075h, 073h, 065h, 020h, 043h, 04Fh, 04Dh, 031h, 02Dh, 043h, 04Fh, 04Dh
        db  034h, 03Ah, 020h, 000h, 049h, 06Eh, 076h, 061h, 06Ch, 069h, 064h, 020h, 049h, 050h, 020h, 061h
        db  064h, 064h, 072h, 065h, 073h, 073h, 03Ah, 020h, 000h, 072h, 03Ah, 020h, 049h, 06Eh, 076h, 061h
        db  06Ch, 069h, 064h, 020h, 073h, 079h, 06Eh, 074h, 061h, 078h, 02Ch, 020h, 075h, 073h, 065h, 020h
        db  04Eh, 041h, 04Dh, 045h, 020h, 03Dh, 020h, 056h, 041h, 04Ch, 055h, 045h, 000h, 020h, 000h, 04Ch
        db  055h, 06Eh, 072h, 065h, 063h, 06Fh, 067h, 06Eh, 069h, 07Ah, 065h, 064h, 020h, 070h, 061h, 072h
        db  061h, 06Dh, 065h, 074h, 065h, 072h, 020h, 06Eh, 061h, 06Dh, 065h, 03Ah, 020h, 000h, 000h, 044h
        db  043h, 06Fh, 075h, 06Ch, 064h, 020h, 06Eh, 06Fh, 074h, 020h, 06Fh, 070h, 065h, 06Eh, 020h, 066h
        db  069h, 06Ch, 065h, 03Ah, 020h, 000h, 020h, 075h, 043h, 06Fh, 075h, 06Ch, 064h, 020h, 06Eh, 06Fh
        db  074h, 020h, 072h, 065h, 061h, 064h, 020h, 066h, 069h, 06Ch, 065h, 03Ah, 020h, 000h, 076h, 061h
        db  043h, 06Fh, 075h, 06Ch, 064h, 020h, 06Eh, 06Fh, 074h, 020h, 06Fh, 070h, 065h, 06Eh, 020h, 072h
        db  065h, 067h, 069h, 073h, 074h, 072h, 079h, 020h, 06Bh, 065h, 079h, 03Ah, 020h, 000h, 000h, 000h
        db  043h, 06Fh, 075h, 06Ch, 064h, 020h, 06Eh, 06Fh, 074h, 020h, 061h, 063h, 063h, 065h, 073h, 073h
        db  020h, 06Bh, 065h, 079h, 077h, 06Fh, 072h, 064h, 020h, 069h, 06Eh, 020h, 072h, 065h, 067h, 069h
        db  073h, 074h, 072h, 079h, 03Ah, 020h, 000h, 064h, 049h, 06Eh, 076h, 061h, 06Ch, 069h, 064h, 020h
        db  06Bh, 065h, 079h, 077h, 06Fh, 072h, 064h, 020h, 074h, 079h, 070h, 065h, 020h, 069h, 06Eh, 020h
        db  072h, 065h, 067h, 069h, 073h, 074h, 072h, 079h, 03Ah, 020h, 000h, 067h, 030h, 031h, 032h, 033h
        db  034h, 035h, 036h, 037h, 038h, 039h, 041h, 042h, 043h, 044h, 045h, 046h, 000h, 061h, 06Dh, 065h
        db  000h, 020h, 000h, 000h, 07Bh, 025h, 025h, 031h, 025h, 073h, 028h, 025h, 075h, 029h, 07Dh, 03Ah
        db  020h, 07Bh, 025h, 025h, 032h, 025h, 073h, 07Dh, 020h, 025h, 073h, 00Dh, 00Ah, 000h, 06Fh, 075h
        db  07Bh, 025h, 025h, 032h, 025h, 073h, 07Dh, 020h, 025h, 073h, 00Dh, 00Ah, 000h, 000h, 000h, 000h
        db  05Ch, 000h, 000h, 000h, 043h, 03Ah, 05Ch, 000h, 04Dh, 045h, 04Dh, 049h, 04Eh, 055h, 053h, 045h
        db  000h, 000h, 000h, 000h, 041h, 04Ch, 04Ch, 04Fh, 043h, 043h, 04Eh, 054h, 000h, 000h, 000h, 000h
        db  052h, 045h, 051h, 000h, 044h, 045h, 043h, 000h, 054h, 055h, 04Eh, 045h, 05Fh, 05Fh, 05Fh, 030h
        db  000h, 000h, 000h, 000h
sub_4_04F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        inc    ecx
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [esi + 0x35], dl
        and    byte ptr [esi + 0x6f], ah
        jb     0x529
        inc    esp
        dec    edi
        push   ebx
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     loc_4_058B
        and    byte ptr [eax], ch
        dec    esi
        je     0x575
        popal  
        jb     loc_4_0586
        sub    dword ptr [eax], esp
        inc    ebx
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebp + 0x6e], 0x694c2074
        arpl   word ptr [ebp + 0x6e], sp
        jae    0x596
        and    byte ptr [edx + 0x65], dl
        jno    0x5ab
        jae    0x5ad
        jb     0x55c
        and    eax, 0x430a2e73
        outsd  dx, dword ptr [esi]
        jo     loc_4_05BD
        jb     loc_4_05AF
        push   0x43282074
        sub    dword ptr [eax], esp
        xor    dword ptr [ecx], edi
        cmp    dword ptr [ebx], esi
        sub    eax, 0x34393931
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ecx + ebp*2 + 0x67], al
        imul   esi, dword ptr [ecx + 0x6c], 0x75714520
        imul   esi, dword ptr [eax + 0x6d], 0x20746e65
        inc    ebx
        outsd  dx, dword ptr [esi]
        jb     0x5e1
        outsd  dx, dword ptr [esi]
        jb     loc_4_05D5
        je     loc_4_05DF
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        or     al, byte ptr cs:[ecx + 0x6c]
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x69], dl
        push   0x52207374
loc_4_0586:
        jae    loc_4_05EE
        jb     loc_4_0601
loc_4_058B:
        or     cl, byte ptr cs:[edx]
        add    byte ptr [ebp + 0x73], dl
        popal  
        cmp    ah, byte ptr gs:[bx + si]
        jo     loc_4_0611
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x6e], 0x2f5b2077
        imul   edi, dword ptr [edx], 0x68746170
        pop    ebp
        and    byte ptr [ebx + 0x2f], bl
        jae    0x616
        outsd  dx, dword ptr [esi]
loc_4_05AF:
        ja     loc_4_060E
        and    byte ptr [ebx + 0x2f], bl
        je     0x5f0
        jae    0x61d
        arpl   word ptr [ebp + 0x20], bx
        pop    ebx
        das    
loc_4_05BD:
        jb     0x62f
        jne    loc_4_0632
        cmp    ah, byte ptr [edi + 0x72]
        outsd  dx, dword ptr [esi]
        jne    0x638
        pop    ebp
        and    byte ptr [ebx + 0x2f], bl
        imul   bp, word ptr [ebp + 0x3a], 0x696c
        arpl   word ptr [esi], bp
loc_4_05D5:
        je     0x644
        jo     0x636
        and    byte ptr [ebx + 0x73], bl
        jb     0x655
loc_4_05DF:
        jb     loc_4_063F
        or     dl, byte ptr [edi + 0x68]
        jb     0x64d
        cmp    cl, byte ptr [edx]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_05EE:
        das    
        imul   edi, dword ptr [edx], 0x68746170
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [0x636f4c20], bh
loc_4_0601:
        popal  
        je     loc_4_066D
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [eax + 0x57], dl
        dec    esp
        dec    ecx
loc_4_060E:
        inc    ebx
        dec    esi
        push   edi
loc_4_0611:
        inc    esp
        inc    ecx
        push   esp
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        or     ah, byte ptr gs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [edi], ch
        jae    loc_4_068A
        outsd  dx, dword ptr [esi]
        ja     0x645
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        cmp    eax, 0x73694420
loc_4_0632:
        jo     loc_4_06A0
        popal  
        jns    0x657
        arpl   word ptr [ecx + ebp*2 + 0x65], bp
        outsb  dx, byte ptr [esi]
        je     loc_4_065E
        insb   byte ptr es:[edi], dx
loc_4_063F:
        imul   esp, dword ptr [ebx + 0x65], 0x7365736e
        or     ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [edi], ch
        je     0x688
        jae    loc_4_06B5
        arpl   word ptr [eax], sp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [0x6d754e20], bh
loc_4_065E:
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ebx + 0x65], dh
        arpl   word ptr [edi + 0x6e], bp
        jae    0x68d
loc_4_066D:
        je     0x6de
        and    byte ptr [edi + 0x61], dh
        imul   esi, dword ptr [eax + 0x66], 0x7220726f
        jae    loc_4_06ED
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jae    loc_4_06E6
        and    byte ptr [esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + ebp*2 + 0x63], ch
loc_4_068A:
        outsb  dx, byte ptr gs:[esi]
        jae    0x6f3
        and    byte ptr [ebx + 0x65], dh
        jb     loc_4_0709
        jb     loc_4_06A0
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        das    
        jb     loc_4_070D
        jne    0x710
loc_4_06A0:
        cmp    ah, byte ptr [edi + 0x72]
        outsd  dx, dword ptr [esi]
        jne    0x716
        and    byte ptr [eax], ah
        cmp    eax, 0x6d614e20
        and    byte ptr gs:[edi + 0x66], ch
        and    byte ptr [edi + 0x72], ah
        outsd  dx, dword ptr [esi]
loc_4_06B5:
        jne    loc_4_0727
        and    byte ptr [esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x68], dh
        imul   esp, dword ptr [ebx + 0x68], 0x206f7420
        jb     0x72d
        jno    0x73f
        jae    0x741
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    0x73a
        or     ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [edi], ch
        imul   bp, word ptr [ebp + 0x3a], 0x696c
        arpl   word ptr [esi], bp
        je     loc_4_0753
loc_4_06E6:
        jo     0x708
        cmp    eax, 0x63694c20
loc_4_06ED:
        outsb  dx, byte ptr gs:[esi]
        jae    0x756
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        or     ah, byte ptr gs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 0x65], dh
        jb     0x77a
        jb     loc_4_0727
        and    byte ptr [eax], ah
loc_4_0709:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_070D:
        and    byte ptr [0x6d614e20], bh
        and    byte ptr gs:[edi + 0x66], ch
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    0x784
        and    byte ptr [ebx + 0x65], dh
        jb     loc_4_079A
        jb     0x747
loc_4_0727:
        jb     0x799
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x68], dh
        imul   esp, dword ptr [ebx + 0x68], 0x206f7420
        jb     0x79c
        jno    loc_4_07AE
        jae    0x7b0
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    0x7a9
        or     ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [edi], ch
        aas    
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_0753:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        cmp    eax, 0x73694420
        jo     loc_4_07CA
        popal  
        jns    0x781
        je     0x7cb
        imul   esi, dword ptr [ebx + 0x20], 0x7373656d
        popal  
        or     al, byte ptr gs:[bx + si]
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    byte ptr [ecx], dh
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        jb     0x7e4
        arpl   word ptr [edi + 0x67], bp
        outsb  dx, byte ptr [esi]
        imul   edi, dword ptr [edx + 0x65], 0x72612064
        jne    loc_4_07FA
        outsb  dx, byte ptr gs:[esi]
        je     loc_4_07B1
        and    eax, 0xa2e73
        or     dl, byte ptr [ecx + ebp*2 + 0x6d]
loc_4_079A:
        sub    eax, 0x2074756f
        jbe    loc_4_0803
        insb   byte ptr es:[edi], dx
        jne    loc_4_080A
        and    byte ptr [ebx + 0x65], dh
        je     loc_4_07CA
        je     0x81b
        and    byte ptr [ecx], dh
loc_4_07AE:
        and    byte ptr [ebx + 0x65], dh
loc_4_07B1:
        arpl   word ptr [edi + 0x6e], bp
        or     al, byte ptr cs:[eax]
        or     dl, byte ptr [ecx + ebp*2 + 0x6d]
        sub    eax, 0x2074756f
        jbe    0x825
        insb   byte ptr es:[edi], dx
        jne    loc_4_082C
        and    byte ptr [ebx + 0x65], dh
loc_4_07CA:
        je     0x7ec
        je     0x83d
        and    byte ptr [0x65732064], ah
        arpl   word ptr [edi + 0x6e], bp
        jae    0x808
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dword ptr [edx + edi], esi
        and    byte ptr [ecx + 0x6e], cl
        jbe    0x84b
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x74], 0x2d656d69
        outsd  dx, dword ptr [esi]
        jne    0x86a
        and    byte ptr [esi + 0x61], dh
        insb   byte ptr es:[edi], dx
loc_4_07FA:
        jne    0x861
        and    byte ptr [ebx + 0x70], dh
        arpl   word ptr gs:[ecx + 0x66], bp
loc_4_0803:
        imul   esp, dword ptr [ebp + 0x64], 0x2e732520
loc_4_080A:
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    al, 0x32
        cmp    ah, byte ptr [eax]
        push   eax
        popal  
        je     loc_4_0882
        and    byte ptr [edi + ebp*2 + 0x63], ch
        popal  
        je     0x88a
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x6c], ah
        jb     loc_4_088D
        popal  
        jns    0x84c
loc_4_082C:
        jae    0x89e
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0xa000a2e
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    dword ptr [eax], edi
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    0x8a7
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x70], 0x20687461
        jae    0x8c1
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0x2e732520
        and    byte ptr [eax], ch
        insd   dword ptr es:[edi], dx
        popal  
        js     loc_4_08CB
        insd   dword ptr es:[edi], dx
        jne    0x8d2
        and    byte ptr [ebp + 0x6e], ch
        je     0x8d4
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [0x68632064], ah
        popal  
        jb     0x8d9
        arpl   word ptr [ebp + 0x72], si
        jae    0x8ac
        sub    dword ptr [edx], ecx
        add    byte ptr [edx], cl
loc_4_0882:
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    dword ptr [ecx], edi
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
loc_4_088D:
        jbe    0x8f0
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x6c], 0x6e656369
        jae    loc_4_08FF
        and    byte ptr [ebx + 0x65], dh
        jb     loc_4_0915
        jb     loc_4_08C2
        jae    0x914
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0x2e732520
        and    byte ptr [eax], ch
        insd   dword ptr es:[edi], dx
        popal  
        js     0x91e
        insd   dword ptr es:[edi], dx
        jne    0x925
        and    byte ptr [ebp + 0x6e], ch
        je     0x927
        and    byte ptr [ecx + 0x73], ch
loc_4_08C2:
        and    byte ptr [0x68632064], ah
        popal  
        jb     loc_4_092C
loc_4_08CB:
        arpl   word ptr [ebp + 0x72], si
        jae    loc_4_08FF
        sub    dword ptr [edx], ecx
        add    byte ptr [edx], cl
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    dh, byte ptr [eax]
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    0x943
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x67], 0x70756f72
        and    byte ptr [ebx + 0x70], dh
        arpl   word ptr gs:[ecx + 0x66], bp
        imul   esp, dword ptr [ebp + 0x64], 0x2e732520
        and    byte ptr [eax], ch
        insd   dword ptr es:[edi], dx
        popal  
        js     0x968
loc_4_08FF:
        insd   dword ptr es:[edi], dx
        jne    loc_4_096F
        and    byte ptr [ebp + 0x6e], ch
        je     loc_4_0971
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [0x68632064], ah
        popal  
        jb     0x976
loc_4_0915:
        arpl   word ptr [ebp + 0x72], si
        jae    loc_4_0949
        sub    dword ptr [edx], ecx
        add    byte ptr [edx], cl
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], bh
        cmp    ah, byte ptr [eax]
        dec    esi
        je     0x9a3
loc_4_092C:
        outsd  dx, dword ptr [esi]
        jb     loc_4_099A
        and    byte ptr [ebx + 0x6f], dh
        je     loc_4_09AC
        popal  
        jb     0x99d
        and    byte ptr [esi + 0x6f], ch
        je     loc_4_095D
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[edi + 0x72], ch
        and    byte ptr [eax + 0x72], dh
loc_4_0949:
        outsd  dx, dword ptr [esi]
        bound  ebp, qword ptr [ebp + 0x6d]
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6972
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + 0x65], ch
loc_4_095D:
        je     0x9d6
        outsd  dx, dword ptr [esi]
        jb     0x9cd
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    byte ptr [ecx], bh
        cmp    ah, byte ptr [eax]
loc_4_096F:
        inc    ecx
        insb   byte ptr es:[edi], dx
loc_4_0971:
        insb   byte ptr es:[edi], dx
        and    byte ptr [esi + 0x65], ch
        je     loc_4_09EE
        outsd  dx, dword ptr [esi]
        jb     0x9e5
        and    byte ptr [ebx + 0x6f], dh
        arpl   word ptr [ebx + 0x65], bp
        je     0x9f5
        and    byte ptr [ecx + 0x72], ah
        and    byte ptr gs:[ebx + 0x75], ah
        jb     loc_4_09FD
        outsb  dx, byte ptr gs:[esi]
        je     loc_4_09FB
        jns    loc_4_09B1
        imul   ebp, dword ptr [esi + 0x20], 0x2e657375
        or     al, byte ptr [eax]
loc_4_099A:
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dh, byte ptr [ecx]
        cmp    ah, byte ptr [eax]
        imul   ebp, dword ptr [esi + 0x74], 0x616e7265
        insb   byte ptr es:[edi], dx
loc_4_09AC:
        and    byte ptr [ebp + 0x72], ah
        jb     0xa20
loc_4_09B1:
        jb     loc_4_09D3
        outsb  dx, byte ptr [esi]
        je     0xa2e
        outsd  dx, dword ptr [esi]
        jb     loc_4_0A25
        and    byte ptr [edx + 0x75], ah
        jb     0x9e2
        je     0xa33
        outsd  dx, dword ptr [esi]
        and    byte ptr [ebx + 0x6d], dh
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr cs:[eax], ch
        and    eax, 0x79622064
loc_4_09D3:
        je     loc_4_0A3A
        jae    0x9f7
        insb   byte ptr es:[edi], dx
        jae    loc_4_0A4E
        and    byte ptr [eax + ebp*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x69], ch
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [ebp + 0x75], 0x6425206d
        and    byte ptr [edx + 0x79], ah
loc_4_09EE:
        je     0xa55
        jae    0xa12
        outsb  dx, byte ptr [esi]
        sub    dword ptr cs:[edx], ecx
loc_4_09FB:
        add    byte ptr [edx], cl
loc_4_09FD:
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    dword ptr [edi], esi
        cmp    ah, byte ptr [eax]
        dec    esi
        je     0xa81
        outsd  dx, dword ptr [esi]
        jb     loc_4_0A78
        and    byte ptr [esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[esi + 0x6f], ch
        je     0xa37
        jae    loc_4_0A7E
        je     0xa49
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dh, byte ptr [edx]
loc_4_0A25:
        cmp    ah, byte ptr [eax]
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    loc_4_0A97
        and    byte ptr fs:[esi + 0x6f], ch
        je     0xa51
        popal  
        and    byte ptr fs:[ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
loc_4_0A3A:
        jae    loc_4_0AA1
        and    byte ptr [esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[edi + ebp*2 + 0x20], dh
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x6c], sp
        and    byte ptr [ecx + 0x62], dh
loc_4_0A4E:
        insb   byte ptr es:[edi], dx
        jae    loc_4_0A80
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dh, byte ptr [ebx]
        cmp    ah, byte ptr [eax]
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    0xace
        and    byte ptr fs:[esi + 0x6f], ch
        je     0xa88
        jb     0xacf
        je     0xade
        imul   esp, dword ptr [ebp + 0x76], 0x74732065
        popal  
        je     loc_4_0ADF
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_4_0A78:
        and    byte ptr [ecx + 0x64], ah
        jb     loc_4_0AE3
loc_4_0A7E:
        jae    0xaf3
loc_4_0A80:
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dh, byte ptr [edx + edi]
        and    byte ptr [ebx + 0x6f], al
        jne    loc_4_0AFD
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_0AB7
loc_4_0A97:
        jo     0xb08
        jae    0xb0f
        and    byte ptr [edx + 0x65], dh
        arpl   word ptr [ebp + 0x69], sp
loc_4_0AA1:
        jbe    0xb08
        and    byte ptr [edx + 0x75], ah
        jb     0xad9
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dh, byte ptr [edi]
        cmp    ah, byte ptr [eax]
loc_4_0AB7:
        push   edi
        popal  
        jb     loc_4_0B29
        imul   ebp, dword ptr [esi + 0x67], 0x756f6320
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_0AE9
        jo     loc_4_0B3A
        jae    0xb41
        and    byte ptr [ecx + ebp*2 + 0x73], ch
        je     0xb38
        outsb  dx, byte ptr [esi]
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    bh, byte ptr [eax]
loc_4_0ADF:
        cmp    ah, byte ptr [eax]
        inc    ebx
        outsd  dx, dword ptr [esi]
loc_4_0AE3:
        jne    loc_4_0B51
        and    byte ptr fs:[esi + 0x6f], ch
loc_4_0AE9:
        je     0xb0b
        arpl   word ptr [edi + 0x6e], bp
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[eax + 0x74], si
        outsd  dx, dword ptr [esi]
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_0B62
loc_4_0AFD:
        and    byte ptr [ebx + 0x65], dh
        jb     0xb78
        jb     0xb25
        and    eax, 0xa2e73
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    bh, byte ptr [ecx]
        cmp    ah, byte ptr [eax]
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    0xb84
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_0B3E
        jae    loc_4_0B88
        jne    0xb96
        outsd  dx, dword ptr fs:[esi]
        ja     loc_4_0B94
        and    byte ptr [esi + 0x45], cl
loc_4_0B29:
        push   esp
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
        and    byte ptr [ebx + 0x65], dh
        jae    loc_4_0BA6
        imul   ebp, dword ptr [edi + 0x6e], 0xa202e
loc_4_0B3A:
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
loc_4_0B3E:
        xor    byte ptr [eax], dh
        xor    esi, dword ptr [eax]
        cmp    ah, byte ptr [eax]
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    0xbb4
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_0B6E
        jae    loc_4_0BB5
        outsb  dx, byte ptr [esi]
loc_4_0B51:
        and    byte ptr fs:[edx + 0x65], dh
        jno    loc_4_0BCC
        jae    loc_4_0BCE
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jae    0xbc5
        jb     0xbd8
loc_4_0B62:
        jb     0xb85
        and    eax, 0xa202e73
        add    byte ptr [edx], cl
        dec    esp
        dec    ecx
loc_4_0B6E:
        inc    ebx
        xor    byte ptr [eax], dh
        xor    esi, dword ptr [ecx]
        cmp    ah, byte ptr [eax]
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    loc_4_0BE5
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_0B9F
        jo     0xbf0
        jae    0xbf7
        and    byte ptr [ecx + 0x6e], ah
        jns    0xba8
loc_4_0B88:
        insb   byte ptr es:[edi], dx
        imul   esi, dword ptr [ebx + 0x74], 0x63206e65
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        popal  
loc_4_0B94:
        outsb  dx, byte ptr [esi]
        jae    loc_4_0BC6
        and    byte ptr [edx], cl
        add    byte ptr [ebp + eax*2 + 0x42], al
        push   ebp
loc_4_0B9F:
        inc    edi
        cmp    ah, byte ptr [eax]
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    0xc12
loc_4_0BA6:
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_0BCC
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x6c], sp
        and    byte ptr [ebx + 0x6f], ah
loc_4_0BB5:
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr cs:[edx], cl
        add    byte ptr [edx], cl
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    edi, dword ptr [ecx]
loc_4_0BC6:
        cmp    ah, byte ptr [eax]
        push   edi
        inc    ecx
        push   edx
        dec    esi
loc_4_0BCC:
        dec    ecx
        dec    esi
loc_4_0BCE:
        inc    edi
        and    byte ptr [ebp + eax*2 + 0x43], al
        dec    esi
        inc    ebp
        push   esp
        and    byte ptr [edi + 0x69], dl
        and    byte ptr gs:[ecx + 0x72], al
        popal  
        and    byte ptr [esi + 0x65], cl
        je     loc_4_0C5C
loc_4_0BE5:
        outsd  dx, dword ptr [esi]
        jb     0xc53
        and    byte ptr [esi + 0x6f], ch
        je     loc_4_0C0D
        jae    0xc64
        jo     0xc61
        outsd  dx, dword ptr [esi]
        jb     0xc68
        and    byte ptr fs:[edi + 0x69], dh
        je     0xc63
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    0xc21
        arpl   word ptr [edi + 0x6e], bp
        imul   sp, word ptr [edi + 0x75], 0x6172
        je     0xc75
        outsd  dx, dword ptr [esi]
loc_4_0C0D:
        outsb  dx, byte ptr [esi]
        or     ah, byte ptr [ebx + 0x68]
        arpl   word ptr gs:[ebx + 0x20], bp
        imul   esp, dword ptr [esi + 0x20], 0x65646f6e
        and    byte ptr [0x73692073], ah
        and    byte ptr [ebp + 0x66], ah
        imul   ebp, dword ptr [esi + 0x65], 0x6e692064
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + eax*2 + 0x43], al
        dec    esi
        inc    ebp
        push   esp
        and    byte ptr [ecx + 0x74], ah
        popal  
        bound  esp, qword ptr [ecx + 0x73]
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    al, 0x30
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xcc6
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
loc_4_0C5C:
        jne    0xccc
        imul   esp, dword ptr [ebx + 0x61], 0x6f206574
        jbe    0xccc
        jb     0xc89
        outsb  dx, byte ptr [esi]
        je     0xce4
        outsd  dx, dword ptr [esi]
        jb     loc_4_0CDB
        sub    al, 0x20
        imul   ebp, dword ptr [esi + 0x63], 0x73616572
        and    byte ptr gs:[esi + 0x75], ch
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ebx + 0x69], dh
        insd   dword ptr es:[edi], dx
        jne    0xcf6
        je     loc_4_0CED
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr gs:[esi]
        jne    0xd04
        or     ah, byte ptr [ebx + 0x6f]
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        popal  
        outsb  dx, byte ptr [esi]
        jae    0xcbb
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        ja     0xd06
        and    byte ptr fs:[edx + 0x79], ah
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    0xd1c
        outsd  dx, dword ptr [esi]
        jb     loc_4_0D23
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[esi + 0x75], ch
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ebx + 0x65], dh
        jae    0xd35
        imul   ebp, dword ptr [edi + 0x6e], 0x6c612073
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        ja     0xd32
        and    byte ptr fs:[edx + 0x79], ah
        and    byte ptr [edx + esi*2 + 0x61], dh
        outsb  dx, byte ptr [esi]
        jae    loc_4_0D48
        outsd  dx, dword ptr [esi]
        jb     loc_4_0D4F
loc_4_0CDB:
        or     al, byte ptr cs:[eax]
        inc    esp
        inc    ebp
        inc    edx
        push   ebp
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    esi
        outsd  dx, dword ptr [esi]
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
loc_4_0CED:
        jae    0xd54
        jae    0xd11
        popal  
        jbe    0xd55
        imul   ebp, dword ptr [ecx + 0x62], 0x7420656c
        outsd  dx, dword ptr [esi]
        and    byte ptr [edi + ebp*2 + 0x61], ch
        and    byte ptr fs:[ecx + 0x6e], ch
        je     0xd76
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_0D74
        and    byte ptr [ebx + edx*2 + 0x52], dl
        or     al, byte ptr cs:[eax]
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        imul   ebp, dword ptr fs:[esi + 0x67], 0x63696c20
        outsb  dx, byte ptr gs:[esi]
loc_4_0D23:
        jae    0xd8a
        jae    0xd47
        imul   ebp, dword ptr [esi + 0x74], 0x696c206f
        arpl   word ptr [ebp + 0x6e], sp
        jae    0xd98
        and    byte ptr [edx + 0x65], dl
        jae    0xda8
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     0xd6c
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    byte ptr [edx], dh
loc_4_0D48:
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        arpl   word ptr [edi + 0x6d], bp
loc_4_0D4F:
        jo     loc_4_0DB2
        je     0xdbc
        bound  ebp, qword ptr [ebp + 0x20]
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        jbe    loc_4_0DC6
        jb     0xdd6
        imul   ebp, dword ptr [edi + 0x6e], 0xa2e73
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    esi, dword ptr [edx]
        cmp    ah, byte ptr [eax]
loc_4_0D74:
        imul   ebp, dword ptr [esi + 0x63], 0x61706d6f
        je     0xde6
        bound  ebp, qword ptr [ebp + 0x20]
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        jb     0xdf0
        arpl   word ptr [edi + 0x72], bp
        jae    0xdbf
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    esi, dword ptr [ebx]
        cmp    ah, byte ptr [eax]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xdc1
        outsb  dx, byte ptr gs:[esi]
        outsd  dx, dword ptr [esi]
        jne    0xe0d
        push   0x6d656d20
        outsd  dx, dword ptr [esi]
        jb     loc_4_0E27
        and    byte ptr [ecx + 0x76], ah
        popal  
loc_4_0DB2:
        imul   ebp, dword ptr [ecx + 0x62], 0x6620656c
        outsd  dx, dword ptr [esi]
        jb     loc_4_0DDD
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
loc_4_0DC6:
        jae    loc_4_0E39
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     0xdfd
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    byte ptr [0x6142203a], dh
loc_4_0DDD:
        and    byte ptr fs:[ebx + 0x65], dh
        insd   dword ptr es:[di], dx
        outsb  dx, byte ptr gs:[esi]
        je     0xe07
        outsd  dx, dword ptr [esi]
        jb     loc_4_0E4E
        jb     0xe56
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + di + 0x6e], ch
        and    byte ptr [ebp + 0x58], al
        inc    ebp
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    byte ptr [esi], dh
        cmp    ah, byte ptr [eax]
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jae    0xe84
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     0xe3a
        arpl   word ptr [ecx + 0x6e], sp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xe41
        bound  esp, qword ptr [ebp + 0x20]
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
loc_4_0E27:
        and    byte ptr fs:[ebp + 0x6e], dh
        jb     loc_4_0E51
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x6f20776f
loc_4_0E39:
        jb     loc_4_0E5B
        je     0xea5
        and    byte ptr gs:[ecx + 0x73], dh
        imul   esp, dword ptr [eax], 0x73
        ja     loc_4_0EB0
        je     0xeac
        push   0xa2e7265
loc_4_0E4E:
        add    byte ptr [edx], cl
        dec    esp
loc_4_0E51:
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    byte ptr [edi], dh
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
loc_4_0E5B:
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xed1
        and    byte ptr [edx + 0x65], dh
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_4_0ECF
        and    byte ptr [ebp + 0x78], ah
        arpl   word ptr [ebp + 0x73], sp
        jae    loc_4_0E92
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0xef1
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], bh
        cmp    ah, byte ptr [eax]
        dec    esi
        je     loc_4_0F00
        outsd  dx, dword ptr [esi]
        jb     0xef7
        and    byte ptr [ebx + 0x6f], dh
        je     0xf09
loc_4_0E92:
        popal  
        jb     0xefa
        and    byte ptr [esi + 0x6f], ch
        je     0xeba
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    byte ptr [ecx], bh
        cmp    ah, byte ptr [eax]
        inc    ecx
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
loc_4_0EB0:
        and    byte ptr [esi + 0x65], ch
        je     0xf2c
        outsd  dx, dword ptr [esi]
        jb     loc_4_0F23
        and    byte ptr [ebx + 0x6f], dh
        arpl   word ptr [ebx + 0x65], bp
        je     loc_4_0F33
        and    byte ptr [ecx + 0x72], ah
        and    byte ptr gs:[ebx + 0x75], ah
        jb     0xf3b
        outsb  dx, byte ptr gs:[esi]
        je     0xf39
        jns    loc_4_0EEF
loc_4_0ECF:
        imul   ebp, dword ptr [esi + 0x20], 0x2e657375
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dword ptr [eax], esi
        cmp    ah, byte ptr [eax]
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jae    loc_4_0F5E
        outsd  dx, dword ptr [esi]
loc_4_0EEF:
        outsb  dx, byte ptr [esi]
        jb     0xf14
        popal  
        insb   byte ptr es:[edi], dx
        jb     loc_4_0F5D
        popal  
        jns    loc_4_0F1C
        jb     loc_4_0F63
        jae    0xf69
loc_4_0F00:
        outsb  dx, byte ptr gs:[esi]
        je     0xf25
        imul   ebp, dword ptr [esi + 0x20], 0x6f6d656d
        jb     0xf87
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dword ptr [ecx], esi
        cmp    ah, byte ptr [eax]
        dec    esp
loc_4_0F1C:
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
loc_4_0F23:
        push   edx
        jae    0xf97
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     0xf4d
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xf51
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
loc_4_0F33:
        popal  
        and    byte ptr fs:[ecx + 0x6e], ch
        and    byte ptr [ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0xfba
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dh, byte ptr [0x6e69203a]
        je     loc_4_0FB7
        jb     0xfc2
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x72], ah
        jb     0xfca
        jb     loc_4_0F7D
loc_4_0F5D:
        outsd  dx, dword ptr [esi]
loc_4_0F5E:
        arpl   word ptr [ebx + 0x75], sp
        jb     0xfc8
loc_4_0F63:
        or     al, byte ptr cs:[eax]
        or     al, byte ptr [ebp + eax*2 + 0x42]
        push   ebp
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jae    0xfeb
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_4_0F7D:
        jb     loc_4_0FA1
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[edi + 0x4b], cl
        add    byte ptr [eax + 0x41], dl
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    0x1001
        and    byte ptr [edx + 0x65], dl
        jae    0x1011
loc_4_0FA1:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     loc_4_0FC7
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        add    byte ptr fs:[ebp + eax*2 + 0x42], al
        push   ebp
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
loc_4_0FB7:
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x102d
        and    byte ptr [edi + 0x70], ch
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [ecx + ebp*2 + 0x63], ch
loc_4_0FC7:
        outsb  dx, byte ptr gs:[esi]
        jae    0x1030
        and    byte ptr [ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     0x103f
        popal  
        je     0x103e
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[0x6f662073], ah
        jb     0x1004
        jb     0x104b
        popal  
        imul   ebp, dword ptr fs:[esi + 0x67], 0xa0a2e
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dword ptr [edx], esi
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x1071
        and    byte ptr [edx + 0x65], dh
        popal  
        and    byte ptr fs:[ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_1074
        and    byte ptr [ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     loc_4_1083
        popal  
        je     0x1082
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [edx + 0x65], dh
        imul   esi, dword ptr [bp + di + 0x74], 0xa2e7972
        or     al, byte ptr [eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    esi, dword ptr [edx + edi]
        and    byte ptr [ebp + 0x63], al
        jb     loc_4_10B3
        jo     0x10b0
        imul   ebp, dword ptr [edi + 0x6e], 0x20666f20
        imul   bp, word ptr [ebp + 0x20], 0x6166
        imul   ebp, dword ptr [ebp + 0x64], 0xa0a2e
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    edi, dword ptr [eax]
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x10d4
        and    byte ptr [edi + 0x70], ch
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [ebp + 0x73], dh
        jb     0x1090
        jae    0x10e7
        jo     loc_4_10E4
loc_4_1074:
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebp + 0x64], 0x63696c20
        outsb  dx, byte ptr gs:[esi]
        jae    0x10e5
        and    byte ptr [esi + 0x69], ah
loc_4_1083:
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[0xa202e73], ah
        add    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    esi, dword ptr [0x6e55203a]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x110d
        and    byte ptr [edi + 0x72], dh
        imul   esi, dword ptr [ebp + 0x20], 0x6563696c
        outsb  dx, byte ptr [esi]
        jae    loc_4_1111
        and    byte ptr [ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     loc_4_1120
loc_4_10B3:
        popal  
        je     0x111f
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jb     0x1123
        imul   esi, dword ptr [bp + di + 0x74], 0xa2e7972
        or     dword ptr [eax], esp
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        imul   ebp, dword ptr [esi + 0x66], 0x616d726f
        je     loc_4_1142
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [esi + 0x6f], ch
        je     0x1100
        jae    0x1143
        jbe    0x1149
loc_4_10E4:
        or     cl, byte ptr cs:[edx]
        add    byte ptr [edx], cl
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    esi, dword ptr [esi]
        cmp    ah, byte ptr [eax]
        dec    esi
        outsd  dx, dword ptr [esi]
        je     0x1117
        outsb  dx, byte ptr gs:[esi]
        outsd  dx, dword ptr [esi]
        jne    0x1163
        push   0x6d656d20
        outsd  dx, dword ptr [esi]
        jb     loc_4_117D
        and    byte ptr [ecx + 0x76], ah
        popal  
        imul   ebp, dword ptr [ecx + 0x62], 0x7420656c
        outsd  dx, dword ptr [esi]
loc_4_1111:
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jb     loc_4_117D
        insd   dword ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
loc_4_1120:
        inc    ebx
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebp + 0x6e], 0x6e652074
        jbe    loc_4_1194
        jb     loc_4_119C
        outsb  dx, byte ptr [esi]
        insd   dword ptr es:[edi], dx
        outsb  dx, byte ptr gs:[esi]
        je     0x1153
        imul   ebp, dword ptr [esi + 0x66], 0x616d726f
        je     loc_4_11A5
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        cmp    cl, byte ptr [edx]
        and    byte ptr [eax], ah
loc_4_1142:
        and    byte ptr [eax], ah
        inc    ebx
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebp + 0x6e], 0x64692074
        cmp    ah, byte ptr [eax]
        and    eax, 0x25583230
        xor    byte ptr [edx], dh
        pop    eax
        and    eax, 0x25583230
        xor    byte ptr [edx], dh
        pop    eax
        and    eax, 0x25583230
        xor    byte ptr [edx], dh
        pop    eax
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x11e5
        and    byte ptr [ebp + 0x74], ah
        jb     0x11ea
loc_4_117D:
        imul   ebp, dword ptr [esi + 0x65], 0x72657320
        jbe    loc_4_11EB
        jb     loc_4_11A8
        popal  
        arpl   word ptr [ebx + 0x6f], sp
        jne    loc_4_11FC
        je     loc_4_11B0
        arpl   word ptr [ecx + ebp*2 + 0x65], bp
loc_4_1194:
        outsb  dx, byte ptr [esi]
        je     loc_4_11B7
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jae    0x11bc
loc_4_119C:
        imul   ebp, dword ptr [esi + 0x74], 0xa2e6f
        and    byte ptr [eax], ah
loc_4_11A5:
        and    byte ptr [eax], ah
        inc    ebx
loc_4_11A8:
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebp + 0x6e], 0x63612074
loc_4_11B0:
        arpl   word ptr [ebp + 0x73], sp
        jae    loc_4_121A
        jae    loc_4_11D7
loc_4_11B7:
        popal  
        arpl   word ptr [ebx + 0x6f], sp
        jne    0x122b
        je     0x11df
        and    eax, 0x6e6f2073
        and    byte ptr [ebx + 0x65], dh
        jb     loc_4_123F
        jb     0x11ec
        and    eax, 0x20000a73
        and    byte ptr [eax], ah
        and    byte ptr [ebp + eax*2 + 0x43], al
loc_4_11D7:
        outsb  dx, byte ptr [esi]
        je     0x11fb
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        and    byte ptr gs:[esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        cmp    ah, byte ptr gs:[eax]
        and    eax, 0x20000a73
loc_4_11EB:
        and    byte ptr [eax], ah
        and    byte ptr [ebx + eax*2 + 0x50], dl
        das    
        dec    ecx
        push   eax
        and    byte ptr [esi + 0x6f], ch
        and    byte ptr gs:[esi + 0x61], ch
loc_4_11FC:
        insd   dword ptr es:[edi], dx
        cmp    ah, byte ptr gs:[eax]
        and    eax, 0x20000a73
        and    byte ptr [eax], ah
        and    byte ptr [esi + 0x65], cl
        je     0x124e
        dec    ecx
        dec    edi
        push   ebx
        and    byte ptr [esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        cmp    ah, byte ptr gs:[eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_4_121A:
        and    eax, 0x54000a73
        push   0x70207369
        jb     loc_4_1295
        jb     loc_4_128A
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x62], ch
        je     0x1290
        imul   ebp, dword ptr [esi + 0x73], 0x6c206120
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        outsw  dx, word ptr [esi]
loc_4_123F:
        jb     0x1261
        and    eax, 0xa0a73
        push   esp
        push   0x63207369
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebp + 0x6e], 0x6f642074
        jae    0x1277
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x127b
        arpl   word ptr [edi + 0x6e], bp
        je     0x12c1
        imul   ebp, dword ptr [esi + 0x20], 0x20796e61
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x7365736e
        or     al, byte ptr cs:[eax]
        push   esp
        push   0x63207369
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebp + 0x6e], 0x6f632074
        outsb  dx, byte ptr [esi]
        je     0x12e4
        imul   ebp, dword ptr [esi + 0x73], 0x6c206120
loc_4_128A:
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        outsw  dx, word ptr [esi]
        jb     0x12b5
loc_4_1295:
        and    eax, 0x20000a73
        and    byte ptr [eax], ah
        and    byte ptr [esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    0x130e
        and    byte ptr [ebx + 0x65], dh
        jb     loc_4_1324
        jb     0x12d1
        and    eax, 0xa2e73
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        outsd  dx, dword ptr [esi]
        bound  esi, qword ptr [ecx + 0x69]
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x72], ah
        outsd  dx, dword ptr [esi]
        jne    loc_4_1345
        and    byte ptr [0xa2e73], ah
        push   ebx
        popal  
        jb     0x1343
        push   0x20676e69
        outsw  dx, word ptr [esi]
        jb     0x1309
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        jae    0x1358
        jb     0x136b
        jb     0x136b
        add    byte ptr cs:[ebp + eax*2 + 0x42], al
        push   ebp
        inc    edi
        cmp    ah, byte ptr [eax]
        dec    esp
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x74], sp
        imul   ebp, dword ptr [esi + 0x67], 0x63696c20
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_1378
        and    byte ptr [ebx + 0x65], dh
        jb     loc_4_138E
        jb     loc_4_138E
        and    byte ptr [edi + 0x6e], ch
        and    byte ptr [esi + 0x65], ch
        je     0x139a
        outsd  dx, dword ptr [esi]
loc_4_1324:
        jb     loc_4_1391
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    dword ptr [ebx], esi
        cmp    ah, byte ptr [eax]
        dec    esi
        outsd  dx, dword ptr [esi]
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_13A2
        and    byte ptr [ebx + 0x65], dh
        jb     loc_4_13B8
        jb     loc_4_13B8
loc_4_1345:
        and    byte ptr [ecx + 0x72], ah
        and    byte ptr gs:[ecx + 0x76], ah
        popal  
        imul   ebp, dword ptr [ecx + 0x62], 0x7420656c
        outsd  dx, dword ptr [esi]
        and    byte ptr [ebx + 0x61], dh
        je     0x13c4
        jae    0x13c3
        jns    0x137f
        jb     0x13c6
        jno    0x13d8
        jae    0x13da
        add    byte ptr cs:[edx], cl
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        jae    0x13d8
        jb     loc_4_13EB
        jb     0x1398
loc_4_1378:
        and    eax, 0x73692073
        and    byte ptr [esi + 0x6f], ch
        je     loc_4_13A2
        popal  
        jbe    loc_4_13E6
        imul   ebp, dword ptr [ecx + 0x62], 0x7420656c
        outsd  dx, dword ptr [esi]
loc_4_138E:
        and    byte ptr [ebx + 0x61], dh
loc_4_1391:
        je     loc_4_13FC
        jae    0x13fb
        jns    0x13b7
        jb     loc_4_13FE
        jno    0x1410
        jae    loc_4_1412
        add    byte ptr cs:[edx + 0x65], dl
loc_4_13A2:
        jno    0x1419
        jae    0x141b
        imul   ebp, dword ptr [esi + 0x67], 0x63696c20
        outsb  dx, byte ptr gs:[esi]
        jae    0x1417
        and    byte ptr [esi], ch
        and    byte ptr cs:[eax], al
loc_4_13B8:
        or     ah, byte ptr [eax]
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    0x1427
        and    byte ptr [ebx + 0x75], dh
        arpl   word ptr [ebx + 0x65], sp
        jae    loc_4_143D
        jne    loc_4_1439
        insb   byte ptr es:[edi], dx
        jns    loc_4_13F0
        outsd  dx, dword ptr [esi]
        bound  esi, qword ptr [ecx + 0x69]
        outsb  dx, byte ptr [esi]
        add    byte ptr cs:[edx], cl
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    al, 0x30
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
loc_4_13E6:
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
loc_4_13EB:
        je     loc_4_145C
        and    byte ptr [edi + 0x62], ch
loc_4_13F0:
        je     0x1453
        imul   ebp, dword ptr [esi + 0x20], 0x696c2061
        arpl   word ptr [ebp + 0x6e], sp
loc_4_13FC:
        jae    loc_4_1463
loc_4_13FE:
        and    byte ptr [esi + 0x6f], ah
        jb     loc_4_1423
        and    eax, 0x63530073
        popal  
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [esi + 0x67], 0x74656e20
loc_4_1412:
        ja     0x1483
        jb     0x1481
        and    byte ptr [esi], ch
        and    byte ptr cs:[eax], al
        push   esi
        jb     0x1489
        jns    loc_4_148C
loc_4_1423:
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x69], ch
        arpl   word ptr [ebp + 0x6e], sp
        jae    0x1492
        and    byte ptr [esi], ch
        and    byte ptr cs:[eax], al
        or     ah, byte ptr [eax]
        and    byte ptr [ecx + ebp*2 + 0x63], ch
loc_4_1439:
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_14A2
loc_4_143D:
        and    byte ptr [esi + 0x6f], ah
        jb     0x1462
        and    eax, 0x75732073
        arpl   word ptr [ebx + 0x65], sp
        jae    loc_4_14BF
        jne    0x14bb
        insb   byte ptr es:[edi], dx
        jns    0x1472
        jbe    0x14b9
        jb     loc_4_14BF
        imul   sp, word ptr [ebp + 0x64], 0x6620
loc_4_145C:
        jb     0x14cd
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + ebp*2 + 0x63], ch
loc_4_1463:
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_14CC
        and    byte ptr [ebx + 0x65], dh
        jb     loc_4_14E2
        jb     loc_4_148F
        and    eax, 0xa002e73
        and    byte ptr [eax], ah
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        outsw  dx, word ptr [esi]
        jb     loc_4_14A2
        and    eax, 0x73692073
        and    byte ptr [esi + 0x6f], ch
        je     0x14ac
loc_4_148C:
        jbe    loc_4_14EF
        insb   byte ptr es:[edi], dx
loc_4_148F:
        imul   esp, dword ptr [esi + ebp], 0x6c20200a
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        outsw  dx, word ptr [esi]
        jb     0x14c2
loc_4_14A2:
        and    eax, 0x73692073
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     loc_4_151F
        jb     0x14da
        arpl   word ptr [ecx + ebp*2 + 0x65], bp
        outsb  dx, byte ptr [esi]
loc_4_14BF:
        je     loc_4_14EF
        add    byte ptr [edx], cl
        and    byte ptr [eax], ah
        jne    0x1535
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
loc_4_14CC:
        je     loc_4_153D
        and    byte ptr [esi + 0x65], dh
        jb     loc_4_153C
        jns    0x14f6
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        outsw  dx, word ptr [esi]
        jb     loc_4_1502
loc_4_14E2:
        and    eax, 0x6e6f2073
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    0x1554
loc_4_14EF:
        and    byte ptr [ebx + 0x65], dh
        jb     0x156a
        jb     0x1517
        and    eax, 0x52002e73
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_4_156B
loc_4_1502:
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x69], ch
        arpl   word ptr [ebp + 0x6e], sp
        jae    0x1571
        and    byte ptr [esi], ch
        and    byte ptr cs:[eax], al
        or     ah, byte ptr [eax]
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_1581
        and    byte ptr [esi + 0x6f], ah
loc_4_151F:
        jb     0x1541
        and    eax, 0x75732073
        arpl   word ptr [ebx + 0x65], sp
        jae    0x159e
        jne    loc_4_159A
        insb   byte ptr es:[edi], dx
        jns    loc_4_1551
        jb     0x1598
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x159d
        and    byte ptr fs:[esi + 0x72], ah
loc_4_153C:
        outsd  dx, dword ptr [esi]
loc_4_153D:
        insd   dword ptr es:[edi], dx
        and    byte ptr [0x65732073], ah
        jb     0x15bc
        jb     0x1569
        or     al, byte ptr cs:[eax]
        or     ah, byte ptr [eax]
        and    byte ptr [ebp + 0x6e], dh
loc_4_1551:
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x15c7
        and    byte ptr [edx + 0x65], dh
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_4_15C5
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    0x15cd
        and    byte ptr [esi + 0x6f], ah
loc_4_156B:
        jb     0x158d
        and    eax, 0x72662073
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    0x15e1
        and    byte ptr [ebx + 0x65], dh
        jb     0x15f7
loc_4_1581:
        jb     loc_4_15A4
        and    eax, 0x6f4e0073
        and    byte ptr [eax], ah
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x65], 0x7365736e
        and    byte ptr [ecx + 0x72], ah
        and    byte ptr gs:[ecx + 0x76], ah
loc_4_159A:
        popal  
        imul   ebp, dword ptr [ecx + 0x62], 0x7420656c
        outsd  dx, dword ptr [esi]
loc_4_15A4:
        and    byte ptr [edx + 0x65], dh
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x1611
        or     al, byte ptr cs:[eax]
        or     cl, byte ptr [ecx + ecx*2 + 0x43]
        xor    byte ptr [eax], dh
        xor    esi, dword ptr [edi]
        cmp    ah, byte ptr [eax]
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        jae    0x1628
        jb     0x163b
loc_4_15C5:
        jb     loc_4_15E8
        jb     loc_4_162F
        je     0x1641
        jb     0x163c
        and    byte ptr fs:[ecx + 0x6e], ch
        jbe    0x1636
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x6c], 0x6e656369
        jae    loc_4_1645
        or     al, byte ptr cs:[eax]
        and    byte ptr [eax], ah
        dec    esp
        dec    ecx
        inc    ebx
loc_4_15E8:
        xor    byte ptr [eax], dh
        xor    al, 0x33
        cmp    ah, byte ptr [eax]
        inc    ebx
        outsd  dx, dword ptr [esi]
        jne    loc_4_165E
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_1618
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x20], bp
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x74], sp
        and    byte ptr fs:[ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_4_1686
        cmp    ah, byte ptr [eax]
        push   esi
        imul   esi, dword ptr [edx + 0x74], 0x4c6c6175
        outsd  dx, dword ptr [esi]
loc_4_1618:
        arpl   word ptr [ebx + 0x20], bp
        jae    0x1691
        popal  
        je     loc_4_1695
        jae    0x1642
        and    eax, 0x2000436c
        and    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    al, 0x34
loc_4_162F:
        cmp    ah, byte ptr [eax]
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jae    loc_4_16AD
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     loc_4_1663
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
loc_4_1645:
        je     0x1667
        jae    0x16be
        arpl   word ptr [ebx + 0x65], sp
        jae    loc_4_16C1
        jne    0x16bd
        and    byte ptr [edi + ebp*2 + 0x61], ch
        imul   ebp, dword ptr fs:[esi + 0x67], 0x4c202000
        dec    ecx
loc_4_165E:
        inc    ebx
        xor    byte ptr [eax], dh
        xor    al, 0x35
loc_4_1663:
        cmp    ah, byte ptr [eax]
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jae    0x16e1
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     loc_4_1697
        inc    ebx
        jb     0x16df
        popal  
        je     loc_4_16E2
        and    byte ptr [esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        and    byte ptr fs:[eax + 0x69], dh
loc_4_1686:
        jo     0x16ed
        and    byte ptr [ebp + 0x72], al
        jb     0x16fc
        jb     loc_4_16C9
        and    byte ptr [0x202e64], ah
loc_4_1695:
        and    byte ptr [eax], ah
loc_4_1697:
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    al, 0x36
        cmp    ah, byte ptr [eax]
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jae    loc_4_171C
        outsd  dx, dword ptr [esi]
loc_4_16AD:
        outsb  dx, byte ptr [esi]
        jb     0x16d2
        push   edx
        popal  
        inc    esi
        imul   ebp, dword ptr [ebp + 0x45], 0x72652078
        jb     0x1730
loc_4_16C1:
        jb     0x16fd
        and    byte ptr [0x77202c64], ah
loc_4_16C9:
        push   0x20656c69
        jb     0x1735
        popal  
        imul   ebp, dword ptr fs:[esi + 0x67], 0x6d616e20
        and    byte ptr fs:[eax + 0x69], dh
        jo     loc_4_1745
        and    byte ptr [eax], al
loc_4_16E2:
        and    byte ptr [eax], ah
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    eax, 0x694c203a
        arpl   word ptr [ebp + 0x6e], sp
        jae    0x1759
        and    byte ptr [edx + 0x65], dl
        jae    0x1769
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     loc_4_171F
        dec    ecx
        dec    edi
        and    byte ptr [eax + 0x45], dl
        dec    esi
        inc    esp
        dec    ecx
        dec    esi
        inc    edi
        and    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        and    byte ptr [edx + 0x65], dh
        popal  
        imul   ebp, dword ptr fs:[esi + 0x67], 0x6f726620
        insd   dword ptr es:[edi], dx
loc_4_171C:
        and    byte ptr [esi + 0x61], ch
loc_4_171F:
        insd   dword ptr es:[edi], dx
        and    byte ptr fs:[eax + 0x69], dh
        jo     0x178c
        add    byte ptr [eax], ah
        and    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    al, 0x37
        cmp    ah, byte ptr [eax]
        and    eax, 0x72572073
        imul   esi, dword ptr [ebp + 0x46], 0x45656c69
        js     0x1762
        jb     0x17b7
loc_4_1745:
        outsd  dx, dword ptr [esi]
        jb     loc_4_1782
        and    byte ptr [0x77202c64], ah
        push   0x20656c69
        ja     0x17c7
        imul   esi, dword ptr [ecx + ebp*2 + 0x6e], 0x616e2067
        insd   dword ptr es:[edi], dx
        and    byte ptr fs:[eax + 0x69], dh
        jo     loc_4_17CA
        and    byte ptr [eax], al
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jae    loc_4_17E3
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     loc_4_1779
loc_4_1779:
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
loc_4_1782:
        jno    0x17fa
        jae    0x17fc
        jb     loc_4_178B
loc_4_178B:
        and    byte ptr [eax], ah
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    al, 0x38
        cmp    ah, byte ptr [eax]
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jno    0x1817
        jae    0x1819
        jb     0x17c8
        arpl   word ptr [edi + 0x75], bp
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x17d2
        jb     0x1819
        je     loc_4_1828
        imul   esp, dword ptr [ebp + 0x76], 0x45442065
        inc    ebx
        dec    esi
        inc    ebp
        push   esp
        and    byte ptr [esi + 0x6f], ch
        and    byte ptr gs:[esi + 0x61], ch
        insd   dword ptr es:[edi], dx
loc_4_17CA:
        add    byte ptr gs:[eax], ah
        and    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    al, 0x39
        cmp    ah, byte ptr [eax]
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jno    loc_4_1858
loc_4_17E3:
        jae    0x185a
        jb     loc_4_1809
        arpl   word ptr [edi + 0x75], bp
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x1813
        jb     0x185a
        je     0x1869
        imul   esp, dword ptr [ebp + 0x76], 0x43542065
        push   eax
        das    
        dec    ecx
        push   eax
        and    byte ptr [esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], ah
loc_4_1809:
        and    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    eax, 0x4c203a30
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jno    0x1894
        jae    0x1896
        jb     0x1845
        arpl   word ptr [edi + 0x75], bp
loc_4_1828:
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x184f
        jb     0x1896
        je     0x18a5
        imul   esp, dword ptr [ebp + 0x76], 0x454e2065
        push   esp
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
        and    byte ptr [esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], ah
        and    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    eax, 0x4c203a31
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
loc_4_1858:
        push   edx
        jae    loc_4_18CC
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     0x1882
        jb     0x18d7
        outsd  dx, dword ptr [esi]
        jb     0x1888
        ja     0x18d2
        imul   ebp, dword ptr [ebp + 0x20], 0x64616572
        imul   ebp, dword ptr [esi + 0x67], 0x67657220
        imul   esi, dword ptr [ebx + 0x74], 0x20007972
        and    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    eax, 0x25203a32
        jae    0x18ad
        push   edx
        popal  
        inc    esi
        imul   ebp, dword ptr [ebp + 0x20], 0x6f727265
        jb     0x18d6
        and    byte ptr [0x77202c64], ah
        push   0x20656c69
        jb     loc_4_190E
        popal  
        imul   ebp, dword ptr fs:[esi + 0x67], 0x6f726620
        insd   dword ptr es:[edi], dx
        and    byte ptr [esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        and    byte ptr fs:[eax + 0x69], dh
        jo     loc_4_1923
        add    byte ptr [eax], ah
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    eax, 0x45203a33
        jb     0x193e
loc_4_18CC:
        outsd  dx, dword ptr [esi]
        jb     0x18ef
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x75], sp
        jb     loc_4_193A
        and    byte ptr fs:[ecx + 0x74], ah
        je     loc_4_1940
        insd   dword ptr es:[edi], dx
        jo     0x1952
        imul   ebp, dword ptr [esi + 0x67], 0x206f7420
        arpl   word ptr [edi + ebp*2 + 0x73], bp
        and    byte ptr gs:[eax + 0x72], dl
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebp + 0x73], sp
        jae    loc_4_1913
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jae    loc_4_1967
        insb   byte ptr es:[edi], dx
        add    byte ptr gs:[eax], ah
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        xor    eax, 0x45203a34
        jb     0x197a
        outsd  dx, dword ptr [esi]
        jb     0x192b
        arpl   word ptr [edi + 0x75], bp
loc_4_190E:
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
loc_4_1913:
        je     0x1935
        imul   bp, word ptr [esi + 0x64], 0x4c20
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
loc_4_1923:
        jae    0x1996
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     loc_4_194C
        jo     loc_4_19A0
        outsd  dx, dword ptr [esi]
        jb     0x1993
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jae    0x19a9
        popal  
loc_4_193A:
        ja     loc_4_19AA
        add    byte ptr [ecx + ecx*2 + 0x43], cl
loc_4_1940:
        xor    byte ptr [eax], dh
        xor    eax, 0x45203a35
        jb     loc_4_19BB
        outsd  dx, dword ptr [esi]
        jb     loc_4_196C
loc_4_194C:
        arpl   word ptr [edi + 0x75], bp
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x1976
        arpl   word ptr [edx + 0x65], si
        popal  
        je     0x19c1
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_19C9
        and    byte ptr [edx + 0x65], dl
loc_4_1967:
        jno    0x19de
        jae    0x19e0
loc_4_196C:
        jb     loc_4_198F
        push   eax
        jb     0x19e1
        arpl   word ptr [ebp + 0x73], sp
        jae    0x1997
        and    eax, 0x494c0073
        inc    ebx
        xor    byte ptr [eax], dh
        xor    eax, 0x45203a36
        jb     loc_4_19F8
        outsd  dx, dword ptr [esi]
        jb     0x19a9
        push   eax
        inc    ecx
        push   esp
        dec    eax
        push   edi
        dec    edi
loc_4_198F:
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [0x61662073], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
loc_4_19A0:
        jae    0x1a07
        je     loc_4_19C4
        je     loc_4_1A0E
        and    byte ptr gs:[esi + 0x61], dh
loc_4_19AA:
        insb   byte ptr es:[edi], dx
        jne    0x1a12
        and    byte ptr [0x666f2073], ah
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [eax + 0x57], dl
        dec    esp
loc_4_19BB:
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        jae    0x1a39
loc_4_19C4:
        bound  ebp, qword ptr [ebx + 0x65]
        jns    loc_4_19F5
loc_4_19C9:
        and    byte ptr [edx + 0x65], dl
        push   ebx
        je     loc_4_1A27
        popal  
        insb   byte ptr es:[edi], dx
        jne    0x1a3a
        inc    ebp
        js     loc_4_19F8
        popaw  
        imul   ebp, dword ptr [ebp + 0x64], 0x3125203a
        inc    ebx
        add    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    eax, 0x45203a37
        jb     0x1a62
        outsd  dx, dword ptr [esi]
        jb     0x1a13
        push   eax
        inc    ecx
loc_4_19F5:
        push   esp
        dec    eax
        push   edi
loc_4_19F8:
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [0x61662073], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        dec    edi
        jo     0x1a72
        outsb  dx, byte ptr [esi]
loc_4_1A0E:
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ebx + 0x72], al
        popal  
        je     0x1a7d
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [0x654b2073], ah
        jns    loc_4_1A50
        and    byte ptr [edx + 0x65], dl
loc_4_1A27:
        inc    ebx
        jb     loc_4_1A90
        popal  
        je     0x1a93
        dec    ebx
        jns    loc_4_1A77
        pop    eax
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x3125203a
        inc    ebx
        add    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    eax, 0x45203a38
        jb     loc_4_1ABE
        outsd  dx, dword ptr [esi]
        jb     loc_4_1A6F
        push   eax
loc_4_1A50:
        inc    ecx
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [0x61662073], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        jno    0x1add
        jb     0x1ae4
        and    byte ptr [esi + 0x61], dh
        insb   byte ptr es:[edi], dx
loc_4_1A6F:
        jne    0x1ad6
        and    byte ptr [0x666f2073], ah
loc_4_1A77:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [eax + 0x57], dl
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        jae    loc_4_1AFD
        bound  ebp, qword ptr [ebx + 0x65]
        jns    0x1ab9
        and    byte ptr [edx + 0x65], dl
loc_4_1A90:
        push   ebx
        je     0x1aeb
        popal  
        insb   byte ptr es:[edi], dx
        jne    0x1afe
        inc    ebp
        js     loc_4_1ABC
        popaw  
        imul   ebp, dword ptr [ebp + 0x64], 0x3125203a
        inc    ebx
        add    byte ptr [ecx + ecx*2 + 0x43], cl
        xor    byte ptr [eax], dh
        xor    eax, 0x57203a39
        popal  
        jb     0x1b23
        imul   ebp, dword ptr [esi + 0x67], 0x54415020
loc_4_1ABC:
        dec    eax
        push   edi
loc_4_1ABE:
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_1B2F
        and    byte ptr [edx + 0x65], dl
        jae    0x1b3f
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     0x1af5
        imul   esp, dword ptr fs:[eax + 0x6e], 0x7320746f
        jne    0x1b43
        arpl   word ptr [ebp + 0x73], sp
        jae    0x1b4b
        jne    loc_4_1B53
        insb   byte ptr es:[edi], dx
        jns    0x1b0a
        jb     loc_4_1B51
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[ecx + ebp*2 + 0x63], ch
        outsb  dx, byte ptr gs:[esi]
        jae    0x1b5d
        jae    loc_4_1AFA
loc_4_1AFA:
        dec    esp
        dec    ecx
        inc    ebx
loc_4_1AFD:
        xor    byte ptr [eax], dh
        xor    byte ptr ss:[edx], bh
        and    byte ptr [edi + 0x61], dl
        jb     loc_4_1B75
        imul   ebp, dword ptr [esi + 0x67], 0x54415020
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_1B81
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x6d], ch
        jo     loc_4_1B96
        outsd  dx, dword ptr [esi]
        jo     0x1b8c
        jb     0x1b92
        insb   byte ptr es:[edi], dx
        jns    loc_4_1B4C
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_4_1B2F:
        imul   sp, word ptr [edi + 0x75], 0x6572
        sub    al, 0x20
        dec    esp
        imul   esp, dword ptr [ebx + 0x65], 0x2065736e
        push   edx
        jno    0x1bb9
        jae    loc_4_1BBB
        jb     loc_4_1B6A
        ja     0x1bb5
loc_4_1B4C:
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x74], ah
loc_4_1B51:
        je     loc_4_1BB8
loc_4_1B53:
        insd   dword ptr es:[edi], dx
        jo     0x1bca
        and    byte ptr [edi + ebp*2 + 0x20], dh
        outsd  dx, dword ptr [esi]
        bound  esi, qword ptr [ecx + 0x69]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x20], ah
        push   eax
        inc    ecx
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
loc_4_1B6A:
        dec    ebx
        push   ebx
        and    byte ptr [0x25203a73], ah
        jae    loc_4_1B74
loc_4_1B74:
        dec    esp
loc_4_1B75:
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        cmp    ah, byte ptr ss:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
loc_4_1B81:
        push   edx
        and    byte ptr [ebx + 0x6f], al
        jne    0x1bf3
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x1bad
        arpl   word ptr [edi + ebp*2 + 0x73], bp
        and    byte ptr gs:[esi + 0x61], ch
        insd   dword ptr es:[edi], dx
loc_4_1B96:
        and    byte ptr fs:[eax + 0x69], dh
        jo     0x1c02
        and    byte ptr [ebp + 0x72], ah
        jb     loc_4_1C11
        jb     loc_4_1BC4
        jb     loc_4_1C0B
        je     0x1c1d
        jb     0x1c18
        cmp    ah, byte ptr fs:[eax]
        and    eax, 0x494c0064
        inc    ebx
        xor    byte ptr [eax], dh
        aaa    
loc_4_1BB8:
        cmp    ah, byte ptr [eax]
        inc    ebp
loc_4_1BBB:
        push   edx
        push   edx
        dec    edi
        push   edx
        and    byte ptr [eax + 0x41], dl
        push   esp
        dec    eax
loc_4_1BC4:
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_1C36
        and    byte ptr [edx + 0x65], dl
        jae    0x1c46
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     0x1bfc
        arpl   word ptr [edi + 0x75], bp
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_1C06
        jae    0x1c4d
        je     loc_4_1C0A
        jae    0x1c51
        arpl   word ptr [ebp + 0x72], si
        imul   esi, dword ptr [ecx + edi*2 + 0x20], 0x72747461
        imul   esp, dword ptr [edx + 0x75], 0x736574
        dec    esp
        dec    ecx
        inc    ebx
        xor    byte ptr [eax], dh
        cmp    byte ptr ss:[edx], bh
loc_4_1C06:
        and    byte ptr [ebp + 0x52], al
        push   edx
loc_4_1C0A:
        dec    edi
loc_4_1C0B:
        push   edx
        and    byte ptr [eax + 0x41], dl
        push   esp
        dec    eax
loc_4_1C11:
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    0x1c83
        and    byte ptr [edx + 0x65], dl
        jno    0x1c98
        jae    0x1c9a
        jb     0x1c49
        arpl   word ptr [edi + 0x75], bp
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_1C53
        arpl   word ptr [edx + 0x65], si
loc_4_1C36:
        popal  
        je     0x1c9e
        and    byte ptr [esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        and    byte ptr fs:[eax + 0x69], dh
        jo     0x1ca9
        and    byte ptr fs:[ebp + 0x72], ah
        jb     loc_4_1CB9
        jb     0x1c86
        and    byte ptr [0x494c0064], ah
        inc    ebx
loc_4_1C53:
        xor    byte ptr [eax], dh
        cmp    dword ptr ss:[edx], edi
        and    byte ptr [edi + 0x41], dl
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        and    byte ptr [eax + 0x41], dl
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    0x1cd7
        and    byte ptr [edx + 0x65], dl
        jae    0x1ce7
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jb     loc_4_1C9D
        arpl   word ptr [edi + 0x75], bp
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x1ca7
        jo     0x1cfb
        outsd  dx, dword ptr [esi]
        jo     loc_4_1CF1
        jb     loc_4_1CF7
        insb   byte ptr es:[edi], dx
        jns    0x1cb1
        jae    0x1cfb
        jne    0x1d09
        outsd  dx, dword ptr fs:[esi]
        ja     0x1d07
        and    byte ptr [eax + ebp*2 + 0x65], dh
loc_4_1C9D:
        and    byte ptr [esi + 0x61], ch
        insd   dword ptr es:[edi], dx
        and    byte ptr fs:[eax + 0x69], dh
        jo     0x1d0d
        and    byte ptr [ebp + 0x72], ah
        jb     loc_4_1D1C
        jb     0x1ce9
        and    byte ptr [0x494c0064], ah
        inc    ebx
        xor    byte ptr [eax], dh
        aaa    
loc_4_1CB9:
        xor    byte ptr [edx], bh
        and    byte ptr [edi + 0x41], dl
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        and    byte ptr [eax + 0x41], dl
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    loc_4_1D3A
        and    byte ptr [edx + 0x65], dl
        jno    loc_4_1D4F
        jae    loc_4_1D51
        jb     loc_4_1D00
        arpl   word ptr [edi + 0x75], bp
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_4_1D0A
        outsd  dx, dword ptr [esi]
        jo     loc_4_1D52
        outsb  dx, byte ptr [esi]
        and    byte ptr [esi + 0x61], ch
loc_4_1CF1:
        insd   dword ptr es:[edi], dx
        and    byte ptr fs:[eax + 0x69], dh
loc_4_1CF7:
        jo     0x1d5e
        and    byte ptr [edi + ebp*2 + 0x20], dh
        arpl   word ptr [edi + 0x6d], bp
loc_4_1D00:
        insd   dword ptr es:[edi], dx
        jne    loc_4_1D71
        imul   esp, dword ptr [ebx + 0x61], 0x77206574
loc_4_1D0A:
        imul   esi, dword ptr [eax + ebp*2 + 0x20], 0x6e63694c
        jae    loc_4_1D7A
        and    byte ptr [edx + 0x65], dl
        jae    0x1d8a
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_4_1D1C:
        jb     0x1d40
        jb     loc_4_1D95
        outsd  dx, dword ptr [esi]
        jb     0x1d60
        and    byte ptr [0x6f430064], ah
        insd   dword ptr es:[edi], dx
        jo     0x1d9b
        je     0x1d97
        and    byte ptr [ecx + ebp*2 + 0x63], cl
        outsb  dx, byte ptr gs:[esi]
        jae    0x1d9f
loc_4_1D3A:
        add    byte ptr [esi + 0x69], al
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[ecx + 0x6e], ah
        and    byte ptr fs:[eax + 0x72], dl
        imul   ebp, dword ptr [esi + 0x74], 0x696c4300
        outsb  dx, byte ptr gs:[esi]
loc_4_1D4F:
        je     loc_4_1D71
loc_4_1D51:
        dec    esi
loc_4_1D52:
        je     loc_4_1DCC
        outsd  dx, dword ptr [esi]
        jb     loc_4_1DC3
        and    byte ptr [ebx + 0x65], dl
        jb     0x1dd3
        imul   esp, dword ptr [ebx + 0x65], 0x54415000
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ecx + ecx*2 + 0x43], cl
        inc    ebp
        dec    esi
        push   ebx
loc_4_1D71:
        dec    ecx
        dec    esi
        inc    edi
        or     eax, 0x4c28000a
        inc    ecx
loc_4_1D7A:
        dec    esi
        dec    ebp
        inc    ecx
        dec    esi
        sub    dword ptr [eax], esp
        inc    ebx
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebp + 0x6e], 0x694c2074
        arpl   word ptr [ebp + 0x6e], sp
        jae    0x1df3
        and    byte ptr [edx + 0x65], dl
        jae    0x1e03
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_4_1D95:
        jb     0x1db9
        sub    byte ptr [esi + 0x58], dl
        inc    esp
        sub    dword ptr [0x6f43000a], ecx
        jo     0x1e1e
        jb     loc_4_1E10
        push   0x43282074
        sub    dword ptr [eax], esp
        xor    dword ptr [ecx], edi
        cmp    dword ptr [ebx], esi
        sub    al, 0x20
        xor    dword ptr [ecx], edi
        cmp    dword ptr [esp + ebp], esi
        and    byte ptr [ecx], dh
        cmp    dword ptr [ecx], edi
        xor    eax, 0x20796220
loc_4_1DC3:
        inc    esp
        imul   esp, dword ptr [edi + 0x69], 0x206c6174
        inc    ebp
loc_4_1DCC:
        jno    0x1e43
        imul   esi, dword ptr [eax + 0x6d], 0x20746e65
        inc    ebx
        outsd  dx, dword ptr [esi]
        jb     loc_4_1E49
        outsd  dx, dword ptr [esi]
        jb     0x1e3d
        je     loc_4_1E47
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr cs:[eax], ah
        inc    ecx
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + 0x69], dl
        push   0x52207374
        jae    0x1e57
        jb     loc_4_1E6A
        or     eax, 0x654e000a
        je     loc_4_1E75
        outsd  dx, dword ptr [esi]
        jb     0x1e6c
        and    byte ptr [ebx + 0x6f], dh
        je     0x1e7e
        popal  
        jb     loc_4_1E6F
        and    byte ptr [esi + 0x6f], ch
        je     loc_4_1E2F
        insb   byte ptr es:[edi], dx
loc_4_1E10:
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[edi + 0x72], ch
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        bound  ebp, qword ptr [ebp + 0x6d]
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        imul   sp, word ptr [edi + 0x75], 0x6972
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + 0x65], ch
loc_4_1E2F:
        je     0x1ea8
        outsd  dx, dword ptr [esi]
        jb     loc_4_1E9F
        or     eax, 0x6f4e000a
        je     0x1e5c
        outsb  dx, byte ptr gs:[esi]
        outsd  dx, dword ptr [esi]
        jne    0x1ea8
        push   0x6d656d20
        outsd  dx, dword ptr [esi]
loc_4_1E47:
        jb     0x1ec2
loc_4_1E49:
        and    byte ptr [ecx + 0x76], ah
        popal  
        imul   ebp, dword ptr [ecx + 0x62], 0x7420656c
        outsd  dx, dword ptr [esi]
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jb     0x1ec2
        insd   dword ptr es:[edi], dx
        or     eax, 0xa
        add    byte ptr [ecx], al
loc_4_1E6A:
        add    dword ptr [edi - 0x5a], ebx
        add    byte ptr [eax], al
loc_4_1E6F:
        add    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
loc_4_1E75:
        aaa    
        cmp    byte ptr [ecx], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1E9F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh
        cmp    byte ptr [edi], dh
        xor    eax, 0x31323334
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl            ; -> sub_2_0810
        or     byte ptr [edx], al
        add    byte ptr [eax], ah            ; -> sub_2_0820
        or     byte ptr [edx], al
        add    byte ptr [eax], dl            ; -> sub_2_0810
        or     byte ptr [edx], al
        add    byte ptr [eax], dh            ; -> sub_2_0830
        or     byte ptr [edx], al
        add    byte ptr [eax + 8], al        ; -> sub_2_0840
        add    al, byte ptr [eax]
        adc    byte ptr [eax], cl
        add    al, byte ptr [eax]
        pop    edx
        add    dword ptr [eax], eax
        add    byte ptr [edx + 1], bl
        add    byte ptr [eax], al
        pop    edx
        add    dword ptr [eax], eax
        add    byte ptr [edx + 1], bl
        add    byte ptr [eax], al
        pop    edx
        add    dword ptr [eax], eax
        add    byte ptr [edx + 1], bl
        add    byte ptr [eax], al
loc_4_1F18:
        add    byte ptr [0xa5f01], cl
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_4_1F24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1F2C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1F34:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1F38:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1F40:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_1FF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [edi + 0x6c], ebx   ; -> obj4:0x006C
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_4_2004:
        xor    byte ptr [0xd500002], cl      ; -> loc_2_0D50
        add    al, byte ptr [eax]
loc_4_200C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_201C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_2032:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_2054:
        mov    word ptr [eax], es
        add    al, 0
        add    al, 0
        add    byte ptr [eax], al
        or     al, 0x20
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        add    byte ptr [eax + eax + 0x40004], bl ; -> obj4:0x009C
        add    byte ptr [eax], al
        sbb    al, 0x20
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    eax, 0xac000000               ; -> obj4:0x00AC
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    byte ptr [esi], ah
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        mov    ah, 0
        add    al, 0
        add    al, 0
        add    byte ptr [eax], al
        xor    ah, byte ptr [eax]
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_20B8:
        rol    byte ptr [eax], 4
        add    byte ptr [eax + 4], dl        ; -> loc_4_2054
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        xor    dword ptr [edx], esi
        xor    esi, dword ptr [esi + 0x39383736]
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_20DC:
        adc    byte ptr [edx], al
        add    al, 0
loc_4_20E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_20E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
        dd  obj4:0x023C              ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj4:0x0240              ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj4:0x0248              ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj4:0x024C              ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj4:0x0250              ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj4:0x0258              ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj4:0x0260              ; fixup
        db  000h, 000h, 000h, 000h
        dd  obj4:0x0264              ; fixup
        db  000h, 000h, 000h, 000h
        dd  obj4:0x026C              ; fixup
        db  000h, 000h, 000h, 000h
        dd  obj4:0x0270              ; fixup
        db  000h, 000h, 000h, 000h
        dd  obj4:0x0274              ; fixup
        db  000h, 000h, 000h, 000h
        dd  obj4:0x027C              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        dd  obj4:0x0288              ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj4:0x0290              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj4:0x0298              ; fixup
        db  003h, 000h, 000h, 000h
        dd  obj4:0x02A0              ; fixup
        db  004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        dd  obj4:0x02A8              ; fixup
        dd  obj4:0x02B4              ; fixup
        dd  obj4:0x02D0              ; fixup
        dd  obj4:0x02F0              ; fixup
        dd  obj4:0x0310              ; fixup
        dd  obj4:0x0330              ; fixup
        dd  obj4:0x035C              ; fixup
        dd  obj4:0x0384              ; fixup
        dd  obj4:0x039C              ; fixup
        dd  obj4:0x03C0              ; fixup
        dd  obj4:0x03E0              ; fixup
        dd  obj4:0x03F8              ; fixup
        dd  obj4:0x0410              ; fixup
        dd  obj4:0x0430              ; fixup
        dd  obj4:0x0458              ; fixup
        db  000h, 000h, 000h, 000h
sub_4_21BC:
        jo     loc_4_21DB
        add    al, byte ptr [eax]
        jo     loc_4_21DF
        add    al, byte ptr [eax]
        inc    eax
        push   ds
        add    al, byte ptr [eax]
        mov    al, 0x1e
        add    al, byte ptr [eax]
        push   eax
        pop    ds
        add    al, byte ptr [eax]
        push   eax
        pop    ds
        add    al, byte ptr [eax]
        dd  loc_2_1FF0               ; fixup
        dd  loc_2_1D70               ; fixup
loc_4_21DB:
        add    byte ptr [eax + 0x1d], dh     ; -> loc_2_1D70
        db  002h
loc_4_21DF:
        add    byte ptr [eax + 0x1f], dl     ; -> loc_2_1F50
        add    al, byte ptr [eax]
        push   eax
        pop    ds
        add    al, byte ptr [eax]
        mov    al, byte ptr [0x50000220]     ; -> loc_2_2050
        and    byte ptr [edx], al
        add    byte ptr [eax + 0x20], al     ; -> loc_2_2040
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_21F8:
        jl     loc_4_21FE
        add    al, 0
loc_4_21FC:
        add    byte ptr [eax], al
loc_4_21FE:
        add    byte ptr [eax], al
loc_4_2200:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_2204:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_2250:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_2254:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_2258:
        pop    eax
        and    al, byte ptr [eax + eax]
loc_4_225C:
        pop    eax
        and    al, byte ptr [eax + eax]
loc_4_2260:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_2298:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_229C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_22A0:
        mov    al, byte ptr [0xa0000422]     ; -> loc_4_22A0
        and    al, byte ptr [eax + eax]
loc_4_22A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_4_22AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_22B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_22B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 015h, 056h, 069h, 072h, 074h, 075h, 061h, 06Ch, 020h, 044h, 065h, 076h
        db  069h, 063h, 065h, 020h, 044h, 072h, 069h, 076h, 065h, 072h, 000h, 000h, 00Bh, 044h, 045h, 043h
        db  04Ch, 049h, 043h, 04Ch, 05Fh, 044h, 044h, 042h, 001h, 000h, 000h
VxD__LDATA ENDS

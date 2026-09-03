; oak_pcode_04.asm -- OAK object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        push   edx
        push   edi
        mov    edx, 3deh
        in     al, dx
        ror    eax, 8
        mov    edi, ebx
        db  003h, 03Dh, 0D4h, 000h, 000h, 000h
        xor    ecx, ecx
loc_4_0015:
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [ecx + edi], al
        dec    edx
        inc    cl
        cmp    cl, 37h
        jb     SHORT loc_4_0015
        rol    eax, 8
        out    dx, al
        mov    byte ptr [edi + 37h], al
        db  080h, 03Dh, 01Dh, 001h, 000h, 000h, 087h
        je     SHORT loc_4_0048
        mov    dl, 0c4h
        in     al, dx
        ror    eax, 8
        mov    al, 4
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 38h], al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_0048:
        pop    edi
        pop    edx
        ret
loc_4_004B:
        db  080h, 03Dh, 01Fh, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_00C5
        push   edx
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        db  03Bh, 00Dh, 0D8h, 000h, 000h, 000h
        jne    SHORT loc_4_006A
        mov    ah, 6
        jmp    SHORT loc_4_008A
loc_4_006A:
        db  03Bh, 00Dh, 0DCh, 000h, 000h, 000h
        jne    SHORT loc_4_007D
        db  0B9h, 0E0h, 000h, 000h, 000h
        db  02Bh, 00Dh, 0D4h, 000h, 000h, 000h
loc_4_007D:
        db  003h, 00Dh, 0D4h, 000h, 000h, 000h
        mov    ah, byte ptr [ecx + 38h]
        or     ah, ah
        je     SHORT loc_4_00C0
loc_4_008A:
        db  03Bh, 035h, 0D8h, 000h, 000h, 000h
        jne    SHORT loc_4_0096
        mov    al, 6
        jmp    SHORT loc_4_00B6
loc_4_0096:
        db  03Bh, 035h, 0DCh, 000h, 000h, 000h
        jne    SHORT loc_4_00A9
        db  0BEh, 0E0h, 000h, 000h, 000h
        db  02Bh, 035h, 0D4h, 000h, 000h, 000h
loc_4_00A9:
        db  003h, 035h, 0D4h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 38h]
        or     al, al
        je     SHORT loc_4_00C0
loc_4_00B6:
        and    ax, 8f7h
        or     ah, al
        mov    al, 4
        out    dx, ax
loc_4_00C0:
        rol    eax, 8
        out    dx, al
        pop    edx
loc_4_00C5:
        ret
loc_4_00C6:
        push   edx
        mov    edx, 3f1h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        pop    edx
        ret
loc_4_00D4:
        push   edx
        mov    edx, 3f1h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0C6h, 005h, 01Fh, 001h, 000h, 000h, 0FFh
        pop    edx
        ret
loc_4_00E9:
        push   edx
        mov    edx, 3f1h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0C6h, 005h, 01Fh, 001h, 000h, 000h, 000h
        pop    edx
        ret
loc_4_00FE:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 0D0h, 000h, 000h, 000h
        jne    SHORT loc_4_0112
        cmp    ebx, edi
        jne    SHORT loc_4_0126
loc_4_0112:
        cmp    ecx, 4
        je     SHORT loc_4_0123
        cmp    ecx, 0ch
        je     SHORT loc_4_011F
        in     al, dx
        jmp    SHORT loc_4_0195
loc_4_011F:
        out    dx, ax
        jmp    SHORT loc_4_0195
loc_4_0123:
        out    dx, al
        jmp    SHORT loc_4_0195
loc_4_0126:
        mov    edi, ebx
        db  003h, 03Dh, 0D4h, 000h, 000h, 000h
        cmp    ecx, 4
        je     SHORT loc_4_017D
        cmp    ecx, 0ch
        je     SHORT loc_4_016E
        cmp    edx, 3deh
        jne    SHORT loc_4_0145
        mov    al, byte ptr [edi + 37h]
        jmp    SHORT loc_4_0195
loc_4_0145:
        movzx  eax, byte ptr [edi + 37h]
        and    al, 3fh
        or     al, al
        je     SHORT loc_4_0163
        cmp    al, 7
        je     SHORT loc_4_0163
        cmp    al, 8
        je     SHORT loc_4_0163
        cmp    al, 0fh
        je     SHORT loc_4_0163
        cmp    al, 10h
        je     SHORT loc_4_0163
        cmp    al, 12h
        jne    SHORT loc_4_0169
loc_4_0163:
        dec    edx
        out    dx, al
        inc    edx
        in     al, dx
        jmp    SHORT loc_4_0195
loc_4_0169:
        mov    al, byte ptr [eax + edi]
        jmp    SHORT loc_4_0195
loc_4_016E:
        mov    byte ptr [edi + 37h], al
        and    al, 3fh
        mov    cl, ah
        movzx  eax, al
        mov    byte ptr [eax + edi], cl
        jmp    SHORT loc_4_0195
loc_4_017D:
        cmp    edx, 3deh
        jne    SHORT loc_4_018A
        mov    byte ptr [edi + 37h], al
        jmp    SHORT loc_4_0195
loc_4_018A:
        mov    cl, al
        movzx  eax, byte ptr [edi + 37h]
        and    al, 3fh
        mov    byte ptr [eax + edi], cl
loc_4_0195:
        pop    esi
        pop    edi
        ret
loc_4_0198:
        pushf
        cmp    cl, 4
        jne    SHORT loc_4_01B9
        mov    edi, ebx
        db  003h, 03Dh, 0D4h, 000h, 000h, 000h
        mov    byte ptr [edi + 38h], al
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        je     SHORT loc_4_01B9
        popf
        stc
        jmp    SHORT loc_4_01BB
loc_4_01B9:
        popf
loc_4_01BB:
        ret
loc_4_01BC:
        pushf
        cmp    cl, 4
        jne    SHORT loc_4_01DD
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        je     SHORT loc_4_01DD
        mov    edi, ebx
        db  003h, 03Dh, 0D4h, 000h, 000h, 000h
        mov    al, byte ptr [edi + 38h]
        popf
        stc
        jmp    SHORT loc_4_01DF
loc_4_01DD:
        popf
loc_4_01DF:
        ret
loc_4_01E0:
        mov    edx, 3deh
        in     al, dx
        ror    eax, 8
        db  080h, 03Dh, 01Dh, 001h, 000h, 000h, 087h
        je     SHORT loc_4_01FC
        mov    al, 11h
        out    dx, al
        inc    dl
        in     al, dx
        xor    ah, ah
        jmp    SHORT loc_4_020B
loc_4_01FC:
        mov    al, 23h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    dl
        mov    al, 25h
        out    dx, al
        inc    dl
        in     al, dx
loc_4_020B:
        dec    dl
        rol    eax, 8
        out    dx, al
        ror    eax, 8
        movzx  edx, ax
        stc
        ret
loc_4_0219:
        push   eax
        push   edx
        db  080h, 03Dh, 01Dh, 001h, 000h, 000h, 087h
        je     SHORT loc_4_0247
        test   dl, 0f0h
        jne    SHORT loc_4_0230
        mov    dh, dl
        shl    dh, 4
        or     dl, dh
loc_4_0230:
        mov    ah, dl
        mov    edx, 3deh
        in     al, dx
        ror    eax, 8
        mov    ah, al
        mov    al, 11h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    SHORT loc_4_026A
loc_4_0247:
        push   ecx
        or     dh, dh
        jne    SHORT loc_4_024E
        mov    dh, dl
loc_4_024E:
        mov    ecx, edx
        mov    edx, 3deh
        in     al, dx
        ror    eax, 8
        mov    al, 23h
        mov    ah, ch
        out    dx, ax
        mov    al, 24h
        mov    ah, cl
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    ecx
loc_4_026A:
        pop    edx
        pop    eax
        stc
        ret
loc_4_026E:
        db  08Bh, 00Dh, 019h, 001h, 000h, 000h
        stc
        ret
_PTEXT ends
        end

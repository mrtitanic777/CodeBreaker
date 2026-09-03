; es1488_lcode_01.asm -- ES1488 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  084h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  086h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
loc_1_0040:
        sal    byte ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0050:
        sal    dword ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 0F5h, 033h, 004h, 000h, 000h, 000h, 041h, 055h, 044h, 044h
        db  052h, 056h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00DC:
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx + ebx], dl
        xor    dh, byte ptr [esi]
        cmp    byte ptr [esi], bh
        add    ah, cl
AUDDRV_Control:
        cmp    eax, 8
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_010D:
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0116:
        cmp    eax, 3
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_011F:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0128:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0131:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_013A:
        clc
        ret
sub_1_013C:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   esi
        pushfd
        mov    eax, dword ptr [ebp + 0ch]
        shl    eax, 3
        db  08Bh, 090h, 000h, 000h, 000h, 000h
        db  08Bh, 080h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp + 0ch], eax
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0193
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_0193
loc_1_016F:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0ch], 1
        jne    SHORT loc_1_0183
        cmp    dword ptr [edx + edi], ebx
        jne    SHORT loc_1_0189
loc_1_017F:
        popfd
        clc
        jmp    SHORT loc_1_0197
loc_1_0183:
        cmp    word ptr [edx + edi], bx
        je     SHORT loc_1_017F
loc_1_0189:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_0193
        jmp    SHORT loc_1_016F
loc_1_0193:
        popfd
        xor    edi, edi
        stc
loc_1_0197:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_1_019D:
        push   eax
        push   ebx
        pushfd
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     SHORT loc_1_01CA
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_01CA
loc_1_01B5:
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_01C0
        mov    esi, eax
        popfd
        clc
        jmp    SHORT loc_1_01CE
loc_1_01C0:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_01CA
        jmp    SHORT loc_1_01B5
loc_1_01CA:
        popfd
        xor    esi, esi
        stc
loc_1_01CE:
        pop    ebx
        pop    eax
        ret
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_01DC:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_022B
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_022B
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_022B
loc_1_01FD:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_1_0209
        or     ecx, 1
loc_1_0209:
        cmp    dword ptr [edi + 34h], ebx
        jne    SHORT loc_1_0211
        or     ecx, 2
loc_1_0211:
        jecxz  loc_1_021C
        push   eax
        mov    eax, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_021B:
        pop    eax
loc_1_021C:
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_022B
        jmp    SHORT loc_1_01FD
loc_1_022B:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
        db  090h
loc_1_0234:
        db  0CDh, 020h, 012h, 080h, 004h, 000h
        mov    eax, eax
loc_1_023C:
        cld
        push   edi
        push   edx
        push   ebx
        mov    edi, edx
        mov    ebx, dword ptr [edi + 2ch]
        or     ebx, ebx
        je     SHORT loc_1_0250
        cmp    word ptr [edi + 6eh], 0
        je     SHORT loc_1_0262
loc_1_0250:
        push   eax
        movzx  edx, word ptr [edi + 6]
        add    edx, 0eh
        in     al, dx
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        jmp    SHORT loc_1_0268
loc_1_0262:
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_0268:
        pop    ebx
        pop    edx
        pop    edi
        clc
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_0270:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
        db  08Bh, 0C0h
loc_1_0280:
        push   edi
        push   1
        push   eax
        call   sub_1_013C
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_02B1
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_1_02A7
        jecxz  loc_1_029F
loc_1_0297:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_02B1
loc_1_029F:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_1_02B1
loc_1_02A7:
        test   word ptr [edi + 4], 8000h
        jne    SHORT loc_1_029F
        je     SHORT loc_1_0297
loc_1_02B1:
        pop    edi
        clc
        ret
sub_1_02B4:
        push   ecx
        push   edx
        add    edx, 0eh
        mov    ecx, 5dch
loc_1_02BE:
        in     al, dx
        or     al, al
        js     SHORT loc_1_02C8
        loop   loc_1_02BE
        stc
        jmp    SHORT loc_1_02CD
loc_1_02C8:
        sub    edx, 4
        in     al, dx
        clc
loc_1_02CD:
        pop    edx
        pop    ecx
        ret
sub_1_02D0:
        push   eax
        push   ecx
        push   edx
        add    edx, 0ch
        push   eax
        mov    ecx, 0a0000h
loc_1_02DC:
        in     al, dx
        or     al, al
        jns    SHORT loc_1_02E7
        loop   loc_1_02DC
        pop    eax
        stc
        jmp    SHORT loc_1_02EA
loc_1_02E7:
        pop    eax
        out    dx, al
        clc
loc_1_02EA:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_02EE:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    SHORT loc_1_02FE
loc_1_02FE:
        jmp    SHORT loc_1_0300
loc_1_0300:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_02B4
        pop    edx
        pop    eax
        ret
sub_1_0311:
        push   eax
        call   sub_1_02EE
        jb     SHORT loc_1_0329
        mov    al, 10h
        call   sub_1_02D0
        jb     SHORT loc_1_0329
        mov    al, 80h
        call   sub_1_02D0
loc_1_0329:
        pop    eax
        ret
sub_1_032B:
        push   eax
        mov    al, 0c6h
        call   sub_1_02D0
        jb     SHORT loc_1_0359
        mov    al, 0c3h
        call   sub_1_02D0
        jb     SHORT loc_1_0359
        call   sub_1_02B4
        jb     SHORT loc_1_0359
        test   al, 1
        jne    SHORT loc_1_0359
        mov    al, 0c2h
        call   sub_1_02D0
        jb     SHORT loc_1_0359
        mov    al, 1
        call   sub_1_02D0
loc_1_0359:
        pop    eax
        ret
sub_1_035B:
        push   eax
        push   edx
        push   edi
        push   esi
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_038D
        cld
        movzx  edx, word ptr [edi + 6]
        call   sub_1_032B
        add    dx, 2
        add    edi, 50h
        db  0BEh, 000h, 000h, 000h, 000h
loc_1_037D:
        lodsb
        or     al, al
        je     SHORT loc_1_038D
        out    dx, al
        inc    dx
        mov    al, byte ptr [edi]
        inc    edi
        out    dx, al
        dec    dx
        jmp    SHORT loc_1_037D
loc_1_038D:
        pop    esi
        pop    edi
        pop    edx
        pop    eax
        clc
        ret
sub_1_0393:
        push   eax
        push   edx
        push   edi
        push   esi
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_03BE
        cld
        movzx  edx, word ptr [edi + 6]
        add    dx, 2
        add    edi, 50h
        db  0BEh, 000h, 000h, 000h, 000h
loc_1_03B0:
        lodsb
        or     al, al
        je     SHORT loc_1_03BE
        out    dx, al
        inc    dx
        in     al, dx
        stosb
        dec    dx
        jmp    SHORT loc_1_03B0
loc_1_03BE:
        pop    esi
        pop    edi
        pop    edx
        pop    eax
        clc
        ret
sub_1_03C4:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd
        cli
        popfd
        clc
        popad
        ret
sub_1_03D2:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd
        cli
        popfd
        clc
        popad
        ret
sub_1_03E0:
        push   eax
        push   ecx
        mov    ecx, 32h
loc_1_03E7:
        in     al, dx
        jmp    SHORT loc_1_03EA
loc_1_03EA:
        loop   loc_1_03E7
        pop    ecx
        pop    eax
        ret
sub_1_03EF:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_03E0
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_03E0
        pop    edx
        pop    ebx
        ret
_LTEXT ends
        end

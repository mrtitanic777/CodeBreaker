; es488_lcode_01.asm -- ES488 object 1 segment _LTEXT
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
        db  000h, 000h, 000h, 000h, 000h, 004h, 035h, 030h, 004h, 000h, 000h, 000h, 041h, 055h, 044h, 044h
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
        db  000h
AUDDRV_Control:
        cmp    eax, 8
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0105:
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_010E:
        cmp    eax, 3
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0117:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0120:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0129:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0132:
        clc
        ret
sub_1_0134:
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
        je     SHORT loc_1_018B
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_018B
loc_1_0167:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0ch], 1
        jne    SHORT loc_1_017B
        cmp    dword ptr [edx + edi], ebx
        jne    SHORT loc_1_0181
loc_1_0177:
        popfd
        clc
        jmp    SHORT loc_1_018F
loc_1_017B:
        cmp    word ptr [edx + edi], bx
        je     SHORT loc_1_0177
loc_1_0181:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_018B
        jmp    SHORT loc_1_0167
loc_1_018B:
        popfd
        xor    edi, edi
        stc
loc_1_018F:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_1_0195:
        push   eax
        push   ebx
        pushfd
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     SHORT loc_1_01C2
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_01C2
loc_1_01AD:
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_01B8
        mov    esi, eax
        popfd
        clc
        jmp    SHORT loc_1_01C6
loc_1_01B8:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_01C2
        jmp    SHORT loc_1_01AD
loc_1_01C2:
        popfd
        xor    esi, esi
        stc
loc_1_01C6:
        pop    ebx
        pop    eax
        ret
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_01D4:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_0223
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0223
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_0223
loc_1_01F5:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_1_0201
        or     ecx, 1
loc_1_0201:
        cmp    dword ptr [edi + 34h], ebx
        jne    SHORT loc_1_0209
        or     ecx, 2
loc_1_0209:
        jecxz  loc_1_0214
        push   eax
        mov    eax, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0213:
        pop    eax
loc_1_0214:
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_0223
        jmp    SHORT loc_1_01F5
loc_1_0223:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
        db  090h
loc_1_022C:
        db  0CDh, 020h, 012h, 080h, 004h, 000h
        mov    eax, eax
loc_1_0234:
        cld
        push   edi
        push   edx
        push   ebx
        mov    edi, edx
        mov    ebx, dword ptr [edi + 2ch]
        or     ebx, ebx
        je     SHORT loc_1_0248
        cmp    word ptr [edi + 64h], 0
        je     SHORT loc_1_025A
loc_1_0248:
        push   eax
        movzx  edx, word ptr [edi + 6]
        add    edx, 0eh
        in     al, dx
        pop    eax
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        jmp    SHORT loc_1_0260
loc_1_025A:
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_0260:
        pop    ebx
        pop    edx
        pop    edi
        clc
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_0268:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
        db  08Bh, 0C0h
loc_1_0278:
        push   edi
        push   1
        push   eax
        call   sub_1_0134
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_02A9
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_1_029F
        jecxz  loc_1_0297
loc_1_028F:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_02A9
loc_1_0297:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_1_02A9
loc_1_029F:
        test   word ptr [edi + 4], 8000h
        jne    SHORT loc_1_0297
        je     SHORT loc_1_028F
loc_1_02A9:
        pop    edi
        clc
        ret
sub_1_02AC:
        push   ecx
        push   edx
        add    edx, 0eh
        mov    ecx, 5dch
loc_1_02B6:
        in     al, dx
        or     al, al
        js     SHORT loc_1_02C0
        loop   loc_1_02B6
        stc
        jmp    SHORT loc_1_02C5
loc_1_02C0:
        sub    edx, 4
        in     al, dx
        clc
loc_1_02C5:
        pop    edx
        pop    ecx
        ret
sub_1_02C8:
        push   eax
        push   ecx
        push   edx
        add    edx, 0ch
        push   eax
        mov    ecx, 0a0000h
loc_1_02D4:
        in     al, dx
        or     al, al
        jns    SHORT loc_1_02DF
        loop   loc_1_02D4
        pop    eax
        stc
        jmp    SHORT loc_1_02E2
loc_1_02DF:
        pop    eax
        out    dx, al
        clc
loc_1_02E2:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_02E6:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    SHORT loc_1_02F6
loc_1_02F6:
        jmp    SHORT loc_1_02F8
loc_1_02F8:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_02AC
        pop    edx
        pop    eax
        ret
sub_1_0309:
        push   eax
        add    edx, 0ch
        in     al, dx
        sub    edx, 0ch
        or     al, al
        jns    SHORT loc_1_031C
        call   sub_1_02E6
        jb     SHORT loc_1_032C
loc_1_031C:
        mov    al, 10h
        call   sub_1_02C8
        jb     SHORT loc_1_032C
        mov    al, 80h
        call   sub_1_02C8
loc_1_032C:
        pop    eax
        ret
sub_1_032E:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd
        cli
        popfd
        clc
        popad
        ret
sub_1_033C:
        pushad
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd
        cli
        popfd
        clc
        popad
        ret
sub_1_034A:
        push   eax
        push   ecx
        mov    ecx, 32h
loc_1_0351:
        in     al, dx
        jmp    SHORT loc_1_0354
loc_1_0354:
        loop   loc_1_0351
        pop    ecx
        pop    eax
        ret
sub_1_0359:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_034A
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_034A
        pop    edx
        pop    ebx
        ret
        db  0CCh, 0CCh
_LTEXT ends
        end

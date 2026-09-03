; s3mm_lcode_01.asm -- S3MM object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

S3MINI_Control:
        cmp    eax, 1ch
        jae    SHORT loc_1_000C
        db  0FFh, 024h, 085h, 00Eh, 000h, 000h, 000h
loc_1_000C:
        clc
        ret
        db  000h, 000h, 000h, 000h, 01Bh, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 07Eh, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 0D5h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 01Bh, 000h, 000h, 000h
S3MINI_CREATE_VM:
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_1_00D4
        pushad
        db  0C6h, 005h, 038h, 015h, 000h, 000h, 001h
        db  08Bh, 01Dh, 084h, 014h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4022h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
loc_1_00D4:
        ret
S3MINI_VM_INIT:
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_1_00F6
        pushad
        mov    eax, 0fah
        db  0BEh, 0F7h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 049h, 015h, 000h, 000h
        popad
loc_1_00F6:
        ret
loc_1_00F7:
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_1_012E
        pushad
        db  08Bh, 035h, 049h, 015h, 000h, 000h
        cmp    esi, 0
        je     SHORT loc_1_011C
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  0C7h, 005h, 049h, 015h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_011C:
        db  08Bh, 01Dh, 084h, 014h, 000h, 000h
        db  0BEh, 02Fh, 001h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        popad
loc_1_012E:
        ret
loc_1_012F:
        pushad
        db  08Bh, 01Dh, 084h, 014h, 000h, 000h
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4023h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        popad
        ret
loc_1_0178:
        pushad
        db  08Bh, 01Dh, 084h, 014h, 000h, 000h
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4021h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        popad
        ret
loc_1_01C1:
        db  03Bh, 01Dh, 084h, 014h, 000h, 000h
        je     SHORT loc_1_0220
        cmp    edx, 9ae8h
        je     SHORT loc_1_01D9
        cmp    edx, 9ae9h
        jne    SHORT loc_1_0220
loc_1_01D9:
        cmp    ecx, 0
        je     SHORT loc_1_0201
        cmp    ecx, 8
        jne    SHORT loc_1_0220
        mov    ecx, 0ffffh
loc_1_01E8:
        in     ax, dx
        cmp    ax, -1
        je     SHORT loc_1_01FC
        test   ah, 2
        je     SHORT loc_1_021D
        db  0E8h, 0ADh, 009h, 0FFh, 0FFh
        loop   loc_1_01E8
loc_1_01FC:
        and    ah, 0fdh
        jmp    SHORT loc_1_021D
loc_1_0201:
        cmp    dl, 0e9h
        jne    SHORT loc_1_0220
        mov    ecx, 0ffffh
loc_1_020B:
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_1_021B
        test   al, 2
        je     SHORT loc_1_021D
        db  0E8h, 08Eh, 009h, 0FFh, 0FFh
        loop   loc_1_020B
loc_1_021B:
        and    al, 0fdh
loc_1_021D:
        stc
        jmp    SHORT loc_1_0221
loc_1_0220:
        clc
loc_1_0221:
        ret
loc_1_0222:
        cmp    dx, 4f00h
        je     SHORT loc_1_023F
        cmp    dx, 4f02h
        jne    SHORT loc_1_0264
        db  03Bh, 01Dh, 084h, 014h, 000h, 000h
        je     SHORT loc_1_0264
        db  0E8h, 00Ch, 003h, 0FFh, 0FFh
        jmp    SHORT loc_1_0264
loc_1_023F:
        db  03Bh, 01Dh, 084h, 014h, 000h, 000h
        jne    SHORT loc_1_0264
        push   eax
        push   edx
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_1_0264:
        ret
        db  000h, 000h, 000h
sub_1_0268:
        push   eax
        push   ebx
        push   ecx
        push   edx
        test   word ptr [eax + 2], 200h
        jne    SHORT loc_1_02AC
        movzx  ecx, word ptr [eax + 24h]
        movzx  edx, word ptr [eax + 26h]
        db  0BBh, 034h, 004h, 000h, 000h
loc_1_0281:
        mov    al, byte ptr [ebx]
        cmp    dl, al
        jb     SHORT loc_1_0299
        push   ebx
        mov    eax, edi
        shr    eax, 10h
        and    eax, 0fh
        add    ebx, eax
        mov    al, byte ptr [ebx]
        cmp    al, 0ffh
        pop    ebx
        jne    SHORT loc_1_02A0
loc_1_0299:
        add    ebx, 7
        or     al, al
        jne    SHORT loc_1_0281
loc_1_02A0:
        xchg   bx, si
        cmp    bl, al
        jbe    SHORT loc_1_02A9
        xchg   al, bl
loc_1_02A9:
        mov    si, bx
loc_1_02AC:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_1_02B1:
        add    byte ptr [edx], 1
        add    byte ptr [eax], ah
        add    eax, dword ptr [edx]
        add    byte ptr [eax], al
        add    al, 3
        add    byte ptr [eax + 404h], al
        add    eax, 6400005h
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 2], al
        db  000h, 056h, 008h
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    al, 0
        push   cs
        push   es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [edx]
        db  000h, 040h, 007h
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax + eax]
        fadd   qword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 24000h], al
        push   esi
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 44000h], al
        push   cs
        push   es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 2
        add    al, byte ptr [eax]
        inc    eax
        pop    es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 2
        add    al, 0
        fadd   qword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], 2
        db  000h, 056h, 008h
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        db  000h, 080h, 000h, 004h, 000h, 00Eh
        push   es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [edx], al
        db  000h, 040h, 007h
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax + eax], al
        fadd   qword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 20080h], al
        push   esi
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 40080h], al
        push   cs
        push   es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 10h
        add    al, byte ptr [eax]
        inc    eax
        pop    es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 10h
        add    al, 0
        fadd   qword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, byte ptr [eax]
        insb
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, 0
        or     byte ptr [ecx], 0
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
        db  000h, 099h, 00Ah, 000h, 000h, 002h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax + eax], eax
        cwde
        or     dword ptr [eax], eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 2
        db  000h, 010h
        sldt   word ptr [eax]
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], al
        db  000h, 018h
        adc    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 2
        db  000h, 09Ah, 00Bh, 000h, 000h, 001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 1
        db  000h, 0A2h, 00Ch, 000h, 000h, 001h
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [edx + eax], al
        db  000h, 055h, 00Dh
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, 1
        db  000h, 05Dh, 00Eh
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + eax], al
        db  000h, 0CBh
        adc    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 1
        db  000h, 0BEh, 011h, 000h, 000h, 004h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 2
        db  000h, 06Ah, 012h
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], al
        db  000h, 025h, 013h, 000h, 000h, 000h
loc_1_0434:
        pushad
        db  0FFh, 0FFh, 078h, 0FFh, 0FFh, 04Ch, 05Fh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 04Bh, 059h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 048h, 052h, 0FFh, 0FFh, 0FFh, 05Ah, 0FFh, 042h, 051h, 0FFh, 0FFh, 064h, 0FFh
        db  0FFh, 0FFh, 04Fh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 0FFh, 04Eh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ah, 0FFh
        db  04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 0FFh, 04Bh, 0FFh, 078h, 0FFh, 0FFh, 0FFh, 0FFh, 04Ah, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 047h, 0FFh, 0FFh, 0FFh, 04Bh, 0FFh, 0FFh, 040h, 078h, 064h, 050h
        db  046h, 03Ch, 0FFh, 03Ch, 0FFh, 0FFh, 04Bh, 0FFh, 0FFh, 0FFh, 03Ah, 0FFh, 0FFh, 048h, 0FFh, 0FFh
        db  0FFh, 038h, 064h, 05Ah, 046h, 03Ch, 0FFh, 0FFh, 031h, 0FFh, 0FFh, 03Ch, 0FFh, 02Bh, 0FFh, 030h
        db  05Ah, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 02Eh, 0FFh, 04Bh, 0FFh, 0FFh, 0FFh, 0FFh, 026h, 048h, 03Ch
        db  0FFh, 0FFh, 0FFh, 0FFh, 025h, 04Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 024h, 0FFh, 038h, 02Bh, 0FFh
        db  0FFh, 0FFh, 023h, 0FFh, 038h, 0FFh, 0FFh, 0FFh, 0FFh, 01Fh, 03Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 03Ch, 03Ch, 02Bh, 03Ch, 02Bh, 03Ch
loc_1_04DC:
        pop    ebx
        db  0E0h, 0F2h
        add    al, 0
        db  000h, 0F2h
        add    al, 0
        db  000h, 047h, 005h
        add    byte ptr [eax], al
        pushfd
        db  005h, 000h, 000h, 0B9h, 005h
        add    byte ptr [eax], al
loc_1_04F2:
        js     SHORT loc_1_0502
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0502:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0547:
        js     SHORT loc_1_0557
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0557:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_059C:
        dec    ebx
        db  0FFh, 0FFh, 0FFh, 0FFh, 002h, 0FFh, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 0FFh, 03Ch, 0FFh, 0FFh
        db  0FFh, 0FFh, 007h, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 000h
loc_1_05B9:
        js     SHORT loc_1_05C9
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_05C9:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_060E:
        pop    ebx
        db  0E0h, 024h
        push   es
        add    byte ptr [eax], al
        and    al, 6
        add    byte ptr [eax], al
        jns    SHORT loc_1_0620
        add    byte ptr [eax], al
        into
        push   es
        add    byte ptr [eax], al
loc_1_0620:
        db  0EBh, 006h
        db  000h, 000h
loc_1_0624:
        js     SHORT loc_1_0634
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0634:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0679:
        js     SHORT loc_1_0689
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0689:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_06CE:
        dec    ebx
        db  0FFh, 0FFh, 0FFh, 0FFh, 002h, 0FFh, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 0FFh, 03Ch, 0FFh, 0FFh
        db  0FFh, 0FFh, 007h, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 000h
loc_1_06EB:
        js     SHORT loc_1_06FB
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_06FB:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0740:
        pop    ebx
        db  0E0h, 056h
        pop    es
        add    byte ptr [eax], al
        push   esi
        pop    es
        add    byte ptr [eax], al
        stosd
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    dword ptr [eax], ecx
        add    byte ptr [eax], al
loc_1_0756:
        js     SHORT loc_1_0766
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0766:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_07AB:
        js     SHORT loc_1_07BB
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_07BB:
        add    al, 0ffh
        add    al, 0ffh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0801:
        js     SHORT loc_1_0811
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0811:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0856:
        pop    ebx
        db  0E0h, 06Ch
        or     byte ptr [eax], al
        db  000h, 06Ch, 008h, 000h
        db  000h, 0C1h
        or     byte ptr [eax], al
        db  000h, 016h
        or     dword ptr [eax], eax
        db  000h, 017h
        or     dword ptr [eax], eax
        db  000h
loc_1_086C:
        js     SHORT loc_1_087C
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_087C:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_08C1:
        js     SHORT loc_1_08D1
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_08D1:
        add    al, 0ffh
        add    al, 0ffh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0917:
        js     SHORT loc_1_0927
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0927:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_096C:
        pop    ebx
        db  0E0h, 0AFh
        or     al, byte ptr [eax]
        db  000h, 0AFh, 00Ah, 000h, 000h, 0FDh
        or     al, byte ptr [eax]
        db  000h, 04Bh, 00Bh
        add    byte ptr [eax], al
        dec    esp
        or     eax, dword ptr [eax]
        add    byte ptr [ebx - 20h], bl
        scasb
        or     dword ptr [eax], eax
        db  000h, 0AEh, 009h, 000h, 000h, 0FCh
        or     dword ptr [eax], eax
        db  000h, 04Ah, 00Ah
        add    byte ptr [eax], al
        dec    ebx
        or     al, byte ptr [eax]
        add    byte ptr [ebx - 20h], bl
        scasb
        or     dword ptr [eax], eax
        db  000h, 0AEh, 009h, 000h, 000h, 0FCh
        or     dword ptr [eax], eax
        db  000h, 04Ah, 00Ah
        add    byte ptr [eax], al
        dec    ebx
        or     al, byte ptr [eax]
        db  000h
loc_1_09AE:
        js     SHORT loc_1_09BE
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_09BE:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_09FC:
        js     SHORT loc_1_0A0C
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0A0C:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h, 000h
loc_1_0A4B:
        js     SHORT loc_1_0A5B
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0A5B:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0A99:
        pop    ebx
        db  0E0h, 0AFh
        or     al, byte ptr [eax]
        db  000h, 0AFh, 00Ah, 000h, 000h, 0FDh
        or     al, byte ptr [eax]
        db  000h, 04Bh, 00Bh
        add    byte ptr [eax], al
        dec    esp
        or     eax, dword ptr [eax]
        db  000h
loc_1_0AAF:
        js     SHORT loc_1_0ABF
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0ABF:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_0AFD:
        js     SHORT loc_1_0B0D
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0B0D:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0B4C:
        js     SHORT loc_1_0B5C
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0B5C:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_0B9A:
        pop    ebx
        db  0E0h, 0B0h
        or     eax, dword ptr [eax]
        db  000h, 0B0h, 00Bh, 000h, 000h, 0FEh
        or     eax, dword ptr [eax]
        db  000h, 04Ch, 00Ch, 000h
        db  000h, 04Dh, 00Ch
        add    byte ptr [eax], al
loc_1_0BB0:
        js     SHORT loc_1_0BC0
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0BC0:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0BFE:
        js     SHORT loc_1_0C0E
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0C0E:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0C4D:
        db  078h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0CA2:
        pop    ebx
        db  0E0h, 0B8h
        or     al, 0
        db  000h, 0B8h, 00Ch, 000h, 000h, 006h
        db  00Dh, 000h, 000h, 054h, 00Dh
        add    byte ptr [eax], al
        push   esp
        db  00Dh, 000h, 000h
loc_1_0CB8:
        js     SHORT loc_1_0CC8
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0CC8:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0D06:
        js     SHORT loc_1_0D16
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0D16:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0D55:
        pop    ebx
        db  0E0h, 06Bh
        db  00Dh, 000h, 000h, 06Bh, 00Dh
        add    byte ptr [eax], al
        db  0B9h, 00Dh, 000h, 000h, 007h
        push   cs
        add    byte ptr [eax], al
        pop    es
        push   cs
        add    byte ptr [eax], al
loc_1_0D6B:
        js     SHORT loc_1_0D7B
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0D7B:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0DB9:
        js     SHORT loc_1_0DC9
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0DC9:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h, 078h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 000h
loc_1_0E5D:
        pop    ebx
        db  0E0h, 073h
        push   cs
        add    byte ptr [eax], al
        jae    SHORT loc_1_0E73
        add    byte ptr [eax], al
        ror    dword ptr [esi], 0
        db  000h, 00Fh
        sldt   word ptr [eax]
        db  00Fh, 00Fh, 000h, 000h
loc_1_0E73:
        js     SHORT loc_1_0E83
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0E83:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0EC1:
        db  078h, 00Eh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0F10:
        db  06Bh, 0E0h, 026h
        sldt   word ptr [eax]
        sldt   word ptr es:[eax]
        db  074h, 00Fh
        add    byte ptr [eax], al
        ret    0fh
        db  000h, 0C2h, 00Fh, 000h, 000h
loc_1_0F26:
        js     SHORT loc_1_0F36
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0F36:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0F74:
        js     SHORT loc_1_0F84
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0F84:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h, 078h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 000h
loc_1_1018:
        db  06Bh, 0E0h, 02Eh
        adc    byte ptr [eax], al
        db  000h, 02Eh
        adc    byte ptr [eax], al
        db  000h, 07Ch, 010h, 000h
        db  000h, 0CAh
        adc    byte ptr [eax], al
        db  000h, 0CAh
        adc    byte ptr [eax], al
        db  000h
loc_1_102E:
        js     SHORT loc_1_103E
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_103E:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_107C:
        js     SHORT loc_1_108C
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_108C:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_10CB:
        pop    ebx
        db  0E0h, 0E1h
        adc    byte ptr [eax], al
        db  000h, 0E1h
        adc    byte ptr [eax], al
        db  000h, 028h
        adc    dword ptr [eax], eax
        db  000h, 06Fh, 011h
        add    byte ptr [eax], al
        db  077h, 011h
        add    byte ptr [eax], al
loc_1_10E1:
        js     SHORT loc_1_10F1
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_10F1:
        add    al, 0ffh
        add    al, 0ffh
        call   dword ptr [eax - 1]
        dec    dword ptr [4bffffffh]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    bh, bh
        dec    eax
        add    byte ptr [esi], al
        add    al, 0ffh
        add    al, 0ffh
        inc    esi
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_1128:
        js     SHORT loc_1_1138
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1138:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh
        db  0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_116F:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1177:
        db  078h, 00Eh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_11BE:
        pop    ebx
        db  0E0h, 0D4h
        adc    dword ptr [eax], eax
        db  000h, 0D4h
        adc    dword ptr [eax], eax
        db  000h, 01Bh
        adc    al, byte ptr [eax]
        db  000h, 062h, 012h
        add    byte ptr [eax], al
        bound  edx, qword ptr [edx]
        add    byte ptr [eax], al
loc_1_11D4:
        js     SHORT loc_1_11E4
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_11E4:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 00Ah, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 006h, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_121B:
        js     SHORT loc_1_122B
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_122B:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1262:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_126A:
        db  06Bh, 0E0h, 080h
        adc    al, byte ptr [eax]
        db  000h, 080h, 012h, 000h, 000h, 0CEh
        adc    al, byte ptr [eax]
        db  000h, 01Ch, 013h
        add    byte ptr [eax], al
        db  01Dh, 013h, 000h, 000h
loc_1_1280:
        js     SHORT loc_1_1290
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1290:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_12CE:
        db  078h, 00Eh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_131D:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1325:
        db  06Bh, 0E0h, 03Bh
        adc    eax, dword ptr [eax]
        db  000h, 03Bh
        adc    eax, dword ptr [eax]
        db  000h, 089h, 013h, 000h, 000h, 0D7h
        adc    eax, dword ptr [eax]
        db  000h, 0DFh
        adc    eax, dword ptr [eax]
        db  000h
loc_1_133B:
        js     SHORT loc_1_134B
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_134B:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_1389:
        js     SHORT loc_1_1399
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1399:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_13D7:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_13E0:
        db  03Bh, 01Dh, 070h, 015h, 000h, 000h
        jne    SHORT loc_1_13F9
        db  066h, 083h, 03Dh, 0CCh, 015h, 000h, 000h, 040h
        jne    SHORT loc_1_13F9
        cmp    dx, 4f15h
        je     SHORT loc_1_1408
loc_1_13F9:
        db  083h, 03Dh, 0BFh, 015h, 000h, 000h, 000h
        je     SHORT loc_1_142F
        db  0FFh, 025h, 0BFh, 015h, 000h, 000h
loc_1_1408:
        push   eax
        push   edx
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 10h
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 55h
        out    dx, al
        in     ax, dx
        and    ah, 0f8h
        out    dx, ax
        rol    eax, 10h
        out    dx, al
        pop    edx
        pop    eax
loc_1_142F:
        ret
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 053h, 033h, 04Dh, 049h
        db  04Eh, 049h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_14B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_14E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1526:
        add    byte ptr [eax], al
loc_1_1528:
        add    byte ptr [eax], al
loc_1_152A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1530:
        add    byte ptr [eax], al
loc_1_1532:
        add    byte ptr [eax], al
loc_1_1534:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1538:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_153C:
        add    byte ptr [eax], al
loc_1_153E:
        inc    dword ptr [eax]
loc_1_1540:
        add    byte ptr [eax], al
loc_1_1542:
        inc    dword ptr [eax]
loc_1_1544:
        add    byte ptr [eax], al
loc_1_1546:
        add    byte ptr [eax], al
loc_1_1548:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 4eh], al
        push   ebp
        dec    ebp
        pop    esp
        push   eax
        inc    ebx
        dec    ecx
        pop    esp
        push   esi
        inc    ebp
        dec    esi
        pop    edi
        cmp    byte ptr [eax], dh
        cmp    byte ptr [esi], dh
        inc    esp
        inc    ebp
        push   esi
        pop    edi
        xor    byte ptr [eax + edi], dh
        add    byte ptr ss:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_15BF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_15C3:
        and    eax, 5b000013h
        add    byte ptr [ecx], bh
loc_1_15CA:
        add    byte ptr [eax], al
loc_1_15CC:
        add    byte ptr [eax], al
loc_1_15CE:
        add    byte ptr [eax], al
_LTEXT ends
        end

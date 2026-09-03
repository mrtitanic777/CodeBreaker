; s3_lcode_01.asm -- S3 object 1 segment _LTEXT
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
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 01Bh, 000h, 000h, 000h
loc_1_007E:
        db  03Bh, 01Dh, 040h, 013h, 000h, 000h
        je     SHORT loc_1_00DD
        cmp    edx, 9ae8h
        je     SHORT loc_1_0096
        cmp    edx, 9ae9h
        jne    SHORT loc_1_00DD
loc_1_0096:
        cmp    ecx, 0
        je     SHORT loc_1_00BE
        cmp    ecx, 8
        jne    SHORT loc_1_00DD
        mov    ecx, 0ffffh
loc_1_00A5:
        in     ax, dx
        cmp    ax, -1
        je     SHORT loc_1_00B9
        test   ah, 2
        je     SHORT loc_1_00DA
        db  0E8h, 049h, 007h, 0FFh, 0FFh
        loop   loc_1_00A5
loc_1_00B9:
        and    ah, 0fdh
        jmp    SHORT loc_1_00DA
loc_1_00BE:
        cmp    dl, 0e9h
        jne    SHORT loc_1_00DD
        mov    ecx, 0ffffh
loc_1_00C8:
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_1_00D8
        test   al, 2
        je     SHORT loc_1_00DA
        db  0E8h, 02Ah, 007h, 0FFh, 0FFh
        loop   loc_1_00C8
loc_1_00D8:
        and    al, 0fdh
loc_1_00DA:
        stc
        jmp    SHORT loc_1_00DE
loc_1_00DD:
        clc
loc_1_00DE:
        ret
loc_1_00DF:
        cmp    dx, 4f00h
        je     SHORT loc_1_00FC
        cmp    dx, 4f02h
        jne    SHORT loc_1_0121
        db  03Bh, 01Dh, 040h, 013h, 000h, 000h
        je     SHORT loc_1_0121
        db  0E8h, 07Ch, 002h, 0FFh, 0FFh
        jmp    SHORT loc_1_0121
loc_1_00FC:
        db  03Bh, 01Dh, 040h, 013h, 000h, 000h
        jne    SHORT loc_1_0121
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
loc_1_0121:
        ret
        db  000h, 000h
sub_1_0124:
        push   eax
        push   ebx
        push   ecx
        push   edx
        test   word ptr [eax + 2], 200h
        jne    SHORT loc_1_0168
        movzx  ecx, word ptr [eax + 24h]
        movzx  edx, word ptr [eax + 26h]
        db  0BBh, 0F0h, 002h, 000h, 000h
loc_1_013D:
        mov    al, byte ptr [ebx]
        cmp    dl, al
        jb     SHORT loc_1_0155
        push   ebx
        mov    eax, edi
        shr    eax, 10h
        and    eax, 0fh
        add    ebx, eax
        mov    al, byte ptr [ebx]
        cmp    al, 0ffh
        pop    ebx
        jne    SHORT loc_1_015C
loc_1_0155:
        add    ebx, 7
        or     al, al
        jne    SHORT loc_1_013D
loc_1_015C:
        xchg   bx, si
        cmp    bl, al
        jbe    SHORT loc_1_0165
        xchg   al, bl
loc_1_0165:
        mov    si, bx
loc_1_0168:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_1_016D:
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
        db  000h, 012h
        pop    es
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        add    al, 0
        retf   4
        db  000h, 040h, 000h, 000h, 000h, 000h, 002h, 002h, 000h, 0FCh, 005h, 000h, 000h, 040h, 000h, 000h
        db  000h, 000h, 002h, 004h, 000h, 098h, 003h, 000h, 000h, 040h, 000h, 000h, 080h, 000h, 040h, 002h
        db  000h, 012h, 007h, 000h, 000h, 040h, 000h, 000h, 080h, 000h, 040h, 004h, 000h, 0CAh, 004h, 000h
        db  000h, 040h, 000h, 000h, 080h, 000h, 002h, 002h, 000h, 0FCh, 005h, 000h, 000h, 040h, 000h, 000h
        db  080h, 000h, 002h, 004h, 000h, 098h, 003h, 000h, 000h, 040h, 000h, 000h, 000h, 080h, 000h, 002h
        db  000h, 012h, 007h, 000h, 000h, 040h, 000h, 000h, 000h, 080h, 000h, 004h, 000h, 0CAh, 004h, 000h
        db  000h, 040h, 000h, 000h, 000h, 000h, 010h, 002h, 000h, 0FCh, 005h, 000h, 000h, 040h, 000h, 000h
        db  000h, 000h, 010h, 004h, 000h, 098h, 003h, 000h, 000h, 040h, 000h, 000h, 080h, 080h, 000h, 002h
        db  000h, 012h, 007h, 000h, 000h, 040h, 000h, 000h, 080h, 080h, 000h, 004h, 000h, 0CAh, 004h, 000h
        db  000h, 040h, 000h, 000h, 080h, 000h, 010h, 002h, 000h, 0FCh, 005h, 000h, 000h, 040h, 000h, 000h
        db  080h, 000h, 010h, 004h, 000h, 098h, 003h, 000h, 000h, 040h, 000h, 000h, 000h, 000h, 001h, 002h
        db  000h, 028h, 008h, 000h, 000h, 040h, 000h, 000h, 000h, 000h, 001h, 004h, 000h, 03Eh, 008h, 000h
        db  000h, 002h, 000h, 000h, 000h, 000h, 001h, 002h, 000h, 055h, 009h, 000h, 000h, 002h, 000h, 000h
        db  000h, 000h, 001h, 004h, 000h, 054h, 008h, 000h, 000h, 008h, 000h, 000h, 000h, 000h, 004h, 002h
        db  000h, 0CCh, 00Dh, 000h, 000h, 008h, 000h, 000h, 000h, 000h, 004h, 001h, 000h, 0D4h, 00Eh, 000h
        db  000h, 001h, 000h, 000h, 000h, 000h, 004h, 002h, 000h, 056h, 00Ah, 000h, 000h, 001h, 000h, 000h
        db  000h, 000h, 004h, 001h, 000h, 05Eh, 00Bh, 000h, 000h, 001h, 000h, 000h, 040h, 000h, 004h, 002h
        db  000h, 011h, 00Ch, 000h, 000h, 001h, 000h, 000h, 040h, 000h, 004h, 001h, 000h, 019h, 00Dh, 000h
        db  000h, 020h, 000h, 000h, 000h, 000h, 004h, 002h, 000h, 087h, 00Fh, 000h, 000h, 020h, 000h, 000h
        db  000h, 000h, 004h, 001h, 000h, 07Ah, 010h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 004h, 002h
        db  000h, 026h, 011h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 004h, 001h, 000h, 0E1h, 011h, 000h
        db  000h, 000h
loc_1_02F0:
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
loc_1_0398:
        pop    ebx
        db  0E0h, 0AEh
        add    eax, dword ptr [eax]
        db  000h, 0AEh, 003h, 000h, 000h, 003h
        add    al, 0
        db  000h, 058h, 004h
        add    byte ptr [eax], al
        jne    SHORT loc_1_03B0
        add    byte ptr [eax], al
loc_1_03AE:
        js     SHORT loc_1_03BE
loc_1_03B0:
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_03BE:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0403:
        js     SHORT loc_1_0413
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0413:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0458:
        dec    ebx
        db  0FFh, 0FFh, 0FFh, 0FFh, 002h, 0FFh, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 0FFh, 03Ch, 0FFh, 0FFh
        db  0FFh, 0FFh, 007h, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 000h
loc_1_0475:
        js     SHORT loc_1_0485
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0485:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_04CA:
        pop    ebx
        db  0E0h, 0E0h
        add    al, 0
        db  000h, 0E0h
        add    al, 0
        db  000h, 035h, 005h, 000h, 000h, 08Ah
        db  005h, 000h, 000h, 0A7h, 005h
        add    byte ptr [eax], al
loc_1_04E0:
        js     SHORT loc_1_04F0
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_04F0:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0535:
        js     SHORT loc_1_0545
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0545:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_058A:
        dec    ebx
        db  0FFh, 0FFh, 0FFh, 0FFh, 002h, 0FFh, 048h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 0FFh, 03Ch, 0FFh, 0FFh
        db  0FFh, 0FFh, 007h, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 0FFh, 000h
loc_1_05A7:
        js     SHORT loc_1_05B7
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_05B7:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_05FC:
        pop    ebx
        db  0E0h, 012h
        push   es
        add    byte ptr [eax], al
        adc    al, byte ptr [esi]
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        db  0BCh, 006h, 000h, 000h, 0BDh
        push   es
        add    byte ptr [eax], al
loc_1_0612:
        js     SHORT loc_1_0622
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0622:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0667:
        js     SHORT loc_1_0677
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0677:
        add    al, 0ffh
        add    al, 0ffh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_06BD:
        js     SHORT loc_1_06CD
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_06CD:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0712:
        pop    ebx
        db  0E0h, 028h
        pop    es
        add    byte ptr [eax], al
        sub    byte ptr [edi], al
        add    byte ptr [eax], al
        db  07Dh, 007h
        add    byte ptr [eax], al
        rol    byte ptr [edi], cl
        add    byte ptr [eax], al
        rol    dword ptr [edi], cl
        add    byte ptr [eax], al
loc_1_0728:
        js     SHORT loc_1_0738
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0738:
        add    al, 0ffh
        add    al, 0fh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Dh, 04Bh, 002h, 002h, 002h, 002h, 002h, 00Fh, 048h, 000h, 006h
        db  004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 004h, 03Ch, 008h, 000h, 006h, 000h, 007h, 000h, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_077D:
        js     SHORT loc_1_078D
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_078D:
        add    al, 0ffh
        add    al, 0ffh
        call   dword ptr [eax - 1]
        dec    dword ptr [4cffffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h
        db  004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 003h, 00Dh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_07D3:
        js     SHORT loc_1_07E3
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_07E3:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0828:
        pop    ebx
        db  0E0h, 06Bh
        or     dword ptr [eax], eax
        db  000h, 06Bh, 009h
        add    byte ptr [eax], al
        db  0B9h, 009h, 000h, 000h, 007h
        or     al, byte ptr [eax]
        db  000h, 008h
        or     al, byte ptr [eax]
        db  000h
loc_1_083E:
        pop    ebx
        db  0E0h, 06Ah
        or     byte ptr [eax], al
        db  000h, 06Ah, 008h
        add    byte ptr [eax], al
        db  0B8h, 008h, 000h, 000h, 006h
        or     dword ptr [eax], eax
        db  000h, 007h
        or     dword ptr [eax], eax
        add    byte ptr [ebx - 20h], bl
        push   8
        add    byte ptr [eax], al
        push   8
        add    byte ptr [eax], al
        db  0B8h, 008h, 000h, 000h, 006h
        or     dword ptr [eax], eax
        db  000h, 007h
        or     dword ptr [eax], eax
        add    byte ptr [eax + 0eh], bh
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 004h, 0FFh, 0FFh, 0FFh, 0FFh
        db  050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h
        db  002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h, 0FFh, 0FFh, 00Ah
        db  00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh
        db  0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_08B8:
        js     SHORT loc_1_08C8
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_08C8:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h, 000h
loc_1_0907:
        js     SHORT loc_1_0917
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0917:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0955:
        pop    ebx
        db  0E0h, 06Bh
        or     dword ptr [eax], eax
        db  000h, 06Bh, 009h
        add    byte ptr [eax], al
        db  0B9h, 009h, 000h, 000h, 007h
        or     al, byte ptr [eax]
        db  000h, 008h
        or     al, byte ptr [eax]
        db  000h
loc_1_096B:
        js     SHORT loc_1_097B
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_097B:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_09B9:
        js     SHORT loc_1_09C9
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_09C9:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0A08:
        js     SHORT loc_1_0A18
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0A18:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_0A56:
        pop    ebx
        db  0E0h, 06Ch
        or     al, byte ptr [eax]
        db  000h, 06Ch, 00Ah, 000h
        db  000h, 0BAh, 00Ah, 000h, 000h, 008h
        or     eax, dword ptr [eax]
        db  000h, 009h
        or     eax, dword ptr [eax]
        db  000h
loc_1_0A6C:
        js     SHORT loc_1_0A7C
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0A7C:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0ABA:
        js     SHORT loc_1_0ACA
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0ACA:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0B09:
        db  078h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 042h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h
loc_1_0B5E:
        pop    ebx
        db  0E0h, 074h
        or     eax, dword ptr [eax]
        db  000h, 074h, 00Bh, 000h
        db  000h, 0C2h
        or     eax, dword ptr [eax]
        db  000h, 010h
        or     al, 0
        db  000h, 010h
        or     al, 0
        db  000h
loc_1_0B74:
        js     SHORT loc_1_0B84
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0B84:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0BC2:
        js     SHORT loc_1_0BD2
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0BD2:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0C11:
        pop    ebx
        db  0E0h, 027h
        or     al, 0
        db  000h, 027h
        or     al, 0
        db  000h, 075h, 00Ch
        add    byte ptr [eax], al
        ret
        db  00Ch, 000h, 000h, 0C3h, 00Ch, 000h, 000h
loc_1_0C27:
        js     SHORT loc_1_0C37
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0C37:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0C75:
        js     SHORT loc_1_0C85
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0C85:
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
loc_1_0D19:
        pop    ebx
        db  0E0h, 02Fh
        db  00Dh, 000h, 000h, 02Fh, 00Dh
        add    byte ptr [eax], al
        db  07Dh, 00Dh
        add    byte ptr [eax], al
        retf
        db  00Dh, 000h, 000h, 0CBh, 00Dh, 000h, 000h
loc_1_0D2F:
        js     SHORT loc_1_0D3F
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0D3F:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0D7D:
        db  078h, 00Eh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0DCC:
        db  06Bh, 0E0h, 0E2h
        db  00Dh, 000h, 000h, 0E2h, 00Dh
        add    byte ptr [eax], al
        xor    byte ptr [esi], cl
        add    byte ptr [eax], al
        jle    SHORT loc_1_0DEA
        add    byte ptr [eax], al
        db  07Eh, 00Eh
        add    byte ptr [eax], al
loc_1_0DE2:
        js     SHORT loc_1_0DF2
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h
loc_1_0DEA:
        or     eax, 0ffff0e0dh
        call   ptr [edx + 4]
loc_1_0DF2:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 002h, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 004h, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 007h, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_0E30:
        js     SHORT loc_1_0E40
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0E40:
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
loc_1_0ED4:
        db  06Bh, 0E0h, 0EAh
        push   cs
        add    byte ptr [eax], al
        db  0EAh, 00Eh, 000h, 000h, 038h, 00Fh, 000h
        db  000h, 086h, 00Fh, 000h, 000h, 086h
        sldt   word ptr [eax]
loc_1_0EEA:
        js     SHORT loc_1_0EFA
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0EFA:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_0F38:
        js     SHORT loc_1_0F48
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0F48:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0F87:
        pop    ebx
        db  0E0h, 09Dh
        sldt   word ptr [eax]
        popfd
        sldt   word ptr [eax]
        in     al, 0fh
        add    byte ptr [eax], al
        sub    edx, dword ptr [eax]
        add    byte ptr [eax], al
        xor    edx, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_0F9D:
        js     SHORT loc_1_0FAD
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0FAD:
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
loc_1_0FE4:
        js     SHORT loc_1_0FF4
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_0FF4:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 002h, 0FFh
        db  0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_102B:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1033:
        db  078h, 00Eh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_107A:
        pop    ebx
        db  0E0h, 090h
        adc    byte ptr [eax], al
        db  000h, 090h, 010h, 000h, 000h, 0D7h
        adc    byte ptr [eax], al
        db  000h, 01Eh
        adc    dword ptr [eax], eax
        db  000h, 01Eh
        adc    dword ptr [eax], eax
        db  000h
loc_1_1090:
        js     SHORT loc_1_10A0
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_10A0:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 00Ah, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 006h, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_10D7:
        js     SHORT loc_1_10E7
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_10E7:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh
        db  0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch
        db  008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_111E:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1126:
        db  06Bh, 0E0h, 03Ch
        adc    dword ptr [eax], eax
        db  000h, 03Ch, 011h
        add    byte ptr [eax], al
        mov    dl, byte ptr [ecx]
        add    byte ptr [eax], al
        fcom   dword ptr [ecx]
        add    byte ptr [eax], al
        fst    dword ptr [ecx]
        add    byte ptr [eax], al
loc_1_113C:
        js     SHORT loc_1_114C
        push   cs
        db  0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_114C:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 000h, 0FFh, 000h
loc_1_118A:
        db  078h, 00Eh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 000h, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_11D9:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_11E1:
        db  06Bh, 0E0h, 0F7h
        adc    dword ptr [eax], eax
        db  000h, 0F7h
        adc    dword ptr [eax], eax
        db  000h, 045h, 012h
        add    byte ptr [eax], al
        xchg   ebx, eax
        adc    al, byte ptr [eax]
        db  000h, 09Bh, 012h, 000h, 000h
loc_1_11F7:
        js     SHORT loc_1_1207
        push   cs
        ud0
        db  0FFh, 0FFh, 064h, 00Dh, 00Dh, 00Eh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1207:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 00Dh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 002h, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 004h, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 00Ah, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 006h, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 000h
loc_1_1245:
        js     SHORT loc_1_1255
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 064h, 00Dh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 05Ah, 004h
loc_1_1255:
        add    al, 0ffh
        db  0FFh, 0FFh, 0FFh, 050h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 04Ch, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 04Bh, 002h, 002h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 000h, 006h, 0FFh, 0FFh, 0FFh, 0FFh, 046h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 03Ch, 008h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 038h, 0FFh, 008h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
loc_1_1293:
        cmp    al, 8
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_129C:
        db  03Bh, 01Dh, 0ECh, 013h, 000h, 000h
        jne    SHORT loc_1_12B5
        db  066h, 083h, 03Dh, 048h, 014h, 000h, 000h, 040h
        jne    SHORT loc_1_12B5
        cmp    dx, 4f15h
        je     SHORT loc_1_12C4
loc_1_12B5:
        db  083h, 03Dh, 03Bh, 014h, 000h, 000h, 000h
        je     SHORT loc_1_12EB
        db  0FFh, 025h, 03Bh, 014h, 000h, 000h
loc_1_12C4:
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
loc_1_12EB:
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1364:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1368:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1378:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1388:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13D2:
        add    byte ptr [eax], al
loc_1_13D4:
        add    byte ptr [eax], al
loc_1_13D6:
        add    byte ptr [eax], al
loc_1_13D8:
        add    byte ptr [eax], al
loc_1_13DA:
        add    byte ptr [eax], al
loc_1_13DC:
        add    bh, bh
loc_1_13DE:
        add    byte ptr [eax], al
loc_1_13E0:
        add    bh, bh
loc_1_13E2:
        add    byte ptr [eax], al
loc_1_13E4:
        add    byte ptr [eax], al
loc_1_13E6:
        add    byte ptr [eax], al
loc_1_13E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_143A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E1h
        adc    dword ptr [eax], eax
        add    byte ptr [ebx], bl
loc_1_1445:
        cmp    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end

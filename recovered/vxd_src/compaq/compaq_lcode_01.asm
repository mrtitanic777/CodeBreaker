; compaq_lcode_01.asm -- COMPAQ object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

COMPAQ_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_007D:
        mov    edx, 63cah
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 83c6h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 13c8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 13c9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        ret
sub_1_00AA:
        mov    edx, 63cah
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 83c6h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 13c8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 13c9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        ret
loc_1_00D7:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        jne    SHORT loc_1_0111
        mov    edi, ebx
        db  003h, 03Dh, 044h, 002h, 000h, 000h
        cmp    ecx, 4
        je     SHORT loc_1_0107
        cmp    ecx, 0ch
        je     SHORT loc_1_010A
        cmp    ecx, 8
        je     SHORT loc_1_0100
        in     al, dx
        jmp    NEAR PTR loc_1_01A1
loc_1_0100:
        in     ax, dx
        jmp    NEAR PTR loc_1_01A1
loc_1_0107:
        out    dx, al
        jmp    SHORT loc_1_0169
loc_1_010A:
        out    dx, ax
        jmp    NEAR PTR loc_1_0195
loc_1_0111:
        mov    edi, ebx
        db  003h, 03Dh, 044h, 002h, 000h, 000h
        cmp    ecx, 4
        je     SHORT loc_1_0169
        cmp    ecx, 0ch
        je     SHORT loc_1_0195
        cmp    ecx, 8
        je     SHORT loc_1_015B
        cmp    edx, 13c8h
        jne    SHORT loc_1_0135
        mov    al, byte ptr [edi + 2]
        jmp    SHORT loc_1_01A1
loc_1_0135:
        cmp    edx, 13c9h
        jne    SHORT loc_1_0142
        mov    al, byte ptr [edi + 3]
        jmp    SHORT loc_1_01A1
loc_1_0142:
        cmp    edx, 83c6h
        jne    SHORT loc_1_014F
        mov    al, byte ptr [edi + 6]
        jmp    SHORT loc_1_01A1
loc_1_014F:
        in     al, dx
        mov    ah, byte ptr [edi + 5]
        and    ax, 1fe0h
        or     al, ah
        jmp    SHORT loc_1_01A1
loc_1_015B:
        cmp    edx, 13c8h
        jne    SHORT loc_1_01A1
        mov    ax, word ptr [edi + 2]
        jmp    SHORT loc_1_01A1
loc_1_0169:
        cmp    edx, 13c8h
        jne    SHORT loc_1_0176
        mov    byte ptr [edi + 2], al
        jmp    SHORT loc_1_01A1
loc_1_0176:
        cmp    edx, 13c9h
        jne    SHORT loc_1_0183
        mov    byte ptr [edi + 3], al
        jmp    SHORT loc_1_01A1
loc_1_0183:
        cmp    edx, 83c6h
        jne    SHORT loc_1_0190
        mov    byte ptr [edi + 6], al
        jmp    SHORT loc_1_01A1
loc_1_0190:
        mov    byte ptr [edi + 5], al
        jmp    SHORT loc_1_01A1
loc_1_0195:
        cmp    edx, 13c8h
        jne    SHORT loc_1_01A1
        mov    word ptr [edi + 2], ax
loc_1_01A1:
        pop    esi
        pop    edi
        ret
loc_1_01A4:
        push   edi
        push   esi
        db  0F6h, 005h, 048h, 002h, 000h, 000h, 003h
        jne    SHORT loc_1_01C9
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        je     SHORT loc_1_01C9
        cmp    ecx, 4
        jne    SHORT loc_1_01C9
        push   eax
        push   edx
        mov    dh, 83h
        in     al, dx
        test   al, 80h
        pop    edx
        pop    eax
        jne    SHORT loc_1_01CF
loc_1_01C9:
        db  0CDh, 020h, 015h, 000h, 00Ah, 000h
loc_1_01CF:
        pop    esi
        pop    edi
        ret
        db  000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 043h, 04Fh, 04Dh, 050h
        db  041h, 051h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 081h, 000h, 000h, 000h
loc_1_02D0:
        inc    ebp
        inc    esp
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end

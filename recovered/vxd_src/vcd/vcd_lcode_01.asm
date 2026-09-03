; vcd_lcode_01.asm -- VCD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        or     ebx, ebx
        je     SHORT loc_1_0013
        db  0F6h, 005h, 020h, 004h, 000h, 000h, 001h
        je     SHORT loc_1_0013
        db  0CDh, 020h, 002h, 000h, 018h, 000h
loc_1_0013:
        ret
VCD_Control:
        cmp    eax, 0
        db  00Fh, 084h, 00Ah, 000h, 0FFh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 028h, 001h, 0FFh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 053h, 002h, 0FFh, 0FFh
        cmp    eax, 7
        db  00Fh, 084h, 0C4h, 005h, 0FFh, 0FFh
        cmp    eax, 0ch
        db  00Fh, 084h, 0E0h, 009h, 0FFh, 0FFh
        cmp    eax, 0fh
        db  00Fh, 084h, 065h, 004h, 0FFh, 0FFh
        cmp    eax, 0dh
        db  00Fh, 084h, 0BDh, 004h, 0FFh, 0FFh
        cmp    eax, 30h
        db  00Fh, 084h, 0E7h, 00Ah, 0FFh, 0FFh
        clc
        ret
sub_1_005E:
        pop    esi
        add    esi, 17h
        cmp    ecx, 4
        jbe    SHORT loc_1_0070
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0070:
        mov    edi, dword ptr [esi]
        add    edi, ebx
        or     byte ptr [edi + 6], 80h
        cmp    ebx, dword ptr [esi + 1ch]
        jne    SHORT loc_1_00BE
        test   byte ptr [esi + 6], 80h
        jne    SHORT loc_1_00A2
loc_1_0083:
        jecxz  loc_1_0087
        out    dx, al
        ret
loc_1_0087:
        mov    al, dl
        and    al, 7
        cmp    al, 2
        je     SHORT loc_1_0091
        in     al, dx
        ret
loc_1_0091:
        mov    al, 1
        xchg   byte ptr [esi + 3bh], al
        mov    ah, al
        and    ah, 7
        cmp    ah, 2
        je     SHORT loc_1_00A1
        in     al, dx
loc_1_00A1:
        ret
loc_1_00A2:
        test   word ptr [esi + 6], 141h
        jne    SHORT loc_1_0083
        push   edi
        mov    edi, edx
        and    edi, 7
        shl    edi, 3
        add    edi, dword ptr [esi + 3ch]
        mov    edi, dword ptr [ecx + edi + 18h]
        xchg   dword ptr [esp], edi
        ret
loc_1_00BE:
        push   edi
        mov    edi, edx
        and    edi, 7
        db  02Eh, 08Bh, 0BCh, 0F9h, 0D0h, 000h, 000h, 000h
        xchg   dword ptr [esp], edi
        ret
loc_1_00D0:
        or     byte ptr [ecx], al
        add    byte ptr [eax], al
        or     byte ptr [ecx], al
        add    byte ptr [eax], al
        push   ss
        add    dword ptr [eax], eax
        db  000h, 016h
        add    dword ptr [eax], eax
        db  000h, 02Ch, 001h
        add    byte ptr [eax], al
        das
        add    dword ptr [eax], eax
        db  000h, 034h, 001h
        add    byte ptr [eax], al
        cmp    byte ptr [ecx], al
        add    byte ptr [eax], al
        inc    eax
        add    dword ptr [eax], eax
        db  000h, 044h, 001h, 000h
        db  000h, 04Ch, 001h, 000h
        db  000h, 08Dh, 001h, 000h, 000h, 05Ah
        add    dword ptr [eax], eax
        db  000h, 08Dh, 001h, 000h, 000h, 0F6h
        inc    edi
        add    eax, dword ptr [eax + 3e35a74h]
        mov    byte ptr [edi], al
        ret
        db  08Ah, 007h, 0C3h
loc_1_0116:
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_1_0121
        lea    edi, [edi + 1]
        jmp    SHORT loc_1_0124
loc_1_0121:
        lea    edi, [edi + 2]
loc_1_0124:
        jecxz  loc_1_0129
        mov    byte ptr [edi], al
        ret
loc_1_0129:
        mov    al, byte ptr [edi]
        ret
loc_1_012C:
        mov    al, 1
        ret
loc_1_012F:
        or     al, al
        jne    SHORT loc_1_0168
        ret
loc_1_0134:
        mov    al, byte ptr [edi + 3]
        ret
loc_1_0138:
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 3], al
        ret
loc_1_0140:
        mov    al, byte ptr [edi + 4]
        ret
loc_1_0144:
        test   al, 18h
        jne    SHORT loc_1_0168
        mov    byte ptr [edi + 4], al
        ret
loc_1_014C:
        test   byte ptr [edi + 5], 1
        jne    SHORT loc_1_0168
        or     byte ptr [edi + 5], 1
        mov    al, byte ptr [esi + 39h]
        ret
loc_1_015A:
        test   byte ptr [edi + 5], 2
        jne    SHORT loc_1_0168
        or     byte ptr [edi + 5], 2
        mov    al, byte ptr [esi + 3ah]
        ret
loc_1_0168:
        pushad
        or     byte ptr [esi + 7], 4
        push   1
        push   1
        push   dword ptr [esi + 64h]
        db  0CDh, 020h, 064h, 000h, 033h, 000h
        add    esp, 0ch
        popad
        push   ecx
        call   sub_1_0190
        test   ecx, ecx
        pop    ecx
        je     NEAR PTR loc_1_0070
loc_1_018D:
        mov    al, 0ffh
        ret
sub_1_0190:
        db  0E8h, 0F0h, 008h, 0FFh, 0FFh
        cmp    ecx, 3
        jne    SHORT loc_1_01DD
        pushad
        mov    edi, dword ptr [esi]
        bts    word ptr [edi + ebx + 6], 1
        jb     SHORT loc_1_01DC
        db  0B9h, 027h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ecx + 4], eax
        db  068h, 027h, 000h, 000h, 000h
        push   0c0004h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        mov    eax, 0c0000030h
        xor    edi, edi
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
loc_1_01DC:
        popad
loc_1_01DD:
        ret
sub_1_01DE:
        pop    esi
        movzx  edx, dl
        or     edx, edx
        je     SHORT loc_1_01EA
        jmp    dword ptr [esi + edx + 40h]
loc_1_01EA:
        push   esi
        call   dword ptr [esi + edx + 40h]
        pop    esi
        jb     SHORT loc_1_01FA
        test   byte ptr [esi + 7], 4
        je     SHORT loc_1_0214
        stc
        ret
loc_1_01FA:
        test   byte ptr [esi + 7], 2
        stc
        je     SHORT loc_1_0214
        cmp    dword ptr [esi + 1ch], 1
        jb     SHORT loc_1_0214
        mov    eax, dword ptr [esi + 20h]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        bt     ecx, 3
loc_1_0214:
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_0218:
        mov    ecx, eax
        mov    edx, dword ptr [esi + 18h]
        add    edx, 2
        in     al, dx
        mov    byte ptr [esi + 3bh], al
        test   al, 1
        mov    eax, ecx
        stc
        jne    SHORT loc_1_0257
loc_1_022B:
        test   byte ptr [esi + 6], 40h
        jne    SHORT loc_1_029E
        mov    ebx, dword ptr [esi + 1ch]
        test   ebx, ebx
        je     SHORT loc_1_0265
        inc    ebx
        je     SHORT loc_1_0258
        dec    ebx
loc_1_023C:
        test   byte ptr [ebx], 8
        jne    SHORT loc_1_0258
        mov    ecx, eax
        mov    edx, dword ptr [esi + 18h]
        add    dl, 4
        in     al, dx
        test   al, 8
        mov    eax, ecx
        je     SHORT loc_1_0258
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_0256:
        clc
loc_1_0257:
        ret
loc_1_0258:
        call   sub_1_032C
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        jmp    SHORT loc_1_0256
loc_1_0265:
        mov    edx, dword ptr [esi + 18h]
        add    dl, 1
        in     al, dx
        or     al, al
        stc
        je     SHORT loc_1_0257
        jmp    SHORT loc_1_0273
loc_1_0273:
        add    dl, 3
        in     al, dx
        test   al, 8
        stc
        jne    SHORT loc_1_0257
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        bts    word ptr [esi + 6], 5
        jb     SHORT loc_1_023C
        mov    dword ptr [esi + 1ch], ebx
        mov    edx, esi
        db  0BEh, 09Ch, 008h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    esi, edx
        jmp    SHORT loc_1_023C
loc_1_029E:
        test   byte ptr [esi + 6], 10h
        je     SHORT loc_1_02BF
        push   esi
        db  0FFh, 015h, 01Ch, 004h, 000h, 000h
        pop    esi
        mov    eax, dword ptr [esi + 20h]
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi + 24h], eax
        stc
loc_1_02BF:
        cmc
        ret
loc_1_02C1:
        push   es
        push   ds
        push   eax
        mov    eax, ss
        mov    ds, eax
        mov    es, eax
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi + 24h], eax
        pop    eax
        pop    ds
        pop    es
        retf
        db  090h
loc_1_02D8:
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   cl, 80h
        je     SHORT loc_1_0322
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        mov    edx, dword ptr [esi + 18h]
        add    edx, 2
        in     al, dx
        mov    byte ptr [esi + 3bh], al
        test   al, 1
        je     SHORT loc_1_0308
        mov    eax, dword ptr [esi + 20h]
        jmp    SHORT loc_1_0302
loc_1_02FC:
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_1_0302:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_1_0308:
        db  0A1h, 0F0h, 003h, 000h, 000h
        db  0CDh, 020h, 037h, 000h, 001h, 000h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi + 24h], eax
        db  0FFh, 015h, 0D0h, 003h, 000h, 000h
loc_1_0322:
        ret
loc_1_0323:
        jecxz  loc_1_0326
        ret
loc_1_0326:
        call   sub_1_0190
        ret
sub_1_032C:
        push   eax
loc_1_032D:
        mov    edx, dword ptr [esi + 18h]
        add    dl, 2
        in     al, dx
        test   al, 1
        jne    SHORT loc_1_0349
        movzx  eax, al
        shr    eax, 1
        sub    dl, 2
        db  002h, 090h, 044h, 004h, 000h, 000h
        in     al, dx
        jmp    SHORT loc_1_032D
loc_1_0349:
        pop    eax
        ret
        db  000h, 083h, 001h, 000h, 000h, 08Ah, 001h, 000h, 000h, 0F5h, 001h, 000h, 000h, 0D1h, 003h, 000h
        db  000h, 01Bh, 002h, 000h, 000h, 0C6h, 002h, 000h, 000h, 01Eh, 003h, 000h, 000h, 062h, 004h, 000h
        db  000h, 08Ch, 004h, 000h, 000h, 010h, 003h, 000h, 000h, 0E8h, 001h, 000h, 000h, 06Bh, 000h, 000h
        db  000h, 041h, 001h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 00Eh, 000h, 003h, 00Ah, 000h, 000h, 056h, 043h, 044h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 030h, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Ch, 003h, 000h, 000h, 00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  013h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_1_03F4:
        movsb
        add    al, 0
        db  000h, 02Bh
        add    byte ptr [eax], al
        db  000h, 07Ah, 000h
        add    byte ptr [eax], al
        nop
        add    byte ptr [eax], al
        db  000h, 0A6h, 000h, 000h, 000h, 011h
        add    dword ptr [eax], eax
        db  000h, 021h
        add    dword ptr [eax], eax
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
loc_1_0414:
        sub    eax, dword ptr [edx]
        add    byte ptr [eax], al
loc_1_0418:
        cld
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 085h, 00Ah, 000h, 000h, 0F4h
        add    al, 0
        db  000h, 03Bh
        db  005h, 000h, 000h, 013h, 008h
        add    byte ptr [eax], al
loc_1_0438:
        pop    es
        pop    es
        add    byte ptr [eax], al
loc_1_043C:
        jmp    SHORT loc_1_0443
        db  000h, 000h, 0D0h, 005h, 000h
loc_1_0443:
        add    byte ptr [esi], al
        add    al, byte ptr [eax]
        db  005h
_LTEXT ends
        end

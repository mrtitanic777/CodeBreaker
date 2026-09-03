; unimodem_lcode_01.asm -- UNIMODEM object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

UNIMODEM_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
loc_1_000B:
        mov    dword ptr [edx + 4f4h], 0
        mov    eax, 1
        xchg   dword ptr [edx + 624h], eax
        or     eax, eax
        jne    SHORT loc_1_0053
        cmp    dword ptr [edx + 620h], 0
        jne    SHORT loc_1_004B
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0BEh, 092h, 000h, 000h, 000h
        mov    ecx, 0bh
        sub    eax, eax
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [edx + 620h], esi
loc_1_004B:
        xor    eax, eax
        mov    dword ptr [edx + 624h], eax
loc_1_0053:
        ret
loc_1_0054:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, 1
        xchg   dword ptr [edx + 624h], eax
        or     eax, eax
        jne    SHORT loc_1_009A
        cmp    dword ptr [edx + 620h], 0
        jne    SHORT loc_1_0092
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0BEh, 092h, 000h, 000h, 000h
        mov    ecx, 0bh
        sub    eax, eax
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [edx + 620h], esi
loc_1_0092:
        xor    eax, eax
        mov    dword ptr [edx + 624h], eax
loc_1_009A:
        pop    edi
        pop    esi
        leave
        ret
sub_1_009E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + 2e0h], 7
        je     SHORT loc_1_00B8
        cmp    dword ptr [edx + 2e0h], 0bh
        jne    SHORT loc_1_010E
loc_1_00B8:
        mov    eax, dword ptr [ebp + 0ch]
        or     dword ptr [edx + 5b2h], eax
        mov    eax, dword ptr [edx + 29ch]
        mov    eax, dword ptr [eax + 34h]
        or     dword ptr [edx + 60ch], eax
        mov    eax, 1
        xchg   dword ptr [edx + 624h], eax
        or     eax, eax
        jne    SHORT loc_1_010E
        cmp    dword ptr [edx + 620h], 0
        jne    SHORT loc_1_0106
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0BEh, 09Ah, 000h, 000h, 000h
        mov    ecx, 0bh
        sub    eax, eax
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [edx + 620h], esi
loc_1_0106:
        xor    eax, eax
        mov    dword ptr [edx + 624h], eax
loc_1_010E:
        pop    edi
        pop    esi
        leave
        ret
UNIMODEM_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 19h
        jae    SHORT loc_1_015E
        mov    cx, word ptr [ebp + 38h]
        or     cx, word ptr [ebp + 38h]
        je     SHORT loc_1_0133
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
loc_1_0133:
        db  08Bh, 034h, 085h, 0E8h, 005h, 000h, 000h
        db  081h, 0FEh, 041h, 073h, 000h, 000h
        jne    SHORT loc_1_014E
        push   eax
        push   edi
        call   esi
        add    esp, 8
        mov    word ptr [ebp + 1ch], ax
        ret
loc_1_014E:
        push   edi
        db  0FFh, 014h, 085h, 0E8h, 005h, 000h, 000h
        add    esp, 4
        mov    word ptr [ebp + 1ch], ax
        ret
loc_1_015E:
        mov    word ptr [ebp + 1ch], 2
        ret
        db  000h, 000h, 000h
loc_1_0168:
        mov    ecx, 14h
        jmp    NEAR PTR loc_1_035B
sub_1_0172:
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        cmp    edx, 4
        jne    SHORT loc_1_01DF
        mov    eax, dword ptr [ebp + 14h]
        test   dword ptr [esi + 5bah], eax
        je     SHORT loc_1_0197
        push   esi
        push   eax
        push   esi
        call   sub_1_009E
        add    esp, 8
        pop    esi
loc_1_0197:
        test   dword ptr [ebp + 14h], 4
        je     SHORT loc_1_01AA
        mov    dword ptr [esi + 5f6h], 1
loc_1_01AA:
        test   dword ptr [esi + 2c4h], 4
        je     SHORT loc_1_0226
        mov    eax, dword ptr [ebp + 14h]
        test   dword ptr [esi + 5b6h], eax
        je     SHORT loc_1_0226
        cmp    dword ptr [esi + 2bch], 0
        je     SHORT loc_1_0226
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [esi + 2c0h]
        push   esi
        call   dword ptr [esi + 2bch]
        jmp    SHORT loc_1_0223
loc_1_01DF:
        cmp    edx, 1
        jne    SHORT loc_1_0202
        cmp    dword ptr [esi + 2cch], 0
        je     SHORT loc_1_0226
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [esi + 2c8h]
        push   esi
        call   dword ptr [esi + 2cch]
        jmp    SHORT loc_1_0223
loc_1_0202:
        cmp    edx, 2
        jne    SHORT loc_1_0226
        cmp    dword ptr [esi + 2d8h], 0
        je     SHORT loc_1_0226
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [esi + 2d4h]
        push   esi
        call   dword ptr [esi + 2d8h]
loc_1_0223:
        add    esp, 10h
loc_1_0226:
        leave
        ret
loc_1_0228:
        push   dword ptr [esp + 8]
        db  0E8h, 0B6h, 032h, 0FFh, 0FFh
        add    esp, 4
        mov    ecx, 0
        jmp    NEAR PTR sub_1_033F
loc_1_023E:
        mov    ecx, 8
        jmp    NEAR PTR loc_1_035B
loc_1_0248:
        mov    ecx, 0ch
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_1_033F
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_026D
        push   esi
        mov    esi, dword ptr [esp + 8]
        inc    dword ptr [esi + 2b4h]
        pop    esi
loc_1_026D:
        ret
loc_1_026E:
        mov    ecx, 18h
        jmp    NEAR PTR sub_1_033F
loc_1_0278:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 0effffff1h
        jl     SHORT loc_1_02A0
        cmp    eax, 0effffff6h
        jg     SHORT loc_1_02A0
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 001h, 039h, 0FFh, 0FFh
        add    esp, 0ch
        pop    ebp
        ret
loc_1_02A0:
        cmp    eax, 0c0000000h
        jl     SHORT loc_1_02C4
        cmp    eax, 0c0000005h
        jg     SHORT loc_1_02C4
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 049h, 07Bh, 0FFh, 0FFh
        add    esp, 0ch
        pop    ebp
        ret
loc_1_02C4:
        mov    ecx, 24h
        jmp    NEAR PTR loc_1_035B
loc_1_02CE:
        mov    ecx, 30h
        jmp    SHORT sub_1_033F
loc_1_02D5:
        mov    ecx, 34h
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   sub_1_033F
        add    esp, 10h
        mov    ecx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0
        jle    SHORT loc_1_0309
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    dword ptr [esi + 2b4h], ecx
        pop    esi
loc_1_0309:
        ret
loc_1_030A:
        mov    ecx, 38h
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   sub_1_033F
        add    esp, 10h
        mov    ecx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0
        jle    SHORT loc_1_033E
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    dword ptr [esi + 2b0h], ecx
        pop    esi
loc_1_033E:
        ret
sub_1_033F:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 2e0h], 7
        je     SHORT loc_1_035B
        cmp    dword ptr [eax + 2e0h], 1ah
        je     SHORT loc_1_035B
        mov    eax, 0
        ret
loc_1_035B:
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 0ACh, 006h, 000h, 000h
        mov    eax, dword ptr [eax + 29ch]
        or     eax, eax
        je     SHORT loc_1_0380
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     SHORT loc_1_0380
        mov    ecx, dword ptr [ecx + eax]
        jecxz  loc_1_0380
        jmp    ecx
loc_1_0380:
        db  0A1h, 0ACh, 006h, 000h, 000h
        mov    dword ptr [eax + 0ch], 0fffffffah
        xor    eax, eax
        ret
        db  000h
loc_1_0390:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0ch], 0bh
        cmp    dword ptr [eax + 18h], 0
        jne    SHORT loc_1_03D9
        call   sub_1_04E8
        mov    dword ptr [ebp - 10h], eax
        pushad
        mov    ebx, dword ptr [ebp - 10h]
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 4]
        db  08Dh, 035h, 00Fh, 066h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [ebp - 8], esi
        popad
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
loc_1_03D9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_03E0:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_1_03FE
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_1_03FE:
        ret    10h
        db  000h, 000h, 000h
sub_1_0404:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  000h
sub_1_0418:
        db  0CDh, 020h, 00Eh, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_0420:
        db  0CDh, 020h, 00Fh, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_0428:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0430:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0438:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0440:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0448:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0450:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0458:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_0460:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_0468:
        db  0CDh, 020h, 00Ah, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_0470:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_0478:
        db  0CDh, 020h, 00Dh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_0480:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_0488:
        db  0CDh, 020h, 00Fh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_0490:
        db  0CDh, 020h, 010h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_0498:
        db  0CDh, 020h, 011h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_04A0:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_04A8:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_04B0:
        db  0CDh, 020h, 015h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_04B8:
        db  0CDh, 020h, 017h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_04C0:
        db  0CDh, 020h, 018h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_04C8:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_04D0:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_04D8:
        db  0CDh, 020h, 050h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_04E0:
        db  0CDh, 020h, 052h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_04E8:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
sub_1_04F4:
        pop    edx
        pop    ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  000h
sub_1_0504:
        pop    edx
        pop    eax
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        jmp    edx
        db  000h, 000h
sub_1_0510:
        pop    edx
        pop    eax
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        jmp    edx
        db  000h, 000h
sub_1_051C:
        pop    edx
        pop    eax
        pop    ecx
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        jmp    edx
        db  000h
sub_1_0528:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_1_0544:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_1_0554:
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        mov    eax, edx
        ret    4
        db  000h, 000h, 000h
sub_1_0568:
        db  0CDh, 020h, 0FAh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0570:
        db  0CDh, 020h, 027h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0578:
        db  0CDh, 020h, 003h, 080h, 09Dh, 004h
        add    byte ptr [eax], al
sub_1_0580:
        db  0CDh, 020h, 002h, 080h, 09Dh, 004h
        add    byte ptr [eax], al
sub_1_0588:
        db  0CDh, 020h, 001h, 080h, 09Dh, 004h
        add    byte ptr [eax], al
sub_1_0590:
        db  0CDh, 020h, 004h, 080h, 09Dh, 004h
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 055h, 04Eh, 049h, 04Dh
        db  04Fh, 044h, 045h, 04Dh, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  012h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 007h, 000h, 000h, 023h, 008h, 000h, 000h, 087h, 009h, 000h, 000h, 050h, 00Ah, 000h, 000h
        db  035h, 00Ch, 000h, 000h, 034h, 010h, 000h, 000h, 094h, 011h, 000h, 000h, 0BDh, 011h, 000h, 000h
        db  0B0h, 068h, 000h, 000h, 0D5h, 068h, 000h, 000h, 028h, 071h, 000h, 000h, 0DDh, 071h, 000h, 000h
        db  035h, 06Dh, 000h, 000h, 05Ch, 06Eh, 000h, 000h, 0FFh, 06Eh, 000h, 000h, 026h, 070h, 000h, 000h
        db  092h, 072h, 000h, 000h, 041h, 073h, 000h, 000h, 06Ch, 069h, 000h, 000h, 041h, 073h, 000h, 000h
        db  041h, 073h, 000h, 000h, 04Ch, 06Ah, 000h, 000h, 0B3h, 06Ch, 000h, 000h, 0FCh, 068h, 000h, 000h
        db  01Bh, 069h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0650:
        sub    byte ptr [edx], al
        add    byte ptr [eax], al
        pop    es
        add    al, 0
        db  000h, 03Eh
        add    al, byte ptr [eax]
        db  000h, 048h, 002h
        add    byte ptr [eax], al
        xor    al, 3
        add    byte ptr [eax], al
        db  068h, 001h, 000h, 000h, 06Eh
        add    al, byte ptr [eax]
        db  000h, 011h
        add    al, 0
        db  000h, 020h
        push   edx
        add    byte ptr [eax], al
        js     SHORT loc_1_0678
        add    byte ptr [eax], al
loc_1_0678:
        sub    dword ptr [eax + eax], eax
        db  000h, 033h
        add    al, 0
        db  000h, 0CEh
        add    al, byte ptr [eax]
        db  000h, 0D5h
        add    al, byte ptr [eax]
        db  000h, 00Ah
        add    eax, dword ptr [eax]
        db  000h, 05Eh, 004h
        add    byte ptr [eax], al
        db  0BBh, 004h, 000h, 000h, 040h
        db  005h, 000h, 000h, 0C5h, 005h
        add    byte ptr [eax], al
        sti
        push   ebx
        add    byte ptr [eax], al
        int3
        push   esp
        add    byte ptr [eax], al
        iretd
        db  005h, 000h, 000h, 0D9h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 037h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06C0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        db  0CDh, 020h, 020h, 000h, 02Bh, 000h
        pop    ecx
        pop    ebp
        ret
sub_1_06D3:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        db  0CDh, 020h, 01Fh, 000h, 02Bh, 000h
        pop    ecx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   0
        push   0
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_1_071B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        shr    eax, 10h
        mov    dword ptr [ebp - 0ch], eax
        call   sub_1_04E8
        mov    dword ptr [ebp - 8], eax
        push   ebx
        push   0
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        pop    ebx
        and    esi, 0ffffh
        add    esi, dword ptr [ebp - 4]
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_075F:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        ret    4
sub_1_076C:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], 0ffffffffh
        mov    dword ptr [ecx + 4], eax
        push   204c5242h
        call   sub_1_0570
        add    esp, 4
        ret    8
sub_1_078D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_04E8
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 8], eax
        test   ah, 2
        jne    SHORT loc_1_07BC
        int3
        mov    ecx, dword ptr [ebp - 8]
        or     ecx, 200h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2ch], ecx
loc_1_07BC:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_07D8
loc_1_07C4:
        push   1
        push   204c5242h
        call   sub_1_0568
        add    esp, 8
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_07C4
loc_1_07D8:
        mov    eax, dword ptr [esi + 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_07E4:
        push   ebp
        mov    ebp, esp
        push   ecx
        pushfd
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        popfd
        pop    ecx
        pop    ebp
        ret
sub_1_07F6:
        push   ebp
        mov    ebp, esp
        pushfd
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 014h, 000h, 02Ah, 000h
        popfd
        pop    ebp
        ret
        db  000h, 000h
loc_1_0808:
        dec    ebp
        outsd
        outsb
        imul   esi, dword ptr [edi + ebp*2 + 72h], 63696f56h
        push   esi
        imul   esp, dword ptr [ebp + 77h], 6e4fh
loc_1_081C:
        dec    ebp
        outsd
        outsb
        imul   esi, dword ptr [edi + ebp*2 + 72h], 63696f56h
        push   esi
        imul   esp, dword ptr [ebp + 77h], 66664fh
loc_1_0830:
        dec    ebp
        outsd
        outsb
        imul   esi, dword ptr [edi + ebp*2 + 72h], 616e4500h
        bound  ebp, qword ptr [ebp + 43h]
        popad
        insb
        insb
        jb     NEAR PTR loc_1_088E
        inc    esp
        add    byte ptr [eax], al
loc_1_0848:
        inc    ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 44h]
        imul   esi, dword ptr [ebx + 74h], 74636e69h
        imul   esi, dword ptr [esi + 65h], 676e6952h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], al
        db  073h, 077h
        jb     NEAR PTR loc_1_0867
loc_1_0867:
        add    byte ptr [ecx + 6eh], cl
        imul   esi, dword ptr [eax + eax], 6f4e0000h
        push   edx
        jae    NEAR PTR loc_1_08E6
        outsd
        outsb
        jae    SHORT loc_1_08DF
        add    byte ptr [eax], al
loc_1_087C:
        dec    esi
        outsd
        outsb
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_0884:
        sub    ebp, dword ptr [ebx]
        sub    eax, dword ptr [eax]
loc_1_0888:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
loc_1_088E:
        aaa
        cmp    byte ptr [ecx], bh
        popad
        bound  esp, qword ptr [ebx + 64h]
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_089C:
        dec    esi
        dec    edi
        inc    ebp
        dec    ebp
        push   ebx
        add    byte ptr [eax], al
        db  000h
loc_1_08A4:
        inc    edx
        insb
        imul   ebp, dword ptr [esi + 64h], 6e4f5fh
        add    byte ptr [eax], al
        db  000h
loc_1_08B0:
        inc    edx
        insb
        imul   ebp, dword ptr [esi + 64h], 66664f5fh
        add    byte ptr [eax], al
        add    byte ptr [ecx + ebp*2 + 61h], al
        insb
        push   eax
        db  072h, 065h
        imul   di, word ptr [eax], 4400h
        imul   esp, dword ptr [ecx + 6ch], 66667553h
        imul   edi, dword ptr [eax], 6e6f5400h
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_08DC:
        push   eax
        db  075h, 06Ch
loc_1_08DF:
        jae    SHORT loc_1_0946
        add    byte ptr [eax], al
        add    byte ptr [esi + 6fh], dl
loc_1_08E6:
        imul   esp, dword ptr [ebx + 65h], 61446f54h
        je     SHORT loc_1_0950
        inc    ecx
        outsb
        db  073h, 077h
        jb     NEAR PTR loc_1_08F6
loc_1_08F6:
        add    byte ptr [eax], al
loc_1_08F8:
        or     eax, 0d000000h
        add    byte ptr [eax], al
        add    byte ptr [0d000000h], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], bh
        db  072h, 03Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0910:
        cmp    al, 6ch
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_0918:
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_091C:
        xor    dword ptr [esi], ebp
        xor    eax, 3200h
        db  000h, 018h
        or     dword ptr [eax], eax
        db  000h, 01Ch, 009h
        add    byte ptr [eax], al
        and    byte ptr [ecx], cl
        add    byte ptr [eax], al
loc_1_0930:
        and    eax, 63252c64h
        sub    al, 25h
        sub    al, 25h
        jae    SHORT loc_1_0946
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0940:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0944:
        add    byte ptr [eax], al
loc_1_0946:
        add    byte ptr [eax], al
loc_1_0948:
        push   edx
        jae    NEAR PTR loc_1_09BC
        outsd
        outsb
        jae    SHORT loc_1_09B5
loc_1_0950:
        jae    SHORT loc_1_0952
loc_1_0952:
        add    byte ptr [eax], al
loc_1_0954:
        inc    ecx
        db  074h, 074h
        popad
        arpl   word ptr [eax + 65h], bp
        push   esp
        outsd
        add    byte ptr [eax], al
loc_1_0960:
        dec    edi
        db  076h, 065h
        db  072h, 072h
        imul   esp, dword ptr [ebp], 50000000h
        outsd
        jb     SHORT loc_1_09E4
        dec    esi
        popad
        insd
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_0978:
        push   eax
        db  072h, 06Fh
        jo     SHORT loc_1_09E2
        jb     SHORT loc_1_09F3
        imul   esp, dword ptr [ebp + 73h], 65440000h
        popaw
        jne    SHORT loc_1_09F6
        je     SHORT loc_1_098C
loc_1_098C:
        inc    esp
        inc    ebx
        inc    edx
        add    byte ptr [edi + ebp*2 + 67h], cl
        imul   ebp, dword ptr [bp + 67h], 676f4c00h
        imul   ebp, dword ptr [bp + 67h], 68746150h
        add    byte ptr [edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 044h
        jae    NEAR PTR loc_1_0A11
        add    byte ptr [eax], al
loc_1_09B0:
        dec    ecx
        outsb
        push   ax
        popad
loc_1_09B5:
        je     SHORT loc_1_0A1F
        db  000h
loc_1_09B8:
        dec    ecx
        outsb
        push   bx
loc_1_09BC:
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        add    byte ptr [eax], al
loc_1_09C4:
        dec    ebp
        popad
        outsb
        jne    SHORT loc_1_0A2F
        popad
        arpl   word ptr [ebp + esi*2 + 72h], si
        jb     NEAR PTR loc_1_09D1
loc_1_09D1:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 6fh], cl
        insb
        add    byte ptr [eax], al
        add    byte ptr [edx + 65h], dl
        jae    SHORT loc_1_0A45
        je     SHORT loc_1_09E2
loc_1_09E2:
        add    byte ptr [eax], al
loc_1_09E4:
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 54h], sp
        db  079h, 070h
        add    byte ptr gs:[eax], al
loc_1_09F0:
        dec    eax
        popad
        outsb
loc_1_09F3:
        jne    NEAR PTR loc_1_0A66
loc_1_09F6:
        add    byte ptr [eax], al
loc_1_09F8:
        dec    ecx
        outsb
        popad
        arpl   word ptr [ecx + ebp*2 + 76h], si
        imul   esi, dword ptr [ecx + edi*2 + 53h], 656c6163h
        add    byte ptr [esi + 61h], dl
        jb     SHORT loc_1_0A75
        popad
        bound  ebp, qword ptr [ebp + 54h]
loc_1_0A11:
        db  065h, 072h, 06Dh
        imul   ebp, dword ptr [esi + 61h], 726f74h
        add    byte ptr [ebx + 61h], al
        insb
loc_1_0A1F:
        insb
        jb     NEAR PTR loc_1_0A6C
        inc    esp
        push   eax
        jb     SHORT loc_1_0A90
        jbe    SHORT loc_1_0A8A
        je     SHORT loc_1_0A90
        add    byte ptr [ebx + 61h], al
        insb
loc_1_0A2F:
        insb
        db  065h, 072h, 049h
        inc    esp
        dec    edi
        jne    SHORT loc_1_0AAB
        db  073h, 069h
        add    byte ptr gs:[esi + 6fh], dl
        imul   esp, dword ptr [ebx + 65h], 666f7250h
loc_1_0A45:
        imul   ebp, dword ptr [ebp], 53000000h
        db  070h, 065h
        popad
        imul   esp, dword ptr [ebp + 72h], 50h
        push   53656e6fh
        db  070h, 065h
        arpl   word ptr [ebx], si
        add    byte ptr [eax], al
loc_1_0A60:
        dec    eax
        popad
        outsb
        jae    NEAR PTR loc_1_0ACB
loc_1_0A66:
        je     SHORT loc_1_0AAB
        insb
        outsd
        jae    SHORT loc_1_0AD1
loc_1_0A6C:
        inc    esp
        insb
        popad
        jns    SHORT loc_1_0A72
loc_1_0A72:
        add    byte ptr [eax], al
loc_1_0A74:
        push   esp
loc_1_0A75:
        db  065h, 072h, 06Dh
        imul   ebp, dword ptr [esi + 61h], 6c506574h
        popad
        jns    SHORT loc_1_0A82
loc_1_0A82:
        add    byte ptr [eax], al
loc_1_0A84:
        inc    ecx
        bound  ebp, qword ptr [edi + 72h]
        db  074h, 050h
loc_1_0A8A:
        insb
        popad
        jns    SHORT loc_1_0A8E
loc_1_0A8E:
        add    byte ptr [eax], al
loc_1_0A90:
        push   esp
        jb     NEAR PTR loc_1_0B01
        imul   ebp, dword ptr [esi + 61h], 65526574h
        arpl   word ptr [edi + 72h], bp
        add    byte ptr fs:[esp + ebp*2 + 65h], al
        push   esp
        popad
        bound  ebp, qword ptr [ebp]
        add    byte ptr [eax], al
loc_1_0AAB:
        add    byte ptr [ebx + 65h], dl
        db  074h, 074h
        imul   ebp, dword ptr [esi + 67h], 73h
        db  000h
loc_1_0AB8:
        push   ebx
        je     NEAR PTR loc_1_0B30
        imul   ebp, dword ptr [esi + 67h], 6e495c73h
        imul   esi, dword ptr [eax + eax], 65725000h
loc_1_0ACB:
        imul   di, word ptr [eax], 5400h
loc_1_0AD1:
        jb     NEAR PTR loc_1_0B41
        imul   ebp, dword ptr [esi + 61h], 726f74h
        add    byte ptr [ebx + 61h], al
        insb
        insb
        push   ebx
        je     NEAR PTR loc_1_0B59
        db  070h, 046h
        popad
        imul   ebp, dword ptr [esp + edx*2 + 69h], 72656dh
        add    byte ptr [ecx + 6eh], cl
        popad
        arpl   word ptr [ecx + ebp*2 + 76h], si
        imul   esi, dword ptr [ecx + edi*2 + 54h], 6f656d69h
        db  075h, 074h
loc_1_0B01:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 70h], dl
        popad
        imul   esp, dword ptr [ebp + 72h], 56h
        outsd
        insb
        db  075h, 06Dh
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 70h], dl
        popad
        imul   esp, dword ptr [ebp + 72h], 4dh
        outsd
        add    byte ptr gs:[esi + 6ch], al
        outsd
        ja     SHORT loc_1_0B68
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        add    byte ptr [ebp + 72h], al
        db  072h, 06Fh
loc_1_0B30:
        db  072h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0B3C:
        inc    ebx
        outsd
        insd
        db  070h, 072h
loc_1_0B41:
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 646f4d00h
        jne    SHORT loc_1_0BB9
        popad
        je     SHORT loc_1_0BB9
        outsd
        outsb
        add    byte ptr [eax], al
loc_1_0B54:
        pop    edi
        inc    ebx
        inc    ebx
        dec    ecx
        push   esp
loc_1_0B59:
        push   esp
        add    byte ptr [eax], al
loc_1_0B5C:
        pop    edi
        inc    edx
        insb
        insb
        add    byte ptr [eax], al
        add    byte ptr [edi + 43h], bl
        inc    ebx
        dec    ecx
loc_1_0B68:
        push   esp
        push   esp
        pop    edi
        push   esi
        xor    dh, byte ptr [ebx]
        add    byte ptr [eax], al
loc_1_0B70:
        push   ebx
        jo     SHORT loc_1_0BD8
        dec    esi
        outsd
        db  074h, 069h
        popad
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0B84:
        pop    edi
        dec    esp
        outsd
        ja     SHORT loc_1_0B89
loc_1_0B89:
        add    byte ptr [eax], al
        add    byte ptr [edi + 4dh], bl
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_1_0B94:
        pop    edi
        dec    eax
        imul   esp, dword ptr [edi + 68h], 5f000000h
        inc    esp
        imul   esp, dword ptr [ecx + 6ch], 5f000000h
        push   ebx
        db  065h, 074h, 075h
        jo     SHORT loc_1_0BAB
loc_1_0BAB:
        add    byte ptr [edi + 46h], bl
        outsd
        jb     SHORT loc_1_0C14
        add    byte ptr fs:[edi + 43h], bl
        insb
        insb
loc_1_0BB9:
        db  075h, 06Ch
        popad
        jb     SHORT loc_1_0BBE
loc_1_0BBE:
        add    byte ptr [eax], al
loc_1_0BC0:
        pop    edi
        dec    eax
        popad
        db  072h, 064h
        add    byte ptr [eax], al
        add    byte ptr [edi + 53h], bl
        outsd
        je     NEAR PTR loc_1_0BCE
loc_1_0BCE:
        add    byte ptr [eax], al
loc_1_0BD0:
        pop    edi
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD8:
        pop    edi
        dec    edi
        outsb
        db  000h
loc_1_0BDC:
        push   ebp
        jae    SHORT loc_1_0C44
        db  072h, 049h
        outsb
        imul   esi, dword ptr [eax + eax], 233c0000h
        db  03Eh, 000h
loc_1_0BEC:
        dec    esp
        dec    edi
        inc    edi
        inc    edi
        inc    ebp
        push   edx
        push   esi
        pop    eax
        inc    esp
        add    byte ptr [eax], al
        dec    esp
        dec    edi
        inc    edi
        inc    edi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
loc_1_0C00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C08:
        push   ebx
        je     SHORT loc_1_0C6C
        jb     SHORT loc_1_0C81
        push   eax
        insb
        popad
        jns    SHORT loc_1_0C12
loc_1_0C12:
        add    byte ptr [eax], al
loc_1_0C14:
        push   ebx
        je     SHORT loc_1_0C86
        db  070h, 050h
        insb
        popad
        jns    SHORT loc_1_0C1D
loc_1_0C1D:
        add    byte ptr [eax], al
        db  000h
loc_1_0C20:
        push   ebx
        db  074h, 061h
        db  072h, 074h
        push   edx
        arpl   word ptr gs:[edi + 72h], bp
        db  064h, 000h
loc_1_0C2C:
        push   ebx
        db  074h, 06Fh
        jo     SHORT loc_1_0C83
        arpl   word ptr gs:[edi + 72h], bp
        add    byte ptr fs:[eax], al
loc_1_0C38:
        push   esi
        outsd
        imul   esp, dword ptr [ebx + 65h], 6c616944h
        dec    esi
        db  075h, 06Dh
loc_1_0C44:
        bound  esp, qword ptr [ebp + 72h]
        push   ebx
        je     NEAR PTR loc_1_0CC0
        jo     SHORT loc_1_0C4D
loc_1_0C4D:
        add    byte ptr [eax], al
        add    byte ptr [esi + 6fh], dl
        imul   esp, dword ptr [ebx + 65h], 77736e41h
        jb     NEAR PTR loc_1_0C5C
loc_1_0C5C:
        add    byte ptr [ebx], cl
        or     eax, 10050110h
        add    eax, dword ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [edi + 70h], cl
        outsb
loc_1_0C6C:
        dec    eax
        popad
        outsb
        jae    NEAR PTR loc_1_0CD7
        je     SHORT loc_1_0C74
loc_1_0C74:
        inc    ebx
        insb
        outsd
        db  073h, 065h
        dec    eax
        popad
        outsb
        jae    NEAR PTR loc_1_0CE4
        je     SHORT loc_1_0C81
loc_1_0C81:
        add    byte ptr [eax], al
loc_1_0C83:
        add    byte ptr [ebx + 70h], dl
loc_1_0C86:
        popad
        imul   esp, dword ptr [ebp + 72h], 50h
        push   45656e6fh
        outsb
        popad
        bound  ebp, qword ptr [ebp]
        add    byte ptr [ebx + 70h], dl
        popad
        imul   esp, dword ptr [ebp + 72h], 50h
        push   44656e6fh
        imul   esi, dword ptr [ebx + 61h], 656c62h
loc_1_0CAC:
        push   ebx
        db  070h, 065h
        popad
        imul   esp, dword ptr [ebp + 72h], 50h
        push   4d656e6fh
        db  075h, 074h
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_0CC0:
        push   ebx
        db  070h, 065h
        popad
        imul   esp, dword ptr [ebp + 72h], 50h
        push   55656e6fh
        outsb
        dec    ebp
        db  075h, 074h
        add    byte ptr gs:[eax], al
loc_1_0CD4:
        push   ebx
        db  070h, 065h
loc_1_0CD7:
        popad
        imul   esp, dword ptr [ebp + 72h], 50h
        push   53656e6fh
        je     NEAR PTR loc_1_0D3A
loc_1_0CE4:
        outsd
        insb
        db  075h, 06Dh
        inc    edi
        popad
        imul   ebp, dword ptr [esi], 563c0000h
        outsd
        insb
        add    byte ptr ds:[eax], al
        db  000h
loc_1_0CF8:
        cmp    al, 47h
        popad
        imul   ebp, dword ptr [esi + 3eh], 65470000h
        outsb
        db  065h, 072h, 061h
        db  074h, 065h
        inc    esp
        imul   esp, dword ptr [edi + 69h], 74h
loc_1_0D10:
        cmp    al, 44h
        imul   esp, dword ptr [edi + 69h], 0ff003e74h
        db  0FFh, 0FFh, 020h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh
loc_1_0D3A:
        dec    dword ptr [edi]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Eh, 0FFh, 0FFh, 0FFh, 0FFh, 010h, 000h, 001h, 002h, 003h
        db  004h, 005h, 006h, 007h, 008h, 009h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Ah, 00Bh, 00Ch
        db  00Dh, 00Eh, 00Fh, 0FFh, 021h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 027h, 0FFh, 0FFh, 0FFh, 023h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 025h, 026h, 027h
        db  028h, 02Bh, 02Ch, 0FFh, 022h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 02Ah, 024h, 029h
        db  021h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 011h
_LTEXT ends
        end

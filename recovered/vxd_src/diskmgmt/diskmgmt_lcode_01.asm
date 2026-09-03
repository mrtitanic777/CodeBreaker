; diskmgmt_lcode_01.asm -- DISKMGMT object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

DISKMGMT_Control:
        cmp    eax, 2
        jne    SHORT loc_1_000E
        db  0E8h, 0F6h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        ret
loc_1_000E:
        cmp    eax, 1bh
        jne    SHORT loc_1_001C
        db  0E8h, 0E8h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        ret
loc_1_001C:
        cmp    eax, 1ch
        jne    SHORT loc_1_002A
        call   sub_1_0377
        cmp    eax, 1
        ret
loc_1_002A:
        cmp    eax, 23h
        jne    SHORT loc_1_003C
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_1_0070
        cmp    eax, 1
        ret
loc_1_003C:
        clc
        ret
loc_1_003E:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 2
        je     SHORT loc_1_0057
        mov    word ptr [ebx + 2], 0ffffh
loc_1_0057:
        ret
sub_1_0058:
        db  0CDh, 020h, 007h, 080h, 010h, 000h
sub_1_005E:
        push   esi
        db  08Bh, 035h, 0B8h, 003h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 010h, 000h
        pop    esi
        ret
        db  000h, 000h, 000h
sub_1_0070:
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        jne    SHORT loc_1_007C
        sub    eax, eax
        jmp    SHORT loc_1_00A5
loc_1_007C:
        cmp    ecx, -1
        jne    SHORT loc_1_0088
        call   sub_1_0382
        jmp    SHORT loc_1_00A5
loc_1_0088:
        cmp    ecx, 5
        mov    eax, 32h
        ja     SHORT loc_1_00A5
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        db  0FFh, 014h, 08Dh, 020h, 004h, 000h, 000h
loc_1_00A5:
        ret    10h
loc_1_00A8:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        cmp    dword ptr [eax + 1ch], 4
        jae    SHORT loc_1_00BC
        mov    eax, 0eah
        jmp    SHORT loc_1_00CD
loc_1_00BC:
        or     ecx, ecx
        mov    eax, 57h
        je     SHORT loc_1_00CD
        mov    dword ptr [ecx], 1000h
        sub    eax, eax
loc_1_00CD:
        ret    0ch
loc_1_00D0:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [eax + 18h]
        db  0A1h, 0BCh, 003h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        or     eax, eax
        mov    dword ptr [edx], 0
        je     SHORT loc_1_0100
        mov    ecx, 2000h
loc_1_00EE:
        test   dword ptr [eax + 20h], ecx
        jne    SHORT loc_1_00F9
        cmp    dword ptr [eax], eax
        jne    SHORT loc_1_00F9
        inc    dword ptr [edx]
loc_1_00F9:
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        jne    SHORT loc_1_00EE
loc_1_0100:
        sub    eax, eax
        ret    0ch
loc_1_0105:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 1ch]
        mov    esi, dword ptr [eax + 18h]
        mov    dword ptr [esp + 0ch], 0
        cmp    dword ptr [eax + 1ch], 121h
        jae    SHORT loc_1_012D
        mov    eax, 0eah
        jmp    NEAR PTR loc_1_0256
loc_1_012D:
        or     esi, esi
        jne    SHORT loc_1_013B
        mov    eax, 57h
        jmp    NEAR PTR loc_1_0256
loc_1_013B:
        push   0
        lea    eax, [esi + 4]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_0388
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0157
        jmp    NEAR PTR loc_1_0251
loc_1_0157:
        db  0A1h, 0BCh, 003h, 000h, 000h
        sub    dx, dx
        mov    ebx, dword ptr [eax + 30h]
        or     ebx, ebx
        je     NEAR PTR loc_1_0251
        mov    eax, 2000h
loc_1_016F:
        test   dword ptr [ebx + 20h], eax
        jne    SHORT loc_1_0183
        cmp    dword ptr [ebx], ebx
        jne    SHORT loc_1_0183
        inc    dx
        mov    ecx, dword ptr [esp + 0ch]
        cmp    dword ptr [ebx + 70h], ecx
        je     SHORT loc_1_018A
loc_1_0183:
        mov    ebx, dword ptr [ebx + 0ch]
        or     ebx, ebx
        jne    SHORT loc_1_016F
loc_1_018A:
        or     ebx, ebx
        je     NEAR PTR loc_1_0251
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 48h
        rep stosd
        stosb
        mov    dword ptr [esi], 4d4f4547h
        dec    dl
        mov    byte ptr [esi + 104h], dl
        mov    al, byte ptr [ebx + 74h]
        inc    al
        mov    byte ptr [esi + 120h], al
        mov    al, byte ptr [ebx + 40h]
        mov    byte ptr [esi + 11fh], al
        mov    ecx, dword ptr [ebx + 20h]
        test   ecx, 80000h
        jne    SHORT loc_1_0211
        mov    eax, dword ptr [ebx + 54h]
        mov    dword ptr [esi + 107h], eax
        mov    eax, dword ptr [ebx + 58h]
        mov    dword ptr [esi + 10bh], eax
        mov    eax, dword ptr [ebx + 60h]
        mov    dword ptr [esi + 113h], eax
        mov    eax, dword ptr [ebx + 64h]
        mov    dword ptr [esi + 117h], eax
        mov    eax, dword ptr [ebx + 68h]
        mov    dword ptr [esi + 11bh], eax
        mov    eax, dword ptr [ebx + 5ch]
        or     eax, eax
        mov    dword ptr [esi + 10fh], eax
        jne    SHORT loc_1_0211
        mov    eax, dword ptr [ebx + 0dbh]
        mov    dword ptr [esi + 10fh], eax
loc_1_0211:
        mov    edx, dword ptr [ebx + 10h]
        sub    ax, ax
        or     edx, edx
        je     SHORT loc_1_0224
loc_1_021B:
        mov    edx, dword ptr [edx + 10h]
        inc    ax
        or     edx, edx
        jne    SHORT loc_1_021B
loc_1_0224:
        or     ax, ax
        mov    byte ptr [esi + 106h], al
        jne    SHORT loc_1_0236
        mov    byte ptr [esi + 106h], 1
loc_1_0236:
        test   cl, 4
        je     SHORT loc_1_0246
        mov    al, byte ptr [ebx + 14h]
        mov    byte ptr [esi + 105h], al
        jmp    SHORT loc_1_024D
loc_1_0246:
        mov    byte ptr [esi + 105h], 0ffh
loc_1_024D:
        sub    eax, eax
        jmp    SHORT loc_1_0256
loc_1_0251:
        mov    eax, 3f4h
loc_1_0256:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
loc_1_025F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        mov    edx, dword ptr [eax + 18h]
        cmp    dword ptr [eax + 1ch], 1
        jae    SHORT loc_1_027C
        mov    eax, 0eah
        jmp    SHORT loc_1_02E2
loc_1_027C:
        or     edx, edx
        jne    SHORT loc_1_0287
        mov    eax, 57h
        jmp    SHORT loc_1_02E2
loc_1_0287:
        db  0A1h, 0BCh, 003h, 000h, 000h
        mov    esi, dword ptr [eax + 30h]
        mov    eax, esi
        or     esi, esi
        je     SHORT loc_1_02A9
        mov    cl, byte ptr [ecx]
loc_1_0297:
        cmp    byte ptr [eax + 14h], cl
        je     SHORT loc_1_02A5
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        jne    SHORT loc_1_0297
        jmp    SHORT loc_1_02A9
loc_1_02A5:
        mov    edi, dword ptr [eax]
        jmp    SHORT loc_1_02AD
loc_1_02A9:
        mov    edi, dword ptr [esp + 0ch]
loc_1_02AD:
        or     eax, eax
        je     SHORT loc_1_02DD
        sub    bx, bx
        or     esi, esi
        je     SHORT loc_1_02D9
        mov    ecx, 2000h
loc_1_02BD:
        mov    eax, dword ptr [esi]
        cmp    eax, edi
        je     SHORT loc_1_02D7
        test   dword ptr [esi + 20h], ecx
        jne    SHORT loc_1_02CE
        cmp    esi, eax
        jne    SHORT loc_1_02CE
        inc    bx
loc_1_02CE:
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_1_02BD
        jmp    SHORT loc_1_02D9
loc_1_02D7:
        mov    byte ptr [edx], bl
loc_1_02D9:
        sub    eax, eax
        jmp    SHORT loc_1_02E2
loc_1_02DD:
        mov    eax, 3f4h
loc_1_02E2:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
loc_1_02EB:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        db  0A1h, 0BCh, 003h, 000h, 000h
        mov    edx, dword ptr [esi + 10h]
        mov    edi, dword ptr [esi + 18h]
        mov    ebx, dword ptr [eax + 30h]
        sub    ax, ax
        or     ebx, ebx
        je     SHORT loc_1_0368
loc_1_030B:
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_1_0321
        cmp    dword ptr [ebx], ebx
        jne    SHORT loc_1_0321
        movzx  ecx, byte ptr [edx]
        movzx  ebp, ax
        cmp    ecx, ebp
        je     SHORT loc_1_0328
        inc    ax
loc_1_0321:
        mov    ebx, dword ptr [ebx + 0ch]
        or     ebx, ebx
        jne    SHORT loc_1_030B
loc_1_0328:
        or     ebx, ebx
        je     SHORT loc_1_0368
        push   0
        lea    eax, [esp + 14h]
        push   dword ptr [ebx + 70h]
        push   eax
        call   sub_1_0390
        inc    dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        cmp    dword ptr [esi + 1ch], eax
        jae    SHORT loc_1_0352
        mov    eax, 0eah
        jmp    SHORT loc_1_036D
loc_1_0352:
        push   0
        push   dword ptr [esp + 14h]
        push   edi
        push   dword ptr [ebx + 70h]
        call   sub_1_0398
        add    esp, 10h
        sub    eax, eax
        jmp    SHORT loc_1_036D
loc_1_0368:
        mov    eax, 3f4h
loc_1_036D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
sub_1_0377:
        call   sub_1_005E
        mov    eax, 1
        ret
sub_1_0382:
        mov    eax, 1
        ret
sub_1_0388:
        db  0CDh, 020h, 002h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_1_0390:
        db  0CDh, 020h, 006h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_1_0398:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        add    byte ptr [eax], al
loc_1_03A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 044h, 049h, 053h, 04Bh
        db  04Dh, 047h, 04Dh, 054h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 014h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  0A8h, 000h, 000h, 000h, 0D0h, 000h, 000h, 000h, 005h, 001h, 000h, 000h, 05Fh, 002h, 000h, 000h
        db  0EBh, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end

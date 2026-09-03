; vkd_pcode_06.asm -- VKD object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VKD_Define_Paste_Mode:
        push   ebx
        db  003h, 01Dh, 010h, 012h, 000h, 000h
        and    byte ptr [ebx + 1], 7fh
        or     al, al
        je     SHORT loc_6_0013
        or     byte ptr [ebx + 1], 80h
loc_6_0013:
        pop    ebx
        ret
loc_6_0015:
        pop    ecx
        popad
loc_6_0017:
        stc
        ret
VKD_Start_Paste:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_6_0017
        pushad
        mov    edi, ebx
        db  003h, 03Dh, 010h, 012h, 000h, 000h
        mov    dword ptr [edi + 1ch], esi
        mov    dword ptr [edi + 20h], edx
        shl    ecx, 2
        push   ecx
        add    ecx, 0fffh
        shr    ecx, 0ch
        xor    eax, eax
        push   80h
        push   eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_6_0015
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 14h], edx
        pop    ecx
        mov    eax, edx
        add    eax, ecx
        mov    dword ptr [edi + 18h], eax
        xchg   edx, edi
        mov    esi, dword ptr [esp + 1ch]
        cld
        shr    ecx, 2
        rep movsd
        xchg   edx, edi
        test   byte ptr [edi + 1], 80h
        jne    SHORT loc_6_00A1
        or     byte ptr [edi + 1], 4
        or     byte ptr [edi + 1], 2
        mov    eax, dword ptr [edi + 53h]
        mov    edx, ebx
        db  0BEh, 0C1h, 000h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        mov    dword ptr [edi + 24h], esi
        mov    byte ptr [edi + 38h], 1
        db  0E8h, 06Bh, 003h, 0FAh, 0FFh
        jmp    SHORT loc_6_00B5
loc_6_00A1:
        or     byte ptr [edi + 1], 8
        mov    eax, dword ptr [edi + 4bh]
        mov    edx, edi
        db  0BEh, 0B0h, 001h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
loc_6_00B5:
        popad
        clc
        ret
VKD_Cancel_Paste:
        xor    eax, eax
        inc    eax
        db  0E8h, 07Dh, 0FFh, 0F9h, 0FFh
        ret
sub_6_00C1:
        mov    ebx, edx
        db  003h, 015h, 010h, 012h, 000h, 000h
        mov    dword ptr [edx + 24h], 0
        mov    eax, dword ptr [edx]
        and    ah, 0f9h
        or     ah, 8
        mov    dword ptr [edx], eax
        db  0E9h, 0D1h, 000h, 0FAh, 0FFh
        db  000h
loc_6_00E0:
        pushad
        xor    ecx, ecx
        mov    eax, 1388h
        db  0CDh, 020h, 0ECh, 000h, 001h, 000h
        popad
        db  0E9h, 065h, 00Ah, 0FAh, 0FFh
sub_6_00F4:
        push   eax
        push   ebx
        push   ecx
        test   edx, edx
        je     SHORT loc_6_0100
        cmp    edx, 0ch
        jne    SHORT loc_6_0131
loc_6_0100:
        db  087h, 01Dh, 0B4h, 014h, 000h, 000h
        db  0A1h, 0FEh, 014h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 80h
        je     SHORT loc_6_0131
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  08Bh, 01Dh, 0B4h, 014h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        db  089h, 01Dh, 004h, 015h, 000h, 000h
loc_6_0131:
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_6_0135:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 0c2h
        je     SHORT loc_6_014F
        db  08Bh, 00Dh, 0ACh, 014h, 000h, 000h
        db  08Bh, 015h, 0B0h, 014h, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_6_014F:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        jmp    SHORT loc_6_0163
        db  090h
loc_6_0158:
        mov    ax, word ptr [ebp + 1ch]
        cmp    ah, 0c2h
        je     SHORT loc_6_0163
loc_6_0161:
        stc
        ret
loc_6_0163:
        db  080h, 03Dh, 013h, 015h, 000h, 000h, 0FFh
        je     SHORT loc_6_0161
        db  08Bh, 035h, 00Ch, 015h, 000h, 000h
        add    esi, ebx
        movzx  eax, al
        cmp    eax, 7
        ja     SHORT loc_6_018C
        db  080h, 03Dh, 011h, 015h, 000h, 000h, 0FFh
        jne    SHORT loc_6_01B8
        db  0FFh, 024h, 085h, 018h, 015h, 000h, 000h
loc_6_018C:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1dh], 1
        clc
        ret
loc_6_0197:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1dh], 2
        clc
        ret
loc_6_01A2:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1dh], 5
        clc
        ret
loc_6_01AD:
        and    word ptr [ebp + 2ch], 0fffeh
        mov    byte ptr [ebp + 1dh], 0
        clc
        ret
loc_6_01B8:
        cmp    eax, 7
        je     SHORT loc_6_01AD
        db  08Ah, 080h, 038h, 015h, 000h, 000h
        mov    byte ptr [ebp + 1dh], al
        or     word ptr [ebp + 2ch], 1
        clc
        ret
loc_6_01CD:
        cmp    byte ptr [ebp + 11h], 1
        ja     SHORT loc_6_0197
        jb     SHORT loc_6_01E0
        cmp    dword ptr [esi], 0
        je     SHORT loc_6_01A2
        mov    byte ptr [esi + 5], 0ffh
        jmp    SHORT loc_6_01AD
loc_6_01E0:
        mov    byte ptr [esi + 5], 0
        jmp    SHORT loc_6_01AD
loc_6_01E6:
        mov    byte ptr [ebp + 11h], 0
        mov    byte ptr [esi + 6], 0
loc_6_01EE:
        mov    dword ptr [esi], 0
        mov    byte ptr [esi + 5], 0
        jmp    SHORT loc_6_01AD
loc_6_01FA:
        cmp    byte ptr [ebp + 11h], 0
        jne    SHORT loc_6_01AD
        mov    byte ptr [esi + 6], 0ffh
        jmp    SHORT loc_6_01AD
loc_6_0206:
        mov    byte ptr [ebp + 11h], 0
        cmp    byte ptr [esi + 6], 0ffh
        jne    SHORT loc_6_01AD
        db  066h, 0A1h, 008h, 015h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        jmp    SHORT loc_6_01AD
loc_6_021C:
        mov    cl, byte ptr [ebp + 11h]
        cmp    cl, 2
        ja     NEAR PTR loc_6_0197
        je     SHORT loc_6_01AD
        test   cl, cl
        jne    NEAR PTR loc_6_01AD
        cmp    byte ptr [esi + 5], 0ffh
        jne    SHORT loc_6_023B
        or     cl, 20h
loc_6_023B:
        db  08Ah, 015h, 010h, 015h, 000h, 000h
        add    dl, 2
        and    dl, 5
        or     cl, dl
        mov    byte ptr [ebp + 10h], cl
        mov    byte ptr [ebp + 18h], 3
        mov    byte ptr [ebp + 14h], 28h
        jmp    NEAR PTR loc_6_01AD
loc_6_0259:
        mov    cx, word ptr [ebp + 38h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 10h]
        mov    dword ptr [esi], ecx
        mov    byte ptr [esi + 4], 0
        test   dword ptr [ebx], 20h
        je     NEAR PTR loc_6_01AD
        mov    byte ptr [esi + 4], 0ffh
        jmp    NEAR PTR loc_6_01AD
        db  090h
sub_6_0280:
        db  03Bh, 01Dh, 004h, 015h, 000h, 000h
        jne    SHORT loc_6_0292
        db  066h, 083h, 03Dh, 0FCh, 014h, 000h, 000h, 000h
        ja     SHORT loc_6_0294
loc_6_0292:
        clc
        ret
loc_6_0294:
        push   eax
        db  0A1h, 0FEh, 014h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        test   cl, 2
        je     SHORT loc_6_02BF
        db  0A1h, 00Ch, 012h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        test   cl, 2
        jne    SHORT loc_6_02BF
        db  00Fh, 0B7h, 00Dh, 0FAh, 014h, 000h, 000h
        pop    eax
        stc
        ret
loc_6_02BF:
        pop    eax
        clc
        ret
        db  08Bh, 0C0h
sub_6_02C4:
        db  066h, 083h, 03Dh, 0FCh, 014h, 000h, 000h, 000h
        je     SHORT loc_6_02FC
        db  08Ah, 01Dh, 003h, 015h, 000h, 000h
        db  00Ah, 01Dh, 002h, 015h, 000h, 000h
        jne    SHORT loc_6_02FC
        not    bl
        db  088h, 01Dh, 002h, 015h, 000h, 000h
        db  088h, 01Dh, 003h, 015h, 000h, 000h
        db  08Bh, 01Dh, 0B4h, 014h, 000h, 000h
        db  089h, 01Dh, 004h, 015h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_6_02FC:
        ret
        db  02Eh, 08Bh, 0C0h
loc_6_0300:
        in     al, 64h
        test   al, 1
        je     NEAR PTR loc_6_038C
        test   al, 20h
        je     SHORT loc_6_038C
        mov    ah, al
        in     al, 60h
        db  080h, 03Dh, 014h, 015h, 000h, 000h, 0FFh
        je     SHORT loc_6_038E
        db  0FEh, 005h, 016h, 015h, 000h, 000h
        db  080h, 03Dh, 016h, 015h, 000h, 000h, 003h
        jb     SHORT loc_6_0344
        db  0C6h, 005h, 016h, 015h, 000h, 000h, 000h
        db  066h, 083h, 03Dh, 0FCh, 014h, 000h, 000h, 01Eh
        jb     SHORT loc_6_034D
        db  0C6h, 005h, 015h, 015h, 000h, 000h, 002h
        jmp    SHORT loc_6_037A
loc_6_0344:
        db  080h, 03Dh, 015h, 015h, 000h, 000h, 000h
        jne    SHORT loc_6_0374
loc_6_034D:
        push   ebx
        db  066h, 0FFh, 005h, 0FCh, 014h, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 0F8h, 014h, 000h, 000h
        db  066h, 089h, 004h, 05Dh, 0B8h, 014h, 000h, 000h
        inc    bx
        and    bx, 1fh
        db  066h, 089h, 01Dh, 0F8h, 014h, 000h, 000h
        pop    ebx
        jmp    SHORT loc_6_037A
loc_6_0374:
        db  0FEh, 00Dh, 015h, 015h, 000h, 000h
loc_6_037A:
        db  0A1h, 0FEh, 014h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        call   sub_6_02C4
        clc
        ret
loc_6_038C:
        stc
        ret
loc_6_038E:
        db  0A1h, 0FEh, 014h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
        db  090h
loc_6_039C:
        db  0C6h, 005h, 003h, 015h, 000h, 000h, 000h
        jmp    NEAR PTR sub_6_02C4
loc_6_03A8:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    edi, esi
        add    di, 0ah
        mov    al, byte ptr [edi]
        db  0A2h, 010h, 015h, 000h, 000h
        db  03Bh, 01Dh, 0B4h, 014h, 000h, 000h
        jne    SHORT loc_6_0445
        db  003h, 01Dh, 00Ch, 015h, 000h, 000h
        cmp    byte ptr [ebx + 5], 0ffh
        jne    SHORT loc_6_0445
        cmp    byte ptr [ebx + 4], 0ffh
        je     SHORT loc_6_03E9
        mov    ecx, dword ptr [ebx]
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_6_03E9:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        add    si, 0ah
        mov    ecx, 4
        push   dword ptr [ebp + 30h]
loc_6_040A:
        mov    ax, word ptr [esi]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        sub    si, 2
        loop   loc_6_040A
        mov    ecx, dword ptr [ebx]
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    dword ptr [ebp + 30h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_6_0445:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
loc_6_044B:
        ret
_PTEXT ends
        end

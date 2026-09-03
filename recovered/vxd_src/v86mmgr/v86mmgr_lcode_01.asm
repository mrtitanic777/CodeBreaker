; v86mmgr_lcode_01.asm -- V86MMGR object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

V86MMGR_Load_Client_Ptr:
        push   ecx
        movzx  ecx, ah
        mov    fs, word ptr [ecx + ebp]
        movzx  ecx, al
        mov    esi, dword ptr [ecx + ebp]
        test   word ptr [ebx], 180h
        jne    SHORT loc_1_0018
        movzx  esi, si
loc_1_0018:
        pop    ecx
        ret
        db  08Bh, 0C0h
V86MMGR_Allocate_Buffer:
        push   eax
        pushfd
        jae    SHORT loc_1_0031
        mov    ax, fs
        lsl    eax, eax
        inc    eax
        je     SHORT loc_1_0031
        sub    eax, esi
        cmp    eax, ecx
        jae    SHORT loc_1_0031
        mov    ecx, eax
loc_1_0031:
        db  0A1h, 0FCh, 011h, 000h, 000h
        mov    edi, dword ptr [eax + ebx + 14h]
        sub    edi, ecx
        jb     SHORT loc_1_0080
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_0058
        push   ecx
        add    ecx, edi
        cmp    ecx, dword ptr [eax + ebx + 18h]
        jne    SHORT loc_1_0057
        xor    ecx, ecx
        db  0CDh, 020h, 03Ch, 001h, 001h, 000h
loc_1_0057:
        pop    ecx
loc_1_0058:
        mov    dword ptr [eax + ebx + 14h], edi
        popfd
        jae    SHORT loc_1_0079
        push   esi
        push   edi
        push   ecx
        cld
        add    edi, dword ptr [eax + ebx + 20h]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [esp]
        and    ecx, 3
        rep movsb
        pop    ecx
        pop    edi
        pop    esi
loc_1_0079:
        or     edi, dword ptr [eax + ebx + 1ch]
        pop    eax
        clc
        ret
loc_1_0080:
        popfd
        pop    eax
        stc
        ret
V86MMGR_Free_Buffer:
        jae    SHORT loc_1_00BC
        pushad
        mov    edi, fs
        lar    edx, edi
        test   dh, 8
        jne    SHORT loc_1_00E2
        test   dh, 2
        je     SHORT loc_1_00E2
        mov    es, edi
        mov    edi, esi
loc_1_009A:
        db  003h, 01Dh, 0FCh, 011h, 000h, 000h
        mov    esi, dword ptr [ebx + 14h]
        add    esi, dword ptr [ebx + 20h]
        cld
loc_1_00A7:
        mov    eax, ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_00B5:
        mov    ax, ds
        mov    es, ax
        popad
loc_1_00BC:
        push   edx
        push   ecx
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        add    edx, ebx
        add    dword ptr [edx + 14h], ecx
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_00DF
        mov    ecx, dword ptr [edx + 14h]
        cmp    ecx, dword ptr [edx + 18h]
        jne    SHORT loc_1_00DF
        db  0CDh, 020h, 072h, 001h, 001h, 000h
loc_1_00DF:
        pop    ecx
        pop    edx
        ret
loc_1_00E2:
        push   0
        push   fs
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        mov    ecx, dword ptr [esp + 18h]
        lea    edi, [esi + eax]
        jmp    SHORT loc_1_009A
V86MMGR_Xlat_API:
        push   eax
        movzx  eax, byte ptr [edx]
        inc    edx
        db  08Bh, 004h, 085h, 08Ch, 006h, 000h, 000h
        xchg   dword ptr [esp], eax
        ret
loc_1_0109:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   eax
        movzx  eax, byte ptr [edx]
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 086h, 080h, 001h, 000h
loc_1_0120:
        add    edx, 4
        jmp    dword ptr [edx - 4]
loc_1_0126:
        push   ecx
        movzx  ecx, word ptr [edx]
        add    edx, 2
        call   sub_1_015F
        pop    ecx
        ret
loc_1_0134:
        push   ecx
        movzx  ecx, byte ptr [edx]
        mov    cx, word ptr [ecx + ebp]
        inc    edx
        call   sub_1_015F
loc_1_0142:
        pop    ecx
        ret
loc_1_0144:
        push   ecx
        db  068h, 042h, 001h, 000h, 000h
        push   fs
        push   eax
        push   esi
        movzx  eax, word ptr [edx + 4]
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        call   dword ptr [edx]
        add    edx, 4
        jmp    SHORT loc_1_016C
sub_1_015F:
        push   fs
        push   eax
        push   esi
        mov    ax, word ptr [edx]
        db  0CDh, 020h, 007h, 000h, 006h, 000h
loc_1_016C:
        push   edi
        stc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        movzx  eax, al
        add    eax, ebp
        push   dword ptr [eax]
        mov    word ptr [eax], di
        push   eax
        movzx  eax, byte ptr [edx + 1]
        shr    edi, 10h
        xchg   word ptr [ebp + eax + 24h], di
        add    edx, 2
        db  0CDh, 020h, 006h, 000h, 006h, 000h
        mov    word ptr [ebp + eax + 24h], di
        pop    eax
        pop    dword ptr [eax]
        stc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        pop    edi
        pop    esi
        pop    eax
        pop    fs
        ret
loc_1_01A9:
        push   fs
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    ax, word ptr [edx]
        db  0CDh, 020h, 007h, 000h, 006h, 000h
        push   es
        mov    ax, fs
        mov    es, ax
        mov    edi, esi
        lsl    ecx, eax
        sub    ecx, edi
        inc    ecx
        cld
        xor    al, al
        repne scasb
        pop    es
        je     SHORT loc_1_01D5
        lea    edi, [esi + 100h]
loc_1_01D5:
        sub    edi, esi
        lea    ecx, [edi + 3]
        stc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        movzx  eax, byte ptr [edx]
        add    eax, ebp
        push   dword ptr [eax]
        mov    word ptr [eax], di
        push   eax
        movzx  eax, byte ptr [edx + 1]
        shr    edi, 10h
        xchg   word ptr [ebp + eax + 24h], di
        push   eax
        mov    al, byte ptr [edx - 1]
        add    edx, 2
        db  0CDh, 020h, 006h, 000h, 006h, 000h
        mov    edx, eax
        pop    eax
        mov    word ptr [ebp + eax + 24h], di
        pop    eax
        pop    dword ptr [eax]
        cmp    dl, 8
        cmc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        pop    fs
        ret
        db  000h, 000h, 000h
loc_1_0224:
        cmp    byte ptr [ebp + 1dh], 87h
        je     SHORT loc_1_023A
        cmp    byte ptr [ebp + 1dh], 88h
        je     SHORT loc_1_0232
        stc
        ret
loc_1_0232:
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_1_023A:
        movzx  esi, word ptr [ebp + 4]
        movzx  ecx, word ptr [ebp + 38h]
        shl    ecx, 4
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        movzx  ecx, word ptr [ebp + 18h]
        call   sub_1_0255
        clc
        ret
sub_1_0255:
        pushfd
        sti
        cmp    ecx, 8000h
        ja     NEAR PTR loc_1_0351
        mov    eax, ecx
        shl    eax, 1
        dec    eax
        cmp    ax, word ptr [esi + 10h]
        ja     NEAR PTR loc_1_0351
        cmp    ax, word ptr [esi + 18h]
        ja     NEAR PTR loc_1_0351
        mov    dl, byte ptr [esi + 1ch]
        mov    dh, byte ptr [esi + 1fh]
        movzx  edi, dx
        shl    edi, 10h
        mov    di, word ptr [esi + 1ah]
        mov    dl, byte ptr [esi + 14h]
        mov    dh, byte ptr [esi + 17h]
        shl    edx, 10h
        mov    dx, word ptr [esi + 12h]
        mov    esi, edx
        shl    ecx, 1
        cmp    edi, 110000h
        jae    SHORT loc_1_02D5
        mov    eax, edi
        add    eax, ecx
        cmp    eax, 100000h
        jbe    SHORT loc_1_02EB
        cmp    edi, 100000h
        jb     NEAR PTR loc_1_0351
        db  0E8h, 01Dh, 0FEh, 0FEh, 0FFh
        jb     SHORT loc_1_02D5
        jne    NEAR PTR loc_1_0351
        xchg   edi, eax
        add    eax, ecx
        cmp    eax, 110000h
        ja     SHORT loc_1_0351
        jmp    SHORT loc_1_02EE
loc_1_02D5:
        push   ecx
        push   0
        push   ecx
        push   edi
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        mov    edi, eax
        inc    eax
        jne    SHORT loc_1_02EE
        jmp    SHORT loc_1_0351
loc_1_02EB:
        add    edi, dword ptr [ebx + 4]
loc_1_02EE:
        cmp    esi, 110000h
        jae    SHORT loc_1_031F
        mov    edx, esi
        add    edx, ecx
        cmp    edx, 100000h
        jbe    SHORT loc_1_0335
        cmp    esi, 100000h
        jb     SHORT loc_1_0351
        db  0E8h, 0CFh, 0FDh, 0FEh, 0FFh
        jb     SHORT loc_1_031F
        jne    SHORT loc_1_0351
        cmp    edx, 110000h
        ja     SHORT loc_1_0351
        mov    esi, eax
        jmp    SHORT loc_1_0338
loc_1_031F:
        push   ecx
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        mov    esi, eax
        inc    eax
        jne    SHORT loc_1_0338
        jmp    SHORT loc_1_0351
loc_1_0335:
        add    esi, dword ptr [ebx + 4]
loc_1_0338:
        push   ecx
        push   esi
        push   edi
        db  0E8h, 0DDh, 0FDh, 0FEh, 0FFh
        add    esp, 0ch
        mov    byte ptr [ebp + 1dh], 0
        or     byte ptr [ebp + 2ch], 40h
        and    byte ptr [ebp + 2ch], 0feh
loc_1_034F:
        popfd
        ret
loc_1_0351:
        mov    byte ptr [ebp + 1dh], 2
        and    byte ptr [ebp + 2ch], 0bfh
        or     byte ptr [ebp + 2ch], 1
        jmp    SHORT loc_1_034F
        db  000h
sub_1_0360:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        db  08Bh, 03Dh, 0B0h, 006h, 000h, 000h
        cmp    edi, dword ptr [esi + ebx + 28h]
        je     SHORT loc_1_03AA
        xchg   dword ptr [esi + ebx + 28h], edi
        db  08Bh, 035h, 0B4h, 00Eh, 000h, 000h
loc_1_037C:
        db  039h, 03Ch, 0B5h, 0B4h, 006h, 000h, 000h
        jbe    SHORT loc_1_03A1
        db  08Bh, 004h, 0B5h, 0B4h, 00Ah, 000h, 000h
        and    eax, 0ffffffh
        push   0
        push   1
        push   esi
        push   ebx
        push   eax
        db  0CDh, 020h, 062h, 000h, 001h, 000h
        add    esp, 14h
loc_1_03A1:
        inc    esi
        db  03Bh, 035h, 0B8h, 00Eh, 000h, 000h
        jbe    SHORT loc_1_037C
loc_1_03AA:
        ret
V86MMGR_Map_Pages:
        test   ecx, ecx
        jne    SHORT loc_1_03B6
        or     edi, 0ffffffffh
        movzx  esi, di
        ret
loc_1_03B6:
        pushad
        mov    eax, esi
        and    eax, 0fffh
        lea    edi, [ecx + eax + 0fffh]
        shr    edi, 0ch
        mov    ecx, esi
        jecxz  loc_1_03E4
        shr    ecx, 0ch
        push   0
        push   edi
        push   ecx
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_04F0
loc_1_03E4:
        mov    ecx, edi
        test   esi, esi
        je     SHORT loc_1_043C
        db  08Bh, 01Dh, 0B4h, 00Eh, 000h, 000h
loc_1_03F0:
        lea    edx, [ecx + ebx - 1]
        db  03Bh, 015h, 0B8h, 00Eh, 000h, 000h
        ja     SHORT loc_1_043C
        mov    edx, esi
        shr    edx, 0ch
        mov    edi, ecx
        mov    ebp, ebx
loc_1_0405:
        db  08Bh, 004h, 09Dh, 0B4h, 00Ah, 000h, 000h
        and    eax, 0ffffffh
        cmp    eax, edx
        jne    SHORT loc_1_042F
        inc    ebx
        inc    edx
        dec    edi
        jne    SHORT loc_1_0405
        mov    edi, ecx
        mov    ebx, ebp
loc_1_041E:
        db  0FEh, 004h, 09Dh, 0B7h, 00Ah, 000h, 000h
        inc    ebx
        loop   loc_1_041E
        mov    ebx, ebp
        jmp    NEAR PTR loc_1_04B2
loc_1_042F:
        cmp    edi, ecx
        jb     SHORT loc_1_03F0
        inc    ebx
        db  03Bh, 01Dh, 0B8h, 00Eh, 000h, 000h
        jb     SHORT loc_1_03F0
loc_1_043C:
        mov    edi, ecx
        db  0A1h, 0B8h, 00Eh, 000h, 000h
loc_1_0443:
        db  00Fh, 0B7h, 01Ch, 045h, 028h, 00Fh, 000h, 000h
        test   bh, 4
        je     SHORT loc_1_045A
        test   bl, 1
        jne    SHORT loc_1_045A
        dec    edi
        je     SHORT loc_1_0467
        jmp    SHORT loc_1_045C
loc_1_045A:
        mov    edi, ecx
loc_1_045C:
        dec    eax
        db  03Bh, 005h, 0B4h, 00Eh, 000h, 000h
        jae    SHORT loc_1_0443
        jmp    SHORT loc_1_04D9
loc_1_0467:
        push   eax
        mov    edi, ecx
        mov    bx, 1
        test   esi, esi
        jne    SHORT loc_1_047B
        or     bx, 8000h
        xor    edx, edx
        jmp    SHORT loc_1_0491
loc_1_047B:
        shr    esi, 0ch
        or     esi, 1000000h
        db  08Bh, 015h, 0B0h, 006h, 000h, 000h
        inc    edx
        db  089h, 015h, 0B0h, 006h, 000h, 000h
loc_1_0491:
        db  089h, 014h, 085h, 0B4h, 006h, 000h, 000h
        db  089h, 034h, 085h, 0B4h, 00Ah, 000h, 000h
        db  066h, 009h, 01Ch, 045h, 028h, 00Fh, 000h, 000h
        inc    eax
        inc    esi
        loop   loc_1_0491
        db  029h, 03Dh, 0C0h, 00Eh, 000h, 000h
        pop    ebx
loc_1_04B2:
        mov    eax, edi
        shl    eax, 10h
        mov    ax, bx
        xchg   dword ptr [esp + 4], eax
        and    eax, 0fffh
        shl    ebx, 0ch
        add    eax, ebx
        mov    dword ptr [esp], eax
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        call   sub_1_0360
        popad
        clc
        ret
loc_1_04D9:
        xchg   ecx, eax
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_04F0
        shr    ecx, 0ch
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_1_04F0:
        popad
        stc
        ret
V86MMGR_Free_Page_Map_Region:
        pushad
        movzx  edi, si
        shr    esi, 10h
        je     SHORT loc_1_0578
loc_1_04FC:
        db  0F6h, 004h, 07Dh, 029h, 00Fh, 000h, 000h, 080h
        jne    SHORT loc_1_051F
        push   0
        push   1
        db  0FFh, 034h, 0BDh, 0B4h, 00Ah, 000h, 000h
        mov    byte ptr [esp + 3], 0
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_1_051F:
        db  0FEh, 00Ch, 0BDh, 0B7h, 00Ah, 000h, 000h
        jne    SHORT loc_1_0574
        xor    eax, eax
        db  089h, 004h, 0BDh, 0B4h, 006h, 000h, 000h
        db  089h, 004h, 0BDh, 0B4h, 00Ah, 000h, 000h
        db  066h, 081h, 024h, 07Dh, 028h, 00Fh, 000h, 000h, 0FEh, 07Fh
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_1_0548:
        db  0CDh, 020h, 06Ah, 000h, 001h, 000h
        push   0
        push   0
        push   1
        push   edi
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_1_0548
        db  0FFh, 005h, 0C0h, 00Eh, 000h, 000h
loc_1_0574:
        inc    edi
        dec    esi
        jne    SHORT loc_1_04FC
loc_1_0578:
        popad
        ret
V86MMGR_Get_Mapping_Info:
        db  08Ah, 02Dh, 0BCh, 00Eh, 000h, 000h
        db  08Ah, 00Dh, 0C0h, 00Eh, 000h, 000h
        ret
        db  000h
V86MMGR_Get_Version:
        mov    eax, 30ah
        clc
        ret
V86MMGR_Control:
        cmp    eax, 17h
        jae    SHORT loc_1_059B
        db  0FFh, 024h, 085h, 09Dh, 005h, 000h, 000h
loc_1_059B:
        clc
        ret
        db  05Ch, 01Ah, 000h, 000h, 0EAh, 01Ah, 000h, 000h, 04Ah, 01Eh, 000h, 000h, 0F0h, 02Eh, 000h, 000h
        db  000h, 000h, 000h, 000h, 092h, 000h, 000h, 000h, 016h, 02Fh, 000h, 000h, 018h, 006h, 000h, 000h
        db  09Bh, 005h, 000h, 000h, 075h, 004h, 000h, 000h, 09Bh, 005h, 000h, 000h, 0F9h, 000h, 000h, 000h
        db  0F9h, 000h, 000h, 000h, 070h, 000h, 000h, 000h, 0E4h, 000h, 000h, 000h, 09Bh, 005h, 000h, 000h
        db  09Bh, 005h, 000h, 000h, 09Bh, 005h, 000h, 000h, 09Bh, 005h, 000h, 000h, 09Bh, 005h, 000h, 000h
        db  09Bh, 005h, 000h, 000h, 09Bh, 005h, 000h, 000h, 0ECh, 000h, 000h, 000h
sub_1_05F9:
        push   ecx
        push   edx
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        pop    edx
        pop    ecx
        ret
sub_1_060C:
        push   esi
        push   edi
        push   edx
        push   ebx
        push   eax
        add    ecx, 3
        db  03Bh, 00Dh, 010h, 012h, 000h, 000h
        jbe    SHORT loc_1_0622
        db  08Bh, 00Dh, 010h, 012h, 000h, 000h
loc_1_0622:
        or     eax, eax
        je     SHORT loc_1_0642
        db  03Bh, 00Dh, 010h, 012h, 000h, 000h
        jb     SHORT loc_1_0642
        push   ecx
        db  0CDh, 020h, 000h, 000h, 012h, 000h
        jb     SHORT loc_1_0641
        or     eax, eax
        je     SHORT loc_1_0641
        shl    ecx, 2
        sub    dword ptr [esp], ecx
loc_1_0641:
        pop    ecx
loc_1_0642:
        shr    ecx, 2
        db  02Bh, 00Dh, 008h, 012h, 000h, 000h
        ja     SHORT loc_1_0652
        mov    ecx, 1
loc_1_0652:
        pop    eax
        pop    ebx
        pop    edx
        pop    edi
        pop    esi
        ret
loc_1_0658:
        cmp    byte ptr [ebp + 1dh], 43h
        je     SHORT loc_1_0670
        db  08Bh, 00Dh, 0A4h, 012h, 000h, 000h
        db  08Bh, 015h, 0A8h, 012h, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_0670:
        db  0CDh, 020h, 047h, 080h, 001h, 000h
        add    byte ptr [eax], al
loc_1_0678:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmpsd
        add    byte ptr [eax], al
        db  000h, 0B5h, 000h, 000h, 000h, 0B5h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_068B:
        db  000h, 009h
        add    dword ptr [eax], eax
        db  000h, 026h
        add    dword ptr [eax], eax
        db  000h, 034h, 001h
        add    byte ptr [eax], al
        inc    esp
        add    dword ptr [eax], eax
        db  000h, 0A9h, 001h, 000h, 000h, 020h
        add    dword ptr [eax], eax
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 0A9h, 001h, 000h, 000h, 001h
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
loc_1_0AB7:
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
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 028h
        db  02Ch, 000h, 000h, 07Dh, 02Ch, 000h, 000h, 08Bh, 02Dh, 000h, 000h, 09Bh, 02Dh, 000h, 000h, 029h
        db  02Eh, 000h, 000h, 023h, 02Ch, 000h, 000h, 023h, 02Ch, 000h, 000h, 023h, 02Ch, 000h, 000h, 023h
        db  02Ch, 000h, 000h, 000h, 004h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 088h, 005h, 000h, 000h, 077h, 006h, 000h, 000h, 0B1h, 00Ah, 000h, 000h, 022h
        db  02Fh, 000h, 000h, 0B4h, 00Bh, 000h, 000h, 07Ah, 005h, 000h, 000h, 0F9h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 01Ch, 000h, 000h, 000h, 084h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 035h
        db  000h, 000h, 000h, 058h, 000h, 000h, 000h, 0ABh, 003h, 000h, 000h, 0F3h, 004h, 000h, 000h, 005h
        db  014h, 000h, 000h, 0D3h, 02Fh, 000h, 000h, 06Dh, 028h, 000h, 000h, 078h, 009h, 000h, 000h, 0A5h
        db  018h, 000h, 000h, 07Eh, 028h, 000h, 000h, 05Eh, 02Fh, 000h, 000h, 0ECh, 003h, 000h, 000h, 07Ah
        db  01Dh, 000h, 000h, 0CDh, 01Dh, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 006h, 000h, 001h, 000h, 000h, 000h, 056h, 038h, 036h, 04Dh
        db  04Dh, 047h, 052h, 020h, 000h, 000h, 000h, 0A0h, 08Fh, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0B4h, 034h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  048h, 011h, 000h, 000h, 019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1218:
        db  072h, 000h
        db  000h, 080h
loc_1_121C:
        cmp    al, 30h
        db  000h, 080h, 041h, 030h, 000h, 080h
loc_1_1224:
        db  07Ah, 001h
        db  000h, 080h, 0BEh, 001h, 000h, 080h
        cmc
        add    dword ptr [eax], eax
        db  080h, 05Ch, 002h, 000h, 080h
        ret    3
        db  080h, 068h, 005h, 000h, 000h, 030h, 006h, 000h, 000h, 0C7h, 006h, 000h, 080h, 086h, 008h, 000h
        db  080h, 060h, 009h, 000h, 080h, 08Bh, 009h, 000h, 080h, 012h, 00Ah, 000h, 000h, 006h, 00Bh, 000h
        db  000h, 00Ch, 031h, 000h, 080h, 0C1h, 031h, 000h, 080h, 018h, 032h, 000h, 080h
loc_1_1264:
        lds    eax, ptr [edx]
        db  000h, 080h, 0A1h, 030h, 000h, 080h
        sub    al, 3
        db  000h, 080h, 046h, 003h, 000h, 080h
        pop    ebx
        add    eax, dword ptr [eax]
        db  080h, 087h, 003h, 000h, 080h, 000h, 000h
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1328:
        insb
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
_LTEXT ends
        end

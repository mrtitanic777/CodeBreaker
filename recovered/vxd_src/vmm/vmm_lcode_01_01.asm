; vmm_lcode_01_01.asm -- VMM object 1 segment _LTEXT  (part 1/4)
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        cld
        pushad
        mov    eax, dword ptr [esp + 20h]
        db  02Dh, 019h, 030h, 000h, 000h
        shr    eax, 3
        mov    ebp, esp
        mov    bx, 30h
        mov    ds, ebx
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    es, ebx
        xchg   dword ptr [edi + 4ch], esp
        sti
loc_1_0022:
        db  03Dh, 0D9h, 0FFh, 0FEh, 0FFh
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  068h, 000h, 003h, 000h, 000h
        db  0FFh, 024h, 085h, 00Ch, 0CFh, 000h, 000h
loc_1_0039:
        mov    cx, 30h
        mov    ds, cx
        mov    es, cx
        shr    eax, 2
        db  068h, 0B7h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_153C
sub_1_0050:
        cld
        pushad
        db  036h, 08Bh, 015h, 0BCh, 0ECh, 000h, 000h
        mov    cx, word ptr ss:[edx]
        add    ch, cl
        ja     2
        db  036h, 08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  036h, 08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    ebp, esp
        xchg   dword ptr ss:[edi + 4ch], esp
        sti
        mov    eax, dword ptr [ebp + 20h]
        db  02Dh, 019h, 030h, 000h, 000h
        shr    eax, 1
        movzx  edi, word ptr [ebp + 30h]
        sub    edi, 6
        jb     SHORT loc_1_0039
        mov    dword ptr [ebp + 30h], edi
        movzx  esi, word ptr [ebp + 34h]
        shl    esi, 4
        add    edi, esi
        mov    eax, dword ptr ss:[eax]
        xchg   word ptr [ebp + 24h], ax
        ror    eax, 10h
        xchg   word ptr [ebp + 28h], ax
        ror    eax, 10h
        mov    dword ptr ss:[edi], eax
        mov    eax, dword ptr [ebp + 2ch]
        mov    word ptr ss:[edi + 4], ax
        and    ah, 0fch
        mov    byte ptr [ebp + 2dh], ah
loc_1_00B7:
        cli
        db  036h, 083h, 03Dh, 0E0h, 0D9h, 000h, 000h, 000h
        jne    SHORT loc_1_00D4
        db  036h, 08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        xchg   dword ptr ss:[edi + 4ch], esp
        popad
        add    esp, 4
        iretd
        db  08Bh, 0C0h
loc_1_00D4:
        mov    ax, 30h
        mov    ds, eax
        mov    es, eax
        jmp    NEAR PTR sub_1_0300
        db  02Eh, 08Bh, 0C0h
sub_1_00E4:
        pushad
        mov    esi, dword ptr [esp + 20h]
        db  081h, 0EEh, 019h, 030h, 000h, 000h
        shr    esi, 1
        jmp    NEAR PTR loc_1_02C0
        db  02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h, 090h
loc_1_0100:
        sub    esp, 4
        pushad
        mov    esi, 140h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0110:
        sub    esp, 4
        pushad
        mov    esi, 144h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0120:
        sub    esp, 4
        pushad
        mov    esi, 148h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0130:
        sub    esp, 4
        pushad
        mov    esi, 14ch
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0140:
        sub    esp, 4
        pushad
        mov    esi, 150h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0150:
        sub    esp, 4
        pushad
        mov    esi, 154h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0160:
        sub    esp, 4
        pushad
        mov    esi, 158h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0170:
        sub    esp, 4
        pushad
        mov    esi, 15ch
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0180:
        sub    esp, 4
        pushad
        mov    esi, 160h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0190:
        sub    esp, 4
        pushad
        mov    esi, 164h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_01A0:
        sub    esp, 4
        pushad
        mov    esi, 168h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_01B0:
        sub    esp, 4
        pushad
        mov    esi, 16ch
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_01C0:
        sub    esp, 4
        pushad
        mov    esi, 170h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_01D0:
        sub    esp, 4
        pushad
        mov    esi, 174h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_01E0:
        sub    esp, 4
        pushad
        mov    esi, 178h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_01F0:
        sub    esp, 4
        pushad
        mov    esi, 17ch
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0200:
        sub    esp, 4
        pushad
        mov    esi, 0
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0210:
        sub    esp, 4
        pushad
        mov    esi, 4
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0220:
        sub    esp, 4
        pushad
        mov    esi, 0ch
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0230:
        sub    esp, 4
        pushad
        mov    esi, 10h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0240:
        sub    esp, 4
        pushad
        mov    esi, 14h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0250:
        sub    esp, 4
        pushad
        mov    esi, 18h
        jmp    NEAR PTR loc_1_02C0
        db  08Bh, 0C0h
loc_1_0260:
        sub    esp, 4
        pushad
        mov    esi, 1ch
        jmp    SHORT loc_1_02C0
        db  090h
loc_1_026C:
        sub    esp, 4
        pushad
        mov    esi, 24h
        jmp    SHORT loc_1_02C0
        db  090h
loc_1_0278:
        sub    esp, 4
        pushad
        mov    esi, 3ch
        jmp    SHORT loc_1_02C0
        db  090h
loc_1_0284:
        pushad
        mov    esi, 20h
        jmp    SHORT loc_1_02C0
loc_1_028C:
        pushad
        mov    esi, 28h
        jmp    SHORT loc_1_02C0
        db  02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h
loc_1_02A0:
        pushad
        mov    esi, 2ch
        jmp    SHORT loc_1_02C0
_Deallocate_Device_CB_Area:
        pushad
        mov    esi, 30h
        jmp    SHORT loc_1_02C0
loc_1_02B0:
        pushad
        mov    esi, 34h
        jmp    SHORT loc_1_02C0
loc_1_02B8:
        pushad
        mov    esi, 38h
        mov    esi, esi
loc_1_02C0:
        cld
loc_1_02C1:
        mov    di, 30h
loc_1_02C5:
        mov    ebp, esp
loc_1_02C7:
        test   byte ptr [ebp + 2eh], 2
        je     NEAR PTR loc_1_0370
        mov    ds, edi
        mov    es, edi
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        xchg   dword ptr [edi + 4ch], esp
loc_1_02DE:
        db  03Dh, 01Dh, 0FDh, 0FEh, 0FFh
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  068h, 000h, 003h, 000h, 000h
        db  0FFh, 0A6h, 01Ch, 0E4h, 000h, 000h
        db  02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h
sub_1_0300:
        db  03Dh, 015h, 0FDh, 0FEh, 0FFh
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        cli
        xor    eax, eax
        db  03Bh, 005h, 0E0h, 0D9h, 000h, 000h
        jne    SHORT loc_1_0354
loc_1_031C:
        mov    al, byte ptr [ebx + 1fh]
        not    al
        shl    al, 1
        and    al, byte ptr [edi + 73h]
        and    al, 40h
        shr    al, 3
        or     byte ptr [edi + 3], al
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_03AC
loc_1_0333:
        db  03Dh, 00Bh, 0FDh, 0FEh, 0FFh
        xchg   dword ptr [edi + 4ch], esp
        mov    al, byte ptr [esp + 2dh]
        or     al, 30h
        test   al, 2
        jne    SHORT loc_1_0348
        and    al, byte ptr [ebx - 67h]
loc_1_0348:
        mov    byte ptr [esp + 2dh], al
        popad
        add    esp, 4
        iretd
        db  02Eh, 08Bh, 0C0h
loc_1_0354:
        call   sub_1_217F
        je     SHORT loc_1_031C
        db  068h, 000h, 003h, 000h, 000h
        jmp    NEAR PTR sub_1_229A
        db  02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h, 02Eh, 08Bh, 0C0h, 08Bh, 0C0h
loc_1_0370:
        db  02Eh, 08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        cmp    ebp, dword ptr cs:[ebx + 8]
        jne    NEAR PTR loc_1_0410
loc_1_0381:
        mov    word ptr [ebp + 3ch], ds
        mov    word ptr [ebp + 38h], es
        mov    word ptr [ebp + 40h], fs
        mov    word ptr [ebp + 44h], gs
loc_1_038D:
        mov    ds, edi
        mov    es, edi
        mov    fs, edi
        mov    gs, edi
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        xchg   dword ptr [edi + 4ch], esp
        db  068h, 000h, 003h, 000h, 000h
        db  0FFh, 0A6h, 0A4h, 0E5h, 000h, 000h
        db  02Eh, 08Bh, 0C0h
loc_1_03AC:
        xchg   dword ptr [edi + 4ch], esp
        cmp    word ptr [esp + 28h], 3bh
        je     SHORT loc_1_0404
loc_1_03B7:
        mov    ds, word ptr [esp + 3ch]
loc_1_03BB:
        mov    es, word ptr [esp + 38h]
loc_1_03BF:
        mov    fs, word ptr [esp + 40h]
loc_1_03C3:
        mov    gs, word ptr [esp + 44h]
        test   byte ptr ss:[ebx], 80h
        jne    SHORT loc_1_03CD
loc_1_03CD:
        popad
        add    esp, 4
        iretd
loc_1_03D2:
        lar    eax, word ptr [esp + 34h]
        test   eax, 400000h
        jne    SHORT loc_1_03CD
        mov    cx, 4092h
        db  036h, 08Bh, 015h, 0ACh, 0EFh, 000h, 000h
        shld   ecx, esp, 10h
        db  0B8h
loc_1_03EE:
        mov    al, byte ptr [86000000h]
        int    0c1h
        leave
        or     byte ptr [esi], dh
        mov    dword ptr [eax + edx + 4], ecx
        mov    ss, ax
        movzx  esp, sp
        jmp    SHORT loc_1_03CD
loc_1_0404:
        mov    ebp, esp
        movzx  eax, word ptr [esp + 24h]
        jmp    NEAR PTR loc_1_0B3D
loc_1_0410:
        cmp    esi, 34h
        jbe    SHORT loc_1_0448
loc_1_0415:
        push   ds
        mov    ds, edi
        db  0FFh, 005h, 0F8h, 0E3h, 000h, 000h
        push   es
        mov    es, edi
        push   fs
        push   gs
        and    esp, 0fffffffch
        mov    eax, eax
        db  0FFh, 096h, 02Ch, 0E7h, 000h, 000h
        cli
        lea    esp, [ebp - 10h]
        db  0FFh, 00Dh, 0F8h, 0E3h, 000h, 000h
        pop    gs
        pop    fs
        pop    es
        pop    ds
        popad
        add    esp, 4
        iretd
        db  02Eh, 08Bh, 0C0h
loc_1_0448:
        cmp    esi, 2ch
        jb     SHORT loc_1_0415
        db  081h, 07Dh, 024h, 0B7h, 003h, 000h, 000h
        jb     SHORT loc_1_0415
        db  081h, 07Dh, 024h, 0D2h, 003h, 000h, 000h
        jae    SHORT loc_1_0415
        mov    ecx, ebp
        mov    eax, dword ptr [ebp + 24h]
        mov    ebp, dword ptr cs:[ebx + 8]
        lea    edx, [ebp + 3ch]
        db  03Dh, 0B7h, 003h, 000h, 000h
        je     SHORT loc_1_0490
        lea    edx, [ebp + 38h]
        db  03Dh, 0BBh, 003h, 000h, 000h
        je     SHORT loc_1_0490
        lea    edx, [ebp + 40h]
        db  03Dh, 0BFh, 003h, 000h, 000h
        je     SHORT loc_1_0490
        lea    edx, [ebp + 44h]
        db  03Dh, 0C3h, 003h, 000h, 000h
        jne    SHORT loc_1_04BE
loc_1_0490:
        cmp    esi, 34h
        je     SHORT loc_1_04B3
        xchg   ecx, ebp
        db  0C7h, 045h, 024h, 08Dh, 003h, 000h, 000h
        mov    dword ptr [ebp + 4], esi
        mov    dword ptr [ebp + 10h], ebx
        mov    dword ptr [ebp], edi
        mov    dword ptr [ebp + 8], ecx
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr ss:[ecx + 20h], eax
        jmp    SHORT loc_1_04B9
loc_1_04B3:
        mov    word ptr ss:[edx], 0
loc_1_04B9:
        popad
        add    esp, 4
        iretd
loc_1_04BE:
        push   ds
        push   es
        mov    ds, edi
        mov    es, edi
        mov    bp, sp
        mov    ss, edi
        mov    esp, ebp
        mov    bp, cx
        cmp    esi, 34h
        jne    SHORT loc_1_04F4
        shr    esi, 2
        mov    cx, 12h
        call   sub_1_3410
        jb     SHORT loc_1_0507
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    ebp, dword ptr [edi + 10h]
        mov    esp, ebp
        xchg   dword ptr [edi + 4ch], esp
        jmp    NEAR PTR loc_1_333E
loc_1_04F4:
        db  0C7h, 045h, 024h, 00Bh, 005h, 000h, 000h
        mov    eax, dword ptr [ebp + 20h]
        mov    ebp, dword ptr [ebx + 8]
        mov    dword ptr [ebp + 44h], esi
        mov    dword ptr [ebp + 40h], eax
loc_1_0507:
        pop    es
        pop    ds
        jmp    SHORT loc_1_04B9
loc_1_050B:
        sub    esp, 4
        pushad
        cld
        mov    ebp, esp
        mov    di, 30h
        mov    esi, dword ptr [ebp + 44h]
        mov    eax, dword ptr [ebp + 40h]
        mov    dword ptr [ebp + 20h], eax
        jmp    NEAR PTR loc_1_0381
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0530:
        mov    esi, dword ptr [ebp + 24h]
        movzx  eax, word ptr [esi + 2]
        movzx  ebx, word ptr [esi]
        db  0B9h, 0C6h, 006h, 000h, 000h
        cmp    eax, 0f000h
        jae    SHORT loc_1_054B
        db  0B9h, 02Eh, 007h, 000h, 000h
loc_1_054B:
        call   ecx
        jecxz  loc_1_05A3
        btr    ebx, 0fh
        pushfd
        cmp    ebx, dword ptr [ecx + 34h]
        jae    NEAR PTR loc_1_06B2
        mov    edx, dword ptr [ecx + 30h]
        lea    edx, [edx + ebx*4]
        cmp    dword ptr [edx], 0
        je     NEAR PTR loc_1_06C0
        or     word ptr [ecx + 0ah], 2000h
        mov    ax, 25ffh
        popfd
        jb     SHORT loc_1_0596
        mov    ax, 15ffh
        db  081h, 0F9h, 080h, 0D9h, 000h, 000h
        jne    SHORT loc_1_0596
        cmp    ebx, 120h
        jae    SHORT loc_1_0596
        db  00Fh, 0A3h, 01Dh, 011h, 09Bh, 000h, 000h
        jb     SHORT loc_1_05C1
loc_1_0596:
        sub    esi, 2
        mov    dword ptr [ebp + 24h], esi
        mov    word ptr [esi], ax
        mov    dword ptr [esi + 2], edx
        ret
loc_1_05A3:
        test   ebx, ebx
        jne    SHORT loc_1_05D4
loc_1_05A7:
        mov    ax, 0b8h
        mov    edx, 0f9000000h
        test   byte ptr [esi + 1], 80h
        je     SHORT loc_1_0596
        mov    ax, 0c033h
        mov    edx, 9090c3f9h
        jmp    SHORT loc_1_0596
loc_1_05C1:
        mov    edi, dword ptr [edx]
        cmp    byte ptr [edi + 6], 0c3h
        je     SHORT loc_1_05CC
        int3
        jmp    SHORT loc_1_0596
loc_1_05CC:
        mov    ax, word ptr [edi]
        mov    edx, dword ptr [edi + 2]
        jmp    SHORT loc_1_0596
loc_1_05D4:
        db  083h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h
        je     NEAR PTR loc_1_0694
        db  0FEh, 005h, 008h, 09Bh, 000h, 000h
        jne    NEAR PTR loc_1_0694
        sti
        pushad
        cld
        sub    esp, 58h
        mov    edi, esp
        jecxz  loc_1_0601
        mov    eax, dword ptr [ecx + 0ch]
        stosd
        mov    eax, dword ptr [ecx + 10h]
        stosd
        jmp    SHORT loc_1_060A
loc_1_0601:
        mov    cl, 4
        push   ebx
        call   sub_1_0977
        pop    ebx
loc_1_060A:
        mov    byte ptr [edi], 0
        inc    edi
        push   edi
        push   esi
        db  0CDh, 020h, 07Eh, 001h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    edx, esp
        db  0CDh, 020h, 007h, 000h, 001h, 000h
        push   ecx
        push   ebx
        push   edx
        push   edi
        push   140005h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        mov    eax, 0c0001034h
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        xor    edi, edi
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        pop    ecx
        db  0CDh, 020h, 008h, 000h, 001h, 000h
        db  0FEh, 00Dh, 008h, 09Bh, 000h, 000h
        add    esp, 58h
        cmp    eax, 7
        popad
        cli
        jne    NEAR PTR loc_1_05A7
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_0694
        mov    eax, 10007h
        db  0BAh, 08Ch, 0D9h, 000h, 000h
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        xor    ecx, ecx
        mov    eax, 1388h
        db  0CDh, 020h, 0ECh, 000h, 001h, 000h
loc_1_0694:
        push   ebx
        push   eax
        push   100006h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   esi, eax
        pushad
        mov    esi, esi
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_1_06B2:
        popfd
        test   ebx, ebx
        je     NEAR PTR loc_1_05A7
        jmp    NEAR PTR loc_1_05D4
loc_1_06C0:
        popfd
        jmp    NEAR PTR loc_1_05D4
loc_1_06C6:
        xor    ecx, ecx
        and    eax, 0fffh
        db  03Bh, 005h, 00Dh, 09Bh, 000h, 000h
        ja     SHORT loc_1_06EA
        dec    eax
        shl    eax, 2
        db  003h, 005h, 009h, 09Bh, 000h, 000h
        push   esi
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi], -1
        je     SHORT loc_1_06EB
        mov    ecx, esi
        pop    esi
loc_1_06EA:
        ret
loc_1_06EB:
        pushad
        lea    esi, [esi + 4]
        cld
        lodsb
        movzx  ecx, al
        db  08Bh, 03Dh, 040h, 0C3h, 000h, 000h
loc_1_06FA:
        db  0E8h, 028h, 004h, 0FFh, 0FFh
        je     SHORT loc_1_070A
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_06FA
        popad
        pop    esi
        ret
loc_1_070A:
        mov    dword ptr [esp + 18h], edi
        popad
        mov    dword ptr [eax], ecx
        pop    esi
        ret
        db  0FCh, 060h, 0B9h, 004h, 000h, 000h, 000h, 066h, 0C1h, 0C2h, 004h, 08Bh, 0C2h, 083h, 0E0h, 00Fh
        db  08Ah, 080h, 038h, 09Bh, 000h, 000h, 0AAh, 0E2h, 0EEh, 061h, 0C3h
sub_1_072E:
        db  08Bh, 00Dh, 040h, 0C3h, 000h, 000h
loc_1_0734:
        jecxz  loc_1_0740
        cmp    word ptr [ecx + 6], ax
        je     SHORT loc_1_0740
        mov    ecx, dword ptr [ecx]
        jmp    SHORT loc_1_0734
loc_1_0740:
        ret
        db  000h, 000h, 000h
Fatal_Memory_Error:
        push   80001h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   esi, eax
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
Fatal_Error_Handler:
        db  083h, 03Dh, 048h, 09Bh, 000h, 000h, 000h
        jne    SHORT loc_1_0790
        db  08Bh, 03Dh, 098h, 09Bh, 000h, 000h
        and    al, 1
        or     byte ptr [edi + 1ah], al
        test   esi, esi
        je     SHORT loc_1_077D
        db  089h, 035h, 048h, 09Bh, 000h, 000h
        jmp    SHORT loc_1_0790
loc_1_077D:
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 00Dh, 048h, 09Bh, 000h, 000h
loc_1_0790:
        db  066h, 0C7h, 005h, 0F8h, 0E3h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  080h, 03Dh, 069h, 0F6h, 000h, 000h, 004h
        jb     SHORT loc_1_07C7
        db  08Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        je     SHORT loc_1_07C7
        mov    eax, dword ptr [edi + 5ch]
        db  0C7h, 040h, 008h, 05Dh, 007h, 000h, 000h
        jmp    NEAR PTR sub_1_8898
loc_1_07C7:
        cli
        db  0BEh, 0C4h, 0F6h, 000h, 000h
        mov    dword ptr [esi + 10h], edi
        mov    dword ptr [esi + 0ch], 186a0h
        db  080h, 03Dh, 069h, 0F6h, 000h, 000h, 004h
        jb     SHORT loc_1_07E3
        mov    ebp, dword ptr [edi + 10h]
loc_1_07E3:
        db  0FEh, 005h, 04Ch, 09Bh, 000h, 000h
        db  080h, 03Dh, 04Ch, 09Bh, 000h, 000h, 001h
        db  00Fh, 085h, 091h, 003h, 0FFh, 0FFh
        call   sub_1_0831
        db  083h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h
        je     SHORT loc_1_0821
        sti
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
loc_1_080B:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_0821
        db  0CDh, 020h, 02Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_080B
loc_1_0821:
        db  080h, 03Dh, 005h, 0BFh, 000h, 000h, 000h
        je     SHORT loc_1_082C
        int    1
loc_1_082C:
        db  0E9h, 056h, 003h, 0FFh, 0FFh
sub_1_0831:
        db  08Bh, 035h, 048h, 09Bh, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_0859
        pushad
        mov    ecx, 100h
        db  08Bh, 03Dh, 098h, 09Bh, 000h, 000h
        lea    edi, [edi + 1dh]
        cld
loc_1_084B:
        db  0E8h, 0B0h, 0F7h, 0FEh, 0FFh
loc_1_0850:
        lodsb
        stosb
        test   al, al
        je     SHORT loc_1_0858
        loop   loc_1_0850
loc_1_0858:
        popad
loc_1_0859:
        ret
        db  000h, 000h
sub_1_085C:
        pushad
        movzx  ecx, word ptr [esi + eax*2 + 0ah]
        cld
        add    ecx, dword ptr [esi + 4]
        xor    ebx, ebx
        mov    esi, ecx
loc_1_086A:
        lodsb
        or     al, al
        je     NEAR PTR loc_1_0953
        cmp    al, 25h
        je     SHORT loc_1_087F
        inc    ebx
        test   edi, edi
        je     SHORT loc_1_087D
        stosb
loc_1_087D:
        jmp    SHORT loc_1_086A
loc_1_087F:
        xor    ecx, ecx
        mov    edx, dword ptr [ebp]
        lodsb
        or     al, al
        je     NEAR PTR loc_1_0953
        cmp    al, 31h
        jb     SHORT loc_1_08BF
        cmp    al, 39h
        ja     SHORT loc_1_08BF
        movsx  eax, al
        mov    edx, dword ptr [ebp + eax*4 - 0c4h]
        lodsb
        or     al, al
        je     NEAR PTR loc_1_0953
        cmp    al, 31h
        jb     SHORT loc_1_08BF
        cmp    al, 39h
        ja     SHORT loc_1_08BF
        movsx  ecx, al
        sub    ecx, 30h
        lodsb
        or     al, al
        je     NEAR PTR loc_1_0953
loc_1_08BF:
        cmp    al, 78h
        jne    SHORT loc_1_08F1
        jecxz  loc_1_08CE
        mov    eax, edx
        call   sub_1_0977
        jmp    SHORT loc_1_086A
loc_1_08CE:
        xor    eax, eax
        mov    ecx, 4
        rol    edx, 10h
loc_1_08D8:
        rol    edx, 4
        mov    al, dl
        test   ax, 0ff0fh
        je     SHORT loc_1_08EA
        call   sub_1_098A
        inc    ah
loc_1_08EA:
        loop   loc_1_08D8
        jmp    NEAR PTR loc_1_086A
loc_1_08F1:
        cmp    al, 75h
        jne    SHORT loc_1_0932
        mov    eax, edx
        aam    0ah
        add    al, 30h
        mov    cl, al
        mov    al, ah
        xor    ah, ah
        aam    0ah
        add    ax, 3030h
        mov    edx, eax
        cmp    dh, 30h
        jne    SHORT loc_1_0915
        cmp    dl, 30h
        jne    SHORT loc_1_091D
        jmp    SHORT loc_1_0925
loc_1_0915:
        mov    al, dh
        inc    ebx
        test   edi, edi
        je     SHORT loc_1_091D
        stosb
loc_1_091D:
        mov    al, dl
        inc    ebx
        test   edi, edi
        je     SHORT loc_1_0925
        stosb
loc_1_0925:
        mov    al, cl
        inc    ebx
        test   edi, edi
        je     SHORT loc_1_092D
        stosb
loc_1_092D:
        jmp    NEAR PTR loc_1_086A
loc_1_0932:
        cmp    al, 73h
        jne    SHORT loc_1_094E
        mov    ah, al
        push   esi
        mov    esi, edx
loc_1_093B:
        lodsb
        cmp    al, 0
        je     SHORT loc_1_0948
        inc    ebx
        test   edi, edi
        je     SHORT loc_1_0946
        stosb
loc_1_0946:
        jmp    SHORT loc_1_093B
loc_1_0948:
        pop    esi
        jmp    NEAR PTR loc_1_086A
loc_1_094E:
        jmp    NEAR PTR loc_1_086A
loc_1_0953:
        inc    ebx
        test   edi, edi
        je     SHORT loc_1_0959
        stosb
loc_1_0959:
        mov    dword ptr [esp + 10h], ebx
        popad
        ret
loc_1_095F:
        pushad
        mov    edi, dword ptr [esp + 24h]
        mov    esi, dword ptr [esp + 28h]
        mov    eax, dword ptr [esp + 2ch]
        lea    ebp, [esp + 30h]
        call   sub_1_085C
        popad
        ret
sub_1_0977:
        shl    ecx, 2
        ror    eax, cl
        shr    ecx, 2
loc_1_097F:
        rol    eax, 4
        call   sub_1_098A
        loop   loc_1_097F
        ret
sub_1_098A:
        push   eax
        and    eax, 0fh
        db  00Fh, 0B6h, 080h, 050h, 09Bh, 000h, 000h
        cld
        inc    ebx
        test   edi, edi
        je     SHORT loc_1_099C
        stosb
loc_1_099C:
        pop    eax
        ret
loc_1_099E:
        pushad
        xor    edi, edi
        mov    esi, dword ptr [esp + 24h]
        movzx  eax, word ptr [esp + 28h]
        lea    ebp, [esp + 2ch]
        call   sub_1_085C
        add    ebx, 0fh
        and    ebx, 0fffffffch
        mov    dword ptr [esp + 1ch], ebx
        popad
        sub    esp, eax
        pushad
        mov    ecx, dword ptr [esp + eax + 20h]
        mov    dword ptr [esp + 20h], ecx
        mov    edx, dword ptr [esp + eax + 28h]
        mov    ebx, edx
        shr    edx, 10h
        add    edx, eax
        mov    dword ptr [esp + 24h], edx
        lea    edi, [esp + 2ch]
        mov    dword ptr [esp + 1ch], edi
        mov    esi, dword ptr [esp + eax + 24h]
        lea    ebp, [esp + eax + 2ch]
        movzx  eax, bx
        call   sub_1_085C
        dec    ebx
        mov    dword ptr [esp + 28h], ebx
        popad
        ret
        db  000h
loc_1_09F8:
        db  0A1h, 0DCh, 0C5h, 000h, 000h
        xor    ecx, ecx
        ret
loc_1_0A00:
        sti
        movzx  eax, word ptr [ebp + 28h]
        mov    ecx, eax
        mov    edx, dword ptr [ebp + 24h]
        shl    eax, 4
        add    eax, edx
        cmp    eax, 0
        jne    SHORT loc_1_0A2A
        sub    ecx, 0
        db  08Bh, 014h, 0CDh, 070h, 09Bh, 000h, 000h
        db  0FFh, 024h, 0CDh, 06Ch, 09Bh, 000h, 000h
loc_1_0A2A:
        shl    ecx, 10h
        or     edx, ecx
        db  08Bh, 03Dh, 070h, 09Bh, 000h, 000h
        db  08Bh, 00Dh, 078h, 09Bh, 000h, 000h
        add    ecx, ecx
        std
        repne scasd
        cld
        jne    SHORT loc_1_0A59
        mov    esi, eax
        mov    eax, edx
        movzx  ecx, word ptr [edi + 8]
        db  08Bh, 014h, 0CDh, 070h, 09Bh, 000h, 000h
        db  0FFh, 024h, 0CDh, 06Ch, 09Bh, 000h, 000h
loc_1_0A59:
        mov    esi, 18h
        db  0FFh, 025h, 0FCh, 0E3h, 000h, 000h
Allocate_V86_Call_Back:
        pushad
        db  0A1h, 074h, 09Bh, 000h, 000h
        db  03Bh, 005h, 07Ch, 09Bh, 000h, 000h
        ja     SHORT loc_1_0AB1
        lea    edi, [eax*8]
        db  003h, 03Dh, 06Ch, 09Bh, 000h, 000h
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi], esi
        db  0FFh, 005h, 074h, 09Bh, 000h, 000h
        db  083h, 005h, 064h, 09Bh, 000h, 000h, 002h
        popad
        db  066h, 0A1h, 062h, 09Bh, 000h, 000h
        shl    eax, 10h
        db  066h, 0A1h, 060h, 09Bh, 000h, 000h
        db  066h, 0FFh, 005h, 062h, 09Bh, 000h, 000h
        db  066h, 083h, 02Dh, 060h, 09Bh, 000h, 000h, 010h
        ret
loc_1_0AB1:
        popad
        stc
        sbb    eax, eax
        ret
Allocate_PM_Call_Back:
        pushad
        db  0A1h, 074h, 09Bh, 000h, 000h
        db  03Bh, 005h, 07Ch, 09Bh, 000h, 000h
        ja     SHORT loc_1_0AFE
        lea    edi, [eax*8]
        db  003h, 03Dh, 06Ch, 09Bh, 000h, 000h
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], edx
        popad
        db  0A1h, 064h, 09Bh, 000h, 000h
        or     eax, 3b0000h
        db  0FFh, 005h, 074h, 09Bh, 000h, 000h
        db  083h, 005h, 064h, 09Bh, 000h, 000h, 002h
        db  066h, 0FFh, 005h, 062h, 09Bh, 000h, 000h
        db  066h, 083h, 02Dh, 060h, 09Bh, 000h, 000h, 010h
        ret
loc_1_0AFE:
        popad
        stc
        sbb    eax, eax
        ret
        db  090h
loc_1_0B04:
        sub    esp, 4
        cld
        pushad
        mov    ebp, esp
        mov    word ptr [ebp + 3ch], ds
        mov    word ptr [ebp + 38h], es
        mov    word ptr [ebp + 40h], fs
        mov    word ptr [ebp + 44h], gs
        cmp    word ptr [esp + 28h], 3bh
        jne    SHORT loc_1_0B5D
        mov    ax, 30h
        mov    ds, eax
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    es, eax
        mov    fs, eax
        mov    gs, eax
        mov    eax, dword ptr [ebp + 24h]
        sub    eax, 2
        mov    edi, dword ptr [ebx - 20h]
        mov    dword ptr [ebp + 24h], eax
loc_1_0B3D:
        xchg   dword ptr [edi + 4ch], esp
        sti
        db  068h, 000h, 003h, 000h, 000h
        cmp    ah, 2
        jb     NEAR PTR loc_1_1666
        db  08Bh, 014h, 085h, 070h, 093h, 000h, 000h
        db  0FFh, 024h, 085h, 06Ch, 093h, 000h, 000h
loc_1_0B5D:
        mov    esi, 0c0h
        jmp    NEAR PTR loc_1_02C0
        db  090h
Call_When_VM_Returns:
        pushad
        mov    edi, esi
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  0BEh, 088h, 09Bh, 000h, 000h
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_0B81
        add    esi, 4
loc_1_0B81:
        mov    ecx, eax
        mov    esi, dword ptr [esi]
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     SHORT loc_1_0BFA
        xchg   ecx, eax
loc_1_0B8E:
        mov    ebp, dword ptr [ebx + 8]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 0ch], edx
        mov    dword ptr [ecx + 8], edi
        db  08Bh, 035h, 098h, 0F6h, 000h, 000h
        mov    dword ptr [ecx + 18h], esi
        db  08Bh, 035h, 084h, 09Bh, 000h, 000h
        xchg   ecx, eax
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        xchg   ecx, eax
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_0BDF
        jns    SHORT loc_1_0BBA
        neg    eax
loc_1_0BBA:
        mov    edx, ecx
        db  0BEh, 094h, 00Ch, 000h, 000h
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        je     SHORT loc_1_0BD1
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        jmp    SHORT loc_1_0BDF
loc_1_0BD1:
        push   edi
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  0CDh, 020h, 0FDh, 000h, 001h, 000h
        pop    edi
loc_1_0BDF:
        mov    dword ptr [ecx + 4], esi
        movzx  eax, word ptr [ecx + 10h]
        xchg   dword ptr [ebp + 24h], eax
        mov    dword ptr [ecx + 14h], eax
        mov    ax, word ptr [ecx + 12h]
        xchg   word ptr [ebp + 28h], ax
        mov    word ptr [ecx + 1ch], ax
        popad
        ret
loc_1_0BFA:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        test   eax, eax
        je     SHORT loc_1_0C38
        mov    ecx, eax
        mov    byte ptr [ecx + 1eh], 0
        mov    edx, eax
        db  0BEh, 040h, 00Ch, 000h, 000h
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_0C22
        mov    byte ptr [ecx + 1eh], 20h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jmp    SHORT loc_1_0C28
loc_1_0C22:
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
loc_1_0C28:
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 14h]
        jmp    NEAR PTR loc_1_0B8E
loc_1_0C38:
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
        mov    eax, eax
loc_1_0C40:
        db  08Bh, 035h, 084h, 09Bh, 000h, 000h
        mov    eax, edx
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    ax, word ptr [edx + 1ch]
        mov    word ptr [ebp + 28h], ax
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [ebp + 24h], eax
        mov    ecx, dword ptr [edx + 8]
        jecxz  loc_1_0C79
        xor    esi, esi
        xchg   dword ptr [edx + 4], esi
        push   edx
        mov    edx, dword ptr [edx + 0ch]
        test   esi, esi
        je     SHORT loc_1_0C76
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        test   ecx, ecx
loc_1_0C76:
        call   ecx
        pop    edx
loc_1_0C79:
        mov    eax, edx
        db  0BEh, 088h, 09Bh, 000h, 000h
        test   byte ptr [eax + 1eh], 20h
        je     SHORT loc_1_0C8C
        add    esi, 4
loc_1_0C8C:
        mov    esi, dword ptr [esi]
        db  0CDh, 020h, 09Eh, 080h, 001h, 000h
loc_1_0C94:
        xor    ecx, ecx
        mov    dword ptr [edx + 4], ecx
        cmp    dword ptr [edx], ecx
        jl     SHORT loc_1_0CA6
        xchg   dword ptr [edx + 8], ecx
loc_1_0CA0:
        mov    edx, dword ptr [edx + 0ch]
        stc
        jmp    ecx
loc_1_0CA6:
        mov    ecx, dword ptr [edx + 8]
        jmp    SHORT loc_1_0CA0
sub_1_0CAB:
        pushad
        db  08Bh, 035h, 084h, 09Bh, 000h, 000h
        mov    edx, esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_1_0CBA:
        test   eax, eax
        je     SHORT loc_1_0CF1
        mov    ecx, eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        cmp    dword ptr [ecx + 18h], edi
        jne    SHORT loc_1_0CBA
        xchg   ecx, eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0BEh, 088h, 09Bh, 000h, 000h
        test   byte ptr [eax + 1eh], 20h
        je     SHORT loc_1_0CE3
        add    esi, 4
loc_1_0CE3:
        mov    esi, dword ptr [esi]
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    esi, edx
        mov    eax, ecx
        jmp    SHORT loc_1_0CBA
loc_1_0CF1:
        popad
        ret
        db  090h
Simulate_Push:
        push   ebx
        push   ecx
        push   edx
        push   ebp
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        movzx  ecx, word ptr [ebp + 34h]
        mov    edx, dword ptr [ebp + 30h]
        test   dword ptr [ebx], 20h
        jne    SHORT loc_1_0D26
        movzx  edx, dx
        sub    dx, 2
        shl    ecx, 4
        mov    word ptr [edx + ecx], ax
loc_1_0D1E:
        mov    dword ptr [ebp + 30h], edx
        pop    ebp
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_0D26:
        push   fs
        mov    fs, cx
        test   dword ptr [ebx], 180h
        jne    SHORT loc_1_0D42
        movzx  edx, dx
        sub    dx, 2
        mov    word ptr fs:[edx], ax
        pop    fs
        jmp    SHORT loc_1_0D1E
loc_1_0D42:
        lsl    ecx, ecx
        cmp    ecx, 0ffffh
        ja     SHORT loc_1_0D50
        movzx  edx, dx
loc_1_0D50:
        sub    edx, 4
        mov    dword ptr fs:[edx], eax
        pop    fs
        jmp    SHORT loc_1_0D1E
        db  08Bh, 0C0h
Simulate_Pop:
        push   ebx
        push   ecx
        push   edx
        push   ebp
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        movzx  ecx, word ptr [ebp + 34h]
        mov    edx, dword ptr [ebp + 30h]
        test   dword ptr [ebx], 20h
        jne    SHORT loc_1_0D8E
        movzx  edx, dx
        shl    ecx, 4
        movzx  eax, word ptr [edx + ecx]
        add    dx, 2
loc_1_0D86:
        mov    dword ptr [ebp + 30h], edx
        pop    ebp
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_0D8E:
        push   fs
        mov    fs, cx
        test   dword ptr [ebx], 180h
        jne    SHORT loc_1_0DAA
        movzx  edx, dx
        movzx  eax, word ptr fs:[edx]
        add    dx, 2
        pop    fs
        jmp    SHORT loc_1_0D86
loc_1_0DAA:
        lsl    ecx, ecx
        cmp    ecx, 0ffffh
        ja     SHORT loc_1_0DB8
        movzx  edx, dx
loc_1_0DB8:
        mov    eax, dword ptr fs:[edx]
        add    edx, 4
        pop    fs
        jmp    SHORT loc_1_0D86
        db  08Bh, 0C0h
Map_Flat:
        push   ecx
        push   esi
        push   ebp
        db  08Bh, 00Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ecx + 8]
        test   al, al
        js     SHORT loc_1_0DF8
        movzx  esi, al
        mov    esi, dword ptr [esi + ebp]
loc_1_0DDA:
        movzx  eax, ah
        mov    ax, word ptr [eax + ebp]
        test   dword ptr [ecx], 20h
        jne    SHORT loc_1_0DFC
        shl    eax, 4
        movzx  esi, si
        add    eax, esi
loc_1_0DF1:
        add    eax, dword ptr [ecx + 4]
        pop    ebp
        pop    esi
        pop    ecx
        ret
loc_1_0DF8:
        xor    esi, esi
        jmp    SHORT loc_1_0DDA
loc_1_0DFC:
        test   dword ptr [ecx], 180h
        jne    SHORT loc_1_0E07
        movzx  esi, si
loc_1_0E07:
        push   edx
        push   ecx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        pop    edx
        cmp    eax, -1
        je     SHORT loc_1_0E26
        add    eax, esi
        cmp    eax, 110000h
        jb     SHORT loc_1_0DF1
loc_1_0E26:
        pop    ebp
        pop    esi
        pop    ecx
        ret
Map_Lin_To_VM_Addr:
        push   eax
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        test   dword ptr [ebx], 20h
        jne    SHORT loc_1_0E65
        cmp    eax, 110000h
        jb     SHORT loc_1_0E51
        sub    eax, dword ptr [ebx + 4]
        cmp    eax, 110000h
        jae    NEAR PTR loc_1_0F1B
loc_1_0E51:
        shl    eax, 0ch
        shr    ax, 0ch
        movzx  edx, ax
        shr    eax, 10h
        mov    ecx, eax
        jmp    NEAR PTR loc_1_0F04
loc_1_0E65:
        mov    edi, eax
        mov    esi, dword ptr [ebx - 40h]
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_0E9F
loc_1_0E72:
        cmp    dword ptr [eax], edi
        jne    SHORT loc_1_0E97
        movzx  edx, word ptr [eax + 4]
        lsl    edx, edx
        cmp    edx, ecx
        jb     SHORT loc_1_0E97
        test   dword ptr [ebx], 100h
        je     SHORT loc_1_0E91
        test   word ptr [eax + 6], 1
        je     SHORT loc_1_0E97
loc_1_0E91:
        movzx  ecx, word ptr [eax + 4]
        jmp    SHORT loc_1_0F02
loc_1_0E97:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_0E72
loc_1_0E9F:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_1_0F1B
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    esi, eax
        mov    dword ptr [esi], edi
        mov    word ptr [esi + 6], 0
        push   0
        push   0
        push   0f2h
        push   ecx
        push   edi
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        test   dword ptr [ebx], 100h
        jne    SHORT loc_1_0EE5
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        jmp    SHORT loc_1_0EF7
loc_1_0EE5:
        or     word ptr [esi + 6], 1
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
loc_1_0EF7:
        test   eax, eax
        je     SHORT loc_1_0F0A
        mov    word ptr [esi + 4], ax
        movzx  ecx, ax
loc_1_0F02:
        xor    edx, edx
loc_1_0F04:
        clc
loc_1_0F05:
        pop    edi
        pop    esi
        pop    ebx
        pop    eax
        ret
loc_1_0F0A:
        mov    eax, esi
        mov    esi, dword ptr [ebx - 40h]
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_1_0F1B:
        stc
        jmp    SHORT loc_1_0F05
loc_1_0F1E:
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 006h, 000h, 007h, 000h
        jb     SHORT loc_1_0F91
        db  0A1h, 000h, 0C7h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_0F91
        db  08Bh, 015h, 004h, 0C7h, 000h, 000h
loc_1_0F38:
        xor    ecx, ecx
        lea    edi, [eax + 5]
        jmp    SHORT loc_1_0F50
loc_1_0F3F:
        inc    ecx
        mov    esi, dword ptr [esp + 10h]
        mov    ebx, dword ptr [edi]
        sub    esi, ebx
        cmp    esi, dword ptr [edi + 4]
        jb     SHORT loc_1_0F99
        add    edi, 9
loc_1_0F50:
        cmp    cl, byte ptr [eax + 4]
        jb     SHORT loc_1_0F3F
        mov    eax, edi
        dec    edx
        jne    SHORT loc_1_0F38
        db  0CDh, 020h, 000h, 000h, 027h, 000h
        jb     SHORT loc_1_0F91
        db  0CDh, 020h, 005h, 000h, 027h, 000h
        jmp    SHORT loc_1_0F8D
loc_1_0F6A:
        mov    edi, dword ptr [eax + 17h]
        xor    ecx, ecx
        test   edi, edi
        je     SHORT loc_1_0F8B
        jmp    SHORT loc_1_0F86
loc_1_0F75:
        inc    ecx
        mov    esi, dword ptr [esp + 10h]
        mov    ebx, dword ptr [edi]
        sub    esi, ebx
        cmp    esi, dword ptr [edi + 4]
        jb     SHORT loc_1_0F9D
        add    edi, 10h
loc_1_0F86:
        cmp    ecx, dword ptr [eax + 13h]
        jb     SHORT loc_1_0F75
loc_1_0F8B:
        mov    eax, dword ptr [eax]
loc_1_0F8D:
        or     eax, eax
        jne    SHORT loc_1_0F6A
loc_1_0F91:
        xor    edx, edx
        mov    edi, dword ptr [esp + 14h]
        jmp    SHORT loc_1_0FE8
loc_1_0F99:
        mov    esi, dword ptr [eax]
        jmp    SHORT loc_1_0FA0
loc_1_0F9D:
        mov    esi, dword ptr [eax + 5]
loc_1_0FA0:
        push   ebx
        push   ecx
        mov    edi, dword ptr [esp + 1ch]
        cmp    esi, 0
        jge    SHORT loc_1_0FC5
        mov    ecx, dword ptr [esi + 0ch]
        mov    esi, dword ptr [esi + 10h]
        mov    dword ptr [edi], ecx
        mov    dword ptr [edi + 4], esi
        push   8
        pop    ecx
loc_1_0FB9:
        cmp    byte ptr [ecx + edi - 1], 20h
        loope  loc_1_0FB9
        inc    ecx
        add    edi, ecx
        jmp    SHORT loc_1_0FD6
loc_1_0FC5:
        push   0dh
        push   dword ptr [eax + 0bh]
        push   edi
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    edi, edx
loc_1_0FD6:
        mov    al, 28h
        stosb
        pop    eax
        mov    cl, 2
        call   sub_1_0977
        mov    eax, 202b2029h
        stosd
        pop    edx
loc_1_0FE8:
        mov    eax, dword ptr [esp + 10h]
        sub    eax, edx
        push   8
        pop    ecx
        call   sub_1_0977
        mov    byte ptr [edi], 0
        xchg   edx, eax
        mov    edx, dword ptr [esp + 10h]
        db  081h, 0FAh, 032h, 01Ah, 000h, 000h
        jae    SHORT loc_1_100D
        db  081h, 0FAh, 000h, 014h, 000h, 000h
        cmc
loc_1_100D:
        sbb    edx, edx
        pop    edi
        pop    esi
        pop    ebx
        ret
Get_Cur_VM_Handle:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        ret
Test_Cur_VM_Handle:
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        ret
Get_Sys_VM_Handle:
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        ret
Test_Sys_VM_Handle:
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        ret
        db  090h
Validate_VM_Handle:
        push   ecx
        db  08Bh, 00Dh, 014h, 0F7h, 000h, 000h
loc_1_1037:
        jecxz  loc_1_1042
        cmp    ecx, ebx
        je     SHORT loc_1_1045
        mov    ecx, dword ptr [ecx + 2ch]
        jmp    SHORT loc_1_1037
loc_1_1042:
        pop    ecx
        stc
        ret
loc_1_1045:
        cmp    dword ptr [ecx + 10h], 62634d56h
        jne    SHORT loc_1_1042
        pop    ecx
        clc
        ret
loc_1_1051:
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        ret
loc_1_1058:
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        ret
loc_1_105F:
        db  08Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        ret
loc_1_1066:
        db  03Bh, 03Dh, 09Ch, 0F6h, 000h, 000h
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_1070:
        push   ecx
        push   edx
        db  08Bh, 00Dh, 014h, 0F7h, 000h, 000h
loc_1_1078:
        mov    edx, dword ptr [ecx - 1ch]
        test   edx, edx
        je     SHORT loc_1_108A
loc_1_107F:
        cmp    edx, edi
        je     SHORT loc_1_1095
        mov    edx, dword ptr [edx + 40h]
        test   edx, edx
        jne    SHORT loc_1_107F
loc_1_108A:
        mov    ecx, dword ptr [ecx + 2ch]
        test   ecx, ecx
        jne    SHORT loc_1_1078
        pop    edx
        pop    ecx
        stc
        ret
loc_1_1095:
        pop    edx
        pop    ecx
        clc
        ret
loc_1_1099:
        lea    edi, [ebx]
        ret
loc_1_10A0:
        test   dword ptr [edi], 100000h
        ret
Test_Debug_Installed:
        db  0F6h, 005h, 005h, 0BFh, 000h, 000h, 0FFh
        ret
Test_DBCS_Lead_Byte:
        stc
        ret
        db  000h, 000h, 000h
loc_1_10B4:
        sti
        movzx  esi, word ptr [ebp + 28h]
        mov    ecx, dword ptr [ebp + 24h]
        shl    esi, 4
        mov    dword ptr [edi - 58h], ecx
        add    esi, ecx
        mov    cx, word ptr [esi]
        movzx  edi, cl
        db  0FFh, 024h, 0BDh, 008h, 0BFh, 000h, 000h
loc_1_10D1:
        sti
        mov    fs, word ptr [ebp + 28h]
        mov    esi, dword ptr [ebp + 24h]
        mov    dword ptr [edi - 58h], esi
loc_1_10DC:
        mov    cx, word ptr fs:[esi]
        movzx  edi, cl
        cmp    cl, 0ceh
        ja     SHORT loc_1_10F4
        cmp    cl, 0cch
        jb     SHORT loc_1_10F4
        db  0FFh, 024h, 0BDh, 0D8h, 0BFh, 000h, 000h
loc_1_10F4:
        db  0FFh, 024h, 0BDh, 008h, 0BFh, 000h, 000h
loc_1_10FB:
        db  08Bh, 035h, 098h, 0F6h, 000h, 000h
        mov    esi, dword ptr [esi - 58h]
        mov    dword ptr [ebp + 24h], esi
        push   34h
        pop    esi
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_1115
        db  0FFh, 025h, 000h, 0E4h, 000h, 000h
loc_1_1115:
        db  0FFh, 025h, 008h, 0E4h, 000h, 000h
loc_1_111B:
        or     edi, 800h
        jmp    SHORT loc_1_1173
loc_1_1123:
        or     edi, 8000h
        jmp    SHORT loc_1_1173
loc_1_112B:
        or     edi, 1000h
        jmp    SHORT loc_1_1173
loc_1_1133:
        or     edi, 400h
        jmp    SHORT loc_1_1173
loc_1_113B:
        or     edi, 200h
        jmp    SHORT loc_1_1173
loc_1_1143:
        or     edi, 100h
        jmp    SHORT loc_1_1173
loc_1_114B:
        or     edi, 20000h
        jmp    SHORT loc_1_1173
loc_1_1153:
        or     edi, 40000h
        jmp    SHORT loc_1_1173
loc_1_115B:
        or     edi, 10000h
        jmp    SHORT loc_1_1173
loc_1_1163:
        or     edi, 4000h
        jmp    SHORT loc_1_1173
loc_1_116B:
        or     edi, 2000h
        jmp    SHORT loc_1_1173
loc_1_1173:
        inc    dword ptr [ebp + 24h]
        inc    esi
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_1181
        mov    cx, word ptr [esi]
        jmp    SHORT loc_1_1185
loc_1_1181:
        mov    cx, word ptr fs:[esi]
loc_1_1185:
        push   ecx
        movzx  ecx, cl
        db  08Bh, 00Ch, 08Dh, 008h, 0BFh, 000h, 000h
        xchg   dword ptr [esp], ecx
        ret
sub_1_1194:
        push   eax
        push   edx
        push   ecx
        push   1
        push   0ch
        call   sub_1_662C
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_11B0
        lea    ecx, [eax + 4]
        pop    dword ptr [ecx + 4]
        clc
loc_1_11AD:
        pop    edx
        pop    eax
        ret
loc_1_11B0:
        pop    ecx
        stc
        jmp    SHORT loc_1_11AD
List_Create:
        pushad
        push   1
        push   10h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_1224
        mov    dword ptr [esp + 4], eax
        mov    edx, dword ptr [esp + 18h]
        add    edx, 4
        mov    dword ptr [eax + 0ch], edx
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 8], ecx
        test   cl, 2
        jne    SHORT loc_1_1212
        db  08Bh, 035h, 034h, 0C3h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_11FA
loc_1_11ED:
        cmp    dword ptr [eax + 4], edx
        je     SHORT loc_1_120B
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_11ED
loc_1_11FA:
        mov    ecx, edx
        call   sub_1_1194
        jb     SHORT loc_1_1215
        mov    eax, ecx
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
loc_1_120B:
        mov    esi, dword ptr [esp + 4]
        mov    dword ptr [esi + 4], eax
loc_1_1212:
        clc
        popad
        ret
loc_1_1215:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_1_1224:
        stc
        popad
        ret
List_Destroy:
        push   eax
        push   ecx
        push   edx
loc_1_122A:
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     SHORT loc_1_123A
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        jmp    SHORT loc_1_122A
loc_1_123A:
        push   eax
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
        ret
List_Allocate:
        push   ecx
        mov    ecx, dword ptr [esi + 4]
        jecxz  loc_1_1282
        pushfd
        cli
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT Unhook_V86_Int_Chain
        push   dword ptr [eax - 4]
        pop    dword ptr [ecx]
        popfd
        clc
        pop    ecx
        ret
Unhook_V86_Int_Chain:
        push   edx
        push   eax
        push   dword ptr [esi + 0ch]
        call   sub_1_662C
        add    esp, 8
        pop    edx
        test   eax, eax
        je     SHORT loc_1_1278
        add    eax, 4
        popfd
        clc
        pop    ecx
        ret
loc_1_1278:
        test   byte ptr [esi + 8], 4
        je     SHORT loc_1_12B0
        popfd
        pop    ecx
        stc
        ret
loc_1_1282:
        push   edx
        xor    eax, eax
        test   byte ptr [esi + 8], 8
        je     SHORT loc_1_128E
        or     ah, 1
loc_1_128E:
        push   eax
        push   dword ptr [esi + 0ch]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_12A6
        add    eax, 4
        pop    edx
        pop    ecx
        clc
        ret
loc_1_12A6:
        test   byte ptr [esi + 8], 4
        je     SHORT loc_1_12B0
        pop    edx
        pop    ecx
        stc
        ret
loc_1_12B0:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 10007h
        db  0BAh, 08Ch, 0D9h, 000h, 000h
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        db  0CDh, 020h, 02Fh, 080h, 001h, 000h
        nop
List_Attach:
        push   dword ptr [esi]
        pop    dword ptr [eax - 4]
        mov    dword ptr [esi], eax
        ret
List_Insert:
        jecxz  List_Attach
        push   dword ptr [ecx - 4]
        pop    dword ptr [eax - 4]
        mov    dword ptr [ecx - 4], eax
        ret
List_Attach_Tail:
        push   ecx
        push   esi
        lea    esi, [esi + 4]
loc_1_12E9:
        mov    ecx, dword ptr [esi - 4]
        jecxz  loc_1_12F2
        mov    esi, ecx
        jmp    SHORT loc_1_12E9
loc_1_12F2:
        mov    dword ptr [esi - 4], eax
        mov    dword ptr [eax - 4], ecx
        pop    esi
        pop    ecx
        ret
        db  090h
List_Remove:
        push   ecx
        push   esi
        lea    esi, [esi + 4]
loc_1_1301:
        mov    ecx, dword ptr [esi - 4]
        cmp    ecx, eax
        je     SHORT loc_1_1311
        mov    esi, ecx
        test   ecx, ecx
        jne    SHORT loc_1_1301
        stc
        jmp    SHORT loc_1_1318
loc_1_1311:
        mov    ecx, dword ptr [eax - 4]
        mov    dword ptr [esi - 4], ecx
        clc
loc_1_1318:
        pop    esi
        pop    ecx
        ret
        db  090h
List_Deallocate:
        push   ecx
        mov    ecx, dword ptr [esi + 4]
        jecxz  loc_1_132E
        pushfd
        cli
        push   dword ptr [ecx]
        mov    dword ptr [ecx], eax
        pop    dword ptr [eax - 4]
        popfd
        pop    ecx
        ret
loc_1_132E:
        push   edx
        sub    eax, 4
        push   ecx
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        pop    ecx
        ret
List_Get_First:
        mov    eax, dword ptr [esi]
        cmp    eax, 0
        ret
        db  090h
List_Get_Next:
        mov    eax, dword ptr [eax - 4]
        cmp    eax, 0
        ret
        db  090h
List_Remove_First:
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_1_135B
        push   dword ptr [eax - 4]
        pop    dword ptr [esi]
loc_1_135B:
        ret
System_Control:
        stc
        pushfd
        pushad
        xor    ecx, ecx
        mov    dword ptr [esp + 0ch], ecx
        cmp    eax, 31h
        je     SHORT loc_1_139B
        cmp    eax, 32h
        je     SHORT loc_1_139B
        cmp    eax, 21h
        ja     SHORT loc_1_1384
        cmp    al, 1dh
        jae    SHORT loc_1_139B
        cmp    al, 16h
        jne    SHORT loc_1_1384
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_139B
loc_1_1384:
        inc    dword ptr [esp + 0ch]
        cmp    eax, 8
        je     SHORT loc_1_1395
        cmp    eax, 0bh
        je     SHORT loc_1_1395
        add    ecx, 5
loc_1_1395:
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_1_139B:
        cmp    eax, 33h
        jae    SHORT loc_1_140D
        xor    ecx, ecx
        inc    ecx
        db  0FFh, 014h, 085h, 044h, 0C3h, 000h, 000h
        jb     SHORT loc_1_13DD
        db  038h, 080h, 0A8h, 0C5h, 000h, 000h
        je     SHORT loc_1_13C7
        call   sub_1_146B
        jb     SHORT loc_1_13DD
        db  08Ah, 080h, 0A8h, 0C5h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_13CE
loc_1_13C7:
        xor    ecx, ecx
        call   sub_1_14BB
loc_1_13CE:
        mov    eax, dword ptr [esp + 1ch]
        db  0FFh, 014h, 085h, 010h, 0C4h, 000h, 000h
        jae    SHORT loc_1_13E6
        xor    ecx, ecx
loc_1_13DD:
        db  0FFh, 014h, 085h, 0DCh, 0C4h, 000h, 000h
        jmp    SHORT loc_1_13F5
loc_1_13E6:
        cmp    eax, 1dh
        je     SHORT loc_1_1404
        cmp    eax, 7
        je     SHORT loc_1_1407
loc_1_13F0:
        and    byte ptr [esp + 20h], 0feh
loc_1_13F5:
        dec    dword ptr [esp + 0ch]
        js     SHORT loc_1_1401
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_1_1401:
        popad
        popfd
        ret
loc_1_1404:
        mov    dword ptr [esp], edi
loc_1_1407:
        mov    dword ptr [esp + 10h], ebx
        jmp    SHORT loc_1_13F0
loc_1_140D:
        xor    ecx, ecx
        call   sub_1_146B
        jmp    SHORT loc_1_13F5
loc_1_1416:
        jmp    dword ptr [ecx + 18h]
loc_1_1419:
        or     edx, edx
        jne    SHORT loc_1_1423
        db  0CDh, 020h, 031h, 000h, 001h, 000h
loc_1_1423:
        clc
        ret
loc_1_1425:
        pushad
        cli
        db  0E8h, 050h, 016h, 0FFh, 0FFh
        sti
        db  0E8h, 052h, 007h, 0FFh, 0FFh
        popad
        clc
        ret
GetSetDetailedVMError:
        push   esi
        db  0BEh, 038h, 0C3h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_1445
        lea    esi, [ebx + 1a4h]
loc_1_1445:
        jecxz  loc_1_145F
        test   eax, eax
        js     SHORT loc_1_145B
        mov    dword ptr [esi + 4], edx
        je     SHORT loc_1_1459
        test   dword ptr [esi + 4], 7fffffffh
        je     SHORT loc_1_145B
loc_1_1459:
        mov    dword ptr [esi], eax
loc_1_145B:
        or     dword ptr [esi], eax
        jmp    SHORT loc_1_1466
loc_1_145F:
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        or     eax, eax
loc_1_1466:
        pop    esi
        ret
loc_1_1468:
        clc
        popad
        ret
sub_1_146B:
        pushad
        db  0B9h, 040h, 0C3h, 000h, 000h
loc_1_1471:
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_1468
        cmp    ecx, dword ptr [esp + 18h]
        je     SHORT loc_1_1468
        mov    eax, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 4]
        mov    edi, dword ptr [esp]
        mov    ebp, dword ptr [esp + 8]
        push   ecx
        call   dword ptr [ecx + 18h]
        pop    ecx
        jae    SHORT loc_1_1471
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, 33h
        jae    SHORT loc_1_14B1
        db  081h, 03Ch, 085h, 0DCh, 0C4h, 000h, 000h, 023h, 014h, 000h, 000h
        je     SHORT loc_1_1471
loc_1_14B1:
        mov    dword ptr [esp + 18h], ecx
        popad
loc_1_14B6:
        stc
        ret
loc_1_14B8:
        clc
        popad
        ret
sub_1_14BB:
        pushad
        or     ecx, ecx
        jg     SHORT loc_1_14B8
        jmp    SHORT loc_1_14CB
loc_1_14C2:
        db  0B9h, 080h, 0D9h, 000h, 000h
        jmp    SHORT loc_1_14D5
loc_1_14C9:
        mov    ecx, dword ptr [ecx]
loc_1_14CB:
        jecxz  loc_1_14C2
        cmp    word ptr [ecx + 4], 400h
        jb     SHORT loc_1_14C9
loc_1_14D5:
        mov    ecx, dword ptr [ecx + 3ch]
        jecxz  loc_1_14B8
        mov    eax, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 4]
        mov    edi, dword ptr [esp]
        mov    ebp, dword ptr [esp + 8]
        push   ecx
        call   dword ptr [ecx + 18h]
        pop    ecx
        jmp    SHORT loc_1_14D5
VMM_Control:
        db  0F7h, 005h, 0FEh, 014h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_1_150D
        cmp    eax, 14h
        db  00Fh, 084h, 073h, 028h, 0FFh, 0FFh
loc_1_150D:
        clc
        ret
        db  000h
loc_1_1510:
        inc    dword ptr [ebp + 24h]
        mov    eax, 3
        jmp    SHORT loc_1_1533
        db  08Bh, 0C0h
loc_1_151C:
        inc    dword ptr [ebp + 24h]
        mov    eax, 4
        test   byte ptr [ebp + 2dh], 8
        jne    SHORT loc_1_1533
        ret
        db  090h
loc_1_152C:
        add    dword ptr [ebp + 24h], 2
        movzx  eax, ch
loc_1_1533:
        db  0FFh, 024h, 085h, 00Ch, 0CFh, 000h, 000h
        db  08Bh, 0C0h
loc_1_153C:
        mov    esi, ebx
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        call   sub_1_6DE8
        mov    eax, dword ptr [ecx + eax*4]
        mov    ebx, esi
sub_1_154E:
        movzx  ecx, word ptr [ebp + 30h]
        movzx  esi, word ptr [ebp + 34h]
        sub    cx, 6
        shl    esi, 4
        mov    word ptr [ebp + 30h], cx
        jb     SHORT loc_1_1583
        add    esi, ecx
        xchg   word ptr [ebp + 24h], ax
        ror    eax, 10h
        xchg   word ptr [ebp + 28h], ax
        ror    eax, 10h
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 2ch]
        mov    word ptr [esi + 4], ax
loc_1_157C:
        and    ah, 0fch
        mov    byte ptr [ebp + 2dh], ah
        ret
loc_1_1583:
        xchg   word ptr [ebp + 24h], ax
        mov    word ptr [ecx + esi], ax
        shr    eax, 10h
        add    cx, 2
        xchg   word ptr [ebp + 28h], ax
        mov    word ptr [ecx + esi], ax
        add    cx, 2
        mov    ax, word ptr [ebp + 2ch]
        mov    word ptr [ecx + esi], ax
        jmp    SHORT loc_1_157C
loc_1_15A8:
        inc    dword ptr [ebp + 24h]
        mov    eax, 3
        jmp    SHORT loc_1_15CB
        db  08Bh, 0C0h
loc_1_15B4:
        inc    dword ptr [ebp + 24h]
        mov    eax, 4
        test   byte ptr [ebp + 2dh], 8
        jne    SHORT loc_1_15CB
        ret
        db  090h
loc_1_15C4:
        add    dword ptr [ebp + 24h], 2
        movzx  eax, ch
loc_1_15CB:
        lea    esi, [eax*8]
        test   byte ptr [ebx + 1], 1
        jne    SHORT loc_1_162E
        add    esi, dword ptr [ebx - 8]
loc_1_15DB:
        movzx  ecx, word ptr [esi + 4]
        mov    edx, dword ptr [esi]
        cmp    cx, 3bh
        jne    SHORT loc_1_15F2
        lea    edi, [eax*2]
        cmp    edx, edi
        je     SHORT loc_1_166E
loc_1_15F2:
        mov    eax, dword ptr [ebp + 2ch]
        and    ax, word ptr [esi + 6]
sub_1_15F9:
        push   fs
        mov    fs, word ptr [ebp + 34h]
        test   word ptr [ebx], 180h
        jne    SHORT loc_1_1636
        movzx  esi, word ptr [ebp + 30h]
        sub    esi, 6
        mov    word ptr [ebp + 30h], si
        xchg   word ptr [ebp + 28h], cx
        shl    ecx, 10h
        mov    cx, dx
        xchg   word ptr [ebp + 24h], cx
        mov    dword ptr fs:[esi], ecx
        xchg   word ptr [ebp + 2ch], ax
        mov    word ptr fs:[esi + 4], ax
        pop    fs
        ret
loc_1_162E:
        db  08Dh, 0B6h, 00Ch, 0C7h, 000h, 000h
        jmp    SHORT loc_1_15DB
loc_1_1636:
        lar    esi, word ptr [ebp + 34h]
        test   esi, 400000h
        mov    esi, dword ptr [ebp + 30h]
        jne    SHORT loc_1_1648
        movzx  esi, si
loc_1_1648:
        sub    esi, 0ch
        mov    dword ptr [ebp + 30h], esi
        xchg   dword ptr [ebp + 24h], edx
        mov    dword ptr fs:[esi], edx
        xchg   dword ptr [ebp + 28h], ecx
        mov    dword ptr fs:[esi + 4], ecx
        xchg   dword ptr [ebp + 2ch], eax
        mov    dword ptr fs:[esi + 8], eax
        pop    fs
        clc
        ret
loc_1_1666:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        shr    eax, 1
loc_1_166E:
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_1683
        mov    ecx, eax
        shr    ecx, 3
        dec    ecx
        je     SHORT loc_1_16B8
        cmp    cl, 0dh
        je     SHORT loc_1_16B8
loc_1_1683:
        mov    ecx, esp
        db  08Bh, 015h, 098h, 0F6h, 000h, 000h
        sub    ecx, dword ptr [edx + 54h]
        shr    ecx, 8
        cmp    dword ptr [edx - 5ch], 0
        jne    SHORT loc_1_169A
        sub    ecx, 10h
loc_1_169A:
        cmp    ecx, 2
        jb     NEAR PTR loc_1_333E
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pushfd
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        popfd
        ret
loc_1_16B8:
        call   sub_1_2440
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pushfd
        call   sub_1_2604
        popfd
        ret
Get_V86_Int_Vector:
        mov    ecx, dword ptr [eax*4]
        movzx  edx, cx
        shr    ecx, 10h
        push   ebx
        mov    ebx, ecx
        or     ebx, edx
        pop    ebx
        ret
Set_V86_Int_Vector:
        mov    word ptr [eax*4], dx
        mov    word ptr [eax*4 + 2], cx
        ret
Get_PM_Int_Vector:
        push   ebx
        push   eax
        shl    eax, 3
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        test   byte ptr [ebx], 40h
        je     SHORT loc_1_170B
        cmp    dword ptr [ebx - 38h], 0
        je     SHORT loc_1_170B
        add    eax, dword ptr [ebx - 8]
        jmp    SHORT loc_1_1711
loc_1_170B:
        db  08Dh, 080h, 00Ch, 0C7h, 000h, 000h
loc_1_1711:
        movzx  ecx, word ptr [eax + 4]
        mov    edx, dword ptr [eax]
        pop    eax
        lea    ebx, [eax*2]
        cmp    edx, ebx
        jne    SHORT loc_1_1726
        cmp    ecx, 3bh
loc_1_1726:
        pop    ebx
        ret
Set_PM_Int_Vector:
        pushad
        movzx  ecx, cx
        lar    esi, ecx
        jne    SHORT loc_1_1774
        cmp    al, 1dh
        jb     SHORT loc_1_1739
        cmp    al, 1fh
        jbe    SHORT loc_1_1740
loc_1_1739:
        test   si, 800h
        je     SHORT loc_1_1774
loc_1_1740:
        lsl    esi, ecx
        cmp    edx, esi
        ja     SHORT loc_1_1774
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        test   byte ptr [ebx], 40h
        je     SHORT loc_1_1760
        cmp    dword ptr [ebx - 38h], 0
        je     SHORT loc_1_1760
        mov    esi, dword ptr [ebx - 8]
        mov    edi, dword ptr [ebx - 30h]
        jmp    SHORT loc_1_1765
loc_1_1760:
        db  0BEh, 00Ch, 0C7h, 000h, 000h
loc_1_1765:
        mov    word ptr [esi + eax*8 + 4], cx
        mov    dword ptr [esi + eax*8], edx
        je     SHORT loc_1_1774
        db  0E8h, 0A4h, 0FBh, 0FEh, 0FFh
loc_1_1774:
        popad
        ret
        db  08Bh, 0C0h
Simulate_Int:
        pushad
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  068h, 095h, 017h, 000h, 000h
        test   byte ptr [ebx], 20h
        je     NEAR PTR loc_1_1533
        jmp    NEAR PTR loc_1_15CB
loc_1_1795:
        popad
        ret
        db  090h
Build_Int_Stack_Frame:
        pushad
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_17B7
        mov    ax, cx
        shl    eax, 10h
        mov    ax, dx
        call   sub_1_154E
        popad
        ret
loc_1_17B7:
        mov    eax, dword ptr [ebp + 2ch]
        and    ax, 0fcffh
        call   sub_1_15F9
        popad
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_17C8:
        movzx  esi, word ptr [ebp + 34h]
        shl    esi, 4
        mov    eax, dword ptr [ebp + 30h]
        call   sub_1_1BA0
        popad
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_17DC:
        push   fs
        mov    fs, word ptr [ebp + 34h]
        test   word ptr [ebx], 180h
        jne    SHORT loc_1_1829
        movzx  esi, word ptr [ebp + 30h]
        add    esi, 6
        mov    eax, dword ptr fs:[esi - 6]
        mov    word ptr [ebp + 24h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 28h], ax
        mov    ax, word ptr fs:[esi - 2]
loc_1_1804:
        mov    dword ptr [ebp + 30h], esi
        mov    ecx, eax
        xchg   word ptr [ebp + 2ch], cx
        xor    ecx, eax
        test   ch, 2
        je     SHORT loc_1_1825
        test   ah, 2
        je     SHORT loc_1_1825
        and    word ptr [ebp + 2ch], 0fdffh
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_1825:
        pop    fs
        popad
        ret
loc_1_1829:
        mov    esi, dword ptr [ebp + 30h]
        mov    cx, fs
        lsl    ecx, ecx
        cmp    ecx, 0ffffh
        ja     SHORT loc_1_183D
        movzx  esi, si
loc_1_183D:
        add    esi, 0ch
        mov    ax, word ptr fs:[esi - 8]
        mov    word ptr [ebp + 28h], ax
        mov    eax, dword ptr fs:[esi - 0ch]
        mov    dword ptr [ebp + 24h], eax
        mov    ax, word ptr fs:[esi - 4]
        jmp    SHORT loc_1_1804
        db  090h
Simulate_Iret:
        pushad
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        test   byte ptr [ebx], 20h
        je     NEAR PTR loc_1_17C8
        jmp    NEAR PTR loc_1_17DC
Simulate_Far_Call:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   ebp
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        xchg   dword ptr [ebp + 24h], edx
        xchg   word ptr [ebp + 28h], cx
        test   byte ptr [ebp + 2eh], 2
        je     SHORT loc_1_18AB
        movzx  ebx, word ptr [ebp + 34h]
        shl    ebx, 4
        movzx  esi, word ptr [ebp + 30h]
        sub    si, 4
        mov    dword ptr [ebp + 30h], esi
        mov    word ptr [esi + ebx], dx
        add    si, 2
        mov    word ptr [esi + ebx], cx
        jmp    SHORT loc_1_18F0
loc_1_18AB:
        push   fs
        mov    fs, word ptr [ebp + 34h]
        mov    esi, dword ptr [ebp + 30h]
        test   word ptr [ebx], 180h
        jne    SHORT loc_1_18CF
        movzx  esi, si
        sub    esi, 4
        mov    dword ptr [ebp + 30h], esi
        shl    ecx, 10h
        mov    cx, dx
        mov    dword ptr fs:[esi], ecx
        jmp    SHORT loc_1_18EE
loc_1_18CF:
        mov    bx, fs
        lsl    ebx, ebx
        cmp    ebx, 0ffffh
        ja     SHORT loc_1_18E0
        movzx  esi, si
loc_1_18E0:
        sub    esi, 8
        mov    dword ptr [ebp + 30h], esi
        mov    word ptr fs:[esi + 4], cx
        mov    dword ptr fs:[esi], edx
loc_1_18EE:
        pop    fs
loc_1_18F0:
        pop    ebp
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
Simulate_Far_Jmp:
        push   ebp
        db  08Bh, 02Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebp + 8]
        mov    word ptr [ebp + 28h], cx
        mov    dword ptr [ebp + 24h], edx
        pop    ebp
        ret
        db  02Eh, 08Bh, 0C0h
Simulate_Far_Ret:
        push   eax
        push   ebx
        push   esi
        push   ebp
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_193E
        movzx  ebx, word ptr [ebp + 34h]
        shl    ebx, 4
        movzx  esi, word ptr [ebp + 30h]
        movzx  eax, word ptr [esi + ebx]
        mov    dword ptr [ebp + 24h], eax
        add    si, 2
        mov    ax, word ptr [esi + ebx]
        add    si, 2
        jmp    SHORT loc_1_1983
loc_1_193E:
        push   fs
        mov    fs, word ptr [ebp + 34h]
        mov    esi, dword ptr [ebp + 30h]
        test   word ptr [ebx], 180h
        jne    SHORT loc_1_1963
        movzx  esi, si
        movzx  eax, word ptr fs:[esi]
        mov    dword ptr [ebp + 24h], eax
        mov    ax, word ptr fs:[esi + 2]
        add    si, 4
        jmp    SHORT loc_1_1981
loc_1_1963:
        mov    ax, fs
        lsl    eax, eax
        cmp    eax, 0ffffh
        ja     SHORT loc_1_1973
        movzx  esi, si
loc_1_1973:
        mov    eax, dword ptr fs:[esi]
        mov    dword ptr [ebp + 24h], eax
        mov    ax, word ptr fs:[esi + 4]
        add    esi, 8
loc_1_1981:
        pop    fs
loc_1_1983:
        mov    word ptr [ebp + 28h], ax
        mov    dword ptr [ebp + 30h], esi
        pop    ebp
        pop    esi
        pop    ebx
        pop    eax
        ret
Simulate_Far_Ret_N:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        push   ebp
        db  08Bh, 02Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebp + 8]
        add    dword ptr [ebp + 30h], eax
        pop    ebp
        ret
Test_VM_Ints_Enabled:
        cmp    byte ptr [ebx - 68h], 0
        jne    SHORT loc_1_19AF
        test   byte ptr [ebp + 2dh], 2
        ret
loc_1_19AF:
        cmp    ebp, ebp
        ret
        db  08Bh, 0C0h
Call_When_VM_Ints_Enabled:
        pushad
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [edi + 10h]
        test   byte ptr [ebp + 2dh], 2
        je     SHORT loc_1_19D4
        cmp    byte ptr [ebx - 68h], 0
        jne    SHORT loc_1_19D4
        call   esi
        popad
        ret
loc_1_19D4:
        xor    eax, eax
        mov    ecx, 9
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        popad
        ret
        db  090h
Enable_VM_Ints:
        push   ebp
        db  08Bh, 02Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebp + 8]
        or     byte ptr [ebp + 2dh], 2
        pop    ebp
        ret
Disable_VM_Ints:
        push   ebp
        db  08Bh, 02Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebp + 8]
        and    byte ptr [ebp + 2dh], 0fdh
        pop    ebp
        ret
loc_1_1A04:
        mov    eax, dword ptr [ebp + 2ch]
        test   eax, 20000h
        je     SHORT loc_1_1A18
loc_1_1A0E:
        and    ah, 0fdh
loc_1_1A11:
        inc    dword ptr [ebp + 24h]
        mov    dword ptr [ebp + 2ch], eax
        ret
loc_1_1A18:
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_1_1A0E
        test   ah, 2
        je     SHORT loc_1_1A0E
        mov    ecx, dword ptr [ebp + 24h]
        sub    ecx, 1
        jb     SHORT loc_1_1A0E
        push   fs
        mov    fs, word ptr [ebp + 28h]
        cmp    byte ptr fs:[ecx], 9ch
        jne    SHORT loc_1_1A69
        mov    edi, dword ptr [ebp + 30h]
        mov    fs, word ptr [ebp + 34h]
        lar    esi, word ptr [ebp + 34h]
        test   esi, 400000h
        jne    SHORT loc_1_1A4F
        movzx  edi, di
loc_1_1A4F:
        mov    cx, word ptr fs:[edi]
        cmp    cx, ax
        jne    SHORT loc_1_1A65
        or     ch, 1
        mov    byte ptr fs:[edi + 1], ch
loc_1_1A5F:
        inc    dword ptr [ebx + 1bch]
loc_1_1A65:
        pop    fs
        jmp    SHORT loc_1_1A0E
loc_1_1A69:
        sub    ecx, 1
        jb     SHORT loc_1_1A65
        cmp    word ptr [ebp + 1ch], ax
        jne    SHORT loc_1_1AA2
        cmp    word ptr fs:[ecx], 589ch
        je     SHORT loc_1_1A8B
        cmp    ecx, 2
        jb     SHORT loc_1_1AA2
        cmp    dword ptr fs:[ecx - 2], 58669c66h
        jne    SHORT loc_1_1AA2
loc_1_1A8B:
        or     byte ptr [ebp + 1dh], 1
        jmp    SHORT loc_1_1A5F
loc_1_1A91:
        mov    ax, 2824h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   ecx, eax
        mov    byte ptr [ecx], 90h
        pop    fs
        ret
loc_1_1AA2:
        cmp    byte ptr fs:[ecx + 3], 0cfh
        je     SHORT loc_1_1A91
        cmp    ecx, 8
        jbe    SHORT loc_1_1A65
        cmp    dword ptr fs:[ecx - 7], 8f669c66h
        jne    SHORT loc_1_1AD9
        mov    ecx, dword ptr fs:[ecx - 2]
        mov    fs, word ptr [ebp + 3ch]
        xor    edx, edx
        mov    dx, fs
        lsl    edx, edx
        cmp    ecx, edx
        jae    SHORT loc_1_1AD9
        cmp    ax, word ptr fs:[ecx]
        jne    SHORT loc_1_1AD9
        or     byte ptr fs:[ecx + 1], 1
        jmp    SHORT loc_1_1A5F
loc_1_1AD9:
        cmp    word ptr fs:[ecx], 559ch
        jne    SHORT loc_1_1A65
        xchg   ecx, eax
        mov    ax, 2824h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   ecx, eax
        mov    word ptr [ecx - 1], 55fah
        pop    fs
        jmp    NEAR PTR loc_1_1A11
loc_1_1AFA:
        inc    dword ptr [ebp + 24h]
        bts    dword ptr [ebp + 2ch], 9
        ret
loc_1_1B03:
        test   byte ptr [ebx], 20h
        jne    NEAR PTR loc_1_10FB
        inc    dword ptr [ebp + 24h]
        mov    cx, 4
        test   edi, 20000h
        jne    SHORT loc_1_1B1D
        mov    cl, 2
loc_1_1B1D:
        sub    word ptr [ebp + 30h], cx
        movzx  esi, word ptr [ebp + 34h]
        movzx  ecx, word ptr [ebp + 30h]
        shl    esi, 4
        add    esi, ecx
        mov    ecx, dword ptr [ebp + 2ch]
        and    ecx, 0fffcffffh
        or     ch, 30h
        test   edi, 20000h
        jne    SHORT loc_1_1B46
        mov    word ptr [esi], cx
        ret
loc_1_1B46:
        mov    dword ptr [esi], ecx
        ret
loc_1_1B49:
        test   byte ptr [ebx], 20h
        jne    NEAR PTR loc_1_10FB
        inc    dword ptr [ebp + 24h]
        movzx  esi, word ptr [ebp + 34h]
        movzx  eax, word ptr [ebp + 30h]
        shl    esi, 4
        mov    cx, 4
        test   edi, 20000h
        jne    SHORT loc_1_1B6E
        mov    cl, 2
loc_1_1B6E:
        movzx  eax, word ptr [eax + esi]
        add    word ptr [ebp + 30h], cx
        and    eax, 0fffe8fffh
        or     eax, 20000h
        mov    dword ptr [ebp + 2ch], eax
        ret
loc_1_1B84:
        test   byte ptr [ebx], 20h
        jne    NEAR PTR loc_1_10FB
        movzx  esi, word ptr [ebp + 34h]
        shl    esi, 4
        movzx  eax, word ptr [ebp + 30h]
        test   edi, 20000h
        jne    SHORT loc_1_1BD1
sub_1_1BA0:
        mov    cx, word ptr [eax + esi]
        mov    word ptr [ebp + 24h], cx
        add    ax, 2
        mov    cx, word ptr [eax + esi]
        mov    word ptr [ebp + 28h], cx
        add    ax, 2
        movzx  ecx, word ptr [eax + esi]
        and    ecx, 0fffe8fffh
        or     ecx, 20000h
        mov    dword ptr [ebp + 2ch], ecx
        add    word ptr [ebp + 30h], 6
        ret
loc_1_1BD1:
        mov    ecx, dword ptr [eax + esi]
        mov    dword ptr [ebp + 24h], ecx
        add    ax, 4
        mov    ecx, dword ptr [eax + esi]
        mov    word ptr [ebp + 28h], cx
        add    ax, 4
        mov    ecx, dword ptr [eax + esi]
        and    ecx, 0fffe8fffh
        or     ecx, 20000h
        mov    dword ptr [ebp + 2ch], ecx
        add    word ptr [ebp + 30h], 0ch
        ret
        db  000h, 000h
Get_VMM_Version:
        mov    eax, 400h
        xor    ecx, ecx
        clc
        ret
VMM_PM_API:
        mov    eax, 0ffffffffh
        cmp    byte ptr [ebp + 1dh], 0
        jne    SHORT loc_1_1C1E
        db  0E8h, 046h, 0FCh, 0FEh, 0FFh
loc_1_1C19:
        mov    word ptr [ebp + 1ch], ax
        ret
loc_1_1C1E:
        cmp    byte ptr [ebp + 1dh], 1
        jne    SHORT loc_1_1C2B
        db  0E8h, 0F6h, 049h, 0FFh, 0FFh
        jmp    SHORT loc_1_1C19
loc_1_1C2B:
        mov    ax, 1
        cmp    byte ptr [ebp + 1dh], 2
        jne    SHORT loc_1_1C19
        mov    al, byte ptr [ebp + 1ch]
        db  0CDh, 020h, 0DCh, 000h, 001h, 000h
        sete   al
        mov    ah, 2
        jmp    SHORT loc_1_1C19
        db  000h, 000h, 000h
Schedule_Global_Event:
        push   eax
        push   ecx
        pushfd
        cli
        push   esi
        db  08Bh, 035h, 0D0h, 0D9h, 000h, 000h
        db  0A1h, 0D8h, 0D9h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_1C8C
        mov    ecx, dword ptr [eax - 4]
        db  089h, 00Dh, 0D8h, 0D9h, 000h, 000h
loc_1_1C65:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax - 4], ecx
        mov    dword ptr [esi], eax
        pop    dword ptr [eax]
        mov    dword ptr [eax + 4], edx
        xor    esi, esi
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0ch], esi
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [eax + 14h], esi
        mov    esi, eax
        db  066h, 0FFh, 005h, 0E0h, 0D9h, 000h, 000h
        popfd
        pop    ecx
        pop    eax
        ret
loc_1_1C8C:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jmp    SHORT loc_1_1C65
loc_1_1C94:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jmp    SHORT loc_1_1CBA
Schedule_VM_Event:
        push   eax
        push   ecx
        pushfd
        cli
        push   esi
        mov    esi, dword ptr [ebx + 20h]
        test   esi, esi
        je     SHORT loc_1_1D21
        db  0A1h, 0D8h, 0D9h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_1C94
        mov    ecx, dword ptr [eax - 4]
        db  089h, 00Dh, 0D8h, 0D9h, 000h, 000h
loc_1_1CBA:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax - 4], ecx
        mov    dword ptr [esi], eax
        pop    dword ptr [eax]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 14h], ebx
        xor    esi, esi
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0ch], esi
        mov    dword ptr [eax + 10h], esi
        mov    esi, eax
        inc    word ptr [ebx + 18h]
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_1_1CE9
        db  066h, 0FFh, 005h, 0E2h, 0D9h, 000h, 000h
loc_1_1CE9:
        push   edi
loc_1_1CEA:
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_1D11
        db  08Bh, 03Dh, 0F4h, 0F6h, 000h, 000h
        test   edi, edi
        jne    SHORT loc_1_1D17
        db  08Bh, 03Dh, 0D4h, 0F6h, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_1D0B
        cmp    dword ptr [edi + 14h], ebx
        je     SHORT loc_1_1D17
loc_1_1D0B:
        cmp    dword ptr [ebx - 6ch], 0
        jne    SHORT loc_1_1D1C
loc_1_1D11:
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
loc_1_1D17:
        call   sub_1_8168
loc_1_1D1C:
        pop    edi
loc_1_1D1D:
        popfd
        pop    ecx
        pop    eax
        ret
loc_1_1D21:
        pop    eax
        jmp    SHORT loc_1_1D1D
loc_1_1D24:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jmp    SHORT loc_1_1D4A
Schedule_Thread_Event:
        push   eax
        push   ecx
        pushfd
        cli
        push   esi
        mov    esi, dword ptr [edi + 6ch]
        test   esi, esi
        je     SHORT loc_1_1D21
        db  0A1h, 0D8h, 0D9h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_1D24
        mov    ecx, dword ptr [eax - 4]
        db  089h, 00Dh, 0D8h, 0D9h, 000h, 000h
loc_1_1D4A:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax - 4], ecx
        mov    dword ptr [esi], eax
        pop    dword ptr [eax]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 14h], edi
        xor    esi, esi
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0ch], esi
        mov    dword ptr [eax + 10h], esi
        or     byte ptr [eax + 9], 1
        mov    esi, eax
        inc    word ptr [edi + 68h]
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        jne    SHORT loc_1_1D7D
        db  066h, 0FFh, 005h, 0E2h, 0D9h, 000h, 000h
loc_1_1D7D:
        call   sub_1_8168
        popfd
        pop    ecx
        pop    eax
        ret
Cancel_Global_Event:
        test   esi, esi
        je     SHORT loc_1_1DB9
        push   eax
        push   esi
        pushfd
        cli
        mov    eax, esi
        db  08Bh, 035h, 0D0h, 0D9h, 000h, 000h
        call   List_Remove
        jb     SHORT loc_1_1DB6
        or     byte ptr [eax + 0bh], 80h
        db  08Bh, 035h, 0D8h, 0D9h, 000h, 000h
        mov    dword ptr [eax - 4], esi
        db  0A3h, 0D8h, 0D9h, 000h, 000h
        db  066h, 0FFh, 00Dh, 0E0h, 0D9h, 000h, 000h
loc_1_1DB6:
        popfd
        pop    esi
        pop    eax
loc_1_1DB9:
        ret
Cancel_VM_Event:
        test   esi, esi
        je     SHORT loc_1_1DF6
        push   eax
        push   esi
        pushfd
        cli
        mov    eax, esi
        mov    esi, dword ptr [ebx + 20h]
        call   List_Remove
        jb     SHORT loc_1_1DF3
        or     byte ptr [eax + 0bh], 80h
        db  08Bh, 035h, 0D8h, 0D9h, 000h, 000h
        mov    dword ptr [eax - 4], esi
        db  0A3h, 0D8h, 0D9h, 000h, 000h
        dec    word ptr [ebx + 18h]
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_1_1DF3
        db  066h, 0FFh, 00Dh, 0E2h, 0D9h, 000h, 000h
loc_1_1DF3:
        popfd
        pop    esi
        pop    eax
loc_1_1DF6:
        ret
Cancel_Thread_Event:
        test   esi, esi
        je     SHORT loc_1_1E33
        push   eax
        push   esi
        pushfd
        cli
        mov    eax, esi
        mov    esi, dword ptr [edi + 6ch]
        call   List_Remove
        jb     SHORT loc_1_1E30
        or     byte ptr [eax + 0bh], 80h
        db  08Bh, 035h, 0D8h, 0D9h, 000h, 000h
        mov    dword ptr [eax - 4], esi
        db  0A3h, 0D8h, 0D9h, 000h, 000h
        dec    word ptr [edi + 68h]
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        jne    SHORT loc_1_1E30
        db  066h, 0FFh, 00Dh, 0E2h, 0D9h, 000h, 000h
loc_1_1E30:
        popfd
        pop    esi
        pop    eax
loc_1_1E33:
        ret
loc_1_1E34:
        xchg   ebx, edi
        or     ecx, 100h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        xchg   ebx, edi
        and    ecx, 0fffffeffh
        ret
        db  090h
Call_Priority_VM_Event:
        and    ecx, 3fffh
        pushad
        db  08Bh, 035h, 0D0h, 0D9h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_1_1E7A
        test   ch, 1
        je     SHORT loc_1_1E6F
        mov    esi, dword ptr [ebx + 6ch]
        jmp    SHORT loc_1_1E72
loc_1_1E67:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jmp    SHORT loc_1_1E8E
loc_1_1E6F:
        mov    esi, dword ptr [ebx + 20h]
loc_1_1E72:
        test   esi, esi
        je     NEAR PTR loc_1_1FB0
loc_1_1E7A:
        pushfd
        cli
        db  0A1h, 0D8h, 0D9h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_1E67
        mov    edx, dword ptr [eax - 4]
        db  089h, 015h, 0D8h, 0D9h, 000h, 000h
loc_1_1E8E:
        mov    edx, dword ptr [esi]
        mov    dword ptr [eax - 4], edx
        mov    dword ptr [esi], eax
        mov    edx, eax
        xchg   dword ptr [esp + 8], eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edx + 4], eax
        popfd
        mov    edi, dword ptr [esp + 1ch]
        mov    dword ptr [edx + 0ch], edi
        mov    dword ptr [edx + 14h], ebx
        mov    dword ptr [edx + 8], ecx
        test   ebx, ebx
        je     NEAR PTR loc_1_1F68
        mov    esi, edi
        test   ch, 1
        jne    SHORT loc_1_1F40
        cmp    dword ptr [ebx - 6ch], 0
        jne    SHORT loc_1_1ECC
        call   sub_1_2160
loc_1_1ECC:
        inc    word ptr [ebx + 18h]
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_1_1EDF
        db  066h, 0FFh, 005h, 0E2h, 0D9h, 000h, 000h
loc_1_1EDF:
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_1F12
        test   ch, 10h
        je     SHORT loc_1_1F12
        db  0B8h, 0C4h, 0F6h, 000h, 000h
        mov    edi, dword ptr [eax + 10h]
        test   edi, edi
        jne    SHORT loc_1_1EFF
        mov    edi, dword ptr [eax + 30h]
        test   edi, edi
        je     SHORT loc_1_1F12
loc_1_1EFF:
        cmp    ebx, dword ptr [edi + 14h]
        jne    SHORT loc_1_1F12
        test   byte ptr [edi + 1], 4
        je     SHORT loc_1_1F12
        pushfd
        cli
        call   sub_1_8168
        popfd
loc_1_1F12:
        test   cl, 1
        je     SHORT loc_1_1F20
        inc    dword ptr [ebx - 4]
        jne    SHORT loc_1_1F20
        mov    byte ptr [ebx - 67h], 0cfh
loc_1_1F20:
        and    ecx, 10h
        je     SHORT loc_1_1F28
        mov    ecx, dword ptr [esp]
loc_1_1F28:
        mov    dword ptr [edx + 10h], ecx
        or     ecx, esi
        je     SHORT loc_1_1F3E
        or     byte ptr [edx + 0bh], 40h
        db  0BEh, 08Bh, 020h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_1_1F3E:
        popad
        ret
loc_1_1F40:
        mov    edi, ebx
        inc    word ptr [ebx + 68h]
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        jne    SHORT loc_1_1F55
        db  066h, 0FFh, 005h, 0E2h, 0D9h, 000h, 000h
loc_1_1F55:
        mov    ebx, dword ptr [edi + 14h]
        test   byte ptr [edi + 1], 4
        je     SHORT loc_1_1F12
        pushfd
        cli
        call   sub_1_8168
        popfd
        jmp    SHORT loc_1_1F12
loc_1_1F68:
        db  066h, 0FFh, 005h, 0E0h, 0D9h, 000h, 000h
        test   ch, 8
        je     SHORT loc_1_1F95
        db  0BEh, 0C4h, 0F6h, 000h, 000h
        mov    edi, dword ptr [esi + 10h]
        test   edi, edi
        jne    SHORT loc_1_1F87
        mov    edi, dword ptr [esi + 30h]
        test   edi, edi
        je     SHORT loc_1_1F95
loc_1_1F87:
        test   byte ptr [edi + 1], 4
        je     SHORT loc_1_1F95
        pushfd
        cli
        call   sub_1_8168
        popfd
loc_1_1F95:
        and    ecx, 10h
        mov    dword ptr [edx + 10h], ecx
        je     SHORT loc_1_1F3E
        mov    eax, dword ptr [esp]
        db  0BEh, 0DBh, 020h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 10h], esi
        jmp    SHORT loc_1_1F3E
loc_1_1FB0:
        mov    dword ptr [esp + 4], esi
        jmp    SHORT loc_1_1F3E
sub_1_1FB6:
        push   ebx
        mov    eax, esi
        mov    esi, dword ptr [eax + 10h]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    ebx, dword ptr [eax + 14h]
        test   ebx, ebx
        je     SHORT loc_1_201C
        test   byte ptr [eax + 9], 1
        je     SHORT loc_1_1FD4
        mov    edi, ebx
        mov    ebx, dword ptr [ebx + 14h]
loc_1_1FD4:
        test   byte ptr [eax + 8], 1
        je     SHORT loc_1_1FE3
        dec    dword ptr [ebx - 4]
        jns    SHORT loc_1_1FE3
        mov    byte ptr [ebx - 67h], 0ffh
loc_1_1FE3:
        test   byte ptr [eax + 9], 1
        je     SHORT loc_1_1FFA
        mov    esi, dword ptr [edi + 6ch]
        dec    word ptr [edi + 68h]
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        je     SHORT loc_1_2009
        jmp    SHORT loc_1_2010
loc_1_1FFA:
        mov    esi, dword ptr [ebx + 20h]
        dec    word ptr [ebx + 18h]
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_1_2010
loc_1_2009:
        db  066h, 0FFh, 00Dh, 0E2h, 0D9h, 000h, 000h
loc_1_2010:
        pushfd
        cli
        call   List_Remove
        popfd
        mov    esi, eax
        pop    ebx
        ret
loc_1_201C:
        db  08Bh, 035h, 0D0h, 0D9h, 000h, 000h
        mov    dword ptr [eax + 0ch], ebx
        db  066h, 0FFh, 00Dh, 0E0h, 0D9h, 000h, 000h
        jmp    SHORT loc_1_2010
        db  08Bh, 0C0h
Cancel_Priority_VM_Event:
        test   esi, esi
        je     SHORT loc_1_2080
        bts    dword ptr [esi + 8], 1fh
        jb     SHORT loc_1_2080
        push   eax
        push   ebx
        push   edi
        call   sub_1_1FB6
        test   byte ptr [esi + 0bh], 40h
        jne    SHORT loc_1_207D
        mov    eax, dword ptr [esi + 0ch]
        pushfd
        cli
        db  08Bh, 03Dh, 0D8h, 0D9h, 000h, 000h
        mov    dword ptr [esi - 4], edi
        mov    edi, dword ptr [esi + 8]
        mov    ebx, dword ptr [esi + 14h]
        db  089h, 035h, 0D8h, 0D9h, 000h, 000h
        popfd
        neg    eax
        je     SHORT loc_1_207D
        test   di, 100h
        jne    SHORT loc_1_2081
        test   edi, 2000000h
        jne    SHORT loc_1_207D
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_1_207D:
        pop    edi
        pop    ebx
        pop    eax
loc_1_2080:
        ret
loc_1_2081:
        mov    edi, ebx
        db  0CDh, 020h, 014h, 001h, 001h, 000h
        jmp    SHORT loc_1_207D
loc_1_208B:
        test   byte ptr [edx + 0bh], 80h
        jne    SHORT loc_1_20CA
        and    byte ptr [edx + 0bh], 0bfh
        mov    eax, dword ptr [edx + 10h]
        test   eax, eax
        je     SHORT loc_1_20AA
        db  0BEh, 0DBh, 020h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 10h], esi
loc_1_20AA:
        mov    eax, dword ptr [edx + 0ch]
        test   eax, eax
        je     SHORT loc_1_20C9
        test   byte ptr [edx + 9], 1
        jne    SHORT loc_1_20C0
        mov    ebx, dword ptr [edx + 14h]
        db  0CDh, 020h, 01Eh, 080h, 001h, 000h
loc_1_20C0:
        mov    edi, dword ptr [edx + 14h]
        db  0CDh, 020h, 014h, 081h, 001h, 000h
loc_1_20C9:
        ret
loc_1_20CA:
        cli
        db  0A1h, 0D8h, 0D9h, 000h, 000h
        mov    dword ptr [edx - 4], eax
        db  089h, 015h, 0D8h, 0D9h, 000h, 000h
        sti
        ret
loc_1_20DB:
        mov    esi, edx
        xor    edx, edx
        mov    dword ptr [esi + 10h], edx
        or     byte ptr [esi + 0bh], 80h
        call   sub_1_1FB6
        mov    ebx, dword ptr [esi + 14h]
        test   byte ptr [esi + 9], 1
        jne    SHORT loc_1_20FC
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        jmp    SHORT loc_1_2101
loc_1_20FC:
        mov    edi, ebx
        mov    ebx, dword ptr [ebx + 14h]
loc_1_2101:
        mov    edx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT sub_1_213B
        call   sub_1_213B
        neg    eax
        test   ch, 1
        jne    SHORT loc_1_212B
        test   byte ptr [esi + 0bh], 2
        jne    SHORT loc_1_215F
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_1_215F
        db  0CDh, 020h, 01Eh, 080h, 001h, 000h
loc_1_212B:
        mov    edi, ebx
        db  0CDh, 020h, 00Ch, 001h, 001h, 000h
        jb     SHORT loc_1_215F
        db  0CDh, 020h, 014h, 081h, 001h, 000h
sub_1_213B:
        push   ebp
        mov    ebp, dword ptr [edi + 10h]
        stc
        push   esi
        call   dword ptr [esi]
        pop    esi
        pop    ebp
        pushfd
        cli
        db  0A1h, 0D8h, 0D9h, 000h, 000h
        mov    dword ptr [esi - 4], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [esi + 8]
        mov    ebx, dword ptr [esi + 14h]
        db  089h, 035h, 0D8h, 0D9h, 000h, 000h
        popfd
loc_1_215F:
        ret
sub_1_2160:
        mov    ebx, dword ptr [edx + 14h]
        push   eax
        push   ecx
        pushfd
        cli
        push   edi
        jmp    NEAR PTR loc_1_1CEA
sub_1_216D:
        push   esi
        mov    esi, eax
        xor    eax, eax
        db  03Bh, 005h, 0E0h, 0D9h, 000h, 000h
        jne    SHORT loc_1_2184
        pop    esi
        ret
loc_1_217C:
        dec    eax
        pop    esi
        ret
sub_1_217F:
        xor    eax, eax
        push   esi
        xor    esi, esi
loc_1_2184:
        mov    ebp, dword ptr [ebx + 8]
        db  039h, 005h, 0E4h, 0D9h, 000h, 000h
        jne    SHORT loc_1_217C
        push   ecx
        mov    ecx, 800002e3h
        test   byte ptr [ebp + 2dh], 2
        je     SHORT loc_1_21A8
        and    ch, 0fdh
        mov    dl, byte ptr [ebx - 68h]
        rcr    cl, 1
        add    dl, 0ffh
        rcl    cl, 1
loc_1_21A8:
        db  0B8h, 0C4h, 0F6h, 000h, 000h
        mov    edx, dword ptr [eax + 10h]
        test   edx, edx
        jne    SHORT loc_1_21D1
        mov    edx, dword ptr [eax + 30h]
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_21C3
        test   edx, edx
        jne    SHORT loc_1_21D1
loc_1_21C3:
        cmp    dword ptr [edi + 3ch], 100000h
        jae    SHORT loc_1_21E8
        and    ecx, 0fffffffdh
        jmp    SHORT loc_1_21E8
loc_1_21D1:
        cmp    edx, edi
        je     SHORT loc_1_21E8
        or     ch, 8
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_21E8
        cmp    dword ptr [edx + 14h], ebx
        jne    SHORT loc_1_21E8
        or     ch, 10h
loc_1_21E8:
        mov    al, byte ptr [edi + 73h]
        not    al
        and    cl, al
        mov    al, byte ptr [ebx + 1eh]
        or     al, 0dfh
        and    cl, al
        push   ebp
        xor    ebp, ebp
        db  066h, 083h, 03Dh, 0E0h, 0D9h, 000h, 000h, 000h
        je     SHORT loc_1_220D
        db  0A1h, 0D0h, 0D9h, 000h, 000h
        call   sub_1_223C
loc_1_220D:
        cmp    word ptr [ebx + 18h], 0
        je     SHORT loc_1_221C
        mov    eax, dword ptr [ebx + 20h]
        call   sub_1_223C
loc_1_221C:
        cmp    word ptr [edi + 68h], 0
        je     SHORT loc_1_222B
        mov    eax, dword ptr [edi + 6ch]
        call   sub_1_223C
loc_1_222B:
        test   ebp, ebp
        jne    SHORT loc_1_2235
        xor    eax, eax
loc_1_2231:
        pop    ebp
        pop    ecx
        pop    esi
        ret
loc_1_2235:
        mov    edx, ebp
        mov    eax, dword ptr [edx - 4]
        jmp    SHORT loc_1_2231
sub_1_223C:
        lea    edx, [eax + 4]
loc_1_223F:
        mov    eax, dword ptr [edx - 4]
        test   eax, eax
        je     SHORT loc_1_224F
        test   dword ptr [eax + 8], ecx
        je     SHORT loc_1_2250
loc_1_224B:
        mov    edx, eax
        jmp    SHORT loc_1_223F
loc_1_224F:
        ret
loc_1_2250:
        test   esi, esi
        je     SHORT loc_1_2259
        test   dword ptr [eax + 8], esi
        je     SHORT loc_1_224B
loc_1_2259:
        test   dword ptr [eax + 8], 2000h
        jne    SHORT loc_1_2269
        add    esp, 4
        pop    ebp
        pop    ecx
        pop    esi
        ret
loc_1_2269:
        test   ebp, ebp
        jne    SHORT loc_1_224B
        mov    ebp, edx
        jmp    SHORT loc_1_224B
loc_1_2271:
        xor    esi, esi
        db  089h, 035h, 0E4h, 0D9h, 000h, 000h
        db  08Bh, 035h, 0E8h, 0D9h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_2299
loc_1_2287:
        push   eax
        push   ebx
        push   ebp
        push   esi
        call   dword ptr [eax]
        pop    esi
        pop    ebp
        pop    ebx
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_2287
loc_1_2299:
        ret
sub_1_229A:
        inc    eax
        je     SHORT loc_1_2271
        dec    eax
        mov    ecx, dword ptr [eax - 4]
        mov    dword ptr [edx - 4], ecx
        mov    edx, eax
        mov    ecx, dword ptr [edx + 14h]
        test   ecx, ecx
        jne    SHORT loc_1_22D0
        db  066h, 0FFh, 00Dh, 0E0h, 0D9h, 000h, 000h
        mov    eax, dword ptr [edx + 0ch]
        test   eax, eax
        je     SHORT loc_1_2311
        db  0CDh, 020h, 014h, 001h, 001h, 000h
        jmp    SHORT loc_1_2311
loc_1_22C3:
        dec    word ptr [edi + 68h]
        db  066h, 0FFh, 00Dh, 0E2h, 0D9h, 000h, 000h
        jmp    SHORT loc_1_2302
loc_1_22D0:
        test   byte ptr [edx + 9], 1
        jne    SHORT loc_1_22C3
        dec    word ptr [ebx + 18h]
        db  066h, 0FFh, 00Dh, 0E2h, 0D9h, 000h, 000h
        test   byte ptr [edx + 8], 4
        jne    SHORT loc_1_2302
        mov    eax, dword ptr [edx + 0ch]
        test   eax, eax
        je     SHORT loc_1_2302
        db  0CDh, 020h, 014h, 001h, 001h, 000h
        test   byte ptr [edx + 0bh], 2
        jne    SHORT loc_1_2302
        neg    eax
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_1_2302:
        test   byte ptr [edx + 8], 1
        je     SHORT loc_1_2311
        dec    dword ptr [ebx - 4]
        jns    SHORT loc_1_2311
        mov    byte ptr [ebx - 67h], 0ffh
loc_1_2311:
        mov    esi, dword ptr [edx + 10h]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        push   edx
        or     byte ptr [edx + 0bh], 80h
        mov    eax, dword ptr [edx]
        mov    edx, dword ptr [edx + 4]
        push   dword ptr [edi + 44h]
        mov    dword ptr [edi + 44h], esp
        sti
        call   eax
loc_1_232D:
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        pop    dword ptr [edi + 44h]
        cli
        pop    edx
        mov    eax, dword ptr [edx + 0ch]
        db  08Bh, 00Dh, 0D8h, 0D9h, 000h, 000h
        mov    dword ptr [edx - 4], ecx
        mov    ecx, dword ptr [edx + 8]
        db  089h, 015h, 0D8h, 0D9h, 000h, 000h
        neg    eax
        je     SHORT loc_1_235D
        test   cl, 4
        jne    SHORT loc_1_235D
        db  0CDh, 020h, 014h, 001h, 001h, 000h
        cli
loc_1_235D:
        ret
loc_1_235E:
        db  0A1h, 098h, 0F6h, 000h, 000h
        mov    eax, dword ptr [eax + 44h]
        ret
sub_1_2367:
        mov    eax, dword ptr [edi + 44h]
        cli
        mov    edx, dword ptr [eax + 4]
        db  08Bh, 00Dh, 0D8h, 0D9h, 000h, 000h
        mov    dword ptr [edx - 4], ecx
        db  089h, 015h, 0D8h, 0D9h, 000h, 000h
        sti
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 44h], eax
        ret
sub_1_2384:
        mov    eax, dword ptr [edi + 44h]
        test   eax, eax
        stc
        je     SHORT loc_1_23AC
        mov    ecx, dword ptr [eax - 4]
        db  081h, 0F9h, 02Dh, 023h, 000h, 000h
        stc
        jne    SHORT loc_1_23AC
        pop    ecx
        pop    ecx
        pushfd
        push   cs
        db  068h, 024h, 010h, 000h, 000h
        push   eax
        pushad
        mov    ebp, esp
        push   ds
        push   es
        push   fs
        push   gs
        push   ecx
loc_1_23AC:
        ret
loc_1_23AD:
        db  03Bh, 01Dh, 014h, 0F7h, 000h, 000h
        je     SHORT loc_1_2406
        cmp    word ptr [ebx + 18h], 0
        je     SHORT loc_1_23F6
        cli
        mov    esi, dword ptr [ebx + 20h]
        lea    esi, [esi + 4]
        xor    eax, eax
loc_1_23C5:
        mov    esi, dword ptr [esi - 4]
        test   esi, esi
        je     SHORT loc_1_23EB
        mov    edx, dword ptr [esi + 0ch]
        neg    edx
        je     SHORT loc_1_23C5
        test   dword ptr [esi + 8], 2000004h
        jne    SHORT loc_1_23C5
        btc    dword ptr [esi + 8], 1ah
        jae    SHORT loc_1_23C5
        or     byte ptr [esi + 0bh], 2
        add    eax, edx
        jmp    SHORT loc_1_23C5
loc_1_23EB:
        sti
        test   eax, eax
        je     SHORT loc_1_23F6
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_1_23F6:
        mov    eax, 7d0h
        db  0BEh, 0ADh, 023h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
loc_1_2406:
        ret
        db  000h
loc_1_2408:
        push   ecx
        xor    ecx, ecx
        pushfd
        db  0CDh, 020h, 03Ch, 001h, 001h, 000h
        popfd
        pop    ecx
loc_1_2414:
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_2418:
        call   sub_1_2440
        push   eax
        push   esi
        db  0A1h, 098h, 0F6h, 000h, 000h
        mov    esi, dword ptr [eax + 48h]
        mov    esi, dword ptr [esi]
        or     byte ptr [esi], 8
        mov    eax, dword ptr [eax + 10h]
        mov    word ptr [eax + 30h], cx
        ror    ecx, 10h
        mov    word ptr [eax + 34h], cx
        rol    ecx, 10h
        pop    esi
        pop    eax
        ret
sub_1_2440:
        pushad
        db  0BFh, 014h, 024h, 000h, 000h
        jmp    SHORT loc_1_244E
Begin_Nest_V86_Exec:
        pushad
        db  0BFh, 008h, 024h, 000h, 000h
loc_1_244E:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  08Bh, 00Dh, 0ECh, 0D9h, 000h, 000h
        mov    edx, dword ptr [ebx]
        test   dl, 20h
        je     SHORT loc_1_24C4
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_246B
        call   edi
loc_1_246B:
        db  068h, 0C4h, 024h, 000h, 000h
        db  0CDh, 020h, 089h, 080h, 001h, 000h
        mov    eax, eax
Begin_Nest_Exec:
        pushad
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  08Bh, 00Dh, 0ECh, 0D9h, 000h, 000h
        mov    edx, dword ptr [ebx]
        test   dl, 40h
        jne    SHORT loc_1_24AD
        test   dl, 20h
        je     SHORT loc_1_24C4
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_24A5
        push   ecx
        xor    ecx, ecx
        pushfd
        db  0CDh, 020h, 03Ch, 001h, 001h, 000h
        popfd
        pop    ecx
loc_1_24A5:
        db  0CDh, 020h, 089h, 000h, 001h, 000h
        jmp    SHORT loc_1_24C4
loc_1_24AD:
        db  08Bh, 00Dh, 0F0h, 0D9h, 000h, 000h
        test   dl, 20h
        jne    SHORT loc_1_24BE
        db  0CDh, 020h, 08Ah, 000h, 001h, 000h
loc_1_24BE:
        db  0CDh, 020h, 08Bh, 000h, 001h, 000h
loc_1_24C4:
        push   edx
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    esi, dword ptr [edi + 48h]
        pushfd
        cli
        mov    edx, dword ptr [esi + 4]
        mov    eax, dword ptr [edx]
        test   eax, eax
        jne    SHORT loc_1_24E1
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jmp    SHORT loc_1_24E6
loc_1_24E1:
        mov    ebp, dword ptr [eax - 4]
        mov    dword ptr [edx], ebp
loc_1_24E6:
        mov    edx, dword ptr [esi]
        mov    dword ptr [eax - 4], edx
        mov    dword ptr [esi], eax
        popfd
        pop    edx
        and    byte ptr [edi + 73h], 0bfh
        shr    edx, 5
        and    edx, 1
        btr    dword ptr [ebx], 8
        rcl    edx, 1
        mov    byte ptr [eax], dl
        mov    ebp, dword ptr [edi + 10h]
        movzx  esi, cx
        xchg   dword ptr [ebp + 24h], esi
        mov    dword ptr [eax + 4], esi
        shr    ecx, 10h
        xchg   word ptr [ebp + 28h], cx
        mov    word ptr [eax + 2], cx
        mov    cx, word ptr [ebp + 34h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 30h]
        mov    dword ptr [eax + 0ch], ecx
        popad
        ret
Exec_Int:
        db  0CDh, 020h, 046h, 000h, 001h, 000h
Resume_Exec:
        mov    eax, eax
loc_1_2530:
        pushad
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        push   fs
        mov    edx, dword ptr [edi + 48h]
        push   gs
        mov    edx, dword ptr [edx]
        and    byte ptr [edx], 0fbh
        mov    al, byte ptr [edi - 33h]
        mov    ah, byte ptr [edi - 32h]
        mov    byte ptr [edx + 1], ah
        mov    byte ptr [edi - 32h], al
        cmp    al, ah
        jbe    SHORT loc_1_2556
        or     byte ptr [edx], 4
loc_1_2556:
        mov    dword ptr [edx + 8], esp
        jmp    NEAR PTR sub_1_0300
        db  08Bh, 0C0h
loc_1_2560:
        db  081h, 03Dh, 034h, 0E4h, 000h, 000h, 000h, 00Ah, 000h, 000h
        je     SHORT loc_1_25B9
        db  0BEh, 0A0h, 025h, 000h, 000h
loc_1_2571:
        mov    dx, word ptr [ebp + 28h]
        shl    edx, 10h
        mov    dx, word ptr [ebp + 24h]
        db  0CDh, 020h, 0FBh, 080h, 001h, 000h
        mov    eax, eax
loc_1_2584:
        db  081h, 03Dh, 0D8h, 0E5h, 000h, 000h, 0D1h, 010h, 000h, 000h
        je     SHORT loc_1_25B9
        db  0BEh, 097h, 025h, 000h, 000h
        jmp    SHORT loc_1_2571
loc_1_2597:
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_25A5
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_25A0:
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_25DA
loc_1_25A5:
        cmp    dx, word ptr [ebp + 24h]
        jne    SHORT loc_1_25DA
        shr    edx, 10h
        cmp    dx, word ptr [ebp + 28h]
        jne    SHORT loc_1_25DA
        call   sub_1_2367
loc_1_25B9:
        clc
loc_1_25BA:
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    edx, dword ptr [edi + 48h]
        mov    edx, dword ptr [edx]
        mov    esp, dword ptr [edx + 8]
        xor    eax, eax
        mov    dword ptr [edx + 8], eax
        pushfd
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [edi - 32h], al
        popfd
        pop    gs
        pop    fs
        popad
loc_1_25DA:
        ret
sub_1_25DB:
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        or     byte ptr [edi], 40h
loc_1_25E4:
        mov    esi, dword ptr [edi + 48h]
        mov    edx, dword ptr [esi]
        test   byte ptr [edx], 4
        jne    SHORT loc_1_25F6
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        jmp    SHORT loc_1_25E4
loc_1_25F6:
        stc
        jmp    SHORT loc_1_25BA
loc_1_25F9:
        pushfd
        db  0CDh, 020h, 072h, 001h, 001h, 000h
        popfd
        ret
        db  08Bh, 0C0h
sub_1_2604:
        pushad
        db  0BFh, 014h, 024h, 000h, 000h
        jmp    SHORT loc_1_2612
End_Nest_Exec:
        pushad
        db  0BFh, 0F9h, 025h, 000h, 000h
loc_1_2612:
        pushfd
        cli
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  08Bh, 02Dh, 098h, 0F6h, 000h, 000h
        mov    esi, dword ptr [ebp + 48h]
        mov    ecx, dword ptr [esi]
        test   byte ptr [ecx], 8
        je     SHORT loc_1_2640
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    word ptr [edx + 30h], cx
        shl    ecx, 10h
        mov    word ptr [edx + 34h], cx
        db  0BFh, 014h, 024h, 000h, 000h
loc_1_2640:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    edx, eax
        xchg   dword ptr [ecx], edx
        xchg   dword ptr [eax - 4], edx
        mov    dword ptr [esi], edx
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_265D
        cmp    dword ptr [ebp + 24h], 0
        jne    SHORT loc_1_265D
        or     byte ptr [ebp + 73h], 40h
loc_1_265D:
        mov    ebp, dword ptr [ebp + 10h]
        mov    esi, dword ptr [eax + 4]
        mov    dword ptr [ebp + 24h], esi
        mov    si, word ptr [eax + 2]
        mov    word ptr [ebp + 28h], si
        movzx  si, byte ptr [eax]
        popfd
        and    dword ptr [ebx], 0fffffeffh
        test   si, 1
        je     SHORT loc_1_2686
        or     dword ptr [ebx], 100h
loc_1_2686:
        test   si, 2
        je     SHORT loc_1_26AC
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_269A
        db  0CDh, 020h, 08Ch, 000h, 001h, 000h
        jmp    SHORT loc_1_26BD
loc_1_269A:
        db  0CDh, 020h, 08Ah, 000h, 001h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_26BD
        call   edi
        jmp    SHORT loc_1_26BD
loc_1_26AC:
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_26BD
        db  0CDh, 020h, 08Ch, 000h, 001h, 000h
        db  0CDh, 020h, 089h, 000h, 001h, 000h
loc_1_26BD:
        popad
        ret
        db  090h
Set_V86_Exec_Mode:
        pushad
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        btr    dword ptr [ebx], 5
        jae    SHORT loc_1_2714
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        and    byte ptr [edi + 73h], 7fh
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 2ch]
        db  00Fh, 001h, 01Dh, 090h, 09Bh, 000h, 000h
        std
        lea    esi, [ebp + 68h]
        lea    edi, [esi + 24h]
        mov    ecx, 12h
        rep movsd
        lea    esi, [ebp + 8ch]
        mov    ecx, 9
        rep movsd
        cld
        and    eax, 0fffe8fffh
        or     eax, 20000h
        mov    dword ptr [ebp + 2ch], eax
        db  083h, 005h, 0A0h, 09Eh, 000h, 000h, 010h
loc_1_2714:
        popad
        ret
        db  08Bh, 0C0h
Set_PM_Exec_Mode:
        pushad
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        bts    dword ptr [ebx], 5
        jb     SHORT loc_1_2764
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        or     byte ptr [edi + 73h], 80h
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 2ch]
        lidt   [ebx - 32h]
        std
        lea    esi, [ebp + 68h]
        lea    edi, [esi + 24h]
        mov    ecx, 12h
        rep movsd
        lea    esi, [ebp + 8ch]
        mov    ecx, 9
        rep movsd
        cld
        and    eax, 0fffc8fffh
        mov    dword ptr [ebp + 2ch], eax
        db  083h, 02Dh, 0A0h, 09Eh, 000h, 000h, 010h
loc_1_2764:
        popad
        ret
Get_Cur_PM_App_CB:
        mov    edi, dword ptr [ebx - 3ch]
        ret
        db  02Eh, 08Bh, 0C0h
Save_Client_State:
        push   ecx
        push   esi
        mov    ecx, 1bh
        db  08Bh, 035h, 00Ch, 0F7h, 000h, 000h
        mov    esi, dword ptr [esi + 8]
        cld
        rep movsd
        and    byte ptr [esi - 3fh], 0feh
        sub    edi, 6ch
        pop    esi
        pop    ecx
        ret
        db  02Eh, 08Bh, 0C0h
Restore_Client_State:
        pushad
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 2ch]
        xor    eax, dword ptr [esi + 2ch]
        test   eax, 20000h
        je     SHORT loc_1_27BB
        test   byte ptr [esi + 2eh], 2
        je     SHORT loc_1_27B5
        db  0CDh, 020h, 089h, 000h, 001h, 000h
        jmp    SHORT loc_1_27BB
loc_1_27B5:
        db  0CDh, 020h, 08Ah, 000h, 001h, 000h
loc_1_27BB:
        mov    edx, dword ptr [ebp + 2ch]
        and    edx, 37200h
        mov    edi, ebp
        mov    ecx, 1bh
        cld
        rep movsd
        and    dword ptr [ebp + 2ch], 0fffc8dffh
        or     dword ptr [ebp + 2ch], edx
        test   ah, 2
        je     SHORT loc_1_27F1
        test   byte ptr [ebp + 2dh], 2
        jne    SHORT loc_1_27EB
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        jmp    SHORT loc_1_27F1
loc_1_27EB:
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
loc_1_27F1:
        popad
        ret
Begin_Use_Locked_PM_Stack:
        push   ebx
        push   esi
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  08Bh, 035h, 098h, 0F6h, 000h, 000h
        cmp    dword ptr [esi + 24h], 0
        ja     SHORT loc_1_2845
        push   eax
        push   ebp
        mov    ebp, dword ptr [esi + 10h]
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [esi + 20h], eax
        mov    ax, word ptr [ebp + 28h]
        mov    word ptr [esi + 28h], ax
        mov    ax, word ptr [esi - 2eh]
        test   byte ptr [ebx], 40h
        je     SHORT loc_1_282C
        test   byte ptr [ebx], 80h
        jne    SHORT loc_1_282C
        mov    ax, word ptr [esi - 30h]
loc_1_282C:
        xchg   word ptr [ebp + 34h], ax
        mov    word ptr [esi + 1ah], ax
        mov    eax, 1000h
        xchg   dword ptr [ebp + 30h], eax
        mov    dword ptr [esi + 1ch], eax
        and    byte ptr [esi + 73h], 0bfh
        pop    ebp
        pop    eax
loc_1_2845:
        inc    dword ptr [esi + 24h]
        pop    esi
        pop    ebx
        ret
End_Use_Locked_PM_Stack:
        push   edi
        push   eax
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        dec    dword ptr [edi + 24h]
        jne    SHORT loc_1_2885
        mov    eax, dword ptr [edi + 48h]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_2864
        or     byte ptr [edi + 73h], 40h
loc_1_2864:
        push   ebp
        mov    ebp, dword ptr [edi + 10h]
        mov    ax, word ptr [edi + 1ah]
        mov    word ptr [ebp + 34h], ax
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [ebp + 30h], eax
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [ebp + 24h], eax
        mov    ax, word ptr [edi + 28h]
        mov    word ptr [ebp + 28h], ax
        pop    ebp
loc_1_2885:
        pop    eax
        pop    edi
        ret
Set_VM_Time_Out:
        push   ecx
        lea    ecx, [ebx + 38h]
        jmp    SHORT loc_1_28A8
        db  08Bh, 0C0h
Set_Thread_Time_Out:
        push   ecx
        lea    ecx, [edi - 48h]
        jmp    SHORT loc_1_28A8
        db  08Bh, 0C0h
Set_Async_Time_Out:
        push   ecx
        db  0B9h, 000h, 0DAh, 000h, 000h
        jmp    SHORT loc_1_28A8
Set_Global_Time_Out:
        push   ecx
        db  0B9h, 010h, 0DAh, 000h, 000h
        mov    eax, eax
loc_1_28A8:
        push   edi
        push   edx
        push   eax
        or     eax, eax
        jle    SHORT loc_1_2929
        mov    edi, esi
        pushfd
        cli
        db  08Bh, 035h, 024h, 0DAh, 000h, 000h
        test   esi, esi
        jne    SHORT loc_1_28D5
        push   ecx
        push   edx
        push   esi
        push   14h
        call   sub_1_662C
        add    esp, 8
        mov    esi, eax
        pop    edx
        pop    ecx
        test   esi, esi
        jne    SHORT loc_1_28DC
        popfd
        jmp    SHORT loc_1_2929
loc_1_28D5:
        mov    eax, dword ptr [esi]
        db  0A3h, 024h, 0DAh, 000h, 000h
loc_1_28DC:
        popfd
        mov    eax, dword ptr [esp]
        mov    dword ptr [esi + 0ch], edi
        mov    dword ptr [esi + 10h], edx
        cmp    eax, 32h
        jae    SHORT loc_1_28F1
        db  0CDh, 020h, 001h, 000h, 005h, 000h
loc_1_28F1:
        pushfd
        cli
        add    eax, dword ptr [ecx + 0ch]
        jns    SHORT loc_1_28FD
        mov    eax, 7fffffffh
loc_1_28FD:
        mov    edi, ecx
loc_1_28FF:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, edi
        je     SHORT loc_1_2913
        sub    eax, dword ptr [ecx + 8]
        jg     SHORT loc_1_28FF
        mov    edi, eax
        neg    edi
        xchg   dword ptr [ecx + 8], edi
        add    eax, edi
loc_1_2913:
        mov    dword ptr [esi + 8], eax
        mov    edx, esi
        xchg   dword ptr [ecx + 4], edx
        mov    dword ptr [edx], esi
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi], ecx
        popfd
loc_1_2923:
        pop    eax
        pop    edx
        pop    edi
        pop    ecx
        ret
        db  058h
loc_1_2929:
        xor    esi, esi
        jmp    SHORT loc_1_2923
        db  02Eh, 08Bh, 0C0h
sub_1_2930:
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 8], 0
        jl     SHORT loc_1_293E
        mov    edi, dword ptr [esi + 8]
        add    dword ptr [eax + 8], edi
loc_1_293E:
        mov    edi, dword ptr [esi + 4]
        mov    dword ptr [edi], eax
        mov    dword ptr [eax + 4], edi
        xor    eax, eax
        mov    dword ptr [esi + 4], eax
        ret
Cancel_Time_Out:
        test   esi, esi
        je     SHORT loc_1_296F
        pushfd
        cli
        push   eax
        push   edi
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_296C
        call   sub_1_2930
        db  0A1h, 024h, 0DAh, 000h, 000h
        mov    dword ptr [esi], eax
        db  089h, 035h, 024h, 0DAh, 000h, 000h
loc_1_296C:
        pop    edi
        pop    eax
        popfd
loc_1_296F:
        ret
Get_System_Time:
        db  0CDh, 020h, 001h, 000h, 005h, 000h
Get_Last_Updated_System_Time:
        db  08Bh, 005h, 020h, 0DAh, 000h, 000h
        ret
loc_1_297D:
        db  0B8h, 020h, 0DAh, 000h, 000h
        ret
        db  090h
Get_VM_Exec_Time:
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_1_2992
        db  0CDh, 020h, 001h, 000h, 005h, 000h
loc_1_2992:
        mov    eax, eax
Get_Last_Updated_VM_Exec_Time:
        mov    eax, dword ptr [ebx + 14h]
        ret
        db  090h
loc_1_299C:
        mov    eax, dword ptr [esp + 4]
        db  03Bh, 005h, 098h, 0F6h, 000h, 000h
        jne    SHORT loc_1_29AE
        db  0CDh, 020h, 001h, 000h, 005h, 000h
loc_1_29AE:
        mov    eax, dword ptr [eax - 4ch]
        ret
        db  08Bh, 0C0h
_GetLastUpdatedThreadExecTime:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax - 4ch]
        ret
Update_System_Clock:
        db  083h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h
        je     NEAR PTR loc_1_2A81
        push   ebx
        push   edx
        push   esi
        push   edi
        db  001h, 00Dh, 020h, 0DAh, 000h, 000h
        db  0BAh, 000h, 0DAh, 000h, 000h
        mov    esi, dword ptr [edx]
        cmp    esi, edx
        je     SHORT loc_1_29F3
        sub    dword ptr [esi + 8], ecx
        jg     SHORT loc_1_29F3
        add    dword ptr [esi + 8], ecx
        add    dword ptr [edx + 0ch], ecx
        cmp    dword ptr [edx + 0ch], ecx
        jne    SHORT loc_1_29F3
        call   sub_1_2A84
loc_1_29F3:
        sti
        db  0BAh, 010h, 0DAh, 000h, 000h
        cli
        mov    esi, dword ptr [edx]
        cmp    esi, edx
        je     SHORT loc_1_2A1C
        sub    dword ptr [esi + 8], ecx
        jg     SHORT loc_1_2A1C
        add    dword ptr [esi + 8], ecx
        add    dword ptr [edx + 0ch], ecx
        cmp    dword ptr [edx + 0ch], ecx
        jne    SHORT loc_1_2A1C
        sti
        db  0BEh, 0CEh, 02Ah, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_1_2A1C:
        sti
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        add    dword ptr [ebx + 14h], ecx
        lea    edx, [ebx + 38h]
        cli
        mov    esi, dword ptr [edx]
        cmp    esi, edx
        je     SHORT loc_1_2A4C
        sub    dword ptr [esi + 8], ecx
        jg     SHORT loc_1_2A4C
        add    dword ptr [esi + 8], ecx
        add    dword ptr [edx + 0ch], ecx
        cmp    dword ptr [edx + 0ch], ecx
        jne    SHORT loc_1_2A4C
        sti
        db  0BEh, 0CEh, 02Ah, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
loc_1_2A4C:
        sti
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        add    dword ptr [edi - 4ch], ecx
        lea    edx, [edi - 48h]
        cli
        mov    esi, dword ptr [edx]
        cmp    esi, edx
        je     SHORT loc_1_2A7D
        sub    dword ptr [esi + 8], ecx
        jg     SHORT loc_1_2A7D
        add    dword ptr [esi + 8], ecx
        add    dword ptr [edx + 0ch], ecx
        cmp    dword ptr [edx + 0ch], ecx
        jne    SHORT loc_1_2A7D
        sti
        db  0BEh, 0CEh, 02Ah, 000h, 000h
        db  0CDh, 020h, 0FBh, 000h, 001h, 000h
        cli
loc_1_2A7D:
        pop    edi
        pop    esi
        pop    edx
        pop    ebx
loc_1_2A81:
        ret
        db  08Bh, 0C0h
sub_1_2A84:
        push   eax
        push   ebx
        push   ecx
        push   edi
        mov    ecx, dword ptr [edx + 0ch]
        sub    ecx, dword ptr [esi + 8]
loc_1_2A8E:
        mov    edx, dword ptr [esi + 10h]
        mov    ebx, dword ptr [esi + 0ch]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  0FFh, 005h, 0F8h, 0E3h, 000h, 000h
        sti
        call   ebx
        cli
        db  0FFh, 00Dh, 0F8h, 0E3h, 000h, 000h
        db  0BAh, 000h, 0DAh, 000h, 000h
        mov    esi, dword ptr [edx]
        cmp    esi, edx
        je     SHORT loc_1_2AC2
        mov    ecx, dword ptr [edx + 0ch]
        sub    ecx, dword ptr [esi + 8]
        jge    SHORT loc_1_2A8E
        neg    ecx
        mov    dword ptr [esi + 8], ecx
loc_1_2AC2:
        mov    dword ptr [edx + 0ch], 0
        pop    edi
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_1_2ACE:
        cli
        cmp    dword ptr [edx], edx
        je     NEAR PTR loc_1_2B61
        mov    ecx, dword ptr [edx + 0ch]
        mov    esi, dword ptr [edx]
        sub    ecx, dword ptr [esi + 8]
        jl     SHORT loc_1_2B5C
        call   sub_1_2930
        push   esi
        cmp    dword ptr [edx], edx
        jne    SHORT loc_1_2B18
loc_1_2AEB:
        mov    dword ptr [edx + 0ch], 0
        sti
loc_1_2AF3:
        mov    esi, dword ptr [esp]
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    edx, dword ptr [esi + 10h]
        call   dword ptr [esi + 0ch]
loc_1_2B02:
        pop    esi
        cli
        db  0A1h, 024h, 0DAh, 000h, 000h
        mov    dword ptr [esi], eax
        xor    eax, eax
        mov    dword ptr [esi + 4], eax
        db  089h, 035h, 024h, 0DAh, 000h, 000h
        sti
        ret
loc_1_2B18:
        mov    edi, dword ptr [edx + 0ch]
        mov    esi, dword ptr [edx]
        sub    edi, dword ptr [esi + 8]
        jge    SHORT loc_1_2B29
        neg    edi
        mov    dword ptr [esi + 8], edi
        jmp    SHORT loc_1_2AEB
loc_1_2B29:
        sti
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  0BEh, 0CEh, 02Ah, 000h, 000h
        db  081h, 0FAh, 010h, 0DAh, 000h, 000h
        jne    SHORT loc_1_2B45
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_2AF3
loc_1_2B45:
        lea    eax, [ebx + 38h]
        cmp    edx, eax
        jne    SHORT loc_1_2B54
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        jmp    SHORT loc_1_2AF3
loc_1_2B54:
        db  0CDh, 020h, 0FBh, 000h, 001h, 000h
        jmp    SHORT loc_1_2AF3
loc_1_2B5C:
        neg    ecx
        mov    dword ptr [esi + 8], ecx
loc_1_2B61:
        mov    dword ptr [edx + 0ch], 0
        sti
        ret
        db  000h, 000h
Enable_Global_Trapping:
        push   edi
        call   sub_1_2D40
        jne    SHORT loc_1_2BAE
        db  00Fh, 0A3h, 03Dh, 030h, 0DDh, 000h, 000h
        jae    SHORT loc_1_2B8B
        mov    edi, edx
        and    edi, 1ffh
        add    edi, 180h
loc_1_2B8B:
        db  00Fh, 0ABh, 03Dh, 064h, 0E3h, 000h, 000h
        push   ebx
        db  08Bh, 01Dh, 014h, 0F7h, 000h, 000h
loc_1_2B99:
        bts    dword ptr [ebx + 48h], edi
        mov    ebx, dword ptr [ebx + 2ch]
        test   ebx, ebx
        jne    SHORT loc_1_2B99
        pop    ebx
loc_1_2BA5:
        db  00Fh, 0ABh, 015h, 004h, 09Fh, 000h, 000h
loc_1_2BAC:
        pop    edi
        ret
loc_1_2BAE:
        jmp    SHORT loc_1_2BAC
Enable_Local_Trapping:
        push   edi
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT loc_1_2BF0
loc_1_2BCB:
        db  00Fh, 0A3h, 03Dh, 030h, 0DDh, 000h, 000h
        jae    SHORT loc_1_2BE2
        mov    edi, edx
        and    edi, 1ffh
        add    edi, 180h
loc_1_2BE2:
        bts    dword ptr [ebx + 48h], edi
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        je     SHORT loc_1_2BA5
loc_1_2BEE:
        pop    edi
        ret
loc_1_2BF0:
        call   sub_1_2D6C
        je     SHORT loc_1_2BCB
        jmp    SHORT loc_1_2BEE
        db  02Eh, 08Bh, 0C0h
Disable_Global_Trapping:
        push   edi
        call   sub_1_2D40
        jne    SHORT loc_1_2C3E
        db  00Fh, 0A3h, 03Dh, 030h, 0DDh, 000h, 000h
        jae    SHORT loc_1_2C1B
        mov    edi, edx
        and    edi, 1ffh
        add    edi, 180h
loc_1_2C1B:
        db  00Fh, 0B3h, 03Dh, 064h, 0E3h, 000h, 000h
        push   ebx
        db  08Bh, 01Dh, 014h, 0F7h, 000h, 000h
loc_1_2C29:
        btr    dword ptr [ebx + 48h], edi
        mov    ebx, dword ptr [ebx + 2ch]
        test   ebx, ebx
        jne    SHORT loc_1_2C29
        pop    ebx
loc_1_2C35:
        db  00Fh, 0B3h, 015h, 004h, 09Fh, 000h, 000h
loc_1_2C3C:
        pop    edi
        ret
loc_1_2C3E:
        jmp    SHORT loc_1_2C3C
Disable_Local_Trapping:
        push   edi
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT loc_1_2C80
loc_1_2C5B:
        db  00Fh, 0A3h, 03Dh, 030h, 0DDh, 000h, 000h
        jae    SHORT loc_1_2C72
        mov    edi, edx
        and    edi, 1ffh
        add    edi, 180h
loc_1_2C72:
        btr    dword ptr [ebx + 48h], edi
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        je     SHORT loc_1_2C35
loc_1_2C7E:
        pop    edi
        ret
loc_1_2C80:
        call   sub_1_2D6C
        je     SHORT loc_1_2C5B
        jmp    SHORT loc_1_2C7E
        db  02Eh, 08Bh, 0C0h
sub_1_2C8C:
        push   edi
        lea    esi, [ebx + 78h]
        db  0BFh, 004h, 09Fh, 000h, 000h
        mov    ecx, 7
        cld
        rep movsd
        mov    ecx, 9
        add    edi, 40h
        rep movsd
        db  083h, 03Dh, 02Ch, 0DAh, 000h, 000h, 000h
        jne    SHORT loc_1_2CB2
        pop    edi
        ret
loc_1_2CB2:
        xor    edx, edx
        mov    edi, 180h
        mov    ebp, 78h
loc_1_2CBE:
        sub    edi, 20h
        jl     SHORT loc_1_2CF7
loc_1_2CC3:
        sub    ebp, 4
        db  0A1h, 00Ch, 0F7h, 000h, 000h
        mov    eax, dword ptr [ebp + eax]
        xor    eax, dword ptr [ebp + ebx]
        je     SHORT loc_1_2CBE
        mov    esi, edi
loc_1_2CD7:
        bsf    ecx, eax
        add    esi, ecx
        db  066h, 08Bh, 014h, 075h, 030h, 0DAh, 000h, 000h
        db  00Fh, 0BBh, 015h, 004h, 09Fh, 000h, 000h
        inc    esi
        shr    eax, cl
        shr    eax, 1
        jne    SHORT loc_1_2CD7
        sub    edi, 20h
        jge    SHORT loc_1_2CC3
loc_1_2CF7:
        pop    edi
        ret
sub_1_2CF9:
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_2D0C
loc_1_2CFE:
        test   edi, 20000h
        jne    SHORT loc_1_2D20
loc_1_2D06:
        mov    ecx, 8
        ret
loc_1_2D0C:
        lar    ecx, word ptr [ebp + 28h]
        test   ecx, 400000h
        je     SHORT loc_1_2CFE
        test   edi, 20000h
        jne    SHORT loc_1_2D06
loc_1_2D20:
        mov    ecx, 10h
        ret
sub_1_2D26:
        db  081h, 03Ch, 0BDh, 060h, 0DDh, 000h, 000h, 052h, 02Fh, 000h, 000h
        je     SHORT loc_1_2D3E
        db  081h, 03Ch, 0BDh, 060h, 0DDh, 000h, 000h, 028h, 02Fh, 000h, 000h
loc_1_2D3E:
        ret
        db  090h
sub_1_2D40:
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT sub_1_2D6C
        test   edx, edx
        je     SHORT loc_1_2D61
loc_1_2D5E:
        cmp    edx, edx
        ret
loc_1_2D61:
        call   sub_1_2D26
        jne    SHORT loc_1_2D5E
        jmp    SHORT loc_1_2D9C
        db  08Bh, 0C0h
sub_1_2D6C:
        push   eax
        mov    eax, edi
loc_1_2D6F:
        db  081h, 03Ch, 0BDh, 060h, 0DDh, 000h, 000h, 052h, 02Fh, 000h, 000h
        je     SHORT loc_1_2D9B
        dec    edi
        js     SHORT loc_1_2D8F
loc_1_2D7F:
        cmp    eax, edi
        je     SHORT loc_1_2D96
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT loc_1_2D6F
        pop    eax
        ret
loc_1_2D8F:
        mov    edi, 17fh
        jmp    SHORT loc_1_2D7F
loc_1_2D96:
        mov    edi, 180h
loc_1_2D9B:
        pop    eax
loc_1_2D9C:
        cmp    edi, -1
        ret
loc_1_2DA0:
        inc    dword ptr [ebp + 24h]
        movzx  edx, dx
loc_1_2DA6:
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT loc_1_2DCD
loc_1_2DC0:
        xor    ecx, ecx
        db  0FFh, 014h, 0BDh, 060h, 0DDh, 000h, 000h
        mov    byte ptr [ebp + 1ch], al
        ret
loc_1_2DCD:
        db  068h, 0C0h, 02Dh, 000h, 000h
        jmp    SHORT sub_1_2D6C
loc_1_2DD4:
        add    dword ptr [ebp + 24h], 2
        movzx  edx, ch
        jmp    SHORT loc_1_2DA6
        db  02Eh, 08Bh, 0C0h
loc_1_2DE0:
        inc    dword ptr [ebp + 24h]
        movzx  edx, dx
loc_1_2DE6:
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT loc_1_2E0C
loc_1_2E00:
        mov    ecx, 4
        db  0FFh, 024h, 0BDh, 060h, 0DDh, 000h, 000h
loc_1_2E0C:
        db  068h, 000h, 02Eh, 000h, 000h
        jmp    NEAR PTR sub_1_2D6C
        db  08Bh, 0C0h
loc_1_2E18:
        add    dword ptr [ebp + 24h], 2
        movzx  edx, ch
        jmp    SHORT loc_1_2DE6
        db  02Eh, 08Bh, 0C0h
loc_1_2E24:
        inc    dword ptr [ebp + 24h]
        movzx  edx, dx
        call   sub_1_2CF9
loc_1_2E2F:
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT loc_1_2E53
loc_1_2E49:
        or     ecx, 4
        db  0FFh, 024h, 0BDh, 060h, 0DDh, 000h, 000h
loc_1_2E53:
        db  068h, 049h, 02Eh, 000h, 000h
        jmp    NEAR PTR sub_1_2D6C
        db  02Eh, 08Bh, 0C0h
loc_1_2E60:
        xor    ecx, ecx
        call   sub_1_2EA4
        mov    ah, al
        mov    al, bl
        ret
loc_1_2E6C:
        add    ecx, -8
        call   sub_1_2EA4
        shl    eax, 10h
        mov    ax, bx
        ret
        db  090h
loc_1_2E7C:
        add    ecx, -8
        mov    ebx, eax
        shr    ebx, 10h
        jmp    SHORT sub_1_2EA4
        db  08Bh, 0C0h
Simulate_IO:
        and    cl, 0fch
        cmp    ecx, 14h
        db  00Fh, 087h, 0AEh, 0E8h, 0FEh, 0FFh
        db  0FFh, 0A1h, 0DCh, 0E3h, 000h, 000h
        db  08Bh, 0C0h
loc_1_2E9C:
        add    ecx, -8
        mov    bl, ah
        mov    eax, eax
sub_1_2EA4:
        push   ebx
        push   ecx
        push   edx
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT loc_1_2F00
loc_1_2EC1:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        call   sub_1_2F14
        pop    edx
        pop    ecx
        xchg   dword ptr [esp], eax
        inc    edx
        bt     ecx, 3
        adc    edx, 0
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        db  066h, 039h, 014h, 07Dh, 030h, 0DAh, 000h, 000h
        jne    SHORT loc_1_2F0A
loc_1_2EF3:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        call   sub_1_2F14
        pop    ebx
        ret
loc_1_2F00:
        db  068h, 0C1h, 02Eh, 000h, 000h
        jmp    NEAR PTR sub_1_2D6C
loc_1_2F0A:
        db  068h, 0F3h, 02Eh, 000h, 000h
        jmp    NEAR PTR sub_1_2D6C
sub_1_2F14:
        db  081h, 03Ch, 0BDh, 060h, 0DDh, 000h, 000h, 052h, 02Fh, 000h, 000h
        je     SHORT loc_1_2F28
        db  0FFh, 024h, 0BDh, 060h, 0DDh, 000h, 000h
loc_1_2F28:
        cmp    ecx, 14h
        db  00Fh, 087h, 011h, 0E8h, 0FEh, 0FFh
        cmp    ecx, 4
        ja     SHORT loc_1_2F3C
        jb     SHORT loc_1_2F3A
        out    dx, al
        ret
loc_1_2F3A:
        in     al, dx
        ret
loc_1_2F3C:
        cmp    ecx, 0ch
        ja     SHORT loc_1_2F49
        jb     SHORT loc_1_2F46
        out    dx, ax
        ret
loc_1_2F46:
        in     ax, dx
        ret
loc_1_2F49:
        cmp    ecx, 14h
        jb     SHORT loc_1_2F50
        out    dx, eax
        ret
loc_1_2F50:
        in     eax, dx
        ret
loc_1_2F52:
        db  0FFh, 025h, 0D4h, 0E3h, 000h, 000h
loc_1_2F58:
        test   byte ptr [ebx + 1ch], 80h
        jne    SHORT loc_1_2F28
        and    ecx, 1fh
        test   byte ptr [ebx + 1dh], 1
        jne    SHORT loc_1_2F80
        pushad
        shl    ecx, 10h
        or     edx, ecx
        xor    eax, eax
        xor    ebx, ebx
        push   2
        pop    ecx
        db  0BEh, 065h, 00Dh, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        popad
loc_1_2F80:
        cmp    ecx, 4
        jbe    SHORT loc_1_2F9C
        call   sub_1_2FAE
        inc    edx
        cmp    ecx, 0ch
        jbe    SHORT loc_1_2F9C
        call   sub_1_2FAE
        inc    edx
        call   sub_1_2FAE
        inc    edx
loc_1_2F9C:
        call   sub_1_2FAE
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        mov    eax, dword ptr [edi - 58h]
        mov    dword ptr [ebp + 24h], eax
        ret
sub_1_2FAE:
        push   ecx
        push   ebx
        test   byte ptr [ebx + 1dh], 1
        jne    SHORT loc_1_3013
        db  08Bh, 01Dh, 0D8h, 0E3h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_1_3013
        mov    edi, edx
        rol    dl, 1
        xor    dl, dh
        movzx  edx, dl
        lea    edx, [edx + edx*2]
        shr    edx, 1
        xchg   edi, edx
        mov    ecx, 180h
loc_1_2FD5:
        cmp    dword ptr [ebx + edi*4 + 300h], 0
        je     SHORT loc_1_2FF3
        cmp    word ptr [ebx + edi*2], dx
        je     SHORT loc_1_3013
        dec    edi
        js     SHORT loc_1_2FEC
loc_1_2FE8:
        loop   loc_1_2FD5
        jmp    SHORT loc_1_3013
loc_1_2FEC:
        mov    edi, 17fh
        jmp    SHORT loc_1_2FE8
loc_1_2FF3:
        mov    word ptr [ebx + edi*2], dx
        mov    ax, word ptr [ebp + 28h]
        mov    word ptr [ebx + edi*4 + 300h], ax
        db  0A1h, 098h, 0F6h, 000h, 000h
        mov    eax, dword ptr [eax - 58h]
        mov    word ptr [ebx + edi*4 + 302h], ax
loc_1_3013:
        db  00Fh, 0B3h, 015h, 004h, 09Fh, 000h, 000h
        cmp    edx, 0e0h
        jb     SHORT loc_1_3032
        cmp    edx, 2e0h
        jb     SHORT loc_1_3051
        cmp    edx, 400h
        jae    SHORT loc_1_3051
loc_1_3032:
        mov    eax, edx
        and    eax, 1ffh
        db  00Fh, 0B3h, 005h, 094h, 0E3h, 000h, 000h
        db  08Bh, 01Dh, 014h, 0F7h, 000h, 000h
loc_1_3046:
        btr    dword ptr [ebx + 78h], eax
        mov    ebx, dword ptr [ebx + 2ch]
        test   ebx, ebx
        jne    SHORT loc_1_3046
loc_1_3051:
        pop    ebx
        pop    ecx
        ret
loc_1_3054:
        sub    esp, 4
        pushad
        mov    bx, ss
        cmp    bx, 30h
        jne    SHORT loc_1_306B
        mov    esi, 8
        jmp    NEAR PTR loc_1_02C0
loc_1_306B:
        push   ds
        push   es
        push   fs
        push   gs
        mov    bx, 30h
        mov    ds, bx
        mov    es, bx
        xor    ebx, ebx
        mov    fs, bx
        mov    gs, bx
        call   sub_1_3294
        pop    gs
        pop    fs
        pop    es
        pop    ds
        popad
        add    esp, 4
        iretd
        db  090h
loc_1_3094:
        call   sub_1_0050
loc_1_3099:
        mov    eax, eax
loc_1_309C:
        call   sub_1_0050
        mov    eax, eax
loc_1_30A4:
        call   sub_1_0050
        mov    eax, eax
loc_1_30AC:
        call   sub_1_0050
        mov    eax, eax
loc_1_30B4:
        call   sub_1_0050
        mov    eax, eax
loc_1_30BC:
        call   sub_1_0050
        mov    eax, eax
loc_1_30C4:
        call   sub_1_0050
        mov    eax, eax
loc_1_30CC:
        call   sub_1_0050
        mov    eax, eax
loc_1_30D4:
        call   sub_1_0050
        mov    eax, eax
loc_1_30DC:
        call   sub_1_0050
        mov    eax, eax
loc_1_30E4:
        call   sub_1_0050
        mov    eax, eax
loc_1_30EC:
        call   sub_1_0050
        mov    eax, eax
loc_1_30F4:
        call   sub_1_0050
        mov    eax, eax
loc_1_30FC:
        call   sub_1_0050
        mov    eax, eax
loc_1_3104:
        call   sub_1_0050
        mov    eax, eax
loc_1_310C:
        call   sub_1_0050
        mov    eax, eax
loc_1_3114:
        call   sub_1_00E4
        mov    eax, eax
loc_1_311C:
        call   sub_1_0050
        mov    eax, eax
loc_1_3124:
        call   sub_1_00E4
        mov    eax, eax
loc_1_312C:
        call   sub_1_0050
        mov    eax, eax
loc_1_3134:
        call   sub_1_0050
        mov    eax, eax
loc_1_313C:
        call   sub_1_0050
        mov    eax, eax
loc_1_3144:
        call   sub_1_0050
        mov    eax, eax
loc_1_314C:
        call   sub_1_0050
        mov    eax, eax
loc_1_3154:
        call   sub_1_0050
        mov    eax, eax
loc_1_315C:
        call   sub_1_0050
        mov    eax, eax
loc_1_3164:
        call   sub_1_0050
        mov    eax, eax
loc_1_316C:
        call   sub_1_0050
        mov    eax, eax
loc_1_3174:
        call   sub_1_0050
        mov    eax, eax
loc_1_317C:
        call   sub_1_0050
        mov    eax, eax
loc_1_3184:
        call   sub_1_0050
        mov    eax, eax
loc_1_318C:
        call   sub_1_0050
        mov    eax, eax
        call   sub_1_0050
        mov    eax, eax
loc_1_319C:
        call   sub_1_0050
        mov    eax, eax
loc_1_31A4:
        call   sub_1_0050
        mov    eax, eax
loc_1_31AC:
        call   sub_1_0050
        mov    eax, eax
loc_1_31B4:
        call   sub_1_0050
        mov    eax, eax
loc_1_31BC:
        call   sub_1_0050
        mov    eax, eax
loc_1_31C4:
        call   sub_1_0050
        mov    eax, eax
loc_1_31CC:
        call   sub_1_0050
        mov    eax, eax
loc_1_31D4:
        call   sub_1_0050
        mov    eax, eax
loc_1_31DC:
        call   sub_1_0050
        mov    eax, eax
loc_1_31E4:
        call   sub_1_0050
        mov    eax, eax
loc_1_31EC:
        call   sub_1_0050
        mov    eax, eax
loc_1_31F4:
        call   sub_1_0050
        mov    eax, eax
loc_1_31FC:
        call   sub_1_0050
        mov    eax, eax
loc_1_3204:
        call   sub_1_0050
        mov    eax, eax
loc_1_320C:
        call   sub_1_0050
        mov    eax, eax
loc_1_3214:
        call   sub_1_00E4
        mov    eax, eax
loc_1_321C:
        call   sub_1_00E4
        mov    eax, eax
loc_1_3224:
        call   sub_1_0050
        mov    eax, eax
loc_1_322C:
        call   sub_1_00E4
        mov    eax, eax
loc_1_3234:
        call   sub_1_00E4
        mov    eax, eax
loc_1_323C:
        call   sub_1_00E4
        mov    eax, eax
loc_1_3244:
        call   sub_1_00E4
        mov    eax, eax
loc_1_324C:
        call   sub_1_00E4
        mov    eax, eax
loc_1_3254:
        call   sub_1_00E4
        mov    eax, eax
loc_1_325C:
        call   sub_1_00E4
        mov    eax, eax
loc_1_3264:
        call   sub_1_00E4
        mov    eax, eax
loc_1_326C:
        call   sub_1_00E4
        mov    eax, eax
loc_1_3274:
        call   sub_1_00E4
        mov    eax, eax
loc_1_327C:
        call   sub_1_00E4
        mov    eax, eax
loc_1_3284:
        call   sub_1_00E4
        mov    eax, eax
loc_1_328C:
        call   sub_1_00E4
        mov    eax, eax
sub_1_3294:
        db  089h, 025h, 0B0h, 0ECh, 000h, 000h
        db  066h, 08Ch, 015h, 0B4h, 0ECh, 000h, 000h
        db  0BCh, 0ACh, 0ECh, 000h, 000h
        mov    ax, ds
        mov    ss, ax
        db  0A1h, 020h, 0DAh, 000h, 000h
        db  0FFh, 015h, 0ACh, 0ECh, 000h, 000h
        db  0FFh, 005h, 0E4h, 0D9h, 000h, 000h
        db  066h, 08Eh, 015h, 0B4h, 0ECh, 000h, 000h
        db  08Bh, 025h, 0B0h, 0ECh, 000h, 000h
        ret
loc_1_32CB:
        sti
        mov    eax, esi
        shr    eax, 2
        mov    esi, eax
        mov    cx, 9
        call   sub_1_3410
        jae    SHORT loc_1_32DF
        ret
loc_1_32DF:
        db  0CDh, 020h, 046h, 080h, 001h, 000h
loc_1_32E5:
        shr    esi, 2
        mov    cx, 11h
        call   sub_1_3410
        jae    SHORT loc_1_333E
        ret
loc_1_32F4:
        shr    esi, 2
        mov    cx, 12h
        call   sub_1_3410
        jae    SHORT loc_1_333E
        ret
loc_1_3303:
        mov    eax, dword ptr [ebp + 24h]
        db  08Bh, 00Dh, 0F4h, 0E3h, 000h, 000h
loc_1_330C:
        jecxz  loc_1_3323
        cmp    eax, dword ptr [ecx + 4]
        jb     SHORT loc_1_331F
        cmp    eax, dword ptr [ecx + 8]
        ja     SHORT loc_1_331F
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [ebp + 24h], eax
loc_1_331E:
        ret
loc_1_331F:
        mov    ecx, dword ptr [ecx]
        jmp    SHORT loc_1_330C
loc_1_3323:
        shr    esi, 2
        mov    cx, 14h
        call   sub_1_3410
        jb     SHORT loc_1_331E
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        call   sub_1_2384
        jae    SHORT loc_1_331E
loc_1_333E:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    al, byte ptr [edi - 33h]
        test   al, al
        je     SHORT loc_1_3355
        cmp    al, byte ptr [edi - 32h]
        ja     SHORT loc_1_3355
        jmp    NEAR PTR sub_1_25DB
loc_1_3355:
        sti
        pushad
        xor    eax, eax
        xor    edx, edx
        db  0A3h, 0F8h, 0E3h, 000h, 000h
        db  0CDh, 020h, 090h, 001h, 001h, 000h
        popad
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_3390
        db  0CDh, 020h, 00Bh, 001h, 001h, 000h
        je     SHORT loc_1_3390
        movzx  ebx, word ptr [ebp + 28h]
        mov    edi, dword ptr [ebp + 24h]
        db  0CDh, 020h, 070h, 001h, 001h, 000h
        mov    edx, 2
        db  0CDh, 020h, 017h, 000h, 02Ah, 000h
        ret
loc_1_3390:
        mov    eax, 10002h
        cmp    esi, 6
        je     SHORT loc_1_33B3
        mov    eax, 10004h
        cmp    esi, 0dh
        je     SHORT loc_1_33B3
        mov    eax, 10003h
        cmp    esi, 0eh
        je     SHORT loc_1_33B3
        mov    eax, 10005h
loc_1_33B3:
        lea    edx, [ebx + 19ch]
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [edx], ecx
        mov    cx, word ptr [ebp + 28h]
        mov    word ptr [edx + 4], cx
        push   eax
        sub    eax, eax
        sub    esi, esi
loc_1_33CB:
        db  0CDh, 020h, 00Bh, 000h, 003h, 000h
        test   cl, 2
        je     SHORT loc_1_33D9
        bts    esi, eax
loc_1_33D9:
        inc    eax
        cmp    al, 10h
        jb     SHORT loc_1_33CB
        pop    eax
        mov    word ptr [edx + 6], si
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        db  0CDh, 020h, 02Fh, 080h, 001h, 000h
loc_1_33F2:
        db  0F7h, 005h, 0F8h, 033h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_1_3403
        db  0E9h, 0D1h, 014h, 0FFh, 0FFh
loc_1_3403:
        shr    esi, 2
        mov    cx, 14h
        call   sub_1_3410
        ret
sub_1_3410:
        pushad
        db  0CDh, 020h, 000h, 000h, 002h, 000h
        or     eax, eax
        je     SHORT loc_1_3423
        mov    ebx, ebp
        db  0CDh, 020h, 002h, 000h, 002h, 000h
loc_1_3423:
        popad
        ret
loc_1_3425:
        and    byte ptr [ebp + 2dh], 0bfh
        ret
        db  08Bh, 0C0h
Get_VMM_Reenter_Count:
        db  08Bh, 00Dh, 0F8h, 0E3h, 000h, 000h
        ret
Begin_Reentrant_Execution:
        xor    ecx, ecx
        db  087h, 00Dh, 0F8h, 0E3h, 000h, 000h
        ret
End_Reentrant_Execution:
        db  089h, 00Dh, 0F8h, 0E3h, 000h, 000h
        ret
        db  000h
loc_1_3444:
        db  08Bh, 035h, 098h, 09Bh, 000h, 000h
        mov    ebx, esi
        les    edi, ptr [esi + 6]
        movzx  ecx, word ptr [esi + 0ch]
        shr    ecx, 2
        cld
        rep movsd
        db  083h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h
        jne    SHORT loc_1_347D
        db  08Bh, 00Dh, 0E0h, 071h, 000h, 000h
        db  081h, 0E9h, 007h, 06Fh, 000h, 000h
        db  0BEh, 010h, 06Fh, 000h, 000h
        add    ecx, 3
        shr    ecx, 2
        mov    eax, ecx
        rep movsd
loc_1_347D:
        or     eax, 0ffffffffh
        stosd
        jmp    ptr [ebx + 0eh]
loc_1_3484:
        cmp    byte ptr [ebp + 1dh], 16h
        je     SHORT loc_1_349C
        db  08Bh, 00Dh, 0D4h, 0ECh, 000h, 000h
        db  08Bh, 015h, 0D8h, 0ECh, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_349C:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        jmp    SHORT loc_1_34AC
loc_1_34A4:
        cmp    byte ptr [ebp + 1dh], 16h
        je     SHORT loc_1_34AC
loc_1_34AA:
        stc
        ret
loc_1_34AC:
        movzx  eax, byte ptr [ebp + 1ch]
        btr    eax, 7
        jb     SHORT loc_1_34C9
        cmp    eax, 0ah
        je     SHORT loc_1_34FE
        cmp    eax, 2
        ja     SHORT loc_1_34AA
        db  0FFh, 024h, 085h, 0DCh, 0ECh, 000h, 000h
loc_1_34C9:
        cmp    eax, 0dh
        ja     SHORT loc_1_34AA
        db  0FFh, 024h, 085h, 0E8h, 0ECh, 000h, 000h
loc_1_34D7:
        call   sub_1_3606
        jb     SHORT loc_1_34AA
        mov    ax, 2824h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    dword ptr [eax], 0c75033ch
        mov    ax, 0a03h
        je     SHORT loc_1_34F8
        mov    ah, 0
        mov    al, 4
loc_1_34F8:
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_1_34FE:
        call   sub_1_3606
        jb     SHORT loc_1_34AA
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 10h], 400h
        mov    word ptr [ebp + 18h], 3
        clc
        ret
loc_1_3519:
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_352E
        db  0A1h, 0C8h, 0ECh, 000h, 000h
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
loc_1_352E:
        clc
        ret
loc_1_3530:
        mov    edx, ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        jecxz  loc_1_353E
        cmp    edx, ebx
        je     SHORT loc_1_3544
loc_1_353E:
        db  0CDh, 020h, 038h, 000h, 001h, 000h
loc_1_3544:
        mov    byte ptr [ebp + 1ch], 0
        clc
        ret
loc_1_354A:
        db  0BAh, 0A4h, 0D3h, 000h, 000h
        mov    ecx, 5
        jmp    SHORT loc_1_355B
loc_1_3556:
        db  0BAh, 0A8h, 0D3h, 000h, 000h
loc_1_355B:
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_1_3568
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_356A
loc_1_3568:
        call   dword ptr [edx]
loc_1_356A:
        jmp    SHORT loc_1_3544
loc_1_356C:
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [ebp + 10h], ax
        jmp    SHORT loc_1_3544
loc_1_3575:
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_3580
        mov    word ptr [ebp + 1ch], 0
loc_1_3580:
        clc
        ret
loc_1_3582:
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        jmp    SHORT loc_1_3544
loc_1_358A:
        xchg   esi, eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        db  00Fh, 0B7h, 03Dh, 012h, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        cld
loc_1_35A4:
        cmpsb
        jne    NEAR PTR loc_1_34AA
        cmp    byte ptr [esi - 1], 0
        jne    SHORT loc_1_35A4
        db  0A1h, 0CCh, 0ECh, 000h, 000h
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        test   word ptr [ebx], 180h
        je     NEAR PTR loc_1_3544
        mov    word ptr [ebp + 2], 0
        jmp    NEAR PTR loc_1_3544
loc_1_35D7:
        movzx  ecx, word ptr [ebp + 10h]
        jecxz  loc_1_35F2
loc_1_35DD:
        cmp    dword ptr [ebx + 0ch], ecx
        je     SHORT loc_1_35F2
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        jne    SHORT loc_1_35DD
        jmp    SHORT loc_1_3580
loc_1_35F2:
        mov    eax, 0fh
        xor    edx, edx
        xor    esi, esi
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_3544
sub_1_3606:
        test   byte ptr [ebx + 27h], 80h
        db  00Fh, 085h, 06Ch, 0E4h, 0FEh, 0FFh
        ret
loc_1_3611:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        jmp    dword ptr [edx]
        db  000h, 000h, 000h
loc_1_361C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 14h]
        jecxz  loc_1_3642
        or     esi, esi
        jne    SHORT loc_1_3635
        db  0BEh, 03Fh, 036h, 000h, 000h
loc_1_3635:
        lodsb
        stosb
        or     al, al
        loopne loc_1_3635
        inc    ecx
        dec    edi
        mov    byte ptr [edi], 0
        mov    edx, edi
loc_1_3642:
        mov    eax, dword ptr [esp + 0ch]
        pop    edi
        pop    esi
        ret
loc_1_3649:
        push   edi
        mov    edi, dword ptr [esp + 8]
        xor    eax, eax
        test   edi, edi
        je     SHORT loc_1_3661
        lea    ecx, [eax - 1]
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        lea    edx, [edi - 1]
loc_1_3661:
        pop    edi
        ret
loc_1_3663:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, edi
        cmp    ecx, 40h
        jbe    SHORT loc_1_3693
        mov    edx, ecx
        mov    ecx, esi
        neg    ecx
        and    ecx, 3
        je     SHORT loc_1_3687
        sub    edx, ecx
        rep movsb
loc_1_3687:
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
loc_1_3693:
        rep movsb
        mov    edx, edi
        pop    edi
        pop    esi
        ret
        db  000h, 000h
sub_1_369C:
        mov    eax, dword ptr [esp + 4]
        bts    dword ptr [eax + 3], 1
        jb     SHORT loc_1_36AA
loc_1_36A7:
        ret    4
loc_1_36AA:
        or     byte ptr [eax + 3], 4
        push   0
        push   eax
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        xor    eax, eax
        jmp    SHORT loc_1_36A7
sub_1_36BD:
        mov    eax, dword ptr [esp + 4]
        and    byte ptr [eax + 3], 0fdh
        btr    dword ptr [eax + 3], 2
        jb     SHORT loc_1_36CF
loc_1_36CC:
        ret    4
loc_1_36CF:
        push   eax
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
        jmp    SHORT loc_1_36CC
sub_1_36D9:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_1_36E9
        xor    ecx, ecx
        db  0CDh, 020h, 027h, 000h, 001h, 000h
loc_1_36E9:
        ret    4
sub_1_36EC:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_1_36FA
        db  0CDh, 020h, 028h, 000h, 001h, 000h
loc_1_36FA:
        ret    4
sub_1_36FD:
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 080h, 001h, 000h
sub_1_3708:
        db  0CDh, 020h, 020h, 080h, 001h, 000h
sub_1_370E:
        db  0A1h, 0CCh, 0FCh, 000h, 000h
        mov    cr3, eax
        ret
sub_1_3717:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 015h, 098h, 0F6h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_3734
        or     dword ptr [edx], 1000000h
        cli
        call   sub_1_8ACC
        sti
        jmp    SHORT loc_1_374F
loc_1_3734:
        push   esi
        db  0BEh, 052h, 037h, 000h, 000h
        add    edx, 3
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        pop    esi
        push   0
        push   edx
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_1_374F:
        ret    4
loc_1_3752:
        push   edx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
        ret
sub_1_375B:
        push   edi
        db  08Bh, 03Dh, 004h, 0FCh, 000h, 000h
        db  08Bh, 00Dh, 028h, 0EFh, 000h, 000h
        shr    ecx, 3
        xor    eax, eax
        repne scasb
        jne    SHORT loc_1_3774
        mov    eax, edi
        dec    eax
loc_1_3774:
        pop    edi
        ret
sub_1_3776:
        push   esi
        push   edi
        xor    eax, eax
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 002h
        je     SHORT loc_1_379A
        mov    ch, byte ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 10h]
        dec    edi
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 18h]
        db  0CDh, 020h, 007h, 000h, 007h, 000h
loc_1_379A:
        pop    edi
        pop    esi
        ret    10h
sub_1_379F:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, 1fh
        shr    ecx, 5
        mov    eax, 0ffffffffh
        repe scasd
        je     SHORT loc_1_37E4
        sub    edi, 4
        mov    edx, dword ptr [edi]
        mov    eax, 1
        mov    esi, eax
loc_1_37C4:
        test   esi, edx
        je     SHORT loc_1_37CD
        shl    esi, 1
        inc    eax
        jmp    SHORT loc_1_37C4
loc_1_37CD:
        or     edx, esi
        mov    ecx, edi
        sub    ecx, dword ptr [esp + 0ch]
        shl    ecx, 3
        add    eax, ecx
        cmp    eax, dword ptr [esp + 10h]
        ja     SHORT loc_1_37E4
        mov    dword ptr [edi], edx
        jmp    SHORT loc_1_37E6
loc_1_37E4:
        xor    eax, eax
loc_1_37E6:
        pop    edi
        pop    esi
        ret    8
sub_1_37EB:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        dec    ecx
        btr    dword ptr [eax], ecx
        ret    8
sub_1_37FA:
        push   edi
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        jecxz  loc_1_384B
        dec    ecx
        mov    edi, ecx
        shr    edi, 3
        and    edi, 0fffffffch
        mov    edx, dword ptr [edi + edx]
        mov    esi, 0ffffffffh
        shl    esi, cl
        shl    esi, 1
        not    esi
        and    edx, esi
        bsr    esi, edx
        je     SHORT loc_1_382D
        mov    eax, ecx
        and    eax, 0ffffffe0h
        add    eax, esi
        jmp    SHORT loc_1_3850
loc_1_382D:
        std
        add    edi, dword ptr [esp + 0ch]
        sub    edi, 4
        shr    ecx, 5
        jecxz  loc_1_384B
        xor    eax, eax
        repe scasd
        je     SHORT loc_1_384B
        bsr    eax, dword ptr [edi + 4]
        shl    ecx, 5
        add    eax, ecx
        jmp    SHORT loc_1_3850
loc_1_384B:
        mov    eax, 0ffffffffh
loc_1_3850:
        cld
        pop    esi
        pop    edi
        ret    8
loc_1_3856:
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        mov    edx, dword ptr [esp + 4]
        cmp    eax, edx
        je     SHORT loc_1_3893
        push   esi
        push   edi
        mov    edi, 0ffbfe000h
        mov    ecx, dword ptr [edx + 4]
        mov    esi, dword ptr [edx]
        rep movsd
        mov    ecx, dword ptr [eax + 4]
        sub    ecx, dword ptr [edx + 4]
        jbe    SHORT loc_1_3880
        db  0A1h, 0D0h, 0F7h, 000h, 000h
        rep stosd
loc_1_3880:
        db  087h, 015h, 03Ch, 0F8h, 000h, 000h
        mov    eax, edx
        db  08Bh, 00Dh, 0CCh, 0FCh, 000h, 000h
        mov    cr3, ecx
        pop    edi
        pop    esi
loc_1_3893:
        ret
loc_1_3894:
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        ret
sub_1_389A:
        db  0CDh, 020h, 007h, 000h, 001h, 000h
        mov    eax, ecx
        ret
sub_1_38A3:
        mov    ecx, dword ptr [esp + 4]
        db  0CDh, 020h, 008h, 000h, 001h, 000h
        ret    4
sub_1_38B0:
        push   esi
        db  0BEh, 0F8h, 0FCh, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 0c0000h
        jae    SHORT loc_1_38DC
        cmp    eax, 400h
        jb     SHORT loc_1_38DC
        cmp    eax, 7ffffh
        db  0BEh, 08Ch, 0F8h, 000h, 000h
        ja     SHORT loc_1_38DC
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        mov    esi, dword ptr [eax + 8]
loc_1_38DC:
        push   5
        push   dword ptr [esi + 10h]
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        add    esp, 8
        mov    eax, esi
        pop    esi
        ret    4
sub_1_38F0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_1_39E1
        cmp    dword ptr [esp + 18h], 7fc00h
        ja     NEAR PTR loc_1_39E1
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, 0ffbffh
        ja     NEAR PTR loc_1_39E1
        cmp    ecx, 400h
        jb     NEAR PTR loc_1_39E1
        mov    eax, ecx
        and    eax, 0fffffcffh
        shr    eax, 8
        test   byte ptr [eax - 401fffh], 2
        je     NEAR PTR loc_1_39E1
        lea    edx, [ecx*4 - 800000h]
        mov    esi, dword ptr [edx]
        or     esi, esi
        je     NEAR PTR loc_1_39E1
        test   esi, 200h
        je     NEAR PTR loc_1_39EA
        test   esi, 800h
        jne    SHORT loc_1_3995
        test   esi, 1
        je     SHORT loc_1_3981
        shr    esi, 0ch
        imul   esi, esi, 0dh
        db  0A1h, 0E4h, 0F7h, 000h, 000h
        mov    eax, dword ptr [esi + eax]
        movzx  esi, word ptr [eax + 4]
        jmp    SHORT loc_1_3991
loc_1_3981:
        shr    esi, 0ch
        imul   esi, esi, 0ah
        db  0A1h, 0ACh, 0F8h, 000h, 000h
        movzx  esi, word ptr [esi + eax + 4]
loc_1_3991:
        or     esi, esi
        jne    SHORT loc_1_39F5
loc_1_3995:
        db  0B8h, 0F8h, 0FCh, 000h, 000h
        cmp    ecx, 0c0000h
        jae    SHORT loc_1_39B7
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        cmp    ecx, 7ffffh
        mov    eax, dword ptr [eax + 8]
        jbe    SHORT loc_1_39B7
        db  0B8h, 08Ch, 0F8h, 000h, 000h
loc_1_39B7:
        mov    ebx, dword ptr [eax + 0ch]
        mov    eax, ebx
        db  08Bh, 03Dh, 0ECh, 0FAh, 000h, 000h
loc_1_39C2:
        movzx  eax, word ptr [eax + 8]
        imul   eax, eax, 0ch
        add    eax, edi
        mov    esi, dword ptr [eax + 4]
        shr    esi, 0ch
        cmp    ecx, esi
        jb     SHORT loc_1_39DD
        mov    ebp, dword ptr [eax]
        add    ebp, esi
        cmp    ebp, ecx
        ja     SHORT loc_1_39FF
loc_1_39DD:
        cmp    eax, ebx
        jne    SHORT loc_1_39C2
loc_1_39E1:
        sub    eax, eax
loc_1_39E3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_1_39EA:
        shr    esi, 0ch
        cmp    esi, 0fffffh
        je     SHORT loc_1_3995
loc_1_39F5:
        imul   esi, esi, 0ch
        db  0A1h, 0ECh, 0FAh, 000h, 000h
        add    eax, esi
loc_1_39FF:
        mov    esi, dword ptr [eax + 4]
        shr    esi, 0ch
        add    esi, dword ptr [eax]
        mov    edi, dword ptr [esp + 18h]
        add    edi, ecx
        cmp    esi, edi
        jb     SHORT loc_1_39E1
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], edx
        jmp    SHORT loc_1_39E3
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3A20:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
loc_1_3A27:
        mov    edi, dword ptr [esi - 4]
        or     edi, edi
        jne    SHORT loc_1_3A64
        push   5
        db  068h, 008h, 0F9h, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        add    esp, 8
        mov    eax, dword ptr [esi - 18h]
        sub    edx, edx
        div    dword ptr [esi - 1ch]
        inc    eax
        push   eax
        push   esi
        call   sub_1_3A71
        mov    ebx, eax
        db  068h, 008h, 0F9h, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        or     ebx, ebx
        jne    SHORT loc_1_3A27
        jmp    SHORT loc_1_3A69
loc_1_3A64:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi - 4], eax
loc_1_3A69:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_1_3A71:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, 1
        mov    esi, dword ptr [esp + 1ch]
        sub    esi, 1ch
        mov    edi, dword ptr [esp + 20h]
        imul   edi, dword ptr [esi]
        add    edi, 101bh
        and    edi, 0fffff000h
        mov    ebp, dword ptr [esi + 4]
        cmp    edi, ebp
        jbe    SHORT loc_1_3B1B
        cmp    dword ptr [esi + 8], edi
        jbe    SHORT loc_1_3B19
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        push   dword ptr [esi + 0ch]
        mov    eax, edi
        sub    eax, ebp
        shr    eax, 0ch
        push   eax
        lea    eax, [esi + ebp]
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0A0h, 0D7h, 000h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_3B19
        lea    eax, [ebp - 1ch]
        sub    edx, edx
        div    dword ptr [esi]
        imul   eax, dword ptr [esi]
        add    eax, esi
        add    eax, 1ch
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 14h], eax
        mov    eax, edi
        sub    eax, dword ptr [esp + 10h]
        add    eax, esi
        sub    edx, edx
        div    dword ptr [esi]
        dec    eax
        je     SHORT loc_1_3B04
loc_1_3AEE:
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, dword ptr [esp + 10h]
        add    edx, ecx
        mov    dword ptr [ebp], edx
        add    dword ptr [esp + 10h], ecx
        dec    eax
        jne    SHORT loc_1_3AEE
loc_1_3B04:
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [esi + 4], edi
        jmp    SHORT loc_1_3B1B
loc_1_3B19:
        sub    ebx, ebx
loc_1_3B1B:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3B30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi]
        and    ah, 0ah
        cmp    ah, 2
        je     SHORT loc_1_3B4A
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esi], ecx
        jmp    SHORT loc_1_3BC8
loc_1_3B4A:
        push   esi
        call   sub_1_3CC3
        mov    edi, eax
        mov    ax, word ptr [edi]
        dec    ax
        mov    word ptr [edi], ax
        mov    ecx, dword ptr [esi]
        test   cl, 1
        je     NEAR PTR loc_1_3BF7
        mov    eax, ecx
        shr    eax, 0ch
        imul   eax, eax, 0dh
        db  08Bh, 02Dh, 0E4h, 0F7h, 000h, 000h
        lea    ebx, [eax + ebp]
        test   cl, 40h
        je     SHORT loc_1_3B81
        push   edi
        call   sub_1_5B69
loc_1_3B81:
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esi], ecx
        mov    ax, word ptr [ebx + 0ah]
        dec    ax
        mov    word ptr [ebx + 0ah], ax
        jne    SHORT loc_1_3BC2
        cmp    word ptr [edi], 0
        jne    SHORT loc_1_3BAC
        cmp    word ptr [ebx + 8], 0
        je     NEAR PTR loc_1_3C29
        db  0FFh, 00Dh, 01Ch, 0EFh, 000h, 000h
        jmp    SHORT loc_1_3C29
loc_1_3BAC:
        or     byte ptr [edi + 3], 8
        db  0FFh, 035h, 0DCh, 0F7h, 000h, 000h
        db  0FFh, 035h, 068h, 0FAh, 000h, 000h
        push   ebx
        call   sub_1_4D90
loc_1_3BC2:
        push   edi
        call   sub_1_36BD
loc_1_3BC8:
        sub    eax, eax
loc_1_3BCA:
        cmp    esi, 0ffbfe000h
        jb     SHORT loc_1_3BF0
        cmp    esi, 0ffbfe7fch
        ja     SHORT loc_1_3BF0
        mov    edx, dword ptr [esi]
        add    esi, 402000h
        db  08Bh, 00Dh, 03Ch, 0F8h, 000h, 000h
        and    esi, 0fffffffch
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [esi + ecx], edx
loc_1_3BF0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_1_3BF7:
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esi], ecx
        or     ax, ax
        jne    SHORT loc_1_3BC2
        test   byte ptr [edi + 3], 10h
        je     SHORT loc_1_3C79
        mov    ebx, dword ptr [edi + 6]
        db  039h, 01Dh, 0B0h, 0F8h, 000h, 000h
        jne    SHORT loc_1_3C1D
        db  0A1h, 068h, 0FAh, 000h, 000h
        db  0A3h, 0B0h, 0F8h, 000h, 000h
loc_1_3C1D:
        push   ebx
        call   sub_1_4E1F
        db  08Bh, 02Dh, 0E4h, 0F7h, 000h, 000h
loc_1_3C29:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 6], eax
        db  039h, 01Dh, 0A4h, 0F8h, 000h, 000h
        mov    dword ptr [ebx], 0
        jbe    SHORT loc_1_3C5F
        db  0A1h, 060h, 0FAh, 000h, 000h
        db  0FFh, 005h, 040h, 0F9h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        and    eax, 0fffffh
        push   eax
        imul   eax, eax, 0dh
        add    eax, ebp
        push   eax
        push   ebx
        call   sub_1_4D90
        jmp    SHORT loc_1_3C7B
loc_1_3C5F:
        db  0FFh, 005h, 040h, 0F9h, 000h, 000h
        db  0FFh, 035h, 0A0h, 0FDh, 000h, 000h
        db  0FFh, 035h, 060h, 0FAh, 000h, 000h
        push   ebx
        call   sub_1_4D90
        jmp    SHORT loc_1_3C7B
loc_1_3C79:
        sub    ebx, ebx
loc_1_3C7B:
        push   ebx
        push   edi
        call   sub_1_5B9C
        mov    ecx, dword ptr [eax + 1ch]
        and    ecx, 1
        db  0FFh, 00Ch, 08Dh, 084h, 0EEh, 000h, 000h
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 004h
        je     SHORT loc_1_3CA6
        or     ecx, ecx
        jne    SHORT loc_1_3CA6
        or     ebx, ebx
        jne    SHORT loc_1_3CA6
        db  0FFh, 00Dh, 01Ch, 0EFh, 000h, 000h
loc_1_3CA6:
        push   edi
        call   sub_1_36BD
        db  0A1h, 0ACh, 0F8h, 000h, 000h
        mov    ecx, dword ptr [eax - 4]
        mov    dword ptr [edi], ecx
        mov    dword ptr [eax - 4], edi
        mov    eax, 1
        jmp    NEAR PTR loc_1_3BCA
sub_1_3CC3:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 4], eax
        test   al, 1
        je     SHORT loc_1_3CE7
        mov    ecx, eax
        shr    ecx, 0ch
        imul   ecx, ecx, 0dh
        db  0A1h, 0E4h, 0F7h, 000h, 000h
        mov    esi, dword ptr [ecx + eax]
        jmp    SHORT loc_1_3CF9
loc_1_3CE7:
        mov    ecx, dword ptr [esp + 4]
        shr    ecx, 0ch
        imul   ecx, ecx, 0ah
        db  08Bh, 035h, 0ACh, 0F8h, 000h, 000h
        add    esi, ecx
loc_1_3CF9:
        push   esi
        call   sub_1_369C
        or     eax, eax
        je     SHORT loc_1_3CF9
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret    4
_ModifyPageBits:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        test   dword ptr [esp + 24h], 0fffffffch
        jne    NEAR PTR loc_1_3E01
        mov    esi, dword ptr [esp + 20h]
        mov    eax, esi
        or     eax, 7
        cmp    eax, -1
        jne    NEAR PTR loc_1_3E01
        mov    edi, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 18h]
        lea    eax, [edi + ebp]
        cmp    eax, 110h
        ja     NEAR PTR loc_1_3E01
        cmp    ebp, 110h
        ja     NEAR PTR loc_1_3E01
        cmp    dword ptr [esp + 2ch], ebx
        jne    NEAR PTR loc_1_3E01
        cmp    edi, ebx
        je     NEAR PTR loc_1_3DFC
        push   dword ptr [esp + 14h]
        call   sub_1_5630
        cmp    eax, ebx
        je     NEAR PTR loc_1_3E01
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 4]
        shr    eax, 0ch
        lea    ecx, [ebp + eax]
        test   esi, 1
        jne    SHORT loc_1_3DA9
        cmp    dword ptr [esp + 28h], 7
        jne    SHORT loc_1_3E01
        push   ebx
        db  0FFh, 035h, 0D8h, 0F7h, 000h, 000h
        push   edi
        push   ecx
        call   sub_1_5F14
        add    esp, 10h
        mov    ebx, eax
        jmp    SHORT loc_1_3E01
loc_1_3DA9:
        lea    ebx, [ecx*4 - 800000h]
        and    esi, 0ffffff9fh
        lea    edi, [ebx + edi*4]
        db  0FFh, 015h, 028h, 0FDh, 000h, 000h
        cmp    edi, ebx
        jbe    SHORT loc_1_3DF1
loc_1_3DC0:
        mov    eax, dword ptr [ebx]
        and    eax, 840h
        cmp    eax, 40h
        jne    SHORT loc_1_3DE0
        push   ebx
        call   sub_1_3CC3
        mov    ebp, eax
        push   ebp
        call   sub_1_5B69
        push   ebp
        call   sub_1_36BD
loc_1_3DE0:
        mov    eax, dword ptr [ebx]
        and    eax, esi
        or     eax, dword ptr [esp + 24h]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        cmp    edi, ebx
        ja     SHORT loc_1_3DC0
loc_1_3DF1:
        db  0FFh, 015h, 070h, 0F8h, 000h, 000h
        call   sub_1_370E
loc_1_3DFC:
        mov    ebx, 0ffffffffh
loc_1_3E01:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_3E08:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    ebx, dword ptr [esp + 34h]
        test   ebx, 0bff9ff7fh
        jne    NEAR PTR loc_1_3F24
        mov    esi, dword ptr [esp + 30h]
        cmp    esi, -1
        jne    SHORT loc_1_3E38
        db  08Bh, 035h, 0D8h, 0F7h, 000h, 000h
        and    ebx, 0bfffffffh
        jmp    SHORT loc_1_3E44
loc_1_3E38:
        cmp    esi, 0fffffh
        ja     NEAR PTR loc_1_3F24
loc_1_3E44:
        mov    ebp, ebx
        and    ebp, 80h
        jne    SHORT loc_1_3E5B
        push   dword ptr [esp + 28h]
        call   sub_1_38B0
        mov    dword ptr [esp + 18h], eax
loc_1_3E5B:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        call   sub_1_38F0
        or     eax, eax
        je     NEAR PTR loc_1_3F10
        or     ebp, ebp
        jne    SHORT loc_1_3E8F
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        db  0E8h, 063h, 0E3h, 0FEh, 0FFh
        mov    dword ptr [esp + 1ch], eax
loc_1_3E8F:
        mov    edi, ebx
        and    edi, 60000h
        or     edi, 0a010000h
        shr    edi, 10h
        shl    esi, 0ch
        or     edi, esi
        and    ebx, 40000000h
        cmp    ebx, 1
        cmc
        sbb    eax, eax
        and    eax, 1000h
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 2ch]
        lea    esi, [eax*4]
        add    esi, dword ptr [esp + 10h]
        sub    ebx, ebx
        cmp    dword ptr [esp + 10h], esi
        jae    SHORT loc_1_3EEC
loc_1_3ED1:
        push   edi
        push   dword ptr [esp + 14h]
        call   sub_1_3B30
        add    ebx, eax
        add    dword ptr [esp + 10h], 4
        add    edi, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 10h], esi
        jb     SHORT loc_1_3ED1
loc_1_3EEC:
        or     ebx, ebx
        je     SHORT loc_1_3EF5
        db  0E8h, 0DDh, 0E0h, 0FEh, 0FFh
loc_1_3EF5:
        call   sub_1_370E
        or     ebp, ebp
        jne    SHORT loc_1_3F0B
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        db  0E8h, 051h, 0E3h, 0FEh, 0FFh
loc_1_3F0B:
        mov    edi, 1
loc_1_3F10:
        or     ebp, ebp
        jne    SHORT loc_1_3F24
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
loc_1_3F24:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  000h, 000h
_LinPageLock:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 30h], 0ffff10cbh
        test   byte ptr [esp + 51h], 1
        je     SHORT loc_1_3F53
        db  0F6h, 005h, 0F9h, 0F8h, 000h, 000h, 001h
        je     NEAR PTR loc_1_42CE
loc_1_3F53:
        cmp    dword ptr [esp + 4ch], 0
        je     NEAR PTR loc_1_42CE
        mov    eax, dword ptr [esp + 50h]
        and    eax, 0c1ffffffh
        or     eax, 40000000h
        shr    eax, 19h
        mov    dword ptr [esp + 20h], eax
        push   dword ptr [esp + 48h]
        call   sub_1_38B0
        mov    dword ptr [esp + 38h], eax
        cmp    dword ptr [esp + 48h], 0ffbffh
        ja     NEAR PTR loc_1_42B6
        cmp    dword ptr [esp + 4ch], 7fc00h
        ja     NEAR PTR loc_1_42B6
        mov    ecx, dword ptr [esp + 4ch]
        add    ecx, dword ptr [esp + 48h]
        cmp    dword ptr [eax + 4], ecx
        jb     NEAR PTR loc_1_42B6
        mov    eax, dword ptr [esp + 48h]
        lea    esi, [eax*4 - 800000h]
        mov    edi, esi
        mov    eax, dword ptr [esp + 4ch]
        lea    eax, [esi + eax*4]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, esi
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        sub    eax, 800000h
        mov    dword ptr [esp + 24h], eax
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [esp + 3ch], eax
        test   byte ptr [esp + 53h], 40h
        je     NEAR PTR loc_1_410F
        cmp    eax, 80000h
        jb     SHORT loc_1_400A
        and    dword ptr [esp + 50h], 0bfffffffh
        shl    eax, 0ch
        mov    dword ptr [esp + 30h], eax
        jmp    NEAR PTR loc_1_410F
loc_1_400A:
        db  08Bh, 01Dh, 0BCh, 0F8h, 000h, 000h
        db  0C7h, 044h, 024h, 02Ch, 0BCh, 0F8h, 000h, 000h
loc_1_4018:
        or     ebx, ebx
        je     SHORT loc_1_4031
        mov    eax, dword ptr [esp + 4ch]
        cmp    dword ptr [ebx + 0ch], eax
        jae    NEAR PTR loc_1_40D6
        mov    dword ptr [esp + 2ch], ebx
        mov    ebx, dword ptr [ebx]
        jmp    SHORT loc_1_4018
loc_1_4031:
        push   0
        push   10h
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 0A0h, 0D4h, 0FEh, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_42B6
        mov    ebp, dword ptr [esp + 4ch]
        add    ebp, 0fh
        and    ebp, 0fffffff0h
        push   0
        push   ebp
        push   80080000h
        db  0FFh, 015h, 09Ch, 0D7h, 000h, 000h
        mov    dword ptr [esp + 40h], eax
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_40B6
        mov    edi, eax
        and    edi, 0fffff3ffh
        shr    edi, 0ah
        sub    edi, 800000h
        mov    eax, edi
        mov    ecx, eax
        and    ecx, 0fffh
        push   0
        lea    ecx, [ecx + ebp*4]
        add    ecx, 0fffh
        shr    ecx, 0ch
        push   ecx
        shr    eax, 0ch
        push   eax
        call   _LinPageLock
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_40C7
        push   0
        push   dword ptr [esp + 38h]
        db  0E8h, 07Dh, 0C0h, 0FEh, 0FFh
        add    esp, 8
loc_1_40B6:
        push   ebx
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 08Eh, 0D3h, 0FEh, 0FFh
        jmp    NEAR PTR loc_1_42B6
loc_1_40C7:
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 8], edi
        mov    dword ptr [ebx + 0ch], ebp
        jmp    SHORT loc_1_40E1
loc_1_40D6:
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    edi, dword ptr [ebx + 8]
loc_1_40E1:
        mov    eax, edi
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        sub    eax, 800000h
        mov    dword ptr [esp + 24h], eax
        db  0A1h, 024h, 0FDh, 000h, 000h
        mov    dword ptr [ebx], eax
        db  089h, 01Dh, 024h, 0FDh, 000h, 000h
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esp + 30h], eax
        shr    eax, 0ch
        mov    dword ptr [esp + 3ch], eax
loc_1_410F:
        mov    eax, dword ptr [esp + 28h]
        test   byte ptr [eax + 1], 2
        je     NEAR PTR loc_1_4296
loc_1_411D:
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 40h], eax
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [eax]
        shr    ecx, 0ch
        imul   ecx, ecx, 0dh
        db  0A1h, 0E4h, 0F7h, 000h, 000h
        add    eax, ecx
        mov    dword ptr [esp + 18h], eax
loc_1_413A:
        mov    eax, dword ptr [esi]
        test   ah, 2
        je     NEAR PTR loc_1_4296
        test   ah, 8
        push   esi
        je     SHORT loc_1_415D
        call   sub_1_45C7
        or     eax, eax
        jne    NEAR PTR loc_1_424C
        jmp    NEAR PTR loc_1_4296
loc_1_415D:
        call   sub_1_3CC3
        mov    ebx, eax
        test   byte ptr [esi], 1
        db  08Bh, 00Dh, 098h, 0F6h, 000h, 000h
        jne    SHORT loc_1_41AE
        movzx  eax, byte ptr [ebx + 2]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        mov    ebp, 0
        test   byte ptr [eax + 1ch], 2
        jne    SHORT loc_1_418A
        mov    ebp, 1
loc_1_418A:
        push   ebx
        push   esi
        add    dword ptr [ecx - 28h], ebp
        push   dword ptr [esp + 50h]
        call   sub_1_4802
        db  08Bh, 00Dh, 098h, 0F6h, 000h, 000h
        sub    dword ptr [ecx - 28h], ebp
        or     eax, eax
        je     NEAR PTR loc_1_4290
        call   sub_1_5421
loc_1_41AE:
        mov    eax, dword ptr [esp + 20h]
        or     dword ptr [esi], eax
        mov    ebp, dword ptr [ebx + 6]
        mov    ax, word ptr [ebp + 8]
        cmp    ax, 0f188h
        je     SHORT loc_1_4238
        or     ax, ax
        jne    SHORT loc_1_4226
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esp + 10h], 1
        mov    ax, word ptr [eax + 8]
        or     ax, ax
        mov    word ptr [esp + 14h], ax
        jne    SHORT loc_1_41E8
        mov    dword ptr [esp + 10h], 2
loc_1_41E8:
        mov    eax, dword ptr [esp + 10h]
        db  003h, 005h, 01Ch, 0EFh, 000h, 000h
        db  0A3h, 01Ch, 0EFh, 000h, 000h
        db  03Bh, 005h, 024h, 0EFh, 000h, 000h
        jle    SHORT loc_1_420C
        push   dword ptr [esp + 10h]
        call   sub_1_54C6
        or     eax, eax
        je     SHORT loc_1_4286
loc_1_420C:
        cmp    word ptr [esp + 14h], 0f000h
        jae    SHORT loc_1_4234
        mov    cx, word ptr [esp + 14h]
        mov    eax, dword ptr [esp + 18h]
        inc    cx
        mov    word ptr [eax + 8], cx
        jmp    SHORT loc_1_4234
loc_1_4226:
        cmp    ax, 0efffh
        jb     SHORT loc_1_4234
        je     SHORT loc_1_4234
        dec    ax
        mov    word ptr [ebp + 8], ax
loc_1_4234:
        inc    word ptr [ebp + 8]
loc_1_4238:
        test   byte ptr [esp + 53h], 40h
        je     SHORT loc_1_4246
        inc    word ptr [ebp + 0ah]
        inc    word ptr [ebx]
loc_1_4246:
        push   ebx
        call   sub_1_36BD
loc_1_424C:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        add    esi, 4
        cmp    dword ptr [esp + 1ch], esi
        jbe    SHORT loc_1_42BE
        inc    dword ptr [esp + 48h]
        add    edi, 4
        test   edi, 0fffh
        jne    NEAR PTR loc_1_413A
        add    dword ptr [esp + 28h], 4
        mov    eax, dword ptr [esp + 28h]
        add    dword ptr [esp + 24h], 4
        test   byte ptr [eax + 1], 2
        jne    NEAR PTR loc_1_411D
        jmp    SHORT loc_1_4296
loc_1_4286:
        mov    eax, dword ptr [esp + 10h]
        db  029h, 005h, 01Ch, 0EFh, 000h, 000h
loc_1_4290:
        push   ebx
        call   sub_1_36BD
loc_1_4296:
        mov    eax, dword ptr [esp + 4ch]
        mov    ecx, dword ptr [esp + 1ch]
        push   dword ptr [esp + 50h]
        sub    ecx, esi
        sar    ecx, 2
        sub    eax, ecx
        push   eax
        push   dword ptr [esp + 44h]
        call   _LinPageUnLock
        add    esp, 0ch
loc_1_42B6:
        mov    dword ptr [esp + 30h], 0
loc_1_42BE:
        mov    eax, dword ptr [esp + 38h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
loc_1_42CE:
        mov    eax, dword ptr [esp + 30h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
_LinPageUnLock:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, 1
        mov    esi, dword ptr [esp + 20h]
        test   esi, 100h
        je     SHORT loc_1_42FE
        db  0F6h, 005h, 0F9h, 0F8h, 000h, 000h, 001h
        je     NEAR PTR loc_1_43B6
loc_1_42FE:
        mov    ecx, dword ptr [esp + 1ch]
        or     ecx, ecx
        je     NEAR PTR loc_1_43B6
        mov    edx, dword ptr [esp + 18h]
        cmp    edx, 0ffbffh
        ja     NEAR PTR loc_1_43B4
        cmp    ecx, 7fc00h
        ja     NEAR PTR loc_1_43B4
        lea    eax, [edx + ecx]
        cmp    eax, 0ffbffh
        ja     NEAR PTR loc_1_43B4
        test   esi, 40000000h
        je     SHORT loc_1_4380
        db  08Bh, 03Dh, 024h, 0FDh, 000h, 000h
        db  0C7h, 044h, 024h, 00Ch, 024h, 0FDh, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_4370
        mov    eax, edx
        shl    eax, 0ch
loc_1_4353:
        cmp    dword ptr [edi + 4], eax
        je     SHORT loc_1_4362
        mov    dword ptr [esp + 0ch], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_4353
loc_1_4362:
        or     edi, edi
        je     SHORT loc_1_4370
        mov    ebx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx], eax
        jmp    SHORT loc_1_4384
loc_1_4370:
        cmp    edx, 80000h
        jb     SHORT loc_1_43B4
        and    esi, 0bfffffffh
        jmp    SHORT loc_1_4384
loc_1_4380:
        mov    edi, dword ptr [esp + 10h]
loc_1_4384:
        lea    edx, [edx*4 - 800000h]
        push   esi
        lea    eax, [edx + ecx*4]
        push   eax
        push   edx
        call   sub_1_440F
        mov    ebx, eax
        test   esi, 40000000h
        je     SHORT loc_1_43B6
        call   sub_1_370E
        db  0A1h, 0BCh, 0F8h, 000h, 000h
        mov    dword ptr [edi], eax
        db  089h, 03Dh, 0BCh, 0F8h, 000h, 000h
        jmp    SHORT loc_1_43B6
loc_1_43B4:
        sub    ebx, ebx
loc_1_43B6:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
_PageUnLock:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        shr    ecx, 0ch
        mov    ebx, dword ptr [ebp + 10h]
        add    ebx, ecx
        lea    eax, [ebp - 4]
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   ebx
        call   sub_1_38F0
        cmp    eax, edi
        je     SHORT loc_1_4405
        mov    eax, dword ptr [eax + 4]
        and    eax, 0fffff000h
        cmp    eax, dword ptr [ebp + 8]
        jne    SHORT loc_1_4405
        push   dword ptr [ebp + 14h]
        push   esi
        push   ebx
        db  0FFh, 015h, 0B8h, 0D4h, 000h, 000h
        add    esp, 0ch
        mov    edi, eax
loc_1_4405:
        pop    ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_440F:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 38h], 1
        mov    esi, 40000000h
        mov    eax, dword ptr [esp + 40h]
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        sub    eax, 800000h
        mov    dword ptr [esp + 34h], eax
        mov    edi, 1
        mov    ebx, 8000000h
        sub    ebp, ebp
loc_1_4444:
        mov    eax, dword ptr [esp + 34h]
        mov    eax, dword ptr [eax]
        test   edi, eax
        jne    SHORT loc_1_4465
        mov    eax, dword ptr [esp + 40h]
        and    eax, 0fffff000h
        add    eax, 1000h
        mov    dword ptr [esp + 40h], eax
        jmp    NEAR PTR loc_1_459A
loc_1_4465:
        shr    eax, 0ch
        imul   eax, eax, 0dh
        db  003h, 005h, 0E4h, 0F7h, 000h, 000h
        mov    dword ptr [esp + 30h], eax
loc_1_4475:
        mov    eax, dword ptr [esp + 40h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 1ch], eax
        test   ah, 8
        je     SHORT loc_1_44A2
        push   dword ptr [esp + 40h]
        call   sub_1_462B
        test   dword ptr [esp + 48h], esi
        je     NEAR PTR loc_1_4595
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [eax], ebp
        jmp    NEAR PTR loc_1_4595
loc_1_44A2:
        test   dword ptr [esp + 1ch], edi
        je     NEAR PTR loc_1_4540
        mov    eax, dword ptr [esp + 1ch]
        shr    eax, 0ch
        imul   eax, eax, 0dh
        db  003h, 005h, 0E4h, 0F7h, 000h, 000h
        mov    dword ptr [esp + 14h], eax
        add    eax, 8
        mov    dword ptr [esp + 18h], eax
        mov    ax, word ptr [eax]
        mov    word ptr [esp + 10h], ax
        cmp    ax, 0f188h
        je     SHORT loc_1_4546
        test   dword ptr [esp + 48h], ebx
        jne    SHORT loc_1_452A
        cmp    ax, bp
        je     SHORT loc_1_4546
        cmp    ax, 0f000h
        je     SHORT loc_1_4546
        mov    ecx, dword ptr [esp + 18h]
        dec    ax
        mov    word ptr [ecx], ax
        cmp    ax, bp
        jne    SHORT loc_1_4546
        db  0FFh, 00Dh, 01Ch, 0EFh, 000h, 000h
        test   dword ptr [esp + 48h], esi
        jne    SHORT loc_1_454C
        mov    eax, dword ptr [esp + 30h]
        add    eax, 8
        mov    dword ptr [esp + 28h], eax
        mov    ax, word ptr [eax]
        cmp    ax, 0f000h
        jae    SHORT loc_1_4546
        mov    ecx, dword ptr [esp + 28h]
        dec    ax
        mov    word ptr [ecx], ax
        cmp    ax, bp
        jne    SHORT loc_1_4546
        db  0FFh, 00Dh, 01Ch, 0EFh, 000h, 000h
        jmp    SHORT loc_1_4546
loc_1_452A:
        cmp    word ptr [esp + 10h], bp
        je     SHORT loc_1_4546
        mov    eax, dword ptr [esp + 18h]
        db  0FFh, 00Dh, 01Ch, 0EFh, 000h, 000h
        mov    word ptr [eax], bp
        jmp    SHORT loc_1_4546
loc_1_4540:
        test   dword ptr [esp + 48h], ebx
        jne    SHORT loc_1_4595
loc_1_4546:
        test   dword ptr [esp + 48h], esi
        je     SHORT loc_1_4595
loc_1_454C:
        mov    eax, dword ptr [esp + 14h]
        add    eax, 0ah
        mov    dword ptr [esp + 20h], eax
        mov    ax, word ptr [eax]
        mov    word ptr [esp + 24h], ax
        cmp    ax, di
        jne    SHORT loc_1_4574
        push   0fffff000h
        push   dword ptr [esp + 44h]
        call   sub_1_3B30
        jmp    SHORT loc_1_4595
loc_1_4574:
        mov    ax, word ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 20h]
        dec    ax
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        dec    word ptr [eax]
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [eax], 0fffff000h
loc_1_4595:
        add    dword ptr [esp + 40h], 4
loc_1_459A:
        mov    eax, dword ptr [esp + 40h]
        cmp    dword ptr [esp + 44h], eax
        jbe    SHORT loc_1_45B9
        test   eax, 0fffh
        jne    NEAR PTR loc_1_4475
        add    dword ptr [esp + 34h], 4
        jmp    NEAR PTR loc_1_4444
loc_1_45B9:
        mov    eax, dword ptr [esp + 38h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret    0ch
sub_1_45C7:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        db  08Bh, 035h, 0E4h, 0F7h, 000h, 000h
        mov    ecx, dword ptr [eax - 800000h]
        shr    ecx, 0ch
        imul   ecx, ecx, 0dh
        add    esi, ecx
        mov    di, word ptr [esi + 8]
        or     di, di
        jne    SHORT loc_1_4614
        db  0A1h, 024h, 0EFh, 000h, 000h
        db  039h, 005h, 01Ch, 0EFh, 000h, 000h
        jl     SHORT loc_1_460E
        push   1
        call   sub_1_54C6
        or     eax, eax
        jne    SHORT loc_1_460E
        sub    eax, eax
        jmp    SHORT loc_1_4626
loc_1_460E:
        db  0FFh, 005h, 01Ch, 0EFh, 000h, 000h
loc_1_4614:
        cmp    di, 0f000h
        jae    SHORT loc_1_4621
        inc    di
        mov    word ptr [esi + 8], di
loc_1_4621:
        mov    eax, 1
loc_1_4626:
        pop    edi
        pop    esi
        ret    4
sub_1_462B:
        mov    eax, dword ptr [esp + 4]
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        mov    ecx, dword ptr [eax - 800000h]
        shr    ecx, 0ch
        imul   ecx, ecx, 0dh
        db  0A1h, 0E4h, 0F7h, 000h, 000h
        add    ecx, eax
        mov    ax, word ptr [ecx + 8]
        cmp    ax, 0f000h
        jae    SHORT loc_1_4662
        dec    ax
        mov    word ptr [ecx + 8], ax
        jne    SHORT loc_1_4662
        db  0FFh, 00Dh, 01Ch, 0EFh, 000h, 000h
loc_1_4662:
        ret    4
sub_1_4665:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 034h, 0EFh, 000h, 000h
        inc    eax
        db  0A3h, 034h, 0EFh, 000h, 000h
        mov    ebp, dword ptr [esp + 18h]
        shr    ebp, 0ch
        lea    ebx, [ebp*4 - 800000h]
        mov    esi, ebx
        and    esi, 0fffff3ffh
        shr    esi, 0ah
        sub    esi, 800000h
        test   byte ptr [esi + 1], 2
        je     NEAR PTR loc_1_4781
        mov    esi, ebx
        mov    edi, dword ptr [ebx]
        mov    ecx, edi
        and    ecx, 0a01h
        cmp    ecx, 200h
        jne    NEAR PTR loc_1_4777
        sti
        call   sub_1_389A
        mov    dword ptr [esp + 10h], eax
        db  0A1h, 098h, 0F6h, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        add    dword ptr [eax - 28h], ecx
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 008h
        je     SHORT loc_1_46F0
        db  083h, 03Dh, 0D0h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_1_46F0
        db  03Bh, 005h, 0D4h, 0F6h, 000h, 000h
        je     SHORT loc_1_46F0
        db  0FFh, 015h, 028h, 0FDh, 000h, 000h
        jmp    SHORT loc_1_475B
loc_1_46F0:
        db  0FFh, 015h, 028h, 0FDh, 000h, 000h
        shr    edi, 0ch
        imul   edi, edi, 0ah
        db  003h, 03Dh, 0ACh, 0F8h, 000h, 000h
        push   edi
        call   sub_1_369C
        or     eax, eax
        je     SHORT loc_1_475B
        movzx  eax, byte ptr [edi + 2]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        test   byte ptr [eax + 1ch], 2
        jne    SHORT loc_1_4725
        db  0A1h, 098h, 0F6h, 000h, 000h
        inc    dword ptr [eax - 28h]
loc_1_4725:
        push   edi
        push   ebx
        push   ebp
        call   sub_1_4802
        or     eax, eax
        mov    esi, ebx
        je     SHORT loc_1_4735
        sub    esi, esi
loc_1_4735:
        movzx  eax, byte ptr [edi + 2]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        test   byte ptr [eax + 1ch], 2
        jne    SHORT loc_1_474E
        db  0A1h, 098h, 0F6h, 000h, 000h
        dec    dword ptr [eax - 28h]
loc_1_474E:
        push   edi
        call   sub_1_36BD
        call   sub_1_5421
        jmp    SHORT loc_1_475D
loc_1_475B:
        sub    esi, esi
loc_1_475D:
        db  0FFh, 015h, 070h, 0F8h, 000h, 000h
        db  0A1h, 098h, 0F6h, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        sub    dword ptr [eax - 28h], ecx
        push   ecx
        call   sub_1_38A3
        jmp    SHORT loc_1_4781
loc_1_4777:
        db  039h, 005h, 034h, 0EFh, 000h, 000h
        je     SHORT loc_1_4781
        sub    esi, esi
loc_1_4781:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_478B:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 015h, 090h, 0EEh, 000h, 000h
        mov    ecx, 10h
        db  0C7h, 005h, 048h, 0F9h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 094h, 0EEh, 000h, 000h
        db  0A3h, 090h, 0EEh, 000h, 000h
        db  089h, 015h, 094h, 0EEh, 000h, 000h
        mov    edi, 1
        db  08Bh, 035h, 03Ch, 0F8h, 000h, 000h
loc_1_47BE:
        cmp    dword ptr [edx], esi
        jne    SHORT loc_1_47F8
        mov    eax, dword ptr [edx + 4]
        lea    ebx, [eax*4 - 800000h]
        mov    eax, ebx
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        test   dword ptr [eax - 800000h], edi
        je     SHORT loc_1_47F8
        mov    eax, dword ptr [ebx]
        test   edi, eax
        je     SHORT loc_1_47F8
        cmp    ebx, 0ffbfe000h
        jae    SHORT loc_1_47F8
        and    eax, 0ffffffdfh
        mov    dword ptr [ebx], eax
        and    eax, 0fffffbffh
        mov    dword ptr [ebx], eax
loc_1_47F8:
        add    edx, 8
        dec    ecx
        jne    SHORT loc_1_47BE
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_4802:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 2ch]
        mov    esi, dword ptr [edi + 6]
        test   byte ptr [edi + 3], 8
        je     SHORT loc_1_488E
        db  08Bh, 00Dh, 040h, 0F9h, 000h, 000h
        db  0A1h, 068h, 0FAh, 000h, 000h
        cmp    ecx, 1ah
        db  08Bh, 02Dh, 0E4h, 0F7h, 000h, 000h
        jge    SHORT loc_1_4850
loc_1_482C:
        mov    eax, dword ptr [eax + 8]
        and    eax, 0fffffh
        imul   eax, eax, 0dh
        add    eax, ebp
        cmp    esi, eax
        je     SHORT loc_1_4845
        inc    ecx
        cmp    ecx, 1ah
        jl     SHORT loc_1_482C
        jmp    SHORT loc_1_4850
loc_1_4845:
        db  0A1h, 064h, 0F8h, 000h, 000h
        inc    eax
        db  0A3h, 064h, 0F8h, 000h, 000h
loc_1_4850:
        db  039h, 035h, 0B0h, 0F8h, 000h, 000h
        jne    SHORT loc_1_4868
        mov    eax, dword ptr [esi + 9]
        shr    eax, 0ch
        imul   eax, eax, 0dh
        add    eax, ebp
        db  0A3h, 0B0h, 0F8h, 000h, 000h
loc_1_4868:
        push   esi
        call   sub_1_4E1F
        and    byte ptr [edi + 3], 0f7h
        mov    word ptr [esi + 0ah], 0
        mov    word ptr [esi + 8], 0
        db  0A1h, 044h, 0EFh, 000h, 000h
        inc    eax
        db  0A3h, 044h, 0EFh, 000h, 000h
        jmp    NEAR PTR loc_1_49BC
loc_1_488E:
        test   byte ptr [edi + 3], 10h
        jne    NEAR PTR loc_1_49B6
        db  0B8h, 030h, 0FDh, 000h, 000h
loc_1_489D:
        cmp    dword ptr [eax], edi
        je     SHORT loc_1_48AD
        add    eax, 4
        db  03Dh, 098h, 0FDh, 000h, 000h
        jb     SHORT loc_1_489D
        jmp    SHORT loc_1_48B8
loc_1_48AD:
        db  0A1h, 0A4h, 0FDh, 000h, 000h
        inc    eax
        db  0A3h, 0A4h, 0FDh, 000h, 000h
loc_1_48B8:
        mov    ebx, dword ptr [esp + 28h]
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 20h], eax
        call   sub_1_36D9
        movzx  eax, byte ptr [edi + 2]
        db  08Bh, 02Ch, 085h, 008h, 0FCh, 000h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 2
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     SHORT loc_1_48F1
        db  0A1h, 0F4h, 0F8h, 000h, 000h
        inc    eax
        db  0A3h, 0F4h, 0F8h, 000h, 000h
loc_1_48F1:
        call   sub_1_4B88
        mov    esi, eax
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_1_4903
        mov    ebp, dword ptr [ebp + 4]
        jmp    SHORT loc_1_4906
loc_1_4903:
        mov    ebp, dword ptr [ebp]
loc_1_4906:
        mov    dword ptr [esp + 14h], 1
        or     ebp, ebp
        je     SHORT loc_1_4950
        db  0A1h, 098h, 0F6h, 000h, 000h
        db  0FFh, 005h, 038h, 0EFh, 000h, 000h
        inc    dword ptr [eax - 28h]
        push   esi
        call   sub_1_4E68
        mov    dword ptr [esp + 18h], eax
        push   dword ptr [esp + 24h]
        lea    eax, [edi + 6]
        push   dword ptr [esp + 1ch]
        push   eax
        call   ebp
        mov    dword ptr [esp + 20h], eax
        add    esp, 0ch
        push   dword ptr [esp + 18h]
        call   sub_1_4EB1
        db  0A1h, 098h, 0F6h, 000h, 000h
        dec    dword ptr [eax - 28h]
loc_1_4950:
        db  0FFh, 035h, 0D8h, 0FCh, 000h, 000h
        call   sub_1_36EC
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_4999
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_4974
        db  0A1h, 0F4h, 0F8h, 000h, 000h
        dec    eax
        db  0A3h, 0F4h, 0F8h, 000h, 000h
loc_1_4974:
        mov    dword ptr [esi], 0
        db  0FFh, 005h, 040h, 0F9h, 000h, 000h
        db  0FFh, 035h, 0A0h, 0FDh, 000h, 000h
        db  0FFh, 035h, 060h, 0FAh, 000h, 000h
        push   esi
        call   sub_1_4D90
        sub    esi, esi
        jmp    NEAR PTR loc_1_4A68
loc_1_4999:
        mov    dword ptr [esi], edi
        mov    eax, dword ptr [edi + 6]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [edi + 6], esi
        or     byte ptr [edi + 3], 10h
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ebx], eax
        db  08Bh, 02Dh, 0E4h, 0F7h, 000h, 000h
        jmp    SHORT loc_1_49C0
loc_1_49B6:
        db  08Bh, 02Dh, 0E4h, 0F7h, 000h, 000h
loc_1_49BC:
        mov    ebx, dword ptr [esp + 28h]
loc_1_49C0:
        inc    word ptr [esi + 0ah]
        mov    eax, esi
        sub    eax, ebp
        mov    ecx, 0dh
        sub    edx, edx
        div    ecx
        shl    eax, 0ch
        mov    ecx, dword ptr [ebx]
        and    ecx, 0ddeh
        or     eax, ecx
        or     eax, 221h
        mov    dword ptr [ebx], eax
        cmp    ebx, 0ffbfe000h
        jb     SHORT loc_1_4A1A
        cmp    ebx, 0ffbfe7fch
        ja     SHORT loc_1_4A1A
        lea    edx, [ebx + 402000h]
        db  08Bh, 00Dh, 03Ch, 0F8h, 000h, 000h
        and    edx, 0fffffffch
        add    edx, dword ptr [ecx]
        mov    ebp, dword ptr [edx]
        xor    ebp, eax
        and    ebp, 400h
        xor    ebp, eax
        mov    dword ptr [ebx], ebp
        mov    eax, ebp
        mov    dword ptr [edx], eax
        jmp    SHORT loc_1_4A20
loc_1_4A1A:
        db  08Bh, 00Dh, 03Ch, 0F8h, 000h, 000h
loc_1_4A20:
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 004h
        je     SHORT loc_1_4A40
        movzx  eax, byte ptr [edi + 2]
        db  08Bh, 004h, 085h, 008h, 0FCh, 000h, 000h
        test   byte ptr [eax + 1ch], 1
        jne    SHORT loc_1_4A40
        mov    word ptr [esi + 8], 0f188h
loc_1_4A40:
        db  0A1h, 094h, 0EEh, 000h, 000h
        db  08Bh, 03Dh, 048h, 0F9h, 000h, 000h
        lea    edx, [eax + edi*8]
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        inc    edi
        db  089h, 03Dh, 048h, 0F9h, 000h, 000h
        cmp    edi, 10h
        jne    SHORT loc_1_4A68
        call   sub_1_478B
loc_1_4A68:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret    0ch
_LTEXT ends
        end

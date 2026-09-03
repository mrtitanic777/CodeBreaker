; sbawe_lcode_01_01.asm -- SBAWE object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  00Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  011h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  012h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  013h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  015h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  017h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 046h
        db  046h, 0D9h, 0D4h, 047h, 0E8h, 062h, 0CFh, 011h, 093h, 0BCh, 044h, 045h, 053h, 054h, 000h, 000h
        db  083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  084h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  086h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
loc_1_0200:
        sal    byte ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0210:
        sal    dword ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
sub_1_0220:
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0ffh
        mov    esi, dword ptr [esp + 10h]
        push   0
        push   1
        push   ebx
        push   esi
        call   sub_1_1A68
        add    esp, 10h
        mov    edi, eax
        mov    eax, 1
        cmp    edi, ebx
        je     SHORT loc_1_026F
        mov    eax, dword ptr [esp + 14h]
        push   80h
        mov    dword ptr [eax], edi
        mov    eax, edi
        or     eax, 2a0h
        or     edi, 60h
        push   eax
        push   esi
        call   sub_1_19AA
        push   0ffffh
        push   edi
        push   esi
        call   sub_1_1A14
        xor    eax, eax
loc_1_026F:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0273:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   0ffffh
        or     al, 60h
        mov    esi, dword ptr [esp + 0ch]
        push   eax
        push   esi
        call   sub_1_1A14
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    word ptr [eax + esi + 42h], 0ffeh
        xor    eax, eax
        pop    esi
        ret
sub_1_029C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 4], 0
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
loc_1_02B1:
        push   0
        push   0
        push   0ffh
        push   esi
        call   sub_1_1A68
        add    esp, 10h
        mov    byte ptr [ebp + ebx - 0ch], al
        movzx  edi, al
        cmp    edi, 0ffh
        je     NEAR PTR loc_1_035D
        mov    eax, edi
        inc    dword ptr [ebp - 4]
        shl    eax, 5
        cmp    byte ptr [eax + esi + 59h], 0
        je     SHORT loc_1_02E8
        dec    dword ptr [esi + 18h]
loc_1_02E8:
        push   80h
        mov    eax, edi
        or     eax, 2a0h
        push   eax
        push   esi
        call   sub_1_19AA
        push   0
        mov    eax, edi
        or     al, 60h
        push   eax
        push   esi
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 40h
        push   eax
        push   esi
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 0c0h
        push   eax
        push   esi
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 0e0h
        push   eax
        push   esi
        call   sub_1_1A14
        push   40000000h
        mov    eax, edi
        or     al, 20h
        push   eax
        push   esi
        call   sub_1_1A14
        push   40000000h
        push   edi
        push   esi
        or     edi, 200h
        call   sub_1_1A14
        push   6000000h
        push   edi
        push   esi
        call   sub_1_1A14
loc_1_035D:
        inc    ebx
        cmp    ebx, 5
        jb     NEAR PTR loc_1_02B1
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0377
        mov    eax, 1
        jmp    NEAR PTR loc_1_040C
loc_1_0377:
        push   dword ptr [ebp + 0ch]
        push   236h
        push   esi
        call   sub_1_1A14
        mov    eax, 3ah
        mov    edi, dword ptr [ebp + 14h]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        test   edi, edi
        out    dx, ax
        je     SHORT loc_1_03AC
        movzx  edx, word ptr [esi + 2]
        mov    ecx, dword ptr [ebp + 10h]
loc_1_03A1:
        mov    ax, word ptr [ecx]
        add    ecx, 2
        out    dx, ax
        dec    edi
        jne    SHORT loc_1_03A1
loc_1_03AC:
        mov    dx, word ptr [esi + 2]
        xor    eax, eax
        out    dx, ax
        xor    edi, edi
loc_1_03B6:
        movzx  ebx, byte ptr [ebp + edi - 0ch]
        cmp    ebx, 0ffh
        je     SHORT loc_1_0404
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
        push   1d8h
        mov    eax, ebx
        or     al, 0c0h
        push   eax
        push   esi
        call   sub_1_1A14
        push   1e0h
        mov    eax, ebx
        shl    ebx, 5
        or     al, 0e0h
        push   eax
        push   esi
        call   sub_1_1A14
        lea    eax, [ebx + esi]
        mov    word ptr [eax + 42h], 0ffeh
        mov    byte ptr [eax + 59h], 1
        inc    dword ptr [esi + 18h]
loc_1_0404:
        inc    edi
        cmp    edi, 5
        jb     SHORT loc_1_03B6
        xor    eax, eax
loc_1_040C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Dh, 02Dh, 02Dh
        db  020h, 043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 028h, 063h, 029h, 020h, 043h
        db  072h, 065h, 061h, 074h, 069h, 076h, 065h, 020h, 054h, 065h, 063h, 068h, 06Eh, 06Fh, 06Ch, 06Fh
        db  067h, 079h, 020h, 04Ch, 074h, 064h, 02Eh, 020h, 031h, 039h, 039h, 034h, 02Dh, 039h, 036h, 020h
        db  02Dh, 02Dh, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_047C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
sub_1_0480:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0484:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0488:
        add    byte ptr [eax], al
loc_1_048A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_09C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    al, 0
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 800000h], 0
        add    byte ptr [eax + 7f007dh], al
        jg     SHORT loc_1_0A02
loc_1_0A02:
        add    byte ptr [eax], al
        jg     SHORT loc_1_0A06
loc_1_0A06:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 7dh], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jg     SHORT loc_1_0A16
loc_1_0A16:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        jg     SHORT loc_1_0A23
loc_1_0A23:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    cl, dl
        add    byte ptr [eax], al
        and    cl, dl
        add    byte ptr [eax], al
        and    cl, dl
        and    cl, dl
        and    cl, dl
        and    cl, dl
        jg     SHORT loc_1_0A9C
loc_1_0A9C:
        and    cl, dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    cl, dl
        and    cl, dl
        and    cl, dl
        and    cl, dl
        jg     SHORT loc_1_0AAC
loc_1_0AAC:
        and    cl, dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        jg     SHORT loc_1_0ABB
loc_1_0ABB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    bh, bh
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 31h], ah
        xor    byte ptr [edi], ch
        sub    eax, 2a2a2b2ch
        sub    dword ptr [eax], ebp
        daa
        and    eax, 22232424h
        and    dword ptr [ecx], esp
        and    byte ptr [edi], bl
        push   ds
        push   ds
        sbb    eax, 1a1b1c1ch
        sbb    bl, byte ptr [ecx]
        sbb    dword ptr [eax], ebx
        sbb    byte ptr [edi], dl
        push   ss
        push   ss
        adc    eax, 13141415h
        adc    edx, dword ptr [ebx]
        adc    dl, byte ptr [edx]
        adc    dword ptr [ecx], edx
        adc    byte ptr [eax], dl
        adc    byte ptr [edi], cl
        db  00Fh, 00Fh, 00Eh, 00Eh, 00Eh, 00Dh, 00Dh, 00Dh, 00Ch, 00Ch, 00Ch, 00Bh, 00Bh, 00Bh, 00Bh, 00Ah
        db  00Ah, 00Ah, 00Ah, 009h, 009h, 009h, 009h, 009h, 008h, 008h, 008h, 008h, 008h, 007h, 007h, 007h
        db  007h, 007h, 006h, 006h, 006h, 006h, 006h, 006h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 004h
        db  004h, 004h, 004h, 004h, 004h, 003h, 003h, 003h, 003h, 003h, 003h, 002h, 002h, 002h, 002h, 002h
        db  002h, 001h, 001h, 001h, 001h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_0B78:
        arpl   word ptr [ebx + 63h], sp
        arpl   word ptr [ebx + 63h], sp
        arpl   word ptr [ebx + 63h], sp
        arpl   word ptr [ebx + 2bh], sp
        sub    dword ptr [eax], ebp
        daa
        and    eax, 21222324h
        and    byte ptr [edi], bl
        push   ds
        push   ds
        sbb    eax, 1a1b1b1ch
        sbb    dword ptr [ecx], ebx
        sbb    byte ptr [edi], dl
        pop    ss
        push   ss
        push   ss
        adc    eax, 13141415h
        adc    edx, dword ptr [ebx]
        adc    dl, byte ptr [edx]
        adc    dword ptr [ecx], edx
        adc    dword ptr [eax], edx
        adc    byte ptr [eax], dl
        db  00Fh, 00Fh, 00Fh, 00Eh, 00Eh, 00Eh, 00Eh, 00Dh, 00Dh, 00Dh, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Bh
        db  00Bh, 00Bh, 00Bh, 00Ah, 00Ah, 00Ah, 00Ah, 009h, 009h, 009h, 009h, 009h, 008h, 008h, 008h, 008h
        db  008h, 007h, 007h, 007h, 007h, 006h, 006h, 006h, 006h, 006h, 005h, 005h, 005h, 005h, 005h, 004h
        db  004h, 004h, 004h, 004h, 003h, 003h, 003h, 003h, 003h, 002h, 002h, 002h, 002h, 002h, 002h, 001h
        db  001h, 001h, 001h, 001h, 001h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0BF8:
        jg     SHORT loc_1_0C66
        bound  ebx, qword ptr [edx + 54h]
        push   eax
        dec    ebx
        dec    eax
        inc    ebp
        inc    edx
        inc    eax
        cmp    eax, 3638393bh
        xor    al, 33h
        xor    dword ptr [eax], esi
        das
        sub    eax, 292a2b2ch
        sub    byte ptr [edi], ah
        and    eax, 22232424h
        and    dword ptr [ecx], esp
        and    byte ptr [edi], bl
        push   ds
        push   ds
        sbb    eax, 1b1b1c1dh
        sbb    bl, byte ptr [edx]
        sbb    dword ptr [eax], ebx
        sbb    byte ptr [edi], dl
        pop    ss
        push   ss
        push   ss
        adc    eax, 14141515h
        adc    edx, dword ptr [ebx]
        adc    dl, byte ptr [edx]
        adc    dword ptr [ecx], edx
        adc    dword ptr [eax], edx
        adc    byte ptr [edi], cl
        db  00Fh, 00Fh, 00Eh, 00Eh, 00Eh, 00Dh, 00Dh, 00Dh, 00Ch, 00Ch, 00Ch, 00Bh, 00Bh, 00Bh, 00Ah, 00Ah
        db  00Ah, 009h, 009h, 009h, 009h, 008h, 008h, 008h, 007h, 007h, 007h, 007h, 006h, 006h, 006h, 006h
        db  005h, 005h, 005h, 004h, 004h, 004h, 004h, 004h, 003h
loc_1_0C66:
        add    eax, dword ptr [ebx]
        add    eax, dword ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C78:
        shr    dl, 0c8h
        loopne loc_1_0CBD
        xlatb
        and    dh, cl
        pushad
        lds    ecx, ptr [eax]
        mov    ebp, 0ad58b500h
        clc
        movsd
        db  0F0h, 09Eh, 030h, 098h, 0C0h, 091h, 090h, 08Bh, 0A8h, 085h, 000h, 080h, 090h, 07Ah, 0F0h, 041h
        db  0FEh, 042h, 012h, 040h, 001h, 030h, 0FEh, 0FEh, 0F7h, 0FFh, 000h, 000h, 000h, 000h
loc_1_0CA8:
        or     byte ptr [eax], al
sub_1_0CAA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0F0h, 041h, 0FEh, 042h, 012h, 040h, 001h, 038h, 0FEh, 0FEh, 0F7h, 0FFh, 000h, 000h, 000h
loc_1_0CBD:
        add    byte ptr [eax], cl
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0CC8:
        adc    eax, 0
        add    byte ptr [esi], dl
        add    dword ptr [eax + 17000004h], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    dword ptr [eax + 19000004h], esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    eax, 0
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    eax, 0
        add    byte ptr [esi], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], al
        add    byte ptr [edx + eax], al
        add    byte ptr [1046001h], al
        add    byte ptr [esi], al
        add    dword ptr [eax + 7000104h], esp
        add    dword ptr [eax + 4], eax
        add    dword ptr [eax], eax
        or     eax, 1048001h
        add    byte ptr [eax], cl
        add    dword ptr [eax], esp
        add    al, 1
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    dword ptr [eax + 4], esp
        add    byte ptr [eax], al
        or     eax, dword ptr [ecx]
        inc    eax
        add    al, 0
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D82:
        xor    dx, word ptr cs:[edi]
        db  077h, 00Fh
        lcall  0a107h, 0bc09480bh
        push   es
        int    5
        sub    byte ptr [43804a4h], al
        fiadd  word ptr [ebx]
        xchg   edx, eax
        add    edx, dword ptr [eax + 3]
        sbb    byte ptr [ebx], al
        out    2, al
        mov    ebx, 71029402h
        add    dl, byte ptr [edx + 2]
        add    bl, byte ptr ss:[edx + eax]
        add    al, 2
        out    dx, eax
        add    ebx, ebx
        add    ecx, ecx
        add    dword ptr [eax - 65fe57ffh], edi
        add    dword ptr [ecx + eax + 173017fh], ecx
        pop    ebp
        add    dword ptr [edx + 1], ecx
        cmp    dword ptr [ecx], eax
        sub    dword ptr [ecx], eax
        sbb    eax, dword ptr [ecx]
        push   cs
        add    dword ptr [edx], eax
        add    edi, esi
        add    dh, ch
        add    ah, ah
        add    ah, bl
        add    ah, dl
        add    ch, cl
        add    dh, al
        add    al, al
        add    byte ptr [edx - 5aff5100h], bh
        add    byte ptr [eax + eax + 8d0094h], bl
        xchg   dword ptr [eax], eax
        add    dword ptr [eax], 77007ch
        jb     SHORT loc_1_0DF6
loc_1_0DF6:
        outsb
        add    byte ptr [edx], ch
        add    byte ptr [ebx], ah
        pushad
        add    byte ptr [ebp], bl
        push   edi
        add    byte ptr [edx], dl
        dec    esi
        add    byte ptr [edx], cl
        inc    edi
        add    byte ptr [ebx], al
        inc    ecx
        add    byte ptr [esi], bh
        add    byte ptr [ebx], bh
        add    byte ptr [ecx], bh
        add    byte ptr [edi], dh
        add    byte ptr [31003300h], dh
        add    byte ptr [eax], dh
        add    byte ptr [esi], ch
        add    byte ptr [eax + eax], ch
        sub    dword ptr [eax], eax
        daa
        add    byte ptr [22002300h], ah
        add    byte ptr [eax], ah
        add    byte ptr [edi], bl
        add    byte ptr [esi], bl
        add    byte ptr [1b001b00h], bl
        add    byte ptr [edx], bl
        add    byte ptr [ecx], bl
        add    byte ptr [eax], bl
        add    byte ptr [edi], dl
        add    byte ptr [esi], dl
        add    byte ptr [13001400h], dl
        add    byte ptr [edx], dl
        add    byte ptr [ecx], dl
        add    byte ptr [eax], dl
        add    byte ptr [edi], cl
        add    byte ptr [edi], cl
        add    byte ptr [esi], cl
        add    byte ptr [esi], cl
        add    byte ptr [0c000d00h], cl
        add    byte ptr [eax + eax], cl
        or     al, 0
        or     eax, dword ptr [eax]
        or     al, byte ptr [eax]
        or     al, byte ptr [eax]
        or     dword ptr [eax], eax
        or     dword ptr [eax], eax
        or     byte ptr [eax], al
        or     byte ptr [eax], al
        or     byte ptr [eax], al
        pop    es
        add    byte ptr [edi], al
        add    byte ptr [edi], al
        add    byte ptr [edi], al
        add    byte ptr [esi], al
        add    byte ptr [esi], al
        add    byte ptr [esi], al
        db  000h
loc_1_0E80:
        cdq
        mov    ecx, 3dde5ccch
        pop    ds
        and    eax, 1a841eefh
        xor    edx, dword ptr [edi]
        lahf
        adc    al, 8fh
        adc    bl, bh
        adc    byte ptr [edi + 0fh], dh
        inc    edi
        push   cs
        inc    edx
        or     eax, 0b9a0c60h
        jmp    SHORT loc_1_0EAC
        db  050h, 00Ah, 0C5h, 009h, 048h, 009h, 0D7h, 008h, 070h, 008h
loc_1_0EAC:
        adc    cl, byte ptr [eax]
        mov    esp, 23076d07h
        pop    es
        loopne loc_1_0EBC
        mov    eax, dword ptr [30066606h]
        push   es
loc_1_0EBC:
        std
        add    eax, 57505cdh
        sub    byte ptr [4a404e2h], al
        imul   eax, dword ptr [eax + edi], 4
        or     dword ptr [esi + ebx*8], eax
        add    esi, dword ptr [esi + 70039203h]
        add    edx, dword ptr [eax + 3]
        xor    eax, dword ptr [ebx]
        sbb    byte ptr [ebx], al
        inc    byte ptr [edx]
        out    2, al
        mov    ebx, 71029402h
        add    dl, byte ptr [edx + 2]
        add    bl, byte ptr ss:[edx + eax]
        add    al, 2
        out    dx, eax
        add    ebx, ebx
        add    ecx, ecx
        add    dword ptr [eax - 65fe57ffh], edi
        add    dword ptr [ecx + eax + 173017fh], ecx
        pop    ebp
        add    dword ptr [edx + 1], ecx
        cmp    dword ptr [ecx], eax
        sub    dword ptr [ecx], eax
        sbb    eax, dword ptr [ecx]
        push   cs
        add    dword ptr [edx], eax
        add    edi, esi
        add    dh, ch
        add    ah, ah
        add    ah, bl
        add    ah, dl
        add    ch, cl
        add    dh, al
        add    al, al
        add    byte ptr [edx - 5aff5100h], bh
        add    byte ptr [eax + eax + 8d0094h], bl
        xchg   dword ptr [eax], eax
        add    dword ptr [eax], 77007ch
        jb     SHORT loc_1_0F34
loc_1_0F34:
        outsb
        add    byte ptr [edx], ch
        add    byte ptr [ebx], ah
        pushad
        add    byte ptr [ebp], bl
        push   edi
        add    byte ptr [edx], dl
        dec    esi
        add    byte ptr [edx], cl
        inc    edi
        add    byte ptr [ebx], al
        inc    ecx
        add    byte ptr [esi], bh
        add    byte ptr [ebx], bh
        add    byte ptr [ecx], bh
        add    byte ptr [edi], dh
        add    byte ptr [31003300h], dh
        add    byte ptr [eax], dh
        add    byte ptr [esi], ch
        add    byte ptr [eax + eax], ch
        sub    dword ptr [eax], eax
        daa
        add    byte ptr [22002300h], ah
        add    byte ptr [eax], ah
        add    byte ptr [edi], bl
        add    byte ptr [esi], bl
        add    byte ptr [1b001b00h], bl
        add    byte ptr [edx], bl
        add    byte ptr [ecx], bl
        add    byte ptr [eax], bl
        add    byte ptr [edi], dl
        add    byte ptr [esi], bh
        jmp    NEAR PTR loc_1_0F8A
loc_1_0F8A:
        int3
        int3
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 0A4h, 032h, 004h, 01Eh, 000h, 000h, 053h, 042h, 041h, 057h
        db  045h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_0FE0:
        mov    edi, 0b8cccccch
        push   ds
        add    al, 0
        add    bl, al
sub_1_0FEA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_1008
        mov    eax, dword ptr [ebp + 14h]
loc_1_0FFC:
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_1011
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_1_0FFC
loc_1_1008:
        mov    eax, 1
loc_1_100D:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_1011:
        mov    ax, word ptr [ebp + 10h]
        cmp    ax, 64h
        jb     NEAR PTR loc_1_10F2
        movzx  eax, ax
        cmp    eax, 64h
        je     SHORT loc_1_104A
        cmp    eax, 65h
        je     SHORT loc_1_1068
        cmp    eax, 66h
        je     SHORT loc_1_107B
        cmp    eax, 67h
        je     NEAR PTR loc_1_10BE
        cmp    eax, 68h
        je     NEAR PTR loc_1_10E1
        mov    eax, 8006h
        jmp    SHORT loc_1_100D
loc_1_104A:
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_1056
        mov    eax, 8001h
        jmp    SHORT loc_1_100D
loc_1_1056:
        push   esi
        call   sub_1_5115
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        xor    eax, eax
        jmp    SHORT loc_1_100D
loc_1_1068:
        push   esi
        call   sub_1_5118
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi], 0
        jmp    SHORT loc_1_100D
loc_1_107B:
        mov    edi, dword ptr [ebp + 1ch]
        push   dword ptr [edi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1085:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_1_1008
        push   dword ptr [edi]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_109A:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_10AF
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_10A7:
        add    esp, 4
        jmp    NEAR PTR loc_1_1008
loc_1_10AF:
        shl    eax, 10h
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_100D
loc_1_10BE:
        mov    edi, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [edi]
        shr    eax, 10h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_10CC:
        add    esp, 4
        push   dword ptr [edi + 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_10D7:
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_100D
loc_1_10E1:
        push   dword ptr [ebp + 1ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_10EA:
        add    esp, 8
        jmp    NEAR PTR loc_1_100D
loc_1_10F2:
        push   dword ptr [ebp + 1ch]
        add    esi, 24h
        push   esi
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_52FB
        add    esp, 10h
        jmp    NEAR PTR loc_1_100D
sub_1_110A:
        mov    eax, dword ptr [esp + 4]
loc_1_110E:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_1126
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 8], eax
        jne    SHORT loc_1_110E
        mov    eax, ecx
        jmp    SHORT loc_1_1128
loc_1_1126:
        xor    eax, eax
loc_1_1128:
        ret
        db  0CCh, 0CCh, 0CCh
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   dword ptr [ebp + 0ch]
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_1156
        add    eax, dword ptr [ebp + 10h]
        cmp    eax, 110000h
        jae    SHORT loc_1_1156
        add    eax, dword ptr [ebx + 4]
loc_1_1156:
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret
        db  090h
loc_1_115C:
        pushfd
        cld
        push   ebp
        mov    ebp, esp
        push   ecx
        call   edx
        mov    esp, ebp
        pop    ebp
        popfd
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_116C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
SBAWE_Control:
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_11A9:
        cmp    eax, 2
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_11B2:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_11BB:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_11C4:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_11CD:
        clc
        ret
        db  090h, 0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_11D8:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_1221
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_1205
        xor    eax, eax
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_1205
        push   edi
        call   sub_1_5118
        add    esp, 4
loc_1_1205:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
loc_1_120B:
        or     edi, edi
        je     SHORT loc_1_1221
        cmp    ebx, dword ptr [edi]
        je     SHORT loc_1_1218
        mov    edi, dword ptr [edi + 4]
        jmp    SHORT loc_1_120B
loc_1_1218:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_121E:
        add    esp, 4
loc_1_1221:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
SBAWE_PM_API:
        pushfd
        cld
        push   ebp
        xor    eax, eax
        cmp    word ptr [ebp + 38h], 0
        je     SHORT loc_1_123F
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
loc_1_123F:
        push   eax
        movzx  eax, word ptr [ebp + 4]
        push   eax
        mov    ax, word ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 1ch]
        push   eax
        movzx  eax, word ptr [ebp + 14h]
        push   eax
        movzx  eax, word ptr [ebp + 10h]
        push   eax
        mov    eax, ebx
        push   eax
        call   sub_1_0FEA
        add    esp, 18h
        pop    ebp
        popfd
        mov    word ptr [ebp + 1ch], ax
        mov    cl, 10h
        shr    eax, cl
        mov    word ptr [ebp + 14h], ax
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_1278:
        push   eax
        mov    eax, dword ptr [esi]
        cmp    eax, ebx
        jne    SHORT loc_1_12B4
loc_1_127F:
        pop    eax
        cmp    ecx, 0ch
        jne    SHORT loc_1_1288
        out    dx, ax
        ret
loc_1_1288:
        cmp    ecx, 4
        jne    SHORT loc_1_1290
        out    dx, al
        ret
        db  090h
loc_1_1290:
        cmp    ecx, 14h
        jne    SHORT loc_1_1298
        out    dx, eax
        ret
        db  090h
loc_1_1298:
        cmp    ecx, 8
        jne    SHORT loc_1_12A0
        in     ax, dx
        ret
loc_1_12A0:
        cmp    ecx, 10h
        jne    SHORT loc_1_12A8
        in     eax, dx
        ret
        db  090h
loc_1_12A8:
        in     al, dx
        ret
        db  08Bh, 0C0h
loc_1_12AC:
        pop    eax
        xor    eax, eax
        dec    eax
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_12B4:
        or     eax, eax
        jne    SHORT loc_1_12AC
        mov    dword ptr [esi], ebx
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_12C4:
        add    esp, 4
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        jmp    SHORT loc_1_127F
        db  02Eh, 08Bh, 0C0h
loc_1_12D0:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_12FB
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_12E8
        mov    eax, 0ffffffffh
        stc
        ret
loc_1_12E8:
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        pushad
        mov    ebp, esp
        push   esi
        call   sub_1_152E
        add    esp, 4
        popad
loc_1_12FB:
        cmp    ecx, 4
        jne    SHORT loc_1_1340
        test   edx, 1
        jne    SHORT loc_1_1328
        test   edx, 8000h
        je     SHORT loc_1_1318
        test   edx, 100h
        jne    SHORT loc_1_1328
loc_1_1318:
        pushad
        mov    ebp, esp
        push   eax
        push   esi
        call   sub_1_153E
        add    esp, 8
        popad
        ret
        db  090h
loc_1_1328:
        cmp    al, 0ffh
        je     SHORT loc_1_1334
        cmp    al, 3fh
        je     SHORT loc_1_1334
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_1334:
        push   eax
        mov    al, 3fh
        db  0A2h, 000h, 000h, 000h, 000h
        pop    eax
        ret
        db  08Bh, 0C0h
loc_1_1340:
        cmp    ecx, 0
        jne    SHORT loc_1_136E
        test   edx, 1
        jne    SHORT loc_1_1368
        test   edx, 8000h
        je     SHORT loc_1_135D
        test   edx, 100h
        jne    SHORT loc_1_1368
loc_1_135D:
        mov    al, 0bfh
        db  0A2h, 000h, 000h, 000h, 000h
        mov    al, 0feh
        ret
        db  090h
loc_1_1368:
        db  0A0h, 000h, 000h, 000h, 000h
        ret
loc_1_136E:
        mov    eax, 0ffffffffh
        stc
        ret
sub_1_1375:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        test   eax, eax
        push   esi
        je     SHORT loc_1_13AF
        cmp    eax, 1
        je     NEAR PTR loc_1_1424
        cmp    eax, 5
        je     NEAR PTR loc_1_143B
        cmp    eax, 6
        je     NEAR PTR loc_1_14A4
        cmp    eax, 7
        je     NEAR PTR loc_1_14B3
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_14F5
loc_1_13AF:
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_14F3
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   esi
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_14F3
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   2
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        add    esp, 10h
        cmp    ax, si
        jne    SHORT loc_1_13FF
        mov    eax, 1
        jmp    NEAR PTR loc_1_14F5
loc_1_13FF:
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        push   0
        db  0A3h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        push   3
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_14EA
loc_1_1424:
        xor    eax, eax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_14F3
loc_1_143B:
        xor    eax, eax
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_14F3
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_1486
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx], eax
        jne    NEAR PTR loc_1_14F3
        push   10h
        lea    eax, [ecx + 24h]
        push   eax
        call   sub_1_18E9
        push   dword ptr [ebp + 0ch]
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_1_153E
        add    esp, 8
        mov    ecx, dword ptr [ebp + 10h]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_14F3
loc_1_1486:
        mov    eax, dword ptr [ebp + 10h]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_14F3
        push   dword ptr [ebp + 0ch]
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_1_153E
        add    esp, 8
        jmp    SHORT loc_1_14F3
loc_1_14A4:
        xor    eax, eax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_14F3
loc_1_14B3:
        mov    eax, dword ptr [ebp + 0ch]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_14F3
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_14D1
        push   eax
        call   sub_1_5118
        add    esp, 4
loc_1_14D1:
        xor    eax, eax
        db  0A3h, 000h, 000h, 000h, 000h
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_14F3
        push   eax
        push   eax
        push   4
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
loc_1_14EA:
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 10h
loc_1_14F3:
        xor    eax, eax
loc_1_14F5:
        pop    esi
        pop    ebp
        ret
sub_1_14F8:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_152B
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_152B
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [eax + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1528:
        add    esp, 0ch
loc_1_152B:
        xor    eax, eax
        ret
sub_1_152E:
        mov    eax, dword ptr [esp + 4]
        push   10h
        add    eax, 24h
        push   eax
        call   sub_1_18E9
        ret
sub_1_153E:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        je     NEAR PTR loc_1_17C0
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_17C0
        mov    cl, byte ptr [ebp + 0ch]
        test   cl, 80h
        je     NEAR PTR loc_1_15EE
        cmp    cl, 0f7h
        jne    SHORT loc_1_15A3
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_1_15A3
        db  0A1h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  088h, 088h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        call   sub_1_4B8B
        jmp    NEAR PTR loc_1_17B6
loc_1_15A3:
        cmp    cl, 0f8h
        jae    NEAR PTR loc_1_17C0
        mov    eax, 1
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        movzx  edx, cl
        and    dl, 0f0h
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    edx, 0f0h
        je     SHORT loc_1_15D4
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_17C0
loc_1_15D4:
        sub    cl, 0f0h
        cmp    cl, 1
        sbb    eax, eax
        and    ax, 2
        movzx  ecx, ax
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_17C0
loc_1_15EE:
        xor    eax, eax
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_17C0
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  088h, 08Ah, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_1_162E
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 020h
        jne    NEAR PTR loc_1_17C0
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_17C0
loc_1_162E:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 003h
        je     SHORT loc_1_164A
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0f0h
        cmp    al, 0c0h
        je     SHORT loc_1_164A
        cmp    al, 0d0h
        jne    NEAR PTR loc_1_17C0
loc_1_164A:
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        and    eax, 0f0h
        cmp    eax, 80h
        je     SHORT loc_1_16A0
        cmp    eax, 90h
        je     SHORT loc_1_16CC
        cmp    eax, 0a0h
        je     NEAR PTR loc_1_16F8
        cmp    eax, 0b0h
        je     NEAR PTR loc_1_1724
        cmp    eax, 0c0h
        je     NEAR PTR loc_1_174D
        cmp    eax, 0d0h
        je     NEAR PTR loc_1_176E
        cmp    eax, 0e0h
        je     NEAR PTR loc_1_178F
        jmp    NEAR PTR loc_1_17B6
loc_1_16A0:
        db  00Fh, 0B6h, 005h, 002h, 000h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 001h, 000h, 000h, 000h
        push   eax
        push   ecx
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        and    eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        call   sub_1_3C92
        jmp    NEAR PTR loc_1_17B6
loc_1_16CC:
        db  00Fh, 0B6h, 005h, 002h, 000h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 001h, 000h, 000h, 000h
        push   eax
        push   ecx
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        and    eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        call   sub_1_3DB3
        jmp    NEAR PTR loc_1_17B6
loc_1_16F8:
        db  00Fh, 0B6h, 005h, 002h, 000h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 001h, 000h, 000h, 000h
        push   eax
        push   ecx
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        and    eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        call   sub_1_4A99
        jmp    NEAR PTR loc_1_17B6
loc_1_1724:
        db  00Fh, 0B6h, 005h, 002h, 000h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 001h, 000h, 000h, 000h
        push   eax
        push   ecx
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        and    eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        call   sub_1_42A5
        jmp    SHORT loc_1_17B6
loc_1_174D:
        db  00Fh, 0B6h, 005h, 001h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        and    eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        call   sub_1_488D
        jmp    SHORT loc_1_17B6
loc_1_176E:
        db  00Fh, 0B6h, 005h, 001h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        and    eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        call   sub_1_4A9E
        jmp    SHORT loc_1_17B6
loc_1_178F:
        db  00Fh, 0B6h, 005h, 002h, 000h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 001h, 000h, 000h, 000h
        push   eax
        push   ecx
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        and    eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        push   eax
        call   sub_1_4AD6
loc_1_17B6:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_17C0:
        pop    ebp
        ret
loc_1_17C2:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_1839
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   5
        call   sub_1_116C
        add    esp, 0ch
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_1843
loc_1_17F4:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        imul   ecx, ecx, 14h
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  08Bh, 091h, 004h, 000h, 000h, 000h
        db  083h, 025h, 000h, 000h, 000h, 000h, 03Fh
        test   edx, edx
        je     SHORT loc_1_182F
        db  0FFh, 0B1h, 010h, 000h, 000h, 000h
        db  0FFh, 0B1h, 00Ch, 000h, 000h, 000h
        push   dword ptr [esp + 10h]
        db  0FFh, 0B1h, 008h, 000h, 000h, 000h
        call   edx
        add    esp, 10h
loc_1_182F:
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_17F4
        jmp    SHORT loc_1_1843
loc_1_1839:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1843:
        pop    esi
        ret
sub_1_1845:
        push   ebp
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  0A1h, 000h, 000h, 000h, 000h
        push   ebx
        lea    ecx, [edx + 1]
        push   esi
        and    ecx, 3fh
        push   edi
        cmp    ecx, eax
        jne    SHORT loc_1_1867
        mov    eax, 1
        jmp    SHORT loc_1_18E2
loc_1_1867:
        pushfd
        cli
        mov    edi, dword ptr [ebp + 8]
        cmp    edx, eax
        je     SHORT loc_1_188D
        xor    ebx, ebx
loc_1_1872:
        mov    esi, eax
        imul   esi, esi, 14h
        db  039h, 0BEh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_1885
        db  089h, 09Eh, 004h, 000h, 000h, 000h
loc_1_1885:
        inc    eax
        and    eax, 3fh
        cmp    eax, edx
        jne    SHORT loc_1_1872
loc_1_188D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 10h]
        imul   edx, edx, 14h
        mov    esi, dword ptr [ebp + 14h]
        db  089h, 0BAh, 000h, 000h, 000h, 000h
        db  089h, 082h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 18h]
        db  089h, 09Ah, 008h, 000h, 000h, 000h
        db  089h, 0B2h, 00Ch, 000h, 000h, 000h
        db  089h, 082h, 010h, 000h, 000h, 000h
        popfd
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_18E0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   5
        call   sub_1_116C
        add    esp, 0ch
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_18E0:
        xor    eax, eax
loc_1_18E2:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
sub_1_18E9:
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        cmp    dword ptr [esp + 18h], edi
        je     SHORT loc_1_1939
        mov    esi, dword ptr [esp + 14h]
        xor    ebx, ebx
        lea    ebp, [esi + 447h]
loc_1_1901:
        push   ebx
        push   edi
        push   esi
        inc    edi
        call   sub_1_488D
        mov    byte ptr [ebp], bl
        add    ebp, 20h
        mov    byte ptr [ebp - 1eh], 40h
        mov    byte ptr [ebp - 21h], bl
        mov    byte ptr [ebp - 1dh], 7fh
        mov    byte ptr [ebp - 1ch], 64h
        mov    byte ptr [ebp - 1bh], bl
        mov    byte ptr [ebp - 1ah], bl
        mov    byte ptr [ebp - 18h], 2
        mov    dword ptr [ebp - 15h], ebx
        cmp    edi, dword ptr [esp + 18h]
        mov    word ptr [ebp - 0dh], bx
        mov    byte ptr [ebp - 0eh], bl
        jb     SHORT loc_1_1901
loc_1_1939:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_1_1942:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd
        cli
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        mov    ecx, eax
        out    dx, al
        and    ch, 0feh
        and    eax, 100h
        shr    ecx, 8
        movzx  edx, word ptr [ecx + esi]
        shr    eax, 7
        or     edx, eax
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        popfd
        pop    esi
        pop    ebp
        ret    0ch
sub_1_1976:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd
        cli
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        mov    ecx, eax
        out    dx, al
        and    ch, 0feh
        shr    ecx, 8
        movzx  edx, word ptr [ecx + esi]
        mov    ecx, eax
        and    ecx, 100h
        shr    ecx, 7
        or     edx, ecx
        in     al, dx
        popfd
        pop    esi
        pop    ebp
        ret    8
sub_1_19AA:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd
        cli
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        mov    ecx, eax
        out    dx, al
        and    ch, 0feh
        and    eax, 100h
        shr    ecx, 8
        movzx  edx, word ptr [ecx + esi]
        shr    eax, 7
        or     edx, eax
        mov    eax, dword ptr [ebp + 10h]
        out    dx, ax
        popfd
        pop    esi
        pop    ebp
        ret    0ch
sub_1_19DF:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd
        cli
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        mov    ecx, eax
        out    dx, al
        and    ch, 0feh
        shr    ecx, 8
        movzx  edx, word ptr [ecx + esi]
        mov    ecx, eax
        and    ecx, 100h
        shr    ecx, 7
        or     edx, ecx
        in     ax, dx
        popfd
        pop    esi
        pop    ebp
        ret    8
sub_1_1A14:
        push   ebp
        mov    ebp, esp
        pushfd
        cli
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        movzx  edx, word ptr [ecx + 4]
        add    edx, 2
        out    dx, al
        mov    eax, dword ptr [ebp + 0ch]
        and    ah, 0feh
        shr    eax, 8
        mov    dx, word ptr [eax + ecx]
        mov    eax, dword ptr [ebp + 10h]
        out    dx, eax
        popfd
        pop    ebp
        ret    0ch
sub_1_1A3D:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd
        cli
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        movzx  edx, word ptr [ecx + 4]
        add    edx, 2
        mov    esi, dword ptr [ebp + 0ch]
        out    dx, al
        and    esi, 0fffffeffh
        shr    esi, 8
        mov    dx, word ptr [esi + ecx]
        in     eax, dx
        popfd
        pop    esi
        pop    ebp
        ret    8
sub_1_1A68:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        sub    esp, 8
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_1A8D
        cmp    eax, 1
        je     SHORT loc_1_1A9D
        cmp    eax, 2
        je     SHORT loc_1_1AAD
        mov    eax, 0ffh
        jmp    NEAR PTR loc_1_1B93
loc_1_1A8D:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_1B9A
        jmp    SHORT loc_1_1ABB
loc_1_1A9D:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_1CF3
        jmp    SHORT loc_1_1ABB
loc_1_1AAD:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_1E89
loc_1_1ABB:
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0ffh
        je     NEAR PTR loc_1_1B90
        mov    esi, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, esi
        shl    eax, 5
        xor    ebx, ebx
        add    edi, eax
        mov    eax, dword ptr [ebp + 0ch]
        shl    ax, 8
        push   0ffffh
        or     ax, word ptr [ebp + 10h]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [edi + 42h], ax
        mov    eax, esi
        or     al, 60h
        mov    dword ptr [edi + 4ch], ebx
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        cmp    dword ptr [ebp + 14h], 1
        jne    SHORT loc_1_1B34
        mov    dx, word ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 4]
        inc    esi
        push   0ffffh
        mov    eax, esi
        shl    eax, 5
        add    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 42h], dx
        mov    dword ptr [eax + 4ch], ebx
        mov    eax, esi
        or     al, 60h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
loc_1_1B34:
        mov    eax, esi
        mov    ebx, dword ptr [ebp + 8]
        shl    eax, 5
        add    ebx, eax
        test   byte ptr [ebx + 5ch], 2
        je     SHORT loc_1_1B80
        test   byte ptr [ebx + 7ch], 4
        je     SHORT loc_1_1B80
        inc    esi
        push   0ffh
        mov    eax, esi
        or     esi, 2a0h
        or     eax, 420h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        push   807fh
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    word ptr [ebx + 62h], 0ffeh
        mov    word ptr [ebx + 7ch], 0
loc_1_1B80:
        xor    eax, eax
        cmp    dword ptr [ebp + 14h], 1
        mov    word ptr [edi + 5ch], ax
        jne    SHORT loc_1_1B90
        mov    word ptr [edi + 7ch], ax
loc_1_1B90:
        mov    eax, dword ptr [ebp - 4]
loc_1_1B93:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1B9A:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 18h], 2
        ja     SHORT loc_1_1BBD
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_1BBD
        mov    dword ptr [ebp - 1ch], 0
        jmp    SHORT loc_1_1BC4
loc_1_1BBD:
        mov    dword ptr [ebp - 1ch], 1
loc_1_1BC4:
        mov    dword ptr [ebp - 18h], 0ffffffffh
        mov    dword ptr [ebp - 8], 0ffh
        mov    dword ptr [ebp - 0ch], 0
        lea    eax, [esi + 4ch]
        mov    dword ptr [ebp - 14h], eax
loc_1_1BDF:
        movzx  ecx, word ptr [esi + 6]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        cmp    ecx, eax
        jbe    NEAR PTR loc_1_1CAB
        mov    edi, dword ptr [ebp - 14h]
loc_1_1BF4:
        cmp    dword ptr [ebp - 1ch], 0
        jne    SHORT loc_1_1C04
        cmp    byte ptr [edi + 0dh], 0
        jne    NEAR PTR loc_1_1C97
loc_1_1C04:
        movzx  eax, word ptr [edi - 0ah]
        mov    dword ptr [ebp - 10h], eax
        cmp    eax, 2000h
        jae    SHORT loc_1_1C7C
        mov    eax, dword ptr [ebp - 4]
        or     al, 60h
        push   eax
        push   esi
        call   sub_1_1A3D
        shr    eax, 10h
        cmp    dword ptr [ebp - 10h], 0ffeh
        mov    ebx, eax
        je     SHORT loc_1_1C6A
        test   byte ptr [ebp - 0fh], 10h
        je     SHORT loc_1_1C3A
        add    ebx, 1200h
        jmp    SHORT loc_1_1C40
loc_1_1C3A:
        add    ebx, 200h
loc_1_1C40:
        cmp    byte ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_1C4E
        add    ebx, 300h
        jmp    SHORT loc_1_1C6E
loc_1_1C4E:
        mov    eax, dword ptr [ebp - 4]
        or     eax, 2a0h
        push   eax
        push   esi
        call   sub_1_19DF
        test   ah, 80h
        jne    SHORT loc_1_1C6E
        add    ebx, 1300h
        jmp    SHORT loc_1_1C6E
loc_1_1C6A:
        test   ebx, ebx
        je     SHORT loc_1_1CE3
loc_1_1C6E:
        cmp    ebx, dword ptr [ebp - 18h]
        ja     SHORT loc_1_1C7C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], ebx
        mov    dword ptr [ebp - 8], eax
loc_1_1C7C:
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_1C97
        mov    eax, dword ptr [ebp - 4]
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A3D
        and    eax, 0ffffffh
        cmp    dword ptr [edi], eax
        jbe    SHORT loc_1_1CEB
loc_1_1C97:
        add    dword ptr [ebp - 4], 3
        movzx  eax, word ptr [esi + 6]
        add    edi, 60h
        cmp    eax, dword ptr [ebp - 4]
        ja     NEAR PTR loc_1_1BF4
loc_1_1CAB:
        add    dword ptr [ebp - 14h], 20h
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 3
        jb     NEAR PTR loc_1_1BDF
        cmp    dword ptr [ebp - 8], 0ffh
        je     SHORT loc_1_1CD9
loc_1_1CC5:
        push   0ffh
        mov    eax, dword ptr [ebp - 8]
        or     eax, 2a0h
        push   eax
        push   esi
        call   sub_1_19AA
loc_1_1CD9:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1CE3:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_1_1CC5
loc_1_1CEB:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_1_1CC5
sub_1_1CF3:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 18h], 2
        ja     SHORT loc_1_1D16
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_1D16
        mov    dword ptr [ebp - 18h], 0
        jmp    SHORT loc_1_1D1D
loc_1_1D16:
        mov    dword ptr [ebp - 18h], 1
loc_1_1D1D:
        mov    dword ptr [ebp - 8], 0ffh
        mov    dword ptr [ebp - 14h], 0fffffh
        movzx  edi, word ptr [esi + 6]
        dec    edi
        mov    eax, edi
        shl    eax, 5
        mov    dword ptr [ebp - 0ch], 0fffffh
        lea    ebx, [eax + esi + 4ch]
loc_1_1D40:
        cmp    dword ptr [ebp - 18h], 0
        jne    SHORT loc_1_1D65
        cmp    byte ptr [ebx + 0dh], 0
        jne    NEAR PTR loc_1_1E2E
        movzx  eax, word ptr [esi + 6]
        lea    ecx, [edi + 1]
        cmp    eax, ecx
        jbe    SHORT loc_1_1D65
        cmp    byte ptr [ebx + 2dh], 0
        jne    NEAR PTR loc_1_1E2E
loc_1_1D65:
        movzx  eax, word ptr [ebx - 0ah]
        mov    dword ptr [ebp - 10h], eax
        cmp    eax, 2000h
        jae    NEAR PTR loc_1_1E2E
        mov    eax, edi
        or     al, 60h
        push   eax
        push   esi
        call   sub_1_1A3D
        shr    eax, 10h
        cmp    dword ptr [ebp - 10h], 0ffeh
        mov    dword ptr [ebp - 4], eax
        je     SHORT loc_1_1DD0
        test   byte ptr [ebp - 0fh], 10h
        je     SHORT loc_1_1DA0
        add    dword ptr [ebp - 4], 1200h
        jmp    SHORT loc_1_1DA7
loc_1_1DA0:
        add    dword ptr [ebp - 4], 200h
loc_1_1DA7:
        cmp    byte ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_1DB6
        add    dword ptr [ebp - 4], 300h
        jmp    SHORT loc_1_1DD0
loc_1_1DB6:
        mov    eax, edi
        or     eax, 2a0h
        push   eax
        push   esi
        call   sub_1_19DF
        test   ah, 80h
        jne    SHORT loc_1_1DD0
        add    dword ptr [ebp - 4], 1300h
loc_1_1DD0:
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_1DF1
        mov    eax, edi
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A3D
        and    eax, 0ffffffh
        cmp    dword ptr [ebx], eax
        ja     SHORT loc_1_1DF1
        mov    dword ptr [ebp - 4], 0
loc_1_1DF1:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_1E84
        test   byte ptr [ebx + 10h], 1
        je     SHORT loc_1_1E08
        add    eax, 0c00h
loc_1_1E08:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0ch], ecx
        jae    SHORT loc_1_1E15
        lea    eax, [eax + ecx*2]
        jmp    SHORT loc_1_1E1B
loc_1_1E15:
        mov    edx, dword ptr [ebp - 0ch]
        lea    eax, [eax + edx*2]
loc_1_1E1B:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 0ch], ecx
        jb     SHORT loc_1_1E35
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 8], edi
        jmp    SHORT loc_1_1E35
loc_1_1E2E:
        mov    dword ptr [ebp - 0ch], 0fffffh
loc_1_1E35:
        mov    eax, edi
        sub    ebx, 20h
        dec    edi
        test   eax, eax
        jne    NEAR PTR loc_1_1D40
        cmp    dword ptr [ebp - 8], 0ffh
        je     SHORT loc_1_1E7A
        cmp    dword ptr [ebp - 14h], 0fffffh
        ja     SHORT loc_1_1E7A
loc_1_1E55:
        mov    ebx, 0ffh
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    edi, 2a0h
        or     eax, edi
        push   eax
        push   esi
        call   sub_1_19AA
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        or     eax, edi
        push   eax
        push   esi
        call   sub_1_19AA
loc_1_1E7A:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1E84:
        mov    dword ptr [ebp - 8], edi
        jmp    SHORT loc_1_1E55
sub_1_1E89:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 18h], 2
        ja     SHORT loc_1_1EAC
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_1EAC
        mov    dword ptr [ebp - 18h], 0
        jmp    SHORT loc_1_1EB3
loc_1_1EAC:
        mov    dword ptr [ebp - 18h], 1
loc_1_1EB3:
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 10h]
        shr    eax, 8
        and    edi, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        movzx  eax, word ptr [esi + 6]
        cmp    eax, dword ptr [ebp - 0ch]
        jae    SHORT loc_1_1ED1
        mov    dword ptr [ebp - 0ch], eax
loc_1_1ED1:
        mov    dword ptr [ebp - 14h], 0ffffffffh
        mov    dword ptr [ebp - 8], 0ffh
        cmp    dword ptr [ebp - 0ch], edi
        jbe    NEAR PTR loc_1_1FA5
        mov    eax, edi
        shl    eax, 5
        lea    ebx, [eax + esi + 4ch]
loc_1_1EF1:
        cmp    dword ptr [ebp - 18h], 0
        jne    SHORT loc_1_1F01
        cmp    byte ptr [ebx + 0dh], 0
        jne    NEAR PTR loc_1_1F98
loc_1_1F01:
        movzx  eax, word ptr [ebx - 0ah]
        mov    dword ptr [ebp - 10h], eax
        cmp    eax, 2000h
        jae    SHORT loc_1_1F7E
        mov    eax, edi
        or     al, 60h
        push   eax
        push   esi
        call   sub_1_1A3D
        shr    eax, 10h
        cmp    dword ptr [ebp - 10h], 0ffeh
        mov    dword ptr [ebp - 4], eax
        je     SHORT loc_1_1F6A
        test   byte ptr [ebp - 0fh], 10h
        je     SHORT loc_1_1F38
        add    dword ptr [ebp - 4], 1200h
        jmp    SHORT loc_1_1F3F
loc_1_1F38:
        add    dword ptr [ebp - 4], 200h
loc_1_1F3F:
        cmp    byte ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_1F4E
        add    dword ptr [ebp - 4], 300h
        jmp    SHORT loc_1_1F70
loc_1_1F4E:
        mov    eax, edi
        or     eax, 2a0h
        push   eax
        push   esi
        call   sub_1_19DF
        test   ah, 80h
        jne    SHORT loc_1_1F70
        add    dword ptr [ebp - 4], 1300h
        jmp    SHORT loc_1_1F70
loc_1_1F6A:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1FCC
loc_1_1F70:
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 14h]
        ja     SHORT loc_1_1F7E
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 8], edi
loc_1_1F7E:
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_1F98
        mov    eax, edi
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A3D
        and    eax, 0ffffffh
        cmp    dword ptr [ebx], eax
        jbe    SHORT loc_1_1FD1
loc_1_1F98:
        add    ebx, 20h
        inc    edi
        cmp    dword ptr [ebp - 0ch], edi
        ja     NEAR PTR loc_1_1EF1
loc_1_1FA5:
        cmp    dword ptr [ebp - 8], 0ffh
        je     SHORT loc_1_1FC2
loc_1_1FAE:
        push   0ffh
        mov    eax, dword ptr [ebp - 8]
        or     eax, 2a0h
        push   eax
        push   esi
        call   sub_1_19AA
loc_1_1FC2:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1FCC:
        mov    dword ptr [ebp - 8], edi
        jmp    SHORT loc_1_1FAE
loc_1_1FD1:
        mov    dword ptr [ebp - 8], edi
        jmp    SHORT loc_1_1FAE
sub_1_1FD6:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 23ch
        cmp    ecx, 0ffffh
        push   ebx
        push   esi
        push   edi
        jne    SHORT loc_1_1FF5
        xor    eax, eax
        jmp    NEAR PTR loc_1_2350
loc_1_1FF5:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + ecx*4 + 662h]
        test   edx, edx
        jne    SHORT loc_1_200A
        xor    eax, eax
        jmp    NEAR PTR loc_1_2350
loc_1_200A:
        mov    eax, dword ptr [ebp + 10h]
        imul   eax, eax, 1ah
        add    eax, dword ptr [edx]
        cmp    word ptr [eax], 0
        mov    dword ptr [ebp - 1ch], eax
        jne    SHORT loc_1_2022
        xor    eax, eax
        jmp    NEAR PTR loc_1_2350
loc_1_2022:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    bx, word ptr [esi + 4]
        mov    dword ptr [ebp - 14h], ecx
        add    dword ptr [ebp - 1ch], 1ah
        mov    word ptr [ebp - 2], bx
        mov    esi, dword ptr [ebp - 1ch]
        movzx  ecx, bx
        shl    ecx, 2
        add    ecx, dword ptr [edx + 4]
        cmp    word ptr [esi + 4], bx
        mov    dword ptr [ebp - 20h], ecx
        jbe    NEAR PTR loc_1_2350
        mov    ebx, dword ptr [ebp + 1ch]
loc_1_2058:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 20h]
        mov    di, word ptr [esi]
        mov    dword ptr [ebp - 3ch], ecx
        add    dword ptr [ebp - 20h], 4
        mov    word ptr [ebp - 4], di
        mov    esi, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 38h], 0
        movzx  ecx, di
        imul   ecx, ecx, 3
        add    ecx, dword ptr [edx + 8]
        cmp    word ptr [esi], di
        mov    dword ptr [ebp - 2ch], ecx
        jbe    NEAR PTR loc_1_233B
loc_1_208B:
        mov    ecx, dword ptr [ebp - 2ch]
        mov    di, word ptr [ecx + 1]
        movzx  si, byte ptr [ecx]
        cmp    si, 2bh
        jne    SHORT loc_1_20C5
        xor    ecx, ecx
        mov    cx, di
        and    ecx, 0ffh
        cmp    ecx, dword ptr [ebp - 14h]
        ja     NEAR PTR loc_1_233B
        sar    di, 8
        movsx  ecx, di
        cmp    ecx, dword ptr [ebp - 14h]
        jae    NEAR PTR loc_1_2323
        jmp    NEAR PTR loc_1_233B
loc_1_20C5:
        cmp    si, 2eh
        jne    NEAR PTR loc_1_21C1
        movsx  ecx, di
        mov    dword ptr [ebp - 14h], ecx
loc_1_20D5:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ebp + ecx*4 - 23ch], si
        inc    dword ptr [ebp - 0ch]
        mov    word ptr [ebp + ecx*4 - 23ah], di
        cmp    dword ptr [ebp - 0ch], 80h
        je     NEAR PTR loc_1_2350
loc_1_20F8:
        cmp    dword ptr [ebp - 38h], 0
        jne    NEAR PTR loc_1_233B
        cmp    si, 29h
        jne    NEAR PTR loc_1_2323
        movsx  ecx, di
        add    ecx, ecx
        add    ecx, dword ptr [edx + 0ch]
        mov    di, word ptr [ecx]
        mov    dword ptr [ebp - 24h], ecx
        add    dword ptr [ebp - 24h], 2
        movzx  ecx, di
        mov    word ptr [ebp - 6], di
        mov    esi, dword ptr [ebp - 24h]
        shl    ecx, 2
        add    ecx, dword ptr [edx + 10h]
        cmp    word ptr [esi], di
        mov    dword ptr [ebp - 28h], ecx
        jbe    NEAR PTR loc_1_231D
loc_1_213A:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 28h]
        mov    di, word ptr [esi]
        mov    dword ptr [ebp - 34h], ecx
        add    dword ptr [ebp - 28h], 4
        mov    word ptr [ebp - 0eh], di
        mov    esi, dword ptr [ebp - 28h]
        movzx  ecx, di
        mov    di, word ptr [esi]
        imul   ecx, ecx, 3
        add    ecx, dword ptr [edx + 14h]
        cmp    word ptr [ebp - 0eh], di
        mov    dword ptr [ebp - 30h], ecx
        jae    NEAR PTR loc_1_2309
        mov    ecx, dword ptr [ebp - 0ch]
        lea    esi, [ebp + ecx*4 - 23ch]
        mov    dword ptr [ebp - 18h], esi
loc_1_2177:
        mov    ecx, dword ptr [ebp - 30h]
        mov    di, word ptr [ecx + 1]
        movzx  si, byte ptr [ecx]
        cmp    si, 29h
        je     NEAR PTR loc_1_20F8
        cmp    si, 35h
        je     NEAR PTR loc_1_2265
        cmp    si, 2bh
        jne    SHORT loc_1_21F4
        xor    ecx, ecx
        mov    cx, di
        and    ecx, 0ffh
        cmp    ecx, dword ptr [ebp - 14h]
        ja     NEAR PTR loc_1_2309
        sar    di, 8
        movsx  ecx, di
        cmp    ecx, dword ptr [ebp - 14h]
        jae    SHORT loc_1_221D
        jmp    NEAR PTR loc_1_2309
loc_1_21C1:
        cmp    si, 2ch
        jne    NEAR PTR loc_1_20D5
        xor    ecx, ecx
        mov    cx, di
        and    ecx, 0ffh
        cmp    ecx, dword ptr [ebp + 18h]
        ja     NEAR PTR loc_1_233B
        sar    di, 8
        movsx  ecx, di
        cmp    ecx, dword ptr [ebp + 18h]
        jae    NEAR PTR loc_1_2323
        jmp    NEAR PTR loc_1_233B
loc_1_21F4:
        cmp    si, 2eh
        jne    SHORT loc_1_223A
        movsx  ecx, di
        mov    dword ptr [ebp - 14h], ecx
loc_1_2200:
        mov    ecx, dword ptr [ebp - 18h]
        inc    dword ptr [ebp - 0ch]
        mov    word ptr [ecx], si
        lea    esi, [ebp - 3ch]
        add    dword ptr [ebp - 18h], 4
        mov    word ptr [ecx + 2], di
        cmp    dword ptr [ebp - 18h], esi
        je     NEAR PTR loc_1_2350
loc_1_221D:
        inc    word ptr [ebp - 0eh]
        mov    ecx, dword ptr [ebp - 28h]
        add    dword ptr [ebp - 30h], 3
        mov    cx, word ptr [ecx]
        cmp    word ptr [ebp - 0eh], cx
        jb     NEAR PTR loc_1_2177
        jmp    NEAR PTR loc_1_2309
loc_1_223A:
        cmp    si, 2ch
        jne    SHORT loc_1_2200
        xor    ecx, ecx
        mov    cx, di
        and    ecx, 0ffh
        cmp    ecx, dword ptr [ebp + 18h]
        ja     NEAR PTR loc_1_2309
        sar    di, 8
        movsx  ecx, di
        cmp    ecx, dword ptr [ebp + 18h]
        jae    SHORT loc_1_221D
        jmp    NEAR PTR loc_1_2309
loc_1_2265:
        mov    dword ptr [ebp - 38h], 1
        movsx  ecx, di
        imul   ecx, ecx, 24h
        add    ecx, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 10h], ecx
        test   byte ptr [ecx + 3], 80h
        jne    NEAR PTR loc_1_2303
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, ebx
        mov    ecx, 25h
        rep movsd
        mov    cx, word ptr [ebp + 14h]
        mov    si, word ptr [ebp + 18h]
        cmp    dword ptr [ebp - 0ch], 0
        mov    word ptr [ebx + 5ch], cx
        mov    word ptr [ebx + 5eh], si
        je     SHORT loc_1_22CB
        lea    esi, [ebp - 23ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 0ch], ecx
loc_1_22B2:
        cmp    word ptr [esi], 3ah
        jg     SHORT loc_1_22C3
        mov    cx, word ptr [esi + 2]
        movsx  edi, word ptr [esi]
        mov    word ptr [ebx + edi*2], cx
loc_1_22C3:
        add    esi, 4
        dec    dword ptr [ebp - 0ch]
        jne    SHORT loc_1_22B2
loc_1_22CB:
        mov    esi, dword ptr [ebp - 10h]
        movsx  ecx, word ptr [ebx]
        add    ecx, dword ptr [esi]
        mov    dword ptr [ebx + 76h], ecx
        movsx  ecx, word ptr [ebx + 2]
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebx + 7ah], ecx
        inc    eax
        movsx  ecx, word ptr [ebx + 4]
        add    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebx + 7eh], ecx
        movsx  ecx, word ptr [ebx + 6]
        add    ecx, dword ptr [esi + 0ch]
        cmp    eax, 6
        mov    dword ptr [ebx + 82h], ecx
        je     SHORT loc_1_2350
        add    ebx, 94h
loc_1_2303:
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 0ch], ecx
loc_1_2309:
        inc    word ptr [ebp - 6]
        mov    esi, dword ptr [ebp - 24h]
        mov    cx, word ptr [ebp - 6]
        cmp    word ptr [esi], cx
        ja     NEAR PTR loc_1_213A
loc_1_231D:
        mov    ecx, dword ptr [ebp - 3ch]
        mov    dword ptr [ebp - 0ch], ecx
loc_1_2323:
        inc    word ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 20h]
        mov    cx, word ptr [ebp - 4]
        add    dword ptr [ebp - 2ch], 3
        cmp    word ptr [esi], cx
        ja     NEAR PTR loc_1_208B
loc_1_233B:
        inc    word ptr [ebp - 2]
        mov    esi, dword ptr [ebp - 1ch]
        mov    cx, word ptr [ebp - 2]
        cmp    word ptr [esi + 4], cx
        ja     NEAR PTR loc_1_2058
loc_1_2350:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
sub_1_2359:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 2d8h
        cmp    ecx, 0ffffh
        push   ebx
        push   esi
        push   edi
        jne    SHORT loc_1_2378
        xor    eax, eax
        jmp    NEAR PTR loc_1_27B3
loc_1_2378:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx*4 + 662h]
        test   eax, eax
        jne    SHORT loc_1_238D
        xor    eax, eax
        jmp    NEAR PTR loc_1_27B3
loc_1_238D:
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 14h]
        imul   eax, eax, 1ah
        add    eax, dword ptr [ecx]
        cmp    word ptr [eax], 0
        mov    dword ptr [ebp - 1ch], eax
        jne    SHORT loc_1_23AB
        xor    eax, eax
        jmp    NEAR PTR loc_1_27B3
loc_1_23AB:
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 8], 0
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ax, word ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 14h]
        add    dword ptr [ebp - 1ch], 1ah
        mov    word ptr [ebp - 2], ax
        movzx  eax, ax
        shl    eax, 2
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dx, word ptr [ebp - 2]
        cmp    word ptr [ecx + 4], dx
        mov    dword ptr [ebp - 28h], eax
        jbe    NEAR PTR loc_1_27B0
        mov    edx, dword ptr [ebp + 1ch]
loc_1_23EB:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 28h]
        mov    si, word ptr [eax]
        mov    dword ptr [ebp - 38h], ecx
        add    dword ptr [ebp - 28h], 4
        movzx  eax, si
        mov    ecx, dword ptr [ebp - 14h]
        imul   eax, eax, 3
        add    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 28h]
        cmp    word ptr [eax], si
        jbe    NEAR PTR loc_1_279B
        mov    ecx, dword ptr [ebp - 8]
        lea    ecx, [ebp + ecx*4 - 2d8h]
loc_1_2420:
        mov    eax, dword ptr [ebp - 0ch]
        mov    di, word ptr [eax + 1]
        movzx  bx, byte ptr [eax]
        cmp    bx, 2bh
        jne    SHORT loc_1_2455
        xor    eax, eax
        mov    ax, di
        and    eax, 0ffh
        cmp    eax, dword ptr [ebp + 14h]
        ja     NEAR PTR loc_1_279B
        sar    di, 8
        movsx  eax, di
        cmp    eax, dword ptr [ebp + 14h]
        jae    SHORT loc_1_24A0
        jmp    NEAR PTR loc_1_279B
loc_1_2455:
        cmp    bx, 2ch
        jne    SHORT loc_1_247F
        xor    eax, eax
        mov    ax, di
        and    eax, 0ffh
        cmp    eax, dword ptr [ebp + 18h]
        ja     NEAR PTR loc_1_279B
        sar    di, 8
        movsx  eax, di
        cmp    eax, dword ptr [ebp + 18h]
        jae    SHORT loc_1_24A0
        jmp    NEAR PTR loc_1_279B
loc_1_247F:
        mov    word ptr [ecx], bx
        lea    eax, [ebp - 0d8h]
        mov    word ptr [ecx + 2], di
        add    ecx, 4
        inc    dword ptr [ebp - 8]
        cmp    ecx, eax
        je     NEAR PTR loc_1_27B0
        cmp    bx, 29h
        je     SHORT loc_1_24B7
loc_1_24A0:
        inc    si
        mov    eax, dword ptr [ebp - 28h]
        add    dword ptr [ebp - 0ch], 3
        cmp    word ptr [eax], si
        ja     NEAR PTR loc_1_2420
        jmp    NEAR PTR loc_1_279B
loc_1_24B7:
        movsx  eax, di
        mov    ecx, dword ptr [ebp - 8]
        add    eax, eax
        mov    dword ptr [ebp - 30h], ecx
        mov    ecx, dword ptr [ebp - 14h]
        add    eax, dword ptr [ecx + 0ch]
        mov    si, word ptr [eax]
        mov    dword ptr [ebp - 20h], eax
        add    dword ptr [ebp - 20h], 2
        movzx  eax, si
        mov    word ptr [ebp - 16h], si
        mov    ebx, dword ptr [ebp - 20h]
        shl    eax, 2
        add    eax, dword ptr [ecx + 10h]
        cmp    word ptr [ebx], si
        mov    dword ptr [ebp - 24h], eax
        jbe    NEAR PTR loc_1_2795
        xor    eax, eax
loc_1_24F0:
        mov    ecx, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp - 24h]
        add    dword ptr [ebp - 24h], 4
        mov    si, word ptr [ebx]
        mov    dword ptr [ebp - 34h], ecx
        mov    ecx, dword ptr [ebp - 14h]
        mov    ebx, dword ptr [ebp - 24h]
        movzx  edi, si
        imul   edi, edi, 3
        add    edi, dword ptr [ecx + 14h]
        cmp    word ptr [ebx], si
        mov    dword ptr [ebp - 10h], edi
        jbe    NEAR PTR loc_1_2781
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [ebp + ecx*4 - 2d8h]
loc_1_2525:
        mov    ecx, dword ptr [ebp - 10h]
        movzx  bx, byte ptr [ecx]
        mov    cx, word ptr [ecx + 1]
        mov    word ptr [ebp - 0ah], cx
        cmp    bx, 35h
        je     NEAR PTR loc_1_25D4
        cmp    bx, 2bh
        jne    SHORT loc_1_256E
        xor    ecx, ecx
        mov    cx, word ptr [ebp - 0ah]
        and    ecx, 0ffh
        cmp    ecx, dword ptr [ebp + 14h]
        ja     NEAR PTR loc_1_2781
        mov    cx, word ptr [ebp - 0ah]
        sar    cx, 8
        movsx  ebx, cx
        cmp    ebx, dword ptr [ebp + 14h]
        jae    SHORT loc_1_25BD
        jmp    NEAR PTR loc_1_2781
loc_1_256E:
        cmp    bx, 2ch
        jne    SHORT loc_1_259E
        xor    ecx, ecx
        mov    cx, word ptr [ebp - 0ah]
        and    ecx, 0ffh
        cmp    ecx, dword ptr [ebp + 18h]
        ja     NEAR PTR loc_1_2781
        mov    cx, word ptr [ebp - 0ah]
        sar    cx, 8
        movsx  ebx, cx
        cmp    ebx, dword ptr [ebp + 18h]
        jae    SHORT loc_1_25BD
        jmp    NEAR PTR loc_1_2781
loc_1_259E:
        mov    word ptr [edi], bx
        add    edi, 4
        mov    cx, word ptr [ebp - 0ah]
        inc    dword ptr [ebp - 8]
        mov    word ptr [edi - 2], cx
        lea    ebx, [ebp - 0d8h]
        cmp    edi, ebx
        je     NEAR PTR loc_1_27B0
loc_1_25BD:
        inc    si
        mov    ecx, dword ptr [ebp - 24h]
        add    dword ptr [ebp - 10h], 3
        cmp    word ptr [ecx], si
        ja     NEAR PTR loc_1_2525
        jmp    NEAR PTR loc_1_2781
loc_1_25D4:
        movsx  ebx, word ptr [ebp - 0ah]
        mov    ecx, dword ptr [ebp - 14h]
        imul   ebx, ebx, 24h
        add    ebx, dword ptr [ecx + 18h]
        test   byte ptr [ebx + 3], 80h
        jne    NEAR PTR loc_1_277B
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, edx
        mov    ecx, 25h
        rep movsd
        lea    edi, [ebp - 0d8h]
        mov    ecx, 25h
        rep stosd
        mov    cx, word ptr [ebp + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    word ptr [edx + 5ch], cx
        mov    si, word ptr [ebp + 18h]
        cmp    dword ptr [ebp - 30h], eax
        mov    word ptr [edx + 5eh], si
        je     SHORT loc_1_264A
        lea    esi, [ebp - 2d8h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ebp - 3ch], ecx
        mov    dword ptr [ebp - 10h], ecx
loc_1_262D:
        cmp    word ptr [esi], 3ah
        jg     SHORT loc_1_2642
        mov    cx, word ptr [esi + 2]
        movsx  edi, word ptr [esi]
        mov    word ptr [ebp + edi*2 - 0d8h], cx
loc_1_2642:
        add    esi, 4
        dec    dword ptr [ebp - 3ch]
        jne    SHORT loc_1_262D
loc_1_264A:
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 8], ecx
        jbe    SHORT loc_1_267E
        lea    esi, [ebp + ecx*4 - 2d8h]
        mov    ecx, dword ptr [ebp - 8]
        sub    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], ecx
loc_1_2662:
        mov    cx, word ptr [esi]
        cmp    cx, 3ah
        jg     SHORT loc_1_2676
        mov    di, word ptr [esi + 2]
        movsx  ecx, cx
        mov    word ptr [edx + ecx*2], di
loc_1_2676:
        add    esi, 4
        dec    dword ptr [ebp - 8]
        jne    SHORT loc_1_2662
loc_1_267E:
        mov    cx, word ptr [ebp - 0ah]
        mov    esi, edx
        mov    word ptr [ebp - 7ch], ax
        mov    word ptr [ebp - 7ah], ax
        mov    word ptr [ebp - 6eh], cx
        lea    ecx, [ebp - 0d8h]
        mov    dword ptr [ebp - 0ch], ecx
loc_1_2699:
        movsx  ecx, word ptr [esi]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp - 0ch]
        movsx  edi, word ptr [ecx]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, edi
        cmp    ecx, 0ffff8000h
        jge    SHORT loc_1_26B9
        mov    word ptr [esi], 8000h
        jmp    SHORT loc_1_26C9
loc_1_26B9:
        mov    word ptr [esi], 7fffh
        cmp    ecx, 7fffh
        jg     SHORT loc_1_26C9
        mov    word ptr [esi], cx
loc_1_26C9:
        add    esi, 2
        lea    ecx, [ebp - 64h]
        add    dword ptr [ebp - 0ch], 2
        cmp    dword ptr [ebp - 0ch], ecx
        jbe    SHORT loc_1_2699
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [edx + 90h], ecx
        movsx  ecx, word ptr [edx]
        add    ecx, dword ptr [ebx]
        mov    dword ptr [edx + 76h], ecx
        movsx  esi, word ptr [edx + 8]
        shl    esi, 0fh
        add    esi, ecx
        mov    dword ptr [edx + 76h], esi
        movsx  ecx, word ptr [edx + 2]
        add    ecx, dword ptr [ebx + 4]
        mov    dword ptr [edx + 7ah], ecx
        movsx  esi, word ptr [edx + 18h]
        shl    esi, 0fh
        add    esi, ecx
        mov    dword ptr [edx + 7ah], esi
        movsx  ecx, word ptr [edx + 4]
        add    ecx, dword ptr [ebx + 8]
        mov    dword ptr [edx + 7eh], ecx
        movsx  esi, word ptr [edx + 5ah]
        shl    esi, 0fh
        add    esi, ecx
        mov    dword ptr [edx + 7eh], esi
        lea    esi, [edx + 82h]
        movsx  ecx, word ptr [edx + 6]
        add    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [esi], ecx
        movsx  edi, word ptr [edx + 64h]
        shl    edi, 0fh
        add    edi, ecx
        mov    dword ptr [esi], edi
        mov    ecx, dword ptr [ebx + 16h]
        mov    dword ptr [edx + 86h], ecx
        mov    cl, byte ptr [ebx + 14h]
        mov    byte ptr [edx + 8ah], cl
        mov    cl, byte ptr [ebx + 15h]
        mov    byte ptr [edx + 8bh], cl
        inc    dword ptr [ebp - 2ch]
        mov    si, word ptr [ebx + 12h]
        mov    word ptr [edx + 8ch], si
        cmp    dword ptr [ebp - 2ch], 6
        mov    bx, word ptr [ebx + 10h]
        mov    word ptr [edx + 8eh], bx
        je     SHORT loc_1_27B0
        add    edx, 94h
loc_1_277B:
        mov    ecx, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 8], ecx
loc_1_2781:
        inc    word ptr [ebp - 16h]
        mov    ecx, dword ptr [ebp - 20h]
        mov    si, word ptr [ebp - 16h]
        cmp    word ptr [ecx], si
        ja     NEAR PTR loc_1_24F0
loc_1_2795:
        mov    ecx, dword ptr [ebp - 38h]
        mov    dword ptr [ebp - 8], ecx
loc_1_279B:
        inc    word ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ax, word ptr [ebp - 2]
        cmp    word ptr [ecx + 4], ax
        ja     NEAR PTR loc_1_23EB
loc_1_27B0:
        mov    eax, dword ptr [ebp - 2ch]
loc_1_27B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
sub_1_27BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 1ch]
        cmp    word ptr [edi + 72h], 0
        je     SHORT loc_1_281F
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        add    esi, 42h
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 6], bx
        jbe    SHORT loc_1_281F
loc_1_27E0:
        movzx  eax, byte ptr [esi + 13h]
        movsx  ecx, word ptr [edi + 72h]
        cmp    eax, ecx
        jne    SHORT loc_1_2810
        push   807fh
        mov    eax, ebx
        or     eax, 2a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    word ptr [esi], 0ffeh
        mov    word ptr [esi + 1ah], 0
        mov    byte ptr [esi + 13h], 0
loc_1_2810:
        add    esi, 20h
        inc    ebx
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, word ptr [ecx + 6]
        cmp    eax, ebx
        ja     SHORT loc_1_27E0
loc_1_281F:
        mov    ebx, edi
        mov    eax, dword ptr [ebp + 0ch]
        shl    eax, 5
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 18h], 0
        lea    edx, [eax + ecx + 442h]
        mov    dword ptr [ebp - 0ch], edx
        je     NEAR PTR loc_1_2AD4
loc_1_2845:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0ae2h]
        test   edx, edx
        je     SHORT loc_1_2871
        mov    eax, dword ptr [ebp + 0ch]
        imul   eax, eax, 5ch
        cmp    word ptr [eax + ecx + 0eeeh], 0
        je     SHORT loc_1_2871
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        or     ah, 80h
        push   eax
        push   ecx
        call   edx
        add    esp, 0ch
loc_1_2871:
        push   0
        push   0feh
        push   0ffh
        push   dword ptr [ebp + 8]
        call   sub_1_1A68
        add    esp, 10h
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, 0ffh
        mov    byte ptr [ebp + ecx - 3ch], al
        je     NEAR PTR loc_1_2ABF
        shl    eax, 5
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 76h], 200000h
        lea    esi, [eax + ecx + 42h]
        jb     SHORT loc_1_28BC
        cmp    byte ptr [esi + 17h], 0
        je     SHORT loc_1_28CC
        dec    dword ptr [ecx + 18h]
        mov    byte ptr [esi + 17h], 0
        jmp    SHORT loc_1_28CC
loc_1_28BC:
        cmp    byte ptr [esi + 17h], 0
        jne    SHORT loc_1_28CC
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [ecx + 18h]
        mov    byte ptr [esi + 17h], 1
loc_1_28CC:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 9]
        cmp    al, 0ah
        jbe    NEAR PTR loc_1_296E
        mov    dword ptr [ebp - 4], 18h
        movsx  edx, word ptr [ebx + 5eh]
        movzx  eax, al
        db  00Fh, 0B6h, 08Ah, 000h, 000h, 000h, 000h
        db  00Fh, 0B6h, 090h, 000h, 000h, 000h, 000h
        movsx  eax, word ptr [ebx + 60h]
        add    ecx, edx
        add    eax, 0ch
        cdq
        idiv   dword ptr [ebp - 4]
        add    ecx, eax
        sub    edx, edx
        lea    eax, [ecx*8]
        mov    ecx, 3
        div    ecx
        mov    edx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [edx + 10h]
        lea    edx, [ecx + eax]
        cmp    edx, 0ffh
        ja     SHORT loc_1_296E
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 8]
        cmp    al, 7fh
        jae    SHORT loc_1_294E
        movzx  ecx, al
        db  00Fh, 0B6h, 081h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 24h], eax
        mov    eax, 100h
        sub    eax, edx
        imul   eax, dword ptr [ebp - 24h]
        shr    eax, 7
        add    edx, eax
loc_1_294E:
        cmp    byte ptr [esi + 17h], 0
        je     SHORT loc_1_2973
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 151eh], 4d474d31h
        jne    SHORT loc_1_2973
        add    edx, 10h
        cmp    edx, 0ffh
        jbe    SHORT loc_1_2973
loc_1_296E:
        mov    edx, 0ffh
loc_1_2973:
        mov    word ptr [ebx + 26h], dx
        mov    eax, dword ptr [ebp - 0ch]
        cmp    word ptr [eax + 2], 80h
        jae    SHORT loc_1_29AB
        cmp    word ptr [ebx + 44h], 7dh
        jge    SHORT loc_1_29AB
        movsx  eax, word ptr [ebx + 5eh]
        cmp    eax, 46h
        jae    SHORT loc_1_2997
        mov    eax, 46h
loc_1_2997:
        movsx  ecx, word ptr [ebx + 10h]
        imul   ecx, eax
        add    ecx, 0a0h
        shr    ecx, 7
        mov    word ptr [ebx + 10h], cx
loc_1_29AB:
        mov    eax, dword ptr [ebp - 0ch]
        movzx  ecx, word ptr [ebx + 48h]
        cmp    word ptr [eax + 2], 80h
        jae    SHORT loc_1_29FC
        mov    dx, 3ch
        lea    eax, [ebx + 46h]
        sub    dx, word ptr [ebx + 5ch]
        imul   dx, word ptr [ebx + 4eh]
        add    dx, word ptr [eax]
        mov    word ptr [eax], dx
        jns    SHORT loc_1_29D7
        mov    word ptr [eax], 0
loc_1_29D7:
        movsx  edx, word ptr [ebx + 5ch]
        mov    eax, 3ch
        sub    eax, edx
        movsx  edx, word ptr [ebx + 50h]
        imul   eax, edx
        add    ecx, eax
        jns    SHORT loc_1_29F1
        xor    ecx, ecx
        jmp    SHORT loc_1_29FC
loc_1_29F1:
        mov    eax, 0ffffh
        cmp    ecx, eax
        jbe    SHORT loc_1_29FC
        mov    ecx, eax
loc_1_29FC:
        mov    ax, word ptr [ebx + 46h]
        cmp    ax, 2da4h
        jle    SHORT loc_1_2A0E
        mov    word ptr [ebx + 46h], 0
        jmp    SHORT loc_1_2A22
loc_1_2A0E:
        mov    word ptr [ebp - 4], 0ffa4h
        cwd
        idiv   word ptr [ebp - 4]
        add    ax, 7fh
        mov    word ptr [ebx + 46h], ax
loc_1_2A22:
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2A28:
        mov    word ptr [ebx + 48h], ax
        add    esp, 4
        movsx  ecx, word ptr [ebx + 5ch]
        movsx  eax, word ptr [ebx + 74h]
        sub    ecx, eax
        movsx  edx, word ptr [ebx + 66h]
        add    ecx, edx
        movsx  eax, word ptr [ebx + 6eh]
        movsx  edx, word ptr [ebx + 68h]
        add    ecx, 3ch
        imul   ecx, ecx, 64h
        sub    ecx, eax
        add    ecx, edx
        cmp    word ptr [ebx + 70h], 1
        mov    dword ptr [ebp - 24h], ecx
        jne    SHORT loc_1_2A65
        mov    eax, ecx
        cdq
        sub    eax, edx
        sar    eax, 1
        mov    dword ptr [ebp - 24h], eax
loc_1_2A65:
        push   dword ptr [ebp - 24h]
        call   sub_1_2FB3
        mov    word ptr [esi + 0eh], ax
        mov    ax, word ptr [ebx + 60h]
        push   dword ptr [ebp - 0ch]
        push   ebx
        mov    word ptr [esi + 4], ax
        mov    cx, word ptr [ebx + 5eh]
        mov    word ptr [esi + 2], cx
        mov    dl, byte ptr [ebx + 0ah]
        mov    byte ptr [esi + 14h], dl
        mov    al, byte ptr [ebx + 3ch]
        mov    byte ptr [esi + 15h], al
        mov    cl, byte ptr [ebx + 4ch]
        mov    byte ptr [esi + 16h], cl
        mov    dl, byte ptr [ebx + 22h]
        mov    byte ptr [esi + 12h], dl
        mov    edx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 72h]
        mov    byte ptr [esi + 13h], al
        movzx  eax, word ptr [edx + 12h]
        push   eax
        call   sub_1_4FCB
        add    esp, 0ch
        mov    cl, byte ptr [ebx + 22h]
        mov    byte ptr [esi + 18h], cl
        mov    cl, byte ptr [ebx + 22h]
        mov    byte ptr [esi + 19h], cl
loc_1_2ABF:
        add    ebx, 94h
        inc    dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 18h], ecx
        ja     NEAR PTR loc_1_2845
loc_1_2AD4:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_1_2FA8
        mov    esi, 0ffffh
loc_1_2AEA:
        mov    ecx, dword ptr [ebp - 10h]
        movzx  eax, byte ptr [ebp + ecx - 3ch]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_2F93
        shl    eax, 5
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + ecx + 42h]
        mov    eax, dword ptr [ebp - 0ch]
        movsx  ecx, word ptr [eax + 0eh]
        movzx  eax, word ptr [edx + 0eh]
        add    ecx, eax
        mov    dword ptr [ebp - 14h], edx
        mov    eax, dword ptr [ebp - 0ch]
        add    ecx, dword ptr [eax + 10h]
        cmp    ecx, esi
        jbe    SHORT loc_1_2B29
        mov    dword ptr [ebp - 8], esi
        jmp    SHORT loc_1_2B37
loc_1_2B29:
        mov    eax, 0
        test   ecx, ecx
        jl     SHORT loc_1_2B34
        mov    eax, ecx
loc_1_2B34:
        mov    dword ptr [ebp - 8], eax
loc_1_2B37:
        mov    eax, dword ptr [edi + 76h]
        sub    eax, 4
        test   byte ptr [edi + 6ch], 1
        mov    dword ptr [ebp - 28h], eax
        je     SHORT loc_1_2B82
        mov    ecx, dword ptr [edi + 7eh]
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 18h], ecx
        lea    ecx, [edi + 82h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 0ah], 0
        inc    eax
        mov    ebx, dword ptr [edi + 7ah]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, ebx
        sub    eax, dword ptr [ecx]
        cmp    eax, 14h
        jae    SHORT loc_1_2B77
        mov    dword ptr [edx + 6], 0
        jmp    SHORT loc_1_2B9E
loc_1_2B77:
        add    ebx, 4
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [edx + 6], ebx
        jmp    SHORT loc_1_2B9E
loc_1_2B82:
        mov    eax, dword ptr [edi + 7ah]
        mov    edx, dword ptr [ebp - 14h]
        add    eax, 8
        mov    dword ptr [edx + 6], 0
        lea    ecx, [eax - 4]
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [edx + 0ah], ecx
loc_1_2B9E:
        mov    eax, dword ptr [ebp - 4]
        push   esi
        or     al, 60h
        push   eax
        mov    dword ptr [ebp - 34h], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    eax, dword ptr [ebp - 4]
        push   esi
        or     al, 40h
        push   eax
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [edi + 46h]
        movsx  ecx, word ptr [edi + 44h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 380h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 2ah]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        or     eax, 3a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [edi + 36h]
        movsx  ecx, word ptr [edi + 34h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 3c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 3ah]
        movsx  eax, word ptr [edi + 38h]
        shl    ecx, 8
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     eax, 2e0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 2eh]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        or     eax, 3e0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        or     ah, 4
        push   eax
        lea    ebx, [edi + 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [ebx]
        movsx  ecx, word ptr [edi + 26h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 420h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [edi + 0eh]
        movsx  ecx, word ptr [edi + 16h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 440h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    edx, dword ptr [ebp - 0ch]
        movzx  ecx, byte ptr [edx + 0ah]
        movzx  eax, byte ptr [edx + 0bh]
        add    ecx, eax
        movsx  eax, word ptr [edi + 0ah]
        add    eax, ecx
        cmp    eax, 7fh
        jle    SHORT loc_1_2CBA
        mov    eax, 7fh
loc_1_2CBA:
        movsx  ecx, word ptr [edi + 14h]
        shl    eax, 8
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     eax, 460h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [edi + 1ah]
        movsx  ecx, word ptr [edi + 2ch]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 480h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 0ch]
        movsx  eax, word ptr [edi + 30h]
        shl    ecx, 8
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     eax, 4a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        cmp    word ptr [edi + 34h], 7fh
        jne    SHORT loc_1_2D6C
        cmp    word ptr [edi + 32h], 8000h
        jb     SHORT loc_1_2D6C
        push   0bfffh
        mov    eax, dword ptr [ebp - 4]
        or     eax, 2c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 0eh]
        shl    ecx, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, ecx
        mov    ecx, esi
        cmp    eax, esi
        ja     SHORT loc_1_2D4C
        mov    ecx, eax
loc_1_2D4C:
        mov    dword ptr [ebp - 8], ecx
        movsx  eax, word ptr [ebx]
        movsx  ecx, word ptr [edi + 16h]
        add    eax, ecx
        jns    SHORT loc_1_2D5E
        xor    eax, eax
        jmp    SHORT loc_1_2D85
loc_1_2D5E:
        cmp    eax, 0ffh
        jle    SHORT loc_1_2D85
        mov    eax, 0ffh
        jmp    SHORT loc_1_2D85
loc_1_2D6C:
        movsx  eax, word ptr [edi + 32h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 2c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [ebx]
loc_1_2D85:
        shl    eax, 8
        cmp    dword ptr [ebp - 8], esi
        mov    dword ptr [ebp - 1ch], eax
        jne    SHORT loc_1_2D95
        mov    dword ptr [ebp - 24h], esi
        jmp    SHORT loc_1_2DFB
loc_1_2D95:
        mov    ebx, 1
        mov    ecx, dword ptr [ebp - 8]
        shr    ecx, 0ch
        shl    ebx, cl
        test   byte ptr [ebp - 7], 8
        je     SHORT loc_1_2DBA
        mov    eax, ebx
        mov    ecx, 2710h
        imul   eax, eax, 102eh
        cdq
        idiv   ecx
        add    ebx, eax
loc_1_2DBA:
        test   byte ptr [ebp - 7], 4
        je     SHORT loc_1_2DD2
        mov    eax, ebx
        mov    ecx, 2710h
        imul   eax, eax, 764h
        cdq
        idiv   ecx
        add    ebx, eax
loc_1_2DD2:
        test   byte ptr [ebp - 7], 2
        je     SHORT loc_1_2DEA
        mov    eax, ebx
        mov    ecx, 2710h
        imul   eax, eax, 389h
        cdq
        idiv   ecx
        add    ebx, eax
loc_1_2DEA:
        mov    eax, ebx
        mov    dword ptr [ebp - 24h], esi
        sar    eax, 2
        add    ebx, eax
        cmp    ebx, esi
        ja     SHORT loc_1_2DFB
        mov    dword ptr [ebp - 24h], ebx
loc_1_2DFB:
        cmp    word ptr [edi + 44h], 7fh
        jne    SHORT loc_1_2E55
        cmp    word ptr [edi + 42h], 8000h
        jb     SHORT loc_1_2E55
        push   0bfffh
        mov    eax, dword ptr [ebp - 4]
        or     eax, 280h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [edi + 26h]
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 0fh
        xor    eax, edx
        sub    eax, edx
        movsx  edx, ax
        movsx  eax, word ptr [edi + 26h]
        db  066h, 08Bh, 01Ch, 055h, 000h, 000h, 000h, 000h
        cdq
        and    edx, 0fh
        add    eax, edx
        sar    eax, 4
        mov    cl, al
        shr    bx, cl
        movzx  eax, bx
        mov    dword ptr [ebp - 20h], eax
        jmp    SHORT loc_1_2E72
loc_1_2E55:
        movsx  eax, word ptr [edi + 42h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 280h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    dword ptr [ebp - 20h], 0
loc_1_2E72:
        mov    eax, dword ptr [ebp - 4]
        push   0
        or     al, 20h
        push   eax
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [edi + 22h]
        shl    eax, 18h
        or     dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [ebp - 18h]
        or     al, 0c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [edi + 1eh]
        shl    eax, 18h
        or     dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [ebp - 2ch]
        or     al, 0e0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   dword ptr [ebp - 28h]
        mov    eax, dword ptr [ebp - 4]
        or     ah, 2
        push   eax
        mov    dword ptr [ebp - 18h], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        mov    eax, dword ptr [ebp - 4]
        or     al, 0a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        mov    eax, dword ptr [ebp - 4]
        or     al, 80h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [edi + 12h]
        shl    eax, 1ch
        or     dword ptr [ebp - 28h], eax
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    ebx, dword ptr [ebp - 20h]
        shl    ebx, 10h
        or     ebx, dword ptr [ebp - 1ch]
        push   ebx
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   ebx
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  ecx, word ptr [edi + 20h]
        movsx  eax, word ptr [edi + 24h]
        shl    ecx, 8
        mov    ebx, dword ptr [ebp - 24h]
        shl    ebx, 10h
        or     ecx, eax
        or     ecx, ebx
        push   ecx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   ebx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [edi + 4ah]
        movsx  ecx, word ptr [edi + 48h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 2a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 14h]
        shl    ax, 8
        or     ax, word ptr [ebp + 10h]
        mov    word ptr [edx], ax
loc_1_2F93:
        add    edi, 94h
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 18h], eax
        ja     NEAR PTR loc_1_2AEA
loc_1_2FA8:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
sub_1_2FB3:
        push   esi
        mov    ecx, 4b0h
        mov    esi, dword ptr [esp + 8]
        push   edi
        add    esi, 41a0h
        mov    eax, esi
        cdq
        idiv   ecx
        cmp    eax, 0fh
        mov    edi, eax
        jle    SHORT loc_1_2FD5
        mov    edi, 0fh
loc_1_2FD5:
        mov    ecx, 4b0h
        mov    eax, esi
        cdq
        idiv   ecx
        mov    eax, edx
        mov    ecx, edx
        shl    edi, 0ch
        mov    esi, 4bh
        imul   eax, eax, 1fh
        imul   ecx, ecx, 3
        sub    edx, edx
        div    esi
        add    eax, ecx
        or     eax, edi
        pop    edi
        pop    esi
        ret    4
sub_1_2FFE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 1ch]
        cmp    word ptr [ebx + 72h], 0
        je     SHORT loc_1_3073
        mov    edi, dword ptr [ebp + 8]
        xor    esi, esi
        add    edi, 42h
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 6], si
        jbe    SHORT loc_1_3073
loc_1_3022:
        mov    eax, dword ptr [edi + 1ch]
        cmp    dword ptr [ebx + 90h], eax
        jne    SHORT loc_1_3064
        movzx  eax, byte ptr [edi + 13h]
        movsx  ecx, word ptr [ebx + 72h]
        cmp    eax, ecx
        jne    SHORT loc_1_3064
        push   807fh
        mov    eax, esi
        or     eax, 2a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    word ptr [edi], 0ffeh
        mov    word ptr [edi + 1ah], 0
        mov    dword ptr [edi + 1ch], 0
        mov    byte ptr [edi + 13h], 0
loc_1_3064:
        add    edi, 20h
        inc    esi
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, word ptr [ecx + 6]
        cmp    eax, esi
        ja     SHORT loc_1_3022
loc_1_3073:
        mov    dword ptr [ebp - 18h], 0
        cmp    dword ptr [ebp + 18h], 0
        mov    ecx, ebx
        je     NEAR PTR loc_1_3125
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 8], eax
loc_1_308C:
        cmp    word ptr [ecx + 8ch], 2
        jne    SHORT loc_1_30EE
        xor    eax, eax
        cmp    dword ptr [ebp + 18h], eax
        je     SHORT loc_1_30EE
        lea    esi, [ebx + 6ah]
loc_1_30A0:
        movsx  edi, word ptr [esi]
        movzx  edx, word ptr [ecx + 8eh]
        cmp    edi, edx
        je     SHORT loc_1_30BC
        add    esi, 94h
        inc    eax
        cmp    eax, dword ptr [ebp + 18h]
        jb     SHORT loc_1_30A0
        jmp    SHORT loc_1_30EE
loc_1_30BC:
        imul   eax, eax, 94h
        mov    esi, dword ptr [ebp - 18h]
        lea    edx, [eax + ebx]
        mov    word ptr [ecx + 6ah], 0ffffh
        mov    word ptr [edx + 6ah], 0ffffh
        mov    dword ptr [ebp + esi*4 - 4ch], edx
        inc    dword ptr [ebp - 18h]
        mov    word ptr [edx + 8ch], 4
        mov    esi, dword ptr [ebp - 18h]
        inc    dword ptr [ebp - 18h]
        mov    dword ptr [ebp + esi*4 - 4ch], ecx
loc_1_30EE:
        add    ecx, 94h
        dec    dword ptr [ebp - 8]
        jne    SHORT loc_1_308C
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_1_3125
        mov    eax, dword ptr [ebp + 18h]
loc_1_3102:
        cmp    word ptr [ebx + 6ah], -1
        je     SHORT loc_1_311C
        mov    word ptr [ebx + 8ch], 1
        mov    ecx, dword ptr [ebp - 18h]
        inc    dword ptr [ebp - 18h]
        mov    dword ptr [ebp + ecx*4 - 4ch], ebx
loc_1_311C:
        add    ebx, 94h
        dec    eax
        jne    SHORT loc_1_3102
loc_1_3125:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        shl    eax, 5
        cmp    dword ptr [ebp + 18h], 0
        lea    edx, [eax + ecx + 442h]
        mov    dword ptr [ebp - 0ch], edx
        je     NEAR PTR loc_1_371E
        mov    ebx, 0ffh
loc_1_314E:
        mov    edx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + edx*4 - 4ch]
        mov    ax, word ptr [edi + 8ch]
        cmp    ax, 2
        jne    SHORT loc_1_3169
        movzx  eax, byte ptr [ebp + edx - 28h]
        jmp    SHORT loc_1_31C7
loc_1_3169:
        cmp    ax, 4
        jne    SHORT loc_1_31A5
        push   1
        push   0feh
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A68
        add    esp, 10h
        cmp    eax, ebx
        jne    SHORT loc_1_3195
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 8]
        mov    byte ptr [ebp + edx - 28h], bl
        jmp    NEAR PTR loc_1_370F
loc_1_3195:
        mov    edx, dword ptr [ebp - 8]
        lea    ecx, [eax]
        inc    eax
        mov    byte ptr [ebp + edx - 28h], al
        mov    byte ptr [ebp + edx - 27h], cl
        jmp    SHORT loc_1_31C7
loc_1_31A5:
        push   0
        push   0feh
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A68
        add    esp, 10h
        mov    edx, dword ptr [ebp - 8]
        cmp    eax, ebx
        mov    byte ptr [ebp + edx - 28h], al
        je     NEAR PTR loc_1_370F
loc_1_31C7:
        shl    eax, 5
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 76h], 200000h
        lea    esi, [eax + ecx + 42h]
        jb     SHORT loc_1_31E9
        cmp    byte ptr [esi + 17h], 0
        je     SHORT loc_1_31F9
        dec    dword ptr [ecx + 18h]
        mov    byte ptr [esi + 17h], 0
        jmp    SHORT loc_1_31F9
loc_1_31E9:
        cmp    byte ptr [esi + 17h], 0
        jne    SHORT loc_1_31F9
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [ecx + 18h]
        mov    byte ptr [esi + 17h], 1
loc_1_31F9:
        mov    ax, word ptr [edi + 0ah]
        cmp    ax, 0ff80h
        jge    SHORT loc_1_320B
        mov    word ptr [edi + 0ah], 0ff80h
        jmp    SHORT loc_1_3217
loc_1_320B:
        cmp    ax, 7fh
        jle    SHORT loc_1_3217
        mov    word ptr [edi + 0ah], 7fh
loc_1_3217:
        lea    eax, [edi + 0ch]
        mov    cx, word ptr [eax]
        cmp    cx, -80h
        jge    SHORT loc_1_322A
        mov    word ptr [eax], 0ff80h
        jmp    SHORT loc_1_3235
loc_1_322A:
        cmp    cx, 7fh
        jle    SHORT loc_1_3235
        mov    word ptr [eax], 7fh
loc_1_3235:
        lea    ecx, [edi + 0eh]
        mov    ax, word ptr [ecx]
        cmp    ax, 0ff80h
        jge    SHORT loc_1_3248
        mov    word ptr [ecx], 0ff80h
        jmp    SHORT loc_1_3253
loc_1_3248:
        cmp    ax, 7fh
        jle    SHORT loc_1_3253
        mov    word ptr [ecx], 7fh
loc_1_3253:
        mov    ax, word ptr [edi + 10h]
        test   ax, ax
        jge    SHORT loc_1_3264
        mov    word ptr [edi + 10h], 0
        jmp    SHORT loc_1_326D
loc_1_3264:
        cmp    ax, bx
        jle    SHORT loc_1_326D
        mov    word ptr [edi + 10h], bx
loc_1_326D:
        lea    eax, [edi + 12h]
        mov    cx, word ptr [eax]
        test   cx, cx
        jge    SHORT loc_1_327F
        mov    word ptr [eax], 0
        jmp    SHORT loc_1_328A
loc_1_327F:
        cmp    cx, 0fh
        jle    SHORT loc_1_328A
        mov    word ptr [eax], 0fh
loc_1_328A:
        lea    eax, [edi + 14h]
        mov    cx, word ptr [eax]
        cmp    cx, -80h
        jge    SHORT loc_1_329D
        mov    word ptr [eax], 0ff80h
        jmp    SHORT loc_1_32A8
loc_1_329D:
        cmp    cx, 7fh
        jle    SHORT loc_1_32A8
        mov    word ptr [eax], 7fh
loc_1_32A8:
        lea    ecx, [edi + 16h]
        mov    ax, word ptr [ecx]
        cmp    ax, 0ff80h
        jge    SHORT loc_1_32BB
        mov    word ptr [ecx], 0ff80h
        jmp    SHORT loc_1_32C6
loc_1_32BB:
        cmp    ax, 7fh
        jle    SHORT loc_1_32C6
        mov    word ptr [ecx], 7fh
loc_1_32C6:
        lea    ecx, [edi + 1ah]
        mov    ax, word ptr [ecx]
        cmp    ax, 0ff80h
        jge    SHORT loc_1_32D9
        mov    word ptr [ecx], 0ff80h
        jmp    SHORT loc_1_32E4
loc_1_32D9:
        cmp    ax, 7fh
        jle    SHORT loc_1_32E4
        mov    word ptr [ecx], 7fh
loc_1_32E4:
        lea    eax, [edi + 3ah]
        mov    cx, word ptr [eax]
        test   cx, cx
        jge    SHORT loc_1_32F6
        mov    word ptr [eax], 0
        jmp    SHORT loc_1_3301
loc_1_32F6:
        cmp    cx, 7fh
        jle    SHORT loc_1_3301
        mov    word ptr [eax], 7fh
loc_1_3301:
        lea    eax, [edi + 4ah]
        mov    cx, word ptr [eax]
        test   cx, cx
        jge    SHORT loc_1_3313
        mov    word ptr [eax], 0
        jmp    SHORT loc_1_331E
loc_1_3313:
        cmp    cx, 7fh
        jle    SHORT loc_1_331E
        mov    word ptr [eax], 7fh
loc_1_331E:
        movsx  ecx, byte ptr [edi + 8bh]
        movsx  eax, word ptr [edi + 68h]
        add    ecx, eax
        movsx  edx, word ptr [edi + 66h]
        mov    ax, word ptr [edi + 74h]
        add    ecx, edx
        cmp    ax, 0ffffh
        mov    dword ptr [ebp - 18h], ecx
        jne    SHORT loc_1_3347
        movzx  ecx, byte ptr [edi + 8ah]
        jmp    SHORT loc_1_334A
loc_1_3347:
        movsx  ecx, ax
loc_1_334A:
        movsx  eax, word ptr [edi + 70h]
        imul   eax, ecx
        mov    ecx, 36ah
        sub    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [edi + 86h]
        mov    edx, dword ptr [ebp - 18h]
        imul   edx, ecx
        add    eax, edx
        mov    dword ptr [ebp - 18h], eax
        movsx  edx, word ptr [edi + 5ch]
        movsx  eax, word ptr [edi + 70h]
        imul   edx, eax
        imul   edx, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 18h], edx
        mov    eax, dword ptr [ebp - 18h]
        sar    eax, 8
        mov    word ptr [esi + 0eh], ax
        mov    al, byte ptr [ecx + 9]
        cmp    al, 0ah
        ja     SHORT loc_1_3398
        mov    word ptr [edi + 26h], bx
        jmp    NEAR PTR loc_1_342A
loc_1_3398:
        mov    dword ptr [ebp - 4], 18h
        movsx  edx, word ptr [edi + 5eh]
        movzx  eax, al
        db  00Fh, 0B6h, 08Ah, 000h, 000h, 000h, 000h
        db  00Fh, 0B6h, 090h, 000h, 000h, 000h, 000h
        movsx  eax, word ptr [edi + 60h]
        add    ecx, edx
        add    eax, 0ch
        cdq
        idiv   dword ptr [ebp - 4]
        add    ecx, eax
        sub    edx, edx
        lea    eax, [ecx*8]
        mov    ecx, 3
        div    ecx
        mov    edx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [edx + 10h]
        lea    edx, [ecx + eax]
        cmp    edx, ebx
        ja     SHORT loc_1_3424
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 8]
        cmp    al, 7fh
        jae    SHORT loc_1_3408
        movzx  ecx, al
        db  00Fh, 0B6h, 081h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], eax
        mov    eax, 100h
        sub    eax, edx
        imul   eax, dword ptr [ebp - 18h]
        shr    eax, 7
        add    edx, eax
loc_1_3408:
        cmp    byte ptr [esi + 17h], 0
        je     SHORT loc_1_3426
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 151eh], 4d474d31h
        jne    SHORT loc_1_3426
        add    edx, 10h
        cmp    edx, ebx
        jbe    SHORT loc_1_3426
loc_1_3424:
        mov    edx, ebx
loc_1_3426:
        mov    word ptr [edi + 26h], dx
loc_1_342A:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    word ptr [eax + 2], 80h
        jae    NEAR PTR loc_1_3519
        mov    ax, word ptr [edi + 40h]
        test   ax, ax
        je     SHORT loc_1_3471
        movsx  edx, word ptr [edi + 5ch]
        mov    ecx, 3ch
        sub    ecx, edx
        movsx  eax, ax
        imul   ecx, eax
        movsx  edx, word ptr [edi + 38h]
        add    ecx, edx
        mov    eax, 7fffh
        cmp    ecx, eax
        jg     SHORT loc_1_346B
        mov    eax, 0ffff8000h
        cmp    ecx, eax
        jge    SHORT loc_1_346D
loc_1_346B:
        mov    ecx, eax
loc_1_346D:
        mov    word ptr [edi + 38h], cx
loc_1_3471:
        mov    ax, word ptr [edi + 50h]
        test   ax, ax
        je     SHORT loc_1_34A9
        movsx  edx, word ptr [edi + 5ch]
        mov    ecx, 3ch
        sub    ecx, edx
        movsx  eax, ax
        imul   ecx, eax
        movsx  edx, word ptr [edi + 48h]
        add    ecx, edx
        mov    eax, 7fffh
        cmp    ecx, eax
        jg     SHORT loc_1_34A3
        mov    eax, 0ffff8000h
        cmp    ecx, eax
        jge    SHORT loc_1_34A5
loc_1_34A3:
        mov    ecx, eax
loc_1_34A5:
        mov    word ptr [edi + 48h], cx
loc_1_34A9:
        mov    ax, word ptr [edi + 3eh]
        test   ax, ax
        je     SHORT loc_1_34E1
        movsx  edx, word ptr [edi + 5ch]
        mov    ecx, 3ch
        sub    ecx, edx
        movsx  eax, ax
        imul   ecx, eax
        movsx  edx, word ptr [edi + 36h]
        add    ecx, edx
        mov    eax, 7fffh
        cmp    ecx, eax
        jg     SHORT loc_1_34DB
        mov    eax, 0ffff8000h
        cmp    ecx, eax
        jge    SHORT loc_1_34DD
loc_1_34DB:
        mov    ecx, eax
loc_1_34DD:
        mov    word ptr [edi + 36h], cx
loc_1_34E1:
        mov    ax, word ptr [edi + 4eh]
        test   ax, ax
        je     SHORT loc_1_3519
        movsx  edx, word ptr [edi + 5ch]
        mov    ecx, 3ch
        sub    ecx, edx
        movsx  eax, ax
        imul   ecx, eax
        movsx  edx, word ptr [edi + 46h]
        add    ecx, edx
        mov    eax, 7fffh
        cmp    ecx, eax
        jg     SHORT loc_1_3513
        mov    eax, 0ffff8000h
        cmp    ecx, eax
        jge    SHORT loc_1_3515
loc_1_3513:
        mov    ecx, eax
loc_1_3515:
        mov    word ptr [edi + 46h], cx
loc_1_3519:
        movsx  eax, word ptr [edi + 34h]
        push   eax
        push   1ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3525:
        mov    word ptr [edi + 34h], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 44h]
        push   eax
        push   22h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3538:
        mov    word ptr [edi + 44h], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 2ah]
        push   eax
        push   15h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_354B:
        mov    word ptr [edi + 2ah], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 2eh]
        push   eax
        push   17h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_355E:
        mov    word ptr [edi + 2eh], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 32h]
        push   eax
        push   19h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3571:
        mov    word ptr [edi + 32h], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 42h]
        push   eax
        push   21h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3584:
        mov    word ptr [edi + 42h], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 38h]
        push   eax
        push   1ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3597:
        mov    word ptr [edi + 38h], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 48h]
        push   eax
        push   24h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_35AA:
        mov    word ptr [edi + 48h], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 3ch]
        push   eax
        push   1eh
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_35BD:
        mov    word ptr [edi + 3ch], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 4ch]
        push   eax
        push   26h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_35D0:
        mov    word ptr [edi + 4ch], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 36h]
        push   eax
        push   1bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_35E3:
        mov    word ptr [edi + 36h], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 46h]
        push   eax
        push   23h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_35F6:
        mov    word ptr [edi + 46h], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 2ch]
        push   eax
        push   16h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3609:
        mov    word ptr [edi + 2ch], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 30h]
        push   eax
        push   18h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_361C:
        mov    word ptr [edi + 30h], ax
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        cmp    word ptr [eax + 2], 80h
        jae    SHORT loc_1_3657
        cmp    word ptr [edi + 44h], 7dh
        jge    SHORT loc_1_3657
        movsx  eax, word ptr [edi + 5eh]
        cmp    eax, 46h
        jae    SHORT loc_1_3643
        mov    eax, 46h
loc_1_3643:
        movsx  ecx, word ptr [edi + 10h]
        imul   ecx, eax
        add    ecx, 0a0h
        shr    ecx, 7
        mov    word ptr [edi + 10h], cx
loc_1_3657:
        movsx  eax, word ptr [edi + 22h]
        shl    eax, 4
        mov    ecx, 7dh
        cdq
        idiv   ecx
        add    eax, 40h
        jns    SHORT loc_1_3673
        mov    word ptr [edi + 22h], 0
        jmp    SHORT loc_1_3682
loc_1_3673:
        mov    word ptr [edi + 22h], 7fh
        cmp    eax, 7fh
        jg     SHORT loc_1_3682
        mov    word ptr [edi + 22h], ax
loc_1_3682:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 12h], 0
        jne    SHORT loc_1_36AD
        movsx  eax, word ptr [edi + 1eh]
        imul   eax, eax, 64h
        cdq
        mov    ecx, 188h
        idiv   ecx
        mov    word ptr [edi + 1eh], ax
        movsx  eax, word ptr [edi + 20h]
        imul   eax, eax, 64h
        cdq
        idiv   ecx
        mov    word ptr [edi + 20h], ax
loc_1_36AD:
        mov    ax, word ptr [edi + 60h]
        push   dword ptr [ebp - 0ch]
        push   edi
        mov    word ptr [esi + 4], ax
        mov    cx, word ptr [edi + 5eh]
        mov    word ptr [esi + 2], cx
        mov    dl, byte ptr [edi + 0ah]
        mov    byte ptr [esi + 14h], dl
        mov    al, byte ptr [edi + 3ch]
        mov    byte ptr [esi + 15h], al
        mov    cl, byte ptr [edi + 4ch]
        mov    byte ptr [esi + 16h], cl
        mov    dl, byte ptr [edi + 22h]
        mov    byte ptr [esi + 12h], dl
        mov    eax, dword ptr [edi + 90h]
        mov    dword ptr [esi + 1ch], eax
        mov    cl, byte ptr [edi + 72h]
        mov    byte ptr [esi + 13h], cl
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [edi + 8ch]
        mov    word ptr [esi + 1ah], dx
        movzx  eax, word ptr [ecx + 12h]
        push   eax
        call   sub_1_4FCB
        add    esp, 0ch
        mov    cl, byte ptr [edi + 22h]
        mov    byte ptr [esi + 18h], cl
        mov    cl, byte ptr [edi + 22h]
        mov    byte ptr [esi + 19h], cl
loc_1_370F:
        inc    dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 18h], edx
        ja     NEAR PTR loc_1_314E
loc_1_371E:
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_1_3C87
        lea    eax, [ebp - 4ch]
        mov    dword ptr [ebp - 2ch], eax
loc_1_3735:
        mov    eax, dword ptr [ebp - 8]
        movzx  edi, byte ptr [ebp + eax - 28h]
        cmp    edi, 0ffh
        je     NEAR PTR loc_1_3C74
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax]
        mov    eax, edi
        shl    eax, 5
        lea    edx, [eax + ecx + 42h]
        mov    eax, dword ptr [ebp - 0ch]
        movzx  ecx, word ptr [edx + 0eh]
        add    ecx, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 1ch], edx
        cmp    ecx, 0ffffh
        jbe    SHORT loc_1_3778
        mov    dword ptr [ebp - 4], 0ffffh
        jmp    SHORT loc_1_3786
loc_1_3778:
        mov    eax, 0
        test   ecx, ecx
        jl     SHORT loc_1_3783
        mov    eax, ecx
loc_1_3783:
        mov    dword ptr [ebp - 4], eax
loc_1_3786:
        mov    eax, dword ptr [esi + 76h]
        sub    eax, 4
        test   byte ptr [esi + 6ch], 1
        mov    dword ptr [ebp - 30h], eax
        je     SHORT loc_1_37CD
        mov    eax, dword ptr [esi + 7eh]
        mov    edx, dword ptr [ebp - 1ch]
        dec    eax
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [esi + 82h]
        mov    dword ptr [edx + 0ah], 0
        dec    eax
        test   byte ptr [esi + 6ch], 2
        mov    dword ptr [ebp - 34h], eax
        je     SHORT loc_1_37C1
        mov    eax, dword ptr [esi + 7ah]
        add    eax, 4
        mov    dword ptr [edx + 6], eax
        jmp    SHORT loc_1_37E9
loc_1_37C1:
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [edx + 6], 0
        jmp    SHORT loc_1_37E9
loc_1_37CD:
        mov    eax, dword ptr [esi + 7ah]
        mov    edx, dword ptr [ebp - 1ch]
        add    eax, 8
        mov    dword ptr [edx + 6], 0
        lea    ecx, [eax - 4]
        mov    dword ptr [ebp - 34h], eax
        mov    dword ptr [ebp - 20h], ecx
        mov    dword ptr [edx + 0ah], ecx
loc_1_37E9:
        push   0ffffh
        mov    eax, edi
        or     al, 60h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0ffffh
        mov    eax, edi
        or     al, 40h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    ax, word ptr [esi + 44h]
        cmp    ax, 80h
        jne    SHORT loc_1_381E
        mov    eax, 7fh
        jmp    SHORT loc_1_3821
loc_1_381E:
        movsx  eax, ax
loc_1_3821:
        movsx  ecx, word ptr [esi + 46h]
        shl    ecx, 8
        or     ecx, eax
        mov    eax, edi
        push   ecx
        or     eax, 380h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [esi + 2ah]
        push   ecx
        mov    eax, edi
        or     eax, 3a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    ax, word ptr [esi + 34h]
        cmp    ax, 80h
        jne    SHORT loc_1_3861
        mov    eax, 7fh
        jmp    SHORT loc_1_3864
loc_1_3861:
        movsx  eax, ax
loc_1_3864:
        movsx  ecx, word ptr [esi + 36h]
        shl    ecx, 8
        lea    ebx, [esi + 10h]
        or     ecx, eax
        mov    eax, edi
        push   ecx
        or     eax, 3c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 3ah]
        movsx  ecx, word ptr [esi + 38h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 2e0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [esi + 2eh]
        push   ecx
        mov    eax, edi
        or     eax, 3e0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        push   dword ptr [ebp - 4]
        mov    eax, edi
        or     ah, 4
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [ebx]
        movsx  ecx, word ptr [esi + 26h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 420h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 0eh]
        movsx  ecx, byte ptr [esi + 16h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 440h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    edx, dword ptr [ebp - 0ch]
        movzx  ecx, byte ptr [edx + 0ah]
        movzx  eax, byte ptr [edx + 0bh]
        add    ecx, eax
        movsx  eax, word ptr [esi + 0ah]
        add    eax, ecx
        cmp    eax, 7fh
        jle    SHORT loc_1_391D
        mov    eax, 7fh
loc_1_391D:
        movsx  ecx, byte ptr [esi + 14h]
        shl    eax, 8
        or     ecx, eax
        mov    eax, edi
        push   ecx
        or     eax, 460h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 1ah]
        movsx  ecx, word ptr [esi + 2ch]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 480h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 0ch]
        movsx  ecx, word ptr [esi + 30h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 4a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        cmp    word ptr [esi + 34h], 80h
        jne    SHORT loc_1_39D2
        cmp    word ptr [esi + 32h], 8000h
        jb     SHORT loc_1_39D2
        push   0bfffh
        mov    eax, edi
        or     eax, 2c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [esi + 0eh]
        shl    ecx, 4
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        mov    ecx, 0ffffh
        cmp    eax, 0ffffh
        ja     SHORT loc_1_39B2
        mov    ecx, eax
loc_1_39B2:
        mov    dword ptr [ebp - 4], ecx
        movsx  eax, word ptr [ebx]
        movsx  ecx, word ptr [esi + 16h]
        add    eax, ecx
        jns    SHORT loc_1_39C4
        xor    eax, eax
        jmp    SHORT loc_1_39EA
loc_1_39C4:
        cmp    eax, 0ffh
        jle    SHORT loc_1_39EA
        mov    eax, 0ffh
        jmp    SHORT loc_1_39EA
loc_1_39D2:
        movsx  eax, word ptr [esi + 32h]
        push   eax
        mov    eax, edi
        or     eax, 2c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [ebx]
loc_1_39EA:
        shl    eax, 8
        cmp    dword ptr [ebp - 4], 0ffffh
        mov    dword ptr [ebp - 10h], eax
        jne    SHORT loc_1_3A02
        mov    dword ptr [ebp - 18h], 0ffffh
        jmp    SHORT loc_1_3A70
loc_1_3A02:
        mov    ebx, 1
        mov    ecx, dword ptr [ebp - 4]
        shr    ecx, 0ch
        shl    ebx, cl
        test   byte ptr [ebp - 3], 8
        je     SHORT loc_1_3A27
        mov    eax, ebx
        mov    ecx, 2710h
        imul   eax, eax, 102eh
        cdq
        idiv   ecx
        add    ebx, eax
loc_1_3A27:
        test   byte ptr [ebp - 3], 4
        je     SHORT loc_1_3A3F
        mov    eax, ebx
        mov    ecx, 2710h
        imul   eax, eax, 764h
        cdq
        idiv   ecx
        add    ebx, eax
loc_1_3A3F:
        test   byte ptr [ebp - 3], 2
        je     SHORT loc_1_3A57
        mov    eax, ebx
        mov    ecx, 2710h
        imul   eax, eax, 389h
        cdq
        idiv   ecx
        add    ebx, eax
loc_1_3A57:
        mov    dword ptr [ebp - 18h], 0ffffh
        mov    eax, ebx
        sar    eax, 2
        add    ebx, eax
        cmp    ebx, 0ffffh
        ja     SHORT loc_1_3A70
        mov    dword ptr [ebp - 18h], ebx
loc_1_3A70:
        cmp    word ptr [esi + 44h], 80h
        jne    SHORT loc_1_3ACA
        cmp    word ptr [esi + 42h], 8000h
        jb     SHORT loc_1_3ACA
        push   0bfffh
        mov    eax, edi
        or     eax, 280h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 26h]
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 0fh
        xor    eax, edx
        sub    eax, edx
        movsx  edx, ax
        movsx  eax, word ptr [esi + 26h]
        db  066h, 08Bh, 01Ch, 055h, 000h, 000h, 000h, 000h
        cdq
        and    edx, 0fh
        add    eax, edx
        sar    eax, 4
        mov    cl, al
        shr    bx, cl
        movzx  eax, bx
        mov    dword ptr [ebp - 14h], eax
        jmp    SHORT loc_1_3AE6
loc_1_3ACA:
        movsx  eax, word ptr [esi + 42h]
        push   eax
        mov    eax, edi
        or     eax, 280h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    dword ptr [ebp - 14h], 0
loc_1_3AE6:
        mov    ebx, edi
        push   0
        or     bl, 20h
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [esi + 22h]
        shl    eax, 18h
        or     dword ptr [ebp - 20h], eax
        mov    eax, edi
        push   dword ptr [ebp - 20h]
        or     al, 0c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [esi + 1eh]
        shl    eax, 18h
        or     dword ptr [ebp - 34h], eax
        mov    eax, edi
        push   dword ptr [ebp - 34h]
        or     al, 0e0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   dword ptr [ebp - 30h]
        mov    eax, edi
        or     ah, 2
        push   eax
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 0a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 80h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [esi + 12h]
        shl    eax, 1ch
        or     dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    eax, dword ptr [ebp - 14h]
        shl    eax, 10h
        or     eax, dword ptr [ebp - 10h]
        push   eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, edi
        or     al, 60h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   dword ptr [ebp - 4]
        mov    eax, edi
        or     al, 40h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        cmp    word ptr [esi + 8ch], 4
        jne    SHORT loc_1_3BEA
        push   0
        lea    eax, [edi - 1]
        or     al, 20h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  ecx, word ptr [esi + 20h]
        movsx  eax, word ptr [esi + 24h]
        shl    ecx, 8
        or     ecx, eax
        push   ecx
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   8000h
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        jmp    SHORT loc_1_3C63
loc_1_3BEA:
        mov    eax, dword ptr [ebp - 18h]
        shl    eax, 10h
        mov    dword ptr [ebp - 4], eax
        movsx  ecx, word ptr [esi + 24h]
        movsx  eax, word ptr [esi + 20h]
        shl    eax, 8
        or     eax, ecx
        or     eax, dword ptr [ebp - 4]
        push   eax
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   dword ptr [ebp - 4]
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [esi + 4ah]
        movsx  ecx, word ptr [esi + 48h]
        shl    eax, 8
        mov    ebx, 2a0h
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, ebx
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        cmp    word ptr [esi + 8ch], 2
        jne    SHORT loc_1_3C63
        mov    eax, dword ptr [ebp - 2ch]
        inc    edi
        or     edi, ebx
        mov    eax, dword ptr [eax - 4]
        movsx  ecx, word ptr [eax + 4ah]
        movsx  eax, word ptr [eax + 48h]
        shl    ecx, 8
        or     ecx, eax
        push   ecx
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
loc_1_3C63:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 1ch]
        shl    ax, 8
        or     ax, word ptr [ebp + 10h]
        mov    word ptr [edx], ax
loc_1_3C74:
        add    dword ptr [ebp - 2ch], 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 18h], eax
        ja     NEAR PTR loc_1_3735
loc_1_3C87:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
sub_1_3C92:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 8
        mov    ebp, esp
        sub    esp, 8
        or     eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        shl    eax, 5
        lea    esi, [edi + 42h]
        cmp    word ptr [edi + 6], 0
        lea    ebx, [eax + edi + 442h]
        jbe    SHORT loc_1_3CF9
loc_1_3CC9:
        mov    ax, word ptr [esi]
        movzx  ecx, ax
        cmp    ecx, dword ptr [ebp - 8]
        jne    SHORT loc_1_3CEA
        cmp    byte ptr [ebx + 4], 0
        jne    SHORT loc_1_3CE5
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_1_3D04
        jmp    SHORT loc_1_3CEA
loc_1_3CE5:
        or     al, 0ffh
        mov    word ptr [esi], ax
loc_1_3CEA:
        add    esi, 20h
        inc    dword ptr [ebp - 4]
        movzx  eax, word ptr [edi + 6]
        cmp    eax, dword ptr [ebp - 4]
        ja     SHORT loc_1_3CC9
loc_1_3CF9:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_1_3D04:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 8000h
        lea    edi, [eax + esi + 42h]
        mov    word ptr [edi], 0ff00h
        movzx  eax, byte ptr [edi + 16h]
        or     eax, ebx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, 2a0h
        push   eax
        push   esi
        call   sub_1_19AA
        movzx  eax, byte ptr [edi + 15h]
        or     eax, ebx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, 2e0h
        push   eax
        push   esi
        call   sub_1_19AA
        cmp    dword ptr [edi + 6], 0
        je     SHORT loc_1_3DA5
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, 0ff000000h
        or     al, 0e0h
        push   eax
        mov    dword ptr [ebp - 4], eax
        push   esi
        call   sub_1_1A3D
        and    eax, ebx
        mov    ecx, dword ptr [edi + 6]
        add    ecx, 4
        or     ecx, eax
        push   ecx
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_1_1A14
        mov    eax, dword ptr [ebp + 0ch]
        or     al, 0c0h
        push   eax
        mov    dword ptr [ebp - 4], eax
        push   esi
        call   sub_1_1A3D
        and    eax, ebx
        mov    ecx, dword ptr [edi + 6]
        or     ecx, eax
        push   ecx
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_1_1A14
        mov    ecx, dword ptr [edi + 6]
        mov    dword ptr [edi + 0ah], ecx
loc_1_3DA5:
        mov    word ptr [edi], 0ffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_3DB3:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 37ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 14h]
        cmp    edi, eax
        jne    SHORT loc_1_3DDC
        push   eax
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_3C92
        jmp    NEAR PTR loc_1_3E8F
loc_1_3DDC:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        shl    ecx, 5
        lea    ebx, [ecx + esi + 442h]
        lea    ecx, [ecx + esi + 456h]
        mov    dx, word ptr [ecx]
        cmp    dx, 120h
        jae    SHORT loc_1_3E37
        movzx  edx, dx
        mov    edx, dword ptr [esi + edx*4 + 662h]
        test   edx, edx
        je     SHORT loc_1_3E37
        movzx  eax, word ptr [edx + 48h]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 1
        movzx  ecx, word ptr [ecx]
        lea    eax, [ebp - 37ch]
        push   eax
        push   edi
        push   dword ptr [ebp + 10h]
        movzx  eax, word ptr [ebx]
        push   eax
        push   ecx
        push   esi
        jne    SHORT loc_1_3E32
        call   sub_1_1FD6
        jmp    SHORT loc_1_3E37
loc_1_3E32:
        call   sub_1_2359
loc_1_3E37:
        test   eax, eax
        jne    SHORT loc_1_3E6D
        cmp    word ptr [ebx + 2], 80h
        jae    SHORT loc_1_3E4A
        mov    eax, 1
        jmp    SHORT loc_1_3E8F
loc_1_3E4A:
        lea    eax, [ebp - 37ch]
        lea    edx, [ebp - 4]
        push   eax
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   edx
        push   esi
        call   sub_1_3E98
        test   eax, eax
        jne    SHORT loc_1_3E6D
        mov    eax, 1
        jmp    SHORT loc_1_3E8F
loc_1_3E6D:
        cmp    dword ptr [ebp - 4], 1
        lea    ecx, [ebp - 37ch]
        push   ecx
        push   eax
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   esi
        jne    SHORT loc_1_3E8A
        call   sub_1_27BC
        jmp    SHORT loc_1_3E8F
loc_1_3E8A:
        call   sub_1_2FFE
loc_1_3E8F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_1_3E98:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        shl    eax, 5
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        push   ebx
        lea    eax, [eax + ecx + 442h]
        push   esi
        mov    ebx, 80h
        movzx  ecx, word ptr [eax + 2]
        push   edi
        mov    dword ptr [ebp - 4], ecx
        movzx  esi, byte ptr [eax + 16h]
loc_1_3EC2:
        cmp    dword ptr [ebp - 4], ebx
        jbe    SHORT loc_1_3ECC
        mov    dword ptr [ebp - 4], ebx
        jmp    SHORT loc_1_3EE7
loc_1_3ECC:
        test   esi, esi
        je     NEAR PTR loc_1_3F54
        sub    esi, 80h
        cmp    esi, 1
        mov    esi, 0
        adc    esi, -1
        and    esi, ebx
loc_1_3EE7:
        mov    edi, esi
        cmp    esi, 120h
        jae    SHORT loc_1_3EC2
loc_1_3EF1:
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + edi*4 + 662h]
        test   ecx, ecx
        je     SHORT loc_1_3F37
        movzx  eax, word ptr [ecx + 24h]
        cmp    eax, esi
        jne    SHORT loc_1_3F37
        push   dword ptr [ebp + 1ch]
        movzx  eax, word ptr [ecx + 48h]
        mov    edx, dword ptr [ebp + 0ch]
        cmp    eax, 1
        push   dword ptr [ebp + 18h]
        mov    dword ptr [edx], eax
        push   dword ptr [ebp + 14h]
        movzx  ecx, word ptr [ecx + 26h]
        push   ecx
        push   edi
        push   dword ptr [ebp + 8]
        jne    SHORT loc_1_3F2E
        call   sub_1_1FD6
        jmp    SHORT loc_1_3F33
loc_1_3F2E:
        call   sub_1_2359
loc_1_3F33:
        test   eax, eax
        jne    SHORT loc_1_3F56
loc_1_3F37:
        cmp    edi, 80h
        jbe    SHORT loc_1_3F42
        inc    edi
        jmp    SHORT loc_1_3F47
loc_1_3F42:
        mov    edi, 81h
loc_1_3F47:
        cmp    edi, 120h
        jb     SHORT loc_1_3EF1
        jmp    NEAR PTR loc_1_3EC2
loc_1_3F54:
        xor    eax, eax
loc_1_3F56:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
sub_1_3F5F:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 10h]
        push   edi
        mov    eax, ebx
        mov    edi, dword ptr [esp + 10h]
        shl    eax, 5
        mov    esi, dword ptr [esp + 18h]
        lea    ecx, [eax + edi]
        xor    eax, eax
        mov    word ptr [ecx + 45ah], ax
        test   byte ptr [esi + 64h], 80h
        jne    SHORT loc_1_3F91
        mov    byte ptr [esi + 64h], al
        and    word ptr [ecx + 45ch], 0ff00h
loc_1_3F91:
        test   byte ptr [esi + 65h], 80h
        jne    SHORT loc_1_3FA4
        mov    byte ptr [esi + 65h], 0
        and    word ptr [ecx + 45ch], 0ffh
loc_1_3FA4:
        test   byte ptr [esi + 62h], 80h
        jne    SHORT loc_1_3FB7
        mov    byte ptr [esi + 62h], 0
        and    word ptr [ecx + 45eh], 0ff80h
loc_1_3FB7:
        test   byte ptr [esi + 63h], 80h
        jne    SHORT loc_1_3FDC
        mov    byte ptr [esi + 63h], 0
        and    word ptr [ecx + 45eh], 7fh
        mov    ecx, dword ptr [edi + 0aeah]
        test   ecx, ecx
        je     SHORT loc_1_3FDC
        push   0
        push   ebx
        push   edi
        call   ecx
        add    esp, 0ch
loc_1_3FDC:
        test   byte ptr [esi + 1], 80h
        jne    SHORT loc_1_3FEF
        mov    byte ptr [esi + 1], 0
        push   0
        push   ebx
        push   edi
        call   sub_1_41FA
loc_1_3FEF:
        test   byte ptr [esi + 40h], 80h
        jne    SHORT loc_1_4002
        mov    byte ptr [esi + 40h], 0
        push   0
        push   ebx
        push   edi
        call   sub_1_4190
loc_1_4002:
        test   byte ptr [esi + 0bh], 80h
        jne    SHORT loc_1_4015
        mov    byte ptr [esi + 0bh], 7fh
        push   7fh
        push   ebx
        push   edi
        call   sub_1_4031
loc_1_4015:
        push   40h
        push   0
        push   ebx
        push   edi
        call   sub_1_4AD6
        push   0
        push   ebx
        push   edi
        call   sub_1_4A9E
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_1_4031:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 10h]
        add    eax, edx
        mov    byte ptr [eax + 44ah], cl
        movzx  eax, byte ptr [eax + 44bh]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   edx
        call   sub_1_405E
        pop    ebp
        ret    0ch
sub_1_405E:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    ebp, esp
        sub    esp, 0ch
        mov    dl, byte ptr [ebp + 10h]
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    byte ptr [eax + esi + 44bh], dl
        lea    ecx, [eax + esi + 442h]
        lea    edi, [esi + 42h]
        mov    dword ptr [ebp - 0ch], ecx
        cmp    word ptr [esi + 6], 0
        jbe    NEAR PTR loc_1_4185
loc_1_409A:
        mov    ax, word ptr [edi]
        shr    ax, 8
        movzx  ecx, ax
        cmp    ecx, dword ptr [ebp + 0ch]
        jne    NEAR PTR loc_1_4172
        cmp    dword ptr [ebp + 10h], 0ah
        jbe    NEAR PTR loc_1_4172
        movsx  eax, word ptr [edi + 2]
        db  00Fh, 0B6h, 098h, 000h, 000h, 000h, 000h
        movsx  eax, word ptr [edi + 4]
        add    eax, 0ch
        mov    ecx, 18h
        cdq
        idiv   ecx
        add    ebx, eax
        mov    edx, dword ptr [ebp + 10h]
        db  00Fh, 0B6h, 082h, 000h, 000h, 000h, 000h
        add    ebx, eax
        sub    edx, edx
        mov    ecx, 3
        lea    eax, [ebx*8]
        div    ecx
        movzx  edx, word ptr [esi + 10h]
        add    eax, edx
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 0ffh
        jae    SHORT loc_1_4143
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 8]
        cmp    al, 7fh
        jae    SHORT loc_1_4124
        movzx  eax, al
        db  00Fh, 0B6h, 088h, 000h, 000h, 000h, 000h
        mov    eax, 100h
        sub    eax, dword ptr [ebp - 4]
        imul   ecx, eax
        shr    ecx, 7
        add    dword ptr [ebp - 4], ecx
loc_1_4124:
        cmp    dword ptr [esi + 151eh], 4d474d31h
        jne    SHORT loc_1_414A
        cmp    byte ptr [edi + 17h], 0
        je     SHORT loc_1_414A
        add    dword ptr [ebp - 4], 10h
        cmp    dword ptr [ebp - 4], 0ffh
        jbe    SHORT loc_1_414A
loc_1_4143:
        mov    dword ptr [ebp - 4], 0ffh
loc_1_414A:
        mov    ebx, dword ptr [ebp - 8]
        or     ebx, 420h
        push   ebx
        push   esi
        call   sub_1_19DF
        xor    ecx, ecx
        mov    cx, ax
        and    ecx, 0ff00h
        or     dword ptr [ebp - 4], ecx
        push   dword ptr [ebp - 4]
        push   ebx
        push   esi
        call   sub_1_19AA
loc_1_4172:
        add    edi, 20h
        inc    dword ptr [ebp - 8]
        movzx  eax, word ptr [esi + 6]
        cmp    eax, dword ptr [ebp - 8]
        ja     NEAR PTR loc_1_409A
loc_1_4185:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_4190:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 10h], 40h
        jae    SHORT loc_1_41E3
        shl    eax, 5
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        lea    edi, [esi + 42h]
        mov    byte ptr [eax + esi + 446h], bl
        cmp    word ptr [esi + 6], bx
        jbe    SHORT loc_1_41F1
loc_1_41B8:
        mov    ax, word ptr [edi]
        mov    cx, ax
        shr    cx, 8
        cmp    al, 0ffh
        movzx  ecx, cx
        jne    SHORT loc_1_41D5
        cmp    ecx, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_41D5
        push   ebx
        push   esi
        call   sub_1_3D04
loc_1_41D5:
        add    edi, 20h
        inc    ebx
        movzx  eax, word ptr [esi + 6]
        cmp    eax, ebx
        ja     SHORT loc_1_41B8
        jmp    SHORT loc_1_41F1
loc_1_41E3:
        shl    eax, 5
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [eax + esi + 446h], 1
loc_1_41F1:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
sub_1_41FA:
        push   ebp
        mov    ecx, 1eh
        mov    ebp, esp
        sub    edx, edx
        sub    esp, 0ch
        mov    eax, dword ptr [ebp + 10h]
        div    ecx
        push   ebx
        mov    dword ptr [ebp + 10h], eax
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        shl    eax, 5
        mov    cl, byte ptr [ebp + 10h]
        add    eax, edi
        lea    ebx, [edi + 42h]
        xor    esi, esi
        mov    dword ptr [ebp - 0ch], eax
        mov    byte ptr [eax + 44ch], cl
        cmp    word ptr [edi + 6], si
        jbe    SHORT loc_1_429A
loc_1_4234:
        mov    ax, word ptr [ebx]
        shr    ax, 8
        movzx  ecx, ax
        cmp    ecx, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_428E
        mov    edx, dword ptr [ebp - 0ch]
        movsx  ecx, byte ptr [ebx + 14h]
        movzx  eax, byte ptr [edx + 44dh]
        add    ecx, eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 7fh
        jle    SHORT loc_1_4267
        mov    dword ptr [ebp - 4], 7fh
loc_1_4267:
        shl    dword ptr [ebp - 4], 8
        mov    eax, esi
        or     eax, 460h
        push   eax
        mov    dword ptr [ebp - 8], eax
        push   edi
        call   sub_1_19DF
        movzx  ecx, al
        or     dword ptr [ebp - 4], ecx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        push   edi
        call   sub_1_19AA
loc_1_428E:
        add    ebx, 20h
        inc    esi
        movzx  eax, word ptr [edi + 6]
        cmp    eax, esi
        ja     SHORT loc_1_4234
loc_1_429A:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_42A5:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        cmp    eax, 6
        ja     SHORT loc_1_42CA
        je     NEAR PTR loc_1_4344
        test   eax, eax
        je     SHORT loc_1_431E
        cmp    eax, 1
        je     SHORT loc_1_4331
loc_1_42C0:
        mov    eax, 1
        jmp    NEAR PTR loc_1_4466
loc_1_42CA:
        cmp    eax, 0ah
        ja     SHORT loc_1_42DC
        je     NEAR PTR loc_1_436A
        cmp    eax, 7
        je     SHORT loc_1_4357
        jmp    SHORT loc_1_42C0
loc_1_42DC:
        cmp    eax, 26h
        ja     SHORT loc_1_42F2
        je     NEAR PTR loc_1_4390
        cmp    eax, 0bh
        je     NEAR PTR loc_1_437D
        jmp    SHORT loc_1_42C0
loc_1_42F2:
        cmp    eax, 5bh
        ja     SHORT loc_1_4308
        je     NEAR PTR loc_1_43B6
        cmp    eax, 40h
        je     NEAR PTR loc_1_43A3
        jmp    SHORT loc_1_42C0
loc_1_4308:
        sub    eax, 5dh
        cmp    eax, 1eh
        ja     SHORT loc_1_42C0
        db  00Fh, 0B6h, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_1_431E:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_44AD
        jmp    NEAR PTR loc_1_4464
loc_1_4331:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_41FA
        jmp    NEAR PTR loc_1_4464
loc_1_4344:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_46C7
        jmp    NEAR PTR loc_1_4464
loc_1_4357:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_405E
        jmp    NEAR PTR loc_1_4464
loc_1_436A:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_4516
        jmp    NEAR PTR loc_1_4464
loc_1_437D:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_4031
        jmp    NEAR PTR loc_1_4464
loc_1_4390:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_4745
        jmp    NEAR PTR loc_1_4464
loc_1_43A3:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_4190
        jmp    NEAR PTR loc_1_4464
loc_1_43B6:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_44C8
        jmp    NEAR PTR loc_1_4464
loc_1_43C9:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_44EF
        jmp    NEAR PTR loc_1_4464
loc_1_43DC:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0ae6h]
        test   ecx, ecx
        je     SHORT loc_1_43F7
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   ecx
        add    esp, 0ch
        jmp    SHORT loc_1_4466
loc_1_43F7:
        mov    eax, 1
        jmp    SHORT loc_1_4466
loc_1_43FE:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0aeah]
        test   ecx, ecx
        je     SHORT loc_1_4419
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   eax
        call   ecx
        add    esp, 0ch
        jmp    SHORT loc_1_4466
loc_1_4419:
        mov    eax, 1
        jmp    SHORT loc_1_4466
loc_1_4420:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_4657
        jmp    SHORT loc_1_4464
loc_1_4430:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_468E
        jmp    SHORT loc_1_4464
loc_1_4440:
        push   0
        jmp    SHORT loc_1_4456
loc_1_4444:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_47A1
        jmp    SHORT loc_1_4464
loc_1_4454:
        push   1
loc_1_4456:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_480D
loc_1_4464:
        xor    eax, eax
loc_1_4466:
        pop    ebp
        ret    10h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 008h, 008h, 008h, 008h, 001h, 002h, 003h, 004h, 008h, 008h, 008h
        db  008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 005h
        db  006h, 008h, 007h
sub_1_44AD:
        mov    ecx, dword ptr [esp + 8]
        mov    dl, byte ptr [esp + 0ch]
        shl    ecx, 5
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [ecx + eax + 459h], dl
        xor    eax, eax
        ret    0ch
sub_1_44C8:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, 64h
        imul   eax, eax, 5ah
        sub    edx, edx
        div    ecx
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shl    edx, 5
        mov    byte ptr [edx + ecx + 447h], al
        xor    eax, eax
        ret    0ch
sub_1_44EF:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, 64h
        imul   eax, eax, 5ah
        sub    edx, edx
        div    ecx
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shl    edx, 5
        mov    byte ptr [edx + ecx + 448h], al
        xor    eax, eax
        ret    0ch
sub_1_4516:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 10h]
        lea    eax, [eax + esi + 449h]
        cmp    byte ptr [eax], cl
        je     SHORT loc_1_4599
        mov    byte ptr [eax], cl
        cmp    word ptr [esi + 12h], 1
        je     SHORT loc_1_4599
        xor    ebx, ebx
        cmp    word ptr [esi + 6], bx
        jbe    SHORT loc_1_4599
        lea    edi, [esi + 42h]
loc_1_4548:
        mov    ax, word ptr [edi]
        shr    ax, 8
        movzx  ecx, ax
        cmp    ecx, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_458D
        movzx  ecx, byte ptr [edi + 12h]
        mov    eax, 17fh
        add    ecx, dword ptr [ebp + 10h]
        add    ecx, ecx
        sub    eax, ecx
        cmp    eax, 0feh
        jl     SHORT loc_1_4575
        mov    eax, 0ffh
        jmp    SHORT loc_1_457C
loc_1_4575:
        cmp    eax, 1
        jg     SHORT loc_1_457C
        xor    eax, eax
loc_1_457C:
        push   0
        mov    byte ptr [edi + 18h], al
        push   ebx
        push   0
        push   esi
        call   sub_1_45A2
        add    esp, 10h
loc_1_458D:
        add    edi, 20h
        inc    ebx
        movzx  eax, word ptr [esi + 6]
        cmp    eax, ebx
        ja     SHORT loc_1_4548
loc_1_4599:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
sub_1_45A2:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        shl    ecx, 5
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, ecx
        cmp    word ptr [esi + 42h], 2000h
        jae    NEAR PTR loc_1_4652
        mov    cl, byte ptr [esi + 5ah]
        mov    al, byte ptr [esi + 5bh]
        cmp    cl, al
        je     NEAR PTR loc_1_4652
        movzx  ebx, al
        movzx  eax, cl
        cmp    eax, ebx
        jle    SHORT loc_1_45DB
        inc    ebx
        jmp    SHORT loc_1_45DC
loc_1_45DB:
        dec    ebx
loc_1_45DC:
        mov    edi, dword ptr [ebp + 10h]
        mov    byte ptr [esi + 5bh], bl
        or     edi, 0c0h
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_1A3D
        and    eax, 0ffffffh
        mov    ecx, ebx
        shl    ecx, 18h
        or     ecx, eax
        push   ecx
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        test   ebx, ebx
        je     SHORT loc_1_4612
        neg    bl
        movzx  edi, bl
        jmp    SHORT loc_1_4617
loc_1_4612:
        mov    edi, 0ffh
loc_1_4617:
        mov    ebx, dword ptr [ebp + 10h]
        or     bl, 20h
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A3D
        and    al, 0
        or     edi, eax
        push   edi
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    al, byte ptr [esi + 5bh]
        cmp    byte ptr [esi + 5ah], al
        je     SHORT loc_1_4652
        push   0
        add    esi, 5ah
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_1_1845
loc_1_4652:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_4657:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        shl    ecx, 5
        add    ecx, eax
        movzx  dx, byte ptr [esp + 0ch]
        mov    word ptr [ecx + 45ah], 100h
        add    ecx, 45ch
        mov    ax, word ptr [ecx]
        and    ax, 0ff00h
        or     dx, ax
        mov    word ptr [ecx], ax
        xor    eax, eax
        mov    word ptr [ecx], dx
        ret    0ch
sub_1_468E:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        shl    ecx, 5
        mov    edx, dword ptr [esp + 0ch]
        shl    dx, 8
        add    ecx, eax
        mov    word ptr [ecx + 45ah], 100h
        add    ecx, 45ch
        mov    ax, word ptr [ecx]
        and    ax, 0ffh
        or     dx, ax
        mov    word ptr [ecx], ax
        xor    eax, eax
        mov    word ptr [ecx], dx
        ret    0ch
sub_1_46C7:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [eax + edx]
        movzx  eax, word ptr [esi + 45ah]
        cmp    eax, 100h
        je     SHORT loc_1_46F9
        cmp    eax, 200h
        je     SHORT loc_1_4710
loc_1_46EE:
        mov    eax, 1
loc_1_46F3:
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_1_46F9:
        cmp    word ptr [esi + 45ch], 0
        jne    SHORT loc_1_46EE
        mov    al, byte ptr [ebp + 10h]
        mov    byte ptr [esi + 44fh], al
        xor    eax, eax
        jmp    SHORT loc_1_46F3
loc_1_4710:
        lea    edi, [edx + 0ae2h]
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_46EE
        add    esi, 460h
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [esi]
        push   0
        and    ax, 7fh
        push   dword ptr [ebp + 0ch]
        push   edx
        shl    cx, 7
        or     cx, ax
        mov    word ptr [esi], ax
        mov    word ptr [esi], cx
        call   dword ptr [edi]
        add    esp, 0ch
        jmp    SHORT loc_1_46F3
sub_1_4745:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [eax + edx]
        movzx  eax, word ptr [esi + 45ah]
        cmp    eax, 200h
        jne    SHORT loc_1_4796
        lea    edi, [edx + 0ae2h]
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_4796
        add    esi, 460h
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [esi]
        push   0
        and    ax, 0ff80h
        push   dword ptr [ebp + 0ch]
        push   edx
        or     cx, ax
        mov    word ptr [esi], ax
        mov    word ptr [esi], cx
        call   dword ptr [edi]
        add    esp, 0ch
        jmp    SHORT loc_1_479B
loc_1_4796:
        mov    eax, 1
loc_1_479B:
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
sub_1_47A1:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        xor    ebx, ebx
        mov    eax, edi
        push   ebx
        shl    eax, 5
        push   edi
        add    eax, esi
        push   esi
        mov    word ptr [eax + 45ah], bx
        mov    word ptr [eax + 45ch], bx
        mov    word ptr [eax + 45eh], bx
        mov    byte ptr [eax + 44ch], bl
        call   sub_1_4190
        push   7fh
        push   edi
        push   esi
        call   sub_1_4031
        push   40h
        push   ebx
        push   edi
        push   esi
        call   sub_1_4AD6
        push   ebx
        push   edi
        push   esi
        call   sub_1_4A9E
        mov    eax, dword ptr [esi + 0aeah]
        test   eax, eax
        je     SHORT loc_1_4805
        push   ebx
        push   edi
        push   esi
        call   eax
        add    esp, 0ch
loc_1_4805:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_1_480D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [ebp + 14h], 0
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_4836
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        shl    eax, 5
        cmp    byte ptr [eax + edi + 446h], 0
        je     SHORT loc_1_4839
        mov    ebx, 1
        jmp    SHORT loc_1_483B
loc_1_4836:
        mov    edi, dword ptr [ebp + 8]
loc_1_4839:
        xor    ebx, ebx
loc_1_483B:
        mov    dword ptr [ebp - 4], 0
        cmp    word ptr [edi + 6], 0
        jbe    SHORT loc_1_4882
        lea    esi, [edi + 42h]
loc_1_484C:
        mov    cx, word ptr [esi]
        mov    ax, cx
        shr    ax, 8
        movzx  edx, ax
        cmp    edx, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_4873
        test   ebx, ebx
        je     SHORT loc_1_486A
        or     cl, 0ffh
        mov    word ptr [esi], cx
        jmp    SHORT loc_1_4873
loc_1_486A:
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_1_3D04
loc_1_4873:
        add    esi, 20h
        inc    dword ptr [ebp - 4]
        movzx  eax, word ptr [edi + 6]
        cmp    eax, dword ptr [ebp - 4]
        ja     SHORT loc_1_484C
loc_1_4882:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_1_488D:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, edx
        shl    eax, 5
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        xor    esi, esi
        mov    bl, byte ptr [eax + ecx + 459h]
        mov    byte ptr [eax + ecx + 458h], bl
        lea    eax, [eax + ecx + 442h]
        push   dword ptr [ebp + 10h]
        push   edx
        mov    word ptr [eax + 14h], si
        push   ecx
        mov    word ptr [eax], si
        cmp    edx, 9
        mov    word ptr [eax + 2], si
        jne    SHORT loc_1_48D1
        call   sub_1_48DC
        jmp    SHORT loc_1_48D6
loc_1_48D1:
        call   sub_1_49F5
loc_1_48D6:
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
sub_1_48DC:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        shl    eax, 5
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 8], 1
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        movzx  edx, byte ptr [eax + ebx + 458h]
        lea    edi, [eax + ebx + 442h]
        mov    esi, 80h
        or     dword ptr [ebp + 10h], esi
        test   edx, edx
        jne    SHORT loc_1_4913
        mov    edx, esi
loc_1_4913:
        mov    ecx, edx
loc_1_4915:
        cmp    ecx, 120h
        jae    NEAR PTR loc_1_49AB
loc_1_4921:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + ecx*4 + 662h]
        test   eax, eax
        je     SHORT loc_1_498F
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [eax]
        cmp    word ptr [eax], 0ffffh
        je     SHORT loc_1_498F
        add    eax, 2
loc_1_4942:
        cmp    word ptr [eax - 2], 0
        je     SHORT loc_1_4981
        cmp    byte ptr [eax + 1], dl
        jne    SHORT loc_1_4981
        movzx  ebx, byte ptr [eax]
        mov    dword ptr [ebp - 4], ebx
        cmp    ebx, dword ptr [ebp + 10h]
        ja     SHORT loc_1_4981
        cmp    esi, ebx
        ja     SHORT loc_1_4981
        mov    word ptr [edi + 14h], cx
        mov    esi, ebx
        mov    bx, word ptr [ebp - 0ch]
        mov    word ptr [edi], bx
        mov    bx, word ptr [ebp - 4]
        mov    word ptr [edi + 2], bx
        movzx  ebx, byte ptr [eax]
        cmp    ebx, dword ptr [ebp + 10h]
        je     SHORT loc_1_49DF
        mov    dword ptr [ebp - 8], 0
loc_1_4981:
        add    eax, 1ah
        inc    dword ptr [ebp - 0ch]
        cmp    word ptr [eax - 2], 0ffffh
        jne    SHORT loc_1_4942
loc_1_498F:
        cmp    ecx, 80h
        jbe    SHORT loc_1_499A
        inc    ecx
        jmp    SHORT loc_1_499F
loc_1_499A:
        mov    ecx, 81h
loc_1_499F:
        cmp    ecx, 120h
        jb     NEAR PTR loc_1_4921
loc_1_49AB:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_49BB
        test   edx, edx
        je     SHORT loc_1_49E3
        cmp    byte ptr [edi + 16h], 0
        jne    SHORT loc_1_49E3
loc_1_49BB:
        test   edx, edx
        je     SHORT loc_1_49E7
        cmp    edx, 80h
        jne    SHORT loc_1_49D0
        xor    edx, edx
        xor    ecx, ecx
        jmp    NEAR PTR loc_1_4915
loc_1_49D0:
        mov    edx, 80h
        mov    ecx, edx
        mov    dword ptr [ebp + 10h], edx
        jmp    NEAR PTR loc_1_4915
loc_1_49DF:
        xor    eax, eax
        jmp    SHORT loc_1_49EC
loc_1_49E3:
        xor    eax, eax
        jmp    SHORT loc_1_49EC
loc_1_49E7:
        mov    eax, 1
loc_1_49EC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
_LTEXT ends
        end

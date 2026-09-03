; pppmac_lcode_01_05.asm -- PPPMAC object 1 segment _LTEXT  (part 5/6)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_134D0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    eax, eax
        sub    esi, esi
        mov    edx, dword ptr [ebp + 8]
loc_1_134DB:
        mov    cl, byte ptr [edx]
        cmp    cl, 20h
        je     SHORT loc_1_134EC
        cmp    cl, 0ah
        je     SHORT loc_1_134EC
        cmp    cl, 9
        jne    SHORT loc_1_134F0
loc_1_134EC:
        inc    edx
        jmp    SHORT loc_1_134DB
        db  090h
loc_1_134F0:
        mov    cl, byte ptr [edx]
        cmp    cl, 2bh
        je     SHORT loc_1_134FC
        cmp    cl, 2dh
        jne    SHORT loc_1_13506
loc_1_134FC:
        sub    cl, 2bh
        cmp    cl, 1
        sbb    esi, esi
        inc    esi
        inc    edx
loc_1_13506:
        cmp    byte ptr [edx], 30h
        jb     SHORT loc_1_13525
loc_1_1350B:
        mov    cl, byte ptr [edx]
        cmp    cl, 39h
        ja     SHORT loc_1_13525
        and    ecx, 0ffh
        lea    eax, [eax + eax*4]
        inc    edx
        lea    eax, [ecx + eax*2 - 30h]
        cmp    byte ptr [edx], 30h
        jae    SHORT loc_1_1350B
loc_1_13525:
        or     esi, esi
        je     SHORT loc_1_1352B
        neg    eax
loc_1_1352B:
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_13530:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, esi
        jg     SHORT loc_1_13549
        mov    esi, 1
        neg    ebx
loc_1_13549:
        mov    eax, ebx
        or     ebx, ebx
        je     SHORT loc_1_1355D
        nop
loc_1_13550:
        mov    ecx, 0ah
        cdq
        idiv   ecx
        inc    esi
        or     eax, eax
        jne    SHORT loc_1_13550
loc_1_1355D:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [esi + ecx], 0
        lea    edi, [esi - 1]
loc_1_13567:
        mov    dword ptr [ebp - 8], 0ah
        mov    eax, ebx
        cdq
        idiv   dword ptr [ebp - 8]
        mov    edx, eax
        mov    byte ptr [ebp - 4], 0ah
        imul   byte ptr [ebp - 4]
        sub    bl, al
        add    bl, 30h
        mov    byte ptr [ecx + edi], bl
        dec    edi
        mov    ebx, edx
        or     edx, edx
        jne    SHORT loc_1_13567
        or     edi, edi
        jne    SHORT loc_1_13593
        mov    byte ptr [ecx], 2dh
loc_1_13593:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_135A0:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 30h
        jb     SHORT loc_1_135B8
        cmp    al, 39h
        ja     SHORT loc_1_135B8
        and    eax, 0ffh
        sub    eax, 30h
        pop    ebp
        ret
loc_1_135B8:
        cmp    al, 41h
        jb     SHORT loc_1_135CA
        cmp    al, 46h
        ja     SHORT loc_1_135CA
        and    eax, 0ffh
        sub    eax, 37h
        pop    ebp
        ret
loc_1_135CA:
        cmp    al, 61h
        jb     SHORT loc_1_135E0
        cmp    al, 66h
        ja     SHORT loc_1_135E0
        and    eax, 0ffh
        sub    eax, 57h
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_135E0:
        mov    eax, 0ffh
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_135F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0fh
        jbe    SHORT loc_1_13600
        mov    al, 58h
        pop    ebp
        ret
        db  090h
loc_1_13600:
        cmp    eax, 0ah
        jb     SHORT loc_1_13609
        add    al, 37h
        pop    ebp
        ret
loc_1_13609:
        add    al, 30h
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_1_13610:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    dword ptr [ebp - 4ch], 4ch
        mov    word ptr [ebp - 48h], 4
        mov    word ptr [ebp - 46h], 0fd80h
        mov    word ptr [ebp - 44h], 0
        mov    dword ptr [ebp - 42h], 16h
        mov    byte ptr [ebp - 3eh], 1
        mov    byte ptr [ebp - 3dh], 1
        mov    dword ptr [ebp - 3ch], 0
        mov    dword ptr [ebp - 38h], 0
        db  0C7h, 045h, 0CCh, 0F0h, 036h, 001h, 000h
        db  0C7h, 045h, 0D0h, 050h, 037h, 001h, 000h
        db  0C7h, 045h, 0D4h, 090h, 038h, 001h, 000h
        db  0C7h, 045h, 0D8h, 0B0h, 03Ah, 001h, 000h
        db  0C7h, 045h, 0DCh, 060h, 03Bh, 001h, 000h
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 090h, 03Bh, 001h, 000h
        xor    eax, eax
        db  0C7h, 045h, 0E8h, 0C0h, 03Bh, 001h, 000h
        db  0C7h, 045h, 0ECh, 030h, 03Ch, 001h, 000h
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        db  066h, 0A1h, 002h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], eax
        lea    eax, [ebp - 4ch]
        push   eax
        call   PPPMAC_Service_01
        add    esp, 4
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_136D0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax], 0
        mov    byte ptr [eax + 5], 0
        mov    dword ptr [eax + 1], 0
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_136F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [eax + 4bh]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   esi
        mov    word ptr [eax + 114h], 0
        add    esi, 0ch
        mov    word ptr [eax + 116h], 0
        mov    dword ptr [eax + 128h], 0
        mov    dword ptr [eax + 120h], 0
        mov    dword ptr [eax + 124h], 0
        mov    byte ptr [eax + 12ch], 0
        call   sub_1_136D0
        push   esi
        call   sub_1_136D0
        mov    eax, 800h
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_13750:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_13760:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        cmp    eax, 11h
        je     SHORT loc_1_137A2
        cmp    eax, 12h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 135h], 0
        je     SHORT loc_1_137D0
        mov    dword ptr [eax + 131h], 0
        mov    dword ptr [eax + 139h], 0
        mov    dword ptr [eax + 13dh], 0
        jmp    SHORT loc_1_137EE
loc_1_137A2:
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 080h, 035h, 001h, 000h, 000h, 030h, 076h, 001h, 000h
        db  0C7h, 080h, 031h, 001h, 000h, 000h, 0F0h, 075h, 001h, 000h
        db  0C7h, 080h, 039h, 001h, 000h, 000h, 060h, 076h, 001h, 000h
        db  0C7h, 080h, 03Dh, 001h, 000h, 000h, 060h, 075h, 001h, 000h
        jmp    SHORT loc_1_137EE
        db  090h
loc_1_137D0:
        db  0C7h, 080h, 031h, 001h, 000h, 000h, 0C0h, 04Eh, 001h, 000h
        db  0C7h, 080h, 039h, 001h, 000h, 000h, 0E0h, 04Eh, 001h, 000h
        db  0C7h, 080h, 03Dh, 001h, 000h, 000h, 060h, 043h, 001h, 000h
loc_1_137EE:
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        cmp    ecx, 11h
        je     SHORT loc_1_13830
        cmp    ecx, 12h
        mov    dword ptr [eax + 145h], 0
        je     SHORT loc_1_13860
        mov    dword ptr [eax + 141h], 0
        mov    dword ptr [eax + 149h], 0
        mov    dword ptr [eax + 14dh], 0
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_13830:
        db  0C7h, 080h, 045h, 001h, 000h, 000h, 050h, 076h, 001h, 000h
        db  0C7h, 080h, 041h, 001h, 000h, 000h, 010h, 076h, 001h, 000h
        db  0C7h, 080h, 049h, 001h, 000h, 000h, 0B0h, 076h, 001h, 000h
        db  0C7h, 080h, 04Dh, 001h, 000h, 000h, 0C0h, 076h, 001h, 000h
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_13860:
        db  0C7h, 080h, 041h, 001h, 000h, 000h, 0D0h, 04Eh, 001h, 000h
        db  0C7h, 080h, 049h, 001h, 000h, 000h, 020h, 04Fh, 001h, 000h
        db  0C7h, 080h, 04Dh, 001h, 000h, 000h, 040h, 04Fh, 001h, 000h
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_13890:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        lea    ecx, [esi + 4bh]
        mov    edi, dword ptr [eax + 0ch]
        lea    eax, [ecx + 6]
        add    ecx, 0ch
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        sub    dx, dx
        sub    bx, bx
        cmp    byte ptr [ecx + 5], dl
        je     SHORT loc_1_138D4
        mov    dword ptr [edi + 120h], 1
        mov    dx, 11h
        jmp    SHORT loc_1_138E6
loc_1_138D4:
        cmp    byte ptr [ecx], 0
        je     SHORT loc_1_138E6
        mov    edx, dword ptr [eax + 1]
        mov    dword ptr [edi + 120h], edx
        mov    dx, 12h
loc_1_138E6:
        cmp    byte ptr [eax + 5], 0
        je     SHORT loc_1_13900
        mov    dword ptr [edi + 124h], 1
        mov    bx, 11h
        jmp    SHORT loc_1_13912
        db  08Dh, 064h, 024h, 000h
loc_1_13900:
        cmp    byte ptr [ecx], 0
        je     SHORT loc_1_13912
        mov    eax, dword ptr [eax + 1]
        mov    dword ptr [edi + 124h], eax
        mov    bx, 12h
loc_1_13912:
        or     dx, dx
        je     NEAR PTR loc_1_13A83
        or     bx, bx
        je     NEAR PTR loc_1_13A83
        push   ebx
        push   edx
        push   edi
        call   sub_1_13760
        mov    eax, dword ptr [edi + 131h]
        or     eax, eax
        je     SHORT loc_1_1393C
        lea    ecx, [ebp - 8]
        push   ecx
        call   eax
loc_1_1393C:
        mov    eax, dword ptr [edi + 141h]
        or     eax, eax
        je     SHORT loc_1_1394C
        lea    ecx, [ebp - 4]
        push   ecx
        call   eax
loc_1_1394C:
        mov    eax, dword ptr [edi + 0feh]
        lea    ebx, [edi + 118h]
        test   byte ptr [eax + 0ch], 10h
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_1_13973
        add    eax, dword ptr [ebp - 8]
        add    eax, 8b6h
        jmp    SHORT loc_1_1397B
loc_1_13973:
        add    eax, dword ptr [ebp - 8]
        add    eax, 6b2h
loc_1_1397B:
        push   eax
        push   ebx
        call   sub_1_0756
        mov    ecx, dword ptr [ebx]
        or     ecx, ecx
        jne    SHORT loc_1_139A9
        mov    byte ptr [esi + 11h], 3
        mov    byte ptr [esi + 10h], 9
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_139A9:
        mov    eax, dword ptr [ebp - 8]
        add    eax, ecx
        mov    dword ptr [edi + 11ch], eax
        add    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edi + 0feh]
        mov    dword ptr [edi + 12dh], eax
        test   byte ptr [ecx + 0ch], 10h
        je     SHORT loc_1_13A38
        add    eax, 6b2h
        mov    dword ptr [edi + 151h], eax
        add    eax, 102h
        mov    dword ptr [edi + 155h], eax
        mov    word ptr [edi + 169h], 0
        push   8
        mov    eax, ecx
        add    eax, 40h
        push   eax
        lea    eax, [edi + 159h]
        push   eax
        call   sub_1_083C
        lea    esi, [edi + 161h]
        push   8
        mov    eax, dword ptr [edi + 0feh]
        add    eax, 40h
        push   eax
        push   esi
        call   sub_1_083C
        lea    eax, [edi + 159h]
        push   eax
        push   8
        mov    eax, dword ptr [edi + 151h]
        push   eax
        call   sub_1_17730
        push   esi
        push   8
        mov    eax, dword ptr [edi + 155h]
        push   eax
        call   sub_1_17730
loc_1_13A38:
        mov    eax, dword ptr [edi + 135h]
        or     eax, eax
        je     SHORT loc_1_13A47
        mov    ecx, dword ptr [ebx]
        push   ecx
        call   eax
loc_1_13A47:
        mov    eax, dword ptr [edi + 145h]
        or     eax, eax
        je     SHORT loc_1_13A5A
        mov    ecx, dword ptr [edi + 11ch]
        push   ecx
        call   eax
loc_1_13A5A:
        mov    ecx, dword ptr [edi + 139h]
        or     ecx, ecx
        je     SHORT loc_1_13A69
        mov    eax, dword ptr [ebx]
        push   eax
        call   ecx
loc_1_13A69:
        mov    ecx, dword ptr [edi + 149h]
        or     ecx, ecx
        je     SHORT loc_1_13A9D
        mov    eax, dword ptr [edi + 11ch]
        push   eax
        call   ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_13A83:
        mov    byte ptr [esi + 11h], 3
        mov    byte ptr [esi + 10h], 9
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_13A9D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_13AB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 120h], 0
        mov    dword ptr [esi + 124h], 0
        cmp    dword ptr [esi + 118h], 0
        je     SHORT loc_1_13B57
        lea    eax, [ebp - 4]
        push   eax
        call   dword ptr [esi + 131h]
        lea    eax, [ebp - 8]
        push   eax
        call   dword ptr [esi + 141h]
        mov    eax, dword ptr [esi + 0feh]
        push   0
        test   byte ptr [eax + 0ch], 10h
        mov    eax, dword ptr [ebp - 8]
        je     SHORT loc_1_13B10
        add    eax, dword ptr [ebp - 4]
        add    eax, 8b6h
        jmp    SHORT loc_1_13B18
        db  08Dh, 064h, 024h, 000h
loc_1_13B10:
        add    eax, dword ptr [ebp - 4]
        add    eax, 6b2h
loc_1_13B18:
        push   eax
        mov    eax, dword ptr [esi + 118h]
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 118h], 0
        mov    dword ptr [esi + 11ch], 0
        mov    dword ptr [esi + 12dh], 0
        mov    dword ptr [esi + 151h], 0
        mov    dword ptr [esi + 155h], 0
loc_1_13B57:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_13B60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        mov    byte ptr [eax], 1
        mov    byte ptr [eax + 5], 1
        mov    dword ptr [eax + 1], 1
        lea    edx, [eax + 6]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_13B90:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 51h
        sub    cx, cx
        cmp    byte ptr [eax + 5], cl
        je     SHORT loc_1_13BA5
        mov    cx, 5
loc_1_13BA5:
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_13BAE
        add    cx, 6
loc_1_13BAE:
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    word ptr [eax], cx
        mov    al, 1
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_13BC0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        add    edx, 51h
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    byte ptr [edx], 0
        je     SHORT loc_1_13C06
        mov    byte ptr [ecx], 12h
        inc    ecx
        mov    byte ptr [ecx], 6
        inc    ecx
        mov    esi, dword ptr [edx + 1]
        mov    edi, esi
        and    edi, 0ff00h
        mov    eax, esi
        shl    eax, 10h
        or     edi, eax
        shl    edi, 8
        mov    eax, esi
        and    eax, 0ff0000h
        shr    esi, 10h
        or     eax, esi
        shr    eax, 8
        or     edi, eax
        mov    dword ptr [ecx], edi
        add    ecx, 4
loc_1_13C06:
        cmp    byte ptr [edx + 5], 0
        je     SHORT loc_1_13C1D
        mov    byte ptr [ecx], 11h
        inc    ecx
        mov    byte ptr [ecx], 5
        inc    ecx
        mov    word ptr [ecx], 100h
        mov    byte ptr [ecx + 2], 4
loc_1_13C1D:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_13C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, 10h
        lea    eax, [esi + 4bh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 2fh]
        mov    edi, dword ptr [eax + 0ch]
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        cmp    edx, 2
        jl     SHORT loc_1_13C6C
        cmp    edx, 4
        jle    SHORT loc_1_13C60
        cmp    edx, 6
        jne    SHORT loc_1_13C6C
loc_1_13C60:
        mov    al, byte ptr [ecx + 1]
        cmp    byte ptr [esi + 15h], al
        jne    NEAR PTR loc_1_13D9A
loc_1_13C6C:
        dec    edx
        cmp    edx, 5
        ja     SHORT loc_1_13C79
        db  0FFh, 024h, 095h, 0A4h, 03Dh, 001h, 000h
loc_1_13C79:
        mov    bl, 0ch
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_13C85:
        mov    eax, dword ptr [edi + 0feh]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        lea    eax, [esi + 39h]
        push   eax
        mov    eax, dword ptr [esi + 35h]
        push   eax
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_13DC0
        or     al, al
        je     SHORT loc_1_13CB6
        mov    bl, 6
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_13CB6:
        mov    bl, 7
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_13CC2:
        mov    eax, dword ptr [edi + 0feh]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_140C0
        or     al, al
        je     NEAR PTR loc_1_13D9A
        mov    bl, 8
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_13CF0:
        mov    eax, dword ptr [edi + 0feh]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_14190
        or     al, al
        je     NEAR PTR loc_1_13D9A
        mov    bl, 9
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_13D20:
        mov    eax, dword ptr [edi + 0feh]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_14280
        or     al, al
        je     SHORT loc_1_13D9A
        mov    bl, 9
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_13D50:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_13D76
        xor    eax, eax
        db  066h, 0A1h, 0D2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0C1h, 0DEh, 0FCh, 0FFh
        add    esp, 8
loc_1_13D76:
        mov    byte ptr [esi + 11h], 0ah
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    bl, 5
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_13D98:
        mov    bl, 0bh
loc_1_13D9A:
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  085h, 03Ch, 001h, 000h, 0C2h, 03Ch, 001h, 000h, 0F0h, 03Ch, 001h, 000h, 020h, 03Dh, 001h, 000h
        db  050h, 03Dh, 001h, 000h, 098h, 03Dh, 001h, 000h, 08Dh, 064h, 024h, 000h
sub_1_13DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        lea    ecx, [eax + 0ch]
        mov    dword ptr [ebp - 14h], ecx
        lea    edi, [esi + 4]
        mov    ax, word ptr [ebp + 10h]
        sub    ax, 4
        mov    word ptr [ebp - 6], ax
        mov    byte ptr [ebp - 1], 2
        mov    word ptr [ebp - 8], 0
        push   ecx
        call   sub_1_136D0
        mov    edx, dword ptr [ebp + 14h]
        mov    al, byte ptr [esi + 1]
        mov    byte ptr [edx + 1], al
        mov    eax, edx
        add    eax, 4
        mov    dword ptr [ebp - 18h], eax
        mov    esi, eax
        cmp    word ptr [ebp - 6], 0
        je     SHORT loc_1_13E88
        mov    edi, edi
loc_1_13E10:
        mov    al, byte ptr [edi]
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [edi + 1]
        inc    edi
        mov    byte ptr [ebp - 2], al
        inc    edi
        and    ax, 0ffh
        mov    word ptr [ebp - 10h], ax
        mov    ebx, eax
        and    ebx, 0ffffh
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, ebx
        jb     NEAR PTR loc_1_140A9
        mov    cl, byte ptr [ebp - 3]
        and    ecx, 0ffff00ffh
        mov    eax, ecx
        and    eax, 0ffffh
        cmp    eax, 11h
        je     SHORT loc_1_13EC7
        cmp    eax, 12h
        je     NEAR PTR loc_1_13F50
loc_1_13E59:
        cmp    byte ptr [ebp - 1], 4
        je     SHORT loc_1_13E66
        mov    esi, dword ptr [ebp - 18h]
        mov    byte ptr [ebp - 1], 4
loc_1_13E66:
        sub    edi, 2
        push   ebx
        push   edi
        push   esi
        call   sub_1_083C
        add    esi, ebx
        add    edi, ebx
loc_1_13E75:
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 6], ax
        cmp    word ptr [ebp - 6], 0
        jne    SHORT loc_1_13E10
loc_1_13E88:
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_13E97
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        call   sub_1_136D0
loc_1_13E97:
        mov    eax, dword ptr [ebp + 18h]
        sub    esi, dword ptr [ebp + 14h]
        mov    word ptr [eax], si
        mov    edx, dword ptr [ebp + 14h]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx], cl
        mov    ax, word ptr [eax]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 2], cx
        mov    al, byte ptr [ebp - 1]
        sub    al, 2
        cmp    al, 1
        sbb    al, al
        neg    al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_13EC7:
        cmp    word ptr [ebp - 8], 0
        jne    SHORT loc_1_13E59
        mov    word ptr [ebp - 8], cx
        cmp    byte ptr [ebp - 2], 5
        jne    SHORT loc_1_13E59
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_13EE4
        lea    edi, [ebx + edi - 2]
        jmp    SHORT loc_1_13E75
loc_1_13EE4:
        mov    ax, word ptr [edi]
        mov    ch, al
        mov    cl, ah
        mov    al, byte ptr [edi + 2]
        cmp    cx, 1
        jne    SHORT loc_1_13EFA
        cmp    al, 4
        mov    al, 1
        je     SHORT loc_1_13EFC
loc_1_13EFA:
        sub    al, al
loc_1_13EFC:
        or     al, al
        jne    SHORT loc_1_13F17
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_13F13
        mov    esi, dword ptr [ebp - 18h]
loc_1_13F09:
        or     al, al
        jne    SHORT loc_1_13F26
loc_1_13F0D:
        mov    byte ptr [ebp - 1], 3
        jmp    SHORT loc_1_13F2D
loc_1_13F13:
        or     al, al
        je     SHORT loc_1_13F0D
loc_1_13F17:
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_1_13F09
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_13E75
loc_1_13F26:
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 5], 1
loc_1_13F2D:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [esi], al
        inc    esi
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [esi], al
        inc    esi
        lea    edi, [ebx + edi - 2]
        mov    word ptr [esi], 100h
        add    esi, 2
        mov    byte ptr [esi], 4
        inc    esi
        jmp    NEAR PTR loc_1_13E75
        db  08Bh, 0FFh
loc_1_13F50:
        cmp    word ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_13E59
        mov    word ptr [ebp - 8], cx
        cmp    byte ptr [ebp - 2], 6
        jne    NEAR PTR loc_1_13E59
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_13F78
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_13E75
loc_1_13F78:
        mov    eax, dword ptr [edi]
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    eax, 10h
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 10h
        mov    dword ptr [ebp - 1ch], eax
        or     eax, eax
        je     SHORT loc_1_13FB5
        mov    eax, ecx
        and    eax, 0ffffffeeh
        jmp    SHORT loc_1_13FBB
loc_1_13FB5:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0fffffffeh
loc_1_13FBB:
        or     eax, eax
        je     SHORT loc_1_1400D
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_14009
        mov    esi, dword ptr [ebp - 18h]
loc_1_13FC8:
        or     eax, eax
        je     SHORT loc_1_14030
loc_1_13FCC:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_13FF6
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0D4h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 041h, 0DCh, 0FCh, 0FFh
        add    esp, 0ch
loc_1_13FF6:
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_1_14020
        mov    dword ptr [ebp - 0ch], 11h
        mov    byte ptr [ebp - 1], 3
        jmp    SHORT loc_1_14066
loc_1_14009:
        or     eax, eax
        jne    SHORT loc_1_13FCC
loc_1_1400D:
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_1_13FC8
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_13E75
        db  08Dh, 064h, 024h, 000h
loc_1_14020:
        mov    dword ptr [ebp - 0ch], 1
        mov    byte ptr [ebp - 1], 3
        jmp    SHORT loc_1_14066
        db  08Dh, 049h, 000h
loc_1_14030:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_1405A
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0D6h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0DDh, 0DBh, 0FCh, 0FFh
        add    esp, 0ch
loc_1_1405A:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 1], eax
        mov    byte ptr [ecx], 1
loc_1_14066:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [esi], al
        inc    esi
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [esi], al
        inc    esi
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ff00h
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ff0000h
        mov    edx, dword ptr [ebp - 0ch]
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [esi], ecx
        add    esi, 4
        lea    edi, [ebx + edi - 2]
        jmp    NEAR PTR loc_1_13E75
loc_1_140A9:
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    word ptr [eax], 0
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h
sub_1_140C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 6
        sub    al, al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        mov    di, word ptr [ebp + 10h]
        sub    di, 4
        mov    cl, byte ptr [esi]
        cmp    cl, al
        jne    SHORT loc_1_140F0
        cmp    byte ptr [esi + 5], al
        jne    SHORT loc_1_140F0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_140F0:
        or     di, di
        je     NEAR PTR loc_1_14185
        mov    ebx, edx
        inc    edx
        mov    bl, byte ptr [ebx]
        and    ebx, 0ffh
        cmp    ebx, 11h
        je     SHORT loc_1_14115
        cmp    ebx, 12h
        je     SHORT loc_1_14142
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_14115:
        cmp    byte ptr [esi + 5], 0
        je     SHORT loc_1_14185
        cmp    di, 5
        jb     SHORT loc_1_14185
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 5
        jne    SHORT loc_1_14185
        cmp    word ptr [edx], 100h
        jne    SHORT loc_1_14185
        cmp    byte ptr [edx + 2], 4
        jne    SHORT loc_1_14185
        mov    byte ptr [esi], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_14142:
        or     cl, cl
        je     SHORT loc_1_14185
        cmp    di, 6
        jb     SHORT loc_1_14185
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 6
        jne    SHORT loc_1_14185
        mov    ecx, dword ptr [esi + 1]
        mov    ebx, ecx
        and    ebx, 0ff00h
        mov    edi, ecx
        shl    edi, 10h
        or     ebx, edi
        shl    ebx, 8
        mov    edi, ecx
        and    edi, 0ff0000h
        shr    ecx, 10h
        or     edi, ecx
        shr    edi, 8
        or     ebx, edi
        cmp    ebx, dword ptr [edx]
        jne    SHORT loc_1_14185
        mov    byte ptr [esi + 5], 0
        mov    al, 1
loc_1_14185:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_1_14190:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 4
        mov    dx, word ptr [ebp + 10h]
        sub    dx, 4
        je     NEAR PTR loc_1_14264
loc_1_141B0:
        xor    eax, eax
        mov    al, byte ptr [ecx]
        cmp    eax, 11h
        je     SHORT loc_1_141C7
        cmp    eax, 12h
        je     SHORT loc_1_141E0
loc_1_141BE:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_141C7:
        cmp    byte ptr [ecx + 1], 5
        jne    SHORT loc_1_141BE
        mov    byte ptr [esi + 5], 0
        add    ecx, 5
        sub    dx, 5
        jmp    NEAR PTR loc_1_1425B
        db  08Dh, 049h, 000h
loc_1_141E0:
        cmp    byte ptr [ecx + 1], 6
        jne    SHORT loc_1_141BE
        mov    eax, dword ptr [ecx + 2]
        add    ecx, 2
        mov    edi, eax
        and    edi, 0ff00h
        mov    ebx, eax
        shl    ebx, 10h
        or     edi, ebx
        shl    edi, 8
        mov    ebx, eax
        and    ebx, 0ff0000h
        shr    eax, 10h
        or     ebx, eax
        shr    ebx, 8
        or     edi, ebx
        test   byte ptr [ebp + 14h], 10h
        je     SHORT loc_1_14242
        test   edi, 11h
        jne    SHORT loc_1_14223
        mov    byte ptr [esi], 0
        jmp    SHORT loc_1_1422F
loc_1_14223:
        test   edi, 1
        jne    SHORT loc_1_1422F
        and    dword ptr [esi + 1], 0fffffffeh
loc_1_1422F:
        mov    eax, dword ptr [esi + 1]
        and    eax, 0ffffffefh
        mov    dword ptr [esi + 1], eax
        and    edi, 10h
        or     edi, eax
        mov    dword ptr [esi + 1], edi
        jmp    SHORT loc_1_14254
loc_1_14242:
        test   edi, 1
        jne    SHORT loc_1_14254
        mov    dword ptr [esi + 1], 0
        mov    byte ptr [esi], 0
loc_1_14254:
        add    ecx, 4
        sub    dx, 6
loc_1_1425B:
        or     dx, dx
        jne    NEAR PTR loc_1_141B0
loc_1_14264:
        or     dx, dx
        jne    NEAR PTR loc_1_141BE
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_14280:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        add    edx, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 4
        mov    ax, word ptr [ebp + 10h]
        sub    ax, 4
        je     SHORT loc_1_142D0
        xor    eax, eax
        mov    al, byte ptr [ecx]
        cmp    eax, 11h
        je     SHORT loc_1_142B8
        cmp    eax, 12h
        jne    SHORT loc_1_142C8
        cmp    byte ptr [edx], 0
        je     SHORT loc_1_142B5
        cmp    byte ptr [ecx + 1], 6
        jne    SHORT loc_1_142C8
        mov    byte ptr [edx], 0
loc_1_142B5:
        add    ecx, 6
loc_1_142B8:
        cmp    byte ptr [edx + 5], 0
        je     SHORT loc_1_142C8
        cmp    byte ptr [ecx + 1], 5
        jne    SHORT loc_1_142C8
        mov    byte ptr [edx + 5], 0
loc_1_142C8:
        sub    al, al
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_1_142D0:
        or     ax, ax
        jne    SHORT loc_1_142C8
        mov    al, 1
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_1_142E0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   4
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   PPPMAC_Service_02
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_14357
        mov    eax, dword ptr [esi + 1ch]
        add    eax, dword ptr [esi + 20h]
        mov    word ptr [esi + 28h], 0fd80h
        mov    byte ptr [eax], 0eh
        inc    eax
        mov    cl, byte ptr [edi + 12ch]
        mov    byte ptr [eax], cl
        inc    eax
        inc    byte ptr [edi + 12ch]
        mov    byte ptr [eax], 0
        mov    byte ptr [eax + 1], 4
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_1434F
        xor    eax, eax
        mov    al, byte ptr [edi + 12ch]
        dec    eax
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0D8h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0E8h, 0D8h, 0FCh, 0FFh
        add    esp, 0ch
loc_1_1434F:
        push   0
        push   esi
        call   sub_1_4360
loc_1_14357:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_14360:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ebp - 4], 0
        mov    eax, dword ptr [ecx + 2001h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        add    ecx, eax
        cmp    ecx, 1ffeh
        jae    SHORT loc_1_14389
        or     eax, eax
        jne    SHORT loc_1_1439A
loc_1_14389:
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ebp - 4], 40h
        mov    dword ptr [ecx + 2001h], 0
loc_1_1439A:
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 2009h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 44h], eax
        mov    eax, dword ptr [ecx]
        push   eax
        sub    ebx, ebx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 44h]
        push   eax
        call   sub_1_083C
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, 10h
        mov    eax, dword ptr [ecx + 2001h]
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 44h]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [ecx]
        mov    eax, dword ptr [ebp - 44h]
        lea    eax, [ecx + eax - 1]
        mov    dword ptr [ebp - 20h], eax
        sub    eax, 2
        mov    dword ptr [ebp - 40h], eax
        cmp    eax, dword ptr [ebp - 44h]
        jbe    NEAR PTR loc_1_14D52
loc_1_143F4:
        mov    ecx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [ebp - 3], cl
        mov    byte ptr [eax], cl
        mov    ecx, dword ptr [ebp - 8]
        xor    eax, eax
        mov    cl, byte ptr [ecx]
        mov    byte ptr [ebp - 2], cl
        mov    ecx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 10h]
        mov    cl, byte ptr [ecx + 1]
        mov    al, cl
        mov    byte ptr [ebp - 1], cl
        movzx  edi, byte ptr [ebp - 3]
        db  08Bh, 004h, 085h, 048h, 094h, 001h, 000h
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 2]
        db  003h, 004h, 08Dh, 048h, 090h, 001h, 000h
        db  003h, 004h, 0BDh, 048h, 08Ch, 001h, 000h
        and    eax, 0fff000h
        mov    ecx, dword ptr [ebp + 10h]
        shr    eax, 0ch
        lea    eax, [ecx + eax*2 + 2649h]
        mov    dword ptr [ebp - 38h], eax
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 30h], eax
        cmp    eax, 1
        je     SHORT loc_1_1447D
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 38h]
        sub    dword ptr [ebp - 34h], ecx
        mov    cx, word ptr [ebp - 34h]
        mov    word ptr [eax], cx
loc_1_1447D:
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ecx + 2005h], eax
        jae    SHORT loc_1_14491
        mov    dword ptr [ecx + 2005h], eax
loc_1_14491:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    eax, dword ptr [ebp + 10h]
        je     NEAR PTR loc_1_14D07
        mov    cl, byte ptr [ebp - 3]
        cmp    byte ptr [eax - 1], cl
        jne    NEAR PTR loc_1_14D07
        mov    cl, byte ptr [ebp - 2]
        cmp    byte ptr [eax], cl
        jne    NEAR PTR loc_1_14D07
        mov    cl, byte ptr [ebp - 1]
        cmp    byte ptr [eax + 1], cl
        jne    NEAR PTR loc_1_14D07
        cmp    dword ptr [ebp - 30h], 1
        je     NEAR PTR loc_1_14D07
        cmp    eax, dword ptr [ebp - 10h]
        je     NEAR PTR loc_1_14D07
        mov    ecx, dword ptr [ebp + 10h]
        inc    eax
        cmp    dword ptr [ecx + 2005h], eax
        jb     NEAR PTR loc_1_14D07
        mov    eax, dword ptr [ebp - 30h]
        and    eax, 1fffh
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [ebp - 2]
        mov    byte ptr [eax], cl
        mov    edi, 3
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 10h]
        add    dword ptr [ebp - 0ch], 2
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 8], 2
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        cmp    byte ptr [ecx], al
        jne    SHORT loc_1_14556
loc_1_14520:
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 20h], ecx
        jbe    SHORT loc_1_14556
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ecx + 2005h], eax
        jb     SHORT loc_1_14556
        mov    ecx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0ch]
        inc    edi
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [ecx]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        cmp    byte ptr [ecx], al
        je     SHORT loc_1_14520
loc_1_14556:
        cmp    dword ptr [ebp - 14h], 140h
        jl     SHORT loc_1_14590
        sub    dword ptr [ebp - 14h], 140h
        lea    ecx, [edx - 8]
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0c000h
        mov    dword ptr [ebp - 2ch], eax
        sar    dword ptr [ebp - 2ch], 8
        shl    dword ptr [ebp - 2ch], cl
        or     ebx, dword ptr [ebp - 2ch]
        mov    byte ptr [esi], bh
        inc    esi
        shl    ebx, 8
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_14620
        db  08Dh, 064h, 024h, 000h
loc_1_14590:
        cmp    dword ptr [ebp - 14h], 40h
        jge    SHORT loc_1_145D2
        add    dword ptr [ebp - 14h], 3c0h
        cmp    edx, 0ah
        jle    SHORT loc_1_145B6
        sub    edx, 0ah
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    SHORT loc_1_14627
loc_1_145B6:
        sub    edx, 2
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 28h], eax
        sar    dword ptr [ebp - 28h], 8
        mov    cl, dl
        shl    dword ptr [ebp - 28h], cl
        or     ebx, dword ptr [ebp - 28h]
        cmp    edx, 9
        jge    SHORT loc_1_14615
        jmp    SHORT loc_1_1460C
loc_1_145D2:
        add    dword ptr [ebp - 14h], 0dc0h
        cmp    edx, 0ch
        jle    SHORT loc_1_145F2
        sub    edx, 0ch
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    SHORT loc_1_14627
loc_1_145F2:
        sub    edx, 4
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 24h], eax
        sar    dword ptr [ebp - 24h], 8
        mov    cl, dl
        shl    dword ptr [ebp - 24h], cl
        or     ebx, dword ptr [ebp - 24h]
        cmp    edx, 9
        jge    SHORT loc_1_14615
loc_1_1460C:
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_14615:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffh
        lea    ecx, [edx - 8]
loc_1_14620:
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
loc_1_14627:
        shl    ebx, 8
        lea    eax, [edi - 3]
        cmp    eax, 1ch
        ja     SHORT loc_1_14639
        db  0FFh, 024h, 085h, 040h, 04Eh, 001h, 000h
loc_1_14639:
        cmp    edi, 40h
        jge    SHORT loc_1_14680
        sub    edx, 4
        mov    eax, 0fh
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_1465A
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_1465A:
        sub    edx, 6
        sub    edi, 20h
        mov    cl, dl
        shl    edi, cl
        or     ebx, edi
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_14680:
        cmp    edi, 80h
        jge    SHORT loc_1_146C4
        sub    edx, 5
        mov    eax, 1fh
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_146A4
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_146A4:
        sub    edx, 7
        sub    edi, 40h
        mov    cl, dl
        shl    edi, cl
        or     ebx, edi
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_146C4:
        cmp    edi, 100h
        jge    SHORT loc_1_146F3
        sub    edx, 6
        mov    eax, 3fh
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_146E8
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_146E8:
        sub    edi, 80h
        jmp    NEAR PTR loc_1_14D39
loc_1_146F3:
        cmp    edi, 200h
        jge    SHORT loc_1_14747
        sub    edx, 7
        mov    eax, 7fh
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_14717
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_14717:
        sub    edi, 100h
        cmp    edx, 9
        jle    SHORT loc_1_14732
        lea    ecx, [edx - 9]
        shl    edi, cl
        or     ebx, edi
        mov    byte ptr [esi], bh
        inc    esi
        dec    edx
        jmp    NEAR PTR loc_1_14D43
loc_1_14732:
        mov    edx, 10h
        or     ebx, edi
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        inc    esi
        inc    esi
        sub    ebx, ebx
        jmp    NEAR PTR loc_1_14D46
loc_1_14747:
        cmp    edi, 400h
        jge    SHORT loc_1_147A7
        mov    eax, 0ffh
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
        shl    ebx, 8
        sub    edi, 200h
        cmp    edx, 0ah
        jle    SHORT loc_1_14780
        sub    edx, 0ah
        mov    cl, dl
        shl    edi, cl
        or     ebx, edi
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14780:
        sub    edx, 2
        mov    eax, edi
        sar    eax, 8
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_1479C
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_1479C:
        and    edi, 0ffh
        jmp    NEAR PTR loc_1_14D39
loc_1_147A7:
        cmp    edi, 800h
        jge    NEAR PTR loc_1_14846
        cmp    edx, 9
        jle    SHORT loc_1_147D0
        mov    eax, 1ffh
        lea    ecx, [edx - 9]
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
        dec    edx
        shl    ebx, 8
        jmp    SHORT loc_1_147E4
        db  08Dh, 049h, 000h
loc_1_147D0:
        mov    edx, 10h
        or     ebx, 1ffh
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        inc    esi
        inc    esi
        sub    ebx, ebx
loc_1_147E4:
        sub    edi, 400h
        cmp    edx, 0bh
        jle    SHORT loc_1_14803
        sub    edx, 0bh
        mov    cl, dl
        shl    edi, cl
        or     ebx, edi
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14803:
        jne    SHORT loc_1_14820
        mov    edx, 10h
        or     ebx, edi
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        inc    esi
        inc    esi
        sub    ebx, ebx
        jmp    NEAR PTR loc_1_14D46
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_14820:
        mov    ecx, 0bh
        sub    ecx, edx
        mov    eax, edi
        sar    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        inc    esi
        inc    esi
        mov    edx, 10h
        sub    edx, ecx
        mov    cl, dl
        shl    edi, cl
        mov    ebx, edi
        jmp    NEAR PTR loc_1_14D46
loc_1_14846:
        cmp    edi, 1000h
        jge    NEAR PTR loc_1_148E7
        cmp    edx, 0ah
        jle    SHORT loc_1_14870
        sub    edx, 0ah
        mov    eax, 3ffh
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    SHORT loc_1_1489B
        db  08Dh, 049h, 000h
loc_1_14870:
        sub    edx, 2
        mov    eax, 3
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_1488C
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_1488C:
        mov    eax, 0ffh
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
loc_1_1489B:
        shl    ebx, 8
        sub    edi, 800h
        cmp    edx, 0ch
        jle    SHORT loc_1_148C0
        sub    edx, 0ch
        mov    cl, dl
        shl    edi, cl
        or     ebx, edi
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
        db  08Dh, 049h, 000h
loc_1_148C0:
        sub    edx, 4
        mov    eax, edi
        sar    eax, 8
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_148DC
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_148DC:
        and    edi, 0ffh
        jmp    NEAR PTR loc_1_14D39
loc_1_148E7:
        cmp    edi, 2000h
        jge    NEAR PTR loc_1_14997
        cmp    edx, 0bh
        jle    SHORT loc_1_14911
        sub    edx, 0bh
        mov    eax, 7ffh
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
        jmp    SHORT loc_1_14950
loc_1_14911:
        jne    SHORT loc_1_14929
        mov    edx, 10h
        or     ebx, 7ffh
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        inc    esi
        inc    esi
        sub    ebx, ebx
        jmp    SHORT loc_1_14950
loc_1_14929:
        mov    ecx, 0bh
        sub    ecx, edx
        mov    eax, 7ffh
        sar    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        inc    esi
        inc    esi
        mov    edx, 10h
        sub    edx, ecx
        mov    ebx, 7ffh
        mov    cl, dl
        shl    ebx, cl
loc_1_14950:
        sub    edi, 1000h
        cmp    edx, 0dh
        jle    SHORT loc_1_14970
        sub    edx, 0dh
        mov    cl, dl
        shl    edi, cl
        or     ebx, edi
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
        db  090h
loc_1_14970:
        sub    edx, 5
        mov    eax, edi
        sar    eax, 8
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_1498C
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_1498C:
        and    edi, 0ffh
        jmp    NEAR PTR loc_1_14D39
loc_1_14997:
        cmp    edx, 0ch
        jle    SHORT loc_1_149B2
        sub    edx, 0ch
        mov    eax, 0fffh
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    SHORT loc_1_149DD
loc_1_149B2:
        sub    edx, 4
        mov    eax, 0fh
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_149CE
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_149CE:
        mov    eax, 0ffh
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
loc_1_149DD:
        shl    ebx, 8
        sub    edi, 2000h
        cmp    edx, 0eh
        jle    SHORT loc_1_14A00
        sub    edx, 0eh
        mov    cl, dl
        shl    edi, cl
        or     ebx, edi
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
        db  090h
loc_1_14A00:
        sub    edx, 6
        mov    eax, edi
        sar    eax, 8
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    SHORT loc_1_14A1C
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        shl    ebx, 8
loc_1_14A1C:
        and    edi, 0ffh
        jmp    NEAR PTR loc_1_14D39
loc_1_14A27:
        cmp    edx, 0ah
        jge    SHORT loc_1_14A40
        mov    byte ptr [esi], bh
        inc    esi
        shl    ebx, 8
        mov    edx, 10h
        jmp    NEAR PTR loc_1_14D46
        db  08Dh, 064h, 024h, 000h
loc_1_14A40:
        dec    edx
        jmp    NEAR PTR loc_1_14D46
loc_1_14A46:
        sub    edx, 4
        mov    eax, 8
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14A68:
        sub    edx, 4
        mov    eax, 9
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_14A90:
        sub    edx, 4
        mov    eax, 0ah
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14AB2:
        sub    edx, 4
        mov    eax, 0bh
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14AD4:
        sub    edx, 6
        mov    eax, 30h
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14AF6:
        sub    edx, 6
        mov    eax, 31h
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14B18:
        sub    edx, 6
        mov    eax, 32h
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_14B40:
        sub    edx, 6
        mov    eax, 33h
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14B62:
        sub    edx, 6
        mov    eax, 34h
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14B84:
        sub    edx, 6
        mov    eax, 35h
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14BA6:
        sub    edx, 6
        mov    eax, 36h
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
loc_1_14BC8:
        sub    edx, 6
        mov    eax, 37h
        mov    cl, dl
        shl    eax, cl
        or     ebx, eax
        cmp    edx, 9
        jge    NEAR PTR loc_1_14D46
        mov    byte ptr [esi], bh
        inc    esi
        add    edx, 8
        jmp    NEAR PTR loc_1_14D43
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_14BF0:
        mov    eax, 0e0h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14C01:
        mov    eax, 0e1h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14C12:
        mov    eax, 0e2h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14C23:
        mov    eax, 0e3h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14C34:
        mov    eax, 0e4h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14C45:
        mov    eax, 0e5h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14C56:
        mov    eax, 0e6h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14C67:
        mov    eax, 0e7h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14C78:
        mov    eax, 0e8h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_14C90:
        mov    eax, 0e9h
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14CA1:
        mov    eax, 0eah
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14CB2:
        mov    eax, 0ebh
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14CC3:
        mov    eax, 0ech
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14CD4:
        mov    eax, 0edh
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14CE5:
        mov    eax, 0eeh
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14CF6:
        mov    eax, 0efh
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        jmp    NEAR PTR loc_1_14D40
loc_1_14D07:
        test   edi, 80h
        je     SHORT loc_1_14D39
        add    edi, 80h
        cmp    edx, 9
        jle    SHORT loc_1_14D27
        lea    ecx, [edx - 9]
        shl    edi, cl
        or     ebx, edi
        mov    byte ptr [esi], bh
        inc    esi
        dec    edx
        jmp    SHORT loc_1_14D43
loc_1_14D27:
        mov    edx, 10h
        or     ebx, edi
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        inc    esi
        inc    esi
        sub    ebx, ebx
        jmp    SHORT loc_1_14D46
loc_1_14D39:
        lea    ecx, [edx - 8]
        shl    edi, cl
        or     ebx, edi
loc_1_14D40:
        mov    byte ptr [esi], bh
        inc    esi
loc_1_14D43:
        shl    ebx, 8
loc_1_14D46:
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 40h], ecx
        ja     NEAR PTR loc_1_143F4
loc_1_14D52:
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 20h], ecx
        jb     NEAR PTR loc_1_14DB9
        mov    edi, edi
loc_1_14D60:
        mov    ecx, dword ptr [ebp - 8]
        xor    eax, eax
        mov    al, byte ptr [ecx]
        test   al, 80h
        je     SHORT loc_1_14D94
        add    eax, 80h
        cmp    edx, 9
        jle    SHORT loc_1_14D82
        lea    ecx, [edx - 9]
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
        dec    edx
        jmp    SHORT loc_1_14D9E
loc_1_14D82:
        mov    edx, 10h
        or     ebx, eax
        mov    byte ptr [esi], bh
        mov    byte ptr [esi + 1], bl
        inc    esi
        inc    esi
        sub    ebx, ebx
        jmp    SHORT loc_1_14DA1
loc_1_14D94:
        lea    ecx, [edx - 8]
        shl    eax, cl
        or     ebx, eax
        mov    byte ptr [esi], bh
        inc    esi
loc_1_14D9E:
        shl    ebx, 8
loc_1_14DA1:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [ecx]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 8]
        inc    dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 20h], ecx
        jae    SHORT loc_1_14D60
loc_1_14DB9:
        cmp    edx, 10h
        je     SHORT loc_1_14DC1
        mov    byte ptr [esi], bh
        inc    esi
loc_1_14DC1:
        mov    ecx, dword ptr [ebp + 0ch]
        sub    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        jbe    NEAR PTR loc_1_14E19
        push   eax
        mov    eax, dword ptr [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_083C
        mov    edi, dword ptr [ebp + 10h]
        sub    eax, eax
        mov    ecx, 800h
        rep stosd
        stosb
        mov    edi, dword ptr [ebp + 10h]
        add    edi, 2649h
        mov    ecx, 800h
        rep stosd
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ebp - 4], 80h
        mov    dword ptr [ecx + 2001h], 2001h
        mov    al, byte ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_14E19:
        mov    ecx, dword ptr [ebp + 0ch]
        or     byte ptr [ebp - 4], 20h
        mov    dword ptr [ecx], esi
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 10h]
        sub    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 2001h], eax
        mov    al, byte ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 027h, 04Ah, 001h, 000h, 046h, 04Ah, 001h, 000h, 068h, 04Ah, 001h, 000h, 090h
        db  04Ah, 001h, 000h, 0B2h, 04Ah, 001h, 000h, 0D4h, 04Ah, 001h, 000h, 0F6h, 04Ah, 001h, 000h, 018h
        db  04Bh, 001h, 000h, 040h, 04Bh, 001h, 000h, 062h, 04Bh, 001h, 000h, 084h, 04Bh, 001h, 000h, 0A6h
        db  04Bh, 001h, 000h, 0C8h, 04Bh, 001h, 000h, 0F0h, 04Bh, 001h, 000h, 001h, 04Ch, 001h, 000h, 012h
        db  04Ch, 001h, 000h, 023h, 04Ch, 001h, 000h, 034h, 04Ch, 001h, 000h, 045h, 04Ch, 001h, 000h, 056h
        db  04Ch, 001h, 000h, 067h, 04Ch, 001h, 000h, 078h, 04Ch, 001h, 000h, 090h, 04Ch, 001h, 000h, 0A1h
        db  04Ch, 001h, 000h, 0B2h, 04Ch, 001h, 000h, 0C3h, 04Ch, 001h, 000h, 0D4h, 04Ch, 001h, 000h, 0E5h
        db  04Ch, 001h, 000h, 0F6h, 04Ch, 001h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h
loc_1_14EC0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 4649h
        ret    4
loc_1_14ED0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 2005h
        ret    4
loc_1_14EE0:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    dword ptr [edx + 2001h], 0
        mov    dword ptr [edx + 2005h], 0
        lea    edi, [edx + 2649h]
        mov    ecx, 800h
        rep stosd
        mov    edi, edx
        mov    ecx, 800h
        rep stosd
        stosb
        pop    edi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_14F20:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    dword ptr [edi + 2001h], edi
        mov    ecx, 800h
        rep stosd
        stosb
        pop    edi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_14F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, 10h
        mov    edx, dword ptr [ebp + 8]
        add    eax, edx
        mov    dword ptr [ebp - 24h], eax
        lea    esi, [edx + 1]
        xor    eax, eax
        mov    al, byte ptr [edx]
        shl    eax, 8
        xor    edx, edx
        mov    dl, byte ptr [esi]
        add    edx, eax
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_14F80
        mov    edi, dword ptr [ebp + 1ch]
        mov    eax, edi
        mov    dword ptr [ebp - 0ch], edi
        mov    dword ptr [eax + 2001h], eax
        jmp    SHORT loc_1_14F8C
        db  090h
loc_1_14F80:
        mov    eax, dword ptr [ebp + 1ch]
        mov    edi, dword ptr [eax + 2001h]
        mov    dword ptr [ebp - 0ch], edi
loc_1_14F8C:
        mov    edi, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 2ch], edi
        cmp    dword ptr [ebp - 24h], esi
        jbe    NEAR PTR loc_1_154DF
        mov    eax, dword ptr [ebp + 1ch]
        add    eax, 2001h
        mov    dword ptr [ebp - 28h], eax
loc_1_14FA6:
        mov    edi, dword ptr [ebp - 0ch]
        cmp    edi, dword ptr [ebp - 28h]
        ja     NEAR PTR loc_1_1552D
        sub    ecx, 3
        mov    edi, edx
        sar    edi, cl
        and    edi, 7
        cmp    ecx, 9
        jge    SHORT loc_1_14FCF
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
loc_1_14FCF:
        cmp    edi, 7
        ja     NEAR PTR loc_1_15194
        db  0FFh, 024h, 0BDh, 038h, 055h, 001h, 000h
        db  090h
loc_1_14FE0:
        sub    ecx, 5
        mov    eax, edx
        sar    eax, cl
        and    eax, 1fh
        cmp    ecx, 9
        jge    NEAR PTR loc_1_150B1
        add    ecx, 8
        shl    edx, 8
        movzx  edi, byte ptr [esi + 1]
        inc    esi
        or     edx, edi
        jmp    NEAR PTR loc_1_150B1
loc_1_15005:
        sub    ecx, 5
        mov    eax, edx
        sar    eax, cl
        and    eax, 1fh
        cmp    ecx, 9
        jge    SHORT loc_1_15021
        add    ecx, 8
        shl    edx, 8
        movzx  edi, byte ptr [esi + 1]
        inc    esi
        or     edx, edi
loc_1_15021:
        add    eax, 20h
        jmp    NEAR PTR loc_1_150B1
loc_1_15029:
        sub    ecx, 5
        mov    eax, edx
        sar    eax, cl
        and    eax, 1fh
        cmp    ecx, 9
        jge    SHORT loc_1_15045
        add    ecx, 8
        shl    edx, 8
        movzx  edi, byte ptr [esi + 1]
        inc    esi
        or     edx, edi
loc_1_15045:
        add    eax, 40h
        jmp    SHORT loc_1_150B1
loc_1_1504A:
        sub    ecx, 5
        mov    eax, edx
        sar    eax, cl
        and    eax, 1fh
        cmp    ecx, 9
        jge    SHORT loc_1_15066
        add    ecx, 8
        shl    edx, 8
        movzx  edi, byte ptr [esi + 1]
        inc    esi
        or     edx, edi
loc_1_15066:
        add    eax, 60h
        jmp    SHORT loc_1_150B1
loc_1_1506B:
        sub    ecx, 6
        mov    eax, edx
        sar    eax, cl
        and    eax, 3fh
        cmp    ecx, 9
        jge    SHORT loc_1_15087
        add    ecx, 8
        shl    edx, 8
        movzx  edi, byte ptr [esi + 1]
        inc    esi
        or     edx, edi
loc_1_15087:
        add    eax, 80h
        jmp    SHORT loc_1_150B1
        db  08Bh, 0FFh
loc_1_15090:
        sub    ecx, 6
        mov    eax, edx
        sar    eax, cl
        and    eax, 3fh
        cmp    ecx, 9
        jge    SHORT loc_1_150AC
        add    ecx, 8
        shl    edx, 8
        movzx  edi, byte ptr [esi + 1]
        inc    esi
        or     edx, edi
loc_1_150AC:
        add    eax, 0c0h
loc_1_150B1:
        mov    edi, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 0ch]
        mov    byte ptr [edi], al
        jmp    NEAR PTR loc_1_154D6
        db  08Bh, 0FFh
loc_1_150C0:
        cmp    ecx, 0dh
        jle    SHORT loc_1_150E4
        sub    ecx, 0dh
        mov    eax, edx
        sar    eax, cl
        and    eax, 1fffh
        mov    dword ptr [ebp - 10h], eax
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
        jmp    SHORT loc_1_15128
loc_1_150E4:
        sub    ecx, 5
        mov    eax, edx
        sar    eax, cl
        and    eax, 1fh
        cmp    ecx, 9
        jge    SHORT loc_1_15101
        add    ecx, 8
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_15101:
        sub    ecx, 8
        mov    dword ptr [ebp - 14h], edx
        sar    dword ptr [ebp - 14h], cl
        and    dword ptr [ebp - 14h], 0ffh
        add    ecx, 8
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
        shl    eax, 8
        add    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 10h], eax
loc_1_15128:
        add    dword ptr [ebp - 10h], 140h
        jmp    SHORT loc_1_15194
loc_1_15131:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_1514E
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_1514E:
        or     eax, eax
        je     SHORT loc_1_15174
        sub    ecx, 6
        mov    eax, edx
        sar    eax, cl
        and    eax, 3fh
        mov    dword ptr [ebp - 10h], eax
        cmp    ecx, 9
        jge    SHORT loc_1_15194
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
        jmp    SHORT loc_1_15194
loc_1_15174:
        sub    ecx, 8
        mov    eax, edx
        sar    eax, cl
        and    eax, 0ffh
        add    ecx, 8
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
        add    eax, 40h
        mov    dword ptr [ebp - 10h], eax
loc_1_15194:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_151B1
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_151B1:
        or     eax, eax
        jne    SHORT loc_1_151C0
        mov    edi, 3
        jmp    NEAR PTR loc_1_15493
        db  090h
loc_1_151C0:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_151DD
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_151DD:
        or     eax, eax
        jne    SHORT loc_1_15206
        sub    ecx, 2
        mov    edi, edx
        sar    edi, cl
        and    edi, 3
        cmp    ecx, 9
        jge    SHORT loc_1_151FE
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
loc_1_151FE:
        add    edi, 4
        jmp    NEAR PTR loc_1_15493
loc_1_15206:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_15223
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_15223:
        or     eax, eax
        jne    SHORT loc_1_1524C
        sub    ecx, 3
        mov    edi, edx
        sar    edi, cl
        and    edi, 7
        cmp    ecx, 9
        jge    SHORT loc_1_15244
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
loc_1_15244:
        add    edi, 8
        jmp    NEAR PTR loc_1_15493
loc_1_1524C:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_15269
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_15269:
        or     eax, eax
        jne    SHORT loc_1_15292
        sub    ecx, 4
        mov    edi, edx
        sar    edi, cl
        and    edi, 0fh
        cmp    ecx, 9
        jge    SHORT loc_1_1528A
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
loc_1_1528A:
        add    edi, 10h
        jmp    NEAR PTR loc_1_15493
loc_1_15292:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_152AF
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_152AF:
        or     eax, eax
        jne    SHORT loc_1_152D8
        sub    ecx, 5
        mov    edi, edx
        sar    edi, cl
        and    edi, 1fh
        cmp    ecx, 9
        jge    SHORT loc_1_152D0
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
loc_1_152D0:
        add    edi, 20h
        jmp    NEAR PTR loc_1_15493
loc_1_152D8:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_152F5
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_152F5:
        or     eax, eax
        jne    SHORT loc_1_15320
        sub    ecx, 6
        mov    edi, edx
        sar    edi, cl
        and    edi, 3fh
        cmp    ecx, 9
        jge    SHORT loc_1_15316
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
loc_1_15316:
        add    edi, 40h
        jmp    NEAR PTR loc_1_15493
        db  08Bh, 0FFh
loc_1_15320:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_1533D
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_1533D:
        or     eax, eax
        jne    SHORT loc_1_15369
        sub    ecx, 7
        mov    edi, edx
        sar    edi, cl
        and    edi, 7fh
        cmp    ecx, 9
        jge    SHORT loc_1_1535E
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
loc_1_1535E:
        add    edi, 80h
        jmp    NEAR PTR loc_1_15493
loc_1_15369:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_15386
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_15386:
        or     eax, eax
        jne    SHORT loc_1_153B0
        sub    ecx, 8
        mov    edi, edx
        sar    edi, cl
        and    edi, 0ffh
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
        add    edi, 100h
        jmp    NEAR PTR loc_1_15493
loc_1_153B0:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_153CD
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_153CD:
        or     eax, eax
        jne    SHORT loc_1_1540A
        sub    ecx, 9
        mov    edi, edx
        sar    edi, cl
        and    edi, 1ffh
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
        cmp    ecx, 9
        jge    SHORT loc_1_153FF
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
loc_1_153FF:
        add    edi, 200h
        jmp    NEAR PTR loc_1_15493
loc_1_1540A:
        dec    ecx
        mov    eax, edx
        sar    eax, cl
        and    eax, 1
        cmp    ecx, 9
        jge    SHORT loc_1_15427
        mov    ecx, 10h
        shl    edx, 8
        xor    ebx, ebx
        mov    bl, byte ptr [esi + 1]
        inc    esi
        or     edx, ebx
loc_1_15427:
        or     eax, eax
        jne    SHORT loc_1_15493
        cmp    ecx, 0ah
        jle    SHORT loc_1_15450
        sub    ecx, 0ah
        mov    edi, edx
        sar    edi, cl
        and    edi, 3ffh
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
        jmp    SHORT loc_1_1548D
        db  08Dh, 049h, 000h
loc_1_15450:
        sub    ecx, 2
        mov    edi, edx
        sar    edi, cl
        and    edi, 3
        cmp    ecx, 9
        jge    SHORT loc_1_1546D
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
loc_1_1546D:
        sub    ecx, 8
        mov    ebx, edx
        sar    ebx, cl
        and    ebx, 0ffh
        add    ecx, 8
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        inc    esi
        or     edx, eax
        shl    edi, 8
        add    edi, ebx
loc_1_1548D:
        add    edi, 400h
loc_1_15493:
        mov    ebx, dword ptr [ebp - 0ch]
        sub    ebx, dword ptr [ebp + 1ch]
        sub    ebx, dword ptr [ebp - 10h]
        and    ebx, 1fffh
        add    ebx, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 4], ebx
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 2
        mov    bl, byte ptr [ebx]
        mov    byte ptr [eax - 2], bl
        mov    ebx, dword ptr [ebp - 4]
        add    dword ptr [ebp - 0ch], edi
        mov    bl, byte ptr [ebx + 1]
        mov    byte ptr [eax - 1], bl
        add    dword ptr [ebp - 4], 2
        sub    edi, 2
        je     SHORT loc_1_154D6
loc_1_154C8:
        mov    ebx, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        mov    bl, byte ptr [ebx]
        mov    byte ptr [eax], bl
        inc    eax
        dec    edi
        jne    SHORT loc_1_154C8
loc_1_154D6:
        cmp    dword ptr [ebp - 24h], esi
        ja     NEAR PTR loc_1_14FA6
loc_1_154DF:
        cmp    ecx, 10h
        jne    SHORT loc_1_154F4
        cmp    dword ptr [ebp - 24h], esi
        jne    SHORT loc_1_154F4
        mov    edi, dword ptr [ebp - 0ch]
        mov    al, byte ptr [esi - 1]
        mov    byte ptr [edi], al
        inc    dword ptr [ebp - 0ch]
loc_1_154F4:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp + 18h]
        sub    ecx, dword ptr [ebp - 2ch]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [eax + 2001h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp + 1ch]
        mov    edi, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 2001h], edi
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1552D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  0E0h, 04Fh, 001h, 000h, 005h, 050h, 001h, 000h, 029h, 050h, 001h, 000h, 04Ah, 050h, 001h, 000h
        db  06Bh, 050h, 001h, 000h, 090h, 050h, 001h, 000h, 0C0h, 050h, 001h, 000h, 031h, 051h, 001h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_15560:
        mov    ax, 4f86h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15570:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 3f60h], 0ffffffffh
        mov    word ptr [eax + 3f74h], 0
        mov    word ptr [eax + 3f80h], 4321h
        db  08Ah, 00Dh, 048h, 098h, 001h, 000h
        mov    byte ptr [eax + 3f82h], cl
        mov    byte ptr [eax + 3f82h], 2
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_155B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 0ffffh
        test   eax, 0ffffffe0h
        je     SHORT loc_1_155E0
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_155E0:
        mov    eax, dword ptr [ebp + 18h]
        cmp    word ptr [eax + 3f80h], 4321h
        je     SHORT loc_1_15600
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_15600:
        mov    esi, dword ptr [ebp + 18h]
        mov    dl, byte ptr [esi + 3f82h]
        add    esi, 3f82h
        mov    eax, dword ptr [ebp + 8]
        and    edx, 0ffff00ffh
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax], 10h
        jae    SHORT loc_1_15640
        test   dl, 8
        jne    SHORT loc_1_15640
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_15640:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [ebp - 10h]
        sub    eax, 4
        mov    dword ptr [ebp - 14h], eax
        test   byte ptr [ebp + 1ch], 2
        je     SHORT loc_1_15660
        sub    dword ptr [ebp - 14h], 0ch
loc_1_15660:
        test   dl, 0f0h
        je     NEAR PTR loc_1_156F0
        mov    eax, dword ptr [ebp + 18h]
        and    byte ptr [esi], 0fh
        mov    esi, dword ptr [eax + 4]
        and    dx, 10h
        jne    SHORT loc_1_15680
        sub    di, di
        jmp    SHORT loc_1_15692
        db  08Dh, 049h, 000h
loc_1_15680:
        mov    eax, dword ptr [ebp + 18h]
        xor    edi, edi
        mov    di, word ptr [eax + 3f7eh]
        add    edi, 2
        sar    edi, 3
loc_1_15692:
        mov    eax, dword ptr [ebp + 18h]
        mov    bx, word ptr [eax + 3f78h]
        cmp    di, bx
        jae    SHORT loc_1_156BB
loc_1_156A1:
        mov    eax, esi
        lea    ecx, [ebx*8 - 8]
        shr    eax, cl
        mov    ecx, dword ptr [ebp - 10h]
        dec    bx
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 10h]
        cmp    di, bx
        jb     SHORT loc_1_156A1
loc_1_156BB:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 3f68h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 18h]
        test   dl, 10h
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 3f76h]
        mov    word ptr [ebp - 2], ax
        je     NEAR PTR loc_1_157AA
        jmp    NEAR PTR loc_1_15AE5
        db  08Dh, 064h, 024h, 000h
loc_1_156F0:
        mov    ecx, dword ptr [ebp + 18h]
        add    ecx, 3f64h
        mov    dword ptr [ebp - 28h], ecx
        mov    edi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [edi + 3f60h]
        add    edi, 3f60h
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_15720
        mov    word ptr [ebp - 2], 1
        jmp    NEAR PTR loc_1_15AE5
        db  08Bh, 0FFh
loc_1_15720:
        test   dl, 2
        je     NEAR PTR loc_1_158B7
        mov    ebx, dword ptr [edi]
        test   byte ptr [ebp + 1ch], 1
        mov    eax, 0ffffffffh
        je     SHORT loc_1_15739
        mov    eax, dword ptr [ebp - 0ch]
loc_1_15739:
        mov    ecx, 0ffffffffh
        sub    ecx, eax
        cmp    ecx, ebx
        jae    SHORT loc_1_15768
        sub    ebx, ebx
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 200ch], ebx
        add    eax, 200ch
        push   1f4ch
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 2010h
        push   eax
        call   sub_1_17510
loc_1_15768:
        add    ebx, 800h
        mov    ecx, dword ptr [ebp - 28h]
        mov    dword ptr [edi], ebx
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], ebx
        mov    word ptr [eax + 3f74h], 0
        mov    byte ptr [esi], 0
        mov    dword ptr [eax + 4], 0
        mov    word ptr [eax + 3f7eh], 20h
loc_1_15794:
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 3f5ch], eax
loc_1_157A0:
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 3f83h], 4
loc_1_157AA:
        mov    esi, 1
        nop
loc_1_157B0:
        mov    edi, dword ptr [ebp - 8]
        mov    ax, word ptr [ebp + 20h]
        push   eax
        add    edi, dword ptr [ebp - 0ch]
        mov    ax, word ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 8]
        push   edi
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_15C10
        test   al, 0e0h
        jne    NEAR PTR loc_1_159CA
        test   al, 8
        jne    NEAR PTR loc_1_15B07
        test   word ptr [ebp - 2], si
        jne    NEAR PTR loc_1_15984
        mov    ecx, dword ptr [ebp + 18h]
        mov    al, byte ptr [ecx + 3f82h]
        add    ecx, 3f82h
        test   al, 4
        je     SHORT loc_1_1584F
        and    al, 0fbh
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 3f60h]
        sub    edx, dword ptr [eax + 3f64h]
        add    eax, 3f5ch
        mov    edi, dword ptr [ebp - 8]
        sub    edi, dword ptr [eax]
        mov    ebx, dword ptr [ebx]
        add    ebx, edx
        mov    dword ptr [ebp - 8], ebx
        and    edi, 0ffffh
        sub    ebx, edi
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        sub    eax, edx
        mov    dword ptr [ebp - 0ch], eax
        cmp    edx, 7ffh
        jae    NEAR PTR loc_1_15973
        cmp    eax, 4
        ja     SHORT loc_1_158C3
loc_1_1584F:
        mov    eax, dword ptr [ebp + 18h]
        or     word ptr [ebp - 2], si
        mov    edx, dword ptr [eax + 3f64h]
        mov    eax, dword ptr [ebp + 10h]
        add    edx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [eax + 3f60h], edx
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [edx]
        mov    edx, dword ptr [ebp + 18h]
        add    edx, 3f7ah
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 3f70h
        mov    word ptr [edx], 0
        mov    dword ptr [eax], 2
        test   word ptr [ebp + 1ch], si
        jne    NEAR PTR loc_1_157B0
        or     byte ptr [ecx], 8
        mov    word ptr [edx], 7ffh
        mov    dword ptr [eax], 0c8h
        jmp    NEAR PTR loc_1_157B0
loc_1_158B7:
        test   dl, 8
        je     NEAR PTR loc_1_15794
        and    byte ptr [esi], 0f7h
loc_1_158C3:
        mov    eax, dword ptr [ebp + 18h]
        or     byte ptr [eax + 3f82h], 4
        add    eax, 3f74h
        mov    dword ptr [ebp - 18h], eax
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 4], ax
        cmp    ax, 7ffh
        jbe    SHORT loc_1_15921
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 0ch]
        cmp    eax, 1002h
        jbe    SHORT loc_1_15921
        sub    word ptr [ebp - 4], 7ffh
        push   7ffh
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp + 18h]
        add    eax, 3f84h
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 3f84h
        push   eax
        call   sub_1_17300
        mov    word ptr [ebp - 4], 7ffh
loc_1_15921:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, 1002h
        sub    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 20h], eax
        jae    SHORT loc_1_15945
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_15945:
        mov    si, word ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 1ch]
        lea    edi, [eax + ecx + 3f84h]
        push   esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_17300
        mov    dword ptr [ebp - 8], edi
        mov    eax, dword ptr [ebp - 18h]
        add    si, word ptr [ebp - 4]
        mov    word ptr [eax], si
        jmp    NEAR PTR loc_1_15794
loc_1_15973:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 3f74h], 0
        jmp    NEAR PTR loc_1_157A0
loc_1_15984:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [ecx]
        mov    si, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 3f7eh]
        and    eax, 0ffffh
        add    eax, 2
        sar    eax, 3
        sub    si, ax
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, esi
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 10h]
        add    ecx, 4
        cmp    eax, ecx
        jbe    NEAR PTR loc_1_15A50
        mov    eax, 10h
loc_1_159CA:
        mov    edx, dword ptr [ebp + 18h]
        or     byte ptr [edx + 3f82h], al
        add    edx, 3f82h
        mov    esi, dword ptr [ebp + 18h]
        mov    cx, word ptr [ebp - 2]
        mov    word ptr [esi + 3f76h], cx
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 8], ecx
        mov    ecx, esi
        sub    edi, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 3f68h], edi
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebp - 2], 2
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ebp - 8], ecx
        test   byte ptr [ebp + 1ch], 2
        je     NEAR PTR loc_1_15B92
        mov    ecx, esi
        mov    ebx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 3], bl
        mov    ecx, dword ptr [ebp - 10h]
        shr    ebx, 8
        mov    byte ptr [ecx + 2], bl
        mov    ecx, dword ptr [ebp - 10h]
        shr    ebx, 8
        mov    byte ptr [ecx + 1], bl
        mov    ecx, dword ptr [ebp - 10h]
        add    esi, 3f7eh
        mov    byte ptr [ecx], bh
        mov    cx, 20h
        add    dword ptr [ebp - 10h], 4
        sub    cx, word ptr [esi]
        mov    word ptr [esi], cx
        jne    SHORT loc_1_15A58
        sub    edi, edi
        jmp    SHORT loc_1_15A5F
        db  08Dh, 064h, 024h, 000h
loc_1_15A50:
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, dword ptr [eax + 4]
        jmp    SHORT loc_1_15ABC
loc_1_15A58:
        mov    edi, dword ptr [ebp + 18h]
        mov    edi, dword ptr [edi]
        shl    edi, cl
loc_1_15A5F:
        test   al, 40h
        je     SHORT loc_1_15A95
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebx + 3f70h]
        add    ebx, 3f70h
        cmp    eax, 0eh
        jb     SHORT loc_1_15A7B
        mov    ax, 0eh
loc_1_15A7B:
        sub    cx, 4
        mov    word ptr [esi], cx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 24h], eax
        shl    dword ptr [ebp - 24h], cl
        or     edi, dword ptr [ebp - 24h]
        sub    eax, dword ptr [ebx]
        add    dword ptr [ebp - 8], eax
loc_1_15A95:
        mov    cx, word ptr [esi]
        sub    cx, 9
        mov    word ptr [esi], cx
        mov    eax, 180h
        shl    eax, cl
        or     edi, eax
        mov    si, 4
        shr    cx, 3
        sub    si, cx
        mov    byte ptr [edx], 2
        mov    word ptr [ebp - 2], 6
loc_1_15ABC:
        mov    eax, esi
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 10h]
        mov    cx, 18h
        cmp    eax, dword ptr [ebp - 10h]
        jbe    SHORT loc_1_15AE5
        nop
loc_1_15AD0:
        mov    edx, edi
        mov    esi, dword ptr [ebp - 10h]
        shr    edx, cl
        mov    byte ptr [esi], dl
        inc    dword ptr [ebp - 10h]
        sub    cx, 8
        cmp    eax, dword ptr [ebp - 10h]
        ja     SHORT loc_1_15AD0
loc_1_15AE5:
        or     word ptr [ebp - 2], 4
        test   byte ptr [ebp + 1ch], 4
        je     NEAR PTR loc_1_15B80
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 4], 0
        mov    word ptr [eax + 3f7eh], 20h
loc_1_15B07:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, eax
        add    esi, 3f74h
        or     byte ptr [eax + 3f82h], 8
        cmp    word ptr [esi], 0
        jne    SHORT loc_1_15B57
        mov    eax, dword ptr [ebp + 10h]
        mov    di, 7ffh
        mov    edx, dword ptr [eax]
        cmp    edx, 7ffh
        jae    SHORT loc_1_15B33
        mov    di, dx
loc_1_15B33:
        mov    eax, dword ptr [ebp + 8]
        push   edi
        mov    eax, dword ptr [eax]
        mov    ecx, edi
        and    ecx, 0ffffh
        sub    eax, ecx
        add    eax, edx
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 3f84h
        push   eax
        call   sub_1_17300
        mov    word ptr [esi], di
loc_1_15B57:
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [eax + 3f64h]
        mov    eax, dword ptr [ebp + 10h]
        add    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 3f60h], ecx
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_1_15BCA
        db  08Dh, 049h, 000h
loc_1_15B80:
        test   byte ptr [ebp + 1ch], 1
        je     SHORT loc_1_15BCA
        mov    eax, dword ptr [ebp + 18h]
        or     byte ptr [eax + 3f82h], 2
        jmp    SHORT loc_1_15BCA
loc_1_15B92:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp - 10h]
        mov    edx, dword ptr [eax + 4]
        mov    di, 3
        sub    esi, dword ptr [ebp - 14h]
        mov    word ptr [eax + 3f78h], si
        cmp    si, di
        ja     SHORT loc_1_15BCA
        mov    edi, edi
loc_1_15BB0:
        mov    ebx, edx
        lea    ecx, [edi*8]
        mov    eax, dword ptr [ebp - 10h]
        shr    ebx, cl
        mov    byte ptr [eax], bl
        dec    di
        inc    dword ptr [ebp - 10h]
        cmp    si, di
        jbe    SHORT loc_1_15BB0
loc_1_15BCA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 10h]
        add    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 8]
        add    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 60h
        push   ebx
        push   esi
        push   edi
        and    word ptr [ebp + 20h], 0ffh
        mov    ax, word ptr [ebp + 20h]
        mov    word ptr [ebp - 6], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0ah], 7d3h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, dword ptr [ebp - 24h]
        cmp    eax, 4
        jb     SHORT loc_1_15C50
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, 4
        jmp    SHORT loc_1_15C53
loc_1_15C50:
        mov    eax, dword ptr [ebp - 24h]
loc_1_15C53:
        mov    dword ptr [ebp - 60h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 3f5ch]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 3f60h]
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 3f6ch]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax + 3f70h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 3f7ah]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 3f7ch]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 3f7eh]
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    al, byte ptr [eax + 3f83h]
        test   al, 4
        jne    NEAR PTR loc_1_15DE0
        test   al, 1
        jne    NEAR PTR loc_1_15FF8
        test   al, 20h
        jne    NEAR PTR loc_1_1609A
        test   al, 80h
        je     NEAR PTR loc_1_1632B
        jmp    NEAR PTR loc_1_1616B
        db  090h
loc_1_15CF0:
        mov    eax, dword ptr [ebp - 24h]
        inc    eax
        mov    dword ptr [ebp - 40h], eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 0ah]
        mov    ecx, dword ptr [ebp - 40h]
        mov    dword ptr [ebp - 50h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        shl    eax, 7
        or     eax, ecx
        cdq
        idiv   dword ptr [ebp - 50h]
        mov    eax, edx
        mov    ecx, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        lea    ecx, [ecx + eax*4 + 200ch]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 34h]
        mov    edx, dword ptr [ebp + 18h]
        and    ecx, 7ffh
        mov    dword ptr [edx + ecx*4 + 0ch], eax
        mov    eax, dword ptr [ebp - 34h]
        sub    eax, dword ptr [ebp - 30h]
        mov    dword ptr [ebp - 2ch], eax
        cmp    eax, 7ffh
        ja     SHORT loc_1_15DD8
        mov    ecx, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        sub    ecx, eax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 3ch], ecx
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 48h], al
        mov    eax, ecx
        mov    cl, byte ptr [ebp - 48h]
        cmp    byte ptr [eax], cl
        jne    SHORT loc_1_15D87
        mov    ecx, dword ptr [ebp - 40h]
        mov    eax, dword ptr [ebp - 3ch]
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [eax + 1], cl
        je     SHORT loc_1_15DF8
loc_1_15D87:
        cmp    word ptr [ebp - 6], 0
        je     SHORT loc_1_15DD8
        mov    ecx, 7ffh
loc_1_15D93:
        mov    eax, dword ptr [ebp - 30h]
        mov    edx, dword ptr [ebp + 18h]
        and    eax, ecx
        mov    eax, dword ptr [edx + eax*4 + 0ch]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 34h]
        sub    eax, dword ptr [ebp - 30h]
        cmp    eax, ecx
        mov    dword ptr [ebp - 2ch], eax
        ja     SHORT loc_1_15DD0
        mov    edx, dword ptr [ebp - 24h]
        and    eax, 0ffffh
        sub    edx, eax
        mov    al, byte ptr [ebp - 48h]
        cmp    byte ptr [edx], al
        jne    SHORT loc_1_15DCA
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax]
        cmp    byte ptr [edx + 1], al
        je     SHORT loc_1_15DF8
loc_1_15DCA:
        dec    word ptr [ebp - 6]
        jne    SHORT loc_1_15D93
loc_1_15DD0:
        mov    ax, word ptr [ebp + 20h]
        mov    word ptr [ebp - 6], ax
loc_1_15DD8:
        inc    dword ptr [ebp - 34h]
        inc    dword ptr [ebp - 24h]
        mov    edi, edi
loc_1_15DE0:
        mov    eax, dword ptr [ebp - 24h]
        cmp    dword ptr [ebp - 60h], eax
        ja     NEAR PTR loc_1_15CF0
        mov    dword ptr [ebp - 20h], 1
        jmp    NEAR PTR loc_1_16286
loc_1_15DF8:
        mov    eax, dword ptr [ebp - 2ch]
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp + 20h]
        sub    ecx, ecx
        mov    word ptr [ebp - 6], ax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 1ch], 2
        mov    al, byte ptr [eax + 2]
        mov    edx, 7ffh
        mov    byte ptr [ebp - 54h], al
        mov    byte ptr [ebp - 5ch], al
loc_1_15E21:
        mov    esi, dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffffh
        sub    esi, eax
        mov    al, byte ptr [ebp - 5ch]
        cmp    byte ptr [esi + 2], al
        jne    SHORT loc_1_15E47
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax]
        cmp    byte ptr [esi + 1], al
        jne    SHORT loc_1_15E47
        mov    al, byte ptr [ebp - 48h]
        cmp    byte ptr [esi], al
        je     SHORT loc_1_15E77
loc_1_15E47:
        mov    esi, dword ptr [ebp - 30h]
        mov    eax, dword ptr [ebp + 18h]
        and    esi, edx
        mov    eax, dword ptr [eax + esi*4 + 0ch]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 34h]
        sub    eax, dword ptr [ebp - 30h]
        mov    dword ptr [ebp - 2ch], eax
        cmp    word ptr [ebp - 6], cx
        je     NEAR PTR loc_1_15F60
        cmp    eax, edx
        ja     NEAR PTR loc_1_15F60
        dec    word ptr [ebp - 6]
        jmp    SHORT loc_1_15E21
loc_1_15E77:
        mov    eax, dword ptr [ebp - 2ch]
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp + 20h]
        sub    edi, edi
        mov    word ptr [ebp - 6], ax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 1ch], 3
        mov    al, byte ptr [eax + 3]
        mov    byte ptr [ebp - 58h], al
loc_1_15E98:
        mov    dx, word ptr [ebp - 2ch]
        mov    eax, dword ptr [ebp - 24h]
        mov    esi, edx
        and    esi, 0ffffh
        sub    eax, esi
        mov    cl, byte ptr [ebp - 58h]
        cmp    byte ptr [eax + 3], cl
        jne    SHORT loc_1_15F27
        mov    cl, byte ptr [ebp - 54h]
        cmp    byte ptr [eax + 2], cl
        jne    SHORT loc_1_15F27
        mov    ecx, dword ptr [ebp - 40h]
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [eax + 1], cl
        jne    SHORT loc_1_15F27
        mov    cl, byte ptr [ebp - 48h]
        cmp    byte ptr [eax], cl
        jne    SHORT loc_1_15F27
        mov    ecx, dword ptr [ebp - 24h]
        add    ecx, 4
        mov    eax, ecx
        sub    eax, esi
        mov    bl, byte ptr [eax]
        cmp    byte ptr [ecx], bl
        jne    SHORT loc_1_15F0B
        inc    ecx
        inc    eax
        mov    esi, dword ptr [ebp + 14h]
        sub    esi, ecx
        sar    esi, 1
        cmp    esi, 19h
        jbe    SHORT loc_1_15EED
        mov    esi, 19h
loc_1_15EED:
        cmp    esi, edi
        je     SHORT loc_1_15F0B
loc_1_15EF1:
        mov    bl, byte ptr [eax]
        cmp    byte ptr [ecx], bl
        jne    SHORT loc_1_15F0B
        mov    bl, byte ptr [eax + 1]
        cmp    byte ptr [ecx + 1], bl
        jne    SHORT loc_1_15F0A
        add    ecx, 2
        add    eax, 2
        dec    esi
        jne    SHORT loc_1_15EF1
        jmp    SHORT loc_1_15F0B
loc_1_15F0A:
        inc    ecx
loc_1_15F0B:
        sub    ecx, dword ptr [ebp - 24h]
        cmp    dword ptr [ebp - 1ch], ecx
        jae    SHORT loc_1_15F1A
        mov    dword ptr [ebp - 1ch], ecx
        mov    word ptr [ebp - 8], dx
loc_1_15F1A:
        mov    ax, word ptr [ebp - 6]
        dec    word ptr [ebp - 6]
        cmp    ax, di
        je     SHORT loc_1_15F60
loc_1_15F27:
        cmp    word ptr [ebp - 6], di
        je     SHORT loc_1_15F60
        mov    ecx, dword ptr [ebp - 30h]
        mov    eax, dword ptr [ebp + 18h]
        and    ecx, 7ffh
        mov    eax, dword ptr [eax + ecx*4 + 0ch]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 34h]
        sub    eax, dword ptr [ebp - 30h]
        cmp    eax, 7ffh
        mov    dword ptr [ebp - 2ch], eax
        ja     SHORT loc_1_15F60
        dec    word ptr [ebp - 6]
        jmp    NEAR PTR loc_1_15E98
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_15F60:
        test   byte ptr [ebp + 1ch], 18h
        je     NEAR PTR loc_1_15FF2
        mov    eax, dword ptr [ebp - 24h]
        add    eax, dword ptr [ebp - 1ch]
        dec    eax
        mov    dword ptr [ebp - 4ch], eax
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 44h], eax
        cmp    dword ptr [ebp - 1ch], 2
        jbe    SHORT loc_1_15FA4
        test   byte ptr [ebp + 1ch], 10h
        je     SHORT loc_1_15F98
        cmp    word ptr [ebp - 8], 1
        jbe    SHORT loc_1_15F98
        mov    eax, dword ptr [ebp - 40h]
        mov    dword ptr [ebp - 3ch], eax
        jmp    SHORT loc_1_15FA4
loc_1_15F98:
        mov    eax, dword ptr [ebp - 34h]
        add    eax, dword ptr [ebp - 1ch]
        sub    eax, 2
        mov    dword ptr [ebp - 44h], eax
loc_1_15FA4:
        inc    dword ptr [ebp - 44h]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    eax, dword ptr [ebp - 3ch]
        inc    ecx
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        shl    eax, 7
        or     eax, edx
        cdq
        idiv   dword ptr [ebp - 50h]
        mov    eax, edx
        mov    edx, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        lea    edi, [edx + eax*4 + 200ch]
        mov    edx, dword ptr [ebp - 44h]
        mov    eax, dword ptr [edi]
        mov    esi, dword ptr [ebp + 18h]
        and    edx, 7ffh
        mov    dword ptr [esi + edx*4 + 0ch], eax
        mov    eax, dword ptr [ebp - 44h]
        mov    dword ptr [edi], eax
        mov    dword ptr [ebp - 3ch], ecx
        cmp    dword ptr [ebp - 4ch], ecx
        jae    SHORT loc_1_15FA4
loc_1_15FF2:
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 34h], eax
loc_1_15FF8:
        mov    eax, dword ptr [ebp - 24h]
        sub    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 38h], eax
        or     eax, eax
        je     NEAR PTR loc_1_161A0
        test   al, 1
        je     NEAR PTR loc_1_160C2
        cmp    word ptr [ebp - 2], 9
        jbe    SHORT loc_1_16034
        mov    eax, dword ptr [ebp - 28h]
        sub    word ptr [ebp - 2], 9
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    cl, byte ptr [ebp - 2]
        shl    eax, cl
        or     dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_160BF
loc_1_16034:
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    dword ptr [ebp - 18h], eax
        mov    ax, 9
        sub    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 2], ax
        mov    cl, byte ptr [ebp - 2]
        mov    eax, dword ptr [ebp - 18h]
        shr    eax, cl
        or     dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 0ch], eax
        jae    SHORT loc_1_16070
        mov    dword ptr [ebp - 20h], 20h
        jmp    NEAR PTR loc_1_16286
        db  08Dh, 064h, 024h, 000h
loc_1_16070:
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax + 3], cl
        shr    dword ptr [ebp - 10h], 8
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax + 2], cl
        shr    dword ptr [ebp - 10h], 8
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax + 1], cl
        shr    dword ptr [ebp - 10h], 8
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax], cl
        add    dword ptr [ebp - 14h], 4
loc_1_1609A:
        mov    ax, 20h
        sub    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    cl, byte ptr [ebp - 2]
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        cmp    word ptr [ebp - 2], 20h
        jne    SHORT loc_1_160BF
        mov    dword ptr [ebp - 10h], 0
loc_1_160BF:
        inc    dword ptr [ebp - 28h]
loc_1_160C2:
        shr    dword ptr [ebp - 38h], 1
loc_1_160C5:
        cmp    dword ptr [ebp - 38h], 0
        jbe    NEAR PTR loc_1_161A0
        cmp    word ptr [ebp - 2], 12h
        jbe    SHORT loc_1_16100
        sub    word ptr [ebp - 2], 12h
        mov    eax, dword ptr [ebp - 28h]
        xor    edx, edx
        mov    dl, byte ptr [eax]
        shl    edx, 9
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        or     edx, eax
        mov    cl, byte ptr [ebp - 2]
        shl    edx, cl
        or     dword ptr [ebp - 10h], edx
        jmp    NEAR PTR loc_1_16190
        db  08Dh, 064h, 024h, 000h
loc_1_16100:
        mov    eax, dword ptr [ebp - 28h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        shl    ecx, 9
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        or     ecx, eax
        mov    dword ptr [ebp - 18h], ecx
        mov    ax, 12h
        sub    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 2], ax
        mov    eax, ecx
        mov    cl, byte ptr [ebp - 2]
        shr    eax, cl
        or     dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 0ch], eax
        jae    SHORT loc_1_16141
        mov    dword ptr [ebp - 20h], 80h
        jmp    NEAR PTR loc_1_16286
loc_1_16141:
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax + 3], cl
        shr    dword ptr [ebp - 10h], 8
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax + 2], cl
        shr    dword ptr [ebp - 10h], 8
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax + 1], cl
        shr    dword ptr [ebp - 10h], 8
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax], cl
        add    dword ptr [ebp - 14h], 4
loc_1_1616B:
        mov    ax, 20h
        sub    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    cl, byte ptr [ebp - 2]
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        cmp    word ptr [ebp - 2], 20h
        jne    SHORT loc_1_16190
        mov    dword ptr [ebp - 10h], 0
loc_1_16190:
        add    dword ptr [ebp - 28h], 2
        dec    dword ptr [ebp - 38h]
        jmp    NEAR PTR loc_1_160C5
        db  08Dh, 064h, 024h, 000h
loc_1_161A0:
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 28h], eax
        test   word ptr [ebp - 8], 0ff80h
        mov    ax, word ptr [ebp - 8]
        je     SHORT loc_1_161D0
        or     ax, 1000h
        and    eax, 0ffffh
        mov    dword ptr [ebp - 18h], eax
        mov    word ptr [ebp - 4], 0dh
        jmp    SHORT loc_1_161E2
        db  08Dh, 064h, 024h, 000h
loc_1_161D0:
        or     ax, 180h
        and    eax, 0ffffh
        mov    dword ptr [ebp - 18h], eax
        mov    word ptr [ebp - 4], 9
loc_1_161E2:
        cmp    dword ptr [ebp - 1ch], 5
        jae    SHORT loc_1_16203
        mov    eax, dword ptr [ebp - 1ch]
        sub    eax, 2
        mov    ecx, dword ptr [ebp - 18h]
        shl    ecx, 2
        or     eax, ecx
        mov    dword ptr [ebp - 18h], eax
        add    word ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_16382
loc_1_16203:
        cmp    dword ptr [ebp - 1ch], 8
        jae    SHORT loc_1_16220
        mov    eax, dword ptr [ebp - 18h]
        shl    eax, 4
        mov    ecx, dword ptr [ebp - 1ch]
        sub    ecx, 5
        or     eax, ecx
        or     eax, 0ch
        jmp    NEAR PTR loc_1_1637A
        db  090h
loc_1_16220:
        add    dword ptr [ebp - 1ch], 7
loc_1_16224:
        mov    eax, dword ptr [ebp - 18h]
        shl    eax, 4
        or     eax, 0fh
        mov    dword ptr [ebp - 18h], eax
        add    word ptr [ebp - 4], 4
        cmp    word ptr [ebp - 4], 1ch
        jbe    NEAR PTR loc_1_16363
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax + 3f82h], 8
        je     NEAR PTR loc_1_16382
        mov    dword ptr [ebp - 20h], 8
        jmp    SHORT loc_1_16286
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_16260:
        sub    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 2], ax
        mov    cl, byte ptr [ebp - 2]
        mov    eax, dword ptr [ebp - 18h]
        shr    eax, cl
        or     dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 0ch], eax
        jae    NEAR PTR loc_1_16301
        mov    dword ptr [ebp - 20h], 40h
loc_1_16286:
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ecx + 3f5ch], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ecx + 3f60h], eax
        mov    eax, dword ptr [ebp - 38h]
        mov    dword ptr [ecx + 3f6ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 3f70h], eax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ecx + 3f7ah], ax
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ecx + 3f7ch], ax
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ecx + 3f7eh], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    cl, byte ptr [ebp - 20h]
        mov    byte ptr [eax + 3f83h], cl
        mov    eax, dword ptr [ebp - 20h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_16301:
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax + 3], cl
        shr    dword ptr [ebp - 10h], 8
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax + 2], cl
        shr    dword ptr [ebp - 10h], 8
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax + 1], cl
        shr    dword ptr [ebp - 10h], 8
        mov    cl, byte ptr [ebp - 10h]
        mov    byte ptr [eax], cl
        add    dword ptr [ebp - 14h], 4
loc_1_1632B:
        mov    ax, 20h
        sub    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    cl, byte ptr [ebp - 2]
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        cmp    word ptr [ebp - 2], 20h
        jne    SHORT loc_1_16350
        mov    dword ptr [ebp - 10h], 0
loc_1_16350:
        cmp    dword ptr [ebp - 1ch], 0eh
        jbe    SHORT loc_1_163A5
        mov    word ptr [ebp - 4], 0
        mov    dword ptr [ebp - 18h], 0
loc_1_16363:
        sub    dword ptr [ebp - 1ch], 0fh
        cmp    dword ptr [ebp - 1ch], 0eh
        ja     NEAR PTR loc_1_16224
        mov    eax, dword ptr [ebp - 18h]
        shl    eax, 4
        or     eax, dword ptr [ebp - 1ch]
loc_1_1637A:
        mov    dword ptr [ebp - 18h], eax
        add    word ptr [ebp - 4], 4
loc_1_16382:
        mov    ax, word ptr [ebp - 2]
        cmp    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 4]
        jae    NEAR PTR loc_1_16260
        sub    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    cl, byte ptr [ebp - 2]
        shl    eax, cl
        or     dword ptr [ebp - 10h], eax
        jmp    SHORT loc_1_16350
loc_1_163A5:
        mov    ax, word ptr [ebp + 20h]
        mov    word ptr [ebp - 6], ax
        jmp    NEAR PTR loc_1_15DE0
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_163C0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 4], 0
        mov    word ptr [eax + 8], 1234h
        mov    byte ptr [eax + 0ah], 40h
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_163E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0ch]
        push   eax
        call   sub_1_163C0
        lea    eax, [esi + 1fh]
        mov    dword ptr [esi], eax
        mov    word ptr [esi + 8], 0
        mov    word ptr [esi + 0ah], 0
        mov    byte ptr [esi + 17h], 0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_16410:
        mov    ax, 102fh
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16420:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, dword ptr [ebp - 0ch]
        cmp    eax, 10h
        jb     SHORT loc_1_16450
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, 0fh
        jmp    SHORT loc_1_16453
        db  005h, 000h, 000h, 000h, 000h
loc_1_16450:
        mov    eax, dword ptr [ebp - 0ch]
loc_1_16453:
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 18h], eax
        mov    ax, word ptr [ebp - 2]
        and    word ptr [ebp - 2], 7
        shr    ax, 3
        and    eax, 0ffffh
        add    dword ptr [ebp - 14h], eax
        mov    edx, dword ptr [ebp + 0ch]
        sub    edx, dword ptr [ebp - 14h]
        cmp    edx, 4
        jae    NEAR PTR loc_1_16690
        mov    dword ptr [ebp - 8], 0
        sub    dword ptr [ebp - 14h], 4
        add    word ptr [ebp - 2], 20h
        sub    esi, esi
        cmp    edx, esi
        je     SHORT loc_1_164D8
        add    dword ptr [ebp - 14h], edx
        mov    edi, 0fffffff8h
loc_1_164B6:
        dec    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        add    word ptr [ebp - 2], di
        lea    ecx, [esi*8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        shl    eax, cl
        or     dword ptr [ebp - 8], eax
        inc    esi
        cmp    esi, edx
        jb     SHORT loc_1_164B6
loc_1_164D8:
        mov    cl, byte ptr [ebp - 2]
        shl    dword ptr [ebp - 8], cl
        cmp    word ptr [ebp - 2], 17h
        ja     NEAR PTR loc_1_16680
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 0ah
        mov    al, byte ptr [eax]
        test   al, 49h
        je     NEAR PTR loc_1_165E0
        test   byte ptr [ebp - 5], 80h
        jne    SHORT loc_1_16540
        mov    ecx, dword ptr [ebp - 8]
        shr    ecx, 17h
loc_1_16505:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 0ch]
        mov    byte ptr [eax], cl
        add    word ptr [ebp - 2], 9
loc_1_16512:
        mov    ax, word ptr [ebp - 2]
        and    word ptr [ebp - 2], 7
        shr    ax, 3
        and    eax, 0ffffh
        add    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 0ch], eax
        ja     NEAR PTR loc_1_166B9
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 0ah], 1
        jmp    SHORT loc_1_1659E
        db  08Dh, 064h, 024h, 000h
loc_1_16540:
        cmp    word ptr [ebp - 2], 11h
        jb     NEAR PTR loc_1_167EA
        test   byte ptr [ebp - 5], 40h
        je     NEAR PTR loc_1_16680
        cmp    word ptr [ebp - 2], 15h
        jb     NEAR PTR loc_1_1677B
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 17h
        test   al, 7fh
        jne    NEAR PTR loc_1_16680
        add    word ptr [ebp - 2], 9
loc_1_16573:
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 0ah], 80h
        mov    ax, word ptr [ebp - 2]
        neg    ax
        and    ax, 7
        add    word ptr [ebp - 2], ax
loc_1_16589:
        mov    ax, word ptr [ebp - 2]
        and    word ptr [ebp - 2], 7
        shr    ax, 3
        and    eax, 0ffffh
        add    dword ptr [ebp - 14h], eax
loc_1_1659E:
        mov    ecx, dword ptr [ebp + 18h]
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ecx + 4], ax
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_165E0:
        test   al, 20h
        jne    NEAR PTR loc_1_16900
        cmp    dword ptr [ebp - 18h], 8
        jae    NEAR PTR loc_1_16874
loc_1_165F2:
        cmp    word ptr [ebp - 2], 1ch
        jbe    SHORT loc_1_1665D
        mov    dword ptr [ebp - 18h], 0
        mov    ax, word ptr [ebp - 2]
        and    word ptr [ebp - 2], 7
        shr    ax, 3
        and    eax, 0ffffh
        add    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 0ch], eax
        jbe    NEAR PTR loc_1_16898
loc_1_16621:
        mov    eax, dword ptr [ebp - 14h]
        xor    edx, edx
        mov    dl, byte ptr [eax + 2]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        shl    eax, 10h
        or     edx, eax
        mov    eax, dword ptr [ebp - 14h]
        shl    edx, 8
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        shl    eax, 10h
        or     edx, eax
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, byte ptr [ebp - 2]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     edx, eax
        shl    edx, cl
        mov    dword ptr [ebp - 8], edx
loc_1_1665D:
        cmp    dword ptr [ebp - 8], 0f0000000h
        jae    NEAR PTR loc_1_16860
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 1ch
        and    eax, 3
        add    eax, 5
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_168FB
        db  08Bh, 0FFh
loc_1_16680:
        mov    eax, dword ptr [ebp + 18h]
        or     byte ptr [eax + 0ah], 4
        jmp    NEAR PTR loc_1_16589
        db  08Dh, 064h, 024h, 000h
loc_1_16690:
        mov    eax, dword ptr [ebp + 18h]
        sub    dword ptr [ebp + 0ch], 3
        mov    al, byte ptr [eax + 0ah]
        test   al, 49h
        jne    NEAR PTR loc_1_16512
        test   al, 20h
        jne    NEAR PTR loc_1_16900
        cmp    dword ptr [ebp - 18h], 8
        jae    NEAR PTR loc_1_168A4
        jmp    NEAR PTR loc_1_16621
loc_1_166B9:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 1ch], eax
        ja     SHORT loc_1_166D0
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 0ah], 8
        jmp    NEAR PTR loc_1_1659E
        db  08Dh, 049h, 000h
loc_1_166D0:
        mov    eax, dword ptr [ebp - 14h]
        xor    edx, edx
        mov    dl, byte ptr [eax + 2]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        shl    eax, 10h
        or     edx, eax
        mov    eax, dword ptr [ebp - 14h]
        shl    edx, 8
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        shl    eax, 10h
        or     edx, eax
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, byte ptr [ebp - 2]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     edx, eax
        shl    edx, cl
        mov    dword ptr [ebp - 8], edx
        test   edx, 80000000h
        jne    NEAR PTR loc_1_167EA
        mov    ecx, edx
        mov    eax, dword ptr [ebp - 0ch]
        shr    ecx, 17h
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 0ch]
        add    word ptr [ebp - 2], 9
        test   edx, 400000h
        jne    NEAR PTR loc_1_167E6
        mov    ecx, edx
        mov    eax, dword ptr [ebp - 0ch]
        shr    ecx, 0eh
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 0ch]
        add    word ptr [ebp - 2], 9
        cmp    word ptr [ebp - 2], 17h
        ja     NEAR PTR loc_1_16512
        test   dh, 20h
        jne    SHORT loc_1_16762
        mov    ecx, edx
        shr    ecx, 5
        jmp    NEAR PTR loc_1_16505
loc_1_16762:
        test   byte ptr [ebp - 7], 10h
        je     NEAR PTR loc_1_16512
        cmp    word ptr [ebp - 2], 15h
        ja     NEAR PTR loc_1_16512
        shl    dword ptr [ebp - 8], 12h
loc_1_1677B:
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 17h
        add    word ptr [ebp - 2], 9
        and    ax, 7fh
        mov    word ptr [ebp - 4], ax
        or     ax, ax
        je     NEAR PTR loc_1_16573
        shl    dword ptr [ebp - 8], 9
loc_1_1679B:
        cmp    dword ptr [ebp - 8], 0c0000000h
        jae    NEAR PTR loc_1_165F2
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    dword ptr [ebp - 10h], eax
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [eax + 1]
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 2
        mov    byte ptr [eax + 1], cl
        test   byte ptr [ebp - 5], 40h
        je     SHORT loc_1_16809
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [eax + 2]
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 0ch]
        mov    byte ptr [eax], cl
        jmp    SHORT loc_1_1682A
loc_1_167E6:
        shl    dword ptr [ebp - 8], 9
loc_1_167EA:
        test   byte ptr [ebp - 5], 40h
        jne    SHORT loc_1_1677B
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 13h
        shl    dword ptr [ebp - 8], 0dh
        and    ax, 7ffh
        add    word ptr [ebp - 2], 0dh
        mov    word ptr [ebp - 4], ax
        jmp    SHORT loc_1_1679B
loc_1_16809:
        test   byte ptr [ebp - 5], 80h
        je     SHORT loc_1_1682A
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [eax + 2]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [eax + 3]
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 2
        mov    byte ptr [eax + 1], cl
loc_1_1682A:
        add    word ptr [ebp - 2], 2
        test   byte ptr [ebp - 5], 20h
        je     NEAR PTR loc_1_16512
        cmp    word ptr [ebp - 2], 15h
        ja     NEAR PTR loc_1_16512
        shl    dword ptr [ebp - 8], 2
        test   byte ptr [ebp - 5], 40h
        jne    NEAR PTR loc_1_1677B
        cmp    word ptr [ebp - 2], 11h
        ja     NEAR PTR loc_1_16512
        jmp    SHORT loc_1_167EA
        db  090h
loc_1_16860:
        mov    dword ptr [ebp - 18h], 0fffffff9h
loc_1_16867:
        add    dword ptr [ebp - 18h], 0fh
        shl    dword ptr [ebp - 8], 4
        add    word ptr [ebp - 2], 4
loc_1_16874:
        cmp    word ptr [ebp - 2], 1ch
        jbe    SHORT loc_1_168E0
        mov    ax, word ptr [ebp - 2]
        and    word ptr [ebp - 2], 7
        shr    ax, 3
        and    eax, 0ffffh
        add    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 0ch], eax
        ja     SHORT loc_1_168A4
loc_1_16898:
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 0ah], 2
        jmp    NEAR PTR loc_1_1659E
loc_1_168A4:
        mov    eax, dword ptr [ebp - 14h]
        xor    edx, edx
        mov    dl, byte ptr [eax + 2]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        shl    eax, 10h
        or     edx, eax
        mov    eax, dword ptr [ebp - 14h]
        shl    edx, 8
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        shl    eax, 10h
        or     edx, eax
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, byte ptr [ebp - 2]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     edx, eax
        shl    edx, cl
        mov    dword ptr [ebp - 8], edx
loc_1_168E0:
        cmp    dword ptr [ebp - 8], 0f0000000h
        jae    NEAR PTR loc_1_16867
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 1ch
        and    eax, 0ffffh
        add    dword ptr [ebp - 18h], eax
loc_1_168FB:
        add    word ptr [ebp - 2], 4
loc_1_16900:
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, dword ptr [ebp - 0ch]
        cmp    eax, dword ptr [ebp - 18h]
        jb     SHORT loc_1_16925
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_173D0
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_16512
loc_1_16925:
        mov    esi, dword ptr [ebp + 14h]
        sub    esi, dword ptr [ebp - 0ch]
        push   esi
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_173D0
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 18h]
        sub    dword ptr [ebp - 18h], esi
        mov    byte ptr [eax + 0ah], 20h
        jmp    NEAR PTR loc_1_16589
        db  08Dh, 064h, 024h, 000h
sub_1_16950:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [edx]
        lea    ecx, [edi + 2]
        cmp    ecx, dword ptr [ebp + 0ch]
        jbe    SHORT loc_1_16970
        mov    eax, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  090h
loc_1_16970:
        mov    esi, dword ptr [ebp + 10h]
        xor    ebx, ebx
        mov    cx, word ptr [esi + 4]
        mov    bl, byte ptr [edi]
        xor    eax, eax
        mov    al, byte ptr [edi + 1]
        shl    ebx, 8
        or     ebx, eax
        shl    ebx, cl
        and    ebx, 0ff80h
        cmp    ebx, 0c000h
        jne    SHORT loc_1_169C6
        add    cx, 9
        mov    word ptr [esi + 4], cx
        shr    cx, 3
        mov    eax, ecx
        and    eax, 0ffffh
        add    dword ptr [edx], eax
        mov    ax, word ptr [esi + 4]
        and    ax, 7
        mov    word ptr [esi + 4], ax
        je     SHORT loc_1_169BA
        inc    dword ptr [edx]
loc_1_169BA:
        mov    eax, 80h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_169C6:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  090h
sub_1_169D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 24h], 0
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 0ffffh
        test   eax, 0fffffff9h
        je     SHORT loc_1_16A1B
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_16A1B:
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        jne    SHORT loc_1_16A34
        mov    ax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_16A34:
        mov    eax, dword ptr [ebp + 18h]
        cmp    word ptr [eax + 14h], 1234h
        je     SHORT loc_1_16A50
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  005h, 000h, 000h, 000h, 000h
loc_1_16A50:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4ch], eax
        add    edx, 17h
        mov    dword ptr [ebp - 8], eax
        add    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 1ch], eax
        mov    cl, byte ptr [edx]
        or     cl, cl
        jne    SHORT loc_1_16AA2
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_16A83
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_16A83:
        cmp    eax, 3
        jae    SHORT loc_1_16A94
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_16A94:
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp - 4ch]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_16C89
loc_1_16AA2:
        test   byte ptr [ebp + 1ch], 2
        je     SHORT loc_1_16AC0
        xor    eax, eax
        mov    al, cl
        test   eax, 0ffffffefh
        je     SHORT loc_1_16AC0
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  090h
loc_1_16AC0:
        test   cl, 10h
        je     NEAR PTR loc_1_16C33
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp - 4ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, ecx
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 20h], eax
        test   byte ptr [edx], 20h
        jne    NEAR PTR loc_1_16CC1
loc_1_16AE9:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 20h], eax
        jb     NEAR PTR loc_1_16B80
        or     word ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 17h
        mov    dword ptr [ebp - 48h], eax
        and    byte ptr [eax], 0f7h
        mov    si, word ptr [ebp - 10h]
        mov    eax, dword ptr [ebp + 18h]
        push   esi
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_17300
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp - 10h]
        add    dword ptr [ecx], eax
        mov    cx, word ptr [ebp - 20h]
        mov    eax, dword ptr [ebp + 18h]
        sub    cx, si
        mov    word ptr [eax + 0ah], cx
        jne    NEAR PTR loc_1_17240
        mov    eax, dword ptr [ebp - 48h]
        test   byte ptr [eax], 20h
        jne    NEAR PTR loc_1_17240
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 18h
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp + 18h]
        add    eax, 18h
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_1_16950
        test   al, 80h
        je     NEAR PTR loc_1_17240
        jmp    NEAR PTR loc_1_17152
        db  08Bh, 0FFh
loc_1_16B80:
        mov    eax, dword ptr [ebp - 20h]
        mov    word ptr [ebp - 40h], ax
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_17300
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 17h
        mov    dword ptr [ebp - 3ch], eax
        mov    al, byte ptr [eax]
        test   al, 20h
        jne    SHORT loc_1_16BB4
        test   al, 80h
        jne    NEAR PTR loc_1_17152
loc_1_16BB4:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp + 18h]
        add    ax, word ptr [ebp - 40h]
        sub    ax, 1fh
        mov    word ptr [ebp - 2], ax
        cmp    ax, 7ffh
        jbe    SHORT loc_1_16BF3
        push   7ffh
        and    eax, 0ffffh
        add    eax, dword ptr [ebp + 18h]
        sub    eax, 7e0h
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 1fh
        push   eax
        call   sub_1_17300
        mov    word ptr [ebp - 2], 7ffh
loc_1_16BF3:
        mov    eax, dword ptr [ebp - 20h]
        sub    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 20h], 0
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        add    eax, dword ptr [ebp + 18h]
        add    eax, 1fh
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 30h], eax
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 3ch]
        test   byte ptr [eax], 20h
        jne    NEAR PTR loc_1_16CC1
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 18h], eax
        mov    eax, ecx
        add    eax, 102fh
        mov    dword ptr [ebp - 1ch], eax
loc_1_16C33:
        mov    ecx, dword ptr [ebp + 18h]
        add    ecx, 17h
        mov    eax, dword ptr [ebp + 18h]
        and    byte ptr [ecx], 0bfh
        mov    ax, word ptr [eax + 8]
        or     ax, ax
        je     SHORT loc_1_16C89
        mov    edx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [edx]
        cmp    edx, 4
        jb     SHORT loc_1_16C56
        mov    dx, 4
loc_1_16C56:
        and    eax, 0ffffh
        add    eax, dword ptr [ebp + 18h]
        add    eax, 18h
        mov    dword ptr [ebp - 14h], eax
        or     dx, dx
        je     SHORT loc_1_16C7D
loc_1_16C69:
        mov    eax, dword ptr [ebp - 0ch]
        mov    bl, byte ptr [eax]
        mov    eax, dword ptr [ebp - 14h]
        inc    dword ptr [ebp - 14h]
        mov    byte ptr [eax], bl
        inc    dword ptr [ebp - 0ch]
        dec    dx
        jne    SHORT loc_1_16C69
loc_1_16C7D:
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 18h
        mov    dword ptr [ebp - 0ch], eax
        or     byte ptr [ecx], 4
loc_1_16C89:
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 2
        mov    word ptr [ebp - 50h], ax
loc_1_16C95:
        cmp    word ptr [ebp - 50h], 0
        mov    eax, dword ptr [ebp + 18h]
        je     NEAR PTR loc_1_16D40
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_17280
        jmp    NEAR PTR loc_1_16D59
loc_1_16CC1:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp + 18h]
        sub    eax, dword ptr [ebp - 28h]
        add    ax, 102fh
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 0ch
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 2ch], eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        cmp    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 2ch]
        jbe    SHORT loc_1_16D03
        mov    ecx, dword ptr [ebp - 34h]
        sub    eax, dword ptr [ebp - 38h]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_16D12
loc_1_16D03:
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp + 18h]
        and    byte ptr [eax + 17h], 0dfh
        or     byte ptr [eax + 16h], 1
loc_1_16D12:
        cmp    word ptr [ebp - 2], 0
        je     NEAR PTR loc_1_16AE9
        xor    esi, esi
        mov    si, word ptr [ebp - 2]
        mov    eax, dword ptr [ebp + 18h]
        push   esi
        mov    ax, word ptr [eax + 12h]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_1_173D0
        add    dword ptr [ebp - 20h], esi
        jmp    NEAR PTR loc_1_16AE9
        db  08Dh, 049h, 000h
loc_1_16D40:
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_16420
loc_1_16D59:
        test   al, 80h
        jne    NEAR PTR loc_1_16FC1
        test   al, 7
        jne    NEAR PTR loc_1_16EB5
        test   al, 20h
        mov    esi, dword ptr [ebp + 18h]
        je     NEAR PTR loc_1_16E00
        mov    al, byte ptr [esi + 17h]
        add    esi, 17h
        or     al, 20h
        mov    byte ptr [esi], al
        test   al, 10h
        je     NEAR PTR loc_1_17050
        mov    eax, dword ptr [ebp - 1ch]
        sub    eax, dword ptr [ebp - 18h]
        mov    edi, eax
        and    edi, 0ffffh
        cmp    dword ptr [ebp - 10h], edi
        jbe    NEAR PTR loc_1_170A2
        sub    dword ptr [ebp - 10h], edi
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_17300
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp + 18h]
        sub    ax, 1fh
        add    dword ptr [ebp - 24h], edi
        cmp    ax, 7ffh
        jbe    SHORT loc_1_16DE4
        push   7ffh
        and    eax, 0ffffh
        add    eax, dword ptr [ebp + 18h]
        sub    eax, 7e0h
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 1fh
        push   eax
        call   sub_1_17300
        mov    ax, 7ffh
loc_1_16DE4:
        and    eax, 0ffffh
        add    eax, dword ptr [ebp + 18h]
        add    eax, 1fh
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 18h], eax
        and    byte ptr [esi], 0dfh
        jmp    NEAR PTR loc_1_16C95
        db  08Dh, 049h, 000h
loc_1_16E00:
        mov    al, byte ptr [esi + 17h]
        add    esi, 17h
        test   al, 8
        jne    NEAR PTR loc_1_170A2
        test   al, 10h
        je     SHORT loc_1_16E50
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 18h]
        mov    ecx, eax
        and    ecx, 0ffffh
        cmp    ecx, dword ptr [ebp - 10h]
        ja     NEAR PTR loc_1_170A2
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_17300
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [ebp - 4ch]
        mov    dword ptr [ebp - 1ch], eax
        and    byte ptr [esi], 0efh
        jmp    NEAR PTR loc_1_16C95
        db  08Dh, 049h, 000h
loc_1_16E50:
        or     al, 18h
        mov    byte ptr [esi], al
        mov    eax, dword ptr [ebp - 1ch]
        sub    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 24h], eax
        sub    eax, dword ptr [ecx + 4]
        cmp    eax, 7ffh
        jbe    SHORT loc_1_16E75
        mov    eax, 7ffh
loc_1_16E75:
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        and    eax, 0ffffh
        sub    ecx, eax
        push   ecx
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 1fh
        push   eax
        call   sub_1_17300
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 18h], eax
        mov    eax, ecx
        add    eax, 102fh
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_16C95
        jmp    NEAR PTR loc_1_170A2
loc_1_16EB5:
        sub    ecx, ecx
        mov    edi, dword ptr [ebp + 18h]
        mov    dl, byte ptr [edi + 17h]
        add    edi, 17h
        test   dl, 4
        je     SHORT loc_1_16F14
        mov    ecx, dword ptr [ebp - 0ch]
        sub    ecx, dword ptr [ebp + 18h]
        sub    ecx, 18h
        mov    esi, dword ptr [ebp + 18h]
        add    esi, 8
        xor    ebx, ebx
        mov    bx, word ptr [esi]
        cmp    ebx, ecx
        jbe    SHORT loc_1_16EE4
        or     dl, 40h
        mov    byte ptr [edi], dl
        jmp    SHORT loc_1_16F14
loc_1_16EE4:
        and    dl, 0fbh
        mov    byte ptr [edi], dl
        mov    edx, dword ptr [ebp + 8]
        mov    edi, ecx
        mov    edx, dword ptr [edx]
        mov    dword ptr [ebp - 0ch], edx
        mov    edx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [edx]
        add    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 14h], edx
        xor    edx, edx
        mov    dx, word ptr [esi]
        sub    edi, edx
        mov    edx, dword ptr [ebp + 10h]
        add    dword ptr [ebp - 0ch], edi
        cmp    dword ptr [edx], 4
        jae    NEAR PTR loc_1_16C95
loc_1_16F14:
        test   al, 4
        jne    SHORT loc_1_16F24
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 14h], eax
        jne    NEAR PTR loc_1_16C95
loc_1_16F24:
        mov    esi, dword ptr [ebp + 18h]
        add    esi, 17h
        test   byte ptr [esi], 10h
        je     SHORT loc_1_16F47
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 18h]
        mov    word ptr [edx + 0ah], ax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 8], eax
loc_1_16F47:
        or     word ptr [ebp - 4], 1
        test   byte ptr [esi], 40h
        je     NEAR PTR loc_1_17128
        or     ecx, ecx
        je     SHORT loc_1_16F9D
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 10h]
        xor    edx, edx
        mov    dx, word ptr [eax]
        mov    eax, ecx
        and    eax, 0ffffh
        sub    edx, eax
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        sub    si, si
        add    edx, eax
        add    edx, dword ptr [ebp - 0ch]
        cmp    edx, dword ptr [ebp - 0ch]
        jbe    SHORT loc_1_16F9D
loc_1_16F80:
        mov    eax, dword ptr [ebp - 0ch]
        mov    bl, byte ptr [eax]
        mov    eax, esi
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    byte ptr [eax + ecx + 18h], bl
        inc    dword ptr [ebp - 0ch]
        inc    si
        cmp    edx, dword ptr [ebp - 0ch]
        ja     SHORT loc_1_16F80
loc_1_16F9D:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        add    word ptr [ecx + 8], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        add    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_17240
loc_1_16FC1:
        mov    ecx, dword ptr [ebp + 18h]
        mov    al, byte ptr [ecx + 17h]
        add    ecx, 17h
        test   al, 4
        je     SHORT loc_1_17003
        and    al, 0fbh
        mov    byte ptr [ecx], al
        mov    edx, dword ptr [ebp - 0ch]
        sub    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 8]
        sub    edx, 18h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        add    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        cmp    edx, eax
        jbe    SHORT loc_1_17003
        sub    edx, eax
        add    dword ptr [ebp - 0ch], edx
loc_1_17003:
        mov    al, byte ptr [ecx]
        test   al, 10h
        je     SHORT loc_1_17035
        or     al, 80h
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 18h]
        mov    ecx, eax
        and    ecx, 0ffffh
        cmp    ecx, dword ptr [ebp - 10h]
        ja     NEAR PTR loc_1_170A2
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_17300
        mov    dword ptr [ebp - 8], eax
loc_1_17035:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 14h]
        sub    eax, dword ptr [ebp - 4ch]
        cmp    eax, dword ptr [ecx]
        jne    NEAR PTR loc_1_17152
        or     word ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_17152
loc_1_17050:
        mov    esi, dword ptr [ebp + 18h]
        add    esi, 17h
        mov    ecx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp + 18h]
        or     byte ptr [esi], 10h
        sub    ecx, dword ptr [eax + 4]
        cmp    ecx, 7ffh
        jbe    SHORT loc_1_1706F
        mov    ecx, 7ffh
loc_1_1706F:
        mov    edi, ecx
        and    edi, 0ffffh
        push   ecx
        mov    eax, dword ptr [ebp - 1ch]
        sub    eax, edi
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 1fh
        push   eax
        call   sub_1_17300
        mov    ecx, dword ptr [ebp + 18h]
        add    ecx, edi
        mov    eax, dword ptr [ebp + 18h]
        add    ecx, 1fh
        mov    dword ptr [eax], ecx
        mov    word ptr [eax + 0ah], 0
        test   byte ptr [esi], 8
        je     SHORT loc_1_170D9
loc_1_170A2:
        mov    esi, dword ptr [ebp + 18h]
        add    esi, 17h
        mov    eax, dword ptr [ebp - 10h]
        and    byte ptr [esi], 0f7h
        mov    ecx, dword ptr [ebp + 18h]
        add    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 8]
        sub    ecx, eax
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 0ah], cx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_17300
        mov    dword ptr [ebp - 8], eax
        or     byte ptr [esi], 10h
loc_1_170D9:
        or     word ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp + 18h]
        mov    cl, byte ptr [eax + 17h]
        add    eax, 17h
        test   cl, 4
        je     SHORT loc_1_17112
        and    cl, 0fbh
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 18h]
        sub    eax, dword ptr [ebp + 18h]
        sub    ax, word ptr [ecx + 8]
        sub    ax, 18h
        mov    ecx, eax
        mov    eax, dword ptr [ebp + 8]
        and    ecx, 0ffffh
        add    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], ecx
loc_1_17112:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 14h], eax
        jne    SHORT loc_1_17121
        or     word ptr [ebp - 4], 1
        jmp    SHORT loc_1_17128
loc_1_17121:
        mov    eax, dword ptr [ebp - 0ch]
        inc    eax
        mov    dword ptr [ebp - 14h], eax
loc_1_17128:
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_16950
        test   al, 80h
        je     NEAR PTR loc_1_17200
        mov    eax, dword ptr [ebp + 18h]
        cmp    word ptr [eax + 0ah], 0
        jne    NEAR PTR loc_1_17200
loc_1_17152:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        sub    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 10h]
        cmp    ecx, dword ptr [eax]
        jne    SHORT loc_1_17166
        or     word ptr [ebp - 4], 1
loc_1_17166:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    esi, dword ptr [eax]
        sub    esi, eax
        sub    esi, 1fh
        call   sub_1_163E0
        test   byte ptr [ebp + 1ch], 4
        je     SHORT loc_1_171F7
        mov    edi, dword ptr [ebp - 8]
        sub    edi, dword ptr [ebp - 4ch]
        lea    eax, [esi + edi]
        cmp    eax, 7ffh
        jbe    SHORT loc_1_171D0
        cmp    edi, 7ffh
        jbe    SHORT loc_1_171A0
        mov    edi, 7ffh
        sub    esi, esi
        jmp    SHORT loc_1_171D0
        db  08Dh, 049h, 000h
loc_1_171A0:
        mov    ax, 7ffh
        sub    ax, di
        mov    cx, si
        mov    esi, eax
        and    esi, 0ffffh
        sub    cx, si
        push   esi
        mov    eax, ecx
        and    eax, 0ffffh
        add    eax, dword ptr [ebp + 18h]
        add    eax, 1fh
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 1fh
        push   eax
        call   sub_1_17300
loc_1_171D0:
        push   edi
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, edi
        and    eax, 0ffffh
        sub    ecx, eax
        push   ecx
        mov    eax, dword ptr [ebp + 18h]
        add    eax, esi
        add    eax, 1fh
        push   eax
        call   sub_1_17300
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, ecx
        mov    byte ptr [eax + 17h], 10h
loc_1_171F7:
        or     word ptr [ebp - 4], 4
        jmp    SHORT loc_1_17240
        db  08Bh, 0FFh
loc_1_17200:
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 18h]
        or     byte ptr [eax + 17h], 40h
        mov    eax, dword ptr [ebp - 14h]
        sub    dx, dx
        sub    eax, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 14h], eax
        jbe    SHORT loc_1_17240
        nop
loc_1_17220:
        mov    eax, dword ptr [ebp - 0ch]
        mov    bl, byte ptr [eax]
        mov    eax, edx
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    byte ptr [eax + ecx + 18h], bl
        inc    dword ptr [ebp - 0ch]
        inc    dx
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 14h], eax
        ja     SHORT loc_1_17220
loc_1_17240:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4ch]
        mov    ecx, dword ptr [ebp + 14h]
        sub    eax, dword ptr [ebp - 8]
        add    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17280:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0ah], 80h
        sub    edi, dword ptr [ebx]
        mov    esi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, dword ptr [esi]
        cmp    eax, edi
        jae    SHORT loc_1_172AD
        mov    edi, eax
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 0ah], 8
loc_1_172AD:
        or     edi, edi
        je     SHORT loc_1_172E1
loc_1_172B1:
        cmp    edi, 0ffffh
        mov    word ptr [ebp - 2], di
        ja     SHORT loc_1_172C3
        mov    word ptr [ebp - 2], 1
loc_1_172C3:
        mov    ax, word ptr [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_17300
        mov    dword ptr [esi], eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        add    dword ptr [ebx], eax
        sub    edi, eax
        jne    SHORT loc_1_172B1
loc_1_172E1:
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17300:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    cx, word ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, ecx
        and    edx, 0ffffh
        add    edx, eax
        cmp    cx, 7
        mov    esi, dword ptr [ebp + 0ch]
        jbe    NEAR PTR loc_1_173AE
        test   esi, 1
        je     SHORT loc_1_17330
        mov    cl, byte ptr [esi]
        mov    byte ptr [eax], cl
        inc    eax
        inc    esi
loc_1_17330:
        test   esi, 2
        je     SHORT loc_1_17348
        mov    cl, byte ptr [esi]
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [eax + 1], cl
        add    eax, 2
        add    esi, 2
loc_1_17348:
        sub    edx, 3
        test   al, 1
        jne    SHORT loc_1_1738B
        test   al, 2
        je     SHORT loc_1_17374
        cmp    edx, eax
        jbe    SHORT loc_1_173AB
loc_1_17357:
        mov    cx, word ptr [esi]
        mov    word ptr [eax], cx
        mov    cx, word ptr [esi + 2]
        mov    word ptr [eax + 2], cx
        add    eax, 4
        add    esi, 4
        cmp    edx, eax
        ja     SHORT loc_1_17357
        add    edx, 3
        jmp    SHORT loc_1_173AE
loc_1_17374:
        cmp    edx, eax
        jbe    SHORT loc_1_173AB
loc_1_17378:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        add    eax, 4
        add    esi, 4
        cmp    edx, eax
        ja     SHORT loc_1_17378
        add    edx, 3
        jmp    SHORT loc_1_173AE
loc_1_1738B:
        cmp    edx, eax
        jbe    SHORT loc_1_173AB
        nop
loc_1_17390:
        mov    ecx, dword ptr [esi]
        mov    byte ptr [eax], cl
        inc    eax
        shr    ecx, 8
        mov    word ptr [eax], cx
        shr    ecx, 10h
        mov    byte ptr [eax + 2], cl
        add    eax, 3
        add    esi, 4
        cmp    edx, eax
        ja     SHORT loc_1_17390
loc_1_173AB:
        add    edx, 3
loc_1_173AE:
        cmp    edx, eax
        jbe    SHORT loc_1_173BC
loc_1_173B2:
        mov    cl, byte ptr [esi]
        mov    byte ptr [eax], cl
        inc    eax
        inc    esi
        cmp    edx, eax
        ja     SHORT loc_1_173B2
loc_1_173BC:
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_173D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    bx, word ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, ebx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4], eax
        mov    edi, edx
        sub    edi, eax
        mov    ecx, dword ptr [ebp + 10h]
        lea    esi, [edx + ecx]
        cmp    bx, 4
        jae    SHORT loc_1_1741D
        cmp    bx, 1
        jbe    NEAR PTR loc_1_17485
        cmp    ecx, 3
        jb     SHORT loc_1_1746C
        sub    ecx, eax
        or     bx, bx
        je     SHORT loc_1_1741A
loc_1_17410:
        mov    al, byte ptr [edi]
        mov    byte ptr [edx], al
        inc    edi
        inc    edx
        dec    bx
        jne    SHORT loc_1_17410
loc_1_1741A:
        sub    edi, dword ptr [ebp - 4]
loc_1_1741D:
        test   cl, 1
        je     SHORT loc_1_17428
        mov    al, byte ptr [edi]
        mov    byte ptr [edx], al
        inc    edi
        inc    edx
loc_1_17428:
        test   cl, 2
        je     SHORT loc_1_1743D
        mov    al, byte ptr [edi]
        mov    byte ptr [edx], al
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [edx + 1], al
        add    edx, 2
        add    edi, 2
loc_1_1743D:
        cmp    edx, esi
        jae    SHORT loc_1_17461
loc_1_17441:
        mov    al, byte ptr [edi]
        mov    byte ptr [edx], al
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [edx + 1], al
        add    edx, 4
        mov    al, byte ptr [edi + 2]
        mov    byte ptr [edx - 2], al
        mov    al, byte ptr [edi + 3]
        mov    byte ptr [edx - 1], al
        add    edi, 4
        cmp    edx, esi
        jb     SHORT loc_1_17441
loc_1_17461:
        mov    eax, edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1746C:
        cmp    edx, esi
        jae    SHORT loc_1_1747A
loc_1_17470:
        mov    al, byte ptr [edi]
        mov    byte ptr [edx], al
        inc    edi
        inc    edx
        cmp    edx, esi
        jb     SHORT loc_1_17470
loc_1_1747A:
        mov    eax, edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_17485:
        movzx  edi, byte ptr [edi]
        mov    eax, edi
        shl    eax, 8
        or     eax, edi
        mov    edi, eax
        shl    edi, 10h
        or     eax, edi
        cmp    ecx, 7
        jbe    SHORT loc_1_174CD
        test   dl, 1
        je     SHORT loc_1_174A3
        mov    byte ptr [edx], al
        inc    edx
loc_1_174A3:
        test   dl, 2
        je     SHORT loc_1_174B0
        mov    byte ptr [edx], al
        mov    byte ptr [edx + 1], al
        add    edx, 2
loc_1_174B0:
        sub    esi, 3
        cmp    edx, esi
        jae    SHORT loc_1_174CA
        mov    ebx, esi
        sub    ebx, edx
        add    ebx, 3
        shr    ebx, 2
        mov    edi, edx
        mov    ecx, ebx
        rep stosd
        lea    edx, [edx + ebx*4]
loc_1_174CA:
        add    esi, 3
loc_1_174CD:
        cmp    edx, esi
        jae    SHORT loc_1_174F6
        and    eax, 0ffh
        mov    ch, al
        mov    cl, al
        sub    esi, edx
        mov    eax, ecx
        shl    eax, 10h
        mov    ax, cx
        mov    edi, edx
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
        add    edx, esi
loc_1_174F6:
        mov    eax, edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17510:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dx, word ptr [ebp + 10h]
        mov    esi, dword ptr [eax]
        mov    ax, dx
        and    eax, 0fff8h
        mov    ecx, dword ptr [ebp + 8]
        add    eax, ecx
        cmp    ecx, eax
        jae    SHORT loc_1_1753C
        mov    edi, edi
loc_1_17530:
        mov    dword ptr [ecx], esi
        mov    dword ptr [ecx + 4], esi
        add    ecx, 8
        cmp    ecx, eax
        jb     SHORT loc_1_17530
loc_1_1753C:
        test   dl, 4
        je     SHORT loc_1_17546
        mov    dword ptr [ecx], esi
        add    ecx, 4
loc_1_17546:
        test   dl, 2
        je     SHORT loc_1_1754E
        mov    word ptr [ecx], si
loc_1_1754E:
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_17560:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], 73ah
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 14h], eax
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], esi
        lea    eax, [ebx + 2]
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [ebx]
        push   eax
        push   15h
        lea    eax, [ebx + 73ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_155B0
        mov    eax, 73ah
        sub    eax, dword ptr [ebp - 8]
        cmp    dword ptr [edi], eax
        jae    SHORT loc_1_175D0
        mov    byte ptr [ebp - 1], 80h
        push   ebx
        call   sub_1_17660
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_1_175D0:
        mov    byte ptr [ebp - 1], 20h
        mov    dword ptr [edi], eax
        push   eax
        lea    eax, [ebx + 2]
        push   eax
        push   esi
        call   sub_1_083C
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_175F0:
        push   ebp
        mov    ebp, esp
        call   sub_1_15560
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    eax, dword ptr [ebp + 8]
        add    ecx, 73dh
        mov    dword ptr [eax], ecx
        pop    ebp
        ret    4
        db  090h
loc_1_17610:
        push   ebp
        mov    ebp, esp
        call   sub_1_16410
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    eax, dword ptr [ebp + 8]
        inc    ecx
        mov    dword ptr [eax], ecx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_17630:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 73ch]
        push   eax
        call   sub_1_15570
        mov    word ptr [esi], 7
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_17650:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_163E0
        pop    ebp
        ret    4
sub_1_17660:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        mov    dword ptr [ebp - 10h], 73ah
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 0
        lea    eax, [ecx + 2]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], eax
        mov    ax, word ptr [ecx]
        push   eax
        add    ecx, 73ch
        push   11h
        push   ecx
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_155B0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_176B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_163E0
        pop    ebp
        ret    4
loc_1_176C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
        push   4
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_169D0
        mov    ecx, dword ptr [ebp - 4]
        sub    dword ptr [esi], ecx
        mov    cx, ax
        or     cx, 4
        je     SHORT loc_1_1771C
        or     ax, 1
        je     SHORT loc_1_1771C
        mov    eax, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1771C:
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17730:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        mov    esi, dword ptr [ebp + 8]
        mov    edi, edi
loc_1_17740:
        mov    byte ptr [esi + eax], al
        inc    eax
        cmp    eax, 100h
        jl     SHORT loc_1_17740
        mov    byte ptr [esi + 101h], 0
        mov    byte ptr [esi + 100h], 0
        sub    dl, dl
        mov    byte ptr [ebp - 1], dl
        sub    ecx, ecx
loc_1_17760:
        mov    edi, edx
        and    edi, 0ffh
        mov    eax, dword ptr [ebp + 10h]
        xor    edx, edx
        mov    dl, byte ptr [edi + eax]
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        mov    bl, byte ptr [esi + ecx]
        add    edx, eax
        xor    eax, eax
        mov    al, bl
        add    eax, edx
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 0ffh
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 1], al
        and    eax, 0ffh
        lea    edx, [esi + eax]
        inc    ecx
        mov    al, byte ptr [edx]
        mov    byte ptr [esi + ecx - 1], al
        mov    byte ptr [edx], bl
        lea    eax, [edi + 1]
        cdq
        idiv   dword ptr [ebp + 0ch]
        cmp    ecx, 100h
        jl     SHORT loc_1_17760
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_177C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    al, byte ptr [edi + 100h]
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [edi + 101h]
        mov    byte ptr [ebp - 1], al
        cmp    dword ptr [ebp + 0ch], 0
        jle    SHORT loc_1_17865
loc_1_177EB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        inc    eax
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 0ffh
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 2], al
        and    eax, 0ffh
        lea    ecx, [edi + eax]
        xor    eax, eax
        mov    bl, byte ptr [ecx]
        mov    al, byte ptr [ebp - 1]
        mov    esi, ebx
        and    esi, 0ffh
        add    eax, esi
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 0ffh
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 1], al
        and    eax, 0ffh
        lea    edx, [edi + eax]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx], al
        mov    byte ptr [edx], bl
        xor    eax, eax
        mov    al, byte ptr [ecx]
        add    eax, esi
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 0ffh
        xor    eax, edx
        sub    eax, edx
        mov    dl, byte ptr [edi + eax]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 8]
        xor    byte ptr [eax + ecx], dl
        inc    dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    ecx, dword ptr [ebp + 0ch]
        jl     SHORT loc_1_177EB
loc_1_17865:
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi + 100h], al
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edi + 101h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_17880:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    dword ptr [ebp - 4ch], 4ch
        mov    word ptr [ebp - 48h], 1
        mov    word ptr [ebp - 46h], 23c0h
        mov    word ptr [ebp - 44h], 0
        mov    dword ptr [ebp - 42h], 24h
        mov    byte ptr [ebp - 3eh], 1
        mov    byte ptr [ebp - 3dh], 1
        mov    dword ptr [ebp - 3ch], 0
        mov    dword ptr [ebp - 38h], 0
        mov    dword ptr [ebp - 34h], 0
        db  0C7h, 045h, 0D0h, 040h, 079h, 001h, 000h
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 28h], 0
        db  0C7h, 045h, 0DCh, 0E0h, 079h, 001h, 000h
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 020h, 07Ah, 001h, 000h
        xor    eax, eax
        db  0C7h, 045h, 0E8h, 060h, 07Ah, 001h, 000h
        db  0C7h, 045h, 0ECh, 0C0h, 07Ah, 001h, 000h
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        db  066h, 0A1h, 006h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], eax
        lea    eax, [ebp - 4ch]
        push   eax
        call   PPPMAC_Service_01
        add    esp, 4
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_17940:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        mov    edi, dword ptr [eax + 0ch]
        lea    esi, [ecx + 4bh]
        cmp    dword ptr [edi + 0dch], ecx
        jne    SHORT loc_1_1799A
        mov    dword ptr [edi + 0d4h], 0
        mov    dword ptr [edi + 0dch], 0
        mov    dword ptr [edi + 0e0h], 0
        mov    eax, dword ptr [edi + 0d8h]
        or     eax, eax
        je     SHORT loc_1_1799A
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_075C
        mov    dword ptr [edi + 0d8h], 0
loc_1_1799A:
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_1_179B4
        mov    dword ptr [esi + 10h], 0
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        call   sub_1_0738
loc_1_179B4:
        cmp    dword ptr [esi + 20h], 0
        je     SHORT loc_1_179CE
        mov    dword ptr [esi + 20h], 0
        lea    eax, [ebp - 2]
        push   eax
        add    esi, 14h
        push   esi
        call   sub_1_0738
loc_1_179CE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_179E0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        lea    edx, [ecx + 4bh]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 0feh]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], 0
        db  0C7h, 042h, 008h, 0B0h, 080h, 001h, 000h
        mov    dword ptr [edx + 0ch], ecx
        mov    dword ptr [edx + 14h], 0
        db  0C7h, 042h, 018h, 0F0h, 080h, 001h, 000h
        mov    dword ptr [edx + 1ch], ecx
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_17A20:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 10h], 0
        mov    eax, dword ptr [eax + 0feh]
        je     SHORT loc_1_17A50
        mov    ecx, dword ptr [eax + 28h]
        add    cx, word ptr [eax + 20h]
        mov    eax, dword ptr [ebp + 0ch]
        add    cx, 2
        mov    word ptr [eax], cx
        mov    al, 1
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_17A50:
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    word ptr [eax], 0
        sub    al, al
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_17A60:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 10h], 0
        mov    edi, dword ptr [ecx + 0feh]
        je     SHORT loc_1_17AAC
        mov    esi, dword ptr [ebp + 0ch]
        mov    al, byte ptr [edi + 20h]
        mov    byte ptr [esi], al
        mov    eax, dword ptr [edi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 30h]
        push   eax
        inc    esi
        push   esi
        call   sub_1_083C
        add    esi, dword ptr [edi + 20h]
        mov    al, byte ptr [edi + 28h]
        mov    byte ptr [esi], al
        mov    eax, dword ptr [edi + 28h]
        inc    esi
        push   eax
        mov    eax, dword ptr [edi + 38h]
        push   eax
        push   esi
        call   sub_1_083C
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_17AAC:
        mov    esi, dword ptr [ebp + 0ch]
        pop    edi
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1], 0
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_17AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 2fh]
        mov    edx, dword ptr [esi + 4]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [edx + 0ch]
        cmp    byte ptr [esi + 10h], 3
        mov    edi, dword ptr [eax + 0feh]
        jne    SHORT loc_1_17AEC
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_17AEC:
        xor    eax, eax
        mov    al, cl
        cmp    eax, 1
        je     SHORT loc_1_17B11
        cmp    eax, 2
        je     NEAR PTR loc_1_17BA5
        cmp    eax, 3
        je     NEAR PTR loc_1_17C60
        mov    byte ptr [esi + 11h], 0ch
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_17B11:
        cmp    dword ptr [edi], 0
        jne    NEAR PTR loc_1_17B99
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_17B41
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0DAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0F6h, 0A0h, 0FCh, 0FFh
        add    esp, 0ch
loc_1_17B41:
        mov    eax, dword ptr [esi + 35h]
        mov    byte ptr [eax], 2
        mov    eax, dword ptr [esi + 2fh]
        mov    cl, byte ptr [eax + 1]
        mov    eax, dword ptr [esi + 35h]
        mov    byte ptr [eax + 1], cl
        mov    eax, dword ptr [esi + 35h]
        mov    word ptr [eax + 2], 500h
        mov    eax, dword ptr [esi + 35h]
        mov    byte ptr [eax + 4], 0
        mov    word ptr [esi + 39h], 5
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_17B8F
        xor    eax, eax
        db  066h, 0A1h, 0DCh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0A8h, 0A0h, 0FCh, 0FFh
        add    esp, 8
loc_1_17B8F:
        mov    byte ptr [esi + 11h], 6
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_17B99:
        push   esi
        call   sub_1_17D10
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_17BA5:
        cmp    dword ptr [edx + 10h], 0
        jne    SHORT loc_1_17BE0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_17D06
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0DAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 061h, 0A0h, 0FCh, 0FFh
        add    esp, 0ch
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_17BE0:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_17C06
        xor    eax, eax
        db  066h, 0A1h, 0DEh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 031h, 0A0h, 0FCh, 0FFh
        add    esp, 8
loc_1_17C06:
        cmp    byte ptr [esi + 10h], 9
        je     SHORT loc_1_17C50
        sub    al, al
        sub    cl, cl
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_17C17
        mov    al, 1
loc_1_17C17:
        mov    edx, dword ptr [esi + 4]
        cmp    dword ptr [edx + 10h], 0
        je     SHORT loc_1_17C22
        mov    cl, 1
loc_1_17C22:
        cmp    cl, al
        je     SHORT loc_1_17C42
        mov    byte ptr [esi + 11h], 6
        mov    word ptr [esi + 39h], 0
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_17C42:
        mov    byte ptr [esi + 11h], 8
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_17C50:
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_17C60:
        lea    edi, [esi + 4bh]
        cmp    dword ptr [edx + 10h], 0
        jne    SHORT loc_1_17CA0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_17D06
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0DAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0A3h, 09Fh, 0FCh, 0FFh
        add    esp, 0ch
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_17CA0:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_17CC6
        xor    eax, eax
        db  066h, 0A1h, 0E0h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 071h, 09Fh, 0FCh, 0FFh
        add    esp, 8
loc_1_17CC6:
        mov    eax, dword ptr [esi + 4]
        push   3e8h
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [eax + 0d0h], 803h
        lea    eax, [edi + 14h]
        push   eax
        call   sub_1_0732
        mov    dword ptr [edi + 20h], 1
        mov    byte ptr [esi + 11h], 10h
        cmp    byte ptr [esi + 22h], 0
        je     SHORT loc_1_17D06
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_0738
        mov    byte ptr [esi + 22h], 0
loc_1_17D06:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_17D10:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi + 4bh]
        mov    eax, dword ptr [esi + 4]
        mov    ebx, dword ptr [esi + 2fh]
        mov    edi, dword ptr [eax + 0ch]
        cmp    dword ptr [ecx + 10h], 0
        mov    ax, word ptr [esi + 33h]
        mov    word ptr [ebp - 4], ax
        je     SHORT loc_1_17D4A
        mov    dword ptr [ecx + 10h], 0
        lea    eax, [ebp - 1]
        push   eax
        add    ecx, 4
        push   ecx
        call   sub_1_0738
loc_1_17D4A:
        cmp    dword ptr [edi + 0dch], 0
        je     SHORT loc_1_17D60
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_17D60:
        cmp    word ptr [ebp - 4], 6
        jae    SHORT loc_1_17D74
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_17D74:
        mov    al, byte ptr [ebx + 4]
        or     al, al
        je     SHORT loc_1_17D81
        lea    ecx, [ebx + 5]
        mov    dword ptr [ebp - 1ch], ecx
loc_1_17D81:
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        lea    ecx, [eax + ebx]
        mov    al, byte ptr [ecx + 5]
        or     al, al
        je     SHORT loc_1_17D99
        add    ecx, 6
        mov    dword ptr [ebp - 18h], ecx
loc_1_17D99:
        xor    ecx, ecx
        mov    cl, al
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], ecx
        add    ecx, dword ptr [ebp - 0ch]
        add    ecx, 6
        and    eax, 0ffffh
        cmp    ecx, eax
        jle    SHORT loc_1_17DC0
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_17DC0:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [eax + ecx + 2ah]
        mov    dword ptr [ebp - 14h], eax
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_17DF5
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_17DF5:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10h], 800h
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 20h], ecx
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebx + 1]
        mov    dword ptr [ecx + 14h], eax
        mov    ebx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp - 1ch]
        push   ecx
        add    ebx, 28h
        push   ebx
        call   sub_1_083C
        add    ebx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ebx], 0
        inc    ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 18h]
        push   ecx
        push   ebx
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [ebx + eax], 0
        mov    dword ptr [edi + 0d0h], 800h
        mov    dword ptr [edi + 0d4h], 900h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edi + 0d8h], eax
        mov    dword ptr [edi + 0dch], esi
        db  0C7h, 087h, 0E0h, 000h, 000h, 000h, 040h, 07Fh, 001h, 000h
        push   edi
        call   PPPMAC_Service_09
        add    esp, 4
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
_LTEXT ends
        end

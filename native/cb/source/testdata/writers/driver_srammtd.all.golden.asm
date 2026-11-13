; SRAMMTD -- a virtual device driver, reconstructed from the image by CodeBreaker.
;
; One segment per object of the original. Nothing here is an address: every place the
; loader patches is written as the label it refers to. Labels are L<object>_<offset in the
; original>; names the image itself carries (entry points, the descriptor block) are used
; where there are any. Instructions written as bytes are ones the assembler would encode
; differently from the original; their mnemonic is in the comment.

.386p
.model flat

; object 1: 1648 bytes, preload
S1 segment dword public use32 'O1CODE'
L1_00000:
        enter  8, 0
        mov    dword ptr [ebp - 4], ebx
        cmp    edx, 2
        jne    near ptr L1_00202
        lea    eax, [ebp - 8]
        push   0
        push   ebx
        push   ebx
        push   eax
        int  20h
        dd   000330009h    ; _CONFIGMG_Get_Private_DWord
        add    esp, 010h
        mov    edi, dword ptr [ebp - 8]
        or     edi, edi
        je     near ptr L1_00202
        push   1
        push   038h
        int  20h
        dd   00001004Fh    ; _HeapAllocate
        add    esp, 8
        or     eax, eax
        je     near ptr L1_00202
        mov    dword ptr [ebp - 8], eax
        push   0
        push   eax
        push   ebx
        push   ebx
        int  20h
        dd   00033000Ah    ; _CONFIGMG_Set_Private_DWord
        add    esp, 010h
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 010h], 0FFFFFFFFh
        mov    ax, word ptr [edi]
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [edi + 2]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], eax
        mov    byte ptr [L1_0058E], 0
        mov    ebx, offset L1_0058C
        mov    ax, 0AF0Bh
        mov    cx, 0Eh
        int  20h
        dd   0097C0001h    ; PCCARD_Card_Services
        cmp    word ptr [L1_0058E], 05343h
        jne    near ptr L1_001FA
        cmp    word ptr [L1_00594], 0201h
        je     short L1_000B1
        cmp    word ptr [L1_00594], 0210h
        jne    near ptr L1_001FA
L1_000B1:
        mov    ebx, dword ptr [ebp - 8]
        mov    ax, word ptr [ebx]
        mov    word ptr [L1_005B2], ax
        mov    ebx, offset L1_005B2
        mov    ax, 0AF0Ch
        mov    cx, 6
        int  20h
        dd   0097C0001h    ; PCCARD_Card_Services
        jb     near ptr L1_001FA
        mov    ax, word ptr [L1_005B4]
        mov    ebx, dword ptr [ebp - 8]
        mov    word ptr [ebx + 016h], ax
        mov    word ptr [L1_0059A], 0Ah
        mov    word ptr [L1_0059C], 080h
        mov    word ptr [L1_005A6], 0201h
        lea    esi, [L1_0059E]
        mov    word ptr [esi], 0
        mov    word ptr [esi + 2], 0
        mov    dword ptr [esi + 4], ebx
        mov    esi, offset L1_00283
        mov    ebx, offset L1_0059A
        mov    ax, 0AF10h
        mov    cx, 0Eh
        int  20h
        dd   0097C0001h    ; PCCARD_Card_Services
        jb     near ptr L1_001FA
        mov    ebx, dword ptr [ebp - 8]
        mov    word ptr [ebx + 2], dx
        or     dword ptr [ebx + 0Ch], 1
        mov    ax, word ptr [ebx]
        mov    word ptr [L1_005B8], ax
        mov    dword ptr [L1_005C0], 0
        mov    dword ptr [L1_005BC], 0
        mov    word ptr [L1_005BA], 0Ch
        mov    byte ptr [L1_005C4], 080h
        mov    dx, word ptr [ebx + 2]
        mov    ebx, offset L1_005B8
        mov    ax, 0AF21h
        mov    cx, 0Dh
        int  20h
        dd   0097C0001h    ; PCCARD_Card_Services
        jb     short L1_001FA
        mov    ebx, dword ptr [ebp - 8]
        mov    word ptr [ebx + 014h], dx
        push   0
        push   dword ptr [L1_005C0]
        push   dword ptr [L1_005BC]
        int  20h
        dd   00001006Ch    ; _MapPhysToLinear
        add    esp, 0Ch
        cmp    eax, -1
        je     short L1_001FA
        mov    dword ptr [ebx + 018h], eax
        mov    edx, dword ptr [L1_005C0]
        mov    dword ptr [ebx + 028h], edx
        add    eax, edx
        mov    dword ptr [ebx + 01Ch], eax
        dec    edx
        mov    dword ptr [ebx + 024h], edx
        not    edx
        mov    dword ptr [ebx + 020h], edx
        mov    ax, word ptr [ebx]
        mov    word ptr [L1_005A8], ax
        mov    word ptr [L1_005AA], 01000h
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [L1_005AC], eax
        mov    word ptr [L1_005B0], 0
        mov    dx, word ptr [ebx + 2]
        mov    ebx, offset L1_005A8
        mov    ax, 0AF1Ah
        mov    cx, 0Ah
        int  20h
        dd   0097C0001h    ; PCCARD_Card_Services
        jae    short L1_00208
L1_001FA:
        mov    esi, dword ptr [ebp - 8]
        call   near ptr L1_0022D
L1_00202:
        mov    eax, 013h
        stc
L1_00208:
        leave
        ret
        db   0B8h, 000h, 001h, 000h, 000h, 0F8h, 0C3h
SRAMMTD_control:
        cmp    eax, 022h
        je     near ptr L1_00000
        clc
        ret
L1_0021C:
        mov    ebx, esi
        and    dword ptr [ebx + 0Ch], 0FFFFFFFEh
        test   dword ptr [ebx + 0Ch], 2
        jne    short L1_0022D
        clc
        ret
L1_0022D:
        mov    ebx, esi
        or     dword ptr [ebx + 0Ch], 2
        test   dword ptr [ebx + 0Ch], 1
        stc
        jne    short L1_00282
        movzx  edx, word ptr [ebx + 014h]
        or     edx, edx
        je     short L1_00257
        push   ebx
        mov    ax, 0AF1Dh
        mov    cx, 0
        int  20h
        dd   0097C0001h    ; PCCARD_Card_Services
        pop    ebx
        jb     short L1_00282
L1_00257:
        movzx  edx, word ptr [ebx + 2]
        or     edx, edx
        je     short L1_00271
        push   ebx
        mov    ax, 0AF02h
        mov    cx, 0
        int  20h
        dd   0097C0001h    ; PCCARD_Card_Services
        pop    ebx
        jb     short L1_00282
L1_00271:
        push   0
        push   ebx
        int  20h
        dd   000010051h    ; _HeapFree
        add    esp, 8
        mov    ax, 0
        clc
L1_00282:
        ret
L1_00283:
        cmp    al, 082h
        je     short L1_0021C
        cmp    cx, word ptr [esi]
        jne    short L1_00298
        cmp    al, 012h
        je     near ptr L1_00501
        cmp    al, 5
        je     short L1_0022D
L1_00298:
        mov    ax, 0
        clc
        ret
        db   000h, 000h
L1_002A0:
        push   esi
        and    esi, dword ptr [ebx + 020h]
        cmp    esi, dword ptr [ebx + 010h]
        clc
        je     short L1_002D1
        push   eax
        push   ebx
        push   edx
        push   edi
        mov    dword ptr [ebx + 010h], esi
        mov    edi, esi
        mov    ax, 0
        mov    dx, word ptr [ebx + 014h]
        mov    bh, 0
        mov    bl, byte ptr [ebp + 011h]
        call   dword ptr [ebp + 064h]
        pop    edi
        pop    edx
        pop    ebx
        pop    eax
        jae    short L1_002D1
        mov    dword ptr [ebx + 010h], 0FFFFFFFFh
        stc
L1_002D1:
        pop    esi
        ret
        db   03Ch, 000h, 0B0h, 078h, 075h, 003h, 08Ah, 043h, 02Dh, 0B0h
        db   'x:C,u'
        db   001h, 0C3h, 052h, 053h, 088h, 043h, 02Ch, 066h, 08Bh, 055h, 002h, 08Ah, 0D8h, 08Ah, 0FBh, 066h
        db   0B8h, 003h, 000h, 0FFh, 055h, 064h, 05Bh, 05Ah, 0C3h, 000h, 0F8h, 0C3h
L1_002FE:
        clc
        ret
        db   066h, 02Bh, 0C0h, 0C3h
L1_00304:
        inc    dword ptr [L1_00664]
        db   0C7h, 005h                      ; mov dword ptr [0x7a00030c], 0x7a000310
        dd   offset L1_0066C
        dd   offset L1_00304
        mov    ax, 0
        mov    bx, ax
        ret
L1_0031C:
        inc    dword ptr [L1_0065C]
        db   0C7h, 005h                      ; mov dword ptr [0x7a000324], 0x7a000328
        dd   offset L1_0066C
        dd   offset L1_0031C
        cld
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 4]
        call   near ptr L1_002A0
        jae    short L1_0033D
        jmp    short L1_0039F
L1_0033D:
        and    esi, dword ptr [ebx + 024h]
        add    esi, dword ptr [ebx + 018h]
        mov    edi, dword ptr [ebp + 018h]
        mov    ecx, dword ptr [ebp + 0Ch]
        or     ecx, ecx
        jne    short L1_0034F
        jmp    short L1_003A3
L1_0034F:
        test   esi, 1
        je     short L1_0035B
        movsb
        dec    ecx
        je     short L1_003A3
L1_0035B:
        mov    byte ptr [ebp + 012h], cl
        and    ecx, 0FFFFFFFEh
        mov    edx, ecx
L1_00363:
        mov    ecx, edx
        cmp    esi, dword ptr [ebx + 01Ch]
        jb     short L1_0037C
        mov    esi, dword ptr [ebp + 4]
        add    esi, dword ptr [ebp + 0Ch]
        sub    esi, edx
        call   near ptr L1_002A0
        jb     short L1_0039F
        mov    esi, dword ptr [ebx + 018h]
L1_0037C:
        mov    eax, dword ptr [ebx + 01Ch]
        sub    eax, esi
        cmp    eax, ecx
        jae    short L1_00387
        mov    ecx, eax
L1_00387:
        sub    edx, ecx
        shr    ecx, 1
        jecxz  L1_0039C
        rep movsw
        or     edx, edx
        jne    short L1_00363
        test   byte ptr [ebp + 012h], 1
        jne    short L1_00363
        jmp    short L1_003A3
L1_0039C:
        movsb
        jmp    short L1_003A3
L1_0039F:
        mov    ebx, eax
        jmp    short L1_003A7
L1_003A3:
        mov    bx, 0
L1_003A7:
        mov    ax, 0
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        ret
L1_003B0:
        inc    dword ptr [L1_00660]
        db   0C7h, 005h                      ; mov dword ptr [0x7a0003b8], 0x7a0003bc
        dd   offset L1_0066C
        dd   offset L1_003B0
        cld
        push   ecx
        push   edx
        push   esi
        push   edi
        test   word ptr [ebx + 016h], 1
        je     short L1_003D3
        mov    ax, 01Ah
        jmp    short L1_0044B
L1_003D3:
        mov    esi, dword ptr [ebp + 8]
        call   near ptr L1_002A0
        jae    short L1_003DF
        jmp    short L1_0044B
L1_003DF:
        mov    edi, esi
        and    edi, dword ptr [ebx + 024h]
        add    edi, dword ptr [ebx + 018h]
        mov    esi, dword ptr [ebp + 018h]
        mov    ecx, dword ptr [ebp + 0Ch]
        or     ecx, ecx
        jne    short L1_003F3
        jmp    short L1_00447
L1_003F3:
        test   edi, 1
        je     short L1_00401
        movsb
        dec    ecx
        jne    short L1_00401
        jmp    short L1_00447
L1_00401:
        mov    byte ptr [ebp + 012h], cl
        and    ecx, 0FFFFFFFEh
        mov    edx, ecx
L1_00409:
        mov    ecx, edx
        cmp    edi, dword ptr [ebx + 01Ch]
        jb     short L1_00426
        mov    edi, esi
        mov    esi, dword ptr [ebp + 8]
        add    esi, dword ptr [ebp + 0Ch]
        sub    esi, edx
        call   near ptr L1_002A0
        jb     short L1_0044B
        mov    esi, edi
        mov    edi, dword ptr [ebx + 018h]
L1_00426:
        mov    eax, dword ptr [ebx + 01Ch]
        sub    eax, edi
        cmp    eax, ecx
        jae    short L1_00431
        mov    ecx, eax
L1_00431:
        sub    edx, ecx
        shr    ecx, 1
        jecxz  L1_00446
        rep movsw
        or     edx, edx
        jne    short L1_00409
        test   byte ptr [ebp + 012h], 1
        jne    short L1_00409
        jmp    short L1_00447
L1_00446:
        movsb
L1_00447:
        mov    ax, 0
L1_0044B:
        mov    bx, ax
        mov    ax, 0
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        ret
L1_00457:
        inc    dword ptr [L1_00668]
        db   0C7h, 005h                      ; mov dword ptr [0x7a00045f], 0x7a000463
        dd   offset L1_0066C
        dd   offset L1_00457
        push   edx
        mov    edx, dword ptr [ebp + 4]
        call   near ptr L1_002FE
        jb     short L1_0047C
        mov    edx, dword ptr [ebp + 8]
        call   near ptr L1_002FE
        jae    short L1_00486
L1_0047C:
        mov    ax, 018h
        mov    bx, 0
        pop    edx
        ret
L1_00486:
        pop    edx
        push   dword ptr [ebp + 4]
        push   dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0Ch]
        mov    esi, offset L1_005C8
L1_00495:
        mov    ecx, edx
        jecxz  L1_004E3
        mov    edi, dword ptr [ebp + 4]
        and    edi, 1
        cmp    ecx, 080h
        jb     short L1_004AE
        mov    ecx, 080h
        sub    ecx, edi
L1_004AE:
        add    edi, esi
        mov    dword ptr [ebp + 0Ch], ecx
        mov    dword ptr [ebp + 018h], edi
        push   ebx
        call   near ptr L1_0031C
        or     ax, ax
        jne    short L1_004DE
        pop    ebx
        add    dword ptr [ebp + 4], ecx
        mov    dword ptr [ebp + 0Ch], ecx
        mov    dword ptr [ebp + 018h], edi
        push   ebx
        call   near ptr L1_003B0
        or     ax, ax
        jne    short L1_004DE
        pop    ebx
        add    dword ptr [ebp + 8], ecx
        sub    edx, ecx
        jmp    short L1_00495
L1_004DE:
        add    esp, 4
        jmp    short L1_004EA
L1_004E3:
        mov    ax, 0
        mov    bx, ax
L1_004EA:
        pop    dword ptr [ebp + 8]
        pop    dword ptr [ebp + 4]
        ret
L1_004F1:
        dd   offset L1_00304
        dd   offset L1_0031C
        dd   offset L1_003B0
        dd   offset L1_00457
L1_00501:
        push   dword ptr [ebp + 0Ch]
        push   dword ptr [ebp + 010h]
        push   dword ptr [ebp + 018h]
        push   ebx
        mov    dword ptr [ebp + 018h], ebx
        mov    ebx, esi
        mov    word ptr [ebp + 2], cx
        xchg   esi, eax
        mov    al, byte ptr [ebp + 010h]
        xchg   esi, eax
        and    esi, 3
        shl    esi, 2
        call   dword ptr [esi + L1_004F1]
        mov    word ptr [ebp + 014h], bx
        pop    ebx
        pop    dword ptr [ebp + 018h]
        pop    dword ptr [ebp + 010h]
        pop    dword ptr [ebp + 0Ch]
        or     ax, ax
        jne    short L1_00539
        ret
L1_00539:
        stc
        ret
        db   000h
public SRAMMTD_DDB
SRAMMTD_DDB:
        db   000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h
        db   'SRAMMTD '
        db   000h, 000h, 080h, 016h
        dd   offset SRAMMTD_control
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   'verPP'
        db   000h, 000h, 000h
        db   '1vsR2vsR3vsR'
L1_0058C:
        db   000h, 000h
L1_0058E:
        db   000h, 000h, 000h, 000h, 000h, 000h
L1_00594:
        db   000h, 000h, 000h, 000h, 000h, 000h
L1_0059A:
        db   000h, 000h
L1_0059C:
        db   000h, 000h
L1_0059E:
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
L1_005A6:
        db   000h, 000h
L1_005A8:
        db   000h, 000h
L1_005AA:
        db   000h, 000h
L1_005AC:
        db   000h, 000h, 000h, 000h
L1_005B0:
        db   000h, 000h
L1_005B2:
        db   000h, 000h
L1_005B4:
        db   000h, 000h, 000h, 000h
L1_005B8:
        db   000h, 000h
L1_005BA:
        db   000h, 000h
L1_005BC:
        db   000h, 000h, 000h, 000h
L1_005C0:
        db   000h, 000h, 000h, 000h
L1_005C4:
        db   000h, 000h, 000h, 000h
L1_005C8:
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   'CALL COUNTERS(RWEC):'
L1_0065C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
L1_00660:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
L1_00664:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
L1_00668:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
L1_0066C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
S1 ends

end

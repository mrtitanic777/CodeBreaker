; v86mmgr_pcode_10.asm -- V86MMGR object 10 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_10_0000:
        push   eax
        push   esi
        movzx  eax, word ptr [edx]
        movzx  esi, ah
        push   dword ptr [esi + ebp + 24h]
        add    edx, 2
        db  0CDh, 020h, 006h, 000h, 006h, 000h
        pop    edx
        xchg   word ptr [esi + ebp + 24h], dx
        movzx  edx, dx
        shl    edx, 4
        pop    esi
        test   byte ptr [ebx + 1], 1
        jne    SHORT loc_10_005E
        test   byte ptr [ebx], 80h
        jne    SHORT loc_10_003D
        push   eax
        call   sub_10_006E
        pop    edx
        movzx  edx, dh
        mov    word ptr [edx + ebp], ax
        jmp    SHORT loc_10_006C
loc_10_003D:
        push   eax
        push   ebp
        movzx  eax, al
        add    ebp, eax
        movzx  eax, word ptr [ebp]
        add    edx, eax
        mov    dword ptr [ebp], edx
        pop    ebp
        db  0CDh, 020h, 00Ch, 000h, 006h, 000h
        pop    edx
        movzx  edx, dh
        mov    word ptr [edx + ebp], ax
        jmp    SHORT loc_10_006C
loc_10_005E:
        movzx  eax, al
        push   eax
        movzx  eax, word ptr [eax + ebp]
        add    edx, eax
        pop    eax
        mov    dword ptr [eax + ebp], edx
loc_10_006C:
        pop    eax
        ret
sub_10_006E:
        push   ecx
        push   edx
        mov    eax, edx
        mov    ecx, 0ffffh
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        movzx  eax, cx
        pop    edx
        pop    ecx
        ret
        db  000h
_PTEXT ends
        end

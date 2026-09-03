; vmm_pcode_10.asm -- VMM object 10 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_10_0000:
        db  08Bh, 01Dh, 0BCh, 0ECh, 000h, 000h
        mov    cx, word ptr [ebx]
        cmp    cl, ch
        jbe    SHORT loc_10_0019
        mov    byte ptr [ebx + 1], 0ffh
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
loc_10_0019:
        ret
loc_10_001A:
        db  08Bh, 01Dh, 0BCh, 0ECh, 000h, 000h
        mov    cx, word ptr [ebx]
        xor    ch, cl
        inc    ch
        jne    SHORT loc_10_0038
        push   eax
        db  0A1h, 098h, 0F6h, 000h, 000h
        db  03Bh, 005h, 0D4h, 0F6h, 000h, 000h
        pop    eax
        je     SHORT loc_10_0039
loc_10_0038:
        ret
loc_10_0039:
        mov    byte ptr [ebx + 1], 0
        db  0CDh, 020h, 020h, 080h, 001h, 000h
loc_10_0043:
        db  08Bh, 00Dh, 0D4h, 0F6h, 000h, 000h
        db  08Bh, 015h, 0BCh, 0ECh, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_10_0059
        xor    ecx, edi
        je     SHORT loc_10_0059
        mov    cl, 1
loc_10_0059:
        mov    byte ptr [edx + 2], cl
loc_10_005C:
        ret
        db  000h, 000h, 000h
_PTEXT ends
        end

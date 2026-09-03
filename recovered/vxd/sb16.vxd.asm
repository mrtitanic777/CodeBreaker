; VSB16 -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 15737 instructions recovered by recursive descent

Declare_Virtual_Device VSB16, 4, 30, VSB16_Control, 032A5h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x917c, 3 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xa5c8  R-X
loc_1_0000:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0014:
        inc    ebx
        dec    edi
        push   eax
        pop    ecx
        push   edx
        dec    ecx
        inc    edi
        dec    eax
        push   esp
        and    byte ptr [eax], ch
        inc    ebx
        sub    dword ptr [eax], esp
        inc    ebx
        push   edx
        inc    ebp
        inc    ecx
        push   esp
        dec    ecx
        push   esi
        inc    ebp
        and    byte ptr [ebp + eax*2 + 0x43], dl
        dec    eax
        dec    esi
        dec    edi
        dec    esp
        dec    edi
        inc    edi
        pop    ecx
        and    byte ptr [esp + edx*2 + 0x44], cl
        sub    al, 0x20
        xor    dword ptr [ecx], edi
        cmp    dword ptr [edx], esi
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
loc_1_0044:
        push   edi
        popal  
        je     0xab
        push   0x4f44534d
        push   ebx
        inc    ebp
        js     0xb6
        arpl   word ptr [eax], ax
        add    byte ptr [ebp + ecx*2 + 0x41], al
        inc    edx
        jne    0xc0
        jb     0xb1
        db  069h, 07Ah, 065h, 000h, 000h, 000h
loc_1_0064:
        inc    ecx
        push   edi
        inc    ebp
        add    byte ptr [esi + 0x4d], al
        add    byte ptr [eax], al
loc_1_006C:
        dec    ebp
        imul   esp, dword ptr [ecx + ebp*2], 0x44000000
        dec    ebp
        inc    ecx
        add    byte ptr [eax + 0x44], cl
        dec    ebp
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0080:
        imul   ebp, dword ptr [esi + 0x74], 0x726f7000
        je     loc_1_0089
loc_1_0089:
        add    byte ptr [eax], al
        add    byte ptr [ecx + ebp*2 + 0x73], ah
        popal  
        bound  ebp, qword ptr [ebp + 0x77]
        popal  
        jb     0x105
        imul   ebp, dword ptr [esi + 0x67], 0x6e730000
        bound  ebp, qword ptr fs:[ebx + esi*2 + 0x74]
        xor    byte ptr cs:[eax], dh
        xor    byte ptr [eax], al
loc_1_00A8:
        jae    0x118
        bound  ebp, qword ptr fs:[ebx + esi*2 + 0x74]
        jb     0x129
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_00A8
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x009C
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x008C
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0084
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0080
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0078
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0074
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_006C
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0068
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0064
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0054
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0044
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0014
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
loc_1_00F4:
        add    byte ptr [eax], al
loc_1_00F6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00FC:
        add    byte ptr [eax], al
loc_1_00FE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0108:
        inc    ebx
        push   esp
        xor    dword ptr [edi], esi
        xor    byte ptr [0x75746341], dh
        popal  
        insb   byte ptr es:[edi], dx
        inc    esp
        dec    ebp
        inc    ecx
        add    byte ptr [edx + esi*2 + 0x69], al
        jbe    loc_1_0182
        jb     0x192
        add    byte ptr [edx + esi*2 + 0x42], al
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x19b
        jb     loc_1_012A
loc_1_012A:
        add    byte ptr [eax], al
loc_1_012C:
        push   edx
        jae    0x195
        je     loc_1_017F
        dec    ecx
        inc    esp
        dec    ecx
        add    byte ptr [eax], al
        db  000h
loc_1_0138:
        push   ebx
        je     loc_1_019C
        je     0x1b2
        jae    loc_1_013F
loc_1_013F:
        add    byte ptr [edx + 0x65], dl
        jae    0x1a9
        je     loc_1_018A
        push   ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_014C:
        inc    edx
        dec    esp
        inc    ecx
        push   ebx
        push   esp
        inc    ebp
        push   edx
        add    byte ptr [ebx + 0x53], al
        dec    esi
        add    byte ptr [ebp + 0x69], ch
        imul   ebx, dword ptr fs:[ebx + esi*2 + 0x62], 0x6e733631
        jb     loc_1_01DE
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_016C:
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr [ecx + ebp*2 + 0x5c], 0x6d666273
        jb     loc_1_01EF
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x6e], dl
        push   eax
loc_1_017F:
        inc    ebx
        jb     0x1e7
loc_1_0182:
        popal  
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0188:
        inc    ebx
        push   esp
loc_1_018A:
        dec    esp
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        pop    esp
        sub    al, byte ptr [ebx + 0x54]
        dec    esp
        cmp    byte ptr [eax], dh
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_019C:
        inc    ecx
        and    eax, 0x20583330
        dec    ecx
        and    eax, 0x25442064
        and    byte ptr fs:[eax + 0x25], cl
        and    byte ptr fs:[esi + esi], dl
        add    byte ptr [eax], al
        db  000h
loc_1_01B4:
        inc    ecx
        and    eax, 0x20583330
        dec    ecx
        and    eax, 0x25442064
        and    byte ptr fs:[eax + 0x25], cl
        and    byte ptr fs:[eax + 0x25], dl
        xor    byte ptr [ebx], dh
        pop    eax
        and    byte ptr [esi + esi], dl
        add    byte ptr [ebp + 0x63], cl
        jne    0x22a
        jb     loc_1_024A
        imul   ebp, dword ptr [edi + 0x6e], 0x61430000
loc_1_01DE:
        jb     loc_1_0244
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01E8:
        add    dword ptr [edi + 0x100f4f2], ecx
        push   es
loc_1_01EF:
        test   dword ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        cmp    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    dword ptr [ebx - 0xe], ecx
        hlt    
        add    byte ptr [ecx], al
        add    dl, dh
        test   dword ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        cmp    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    dword ptr [ecx - 0xe], ecx
        hlt    
        add    byte ptr [ecx], al
        add    dl, dh
        test   byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], bh
        add    byte ptr [edx], al
        add    byte ptr [ecx + 0xf7f212], al
        inc    ecx
        add    dl, dh
loc_1_0244:
        test   dword ptr [eax], 0
loc_1_024A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr ss:[edx], al
        add    byte ptr [ecx], al
        push   edi
        int1   
        test   dword ptr [eax], 0xf7f20001
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], dh
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        xchg   ebx, eax
        int1   
        test   dword ptr [eax], 0xf7f20001
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], dh
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        and    byte ptr [ecx + 0xe1600f2], 0xf2
        cmc    
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], bh
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        xchg   edx, eax
        ret    0xf8
        db  001h, 000h, 0C2h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 00Ch, 05Ch, 0F6h, 0F4h, 000h, 081h, 000h, 0F3h, 0F5h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 007h, 097h, 0F3h, 0F2h, 000h, 011h, 080h, 0F2h, 0F1h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 017h
        db  021h, 054h, 0F4h, 000h, 001h, 000h, 0F4h, 0F4h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 098h, 062h, 0F3h, 0F6h, 000h
        db  081h, 000h, 0F2h, 0F6h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 018h, 023h, 0F6h, 0F6h, 000h, 001h, 000h, 0E7h, 0F7h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 015h, 091h, 0F6h, 0F6h, 000h, 001h, 000h, 0F6h, 0F6h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 045h
        db  059h, 0D3h, 0F3h, 000h, 081h, 080h, 0A3h, 0F3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 003h, 049h, 075h, 0F5h, 001h
        db  081h, 080h, 0B5h, 0F5h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 071h, 092h, 0F6h, 014h, 000h, 031h, 000h, 0F1h, 007h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 072h, 014h, 0C7h, 058h, 000h, 030h, 000h, 0C7h, 008h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 070h
        db  044h, 0AAh, 018h, 000h, 0B1h, 000h, 08Ah, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 023h, 093h, 097h, 023h, 001h
        db  0B1h, 000h, 055h, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 061h, 013h, 097h, 004h, 001h, 0B1h, 080h, 055h, 004h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 024h, 048h, 098h, 02Ah, 001h, 0B1h, 000h, 046h, 01Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 061h
        db  013h, 091h, 006h, 001h, 021h, 000h, 061h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 021h, 013h, 071h, 006h, 000h
        db  0A1h, 089h, 061h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 036h, 000h, 002h, 000h, 002h, 09Ch, 0F3h, 094h, 001h, 041h, 080h, 0F3h, 0C8h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch
        db  000h, 002h, 000h, 003h, 054h, 0F3h, 09Ah, 001h, 011h, 000h, 0F1h, 0E7h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 023h
        db  05Fh, 0F1h, 03Ah, 000h, 021h, 000h, 0F2h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 003h, 087h, 0F6h, 022h, 001h
        db  021h, 080h, 0F3h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 036h, 000h, 002h, 000h, 003h, 047h, 0F9h, 054h, 000h, 021h, 000h, 0F6h, 03Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 023h, 04Ah, 091h, 041h, 001h, 021h, 005h, 084h, 019h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 023h
        db  04Ah, 095h, 019h, 001h, 021h, 000h, 094h, 019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 009h, 0A1h, 020h, 04Fh, 000h
        db  084h, 080h, 0D1h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 038h, 000h, 002h, 000h, 021h, 01Eh, 094h, 006h, 000h, 0A2h, 000h, 0C3h, 0A6h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 031h, 012h, 0F1h, 028h, 000h, 031h, 000h, 0F1h, 018h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h
        db  08Dh, 0F1h, 0E8h, 000h, 031h, 000h, 0F1h, 078h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h, 05Bh, 051h, 028h, 000h
        db  032h, 000h, 071h, 048h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 001h, 08Bh, 0A1h, 09Ah, 000h, 021h, 040h, 0F2h, 0DFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 021h, 08Bh, 0A2h, 016h, 000h, 021h, 008h, 0A1h, 0DFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 031h
        db  08Bh, 0F4h, 0E8h, 000h, 031h, 000h, 0F1h, 078h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h, 012h, 0F1h, 028h, 000h
        db  031h, 000h, 0F1h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h, 015h, 0DDh, 013h, 001h, 021h, 000h, 056h, 026h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 031h, 016h, 0DDh, 013h, 001h, 021h, 000h, 066h, 006h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 071h
        db  049h, 0D1h, 01Ch, 001h, 031h, 000h, 061h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 021h, 04Dh, 071h, 012h, 001h
        db  023h, 080h, 072h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 032h, 000h, 002h, 000h, 0F1h, 040h, 0F1h, 021h, 001h, 0E1h, 000h, 06Fh, 016h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 002h, 01Ah, 0F5h, 075h, 001h, 001h, 080h, 085h, 035h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 002h
        db  01Dh, 0F5h, 075h, 001h, 001h, 080h, 0F3h, 0F4h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 010h, 041h, 0F5h, 005h, 001h
        db  011h, 000h, 0F2h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 032h, 000h, 002h, 000h, 021h, 09Bh, 0B1h, 025h, 001h, 0A2h, 001h, 072h, 008h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 0A1h, 098h, 07Fh, 003h, 001h, 021h, 000h, 03Fh, 007h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 0A1h
        db  093h, 0C1h, 012h, 000h, 061h, 000h, 04Fh, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 021h, 018h, 0C1h, 022h, 000h
        db  061h, 000h, 04Fh, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 031h, 05Bh, 0F4h, 015h, 000h, 072h, 083h, 08Ah, 005h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 0A1h, 090h, 074h, 039h, 000h, 061h, 000h, 071h, 067h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 071h
        db  057h, 054h, 005h, 000h, 072h, 000h, 07Ah, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 090h, 000h, 054h, 063h, 000h
        db  041h, 000h, 0A5h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 038h, 000h, 002h, 000h, 021h, 092h, 085h, 017h, 000h, 021h, 001h, 08Fh, 009h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch
        db  000h, 002h, 000h, 021h, 094h, 075h, 017h, 000h, 021h, 005h, 08Fh, 009h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 021h
        db  094h, 076h, 015h, 000h, 061h, 000h, 082h, 037h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 031h, 043h, 09Eh, 017h, 001h
        db  021h, 000h, 062h, 02Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 032h, 000h, 002h, 000h, 021h, 09Bh, 061h, 06Ah, 000h, 021h, 000h, 07Fh, 00Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 061h, 08Ah, 075h, 01Fh, 000h, 022h, 006h, 074h, 00Fh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 0A1h
        db  086h, 072h, 055h, 001h, 021h, 083h, 071h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 04Dh, 054h, 03Ch, 000h
        db  021h, 000h, 0A6h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 038h, 000h, 002h, 000h, 031h, 08Fh, 093h, 002h, 001h, 061h, 000h, 072h, 00Bh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 031h, 08Eh, 093h, 003h, 001h, 061h, 000h, 072h, 009h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 031h
        db  091h, 093h, 003h, 001h, 061h, 000h, 082h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h, 08Eh, 093h, 00Fh, 001h
        db  061h, 000h, 072h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 021h, 04Bh, 0AAh, 016h, 001h, 021h, 000h, 08Fh, 00Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 031h, 090h, 07Eh, 017h, 001h, 021h, 000h, 08Bh, 00Ch, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 031h
        db  081h, 075h, 019h, 001h, 032h, 000h, 061h, 019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 032h, 090h, 09Bh, 021h, 000h
        db  021h, 000h, 072h, 017h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 0E1h, 01Fh, 085h, 05Fh, 000h, 0E1h, 000h, 065h, 01Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 0E1h, 046h, 088h, 05Fh, 000h, 0E1h, 000h, 065h, 01Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 0A1h
        db  09Ch, 075h, 01Fh, 000h, 021h, 000h, 075h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 031h, 08Bh, 084h, 058h, 000h
        db  021h, 000h, 065h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 0E1h, 04Ch, 066h, 056h, 000h, 0A1h, 000h, 065h, 026h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 062h, 0CBh, 076h, 046h, 000h, 0A1h, 000h, 055h, 036h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 062h
        db  099h, 057h, 007h, 000h, 0A1h, 000h, 056h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Bh, 000h, 002h, 000h, 062h, 093h, 077h, 007h, 000h
        db  0A1h, 000h, 076h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Bh, 000h, 002h, 000h, 022h, 059h, 0FFh, 003h, 002h, 021h, 000h, 0FFh, 00Fh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 021h, 00Eh, 0FFh, 00Fh, 001h, 021h, 000h, 0FFh, 00Fh, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 022h
        db  046h, 086h, 055h, 000h, 021h, 080h, 064h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 045h, 066h, 012h, 000h
        db  0A1h, 000h, 096h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 08Bh, 092h, 02Ah, 001h, 022h, 000h, 091h, 02Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 0A2h, 09Eh, 0DFh, 005h, 000h, 061h, 040h, 06Fh, 007h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 020h
        db  01Ah, 0EFh, 001h, 000h, 060h, 000h, 08Fh, 006h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 08Fh, 0F1h, 029h, 000h
        db  021h, 080h, 0F4h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 077h, 0A5h, 053h, 094h, 000h, 0A1h, 000h, 0A0h, 005h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 061h, 01Fh, 0A8h, 011h, 000h, 0B1h, 080h, 025h, 003h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 061h
        db  017h, 091h, 034h, 000h, 061h, 000h, 055h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 071h, 05Dh, 054h, 001h, 000h
        db  072h, 000h, 06Ah, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 097h, 021h, 043h, 000h, 0A2h, 000h, 042h, 035h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 0A1h, 01Ch, 0A1h, 077h, 001h, 021h, 000h, 031h, 047h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h
        db  089h, 011h, 033h, 000h, 061h, 003h, 042h, 025h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 0A1h, 015h, 011h, 047h, 001h
        db  021h, 000h, 0CFh, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 03Ah, 0CEh, 0F8h, 0F6h, 000h, 051h, 000h, 086h, 002h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 021h, 015h, 021h, 023h, 001h, 021h, 000h, 041h, 013h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 006h
        db  05Bh, 074h, 095h, 000h, 001h, 000h, 0A5h, 072h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 022h, 092h, 0B1h, 081h, 000h
        db  061h, 083h, 0F2h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 041h, 04Dh, 0F1h, 051h, 001h, 042h, 000h, 0F2h, 0F5h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 061h, 094h, 011h, 051h, 001h, 0A3h, 080h, 011h, 013h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 061h
        db  08Ch, 011h, 031h, 000h, 0A1h, 080h, 01Dh, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 0A4h, 04Ch, 0F3h, 073h, 001h
        db  061h, 000h, 081h, 023h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 002h, 085h, 0D2h, 053h, 000h, 007h, 003h, 0F2h, 0F6h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 011h, 00Ch, 0A3h, 011h, 001h, 013h, 080h, 0A2h, 0E5h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 011h
        db  006h, 0F6h, 041h, 001h, 011h, 000h, 0F2h, 0E6h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 093h, 091h, 0D4h, 032h, 000h
        db  091h, 000h, 0EBh, 011h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 038h, 000h, 002h, 000h, 004h, 04Fh, 0FAh, 056h, 000h, 001h, 000h, 0C2h, 005h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch
        db  000h, 002h, 000h, 021h, 049h, 07Ch, 020h, 000h, 022h, 000h, 06Fh, 00Ch, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 031h
        db  085h, 0DDh, 033h, 001h, 021h, 000h, 056h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 020h, 004h, 0DAh, 005h, 002h
        db  021h, 081h, 08Fh, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 036h, 000h, 002h, 000h, 005h, 06Ah, 0F1h, 0E5h, 000h, 003h, 080h, 0C3h, 0E5h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h
        db  000h, 002h, 000h, 007h, 015h, 0ECh, 026h, 000h, 002h, 000h, 0F8h, 016h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 005h
        db  09Dh, 067h, 035h, 000h, 001h, 000h, 0DFh, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 018h, 096h, 0FAh, 028h, 000h
        db  012h, 000h, 0F8h, 0E5h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 010h, 086h, 0A8h, 007h, 000h, 000h, 003h, 0FAh, 003h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h
        db  000h, 002h, 000h, 011h, 041h, 0F8h, 047h, 002h, 010h, 003h, 0F3h, 003h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 001h
        db  08Eh, 0F1h, 006h, 002h, 010h, 000h, 0F3h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 00Eh, 000h, 01Fh, 000h, 000h
        db  0C0h, 000h, 01Fh, 0FFh, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 006h, 080h, 0F8h, 024h, 000h, 003h, 088h, 056h, 084h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 00Eh, 000h, 0F8h, 000h, 000h, 0D0h, 005h, 034h, 004h, 003h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 00Eh
        db  000h, 0F6h, 000h, 000h, 0C0h, 000h, 01Fh, 002h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 0D5h, 095h, 037h, 0A3h, 000h
        db  0DAh, 040h, 056h, 037h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 035h, 05Ch, 0B2h, 061h, 002h, 014h, 008h, 0F4h, 015h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah
        db  000h, 002h, 000h, 00Eh, 000h, 0F6h, 000h, 000h, 0D0h, 000h, 04Fh, 0F5h, 003h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 026h
        db  000h, 0FFh, 001h, 000h, 0E4h, 000h, 012h, 016h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h, 000h, 0F3h, 0F0h, 000h
        db  000h, 000h, 0F6h, 0C9h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 010h, 044h, 0F8h, 077h, 002h, 011h, 000h, 0F3h, 006h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 010h, 044h, 0F8h, 077h, 002h, 011h, 000h, 0F3h, 006h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 002h
        db  007h, 0F9h, 0FFh, 000h, 011h, 000h, 0F8h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 000h, 000h, 0FCh, 005h, 002h
        db  000h, 000h, 0FAh, 017h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h, 002h, 0FFh, 007h, 000h, 001h, 000h, 0FFh, 008h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 000h, 000h, 0FCh, 005h, 002h, 000h, 000h, 0FAh, 017h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h
        db  000h, 0F6h, 00Ch, 000h, 000h, 000h, 0F6h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 00Ch, 000h, 0F6h, 008h, 000h
        db  012h, 000h, 0FBh, 047h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 000h, 003h, 0F8h, 02Ah, 000h, 000h, 000h, 0F6h, 045h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h
        db  000h, 002h, 000h, 00Ch, 000h, 0F6h, 008h, 000h, 012h, 005h, 07Bh, 047h, 002h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 000h
        db  003h, 0F8h, 02Ah, 000h, 000h, 000h, 0F6h, 045h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 00Ch, 000h, 0F6h, 002h, 000h
        db  012h, 000h, 0CBh, 043h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 000h, 003h, 0F8h, 02Ah, 000h, 000h, 000h, 0F6h, 045h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h
        db  000h, 002h, 000h, 000h, 003h, 0F8h, 02Ah, 000h, 000h, 000h, 0F6h, 045h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 00Eh
        db  000h, 0F6h, 000h, 000h, 0D0h, 000h, 09Fh, 002h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h, 003h, 0F8h, 02Ah, 000h
        db  000h, 000h, 0F6h, 045h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 00Eh, 008h, 0F8h, 042h, 000h, 007h, 04Ah, 0F4h, 0E4h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 00Eh, 000h, 0F5h, 030h, 000h, 0D0h, 00Ah, 09Fh, 002h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 00Eh
        db  00Ah, 0E4h, 0E4h, 003h, 007h, 05Dh, 0F5h, 0E5h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 002h, 003h, 0B4h, 004h, 000h
        db  005h, 00Ah, 097h, 0F7h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 04Eh, 000h, 0F6h, 000h, 000h, 09Eh, 000h, 09Fh, 002h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 011h, 045h, 0F8h, 037h, 002h, 010h, 008h, 0F3h, 005h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 00Eh
        db  000h, 0F6h, 000h, 000h, 0D0h, 000h, 09Fh, 002h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 080h, 000h, 0FFh, 003h, 003h
        db  010h, 00Dh, 0FFh, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 00Eh, 008h, 0F8h, 042h, 000h, 007h, 051h, 0F4h, 0E4h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 006h, 00Bh, 0F5h, 00Ch, 000h, 002h, 000h, 0F5h, 008h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 001h
        db  000h, 0FAh, 0BFh, 000h, 002h, 000h, 0C8h, 097h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 037h, 000h, 002h, 000h, 001h, 051h, 0FAh, 087h, 000h
        db  001h, 000h, 0FAh, 0B7h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 036h, 000h, 002h, 000h, 001h, 054h, 0FAh, 08Dh, 000h, 002h, 000h, 0F8h, 0B8h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h
        db  000h, 002h, 000h, 001h, 059h, 0FAh, 088h, 000h, 002h, 000h, 0F8h, 0B6h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 001h
        db  000h, 0F9h, 00Ah, 003h, 000h, 000h, 0FAh, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h, 080h, 0F9h, 089h, 003h
        db  000h, 000h, 0F6h, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 003h, 080h, 0F8h, 088h, 003h, 00Ch, 008h, 0F6h, 0B6h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Fh
        db  000h, 002h, 000h, 003h, 085h, 0F8h, 088h, 003h, 00Ch, 000h, 0F6h, 0B6h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Fh, 000h, 002h, 000h, 00Eh
        db  040h, 076h, 04Fh, 000h, 000h, 008h, 077h, 018h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 00Eh, 040h, 0C8h, 049h, 000h
        db  003h, 000h, 09Bh, 069h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 0D7h, 0DCh, 0ADh, 005h, 003h, 0C7h, 000h, 08Dh, 005h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 0D7h, 0DCh, 0A8h, 004h, 003h, 0C7h, 000h, 088h, 004h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 080h
        db  000h, 0F6h, 006h, 003h, 011h, 000h, 067h, 017h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 080h, 000h, 0F5h, 005h, 002h
        db  011h, 009h, 046h, 016h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 006h, 03Fh, 000h, 0F4h, 000h, 015h, 000h, 0F7h, 0F5h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 031h
        db  000h, 002h, 000h, 006h, 03Fh, 000h, 0F4h, 003h, 012h, 000h, 0F7h, 0F5h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1DEA:
        add    eax, dword ptr [eax]
        add    dword ptr [eax], eax
        add    al, 0
        add    al, byte ptr [eax]
        add    eax, 0xb000800
        add    byte ptr [ecx], cl
        add    byte ptr [eax + eax], cl
        or     al, byte ptr [eax]
        or     eax, 0x13001000
        add    byte ptr [ecx], dl
        add    byte ptr [eax + eax], dl
        adc    al, byte ptr [eax]
        adc    eax, 0x3010000
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    al, byte ptr [ecx]
        add    eax, 0xb010801
        add    dword ptr [ecx], ecx
        add    dword ptr [ecx + eax], ecx
        or     al, byte ptr [ecx]
        or     eax, 0x13011001
        add    dword ptr [ecx], edx
        add    dword ptr [ecx + eax], edx
        adc    al, byte ptr [ecx]
        adc    eax, 0x230001
        and    eax, dword ptr [edx]
        xor    al, 3
        xor    byte ptr [edx + edi], al
        add    eax, 0x72f063c
        sub    eax, dword ptr [esi]
        xor    dword ptr [ecx], ecx
        sub    eax, dword ptr [esi]
        xor    ecx, dword ptr [ebx]
        sub    eax, dword ptr [esi]
        push   es
        cmp    dword ptr [esi], ecx
        dec    eax
        push   es
        cmp    al, 0x10
        dec    esp
        adc    dword ptr [edx + edx + 0x24], edx
        adc    ecx, dword ptr [esp + edx + 0x54]
        adc    eax, 0x17541653
        sbb    byte ptr [eax], dl
        dec    ebp
        sbb    dword ptr [edx + ebx], edi
        inc    ecx
        sbb    edi, dword ptr [ebx]
        sbb    al, 0x33
        sbb    eax, 0x1f471e2d
        cmp    al, 0x20
        cmp    ah, byte ptr [ecx]
        xor    eax, 0x47234022
        and    al, 0x3d
        and    eax, 0x2730263d
        xor    byte ptr [eax], ch
        inc    ebp
        sub    dword ptr [edx + ebp + 0x3f], eax
        sub    ecx, dword ptr [edx + 0x2c]
        cmp    al, 0x2d
        push   eax
        inc    eax
        das    
        inc    ebp
        xor    byte ptr [ecx + 0x31], cl
        dec    ebx
        xor    al, byte ptr [ebx + esi + 0x30]
        xor    al, 0x35
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1EA0:
        mov    bh, 0xa
        add    byte ptr [eax], al
        pop    edx
        or     eax, dword ptr [eax]
        add    byte ptr [edi], al
        or     al, 0
        add    byte ptr [esi - 0x7ffffff4], bh
        or     eax, 0xe4d0000
        add    byte ptr [eax], al
        daa    
        sldt   word ptr [eax]
        push   cs
        adc    byte ptr [eax], al
        add    byte ptr [edx], al
        adc    dword ptr [eax], eax
        add    byte ptr [0x18000012], al
        adc    eax, dword ptr [eax]
        add    byte ptr [edx], bh
        adc    al, 0
        add    byte ptr [eax], ch
        and    al, 0x20
        sbb    al, 0x17
        adc    eax, 0xe0f1113
        or     eax, 0x90a0b0c
        or     byte ptr [edi], al
        push   es
        add    eax, 0x3040405
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        db  000h
loc_1_1EF0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1EF4:
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 0x75], cl
        jae    loc_1_1F66
        arpl   word ptr [eax], sp
        push   ebx
        jns    loc_1_1F70
        je     0x1f6c
        jae    loc_1_1F70
        jp     0x1f6e
        jb     loc_1_1F0B
loc_1_1F0B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1F66:
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
loc_1_1F70:
        add    eax, 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     eax, 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        sldt   word ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    eax, 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        push   ss
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        pop    ss
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        inc    esi
        db  0D9h, 0D4h, 047h, 0E8h, 062h, 0CFh, 011h, 093h, 0BCh, 044h, 045h, 053h, 054h, 000h, 000h, 083h
        db  0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 085h
        db  0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 084h
        db  0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 086h
        db  0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 0C0h
        db  033h, 042h, 08Ch, 0CCh, 0B4h, 0CEh, 011h, 092h, 094h, 044h, 045h, 053h, 054h, 000h, 000h
loc_1_2120:
        sal    dword ptr [ebx], 0x42
        mov    esp, cs
        mov    ah, 0xce
        adc    dword ptr [edx + 0x53454494], edx
        push   esp
        add    byte ptr [eax], al
sub_1_2130:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    loc_1_2140
        mov    eax, 1
        jmp    loc_1_214F
loc_1_2140:
        push   dword ptr [eax + 8]
        push   dword ptr [esp + 0xc]
        call   sub_1_936C
        add    esp, 8
loc_1_214F:
        ret    
sub_1_2150:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    loc_1_2160
        mov    eax, 1
        jmp    loc_1_216B
loc_1_2160:
        push   dword ptr [eax + 8]
        call   sub_1_931C
        add    esp, 4
loc_1_216B:
        ret    
sub_1_216C:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    word ptr [eax + 6], cx
        mov    dword ptr [eax], ecx
        mov    word ptr [eax + 4], 0x6a0
        mov    dword ptr [eax + 8], ecx
        xor    eax, eax
        ret    
sub_1_2184:
        push   ebp
        movzx  ax, byte ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x20
        mov    word ptr [ebp - 0x20], ax
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [esi + 6], ax
        mov    dword ptr [ebp - 0x1c], ecx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x18], ecx
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], edx
        lea    eax, [esi + 8]
        mov    dword ptr [ebp - 0xc], ecx
        mov    ecx, dword ptr [ebp + 0x1c]
        push   eax
        lea    edx, [ebp - 0x20]
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 8], 0x1f4
        mov    word ptr [ebp - 0x1e], 4
        push   edx
        call   sub_1_95AD
        add    esp, 8
        test   eax, eax
        je     loc_1_21E4
        mov    eax, 1
        jmp    loc_1_21FB
loc_1_21E4:
        push   esi
        or     byte ptr [esi], 1
        call   sub_1_2150
        add    esp, 4
        push   esi
        call   sub_1_221C
        add    esp, 4
        xor    eax, eax
loc_1_21FB:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2200:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    loc_1_2210
        mov    eax, 1
        jmp    loc_1_221B
loc_1_2210:
        push   dword ptr [eax + 8]
        call   sub_1_939E
        add    esp, 4
loc_1_221B:
        ret    
sub_1_221C:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    loc_1_222C
        mov    eax, 1
        jmp    loc_1_2237
loc_1_222C:
        push   dword ptr [eax + 8]
        call   sub_1_93B2
        add    esp, 4
loc_1_2237:
        ret    
sub_1_2238:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_2200
        add    esp, 4
        push   esi
        call   sub_1_2150
        add    esp, 4
        push   dword ptr [esi + 8]
        call   sub_1_95D4
        add    esp, 4
        xor    eax, eax
        and    byte ptr [esi], 0xfe
        pop    esi
        ret    
sub_1_2261:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    word ptr [eax + 6], cx
        and    byte ptr [eax], 0xfe
        mov    word ptr [eax + 4], 0xeb
        mov    dword ptr [eax + 8], ecx
        xor    eax, eax
        ret    
sub_1_227A:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 0xc]
        shl    edx, cl
        movzx  eax, word ptr [esi + 4]
        test   edx, eax
        je     loc_1_22B6
        movzx  cx, cl
        mov    word ptr [esi + 6], cx
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_1_9681
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_22B6
        xor    eax, eax
        or     byte ptr [esi], 1
        jmp    loc_1_22BB
loc_1_22B6:
        mov    eax, 1
loc_1_22BB:
        pop    esi
        pop    ebp
        ret    
sub_1_22BE:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    loc_1_22CE
        mov    eax, 3
        jmp    loc_1_22DB
loc_1_22CE:
        mov    eax, dword ptr [eax + 8]
        sub    eax, dword ptr [esp + 8]
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_22DB:
        ret    
sub_1_22DC:
        push   ebp
        mov    eax, 3
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 1
        je     loc_1_2316
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x14]
        push   -1
        push   dword ptr [esi + 8]
        call   sub_1_96BF
        add    esp, 0x14
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi + 8]
        call   sub_1_96EE
        add    esp, 0xc
        xor    eax, eax
loc_1_2316:
        pop    esi
        pop    ebp
        ret    
sub_1_2319:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    loc_1_2329
        mov    eax, 3
        jmp    loc_1_233C
loc_1_2329:
        xor    ecx, ecx
        push   ecx
        push   ecx
        push   ecx
        push   ecx
        push   dword ptr [eax + 8]
        call   sub_1_96BF
        add    esp, 0x14
        xor    eax, eax
loc_1_233C:
        ret    
sub_1_233D:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    loc_1_234D
        mov    eax, 3
        jmp    loc_1_235C
loc_1_234D:
        push   dword ptr [esp + 8]
        push   dword ptr [eax + 8]
        call   sub_1_9757
        add    esp, 8
loc_1_235C:
        ret    
sub_1_235D:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    loc_1_236D
        mov    eax, 3
        jmp    loc_1_237C
loc_1_236D:
        push   dword ptr [esp + 8]
        push   dword ptr [eax + 8]
        call   sub_1_9725
        add    esp, 8
loc_1_237C:
        ret    
sub_1_237D:
        push   esi
        mov    eax, 3
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi], 1
        je     loc_1_23AD
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [esi + 8]
        call   sub_1_96BF
        add    esp, 0x14
        push   dword ptr [esi + 8]
        call   sub_1_96AB
        add    esp, 4
        xor    eax, eax
        and    byte ptr [esi], 0xfe
loc_1_23AD:
        pop    esi
        ret    
sub_1_23AF:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [edx], 0
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 0
        je     loc_1_23CF
loc_1_23C4:
        inc    word ptr [edx]
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 0
        jne    loc_1_23C4
loc_1_23CF:
        xor    eax, eax
        ret    
sub_1_23D2:
        xor    dx, dx
        push   ebx
        mov    ecx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        cmp    byte ptr [ecx], dl
        je     loc_1_23F7
loc_1_23E3:
        movzx  eax, dx
        inc    dx
        mov    bl, byte ptr [eax + ecx]
        mov    byte ptr [eax + esi], bl
        movzx  eax, dx
        cmp    byte ptr [eax + ecx], 0
        jne    loc_1_23E3
loc_1_23F7:
        movzx  eax, dx
        mov    byte ptr [eax + esi], 0
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    
sub_1_2403:
        push   ebp
        lea    ecx, [esp + 0x10]
        mov    ebp, esp
        sub    esp, 0x1c
        mov    word ptr [ebp - 2], 0
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        cmp    byte ptr [eax], 0
        mov    word ptr [ebp - 4], 0
        je     loc_1_25B3
loc_1_242B:
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ebp - 4]
        mov    dl, byte ptr [eax + edx]
        cmp    dl, 0x25
        jne    loc_1_2593
        inc    word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ebp - 4]
        mov    dl, byte ptr [eax + edx]
        movzx  eax, dl
        cmp    eax, 0x63
        je     loc_1_2483
        cmp    eax, 0x64
        je     loc_1_2498
        cmp    eax, 0x73
        je     loc_1_24FA
        cmp    eax, 0x78
        je     loc_1_2528
        movzx  eax, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        mov    byte ptr [eax + ebx], 0x25
        movzx  esi, word ptr [ebp - 2]
        mov    byte ptr [esi + ebx], dl
        jmp    loc_1_259A
loc_1_2483:
        movzx  eax, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        mov    dl, byte ptr [ecx]
        add    ecx, 4
        mov    byte ptr [eax + ebx], dl
        jmp    loc_1_259E
loc_1_2498:
        mov    si, word ptr [ecx]
        mov    di, 1
        cmp    si, 0xa
        jbe    loc_1_24B8
loc_1_24A5:
        mov    ax, si
        imul   di, di, 0xa
        sub    dx, dx
        div    di
        cmp    ax, 0xa
        ja     loc_1_24A5
loc_1_24B8:
        mov    word ptr [ebp - 8], 0xa
        mov    ax, si
        sub    dx, dx
        div    di
        movzx  edx, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        add    al, 0x30
        mov    byte ptr [edx + ebx], al
        mov    ax, si
        sub    dx, dx
        div    di
        mov    si, dx
        mov    ax, di
        sub    dx, dx
        div    word ptr [ebp - 8]
        mov    di, ax
        test   di, di
        jne    loc_1_24B8
        add    ecx, 4
        jmp    loc_1_259E
loc_1_24FA:
        xor    dx, dx
        mov    eax, dword ptr [ecx]
        cmp    byte ptr [eax], dl
        je     loc_1_2523
loc_1_2503:
        movzx  esi, dx
        inc    dx
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [esi + eax]
        movzx  edi, dx
        movzx  esi, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        mov    byte ptr [esi + ebx], al
        mov    eax, dword ptr [ecx]
        cmp    byte ptr [edi + eax], 0
        jne    loc_1_2503
loc_1_2523:
        add    ecx, 4
        jmp    loc_1_259E
loc_1_2528:
        mov    ax, word ptr [ecx]
        mov    esi, 0                        ; -> loc_1_0000
        mov    word ptr [ebp - 6], ax
        lea    edi, [ebp - 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        cmp    word ptr [ebp - 6], 0x10
        mov    si, 1
        jbe    loc_1_255B
loc_1_2547:
        shl    si, 4
        mov    ax, word ptr [ebp - 6]
        sub    dx, dx
        div    si
        cmp    ax, 0x10
        ja     loc_1_2547
loc_1_255B:
        mov    ax, word ptr [ebp - 6]
        sub    dx, dx
        div    si
        movzx  eax, ax
        movzx  edx, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        mov    al, byte ptr [ebp + eax - 0x1c]
        mov    byte ptr [edx + ebx], al
        mov    ax, word ptr [ebp - 6]
        sub    dx, dx
        div    si
        shr    si, 4
        mov    word ptr [ebp - 6], dx
        test   si, si
        jne    loc_1_255B
        add    ecx, 4
        jmp    loc_1_259E
loc_1_2593:
        movzx  eax, word ptr [ebp - 2]
        mov    byte ptr [eax + ebx], dl
loc_1_259A:
        inc    word ptr [ebp - 2]
loc_1_259E:
        inc    word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ebp - 4]
        cmp    byte ptr [eax + edx], 0
        jne    loc_1_242B
loc_1_25B3:
        movzx  eax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        mov    byte ptr [eax + ebx], 0
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_25C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        call   sub_1_301E
        test   eax, eax
        je     loc_1_25EE
        mov    esi, dword ptr [ebp + 0x14]
loc_1_25D7:
        cmp    dword ptr [eax + 8], esi
        jne    loc_1_25E1
        test   byte ptr [eax], 1
        jne    loc_1_25F8
loc_1_25E1:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    loc_1_25D7
loc_1_25EE:
        mov    eax, 1
loc_1_25F3:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_25F8:
        movzx  ecx, word ptr [ebp + 0xc]
        cmp    ecx, 1
        je     loc_1_263B
        cmp    ecx, 2
        je     loc_1_266D
        cmp    ecx, 4
        je     loc_1_26BA
        cmp    ecx, 8
        je     loc_1_2704
        cmp    ecx, 0x10
        je     loc_1_273B
        cmp    ecx, 0x80
        je     loc_1_2775
        cmp    ecx, 0x100
        je     loc_1_27AC
        jmp    loc_1_25EE
loc_1_263B:
        cmp    word ptr [ebp + 0x18], 2
        jb     loc_1_2649
        mov    eax, 2
        jmp    loc_1_25F3
loc_1_2649:
        push   dword ptr [ebp + 0x1c]
        mov    ecx, 0xec
        movzx  edx, word ptr [ebp + 0x18]
        imul   edx, ecx
        add    edx, eax
        add    edx, ecx
        push   edx
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_5FAC
        add    esp, 0x10
        jmp    loc_1_25F3
loc_1_266D:
        cmp    word ptr [ebp + 0x18], 1
        jb     loc_1_267E
        mov    eax, 2
        jmp    loc_1_25F3
loc_1_267E:
        test   byte ptr [eax + 0xd4], 2
        jne    loc_1_2691
        mov    eax, 2
        jmp    loc_1_25F3
loc_1_2691:
        push   dword ptr [ebp + 0x1c]
        movzx  ecx, word ptr [ebp + 0x18]
        imul   ecx, ecx, 0xe0
        add    ecx, eax
        add    ecx, 0x2ee
        push   ecx
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_4_1613
loc_1_26B2:
        add    esp, 0x10
        jmp    loc_1_25F3
loc_1_26BA:
        cmp    word ptr [ebp + 0x18], 1
        jb     loc_1_26CB
        mov    eax, 2
        jmp    loc_1_25F3
loc_1_26CB:
        test   byte ptr [eax + 0xd4], 2
        jne    loc_1_26DE
        mov    eax, 2
        jmp    loc_1_25F3
loc_1_26DE:
        push   dword ptr [ebp + 0x1c]
        movzx  ecx, word ptr [ebp + 0x18]
        imul   ecx, ecx, 0x5a
        add    ecx, eax
        add    ecx, 0x3ce
        push   ecx
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_4_13B4
loc_1_26FC:
        add    esp, 0x10
        jmp    loc_1_25F3
loc_1_2704:
        cmp    word ptr [ebp + 0x18], 1
        jb     loc_1_2715
        mov    eax, 2
        jmp    loc_1_25F3
loc_1_2715:
        push   dword ptr [ebp + 0x1c]
        movzx  ecx, word ptr [ebp + 0x18]
        imul   ecx, ecx, 0x2a
        add    ecx, eax
        add    ecx, 0x2c4
        push   ecx
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_4_1826
loc_1_2733:
        add    esp, 0x10
        jmp    loc_1_25F3
loc_1_273B:
        cmp    word ptr [ebp + 0x18], 1
        jb     loc_1_274C
        mov    eax, 2
        jmp    loc_1_25F3
loc_1_274C:
        push   dword ptr [ebp + 0x1c]
        movzx  ecx, word ptr [ebp + 0x18]
        imul   ecx, ecx, 0x229
        add    ecx, eax
        add    ecx, 0x428
        push   ecx
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_8656
        add    esp, 0x10
        jmp    loc_1_25F3
loc_1_2775:
        cmp    word ptr [ebp + 0x18], 1
        jb     loc_1_2786
        mov    eax, 2
        jmp    loc_1_25F3
loc_1_2786:
        push   dword ptr [ebp + 0x1c]
        movzx  ecx, word ptr [ebp + 0x18]
        imul   ecx, ecx, 0x2c
        add    ecx, eax
        add    ecx, 0x651
        push   ecx
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_87C6
        add    esp, 0x10
        jmp    loc_1_25F3
loc_1_27AC:
        movzx  ecx, word ptr [ebp + 0x10]
        cmp    ecx, 1
        je     loc_1_27D6
        cmp    ecx, 2
        je     loc_1_27F6
        cmp    ecx, 3
        je     loc_1_2829
        cmp    ecx, 4
        je     loc_1_2858
        cmp    ecx, 5
        je     loc_1_2866
        jmp    loc_1_25EE
loc_1_27D6:
        mov    ax, word ptr [eax + 0x681]
        mov    esi, dword ptr [ebp + 0x1c]
        push   eax
        call   sub_1_3B3A
        mov    word ptr [esi], ax
        add    esp, 4
        and    word ptr [esi], 0xff
        jmp    loc_1_2887
loc_1_27F6:
        cmp    dword ptr [eax + 0xe4], 0
        je     loc_1_2887
        lea    esi, [eax + 0x67d]
        lea    eax, [ebp - 1]
        push   eax
        push   0x7c
        push   esi
        call   sub_1_40A6
        add    esp, 0xc
        or     byte ptr [ebp - 1], 0x80
        push   dword ptr [ebp - 1]
        push   0x7c
        push   esi
        call   sub_1_4076
        jmp    loc_1_2884
loc_1_2829:
        cmp    dword ptr [eax + 0xe4], 0
        je     loc_1_2887
        lea    esi, [eax + 0x67d]
        lea    eax, [ebp - 1]
        push   eax
        push   0x7c
        push   esi
        call   sub_1_40A6
        add    esp, 0xc
        and    byte ptr [ebp - 1], 0x7f
        push   dword ptr [ebp - 1]
        push   0x7c
        push   esi
        call   sub_1_4076
        jmp    loc_1_2884
loc_1_2858:
        mov    eax, dword ptr [eax + 0xe8]
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    word ptr [ecx], ax
        jmp    loc_1_2887
loc_1_2866:
        mov    ecx, dword ptr [ebp + 0x1c]
        cmp    dword ptr [eax + 0xe8], 0
        mov    cl, byte ptr [ecx]
        je     loc_1_2887
        mov    ax, word ptr [eax + 0x681]
        push   0
        push   eax
        push   ecx
        call   sub_1_3E36
loc_1_2884:
        add    esp, 0xc
loc_1_2887:
        xor    eax, eax
        jmp    loc_1_25F3
sub_1_288E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        test   byte ptr [esi], 4
        je     loc_1_28B9
        push   dword ptr [esi + 0xd8]
        add    esi, 0x6a7
        push   esi
        call   sub_1_2130
        add    esp, 8
        jmp    loc_1_2A9B
loc_1_28B9:
        lea    edi, [esi + 0x67d]
        lea    eax, [ebp - 1]
        push   eax
        push   0x82
        push   edi
        call   sub_1_40A6
        add    esp, 0xc
        test   byte ptr [ebp - 1], 7
        je     loc_1_2A64
        test   byte ptr [ebp - 1], 1
        je     loc_1_2982
        lea    ebx, [esi + 0xec]
        push   8
        push   ebx
        call   sub_1_9DEB
        add    esp, 8
        test   eax, eax
        push   ebx
        jne    loc_1_296B
        call   sub_1_9E43
        add    esp, 4
        cmp    dword ptr [esi + 0xf4], 0
        je     loc_1_2963
        cmp    dword ptr [esi + 0x1e0], 0
        je     loc_1_2963
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 5], 1
loc_1_291D:
        cmp    byte ptr [ebp - 5], 6
        jae    loc_1_294B
        push   ebx
        call   sub_1_A0F0
        add    esp, 4
        lea    eax, [ebp - 3]
        push   eax
        push   0x82
        push   edi
        call   sub_1_40A6
        add    esp, 0xc
        inc    byte ptr [ebp - 5]
        and    byte ptr [ebp - 3], 1
        cmp    byte ptr [ebp - 3], 0
        jne    loc_1_291D
loc_1_294B:
        mov    ax, word ptr [esi + 0x79f]
        cmp    ah, 4
        jne    loc_1_2982
        cmp    al, 0xf
        jae    loc_1_2982
        push   esi
        call   sub_1_2AA4
        jmp    loc_1_297F
loc_1_2963:
        push   ebx
        call   sub_1_A0F0
        jmp    loc_1_297F
loc_1_296B:
        call   sub_1_A0F0
        add    esp, 4
        lea    eax, [esi + 0x6a7]
        push   eax
        call   sub_1_2150
loc_1_297F:
        add    esp, 4
loc_1_2982:
        test   byte ptr [ebp - 1], 2
        je     loc_1_2A38
        test   byte ptr [esi + 0xd4], 0x20
        lea    ebx, [esi + 0xec]
        je     loc_1_29A1
        lea    ebx, [esi + 0x1d8]
loc_1_29A1:
        push   0x10
        push   ebx
        call   sub_1_9DEB
        add    esp, 8
        test   eax, eax
        push   ebx
        jne    loc_1_2A21
        call   sub_1_9E43
        add    esp, 4
        cmp    dword ptr [esi + 0xf4], 0
        je     loc_1_2A19
        cmp    dword ptr [esi + 0x1e0], 0
        je     loc_1_2A19
        mov    byte ptr [ebp - 2], 1
        mov    byte ptr [ebp - 5], 1
loc_1_29D3:
        cmp    byte ptr [ebp - 5], 6
        jae    loc_1_2A01
        push   ebx
        call   sub_1_A0F0
        add    esp, 4
        lea    eax, [ebp - 2]
        push   eax
        push   0x82
        push   edi
        call   sub_1_40A6
        add    esp, 0xc
        inc    byte ptr [ebp - 5]
        and    byte ptr [ebp - 2], 2
        cmp    byte ptr [ebp - 2], 0
        jne    loc_1_29D3
loc_1_2A01:
        mov    ax, word ptr [esi + 0x79f]
        cmp    ah, 4
        jne    loc_1_2A38
        cmp    al, 0xf
        jae    loc_1_2A38
        push   esi
        call   sub_1_2AA4
        jmp    loc_1_2A35
loc_1_2A19:
        push   ebx
        call   sub_1_A0F0
        jmp    loc_1_2A35
loc_1_2A21:
        call   sub_1_A0F0
        add    esp, 4
        lea    eax, [esi + 0x6a7]
        push   eax
        call   sub_1_2150
loc_1_2A35:
        add    esp, 4
loc_1_2A38:
        test   byte ptr [ebp - 1], 4
        je     loc_1_2A9B
        lea    ebx, [esi + 0x2ee]
        push   ebx
        call   sub_1_A447
        add    esp, 4
        test   eax, eax
        jne    loc_1_2A59
        push   ebx
        call   sub_1_A309
        jmp    loc_1_2A5F
loc_1_2A59:
        push   edi
        call   sub_1_41D9
loc_1_2A5F:
        add    esp, 4
        jmp    loc_1_2A8C
loc_1_2A64:
        test   byte ptr [ebp - 1], 8
        je     loc_1_2A8C
        lea    eax, [ebp - 6]
        push   eax
        push   edi
        call   sub_1_4807
        mov    ax, word ptr [ebp - 6]
        add    esp, 8
        add    ax, 0xd
        lea    ecx, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_9CE4
        add    esp, 8
loc_1_2A8C:
        add    esi, 0x6a7
        push   esi
        call   sub_1_2150
        add    esp, 4
loc_1_2A9B:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2AA4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
        push   dword ptr [ebx + 0x1a0]
        push   dword ptr [ebx + 0x1c8]
        lea    edi, [ebx + 0xec]
        lea    esi, [ebx + 0x1d8]
        add    ebx, 0x67d
        call   sub_1_233D
        add    esp, 8
        push   dword ptr [esi + 0xb4]
        push   dword ptr [esi + 0xdc]
        call   sub_1_233D
        add    esp, 8
        lea    ecx, [ebp - 2]
        push   ecx
        push   0x82
        push   ebx
        call   sub_1_40A6
        add    esp, 0xc
        and    byte ptr [ebp - 2], 3
        jne    loc_1_2C03
        push   0xf
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   6
        push   ebx
        call   sub_1_433E
        add    esp, 8
        lea    eax, [ebp - 1]
        push   eax
        push   ebx
        call   sub_1_43BA
        add    esp, 8
        test   byte ptr [ebp - 1], 1
        je     loc_1_2B9A
        push   0xe
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   8
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0x20
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0xe
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   8
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   4
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0xe
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   8
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0
        push   ebx
        call   sub_1_433E
        add    esp, 8
loc_1_2B9A:
        test   byte ptr [ebp - 1], 2
        je     loc_1_2C03
        push   0xe
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0x10
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0x20
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0xe
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0x10
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   4
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0xe
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0x10
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0
        push   ebx
        call   sub_1_433E
        add    esp, 8
loc_1_2C03:
        push   dword ptr [edi + 0xb4]
        push   dword ptr [edi + 0xdc]
        call   sub_1_235D
        add    esp, 8
        push   dword ptr [esi + 0xb4]
        push   dword ptr [esi + 0xdc]
        call   sub_1_235D
        add    esp, 8
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2C34:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        push   2
        call   sub_1_2E0C
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_2C87
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 4
        je     loc_1_2C82
        mov    ecx, eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0xd8], eax
        jne    loc_1_2C87
        cmp    dword ptr [ebp + 0x10], 0
        push   esi
        je     loc_1_2C78
        call   sub_1_939E
        jmp    loc_1_2C7D
loc_1_2C78:
        call   sub_1_93B2
loc_1_2C7D:
        add    esp, 4
        jmp    loc_1_2C87
loc_1_2C82:
        call   sub_1_A58C
loc_1_2C87:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2C8E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        push   5
        call   sub_1_2E0C
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_2DAC
        xor    di, di
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, 0xec
        and    byte ptr [eax], 0xfb
loc_1_2CC3:
        movzx  eax, di
        imul   eax, ebx
        inc    di
        push   esi
        add    eax, dword ptr [ebp - 8]
        add    eax, ebx
        push   eax
        call   sub_1_A09E
        add    esp, 8
        cmp    di, 2
        jb     loc_1_2CC3
        xor    di, di
loc_1_2CE3:
        movzx  eax, di
        inc    di
        push   esi
        imul   eax, eax, 0xe0
        add    eax, dword ptr [ebp - 8]
        add    eax, 0x2ee
        push   eax
        call   sub_4_15DE
loc_1_2CFD:
        add    esp, 8
        cmp    di, 1
        jb     loc_1_2CE3
        xor    di, di
loc_1_2D09:
        movzx  eax, di
        inc    di
        push   esi
        imul   eax, eax, 0x5a
        add    eax, dword ptr [ebp - 8]
        add    eax, 0x3ce
        push   eax
        call   sub_4_137F
loc_1_2D20:
        add    esp, 8
        cmp    di, 1
        jb     loc_1_2D09
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   esi
        add    eax, 0x67d
        push   eax
        call   sub_1_4614
        add    esp, 0xc
        cmp    dword ptr [0], 0              ; -> sub_1_9178
        je     loc_1_2D67
        mov    cx, word ptr [0]              ; -> obj1:0x916C
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax + 0x683], cx
        jne    loc_1_2D67
        push   0
        push   esi
        push   7
        call   sub_1_9178
        add    esp, 0xc
loc_1_2D67:
        mov    eax, dword ptr [ebp - 8]
        mov    edi, 0x6a7
        add    eax, edi
        push   eax
        call   sub_1_2200
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        push   eax
        call   sub_1_2150
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        push   eax
        call   sub_1_221C
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x67d
        push   0xffff
        push   eax
        call   sub_1_4288
        add    esp, 8
loc_1_2DAC:
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   esi
        push   6
        call   sub_1_2E0C
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_2E03
        xor    di, di
        mov    eax, dword ptr [ebp - 8]
        and    byte ptr [eax], 0xf7
loc_1_2DCC:
        movzx  eax, di
        inc    di
        push   esi
        imul   eax, eax, 0x229
        add    eax, dword ptr [ebp - 8]
        add    eax, 0x428
        push   eax
        call   sub_1_6531
        add    esp, 8
        cmp    di, 1
        jb     loc_1_2DCC
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   esi
        add    eax, 0x67d
        push   eax
        call   sub_1_46AB
        add    esp, 0xc
loc_1_2E03:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2E0C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    eax, 1
        push   esi
        je     loc_1_2E4A
        cmp    eax, 2
        je     loc_1_2E92
        cmp    eax, 3
        je     loc_1_2ECE
        cmp    eax, 4
        je     loc_1_2F4A
        cmp    eax, 5
        je     loc_1_2F8A
        cmp    eax, 6
        je     loc_1_2FD4
loc_1_2E42:
        mov    eax, 1
loc_1_2E47:
        pop    esi
        pop    ebp
        ret    
loc_1_2E4A:
        call   sub_1_301E
        mov    esi, eax
        test   esi, esi
        je     loc_1_2E42
loc_1_2E55:
        test   byte ptr [esi], 1
        je     loc_1_2E70
        push   dword ptr [ebp + 0xc]
        lea    ecx, [esi + 0x67d]
        push   ecx
        call   sub_1_41F5
        add    esp, 8
        test   eax, eax
        je     loc_1_2E81
loc_1_2E70:
        push   esi
        call   sub_1_303E
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    loc_1_2E55
        jmp    loc_1_2E42
loc_1_2E81:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], esi
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    loc_1_2E47
loc_1_2E92:
        call   sub_1_301E
        test   eax, eax
        je     loc_1_2E42
loc_1_2E9B:
        test   byte ptr [eax], 1
        je     loc_1_2EAB
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x6af], ecx
        je     loc_1_2EBA
loc_1_2EAB:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    loc_1_2E9B
        jmp    loc_1_2E42
loc_1_2EBA:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    loc_1_2E47
loc_1_2ECE:
        call   sub_1_301E
        mov    esi, eax
        test   esi, esi
        je     loc_1_2E42
loc_1_2EDD:
        test   byte ptr [esi], 1
        je     loc_1_2F0E
        push   dword ptr [ebp + 0xc]
        lea    ecx, [esi + 0x6b3]
        push   ecx
        call   sub_1_22BE
        add    esp, 8
        test   eax, eax
        je     loc_1_2F22
        push   dword ptr [ebp + 0xc]
        lea    ecx, [esi + 0x6bf]
        push   ecx
        call   sub_1_22BE
        add    esp, 8
        test   eax, eax
        je     loc_1_2F36
loc_1_2F0E:
        push   esi
        call   sub_1_303E
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    loc_1_2EDD
        jmp    loc_1_2E42
loc_1_2F22:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], esi
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    loc_1_2E47
loc_1_2F36:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], esi
        mov    word ptr [eax], 1
        xor    eax, eax
        jmp    loc_1_2E47
loc_1_2F4A:
        call   sub_1_301E
        test   eax, eax
        je     loc_1_2E42
loc_1_2F57:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_2F64
        test   byte ptr [eax], 1
        jne    loc_1_2F76
loc_1_2F64:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    loc_1_2F57
        jmp    loc_1_2E42
loc_1_2F76:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    loc_1_2E47
loc_1_2F8A:
        call   sub_1_301E
        test   eax, eax
        je     loc_1_2E42
loc_1_2F97:
        mov    ecx, dword ptr [eax]
        test   cl, 1
        je     loc_1_2FAE
        test   cl, 4
        je     loc_1_2FAE
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0xd8], ecx
        je     loc_1_2FC0
loc_1_2FAE:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    loc_1_2F97
        jmp    loc_1_2E42
loc_1_2FC0:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    loc_1_2E47
loc_1_2FD4:
        call   sub_1_301E
        test   eax, eax
        je     loc_1_2E42
loc_1_2FE1:
        mov    ecx, dword ptr [eax]
        test   cl, 1
        je     loc_1_2FF8
        test   cl, 8
        je     loc_1_2FF8
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0xdc], ecx
        je     loc_1_300A
loc_1_2FF8:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    loc_1_2FE1
        jmp    loc_1_2E42
loc_1_300A:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    loc_1_2E47
sub_1_301E:
        push   esi
        xor    esi, esi
        cmp    dword ptr [0xe], esi          ; -> loc_1_00FE
        je     loc_1_303A
        mov    eax, dword ptr [0xe]          ; -> loc_1_00FE
        push   eax
        call   sub_1_A598
        test   eax, eax
        je     loc_1_303A
        mov    esi, dword ptr [eax]
loc_1_303A:
        mov    eax, esi
        pop    esi
        ret    
sub_1_303E:
        mov    eax, dword ptr [esp + 4]
        push   esi
        xor    esi, esi
        push   dword ptr [eax + 4]
        push   dword ptr [0xe]               ; -> loc_1_00FE
        call   sub_1_A5A8
        test   eax, eax
        je     loc_1_3059
        mov    esi, dword ptr [eax]
loc_1_3059:
        mov    eax, esi
        pop    esi
        ret    
sub_1_305D:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x2c]
        add    eax, 0x2c4
        ret    
sub_1_306A:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x24]
        ret    
sub_1_3072:
        push   0                             ; -> obj1:0x00F0
        call   sub_1_9614
        add    esp, 4
        push   0                             ; -> loc_1_9990
        call   sub_1_98D2
        add    esp, 4
        xor    eax, eax
        ret    
sub_1_308F:
        cmp    word ptr [4], 0               ; -> loc_1_00F4
        jne    loc_1_30CA
        cmp    word ptr [6], 0               ; -> loc_1_00F6
        jne    loc_1_30CA
        inc    word ptr [6]                  ; -> loc_1_00F6
        call   sub_1_3072
        test   eax, eax
        jne    loc_1_30CA
        push   0xe                           ; -> loc_1_00FE
        push   4
        call   sub_1_9828
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    loc_1_30CF
loc_1_30CA:
        mov    eax, 1
loc_1_30CF:
        ret    
sub_1_30D0:
        xor    ecx, ecx
        mov    eax, dword ptr [0]            ; -> sub_1_9178
        mov    word ptr [0], cx              ; -> obj1:0x916C
        cmp    eax, ecx
        je     loc_1_30EB
        push   ecx
        push   ecx
        push   1
        call   eax
        add    esp, 0xc
loc_1_30EB:
        push   0                             ; -> loc_1_9990
        call   sub_1_98F5
        add    esp, 4
        mov    eax, dword ptr [0xe]          ; -> loc_1_00FE
        push   eax
        call   sub_1_9854
        add    esp, 4
        xor    eax, eax
        ret    
sub_1_3109:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        cmp    dword ptr [0], 0              ; -> sub_1_9178
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        je     loc_1_3158
        mov    ax, word ptr [ebp + 0xc]
        cmp    word ptr [0], ax              ; -> obj1:0x916C
        je     loc_1_313F
        movzx  eax, word ptr [0]             ; -> obj1:0x916C
        movzx  ecx, word ptr [ebp + 0xc]
        sub    eax, ecx
        cmp    eax, -1
        jne    loc_1_3158
loc_1_313F:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_92B5
        add    esp, 0x10
        jmp    loc_1_3384
loc_1_3158:
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 8]
        movzx  edi, word ptr [ebp + 0xc]
        push   eax
        push   ecx
        push   edi
        push   1
        call   sub_1_2E0C
        add    esp, 0x10
        test   eax, eax
        je     loc_1_317D
        mov    eax, 1
        jmp    loc_1_3386
loc_1_317D:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 4
        je     loc_1_320F
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xd8], ecx
        jne    loc_1_3384
        cmp    dword ptr [0], 0              ; -> sub_1_9178
        je     loc_1_31F5
        mov    ax, word ptr [ebp + 0xc]
        cmp    word ptr [0], ax              ; -> obj1:0x9170
        je     loc_1_31BC
        movzx  eax, word ptr [0]             ; -> obj1:0x9170
        sub    eax, edi
        cmp    eax, -1
        jne    loc_1_31F5
loc_1_31BC:
        movzx  edx, word ptr [0]             ; -> obj1:0x9170
        inc    edx
        mov    al, 0x3f
        out    dx, al
        mov    ax, word ptr [0]              ; -> obj1:0x9170
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_95FE
        mov    ax, word ptr [0]              ; -> obj1:0x9170
        add    esp, 8
        inc    ax
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_95FE
        mov    dx, word ptr [0]              ; -> obj1:0x9170
        add    esp, 8
        in     al, dx
loc_1_31F5:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_3373
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_9CE4
        jmp    loc_1_3381
loc_1_320F:
        call   sub_1_A58C
        cmp    eax, dword ptr [ebp + 8]
        jne    loc_1_3233
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_3373
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_9CE4
        jmp    loc_1_3381
loc_1_3233:
        xor    di, di
        mov    ebx, 0xec
loc_1_323B:
        push   dword ptr [ebp + 8]
        movzx  eax, di
        imul   eax, ebx
        add    eax, dword ptr [ebp - 8]
        add    eax, ebx
        push   eax
        call   sub_1_A04E
        add    esp, 8
        cmp    eax, 4
        jne    loc_1_3259
        xor    esi, esi
loc_1_3259:
        inc    di
        cmp    di, 2
        jb     loc_1_323B
        xor    di, di
loc_1_3264:
        movzx  eax, di
        imul   eax, eax, 0xe0
        push   0
        add    eax, dword ptr [ebp - 8]
        push   dword ptr [ebp + 8]
        add    eax, 0x2ee
        push   eax
        call   sub_4_1548
loc_1_3280:
        add    esp, 0xc
        cmp    eax, 4
        jne    loc_1_328A
        xor    esi, esi
loc_1_328A:
        inc    di
        cmp    di, 1
        jb     loc_1_3264
        xor    di, di
loc_1_3295:
        movzx  eax, di
        imul   eax, eax, 0x5a
        push   0
        add    eax, dword ptr [ebp - 8]
        push   dword ptr [ebp + 8]
        add    eax, 0x3ce
        push   eax
        call   sub_4_130A
loc_1_32AE:
        add    esp, 0xc
        cmp    eax, 4
        jne    loc_1_32B8
        xor    esi, esi
loc_1_32B8:
        inc    di
        cmp    di, 1
        jb     loc_1_3295
        test   esi, esi
        jne    loc_1_333A
        xor    si, si
        mov    edi, 0xec
loc_1_32CC:
        push   dword ptr [ebp + 8]
        movzx  eax, si
        imul   eax, edi
        inc    si
        add    eax, dword ptr [ebp - 8]
        add    eax, edi
        push   eax
        call   sub_1_A09E
        add    esp, 8
        cmp    si, 2
        jb     loc_1_32CC
        xor    si, si
loc_1_32EE:
        push   dword ptr [ebp + 8]
        movzx  eax, si
        inc    si
        imul   eax, eax, 0xe0
        add    eax, dword ptr [ebp - 8]
        add    eax, 0x2ee
        push   eax
        call   sub_4_15DE
loc_1_330A:
        add    esp, 8
        cmp    si, 1
        jb     loc_1_32EE
        xor    si, si
loc_1_3316:
        push   dword ptr [ebp + 8]
        movzx  eax, si
        inc    si
        imul   eax, eax, 0x5a
        add    eax, dword ptr [ebp - 8]
        add    eax, 0x3ce
        push   eax
        call   sub_4_137F
loc_1_332F:
        add    esp, 8
        cmp    si, 1
        jb     loc_1_3316
        jmp    loc_1_3384
loc_1_333A:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        or     byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 8]
        push   2
        push   ecx
        mov    dword ptr [eax + 0xd8], ecx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x67d
        push   eax
        call   sub_1_4614
        add    esp, 0xc
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_3373
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_9CE4
        jmp    loc_1_3381
loc_1_3373:
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [ebp + 0xc]
        call   sub_1_9CF5
loc_1_3381:
        add    esp, 8
loc_1_3384:
        xor    eax, eax
loc_1_3386:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_338D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    esi, 1
        movzx  edx, word ptr [ebp + 0xc]
        push   eax
        push   ecx
        push   edx
        push   esi
        call   sub_1_2E0C
        add    esp, 0x10
        test   eax, eax
        je     loc_1_33BC
        mov    eax, esi
        jmp    loc_1_34B6
loc_1_33BC:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 8
        je     loc_1_33ED
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xdc], edi
        jne    loc_1_34B4
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_34A3
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_9CE4
        jmp    loc_1_34B1
loc_1_33ED:
        call   sub_1_A58C
        mov    edi, dword ptr [ebp + 8]
        cmp    eax, edi
        jne    loc_1_3413
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_34A3
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_9CE4
        jmp    loc_1_34B1
loc_1_3413:
        xor    bx, bx
loc_1_3416:
        movzx  eax, bx
        imul   eax, eax, 0x229
        push   0
        add    eax, dword ptr [ebp - 8]
        push   edi
        add    eax, 0x428
        push   eax
        call   sub_1_63E1
        add    esp, 0xc
        test   eax, eax
        je     loc_1_3439
        xor    esi, esi
loc_1_3439:
        inc    bx
        cmp    bx, 1
        jb     loc_1_3416
        test   esi, esi
        jne    loc_1_346D
        xor    si, si
loc_1_3448:
        movzx  eax, si
        inc    si
        push   edi
        imul   eax, eax, 0x229
        add    eax, dword ptr [ebp - 8]
        add    eax, 0x428
        push   eax
        call   sub_1_6531
        add    esp, 8
        cmp    si, 1
        jb     loc_1_3448
        jmp    loc_1_34B4
loc_1_346D:
        mov    eax, dword ptr [ebp - 8]
        push   2
        push   edi
        or     byte ptr [eax], 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xdc], edi
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x67d
        push   eax
        call   sub_1_46AB
        add    esp, 0xc
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_34A3
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_1_9CE4
        jmp    loc_1_34B1
loc_1_34A3:
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [ebp + 0xc]
        call   sub_1_9CF5
loc_1_34B1:
        add    esp, 8
loc_1_34B4:
        xor    eax, eax
loc_1_34B6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_34BD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   byte ptr [edi + 4], 1
        je     loc_1_3744
        lea    esi, [edi + 0xd5]
        push   1
        mov    ax, word ptr [esi]
        add    ax, 6
        push   eax
        call   sub_1_9CF5
        xor    ax, ax
        add    esp, 8
loc_1_34EE:
        inc    ax
        cmp    ax, 0xffff
        jb     loc_1_34EE
        mov    ax, word ptr [esi]
        push   0
        add    ax, 6
        xor    bx, bx
        push   eax
        call   sub_1_9CF5
        add    esp, 8
loc_1_350B:
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, word ptr [esi]
        add    ax, 0xe
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 0x80
        je     loc_1_353F
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, word ptr [esi]
        add    ax, 0xa
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0xaa
        je     loc_1_3578
loc_1_353F:
        inc    bx
        cmp    bx, 0xffff
        jb     loc_1_350B
        cmp    byte ptr [ebp - 1], 0xaa
        je     loc_1_3578
        push   1
        lea    eax, [ebp - 4]
        mov    word ptr [ebp - 4], 0
        push   2
        push   eax
        push   3
        push   0                             ; -> obj1:0x0140
        push   0                             ; -> loc_1_0138
        push   dword ptr [edi + 8]
        call   sub_1_A5B8
        add    esp, 0x1c
        jmp    loc_1_3744
loc_1_3578:
        test   byte ptr [edi + 4], 2
        je     loc_1_3631
        mov    dword ptr [ebp - 8], 9
        lea    ebx, [edi + 0xd7]
loc_1_358F:
        mov    ax, word ptr [ebx]
        push   0xff
        inc    ax
        push   eax
        call   sub_1_9CF5
        mov    word ptr [ebp - 4], 0
        add    esp, 8
loc_1_35A8:
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, word ptr [ebx]
        inc    ax
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 0x40
        je     loc_1_35CC
        inc    word ptr [ebp - 4]
        cmp    word ptr [ebp - 4], 0xffff
        jb     loc_1_35A8
loc_1_35CC:
        mov    ax, word ptr [ebx]
        push   0xff
        inc    ax
        push   eax
        call   sub_1_9CF5
        mov    word ptr [ebp - 4], 0
        add    esp, 8
loc_1_35E5:
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, word ptr [ebx]
        inc    ax
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 0x80
        je     loc_1_3609
        inc    word ptr [ebp - 4]
        cmp    word ptr [ebp - 4], 0xffff
        jb     loc_1_35E5
loc_1_3609:
        mov    cx, word ptr [ebx]
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0xfe
        je     loc_1_3631
        and    byte ptr [edi + 4], 0xfd
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [ebp - 8]
        test   eax, eax
        jne    loc_1_358F
loc_1_3631:
        test   byte ptr [edi + 4], 2
        mov    word ptr [ebp - 4], 0
        jne    loc_1_365C
        push   1
        lea    eax, [ebp - 4]
        push   2
        push   eax
        push   3
        push   0                             ; -> loc_1_012C
        push   0                             ; -> loc_1_0138
        push   dword ptr [edi + 8]
        call   sub_1_A5B8
        add    esp, 0x1c
loc_1_365C:
        test   byte ptr [edi], 2
        je     loc_1_36AD
        mov    ax, word ptr [esi]
        push   eax
        call   sub_1_3C93
        add    esp, 4
        test   eax, eax
        je     loc_1_3740
        test   byte ptr [edi + 4], 0x20
        mov    ax, word ptr [esi]
        push   eax
        jne    loc_1_368C
        push   dword ptr [edi + 8]
        call   sub_1_3EB9
        jmp    loc_1_373D
loc_1_368C:
        call   sub_1_3F76
        add    esp, 4
        test   eax, eax
        je     loc_1_3740
        mov    ax, word ptr [esi]
        push   eax
        push   dword ptr [edi + 8]
        call   sub_1_3FE6
        jmp    loc_1_373D
loc_1_36AD:
        movzx  eax, byte ptr [edi + 0xdf]
        cmp    eax, 5
        je     loc_1_36CA
        cmp    eax, 7
        je     loc_1_36CE
        cmp    eax, 9
        je     loc_1_36D2
        cmp    eax, 0xa
        je     loc_1_36D6
        jmp    loc_1_3744
loc_1_36CA:
        mov    bl, 2
        jmp    loc_1_36D8
loc_1_36CE:
        mov    bl, 4
        jmp    loc_1_36D8
loc_1_36D2:
        mov    bl, 1
        jmp    loc_1_36D8
loc_1_36D6:
        mov    bl, 8
loc_1_36D8:
        mov    ax, word ptr [esi]
        push   0x80
        add    ax, 4
        push   eax
        call   sub_1_9CF5
        mov    ax, word ptr [esi]
        add    esp, 8
        add    ax, 5
        push   ebx
        push   eax
        mov    bl, 1
        call   sub_1_9CF5
        add    esp, 8
        mov    cl, byte ptr [edi + 0xdd]
        shl    bl, cl
        test   byte ptr [edi + 4], 0x20
        je     loc_1_371A
        mov    al, 1
        mov    cl, byte ptr [edi + 0xde]
        shl    al, cl
        or     bl, al
loc_1_371A:
        mov    ax, word ptr [esi]
        push   0x81
        add    ax, 4
        push   eax
        call   sub_1_9CF5
        mov    ax, word ptr [esi]
        add    esp, 8
        add    ax, 5
        push   ebx
        push   eax
        call   sub_1_9CF5
loc_1_373D:
        add    esp, 8
loc_1_3740:
        xor    eax, eax
        jmp    loc_1_3749
loc_1_3744:
        mov    eax, 1
loc_1_3749:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3750:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    si, word ptr [eax + 0xd5]
        push   0xe0
        add    si, 0x800
        lea    edi, [esi + 0x402]
        push   edi
        call   sub_1_9D15
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        mov    ax, si
        add    ax, 0x400
        push   eax
        call   sub_1_9D02
        add    esp, 8
        test   byte ptr [ebp - 2], 0xc
        jne    loc_1_379D
        mov    eax, 1
        jmp    loc_1_3805
loc_1_379D:
        push   0x3d
        push   edi
        call   sub_1_9D15
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_1_9D02
        add    esp, 8
        test   byte ptr [ebp - 2], 0x58
        jne    loc_1_37C2
        mov    eax, 1
        jmp    loc_1_3805
loc_1_37C2:
        push   0x3e
        push   edi
        call   sub_1_9D15
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_1_9D02
        add    esp, 8
        mov    eax, 1
        test   byte ptr [ebp - 2], 3
        je     loc_1_3805
        push   0x3f
        push   edi
        call   sub_1_9D15
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_1_9D02
        add    esp, 8
        cmp    word ptr [ebp - 2], 1
        sbb    eax, eax
        inc    eax
loc_1_3805:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_380B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   dword ptr [ebp + 8]
        push   4
        call   sub_1_2E0C
        add    esp, 0x10
        test   eax, eax
        mov    eax, 1
        jne    loc_1_3842
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ecx + 0xe0]
        mov    eax, 1
        adc    eax, -1
loc_1_3842:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3846:
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_1_5856
        add    esp, 0xc
        ret    
sub_1_385B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        cmp    word ptr [0xc], 4             ; -> loc_1_00FC
        jae    loc_1_38DA
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [0xe]               ; -> loc_1_00FE
        call   sub_1_97E9
        add    esp, 8
        test   eax, eax
        jne    loc_1_38DA
        push   dword ptr [ebp - 8]
        push   dword ptr [0xe]               ; -> loc_1_00FE
        call   sub_1_9812
        add    esp, 8
        lea    ecx, [ebp - 4]
        push   ecx
        push   0x7ab
        call   sub_1_9890
        add    esp, 8
        test   eax, eax
        jne    loc_1_38DA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 4], edx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax], 1
        xor    eax, eax
        inc    word ptr [0xc]                ; -> loc_1_00FC
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        jmp    loc_1_38DF
loc_1_38DA:
        mov    eax, 1
loc_1_38DF:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_38E3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [0xe]               ; -> loc_1_00FE
        call   sub_1_987A
        add    esp, 8
        push   dword ptr [esi + 4]
        push   dword ptr [0xe]               ; -> loc_1_00FE
        call   sub_1_9864
        mov    cx, word ptr [0]              ; -> obj1:0x916C
        add    esp, 8
        cmp    word ptr [esi + 0x683], cx
        jne    loc_1_3937
        xor    eax, eax
        mov    word ptr [0], ax              ; -> obj1:0x916C
        cmp    dword ptr [0], eax            ; -> sub_1_9178
        je     loc_1_3937
        push   eax
        push   eax
        push   6
        call   sub_1_9178
        add    esp, 0xc
loc_1_3937:
        push   esi
        call   sub_1_98BE
        dec    word ptr [0xc]                ; -> loc_1_00FC
        add    esp, 4
        xor    eax, eax
        pop    esi
        ret    
sub_1_394B:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 2
        je     loc_1_395B
        cmp    eax, 3
        je     loc_1_3967
        jmp    loc_1_397E
loc_1_395B:
        mov    ecx, dword ptr [esp + 4]
        inc    dword ptr [ecx + 0xe0]
        jmp    loc_1_397A
loc_1_3967:
        mov    ecx, dword ptr [esp + 4]
        add    ecx, 0xe0
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     loc_1_397E
        dec    eax
        mov    dword ptr [ecx], eax
loc_1_397A:
        xor    eax, eax
        jmp    loc_1_3983
loc_1_397E:
        mov    eax, 1
loc_1_3983:
        ret    
sub_1_3984:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 0x7a5]
        xor    eax, eax
        mov    word ptr [edx], cx
        ret    
sub_1_3999:
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 8]
        add    eax, 0x67d
        push   eax
        call   sub_1_4807
        add    esp, 8
        ret    
sub_1_39B0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        add    eax, 0x713
        mov    dword ptr [ecx], eax
        ret    
sub_1_39C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    si, word ptr [ebp + 8]
        push   edi
        mov    ax, si
        push   0xe3
        add    ax, 0xc
        xor    di, di
        push   eax
        call   sub_1_9CF5
        add    esp, 8
loc_1_39E5:
        xor    bx, bx
loc_1_39E8:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0xe]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 0x80
        jne    loc_1_3A0D
        inc    bx
        cmp    bx, 0x200
        jb     loc_1_39E8
        test   byte ptr [ebp - 1], 0x80
        je     loc_1_3A52
loc_1_3A0D:
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, si
        add    ax, 0xa
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_3A49
        cmp    di, 0x28
        jae    loc_1_3A41
        mov    eax, dword ptr [0x30]         ; -> obj1:0x00E8
        movzx  ecx, di
        movsx  edx, byte ptr [ecx + eax]
        movzx  ecx, byte ptr [ebp - 1]
        cmp    edx, ecx
        jne    loc_1_3A59
loc_1_3A41:
        inc    di
        cmp    di, 0x2d
        jb     loc_1_39E5
loc_1_3A49:
        xor    eax, eax
loc_1_3A4B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_3A52:
        mov    eax, 1
        jmp    loc_1_3A4B
loc_1_3A59:
        mov    eax, 1
        jmp    loc_1_3A4B
sub_1_3A60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        test   eax, eax
        push   esi
        je     loc_1_3ACC
        cmp    eax, 2
        jb     loc_1_3A85
        cmp    eax, 3
        jbe    loc_1_3ACC
        cmp    eax, 0x1d4048
        je     loc_1_3A8C
        cmp    eax, -1
        je     loc_1_3ACC
loc_1_3A85:
        mov    eax, 1
        jmp    loc_1_3ACE
loc_1_3A8C:
        lea    eax, [ebp - 2]
        push   eax
        push   dword ptr [0x30]              ; -> obj1:0x00E8
        call   sub_1_23AF
        add    esp, 8
        mov    esi, dword ptr [ebp + 0x14]
        movzx  ecx, word ptr [ebp - 2]
        cmp    dword ptr [esi + 0x1c], ecx
        jae    loc_1_3AB1
        mov    eax, 1
        jmp    loc_1_3ACE
loc_1_3AB1:
        mov    eax, dword ptr [0x30]         ; -> obj1:0x00E8
        push   eax
        push   dword ptr [esi + 0x18]
        call   sub_1_23D2
        mov    cx, word ptr [ebp - 2]
        add    esp, 8
        mov    edx, dword ptr [esi + 0x20]
        mov    word ptr [edx], cx
loc_1_3ACC:
        xor    eax, eax
loc_1_3ACE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3AD3:
        push   ebp
        mov    ax, word ptr [esp + 8]
        mov    ebp, esp
        push   dword ptr [esp + 0xc]
        add    ax, 4
        push   eax
        call   sub_1_9CF5
        mov    ax, word ptr [ebp + 8]
        add    esp, 8
        push   dword ptr [ebp + 0x10]
        add    ax, 5
        push   eax
        call   sub_1_9CF5
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3B01:
        push   ebp
        mov    ax, word ptr [esp + 8]
        mov    ebp, esp
        add    ax, 4
        sub    esp, 4
        push   dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_9CF5
        mov    ax, word ptr [ebp + 8]
        add    esp, 8
        add    ax, 5
        lea    ecx, [ebp - 1]
        push   ecx
        push   eax
        call   sub_1_9CE4
        movzx  ax, byte ptr [ebp - 1]
        add    esp, 8
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3B3A:
        sub    esp, 4
        push   ebx
        push   esi
        xor    bx, bx
        push   edi
        mov    di, word ptr [esp + 0x14]
        push   ebp
        push   0x82
        push   edi
        call   sub_1_3B01
        and    ax, 0xf0
        add    esp, 8
        cmp    ax, 0x10
        mov    si, ax
        je     loc_1_3C60
        cmp    si, 0x20
        je     loc_1_3C60
        cmp    si, 0x80
        je     loc_1_3C60
        cmp    si, 0xa0
        je     loc_1_3C5B
        cmp    si, 0xb0
        je     loc_1_3C5B
        cmp    si, 0xc0
        je     loc_1_3C5B
        push   edi
        call   sub_1_3C93
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        test   eax, eax
        je     loc_1_3BBF
        push   0x14
        push   0x81
        push   edi
        call   sub_1_3AD3
        add    esp, 0xc
loc_1_3BBF:
        cmp    si, 0x40
        jne    loc_1_3C01
        mov    si, 4
        mov    ebp, 0xff
loc_1_3BCE:
        push   esi
        push   ebp
        push   edi
        call   sub_1_3AD3
        add    esp, 0xc
        push   ebp
        push   edi
        call   sub_1_3B01
        add    esp, 8
        test   ax, ax
        je     loc_1_3BED
        cmp    ax, bp
        jne    loc_1_3BF7
loc_1_3BED:
        inc    si
        cmp    si, 5
        jbe    loc_1_3BCE
        jmp    loc_1_3C01
loc_1_3BF7:
        shl    ax, 8
        or     ax, si
        mov    bx, ax
loc_1_3C01:
        test   bx, bx
        jne    loc_1_3C37
        push   0xfe
        push   edi
        call   sub_1_3B01
        add    esp, 8
        test   ax, ax
        je     loc_1_3C37
        cmp    ax, 0xff
        je     loc_1_3C37
        mov    bx, ax
        push   0xfd
        push   edi
        call   sub_1_3B01
        shl    ax, 8
        add    esp, 8
        or     bx, ax
loc_1_3C37:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_3C88
        mov    esi, 0x81
        push   esi
        push   edi
        call   sub_1_3B01
        add    esp, 8
        push   1
        push   esi
        push   edi
        call   sub_1_3AD3
        add    esp, 0xc
        jmp    loc_1_3C88
loc_1_3C5B:
        xor    ax, ax
        jmp    loc_1_3C8B
loc_1_3C60:
        push   3
        mov    esi, 0xff
        push   esi
        push   edi
        call   sub_1_3AD3
        add    esp, 0xc
        push   esi
        push   edi
        call   sub_1_3B01
        add    esp, 8
        cmp    ax, 0x60
        jne    loc_1_3C88
        mov    bx, ax
        mov    bh, bl
        mov    bl, 3
loc_1_3C88:
        mov    ax, bx
loc_1_3C8B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_3C93:
        push   ebx
        push   esi
        push   edi
        mov    esi, 0xff
        mov    di, word ptr [esp + 0x10]
        push   ebp
        push   0x20
        push   esi
        push   edi
        call   sub_1_3AD3
        add    esp, 0xc
        push   esi
        push   edi
        call   sub_1_3B01
        add    esp, 8
        cmp    ax, 0x60
        jne    loc_1_3D01
        mov    ebp, 0x81
        push   ebp
        push   edi
        call   sub_1_3B01
        mov    si, ax
        add    esp, 8
        push   0x14
        push   ebp
        push   edi
        call   sub_1_3AD3
        add    esp, 0xc
        xor    ebx, ebx
        push   ebp
        push   edi
        call   sub_1_3B01
        mov    bx, ax
        add    esp, 8
        and    bl, 0xeb
        push   esi
        push   ebp
        push   edi
        call   sub_1_3AD3
        add    esp, 0xc
        mov    eax, 1
        test   bl, bl
        jne    loc_1_3D03
loc_1_3D01:
        xor    eax, eax
loc_1_3D03:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3D08:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    si, word ptr [ebp + 8]
        push   0x84
        lea    edi, [esi + 4]
        add    si, 5
        push   edi
        call   sub_1_9CF5
        add    esp, 8
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_9CE4
        add    esp, 8
        cmp    word ptr [ebp + 0xc], 0x300
        jne    loc_1_3D48
        and    byte ptr [ebp - 1], 0xfd
        or     byte ptr [ebp - 1], 4
        jmp    loc_1_3D54
loc_1_3D48:
        cmp    word ptr [ebp + 0xc], 0x330
        jne    loc_1_3D54
        and    byte ptr [ebp - 1], 0xf9
loc_1_3D54:
        push   0x84
        push   edi
        call   sub_1_9CF5
        add    esp, 8
        push   dword ptr [ebp - 1]
        push   esi
        call   sub_1_9CF5
        add    esp, 8
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3D74:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    word ptr [ebp - 0x14], 0xffff
        mov    word ptr [ebp - 0x12], 0
        mov    word ptr [ebp - 0x10], 0xff00
        mov    word ptr [ebp - 0xe], 0x100
        mov    word ptr [ebp - 0xc], 0xfe00
        mov    word ptr [ebp - 0xa], 0x200
        mov    word ptr [ebp - 8], 0xffff
        mov    word ptr [ebp - 6], 0x420
        push   esi
        push   edi
        mov    esi, 5
        mov    eax, 1
        mov    word ptr [ebp - 4], 0xfffe
        movzx  cx, byte ptr [ebp + 0xc]
        mov    word ptr [ebp - 2], 0x520
        movzx  di, byte ptr [ebp + 8]
        shl    di, 8
        or     di, cx
        lea    ecx, [ebp - 4]
loc_1_3DD6:
        mov    dx, word ptr [ecx]
        and    dx, di
        cmp    dx, word ptr [ecx + 2]
        je     loc_1_3DEC
        sub    ecx, 4
        dec    esi
        test   esi, esi
        jg     loc_1_3DD6
        jmp    loc_1_3DEE
loc_1_3DEC:
        xor    eax, eax
loc_1_3DEE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3DF4:
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0x90
        mov    di, word ptr [esp + 0x10]
        push   ebx
        push   edi
        call   sub_1_3B01
        mov    si, ax
        add    esp, 8
        xor    si, 1
        push   esi
        push   ebx
        push   edi
        call   sub_1_3AD3
        add    esp, 0xc
        push   ebx
        push   edi
        call   sub_1_3B01
        sub    si, ax
        add    esp, 8
        cmp    si, 1
        pop    edi
        sbb    eax, eax
        pop    esi
        neg    eax
        pop    ebx
        ret    
sub_1_3E36:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    si, word ptr [ebp + 0x10]
        test   si, si
        je     loc_1_3E76
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_9CE4
        add    esp, 8
        mov    al, byte ptr [ebp + 8]
        test   al, al
        jne    loc_1_3E60
        and    byte ptr [ebp - 1], 0xfe
        jmp    loc_1_3E68
loc_1_3E60:
        cmp    al, 1
        jne    loc_1_3E68
        or     byte ptr [ebp - 1], 1
loc_1_3E68:
        push   dword ptr [ebp - 1]
        push   esi
        call   sub_1_9CF5
        add    esp, 8
        jmp    loc_1_3EB4
loc_1_3E76:
        push   0x90
        push   dword ptr [ebp + 0xc]
        call   sub_1_3B01
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp + 8]
        test   al, al
        jne    loc_1_3E96
        and    byte ptr [ebp - 1], 0xfe
        jmp    loc_1_3E9E
loc_1_3E96:
        cmp    al, 1
        jne    loc_1_3E9E
        or     byte ptr [ebp - 1], 1
loc_1_3E9E:
        movzx  ax, byte ptr [ebp - 1]
        push   eax
        push   0x90
        push   dword ptr [ebp + 0xc]
        call   sub_1_3AD3
        add    esp, 0xc
loc_1_3EB4:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3EB9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        push   edi
        push   eax
        mov    edi, 0x81
        call   sub_1_9B4D
        mov    si, word ptr [ebp + 0xc]
        add    esp, 4
        push   edi
        push   esi
        call   sub_1_3B01
        mov    bx, ax
        add    esp, 8
        push   0x14
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0xc
        push   edi
        push   esi
        call   sub_1_3B01
        mov    word ptr [ebp - 2], ax
        add    esp, 8
        push   ebx
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0xc
        test   byte ptr [ebp - 2], 0xeb
        je     loc_1_3F45
        push   0x14
        push   edi
        push   esi
        call   sub_1_3AD3
        mov    ax, word ptr [ebp - 2]
        add    esp, 0xc
        and    al, 0xb
        movzx  ax, al
        push   eax
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0xc
        push   edi
        push   esi
        call   sub_1_3B01
        add    esp, 8
        push   eax
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0xc
loc_1_3F45:
        push   dword ptr [ebp - 8]
        call   sub_1_9B5B
        add    esp, 4
        lea    eax, [ebp - 2]
        push   1
        push   1
        push   eax
        push   3
        push   0                             ; -> loc_1_0108
        push   0                             ; -> obj1:0x0118
        push   dword ptr [ebp + 8]
        call   sub_1_A5B8
        add    esp, 0x1c
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3F76:
        sub    esp, 4
        lea    eax, [esp]
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   eax
        mov    ebp, 0x81
        call   sub_1_9B4D
        mov    si, word ptr [esp + 0x1c]
        add    esp, 4
        push   ebp
        push   esi
        call   sub_1_3B01
        mov    di, ax
        add    esp, 8
        push   0x14
        push   ebp
        push   esi
        call   sub_1_3AD3
        add    esp, 0xc
        push   ebp
        push   esi
        call   sub_1_3B01
        mov    bx, ax
        add    esp, 8
        push   edi
        push   ebp
        and    bx, 0xe0
        push   esi
        call   sub_1_3AD3
        add    esp, 0xc
        push   dword ptr [esp + 0x10]
        call   sub_1_9B5B
        add    esp, 4
        cmp    bx, 1
        pop    ebp
        sbb    eax, eax
        pop    edi
        neg    eax
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_3FE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 2], 0
        push   esi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0xc], 1
        push   edi
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 2]
        push   1
        push   eax
        push   ecx
        push   3
        push   0                             ; -> loc_1_0108
        push   0                             ; -> obj1:0x0118
        mov    edi, 0x81
        push   dword ptr [ebp + 8]
        call   sub_1_A5C0
        mov    si, word ptr [ebp + 0xc]
        add    esp, 0x1c
        push   0x14
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0xc
        push   dword ptr [ebp - 2]
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0xc
        push   edi
        push   esi
        call   sub_1_3B01
        mov    word ptr [ebp - 2], ax
        add    esp, 8
        push   dword ptr [ebp - 2]
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0xc
        push   dword ptr [ebp - 8]
        call   sub_1_9B5B
        add    esp, 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_4076:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 0x18]
        mov    ebp, esp
        mov    ax, word ptr [eax + 0x1a]
        sub    esp, 4
        mov    word ptr [ebp - 4], cx
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 4]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        mov    al, byte ptr [ebp + 0x10]
        mov    dx, word ptr [ebp - 2]
        out    dx, al
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_40A6:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 0x18]
        mov    ebp, esp
        mov    ax, word ptr [eax + 0x1a]
        sub    esp, 4
        mov    word ptr [ebp - 4], cx
        push   esi
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 4]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        mov    esi, dword ptr [ebp + 0x10]
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        mov    byte ptr [esi], al
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_40DA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    si, si
        push   edi
        mov    edi, dword ptr [ebp + 8]
loc_1_40E9:
        mov    ax, word ptr [edi + 0x22]
        push   dword ptr [ebp + 0xc]
        xor    bx, bx
        push   eax
        call   sub_1_9CF5
        add    esp, 8
loc_1_40FC:
        mov    cx, word ptr [edi + 0x20]
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 0x80
        jne    loc_1_412A
        mov    cx, word ptr [edi + 0x1e]
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0xfe
        je     loc_1_4147
loc_1_412A:
        inc    bx
        cmp    bx, 0xffff
        jb     loc_1_40FC
        inc    si
        cmp    si, 0xa
        jb     loc_1_40E9
        mov    eax, 1
loc_1_4140:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_4147:
        xor    eax, eax
        jmp    loc_1_4140
sub_1_414B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    si, si
        push   edi
        mov    bx, word ptr [ebp + 0x10]
        cmp    bx, si
        je     loc_1_4181
        mov    edi, dword ptr [ebp + 8]
loc_1_4163:
        mov    cx, word ptr [edi + 0x20]
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 0x80
        je     loc_1_418D
        inc    si
        cmp    si, bx
        jb     loc_1_4163
loc_1_4181:
        mov    eax, 1
loc_1_4186:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_418D:
        push   dword ptr [ebp + 0xc]
        mov    cx, word ptr [edi + 0x1e]
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_4186
sub_1_41A1:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ax, word ptr [eax + 0x14]
        mov    word ptr [esp + 2], ax
        mov    dx, word ptr [esp + 2]
        in     al, dx
        xor    eax, eax
        add    esp, 4
        ret    
sub_1_41BD:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ax, word ptr [eax + 0x16]
        mov    word ptr [esp + 2], ax
        mov    dx, word ptr [esp + 2]
        in     al, dx
        xor    eax, eax
        add    esp, 4
        ret    
sub_1_41D9:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ax, word ptr [eax + 0x1c]
        mov    word ptr [esp + 2], ax
        mov    dx, word ptr [esp + 2]
        in     al, dx
        xor    eax, eax
        add    esp, 4
        ret    
sub_1_41F5:
        push   ebp
        xor    eax, eax
        mov    dx, word ptr [esp + 0xc]
        mov    ebp, esp
        mov    ax, dx
        and    eax, 0xf00
        cmp    eax, 0x200
        je     loc_1_421C
        cmp    eax, 0x300
        je     loc_1_423A
        mov    eax, 1
        jmp    loc_1_4286
loc_1_421C:
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 4]
        xor    ax, dx
        test   al, 0xf0
        jne    loc_1_4233
        test   byte ptr [ecx], 2
        je     loc_1_4233
        xor    eax, eax
        jmp    loc_1_4286
loc_1_4233:
        mov    eax, 1
        jmp    loc_1_4286
loc_1_423A:
        xor    eax, eax
        mov    ax, dx
        and    eax, 0xf0
        je     loc_1_4259
        cmp    eax, 0x30
        je     loc_1_4259
        cmp    eax, 0x80
        je     loc_1_4277
        mov    eax, 1
        jmp    loc_1_4286
loc_1_4259:
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 6]
        xor    ax, dx
        test   al, 0xf0
        jne    loc_1_4270
        test   byte ptr [ecx], 4
        je     loc_1_4270
        xor    eax, eax
        jmp    loc_1_4286
loc_1_4270:
        mov    eax, 1
        jmp    loc_1_4286
loc_1_4277:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        and    eax, 8
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_1_4286:
        pop    ebp
        ret    
sub_1_4288:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0x80
        mov    ax, word ptr [esi + 0xa]
        mov    cx, word ptr [esi + 0x12]
        mov    dx, word ptr [esi + 0x10]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], cx
        mov    word ptr [ebp - 4], dx
        mov    dx, word ptr [ebp - 8]
        mov    al, 1
        out    dx, al
        mov    ecx, 0xffff
loc_1_42BB:
        loop   loc_1_42BB
        mov    al, 0
        out    dx, al
        mov    cx, word ptr [ebp + 0xc]
        mov    dx, word ptr [ebp - 6]
loc_1_42C8:
        in     al, dx
        and    al, byte ptr [ebp - 2]
        jne    loc_1_42D0
        loop   loc_1_42C8
loc_1_42D0:
        mov    dx, word ptr [ebp - 4]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [ebp - 1], 0xaa
        mov    eax, 1
        jne    loc_1_4338
        mov    edi, 0xffff
        push   edi
        push   0xf
        push   esi
        call   sub_1_436F
        add    esp, 0xc
        push   edi
        push   5
        push   esi
        call   sub_1_436F
        add    esp, 0xc
        lea    eax, [ebp - 1]
        push   edi
        push   eax
        push   esi
        call   sub_1_43EF
        add    esp, 0xc
        push   edi
        push   0xe
        push   esi
        call   sub_1_436F
        add    esp, 0xc
        push   edi
        push   5
        push   esi
        call   sub_1_436F
        add    esp, 0xc
        mov    al, byte ptr [ebp - 1]
        or     al, 3
        push   edi
        push   eax
        push   esi
        call   sub_1_436F
        add    esp, 0xc
        xor    eax, eax
loc_1_4338:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_433E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 0xe]
        mov    ebp, esp
        mov    ax, word ptr [eax + 0xc]
        sub    esp, 4
        mov    word ptr [ebp - 4], cx
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 4]
loc_1_435C:
        in     al, dx
        or     al, al
        js     loc_1_435C
        mov    dx, word ptr [ebp - 2]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_436F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 0xe]
        mov    ebp, esp
        mov    ax, word ptr [eax + 0xc]
        sub    esp, 8
        mov    word ptr [ebp - 2], 0
        mov    word ptr [ebp - 6], cx
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 6]
        mov    cx, word ptr [ebp + 0x10]
loc_1_4397:
        in     al, dx
        or     al, al
        jns    loc_1_43A6
        loop   loc_1_4397
        mov    word ptr [ebp - 2], 1
        jmp    loc_1_43AE
loc_1_43A6:
        mov    dx, word ptr [ebp - 4]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
loc_1_43AE:
        cmp    word ptr [ebp - 2], 1
        sbb    eax, eax
        mov    esp, ebp
        inc    eax
        pop    ebp
        ret    
sub_1_43BA:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 0x12]
        mov    ebp, esp
        mov    ax, word ptr [eax + 0x10]
        sub    esp, 4
        mov    word ptr [ebp - 4], cx
        push   esi
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 4]
loc_1_43D9:
        in     al, dx
        or     al, al
        jns    loc_1_43D9
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_43EF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 0x12]
        mov    ebp, esp
        mov    ax, word ptr [eax + 0x10]
        sub    esp, 8
        mov    word ptr [ebp - 6], cx
        push   esi
        mov    word ptr [ebp - 2], 0
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 6]
        mov    cx, word ptr [ebp + 0x10]
loc_1_4418:
        in     al, dx
        or     al, al
        js     loc_1_4427
        loop   loc_1_4418
        mov    word ptr [ebp - 2], 1
        jmp    loc_1_4431
loc_1_4427:
        mov    dx, word ptr [ebp - 4]
        in     al, dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
loc_1_4431:
        cmp    word ptr [ebp - 2], 1
        sbb    eax, eax
        pop    esi
        inc    eax
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_443E:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 4], 0x220
        mov    word ptr [eax + 6], 0x330
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_1_4457:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    cx, word ptr [ebp + 0xc]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ecx + 6]
        lea    edi, [esi + 4]
        mov    word ptr [edi], cx
        lea    edx, [ecx + 0xa]
        mov    word ptr [esi + 0xa], ax
        or     byte ptr [esi], 1
        mov    word ptr [esi + 0x10], dx
        lea    eax, [ecx + 0xc]
        mov    word ptr [esi + 0xc], ax
        lea    ebx, [ecx + 0xf]
        mov    word ptr [esi + 0xe], ax
        lea    edx, [ecx + 4]
        mov    word ptr [esi + 0x16], bx
        lea    eax, [ecx + 0xe]
        add    cx, 5
        mov    bx, 4
        mov    word ptr [esi + 0x12], ax
        mov    word ptr [esi + 0x14], ax
        mov    word ptr [esi + 0x18], dx
        mov    word ptr [esi + 0x1a], cx
loc_1_44AB:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [edi]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    loc_1_455A
        inc    bx
        cmp    bx, 7
        jbe    loc_1_44AB
        mov    bx, 0xa
loc_1_44D3:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [edi]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    loc_1_4561
        inc    bx
        cmp    bx, 0xf
        jbe    loc_1_44D3
        mov    cx, word ptr [edi]
        xor    bx, bx
        lea    eax, [ecx + 8]
        add    cx, 9
        mov    word ptr [esi + 0x24], ax
        mov    word ptr [esi + 0x26], ax
        mov    word ptr [esi + 0x28], cx
loc_1_450C:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [edi]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    loc_1_4568
        inc    bx
        cmp    bx, 3
        jbe    loc_1_450C
        mov    bx, 8
loc_1_4530:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [edi]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    loc_1_456F
        inc    bx
        cmp    bx, 9
        jbe    loc_1_4530
        xor    eax, eax
        or     byte ptr [esi], 2
loc_1_4555:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_455A:
        mov    eax, 1
        jmp    loc_1_4555
loc_1_4561:
        mov    eax, 1
        jmp    loc_1_4555
loc_1_4568:
        mov    eax, 1
        jmp    loc_1_4555
loc_1_456F:
        mov    eax, 1
        jmp    loc_1_4555
sub_1_4576:
        mov    ax, word ptr [esp + 8]
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        xor    bx, bx
        mov    word ptr [esi + 6], ax
        mov    word ptr [esi + 0x1c], ax
        mov    word ptr [esi + 0x1e], ax
        inc    ax
        mov    word ptr [esi + 0x20], ax
        mov    word ptr [esi + 0x22], ax
loc_1_459A:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [esi + 6]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    loc_1_45C4
        inc    bx
        cmp    bx, 1
        jbe    loc_1_459A
        xor    eax, eax
        or     byte ptr [esi], 4
loc_1_45C1:
        pop    esi
        pop    ebx
        ret    
loc_1_45C4:
        mov    eax, 1
        jmp    loc_1_45C1
sub_1_45CB:
        mov    ax, word ptr [esp + 8]
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    word ptr [esi + 8], ax
        cmp    word ptr [esi + 4], ax
        je     loc_1_4605
        xor    bx, bx
loc_1_45E3:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [esi + 8]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    loc_1_460D
        inc    bx
        cmp    bx, 3
        jbe    loc_1_45E3
loc_1_4605:
        xor    eax, eax
        or     byte ptr [esi], 8
loc_1_460A:
        pop    esi
        pop    ebx
        ret    
loc_1_460D:
        mov    eax, 1
        jmp    loc_1_460A
sub_1_4614:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        cmp    eax, 1
        push   ebx
        push   esi
        push   edi
        je     loc_1_462F
        cmp    eax, 2
        je     loc_1_4636
        mov    eax, 1
        jmp    loc_1_46A6
loc_1_462F:
        mov    edi, 0                        ; -> loc_1_95E8
        jmp    loc_1_463B
loc_1_4636:
        mov    edi, 0                        ; -> sub_1_95FE
loc_1_463B:
        mov    bx, 4
        mov    esi, dword ptr [ebp + 8]
loc_1_4642:
        movzx  ax, bl
        add    ax, word ptr [esi + 4]
        inc    bx
        push   eax
        push   dword ptr [ebp + 0xc]
        call   edi
        add    esp, 8
        cmp    bx, 7
        jbe    loc_1_4642
        mov    bx, 0xa
loc_1_465F:
        movzx  ax, bl
        add    ax, word ptr [esi + 4]
        inc    bx
        push   eax
        push   dword ptr [ebp + 0xc]
        call   edi
        add    esp, 8
        cmp    bx, 0xf
        jbe    loc_1_465F
        cmp    word ptr [0], 0               ; -> obj1:0x916C
        je     loc_1_4688
        cmp    dword ptr [ebp + 0x10], 1
        jne    loc_1_46A4
loc_1_4688:
        xor    bx, bx
loc_1_468B:
        movzx  ax, bl
        add    ax, word ptr [esi + 6]
        inc    bx
        push   eax
        push   dword ptr [ebp + 0xc]
        call   edi
        add    esp, 8
        cmp    bx, 1
        jbe    loc_1_468B
loc_1_46A4:
        xor    eax, eax
loc_1_46A6:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_46AB:
        mov    eax, dword ptr [esp + 0xc]
        push   ebx
        push   esi
        cmp    eax, 1
        push   edi
        push   ebp
        je     loc_1_46C4
        cmp    eax, 2
        je     loc_1_46CB
        mov    eax, 1
        jmp    loc_1_4729
loc_1_46C4:
        mov    esi, 0                        ; -> loc_1_95E8
        jmp    loc_1_46D0
loc_1_46CB:
        mov    esi, 0                        ; -> sub_1_95FE
loc_1_46D0:
        xor    bx, bx
        mov    edi, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [esp + 0x18]
loc_1_46DB:
        movzx  ax, bl
        add    ax, word ptr [edi + 4]
        inc    bx
        push   eax
        push   ebp
        call   esi
        add    esp, 8
        cmp    bx, 3
        jbe    loc_1_46DB
        mov    bx, 8
loc_1_46F6:
        movzx  ax, bl
        add    ax, word ptr [edi + 4]
        inc    bx
        push   eax
        push   ebp
        call   esi
        add    esp, 8
        cmp    bx, 9
        jbe    loc_1_46F6
        xor    bx, bx
loc_1_4710:
        movzx  ax, bl
        add    ax, word ptr [edi + 8]
        inc    bx
        push   eax
        push   ebp
        call   esi
        add    esp, 8
        cmp    bx, 3
        jbe    loc_1_4710
        xor    eax, eax
loc_1_4729:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_472E:
        push   ebx
        push   esi
        mov    bx, 4
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        lea    edi, [esi + 4]
        and    byte ptr [esi], 0xfe
loc_1_473F:
        movzx  ax, bl
        add    ax, word ptr [edi]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 7
        jbe    loc_1_473F
        mov    bx, 0xa
loc_1_475B:
        movzx  ax, bl
        add    ax, word ptr [edi]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 0xf
        jbe    loc_1_475B
        xor    bx, bx
loc_1_4776:
        movzx  ax, bl
        add    ax, word ptr [edi]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 3
        jbe    loc_1_4776
        mov    bx, 8
loc_1_4792:
        movzx  ax, bl
        add    ax, word ptr [edi]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 9
        jbe    loc_1_4792
        xor    eax, eax
        pop    edi
        and    byte ptr [esi], 0xfd
        pop    esi
        pop    ebx
        ret    
sub_1_47B3:
        push   ebx
        push   esi
        xor    bx, bx
        mov    esi, dword ptr [esp + 0xc]
loc_1_47BC:
        movzx  ax, bl
        add    ax, word ptr [esi + 6]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 1
        jbe    loc_1_47BC
        xor    eax, eax
        and    byte ptr [esi], 0xfb
        pop    esi
        pop    ebx
        ret    
sub_1_47DD:
        push   ebx
        push   esi
        xor    bx, bx
        mov    esi, dword ptr [esp + 0xc]
loc_1_47E6:
        movzx  ax, bl
        add    ax, word ptr [esi + 8]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 3
        jbe    loc_1_47E6
        xor    eax, eax
        and    byte ptr [esi], 0xf7
        pop    esi
        pop    ebx
        ret    
sub_1_4807:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        je     loc_1_481F
        mov    cx, word ptr [eax + 4]
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax], cx
        xor    eax, eax
        jmp    loc_1_4824
loc_1_481F:
        mov    eax, 1
loc_1_4824:
        ret    
sub_1_4825:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0x82
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x18]
        push   eax
        call   sub_1_9CF5
        mov    cx, word ptr [esi + 0x1a]
        add    esp, 8
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        mov    al, byte ptr [ebp - 1]
        shr    al, 4
        mov    edx, dword ptr [ebp + 0xc]
        movzx  cx, al
        mov    word ptr [edx], cx
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_4869:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x310
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_1_48AB
        cmp    eax, 1
        je     loc_1_4A1F
        cmp    eax, 2
        je     loc_1_4ECD
        cmp    eax, 3
        je     loc_1_4F27
        cmp    eax, 4
        je     loc_1_4ECD
        mov    eax, 1
        jmp    loc_1_4F4B
loc_1_48AB:
        xor    edi, edi
        lea    eax, [ebp - 0x54]
        push   edi
        lea    ecx, [ebp - 0x1c]
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        mov    dword ptr [ebp - 0x54], edi
        push   ecx
        push   esi
        call   sub_3_0008
loc_1_48C3:
        add    esp, 0x14
        test   eax, eax
        je     loc_1_48CD
        mov    dword ptr [ebp - 0x1c], edi
loc_1_48CD:
        cmp    dword ptr [ebp - 0x1c], 0x10
        je     loc_1_4A18
        cmp    dword ptr [ebp - 0x1c], 0x40
        je     loc_1_4A18
        push   3
        lea    eax, [ebp - 0x10]
        push   esi
        push   eax
        call   sub_3_0058
loc_1_48ED:
        add    esp, 0xc
        test   eax, eax
        je     loc_1_48FB
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_48FB:
        mov    eax, dword ptr [ebp - 0x10]
        push   0
        lea    ecx, [ebp - 0x18]
        lea    edx, [ebp - 0xc]
        push   ecx
        mov    dword ptr [ebp - 0xc], eax
        xor    di, di
        push   2
        push   eax
        push   edx
        call   sub_3_0050
loc_1_4916:
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_494D
loc_1_491D:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + 4]
        mov    cx, ax
        and    ch, 0xf
        cmp    ch, 2
        jne    loc_1_4932
        mov    di, ax
loc_1_4932:
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0xc]
        push   2
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_3_0050
loc_1_4946:
        add    esp, 0x14
        test   eax, eax
        je     loc_1_491D
loc_1_494D:
        cmp    eax, 0xf
        je     loc_1_4959
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_4959:
        test   di, di
        jne    loc_1_4965
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_4965:
        push   edi
        call   sub_1_3B3A
        add    esp, 4
        cmp    al, 5
        je     loc_1_4979
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_4979:
        push   1
        lea    eax, [ebp - 0x10]
        push   esi
        push   eax
        call   sub_3_0058
loc_1_4985:
        add    esp, 0xc
        test   eax, eax
        je     loc_1_4993
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_4993:
        mov    eax, dword ptr [ebp - 0x10]
        push   0
        lea    ecx, [ebp - 0x18]
        lea    edx, [ebp - 0xc]
        push   ecx
        mov    dword ptr [ebp - 0xc], eax
        push   3
        xor    edi, edi
        push   eax
        xor    ebx, ebx
        push   edx
        call   sub_3_0050
loc_1_49AF:
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_49E3
loc_1_49B6:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [ecx + 2]
        test   al, 0xf
        je     loc_1_49C2
        mov    edi, ecx
loc_1_49C2:
        test   al, 0xf0
        je     loc_1_49C8
        mov    ebx, ecx
loc_1_49C8:
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0xc]
        push   3
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_3_0050
loc_1_49DC:
        add    esp, 0x14
        test   eax, eax
        je     loc_1_49B6
loc_1_49E3:
        cmp    eax, 0xf
        jne    loc_1_4F4B
        test   edi, edi
        je     loc_1_49F4
        and    byte ptr [edi + 2], 0xfe
loc_1_49F4:
        test   ebx, ebx
        je     loc_1_49FC
        and    byte ptr [ebx + 2], 0xbf
loc_1_49FC:
        push   0
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_3_0048
loc_1_4A0A:
        add    esp, 0xc
        test   eax, eax
        je     loc_1_4993
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_4A18:
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_4A1F:
        cmp    dword ptr [ebp + 0xc], 1
        jbe    loc_1_4A2F
        mov    eax, 1
        jmp    loc_1_4F4B
loc_1_4A2F:
        xor    eax, eax
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x50], eax
        mov    dword ptr [ebp - 0x210], eax
        push   eax
        lea    ecx, [ebp - 0x50]
        push   eax
        mov    dword ptr [ebp - 0x20c], eax
        push   ecx
        mov    byte ptr [ebp - 0x204], al
        lea    eax, [ebp - 0x24]
        mov    dword ptr [ebp - 0x208], esi
        push   eax
        push   esi
        call   sub_3_0008
loc_1_4A5F:
        add    esp, 0x14
        test   eax, eax
        je     loc_1_4A70
        mov    eax, 0x13
        jmp    loc_1_4F4B
loc_1_4A70:
        mov    eax, dword ptr [ebp - 0x24]
        cmp    eax, 0x10
        je     loc_1_4A86
        cmp    eax, 0x40
        je     loc_1_4A86
        and    byte ptr [ebp - 0x210], 0xfd
        jmp    loc_1_4A8D
loc_1_4A86:
        or     byte ptr [ebp - 0x210], 2
loc_1_4A8D:
        push   0
        lea    eax, [ebp - 0x12c]
        push   esi
        push   eax
        call   sub_3_0040
loc_1_4A9C:
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_4F4B
        push   0
        lea    eax, [ebp - 0x204]
        push   0xc8
        push   eax
        push   esi
        call   sub_3_0038
loc_1_4ABB:
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_4F4B
        xor    dx, dx
        cmp    word ptr [ebp - 0xd0], dx
        jbe    loc_1_4B97
loc_1_4AD6:
        movzx  eax, dx
        xor    ecx, ecx
        lea    edi, [eax*2]
        mov    ax, word ptr [ebp + edi - 0xce]
        mov    cx, ax
        and    ecx, 0xf00
        cmp    ecx, 0x100
        je     loc_1_4B15
        cmp    ecx, 0x200
        je     loc_1_4B15
        cmp    ecx, 0x300
        je     loc_1_4B49
        mov    eax, 0x13
        jmp    loc_1_4F4B
loc_1_4B15:
        cmp    word ptr [ebp + edi - 0xa6], 0x10
        jb     loc_1_4BBB
        or     byte ptr [ebp - 0x20c], 1
        mov    word ptr [ebp - 0x13b], ax
        or     byte ptr [ebp - 0x20c], 0x80
        mov    word ptr [ebp - 0x137], ax
        or     byte ptr [ebp - 0x20c], 4
        jmp    loc_1_4B88
loc_1_4B49:
        mov    cx, ax
        and    cl, 0xf0
        cmp    cl, 0x70
        jbe    loc_1_4B6F
        cmp    word ptr [ebp + edi - 0xa6], 2
        jb     loc_1_4B88
        or     byte ptr [ebp - 0x20c], 4
        mov    word ptr [ebp - 0x137], ax
        jmp    loc_1_4B88
loc_1_4B6F:
        cmp    word ptr [ebp + edi - 0xa6], 2
        jb     loc_1_4B88
        or     byte ptr [ebp - 0x20c], 2
        mov    word ptr [ebp - 0x139], ax
loc_1_4B88:
        inc    dx
        cmp    dx, word ptr [ebp - 0xd0]
        jb     loc_1_4AD6
loc_1_4B97:
        xor    ax, ax
        cmp    word ptr [ebp - 0x6e], ax
        jbe    loc_1_4BDA
loc_1_4BA0:
        movzx  ecx, ax
        mov    cl, byte ptr [ebp + ecx - 0x6c]
        cmp    cl, 4
        jae    loc_1_4BC5
        or     byte ptr [ebp - 0x20c], 0x10
        mov    byte ptr [ebp - 0x133], cl
        jmp    loc_1_4BD2
loc_1_4BBB:
        mov    eax, 0x13
        jmp    loc_1_4F4B
loc_1_4BC5:
        or     byte ptr [ebp - 0x20c], 0x20
        mov    byte ptr [ebp - 0x132], cl
loc_1_4BD2:
        inc    ax
        cmp    ax, word ptr [ebp - 0x6e]
        jb     loc_1_4BA0
loc_1_4BDA:
        xor    ax, ax
        test   byte ptr [ebp - 0x20c], 1
        je     loc_1_4BF8
        push   dword ptr [ebp - 0x13b]
        call   sub_1_3B3A
        and    ax, 0xff
        add    esp, 4
loc_1_4BF8:
        cmp    ax, 5
        jne    loc_1_4C35
        test   byte ptr [ebp - 0x210], 2
        jne    loc_1_4C35
        cmp    byte ptr [ebp - 0x133], 0
        je     loc_1_4CCB
        cmp    byte ptr [ebp - 0x132], 6
        je     loc_1_4CCB
        push   dword ptr [ebp - 0x139]
        push   dword ptr [ebp - 0x13b]
        call   sub_1_3D08
        add    esp, 8
loc_1_4C35:
        cmp    word ptr [ebp - 0x7e], 0
        jbe    loc_1_4EC6
        cmp    byte ptr [ebp - 0x7c], 2
        jne    loc_1_4C4A
        mov    byte ptr [ebp - 0x7c], 9
loc_1_4C4A:
        mov    al, byte ptr [ebp - 0x7c]
        push   1
        push   0x100
        lea    ecx, [ebp - 0x310]
        push   ecx
        mov    byte ptr [ebp - 0x131], al
        or     byte ptr [ebp - 0x20c], 0x40
        push   0
        push   esi
        call   sub_3_0030
loc_1_4C70:
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_4CAE
        lea    eax, [ebp - 0x28]
        lea    ecx, [ebp - 0x310]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_3_0028
loc_1_4C8C:
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_4CAE
        push   0                             ; -> loc_1_0138
        push   dword ptr [ebp - 0x28]
        call   sub_3_0020
loc_1_4CA0:
        add    esp, 8
        push   dword ptr [ebp - 0x28]
        call   sub_3_0018
loc_1_4CAB:
        add    esp, 4
loc_1_4CAE:
        lea    eax, [ebp - 0x210]
        push   eax
        call   sub_1_34BD
        add    esp, 4
        test   eax, eax
        je     loc_1_4CD5
        mov    eax, 0x24
        jmp    loc_1_4F4B
loc_1_4CCB:
        mov    eax, 0x13
        jmp    loc_1_4F4B
loc_1_4CD5:
        push   dword ptr [ebp - 0x13b]
        call   sub_1_39C0
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_1_4CF3
        mov    eax, 0x24
        jmp    loc_1_4F4B
loc_1_4CF3:
        mov    byte ptr [ebp - 2], 1
        test   byte ptr [ebp - 0x210], 2
        mov    byte ptr [ebp - 1], 1
        jne    loc_1_4D08
        mov    byte ptr [ebp - 1], 0
loc_1_4D08:
        push   1
        lea    eax, [ebp - 2]
        push   2
        push   eax
        push   3
        push   0                             ; -> obj1:0x017C
        push   0                             ; -> loc_1_0138
        push   esi
        call   sub_3_0000
loc_1_4D22:
        add    esp, 0x1c
        lea    ecx, [ebp - 0x14]
        push   ecx
        call   sub_1_385B
        add    esp, 4
        test   eax, eax
        je     loc_1_4D3F
        mov    eax, 0x13
        jmp    loc_1_4F4B
loc_1_4D3F:
        call   sub_1_9C82
        push   dword ptr [ebp - 0x14]
        call   sub_1_5368
        add    esp, 4
        lea    ecx, [ebp - 0x210]
        push   dword ptr [ebp - 0x14]
        push   ecx
        call   sub_1_5496
        add    esp, 8
        test   eax, eax
        je     loc_1_4D85
        push   dword ptr [ebp - 0x14]
        call   sub_1_59AD
        add    esp, 4
        push   dword ptr [ebp - 0x14]
        call   sub_1_38E3
        add    esp, 4
        mov    eax, 0x13
        jmp    loc_1_4F4B
loc_1_4D85:
        lea    eax, [ebp - 0x210]
        push   eax
        call   sub_1_508A
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x208]
        mov    dword ptr [ebp - 0x4c], 1
        mov    dword ptr [ebp - 0x48], 0     ; -> loc_1_016C
        mov    dword ptr [ebp - 0x44], eax
        push   dword ptr [ebp - 0x4c]
        push   dword ptr [ebp - 0x48]
        push   dword ptr [ebp - 0x44]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0001h              ; cd2001004a04
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x20c]
        mov    ecx, dword ptr [ebp - 0x208]
        and    eax, 2
        mov    dword ptr [ebp - 0x38], ecx
        mov    dword ptr [ebp - 0x3c], 0     ; -> obj1:0x0158
        shr    eax, 1
        mov    dword ptr [ebp - 0x40], eax
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        push   dword ptr [ebp - 0x38]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0001h              ; cd2001004a04
        add    esp, 0xc
        test   byte ptr [ebp - 0x210], 2
        jne    loc_1_4E27
        lea    eax, [ebp - 0x210]
        push   eax
        call   sub_1_3750
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_4E27
        push   1
        push   0                             ; -> loc_1_4F52
        push   esi
        call   sub_3_0010
loc_1_4E24:
        add    esp, 0xc
loc_1_4E27:
        mov    word ptr [ebp - 4], 1
        mov    dword ptr [ebp - 0x34], 4
        test   byte ptr [ebp - 0x210], 2
        mov    word ptr [ebp - 6], 0
        jne    loc_1_4E70
        mov    word ptr [ebp - 4], 1
        test   edi, edi
        je     loc_1_4E53
        mov    word ptr [ebp - 4], 0
loc_1_4E53:
        push   1
        lea    eax, [ebp - 4]
        push   2
        push   eax
        push   3
        push   0                             ; -> loc_1_0064
        push   0                             ; -> loc_1_0138
        push   esi
        call   sub_3_0000
loc_1_4E6D:
        add    esp, 0x1c
loc_1_4E70:
        push   0
        lea    eax, [ebp - 0x30]
        push   eax
        lea    ecx, [ebp - 0x34]
        push   ecx
        lea    edx, [ebp - 0x24]
        push   edx
        push   esi
        call   sub_3_0008
loc_1_4E84:
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_4EB1
        movzx  ax, byte ptr [ebp - 0x30]
        mov    word ptr [ebp - 6], ax
        push   1
        push   2
        lea    eax, [ebp - 6]
        push   eax
        push   3
        push   0                             ; -> obj1:0x0154
        push   0                             ; -> loc_1_0138
        push   esi
        call   sub_3_0000
loc_1_4EAE:
        add    esp, 0x1c
loc_1_4EB1:
        push   dword ptr [ebp - 0x208]
        call   sub_4_0F1C
loc_1_4EBC:
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_4EC6:
        mov    eax, 0x13
        jmp    loc_1_4F4B
loc_1_4ECD:
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_4_1065
loc_1_4ED6:
        add    esp, 4
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x20]
        push   eax
        push   ecx
        push   esi
        push   4
        call   sub_1_2E0C
        add    esp, 0x10
        test   eax, eax
        je     loc_1_4EF4
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_4EF4:
        push   dword ptr [ebp - 0x20]
        call   sub_1_59AD
        add    esp, 4
        push   dword ptr [ebp - 0x20]
        call   sub_1_38E3
        mov    dword ptr [ebp - 0x2c], 0     ; -> loc_1_014C
        add    esp, 4
        push   dword ptr [ebp - 0x2c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0004h              ; cd2004004a04
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_4F4B
loc_1_4F27:
        cmp    dword ptr [ebp + 0xc], 1
        mov    eax, 1
        ja     loc_1_4F4B
        push   dword ptr [ebp + 0x10]
        call   sub_1_380B
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
loc_1_4F4B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_4F52:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        cmp    dword ptr [ebp + 8], 5
        push   ebx
        push   esi
        push   edi
        je     loc_1_4F6B
        mov    eax, 1
        jmp    loc_1_5083
loc_1_4F6B:
        push   0
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_3_0078
loc_1_4F7A:
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_4F8B
        mov    eax, 1
        jmp    loc_1_5083
loc_1_4F8B:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x28]
        push   eax
        push   ecx
        push   esi
        push   4
        call   sub_1_2E0C
        add    esp, 0x10
        test   eax, eax
        je     loc_1_4FAC
        mov    eax, 0x13
        jmp    loc_1_5083
loc_1_4FAC:
        lea    eax, [ebp - 2]
        push   eax
        push   dword ptr [ebp - 0x28]
        call   sub_1_3999
        add    esp, 8
        test   eax, eax
        je     loc_1_4FC9
        mov    eax, 0x13
        jmp    loc_1_5083
loc_1_4FC9:
        push   0
        lea    eax, [ebp - 8]
        push   esi
        push   0                             ; -> loc_1_0188
        push   eax
        call   sub_3_0070
loc_1_4FDA:
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_5083
        push   0
        lea    ecx, [ebp - 0x24]
        push   0x3000
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_3_0068
loc_1_4FF8:
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_5083
        xor    di, di
        xor    ebx, ebx
        mov    si, 0x400
loc_1_500C:
        mov    ax, word ptr [ebp - 2]
        mov    byte ptr [ebp - 0x16], bl
        add    ax, si
        mov    byte ptr [ebp - 0x15], bl
        mov    byte ptr [ebp - 0xa], 0xff
        mov    byte ptr [ebp - 9], 0xf
        mov    word ptr [ebp - 0x1c], bx
        push   ebx
        mov    word ptr [ebp - 0x1a], bx
        push   0x18
        mov    word ptr [ebp - 0x18], bx
        lea    ecx, [ebp - 0x20]
        mov    word ptr [ebp - 0x10], ax
        push   ecx
        add    ax, 3
        push   2
        mov    word ptr [ebp - 0x20], 1
        push   dword ptr [ebp - 0x24]
        mov    word ptr [ebp - 0x1e], 0xc
        mov    word ptr [ebp - 0x14], 0xffff
        mov    word ptr [ebp - 0x12], 4
        mov    word ptr [ebp - 0xe], ax
        lea    eax, [ebp - 0x2c]
        mov    word ptr [ebp - 0xc], bx
        push   eax
        call   sub_3_0060
loc_1_506A:
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_5083
        movzx  esi, si
        add    si, 0x400
        inc    di
        cmp    di, 3
        jb     loc_1_500C
        xor    eax, eax
loc_1_5083:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_508A:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x94
        mov    eax, dword ptr [edx + 4]
        push   ebx
        test   al, 0x20
        je     loc_1_50A7
        mov    bl, byte ptr [edx + 0xde]
        jmp    loc_1_50AD
loc_1_50A7:
        mov    bl, byte ptr [edx + 0xdd]
loc_1_50AD:
        mov    cl, byte ptr [edx + 0xdf]
        cmp    cl, 9
        jne    loc_1_50BF
        mov    ecx, 2
        jmp    loc_1_50C2
loc_1_50BF:
        movzx  ecx, cl
loc_1_50C2:
        test   al, 2
        je     loc_1_50F6
        movzx  eax, word ptr [edx + 0xd7]
        movzx  ebx, bl
        push   eax
        push   ebx
        movzx  eax, byte ptr [edx + 0xdd]
        movzx  edx, word ptr [edx + 0xd5]
        push   eax
        push   ecx
        push   edx
        lea    eax, [ebp - 0x54]
        push   0                             ; -> loc_1_01B4
        push   eax
        call   sub_3_0080
loc_1_50F1:
        add    esp, 0x1c
        jmp    loc_1_511C
loc_1_50F6:
        movzx  eax, bl
        movzx  ebx, byte ptr [edx + 0xdd]
        push   eax
        push   ebx
        movzx  eax, word ptr [edx + 0xd5]
        push   ecx
        lea    edx, [ebp - 0x54]
        push   eax
        push   0                             ; -> loc_1_019C
        push   edx
        call   sub_3_0080
loc_1_5119:
        add    esp, 0x18
loc_1_511C:
        mov    dword ptr [ebp - 0x14], 0x40
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_014C
        lea    eax, [ebp - 0x94]
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0003h              ; cd2003004a04
        add    esp, 0xc
        lea    eax, [ebp - 0x94]
        push   eax
        push   0x45
        call   sub_1_51D7
        add    esp, 8
        test   eax, eax
        jne    loc_1_51B0
        xor    eax, eax
        xor    ecx, ecx
        cmp    byte ptr [ebp - 0x54], al
        je     loc_1_5176
loc_1_5169:
        cmp    eax, 0x3f
        jge    loc_1_51B0
        inc    eax
        cmp    byte ptr [ebp + eax - 0x54], 0
        jne    loc_1_5169
loc_1_5176:
        cmp    eax, 0x3f
        jge    loc_1_51B0
        mov    byte ptr [ebp + eax - 0x54], 0x20
        inc    eax
        cmp    byte ptr [ebp - 0x94], 0
        je     loc_1_51AB
loc_1_518A:
        mov    dl, byte ptr [ebp + ecx - 0x94]
        cmp    dl, 0x20
        je     loc_1_51AB
        cmp    eax, 0x3f
        jge    loc_1_51AB
        mov    byte ptr [ebp + eax - 0x54], dl
        inc    ecx
        inc    eax
        cmp    byte ptr [ebp + ecx - 0x94], 0
        jne    loc_1_518A
loc_1_51AB:
        mov    byte ptr [ebp + eax - 0x54], 0
loc_1_51B0:
        mov    dword ptr [ebp - 4], 0        ; -> loc_1_014C
        lea    eax, [ebp - 0x54]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_51D7:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    al, byte ptr [ecx]
        test   al, al
        je     loc_1_5227
        xor    esi, esi
        cmp    byte ptr [ecx + esi], 0
        je     loc_1_51F8
loc_1_51EB:
        cmp    byte ptr [ecx + esi], 0x20
        jne    loc_1_51F8
        inc    esi
        cmp    byte ptr [ecx + esi], 0
        jne    loc_1_51EB
loc_1_51F8:
        mov    dl, byte ptr [ecx + esi]
        cmp    byte ptr [esp + 0xc], dl
        je     loc_1_522F
        movsx  eax, dl
        movsx  edi, byte ptr [esp + 0xc]
        sub    eax, edi
        cmp    eax, 0x20
        je     loc_1_522F
        test   dl, dl
        je     loc_1_5221
loc_1_5214:
        cmp    byte ptr [ecx + esi], 0x20
        je     loc_1_5221
        inc    esi
        cmp    byte ptr [ecx + esi], 0
        jne    loc_1_5214
loc_1_5221:
        cmp    byte ptr [ecx + esi], 0
        jne    loc_1_51EB
loc_1_5227:
        mov    eax, 1
loc_1_522C:
        pop    edi
        pop    esi
        ret    
loc_1_522F:
        mov    edi, esi
        mov    dl, byte ptr [ecx + esi]
        test   dl, dl
        je     loc_1_5245
loc_1_5238:
        cmp    byte ptr [ecx + edi], 0x20
        je     loc_1_5245
        inc    edi
        cmp    byte ptr [ecx + edi], 0
        jne    loc_1_5238
loc_1_5245:
        xor    eax, eax
        test   dl, dl
        je     loc_1_525D
loc_1_524B:
        cmp    esi, edi
        je     loc_1_525D
        mov    dl, byte ptr [ecx + esi]
        inc    esi
        mov    byte ptr [ecx + eax], dl
        inc    eax
        cmp    byte ptr [ecx + esi], 0
        jne    loc_1_524B
loc_1_525D:
        mov    byte ptr [ecx + eax], 0
        xor    eax, eax
        jmp    loc_1_522C
sub_1_5265:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_1_5272:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    ebx, 0x40
        jbe    loc_1_528D
        mov    eax, 1
        jmp    loc_1_5334
loc_1_528D:
        add    ebx, 3
        mov    esi, dword ptr [ebp + 8]
        shr    ebx, 2
        test   ebx, ebx
        je     loc_1_52EB
        lea    edi, [esi + 8]
loc_1_529D:
        mov    word ptr [ebp - 4], bx
        push   ax
        push   cx
        mov    ax, word ptr [ebp - 4]
        dec    ax
        bsr    cx, ax
        inc    cl
        mov    ax, 1
        shl    ax, cl
        dec    ax
        and    ax, 0x1f
        mov    word ptr [ebp - 2], ax
        pop    cx
        pop    ax
        movzx  ecx, word ptr [ebp - 2]
        lea    eax, [esi + 0x14]
        push   eax
        push   edi
        lea    eax, [esi + 0xc]
        push   eax
        lea    eax, [esi + 0x10]
        push   eax
        push   ebx
        push   ecx
        call   sub_1_9440
        add    esp, 0x18
        test   eax, eax
        je     loc_1_52EB
        shr    ebx, 1
        test   ebx, ebx
        jne    loc_1_529D
loc_1_52EB:
        test   ebx, ebx
        jne    loc_1_52F6
        mov    eax, 1
        jmp    loc_1_5334
loc_1_52F6:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 0x14]
        call   sub_1_94B8
        add    esp, 0x10
        lea    ecx, [esi + 4]
        push   ecx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        call   sub_1_93F9
        add    esp, 0xc
        test   eax, eax
        je     loc_1_5329
        mov    eax, 1
        jmp    loc_1_5334
loc_1_5329:
        xor    eax, eax
        or     byte ptr [esi], 1
        mov    word ptr [esi + 6], 0
loc_1_5334:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_533B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi], 1
        jne    loc_1_534C
        mov    eax, 3
        jmp    loc_1_5366
loc_1_534C:
        mov    ax, word ptr [esi + 4]
        push   eax
        call   sub_1_942B
        add    esp, 4
        push   dword ptr [esi + 0x10]
        call   sub_1_94A4
        add    esp, 4
        xor    eax, eax
loc_1_5366:
        pop    esi
        ret    
sub_1_5368:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        xor    edi, edi
        lea    eax, [esi + 0x67d]
        push   eax
        mov    dword ptr [esi + 0xd4], edi
        call   sub_1_443E
        add    esp, 4
        lea    eax, [esi + 0x6a7]
        push   eax
        call   sub_1_216C
        add    esp, 4
loc_1_5395:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0xc
        add    eax, esi
        add    eax, 0x6b3
        push   eax
        call   sub_1_2261
        add    esp, 4
        cmp    di, 2
        jb     loc_1_5395
        xor    di, di
        lea    eax, [esi + 0x6fb]
        push   eax
        mov    ebx, 0xec
        call   sub_1_5265
        add    esp, 4
loc_1_53CA:
        movzx  eax, di
        imul   eax, ebx
        inc    di
        add    eax, esi
        add    eax, ebx
        push   eax
        call   sub_1_9EC9
        add    esp, 4
        cmp    di, 2
        jb     loc_1_53CA
        xor    di, di
loc_1_53E8:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0x2a
        add    eax, esi
        add    eax, 0x2c4
        push   eax
        call   sub_4_1944
loc_1_53FD:
        add    esp, 4
        cmp    di, 1
        jb     loc_1_53E8
        xor    di, di
loc_1_5409:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0xe0
        add    eax, esi
        add    eax, 0x2ee
        push   eax
        call   sub_4_170A
loc_1_5421:
        add    esp, 4
        cmp    di, 1
        jb     loc_1_5409
        xor    di, di
loc_1_542D:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0x5a
        add    eax, esi
        add    eax, 0x3ce
        push   eax
        call   sub_4_14A4
loc_1_5442:
        add    esp, 4
        cmp    di, 1
        jb     loc_1_542D
        xor    di, di
loc_1_544E:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0x229
        add    eax, esi
        add    eax, 0x428
        push   eax
        call   sub_1_639C
        add    esp, 4
        cmp    di, 1
        jb     loc_1_544E
        xor    di, di
loc_1_5472:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0x2c
        add    eax, esi
        add    eax, 0x651
        push   eax
        call   sub_1_8EA8
        add    esp, 4
        cmp    di, 1
        jb     loc_1_5472
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_5496:
        sub    esp, 8
        mov    dword ptr [esp + 4], 1
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        push   edi
        mov    edi, dword ptr [esp + 0x1c]
        push   ebp
        mov    eax, dword ptr [esi + 8]
        lea    ecx, [esi + 0xc]
        push   ecx
        lea    edx, [edi + 0xc]
        push   edx
        mov    dword ptr [edi + 8], eax
        lea    ebp, [edi + 0xd4]
        call   sub_1_23D2
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [edi + 0xd8], eax
        mov    dword ptr [edi + 0xdc], eax
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [edi + 0xe0], eax
        test   byte ptr [esi], 2
        je     loc_1_54ED
        or     byte ptr [edi], 0x20
        jmp    loc_1_54F0
loc_1_54ED:
        and    byte ptr [edi], 0xdf
loc_1_54F0:
        xor    eax, eax
        mov    dword ptr [edi + 0x713], eax
        mov    dword ptr [edi + 0x717], eax
        mov    dword ptr [edi + 0x71b], eax
        test   byte ptr [ebp], 1
        je     loc_1_5533
        mov    cx, word ptr [esi + 0xd5]
        lea    eax, [edi + 0x67d]
        push   0                             ; -> loc_1_9BA8
        push   0                             ; -> loc_1_9B80
        push   ecx
        push   eax
        call   sub_1_4457
        add    esp, 0x10
        test   eax, eax
        je     loc_1_5533
        and    byte ptr [ebp], 0xfe
loc_1_5533:
        test   byte ptr [ebp], 2
        je     loc_1_555D
        mov    cx, word ptr [esi + 0xd7]
        lea    eax, [edi + 0x67d]
        push   0                             ; -> loc_1_9B80
        push   ecx
        push   eax
        call   sub_1_4576
        add    esp, 0xc
        test   eax, eax
        je     loc_1_555D
        and    byte ptr [ebp], 0xfd
loc_1_555D:
        test   byte ptr [ebp], 4
        je     loc_1_5587
        mov    cx, word ptr [esi + 0xd9]
        lea    eax, [edi + 0x67d]
        push   0                             ; -> loc_1_9BA8
        push   ecx
        push   eax
        call   sub_1_45CB
        add    esp, 0xc
        test   eax, eax
        je     loc_1_5587
        and    byte ptr [ebp], 0xfb
loc_1_5587:
        lea    ebx, [edi + 0x67d]
        lea    eax, [edi + 0x7a5]
        push   eax
        push   ebx
        call   sub_1_4825
        add    esp, 8
        test   byte ptr [ebp], 0x40
        je     loc_1_55D1
        push   edi
        mov    al, byte ptr [esi + 0xdf]
        push   0                             ; -> loc_1_9970
        lea    ecx, [edi + 0x6a7]
        push   0                             ; -> loc_1_9958
        push   0                             ; -> loc_1_99EC
        push   eax
        push   ecx
        call   sub_1_2184
        add    esp, 0x18
        test   eax, eax
        je     loc_1_55D1
        and    byte ptr [ebp], 0xbf
loc_1_55D1:
        test   byte ptr [ebp], 0x10
        je     loc_1_5634
        push   0                             ; -> loc_1_9980
        mov    al, byte ptr [esi + 0xdd]
        push   eax
        lea    eax, [edi + 0x6b3]
        push   eax
        call   sub_1_227A
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        lea    eax, [edi + 0x6cb]
        push   0x20
        push   eax
        call   sub_1_5272
        add    esp, 8
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_5612
        test   eax, eax
        je     loc_1_5634
loc_1_5612:
        lea    eax, [edi + 0x6b3]
        push   eax
        call   sub_1_237D
        add    esp, 4
        lea    eax, [edi + 0x6cb]
        push   eax
        call   sub_1_533B
        add    esp, 4
        and    byte ptr [ebp], 0xef
loc_1_5634:
        test   byte ptr [ebp], 0x20
        je     loc_1_5691
        push   0                             ; -> loc_1_9980
        mov    al, byte ptr [esi + 0xde]
        push   eax
        lea    esi, [edi + 0x6e3]
        lea    eax, [edi + 0x6bf]
        push   eax
        call   sub_1_227A
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        push   0x20
        push   esi
        call   sub_1_5272
        add    esp, 8
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_5675
        test   eax, eax
        je     loc_1_5691
loc_1_5675:
        lea    eax, [edi + 0x6bf]
        push   eax
        call   sub_1_237D
        add    esp, 4
        push   esi
        call   sub_1_533B
        add    esp, 4
        and    byte ptr [ebp], 0xdf
loc_1_5691:
        mov    eax, dword ptr [ebp]
        test   al, 0x10
        je     loc_1_56A4
        test   al, 0x20
        je     loc_1_56A4
        mov    dword ptr [esp + 0x14], 0
loc_1_56A4:
        test   al, 0x80
        je     loc_1_56C8
        lea    eax, [edi + 0x6fb]
        lea    ecx, [edi + 0x651]
        push   eax
        push   ebx
        push   ecx
        push   edi
        call   sub_1_8EB5
        add    esp, 0x10
        test   eax, eax
        je     loc_1_56C8
        and    byte ptr [ebp], 0x7f
loc_1_56C8:
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     loc_1_56FA
        test   al, 0x10
        je     loc_1_56FA
        test   al, 0x40
        je     loc_1_56FA
        lea    ecx, [edi + 0x6cb]
        lea    edx, [edi + 0x6b3]
        push   dword ptr [esp + 0x14]
        push   ecx
        push   edx
        lea    eax, [edi + 0xec]
        push   ebx
        push   eax
        push   edi
        call   sub_1_9ED7
        add    esp, 0x18
loc_1_56FA:
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     loc_1_572C
        test   al, 0x20
        je     loc_1_572C
        test   al, 0x40
        je     loc_1_572C
        lea    ecx, [edi + 0x6e3]
        lea    edx, [edi + 0x6bf]
        push   dword ptr [esp + 0x14]
        push   ecx
        push   edx
        lea    eax, [edi + 0x1d8]
        push   ebx
        push   eax
        push   edi
        call   sub_1_9ED7
        add    esp, 0x18
loc_1_572C:
        test   byte ptr [ebp], 1
        je     loc_1_5743
        push   ebx
        lea    eax, [edi + 0x2c4]
        push   eax
        push   edi
        call   sub_4_1951
loc_1_5740:
        add    esp, 0xc
loc_1_5743:
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     loc_1_5763
        test   al, 0x40
        je     loc_1_5763
        test   al, 2
        je     loc_1_5763
        push   ebx
        lea    eax, [edi + 0x2ee]
        push   eax
        push   edi
        call   sub_4_1717
loc_1_5760:
        add    esp, 0xc
loc_1_5763:
        test   byte ptr [ebp], 2
        je     loc_1_577A
        push   ebx
        lea    eax, [edi + 0x3ce]
        push   eax
        push   edi
        call   sub_4_14B1
loc_1_5777:
        add    esp, 0xc
loc_1_577A:
        test   byte ptr [ebp], 1
        je     loc_1_5791
        push   ebx
        lea    eax, [edi + 0x428]
        push   eax
        push   edi
        call   sub_1_63B7
        add    esp, 0xc
loc_1_5791:
        lea    ebp, [edi + 0x681]
        mov    ax, word ptr [ebp]
        push   eax
        call   sub_1_3B3A
        mov    word ptr [esp + 0x14], ax
        add    esp, 4
        shr    ax, 8
        push   eax
        push   dword ptr [esp + 0x14]
        call   sub_1_3D74
        lea    ecx, [esp + 0x18]
        add    esp, 8
        mov    esi, 0                        ; -> loc_1_0138
        mov    dword ptr [edi + 0xe4], eax
        push   1
        push   2
        push   ecx
        push   3
        push   0                             ; -> obj1:0x01DC
        push   esi
        push   dword ptr [edi + 8]
        call   sub_3_0000
loc_1_57DE:
        mov    ax, word ptr [ebp]
        add    esp, 0x1c
        mov    ebp, 0xffff
        push   eax
        call   sub_1_3DF4
        add    esp, 4
        mov    dword ptr [edi + 0xe8], eax
        push   ebp
        push   0xe1
        push   ebx
        call   sub_1_436F
        lea    eax, [esp + 0x1d]
        add    esp, 0xc
        push   ebp
        push   eax
        push   ebx
        call   sub_1_43EF
        lea    ecx, [esp + 0x1c]
        add    esp, 0xc
        push   ebp
        push   ecx
        push   ebx
        call   sub_1_43EF
        mov    cx, word ptr [esp + 0x1c]
        add    esp, 0xc
        lea    eax, [edi + 0x79f]
        push   1
        mov    word ptr [eax], cx
        push   2
        push   eax
        push   3
        push   0                             ; -> obj1:0x01D0
        push   esi
        push   dword ptr [edi + 8]
        call   sub_3_0000
loc_1_5849:
        add    esp, 0x1c
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_5856:
        push   ebp
        movzx  eax, word ptr [esp + 0xc]
        mov    ebp, esp
        cmp    eax, 1
        je     loc_1_589C
        cmp    eax, 2
        je     loc_1_58E2
        cmp    eax, 4
        je     loc_1_58FF
        cmp    eax, 8
        je     loc_1_5913
        cmp    eax, 0x10
        je     loc_1_5927
        cmp    eax, 0x20
        je     loc_1_593B
        cmp    eax, 0x80
        je     loc_1_5954
        jmp    loc_1_5976
loc_1_589C:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        add    edx, 0xd4
        mov    eax, dword ptr [edx]
        test   al, 1
        je     loc_1_58C0
        test   al, 0x10
        je     loc_1_58C0
        test   al, 0x40
        je     loc_1_58C0
        mov    word ptr [ecx], 1
loc_1_58C0:
        mov    eax, dword ptr [edx]
        test   al, 1
        je     loc_1_5976
        test   al, 0x20
        je     loc_1_5976
        test   al, 0x40
        je     loc_1_5976
        inc    word ptr [ecx]
        jmp    loc_1_5976
loc_1_58E2:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        mov    eax, dword ptr [edx + 0xd4]
        test   al, 1
        je     loc_1_5976
        test   al, 0x40
        je     loc_1_5976
        test   al, 2
        jmp    loc_1_596F
loc_1_58FF:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        test   byte ptr [edx + 0xd4], 2
        jmp    loc_1_596F
loc_1_5913:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        test   byte ptr [edx + 0xd4], 1
        jmp    loc_1_596F
loc_1_5927:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        test   byte ptr [edx + 0xd4], 1
        jmp    loc_1_596F
loc_1_593B:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        mov    eax, dword ptr [edx + 0xd4]
        test   al, 1
        je     loc_1_5976
        test   al, 8
        jmp    loc_1_596F
loc_1_5954:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        mov    eax, dword ptr [edx + 0xd4]
        test   al, 1
        je     loc_1_5976
        test   al, 0x10
        je     loc_1_5976
        test   al, 0x40
loc_1_596F:
        je     loc_1_5976
        mov    word ptr [ecx], 1
loc_1_5976:
        xor    eax, eax
        pop    ebp
        ret    
sub_1_597A:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0xc]
        add    edi, 0xec
loc_1_5988:
        push   edi
        call   sub_1_9DDD
        add    esp, 4
        test   eax, eax
        jne    loc_1_59A6
        add    edi, 0xec
        inc    esi
        cmp    esi, 2
        jl     loc_1_5988
        xor    eax, eax
loc_1_59A3:
        pop    edi
        pop    esi
        ret    
loc_1_59A6:
        mov    eax, 1
        jmp    loc_1_59A3
sub_1_59AD:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        lea    edi, [esi + 0xd4]
        test   byte ptr [edi], 1
        je     loc_1_59CE
        lea    eax, [esi + 0x67d]
        push   eax
        call   sub_1_472E
        add    esp, 4
loc_1_59CE:
        test   byte ptr [edi], 2
        je     loc_1_59E2
        lea    eax, [esi + 0x67d]
        push   eax
        call   sub_1_47B3
        add    esp, 4
loc_1_59E2:
        test   byte ptr [edi], 4
        je     loc_1_59F6
        lea    eax, [esi + 0x67d]
        push   eax
        call   sub_1_47DD
        add    esp, 4
loc_1_59F6:
        xor    di, di
        lea    eax, [esi + 0x6a7]
        push   eax
        call   sub_1_2238
        add    esp, 4
loc_1_5A08:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0xc
        add    eax, esi
        add    eax, 0x6b3
        push   eax
        call   sub_1_237D
        add    esp, 4
        cmp    di, 2
        jb     loc_1_5A08
        xor    di, di
loc_1_5A29:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0x18
        add    eax, esi
        add    eax, 0x6cb
        push   eax
        call   sub_1_533B
        add    esp, 4
        cmp    di, 2
        jb     loc_1_5A29
        xor    di, di
        lea    eax, [esi + 0x6fb]
        push   eax
        mov    ebx, 0xec
        call   sub_1_533B
        add    esp, 4
loc_1_5A5E:
        movzx  eax, di
        imul   eax, ebx
        inc    di
        add    eax, esi
        add    eax, ebx
        push   eax
        call   sub_1_A033
        add    esp, 4
        cmp    di, 2
        jb     loc_1_5A5E
        xor    di, di
loc_1_5A7C:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0x2a
        add    eax, esi
        add    eax, 0x2c4
        push   eax
        call   sub_4_1980
loc_1_5A91:
        add    esp, 4
        cmp    di, 1
        jb     loc_1_5A7C
        xor    di, di
loc_1_5A9D:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0xe0
        add    eax, esi
        add    eax, 0x2ee
        push   eax
        call   sub_4_17AF
loc_1_5AB5:
        add    esp, 4
        cmp    di, 1
        jb     loc_1_5A9D
        xor    di, di
loc_1_5AC1:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0x5a
        add    eax, esi
        add    eax, 0x3ce
        push   eax
        call   sub_4_1520
loc_1_5AD6:
        add    esp, 4
        cmp    di, 1
        jb     loc_1_5AC1
        xor    di, di
loc_1_5AE2:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0x229
        add    eax, esi
        add    eax, 0x428
        push   eax
        call   sub_1_660A
        add    esp, 4
        cmp    di, 1
        jb     loc_1_5AE2
        xor    di, di
loc_1_5B06:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0x2c
        add    eax, esi
        add    eax, 0x651
        push   eax
        call   sub_1_912F
        add    esp, 4
        cmp    di, 1
        jb     loc_1_5B06
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_5B2A:
        xor    dx, dx
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 8]
loc_1_5B34:
        movzx  eax, dx
        imul   eax, eax, 0xec
        lea    eax, [eax + ecx + 0xec]
        test   byte ptr [eax + 4], 1
        je     loc_1_5B57
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 1
        je     loc_1_5B62
        cmp    eax, 2
        je     loc_1_5B62
loc_1_5B57:
        inc    dx
        cmp    dx, 2
        jb     loc_1_5B34
        xor    eax, eax
loc_1_5B61:
        ret    
loc_1_5B62:
        mov    eax, 1
        jmp    loc_1_5B61
sub_1_5B69:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        add    esi, 0xe8
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x651]
        test   al, 1
        je     loc_1_5BB2
        test   al, 4
        je     loc_1_5BB2
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        mov    eax, dword ptr [esi]
        add    eax, 0x651
        push   eax
        call   sub_1_8E1E
        add    esp, 4
        push   dword ptr [esp + 4]
        call   sub_1_9B5B
        add    esp, 4
        jmp    loc_1_5BB7
loc_1_5BB2:
        mov    eax, 1
loc_1_5BB7:
        pop    esi
        add    esp, 4
        ret    
sub_1_5BBC:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        add    esi, 0xe8
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x651]
        test   al, 1
        je     loc_1_5C05
        test   al, 4
        je     loc_1_5C05
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        mov    eax, dword ptr [esi]
        add    eax, 0x651
        push   eax
        call   sub_1_8E3B
        add    esp, 4
        push   dword ptr [esp + 4]
        call   sub_1_9B5B
        add    esp, 4
        jmp    loc_1_5C0A
loc_1_5C05:
        mov    eax, 1
loc_1_5C0A:
        pop    esi
        add    esp, 4
        ret    
sub_1_5C0F:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        add    esi, 0xe8
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0x651]
        test   al, 1
        je     loc_1_5C58
        test   al, 4
        je     loc_1_5C58
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        mov    eax, dword ptr [esi]
        add    eax, 0x651
        push   eax
        call   sub_1_8E54
        add    esp, 4
        push   dword ptr [esp + 4]
        call   sub_1_9B5B
        add    esp, 4
        jmp    loc_1_5C5D
loc_1_5C58:
        mov    eax, 1
loc_1_5C5D:
        pop    esi
        add    esp, 4
        ret    
sub_1_5C62:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], 0
        mov    edi, dword ptr [esi + 0x24]
        add    edi, 0x2ee
loc_1_5C7E:
        test   byte ptr [edi], 8
        je     loc_1_5CEA
        lea    eax, [ebp - 1]
        mov    ecx, dword ptr [esi + 8]
        mov    dx, word ptr [ecx + 0x20]
        push   eax
        push   edx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 0x40
        je     loc_1_5D7F
        test   byte ptr [ebp - 1], 0x80
        jne    loc_1_5D63
        lea    eax, [ebp - 2]
        mov    ecx, dword ptr [edi + 8]
        mov    dx, word ptr [ecx + 0x1e]
        push   eax
        push   edx
        call   sub_1_9CE4
        add    esp, 8
        lea    edx, [edi + 0xd8]
        mov    ax, word ptr [edx]
        cmp    ax, 0x40
        jae    loc_1_5D63
        movzx  ebx, word ptr [edi + 0xda]
        movzx  eax, ax
        mov    cl, byte ptr [ebp - 2]
        add    ebx, eax
        mov    byte ptr [ebx + edi + 0x58], cl
        inc    word ptr [edx]
        jmp    loc_1_5D63
loc_1_5CEA:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        lea    eax, [ebp - 1]
        mov    ecx, dword ptr [esi + 8]
        push   eax
        mov    dx, word ptr [ecx + 0x20]
        push   edx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 0x40
        je     loc_1_5D96
        test   byte ptr [ebp - 1], 0x80
        jne    loc_1_5D58
        lea    eax, [ebp - 2]
        mov    ecx, dword ptr [edi + 8]
        mov    dx, word ptr [ecx + 0x1e]
        push   eax
        push   edx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [edi], 4
        je     loc_1_5D58
        lea    eax, [edi + 0xd8]
        mov    dx, word ptr [eax]
        cmp    dx, 0x40
        jae    loc_1_5D58
        movzx  ebx, word ptr [edi + 0xda]
        movzx  edx, dx
        mov    cl, byte ptr [ebp - 2]
        add    ebx, edx
        mov    byte ptr [ebx + edi + 0x58], cl
        inc    word ptr [eax]
loc_1_5D58:
        push   dword ptr [ebp - 8]
        call   sub_1_9B5B
        add    esp, 4
loc_1_5D63:
        inc    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], 0xffff
        jb     loc_1_5C7E
        mov    eax, 1
loc_1_5D78:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_5D7F:
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x1e]
        push   eax
        call   sub_1_9CF5
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_5D78
loc_1_5D96:
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 0x1e]
        push   eax
        call   sub_1_9CF5
        add    esp, 8
        push   dword ptr [ebp - 8]
        call   sub_1_9B5B
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_5D78
sub_1_5DB8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x24]
        mov    eax, dword ptr [ecx + 0x2ee]
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        ret    
sub_1_5DD0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x54]
        mov    eax, dword ptr [ecx + 0x3ce]
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        ret    
sub_1_5DE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [edi + 0xc]
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 1
        sbb    al, al
        mov    ecx, dword ptr [edi + 0x12]
        and    al, 4
        lea    ebx, [esi + 0xe4]
        add    al, 4
        mov    dword ptr [ebp - 0xc], ecx
        or     al, 0x51
        cmp    dword ptr [ebx], 0
        mov    byte ptr [ebp - 1], al
        jne    loc_1_5E31
        push   dword ptr [esi + 0xb4]
        push   dword ptr [esi + 0xdc]
        call   sub_1_233D
        add    esp, 8
loc_1_5E31:
        push   dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [esi + 0xe0]
        push   dword ptr [ebp - 1]
        push   dword ptr [ecx + 0x14]
        push   dword ptr [ecx + 0xc]
        push   dword ptr [esi + 0xb4]
        push   dword ptr [esi + 0xdc]
        call   sub_1_22DC
        add    esp, 0x18
        mov    ecx, dword ptr [edi + 0xe]
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebx], 0
        jne    loc_1_5E73
        movzx  eax, word ptr [edi + 8]
        push   eax
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_1_A2BD
        add    esp, 0xc
loc_1_5E73:
        push   edi
        xor    edi, edi
        push   dword ptr [ebp - 4]
        push   edi
        push   esi
        call   sub_1_A13B
        add    esp, 0x10
        cmp    dword ptr [ebx], edi
        je     loc_1_5E89
        mov    dword ptr [ebx], edi
loc_1_5E89:
        push   esi
        call   sub_1_5B69
        add    esp, 4
        push   dword ptr [esi + 0xb4]
        push   dword ptr [esi + 0xdc]
        call   sub_1_235D
        add    esp, 8
        push   esi
        call   sub_1_5BBC
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_5EB8:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    edi, dword ptr [esi + 0xe8]
        cmp    dword ptr [edi + 0xf4], 0
        je     loc_1_5EF7
        cmp    dword ptr [edi + 0x1e0], 0
        je     loc_1_5EF7
        push   dword ptr [esi + 0xb4]
        push   dword ptr [esi + 0xdc]
        call   sub_1_233D
        mov    dword ptr [esi + 0xe4], 1
        add    esp, 8
        jmp    loc_1_5F5B
loc_1_5EF7:
        push   esi
        call   sub_1_A1BE
        add    esp, 4
        push   esi
        call   sub_1_5C0F
        add    esp, 4
        xor    ebx, ebx
        push   dword ptr [esi + 0xdc]
        call   sub_1_2319
        add    esp, 4
        lea    esi, [edi + 0x1d0]
        cmp    dword ptr [esi], ebx
        je     loc_1_5F39
        push   dword ptr [edi + 0x1a0]
        push   dword ptr [edi + 0x1c8]
        call   sub_1_235D
        add    esp, 8
        mov    dword ptr [esi], ebx
loc_1_5F39:
        lea    esi, [edi + 0x2bc]
        xor    ebx, ebx
        cmp    dword ptr [esi], ebx
        je     loc_1_5F5B
        push   dword ptr [edi + 0x28c]
        push   dword ptr [edi + 0x2b4]
        call   sub_1_235D
        add    esp, 8
        mov    dword ptr [esi], ebx
loc_1_5F5B:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_5F61:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0xb4]
        push   dword ptr [eax + 0xdc]
        call   sub_1_233D
        add    esp, 8
        xor    eax, eax
        ret    
sub_1_5F7C:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0xb4]
        push   dword ptr [eax + 0xdc]
        call   sub_1_235D
        add    esp, 8
        xor    eax, eax
        ret    
sub_1_5F97:
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_1_A2BD
        add    esp, 0xc
        ret    
sub_1_5FAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebx + 4]
        test   al, 1
        jne    loc_1_5FC9
        mov    eax, 3
        jmp    loc_1_61A8
loc_1_5FC9:
        movzx  ecx, word ptr [ebp + 0xc]
        cmp    ecx, 0xa
        ja     loc_1_5FD9
        jmp    dword ptr [ecx*4]             ; -> obj1:0x61AF
loc_1_5FD9:
        mov    eax, 0xc
        jmp    loc_1_61A8
loc_1_5FE3:
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_1_61DB
        add    esp, 8
        jmp    loc_1_6155
loc_1_5FF4:
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_1_61F6
        add    esp, 8
        jmp    loc_1_6155
loc_1_6005:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_1_6217
        jmp    loc_1_61A5
loc_1_6016:
        test   al, 2
        jne    loc_1_6024
        mov    eax, 5
        jmp    loc_1_61A8
loc_1_6024:
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_1_62FB
        add    esp, 8
        jmp    loc_1_61A8
loc_1_6035:
        test   al, 2
        jne    loc_1_6043
        mov    eax, 5
        jmp    loc_1_61A8
loc_1_6043:
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_1_634F
        add    esp, 8
        jmp    loc_1_61A8
loc_1_6054:
        mov    edx, dword ptr [ebp + 0x14]
        test   al, 2
        jne    loc_1_6065
        mov    eax, 5
        jmp    loc_1_61A8
loc_1_6065:
        cmp    word ptr [edx + 0x16], 1
        jne    loc_1_6073
        xor    eax, eax
        jmp    loc_1_61A8
loc_1_6073:
        cmp    dword ptr [ebx + 8], 0
        je     loc_1_6083
        mov    eax, 1
        jmp    loc_1_61A8
loc_1_6083:
        lea    edi, [ebx + 0xc]
        mov    esi, edx
        mov    ecx, 6
        push   edx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [ebx + 8], 1
        push   ebx
        call   sub_1_5DE8
        add    esp, 8
        jmp    loc_1_6155
loc_1_60A5:
        test   al, 2
        jne    loc_1_60B3
        mov    eax, 5
        jmp    loc_1_61A8
loc_1_60B3:
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_1_636A
        add    esp, 8
        jmp    loc_1_6155
loc_1_60C4:
        test   al, 2
        jne    loc_1_60D2
        mov    eax, 5
        jmp    loc_1_61A8
loc_1_60D2:
        mov    eax, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [eax]
        cmp    eax, 1
        je     loc_1_60E8
        cmp    eax, 2
        je     loc_1_610E
        cmp    eax, 3
        je     loc_1_6134
        jmp    loc_1_6155
loc_1_60E8:
        cmp    dword ptr [ebx + 8], 1
        je     loc_1_60F8
        mov    eax, 1
        jmp    loc_1_61A8
loc_1_60F8:
        push   ebx
        call   sub_1_5F61
        add    esp, 4
        test   eax, eax
        jne    loc_1_6155
        mov    dword ptr [ebx + 8], 2
        jmp    loc_1_6155
loc_1_610E:
        cmp    dword ptr [ebx + 8], 2
        je     loc_1_611E
        mov    eax, 1
        jmp    loc_1_61A8
loc_1_611E:
        push   ebx
        call   sub_1_5F7C
        add    esp, 4
        test   eax, eax
        jne    loc_1_6155
        mov    dword ptr [ebx + 8], 1
        jmp    loc_1_6155
loc_1_6134:
        cmp    dword ptr [ebx + 8], 0
        jne    loc_1_6141
        mov    eax, 1
        jmp    loc_1_61A8
loc_1_6141:
        push   ebx
        call   sub_1_5EB8
        add    esp, 4
        test   eax, eax
        jne    loc_1_6155
        mov    dword ptr [ebx + 8], 0
loc_1_6155:
        xor    eax, eax
        jmp    loc_1_61A8
loc_1_6159:
        mov    esi, dword ptr [ebx + 0xdc]
        mov    edi, dword ptr [ebp + 0x14]
        test   byte ptr [esi], 1
        jne    loc_1_616E
        mov    eax, 3
        jmp    loc_1_61A8
loc_1_616E:
        lea    eax, [ebp - 2]
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [esi + 8]
        call   sub_1_97C6
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_61A8
        mov    cx, word ptr [ebp - 2]
        inc    cx
        movzx  ecx, cx
        cmp    word ptr [esi + 6], 4
        jb     loc_1_6196
        add    ecx, ecx
loc_1_6196:
        mov    dword ptr [edi], ecx
        jmp    loc_1_61A8
loc_1_619A:
        push   dword ptr [ebp + 0x14]
        push   0
        push   ebx
        call   sub_1_5F97
loc_1_61A5:
        add    esp, 0xc
loc_1_61A8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        ; ---- dispatch table (11 entries) ----
        dd  loc_1_5FE3
        dd  loc_1_5FF4
        dd  loc_1_6005
        dd  loc_1_6016
        dd  loc_1_6035
        dd  loc_1_6054
        dd  loc_1_60A5
        dd  loc_1_60C4
        dd  loc_1_5FD9
        dd  loc_1_6159
        dd  loc_1_619A
sub_1_61DB:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   1
        push   dword ptr [eax + 0xe8]
        call   sub_1_3846
        add    esp, 0xc
        xor    eax, eax
        ret    
sub_1_61F6:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        push   edi
        lea    esi, [eax + 0x4c]
        mov    edi, edx
        mov    ecx, 0x1a
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [eax + 4]
        pop    edi
        xor    eax, eax
        pop    esi
        mov    dword ptr [edx + 4], ecx
        ret    
sub_1_6217:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ecx + 4]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        lea    esi, [ecx + 0xe8]
        test   al, 1
        mov    edx, dword ptr [esi]
        jne    loc_1_623F
        mov    eax, 3
        jmp    loc_1_62F4
loc_1_623F:
        test   al, 2
        je     loc_1_624D
        mov    eax, 4
        jmp    loc_1_62F4
loc_1_624D:
        mov    eax, dword ptr [edx + 0xf0]
        and    eax, 4
        jne    loc_1_6261
        test   byte ptr [edx + 0x1dc], 4
        je     loc_1_6266
loc_1_6261:
        mov    ebx, 1
loc_1_6266:
        mov    edi, dword ptr [ebp + 0x10]
        cmp    word ptr [edi + 0x12], 2
        jne    loc_1_62A3
        test   ebx, ebx
        je     loc_1_62A3
        test   eax, eax
        je     loc_1_628A
        mov    eax, dword ptr [edi + 0xe]
        cmp    dword ptr [edx + 0x11c], eax
        je     loc_1_628A
        mov    eax, 4
        jmp    loc_1_62F4
loc_1_628A:
        test   byte ptr [edx + 0x1dc], 4
        je     loc_1_62A3
        mov    eax, dword ptr [edi + 0xe]
        cmp    dword ptr [edx + 0x208], eax
        mov    eax, 4
        jne    loc_1_62F4
loc_1_62A3:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0xb4], eax
        mov    edx, dword ptr [edi + 6]
        mov    dword ptr [ecx + 0xb8], edx
        mov    ebx, dword ptr [edi + 0xa]
        mov    dword ptr [ecx + 0xbc], ebx
        mov    eax, dword ptr [edi + 0xe]
        mov    dword ptr [ecx + 0x30], eax
        mov    dx, word ptr [edi + 0x12]
        mov    word ptr [ecx + 0x34], dx
        or     byte ptr [ecx + 4], 2
        lea    ecx, [ebp - 2]
        push   ecx
        push   dword ptr [esi]
        call   sub_1_3984
        mov    cx, word ptr [ebp - 2]
        add    esp, 8
        mov    word ptr [edi + 4], cx
        push   2
        push   dword ptr [esi]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
loc_1_62F4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_62FB:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        lea    edx, [ecx + 0xb4]
        cmp    dword ptr [edx], eax
        je     loc_1_6314
        mov    eax, 1
        jmp    loc_1_634E
loc_1_6314:
        cmp    dword ptr [ecx + 8], 0
        je     loc_1_6321
        mov    eax, 8
        jmp    loc_1_634E
loc_1_6321:
        test   byte ptr [ecx + 4], 1
        mov    eax, 3
        je     loc_1_634E
        xor    eax, eax
        push   3
        mov    dword ptr [edx], eax
        mov    dword ptr [ecx + 0xb8], eax
        mov    dword ptr [edx], eax
        and    byte ptr [ecx + 4], 0xfd
        push   dword ptr [ecx + 0xe8]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
loc_1_634E:
        ret    
sub_1_634F:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0xe0]
        mov    edi, dword ptr [esp + 0x10]
        add    esi, 4
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        xor    eax, eax
        pop    edi
        pop    esi
        ret    
sub_1_636A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_6389
        add    esp, 4
        test   eax, eax
        jne    loc_1_6387
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_6387:
        pop    esi
        ret    
sub_1_6389:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 3
        ret    
sub_1_639C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        xor    eax, eax
        jmp    loc_1_63B2
loc_1_63B2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_63B7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        or     byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x2c], eax
        xor    eax, eax
        jmp    loc_1_63DC
loc_1_63DC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_63E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax], 1
        je     loc_1_63FB
        jmp    loc_1_6405
loc_1_63FB:
        mov    eax, 3
        jmp    loc_1_652C
loc_1_6405:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax], 2
        je     loc_1_6439
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_642F
        mov    eax, 4
        jmp    loc_1_652C
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_642F:
        mov    eax, 4
        jmp    loc_1_652C
loc_1_6439:
        mov    eax, dword ptr [ebp + 8]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x30], 2
        push   dword ptr [ebp + 8]
        call   sub_1_672C
        add    esp, 4
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_647D
loc_1_6479:
        inc    word ptr [ebp - 4]
loc_1_647D:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 0x12
        jge    loc_1_64A4
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + ecx + 0x87], 0
        jmp    loc_1_6479
loc_1_64A4:
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_64B3
loc_1_64AF:
        inc    word ptr [ebp - 4]
loc_1_64B3:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 0x10
        jge    loc_1_64E3
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x1e9], 4
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x1f9], 0xff
        jmp    loc_1_64AF
loc_1_64E3:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_650A
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xc], eax
        jmp    loc_1_6515
loc_1_650A:
        push   dword ptr [ebp + 8]
        call   sub_1_6667
        add    esp, 4
loc_1_6515:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x2c]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_652C
loc_1_652C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6531:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax], 1
        je     loc_1_654B
        jmp    loc_1_6555
loc_1_654B:
        mov    eax, 3
        jmp    loc_1_6605
loc_1_6555:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax], 2
        je     loc_1_65FE
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_65F4
        mov    eax, dword ptr [ebp + 8]
        and    byte ptr [eax], 0xfd
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_6585
loc_1_6581:
        inc    word ptr [ebp - 4]
loc_1_6585:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 0x12
        jge    loc_1_65D8
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x82]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x81]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x83]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 0x10
        jmp    loc_1_6581
loc_1_65D8:
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x2c]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_6605
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_65F4:
        mov    eax, 4
        jmp    loc_1_6605
loc_1_65FE:
        xor    eax, eax
        jmp    loc_1_6605
loc_1_6605:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_660A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_68A3
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        and    byte ptr [eax], 0xfe
        xor    eax, eax
        jmp    loc_1_6628
loc_1_6628:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_662D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   eax
        in     al, dx
        jmp    loc_1_6637
loc_1_6637:
        in     al, dx
        jmp    loc_1_663A
loc_1_663A:
        pop    eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6640:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_662D
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_662D
        pop    edx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6667:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 4], ax
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [ebp - 4]
        mov    ebx, 0x105
        mov    al, 1
        call   sub_1_6640
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_6640
        mov    ebx, 0x104
        mov    al, 0x3f
        call   sub_1_6640
        mov    ebx, 8
        xor    al, al
        call   sub_1_6640
        mov    ebx, 0xbd
        mov    al, 0xc0
        call   sub_1_6640
        mov    ecx, 0x15
        mov    ebx, 0x40
        mov    al, 0x3f
loc_1_66CE:
        xor    bh, bh
        call   sub_1_6640
        mov    bh, 1
        call   sub_1_6640
        inc    bl
        loop   loc_1_66CE
        mov    ecx, 8
        mov    ebx, 0xb0
        xor    eax, eax
loc_1_66EC:
        xor    bh, bh
        call   sub_1_6640
        mov    bh, 1
        call   sub_1_6640
        inc    bl
        loop   loc_1_66EC
        mov    ebx, 0x104
        xor    eax, eax
        call   sub_1_6640
        mov    ebx, 0x105
        call   sub_1_6640
        call   sub_1_662D
        call   sub_1_662D
        call   sub_1_662D
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_672C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        push   0x60
        push   4
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        push   0
        push   8
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 0x30]
        cmp    eax, 2
        jne    loc_1_6796
        push   1
        push   0x105
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        push   0
        push   0x104
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
loc_1_6796:
        push   0xc0
        push   0xbd
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_67BA
loc_1_67B6:
        inc    word ptr [ebp - 4]
loc_1_67BA:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 0x15
        jge    loc_1_6808
        push   0x3f
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0x40
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 0x30]
        cmp    eax, 2
        jne    loc_1_6803
        push   0x3f
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0x140
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
loc_1_6803:
        jmp    loc_1_67B6
loc_1_6808:
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_6817
loc_1_6813:
        inc    word ptr [ebp - 4]
loc_1_6817:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 8
        jge    loc_1_6867
        push   0
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0xb0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 0x30]
        cmp    eax, 2
        jne    loc_1_6862
        push   0
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0x1b0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
loc_1_6862:
        jmp    loc_1_6813
loc_1_6867:
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_6876
loc_1_6872:
        inc    word ptr [ebp - 4]
loc_1_6876:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 0x10
        jge    loc_1_6897
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + eax*4 + 0x31], 0
        jmp    loc_1_6872
loc_1_6897:
        xor    eax, eax
        jmp    loc_1_689E
loc_1_689E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_68A3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_68BB
loc_1_68B7:
        inc    word ptr [ebp - 4]
loc_1_68BB:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 0x12
        jge    loc_1_690E
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x82]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x81]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x83]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 0x10
        jmp    loc_1_68B7
loc_1_690E:
        push   dword ptr [ebp + 8]
        call   sub_1_672C
        add    esp, 4
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_6928
loc_1_6924:
        inc    word ptr [ebp - 4]
loc_1_6928:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 0x12
        jge    loc_1_694F
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + ecx + 0x87], 0
        jmp    loc_1_6924
loc_1_694F:
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_695E
loc_1_695A:
        inc    word ptr [ebp - 4]
loc_1_695E:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 0x10
        jge    loc_1_698E
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x1e9], 4
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x1f9], 0xff
        jmp    loc_1_695A
loc_1_698E:
        xor    eax, eax
        jmp    loc_1_6995
loc_1_6995:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_699A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 1
        jmp    loc_1_69B2
loc_1_69AC:
        shr    dword ptr [ebp + 8], 1
        inc    byte ptr [ebp - 4]
loc_1_69B2:
        cmp    dword ptr [ebp + 8], 0x400
        jb     loc_1_69C4
        jmp    loc_1_69AC
loc_1_69C4:
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 7
        jle    loc_1_69D5
        mov    byte ptr [ebp - 4], 7
loc_1_69D5:
        movzx  eax, byte ptr [ebp - 4]
        shl    eax, 0xa
        movzx  ecx, word ptr [ebp + 8]
        or     eax, ecx
        jmp    loc_1_69E7
loc_1_69E7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_69EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0xc]
        test   eax, eax
        jle    loc_1_6A20
        movsx  eax, word ptr [ebp + 0xc]
        imul   eax, eax, 0x1f
        sar    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        imul   eax, dword ptr [ebp + 8]
        shr    eax, 0xf
        add    dword ptr [ebp + 8], eax
        jmp    loc_1_6A4E
loc_1_6A20:
        movsx  eax, word ptr [ebp + 0xc]
        test   eax, eax
        jge    loc_1_6A4E
        movsx  eax, word ptr [ebp + 0xc]
        imul   eax, eax, 0x1b
        neg    eax
        sar    eax, 8
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        imul   ecx, dword ptr [ebp + 8]
        shr    ecx, 0xf
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp + 8], eax
loc_1_6A4E:
        mov    eax, dword ptr [ebp + 8]
        jmp    loc_1_6A56
loc_1_6A56:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6A5B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        movzx  eax, byte ptr [ebp + 0x1c]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_6B7D
loc_1_6A70:
        movzx  eax, byte ptr [ebp + 0x18]
        cmp    eax, 3
        jne    loc_1_6A86
        mov    byte ptr [ebp - 0xc], 1
        jmp    loc_1_6A8A
loc_1_6A86:
        mov    byte ptr [ebp - 0xc], 0
loc_1_6A8A:
        jmp    loc_1_6BB1
loc_1_6A8F:
        movzx  eax, byte ptr [ebp + 0x18]
        cmp    eax, 1
        je     loc_1_6AA9
        movzx  eax, byte ptr [ebp + 0x18]
        cmp    eax, 3
        jne    loc_1_6AB2
loc_1_6AA9:
        mov    byte ptr [ebp - 0xc], 1
        jmp    loc_1_6AB6
loc_1_6AB2:
        mov    byte ptr [ebp - 0xc], 0
loc_1_6AB6:
        jmp    loc_1_6BB1
loc_1_6ABB:
        movzx  eax, byte ptr [ebp + 0x18]
        test   eax, eax
        je     loc_1_6AD4
        movzx  eax, byte ptr [ebp + 0x18]
        cmp    eax, 3
        jne    loc_1_6ADD
loc_1_6AD4:
        mov    byte ptr [ebp - 0xc], 1
        jmp    loc_1_6AE1
loc_1_6ADD:
        mov    byte ptr [ebp - 0xc], 0
loc_1_6AE1:
        jmp    loc_1_6BB1
loc_1_6AE6:
        movzx  eax, byte ptr [ebp + 0x18]
        cmp    eax, 1
        je     loc_1_6AFC
        mov    byte ptr [ebp - 0xc], 1
        jmp    loc_1_6B00
loc_1_6AFC:
        mov    byte ptr [ebp - 0xc], 0
loc_1_6B00:
        jmp    loc_1_6BB1
loc_1_6B05:
        movzx  eax, byte ptr [ebp + 0x18]
        cmp    eax, 1
        je     loc_1_6B1F
        movzx  eax, byte ptr [ebp + 0x18]
        cmp    eax, 3
        jne    loc_1_6B28
loc_1_6B1F:
        mov    byte ptr [ebp - 0xc], 1
        jmp    loc_1_6B2C
loc_1_6B28:
        mov    byte ptr [ebp - 0xc], 0
loc_1_6B2C:
        jmp    loc_1_6BB1
loc_1_6B31:
        movzx  eax, byte ptr [ebp + 0x18]
        cmp    eax, 1
        jl     loc_1_6B47
        mov    byte ptr [ebp - 0xc], 1
        jmp    loc_1_6B4B
loc_1_6B47:
        mov    byte ptr [ebp - 0xc], 0
loc_1_6B4B:
        jmp    loc_1_6BB1
loc_1_6B50:
        movzx  eax, byte ptr [ebp + 0x18]
        cmp    eax, 2
        jg     loc_1_6B66
        mov    byte ptr [ebp - 0xc], 1
        jmp    loc_1_6B6A
loc_1_6B66:
        mov    byte ptr [ebp - 0xc], 0
loc_1_6B6A:
        jmp    loc_1_6BB1
loc_1_6B6F:
        mov    byte ptr [ebp - 0xc], 1
        jmp    loc_1_6BB1
        db  0E9h, 034h, 000h, 000h, 000h
loc_1_6B7D:
        cmp    dword ptr [ebp - 0x10], 7
        ja     loc_1_6BB1
        mov    eax, dword ptr [ebp - 0x10]
        jmp    dword ptr [eax*4]             ; -> obj1:0x6B91
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_6A70
        dd  loc_1_6A8F
        dd  loc_1_6ABB
        dd  loc_1_6AE6
        dd  loc_1_6B05
        dd  loc_1_6B31
        dd  loc_1_6B50
        dd  loc_1_6B6F
loc_1_6BB1:
        movzx  eax, byte ptr [ebp - 0xc]
        test   eax, eax
        jne    loc_1_6BC5
        mov    al, byte ptr [ebp + 0xc]
        jmp    loc_1_6C33
loc_1_6BC5:
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 0x227]
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 0x225]
        cmp    eax, ecx
        jl     loc_1_6BE3
        mov    eax, ecx
loc_1_6BE3:
        mov    word ptr [ebp - 4], ax
        movzx  eax, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x1e9]
        movzx  ecx, byte ptr [ebp + 0x14]
        sar    ecx, 2
        movzx  ecx, byte ptr [ecx]           ; -> obj1:0x1ED0
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 0xc]
        add    eax, ecx
        movzx  ecx, word ptr [ebp - 4]
        lea    eax, [eax + ecx*2]
        mov    word ptr [ebp - 8], ax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 0x3f
        jle    loc_1_6C2B
        mov    al, 0x3f
        jmp    loc_1_6C2E
loc_1_6C2B:
        mov    al, byte ptr [ebp - 8]
loc_1_6C2E:
        jmp    loc_1_6C33
loc_1_6C33:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6C38:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        movzx  eax, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x1e9]
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 0x225]
        lea    eax, [eax + ecx*2]
        mov    word ptr [ebp - 8], ax
        movzx  eax, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x1e9]
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 0x227]
        lea    eax, [eax + ecx*2]
        mov    word ptr [ebp - 4], ax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 0x3f
        jle    loc_1_6CA2
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 0x3f
        jle    loc_1_6CA2
        mov    al, 0xcf
        jmp    loc_1_6D11
loc_1_6CA2:
        movzx  eax, word ptr [ebp - 8]
        add    eax, 8
        movzx  ecx, word ptr [ebp - 4]
        cmp    eax, ecx
        jge    loc_1_6CD0
        movzx  eax, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x1f9]
        and    al, 0xef
        jmp    loc_1_6D11
        db  0E9h, 041h, 000h, 000h, 000h
loc_1_6CD0:
        movzx  eax, word ptr [ebp - 4]
        add    eax, 8
        movzx  ecx, word ptr [ebp - 8]
        cmp    eax, ecx
        jge    loc_1_6CFE
        movzx  eax, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x1f9]
        and    al, 0xdf
        jmp    loc_1_6D11
        db  0E9h, 013h, 000h, 000h, 000h
loc_1_6CFE:
        movzx  eax, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x1f9]
        jmp    loc_1_6D11
loc_1_6D11:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6D16:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 0x30]
        cmp    eax, 2
        jne    loc_1_6D3A
        mov    word ptr [ebp - 0xc], 0x12
        jmp    loc_1_6D40
loc_1_6D3A:
        mov    word ptr [ebp - 0xc], 9
loc_1_6D40:
        mov    word ptr [ebp - 0x10], 0
        jmp    loc_1_6D4F
loc_1_6D4B:
        inc    word ptr [ebp - 0x10]
loc_1_6D4F:
        movzx  eax, word ptr [ebp - 0x10]
        movzx  ecx, word ptr [ebp - 0xc]
        cmp    eax, ecx
        jge    loc_1_6D85
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + ecx + 0x87], 0
        jne    loc_1_6D80
        mov    ax, word ptr [ebp - 0x10]
        jmp    loc_1_6F3B
loc_1_6D80:
        jmp    loc_1_6D4B
loc_1_6D85:
        mov    dword ptr [ebp - 4], 0xffffffff
        mov    word ptr [ebp - 8], 0xffff
        mov    word ptr [ebp - 0x10], 0
        jmp    loc_1_6DA1
loc_1_6D9D:
        inc    word ptr [ebp - 0x10]
loc_1_6DA1:
        movzx  eax, word ptr [ebp - 0x10]
        movzx  ecx, word ptr [ebp - 0xc]
        cmp    eax, ecx
        jge    loc_1_6E06
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x84]
        test   eax, eax
        jne    loc_1_6E01
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx + 0x87], edx
        jae    loc_1_6E01
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx + 0x87]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp - 0x10]
        mov    word ptr [ebp - 8], ax
loc_1_6E01:
        jmp    loc_1_6D9D
loc_1_6E06:
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 0xffff
        je     loc_1_6E1E
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_6F3B
loc_1_6E1E:
        mov    dword ptr [ebp - 4], 0xffffffff
        mov    word ptr [ebp - 8], 0xffff
        mov    word ptr [ebp - 0x10], 0
        jmp    loc_1_6E3A
loc_1_6E36:
        inc    word ptr [ebp - 0x10]
loc_1_6E3A:
        movzx  eax, word ptr [ebp - 0x10]
        movzx  ecx, word ptr [ebp - 0xc]
        cmp    eax, ecx
        jge    loc_1_6EA3
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x83]
        movzx  ecx, byte ptr [ebp + 0xc]
        cmp    eax, ecx
        jne    loc_1_6E9E
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx + 0x87], edx
        jae    loc_1_6E9E
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx + 0x87]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp - 0x10]
        mov    word ptr [ebp - 8], ax
loc_1_6E9E:
        jmp    loc_1_6E36
loc_1_6EA3:
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 0xffff
        je     loc_1_6EBB
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_6F3B
loc_1_6EBB:
        mov    word ptr [ebp - 8], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx + 0x87]
        mov    dword ptr [ebp - 4], eax
        movzx  eax, word ptr [ebp - 8]
        inc    eax
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_6EE7
loc_1_6EE3:
        inc    word ptr [ebp - 0x10]
loc_1_6EE7:
        movzx  eax, word ptr [ebp - 0x10]
        movzx  ecx, word ptr [ebp - 0xc]
        cmp    eax, ecx
        jge    loc_1_6F32
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx + 0x87], edx
        jae    loc_1_6F2D
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx + 0x87]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp - 0x10]
        mov    word ptr [ebp - 8], ax
loc_1_6F2D:
        jmp    loc_1_6EE3
loc_1_6F32:
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_6F3B
loc_1_6F3B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_6F58
loc_1_6F54:
        inc    word ptr [ebp - 4]
loc_1_6F58:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 4
        jge    loc_1_6FF4
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + ecx]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx], al
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + ecx + 1]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 1], al
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + ecx + 2]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 2], al
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + ecx + 3]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 3], al
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + ecx + 4]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 4], al
        jmp    loc_1_6F54
loc_1_6FF4:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x14]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x14], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x15]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x15], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x16]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x16], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x17]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x17], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x18]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x18], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x19]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x19], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x1a]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x1a], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x1b]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x1b], al
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7059:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        movzx  eax, word ptr [ebp + 0xc]
        cmp    eax, 9
        jl     loc_1_7081
        movzx  eax, word ptr [ebp + 0xc]
        add    eax, 0xf7
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_7089
loc_1_7081:
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [ebp - 4], ax
loc_1_7089:
        push   0
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0xb0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_70AF
loc_1_70AB:
        inc    word ptr [ebp - 8]
loc_1_70AF:
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 2
        jge    loc_1_716A
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 5
        add    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0xc], eax
        movzx  eax, word ptr [ebp - 8]
        movzx  ecx, word ptr [ebp + 0xc]
        shl    ecx, 2
        mov    ax, word ptr [ecx + eax*2]    ; -> obj1:0x1DE8
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0x20
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 1]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0x40
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 2]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0x60
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 3]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0x80
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 4]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0xe0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        jmp    loc_1_70AB
loc_1_716A:
        movzx  eax, word ptr [ebp + 0xc]
        cmp    eax, 9
        jl     loc_1_7189
        movzx  eax, word ptr [ebp + 0xc]
        add    eax, 0xf7
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_7191
loc_1_7189:
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [ebp - 4], ax
loc_1_7191:
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + 0x14]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0xa0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + 0x18]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0xc0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x10]
        movzx  eax, byte ptr [eax + 0x16]
        or     al, 0x20
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0xb0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_71ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x44
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_01E8
        movzx  eax, byte ptr [ebp + 0xc]
        imul   eax, eax, 0x1c
        add    dword ptr [ebp - 0xc], eax
        mov    ecx, 0xc
        movzx  eax, byte ptr [ebp + 0x10]
        cdq    
        idiv   ecx
        mov    eax, dword ptr [edx*4]        ; -> loc_1_1EA0
        mov    dword ptr [ebp - 4], eax
        mov    ecx, 0xc
        movzx  eax, byte ptr [ebp + 0x10]
        cdq    
        idiv   ecx
        mov    byte ptr [ebp - 0x44], al
        movzx  eax, byte ptr [ebp - 0x44]
        cmp    eax, 5
        jle    loc_1_724A
        movzx  eax, byte ptr [ebp - 0x44]
        sub    eax, 5
        mov    cl, al
        shl    dword ptr [ebp - 4], cl
        jmp    loc_1_7267
loc_1_724A:
        movzx  eax, byte ptr [ebp - 0x44]
        cmp    eax, 5
        jge    loc_1_7267
        mov    eax, 5
        movzx  ecx, byte ptr [ebp - 0x44]
        sub    eax, ecx
        mov    cl, al
        shr    dword ptr [ebp - 4], cl
loc_1_7267:
        push   dword ptr [ebp - 0xc]
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_6F40
        add    esp, 8
        mov    word ptr [ebp - 0x18], 0
        jmp    loc_1_7285
loc_1_7281:
        inc    word ptr [ebp - 0x18]
loc_1_7285:
        movzx  eax, word ptr [ebp - 0x18]
        cmp    eax, 2
        jge    loc_1_7334
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 0x18]
        mov    dword ptr [ebp + ecx*4 - 0x3c], eax
        movzx  eax, word ptr [ebp - 0x18]
        movzx  eax, byte ptr [ebp + eax - 0x1e]
        shr    eax, 2
        and    al, 7
        mov    byte ptr [ebp - 0x44], al
        movzx  eax, byte ptr [ebp - 0x44]
        cmp    eax, 4
        jle    loc_1_72D1
        movzx  eax, byte ptr [ebp - 0x44]
        sub    eax, 4
        movzx  edx, word ptr [ebp - 0x18]
        mov    cl, al
        shl    dword ptr [ebp + edx*4 - 0x3c], cl
        jmp    loc_1_72F3
loc_1_72D1:
        movzx  eax, byte ptr [ebp - 0x44]
        cmp    eax, 4
        jge    loc_1_72F3
        mov    eax, 4
        movzx  ecx, byte ptr [ebp - 0x44]
        sub    eax, ecx
        movzx  edx, word ptr [ebp - 0x18]
        mov    cl, al
        shr    dword ptr [ebp + edx*4 - 0x3c], cl
loc_1_72F3:
        push   dword ptr [ebp + 0x1c]
        movzx  eax, word ptr [ebp - 0x18]
        push   dword ptr [ebp + eax*4 - 0x3c]
        call   sub_1_69EC
        add    esp, 8
        push   eax
        call   sub_1_699A
        add    esp, 4
        mov    word ptr [ebp - 0x10], ax
        mov    al, byte ptr [ebp - 0x10]
        movzx  ecx, word ptr [ebp - 0x18]
        mov    byte ptr [ebp + ecx - 0x20], al
        movzx  eax, word ptr [ebp - 0x10]
        movzx  eax, ah
        or     al, 0x20
        movzx  ecx, word ptr [ebp - 0x18]
        mov    byte ptr [ebp + ecx - 0x1e], al
        jmp    loc_1_7281
loc_1_7334:
        movzx  eax, byte ptr [ebp - 0x1c]
        and    eax, 1
        add    eax, eax
        add    eax, 4
        mov    byte ptr [ebp - 8], al
        mov    word ptr [ebp - 0x14], 0
        jmp    loc_1_7352
loc_1_734E:
        inc    word ptr [ebp - 0x14]
loc_1_7352:
        movzx  eax, word ptr [ebp - 0x14]
        cmp    eax, 2
        jge    loc_1_73B2
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        movzx  eax, word ptr [ebp - 0x14]
        imul   eax, eax, 5
        movzx  eax, byte ptr [ebp + eax - 0x33]
        and    eax, 0x3f
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_6A5B
        add    esp, 0x18
        movzx  ax, al
        mov    word ptr [ebp - 0x10], ax
        movzx  eax, word ptr [ebp - 0x14]
        imul   eax, eax, 5
        movzx  eax, byte ptr [ebp + eax - 0x33]
        and    eax, 0xc0
        or     al, byte ptr [ebp - 0x10]
        movzx  ecx, word ptr [ebp - 0x14]
        imul   ecx, ecx, 5
        mov    byte ptr [ebp + ecx - 0x33], al
        jmp    loc_1_734E
loc_1_73B2:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 8]
        call   sub_1_6C38
        add    esp, 8
        mov    byte ptr [ebp - 0x40], al
        movzx  eax, byte ptr [ebp - 0x1c]
        movzx  ecx, byte ptr [ebp - 0x40]
        and    eax, ecx
        mov    byte ptr [ebp - 0x1c], al
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_6D16
        add    esp, 8
        mov    word ptr [ebp - 0x10], ax
        lea    eax, [ebp - 0x34]
        push   eax
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_7059
        add    esp, 0xc
        mov    al, byte ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp - 0x10]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x81], al
        mov    al, byte ptr [ebp + 0x14]
        movzx  ecx, word ptr [ebp - 0x10]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x82], al
        mov    al, byte ptr [ebp + 0xc]
        movzx  ecx, word ptr [ebp - 0x10]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x83], al
        mov    al, byte ptr [ebp + 0x18]
        movzx  ecx, word ptr [ebp - 0x10]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x85], al
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x84], 1
        movzx  eax, word ptr [ebp - 0x10]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x86], 0
        mov    eax, dword ptr [0]            ; -> loc_1_1EF0
        movzx  ecx, word ptr [ebp - 0x10]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 0x87], eax
        inc    dword ptr [0]                 ; -> loc_1_1EF0
        mov    eax, dword ptr [ebp - 0x3c]
        movzx  ecx, word ptr [ebp - 0x10]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 0x8b], eax
        mov    eax, dword ptr [ebp - 0x38]
        movzx  ecx, word ptr [ebp - 0x10]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 0x8f], eax
        mov    al, byte ptr [ebp - 0x1e]
        movzx  ecx, word ptr [ebp - 0x10]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x93], al
        mov    al, byte ptr [ebp - 0x1d]
        movzx  ecx, word ptr [ebp - 0x10]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x94], al
        xor    eax, eax
        jmp    loc_1_74DB
loc_1_74DB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_74E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 0x30]
        cmp    eax, 2
        jne    loc_1_7504
        mov    word ptr [ebp - 4], 0x12
        jmp    loc_1_750A
loc_1_7504:
        mov    word ptr [ebp - 4], 9
loc_1_750A:
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_7519
loc_1_7515:
        inc    word ptr [ebp - 8]
loc_1_7519:
        movzx  eax, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 8]
        cmp    eax, ecx
        jle    loc_1_758D
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x82]
        movzx  ecx, byte ptr [ebp + 0x10]
        cmp    eax, ecx
        jne    loc_1_7588
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x81]
        movzx  ecx, byte ptr [ebp + 0xc]
        cmp    eax, ecx
        jne    loc_1_7588
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x84]
        test   eax, eax
        je     loc_1_7588
        mov    ax, word ptr [ebp - 8]
        jmp    loc_1_7596
loc_1_7588:
        jmp    loc_1_7515
loc_1_758D:
        mov    ax, 0xffff
        jmp    loc_1_7596
loc_1_7596:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_759B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0        ; -> loc_1_01E8
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_74E0
        add    esp, 0xc
        mov    word ptr [ebp - 8], ax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 0xffff
        je     loc_1_76E0
        movzx  eax, byte ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x209]
        test   eax, eax
        je     loc_1_75FF
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x86], 1
        xor    eax, eax
        jmp    loc_1_76E7
loc_1_75FF:
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x83]
        imul   eax, eax, 0x1c
        add    dword ptr [ebp - 4], eax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 9
        jl     loc_1_7636
        movzx  eax, word ptr [ebp - 8]
        add    eax, 0xf7
        mov    word ptr [ebp - 0xc], ax
        jmp    loc_1_763E
loc_1_7636:
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 0xc], ax
loc_1_763E:
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x93]
        and    eax, 0x1f
        push   eax
        movzx  eax, word ptr [ebp - 0xc]
        add    eax, 0xb0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x84], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x93]
        and    al, 0x1f
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x93], al
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x94]
        and    al, 0x1f
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x94], al
        mov    eax, dword ptr [0]            ; -> loc_1_1EF0
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 0x87], eax
        jmp    loc_1_76E0
loc_1_76E0:
        xor    eax, eax
        jmp    loc_1_76E7
loc_1_76E7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_76EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 0x30]
        cmp    eax, 2
        jne    loc_1_77CB
        movzx  eax, word ptr [ebp + 0xc]
        cmp    eax, 0x100
        jge    loc_1_7727
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
        jmp    loc_1_7738
loc_1_7727:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        movzx  eax, word ptr [eax + 4]
        add    eax, 2
        mov    word ptr [ebp - 8], ax
loc_1_7738:
        mov    word ptr [ebp - 4], 1
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [ebp - 0x10], al
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 0x10]
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_7769
loc_1_7766:
        inc    dword ptr [ebp - 0xc]
loc_1_7769:
        cmp    dword ptr [ebp - 0xc], 0x19
        jae    loc_1_777E
        mov    dx, word ptr [ebp - 8]
        in     ax, dx
        jmp    loc_1_7766
loc_1_777E:
        movzx  eax, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 8]
        add    eax, ecx
        mov    word ptr [ebp - 8], ax
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp + 0x10]
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_77B1
loc_1_77AE:
        inc    dword ptr [ebp - 0xc]
loc_1_77B1:
        cmp    dword ptr [ebp - 0xc], 0x19
        jae    loc_1_77C6
        mov    dx, word ptr [ebp - 8]
        in     ax, dx
        jmp    loc_1_77AE
loc_1_77C6:
        jmp    loc_1_77CB
loc_1_77CB:
        xor    eax, eax
        jmp    loc_1_77D2
loc_1_77D2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_77D7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x28], 0
        mov    word ptr [ebp - 0x26], 0
        mov    word ptr [ebp - 0x24], 0
        mov    word ptr [ebp - 0x22], 1
        mov    word ptr [ebp - 0x20], 1
        mov    word ptr [ebp - 0x1e], 1
        mov    word ptr [ebp - 0x1c], 2
        mov    word ptr [ebp - 0x1a], 2
        mov    word ptr [ebp - 0x18], 2
        mov    word ptr [ebp - 0x16], 2
        mov    word ptr [ebp - 0x14], 3
        mov    word ptr [ebp - 0x12], 3
        mov    word ptr [ebp - 0x10], 3
        mov    word ptr [ebp - 0xe], 3
        mov    word ptr [ebp - 0xc], 3
        mov    word ptr [ebp - 0xa], 3
        movzx  eax, word ptr [ebp + 8]
        cmp    eax, 0xffff
        jne    loc_1_7856
        xor    al, al
        jmp    loc_1_794C
loc_1_7856:
        movzx  eax, word ptr [ebp + 8]
        test   eax, eax
        jne    loc_1_7869
        mov    al, 0xc0
        jmp    loc_1_794C
loc_1_7869:
        mov    ax, word ptr [ebp + 8]
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 0x30], 0
        mov    word ptr [ebp - 0x2c], 0
        jmp    loc_1_7895
loc_1_7882:
        movzx  eax, word ptr [ebp - 0x30]
        add    eax, 4
        mov    word ptr [ebp - 0x30], ax
        shr    word ptr [ebp - 4], 1
        inc    word ptr [ebp - 0x2c]
loc_1_7895:
        movzx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     loc_1_78A6
        jmp    loc_1_7882
loc_1_78A6:
        movzx  eax, word ptr [ebp - 0x2c]
        cmp    eax, 5
        jle    loc_1_78C9
        movzx  eax, word ptr [ebp + 8]
        movzx  ecx, word ptr [ebp - 0x2c]
        sub    ecx, 5
        sar    eax, cl
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_78F8
loc_1_78C9:
        movzx  eax, word ptr [ebp - 0x2c]
        cmp    eax, 5
        jge    loc_1_78F0
        movzx  eax, word ptr [ebp + 8]
        mov    ecx, 5
        movzx  edx, word ptr [ebp - 0x2c]
        sub    ecx, edx
        shl    eax, cl
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_78F8
loc_1_78F0:
        mov    ax, word ptr [ebp + 8]
        mov    word ptr [ebp - 4], ax
loc_1_78F8:
        movzx  eax, word ptr [ebp - 4]
        and    eax, 0xf
        mov    word ptr [ebp - 4], ax
        movzx  eax, word ptr [ebp - 4]
        movzx  eax, word ptr [ebp + eax*2 - 0x28]
        movzx  ecx, word ptr [ebp - 0x30]
        add    eax, ecx
        mov    word ptr [ebp - 0x30], ax
        mov    eax, 0x43
        movzx  ecx, byte ptr [ebp - 0x30]
        sub    eax, ecx
        mov    byte ptr [ebp - 8], al
        movzx  eax, byte ptr [ebp - 8]
        xor    ecx, ecx
        cmp    eax, 0x80
        setl   cl
        movzx  eax, cl
        test   eax, eax
        je     loc_1_7945
        mov    al, byte ptr [ebp - 8]
        jmp    loc_1_7947
loc_1_7945:
        mov    al, 0x7f
loc_1_7947:
        jmp    loc_1_794C
loc_1_794C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7951:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x1c], 0     ; -> loc_1_01E8
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x221], 0
        je     loc_1_79F0
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x219]
        call   sub_1_77D7
        add    esp, 4
        movzx  ebx, al
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x21d]
        call   sub_1_77D7
        add    esp, 4
        movzx  eax, al
        add    ebx, eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x225], bx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x21d]
        shr    eax, 0x10
        and    eax, 0xffff
        push   eax
        call   sub_1_77D7
        add    esp, 4
        movzx  ebx, al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x219]
        shr    eax, 0x10
        and    eax, 0xffff
        push   eax
        call   sub_1_77D7
        add    esp, 4
        movzx  eax, al
        add    ebx, eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x227], bx
        jmp    loc_1_7A08
loc_1_79F0:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x225], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x227], 0
loc_1_7A08:
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 0x225]
        add    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x225], ax
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 0x225]
        cmp    eax, 0xc0
        jle    loc_1_7A3F
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x225], 0xc0
loc_1_7A3F:
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 0x227]
        add    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x227], ax
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 0x227]
        cmp    eax, 0xc0
        jle    loc_1_7A76
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x227], 0xc0
loc_1_7A76:
        mov    word ptr [ebp - 0x14], 0
        jmp    loc_1_7A85
loc_1_7A81:
        inc    word ptr [ebp - 0x14]
loc_1_7A85:
        movzx  eax, word ptr [ebp - 0x14]
        cmp    eax, 0x12
        jge    loc_1_7C10
        movzx  eax, word ptr [ebp - 0x14]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x82]
        movzx  ecx, byte ptr [ebp + 0xc]
        cmp    eax, ecx
        je     loc_1_7ABF
        movzx  eax, byte ptr [ebp + 0xc]
        cmp    eax, 0xff
        jne    loc_1_7C0B
loc_1_7ABF:
        movzx  eax, word ptr [ebp - 0x14]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x83]
        imul   eax, eax, 0x1c
        add    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, byte ptr [eax + 0x18]
        and    eax, 1
        add    eax, eax
        add    eax, 4
        mov    byte ptr [ebp - 4], al
        mov    word ptr [ebp - 0x18], 0
        jmp    loc_1_7AFB
loc_1_7AF7:
        inc    word ptr [ebp - 0x18]
loc_1_7AFB:
        movzx  eax, word ptr [ebp - 0x18]
        cmp    eax, 2
        jge    loc_1_7BA1
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 0x18]
        movzx  eax, word ptr [ebp - 0x14]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x85]
        push   eax
        movzx  eax, word ptr [ebp - 0x14]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x82]
        push   eax
        movzx  eax, word ptr [ebp - 0x18]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp - 8]
        movzx  eax, byte ptr [eax + ecx + 1]
        and    eax, 0x3f
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_6A5B
        add    esp, 0x18
        movzx  ax, al
        mov    word ptr [ebp - 0x10], ax
        movzx  eax, word ptr [ebp - 0x14]
        movzx  ecx, word ptr [ebp - 0x18]
        add    ecx, ecx
        mov    ax, word ptr [ecx + eax*4]    ; -> obj1:0x1DE8
        mov    word ptr [ebp - 0xc], ax
        movzx  eax, word ptr [ebp - 0x18]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp - 8]
        movzx  eax, byte ptr [eax + ecx + 1]
        and    eax, 0xc0
        movzx  ecx, byte ptr [ebp - 0x10]
        or     eax, ecx
        push   eax
        movzx  eax, word ptr [ebp - 0xc]
        add    eax, 0x40
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        jmp    loc_1_7AF7
loc_1_7BA1:
        movzx  eax, word ptr [ebp - 0x14]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x82]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_6C38
        add    esp, 8
        mov    byte ptr [ebp - 0x20], al
        movzx  eax, word ptr [ebp - 0x14]
        cmp    eax, 9
        jl     loc_1_7BE0
        movzx  eax, word ptr [ebp - 0x14]
        add    eax, 0xf7
        mov    word ptr [ebp - 0xc], ax
        jmp    loc_1_7BE8
loc_1_7BE0:
        mov    ax, word ptr [ebp - 0x14]
        mov    word ptr [ebp - 0xc], ax
loc_1_7BE8:
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, byte ptr [eax + 0x18]
        movzx  ecx, byte ptr [ebp - 0x20]
        and    eax, ecx
        push   eax
        movzx  eax, word ptr [ebp - 0xc]
        add    eax, 0xc0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
loc_1_7C0B:
        jmp    loc_1_7A81
loc_1_7C10:
        xor    eax, eax
        jmp    loc_1_7C17
loc_1_7C17:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7C1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        movzx  ecx, byte ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x31], eax
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_7C42
loc_1_7C3E:
        inc    word ptr [ebp - 0xc]
loc_1_7C42:
        movzx  eax, word ptr [ebp - 0xc]
        cmp    eax, 0x12
        jge    loc_1_7D57
        movzx  eax, word ptr [ebp - 0xc]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x82]
        movzx  ecx, byte ptr [ebp + 0xc]
        cmp    eax, ecx
        jne    loc_1_7D52
        mov    word ptr [ebp - 0x10], 0
        push   dword ptr [ebp + 0x10]
        movzx  eax, word ptr [ebp - 0x10]
        movzx  ecx, word ptr [ebp - 0xc]
        imul   ecx, ecx, 0x14
        lea    eax, [ecx + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [eax + ecx + 0x8b]
        call   sub_1_69EC
        add    esp, 8
        mov    dword ptr [ebp - 0x14], eax
        push   dword ptr [ebp - 0x14]
        call   sub_1_699A
        add    esp, 4
        movzx  ecx, word ptr [ebp - 0x10]
        mov    word ptr [ebp + ecx*2 - 8], ax
        movzx  eax, word ptr [ebp - 0xc]
        imul   eax, eax, 0x14
        movzx  ecx, word ptr [ebp - 0x10]
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x93]
        and    al, 0xe0
        movzx  eax, al
        movzx  ecx, word ptr [ebp - 0x10]
        movzx  ecx, word ptr [ebp + ecx*2 - 8]
        or     al, ch
        movzx  ecx, word ptr [ebp - 0xc]
        imul   ecx, ecx, 0x14
        movzx  edx, word ptr [ebp - 0x10]
        add    ecx, edx
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x93], al
        movzx  eax, word ptr [ebp - 0xc]
        cmp    eax, 9
        jl     loc_1_7D0B
        movzx  eax, word ptr [ebp - 0xc]
        add    eax, 0xf7
        mov    word ptr [ebp - 4], ax
        jmp    loc_1_7D13
loc_1_7D0B:
        mov    ax, word ptr [ebp - 0xc]
        mov    word ptr [ebp - 4], ax
loc_1_7D13:
        movzx  eax, word ptr [ebp - 0xc]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x93]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0xb0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        push   dword ptr [ebp - 8]
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0xa0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
loc_1_7D52:
        jmp    loc_1_7C3E
loc_1_7D57:
        xor    eax, eax
        jmp    loc_1_7D5E
loc_1_7D5E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7D63:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        movzx  eax, byte ptr [ebp + 0x10]
        cmp    eax, 0x40
        jge    loc_1_7F19
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 0x30]
        cmp    eax, 2
        jne    loc_1_7D94
        mov    word ptr [ebp - 4], 0x12
        jmp    loc_1_7D9A
loc_1_7D94:
        mov    word ptr [ebp - 4], 9
loc_1_7D9A:
        movzx  eax, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x209], 0
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_7DB8
loc_1_7DB4:
        inc    word ptr [ebp - 8]
loc_1_7DB8:
        movzx  eax, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 8]
        cmp    eax, ecx
        jle    loc_1_7F14
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x82]
        movzx  ecx, byte ptr [ebp + 0xc]
        cmp    eax, ecx
        jne    loc_1_7F0F
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x84]
        test   eax, eax
        je     loc_1_7F0F
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x86]
        test   eax, eax
        je     loc_1_7F0F
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_01E8
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x83]
        imul   eax, eax, 0x1c
        add    dword ptr [ebp - 0xc], eax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 9
        jl     loc_1_7E58
        movzx  eax, word ptr [ebp - 8]
        add    eax, 0xf7
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_7E60
loc_1_7E58:
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 0x10], ax
loc_1_7E60:
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x93]
        and    eax, 0x1f
        push   eax
        movzx  eax, word ptr [ebp - 0x10]
        add    eax, 0xb0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x84], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x86], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x93]
        and    al, 0x1f
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x93], al
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x94]
        and    al, 0x1f
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x94], al
        mov    eax, dword ptr [0]            ; -> loc_1_1EF0
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 0x87], eax
loc_1_7F0F:
        jmp    loc_1_7DB4
loc_1_7F14:
        jmp    loc_1_7F28
loc_1_7F19:
        movzx  eax, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x209], 1
loc_1_7F28:
        xor    eax, eax
        jmp    loc_1_7F2F
loc_1_7F2F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7F34:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 0x30]
        cmp    eax, 2
        jne    loc_1_7F58
        mov    word ptr [ebp - 4], 0x12
        jmp    loc_1_7F5E
loc_1_7F58:
        mov    word ptr [ebp - 4], 9
loc_1_7F5E:
        mov    word ptr [ebp - 8], 0
        jmp    loc_1_7F6D
loc_1_7F69:
        inc    word ptr [ebp - 8]
loc_1_7F6D:
        movzx  eax, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 8]
        cmp    eax, ecx
        jle    loc_1_80DE
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x82]
        movzx  ecx, byte ptr [ebp + 0xc]
        cmp    eax, ecx
        jne    loc_1_80D9
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x84]
        test   eax, eax
        je     loc_1_80D9
        movzx  eax, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x209]
        cmp    eax, 1
        jne    loc_1_7FE4
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x86], 1
        jmp    loc_1_7F69
loc_1_7FE4:
        mov    dword ptr [ebp - 0xc], 0      ; -> loc_1_01E8
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x83]
        imul   eax, eax, 0x1c
        add    dword ptr [ebp - 0xc], eax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 9
        jl     loc_1_8022
        movzx  eax, word ptr [ebp - 8]
        add    eax, 0xf7
        mov    word ptr [ebp - 0x10], ax
        jmp    loc_1_802A
loc_1_8022:
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 0x10], ax
loc_1_802A:
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x93]
        and    eax, 0x1f
        push   eax
        movzx  eax, word ptr [ebp - 0x10]
        add    eax, 0xb0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0xc
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x84], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x86], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x93]
        and    al, 0x1f
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x93], al
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x94]
        and    al, 0x1f
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x94], al
        mov    eax, dword ptr [0]            ; -> loc_1_1EF0
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 0x14
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 0x87], eax
loc_1_80D9:
        jmp    loc_1_7F69
loc_1_80DE:
        xor    eax, eax
        jmp    loc_1_80E5
loc_1_80E5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_80EA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        and    al, 0xf0
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 0x18]
        and    al, 0xf
        mov    byte ptr [ebp - 0x10], al
        mov    eax, dword ptr [ebp - 0x18]
        shr    eax, 8
        and    al, 0x7f
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 0x18]
        shr    eax, 0x10
        and    al, 0x7f
        mov    byte ptr [ebp - 0xc], al
        movzx  eax, byte ptr [ebp - 8]
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_84AA
loc_1_812D:
        movzx  eax, byte ptr [ebp - 0xc]
        test   eax, eax
        je     loc_1_81D4
        movzx  eax, byte ptr [ebp - 0x10]
        cmp    eax, 9
        jne    loc_1_81A4
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 0x23
        jl     loc_1_819F
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 0x57
        jg     loc_1_819F
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + eax*4 + 0x31]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 0x10]
        movzx  eax, byte ptr [ebp - 4]
        mov    al, byte ptr [eax*2 - 0x45]   ; -> obj1:0x1DEB
        push   eax
        movzx  eax, byte ptr [ebp - 4]
        movzx  eax, byte ptr [eax*2 - 0x46]  ; -> loc_1_1DEA
        add    eax, 0x80
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_71ED
        add    esp, 0x18
        jmp    loc_1_84F0
loc_1_819F:
        jmp    loc_1_81D4
loc_1_81A4:
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + eax*4 + 0x31]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 4]
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x71]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_71ED
        add    esp, 0x18
        jmp    loc_1_84F0
loc_1_81D4:
        movzx  eax, byte ptr [ebp - 0x10]
        cmp    eax, 9
        jne    loc_1_8231
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 0x23
        jl     loc_1_822C
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 0x57
        jg     loc_1_822C
        push   dword ptr [ebp - 0x10]
        movzx  eax, byte ptr [ebp - 4]
        mov    al, byte ptr [eax*2 - 0x45]   ; -> obj1:0x1DEB
        push   eax
        movzx  eax, byte ptr [ebp - 4]
        movzx  eax, byte ptr [eax*2 - 0x46]  ; -> loc_1_1DEA
        add    eax, 0x80
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 0x10
        jmp    loc_1_84F0
loc_1_822C:
        jmp    loc_1_8253
loc_1_8231:
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 4]
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x71]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 0x10
        jmp    loc_1_84F0
loc_1_8253:
        xor    eax, eax
        jmp    loc_1_84F0
loc_1_825A:
        movzx  eax, byte ptr [ebp - 4]
        mov    dword ptr [ebp - 0x24], eax
        jmp    loc_1_83D6
loc_1_8266:
        movzx  eax, byte ptr [ebp - 0xc]
        and    eax, 0x7c
        shr    eax, 2
        mov    al, byte ptr [eax]            ; -> obj1:0x1ED0
        movzx  ecx, byte ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x1e9], al
        jmp    loc_1_8436
loc_1_8289:
        movzx  eax, byte ptr [ebp - 0xc]
        cmp    eax, 0x50
        jle    loc_1_82AA
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x1f9], 0xef
        jmp    loc_1_82DA
loc_1_82AA:
        movzx  eax, byte ptr [ebp - 0xc]
        cmp    eax, 0x30
        jge    loc_1_82CB
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x1f9], 0xdf
        jmp    loc_1_82DA
loc_1_82CB:
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 0x1f9], 0xff
loc_1_82DA:
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_7951
        add    esp, 8
        jmp    loc_1_8436
loc_1_82ED:
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_7D63
        add    esp, 0xc
        jmp    loc_1_84F0
loc_1_8303:
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_7F34
        add    esp, 8
        jmp    loc_1_84F0
loc_1_8316:
        push   0
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_7D63
        add    esp, 0xc
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + eax*4 + 0x31], 0xffff
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + eax*4 + 0x31]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_7C1C
        add    esp, 0xc
        jmp    loc_1_8436
loc_1_8353:
        mov    dword ptr [ebp - 0x1c], 0
        jmp    loc_1_8362
loc_1_835F:
        inc    dword ptr [ebp - 0x1c]
loc_1_8362:
        cmp    dword ptr [ebp - 0x1c], 0x12
        jae    loc_1_83CC
        mov    eax, dword ptr [ebp - 0x1c]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 0x82]
        movzx  ecx, byte ptr [ebp - 0x10]
        cmp    eax, ecx
        jne    loc_1_83C7
        mov    eax, dword ptr [ebp - 0x1c]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x82]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x81]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        imul   eax, eax, 0x14
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x83]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 0x10
loc_1_83C7:
        jmp    loc_1_835F
loc_1_83CC:
        jmp    loc_1_8436
        db  0E9h, 060h, 000h, 000h, 000h
loc_1_83D6:
        cmp    dword ptr [ebp - 0x24], 0x40
        jg     loc_1_8409
        je     loc_1_82ED
        cmp    dword ptr [ebp - 0x24], 7
        je     loc_1_8266
        cmp    dword ptr [ebp - 0x24], 8
        je     loc_1_8289
        cmp    dword ptr [ebp - 0x24], 0xa
        je     loc_1_8289
        jmp    loc_1_8436
loc_1_8409:
        cmp    dword ptr [ebp - 0x24], 0x78
        je     loc_1_8303
        cmp    dword ptr [ebp - 0x24], 0x79
        je     loc_1_8316
        cmp    dword ptr [ebp - 0x24], 0x7b
        jl     loc_1_8436
        cmp    dword ptr [ebp - 0x24], 0x7f
        jle    loc_1_8353
        jmp    loc_1_8436
loc_1_8436:
        xor    eax, eax
        jmp    loc_1_84F0
loc_1_843D:
        mov    al, byte ptr [ebp - 4]
        movzx  ecx, byte ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x71], al
        xor    eax, eax
        jmp    loc_1_84F0
loc_1_8452:
        movzx  eax, byte ptr [ebp - 4]
        shl    eax, 2
        movzx  ecx, byte ptr [ebp - 0xc]
        shl    ecx, 9
        or     eax, ecx
        mov    word ptr [ebp - 0x14], ax
        movzx  eax, word ptr [ebp - 0x14]
        add    eax, 0x7fff
        movsx  eax, ax
        movzx  ecx, byte ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0x31], eax
        movzx  eax, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + eax*4 + 0x31]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_7C1C
        add    esp, 0xc
        jmp    loc_1_84F0
loc_1_849B:
        mov    eax, 1
        jmp    loc_1_84F0
        db  0E9h, 046h, 000h, 000h, 000h
loc_1_84AA:
        cmp    dword ptr [ebp - 0x20], 0x80
        je     loc_1_81D4
        cmp    dword ptr [ebp - 0x20], 0x90
        je     loc_1_812D
        cmp    dword ptr [ebp - 0x20], 0xb0
        je     loc_1_825A
        cmp    dword ptr [ebp - 0x20], 0xc0
        je     loc_1_843D
        cmp    dword ptr [ebp - 0x20], 0xe0
        je     loc_1_8452
        jmp    loc_1_849B
loc_1_84F0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_84F5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 4], 0x41e
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x26], 4
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x28], 0x12
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x2c], 0xffff
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x2e], 1
        mov    dword ptr [ebp - 4], 2
        cmp    dword ptr [ebp - 4], 1
        jne    loc_1_8586
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0x12d
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x2a], 9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        movzx  eax, word ptr [eax + 4]
        push   eax
        push   0                             ; -> loc_1_1EF4
        push   0                             ; -> loc_1_91FC
        mov    eax, dword ptr [ebp - 8]
        add    eax, 6
        push   eax
        call   sub_1_2403
        add    esp, 0x10
        jmp    loc_1_85BC
loc_1_8586:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 0x12e
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0x2a], 0x12
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        movzx  eax, word ptr [eax + 4]
        push   eax
        push   0                             ; -> loc_1_1EF4
        push   0                             ; -> loc_1_91FC
        mov    eax, dword ptr [ebp - 8]
        add    eax, 6
        push   eax
        call   sub_1_2403
        add    esp, 0x10
loc_1_85BC:
        xor    eax, eax
        jmp    loc_1_85C3
loc_1_85C3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_85C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x24], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x1e], eax
        mov    dword ptr [ebp - 0x22], 0x30
        push   dword ptr [ebp + 8]
        call   sub_1_305D
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_8606
        mov    eax, 1
        jmp    loc_1_8651
loc_1_8606:
        lea    eax, [ebp - 0x10]
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 0x14]
        call   sub_4_18D9
loc_1_8615:
        add    esp, 0xc
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_862D
        mov    eax, dword ptr [ebp - 0x18]
        jmp    loc_1_8651
loc_1_862D:
        lea    eax, [ebp - 0x24]
        push   eax
        push   dword ptr [ebp - 0x14]
        call   sub_4_26DA
loc_1_8639:
        add    esp, 8
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 0x14]
        call   sub_4_191D
loc_1_8647:
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_8651
loc_1_8651:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_8656:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        movzx  eax, word ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_8777
loc_1_8671:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        call   sub_1_63E1
        add    esp, 0xc
        jmp    loc_1_87C1
loc_1_8687:
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        call   sub_1_6531
        add    esp, 8
        jmp    loc_1_87C1
loc_1_869A:
        push   dword ptr [ebp + 0x14]
        push   0x10
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0x2c]
        call   sub_1_3846
        add    esp, 0xc
        xor    eax, eax
        jmp    loc_1_87C1
loc_1_86B4:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp - 4]
        call   sub_1_84F5
        add    esp, 8
        jmp    loc_1_87C1
loc_1_86C7:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp - 4]
        call   sub_1_80EA
        add    esp, 8
        jmp    loc_1_87C1
loc_1_86DA:
        xor    eax, eax
        jmp    loc_1_87C1
loc_1_86E1:
        xor    eax, eax
        jmp    loc_1_87C1
loc_1_86E8:
        mov    eax, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x221], 0
        je     loc_1_8726
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x21d], eax
        push   0xff
        push   dword ptr [ebp - 4]
        call   sub_1_7951
        add    esp, 8
        jmp    loc_1_87C1
        db  0E9h, 016h, 000h, 000h, 000h
loc_1_8726:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        call   sub_1_85C8
        add    esp, 0xc
        jmp    loc_1_87C1
loc_1_873C:
        xor    eax, eax
        jmp    loc_1_87C1
loc_1_8743:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], 0x41e
        xor    eax, eax
        jmp    loc_1_87C1
loc_1_8753:
        push   dword ptr [ebp - 4]
        call   sub_1_68A3
        add    esp, 4
        jmp    loc_1_87C1
loc_1_8763:
        mov    eax, 1
        jmp    loc_1_87C1
        db  0E9h, 048h, 000h, 000h, 000h, 0E9h, 043h, 000h, 000h, 000h
loc_1_8777:
        dec    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], 0xa
        ja     loc_1_8763
        mov    eax, dword ptr [ebp - 0xc]
        jmp    dword ptr [eax*4]             ; -> obj1:0x878E
        ; ---- dispatch table (11 entries) ----
        dd  loc_1_8671
        dd  loc_1_8687
        dd  loc_1_869A
        dd  loc_1_86B4
        dd  loc_1_86C7
        dd  loc_1_86DA
        dd  loc_1_86E1
        dd  loc_1_86E8
        dd  loc_1_873C
        dd  loc_1_8743
        dd  loc_1_8753
        db  033h, 0C0h, 0E9h, 000h, 000h, 000h, 000h
loc_1_87C1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_87C6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 0x10]
        xor    esi, esi
        mov    ecx, dword ptr [edi]
        test   cl, 1
        jne    loc_1_87E8
        mov    eax, 3
        jmp    loc_1_8A77
loc_1_87E8:
        movzx  eax, word ptr [ebp + 0xc]
        cmp    eax, 1
        je     loc_1_8A0E
        cmp    eax, 2
        je     loc_1_8A48
        test   cl, 2
        jne    loc_1_880D
        mov    eax, 5
        jmp    loc_1_8A77
loc_1_880D:
        sub    eax, 3
        cmp    eax, 0xa
        ja     loc_1_881C
        jmp    dword ptr [eax*4]             ; -> obj1:0x8A7E
loc_1_881C:
        mov    eax, 0xc
        jmp    loc_1_8A77
loc_1_8826:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_5B2A
        add    esp, 4
        test   eax, eax
        je     loc_1_8849
        mov    esi, 1
        jmp    loc_1_8A0A
loc_1_8849:
        cmp    word ptr [ebp + 0xc], 3
        jne    loc_1_8857
        push   0
        or     byte ptr [edi], 4
        jmp    loc_1_8859
loc_1_8857:
        push   1
loc_1_8859:
        mov    ecx, dword ptr [edi + 0x24]
        push   dword ptr [ebx + 4]
        push   dword ptr [ecx + 0x14]
        push   edi
        call   sub_1_8B9B
        add    esp, 0x10
        test   eax, eax
        je     loc_1_8874
        mov    esi, 1
loc_1_8874:
        mov    dword ptr [edi + 0xc], 0
        push   dword ptr [ebp - 8]
        jmp    loc_1_8A02
loc_1_8883:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_8DA5
        add    esp, 4
        test   eax, eax
        je     loc_1_88A1
        mov    esi, 1
loc_1_88A1:
        push   dword ptr [ebp - 8]
        jmp    loc_1_8A02
loc_1_88A9:
        push   edi
        call   sub_1_8E06
        add    esp, 4
        test   eax, eax
        je     loc_1_88BB
        mov    esi, 1
loc_1_88BB:
        and    byte ptr [edi], 0xfb
        jmp    loc_1_8A0A
loc_1_88C3:
        mov    eax, dword ptr [ebx]
        cmp    eax, 4
        je     loc_1_88DE
        cmp    eax, 5
        je     loc_1_88E6
        cmp    eax, 6
        je     loc_1_88F0
        cmp    eax, 7
        je     loc_1_88FB
        jmp    loc_1_8A0A
loc_1_88DE:
        mov    dword ptr [edi + 0x28], eax
        jmp    loc_1_8A0A
loc_1_88E6:
        mov    esi, 0xc
        jmp    loc_1_8A0A
loc_1_88F0:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 0xc], eax
        jmp    loc_1_8A0A
loc_1_88FB:
        mov    esi, 0xc
        jmp    loc_1_8A0A
loc_1_8905:
        mov    eax, dword ptr [ebx]
        cmp    eax, 1
        je     loc_1_891B
        cmp    eax, 3
        je     loc_1_8926
        cmp    eax, 6
        je     loc_1_8931
        jmp    loc_1_8A0A
loc_1_891B:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [ebx + 4], eax
        jmp    loc_1_8A0A
loc_1_8926:
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebx + 4], eax
        jmp    loc_1_8A0A
loc_1_8931:
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 4], eax
        jmp    loc_1_8A0A
loc_1_893C:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   dword ptr [ebx]
        push   dword ptr [ebx + 4]
        push   edi
        call   sub_1_8D8A
        add    esp, 0xc
        test   eax, eax
        je     loc_1_895F
        mov    esi, 1
loc_1_895F:
        push   dword ptr [ebp - 8]
        jmp    loc_1_8A02
loc_1_8967:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        lea    eax, [ebp - 1]
        push   eax
        push   dword ptr [ebx]
        push   edi
        call   sub_1_8D1B
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_898E
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 4], al
        jmp    loc_1_8993
loc_1_898E:
        mov    esi, 1
loc_1_8993:
        push   dword ptr [ebp - 8]
        jmp    loc_1_8A02
loc_1_8998:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_8E1E
        add    esp, 4
        test   eax, eax
        je     loc_1_89B6
        mov    esi, 1
loc_1_89B6:
        push   dword ptr [ebp - 8]
        jmp    loc_1_8A02
loc_1_89BB:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_8E3B
        add    esp, 4
        test   eax, eax
        je     loc_1_89D9
        mov    esi, 1
loc_1_89D9:
        push   dword ptr [ebp - 8]
        jmp    loc_1_8A02
loc_1_89DE:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_8E54
        add    esp, 4
        test   eax, eax
        je     loc_1_89FC
        mov    esi, 1
loc_1_89FC:
        or     byte ptr [edi], 4
        push   dword ptr [ebp - 8]
loc_1_8A02:
        call   sub_1_9B5B
        add    esp, 4
loc_1_8A0A:
        mov    eax, esi
        jmp    loc_1_8A77
loc_1_8A0E:
        test   cl, 2
        jne    loc_1_8A72
        or     cl, 2
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi], ecx
        mov    dword ptr [edi + 4], eax
        mov    edx, dword ptr [edi + 0x24]
        movzx  eax, word ptr [edx + 4]
        shl    eax, 0x10
        push   2
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edi + 0x18]
        inc    eax
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [edi + 0x1c], eax
        mov    dword ptr [ebx + 4], eax
        push   dword ptr [edi + 8]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_8A77
loc_1_8A48:
        test   cl, 2
        jne    loc_1_8A54
        mov    eax, 5
        jmp    loc_1_8A77
loc_1_8A54:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 4], eax
        jne    loc_1_8A72
        and    cl, 0xfd
        push   3
        mov    dword ptr [edi], ecx
        push   dword ptr [edi + 8]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_8A77
loc_1_8A72:
        mov    eax, 4
loc_1_8A77:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        ; ---- dispatch table (11 entries) ----
        dd  loc_1_8826
        dd  loc_1_8883
        dd  loc_1_88A9
        dd  loc_1_88C3
        dd  loc_1_8905
        dd  loc_1_893C
        dd  loc_1_8967
        dd  loc_1_8826
        dd  loc_1_8998
        dd  loc_1_89BB
        dd  loc_1_89DE
sub_1_8AAA:
        push   esi
        push   0xffff
        mov    esi, dword ptr [esp + 0xc]
        push   4
        push   dword ptr [esi + 0x20]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        mov    eax, 1
        jne    loc_1_8AE4
        push   0xffff
        push   dword ptr [esp + 0x10]
        push   dword ptr [esi + 0x20]
        call   sub_1_436F
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_8AE4:
        pop    esi
        ret    
sub_1_8AE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   0xffff
        push   5
        lea    esi, [edi + 0x20]
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8B13
        mov    eax, 1
        jmp    loc_1_8B95
loc_1_8B13:
        push   0xffff
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8B30
        mov    eax, 1
        jmp    loc_1_8B95
loc_1_8B30:
        push   0xffff
        push   dword ptr [ebp + 0x10]
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8B4D
        mov    eax, 1
        jmp    loc_1_8B95
loc_1_8B4D:
        push   0xffff
        push   3
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        mov    eax, 1
        jne    loc_1_8B95
        push   edi
        call   sub_1_913C
        add    esp, 4
        mov    eax, dword ptr [esi]
        mov    ax, word ptr [eax + 0x10]
        lea    ecx, [ebp - 1]
        mov    word ptr [ebp - 4], ax
        mov    dword ptr [ebp - 8], ecx
        mov    dx, word ptr [ebp - 4]
        in     al, dx
        mov    esi, dword ptr [ebp - 8]
        mov    byte ptr [esi], al
        mov    al, byte ptr [ebp - 1]
        sub    al, byte ptr [ebp + 0x10]
        cmp    al, 1
        sbb    eax, eax
        inc    eax
loc_1_8B95:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_8B9B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0x20
        push   0xffff
        push   1
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8BC9
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8BC9:
        push   0xffff
        lea    eax, [ebp - 1]
        push   eax
        push   dword ptr [esi]
        call   sub_1_43EF
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8BEA
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8BEA:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_8BFA
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8BFA:
        mov    di, word ptr [ebp + 0x10]
        push   0xffff
        dec    di
        push   edi
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8C1E
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8C1E:
        mov    ax, di
        push   0xffff
        shr    ax, 8
        push   eax
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8C43
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8C43:
        movzx  edi, di
        xor    bx, bx
        inc    edi
        test   edi, edi
        jle    loc_1_8C74
loc_1_8C4E:
        movzx  ecx, bx
        push   0xffff
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [ecx + eax]
        push   eax
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_8C9D
        inc    bx
        movzx  eax, bx
        cmp    eax, edi
        jl     loc_1_8C4E
loc_1_8C74:
        push   dword ptr [ebp + 8]
        call   sub_1_913C
        add    esp, 4
        lea    eax, [ebp - 1]
        push   0xffff
        push   eax
        push   dword ptr [esi]
        call   sub_1_43EF
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8CA4
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8C9D:
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8CA4:
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_8CB1
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8CB1:
        cmp    dword ptr [ebp + 0x14], 1
        jne    loc_1_8D12
        push   dword ptr [ebp + 8]
        call   sub_1_913C
        add    esp, 4
        push   0xffff
        push   3
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8CDE
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8CDE:
        push   dword ptr [ebp + 8]
        call   sub_1_913C
        add    esp, 4
        lea    eax, [ebp - 1]
        push   0xffff
        push   eax
        push   dword ptr [esi]
        call   sub_1_43EF
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8D07
        mov    eax, 1
        jmp    loc_1_8D14
loc_1_8D07:
        cmp    byte ptr [ebp - 1], 0x55
        mov    eax, 1
        jne    loc_1_8D14
loc_1_8D12:
        xor    eax, eax
loc_1_8D14:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_8D1B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0xd0
        push   dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_8AE6
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8D41
        mov    eax, 1
        jmp    loc_1_8D85
loc_1_8D41:
        push   esi
        call   sub_1_913C
        add    esp, 4
        push   0xffff
        push   3
        push   dword ptr [esi + 0x20]
        call   sub_1_436F
        add    esp, 0xc
        test   eax, eax
        mov    eax, 1
        jne    loc_1_8D85
        push   esi
        call   sub_1_913C
        add    esp, 4
        mov    eax, dword ptr [esi + 0x20]
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        mov    esi, dword ptr [ebp + 0x10]
        mov    byte ptr [esi], al
        xor    eax, eax
loc_1_8D85:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_8D8A:
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_1_8AE6
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret    
sub_1_8DA5:
        push   esi
        push   0x80
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_1_8AAA
        add    esp, 8
        test   eax, eax
        je     loc_1_8DC3
        mov    eax, 1
        jmp    loc_1_8E04
loc_1_8DC3:
        push   0x70
        push   esi
        call   sub_1_8AAA
        add    esp, 8
        test   eax, eax
        je     loc_1_8DD9
        mov    eax, 1
        jmp    loc_1_8E04
loc_1_8DD9:
        push   dword ptr [esi + 0x28]
        push   esi
        call   sub_1_8E83
        add    esp, 8
        test   eax, eax
        mov    eax, 1
        jne    loc_1_8E04
        push   0x80
        push   0
        push   esi
        call   sub_1_8AE6
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_8E04:
        pop    esi
        ret    
sub_1_8E06:
        push   0x80
        push   dword ptr [esp + 8]
        call   sub_1_8AAA
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret    
sub_1_8E1E:
        push   0x80
        push   0xff
        push   dword ptr [esp + 0xc]
        call   sub_1_8AE6
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret    
sub_1_8E3B:
        push   0x80
        push   0
        push   dword ptr [esp + 0xc]
        call   sub_1_8AE6
        add    esp, 0xc
        mov    eax, 1
        ret    
sub_1_8E54:
        push   0x80
        push   dword ptr [esp + 8]
        call   sub_1_8AAA
        add    esp, 8
        test   eax, eax
        mov    eax, 1
        jne    loc_1_8E82
        push   0x70
        push   dword ptr [esp + 8]
        call   sub_1_8AAA
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_8E82:
        ret    
sub_1_8E83:
        xor    al, al
        cmp    word ptr [esp + 8], 1
        jne    loc_1_8E8F
        mov    al, 0x11
loc_1_8E8F:
        push   0x81
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_1_8AE6
        add    esp, 0xc
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret    
sub_1_8EA8:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_1_8EB5:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        xor    ebx, ebx
        mov    dword ptr [esi + 8], eax
        or     byte ptr [esi], 1
        mov    dword ptr [esi + 4], ebx
        mov    dword ptr [esi + 0xc], ebx
        mov    dword ptr [esi + 0x14], ebx
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x18], ebx
        push   ebx
        mov    dword ptr [esi + 0x1c], ebx
        push   ebx
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x10], 0x1200000
        mov    dword ptr [esi + 0x28], 2
        push   esi
        call   sub_1_8AE6
        add    esp, 0xc
        push   0xfc
        push   esi
        mov    edi, 0xffff
        call   sub_1_8AAA
        add    esp, 8
        push   0xfa
        push   esi
        call   sub_1_8AAA
        add    esp, 8
loc_1_8F1C:
        push   edi
        push   0xf
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        push   edi
        push   0x83
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        movzx  eax, bl
        add    esp, 0xc
        inc    bl
        lea    ecx, [ebp + eax - 8]
        push   edi
        push   ecx
        mov    dword ptr [ebp - 0xc], ecx
        push   dword ptr [ebp + 0x10]
        call   sub_1_43EF
        add    esp, 0xc
        push   edi
        push   0xe
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        push   edi
        push   0x83
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 0xc]
        push   edi
        mov    al, byte ptr [ecx]
        not    al
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        cmp    bl, 4
        jb     loc_1_8F1C
        push   0xfc
        push   esi
        xor    bl, bl
        call   sub_1_8AAA
        add    esp, 8
        mov    edi, 0xffff
        push   0xf9
        push   esi
        call   sub_1_8AAA
        add    esp, 8
loc_1_8FAF:
        push   edi
        push   0xf
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        push   edi
        push   0x83
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        lea    eax, [ebp - 1]
        push   edi
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_43EF
        movzx  ecx, bl
        add    esp, 0xc
        mov    al, byte ptr [ebp + ecx - 8]
        not    al
        cmp    al, byte ptr [ebp - 1]
        jne    loc_1_90EF
        inc    bl
        cmp    bl, 4
        jb     loc_1_8FAF
        push   0xfc
        push   esi
        xor    bl, bl
        call   sub_1_8AAA
        add    esp, 8
        mov    edi, 0xffff
        push   0xfa
        push   esi
        call   sub_1_8AAA
        add    esp, 8
loc_1_901A:
        push   edi
        push   0xe
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        push   edi
        push   0x83
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        movzx  eax, bl
        add    esp, 0xc
        inc    bl
        push   edi
        push   dword ptr [ebp + eax - 8]
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        cmp    bl, 4
        jb     loc_1_901A
        push   0xfc
        push   esi
        xor    bl, bl
        call   sub_1_8AAA
        add    esp, 8
        mov    edi, 0xffff
        push   0xf9
        push   esi
        call   sub_1_8AAA
        add    esp, 8
loc_1_9076:
        push   edi
        push   0xf
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        push   edi
        push   0x83
        push   dword ptr [ebp + 0x10]
        call   sub_1_436F
        add    esp, 0xc
        lea    eax, [ebp - 1]
        push   edi
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_43EF
        movzx  ecx, bl
        add    esp, 0xc
        mov    dl, byte ptr [ebp - 1]
        cmp    byte ptr [ebp + ecx - 8], dl
        jne    loc_1_9109
        inc    bl
        cmp    bl, 4
        jb     loc_1_9076
        push   0
        push   esi
        call   sub_1_8AAA
        add    esp, 8
        push   0xffff
        push   dword ptr [ebp + 0x10]
        call   sub_1_4288
        add    esp, 8
        mov    edi, dword ptr [ebp + 0x14]
        push   4
        push   edi
        call   sub_1_5272
        add    esp, 8
        test   eax, eax
        je     loc_1_9123
        mov    eax, 1
        and    byte ptr [esi], 0xfe
        jmp    loc_1_9128
loc_1_90EF:
        push   0xffff
        and    byte ptr [esi], 0xfe
        push   dword ptr [ebp + 0x10]
        call   sub_1_4288
        add    esp, 8
        mov    eax, 1
        jmp    loc_1_9128
loc_1_9109:
        push   0xffff
        and    byte ptr [esi], 0xfe
        push   dword ptr [ebp + 0x10]
        call   sub_1_4288
        add    esp, 8
        mov    eax, 1
        jmp    loc_1_9128
loc_1_9123:
        xor    eax, eax
        mov    dword ptr [esi + 0x24], edi
loc_1_9128:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_912F:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_1_913C:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ecx, dword ptr [eax + 0x20]
        mov    dx, word ptr [ecx + 4]
        mov    word ptr [esp + 2], dx
        mov    cx, 0x6a4
        mov    dx, word ptr [esp + 2]
        add    dx, 5
loc_1_915C:
        in     al, dx
        dec    cx
        cmp    cx, 0
        jne    loc_1_915C
        add    esp, 4
        ret    
        db  0CCh, 0CCh, 0CCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0BFh, 000h, 000h, 000h
sub_1_9178:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dd  VSB16_Service_00         ; fixup
        dd  VSB16_Service_01         ; fixup
        dd  VSB16_Service_02         ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  032A5h                      ; DDB_Req_Device_Number
        db  4, 30                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VSB16   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VSB16_Control            ; DDB_Control_Proc
        dd  VSB16_PM_API             ; DDB_V86_API_Proc
        dd  VSB16_PM_API             ; DDB_PM_API_Proc
        dd  obj1:0x917C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_91FC:
        and    eax, 0x255b2073
        js     0x9260
        add    byte ptr [eax], al
        int3   
        int3   
        int3   
loc_1_9208:
        cmp    dword ptr [esp + 4], 1
        je     loc_1_9219
        mov    eax, 0xffffffff
        jmp    loc_1_92B4
loc_1_9219:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_1_92B2
        cmp    eax, 1
        je     loc_1_9240
        cmp    eax, 2
        je     loc_1_925C
        cmp    eax, 3
        je     loc_1_929B
        cmp    eax, 4
        je     loc_1_92A9
        mov    eax, 0xffffffff
        jmp    loc_1_92B4
loc_1_9240:
        mov    byte ptr [0], 0xbf            ; -> obj1:0x9174
        xor    eax, eax
        mov    word ptr [0], ax              ; -> obj1:0x916C
        mov    dword ptr [0], eax            ; -> sub_1_9178
        mov    word ptr [0], ax              ; -> obj1:0x9170
        jmp    loc_1_92B2
loc_1_925C:
        mov    word ptr [0], 0               ; -> obj1:0x9170
        call   sub_1_301E
        test   eax, eax
        je     loc_1_9292
loc_1_926E:
        test   byte ptr [eax + 0xd4], 2
        je     loc_1_9285
        mov    cx, word ptr [eax + 0x683]
        mov    word ptr [0], cx              ; -> obj1:0x9170
loc_1_9285:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    loc_1_926E
loc_1_9292:
        movzx  eax, word ptr [0]             ; -> obj1:0x9170
        jmp    loc_1_92B4
loc_1_929B:
        mov    ax, word ptr [0]              ; -> obj1:0x9170
        mov    word ptr [0], ax              ; -> obj1:0x916C
        jmp    loc_1_92B2
loc_1_92A9:
        mov    word ptr [0], 0               ; -> obj1:0x916C
loc_1_92B2:
        xor    eax, eax
loc_1_92B4:
        ret    
sub_1_92B5:
        push   ebp
        mov    ax, word ptr [esp + 0xc]
        mov    ebp, esp
        cmp    word ptr [0], ax              ; -> obj1:0x916C
        jne    loc_1_92F2
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_92DB
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [0], 0xbf            ; -> obj1:0x9174
        mov    byte ptr [eax], 0xfe
        jmp    loc_1_9319
loc_1_92DB:
        mov    ecx, dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        movzx  edx, byte ptr [ecx]
        push   edx
        push   5
        call   sub_1_9178
        add    esp, 0xc
        jmp    loc_1_9319
loc_1_92F2:
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_9305
        mov    cl, byte ptr [0]              ; -> obj1:0x9174
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [eax], cl
        jmp    loc_1_9319
loc_1_9305:
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax]
        cmp    al, 0x3f
        je     loc_1_9312
        cmp    al, 0xff
        jne    loc_1_9319
loc_1_9312:
        mov    byte ptr [0], 0x3f            ; -> obj1:0x9174
loc_1_9319:
        pop    ebp
        ret    
        db  0CCh
sub_1_931C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        mov    eax, 0
        clc    
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi], ecx
        mov    ax, 0
        clc    
        pop    ebx
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Convert_Handle_To_IRQ ; cd200c000300
        mov    eax, esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        mov    eax, 0
        clc    
        pop    esi
        leave  
        ret    
sub_1_936C:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        mov    eax, 0
        clc    
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    eax, 0
        clc    
        pop    ebx
        leave  
        ret    
sub_1_939E:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        mov    eax, 0
        clc    
        leave  
        ret    
sub_1_93B2:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        mov    eax, 0
        clc    
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Set_Auto_Masking    ; cd200a000300
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        VMMCall  VMM._Deallocate_Device_CB_Area  ; cd2015010100
        add    esp, 8
        or     eax, eax
        je     loc_1_93F1
        mov    eax, 0
        clc    
        jmp    loc_1_93F7
loc_1_93F1:
        stc    
        mov    eax, 1
loc_1_93F7:
        leave  
        ret    
sub_1_93F9:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_9422
        mov    esi, dword ptr [ebp + 0x10]
        mov    word ptr [esi], ax
        mov    eax, 0
        clc    
        jmp    loc_1_9428
loc_1_9422:
        mov    eax, 1
        stc    
loc_1_9428:
        pop    esi
        leave  
        ret    
sub_1_942B:
        push   ebp
        mov    ebp, esp
        movzx  eax, word ptr [ebp + 8]
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        leave  
        ret    
sub_1_9440:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 0x14]
        push   ecx
        push   eax
        VxDCall  VDMAD.VDMAD_Get_Max_Phys_Page   ; cd201e000400
        mov    edx, 0xfff
        cmp    eax, edx
        jae    loc_1_9463
        mov    edx, eax
loc_1_9463:
        pop    eax
        push   0xe
        push   ebx
        push   edx
        push   0
        push   eax
        push   0
        push   1
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        pop    ecx
        or     eax, eax
        je     loc_1_9498
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [ebp + 0x18]
        shl    ecx, 0xc
        mov    dword ptr [esi], ecx
        mov    esi, dword ptr [ebp + 0x1c]
        mov    dword ptr [esi], edx
        mov    eax, 0
        clc    
        jmp    loc_1_949E
loc_1_9498:
        mov    eax, 1
        stc    
loc_1_949E:
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave  
        ret    
sub_1_94A4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        leave  
        ret    
sub_1_94B8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   0
        push   0
        push   0x92
        push   ecx
        push   edx
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        or     eax, eax
        je     loc_1_94EF
        mov    esi, dword ptr [ebp + 0x14]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi], edx
        mov    eax, 0
        clc    
        jmp    loc_1_94F5
loc_1_94EF:
        mov    eax, 1
        stc    
loc_1_94F5:
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        jb     loc_1_9517
        mov    eax, 0
        clc    
        jmp    loc_1_951D
loc_1_9517:
        mov    eax, 1
        stc    
loc_1_951D:
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        jb     loc_1_9538
        mov    eax, 0
        clc    
        jmp    loc_1_953E
loc_1_9538:
        mov    eax, 1
        stc    
loc_1_953E:
        leave  
        ret    
sub_1_9540:
        push   ebp
        mov    ebp, esp
        push   esi
        movzx  edx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_1_955B
        mov    eax, 0
        clc    
        jmp    loc_1_9561
loc_1_955B:
        mov    eax, 1
        stc    
loc_1_9561:
        pop    esi
        leave  
        ret    
sub_1_9564:
        push   ebp
        mov    ebp, esp
        movzx  edx, word ptr [ebp + 8]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        mov    eax, 0
        clc    
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Install_Mult_IO_Handlers    ; cd2095000100
        jb     loc_1_9590
        mov    eax, 0
        clc    
        jmp    loc_1_9596
loc_1_9590:
        mov    eax, 1
        stc    
loc_1_9596:
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi], ebx
        pop    ebx
        pop    esi
        leave  
        ret    
sub_1_95AD:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_1_95CA
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc    
        jmp    loc_1_95D0
loc_1_95CA:
        mov    eax, 1
        stc    
loc_1_95D0:
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_95D4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        mov    eax, 0
        clc    
        leave  
        ret    
loc_1_95E8:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [ebp + 0xc]
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        pop    ebx
        leave  
        ret    
sub_1_95FE:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [ebp + 0xc]
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        pop    ebx
        leave  
        ret    
sub_1_9614:
        push   ebp
        mov    ebp, esp
        push   esi
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi], ax
        clc    
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012000400
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
        jb     loc_1_9655
        mov    eax, 0
        clc    
        jmp    loc_1_965B
loc_1_9655:
        mov    eax, 1
        stc    
loc_1_965B:
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
        jb     loc_1_9678
        mov    eax, 0
        clc    
        jmp    loc_1_967E
loc_1_9678:
        mov    eax, 1
        stc    
loc_1_967E:
        pop    ebx
        leave  
        ret    
sub_1_9681:
        push   ebp
        mov    ebp, esp
        push   esi
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        jb     loc_1_96A2
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc    
        jmp    loc_1_96A8
loc_1_96A2:
        mov    eax, 1
        stc    
loc_1_96A8:
        pop    esi
        leave  
        ret    
sub_1_96AB:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
        mov    eax, 0
        clc    
        leave  
        ret    
sub_1_96BF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    ebx, 1
        sbb    ebx, ebx
        inc    bh
        mov    bl, 0
        mov    edx, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        mov    eax, 0
        clc    
        pop    esi
        pop    ecx
        pop    ebx
        leave  
        ret    
sub_1_96EE:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp + 0x10]
        mov    dh, 0
        VxDCall  VDMAD.VDMAD_Set_Phys_State      ; cd2006000400
        mov    eax, 0
        clc    
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_UnMask_Channel      ; cd2008000400
        mov    eax, 0
        clc    
        pop    ebx
        leave  
        ret    
sub_1_9725:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Unmask_Channel ; cd2019000400
        mov    eax, 0
        clc    
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Mask_Channel        ; cd2007000400
        mov    eax, 0
        clc    
        pop    ebx
        leave  
        ret    
sub_1_9757:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
        mov    eax, 0
        clc    
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx], esi
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx], ecx
        mov    ebx, dword ptr [ebp + 0x18]
        mov    byte ptr [ebx], dl
        mov    eax, 0
        clc    
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        movzx  dx, byte ptr [ebp + 0x18]
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
        mov    eax, 0
        clc    
        pop    esi
        pop    ecx
        pop    ebx
        leave  
        ret    
sub_1_97C6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        mov    esi, dword ptr [ebp + 0x10]
        mov    word ptr [esi], cx
        mov    eax, 0
        clc    
        pop    esi
        pop    ecx
        pop    ebx
        leave  
        ret    
sub_1_97E9:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd 
        cli    
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_1_9808
        popfd  
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc    
        jmp    loc_1_980F
loc_1_9808:
        popfd  
        mov    eax, 1
        stc    
loc_1_980F:
        pop    esi
        leave  
        ret    
sub_1_9812:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd 
        cli    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        popfd  
        pop    esi
        leave  
        ret    
sub_1_9828:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 5
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_1_984A
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx], esi
        mov    eax, 0
        clc    
        jmp    loc_1_9850
loc_1_984A:
        mov    eax, 1
        stc    
loc_1_9850:
        pop    ecx
        pop    ebx
        leave  
        ret    
sub_1_9854:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.List_Destroy                ; cd209c000100
        pop    esi
        leave  
        ret    
sub_1_9864:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd 
        cli    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        popfd  
        pop    esi
        leave  
        ret    
sub_1_987A:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd 
        cli    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        popfd  
        pop    esi
        leave  
        ret    
sub_1_9890:
        push   ebp
        mov    ebp, esp
        push   esi
        movzx  eax, word ptr [ebp + 8]
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_98B5
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc    
        jmp    loc_1_98BB
loc_1_98B5:
        mov    eax, 1
        stc    
loc_1_98BB:
        pop    esi
        leave  
        ret    
sub_1_98BE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        leave  
        ret    
sub_1_98D2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, 0x15000e
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_1_98ED
        mov    eax, 0
        jmp    loc_1_98F2
loc_1_98ED:
        mov    eax, 1
loc_1_98F2:
        pop    esi
        leave  
        ret    
sub_1_98F5:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, 0x15000e
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        jb     loc_1_9910
        mov    eax, 0
        jmp    loc_1_9915
loc_1_9910:
        mov    eax, 1
loc_1_9915:
        pop    esi
        leave  
        ret    
        db  050h, 053h, 0CDh, 020h, 03Fh, 000h, 001h, 000h, 08Bh, 0D8h, 0CDh, 020h, 03Fh, 000h, 001h, 000h
        db  03Bh, 0C3h, 074h, 0F6h, 05Bh, 058h, 0C3h, 0CCh
VSB16_Service_00:
        mov    eax, 0x101
        cmp    ecx, 0x4354454d
        jne    loc_1_994F
        mov    ecx, 0x454d4354
        mov    dword ptr [0], edx            ; -> sub_1_9178
        mov    edx, 0                        ; -> loc_1_9208
        clc    
        ret    
loc_1_994F:
        mov    ah, 4
        mov    al, 0x1e
        clc    
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_9958:
        push   edi
        push   ecx
        push   eax
        push   ebx
        call   sub_1_2C34
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_996B
        clc    
        pop    edi
        ret    
loc_1_996B:
        stc    
        pop    edi
        ret    
        db  08Bh, 0C0h
loc_1_9970:
        push   edi
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        pop    edi
        ret    
        db  090h
loc_1_9980:
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012000400
        clc    
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  obj1:0x91D8              ; fixup
loc_1_9990:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_99A5
        xor    edx, edx
        push   edx
        push   ebx
        call   sub_1_2C8E
        pop    ecx
        pop    ecx
loc_1_99A5:
        popfd  
        popal  
        jmp    dword ptr [0]                 ; -> obj1:0x91D8
loc_1_99AD:
        pushal 
        or     edx, 0xf000000
        push   edx
        push   ebx
        call   sub_1_2C8E
        pop    ecx
        pop    ecx
        popal  
        clc    
        ret    
VSB16_Control:
        cmp    eax, 0xb
        je     loc_1_99AD
        cmp    eax, 0x1b
        je     loc_1_9B64
        cmp    eax, 0x1c
        je     loc_1_9B71
        cmp    eax, 0x22
        je     loc_1_9C6C
        cmp    eax, 0x23
        je     loc_1_9C31
        clc    
        ret    
        db  090h
loc_1_99EC:
        push   edi
        push   edx
        push   eax
        push   ebx
        call   sub_1_288E
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_99FF
        pop    edi
        clc    
        ret    
loc_1_99FF:
        pop    edi
        stc    
        ret    
VSB16_PM_API:
        push   ebp
        push   eax
        movzx  eax, word ptr [ebp + 0x38]
        or     ax, ax
        pop    eax
        je     loc_1_9A18
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
loc_1_9A18:
        push   eax
        movzx  eax, word ptr [ebp + 4]
        push   eax
        mov    ax, word ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x1c]
        push   eax
        movzx  eax, word ptr [ebp + 0x14]
        push   eax
        movzx  eax, word ptr [ebp + 0x10]
        push   eax
        mov    eax, ebx
        push   eax
        call   sub_1_25C4
        add    esp, 0x18
        pop    ebp
        mov    ebx, eax
        mov    word ptr [ebp + 0x1c], ax
        mov    cl, 0x10
        shr    eax, cl
        mov    word ptr [ebp + 0x14], ax
        or     ebx, ebx
        jne    loc_1_9A5A
        and    dword ptr [ebp + 0x2c], 0xfffe
        ret    
loc_1_9A5A:
        or     dword ptr [ebp + 0x2c], 1
        ret    
        db  090h
sub_1_9A60:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 1
        pushfd 
        cli    
        xchg   dword ptr [esi + 8], eax
        or     eax, eax
        jne    loc_1_9A92
        popfd  
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 0                        ; -> loc_1_9A98
        mov    ecx, 1
        mov    eax, 0x400000
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        pushfd 
loc_1_9A92:
        popfd  
        popal  
        leave  
        ret    
        db  08Bh, 0C0h
loc_1_9A98:
        pushal 
        pushfd 
        mov    edi, edx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_1_9AB1:
        pushfd 
        cli    
        mov    ebx, dword ptr [edi + 4]
        cmp    ebx, dword ptr [edi]
        je     loc_1_9B2F
        mov    esi, dword ptr [edi + ebx*4 + 0xc]
        xor    edx, edx
        mov    eax, ebx
        inc    eax
        mov    ebx, 0x20
        div    bx
        mov    dword ptr [edi + 4], edx
        popfd  
        mov    eax, dword ptr [esi + 0x14]
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        movzx  eax, word ptr [esi + 0x12]
        mov    word ptr [ebp + 0x10], ax
        movzx  eax, word ptr [esi + 0x10]
        mov    word ptr [ebp + 0x14], ax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 4], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        movzx  edx, word ptr [esi + 0xe]
        mov    cx, word ptr [esi + 0xc]
        push   esi
        push   edi
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    edi
        pop    esi
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_1_9AB1
        push   dword ptr [esi + 0x14]
        call   eax
        pop    eax
        jmp    loc_1_9AB1
loc_1_9B2F:
        xor    eax, eax
        xchg   dword ptr [edi + 8], eax
        popfd  
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popfd  
        popal  
        ret    
sub_1_9B4D:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        pushfd 
        pop    dword ptr [esi]
        cli    
        pop    esi
        leave  
        ret    
sub_1_9B5B:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        popfd  
        leave  
        ret    
loc_1_9B64:
        call   sub_1_308F
        or     eax, eax
        jne    loc_1_9B6F
        clc    
        ret    
loc_1_9B6F:
        stc    
        ret    
loc_1_9B71:
        call   sub_1_30D0
        or     eax, eax
        jne    loc_1_9B7C
        clc    
        ret    
loc_1_9B7C:
        stc    
        ret    
        db  08Bh, 0C0h
loc_1_9B80:
        cmp    ecx, 4
        jbe    loc_1_9B8B
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_9B8B:
        je     loc_1_9B94
        push   eax
        mov    eax, esp
        push   1
        jmp    loc_1_9B99
loc_1_9B94:
        push   eax
        mov    eax, esp
        push   0
loc_1_9B99:
        push   eax
        push   edx
        push   ebx
        call   sub_1_3109
        add    esp, 0x10
        pop    eax
        ret    
        db  08Bh, 0C0h
loc_1_9BA8:
        cmp    ecx, 4
        jbe    loc_1_9BB3
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_9BB3:
        je     loc_1_9BBC
        push   eax
        mov    eax, esp
        push   1
        jmp    loc_1_9BC1
loc_1_9BBC:
        push   eax
        mov    eax, esp
        push   0
loc_1_9BC1:
        push   eax
        push   edx
        push   ebx
        call   sub_1_338D
        add    esp, 0x10
        pop    eax
        ret    
        db  066h, 081h, 07Dh, 01Ch, 0F0h, 091h, 075h, 032h, 0CDh, 020h, 004h, 000h, 001h, 000h, 074h, 02Ah
        db  052h, 051h, 056h, 0CDh, 020h, 083h, 000h, 001h, 000h, 0BEh, 054h, 000h, 000h, 000h, 08Bh, 00Eh
        db  08Bh, 0D1h, 0C1h, 0C1h, 010h, 0CDh, 020h, 04Ch, 000h, 001h, 000h, 0CDh, 020h, 085h, 000h, 001h
        db  000h, 0CDh, 020h, 086h, 000h, 001h, 000h, 05Eh, 059h, 05Ah, 0F9h, 0C3h, 08Bh, 0C0h
sub_1_9C0C:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        pushfd 
        cli    
        mov    ebx, dword ptr [edi]
        mov    dword ptr [edi + ebx*4 + 0xc], eax
        xor    edx, edx
        mov    eax, ebx
        inc    eax
        mov    ebx, 0x20
        div    bx
        mov    dword ptr [edi], edx
        popfd  
        popal  
        leave  
        ret    
loc_1_9C31:
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_1_3A60
        add    esp, 0x10
        ret    
sub_1_9C3E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   0
        push   word ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_9C66
        movzx  ebx, word ptr [ebp + 0x10]
        add    eax, ebx
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx], eax
        xor    eax, eax
loc_1_9C66:
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
loc_1_9C6C:
        pushal 
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_1_4869
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        popal  
        mov    eax, 0
        stc    
        ret    
sub_1_9C82:
        push   ecx
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        mov    dword ptr [0], ecx            ; -> obj1:0x91E0
        movzx  ecx, word ptr [0xe]           ; -> loc_2_000E
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        mov    dword ptr [0], ecx            ; -> obj1:0x91E4
        movzx  ecx, word ptr [0x10]          ; -> loc_2_0010
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        mov    dword ptr [0], ecx            ; -> obj1:0x91E8
        movzx  ecx, word ptr [0x12]          ; -> loc_2_0012
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        mov    dword ptr [0], ecx            ; -> obj1:0x91EC
        movzx  ecx, word ptr [0x14]          ; -> obj2:0x0014
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        mov    dword ptr [0], ecx            ; -> obj1:0x91F0
        pop    ecx
        ret    
sub_1_9CE4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    dx, word ptr [ebp + 8]
        in     al, dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [esi], al
        pop    esi
        leave  
        ret    
sub_1_9CF5:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        leave  
        ret    
sub_1_9D02:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    dx, word ptr [ebp + 8]
        in     ax, dx
        mov    esi, dword ptr [ebp + 0xc]
        mov    word ptr [esi], ax
        pop    esi
        leave  
        ret    
sub_1_9D15:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0xc]
        out    dx, ax
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        pushal 
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dx, word ptr [ebp + 8]
        mov    word ptr [0], dx              ; -> obj1:0x91F4
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [0], edx            ; -> obj1:0x91F6
        mov    dx, word ptr [ebp + 0x10]
        mov    word ptr [0], dx              ; -> obj1:0x91FA
        mov    esi, 0                        ; -> loc_1_9D65
        mov    ecx, 3
        mov    eax, 0x10
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        popal  
        leave  
        ret    
loc_1_9D65:
        pushal 
        sub    esp, 0x6c
        mov    edi, esp
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x1607
        mov    dx, word ptr [0]              ; -> obj1:0x91F4
        mov    word ptr [ebp + 0x10], dx
        movzx  ecx, word ptr [0]             ; -> obj1:0x91FA
        sub    word ptr [ebp + 0x30], cx
        xchg   edi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    esi, dword ptr [0]            ; -> obj1:0x91F6
        mov    cx, word ptr [0]              ; -> obj1:0x91FA
        cld    
        rep movsw word ptr es:[edi], word ptr [esi]
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  ecx, word ptr [0]             ; -> obj1:0x91FA
        add    word ptr [ebp + 0x30], cx
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        ret    
sub_1_9DDD:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        and    eax, 4
        shr    eax, 2
        ret    
sub_1_9DEB:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        test   al, 1
        jne    loc_1_9DFE
        mov    eax, 1
        jmp    loc_1_9E41
loc_1_9DFE:
        cmp    dword ptr [esi + 8], 0
        jne    loc_1_9E14
        push   esi
        call   sub_1_A0F0
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_9E41
loc_1_9E14:
        test   al, 4
        je     loc_1_9E31
        push   esi
        call   sub_1_A0F0
        add    esp, 4
        push   esi
        call   sub_1_A122
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_9E41
loc_1_9E31:
        mov    ax, word ptr [esi + 0x10]
        sub    ax, word ptr [esp + 0xc]
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_1_9E41:
        pop    esi
        ret    
sub_1_9E43:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0xc0]
        lea    ecx, [esi + 0xb8]
        cmp    dword ptr [ecx], 0
        je     loc_1_9EC1
        mov    dword ptr [edi + 0x14], esi
        mov    word ptr [edi + 0x12], 1
        mov    word ptr [edi + 0x10], 1
        mov    eax, dword ptr [esi + 0xbc]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ecx]
        shr    eax, 0x10
        lea    edx, [ebp - 4]
        mov    word ptr [edi + 0xc], ax
        push   edx
        mov    eax, dword ptr [ecx]
        mov    word ptr [edi + 0xe], ax
        mov    ecx, dword ptr [esi + 0x1a]
        mov    dword ptr [edi + 4], ecx
        mov    dword ptr [edi], 0            ; -> sub_1_A122
        push   dword ptr [esi + 0xe8]
        call   sub_1_39B0
        add    esp, 8
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_1_9C0C
        add    esp, 8
        push   dword ptr [esi + 0xb4]
        push   dword ptr [ebp - 4]
        call   sub_1_9A60
        add    esp, 8
loc_1_9EC1:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_9EC9:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], 0
        xor    eax, eax
        ret    
sub_1_9ED7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        xor    edi, edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 8], edi
        cmp    dword ptr [ebp + 0x10], edi
        mov    dword ptr [esi], edi
        mov    dword ptr [esi + 0xe8], eax
        mov    dword ptr [esi + 0xe4], edi
        je     loc_1_A02A
        cmp    dword ptr [ebp + 0x18], edi
        je     loc_1_A02A
        mov    dword ptr [esi + 0x54], 0x300000c
        mov    word ptr [esi + 0x58], 2
        mov    word ptr [esi + 0x5a], 0x68
        lea    eax, [ebp - 2]
        push   eax
        mov    ebx, 0                        ; -> loc_1_91FC
        push   dword ptr [ebp + 0x10]
        call   sub_1_4807
        add    esp, 8
        lea    eax, [esi + 0x5c]
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        push   dword ptr [0]                 ; -> obj1:0x91E8
        push   ebx
        push   eax
        call   sub_1_2403
        mov    word ptr [esi + 0x86], 2
        add    esp, 0x10
        lea    edx, [esi + 0x8a]
        mov    word ptr [esi + 0x88], 4
        movzx  eax, word ptr [ebp - 2]
        push   eax
        push   dword ptr [0]                 ; -> obj1:0x91E4
        push   ebx
        push   edx
        call   sub_1_2403
        add    esp, 0x10
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    word ptr [ecx + 6], 4
        jae    loc_1_9FBB
        mov    eax, 0x333
        lea    edx, [esi + 0x84]
        lea    ecx, [esi + 0xaa]
        mov    dword ptr [esi + 0x7c], eax
        mov    dword ptr [esi + 0x80], edi
        mov    word ptr [edx], 2
        mov    dword ptr [ecx], eax
        lea    eax, [esi + 0xb2]
        mov    dword ptr [esi + 0xae], edi
        mov    word ptr [eax], 2
        mov    dword ptr [esi], 1
        jmp    loc_1_9FF2
loc_1_9FBB:
        mov    eax, 0xccc
        xor    ebx, ebx
        lea    edx, [esi + 0x84]
        mov    edi, 2
        lea    ecx, [esi + 0xaa]
        mov    dword ptr [esi + 0x7c], eax
        mov    dword ptr [esi + 0x80], ebx
        mov    word ptr [edx], di
        mov    dword ptr [ecx], eax
        lea    eax, [esi + 0xb2]
        mov    dword ptr [esi + 0xae], ebx
        mov    word ptr [eax], di
        mov    dword ptr [esi], edi
loc_1_9FF2:
        cmp    dword ptr [ebp + 0x1c], 0
        je     loc_1_A00B
        mov    edi, 0xccc
        or     dword ptr [esi + 0x7c], edi
        or     byte ptr [edx], 2
        or     dword ptr [ecx], edi
        or     byte ptr [eax], 2
        or     byte ptr [esi], 2
loc_1_A00B:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 0x18]
        mov    dword ptr [esi + 0xd8], eax
        mov    dword ptr [esi + 0xdc], ecx
        mov    dword ptr [esi + 0xe0], edx
        or     byte ptr [esi + 4], 1
loc_1_A02A:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_A033:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 4], 1
        jne    loc_1_A044
        mov    eax, 1
        jmp    loc_1_A04D
loc_1_A044:
        mov    dword ptr [eax + 4], 0
        xor    eax, eax
loc_1_A04D:
        ret    
sub_1_A04E:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        test   esi, 1
        jne    loc_1_A065
        mov    eax, 1
        jmp    loc_1_A09C
loc_1_A065:
        lea    edx, [eax + 0xb4]
        mov    ecx, dword ptr [esp + 0xc]
        cmp    dword ptr [edx], ecx
        je     loc_1_A09A
        test   esi, 2
        je     loc_1_A082
        mov    eax, 4
        jmp    loc_1_A09C
loc_1_A082:
        or     esi, 2
        push   2
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [edx], ecx
        push   dword ptr [eax + 0xe8]
        call   sub_1_394B
        add    esp, 8
loc_1_A09A:
        xor    eax, eax
loc_1_A09C:
        pop    esi
        ret    
sub_1_A09E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        test   al, 1
        jne    loc_1_A0B1
        mov    eax, 1
        jmp    loc_1_A0EE
loc_1_A0B1:
        lea    edx, [esi + 0xb4]
        mov    ecx, dword ptr [esp + 0xc]
        cmp    dword ptr [edx], ecx
        je     loc_1_A0C6
        mov    eax, 1
        jmp    loc_1_A0EE
loc_1_A0C6:
        test   al, 2
        jne    loc_1_A0D1
        mov    eax, 1
        jmp    loc_1_A0EE
loc_1_A0D1:
        and    al, 0xfd
        push   3
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [edx], 0
        push   dword ptr [esi + 0xe8]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
loc_1_A0EE:
        pop    esi
        ret    
sub_1_A0F0:
        xor    eax, eax
        mov    ecx, dword ptr [esp + 4]
        movzx  edx, word ptr [ecx + 0x10]
        cmp    edx, 8
        je     loc_1_A106
        cmp    edx, 0x10
        je     loc_1_A113
        jmp    loc_1_A121
loc_1_A106:
        push   dword ptr [ecx + 0xd8]
        call   sub_1_41A1
        jmp    loc_1_A11E
loc_1_A113:
        push   dword ptr [ecx + 0xd8]
        call   sub_1_41BD
loc_1_A11E:
        add    esp, 4
loc_1_A121:
        ret    
sub_1_A122:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0xe8]
        push   dword ptr [ecx + 0x6af]
        call   sub_1_931C
        add    esp, 4
        ret    
sub_1_A13B:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    edx, dword ptr [ebp + 0x14]
        cmp    word ptr [edx + 4], 0x10
        push   edi
        jne    loc_1_A15E
        mov    byte ptr [ebp - 4], 0xb2
        mov    byte ptr [ebp - 3], 0x10
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 1
        jmp    loc_1_A169
loc_1_A15E:
        mov    byte ptr [ebp - 4], 0xc2
        mov    byte ptr [ebp - 3], 0
        mov    ecx, dword ptr [ebp + 0x10]
loc_1_A169:
        dec    cx
        mov    byte ptr [ebp - 2], cl
        mov    byte ptr [ebp - 1], ch
        cmp    word ptr [edx + 0xc], 0
        je     loc_1_A17C
        or     byte ptr [ebp - 4], 8
loc_1_A17C:
        cmp    word ptr [ebp + 0xc], 0
        jne    loc_1_A187
        or     byte ptr [ebp - 4], 4
loc_1_A187:
        cmp    word ptr [edx + 6], 2
        jne    loc_1_A192
        or     byte ptr [ebp - 3], 0x20
loc_1_A192:
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_1_A197:
        test   eax, eax
        jne    loc_1_A1B8
        push   0xffff
        inc    edi
        push   dword ptr [ebp + edi - 5]
        push   dword ptr [esi + 0xd8]
        call   sub_1_436F
        add    esp, 0xc
        cmp    edi, 4
        jl     loc_1_A197
loc_1_A1B8:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_A1BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0xe8]
        cmp    dword ptr [edi + 0xf4], 0
        je     loc_1_A1E2
        cmp    dword ptr [edi + 0x1e0], 0
        jne    loc_1_A221
loc_1_A1E2:
        mov    ax, word ptr [esi + 0x10]
        mov    ebx, 0xffff
        sub    ax, 0x10
        push   ebx
        cmp    ax, 1
        sbb    al, al
        and    al, 5
        add    al, 0xd0
        push   eax
        push   dword ptr [esi + 0xd8]
        call   sub_1_436F
        mov    dword ptr [ebp - 8], 0
        add    esp, 0xc
        cmp    dword ptr [ebp - 8], ebx
        jae    loc_1_A221
loc_1_A215:
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 0xffff
        jb     loc_1_A215
loc_1_A221:
        mov    ax, word ptr [esi + 0x10]
        push   0xffff
        sub    ax, 0x10
        cmp    ax, 1
        sbb    al, al
        xor    ebx, ebx
        add    al, 0xda
        push   eax
        push   dword ptr [esi + 0xd8]
        call   sub_1_436F
        mov    word ptr [ebp - 2], bx
        add    esp, 0xc
        lea    eax, [ebp - 2]
        mov    edx, dword ptr [esi + 0xdc]
        push   eax
        push   dword ptr [esi + 0xb4]
        push   dword ptr [edx + 8]
        call   sub_1_97C6
        add    esp, 0xc
        cmp    word ptr [ebp - 2], bx
        je     loc_1_A291
loc_1_A26C:
        mov    bx, word ptr [ebp - 2]
        lea    eax, [ebp - 2]
        push   eax
        mov    edx, dword ptr [esi + 0xdc]
        push   dword ptr [esi + 0xb4]
        push   dword ptr [edx + 8]
        call   sub_1_97C6
        add    esp, 0xc
        cmp    word ptr [ebp - 2], bx
        jne    loc_1_A26C
loc_1_A291:
        cmp    dword ptr [edi + 0xf4], 0
        je     loc_1_A2A3
        cmp    dword ptr [edi + 0x1e0], 0
        jne    loc_1_A2B6
loc_1_A2A3:
        push   0xffff
        push   dword ptr [esi + 0xd8]
        call   sub_1_4288
        add    esp, 8
loc_1_A2B6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_A2BD:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [ebp + 0xc], 1
        sbb    cl, cl
        push   esi
        push   edi
        add    cl, 0x42
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], cl
        mov    cx, word ptr [ebp + 0x10]
        mov    byte ptr [ebp - 3], ch
        mov    byte ptr [ebp - 2], cl
loc_1_A2E2:
        test   eax, eax
        jne    loc_1_A303
        push   0xffff
        inc    edi
        push   dword ptr [ebp + edi - 5]
        push   dword ptr [esi + 0xd8]
        call   sub_1_436F
        add    esp, 0xc
        cmp    edi, 3
        jl     loc_1_A2E2
loc_1_A303:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_A309:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   0
        push   esi
        call   sub_1_A4AA
        add    esp, 8
        lea    edi, [esi + 0xd8]
        or     byte ptr [esi], 8
        cmp    word ptr [edi], 0x40
        jae    loc_1_A35A
loc_1_A329:
        movzx  eax, word ptr [edi]
        movzx  ecx, word ptr [esi + 0xda]
        push   1
        add    eax, ecx
        add    eax, esi
        add    eax, 0x58
        push   eax
        push   dword ptr [esi + 8]
        call   sub_1_414B
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_A35A
        mov    ax, word ptr [edi]
        inc    ax
        mov    word ptr [edi], ax
        cmp    ax, 0x40
        jb     loc_1_A329
loc_1_A35A:
        lea    eax, [esi + 0xdc]
        cmp    dword ptr [eax], 0
        jne    loc_1_A373
        push   esi
        mov    dword ptr [eax], 1
        call   sub_1_A381
        jmp    loc_1_A379
loc_1_A373:
        push   esi
        call   sub_1_A460
loc_1_A379:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        ret    
sub_1_A381:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x50], esi
        lea    edi, [esi + 0x3c]
        mov    word ptr [edi + 0x12], 2
        mov    word ptr [edi + 0x10], 1
        mov    eax, dword ptr [esi + 0xc]
        lea    edx, [esi + 0xda]
        lea    ecx, [esi + 0xd8]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x10]
        shr    eax, 0x10
        mov    word ptr [edi + 0xc], ax
        mov    eax, dword ptr [esi + 0x10]
        mov    word ptr [edi + 0xe], ax
        mov    ax, word ptr [edx]
        shl    ax, 8
        or     ax, word ptr [ecx]
        mov    word ptr [edi + 4], ax
        xor    eax, eax
        mov    word ptr [ecx], ax
        cmp    word ptr [edx], ax
        mov    word ptr [edx], 0x40
        je     loc_1_A3E3
        mov    word ptr [edx], 0
loc_1_A3E3:
        lea    eax, [ebp - 4]
        mov    dword ptr [edi], 0            ; -> loc_1_A423
        push   eax
        push   dword ptr [esi + 0x54]
        call   sub_1_39B0
        add    esp, 8
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_1_9C0C
        add    esp, 8
        push   dword ptr [esi + 4]
        push   dword ptr [ebp - 4]
        call   sub_1_9A60
        add    esp, 8
        push   esi
        call   sub_1_A460
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_A423:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0xd8], 0
        jbe    loc_1_A43C
        push   eax
        call   sub_1_A381
        add    esp, 4
        jmp    loc_1_A446
loc_1_A43C:
        mov    dword ptr [eax + 0xdc], 0
loc_1_A446:
        ret    
sub_1_A447:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        test   al, 1
        je     loc_1_A45A
        test   al, 4
        mov    eax, 0
        jne    loc_1_A45F
loc_1_A45A:
        mov    eax, 1
loc_1_A45F:
        ret    
sub_1_A460:
        mov    ecx, dword ptr [esp + 4]
        lea    eax, [ecx + 0xdc]
        cmp    dword ptr [eax], 0
        jne    loc_1_A48A
        cmp    word ptr [ecx + 0xd8], 0
        jbe    loc_1_A48A
        push   ecx
        mov    dword ptr [eax], 1
        call   sub_1_A381
        add    esp, 4
        jmp    loc_1_A4A9
loc_1_A48A:
        mov    eax, dword ptr [ecx]
        test   al, 0x10
        je     loc_1_A49A
        and    al, 0xef
        mov    dword ptr [ecx], eax
        and    al, 0xf7
        mov    dword ptr [ecx], eax
        jmp    loc_1_A4A9
loc_1_A49A:
        and    al, 0xf7
        push   1
        push   ecx
        mov    dword ptr [ecx], eax
        call   sub_1_A4AA
        add    esp, 8
loc_1_A4A9:
        ret    
sub_1_A4AA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 1
        je     loc_1_A500
        lea    edi, [esi + 8]
        lea    eax, [ebp - 1]
        push   eax
        mov    ebx, 0x83
        push   ebx
        push   dword ptr [edi]
        call   sub_1_40A6
        add    esp, 0xc
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_1_A4E4
        mov    al, byte ptr [ebp - 1]
        or     byte ptr [esi], 4
        or     al, 4
        push   eax
        push   ebx
        jmp    loc_1_A4F2
loc_1_A4E4:
        mov    al, byte ptr [ebp - 1]
        and    byte ptr [esi], 0xfb
        and    al, 0xfb
        push   eax
        push   0x83
loc_1_A4F2:
        push   dword ptr [edi]
        call   sub_1_4076
        add    esp, 0xc
        xor    eax, eax
        jmp    loc_1_A505
loc_1_A500:
        mov    eax, 1
loc_1_A505:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  060h, 09Ch, 0CDh, 020h, 007h, 000h, 005h, 000h, 08Bh, 0DAh, 08Bh, 0F0h, 0B9h, 032h, 000h, 000h
        db  000h, 0E2h, 0FEh, 0CDh, 020h, 007h, 000h, 005h, 000h, 02Bh, 0C6h, 08Bh, 0D8h, 0B8h, 0C8h, 000h
        db  000h, 000h, 0F6h, 0F3h, 0FEh, 0C4h, 088h, 025h
        dd  obj1:0x9204              ; fixup
        db  09Dh, 061h, 0C3h, 060h, 09Ch, 0CDh, 020h, 007h, 000h, 005h, 000h, 0BAh, 000h, 000h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h, 08Ah, 01Dh
        dd  obj1:0x9204              ; fixup
        db  0B9h, 032h, 000h, 000h, 000h, 0E2h, 0FEh, 0FEh, 0CBh, 075h, 0F5h, 09Dh, 061h, 0C3h, 060h, 09Ch
        db  0CDh, 020h, 007h, 000h, 005h, 000h, 08Bh, 0DAh, 08Bh, 0C8h, 081h, 0C1h, 0FAh, 000h, 000h, 000h
        db  083h, 0D3h, 000h, 0CDh, 020h, 007h, 000h, 005h, 000h, 03Bh, 0D3h, 072h, 0F6h, 077h, 004h, 03Bh
        db  0C1h, 072h, 0F0h, 09Dh, 061h, 0C3h, 0CCh, 0CCh, 0CCh
sub_1_A58C:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  0CCh
sub_1_A598:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_1_A5A8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_1_A5B8:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        int3   
        int3   
sub_1_A5C0:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x81  R-X
        db  006h, 000h, 000h, 000h
        dd  loc_2_001C               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        or     eax, dword ptr [eax]
loc_2_0010:
        sbb    byte ptr [eax], al
loc_2_0012:
        add    byte ptr es:[ebx], dh
        db  000h
loc_2_0016:
        inc    ecx
        add    byte ptr [ebp], ah
        int3   
        int3   
loc_2_001C:
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 0x69], cl
        js     0x8a
        jb     loc_2_0027
loc_2_0027:
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        and    byte ptr [edi + 0x61], dl
        jbe    0x95
        and    byte ptr [ecx + 0x6e], cl
        add    byte ptr [ebx + 0x42], dl
        xor    dword ptr [esi], esi
        and    byte ptr [edi + 0x61], dl
        jbe    0xa2
        and    byte ptr [edi + 0x75], cl
        je     loc_2_0042
loc_2_0042:
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 0x49], cl
        inc    esp
        dec    ecx
        and    byte ptr [ecx + 0x6e], cl
        add    byte ptr [ebx + 0x42], dl
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 0x49], cl
        inc    esp
        dec    ecx
        and    byte ptr [edi + 0x75], cl
        je     loc_2_005D
loc_2_005D:
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    0xcf
        inc    edx
        insb   byte ptr es:[edi], dx
        popal  
        jae    0xdb
        jb     0x8a
        xor    dword ptr [esi], esi
        and    byte ptr [ecx + ebp*2 + 0x72], al
        arpl   word ptr gs:[eax + 0x53], si
        outsd  dx, dword ptr [esi]
        jne    0xe6
        and    byte ptr fs:[edx + esi*2 + 0x69], al
        jbe    0xe4
        jb     0x81
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x86  R-X
sub_3_0000:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        int3   
        int3   
sub_3_0008:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_3_0010:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        int3   
        int3   
sub_3_0018:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_3_0020:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        int3   
        int3   
sub_3_0028:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_3_0030:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_3_0038:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        int3   
        int3   
sub_3_0040:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_3_0048:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Log_Conf ; cd201b803300
        int3   
        int3   
sub_3_0050:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_3_0058:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_3_0060:
        VxDCall  CONFIGMG._CONFIGMG_Add_Res_Des  ; cd201c803300
        int3   
        int3   
sub_3_0068:
        VxDCall  CONFIGMG._CONFIGMG_Add_Empty_Log_Conf ; cd2018803300
        int3   
        int3   
sub_3_0070:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        int3   
        int3   
sub_3_0078:
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004803300
        int3   
        int3   
sub_3_0080:
        VMMCall  VMM._Sprintf                    ; cd2074810100
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x2e52  R-X
        db  08Bh, 044h, 024h, 004h, 08Bh, 048h, 030h, 085h, 0C9h, 075h, 009h, 08Bh, 054h, 024h, 008h, 089h
        db  050h, 030h, 0EBh, 01Dh, 083h, 079h, 014h, 000h, 074h, 009h, 08Bh, 049h, 014h, 083h, 079h, 014h
        db  000h, 075h, 0F7h, 08Bh, 054h, 024h, 008h, 089h, 051h, 014h, 0C7h, 042h, 014h, 000h, 000h, 000h
        db  000h, 0C3h
sub_4_0032:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 0x30]
        cmp    eax, edx
        jne    loc_4_0049
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ecx + 0x30], eax
        jmp    loc_4_005F
loc_4_0049:
        cmp    dword ptr [edx + 0x14], eax
        je     loc_4_0056
loc_4_004E:
        mov    edx, dword ptr [edx + 0x14]
        cmp    dword ptr [edx + 0x14], eax
        jne    loc_4_004E
loc_4_0056:
        mov    eax, dword ptr [edx + 0x14]
        mov    ecx, dword ptr [eax + 0x14]
        mov    dword ptr [edx + 0x14], ecx
loc_4_005F:
        ret    
sub_4_0060:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        jne    loc_4_0071
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x30]
        jmp    loc_4_0074
loc_4_0071:
        mov    eax, dword ptr [eax + 0x14]
loc_4_0074:
        ret    
loc_4_0075:
        push   ebp
        mov    ecx, 0x10
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    edi, 0                        ; -> obj1:0x1F20
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, dword ptr [ebp + 8]
        jne    loc_4_0099
        mov    dword ptr [edx], eax
loc_4_0099:
        mov    edi, 0                        ; -> loc_1_2120
        mov    ecx, 0x10
        mov    esi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_00AF
        add    eax, 4
        mov    dword ptr [edx], eax
loc_4_00AF:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     loc_4_00BF
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
        jmp    loc_4_00C4
loc_4_00BF:
        mov    eax, 0x80004002
loc_4_00C4:
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
loc_4_00CA:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x1c]
        inc    eax
        mov    dword ptr [ecx + 0x1c], eax
        ret    4
loc_4_00D8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x1c]
        dec    eax
        mov    dword ptr [esi + 0x1c], eax
        jne    loc_4_012A
        cmp    dword ptr [esi + 0x84], 0
        je     loc_4_0101
        mov    eax, dword ptr [esi + 0x88]
        test   eax, eax
        je     loc_4_010A
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        jmp    loc_4_010A
loc_4_0101:
        push   esi
        call   sub_4_0AAE
        add    esp, 4
loc_4_010A:
        push   esi
        push   dword ptr [esi + 0x20]
        call   sub_4_0032
        add    esp, 8
        mov    dword ptr [esi + 0x18], 0xdeaddead
        push   0
        push   esi
        call   sub_4_2D4C
        add    esp, 8
        xor    eax, eax
loc_4_012A:
        pop    esi
        ret    4
loc_4_012E:
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret    0xc
loc_4_0145:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        ret    4
loc_4_0158:
        mov    eax, dword ptr [esp + 4]
        dec    dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        ret    4
loc_4_016B:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 0x84], 0
        jne    loc_4_0185
        mov    eax, 0x80004001
        jmp    loc_4_01AF
loc_4_0185:
        mov    ecx, dword ptr [eax + 0x88]
        mov    eax, 0x88780096
        test   ecx, ecx
        je     loc_4_01AF
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   dword ptr [eax + 0xc]
loc_4_01AF:
        pop    ebp
        ret    0x20
loc_4_01B3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 0x84], 0
        jne    loc_4_01CD
        mov    eax, 0x80004001
        jmp    loc_4_01EE
loc_4_01CD:
        mov    ecx, dword ptr [eax + 0x88]
        mov    eax, 0x88780096
        test   ecx, ecx
        je     loc_4_01EE
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   dword ptr [eax + 0x10]
loc_4_01EE:
        pop    ebp
        ret    0x14
loc_4_01F2:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   dword ptr [ecx + 4]
        call   sub_4_0B72
        add    esp, 8
        ret    8
loc_4_0208:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 0x84], 0
        je     loc_4_0235
        mov    eax, dword ptr [eax + 0x88]
        test   eax, eax
        jne    loc_4_0229
        mov    eax, 0x88780096
        jmp    loc_4_0242
loc_4_0229:
        push   dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0x18]
        jmp    loc_4_0242
loc_4_0235:
        push   dword ptr [esp + 8]
        push   eax
        call   sub_4_0B34
        add    esp, 8
loc_4_0242:
        ret    8
loc_4_0245:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 0x84], 0
        jne    loc_4_025C
        mov    eax, 0x8878001e
        jmp    loc_4_0277
loc_4_025C:
        mov    eax, dword ptr [eax + 0x88]
        test   eax, eax
        jne    loc_4_026D
        mov    eax, 0x88780096
        jmp    loc_4_0277
loc_4_026D:
        push   dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0x1c]
loc_4_0277:
        ret    8
loc_4_027A:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 0x84], 0
        jne    loc_4_0291
        mov    eax, 0x80004001
        jmp    loc_4_02AC
loc_4_0291:
        mov    eax, dword ptr [eax + 0x88]
        test   eax, eax
        jne    loc_4_02A2
        mov    eax, 0x88780096
        jmp    loc_4_02AC
loc_4_02A2:
        push   dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0x20]
loc_4_02AC:
        ret    8
loc_4_02AF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 0x84], 0
        je     loc_4_02E4
        mov    ecx, dword ptr [eax + 0x88]
        test   ecx, ecx
        jne    loc_4_02D6
        mov    eax, 0x88780096
        jmp    loc_4_0304
loc_4_02D6:
        push   dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   dword ptr [eax + 0x24]
        jmp    loc_4_0304
loc_4_02E4:
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        call   sub_4_0D57
        add    esp, 8
        test   eax, eax
        jne    loc_4_0304
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    dword ptr [edx], ecx
loc_4_0304:
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_4_030A:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 4]
        cmp    dword ptr [esi + 0x84], 0
        je     loc_4_0340
        mov    ecx, dword ptr [esi + 0x88]
        test   ecx, ecx
        jne    loc_4_032F
        mov    eax, 0x88780096
        jmp    loc_4_0361
loc_4_032F:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   dword ptr [eax + 0x28]
        jmp    loc_4_0361
loc_4_0340:
        cmp    dword ptr [esi + 0x24], 1
        jne    loc_4_034A
        xor    eax, eax
        jmp    loc_4_0361
loc_4_034A:
        push   dword ptr [ebp + 0x14]
        push   esi
        call   sub_4_0C5C
        add    esp, 8
        test   eax, eax
        jne    loc_4_0361
        mov    dword ptr [esi + 0x24], 1
loc_4_0361:
        pop    esi
        pop    ebp
        ret    0x10
loc_4_0366:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        cmp    dword ptr [esi + 0x84], 0
        je     loc_4_0390
        mov    eax, dword ptr [esi + 0x88]
        test   eax, eax
        jne    loc_4_0388
        mov    eax, 0x88780096
        jmp    loc_4_03AC
loc_4_0388:
        push   eax
        mov    esi, dword ptr [eax]
        call   dword ptr [esi + 0x2c]
        jmp    loc_4_03AC
loc_4_0390:
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_4_039A
        xor    eax, eax
        jmp    loc_4_03AC
loc_4_039A:
        push   esi
        call   sub_4_0D02
        add    esp, 4
        xor    ecx, ecx
        test   eax, eax
        jne    loc_4_03AC
        mov    dword ptr [esi + 0x24], ecx
loc_4_03AC:
        pop    esi
        ret    4
loc_4_03B0:
        push   ebp
        mov    ecx, 0x10
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    edi, 0                        ; -> obj1:0x1F20
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, dword ptr [ebp + 8]
        jne    loc_4_03D4
        mov    dword ptr [edx], eax
loc_4_03D4:
        mov    edi, 0                        ; -> loc_1_2120
        mov    ecx, 0x10
        mov    esi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_03EA
        add    eax, 4
        mov    dword ptr [edx], eax
loc_4_03EA:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     loc_4_03FA
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
        jmp    loc_4_03FF
loc_4_03FA:
        mov    eax, 0x80004002
loc_4_03FF:
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
loc_4_0405:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x18]
        inc    eax
        mov    dword ptr [ecx + 0x18], eax
        ret    4
loc_4_0413:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x18]
        dec    eax
        mov    dword ptr [ecx + 0x18], eax
        jne    loc_4_0434
        mov    dword ptr [ecx + 0x14], 0xdeaddead
        push   0
        push   ecx
        call   sub_4_2D4C
        add    esp, 8
        xor    eax, eax
loc_4_0434:
        ret    4
loc_4_0437:
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret    0xc
loc_4_044E:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        ret    4
loc_4_0461:
        mov    eax, dword ptr [esp + 4]
        dec    dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        ret    4
loc_4_0474:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 0x100
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 4]
        push   ebp
        call   sub_4_12FC
        mov    edi, dword ptr [esp + 0x118]
        mov    ecx, dword ptr [ebx + 0x28]
        movzx  edx, word ptr [ecx + 0x681]
        lea    esi, [edi + 4]
        push   edx
        push   eax
        xor    ebp, ebp
        push   0                             ; -> obj5:0x0080
        push   esi
        call   sub_4_2D5C
        add    esp, 0x10
        mov    dword ptr [edi + 0x210], ebp
        mov    eax, dword ptr [ebx + 0x2c]
        test   eax, eax
        je     loc_4_04FD
        push   edi
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0xc]
        test   eax, eax
        jne    loc_4_0559
        mov    eax, dword ptr [ebx + 0x28]
        movzx  ecx, word ptr [eax + 0x681]
        lea    eax, [esp + 0x10]
        push   ecx
        push   esi
        push   0                             ; -> obj5:0x0080
        push   eax
        call   sub_4_2D5C
        lea    eax, [esp + 0x20]
        add    esp, 0x10
        push   0x100
        push   eax
        push   esi
        call   sub_4_2D54
        add    esp, 0xc
loc_4_04FD:
        lea    esi, [edi + 0x104]
        push   0xff
        push   0                             ; -> obj5:0x0074
        or     byte ptr [edi], 0xb
        push   esi
        call   sub_4_2D54
        add    esp, 0xc
        mov    eax, dword ptr [ebx + 0x20]
        xor    ecx, ecx
        mov    dword ptr [edi + 0x204], eax
        mov    eax, 0xb3
        mov    dword ptr [edi + 0x20c], ecx
        mov    word ptr [edi + 0x208], 0x32a5
        cmp    byte ptr [esi], cl
        je     loc_4_0547
loc_4_053C:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_4_053C
loc_4_0547:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 0x20a], dx
        xor    eax, eax
loc_4_0559:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x100
        ret    8
loc_4_0566:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        xor    eax, eax
        inc    dword ptr [esi + 0x24]
        mov    ecx, dword ptr [esi + 0x2c]
        test   ecx, ecx
        je     loc_4_0587
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 0x10]
        test   eax, eax
        je     loc_4_0587
        dec    dword ptr [esi + 0x24]
loc_4_0587:
        pop    esi
        ret    4
loc_4_058B:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0x2c]
        test   ecx, ecx
        je     loc_4_05A2
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 0x14]
loc_4_05A2:
        test   eax, eax
        jne    loc_4_05A9
        dec    dword ptr [esi + 0x24]
loc_4_05A9:
        pop    esi
        ret    4
loc_4_05AD:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   ecx
        mov    edi, dword ptr [eax + 4]
        push   edi
        call   sub_4_0921
        mov    esi, dword ptr [ebp + 0xc]
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0xc], 1
        mov    dword ptr [esi], ecx
        xor    ecx, ecx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0x10], ecx
        mov    dword ptr [esi + 0x14], ecx
        mov    dword ptr [esi + 0x18], ecx
        mov    dword ptr [esi + 0x1c], ecx
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], ecx
        mov    dword ptr [esi + 0x28], ecx
        mov    dword ptr [esi + 0x2c], ecx
        mov    dword ptr [esi + 0x30], ecx
        mov    dword ptr [esi + 0x34], ecx
        mov    dword ptr [esi + 0x38], ecx
        mov    dword ptr [esi + 0x3c], ecx
        mov    dword ptr [esi + 0x40], ecx
        mov    dword ptr [esi + 0x44], ecx
        mov    dword ptr [esi + 0x48], ecx
        mov    eax, dword ptr [edi + 0x2c]
        test   eax, eax
        je     loc_4_0627
        push   esi
        mov    edi, dword ptr [eax]
        push   eax
        call   dword ptr [edi + 0x18]
        test   eax, eax
        jne    loc_4_0629
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [esi + 0xc]
        or     dword ptr [esi], eax
loc_4_0627:
        xor    eax, eax
loc_4_0629:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0631:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0x20]
        push   0x201
        push   0x8c
        mov    dword ptr [ecx], 0
        call   sub_4_2D64
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_4_0669
        mov    eax, 0x8007000e
        jmp    loc_4_072E
loc_4_0669:
        xor    eax, eax
        mov    dword ptr [edi], 0            ; -> obj5:0x0000
        mov    dword ptr [edi + 4], 0        ; -> obj5:0x0010
        test   byte ptr [ebp + 0x10], 1
        mov    dword ptr [edi + 8], edi
        mov    dword ptr [edi + 0xc], edi
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [edi + 0x20], esi
        mov    dword ptr [edi + 0x18], 0x8c4233c0
        jne    loc_4_06D0
        mov    ecx, dword ptr [esi + 0x2c]
        test   ecx, ecx
        jne    loc_4_06A2
        mov    eax, 0x88780096
        jmp    loc_4_072E
loc_4_06A2:
        lea    eax, [edi + 0x88]
        push   eax
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   dword ptr [eax + 0x1c]
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_4_0705
        mov    dword ptr [edi + 0x84], 1
        jmp    loc_4_0701
loc_4_06D0:
        push   edi
        call   sub_4_0944
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_4_0701
        push   dword ptr [ebp + 0xc]
        push   edi
        call   sub_4_0B72
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_4_0705
        mov    ecx, dword ptr [edi + 0x34]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp + 0x18]
        mov    edx, dword ptr [edi + 0x38]
        mov    dword ptr [ecx], edx
loc_4_0701:
        test   ebx, ebx
        je     loc_4_0714
loc_4_0705:
        push   0
        push   edi
        call   sub_4_2D4C
        add    esp, 8
        mov    eax, ebx
        jmp    loc_4_072E
loc_4_0714:
        lea    eax, [edi + 4]
        mov    ecx, dword ptr [ebp + 0x20]
        push   edi
        push   esi
        mov    dword ptr [ecx], eax
        call   0
        add    esp, 8
        mov    eax, dword ptr [edi]
        push   edi
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_4_072E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x1c
loc_4_0735:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    ecx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        push   edi
        push   ebp
        mov    edi, dword ptr [eax + 4]
        mov    ebp, dword ptr [ecx + 4]
        xor    eax, eax
        mov    dword ptr [esi], eax
        cmp    dword ptr [edi + 0x2c], eax
        jne    loc_4_075E
        mov    eax, 0x88780096
        jmp    loc_4_07F1
loc_4_075E:
        push   0x201
        push   0x8c
        call   sub_4_2D64
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_4_077D
        mov    eax, 0x8007000e
        jmp    loc_4_07F1
loc_4_077D:
        mov    dword ptr [ebx], 0            ; -> obj5:0x0000
        mov    dword ptr [ebx + 4], 0        ; -> obj5:0x0010
        lea    eax, [ebx + 0x88]
        mov    dword ptr [ebx + 8], ebx
        push   eax
        mov    dword ptr [ebx + 0xc], ebx
        mov    dword ptr [ebx + 0x24], 0
        mov    dword ptr [ebx + 0x20], edi
        mov    dword ptr [ebx + 0x18], 0x8c4233c0
        push   dword ptr [ebp + 0x88]
        mov    ecx, dword ptr [edi + 0x2c]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 0x20]
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_4_07CB
        mov    dword ptr [ebx + 0x84], 1
        test   ebp, ebp
        je     loc_4_07DA
loc_4_07CB:
        push   0
        push   ebx
        call   sub_4_2D4C
        add    esp, 8
        mov    eax, ebp
        jmp    loc_4_07F1
loc_4_07DA:
        lea    eax, [ebx + 4]
        push   ebx
        push   edi
        mov    dword ptr [esi], eax
        call   0
        add    esp, 8
        mov    eax, dword ptr [ebx]
        push   ebx
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_4_07F1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0xc
sub_4_07F8:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0x10]
        push   edi
        xor    esi, esi
        mov    dword ptr [ebx], esi
        call   sub_1_301E
loc_4_0808:
        test   eax, eax
        je     loc_4_082C
        mov    edi, dword ptr [esp + 0x10]
loc_4_0810:
        mov    ecx, dword ptr [eax + 0x7a7]
        test   ecx, ecx
        je     loc_4_081F
        cmp    dword ptr [ecx + 0x2c], edi
        je     loc_4_0835
loc_4_081F:
        push   eax
        call   sub_1_303E
loc_4_0825:
        add    esp, 4
        test   eax, eax
        jne    loc_4_0810
loc_4_082C:
        mov    eax, 0xa
loc_4_0831:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_4_0835:
        mov    dword ptr [ebx], eax
        xor    eax, eax
        jmp    loc_4_0831
sub_4_083B:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 0x1c
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        push   edi
        call   sub_1_301E
loc_4_0853:
        mov    ebx, eax
        test   ebx, ebx
        je     loc_4_08C2
loc_4_0859:
        push   0
        lea    eax, [ebp - 8]
        push   0
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 8], 0
        push   eax
        push   ecx
        push   dword ptr [ebx + 8]
        call   sub_4_2D6C
        add    esp, 0x14
        test   eax, eax
        jne    loc_4_08B3
        cmp    dword ptr [ebp - 4], 0x10
        jne    loc_4_08B3
        lea    eax, [ebp - 0x1c]
        lea    ecx, [ebp - 0x10]
        push   eax
        lea    edx, [ebx + 0xc]
        push   ecx
        lea    esi, [ebp - 0x10]
        push   edx
        call   sub_4_08D7
        add    esp, 0xc
        mov    ecx, 8
        mov    edi, dword ptr [ebp + 8]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_08B3
        lea    esi, [ebp - 0x1c]
        mov    ecx, 9
        mov    edi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_4_08CE
loc_4_08B3:
        push   ebx
        call   sub_1_303E
loc_4_08B9:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_4_0859
loc_4_08C2:
        mov    eax, 0xa
loc_4_08C7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_4_08CE:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ebx
        xor    eax, eax
        jmp    loc_4_08C7
sub_4_08D7:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
loc_4_08DE:
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 0x5c
        jne    loc_4_08DE
        mov    eax, dword ptr [ebp + 0xc]
        cmp    byte ptr [edx], 0x5c
        je     loc_4_08FE
loc_4_08EE:
        mov    cl, byte ptr [edx]
        cmp    cl, 0x5f
        je     loc_4_08FE
        inc    edx
        mov    byte ptr [eax], cl
        inc    eax
        cmp    byte ptr [edx], 0x5c
        jne    loc_4_08EE
loc_4_08FE:
        mov    byte ptr [eax], 0
loc_4_0901:
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 0x5c
        jne    loc_4_0901
        mov    eax, dword ptr [ebp + 0x10]
        cmp    byte ptr [edx], 0
        je     loc_4_091C
loc_4_0911:
        mov    cl, byte ptr [edx]
        inc    edx
        mov    byte ptr [eax], cl
        inc    eax
        cmp    byte ptr [edx], 0
        jne    loc_4_0911
loc_4_091C:
        pop    ebp
        mov    byte ptr [eax], 0
        ret    
sub_4_0921:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ecx], 7
        mov    edx, dword ptr [eax + 0x28]
        test   byte ptr [edx + 0xd4], 0x20
        je     loc_4_0941
        mov    dword ptr [ecx], 0xf
loc_4_0941:
        xor    eax, eax
        ret    
sub_4_0944:
        sub    esp, 0x34
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0x40]
        push   edi
        push   ebp
        mov    eax, dword ptr [ebx + 0x20]
        mov    ebp, dword ptr [eax + 0x28]
        test   byte ptr [ebp + 0xf0], 2
        je     loc_4_0971
        cmp    word ptr [ebp + 0x120], 1
        jne    loc_4_0971
        mov    dword ptr [ebx + 0x3c], 0
        jmp    loc_4_098B
loc_4_0971:
        test   byte ptr [ebp + 0x1dc], 2
        je     loc_4_098B
        cmp    word ptr [ebp + 0x20c], 1
        jne    loc_4_098B
        mov    dword ptr [ebx + 0x3c], 1
loc_4_098B:
        call   sub_4_2D74
        mov    dword ptr [esp + 0x10], eax
        push   ebp
        mov    dword ptr [ebx + 0x40], eax
        call   sub_4_0EE3
        add    esp, 4
        test   eax, eax
        je     loc_4_0A4F
        lea    edi, [esp + 0x30]
        xor    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    esi, [ebp + 0x2c4]
        lea    eax, [esp + 0x20]
        push   eax
        push   esi
        push   8
        push   dword ptr [esp + 0x1c]
        call   sub_4_1826
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_0A4F
        mov    dword ptr [ebx + 0x4c], 1
        mov    dword ptr [esp + 0x16], 0x30
        mov    word ptr [esp + 0x14], 0
        lea    edi, [esp + 0x14]
        push   edi
        push   esi
        push   0x80
        push   dword ptr [esp + 0x1c]
        call   sub_4_1826
        mov    eax, dword ptr [esp + 0x2a]
        add    esp, 0x10
        mov    dword ptr [ebx + 0x50], eax
        push   edi
        mov    dword ptr [esp + 0x1a], 0xc
        push   esi
        push   0x80
        push   dword ptr [esp + 0x1c]
        call   sub_4_1826
        mov    dword ptr [esp + 0x26], 0x30
        add    esp, 0x10
        push   edi
        push   esi
        push   0x100
        push   dword ptr [esp + 0x1c]
        call   sub_4_1826
        lea    eax, [esp + 0x30]
        add    esp, 0x10
        push   eax
        push   esi
        push   0x10
        push   dword ptr [esp + 0x1c]
        call   sub_4_1826
        add    esp, 0x10
loc_4_0A4F:
        lea    esi, [ebp + 0x6cb]
        lea    edi, [ebx + 0x6c]
        mov    eax, 6
        mov    ecx, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebp + 0x6e3]
        lea    edi, [ebx + 0x54]
        mov    ecx, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        cmp    dword ptr [ebx + 0x3c], 1
        lea    eax, [ebx + 0x48]
        jne    loc_4_0A7F
        mov    dword ptr [eax], 1
        jmp    loc_4_0A85
loc_4_0A7F:
        mov    dword ptr [eax], 0
loc_4_0A85:
        cmp    dword ptr [eax], 0
        je     loc_4_0A95
        mov    eax, dword ptr [ebx + 0x68]
        mov    ecx, dword ptr [ebx + 0x5c]
        mov    dword ptr [ebx + 0x34], eax
        jmp    loc_4_0AA1
loc_4_0A95:
        mov    eax, dword ptr [ebx + 0x80]
        mov    ecx, dword ptr [ebx + 0x74]
        mov    dword ptr [ebx + 0x34], eax
loc_4_0AA1:
        xor    eax, eax
        pop    ebp
        pop    edi
        mov    dword ptr [ebx + 0x38], ecx
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
sub_4_0AAE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0x20]
        mov    esi, dword ptr [eax + 0x28]
        push   esi
        call   sub_4_0EE3
        add    esp, 4
        test   eax, eax
        je     loc_4_0B2D
        cmp    dword ptr [edi + 0x4c], 0
        je     loc_4_0B2D
        lea    ebx, [ebp - 0x1c]
        add    esi, 0x2c4
        mov    dword ptr [edi + 0x4c], 0
        push   ebx
        push   esi
        push   8
        push   dword ptr [edi + 0x40]
        call   sub_4_1826
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_0B2D
        mov    eax, dword ptr [edi + 0x50]
        lea    ecx, [ebp - 0xc]
        push   ecx
        mov    dword ptr [ebp - 6], eax
        mov    dword ptr [ebp - 0xa], 0x30
        mov    word ptr [ebp - 0xc], 0
        push   esi
        push   0x100
        push   dword ptr [edi + 0x40]
        call   sub_4_1826
        add    esp, 0x10
        push   ebx
        push   esi
        push   0x10
        push   dword ptr [edi + 0x40]
        call   sub_4_1826
        add    esp, 0x10
loc_4_0B2D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_0B34:
        push   esi
        mov    eax, 0xec
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        mov    edx, dword ptr [esi + 0x20]
        mov    ecx, dword ptr [edx + 0x28]
        mov    edx, dword ptr [esi + 0x3c]
        imul   edx, eax
        add    ecx, edx
        add    ecx, eax
        push   ecx
        push   0xa
        push   dword ptr [esi + 0x40]
        call   sub_1_5FAC
loc_4_0B5D:
        add    esp, 0x10
        cmp    eax, 1
        mov    eax, 0
        pop    esi
        adc    eax, -1
        and    eax, 0x80004005
        ret    
sub_4_0B72:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    eax, dword ptr [edx + 0x20]
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 0x28]
        mov    eax, 2
        test   dword ptr [ebx + 0xf0], eax
        je     loc_4_0BA0
        cmp    word ptr [ebx + 0x120], 1
        jne    loc_4_0BA0
        xor    eax, eax
        jmp    loc_4_0BB8
loc_4_0BA0:
        test   byte ptr [ebx + 0x1dc], 2
        je     loc_4_0BB8
        cmp    word ptr [ebx + 0x20c], 1
        jne    loc_4_0BB8
        mov    eax, 1
loc_4_0BB8:
        cmp    eax, 1
        sbb    ecx, ecx
        neg    ecx
        imul   ecx, ecx, 0xec
        lea    esi, [ecx + ebx]
        mov    ecx, dword ptr [ecx + ebx + 0xf0]
        test   cl, 1
        mov    esi, dword ptr [esi + 0x11c]
        je     loc_4_0C01
        test   cl, 2
        je     loc_4_0C01
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 4], esi
        je     loc_4_0BEE
        mov    eax, 0x8878000a
        jmp    loc_4_0C55
loc_4_0BEE:
        mov    ecx, dword ptr [ebp + 0xc]
        movzx  esi, word ptr [ecx + 0xe]
        cmp    esi, dword ptr [edx + 0x2c]
        je     loc_4_0C01
        mov    eax, 0x8878000a
        jmp    loc_4_0C55
loc_4_0C01:
        cmp    dword ptr [edx + 0x3c], eax
        je     loc_4_0C39
        lea    edi, [ebp - 0x18]
        lea    esi, [ebx + 0x6cb]
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        lea    edi, [ebx + 0x6cb]
        lea    esi, [ebx + 0x6e3]
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebp - 0x18]
        lea    edi, [ebx + 0x6e3]
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_4_0C39:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x3c], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0x28], eax
        movzx  ebx, word ptr [ecx + 0xe]
        mov    dword ptr [edx + 0x2c], ebx
        movzx  eax, word ptr [ecx + 2]
        mov    dword ptr [edx + 0x30], eax
        xor    eax, eax
loc_4_0C55:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_0C5C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        mov    eax, dword ptr [ecx + 0x20]
        mov    edx, 0xec
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 0x28]
        mov    esi, dword ptr [ecx + 0x3c]
        xor    edi, edi
        imul   esi, edx
        or     byte ptr [ebx + esi + 0xf0], 4
        mov    ax, word ptr [ecx + 0x30]
        mov    esi, dword ptr [ecx + 0x2c]
        mov    word ptr [ebp - 0x14], si
        mov    ebx, dword ptr [ecx + 0x28]
        mov    word ptr [ebp - 0x12], ax
        mov    dword ptr [ebp - 0x18], edi
        shr    esi, 3
        imul   si, ax
        mov    word ptr [ebp - 0x10], bx
        mov    eax, dword ptr [ecx + 0x38]
        mov    word ptr [ebp - 0xe], si
        mov    dword ptr [ebp - 6], eax
        mov    word ptr [ebp - 0xc], di
        mov    esi, eax
        mov    word ptr [ebp - 2], di
        lea    eax, [ebp - 0x18]
        shr    esi, 1
        push   eax
        mov    ebx, dword ptr [ecx + 0x20]
        mov    dword ptr [ebp - 0xa], esi
        mov    esi, dword ptr [ecx + 0x3c]
        mov    eax, dword ptr [ebx + 0x28]
        imul   esi, edx
        add    eax, esi
        add    eax, edx
        push   eax
        push   5
        push   dword ptr [ecx + 0x40]
        call   sub_1_5FAC
loc_4_0CDC:
        add    esp, 0x10
        cmp    eax, 8
        jne    loc_4_0CEB
        mov    eax, 0x8878000a
        jmp    loc_4_0CFB
loc_4_0CEB:
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x80004005
loc_4_0CFB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_0D02:
        sub    esp, 4
        lea    eax, [esp]
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   eax
        mov    edi, 0xec
        mov    ecx, dword ptr [esi + 0x20]
        mov    dword ptr [esp + 0xc], 3
        mov    eax, dword ptr [ecx + 0x28]
        mov    ecx, dword ptr [esi + 0x3c]
        imul   ecx, edi
        add    eax, ecx
        add    eax, edi
        push   eax
        push   7
        push   dword ptr [esi + 0x40]
        call   sub_1_5FAC
loc_4_0D38:
        add    esp, 0x10
        mov    edx, dword ptr [esi + 0x20]
        mov    eax, dword ptr [esi + 0x3c]
        mov    ecx, dword ptr [edx + 0x28]
        imul   eax, edi
        pop    edi
        pop    esi
        and    byte ptr [ecx + eax + 0xf0], 0xfb
        xor    eax, eax
        add    esp, 4
        ret    
sub_4_0D57:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x20]
        mov    ecx, dword ptr [esi + 0x3c]
        mov    edx, dword ptr [eax + 0x28]
        imul   ecx, ecx, 0xec
        lea    eax, [edx + ecx + 0xec]
        lea    edx, [ebp - 4]
        push   edx
        push   eax
        push   9
        push   dword ptr [esi + 0x40]
        call   sub_1_5FAC
loc_4_0D86:
        add    esp, 0x10
        test   eax, eax
        mov    eax, 0x80004005
        jne    loc_4_0D9F
        mov    ecx, dword ptr [esi + 0x38]
        mov    eax, dword ptr [ebp + 0xc]
        sub    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_4_0D9F:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_0DA4:
        cmp    dword ptr [0], 0              ; -> obj5:0x008C
        push   ebp
        mov    ebp, esp
        push   esi
        jne    loc_4_0DCC
        push   0x19
        push   0xe
        call   sub_4_2DA4
        mov    dword ptr [0], eax            ; -> obj5:0x008C
        cmp    dword ptr [0], 0              ; -> obj5:0x008C
        jne    loc_4_0DCC
        xor    eax, eax
        jmp    loc_4_0E1D
loc_4_0DCC:
        mov    eax, dword ptr [0]            ; -> obj5:0x008C
        push   eax
        call   sub_4_2D90
        mov    esi, eax
        mov    eax, 0
        test   esi, esi
        je     loc_4_0E1D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        push   8
        mov    dword ptr [esi], eax
        lea    eax, [esi + 8]
        mov    dword ptr [esi + 4], ecx
        push   dword ptr [ebp + 0x10]
        push   eax
        call   sub_4_2D54
        add    esp, 0xc
        lea    edx, [esi + 0x10]
        push   9
        push   dword ptr [ebp + 0x14]
        push   edx
        call   sub_4_2D54
        add    esp, 0xc
        push   esi
        push   dword ptr [0]                 ; -> obj5:0x008C
        call   sub_4_2D80
        mov    eax, esi
loc_4_0E1D:
        pop    esi
        pop    ebp
        ret    
sub_4_0E20:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj5:0x008C
        push   edi
        test   eax, eax
        je     loc_4_0E65
        push   eax
        call   sub_4_2DC8
        test   eax, eax
        je     loc_4_0E65
loc_4_0E35:
        lea    esi, [eax + 8]
        mov    ecx, 8
        mov    edi, dword ptr [esp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_0E55
        lea    esi, [eax + 0x10]
        mov    ecx, 9
        mov    edi, dword ptr [esp + 0x10]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_4_0E67
loc_4_0E55:
        push   eax
        push   dword ptr [0]                 ; -> obj5:0x008C
        call   sub_4_2DB8
        test   eax, eax
        jne    loc_4_0E35
loc_4_0E65:
        xor    eax, eax
loc_4_0E67:
        pop    edi
        pop    esi
        ret    
sub_4_0E6A:
        cmp    dword ptr [0], 0              ; -> obj5:0x008C
        push   esi
        je     loc_4_0E9C
        mov    eax, dword ptr [0]            ; -> obj5:0x008C
        push   eax
        call   sub_4_2DC8
        test   eax, eax
        je     loc_4_0E9C
        mov    esi, dword ptr [esp + 8]
loc_4_0E87:
        cmp    dword ptr [eax + 4], esi
        je     loc_4_0E9E
        push   eax
        push   dword ptr [0]                 ; -> obj5:0x008C
        call   sub_4_2DB8
        test   eax, eax
        jne    loc_4_0E87
loc_4_0E9C:
        xor    eax, eax
loc_4_0E9E:
        pop    esi
        ret    
sub_4_0EA0:
        push   dword ptr [esp + 4]
        push   dword ptr [0]                 ; -> obj5:0x008C
        call   sub_4_2DF8
        push   dword ptr [esp + 4]
        push   dword ptr [0]                 ; -> obj5:0x008C
        call   sub_4_2DE8
        push   dword ptr [0]                 ; -> obj5:0x008C
        call   sub_4_2DC8
        test   eax, eax
        jne    loc_4_0EE2
        mov    eax, dword ptr [0]            ; -> obj5:0x008C
        push   eax
        call   sub_4_2DD8
        mov    dword ptr [0], 0              ; -> obj5:0x008C
loc_4_0EE2:
        ret    
sub_4_0EE3:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 0x4c
        mov    ecx, dword ptr [eax + 0x7a7]
        mov    eax, dword ptr [ecx + 0x2c]
        test   eax, eax
        je     loc_4_0F16
        lea    ecx, [esp]
        push   ecx
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0x18]
        test   eax, eax
        jne    loc_4_0F16
        xor    eax, eax
        cmp    eax, dword ptr [esp + 0x40]
        mov    eax, 1
        adc    eax, -1
        jmp    loc_4_0F18
loc_4_0F16:
        xor    eax, eax
loc_4_0F18:
        add    esp, 0x4c
        ret    
sub_4_0F1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   ebx
        push   4
        call   sub_1_2E0C
loc_4_0F38:
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_105E
        push   0x201
        push   0x34
        call   sub_4_2D64
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_4_105E
        lea    edi, [esi + 4]
        mov    dword ptr [esi], 0            ; -> obj5:0x0040
        push   1
        push   0                             ; -> obj5:0x0090
        lea    eax, [ebp - 0xc]
        mov    dword ptr [edi], 0            ; -> obj5:0x0050
        push   eax
        lea    ecx, [ebp - 0x24]
        push   ecx
        mov    dword ptr [esi + 8], esi
        mov    dword ptr [esi + 0xc], esi
        mov    dword ptr [esi + 0x20], ebx
        mov    dword ptr [esi + 0x14], 0x8c4233c0
        call   sub_4_2E20
        test   eax, eax
        je     loc_4_0FA3
        push   0
        push   esi
        call   sub_4_2D4C
        add    esp, 8
        jmp    loc_4_105E
loc_4_0FA3:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x1c], edi
        mov    dword ptr [ebp - 0x20], 0
        mov    dword ptr [esi + 0x1c], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        VxDCall  Dev_357E.Service_0001h          ; cd2001007e35
        add    esp, 8
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_4_0FEE
        mov    eax, dword ptr [ebp - 0xc]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_2E0C
        push   0
        push   esi
        call   sub_4_2D4C
        add    esp, 8
        jmp    loc_4_105E
loc_4_0FEE:
        mov    eax, dword ptr [ebp - 8]
        xor    edi, edi
        push   edi
        lea    ecx, [ebp - 0x14]
        push   edi
        mov    dword ptr [eax + 0x7a7], esi
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        mov    dword ptr [esi + 0x28], eax
        lea    eax, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x14], edi
        push   dword ptr [edx + 8]
        call   sub_4_2D6C
        add    esp, 0x14
        test   eax, eax
        jne    loc_4_105E
        cmp    dword ptr [ebp - 0x10], 0x10
        jne    loc_4_105E
        lea    eax, [ebp - 0x38]
        lea    ecx, [ebp - 0x2c]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0xc
        push   eax
        call   sub_4_08D7
        add    esp, 0xc
        lea    ecx, [ebp - 0x38]
        lea    edx, [ebp - 0x2c]
        push   ecx
        push   edx
        call   sub_4_0E20
        add    esp, 8
        test   eax, eax
        je     loc_4_105E
        mov    ecx, dword ptr [eax + 4]
        push   eax
        mov    dword ptr [esi + 0x2c], ecx
        call   sub_4_0EA0
        add    esp, 4
loc_4_105E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1065:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x324
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        push   eax
        push   ecx
        push   dword ptr [ebp + 8]
        push   4
        call   sub_1_2E0C
loc_4_1083:
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_116B
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 0x7a7]
        xor    eax, eax
        test   esi, esi
        je     loc_4_116B
        mov    dword ptr [ebp - 0x18], eax
        lea    eax, [esi + 4]
        mov    dword ptr [ebp - 0x14], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        VxDCall  Dev_357E.Service_0002h          ; cd2002007e35
        add    esp, 8
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [esi + 0x1c]
        xor    edi, edi
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_2E0C
        mov    eax, dword ptr [esi + 0x2c]
        test   eax, eax
        je     loc_4_1157
        lea    ecx, [ebp - 0x324]
        mov    ebx, dword ptr [eax]
        push   ecx
        push   eax
        call   dword ptr [ebx + 0xc]
        test   eax, eax
        jne    loc_4_1157
        mov    ebx, dword ptr [ebp - 0x120]
        push   edi
        push   edi
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 0xc]
        push   ecx
        mov    dword ptr [ebp - 0x10], edi
        push   ebx
        call   sub_4_2D6C
        add    esp, 0x14
        test   eax, eax
        jne    loc_4_1157
        cmp    dword ptr [ebp - 0xc], 0x10
        jne    loc_4_1157
        push   edi
        lea    eax, [ebp - 0xf8]
        push   0xc8
        push   eax
        push   ebx
        call   sub_4_2E44
        add    esp, 0x10
        test   eax, eax
        jne    loc_4_1157
        lea    eax, [ebp - 0x30]
        lea    ecx, [ebp - 0x24]
        push   eax
        lea    edx, [ebp - 0xf8]
        push   ecx
        push   edx
        call   sub_4_08D7
        add    esp, 0xc
        lea    ecx, [ebp - 0x30]
        lea    edx, [ebp - 0x24]
        push   ecx
        push   edx
        push   dword ptr [esi + 0x2c]
        push   ebx
        call   sub_4_0DA4
        add    esp, 0x10
loc_4_1157:
        xor    edi, edi
        push   esi
        mov    dword ptr [esi + 0x1c], edi
        mov    eax, dword ptr [esi]
        call   dword ptr [eax + 8]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x7a7], edi
loc_4_116B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
VSB16_Service_01:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x31c
        push   esi
        lea    eax, [ebp - 0x31c]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   esi
        mov    eax, dword ptr [esi]
        call   dword ptr [eax + 0xc]
        test   eax, eax
        jne    loc_4_1280
        mov    edi, dword ptr [ebp - 0x118]
        xor    eax, eax
        push   eax
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    dword ptr [ebp - 0x14], eax
        lea    eax, [ebp - 0x14]
        push   eax
        push   ecx
        push   edi
        call   sub_4_2D6C
        add    esp, 0x14
        test   eax, eax
        je     loc_4_11C1
        mov    eax, 0x88780078
        jmp    loc_4_1280
loc_4_11C1:
        cmp    dword ptr [ebp - 0x10], 0x10
        push   0
        jne    loc_4_122F
        push   0xc8
        lea    eax, [ebp - 0xf0]
        push   eax
        push   edi
        call   sub_4_2E44
        add    esp, 0x10
        test   eax, eax
        je     loc_4_11EC
        mov    eax, 0x88780078
        jmp    loc_4_1280
loc_4_11EC:
        lea    eax, [ebp - 0x28]
        lea    ecx, [ebp - 0x1c]
        push   eax
        lea    edx, [ebp - 0xf0]
        push   ecx
        push   edx
        call   sub_4_08D7
        add    esp, 0xc
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 0x28]
        lea    eax, [ebp - 0x1c]
        push   ecx
        push   edx
        push   eax
        call   sub_4_083B
        add    esp, 0xc
        test   eax, eax
        je     loc_4_1267
        lea    eax, [ebp - 0x28]
        lea    ecx, [ebp - 0x1c]
        push   eax
        push   ecx
        push   esi
        push   edi
        call   sub_4_0DA4
        add    esp, 0x10
        jmp    loc_4_127E
loc_4_122F:
        push   edi
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_4_2E4C
        add    esp, 0xc
        test   eax, eax
        je     loc_4_1247
        mov    eax, 0x88780078
        jmp    loc_4_1280
loc_4_1247:
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   dword ptr [ebp - 0xc]
        push   4
        call   sub_1_2E0C
loc_4_1259:
        add    esp, 0x10
        test   eax, eax
        je     loc_4_1267
        mov    eax, 0x88780078
        jmp    loc_4_1280
loc_4_1267:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x7a7]
        test   eax, eax
        jne    loc_4_127B
        mov    eax, 0x88780078
        jmp    loc_4_1280
loc_4_127B:
        mov    dword ptr [eax + 0x2c], esi
loc_4_127E:
        xor    eax, eax
loc_4_1280:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
VSB16_Service_02:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_07F8
        add    esp, 8
        test   eax, eax
        je     loc_4_12C2
        push   dword ptr [ebp + 8]
        call   sub_4_0E6A
        add    esp, 4
        test   eax, eax
        je     loc_4_12BB
        push   eax
        call   sub_4_0EA0
        add    esp, 4
        jmp    loc_4_12F2
loc_4_12BB:
        mov    eax, 0x88780078
        jmp    loc_4_12F4
loc_4_12C2:
        mov    eax, dword ptr [ebp - 4]
        xor    edi, edi
        push   edi
        mov    esi, dword ptr [eax + 0x7a7]
        push   esi
        mov    dword ptr [esi + 0x2c], edi
        call   sub_4_0060
        add    esp, 8
        test   eax, eax
        je     loc_4_12F2
loc_4_12DE:
        push   eax
        mov    dword ptr [eax + 0x88], edi
        push   esi
        call   sub_4_0060
        add    esp, 8
        test   eax, eax
        jne    loc_4_12DE
loc_4_12F2:
        xor    eax, eax
loc_4_12F4:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
sub_4_12FC:
        movzx  eax, word ptr [0x16]          ; -> loc_2_0016
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_4_130A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        test   al, 1
        je     loc_4_1376
        test   al, 2
        je     loc_4_1323
        mov    eax, 4
        jmp    loc_4_137B
loc_4_1323:
        or     al, 2
        mov    ecx, dword ptr [ebp + 0xc]
        push   2
        mov    dword ptr [esi], eax
        push   dword ptr [esi + 0x24]
        mov    dword ptr [esi + 4], ecx
        call   sub_1_394B
loc_4_1337:
        add    esp, 8
        push   esi
        call   sub_1_5DB8
loc_4_1340:
        add    esp, 4
        mov    edi, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_4_136B
        test   edi, edi
        je     loc_4_1372
        push   0xff
        push   dword ptr [esi + 8]
        call   sub_1_40DA
loc_4_135B:
        add    esp, 8
        push   0x3f
        push   dword ptr [esi + 8]
        call   sub_1_40DA
loc_4_1368:
        add    esp, 8
loc_4_136B:
        test   edi, edi
        je     loc_4_1372
        mov    dword ptr [edi + 2], esi
loc_4_1372:
        xor    eax, eax
        jmp    loc_4_137B
loc_4_1376:
        mov    eax, 3
loc_4_137B:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_4_137F:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        test   dl, 1
        je     loc_4_13B1
        test   dl, 2
        je     loc_4_13B1
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     loc_4_139F
        mov    eax, 1
        jmp    loc_4_13B3
loc_4_139F:
        and    dl, 0xfd
        push   3
        push   dword ptr [eax + 0x24]
        mov    dword ptr [eax], edx
        call   sub_1_394B
loc_4_13AE:
        add    esp, 8
loc_4_13B1:
        xor    eax, eax
loc_4_13B3:
        ret    
sub_4_13B4:
        push   ebp
        movzx  eax, word ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        je     loc_4_13ED
        cmp    eax, 2
        je     loc_4_1402
        cmp    eax, 3
        je     loc_4_1413
        cmp    eax, 4
        je     loc_4_1424
        cmp    eax, 5
        je     loc_4_1432
        cmp    eax, 6
        je     loc_4_145B
        mov    eax, 0xf
        jmp    loc_4_149D
loc_4_13ED:
        push   dword ptr [ebp + 0x14]
        push   4
        push   dword ptr [esi + 0x24]
        call   sub_1_3846
loc_4_13FA:
        add    esp, 0xc
        jmp    loc_4_149D
loc_4_1402:
        push   dword ptr [ebp + 0x14]
        push   esi
        call   sub_4_152D
        add    esp, 8
        jmp    loc_4_149D
loc_4_1413:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_4_130A
        add    esp, 0xc
        jmp    loc_4_149D
loc_4_1424:
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_4_137F
        add    esp, 8
        jmp    loc_4_149D
loc_4_1432:
        mov    edi, dword ptr [ebp + 0x14]
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [edi]
        test   bl, bl
        je     loc_4_149B
loc_4_143F:
        movzx  eax, byte ptr [ebp - 1]
        mov    al, byte ptr [eax + edi + 1]
        push   eax
        push   esi
        call   sub_1_5C62
loc_4_144E:
        add    esp, 8
        inc    byte ptr [ebp - 1]
        cmp    bl, byte ptr [ebp - 1]
        ja     loc_4_143F
        jmp    loc_4_149B
loc_4_145B:
        mov    edi, dword ptr [ebp + 0x14]
        lea    ecx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [edi + 4]
        push   eax
        shr    eax, 0x10
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_9C3E
loc_4_1472:
        add    esp, 0x10
        test   eax, eax
        je     loc_4_1480
        mov    eax, 1
        jmp    loc_4_149D
loc_4_1480:
        xor    ebx, ebx
        cmp    dword ptr [edi], ebx
        jbe    loc_4_149B
loc_4_1486:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ebx]
        inc    ebx
        push   eax
        push   esi
        call   sub_1_5C62
loc_4_1494:
        add    esp, 8
        cmp    dword ptr [edi], ebx
        ja     loc_4_1486
loc_4_149B:
        xor    eax, eax
loc_4_149D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_14A4:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_4_14B1:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    word ptr [esi + 0x2a], 2
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0x24], eax
        mov    word ptr [esi + 0x2c], 0xc9
        xor    eax, eax
        lea    edx, [ebp - 2]
        mov    word ptr [esi + 0x50], ax
        mov    dword ptr [esi + 0x56], eax
        mov    word ptr [esi + 0x52], ax
        push   edx
        mov    word ptr [esi + 0x4e], 1
        mov    word ptr [esi + 0x54], 0xffff
        push   ecx
        call   sub_1_4807
loc_4_14F7:
        add    esp, 8
        lea    eax, [esi + 0x2e]
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        push   dword ptr [0]                 ; -> obj1:0x91F0
        push   0                             ; -> loc_1_91FC
        push   eax
        call   sub_1_2403
loc_4_1513:
        add    esp, 0x10
        xor    eax, eax
        or     byte ptr [esi], 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1520:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_4_152D:
        push   esi
        mov    ecx, 0xc
        mov    esi, dword ptr [esp + 8]
        push   edi
        add    esi, 0x28
        mov    edi, dword ptr [esp + 0x10]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        xor    eax, eax
        pop    edi
        pop    esi
        ret    
sub_4_1548:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        test   al, 1
        je     loc_4_15D5
        test   al, 2
        je     loc_4_1561
        mov    eax, 4
        jmp    loc_4_15DA
loc_4_1561:
        or     al, 2
        mov    ecx, dword ptr [ebp + 0xc]
        push   2
        mov    dword ptr [esi], eax
        push   dword ptr [esi + 0x54]
        mov    dword ptr [esi + 4], ecx
        call   sub_1_394B
loc_4_1575:
        add    esp, 8
        push   esi
        call   sub_1_5DD0
loc_4_157E:
        add    esp, 4
        mov    edi, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_4_15A9
        test   edi, edi
        je     loc_4_15D1
        push   0xff
        push   dword ptr [esi + 8]
        call   sub_1_40DA
loc_4_1599:
        add    esp, 8
        push   0x3f
        push   dword ptr [esi + 8]
        call   sub_1_40DA
loc_4_15A6:
        add    esp, 8
loc_4_15A9:
        test   edi, edi
        je     loc_4_15D1
        mov    eax, dword ptr [edi + 0xa]
        mov    dword ptr [esi + 0xc], eax
        mov    ecx, dword ptr [edi + 6]
        mov    dword ptr [esi + 0x10], ecx
        mov    dword ptr [edi + 2], esi
        mov    dx, word ptr [esi + 0x3a]
        mov    dword ptr [edi + 0x12], 0x80
        mov    word ptr [edi + 0x10], 0
        mov    word ptr [edi + 0xe], dx
loc_4_15D1:
        xor    eax, eax
        jmp    loc_4_15DA
loc_4_15D5:
        mov    eax, 3
loc_4_15DA:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_4_15DE:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        test   dl, 1
        je     loc_4_1610
        test   dl, 2
        je     loc_4_1610
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     loc_4_15FE
        mov    eax, 1
        jmp    loc_4_1612
loc_4_15FE:
        and    dl, 0xfd
        push   3
        push   dword ptr [eax + 0x54]
        mov    dword ptr [eax], edx
        call   sub_1_394B
loc_4_160D:
        add    esp, 8
loc_4_1610:
        xor    eax, eax
loc_4_1612:
        ret    
sub_4_1613:
        push   ebp
        movzx  eax, word ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        xor    edi, edi
        cmp    eax, 1
        je     loc_4_1651
        cmp    eax, 2
        je     loc_4_1668
        cmp    eax, 3
        je     loc_4_167B
        cmp    eax, 4
        je     loc_4_168E
        cmp    eax, 5
        je     loc_4_169E
        cmp    eax, 6
        je     loc_4_16D4
        mov    edi, 0xf
        jmp    loc_4_1702
loc_4_1651:
        push   dword ptr [ebp + 0x14]
        push   2
        push   dword ptr [esi + 0x54]
        call   sub_1_3846
loc_4_165E:
        add    esp, 0xc
        mov    edi, eax
        jmp    loc_4_1702
loc_4_1668:
        push   dword ptr [ebp + 0x14]
        push   esi
        call   sub_4_17BC
        add    esp, 8
        mov    edi, eax
        jmp    loc_4_1702
loc_4_167B:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_4_1548
        add    esp, 0xc
        mov    edi, eax
        jmp    loc_4_1702
loc_4_168E:
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_4_15DE
        add    esp, 8
        mov    edi, eax
        jmp    loc_4_1702
loc_4_169E:
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_9B4D
loc_4_16A7:
        add    esp, 4
        push   esi
        call   sub_4_17D7
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 0xdc], eax
        push   1
        mov    word ptr [esi + 0xd8], ax
        push   esi
        call   sub_1_A4AA
loc_4_16CA:
        add    esp, 8
        mov    edi, eax
        push   dword ptr [ebp - 4]
        jmp    loc_4_16FA
loc_4_16D4:
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_9B4D
loc_4_16DD:
        add    esp, 4
        mov    eax, dword ptr [esi]
        test   al, 8
        je     loc_4_16EC
        or     al, 0x10
        mov    dword ptr [esi], eax
        jmp    loc_4_16F7
loc_4_16EC:
        push   0
        push   esi
        call   sub_1_A4AA
loc_4_16F4:
        add    esp, 8
loc_4_16F7:
        push   dword ptr [ebp - 4]
loc_4_16FA:
        call   sub_1_9B5B
loc_4_16FF:
        add    esp, 4
loc_4_1702:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_170A:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_4_1717:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        lea    edx, [ebp - 2]
        mov    word ptr [esi + 0x16], 2
        push   edx
        mov    dword ptr [esi + 8], ecx
        push   ecx
        mov    dword ptr [esi + 0x54], eax
        mov    word ptr [esi + 0x18], 0xca
        call   sub_1_4807
loc_4_1744:
        add    esp, 8
        lea    eax, [esi + 0x1a]
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        push   dword ptr [0]                 ; -> obj1:0x91EC
        push   0                             ; -> loc_1_91FC
        push   eax
        call   sub_1_2403
loc_4_1760:
        add    esp, 0x10
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 8]
        lea    eax, [esi + 0x58]
        push   ecx
        push   edx
        push   0x80
        push   eax
        call   sub_1_94B8
loc_4_1779:
        add    esp, 0x10
        lea    ecx, [esi + 0x3a]
        push   ecx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        call   sub_1_93F9
loc_4_178B:
        add    esp, 0xc
        test   eax, eax
        mov    eax, 1
        jne    loc_4_17AA
        xor    eax, eax
        or     byte ptr [esi], 1
        mov    word ptr [esi + 0xd8], ax
        mov    word ptr [esi + 0xda], ax
loc_4_17AA:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_17AF:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_4_17BC:
        push   esi
        mov    ecx, 9
        mov    esi, dword ptr [esp + 8]
        push   edi
        add    esi, 0x14
        mov    edi, dword ptr [esp + 0x10]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        xor    eax, eax
        pop    edi
        pop    esi
        ret    
sub_4_17D7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        xor    di, di
        mov    esi, dword ptr [ebp + 8]
loc_4_17E5:
        lea    eax, [ebp - 1]
        mov    ecx, dword ptr [esi + 8]
        mov    dx, word ptr [ecx + 0x20]
        push   eax
        push   edx
        call   sub_1_9CE4
loc_4_17F6:
        add    esp, 8
        test   byte ptr [ebp - 1], 0x80
        jne    loc_4_1816
        xor    di, di
        lea    eax, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [esi + 8]
        mov    dx, word ptr [ecx + 0x1e]
        push   edx
        call   sub_1_9CE4
loc_4_1813:
        add    esp, 8
loc_4_1816:
        inc    di
        cmp    di, 0x3c
        jb     loc_4_17E5
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1826:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        movzx  eax, word ptr [esp + 0xc]
        mov    ebp, esp
        cmp    eax, 8
        jg     loc_4_184A
        je     loc_4_1893
        cmp    eax, 1
        je     loc_4_186E
        cmp    eax, 2
        je     loc_4_1880
        cmp    eax, 4
        je     loc_4_188C
        jmp    loc_4_1867
loc_4_184A:
        cmp    eax, 0x10
        je     loc_4_18A4
        cmp    eax, 0x20
        je     loc_4_18AF
        cmp    eax, 0x40
        je     loc_4_18BA
        cmp    eax, 0x80
        je     loc_4_18C1
        cmp    eax, 0x100
        je     loc_4_18CC
loc_4_1867:
        mov    eax, 0xf
        jmp    loc_4_18D5
loc_4_186E:
        push   dword ptr [ebp + 0x14]
        push   8
        push   dword ptr [ecx + 0x24]
        call   sub_1_3846
loc_4_187B:
        add    esp, 0xc
        jmp    loc_4_18D5
loc_4_1880:
        mov    eax, dword ptr [ebp + 0x14]
        mov    word ptr [eax], 7
        xor    eax, eax
        jmp    loc_4_18D5
loc_4_188C:
        mov    eax, 0xc
        jmp    loc_4_18D5
loc_4_1893:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_4_18D9
        add    esp, 0xc
        jmp    loc_4_18D5
loc_4_18A4:
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_4_191D
        jmp    loc_4_18D5
loc_4_18AF:
        push   dword ptr [ebp + 0x14]
        push   ecx
        call   sub_4_2CEF
        jmp    loc_4_18D5
loc_4_18BA:
        mov    eax, 0xc
        jmp    loc_4_18D5
loc_4_18C1:
        push   dword ptr [ebp + 0x14]
        push   ecx
        call   sub_4_1AF2
        jmp    loc_4_18D5
loc_4_18CC:
        push   dword ptr [ebp + 0x14]
        push   ecx
        call   sub_4_2368
loc_4_18D5:
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_18D9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        test   al, 1
        je     loc_4_1916
        test   al, 2
        je     loc_4_18EF
        mov    eax, 4
        jmp    loc_4_191B
loc_4_18EF:
        or     al, 2
        mov    ecx, dword ptr [esp + 0xc]
        push   2
        mov    dword ptr [esi], eax
        push   dword ptr [esi + 0x24]
        mov    dword ptr [esi + 4], ecx
        call   sub_1_394B
loc_4_1904:
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 8
        test   eax, eax
        je     loc_4_1912
        mov    dword ptr [eax + 2], esi
loc_4_1912:
        xor    eax, eax
        jmp    loc_4_191B
loc_4_1916:
        mov    eax, 3
loc_4_191B:
        pop    esi
        ret    
sub_4_191D:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     loc_4_1931
        mov    eax, 4
        jmp    loc_4_1943
loc_4_1931:
        push   3
        and    byte ptr [eax], 0xfd
        push   dword ptr [eax + 0x24]
        call   sub_1_394B
loc_4_193E:
        add    esp, 8
        xor    eax, eax
loc_4_1943:
        ret    
sub_4_1944:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_4_1951:
        mov    ecx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0x24], eax
        or     byte ptr [esi], 1
        mov    cx, word ptr [ecx + 4]
        push   ecx
        call   sub_1_3B3A
loc_4_1971:
        and    ax, 0xff
        add    esp, 4
        mov    word ptr [esi + 0x28], ax
        xor    eax, eax
        pop    esi
        ret    
sub_4_1980:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret    
sub_4_198D:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 0x10]
        test   byte ptr [esi + 2], 3
        je     loc_4_19AA
        push   esi
        push   dword ptr [esp + 0x10]
        call   sub_4_19C4
        add    esp, 8
        mov    edi, eax
loc_4_19AA:
        test   byte ptr [esi + 2], 0xc
        je     loc_4_19BF
        push   esi
        push   dword ptr [esp + 0x10]
        call   sub_4_1A16
        add    esp, 8
        or     edi, eax
loc_4_19BF:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_4_19C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   0x47
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_19DE:
        add    esp, 0xc
        mov    ebx, eax
        mov    edi, dword ptr [ebp + 0xc]
        lea    ecx, [ebp - 1]
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 0x18
        push   ecx
        push   0x46
        mov    dword ptr [edi + 6], eax
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_19FE:
        add    esp, 0xc
        or     ebx, eax
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 8
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1A16:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   0x45
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1A30:
        add    esp, 0xc
        mov    ebx, eax
        mov    edi, dword ptr [ebp + 0xc]
        lea    ecx, [ebp - 1]
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 0x18
        push   ecx
        push   0x44
        mov    dword ptr [edi + 6], eax
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1A50:
        add    esp, 0xc
        or     ebx, eax
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 8
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1A68:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        lea    esi, [eax + 6]
        movzx  eax, word ptr [eax]
        mov    dword ptr [esi], ecx
        cmp    eax, 3
        je     loc_4_1A88
        cmp    eax, 4
        je     loc_4_1A8C
        jmp    loc_4_1A8E
loc_4_1A88:
        mov    cl, 0x3d
        jmp    loc_4_1A8E
loc_4_1A8C:
        mov    cl, 0x3e
loc_4_1A8E:
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + 8]
        call   sub_1_40A6
loc_4_1A9E:
        add    esp, 0xc
        test   byte ptr [ebp - 1], 1
        je     loc_4_1AB3
        mov    eax, dword ptr [esi]
        or     ah, 8
        mov    dword ptr [esi], eax
        or     ah, 4
        mov    dword ptr [esi], eax
loc_4_1AB3:
        test   byte ptr [ebp - 1], 2
        je     loc_4_1ABC
        or     byte ptr [esi], 0x40
loc_4_1ABC:
        test   byte ptr [ebp - 1], 4
        je     loc_4_1AC5
        or     byte ptr [esi], 0x80
loc_4_1AC5:
        test   byte ptr [ebp - 1], 8
        je     loc_4_1ACF
        or     byte ptr [esi + 1], 1
loc_4_1ACF:
        test   byte ptr [ebp - 1], 0x10
        je     loc_4_1AD9
        or     byte ptr [esi + 1], 2
loc_4_1AD9:
        test   byte ptr [ebp - 1], 0x20
        je     loc_4_1AE2
        or     byte ptr [esi], 0x10
loc_4_1AE2:
        test   byte ptr [ebp - 1], 0x40
        je     loc_4_1AEB
        or     byte ptr [esi], 0x20
loc_4_1AEB:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1AF2:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        movzx  ecx, word ptr [eax]
        mov    ebp, esp
        test   ecx, ecx
        je     loc_4_1B25
        cmp    ecx, 1
        je     loc_4_1B30
        cmp    ecx, 2
        je     loc_4_1B3B
        cmp    ecx, 3
        jl     loc_4_1B1E
        cmp    ecx, 4
        jle    loc_4_1B46
        cmp    ecx, 5
        je     loc_4_1B51
        cmp    ecx, 6
        je     loc_4_1B5C
loc_4_1B1E:
        mov    eax, 0xf
        jmp    loc_4_1B65
loc_4_1B25:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1CBD
        jmp    loc_4_1B65
loc_4_1B30:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_198D
        jmp    loc_4_1B65
loc_4_1B3B:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1B69
        jmp    loc_4_1B65
loc_4_1B46:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1A68
        jmp    loc_4_1B65
loc_4_1B51:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1BCF
        jmp    loc_4_1B65
loc_4_1B5C:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_1CA2
loc_4_1B65:
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1B69:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 1]
        mov    esi, dword ptr [ebp + 0xc]
        push   ecx
        add    esi, 6
        push   0x3c
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [esi], 0
        push   dword ptr [edx + 8]
        call   sub_1_40A6
loc_4_1B8D:
        add    esp, 0xc
        test   byte ptr [ebp - 1], 1
        je     loc_4_1BA2
        mov    eax, dword ptr [esi]
        or     ah, 8
        mov    dword ptr [esi], eax
        or     ah, 4
        mov    dword ptr [esi], eax
loc_4_1BA2:
        test   byte ptr [ebp - 1], 2
        je     loc_4_1BAB
        or     byte ptr [esi], 0x40
loc_4_1BAB:
        test   byte ptr [ebp - 1], 4
        je     loc_4_1BB4
        or     byte ptr [esi], 0x80
loc_4_1BB4:
        test   byte ptr [ebp - 1], 8
        je     loc_4_1BBE
        or     byte ptr [esi + 1], 1
loc_4_1BBE:
        test   byte ptr [ebp - 1], 0x10
        je     loc_4_1BC8
        or     byte ptr [esi + 1], 2
loc_4_1BC8:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1BCF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        test   byte ptr [esi + 2], 3
        je     loc_4_1C28
        lea    eax, [ebp - 1]
        lea    ebx, [esi + 6]
        push   eax
        push   0x40
        push   dword ptr [edi + 8]
        call   sub_1_40A6
loc_4_1BF9:
        add    esp, 0xc
        mov    al, byte ptr [ebp - 1]
        shr    al, 6
        lea    ecx, [ebp - 1]
        movzx  eax, al
        push   ecx
        mov    dword ptr [ebx], eax
        shl    eax, 0x10
        push   0x3f
        mov    dword ptr [ebx], eax
        push   dword ptr [edi + 8]
        call   sub_1_40A6
loc_4_1C1A:
        add    esp, 0xc
        mov    al, byte ptr [ebp - 1]
        shr    al, 6
        movzx  ecx, al
        or     dword ptr [ebx], ecx
loc_4_1C28:
        test   byte ptr [esi + 2], 0xc
        je     loc_4_1C6E
        lea    eax, [ebp - 1]
        lea    ebx, [esi + 6]
        push   eax
        push   0x42
        push   dword ptr [edi + 8]
        call   sub_1_40A6
loc_4_1C3F:
        add    esp, 0xc
        mov    al, byte ptr [ebp - 1]
        shr    al, 6
        lea    ecx, [ebp - 1]
        movzx  eax, al
        push   ecx
        mov    dword ptr [ebx], eax
        shl    eax, 0x10
        push   0x41
        mov    dword ptr [ebx], eax
        push   dword ptr [edi + 8]
        call   sub_1_40A6
loc_4_1C60:
        add    esp, 0xc
        mov    al, byte ptr [ebp - 1]
        shr    al, 6
        movzx  ecx, al
        or     dword ptr [ebx], ecx
loc_4_1C6E:
        test   byte ptr [esi + 2], 0x10
        je     loc_4_1C99
        lea    eax, [ebp - 1]
        push   eax
        push   0x43
        push   dword ptr [edi + 8]
        call   sub_1_40A6
loc_4_1C82:
        mov    dword ptr [esi + 6], 0
        add    esp, 0xc
        test   byte ptr [ebp - 1], 1
        jne    loc_4_1C99
        mov    dword ptr [esi + 6], 1
loc_4_1C99:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1CA2:
        mov    eax, dword ptr [esp + 8]
        lea    ecx, [eax + 6]
        push   ecx
        push   dword ptr [eax + 2]
        mov    eax, dword ptr [esp + 0xc]
        push   dword ptr [eax + 8]
        call   sub_1_40A6
loc_4_1CB9:
        add    esp, 0xc
        ret    
sub_4_1CBD:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x14]
        push   ebp
        lea    ebp, [edi + 2]
        test   byte ptr [ebp], 3
        je     loc_4_1CE2
        mov    esi, dword ptr [esp + 0x14]
        push   edi
        push   esi
        call   sub_4_1D59
        add    esp, 8
        mov    ebx, eax
        jmp    loc_4_1CE6
loc_4_1CE2:
        mov    esi, dword ptr [esp + 0x14]
loc_4_1CE6:
        test   byte ptr [ebp], 0xc
        je     loc_4_1CF8
        push   edi
        push   esi
        call   sub_4_1E3E
        add    esp, 8
        or     ebx, eax
loc_4_1CF8:
        test   byte ptr [ebp], 0x30
        je     loc_4_1D0A
        push   edi
        push   esi
        call   sub_4_1F23
        add    esp, 8
        or     ebx, eax
loc_4_1D0A:
        test   byte ptr [ebp], 0xc0
        je     loc_4_1D1C
        push   edi
        push   esi
        call   sub_4_2008
        add    esp, 8
        or     ebx, eax
loc_4_1D1C:
        test   byte ptr [ebp + 1], 3
        je     loc_4_1D2E
        push   edi
        push   esi
        call   sub_4_20ED
        add    esp, 8
        or     ebx, eax
loc_4_1D2E:
        test   byte ptr [ebp + 1], 0xc
        je     loc_4_1D40
        push   edi
        push   esi
        call   sub_4_21D2
        add    esp, 8
        or     ebx, eax
loc_4_1D40:
        test   byte ptr [ebp + 1], 0x30
        je     loc_4_1D52
        push   edi
        push   esi
        call   sub_4_2254
        add    esp, 8
        or     ebx, eax
loc_4_1D52:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_1D59:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x28]
        cmp    ax, 4
        je     loc_4_1DD6
        cmp    ax, 5
        je     loc_4_1DD6
        cmp    ax, 7
        je     loc_4_1DD6
        cmp    ax, 8
        je     loc_4_1DD6
        lea    eax, [ebp - 1]
        push   eax
        push   0x31
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1D8F:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x30
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1DC0:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        jmp    loc_4_1E29
loc_4_1DD6:
        lea    eax, [ebp - 1]
        push   eax
        push   0x31
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1DE4:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x30
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1E15:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
loc_4_1E29:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1E3E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x28]
        cmp    ax, 4
        je     loc_4_1EBB
        cmp    ax, 5
        je     loc_4_1EBB
        cmp    ax, 7
        je     loc_4_1EBB
        cmp    ax, 8
        je     loc_4_1EBB
        lea    eax, [ebp - 1]
        push   eax
        push   0x33
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1E74:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x32
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1EA5:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        jmp    loc_4_1F0E
loc_4_1EBB:
        lea    eax, [ebp - 1]
        push   eax
        push   0x33
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1EC9:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x32
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1EFA:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
loc_4_1F0E:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_1F23:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x28]
        cmp    ax, 4
        je     loc_4_1FA0
        cmp    ax, 5
        je     loc_4_1FA0
        cmp    ax, 7
        je     loc_4_1FA0
        cmp    ax, 8
        je     loc_4_1FA0
        lea    eax, [ebp - 1]
        push   eax
        push   0x35
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1F59:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x34
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1F8A:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        jmp    loc_4_1FF3
loc_4_1FA0:
        lea    eax, [ebp - 1]
        push   eax
        push   0x35
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1FAE:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x34
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_1FDF:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
loc_4_1FF3:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_2008:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x28]
        cmp    ax, 4
        je     loc_4_2085
        cmp    ax, 5
        je     loc_4_2085
        cmp    ax, 7
        je     loc_4_2085
        cmp    ax, 8
        je     loc_4_2085
        lea    eax, [ebp - 1]
        push   eax
        push   0x37
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_203E:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x36
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_206F:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        jmp    loc_4_20D8
loc_4_2085:
        lea    eax, [ebp - 1]
        push   eax
        push   0x37
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_2093:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x36
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_20C4:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
loc_4_20D8:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_20ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x28]
        cmp    ax, 4
        je     loc_4_216A
        cmp    ax, 5
        je     loc_4_216A
        cmp    ax, 7
        je     loc_4_216A
        cmp    ax, 8
        je     loc_4_216A
        lea    eax, [ebp - 1]
        push   eax
        push   0x39
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_2123:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x38
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_2154:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 3
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00A0
        jmp    loc_4_21BD
loc_4_216A:
        lea    eax, [ebp - 1]
        push   eax
        push   0x39
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_2178:
        add    esp, 0xc
        mov    ebx, eax
        mov    al, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0xc]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
        movzx  eax, dl
        shl    eax, 0x18
        mov    byte ptr [ebp - 1], dl
        lea    edx, [ebp - 1]
        mov    dword ptr [edi + 6], eax
        push   edx
        push   0x38
        push   dword ptr [esi + 8]
        call   sub_1_40A6
loc_4_21A9:
        add    esp, 0xc
        or     ebx, eax
        mov    al, byte ptr [ebp - 1]
        shr    al, 5
        movzx  ecx, al
        mov    dl, byte ptr [ecx]            ; -> obj5:0x00C0
loc_4_21BD:
        movzx  eax, dl
        shl    eax, 8
        mov    byte ptr [ebp - 1], dl
        or     dword ptr [edi + 6], eax
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_21D2:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [ecx + 0x28]
        mov    ebp, esp
        sub    esp, 4
        cmp    ax, 4
        je     loc_4_221A
        cmp    ax, 5
        je     loc_4_221A
        cmp    ax, 7
        je     loc_4_221A
        cmp    ax, 8
        je     loc_4_221A
        lea    eax, [ebp - 1]
        push   eax
        push   0x3a
        push   dword ptr [ecx + 8]
        call   sub_1_40A6
loc_4_2206:
        add    esp, 0xc
        mov    cl, byte ptr [ebp - 1]
        shr    cl, 3
        movzx  edx, cl
        mov    cl, byte ptr [edx]            ; -> obj5:0x00A0
        jmp    loc_4_223A
loc_4_221A:
        lea    eax, [ebp - 1]
        push   eax
        push   0x3a
        push   dword ptr [ecx + 8]
        call   sub_1_40A6
loc_4_2228:
        add    esp, 0xc
        mov    cl, byte ptr [ebp - 1]
        shr    cl, 5
        movzx  edx, cl
        mov    cl, byte ptr [edx]            ; -> obj5:0x00C0
loc_4_223A:
        mov    byte ptr [ebp - 1], cl
        movzx  ecx, cl
        mov    edx, ecx
        shl    edx, 0x10
        or     edx, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        shl    edx, 8
        mov    dword ptr [ecx + 6], edx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_2254:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 1]
        push   eax
        push   0x3b
        push   dword ptr [ecx + 8]
        call   sub_1_40A6
loc_4_226C:
        movzx  ecx, byte ptr [ebp - 1]
        add    esp, 0xc
        mov    edx, ecx
        shl    edx, 0x10
        or     edx, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        shl    edx, 8
        mov    dword ptr [ecx + 6], edx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_2287:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 0xc]
        lea    ebx, [edi + 2]
        test   byte ptr [ebx], 2
        je     loc_4_22AA
        push   dword ptr [edi + 6]
        push   dword ptr [ebp + 8]
        call   sub_4_22F8
        add    esp, 8
        mov    esi, eax
loc_4_22AA:
        test   byte ptr [ebx], 1
        je     loc_4_22C3
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_2314
        add    esp, 8
        or     esi, eax
loc_4_22C3:
        test   byte ptr [ebx], 8
        je     loc_4_22D8
        push   dword ptr [edi + 6]
        push   dword ptr [ebp + 8]
        call   sub_4_2330
        add    esp, 8
        or     esi, eax
loc_4_22D8:
        test   byte ptr [ebx], 4
        je     loc_4_22F1
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_234C
        add    esp, 8
        or     esi, eax
loc_4_22F1:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_4_22F8:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   0x46
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2310:
        add    esp, 0xc
        ret    
sub_4_2314:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   0x47
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_232C:
        add    esp, 0xc
        ret    
sub_4_2330:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   0x44
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2348:
        add    esp, 0xc
        ret    
sub_4_234C:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   0x45
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2364:
        add    esp, 0xc
        ret    
sub_4_2368:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        movzx  ecx, word ptr [eax]
        mov    ebp, esp
        test   ecx, ecx
        je     loc_4_239B
        cmp    ecx, 1
        je     loc_4_23A6
        cmp    ecx, 2
        je     loc_4_23B1
        cmp    ecx, 3
        jl     loc_4_2394
        cmp    ecx, 4
        jle    loc_4_23BC
        cmp    ecx, 5
        je     loc_4_23C7
        cmp    ecx, 6
        je     loc_4_23D2
loc_4_2394:
        mov    eax, 0xf
        jmp    loc_4_23DB
loc_4_239B:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_26DA
        jmp    loc_4_23DB
loc_4_23A6:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_2287
        jmp    loc_4_23DB
loc_4_23B1:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_23DF
        jmp    loc_4_23DB
loc_4_23BC:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_24DA
        jmp    loc_4_23DB
loc_4_23C7:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_261E
        jmp    loc_4_23DB
loc_4_23D2:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_26C0
loc_4_23DB:
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_23DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   0x3c
        push   dword ptr [edi + 8]
        call   sub_1_40A6
loc_4_23FA:
        add    esp, 0xc
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [edx + 2]
        test   cl, 2
        je     loc_4_240D
        mov    esi, 0xe
loc_4_240D:
        test   cl, 1
        je     loc_4_2415
        or     esi, 0xe
loc_4_2415:
        test   cl, 8
        je     loc_4_241D
        or     esi, 0xc
loc_4_241D:
        test   cl, 4
        je     loc_4_2425
        or     esi, 0xc
loc_4_2425:
        test   cl, 0x20
        je     loc_4_242D
        or     esi, 0xc
loc_4_242D:
        test   cl, 0x10
        je     loc_4_2435
        or     esi, 0xc
loc_4_2435:
        test   ch, 0x20
        je     loc_4_243D
        or     esi, 0xc
loc_4_243D:
        test   ch, 0x10
        je     loc_4_2445
        or     esi, 0xc
loc_4_2445:
        test   ch, 4
        je     loc_4_244D
        or     esi, 0xc
loc_4_244D:
        mov    eax, 0x80
        test   eax, ecx
        je     loc_4_2465
        test   dword ptr [edx + 6], eax
        je     loc_4_2461
        or     byte ptr [ebp - 1], 4
        jmp    loc_4_2465
loc_4_2461:
        and    byte ptr [ebp - 1], 0xfb
loc_4_2465:
        test   cl, 0x40
        je     loc_4_247A
        test   byte ptr [edx + 6], 0x40
        je     loc_4_2476
        or     byte ptr [ebp - 1], 2
        jmp    loc_4_247A
loc_4_2476:
        and    byte ptr [ebp - 1], 0xfd
loc_4_247A:
        mov    eax, 0x200
        test   eax, ecx
        je     loc_4_2492
        test   dword ptr [edx + 6], eax
        je     loc_4_248E
        or     byte ptr [ebp - 1], 0x10
        jmp    loc_4_2492
loc_4_248E:
        and    byte ptr [ebp - 1], 0xef
loc_4_2492:
        mov    eax, 0x100
        test   eax, ecx
        je     loc_4_24AA
        test   dword ptr [edx + 6], eax
        je     loc_4_24A6
        or     byte ptr [ebp - 1], 8
        jmp    loc_4_24AA
loc_4_24A6:
        and    byte ptr [ebp - 1], 0xf7
loc_4_24AA:
        mov    eax, 0x800
        test   eax, ecx
        je     loc_4_24C2
        test   dword ptr [edx + 6], eax
        je     loc_4_24BE
        or     byte ptr [ebp - 1], 1
        jmp    loc_4_24C2
loc_4_24BE:
        and    byte ptr [ebp - 1], 0xfe
loc_4_24C2:
        push   dword ptr [ebp - 1]
        push   0x3c
        push   dword ptr [edi + 8]
        call   sub_1_4076
loc_4_24CF:
        add    esp, 0xc
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_24DA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [edi]
        cmp    eax, 3
        je     loc_4_24FF
        cmp    eax, 4
        je     loc_4_2503
        mov    eax, 1
        jmp    loc_4_2617
loc_4_24FF:
        mov    bl, 0x3d
        jmp    loc_4_2505
loc_4_2503:
        mov    bl, 0x3e
loc_4_2505:
        lea    eax, [ebp - 1]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ebx
        push   dword ptr [ecx + 8]
        call   sub_1_40A6
loc_4_2515:
        add    esp, 0xc
        mov    ecx, dword ptr [edi + 2]
        test   cl, 0x10
        je     loc_4_2530
        test   byte ptr [edi + 6], 0x10
        je     loc_4_252C
        or     byte ptr [ebp - 1], 0x20
        jmp    loc_4_2530
loc_4_252C:
        and    byte ptr [ebp - 1], 0xdf
loc_4_2530:
        test   cl, 0x20
        je     loc_4_2545
        test   byte ptr [edi + 6], 0x20
        je     loc_4_2541
        or     byte ptr [ebp - 1], 0x40
        jmp    loc_4_2545
loc_4_2541:
        and    byte ptr [ebp - 1], 0xbf
loc_4_2545:
        mov    eax, 0x80
        test   eax, ecx
        je     loc_4_255D
        test   dword ptr [edi + 6], eax
        je     loc_4_2559
        or     byte ptr [ebp - 1], 4
        jmp    loc_4_255D
loc_4_2559:
        and    byte ptr [ebp - 1], 0xfb
loc_4_255D:
        test   cl, 0x40
        je     loc_4_2572
        test   byte ptr [edi + 6], 0x40
        je     loc_4_256E
        or     byte ptr [ebp - 1], 2
        jmp    loc_4_2572
loc_4_256E:
        and    byte ptr [ebp - 1], 0xfd
loc_4_2572:
        mov    eax, 0x200
        test   eax, ecx
        je     loc_4_258A
        test   dword ptr [edi + 6], eax
        je     loc_4_2586
        or     byte ptr [ebp - 1], 0x10
        jmp    loc_4_258A
loc_4_2586:
        and    byte ptr [ebp - 1], 0xef
loc_4_258A:
        mov    eax, 0x100
        test   eax, ecx
        je     loc_4_25A2
        test   dword ptr [edi + 6], eax
        je     loc_4_259E
        or     byte ptr [ebp - 1], 8
        jmp    loc_4_25A2
loc_4_259E:
        and    byte ptr [ebp - 1], 0xf7
loc_4_25A2:
        mov    eax, 0x800
        test   eax, ecx
        je     loc_4_25BA
        test   dword ptr [edi + 6], eax
        je     loc_4_25B6
        or     byte ptr [ebp - 1], 1
        jmp    loc_4_25BA
loc_4_25B6:
        and    byte ptr [ebp - 1], 0xfe
loc_4_25BA:
        mov    eax, 0x400
        test   eax, ecx
        je     loc_4_25D2
        test   dword ptr [edi + 6], eax
        je     loc_4_25CE
        or     byte ptr [ebp - 1], 1
        jmp    loc_4_25D2
loc_4_25CE:
        and    byte ptr [ebp - 1], 0xfe
loc_4_25D2:
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [ebp - 1]
        push   ebx
        push   dword ptr [eax + 8]
        call   sub_1_4076
loc_4_25E1:
        add    esp, 0xc
        mov    eax, dword ptr [edi + 2]
        test   al, 2
        je     loc_4_25F0
        mov    esi, 0xe
loc_4_25F0:
        test   al, 1
        je     loc_4_25F7
        or     esi, 0xe
loc_4_25F7:
        test   al, 8
        je     loc_4_25FE
        or     esi, 0xc
loc_4_25FE:
        test   al, 4
        je     loc_4_2605
        or     esi, 0xc
loc_4_2605:
        test   ah, 0x20
        je     loc_4_260D
        or     esi, 0xc
loc_4_260D:
        test   ah, 0x10
        je     loc_4_2615
        or     esi, 0xc
loc_4_2615:
        mov    eax, esi
loc_4_2617:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_261E:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        lea    ebx, [edi + 2]
        test   byte ptr [ebx], 2
        je     loc_4_2647
        mov    eax, dword ptr [edi + 6]
        mov    esi, dword ptr [esp + 0x10]
        shl    al, 6
        push   eax
        push   0x3f
        push   dword ptr [esi + 8]
        call   sub_1_4076
loc_4_2642:
        add    esp, 0xc
        jmp    loc_4_264B
loc_4_2647:
        mov    esi, dword ptr [esp + 0x10]
loc_4_264B:
        test   byte ptr [ebx], 1
        je     loc_4_2667
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        shl    al, 6
        push   eax
        push   0x40
        push   dword ptr [esi + 8]
        call   sub_1_4076
loc_4_2664:
        add    esp, 0xc
loc_4_2667:
        test   byte ptr [ebx], 4
        je     loc_4_2683
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        shl    al, 6
        push   eax
        push   0x42
        push   dword ptr [esi + 8]
        call   sub_1_4076
loc_4_2680:
        add    esp, 0xc
loc_4_2683:
        test   byte ptr [ebx], 8
        je     loc_4_269C
        mov    eax, dword ptr [edi + 6]
        shl    al, 6
        push   eax
        push   0x41
        push   dword ptr [esi + 8]
        call   sub_1_4076
loc_4_2699:
        add    esp, 0xc
loc_4_269C:
        test   byte ptr [ebx], 0x10
        je     loc_4_26BA
        test   byte ptr [edi + 6], 1
        je     loc_4_26AB
        push   0
        jmp    loc_4_26AD
loc_4_26AB:
        push   1
loc_4_26AD:
        push   0x43
        push   dword ptr [esi + 8]
        call   sub_1_4076
loc_4_26B7:
        add    esp, 0xc
loc_4_26BA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_26C0:
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [eax + 6]
        push   dword ptr [eax + 2]
        mov    eax, dword ptr [esp + 0xc]
        push   dword ptr [eax + 8]
        call   sub_1_4076
loc_4_26D6:
        add    esp, 0xc
        ret    
sub_4_26DA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_306A
loc_4_26EE:
        add    esp, 4
        mov    edi, eax
        push   edi
        call   sub_4_0EE3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   edi
        call   sub_1_597A
loc_4_2705:
        add    esp, 4
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        test   byte ptr [edi + 2], 2
        je     loc_4_2722
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_28B7
        add    esp, 8
        mov    ebx, eax
loc_4_2722:
        test   byte ptr [edi + 2], 1
        je     loc_4_273A
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   esi
        call   sub_4_290C
        add    esp, 8
        or     ebx, eax
loc_4_273A:
        test   byte ptr [edi + 2], 8
        je     loc_4_2768
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2961
        add    esp, 8
        or     ebx, eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_2768
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_2768
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2A0B
        add    esp, 8
        or     ebx, eax
loc_4_2768:
        test   byte ptr [edi + 2], 4
        je     loc_4_279A
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   esi
        call   sub_4_29B6
        add    esp, 8
        or     ebx, eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_279A
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_279A
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2A60
        add    esp, 8
        or     ebx, eax
loc_4_279A:
        test   byte ptr [edi + 2], 0x20
        je     loc_4_27C8
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2A0B
        add    esp, 8
        or     ebx, eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_27C8
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_27C8
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2961
        add    esp, 8
        or     ebx, eax
loc_4_27C8:
        test   byte ptr [edi + 2], 0x10
        je     loc_4_27FE
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   esi
        call   sub_4_2A60
        add    esp, 8
        or     ebx, eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_4_27FE
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_27FE
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   esi
        call   sub_4_29B6
        add    esp, 8
        or     ebx, eax
loc_4_27FE:
        test   byte ptr [edi + 2], 0x80
        je     loc_4_2812
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2AB5
        add    esp, 8
        or     ebx, eax
loc_4_2812:
        test   byte ptr [edi + 2], 0x40
        je     loc_4_282A
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   esi
        call   sub_4_2B0A
        add    esp, 8
        or     ebx, eax
loc_4_282A:
        test   byte ptr [edi + 3], 2
        je     loc_4_283E
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2B5F
        add    esp, 8
        or     ebx, eax
loc_4_283E:
        test   byte ptr [edi + 3], 1
        je     loc_4_2856
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   esi
        call   sub_4_2BB4
        add    esp, 8
        or     ebx, eax
loc_4_2856:
        test   byte ptr [edi + 3], 8
        je     loc_4_286A
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2C09
        add    esp, 8
        or     ebx, eax
loc_4_286A:
        test   byte ptr [edi + 3], 4
        je     loc_4_2882
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   esi
        call   sub_4_2C5E
        add    esp, 8
        or     ebx, eax
loc_4_2882:
        test   byte ptr [edi + 3], 0x20
        je     loc_4_2896
        push   dword ptr [edi + 6]
        push   esi
        call   sub_4_2CB5
        add    esp, 8
        or     ebx, eax
loc_4_2896:
        test   byte ptr [edi + 3], 0x10
        je     loc_4_28AE
        mov    eax, dword ptr [edi + 6]
        shr    eax, 0x10
        push   eax
        push   esi
        call   sub_4_2CD1
        add    esp, 8
        or     ebx, eax
loc_4_28AE:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_28B7:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_28EB
        cmp    ax, 5
        je     loc_4_28EB
        cmp    ax, 7
        je     loc_4_28EB
        cmp    ax, 8
        je     loc_4_28EB
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_28FD
loc_4_28EB:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_28FD:
        push   eax
        push   0x30
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2908:
        add    esp, 0xc
        ret    
sub_4_290C:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2940
        cmp    ax, 5
        je     loc_4_2940
        cmp    ax, 7
        je     loc_4_2940
        cmp    ax, 8
        je     loc_4_2940
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_2952
loc_4_2940:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_2952:
        push   eax
        push   0x31
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_295D:
        add    esp, 0xc
        ret    
sub_4_2961:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2995
        cmp    ax, 5
        je     loc_4_2995
        cmp    ax, 7
        je     loc_4_2995
        cmp    ax, 8
        je     loc_4_2995
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_29A7
loc_4_2995:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_29A7:
        push   eax
        push   0x32
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_29B2:
        add    esp, 0xc
        ret    
sub_4_29B6:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_29EA
        cmp    ax, 5
        je     loc_4_29EA
        cmp    ax, 7
        je     loc_4_29EA
        cmp    ax, 8
        je     loc_4_29EA
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_29FC
loc_4_29EA:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_29FC:
        push   eax
        push   0x33
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2A07:
        add    esp, 0xc
        ret    
sub_4_2A0B:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2A3F
        cmp    ax, 5
        je     loc_4_2A3F
        cmp    ax, 7
        je     loc_4_2A3F
        cmp    ax, 8
        je     loc_4_2A3F
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_2A51
loc_4_2A3F:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_2A51:
        push   eax
        push   0x34
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2A5C:
        add    esp, 0xc
        ret    
sub_4_2A60:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2A94
        cmp    ax, 5
        je     loc_4_2A94
        cmp    ax, 7
        je     loc_4_2A94
        cmp    ax, 8
        je     loc_4_2A94
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_2AA6
loc_4_2A94:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_2AA6:
        push   eax
        push   0x35
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2AB1:
        add    esp, 0xc
        ret    
sub_4_2AB5:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2AE9
        cmp    ax, 5
        je     loc_4_2AE9
        cmp    ax, 7
        je     loc_4_2AE9
        cmp    ax, 8
        je     loc_4_2AE9
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_2AFB
loc_4_2AE9:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_2AFB:
        push   eax
        push   0x36
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2B06:
        add    esp, 0xc
        ret    
sub_4_2B0A:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2B3E
        cmp    ax, 5
        je     loc_4_2B3E
        cmp    ax, 7
        je     loc_4_2B3E
        cmp    ax, 8
        je     loc_4_2B3E
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_2B50
loc_4_2B3E:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_2B50:
        push   eax
        push   0x37
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2B5B:
        add    esp, 0xc
        ret    
sub_4_2B5F:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2B93
        cmp    ax, 5
        je     loc_4_2B93
        cmp    ax, 7
        je     loc_4_2B93
        cmp    ax, 8
        je     loc_4_2B93
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_2BA5
loc_4_2B93:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_2BA5:
        push   eax
        push   0x38
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2BB0:
        add    esp, 0xc
        ret    
sub_4_2BB4:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2BE8
        cmp    ax, 5
        je     loc_4_2BE8
        cmp    ax, 7
        je     loc_4_2BE8
        cmp    ax, 8
        je     loc_4_2BE8
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_2BFA
loc_4_2BE8:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_2BFA:
        push   eax
        push   0x39
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2C05:
        add    esp, 0xc
        ret    
sub_4_2C09:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2C3D
        cmp    ax, 5
        je     loc_4_2C3D
        cmp    ax, 7
        je     loc_4_2C3D
        cmp    ax, 8
        je     loc_4_2C3D
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_2C4F
loc_4_2C3D:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_2C4F:
        push   eax
        push   0x3a
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2C5A:
        add    esp, 0xc
        ret    
sub_4_2C5E:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x28]
        cmp    ax, 4
        je     loc_4_2C92
        cmp    ax, 5
        je     loc_4_2C92
        cmp    ax, 7
        je     loc_4_2C92
        cmp    ax, 8
        je     loc_4_2C92
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xb
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00C8
        jmp    loc_4_2CA4
loc_4_2C92:
        mov    ax, word ptr [esp + 8]
        shr    ax, 0xd
        movzx  edx, al
        mov    al, byte ptr [edx]            ; -> obj5:0x00E8
loc_4_2CA4:
        push   eax
        push   0x3a
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2CAF:
        add    esp, 0xc
        or     al, 0xc
        ret    
sub_4_2CB5:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   0x3b
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2CCD:
        add    esp, 0xc
        ret    
sub_4_2CD1:
        mov    ax, word ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shr    ax, 8
        push   eax
        push   0x3b
        push   dword ptr [ecx + 8]
        call   sub_1_4076
loc_4_2CE9:
        add    esp, 0xc
        or     al, 0xc
        ret    
sub_4_2CEF:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    word ptr [esi + 6], ax
        lea    ecx, [ebp - 2]
        push   ecx
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [esi + 0x2c], 3
        mov    word ptr [esi + 2], 2
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [esi + 4], 0x199
        push   dword ptr [edx + 8]
        call   sub_1_4807
loc_4_2D24:
        add    esp, 8
        add    esi, 8
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        push   dword ptr [0]                 ; -> obj1:0x91E0
        push   0                             ; -> loc_1_91FC
        push   esi
        call   sub_1_2403
loc_4_2D40:
        add    esp, 0x10
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
sub_4_2D4C:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_4_2D54:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_4_2D5C:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_4_2D64:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_4_2D6C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_4_2D74:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  0CCh
sub_4_2D80:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_4_2D90:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Allocate               ; cd209d000100
        mov    esi, ecx
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_4_2DA4:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    eax, esi
        pop    esi
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_4_2DB8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_4_2DC8:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_4_2DD8:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_4_2DE8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_4_2DF8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_4_2E0C:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        VxDCall  APM.Service_0002h               ; cd2002002700
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_4_2E20:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_4_2E3E
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_4_2E3E:
        ret    0x10
        db  0CCh, 0CCh, 0CCh
sub_4_2E44:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        int3   
        int3   
sub_4_2E4C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0xf0  RW-
        dd  loc_4_0075               ; fixup
        dd  loc_4_00CA               ; fixup
        dd  loc_4_00D8               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_012E               ; fixup
        dd  loc_4_0145               ; fixup
        dd  loc_4_0158               ; fixup
        dd  loc_4_016B               ; fixup
        dd  loc_4_01B3               ; fixup
        dd  loc_4_01F2               ; fixup
        dd  loc_4_0208               ; fixup
        dd  loc_4_0245               ; fixup
        dd  loc_4_027A               ; fixup
        dd  loc_4_02AF               ; fixup
        dd  loc_4_030A               ; fixup
        dd  loc_4_0366               ; fixup
        dd  loc_4_03B0               ; fixup
        dd  loc_4_0405               ; fixup
        dd  loc_4_0413               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_0437               ; fixup
        dd  loc_4_044E               ; fixup
        dd  loc_4_0461               ; fixup
        dd  loc_4_0474               ; fixup
        dd  loc_4_0566               ; fixup
        dd  loc_4_058B               ; fixup
        dd  loc_4_05AD               ; fixup
        dd  loc_4_0631               ; fixup
        dd  loc_4_0735               ; fixup
        db  053h, 042h, 031h, 036h, 02Eh, 056h, 058h, 044h, 000h, 000h, 000h, 000h, 025h, 073h, 020h, 05Bh
        db  025h, 030h, 033h, 078h, 05Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 064h, 073h, 06Fh, 075h
        db  06Eh, 064h, 02Eh, 076h, 078h, 064h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 008h, 008h, 008h, 010h, 010h, 018h, 018h
        db  020h, 028h, 030h, 038h, 040h, 050h, 060h, 078h, 090h, 0A8h, 0D0h, 0F8h, 000h, 020h, 020h, 020h
        db  040h, 060h, 0A0h, 0E0h, 000h, 060h, 080h, 090h, 0A0h, 0A8h, 0B0h, 0B8h, 0C0h, 0C8h, 0C8h, 0D0h
        db  0D0h, 0D8h, 0D8h, 0D8h, 0E0h, 0E0h, 0E0h, 0E8h, 0E8h, 0E8h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F8h
        db  0F8h, 0F8h, 0F8h, 0F8h, 000h, 060h, 080h, 0A0h, 0C0h, 0C0h, 0E0h, 0E0h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x2d  R-X
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        push   word ptr [ebp + 8]
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        or     eax, eax
        je     loc_6_0024
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc    
        jmp    loc_6_002A
loc_6_0024:
        stc    
        mov    eax, 1
loc_6_002A:
        pop    esi
        leave  
        ret    
VxD__LTEXT ENDS

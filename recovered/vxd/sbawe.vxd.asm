; SBAWE -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 16096 instructions recovered by recursive descent

Declare_Virtual_Device SBAWE, 4, 30, SBAWE_Control, 032A4h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x553c  R-X
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
        sal    byte ptr [ebx], 0x42
        mov    esp, cs
        mov    ah, 0xce
        adc    dword ptr [edx + 0x53454494], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0210:
        sal    dword ptr [ebx], 0x42
        mov    esp, cs
        mov    ah, 0xce
        adc    dword ptr [edx + 0x53454494], edx
        push   esp
        add    byte ptr [eax], al
sub_1_0220:
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0xff
        mov    esi, dword ptr [esp + 0x10]
        push   0
        push   1
        push   ebx
        push   esi
        call   sub_1_1A68
        add    esp, 0x10
        mov    edi, eax
        mov    eax, 1
        cmp    edi, ebx
        je     loc_1_026F
        mov    eax, dword ptr [esp + 0x14]
        push   0x80
        mov    dword ptr [eax], edi
        mov    eax, edi
        or     eax, 0x2a0
        or     edi, 0x60
        push   eax
        push   esi
        call   sub_1_19AA
        push   0xffff
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
        push   0xffff
        or     al, 0x60
        mov    esi, dword ptr [esp + 0xc]
        push   eax
        push   esi
        call   sub_1_1A14
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    word ptr [eax + esi + 0x42], 0xffe
        xor    eax, eax
        pop    esi
        ret    
sub_1_029C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    dword ptr [ebp - 4], 0
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
loc_1_02B1:
        push   0
        push   0
        push   0xff
        push   esi
        call   sub_1_1A68
        add    esp, 0x10
        mov    byte ptr [ebp + ebx - 0xc], al
        movzx  edi, al
        cmp    edi, 0xff
        je     loc_1_035D
        mov    eax, edi
        inc    dword ptr [ebp - 4]
        shl    eax, 5
        cmp    byte ptr [eax + esi + 0x59], 0
        je     loc_1_02E8
        dec    dword ptr [esi + 0x18]
loc_1_02E8:
        push   0x80
        mov    eax, edi
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19AA
        push   0
        mov    eax, edi
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 0x40
        push   eax
        push   esi
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
        push   0x40000000
        mov    eax, edi
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
        push   0x40000000
        push   edi
        push   esi
        or     edi, 0x200
        call   sub_1_1A14
        push   0x6000000
        push   edi
        push   esi
        call   sub_1_1A14
loc_1_035D:
        inc    ebx
        cmp    ebx, 5
        jb     loc_1_02B1
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0377
        mov    eax, 1
        jmp    loc_1_040C
loc_1_0377:
        push   dword ptr [ebp + 0xc]
        push   0x236
        push   esi
        call   sub_1_1A14
        mov    eax, 0x3a
        mov    edi, dword ptr [ebp + 0x14]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        test   edi, edi
        out    dx, ax
        je     loc_1_03AC
        movzx  edx, word ptr [esi + 2]
        mov    ecx, dword ptr [ebp + 0x10]
loc_1_03A1:
        mov    ax, word ptr [ecx]
        add    ecx, 2
        out    dx, ax
        dec    edi
        jne    loc_1_03A1
loc_1_03AC:
        mov    dx, word ptr [esi + 2]
        xor    eax, eax
        out    dx, ax
        xor    edi, edi
loc_1_03B6:
        movzx  ebx, byte ptr [ebp + edi - 0xc]
        cmp    ebx, 0xff
        je     loc_1_0404
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
        push   0x1d8
        mov    eax, ebx
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
        push   0x1e0
        mov    eax, ebx
        shl    ebx, 5
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
        lea    eax, [ebx + esi]
        mov    word ptr [eax + 0x42], 0xffe
        mov    byte ptr [eax + 0x59], 1
        inc    dword ptr [esi + 0x18]
loc_1_0404:
        inc    edi
        cmp    edi, 5
        jb     loc_1_03B6
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
        add    byte ptr [eax + 0x800000], 0
        add    byte ptr [eax + 0x7f007d], al
        jg     loc_1_0A02
loc_1_0A02:
        add    byte ptr [eax], al
        jg     loc_1_0A06
loc_1_0A06:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x7d], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jg     loc_1_0A16
loc_1_0A16:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        jg     loc_1_0A23
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
        jg     loc_1_0A9C
loc_1_0A9C:
        and    cl, dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    cl, dl
        and    cl, dl
        and    cl, dl
        and    cl, dl
        jg     loc_1_0AAC
loc_1_0AAC:
        and    cl, dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        jg     loc_1_0ABB
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
        add    byte ptr [ebx + 0x31], ah
        xor    byte ptr [edi], ch
        sub    eax, 0x2a2a2b2c
        sub    dword ptr [eax], ebp
        daa    
        and    eax, 0x22232424
        and    dword ptr [ecx], esp
        and    byte ptr [edi], bl
        push   ds
        push   ds
        sbb    eax, 0x1a1b1c1c
        sbb    bl, byte ptr [ecx]
        sbb    dword ptr [eax], ebx
        sbb    byte ptr [edi], dl
        push   ss
        push   ss
        adc    eax, 0x13141415
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
        arpl   word ptr [ebx + 0x63], sp
        arpl   word ptr [ebx + 0x63], sp
        arpl   word ptr [ebx + 0x63], sp
        arpl   word ptr [ebx + 0x2b], sp
        sub    dword ptr [eax], ebp
        daa    
        and    eax, 0x21222324
        and    byte ptr [edi], bl
        push   ds
        push   ds
        sbb    eax, 0x1a1b1b1c
        sbb    dword ptr [ecx], ebx
        sbb    byte ptr [edi], dl
        pop    ss
        push   ss
        push   ss
        adc    eax, 0x13141415
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
        jg     loc_1_0C66
        bound  ebx, qword ptr [edx + 0x54]
        push   eax
        dec    ebx
        dec    eax
        inc    ebp
        inc    edx
        inc    eax
        cmp    eax, 0x3638393b
        xor    al, 0x33
        xor    dword ptr [eax], esi
        das    
        sub    eax, 0x292a2b2c
        sub    byte ptr [edi], ah
        and    eax, 0x22232424
        and    dword ptr [ecx], esp
        and    byte ptr [edi], bl
        push   ds
        push   ds
        sbb    eax, 0x1b1b1c1d
        sbb    bl, byte ptr [edx]
        sbb    dword ptr [eax], ebx
        sbb    byte ptr [edi], dl
        pop    ss
        push   ss
        push   ss
        adc    eax, 0x14141515
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
        shr    dl, 0xc8
        loopne loc_1_0CBD
        xlatb  
        and    dh, cl
        pushal 
        lds    ecx, ptr [eax]
        mov    ebp, 0xad58b500
        clc    
        movsd  dword ptr es:[edi], dword ptr [esi]
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
        add    byte ptr [eax], al            ; -> sub_4_25AA
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0CC8:
        adc    eax, 0
        add    byte ptr [esi], dl
        add    dword ptr [eax + 0x17000004], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    dword ptr [eax + 0x19000004], esp
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
        add    byte ptr [0x1046001], al
        add    byte ptr [esi], al
        add    dword ptr [eax + 0x7000104], esp
        add    dword ptr [eax + 4], eax
        add    dword ptr [eax], eax
        or     eax, 0x1048001
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
        ja     0xd97
        lcall  0xa107, 0xbc09480b
        push   es
        int    5
        sub    byte ptr [0x43804a4], al
        fiadd  word ptr [ebx]
        xchg   edx, eax
        add    edx, dword ptr [eax + 3]
        sbb    byte ptr [ebx], al
        out    2, al
        mov    ebx, 0x71029402
        add    dl, byte ptr [edx + 2]
        add    bl, byte ptr ss:[edx + eax]
        add    al, 2
        out    dx, eax
        add    ebx, ebx
        add    ecx, ecx
        add    dword ptr [eax - 0x65fe57ff], edi
        add    dword ptr [ecx + eax + 0x173017f], ecx
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
        add    byte ptr [edx - 0x5aff5100], bh
        add    byte ptr [eax + eax + 0x8d0094], bl
        xchg   dword ptr [eax], eax
        add    dword ptr [eax], 0x77007c
        jb     loc_1_0DF6
loc_1_0DF6:
        outsb  dx, byte ptr [esi]
        add    byte ptr [edx], ch
        add    byte ptr [ebx], ah
        pushal 
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
        add    byte ptr [0x31003300], dh
        add    byte ptr [eax], dh
        add    byte ptr [esi], ch
        add    byte ptr [eax + eax], ch
        sub    dword ptr [eax], eax
        daa    
        add    byte ptr [0x22002300], ah
        add    byte ptr [eax], ah
        add    byte ptr [edi], bl
        add    byte ptr [esi], bl
        add    byte ptr [0x1b001b00], bl
        add    byte ptr [edx], bl
        add    byte ptr [ecx], bl
        add    byte ptr [eax], bl
        add    byte ptr [edi], dl
        add    byte ptr [esi], dl
        add    byte ptr [0x13001400], dl
        add    byte ptr [edx], dl
        add    byte ptr [ecx], dl
        add    byte ptr [eax], dl
        add    byte ptr [edi], cl
        add    byte ptr [edi], cl
        add    byte ptr [esi], cl
        add    byte ptr [esi], cl
        add    byte ptr [0xc000d00], cl
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
        mov    ecx, 0x3dde5ccc
        pop    ds
        and    eax, 0x1a841eef
        xor    edx, dword ptr [edi]
        lahf   
        adc    al, 0x8f
        adc    bl, bh
        adc    byte ptr [edi + 0xf], dh
        inc    edi
        push   cs
        inc    edx
        or     eax, 0xb9a0c60
        jmp    loc_1_0EAC
        db  050h, 00Ah, 0C5h, 009h, 048h, 009h, 0D7h, 008h, 070h, 008h
loc_1_0EAC:
        adc    cl, byte ptr [eax]
        mov    esp, 0x23076d07
        pop    es
        loopne loc_1_0EBC
        mov    eax, dword ptr [0x30066606]
        push   es
loc_1_0EBC:
        std    
        add    eax, 0x57505cd
        sub    byte ptr [0x4a404e2], al
        imul   eax, dword ptr [eax + edi], 4
        or     dword ptr [esi + ebx*8], eax
        add    esi, dword ptr [esi + 0x70039203]
        add    edx, dword ptr [eax + 3]
        xor    eax, dword ptr [ebx]
        sbb    byte ptr [ebx], al
        inc    byte ptr [edx]
        out    2, al
        mov    ebx, 0x71029402
        add    dl, byte ptr [edx + 2]
        add    bl, byte ptr ss:[edx + eax]
        add    al, 2
        out    dx, eax
        add    ebx, ebx
        add    ecx, ecx
        add    dword ptr [eax - 0x65fe57ff], edi
        add    dword ptr [ecx + eax + 0x173017f], ecx
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
        add    byte ptr [edx - 0x5aff5100], bh
        add    byte ptr [eax + eax + 0x8d0094], bl
        xchg   dword ptr [eax], eax
        add    dword ptr [eax], 0x77007c
        jb     loc_1_0F34
loc_1_0F34:
        outsb  dx, byte ptr [esi]
        add    byte ptr [edx], ch
        add    byte ptr [ebx], ah
        pushal 
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
        add    byte ptr [0x31003300], dh
        add    byte ptr [eax], dh
        add    byte ptr [esi], ch
        add    byte ptr [eax + eax], ch
        sub    dword ptr [eax], eax
        daa    
        add    byte ptr [0x22002300], ah
        add    byte ptr [eax], ah
        add    byte ptr [edi], bl
        add    byte ptr [esi], bl
        add    byte ptr [0x1b001b00], bl
        add    byte ptr [edx], bl
        add    byte ptr [ecx], bl
        add    byte ptr [eax], bl
        add    byte ptr [edi], dl
        add    byte ptr [esi], bh
        jmp    loc_1_0F8A
loc_1_0F8A:
        int3   
        int3   
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  032A4h                      ; DDB_Req_Device_Number
        db  4, 30                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SBAWE   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  SBAWE_Control            ; DDB_Control_Proc
        dd  SBAWE_PM_API             ; DDB_V86_API_Proc
        dd  SBAWE_PM_API             ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
loc_1_0FE0:
        mov    edi, 0xb8cccccc
        push   ds
        add    al, 0
        add    bl, al
sub_1_0FEA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> obj1:0x0418
        test   esi, esi
        je     loc_1_1008
        mov    eax, dword ptr [ebp + 0x14]
loc_1_0FFC:
        cmp    dword ptr [esi + 8], eax
        je     loc_1_1011
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    loc_1_0FFC
loc_1_1008:
        mov    eax, 1
loc_1_100D:
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_1011:
        mov    ax, word ptr [ebp + 0x10]
        cmp    ax, 0x64
        jb     loc_1_10F2
        movzx  eax, ax
        cmp    eax, 0x64
        je     loc_1_104A
        cmp    eax, 0x65
        je     loc_1_1068
        cmp    eax, 0x66
        je     loc_1_107B
        cmp    eax, 0x67
        je     loc_1_10BE
        cmp    eax, 0x68
        je     loc_1_10E1
        mov    eax, 0x8006
        jmp    loc_1_100D
loc_1_104A:
        cmp    dword ptr [esi], 0
        je     loc_1_1056
        mov    eax, 0x8001
        jmp    loc_1_100D
loc_1_1056:
        push   esi
        call   sub_1_5115
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        xor    eax, eax
        jmp    loc_1_100D
loc_1_1068:
        push   esi
        call   sub_1_5118
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi], 0
        jmp    loc_1_100D
loc_1_107B:
        mov    edi, dword ptr [ebp + 0x1c]
        push   dword ptr [edi]
        call   sub_4_0D74
loc_1_1085:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_1_1008
        push   dword ptr [edi]
        push   esi
        call   sub_4_0F21
loc_1_109A:
        add    esp, 8
        test   eax, eax
        jne    loc_1_10AF
        push   esi
        call   sub_4_0D91
loc_1_10A7:
        add    esp, 4
        jmp    loc_1_1008
loc_1_10AF:
        shl    eax, 0x10
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    loc_1_100D
loc_1_10BE:
        mov    edi, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [edi]
        shr    eax, 0x10
        push   eax
        call   sub_4_0F4B
loc_1_10CC:
        add    esp, 4
        push   dword ptr [edi + 4]
        call   sub_4_0D91
loc_1_10D7:
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_100D
loc_1_10E1:
        push   dword ptr [ebp + 0x1c]
        push   esi
        call   sub_4_4159
loc_1_10EA:
        add    esp, 8
        jmp    loc_1_100D
loc_1_10F2:
        push   dword ptr [ebp + 0x1c]
        add    esi, 0x24
        push   esi
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_52FB
        add    esp, 0x10
        jmp    loc_1_100D
sub_1_110A:
        mov    eax, dword ptr [esp + 4]
loc_1_110E:
        cmp    dword ptr [0], 0              ; -> obj1:0x0418
        je     loc_1_1126
        mov    ecx, dword ptr [0]            ; -> obj1:0x0418
        cmp    dword ptr [ecx + 8], eax
        jne    loc_1_110E
        mov    eax, ecx
        jmp    loc_1_1128
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
        push   dword ptr [ebp + 0xc]
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_1156
        add    eax, dword ptr [ebp + 0x10]
        cmp    eax, 0x110000
        jae    loc_1_1156
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
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, 0                        ; -> loc_1_115C
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
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
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
SBAWE_Control:
        cmp    eax, 0xb
        je     loc_4_0ED8
loc_1_11A9:
        cmp    eax, 2
        je     sub_4_0BE2
loc_1_11B2:
        cmp    eax, 0x1b
        je     loc_4_0E9E
loc_1_11BB:
        cmp    eax, 0x1c
        je     loc_4_0EBB
loc_1_11C4:
        cmp    eax, 0x22
        je     loc_3_011C
loc_1_11CD:
        clc    
        ret    
        db  090h, 0EBh, 006h, 0FFh, 025h
        dd  obj1:0x0FDC              ; fixup
loc_1_11D8:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_1221
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0478
        jne    loc_1_1205
        xor    eax, eax
        mov    dword ptr [0], eax            ; -> obj1:0x0478
        mov    edi, dword ptr [0]            ; -> obj1:0x0474
        cmp    dword ptr [edi], 0
        jne    loc_1_1205
        push   edi
        call   sub_1_5118
        add    esp, 4
loc_1_1205:
        mov    edi, dword ptr [0]            ; -> obj1:0x0418
loc_1_120B:
        or     edi, edi
        je     loc_1_1221
        cmp    ebx, dword ptr [edi]
        je     loc_1_1218
        mov    edi, dword ptr [edi + 4]
        jmp    loc_1_120B
loc_1_1218:
        push   edi
        call   sub_4_0CC3
loc_1_121E:
        add    esp, 4
loc_1_1221:
        popfd  
        popal  
        jmp    dword ptr [0]                 ; -> obj1:0x0FDC
SBAWE_PM_API:
        pushfd 
        cld    
        push   ebp
        xor    eax, eax
        cmp    word ptr [ebp + 0x38], 0
        je     loc_1_123F
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
loc_1_123F:
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
        call   sub_1_0FEA
        add    esp, 0x18
        pop    ebp
        popfd  
        mov    word ptr [ebp + 0x1c], ax
        mov    cl, 0x10
        shr    eax, cl
        mov    word ptr [ebp + 0x14], ax
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_1278:
        push   eax
        mov    eax, dword ptr [esi]
        cmp    eax, ebx
        jne    loc_1_12B4
loc_1_127F:
        pop    eax
        cmp    ecx, 0xc
        jne    loc_1_1288
        out    dx, ax
        ret    
loc_1_1288:
        cmp    ecx, 4
        jne    loc_1_1290
        out    dx, al
        ret    
        db  090h
loc_1_1290:
        cmp    ecx, 0x14
        jne    loc_1_1298
        out    dx, eax
        ret    
        db  090h
loc_1_1298:
        cmp    ecx, 8
        jne    loc_1_12A0
        in     ax, dx
        ret    
loc_1_12A0:
        cmp    ecx, 0x10
        jne    loc_1_12A8
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
        jne    loc_1_12AC
        mov    dword ptr [esi], ebx
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        call   sub_4_0D1E
loc_1_12C4:
        add    esp, 4
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        jmp    loc_1_127F
        db  02Eh, 08Bh, 0C0h
loc_1_12D0:
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0478
        je     loc_1_12FB
        cmp    dword ptr [0], 0              ; -> obj1:0x0478
        je     loc_1_12E8
        mov    eax, 0xffffffff
        stc    
        ret    
loc_1_12E8:
        mov    dword ptr [0], ebx            ; -> obj1:0x0478
        pushal 
        mov    ebp, esp
        push   esi
        call   sub_1_152E
        add    esp, 4
        popal  
loc_1_12FB:
        cmp    ecx, 4
        jne    loc_1_1340
        test   edx, 1
        jne    loc_1_1328
        test   edx, 0x8000
        je     loc_1_1318
        test   edx, 0x100
        jne    loc_1_1328
loc_1_1318:
        pushal 
        mov    ebp, esp
        push   eax
        push   esi
        call   sub_1_153E
        add    esp, 8
        popal  
        ret    
        db  090h
loc_1_1328:
        cmp    al, 0xff
        je     loc_1_1334
        cmp    al, 0x3f
        je     loc_1_1334
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_1334:
        push   eax
        mov    al, 0x3f
        mov    byte ptr [0], al              ; -> loc_1_0FE0
        pop    eax
        ret    
        db  08Bh, 0C0h
loc_1_1340:
        cmp    ecx, 0
        jne    loc_1_136E
        test   edx, 1
        jne    loc_1_1368
        test   edx, 0x8000
        je     loc_1_135D
        test   edx, 0x100
        jne    loc_1_1368
loc_1_135D:
        mov    al, 0xbf
        mov    byte ptr [0], al              ; -> loc_1_0FE0
        mov    al, 0xfe
        ret    
        db  090h
loc_1_1368:
        mov    al, byte ptr [0]              ; -> loc_1_0FE0
        ret    
loc_1_136E:
        mov    eax, 0xffffffff
        stc    
        ret    
sub_1_1375:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        test   eax, eax
        push   esi
        je     loc_1_13AF
        cmp    eax, 1
        je     loc_1_1424
        cmp    eax, 5
        je     loc_1_143B
        cmp    eax, 6
        je     loc_1_14A4
        cmp    eax, 7
        je     loc_1_14B3
        mov    eax, 0xffffffff
        jmp    loc_1_14F5
loc_1_13AF:
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> sub_1_0480
        je     loc_1_14F3
        push   esi
        mov    eax, dword ptr [0]            ; -> loc_1_047C
        push   esi
        push   esi
        push   eax
        call   sub_1_0480
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_14F3
        push   esi
        mov    eax, dword ptr [0]            ; -> loc_1_047C
        push   esi
        push   2
        push   eax
        call   sub_1_0480
        mov    word ptr [0], ax              ; -> obj1:0x0468
        add    esp, 0x10
        cmp    ax, si
        jne    loc_1_13FF
        mov    eax, 1
        jmp    loc_1_14F5
loc_1_13FF:
        mov    eax, dword ptr [0]            ; -> obj1:0x0418
        push   0
        push   0
        mov    dword ptr [0], eax            ; -> obj1:0x0474
        mov    dword ptr [0], 1              ; -> obj1:0x0464
        push   3
        push   dword ptr [0]                 ; -> loc_1_047C
        jmp    loc_1_14EA
loc_1_1424:
        xor    eax, eax
        mov    word ptr [0], ax              ; -> obj1:0x0468
        mov    dword ptr [0], eax            ; -> sub_1_0480
        mov    dword ptr [0], eax            ; -> obj1:0x0464
        jmp    loc_1_14F3
loc_1_143B:
        xor    eax, eax
        cmp    dword ptr [0], eax            ; -> obj1:0x0464
        je     loc_1_14F3
        cmp    dword ptr [0], eax            ; -> obj1:0x0478
        jne    loc_1_1486
        mov    ecx, dword ptr [0]            ; -> obj1:0x0474
        cmp    dword ptr [ecx], eax
        jne    loc_1_14F3
        push   0x10
        lea    eax, [ecx + 0x24]
        push   eax
        call   sub_1_18E9
        push   dword ptr [ebp + 0xc]
        push   dword ptr [0]                 ; -> obj1:0x0474
        call   sub_1_153E
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [0], ecx            ; -> obj1:0x0478
        jmp    loc_1_14F3
loc_1_1486:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, dword ptr [0]            ; -> obj1:0x0478
        jne    loc_1_14F3
        push   dword ptr [ebp + 0xc]
        push   dword ptr [0]                 ; -> obj1:0x0474
        call   sub_1_153E
        add    esp, 8
        jmp    loc_1_14F3
loc_1_14A4:
        xor    eax, eax
        mov    word ptr [0], ax              ; -> obj1:0x0468
        mov    dword ptr [0], eax            ; -> obj1:0x0464
        jmp    loc_1_14F3
loc_1_14B3:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, dword ptr [0]            ; -> obj1:0x0478
        jne    loc_1_14F3
        mov    eax, dword ptr [0]            ; -> obj1:0x0474
        cmp    dword ptr [eax], 0
        jne    loc_1_14D1
        push   eax
        call   sub_1_5118
        add    esp, 4
loc_1_14D1:
        xor    eax, eax
        mov    dword ptr [0], eax            ; -> obj1:0x0478
        cmp    dword ptr [0], eax            ; -> obj1:0x0464
        jne    loc_1_14F3
        push   eax
        push   eax
        push   4
        mov    eax, dword ptr [0]            ; -> loc_1_047C
        push   eax
loc_1_14EA:
        call   sub_1_0480
        add    esp, 0x10
loc_1_14F3:
        xor    eax, eax
loc_1_14F5:
        pop    esi
        pop    ebp
        ret    
sub_1_14F8:
        cmp    dword ptr [0], 0              ; -> loc_1_0484
        jne    loc_1_152B
        cmp    dword ptr [0], 0              ; -> obj1:0x0418
        je     loc_1_152B
        push   1
        mov    eax, dword ptr [0]            ; -> obj1:0x0418
        mov    dword ptr [0], 1              ; -> loc_1_0484
        push   0                             ; -> loc_3_11E3
        push   dword ptr [eax + 8]
        call   sub_3_1D20
loc_1_1528:
        add    esp, 0xc
loc_1_152B:
        xor    eax, eax
        ret    
sub_1_152E:
        mov    eax, dword ptr [esp + 4]
        push   0x10
        add    eax, 0x24
        push   eax
        call   sub_1_18E9
        ret    
sub_1_153E:
        cmp    dword ptr [0], 0              ; -> obj1:0x0464
        push   ebp
        mov    ebp, esp
        je     loc_1_17C0
        mov    eax, dword ptr [0]            ; -> obj1:0x0474
        cmp    dword ptr [eax], 0
        jne    loc_1_17C0
        mov    cl, byte ptr [ebp + 0xc]
        test   cl, 0x80
        je     loc_1_15EE
        cmp    cl, 0xf7
        jne    loc_1_15A3
        cmp    dword ptr [0], 2              ; -> obj1:0x046C
        jne    loc_1_15A3
        mov    eax, dword ptr [0]            ; -> obj1:0x0470
        inc    dword ptr [0]                 ; -> obj1:0x0470
        mov    byte ptr [eax], cl            ; -> loc_1_0488
        mov    eax, dword ptr [0]            ; -> obj1:0x0470
        push   eax
        push   0                             ; -> loc_1_0488
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        call   sub_1_4B8B
        jmp    loc_1_17B6
loc_1_15A3:
        cmp    cl, 0xf8
        jae    loc_1_17C0
        mov    eax, 1
        mov    byte ptr [0], cl              ; -> loc_1_0488
        movzx  edx, cl
        and    dl, 0xf0
        mov    dword ptr [0], eax            ; -> obj1:0x0470
        cmp    edx, 0xf0
        je     loc_1_15D4
        mov    dword ptr [0], eax            ; -> obj1:0x046C
        jmp    loc_1_17C0
loc_1_15D4:
        sub    cl, 0xf0
        cmp    cl, 1
        sbb    eax, eax
        and    ax, 2
        movzx  ecx, ax
        mov    dword ptr [0], ecx            ; -> obj1:0x046C
        jmp    loc_1_17C0
loc_1_15EE:
        xor    eax, eax
        cmp    dword ptr [0], eax            ; -> obj1:0x046C
        je     loc_1_17C0
        mov    edx, dword ptr [0]            ; -> obj1:0x0470
        inc    dword ptr [0]                 ; -> obj1:0x0470
        mov    byte ptr [edx], cl            ; -> loc_1_0488
        cmp    dword ptr [0], 2              ; -> obj1:0x046C
        jne    loc_1_162E
        cmp    dword ptr [0], 0x20           ; -> obj1:0x0470
        jne    loc_1_17C0
        mov    dword ptr [0], eax            ; -> obj1:0x046C
        jmp    loc_1_17C0
loc_1_162E:
        cmp    dword ptr [0], 3              ; -> obj1:0x0470
        je     loc_1_164A
        mov    al, byte ptr [0]              ; -> loc_1_0488
        and    al, 0xf0
        cmp    al, 0xc0
        je     loc_1_164A
        cmp    al, 0xd0
        jne    loc_1_17C0
loc_1_164A:
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> loc_1_0488
        and    eax, 0xf0
        cmp    eax, 0x80
        je     loc_1_16A0
        cmp    eax, 0x90
        je     loc_1_16CC
        cmp    eax, 0xa0
        je     loc_1_16F8
        cmp    eax, 0xb0
        je     loc_1_1724
        cmp    eax, 0xc0
        je     loc_1_174D
        cmp    eax, 0xd0
        je     loc_1_176E
        cmp    eax, 0xe0
        je     loc_1_178F
        jmp    loc_1_17B6
loc_1_16A0:
        movzx  eax, byte ptr [2]             ; -> loc_1_048A
        movzx  ecx, byte ptr [1]             ; -> obj1:0x0489
        push   eax
        push   ecx
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> loc_1_0488
        and    eax, 0xf
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        call   sub_1_3C92
        jmp    loc_1_17B6
loc_1_16CC:
        movzx  eax, byte ptr [2]             ; -> loc_1_048A
        movzx  ecx, byte ptr [1]             ; -> obj1:0x0489
        push   eax
        push   ecx
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> loc_1_0488
        and    eax, 0xf
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        call   sub_1_3DB3
        jmp    loc_1_17B6
loc_1_16F8:
        movzx  eax, byte ptr [2]             ; -> loc_1_048A
        movzx  ecx, byte ptr [1]             ; -> obj1:0x0489
        push   eax
        push   ecx
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> loc_1_0488
        and    eax, 0xf
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        call   sub_1_4A99
        jmp    loc_1_17B6
loc_1_1724:
        movzx  eax, byte ptr [2]             ; -> loc_1_048A
        movzx  ecx, byte ptr [1]             ; -> obj1:0x0489
        push   eax
        push   ecx
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> loc_1_0488
        and    eax, 0xf
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        call   sub_1_42A5
        jmp    loc_1_17B6
loc_1_174D:
        movzx  eax, byte ptr [1]             ; -> obj1:0x0489
        push   eax
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> loc_1_0488
        and    eax, 0xf
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        call   sub_1_488D
        jmp    loc_1_17B6
loc_1_176E:
        movzx  eax, byte ptr [1]             ; -> obj1:0x0489
        push   eax
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> loc_1_0488
        and    eax, 0xf
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        call   sub_1_4A9E
        jmp    loc_1_17B6
loc_1_178F:
        movzx  eax, byte ptr [2]             ; -> loc_1_048A
        movzx  ecx, byte ptr [1]             ; -> obj1:0x0489
        push   eax
        push   ecx
        xor    eax, eax
        mov    al, byte ptr [0]              ; -> loc_1_0488
        and    eax, 0xf
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        call   sub_1_4AD6
loc_1_17B6:
        mov    dword ptr [0], 1              ; -> obj1:0x0470
loc_1_17C0:
        pop    ebp
        ret    
loc_1_17C2:
        push   esi
        mov    eax, dword ptr [0]            ; -> loc_1_04BC
        cmp    dword ptr [0], eax            ; -> loc_1_04C0
        je     loc_1_1839
        push   0
        push   0                             ; -> loc_1_17C2
        push   5
        call   sub_1_116C
        add    esp, 0xc
        mov    esi, dword ptr [0]            ; -> loc_1_04C0
        mov    dword ptr [0], eax            ; -> loc_1_04B8
        cmp    esi, dword ptr [0]            ; -> loc_1_04BC
        je     loc_1_1843
loc_1_17F4:
        mov    ecx, dword ptr [0]            ; -> loc_1_04BC
        imul   ecx, ecx, 0x14
        inc    dword ptr [0]                 ; -> loc_1_04BC
        mov    edx, dword ptr [ecx + 4]      ; -> loc_1_04CC
        and    dword ptr [0], 0x3f           ; -> loc_1_04BC
        test   edx, edx
        je     loc_1_182F
        push   dword ptr [ecx + 0x10]        ; -> loc_1_04D8
        push   dword ptr [ecx + 0xc]         ; -> loc_1_04D4
        push   dword ptr [esp + 0x10]
        push   dword ptr [ecx + 8]           ; -> loc_1_04D0
        call   edx
        add    esp, 0x10
loc_1_182F:
        cmp    esi, dword ptr [0]            ; -> loc_1_04BC
        jne    loc_1_17F4
        jmp    loc_1_1843
loc_1_1839:
        mov    dword ptr [0], 0              ; -> loc_1_04B8
loc_1_1843:
        pop    esi
        ret    
sub_1_1845:
        push   ebp
        mov    edx, dword ptr [0]            ; -> loc_1_04C0
        mov    ebp, esp
        mov    eax, dword ptr [0]            ; -> loc_1_04BC
        push   ebx
        lea    ecx, [edx + 1]
        push   esi
        and    ecx, 0x3f
        push   edi
        cmp    ecx, eax
        jne    loc_1_1867
        mov    eax, 1
        jmp    loc_1_18E2
loc_1_1867:
        pushfd 
        cli    
        mov    edi, dword ptr [ebp + 8]
        cmp    edx, eax
        je     loc_1_188D
        xor    ebx, ebx
loc_1_1872:
        mov    esi, eax
        imul   esi, esi, 0x14
        cmp    dword ptr [esi], edi          ; -> loc_1_04C8
        jne    loc_1_1885
        mov    dword ptr [esi + 4], ebx      ; -> loc_1_04CC
loc_1_1885:
        inc    eax
        and    eax, 0x3f
        cmp    eax, edx
        jne    loc_1_1872
loc_1_188D:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 0x10]
        imul   edx, edx, 0x14
        mov    esi, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], edi          ; -> loc_1_04C8
        mov    dword ptr [edx + 4], eax      ; -> loc_1_04CC
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [edx + 8], ebx      ; -> loc_1_04D0
        mov    dword ptr [edx + 0xc], esi    ; -> loc_1_04D4
        mov    dword ptr [edx + 0x10], eax   ; -> loc_1_04D8
        popfd  
        cmp    dword ptr [0], 0              ; -> loc_1_04B8
        mov    dword ptr [0], ecx            ; -> loc_1_04C0
        jne    loc_1_18E0
        push   0
        push   0                             ; -> loc_1_17C2
        push   5
        call   sub_1_116C
        add    esp, 0xc
        mov    dword ptr [0], eax            ; -> loc_1_04B8
loc_1_18E0:
        xor    eax, eax
loc_1_18E2:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
sub_1_18E9:
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        cmp    dword ptr [esp + 0x18], edi
        je     loc_1_1939
        mov    esi, dword ptr [esp + 0x14]
        xor    ebx, ebx
        lea    ebp, [esi + 0x447]
loc_1_1901:
        push   ebx
        push   edi
        push   esi
        inc    edi
        call   sub_1_488D
        mov    byte ptr [ebp], bl
        add    ebp, 0x20
        mov    byte ptr [ebp - 0x1e], 0x40
        mov    byte ptr [ebp - 0x21], bl
        mov    byte ptr [ebp - 0x1d], 0x7f
        mov    byte ptr [ebp - 0x1c], 0x64
        mov    byte ptr [ebp - 0x1b], bl
        mov    byte ptr [ebp - 0x1a], bl
        mov    byte ptr [ebp - 0x18], 2
        mov    dword ptr [ebp - 0x15], ebx
        cmp    edi, dword ptr [esp + 0x18]
        mov    word ptr [ebp - 0xd], bx
        mov    byte ptr [ebp - 0xe], bl
        jb     loc_1_1901
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
        mov    eax, dword ptr [ebp + 0xc]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        mov    ecx, eax
        out    dx, al
        and    ch, 0xfe
        and    eax, 0x100
        shr    ecx, 8
        movzx  edx, word ptr [ecx + esi]
        shr    eax, 7
        or     edx, eax
        mov    al, byte ptr [ebp + 0x10]
        out    dx, al
        popfd  
        pop    esi
        pop    ebp
        ret    0xc
sub_1_1976:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd 
        cli    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        mov    ecx, eax
        out    dx, al
        and    ch, 0xfe
        shr    ecx, 8
        movzx  edx, word ptr [ecx + esi]
        mov    ecx, eax
        and    ecx, 0x100
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
        mov    eax, dword ptr [ebp + 0xc]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        mov    ecx, eax
        out    dx, al
        and    ch, 0xfe
        and    eax, 0x100
        shr    ecx, 8
        movzx  edx, word ptr [ecx + esi]
        shr    eax, 7
        or     edx, eax
        mov    eax, dword ptr [ebp + 0x10]
        out    dx, ax
        popfd  
        pop    esi
        pop    ebp
        ret    0xc
sub_1_19DF:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd 
        cli    
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        movzx  edx, word ptr [esi + 4]
        add    edx, 2
        mov    ecx, eax
        out    dx, al
        and    ch, 0xfe
        shr    ecx, 8
        movzx  edx, word ptr [ecx + esi]
        mov    ecx, eax
        and    ecx, 0x100
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
        mov    al, byte ptr [ebp + 0xc]
        movzx  edx, word ptr [ecx + 4]
        add    edx, 2
        out    dx, al
        mov    eax, dword ptr [ebp + 0xc]
        and    ah, 0xfe
        shr    eax, 8
        mov    dx, word ptr [eax + ecx]
        mov    eax, dword ptr [ebp + 0x10]
        out    dx, eax
        popfd  
        pop    ebp
        ret    0xc
sub_1_1A3D:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd 
        cli    
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        movzx  edx, word ptr [ecx + 4]
        add    edx, 2
        mov    esi, dword ptr [ebp + 0xc]
        out    dx, al
        and    esi, 0xfffffeff
        shr    esi, 8
        mov    dx, word ptr [esi + ecx]
        in     eax, dx
        popfd  
        pop    esi
        pop    ebp
        ret    8
sub_1_1A68:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        sub    esp, 8
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_1_1A8D
        cmp    eax, 1
        je     loc_1_1A9D
        cmp    eax, 2
        je     loc_1_1AAD
        mov    eax, 0xff
        jmp    loc_1_1B93
loc_1_1A8D:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_1B9A
        jmp    loc_1_1ABB
loc_1_1A9D:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_1CF3
        jmp    loc_1_1ABB
loc_1_1AAD:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_1E89
loc_1_1ABB:
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0xff
        je     loc_1_1B90
        mov    esi, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, esi
        shl    eax, 5
        xor    ebx, ebx
        add    edi, eax
        mov    eax, dword ptr [ebp + 0xc]
        shl    ax, 8
        push   0xffff
        or     ax, word ptr [ebp + 0x10]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [edi + 0x42], ax
        mov    eax, esi
        or     al, 0x60
        mov    dword ptr [edi + 0x4c], ebx
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        cmp    dword ptr [ebp + 0x14], 1
        jne    loc_1_1B34
        mov    dx, word ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 4]
        inc    esi
        push   0xffff
        mov    eax, esi
        shl    eax, 5
        add    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x42], dx
        mov    dword ptr [eax + 0x4c], ebx
        mov    eax, esi
        or     al, 0x60
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
loc_1_1B34:
        mov    eax, esi
        mov    ebx, dword ptr [ebp + 8]
        shl    eax, 5
        add    ebx, eax
        test   byte ptr [ebx + 0x5c], 2
        je     loc_1_1B80
        test   byte ptr [ebx + 0x7c], 4
        je     loc_1_1B80
        inc    esi
        push   0xff
        mov    eax, esi
        or     esi, 0x2a0
        or     eax, 0x420
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        push   0x807f
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    word ptr [ebx + 0x62], 0xffe
        mov    word ptr [ebx + 0x7c], 0
loc_1_1B80:
        xor    eax, eax
        cmp    dword ptr [ebp + 0x14], 1
        mov    word ptr [edi + 0x5c], ax
        jne    loc_1_1B90
        mov    word ptr [edi + 0x7c], ax
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
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x18], 2
        ja     loc_1_1BBD
        mov    eax, dword ptr [esi + 0xc]
        cmp    dword ptr [esi + 8], eax
        je     loc_1_1BBD
        mov    dword ptr [ebp - 0x1c], 0
        jmp    loc_1_1BC4
loc_1_1BBD:
        mov    dword ptr [ebp - 0x1c], 1
loc_1_1BC4:
        mov    dword ptr [ebp - 0x18], 0xffffffff
        mov    dword ptr [ebp - 8], 0xff
        mov    dword ptr [ebp - 0xc], 0
        lea    eax, [esi + 0x4c]
        mov    dword ptr [ebp - 0x14], eax
loc_1_1BDF:
        movzx  ecx, word ptr [esi + 6]
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        cmp    ecx, eax
        jbe    loc_1_1CAB
        mov    edi, dword ptr [ebp - 0x14]
loc_1_1BF4:
        cmp    dword ptr [ebp - 0x1c], 0
        jne    loc_1_1C04
        cmp    byte ptr [edi + 0xd], 0
        jne    loc_1_1C97
loc_1_1C04:
        movzx  eax, word ptr [edi - 0xa]
        mov    dword ptr [ebp - 0x10], eax
        cmp    eax, 0x2000
        jae    loc_1_1C7C
        mov    eax, dword ptr [ebp - 4]
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A3D
        shr    eax, 0x10
        cmp    dword ptr [ebp - 0x10], 0xffe
        mov    ebx, eax
        je     loc_1_1C6A
        test   byte ptr [ebp - 0xf], 0x10
        je     loc_1_1C3A
        add    ebx, 0x1200
        jmp    loc_1_1C40
loc_1_1C3A:
        add    ebx, 0x200
loc_1_1C40:
        cmp    byte ptr [ebp - 0x10], 0xff
        jne    loc_1_1C4E
        add    ebx, 0x300
        jmp    loc_1_1C6E
loc_1_1C4E:
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19DF
        test   ah, 0x80
        jne    loc_1_1C6E
        add    ebx, 0x1300
        jmp    loc_1_1C6E
loc_1_1C6A:
        test   ebx, ebx
        je     loc_1_1CE3
loc_1_1C6E:
        cmp    ebx, dword ptr [ebp - 0x18]
        ja     loc_1_1C7C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x18], ebx
        mov    dword ptr [ebp - 8], eax
loc_1_1C7C:
        cmp    dword ptr [edi], 0
        je     loc_1_1C97
        mov    eax, dword ptr [ebp - 4]
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A3D
        and    eax, 0xffffff
        cmp    dword ptr [edi], eax
        jbe    loc_1_1CEB
loc_1_1C97:
        add    dword ptr [ebp - 4], 3
        movzx  eax, word ptr [esi + 6]
        add    edi, 0x60
        cmp    eax, dword ptr [ebp - 4]
        ja     loc_1_1BF4
loc_1_1CAB:
        add    dword ptr [ebp - 0x14], 0x20
        inc    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], 3
        jb     loc_1_1BDF
        cmp    dword ptr [ebp - 8], 0xff
        je     loc_1_1CD9
loc_1_1CC5:
        push   0xff
        mov    eax, dword ptr [ebp - 8]
        or     eax, 0x2a0
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
        jmp    loc_1_1CC5
loc_1_1CEB:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_1CC5
sub_1_1CF3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x18], 2
        ja     loc_1_1D16
        mov    eax, dword ptr [esi + 0xc]
        cmp    dword ptr [esi + 8], eax
        je     loc_1_1D16
        mov    dword ptr [ebp - 0x18], 0
        jmp    loc_1_1D1D
loc_1_1D16:
        mov    dword ptr [ebp - 0x18], 1
loc_1_1D1D:
        mov    dword ptr [ebp - 8], 0xff
        mov    dword ptr [ebp - 0x14], 0xfffff
        movzx  edi, word ptr [esi + 6]
        dec    edi
        mov    eax, edi
        shl    eax, 5
        mov    dword ptr [ebp - 0xc], 0xfffff
        lea    ebx, [eax + esi + 0x4c]
loc_1_1D40:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_1D65
        cmp    byte ptr [ebx + 0xd], 0
        jne    loc_1_1E2E
        movzx  eax, word ptr [esi + 6]
        lea    ecx, [edi + 1]
        cmp    eax, ecx
        jbe    loc_1_1D65
        cmp    byte ptr [ebx + 0x2d], 0
        jne    loc_1_1E2E
loc_1_1D65:
        movzx  eax, word ptr [ebx - 0xa]
        mov    dword ptr [ebp - 0x10], eax
        cmp    eax, 0x2000
        jae    loc_1_1E2E
        mov    eax, edi
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A3D
        shr    eax, 0x10
        cmp    dword ptr [ebp - 0x10], 0xffe
        mov    dword ptr [ebp - 4], eax
        je     loc_1_1DD0
        test   byte ptr [ebp - 0xf], 0x10
        je     loc_1_1DA0
        add    dword ptr [ebp - 4], 0x1200
        jmp    loc_1_1DA7
loc_1_1DA0:
        add    dword ptr [ebp - 4], 0x200
loc_1_1DA7:
        cmp    byte ptr [ebp - 0x10], 0xff
        jne    loc_1_1DB6
        add    dword ptr [ebp - 4], 0x300
        jmp    loc_1_1DD0
loc_1_1DB6:
        mov    eax, edi
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19DF
        test   ah, 0x80
        jne    loc_1_1DD0
        add    dword ptr [ebp - 4], 0x1300
loc_1_1DD0:
        cmp    dword ptr [ebx], 0
        je     loc_1_1DF1
        mov    eax, edi
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A3D
        and    eax, 0xffffff
        cmp    dword ptr [ebx], eax
        ja     loc_1_1DF1
        mov    dword ptr [ebp - 4], 0
loc_1_1DF1:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, dword ptr [ebp - 4]
        je     loc_1_1E84
        test   byte ptr [ebx + 0x10], 1
        je     loc_1_1E08
        add    eax, 0xc00
loc_1_1E08:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0xc], ecx
        jae    loc_1_1E15
        lea    eax, [eax + ecx*2]
        jmp    loc_1_1E1B
loc_1_1E15:
        mov    edx, dword ptr [ebp - 0xc]
        lea    eax, [eax + edx*2]
loc_1_1E1B:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0xc], ecx
        jb     loc_1_1E35
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 8], edi
        jmp    loc_1_1E35
loc_1_1E2E:
        mov    dword ptr [ebp - 0xc], 0xfffff
loc_1_1E35:
        mov    eax, edi
        sub    ebx, 0x20
        dec    edi
        test   eax, eax
        jne    loc_1_1D40
        cmp    dword ptr [ebp - 8], 0xff
        je     loc_1_1E7A
        cmp    dword ptr [ebp - 0x14], 0xfffff
        ja     loc_1_1E7A
loc_1_1E55:
        mov    ebx, 0xff
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    edi, 0x2a0
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
        jmp    loc_1_1E55
sub_1_1E89:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x18], 2
        ja     loc_1_1EAC
        mov    eax, dword ptr [esi + 0xc]
        cmp    dword ptr [esi + 8], eax
        je     loc_1_1EAC
        mov    dword ptr [ebp - 0x18], 0
        jmp    loc_1_1EB3
loc_1_1EAC:
        mov    dword ptr [ebp - 0x18], 1
loc_1_1EB3:
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x10]
        shr    eax, 8
        and    edi, 0xff
        mov    dword ptr [ebp - 0xc], eax
        movzx  eax, word ptr [esi + 6]
        cmp    eax, dword ptr [ebp - 0xc]
        jae    loc_1_1ED1
        mov    dword ptr [ebp - 0xc], eax
loc_1_1ED1:
        mov    dword ptr [ebp - 0x14], 0xffffffff
        mov    dword ptr [ebp - 8], 0xff
        cmp    dword ptr [ebp - 0xc], edi
        jbe    loc_1_1FA5
        mov    eax, edi
        shl    eax, 5
        lea    ebx, [eax + esi + 0x4c]
loc_1_1EF1:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_1F01
        cmp    byte ptr [ebx + 0xd], 0
        jne    loc_1_1F98
loc_1_1F01:
        movzx  eax, word ptr [ebx - 0xa]
        mov    dword ptr [ebp - 0x10], eax
        cmp    eax, 0x2000
        jae    loc_1_1F7E
        mov    eax, edi
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A3D
        shr    eax, 0x10
        cmp    dword ptr [ebp - 0x10], 0xffe
        mov    dword ptr [ebp - 4], eax
        je     loc_1_1F6A
        test   byte ptr [ebp - 0xf], 0x10
        je     loc_1_1F38
        add    dword ptr [ebp - 4], 0x1200
        jmp    loc_1_1F3F
loc_1_1F38:
        add    dword ptr [ebp - 4], 0x200
loc_1_1F3F:
        cmp    byte ptr [ebp - 0x10], 0xff
        jne    loc_1_1F4E
        add    dword ptr [ebp - 4], 0x300
        jmp    loc_1_1F70
loc_1_1F4E:
        mov    eax, edi
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19DF
        test   ah, 0x80
        jne    loc_1_1F70
        add    dword ptr [ebp - 4], 0x1300
        jmp    loc_1_1F70
loc_1_1F6A:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1FCC
loc_1_1F70:
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 0x14]
        ja     loc_1_1F7E
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 8], edi
loc_1_1F7E:
        cmp    dword ptr [ebx], 0
        je     loc_1_1F98
        mov    eax, edi
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A3D
        and    eax, 0xffffff
        cmp    dword ptr [ebx], eax
        jbe    loc_1_1FD1
loc_1_1F98:
        add    ebx, 0x20
        inc    edi
        cmp    dword ptr [ebp - 0xc], edi
        ja     loc_1_1EF1
loc_1_1FA5:
        cmp    dword ptr [ebp - 8], 0xff
        je     loc_1_1FC2
loc_1_1FAE:
        push   0xff
        mov    eax, dword ptr [ebp - 8]
        or     eax, 0x2a0
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
        jmp    loc_1_1FAE
loc_1_1FD1:
        mov    dword ptr [ebp - 8], edi
        jmp    loc_1_1FAE
sub_1_1FD6:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x23c
        cmp    ecx, 0xffff
        push   ebx
        push   esi
        push   edi
        jne    loc_1_1FF5
        xor    eax, eax
        jmp    loc_1_2350
loc_1_1FF5:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + ecx*4 + 0x662]
        test   edx, edx
        jne    loc_1_200A
        xor    eax, eax
        jmp    loc_1_2350
loc_1_200A:
        mov    eax, dword ptr [ebp + 0x10]
        imul   eax, eax, 0x1a
        add    eax, dword ptr [edx]
        cmp    word ptr [eax], 0
        mov    dword ptr [ebp - 0x1c], eax
        jne    loc_1_2022
        xor    eax, eax
        jmp    loc_1_2350
loc_1_2022:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebp - 0xc], eax
        mov    bx, word ptr [esi + 4]
        mov    dword ptr [ebp - 0x14], ecx
        add    dword ptr [ebp - 0x1c], 0x1a
        mov    word ptr [ebp - 2], bx
        mov    esi, dword ptr [ebp - 0x1c]
        movzx  ecx, bx
        shl    ecx, 2
        add    ecx, dword ptr [edx + 4]
        cmp    word ptr [esi + 4], bx
        mov    dword ptr [ebp - 0x20], ecx
        jbe    loc_1_2350
        mov    ebx, dword ptr [ebp + 0x1c]
loc_1_2058:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [ebp - 0x20]
        mov    di, word ptr [esi]
        mov    dword ptr [ebp - 0x3c], ecx
        add    dword ptr [ebp - 0x20], 4
        mov    word ptr [ebp - 4], di
        mov    esi, dword ptr [ebp - 0x20]
        mov    dword ptr [ebp - 0x38], 0
        movzx  ecx, di
        imul   ecx, ecx, 3
        add    ecx, dword ptr [edx + 8]
        cmp    word ptr [esi], di
        mov    dword ptr [ebp - 0x2c], ecx
        jbe    loc_1_233B
loc_1_208B:
        mov    ecx, dword ptr [ebp - 0x2c]
        mov    di, word ptr [ecx + 1]
        movzx  si, byte ptr [ecx]
        cmp    si, 0x2b
        jne    loc_1_20C5
        xor    ecx, ecx
        mov    cx, di
        and    ecx, 0xff
        cmp    ecx, dword ptr [ebp - 0x14]
        ja     loc_1_233B
        sar    di, 8
        movsx  ecx, di
        cmp    ecx, dword ptr [ebp - 0x14]
        jae    loc_1_2323
        jmp    loc_1_233B
loc_1_20C5:
        cmp    si, 0x2e
        jne    loc_1_21C1
        movsx  ecx, di
        mov    dword ptr [ebp - 0x14], ecx
loc_1_20D5:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    word ptr [ebp + ecx*4 - 0x23c], si
        inc    dword ptr [ebp - 0xc]
        mov    word ptr [ebp + ecx*4 - 0x23a], di
        cmp    dword ptr [ebp - 0xc], 0x80
        je     loc_1_2350
loc_1_20F8:
        cmp    dword ptr [ebp - 0x38], 0
        jne    loc_1_233B
        cmp    si, 0x29
        jne    loc_1_2323
        movsx  ecx, di
        add    ecx, ecx
        add    ecx, dword ptr [edx + 0xc]
        mov    di, word ptr [ecx]
        mov    dword ptr [ebp - 0x24], ecx
        add    dword ptr [ebp - 0x24], 2
        movzx  ecx, di
        mov    word ptr [ebp - 6], di
        mov    esi, dword ptr [ebp - 0x24]
        shl    ecx, 2
        add    ecx, dword ptr [edx + 0x10]
        cmp    word ptr [esi], di
        mov    dword ptr [ebp - 0x28], ecx
        jbe    loc_1_231D
loc_1_213A:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [ebp - 0x28]
        mov    di, word ptr [esi]
        mov    dword ptr [ebp - 0x34], ecx
        add    dword ptr [ebp - 0x28], 4
        mov    word ptr [ebp - 0xe], di
        mov    esi, dword ptr [ebp - 0x28]
        movzx  ecx, di
        mov    di, word ptr [esi]
        imul   ecx, ecx, 3
        add    ecx, dword ptr [edx + 0x14]
        cmp    word ptr [ebp - 0xe], di
        mov    dword ptr [ebp - 0x30], ecx
        jae    loc_1_2309
        mov    ecx, dword ptr [ebp - 0xc]
        lea    esi, [ebp + ecx*4 - 0x23c]
        mov    dword ptr [ebp - 0x18], esi
loc_1_2177:
        mov    ecx, dword ptr [ebp - 0x30]
        mov    di, word ptr [ecx + 1]
        movzx  si, byte ptr [ecx]
        cmp    si, 0x29
        je     loc_1_20F8
        cmp    si, 0x35
        je     loc_1_2265
        cmp    si, 0x2b
        jne    loc_1_21F4
        xor    ecx, ecx
        mov    cx, di
        and    ecx, 0xff
        cmp    ecx, dword ptr [ebp - 0x14]
        ja     loc_1_2309
        sar    di, 8
        movsx  ecx, di
        cmp    ecx, dword ptr [ebp - 0x14]
        jae    loc_1_221D
        jmp    loc_1_2309
loc_1_21C1:
        cmp    si, 0x2c
        jne    loc_1_20D5
        xor    ecx, ecx
        mov    cx, di
        and    ecx, 0xff
        cmp    ecx, dword ptr [ebp + 0x18]
        ja     loc_1_233B
        sar    di, 8
        movsx  ecx, di
        cmp    ecx, dword ptr [ebp + 0x18]
        jae    loc_1_2323
        jmp    loc_1_233B
loc_1_21F4:
        cmp    si, 0x2e
        jne    loc_1_223A
        movsx  ecx, di
        mov    dword ptr [ebp - 0x14], ecx
loc_1_2200:
        mov    ecx, dword ptr [ebp - 0x18]
        inc    dword ptr [ebp - 0xc]
        mov    word ptr [ecx], si
        lea    esi, [ebp - 0x3c]
        add    dword ptr [ebp - 0x18], 4
        mov    word ptr [ecx + 2], di
        cmp    dword ptr [ebp - 0x18], esi
        je     loc_1_2350
loc_1_221D:
        inc    word ptr [ebp - 0xe]
        mov    ecx, dword ptr [ebp - 0x28]
        add    dword ptr [ebp - 0x30], 3
        mov    cx, word ptr [ecx]
        cmp    word ptr [ebp - 0xe], cx
        jb     loc_1_2177
        jmp    loc_1_2309
loc_1_223A:
        cmp    si, 0x2c
        jne    loc_1_2200
        xor    ecx, ecx
        mov    cx, di
        and    ecx, 0xff
        cmp    ecx, dword ptr [ebp + 0x18]
        ja     loc_1_2309
        sar    di, 8
        movsx  ecx, di
        cmp    ecx, dword ptr [ebp + 0x18]
        jae    loc_1_221D
        jmp    loc_1_2309
loc_1_2265:
        mov    dword ptr [ebp - 0x38], 1
        movsx  ecx, di
        imul   ecx, ecx, 0x24
        add    ecx, dword ptr [edx + 0x18]
        mov    dword ptr [ebp - 0x10], ecx
        test   byte ptr [ecx + 3], 0x80
        jne    loc_1_2303
        mov    esi, 0                        ; -> loc_1_09C8
        mov    edi, ebx
        mov    ecx, 0x25
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cx, word ptr [ebp + 0x14]
        mov    si, word ptr [ebp + 0x18]
        cmp    dword ptr [ebp - 0xc], 0
        mov    word ptr [ebx + 0x5c], cx
        mov    word ptr [ebx + 0x5e], si
        je     loc_1_22CB
        lea    esi, [ebp - 0x23c]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0xc], ecx
loc_1_22B2:
        cmp    word ptr [esi], 0x3a
        jg     loc_1_22C3
        mov    cx, word ptr [esi + 2]
        movsx  edi, word ptr [esi]
        mov    word ptr [ebx + edi*2], cx
loc_1_22C3:
        add    esi, 4
        dec    dword ptr [ebp - 0xc]
        jne    loc_1_22B2
loc_1_22CB:
        mov    esi, dword ptr [ebp - 0x10]
        movsx  ecx, word ptr [ebx]
        add    ecx, dword ptr [esi]
        mov    dword ptr [ebx + 0x76], ecx
        movsx  ecx, word ptr [ebx + 2]
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebx + 0x7a], ecx
        inc    eax
        movsx  ecx, word ptr [ebx + 4]
        add    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebx + 0x7e], ecx
        movsx  ecx, word ptr [ebx + 6]
        add    ecx, dword ptr [esi + 0xc]
        cmp    eax, 6
        mov    dword ptr [ebx + 0x82], ecx
        je     loc_1_2350
        add    ebx, 0x94
loc_1_2303:
        mov    ecx, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0xc], ecx
loc_1_2309:
        inc    word ptr [ebp - 6]
        mov    esi, dword ptr [ebp - 0x24]
        mov    cx, word ptr [ebp - 6]
        cmp    word ptr [esi], cx
        ja     loc_1_213A
loc_1_231D:
        mov    ecx, dword ptr [ebp - 0x3c]
        mov    dword ptr [ebp - 0xc], ecx
loc_1_2323:
        inc    word ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 0x20]
        mov    cx, word ptr [ebp - 4]
        add    dword ptr [ebp - 0x2c], 3
        cmp    word ptr [esi], cx
        ja     loc_1_208B
loc_1_233B:
        inc    word ptr [ebp - 2]
        mov    esi, dword ptr [ebp - 0x1c]
        mov    cx, word ptr [ebp - 2]
        cmp    word ptr [esi + 4], cx
        ja     loc_1_2058
loc_1_2350:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
sub_1_2359:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x2d8
        cmp    ecx, 0xffff
        push   ebx
        push   esi
        push   edi
        jne    loc_1_2378
        xor    eax, eax
        jmp    loc_1_27B3
loc_1_2378:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx*4 + 0x662]
        test   eax, eax
        jne    loc_1_238D
        xor    eax, eax
        jmp    loc_1_27B3
loc_1_238D:
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 0x14]
        imul   eax, eax, 0x1a
        add    eax, dword ptr [ecx]
        cmp    word ptr [eax], 0
        mov    dword ptr [ebp - 0x1c], eax
        jne    loc_1_23AB
        xor    eax, eax
        jmp    loc_1_27B3
loc_1_23AB:
        mov    dword ptr [ebp - 0x2c], 0
        mov    dword ptr [ebp - 8], 0
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 0x14]
        add    dword ptr [ebp - 0x1c], 0x1a
        mov    word ptr [ebp - 2], ax
        movzx  eax, ax
        shl    eax, 2
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    dx, word ptr [ebp - 2]
        cmp    word ptr [ecx + 4], dx
        mov    dword ptr [ebp - 0x28], eax
        jbe    loc_1_27B0
        mov    edx, dword ptr [ebp + 0x1c]
loc_1_23EB:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x28]
        mov    si, word ptr [eax]
        mov    dword ptr [ebp - 0x38], ecx
        add    dword ptr [ebp - 0x28], 4
        movzx  eax, si
        mov    ecx, dword ptr [ebp - 0x14]
        imul   eax, eax, 3
        add    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0x28]
        cmp    word ptr [eax], si
        jbe    loc_1_279B
        mov    ecx, dword ptr [ebp - 8]
        lea    ecx, [ebp + ecx*4 - 0x2d8]
loc_1_2420:
        mov    eax, dword ptr [ebp - 0xc]
        mov    di, word ptr [eax + 1]
        movzx  bx, byte ptr [eax]
        cmp    bx, 0x2b
        jne    loc_1_2455
        xor    eax, eax
        mov    ax, di
        and    eax, 0xff
        cmp    eax, dword ptr [ebp + 0x14]
        ja     loc_1_279B
        sar    di, 8
        movsx  eax, di
        cmp    eax, dword ptr [ebp + 0x14]
        jae    loc_1_24A0
        jmp    loc_1_279B
loc_1_2455:
        cmp    bx, 0x2c
        jne    loc_1_247F
        xor    eax, eax
        mov    ax, di
        and    eax, 0xff
        cmp    eax, dword ptr [ebp + 0x18]
        ja     loc_1_279B
        sar    di, 8
        movsx  eax, di
        cmp    eax, dword ptr [ebp + 0x18]
        jae    loc_1_24A0
        jmp    loc_1_279B
loc_1_247F:
        mov    word ptr [ecx], bx
        lea    eax, [ebp - 0xd8]
        mov    word ptr [ecx + 2], di
        add    ecx, 4
        inc    dword ptr [ebp - 8]
        cmp    ecx, eax
        je     loc_1_27B0
        cmp    bx, 0x29
        je     loc_1_24B7
loc_1_24A0:
        inc    si
        mov    eax, dword ptr [ebp - 0x28]
        add    dword ptr [ebp - 0xc], 3
        cmp    word ptr [eax], si
        ja     loc_1_2420
        jmp    loc_1_279B
loc_1_24B7:
        movsx  eax, di
        mov    ecx, dword ptr [ebp - 8]
        add    eax, eax
        mov    dword ptr [ebp - 0x30], ecx
        mov    ecx, dword ptr [ebp - 0x14]
        add    eax, dword ptr [ecx + 0xc]
        mov    si, word ptr [eax]
        mov    dword ptr [ebp - 0x20], eax
        add    dword ptr [ebp - 0x20], 2
        movzx  eax, si
        mov    word ptr [ebp - 0x16], si
        mov    ebx, dword ptr [ebp - 0x20]
        shl    eax, 2
        add    eax, dword ptr [ecx + 0x10]
        cmp    word ptr [ebx], si
        mov    dword ptr [ebp - 0x24], eax
        jbe    loc_1_2795
        xor    eax, eax
loc_1_24F0:
        mov    ecx, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp - 0x24]
        add    dword ptr [ebp - 0x24], 4
        mov    si, word ptr [ebx]
        mov    dword ptr [ebp - 0x34], ecx
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ebx, dword ptr [ebp - 0x24]
        movzx  edi, si
        imul   edi, edi, 3
        add    edi, dword ptr [ecx + 0x14]
        cmp    word ptr [ebx], si
        mov    dword ptr [ebp - 0x10], edi
        jbe    loc_1_2781
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [ebp + ecx*4 - 0x2d8]
loc_1_2525:
        mov    ecx, dword ptr [ebp - 0x10]
        movzx  bx, byte ptr [ecx]
        mov    cx, word ptr [ecx + 1]
        mov    word ptr [ebp - 0xa], cx
        cmp    bx, 0x35
        je     loc_1_25D4
        cmp    bx, 0x2b
        jne    loc_1_256E
        xor    ecx, ecx
        mov    cx, word ptr [ebp - 0xa]
        and    ecx, 0xff
        cmp    ecx, dword ptr [ebp + 0x14]
        ja     loc_1_2781
        mov    cx, word ptr [ebp - 0xa]
        sar    cx, 8
        movsx  ebx, cx
        cmp    ebx, dword ptr [ebp + 0x14]
        jae    loc_1_25BD
        jmp    loc_1_2781
loc_1_256E:
        cmp    bx, 0x2c
        jne    loc_1_259E
        xor    ecx, ecx
        mov    cx, word ptr [ebp - 0xa]
        and    ecx, 0xff
        cmp    ecx, dword ptr [ebp + 0x18]
        ja     loc_1_2781
        mov    cx, word ptr [ebp - 0xa]
        sar    cx, 8
        movsx  ebx, cx
        cmp    ebx, dword ptr [ebp + 0x18]
        jae    loc_1_25BD
        jmp    loc_1_2781
loc_1_259E:
        mov    word ptr [edi], bx
        add    edi, 4
        mov    cx, word ptr [ebp - 0xa]
        inc    dword ptr [ebp - 8]
        mov    word ptr [edi - 2], cx
        lea    ebx, [ebp - 0xd8]
        cmp    edi, ebx
        je     loc_1_27B0
loc_1_25BD:
        inc    si
        mov    ecx, dword ptr [ebp - 0x24]
        add    dword ptr [ebp - 0x10], 3
        cmp    word ptr [ecx], si
        ja     loc_1_2525
        jmp    loc_1_2781
loc_1_25D4:
        movsx  ebx, word ptr [ebp - 0xa]
        mov    ecx, dword ptr [ebp - 0x14]
        imul   ebx, ebx, 0x24
        add    ebx, dword ptr [ecx + 0x18]
        test   byte ptr [ebx + 3], 0x80
        jne    loc_1_277B
        mov    esi, 0                        ; -> loc_1_0A60
        mov    edi, edx
        mov    ecx, 0x25
        rep movsd dword ptr es:[edi], dword ptr [esi]
        lea    edi, [ebp - 0xd8]
        mov    ecx, 0x25
        rep stosd dword ptr es:[edi], eax
        mov    cx, word ptr [ebp + 0x14]
        mov    dword ptr [ebp - 0x10], eax
        mov    word ptr [edx + 0x5c], cx
        mov    si, word ptr [ebp + 0x18]
        cmp    dword ptr [ebp - 0x30], eax
        mov    word ptr [edx + 0x5e], si
        je     loc_1_264A
        lea    esi, [ebp - 0x2d8]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ebp - 0x3c], ecx
        mov    dword ptr [ebp - 0x10], ecx
loc_1_262D:
        cmp    word ptr [esi], 0x3a
        jg     loc_1_2642
        mov    cx, word ptr [esi + 2]
        movsx  edi, word ptr [esi]
        mov    word ptr [ebp + edi*2 - 0xd8], cx
loc_1_2642:
        add    esi, 4
        dec    dword ptr [ebp - 0x3c]
        jne    loc_1_262D
loc_1_264A:
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 8], ecx
        jbe    loc_1_267E
        lea    esi, [ebp + ecx*4 - 0x2d8]
        mov    ecx, dword ptr [ebp - 8]
        sub    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 8], ecx
loc_1_2662:
        mov    cx, word ptr [esi]
        cmp    cx, 0x3a
        jg     loc_1_2676
        mov    di, word ptr [esi + 2]
        movsx  ecx, cx
        mov    word ptr [edx + ecx*2], di
loc_1_2676:
        add    esi, 4
        dec    dword ptr [ebp - 8]
        jne    loc_1_2662
loc_1_267E:
        mov    cx, word ptr [ebp - 0xa]
        mov    esi, edx
        mov    word ptr [ebp - 0x7c], ax
        mov    word ptr [ebp - 0x7a], ax
        mov    word ptr [ebp - 0x6e], cx
        lea    ecx, [ebp - 0xd8]
        mov    dword ptr [ebp - 0xc], ecx
loc_1_2699:
        movsx  ecx, word ptr [esi]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp - 0xc]
        movsx  edi, word ptr [ecx]
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, edi
        cmp    ecx, 0xffff8000
        jge    loc_1_26B9
        mov    word ptr [esi], 0x8000
        jmp    loc_1_26C9
loc_1_26B9:
        mov    word ptr [esi], 0x7fff
        cmp    ecx, 0x7fff
        jg     loc_1_26C9
        mov    word ptr [esi], cx
loc_1_26C9:
        add    esi, 2
        lea    ecx, [ebp - 0x64]
        add    dword ptr [ebp - 0xc], 2
        cmp    dword ptr [ebp - 0xc], ecx
        jbe    loc_1_2699
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [edx + 0x90], ecx
        movsx  ecx, word ptr [edx]
        add    ecx, dword ptr [ebx]
        mov    dword ptr [edx + 0x76], ecx
        movsx  esi, word ptr [edx + 8]
        shl    esi, 0xf
        add    esi, ecx
        mov    dword ptr [edx + 0x76], esi
        movsx  ecx, word ptr [edx + 2]
        add    ecx, dword ptr [ebx + 4]
        mov    dword ptr [edx + 0x7a], ecx
        movsx  esi, word ptr [edx + 0x18]
        shl    esi, 0xf
        add    esi, ecx
        mov    dword ptr [edx + 0x7a], esi
        movsx  ecx, word ptr [edx + 4]
        add    ecx, dword ptr [ebx + 8]
        mov    dword ptr [edx + 0x7e], ecx
        movsx  esi, word ptr [edx + 0x5a]
        shl    esi, 0xf
        add    esi, ecx
        mov    dword ptr [edx + 0x7e], esi
        lea    esi, [edx + 0x82]
        movsx  ecx, word ptr [edx + 6]
        add    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [esi], ecx
        movsx  edi, word ptr [edx + 0x64]
        shl    edi, 0xf
        add    edi, ecx
        mov    dword ptr [esi], edi
        mov    ecx, dword ptr [ebx + 0x16]
        mov    dword ptr [edx + 0x86], ecx
        mov    cl, byte ptr [ebx + 0x14]
        mov    byte ptr [edx + 0x8a], cl
        mov    cl, byte ptr [ebx + 0x15]
        mov    byte ptr [edx + 0x8b], cl
        inc    dword ptr [ebp - 0x2c]
        mov    si, word ptr [ebx + 0x12]
        mov    word ptr [edx + 0x8c], si
        cmp    dword ptr [ebp - 0x2c], 6
        mov    bx, word ptr [ebx + 0x10]
        mov    word ptr [edx + 0x8e], bx
        je     loc_1_27B0
        add    edx, 0x94
loc_1_277B:
        mov    ecx, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 8], ecx
loc_1_2781:
        inc    word ptr [ebp - 0x16]
        mov    ecx, dword ptr [ebp - 0x20]
        mov    si, word ptr [ebp - 0x16]
        cmp    word ptr [ecx], si
        ja     loc_1_24F0
loc_1_2795:
        mov    ecx, dword ptr [ebp - 0x38]
        mov    dword ptr [ebp - 8], ecx
loc_1_279B:
        inc    word ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [ebp - 2]
        cmp    word ptr [ecx + 4], ax
        ja     loc_1_23EB
loc_1_27B0:
        mov    eax, dword ptr [ebp - 0x2c]
loc_1_27B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
sub_1_27BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x3c
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x1c]
        cmp    word ptr [edi + 0x72], 0
        je     loc_1_281F
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        add    esi, 0x42
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 6], bx
        jbe    loc_1_281F
loc_1_27E0:
        movzx  eax, byte ptr [esi + 0x13]
        movsx  ecx, word ptr [edi + 0x72]
        cmp    eax, ecx
        jne    loc_1_2810
        push   0x807f
        mov    eax, ebx
        or     eax, 0x2a0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    word ptr [esi], 0xffe
        mov    word ptr [esi + 0x1a], 0
        mov    byte ptr [esi + 0x13], 0
loc_1_2810:
        add    esi, 0x20
        inc    ebx
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, word ptr [ecx + 6]
        cmp    eax, ebx
        ja     loc_1_27E0
loc_1_281F:
        mov    ebx, edi
        mov    eax, dword ptr [ebp + 0xc]
        shl    eax, 5
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x18], 0
        lea    edx, [eax + ecx + 0x442]
        mov    dword ptr [ebp - 0xc], edx
        je     loc_1_2AD4
loc_1_2845:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0xae2]
        test   edx, edx
        je     loc_1_2871
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x5c
        cmp    word ptr [eax + ecx + 0xeee], 0
        je     loc_1_2871
        push   ebx
        mov    eax, dword ptr [ebp + 0xc]
        or     ah, 0x80
        push   eax
        push   ecx
        call   edx
        add    esp, 0xc
loc_1_2871:
        push   0
        push   0xfe
        push   0xff
        push   dword ptr [ebp + 8]
        call   sub_1_1A68
        add    esp, 0x10
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    eax, 0xff
        mov    byte ptr [ebp + ecx - 0x3c], al
        je     loc_1_2ABF
        shl    eax, 5
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 0x76], 0x200000
        lea    esi, [eax + ecx + 0x42]
        jb     loc_1_28BC
        cmp    byte ptr [esi + 0x17], 0
        je     loc_1_28CC
        dec    dword ptr [ecx + 0x18]
        mov    byte ptr [esi + 0x17], 0
        jmp    loc_1_28CC
loc_1_28BC:
        cmp    byte ptr [esi + 0x17], 0
        jne    loc_1_28CC
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [ecx + 0x18]
        mov    byte ptr [esi + 0x17], 1
loc_1_28CC:
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 9]
        cmp    al, 0xa
        jbe    loc_1_296E
        mov    dword ptr [ebp - 4], 0x18
        movsx  edx, word ptr [ebx + 0x5e]
        movzx  eax, al
        movzx  ecx, byte ptr [edx]           ; -> obj1:0x0AF8
        movzx  edx, byte ptr [eax]           ; -> loc_1_0B78
        movsx  eax, word ptr [ebx + 0x60]
        add    ecx, edx
        add    eax, 0xc
        cdq    
        idiv   dword ptr [ebp - 4]
        add    ecx, eax
        sub    edx, edx
        lea    eax, [ecx*8]
        mov    ecx, 3
        div    ecx
        mov    edx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [edx + 0x10]
        lea    edx, [ecx + eax]
        cmp    edx, 0xff
        ja     loc_1_296E
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 8]
        cmp    al, 0x7f
        jae    loc_1_294E
        movzx  ecx, al
        movzx  eax, byte ptr [ecx]           ; -> loc_1_0BF8
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, 0x100
        sub    eax, edx
        imul   eax, dword ptr [ebp - 0x24]
        shr    eax, 7
        add    edx, eax
loc_1_294E:
        cmp    byte ptr [esi + 0x17], 0
        je     loc_1_2973
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x151e], 0x4d474d31
        jne    loc_1_2973
        add    edx, 0x10
        cmp    edx, 0xff
        jbe    loc_1_2973
loc_1_296E:
        mov    edx, 0xff
loc_1_2973:
        mov    word ptr [ebx + 0x26], dx
        mov    eax, dword ptr [ebp - 0xc]
        cmp    word ptr [eax + 2], 0x80
        jae    loc_1_29AB
        cmp    word ptr [ebx + 0x44], 0x7d
        jge    loc_1_29AB
        movsx  eax, word ptr [ebx + 0x5e]
        cmp    eax, 0x46
        jae    loc_1_2997
        mov    eax, 0x46
loc_1_2997:
        movsx  ecx, word ptr [ebx + 0x10]
        imul   ecx, eax
        add    ecx, 0xa0
        shr    ecx, 7
        mov    word ptr [ebx + 0x10], cx
loc_1_29AB:
        mov    eax, dword ptr [ebp - 0xc]
        movzx  ecx, word ptr [ebx + 0x48]
        cmp    word ptr [eax + 2], 0x80
        jae    loc_1_29FC
        mov    dx, 0x3c
        lea    eax, [ebx + 0x46]
        sub    dx, word ptr [ebx + 0x5c]
        imul   dx, word ptr [ebx + 0x4e]
        add    dx, word ptr [eax]
        mov    word ptr [eax], dx
        jns    loc_1_29D7
        mov    word ptr [eax], 0
loc_1_29D7:
        movsx  edx, word ptr [ebx + 0x5c]
        mov    eax, 0x3c
        sub    eax, edx
        movsx  edx, word ptr [ebx + 0x50]
        imul   eax, edx
        add    ecx, eax
        jns    loc_1_29F1
        xor    ecx, ecx
        jmp    loc_1_29FC
loc_1_29F1:
        mov    eax, 0xffff
        cmp    ecx, eax
        jbe    loc_1_29FC
        mov    ecx, eax
loc_1_29FC:
        mov    ax, word ptr [ebx + 0x46]
        cmp    ax, 0x2da4
        jle    loc_1_2A0E
        mov    word ptr [ebx + 0x46], 0
        jmp    loc_1_2A22
loc_1_2A0E:
        mov    word ptr [ebp - 4], 0xffa4
        cwd    
        idiv   word ptr [ebp - 4]
        add    ax, 0x7f
        mov    word ptr [ebx + 0x46], ax
loc_1_2A22:
        push   ecx
        call   sub_4_272F
loc_1_2A28:
        mov    word ptr [ebx + 0x48], ax
        add    esp, 4
        movsx  ecx, word ptr [ebx + 0x5c]
        movsx  eax, word ptr [ebx + 0x74]
        sub    ecx, eax
        movsx  edx, word ptr [ebx + 0x66]
        add    ecx, edx
        movsx  eax, word ptr [ebx + 0x6e]
        movsx  edx, word ptr [ebx + 0x68]
        add    ecx, 0x3c
        imul   ecx, ecx, 0x64
        sub    ecx, eax
        add    ecx, edx
        cmp    word ptr [ebx + 0x70], 1
        mov    dword ptr [ebp - 0x24], ecx
        jne    loc_1_2A65
        mov    eax, ecx
        cdq    
        sub    eax, edx
        sar    eax, 1
        mov    dword ptr [ebp - 0x24], eax
loc_1_2A65:
        push   dword ptr [ebp - 0x24]
        call   sub_1_2FB3
        mov    word ptr [esi + 0xe], ax
        mov    ax, word ptr [ebx + 0x60]
        push   dword ptr [ebp - 0xc]
        push   ebx
        mov    word ptr [esi + 4], ax
        mov    cx, word ptr [ebx + 0x5e]
        mov    word ptr [esi + 2], cx
        mov    dl, byte ptr [ebx + 0xa]
        mov    byte ptr [esi + 0x14], dl
        mov    al, byte ptr [ebx + 0x3c]
        mov    byte ptr [esi + 0x15], al
        mov    cl, byte ptr [ebx + 0x4c]
        mov    byte ptr [esi + 0x16], cl
        mov    dl, byte ptr [ebx + 0x22]
        mov    byte ptr [esi + 0x12], dl
        mov    edx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 0x72]
        mov    byte ptr [esi + 0x13], al
        movzx  eax, word ptr [edx + 0x12]
        push   eax
        call   sub_1_4FCB
        add    esp, 0xc
        mov    cl, byte ptr [ebx + 0x22]
        mov    byte ptr [esi + 0x18], cl
        mov    cl, byte ptr [ebx + 0x22]
        mov    byte ptr [esi + 0x19], cl
loc_1_2ABF:
        add    ebx, 0x94
        inc    dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x18], ecx
        ja     loc_1_2845
loc_1_2AD4:
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_1_2FA8
        mov    esi, 0xffff
loc_1_2AEA:
        mov    ecx, dword ptr [ebp - 0x10]
        movzx  eax, byte ptr [ebp + ecx - 0x3c]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 0xff
        je     loc_1_2F93
        shl    eax, 5
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + ecx + 0x42]
        mov    eax, dword ptr [ebp - 0xc]
        movsx  ecx, word ptr [eax + 0xe]
        movzx  eax, word ptr [edx + 0xe]
        add    ecx, eax
        mov    dword ptr [ebp - 0x14], edx
        mov    eax, dword ptr [ebp - 0xc]
        add    ecx, dword ptr [eax + 0x10]
        cmp    ecx, esi
        jbe    loc_1_2B29
        mov    dword ptr [ebp - 8], esi
        jmp    loc_1_2B37
loc_1_2B29:
        mov    eax, 0
        test   ecx, ecx
        jl     loc_1_2B34
        mov    eax, ecx
loc_1_2B34:
        mov    dword ptr [ebp - 8], eax
loc_1_2B37:
        mov    eax, dword ptr [edi + 0x76]
        sub    eax, 4
        test   byte ptr [edi + 0x6c], 1
        mov    dword ptr [ebp - 0x28], eax
        je     loc_1_2B82
        mov    ecx, dword ptr [edi + 0x7e]
        mov    edx, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x18], ecx
        lea    ecx, [edi + 0x82]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 0xa], 0
        inc    eax
        mov    ebx, dword ptr [edi + 0x7a]
        mov    dword ptr [ebp - 0x2c], eax
        mov    eax, ebx
        sub    eax, dword ptr [ecx]
        cmp    eax, 0x14
        jae    loc_1_2B77
        mov    dword ptr [edx + 6], 0
        jmp    loc_1_2B9E
loc_1_2B77:
        add    ebx, 4
        mov    edx, dword ptr [ebp - 0x14]
        mov    dword ptr [edx + 6], ebx
        jmp    loc_1_2B9E
loc_1_2B82:
        mov    eax, dword ptr [edi + 0x7a]
        mov    edx, dword ptr [ebp - 0x14]
        add    eax, 8
        mov    dword ptr [edx + 6], 0
        lea    ecx, [eax - 4]
        mov    dword ptr [ebp - 0x2c], eax
        mov    dword ptr [ebp - 0x18], ecx
        mov    dword ptr [edx + 0xa], ecx
loc_1_2B9E:
        mov    eax, dword ptr [ebp - 4]
        push   esi
        or     al, 0x60
        push   eax
        mov    dword ptr [ebp - 0x34], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    eax, dword ptr [ebp - 4]
        push   esi
        or     al, 0x40
        push   eax
        mov    dword ptr [ebp - 0x30], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [edi + 0x46]
        movsx  ecx, word ptr [edi + 0x44]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x380
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 0x2a]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x3a0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [edi + 0x36]
        movsx  ecx, word ptr [edi + 0x34]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x3c0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 0x3a]
        movsx  eax, word ptr [edi + 0x38]
        shl    ecx, 8
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     eax, 0x2e0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 0x2e]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x3e0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        or     ah, 4
        push   eax
        lea    ebx, [edi + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [ebx]
        movsx  ecx, word ptr [edi + 0x26]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x420
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [edi + 0xe]
        movsx  ecx, word ptr [edi + 0x16]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x440
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    edx, dword ptr [ebp - 0xc]
        movzx  ecx, byte ptr [edx + 0xa]
        movzx  eax, byte ptr [edx + 0xb]
        add    ecx, eax
        movsx  eax, word ptr [edi + 0xa]
        add    eax, ecx
        cmp    eax, 0x7f
        jle    loc_1_2CBA
        mov    eax, 0x7f
loc_1_2CBA:
        movsx  ecx, word ptr [edi + 0x14]
        shl    eax, 8
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     eax, 0x460
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [edi + 0x1a]
        movsx  ecx, word ptr [edi + 0x2c]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x480
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 0xc]
        movsx  eax, word ptr [edi + 0x30]
        shl    ecx, 8
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     eax, 0x4a0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        cmp    word ptr [edi + 0x34], 0x7f
        jne    loc_1_2D6C
        cmp    word ptr [edi + 0x32], 0x8000
        jb     loc_1_2D6C
        push   0xbfff
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x2c0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [edi + 0xe]
        shl    ecx, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, ecx
        mov    ecx, esi
        cmp    eax, esi
        ja     loc_1_2D4C
        mov    ecx, eax
loc_1_2D4C:
        mov    dword ptr [ebp - 8], ecx
        movsx  eax, word ptr [ebx]
        movsx  ecx, word ptr [edi + 0x16]
        add    eax, ecx
        jns    loc_1_2D5E
        xor    eax, eax
        jmp    loc_1_2D85
loc_1_2D5E:
        cmp    eax, 0xff
        jle    loc_1_2D85
        mov    eax, 0xff
        jmp    loc_1_2D85
loc_1_2D6C:
        movsx  eax, word ptr [edi + 0x32]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x2c0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [ebx]
loc_1_2D85:
        shl    eax, 8
        cmp    dword ptr [ebp - 8], esi
        mov    dword ptr [ebp - 0x1c], eax
        jne    loc_1_2D95
        mov    dword ptr [ebp - 0x24], esi
        jmp    loc_1_2DFB
loc_1_2D95:
        mov    ebx, 1
        mov    ecx, dword ptr [ebp - 8]
        shr    ecx, 0xc
        shl    ebx, cl
        test   byte ptr [ebp - 7], 8
        je     loc_1_2DBA
        mov    eax, ebx
        mov    ecx, 0x2710
        imul   eax, eax, 0x102e
        cdq    
        idiv   ecx
        add    ebx, eax
loc_1_2DBA:
        test   byte ptr [ebp - 7], 4
        je     loc_1_2DD2
        mov    eax, ebx
        mov    ecx, 0x2710
        imul   eax, eax, 0x764
        cdq    
        idiv   ecx
        add    ebx, eax
loc_1_2DD2:
        test   byte ptr [ebp - 7], 2
        je     loc_1_2DEA
        mov    eax, ebx
        mov    ecx, 0x2710
        imul   eax, eax, 0x389
        cdq    
        idiv   ecx
        add    ebx, eax
loc_1_2DEA:
        mov    eax, ebx
        mov    dword ptr [ebp - 0x24], esi
        sar    eax, 2
        add    ebx, eax
        cmp    ebx, esi
        ja     loc_1_2DFB
        mov    dword ptr [ebp - 0x24], ebx
loc_1_2DFB:
        cmp    word ptr [edi + 0x44], 0x7f
        jne    loc_1_2E55
        cmp    word ptr [edi + 0x42], 0x8000
        jb     loc_1_2E55
        push   0xbfff
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x280
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [edi + 0x26]
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0xf
        xor    eax, edx
        sub    eax, edx
        movsx  edx, ax
        movsx  eax, word ptr [edi + 0x26]
        mov    bx, word ptr [edx*2]          ; -> loc_1_0C78
        cdq    
        and    edx, 0xf
        add    eax, edx
        sar    eax, 4
        mov    cl, al
        shr    bx, cl
        movzx  eax, bx
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_1_2E72
loc_1_2E55:
        movsx  eax, word ptr [edi + 0x42]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x280
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    dword ptr [ebp - 0x20], 0
loc_1_2E72:
        mov    eax, dword ptr [ebp - 4]
        push   0
        or     al, 0x20
        push   eax
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [edi + 0x22]
        shl    eax, 0x18
        or     dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [ebp - 0x18]
        or     al, 0xc0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [edi + 0x1e]
        shl    eax, 0x18
        or     dword ptr [ebp - 0x2c], eax
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [ebp - 0x2c]
        or     al, 0xe0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   dword ptr [ebp - 0x28]
        mov    eax, dword ptr [ebp - 4]
        or     ah, 2
        push   eax
        mov    dword ptr [ebp - 0x18], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        mov    eax, dword ptr [ebp - 4]
        or     al, 0xa0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        mov    eax, dword ptr [ebp - 4]
        or     al, 0x80
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [edi + 0x12]
        shl    eax, 0x1c
        or     dword ptr [ebp - 0x28], eax
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    ebx, dword ptr [ebp - 0x20]
        shl    ebx, 0x10
        or     ebx, dword ptr [ebp - 0x1c]
        push   ebx
        push   dword ptr [ebp - 0x34]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   ebx
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  ecx, word ptr [edi + 0x20]
        movsx  eax, word ptr [edi + 0x24]
        shl    ecx, 8
        mov    ebx, dword ptr [ebp - 0x24]
        shl    ebx, 0x10
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
        movsx  eax, word ptr [edi + 0x4a]
        movsx  ecx, word ptr [edi + 0x48]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x2a0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp - 0x14]
        shl    ax, 8
        or     ax, word ptr [ebp + 0x10]
        mov    word ptr [edx], ax
loc_1_2F93:
        add    edi, 0x94
        inc    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp + 0x18], eax
        ja     loc_1_2AEA
loc_1_2FA8:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
sub_1_2FB3:
        push   esi
        mov    ecx, 0x4b0
        mov    esi, dword ptr [esp + 8]
        push   edi
        add    esi, 0x41a0
        mov    eax, esi
        cdq    
        idiv   ecx
        cmp    eax, 0xf
        mov    edi, eax
        jle    loc_1_2FD5
        mov    edi, 0xf
loc_1_2FD5:
        mov    ecx, 0x4b0
        mov    eax, esi
        cdq    
        idiv   ecx
        mov    eax, edx
        mov    ecx, edx
        shl    edi, 0xc
        mov    esi, 0x4b
        imul   eax, eax, 0x1f
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
        sub    esp, 0x4c
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0x1c]
        cmp    word ptr [ebx + 0x72], 0
        je     loc_1_3073
        mov    edi, dword ptr [ebp + 8]
        xor    esi, esi
        add    edi, 0x42
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 6], si
        jbe    loc_1_3073
loc_1_3022:
        mov    eax, dword ptr [edi + 0x1c]
        cmp    dword ptr [ebx + 0x90], eax
        jne    loc_1_3064
        movzx  eax, byte ptr [edi + 0x13]
        movsx  ecx, word ptr [ebx + 0x72]
        cmp    eax, ecx
        jne    loc_1_3064
        push   0x807f
        mov    eax, esi
        or     eax, 0x2a0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    word ptr [edi], 0xffe
        mov    word ptr [edi + 0x1a], 0
        mov    dword ptr [edi + 0x1c], 0
        mov    byte ptr [edi + 0x13], 0
loc_1_3064:
        add    edi, 0x20
        inc    esi
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, word ptr [ecx + 6]
        cmp    eax, esi
        ja     loc_1_3022
loc_1_3073:
        mov    dword ptr [ebp - 0x18], 0
        cmp    dword ptr [ebp + 0x18], 0
        mov    ecx, ebx
        je     loc_1_3125
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [ebp - 8], eax
loc_1_308C:
        cmp    word ptr [ecx + 0x8c], 2
        jne    loc_1_30EE
        xor    eax, eax
        cmp    dword ptr [ebp + 0x18], eax
        je     loc_1_30EE
        lea    esi, [ebx + 0x6a]
loc_1_30A0:
        movsx  edi, word ptr [esi]
        movzx  edx, word ptr [ecx + 0x8e]
        cmp    edi, edx
        je     loc_1_30BC
        add    esi, 0x94
        inc    eax
        cmp    eax, dword ptr [ebp + 0x18]
        jb     loc_1_30A0
        jmp    loc_1_30EE
loc_1_30BC:
        imul   eax, eax, 0x94
        mov    esi, dword ptr [ebp - 0x18]
        lea    edx, [eax + ebx]
        mov    word ptr [ecx + 0x6a], 0xffff
        mov    word ptr [edx + 0x6a], 0xffff
        mov    dword ptr [ebp + esi*4 - 0x4c], edx
        inc    dword ptr [ebp - 0x18]
        mov    word ptr [edx + 0x8c], 4
        mov    esi, dword ptr [ebp - 0x18]
        inc    dword ptr [ebp - 0x18]
        mov    dword ptr [ebp + esi*4 - 0x4c], ecx
loc_1_30EE:
        add    ecx, 0x94
        dec    dword ptr [ebp - 8]
        jne    loc_1_308C
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_1_3125
        mov    eax, dword ptr [ebp + 0x18]
loc_1_3102:
        cmp    word ptr [ebx + 0x6a], -1
        je     loc_1_311C
        mov    word ptr [ebx + 0x8c], 1
        mov    ecx, dword ptr [ebp - 0x18]
        inc    dword ptr [ebp - 0x18]
        mov    dword ptr [ebp + ecx*4 - 0x4c], ebx
loc_1_311C:
        add    ebx, 0x94
        dec    eax
        jne    loc_1_3102
loc_1_3125:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        shl    eax, 5
        cmp    dword ptr [ebp + 0x18], 0
        lea    edx, [eax + ecx + 0x442]
        mov    dword ptr [ebp - 0xc], edx
        je     loc_1_371E
        mov    ebx, 0xff
loc_1_314E:
        mov    edx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + edx*4 - 0x4c]
        mov    ax, word ptr [edi + 0x8c]
        cmp    ax, 2
        jne    loc_1_3169
        movzx  eax, byte ptr [ebp + edx - 0x28]
        jmp    loc_1_31C7
loc_1_3169:
        cmp    ax, 4
        jne    loc_1_31A5
        push   1
        push   0xfe
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A68
        add    esp, 0x10
        cmp    eax, ebx
        jne    loc_1_3195
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 8]
        mov    byte ptr [ebp + edx - 0x28], bl
        jmp    loc_1_370F
loc_1_3195:
        mov    edx, dword ptr [ebp - 8]
        lea    ecx, [eax]
        inc    eax
        mov    byte ptr [ebp + edx - 0x28], al
        mov    byte ptr [ebp + edx - 0x27], cl
        jmp    loc_1_31C7
loc_1_31A5:
        push   0
        push   0xfe
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A68
        add    esp, 0x10
        mov    edx, dword ptr [ebp - 8]
        cmp    eax, ebx
        mov    byte ptr [ebp + edx - 0x28], al
        je     loc_1_370F
loc_1_31C7:
        shl    eax, 5
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0x76], 0x200000
        lea    esi, [eax + ecx + 0x42]
        jb     loc_1_31E9
        cmp    byte ptr [esi + 0x17], 0
        je     loc_1_31F9
        dec    dword ptr [ecx + 0x18]
        mov    byte ptr [esi + 0x17], 0
        jmp    loc_1_31F9
loc_1_31E9:
        cmp    byte ptr [esi + 0x17], 0
        jne    loc_1_31F9
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [ecx + 0x18]
        mov    byte ptr [esi + 0x17], 1
loc_1_31F9:
        mov    ax, word ptr [edi + 0xa]
        cmp    ax, 0xff80
        jge    loc_1_320B
        mov    word ptr [edi + 0xa], 0xff80
        jmp    loc_1_3217
loc_1_320B:
        cmp    ax, 0x7f
        jle    loc_1_3217
        mov    word ptr [edi + 0xa], 0x7f
loc_1_3217:
        lea    eax, [edi + 0xc]
        mov    cx, word ptr [eax]
        cmp    cx, -0x80
        jge    loc_1_322A
        mov    word ptr [eax], 0xff80
        jmp    loc_1_3235
loc_1_322A:
        cmp    cx, 0x7f
        jle    loc_1_3235
        mov    word ptr [eax], 0x7f
loc_1_3235:
        lea    ecx, [edi + 0xe]
        mov    ax, word ptr [ecx]
        cmp    ax, 0xff80
        jge    loc_1_3248
        mov    word ptr [ecx], 0xff80
        jmp    loc_1_3253
loc_1_3248:
        cmp    ax, 0x7f
        jle    loc_1_3253
        mov    word ptr [ecx], 0x7f
loc_1_3253:
        mov    ax, word ptr [edi + 0x10]
        test   ax, ax
        jge    loc_1_3264
        mov    word ptr [edi + 0x10], 0
        jmp    loc_1_326D
loc_1_3264:
        cmp    ax, bx
        jle    loc_1_326D
        mov    word ptr [edi + 0x10], bx
loc_1_326D:
        lea    eax, [edi + 0x12]
        mov    cx, word ptr [eax]
        test   cx, cx
        jge    loc_1_327F
        mov    word ptr [eax], 0
        jmp    loc_1_328A
loc_1_327F:
        cmp    cx, 0xf
        jle    loc_1_328A
        mov    word ptr [eax], 0xf
loc_1_328A:
        lea    eax, [edi + 0x14]
        mov    cx, word ptr [eax]
        cmp    cx, -0x80
        jge    loc_1_329D
        mov    word ptr [eax], 0xff80
        jmp    loc_1_32A8
loc_1_329D:
        cmp    cx, 0x7f
        jle    loc_1_32A8
        mov    word ptr [eax], 0x7f
loc_1_32A8:
        lea    ecx, [edi + 0x16]
        mov    ax, word ptr [ecx]
        cmp    ax, 0xff80
        jge    loc_1_32BB
        mov    word ptr [ecx], 0xff80
        jmp    loc_1_32C6
loc_1_32BB:
        cmp    ax, 0x7f
        jle    loc_1_32C6
        mov    word ptr [ecx], 0x7f
loc_1_32C6:
        lea    ecx, [edi + 0x1a]
        mov    ax, word ptr [ecx]
        cmp    ax, 0xff80
        jge    loc_1_32D9
        mov    word ptr [ecx], 0xff80
        jmp    loc_1_32E4
loc_1_32D9:
        cmp    ax, 0x7f
        jle    loc_1_32E4
        mov    word ptr [ecx], 0x7f
loc_1_32E4:
        lea    eax, [edi + 0x3a]
        mov    cx, word ptr [eax]
        test   cx, cx
        jge    loc_1_32F6
        mov    word ptr [eax], 0
        jmp    loc_1_3301
loc_1_32F6:
        cmp    cx, 0x7f
        jle    loc_1_3301
        mov    word ptr [eax], 0x7f
loc_1_3301:
        lea    eax, [edi + 0x4a]
        mov    cx, word ptr [eax]
        test   cx, cx
        jge    loc_1_3313
        mov    word ptr [eax], 0
        jmp    loc_1_331E
loc_1_3313:
        cmp    cx, 0x7f
        jle    loc_1_331E
        mov    word ptr [eax], 0x7f
loc_1_331E:
        movsx  ecx, byte ptr [edi + 0x8b]
        movsx  eax, word ptr [edi + 0x68]
        add    ecx, eax
        movsx  edx, word ptr [edi + 0x66]
        mov    ax, word ptr [edi + 0x74]
        add    ecx, edx
        cmp    ax, 0xffff
        mov    dword ptr [ebp - 0x18], ecx
        jne    loc_1_3347
        movzx  ecx, byte ptr [edi + 0x8a]
        jmp    loc_1_334A
loc_1_3347:
        movsx  ecx, ax
loc_1_334A:
        movsx  eax, word ptr [edi + 0x70]
        imul   eax, ecx
        mov    ecx, 0x36a
        sub    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [edi + 0x86]
        mov    edx, dword ptr [ebp - 0x18]
        imul   edx, ecx
        add    eax, edx
        mov    dword ptr [ebp - 0x18], eax
        movsx  edx, word ptr [edi + 0x5c]
        movsx  eax, word ptr [edi + 0x70]
        imul   edx, eax
        imul   edx, ecx
        mov    ecx, dword ptr [ebp - 0xc]
        add    dword ptr [ebp - 0x18], edx
        mov    eax, dword ptr [ebp - 0x18]
        sar    eax, 8
        mov    word ptr [esi + 0xe], ax
        mov    al, byte ptr [ecx + 9]
        cmp    al, 0xa
        ja     loc_1_3398
        mov    word ptr [edi + 0x26], bx
        jmp    loc_1_342A
loc_1_3398:
        mov    dword ptr [ebp - 4], 0x18
        movsx  edx, word ptr [edi + 0x5e]
        movzx  eax, al
        movzx  ecx, byte ptr [edx]           ; -> obj1:0x0AF8
        movzx  edx, byte ptr [eax]           ; -> loc_1_0B78
        movsx  eax, word ptr [edi + 0x60]
        add    ecx, edx
        add    eax, 0xc
        cdq    
        idiv   dword ptr [ebp - 4]
        add    ecx, eax
        sub    edx, edx
        lea    eax, [ecx*8]
        mov    ecx, 3
        div    ecx
        mov    edx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [edx + 0x10]
        lea    edx, [ecx + eax]
        cmp    edx, ebx
        ja     loc_1_3424
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 8]
        cmp    al, 0x7f
        jae    loc_1_3408
        movzx  ecx, al
        movzx  eax, byte ptr [ecx]           ; -> loc_1_0BF8
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, 0x100
        sub    eax, edx
        imul   eax, dword ptr [ebp - 0x18]
        shr    eax, 7
        add    edx, eax
loc_1_3408:
        cmp    byte ptr [esi + 0x17], 0
        je     loc_1_3426
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0x151e], 0x4d474d31
        jne    loc_1_3426
        add    edx, 0x10
        cmp    edx, ebx
        jbe    loc_1_3426
loc_1_3424:
        mov    edx, ebx
loc_1_3426:
        mov    word ptr [edi + 0x26], dx
loc_1_342A:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    word ptr [eax + 2], 0x80
        jae    loc_1_3519
        mov    ax, word ptr [edi + 0x40]
        test   ax, ax
        je     loc_1_3471
        movsx  edx, word ptr [edi + 0x5c]
        mov    ecx, 0x3c
        sub    ecx, edx
        movsx  eax, ax
        imul   ecx, eax
        movsx  edx, word ptr [edi + 0x38]
        add    ecx, edx
        mov    eax, 0x7fff
        cmp    ecx, eax
        jg     loc_1_346B
        mov    eax, 0xffff8000
        cmp    ecx, eax
        jge    loc_1_346D
loc_1_346B:
        mov    ecx, eax
loc_1_346D:
        mov    word ptr [edi + 0x38], cx
loc_1_3471:
        mov    ax, word ptr [edi + 0x50]
        test   ax, ax
        je     loc_1_34A9
        movsx  edx, word ptr [edi + 0x5c]
        mov    ecx, 0x3c
        sub    ecx, edx
        movsx  eax, ax
        imul   ecx, eax
        movsx  edx, word ptr [edi + 0x48]
        add    ecx, edx
        mov    eax, 0x7fff
        cmp    ecx, eax
        jg     loc_1_34A3
        mov    eax, 0xffff8000
        cmp    ecx, eax
        jge    loc_1_34A5
loc_1_34A3:
        mov    ecx, eax
loc_1_34A5:
        mov    word ptr [edi + 0x48], cx
loc_1_34A9:
        mov    ax, word ptr [edi + 0x3e]
        test   ax, ax
        je     loc_1_34E1
        movsx  edx, word ptr [edi + 0x5c]
        mov    ecx, 0x3c
        sub    ecx, edx
        movsx  eax, ax
        imul   ecx, eax
        movsx  edx, word ptr [edi + 0x36]
        add    ecx, edx
        mov    eax, 0x7fff
        cmp    ecx, eax
        jg     loc_1_34DB
        mov    eax, 0xffff8000
        cmp    ecx, eax
        jge    loc_1_34DD
loc_1_34DB:
        mov    ecx, eax
loc_1_34DD:
        mov    word ptr [edi + 0x36], cx
loc_1_34E1:
        mov    ax, word ptr [edi + 0x4e]
        test   ax, ax
        je     loc_1_3519
        movsx  edx, word ptr [edi + 0x5c]
        mov    ecx, 0x3c
        sub    ecx, edx
        movsx  eax, ax
        imul   ecx, eax
        movsx  edx, word ptr [edi + 0x46]
        add    ecx, edx
        mov    eax, 0x7fff
        cmp    ecx, eax
        jg     loc_1_3513
        mov    eax, 0xffff8000
        cmp    ecx, eax
        jge    loc_1_3515
loc_1_3513:
        mov    ecx, eax
loc_1_3515:
        mov    word ptr [edi + 0x46], cx
loc_1_3519:
        movsx  eax, word ptr [edi + 0x34]
        push   eax
        push   0x1a
        call   sub_4_2B72
loc_1_3525:
        mov    word ptr [edi + 0x34], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x44]
        push   eax
        push   0x22
        call   sub_4_2B72
loc_1_3538:
        mov    word ptr [edi + 0x44], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x2a]
        push   eax
        push   0x15
        call   sub_4_2B72
loc_1_354B:
        mov    word ptr [edi + 0x2a], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x2e]
        push   eax
        push   0x17
        call   sub_4_2B72
loc_1_355E:
        mov    word ptr [edi + 0x2e], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x32]
        push   eax
        push   0x19
        call   sub_4_2B72
loc_1_3571:
        mov    word ptr [edi + 0x32], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x42]
        push   eax
        push   0x21
        call   sub_4_2B72
loc_1_3584:
        mov    word ptr [edi + 0x42], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x38]
        push   eax
        push   0x1c
        call   sub_4_2B72
loc_1_3597:
        mov    word ptr [edi + 0x38], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x48]
        push   eax
        push   0x24
        call   sub_4_2B72
loc_1_35AA:
        mov    word ptr [edi + 0x48], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x3c]
        push   eax
        push   0x1e
        call   sub_4_2B72
loc_1_35BD:
        mov    word ptr [edi + 0x3c], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x4c]
        push   eax
        push   0x26
        call   sub_4_2B72
loc_1_35D0:
        mov    word ptr [edi + 0x4c], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x36]
        push   eax
        push   0x1b
        call   sub_4_2B72
loc_1_35E3:
        mov    word ptr [edi + 0x36], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x46]
        push   eax
        push   0x23
        call   sub_4_2B72
loc_1_35F6:
        mov    word ptr [edi + 0x46], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x2c]
        push   eax
        push   0x16
        call   sub_4_2B72
loc_1_3609:
        mov    word ptr [edi + 0x2c], ax
        add    esp, 8
        movsx  eax, word ptr [edi + 0x30]
        push   eax
        push   0x18
        call   sub_4_2B72
loc_1_361C:
        mov    word ptr [edi + 0x30], ax
        add    esp, 8
        mov    eax, dword ptr [ebp - 0xc]
        cmp    word ptr [eax + 2], 0x80
        jae    loc_1_3657
        cmp    word ptr [edi + 0x44], 0x7d
        jge    loc_1_3657
        movsx  eax, word ptr [edi + 0x5e]
        cmp    eax, 0x46
        jae    loc_1_3643
        mov    eax, 0x46
loc_1_3643:
        movsx  ecx, word ptr [edi + 0x10]
        imul   ecx, eax
        add    ecx, 0xa0
        shr    ecx, 7
        mov    word ptr [edi + 0x10], cx
loc_1_3657:
        movsx  eax, word ptr [edi + 0x22]
        shl    eax, 4
        mov    ecx, 0x7d
        cdq    
        idiv   ecx
        add    eax, 0x40
        jns    loc_1_3673
        mov    word ptr [edi + 0x22], 0
        jmp    loc_1_3682
loc_1_3673:
        mov    word ptr [edi + 0x22], 0x7f
        cmp    eax, 0x7f
        jg     loc_1_3682
        mov    word ptr [edi + 0x22], ax
loc_1_3682:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 0x12], 0
        jne    loc_1_36AD
        movsx  eax, word ptr [edi + 0x1e]
        imul   eax, eax, 0x64
        cdq    
        mov    ecx, 0x188
        idiv   ecx
        mov    word ptr [edi + 0x1e], ax
        movsx  eax, word ptr [edi + 0x20]
        imul   eax, eax, 0x64
        cdq    
        idiv   ecx
        mov    word ptr [edi + 0x20], ax
loc_1_36AD:
        mov    ax, word ptr [edi + 0x60]
        push   dword ptr [ebp - 0xc]
        push   edi
        mov    word ptr [esi + 4], ax
        mov    cx, word ptr [edi + 0x5e]
        mov    word ptr [esi + 2], cx
        mov    dl, byte ptr [edi + 0xa]
        mov    byte ptr [esi + 0x14], dl
        mov    al, byte ptr [edi + 0x3c]
        mov    byte ptr [esi + 0x15], al
        mov    cl, byte ptr [edi + 0x4c]
        mov    byte ptr [esi + 0x16], cl
        mov    dl, byte ptr [edi + 0x22]
        mov    byte ptr [esi + 0x12], dl
        mov    eax, dword ptr [edi + 0x90]
        mov    dword ptr [esi + 0x1c], eax
        mov    cl, byte ptr [edi + 0x72]
        mov    byte ptr [esi + 0x13], cl
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [edi + 0x8c]
        mov    word ptr [esi + 0x1a], dx
        movzx  eax, word ptr [ecx + 0x12]
        push   eax
        call   sub_1_4FCB
        add    esp, 0xc
        mov    cl, byte ptr [edi + 0x22]
        mov    byte ptr [esi + 0x18], cl
        mov    cl, byte ptr [edi + 0x22]
        mov    byte ptr [esi + 0x19], cl
loc_1_370F:
        inc    dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 0x18], edx
        ja     loc_1_314E
loc_1_371E:
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_1_3C87
        lea    eax, [ebp - 0x4c]
        mov    dword ptr [ebp - 0x2c], eax
loc_1_3735:
        mov    eax, dword ptr [ebp - 8]
        movzx  edi, byte ptr [ebp + eax - 0x28]
        cmp    edi, 0xff
        je     loc_1_3C74
        mov    eax, dword ptr [ebp - 0x2c]
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax]
        mov    eax, edi
        shl    eax, 5
        lea    edx, [eax + ecx + 0x42]
        mov    eax, dword ptr [ebp - 0xc]
        movzx  ecx, word ptr [edx + 0xe]
        add    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x1c], edx
        cmp    ecx, 0xffff
        jbe    loc_1_3778
        mov    dword ptr [ebp - 4], 0xffff
        jmp    loc_1_3786
loc_1_3778:
        mov    eax, 0
        test   ecx, ecx
        jl     loc_1_3783
        mov    eax, ecx
loc_1_3783:
        mov    dword ptr [ebp - 4], eax
loc_1_3786:
        mov    eax, dword ptr [esi + 0x76]
        sub    eax, 4
        test   byte ptr [esi + 0x6c], 1
        mov    dword ptr [ebp - 0x30], eax
        je     loc_1_37CD
        mov    eax, dword ptr [esi + 0x7e]
        mov    edx, dword ptr [ebp - 0x1c]
        dec    eax
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [esi + 0x82]
        mov    dword ptr [edx + 0xa], 0
        dec    eax
        test   byte ptr [esi + 0x6c], 2
        mov    dword ptr [ebp - 0x34], eax
        je     loc_1_37C1
        mov    eax, dword ptr [esi + 0x7a]
        add    eax, 4
        mov    dword ptr [edx + 6], eax
        jmp    loc_1_37E9
loc_1_37C1:
        mov    edx, dword ptr [ebp - 0x1c]
        mov    dword ptr [edx + 6], 0
        jmp    loc_1_37E9
loc_1_37CD:
        mov    eax, dword ptr [esi + 0x7a]
        mov    edx, dword ptr [ebp - 0x1c]
        add    eax, 8
        mov    dword ptr [edx + 6], 0
        lea    ecx, [eax - 4]
        mov    dword ptr [ebp - 0x34], eax
        mov    dword ptr [ebp - 0x20], ecx
        mov    dword ptr [edx + 0xa], ecx
loc_1_37E9:
        push   0xffff
        mov    eax, edi
        or     al, 0x60
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0xffff
        mov    eax, edi
        or     al, 0x40
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    ax, word ptr [esi + 0x44]
        cmp    ax, 0x80
        jne    loc_1_381E
        mov    eax, 0x7f
        jmp    loc_1_3821
loc_1_381E:
        movsx  eax, ax
loc_1_3821:
        movsx  ecx, word ptr [esi + 0x46]
        shl    ecx, 8
        or     ecx, eax
        mov    eax, edi
        push   ecx
        or     eax, 0x380
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [esi + 0x2a]
        push   ecx
        mov    eax, edi
        or     eax, 0x3a0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    ax, word ptr [esi + 0x34]
        cmp    ax, 0x80
        jne    loc_1_3861
        mov    eax, 0x7f
        jmp    loc_1_3864
loc_1_3861:
        movsx  eax, ax
loc_1_3864:
        movsx  ecx, word ptr [esi + 0x36]
        shl    ecx, 8
        lea    ebx, [esi + 0x10]
        or     ecx, eax
        mov    eax, edi
        push   ecx
        or     eax, 0x3c0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 0x3a]
        movsx  ecx, word ptr [esi + 0x38]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 0x2e0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [esi + 0x2e]
        push   ecx
        mov    eax, edi
        or     eax, 0x3e0
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
        movsx  ecx, word ptr [esi + 0x26]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 0x420
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 0xe]
        movsx  ecx, byte ptr [esi + 0x16]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 0x440
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    edx, dword ptr [ebp - 0xc]
        movzx  ecx, byte ptr [edx + 0xa]
        movzx  eax, byte ptr [edx + 0xb]
        add    ecx, eax
        movsx  eax, word ptr [esi + 0xa]
        add    eax, ecx
        cmp    eax, 0x7f
        jle    loc_1_391D
        mov    eax, 0x7f
loc_1_391D:
        movsx  ecx, byte ptr [esi + 0x14]
        shl    eax, 8
        or     ecx, eax
        mov    eax, edi
        push   ecx
        or     eax, 0x460
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 0x1a]
        movsx  ecx, word ptr [esi + 0x2c]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 0x480
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 0xc]
        movsx  ecx, word ptr [esi + 0x30]
        shl    eax, 8
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, 0x4a0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        cmp    word ptr [esi + 0x34], 0x80
        jne    loc_1_39D2
        cmp    word ptr [esi + 0x32], 0x8000
        jb     loc_1_39D2
        push   0xbfff
        mov    eax, edi
        or     eax, 0x2c0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  ecx, word ptr [esi + 0xe]
        shl    ecx, 4
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        mov    ecx, 0xffff
        cmp    eax, 0xffff
        ja     loc_1_39B2
        mov    ecx, eax
loc_1_39B2:
        mov    dword ptr [ebp - 4], ecx
        movsx  eax, word ptr [ebx]
        movsx  ecx, word ptr [esi + 0x16]
        add    eax, ecx
        jns    loc_1_39C4
        xor    eax, eax
        jmp    loc_1_39EA
loc_1_39C4:
        cmp    eax, 0xff
        jle    loc_1_39EA
        mov    eax, 0xff
        jmp    loc_1_39EA
loc_1_39D2:
        movsx  eax, word ptr [esi + 0x32]
        push   eax
        mov    eax, edi
        or     eax, 0x2c0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [ebx]
loc_1_39EA:
        shl    eax, 8
        cmp    dword ptr [ebp - 4], 0xffff
        mov    dword ptr [ebp - 0x10], eax
        jne    loc_1_3A02
        mov    dword ptr [ebp - 0x18], 0xffff
        jmp    loc_1_3A70
loc_1_3A02:
        mov    ebx, 1
        mov    ecx, dword ptr [ebp - 4]
        shr    ecx, 0xc
        shl    ebx, cl
        test   byte ptr [ebp - 3], 8
        je     loc_1_3A27
        mov    eax, ebx
        mov    ecx, 0x2710
        imul   eax, eax, 0x102e
        cdq    
        idiv   ecx
        add    ebx, eax
loc_1_3A27:
        test   byte ptr [ebp - 3], 4
        je     loc_1_3A3F
        mov    eax, ebx
        mov    ecx, 0x2710
        imul   eax, eax, 0x764
        cdq    
        idiv   ecx
        add    ebx, eax
loc_1_3A3F:
        test   byte ptr [ebp - 3], 2
        je     loc_1_3A57
        mov    eax, ebx
        mov    ecx, 0x2710
        imul   eax, eax, 0x389
        cdq    
        idiv   ecx
        add    ebx, eax
loc_1_3A57:
        mov    dword ptr [ebp - 0x18], 0xffff
        mov    eax, ebx
        sar    eax, 2
        add    ebx, eax
        cmp    ebx, 0xffff
        ja     loc_1_3A70
        mov    dword ptr [ebp - 0x18], ebx
loc_1_3A70:
        cmp    word ptr [esi + 0x44], 0x80
        jne    loc_1_3ACA
        cmp    word ptr [esi + 0x42], 0x8000
        jb     loc_1_3ACA
        push   0xbfff
        mov    eax, edi
        or     eax, 0x280
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        movsx  eax, word ptr [esi + 0x26]
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 0xf
        xor    eax, edx
        sub    eax, edx
        movsx  edx, ax
        movsx  eax, word ptr [esi + 0x26]
        mov    bx, word ptr [edx*2]          ; -> loc_1_0C78
        cdq    
        and    edx, 0xf
        add    eax, edx
        sar    eax, 4
        mov    cl, al
        shr    bx, cl
        movzx  eax, bx
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_3AE6
loc_1_3ACA:
        movsx  eax, word ptr [esi + 0x42]
        push   eax
        mov    eax, edi
        or     eax, 0x280
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        mov    dword ptr [ebp - 0x14], 0
loc_1_3AE6:
        mov    ebx, edi
        push   0
        or     bl, 0x20
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [esi + 0x22]
        shl    eax, 0x18
        or     dword ptr [ebp - 0x20], eax
        mov    eax, edi
        push   dword ptr [ebp - 0x20]
        or     al, 0xc0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [esi + 0x1e]
        shl    eax, 0x18
        or     dword ptr [ebp - 0x34], eax
        mov    eax, edi
        push   dword ptr [ebp - 0x34]
        or     al, 0xe0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   dword ptr [ebp - 0x30]
        mov    eax, edi
        or     ah, 2
        push   eax
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 0xa0
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0
        mov    eax, edi
        or     al, 0x80
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  eax, word ptr [esi + 0x12]
        shl    eax, 0x1c
        or     dword ptr [ebp - 0x30], eax
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    eax, dword ptr [ebp - 0x14]
        shl    eax, 0x10
        or     eax, dword ptr [ebp - 0x10]
        push   eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, edi
        or     al, 0x60
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   dword ptr [ebp - 4]
        mov    eax, edi
        or     al, 0x40
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        cmp    word ptr [esi + 0x8c], 4
        jne    loc_1_3BEA
        push   0
        lea    eax, [edi - 1]
        or     al, 0x20
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        movsx  ecx, word ptr [esi + 0x20]
        movsx  eax, word ptr [esi + 0x24]
        shl    ecx, 8
        or     ecx, eax
        push   ecx
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        push   0x8000
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        jmp    loc_1_3C63
loc_1_3BEA:
        mov    eax, dword ptr [ebp - 0x18]
        shl    eax, 0x10
        mov    dword ptr [ebp - 4], eax
        movsx  ecx, word ptr [esi + 0x24]
        movsx  eax, word ptr [esi + 0x20]
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
        movsx  eax, word ptr [esi + 0x4a]
        movsx  ecx, word ptr [esi + 0x48]
        shl    eax, 8
        mov    ebx, 0x2a0
        or     eax, ecx
        push   eax
        mov    eax, edi
        or     eax, ebx
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
        cmp    word ptr [esi + 0x8c], 2
        jne    loc_1_3C63
        mov    eax, dword ptr [ebp - 0x2c]
        inc    edi
        or     edi, ebx
        mov    eax, dword ptr [eax - 4]
        movsx  ecx, word ptr [eax + 0x4a]
        movsx  eax, word ptr [eax + 0x48]
        shl    ecx, 8
        or     ecx, eax
        push   ecx
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_19AA
loc_1_3C63:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp - 0x1c]
        shl    ax, 8
        or     ax, word ptr [ebp + 0x10]
        mov    word ptr [edx], ax
loc_1_3C74:
        add    dword ptr [ebp - 0x2c], 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 0x18], eax
        ja     loc_1_3735
loc_1_3C87:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
sub_1_3C92:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 8
        mov    ebp, esp
        sub    esp, 8
        or     eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], eax
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        shl    eax, 5
        lea    esi, [edi + 0x42]
        cmp    word ptr [edi + 6], 0
        lea    ebx, [eax + edi + 0x442]
        jbe    loc_1_3CF9
loc_1_3CC9:
        mov    ax, word ptr [esi]
        movzx  ecx, ax
        cmp    ecx, dword ptr [ebp - 8]
        jne    loc_1_3CEA
        cmp    byte ptr [ebx + 4], 0
        jne    loc_1_3CE5
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_1_3D04
        jmp    loc_1_3CEA
loc_1_3CE5:
        or     al, 0xff
        mov    word ptr [esi], ax
loc_1_3CEA:
        add    esi, 0x20
        inc    dword ptr [ebp - 4]
        movzx  eax, word ptr [edi + 6]
        cmp    eax, dword ptr [ebp - 4]
        ja     loc_1_3CC9
loc_1_3CF9:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_1_3D04:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0x8000
        lea    edi, [eax + esi + 0x42]
        mov    word ptr [edi], 0xff00
        movzx  eax, byte ptr [edi + 0x16]
        or     eax, ebx
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19AA
        movzx  eax, byte ptr [edi + 0x15]
        or     eax, ebx
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, 0x2e0
        push   eax
        push   esi
        call   sub_1_19AA
        cmp    dword ptr [edi + 6], 0
        je     loc_1_3DA5
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, 0xff000000
        or     al, 0xe0
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
        mov    eax, dword ptr [ebp + 0xc]
        or     al, 0xc0
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
        mov    dword ptr [edi + 0xa], ecx
loc_1_3DA5:
        mov    word ptr [edi], 0xffe
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
        sub    esp, 0x37c
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x14]
        cmp    edi, eax
        jne    loc_1_3DDC
        push   eax
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_3C92
        jmp    loc_1_3E8F
loc_1_3DDC:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        shl    ecx, 5
        lea    ebx, [ecx + esi + 0x442]
        lea    ecx, [ecx + esi + 0x456]
        mov    dx, word ptr [ecx]
        cmp    dx, 0x120
        jae    loc_1_3E37
        movzx  edx, dx
        mov    edx, dword ptr [esi + edx*4 + 0x662]
        test   edx, edx
        je     loc_1_3E37
        movzx  eax, word ptr [edx + 0x48]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 1
        movzx  ecx, word ptr [ecx]
        lea    eax, [ebp - 0x37c]
        push   eax
        push   edi
        push   dword ptr [ebp + 0x10]
        movzx  eax, word ptr [ebx]
        push   eax
        push   ecx
        push   esi
        jne    loc_1_3E32
        call   sub_1_1FD6
        jmp    loc_1_3E37
loc_1_3E32:
        call   sub_1_2359
loc_1_3E37:
        test   eax, eax
        jne    loc_1_3E6D
        cmp    word ptr [ebx + 2], 0x80
        jae    loc_1_3E4A
        mov    eax, 1
        jmp    loc_1_3E8F
loc_1_3E4A:
        lea    eax, [ebp - 0x37c]
        lea    edx, [ebp - 4]
        push   eax
        push   edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   edx
        push   esi
        call   sub_1_3E98
        test   eax, eax
        jne    loc_1_3E6D
        mov    eax, 1
        jmp    loc_1_3E8F
loc_1_3E6D:
        cmp    dword ptr [ebp - 4], 1
        lea    ecx, [ebp - 0x37c]
        push   ecx
        push   eax
        push   edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   esi
        jne    loc_1_3E8A
        call   sub_1_27BC
        jmp    loc_1_3E8F
loc_1_3E8A:
        call   sub_1_2FFE
loc_1_3E8F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_1_3E98:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        shl    eax, 5
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        push   ebx
        lea    eax, [eax + ecx + 0x442]
        push   esi
        mov    ebx, 0x80
        movzx  ecx, word ptr [eax + 2]
        push   edi
        mov    dword ptr [ebp - 4], ecx
        movzx  esi, byte ptr [eax + 0x16]
loc_1_3EC2:
        cmp    dword ptr [ebp - 4], ebx
        jbe    loc_1_3ECC
        mov    dword ptr [ebp - 4], ebx
        jmp    loc_1_3EE7
loc_1_3ECC:
        test   esi, esi
        je     loc_1_3F54
        sub    esi, 0x80
        cmp    esi, 1
        mov    esi, 0
        adc    esi, -1
        and    esi, ebx
loc_1_3EE7:
        mov    edi, esi
        cmp    esi, 0x120
        jae    loc_1_3EC2
loc_1_3EF1:
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + edi*4 + 0x662]
        test   ecx, ecx
        je     loc_1_3F37
        movzx  eax, word ptr [ecx + 0x24]
        cmp    eax, esi
        jne    loc_1_3F37
        push   dword ptr [ebp + 0x1c]
        movzx  eax, word ptr [ecx + 0x48]
        mov    edx, dword ptr [ebp + 0xc]
        cmp    eax, 1
        push   dword ptr [ebp + 0x18]
        mov    dword ptr [edx], eax
        push   dword ptr [ebp + 0x14]
        movzx  ecx, word ptr [ecx + 0x26]
        push   ecx
        push   edi
        push   dword ptr [ebp + 8]
        jne    loc_1_3F2E
        call   sub_1_1FD6
        jmp    loc_1_3F33
loc_1_3F2E:
        call   sub_1_2359
loc_1_3F33:
        test   eax, eax
        jne    loc_1_3F56
loc_1_3F37:
        cmp    edi, 0x80
        jbe    loc_1_3F42
        inc    edi
        jmp    loc_1_3F47
loc_1_3F42:
        mov    edi, 0x81
loc_1_3F47:
        cmp    edi, 0x120
        jb     loc_1_3EF1
        jmp    loc_1_3EC2
loc_1_3F54:
        xor    eax, eax
loc_1_3F56:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
sub_1_3F5F:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0x10]
        push   edi
        mov    eax, ebx
        mov    edi, dword ptr [esp + 0x10]
        shl    eax, 5
        mov    esi, dword ptr [esp + 0x18]
        lea    ecx, [eax + edi]
        xor    eax, eax
        mov    word ptr [ecx + 0x45a], ax
        test   byte ptr [esi + 0x64], 0x80
        jne    loc_1_3F91
        mov    byte ptr [esi + 0x64], al
        and    word ptr [ecx + 0x45c], 0xff00
loc_1_3F91:
        test   byte ptr [esi + 0x65], 0x80
        jne    loc_1_3FA4
        mov    byte ptr [esi + 0x65], 0
        and    word ptr [ecx + 0x45c], 0xff
loc_1_3FA4:
        test   byte ptr [esi + 0x62], 0x80
        jne    loc_1_3FB7
        mov    byte ptr [esi + 0x62], 0
        and    word ptr [ecx + 0x45e], 0xff80
loc_1_3FB7:
        test   byte ptr [esi + 0x63], 0x80
        jne    loc_1_3FDC
        mov    byte ptr [esi + 0x63], 0
        and    word ptr [ecx + 0x45e], 0x7f
        mov    ecx, dword ptr [edi + 0xaea]
        test   ecx, ecx
        je     loc_1_3FDC
        push   0
        push   ebx
        push   edi
        call   ecx
        add    esp, 0xc
loc_1_3FDC:
        test   byte ptr [esi + 1], 0x80
        jne    loc_1_3FEF
        mov    byte ptr [esi + 1], 0
        push   0
        push   ebx
        push   edi
        call   sub_1_41FA
loc_1_3FEF:
        test   byte ptr [esi + 0x40], 0x80
        jne    loc_1_4002
        mov    byte ptr [esi + 0x40], 0
        push   0
        push   ebx
        push   edi
        call   sub_1_4190
loc_1_4002:
        test   byte ptr [esi + 0xb], 0x80
        jne    loc_1_4015
        mov    byte ptr [esi + 0xb], 0x7f
        push   0x7f
        push   ebx
        push   edi
        call   sub_1_4031
loc_1_4015:
        push   0x40
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
        ret    0xc
sub_1_4031:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 0x10]
        add    eax, edx
        mov    byte ptr [eax + 0x44a], cl
        movzx  eax, byte ptr [eax + 0x44b]
        push   eax
        push   dword ptr [ebp + 0xc]
        push   edx
        call   sub_1_405E
        pop    ebp
        ret    0xc
sub_1_405E:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    ebp, esp
        sub    esp, 0xc
        mov    dl, byte ptr [ebp + 0x10]
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    byte ptr [eax + esi + 0x44b], dl
        lea    ecx, [eax + esi + 0x442]
        lea    edi, [esi + 0x42]
        mov    dword ptr [ebp - 0xc], ecx
        cmp    word ptr [esi + 6], 0
        jbe    loc_1_4185
loc_1_409A:
        mov    ax, word ptr [edi]
        shr    ax, 8
        movzx  ecx, ax
        cmp    ecx, dword ptr [ebp + 0xc]
        jne    loc_1_4172
        cmp    dword ptr [ebp + 0x10], 0xa
        jbe    loc_1_4172
        movsx  eax, word ptr [edi + 2]
        movzx  ebx, byte ptr [eax]           ; -> obj1:0x0AF8
        movsx  eax, word ptr [edi + 4]
        add    eax, 0xc
        mov    ecx, 0x18
        cdq    
        idiv   ecx
        add    ebx, eax
        mov    edx, dword ptr [ebp + 0x10]
        movzx  eax, byte ptr [edx]           ; -> loc_1_0B78
        add    ebx, eax
        sub    edx, edx
        mov    ecx, 3
        lea    eax, [ebx*8]
        div    ecx
        movzx  edx, word ptr [esi + 0x10]
        add    eax, edx
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 0xff
        jae    loc_1_4143
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 8]
        cmp    al, 0x7f
        jae    loc_1_4124
        movzx  eax, al
        movzx  ecx, byte ptr [eax]           ; -> loc_1_0BF8
        mov    eax, 0x100
        sub    eax, dword ptr [ebp - 4]
        imul   ecx, eax
        shr    ecx, 7
        add    dword ptr [ebp - 4], ecx
loc_1_4124:
        cmp    dword ptr [esi + 0x151e], 0x4d474d31
        jne    loc_1_414A
        cmp    byte ptr [edi + 0x17], 0
        je     loc_1_414A
        add    dword ptr [ebp - 4], 0x10
        cmp    dword ptr [ebp - 4], 0xff
        jbe    loc_1_414A
loc_1_4143:
        mov    dword ptr [ebp - 4], 0xff
loc_1_414A:
        mov    ebx, dword ptr [ebp - 8]
        or     ebx, 0x420
        push   ebx
        push   esi
        call   sub_1_19DF
        xor    ecx, ecx
        mov    cx, ax
        and    ecx, 0xff00
        or     dword ptr [ebp - 4], ecx
        push   dword ptr [ebp - 4]
        push   ebx
        push   esi
        call   sub_1_19AA
loc_1_4172:
        add    edi, 0x20
        inc    dword ptr [ebp - 8]
        movzx  eax, word ptr [esi + 6]
        cmp    eax, dword ptr [ebp - 8]
        ja     loc_1_409A
loc_1_4185:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_4190:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x10], 0x40
        jae    loc_1_41E3
        shl    eax, 5
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        lea    edi, [esi + 0x42]
        mov    byte ptr [eax + esi + 0x446], bl
        cmp    word ptr [esi + 6], bx
        jbe    loc_1_41F1
loc_1_41B8:
        mov    ax, word ptr [edi]
        mov    cx, ax
        shr    cx, 8
        cmp    al, 0xff
        movzx  ecx, cx
        jne    loc_1_41D5
        cmp    ecx, dword ptr [ebp + 0xc]
        jne    loc_1_41D5
        push   ebx
        push   esi
        call   sub_1_3D04
loc_1_41D5:
        add    edi, 0x20
        inc    ebx
        movzx  eax, word ptr [esi + 6]
        cmp    eax, ebx
        ja     loc_1_41B8
        jmp    loc_1_41F1
loc_1_41E3:
        shl    eax, 5
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [eax + esi + 0x446], 1
loc_1_41F1:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
sub_1_41FA:
        push   ebp
        mov    ecx, 0x1e
        mov    ebp, esp
        sub    edx, edx
        sub    esp, 0xc
        mov    eax, dword ptr [ebp + 0x10]
        div    ecx
        push   ebx
        mov    dword ptr [ebp + 0x10], eax
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        shl    eax, 5
        mov    cl, byte ptr [ebp + 0x10]
        add    eax, edi
        lea    ebx, [edi + 0x42]
        xor    esi, esi
        mov    dword ptr [ebp - 0xc], eax
        mov    byte ptr [eax + 0x44c], cl
        cmp    word ptr [edi + 6], si
        jbe    loc_1_429A
loc_1_4234:
        mov    ax, word ptr [ebx]
        shr    ax, 8
        movzx  ecx, ax
        cmp    ecx, dword ptr [ebp + 0xc]
        jne    loc_1_428E
        mov    edx, dword ptr [ebp - 0xc]
        movsx  ecx, byte ptr [ebx + 0x14]
        movzx  eax, byte ptr [edx + 0x44d]
        add    ecx, eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 0x7f
        jle    loc_1_4267
        mov    dword ptr [ebp - 4], 0x7f
loc_1_4267:
        shl    dword ptr [ebp - 4], 8
        mov    eax, esi
        or     eax, 0x460
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
        add    ebx, 0x20
        inc    esi
        movzx  eax, word ptr [edi + 6]
        cmp    eax, esi
        ja     loc_1_4234
loc_1_429A:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_42A5:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        cmp    eax, 6
        ja     loc_1_42CA
        je     loc_1_4344
        test   eax, eax
        je     loc_1_431E
        cmp    eax, 1
        je     loc_1_4331
loc_1_42C0:
        mov    eax, 1
        jmp    loc_1_4466
loc_1_42CA:
        cmp    eax, 0xa
        ja     loc_1_42DC
        je     loc_1_436A
        cmp    eax, 7
        je     loc_1_4357
        jmp    loc_1_42C0
loc_1_42DC:
        cmp    eax, 0x26
        ja     loc_1_42F2
        je     loc_1_4390
        cmp    eax, 0xb
        je     loc_1_437D
        jmp    loc_1_42C0
loc_1_42F2:
        cmp    eax, 0x5b
        ja     loc_1_4308
        je     loc_1_43B6
        cmp    eax, 0x40
        je     loc_1_43A3
        jmp    loc_1_42C0
loc_1_4308:
        sub    eax, 0x5d
        cmp    eax, 0x1e
        ja     loc_1_42C0
        movzx  ecx, byte ptr [eax]           ; -> obj1:0x448E
        jmp    dword ptr [ecx*4]             ; -> obj1:0x446A
loc_1_431E:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_44AD
        jmp    loc_1_4464
loc_1_4331:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_41FA
        jmp    loc_1_4464
loc_1_4344:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_46C7
        jmp    loc_1_4464
loc_1_4357:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_405E
        jmp    loc_1_4464
loc_1_436A:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_4516
        jmp    loc_1_4464
loc_1_437D:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_4031
        jmp    loc_1_4464
loc_1_4390:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_4745
        jmp    loc_1_4464
loc_1_43A3:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_4190
        jmp    loc_1_4464
loc_1_43B6:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_44C8
        jmp    loc_1_4464
loc_1_43C9:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_44EF
        jmp    loc_1_4464
loc_1_43DC:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0xae6]
        test   ecx, ecx
        je     loc_1_43F7
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   ecx
        add    esp, 0xc
        jmp    loc_1_4466
loc_1_43F7:
        mov    eax, 1
        jmp    loc_1_4466
loc_1_43FE:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0xaea]
        test   ecx, ecx
        je     loc_1_4419
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   eax
        call   ecx
        add    esp, 0xc
        jmp    loc_1_4466
loc_1_4419:
        mov    eax, 1
        jmp    loc_1_4466
loc_1_4420:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_4657
        jmp    loc_1_4464
loc_1_4430:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_468E
        jmp    loc_1_4464
loc_1_4440:
        push   0
        jmp    loc_1_4456
loc_1_4444:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_47A1
        jmp    loc_1_4464
loc_1_4454:
        push   1
loc_1_4456:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_480D
loc_1_4464:
        xor    eax, eax
loc_1_4466:
        pop    ebp
        ret    0x10
        ; ---- dispatch table (9 entries) ----
        dd  loc_1_43C9
        dd  loc_1_43DC
        dd  loc_1_43FE
        dd  loc_1_4420
        dd  loc_1_4430
        dd  loc_1_4440
        dd  loc_1_4444
        dd  loc_1_4454
        dd  loc_1_42C0
        db  000h, 008h, 008h, 008h, 008h, 001h, 002h, 003h, 004h, 008h, 008h, 008h, 008h, 008h, 008h, 008h
        db  008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 008h, 005h, 006h, 008h, 007h
sub_1_44AD:
        mov    ecx, dword ptr [esp + 8]
        mov    dl, byte ptr [esp + 0xc]
        shl    ecx, 5
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [ecx + eax + 0x459], dl
        xor    eax, eax
        ret    0xc
sub_1_44C8:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, 0x64
        imul   eax, eax, 0x5a
        sub    edx, edx
        div    ecx
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shl    edx, 5
        mov    byte ptr [edx + ecx + 0x447], al
        xor    eax, eax
        ret    0xc
sub_1_44EF:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, 0x64
        imul   eax, eax, 0x5a
        sub    edx, edx
        div    ecx
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        shl    edx, 5
        mov    byte ptr [edx + ecx + 0x448], al
        xor    eax, eax
        ret    0xc
sub_1_4516:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 0x10]
        lea    eax, [eax + esi + 0x449]
        cmp    byte ptr [eax], cl
        je     loc_1_4599
        mov    byte ptr [eax], cl
        cmp    word ptr [esi + 0x12], 1
        je     loc_1_4599
        xor    ebx, ebx
        cmp    word ptr [esi + 6], bx
        jbe    loc_1_4599
        lea    edi, [esi + 0x42]
loc_1_4548:
        mov    ax, word ptr [edi]
        shr    ax, 8
        movzx  ecx, ax
        cmp    ecx, dword ptr [ebp + 0xc]
        jne    loc_1_458D
        movzx  ecx, byte ptr [edi + 0x12]
        mov    eax, 0x17f
        add    ecx, dword ptr [ebp + 0x10]
        add    ecx, ecx
        sub    eax, ecx
        cmp    eax, 0xfe
        jl     loc_1_4575
        mov    eax, 0xff
        jmp    loc_1_457C
loc_1_4575:
        cmp    eax, 1
        jg     loc_1_457C
        xor    eax, eax
loc_1_457C:
        push   0
        mov    byte ptr [edi + 0x18], al
        push   ebx
        push   0
        push   esi
        call   sub_1_45A2
        add    esp, 0x10
loc_1_458D:
        add    edi, 0x20
        inc    ebx
        movzx  eax, word ptr [esi + 6]
        cmp    eax, ebx
        ja     loc_1_4548
loc_1_4599:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
sub_1_45A2:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        shl    ecx, 5
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, ecx
        cmp    word ptr [esi + 0x42], 0x2000
        jae    loc_1_4652
        mov    cl, byte ptr [esi + 0x5a]
        mov    al, byte ptr [esi + 0x5b]
        cmp    cl, al
        je     loc_1_4652
        movzx  ebx, al
        movzx  eax, cl
        cmp    eax, ebx
        jle    loc_1_45DB
        inc    ebx
        jmp    loc_1_45DC
loc_1_45DB:
        dec    ebx
loc_1_45DC:
        mov    edi, dword ptr [ebp + 0x10]
        mov    byte ptr [esi + 0x5b], bl
        or     edi, 0xc0
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_1A3D
        and    eax, 0xffffff
        mov    ecx, ebx
        shl    ecx, 0x18
        or     ecx, eax
        push   ecx
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        test   ebx, ebx
        je     loc_1_4612
        neg    bl
        movzx  edi, bl
        jmp    loc_1_4617
loc_1_4612:
        mov    edi, 0xff
loc_1_4617:
        mov    ebx, dword ptr [ebp + 0x10]
        or     bl, 0x20
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A3D
        and    al, 0
        or     edi, eax
        push   edi
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_1A14
        mov    al, byte ptr [esi + 0x5b]
        cmp    byte ptr [esi + 0x5a], al
        je     loc_1_4652
        push   0
        add    esi, 0x5a
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        push   0                             ; -> sub_1_45A2
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
        movzx  dx, byte ptr [esp + 0xc]
        mov    word ptr [ecx + 0x45a], 0x100
        add    ecx, 0x45c
        mov    ax, word ptr [ecx]
        and    ax, 0xff00
        or     dx, ax
        mov    word ptr [ecx], ax
        xor    eax, eax
        mov    word ptr [ecx], dx
        ret    0xc
sub_1_468E:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        shl    ecx, 5
        mov    edx, dword ptr [esp + 0xc]
        shl    dx, 8
        add    ecx, eax
        mov    word ptr [ecx + 0x45a], 0x100
        add    ecx, 0x45c
        mov    ax, word ptr [ecx]
        and    ax, 0xff
        or     dx, ax
        mov    word ptr [ecx], ax
        xor    eax, eax
        mov    word ptr [ecx], dx
        ret    0xc
sub_1_46C7:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [eax + edx]
        movzx  eax, word ptr [esi + 0x45a]
        cmp    eax, 0x100
        je     loc_1_46F9
        cmp    eax, 0x200
        je     loc_1_4710
loc_1_46EE:
        mov    eax, 1
loc_1_46F3:
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
loc_1_46F9:
        cmp    word ptr [esi + 0x45c], 0
        jne    loc_1_46EE
        mov    al, byte ptr [ebp + 0x10]
        mov    byte ptr [esi + 0x44f], al
        xor    eax, eax
        jmp    loc_1_46F3
loc_1_4710:
        lea    edi, [edx + 0xae2]
        cmp    dword ptr [edi], 0
        je     loc_1_46EE
        add    esi, 0x460
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ax, word ptr [esi]
        push   0
        and    ax, 0x7f
        push   dword ptr [ebp + 0xc]
        push   edx
        shl    cx, 7
        or     cx, ax
        mov    word ptr [esi], ax
        mov    word ptr [esi], cx
        call   dword ptr [edi]
        add    esp, 0xc
        jmp    loc_1_46F3
sub_1_4745:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [eax + edx]
        movzx  eax, word ptr [esi + 0x45a]
        cmp    eax, 0x200
        jne    loc_1_4796
        lea    edi, [edx + 0xae2]
        cmp    dword ptr [edi], 0
        je     loc_1_4796
        add    esi, 0x460
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ax, word ptr [esi]
        push   0
        and    ax, 0xff80
        push   dword ptr [ebp + 0xc]
        push   edx
        or     cx, ax
        mov    word ptr [esi], ax
        mov    word ptr [esi], cx
        call   dword ptr [edi]
        add    esp, 0xc
        jmp    loc_1_479B
loc_1_4796:
        mov    eax, 1
loc_1_479B:
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
sub_1_47A1:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        xor    ebx, ebx
        mov    eax, edi
        push   ebx
        shl    eax, 5
        push   edi
        add    eax, esi
        push   esi
        mov    word ptr [eax + 0x45a], bx
        mov    word ptr [eax + 0x45c], bx
        mov    word ptr [eax + 0x45e], bx
        mov    byte ptr [eax + 0x44c], bl
        call   sub_1_4190
        push   0x7f
        push   edi
        push   esi
        call   sub_1_4031
        push   0x40
        push   ebx
        push   edi
        push   esi
        call   sub_1_4AD6
        push   ebx
        push   edi
        push   esi
        call   sub_1_4A9E
        mov    eax, dword ptr [esi + 0xaea]
        test   eax, eax
        je     loc_1_4805
        push   ebx
        push   edi
        push   esi
        call   eax
        add    esp, 0xc
loc_1_4805:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    0xc
sub_1_480D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [ebp + 0x14], 0
        push   ebx
        push   esi
        push   edi
        je     loc_1_4836
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        shl    eax, 5
        cmp    byte ptr [eax + edi + 0x446], 0
        je     loc_1_4839
        mov    ebx, 1
        jmp    loc_1_483B
loc_1_4836:
        mov    edi, dword ptr [ebp + 8]
loc_1_4839:
        xor    ebx, ebx
loc_1_483B:
        mov    dword ptr [ebp - 4], 0
        cmp    word ptr [edi + 6], 0
        jbe    loc_1_4882
        lea    esi, [edi + 0x42]
loc_1_484C:
        mov    cx, word ptr [esi]
        mov    ax, cx
        shr    ax, 8
        movzx  edx, ax
        cmp    edx, dword ptr [ebp + 0xc]
        jne    loc_1_4873
        test   ebx, ebx
        je     loc_1_486A
        or     cl, 0xff
        mov    word ptr [esi], cx
        jmp    loc_1_4873
loc_1_486A:
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_1_3D04
loc_1_4873:
        add    esi, 0x20
        inc    dword ptr [ebp - 4]
        movzx  eax, word ptr [edi + 6]
        cmp    eax, dword ptr [ebp - 4]
        ja     loc_1_484C
loc_1_4882:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_1_488D:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    eax, edx
        shl    eax, 5
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        xor    esi, esi
        mov    bl, byte ptr [eax + ecx + 0x459]
        mov    byte ptr [eax + ecx + 0x458], bl
        lea    eax, [eax + ecx + 0x442]
        push   dword ptr [ebp + 0x10]
        push   edx
        mov    word ptr [eax + 0x14], si
        push   ecx
        mov    word ptr [eax], si
        cmp    edx, 9
        mov    word ptr [eax + 2], si
        jne    loc_1_48D1
        call   sub_1_48DC
        jmp    loc_1_48D6
loc_1_48D1:
        call   sub_1_49F5
loc_1_48D6:
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
sub_1_48DC:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    ebp, esp
        sub    esp, 0xc
        mov    dword ptr [ebp - 8], 1
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        movzx  edx, byte ptr [eax + ebx + 0x458]
        lea    edi, [eax + ebx + 0x442]
        mov    esi, 0x80
        or     dword ptr [ebp + 0x10], esi
        test   edx, edx
        jne    loc_1_4913
        mov    edx, esi
loc_1_4913:
        mov    ecx, edx
loc_1_4915:
        cmp    ecx, 0x120
        jae    loc_1_49AB
loc_1_4921:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + ecx*4 + 0x662]
        test   eax, eax
        je     loc_1_498F
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [eax]
        cmp    word ptr [eax], 0xffff
        je     loc_1_498F
        add    eax, 2
loc_1_4942:
        cmp    word ptr [eax - 2], 0
        je     loc_1_4981
        cmp    byte ptr [eax + 1], dl
        jne    loc_1_4981
        movzx  ebx, byte ptr [eax]
        mov    dword ptr [ebp - 4], ebx
        cmp    ebx, dword ptr [ebp + 0x10]
        ja     loc_1_4981
        cmp    esi, ebx
        ja     loc_1_4981
        mov    word ptr [edi + 0x14], cx
        mov    esi, ebx
        mov    bx, word ptr [ebp - 0xc]
        mov    word ptr [edi], bx
        mov    bx, word ptr [ebp - 4]
        mov    word ptr [edi + 2], bx
        movzx  ebx, byte ptr [eax]
        cmp    ebx, dword ptr [ebp + 0x10]
        je     loc_1_49DF
        mov    dword ptr [ebp - 8], 0
loc_1_4981:
        add    eax, 0x1a
        inc    dword ptr [ebp - 0xc]
        cmp    word ptr [eax - 2], 0xffff
        jne    loc_1_4942
loc_1_498F:
        cmp    ecx, 0x80
        jbe    loc_1_499A
        inc    ecx
        jmp    loc_1_499F
loc_1_499A:
        mov    ecx, 0x81
loc_1_499F:
        cmp    ecx, 0x120
        jb     loc_1_4921
loc_1_49AB:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_49BB
        test   edx, edx
        je     loc_1_49E3
        cmp    byte ptr [edi + 0x16], 0
        jne    loc_1_49E3
loc_1_49BB:
        test   edx, edx
        je     loc_1_49E7
        cmp    edx, 0x80
        jne    loc_1_49D0
        xor    edx, edx
        xor    ecx, ecx
        jmp    loc_1_4915
loc_1_49D0:
        mov    edx, 0x80
        mov    ecx, edx
        mov    dword ptr [ebp + 0x10], edx
        jmp    loc_1_4915
loc_1_49DF:
        xor    eax, eax
        jmp    loc_1_49EC
loc_1_49E3:
        xor    eax, eax
        jmp    loc_1_49EC
loc_1_49E7:
        mov    eax, 1
loc_1_49EC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_49F5:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        shl    ecx, 5
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   ebx
        lea    edx, [ecx + eax + 0x442]
        push   esi
        xor    ecx, ecx
        push   edi
        movzx  esi, byte ptr [edx + 0x16]
        test   esi, esi
        jne    loc_1_4A20
loc_1_4A19:
        mov    ecx, 0x80
        mov    esi, ecx
loc_1_4A20:
        cmp    ecx, 0x120
        jae    loc_1_4A66
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx*4 + 0x662]
        test   eax, eax
        je     loc_1_4A63
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    word ptr [edi], 0xffff
        je     loc_1_4A63
loc_1_4A41:
        cmp    word ptr [edi], 0
        je     loc_1_4A58
        movzx  ebx, byte ptr [edi + 3]
        cmp    ebx, esi
        jne    loc_1_4A58
        movzx  ebx, byte ptr [edi + 2]
        cmp    ebx, dword ptr [ebp + 0x10]
        je     loc_1_4A78
loc_1_4A58:
        add    edi, 0x1a
        inc    eax
        cmp    word ptr [edi], 0xffff
        jne    loc_1_4A41
loc_1_4A63:
        inc    ecx
        jmp    loc_1_4A20
loc_1_4A66:
        test   esi, esi
        je     loc_1_4A8B
        cmp    esi, 0x80
        jne    loc_1_4A19
        xor    ecx, ecx
        xor    esi, esi
        jmp    loc_1_4A20
loc_1_4A78:
        mov    word ptr [edx + 0x14], cx
        mov    word ptr [edx], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [edx + 2], ax
        xor    eax, eax
        jmp    loc_1_4A90
loc_1_4A8B:
        mov    eax, 1
loc_1_4A90:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_1_4A99:
        xor    eax, eax
        ret    0x10
sub_1_4A9E:
        push   ebp
        mov    ecx, 0x1e
        mov    ebp, esp
        sub    edx, edx
        mov    eax, dword ptr [ebp + 0x10]
        div    ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        shl    ecx, 5
        add    ecx, edx
        mov    byte ptr [ecx + 0x44d], al
        movzx  eax, byte ptr [ecx + 0x44c]
        imul   eax, eax, 0x1e
        push   eax
        push   dword ptr [ebp + 0xc]
        push   edx
        call   sub_1_41FA
        pop    ebp
        ret    0xc
sub_1_4AD6:
        mov    ecx, dword ptr [esp + 0x10]
        sub    esp, 4
        sub    ecx, 0x40
        push   ebx
        shl    ecx, 7
        push   esi
        add    ecx, dword ptr [esp + 0x18]
        push   edi
        mov    eax, dword ptr [esp + 0x18]
        push   ebp
        shl    eax, 5
        mov    edi, dword ptr [esp + 0x18]
        lea    ebx, [eax + edi]
        movzx  eax, byte ptr [ebx + 0x44f]
        test   eax, eax
        jg     loc_1_4B09
        mov    eax, 2
loc_1_4B09:
        movsx  eax, ax
        imul   eax, ecx
        mov    ecx, 0x18
        xor    ebp, ebp
        cdq    
        idiv   ecx
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [ebx + 0x452], eax
        cmp    word ptr [edi + 6], bp
        jbe    loc_1_4B7F
        lea    esi, [edi + 0x42]
loc_1_4B2C:
        mov    ax, word ptr [esi]
        shr    ax, 8
        movzx  ecx, ax
        cmp    ecx, dword ptr [esp + 0x1c]
        jne    loc_1_4B73
        movsx  ecx, word ptr [ebx + 0x450]
        movzx  eax, word ptr [esi + 0xe]
        add    ecx, eax
        mov    eax, dword ptr [esp + 0x10]
        add    eax, ecx
        cmp    eax, 0xffff
        jbe    loc_1_4B5D
        mov    eax, 0xffff
        jmp    loc_1_4B63
loc_1_4B5D:
        test   eax, eax
        jge    loc_1_4B63
        xor    eax, eax
loc_1_4B63:
        movzx  eax, ax
        push   eax
        mov    eax, ebp
        or     ah, 4
        push   eax
        push   edi
        call   sub_1_19AA
loc_1_4B73:
        add    esi, 0x20
        inc    ebp
        movzx  eax, word ptr [edi + 6]
        cmp    eax, ebp
        ja     loc_1_4B2C
loc_1_4B7F:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0x10
sub_1_4B8B:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, 0                        ; -> obj1:0x0C98
        mov    esi, dword ptr [ebp + 0xc]
loc_1_4B9B:
        mov    eax, edi
        xor    ecx, ecx
        cmp    dword ptr [ebp + 0x10], ecx
        je     loc_1_4BBC
loc_1_4BA4:
        mov    bl, byte ptr [eax]
        cmp    bl, 0xfe
        je     loc_1_4BB5
        cmp    bl, 0xff
        je     loc_1_4BBC
        cmp    byte ptr [esi + ecx], bl
        jne    loc_1_4BBC
loc_1_4BB5:
        inc    ecx
        inc    eax
        cmp    ecx, dword ptr [ebp + 0x10]
        jb     loc_1_4BA4
loc_1_4BBC:
        cmp    byte ptr [eax], 0xff
        je     loc_1_4BD9
        add    edi, 0x16
        inc    edx
        cmp    edi, 0x2c                     ; -> obj1:0x0CC4
        jb     loc_1_4B9B
        mov    eax, 1
loc_1_4BD2:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_4BD9:
        imul   edx, edx, 0x16
        movzx  ecx, word ptr [edx + 0x10]    ; -> loc_1_0CA8
        movzx  eax, byte ptr [ecx + esi]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_0CAA
        add    esp, 8
        jmp    loc_1_4BD2
sub_1_4BF6:
        push   ebx
        xor    edx, edx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0xae2], 0    ; -> loc_1_4C56
        mov    dword ptr [esi + 0xae6], 0    ; -> loc_1_4E7C
        mov    dword ptr [esi + 0xaea], 0    ; -> loc_1_4F86
        lea    ecx, [esi + 0xeee]
loc_1_4C22:
        mov    word ptr [ecx], 0
        xor    eax, eax
loc_1_4C29:
        mov    byte ptr [eax + ecx + 0x3e], 0
        lea    ebx, [eax + edx]
        inc    eax
        mov    word ptr [esi + ebx*2 + 0xef0], 0x2000
        cmp    eax, 0x1e
        jl     loc_1_4C29
        add    edx, 0x2e
        add    ecx, 0x5c
        cmp    edx, 0x2e0
        jl     loc_1_4C22
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    4
loc_1_4C56:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        test   byte ptr [ebp + 0xd], 0x80
        push   ebx
        push   esi
        push   edi
        je     loc_1_4CD2
        and    dword ptr [ebp + 0xc], 0x1f
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        imul   eax, eax, 0x5c
        mov    dword ptr [ebp - 4], 0        ; -> loc_1_0CC8
        lea    edi, [eax + esi + 0xef0]
        lea    ebx, [eax + esi + 0xeee]
        xor    esi, esi
loc_1_4C89:
        mov    al, byte ptr [ebx + esi + 0x3e]
        test   al, al
        je     loc_1_4CBC
        cmp    al, 1
        jne    loc_1_4CBC
        push   4
        mov    edx, dword ptr [ebp - 4]
        movsx  eax, word ptr [edi]
        movzx  ecx, byte ptr [edx]
        sub    eax, 0x2000
        push   eax
        push   ecx
        call   sub_4_2758
loc_1_4CAC:
        mov    ecx, dword ptr [ebp - 4]
        movzx  ecx, byte ptr [ecx]
        add    esp, 0xc
        mov    edx, dword ptr [ebp + 0x10]
        mov    word ptr [edx + ecx*2], ax
loc_1_4CBC:
        add    edi, 2
        inc    esi
        add    dword ptr [ebp - 4], 6
        cmp    dword ptr [ebp - 4], 0xb4     ; -> loc_1_0D7C
        jb     loc_1_4C89
        jmp    loc_1_4E73
loc_1_4CD2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        shl    eax, 5
        xor    ebx, ebx
        lea    edi, [eax + esi + 0x442]
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x5c
        lea    edx, [eax + esi + 0xeee]
        mov    ax, word ptr [edi + 0x1c]
        mov    bx, ax
        and    ebx, 0x7f
        cmp    ebx, 0x1e
        jb     loc_1_4D0A
        mov    eax, 1
        jmp    loc_1_4E75
loc_1_4D0A:
        mov    cx, word ptr [edi + 0x1e]
        shr    ax, 8
        movzx  eax, ax
        cmp    eax, 0x7f
        je     loc_1_4D24
        mov    eax, 1
        jmp    loc_1_4E75
loc_1_4D24:
        mov    word ptr [edx], 1
        mov    byte ptr [ebx + edx + 0x3e], 1
        mov    word ptr [edx + ebx*2 + 2], cx
        imul   ebx, ebx, 6
        cmp    byte ptr [ebx + 1], 0         ; -> obj1:0x0CC9
        je     loc_1_4E75
        movzx  eax, byte ptr [ebx]           ; -> loc_1_0CC8
        sub    cx, 0x2000
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, 4
        jne    loc_1_4D6F
        movsx  eax, cx
        shl    eax, 0xc
        mov    ecx, 0x4b0
        cdq    
        idiv   ecx
        mov    word ptr [ebp - 2], ax
        mov    word ptr [edi + 0xe], ax
        jmp    loc_1_4DC4
loc_1_4D6F:
        cmp    dword ptr [ebp - 0xc], 5
        movsx  eax, cx
        push   4
        push   eax
        jne    loc_1_4DA7
        push   5
        call   sub_4_2758
loc_1_4D82:
        movzx  cx, byte ptr [edi + 0xa]
        movzx  dx, byte ptr [edi + 0xb]
        add    cx, dx
        add    esp, 0xc
        add    ax, cx
        mov    word ptr [ebp - 2], ax
        cmp    ax, 0x7f
        jle    loc_1_4DBF
        mov    word ptr [ebp - 2], 0x7f
        jmp    loc_1_4DBF
loc_1_4DA7:
        push   dword ptr [ebp - 0xc]
        call   sub_4_2758
loc_1_4DAF:
        mov    word ptr [ebp - 2], ax
        add    esp, 0xc
        cmp    byte ptr [ebx + 4], 0         ; -> obj1:0x0CCC
        je     loc_1_4DC4
loc_1_4DBF:
        shl    word ptr [ebp - 2], 8
loc_1_4DC4:
        lea    eax, [esi + 0x42]
        mov    dword ptr [ebp - 0x10], eax
        xor    eax, eax
        cmp    word ptr [esi + 6], ax
        mov    dword ptr [ebp - 8], eax
        jbe    loc_1_4E73
loc_1_4DD9:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        shr    ax, 8
        movzx  eax, ax
        cmp    eax, 0xff
        je     loc_1_4E5F
        cmp    eax, dword ptr [ebp + 0xc]
        jne    loc_1_4E5F
        cmp    dword ptr [ebp - 0xc], 4
        jne    loc_1_4E1D
        mov    eax, dword ptr [ebp - 0x10]
        movsx  ecx, word ptr [ebp - 2]
        movzx  eax, word ptr [eax + 0xe]
        add    eax, dword ptr [edi + 0x10]
        add    eax, ecx
        cmp    eax, 0xffff
        jle    loc_1_4E14
        mov    eax, 0xffff
loc_1_4E14:
        push   eax
        mov    eax, dword ptr [ebp - 8]
        or     ah, 4
        jmp    loc_1_4E58
loc_1_4E1D:
        cmp    byte ptr [ebx + 4], 0         ; -> obj1:0x0CCC
        movzx  eax, word ptr [ebx + 2]       ; -> obj1:0x0CCA
        je     loc_1_4E3C
        or     eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_1_19DF
        mov    byte ptr [ebp - 2], al
        jmp    loc_1_4E49
loc_1_4E3C:
        or     eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_1_19DF
        mov    byte ptr [ebp - 1], ah
loc_1_4E49:
        movsx  eax, word ptr [ebp - 2]
        push   eax
        movzx  eax, word ptr [ebx + 2]       ; -> obj1:0x0CCA
        or     eax, dword ptr [ebp - 8]
loc_1_4E58:
        push   eax
        push   esi
        call   sub_1_19AA
loc_1_4E5F:
        add    dword ptr [ebp - 0x10], 0x20
        movzx  eax, word ptr [esi + 6]
        inc    dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 8]
        ja     loc_1_4DD9
loc_1_4E73:
        xor    eax, eax
loc_1_4E75:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_4E7C:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [eax + edx]
        mov    word ptr [esi + 0x45a], 0x200
        add    esi, 0x45e
        movzx  ax, byte ptr [ebp + 0x10]
        mov    cx, word ptr [esi]
        and    cx, 0x7f00
        or     ax, cx
        mov    word ptr [esi], cx
        movzx  ecx, ax
        mov    word ptr [esi], ax
        push   ecx
        push   dword ptr [ebp + 0xc]
        push   edx
        call   sub_1_4EC2
        pop    esi
        pop    ebp
        ret    
sub_1_4EC2:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    bl, ch
        test   bl, bl
        jne    loc_1_4F25
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0xc]
        imul   esi, esi, 0x5c
        imul   edi, edi, 0x2e
        mov    word ptr [edx + esi + 0xeee], 0
        shl    eax, 5
        mov    dword ptr [ebp - 4], edi
        mov    word ptr [eax + edx + 0x450], 0
        xor    eax, eax
loc_1_4F03:
        lea    edi, [esi + eax]
        mov    byte ptr [edi + edx + 0xf2c], 0
        mov    edi, dword ptr [ebp - 4]
        add    edi, eax
        inc    eax
        mov    word ptr [edx + edi*2 + 0xef0], 0x2000
        cmp    eax, 0x1e
        jb     loc_1_4F03
        jmp    loc_1_4F28
loc_1_4F25:
        mov    edx, dword ptr [ebp + 8]
loc_1_4F28:
        cmp    bl, 0x7f
        jne    loc_1_4F78
        and    ecx, 0x7f
        cmp    cl, 0x1e
        jae    loc_1_4F78
        mov    si, 0x2000
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x5c
        cmp    word ptr [edx + eax + 0xeee], 0
        je     loc_1_4F66
        add    eax, ecx
        cmp    byte ptr [eax + edx + 0xf2c], 1
        jne    loc_1_4F66
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x2e
        add    eax, ecx
        mov    si, word ptr [edx + eax*2 + 0xef0]
loc_1_4F66:
        mov    eax, dword ptr [ebp + 0xc]
        shl    eax, 5
        mov    word ptr [eax + edx + 0x460], si
        xor    eax, eax
        jmp    loc_1_4F7D
loc_1_4F78:
        mov    eax, 1
loc_1_4F7D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_1_4F86:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        shl    eax, 5
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [eax + edx]
        mov    eax, dword ptr [ebp + 0x10]
        shl    ax, 8
        add    esi, 0x45e
        mov    word ptr [esi - 4], 0x200
        mov    cx, word ptr [esi]
        and    cx, 0xff
        or     ax, cx
        mov    word ptr [esi], cx
        movzx  ecx, ax
        mov    word ptr [esi], ax
        push   ecx
        push   dword ptr [ebp + 0xc]
        push   edx
        call   sub_1_4EC2
        pop    esi
        pop    ebp
        ret    
sub_1_4FCB:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    edx, 0x17f
        mov    esi, dword ptr [esp + 0x14]
        movsx  edi, word ptr [ecx + 0x22]
        movzx  eax, byte ptr [esi + 7]
        add    eax, edi
        add    eax, eax
        sub    edx, eax
        cmp    edx, 0xfe
        jl     loc_1_4FF7
        mov    edx, 0xff
        jmp    loc_1_4FFE
loc_1_4FF7:
        cmp    edx, 1
        jg     loc_1_4FFE
        xor    edx, edx
loc_1_4FFE:
        movzx  ax, dl
        mov    word ptr [ecx + 0x22], ax
        test   edx, edx
        je     loc_1_5016
        neg    dl
        movzx  ax, dl
        mov    word ptr [ecx + 0x24], ax
        jmp    loc_1_501C
loc_1_5016:
        mov    word ptr [ecx + 0x24], 0xff
loc_1_501C:
        lea    edi, [ecx + 0x1e]
        movzx  eax, byte ptr [esi + 6]
        movsx  edx, word ptr [edi]
        add    eax, edx
        mov    edx, 0xff
        mov    word ptr [edi], dx
        cmp    eax, edx
        jg     loc_1_5037
        mov    word ptr [edi], ax
loc_1_5037:
        add    ecx, 0x20
        movzx  eax, byte ptr [esi + 5]
        movsx  edx, word ptr [ecx]
        add    eax, edx
        mov    edx, 0xff
        mov    word ptr [ecx], dx
        cmp    eax, edx
        jg     loc_1_5052
        mov    word ptr [ecx], ax
loc_1_5052:
        pop    edi
        pop    esi
        ret    
sub_1_5055:
        mov    edx, dword ptr [esp + 4]
        push   esi
        test   edx, edx
        jne    loc_1_5064
        mov    ax, 0x7f
        jmp    loc_1_5085
loc_1_5064:
        mov    eax, 1
        mov    ecx, 2                        ; -> loc_1_0D82
loc_1_506E:
        movzx  esi, word ptr [ecx]
        cmp    esi, edx
        jb     loc_1_5085
        add    ecx, 2
        inc    eax
        cmp    ecx, 0xfe                     ; -> obj1:0x0E7E
        jb     loc_1_506E
        mov    ax, 0x7e
loc_1_5085:
        pop    esi
        ret    
loc_1_5087:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    word ptr [esi + 0x2a], 0x1e
        cmp    dword ptr [esi + 0x154e], 0
        jne    loc_1_50A2
        mov    word ptr [esi + 0x2a], 0x20
loc_1_50A2:
        lea    edi, [esi + 0x24]
        push   edi
        call   sub_3_173B
loc_1_50AB:
        push   0x11
        push   edi
        call   sub_1_18E9
        mov    al, 0xa0
        pop    edi
        movzx  edx, word ptr [esi + 0x28]
        add    edx, 2
        pop    esi
        out    dx, al
        xor    eax, eax
        ret    
sub_1_50C2:
        mov    eax, dword ptr [esp + 4]
        push   0
        add    eax, 0x24
        push   eax
        call   sub_3_153C
loc_1_50D1:
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret    
sub_1_50D8:
        push   ebx
        push   esi
        push   edi
        mov    esi, 0x120
        mov    edi, dword ptr [esp + 0x10]
        push   ebp
        xor    ebp, ebp
        lea    ebx, [edi + 0x686]
loc_1_50ED:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     loc_1_50FE
        push   eax
        call   sub_4_0D91
loc_1_50F9:
        add    esp, 4
        mov    dword ptr [ebx], ebp
loc_1_50FE:
        add    ebx, 4
        dec    esi
        jne    loc_1_50ED
        mov    al, 0xa0
        pop    ebp
        movzx  edx, word ptr [edi + 0x28]
        add    edx, 2
        pop    edi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    
sub_1_5115:
        xor    eax, eax
        ret    
sub_1_5118:
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [esp + 0x10]
        cmp    word ptr [ebx + 0x2a], si
        jbe    loc_1_515E
        lea    edi, [ebx + 0x66]
loc_1_512A:
        cmp    word ptr [edi], 0x2000
        jae    loc_1_5152
        push   0x807f
        mov    eax, esi
        or     eax, 0x2a0
        lea    ecx, [ebx + 0x24]
        push   eax
        push   ecx
        call   sub_1_19AA
        mov    word ptr [edi], 0xffe
        mov    word ptr [edi + 0x1a], 0
loc_1_5152:
        add    edi, 0x20
        inc    esi
        movzx  eax, word ptr [ebx + 0x2a]
        cmp    eax, esi
        jg     loc_1_512A
loc_1_515E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_5164:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        lea    edi, [esi + 0x24]
        push   edi
        call   sub_4_2380
loc_1_5173:
        add    esp, 4
        push   9
        push   5
        push   edi
        call   sub_4_2458
loc_1_5180:
        add    esp, 0xc
        push   0x327f
        push   0x373
        push   edi
        call   sub_1_19AA
        push   0x337f
        push   0x37b
        push   edi
        call   sub_1_19AA
        push   edi
        call   sub_4_106D
loc_1_51A9:
        push   -1
        push   0x7e
        push   edi
        call   sub_1_1A14
        push   -1
        push   0x7f
        push   edi
        call   sub_1_1A14
        push   4
        push   0x23f
        push   edi
        call   sub_1_1942
        mov    al, 0xa0
        pop    edi
        movzx  edx, word ptr [esi + 0x28]
        add    edx, 2
        pop    esi
        out    dx, al
        xor    eax, eax
        ret    
sub_1_51D9:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        xor    edi, edi
        lea    ebx, [esi + 0x7d]
        mov    ebp, 0x1d8
        mov    dword ptr [esi + 0x3c], 0x20
        add    esi, 0x24
loc_1_51F5:
        push   0x80
        mov    eax, edi
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19AA
        push   ebp
        mov    eax, edi
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
        push   0x1e0
        mov    eax, edi
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
        push   ebp
        mov    eax, edi
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
        push   0xffff
        mov    eax, edi
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A14
        push   0xffff
        mov    eax, edi
        or     al, 0x40
        push   eax
        push   esi
        call   sub_1_1A14
        push   0x40000000
        mov    eax, edi
        or     al, 0x20
        inc    edi
        push   eax
        push   esi
        call   sub_1_1A14
        mov    byte ptr [ebx], 1
        add    ebx, 0x20
        cmp    edi, 0x20
        jb     loc_1_51F5
        push   0
        push   0x23f
        push   esi
        call   sub_1_1942
        push   esi
        call   sub_3_160C
loc_1_5280:
        movzx  eax, word ptr [esi + 0x12]
        test   eax, eax
        jne    loc_1_52C0
        push   esi
        call   sub_4_2380
loc_1_528E:
        add    esp, 4
        movsx  eax, byte ptr [esi + 0x14]
        push   eax
        push   esi
        call   sub_4_264C
loc_1_529C:
        add    esp, 8
        movsx  ecx, byte ptr [esi + 0x15]
        push   ecx
        push   esi
        call   sub_4_25AA
loc_1_52AA:
        movsx  ecx, byte ptr [esi + 0x17]
        add    esp, 8
        movsx  eax, byte ptr [esi + 0x16]
        push   ecx
        push   eax
        push   esi
        call   sub_4_2458
loc_1_52BD:
        add    esp, 0xc
loc_1_52C0:
        push   0x59
        push   0x23d
        push   esi
        call   sub_1_1942
        push   0x20
        push   0x23e
        push   esi
        call   sub_1_1942
        push   6
        push   0x23f
        push   esi
        call   sub_1_1942
        cmp    word ptr [esi + 6], 0x1e
        jne    loc_1_52F4
        push   esi
        call   sub_4_106D
loc_1_52F4:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_52FB:
        push   ebp
        movzx  eax, word ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 0xc
        push   ebx
        push   esi
        push   edi
        ja     loc_1_5315
        jmp    dword ptr [eax*4]             ; -> obj1:0x5508
loc_1_5315:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_42A7
loc_1_5326:
        add    esp, 0x10
        jmp    loc_1_5501
loc_1_532E:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 0x10
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_3C92
        jmp    loc_1_54FF
loc_1_5351:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 0x10
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_3DB3
        jmp    loc_1_54FF
loc_1_5374:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 0x10
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_4A99
        jmp    loc_1_54FF
loc_1_5397:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 0x10
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_42A5
        jmp    loc_1_54FF
loc_1_53BA:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [esi]
        movzx  eax, ch
        movzx  edx, cl
        push   eax
        push   edx
        push   dword ptr [ebp + 0x10]
        call   sub_1_488D
        jmp    loc_1_54FF
loc_1_53D4:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [esi]
        movzx  eax, ch
        movzx  edx, cl
        push   eax
        push   edx
        push   dword ptr [ebp + 0x10]
        call   sub_1_4A9E
        jmp    loc_1_54FF
loc_1_53EE:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [esi]
        mov    eax, ecx
        shr    eax, 0x10
        movzx  ebx, ch
        movzx  edx, al
        movzx  eax, cl
        push   edx
        push   ebx
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_4AD6
        jmp    loc_1_54FF
loc_1_5411:
        mov    esi, dword ptr [ebp + 0x14]
        push   dword ptr [esi]
        add    esi, 8
        push   esi
        push   dword ptr [ebp + 0x10]
        call   sub_1_4B8B
        jmp    loc_1_54FF
loc_1_5427:
        push   0x10
        push   dword ptr [ebp + 0x10]
        call   sub_1_18E9
        push   dword ptr [ebp + 0x10]
        call   sub_1_4BF6
        jmp    loc_1_54FF
loc_1_543E:
        xor    edi, edi
        mov    ecx, dword ptr [ebp + 0x10]
        add    ecx, 6
        cmp    word ptr [ecx], di
        mov    dword ptr [ebp - 4], ecx
        jbe    loc_1_54FF
        mov    esi, dword ptr [ebp + 0x10]
        add    esi, 0x42
loc_1_5458:
        push   0x807f
        mov    eax, edi
        or     eax, 0x2a0
        inc    edi
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_1_19AA
        mov    word ptr [esi], 0xffe
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0x1a], 0
        movzx  ecx, word ptr [eax]
        add    esi, 0x20
        cmp    ecx, edi
        jg     loc_1_5458
        jmp    loc_1_54FF
loc_1_5488:
        mov    esi, dword ptr [ebp + 0x14]
        movzx  ecx, word ptr [esi]
        lea    eax, [esi + 2]
        push   eax
        push   ecx
        push   dword ptr [ebp + 0x10]
        call   sub_1_3F5F
        jmp    loc_1_54FF
loc_1_549D:
        mov    esi, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi]
        sub    eax, 2
        cdq    
        test   al, 1
        je     loc_1_54BF
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 0x10]
        sar    eax, 1
        movzx  edx, word ptr [ecx + eax*2]
        add    edx, 2
        in     ax, dx
        movzx  eax, ax
        jmp    loc_1_5501
loc_1_54BF:
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 0x10]
        sar    eax, 1
        mov    dx, word ptr [ecx + eax*2]
        in     ax, dx
        movzx  eax, ax
        jmp    loc_1_5501
loc_1_54D1:
        mov    esi, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi]
        sub    eax, 2
        cdq    
        test   al, 1
        je     loc_1_54EE
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 0x10]
        sar    eax, 1
        movzx  edx, word ptr [ecx + eax*2]
        add    edx, 2
        jmp    loc_1_54F9
loc_1_54EE:
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 0x10]
        sar    eax, 1
        mov    dx, word ptr [ecx + eax*2]
loc_1_54F9:
        mov    ax, word ptr [esi + 4]
        out    dx, ax
loc_1_54FF:
        xor    eax, eax
loc_1_5501:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        ; ---- dispatch table (13 entries) ----
        dd  loc_1_532E
        dd  loc_1_5351
        dd  loc_1_5374
        dd  loc_1_5397
        dd  loc_1_53BA
        dd  loc_1_53D4
        dd  loc_1_53EE
        dd  loc_1_5411
        dd  loc_1_5427
        dd  loc_1_543E
        dd  loc_1_5488
        dd  loc_1_549D
        dd  loc_1_54D1
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x37  R-X
        db  001h, 000h, 000h, 000h
        dd  loc_2_0010               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h, 027h, 000h
loc_2_0010:
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    0x82
        inc    edx
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x8e
        jb     0x3d
        inc    ecx
        push   edi
        inc    ebp
        xor    esi, dword ptr [edx]
        and    byte ptr [ecx + ebp*2 + 0x72], al
        arpl   word ptr gs:[eax + 0x53], si
        outsd  dx, dword ptr [esi]
        jne    0x9c
        and    byte ptr fs:[edx + esi*2 + 0x69], al
        jbe    0x9a
        jb     0x37
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x1d26  R-X
        db  083h, 0ECh, 018h, 033h, 0C9h, 053h, 056h, 08Bh, 074h, 024h, 024h, 057h, 055h, 08Dh, 056h, 024h
        db  066h, 08Bh, 03Ah, 033h, 0C0h, 08Dh, 01Ch, 007h, 08Dh, 02Ch, 001h, 066h, 089h, 05Ch, 06Ch, 010h
        db  040h, 083h, 0F8h, 004h, 07Ch, 0EFh, 083h, 0C2h, 002h, 083h, 0C1h, 004h, 083h, 0F9h, 00Ch, 07Ch
        db  0DFh, 056h, 08Dh, 046h, 00Ch, 08Dh, 04Ch, 024h, 014h, 050h, 068h
        dd  loc_1_1278               ; fixup
        db  051h, 06Ah, 00Ch, 0E8h
        dd  sub_4_0E11               ; fixup
        db  083h, 0C4h, 014h, 08Bh, 0F8h, 085h, 0FFh, 075h, 009h, 056h, 0E8h
        dd  loc_1_5087               ; fixup
        db  083h, 0C4h, 004h, 08Bh, 0C7h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 018h, 0C3h
sub_3_0063:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> obj1:0x0418
        push   ebp
        xor    edi, edi
        cmp    esi, edi
        je     loc_3_0088
        mov    eax, dword ptr [esp + 0x2c]
loc_3_007A:
        cmp    dword ptr [esi + 8], eax
        je     loc_3_0092
        mov    edi, esi
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    loc_3_007A
loc_3_0088:
        xor    eax, eax
loc_3_008A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
loc_3_0092:
        lea    ecx, [esi + 0x24]
        xor    eax, eax
loc_3_0097:
        xor    edx, edx
loc_3_0099:
        mov    bx, word ptr [ecx]
        lea    ebp, [edx + eax]
        add    bx, dx
        inc    edx
        mov    word ptr [esp + ebp*2 + 0x10], bx
        cmp    edx, 4
        jl     loc_3_0099
        add    ecx, 2
        add    eax, 4
        cmp    eax, 0xc
        jl     loc_3_0097
        lea    eax, [esp + 0x10]
        push   eax
        push   0xc
        call   sub_4_0DAB
loc_3_00C4:
        add    esp, 8
        push   esi
        call   sub_1_50D8
loc_3_00CD:
        add    esp, 4
        mov    eax, dword ptr [esi + 4]
        test   edi, edi
        je     loc_3_00DC
        mov    dword ptr [edi + 4], eax
        jmp    loc_3_00E1
loc_3_00DC:
        mov    dword ptr [0], eax            ; -> obj1:0x0418
loc_3_00E1:
        mov    eax, esi
        jmp    loc_3_008A
sub_3_00E5:
        push   dword ptr [esp + 4]
        call   sub_1_50C2
loc_3_00EE:
        add    esp, 4
        ret    
sub_3_00F2:
        mov    ecx, dword ptr [0]            ; -> obj1:0x0418
        test   ecx, ecx
        je     loc_3_010C
        mov    eax, dword ptr [esp + 4]
loc_3_0100:
        cmp    dword ptr [ecx + 8], eax
        je     loc_3_0112
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    loc_3_0100
loc_3_010C:
        mov    eax, 1
loc_3_0111:
        ret    
loc_3_0112:
        cmp    dword ptr [ecx], 1
        sbb    eax, eax
        inc    eax
        jmp    loc_3_0111
        db  0CCh, 0CCh
loc_3_011C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, esp
        push   0
        push   ebx
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003003300
        add    esp, 0xc
        cmp    eax, 0xd
        je     loc_3_016E
        cmp    eax, 0
        jne    loc_3_0180
        push   ecx
        mov    ecx, dword ptr [esp + 4]
        push   edi
        mov    edi, dword ptr [0]            ; -> obj1:0x0418
loc_3_0147:
        cmp    edi, 0
        je     loc_3_016C
        cmp    dword ptr [edi + 8], ecx
        je     loc_3_0156
        mov    edi, dword ptr [edi + 4]
        jmp    loc_3_0147
loc_3_0156:
        push   3
        push   edi
        push   0                             ; -> loc_3_1045
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e003300
        add    esp, 0x10
        pop    edi
        pop    ecx
        jmp    loc_3_0180
loc_3_016C:
        pop    edi
        pop    ecx
loc_3_016E:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_0F5E
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
loc_3_0180:
        add    esp, 4
        pop    ebp
        stc    
        ret    
sub_3_0186:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [0]            ; -> obj1:0x0418
        test   esi, esi
        je     loc_3_01D9
loc_3_0194:
        mov    ecx, dword ptr [esi + 8]
        xor    eax, eax
loc_3_0199:
        mov    edx, ecx
        inc    eax
        shr    edx, 0x1c
        shl    ecx, 4
        mov    dl, byte ptr [edx]            ; -> obj5:0x00D8
        mov    byte ptr [eax + 0x13], dl     ; -> obj5:0x01BB
        cmp    eax, 8
        jl     loc_3_0199
        lea    eax, [esp + 4]
        push   eax
        push   8
        push   0                             ; -> obj5:0x0128
        push   0                             ; -> obj5:0x0110
        push   esi
        push   0                             ; -> obj5:0x01A8
        call   sub_4_4DAC
loc_3_01CF:
        add    esp, 0x18
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    loc_3_0194
loc_3_01D9:
        pop    esi
        add    esp, 4
        ret    
loc_3_01DE:
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax]
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 0xc]
        push   edi
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 0x10], 0
        mov    dword ptr [eax], 0
        je     loc_3_02E5
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x14]
loc_3_0210:
        mov    eax, dword ptr [ebx + 4]
        shl    eax, 4
        mov    eax, dword ptr [eax + 4]      ; -> obj5:0x012C
        mov    dword ptr [ebx], eax
        cmp    dword ptr [ebp - 8], eax
        jb     loc_3_02CB
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, 7
        ja     loc_3_0236
        jmp    dword ptr [eax*4]             ; -> obj3:0x02EF
loc_3_0236:
        mov    dword ptr [edi], 0
        jmp    loc_3_02B8
loc_3_023E:
        mov    eax, dword ptr [esi + 0x1558]
        jmp    loc_3_02B6
loc_3_0246:
        cmp    dword ptr [esi + 0x1558], 0x10
        jne    loc_3_0258
        movzx  eax, byte ptr [esi + 0x155c]
        jmp    loc_3_02B6
loc_3_0258:
        mov    dword ptr [edi], 0xffffffff
        jmp    loc_3_02B8
loc_3_0260:
        cmp    dword ptr [esi + 0x1558], 0x10
        jne    loc_3_0272
        movzx  eax, byte ptr [esi + 0x155d]
        jmp    loc_3_02B6
loc_3_0272:
        mov    dword ptr [edi], 0xffffffff
        jmp    loc_3_02B8
loc_3_027A:
        movzx  eax, word ptr [esi + 0x24]
        movzx  edx, word ptr [esi + 0x26]
        mov    ecx, eax
        sub    ecx, edx
        cmp    ecx, -4
        jne    loc_3_02B6
        xor    eax, eax
        mov    ax, word ptr [esi + 0x24]
        and    eax, 0xfff8
        jmp    loc_3_02B6
loc_3_0298:
        mov    eax, dword ptr [esi + 0x154e]
        jmp    loc_3_02B6
loc_3_02A0:
        mov    eax, dword ptr [esi + 0x2c]
        jmp    loc_3_02B6
loc_3_02A5:
        mov    eax, dword ptr [esi + 0x30]
        jmp    loc_3_02B6
loc_3_02AA:
        lea    eax, [esi + 0x24]
        push   eax
        call   sub_4_46CD
loc_3_02B3:
        add    esp, 4
loc_3_02B6:
        mov    dword ptr [edi], eax
loc_3_02B8:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebp + 0x18]
        add    dword ptr [eax], ecx
        mov    dword ptr [ebx + 8], edi
        mov    eax, dword ptr [ebx]
        add    edi, eax
        sub    dword ptr [ebp - 8], eax
        jmp    loc_3_02D9
loc_3_02CB:
        mov    dword ptr [ebx + 8], 0
        mov    dword ptr [ebp - 4], 0xea
loc_3_02D9:
        add    ebx, 0xc
        dec    dword ptr [ebp + 0x10]
        jne    loc_3_0210
loc_3_02E5:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        ; ---- dispatch table (8 entries) ----
        dd  loc_3_023E
        dd  loc_3_0246
        dd  loc_3_0260
        dd  loc_3_027A
        dd  loc_3_0298
        dd  loc_3_02A0
        dd  loc_3_02A5
        dd  loc_3_02AA
sub_3_030F:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        xor    edi, edi
        cmp    byte ptr [esi], 0
        je     loc_3_03A4
        xor    edx, edx
        cmp    byte ptr [esi + edi], dl
        je     loc_3_0335
loc_3_0328:
        cmp    byte ptr [esi + edi], 0x20
        jne    loc_3_0335
        inc    edi
        cmp    byte ptr [esi + edi], 0
        jne    loc_3_0328
loc_3_0335:
        mov    cl, byte ptr [esi + edi]
        cmp    byte ptr [esp + 0x10], cl
        je     loc_3_0360
        movsx  eax, cl
        movsx  ebx, byte ptr [esp + 0x10]
        sub    eax, ebx
        cmp    eax, 0x20
        je     loc_3_0360
        test   cl, cl
        je     loc_3_039F
loc_3_0351:
        cmp    byte ptr [esi + edi], 0x20
        je     loc_3_039F
        inc    edi
        cmp    byte ptr [esi + edi], 0
        jne    loc_3_0351
        jmp    loc_3_039F
loc_3_0360:
        mov    ebx, edi
        mov    eax, edi
        test   cl, cl
        je     loc_3_0375
loc_3_0368:
        cmp    byte ptr [esi + eax], 0x20
        je     loc_3_0375
        inc    eax
        cmp    byte ptr [esi + eax], 0
        jne    loc_3_0368
loc_3_0375:
        cmp    byte ptr [esi + eax], dl
        je     loc_3_039A
loc_3_037A:
        cmp    byte ptr [esi + eax], 0x20
        jne    loc_3_0387
        inc    eax
        cmp    byte ptr [esi + eax], 0
        jne    loc_3_037A
loc_3_0387:
        cmp    byte ptr [esi + eax], dl
        je     loc_3_039A
loc_3_038C:
        mov    cl, byte ptr [esi + eax]
        inc    eax
        mov    byte ptr [esi + edi], cl
        inc    edi
        cmp    byte ptr [esi + eax], 0
        jne    loc_3_038C
loc_3_039A:
        mov    byte ptr [esi + edi], dl
        mov    edi, ebx
loc_3_039F:
        cmp    byte ptr [esi + edi], dl
        jne    loc_3_0328
loc_3_03A4:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_03A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x64
        push   esi
        lea    eax, [ebp - 0x64]
        mov    dword ptr [ebp - 0x14], 0x50
        mov    dword ptr [ebp - 0xc], 0      ; -> obj5:0x00D0
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0003h              ; cd2003004a04
        add    esp, 0xc
        cmp    byte ptr [ebp - 0x64], 0
        je     loc_3_03F0
        lea    eax, [ebp - 0x64]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_3_030F
        add    esp, 8
loc_3_03F0:
        xor    esi, esi
        cmp    byte ptr [ebp - 0x64], 0
        je     loc_3_0400
loc_3_03F8:
        inc    esi
        cmp    byte ptr [ebp + esi - 0x64], 0
        jne    loc_3_03F8
loc_3_0400:
        test   esi, esi
        je     loc_3_0416
        cmp    byte ptr [ebp + esi - 0x65], 0x20
        jne    loc_3_040C
        dec    esi
loc_3_040C:
        test   esi, esi
        je     loc_3_0416
        mov    byte ptr [ebp + esi - 0x64], 0x20
        inc    esi
loc_3_0416:
        mov    al, byte ptr [ebp + 8]
        inc    esi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ebp + esi - 0x65], al
        xor    eax, eax
loc_3_0423:
        test   ecx, 0xf0000000
        jne    loc_3_0434
        shl    ecx, 4
        inc    eax
        cmp    eax, 8
        jl     loc_3_0423
loc_3_0434:
        cmp    eax, 8
        jge    loc_3_0456
        mov    edx, 8
        sub    edx, eax
loc_3_0440:
        mov    eax, ecx
        inc    esi
        shr    eax, 0x1c
        shl    ecx, 4
        mov    al, byte ptr [eax]            ; -> obj5:0x00D8
        dec    edx
        mov    byte ptr [ebp + esi - 0x65], al
        jne    loc_3_0440
loc_3_0456:
        mov    byte ptr [ebp + esi - 0x64], 0
        mov    dword ptr [ebp - 4], 0        ; -> obj5:0x00D0
        lea    eax, [ebp - 0x64]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0482:
        push   ebp
        mov    ecx, dword ptr [0]            ; -> obj1:0x0418
        mov    ebp, esp
        sub    esp, 0x64
        test   ecx, ecx
        je     loc_3_052C
        mov    eax, dword ptr [ebp + 8]
loc_3_0499:
        cmp    dword ptr [ecx + 8], eax
        je     loc_3_04AA
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    loc_3_0499
        jmp    loc_3_052C
loc_3_04AA:
        mov    eax, dword ptr [ecx + 4]
        test   eax, eax
        je     loc_3_04CA
        xor    ecx, ecx
        mov    cx, word ptr [eax + 0x24]
        and    ecx, 0xfff8
        push   ecx
        push   0x45
        call   sub_3_03A8
        add    esp, 8
        jmp    loc_3_052C
loc_3_04CA:
        mov    dword ptr [ebp - 0x14], 0x50
        mov    dword ptr [ebp - 0xc], 0      ; -> obj5:0x00D0
        lea    eax, [ebp - 0x64]
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0003h              ; cd2003004a04
        add    esp, 0xc
        cmp    byte ptr [ebp - 0x64], 0
        je     loc_3_052C
        lea    eax, [ebp - 0x64]
        push   eax
        push   0x45
        call   sub_3_030F
        add    esp, 8
        lea    eax, [ebp - 0x64]
        mov    dword ptr [ebp - 4], 0        ; -> obj5:0x00D0
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_3_052C:
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0530:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x14]
        mov    dword ptr [ebp - 4], 0x10
        mov    byte ptr [ebp - 0x14], 0
        push   ecx
        push   1
        push   0                             ; -> obj5:0x01C8
        push   0
        push   dword ptr [ebp + 8]
        call   sub_4_4DB4
loc_3_055C:
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_0592
        cmp    dword ptr [ebp - 4], 4
        jb     loc_3_0592
        cmp    byte ptr [ebp - 0x14], 0x73
        je     loc_3_0575
        cmp    byte ptr [ebp - 0x14], 0x53
        jne    loc_3_0592
loc_3_0575:
        cmp    byte ptr [ebp - 0x13], 0x62
        je     loc_3_0581
        cmp    byte ptr [ebp - 0x13], 0x42
        jne    loc_3_0592
loc_3_0581:
        cmp    byte ptr [ebp - 0x12], 0x31
        jne    loc_3_0592
        cmp    byte ptr [ebp - 0x11], 0x36
        mov    eax, 1
        je     loc_3_0594
loc_3_0592:
        xor    eax, eax
loc_3_0594:
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0598:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        lea    eax, [ebp - 0x10]
        push   esi
        lea    ecx, [ebp - 0x20]
        push   edi
        lea    edx, [ebp - 0xc]
        push   0
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x20], 4
        push   eax
        push   ecx
        push   edx
        push   edi
        call   sub_4_4DDC
loc_3_05BF:
        add    esp, 0x14
        cmp    dword ptr [ebp - 0xc], 0x10
        je     loc_3_05E1
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_3_05E1
        cmp    dword ptr [ebp - 0xc], 1
        je     loc_3_05E1
        cmp    dword ptr [ebp - 0xc], 2
        je     loc_3_05E1
        xor    eax, eax
        jmp    loc_3_0806
loc_3_05E1:
        cmp    dword ptr [ebp - 0xc], 0x10
        mov    si, 0xffff
        jne    loc_3_06B6
        cmp    byte ptr [ebp - 0xf], 0
        jne    loc_3_05FC
        xor    eax, eax
        jmp    loc_3_0806
loc_3_05FC:
        mov    bl, byte ptr [ebp - 0x10]
        push   0
        push   edi
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_4_4DCC
loc_3_060B:
        add    esp, 0xc
        lea    ecx, [ebp - 8]
        push   0
        push   dword ptr [ebp - 0x1c]
        push   ecx
        call   sub_4_4DE4
loc_3_061C:
        add    esp, 0xc
loc_3_061F:
        cmp    dword ptr [ebp - 8], edi
        je     loc_3_069B
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        lea    ecx, [ebp - 0x20]
        push   ecx
        lea    edx, [ebp - 0xc]
        mov    byte ptr [ebp - 0x10], 0xff
        mov    dword ptr [ebp - 0x20], 4
        push   edx
        push   dword ptr [ebp - 8]
        call   sub_4_4DDC
loc_3_0645:
        add    esp, 0x14
        cmp    byte ptr [ebp - 0x10], bl
        jne    loc_3_069B
        push   dword ptr [ebp - 8]
        call   sub_3_0530
        add    esp, 4
        test   eax, eax
        je     loc_3_069B
        push   2
        lea    ecx, [ebp - 0x18]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_4_4DC4
loc_3_066A:
        add    esp, 0xc
        lea    edx, [ebp - 0x14]
        push   0
        push   0
        push   2
        push   dword ptr [ebp - 0x18]
        push   edx
        call   sub_4_4DBC
loc_3_067F:
        add    esp, 0x14
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ax, word ptr [ecx + 4]
        mov    cx, ax
        and    cx, 0xff0f
        cmp    cx, 0x200
        jne    loc_3_069B
        mov    si, ax
loc_3_069B:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_4_4DD4
loc_3_06A9:
        add    esp, 0xc
        test   eax, eax
        je     loc_3_061F
        jmp    loc_3_0703
loc_3_06B6:
        xor    ebx, ebx
        lea    eax, [ebp - 0x1c]
        push   ebx
        push   edi
        push   eax
        call   sub_4_4DCC
loc_3_06C3:
        add    esp, 0xc
        lea    ecx, [ebp - 0x18]
        push   2
        push   dword ptr [ebp - 0x1c]
        push   ecx
        call   sub_4_4DC4
loc_3_06D4:
        add    esp, 0xc
        lea    edx, [ebp - 0x14]
        push   ebx
        push   ebx
        push   2
        push   dword ptr [ebp - 0x18]
        push   edx
        call   sub_4_4DBC
loc_3_06E7:
        add    esp, 0x14
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ax, word ptr [ecx + 4]
        mov    cx, ax
        and    cx, 0xff0f
        cmp    cx, 0x200
        jne    loc_3_0703
        mov    si, ax
loc_3_0703:
        cmp    si, 0xffff
        jne    loc_3_0711
        xor    eax, eax
        jmp    loc_3_0806
loc_3_0711:
        lea    ebx, [esi + 4]
        xor    edi, edi
        add    si, 5
        mov    al, 0xfe
        mov    dx, bx
        mov    dword ptr [ebp - 4], edi
        out    dx, al
        cmp    dword ptr [ebp - 4], 0xc8
        jae    loc_3_0738
loc_3_072C:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0xc8
        jb     loc_3_072C
loc_3_0738:
        mov    dx, si
        in     al, dx
        cmp    al, 0xff
        mov    cl, al
        je     loc_3_0785
        mov    dx, bx
        mov    al, 0xfd
        out    dx, al
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 4], 0xc8
        jae    loc_3_0764
loc_3_0758:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0xc8
        jb     loc_3_0758
loc_3_0764:
        mov    dx, si
        in     al, dx
        cmp    al, 0xff
        je     loc_3_0785
        cmp    cl, 0x10
        jb     loc_3_0776
        cmp    cl, 0x1f
        jbe    loc_3_0780
loc_3_0776:
        cmp    cl, 0x30
        jb     loc_3_0785
        cmp    cl, 0x3f
        ja     loc_3_0785
loc_3_0780:
        mov    edi, 2
loc_3_0785:
        xor    ecx, ecx
        test   edi, edi
        jne    loc_3_0804
        mov    dx, bx
        mov    al, 0xff
        out    dx, al
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebp - 4], 0xc8
        jae    loc_3_07A9
loc_3_079D:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0xc8
        jb     loc_3_079D
loc_3_07A9:
        mov    dx, si
        mov    al, 4
        out    dx, al
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 4], 0xc8
        jae    loc_3_07CB
loc_3_07BF:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0xc8
        jb     loc_3_07BF
loc_3_07CB:
        mov    dx, si
        in     al, dx
        cmp    al, 0xff
        jne    loc_3_0804
        mov    dx, bx
        mov    al, 0x82
        out    dx, al
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 4], 0xc8
        jae    loc_3_07F5
loc_3_07E9:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0xc8
        jb     loc_3_07E9
loc_3_07F5:
        mov    dx, si
        in     al, dx
        and    al, 0xf0
        cmp    al, 0x80
        jne    loc_3_0804
        mov    edi, 1
loc_3_0804:
        mov    eax, edi
loc_3_0806:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_080D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Get_Version  ; cd2000003300
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x400
        mov    eax, 1
        ja     loc_3_0863
        xor    esi, esi
        lea    eax, [ebp - 8]
        push   esi
        push   2
        push   esi
        push   4
        push   eax
        call   sub_4_4DF4
loc_3_083C:
        add    esp, 0x14
        lea    ecx, [ebp - 0xc]
        push   esi
        push   dword ptr [ebp - 8]
        push   0xfcff
        push   0xfc00
        push   0x10
        push   esi
        push   ecx
        call   sub_4_4DEC
loc_3_0859:
        add    esp, 0x1c
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_3_0863:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0868:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x48
        push   ebx
        lea    eax, [ebp - 0x14]
        push   esi
        lea    ecx, [ebp - 0x18]
        push   edi
        xor    esi, esi
        push   esi
        lea    edx, [ebp - 0x20]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], 4
        push   ecx
        push   edx
        push   edi
        call   sub_4_4DDC
loc_3_0890:
        add    esp, 0x14
        cmp    dword ptr [ebp - 0x20], 0x10
        jne    loc_3_0A10
        mov    al, byte ptr [ebp - 0x14]
        push   1
        push   edi
        lea    ecx, [ebp - 0xc]
        push   ecx
        mov    byte ptr [ebp - 1], al
        call   sub_4_4DC4
loc_3_08AF:
        add    esp, 0xc
        lea    ecx, [ebp - 8]
        push   esi
        push   esi
        push   2
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_4_4DBC
loc_3_08C2:
        add    esp, 0x14
        mov    ecx, dword ptr [ebp - 8]
        cmp    word ptr [ecx], 1
        ja     loc_3_0A10
        cmp    word ptr [ecx + 0xe], 4
        jne    loc_3_0A10
        push   esi
        lea    eax, [ebp - 0x44]
        push   edi
        push   eax
        call   sub_4_4DCC
loc_3_08E8:
        add    esp, 0xc
        lea    ecx, [ebp - 0x10]
        push   esi
        push   dword ptr [ebp - 0x44]
        push   ecx
        call   sub_4_4DE4
loc_3_08F8:
        add    esp, 0xc
loc_3_08FB:
        cmp    dword ptr [ebp - 0x10], edi
        je     loc_3_0985
        push   esi
        lea    eax, [ebp - 0x14]
        push   eax
        lea    ecx, [ebp - 0x18]
        push   ecx
        lea    edx, [ebp - 0x20]
        mov    byte ptr [ebp - 0x14], 0xff
        mov    dword ptr [ebp - 0x18], 4
        push   edx
        push   dword ptr [ebp - 0x10]
        call   sub_4_4DDC
loc_3_0924:
        add    esp, 0x14
        mov    cl, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 0x14], cl
        jne    loc_3_0985
        push   esi
        lea    ecx, [ebp - 0x48]
        push   dword ptr [ebp - 0x10]
        push   ecx
        lea    edx, [ebp - 0x40]
        push   edx
        call   sub_4_4E04
loc_3_0940:
        add    esp, 0x10
        test   byte ptr [ebp - 0x40], 8
        je     loc_3_0985
        push   2
        lea    ecx, [ebp - 0xc]
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_4_4DC4
loc_3_0957:
        add    esp, 0xc
        lea    edx, [ebp - 8]
        push   esi
        push   esi
        push   2
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_4_4DBC
loc_3_096A:
        add    esp, 0x14
        mov    ecx, dword ptr [ebp - 8]
        mov    bx, word ptr [ecx + 4]
        mov    ax, bx
        and    ax, 0xff0f
        cmp    ax, 0x200
        je     loc_3_0A17
loc_3_0985:
        push   esi
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_4_4DD4
loc_3_0992:
        add    esp, 0xc
        test   eax, eax
        je     loc_3_08FB
        push   1
        lea    eax, [ebp - 0xc]
        push   edi
        xor    ebx, ebx
        push   eax
        call   sub_4_4DC4
loc_3_09AB:
        add    esp, 0xc
        lea    ecx, [ebp - 8]
        push   ebx
        push   ebx
        push   2
        lea    edi, [ebp - 0x38]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_4_4DBC
loc_3_09C1:
        add    esp, 0x14
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x400
        push   ebx
        add    word ptr [ebp - 0x28], si
        push   0x18
        add    word ptr [ebp - 0x26], si
        lea    ecx, [ebp - 0x38]
        push   ecx
        lea    edx, [ebp - 8]
        push   2
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_4_4DFC
loc_3_09F0:
        add    word ptr [ebp - 0x28], si
        add    esp, 0x18
        add    word ptr [ebp - 0x26], si
        lea    ecx, [ebp - 0x38]
        push   ebx
        push   0x18
        push   ecx
        push   2
        push   dword ptr [ebp - 0xc]
        push   ebx
loc_3_0A08:
        call   sub_4_4DFC
loc_3_0A0D:
        add    esp, 0x18
loc_3_0A10:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_0A17:
        add    bx, 0x400
        push   0
        push   2
        lea    eax, [ebp - 0x1c]
        movzx  esi, bx
        push   0
        push   4
        push   eax
        call   sub_4_4DF4
loc_3_0A30:
        add    esp, 0x14
        lea    ecx, [esi + 3]
        lea    eax, [ebp - 0x3c]
        push   0
        push   dword ptr [ebp - 0x1c]
        push   ecx
        push   esi
        push   4
        push   0
        push   eax
        call   sub_4_4DEC
loc_3_0A4A:
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_0A10
        lea    ecx, [esi + 0x403]
        push   0
        add    esi, 0x400
        lea    eax, [ebp - 0x3c]
        push   dword ptr [ebp - 0x1c]
        push   ecx
        push   esi
        push   4
        push   0
        push   eax
        call   sub_4_4DEC
loc_3_0A71:
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_0A10
        push   1
        lea    eax, [ebp - 0xc]
        push   edi
        push   eax
        call   sub_4_4DC4
loc_3_0A84:
        add    esp, 0xc
        lea    ecx, [ebp - 8]
        push   0
        push   0
        push   2
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_4_4DBC
loc_3_0A99:
        add    esp, 0x14
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x10], bx
        lea    edx, [ebx + 3]
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [ebp - 0x38]
        mov    word ptr [ecx + 0x12], dx
        mov    ecx, 6
        mov    esi, dword ptr [ebp - 8]
        lea    edx, [ebx + 0x403]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    word ptr [ebp - 0x26], dx
        lea    eax, [ebx + 0x400]
        mov    word ptr [ebp - 0x28], ax
        push   0
        push   0x18
        lea    ecx, [ebp - 0x38]
        push   ecx
        lea    edx, [ebp - 8]
        push   2
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_4_4DFC
loc_3_0AE4:
        add    esp, 0x18
        lea    ecx, [ebx + 0x800]
        add    bx, 0x803
        lea    eax, [ebp - 0x38]
        mov    word ptr [ebp - 0x28], cx
        push   0
        mov    word ptr [ebp - 0x26], bx
        push   0x18
        push   eax
        push   2
        push   dword ptr [ebp - 0xc]
        push   0
        jmp    loc_3_0A08
sub_3_0B0E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xf8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        push   1
        push   0xf0
        lea    eax, [esi + 0x15e1]
        push   eax
        mov    dword ptr [esi + 8], edi
        push   0
        push   edi
        call   sub_4_4E2C
loc_3_0B39:
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_0EA8
        push   0
        lea    eax, [ebp - 0xf8]
        push   edi
        push   eax
        call   sub_4_4E24
loc_3_0B53:
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        je     loc_3_0B6C
        push   esi
        call   sub_4_0D91
loc_3_0B62:
        add    esp, 4
        mov    eax, ebx
        jmp    loc_3_0EAD
loc_3_0B6C:
        push   0
        lea    eax, [esi + 0x1560]
        push   0x80
        push   eax
        push   edi
        call   sub_4_4E1C
loc_3_0B80:
        add    esp, 0x10
        mov    ebx, eax
        test   ebx, ebx
        je     loc_3_0B99
        push   esi
        call   sub_4_0D91
loc_3_0B8F:
        add    esp, 4
        mov    eax, ebx
        jmp    loc_3_0EAD
loc_3_0B99:
        lea    ebx, [esi + 0x1558]
        push   0
        lea    eax, [esi + 0x155c]
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 0xc], 4
        push   eax
        push   ecx
        push   ebx
        push   dword ptr [esi + 8]
        call   sub_4_4DDC
loc_3_0BBC:
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_0EA8
        cmp    dword ptr [ebx], 0x10
        jne    loc_3_0D46
        cmp    word ptr [ebp - 0x9c], 1
        jne    loc_3_0D46
        cmp    word ptr [ebp - 0x72], 4
        jne    loc_3_0D46
        push   0
        lea    eax, [ebp - 0x1c]
        push   edi
        push   eax
        call   sub_4_4DCC
loc_3_0BF5:
        add    esp, 0xc
        lea    ecx, [ebp - 8]
        push   0
        push   dword ptr [ebp - 0x1c]
        push   ecx
        call   sub_4_4DE4
loc_3_0C06:
        add    esp, 0xc
loc_3_0C09:
        cmp    dword ptr [ebp - 8], edi
        je     loc_3_0CC4
        push   0
        lea    eax, [ebp - 0x18]
        push   eax
        lea    ecx, [ebp - 0xc]
        push   ecx
        lea    edx, [ebp - 0x20]
        mov    byte ptr [ebp - 0x18], 0xff
        mov    dword ptr [ebp - 0xc], 4
        push   edx
        push   dword ptr [ebp - 8]
        call   sub_4_4DDC
loc_3_0C33:
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_0CC4
        cmp    byte ptr [esi + 0x155d], 0
        je     loc_3_0C52
        mov    al, byte ptr [ebp - 0x18]
        cmp    byte ptr [esi + 0x155c], al
        jne    loc_3_0CC4
loc_3_0C52:
        push   dword ptr [ebp - 8]
        call   sub_3_0530
        add    esp, 4
        test   eax, eax
        je     loc_3_0CC4
        cmp    byte ptr [esi + 0x155d], 0
        jne    loc_3_0C89
        mov    eax, dword ptr [0]            ; -> obj1:0x0418
        test   eax, eax
        je     loc_3_0C89
loc_3_0C73:
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x1554], ecx
        je     loc_3_0C85
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_3_0C73
loc_3_0C85:
        test   eax, eax
        jne    loc_3_0CC4
loc_3_0C89:
        push   2
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_4_4DC4
loc_3_0C97:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_0CC4
        push   0
        lea    ecx, [ebp - 0x14]
        push   0
        push   2
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_4_4DBC
loc_3_0CB0:
        add    esp, 0x14
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ax, word ptr [ecx + 4]
        and    ax, 0xff0f
        cmp    ax, 0x200
        je     loc_3_0CDF
loc_3_0CC4:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_4_4DD4
loc_3_0CD2:
        add    esp, 0xc
        test   eax, eax
        je     loc_3_0C09
        jmp    loc_3_0D46
loc_3_0CDF:
        push   1
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 2]
        mov    dword ptr [ebp - 0xc], 2
        push   ecx
        push   1
        push   0                             ; -> obj5:0x01DC
        push   0                             ; -> obj5:0x01D0
        push   edi
        call   sub_4_4DB4
loc_3_0D02:
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_0D0F
        cmp    byte ptr [ebp - 2], 0x31
        jne    loc_3_0D46
loc_3_0D0F:
        mov    eax, dword ptr [ebp - 0x14]
        push   2
        mov    ax, word ptr [eax + 4]
        push   edi
        add    ax, 0x400
        lea    ecx, [ebp - 0x10]
        mov    word ptr [ebp - 0x9a], ax
        push   ecx
        call   sub_4_4DC4
loc_3_0D2D:
        add    esp, 0xc
        push   0
        push   dword ptr [ebp - 0x10]
        call   sub_4_4E14
loc_3_0D3A:
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0x1554], eax
loc_3_0D46:
        cmp    word ptr [ebp - 0x9c], 3
        jne    loc_3_0D7E
        xor    cx, cx
loc_3_0D53:
        movzx  eax, cx
        add    eax, eax
        cmp    word ptr [ebp + eax - 0x72], 4
        jne    loc_3_0E9F
        mov    dx, word ptr [ebp + eax - 0x9a]
        inc    cx
        mov    word ptr [esi + eax + 0x24], dx
        cmp    cx, 3
        jb     loc_3_0D53
        jmp    loc_3_0E1F
loc_3_0D7E:
        cmp    word ptr [ebp - 0x9c], 1
        jne    loc_3_0E9F
        cmp    word ptr [ebp - 0x72], 4
        jne    loc_3_0DC7
        mov    ax, word ptr [ebp - 0x9a]
        push   esi
        mov    word ptr [esi + 0x24], ax
        add    ax, 0x400
        mov    word ptr [esi + 0x26], ax
        mov    ax, word ptr [ebp - 0x9a]
        add    ax, 0x800
        mov    word ptr [esi + 0x28], ax
        call   sub_3_0EB4
        add    esp, 4
        test   eax, eax
        je     loc_3_0E1F
        jmp    loc_3_0E9F
loc_3_0DC7:
        cmp    word ptr [ebp - 0x9c], 1
        jne    loc_3_0E9F
        cmp    word ptr [ebp - 0x72], 0x10
        jne    loc_3_0E9F
        mov    ax, word ptr [ebp - 0x9a]
        add    ax, 4
        mov    word ptr [esi + 0x24], ax
        mov    ax, word ptr [ebp - 0x9a]
        add    ax, 8
        mov    word ptr [esi + 0x26], ax
        mov    ax, word ptr [ebp - 0x9a]
        add    ax, 0xc
        test   byte ptr [ebp - 0x99], 0x80
        mov    word ptr [esi + 0x28], ax
        je     loc_3_0E1F
        push   esi
        call   sub_3_0EB4
        add    esp, 4
loc_3_0E1F:
        push   esi
        call   sub_3_00E5
        add    esp, 4
        test   eax, eax
        je     loc_3_0E4A
        cmp    dword ptr [ebx], 0x10
        jne    loc_3_0E3A
        push   esi
        call   sub_3_0F98
        add    esp, 4
loc_3_0E3A:
        push   esi
        call   sub_4_0D91
loc_3_0E40:
        add    esp, 4
        mov    eax, 0x24
        jmp    loc_3_0EAD
loc_3_0E4A:
        push   edi
        call   sub_3_0598
        movzx  eax, ax
        add    esp, 4
        mov    dword ptr [esi + 0x154e], eax
        push   esi
        call   0
        add    esp, 4
        test   eax, eax
        jne    loc_3_0E9F
        mov    eax, dword ptr [0]            ; -> obj1:0x0418
        mov    dword ptr [esi + 4], eax
        xor    eax, eax
        mov    dword ptr [0], esi            ; -> obj1:0x0418
        mov    ax, word ptr [esi + 0x24]
        and    eax, 0xfff8
        push   eax
        push   0x45
        call   sub_3_03A8
        add    esp, 8
        call   sub_4_4E0C
loc_3_0E92:
        test   eax, eax
        je     loc_3_0E9B
        call   sub_4_0BE2
loc_3_0E9B:
        xor    eax, eax
        jmp    loc_3_0EAD
loc_3_0E9F:
        push   esi
        call   sub_4_0D91
loc_3_0EA5:
        add    esp, 4
loc_3_0EA8:
        mov    eax, 0x13
loc_3_0EAD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0EB4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 0x1558], 0x10
        je     loc_3_0ECF
        mov    eax, 0x13
        jmp    loc_3_0F93
loc_3_0ECF:
        mov    edx, 0x279
        xor    al, al
        out    dx, al
        out    dx, al
        xor    ecx, ecx
loc_3_0EDA:
        mov    al, byte ptr [ecx]            ; -> obj5:0x00F0
        mov    edx, 0x279
        out    dx, al
        inc    ecx
        cmp    ecx, 0x20
        jl     loc_3_0EDA
        mov    esi, edx
        mov    al, 3
        out    dx, al
        mov    edi, 0xa79
        mov    al, byte ptr [ebp + 0x155c]
        mov    edx, edi
        mov    bl, byte ptr [ebp + 0x155d]
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, esi
        mov    al, 0x30
        out    dx, al
        mov    cx, word ptr [ebp + 0x26]
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 0x62
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    edx, esi
        mov    al, 0x63
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    cx, word ptr [ebp + 0x28]
        mov    edx, esi
        mov    al, 0x64
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    edx, esi
        mov    al, 0x65
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    cx, word ptr [ebp + 0x24]
        mov    edx, esi
        mov    al, 0x66
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    edx, esi
        mov    al, 0x67
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, 0x60
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    edx, esi
        mov    al, 0x61
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, esi
        mov    al, 0x30
        out    dx, al
        mov    edx, edi
        mov    al, 1
        out    dx, al
        mov    edx, esi
        mov    al, 2
        out    dx, al
        mov    edx, edi
        out    dx, al
        xor    eax, eax
loc_3_0F93:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_0F98:
        push   esi
        mov    edx, 0x279
        push   edi
        xor    al, al
        out    dx, al
        out    dx, al
        xor    ecx, ecx
loc_3_0FA5:
        mov    al, byte ptr [ecx]            ; -> obj5:0x00F0
        mov    edx, 0x279
        out    dx, al
        inc    ecx
        cmp    ecx, 0x20
        jl     loc_3_0FA5
        mov    esi, edx
        mov    al, 3
        mov    edi, 0xa79
        mov    ecx, dword ptr [esp + 0xc]
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ecx + 0x155c]
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    cl, byte ptr [ecx + 0x155d]
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, 0x30
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 0x62
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 0x63
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 0x64
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 0x65
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 0x66
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 0x67
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, 0x30
        out    dx, al
        mov    edx, edi
        mov    al, 1
        out    dx, al
        mov    edx, esi
        mov    al, 2
        out    dx, al
        mov    edx, edi
        pop    edi
        out    dx, al
        xor    eax, eax
        pop    esi
        ret    
loc_3_1045:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xf0
        test   eax, eax
        je     loc_3_1089
        cmp    eax, 1
        je     loc_3_10A9
        cmp    eax, 2
        je     loc_3_1172
        cmp    eax, 3
        je     loc_3_11D2
        cmp    eax, 4
        je     loc_3_1172
        cmp    eax, 0xd
        je     loc_3_1172
        mov    eax, 1
        jmp    loc_3_11DF
loc_3_1089:
        cmp    dword ptr [0], 0              ; -> obj1:0x0460
        jne    loc_3_109F
        cmp    dword ptr [0], 0              ; -> obj1:0x0474
        je     loc_3_11DD
loc_3_109F:
        mov    eax, 7
        jmp    loc_3_11DF
loc_3_10A9:
        cmp    dword ptr [ebp + 0xc], 1
        jbe    loc_3_10B9
        mov    eax, 1
        jmp    loc_3_11DF
loc_3_10B9:
        push   0
        lea    ecx, [ebp - 0xf0]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_4_4E24
loc_3_10CA:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_11DF
        push   dword ptr [ebp + 0x14]
        push   0                             ; -> loc_1_04A8
        mov    ax, word ptr [ebp - 0x92]
        push   0                             ; -> loc_1_12D0
        mov    word ptr [ebp - 4], ax
        lea    edx, [ebp - 4]
        inc    ax
        push   edx
        mov    word ptr [ebp - 2], ax
        push   2
        call   sub_4_0E11
loc_3_10FE:
        add    esp, 0x14
        test   eax, eax
        je     loc_3_110F
        mov    eax, 0x13
        jmp    loc_3_11DF
loc_3_110F:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [0], eax            ; -> obj1:0x0474
        mov    dword ptr [0], ecx            ; -> obj1:0x0460
        mov    dword ptr [0], 1              ; -> obj1:0x0464
        mov    dword ptr [ebp - 0x18], 0x80
        mov    dword ptr [ebp - 0x14], 0     ; -> obj5:0x01F8
        mov    dword ptr [ebp - 0x10], 0     ; -> obj5:0x01F0
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [0], ax              ; -> obj1:0x0468
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0003h              ; cd2003004a04
        add    esp, 0xc
        movzx  eax, word ptr [ebp - 4]
        push   eax
        push   0x50
        call   sub_3_03A8
        add    esp, 8
        jmp    loc_3_11DD
loc_3_1172:
        mov    ax, word ptr [0]              ; -> obj1:0x0468
        xor    ecx, ecx
        mov    word ptr [ebp - 4], ax
        mov    dword ptr [0], ecx            ; -> obj1:0x0460
        inc    ax
        mov    dword ptr [0], ecx            ; -> loc_1_0484
        mov    word ptr [ebp - 2], ax
        mov    dword ptr [0], ecx            ; -> obj1:0x0478
        mov    word ptr [0], cx              ; -> obj1:0x0468
        lea    ecx, [ebp - 4]
        push   ecx
        push   2
        call   sub_4_0DAB
loc_3_11A8:
        mov    dword ptr [ebp - 0xc], 0      ; -> obj5:0x01F8
        mov    dword ptr [ebp - 8], 0        ; -> obj5:0x01F0
        add    esp, 8
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
        xor    eax, eax
        jmp    loc_3_11DF
loc_3_11D2:
        cmp    dword ptr [ebp + 0xc], 1
        mov    eax, 1
        ja     loc_3_11DF
loc_3_11DD:
        xor    eax, eax
loc_3_11DF:
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_11E3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        cmp    dword ptr [ebp + 8], 5
        push   ebx
        push   esi
        push   edi
        je     loc_3_11FC
        mov    eax, 1
        jmp    loc_3_148C
loc_3_11FC:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_4_4DE4
loc_3_120A:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_121B
        mov    eax, 1
        jmp    loc_3_148C
loc_3_121B:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 0x10]
        push   0                             ; -> obj5:0x0290
        push   ecx
        call   sub_4_4E44
loc_3_122E:
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_148A
        mov    eax, dword ptr [0]            ; -> obj1:0x0418
        test   byte ptr [eax + 0x25], 0x80
        je     loc_3_12C8
        mov    si, 2
        mov    edi, 0                        ; -> obj5:0x0278
        mov    word ptr [0xe], 1             ; -> obj5:0x0286
        xor    ebx, ebx
loc_3_125C:
        push   ebx
        lea    ecx, [ebp - 4]
        push   0x3000
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_4_4E3C
loc_3_126E:
        add    esp, 0x10
        lea    eax, [esi - 0x7f30]
        mov    word ptr [0x10], ax           ; -> obj5:0x0288
        push   ebx
        mov    word ptr [0x12], ax           ; -> obj5:0x028A
        push   0x18
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_4_4DFC
loc_3_1292:
        add    esp, 0x18
        lea    eax, [esi - 0x7e30]
        add    si, 2
        push   ebx
        mov    word ptr [0x10], ax           ; -> obj5:0x0288
        push   0x18
        mov    word ptr [0x12], ax           ; -> obj5:0x028A
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_4_4DFC
loc_3_12BA:
        add    esp, 0x18
        cmp    si, 0xe
        jbe    loc_3_125C
        jmp    loc_3_148A
loc_3_12C8:
        xor    esi, esi
        mov    ebx, 0x2000
        push   esi
        lea    ecx, [ebp - 4]
        push   ebx
        push   dword ptr [ebp - 8]
        push   ecx
        mov    edi, 0                        ; -> obj5:0x0278
        call   sub_4_4E3C
loc_3_12E2:
        mov    word ptr [0x10], 0x300        ; -> obj5:0x0288
        add    esp, 0x10
        mov    word ptr [0x12], 0x301        ; -> obj5:0x028A
        push   esi
        push   0x18
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_4_4DFC
loc_3_1306:
        add    esp, 0x18
        lea    ecx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_4_4E3C
loc_3_1317:
        mov    word ptr [0x10], 0x330        ; -> obj5:0x0288
        add    esp, 0x10
        mov    word ptr [0x12], 0x331        ; -> obj5:0x028A
        push   esi
        push   0x18
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        mov    ebx, 0x3000
        call   sub_4_4DFC
loc_3_1340:
        add    esp, 0x18
        lea    eax, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   eax
        call   sub_4_4E3C
loc_3_1351:
        mov    word ptr [0x10], 0x320        ; -> obj5:0x0288
        add    esp, 0x10
        mov    word ptr [0x12], 0x321        ; -> obj5:0x028A
        push   esi
        push   0x18
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_4_4DFC
loc_3_1375:
        add    esp, 0x18
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_4_4E3C
loc_3_1386:
        mov    word ptr [0x10], 0x332        ; -> obj5:0x0288
        mov    word ptr [0x12], 0x337        ; -> obj5:0x028A
        add    esp, 0x10
        mov    word ptr [0xc], 0xfffe        ; -> obj5:0x0284
        push   esi
        push   0x18
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_4_4DFC
loc_3_13B3:
        add    esp, 0x18
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_4_4E3C
loc_3_13C4:
        mov    word ptr [0x10], 0x340        ; -> obj5:0x0288
        add    esp, 0x10
        mov    word ptr [0x12], 0x341        ; -> obj5:0x028A
        push   esi
        push   0x18
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_4_4DFC
loc_3_13E8:
        add    esp, 0x18
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_4_4E3C
loc_3_13F9:
        mov    word ptr [0x10], 0x350        ; -> obj5:0x0288
        add    esp, 0x10
        mov    word ptr [0x12], 0x351        ; -> obj5:0x028A
        push   esi
        push   0x18
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_4_4DFC
loc_3_141D:
        add    esp, 0x18
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_4_4E3C
loc_3_142E:
        mov    word ptr [0x10], 0x360        ; -> obj5:0x0288
        add    esp, 0x10
        mov    word ptr [0x12], 0x361        ; -> obj5:0x028A
        push   esi
        push   0x18
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_4_4DFC
loc_3_1452:
        add    esp, 0x18
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_4_4E3C
loc_3_1463:
        mov    word ptr [0x10], 0x370        ; -> obj5:0x0288
        add    esp, 0x10
        mov    word ptr [0x12], 0x371        ; -> obj5:0x028A
        push   esi
        push   0x18
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_4_4DFC
loc_3_1487:
        add    esp, 0x18
loc_3_148A:
        xor    eax, eax
loc_3_148C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1493:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> obj1:0x0464
        test   eax, eax
        je     loc_3_14D8
        cmp    dword ptr [0], ecx            ; -> sub_1_0480
        je     loc_3_14C4
        push   ecx
        mov    eax, dword ptr [0]            ; -> loc_1_047C
        push   ecx
        push   3
        push   eax
        call   sub_1_0480
        add    esp, 0x10
        jmp    loc_3_1536
loc_3_14C4:
        movzx  eax, word ptr [0]             ; -> obj1:0x0468
        push   eax
        push   0x50
        call   sub_3_03A8
        add    esp, 8
        jmp    loc_3_1536
loc_3_14D8:
        cmp    dword ptr [0], 0              ; -> obj1:0x0478
        je     loc_3_14F4
        mov    eax, dword ptr [0]            ; -> obj1:0x0474
        cmp    dword ptr [eax], 0
        jne    loc_3_14F4
        push   eax
        call   sub_1_5118
loc_3_14F1:
        add    esp, 4
loc_3_14F4:
        xor    eax, eax
        cmp    dword ptr [0], eax            ; -> sub_1_0480
        je     loc_3_1513
        push   eax
        push   eax
        push   4
        mov    eax, dword ptr [0]            ; -> loc_1_047C
        push   eax
        call   sub_1_0480
        add    esp, 0x10
        jmp    loc_3_1536
loc_3_1513:
        mov    dword ptr [ebp - 8], 0        ; -> obj5:0x01F8
        mov    dword ptr [ebp - 4], 0        ; -> obj5:0x01F0
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_3_1536:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_153C:
        push   dword ptr [esp + 4]
        call   sub_3_1548
        ret    8
sub_3_1548:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        xor    esi, esi
        mov    ax, word ptr [edi + 4]
        add    ax, 2
        movzx  edx, ax
        inc    edx
        in     al, dx
        mov    cl, al
loc_3_1564:
        in     al, dx
        cmp    al, cl
        jne    loc_3_156F
        inc    esi
        cmp    esi, 0x20
        jb     loc_3_1564
loc_3_156F:
        cmp    esi, 0x20
        je     loc_3_15FD
        push   0x4e0
        push   edi
        call   sub_1_1976
loc_3_1583:
        and    al, 0x8c
        cmp    al, 0xc
        jne    loc_3_15FD
        mov    ebx, 0x23e
        push   ebx
        push   edi
        call   sub_1_1976
loc_3_1595:
        movzx  bp, al
        not    bp
        and    bp, 6
        push   0x23f
        movzx  esi, bp
        shl    esi, 4
        push   edi
        call   sub_1_1976
loc_3_15B1:
        xor    cx, cx
        mov    cl, al
        and    cx, 0x1f
        mov    word ptr [esp + 0x12], cx
        mov    ecx, esi
        movzx  eax, word ptr [esp + 0x12]
        or     ecx, eax
        push   ecx
        push   ebx
        push   edi
        call   sub_1_1942
loc_3_15D0:
        push   ebx
        push   edi
        call   sub_1_1976
loc_3_15D7:
        xor    ecx, ecx
        mov    cl, al
        movzx  eax, bp
        and    ecx, 6
        cmp    ecx, eax
        jne    loc_3_15FD
        not    esi
        movzx  eax, word ptr [esp + 0x12]
        and    esi, 0x60
        or     esi, eax
        push   esi
        push   ebx
        push   edi
        call   sub_1_1942
loc_3_15F9:
        xor    eax, eax
        jmp    loc_3_1602
loc_3_15FD:
        mov    eax, 1
loc_3_1602:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
sub_3_160C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        mov    dword ptr [ebp - 0x10], 0x240
        mov    dword ptr [ebp - 0xc], 0x340
        mov    dword ptr [ebp - 8], 0x260
        mov    dword ptr [ebp - 4], 0x360
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_3_1635:
        movzx  eax, word ptr [edi*2]         ; -> obj5:0x02B0
        push   eax
        mov    ecx, edi
        mov    eax, edi
        and    ecx, 0x1f
        and    al, 0xe7
        inc    edi
        shr    eax, 3
        mov    eax, dword ptr [ebp + eax - 0x10]
        or     eax, ecx
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1658:
        cmp    edi, 0x80
        jb     loc_3_1635
        push   0x400
        xor    edi, edi
        push   esi
        call   sub_3_16D1
loc_3_166D:
        test   edi, 1
        jne    loc_3_1680
        movzx  eax, word ptr [edi*2]         ; -> obj5:0x02B0
        push   eax
        jmp    loc_3_168F
loc_3_1680:
        mov    ax, word ptr [edi*2]          ; -> obj5:0x02B0
        or     ah, 0x80
        movzx  ecx, ax
        push   ecx
loc_3_168F:
        mov    eax, edi
        mov    ecx, edi
        and    al, 0xe7
        and    ecx, 0x1f
        shr    eax, 3
        inc    edi
        mov    eax, dword ptr [ebp + eax - 0x10]
        or     eax, ecx
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_16A9:
        cmp    edi, 0x80
        jb     loc_3_166D
        xor    edi, edi
loc_3_16B3:
        push   0
        mov    eax, edi
        or     eax, 0x220
        inc    edi
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_16C4:
        cmp    edi, 0x14
        jb     loc_3_16B3
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_16D1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0x33b
        push   dword ptr [ebp + 8]
        call   sub_1_19DF
loc_3_16E7:
        mov    esi, dword ptr [ebp + 0xc]
        xor    edi, edi
        add    si, ax
loc_3_16EF:
        push   0x33b
        push   dword ptr [ebp + 8]
        call   sub_1_19DF
loc_3_16FC:
        cmp    ax, si
        mov    bx, ax
        je     loc_3_1732
        mov    dword ptr [ebp - 4], edi
loc_3_1707:
        push   0x33b
        push   dword ptr [ebp + 8]
        call   sub_1_19DF
loc_3_1714:
        cmp    ax, bx
        jne    loc_3_1729
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0x4000
        lea    ecx, [eax + 1]
        mov    dword ptr [ebp - 4], ecx
        jb     loc_3_1707
loc_3_1729:
        cmp    dword ptr [ebp - 4], 0x4000
        jb     loc_3_16EF
loc_3_1732:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_173B:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   0xe0c
        push   0x4e0
        push   esi
        call   sub_1_19AA
loc_3_1751:
        push   0x59
        push   0x23d
        push   esi
        call   sub_1_1942
loc_3_175E:
        push   0x20
        push   0x23e
        push   esi
        call   sub_1_1942
loc_3_176B:
        push   0
        push   0x23f
        push   esi
        call   sub_1_1942
loc_3_1778:
        push   esi
        call   sub_3_184B
        push   esi
        call   sub_3_1953
        push   esi
        call   sub_3_160C
        cmp    dword ptr [esi + 8], 0
        jne    loc_3_1796
        push   esi
        call   sub_3_1A39
loc_3_1796:
        mov    dword ptr [esi + 0x14], 0x5050204
        mov    word ptr [esi + 0x12], 0
        push   esi
        call   sub_4_2380
loc_3_17A9:
        add    esp, 4
        push   -1
        push   -1
        push   esi
        call   sub_4_2458
loc_3_17B6:
        add    esp, 0xc
        cmp    word ptr [esi + 6], 0x1e
        ja     loc_3_17C6
        push   esi
        call   sub_4_106D
loc_3_17C6:
        push   6
        push   0x23f
        push   esi
        call   sub_1_1942
loc_3_17D3:
        push   0x23e
        push   esi
        call   sub_1_1976
loc_3_17DE:
        test   al, 0x40
        jne    loc_3_17E9
        mov    eax, 1
        jmp    loc_3_1846
loc_3_17E9:
        mov    eax, 0x20
        lea    ecx, [esi + 0x42]
        xor    edx, edx
        mov    dword ptr [esi + 0x18], eax
loc_3_17F6:
        mov    word ptr [ecx], 0xffe
        add    ecx, 0x20
        dec    eax
        mov    byte ptr [ecx - 0xc], dl
        mov    dword ptr [ecx - 0x16], edx
        mov    byte ptr [ecx - 9], 1
        jne    loc_3_17F6
        lea    edi, [esi + 0x662]
        xor    eax, eax
        mov    ecx, 0x120
        xor    edx, edx
        rep stosd dword ptr es:[edi], eax
        mov    eax, 0x80
        lea    edi, [esi + 0xaee]
        mov    ecx, edi
loc_3_1829:
        mov    dword ptr [ecx], edx
        add    ecx, 8
        dec    eax
        jne    loc_3_1829
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_3_1844
        mov    dword ptr [edi], 0x200000
        mov    dword ptr [esi + 0xaf2], eax
loc_3_1844:
        xor    eax, eax
loc_3_1846:
        pop    edi
        pop    esi
        ret    4
sub_3_184B:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        xor    edi, edi
        xor    ebx, ebx
loc_3_1856:
        push   0x80
        mov    eax, edi
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1869:
        push   ebx
        mov    eax, edi
        or     eax, 0x3c0
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1878:
        push   ebx
        mov    eax, edi
        or     eax, 0x2e0
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1887:
        push   0xd000
        mov    eax, edi
        or     ah, 4
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1898:
        push   0xff00
        mov    eax, edi
        or     eax, 0x420
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_18AB:
        push   ebx
        mov    eax, edi
        or     eax, 0x440
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_18BA:
        push   ebx
        mov    eax, edi
        or     eax, 0x460
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_18C9:
        push   0x18
        mov    eax, edi
        or     eax, 0x480
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_18D9:
        push   0x18
        mov    eax, edi
        or     eax, 0x4a0
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_18E9:
        push   ebx
        mov    eax, edi
        or     eax, 0x4c0
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_18F8:
        push   ebx
        mov    eax, edi
        or     eax, 0x3e0
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1907:
        push   ebx
        mov    eax, edi
        or     eax, 0x3a0
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1916:
        push   ebx
        mov    eax, edi
        or     eax, 0x380
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1925:
        push   ebx
        mov    eax, edi
        or     eax, 0x280
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1934:
        push   ebx
        mov    eax, edi
        or     eax, 0x2c0
        inc    edi
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1944:
        cmp    edi, 0x20
        jb     loc_3_1856
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_3_1953:
        push   ebx
        push   esi
        push   edi
        mov    esi, 0x80000000
        mov    ebx, dword ptr [esp + 0x10]
        push   ebp
        push   esi
        push   0x234
        push   ebx
        call   sub_1_1A14
loc_3_196C:
        push   esi
        push   0x235
        push   ebx
        call   sub_1_1A14
loc_3_1978:
        push   esi
        push   0x236
        push   ebx
        call   sub_1_1A14
loc_3_1984:
        push   esi
        push   0x237
        push   ebx
        xor    esi, esi
        mov    edi, 0xffff
        call   sub_1_1A14
loc_3_1997:
        push   2
        mov    ebp, 0x1d8
        push   ebx
        call   sub_3_16D1
loc_3_19A4:
        push   0
        mov    eax, esi
        or     al, 0x20
        push   eax
        push   ebx
        call   sub_1_1A14
loc_3_19B1:
        push   edi
        mov    eax, esi
        or     al, 0x60
        push   eax
        push   ebx
        call   sub_1_1A14
loc_3_19BD:
        push   ebp
        mov    eax, esi
        or     al, 0xc0
        push   eax
        push   ebx
        call   sub_1_1A14
loc_3_19C9:
        push   0x1e0
        mov    eax, esi
        or     al, 0xe0
        push   eax
        push   ebx
        call   sub_1_1A14
loc_3_19D9:
        push   0
        push   esi
        push   ebx
        call   sub_1_1A14
loc_3_19E2:
        push   edi
        mov    eax, esi
        or     al, 0x40
        push   eax
        push   ebx
        call   sub_1_1A14
loc_3_19EE:
        push   ebp
        mov    eax, esi
        or     ah, 2
        push   eax
        push   ebx
        call   sub_1_1A14
loc_3_19FB:
        push   0
        mov    eax, esi
        or     al, 0xa0
        push   eax
        push   ebx
        call   sub_1_1A14
loc_3_1A08:
        push   0
        mov    eax, esi
        or     al, 0x80
        push   eax
        push   ebx
        call   sub_1_1A14
loc_3_1A15:
        push   0x807f
        mov    eax, esi
        or     eax, 0x2a0
        inc    esi
        push   eax
        push   ebx
        call   sub_1_19AA
loc_3_1A29:
        cmp    esi, 0x20
        jb     loc_3_19A4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_3_1A39:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        xor    edi, edi
        mov    esi, dword ptr [esp + 0x18]
loc_3_1A48:
        push   0x80
        mov    eax, edi
        or     eax, 0x2a0
        inc    edi
        push   eax
        push   esi
        call   sub_1_19AA
loc_3_1A5C:
        cmp    edi, 0x1e
        jb     loc_3_1A48
        xor    edi, edi
        mov    ebp, 0x40000000
loc_3_1A68:
        push   0
        mov    eax, edi
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1A75:
        push   0
        mov    eax, edi
        or     al, 0x40
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1A82:
        push   0
        mov    eax, edi
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1A8F:
        push   0
        mov    eax, edi
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1A9C:
        push   ebp
        mov    eax, edi
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1AA8:
        push   ebp
        push   edi
        push   esi
        call   sub_1_1A14
loc_3_1AB0:
        test   edi, 1
        je     loc_3_1ABF
        push   0x6000000
        jmp    loc_3_1AC4
loc_3_1ABF:
        push   0x4000000
loc_3_1AC4:
        mov    eax, edi
        inc    edi
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1AD1:
        cmp    edi, 0x1e
        jb     loc_3_1A68
        push   0x200000
        push   0x236
        push   esi
        mov    edi, 0x80
        call   sub_1_1A14
loc_3_1AEB:
        push   0xaa55
        push   0x23a
        push   esi
        call   sub_1_19AA
loc_3_1AFB:
        push   0x234
        push   esi
        call   sub_1_1A3D
loc_3_1B06:
        test   eax, 0x80000000
        je     loc_3_1B10
        dec    edi
        jne    loc_3_1AFB
loc_3_1B10:
        mov    edi, 0x200000
        mov    ebp, 0x23a
        push   edi
        push   0x234
        push   esi
        call   sub_1_1A14
loc_3_1B26:
        push   ebp
        push   esi
        call   sub_1_19DF
loc_3_1B2D:
        push   ebp
        push   esi
        call   sub_1_19DF
loc_3_1B34:
        cmp    ax, 0xaa55
        jne    loc_3_1C3F
        push   edi
        push   0x236
        push   esi
        mov    edi, 0x80
        call   sub_1_1A14
loc_3_1B4F:
        push   0x55aa
        push   ebp
        push   esi
        call   sub_1_19AA
loc_3_1B5B:
        push   0x234
        push   esi
        call   sub_1_1A3D
loc_3_1B66:
        test   eax, 0x80000000
        je     loc_3_1B70
        dec    edi
        jne    loc_3_1B5B
loc_3_1B70:
        push   0x200000
        mov    ebp, 0x23a
        push   0x234
        push   esi
        call   sub_1_1A14
loc_3_1B85:
        push   ebp
        push   esi
        call   sub_1_19DF
loc_3_1B8C:
        push   ebp
        push   esi
        call   sub_1_19DF
loc_3_1B93:
        cmp    ax, 0x55aa
        jne    loc_3_1C3F
        mov    ebx, 0x40000
loc_3_1BA2:
        lea    edi, [ebx + 0x200000]
        push   edi
        push   0x236
        push   esi
        call   sub_1_1A14
loc_3_1BB4:
        push   0xaa55
        push   ebp
        push   esi
        call   sub_1_19AA
loc_3_1BC0:
        push   0x200000
        push   0x234
        push   esi
        call   sub_1_1A14
loc_3_1BD0:
        push   ebp
        push   esi
        call   sub_1_19DF
loc_3_1BD7:
        push   ebp
        push   esi
        call   sub_1_19DF
loc_3_1BDE:
        cmp    ax, 0x55aa
        jne    loc_3_1C32
        mov    dword ptr [esp + 0x10], 0x80
loc_3_1BEC:
        push   0x234
        push   esi
        call   sub_1_1A3D
loc_3_1BF7:
        test   eax, 0x80000000
        je     loc_3_1C04
        dec    dword ptr [esp + 0x10]
        jne    loc_3_1BEC
loc_3_1C04:
        push   edi
        push   0x234
        push   esi
        call   sub_1_1A14
loc_3_1C10:
        push   ebp
        push   esi
        call   sub_1_19DF
loc_3_1C17:
        push   ebp
        push   esi
        call   sub_1_19DF
loc_3_1C1E:
        cmp    ax, 0xaa55
        jne    loc_3_1C32
        add    ebx, ebx
        cmp    ebx, 0x1000000
        jb     loc_3_1BA2
loc_3_1C32:
        cmp    ebx, 0x1000000
        jne    loc_3_1C3F
        mov    ebx, 0xdfffe0
loc_3_1C3F:
        mov    edi, 0x80
        mov    dword ptr [esi + 8], ebx
        mov    dword ptr [esi + 0xc], ebx
loc_3_1C4A:
        push   0x234
        push   esi
        call   sub_1_1A3D
loc_3_1C55:
        test   eax, 0x80000000
        je     loc_3_1C5F
        dec    edi
        jne    loc_3_1C4A
loc_3_1C5F:
        xor    ebx, ebx
        mov    edi, 0x23a
        push   ebx
        push   0x234
        push   esi
        call   sub_1_1A14
loc_3_1C72:
        push   edi
        push   esi
        call   sub_1_19DF
loc_3_1C79:
        push   edi
        push   esi
        call   sub_1_19DF
loc_3_1C80:
        and    ax, 0xff
        push   edi
        sub    ax, 0x30
        push   esi
        mov    word ptr [esi + 0x1526], ax
        call   sub_1_19DF
loc_3_1C96:
        and    ax, 0xff
        push   edi
        sub    ax, 0x30
        push   esi
        mov    word ptr [esi + 0x1528], ax
        call   sub_1_19DF
loc_3_1CAC:
        push   edi
        mov    byte ptr [esi + 0x151e], ah
        push   esi
        mov    byte ptr [esi + 0x151f], al
        call   sub_1_19DF
loc_3_1CBF:
        mov    byte ptr [esi + 0x1520], ah
        mov    byte ptr [esi + 0x1521], al
        mov    byte ptr [esi + 0x1522], bl
loc_3_1CD1:
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1CDF:
        push   0x1d8
        mov    eax, ebx
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1CEF:
        push   0x1e0
        mov    eax, ebx
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1CFF:
        push   0x40000000
        mov    eax, ebx
        or     al, 0x20
        inc    ebx
        push   eax
        push   esi
        call   sub_1_1A14
loc_3_1D10:
        cmp    ebx, 0x1e
        jb     loc_3_1CD1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  0CCh
sub_3_1D20:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x4e4a  R-X
        db  0FFh, 074h, 024h, 004h, 0E8h
        dd  sub_1_110A               ; fixup
        db  083h, 0C4h, 004h, 085h, 0C0h, 075h, 004h, 033h, 0C0h, 0EBh, 006h, 08Bh, 080h, 0D1h, 016h, 000h
        db  000h, 0C3h
sub_4_001B:
        push   dword ptr [esp + 4]
        call   sub_1_110A
loc_4_0024:
        add    esp, 4
        test   eax, eax
        je     loc_4_0035
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x16d1], ecx
loc_4_0035:
        ret    
sub_4_0036:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax], 1
        je     loc_4_0044
        xor    eax, eax
        jmp    loc_4_0072
loc_4_0044:
        cmp    word ptr [eax + 2], 1
        je     loc_4_004F
        xor    eax, eax
        jmp    loc_4_0072
loc_4_004F:
        cmp    word ptr [eax + 0xe], 0x10
        je     loc_4_005A
        xor    eax, eax
        jmp    loc_4_0072
loc_4_005A:
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0xfa0
        jae    loc_4_0068
        xor    eax, eax
        jmp    loc_4_0072
loc_4_0068:
        mov    ecx, 0x157c0
        cmp    ecx, eax
        sbb    eax, eax
        inc    eax
loc_4_0072:
        ret    
loc_4_0073:
        push   ebp
        mov    ecx, 0x10
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    edi, 0                        ; -> obj1:0x0010
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, dword ptr [ebp + 8]
        jne    loc_4_0097
        mov    dword ptr [edx], eax
loc_4_0097:
        mov    edi, 0                        ; -> loc_1_0210
        mov    ecx, 0x10
        mov    esi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_00AD
        add    eax, 4
        mov    dword ptr [edx], eax
loc_4_00AD:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     loc_4_00BD
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
        jmp    loc_4_00C2
loc_4_00BD:
        mov    eax, 0x80004002
loc_4_00C2:
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
loc_4_00C8:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x10]
        inc    eax
        mov    dword ptr [ecx + 0x10], eax
        ret    4
loc_4_00D6:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi + 0x10]
        dec    eax
        mov    dword ptr [edi + 0x10], eax
        jne    loc_4_0132
        mov    eax, dword ptr [edi + 0x14]
        mov    ebx, dword ptr [edi + 0x18]
        push   0xffff
        mov    esi, dword ptr [eax + 0x1c]
        mov    eax, ebx
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_00FF:
        push   0
        mov    eax, ebx
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_010C:
        push   0
        push   ebx
        push   esi
        call   sub_1_1A14
loc_4_0115:
        push   ebx
        push   esi
        call   sub_1_0273
loc_4_011C:
        add    esp, 8
        mov    eax, dword ptr [edi + 0x14]
        push   0
        dec    dword ptr [eax + 0x20]
        push   edi
        call   sub_4_4D5C
        add    esp, 8
        xor    eax, eax
loc_4_0132:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_4_0138:
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret    0xc
loc_4_014F:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        ret    4
loc_4_015F:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        ret    4
loc_4_016F:
        xor    eax, eax
        ret    0x20
loc_4_0174:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi + 0x14]
        test   edi, edi
        mov    ebx, dword ptr [ecx + 0x1c]
        je     loc_4_01B3
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x30]
        sub    eax, dword ptr [esi + 0x2c]
        add    ecx, eax
        mov    dword ptr [ebp - 4], eax
        shr    ecx, 1
        mov    eax, edi
        shr    eax, 1
        push   eax
        push   dword ptr [ebp + 0xc]
        push   ecx
        push   ebx
        call   sub_1_029C
loc_4_01B0:
        add    esp, 0x10
loc_4_01B3:
        mov    eax, dword ptr [ebp + 0x18]
        test   eax, eax
        je     loc_4_01CF
        shr    eax, 1
        mov    ecx, dword ptr [esi + 0x30]
        shr    ecx, 1
        push   eax
        push   dword ptr [ebp + 0x14]
        push   ecx
        push   ebx
        call   sub_1_029C
loc_4_01CC:
        add    esp, 0x10
loc_4_01CF:
        mov    eax, dword ptr [ebp - 4]
        add    eax, edi
        cmp    eax, dword ptr [esi + 0x28]
        jne    loc_4_020E
        and    edi, 0xfffffffe
        mov    eax, dword ptr [ebp + 0xc]
        movsx  ecx, word ptr [edi + eax - 2]
        mov    eax, ecx
        lea    edi, [ebp - 0x24]
        shl    eax, 0x10
        mov    ax, cx
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        push   0x10
        lea    eax, [ebp - 0x24]
        mov    ecx, dword ptr [esi + 0x30]
        push   eax
        add    ecx, dword ptr [esi + 0x28]
        shr    ecx, 1
        push   ecx
        push   ebx
        call   sub_1_029C
loc_4_020B:
        add    esp, 0x10
loc_4_020E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_4_0219:
        mov    eax, 0x88780032
        ret    8
loc_4_0221:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [eax + 8]
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [eax + 0x14]
        mov    esi, dword ptr [eax + 0x18]
        mov    ebx, dword ptr [ecx + 0x1c]
        or     esi, 0x20
        mov    dword ptr [eax + 0x24], edi
        push   esi
        push   ebx
        call   sub_1_1A3D
loc_4_0246:
        test   eax, 0xffff0000
        mov    ecx, eax
        je     loc_4_0270
        shl    edi, 0xe
        mov    ebp, 0xac44
        mov    eax, edi
        sub    edx, edx
        div    ebp
        shl    eax, 0x10
        and    ecx, 0xffff
        or     eax, ecx
        push   eax
        push   esi
        push   ebx
        call   sub_1_1A14
loc_4_0270:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_0279:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [esi + 0x14]
        mov    edx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [ecx + 0x1c]
        mov    edi, dword ptr [ebx + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], edx
        cmp    dword ptr [esi + 0x34], edi
        je     loc_4_02E1
        mov    eax, edx
        mov    dword ptr [esi + 0x34], edi
        or     al, 0x60
        push   eax
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 4]
        call   sub_1_1A3D
loc_4_02B6:
        mov    ecx, edi
        and    eax, 0xffff
        shl    ecx, 0x10
        and    di, 0xff00
        or     ecx, eax
        push   ecx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_02D3:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        shl    ecx, 5
        mov    word ptr [ecx + eax + 0x52], di
loc_4_02E1:
        mov    eax, dword ptr [ebx + 0x14]
        cmp    dword ptr [esi + 0x38], eax
        jne    loc_4_02F1
        mov    eax, dword ptr [ebx + 0x18]
        cmp    dword ptr [esi + 0x3c], eax
        je     loc_4_035E
loc_4_02F1:
        mov    eax, dword ptr [ebp - 8]
        or     al, 0x20
        push   eax
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 4]
        call   sub_1_1A3D
loc_4_0302:
        mov    edi, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        or     edi, 0xc0
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_1_1A3D
loc_4_0317:
        mov    ecx, dword ptr [ebx + 0x14]
        mov    edx, dword ptr [ebx + 0x18]
        shr    edx, 8
        mov    dword ptr [ebp - 8], ecx
        shr    dword ptr [ebp - 8], 8
        mov    dword ptr [esi + 0x38], ecx
        mov    ecx, dword ptr [ebx + 0x18]
        mov    dword ptr [esi + 0x3c], ecx
        mov    ecx, dword ptr [ebp - 0x10]
        and    cl, 0
        mov    esi, eax
        or     ecx, edx
        and    esi, 0xffffff
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 4]
        shl    eax, 0x18
        or     esi, eax
        call   sub_1_1A14
loc_4_0354:
        push   esi
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_035E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0369:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 8]
        mov    ecx, dword ptr [edi + 0x14]
        mov    esi, dword ptr [edi + 0x18]
        or     esi, 0x200
        mov    ebx, dword ptr [ecx + 0x1c]
        push   esi
        push   ebx
        call   sub_1_1A3D
loc_4_0389:
        mov    ecx, dword ptr [edi + 0x30]
        and    eax, 0xff000000
        add    ecx, dword ptr [esp + 0x14]
        shr    ecx, 1
        dec    ecx
        or     eax, ecx
        push   eax
        push   esi
        push   ebx
        call   sub_1_1A14
loc_4_03A2:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_03AA:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 8]
        mov    edx, dword ptr [esi + 0x14]
        mov    ecx, dword ptr [esi + 0x18]
        or     ch, 2
        push   ecx
        push   dword ptr [edx + 0x1c]
        call   sub_1_1A3D
loc_4_03C4:
        and    eax, 0xffffff
        lea    ecx, [eax*2 + 2]
        mov    eax, dword ptr [esi + 0x30]
        and    al, 0xfe
        sub    ecx, eax
        mov    eax, dword ptr [esi + 0x28]
        cmp    ecx, eax
        jb     loc_4_03E0
        mov    ecx, eax
loc_4_03E0:
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 0x10]
        pop    esi
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    dword ptr [edx], ecx
        ret    0xc
loc_4_03F2:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    ecx, dword ptr [esi + 0x14]
        mov    edi, dword ptr [esi + 0x18]
        mov    eax, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [esi + 0x24]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0x14], ecx
        mov    ecx, dword ptr [ebp + 0x14]
        and    ecx, 1
        je     loc_4_0430
        mov    edx, dword ptr [esi + 0x30]
        mov    ebx, dword ptr [esi + 0x28]
        mov    eax, edx
        add    ebx, edx
        shr    eax, 1
        shr    ebx, 1
        mov    dword ptr [ebp - 0x10], eax
        dec    ebx
        jmp    loc_4_043E
loc_4_0430:
        mov    eax, dword ptr [esi + 0x28]
        add    eax, dword ptr [esi + 0x30]
        shr    eax, 1
        mov    dword ptr [ebp - 0x10], eax
        lea    ebx, [eax + 0xf]
loc_4_043E:
        mov    eax, edi
        test   ecx, ecx
        je     loc_4_047C
        or     al, 0xc0
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        or     eax, 0xc0000000
        push   eax
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_045E:
        mov    eax, edi
        dec    ebx
        or     al, 0xe0
        push   eax
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 4]
        call   sub_1_1A3D
loc_4_046F:
        and    eax, 0xff000000
        or     ebx, eax
        push   ebx
        push   dword ptr [ebp - 8]
        jmp    loc_4_04B2
loc_4_047C:
        or     al, 0xe0
        dec    ebx
        push   eax
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 4]
        call   sub_1_1A3D
loc_4_048B:
        and    eax, 0xff000000
        or     ebx, eax
        push   ebx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_049E:
        mov    eax, edi
        or     al, 0xc0
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0x10]
        dec    eax
        or     eax, 0xc0000000
        push   eax
        push   dword ptr [ebp - 0xc]
loc_4_04B2:
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_04BA:
        mov    ebx, edi
        push   -1
        or     bl, 0x60
        or     edi, 0x20
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_04CD:
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_1_1A3D
loc_4_04D6:
        mov    ecx, 0xac44
        sub    edx, edx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0x14]
        shl    eax, 0xe
        div    ecx
        shl    eax, 0x10
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0xffff
        or     eax, ecx
        and    al, 0x80
        or     al, 0x80
        push   eax
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_0504:
        mov    eax, dword ptr [esi + 0x34]
        push   ebx
        push   dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        call   sub_1_1A3D
loc_4_0513:
        mov    ecx, dword ptr [ebp - 8]
        and    eax, 0xffff
        shl    ecx, 0x10
        or     ecx, eax
        push   ecx
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_052A:
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_1_1A3D
loc_4_0533:
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 4]
        mov    ebx, eax
        call   sub_1_1A3D
loc_4_0540:
        mov    ecx, dword ptr [esi + 0x3c]
        and    bl, 0
        shr    ecx, 8
        mov    edx, dword ptr [esi + 0x38]
        shl    edx, 0x10
        or     ecx, ebx
        mov    esi, eax
        push   ecx
        xor    esi, edx
        push   edi
        and    esi, 0xffffff
        xor    esi, edx
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_0567:
        push   esi
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 4]
        call   sub_1_1A14
loc_4_0573:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_4_057E:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 0x14]
        mov    esi, dword ptr [eax + 0x18]
        mov    edi, esi
        mov    ebx, dword ptr [ecx + 0x1c]
        or     edi, 0x20
        push   edi
        push   ebx
        call   sub_1_1A3D
loc_4_059D:
        and    eax, 0xffff
        push   eax
        push   edi
        push   ebx
        call   sub_1_1A14
loc_4_05AA:
        push   0
        push   esi
        push   ebx
        call   sub_1_1A14
loc_4_05B3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_4_05BB:
        push   ebp
        mov    ecx, 0x10
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0x10]
        mov    edi, 0                        ; -> obj1:0x0010
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, dword ptr [ebp + 8]
        jne    loc_4_05DF
        mov    dword ptr [edx], eax
loc_4_05DF:
        mov    edi, 0                        ; -> loc_1_0200
        mov    ecx, 0x10
        mov    esi, dword ptr [ebp + 0xc]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_05F5
        add    eax, 4
        mov    dword ptr [edx], eax
loc_4_05F5:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     loc_4_0605
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
        jmp    loc_4_060A
loc_4_0605:
        mov    eax, 0x80004002
loc_4_060A:
        pop    edi
        pop    esi
        pop    ebp
        ret    0xc
loc_4_0610:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x10]
        inc    eax
        mov    dword ptr [ecx + 0x10], eax
        ret    4
loc_4_061E:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x10]
        dec    eax
        mov    dword ptr [ecx + 0x10], eax
        ret    4
loc_4_062C:
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret    0xc
loc_4_0643:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        ret    4
loc_4_0653:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        ret    4
loc_4_0663:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [eax + 8]
        mov    esi, dword ptr [esp + 0x18]
        mov    ebp, 0xff
        mov    dword ptr [esi], 4
        call   sub_4_0BD4
        push   ebp
        lea    ecx, [esi + 4]
        push   eax
        lea    edi, [esi + 0x104]
        push   ecx
        call   sub_4_4D64
        add    esp, 0xc
        push   ebp
        push   0                             ; -> obj5:0x0074
        push   edi
        call   sub_4_4D64
        add    esp, 0xc
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [esi + 0x210], 1
        mov    dword ptr [esi + 0x218], 0x400000
        mov    word ptr [esi + 0x208], 0x32a4
        mov    dword ptr [esi + 0x204], eax
        mov    ecx, dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esi + 0x220], 0x20
        add    eax, eax
        add    eax, 0x3fffff
        mov    dword ptr [esi + 0x21c], eax
        mov    eax, 0xb3
        cmp    byte ptr [edi], 0
        je     loc_4_06FD
loc_4_06F2:
        movsx  ecx, byte ptr [edi]
        add    eax, ecx
        inc    edi
        cmp    byte ptr [edi], 0
        jne    loc_4_06F2
loc_4_06FD:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 0x20a], dx
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_0716:
        xor    eax, eax
        ret    4
loc_4_071B:
        xor    eax, eax
        ret    4
loc_4_0720:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    eax, dword ptr [eax + 8]
        xor    edi, edi
        mov    ecx, 0x20
        mov    dword ptr [esi], 0x910
        mov    dword ptr [esi + 0xc], edi
        mov    dword ptr [esi + 0x10], ecx
        mov    edx, ecx
        mov    dword ptr [esi + 0x14], ecx
        mov    dword ptr [esi + 4], 0x64
        mov    dword ptr [esi + 8], 0x186a0
        mov    dword ptr [esi + 0x18], edi
        sub    edx, dword ptr [eax + 0x20]
        mov    dword ptr [esi + 0x1c], edx
        sub    ecx, dword ptr [eax + 0x20]
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [esi + 0x24], edi
        mov    dword ptr [esi + 0x28], edi
        mov    dword ptr [esi + 0x2c], edi
        mov    dword ptr [esi + 0x30], edi
        mov    dword ptr [esi + 0x34], edi
        mov    dword ptr [esi + 0x38], edi
        mov    dword ptr [esi + 0x3c], edi
        mov    eax, dword ptr [eax + 0x1c]
        mov    eax, dword ptr [eax + 8]
        add    eax, eax
        mov    dword ptr [esi + 0x44], edi
        mov    dword ptr [esi + 0x40], eax
        mov    dword ptr [esi + 0x48], edi
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_4_078F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    ecx, dword ptr [ebp + 0x20]
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 8]
        push   0x201
        push   0x40
        mov    dword ptr [ecx], 0
        call   sub_4_4D6C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_4_07C7
        mov    eax, 0x8007000e
        jmp    loc_4_0908
loc_4_07C7:
        mov    eax, 0xffff
        mov    dword ptr [esi], 0            ; -> obj5:0x0000
        mov    dword ptr [esi + 4], 0        ; -> obj5:0x0010
        mov    dword ptr [esi + 8], esi
        push   dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0xc], esi
        mov    dword ptr [esi + 0x14], edi
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [esi + 0x3c], eax
        mov    ebx, dword ptr [edi + 0x1c]
        call   sub_4_0036
        add    esp, 4
        test   eax, eax
        jne    loc_4_0812
        push   0
        push   esi
        call   sub_4_4D5C
        add    esp, 8
        mov    eax, 0x88780064
        jmp    loc_4_0908
loc_4_0812:
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 0x18]
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x30], ecx
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 0x28], eax
        lea    eax, [ebp - 4]
        mov    edx, dword ptr [ecx]
        push   eax
        push   ebx
        mov    dword ptr [esi + 0x2c], edx
        call   sub_1_0220
loc_4_0838:
        add    esp, 8
        test   eax, eax
        je     loc_4_0854
        push   0
        push   esi
        call   sub_4_4D5C
        add    esp, 8
        mov    eax, 0x8878000a
        jmp    loc_4_0908
loc_4_0854:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        push   0x80
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x2a0
        push   eax
        push   ebx
        call   sub_1_19AA
loc_4_087A:
        push   0xffff
        mov    eax, dword ptr [ebp - 4]
        or     al, 0x60
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_088B:
        push   0
        mov    eax, dword ptr [ebp - 4]
        or     al, 0x20
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_0899:
        push   0
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_1_1A14
loc_4_08A4:
        mov    eax, dword ptr [esi + 0x30]
        shr    eax, 1
        dec    eax
        or     eax, 0xc0000000
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     al, 0xc0
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_08BC:
        mov    eax, dword ptr [ebp - 4]
        or     al, 0xe0
        push   eax
        push   ebx
        call   sub_1_1A3D
loc_4_08C8:
        mov    ecx, dword ptr [esi + 0x30]
        and    eax, 0xff000000
        shr    ecx, 1
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     al, 0xe0
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_08E1:
        mov    eax, dword ptr [esi + 0x30]
        shr    eax, 1
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     ah, 2
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_08F5:
        lea    edx, [esi + 4]
        mov    ecx, dword ptr [ebp + 0x20]
        push   esi
        inc    dword ptr [edi + 0x20]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [esi]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_4_0908:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x1c
loc_4_0911:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   edi
        mov    dword ptr [ebp - 8], ecx
        mov    edi, dword ptr [edx + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        push   0x201
        push   0x40
        mov    dword ptr [ecx], 0
        call   sub_4_4D6C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_4_0952
        mov    eax, 0x8007000e
        jmp    loc_4_0A79
loc_4_0952:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi], 0            ; -> obj5:0x0000
        mov    dword ptr [esi + 4], 0        ; -> obj5:0x0010
        mov    dword ptr [esi + 8], esi
        mov    dword ptr [esi + 0xc], esi
        mov    dword ptr [esi + 0x14], eax
        mov    eax, 0xffff
        mov    dword ptr [esi + 0x34], eax
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [esi + 0x3c], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [edi + 0x1c]
        mov    ebx, dword ptr [eax + 0x1c]
        mov    dword ptr [esi + 0x1c], ecx
        mov    eax, dword ptr [edi + 0x30]
        mov    dword ptr [esi + 0x30], eax
        mov    edx, dword ptr [edi + 0x28]
        mov    dword ptr [esi + 0x28], edx
        lea    edx, [ebp - 4]
        mov    ecx, dword ptr [edi + 0x2c]
        push   edx
        push   ebx
        mov    dword ptr [esi + 0x2c], ecx
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [esi + 0x20], eax
        call   sub_1_0220
loc_4_09A7:
        add    esp, 8
        test   eax, eax
        je     loc_4_09C3
        push   0
        push   esi
        call   sub_4_4D5C
        add    esp, 8
        mov    eax, 0x8878000a
        jmp    loc_4_0A79
loc_4_09C3:
        mov    eax, dword ptr [ebp - 4]
        push   0x80
        mov    dword ptr [esi + 0x18], eax
        mov    ecx, dword ptr [esi + 0x20]
        mov    dword ptr [esi + 0x24], ecx
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x2a0
        push   eax
        push   ebx
        call   sub_1_19AA
loc_4_09E3:
        push   0xffff
        mov    eax, dword ptr [ebp - 4]
        or     al, 0x60
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_09F4:
        push   0
        mov    eax, dword ptr [ebp - 4]
        or     al, 0x20
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_0A02:
        push   0
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_1_1A14
loc_4_0A0D:
        mov    eax, dword ptr [esi + 0x30]
        shr    eax, 1
        dec    eax
        or     eax, 0xc0000000
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     al, 0xc0
        mov    edi, 0xe0
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_0A2A:
        mov    eax, dword ptr [ebp - 4]
        or     eax, edi
        push   eax
        push   ebx
        call   sub_1_1A3D
loc_4_0A36:
        mov    ecx, dword ptr [esi + 0x30]
        and    eax, 0xff000000
        shr    ecx, 1
        or     ecx, eax
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        or     eax, edi
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_0A4F:
        mov    eax, dword ptr [esi + 0x30]
        shr    eax, 1
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        or     ah, 2
        push   eax
        push   ebx
        call   sub_1_1A14
loc_4_0A63:
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [esi + 4]
        mov    edx, dword ptr [ebp + 0x10]
        push   esi
        inc    dword ptr [ecx + 0x20]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esi]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_4_0A79:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_4_0A82:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   0x201
        push   0x24
        call   sub_4_4D6C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_4_0B63
        lea    edi, [esi + 4]
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi], 0            ; -> obj5:0x0040
        push   1
        mov    dword ptr [edi], 0            ; -> obj5:0x0050
        push   0                             ; -> obj5:0x0080
        lea    ecx, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 0x18]
        push   edx
        mov    dword ptr [esi + 8], esi
        mov    dword ptr [esi + 0xc], esi
        mov    dword ptr [esi + 0x14], ebx
        mov    dword ptr [esi + 0x1c], eax
        call   sub_4_4D88
        test   eax, eax
        je     loc_4_0AE1
        push   0
        jmp    loc_4_0B5A
loc_4_0AE1:
        xor    eax, eax
        VxDCall  Dev_32A5.Service_0000h          ; cd200000a532
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0x414
        mov    eax, dword ptr [ebp - 4]
        jae    loc_4_0B09
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_4D74
        push   0
        jmp    loc_4_0B5A
loc_4_0B09:
        push   esi
        mov    dword ptr [esi + 0x18], eax
        push   ebx
        call   sub_4_001B
        add    esp, 8
        mov    dword ptr [ebp - 0xc], edi
        mov    dword ptr [ebp - 0x10], 0
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VxDCall  Dev_32A5.Service_0001h          ; cd200100a532
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        xor    edi, edi
        cmp    dword ptr [ebp - 8], edi
        je     loc_4_0B63
        push   edi
        push   ebx
        call   sub_4_001B
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_4D74
        push   edi
loc_4_0B5A:
        push   esi
        call   sub_4_4D5C
        add    esp, 8
loc_4_0B63:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_0B6A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        mov    esi, eax
        xor    eax, eax
        test   esi, esi
        je     loc_4_0BCD
        mov    dword ptr [ebp - 8], eax
        lea    eax, [esi + 4]
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        VxDCall  Dev_32A5.Service_0002h          ; cd200200a532
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_4_001B
        add    esp, 8
        mov    eax, dword ptr [esi + 0x18]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_4_4D74
        push   0
        push   esi
        call   sub_4_4D5C
        add    esp, 8
loc_4_0BCD:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
sub_4_0BD4:
        movzx  eax, word ptr [0xc]           ; -> obj2:0x000C
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_4_0BE2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [0], 0              ; -> sub_1_0480
        mov    dword ptr [ebp - 4], 0        ; -> sub_1_0480
        mov    edx, dword ptr [ebp - 4]
        mov    edx, dword ptr [edx]
        mov    ecx, 0x4354454d
        xor    eax, eax
        VxDCall  Dev_32A5.Service_0000h          ; cd200000a532
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_4_0C1E
        xor    eax, eax
        cmp    ecx, 0x454d4354
        jne    loc_4_0C1E
        mov    eax, edx
loc_4_0C1E:
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx], eax
        xor    eax, eax
        cmp    dword ptr [ebp - 8], eax
        je     loc_4_0C41
        cmp    dword ptr [0], eax            ; -> sub_1_0480
        je     loc_4_0C41
        push   eax
        push   eax
        push   eax
        call   sub_1_1375
loc_4_0C3A:
        add    esp, 0xc
        test   eax, eax
        je     loc_4_0C46
loc_4_0C41:
        call   sub_1_14F8
loc_4_0C46:
        call   sub_3_0186
loc_4_0C4B:
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_0C4F:
        cmp    dword ptr [0], 0              ; -> obj1:0x0458
        jne    loc_4_0C6B
        cmp    dword ptr [0], 0              ; -> obj1:0x045C
        jne    loc_4_0C6B
        xor    eax, eax
        inc    dword ptr [0]                 ; -> obj1:0x045C
        jmp    loc_4_0C70
loc_4_0C6B:
        mov    eax, 1
loc_4_0C70:
        ret    
sub_4_0C71:
        xor    eax, eax
        cmp    dword ptr [0], eax            ; -> sub_1_0480
        je     loc_4_0C8E
        push   eax
        push   eax
        push   1
        mov    eax, dword ptr [0]            ; -> loc_1_047C
        push   eax
        call   sub_1_0480
        add    esp, 0x10
loc_4_0C8E:
        cmp    dword ptr [0], 0              ; -> obj1:0x0418
        je     loc_4_0CC0
        mov    eax, dword ptr [0]            ; -> obj1:0x0418
        push   eax
        call   sub_1_5164
loc_4_0CA2:
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0x0418
        push   dword ptr [eax + 8]
        call   sub_3_0063
loc_4_0CB2:
        add    esp, 4
        test   eax, eax
        jne    loc_4_0C8E
        mov    eax, 1
        jmp    loc_4_0CC2
loc_4_0CC0:
        xor    eax, eax
loc_4_0CC2:
        ret    
sub_4_0CC3:
        sub    esp, 0x18
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x24]
        push   edi
        push   ebp
        push   esi
        call   sub_1_51D9
loc_4_0CD4:
        add    esp, 4
        lea    edx, [esi + 0x24]
        xor    ecx, ecx
loc_4_0CDC:
        mov    di, word ptr [edx]
        xor    eax, eax
loc_4_0CE1:
        lea    ebx, [eax + edi]
        lea    ebp, [eax + ecx]
        mov    word ptr [esp + ebp*2 + 0x10], bx
        inc    eax
        cmp    eax, 4
        jl     loc_4_0CE1
        add    edx, 2
        add    ecx, 4
        cmp    ecx, 0xc
        jl     loc_4_0CDC
        lea    eax, [esp + 0x10]
        push   eax
        push   0xc
        push   dword ptr [esi]
        call   sub_4_0DCB
        add    esp, 0xc
        xor    eax, eax
        mov    dword ptr [esi], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
sub_4_0D1E:
        sub    esp, 0x18
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x24]
        push   edi
        push   ebp
        push   esi
        call   sub_1_5164
loc_4_0D2F:
        add    esp, 4
        lea    ecx, [esi + 0x24]
        xor    eax, eax
loc_4_0D37:
        mov    dx, word ptr [ecx]
        xor    edi, edi
loc_4_0D3C:
        lea    ebx, [edx + edi]
        lea    ebp, [eax + edi]
        mov    word ptr [esp + ebp*2 + 0x10], bx
        inc    edi
        cmp    edi, 4
        jl     loc_4_0D3C
        add    ecx, 2
        add    eax, 4
        cmp    eax, 0xc
        jl     loc_4_0D37
        lea    eax, [esp + 0x10]
        push   eax
        push   0xc
        push   dword ptr [esi]
        call   sub_4_0DEE
        add    esp, 0xc
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
        db  0CCh
sub_4_0D74:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0x101
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_4_0D91:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_4_0DAB:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
loc_4_0DB7:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        inc    edi
        inc    edi
        loop   loc_4_0DB7
        xor    eax, eax
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
sub_4_0DCB:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x10]
loc_4_0DDA:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    edi
        inc    edi
        loop   loc_4_0DDA
        xor    eax, eax
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_4_0DEE:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x10]
loc_4_0DFD:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    edi
        inc    edi
        loop   loc_4_0DFD
        xor    eax, eax
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_4_0E11:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    edi, dword ptr [ebp + 0x14]
        mov    ecx, 0xa
        lea    esi, [0]                      ; -> obj1:0x0F80
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [ebp + 0x10]
        sub    eax, esi
        sub    eax, 0xa
        mov    dword ptr [esi + 6], eax
        mov    ecx, dword ptr [ebp + 8]
loc_4_0E40:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_0E57
        inc    edi
        inc    edi
        loop   loc_4_0E40
        xor    eax, eax
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_4_0E77
loc_4_0E57:
        sub    ecx, dword ptr [ebp + 8]
        neg    ecx
        jecxz  loc_4_0E6E
        mov    edi, dword ptr [ebp + 0xc]
loc_4_0E61:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        inc    edi
        inc    edi
        loop   loc_4_0E61
loc_4_0E6E:
        mov    eax, 1
        mov    dword ptr [esp + 0x1c], eax
loc_4_0E77:
        popal  
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi], ax
        clc    
        pop    esi
        pop    ebp
        ret    
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h, 0C3h, 0CCh, 0CCh, 0CCh, 0A1h
        dd  obj5:0x008C              ; fixup
        db  0C3h
loc_4_0E9E:
        call   sub_4_0C4F
        or     eax, eax
        jne    loc_4_0EB9
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_11D8
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        clc    
        ret    
loc_4_0EB9:
        stc    
        ret    
loc_4_0EBB:
        call   sub_4_0C71
        or     eax, eax
        jne    loc_4_0ED6
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_11D8
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        clc    
        ret    
loc_4_0ED6:
        stc    
        ret    
loc_4_0ED8:
        pushfd 
        pushal 
        cmp    ebx, dword ptr [0]            ; -> obj1:0x0478
        jne    loc_4_0EFD
        xor    eax, eax
        mov    dword ptr [0], eax            ; -> obj1:0x0478
        mov    edi, dword ptr [0]            ; -> obj1:0x0474
        cmp    dword ptr [esi], 0
        jne    loc_4_0EFD
        push   esi
        call   sub_1_5118
loc_4_0EFA:
        add    esp, 4
loc_4_0EFD:
        mov    esi, dword ptr [0]            ; -> obj1:0x0418
loc_4_0F03:
        or     esi, esi
        je     loc_4_0F1D
        cmp    ebx, dword ptr [esi]
        je     loc_4_0F10
        mov    esi, dword ptr [esi + 4]
        jmp    loc_4_0F03
loc_4_0F10:
        xor    eax, eax
        mov    dword ptr [esi], eax
        push   esi
        call   sub_1_5118
loc_4_0F1A:
        add    esp, 4
loc_4_0F1D:
        popal  
        popfd  
        clc    
        ret    
sub_4_0F21:
        push   ebp
        mov    ebp, esp
        push   0
        push   0
        push   0x92
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        pop    ebp
        ret    
sub_4_0F4B:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        pop    ebp
        ret    
loc_4_0F5E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        test   eax, eax
        push   esi
        push   edi
        je     loc_4_0F96
        cmp    eax, 1
        je     loc_4_0FB1
        cmp    eax, 2
        je     loc_4_0FFF
        cmp    eax, 3
        je     loc_4_1045
        cmp    eax, 4
        je     loc_4_0FFF
        cmp    eax, 0xd
        je     loc_4_0FFF
        mov    eax, 1
        jmp    loc_4_1069
loc_4_0F96:
        call   sub_3_080D
loc_4_0F9B:
        test   eax, eax
        je     loc_4_0FAA
        push   dword ptr [ebp + 0x10]
        call   sub_3_0868
loc_4_0FA7:
        add    esp, 4
loc_4_0FAA:
        xor    eax, eax
        jmp    loc_4_1069
loc_4_0FB1:
        cmp    dword ptr [ebp + 0xc], 1
        jbe    loc_4_0FC1
        mov    eax, 1
        jmp    loc_4_1069
loc_4_0FC1:
        push   0x16d5
        call   sub_4_0D74
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    loc_4_0FDE
        mov    eax, 0x13
        jmp    loc_4_1069
loc_4_0FDE:
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_3_0B0E
loc_4_0FE7:
        add    esp, 8
        mov    edi, eax
        lea    eax, [esi + 0x24]
        push   eax
        push   dword ptr [esi + 8]
        call   sub_4_0A82
        add    esp, 8
        mov    eax, edi
        jmp    loc_4_1069
loc_4_0FFF:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0B6A
        add    esp, 4
        push   dword ptr [ebp + 0x10]
        call   sub_3_0482
loc_4_1012:
        add    esp, 4
        push   dword ptr [ebp + 0x10]
        call   sub_3_0063
loc_4_101D:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_4_1041
        cmp    dword ptr [esi + 0x1558], 0x10
        jne    loc_4_1038
        push   esi
        call   sub_3_0F98
loc_4_1035:
        add    esp, 4
loc_4_1038:
        push   esi
        call   sub_4_0D91
        add    esp, 4
loc_4_1041:
        xor    eax, eax
        jmp    loc_4_1069
loc_4_1045:
        cmp    dword ptr [ebp + 0xc], 1
        mov    eax, 1
        ja     loc_4_1069
        push   dword ptr [ebp + 0x10]
        call   sub_3_00F2
loc_4_1058:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
loc_4_1069:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_4_106D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0x80
        push   edi
        push   0x2be
        push   esi
        call   sub_1_19AA
loc_4_108A:
        push   -0x20
        push   0xde
        push   esi
        call   sub_1_1A14
loc_4_1097:
        push   0xffffe8
        push   0xfe
        push   esi
        call   sub_1_1A14
loc_4_10A7:
        push   0
        push   0x3e
        push   esi
        call   sub_1_1A14
loc_4_10B1:
        push   0
        push   0x1e
        push   esi
        call   sub_1_1A14
loc_4_10BB:
        push   0xffffe3
        push   0x21e
        push   esi
        call   sub_1_1A14
loc_4_10CB:
        push   edi
        push   0x2bf
        push   esi
        call   sub_1_19AA
loc_4_10D7:
        push   0xfffff0
        mov    edi, 0xff
        push   0xdf
        push   esi
        call   sub_1_1A14
loc_4_10EC:
        push   0xfffff8
        push   edi
        push   esi
        call   sub_1_1A14
loc_4_10F8:
        push   edi
        push   0x3f
        push   esi
        call   sub_1_1A14
loc_4_1101:
        push   0x8000
        push   0x1f
        push   esi
        call   sub_1_1A14
loc_4_110E:
        push   0xfffff3
        push   0x21f
        push   esi
        call   sub_1_1A14
loc_4_111E:
        mov    cx, word ptr [esi + 4]
        add    cx, 2
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebp - 2], ax
        mov    di, word ptr [esi]
        mov    eax, dword ptr [esi + 0x152a]
        sub    eax, 2
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0xffffffd8
        lea    ebx, [ebx + 0x4828]
        pushfd 
        mov    edx, dword ptr [esp]
        test   dh, 2
        je     loc_4_1152
        cli    
loc_4_1152:
        mov    dx, cx
        mov    ax, 0x3e
        out    dx, ax
        mov    dx, di
        xor    eax, eax
        out    dx, ax
        add    di, 2
loc_4_1166:
        mov    dx, cx
        in     ax, dx
        test   ah, 0x10
        je     loc_4_1166
loc_4_1170:
        mov    dx, cx
        in     ax, dx
        test   ah, 0x10
        jne    loc_4_1170
        mov    dx, di
        mov    ax, bx
        out    dx, ax
        mov    dx, cx
        mov    ax, 0x3c
        out    dx, ax
        mov    dx, word ptr [ebp - 2]
        xor    eax, eax
        out    dx, ax
        pop    eax
        test   ah, 2
        je     loc_4_119A
        sti    
loc_4_119A:
        mov    edi, 0x8000ffff
        push   edi
        push   0x7e
        push   esi
        call   sub_1_1A14
loc_4_11A8:
        push   edi
        push   0x7f
        push   esi
        call   sub_1_1A14
loc_4_11B1:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_4_11BC:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0xc]
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        lea    esi, [ebx + 0xc]
        cmp    dword ptr [esi], edi
        jae    loc_4_11D2
        xor    eax, eax
        jmp    loc_4_122A
loc_4_11D2:
        xor    ecx, ecx
        lea    eax, [ebx + 0xaee]
loc_4_11DA:
        cmp    dword ptr [eax], 0
        je     loc_4_11E4
        cmp    dword ptr [eax + 4], edi
        jae    loc_4_11F0
loc_4_11E4:
        add    eax, 8
        inc    ecx
        cmp    ecx, 0x80
        jb     loc_4_11DA
loc_4_11F0:
        cmp    ecx, 0x80
        jne    loc_4_1201
        push   edi
        push   ebx
        call   sub_4_122E
        mov    ecx, eax
loc_4_1201:
        lea    edx, [ebx + ecx*8 + 0xaee]
        lea    ebx, [ebx + ecx*8 + 0xaf2]
        mov    eax, dword ptr [edx]
        cmp    dword ptr [ebx], edi
        jbe    loc_4_1220
        lea    ecx, [edi + eax]
        mov    dword ptr [edx], ecx
        sub    dword ptr [ebx], edi
        sub    dword ptr [esi], edi
        jmp    loc_4_122A
loc_4_1220:
        mov    dword ptr [edx], 0
        mov    edx, dword ptr [ebx]
        sub    dword ptr [esi], edx
loc_4_122A:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_122E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
loc_4_123C:
        push   0
        push   0
        push   0xff
        push   esi
        call   sub_1_1A68
loc_4_124B:
        mov    byte ptr [ebp + edi - 0x2c], al
        add    esp, 0x10
        cmp    eax, 0xff
        mov    ebx, eax
        je     loc_4_12ED
        shl    eax, 5
        cmp    byte ptr [eax + esi + 0x59], 0
        je     loc_4_126C
        dec    dword ptr [esi + 0x18]
loc_4_126C:
        push   0x80
        mov    eax, ebx
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19AA
loc_4_127F:
        push   0
        mov    eax, ebx
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_128C:
        push   0
        mov    eax, ebx
        or     al, 0x40
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_1299:
        push   0
        mov    eax, ebx
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_12A6:
        push   0
        mov    eax, ebx
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_12B3:
        push   0x40000000
        mov    eax, ebx
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_12C3:
        push   0x40000000
        push   ebx
        push   esi
        call   sub_1_1A14
loc_4_12CF:
        test   edi, 1
        je     loc_4_12DE
        push   0x4000000
        jmp    loc_4_12E3
loc_4_12DE:
        push   0x6000000
loc_4_12E3:
        or     bh, 2
        push   ebx
        push   esi
        call   sub_1_1A14
loc_4_12ED:
        inc    edi
        cmp    edi, 0xa
        jb     loc_4_123C
        xor    ecx, ecx
        mov    edx, 0xffffffff
        xor    edi, edi
        lea    eax, [esi + 0xaee]
        mov    dword ptr [ebp - 0x1c], ecx
        mov    dword ptr [ebp - 8], ecx
loc_4_130C:
        mov    ebx, dword ptr [eax]
        test   ebx, ebx
        je     loc_4_131E
        cmp    edx, ebx
        jbe    loc_4_131E
        mov    edx, ebx
        mov    dword ptr [ebp - 0x1c], edi
        mov    dword ptr [ebp - 8], eax
loc_4_131E:
        add    eax, 8
        inc    edi
        cmp    edi, 0x80
        jb     loc_4_130C
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 4
        mov    dword ptr [ebp - 4], ecx
        mov    edi, dword ptr [ecx]
        add    edi, edx
loc_4_1337:
        mov    dword ptr [ebp - 0x20], 0
        lea    eax, [esi + 0x662]
        mov    dword ptr [ebp - 0x14], eax
loc_4_1347:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        test   eax, eax
        je     loc_4_1468
        mov    ebx, dword ptr [eax + 0x18]
        cmp    dword ptr [ebx + 0x1c], edi
        ja     loc_4_1468
        movzx  eax, word ptr [eax + 0x2a]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        imul   eax, eax, 0x24
        add    eax, ebx
        mov    edx, dword ptr [eax - 4]
        add    edx, dword ptr [eax - 8]
        cmp    edx, dword ptr [ecx]
        jb     loc_4_1468
        mov    dword ptr [ebp - 0x18], 0
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_4_1468
loc_4_138E:
        cmp    dword ptr [ebx + 0x1c], edi
        jne    loc_4_1456
        cmp    dword ptr [ebx + 0x20], 0
        je     loc_4_1456
        or     byte ptr [ebx + 3], 0x80
        push   dword ptr [ebx + 0x1c]
        push   0x234
        mov    eax, dword ptr [ebx + 0x20]
        push   esi
        mov    dword ptr [ebp - 0x10], eax
        call   sub_1_1A14
loc_4_13B9:
        mov    ecx, dword ptr [ebp - 8]
        push   dword ptr [ecx]
        push   0x236
        push   esi
        call   sub_1_1A14
loc_4_13C9:
        push   0x23a
        push   esi
        call   sub_1_19DF
loc_4_13D4:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [ebp - 0x10]
        test   eax, eax
        je     loc_4_1402
loc_4_13DE:
        push   0x23a
        push   esi
        call   sub_1_19DF
loc_4_13E9:
        movzx  eax, ax
        push   eax
        push   0x23a
        push   esi
        call   sub_1_19AA
loc_4_13F8:
        mov    ecx, dword ptr [ebp - 0x10]
        dec    dword ptr [ebp - 0x10]
        test   ecx, ecx
        jne    loc_4_13DE
loc_4_1402:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        sub    dword ptr [ebx], eax
        mov    eax, dword ptr [ecx]
        sub    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ecx]
        sub    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [ecx]
        mov    ecx, dword ptr [ebp - 0x14]
        sub    dword ptr [ebx + 0xc], eax
        mov    edx, dword ptr [ecx]
        add    edx, 0x40
        mov    eax, dword ptr [edx]
        cmp    dword ptr [ebx + 0x1c], eax
        jne    loc_4_142E
        mov    ecx, dword ptr [ebp - 4]
        sub    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
loc_4_142E:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        mov    ecx, dword ptr [ebp - 8]
        sub    dword ptr [ebx + 0x1c], eax
        mov    eax, dword ptr [ebp - 0x18]
        and    byte ptr [ebx + 3], 0
        mov    edx, dword ptr [ebx + 0x20]
        sub    eax, dword ptr [ebp - 0xc]
        add    dword ptr [ecx], edx
        add    edi, dword ptr [ebx + 0x20]
        cmp    eax, -1
        je     loc_4_147E
        cmp    dword ptr [ebx + 0x44], 0
        je     loc_4_147E
loc_4_1456:
        add    ebx, 0x24
        mov    eax, dword ptr [ebp - 0xc]
        inc    dword ptr [ebp - 0x18]
        cmp    dword ptr [ebp - 0x18], eax
        jb     loc_4_138E
loc_4_1468:
        add    dword ptr [ebp - 0x14], 4
        inc    dword ptr [ebp - 0x20]
        cmp    dword ptr [ebp - 0x20], 0x120
        jb     loc_4_1347
        jmp    loc_4_14BF
loc_4_147E:
        xor    edx, edx
        lea    eax, [esi + 0xaee]
loc_4_1486:
        cmp    dword ptr [eax], edi
        je     loc_4_149B
        add    eax, 8
        inc    edx
        cmp    edx, 0x80
        jb     loc_4_1486
        jmp    loc_4_1337
loc_4_149B:
        mov    eax, dword ptr [esi + edx*8 + 0xaf2]
        mov    ecx, dword ptr [ebp - 4]
        add    edi, eax
        add    dword ptr [ecx], eax
        mov    dword ptr [esi + edx*8 + 0xaee], 0
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx], eax
        jb     loc_4_1337
loc_4_14BF:
        xor    edi, edi
loc_4_14C1:
        movzx  ebx, byte ptr [ebp + edi - 0x2c]
        cmp    ebx, 0xff
        je     loc_4_151F
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_14DC:
        push   0x1d8
        mov    eax, ebx
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_14EC:
        push   0x1e0
        mov    eax, ebx
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_14FC:
        push   0x48000000
        mov    eax, ebx
        shl    ebx, 5
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_150F:
        lea    eax, [ebx + esi]
        mov    word ptr [eax + 0x42], 0xffe
        mov    byte ptr [eax + 0x59], 1
        inc    dword ptr [esi + 0x18]
loc_4_151F:
        inc    edi
        cmp    edi, 0xa
        jb     loc_4_14C1
        mov    eax, dword ptr [ebp - 0x1c]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_4_1531:
        mov    eax, dword ptr [esp + 0xc]
        sub    esp, 4
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     loc_4_161E
        mov    edi, dword ptr [esp + 0x1c]
        cmp    edi, 0x200000
        jb     loc_4_161E
        cmp    edi, 0x1000000
        jae    loc_4_161E
        lea    ecx, [edi + eax]
        mov    esi, dword ptr [esp + 0x18]
        xor    ebx, ebx
        mov    dword ptr [esp + 0x10], ecx
        lea    edx, [esi + 0xaee]
        add    dword ptr [esi + 0xc], eax
loc_4_1576:
        xor    eax, eax
        mov    ecx, edx
loc_4_157A:
        mov    ebp, dword ptr [ecx]
        cmp    dword ptr [esp + 0x10], ebp
        je     loc_4_1626
        mov    ebp, dword ptr [ecx + 4]
        add    ebp, dword ptr [ecx]
        cmp    ebp, edi
        je     loc_4_163D
        add    ecx, 8
        inc    eax
        cmp    eax, 0x80
        jb     loc_4_157A
        xor    eax, eax
loc_4_15A0:
        cmp    dword ptr [edx], 0
        je     loc_4_15B0
        add    edx, 8
        inc    eax
        cmp    eax, 0x80
        jb     loc_4_15A0
loc_4_15B0:
        cmp    eax, 0x80
        je     loc_4_15CB
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [esi + eax*8 + 0xaee], edi
        sub    ecx, edi
        mov    dword ptr [esi + eax*8 + 0xaf2], ecx
loc_4_15CB:
        xor    ebx, ebx
        cmp    word ptr [esi + 6], bx
        jbe    loc_4_161E
        lea    ebp, [esi + 0x42]
loc_4_15D6:
        movzx  eax, word ptr [ebp]
        and    al, 0
        cmp    eax, 0xff00
        je     loc_4_1612
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A3D
loc_4_15EF:
        cmp    eax, edi
        jb     loc_4_1612
        cmp    eax, dword ptr [esp + 0x10]
        jae    loc_4_1612
        push   0x807f
        mov    eax, ebx
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19AA
loc_4_160C:
        mov    word ptr [ebp], 0xffe
loc_4_1612:
        add    ebp, 0x20
        inc    ebx
        movzx  eax, word ptr [esi + 6]
        cmp    eax, ebx
        ja     loc_4_15D6
loc_4_161E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_4_1626:
        mov    ecx, dword ptr [esi + eax*8 + 0xaf2]
        add    dword ptr [esp + 0x10], ecx
        mov    dword ptr [esi + eax*8 + 0xaee], ebx
        jmp    loc_4_1576
loc_4_163D:
        lea    eax, [esi + eax*8 + 0xaee]
        mov    edi, dword ptr [eax]
        mov    dword ptr [eax], ebx
        jmp    loc_4_1576
sub_4_164D:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        push   ebp
        movzx  eax, word ptr [edi]
        mov    eax, dword ptr [esi + eax*4 + 0x662]
        movzx  ebx, word ptr [eax + 0x2a]
        mov    ecx, dword ptr [eax + 0x18]
        mov    eax, ebx
        dec    ebx
        test   eax, eax
        je     loc_4_1698
        mov    eax, ebx
        imul   eax, eax, 0x24
        lea    ebp, [eax + ecx + 0x1c]
loc_4_167A:
        mov    eax, dword ptr [ebp]
        test   eax, eax
        je     loc_4_1698
        push   dword ptr [ebp + 4]
        push   eax
        push   esi
        sub    ebp, 0x24
        call   sub_4_1531
        add    esp, 0xc
        mov    ecx, ebx
        dec    ebx
        test   ecx, ecx
        jne    loc_4_167A
loc_4_1698:
        xor    eax, eax
        pop    ebp
        movzx  ecx, word ptr [edi]
        pop    edi
        mov    dword ptr [esi + ecx*4 + 0x662], 0
        pop    esi
        pop    ebx
        ret    8
sub_4_16AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x3c
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 0x2c], esi
        mov    dword ptr [ebp - 0x20], esi
        mov    dword ptr [ebp - 0x18], esi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x14], esi
        mov    dword ptr [ebp - 0x1c], esi
        mov    dword ptr [ebp - 0x10], esi
        mov    eax, dword ptr [ecx + 0x3c]
        test   eax, eax
        je     loc_4_1704
        add    eax, 0x10
        push   eax
        push   ecx
        call   sub_4_11BC
        add    esp, 8
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, esi
        mov    dword ptr [ecx + 0x38], eax
        jne    loc_4_16FA
        mov    eax, 2
        jmp    loc_4_1E09
loc_4_16FA:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x3c], 0
loc_4_1704:
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x10]
        add    ecx, 0x40
        lea    ebx, [edi + 0x4c]
        mov    ax, word ptr [ecx]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edi + 0x48], ax
        mov    dword ptr [ebp - 0x34], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edx + 0x20]
        add    ecx, 0x14
        mov    dword ptr [ebp - 0x38], eax
        mov    dword ptr [ebp - 8], ecx
        test   eax, eax
        je     loc_4_1B85
loc_4_1734:
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 8], 8
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    eax, edx
        mov    dword ptr [ebp - 0x28], ecx
        mov    dword ptr [ebp - 0x24], edx
        lea    ecx, [eax + 8]
        sub    dword ptr [ebp - 0x38], ecx
        js     loc_4_1BEF
        mov    ecx, dword ptr [ebp - 0x28]
        cmp    ecx, 0x67616269
        je     loc_4_17A8
        cmp    ecx, 0x67616270
        je     loc_4_17D2
        cmp    ecx, 0x6e656769
        je     loc_4_17FC
        cmp    ecx, 0x6e656770
        je     loc_4_185D
        cmp    ecx, 0x72646870
        je     loc_4_18BE
        cmp    ecx, 0x72646873
        je     loc_4_1988
        cmp    ecx, 0x74736e69
        je     loc_4_1B38
        jmp    loc_4_1B6F
loc_4_17A8:
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [edi + 0x10], ebx
        shr    ecx, 2
        mov    edi, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 0x24]
        add    ebx, dword ptr [ebp - 0x24]
        shr    eax, 2
        mov    dword ptr [ebp - 0x1c], eax
        jmp    loc_4_1B6F
loc_4_17D2:
        mov    edi, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [edi + 4], ebx
        shr    ecx, 2
        mov    edi, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 0x24]
        add    ebx, dword ptr [ebp - 0x24]
        shr    eax, 2
        mov    dword ptr [ebp - 0x20], eax
        jmp    loc_4_1B6F
loc_4_17FC:
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 0x24]
        mov    dword ptr [edi + 0x14], ebx
        shr    eax, 2
        mov    dword ptr [ebp - 0x10], eax
        test   eax, eax
        je     loc_4_1B6F
        mov    edi, eax
loc_4_1818:
        mov    al, byte ptr [esi]
        mov    ecx, dword ptr [ebp - 0x34]
        mov    byte ptr [ebx], al
        cmp    word ptr [ecx], 1
        jne    loc_4_183A
        movsx  eax, word ptr [esi + 2]
        movzx  ecx, word ptr [esi]
        push   1
        push   eax
        push   ecx
        call   sub_4_2758
        add    esp, 0xc
        jmp    loc_4_184B
loc_4_183A:
        movsx  eax, word ptr [esi + 2]
        movzx  ecx, word ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2923
        add    esp, 8
loc_4_184B:
        mov    word ptr [ebx + 1], ax
        add    esi, 4
        add    ebx, 3
        dec    edi
        jne    loc_4_1818
        jmp    loc_4_1B6F
loc_4_185D:
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 0x24]
        mov    dword ptr [edi + 8], ebx
        shr    eax, 2
        mov    dword ptr [ebp - 0x18], eax
        test   eax, eax
        je     loc_4_1B6F
        mov    edi, eax
loc_4_1879:
        mov    al, byte ptr [esi]
        mov    ecx, dword ptr [ebp - 0x34]
        mov    byte ptr [ebx], al
        cmp    word ptr [ecx], 1
        jne    loc_4_189B
        movsx  eax, word ptr [esi + 2]
        movzx  ecx, word ptr [esi]
        push   1
        push   eax
        push   ecx
        call   sub_4_2758
        add    esp, 0xc
        jmp    loc_4_18AC
loc_4_189B:
        movsx  eax, word ptr [esi + 2]
        movzx  ecx, word ptr [esi]
        push   eax
        push   ecx
        call   sub_4_2A95
        add    esp, 8
loc_4_18AC:
        mov    word ptr [ebx + 1], ax
        add    esi, 4
        add    ebx, 3
        dec    edi
        jne    loc_4_1879
        jmp    loc_4_1B6F
loc_4_18BE:
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 0x10]
        sub    edx, edx
        mov    ecx, 0x26
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [edi], ebx
        mov    word ptr [edi + 0x24], 0xffff
        mov    word ptr [edi + 0x26], 0xffff
        mov    eax, dword ptr [ebp - 0x24]
        div    ecx
        mov    dword ptr [ebp - 0x2c], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0x2c]
        dec    ecx
        mov    dword ptr [ebp - 0x30], ecx
        cmp    ecx, eax
        je     loc_4_196C
        mov    edx, 0x80
loc_4_18F6:
        mov    word ptr [ebx], 1
        mov    ecx, dword ptr [ebp - 0xc]
        mov    cx, word ptr [ecx + 0x16]
        mov    word ptr [ebp - 0x3c], cx
        cmp    cx, dx
        mov    ecx, dword ptr [ebp + 0xc]
        jne    loc_4_1935
        mov    cl, byte ptr [ecx]
        mov    esi, dword ptr [ebp - 0xc]
        mov    byte ptr [ebx + 3], cl
        mov    cx, word ptr [esi + 0x14]
        or     cl, dl
        mov    byte ptr [ebx + 2], cl
        cmp    cl, dl
        jne    loc_4_1947
        mov    edi, dword ptr [ebp + 0x10]
        movzx  cx, byte ptr [ebx + 3]
        mov    word ptr [edi + 0x24], cx
        mov    word ptr [edi + 0x26], ax
        jmp    loc_4_1947
loc_4_1935:
        mov    cx, word ptr [ecx]
        mov    esi, dword ptr [ebp - 0xc]
        add    cl, byte ptr [ebp - 0x3c]
        mov    byte ptr [ebx + 3], cl
        mov    cl, byte ptr [esi + 0x14]
        mov    byte ptr [ebx + 2], cl
loc_4_1947:
        lea    edi, [ebx + 6]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    cx, word ptr [ecx + 0x18]
        mov    esi, dword ptr [ebp - 0xc]
        mov    word ptr [ebx + 4], cx
        add    ebx, 0x1a
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        add    dword ptr [ebp - 0xc], 0x26
        inc    eax
        cmp    dword ptr [ebp - 0x30], eax
        ja     loc_4_18F6
loc_4_196C:
        mov    word ptr [ebx], 0xffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ebx + 3], 0xff
        mov    ax, word ptr [ecx + 0x18]
        add    ebx, 0x1a
        mov    word ptr [ebx - 0x16], ax
        jmp    loc_4_1B6F
loc_4_1988:
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [edi + 0x18], ebx
        cmp    word ptr [eax], 1
        jne    loc_4_1A03
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x1524], 0
        je     loc_4_19B9
        cmp    dword ptr [edx], 0x100
        jae    loc_4_1BF9
loc_4_19B9:
        mov    eax, dword ptr [ebp - 0x24]
        mov    edi, dword ptr [ebp + 0x10]
        shr    eax, 4
        mov    word ptr [edi + 0x2a], ax
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_4_1A94
        mov    ecx, eax
        xor    esi, esi
loc_4_19D5:
        mov    eax, dword ptr [edx]
        add    edx, 0x10
        mov    dword ptr [ebx], eax
        add    ebx, 0x24
        mov    edi, dword ptr [edx - 0xc]
        dec    ecx
        mov    dword ptr [ebx - 0x20], edi
        mov    eax, dword ptr [edx - 8]
        mov    dword ptr [ebx - 0x1c], eax
        mov    edi, dword ptr [edx - 4]
        mov    dword ptr [ebx - 0x18], edi
        mov    dword ptr [ebx - 8], esi
        mov    dword ptr [ebx - 4], esi
        mov    word ptr [ebx - 0xa], si
        jne    loc_4_19D5
        jmp    loc_4_1A94
loc_4_1A03:
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0x1524], 0
        je     loc_4_1A1F
        cmp    dword ptr [esi + 0x14], 0x100
        jae    loc_4_1C03
loc_4_1A1F:
        mov    eax, dword ptr [ebp - 0x24]
        sub    edx, edx
        mov    ecx, 0x2e
        mov    edi, dword ptr [ebp + 0x10]
        div    ecx
        mov    word ptr [edi + 0x2a], ax
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_4_1A94
        xor    edi, edi
        mov    dword ptr [ebp - 0x30], eax
loc_4_1A3E:
        mov    eax, dword ptr [esi + 0x14]
        add    esi, 0x2e
        mov    dword ptr [ebx], eax
        add    ebx, 0x24
        mov    ecx, dword ptr [esi - 0x16]
        mov    dword ptr [ebx - 0x20], ecx
        mov    edx, dword ptr [esi - 0x12]
        mov    dword ptr [ebx - 0x1c], edx
        mov    eax, dword ptr [esi - 0xe]
        mov    dword ptr [ebx - 0x18], eax
        mov    dword ptr [ebx - 8], edi
        mov    dword ptr [ebx - 4], edi
        mov    word ptr [ebx - 0xa], di
        push   dword ptr [esi - 0xa]
        call   sub_4_26D3
        add    esp, 4
        mov    dword ptr [ebx - 0xe], eax
        mov    cl, byte ptr [esi - 6]
        dec    dword ptr [ebp - 0x30]
        mov    byte ptr [ebx - 0x10], cl
        mov    dl, byte ptr [esi - 5]
        mov    byte ptr [ebx - 0xf], dl
        mov    ax, word ptr [esi - 4]
        mov    word ptr [ebx - 0x14], ax
        mov    cx, word ptr [esi - 2]
        mov    word ptr [ebx - 0x12], cx
        jne    loc_4_1A3E
loc_4_1A94:
        mov    edi, dword ptr [ebp + 0x10]
        xor    eax, eax
        cmp    dword ptr [ebp - 4], eax
        mov    esi, dword ptr [edi + 0x18]
        je     loc_4_1B6F
loc_4_1AA5:
        cmp    dword ptr [esi], 0x100
        jb     loc_4_1ABB
        inc    eax
        add    esi, 0x24
        cmp    dword ptr [ebp - 4], eax
        ja     loc_4_1AA5
        jmp    loc_4_1B6F
loc_4_1ABB:
        mov    edx, dword ptr [ebp + 8]
        add    edx, 0x38
        mov    ecx, dword ptr [edx]
        cmp    ecx, 0x1000000
        jb     loc_4_1ACE
        add    ecx, 0x10
loc_4_1ACE:
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_4_1B1C
        mov    edi, dword ptr [ebp - 4]
        sub    edi, eax
        mov    dword ptr [ebp - 0x30], edi
loc_4_1ADB:
        mov    dword ptr [esi + 0x1c], ecx
        cmp    dword ptr [esi + 4], 0
        je     loc_4_1B14
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebp - 0xc], esi
        add    eax, 0x10
        add    dword ptr [esi], eax
        mov    eax, dword ptr [edx]
        add    eax, 0x10
        add    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edx]
        add    eax, 0x10
        add    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [edx]
        mov    edi, dword ptr [esi + 4]
        lea    eax, [edi + eax + 0x10]
        mov    dword ptr [esi + 4], eax
        sub    eax, ecx
        inc    eax
        mov    dword ptr [esi + 0x20], eax
        mov    ecx, dword ptr [esi + 4]
        inc    ecx
loc_4_1B14:
        add    esi, 0x24
        dec    dword ptr [ebp - 0x30]
        jne    loc_4_1ADB
loc_4_1B1C:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_4_1B6F
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ecx + 0x1c]
        sub    ecx, dword ptr [eax + 0x1c]
        add    ecx, dword ptr [edx]
        add    ecx, 0x10
        mov    dword ptr [eax + 0x20], ecx
        jmp    loc_4_1B6F
loc_4_1B38:
        mov    ecx, dword ptr [ebp - 8]
        mov    esi, 0x16
        mov    eax, dword ptr [ebp - 0x24]
        sub    edx, edx
        div    esi
        mov    edi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x14], eax
        dec    ax
        mov    dword ptr [edi + 0xc], ebx
        cmp    dword ptr [ebp - 0x14], 0
        mov    word ptr [edi + 0x28], ax
        je     loc_4_1B6F
        mov    eax, dword ptr [ebp - 0x14]
loc_4_1B5F:
        mov    dx, word ptr [ecx + 0x14]
        add    ecx, 0x16
        mov    word ptr [ebx], dx
        add    ebx, 2
        dec    eax
        jne    loc_4_1B5F
loc_4_1B6F:
        mov    eax, dword ptr [ebp - 0x24]
        add    dword ptr [ebp - 8], eax
        test   bl, 1
        je     loc_4_1B7B
        inc    ebx
loc_4_1B7B:
        cmp    dword ptr [ebp - 0x38], 0
        jne    loc_4_1734
loc_4_1B85:
        xor    ecx, ecx
        cmp    dword ptr [ebp - 4], ecx
        je     loc_4_1DE1
        cmp    dword ptr [ebp - 0x2c], ecx
        je     loc_4_1DE1
        cmp    dword ptr [ebp - 0x20], ecx
        je     loc_4_1DE1
        cmp    dword ptr [ebp - 0x18], ecx
        je     loc_4_1DE1
        cmp    dword ptr [ebp - 0x14], ecx
        je     loc_4_1DE1
        cmp    dword ptr [ebp - 0x1c], ecx
        je     loc_4_1DE1
        cmp    dword ptr [ebp - 0x10], ecx
        je     loc_4_1DE1
loc_4_1BC6:
        mov    eax, dword ptr [ebp - 0x2c]
        dec    dword ptr [ebp - 0x2c]
        test   eax, eax
        je     loc_4_1C0D
        mov    edx, dword ptr [ebp - 0x2c]
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [edi]
        imul   edx, edx, 0x1a
        movzx  edx, word ptr [edx + eax + 4]
        cmp    edx, dword ptr [ebp - 0x20]
        jb     loc_4_1BC6
        mov    esi, 0x20
        jmp    loc_4_1D7A
loc_4_1BEF:
        mov    eax, 3
        jmp    loc_4_1E09
loc_4_1BF9:
        mov    esi, 4
        jmp    loc_4_1D7A
loc_4_1C03:
        mov    esi, 4
        jmp    loc_4_1D7A
loc_4_1C0D:
        mov    edi, dword ptr [ebp + 0x10]
        dec    dword ptr [ebp - 0x20]
        mov    eax, dword ptr [ebp - 0x20]
        mov    ecx, dword ptr [edi + 4]
        movzx  edx, word ptr [ecx + eax*4]
        cmp    edx, dword ptr [ebp - 0x18]
        ja     loc_4_1C38
loc_4_1C22:
        mov    eax, dword ptr [ebp - 0x20]
        dec    dword ptr [ebp - 0x20]
        test   eax, eax
        je     loc_4_1C42
        mov    eax, dword ptr [ebp - 0x20]
        movzx  edx, word ptr [ecx + eax*4]
        cmp    edx, dword ptr [ebp - 0x18]
        jb     loc_4_1C22
loc_4_1C38:
        mov    esi, 0x21
        jmp    loc_4_1D7A
loc_4_1C42:
        mov    eax, dword ptr [ebp - 0x18]
        dec    dword ptr [ebp - 0x18]
        test   eax, eax
        je     loc_4_1C75
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 0x18]
        imul   ecx, ecx, 3
        mov    eax, dword ptr [edi + 8]
        add    eax, ecx
        cmp    byte ptr [eax], 0x29
        jne    loc_4_1C42
        movsx  eax, word ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0x14]
        dec    ecx
        cmp    eax, ecx
        jb     loc_4_1C42
        mov    esi, 0x22
        jmp    loc_4_1D7A
loc_4_1C75:
        mov    edi, dword ptr [ebp + 0x10]
        dec    dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [edi + 0x10]
        movzx  edx, word ptr [ecx + eax*4]
        cmp    edx, dword ptr [ebp - 0x10]
        ja     loc_4_1CA0
loc_4_1C8A:
        mov    eax, dword ptr [ebp - 0x1c]
        dec    dword ptr [ebp - 0x1c]
        test   eax, eax
        je     loc_4_1CAA
        mov    eax, dword ptr [ebp - 0x1c]
        movzx  edx, word ptr [ecx + eax*4]
        cmp    edx, dword ptr [ebp - 0x10]
        jb     loc_4_1C8A
loc_4_1CA0:
        mov    esi, 0x24
        jmp    loc_4_1D7A
loc_4_1CAA:
        mov    eax, dword ptr [ebp - 0x10]
        dec    dword ptr [ebp - 0x10]
        test   eax, eax
        je     loc_4_1D04
        mov    ecx, dword ptr [ebp - 0x10]
        imul   ecx, ecx, 3
loc_4_1CBA:
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [edi + 0x14]
        add    eax, ecx
        cmp    byte ptr [eax], 0x29
        jne    loc_4_1CD7
        movsx  edx, word ptr [eax + 1]
        mov    esi, dword ptr [ebp - 0x14]
        dec    esi
        cmp    edx, esi
        jae    loc_4_1DA1
loc_4_1CD7:
        cmp    byte ptr [eax], 0x35
        jne    loc_4_1CF7
        movsx  edx, word ptr [eax + 1]
        cmp    dword ptr [ebp - 4], edx
        jbe    loc_4_1DA1
        mov    edi, dword ptr [ebp + 0x10]
        imul   edx, edx, 0x24
        mov    eax, dword ptr [edi + 0x18]
        inc    word ptr [eax + edx + 0x1a]
loc_4_1CF7:
        mov    eax, dword ptr [ebp - 0x10]
        sub    ecx, 3
        dec    dword ptr [ebp - 0x10]
        test   eax, eax
        jne    loc_4_1CBA
loc_4_1D04:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    word ptr [ecx], 0x80
        ja     loc_4_1D59
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0x1c]
        mov    eax, dword ptr [ecx + 0x38]
        lea    ecx, [edx + eax + 0x10]
        mov    edx, dword ptr [ebp - 4]
        dec    dword ptr [ebp - 4]
        test   edx, edx
        je     loc_4_1D59
        mov    edi, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 0x18]
        imul   edx, edx, 0x24
        lea    esi, [eax + edx + 0x1c]
loc_4_1D35:
        mov    edx, dword ptr [esi]
        test   edx, edx
        je     loc_4_1D59
        mov    eax, dword ptr [esi - 0x18]
        test   eax, eax
        je     loc_4_1D4C
        cmp    ecx, edx
        jb     loc_4_1DA8
        cmp    ecx, eax
        jb     loc_4_1DA8
        mov    ecx, edx
loc_4_1D4C:
        mov    eax, dword ptr [ebp - 4]
        sub    esi, 0x24
        dec    dword ptr [ebp - 4]
        test   eax, eax
        jne    loc_4_1D35
loc_4_1D59:
        mov    eax, dword ptr [ebp - 0x14]
        dec    dword ptr [ebp - 0x14]
        test   eax, eax
        je     loc_4_1DAF
        mov    edi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [edi + 0xc]
        movzx  edx, word ptr [ecx + eax*2]
        cmp    edx, dword ptr [ebp - 0x1c]
        jb     loc_4_1D59
        mov    esi, 0x23
loc_4_1D7A:
        mov    ecx, dword ptr [ebp + 8]
        xor    edi, edi
        mov    edx, dword ptr [ecx + 0x38]
        test   edx, edx
        je     loc_4_1D9D
        mov    eax, dword ptr [ecx + 0x1c]
        add    eax, 0x10
        push   eax
        push   edx
        push   ecx
        call   sub_4_1531
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x38], edi
loc_4_1D9D:
        mov    eax, esi
        jmp    loc_4_1E09
loc_4_1DA1:
        mov    esi, 0x25
        jmp    loc_4_1D7A
loc_4_1DA8:
        mov    esi, 0x26
        jmp    loc_4_1D7A
loc_4_1DAF:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0x10]
        add    esi, 0x150a
        add    edi, 0x2c
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        movzx  eax, word ptr [ecx]
        mov    edi, dword ptr [ebp + 0x10]
        sub    ebx, edi
        mov    dword ptr [edx + eax*4 + 0x662], edi
        xor    eax, eax
        mov    dword ptr [ecx + 0x10], ebx
        jmp    loc_4_1E09
loc_4_1DE1:
        mov    ecx, dword ptr [ebp + 8]
        xor    esi, esi
        mov    edx, dword ptr [ecx + 0x38]
        test   edx, edx
        je     loc_4_1E04
        mov    eax, dword ptr [ecx + 0x1c]
        add    eax, 0x10
        push   eax
        push   edx
        push   ecx
        call   sub_4_1531
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x38], esi
loc_4_1E04:
        mov    eax, 1
loc_4_1E09:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_4_1E12:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        lea    edi, [esi + 0x24]
        mov    eax, dword ptr [esi + 0x1c]
        cmp    dword ptr [edi], eax
        jb     loc_4_1E2B
        mov    eax, 1
        jmp    loc_4_1EA5
loc_4_1E2B:
        mov    eax, dword ptr [esi + 0x3c]
        test   eax, eax
        je     loc_4_1E54
        add    eax, 0x10
        push   eax
        push   esi
        call   sub_4_11BC
        add    esp, 8
        mov    dword ptr [esi + 0x38], eax
        test   eax, eax
        jne    loc_4_1E4D
        mov    eax, 2
        jmp    loc_4_1EA5
loc_4_1E4D:
        mov    dword ptr [esi + 0x3c], 0
loc_4_1E54:
        mov    eax, dword ptr [edi]
        mov    ebx, dword ptr [esi + 0x1c]
        sub    ebx, eax
        mov    ecx, 0x400
        cmp    ebx, ecx
        mov    ebp, ebx
        mov    edx, dword ptr [esp + 0x18]
        jb     loc_4_1E6C
        mov    ebp, ecx
loc_4_1E6C:
        push   ebp
        add    edx, 0x14
        push   eax
        push   dword ptr [esi + 0x38]
        push   0x10
        push   edx
        push   esi
        call   sub_4_1EAC
        cmp    ebx, 0x400
        ja     loc_4_1E9D
        mov    eax, dword ptr [edi]
        push   0xf
        add    eax, ebx
        push   eax
        mov    dword ptr [edi], eax
        push   dword ptr [esi + 0x38]
        push   0
        push   0
        push   esi
        call   sub_4_1EAC
        jmp    loc_4_1EA3
loc_4_1E9D:
        add    dword ptr [edi], 0x400
loc_4_1EA3:
        xor    eax, eax
loc_4_1EA5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_1EAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
loc_4_1EBA:
        push   0
        push   0
        push   0xff
        push   esi
        call   sub_1_1A68
loc_4_1EC9:
        mov    byte ptr [ebp + edi - 0xc], al
        add    esp, 0x10
        cmp    eax, 0xff
        mov    ebx, eax
        je     loc_4_1F58
        shl    eax, 5
        cmp    byte ptr [eax + esi + 0x59], 0
        je     loc_4_1EE6
        dec    dword ptr [esi + 0x18]
loc_4_1EE6:
        push   0x80
        mov    eax, ebx
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19AA
loc_4_1EF9:
        push   0
        mov    eax, ebx
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_1F06:
        push   0
        mov    eax, ebx
        or     al, 0x40
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_1F13:
        push   0
        mov    eax, ebx
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_1F20:
        push   0
        mov    eax, ebx
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_1F2D:
        push   0x40000000
        mov    eax, ebx
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_1F3D:
        push   0x40000000
        push   ebx
        push   esi
        or     bh, 2
        call   sub_1_1A14
loc_4_1F4C:
        push   0x6000000
        push   ebx
        push   esi
        call   sub_1_1A14
loc_4_1F58:
        inc    edi
        cmp    edi, 0xa
        jb     loc_4_1EBA
        mov    ecx, dword ptr [ebp + 0x18]
        xor    edi, edi
        test   ecx, ecx
        jne    loc_4_1F9C
        mov    dword ptr [esi + 0x28], edi
        mov    dword ptr [esi + 0x2c], edi
        mov    ebx, 0xe
        mov    dword ptr [esi + 0x30], edi
        push   dword ptr [ebp + 0x14]
        push   0x236
        push   esi
        mov    dword ptr [esi + 0x34], edi
        call   sub_1_1A14
loc_4_1F8A:
        push   0
        push   0x23a
        push   esi
        call   sub_1_19AA
loc_4_1F97:
        dec    ebx
        jne    loc_4_1F8A
        jmp    loc_4_1FAF
loc_4_1F9C:
        mov    eax, dword ptr [ebp + 0x14]
        lea    ecx, [ecx + eax + 0xe]
        push   ecx
        push   0x236
        push   esi
        call   sub_1_1A14
loc_4_1FAF:
        mov    ax, word ptr [ebp + 0x10]
        test   ax, ax
        jne    loc_4_1FFC
        push   0
        mov    edi, 0x23a
        push   esi
        call   sub_4_20A9
        movsx  eax, ax
        push   eax
        push   edi
        push   esi
        call   sub_1_19AA
loc_4_1FD0:
        push   0
        push   esi
        call   sub_4_20A9
        movsx  eax, ax
        push   eax
        push   edi
        push   esi
        call   sub_1_19AA
loc_4_1FE3:
        mov    ebx, dword ptr [ebp + 0x1c]
        test   ebx, ebx
        je     loc_4_203A
loc_4_1FEA:
        push   0
        push   0x23a
        push   esi
        call   sub_1_19AA
loc_4_1FF7:
        dec    ebx
        jne    loc_4_1FEA
        jmp    loc_4_203A
loc_4_1FFC:
        cmp    ax, 0x10
        jne    loc_4_203A
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 0x1c]
        test   ebx, ebx
        je     loc_4_202D
loc_4_200C:
        movsx  ecx, word ptr [edi]
        mov    eax, edi
        push   ecx
        add    edi, 2
        push   esi
        call   sub_4_20A9
        movsx  ecx, ax
        push   ecx
        push   0x23a
        push   esi
        call   sub_1_19AA
loc_4_202A:
        dec    ebx
        jne    loc_4_200C
loc_4_202D:
        push   0
        push   0x23a
        push   esi
        call   sub_1_19AA
loc_4_203A:
        xor    edi, edi
loc_4_203C:
        movzx  ebx, byte ptr [ebp + edi - 0xc]
        cmp    ebx, 0xff
        je     loc_4_209A
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_2057:
        push   0x1d8
        mov    eax, ebx
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_2067:
        push   0x1e0
        mov    eax, ebx
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_2077:
        push   0x48000000
        mov    eax, ebx
        shl    ebx, 5
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_208A:
        lea    eax, [ebx + esi]
        mov    word ptr [eax + 0x42], 0xffe
        mov    byte ptr [eax + 0x59], 1
        inc    dword ptr [esi + 0x18]
loc_4_209A:
        inc    edi
        cmp    edi, 0xa
        jb     loc_4_203C
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
sub_4_20A9:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    edi, [edx + 0x2c]
        mov    esi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [edi]
        test   ebx, ebx
        jne    loc_4_20EA
        test   esi, esi
        jne    loc_4_20C8
        cmp    dword ptr [edx + 0x28], 0
        je     loc_4_20D4
loc_4_20C8:
        cmp    dword ptr [edx + 0x30], 0
        jne    loc_4_20EA
        cmp    dword ptr [edx + 0x34], 0
        jne    loc_4_20EA
loc_4_20D4:
        mov    eax, dword ptr [edx + 0x30]
        mov    ecx, dword ptr [edx + 0x28]
        mov    dword ptr [edx + 0x34], eax
        mov    dword ptr [edx + 0x30], ebx
        xor    ax, ax
        mov    dword ptr [edi], ecx
        mov    dword ptr [edx + 0x28], esi
        jmp    loc_4_2107
loc_4_20EA:
        mov    ebp, dword ptr [edx + 0x30]
        mov    ecx, dword ptr [edx + 0x28]
        mov    eax, ebx
        mov    dword ptr [edx + 0x34], ebp
        mov    dword ptr [edx + 0x30], ebx
        mov    dword ptr [edi], ecx
        imul   eax, eax, 6
        sub    eax, ebp
        mov    dword ptr [edx + 0x28], esi
        sub    eax, ecx
        sar    eax, 3
loc_4_2107:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_210E:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    edx, dword ptr [ebp + 0xc]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        add    edx, 0x14
        mov    ecx, edx
        mov    dword ptr [ebx + 0x3c], eax
        add    edx, 0xc
        mov    dword ptr [ebx + 0x38], eax
        cmp    dword ptr [ecx], 0x46464952
        je     loc_4_213F
        mov    eax, 2
        jmp    loc_4_234E
loc_4_213F:
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 0x4b424653
        je     loc_4_215A
        cmp    eax, 0x6b626673
        je     loc_4_215A
        mov    eax, 2
        jmp    loc_4_234E
loc_4_215A:
        mov    eax, dword ptr [ecx + 4]
        add    eax, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, edx
        add    edx, 0xc
        cmp    dword ptr [eax], 0x5453494c
        je     loc_4_217A
        mov    eax, 2
        jmp    loc_4_234E
loc_4_217A:
        cmp    dword ptr [eax + 8], 0x4f464e49
        je     loc_4_218D
        mov    eax, 2
        jmp    loc_4_234E
loc_4_218D:
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0xc], eax
        sub    eax, 4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 0x7ec
        jb     loc_4_21AA
        mov    eax, 2
        jmp    loc_4_234E
loc_4_21AA:
        cmp    dword ptr [ebp - 4], 0
        je     loc_4_228E
loc_4_21B4:
        mov    ecx, dword ptr [edx]
        mov    eax, edx
        add    edx, 8
        cmp    ecx, 0x4d414e49
        je     loc_4_21DB
        cmp    ecx, 0x6c696669
        je     loc_4_21F2
        cmp    ecx, 0x6d6f7269
        je     loc_4_221C
        add    eax, 4
        jmp    loc_4_227E
loc_4_21DB:
        lea    edi, [ebx + 0x150a]
        mov    esi, edx
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        add    eax, 4
        jmp    loc_4_227E
loc_4_21F2:
        add    eax, 4
        cmp    dword ptr [eax], 4
        jne    loc_4_22A5
        mov    ecx, dword ptr [edx]
        add    edx, 4
        test   cx, cx
        je     loc_4_22AF
        cmp    cx, 2
        ja     loc_4_22AF
        mov    word ptr [ebx + 0x40], cx
        jmp    loc_4_2280
loc_4_221C:
        add    eax, 4
        cmp    dword ptr [eax], 0
        je     loc_4_2280
        mov    esi, 0
        jbe    loc_4_225E
loc_4_222B:
        cmp    byte ptr [ebx + esi + 0x151e], 0
        je     loc_4_2268
        mov    cl, byte ptr [edx + esi]
        cmp    cl, 0x61
        jb     loc_4_224A
        cmp    cl, 0x7a
        ja     loc_4_224A
        movzx  ecx, cl
        sub    ecx, 0x20
        jmp    loc_4_224D
loc_4_224A:
        movzx  ecx, cl
loc_4_224D:
        movsx  edi, byte ptr [ebx + esi + 0x151e]
        cmp    edi, ecx
        jne    loc_4_225E
        inc    esi
        cmp    esi, dword ptr [eax]
        jb     loc_4_222B
loc_4_225E:
        cmp    byte ptr [ebx + esi + 0x151e], 0
        jne    loc_4_2277
loc_4_2268:
        cmp    byte ptr [edx + esi], 0
        jne    loc_4_2277
        mov    byte ptr [ebx + 0x1524], 0
        jmp    loc_4_227E
loc_4_2277:
        mov    byte ptr [ebx + 0x1524], 1
loc_4_227E:
        add    edx, dword ptr [eax]
loc_4_2280:
        mov    eax, dword ptr [eax]
        add    eax, 8
        sub    dword ptr [ebp - 4], eax
        jne    loc_4_21B4
loc_4_228E:
        mov    esi, edx
        add    edx, 0xc
        cmp    dword ptr [esi], 0x5453494c
        je     loc_4_22B9
        mov    eax, 2
        jmp    loc_4_234E
loc_4_22A5:
        mov    eax, 2
        jmp    loc_4_234E
loc_4_22AF:
        mov    eax, 2
        jmp    loc_4_234E
loc_4_22B9:
        cmp    dword ptr [esi + 8], 0x61746473
        je     loc_4_22CC
        mov    eax, 2
        jmp    loc_4_234E
loc_4_22CC:
        mov    edi, edx
        mov    ecx, dword ptr [ebp + 0xc]
        add    edx, 8
        sub    edx, dword ptr [ebp + 0xc]
        sub    edx, 0x14
        mov    dword ptr [ecx + 4], edx
        cmp    dword ptr [edi], 0x6d616e73
        jne    loc_4_22FD
        mov    eax, dword ptr [edi + 4]
        add    esi, 4
        add    eax, edx
        add    eax, 8
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi]
        sub    eax, dword ptr [edi + 4]
        sub    eax, 0x14
        jmp    loc_4_2305
loc_4_22FD:
        add    esi, 4
        mov    eax, dword ptr [esi]
        sub    eax, 0xc
loc_4_2305:
        test   eax, eax
        jge    loc_4_230B
        xor    eax, eax
loc_4_230B:
        mov    dword ptr [ebx + 0x24], 0
        shr    eax, 1
        mov    dword ptr [ebx + 0x3c], eax
        mov    dword ptr [ebx + 0x1c], eax
        add    eax, 0x3ff
        cdq    
        and    edx, 0x3ff
        add    eax, edx
        mov    edx, dword ptr [ebp + 0xc]
        sar    eax, 0xa
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [edx + 2], ax
        mov    eax, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [esi]
        lea    edx, [esi + eax + 0x28]
        mov    eax, dword ptr [ebp - 8]
        sub    eax, edx
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [ebx + 0x20], eax
        xor    eax, eax
loc_4_234E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_4_2357:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x38]
        test   eax, eax
        je     loc_4_2374
        mov    ecx, dword ptr [esi + 0x1c]
        add    ecx, 0x10
        push   ecx
        push   eax
        push   esi
        call   sub_4_1531
        add    esp, 0xc
loc_4_2374:
        xor    eax, eax
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [esi + 0x3c], eax
        pop    esi
        ret    4
sub_4_2380:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        mov    dword ptr [ebp - 0x10], 0x240
        mov    dword ptr [ebp - 0xc], 0x340
        mov    dword ptr [ebp - 8], 0x260
        mov    dword ptr [ebp - 4], 0x360
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_4_23A9:
        test   edi, 1
        jne    loc_4_23BC
        movzx  eax, word ptr [edi*2]         ; -> obj5:0x03B0
        push   eax
        jmp    loc_4_23CB
loc_4_23BC:
        mov    ax, word ptr [edi*2]          ; -> obj5:0x03B0
        or     ah, 0x80
        movzx  ecx, ax
        push   ecx
loc_4_23CB:
        mov    eax, edi
        mov    ecx, edi
        and    al, 0xe7
        and    ecx, 0x1f
        shr    eax, 3
        inc    edi
        mov    eax, dword ptr [ebp + eax - 0x10]
        or     eax, ecx
        push   eax
        push   esi
        call   sub_1_19AA
loc_4_23E5:
        cmp    edi, 0x80
        jb     loc_4_23A9
        xor    edi, edi
        push   edi
        push   0x229
        push   esi
        call   sub_1_1A14
loc_4_23FB:
        push   0x83
        push   0x22a
        push   esi
        call   sub_1_1A14
loc_4_240B:
        push   0x8000
        push   0x22d
        push   esi
        call   sub_1_1A14
loc_4_241B:
        push   edi
        push   0x22e
        push   esi
        call   sub_1_1A14
loc_4_2427:
        movzx  eax, word ptr [edi*2]         ; -> obj5:0x03B0
        push   eax
        mov    ecx, edi
        mov    eax, edi
        and    ecx, 0x1f
        and    al, 0xe7
        inc    edi
        shr    eax, 3
        mov    eax, dword ptr [ebp + eax - 0x10]
        or     eax, ecx
        push   eax
        push   esi
        call   sub_1_19AA
loc_4_244A:
        cmp    edi, 0x80
        jb     loc_4_2427
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_2458:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], -1
        jne    loc_4_246B
        mov    dword ptr [ebp + 0xc], 5
loc_4_246B:
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    ebx, -1
        jne    loc_4_248E
        mov    ebx, 5
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x151e], 0x4d474d31
        jne    loc_4_2491
        mov    ebx, 9
        jmp    loc_4_2491
loc_4_248E:
        mov    esi, dword ptr [ebp + 8]
loc_4_2491:
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [esi + 0x17], bl
        mov    byte ptr [esi + 0x16], al
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 6
        movzx  ecx, word ptr [eax]           ; -> obj5:0x04B0
        push   ecx
        push   0x361
        push   esi
        call   sub_1_19AA
loc_4_24B3:
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 6
        movzx  ecx, word ptr [eax + 2]       ; -> obj5:0x04B2
        push   ecx
        push   0x371
        push   esi
        call   sub_1_19AA
loc_4_24CC:
        imul   ebx, ebx, 0x12
        movzx  eax, word ptr [ebx]           ; -> obj5:0x04F8
        push   eax
        push   0x271
        push   esi
        call   sub_1_19AA
loc_4_24E2:
        movzx  ecx, word ptr [ebx + 2]       ; -> obj5:0x04FA
        push   ecx
        push   0x273
        push   esi
        call   sub_1_19AA
loc_4_24F5:
        movzx  ecx, word ptr [ebx + 4]       ; -> obj5:0x04FC
        push   ecx
        push   0x27b
        push   esi
        call   sub_1_19AA
loc_4_2508:
        movzx  ecx, word ptr [ebx + 6]       ; -> obj5:0x04FE
        push   ecx
        push   0x367
        push   esi
        call   sub_1_19AA
loc_4_251B:
        movzx  ecx, word ptr [ebx + 8]       ; -> obj5:0x0500
        push   ecx
        push   0x36b
        push   esi
        call   sub_1_19AA
loc_4_252E:
        movzx  ecx, word ptr [ebx + 0xa]     ; -> obj5:0x0502
        push   ecx
        push   0x36d
        push   esi
        call   sub_1_19AA
loc_4_2541:
        movzx  ecx, word ptr [ebx + 0xc]     ; -> obj5:0x0504
        push   ecx
        push   0x377
        push   esi
        call   sub_1_19AA
loc_4_2554:
        movzx  ecx, word ptr [ebx + 0xe]     ; -> obj5:0x0506
        push   ecx
        push   0x379
        push   esi
        call   sub_1_19AA
loc_4_2567:
        movzx  ecx, word ptr [ebx + 0x10]    ; -> obj5:0x0508
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 6
        movzx  edx, word ptr [eax + 4]       ; -> obj5:0x04B4
        lea    eax, [ecx + edx + 0x262]
        lea    edi, [ecx + edx]
        add    edi, 0x8362
        push   eax
        push   0x375
        push   esi
        call   sub_1_19AA
loc_4_2597:
        push   edi
        push   0x37d
        push   esi
        call   sub_1_19AA
loc_4_25A3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_4_25AA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        cmp    word ptr [esi + 0x12], 0
        jne    loc_4_2647
        mov    ebx, dword ptr [esp + 0x10]
        cmp    ebx, -1
        jne    loc_4_25C9
        mov    ebx, 2
loc_4_25C9:
        mov    byte ptr [esi + 0x15], bl
        imul   ebx, ebx, 0xe
        movzx  eax, word ptr [ebx]           ; -> obj5:0x05D0
        push   eax
        push   0x269
        push   esi
        call   sub_1_19AA
loc_4_25E2:
        movzx  ecx, word ptr [ebx + 2]       ; -> obj5:0x05D2
        push   ecx
        push   0x26c
        push   esi
        call   sub_1_19AA
loc_4_25F5:
        movzx  ecx, word ptr [ebx + 4]       ; -> obj5:0x05D4
        push   ecx
        push   0x363
        push   esi
        call   sub_1_19AA
loc_4_2608:
        push   dword ptr [ebx + 6]           ; -> obj5:0x05D6
        push   0x229
        push   esi
        call   sub_1_1A14
loc_4_2619:
        push   dword ptr [ebx + 0xa]         ; -> obj5:0x05DA
        push   0x22a
        push   esi
        call   sub_1_1A14
loc_4_262A:
        push   0x8000
        push   0x22d
        push   esi
        call   sub_1_1A14
loc_4_263A:
        push   0
        push   0x22e
        push   esi
        call   sub_1_1A14
loc_4_2647:
        xor    eax, eax
        pop    esi
        pop    ebx
        ret    
sub_4_264C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    word ptr [edi + 0x12], 0
        jne    loc_4_26CA
        mov    ebx, dword ptr [ebp + 0xc]
        cmp    ebx, -1
        jne    loc_4_266C
        mov    ebx, 4
loc_4_266C:
        mov    word ptr [ebp - 8], 0x240
        xor    esi, esi
        mov    byte ptr [edi + 0x14], bl
        mov    word ptr [ebp - 6], 0x340
        imul   ebx, ebx, 0x1c
        mov    word ptr [ebp - 4], 0x260
        mov    word ptr [ebp - 2], 0x360
loc_4_268C:
        mov    al, byte ptr [esi]            ; -> obj5:0x0640
        lea    ecx, [ebx + esi]
        movzx  edx, word ptr [ecx*2]         ; -> obj5:0x0660
        push   edx
        mov    cl, al
        shr    cl, 4
        inc    esi
        movzx  ecx, cl
        and    cl, 0xfe
        mov    dx, word ptr [ebp + ecx - 8]
        xor    cx, cx
        mov    cl, al
        and    cx, 0x1f
        or     dx, cx
        movzx  eax, dx
        push   eax
        push   edi
        call   sub_1_19AA
loc_4_26C5:
        cmp    esi, 0x1c
        jl     loc_4_268C
loc_4_26CA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_26D3:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        test   ecx, ecx
        jne    loc_4_26E0
        xor    eax, eax
        jmp    loc_4_272D
loc_4_26E0:
        imul   ecx, ecx, 0x2f8e
        mov    edx, 0x1f
loc_4_26EB:
        test   ecx, 0x80000000
        jne    loc_4_26FC
        add    ecx, ecx
        dec    edx
        test   edx, edx
        jg     loc_4_26EB
        jmp    loc_4_272D
loc_4_26FC:
        mov    esi, ecx
        and    ecx, 0xfe0000
        shr    ecx, 0x11
        and    esi, 0x7f000000
        mov    eax, esi
        sub    edx, 0xf
        shr    eax, 0x18
        movsx  eax, byte ptr [eax]           ; -> obj5:0x0A20
        imul   eax, ecx
        shr    esi, 0x16
        shl    edx, 0x14
        add    eax, dword ptr [esi]          ; -> obj5:0x0820
        add    eax, edx
loc_4_272D:
        pop    esi
        ret    
sub_4_272F:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        test   ecx, ecx
        je     loc_4_2752
        xor    eax, eax
        mov    edx, 0                        ; -> loc_1_0E80
loc_4_273F:
        movzx  esi, word ptr [edx]
        cmp    esi, ecx
        jb     loc_4_2756
        add    edx, 2
        inc    eax
        cmp    edx, 0x100                    ; -> obj1:0x0F80
        jb     loc_4_273F
loc_4_2752:
        mov    ax, 0x7f
loc_4_2756:
        pop    esi
        ret    
sub_4_2758:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    eax, 5
        push   esi
        cmp    eax, 0x2b
        ja     loc_4_2776
        movzx  ecx, byte ptr [eax]           ; -> obj4:0x28F7
        jmp    dword ptr [ecx*4]             ; -> obj4:0x289B
loc_4_2776:
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_4_2898
loc_4_277E:
        cmp    dword ptr [ebp + 0xc], 0x7f
        jle    loc_4_278E
        mov    eax, 0x7f
        jmp    loc_4_2898
loc_4_278E:
        cmp    dword ptr [ebp + 0xc], -0x7f
        mov    eax, 0xffffff81
        jl     loc_4_2898
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_4_2898
loc_4_27A5:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, eax
        jmp    loc_4_2898
loc_4_27AF:
        mov    eax, dword ptr [ebp + 0xc]
        cdq    
        and    edx, 7
        add    eax, edx
        sar    eax, 3
        jmp    loc_4_2898
loc_4_27C0:
        mov    esi, 0x8000
        mov    ecx, 0x2d5
        movsx  eax, word ptr [ebp + 0x10]
        imul   eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x3e8
        cdq    
        idiv   ecx
        movsx  edx, ax
        sub    esi, edx
        mov    eax, esi
        jmp    loc_4_2898
loc_4_27E7:
        movzx  eax, word ptr [ebp + 0xc]
        push   eax
        call   sub_1_5055
loc_4_27F1:
        movsx  eax, ax
        add    esp, 4
        jmp    loc_4_2898
loc_4_27FC:
        mov    ecx, 0xffffffa4
        mov    eax, dword ptr [ebp + 0xc]
        cdq    
        idiv   ecx
        lea    ecx, [eax + 0x7f]
        test   ecx, ecx
        jge    loc_4_2815
        xor    eax, eax
        jmp    loc_4_2898
loc_4_2815:
        mov    eax, 0x7f
        cmp    ecx, 0x7f
        jg     loc_4_2898
        mov    eax, ecx
        jmp    loc_4_2898
loc_4_2823:
        movsx  eax, word ptr [ebp + 0x10]
        imul   eax, dword ptr [ebp + 0xc]
        movzx  ecx, ax
        push   ecx
        call   sub_4_272F
        movsx  eax, ax
        add    esp, 4
        jmp    loc_4_2898
loc_4_283C:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, 0x4b
        imul   eax, eax, 0x64
        cdq    
        idiv   ecx
        mov    ecx, eax
        test   ecx, ecx
        jge    loc_4_2854
        xor    eax, eax
        jmp    loc_4_2898
loc_4_2854:
        mov    eax, 0x7f
        cmp    ecx, 0x7f
        jg     loc_4_2898
        mov    eax, ecx
        jmp    loc_4_2898
loc_4_2862:
        movsx  eax, word ptr [ebp + 0x10]
        imul   eax, dword ptr [ebp + 0xc]
        movzx  ecx, ax
        push   ecx
        call   sub_4_272F
        movsx  eax, ax
        add    esp, 4
        jmp    loc_4_2898
loc_4_287B:
        movsx  eax, word ptr [ebp + 0x10]
        imul   eax, dword ptr [ebp + 0xc]
        jmp    loc_4_2898
loc_4_2885:
        mov    eax, 0x7f
        mov    ecx, 0xa
        sub    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x4b
        cdq    
        idiv   ecx
loc_4_2898:
        pop    esi
        pop    ebp
        ret    
        ; ---- dispatch table (23 entries) ----
        dd  loc_4_277E
        dd  loc_4_27A5
        dd  loc_4_27AF
        dd  loc_4_27A5
        dd  loc_4_27A5
        dd  loc_4_27AF
        dd  loc_4_27C0
        dd  loc_4_27A5
        dd  loc_4_27C0
        dd  loc_4_27A5
        dd  loc_4_27C0
        dd  loc_4_27E7
        dd  loc_4_27FC
        dd  loc_4_2823
        dd  loc_4_283C
        dd  loc_4_2862
        dd  loc_4_27C0
        dd  loc_4_27E7
        dd  loc_4_287B
        dd  loc_4_283C
        dd  loc_4_2862
        dd  loc_4_2885
        dd  loc_4_2776
        db  000h, 000h, 000h, 001h, 002h, 003h, 003h, 016h, 004h, 005h, 016h, 016h, 016h, 016h, 016h, 016h
        db  006h, 007h, 008h, 009h, 00Ah, 00Bh, 00Ch, 00Dh, 00Eh, 00Fh, 016h, 016h, 010h, 011h, 016h, 012h
        db  013h, 014h, 016h, 016h, 016h, 016h, 016h, 016h, 016h, 016h, 016h, 015h
sub_4_2923:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    eax, 5
        cmp    eax, 0x34
        ja     loc_4_2940
        movzx  ecx, byte ptr [eax]           ; -> obj4:0x2A60
        jmp    dword ptr [ecx*4]             ; -> obj4:0x29EC
loc_4_2940:
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_4_29EA
loc_4_2948:
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x1b4f
        add    eax, 0x8000
        cdq    
        and    edx, 0xffff
        add    eax, edx
        sar    eax, 0x10
        jmp    loc_4_29EA
loc_4_2967:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, 0x1d
        add    eax, 0xf
        cdq    
        idiv   ecx
        sub    eax, 0x99
        jmp    loc_4_29EA
loc_4_297C:
        mov    ecx, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        cdq    
        idiv   ecx
        jmp    loc_4_29EA
loc_4_2989:
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x919
        add    eax, 0x8000
        cdq    
        and    edx, 0xffff
        add    eax, edx
        sar    eax, 0x10
        jmp    loc_4_29EA
loc_4_29A5:
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x48d
        add    eax, 0x8000
        cdq    
        and    edx, 0xffff
        add    eax, edx
        sar    eax, 0x10
        jmp    loc_4_29EA
loc_4_29C1:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, 0x78
        shl    eax, 7
        cdq    
        idiv   ecx
        jmp    loc_4_29EA
loc_4_29D1:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, 0x3e8
        imul   eax, eax, -0x7f
        cdq    
        idiv   ecx
        add    eax, 0x7f
        jmp    loc_4_29EA
loc_4_29E4:
        mov    eax, dword ptr [ebp + 0xc]
        imul   eax, eax, 0x64
loc_4_29EA:
        pop    ebp
        ret    
        ; ---- dispatch table (29 entries) ----
        dd  loc_4_2948
        dd  loc_4_2967
        dd  loc_4_297C
        dd  loc_4_2989
        dd  loc_4_29A5
        dd  loc_4_29C1
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_29D1
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_29D1
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_29E4
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        dd  loc_4_2940
        db  000h, 000h, 000h, 001h, 002h, 003h, 004h, 01Ch, 005h, 01Ch, 006h, 01Ch, 007h, 01Ch, 008h, 01Ch
        db  009h, 01Ch, 00Ah, 01Ch, 00Bh, 01Ch, 00Ch, 01Ch, 00Dh, 01Ch, 00Eh, 01Ch, 00Fh, 01Ch, 010h, 01Ch
        db  011h, 01Ch, 012h, 01Ch, 013h, 01Ch, 014h, 01Ch, 015h, 01Ch, 016h, 01Ch, 017h, 01Ch, 018h, 01Ch
        db  019h, 01Ch, 01Ah, 01Ch, 01Bh
sub_4_2A95:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        lea    eax, [esi - 5]
        cmp    eax, 0x35
        ja     loc_4_2AB1
        movzx  ecx, byte ptr [eax]           ; -> obj4:0x2B3C
        jmp    dword ptr [ecx*4]             ; -> obj4:0x2AD8
loc_4_2AB1:
        mov    eax, dword ptr [esp + 0x10]
        jmp    loc_4_2AD5
loc_4_2AB7:
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_4_2923
        add    esp, 8
        mov    edi, eax
        push   0
        push   esi
        call   sub_4_2923
        add    esp, 8
        sub    edi, eax
        mov    eax, edi
loc_4_2AD5:
        pop    edi
        pop    esi
        ret    
        ; ---- dispatch table (25 entries) ----
        dd  loc_4_2AB7
        dd  loc_4_2AB7
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB7
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB7
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB1
        dd  loc_4_2AB7
        dd  loc_4_2AB7
        dd  loc_4_2AB7
        dd  loc_4_2AB1
        dd  loc_4_2AB7
        dd  loc_4_2AB7
        dd  loc_4_2AB1
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 001h, 018h, 002h, 018h, 003h, 018h, 004h, 018h
        db  005h, 018h, 006h, 018h, 007h, 018h, 008h, 018h, 009h, 018h, 00Ah, 018h, 00Bh, 018h, 00Ch, 018h
        db  00Dh, 018h, 00Eh, 018h, 00Fh, 018h, 010h, 018h, 011h, 012h, 012h, 018h, 013h, 018h, 014h, 014h
        db  018h, 015h, 018h, 016h, 018h, 017h
sub_4_2B72:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    eax, 0x15
        push   ebx
        cmp    eax, 0x25
        push   esi
        push   edi
        ja     loc_4_2B92
        movzx  ecx, byte ptr [eax]           ; -> obj4:0x2DC9
        jmp    dword ptr [ecx*4]             ; -> obj4:0x2D61
loc_4_2B92:
        mov    eax, dword ptr [ebp + 0xc]
        jmp    loc_4_2D5C
loc_4_2B9A:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0xffffd120
        jg     loc_4_2BAE
        mov    eax, 0x8000
        jmp    loc_4_2D5C
loc_4_2BAE:
        add    eax, 0x30e4
        mov    ecx, 0x4b0
        shl    eax, 0x10
        mov    edi, 0x8000
        cdq    
        idiv   ecx
        mov    ebx, eax
        mov    esi, 0xffff
        and    ebx, esi
        mov    cl, 0x10
        cdq    
        add    ebx, 0x10000
        and    edx, esi
        add    eax, edx
        sar    eax, cl
        sub    cl, al
        sar    ebx, cl
        sub    edi, ebx
        mov    eax, edi
        test   eax, eax
        jge    loc_4_2D5C
        xor    eax, eax
        jmp    loc_4_2D5C
loc_4_2BF2:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0xffffc180
        jg     loc_4_2C03
        xor    eax, eax
        jmp    loc_4_2D5C
loc_4_2C03:
        add    eax, 0x23a6
        mov    ecx, 0x4b0
        shl    eax, 0x10
        mov    esi, 0xffff
        cdq    
        idiv   ecx
        mov    edi, eax
        mov    cl, 0x10
        cdq    
        and    edi, esi
        and    edx, esi
        add    edi, 0x10000
        add    eax, edx
        sar    eax, cl
        sub    cl, al
        sar    edi, cl
        mov    eax, edi
        mov    ecx, 0xff
        cmp    eax, ecx
        jle    loc_4_2D5C
        mov    eax, ecx
        jmp    loc_4_2D5C
loc_4_2C45:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0xfffffda8
        jle    loc_4_2C99
        cmp    eax, 0x10cc
        jl     loc_4_2C60
        mov    eax, 1
        jmp    loc_4_2D5C
loc_4_2C60:
        add    eax, 0x1f4
        mov    ecx, 0x96
        cdq    
        idiv   ecx
        cdq    
        mov    esi, eax
        and    edx, 7
        not    esi
        and    esi, 7
        add    eax, edx
        sar    eax, 3
        or     esi, 8
        mov    cl, al
        sar    esi, cl
        cmp    esi, 1
        mov    eax, esi
        jge    loc_4_2D5C
        mov    eax, 1
        jmp    loc_4_2D5C
loc_4_2C99:
        mov    ecx, 0x25
        mov    esi, 0x4b
        sub    ecx, eax
        mov    eax, ecx
        mov    ecx, 0x80
        cdq    
        idiv   esi
        add    eax, 8
        cmp    eax, ecx
        jle    loc_4_2D5C
        mov    eax, ecx
        jmp    loc_4_2D5C
loc_4_2CC1:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0xffffeb08
        jl     loc_4_2D57
        add    eax, 0x1022
        mov    ecx, 0x4b0
        shl    eax, 0x10
        mov    edi, 0x7f
        cdq    
        idiv   ecx
        mov    ebx, eax
        mov    esi, 0xffff
        and    ebx, esi
        mov    cl, 0x10
        cdq    
        add    ebx, 0x10000
        and    edx, esi
        add    eax, edx
        sar    eax, cl
        sub    cl, al
        sar    ebx, cl
        sub    edi, ebx
        mov    eax, edi
        test   eax, eax
        jge    loc_4_2D5C
        xor    eax, eax
        jmp    loc_4_2D5C
loc_4_2D0C:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0x708
        jle    loc_4_2D3E
        mov    ecx, 0x96
        sub    eax, 0x708
        cdq    
        idiv   ecx
        cdq    
        mov    esi, eax
        and    edx, 7
        not    esi
        and    esi, 7
        add    eax, edx
        sar    eax, 3
        or     esi, 8
        mov    cl, al
        sar    esi, cl
        mov    eax, esi
        jmp    loc_4_2D5C
loc_4_2D3E:
        mov    ecx, 0x25
        mov    esi, 0x4b
        sub    ecx, eax
        mov    eax, ecx
        cdq    
        idiv   esi
        add    eax, 0x27
        cmp    eax, 0x7f
        jle    loc_4_2D5C
loc_4_2D57:
        mov    eax, 0x7f
loc_4_2D5C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        ; ---- dispatch table (26 entries) ----
        dd  loc_4_2B9A
        dd  loc_4_2BF2
        dd  loc_4_2B9A
        dd  loc_4_2BF2
        dd  loc_4_2B9A
        dd  loc_4_2C45
        dd  loc_4_2CC1
        dd  loc_4_2D0C
        dd  loc_4_2D0C
        dd  loc_4_2B92
        dd  loc_4_2B9A
        dd  loc_4_2C45
        dd  loc_4_2CC1
        dd  loc_4_2D0C
        dd  loc_4_2D0C
        dd  loc_4_2B92
        dd  loc_4_2B92
        dd  loc_4_2B92
        dd  loc_4_2B92
        dd  loc_4_2B92
        dd  loc_4_2B92
        dd  loc_4_2B92
        dd  loc_4_2B92
        dd  loc_4_2B92
        dd  loc_4_2B92
        dd  loc_4_2B92
        db  000h, 001h, 002h, 003h, 004h, 005h, 006h, 007h, 019h, 008h, 019h, 009h, 00Ah, 00Bh, 00Ch, 00Dh
        db  019h, 00Eh, 019h, 00Fh, 019h, 010h, 019h, 011h, 019h, 012h, 019h, 013h, 019h, 014h, 019h, 015h
        db  019h, 016h, 019h, 017h, 019h, 018h
sub_4_2DEF:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        add    edi, 0x662
        mov    ebx, dword ptr [ebp + 0x10]
loc_4_2E06:
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_4_2E32
        mov    eax, dword ptr [eax]
        xor    esi, esi
        cmp    word ptr [eax], 0xffff
        je     loc_4_2E32
loc_4_2E17:
        cmp    word ptr [eax], 0
        je     loc_4_2E27
        cmp    byte ptr [eax + 3], dl
        jne    loc_4_2E27
        cmp    byte ptr [eax + 2], bl
        je     loc_4_2E45
loc_4_2E27:
        add    eax, 0x1a
        inc    esi
        cmp    word ptr [eax], 0xffff
        jne    loc_4_2E17
loc_4_2E32:
        add    edi, 4
        inc    ecx
        cmp    ecx, 0x120
        jb     loc_4_2E06
        xor    eax, eax
loc_4_2E40:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_4_2E45:
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp + 0x18]
        mov    eax, 1
        mov    dword ptr [ecx], esi
        jmp    loc_4_2E40
sub_4_2E56:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi + edi*4 + 0x662]
        test   ebx, ebx
        je     loc_4_2EA4
        mov    word ptr [ebp - 0x18], di
        lea    eax, [ebp - 0x18]
        push   eax
        push   esi
        add    esi, 0x456
        call   sub_4_164D
        push   ebx
        call   sub_4_0D91
        add    esp, 4
        mov    eax, 0x11
loc_4_2E92:
        movzx  ecx, word ptr [esi]
        cmp    ecx, edi
        jne    loc_4_2E9E
        mov    word ptr [esi], 0xffff
loc_4_2E9E:
        add    esi, 0x20
        dec    eax
        jne    loc_4_2E92
loc_4_2EA4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_2EAD:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        push   esi
        add    ecx, 0x662
        xor    esi, esi
        mov    edx, dword ptr [esp + 0x10]
loc_4_2EBF:
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     loc_4_2EE5
        mov    eax, dword ptr [eax]
        cmp    word ptr [eax], 0xffff
        je     loc_4_2EE5
loc_4_2ECE:
        cmp    word ptr [eax], 0
        je     loc_4_2EDB
        mov    bl, byte ptr [edx]
        cmp    byte ptr [eax + 3], bl
        je     loc_4_2EFE
loc_4_2EDB:
        add    eax, 0x1a
        cmp    word ptr [eax], 0xffff
        jne    loc_4_2ECE
loc_4_2EE5:
        add    ecx, 4
        inc    esi
        cmp    esi, 0x120
        jb     loc_4_2EBF
        mov    word ptr [edx], 0xffff
        mov    eax, 0x8005
loc_4_2EFB:
        pop    esi
        pop    ebx
        ret    
loc_4_2EFE:
        cmp    esi, 0x80
        jae    loc_4_2F09
        mov    word ptr [edx], si
loc_4_2F09:
        xor    eax, eax
        jmp    loc_4_2EFB
sub_4_2F0D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [esp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_2E56
        add    esp, 8
        push   0xffff
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_30D1
        add    esp, 0xc
        xor    eax, eax
        pop    ebp
        ret    
sub_4_2F36:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [edi]
        mov    esi, dword ptr [eax + ecx*4 + 0x662]
        test   esi, esi
        je     loc_4_2F5E
        add    esi, 0x2c
        add    edi, 8
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xor    eax, eax
        jmp    loc_4_2F67
loc_4_2F5E:
        mov    byte ptr [edi + 8], 0
        mov    eax, 0x8005
loc_4_2F67:
        pop    edi
        pop    esi
        ret    
sub_4_2F6A:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [eax + 0x14], 0x72646870
        push   ebx
        push   esi
        push   edi
        je     loc_4_2F8A
        mov    eax, 1
        jmp    loc_4_30CA
loc_4_2F8A:
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, 0x26
        add    edi, 0x1c
        mov    eax, dword ptr [ebp + 0xc]
        sub    edx, edx
        mov    eax, dword ptr [eax + 0x18]
        div    ecx
        lea    esi, [eax - 1]
        test   esi, esi
        je     loc_4_2FE2
        mov    ebx, 0x80
loc_4_2FAB:
        mov    ax, word ptr [edi + 0x16]
        test   ax, ax
        je     loc_4_2FDC
        cmp    ax, bx
        je     loc_4_2FDC
        movzx  ecx, ax
        mov    eax, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [eax]
        lea    edx, [eax + ecx]
        cmp    edx, ebx
        je     loc_4_2FDC
        movzx  edx, word ptr [edi + 0x14]
        push   edx
        add    eax, ecx
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_4_30D1
        add    esp, 0xc
loc_4_2FDC:
        add    edi, 0x26
        dec    esi
        jne    loc_4_2FAB
loc_4_2FE2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        add    eax, 0x800
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_4_3007
        mov    eax, 0x8003
        jmp    loc_4_30CA
loc_4_3007:
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_16AF
        mov    esi, eax
        test   esi, esi
        je     loc_4_302D
        push   dword ptr [ebp - 4]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, esi
        jmp    loc_4_30CA
loc_4_302D:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x10]
        add    eax, 0x80
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_4_304E
        mov    eax, 0x8003
        jmp    loc_4_30CA
loc_4_304E:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edi, ebx
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   dword ptr [ebp - 4]
        call   sub_4_0D91
        add    esp, 4
        mov    ecx, dword ptr [ebp + 0xc]
        movzx  edi, word ptr [ecx]
        xor    esi, esi
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + edi*4 + 0x662], esi
        mov    edx, ebx
        sub    edx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0x40], esi
        add    dword ptr [ebx], edx
        add    dword ptr [ebx + 4], edx
        add    dword ptr [ebx + 8], edx
        add    dword ptr [ebx + 0xc], edx
        add    dword ptr [ebx + 0x10], edx
        add    dword ptr [ebx + 0x14], edx
        add    dword ptr [ebx + 0x18], edx
        mov    eax, dword ptr [ebx + 0x1c]
        test   eax, eax
        je     loc_4_30AC
        add    eax, edx
        mov    dword ptr [ebx + 0x1c], eax
loc_4_30AC:
        mov    eax, dword ptr [ebx + 0x20]
        test   eax, eax
        je     loc_4_30B8
        add    eax, edx
        mov    dword ptr [ebx + 0x20], eax
loc_4_30B8:
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        movzx  esi, word ptr [eax]
        xor    eax, eax
        mov    dword ptr [edx + esi*4 + 0x662], ebx
loc_4_30CA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_30D1:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 0x662
        sub    esp, 0x38
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0x20], 0x120
        push   ebx
        push   esi
        push   edi
loc_4_30ED:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     loc_4_331A
        mov    dword ptr [ebp - 0x10], 1
        mov    edi, dword ptr [esi]
        cmp    word ptr [edi], 0xffff
        je     loc_4_32FD
loc_4_310E:
        cmp    word ptr [edi], 0
        je     loc_4_32C6
        mov    al, byte ptr [ebp + 0xc]
        cmp    byte ptr [edi + 3], al
        je     loc_4_312C
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_4_32C6
loc_4_312C:
        cmp    dword ptr [ebp + 0x10], 0xffff
        je     loc_4_3149
        mov    al, byte ptr [ebp + 0x10]
        cmp    byte ptr [edi + 2], al
        je     loc_4_3149
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_4_32C6
loc_4_3149:
        cmp    dword ptr [ebp + 0x10], 0x80
        jne    loc_4_315E
        mov    word ptr [esi + 0x24], 0xffff
        mov    word ptr [esi + 0x26], 0xffff
loc_4_315E:
        cmp    dword ptr [esi + 0x40], 0
        jne    loc_4_32D6
        movzx  eax, word ptr [edi + 4]
        mov    ecx, dword ptr [esi + 4]
        movzx  ebx, word ptr [edi + 0x1e]
        movzx  edx, word ptr [ecx + eax*4]
        movzx  eax, word ptr [ecx + ebx*4]
        mov    dword ptr [ebp - 0xc], edx
        mov    dword ptr [ebp - 0x1c], eax
        cmp    eax, edx
        jbe    loc_4_32C1
loc_4_3189:
        mov    eax, dword ptr [ebp - 0xc]
        imul   eax, eax, 3
        add    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax], 0x29
        jne    loc_4_32B2
        movsx  edx, word ptr [eax + 1]
        add    edx, edx
        mov    ecx, dword ptr [esi + 0x10]
        add    edx, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 4], 0
        movzx  eax, word ptr [edx]
        movzx  edx, word ptr [edx + 2]
        movzx  ebx, word ptr [ecx + eax*4]
        movzx  eax, word ptr [ecx + edx*4]
        mov    dword ptr [ebp - 0x2c], ebx
        mov    dword ptr [ebp - 0x38], eax
loc_4_31C3:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_31DA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + eax*4 - 0x2c]
        cmp    dword ptr [ebp + eax*4 - 0x38], ecx
        jbe    loc_4_32B2
loc_4_31DA:
        mov    edx, dword ptr [ebp - 4]
        shl    edx, 2
        lea    ecx, [ebp + edx - 0x2c]
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [ebp + edx - 0x38], eax
        jne    loc_4_31F1
        dec    dword ptr [ebp - 4]
        jmp    loc_4_31C3
loc_4_31F1:
        imul   eax, eax, 3
        add    eax, dword ptr [esi + 0x14]
        cmp    byte ptr [eax], 0x29
        jne    loc_4_3237
        movsx  ebx, word ptr [eax + 1]
        mov    eax, dword ptr [esi + 0xc]
        inc    dword ptr [ebp - 4]
        movzx  ebx, word ptr [eax + ebx*2]
        mov    eax, dword ptr [esi + 0x10]
        movzx  ebx, word ptr [eax + ebx*4]
        mov    dword ptr [ebp + edx - 0x28], ebx
        mov    ebx, dword ptr [esi + 0x14]
        mov    eax, dword ptr [ecx]
        imul   eax, eax, 3
        movsx  ebx, word ptr [eax + ebx + 1]
        mov    eax, dword ptr [esi + 0xc]
        inc    dword ptr [ecx]
        movzx  ebx, word ptr [eax + ebx*2 + 2]
        mov    eax, dword ptr [esi + 0x10]
        movzx  ebx, word ptr [eax + ebx*4]
        mov    dword ptr [ebp + edx - 0x34], ebx
loc_4_3237:
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [ebp + eax*4 - 0x2c]
        mov    dword ptr [ebp - 0x14], ecx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 0x18], eax
        imul   eax, eax, 3
        add    eax, dword ptr [esi + 0x14]
        cmp    byte ptr [eax], 0x35
        je     loc_4_325C
        mov    eax, dword ptr [ebp - 0x18]
        inc    eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_31C3
loc_4_325C:
        movsx  ebx, word ptr [eax + 1]
        mov    eax, dword ptr [esi + 0x18]
        imul   ebx, ebx, 0x24
        lea    eax, [eax + ebx + 0x1a]
        mov    cx, word ptr [eax]
        test   cx, cx
        je     loc_4_32A4
        dec    cx
        mov    word ptr [eax], cx
        mov    eax, dword ptr [esi + 0x18]
        add    eax, ebx
        cmp    word ptr [eax + 0x1a], 0
        jne    loc_4_32A4
        mov    ecx, dword ptr [eax + 0x1c]
        test   ecx, ecx
        je     loc_4_32A4
        push   dword ptr [eax + 0x20]
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_4_1531
        add    esp, 0xc
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ecx + ebx + 0x20], 0
loc_4_32A4:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        inc    eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_31C3
loc_4_32B2:
        inc    dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0x1c], eax
        ja     loc_4_3189
loc_4_32C1:
        mov    word ptr [edi], 0
loc_4_32C6:
        add    edi, 0x1a
        cmp    word ptr [edi], 0xffff
        jne    loc_4_310E
        jmp    loc_4_32FD
loc_4_32D6:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esi + 0x44]
        add    eax, 0x10
        push   eax
        push   dword ptr [esi + 0x40]
        push   dword ptr [ebp + 8]
        call   sub_4_1531
        add    esp, 0xc
        push   esi
        call   sub_4_0D91
        add    esp, 4
loc_4_32FD:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_4_331A
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     loc_4_331A
        push   esi
        mov    dword ptr [eax], 0
        call   sub_4_0D91
        add    esp, 4
loc_4_331A:
        add    dword ptr [ebp - 8], 4
        dec    dword ptr [ebp - 0x20]
        jne    loc_4_30ED
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_3330:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        lea    ecx, [ebp - 8]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   ecx
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   esi
        call   sub_4_2DEF
        add    esp, 0x14
        test   eax, eax
        je     loc_4_337C
        mov    eax, dword ptr [ebp - 8]
        add    edi, 8
        mov    ecx, dword ptr [esi + eax*4 + 0x662]
        mov    edx, dword ptr [ecx]
        mov    ecx, dword ptr [ebp - 4]
        imul   ecx, ecx, 0x1a
        lea    esi, [edx + ecx + 6]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xor    eax, eax
        jmp    loc_4_3385
loc_4_337C:
        mov    byte ptr [edi + 8], 0
        mov    eax, 0x8005
loc_4_3385:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_338B:
        mov    edx, 0x81
        mov    ecx, dword ptr [esp + 4]
        lea    eax, [ecx + 0x866]
loc_4_339A:
        cmp    dword ptr [eax], 0
        je     loc_4_33B1
        add    eax, 4
        inc    edx
        cmp    edx, 0x120
        jb     loc_4_339A
        mov    eax, 4
loc_4_33B0:
        ret    
loc_4_33B1:
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax], dx
        push   eax
        push   ecx
        call   sub_4_210E
        jmp    loc_4_33B0
sub_4_33C1:
        mov    edx, dword ptr [esp + 0xc]
        push   ebx
        mov    ecx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        lea    esi, [edx + 0x10]
        push   ebp
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     loc_4_3433
        mov    edi, dword ptr [esp + 0x14]
loc_4_33DB:
        lea    ebx, [edi + 0x1c]
        mov    eax, dword ptr [ebx]
        test   eax, 0x80000000
        je     loc_4_3429
        cmp    dword ptr [edi + 0x20], 0
        je     loc_4_3429
        sub    dword ptr [edi], eax
        mov    eax, dword ptr [ebx]
        sub    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebx]
        sub    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [ebx]
        sub    dword ptr [edi + 4], eax
        add    dword ptr [edi], esi
        add    dword ptr [edi + 8], esi
        add    dword ptr [edi + 0xc], esi
        add    dword ptr [edi + 4], esi
        mov    eax, dword ptr [edi + 0x20]
        test   eax, eax
        je     loc_4_3423
        mov    ebp, edx
        sub    ebp, esi
        cmp    ebp, -0x10
        jne    loc_4_3423
        add    eax, 0x10
        mov    dword ptr [edi + 0x20], eax
        mov    dword ptr [ebx], edx
        jmp    loc_4_3425
loc_4_3423:
        mov    dword ptr [ebx], esi
loc_4_3425:
        mov    esi, dword ptr [edi + 4]
        inc    esi
loc_4_3429:
        add    edi, 0x24
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        jne    loc_4_33DB
loc_4_3433:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_343A:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        add    edx, 0xa
        sub    esp, 0x3c
        mov    dword ptr [ebp - 0x18], edx
        cmp    dword ptr [edx + 0x14], 0x72646870
        push   ebx
        push   esi
        push   edi
        je     loc_4_3460
        mov    eax, 1
        jmp    loc_4_3D41
loc_4_3460:
        mov    esi, dword ptr [ebp - 0x18]
        mov    edi, 0x26
        add    esi, 0x1c
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ecx + 0x18]
        sub    edx, edx
        div    edi
        dec    eax
        je     loc_4_34A4
        mov    edx, dword ptr [ebp + 0x10]
        mov    cx, word ptr [edx + 2]
loc_4_3486:
        cmp    word ptr [esi + 0x16], cx
        jne    loc_4_3499
        mov    edx, dword ptr [ebp + 0x10]
        mov    dx, word ptr [edx + 4]
        cmp    word ptr [esi + 0x14], dx
        je     loc_4_34A4
loc_4_3499:
        add    esi, 0x26
        inc    dword ptr [ebp - 0xc]
        cmp    eax, dword ptr [ebp - 0xc]
        ja     loc_4_3486
loc_4_34A4:
        cmp    eax, dword ptr [ebp - 0xc]
        jne    loc_4_34B3
        mov    eax, 0x8005
        jmp    loc_4_3D41
loc_4_34B3:
        xor    edi, edi
        mov    edx, dword ptr [ebp + 0x10]
        cmp    word ptr [edx], di
        je     loc_4_361C
        add    edx, 8
        mov    esi, dword ptr [ebp + 0x10]
        add    esi, 6
        lea    ecx, [ebp - 8]
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 0xc], edx
        movzx  ecx, word ptr [edx]
        movzx  eax, word ptr [esi]
        push   ecx
        push   eax
        push   dword ptr [ebp + 0xc]
        call   sub_4_2DEF
        add    esp, 0x14
        test   eax, eax
        je     loc_4_35BB
        cmp    dword ptr [ebp - 4], 0x80
        jbe    loc_4_35BB
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, 0x38
        mov    dword ptr [ebp - 0x10], ecx
        lea    eax, [ecx + eax*4 + 0x62a]
        mov    ebx, dword ptr [eax]
        mov    dword ptr [ecx + 4], edi
        push   dword ptr [ebp - 0x18]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebx + 0x40]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebx + 0x44]
        push   eax
        mov    dword ptr [eax + 0x1c], edx
        call   sub_4_2F6A
        add    esp, 8
        mov    dword ptr [ebp - 0x14], eax
        cmp    eax, edi
        jne    loc_4_35AA
        mov    ecx, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edx + eax*4 + 0x662]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 0x40], edx
        mov    ecx, dword ptr [edi + 0x1c]
        mov    dword ptr [eax + 0x44], ecx
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dl, byte ptr [esi]
        mov    esi, dword ptr [eax]
        mov    byte ptr [esi + 3], dl
        mov    dl, byte ptr [ecx]
        mov    esi, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [esi + 2], dl
        mov    dword ptr [ecx], 0x80000000
        movzx  edx, word ptr [ebx + 0x2a]
        movzx  ecx, word ptr [eax + 0x2a]
        mov    dword ptr [ebp - 0x24], edx
        cmp    ecx, edx
        jae    loc_4_3585
        mov    dword ptr [ebp - 0x24], ecx
loc_4_3585:
        mov    edi, dword ptr [eax + 0x18]
        mov    esi, dword ptr [ebx + 0x18]
        imul   edx, edx, 0x24
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        push   ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    cx, word ptr [ebp - 0x24]
        mov    word ptr [eax + 0x2a], cx
        jmp    loc_4_3D37
loc_4_35AA:
        push   ebx
        call   sub_4_0D91
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x14]
        jmp    loc_4_3D41
loc_4_35BB:
        mov    ecx, dword ptr [ebp - 0xc]
        movzx  edx, word ptr [esi]
        movzx  eax, word ptr [ecx]
        push   eax
        push   edx
        push   dword ptr [ebp + 0xc]
        call   sub_4_30D1
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 0x18]
        push   ecx
        mov    dword ptr [ecx + 0x3c], 0
        mov    dword ptr [ecx + 0x38], 0x7ffffff0
        call   sub_4_2F6A
        add    esp, 8
        test   eax, eax
        jne    loc_4_3D41
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ebx, dword ptr [ebp + 0xc]
        movzx  edx, word ptr [ecx]
        mov    edx, dword ptr [ebx + edx*4 + 0x662]
        mov    bl, byte ptr [esi]
        mov    esi, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [edx]
        mov    byte ptr [ecx + 3], bl
        mov    bl, byte ptr [esi]
        mov    ecx, dword ptr [edx]
        mov    byte ptr [ecx + 2], bl
        jmp    loc_4_3D41
loc_4_361C:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ecx + 0xc]
        add    eax, 0x800
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 0x30], eax
        test   eax, eax
        jne    loc_4_3641
        mov    eax, 0x8003
        jmp    loc_4_3D41
loc_4_3641:
        mov    edx, dword ptr [ebp + 0x10]
        movzx  eax, word ptr [edx + 8]
        movzx  ecx, word ptr [edx + 6]
        push   eax
        push   ecx
        push   dword ptr [ebp + 0xc]
        call   sub_4_30D1
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp - 0x18]
        push   ecx
        mov    dword ptr [ecx + 0x3c], 0
        mov    dword ptr [ecx + 0x38], 0x7ffffff0
        call   sub_4_16AF
        mov    edi, eax
        test   edi, edi
        je     loc_4_368E
        push   dword ptr [ebp - 0x30]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, edi
        jmp    loc_4_3D41
loc_4_368E:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    edx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx]
        mov    ebx, dword ptr [edx + eax*4 + 0x662]
        movzx  eax, word ptr [ebx + 0x28]
        shl    eax, 2
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 0x1c], eax
        test   eax, eax
        jne    loc_4_36CA
        push   dword ptr [ebp - 0x30]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, 0x8003
        jmp    loc_4_3D41
loc_4_36CA:
        movzx  eax, word ptr [ebx + 0x2a]
        shl    eax, 2
        push   eax
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 0x34], eax
        test   eax, eax
        jne    loc_4_3701
        push   dword ptr [ebp - 0x1c]
        call   sub_4_0D91
        add    esp, 4
        push   dword ptr [ebp - 0x30]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, 0x8003
        jmp    loc_4_3D41
loc_4_3701:
        xor    eax, eax
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x2c], eax
        cmp    dword ptr [ebp - 0xc], eax
        jne    loc_4_3726
        mov    eax, dword ptr [ebx]
        mov    esi, dword ptr [ebx + 4]
        movzx  ecx, word ptr [eax + 0x1e]
        movzx  edx, word ptr [esi + ecx*4]
        mov    dword ptr [ebp - 0x2c], ecx
        mov    dword ptr [ebp - 0x14], edx
        jmp    loc_4_37D6
loc_4_3726:
        mov    edi, dword ptr [ebx]
        mov    edx, dword ptr [ebp - 0xc]
        imul   edx, edx, 0x1a
        mov    ecx, 6
        lea    esi, [edi + edx]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    edi, dword ptr [ebx]
        xor    ecx, ecx
        mov    word ptr [edi + 4], cx
        mov    edx, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [ebx]
        imul   edx, edx, 0x1a
        movzx  eax, word ptr [edi + edx + 4]
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    edi, dword ptr [ebx]
        imul   edx, edx, 0x1a
        movzx  eax, word ptr [edi + edx + 0x1e]
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_4_37D6
loc_4_3764:
        mov    dx, word ptr [ebp - 0x14]
        mov    eax, dword ptr [ebx + 4]
        mov    word ptr [eax + ecx], dx
        inc    dword ptr [ebp - 0x2c]
        add    ecx, 4
        mov    edx, dword ptr [ebp - 4]
        shl    edx, 2
        mov    esi, dword ptr [ebx + 4]
        movzx  eax, word ptr [esi + edx]
        mov    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebx + 4]
        movzx  edx, word ptr [esi + edx + 4]
        cmp    edx, eax
        jbe    loc_4_37C1
        mov    edx, dword ptr [ebp - 0x14]
        imul   edx, edx, 3
loc_4_3797:
        mov    eax, dword ptr [ebx + 8]
        mov    esi, dword ptr [ebp - 8]
        imul   esi, esi, 3
        lea    edi, [edx + eax]
        add    esi, eax
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        add    edx, 3
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 0x14]
        inc    dword ptr [ebp - 8]
        mov    esi, dword ptr [ebx + 4]
        movzx  edi, word ptr [esi + eax*4 + 4]
        cmp    edi, dword ptr [ebp - 8]
        ja     loc_4_3797
loc_4_37C1:
        mov    edx, dword ptr [ebp - 0xc]
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebx]
        imul   edx, edx, 0x1a
        movzx  eax, word ptr [eax + edx + 0x1e]
        cmp    eax, dword ptr [ebp - 4]
        ja     loc_4_3764
loc_4_37D6:
        mov    edx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebx]
        mov    cl, byte ptr [edx + 6]
        mov    byte ptr [eax + 3], cl
        mov    cl, byte ptr [edx + 8]
        mov    eax, dword ptr [ebx]
        mov    byte ptr [eax + 2], cl
        mov    eax, dword ptr [ebx]
        mov    word ptr [eax + 0x1a], 0xffff
        mov    cx, word ptr [ebp - 0x2c]
        mov    eax, dword ptr [ebx]
        mov    byte ptr [eax + 0x1d], 0xff
        mov    dx, word ptr [ebp - 0x14]
        mov    eax, dword ptr [ebx]
        cmp    dword ptr [ebp - 0x14], 0
        mov    word ptr [eax + 0x1e], cx
        mov    ecx, dword ptr [ebp - 0x2c]
        mov    esi, dword ptr [ebx + 4]
        mov    word ptr [esi + ecx*4], dx
        je     loc_4_3892
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0x14]
loc_4_381A:
        mov    eax, dword ptr [ebx + 8]
        add    eax, edx
        cmp    byte ptr [eax], 0x29
        jne    loc_4_388C
        movsx  esi, word ptr [eax + 1]
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + esi*4], 1
        mov    edi, dword ptr [ebx + 0xc]
        movzx  eax, word ptr [edi + esi*2]
        mov    edi, eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebx + 0x10]
        movzx  edi, word ptr [eax + edi*4]
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebx + 0xc]
        movzx  esi, word ptr [eax + esi*2 + 2]
        mov    dword ptr [ebp - 8], esi
        mov    eax, dword ptr [ebx + 0x10]
        movzx  edi, word ptr [eax + esi*4]
        mov    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], edi
        jbe    loc_4_388C
loc_4_3865:
        mov    eax, dword ptr [ebp - 4]
        imul   eax, eax, 3
        add    eax, dword ptr [ebx + 0x14]
        cmp    byte ptr [eax], 0x29
        jne    loc_4_3881
        movsx  esi, word ptr [eax + 1]
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [eax + esi*4], 1
loc_4_3881:
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        ja     loc_4_3865
loc_4_388C:
        add    edx, 3
        dec    ecx
        jne    loc_4_381A
loc_4_3892:
        mov    edx, 1
        xor    eax, eax
        cmp    word ptr [ebx + 0x28], ax
        jbe    loc_4_38B6
        mov    ecx, dword ptr [ebp - 0x1c]
loc_4_38A2:
        cmp    dword ptr [ecx], 0
        je     loc_4_38AA
        mov    dword ptr [ecx], edx
        inc    edx
loc_4_38AA:
        add    ecx, 4
        inc    eax
        movzx  esi, word ptr [ebx + 0x28]
        cmp    esi, eax
        ja     loc_4_38A2
loc_4_38B6:
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_4_3940
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0x14]
loc_4_38C5:
        mov    edi, dword ptr [ebx + 8]
        add    edi, edx
        cmp    byte ptr [edi], 0x29
        jne    loc_4_393A
        mov    eax, dword ptr [ebp - 0x1c]
        movsx  esi, word ptr [edi + 1]
        mov    eax, dword ptr [eax + esi*4]
        dec    ax
        mov    word ptr [edi + 1], ax
        mov    edi, dword ptr [ebx + 0xc]
        movzx  eax, word ptr [edi + esi*2]
        mov    edi, eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebx + 0x10]
        movzx  edi, word ptr [eax + edi*4]
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [ebx + 0xc]
        movzx  esi, word ptr [eax + esi*2 + 2]
        mov    edi, esi
        mov    dword ptr [ebp - 8], esi
        mov    esi, dword ptr [ebx + 0x10]
        movzx  eax, word ptr [esi + edi*4]
        mov    dword ptr [ebp - 8], eax
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_4_393A
loc_4_3911:
        mov    esi, dword ptr [ebp - 4]
        imul   esi, esi, 3
        add    esi, dword ptr [ebx + 0x14]
        cmp    byte ptr [esi], 0x29
        jne    loc_4_392F
        mov    eax, dword ptr [ebp - 0x1c]
        movsx  edi, word ptr [esi + 1]
        mov    eax, dword ptr [eax + edi*4]
        dec    ax
        mov    word ptr [esi + 1], ax
loc_4_392F:
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        ja     loc_4_3911
loc_4_393A:
        add    edx, 3
        dec    ecx
        jne    loc_4_38C5
loc_4_3940:
        xor    esi, esi
        xor    ecx, ecx
        cmp    word ptr [ebx + 0x28], si
        mov    dword ptr [ebp - 0x28], esi
        jbe    loc_4_396B
        mov    edx, dword ptr [ebp - 0x1c]
        mov    eax, edx
loc_4_3952:
        cmp    dword ptr [eax], 0
        je     loc_4_395F
        mov    dword ptr [edx], ecx
        inc    dword ptr [ebp - 0x28]
        add    edx, 4
loc_4_395F:
        add    eax, 4
        inc    ecx
        movzx  edi, word ptr [ebx + 0x28]
        cmp    edi, ecx
        ja     loc_4_3952
loc_4_396B:
        xor    ecx, ecx
        mov    dword ptr [ebp - 0x10], ecx
        mov    dword ptr [ebp - 0x20], ecx
        mov    dword ptr [ebp - 0xc], ecx
        cmp    dword ptr [ebp - 0x28], ecx
        je     loc_4_3A6F
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebp - 0x24], eax
loc_4_3985:
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebp - 0xc], eax
        jne    loc_4_39A9
        mov    eax, dword ptr [ebx + 0xc]
        mov    edi, dword ptr [ebx + 0x10]
        movzx  esi, word ptr [eax + ecx + 2]
        movzx  edx, word ptr [edi + esi*4]
        mov    dword ptr [ebp - 0x20], esi
        mov    dword ptr [ebp - 0x10], edx
        jmp    loc_4_3A59
loc_4_39A9:
        mov    di, word ptr [ebp - 0x20]
        mov    edx, dword ptr [ebx + 0xc]
        mov    si, word ptr [edx + eax*2]
        mov    word ptr [ecx + edx], si
        mov    edx, dword ptr [ebx + 0xc]
        mov    word ptr [edx + ecx], di
        mov    edx, dword ptr [ebx + 0xc]
        movzx  esi, word ptr [edx + eax*2]
        mov    dword ptr [ebp - 4], esi
        mov    edx, dword ptr [ebx + 0xc]
        movzx  edi, word ptr [edx + eax*2 + 2]
        cmp    edi, esi
        jbe    loc_4_3A59
        mov    esi, dword ptr [ebp - 0x20]
        shl    esi, 2
        mov    dword ptr [ebp - 0x3c], esi
loc_4_39E2:
        mov    dx, word ptr [ebp - 0x10]
        mov    edi, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [ebp - 0x3c]
        inc    dword ptr [ebp - 0x20]
        mov    word ptr [edi + esi], dx
        mov    esi, dword ptr [ebp - 4]
        shl    esi, 2
        mov    edx, dword ptr [ebx + 0x10]
        add    dword ptr [ebp - 0x3c], 4
        movzx  edi, word ptr [edx + esi]
        mov    dword ptr [ebp - 8], edi
        mov    edx, dword ptr [ebx + 0x10]
        movzx  esi, word ptr [edx + esi + 4]
        cmp    esi, edi
        jbe    loc_4_3A49
        mov    edx, dword ptr [ebp - 0x10]
        imul   edx, edx, 3
        mov    dword ptr [ebp - 0x38], edx
loc_4_3A1C:
        mov    edx, dword ptr [ebx + 0x14]
        mov    esi, dword ptr [ebp - 8]
        imul   esi, esi, 3
        mov    edi, dword ptr [ebp - 0x38]
        add    esi, edx
        add    edi, edx
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 0x10]
        inc    dword ptr [ebp - 8]
        mov    edi, dword ptr [ebx + 0x10]
        add    dword ptr [ebp - 0x38], 3
        movzx  esi, word ptr [edi + edx*4 + 4]
        cmp    esi, dword ptr [ebp - 8]
        ja     loc_4_3A1C
loc_4_3A49:
        inc    dword ptr [ebp - 4]
        mov    edx, dword ptr [ebx + 0xc]
        movzx  esi, word ptr [edx + eax*2 + 2]
        cmp    esi, dword ptr [ebp - 4]
        ja     loc_4_39E2
loc_4_3A59:
        add    ecx, 2
        mov    edi, dword ptr [ebp - 0x28]
        add    dword ptr [ebp - 0x24], 4
        inc    dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], edi
        jb     loc_4_3985
loc_4_3A6F:
        mov    si, word ptr [ebp - 0x20]
        mov    eax, dword ptr [ebx + 0xc]
        mov    dx, word ptr [ebp - 0x10]
        mov    edi, dword ptr [ebp - 0x28]
        mov    word ptr [eax + edi*2], si
        xor    ecx, ecx
        mov    eax, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [ebp - 0x20]
        mov    word ptr [eax + esi*4], dx
        cmp    word ptr [ebx + 0x2a], cx
        jbe    loc_4_3AAB
        xor    edx, edx
loc_4_3A95:
        mov    eax, dword ptr [ebx + 0x18]
        add    edx, 0x24
        mov    word ptr [eax + edx - 0xa], 0
        movzx  eax, word ptr [ebx + 0x2a]
        inc    ecx
        cmp    eax, ecx
        ja     loc_4_3A95
loc_4_3AAB:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_4_3AE7
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0x10]
loc_4_3AB6:
        mov    eax, dword ptr [ebx + 0x14]
        add    eax, edx
        cmp    byte ptr [eax], 0x35
        jne    loc_4_3AE1
        mov    esi, dword ptr [ebp - 0x34]
        movsx  eax, word ptr [eax + 1]
        mov    dword ptr [esi + eax*4], 1
        mov    esi, dword ptr [ebx + 0x18]
        mov    edi, dword ptr [ebx + 0x14]
        movsx  eax, word ptr [edi + edx + 1]
        imul   eax, eax, 0x24
        inc    word ptr [eax + esi + 0x1a]
loc_4_3AE1:
        add    edx, 3
        dec    ecx
        jne    loc_4_3AB6
loc_4_3AE7:
        mov    ecx, 1
        xor    eax, eax
        cmp    word ptr [ebx + 0x2a], ax
        jbe    loc_4_3B0B
        mov    edx, dword ptr [ebp - 0x34]
loc_4_3AF7:
        cmp    dword ptr [edx], 0
        je     loc_4_3AFF
        mov    dword ptr [edx], ecx
        inc    ecx
loc_4_3AFF:
        add    edx, 4
        inc    eax
        movzx  esi, word ptr [ebx + 0x2a]
        cmp    esi, eax
        ja     loc_4_3AF7
loc_4_3B0B:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_4_3B36
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0x10]
loc_4_3B16:
        mov    edi, dword ptr [ebx + 0x14]
        add    edi, edx
        cmp    byte ptr [edi], 0x35
        jne    loc_4_3B30
        mov    esi, dword ptr [ebp - 0x34]
        movsx  eax, word ptr [edi + 1]
        mov    eax, dword ptr [esi + eax*4]
        dec    ax
        mov    word ptr [edi + 1], ax
loc_4_3B30:
        add    edx, 3
        dec    ecx
        jne    loc_4_3B16
loc_4_3B36:
        xor    edx, edx
        cmp    word ptr [ebx + 0x2a], dx
        mov    dword ptr [ebp - 0x24], edx
        mov    dword ptr [ebp - 0xc], edx
        jbe    loc_4_3B83
        xor    eax, eax
        mov    esi, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0x3c], edx
        mov    dword ptr [ebp - 0x38], esi
loc_4_3B4F:
        mov    ecx, dword ptr [ebp - 0x38]
        cmp    dword ptr [ecx], 0
        je     loc_4_3B6F
        mov    ecx, dword ptr [ebx + 0x18]
        mov    esi, dword ptr [ebp - 0x3c]
        add    esi, ecx
        lea    edi, [eax + ecx]
        mov    ecx, 9
        rep movsd dword ptr es:[edi], dword ptr [esi]
        add    eax, 0x24
        inc    dword ptr [ebp - 0x24]
loc_4_3B6F:
        add    dword ptr [ebp - 0x3c], 0x24
        movzx  ecx, word ptr [ebx + 0x2a]
        add    dword ptr [ebp - 0x38], 4
        inc    dword ptr [ebp - 0xc]
        cmp    ecx, dword ptr [ebp - 0xc]
        ja     loc_4_3B4F
loc_4_3B83:
        push   dword ptr [ebp - 0x1c]
        call   sub_4_0D91
        add    esp, 4
        push   dword ptr [ebp - 0x34]
        call   sub_4_0D91
        add    esp, 4
        mov    ecx, dword ptr [ebp - 0x24]
        mov    edi, dword ptr [ebp - 0x28]
        mov    eax, dword ptr [ebp - 0x10]
        imul   ecx, ecx, 9
        add    ecx, dword ptr [ebp - 0x20]
        add    eax, dword ptr [ebp - 0x14]
        add    ecx, dword ptr [ebp - 0x2c]
        imul   eax, eax, 3
        lea    edx, [edi + ecx*2 + 0x54]
        mov    ecx, dword ptr [ebp - 0x18]
        lea    eax, [eax + edx*2]
        push   eax
        mov    dword ptr [ecx + 0x10], eax
        call   sub_4_0D74
        add    esp, 4
        test   eax, eax
        jne    loc_4_3BE0
        push   dword ptr [ebp - 0x30]
        call   sub_4_0D91
        add    esp, 4
        mov    eax, 0x8003
        jmp    loc_4_3D41
loc_4_3BE0:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    esi, dword ptr [ebp + 0xc]
        movzx  edx, word ptr [ecx]
        mov    edi, eax
        mov    ecx, 0x12
        mov    dword ptr [esi + edx*4 + 0x662], 0
        mov    esi, ebx
        mov    edx, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        add    eax, 0x4c
        mov    esi, dword ptr [edx]
        mov    edi, eax
        mov    ecx, 0xd
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [edx], eax
        add    eax, 0x34
        mov    ecx, eax
        mov    esi, dword ptr [edx + 4]
        and    ecx, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 0x2c]
        add    eax, 4
        lea    ecx, [ecx*4 + 4]
        shr    ecx, 2
        mov    edi, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [ebp - 0x2c]
        mov    dword ptr [edx + 4], eax
        mov    ebx, dword ptr [ebp - 0x14]
        lea    eax, [eax + ecx*4 + 4]
        mov    ecx, eax
        mov    esi, dword ptr [edx + 8]
        imul   ebx, ebx, 3
        and    ecx, 3
        sub    eax, ecx
        mov    ecx, ebx
        shr    ecx, 2
        add    eax, 4
        mov    edi, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [ebp - 0x28]
        mov    dword ptr [edx + 8], eax
        add    eax, ebx
        lea    ebx, [edi*2 + 2]
        mov    ecx, eax
        mov    esi, dword ptr [edx + 0xc]
        and    ecx, 3
        sub    eax, ecx
        mov    ecx, ebx
        add    eax, 4
        mov    edi, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [ebp - 0x28]
        mov    dword ptr [edx + 0xc], eax
        mov    esi, dword ptr [ebp - 0x20]
        lea    eax, [eax + edi*2 + 2]
        mov    ecx, eax
        and    ecx, 3
        sub    eax, ecx
        lea    ecx, [esi*4 + 4]
        shr    ecx, 2
        add    eax, 4
        mov    edi, eax
        mov    esi, dword ptr [edx + 0x10]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, dword ptr [ebp - 0x20]
        mov    dword ptr [edx + 0x10], eax
        mov    ebx, dword ptr [ebp - 0x10]
        lea    eax, [eax + esi*4 + 4]
        mov    ecx, eax
        mov    esi, dword ptr [edx + 0x14]
        imul   ebx, ebx, 3
        and    ecx, 3
        sub    eax, ecx
        mov    ecx, ebx
        shr    ecx, 2
        add    eax, 4
        mov    edi, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [edx + 0x14], eax
        add    eax, ebx
        mov    ecx, eax
        mov    ebx, dword ptr [ebp - 0x24]
        and    ecx, 3
        mov    esi, dword ptr [edx + 0x18]
        imul   ebx, ebx, 0x24
        sub    eax, ecx
        mov    ecx, ebx
        shr    ecx, 2
        add    eax, 4
        mov    edi, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    di, word ptr [ebp - 0x28]
        mov    dword ptr [edx + 0x18], eax
        mov    dword ptr [edx + 0x40], 0
        mov    word ptr [edx + 0x28], di
        mov    bx, word ptr [ebp - 0x24]
        mov    ecx, dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x30]
        mov    word ptr [edx + 0x2a], bx
        mov    ebx, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [ecx]
        mov    dword ptr [ebx + eax*4 + 0x662], edx
loc_4_3D37:
        call   sub_4_0D91
        add    esp, 4
        xor    eax, eax
loc_4_3D41:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_3D48:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        cmp    dword ptr [esi + 0x38], 0x1000000
        jb     loc_4_3E20
        xor    ebx, ebx
        mov    edx, dword ptr [esp + 0x18]
        movzx  eax, word ptr [edx]
        mov    edi, dword ptr [esi + eax*4 + 0x662]
        mov    ax, word ptr [edi + 0x2a]
        mov    ebp, dword ptr [edi + 0x18]
        test   ax, ax
        je     loc_4_3D8E
        movzx  eax, ax
        lea    ecx, [ebp + 0x1c]
loc_4_3D7F:
        test   byte ptr [ecx + 3], 0x80
        je     loc_4_3D88
        add    ebx, dword ptr [ecx + 4]
loc_4_3D88:
        add    ecx, 0x24
        dec    eax
        jne    loc_4_3D7F
loc_4_3D8E:
        xor    eax, eax
        test   ebx, ebx
        jne    loc_4_3DA0
        mov    edx, dword ptr [esp + 0x18]
        mov    dword ptr [edx + 0x14], eax
        jmp    loc_4_3E2A
loc_4_3DA0:
        lea    eax, [ebx + 0x10]
        cmp    dword ptr [esi + 0xc], eax
        jae    loc_4_3DAF
        mov    eax, 1
        jmp    loc_4_3E2A
loc_4_3DAF:
        push   eax
        push   esi
        call   sub_4_11BC
        add    esp, 8
        xor    ecx, ecx
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [esi + 0x1c], ebx
        mov    dword ptr [edi + 0x40], eax
        mov    dword ptr [edi + 0x44], ebx
        mov    dword ptr [esi + 0x24], ecx
        mov    esi, dword ptr [esp + 0x18]
        add    esi, 0x14
        cmp    word ptr [edi + 0x2a], cx
        jbe    loc_4_3E05
loc_4_3DD7:
        mov    ebx, dword ptr [ebp + 0x1c]
        test   ebx, 0x80000000
        je     loc_4_3DF9
        mov    edx, dword ptr [esp + 0x18]
        mov    edx, dword ptr [edx + 4]
        lea    ebx, [edx + ebx*2]
        mov    dword ptr [esi], ebx
        add    esi, 8
        mov    edx, dword ptr [ebp + 0x20]
        add    edx, edx
        mov    dword ptr [esi - 4], edx
loc_4_3DF9:
        inc    ecx
        add    ebp, 0x24
        movzx  edx, word ptr [edi + 0x2a]
        cmp    edx, ecx
        ja     loc_4_3DD7
loc_4_3E05:
        push   eax
        mov    dword ptr [esi], 0
        movzx  edx, word ptr [edi + 0x2a]
        push   edx
        push   dword ptr [edi + 0x18]
        call   sub_4_33C1
        add    esp, 0xc
        xor    eax, eax
        jmp    loc_4_3E2A
loc_4_3E20:
        push   dword ptr [esp + 0x18]
        push   esi
        call   sub_4_1E12
loc_4_3E2A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_3E2F:
        push   esi
        mov    ecx, 5
        push   edi
        mov    esi, 0                        ; -> obj5:0x0AA0
        mov    edi, dword ptr [esp + 0xc]
        mov    word ptr [edi], 1
        add    edi, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        xor    eax, eax
        pop    edi
        pop    esi
        ret    
sub_4_3E50:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        movzx  eax, word ptr [edi]
        test   eax, eax
        je     loc_4_3E67
        mov    eax, 1
        jmp    loc_4_3EF7
loc_4_3E67:
        mov    cx, word ptr [edi + 2]
        movzx  eax, cx
        test   eax, eax
        je     loc_4_3E88
        cmp    eax, 1
        je     loc_4_3EA3
        cmp    eax, 2
        jl     loc_4_3E81
        cmp    eax, 3
        jle    loc_4_3EBE
loc_4_3E81:
        mov    eax, 1
        jmp    loc_4_3EF7
loc_4_3E88:
        mov    esi, 0                        ; -> obj5:0x0AC0
        mov    ecx, 0x28
        mov    word ptr [edi + 4], 8
        mov    word ptr [edi + 6], 4
        add    edi, 8
        jmp    loc_4_3EF3
loc_4_3EA3:
        mov    esi, 0                        ; -> obj5:0x0B60
        mov    ecx, 0x28
        mov    word ptr [edi + 4], 8
        mov    word ptr [edi + 6], 2
        add    edi, 8
        jmp    loc_4_3EF3
loc_4_3EBE:
        mov    word ptr [edi + 4], 0xc
        mov    eax, dword ptr [esp + 0xc]
        mov    word ptr [edi + 6], 5
        cmp    dword ptr [eax + 0x151e], 0x4d474d31
        jne    loc_4_3EE6
        cmp    cx, 3
        jne    loc_4_3EE6
        mov    word ptr [edi + 6], 9
loc_4_3EE6:
        mov    esi, 0                        ; -> obj5:0x0C00
        add    edi, 8
        mov    ecx, 0x3c
loc_4_3EF3:
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xor    eax, eax
loc_4_3EF7:
        pop    edi
        pop    esi
        ret    
sub_4_3EFA:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        movzx  eax, word ptr [edi]
        test   eax, eax
        jne    loc_4_3F8D
        mov    esi, dword ptr [esp + 0xc]
        cmp    word ptr [esi + 0x12], 0
        je     loc_4_3F25
        push   esi
        call   sub_4_2380
        mov    word ptr [esi + 0x12], 0
        add    esp, 4
loc_4_3F25:
        mov    cl, byte ptr [edi + 2]
        cmp    cl, 8
        jge    loc_4_3F8D
        mov    al, byte ptr [edi + 3]
        cmp    al, 8
        jge    loc_4_3F8D
        cmp    cl, 0xff
        jl     loc_4_3F8D
        cmp    al, 0xff
        jl     loc_4_3F8D
        mov    al, byte ptr [edi + 4]
        cmp    al, 0xc
        jge    loc_4_3F8D
        mov    dl, byte ptr [edi + 5]
        cmp    dl, 0xc
        jge    loc_4_3F8D
        cmp    al, 0xff
        jl     loc_4_3F8D
        cmp    dl, 0xff
        jl     loc_4_3F8D
        movsx  eax, cl
        push   eax
        push   esi
        call   sub_4_264C
        add    esp, 8
        movsx  ecx, byte ptr [edi + 3]
        push   ecx
        push   esi
        call   sub_4_25AA
        movsx  ecx, byte ptr [edi + 5]
        add    esp, 8
        movsx  eax, byte ptr [edi + 4]
        push   ecx
        push   eax
        push   esi
        call   sub_4_2458
        add    esp, 0xc
        mov    ecx, dword ptr [esi + 0x14]
        xor    eax, eax
        mov    dword ptr [edi + 2], ecx
        jmp    loc_4_3F92
loc_4_3F8D:
        mov    eax, 1
loc_4_3F92:
        pop    edi
        pop    esi
        ret    
sub_4_3F95:
        push   esi
        mov    cl, 0x1e
        mov    esi, dword ptr [esp + 8]
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    al, byte ptr [esi + 0x458]
        mov    byte ptr [edi], al
        mov    al, byte ptr [esi + 0x44c]
        imul   cl
        mov    byte ptr [edi + 1], al
        mov    al, byte ptr [esi + 0x446]
        mov    byte ptr [edi + 0x40], al
        mov    dl, byte ptr [esi + 0x44b]
        mov    byte ptr [edi + 7], dl
        mov    cl, byte ptr [esi + 0x449]
        mov    byte ptr [edi + 0xa], cl
        mov    ecx, 9
        mov    al, byte ptr [esi + 0x44a]
        mov    byte ptr [edi + 0xb], al
        movzx  eax, byte ptr [esi + 0x447]
        imul   eax, eax, 0xa
        cdq    
        idiv   ecx
        mov    byte ptr [edi + 0x5b], al
        movzx  eax, byte ptr [esi + 0x448]
        imul   eax, eax, 0xa
        cdq    
        idiv   ecx
        mov    byte ptr [edi + 0x5d], al
        xor    eax, eax
        pop    edi
        pop    esi
        ret    
sub_4_4002:
        sub    esp, 0xc
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        push   edi
        push   ebp
        xor    edi, edi
        mov    ebp, 0xff
loc_4_4014:
        push   0
        push   0
        push   ebp
        push   esi
        call   sub_1_1A68
loc_4_401F:
        mov    byte ptr [esp + edi + 0x20], al
        add    esp, 0x10
        cmp    eax, ebp
        mov    ebx, eax
        je     loc_4_40AB
        shl    eax, 5
        cmp    byte ptr [eax + esi + 0x59], 0
        je     loc_4_4039
        dec    dword ptr [esi + 0x18]
loc_4_4039:
        push   0x80
        mov    eax, ebx
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19AA
loc_4_404C:
        push   0
        mov    eax, ebx
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4059:
        push   0
        mov    eax, ebx
        or     al, 0x40
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4066:
        push   0
        mov    eax, ebx
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4073:
        push   0
        mov    eax, ebx
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4080:
        push   0x40000000
        mov    eax, ebx
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4090:
        push   0x40000000
        push   ebx
        push   esi
        or     bh, 2
        call   sub_1_1A14
loc_4_409F:
        push   0x4000000
        push   ebx
        push   esi
        call   sub_1_1A14
loc_4_40AB:
        inc    edi
        cmp    edi, 0xa
        jb     loc_4_4014
        mov    ebx, dword ptr [esp + 0x24]
        push   dword ptr [ebx]
        push   0x234
        lea    edi, [ebx + 8]
        push   esi
        add    ebx, 4
        call   sub_1_1A14
loc_4_40CC:
        cmp    dword ptr [ebx], 0
        je     loc_4_40E9
loc_4_40D1:
        push   0x23a
        push   esi
        call   sub_1_19DF
loc_4_40DC:
        mov    word ptr [edi], ax
        add    edi, 2
        mov    eax, dword ptr [ebx]
        dec    eax
        mov    dword ptr [ebx], eax
        jne    loc_4_40D1
loc_4_40E9:
        xor    edi, edi
loc_4_40EB:
        movzx  ebx, byte ptr [esp + edi + 0x10]
        cmp    ebx, 0xff
        je     loc_4_4149
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4106:
        push   0x1d8
        mov    eax, ebx
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4116:
        push   0x1e0
        mov    eax, ebx
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4126:
        push   0x48000000
        mov    eax, ebx
        shl    ebx, 5
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4139:
        lea    eax, [ebx + esi]
        mov    word ptr [eax + 0x42], 0xffe
        mov    byte ptr [eax + 0x59], 1
        inc    dword ptr [esi + 0x18]
loc_4_4149:
        inc    edi
        cmp    edi, 0xa
        jb     loc_4_40EB
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_4_4159:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [edi]
        cmp    eax, 1
        je     loc_4_4176
        cmp    eax, 2
        je     loc_4_419A
        mov    eax, 0x8006
        jmp    loc_4_421C
loc_4_4176:
        add    edi, 4
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [edi], 0x41
        cmp    dword ptr [esi + 0x154e], 0
        je     loc_4_4192
        mov    dword ptr [edi], 0x51
loc_4_4192:
        or     byte ptr [edi], 0x20
        jmp    loc_4_421A
loc_4_419A:
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 0x1558], 0x10
        je     loc_4_41B0
        mov    dword ptr [edi + 4], 0xffffffff
        jmp    loc_4_41BA
loc_4_41B0:
        movzx  eax, byte ptr [esi + 0x155c]
        mov    dword ptr [edi + 4], eax
loc_4_41BA:
        movzx  eax, byte ptr [esi + 0x155d]
        mov    dword ptr [edi + 8], eax
        movzx  ecx, word ptr [esi + 0x155e]
        mov    dword ptr [edi + 0xc], ecx
        mov    bx, word ptr [esi + 0x24]
        movzx  edx, word ptr [esi + 0x26]
        movzx  eax, bx
        mov    ecx, eax
        sub    ecx, edx
        cmp    ecx, -4
        jne    loc_4_41F9
        xor    eax, eax
        mov    ax, bx
        and    eax, 0xfff8
        mov    dword ptr [edi + 0x10], eax
        xor    eax, eax
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [edi + 0x18], eax
        jmp    loc_4_420A
loc_4_41F9:
        mov    dword ptr [edi + 0x10], eax
        movzx  eax, word ptr [esi + 0x26]
        mov    dword ptr [edi + 0x14], eax
        movzx  ecx, word ptr [esi + 0x28]
        mov    dword ptr [edi + 0x18], ecx
loc_4_420A:
        add    esi, 0x1560
        add    edi, 0x1c
        mov    ecx, 0x20
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_4_421A:
        xor    eax, eax
loc_4_421C:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_4220:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x152e]
        or     al, 4
        movzx  edx, ax
        push   edx
        push   0x23f
        push   ecx
        call   sub_1_1942
loc_4_423C:
        xor    eax, eax
        ret    
sub_4_423F:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   0x23e
        push   esi
        call   sub_1_1976
loc_4_4250:
        movzx  ax, al
        mov    di, ax
        and    di, 0x40
        shr    di, 1
        mov    cx, ax
        push   0x23d
        and    cx, 0x90
        push   esi
        or     di, cx
        shr    di, 1
        and    ax, 0x20
        or     di, ax
        call   sub_1_1976
loc_4_427E:
        shr    di, 2
        push   0
        xor    cx, cx
        push   0x23f
        mov    cl, al
        push   esi
        and    cx, 1
        or     di, cx
        mov    word ptr [esi + 0x152e], di
        call   sub_1_1942
loc_4_42A2:
        xor    eax, eax
        pop    edi
        pop    esi
        ret    
sub_4_42A7:
        push   ebp
        movzx  eax, word ptr [esp + 0xc]
        mov    ebp, esp
        sub    eax, 0x14
        sub    esp, 4
        cmp    eax, 0x2b
        push   esi
        push   edi
        ja     loc_4_42C3
        jmp    dword ptr [eax*4]             ; -> obj4:0x461D
loc_4_42C3:
        mov    eax, 0x8006
        jmp    loc_4_43EA
loc_4_42CD:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_210E
        jmp    loc_4_43EA
loc_4_42DD:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_1E12
        jmp    loc_4_43EA
loc_4_42ED:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_2F6A
        add    esp, 8
        jmp    loc_4_43EA
loc_4_4300:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_338B
        add    esp, 8
        jmp    loc_4_43EA
loc_4_4313:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_4_343A
        add    esp, 0xc
        jmp    loc_4_43EA
loc_4_4329:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_3D48
        add    esp, 8
        jmp    loc_4_43EA
loc_4_433C:
        mov    ecx, dword ptr [ebp + 0x14]
        movzx  eax, word ptr [ecx]
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_4_2E56
        add    esp, 8
        jmp    loc_4_43EA
loc_4_4353:
        mov    ecx, dword ptr [ebp + 0x14]
        movzx  eax, word ptr [ecx]
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_4_2F0D
        add    esp, 8
        jmp    loc_4_43EA
loc_4_436A:
        mov    ecx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ecx]
        mov    ecx, eax
        shr    eax, 0x10
        and    ecx, 0xffff
        push   ecx
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_4_30D1
        add    esp, 0xc
        jmp    loc_4_43EA
loc_4_4389:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_2F36
        add    esp, 8
        jmp    loc_4_43EA
loc_4_4399:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_3330
        add    esp, 8
        jmp    loc_4_43EA
loc_4_43A9:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_2EAD
        add    esp, 8
        jmp    loc_4_43EA
loc_4_43B9:
        push   dword ptr [ebp + 0x10]
        call   sub_4_2357
        jmp    loc_4_43EA
loc_4_43C3:
        mov    ecx, dword ptr [ebp + 0x14]
        push   dword ptr [ecx]
        push   dword ptr [ebp + 0x10]
        call   sub_4_11BC
        add    esp, 8
        jmp    loc_4_43EA
loc_4_43D5:
        mov    ecx, dword ptr [ebp + 0x14]
        push   dword ptr [ecx + 4]
        push   dword ptr [ecx]
        push   dword ptr [ebp + 0x10]
        call   sub_4_1531
        add    esp, 0xc
loc_4_43E8:
        xor    eax, eax
loc_4_43EA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_4_43F0:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_3EFA
        add    esp, 8
        jmp    loc_4_43EA
loc_4_4400:
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [ebp + 0x10]
        movzx  ax, byte ptr [ecx + 1]
        mov    word ptr [edx + 0x10], ax
        jmp    loc_4_43E8
loc_4_4411:
        cmp    word ptr [0], 0               ; -> obj1:0x0468
        je     loc_4_4437
        mov    eax, dword ptr [0]            ; -> obj1:0x0474
        sub    eax, dword ptr [ebp + 0x10]
        cmp    eax, -0x24
        jne    loc_4_4437
        mov    ecx, dword ptr [ebp + 0x14]
        push   dword ptr [ecx]
        call   sub_3_1493
loc_4_4432:
        add    esp, 4
        jmp    loc_4_43E8
loc_4_4437:
        mov    eax, 0x8007
        jmp    loc_4_43EA
loc_4_443E:
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    dword ptr [ecx + 0x152a], 0
        jne    loc_4_4451
        mov    eax, 0x8007
        jmp    loc_4_43EA
loc_4_4451:
        mov    ecx, dword ptr [ebp + 0x14]
        cmp    dword ptr [ecx], 0
        je     loc_4_44AF
        mov    edx, dword ptr [ebp + 0x10]
        add    edx, 0x419
        cmp    byte ptr [edx], 0
        jne    loc_4_446D
        mov    ecx, dword ptr [ebp + 0x10]
        inc    dword ptr [ecx + 0x18]
loc_4_446D:
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 0x439
        cmp    byte ptr [eax], 0
        jne    loc_4_4480
        mov    ecx, dword ptr [ebp + 0x10]
        inc    dword ptr [ecx + 0x18]
loc_4_4480:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    byte ptr [edx], 1
        push   ecx
        mov    byte ptr [eax], 1
        mov    word ptr [ecx + 0x402], 0xffe
        mov    word ptr [ecx + 0x422], 0xffe
        call   sub_4_106D
        mov    ecx, dword ptr [ebp + 0x10]
        mov    word ptr [ecx + 6], 0x1e
        jmp    loc_4_43E8
loc_4_44AF:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    esi, 0xffff
        mov    word ptr [ecx + 6], 0x20
        push   esi
        push   0x7e
        push   ecx
        call   sub_1_1A14
loc_4_44C6:
        push   esi
        push   0x7f
        push   dword ptr [ebp + 0x10]
        call   sub_1_1A14
loc_4_44D1:
        jmp    loc_4_43E8
loc_4_44D6:
        mov    eax, dword ptr [0]            ; -> obj5:0x008C
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], eax
        jmp    loc_4_43E8
loc_4_44E5:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ax, word ptr [ecx]
        mov    edx, dword ptr [ebp + 0x10]
        and    eax, 0xfff8
        mov    ecx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 0x151e]
        mov    dword ptr [ecx + 4], eax
        movzx  edx, word ptr [edx + 0x1526]
        movzx  eax, word ptr [esi + 0x1528]
        shl    edx, 0x10
        or     edx, eax
        mov    dword ptr [ecx + 8], edx
        jmp    loc_4_43E8
loc_4_4521:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx], eax
        jmp    loc_4_43E8
loc_4_4531:
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx], eax
        jmp    loc_4_43E8
loc_4_4541:
        mov    esi, 0                        ; -> obj5:0x0AB8
        mov    edi, dword ptr [ebp + 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_4_43E8
loc_4_4550:
        push   dword ptr [ebp + 0x14]
        call   sub_4_3E2F
        add    esp, 4
        jmp    loc_4_43EA
loc_4_4560:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_3E50
        add    esp, 8
        jmp    loc_4_43EA
loc_4_4573:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_3F95
        add    esp, 8
        jmp    loc_4_43EA
loc_4_4586:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_4_4002
        add    esp, 8
        jmp    loc_4_43EA
loc_4_4599:
        cmp    word ptr [0], 0               ; -> obj1:0x0468
        je     loc_4_45BF
        mov    eax, dword ptr [0]            ; -> obj1:0x0474
        sub    eax, dword ptr [ebp + 0x10]
        cmp    eax, -0x24
        jne    loc_4_45BF
        mov    eax, dword ptr [0]            ; -> obj1:0x0464
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], eax
        jmp    loc_4_43E8
loc_4_45BF:
        mov    eax, 0x8007
        jmp    loc_4_43EA
loc_4_45C9:
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    dword ptr [ecx + 0x152a], 0
        je     loc_4_45EF
        mov    ax, word ptr [ecx + 6]
        mov    ecx, dword ptr [ebp + 0x14]
        sub    ax, 0x1e
        cmp    ax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ecx], eax
        jmp    loc_4_43E8
loc_4_45EF:
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], 0
        jmp    loc_4_43E8
loc_4_45FD:
        push   dword ptr [ebp + 0x10]
        call   sub_4_4220
        add    esp, 4
        jmp    loc_4_43EA
loc_4_460D:
        push   dword ptr [ebp + 0x10]
        call   sub_4_423F
        add    esp, 4
        jmp    loc_4_43EA
        ; ---- dispatch table (44 entries) ----
        dd  loc_4_42CD
        dd  loc_4_42DD
        dd  loc_4_42ED
        dd  loc_4_4300
        dd  loc_4_4313
        dd  loc_4_4329
        dd  loc_4_433C
        dd  loc_4_4353
        dd  loc_4_436A
        dd  loc_4_4389
        dd  loc_4_4399
        dd  loc_4_43A9
        dd  loc_4_43B9
        dd  loc_4_42C3
        dd  loc_4_42C3
        dd  loc_4_43C3
        dd  loc_4_43D5
        dd  loc_4_42C3
        dd  loc_4_42C3
        dd  loc_4_42C3
        dd  loc_4_43F0
        dd  loc_4_4400
        dd  loc_4_43E8
        dd  loc_4_4411
        dd  loc_4_443E
        dd  loc_4_42C3
        dd  loc_4_42C3
        dd  loc_4_42C3
        dd  loc_4_42C3
        dd  loc_4_42C3
        dd  loc_4_44D6
        dd  loc_4_44E5
        dd  loc_4_4521
        dd  loc_4_4531
        dd  loc_4_4541
        dd  loc_4_4550
        dd  loc_4_4560
        dd  loc_4_4573
        dd  loc_4_4586
        dd  loc_4_4599
        dd  loc_4_45C9
        dd  loc_4_42C3
        dd  loc_4_45FD
        dd  loc_4_460D
sub_4_46CD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [ebp - 4], ebx
        lea    edi, [esi + 0x42]
loc_4_46E4:
        cmp    word ptr [edi], 0x2000
        jae    loc_4_4718
        inc    bx
        push   0x80
        mov    eax, dword ptr [ebp - 4]
        or     eax, 0x2a0
        push   eax
        push   esi
        call   sub_1_19AA
loc_4_4701:
        mov    word ptr [edi], 0xffff
        cmp    byte ptr [edi + 0x17], 0
        je     loc_4_4718
        mov    eax, dword ptr [esi + 0x18]
        dec    eax
        mov    dword ptr [esi + 0x18], eax
        cmp    eax, 2
        jbe    loc_4_4724
loc_4_4718:
        add    edi, 0x20
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0x1e
        jb     loc_4_46E4
loc_4_4724:
        test   bx, bx
        jne    loc_4_4733
        mov    eax, 7
        jmp    loc_4_4D55
loc_4_4733:
        xor    edi, edi
        lea    ebx, [esi + 0x42]
loc_4_4738:
        cmp    word ptr [ebx], 0xffff
        jne    loc_4_47AF
        push   0
        mov    eax, edi
        or     al, 0x60
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_474C:
        push   0
        mov    eax, edi
        or     al, 0x40
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4759:
        push   0
        mov    eax, edi
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4766:
        push   0
        mov    eax, edi
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4773:
        push   0x40000000
        mov    eax, edi
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4783:
        push   0x40000000
        push   edi
        push   esi
        call   sub_1_1A14
loc_4_478F:
        test   edi, 1
        je     loc_4_479E
        push   0x7000000
        jmp    loc_4_47A3
loc_4_479E:
        push   0x5000000
loc_4_47A3:
        mov    eax, edi
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_47AF:
        add    ebx, 0x20
        inc    edi
        cmp    edi, 0x1e
        jb     loc_4_4738
        mov    dword ptr [ebp - 0x18], 1
        mov    edi, 0x200000
        mov    ebx, 0x33a
        push   edi
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_47D9:
        push   0
        push   ebx
        push   esi
        call   sub_1_19AA
loc_4_47E2:
        push   edi
        push   0x235
        push   esi
        call   sub_1_1A14
loc_4_47EE:
        push   ebx
        push   esi
        call   sub_1_19DF
loc_4_47F5:
        push   ebx
        push   esi
        call   sub_1_19DF
loc_4_47FC:
        test   ax, ax
        jne    loc_4_4CE2
        push   edi
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_4811:
        push   0xaaaa
        push   ebx
        push   esi
        call   sub_1_19AA
loc_4_481D:
        push   edi
        push   0x235
        push   esi
        call   sub_1_1A14
loc_4_4829:
        push   ebx
        push   esi
        call   sub_1_19DF
loc_4_4830:
        push   ebx
        push   esi
        call   sub_1_19DF
loc_4_4837:
        cmp    ax, 0xaaaa
        jne    loc_4_4CE2
        push   edi
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_484D:
        push   0x5555
        push   ebx
        push   esi
        call   sub_1_19AA
loc_4_4859:
        push   edi
        push   0x235
        push   esi
        call   sub_1_1A14
loc_4_4865:
        push   ebx
        push   esi
        call   sub_1_19DF
loc_4_486C:
        push   ebx
        push   esi
        call   sub_1_19DF
loc_4_4873:
        cmp    ax, 0x5555
        jne    loc_4_4CE2
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 0x80000
        ja     loc_4_4892
        je     loc_4_48D1
        cmp    eax, 0x40000
        je     loc_4_48C8
        jmp    loc_4_48BC
loc_4_4892:
        cmp    eax, 0x100000
        je     loc_4_48DA
        cmp    eax, 0x200000
        je     loc_4_48E3
        cmp    eax, 0x400000
        je     loc_4_48EC
        cmp    eax, 0x800000
        je     loc_4_48F5
        cmp    eax, 0xdfffe0
        je     loc_4_48F5
        cmp    eax, 0xe00000
        je     loc_4_48F5
loc_4_48BC:
        mov    dword ptr [ebp - 0x18], 5
        jmp    loc_4_4CE2
loc_4_48C8:
        mov    dword ptr [ebp - 8], 9
        jmp    loc_4_48FC
loc_4_48D1:
        mov    dword ptr [ebp - 8], 0x13
        jmp    loc_4_48FC
loc_4_48DA:
        mov    dword ptr [ebp - 8], 0x14
        jmp    loc_4_48FC
loc_4_48E3:
        mov    dword ptr [ebp - 8], 0x15
        jmp    loc_4_48FC
loc_4_48EC:
        mov    dword ptr [ebp - 8], 0x16
        jmp    loc_4_48FC
loc_4_48F5:
        mov    dword ptr [ebp - 8], 0x17
loc_4_48FC:
        push   0x8000
        call   sub_4_0D74
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        test   eax, eax
        jne    loc_4_491C
        mov    dword ptr [ebp - 0x18], 6
        jmp    loc_4_4CE2
loc_4_491C:
        xor    edi, edi
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ebp - 2], di
        push   0x200000
        push   0x237
        mov    dword ptr [ebp - 0xc], ecx
        push   esi
        call   sub_1_1A14
loc_4_4938:
        push   edi
        push   0x33a
        push   esi
        call   sub_1_19AA
loc_4_4944:
        mov    dword ptr [ebp - 0x14], edi
        cmp    dword ptr [ebp - 8], edi
        je     loc_4_49D5
loc_4_4950:
        lea    edi, [edi*2 + 1]
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, edi
        cmp    dword ptr [ebp - 0x14], eax
        jae    loc_4_49C6
        sub    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0x18], eax
loc_4_4967:
        inc    word ptr [ebp - 2]
        lea    eax, [ebx + 0x200000]
        push   eax
        push   0x235
        push   esi
        call   sub_1_1A14
loc_4_497D:
        push   0x33a
        push   esi
        call   sub_1_19DF
loc_4_4988:
        push   0x33a
        push   esi
        call   sub_1_19DF
loc_4_4993:
        mov    ecx, dword ptr [ebp - 0xc]
        add    dword ptr [ebp - 0xc], 2
        mov    word ptr [ecx], ax
        lea    eax, [ebx + 0x200000]
        push   eax
        add    ebx, ebx
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_49B1:
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        push   0x33a
        push   esi
        call   sub_1_19AA
loc_4_49C1:
        dec    dword ptr [ebp - 0x18]
        jne    loc_4_4967
loc_4_49C6:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x14], eax
        jb     loc_4_4950
loc_4_49D5:
        xor    edi, edi
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [ebp - 2], di
        push   0x200000
        push   0x235
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x18], 2
        push   esi
        call   sub_1_1A14
loc_4_49F8:
        push   0x33a
        push   esi
        call   sub_1_19DF
loc_4_4A03:
        push   0x33a
        push   esi
        call   sub_1_19DF
loc_4_4A0E:
        test   ax, ax
        jne    loc_4_4CE2
        mov    dword ptr [ebp - 0x14], edi
        cmp    dword ptr [ebp - 8], edi
        je     loc_4_4AD7
loc_4_4A23:
        lea    edi, [edi*2 + 1]
        mov    eax, dword ptr [ebp - 0x14]
        mov    ebx, edi
        mov    dword ptr [ebp - 0x1c], eax
        cmp    eax, dword ptr [ebp - 8]
        jae    loc_4_4AC8
loc_4_4A3B:
        mov    dword ptr [ebp - 0x20], 0x80
loc_4_4A42:
        push   0x234
        push   esi
        call   sub_1_1A3D
loc_4_4A4D:
        test   eax, 0x80000000
        je     loc_4_4A59
        dec    dword ptr [ebp - 0x20]
        jne    loc_4_4A42
loc_4_4A59:
        inc    word ptr [ebp - 2]
        lea    eax, [ebx + 0x200000]
        push   eax
        push   0x235
        push   esi
        call   sub_1_1A14
loc_4_4A6F:
        push   0x33a
        push   esi
        call   sub_1_19DF
loc_4_4A7A:
        push   0x33a
        push   esi
        call   sub_1_19DF
loc_4_4A85:
        cmp    word ptr [ebp - 2], ax
        jne    loc_4_4CE2
        lea    eax, [ebx + 0x200000]
        push   eax
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_4AA1:
        mov    ecx, dword ptr [ebp - 0xc]
        add    ebx, ebx
        add    dword ptr [ebp - 0xc], 2
        movzx  eax, word ptr [ecx]
        push   eax
        push   0x33a
        push   esi
        call   sub_1_19AA
loc_4_4AB9:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0x1c]
        cmp    dword ptr [ebp - 0x1c], eax
        jb     loc_4_4A3B
loc_4_4AC8:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x14], eax
        jb     loc_4_4A23
loc_4_4AD7:
        mov    ebx, dword ptr [ebp - 0x10]
        mov    edi, 0x200000
        mov    dword ptr [ebp - 0x18], 4
        push   edi
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_4AF2:
        push   edi
        push   0x235
        push   esi
        mov    edi, 0x33a
        call   sub_1_1A14
loc_4_4B03:
        push   0x33a
        push   esi
        call   sub_1_19DF
loc_4_4B0E:
        mov    dword ptr [ebp - 4], 0x2000
loc_4_4B15:
        push   edi
        push   esi
        call   sub_1_19DF
loc_4_4B1C:
        mov    word ptr [ebx], ax
        push   0
        add    ebx, 2
        push   edi
        push   esi
        call   sub_1_19AA
loc_4_4B2B:
        dec    dword ptr [ebp - 4]
        jne    loc_4_4B15
        mov    ebx, 0x200000
        mov    edi, 0x33a
loc_4_4B3A:
        push   ebx
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_4B46:
        push   0xffff
        push   edi
        push   esi
        call   sub_1_19AA
loc_4_4B52:
        push   ebx
        push   0x235
        push   esi
        call   sub_1_1A14
loc_4_4B5E:
        push   edi
        push   esi
        call   sub_1_19DF
loc_4_4B65:
        push   edi
        push   esi
        call   sub_1_19DF
loc_4_4B6C:
        cmp    ax, 0xffff
        jne    loc_4_4CE2
        push   ebx
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_4B82:
        push   0
        push   edi
        push   esi
        call   sub_1_19AA
loc_4_4B8B:
        push   ebx
        push   0x235
        push   esi
        call   sub_1_1A14
loc_4_4B97:
        push   edi
        push   esi
        call   sub_1_19DF
loc_4_4B9E:
        push   edi
        push   esi
        call   sub_1_19DF
loc_4_4BA5:
        test   ax, ax
        jne    loc_4_4CE2
        inc    ebx
        cmp    ebx, 0x202000
        jb     loc_4_4B3A
        mov    ebx, dword ptr [ebp - 0x10]
        push   0x200000
        push   0x237
        mov    edi, 0x2000
        push   esi
        call   sub_1_1A14
loc_4_4BCF:
        movzx  ecx, word ptr [ebx]
        mov    eax, ebx
        push   ecx
        add    ebx, 2
        push   0x33a
        push   esi
        call   sub_1_19AA
loc_4_4BE3:
        dec    edi
        jne    loc_4_4BCF
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edi, 0x200000
        push   edi
        mov    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [ebp - 0x18], 3
        mov    word ptr [ebp - 2], 0
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_4C0A:
        push   edi
        push   0x235
        push   esi
        call   sub_1_1A14
loc_4_4C16:
        push   0x33a
        push   esi
        call   sub_1_19DF
loc_4_4C21:
        mov    eax, dword ptr [esi + 8]
        add    eax, edi
        cmp    eax, edi
        jbe    loc_4_4CDB
        mov    ebx, 0x33a
loc_4_4C33:
        push   ebx
        push   esi
        call   sub_1_19DF
loc_4_4C3A:
        mov    ecx, dword ptr [ebp - 0xc]
        push   0x5a5a
        add    dword ptr [ebp - 0xc], 2
        mov    word ptr [ecx], ax
        push   ebx
        push   esi
        call   sub_1_19AA
loc_4_4C50:
        inc    word ptr [ebp - 2]
        cmp    word ptr [ebp - 2], 0x4000
        jne    loc_4_4CCA
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        lea    eax, [edi - 0x3fff]
        push   eax
        push   0x237
        push   esi
        call   sub_1_1A14
loc_4_4C74:
        lea    eax, [edi - 0x3fff]
        push   eax
        push   0x235
        push   esi
        call   sub_1_1A14
loc_4_4C86:
        push   ebx
        push   esi
        call   sub_1_19DF
loc_4_4C8D:
        mov    word ptr [ebp - 2], 0
loc_4_4C93:
        push   ebx
        push   esi
        call   sub_1_19DF
loc_4_4C9A:
        cmp    ax, 0x5a5a
        jne    loc_4_4CE2
        mov    eax, dword ptr [ebp - 0xc]
        add    dword ptr [ebp - 0xc], 2
        movzx  ecx, word ptr [eax]
        push   ecx
        push   ebx
        push   esi
        call   sub_1_19AA
loc_4_4CB2:
        inc    word ptr [ebp - 2]
        cmp    word ptr [ebp - 2], 0x4000
        jb     loc_4_4C93
        mov    word ptr [ebp - 2], 0
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], ecx
loc_4_4CCA:
        inc    edi
        mov    eax, dword ptr [esi + 8]
        add    eax, 0x200000
        cmp    eax, edi
        ja     loc_4_4C33
loc_4_4CDB:
        mov    dword ptr [ebp - 0x18], 0
loc_4_4CE2:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_4_4CF3
        push   dword ptr [ebp - 0x10]
        call   sub_4_0D91
        add    esp, 4
loc_4_4CF3:
        xor    edi, edi
        lea    ebx, [esi + 0x42]
loc_4_4CF8:
        cmp    word ptr [ebx], 0xffff
        jne    loc_4_4D49
        mov    word ptr [ebx], 0xffe
        push   0
        mov    byte ptr [ebx + 0x17], 1
        mov    eax, edi
        inc    dword ptr [esi + 0x18]
        or     ah, 2
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4D19:
        push   0x1d8
        mov    eax, edi
        or     al, 0xc0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4D29:
        push   0x1e0
        mov    eax, edi
        or     al, 0xe0
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4D39:
        push   0x48000000
        mov    eax, edi
        or     al, 0x20
        push   eax
        push   esi
        call   sub_1_1A14
loc_4_4D49:
        add    ebx, 0x20
        inc    edi
        cmp    edi, 0x1e
        jb     loc_4_4CF8
        mov    eax, dword ptr [ebp - 0x18]
loc_4_4D55:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_4D5C:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_4_4D64:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_4_4D6C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_4_4D74:
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
sub_4_4D88:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_4_4DA6
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_4_4DA6:
        ret    0x10
        db  0CCh, 0CCh, 0CCh
sub_4_4DAC:
        VMMCall  VMM._RegCreateDynKey            ; cd2076810100
        int3   
        int3   
sub_4_4DB4:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_4_4DBC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_4_4DC4:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_4_4DCC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        int3   
        int3   
sub_4_4DD4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Sibling  ; cd2005803300
        int3   
        int3   
sub_4_4DDC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_4_4DE4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004803300
        int3   
        int3   
sub_4_4DEC:
        VxDCall  CONFIGMG._CONFIGMG_Find_Range   ; cd2056803300
        int3   
        int3   
sub_4_4DF4:
        VxDCall  CONFIGMG._CONFIGMG_Query_Arbitrator_Free_Data ; cd2013803300
        int3   
        int3   
sub_4_4DFC:
        VxDCall  CONFIGMG._CONFIGMG_Add_Res_Des  ; cd201c803300
        int3   
        int3   
sub_4_4E04:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Status ; cd2036803300
        int3   
        int3   
sub_4_4E0C:
        VxDCall  SHELL._SHELL_QueryAppyTimeAvailable ; cd200d801700
        int3   
        int3   
sub_4_4E14:
        VxDCall  CONFIGMG._CONFIGMG_Free_Log_Conf ; cd2019803300
        int3   
        int3   
sub_4_4E1C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        int3   
        int3   
sub_4_4E24:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_4_4E2C:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        int3   
        int3   
sub_4_4E3C:
        VxDCall  CONFIGMG._CONFIGMG_Add_Empty_Log_Conf ; cd2018803300
        int3   
        int3   
sub_4_4E44:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0xcf0  RW-
        dd  loc_4_0073               ; fixup
        dd  loc_4_00C8               ; fixup
        dd  loc_4_00D6               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_0138               ; fixup
        dd  loc_4_014F               ; fixup
        dd  loc_4_015F               ; fixup
        dd  loc_4_016F               ; fixup
        dd  loc_4_0174               ; fixup
        dd  loc_4_0219               ; fixup
        dd  loc_4_0221               ; fixup
        dd  loc_4_0279               ; fixup
        dd  loc_4_0369               ; fixup
        dd  loc_4_03AA               ; fixup
        dd  loc_4_03F2               ; fixup
        dd  loc_4_057E               ; fixup
        dd  loc_4_05BB               ; fixup
        dd  loc_4_0610               ; fixup
        dd  loc_4_061E               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_4_062C               ; fixup
        dd  loc_4_0643               ; fixup
        dd  loc_4_0653               ; fixup
        dd  loc_4_0663               ; fixup
        dd  loc_4_0716               ; fixup
        dd  loc_4_071B               ; fixup
        dd  loc_4_0720               ; fixup
        dd  loc_4_078F               ; fixup
        dd  loc_4_0911               ; fixup
        db  053h, 042h, 041h, 057h, 045h, 02Eh, 056h, 058h, 044h, 000h, 000h, 000h, 073h, 062h, 031h, 036h
        db  02Eh, 076h, 078h, 064h, 000h, 000h, 000h, 000h, 01Eh, 000h, 004h, 000h, 052h, 041h, 04Dh, 054h
        db  065h, 073h, 074h, 000h, 052h, 041h, 04Dh, 046h, 072h, 065h, 065h, 000h, 052h, 041h, 04Dh, 053h
        db  069h, 07Ah, 065h, 000h, 043h, 051h, 04Dh, 054h, 068h, 072h, 075h, 000h, 042h, 061h, 073h, 065h
        db  000h, 000h, 000h, 000h, 04Ch, 06Fh, 067h, 044h, 065h, 076h, 069h, 063h, 065h, 000h, 000h, 000h
        db  043h, 053h, 04Eh, 000h, 042h, 075h, 073h, 054h, 079h, 070h, 065h, 000h, 042h, 04Ch, 041h, 053h
        db  054h, 045h, 052h, 000h, 030h, 031h, 032h, 033h, 034h, 035h, 036h, 037h, 038h, 039h, 041h, 042h
        db  043h, 044h, 045h, 046h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 06Ah, 0B5h, 0DAh, 0EDh
        db  0F6h, 0FBh, 07Dh, 0BEh, 0DFh, 06Fh, 037h, 01Bh, 00Dh, 086h, 0C3h, 061h, 0B0h, 058h, 02Ch, 016h
        db  08Bh, 045h, 0A2h, 0D1h, 0E8h, 074h, 03Ah, 09Dh, 0CEh, 0E7h, 073h, 039h
        dd  loc_3_01DE               ; fixup
        dd  loc_3_01DE               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj5:0x00C8              ; fixup
        db  004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  obj5:0x00C4              ; fixup
        db  004h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  obj5:0x00B8              ; fixup
        db  004h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  obj5:0x00B0              ; fixup
        db  004h, 000h, 000h, 000h, 003h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  obj5:0x00A8              ; fixup
        db  004h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  obj5:0x00A0              ; fixup
        db  004h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  obj5:0x0098              ; fixup
        db  004h, 000h, 000h, 000h, 006h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  obj5:0x0090              ; fixup
        db  004h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 003h, 000h, 000h, 000h, 043h, 072h, 065h, 061h
        db  074h, 069h, 076h, 065h, 020h, 054h, 065h, 063h, 068h, 06Eh, 06Fh, 06Ch, 06Fh, 067h, 079h, 05Ch
        db  030h, 030h, 030h, 030h, 030h, 030h, 030h, 030h, 000h, 000h, 000h, 000h, 044h, 072h, 069h, 076h
        db  065h, 072h, 000h, 000h, 053h, 065h, 074h, 074h, 069h, 06Eh, 067h, 073h, 000h, 000h, 000h, 000h
        db  050h, 06Eh, 050h, 046h, 06Fh, 072h, 063h, 065h, 042h, 061h, 073h, 065h, 049h, 04Fh, 000h, 000h
        db  000h, 000h, 000h, 000h, 042h, 04Ch, 041h, 053h, 054h, 045h, 052h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 00Ch, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 002h, 000h, 000h, 003h, 0FFh, 003h
        db  000h, 000h, 0FFh, 000h, 056h, 049h, 052h, 054h, 055h, 041h, 04Ch, 05Ch, 053h, 042h, 041h, 057h
        db  045h, 02Dh, 04Dh, 050h, 055h, 045h, 06Dh, 075h, 06Ch, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FFh, 003h, 030h, 000h, 0FFh, 007h, 030h, 001h, 0FFh, 00Bh, 030h, 002h
        db  0FFh, 00Fh, 030h, 003h, 0FFh, 013h, 030h, 004h, 0FFh, 017h, 030h, 005h, 0FFh, 01Bh, 030h, 006h
        db  0FFh, 01Fh, 030h, 007h, 0FFh, 023h, 030h, 008h, 0FFh, 027h, 030h, 009h, 0FFh, 02Bh, 030h, 00Ah
        db  0FFh, 02Fh, 030h, 00Bh, 0FFh, 033h, 030h, 00Ch, 0FFh, 037h, 030h, 00Dh, 0FFh, 03Bh, 030h, 00Eh
        db  0FFh, 03Fh, 030h, 00Fh, 0FFh, 043h, 030h, 000h, 0FFh, 047h, 030h, 001h, 0FFh, 04Bh, 030h, 002h
        db  0FFh, 04Fh, 030h, 003h, 0FFh, 053h, 030h, 004h, 0FFh, 057h, 030h, 005h, 0FFh, 05Bh, 030h, 006h
        db  0FFh, 05Fh, 030h, 007h, 0FFh, 063h, 030h, 008h, 0FFh, 067h, 030h, 009h, 0FFh, 06Bh, 030h, 00Ah
        db  0FFh, 06Fh, 030h, 00Bh, 0FFh, 073h, 030h, 00Ch, 0FFh, 077h, 030h, 00Dh, 0FFh, 07Bh, 030h, 00Eh
        db  0FFh, 07Fh, 030h, 00Fh, 0FFh, 083h, 030h, 000h, 0FFh, 087h, 030h, 001h, 0FFh, 08Bh, 030h, 002h
        db  0FFh, 08Fh, 030h, 003h, 0FFh, 093h, 030h, 004h, 0FFh, 097h, 030h, 005h, 0FFh, 09Bh, 030h, 006h
        db  0FFh, 09Fh, 030h, 007h, 0FFh, 0A3h, 030h, 008h, 0FFh, 0A7h, 030h, 009h, 0FFh, 0ABh, 030h, 00Ah
        db  0FFh, 0AFh, 030h, 00Bh, 0FFh, 0B3h, 030h, 00Ch, 0FFh, 0B7h, 030h, 00Dh, 0FFh, 0BBh, 030h, 00Eh
        db  0FFh, 0BFh, 030h, 00Fh, 0FFh, 0C3h, 030h, 000h, 0FFh, 0C7h, 030h, 001h, 0FFh, 0CBh, 030h, 002h
        db  0FFh, 0CFh, 030h, 003h, 0FFh, 0D3h, 030h, 004h, 0FFh, 0D7h, 030h, 005h, 0FFh, 0DBh, 030h, 006h
        db  0FFh, 0DFh, 030h, 007h, 0FFh, 0E3h, 030h, 008h, 0FFh, 0E7h, 030h, 009h, 0FFh, 0EBh, 030h, 00Ah
        db  0FFh, 0EFh, 030h, 00Bh, 0FFh, 0F3h, 030h, 00Ch, 0FFh, 0F7h, 030h, 00Dh, 0FFh, 0FBh, 030h, 00Eh
        db  0FFh, 0FFh, 030h, 00Fh, 010h, 00Ch, 070h, 084h, 0FEh, 014h, 088h, 0B4h, 07Fh, 016h, 070h, 0A4h
        db  0E7h, 018h, 0B5h, 084h, 06Eh, 01Bh, 02Ah, 084h, 01Dh, 01Fh, 02Ah, 085h, 0A3h, 00Dh, 07Ch, 00Fh
        db  07Eh, 016h, 054h, 072h, 000h, 000h, 02Ah, 084h, 001h, 000h, 02Ah, 085h, 0E6h, 018h, 0AAh, 00Bh
        db  06Dh, 01Bh, 034h, 072h, 09Fh, 022h, 029h, 084h, 046h, 027h, 029h, 085h, 01Ch, 01Fh, 0E7h, 006h
        db  09Eh, 022h, 024h, 072h, 0A4h, 00Dh, 029h, 084h, 029h, 02Ch, 029h, 085h, 045h, 027h, 0F6h, 007h
        db  028h, 02Ch, 054h, 072h, 03Bh, 038h, 028h, 084h, 00Fh, 032h, 028h, 085h, 00Eh, 032h, 002h, 00Fh
        db  041h, 013h, 064h, 072h, 0B6h, 03Eh, 028h, 084h, 0B9h, 03Eh, 028h, 085h, 03Ah, 038h, 0A9h, 00Fh
        db  0B5h, 03Eh, 094h, 072h, 0B7h, 03Eh, 074h, 084h, 0BAh, 03Eh, 075h, 085h, 0B8h, 03Eh, 0C3h, 044h
        db  0BBh, 03Eh, 0C3h, 045h, 000h, 000h, 004h, 0A4h, 001h, 000h, 004h, 0A5h, 01Fh, 014h, 071h, 006h
        db  0FDh, 014h, 087h, 002h, 0BCh, 03Eh, 010h, 0E6h, 0C8h, 03Eh, 07Bh, 00Ch, 01Ah, 003h, 0E6h, 007h
        db  0C8h, 03Eh, 0F7h, 086h, 0C0h, 03Eh, 01Eh, 082h, 0BEh, 03Eh, 008h, 0D2h, 0BDh, 03Eh, 01Fh, 002h
        db  0CAh, 03Eh, 086h, 003h, 0C1h, 03Eh, 003h, 00Ch, 0C9h, 03Eh, 01Eh, 003h, 0CAh, 03Eh, 04Ch, 08Ch
        db  0BFh, 03Eh, 055h, 00Ch, 0C9h, 03Eh, 008h, 0C2h, 0C4h, 03Eh, 084h, 0BCh, 0C8h, 03Eh, 0ADh, 00Eh
        db  0C8h, 03Eh, 008h, 0D3h, 0C2h, 03Eh, 07Eh, 08Fh, 0CBh, 03Eh, 01Eh, 002h, 0CBh, 03Eh, 008h, 0D2h
        db  0C5h, 03Eh, 01Fh, 003h, 0C6h, 03Eh, 008h, 0C3h, 0C3h, 03Eh, 0FFh, 032h, 0C9h, 03Eh, 065h, 002h
        db  0C9h, 03Eh, 01Eh, 083h, 042h, 013h, 008h, 0D3h, 0C7h, 03Eh, 0FFh, 033h, 000h, 000h, 065h, 083h
        db  020h, 014h, 070h, 095h, 06Ah, 0D2h, 06Ah, 0D3h, 000h, 000h, 05Bh, 0D2h, 05Bh, 0D3h, 000h, 000h
        db  04Ch, 0D2h, 04Ch, 0D3h, 000h, 000h, 03Dh, 0D2h, 03Dh, 0D3h, 000h, 000h, 01Fh, 0D2h, 01Fh, 0D3h
        db  000h, 000h, 008h, 0C2h, 008h, 0C3h, 001h, 000h, 019h, 0C2h, 019h, 0C3h, 001h, 000h, 02Ah, 0C2h
        db  02Ah, 0C3h, 001h, 000h, 04Ch, 0C2h, 04Ch, 0C3h, 001h, 000h, 06Eh, 0C2h, 06Eh, 0C3h, 001h, 000h
        db  048h, 0C2h, 084h, 0C3h, 002h, 000h, 06Ah, 0C2h, 06Ah, 0C3h, 002h, 000h, 01Eh, 082h, 06Ah, 0C2h
        db  01Eh, 003h, 06Ah, 0C3h, 01Eh, 002h, 008h, 0D2h, 01Eh, 083h, 008h, 0D3h, 001h, 000h, 01Eh, 082h
        db  05Bh, 0C2h, 01Eh, 003h, 05Bh, 0C3h, 01Eh, 002h, 008h, 0D2h, 01Eh, 083h, 008h, 0D3h, 001h, 000h
        db  01Eh, 082h, 04Ch, 0C2h, 01Eh, 003h, 04Ch, 0C3h, 01Eh, 002h, 008h, 0D2h, 01Eh, 083h, 008h, 0D3h
        db  001h, 000h, 01Eh, 082h, 03Dh, 0C2h, 01Eh, 003h, 03Dh, 0C3h, 01Eh, 002h, 008h, 0D2h, 01Eh, 083h
        db  008h, 0D3h, 001h, 000h, 01Eh, 082h, 01Fh, 0C2h, 01Eh, 003h, 01Fh, 0C3h, 01Eh, 002h, 008h, 0D2h
        db  01Eh, 083h, 008h, 0D3h, 001h, 000h, 01Eh, 082h, 008h, 0D2h, 01Eh, 003h, 008h, 0D3h, 01Eh, 002h
        db  008h, 0D2h, 01Eh, 083h, 008h, 0D3h, 002h, 000h, 01Eh, 082h, 008h, 0D2h, 01Eh, 003h, 008h, 0D3h
        db  01Dh, 002h, 019h, 0D2h, 01Dh, 083h, 019h, 0D3h, 002h, 000h, 01Eh, 082h, 008h, 0D2h, 01Eh, 003h
        db  008h, 0D3h, 01Ch, 002h, 02Ah, 0D2h, 01Ch, 083h, 02Ah, 0D3h, 002h, 000h, 01Eh, 082h, 008h, 0D2h
        db  01Eh, 003h, 008h, 0D3h, 01Ah, 002h, 04Ch, 0D2h, 01Ah, 083h, 04Ch, 0D3h, 002h, 000h, 01Eh, 082h
        db  008h, 0D2h, 01Eh, 003h, 008h, 0D3h, 019h, 002h, 06Eh, 0D2h, 019h, 083h, 06Eh, 0D3h, 002h, 000h
        db  01Dh, 082h, 019h, 0D2h, 01Dh, 003h, 019h, 0D3h, 019h, 002h, 06Eh, 0D2h, 019h, 083h, 06Eh, 0D3h
        db  002h, 000h, 01Ch, 082h, 02Ah, 0D2h, 01Ch, 003h, 02Ah, 0D3h, 019h, 002h, 06Eh, 0D2h, 019h, 083h
        db  06Eh, 0D3h, 002h, 000h, 000h, 0E6h, 0F6h, 003h, 02Ch, 0BCh, 000h, 000h, 000h, 000h, 06Dh, 000h
        db  000h, 000h, 008h, 0E6h, 01Ah, 003h, 06Eh, 0BCh, 000h, 000h, 000h, 000h, 07Ch, 001h, 000h, 000h
        db  010h, 0E6h, 01Ah, 003h, 084h, 0BCh, 000h, 000h, 000h, 000h, 083h, 000h, 000h, 000h, 020h, 0E6h
        db  069h, 002h, 06Eh, 0BCh, 000h, 000h, 000h, 000h, 07Ch, 001h, 000h, 000h, 080h, 0E6h, 0D3h, 004h
        db  0A6h, 0BCh, 000h, 000h, 000h, 000h, 05Bh, 000h, 000h, 000h, 0E0h, 0E6h, 04Eh, 004h, 037h, 0BCh
        db  000h, 000h, 000h, 000h, 026h, 000h, 000h, 000h, 000h, 0E6h, 006h, 00Bh, 000h, 0BCh, 000h, 0E0h
        db  006h, 000h, 083h, 000h, 000h, 000h, 0C0h, 0E6h, 006h, 00Bh, 000h, 0BCh, 000h, 0E0h, 006h, 000h
        db  083h, 000h, 000h, 000h, 003h, 005h, 07Fh, 007h, 034h, 036h, 00Fh, 017h, 01Fh, 027h, 02Fh, 037h
        db  03Dh, 03Fh, 041h, 043h, 009h, 00Bh, 011h, 013h, 019h, 01Bh, 021h, 023h, 029h, 02Bh, 031h, 033h
        db  000h, 000h, 000h, 000h, 088h, 0B4h, 050h, 0A4h, 050h, 095h, 0B5h, 084h, 03Ah, 038h, 0B5h, 03Eh
        db  0F4h, 072h, 0A4h, 072h, 054h, 072h, 004h, 072h, 004h, 072h, 004h, 072h, 016h, 044h, 016h, 045h
        db  090h, 0A4h, 090h, 0A5h, 02Ah, 084h, 02Ah, 085h, 02Ah, 084h, 02Ah, 085h, 029h, 084h, 029h, 085h
        db  029h, 084h, 029h, 085h, 028h, 084h, 028h, 085h, 028h, 084h, 028h, 085h, 088h, 0B4h, 058h, 0A4h
        db  058h, 095h, 0B5h, 084h, 03Ah, 038h, 0B5h, 03Eh, 084h, 072h, 054h, 072h, 024h, 072h, 024h, 072h
        db  054h, 072h, 084h, 072h, 048h, 044h, 048h, 045h, 040h, 0A4h, 040h, 0A5h, 02Ah, 084h, 02Ah, 085h
        db  02Ah, 084h, 02Ah, 085h, 029h, 084h, 029h, 085h, 029h, 084h, 029h, 085h, 028h, 084h, 028h, 085h
        db  028h, 084h, 028h, 085h, 088h, 0B4h, 060h, 0A4h, 060h, 095h, 0B5h, 084h, 03Ah, 038h, 0B5h, 03Eh
        db  084h, 072h, 054h, 072h, 024h, 072h, 024h, 072h, 054h, 072h, 084h, 072h, 016h, 044h, 016h, 045h
        db  090h, 0A4h, 090h, 0A5h, 02Ch, 084h, 02Ch, 085h, 02Ch, 084h, 02Ch, 085h, 02Bh, 084h, 02Bh, 085h
        db  02Bh, 084h, 02Bh, 085h, 02Ah, 084h, 02Ah, 085h, 02Ah, 084h, 02Ah, 085h, 088h, 0B4h, 070h, 0A4h
        db  070h, 095h, 0B5h, 084h, 03Ah, 038h, 0B5h, 03Eh, 084h, 072h, 054h, 072h, 024h, 072h, 024h, 072h
        db  054h, 072h, 084h, 072h, 048h, 044h, 048h, 045h, 040h, 0A4h, 040h, 0A5h, 02Bh, 084h, 02Bh, 085h
        db  02Bh, 084h, 02Bh, 085h, 02Ah, 084h, 02Ah, 085h, 02Ah, 084h, 02Ah, 085h, 029h, 084h, 029h, 085h
        db  029h, 084h, 029h, 085h, 088h, 0B4h, 070h, 0A4h, 070h, 095h, 0B5h, 084h, 03Ah, 038h, 0B5h, 03Eh
        db  054h, 072h, 034h, 072h, 024h, 072h, 054h, 072h, 064h, 072h, 094h, 072h, 0C3h, 044h, 0C3h, 045h
        db  004h, 0A4h, 004h, 0A5h, 02Ah, 084h, 02Ah, 085h, 02Ah, 084h, 02Ah, 085h, 029h, 084h, 029h, 085h
        db  029h, 084h, 029h, 085h, 028h, 084h, 028h, 085h, 028h, 084h, 028h, 085h, 0FFh, 0B4h, 070h, 0A4h
        db  070h, 095h, 0B5h, 084h, 03Ah, 038h, 0B5h, 03Eh, 034h, 072h, 034h, 072h, 034h, 072h, 034h, 072h
        db  034h, 072h, 034h, 072h, 048h, 044h, 048h, 045h, 040h, 0A4h, 040h, 0A5h, 02Ah, 084h, 02Ah, 085h
        db  02Ah, 084h, 02Ah, 085h, 029h, 084h, 029h, 085h, 029h, 084h, 029h, 085h, 028h, 084h, 028h, 085h
        db  028h, 084h, 028h, 085h, 0FFh, 0B4h, 070h, 0A4h, 000h, 095h, 0B5h, 084h, 03Ah, 033h, 0B5h, 039h
        db  004h, 072h, 004h, 072h, 004h, 072h, 004h, 072h, 004h, 072h, 0F4h, 072h, 000h, 044h, 000h, 045h
        db  0FFh, 0A4h, 0FFh, 0A5h, 020h, 084h, 020h, 085h, 020h, 084h, 020h, 085h, 020h, 084h, 020h, 085h
        db  020h, 084h, 020h, 085h, 020h, 084h, 020h, 085h, 020h, 084h, 020h, 085h, 0FFh, 0B4h, 090h, 0A4h
        db  090h, 095h, 074h, 084h, 03Ah, 033h, 0B5h, 039h, 004h, 072h, 004h, 072h, 004h, 072h, 004h, 072h
        db  004h, 072h, 0F4h, 072h, 000h, 044h, 000h, 045h, 0FFh, 0A4h, 0FFh, 0A5h, 020h, 084h, 020h, 085h
        db  020h, 084h, 020h, 085h, 020h, 084h, 020h, 085h, 020h, 084h, 020h, 085h, 020h, 084h, 020h, 085h
        db  020h, 084h, 020h, 085h, 000h, 000h, 000h, 000h, 0FCh, 02Dh, 000h, 000h, 09Eh, 05Bh, 000h, 000h
        db  0E6h, 088h, 000h, 000h, 0D6h, 0B5h, 000h, 000h, 06Fh, 0E2h, 000h, 000h, 0B3h, 00Eh, 001h, 000h
        db  0A2h, 03Ah, 001h, 000h, 03Fh, 066h, 001h, 000h, 08Ah, 091h, 001h, 000h, 084h, 0BCh, 001h, 000h
        db  02Eh, 0E7h, 001h, 000h, 08Bh, 011h, 002h, 000h, 09Ah, 03Bh, 002h, 000h, 05Dh, 065h, 002h, 000h
        db  0D5h, 08Eh, 002h, 000h, 003h, 0B8h, 002h, 000h, 0E8h, 0E0h, 002h, 000h, 085h, 009h, 003h, 000h
        db  0DBh, 031h, 003h, 000h, 0EBh, 059h, 003h, 000h, 0B6h, 081h, 003h, 000h, 03Dh, 0A9h, 003h, 000h
        db  081h, 0D0h, 003h, 000h, 082h, 0F7h, 003h, 000h, 042h, 01Eh, 004h, 000h, 0C1h, 044h, 004h, 000h
        db  001h, 06Bh, 004h, 000h, 001h, 091h, 004h, 000h, 0C4h, 0B6h, 004h, 000h, 049h, 0DCh, 004h, 000h
        db  091h, 001h, 005h, 000h, 09Eh, 026h, 005h, 000h, 06Fh, 04Bh, 005h, 000h, 006h, 070h, 005h, 000h
        db  063h, 094h, 005h, 000h, 088h, 0B8h, 005h, 000h, 074h, 0DCh, 005h, 000h, 029h, 000h, 006h, 000h
        db  0A7h, 023h, 006h, 000h, 0EEh, 046h, 006h, 000h, 000h, 06Ah, 006h, 000h, 0DDh, 08Ch, 006h, 000h
        db  086h, 0AFh, 006h, 000h, 0FAh, 0D1h, 006h, 000h, 03Ch, 0F4h, 006h, 000h, 04Bh, 016h, 007h, 000h
        db  029h, 038h, 007h, 000h, 0D4h, 059h, 007h, 000h, 04Fh, 07Bh, 007h, 000h, 09Ah, 09Ch, 007h, 000h
        db  0B5h, 0BDh, 007h, 000h, 0A1h, 0DEh, 007h, 000h, 05Eh, 0FFh, 007h, 000h, 0EDh, 01Fh, 008h, 000h
        db  04Eh, 040h, 008h, 000h, 082h, 060h, 008h, 000h, 089h, 080h, 008h, 000h, 064h, 0A0h, 008h, 000h
        db  014h, 0C0h, 008h, 000h, 098h, 0DFh, 008h, 000h, 0F1h, 0FEh, 008h, 000h, 020h, 01Eh, 009h, 000h
        db  026h, 03Dh, 009h, 000h, 001h, 05Ch, 009h, 000h, 0B4h, 07Ah, 009h, 000h, 03Eh, 099h, 009h, 000h
        db  09Fh, 0B7h, 009h, 000h, 0D9h, 0D5h, 009h, 000h, 0ECh, 0F3h, 009h, 000h, 0D8h, 011h, 00Ah, 000h
        db  09Dh, 02Fh, 00Ah, 000h, 03Ch, 04Dh, 00Ah, 000h, 0B5h, 06Ah, 00Ah, 000h, 008h, 088h, 00Ah, 000h
        db  037h, 0A5h, 00Ah, 000h, 041h, 0C2h, 00Ah, 000h, 026h, 0DFh, 00Ah, 000h, 0E7h, 0FBh, 00Ah, 000h
        db  085h, 018h, 00Bh, 000h, 000h, 035h, 00Bh, 000h, 057h, 051h, 00Bh, 000h, 08Ch, 06Dh, 00Bh, 000h
        db  09Fh, 089h, 00Bh, 000h, 08Fh, 0A5h, 00Bh, 000h, 05Eh, 0C1h, 00Bh, 000h, 00Ch, 0DDh, 00Bh, 000h
        db  099h, 0F8h, 00Bh, 000h, 004h, 014h, 00Ch, 000h, 050h, 02Fh, 00Ch, 000h, 07Bh, 04Ah, 00Ch, 000h
        db  087h, 065h, 00Ch, 000h, 073h, 080h, 00Ch, 000h, 03Fh, 09Bh, 00Ch, 000h, 0EDh, 0B5h, 00Ch, 000h
        db  07Ch, 0D0h, 00Ch, 000h, 0ECh, 0EAh, 00Ch, 000h, 03Fh, 005h, 00Dh, 000h, 073h, 01Fh, 00Dh, 000h
        db  08Ah, 039h, 00Dh, 000h, 084h, 053h, 00Dh, 000h, 060h, 06Dh, 00Dh, 000h, 020h, 087h, 00Dh, 000h
        db  0C3h, 0A0h, 00Dh, 000h, 04Ah, 0BAh, 00Dh, 000h, 0B4h, 0D3h, 00Dh, 000h, 003h, 0EDh, 00Dh, 000h
        db  036h, 006h, 00Eh, 000h, 04Eh, 01Fh, 00Eh, 000h, 04Ah, 038h, 00Eh, 000h, 02Ch, 051h, 00Eh, 000h
        db  0F3h, 069h, 00Eh, 000h, 09Fh, 082h, 00Eh, 000h, 031h, 09Bh, 00Eh, 000h, 0A9h, 0B3h, 00Eh, 000h
        db  008h, 0CCh, 00Eh, 000h, 04Ch, 0E4h, 00Eh, 000h, 078h, 0FCh, 00Eh, 000h, 08Ah, 014h, 00Fh, 000h
        db  083h, 02Ch, 00Fh, 000h, 063h, 044h, 00Fh, 000h, 02Ah, 05Ch, 00Fh, 000h, 0DAh, 073h, 00Fh, 000h
        db  071h, 08Bh, 00Fh, 000h, 0F0h, 0A2h, 00Fh, 000h, 057h, 0BAh, 00Fh, 000h, 0A7h, 0D1h, 00Fh, 000h
        db  0DFh, 0E8h, 00Fh, 000h, 05Ch, 05Ch, 05Bh, 05Ah, 05Ah, 059h, 058h, 058h, 057h, 056h, 056h, 055h
        db  055h, 054h, 053h, 053h, 052h, 052h, 051h, 051h, 050h, 050h, 04Fh, 04Fh, 04Eh, 04Dh, 04Dh, 04Dh
        db  04Ch, 04Ch, 04Bh, 04Bh, 04Ah, 04Ah, 049h, 049h, 048h, 048h, 047h, 047h, 047h, 046h, 046h, 045h
        db  045h, 045h, 044h, 044h, 043h, 043h, 043h, 042h, 042h, 042h, 041h, 041h, 041h, 040h, 040h, 040h
        db  03Fh, 03Fh, 03Fh, 03Eh, 03Eh, 03Eh, 03Dh, 03Dh, 03Dh, 03Ch, 03Ch, 03Ch, 03Bh, 03Bh, 03Bh, 03Bh
        db  03Ah, 03Ah, 03Ah, 039h, 039h, 039h, 039h, 038h, 038h, 038h, 038h, 037h, 037h, 037h, 037h, 036h
        db  036h, 036h, 036h, 035h, 035h, 035h, 035h, 034h, 034h, 034h, 034h, 034h, 033h, 033h, 033h, 033h
        db  032h, 032h, 032h, 032h, 032h, 031h, 031h, 031h, 031h, 031h, 030h, 030h, 030h, 030h, 030h, 02Fh
        db  02Fh, 02Fh, 02Fh, 02Fh, 052h, 065h, 076h, 065h, 072h, 062h, 020h, 026h, 020h, 043h, 068h, 06Fh
        db  072h, 075h, 073h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 045h, 04Dh, 055h, 038h
        db  030h, 030h, 030h, 000h, 052h, 06Fh, 06Fh, 06Dh, 020h, 031h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 052h, 06Fh, 06Fh, 06Dh, 020h, 032h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 052h, 06Fh, 06Fh, 06Dh
        db  020h, 033h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  048h, 061h, 06Ch, 06Ch, 020h, 031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 048h, 061h, 06Ch, 06Ch, 020h, 032h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 06Ch, 061h, 074h, 065h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 044h, 065h, 06Ch, 061h
        db  079h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  050h, 061h, 06Eh, 06Eh, 069h, 06Eh, 067h, 020h, 044h, 065h, 06Ch, 061h, 079h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 043h, 068h, 06Fh, 072h, 075h, 073h, 020h, 031h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 043h, 068h, 06Fh, 072h, 075h, 073h, 020h, 032h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 043h, 068h, 06Fh, 072h
        db  075h, 073h, 020h, 033h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  043h, 068h, 06Fh, 072h, 075h, 073h, 020h, 034h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 046h, 065h, 065h, 064h, 062h, 061h, 063h, 06Bh, 020h, 044h, 065h, 06Ch
        db  061h, 079h, 000h, 000h, 000h, 000h, 000h, 000h, 046h, 06Ch, 061h, 06Eh, 067h, 065h, 072h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 068h, 06Fh, 072h
        db  074h, 020h, 044h, 065h, 06Ch, 061h, 079h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  053h, 068h, 06Fh, 072h, 074h, 020h, 044h, 065h, 06Ch, 061h, 079h, 020h, 046h, 042h, 000h, 000h
        db  000h, 000h, 000h, 000h, 02Dh, 031h, 032h, 064h, 042h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Dh, 038h, 064h, 042h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Dh, 036h, 064h, 042h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  02Dh, 034h, 064h, 042h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 02Dh, 032h, 064h, 042h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 030h, 064h, 042h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Bh, 032h, 064h, 042h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  02Bh, 034h, 064h, 042h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 02Bh, 036h, 064h, 042h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Bh, 038h, 064h, 042h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Bh, 031h, 030h, 064h
        db  042h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  02Bh, 031h, 032h, 064h, 042h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LDATA ENDS

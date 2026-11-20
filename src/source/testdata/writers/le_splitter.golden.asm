; splitter.vxd -- assembled with NASM: nasm -f bin -o splitter.vxd this-file

; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one
; Microsoft's tools write; NASM itself writes the other
%assign _r_ah 4
%assign _r_al 0
%assign _r_ax 0
%assign _r_bh 7
%assign _r_bl 3
%assign _r_bp 5
%assign _r_bx 3
%assign _r_ch 5
%assign _r_cl 1
%assign _r_cx 1
%assign _r_dh 6
%assign _r_di 7
%assign _r_dl 2
%assign _r_dx 2
%assign _r_eax 0
%assign _r_ebp 5
%assign _r_ebx 3
%assign _r_ecx 1
%assign _r_edi 7
%assign _r_edx 2
%assign _r_esi 6
%assign _r_esp 4
%assign _r_si 6
%assign _r_sp 4
%assign _w_ah 0
%assign _w_al 0
%assign _w_ax 2
%assign _w_bh 0
%assign _w_bl 0
%assign _w_bp 2
%assign _w_bx 2
%assign _w_ch 0
%assign _w_cl 0
%assign _w_cx 2
%assign _w_dh 0
%assign _w_di 2
%assign _w_dl 0
%assign _w_dx 2
%assign _w_eax 4
%assign _w_ebp 4
%assign _w_ebx 4
%assign _w_ecx 4
%assign _w_edi 4
%assign _w_edx 4
%assign _w_esi 4
%assign _w_esp 4
%assign _w_si 2
%assign _w_sp 2
%macro _rr 3
  %if (_w_%2 == 2 && __?BITS?__ == 32) || (_w_%2 == 4 && __?BITS?__ == 16)
    db 0x66
  %endif
  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3
%endmacro
%define adc_ _rr 0x10,
%define add_ _rr 0x00,
%define and_ _rr 0x20,
%define cmp_ _rr 0x38,
%define mov_ _rr 0x88,
%define or_ _rr 0x08,
%define sbb_ _rr 0x18,
%define sub_ _rr 0x28,
%define xor_ _rr 0x30,
o1_file equ 0x200

section part0 start=0x0 vstart=0x0 align=1
            db   04Dh, 05Ah, 000h, 000h, 001h, 000h, 000h, 000h, 004h, 000h, 00Fh, 000h, 0FFh, 0FFh, 000h, 000h
            db   0B8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h
            db   00Eh, 01Fh, 0BAh, 00Eh, 000h, 0B4h, 009h, 0CDh, 021h, 0B8h, 001h, 04Ch, 0CDh
            db   '!This program cannot be run in DOS mode.'
            db   00Dh, 00Ah, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Ch, 045h, 000h, 000h, 000h
            db   000h, 000h, 000h, 002h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 003h, 000h, 001h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 010h, 000h, 000h, 0ECh, 005h, 000h, 000h, 08Ah, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C4h, 000h, 000h, 000h, 001h
            db   000h, 000h, 000h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 0E0h, 000h, 000h, 000h, 0ECh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 0F6h, 000h, 000h, 000h, 0FEh, 000h, 000h, 000h, 080h, 001h, 000h, 000h, 000h
            db   000h, 000h, 000h, 080h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 001h
            db   000h, 000h, 000h, 0ECh, 007h, 000h, 000h, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 024h, 008h, 000h, 000h, 000h, 002h, 000h, 000h, 098h, 004h, 000h, 004h, 004h
            db   006h, 000h, 000h, 000h, 000h, 000h, 000h, 045h, 020h, 000h, 000h, 001h, 000h, 000h, 000h, 001h
            db   000h, 000h, 000h, 04Ch, 043h, 04Fh, 044h, 000h, 000h, 001h, 000h, 008h
            db   'Splitter'
            db   000h, 000h, 000h, 001h, 003h, 001h, 000h, 003h, 09Ch, 005h, 000h, 000h, 000h

section fixpage start=0x176 vstart=0 align=1
    dd fr_p0-fr_p0
    dd fr_p1-fr_p0

section fixrec start=0x17e vstart=0 align=1
fr_p0:
    db 0x07,0x00
    dw o1_D29b-0x0
    db 0x01
    dw o1_start+0x600
    db 0x07,0x00
    dw o1_D58c-0x0
    db 0x01
    dw o1_L142
    db 0x07,0x00
    dw o1_L5cc-0x0
    db 0x01
    dw o1_D588
    db 0x07,0x00
    dw o1_D594-0x0
    db 0x01
    dw o1_L258
    db 0x07,0x00
    dw o1_S5b4-0x0
    db 0x01
    dw o1_L99
    db 0x27,0x00
    db 3
    db 0x01
    dw o1_L29f
    dw o1_D598-0x0
    dw o1_S268-0x0
    dw o1_S28b-0x0
    db 0x07,0x00
    dw o1_D42f-0x0
    db 0x01
    dw o1_L4e6
    db 0x27,0x00
    db 2
    db 0x01
    dw o1_D5ec
    dw o1_D17-0x0
    dw o1_D6d-0x0
    db 0x07,0x00
    dw o1_D590-0x0
    db 0x01
    dw o1_L1ee
    db 0x27,0x00
    db 9
    db 0x01
    dw o1_start+0x5f0
    dw o1_D94-0x0
    dw o1_Sa9-0x0
    dw o1_Sd5-0x0
    dw o1_S115-0x0
    dw o1_S153-0x0
    dw o1_S1bf-0x0
    dw o1_D1da-0x0
    dw o1_S479-0x0
    dw o1_D4d5-0x0
    db 0x27,0x00
    db 4
    db 0x01
    dw o1_start+0x5f4
    dw o1_S25a-0x0
    dw o1_S2f2-0x0
    dw o1_S386-0x0
    dw o1_S469-0x0
    db 0x07,0x00
    dw o1_D588-0x0
    db 0x01
    dw o1_L13b
    db 0x27,0x00
    db 4
    db 0x01
    dw o1_start+0x5fc
    dw o1_S276-0x0
    dw o1_S27e-0x0
    dw o1_S2d5-0x0
    dw o1_S2de-0x0
fr_p1:

section o1 start=o1_file vstart=0 align=1
bits 32
o1_start:
            db   055h, 08Bh, 0ECh, 08Bh, 045h, 00Ch, 050h, 0CDh, 020h, 00Dh, 000h, 040h, 000h, 059h, 085h, 0C0h
            db   074h, 003h, 0FCh, 0C9h, 0C3h, 08Bh, 00Dh
o1_D17:
    dd o1_D5ec
            db   066h, 083h, 039h, 000h, 075h, 0F1h, 0CDh, 020h, 011h, 000h, 040h, 000h, 0FFh, 075h, 00Ch, 0CDh
            db   020h, 00Dh, 000h, 040h, 000h, 059h, 0FCh, 0C9h, 0C3h
o1_L34:
    push ebp
    mov_ ebp, esp
    mov eax, dword [ebp + 8]
    push eax
    int 0x20
    push cs
    db 0x00,0x40,0x00                       ; add byte ptr [eax], al
    pop eax
    cld
    xor_ eax, eax
    leave
    ret
o1_L47:
    push ebp
    mov_ ebp, esp
    push esi
    push edi
    cld
    mov edi, dword [ebp + 0xc]
    mov esi, dword [ebp + 8]
    mov ecx, dword [ebp + 0x10]
    repe cmpsb
    movzx eax, byte [esi - 1]
    movzx edx, byte [edi - 1]
    sub_ eax, edx
    pop edi
    pop esi
    leave
    ret
o1_L66:
    int 0x20
    add al, 0
            db   015h, 000h, 0A3h
o1_D6d:
    dd o1_D5ec
            db   0CDh, 020h, 000h, 000h, 014h, 000h, 072h, 01Fh, 03Dh, 00Ah, 003h, 000h, 000h, 072h, 018h, 0B8h
            db   005h, 000h, 000h, 000h, 0B9h, 02Ch, 000h, 000h, 000h, 0CDh, 020h, 09Bh, 000h, 001h, 000h, 072h
            db   006h, 089h, 035h
o1_D94:
    dd o1_start+0x5f0
            db   0C3h
o1_L99:
    cmp eax, 1
    je short o1_L66
    cmp eax, 0x1b
    je short o1_L66
    clc
    ret
o1_La5:
    push esi
    push edi
o1_La7:
    db 0x8b,0x35                            ; mov esi, dword ptr [0x5f0]
o1_Sa9:
    dd o1_start+0x5f0
    int 0x20
    mov dword [0x74000100], eax
    sbb dword [dword edx + 0x304b3808], ecx
    jne short o1_Lc6
    mov cl, byte [eax + 1]
    cmp byte [ebx + 3], cl
    jne short o1_Lc6
    jmp short o1_Lce
o1_Lc6:
    int 0x20
    movsb
    add byte [ecx], al
    add byte [ebp - 0x19], dh
o1_Lce:
    pop edi
    pop esi
    ret
o1_Ld1:
    push esi
    push edi
o1_Ld3:
    db 0x8b,0x35                            ; mov esi, dword ptr [0x5f0]
o1_Sd5:
    dd o1_start+0x5f0
    int 0x20
    mov dword [0x74000100], eax
    sub eax, 0x4b38088a
    xor byte [ebp + 0x1e], dh
    push eax
    push ebx
    push 0x10
    lea ecx, [ebx + 0x1a]
    push ecx
    lea ecx, [eax + 2]
    push ecx
    call o1_L47
    add esp, 0xc
    mov_ ecx, eax
    pop ebx
    pop eax
    or_ ecx, ecx
    jne short o1_L106
    jmp short o1_L10e
o1_L106:
    int 0x20
    movsb
    add byte [ecx], al
    add byte [ebp - 0x2d], dh
o1_L10e:
    pop edi
    pop esi
    ret
o1_L111:
    pushfd
    cli
o1_L113:
    db 0x8b,0x35                            ; mov esi, dword ptr [0x5f0]
o1_S115:
    dd o1_start+0x5f0
    int 0x20
    mov eax, dword [0x9d000100]
    push esi
    mov esi, dword [eax + 0x24]
    int 0x20
    pushfd
    add byte [ecx], al
    add byte [dword ebx + 0x20cd1c70], cl
    pushfd
    add byte [ecx], al
    add byte [esi - 0x33], bl
    and byte [dword edx - 0x3cffff00], ah
o1_L13b:
    mov eax, strict dword 1
    clc
    ret
o1_L142:
    push esi
    push edi
    call o1_Ld1
    or_ eax, eax
    jne near o1_L1e8
o1_L151:
    db 0x8b,0x35                            ; mov esi, dword ptr [0x5f0]
o1_S153:
    dd o1_start+0x5f0
    int 0x20
    popfd
    add byte [ecx], al
    add byte [edx + 0x6d], dh
    mov cl, byte [ebx + 3]
    mov byte [eax + 1], cl
    mov cl, byte [ebx + 0x30]
    mov byte [eax], cl
    sub_ ecx, ecx
    mov dword [eax + 0x28], ecx
    mov dword [eax + 0x20], ecx
    mov dword [eax + 0x18], ecx
    mov dword [eax + 0x14], strict dword 1
    cmp byte [ebx + 1], 0xd
    jne short o1_L186
    or dword [eax + 0x14], 2
o1_L186:
    cld
    lea edi, [eax + 2]
    lea esi, [ebx + 0x1a]
    mov ecx, strict dword 0x10
    rep movsb
    push eax
    mov eax, strict dword 5
    mov ecx, strict dword 4
    int 0x20
    wait
    add byte [ecx], al
    add byte [eax + 0x72], bl
    xor byte [dword ecx - 0x47afdb90], cl
    add eax, 0xcd000000
    and byte [dword ebx + 0x58000100], bl
    jb short o1_start+0x1cf
    mov dword [eax + 0x1c], esi
o1_L1bd:
    db 0x8b,0x35                            ; mov esi, dword ptr [0x5f0]
o1_S1bf:
    dd o1_start+0x5f0
    pushfd
    cli
    int 0x20
    lahf
    add byte [ecx], al
    add byte [dword ebp - 0x743ca1a1], bl
    jo short o1_start+0x1f6
    int 0x20
    pushfd
    add byte [ecx], al
            db   000h, 08Bh, 035h
o1_D1da:
    dd o1_start+0x5f0
            db   0CDh, 020h, 0A2h, 000h, 001h, 000h, 033h, 0C0h, 0EBh, 0E4h
o1_L1e8:
    or dword [eax + 0x14], 1
    jmp short o1_start+0x1cc
o1_L1ee:
    push esi
    push edi
    call o1_Ld1
    or_ eax, eax
    je short o1_L247
    mov_ edi, eax
    btr dword [edi + 0x14], 0
    jae short o1_L237
    mov esi, dword [edi + 0x24]
    pushfd
    cli
    int 0x20
    movsd
    add byte [ecx], al
    add byte [dword ebp + 0x2574c00b], bl
    dec dword [edi + 0x28]
    db 0x60                                 ; pushal 
    mov ebx, dword [eax]
    mov byte [ebx + 1], 0x17
    mov byte [ebx + 0x31], 0x17
    cmp dword [ebx + 0x2c], 0
    je short o1_L22e
    mov eax, strict dword 0x17
    call dword [ebx + 0x2c]
o1_L22e:
    db 0x61                                 ; popal 
    int 0x20
    mov byte [0xeb000100], al
    into
o1_L237:
    mov_ eax, edi
    test dword [eax + 0x14], strict dword 2
    jne short o1_L24d
    call o1_L111
o1_L247:
    int 0x20
    add al, byte [eax]
    adc al, 0
o1_L24d:
    xor_ eax, eax
    mov byte [ebx + 1], al
    mov byte [ebx + 0x31], al
    pop edi
    pop esi
    ret
o1_L258:
    push esi
o1_L259:
    db 0xa3                                 ; mov dword ptr [0x5f4], eax
o1_S25a:
    dd o1_start+0x5f4
    or_ eax, eax
    je short o1_L27c
    mov eax, 0x140002
o1_L267:
    db 0xbe                                 ; mov esi, 0x29f
o1_S268:
    dd o1_L29f
    int 0x20
    nop
    add byte [ecx], al
    add byte [edx + 0x21], dh
o1_L274:
    db 0x89,0x35                            ; mov dword ptr [0x5fc], esi
o1_S276:
    dd o1_start+0x5fc
    jmp short o1_L295
o1_L27c:
    db 0x83,0x3d                            ; cmp dword ptr [0x5fc], 0
o1_S27e:
    dd o1_start+0x5fc
    db 0x00
    je short o1_L295
    mov eax, 0x140002
o1_L28a:
    db 0xbe                                 ; mov esi, 0x29f
o1_S28b:
    dd o1_L29f
    int 0x20
    sbb al, 1
    add dword [eax], eax
o1_L295:
    pop esi
    ret
            db   0EBh, 006h, 0FFh, 025h
o1_D29b:
    dd o1_start+0x600
o1_L29f:
    push ecx
    push edx
    push esi
    push edi
    push eax
    cld
    sub_ eax, eax
    mov al, byte [ebx]
    and eax, 0xffffff7f
    cmp eax, 0x36
    je short o1_L2f1
    cmp eax, 0x21
    je near o1_L372
    cmp eax, 0x35
    je near o1_L466
    cmp eax, 0x31
    je short o1_L310
o1_L2ca:
    clc
    pop eax
o1_L2cc:
    pop edi
    pop esi
    pop edx
    pop ecx
    cld
    jb short o1_L2e2
o1_L2d3:
    db 0x83,0x3d                            ; cmp dword ptr [0x5fc], 0
o1_S2d5:
    dd o1_start+0x5fc
    db 0x00
    je short o1_L2e2
o1_L2dc:
    db 0xff,0x25                            ; jmp dword ptr [0x5fc]
o1_S2de:
    dd o1_start+0x5fc
o1_L2e2:
    ret
o1_L2e3:
    sub_ eax, eax
    mov byte [ebx + 1], al
    mov byte [ebx + 0x31], al
    add esp, 4
    stc
    jmp short o1_L2cc
o1_L2f1:
    db 0xa1                                 ; mov eax, dword ptr [0x5f4]
o1_S2f2:
    dd o1_start+0x5f4
    or_ eax, eax
    je short o1_L2ca
    cmp dword [ebx + 0x2c], eax
    je short o1_L2ca
    push ebx
    call o1_Ld1
    pop ebx
    or_ eax, eax
    je short o1_L2ca
    or dword [eax + 0x14], 2
    jmp short o1_L2ca
o1_L310:
    push ebx
    call o1_Ld1
    pop ebx
    or_ eax, eax
    je short o1_L2ca
    mov_ edi, eax
    btr dword [edi + 0x14], 1
    jae short o1_L359
    mov esi, dword [edi + 0x1c]
    pushfd
    cli
    int 0x20
    movsd
    add byte [ecx], al
    add byte [dword ebp + 0x2574c00b], bl
    db 0x60                                 ; pushal 
    mov ebx, dword [eax]
    mov byte [ebx + 1], 0x17
    mov byte [ebx + 0x31], 0x17
    cmp dword [ebx + 0x2c], 0
    je short o1_L34d
    mov eax, strict dword 0x17
    call dword [ebx + 0x2c]
o1_L34d:
    db 0x61                                 ; popal 
    dec dword [edi + 0x20]
    int 0x20
    mov byte [0xeb000100], al
    into
o1_L359:
    mov_ eax, edi
    test dword [eax + 0x14], strict dword 1
    jne near o1_L2e3
    call o1_L111
    jmp near o1_L2ca
o1_L372:
    push ebx
    call o1_La5
    pop ebx
    or_ eax, eax
    je near o1_L2ca
    mov_ edi, eax
    sub_ edx, edx
o1_L385:
    db 0xa1                                 ; mov eax, dword ptr [0x5f4]
o1_S386:
    dd o1_start+0x5f4
    or_ eax, eax
    je short o1_L396
    cmp dword [ebx + 0x2c], eax
    jne short o1_L396
    inc edx
    jmp short o1_L3b5
o1_L396:
    test dword [edi + 0x14], strict dword 1
    jne short o1_L3b5
    cmp dword [edi + 0x18], 0
    jne near o1_L2ca
    mov_ eax, edi
    call o1_L111
    jmp near o1_L2ca
o1_L3b5:
    mov esi, dword [edi + edx*8 + 0x1c]
    int 0x20
    popfd
    add byte [ecx], al
    add byte [edi], cl
    db 0x82,0x05,0xff,0xff,0xff,0x89,0x18   ; add byte ptr [0x89ffffff], 0x18
    pushfd
    cli
    int 0x20
    lahf
    add byte [ecx], al
    add bh, bh
    inc esp
    xlatb
    and byte [dword ebx + 0x4c3b184f], cl
    xlatb
    and byte [edx + 0x14], dh
    popfd
    mov byte [ebx + 1], 0xff
    mov byte [ebx + 0x31], 0xff
    sub_ eax, eax
    add esp, 4
    stc
    jmp near o1_L2cc
            db   0FFh, 047h, 018h, 09Dh, 08Bh, 0F0h, 052h, 053h, 0B9h, 040h, 000h, 000h, 000h, 081h, 0C1h, 0ADh
            db   005h, 000h, 000h, 051h, 06Ah, 000h, 0E8h, 0F5h, 0FBh, 0FFh, 0FFh, 083h, 0C4h, 008h, 05Bh, 05Ah
            db   00Bh, 0C0h, 075h, 01Ah, 08Bh, 0C6h, 08Bh, 074h, 0D7h, 01Ch, 09Ch, 0FAh, 0CDh, 020h, 0A1h, 000h
            db   001h, 000h, 09Dh, 0CDh, 020h, 0A2h, 000h, 001h, 000h, 0E9h, 09Ch, 0FEh, 0FFh, 0FFh, 0BFh
o1_D42f:
    dd o1_L4e6
            db   089h, 078h, 02Ch, 08Ah, 00Bh, 088h, 008h, 080h, 008h, 080h, 066h, 0C7h, 040h, 008h, 0ADh, 005h
            db   08Bh, 0F8h, 083h, 0C7h, 040h, 089h, 078h, 004h, 08Ah, 04Bh, 030h, 088h, 048h, 030h, 08Ah, 04Bh
            db   003h, 088h, 048h, 003h, 0C6h, 043h, 001h, 0FFh, 0C6h, 043h, 031h, 0FFh, 08Bh, 0D8h, 0E9h, 064h
            db   0FEh, 0FFh, 0FFh
o1_L466:
    sub_ edx, edx
o1_L468:
    db 0xa1                                 ; mov eax, dword ptr [0x5f4]
o1_S469:
    dd o1_start+0x5f4
    or_ eax, eax
    je short o1_L477
    cmp dword [ebx + 0x2c], eax
    jne short o1_L477
    inc edx
o1_L477:
    db 0x8b,0x35                            ; mov esi, dword ptr [0x5f0]
o1_S479:
    dd o1_start+0x5f0
    int 0x20
    mov dword [0xf000100], eax
    test byte [ecx - 2], al
            db   0FFh, 0FFh, 050h, 08Bh, 074h, 0D0h, 01Ch, 09Ch, 0FAh, 0CDh, 020h, 0A3h, 000h, 001h, 000h, 074h
            db   039h, 039h, 018h, 075h, 02Dh, 0CDh, 020h, 0A1h, 000h, 001h, 000h, 09Dh, 060h, 08Bh, 018h, 0C6h
            db   043h, 001h, 00Bh, 0C6h, 043h, 031h, 00Bh, 083h, 07Bh, 02Ch, 000h, 074h, 008h, 0B8h, 00Bh, 000h
            db   000h, 000h, 0FFh, 053h, 02Ch, 061h, 0CDh, 020h, 0A2h, 000h, 001h, 000h, 058h, 0E9h, 01Ah, 0FEh
            db   0FFh, 0FFh, 0CDh, 020h, 0A4h, 000h, 001h, 000h, 075h, 0C7h, 09Dh, 058h, 08Bh, 035h
o1_D4d5:
    dd o1_start+0x5f0
            db   0CDh, 020h, 0A4h, 000h, 001h, 000h, 075h, 0A8h, 0E9h, 0E4h, 0FDh, 0FFh, 0FFh
o1_L4e6:
    db 0x60                                 ; pushal 
    cld
    push ebx
    call o1_La5
    pop ebx
    or_ eax, eax
    je near o1_L579
    mov_ edi, eax
    mov edx, strict dword 1
o1_L4fe:
    mov esi, dword [edi + edx*8 + 0x1c]
    int 0x20
    movsd
    add byte [ecx], al
    add byte [esp + ebp*2 - 1], dh
    dec esp
    xlatb
    and byte [eax + 0x52], dl
    push esi
    push edi
    mov eax, dword [eax]
    sub_ ecx, ecx
    mov cx, word [eax + 8]
    cmp cx, word [ebx + 8]
    jge short o1_L532
    cmp byte [ebx + 1], 0
    jne short o1_L52d
    mov edx, strict dword 6
    jmp short o1_L53d
o1_L52d:
    mov dl, byte [ebx + 1]
    jmp short o1_L53d
o1_L532:
    mov dl, byte [ebx + 1]
    mov cx, word [ebx + 8]
    mov word [eax + 8], cx
o1_L53d:
    mov edi, dword [eax + 4]
    mov esi, dword [ebx + 4]
    cld
    rep movsb
    mov ecx, strict dword 0x10
    lea edi, [eax + 0xa]
    lea esi, [ebx + 0xa]
    cld
    rep movsb
    mov byte [eax + 1], dl
    mov byte [eax + 0x31], dl
    db 0x60                                 ; pushal 
    mov_ ebx, eax
    sub_ eax, eax
    cmp dword [ebx + 0x2c], eax
    je short o1_L56b
    mov al, byte [ebx + 1]
    call dword [ebx + 0x2c]
    cld
o1_L56b:
    db 0x61                                 ; popal 
    pop edi
    pop esi
    pop edx
    pop eax
    int 0x20
    mov byte [0x4a000100], al
    je short o1_L4fe
o1_L579:
    dec dword [edi + 0x18]
    push ebx
    call o1_L34
    add esp, 4
    db 0x61                                 ; popal 
    ret
            db   000h
o1_D588:
    dd o1_L13b
o1_D58c:
    dd o1_L142
o1_D590:
    dd o1_L1ee
o1_D594:
    dd o1_L258
o1_D598:
    dd o1_L29f
    add byte [eax], al
    add byte [eax], al
    add byte [eax + ebx*4], al
    add al, 3
    or al, byte [eax]
    add byte [ebx + 0x70], dl
    insb
    imul esi, dword [esp + esi*2 + 0x65], strict dword 0x72
o1_L5b3:
    db 0x80                                 ; sbb byte ptr [ecx], 0
o1_S5b4:
    dd o1_L99
    db 0x00,0x00
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
o1_L5cc:
    dd o1_D588                              ; mov byte ptr [0x50000], al
    db 0x05,0x00
    add byte [eax], al
    add byte [eax], al
    add byte [eax], al
    jbe short o1_start+0x63f
    jb short o1_start+0x62c
    push eax
    add byte [eax], al
    add byte [ecx], dh
    jbe short o1_start+0x656
    push edx
    xor dh, byte [esi + 0x73]
    push edx
    xor esi, dword [esi + 0x73]
    push edx
o1_D5ec:

section part1 start=0x7ec vstart=0x7ec align=1
            db   '%Splitter Virtual Device'
            db   009h
            db   '(Version 4.0)'
            db   000h, 000h, 00Ch
            db   'Splitter_DDB'
            db   001h, 000h, 000h, 0FFh, 010h, 000h, 0FFh, 001h, 000h, 030h, 000h, 0F4h, 001h, 000h, 000h, 0F4h
            db   001h, 034h, 000h
            db   'VS_VERSION_INFO'
            db   000h, 0BDh, 004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h
            db   000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 001h, 000h, 001h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 085h, 001h, 000h, 000h
            db   'StringFileInfo'
            db   000h, 000h, 071h, 001h, 000h, 000h
            db   '040904E4'
            db   000h, 000h, 000h, 000h, 026h, 000h, 016h, 000h
            db   'CompanyName'
            db   000h
            db   'Microsoft Corporation'
            db   000h, 000h, 000h, 041h, 000h, 02Dh, 000h
            db   'FileDescription'
            db   000h
            db   'Splitter Virtual Device        (Version 4.0)'
            db   000h, 000h, 000h, 000h, 019h, 000h, 009h, 000h
            db   'FileVersion'
            db   000h
            db   '4.00.950'
            db   000h, 000h, 000h, 000h, 01Dh, 000h, 009h, 000h
            db   'InternalName'
            db   000h, 000h, 000h, 000h
            db   'Splitter'
            db   000h, 000h, 000h, 000h, 03Ah, 000h, 026h, 000h
            db   'LegalCopyright'
            db   000h, 000h
            db   'Copyright '
            db   0A9h
            db   ' Microsoft Corp. 1988-1995'
            db   000h, 000h, 000h, 025h, 000h, 00Dh, 000h
            db   'OriginalFilename'
            db   000h, 000h, 000h, 000h
            db   'Splitter.VXD'
            db   000h, 000h, 000h, 000h, 035h, 000h, 025h, 000h
            db   'ProductName'
            db   000h
            db   'Microsoft'
            db   0AEh
            db   ' Windows'
            db   0AEh
            db   ' Operating System'
            db   000h, 000h, 000h, 000h, 01Dh, 000h, 009h, 000h
            db   'ProductVersion'
            db   000h, 000h
            db   '4.00.950'
            db   000h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
            db   'VarFileInfo'
            db   000h, 014h, 000h, 004h, 000h
            db   'Translation'
            db   000h, 009h, 004h, 0E4h, 004h

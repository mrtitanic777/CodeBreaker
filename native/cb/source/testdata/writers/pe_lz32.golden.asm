; lz32.dll -- assembled with NASM: nasm -f bin -o lz32.dll this file
bits 32
IMAGE equ 0xbfea0000

; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one
; Microsoft's compiler writes; NASM itself writes the other
%assign _r_eax 0
%assign _r_ecx 1
%assign _r_edx 2
%assign _r_ebx 3
%assign _r_esp 4
%assign _r_ebp 5
%assign _r_esi 6
%assign _r_edi 7
%assign _w_eax 1
%assign _w_ecx 1
%assign _w_edx 1
%assign _w_ebx 1
%assign _w_esp 1
%assign _w_ebp 1
%assign _w_esi 1
%assign _w_edi 1
%assign _r_ax 0
%assign _r_cx 1
%assign _r_dx 2
%assign _r_bx 3
%assign _r_sp 4
%assign _r_bp 5
%assign _r_si 6
%assign _r_di 7
%assign _w_ax 2
%assign _w_cx 2
%assign _w_dx 2
%assign _w_bx 2
%assign _w_sp 2
%assign _w_bp 2
%assign _w_si 2
%assign _w_di 2
%assign _r_al 0
%assign _r_cl 1
%assign _r_dl 2
%assign _r_bl 3
%assign _r_ah 4
%assign _r_ch 5
%assign _r_dh 6
%assign _r_bh 7
%assign _w_al 0
%assign _w_cl 0
%assign _w_dl 0
%assign _w_bl 0
%assign _w_ah 0
%assign _w_ch 0
%assign _w_dh 0
%assign _w_bh 0
%macro _rr 3
  %if _w_%2 == 2
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

section hdr start=0 vstart=0xbfea0000 align=1
            db   04Dh, 05Ah, 090h, 000h, 003h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 000h
            db   0B8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h
            db   00Eh, 01Fh, 0BAh, 00Eh, 000h, 0B4h, 009h, 0CDh, 021h, 0B8h, 001h, 04Ch, 0CDh
            db   '!This program cannot be run in DOS mode.'
            db   00Dh, 00Dh, 00Ah, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 045h, 000h, 000h, 04Ch
            db   001h, 006h, 000h, 058h, 003h, 00Dh, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0E0h
            db   000h, 00Eh, 021h, 00Bh, 001h, 002h, 032h, 000h, 006h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
            db   000h, 000h, 000h
    dd Lbfea1000-IMAGE                          ; the entry point
            db   000h, 010h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 000h, 0EAh, 0BFh, 000h, 010h, 000h, 000h
            db   000h, 002h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 070h, 000h, 000h, 000h, 004h, 000h, 000h, 054h, 03Bh, 000h, 000h
            db   002h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 010h, 000h, 000h, 000h, 010h, 000h, 000h
            db   000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 040h, 000h, 000h
            db   021h, 001h, 000h, 000h, 000h, 030h, 000h, 000h, 07Eh, 001h, 000h, 000h, 000h, 050h, 000h, 000h
            db   094h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 060h, 000h, 000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   '.text'
            db   000h, 000h, 000h, 06Ah, 004h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 006h, 000h, 000h, 000h
            db   004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h
            db   000h, 000h
            db   '`.data'
            db   000h, 000h, 000h, 08Ah, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
            db   00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h
            db   000h, 000h, 0D0h
            db   '.idata'
            db   000h, 000h, 07Eh, 001h, 000h, 000h, 000h, 030h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 00Ch
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h
            db   000h
            db   'P.edata'
            db   000h, 000h, 021h, 001h, 000h, 000h, 000h, 040h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 00Eh
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h
            db   000h
            db   '@.rsrc'
            db   000h, 000h, 000h, 094h, 003h, 000h, 000h, 000h, 050h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
            db   010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h
            db   000h, 000h
            db   '@.reloc'
            db   000h, 000h, 08Ah, 000h, 000h, 000h, 000h, 060h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 014h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h
            db   000h, 042h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section .text progbits start=0x400 vstart=0xbfea1000 align=1
Lbfea1000:
    push ebp
    mov_ ebp, esp
    mov eax, dword [ebp + 0xc]
    cmp eax, 1
    jne short Lbfea1032
    mov al, 0
    xchg byte [Dbfea2016], al
    or_ al, al
    je short Lbfea1032
    push dword [ebp + 8]
    call Lbfea13fa
    push dword Dbfea200d
    push dword Dbfea2000
    call Lbfea1056+1
    or_ eax, eax
    je short Lbfea1035
Lbfea1032:
    sub_ eax, eax
    inc eax
Lbfea1035:
    leave
    ret 0xc
            db   0CCh, 0CCh, 0CCh
Lbfea103c:
    inc esi
    push esp
    pop edi
    dec esp
    jp short Lbfea1088
    push esp
    push 0x6b68546b
    inc ebx
    outsd
    outsb
    outsb
    arpl word [gs:ecx + ebp*2 + 0x6f], si
    outsb
    inc esp
    db 0x61                                 ; popal
    je short Lbfea10b7
Lbfea1056:
    add byte [edx + 0x68], bl
    db 0xc1,0x77,0x00,0x00                  ; sal dword ptr [edi], 0
    push dword Lbfea103c
    push dword Dbfea2018
    push edx
    jmp near Lbfea1418
Dbfea106d:
    dd Dbfea201c
    dd Dbfea2052
Lbfea1075:
    mov cl, 8
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    call Lbfea140c
    push eax
            db   0E8h, 077h, 003h, 000h
Lbfea1088:
    add byte [eax + 0x66], dl
    push dword [ebp + 0x10]
    call dword [Dbfea106d]
    movzx eax, ax
    call Lbfea1424
    call Lbfea1412
    leave
    ret 0xc
Lbfea10a5:
    mov cl, 7
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    push word [ebp + 8]
            db   0FFh, 015h
            db   06Dh, 010h, 0EAh
Lbfea10b7:
    mov edi, 0x4c2c998
Lbfea10bc:
    add byte [dword ecx - 0x1374aafa], dh
    push ecx
    sub esp, 0x3c
    push word [ebp + 8]
    push dword [ebp + 0xc]
    push word [ebp + 0x10]
    call dword [Dbfea106d]
    shl eax, 0x10
    shrd eax, edx, 0x10
    leave
    ret 0xc
Lbfea10e2:
    mov cl, 5
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    push word [ebp + 8]
    call Lbfea1400
    push eax
    push word [ebp + 0x10]
    call dword [Dbfea106d]
    cwde
    call Lbfea1412
    leave
    ret 0xc
Lbfea1109:
    mov cl, 4
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    push word [ebp + 8]
    call dword [Dbfea106d]
    xor_ eax, eax
    inc eax
    leave
    ret 4
    mov cl, 3
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    call dword [Dbfea106d]
    cwde
    leave
    ret
Lbfea1135:
    mov cl, 2
    jmp short Lbfea113b
Lbfea1139:
    mov cl, 9
Lbfea113b:
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    push word [ebp + 8]
    push word [ebp + 0xc]
    call dword [Dbfea106d]
    shl eax, 0x10
    shrd eax, edx, 0x10
    leave
    ret 8
    mov cl, 1
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    call dword [Dbfea106d]
    leave
    ret
Lbfea116c:
    mov cl, 0
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    call Lbfea140c
    push eax
    call Lbfea1400
    push eax
    call dword [Dbfea106d]
    cwde
    call Lbfea1424
    call Lbfea1412
    leave
    ret 8
            db   0CCh, 0CCh
    xor_ eax, eax
    mov cl, 2
    jmp near Lbfea142a
    xor_ eax, eax
    mov cl, 3
    jmp near Lbfea142a
    push esi
    push dword [esp + 8]
    call Lbfea1430
    movzx esi, ax
    test esi, esi
    je short Lbfea11d4
    push dword [esp + 0xc]
    call Lbfea1430
    movzx eax, ax
    test eax, eax
    je short Lbfea11d4
    push eax
    push esi
    call Lbfea1139
    jmp short Lbfea11d9
Lbfea11d4:
    mov eax, strict dword 0xffffffff
Lbfea11d9:
    pop esi
    ret 8
    push esi
    push edi
    mov edi, dword [esp + 0xc]
    push edi
    call Lbfea1430
    movzx esi, ax
    test esi, esi
    je short Lbfea11fc
    push edi
    call Lbfea1436
    push esi
    call Lbfea1109
Lbfea11fc:
    pop edi
    pop esi
    ret 4
    push dword [esp + 4]
    call Lbfea1430
    movzx eax, ax
    test eax, eax
    jne short Lbfea1218
    mov eax, strict dword 0xffffffff
    jmp short Lbfea1235
Lbfea1218:
    push eax
    call Lbfea10a5
    test eax, eax
    jl short Lbfea1235
    push eax
    call Lbfea143c
    mov_ ecx, eax
    mov eax, strict dword 0xfffffffb
    test ecx, ecx
    je short Lbfea1235
    mov_ eax, ecx
Lbfea1235:
    ret 4
    push ebp
    mov_ ebp, esp
    push esi
    push edi
    xor_ edi, edi
    mov esi, dword [ebp + 8]
    cmp_ esi, edi
    je short Lbfea127c
    push 0x104
    push edi
    call dword [__imp_GlobalAlloc]
    mov_ edi, eax
    test edi, edi
    jne short Lbfea1262
    push 8
    call dword [__imp_SetLastError]
    jmp short Lbfea12cd
Lbfea1262:
    push 0x104
    push edi
    push esi
    call dword [__imp_GetShortPathNameA]
    test eax, eax
    je short Lbfea12a5
    cmp eax, 0x104
    ja short Lbfea12a5
    mov_ esi, edi
Lbfea127c:
    push dword [ebp + 0x10]
    push dword [ebp + 0xc]
    push esi
    call Lbfea1075
    mov_ esi, eax
    test edi, edi
    je short Lbfea1295
    push edi
    call dword [__imp_GlobalFree]
Lbfea1295:
    mov_ eax, esi
    and eax, 0xffff
    cmp eax, 0xffff
    jne short Lbfea12bd
    jmp short Lbfea12cd
Lbfea12a5:
    push edi
    call dword [__imp_GlobalFree]
    movzx eax, word [ebp + 0x10]
    push eax
    push dword [ebp + 0xc]
    push esi
    call dword [__imp_OpenFile]
    jmp short Lbfea12d2
Lbfea12bd:
    push esi
    call Lbfea143c
    test eax, eax
    jne short Lbfea12d2
    push esi
    call Lbfea1109
Lbfea12cd:
    mov eax, strict dword 0xffffffff
Lbfea12d2:
    pop edi
    pop esi
    pop ebp
    ret 0xc
    push esi
    push edi
    push 0x104
    push 0
    call dword [__imp_GlobalAlloc]
    mov_ esi, eax
    test esi, esi
    je short Lbfea1324
    push 0x104
    push esi
    push dword [esp + 0x14]
    call dword [__imp_GetShortPathNameA]
    test eax, eax
    je short Lbfea1316
    cmp eax, 0x104
    jae short Lbfea1316
    push dword [esp + 0x10]
    push esi
    call Lbfea116c
    mov_ edi, eax
    jmp short Lbfea131b
Lbfea1316:
    mov edi, strict dword 0xfffffff9
Lbfea131b:
    push esi
    call dword [__imp_GlobalFree]
    jmp short Lbfea1329
Lbfea1324:
    mov edi, strict dword 0xfffffff9
Lbfea1329:
    mov_ eax, edi
    pop edi
    pop esi
    ret 8
    push esi
    push dword [esp + 8]
    call Lbfea1430
    movzx esi, ax
    test esi, esi
    je short Lbfea135a
    push dword [esp + 0xc]
    call Lbfea1430
    movzx eax, ax
    test eax, eax
    je short Lbfea135a
    push eax
    push esi
    call Lbfea1135
    jmp short Lbfea135f
Lbfea135a:
    mov eax, strict dword 0xffffffff
Lbfea135f:
    pop esi
    ret 8
    push ebx
    push esi
    push edi
    xor_ ebx, ebx
    push ebp
    push dword [esp + 0x14]
    call Lbfea1430
    movzx ebp, ax
    test ebp, ebp
    jne short Lbfea1380
    mov eax, strict dword 0xffffffff
    jmp short Lbfea13cd
Lbfea1380:
    mov edi, dword [esp + 0x1c]
    mov esi, dword [esp + 0x18]
    cmp edi, 0x7000
    jle short Lbfea13bf
Lbfea1390:
    push 0x7000
    push esi
    push ebp
    call Lbfea10e2
    test eax, eax
    jl short Lbfea13cd
    add_ ebx, eax
    cmp eax, 0x7000
    jl short Lbfea13bb
    add esi, 0x7000
    sub_ edi, eax
    cmp edi, 0x7000
    jg short Lbfea1390
    jmp short Lbfea13bf
Lbfea13bb:
    mov_ eax, ebx
    jmp short Lbfea13cd
Lbfea13bf:
    push edi
    push esi
    push ebp
    call Lbfea10e2
    test eax, eax
    jl short Lbfea13cd
    add_ eax, ebx
Lbfea13cd:
    pop ebp
    pop edi
    pop esi
    pop ebx
    ret 0xc
    push dword [esp + 4]
    call Lbfea1430
    movzx ecx, ax
    mov eax, strict dword 0xffffffff
    test ecx, ecx
    je short Lbfea13f7
    push dword [esp + 0xc]
    push dword [esp + 0xc]
    push ecx
    call Lbfea10bc+1
Lbfea13f7:
    ret 0xc
Lbfea13fa:
    jmp dword [__imp_DisableThreadLibraryCalls]
Lbfea1400:
    jmp dword [__imp_SMapLS_IP_EBP_12]
Lbfea1406:
    jmp dword [__imp_KERNEL32_dll_89]
Lbfea140c:
    jmp dword [__imp_SMapLS_IP_EBP_8]
Lbfea1412:
    jmp dword [__imp_SUnMapLS_IP_EBP_12]
Lbfea1418:
    jmp dword [__imp_KERNEL32_dll_41]
Lbfea141e:
    jmp dword [__imp_KERNEL32_dll_90]
Lbfea1424:
    jmp dword [__imp_SUnMapLS_IP_EBP_8]
Lbfea142a:
    jmp dword [__imp_KERNEL32_dll_17]
Lbfea1430:
    jmp dword [__imp_KERNEL32_dll_21]
Lbfea1436:
    jmp dword [__imp_KERNEL32_dll_22]
Lbfea143c:
    jmp dword [__imp_KERNEL32_dll_20]
            db   0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
            db   'LZA file decompression DLL'
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h

section .data progbits start=0xa00 vstart=0xbfea2000 align=1
Dbfea2000:
            db   'SYSTHUNK.DLL'
            db   000h
Dbfea200d:
            db   'LZ32.DLL'
            db   000h
Dbfea2016:
            db   001h, 000h
Dbfea2018:
            db   000h, 000h, 000h, 000h
Dbfea201c:
            db   0EBh, 01Eh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
            db   0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
            db   0E8h, 000h, 000h, 000h, 000h, 058h, 02Dh, 025h, 000h, 000h, 000h, 0BAh, 0FCh, 0FFh, 0FFh, 0FFh
            db   068h
    dd Lbfea141e
            db   0C3h
Dbfea2052:
            db   0EBh, 01Eh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
            db   0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
            db   0E8h, 000h, 000h, 000h, 000h, 05Ah, 081h, 0EAh, 025h, 000h, 000h, 000h, 052h, 068h, 0C6h, 0FFh
            db   0FFh, 0FFh, 068h
    dd Lbfea1406
            db   0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h

section .idata progbits start=0xc00 vstart=0xbfea3000 align=1
    dd Dbfea3028-IMAGE
            db   0A0h, 01Ch, 00Ch, 032h, 0FFh, 0FFh, 0FFh, 0FFh
    dd Dbfea3170-IMAGE
    dd __imp_KERNEL32_dll_21-IMAGE
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h
Dbfea3028:
            db   015h, 000h, 000h, 080h
    dd Dbfea30d4-IMAGE
            db   059h, 000h, 000h, 080h
    dd Dbfea30e8-IMAGE
    dd Dbfea30fa-IMAGE
            db   029h, 000h, 000h, 080h, 05Ah, 000h, 000h, 080h
    dd Dbfea3110-IMAGE
            db   011h, 000h, 000h, 080h
    dd Dbfea30b8-IMAGE
            db   016h, 000h, 000h, 080h, 014h, 000h, 000h, 080h
    dd Dbfea3124-IMAGE
    dd Dbfea3130-IMAGE
    dd Dbfea313e-IMAGE
    dd Dbfea3152-IMAGE
    dd Dbfea3162-IMAGE
            db   000h, 000h, 000h, 000h
__imp_KERNEL32_dll_21:
            db   06Bh, 09Bh, 0F9h, 0BFh
__imp_SMapLS_IP_EBP_12:
            db   07Eh, 01Fh, 0F7h, 0BFh
__imp_KERNEL32_dll_89:
            db   0F4h, 026h, 0F7h, 0BFh
__imp_SMapLS_IP_EBP_8:
            db   083h, 01Fh, 0F7h, 0BFh
__imp_SUnMapLS_IP_EBP_12:
            db   03Ch, 01Fh, 0F7h, 0BFh
__imp_KERNEL32_dll_41:
            db   077h, 016h, 0F7h, 0BFh
__imp_KERNEL32_dll_90:
            db   043h, 028h, 0F7h, 0BFh
__imp_SUnMapLS_IP_EBP_8:
            db   041h, 01Fh, 0F7h, 0BFh
__imp_KERNEL32_dll_17:
            db   019h, 013h, 0F7h, 0BFh
__imp_DisableThreadLibraryCalls:
            db   051h, 03Fh, 0F9h, 0BFh
__imp_KERNEL32_dll_22:
            db   048h, 09Bh, 0F9h, 0BFh
__imp_KERNEL32_dll_20:
            db   010h, 09Bh, 0F9h, 0BFh
__imp_OpenFile:
            db   047h, 0D7h, 0F9h, 0BFh
__imp_GlobalFree:
            db   0F1h, 06Dh, 0F7h, 0BFh
__imp_GetShortPathNameA:
            db   074h, 0D2h, 0F9h, 0BFh
__imp_SetLastError:
            db   0E2h, 0DAh, 0F7h, 0BFh
__imp_GlobalAlloc:
            db   004h, 049h, 0F7h, 0BFh, 000h, 000h, 000h, 000h
Dbfea30b8:
            db   058h, 000h
            db   'DisableThreadLibraryCalls'
            db   000h
Dbfea30d4:
            db   0EBh, 001h
            db   'SMapLS_IP_EBP_12'
            db   000h, 000h
Dbfea30e8:
            db   0F3h, 001h
            db   'SMapLS_IP_EBP_8'
            db   000h
Dbfea30fa:
            db   0F5h, 001h
            db   'SUnMapLS_IP_EBP_12'
            db   000h, 000h
Dbfea3110:
            db   0FDh, 001h
            db   'SUnMapLS_IP_EBP_8'
            db   000h
Dbfea3124:
            db   0B4h, 001h
            db   'OpenFile'
            db   000h, 000h
Dbfea3130:
            db   056h, 001h
            db   'GlobalFree'
            db   000h, 000h
Dbfea313e:
            db   022h, 001h
            db   'GetShortPathNameA'
            db   000h
Dbfea3152:
            db   028h, 002h
            db   'SetLastError'
            db   000h, 000h
Dbfea3162:
            db   04Fh, 001h
            db   'GlobalAlloc'
            db   000h
Dbfea3170:
            db   'KERNEL32.dll'
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h

section .edata progbits start=0xe00 vstart=0xbfea4000 align=1
            db   000h, 000h, 000h, 000h, 065h, 067h, 0F3h, 02Fh, 000h, 000h, 000h, 000h, 0A0h, 040h, 000h, 000h
            db   001h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 028h, 040h, 000h, 000h
            db   058h, 040h, 000h, 000h, 088h, 040h, 000h, 000h, 0AAh, 011h, 000h, 000h, 038h, 012h, 000h, 000h
            db   001h, 012h, 000h, 000h, 0D4h, 013h, 000h, 000h, 063h, 013h, 000h, 000h, 0DDh, 011h, 000h, 000h
            db   023h, 011h, 000h, 000h, 030h, 013h, 000h, 000h, 05Bh, 011h, 000h, 000h, 0D8h, 012h, 000h, 000h
            db   0A1h, 011h, 000h, 000h, 098h, 011h, 000h, 000h, 0A9h, 040h, 000h, 000h, 0B4h, 040h, 000h, 000h
            db   0C5h, 040h, 000h, 000h, 0D6h, 040h, 000h, 000h, 0DEh, 040h, 000h, 000h, 0E5h, 040h, 000h, 000h
            db   0ECh, 040h, 000h, 000h, 0F3h, 040h, 000h, 000h, 0FFh, 040h, 000h, 000h, 00Bh, 041h, 000h, 000h
            db   012h, 041h, 000h, 000h, 019h, 041h, 000h, 000h, 007h, 000h, 009h, 000h, 00Bh, 000h, 005h, 000h
            db   000h, 000h, 008h, 000h, 002h, 000h, 001h, 000h, 00Ah, 000h, 004h, 000h, 003h, 000h, 006h, 000h
            db   'LZ32.dll'
            db   000h
            db   'CopyLZFile'
            db   000h
            db   'GetExpandedNameA'
            db   000h
            db   'GetExpandedNameW'
            db   000h
            db   'LZClose'
            db   000h
            db   'LZCopy'
            db   000h
            db   'LZDone'
            db   000h
            db   'LZInit'
            db   000h
            db   'LZOpenFileA'
            db   000h
            db   'LZOpenFileW'
            db   000h
            db   'LZRead'
            db   000h
            db   'LZSeek'
            db   000h
            db   'LZStart'
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section .rsrc progbits start=0x1000 vstart=0xbfea5000 align=1
            db   000h, 000h, 000h, 000h, 066h, 067h, 0F3h, 02Fh, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h
            db   010h, 000h, 000h, 000h, 018h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 066h, 067h, 0F3h, 02Fh
            db   000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 000h, 000h, 030h, 000h, 000h, 080h
            db   000h, 000h, 000h, 000h, 066h, 067h, 0F3h, 02Fh, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h
            db   009h, 004h, 000h, 000h, 048h, 000h, 000h, 000h, 060h, 050h, 000h, 000h, 034h, 003h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   034h, 003h, 034h, 000h, 000h, 000h, 056h, 000h, 053h, 000h, 05Fh, 000h, 056h, 000h, 045h, 000h
            db   052h, 000h, 053h, 000h, 049h, 000h, 04Fh, 000h, 04Eh, 000h, 05Fh, 000h, 049h, 000h, 04Eh, 000h
            db   046h, 000h, 04Fh, 000h, 000h, 000h, 000h, 000h, 0BDh, 004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h
            db   000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h
            db   03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 002h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 092h, 002h, 000h, 000h
            db   001h, 000h, 053h, 000h, 074h, 000h, 072h, 000h, 069h, 000h, 06Eh, 000h, 067h, 000h, 046h, 000h
            db   069h, 000h, 06Ch, 000h, 065h, 000h, 049h, 000h, 06Eh, 000h, 066h, 000h, 06Fh, 000h, 000h, 000h
            db   06Eh, 002h, 000h, 000h, 001h, 000h, 030h, 000h, 034h, 000h, 030h, 000h, 039h, 000h, 030h, 000h
            db   034h, 000h, 045h, 000h, 034h, 000h, 000h, 000h, 04Ch, 000h, 016h, 000h, 001h, 000h, 043h, 000h
            db   06Fh, 000h, 06Dh, 000h, 070h, 000h, 061h, 000h, 06Eh, 000h, 079h, 000h, 04Eh, 000h, 061h, 000h
            db   06Dh, 000h, 065h, 000h, 000h, 000h, 000h, 000h, 04Dh, 000h, 069h, 000h, 063h, 000h, 072h, 000h
            db   06Fh, 000h, 073h, 000h, 06Fh, 000h, 066h, 000h, 074h, 000h, 020h, 000h, 043h, 000h, 06Fh, 000h
            db   072h, 000h, 070h, 000h, 06Fh, 000h, 072h, 000h, 061h, 000h, 074h, 000h, 069h, 000h, 06Fh, 000h
            db   06Eh, 000h, 000h, 000h, 05Ch, 000h, 01Ah, 000h, 001h, 000h, 046h, 000h, 069h, 000h, 06Ch, 000h
            db   065h, 000h, 044h, 000h, 065h, 000h, 073h, 000h, 063h, 000h, 072h, 000h, 069h, 000h, 070h, 000h
            db   074h, 000h, 069h, 000h, 06Fh, 000h, 06Eh, 000h, 000h, 000h, 000h, 000h, 057h, 000h, 069h, 000h
            db   06Eh, 000h, 033h, 000h, 032h, 000h, 020h, 000h, 04Ch, 000h, 05Ah, 000h, 033h, 000h, 032h, 000h
            db   020h, 000h, 063h, 000h, 06Fh, 000h, 072h, 000h, 065h, 000h, 020h, 000h, 063h, 000h, 06Fh, 000h
            db   06Dh, 000h, 070h, 000h, 06Fh, 000h, 06Eh, 000h, 065h, 000h, 06Eh, 000h, 074h, 000h, 000h, 000h
            db   032h, 000h, 009h, 000h, 001h, 000h, 046h, 000h, 069h, 000h, 06Ch, 000h, 065h, 000h, 056h, 000h
            db   065h, 000h, 072h, 000h, 073h, 000h, 069h, 000h, 06Fh, 000h, 06Eh, 000h, 000h, 000h, 000h, 000h
            db   034h, 000h, 02Eh, 000h, 030h, 000h, 030h, 000h, 02Eh, 000h, 039h, 000h, 035h, 000h, 030h, 000h
            db   000h, 000h, 000h, 000h, 02Ah, 000h, 005h, 000h, 001h, 000h, 049h, 000h, 06Eh, 000h, 074h, 000h
            db   065h, 000h, 072h, 000h, 06Eh, 000h, 061h, 000h, 06Ch, 000h, 04Eh, 000h, 061h, 000h, 06Dh, 000h
            db   065h, 000h, 000h, 000h, 04Ch, 000h, 05Ah, 000h, 033h, 000h, 032h, 000h, 000h, 000h, 000h, 000h
            db   070h, 000h, 026h, 000h, 001h, 000h, 04Ch, 000h, 065h, 000h, 067h, 000h, 061h, 000h, 06Ch, 000h
            db   043h, 000h, 06Fh, 000h, 070h, 000h, 079h, 000h, 072h, 000h, 069h, 000h, 067h, 000h, 068h, 000h
            db   074h, 000h, 000h, 000h, 043h, 000h, 06Fh, 000h, 070h, 000h, 079h, 000h, 072h, 000h, 069h, 000h
            db   067h, 000h, 068h, 000h, 074h, 000h, 020h, 000h, 0A9h, 000h, 020h, 000h, 04Dh, 000h, 069h, 000h
            db   063h, 000h, 072h, 000h, 06Fh, 000h, 073h, 000h, 06Fh, 000h, 066h, 000h, 074h, 000h, 020h, 000h
            db   043h, 000h, 06Fh, 000h, 072h, 000h, 070h, 000h, 02Eh, 000h, 020h, 000h, 031h, 000h, 039h, 000h
            db   039h, 000h, 031h, 000h, 02Dh, 000h, 031h, 000h, 039h, 000h, 039h, 000h, 035h, 000h, 000h, 000h
            db   03Ah, 000h, 009h, 000h, 001h, 000h, 04Fh, 000h, 072h, 000h, 069h, 000h, 067h, 000h, 069h, 000h
            db   06Eh, 000h, 061h, 000h, 06Ch, 000h, 046h, 000h, 069h, 000h, 06Ch, 000h, 065h, 000h, 06Eh, 000h
            db   061h, 000h, 06Dh, 000h, 065h, 000h, 000h, 000h, 04Ch, 000h, 05Ah, 000h, 033h, 000h, 032h, 000h
            db   02Eh, 000h, 044h, 000h, 04Ch, 000h, 04Ch, 000h, 000h, 000h, 000h, 000h, 06Ah, 000h, 025h, 000h
            db   001h, 000h, 050h, 000h, 072h, 000h, 06Fh, 000h, 064h, 000h, 075h, 000h, 063h, 000h, 074h, 000h
            db   04Eh, 000h, 061h, 000h, 06Dh, 000h, 065h, 000h, 000h, 000h, 000h, 000h, 04Dh, 000h, 069h, 000h
            db   063h, 000h, 072h, 000h, 06Fh, 000h, 073h, 000h, 06Fh, 000h, 066h, 000h, 074h, 000h, 0AEh, 000h
            db   020h, 000h, 057h, 000h, 069h, 000h, 06Eh, 000h, 064h, 000h, 06Fh, 000h, 077h, 000h, 073h, 000h
            db   0AEh, 000h, 020h, 000h, 04Fh, 000h, 070h, 000h, 065h, 000h, 072h, 000h, 061h, 000h, 074h, 000h
            db   069h, 000h, 06Eh, 000h, 067h, 000h, 020h, 000h, 053h, 000h, 079h, 000h, 073h, 000h, 074h, 000h
            db   065h, 000h, 06Dh, 000h, 000h, 000h, 000h, 000h, 036h, 000h, 009h, 000h, 001h, 000h, 050h, 000h
            db   072h, 000h, 06Fh, 000h, 064h, 000h, 075h, 000h, 063h, 000h, 074h, 000h, 056h, 000h, 065h, 000h
            db   072h, 000h, 073h, 000h, 069h, 000h, 06Fh, 000h, 06Eh, 000h, 000h, 000h, 034h, 000h, 02Eh, 000h
            db   030h, 000h, 030h, 000h, 02Eh, 000h, 039h, 000h, 035h, 000h, 030h, 000h, 000h, 000h, 000h, 000h
            db   044h, 000h, 000h, 000h, 001h, 000h, 056h, 000h, 061h, 000h, 072h, 000h, 046h, 000h, 069h, 000h
            db   06Ch, 000h, 065h, 000h, 049h, 000h, 06Eh, 000h, 066h, 000h, 06Fh, 000h, 000h, 000h, 000h, 000h
            db   024h, 000h, 004h, 000h, 000h, 000h, 054h, 000h, 072h, 000h, 061h, 000h, 06Eh, 000h, 073h, 000h
            db   06Ch, 000h, 061h, 000h, 074h, 000h, 069h, 000h, 06Fh, 000h, 06Eh, 000h, 000h, 000h, 000h, 000h
            db   009h, 004h, 0E4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section .reloc progbits start=0x1400 vstart=0xbfea6000 align=1
            db   000h, 010h, 000h, 000h, 054h, 000h, 000h, 000h, 00Fh
            db   '0 0%0^0c0m0q0'
            db   090h, 030h, 0B4h, 030h, 0D3h, 030h, 0FBh, 030h, 018h
            db   '1.1L1f1'
            db   083h
            db   '1N2\2k2'
            db   091h, 032h, 0A8h, 032h, 0B7h, 032h, 0E3h, 032h, 0F9h, 032h, 01Eh, 033h, 0FCh, 033h, 002h, 034h
            db   008h, 034h, 00Eh, 034h, 014h, 034h, 01Ah
            db   '4 4&4,42484>4'
            db   000h, 000h, 000h, 020h, 000h, 000h, 00Ch, 000h, 000h, 000h, 04Dh, 030h, 085h, 030h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

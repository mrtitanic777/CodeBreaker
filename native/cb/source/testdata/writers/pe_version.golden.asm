; version.dll -- assembled with NASM: nasm -f bin -o version.dll this file
bits 32
IMAGE equ 0xbfeb0000

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

section hdr start=0 vstart=0xbfeb0000 align=1
            db   04Dh, 05Ah, 090h, 000h, 003h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 000h
            db   0B8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h
            db   00Eh, 01Fh, 0BAh, 00Eh, 000h, 0B4h, 009h, 0CDh, 021h, 0B8h, 001h, 04Ch, 0CDh
            db   '!This program cannot be run in DOS mode.'
            db   00Dh, 00Dh, 00Ah, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 045h, 000h, 000h, 04Ch
            db   001h, 006h, 000h, 058h, 003h, 00Dh, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0E0h
            db   000h, 00Eh, 021h, 00Bh, 001h, 002h, 032h, 000h, 008h, 000h, 000h, 000h, 00Eh, 000h, 000h, 000h
            db   000h, 000h, 000h
    dd Lbfeb1000-IMAGE                          ; the entry point
            db   000h, 010h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 000h, 0EBh, 0BFh, 000h, 010h, 000h, 000h
            db   000h, 002h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 070h, 000h, 000h, 000h, 004h, 000h, 000h, 01Dh, 04Dh, 000h, 000h
            db   002h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 010h, 000h
            db   000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 040h, 000h, 000h
            db   0BBh, 001h, 000h, 000h, 000h, 030h, 000h, 000h, 054h, 003h, 000h, 000h, 000h, 050h, 000h, 000h
            db   0A4h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 060h, 000h, 000h, 084h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   '.text'
            db   000h, 000h, 000h, 054h, 007h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
            db   004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h
            db   000h, 000h
            db   '`.data'
            db   000h, 000h, 000h, 0A1h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
            db   00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h
            db   000h, 000h, 0D0h
            db   '.idata'
            db   000h, 000h, 054h, 003h, 000h, 000h, 000h, 030h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 00Eh
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h
            db   000h
            db   'P.edata'
            db   000h, 000h, 0BBh, 001h, 000h, 000h, 000h, 040h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 012h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h
            db   000h
            db   '@.rsrc'
            db   000h, 000h, 000h, 0A4h, 003h, 000h, 000h, 000h, 050h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
            db   014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h
            db   000h, 000h
            db   '@.reloc'
            db   000h, 000h, 0C0h, 000h, 000h, 000h, 000h, 060h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 018h
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

section .text progbits start=0x400 vstart=0xbfeb1000 align=1
Lbfeb1000:
    push ebp
    mov_ ebp, esp
    push dword [ebp + 8]
    call Lbfeb16aa
    push dword [ebp + 0xc]
    push dword [ebp + 8]
    push dword Dbfeb2008
    push dword Dbfeb2000
    call Lbfeb12e0+1
    or_ eax, eax
    je short Lbfeb1040
    push dword [ebp + 0xc]
    push dword [ebp + 8]
    push dword Dbfeb2008
    push dword Dbfeb2000
    call Lbfeb1068
    or_ eax, eax
    je short Lbfeb1040
    sub_ eax, eax
    inc eax
Lbfeb1040:
    leave
    ret 0xc
    xor_ eax, eax
    mov cl, 2
    jmp near Lbfeb16b0
    xor_ eax, eax
    mov cl, 3
    jmp near Lbfeb16b0
    xor_ eax, eax
    mov cl, 4
    jmp near Lbfeb16b0
    xor_ eax, eax
    mov cl, 8
    jmp near Lbfeb16b0
Lbfeb1068:
    pop edx
    push dword Lbfeb1079
    push dword Dbfeb2014
    push edx
    jmp near Lbfeb16ec
Lbfeb1079:
    push esi
    db 0x65,0x72                            ; jb 0xbfeb10c3
    db Lbfeb10c3-($+1)
    push esp
    push 0x68545f6b
    jne short Lbfeb10f3
    db 0x6b,0x44,0x61,0x74,0x61             ; imul eax, dword ptr [ecx + 0x74], 0x61
    xor dword [esi], esi
            db   000h
Dbfeb108d:
    dd Dbfeb2038
    dd Dbfeb2058
Lbfeb1095:
    mov cl, 5
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    call Lbfeb16e0
    push eax
    call Lbfeb16b6
    push eax
    call dword [Dbfeb108d]
    shl eax, 0x10
    shrd eax, edx, 0x10
    call Lbfeb1710
    call Lbfeb16e6
    leave
            db   0C2h
Lbfeb10c3:
    or byte [eax], al
Lbfeb10c5:
    mov cl, 4
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    call Lbfeb16e0
    push eax
    push dword [ebp + 0xc]
    push dword [ebp + 0x10]
    call Lbfeb16bc
    push eax
    call dword [Dbfeb108d]
    cwde
    call Lbfeb1710
    call Lbfeb16f2
    leave
            db   0C2h
Lbfeb10f3:
    adc byte [eax], al
Lbfeb10f5:
    mov cl, 3
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    xor_ eax, eax
    push eax
    push eax
    mov eax, dword [ebp + 0x1c]
    or_ eax, eax
    je short Lbfeb110f
    mov word [eax + 2], strict word 0
Lbfeb110f:
    push eax
    call Lbfeb1716
    mov dword [ebp - 0x44], eax
    mov eax, dword [ebp + 0x24]
    or_ eax, eax
    je short Lbfeb1125
    mov word [eax + 2], strict word 0
Lbfeb1125:
    push eax
    call Lbfeb1716
    mov dword [ebp - 0x48], eax
    push word [ebp + 8]
    call Lbfeb16b6
    push eax
    call Lbfeb16c2
    push eax
    call Lbfeb16bc
    push eax
    call Lbfeb16c8
    push eax
    push dword [ebp - 0x44]
    call Lbfeb16ce
    push eax
    push dword [ebp - 0x48]
    call dword [Dbfeb108d]
    movzx eax, ax
    call Lbfeb16e6
    call Lbfeb16f8
    call Lbfeb16f2
    call Lbfeb16fe
    push eax
    push dword [ebp - 0x44]
    call Lbfeb16da
    pop eax
    call Lbfeb1704
    push eax
    push dword [ebp - 0x48]
    call Lbfeb16da
    pop eax
    leave
    ret 0x20
Lbfeb1190:
    mov cl, 2
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    xor_ eax, eax
    push eax
    mov eax, dword [ebp + 0x24]
    or_ eax, eax
    je short Lbfeb11a9
    mov word [eax + 2], strict word 0
Lbfeb11a9:
    push eax
    call Lbfeb1716
    mov dword [ebp - 0x44], eax
    push word [ebp + 8]
    call Lbfeb16b6
    push eax
    call Lbfeb16c2
    push eax
    call Lbfeb16bc
    push eax
    call Lbfeb16c8
    push eax
    call Lbfeb16d4
    push eax
    call Lbfeb16ce
    push eax
    push dword [ebp - 0x44]
    call dword [Dbfeb108d]
    shl eax, 0x10
    shrd eax, edx, 0x10
    call Lbfeb16e6
    call Lbfeb16f8
    call Lbfeb16f2
    call Lbfeb16fe
    call Lbfeb170a
    call Lbfeb1704
    push eax
    push dword [ebp - 0x44]
    call Lbfeb16da
    pop eax
    leave
    ret 0x20
    mov cl, 1
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    push word [ebp + 8]
    call Lbfeb16b6
    push eax
    push word [ebp + 0x10]
    call dword [Dbfeb108d]
    movzx eax, ax
    call Lbfeb16e6
    leave
    ret 0xc
    mov cl, 0
    push ebp
    mov_ ebp, esp
    push ecx
    sub esp, 0x3c
    xor_ eax, eax
    push eax
    push eax
    push eax
    push eax
    mov eax, dword [ebp + 8]
    mov dword [ebp - 0x50], eax
    push dword [ebp + 0x10]
    call Lbfeb1716
    mov dword [ebp - 0x4c], eax
    mov dword [ebp - 0x44], eax
    mov eax, dword [ebp + 0x14]
    or_ eax, eax
    je short Lbfeb126f
    mov word [eax + 2], strict word 0
Lbfeb126f:
    push eax
    call Lbfeb1716
    mov dword [ebp - 0x48], eax
    call Lbfeb16e0
    push eax
    call Lbfeb16b6
    push eax
    push dword [ebp - 0x44]
    push dword [ebp - 0x48]
    call dword [Dbfeb108d]
    cwde
    call Lbfeb1710
    call Lbfeb16e6
    push eax
    push dword [ebp - 0x4c]
    call Lbfeb16da
    pop eax
    or_ eax, eax
    je short Lbfeb12b4
    mov ecx, dword [ebp + 0x10]
    movzx edx, word [ecx]
    add edx, dword [ebp - 0x50]
    mov dword [ecx], edx
Lbfeb12b4:
    push eax
    push dword [ebp - 0x48]
    call Lbfeb16da
    pop eax
    leave
    ret 0x10
            db   0CCh, 0CCh
    dd Lbfeb1301
    dd Lbfeb1359
Lbfeb12cc:
    push esi
    db 0x65,0x72                            ; jb 0xbfeb1324
    db Lbfeb1322+2-($+1)
    push 0x5f4c536b
    push esp
    push 0x446b6e75
    db 0x61                                 ; popal
    je short Lbfeb133e+1
    xor dword [esi], esi
Lbfeb12e0:
    add byte [edx + 0x68], bl
    dd Lbfeb12cc
            db   068h
    dd Dbfeb2078
            db   052h, 0E9h, 0FAh, 003h, 000h, 000h, 08Bh, 0C0h
Lbfeb12f4:
    mov cl, 0x10
    mov_ esp, ebp
    ret
            db   02Eh, 08Bh, 0C0h
Lbfeb12fc:
    mov cl, 0x18
    mov_ esp, ebp
    ret
Lbfeb1301:
    xor_ eax, eax
    push eax
    push eax
    push eax
    push eax
    mov eax, dword [ebx + 0x16]
    mov dword [esp], eax
    push eax
    call Lbfeb1722
    push eax
    push dword [ebx + 0x1a]
    push dword [ebx + 0x1e]
    mov eax, dword [ebx + 0x22]
    mov dword [esp + 0x10], eax
    push eax
Lbfeb1322:
    call Lbfeb1722
    push eax
    mov eax, dword [ebx + 0x26]
    mov dword [esp + 0x18], eax
    push eax
    call Lbfeb1722
    push eax
    mov eax, dword [ebx + 0x2a]
    mov dword [esp + 0x20], eax
    push eax
Lbfeb133e:
    call Lbfeb1722
    push eax
    call Lbfeb13fb
    lea ecx, [esp]
    push ecx
    push strict dword 4
    call Lbfeb171c
    jmp short Lbfeb12fc
Lbfeb1359:
    xor_ eax, eax
    push eax
    push eax
    push eax
    push eax
    mov eax, dword [ebx + 0x16]
    mov dword [esp], eax
    push eax
    call Lbfeb1722
    push eax
    mov eax, dword [ebx + 0x1a]
    mov dword [esp + 8], eax
    push eax
    call Lbfeb1722
    push eax
    mov eax, dword [ebx + 0x1e]
    mov dword [esp + 0x10], eax
    push eax
    call Lbfeb1722
    push eax
    mov eax, dword [ebx + 0x22]
    mov dword [esp + 0x18], eax
    push eax
    call Lbfeb1722
    push eax
    call Lbfeb13b3
    mov_ edx, eax
    rol edx, 0x10
    lea ecx, [esp]
    push ecx
    push strict dword 4
    call Lbfeb171c
    jmp near Lbfeb12f4
Lbfeb13b3:
    mov eax, dword [esp + 0x10]
    push esi
    push edi
    xor_ esi, esi
    push 2
    mov dword [eax], esi
    push esi
    push dword [esp + 0x14]
    call dword [__imp_LoadLibraryExA]
    cmp_ eax, esi
    mov_ edi, eax
    je short Lbfeb13f4
    push dword [esp + 0x10]
    push dword [esp + 0x18]
    push edi
    call dword [__imp_FindResourceA]
    test eax, eax
    je short Lbfeb13ed
    push eax
    push edi
    call dword [__imp_SizeofResource]
    mov_ esi, eax
Lbfeb13ed:
    push edi
    call dword [__imp_FreeLibrary]
Lbfeb13f4:
    mov_ eax, esi
    pop edi
    pop esi
    ret 0x10
Lbfeb13fb:
    push ebx
    push esi
    push edi
    xor_ esi, esi
    push ebp
    push 2
    push esi
    push dword [esp + 0x1c]
    call dword [__imp_LoadLibraryExA]
    cmp_ eax, esi
    mov_ edi, eax
    je short Lbfeb1463
    push dword [esp + 0x18]
    push dword [esp + 0x20]
    push edi
    call dword [__imp_FindResourceA]
    cmp_ eax, esi
    mov_ ebp, eax
    je short Lbfeb145c
    push ebp
    push edi
    call dword [__imp_SizeofResource]
    test eax, eax
    je short Lbfeb145c
    mov ebx, dword [esp + 0x24]
    cmp_ ebx, eax
    jbe short Lbfeb143f
    mov_ ebx, eax
Lbfeb143f:
    push ebp
    push edi
    call dword [__imp_LoadResource]
    test eax, eax
    je short Lbfeb145c
    push ebx
    mov esi, strict dword 1
    push eax
    push dword [esp + 0x30]
    call dword [__imp_RtlMoveMemory]
Lbfeb145c:
    push edi
    call dword [__imp_FreeLibrary]
Lbfeb1463:
    mov_ eax, esi
    pop ebp
    pop edi
    pop esi
    pop ebx
    ret 0x18
    push ebp
    mov eax, dword [esp + 0xc]
    mov_ ebp, esp
    sub esp, 0x10c
    test eax, eax
    push ebx
    push esi
    push edi
    je short Lbfeb1486
    mov dword [eax], strict dword 0
Lbfeb1486:
    push 0x104
    push 0
    call dword [__imp_LocalAlloc]
    mov_ esi, eax
    test esi, esi
    jne short Lbfeb14dd
    mov ebx, dword [ebp + 8]
    mov_ esi, ebx
Lbfeb149e:
    push 0x104
    lea eax, [dword ebp - 0x10c]
    push eax
    push esi
    call dword [__imp_GetShortPathNameA]
    test eax, eax
    je short Lbfeb14fc
    cmp eax, 0x104
    ja short Lbfeb14fc
    lea eax, [ebp - 8]
    lea ecx, [dword ebp - 0x10c]
    push eax
    push ecx
    call Lbfeb1095
    mov_ edi, eax
    cmp_ esi, ebx
    je short Lbfeb14d9
    push esi
    call dword [__imp_LocalFree]
Lbfeb14d9:
    mov_ eax, edi
    jmp short Lbfeb14fe
Lbfeb14dd:
    lea eax, [ebp - 4]
    mov ebx, dword [ebp + 8]
    push eax
    push esi
    push 0x104
    push dword Dbfeb209c
    push ebx
    push 0
    call dword [__imp_SearchPathA]
    test eax, eax
    jne short Lbfeb149e
Lbfeb14fc:
    xor_ eax, eax
Lbfeb14fe:
    pop edi
    pop esi
    pop ebx
    mov_ esp, ebp
    pop ebp
    ret 8
    push ebp
    mov_ ebp, esp
    sub esp, 0x108
    push ebx
    push esi
    push edi
    push 0x104
    push 0
    call dword [__imp_LocalAlloc]
    mov_ esi, eax
    test esi, esi
    jne short Lbfeb156e
    mov ebx, dword [ebp + 8]
    mov_ esi, ebx
Lbfeb152b:
    push 0x104
    lea eax, [dword ebp - 0x108]
    push eax
    push esi
    call dword [__imp_GetShortPathNameA]
    test eax, eax
    je short Lbfeb158d
    cmp eax, 0x104
    ja short Lbfeb158d
    push dword [ebp + 0x14]
    push dword [ebp + 0x10]
    push 0
    lea edx, [dword ebp - 0x108]
    push edx
    call Lbfeb10c5
    mov_ edi, eax
    cmp_ ebx, esi
    je short Lbfeb156a
    push esi
    call dword [__imp_LocalFree]
Lbfeb156a:
    mov_ eax, edi
    jmp short Lbfeb158f
Lbfeb156e:
    lea eax, [ebp - 4]
    mov ebx, dword [ebp + 8]
    push eax
    push esi
    push 0x104
    push dword Dbfeb209c
    push ebx
    push 0
    call dword [__imp_SearchPathA]
    test eax, eax
    jne short Lbfeb152b
Lbfeb158d:
    xor_ eax, eax
Lbfeb158f:
    pop edi
    pop esi
    pop ebx
    mov_ esp, ebp
    pop ebp
    ret 0x10
    push ebp
    mov_ ebp, esp
    sub esp, 0x104
    push 0x104
    lea eax, [dword ebp - 0x104]
    push eax
    push dword [ebp + 0x14]
    call dword [__imp_GetShortPathNameA]
    test eax, eax
    je short Lbfeb15e3
    cmp eax, 0x104
    ja short Lbfeb15e3
    push dword [ebp + 0x24]
    push dword [ebp + 0x20]
    push dword [ebp + 0x1c]
    push dword [ebp + 0x18]
    lea ecx, [dword ebp - 0x104]
    push ecx
    push 0
    push dword [ebp + 0xc]
    push dword [ebp + 8]
    call Lbfeb10f5
    jmp short Lbfeb15e8
Lbfeb15e3:
    mov eax, strict dword 4
Lbfeb15e8:
    mov_ esp, ebp
    pop ebp
    ret 0x20
    push ebp
    mov_ ebp, esp
    sub esp, 0x30c
    push edi
    lea eax, [dword ebp - 0x104]
    push 0x104
    mov edi, dword [__imp_GetShortPathNameA]
    push eax
    push dword [ebp + 0x14]
    call edi
    test eax, eax
    je near Lbfeb169d
    cmp eax, 0x104
    ja short Lbfeb169d
    push 0x104
    lea eax, [dword ebp - 0x208]
    push eax
    push dword [ebp + 0x18]
    call edi
    test eax, eax
    je short Lbfeb1696
    cmp eax, 0x104
    ja short Lbfeb1696
    mov eax, dword [ebp + 0x1c]
    test eax, eax
    je short Lbfeb166f
    cmp byte [eax], 0
    je short Lbfeb166f
    push 0x104
    lea ecx, [dword ebp - 0x30c]
    push ecx
    push eax
    call edi
    test eax, eax
    je short Lbfeb1668
    cmp eax, 0x104
    ja short Lbfeb1668
    lea ecx, [dword ebp - 0x30c]
    jmp short Lbfeb1671
Lbfeb1668:
    mov eax, 0x20000
    jmp short Lbfeb16a2
Lbfeb166f:
    xor_ ecx, ecx
Lbfeb1671:
    lea eax, [dword ebp - 0x208]
    lea edx, [dword ebp - 0x104]
    push dword [ebp + 0x24]
    push dword [ebp + 0x20]
    push ecx
    push eax
    push edx
    push dword [ebp + 0x10]
    push dword [ebp + 0xc]
    push dword [ebp + 8]
    call Lbfeb1190
    jmp short Lbfeb16a2
Lbfeb1696:
    mov eax, 0x20000
    jmp short Lbfeb16a2
Lbfeb169d:
    mov eax, 0x10000
Lbfeb16a2:
    pop edi
    mov_ esp, ebp
    pop ebp
    ret 0x20
            db   0CCh
Lbfeb16aa:
    jmp dword [__imp_DisableThreadLibraryCalls]
Lbfeb16b0:
    jmp dword [__imp_KERNEL32_dll_17]
Lbfeb16b6:
    jmp dword [__imp_SMapLS_IP_EBP_12]
Lbfeb16bc:
    jmp dword [__imp_SMapLS_IP_EBP_20]
Lbfeb16c2:
    jmp dword [__imp_SMapLS_IP_EBP_16]
Lbfeb16c8:
    jmp dword [__imp_SMapLS_IP_EBP_24]
Lbfeb16ce:
    jmp dword [__imp_SMapLS_IP_EBP_32]
Lbfeb16d4:
    jmp dword [__imp_SMapLS_IP_EBP_28]
Lbfeb16da:
    jmp dword [__imp_UnMapLS]
Lbfeb16e0:
    jmp dword [__imp_SMapLS_IP_EBP_8]
Lbfeb16e6:
    jmp dword [__imp_SUnMapLS_IP_EBP_12]
Lbfeb16ec:
    jmp dword [__imp_ThunkConnect32]
Lbfeb16f2:
    jmp dword [__imp_SUnMapLS_IP_EBP_20]
Lbfeb16f8:
    jmp dword [__imp_SUnMapLS_IP_EBP_16]
Lbfeb16fe:
    jmp dword [__imp_SUnMapLS_IP_EBP_24]
Lbfeb1704:
    jmp dword [__imp_SUnMapLS_IP_EBP_32]
Lbfeb170a:
    jmp dword [__imp_SUnMapLS_IP_EBP_28]
Lbfeb1710:
    jmp dword [__imp_SUnMapLS_IP_EBP_8]
Lbfeb1716:
    jmp dword [__imp_MapLS]
Lbfeb171c:
    jmp dword [__imp_UnMapSLFixArray]
Lbfeb1722:
    jmp dword [__imp_MapSLFix]
            db   0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
            db   'VERSION - A Version stamping library'
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
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section .data progbits start=0xc00 vstart=0xbfeb2000 align=1
Dbfeb2000:
            db   'VER.DLL'
            db   000h
Dbfeb2008:
            db   'VERSION.DLL'
            db   000h
Dbfeb2014:
            db   04Ch, 053h, 030h, 031h, 0B0h, 067h, 000h, 000h, 000h, 000h, 000h, 000h, 04Ch, 042h, 030h, 031h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
            db   044h, 000h, 000h, 000h
Dbfeb2038:
            db   0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
            db   0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
Dbfeb2058:
            db   0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
            db   0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
Dbfeb2078:
            db   053h, 04Ch, 030h, 031h, 0DDh, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   04Ch, 042h, 030h, 031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   0F8h, 0FFh, 0FFh, 0FFh
Dbfeb209c:
            db   02Eh, 044h, 04Ch, 04Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
            db   000h, 000h, 000h, 000h

section .idata progbits start=0xe00 vstart=0xbfeb3000 align=1
    dd Dbfeb3028-IMAGE
            db   0A0h, 01Ch, 00Ch, 032h, 0FFh, 0FFh, 0FFh, 0FFh
    dd Dbfeb3346-IMAGE
    dd __imp_SUnMapLS_IP_EBP_28-IMAGE
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h
Dbfeb3028:
    dd Dbfeb3258-IMAGE
            db   011h, 000h, 000h, 080h
    dd Dbfeb3144-IMAGE
    dd Dbfeb3158-IMAGE
    dd Dbfeb316c-IMAGE
    dd Dbfeb3180-IMAGE
    dd Dbfeb3194-IMAGE
    dd Dbfeb31a8-IMAGE
    dd Dbfeb31bc-IMAGE
    dd Dbfeb31c6-IMAGE
    dd Dbfeb31d8-IMAGE
    dd Dbfeb31ee-IMAGE
    dd Dbfeb3200-IMAGE
    dd Dbfeb3216-IMAGE
    dd Dbfeb322c-IMAGE
    dd Dbfeb3242-IMAGE
    dd Dbfeb3128-IMAGE
    dd Dbfeb326e-IMAGE
    dd Dbfeb3282-IMAGE
    dd Dbfeb328a-IMAGE
    dd Dbfeb329c-IMAGE
    dd Dbfeb32a8-IMAGE
    dd Dbfeb32b6-IMAGE
    dd Dbfeb32c8-IMAGE
    dd Dbfeb32d8-IMAGE
    dd Dbfeb32ea-IMAGE
    dd Dbfeb32fa-IMAGE
    dd Dbfeb330a-IMAGE
    dd Dbfeb3318-IMAGE
    dd Dbfeb3324-IMAGE
    dd Dbfeb3338-IMAGE
            db   000h, 000h, 000h, 000h
__imp_SUnMapLS_IP_EBP_28:
            db   028h, 01Fh, 0F7h, 0BFh
__imp_KERNEL32_dll_17:
            db   019h, 013h, 0F7h, 0BFh
__imp_SMapLS_IP_EBP_12:
            db   07Eh, 01Fh, 0F7h, 0BFh
__imp_SMapLS_IP_EBP_20:
            db   074h, 01Fh, 0F7h, 0BFh
__imp_SMapLS_IP_EBP_16:
            db   079h, 01Fh, 0F7h, 0BFh
__imp_SMapLS_IP_EBP_24:
            db   06Fh, 01Fh, 0F7h, 0BFh
__imp_SMapLS_IP_EBP_32:
            db   065h, 01Fh, 0F7h, 0BFh
__imp_SMapLS_IP_EBP_28:
            db   06Ah, 01Fh, 0F7h, 0BFh
__imp_UnMapLS:
            db   0EBh, 01Fh, 0F7h, 0BFh
__imp_SMapLS_IP_EBP_8:
            db   083h, 01Fh, 0F7h, 0BFh
__imp_SUnMapLS_IP_EBP_12:
            db   03Ch, 01Fh, 0F7h, 0BFh
__imp_ThunkConnect32:
            db   0FFh, 02Dh, 0F7h, 0BFh
__imp_SUnMapLS_IP_EBP_20:
            db   032h, 01Fh, 0F7h, 0BFh
__imp_SUnMapLS_IP_EBP_16:
            db   037h, 01Fh, 0F7h, 0BFh
__imp_SUnMapLS_IP_EBP_24:
            db   02Dh, 01Fh, 0F7h, 0BFh
__imp_SUnMapLS_IP_EBP_32:
            db   023h, 01Fh, 0F7h, 0BFh
__imp_DisableThreadLibraryCalls:
            db   051h, 03Fh, 0F9h, 0BFh
__imp_SUnMapLS_IP_EBP_8:
            db   041h, 01Fh, 0F7h, 0BFh
__imp_MapLS:
            db   0DFh, 01Fh, 0F7h, 0BFh
__imp_UnMapSLFixArray:
            db   04Eh, 01Ch, 0F7h, 0BFh
__imp_MapSLFix:
            db   078h, 01Bh, 0F7h, 0BFh
__imp_FreeLibrary:
            db   0C6h, 0BAh, 0F8h, 0BFh
__imp_SizeofResource:
            db   019h, 0C0h, 0F9h, 0BFh
__imp_FindResourceA:
            db   0ECh, 07Dh, 0F7h, 0BFh
__imp_LoadLibraryExA:
            db   02Dh, 0EAh, 0F8h, 0BFh
__imp_RtlMoveMemory:
            db   0EFh, 07Ch, 0F7h, 0BFh
__imp_LoadResource:
            db   02Dh, 0E6h, 0F7h, 0BFh
__imp_SearchPathA:
            db   076h, 07Ah, 0F7h, 0BFh
__imp_LocalFree:
            db   020h, 04Ah, 0F7h, 0BFh
__imp_GetShortPathNameA:
            db   074h, 0D2h, 0F9h, 0BFh
__imp_LocalAlloc:
            db   004h, 049h, 0F7h, 0BFh, 000h, 000h, 000h, 000h
Dbfeb3128:
            db   058h, 000h
            db   'DisableThreadLibraryCalls'
            db   000h
Dbfeb3144:
            db   0EBh, 001h
            db   'SMapLS_IP_EBP_12'
            db   000h, 000h
Dbfeb3158:
            db   0EDh, 001h
            db   'SMapLS_IP_EBP_20'
            db   000h, 000h
Dbfeb316c:
            db   0ECh, 001h
            db   'SMapLS_IP_EBP_16'
            db   000h, 000h
Dbfeb3180:
            db   0EEh, 001h
            db   'SMapLS_IP_EBP_24'
            db   000h, 000h
Dbfeb3194:
            db   0F0h, 001h
            db   'SMapLS_IP_EBP_32'
            db   000h, 000h
Dbfeb31a8:
            db   0EFh, 001h
            db   'SMapLS_IP_EBP_28'
            db   000h, 000h
Dbfeb31bc:
            db   056h, 002h
            db   'UnMapLS'
            db   000h
Dbfeb31c6:
            db   0F3h, 001h
            db   'SMapLS_IP_EBP_8'
            db   000h
Dbfeb31d8:
            db   0F5h, 001h
            db   'SUnMapLS_IP_EBP_12'
            db   000h, 000h
Dbfeb31ee:
            db   04Ah, 002h
            db   'ThunkConnect32'
            db   000h, 000h
Dbfeb3200:
            db   0F7h, 001h
            db   'SUnMapLS_IP_EBP_20'
            db   000h, 000h
Dbfeb3216:
            db   0F6h, 001h
            db   'SUnMapLS_IP_EBP_16'
            db   000h, 000h
Dbfeb322c:
            db   0F8h, 001h
            db   'SUnMapLS_IP_EBP_24'
            db   000h, 000h
Dbfeb3242:
            db   0FAh, 001h
            db   'SUnMapLS_IP_EBP_32'
            db   000h, 000h
Dbfeb3258:
            db   0F9h, 001h
            db   'SUnMapLS_IP_EBP_28'
            db   000h, 000h
Dbfeb326e:
            db   0FDh, 001h
            db   'SUnMapLS_IP_EBP_8'
            db   000h
Dbfeb3282:
            db   0A4h, 001h
            db   'MapLS'
            db   000h
Dbfeb328a:
            db   057h, 002h
            db   'UnMapSLFixArray'
            db   000h
Dbfeb329c:
            db   0A6h, 001h
            db   'MapSLFix'
            db   000h, 000h
Dbfeb32a8:
            db   0AAh, 000h
            db   'FreeLibrary'
            db   000h
Dbfeb32b6:
            db   040h, 002h
            db   'SizeofResource'
            db   000h, 000h
Dbfeb32c8:
            db   09Ah, 000h
            db   'FindResourceA'
            db   000h
Dbfeb32d8:
            db   08Ah, 001h
            db   'LoadLibraryExA'
            db   000h, 000h
Dbfeb32ea:
            db   0E7h, 001h
            db   'RtlMoveMemory'
            db   000h
Dbfeb32fa:
            db   08Eh, 001h
            db   'LoadResource'
            db   000h, 000h
Dbfeb330a:
            db   000h, 002h
            db   'SearchPathA'
            db   000h
Dbfeb3318:
            db   093h, 001h
            db   'LocalFree'
            db   000h
Dbfeb3324:
            db   022h, 001h
            db   'GetShortPathNameA'
            db   000h
Dbfeb3338:
            db   08Fh, 001h
            db   'LocalAlloc'
            db   000h, 000h
Dbfeb3346:
            db   'KERNEL32.dll'
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

section .edata progbits start=0x1200 vstart=0xbfeb4000 align=1
            db   000h, 000h, 000h, 000h, 0A8h, 067h, 0F3h, 02Fh, 000h, 000h, 000h, 000h, 0B4h, 040h, 000h, 000h
            db   001h, 000h, 000h, 000h, 00Eh, 000h, 000h, 000h, 00Eh, 000h, 000h, 000h, 028h, 040h, 000h, 000h
            db   060h, 040h, 000h, 000h, 098h, 040h, 000h, 000h, 007h, 015h, 000h, 000h, 06Ch, 014h, 000h, 000h
            db   044h, 010h, 000h, 000h, 056h, 010h, 000h, 000h, 014h, 020h, 000h, 000h, 098h, 015h, 000h, 000h
            db   05Fh, 010h, 000h, 000h, 0EEh, 015h, 000h, 000h, 05Fh, 010h, 000h, 000h, 016h, 012h, 000h, 000h
            db   04Dh, 010h, 000h, 000h, 03Fh, 012h, 000h, 000h, 056h, 010h, 000h, 000h, 078h, 020h, 000h, 000h
            db   0C0h, 040h, 000h, 000h, 0D4h, 040h, 000h, 000h, 0ECh, 040h, 000h, 000h, 004h, 041h, 000h, 000h
            db   018h, 041h, 000h, 000h, 02Ch, 041h, 000h, 000h, 039h, 041h, 000h, 000h, 046h, 041h, 000h, 000h
            db   056h, 041h, 000h, 000h, 066h, 041h, 000h, 000h, 077h, 041h, 000h, 000h, 088h, 041h, 000h, 000h
            db   097h, 041h, 000h, 000h, 0A6h, 041h, 000h, 000h, 000h, 000h, 001h, 000h, 002h, 000h, 003h, 000h
            db   004h, 000h, 005h, 000h, 006h, 000h, 007h, 000h, 008h, 000h, 009h, 000h, 00Ah, 000h, 00Bh, 000h
            db   00Ch, 000h, 00Dh, 000h
            db   'VERSION.dll'
            db   000h
            db   'GetFileVersionInfoA'
            db   000h
            db   'GetFileVersionInfoSizeA'
            db   000h
            db   'GetFileVersionInfoSizeW'
            db   000h
            db   'GetFileVersionInfoW'
            db   000h
            db   'VerFThk_ThunkData32'
            db   000h
            db   'VerFindFileA'
            db   000h
            db   'VerFindFileW'
            db   000h
            db   'VerInstallFileA'
            db   000h
            db   'VerInstallFileW'
            db   000h
            db   'VerLanguageNameA'
            db   000h
            db   'VerLanguageNameW'
            db   000h
            db   'VerQueryValueA'
            db   000h
            db   'VerQueryValueW'
            db   000h
            db   'VerThkSL_ThunkData32'
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h

section .rsrc progbits start=0x1400 vstart=0xbfeb5000 align=1
            db   000h, 000h, 000h, 000h, 0A9h, 067h, 0F3h, 02Fh, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h
            db   010h, 000h, 000h, 000h, 018h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 0A9h, 067h, 0F3h, 02Fh
            db   000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 000h, 000h, 030h, 000h, 000h, 080h
            db   000h, 000h, 000h, 000h, 0A9h, 067h, 0F3h, 02Fh, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h
            db   009h, 004h, 000h, 000h, 048h, 000h, 000h, 000h, 060h, 050h, 000h, 000h, 044h, 003h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   044h, 003h, 034h, 000h, 000h, 000h, 056h, 000h, 053h, 000h, 05Fh, 000h, 056h, 000h, 045h, 000h
            db   052h, 000h, 053h, 000h, 049h, 000h, 04Fh, 000h, 04Eh, 000h, 05Fh, 000h, 049h, 000h, 04Eh, 000h
            db   046h, 000h, 04Fh, 000h, 000h, 000h, 000h, 000h, 0BDh, 004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h
            db   000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h
            db   03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 002h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0A2h, 002h, 000h, 000h
            db   001h, 000h, 053h, 000h, 074h, 000h, 072h, 000h, 069h, 000h, 06Eh, 000h, 067h, 000h, 046h, 000h
            db   069h, 000h, 06Ch, 000h, 065h, 000h, 049h, 000h, 06Eh, 000h, 066h, 000h, 06Fh, 000h, 000h, 000h
            db   07Eh, 002h, 000h, 000h, 001h, 000h, 030h, 000h, 034h, 000h, 030h, 000h, 039h, 000h, 030h, 000h
            db   034h, 000h, 045h, 000h, 034h, 000h, 000h, 000h, 04Ch, 000h, 016h, 000h, 001h, 000h, 043h, 000h
            db   06Fh, 000h, 06Dh, 000h, 070h, 000h, 061h, 000h, 06Eh, 000h, 079h, 000h, 04Eh, 000h, 061h, 000h
            db   06Dh, 000h, 065h, 000h, 000h, 000h, 000h, 000h, 04Dh, 000h, 069h, 000h, 063h, 000h, 072h, 000h
            db   06Fh, 000h, 073h, 000h, 06Fh, 000h, 066h, 000h, 074h, 000h, 020h, 000h, 043h, 000h, 06Fh, 000h
            db   072h, 000h, 070h, 000h, 06Fh, 000h, 072h, 000h, 061h, 000h, 074h, 000h, 069h, 000h, 06Fh, 000h
            db   06Eh, 000h, 000h, 000h, 062h, 000h, 01Dh, 000h, 001h, 000h, 046h, 000h, 069h, 000h, 06Ch, 000h
            db   065h, 000h, 044h, 000h, 065h, 000h, 073h, 000h, 063h, 000h, 072h, 000h, 069h, 000h, 070h, 000h
            db   074h, 000h, 069h, 000h, 06Fh, 000h, 06Eh, 000h, 000h, 000h, 000h, 000h, 057h, 000h, 069h, 000h
            db   06Eh, 000h, 033h, 000h, 032h, 000h, 020h, 000h, 056h, 000h, 045h, 000h, 052h, 000h, 053h, 000h
            db   049h, 000h, 04Fh, 000h, 04Eh, 000h, 020h, 000h, 063h, 000h, 06Fh, 000h, 072h, 000h, 065h, 000h
            db   020h, 000h, 063h, 000h, 06Fh, 000h, 06Dh, 000h, 070h, 000h, 06Fh, 000h, 06Eh, 000h, 065h, 000h
            db   06Eh, 000h, 074h, 000h, 000h, 000h, 000h, 000h, 032h, 000h, 009h, 000h, 001h, 000h, 046h, 000h
            db   069h, 000h, 06Ch, 000h, 065h, 000h, 056h, 000h, 065h, 000h, 072h, 000h, 073h, 000h, 069h, 000h
            db   06Fh, 000h, 06Eh, 000h, 000h, 000h, 000h, 000h, 034h, 000h, 02Eh, 000h, 030h, 000h, 030h, 000h
            db   02Eh, 000h, 039h, 000h, 035h, 000h, 030h, 000h, 000h, 000h, 000h, 000h, 030h, 000h, 008h, 000h
            db   001h, 000h, 049h, 000h, 06Eh, 000h, 074h, 000h, 065h, 000h, 072h, 000h, 06Eh, 000h, 061h, 000h
            db   06Ch, 000h, 04Eh, 000h, 061h, 000h, 06Dh, 000h, 065h, 000h, 000h, 000h, 056h, 000h, 045h, 000h
            db   052h, 000h, 053h, 000h, 049h, 000h, 04Fh, 000h, 04Eh, 000h, 000h, 000h, 070h, 000h, 026h, 000h
            db   001h, 000h, 04Ch, 000h, 065h, 000h, 067h, 000h, 061h, 000h, 06Ch, 000h, 043h, 000h, 06Fh, 000h
            db   070h, 000h, 079h, 000h, 072h, 000h, 069h, 000h, 067h, 000h, 068h, 000h, 074h, 000h, 000h, 000h
            db   043h, 000h, 06Fh, 000h, 070h, 000h, 079h, 000h, 072h, 000h, 069h, 000h, 067h, 000h, 068h, 000h
            db   074h, 000h, 020h, 000h, 0A9h, 000h, 020h, 000h, 04Dh, 000h, 069h, 000h, 063h, 000h, 072h, 000h
            db   06Fh, 000h, 073h, 000h, 06Fh, 000h, 066h, 000h, 074h, 000h, 020h, 000h, 043h, 000h, 06Fh, 000h
            db   072h, 000h, 070h, 000h, 02Eh, 000h, 020h, 000h, 031h, 000h, 039h, 000h, 039h, 000h, 031h, 000h
            db   02Dh, 000h, 031h, 000h, 039h, 000h, 039h, 000h, 035h, 000h, 000h, 000h, 040h, 000h, 00Ch, 000h
            db   001h, 000h, 04Fh, 000h, 072h, 000h, 069h, 000h, 067h, 000h, 069h, 000h, 06Eh, 000h, 061h, 000h
            db   06Ch, 000h, 046h, 000h, 069h, 000h, 06Ch, 000h, 065h, 000h, 06Eh, 000h, 061h, 000h, 06Dh, 000h
            db   065h, 000h, 000h, 000h, 056h, 000h, 045h, 000h, 052h, 000h, 053h, 000h, 049h, 000h, 04Fh, 000h
            db   04Eh, 000h, 02Eh, 000h, 044h, 000h, 04Ch, 000h, 04Ch, 000h, 000h, 000h, 06Ah, 000h, 025h, 000h
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

section .reloc progbits start=0x1800 vstart=0xbfeb6000 align=1
            db   000h, 010h, 000h, 000h, 084h, 000h, 000h, 000h, 012h, 030h, 017h
            db   '0+000j0o0'
            db   08Dh, 030h, 091h, 030h, 0ACh, 030h, 0E2h, 030h, 058h, 031h, 0DFh, 031h, 02Fh, 032h, 08Ch, 032h
            db   0C4h, 032h, 0C8h, 032h, 0E3h, 032h, 0E8h, 032h, 0C6h, 033h, 0DBh, 033h, 0E7h, 033h, 0F0h, 033h
            db   00Ah, 034h, 01Fh
            db   '4-4C4X4_4'
            db   08Fh, 034h, 0ADh, 034h, 0D5h, 034h, 0EBh, 034h, 0F4h, 034h, 01Ch
            db   '5:5f5|5'
            db   085h, 035h, 0B2h, 035h, 005h, 036h, 0ACh, 036h, 0B2h, 036h, 0B8h, 036h, 0BEh, 036h, 0C4h, 036h
            db   0CAh, 036h, 0D0h, 036h, 0D6h, 036h, 0DCh, 036h, 0E2h, 036h, 0E8h, 036h, 0EEh, 036h, 0F4h, 036h
            db   0FAh, 036h, 000h, 037h, 006h, 037h, 00Ch, 037h, 012h, 037h, 018h, 037h, 01Eh, 037h, 024h, 037h
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

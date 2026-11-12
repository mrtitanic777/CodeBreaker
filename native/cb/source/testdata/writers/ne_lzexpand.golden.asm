; lzexpand.dll -- assembled with NASM: nasm -f bin -o lzexpand.dll this-file
bits 16
SHIFT equ 4
; where each segment lies in the file: its section's place, and its sector in the segment table
seg1_start_file equ 0x300
seg2_start_file equ 0x7e0
seg3_start_file equ 0xfd0
seg4_start_file equ 0xff0
seg5_start_file equ 0x1220
seg6_start_file equ 0x13c0
seg7_start_file equ 0x14e0
seg8_start_file equ 0x1840
seg9_start_file equ 0x1ab0
seg10_start_file equ 0x1e10
seg11_start_file equ 0x2140
seg12_start_file equ 0x520

; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one
; Microsoft's compiler writes; NASM itself writes the other
%assign _r_ax 0
%assign _r_cx 1
%assign _r_dx 2
%assign _r_bx 3
%assign _r_sp 4
%assign _r_bp 5
%assign _r_si 6
%assign _r_di 7
%assign _w_ax 1
%assign _w_cx 1
%assign _w_dx 1
%assign _w_bx 1
%assign _w_sp 1
%assign _w_bp 1
%assign _w_si 1
%assign _w_di 1
%assign _r_eax 0
%assign _r_ecx 1
%assign _r_edx 2
%assign _r_ebx 3
%assign _r_esp 4
%assign _r_ebp 5
%assign _r_esi 6
%assign _r_edi 7
%assign _w_eax 2
%assign _w_ecx 2
%assign _w_edx 2
%assign _w_ebx 2
%assign _w_esp 2
%assign _w_ebp 2
%assign _w_esi 2
%assign _w_edi 2
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

section part0 start=0x0 vstart=0x0 align=1
            db   04Dh, 05Ah, 09Bh, 000h, 002h, 000h, 000h, 000h, 004h, 000h, 005h, 000h, 0FFh, 0FFh, 000h, 000h
            db   0B8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h
            db   00Eh, 01Fh, 0BAh, 00Eh, 000h, 0B4h, 009h, 0CDh, 021h, 0B8h, 001h, 04Ch, 0CDh
            db   '!This program cannot be run in DOS mode.'
            db   00Dh, 00Ah, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Eh, 045h, 005h, 014h, 0EDh
            db   000h, 0B7h, 000h, 02Ah, 07Bh, 037h, 080h, 001h, 083h, 00Ch, 000h, 000h, 004h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 002h, 000h, 08Fh, 000h, 040h, 000h, 0A0h
            db   000h, 0B8h, 000h, 0DCh, 000h, 0E0h, 000h, 024h, 002h, 000h, 000h, 01Eh, 000h, 004h, 000h, 000h
            db   000h, 002h, 008h, 02Eh, 000h, 050h, 000h, 000h, 000h, 000h, 003h
    dw seg1_start_file >> SHIFT, seg1_end
            db   050h, 01Dh, 0C8h, 001h
    dw seg2_start_file >> SHIFT, seg2_end
            db   010h, 01Dh, 05Ah, 007h
    dw seg3_start_file >> SHIFT, seg3_end
            db   010h, 01Ch, 018h, 000h
    dw seg4_start_file >> SHIFT, seg4_end
            db   010h, 01Dh, 0F8h, 001h
    dw seg5_start_file >> SHIFT, seg5_end
            db   010h, 01Ch, 092h, 001h
    dw seg6_start_file >> SHIFT, seg6_end
            db   010h, 01Dh, 00Ch, 001h
    dw seg7_start_file >> SHIFT, seg7_end
            db   010h, 01Dh, 0F0h, 002h
    dw seg8_start_file >> SHIFT, seg8_end
            db   010h, 01Dh, 048h, 002h
    dw seg9_start_file >> SHIFT, seg9_end
            db   010h, 01Dh, 03Ah, 003h
    dw seg10_start_file >> SHIFT, seg10_end
            db   010h, 01Dh, 0F8h, 002h
    dw seg11_start_file >> SHIFT, seg11_end
            db   010h, 01Dh, 072h, 003h
    dw seg12_start_file >> SHIFT, seg12_end
            db   071h, 00Ch, 084h, 002h
            db   004h, 000h, 010h, 080h, 001h, 000h, 000h, 000h, 000h, 000h, 04Fh, 002h, 01Eh, 000h, 030h, 00Ch
            db   001h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 008h
            db   'LZEXPAND'
            db   000h, 000h, 003h, 057h, 045h, 050h, 00Bh, 000h, 00Fh
            db   '___EXPORTEDSTUB'
            db   00Ch, 000h, 000h, 001h, 000h, 008h, 000h, 000h, 006h
            db   'KERNEL'
            db   004h, 055h, 053h, 045h, 052h, 01Eh, 0FFh, 003h, 0CDh, 03Fh, 004h, 05Ch, 001h, 003h, 0CDh, 03Fh
            db   002h, 062h, 002h, 003h, 0CDh, 03Fh, 002h, 000h, 000h, 003h, 0CDh, 03Fh, 002h, 0EEh, 002h, 003h
            db   0CDh, 03Fh, 002h, 0D6h, 003h, 003h, 0CDh, 03Fh, 002h, 0F0h, 006h, 003h, 0CDh, 03Fh, 004h, 000h
            db   000h, 003h, 0CDh, 03Fh, 004h, 03Ah, 001h, 003h, 0CDh, 03Fh, 004h, 0A6h, 000h, 003h, 0CDh, 03Fh
            db   002h, 0BCh, 001h, 003h, 0CDh, 03Fh, 003h, 000h, 000h, 003h, 0CDh, 03Fh, 00Bh, 05Eh, 003h, 000h
            db   0CDh, 03Fh, 00Ah, 024h, 001h, 000h, 0CDh, 03Fh, 00Ah, 0B6h, 001h, 000h, 0CDh, 03Fh, 008h, 0A2h
            db   001h, 000h, 0CDh, 03Fh, 008h, 0AAh, 000h, 000h, 0CDh, 03Fh, 005h, 052h, 000h, 000h, 0CDh, 03Fh
            db   005h, 000h, 000h, 000h, 0CDh, 03Fh, 005h, 0D0h, 000h, 000h, 0CDh, 03Fh, 009h, 000h, 000h, 000h
            db   0CDh, 03Fh, 007h, 0F4h, 000h, 000h, 0CDh, 03Fh, 001h, 0B2h, 000h, 000h, 0CDh, 03Fh, 001h, 000h
            db   000h, 000h, 0CDh, 03Fh, 001h, 016h, 001h, 000h, 0CDh, 03Fh, 00Ah, 0A8h, 002h, 000h, 0CDh, 03Fh
            db   006h, 0A4h, 000h, 000h, 0CDh, 03Fh, 006h, 000h, 000h, 000h, 0CDh, 03Fh, 00Bh, 042h, 000h, 000h
            db   0CDh, 03Fh, 00Bh, 010h, 000h, 000h, 0CDh, 03Fh, 00Bh, 04Eh, 003h, 000h, 01Eh
            db   'Windows file expansion library'
            db   000h, 000h, 006h
            db   'LZREAD'
            db   005h, 000h, 006h
            db   'LZCOPY'
            db   001h, 000h, 007h
            db   'LZCLOSE'
            db   006h, 000h, 006h
            db   'LZINIT'
            db   003h, 000h, 00Fh
            db   'GETEXPANDEDNAME'
            db   00Ah, 000h, 006h
            db   'LZSEEK'
            db   004h, 000h, 00Ah
            db   'LZOPENFILE'
            db   002h, 000h, 00Ah
            db   'COPYLZFILE'
            db   008h, 000h, 006h
            db   'LZDONE'
            db   009h, 000h, 007h
            db   'LZSTART'
            db   007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section seg1 start=seg1_start_file vstart=0 align=1
seg1_start:
s1_L0:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    mov ax, word [0x46]
    or ax, word [0x44]
    je short s1_L1c
s1_L16:
    mov ax, strict word 1
    jmp near s1_Laa
s1_L1c:
    push 0x2040
    push 0
    push 0x1080
s1_L24:
    db 0x9a                                 ; lcall 0, 0x58
s1_S25:
    dw s1_S58
    db 0x00,0x00
    mov word [0x44], strict word 0
    mov word [0x46], ax
    or ax, word [0x44]
    je short s1_La8
    mov ax, 0x8000
    mov word [0x27c], ax
    mov word [0x276], ax
s1_L41:
    cmp word [0x276], 0
    je short s1_L9f
    push 0x2040
    mov ax, word [0x27c]
    sub_ dx, dx
    add ax, strict word 1
    adc_ dx, dx
    push dx
    push ax
s1_L57:
    db 0x9a                                 ; lcall 0, 0x75
s1_S58:
    dw s1_S75
    db 0x00,0x00
    mov word [0x48], strict word 0
    mov word [0x4a], ax
    or ax, word [0x48]
    je short s1_L91
    push 0x2040
    push 0
    push word [0x276]
s1_L74:
    db 0x9a                                 ; lcall 0, 0xffff
s1_S75:
    dw 0xffff
    db 0x00,0x00
    mov word [0x54], strict word 0
    mov word [0x56], ax
    or ax, word [0x54]
    jne short s1_L16
    push word [0x4a]
s1_L8c:
    db 0x9a                                 ; lcall 0, 0xa4
s1_S8d:
    dw s1_Sa4
    db 0x00,0x00
s1_L91:
    sub word [0x276], 0x400
    sub word [0x27c], 0x400
    jne short s1_L41
s1_L9f:
    push word [0x46]
s1_La3:
    db 0x9a                                 ; lcall 0, 0xcd
s1_Sa4:
    dw s1_Scd
    db 0x00,0x00
s1_La8:
    sub_ ax, ax
s1_Laa:
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
s1_Lb2:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    mov ax, word [0x46]
    or ax, word [0x44]
    je short s1_Ld9
    push word [0x46]
s1_Lcc:
    db 0x9a                                 ; lcall 0, 0xe7
s1_Scd:
    dw s1_Se7
    db 0x00,0x00
    sub_ ax, ax
    mov word [0x46], ax
    mov word [0x44], ax
s1_Ld9:
    mov ax, word [0x4a]
    or ax, word [0x48]
    je short s1_Lf3
    push word [0x4a]
s1_Le6:
    db 0x9a                                 ; lcall 0, 0x101
s1_Se7:
    dw s1_S101
    db 0x00,0x00
    sub_ ax, ax
    mov word [0x4a], ax
    mov word [0x48], ax
s1_Lf3:
    mov ax, word [0x56]
    or ax, word [0x54]
    je short s1_L10d
    push word [0x56]
s1_L100:
    db 0x9a                                 ; lcall 0, 0xffff
s1_S101:
    dw 0xffff
    db 0x00,0x00
    sub_ ax, ax
    mov word [0x56], ax
    mov word [0x54], ax
s1_L10d:
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
s1_L116:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    push si
    mov cx, word [bp + 6]
    mov ax, word [bp + 8]
    or_ ax, cx
    jne short s1_L13a
    les bx, [bp + 0xc]
    mov word [es:bx], strict word 0xfffe
    jmp short s1_L1ab
            db   090h, 090h
s1_L13a:
    cmp word [bp + 0xa], 0
    jne short s1_L160
    mov si, word [bp + 0xc]
    mov ax, word [bp + 8]
    push ax
    push cx
    push 0
s1_L14a:
    db 0x9a                                 ; lcall 0, 0xffff
s1_S14b:
    dw 0xffff
    db 0x00,0x00
    mov es, word [bp + 0xe]
    mov word [es:si], ax
    cmp ax, strict word 0xffff
    jne short s1_L1ab
    mov ax, strict word 0xfffe
    jmp short s1_L1be
            db   090h
s1_L160:
    mov si, word [bp + 0xc]
    mov ax, word [bp + 8]
    push ax
    push cx
    push 0
s1_L16a:
    db 0x9a                                 ; lcall 0, 0xffff
s1_S16b:
    dw 0xffff
    db 0x00,0x00
    mov es, word [bp + 0xe]
    mov word [es:si], ax
    cmp ax, strict word 0xffff
    je short s1_L1bb
    mov es, word [bp + 0xe]
    push word [es:si]
    push 0
    push 0
    push 2
s1_L186:
    db 0x9a                                 ; lcall 0, 0x1a3
s1_S187:
    dw s1_S1a3
    db 0x00,0x00
    mov word [0x272], ax
    mov word [0x274], dx
    or_ dx, dx
    jl short s1_L1b0
    mov es, word [bp + 0xe]
    push word [es:si]
    push 0
    push 0
    push 0
s1_L1a2:
    db 0x9a                                 ; lcall 0, 0xffff
s1_S1a3:
    dw 0xffff
    db 0x00,0x00
    or_ dx, ax
    jne short s1_L1b0
s1_L1ab:
    mov ax, strict word 1
    jmp short s1_L1be
s1_L1b0:
    mov es, word [bp + 0xe]
    push word [es:si]
s1_L1b6:
    db 0x9a                                 ; lcall 0, 0xffff
s1_S1b7:
    dw 0xffff
    db 0x00,0x00
s1_L1bb:
    mov ax, strict word 0xffff
s1_L1be:
    pop si
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
seg1_end:
    dw 6                                   ; relocations
    db 0x03,0x01
    dw s1_S25
    db 0x01,0x00,0x0f,0x00
    db 0x03,0x01
    dw s1_S8d
    db 0x01,0x00,0x11,0x00
    db 0x03,0x01
    dw s1_S1b7
    db 0x01,0x00,0x51,0x00
    db 0x03,0x01
    dw s1_S14b
    db 0x01,0x00,0x53,0x00
    db 0x03,0x01
    dw s1_S187
    db 0x01,0x00,0x54,0x00
    db 0x03,0x01
    dw s1_S16b
    db 0x01,0x00,0x55,0x00

section part1 start=0x4fa vstart=0x4fa align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h

section seg12 start=seg12_start_file vstart=0 align=1
seg12_start:
            db   000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   053h, 05Ah, 044h, 044h, 088h, 0F0h, 027h, 033h, 000h, 053h, 05Ah, 044h, 044h, 088h, 0F0h, 027h
            db   033h, 000h, 05Ch, 000h, 05Fh, 000h, 02Eh, 05Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 001h, 000h, 016h, 002h, 002h, 018h, 00Dh, 009h, 00Ch, 00Ch, 00Ch, 007h, 008h, 016h, 016h
            db   0FFh, 002h, 00Dh, 012h, 002h, 0FFh
            db   '_C_FILE_INFO='
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 014h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0DFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 070h, 000h, 070h, 000h, 070h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h
            db   '<<NMSG>>C RUNTIME ERROR '
            db   000h, 000h, 000h
            db   'R6000'
            db   00Dh, 00Ah
            db   '- stack overflow'
            db   00Dh, 00Ah, 000h, 003h, 000h
            db   'R6003'
            db   00Dh, 00Ah
            db   '- integer divide by 0'
            db   00Dh, 00Ah, 000h, 009h, 000h
            db   'R6009'
            db   00Dh, 00Ah
            db   '- not enough space for environment'
            db   00Dh, 00Ah, 000h, 0FCh, 000h, 00Dh, 00Ah, 000h, 0FFh, 000h
            db   'run-time error '
            db   000h, 002h, 000h
            db   'R6002'
            db   00Dh, 00Ah
            db   '- floating-point support not loaded'
            db   00Dh, 00Ah, 000h, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
            db   000h, 000h, 000h, 000h, 000h
seg12_end:

section part2 start=0x7a4 vstart=0x7a4 align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section seg2 start=seg2_start_file vstart=0 align=1
seg2_start:
s2_L0:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0x1e
    push di
    push si
    mov di, word [bp + 6]
    push di
    lea ax, [bp - 0x1a]
    push ss
    push ax
s2_L18:
    db 0x9a                                 ; lcall 0, 0x1f3
s2_S19:
    dw s2_S1f3
    db 0x00,0x00
    add sp, 6
    mov_ si, ax
    cmp si, 1
    je short s2_L3e
    cmp word [0x274], 0
    jl short s2_L3e
    jg short s2_L37
    cmp word [0x272], 0xe
    jb short s2_L3e
s2_L37:
    mov ax, strict word 0xffff
    jmp near s2_L1af
            db   090h
s2_L3e:
    dec si
    je short s2_L44
    jmp near s2_L19a
s2_L44:
    lea ax, [bp - 0x1a]
    push ss
    push ax
s2_L49:
    db 0x9a                                 ; lcall 0, 0x236
s2_S4a:
    dw s2_S236
    db 0x00,0x00
    add sp, 4
    dec ax
    je short s2_L57
    jmp near s2_L19a
s2_L57:
    cmp byte [bp - 0x12], 0x41
    jne short s2_L62
    mov ax, strict word 1
    jmp short s2_L64
s2_L62:
    sub_ ax, ax
s2_L64:
    dec ax
    je short s2_L6e
    mov ax, strict word 0xfff8
    jmp near s2_L1af
            db   090h
s2_L6e:
    mov dx, word [0x30]
    mov word [bp - 4], dx
s2_L75:
    mov bx, word [0x30]
    shl bx, 1
    cmp word [word bx + 0x10], 0
    je short s2_La0
    inc word [0x30]
    cmp word [0x30], 0x10
    jl short s2_L93
    mov word [0x30], strict word 0
s2_L93:
    cmp word [0x30], dx
    jne short s2_L75
s2_L99:
    mov ax, strict word 0xfffb
    jmp near s2_L1af
            db   090h
s2_La0:
    mov ax, word [0x30]
    mov word [bp - 8], ax
    push 0x42
    push 0
    push 0x18a5
s2_Lad:
    db 0x9a                                 ; lcall 0, 0xffff
s2_Sae:
    dw 0xffff
    db 0x00,0x00
    mov_ di, ax
    or_ di, di
    je short s2_L99
    push di
s2_Lb9:
    db 0x9a                                 ; lcall 0, 0xffff
s2_Sba:
    dw 0xffff
    db 0x00,0x00
    mov_ si, ax
    mov word [bp - 4], dx
    or_ dx, ax
    jne short s2_Ld4
    push di
s2_Lc8:
    db 0x9a                                 ; lcall 0, 0xffff
s2_Sc9:
    dw 0xffff
    db 0x00,0x00
    mov ax, strict word 0xfffa
    jmp near s2_L1af
            db   090h
s2_Ld4:
    mov ax, word [bp + 6]
    mov es, word [bp - 4]
    mov word [es:si], ax
    mov al, byte [bp - 0x12]
    mov byte [es:si + 2], al
    mov word [es:si + 3], strict word 0
    mov ax, word [bp - 0x10]
    mov dx, word [bp - 0xe]
    mov word [es:si + 5], ax
    mov word [es:si + 7], dx
    mov ax, word [bp - 0xc]
    mov dx, word [bp - 0xa]
    mov word [es:si + 9], ax
    mov word [es:si + 0xb], dx
    sub_ ax, ax
    mov word [es:si + 0xf], ax
    mov word [es:si + 0xd], ax
    mov bx, word [bp - 8]
    shl bx, 1
    mov word [word bx + 0x10], di
    push es
    push si
    mov word [bp - 0x1e], si
    mov word [bp - 0x1c], es
s2_L121:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S122:
    dw 0xffff
    db 0x00,0x00
    add sp, 4
    mov ax, word [0x48]
    mov dx, word [0x4a]
    add ax, word [0x27c]
    inc ax
    mov word [0x4c], ax
    mov word [0x4e], dx
    mov word [0x50], ax
    mov word [0x52], dx
    mov word [0x27e], strict word 0
    mov ax, word [0x54]
    mov dx, word [0x56]
    add ax, word [0x276]
    mov word [0x58], ax
    mov word [0x5a], dx
    mov ax, word [0x54]
    mov word [0x5c], ax
    mov word [0x5e], dx
    sub_ ax, ax
    mov word [0x282], ax
    mov word [0x280], ax
    push word [bp - 0x1c]
    push word [bp - 0x1e]
s2_L173:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S174:
    dw 0xffff
    db 0x00,0x00
    add sp, 4
    push di
s2_L17c:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S17d:
    dw 0xffff
    db 0x00,0x00
    inc word [0x30]
    cmp word [0x30], 0x10
    jl short s2_L192
    mov word [0x30], strict word 0
s2_L192:
    mov ax, word [bp - 8]
    add ah, 4
    jmp short s2_L1af
s2_L19a:
    push di
    push 0
    push 0
    push 0
s2_L1a1:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S1a2:
    dw 0xffff
    db 0x00,0x00
    or_ dx, ax
    je short s2_L1ad
    jmp near s2_L37
s2_L1ad:
    mov_ ax, di
s2_L1af:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf 2
            db   090h
s2_L1bc:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0x9e
    push di
    push si
    push word [bp + 0xc]
    push word [bp + 0xa]
    lea ax, [bp - 0x9e]
    push ss
    push ax
    push 0
s2_L1da:
    db 0x9a                                 ; lcall 0, 0x28f
s2_S1db:
    dw s2_S28f
    db 0x00,0x00
    mov_ di, ax
    cmp di, -1
    jne short s2_L1ec
s2_L1e6:
    mov ax, strict word 0xffff
    jmp short s2_L255
            db   090h
s2_L1ec:
    push di
    lea ax, [bp - 0x16]
    push ss
    push ax
s2_L1f2:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S1f3:
    dw 0xffff
    db 0x00,0x00
    add sp, 6
    mov_ si, ax
    cmp si, 1
    je short s2_L211
    cmp word [0x274], 0
    jl short s2_L211
    jg short s2_L1e6
    cmp word [0x272], 0xe
    jae short s2_L1e6
s2_L211:
    mov word [bp - 4], di
    mov_ cx, di
    push cx
s2_L217:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S218:
    dw 0xffff
    db 0x00,0x00
    push word [bp + 8]
    push word [bp + 6]
    push word [bp + 0xc]
    push word [bp + 0xa]
s2_L228:
    db 0x9a                                 ; lcall 0, 0x27f
s2_S229:
    dw s2_S27f
    db 0x00,0x00
    dec si
    jne short s2_L252
    lea ax, [bp - 0x16]
    push ss
    push ax
s2_L235:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S236:
    dw 0xffff
    db 0x00,0x00
    add sp, 4
    dec ax
    jne short s2_L252
    mov al, byte [bp - 0xd]
    push ax
    push word [bp + 8]
    push word [bp + 6]
s2_L24a:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S24b:
    dw 0xffff
    db 0x00,0x00
    add sp, 6
s2_L252:
    mov ax, strict word 1
s2_L255:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf 8
            db   090h
s2_L262:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    push di
    push si
    mov si, word [bp + 8]
    push ds
    push 0x1e0
    push word [bp + 0xe]
    push word [bp + 0xc]
s2_L27e:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S27f:
    dw 0xffff
    db 0x00,0x00
    push ds
    push 0x1e0
    push word [bp + 0xa]
    push si
    push word [bp + 6]
s2_L28e:
    db 0x9a                                 ; lcall 0, 0x2bd
s2_S28f:
    dw s2_S2bd
    db 0x00,0x00
    mov_ di, ax
    cmp di, -1
    jne short s2_L2c3
    mov es, word [bp + 0xa]
    cmp word [es:si + 2], 2
    jne short s2_L2c3
    push ds
    push 0x1e0
s2_L2a8:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S2a9:
    dw 0xffff
    db 0x00,0x00
    add sp, 4
    push ds
    push 0x1e0
    mov ax, word [bp + 0xa]
    push ax
    push si
    push word [bp + 6]
s2_L2bc:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S2bd:
    dw 0xffff
    db 0x00,0x00
    mov_ di, ax
s2_L2c3:
    cmp di, -1
    je short s2_L2e2
    test word [bp + 6], 0xff0f
    jne short s2_L2e2
    push di
    push cs
    call 0
    mov_ si, ax
    or_ si, si
    jge short s2_L2e0
    push di
s2_L2db:
    db 0x9a                                 ; lcall 0, 0x218
s2_S2dc:
    dw s2_S218
    db 0x00,0x00
s2_L2e0:
    mov_ ax, si
s2_L2e2:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf 0xa
s2_L2ee:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0xc
    push di
    push si
    mov si, word [bp + 0xc]
    or_ si, si
    jge short s2_L307
    jmp near s2_L3c4
s2_L307:
    cmp si, 0x410
    jl short s2_L310
    jmp near s2_L3c4
s2_L310:
    cmp si, 0x400
    jge short s2_L328
    push si
    push word [bp + 0xa]
    push word [bp + 8]
    push word [bp + 6]
s2_L320:
    db 0x9a                                 ; lcall 0, 0x1a2
s2_S321:
    dw s2_S1a2
    db 0x00,0x00
    jmp near s2_L3ca
s2_L328:
    shl si, 1
    mov ax, word [si - 0x7f0]
    mov word [bp - 0xc], ax
    push ax
s2_L332:
    db 0x9a                                 ; lcall 0, 0x42d
s2_S333:
    dw s2_S42d
    db 0x00,0x00
    mov_ di, ax
    mov word [bp - 8], dx
    or_ dx, ax
    jne short s2_L348
    mov ax, strict word 0xfffa
    jmp near s2_L3c7
            db   090h, 090h
s2_L348:
    mov dx, word [bp + 6]
    or_ dx, dx
    jne short s2_L35a
    sub_ ax, ax
    mov word [bp - 4], ax
    mov word [bp - 6], ax
    jmp short s2_L382
            db   090h
s2_L35a:
    cmp dx, 1
    jne short s2_L36c
    mov es, word [bp - 8]
    mov ax, word [es:di + 0xd]
    mov dx, word [es:di + 0xf]
    jmp short s2_L37c
s2_L36c:
    cmp dx, 2
    jne short s2_L394
    mov es, word [bp - 8]
    mov ax, word [es:di + 5]
    mov dx, word [es:di + 7]
s2_L37c:
    mov word [bp - 6], ax
    mov word [bp - 4], dx
s2_L382:
    mov ax, word [bp + 8]
    mov dx, word [bp + 0xa]
    add word [bp - 6], ax
    adc word [bp - 4], dx
    cmp word [bp - 4], 0
    jge short s2_L3a2
s2_L394:
    mov si, word [bp - 0xc]
    push si
s2_L398:
    db 0x9a                                 ; lcall 0, 0x3b8
s2_S399:
    dw s2_S3b8
    db 0x00,0x00
    mov ax, strict word 0xfff9
    jmp short s2_L3c7
s2_L3a2:
    mov si, word [bp - 0xc]
    mov ax, word [bp - 6]
    mov dx, word [bp - 4]
    mov es, word [bp - 8]
    mov word [es:di + 0xd], ax
    mov word [es:di + 0xf], dx
    push si
s2_L3b7:
    db 0x9a                                 ; lcall 0, 0x17d
s2_S3b8:
    dw s2_S17d
    db 0x00,0x00
    mov ax, word [bp - 6]
    mov dx, word [bp - 4]
    jmp short s2_L3ca
s2_L3c4:
    mov ax, strict word 0xffff
s2_L3c7:
    mov dx, strict word 0xffff
s2_L3ca:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf 8
s2_L3d6:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0x22
    push di
    push si
    mov si, word [bp + 0xc]
    mov word [bp - 0x1e], strict word 0
    or_ si, si
    jge short s2_L3f4
    jmp near s2_L6e0
s2_L3f4:
    cmp si, 0x410
    jl short s2_L3fd
    jmp near s2_L6e0
s2_L3fd:
    cmp word [bp + 6], 0
    jge short s2_L40a
    mov ax, strict word 0xfff9
    jmp near s2_L6e3
            db   090h
s2_L40a:
    cmp si, 0x400
    jge short s2_L422
    push si
    push word [bp + 0xa]
    push word [bp + 8]
    push word [bp + 6]
s2_L41a:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S41b:
    dw 0xffff
    db 0x00,0x00
    jmp near s2_L6e3
s2_L422:
    shl si, 1
    mov ax, word [si - 0x7f0]
    mov word [bp - 0x1c], ax
    push ax
s2_L42c:
    db 0x9a                                 ; lcall 0, 0xba
s2_S42d:
    dw s2_Sba
    db 0x00,0x00
    mov_ di, ax
    mov word [bp - 0x14], dx
    or_ dx, ax
    jne short s2_L440
    mov ax, strict word 0xfffa
    jmp near s2_L6e3
s2_L440:
    mov es, word [bp - 0x14]
    test byte [es:di + 3], 1
    jne short s2_L456
    or byte [es:di + 3], 1
    mov word [bp - 0x12], strict word 1
    jmp short s2_L479
s2_L456:
    mov ax, word [es:di + 0x18a1]
    mov dx, word [es:di + 0x18a3]
    cmp word [es:di + 0xf], dx
    jg short s2_L474
    jl short s2_L46e
    cmp word [es:di + 0xd], ax
    jae short s2_L474
s2_L46e:
    mov ax, strict word 1
    jmp short s2_L476
            db   090h
s2_L474:
    sub_ ax, ax
s2_L476:
    mov word [bp - 0x12], ax
s2_L479:
    push word [bp - 0x14]
    push di
s2_L47d:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S47e:
    dw 0xffff
    db 0x00,0x00
    add sp, 4
    mov ax, word [bp + 8]
    mov dx, word [bp + 0xa]
    mov word [bp - 0x22], ax
    mov word [bp - 0x20], dx
    cmp word [bp + 6], 0
    jg short s2_L49a
    jmp near s2_L6ab
s2_L49a:
    mov dx, word [bp - 0x14]
    mov_ cx, di
    mov_ bx, dx
    add cx, 0xd
s2_L4a4:
    mov es, word [bp - 0x14]
    mov ax, word [es:di + 0xd]
    mov dx, word [es:di + 0xf]
    cmp word [es:di + 7], dx
    jge short s2_L4b8
    jmp near s2_L6ab
s2_L4b8:
    jg short s2_L4c3
    cmp word [es:di + 5], ax
    ja short s2_L4c3
    jmp near s2_L6ab
s2_L4c3:
    mov ax, word [0x5c]
    sub ax, word [0x54]
    mov word [bp - 0xc], ax
    mov ax, word [es:di + 0xd]
    mov dx, word [es:di + 0xf]
    sub ax, word [es:di + 0x18a1]
    sbb dx, word [es:di + 0x18a3]
    mov word [bp - 8], ax
    mov word [bp - 6], dx
    mov_ cx, ax
    mov ax, word [es:di + 0x189f]
    mov_ bx, dx
    db 0x99                                 ; cdq 
    cmp_ bx, dx
    jg short s2_L4fe
    jl short s2_L4f9
    cmp_ cx, ax
    jae short s2_L4fe
s2_L4f9:
    mov_ si, cx
    jmp short s2_L501
            db   090h
s2_L4fe:
    mov si, strict word 0xffff
s2_L501:
    or_ si, si
    jl short s2_L540
    mov word [bp - 6], strict word 0
    cmp word [bp - 0xc], si
    jle short s2_L52b
    mov word [bp - 0x16], di
    mov word [bp - 4], si
    mov si, word [bp - 0x22]
    mov di, word [bp - 4]
    mov cx, word [bp + 6]
s2_L51e:
    mov word [bp + 6], cx
    or_ cx, cx
    jg short s2_L592
s2_L525:
    mov word [bp - 0x22], si
    mov di, word [bp - 0x16]
s2_L52b:
    mov ax, word [bp - 6]
    db 0x99                                 ; cdq 
    mov es, word [bp - 0x14]
    add word [es:di + 0xd], ax
    adc word [es:di + 0xf], dx
    mov ax, word [bp - 6]
    add word [bp - 0x1e], ax
s2_L540:
    cmp word [bp + 6], 0
    jg short s2_L549
    jmp near s2_L6a2
s2_L549:
    mov es, word [bp - 0x14]
    mov ax, word [es:di + 0xd]
    mov dx, word [es:di + 0xf]
    cmp word [es:di + 7], dx
    jge short s2_L55d
    jmp near s2_L6a2
s2_L55d:
    jg short s2_L568
    cmp word [es:di + 5], ax
    ja short s2_L568
    jmp near s2_L6a2
s2_L568:
    cmp word [bp - 0x12], 0
    je short s2_L5ba
    mov ax, word [0x270]
    db 0x99                                 ; cdq 
    mov cx, word [es:di + 0xd]
    mov bx, word [es:di + 0xf]
    sub_ cx, ax
    sbb_ bx, dx
    mov word [bp - 0xa], cx
    mov word [bp - 8], bx
    or_ bx, bx
    jge short s2_L5e4
    sub_ ax, ax
    mov word [bp - 8], ax
    mov word [bp - 0xa], ax
    jmp short s2_L5e4
s2_L592:
    les bx, [bp - 0x16]
    add bx, 0x1493
    mov al, byte [es:bx + di]
    mov es, word [bp - 0x20]
    mov byte [es:si], al
    inc si
    mov ax, word [bp - 0x20]
    inc word [bp - 6]
    dec cx
    inc di
    cmp word [bp - 0xc], di
    jle short s2_L5b3
    jmp near s2_L51e
s2_L5b3:
    mov word [bp + 6], cx
    jmp near s2_L525
            db   090h
s2_L5ba:
    mov ax, word [es:di + 0x189f]
    db 0x99                                 ; cdq 
    add ax, word [es:di + 0x18a1]
    adc dx, word [es:di + 0x18a3]
    mov_ cx, ax
    mov ax, word [0x270]
    mov_ bx, dx
    db 0x99                                 ; cdq 
    sub_ cx, ax
    sbb_ bx, dx
    mov ax, word [bp - 0xc]
    db 0x99                                 ; cdq 
    add_ cx, ax
    adc_ bx, dx
    mov word [bp - 0xa], cx
    mov word [bp - 8], bx
s2_L5e4:
    mov ax, word [bp + 6]
    db 0x99                                 ; cdq 
    mov cx, word [es:di + 5]
    mov bx, word [es:di + 7]
    add ax, word [es:di + 0xd]
    adc dx, word [es:di + 0xf]
    sub cx, 1
    sbb bx, 0
    cmp_ dx, bx
    jl short s2_L60c
    jg short s2_L608
    cmp_ ax, cx
    jbe short s2_L60c
s2_L608:
    mov_ dx, bx
    mov_ ax, cx
s2_L60c:
    cmp dx, word [bp - 8]
    jl short s2_L61e
    jg short s2_L618
    cmp ax, word [bp - 0xa]
    jbe short s2_L61e
s2_L618:
    mov dx, word [bp - 8]
    mov ax, word [bp - 0xa]
s2_L61e:
    mov word [bp - 6], ax
    mov word [bp - 4], dx
    mov ax, word [0x54]
    mov dx, word [0x56]
    mov word [0x5c], ax
    mov word [0x5e], dx
    mov al, byte [es:di + 2]
    sub_ ah, ah
    sub ax, strict word 0x41
    je short s2_L642
    mov si, strict word 0xfff8
    jmp short s2_L65c
s2_L642:
    push 1
    push word [bp - 0x12]
    push word [bp - 4]
    push word [bp - 6]
    push -2
    push word [es:di]
s2_L652:
    db 0x9a                                 ; lcall 0, 0xffff
s2_S653:
    dw 0xffff
    db 0x00,0x00
    add sp, 0xc
    mov_ si, ax
s2_L65c:
    cmp si, 1
    jne short s2_L6d4
    mov bx, word [0x5c]
    sub bx, word [0x54]
    or_ bx, bx
    jle short s2_L6c6
    mov_ ax, bx
    db 0x99                                 ; cdq 
    mov cx, word [0x280]
    mov bx, word [0x282]
    sub_ cx, ax
    sbb_ bx, dx
    mov es, word [bp - 0x14]
    mov word [es:di + 0x18a1], cx
    mov word [es:di + 0x18a3], bx
    mov ax, word [bp - 6]
    mov dx, word [bp - 4]
    cmp word [0x282], dx
    jl short s2_L6c6
    jg short s2_L69d
    cmp word [0x280], ax
    jb short s2_L6c6
s2_L69d:
    mov word [bp - 0x12], strict word 0
s2_L6a2:
    cmp word [bp + 6], 0
    jle short s2_L6ab
    jmp near s2_L4a4
s2_L6ab:
    mov ax, word [bp - 0x14]
    push ax
    push di
s2_L6b0:
    db 0x9a                                 ; lcall 0, 0x174
s2_S6b1:
    dw s2_S174
    db 0x00,0x00
    add sp, 4
    push word [bp - 0x1c]
s2_L6bb:
    db 0x9a                                 ; lcall 0, 0x6ca
s2_S6bc:
    dw s2_S6ca
    db 0x00,0x00
    mov ax, word [bp - 0x1e]
    jmp short s2_L6e3
            db   090h
s2_L6c6:
    push word [bp - 0x1c]
s2_L6c9:
    db 0x9a                                 ; lcall 0, 0x6d8
s2_S6ca:
    dw s2_S6d8
    db 0x00,0x00
    mov ax, strict word 0xfffd
    jmp short s2_L6e3
            db   090h
s2_L6d4:
    push word [bp - 0x1c]
s2_L6d7:
    db 0x9a                                 ; lcall 0, 0x743
s2_S6d8:
    dw s2_S743
    db 0x00,0x00
    mov_ ax, si
    jmp short s2_L6e3
s2_L6e0:
    mov ax, strict word 0xffff
s2_L6e3:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf 8
            db   090h
s2_L6f0:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0xa
    push di
    push si
    mov si, word [bp + 6]
    or_ si, si
    jl short s2_L74d
    cmp si, 0x410
    jge short s2_L74d
    cmp si, 0x400
    jge short s2_L71a
    push si
s2_L713:
    db 0x9a                                 ; lcall 0, 0x73d
s2_S714:
    dw s2_S73d
    db 0x00,0x00
    jmp short s2_L74d
s2_L71a:
    shl si, 1
    mov di, word [si - 0x7f0]
    mov word [si - 0x7f0], strict word 0
    push di
s2_L727:
    db 0x9a                                 ; lcall 0, 0x333
s2_S728:
    dw s2_S333
    db 0x00,0x00
    mov word [bp - 0xa], ax
    mov word [bp - 8], dx
    or_ dx, ax
    je short s2_L74d
    les bx, [bp - 0xa]
    push word [es:bx]
s2_L73c:
    db 0x9a                                 ; lcall 0, 0x2dc
s2_S73d:
    dw s2_S2dc
    db 0x00,0x00
    push di
s2_L742:
    db 0x9a                                 ; lcall 0, 0x399
s2_S743:
    dw s2_S399
    db 0x00,0x00
    push di
s2_L748:
    db 0x9a                                 ; lcall 0, 0xc9
s2_S749:
    dw s2_Sc9
    db 0x00,0x00
s2_L74d:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf 2
            db   090h
seg2_end:
    dw 17                                   ; relocations
    db 0x03,0x00
    dw s2_S2a9
    db 0xff,0x00,0x0d,0x00
    db 0x03,0x00
    dw s2_S24b
    db 0xff,0x00,0x0e,0x00
    db 0x03,0x00
    dw s2_S4a
    db 0xff,0x00,0x0f,0x00
    db 0x03,0x00
    dw s2_S19
    db 0xff,0x00,0x10,0x00
    db 0x03,0x00
    dw s2_S6b1
    db 0xff,0x00,0x11,0x00
    db 0x03,0x00
    dw s2_S122
    db 0xff,0x00,0x12,0x00
    db 0x03,0x00
    dw s2_S47e
    db 0xff,0x00,0x13,0x00
    db 0x03,0x00
    dw s2_S653
    db 0xff,0x00,0x14,0x00
    db 0x03,0x01
    dw s2_Sae
    db 0x01,0x00,0x0f,0x00
    db 0x03,0x01
    dw s2_S749
    db 0x01,0x00,0x11,0x00
    db 0x03,0x01
    dw s2_S728
    db 0x01,0x00,0x12,0x00
    db 0x03,0x01
    dw s2_S6bc
    db 0x01,0x00,0x13,0x00
    db 0x03,0x01
    dw s2_S1db
    db 0x01,0x00,0x4a,0x00
    db 0x03,0x01
    dw s2_S714
    db 0x01,0x00,0x51,0x00
    db 0x03,0x01
    dw s2_S41b
    db 0x01,0x00,0x52,0x00
    db 0x03,0x01
    dw s2_S321
    db 0x01,0x00,0x54,0x00
    db 0x03,0x01
    dw s2_S229
    db 0x01,0x00,0x58,0x00

section part3 start=0xfc4 vstart=0xfc4 align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section seg3 start=seg3_start_file vstart=0 align=1
seg3_start:
s3_L0:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf 2
            db   090h
seg3_end:

section part4 start=0xfe8 vstart=0xfe8 align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section seg4 start=seg4_start_file vstart=0 align=1
seg4_start:
s4_L0:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    mov ax, word [0x34]
    mov dx, word [0x36]
    mov word [0x44], ax
    mov word [0x46], dx
    mov ax, word [0x38]
    mov dx, word [0x3a]
    mov word [0x48], ax
    mov word [0x4a], dx
    mov ax, word [0x3c]
    mov dx, word [0x3e]
    mov word [0x54], ax
    mov word [0x56], dx
    mov ax, word [0x40]
    mov word [0x27c], ax
    mov ax, word [0x42]
    mov word [0x276], ax
    cmp word [0x32], 0
    jne short s4_L8e
s4_L4a:
    db 0x9a                                 ; lcall 0, 0xffff
s4_S4b:
    dw 0xffff
    db 0x00,0x00
    dec ax
    je short s4_L58
    mov ax, strict word 0xfffb
    jmp short s4_L9d
            db   090h
s4_L58:
    mov ax, word [0x44]
    mov dx, word [0x46]
    mov word [0x34], ax
    mov word [0x36], dx
    mov ax, word [0x48]
    mov dx, word [0x4a]
    mov word [0x38], ax
    mov word [0x3a], dx
    mov ax, word [0x54]
    mov dx, word [0x56]
    mov word [0x3c], ax
    mov word [0x3e], dx
    mov ax, word [0x27c]
    mov word [0x40], ax
    mov ax, word [0x276]
    mov word [0x42], ax
s4_L8e:
    inc word [0x32]
    jne short s4_L9a
    mov word [0x32], strict word 0xffff
s4_L9a:
    mov ax, strict word 1
s4_L9d:
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
s4_La6:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    cmp word [0x32], 0
    je short s4_L131
    dec word [0x32]
    jne short s4_L131
    mov ax, word [0x34]
    mov dx, word [0x36]
    mov word [0x44], ax
    mov word [0x46], dx
    mov ax, word [0x38]
    mov dx, word [0x3a]
    mov word [0x48], ax
    mov word [0x4a], dx
    mov ax, word [0x3c]
    mov dx, word [0x3e]
    mov word [0x54], ax
    mov word [0x56], dx
    mov ax, word [0x40]
    mov word [0x27c], ax
    mov ax, word [0x42]
    mov word [0x276], ax
s4_Lf6:
    db 0x9a                                 ; lcall 0, 0xffff
s4_Sf7:
    dw 0xffff
    db 0x00,0x00
    mov ax, word [0x44]
    mov dx, word [0x46]
    mov word [0x34], ax
    mov word [0x36], dx
    mov ax, word [0x48]
    mov dx, word [0x4a]
    mov word [0x38], ax
    mov word [0x3a], dx
    mov ax, word [0x54]
    mov dx, word [0x56]
    mov word [0x3c], ax
    mov word [0x3e], dx
    mov ax, word [0x27c]
    mov word [0x40], ax
    mov ax, word [0x276]
    mov word [0x42], ax
s4_L131:
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
s4_L13a:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    push word [bp + 8]
    push word [bp + 6]
    push cs
    call s4_L15c
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf 4
            db   090h
s4_L15c:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0xa
    push di
    push si
    push cs
    call 0
    mov_ si, ax
    cmp si, 1
    je short s4_L17a
    db 0x99                                 ; cdq 
    jmp short s4_L1eb
            db   090h
s4_L17a:
    mov si, word [bp + 8]
    cmp si, 0x400
    jl short s4_L1be
    shl si, 1
    mov di, word [si - 0x7f0]
    or_ di, di
    jne short s4_L19a
    mov word [bp - 6], strict word 0xffff
s4_L192:
    mov word [bp - 4], strict word 0xffff
    jmp short s4_L1e1
            db   090h
s4_L19a:
    push di
s4_L19b:
    db 0x9a                                 ; lcall 0, 0xffff
s4_S19c:
    dw 0xffff
    db 0x00,0x00
    mov_ si, ax
    mov word [bp - 8], dx
    or_ dx, ax
    jne short s4_L1b0
    mov word [bp - 6], strict word 0xfffa
    jmp short s4_L192
s4_L1b0:
    mov_ bx, si
    mov es, word [bp - 8]
    mov si, word [es:bx]
    push di
s4_L1b9:
    db 0x9a                                 ; lcall 0, 0xffff
s4_S1ba:
    dw 0xffff
    db 0x00,0x00
s4_L1be:
    push word [bp + 6]
    push si
s4_L1c2:
    db 0x9a                                 ; lcall 0, 0xffff
s4_S1c3:
    dw 0xffff
    db 0x00,0x00
    add sp, 4
    mov_ di, ax
    cmp di, 1
    je short s4_L1d4
    db 0x99                                 ; cdq 
    jmp short s4_L1db
s4_L1d4:
    mov ax, word [0x280]
    mov dx, word [0x282]
s4_L1db:
    mov word [bp - 6], ax
    mov word [bp - 4], dx
s4_L1e1:
    push cs
    call s4_La6
    mov ax, word [bp - 6]
    mov dx, word [bp - 4]
s4_L1eb:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf 4
            db   090h
seg4_end:
    dw 5                                   ; relocations
    db 0x03,0x00
    dw s4_S1c3
    db 0xff,0x00,0x15,0x00
    db 0x03,0x00
    dw s4_Sf7
    db 0xff,0x00,0x16,0x00
    db 0x03,0x00
    dw s4_S4b
    db 0xff,0x00,0x17,0x00
    db 0x03,0x01
    dw s4_S19c
    db 0x01,0x00,0x12,0x00
    db 0x03,0x01
    dw s4_S1ba
    db 0x01,0x00,0x13,0x00

section part5 start=0x1212 vstart=0x1212 align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section seg5 start=seg5_start_file vstart=0 align=1
seg5_start:
s5_L0:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 6
    mov bx, word [bp + 6]
    mov cx, word [bp + 8]
    mov word [bp - 6], bx
    mov word [bp - 4], cx
    add bx, 0x11
    mov word [0x48], bx
    mov word [0x4a], cx
    mov ax, word [bp - 6]
    mov_ dx, cx
    add ax, 0x1493
    mov word [0x54], ax
    mov word [0x56], dx
    mov ax, word [bp - 6]
    add ax, 0x413
    mov word [0x44], ax
    mov word [0x46], dx
    mov ax, 0x400
    mov word [0x27c], ax
    mov word [0x276], ax
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
s5_L52:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    push si
    mov bx, word [bp + 6]
    mov ax, word [0x50]
    sub ax, word [0x48]
    lea bx, [bx + 0x1893]
    mov es, word [bp + 8]
    mov word [es:bx + 4], ax
    mov ax, word [0x4c]
    sub ax, word [0x48]
    mov word [es:bx + 6], ax
    mov ax, word [0x27e]
    mov word [es:bx + 8], ax
    mov ax, word [0x5c]
    sub ax, word [0x54]
    mov word [es:bx + 0xa], ax
    mov ax, word [0x58]
    sub ax, word [0x54]
    mov word [es:bx + 0xc], ax
    mov ax, word [0x280]
    mov dx, word [0x282]
    mov cx, word [0x5c]
    sub cx, word [0x54]
    sbb_ si, si
    sub_ ax, cx
    sbb_ dx, si
    mov word [es:bx + 0xe], ax
    mov word [es:bx + 0x10], dx
    mov ax, word [0x27a]
    mov word [es:bx], ax
    mov ax, word [0x278]
    mov word [es:bx + 2], ax
    pop si
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
s5_Ld0:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    push di
    push si
    mov si, word [bp + 6]
    mov ax, word [bp + 8]
    push ax
    push si
    mov_ di, ax
    push cs
    call 0
    add sp, 4
    mov es, di
    lea bx, [si + 0x1893]
    mov ax, word [es:bx + 4]
    add ax, word [0x48]
    mov dx, word [0x4a]
    mov word [0x50], ax
    mov word [0x52], dx
    mov ax, word [es:bx + 6]
    add ax, word [0x48]
    mov word [0x4c], ax
    mov word [0x4e], dx
    mov ax, word [es:bx + 8]
    mov word [0x27e], ax
    mov ax, word [es:bx + 0xa]
    add ax, word [0x54]
    mov dx, word [0x56]
    mov word [0x5c], ax
    mov word [0x5e], dx
    mov ax, word [es:bx + 0xc]
    add ax, word [0x54]
    mov word [0x58], ax
    mov word [0x5a], dx
    mov_ ax, di
    mov cx, word [es:si + 9]
    mov dx, word [es:si + 0xb]
    mov word [0x272], cx
    mov word [0x274], dx
    mov cx, word [es:si + 0x18a1]
    mov dx, word [es:si + 0x18a3]
    mov_ si, ax
    mov ax, word [es:bx + 0xa]
    mov_ di, dx
    db 0x99                                 ; cdq 
    add_ cx, ax
    adc_ di, dx
    mov word [0x280], cx
    mov word [0x282], di
    mov es, si
    mov ax, word [es:bx]
    mov word [0x27a], ax
    mov ax, word [es:bx + 2]
    mov word [0x278], ax
    mov word [0x270], strict word 0x10
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
seg5_end:

section part6 start=0x13b2 vstart=0x13b2 align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section seg6 start=seg6_start_file vstart=0 align=1
seg6_start:
s6_L0:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    push si
    les bx, [0x4c]
    mov al, byte [es:bx - 1]
    les bx, [0x48]
    mov byte [es:bx], al
    push word [bp + 0xa]
    mov ax, word [0x48]
    mov dx, word [0x4a]
    inc ax
    push dx
    push ax
    push word [0x27c]
s6_L2e:
    db 0x9a                                 ; lcall 0, 0xffff
s6_S2f:
    dw 0xffff
    db 0x00,0x00
    mov_ si, ax
    cmp word [0x27c], si
    je short s6_L55
    cmp si, -1
    je short s6_L9a
    or_ si, si
    je short s6_L80
    mov ax, word [0x48]
    mov dx, word [0x4a]
    add_ ax, si
    inc ax
    mov word [0x4c], ax
    mov word [0x4e], dx
s6_L55:
    mov ax, word [0x48]
    mov dx, word [0x4a]
    inc ax
    mov word [0x50], ax
    mov word [0x52], dx
    cmp word [0x27e], 0
    je short s6_L86
    les bx, [0x48]
    mov al, byte [es:bx]
    les bx, [bp + 6]
    mov byte [es:bx], al
    mov word [0x27e], strict word 0
    jmp short s6_L97
s6_L80:
    mov ax, 0x1f4
    jmp short s6_L9a
            db   090h
s6_L86:
    les bx, [0x50]
    mov al, byte [es:bx]
    les bx, [bp + 6]
    mov byte [es:bx], al
    inc word [0x50]
s6_L97:
    mov ax, strict word 1
s6_L9a:
    pop si
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
s6_La4:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    push di
    push si
    mov cx, word [bp + 8]
    mov si, word [0x5c]
    sub si, word [0x54]
    mov ax, word [0x54]
    mov dx, word [0x56]
    mov word [0x5c], ax
    mov word [0x5e], dx
    cmp cx, -2
    je short s6_Lf0
    push cx
    push dx
    push ax
    push si
s6_Ld5:
    db 0x9a                                 ; lcall 0, 0xffff
s6_Sd6:
    dw 0xffff
    db 0x00,0x00
    mov_ di, ax
    cmp_ di, si
    je short s6_Lf0
    cmp di, -1
    jne short s6_Lea
    mov ax, strict word 0xfffe
    jmp short s6_L101
s6_Lea:
    mov ax, strict word 0xfffc
    jmp short s6_L101
            db   090h
s6_Lf0:
    mov al, byte [bp + 6]
    les bx, [0x5c]
    mov byte [es:bx], al
    inc word [0x5c]
    mov ax, strict word 1
s6_L101:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
seg6_end:
    dw 2                                   ; relocations
    db 0x03,0x01
    dw s6_S2f
    db 0x01,0x00,0x52,0x00
    db 0x03,0x01
    dw s6_Sd6
    db 0x01,0x00,0x56,0x00

section part7 start=0x14de vstart=0x14de align=1
            db   000h, 000h

section seg7 start=seg7_start_file vstart=0 align=1
seg7_start:
            db   01Eh, 058h, 090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 083h, 0ECh, 002h, 057h, 056h, 0FFh
            db   076h, 006h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 09Ah
s7_D19:
    dw s7_D38
            db   000h, 000h
            db   00Bh, 0D0h, 074h, 007h, 0B8h, 0FFh, 0FFh, 0E9h, 0C2h, 000h, 090h, 08Bh, 076h, 008h, 083h, 0FEh
            db   0FEh, 074h, 016h, 056h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 09Ah
s7_D38:
    dw 0xffff
            db   000h, 000h
            db   00Bh, 0D0h, 074h, 006h, 0B8h, 0FEh, 0FFh, 0E9h, 0A3h, 000h, 0A1h, 048h, 000h, 08Bh, 016h, 04Ah
            db   000h, 003h, 006h, 07Ch, 002h, 040h, 0A3h, 04Ch, 000h, 089h, 016h, 04Eh, 000h, 0A3h, 050h, 000h
            db   089h, 016h, 052h, 000h, 0C7h, 006h, 07Eh, 002h, 000h, 000h, 0A1h, 054h, 000h, 08Bh, 016h, 056h
            db   000h, 003h, 006h, 076h, 002h, 0A3h, 058h, 000h, 089h, 016h, 05Ah, 000h, 0A1h, 054h, 000h, 0A3h
            db   05Ch, 000h, 089h, 016h, 05Eh, 000h, 02Bh, 0C0h, 0A3h, 082h, 002h, 0A3h, 080h, 002h, 0FFh, 076h
            db   006h, 0FFh, 036h, 04Ah, 000h, 0FFh, 036h, 048h, 000h, 0FFh, 036h, 07Ch, 002h, 09Ah
s7_D9a:
    dw 0xffff
            db   000h, 000h
            db   08Bh, 0F0h, 00Bh, 0F6h, 074h, 02Ch, 083h, 0FEh, 0FFh, 074h, 027h, 0FFh, 076h, 008h, 0FFh, 036h
            db   04Ah, 000h, 0FFh, 036h, 048h, 000h, 056h, 09Ah
s7_Db6:
    dw 0xffff
            db   000h, 000h
            db   08Bh, 0F8h, 03Bh, 0FEh, 075h, 018h, 02Bh, 0C9h, 001h, 03Eh, 080h, 002h, 011h, 00Eh, 082h, 002h
            db   039h, 036h, 07Ch, 002h, 074h, 0BAh, 083h, 0FEh, 0FFh, 075h, 011h, 0E9h, 049h, 0FFh, 083h, 0FFh
            db   0FFh, 074h, 003h, 0E9h, 060h, 0FFh, 0B8h, 0FCh, 0FFh, 0EBh, 004h, 090h, 0B8h, 001h, 000h, 05Eh
            db   05Fh, 04Dh, 04Dh, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CBh, 090h
s7_Lf4:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0x16
    push di
    push si
    mov si, word [bp + 6]
    push si
    lea ax, [bp - 0x16]
    push ss
    push ax
s7_L10c:
    db 0x9a                                 ; lcall 0, 0x219
s7_S10d:
    dw s7_D219
    db 0x00,0x00
    add sp, 6
    dec ax
    je short s7_L12e
    cmp word [0x274], 0
    jl short s7_L12e
    jg short s7_L127
    cmp word [0x272], 0xe
    jb short s7_L12e
s7_L127:
    mov ax, strict word 0xffff
    jmp near s7_L1d6
            db   090h
s7_L12e:
    lea ax, [bp - 0x16]
    push ss
    push ax
s7_L133:
    db 0x9a                                 ; lcall 0, 0x271
s7_S134:
    dw s7_D271
    db 0x00,0x00
    add sp, 4
    dec ax
    jne short s7_L144
    mov ax, strict word 1
    jmp short s7_L146
            db   090h
s7_L144:
    sub_ ax, ax
s7_L146:
    mov word [bp - 4], ax
    or_ ax, ax
    je short s7_L17c
    mov al, byte [bp - 0xe]
    sub_ ah, ah
    sub ax, strict word 0x41
    je short s7_L15c
    mov di, strict word 0xfff8
    jmp short s7_L189
s7_L15c:
    push 1
    push 1
    mov ax, word [bp - 0xc]
    mov dx, word [bp - 0xa]
    sub ax, strict word 1
    sbb dx, 0
    push dx
    push ax
    push word [bp + 8]
    push si
s7_L172:
    db 0x9a                                 ; lcall 0, 0xffff
s7_S173:
    dw 0xffff
    db 0x00,0x00
    add sp, 0xc
    jmp short s7_L187
s7_L17c:
    push word [bp + 8]
    push si
    push cs
    call 0
    add sp, 4
s7_L187:
    mov_ di, ax
s7_L189:
    cmp di, 1
    jne short s7_L1d4
    push word [bp + 8]
    push 0x46
s7_L193:
    db 0x9a                                 ; lcall 0, 0xffff
s7_S194:
    dw 0xffff
    db 0x00,0x00
    add sp, 4
    mov_ si, ax
    cmp si, 1
    jne short s7_L1d6
    push word [bp + 8]
    push word [bp + 6]
s7_L1a8:
    db 0x9a                                 ; lcall 0, 0xffff
s7_S1a9:
    dw 0xffff
    db 0x00,0x00
    add sp, 4
    mov_ di, ax
    cmp di, 1
    jne short s7_L1d4
    cmp word [bp - 4], 0
    je short s7_L1d4
    mov ax, word [bp - 0xc]
    mov dx, word [bp - 0xa]
    cmp word [0x280], ax
    jne short s7_L1cf
    cmp word [0x282], dx
    je short s7_L1d4
s7_L1cf:
    mov ax, strict word 0xfffd
    jmp short s7_L1d6
s7_L1d4:
    mov_ ax, di
s7_L1d6:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   01Eh, 058h, 090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 081h, 0ECh, 098h, 000h, 057h, 056h
            db   08Bh, 07Eh, 00Ah, 08Dh, 046h, 0FCh, 016h, 050h, 06Ah, 001h, 0FFh, 076h, 00Ch, 057h, 09Ah
s7_D1ff:
    dw s7_D2b0
            db   000h, 000h
            db   083h, 0C4h, 00Ah, 08Bh, 0F0h, 083h, 0FEh, 001h, 074h, 003h, 0E9h, 0D3h, 000h, 0FFh, 076h, 0FCh
            db   08Dh, 046h, 0E8h, 016h, 050h, 09Ah
s7_D219:
    dw 0xffff
            db   000h, 000h
            db   083h, 0C4h, 006h, 048h, 074h, 01Fh, 083h, 03Eh, 074h, 002h, 000h, 07Ch, 018h, 07Fh, 007h, 083h
            db   03Eh, 072h, 002h, 00Eh, 072h, 00Fh, 0FFh, 076h, 0FCh, 09Ah
s7_D237:
    dw s7_D29a
            db   000h, 000h
            db   0B8h, 0FFh, 0FFh, 0E9h, 0A4h, 000h, 090h, 08Dh, 086h, 068h, 0FFh, 016h, 050h, 0FFh, 076h, 010h
            db   0FFh, 076h, 00Eh, 09Ah
s7_D24f:
    dw 0xffff
            db   000h, 000h
            db   083h, 07Eh, 012h, 001h, 075h, 012h, 08Ah, 046h, 0F1h, 050h, 08Dh, 086h, 068h, 0FFh, 016h, 050h
            db   09Ah
s7_D264:
    dw 0xffff
            db   000h, 000h
            db   083h, 0C4h, 006h, 08Dh, 046h, 0E8h, 016h, 050h, 09Ah
s7_D271:
    dw 0xffff
            db   000h, 000h
            db   083h, 0C4h, 004h, 03Dh, 001h, 000h, 01Bh, 0C0h, 025h, 001h, 000h, 040h, 050h, 08Dh, 086h, 068h
            db   0FFh, 016h, 050h, 0FFh, 076h, 00Ch, 057h, 0FFh, 05Eh, 006h, 083h, 0C4h, 00Ah, 00Bh, 0C0h, 075h
            db   00Ch, 0FFh, 076h, 0FCh, 09Ah
s7_D29a:
    dw s7_D2d7
            db   000h, 000h
            db   02Bh, 0C0h, 0EBh, 043h, 08Dh, 046h, 0FAh, 016h, 050h, 06Ah, 000h, 08Dh, 086h, 068h, 0FFh, 016h
            db   050h, 09Ah
s7_D2b0:
    dw 0xffff
            db   000h, 000h
            db   083h, 0C4h, 00Ah, 08Bh, 0F0h, 083h, 0FEh, 001h, 074h, 006h, 0FFh, 076h, 0FCh, 0EBh, 01Bh, 090h
            db   0FFh, 076h, 0FAh, 0FFh, 076h, 0FCh, 00Eh, 0E8h, 026h, 0FEh, 083h, 0C4h, 004h, 08Bh, 0F0h, 0FFh
            db   076h, 0FCh, 09Ah
s7_D2d7:
    dw s7_D2df
            db   000h, 000h
            db   0FFh, 076h, 0FAh, 09Ah
s7_D2df:
    dw 0xffff
            db   000h, 000h
            db   08Bh, 0C6h, 05Eh, 05Fh, 04Dh, 04Dh, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CBh, 090h
seg7_end:
    dw 12                                   ; relocations
    db 0x03,0x00
    dw s7_D264
    db 0xff,0x00,0x0e,0x00
    db 0x03,0x00
    dw s7_S134
    db 0xff,0x00,0x0f,0x00
    db 0x03,0x00
    dw s7_S10d
    db 0xff,0x00,0x10,0x00
    db 0x03,0x00
    dw s7_S173
    db 0xff,0x00,0x14,0x00
    db 0x03,0x00
    dw s7_D1ff
    db 0xff,0x00,0x18,0x00
    db 0x03,0x00
    dw s7_S1a9
    db 0xff,0x00,0x19,0x00
    db 0x03,0x00
    dw s7_S194
    db 0xff,0x00,0x1a,0x00
    db 0x03,0x01
    dw s7_D237
    db 0x01,0x00,0x51,0x00
    db 0x03,0x01
    dw s7_D9a
    db 0x01,0x00,0x52,0x00
    db 0x03,0x01
    dw s7_D19
    db 0x01,0x00,0x54,0x00
    db 0x03,0x01
    dw s7_Db6
    db 0x01,0x00,0x56,0x00
    db 0x03,0x01
    dw s7_D24f
    db 0x01,0x00,0x58,0x00

section part8 start=0x1832 vstart=0x1832 align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section seg8 start=seg8_start_file vstart=0 align=1
seg8_start:
            db   01Eh, 058h, 090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 083h, 0ECh, 014h, 057h, 056h, 02Bh
            db   0F6h, 08Bh, 07Eh, 006h, 08Eh, 046h, 008h, 08Bh, 0DFh, 003h, 0DEh, 026h, 08Ah, 007h, 088h, 042h
            db   0ECh, 046h, 083h, 0FEh, 008h, 07Ch, 0F0h, 026h, 08Ah, 045h, 008h, 088h, 042h, 0ECh, 026h, 08Ah
            db   045h, 009h, 046h, 088h, 042h, 0ECh, 046h, 0C6h, 046h, 0FDh, 000h, 0B9h, 004h, 000h, 089h, 04Eh
            db   0FAh, 08Bh, 0F9h, 08Bh, 05Eh, 006h, 026h, 08Bh, 047h, 00Ah, 026h, 08Bh, 057h, 00Ch, 08Ah, 04Eh
            db   0FDh, 00Ah, 0C9h, 074h, 008h, 0D1h, 0EAh, 0D1h, 0D8h, 0FEh, 0C9h, 075h, 0F8h, 088h, 042h, 0ECh
            db   046h, 08Ah, 046h, 0FDh, 004h, 008h, 088h, 046h, 0FDh, 04Fh, 075h, 0D7h, 0FFh, 076h, 00Ah, 08Dh
            db   046h, 0ECh, 016h, 050h, 06Ah, 00Eh, 09Ah
s8_D77:
    dw 0xffff
            db   000h, 000h
            db   08Bh, 0F0h, 083h, 0FEh, 00Eh, 074h, 010h, 083h, 0FEh, 0FFh, 075h, 005h, 0B8h, 0FEh, 0FFh, 0EBh
            db   013h, 0B8h, 0FCh, 0FFh, 0EBh, 00Eh, 090h, 02Bh, 0C9h, 001h, 036h, 080h, 002h, 011h, 00Eh, 082h
            db   002h, 0B8h, 001h, 000h, 05Eh, 05Fh, 04Dh, 04Dh, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CBh, 090h
s8_Laa:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0x14
    push di
    push si
    mov si, word [bp + 0xa]
    push si
    push 0
    push 0
    push 2
s8_Lc3:
    db 0x9a                                 ; lcall 0, 0xde
s8_Sc4:
    dw s8_Sde
    db 0x00,0x00
    mov word [0x272], ax
    mov word [0x274], dx
    or_ dx, dx
    jge short s8_Ld6
    jmp near s8_L194
s8_Ld6:
    push si
    push 0
    push 0
    push 0
s8_Ldd:
    db 0x9a                                 ; lcall 0, 0xffff
s8_Sde:
    dw 0xffff
    db 0x00,0x00
    or_ dx, ax
    je short s8_Le9
    jmp near s8_L194
s8_Le9:
    push si
    lea ax, [bp - 0x14]
    push ss
    push ax
    push 0xe
s8_Lf1:
    db 0x9a                                 ; lcall 0, 0xffff
s8_Sf2:
    dw 0xffff
    db 0x00,0x00
    mov_ di, ax
    cmp di, 0xe
    je short s8_L10c
    cmp di, -1
    jne short s8_L105
    jmp near s8_L194
s8_L105:
    mov ax, strict word 0xfffd
    jmp near s8_L197
            db   090h
s8_L10c:
    sub_ si, si
    mov di, word [bp + 6]
    mov es, word [bp + 8]
s8_L114:
    mov al, byte [bp + si - 0x14]
    mov_ bx, di
    add_ bx, si
    mov byte [es:bx], al
    inc si
    cmp si, 8
    jl short s8_L114
    mov al, byte [bp + si - 0x14]
    mov es, word [bp + 8]
    mov byte [es:di + 8], al
    inc si
    mov al, byte [bp + si - 0x14]
    mov byte [es:di + 9], al
    inc si
    sub_ ax, ax
    mov word [es:di + 0xc], ax
    mov word [es:di + 0xa], ax
    mov byte [bp - 3], 0
    mov cx, strict word 4
    mov word [bp - 6], cx
    mov_ di, cx
    mov es, word [bp + 8]
s8_L150:
    mov bx, word [bp + 6]
    mov al, byte [bp + si - 0x14]
    sub_ ah, ah
    sub_ dx, dx
    mov cl, byte [bp - 3]
    or_ cl, cl
    je short s8_L169
s8_L161:
    shl ax, 1
    rcl dx, 1
    dec cl
    jne short s8_L161
s8_L169:
    or word [es:bx + 0xa], ax
    or word [es:bx + 0xc], dx
    inc si
    mov al, byte [bp - 3]
    add al, 8
    mov byte [bp - 3], al
    dec di
    jne short s8_L150
    mov ax, word [0x272]
    mov dx, word [0x274]
    mov bx, word [bp + 6]
    mov word [es:bx + 0xe], ax
    mov word [es:bx + 0x10], dx
    mov ax, strict word 1
    jmp short s8_L197
s8_L194:
    mov ax, strict word 0xffff
s8_L197:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
s8_L1a2:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0xc
    push di
    push si
    sub_ si, si
    mov di, word [bp + 6]
    mov es, word [bp + 8]
s8_L1b9:
    mov_ bx, di
    add_ bx, si
    mov al, byte [es:bx]
    mov byte [bp + si - 0xc], al
    inc si
    cmp si, 8
    jl short s8_L1b9
    mov byte [bp + si - 0xc], 0
    lea ax, [bp - 0xc]
    push ss
    push ax
    push ds
    push strict word 0x60
s8_L1d6:
    db 0x9a                                 ; lcall 0, 0xffff
s8_S1d7:
    dw 0xffff
    db 0x00,0x00
    cmp ax, strict word 1
    sbb_ ax, ax
    neg ax
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   01Eh, 058h, 090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 083h, 0ECh, 002h, 057h, 056h, 02Bh
            db   0F6h, 08Bh, 07Eh, 006h, 08Eh, 046h, 008h, 08Ah, 084h, 069h, 000h, 08Bh, 0DFh, 003h, 0DEh, 026h
            db   088h, 007h, 046h, 083h, 0FEh, 008h, 07Ch, 0EFh, 08Ah, 046h, 00Ah, 08Eh, 046h, 008h, 026h, 088h
            db   045h, 008h, 08Ah, 046h, 00Ch, 026h, 088h, 045h, 009h, 0A1h, 072h, 002h, 08Bh, 016h, 074h, 002h
            db   026h, 089h, 045h, 00Ah, 026h, 089h, 055h, 00Ch, 02Bh, 0C0h, 026h, 089h, 045h, 010h, 026h, 089h
            db   045h, 00Eh, 05Eh, 05Fh, 04Dh, 04Dh, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CBh
seg8_end:
    dw 4                                   ; relocations
    db 0x03,0x01
    dw s8_S1d7
    db 0x02,0x00,0xae,0x01
    db 0x03,0x01
    dw s8_Sf2
    db 0x01,0x00,0x52,0x00
    db 0x03,0x01
    dw s8_Sc4
    db 0x01,0x00,0x54,0x00
    db 0x03,0x01
    dw s8_D77
    db 0x01,0x00,0x56,0x00

section part9 start=0x1aaa vstart=0x1aaa align=1
            db   000h, 000h, 000h, 000h, 000h, 000h

section seg9 start=seg9_start_file vstart=0 align=1
seg9_start:
s9_L0:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0xa
    push di
    push si
    mov word [0x270], strict word 0x10
    cmp word [bp + 0xe], 1
    je short s9_L1e
    jmp near s9_Ld1
s9_L1e:
    push word [bp + 6]
    push 0
    push 0xe
    push 0
s9_L27:
    db 0x9a                                 ; lcall 0, 0x4c
s9_S28:
    dw s9_S4c
    db 0x00,0x00
    cmp ax, strict word 0xe
    jne short s9_L35
    or_ dx, dx
    je short s9_L3c
s9_L35:
    mov ax, strict word 0xffff
    jmp near s9_L330
            db   090h
s9_L3c:
    mov di, word [bp + 8]
    cmp di, -2
    je short s9_L5a
    push di
    push 0
    push 0
    push 0
s9_L4b:
    db 0x9a                                 ; lcall 0, 0xffff
s9_S4c:
    dw 0xffff
    db 0x00,0x00
    or_ dx, ax
    je short s9_L5a
    mov ax, strict word 0xfffe
    jmp near s9_L330
s9_L5a:
    mov ax, word [0x48]
    mov dx, word [0x4a]
    add ax, word [0x27c]
    inc ax
    mov word [0x4c], ax
    mov word [0x4e], dx
    mov word [0x50], ax
    mov word [0x52], dx
    mov ax, word [0x54]
    mov dx, word [0x56]
    add ax, word [0x276]
    mov word [0x58], ax
    mov word [0x5a], dx
    mov ax, word [0x54]
    mov word [0x5c], ax
    mov word [0x5e], dx
    sub_ ax, ax
    mov word [0x282], ax
    mov word [0x280], ax
    sub_ bx, bx
    mov word [0x27e], bx
    mov ax, 0x1000
    sub ax, word [0x270]
    or_ ax, ax
    jle short s9_Lc1
    mov es, word [0x46]
s9_Lad:
    mov si, word [0x44]
    mov byte [es:bx + si], 0x20
    mov ax, 0x1000
    sub ax, word [0x270]
    inc bx
    cmp_ ax, bx
    jg short s9_Lad
s9_Lc1:
    mov word [0x27a], strict word 0
    mov ax, 0x1000
    sub ax, word [0x270]
    mov word [0x278], ax
s9_Ld1:
    mov ax, word [0x50]
    cmp word [0x4c], ax
    ja short s9_Lee
    push word [bp + 6]
    lea ax, [bp - 3]
    push ss
    push ax
s9_Le2:
    db 0x9a                                 ; lcall 0, 0x156
s9_Se3:
    dw s9_S156
    db 0x00,0x00
    add sp, 6
    mov_ si, ax
    jmp short s9_Lff
s9_Lee:
    les bx, [0x50]
    mov al, byte [es:bx]
    mov byte [bp - 3], al
    inc word [0x50]
    mov si, strict word 1
s9_Lff:
    cmp si, 1
    je short s9_L10d
    cmp si, 0x1f4
    je short s9_L10d
    jmp near s9_L32e
s9_L10d:
    mov di, word [bp + 6]
s9_L110:
    cmp si, 0x1f4
    jne short s9_L119
    jmp near s9_L328
s9_L119:
    mov ax, word [bp + 0xa]
    mov dx, word [bp + 0xc]
    cmp word [0x282], dx
    jle short s9_L128
    jmp near s9_L308
s9_L128:
    jl short s9_L133
    cmp word [0x280], ax
    jbe short s9_L133
    jmp near s9_L308
s9_L133:
    shr word [0x27a], 1
    test byte [0x27b], 1
    jne short s9_L179
    mov al, byte [bp - 3]
    mov ah, 0xff
    mov word [0x27a], ax
    mov ax, word [0x50]
    cmp word [0x4c], ax
    ja short s9_L162
    push di
    lea ax, [bp - 3]
    push ss
    push ax
s9_L155:
    db 0x9a                                 ; lcall 0, 0x1f7
s9_S156:
    dw s9_S1f7
    db 0x00,0x00
    add sp, 6
    mov_ si, ax
    jmp short s9_L173
            db   090h
s9_L162:
    les bx, [0x50]
    mov al, byte [es:bx]
    mov byte [bp - 3], al
    inc word [0x50]
    mov si, strict word 1
s9_L173:
    dec si
    je short s9_L179
    jmp near s9_L302
s9_L179:
    test byte [0x27a], 1
    jne short s9_L183
    jmp near s9_L204
s9_L183:
    mov ax, word [0x58]
    cmp word [0x5c], ax
    jb short s9_L1aa
    add word [0x280], 1
    adc word [0x282], 0
    push word [bp + 8]
    mov al, byte [bp - 3]
    push ax
s9_L19d:
    db 0x9a                                 ; lcall 0, 0x28b
s9_S19e:
    dw s9_S28b
    db 0x00,0x00
    add sp, 4
    mov_ si, ax
    jmp short s9_L1c5
            db   090h
s9_L1aa:
    mov al, byte [bp - 3]
    les bx, [0x5c]
    mov byte [es:bx], al
    inc word [0x5c]
    add word [0x280], 1
    adc word [0x282], 0
    mov si, strict word 1
s9_L1c5:
    cmp si, 1
    je short s9_L1cd
    jmp near s9_L32e
s9_L1cd:
    mov al, byte [bp - 3]
    les bx, [0x44]
    mov si, word [0x278]
    mov byte [es:bx + si], al
    inc word [0x278]
    and byte [0x279], 0xf
s9_L1e4:
    mov ax, word [0x50]
    cmp word [0x4c], ax
    jbe short s9_L1f0
    jmp near s9_L2da
s9_L1f0:
    push di
    lea ax, [bp - 3]
    push ss
    push ax
s9_L1f6:
    db 0x9a                                 ; lcall 0, 0x214
s9_S1f7:
    dw s9_S214
    db 0x00,0x00
    add sp, 6
    mov_ si, ax
    jmp near s9_L2eb
            db   090h
s9_L204:
    mov ax, word [0x50]
    cmp word [0x4c], ax
    ja short s9_L220
    push di
    lea ax, [bp - 7]
    push ss
    push ax
s9_L213:
    db 0x9a                                 ; lcall 0, 0xffff
s9_S214:
    dw 0xffff
    db 0x00,0x00
    add sp, 6
    mov_ si, ax
    jmp short s9_L231
            db   090h
s9_L220:
    les bx, [0x50]
    mov al, byte [es:bx]
    mov byte [bp - 7], al
    inc word [0x50]
    mov si, strict word 1
s9_L231:
    dec si
    je short s9_L237
    jmp near s9_L302
s9_L237:
    mov al, byte [bp - 7]
    sub_ ah, ah
    mov_ cx, ax
    and ax, 0xf0
    shl ax, 4
    mov dl, byte [bp - 3]
    sub_ dh, dh
    or_ ax, dx
    mov word [bp - 6], ax
    mov_ dx, cx
    and dx, 0xf
    inc dx
    inc dx
    sub_ si, si
    or_ dx, dx
    jl short s9_L1e4
    mov word [bp - 0xa], dx
s9_L25e:
    mov bx, word [bp - 6]
    add_ bx, si
    and bh, 0xf
    les di, [0x44]
    mov al, byte [es:bx + di]
    mov byte [bp - 3], al
    mov ax, word [0x58]
    cmp word [0x5c], ax
    jb short s9_L296
    add word [0x280], 1
    adc word [0x282], 0
    push word [bp + 8]
    mov al, byte [bp - 3]
    push ax
s9_L28a:
    db 0x9a                                 ; lcall 0, 0xffff
s9_S28b:
    dw 0xffff
    db 0x00,0x00
    add sp, 4
    mov_ di, ax
    jmp short s9_L2b1
s9_L296:
    mov al, byte [bp - 3]
    les bx, [0x5c]
    mov byte [es:bx], al
    inc word [0x5c]
    add word [0x280], 1
    adc word [0x282], 0
    mov di, strict word 1
s9_L2b1:
    cmp di, 1
    jne short s9_L2fe
    mov al, byte [bp - 3]
    les bx, [0x44]
    mov di, word [0x278]
    mov byte [es:bx + di], al
    inc word [0x278]
    and byte [0x279], 0xf
    inc si
    cmp word [bp - 0xa], si
    jge short s9_L25e
    mov di, word [bp + 6]
    jmp near s9_L1e4
            db   090h
s9_L2da:
    les bx, [0x50]
    mov al, byte [es:bx]
    mov byte [bp - 3], al
    inc word [0x50]
    mov si, strict word 1
s9_L2eb:
    cmp si, 1
    jne short s9_L2f3
    jmp near s9_L110
s9_L2f3:
    cmp si, 0x1f4
    jne short s9_L2fc
    jmp near s9_L110
s9_L2fc:
    jmp short s9_L32e
s9_L2fe:
    mov_ ax, di
    jmp short s9_L330
s9_L302:
    mov ax, strict word 0xfffd
    jmp short s9_L330
            db   090h
s9_L308:
    mov ax, word [0x48]
    mov dx, word [0x4a]
    inc ax
    cmp ax, word [0x50]
    jne short s9_L324
    cmp dx, word [0x52]
    jne short s9_L324
    mov word [0x27e], strict word 1
    jmp short s9_L328
s9_L324:
    dec word [0x50]
s9_L328:
    mov ax, strict word 1
    jmp short s9_L330
            db   090h
s9_L32e:
    mov_ ax, si
s9_L330:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
seg9_end:
    dw 3                                   ; relocations
    db 0x03,0x00
    dw s9_S19e
    db 0xff,0x00,0x1a,0x00
    db 0x03,0x00
    dw s9_Se3
    db 0xff,0x00,0x1b,0x00
    db 0x03,0x01
    dw s9_S28
    db 0x01,0x00,0x54,0x00

section part10 start=0x1e04 vstart=0x1e04 align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

section seg10 start=seg10_start_file vstart=0 align=1
seg10_start:
            db   01Eh, 058h, 090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 083h, 0ECh, 006h, 056h, 0C4h, 076h
            db   006h, 08Bh, 0CEh, 08Ch, 046h, 0FCh, 026h, 080h, 03Ch, 000h, 074h, 023h, 026h, 080h, 03Ch, 02Fh
            db   074h, 00Ch, 026h, 080h, 03Ch, 05Ch, 074h, 006h, 026h, 080h, 03Ch, 03Ah, 075h, 00Ah, 08Bh, 0C6h
            db   08Ch, 0C2h, 040h, 08Bh, 0C8h, 089h, 056h, 0FCh, 046h, 026h, 080h, 03Ch, 000h, 075h, 0DDh, 08Bh
            db   0C1h, 08Bh, 056h, 0FCh, 05Eh, 04Dh, 04Dh, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CBh, 090h
s10_L4e:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 6
    push si
    push word [bp + 8]
    push word [bp + 6]
    push cs
    call 0
    add sp, 4
    mov es, dx
    mov_ bx, ax
    mov_ si, bx
    mov word [bp - 4], es
    cmp byte [es:bx], 0
    je short s10_L85
s10_L78:
    cmp byte [es:si], 0x2e
    je short s10_L85
    inc si
    cmp byte [es:si], 0
    jne short s10_L78
s10_L85:
    mov es, word [bp - 4]
    cmp byte [es:si], 0x2e
    jne short s10_L96
    mov_ ax, si
    mov dx, es
    inc ax
    jmp short s10_L99
            db   090h
s10_L96:
    sub_ ax, ax
    db 0x99                                 ; cdq 
s10_L99:
    pop si
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   01Eh, 058h, 090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 083h, 0ECh, 004h, 057h, 056h, 08Bh
            db   076h, 00Ah, 0FFh, 076h, 00Ch, 056h, 00Eh, 0E8h, 044h, 0FFh, 083h, 0C4h, 004h, 08Eh, 0C2h, 08Bh
            db   0D8h, 08Bh, 0F0h, 08Ch, 046h, 00Ch, 026h, 080h, 03Fh, 000h, 074h, 04Ch, 026h, 080h, 03Ch, 02Eh
            db   075h, 007h, 026h, 080h, 07Ch, 001h, 000h, 074h, 03Fh, 08Bh, 07Eh, 006h, 0FFh, 076h, 008h, 057h
            db   09Ah
s10_De3:
    dw s10_S153
            db   000h, 000h
            db   08Bh, 0D8h, 08Bh, 046h, 008h, 003h, 0DFh, 08Eh, 0C0h, 026h, 08Ah, 047h, 0FFh, 03Ch, 02Fh, 074h
            db   013h, 03Ch, 05Ch, 074h, 00Fh, 03Ch, 03Ah, 074h, 00Bh, 006h, 057h, 01Eh, 068h, 072h, 000h, 09Ah
s10_D107:
    dw s10_D116
            db   000h, 000h
            db   08Bh, 046h, 008h, 050h, 057h, 08Bh, 046h, 00Ch, 050h, 056h, 09Ah
s10_D116:
    dw s10_S1a4
            db   000h, 000h
            db   05Eh, 05Fh, 04Dh, 04Dh, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CBh
s10_L124:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0xc
    push di
    push si
    mov di, word [bp + 6]
    mov byte [bp - 7], 0
    push word [bp + 8]
    push di
    push cs
    call s10_L4e
    add sp, 4
    mov_ si, ax
    mov word [bp - 4], dx
    or_ dx, ax
    je short s10_L19a
    push word [bp - 4]
    push si
s10_L152:
    db 0x9a                                 ; lcall 0, 0x161
s10_S153:
    dw s10_S161
    db 0x00,0x00
    cmp ax, strict word 3
    jl short s10_L18e
    push word [bp - 4]
    push si
s10_L160:
    db 0x9a                                 ; lcall 0, 0x17e
s10_S161:
    dw s10_S17e
    db 0x00,0x00
    mov_ di, ax
    mov_ bx, si
    mov es, word [bp - 4]
    mov al, byte [es:bx + di - 1]
    mov byte [bp - 7], al
    push es
    push bx
    mov_ di, bx
    mov word [bp - 0xc], di
    mov word [bp - 0xa], es
s10_L17d:
    db 0x9a                                 ; lcall 0, 0x267
s10_S17e:
    dw s10_S267
    db 0x00,0x00
    les bx, [bp - 0xc]
    mov_ di, ax
    mov byte [es:bx + di - 1], 0x5f
    jmp short s10_L1a8
s10_L18e:
    mov ax, word [bp - 4]
    push ax
    push si
    push ds
    push strict word 0x74
    jmp short s10_L1a3
            db   090h
s10_L19a:
    mov ax, word [bp + 8]
    push ax
    push di
    push ds
    push strict word 0x76
s10_L1a3:
    db 0x9a                                 ; lcall 0, 0xffff
s10_S1a4:
    dw 0xffff
    db 0x00,0x00
s10_L1a8:
    mov al, byte [bp - 7]
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
s10_L1b6:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 0xa
    push di
    push si
    mov si, word [bp + 6]
    push word [bp + 8]
    push si
    push cs
    call s10_L4e
    add sp, 4
    mov_ di, ax
    mov word [bp - 8], dx
    or_ dx, ax
    jne short s10_L1df
    jmp near s10_L29d
s10_L1df:
    cmp byte [bp + 0xa], 0x41
    jb short s10_L1eb
    cmp byte [bp + 0xa], 0x5a
    jbe short s10_L1f7
s10_L1eb:
    cmp byte [bp + 0xa], 0x61
    jb short s10_L262
    cmp byte [bp + 0xa], 0x7a
    ja short s10_L262
s10_L1f7:
    mov es, word [bp + 8]
    cmp byte [es:si], 0
    je short s10_L21f
s10_L200:
    cmp byte [es:si], 0x41
    jl short s10_L20c
    cmp byte [es:si], 0x5a
    jle short s10_L21f
s10_L20c:
    cmp byte [es:si], 0x61
    jl short s10_L218
    cmp byte [es:si], 0x7a
    jle short s10_L21f
s10_L218:
    inc si
    cmp byte [es:si], 0
    jne short s10_L200
s10_L21f:
    mov es, word [bp + 8]
    mov al, byte [es:si]
    push ax
s10_L226:
    db 0x9a                                 ; lcall 0, 0xffff
s10_S227:
    dw 0xffff
    db 0x00,0x00
    or_ ax, ax
    je short s10_L242
    cmp byte [bp + 0xa], 0x41
    jb short s10_L262
    cmp byte [bp + 0xa], 0x5a
    ja short s10_L262
    add byte [bp + 0xa], 0x20
    jmp short s10_L262
            db   090h
s10_L242:
    mov es, word [bp + 8]
    mov al, byte [es:si]
    push ax
s10_L249:
    db 0x9a                                 ; lcall 0, 0xffff
s10_S24a:
    dw 0xffff
    db 0x00,0x00
    or_ ax, ax
    je short s10_L262
    cmp byte [bp + 0xa], 0x61
    jb short s10_L262
    cmp byte [bp + 0xa], 0x7a
    ja short s10_L262
    sub byte [bp + 0xa], 0x20
s10_L262:
    push word [bp - 8]
    push di
s10_L266:
    db 0x9a                                 ; lcall 0, 0xffff
s10_S267:
    dw 0xffff
    db 0x00,0x00
    mov_ si, ax
    or_ si, si
    jg short s10_L276
    mov es, word [bp - 8]
    jmp short s10_L28b
s10_L276:
    mov es, word [bp - 8]
    mov_ bx, di
    dec bx
    cmp byte [es:bx + si], 0x5f
    jne short s10_L28b
    mov_ bx, di
    mov al, byte [bp + 0xa]
    mov byte [es:bx + si - 1], al
s10_L28b:
    cmp byte [es:di], 0
    jne short s10_L29d
    cmp byte [es:di - 1], 0x2e
    jne short s10_L29d
    mov byte [es:di - 1], 0
s10_L29d:
    pop si
    pop di
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
s10_L2a8:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub sp, 2
    push 0x262
    push 0x260
    push word [bp + 6]
s10_L2be:
    db 0x9a                                 ; lcall 0, 0xffff
s10_S2bf:
    dw 0xffff
    db 0x00,0x00
    add sp, 6
    or_ ax, ax
    je short s10_L2d0
    mov ax, strict word 0xffff
    jmp short s10_L2ef
            db   090h
s10_L2d0:
    push word [0x262]
    push word [0x260]
    push word [bp + 8]
s10_L2db:
    db 0x9a                                 ; lcall 0, 0xffff
s10_S2dc:
    dw 0xffff
    db 0x00,0x00
    add sp, 6
    or_ ax, ax
    je short s10_L2ec
    mov ax, strict word 0xfffe
    jmp short s10_L2ef
s10_L2ec:
    mov ax, strict word 1
s10_L2ef:
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   090h
seg10_end:
    dw 6                                   ; relocations
    db 0x03,0x00
    dw s10_S2dc
    db 0xff,0x00,0x1c,0x00
    db 0x03,0x00
    dw s10_S2bf
    db 0xff,0x00,0x1d,0x00
    db 0x03,0x01
    dw s10_S24a
    db 0x02,0x00,0xb3,0x01
    db 0x03,0x01
    dw s10_S227
    db 0x02,0x00,0xb4,0x01
    db 0x03,0x01
    dw s10_D107
    db 0x01,0x00,0x59,0x00
    db 0x03,0x01
    dw s10_De3
    db 0x01,0x00,0x5a,0x00

section part11 start=0x213a vstart=0x213a align=1
            db   000h, 000h, 000h, 000h, 000h, 000h

section seg11 start=seg11_start_file vstart=0 align=1
seg11_start:
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
s11_L10:
    mov ax, ds
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    mov bx, word [bp + 6]
    mov ax, 0x5700
    test word [cs:0x71], strict word 1
    je short s11_L30
s11_L29:
    db 0x9a                                 ; lcall 0, 0xffff
s11_S2a:
    dw 0xffff
    db 0x00,0x00
    jmp short s11_L32
s11_L30:
    int 0x21
s11_L32:
    jb short s11_L3e
    mov bx, word [bp + 0xa]
    mov word [bx], cx
    mov bx, word [bp + 8]
    mov word [bx], dx
s11_L3e:
    jmp near s11_L81
            db   000h
s11_L42:
    mov ax, ds
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    mov bx, word [bp + 6]
    mov cx, word [bp + 0xa]
    mov dx, word [bp + 8]
    mov ax, 0x5701
    test word [cs:0x71], strict word 1
    je short s11_L68
s11_L61:
    db 0x9a                                 ; lcall 0, 0x2a
s11_S62:
    dw s11_S2a
    db 0x00,0x00
    jmp short s11_L6a
s11_L68:
    int 0x21
s11_L6a:
    jmp near s11_L81
            db   000h, 000h, 000h, 0C3h
s11_D71:
    dw 0xffff
            db   000h, 072h, 01Fh
s11_L76:
    xor_ ax, ax
    sub bp, 2
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
s11_L81:
    jae short s11_L76
    push ax
    call s11_Lab
    pop ax
    xor_ ah, ah
    sub bp, 2
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
            db   073h, 007h, 0E8h, 013h, 000h, 0B8h, 0FFh, 0FFh, 099h, 083h, 0EDh, 002h, 08Bh, 0E5h, 01Fh, 05Dh
            db   04Dh, 0CBh, 032h, 0E4h, 0E8h, 001h, 000h, 0CBh
s11_Lab:
    mov byte [0xbd], al
    or_ ah, ah
    jne short s11_Ld4
    cmp byte [0xba], 3
    jb short s11_Lc5
    cmp al, 0x22
    jae short s11_Lc9
    cmp al, 0x20
    jb short s11_Lc5
    mov al, 5
    jmp short s11_Lcb
s11_Lc5:
    cmp al, 0x13
    jbe short s11_Lcb
s11_Lc9:
    mov al, 0x13
s11_Lcb:
    mov bx, 0x82
    xlatb
s11_Lcf:
    db 0x98                                 ; cwde 
    mov word [0xb2], ax
    ret
s11_Ld4:
    mov_ al, ah
    jmp short s11_Lcf
            db   08Ch, 0D8h, 090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 08Ch, 01Eh, 0DDh, 000h, 0B8h, 000h
            db   035h, 02Eh, 0F7h, 006h, 071h, 000h, 001h, 000h, 074h, 007h, 09Ah
s11_Df3:
    dw s11_S62
            db   000h, 000h
            db   0EBh, 002h, 0CDh, 021h, 089h, 01Eh, 0A4h, 000h, 08Ch, 006h, 0A6h, 000h, 00Eh, 01Fh, 0B8h, 000h
            db   025h, 0BAh, 0B4h, 002h, 02Eh, 0F7h, 006h, 071h, 000h, 001h, 000h, 074h, 007h, 09Ah
s11_D115:
    dw s11_Df3
            db   000h, 000h
            db   0EBh, 002h, 0CDh, 021h, 016h, 01Fh, 08Bh, 00Eh, 0F0h, 000h, 0E3h, 02Eh, 08Eh, 006h, 0B8h, 000h
            db   026h, 08Bh, 036h, 02Ch, 000h, 0C5h, 006h, 0F2h, 000h, 08Ch, 0DAh, 033h, 0DBh, 036h, 0FFh, 01Eh
            db   0EEh, 000h, 073h, 005h, 016h, 01Fh, 0E9h, 09Ah, 001h, 036h, 0C5h, 006h, 0F6h, 000h, 08Ch, 0DAh
            db   0BBh, 003h, 000h, 036h, 0FFh, 01Eh, 0EEh, 000h, 016h, 01Fh, 08Eh, 006h, 0B8h, 000h, 026h, 08Bh
            db   00Eh, 02Ch, 000h, 0E3h, 03Eh, 08Eh, 0C1h, 033h, 0FFh, 026h, 080h, 03Dh, 000h, 074h, 034h, 0B9h
            db   00Dh, 000h, 0BEh, 096h, 000h, 0F3h, 0A6h, 074h, 00Bh, 0B9h, 0FFh, 07Fh, 033h, 0C0h, 0F2h, 0AEh
            db   075h, 021h, 0EBh, 0E5h, 006h, 01Eh, 007h, 01Fh, 08Bh, 0F7h, 0BFh, 0C1h, 000h, 0B1h, 004h, 0ACh
            db   02Ch, 041h, 072h, 00Dh, 0D2h, 0E0h, 092h, 0ACh, 02Ch, 041h, 072h, 005h, 00Ah, 0C2h, 0AAh, 0EBh
            db   0EEh, 016h, 01Fh, 0BEh, 0FAh, 000h, 0BFh, 0FAh, 000h, 0E8h, 0FBh, 000h, 0BEh, 0FAh, 000h, 0BFh
            db   0FAh, 000h, 0E8h, 0F2h, 000h, 083h, 0EDh, 002h, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CBh, 08Ch, 0D8h
            db   090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 033h, 0C9h, 0EBh, 02Fh, 08Ch, 0D8h, 090h, 045h
            db   055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 0B9h, 001h, 000h, 0EBh, 020h, 08Ch, 0D8h, 090h, 045h, 055h
            db   08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 056h, 057h, 0B9h, 000h, 001h, 0EBh, 00Fh, 08Ch, 0D8h, 090h, 045h
            db   055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 056h, 057h, 0B9h, 001h, 001h, 051h, 00Ah, 0C9h, 075h, 01Eh
            db   0BEh, 070h, 002h, 0BFh, 070h, 002h, 0E8h, 09Eh, 000h, 0BEh, 0FAh, 000h, 0BFh, 0FAh, 000h, 0E8h
            db   095h, 000h, 081h, 03Eh, 0FAh, 000h, 0D6h, 0D6h, 075h, 004h, 0FFh, 016h, 000h, 001h, 0BEh, 0FAh
            db   000h, 0BFh, 0FAh, 000h, 0E8h, 080h, 000h, 0BEh, 0FAh, 000h, 0BFh, 0FAh, 000h, 0E8h, 077h, 000h
            db   0E8h, 027h, 000h, 058h, 00Ah, 0E4h, 075h, 017h, 08Bh, 046h, 006h, 0B4h, 04Ch, 02Eh, 0F7h, 006h
            db   071h, 000h, 001h, 000h, 074h, 007h, 09Ah
s11_D240:
    dw s11_D115
            db   000h, 000h
            db   0EBh, 002h, 0CDh, 021h, 05Fh, 05Eh, 083h, 0EDh, 002h, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CBh, 08Bh
            db   00Eh, 0F0h, 000h, 0E3h, 007h, 0BBh, 002h, 000h, 0FFh, 01Eh, 0EEh, 000h, 01Eh, 0C5h, 016h, 0A4h
            db   000h, 0B8h, 000h, 025h, 02Eh, 0F7h, 006h, 071h, 000h, 001h, 000h, 074h, 007h, 09Ah
s11_D272:
    dw s11_D240
            db   000h, 000h
            db   0EBh, 002h, 0CDh, 021h, 01Fh, 080h, 03Eh, 0E2h, 000h, 000h, 074h, 01Dh, 01Eh, 0A0h, 0E3h, 000h
            db   0C5h, 016h, 0E4h, 000h, 0B4h, 025h, 02Eh, 0F7h, 006h, 071h, 000h, 001h, 000h, 074h, 007h, 09Ah
s11_D296:
    dw s11_D272
            db   000h, 000h
            db   0EBh, 002h, 0CDh, 021h, 01Fh, 0C3h, 03Bh, 0F7h, 073h, 00Eh, 083h, 0EFh, 004h, 08Bh, 005h, 00Bh
            db   045h, 002h, 074h, 0F2h, 0FFh, 01Dh, 0EBh, 0EEh, 0C3h, 000h, 09Ah
s11_D2b5:
    dw 0xffff
            db   000h, 000h
            db   08Eh, 0D8h, 0B8h, 003h, 000h, 050h, 00Eh, 0E8h, 056h, 000h, 0B8h, 000h, 000h, 050h, 01Eh, 08Dh
            db   006h, 010h, 001h, 050h, 09Ah
s11_D2ce:
    dw 0xffff
            db   000h, 000h
            db   0B8h, 0FFh, 000h, 050h, 09Ah
s11_D2d7:
    dw 0xffff
            db   000h, 000h
            db   000h, 0B8h, 002h, 000h, 0E9h, 0DCh, 0FFh, 08Ch, 0D8h, 090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh
            db   0D8h, 056h, 057h, 01Eh, 007h, 08Bh, 056h, 006h, 0BEh, 021h, 001h, 0ADh, 03Bh, 0C2h, 074h, 010h
            db   040h, 096h, 074h, 00Ch, 097h, 033h, 0C0h, 0B9h, 0FFh, 0FFh, 0F2h, 0AEh, 08Bh, 0F7h, 0EBh, 0EBh
            db   096h, 05Fh, 05Eh, 083h, 0EDh, 002h, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CAh, 002h, 000h, 08Ch, 0D8h
            db   090h, 045h, 055h, 08Bh, 0ECh, 01Eh, 08Eh, 0D8h, 057h, 0FFh, 076h, 006h, 00Eh, 0E8h, 0B7h, 0FFh
            db   00Bh, 0C0h, 074h, 012h, 08Dh, 00Eh, 010h, 001h, 01Eh, 051h, 09Ah
s11_D336:
    dw 0xffff
            db   000h, 000h
            db   01Eh, 050h, 09Ah
s11_D33d:
    dw s11_D336
            db   000h, 000h
            db   05Fh, 083h, 0EDh, 002h, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CAh, 002h, 000h, 000h
s11_L34e:
    cmp byte [cs:0x35e], 0xb8
    je short s11_L359
    mov ax, ss
    retf
s11_L359:
    mov ax, word [cs:0x35f]
    retf
s11_L35e:
    push ds
    pop ax
    nop
    inc bp
    push bp
    mov_ bp, sp
    push ds
    mov ds, ax
    sub_ ax, ax
    dec bp
    dec bp
    mov_ sp, bp
    pop ds
    pop bp
    dec bp
    retf
seg11_end:
    dw 6                                   ; relocations
    db 0x03,0x01
    dw s11_D2d7
    db 0x01,0x00,0x01,0x00
    db 0x03,0x01
    dw s11_D2ce
    db 0x01,0x00,0x89,0x00
    db 0x03,0x00
    dw s11_D2b5
    db 0xff,0x00,0x1e,0x00
    db 0x05,0x01
    dw s11_D71
    db 0x01,0x00,0xb2,0x00
    db 0x03,0x01
    dw s11_D296
    db 0x01,0x00,0x66,0x00
    db 0x03,0x01
    dw s11_D33d
    db 0x01,0x00,0x73,0x00

section part12 start=0x24e4 vstart=0x24e4 align=1
            db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0E0h, 001h, 034h, 000h
            db   'VS_VERSION_INFO'
            db   000h, 0BDh, 004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h, 00Ah, 000h, 003h, 000h, 067h, 000h, 000h
            db   000h, 00Ah, 000h, 003h, 000h, 067h, 000h, 000h, 000h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 001h, 000h, 001h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
            db   000h, 000h, 000h, 000h, 000h, 072h, 001h, 000h, 000h
            db   'StringFileInfo'
            db   000h, 000h, 05Eh, 001h, 000h, 000h
            db   '040904E4'
            db   000h, 000h, 000h, 000h, 027h, 000h, 017h, 000h
            db   'CompanyName'
            db   000h
            db   'Microsoft Corporation'
            db   000h, 000h, 000h, 033h, 000h, 01Fh, 000h
            db   'FileDescription'
            db   000h
            db   'Windows file expansion library'
            db   000h, 000h, 016h, 000h, 006h, 000h
            db   'FileVersion'
            db   000h, 033h, 02Eh, 031h, 030h, 000h, 000h, 000h, 000h, 01Dh, 000h, 009h, 000h
            db   'InternalName'
            db   000h, 000h, 000h, 000h
            db   'LZEXPAND'
            db   000h, 000h, 000h, 000h, 03Bh, 000h, 027h, 000h
            db   'LegalCopyright'
            db   000h, 000h
            db   'Copyright '
            db   0A9h
            db   ' Microsoft Corp. 1989-1992'
            db   000h, 000h, 000h, 025h, 000h, 00Dh, 000h
            db   'OriginalFilename'
            db   000h, 000h, 000h, 000h
            db   'LZEXPAND.DLL'
            db   000h, 000h, 000h, 000h, 039h, 000h, 029h, 000h
            db   'ProductName'
            db   000h
            db   'Microsoft'
            db   0AEh
            db   ' Windows(TM) Operating System'
            db   000h, 000h, 000h, 000h, 000h, 01Ah, 000h, 006h, 000h
            db   'ProductVersion'
            db   000h, 000h, 033h, 02Eh, 031h, 030h, 000h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
            db   'VarFileInfo'
            db   000h, 014h, 000h, 004h, 000h
            db   'Translation'
            db   000h, 009h, 004h, 0E4h, 004h

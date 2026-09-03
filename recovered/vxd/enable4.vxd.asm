; ENABLE4 -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 3832 instructions recovered by recursive descent

Declare_Virtual_Device ENABLE4, 0, 128, ENABLE4_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 000000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x367c  R-X
        db  0EBh, 006h, 0FFh, 025h
        dd  sub_1_291C               ; fixup
loc_1_0008:
        pushal 
        mov    dword ptr [0x2924], eax       ; -> loc_1_2924
        VxDCall  ENABLE.VMINI_Status             ; cd2002003700
        mov    dword ptr [0x2920], eax       ; -> loc_1_2920
        cmp    dword ptr [0x2924], 0x80      ; -> loc_1_2924
        jne    loc_1_0081
        mov    byte ptr [0x299f], 1          ; -> obj1:0x299F
        mov    esi, dword ptr [0x2904]       ; -> obj1:0x2904
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        push   0
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
        push   1
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_0009h              ; cd2009002b00
        add    esp, 4
        xor    edx, edx
        mov    eax, 0x64
        mov    esi, 0xdb                     ; -> loc_1_00DB
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x2904], esi       ; -> obj1:0x2904
loc_1_0081:
        popal  
        clc    
        call   sub_1_291C
        ret    
sub_1_008A:
        cmp    eax, 0x12c
        jne    loc_1_0097
        mov    eax, 0xff11
        ret    
loc_1_0097:
        cmp    eax, 0x4b0
        jne    loc_1_00A4
        mov    eax, 0xff13
        ret    
loc_1_00A4:
        cmp    eax, 0x960
        jne    loc_1_00B1
        mov    eax, 0xff14
        ret    
loc_1_00B1:
        cmp    eax, 0x12c0
        jne    loc_1_00BE
        mov    eax, 0xff15
        ret    
loc_1_00BE:
        cmp    eax, 0x2580
        jne    loc_1_00CB
        mov    eax, 0xff16
        ret    
loc_1_00CB:
        cmp    eax, 0x4b00
        jne    loc_1_00D8
        mov    eax, 0xff18
        ret    
loc_1_00D8:
        xor    eax, eax
        ret    
loc_1_00DB:
        mov    dword ptr [0x2904], 0         ; -> obj1:0x2904
        mov    eax, dword ptr [0x2966]       ; -> loc_1_2966
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
        push   eax
        VxDCall  BIOS.Service_0004h              ; cd2004002b00
        add    esp, 8
        mov    dword ptr [0x2900], eax       ; -> obj1:0x2900
        cmp    eax, -1
        jne    loc_1_010A
        jmp    loc_1_01C7
loc_1_010A:
        cmp    eax, -2
        jne    loc_1_0114
        jmp    loc_1_01C7
loc_1_0114:
        push   0x296e                        ; -> loc_1_296E
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_0006h              ; cd2006002b00
        add    esp, 8
        or     eax, eax
        jne    loc_1_012E
        jmp    loc_1_01AD
loc_1_012E:
        mov    eax, dword ptr [0x295e]       ; -> loc_1_295E
        mov    eax, dword ptr [eax + 0x10]
        call   sub_1_008A
        mov    dword ptr [0x2972], eax       ; -> loc_1_2972
        mov    byte ptr [0x2984], 8          ; -> loc_1_2984
        mov    byte ptr [0x2985], 0          ; -> obj1:0x2985
        mov    byte ptr [0x2986], 0          ; -> loc_1_2986
        push   0x57
        push   0x296e                        ; -> loc_1_296E
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_0005h              ; cd2005002b00
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_0171
        jmp    loc_1_01AD
loc_1_0171:
        push   0x100
        push   0
        push   0x100
        push   0
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_0007h              ; cd2007002b00
        add    esp, 0x14
        or     eax, eax
        jne    loc_1_0194
        jmp    loc_1_01AD
loc_1_0194:
        mov    eax, dword ptr [0x2900]       ; -> obj1:0x2900
        mov    dword ptr [0x29a8], eax       ; -> obj1:0x29A8
        mov    dword ptr [0x356c], 1         ; -> loc_1_356C
        call   sub_1_1516
loc_1_01AD:
        xor    edx, edx
        mov    eax, 0x64
        mov    esi, 0x338                    ; -> loc_1_0338
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x2904], esi       ; -> obj1:0x2904
        clc    
        ret    
loc_1_01C7:
        stc    
        ret    
sub_1_01C9:
        movzx  ebx, byte ptr [0x2929]        ; -> obj1:0x2929
        cmp    bl, byte ptr [0x2928]         ; -> loc_1_2928
        je     loc_1_0243
        cmp    ebx, 0x18
        jne    loc_1_01E2
        mov    ebx, 0
loc_1_01E2:
        movsx  edi, word ptr [ebx*2 + 0x292a] ; -> loc_1_292A
        inc    ebx
        mov    byte ptr [0x2929], bl         ; -> obj1:0x2929
        movzx  ebx, byte ptr [0x2929]        ; -> obj1:0x2929
        cmp    bl, byte ptr [0x2928]         ; -> loc_1_2928
        je     loc_1_0243
        cmp    ebx, 0x18
        jne    loc_1_020A
        mov    ebx, 0
loc_1_020A:
        movsx  esi, word ptr [ebx*2 + 0x292a] ; -> loc_1_292A
        inc    ebx
        mov    byte ptr [0x2929], bl         ; -> obj1:0x2929
        movzx  ebx, byte ptr [0x2929]        ; -> obj1:0x2929
        cmp    bl, byte ptr [0x2928]         ; -> loc_1_2928
        je     loc_1_0243
        cmp    ebx, 0x18
        jne    loc_1_0232
        mov    ebx, 0
loc_1_0232:
        mov    ax, word ptr [ebx*2 + 0x292a] ; -> loc_1_292A
        inc    ebx
        mov    byte ptr [0x2929], bl         ; -> obj1:0x2929
        clc    
        ret    
loc_1_0243:
        stc    
        ret    
sub_1_0245:
        call   sub_1_01C9
        jb     loc_1_0254
        VxDCall  VMD.Service_0003h               ; cd2003000c00
        stc    
        ret    
loc_1_0254:
        clc    
        ret    
sub_1_0256:
        push   0x296e                        ; -> loc_1_296E
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_0006h              ; cd2006002b00
        add    esp, 8
        or     eax, eax
        jne    loc_1_0270
        jmp    loc_1_02B0
loc_1_0270:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x2972], eax       ; -> loc_1_2972
        mov    byte ptr [0x2984], 8          ; -> loc_1_2984
        mov    byte ptr [0x2985], 0          ; -> obj1:0x2985
        mov    byte ptr [0x2986], 0          ; -> loc_1_2986
        push   0x57
        push   0x296e                        ; -> loc_1_296E
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_0005h              ; cd2005002b00
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_02AA
        jmp    loc_1_02B0
loc_1_02AA:
        mov    eax, 0xffffffff
        ret    
loc_1_02B0:
        xor    eax, eax
        ret    
sub_1_02B3:
        mov    eax, dword ptr [esp + 4]
        mov    ebx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0xc]
        push   0x2914                        ; -> obj1:0x2914
        push   ecx
        push   ebx
        push   eax
        VxDCall  BIOS.Service_0013h              ; cd2013002b00
        add    esp, 0x10
        or     eax, eax
        je     loc_1_02DA
        mov    eax, dword ptr [0x2914]       ; -> obj1:0x2914
        ret    
loc_1_02DA:
        mov    eax, dword ptr [esp + 4]
        push   0x2918                        ; -> obj1:0x2918
        push   0
        push   eax
        VxDCall  BIOS.Service_000Bh              ; cd200b002b00
        add    esp, 0xc
        xor    eax, eax
        ret    
sub_1_02F2:
        mov    eax, dword ptr [esp + 4]
        mov    ebx, dword ptr [esp + 8]
        push   0x2918                        ; -> obj1:0x2918
        push   ebx
        push   eax
        VxDCall  BIOS.Service_000Bh              ; cd200b002b00
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0314
        mov    eax, dword ptr [0x2918]       ; -> obj1:0x2918
        ret    
loc_1_0314:
        xor    eax, eax
        ret    
sub_1_0317:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   0
        push   0xd
        push   eax
        VxDCall  BIOS.Service_000Eh              ; cd200e002b00
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0332
        xor    eax, eax
        ret    
loc_1_0332:
        mov    eax, 0xffffffff
        ret    
loc_1_0338:
        mov    dword ptr [0x2904], 0         ; -> obj1:0x2904
        call   sub_1_1266
        call   sub_1_0245
        cmp    byte ptr [0x299f], 0          ; -> obj1:0x299F
        je     loc_1_0356
        ret    
loc_1_0356:
        mov    eax, 0x32
        mov    edx, 0
        mov    esi, 0x338                    ; -> loc_1_0338
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x2904], esi       ; -> obj1:0x2904
        ret    
sub_1_0372:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    byte ptr [0x299e], al         ; -> loc_1_299E
        mov    esi, 0x299e                   ; -> loc_1_299E
        mov    ecx, 1
        VxDCall  VKD.VKD_Force_Keys              ; cd2007000d00
        pop    esi
        ret    
sub_1_038E:
        mov    eax, dword ptr [esp + 8]
        movzx  ebx, byte ptr [0x2928]        ; -> loc_1_2928
        cmp    ebx, 0x18
        jne    loc_1_03A3
        mov    ebx, 0
loc_1_03A3:
        mov    word ptr [ebx*2 + 0x292a], ax ; -> loc_1_292A
        inc    ebx
        mov    byte ptr [0x2928], bl         ; -> loc_1_2928
        mov    eax, dword ptr [esp + 0xc]
        movzx  ebx, byte ptr [0x2928]        ; -> loc_1_2928
        cmp    ebx, 0x18
        jne    loc_1_03C7
        mov    ebx, 0
loc_1_03C7:
        mov    word ptr [ebx*2 + 0x292a], ax ; -> loc_1_292A
        inc    ebx
        mov    byte ptr [0x2928], bl         ; -> loc_1_2928
        mov    eax, dword ptr [esp + 0x10]
        and    eax, 0xff
        movzx  ebx, byte ptr [0x2928]        ; -> loc_1_2928
        cmp    ebx, 0x18
        jne    loc_1_03F0
        mov    ebx, 0
loc_1_03F0:
        mov    word ptr [ebx*2 + 0x292a], ax ; -> loc_1_292A
        inc    ebx
        mov    byte ptr [0x2928], bl         ; -> loc_1_2928
        ret    
sub_1_0400:
        VxDCall  VSD.VSD_Bell                    ; cd2001000b00
        ret    
loc_1_0407:
        mov    byte ptr [0x299f], 1          ; -> obj1:0x299F
        mov    esi, dword ptr [0x2904]       ; -> obj1:0x2904
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    eax, 0x370001
        mov    esi, 8                        ; -> loc_1_0008
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0xe0000000
        jae    loc_1_0471
        cmp    dword ptr [0x2900], 0         ; -> obj1:0x2900
        je     loc_1_0471
        push   0
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
        push   1
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
        push   dword ptr [0x2900]            ; -> obj1:0x2900
        VxDCall  BIOS.Service_0009h              ; cd2009002b00
        add    esp, 4
loc_1_0471:
        clc    
        ret    
loc_1_0473:
        mov    eax, 0x400
        clc    
        ret    
ENABLE4_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        cmp    eax, 0x1c
        je     loc_1_0407
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 5
        je     loc_1_0407
        clc    
        ret    
loc_1_04A0:
        ret    
loc_1_04A1:
        ret    
loc_1_04A2:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 4
        jg     loc_1_04BF
        je     loc_1_04FB
        cmp    eax, 1
        je     loc_1_04D7
        cmp    eax, 2
        je     loc_1_04E3
        cmp    eax, 3
        je     loc_1_04EF
        jmp    loc_1_04D0
loc_1_04BF:
        cmp    eax, 5
        je     loc_1_0507
        cmp    eax, 6
        je     loc_1_0513
        cmp    eax, 0xff
        je     loc_1_051F
loc_1_04D0:
        call   sub_1_0652
        jmp    loc_1_0537
loc_1_04D7:
        mov    dword ptr [0x34a4], 0xff11    ; -> obj1:0x34A4
        jmp    loc_1_0537
loc_1_04E3:
        mov    dword ptr [0x34a4], 0xff12    ; -> obj1:0x34A4
        jmp    loc_1_0537
loc_1_04EF:
        mov    dword ptr [0x34a4], 0xff13    ; -> obj1:0x34A4
        jmp    loc_1_0537
loc_1_04FB:
        mov    dword ptr [0x34a4], 0xff14    ; -> obj1:0x34A4
        jmp    loc_1_0537
loc_1_0507:
        mov    dword ptr [0x34a4], 0xff15    ; -> obj1:0x34A4
        jmp    loc_1_0537
loc_1_0513:
        mov    dword ptr [0x34a4], 0xff16    ; -> obj1:0x34A4
        jmp    loc_1_0537
loc_1_051F:
        cmp    dword ptr [0x34a4], 0         ; -> obj1:0x34A4
        je     loc_1_0537
        mov    dword ptr [0x29b0], 1         ; -> obj1:0x29B0
        call   sub_1_0400
loc_1_0537:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0540:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, eax
        cmp    byte ptr [eax], 0
        je     loc_1_0562
loc_1_054B:
        mov    dl, byte ptr [ecx]
        cmp    dl, 0x41
        jl     loc_1_055C
        cmp    dl, 0x5a
        jg     loc_1_055C
        add    dl, 0x20
        mov    byte ptr [ecx], dl
loc_1_055C:
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    loc_1_054B
loc_1_0562:
        ret    
sub_1_0563:
        mov    ecx, dword ptr [esp + 4]
        sub    eax, eax
        cmp    byte ptr [ecx], al
        je     loc_1_0574
loc_1_056D:
        inc    ecx
        inc    eax
        cmp    byte ptr [ecx], 0
        jne    loc_1_056D
loc_1_0574:
        ret    
sub_1_0575:
        push   ebx
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, eax
        mov    bl, byte ptr [edx]
        mov    byte ptr [eax], bl
        or     bl, bl
        je     loc_1_0593
loc_1_0588:
        inc    ecx
        mov    bl, byte ptr [edx + 1]
        inc    edx
        mov    byte ptr [ecx], bl
        or     bl, bl
        jne    loc_1_0588
loc_1_0593:
        pop    ebx
        ret    
sub_1_0595:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_1_059D:
        mov    dl, byte ptr [eax]
        or     dl, dl
        jne    loc_1_05A8
        cmp    byte ptr [ecx], 0
        je     loc_1_05B0
loc_1_05A8:
        cmp    byte ptr [ecx], dl
        jne    loc_1_05B4
        inc    eax
        inc    ecx
        jmp    loc_1_059D
loc_1_05B0:
        sub    eax, eax
        jmp    loc_1_05BC
loc_1_05B4:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [ecx]
        sub    eax, ecx
loc_1_05BC:
        ret    
sub_1_05BD:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, eax
        cmp    byte ptr [eax], 0
        je     loc_1_05D0
loc_1_05CA:
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_1_05CA
loc_1_05D0:
        mov    edx, dword ptr [esp + 0x14]
        or     edx, edx
        je     loc_1_05E9
        mov    ecx, dword ptr [esp + 0x10]
loc_1_05DC:
        mov    bl, byte ptr [ecx]
        or     bl, bl
        je     loc_1_05E9
        mov    byte ptr [esi], bl
        inc    esi
        inc    ecx
        dec    edx
        jne    loc_1_05DC
loc_1_05E9:
        mov    byte ptr [esi], 0
        pop    esi
        pop    ebx
        ret    
sub_1_05EF:
        push   esi
        sub    esi, esi
        mov    eax, 1
        mov    edx, dword ptr [esp + 8]
loc_1_05FB:
        mov    cl, byte ptr [edx]
        cmp    cl, 0x20
        je     loc_1_0607
        cmp    cl, 9
        jne    loc_1_060A
loc_1_0607:
        inc    edx
        jmp    loc_1_05FB
loc_1_060A:
        mov    cl, byte ptr [edx]
        cmp    cl, 0x2d
        jne    loc_1_0618
        mov    eax, 0xffffffff
        jmp    loc_1_061D
loc_1_0618:
        cmp    cl, 0x2b
        jne    loc_1_061E
loc_1_061D:
        inc    edx
loc_1_061E:
        cmp    byte ptr [edx], 0x30
        jl     loc_1_063A
loc_1_0623:
        mov    cl, byte ptr [edx]
        cmp    cl, 0x39
        jg     loc_1_063A
        movsx  ecx, cl
        imul   esi, esi, 0xa
        inc    edx
        lea    esi, [ecx + esi - 0x30]
        cmp    byte ptr [edx], 0x30
        jge    loc_1_0623
loc_1_063A:
        imul   eax, esi
        pop    esi
        ret    
        db  066h, 02Bh, 0C0h, 0C3h
sub_1_0643:
        call   sub_1_0400
        call   sub_1_0400
        jmp    sub_1_1449
sub_1_0652:
        call   sub_1_0400
        jmp    sub_1_1449
sub_1_065C:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    eax, dword ptr [0x29e8]       ; -> obj1:0x29E8
        mov    esi, 0x29e8                   ; -> obj1:0x29E8
        cmp    byte ptr [eax], 0
        je     loc_1_06A0
        mov    ebx, dword ptr [esp + 0x10]
loc_1_0674:
        or     edi, edi
        jne    loc_1_06A0
        push   dword ptr [esi]
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_0595
        add    esp, 8
        or     eax, eax
        jne    loc_1_0695
        mov    edi, 1
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebx], al
loc_1_0695:
        mov    eax, dword ptr [esi + 5]
        add    esi, 5
        cmp    byte ptr [eax], 0
        jne    loc_1_0674
loc_1_06A0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_06A6:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x3634], 0x3078    ; -> loc_1_3634
        jne    loc_1_06FB
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_0563
        add    esp, 4
        cmp    eax, 1
        jne    loc_1_06FB
        movsx  eax, byte ptr [0x3604]        ; -> loc_1_3604
        add    eax, eax
        mov    cl, byte ptr [eax + 0x2c38]   ; -> obj1:0x2C38
        or     cl, cl
        jne    loc_1_06DE
        sub    eax, eax
        jmp    loc_1_073C
loc_1_06DE:
        cmp    cl, 0x3a
        je     loc_1_06E8
        cmp    cl, 0x2c
        jne    loc_1_06EE
loc_1_06E8:
        mov    cl, byte ptr [eax + 0x2c39]   ; -> loc_1_2C39
loc_1_06EE:
        mov    ebx, dword ptr [esp + 0x10]
        mov    eax, 1
        mov    byte ptr [ebx], cl
        jmp    loc_1_073C
loc_1_06FB:
        mov    esi, dword ptr [0x3634]       ; -> loc_1_3634
        sub    edi, edi
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax], 0
        je     loc_1_073A
        mov    ebx, dword ptr [esp + 0x10]
loc_1_070E:
        or     edi, edi
        jne    loc_1_073A
        push   dword ptr [esi]
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_0595
        add    esp, 8
        or     eax, eax
        jne    loc_1_072F
        mov    edi, 1
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebx], al
loc_1_072F:
        mov    eax, dword ptr [esi + 5]
        add    esi, 5
        cmp    byte ptr [eax], 0
        jne    loc_1_070E
loc_1_073A:
        mov    eax, edi
loc_1_073C:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0740:
        mov    ecx, dword ptr [0x3630]       ; -> loc_1_3630
        cmp    ecx, 0x14
        mov    eax, 0
        jge    loc_1_0779
        lea    edx, [ecx*4]
        mov    eax, dword ptr [0x3634]       ; -> loc_1_3634
        mov    dword ptr [edx + 0x357c], eax ; -> loc_1_357C
        mov    eax, dword ptr [0x3568]       ; -> sub_1_3568
        mov    dword ptr [edx + 0x34f4], eax ; -> loc_1_34F4
        inc    ecx
        mov    dword ptr [0x3630], ecx       ; -> loc_1_3630
        mov    eax, 1
loc_1_0779:
        ret    
loc_1_077A:
        mov    eax, dword ptr [0x3630]       ; -> loc_1_3630
        or     eax, eax
        jle    loc_1_07AD
        dec    eax
        lea    ecx, [eax*4]
        mov    dword ptr [0x3630], eax       ; -> loc_1_3630
        mov    eax, dword ptr [ecx + 0x357c] ; -> loc_1_357C
        mov    dword ptr [0x3634], eax       ; -> loc_1_3634
        mov    eax, dword ptr [ecx + 0x34f4] ; -> loc_1_34F4
        mov    dword ptr [0x3568], eax       ; -> sub_1_3568
        mov    eax, 1
        jmp    loc_1_07AF
loc_1_07AD:
        sub    eax, eax
loc_1_07AF:
        ret    
sub_1_07B0:
        mov    edx, dword ptr [0x3630]       ; -> loc_1_3630
        or     edx, edx
        mov    eax, 0
        jle    loc_1_07E9
        lea    ecx, [edx*4 - 4]
        dec    edx
        mov    eax, dword ptr [ecx + 0x357c] ; -> loc_1_357C
        mov    dword ptr [0x3634], eax       ; -> loc_1_3634
        mov    eax, dword ptr [ecx + 0x34f4] ; -> loc_1_34F4
        mov    dword ptr [0x3568], eax       ; -> sub_1_3568
        inc    edx
        mov    dword ptr [0x3630], edx       ; -> loc_1_3630
        mov    eax, 1
loc_1_07E9:
        ret    
sub_1_07EA:
        mov    eax, dword ptr [0x361c]       ; -> loc_1_361C
        sub    eax, dword ptr [0x3600]       ; -> loc_1_3600
        cmp    eax, -1
        je     loc_1_083D
        cmp    dword ptr [0x361c], 0x13      ; -> loc_1_361C
        jne    loc_1_080C
        cmp    dword ptr [0x3600], 0         ; -> loc_1_3600
        je     loc_1_083D
loc_1_080C:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [0x361c]       ; -> loc_1_361C
        inc    dword ptr [0x361c]            ; -> loc_1_361C
        mov    byte ptr [eax + 0x35cc], cl   ; -> loc_1_35CC
        cmp    dword ptr [0x361c], 0x14      ; -> loc_1_361C
        jne    loc_1_0836
        mov    dword ptr [0x361c], 0         ; -> loc_1_361C
loc_1_0836:
        mov    eax, 1
        jmp    loc_1_083F
loc_1_083D:
        sub    eax, eax
loc_1_083F:
        ret    
sub_1_0840:
        cmp    dword ptr [0x3600], 0x14      ; -> loc_1_3600
        jne    loc_1_0853
        mov    dword ptr [0x3600], 0         ; -> loc_1_3600
loc_1_0853:
        mov    eax, dword ptr [0x361c]       ; -> loc_1_361C
        cmp    dword ptr [0x3600], eax       ; -> loc_1_3600
        mov    eax, 0
        je     loc_1_0881
        inc    dword ptr [0x3600]            ; -> loc_1_3600
        mov    eax, dword ptr [0x3600]       ; -> loc_1_3600
        mov    cl, byte ptr [eax + 0x35cb]   ; -> obj1:0x35CB
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax], cl
        mov    eax, 1
loc_1_0881:
        ret    
loc_1_0882:
        ret    
loc_1_0883:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0x96
        jg     loc_1_08AD
        je     loc_1_0A47
        sub    eax, 2
        cmp    eax, 0x12
        ja     loc_1_0AA2
        shl    eax, 2
        jmp    loc_1_0AC9
loc_1_08AD:
        cmp    eax, 0x97
        je     loc_1_0A5D
        cmp    eax, 0xa0
        je     loc_1_0A73
        cmp    eax, 0xfd
        je     loc_1_0A89
        jmp    loc_1_0AA2
loc_1_08D3:
        mov    dword ptr [0x3568], 0x1dda    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x3078    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_08EC:
        mov    dword ptr [0x3568], 0x1cde    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x3078    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0905:
        mov    dword ptr [0x3568], 0x1bb7    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x3078    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_091E:
        mov    dword ptr [0x3568], 0x1a84    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x3078    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0937:
        mov    dword ptr [0x3568], 0x197d    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x3078    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0950:
        mov    dword ptr [0x3568], 0x1ec6    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2b14    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0969:
        mov    dword ptr [0x3568], 0x256d    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2c0c    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0982:
        mov    dword ptr [0x3568], 0x2478    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2c0c    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_099B:
        mov    dword ptr [0x3568], 0x2725    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2c0c    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_09B4:
        mov    dword ptr [0x3568], 0x264b    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2c0c    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_09CD:
        mov    dword ptr [0x3568], 0x2321    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x34ac    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_09E6:
        mov    dword ptr [0x3568], 0x220e    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x34ac    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_09FF:
        mov    dword ptr [0x3568], 0x1fc4    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x34ac    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0A18:
        mov    dword ptr [0x3568], 0x2103    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x34ac    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0A31:
        mov    dword ptr [0x3568], 0x2872    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2b88    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0A47:
        mov    dword ptr [0x3568], 0x4a1     ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2ba0    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0A5D:
        mov    dword ptr [0x3568], 0x4a2     ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2bd4    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0A73:
        mov    dword ptr [0x3568], 0x4a0     ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2b94    ; -> loc_1_3634
        jmp    loc_1_0ABD
loc_1_0A89:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_0AA2:
        cmp    bl, 0xec
        jb     loc_1_0AAE
        call   sub_1_0643
        jmp    loc_1_0AC7
loc_1_0AAE:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
loc_1_0ABD:
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_0AC7:
        pop    ebx
        ret    
loc_1_0AC9:
        ror    dword ptr [eax], cl
        add    byte ptr [eax], al
        in     al, dx
        or     byte ptr [eax], al
        add    byte ptr [0x1e000009], al     ; -> loc_1_0905
        or     dword ptr [eax], eax
        add    byte ptr [edi], dh            ; -> loc_1_0937
        or     dword ptr [eax], eax
        add    byte ptr [edx - 0x5dfffff6], ah ; -> loc_1_0AA2
        or     al, byte ptr [eax]
        add    byte ptr [edx + 0x5000000a], ah ; -> loc_1_0AA2
        or     dword ptr [eax], eax
        add    byte ptr [ecx + 9], ch        ; -> loc_1_0969
        add    byte ptr [eax], al
        or     byte ptr [ecx], 0
        add    byte ptr [ebx - 0x4bfffff7], bl ; -> loc_1_099B
        or     dword ptr [eax], eax
        add    ch, cl                        ; -> loc_1_09CD
        or     dword ptr [eax], eax
        add    dh, ah                        ; -> loc_1_09E6
        or     dword ptr [eax], eax
        add    bh, bh                        ; -> loc_1_09FF
        or     dword ptr [eax], eax
        add    byte ptr [eax], bl            ; -> loc_1_0A18
        or     al, byte ptr [eax]
        add    byte ptr [edx + 0x3100000a], ah ; -> loc_1_0AA2
        or     al, byte ptr [eax]
        add    bh, bh
        je     0xb3c
        add    al, 0xff
        adc    eax, 0x3568                   ; -> sub_1_3568
        add    esp, 4
        mov    eax, dword ptr [0x35e4]       ; -> loc_1_35E4
        dec    eax
        mov    dword ptr [0x35e4], eax       ; -> loc_1_35E4
        jne    loc_1_0B43
        mov    dword ptr [0x3574], 0         ; -> loc_1_3574
        mov    dword ptr [0x3644], 0xc25     ; -> sub_1_3644
loc_1_0B43:
        ret    
loc_1_0B44:
        mov    eax, dword ptr [0x35e4]       ; -> loc_1_35E4
        mov    ecx, eax
        dec    eax
        mov    dword ptr [0x35e4], eax       ; -> loc_1_35E4
        or     ecx, ecx
        je     loc_1_0B64
        push   dword ptr [esp + 4]
        call   sub_1_3568
        add    esp, 4
        jmp    loc_1_0B86
loc_1_0B64:
        mov    dword ptr [0x3574], 0         ; -> loc_1_3574
        cmp    byte ptr [esp + 4], 0xff
        jne    loc_1_0B81
        mov    dword ptr [0x3644], 0xc25     ; -> sub_1_3644
        jmp    loc_1_0B86
loc_1_0B81:
        call   sub_1_0643
loc_1_0B86:
        ret    
loc_1_0B87:
        movzx  eax, byte ptr [esp + 4]
        mov    dword ptr [0x35e4], eax       ; -> loc_1_35E4
        mov    dword ptr [0x3644], 0xb44     ; -> sub_1_3644
        mov    dword ptr [0x3574], 1         ; -> loc_1_3574
        ret    
loc_1_0BA6:
        cmp    byte ptr [esp + 4], 0xff
        jne    loc_1_0BB4
        call   sub_1_1449
        jmp    loc_1_0BB9
loc_1_0BB4:
        call   sub_1_0643
loc_1_0BB9:
        ret    
loc_1_0BBA:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        cmp    bl, 0xff
        jne    loc_1_0C1E
        mov    eax, 0x77a                    ; -> loc_1_077A
        or     eax, eax
        jne    loc_1_0BCF
        jmp    loc_1_0C1E
loc_1_0BCF:
        call   sub_1_07B0
        or     eax, eax
        je     loc_1_0BEE
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
        mov    dword ptr [0x3644], 0xc25     ; -> sub_1_3644
        jmp    loc_1_0C16
loc_1_0BEE:
        mov    dword ptr [0x3568], 0x883     ; -> sub_1_3568
        mov    dword ptr [0x3644], 0xe1a     ; -> sub_1_3644
        mov    dword ptr [0x3654], 0x11ce    ; -> sub_1_3654
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
loc_1_0C16:
        mov    byte ptr [0x3560], bl         ; -> loc_1_3560
        jmp    loc_1_0C23
loc_1_0C1E:
        call   sub_1_0643
loc_1_0C23:
        pop    ebx
        ret    
loc_1_0C25:
        push   ebx
        cmp    dword ptr [0x3620], 0         ; -> loc_1_3620
        je     loc_1_0C4F
        cmp    byte ptr [esp + 8], 0xfb
        jne    loc_1_0C45
        mov    dword ptr [0x3644], 0xba6     ; -> sub_1_3644
        jmp    loc_1_0DE4
loc_1_0C45:
        call   sub_1_0643
        jmp    loc_1_0DE4
loc_1_0C4F:
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        sub    eax, 0xf3
        cmp    eax, 0xc
        ja     loc_1_0C67
        jmp    loc_1_0DE6
loc_1_0C67:
        push   ebx
        call   sub_1_3568
        add    esp, 4
        jmp    loc_1_0DDE
loc_1_0C76:
        mov    dword ptr [0x3644], 0xb15     ; -> sub_1_3644
        mov    dword ptr [0x35e4], 8         ; -> loc_1_35E4
        mov    dword ptr [0x3574], 1         ; -> loc_1_3574
        push   ebx
        call   sub_1_3568
        add    esp, 4
        jmp    loc_1_0DDE
loc_1_0CA3:
        mov    dword ptr [0x3644], 0xb15     ; -> sub_1_3644
        mov    dword ptr [0x35e4], 4         ; -> loc_1_35E4
        mov    dword ptr [0x3574], 1         ; -> loc_1_3574
        push   ebx
        call   sub_1_3568
        add    esp, 4
        jmp    loc_1_0DDE
loc_1_0CD0:
        mov    dword ptr [0x3644], 0xb15     ; -> sub_1_3644
        mov    dword ptr [0x35e4], 2         ; -> loc_1_35E4
        mov    dword ptr [0x3574], 1         ; -> loc_1_3574
        push   ebx
        call   sub_1_3568
        add    esp, 4
        jmp    loc_1_0DDE
loc_1_0CFD:
        mov    dword ptr [0x3644], 0xb15     ; -> sub_1_3644
        mov    dword ptr [0x35e4], 1         ; -> loc_1_35E4
        mov    dword ptr [0x3574], 1         ; -> loc_1_3574
        push   ebx
        call   sub_1_3568
        add    esp, 4
        jmp    loc_1_0DDE
loc_1_0D2A:
        mov    dword ptr [0x3644], 0xb87     ; -> sub_1_3644
        push   ebx
        call   sub_1_3568
        add    esp, 4
        jmp    loc_1_0DDE
loc_1_0D43:
        cmp    byte ptr [0x3560], 0xff       ; -> loc_1_3560
        jne    loc_1_0D62
        mov    dword ptr [0x3644], 0xbba     ; -> sub_1_3644
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_0DDE
loc_1_0D62:
        call   sub_1_0643
        jmp    loc_1_0DE4
loc_1_0D69:
        cmp    dword ptr [0x3578], 0         ; -> loc_1_3578
        je     loc_1_0D84
        call   sub_1_0740
        or     eax, eax
        je     loc_1_0D7D
        jmp    loc_1_0DDE
loc_1_0D7D:
        call   sub_1_0643
        jmp    loc_1_0DE4
loc_1_0D84:
        call   sub_1_0643
        jmp    loc_1_0DE4
loc_1_0D8B:
        mov    dword ptr [0x3644], 0xba6     ; -> sub_1_3644
        jmp    loc_1_0DDE
loc_1_0D97:
        push   ebx
        call   sub_1_3568
        add    esp, 4
        call   sub_1_07B0
        or     eax, eax
        jne    loc_1_0DD4
        mov    dword ptr [0x3568], 0x883     ; -> sub_1_3568
        mov    dword ptr [0x3644], 0xe1a     ; -> sub_1_3644
        mov    dword ptr [0x3654], 0x11ce    ; -> sub_1_3654
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_0DDE
loc_1_0DD4:
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_0DDE:
        mov    byte ptr [0x3560], bl         ; -> loc_1_3560
loc_1_0DE4:
        pop    ebx
        ret    
loc_1_0DE6:
        jbe    0xdf4
        add    byte ptr [eax], al
        mov    dword ptr [0xd000000c], eax   ; -> loc_1_0CD0
        or     al, 0
        add    ch, bh                        ; -> loc_1_0CFD
        or     al, 0
        add    byte ptr [edx], ch            ; -> loc_1_0D2A
        or     eax, 0xc670000                ; -> loc_1_0C67
        add    byte ptr [eax], al
        inc    ebx
        or     eax, 0xd690000                ; -> loc_1_0D69
        add    byte ptr [eax], al
        mov    ecx, dword ptr [0xc670000]    ; -> loc_1_0C67
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [edi + 0xc], ah      ; -> loc_1_0C67
        add    byte ptr [eax], al
        xchg   edi, eax
        db  00Dh, 000h, 000h
loc_1_0E1A:
        push   ebx
        push   esi
        mov    cl, byte ptr [esp + 0xc]
        cmp    cl, 0x1b
        jne    loc_1_0E34
        mov    dword ptr [0x3644], 0xc25     ; -> sub_1_3644
        jmp    loc_1_0EF2
loc_1_0E34:
        cmp    dword ptr [0x3620], 0         ; -> loc_1_3620
        jne    loc_1_0EED
        cmp    cl, 0x7f
        ja     loc_1_0EED
        movzx  eax, cl
        lea    esi, [eax*2]
        mov    bl, byte ptr [esi + 0x2c38]   ; -> obj1:0x2C38
        or     bl, bl
        jne    loc_1_0E72
        call   sub_1_0652
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        jmp    loc_1_0EF2
loc_1_0E72:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    loc_1_0E9D
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        jne    loc_1_0E9D
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    byte ptr [eax + 0x35ec], bl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
loc_1_0E9D:
        mov    bl, byte ptr [esi + 0x2c39]   ; -> loc_1_2C39
        or     bl, bl
        je     loc_1_0ED2
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    loc_1_0ED2
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        jne    loc_1_0ED2
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    byte ptr [eax + 0x35ec], bl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
loc_1_0ED2:
        call   sub_1_1715
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3548], 0         ; -> loc_1_3548
        jmp    loc_1_0EF2
loc_1_0EED:
        call   sub_1_0643
loc_1_0EF2:
        pop    esi
        pop    ebx
        ret    
sub_1_0EF5:
        mov    dword ptr [0x34d0], 0x3604    ; -> loc_1_34D0
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_0563
        add    esp, 4
        cmp    eax, 0x14
        jle    loc_1_0F1E
        mov    eax, dword ptr [0x34d0]       ; -> loc_1_34D0
        mov    byte ptr [eax], 0xfd
        jmp    loc_1_0FE8
loc_1_0F1E:
        push   dword ptr [0x34d0]            ; -> loc_1_34D0
        call   sub_1_065C
        add    esp, 4
        or     eax, eax
        jne    loc_1_0FE8
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_0540
        add    esp, 4
        push   dword ptr [0x34d0]            ; -> loc_1_34D0
        call   sub_1_06A6
        add    esp, 4
        or     eax, eax
        jne    loc_1_0FE8
        movsx  eax, byte ptr [0x3604]        ; -> loc_1_3604
        cmp    eax, 0x2b
        je     loc_1_0F9F
        cmp    eax, 0x2d
        je     loc_1_0F9F
        cmp    eax, 0x30
        je     loc_1_0F9F
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_05EF
        add    esp, 4
        mov    dword ptr [0x34d4], eax       ; -> loc_1_34D4
        cmp    eax, 0xff
        jbe    loc_1_0F90
        mov    eax, dword ptr [0x34d0]       ; -> loc_1_34D0
        mov    byte ptr [eax], 0xfd
        jmp    loc_1_0FE8
loc_1_0F90:
        mov    eax, dword ptr [0x34d0]       ; -> loc_1_34D0
        mov    cl, byte ptr [0x34d4]         ; -> loc_1_34D4
        mov    byte ptr [eax], cl
        jmp    loc_1_0FE8
loc_1_0F9F:
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_05EF
        mov    dword ptr [0x34d4], eax       ; -> loc_1_34D4
        mov    eax, dword ptr [0x34d0]       ; -> loc_1_34D0
        add    esp, 4
        mov    byte ptr [eax], 0xf5
        push   dword ptr [0x34d0]            ; -> loc_1_34D0
        call   sub_1_07EA
        add    esp, 4
        mov    dword ptr [0x34d0], 0x34d4    ; -> loc_1_34D0
        mov    dword ptr [0x34d0], 0x34d5    ; -> loc_1_34D0
        push   0x34d4                        ; -> loc_1_34D4
        call   sub_1_07EA
        add    esp, 4
loc_1_0FE8:
        push   dword ptr [0x34d0]            ; -> loc_1_34D0
        call   sub_1_07EA
        add    esp, 4
        ret    
sub_1_0FF7:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        mov    dword ptr [0x34dc], 0x34d8    ; -> loc_1_34DC
        movzx  eax, bl
        cmp    eax, 0x20
        jg     loc_1_1025
        je     loc_1_1078
        cmp    eax, 9
        jl     loc_1_102F
        cmp    eax, 0xd
        jle    loc_1_1078
        cmp    eax, 0x1b
        je     loc_1_10A5
        jmp    loc_1_102F
loc_1_1025:
        cmp    eax, 0x2c
        je     loc_1_108D
        cmp    eax, 0x2e
        je     loc_1_108D
loc_1_102F:
        cmp    bl, 0x20
        jb     loc_1_106E
        cmp    bl, 0x7e
        ja     loc_1_106E
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_0563
        add    esp, 4
        cmp    eax, 0x15
        jge    loc_1_1117
        mov    byte ptr [0x34e0], bl         ; -> loc_1_34E0
        push   1
        push   0x34e0                        ; -> loc_1_34E0
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_05BD
        add    esp, 0xc
        jmp    loc_1_1117
loc_1_106E:
        call   sub_1_0643
        jmp    loc_1_1117
loc_1_1078:
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        je     loc_1_1117
loc_1_108D:
        push   0x3604                        ; -> loc_1_3604
        call   sub_1_0563
        add    esp, 4
        or     eax, eax
        je     loc_1_10AE
        call   sub_1_0EF5
        jmp    loc_1_10C3
loc_1_10A5:
        mov    byte ptr [0x3604], 0          ; -> loc_1_3604
        jmp    loc_1_1117
loc_1_10AE:
        mov    byte ptr [0x34d8], 0xfc       ; -> loc_1_34D8
        push   dword ptr [0x34dc]            ; -> loc_1_34DC
        call   sub_1_07EA
        add    esp, 4
loc_1_10C3:
        cmp    bl, 0x2e
        jne    loc_1_10DD
        mov    byte ptr [0x34d8], 0xff       ; -> loc_1_34D8
        push   dword ptr [0x34dc]            ; -> loc_1_34DC
        call   sub_1_07EA
        add    esp, 4
loc_1_10DD:
        mov    byte ptr [0x3604], 0          ; -> loc_1_3604
        push   dword ptr [0x34dc]            ; -> loc_1_34DC
        call   sub_1_0840
        add    esp, 4
        or     eax, eax
        je     loc_1_1117
loc_1_10F6:
        mov    al, byte ptr [0x34d8]         ; -> loc_1_34D8
        push   eax
        call   sub_1_3644
        add    esp, 4
        push   dword ptr [0x34dc]            ; -> loc_1_34DC
        call   sub_1_0840
        add    esp, 4
        or     eax, eax
        jne    loc_1_10F6
loc_1_1117:
        pop    ebx
        ret    
loc_1_1119:
        push   dword ptr [esp + 4]
        call   sub_1_3644
        add    esp, 4
        ret    
loc_1_1127:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0x1b
        je     loc_1_11CC
        cmp    eax, 0x2c
        je     loc_1_11B8
        cmp    bl, 0x20
        jb     loc_1_11A2
        cmp    bl, 0x7e
        ja     loc_1_11A2
        mov    dword ptr [0x3654], 0xff7     ; -> sub_1_3654
        mov    dword ptr [0x3634], 0x2a98    ; -> loc_1_3634
        push   0x29b8                        ; -> loc_1_29B8
        push   0x34e4                        ; -> loc_1_34E4
        call   sub_1_0575
        add    esp, 8
        mov    dword ptr [0x34ec], eax       ; -> loc_1_34EC
        cmp    byte ptr [eax], 0
        je     loc_1_1197
loc_1_1177:
        mov    eax, dword ptr [0x34ec]       ; -> loc_1_34EC
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_0FF7
        inc    dword ptr [0x34ec]            ; -> loc_1_34EC
        mov    eax, dword ptr [0x34ec]       ; -> loc_1_34EC
        add    esp, 4
        cmp    byte ptr [eax], 0
        jne    loc_1_1177
loc_1_1197:
        push   ebx
        call   sub_1_0FF7
        add    esp, 4
        jmp    loc_1_11CC
loc_1_11A2:
        mov    dword ptr [0x3654], 0x1119    ; -> sub_1_3654
        push   ebx
        call   sub_1_3644
        add    esp, 4
        jmp    loc_1_11CC
loc_1_11B8:
        mov    dword ptr [0x3654], 0xff7     ; -> sub_1_3654
        mov    dword ptr [0x3634], 0x2a98    ; -> loc_1_3634
loc_1_11CC:
        pop    ebx
        ret    
loc_1_11CE:
        mov    cl, byte ptr [esp + 4]
        cmp    cl, 0x1b
        jne    loc_1_11F5
        mov    dword ptr [0x3654], 0x1127    ; -> sub_1_3654
        mov    dword ptr [0x3568], 0x883     ; -> sub_1_3568
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_11F5:
        push   ecx
        call   sub_1_3644
        add    esp, 4
        ret    
sub_1_1200:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        mov    byte ptr [0x34f0], bl         ; -> loc_1_34F0
        mov    byte ptr [0x34f1], 0          ; -> obj1:0x34F1
        cmp    dword ptr [0x3574], 0         ; -> loc_1_3574
        jne    loc_1_1255
        or     bl, bl
        jne    loc_1_1255
        call   sub_1_0400
        mov    eax, dword ptr [0x3640]       ; -> loc_1_3640
        inc    eax
        mov    dword ptr [0x3640], eax       ; -> loc_1_3640
        cmp    eax, 3
        jl     loc_1_1255
        call   sub_1_1449
        push   0xff11
        call   sub_1_0256
        add    esp, 4
        call   sub_1_0400
        call   sub_1_0400
        call   sub_1_0400
loc_1_1255:
        push   ebx
        call   sub_1_3654
        add    esp, 4
        mov    eax, 1
        pop    ebx
        ret    
sub_1_1266:
        push   ebx
        cmp    dword ptr [0x356c], 0         ; -> loc_1_356C
        je     loc_1_1335
        push   1
        push   0x3544                        ; -> loc_1_3544
        push   dword ptr [0x29a8]            ; -> obj1:0x29A8
        call   sub_1_02B3
        add    esp, 0xc
        or     eax, eax
        jle    loc_1_12A1
        mov    al, byte ptr [0x3544]         ; -> loc_1_3544
        push   eax
        call   sub_1_1200
        add    esp, 4
        or     eax, eax
        jne    loc_1_1306
        jmp    loc_1_1301
loc_1_12A1:
        push   0x3624                        ; -> loc_1_3624
        push   dword ptr [0x29a8]            ; -> obj1:0x29A8
        call   sub_1_02F2
        add    esp, 8
        mov    ebx, eax
        test   bl, 0x10
        je     loc_1_12C9
        push   dword ptr [0x29a8]            ; -> obj1:0x29A8
        call   sub_1_0317
        add    esp, 4
loc_1_12C9:
        test   bl, 8
        je     loc_1_1306
        call   sub_1_0400
        mov    eax, dword ptr [0x29ac]       ; -> obj1:0x29AC
        inc    eax
        mov    dword ptr [0x29ac], eax       ; -> obj1:0x29AC
        cmp    eax, 3
        jl     loc_1_1306
        mov    dword ptr [0x29ac], 0         ; -> obj1:0x29AC
        mov    dword ptr [0x34a4], 0xff11    ; -> obj1:0x34A4
        mov    dword ptr [0x29b0], 1         ; -> obj1:0x29B0
loc_1_1301:
        call   sub_1_1449
loc_1_1306:
        cmp    dword ptr [0x29b0], 0         ; -> obj1:0x29B0
        je     loc_1_1335
        mov    dword ptr [0x29b0], 0         ; -> obj1:0x29B0
        push   dword ptr [0x34a4]            ; -> obj1:0x34A4
        call   sub_1_0256
        add    esp, 4
        or     eax, eax
        je     loc_1_1335
        mov    dword ptr [0x29b4], 1         ; -> obj1:0x29B4
loc_1_1335:
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3658               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3650               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x34B4              ; fixup
        db  002h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x34B8              ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x34BC              ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x34C0              ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  obj1:0x34C8              ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_34CC               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3578               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3620               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3570               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3564               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3574               ; fixup
        db  000h, 000h, 000h, 000h, 0C6h, 005h
        dd  loc_1_3560               ; fixup
        db  000h, 0C7h, 005h
        dd  loc_1_35E4               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3640               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_35E8               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3548               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3664               ; fixup
        db  000h, 000h, 000h, 000h, 0C6h, 005h
        dd  loc_1_3604               ; fixup
        db  000h, 0C7h, 005h
        dd  loc_1_3630               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_3600               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  loc_1_361C               ; fixup
        db  000h, 000h, 000h, 000h, 0C7h, 005h
        dd  sub_1_3654               ; fixup
        dd  loc_1_11CE               ; fixup
        db  0C7h, 005h
        dd  sub_1_3644               ; fixup
        dd  loc_1_0E1A               ; fixup
        db  0C7h, 005h
        dd  sub_1_3568               ; fixup
        dd  loc_1_0883               ; fixup
        db  0C7h, 005h
        dd  loc_1_3634               ; fixup
        dd  obj1:0x34AC              ; fixup
        db  0C3h
sub_1_1449:
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        mov    dword ptr [0x3620], 0         ; -> loc_1_3620
        mov    dword ptr [0x3570], 0         ; -> loc_1_3570
        mov    dword ptr [0x3564], 0         ; -> loc_1_3564
        mov    dword ptr [0x3574], 0         ; -> loc_1_3574
        mov    byte ptr [0x3560], 0          ; -> loc_1_3560
        mov    dword ptr [0x35e4], 0         ; -> loc_1_35E4
        mov    dword ptr [0x3640], 0         ; -> loc_1_3640
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    byte ptr [0x3604], 0          ; -> loc_1_3604
        mov    dword ptr [0x3630], 0         ; -> loc_1_3630
        mov    dword ptr [0x3600], 0         ; -> loc_1_3600
        mov    dword ptr [0x361c], 0         ; -> loc_1_361C
        mov    dword ptr [0x3654], 0x11ce    ; -> sub_1_3654
        mov    dword ptr [0x3644], 0xe1a     ; -> sub_1_3644
        mov    dword ptr [0x3568], 0x883     ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x34ac    ; -> loc_1_3634
        mov    dword ptr [0x34b4], 2         ; -> obj1:0x34B4
        mov    dword ptr [0x34b8], 0         ; -> obj1:0x34B8
        mov    dword ptr [0x34bc], 0         ; -> obj1:0x34BC
        mov    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        ret    
sub_1_1516:
        mov    dword ptr [0x3658], 0         ; -> loc_1_3658
        mov    dword ptr [0x3650], 0         ; -> loc_1_3650
        mov    dword ptr [0x34b4], 2         ; -> obj1:0x34B4
        mov    dword ptr [0x34b8], 0         ; -> obj1:0x34B8
        mov    dword ptr [0x34bc], 0         ; -> obj1:0x34BC
        mov    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        mov    dword ptr [0x34c8], 0         ; -> obj1:0x34C8
        mov    dword ptr [0x34cc], 0         ; -> loc_1_34CC
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        mov    dword ptr [0x3620], 0         ; -> loc_1_3620
        mov    dword ptr [0x3570], 0         ; -> loc_1_3570
        mov    dword ptr [0x3564], 0         ; -> loc_1_3564
        mov    dword ptr [0x3574], 0         ; -> loc_1_3574
        mov    byte ptr [0x3560], 0          ; -> loc_1_3560
        mov    dword ptr [0x35e4], 0         ; -> loc_1_35E4
        mov    dword ptr [0x3640], 0         ; -> loc_1_3640
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3548], 0         ; -> loc_1_3548
        mov    dword ptr [0x3664], 0         ; -> loc_1_3664
        mov    byte ptr [0x3604], 0          ; -> loc_1_3604
        mov    dword ptr [0x3630], 0         ; -> loc_1_3630
        mov    dword ptr [0x3600], 0         ; -> loc_1_3600
        mov    dword ptr [0x361c], 0         ; -> loc_1_361C
        mov    dword ptr [0x3654], 0x11ce    ; -> sub_1_3654
        mov    dword ptr [0x3644], 0xe1a     ; -> sub_1_3644
        mov    dword ptr [0x3568], 0x883     ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x34ac    ; -> loc_1_3634
        ret    
        db  000h
sub_1_1620:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0x10]
        or     edx, edx
        je     loc_1_1642
        mov    ecx, dword ptr [esp + 0xc]
loc_1_1631:
        movsx  esi, byte ptr [eax]
        cmp    esi, ecx
        je     loc_1_163E
        dec    edx
        inc    eax
        or     edx, edx
        jne    loc_1_1631
loc_1_163E:
        or     edx, edx
        jne    loc_1_1644
loc_1_1642:
        sub    eax, eax
loc_1_1644:
        pop    esi
        ret    
sub_1_1646:
        mov    al, byte ptr [esp + 4]
        or     al, al
        je     loc_1_1665
        movzx  eax, al
        mov    eax, dword ptr [eax*4 + 0x32a4] ; -> loc_1_32A4
        or     eax, eax
        je     loc_1_1665
        push   eax
        call   sub_1_0372
        add    esp, 4
loc_1_1665:
        ret    
sub_1_1666:
        mov    al, byte ptr [esp + 4]
        or     al, al
        je     loc_1_168A
        movzx  eax, al
        mov    eax, dword ptr [eax*4 + 0x32a4] ; -> loc_1_32A4
        or     eax, eax
        je     loc_1_168A
        or     eax, 0x80
        push   eax
        call   sub_1_0372
        add    esp, 4
loc_1_168A:
        ret    
sub_1_168B:
        push   esi
        sub    esi, esi
        cmp    dword ptr [0x3548], esi       ; -> loc_1_3548
        jle    loc_1_16B0
loc_1_1696:
        mov    eax, esi
        inc    esi
        mov    al, byte ptr [eax + 0x354c]   ; -> loc_1_354C
        push   eax
        call   sub_1_1646
        add    esp, 4
        cmp    esi, dword ptr [0x3548]       ; -> loc_1_3548
        jl     loc_1_1696
loc_1_16B0:
        sub    esi, esi
        cmp    dword ptr [0x35e8], esi       ; -> loc_1_35E8
        jle    loc_1_16E1
loc_1_16BA:
        mov    al, byte ptr [esi + 0x35ec]   ; -> loc_1_35EC
        push   eax
        call   sub_1_1646
        add    esp, 4
        mov    al, byte ptr [esi + 0x35ec]   ; -> loc_1_35EC
        push   eax
        call   sub_1_1666
        add    esp, 4
        inc    esi
        cmp    esi, dword ptr [0x35e8]       ; -> loc_1_35E8
        jl     loc_1_16BA
loc_1_16E1:
        mov    esi, dword ptr [0x3548]       ; -> loc_1_3548
        or     esi, esi
        jle    loc_1_16FF
loc_1_16EB:
        mov    al, byte ptr [esi + 0x354b]   ; -> obj1:0x354B
        dec    esi
        push   eax
        call   sub_1_1666
        add    esp, 4
        or     esi, esi
        jg     loc_1_16EB
loc_1_16FF:
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3548], 0         ; -> loc_1_3548
        pop    esi
        ret    
sub_1_1715:
        push   esi
        sub    esi, esi
        cmp    dword ptr [0x3548], esi       ; -> loc_1_3548
        jle    loc_1_173A
loc_1_1720:
        mov    eax, esi
        inc    esi
        mov    al, byte ptr [eax + 0x354c]   ; -> loc_1_354C
        push   eax
        call   sub_1_1646
        add    esp, 4
        cmp    dword ptr [0x3548], esi       ; -> loc_1_3548
        jg     loc_1_1720
loc_1_173A:
        sub    esi, esi
        cmp    dword ptr [0x35e8], esi       ; -> loc_1_35E8
        jle    loc_1_175E
loc_1_1744:
        mov    eax, esi
        inc    esi
        mov    al, byte ptr [eax + 0x35ec]   ; -> loc_1_35EC
        push   eax
        call   sub_1_1646
        add    esp, 4
        cmp    dword ptr [0x35e8], esi       ; -> loc_1_35E8
        jg     loc_1_1744
loc_1_175E:
        mov    esi, dword ptr [0x35e8]       ; -> loc_1_35E8
        or     esi, esi
        jle    loc_1_177C
loc_1_1768:
        mov    al, byte ptr [esi + 0x35eb]   ; -> obj1:0x35EB
        dec    esi
        push   eax
        call   sub_1_1666
        add    esp, 4
        or     esi, esi
        jg     loc_1_1768
loc_1_177C:
        mov    esi, dword ptr [0x3548]       ; -> loc_1_3548
        or     esi, esi
        jle    loc_1_179A
loc_1_1786:
        mov    al, byte ptr [esi + 0x354b]   ; -> obj1:0x354B
        dec    esi
        push   eax
        call   sub_1_1666
        add    esp, 4
        or     esi, esi
        jg     loc_1_1786
loc_1_179A:
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3548], 0         ; -> loc_1_3548
        pop    esi
        ret    
sub_1_17B0:
        movzx  eax, byte ptr [esp + 4]
        push   dword ptr [0x3664]            ; -> loc_1_3664
        push   eax
        push   0x3668                        ; -> loc_1_3668
        call   sub_1_1620
        add    esp, 0xc
        or     eax, eax
        mov    eax, 0
        je     loc_1_17D7
        mov    eax, 1
loc_1_17D7:
        ret    
sub_1_17D8:
        movzx  eax, byte ptr [esp + 4]
        push   dword ptr [0x3548]            ; -> loc_1_3548
        push   eax
        push   0x354c                        ; -> loc_1_354C
        call   sub_1_1620
        add    esp, 0xc
        or     eax, eax
        mov    eax, 0
        je     loc_1_17FF
        mov    eax, 1
loc_1_17FF:
        ret    
sub_1_1800:
        movzx  eax, byte ptr [esp + 4]
        push   dword ptr [0x35e8]            ; -> loc_1_35E8
        push   eax
        push   0x35ec                        ; -> loc_1_35EC
        call   sub_1_1620
        add    esp, 0xc
        or     eax, eax
        mov    eax, 0
        je     loc_1_1827
        mov    eax, 1
loc_1_1827:
        ret    
sub_1_1828:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        je     loc_1_1893
        mov    esi, 0
        mov    eax, 0
        jle    loc_1_1880
        sub    edi, edi
loc_1_1842:
        mov    bl, byte ptr [eax + 0x35ec]   ; -> loc_1_35EC
        cmp    bl, 0xfc
        je     loc_1_1880
        mov    ecx, edi
        cmp    dword ptr [0x3548], edi       ; -> loc_1_3548
        jle    loc_1_1871
loc_1_1857:
        mov    dl, byte ptr [ecx + 0x354c]   ; -> loc_1_354C
        mov    byte ptr [esi + 0x354c], dl   ; -> loc_1_354C
        cmp    bl, dl
        je     loc_1_1868
        inc    esi
loc_1_1868:
        inc    ecx
        cmp    ecx, dword ptr [0x3548]       ; -> loc_1_3548
        jl     loc_1_1857
loc_1_1871:
        mov    dword ptr [0x3548], esi       ; -> loc_1_3548
        inc    eax
        cmp    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        jl     loc_1_1842
loc_1_1880:
        cmp    byte ptr [eax + 0x35ec], 0xfc ; -> loc_1_35EC
        jne    loc_1_1893
        mov    dword ptr [0x3548], 0         ; -> loc_1_3548
loc_1_1893:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1897:
        push   ebx
        mov    dl, byte ptr [esp + 8]
        or     dl, dl
        je     loc_1_18CC
        sub    ecx, ecx
        sub    eax, eax
        cmp    dword ptr [0x3548], eax       ; -> loc_1_3548
        jle    loc_1_18C6
loc_1_18AC:
        mov    bl, byte ptr [eax + 0x354c]   ; -> loc_1_354C
        mov    byte ptr [ecx + 0x354c], bl   ; -> loc_1_354C
        cmp    dl, bl
        je     loc_1_18BD
        inc    ecx
loc_1_18BD:
        inc    eax
        cmp    eax, dword ptr [0x3548]       ; -> loc_1_3548
        jl     loc_1_18AC
loc_1_18C6:
        mov    dword ptr [0x3548], ecx       ; -> loc_1_3548
loc_1_18CC:
        pop    ebx
        ret    
sub_1_18CE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        je     loc_1_1973
        mov    edi, 0
        mov    ebp, 0
        jle    loc_1_1934
loc_1_18EB:
        mov    bl, byte ptr [ebp + 0x35ec]   ; -> loc_1_35EC
        cmp    bl, 0xfc
        je     loc_1_1934
        sub    esi, esi
        cmp    dword ptr [0x3664], esi       ; -> loc_1_3664
        jle    loc_1_1925
loc_1_1900:
        mov    al, byte ptr [esi + 0x3668]   ; -> loc_1_3668
        mov    byte ptr [edi + 0x3668], al   ; -> loc_1_3668
        cmp    al, bl
        je     loc_1_1913
        inc    edi
        jmp    loc_1_191C
loc_1_1913:
        push   ebx
        call   sub_1_1666
        add    esp, 4
loc_1_191C:
        inc    esi
        cmp    dword ptr [0x3664], esi       ; -> loc_1_3664
        jg     loc_1_1900
loc_1_1925:
        mov    dword ptr [0x3548], edi       ; -> loc_1_3548
        inc    ebp
        cmp    ebp, dword ptr [0x35e8]       ; -> loc_1_35E8
        jl     loc_1_18EB
loc_1_1934:
        cmp    byte ptr [ebp + 0x35ec], 0xfc ; -> loc_1_35EC
        jne    loc_1_1973
        sub    esi, esi
        cmp    dword ptr [0x3664], esi       ; -> loc_1_3664
        jle    loc_1_195F
loc_1_1947:
        mov    al, byte ptr [esi + 0x3668]   ; -> loc_1_3668
        push   eax
        call   sub_1_1666
        add    esp, 4
        inc    esi
        cmp    esi, dword ptr [0x3664]       ; -> loc_1_3664
        jl     loc_1_1947
loc_1_195F:
        mov    dword ptr [0x3664], 0         ; -> loc_1_3664
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
loc_1_1973:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_1978:
        ret    
loc_1_1979:
        ret    
loc_1_197A:
        ret    
        db  0C3h
loc_1_197C:
        ret    
loc_1_197D:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0xfd
        je     loc_1_1A33
        cmp    eax, 0xff
        je     loc_1_1A58
        cmp    bl, 0xec
        jb     loc_1_19AF
        cmp    bl, 0xfc
        je     loc_1_19AF
        call   sub_1_0643
        jmp    loc_1_1A82
loc_1_19AF:
        cmp    dword ptr [0x35e8], 0x14      ; -> loc_1_35E8
        jl     loc_1_19CC
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_1A78
loc_1_19CC:
        cmp    bl, 0xfc
        jne    loc_1_19D5
        mov    bl, 0xfc
        jmp    loc_1_1A09
loc_1_19D5:
        or     bl, bl
        jne    loc_1_19ED
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_1A78
loc_1_19ED:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    loc_1_1A07
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     loc_1_1A09
loc_1_1A07:
        sub    bl, bl
loc_1_1A09:
        push   ebx
        call   sub_1_1800
        add    esp, 4
        or     eax, eax
        jne    loc_1_1A27
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    byte ptr [eax + 0x35ec], bl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
loc_1_1A27:
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_1A82
loc_1_1A33:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
        jmp    loc_1_1A82
loc_1_1A58:
        cmp    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        jne    loc_1_1A6E
        mov    byte ptr [0x35ec], 0xfc       ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
loc_1_1A6E:
        call   sub_1_18CE
        call   sub_1_1828
loc_1_1A78:
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
loc_1_1A82:
        pop    ebx
        ret    
loc_1_1A84:
        push   ebx
        push   esi
        mov    bl, byte ptr [esp + 0xc]
        movzx  eax, bl
        cmp    eax, 0xfd
        je     loc_1_1B2A
        cmp    eax, 0xff
        je     loc_1_1B4F
        cmp    bl, 0xec
        jb     loc_1_1AB2
        call   sub_1_0643
        jmp    loc_1_1BB4
loc_1_1AB2:
        mov    eax, dword ptr [0x3664]       ; -> loc_1_3664
        add    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        cmp    eax, 0x14
        jl     loc_1_1AD6
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_1BAA
loc_1_1AD6:
        or     bl, bl
        jne    loc_1_1AEE
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_1BAA
loc_1_1AEE:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     loc_1_1AFD
        sub    bl, bl
loc_1_1AFD:
        push   ebx
        call   sub_1_1800
        add    esp, 4
        or     eax, eax
        jne    loc_1_1B1B
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    byte ptr [eax + 0x35ec], bl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
loc_1_1B1B:
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_1BB4
loc_1_1B2A:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
        jmp    loc_1_1BB4
loc_1_1B4F:
        sub    esi, esi
        cmp    dword ptr [0x35e8], esi       ; -> loc_1_35E8
        jle    loc_1_1B9C
loc_1_1B59:
        mov    bl, byte ptr [esi + 0x35ec]   ; -> loc_1_35EC
        or     bl, bl
        je     loc_1_1B93
        mov    eax, dword ptr [0x3664]       ; -> loc_1_3664
        push   ebx
        mov    byte ptr [eax + 0x3668], bl   ; -> loc_1_3668
        inc    dword ptr [0x3664]            ; -> loc_1_3664
        call   sub_1_1646
        add    esp, 4
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     loc_1_1B93
        push   ebx
        call   sub_1_1897
        add    esp, 4
loc_1_1B93:
        inc    esi
        cmp    dword ptr [0x35e8], esi       ; -> loc_1_35E8
        jg     loc_1_1B59
loc_1_1B9C:
        cmp    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        jne    loc_1_1BAA
        call   sub_1_0652
loc_1_1BAA:
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
loc_1_1BB4:
        pop    esi
        pop    ebx
        ret    
loc_1_1BB7:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0xfd
        je     loc_1_1C66
        cmp    eax, 0xff
        je     loc_1_1C8B
        cmp    bl, 0xec
        jb     loc_1_1BE4
        call   sub_1_0643
        jmp    loc_1_1CDC
loc_1_1BE4:
        mov    eax, dword ptr [0x3548]       ; -> loc_1_3548
        add    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        cmp    eax, 0x14
        jl     loc_1_1C08
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_1CD2
loc_1_1C08:
        or     bl, bl
        jne    loc_1_1C20
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_1CD2
loc_1_1C20:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    loc_1_1C3A
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     loc_1_1C3C
loc_1_1C3A:
        sub    bl, bl
loc_1_1C3C:
        push   ebx
        call   sub_1_1800
        add    esp, 4
        or     eax, eax
        jne    loc_1_1C5A
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    byte ptr [eax + 0x35ec], bl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
loc_1_1C5A:
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_1CDC
loc_1_1C66:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
        jmp    loc_1_1CDC
loc_1_1C8B:
        sub    ecx, ecx
        cmp    dword ptr [0x35e8], ecx       ; -> loc_1_35E8
        jle    loc_1_1CC4
        sub    edx, edx
loc_1_1C97:
        mov    eax, dword ptr [0x3548]       ; -> loc_1_3548
        mov    bl, byte ptr [ecx + 0x35ec]   ; -> loc_1_35EC
        mov    byte ptr [eax + 0x354c], bl   ; -> loc_1_354C
        mov    eax, dword ptr [0x3548]       ; -> loc_1_3548
        cmp    byte ptr [eax + 0x354c], dl   ; -> loc_1_354C
        je     loc_1_1CBB
        inc    dword ptr [0x3548]            ; -> loc_1_3548
loc_1_1CBB:
        inc    ecx
        cmp    ecx, dword ptr [0x35e8]       ; -> loc_1_35E8
        jl     loc_1_1C97
loc_1_1CC4:
        cmp    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        jne    loc_1_1CD2
        call   sub_1_0652
loc_1_1CD2:
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
loc_1_1CDC:
        pop    ebx
        ret    
loc_1_1CDE:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0xfd
        je     loc_1_1D9A
        cmp    eax, 0xff
        je     loc_1_1DBF
        cmp    bl, 0xec
        jb     loc_1_1D0B
        call   sub_1_0643
        jmp    loc_1_1DD8
loc_1_1D0B:
        cmp    dword ptr [0x35e8], 0x14      ; -> loc_1_35E8
        jl     loc_1_1D32
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        jmp    loc_1_1DD8
loc_1_1D32:
        or     bl, bl
        jne    loc_1_1D54
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        jmp    loc_1_1DD8
loc_1_1D54:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    loc_1_1D6E
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     loc_1_1D70
loc_1_1D6E:
        sub    bl, bl
loc_1_1D70:
        push   ebx
        call   sub_1_1800
        add    esp, 4
        or     eax, eax
        jne    loc_1_1D8E
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    byte ptr [eax + 0x35ec], bl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
loc_1_1D8E:
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_1DD8
loc_1_1D9A:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
        jmp    loc_1_1DD8
loc_1_1DBF:
        call   sub_1_1715
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3548], 0         ; -> loc_1_3548
loc_1_1DD8:
        pop    ebx
        ret    
loc_1_1DDA:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        movzx  eax, bl
        cmp    eax, 0xfd
        je     loc_1_1E86
        cmp    eax, 0xff
        je     loc_1_1EAB
        cmp    bl, 0xec
        jb     loc_1_1E07
        call   sub_1_0643
        jmp    loc_1_1EC4
loc_1_1E07:
        cmp    dword ptr [0x35e8], 0x14      ; -> loc_1_35E8
        jl     loc_1_1E2E
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        jmp    loc_1_1EC4
loc_1_1E2E:
        or     bl, bl
        jne    loc_1_1E4D
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        jmp    loc_1_1EC4
loc_1_1E4D:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        jne    loc_1_1E67
        push   ebx
        call   sub_1_17D8
        add    esp, 4
        or     eax, eax
        je     loc_1_1E69
loc_1_1E67:
        sub    bl, bl
loc_1_1E69:
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        mov    byte ptr [eax + 0x35ec], bl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
        jmp    loc_1_1EC4
loc_1_1E86:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
        jmp    loc_1_1EC4
loc_1_1EAB:
        call   sub_1_168B
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3548], 0         ; -> loc_1_3548
loc_1_1EC4:
        pop    ebx
        ret    
loc_1_1EC6:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 6
        je     loc_1_1EFE
        cmp    eax, 7
        je     loc_1_1F14
        cmp    eax, 8
        je     loc_1_1F2A
        cmp    eax, 9
        je     loc_1_1F40
        cmp    cl, 0xec
        jae    loc_1_1EED
        call   sub_1_0643
        jmp    loc_1_1F5E
loc_1_1EED:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_1F54
loc_1_1EFE:
        mov    dword ptr [0x3568], 0x1978    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2b7c    ; -> loc_1_3634
        jmp    loc_1_1F54
loc_1_1F14:
        mov    dword ptr [0x3568], 0x1979    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2b70    ; -> loc_1_3634
        jmp    loc_1_1F54
loc_1_1F2A:
        mov    dword ptr [0x3568], 0x197c    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2b4c    ; -> loc_1_3634
        jmp    loc_1_1F54
loc_1_1F40:
        mov    dword ptr [0x3568], 0x197a    ; -> sub_1_3568
        mov    dword ptr [0x3634], 0x2b64    ; -> loc_1_3634
loc_1_1F54:
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_1F5E:
        ret    
        db  000h
sub_1_1F60:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0xc]
        test   al, 1
        jne    loc_1_1F9E
        test   al, 2
        je     loc_1_1F77
        or     dword ptr [0x29c4], 0x20      ; -> obj1:0x29C4
loc_1_1F77:
        test   byte ptr [esi + 0xc], 8
        je     loc_1_1F84
        or     dword ptr [0x29c4], 0x10      ; -> obj1:0x29C4
loc_1_1F84:
        test   byte ptr [esi + 0xc], 4
        je     loc_1_1F91
        and    dword ptr [0x29c4], 0xffffffdf ; -> obj1:0x29C4
loc_1_1F91:
        test   byte ptr [esi + 0xc], 0x10
        je     loc_1_1F9E
        and    dword ptr [0x29c4], 0xffffffef ; -> obj1:0x29C4
loc_1_1F9E:
        push   dword ptr [0x29c4]            ; -> obj1:0x29C4
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_1_038E
        add    esp, 0x10
        mov    dword ptr [esi + 8], 0
        mov    dword ptr [esi + 4], 0
        pop    esi
        ret    
loc_1_1FC4:
        cmp    byte ptr [esp + 4], 0xff
        jne    loc_1_2019
        mov    dword ptr [0x34c0], 0x14      ; -> obj1:0x34C0
        push   dword ptr [0x34c4]            ; -> obj1:0x34C4
        call   sub_1_1F60
        add    esp, 4
        mov    dword ptr [0x29c4], 0         ; -> obj1:0x29C4
        push   0
        push   0xfffff830
        push   0xfffff830
        push   2
        call   sub_1_038E
        add    esp, 0x10
        mov    dword ptr [0x3658], 0         ; -> loc_1_3658
        mov    dword ptr [0x3650], 0         ; -> loc_1_3650
        jmp    loc_1_2028
loc_1_2019:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
loc_1_2028:
        ret    
sub_1_2029:
        push   esi
        push   edi
        movzx  eax, byte ptr [0x35ec]        ; -> loc_1_35EC
        movzx  ecx, byte ptr [0x35ed]        ; -> obj1:0x35ED
        shl    ecx, 8
        lea    esi, [eax + ecx]
        movzx  ecx, byte ptr [0x35ef]        ; -> obj1:0x35EF
        movzx  eax, byte ptr [0x35ee]        ; -> loc_1_35EE
        shl    ecx, 8
        lea    edi, [eax + ecx]
        mov    dword ptr [0x3650], esi       ; -> loc_1_3650
        mov    dword ptr [0x3658], edi       ; -> loc_1_3658
        push   dword ptr [0x29c4]            ; -> obj1:0x29C4
        push   0xfffff830
        push   0xfffff830
        push   2
        call   sub_1_038E
        add    esp, 0x10
        push   dword ptr [0x29c4]            ; -> obj1:0x29C4
        push   esi
        push   edi
        push   2
        call   sub_1_038E
        add    esp, 0x10
        pop    edi
        pop    esi
        ret    
sub_1_208E:
        push   esi
        movzx  eax, byte ptr [0x35ec]        ; -> loc_1_35EC
        movzx  ecx, byte ptr [0x35ed]        ; -> obj1:0x35ED
        shl    ecx, 8
        lea    edx, [eax + ecx]
        movzx  ecx, byte ptr [0x35ef]        ; -> obj1:0x35EF
        movzx  eax, byte ptr [0x35ee]        ; -> loc_1_35EE
        shl    ecx, 8
        lea    esi, [eax + ecx]
        mov    ecx, dword ptr [0x3650]       ; -> loc_1_3650
        add    ecx, edx
        mov    eax, dword ptr [0x3658]       ; -> loc_1_3658
        add    eax, esi
        or     ecx, ecx
        jge    loc_1_20CC
        sub    ecx, ecx
loc_1_20CC:
        or     eax, eax
        jge    loc_1_20D2
        sub    eax, eax
loc_1_20D2:
        mov    dword ptr [0x3650], ecx       ; -> loc_1_3650
        mov    dword ptr [0x3658], eax       ; -> loc_1_3658
        mov    dword ptr [0x34c0], 1         ; -> obj1:0x34C0
        mov    dword ptr [0x34b8], esi       ; -> obj1:0x34B8
        mov    dword ptr [0x34bc], edx       ; -> obj1:0x34BC
        push   dword ptr [0x34c4]            ; -> obj1:0x34C4
        call   sub_1_1F60
        add    esp, 4
        pop    esi
        ret    
loc_1_2103:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 0xff
        je     loc_1_2116
        call   sub_1_0643
        jmp    loc_1_217B
loc_1_2116:
        cmp    dword ptr [0x29c0], 0         ; -> obj1:0x29C0
        je     loc_1_215D
        mov    eax, dword ptr [0x35e0]       ; -> loc_1_35E0
        cmp    dword ptr [0x3618], eax       ; -> loc_1_3618
        jne    loc_1_214C
        mov    eax, dword ptr [0x365c]       ; -> loc_1_365C
        mov    dword ptr [0x3650], eax       ; -> loc_1_3650
        mov    eax, dword ptr [0x3660]       ; -> loc_1_3660
        mov    dword ptr [0x3658], eax       ; -> loc_1_3658
        mov    dword ptr [0x29c0], 0         ; -> obj1:0x29C0
        jmp    loc_1_2167
loc_1_214C:
        call   sub_1_0400
        mov    dword ptr [0x29c0], 0         ; -> obj1:0x29C0
        jmp    loc_1_2167
loc_1_215D:
        mov    dword ptr [0x29c0], 1         ; -> obj1:0x29C0
loc_1_2167:
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_217B:
        ret    
loc_1_217C:
        cmp    dword ptr [0x35e8], 4         ; -> loc_1_35E8
        jl     loc_1_218C
        call   sub_1_0643
        jmp    loc_1_21BD
loc_1_218C:
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    cl, byte ptr [esp + 4]
        mov    byte ptr [eax + 0x35ec], cl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
        cmp    dword ptr [0x35e8], 2         ; -> loc_1_35E8
        je     loc_1_21B3
        cmp    dword ptr [0x35e8], 4         ; -> loc_1_35E8
        jne    loc_1_21BD
loc_1_21B3:
        mov    dword ptr [0x3568], 0x220e    ; -> sub_1_3568
loc_1_21BD:
        ret    
loc_1_21BE:
        cmp    dword ptr [0x35e8], 4         ; -> loc_1_35E8
        jl     loc_1_21CE
        call   sub_1_0643
        jmp    loc_1_220D
loc_1_21CE:
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    cl, byte ptr [esp + 4]
        mov    byte ptr [eax + 0x35ec], cl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
        cmp    cl, 0x7f
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        jbe    loc_1_21F6
        mov    byte ptr [eax + 0x35ec], 0xff ; -> loc_1_35EC
        jmp    loc_1_21FD
loc_1_21F6:
        mov    byte ptr [eax + 0x35ec], 0    ; -> loc_1_35EC
loc_1_21FD:
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
        mov    dword ptr [0x3568], 0x220e    ; -> sub_1_3568
loc_1_220D:
        ret    
loc_1_220E:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 0xf5
        je     loc_1_222F
        cmp    eax, 0xf6
        je     loc_1_2245
        cmp    eax, 0xff
        je     loc_1_2275
        call   sub_1_0643
        jmp    loc_1_228E
loc_1_222F:
        mov    dword ptr [0x3568], 0x217c    ; -> sub_1_3568
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_228E
loc_1_2245:
        mov    dword ptr [0x3568], 0x21be    ; -> sub_1_3568
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_228E
        db  08Bh, 00Dh
        dd  loc_1_35E8               ; fixup
        db  088h, 091h
        dd  loc_1_35EC               ; fixup
        db  0FFh, 005h
        dd  loc_1_35E8               ; fixup
        db  039h, 005h
        dd  loc_1_35E8               ; fixup
        db  07Dh, 0E6h
loc_1_2275:
        call   sub_1_2029
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_228E:
        ret    
loc_1_228F:
        cmp    dword ptr [0x35e8], 4         ; -> loc_1_35E8
        jl     loc_1_229F
        call   sub_1_0643
        jmp    loc_1_22D0
loc_1_229F:
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    cl, byte ptr [esp + 4]
        mov    byte ptr [eax + 0x35ec], cl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
        cmp    dword ptr [0x35e8], 2         ; -> loc_1_35E8
        je     loc_1_22C6
        cmp    dword ptr [0x35e8], 4         ; -> loc_1_35E8
        jne    loc_1_22D0
loc_1_22C6:
        mov    dword ptr [0x3568], 0x2321    ; -> sub_1_3568
loc_1_22D0:
        ret    
loc_1_22D1:
        cmp    dword ptr [0x35e8], 4         ; -> loc_1_35E8
        jl     loc_1_22E1
        call   sub_1_0643
        jmp    loc_1_2320
loc_1_22E1:
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        mov    cl, byte ptr [esp + 4]
        mov    byte ptr [eax + 0x35ec], cl   ; -> loc_1_35EC
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
        cmp    cl, 0x7f
        mov    eax, dword ptr [0x35e8]       ; -> loc_1_35E8
        jbe    loc_1_2309
        mov    byte ptr [eax + 0x35ec], 0xff ; -> loc_1_35EC
        jmp    loc_1_2310
loc_1_2309:
        mov    byte ptr [eax + 0x35ec], 0    ; -> loc_1_35EC
loc_1_2310:
        inc    dword ptr [0x35e8]            ; -> loc_1_35E8
        mov    dword ptr [0x3568], 0x2321    ; -> sub_1_3568
loc_1_2320:
        ret    
loc_1_2321:
        movzx  eax, byte ptr [esp + 4]
        cmp    eax, 0xf5
        je     loc_1_2342
        cmp    eax, 0xf6
        je     loc_1_2358
        cmp    eax, 0xff
        je     loc_1_2388
        call   sub_1_0643
        jmp    loc_1_23A1
loc_1_2342:
        mov    dword ptr [0x3568], 0x228f    ; -> sub_1_3568
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_23A1
loc_1_2358:
        mov    dword ptr [0x3568], 0x22d1    ; -> sub_1_3568
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_23A1
        db  08Bh, 00Dh
        dd  loc_1_35E8               ; fixup
        db  088h, 091h
        dd  loc_1_35EC               ; fixup
        db  0FFh, 005h
        dd  loc_1_35E8               ; fixup
        db  039h, 005h
        dd  loc_1_35E8               ; fixup
        db  07Dh, 0E6h
loc_1_2388:
        call   sub_1_208E
        mov    dword ptr [0x35e8], 0         ; -> loc_1_35E8
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_23A1:
        ret    
sub_1_23A2:
        mov    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        cmp    dword ptr [0x3638], 0         ; -> loc_1_3638
        je     loc_1_23D2
        cmp    dword ptr [0x3648], 0         ; -> loc_1_3648
        jne    loc_1_23D2
        mov    dword ptr [0x34c0], 2         ; -> obj1:0x34C0
        mov    dword ptr [0x3648], 1         ; -> loc_1_3648
loc_1_23D2:
        cmp    dword ptr [0x363c], 0         ; -> loc_1_363C
        je     loc_1_23F5
        cmp    dword ptr [0x364c], 0         ; -> loc_1_364C
        jne    loc_1_23F5
        add    dword ptr [0x34c0], 8         ; -> obj1:0x34C0
        mov    dword ptr [0x364c], 1         ; -> loc_1_364C
loc_1_23F5:
        cmp    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        je     loc_1_240C
        push   dword ptr [0x34c4]            ; -> obj1:0x34C4
        call   sub_1_1F60
        add    esp, 4
loc_1_240C:
        ret    
sub_1_240D:
        mov    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        cmp    dword ptr [0x3638], 0         ; -> loc_1_3638
        je     loc_1_243D
        cmp    dword ptr [0x3648], 0         ; -> loc_1_3648
        je     loc_1_243D
        mov    dword ptr [0x34c0], 4         ; -> obj1:0x34C0
        mov    dword ptr [0x3648], 0         ; -> loc_1_3648
loc_1_243D:
        cmp    dword ptr [0x363c], 0         ; -> loc_1_363C
        je     loc_1_2460
        cmp    dword ptr [0x364c], 0         ; -> loc_1_364C
        je     loc_1_2460
        add    dword ptr [0x34c0], 0x10      ; -> obj1:0x34C0
        mov    dword ptr [0x364c], 0         ; -> loc_1_364C
loc_1_2460:
        cmp    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        je     loc_1_2477
        push   dword ptr [0x34c4]            ; -> obj1:0x34C4
        call   sub_1_1F60
        add    esp, 4
loc_1_2477:
        ret    
loc_1_2478:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 1
        je     loc_1_24C6
        cmp    eax, 2
        je     loc_1_24DF
        cmp    eax, 0xfc
        je     loc_1_24C6
        cmp    eax, 0xfd
        je     loc_1_24F5
        cmp    eax, 0xff
        je     loc_1_2506
        cmp    cl, 0xec
        jb     loc_1_24AD
        call   sub_1_0643
        jmp    loc_1_256C
loc_1_24AD:
        mov    dword ptr [0x34c8], 1         ; -> obj1:0x34C8
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_256C
loc_1_24C6:
        mov    dword ptr [0x3638], 1         ; -> loc_1_3638
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_256C
loc_1_24DF:
        mov    dword ptr [0x363c], 1         ; -> loc_1_363C
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_256C
loc_1_24F5:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_256C
loc_1_2506:
        cmp    dword ptr [0x3638], 0         ; -> loc_1_3638
        jne    loc_1_253F
        cmp    dword ptr [0x363c], 0         ; -> loc_1_363C
        jne    loc_1_253F
        cmp    dword ptr [0x34c8], 0         ; -> obj1:0x34C8
        jne    loc_1_253F
        mov    dword ptr [0x34c8], 1         ; -> obj1:0x34C8
        mov    dword ptr [0x363c], 1         ; -> loc_1_363C
        mov    dword ptr [0x3638], 1         ; -> loc_1_3638
loc_1_253F:
        call   sub_1_240D
        mov    dword ptr [0x34c8], 0         ; -> obj1:0x34C8
        mov    dword ptr [0x363c], 0         ; -> loc_1_363C
        mov    dword ptr [0x3638], 0         ; -> loc_1_3638
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_256C:
        ret    
loc_1_256D:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 1
        je     loc_1_25BB
        cmp    eax, 2
        je     loc_1_25D1
        cmp    eax, 0xfc
        je     loc_1_25BB
        cmp    eax, 0xfd
        je     loc_1_25E7
        cmp    eax, 0xff
        je     loc_1_25F8
        cmp    cl, 0xec
        jb     loc_1_25A2
        call   sub_1_0643
        jmp    loc_1_264A
loc_1_25A2:
        mov    dword ptr [0x34c8], 1         ; -> obj1:0x34C8
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_264A
loc_1_25BB:
        mov    dword ptr [0x3638], 1         ; -> loc_1_3638
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_264A
loc_1_25D1:
        mov    dword ptr [0x363c], 1         ; -> loc_1_363C
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_264A
loc_1_25E7:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_264A
loc_1_25F8:
        cmp    dword ptr [0x3638], 0         ; -> loc_1_3638
        jne    loc_1_261D
        cmp    dword ptr [0x363c], 0         ; -> loc_1_363C
        jne    loc_1_261D
        cmp    dword ptr [0x34c8], 0         ; -> obj1:0x34C8
        jne    loc_1_261D
        mov    dword ptr [0x3638], 1         ; -> loc_1_3638
loc_1_261D:
        call   sub_1_23A2
        mov    dword ptr [0x34c8], 0         ; -> obj1:0x34C8
        mov    dword ptr [0x363c], 0         ; -> loc_1_363C
        mov    dword ptr [0x3638], 0         ; -> loc_1_3638
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_264A:
        ret    
loc_1_264B:
        mov    al, byte ptr [esp + 4]
        cmp    al, 0xff
        jne    loc_1_271B
        mov    eax, dword ptr [0x3638]       ; -> loc_1_3638
        or     eax, eax
        mov    edx, dword ptr [0x363c]       ; -> loc_1_363C
        jne    loc_1_2678
        or     edx, edx
        jne    loc_1_2678
        cmp    dword ptr [0x34c8], 0         ; -> obj1:0x34C8
        jne    loc_1_2678
        mov    eax, 1
loc_1_2678:
        mov    dword ptr [0x3638], eax       ; -> loc_1_3638
        mov    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        or     eax, eax
        je     loc_1_26A8
        cmp    dword ptr [0x3648], 0         ; -> loc_1_3648
        je     loc_1_26A8
        mov    dword ptr [0x34c0], 4         ; -> obj1:0x34C0
        mov    dword ptr [0x3648], 0         ; -> loc_1_3648
loc_1_26A8:
        or     edx, edx
        je     loc_1_26C6
        cmp    dword ptr [0x364c], 0         ; -> loc_1_364C
        je     loc_1_26C6
        add    dword ptr [0x34c0], 0x10      ; -> obj1:0x34C0
        mov    dword ptr [0x364c], 0         ; -> loc_1_364C
loc_1_26C6:
        cmp    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        jne    loc_1_26DD
        push   dword ptr [0x34c4]            ; -> obj1:0x34C4
        call   sub_1_1F60
        add    esp, 4
loc_1_26DD:
        call   sub_1_23A2
        call   sub_1_240D
        call   sub_1_23A2
        call   sub_1_240D
        mov    dword ptr [0x34c8], 0         ; -> obj1:0x34C8
        mov    dword ptr [0x363c], 0         ; -> loc_1_363C
        mov    dword ptr [0x3638], 0         ; -> loc_1_3638
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
        jmp    loc_1_2724
loc_1_271B:
        push   eax
        call   sub_1_2725
        add    esp, 4
loc_1_2724:
        ret    
sub_1_2725:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 1
        je     loc_1_2773
        cmp    eax, 2
        je     loc_1_278C
        cmp    eax, 0xfc
        je     loc_1_2773
        cmp    eax, 0xfd
        je     loc_1_27A5
        cmp    eax, 0xff
        je     loc_1_27B9
        cmp    cl, 0xec
        jb     loc_1_275A
        call   sub_1_0643
        jmp    loc_1_2871
loc_1_275A:
        mov    dword ptr [0x34c8], 1         ; -> obj1:0x34C8
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_2871
loc_1_2773:
        mov    dword ptr [0x3638], 1         ; -> loc_1_3638
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_2871
loc_1_278C:
        mov    dword ptr [0x363c], 1         ; -> loc_1_363C
        mov    dword ptr [0x3578], 0         ; -> loc_1_3578
        jmp    loc_1_2871
loc_1_27A5:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        jmp    loc_1_2871
loc_1_27B9:
        mov    eax, dword ptr [0x3638]       ; -> loc_1_3638
        or     eax, eax
        mov    edx, dword ptr [0x363c]       ; -> loc_1_363C
        jne    loc_1_27DA
        or     edx, edx
        jne    loc_1_27DA
        cmp    dword ptr [0x34c8], 0         ; -> obj1:0x34C8
        jne    loc_1_27DA
        mov    eax, 1
loc_1_27DA:
        mov    dword ptr [0x3638], eax       ; -> loc_1_3638
        mov    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        or     eax, eax
        je     loc_1_280A
        cmp    dword ptr [0x3648], 0         ; -> loc_1_3648
        je     loc_1_280A
        mov    dword ptr [0x34c0], 4         ; -> obj1:0x34C0
        mov    dword ptr [0x3648], 0         ; -> loc_1_3648
loc_1_280A:
        or     edx, edx
        je     loc_1_2828
        cmp    dword ptr [0x364c], 0         ; -> loc_1_364C
        je     loc_1_2828
        add    dword ptr [0x34c0], 0x10      ; -> obj1:0x34C0
        mov    dword ptr [0x364c], 0         ; -> loc_1_364C
loc_1_2828:
        cmp    dword ptr [0x34c0], 0         ; -> obj1:0x34C0
        jne    loc_1_283F
        push   dword ptr [0x34c4]            ; -> obj1:0x34C4
        call   sub_1_1F60
        add    esp, 4
loc_1_283F:
        call   sub_1_23A2
        call   sub_1_240D
        mov    dword ptr [0x34c8], 0         ; -> obj1:0x34C8
        mov    dword ptr [0x363c], 0         ; -> loc_1_363C
        mov    dword ptr [0x3638], 0         ; -> loc_1_3638
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_2871:
        ret    
loc_1_2872:
        mov    cl, byte ptr [esp + 4]
        movzx  eax, cl
        cmp    eax, 0xfd
        je     loc_1_288E
        cmp    cl, 0xec
        jae    loc_1_2887
        jmp    loc_1_288E
loc_1_2887:
        call   sub_1_0643
        jmp    loc_1_28A7
loc_1_288E:
        call   sub_1_0652
        mov    dword ptr [0x3568], 0x882     ; -> sub_1_3568
        mov    dword ptr [0x3578], 1         ; -> loc_1_3578
loc_1_28A7:
        ret    
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  0, 128                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ENABLE4 '                  ; DDB_Name
        dd  000000000h                  ; DDB_Init_Order
        dd  ENABLE4_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_291C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2920:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2924:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2928:
        add    byte ptr [eax], al
loc_1_292A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_295A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_295E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2966:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_296E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2972:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2984:
        add    byte ptr [eax], al
loc_1_2986:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_299E:
        add    byte ptr [eax], al
        jae    loc_1_29A6
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
loc_1_29A6:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_29B8:
        jo     0x2a2c
        jae    0x2a30
        sub    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_29C8:
        inc    edx
        inc    ebp
        inc    edi
        dec    ecx
        dec    esi
        add    byte ptr [eax], al
        db  000h
loc_1_29D0:
        inc    edx
        dec    esp
        dec    ebx
        push   esp
        push   edx
        dec    esi
        push   ebx
        db  000h
loc_1_29D8:
        inc    ebx
        dec    esp
        inc    ebp
        inc    ecx
        push   edx
        add    byte ptr [eax], al
        db  000h
loc_1_29E0:
        inc    ebp
        dec    esi
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, cl                        ; -> loc_1_29C8
        sub    dword ptr [eax], eax
        add    dl, bh
        shr    byte ptr [ecx], 1
        add    byte ptr [eax], al
        neg    eax                           ; -> loc_1_29D8
        sub    dword ptr [eax], eax
        add    bl, bh
        loopne 0x2a22
        add    byte ptr [eax], al
        stc    
        in     al, 0x29
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A04:
        bound  esp, qword ptr [ecx + 0x75]
        jb     0x2a6b
        je     0x2a71
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A10:
        arpl   word ptr [ecx + ebp*2 + 0x63], bp
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        bound  ebp, qword ptr [ecx + 0x6e]
        db  065h, 000h
loc_1_2A20:
        arpl   word ptr [edi + 0x6d], bp
        insd   dword ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A28:
        bound  ebp, qword ptr fs:[ebx + 0x6c]
        imul   esp, dword ptr [ebx + 0x6b], 0
loc_1_2A34:
        outsb  dx, byte ptr gs:[si]
        db  000h
loc_1_2A38:
        outsd  dx, dword ptr [si]
        je     0x2aab
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A40:
        push   0x646c6f
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x62], ch
        add    byte ptr fs:[edi + ebp*2 + 0x63], ch
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_1_2A54:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jne    loc_1_2A58
loc_1_2A58:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jne    loc_1_2ABD
        outsb  dx, byte ptr [esi]
        arpl   word ptr [eax + 0x6f], bp
        jb     loc_1_2A62
loc_1_2A62:
        add    byte ptr [eax], al
loc_1_2A64:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jne    loc_1_2AD4
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx], bp
loc_1_2A6C:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jne    0x2ae2
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_2A74:
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jne    loc_1_2AEA
        jae    0x2ae0
        je     loc_1_2A7D
loc_1_2A7D:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x6f], ch
        jbe    0x2ae9
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2A88:
        jo     loc_1_2AFC
        jae    loc_1_2B00
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x65], dh
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + ebp], al      ; -> loc_1_2A04
        add    byte ptr [eax], al
        xchg   edi, eax
        adc    byte ptr [edx], ch
        add    byte ptr [eax], al
        or     eax, 0x2a18                   ; -> obj1:0x2A18
        add    esp, dword ptr [eax]          ; -> loc_1_2A20
        sub    al, byte ptr [eax]
        add    byte ptr [esi + 0x2a28], dl   ; -> loc_1_2A28
        push   cs
        xor    al, 0x2a
        add    byte ptr [eax], al
        mov    al, byte ptr [0x2a38]         ; -> loc_1_2A38
        adc    byte ptr [eax + 0x2a], al     ; -> loc_1_2A40
loc_1_2ABD:
        add    byte ptr [eax], al
        add    al, 0x48                      ; -> obj1:0x2A48
        sub    al, byte ptr [eax]
        add    byte ptr [edx], cl
        dec    esp
        sub    al, byte ptr [eax]
        add    byte ptr [0x2a54], al         ; -> loc_1_2A54
        adc    al, 0x58                      ; -> loc_1_2A58
        sub    al, byte ptr [eax]
        add    byte ptr [edx], dl
loc_1_2AD4:
        sub    al, byte ptr fs:[eax]
        add    byte ptr [ebx], cl
        insb   byte ptr es:[edi], dx
        sub    al, byte ptr [eax]
        add    byte ptr [esp + esi*2], cl    ; -> loc_1_2A74
        sub    al, byte ptr [eax]
        add    byte ptr [ecx], dl
        sub    byte ptr [edx], 0
        add    byte ptr [edi], cl
        mov    byte ptr [edx], ch
loc_1_2AEA:
        add    byte ptr [eax], al
        add    dl, byte ptr [eax + 0x600002a] ; -> obj1:0x2A90
        xchg   esp, eax
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_2AF8:
        jae    loc_1_2B5F
loc_1_2AFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B00:
        imul   ebp, dword ptr [esi + 0x64], 0x646f6d00
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x65], dh
        jb     loc_1_2B10
loc_1_2B10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B14:
        clc    
        sub    al, byte ptr [eax]
        add    byte ptr [ecx], cl
        add    byte ptr [ebx], ch
        add    byte ptr [eax], al
        push   es
        add    al, 0x2b
        add    byte ptr [eax], al
        or     byte ptr [ebx + ebp], cl      ; -> obj1:0x2B0C
        add    byte ptr [eax], al
        pop    es
        adc    byte ptr [ebx], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B30:
        imul   esp, dword ptr [edx + 0x6d], 0x7461
        db  000h
loc_1_2B38:
        imul   esp, dword ptr [edx + 0x6d], 0x63686e65
        add    byte ptr [ecx + 0x62], ch
        insd   dword ptr es:[edi], dx
        jo     loc_1_2BA8
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dh            ; -> loc_1_2B30
        sub    eax, dword ptr [eax]
        add    byte ptr [edx], al
        cmp    byte ptr [ebx], ch
        add    byte ptr [eax], al
        add    dword ptr [eax + 0x2b], eax   ; -> obj1:0x2B40
        add    byte ptr [eax], al
        add    ecx, dword ptr [eax + 0x2b]   ; -> obj1:0x2B48
        add    byte ptr [eax], al
loc_1_2B5F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x2b], ah     ; -> obj1:0x2B60
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B70:
        insb   byte ptr es:[edi], dx
        sub    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x2b], bh     ; -> obj1:0x2B78
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B88:
        test   byte ptr [ebx], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B90:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2B94:
        nop    
        sub    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + ebp], bl      ; -> obj1:0x2B9C
        add    byte ptr [eax], al
loc_1_2BA8:
        xor    dword ptr [edx], esi
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BB0:
        xor    dh, byte ptr [eax + esi]
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_2BB8:
        xor    esi, dword ptr [eax]
        xor    byte ptr [eax], al
loc_1_2BBC:
        xor    al, 0x38
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BC4:
        xor    byte ptr ss:[eax], dh
        add    byte ptr [ecx], bh
        xor    byte ptr ss:[eax], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BD4:
        test   al, 0x2b
        add    byte ptr [eax], al
        add    esi, dword ptr [eax + 0x400002b] ; -> loc_1_2BB0
        mov    eax, 0x100002b
        mov    esp, 0x500002b
        les    ebp, ptr [ebx]
        add    byte ptr [eax], al
        add    cl, al                        ; -> obj1:0x2BC8
        sub    eax, dword ptr [eax]
        add    byte ptr [esi], al
        shr    byte ptr [ebx], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2BF8:
        insb   byte ptr es:[edi], dx
        je     loc_1_2BFD
loc_1_2BFD:
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x69], dh
        push   0x74
loc_1_2C08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2C0C:
        clc    
        sub    eax, dword ptr [eax]
        add    byte ptr [ecx], al
        add    byte ptr [eax + eax], ch
        add    byte ptr [edx], al
        or     byte ptr [eax + eax], ch
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], ch
        add    byte ptr [eax], al
        pop    eax
        add    al, byte ptr [eax]
        add    byte ptr [eax + 0x60000004], dh
        or     dword ptr [eax], eax
        add    al, al
        adc    al, byte ptr [eax]
        add    byte ptr [eax + 0x25], al
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
loc_1_2C39:
        add    edi, dword ptr [edx]
        pop    ds
        cmp    dh, byte ptr [edx]
        cmp    dh, byte ptr [eax]
        cmp    ah, byte ptr [ecx]
        cmp    dl, byte ptr [ebx]
        cmp    ah, byte ptr [edx]
        cmp    ah, byte ptr [ebx]
        cmp    ah, byte ptr [edx + edi]
        sbb    byte ptr [edx], bh
        and    eax, 0x273a263a
        cmp    dh, byte ptr [edx + edi]
        xor    edi, dword ptr [edx]
        sbb    dword ptr [edx], edi
        sbb    bh, byte ptr [edx]
        adc    dword ptr [edx], edi
        adc    al, 0x3a
        and    byte ptr [edx], bh
        adc    eax, 0x313a173a
        cmp    dl, byte ptr [edx]
        cmp    ch, byte ptr [edi]
        cmp    dl, byte ptr [esi]
        cmp    ch, byte ptr [esi]
        cmp    bl, byte ptr [ebx]
        cmp    bl, byte ptr [0x73a1c3a]
        cmp    cl, byte ptr [edi + 0x2c022c00]
        sub    dword ptr [esp + eax], ebp
        sub    al, 5
        sub    al, 6
        sub    al, 8
        sub    dword ptr [eax], eax
        sub    al, 0xa
        sub    al, 0xb
        sub    al, 9
        sub    al, 0xd
        xor    eax, 0x36000c00
        add    byte ptr [edi], dh
        add    byte ptr [ebx], cl
        add    byte ptr [edx], al
        add    byte ptr [ebx], al
        add    byte ptr [eax + eax], al
        add    eax, 0x7000600
        add    byte ptr [eax], cl
        add    byte ptr [ecx], cl
        add    byte ptr [edx], cl
        add    byte ptr [eax + ebp], ch
        sub    byte ptr [eax], al
        sub    al, 0x35
        or     eax, 0x2c362c00
        aaa    
        sub    al, 3
        sub    al, 0x1f
        sub    al, 0x32
        sub    al, 0x30
        sub    al, 0x21
        sub    al, 0x13
        sub    al, 0x22
        sub    al, 0x23
        sub    al, 0x24
        sub    al, 0x18
        sub    al, 0x25
        sub    al, 0x26
        sub    al, 0x27
        sub    al, 0x34
        sub    al, 0x33
        sub    al, 0x19
        sub    al, 0x1a
        sub    al, 0x11
        sub    al, 0x14
        sub    al, 0x20
        sub    al, 0x15
        sub    al, 0x17
        sub    al, 0x31
        sub    al, 0x12
        sub    al, 0x2f
        sub    al, 0x16
        sub    al, 0x2e
        sbb    eax, dword ptr [eax]
        sbb    eax, 0x2c001c00
        pop    es
        sub    al, 0xc
        add    dword ptr [eax], eax
        pop    ds
        add    byte ptr [edx], dh
        add    byte ptr [eax], dh
        add    byte ptr [ecx], ah
        add    byte ptr [ebx], dl
        add    byte ptr [edx], ah
        add    byte ptr [ebx], ah
        add    byte ptr [eax + eax], ah
        sbb    byte ptr [eax], al
        and    eax, 0x27002600
        add    byte ptr [eax + eax], dh
        xor    eax, dword ptr [eax]
        sbb    dword ptr [eax], eax
        sbb    al, byte ptr [eax]
        adc    dword ptr [eax], eax
        adc    al, 0
        and    byte ptr [eax], al
        adc    eax, 0x31001700
        add    byte ptr [edx], dl
        add    byte ptr [edi], ch
        add    byte ptr [esi], dl
        add    byte ptr [esi], ch
        add    byte ptr [ebx + ebx], ch
        sub    al, 0x1d
        sub    al, 0x1c
        sub    al, 1
        db  03Ah, 068h
loc_1_2D38:
        popal  
        insb   byte ptr es:[edi], dx
        je     loc_1_2D3C
loc_1_2D3C:
        bound  esp, qword ptr [ecx + 0x63]
        imul   esi, dword ptr [ebx + 0x70], 0x61
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
loc_1_2D48:
        bound  ebp, qword ptr [ebx + 0x73]
        jo     loc_1_2D4D
loc_1_2D4D:
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x72], ah
        popal  
        imul   eax, dword ptr [eax], 0
        add    byte ptr [ebx + 0x61], ah
        jo     loc_1_2DCF
        insb   byte ptr es:[edi], dx
        imul   eax, dword ptr [eax], 0
loc_1_2D60:
        arpl   word ptr [ecx + 0x70], sp
        jae    0x2dd1
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        popal  
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     0x2deb
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        db  000h
loc_1_2D7C:
        arpl   word ptr [edx + esi*2 + 0x6c], si
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2D84:
        insb   byte ptr es:[edi], dx
        add    byte ptr [ebp + 0x6c], ah
        je     0x2df3
        add    byte ptr [eax], al
loc_1_2D90:
        imul   esi, dword ptr fs:[esi + 0x69], 0x6564
loc_1_2D98:
        outsd  dx, dword ptr fs:[esi]
        ja     0x2e0a
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2DA0:
        outsb  dx, byte ptr gs:[esi]
        add    byte ptr fs:[ebp + 0x6e], ah
        je     0x2e0d
        jb     loc_1_2DAA
loc_1_2DAA:
        add    byte ptr [eax], al
loc_1_2DAC:
        jae    0x2e12
        add    byte ptr [ebp + 0x73], ah
        arpl   word ptr [ecx + 0x70], sp
        add    byte ptr gs:[eax], al
loc_1_2DB8:
        xor    word ptr [eax], ax
        db  000h
loc_1_2DBC:
        xor    word ptr [eax], si
        add    byte ptr [esi + 0x31], ah
        xor    dword ptr [eax], eax
loc_1_2DC4:
        xor    word ptr [edx], si
        add    byte ptr [esi + 0x32], ah
        add    byte ptr [eax], al
loc_1_2DCC:
        xor    ax, word ptr [eax]
loc_1_2DCF:
        add    byte ptr [esi + 0x34], ah
        add    byte ptr [eax], al
loc_1_2DD4:
        xor    ax, 0
loc_1_2DD8:
        add    byte ptr ss:[eax], al
loc_1_2DDC:
        aaa    
        add    byte ptr [eax], al
loc_1_2DE0:
        cmp    byte ptr [eax], al
        add    byte ptr [esi + 0x39], ah
        add    byte ptr [eax], al
loc_1_2DE8:
        push   0x656d6f
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], ch
        jae    loc_1_2DF4
loc_1_2DF4:
        imul   ebp, dword ptr [esi + 0x73], 0x747265
        db  000h
loc_1_2DFC:
        imul   esi, dword ptr [eax + 0x2a], 0
loc_1_2E00:
        imul   esi, dword ptr [eax + 0x2b], 0
loc_1_2E04:
        imul   esi, dword ptr [eax + 0x2d], 0
loc_1_2E08:
        imul   esi, dword ptr [eax + 0x2f], 0
loc_1_2E0C:
        imul   esi, dword ptr [eax + 0x30], 0
loc_1_2E10:
        imul   esi, dword ptr [eax + 0x31], 0
loc_1_2E14:
        imul   esi, dword ptr [eax + 0x32], 0
loc_1_2E18:
        imul   esi, dword ptr [eax + 0x33], 0
loc_1_2E1C:
        imul   esi, dword ptr [eax + 0x34], 0
loc_1_2E20:
        imul   esi, dword ptr [eax + 0x35], 0
loc_1_2E24:
        imul   esi, dword ptr [eax + 0x36], 0
loc_1_2E28:
        imul   esi, dword ptr [eax + 0x37], 0
loc_1_2E2C:
        imul   esi, dword ptr [eax + 0x38], 0
loc_1_2E30:
        imul   esi, dword ptr [eax + 0x39], 0
loc_1_2E34:
        imul   esi, dword ptr [eax + 0x64], 0x65
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        db  000h
loc_1_2E3C:
        imul   esi, dword ptr [eax + 0x64], 0x65
        insb   byte ptr es:[edi], dx
        je     0x2ea9
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E48:
        imul   esi, dword ptr [eax + 0x64], 0x69
        jbe    0x2eb7
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_2E54:
        imul   esi, dword ptr [eax + 0x64], 0x6e
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E5C:
        imul   esi, dword ptr [eax + 0x64], 0x6f
        ja     loc_1_2ED0
        add    byte ptr [eax], al
loc_1_2E64:
        imul   esi, dword ptr [eax + 0x64], 0x70
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E6C:
        imul   esi, dword ptr [eax + 0x65], 0x6e
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 0x70], ch
        outsb  dx, byte ptr gs:[esi]
        je     0x2edf
        jb     loc_1_2E7C
loc_1_2E7C:
        imul   esi, dword ptr [eax + 0x68], 0x6f
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
loc_1_2E84:
        imul   esi, dword ptr [eax + 0x69], 0x6e
        jae    loc_1_2E8A
loc_1_2E8A:
        add    byte ptr [eax], al
loc_1_2E8C:
        imul   esi, dword ptr [eax + 0x69], 0x6e
        jae    loc_1_2EF7
        jb     loc_1_2F08
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2E98:
        imul   esi, dword ptr [eax + 0x6c], 0x65
        je     loc_1_2E9F
loc_1_2E9F:
        add    byte ptr [ebx + 0x70], ch
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr [esp + ebp*2], 0x6d706b00
        imul   ebp, dword ptr [esi + 0x75], 0x706b0073
        jo     0x2f15
        outsd  dx, dword ptr fs:[si]
        ja     0x2f28
        add    byte ptr [eax], al
loc_1_2EBC:
        imul   esi, dword ptr [eax + 0x70], 0x61
        jne    loc_1_2F34
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2EC8:
        imul   esi, dword ptr [eax + 0x70], 0x67
        outsb  dx, byte ptr fs:[esi]
        add    byte ptr [eax], al
loc_1_2ED0:
        imul   esi, dword ptr [eax + 0x70], 0x67
        jne    0x2f46
        add    byte ptr [eax], al
loc_1_2ED8:
        imul   esi, dword ptr [eax + 0x70], 0x6c
        jne    loc_1_2F51
        add    byte ptr [eax], al
loc_1_2EE0:
        imul   esi, dword ptr [eax + 0x72], 0x69
        db  067h, 068h, 074h, 000h
sub_1_2EE8:
        imul   esi, dword ptr [eax + 0x73], 0x6c
        popal  
        jae    0x2f57
        add    byte ptr [ebx + 0x70], ch
        jae    0x2f68
        popal  
        jb     loc_1_2EF7
loc_1_2EF7:
        add    byte ptr [ebx + 0x70], ch
        je     loc_1_2F65
        insd   dword ptr es:[edi], dx
        jae    loc_1_2F00
loc_1_2F00:
        imul   esi, dword ptr [eax + 0x75], 0x70
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F08:
        insb   byte ptr es:[edi], dx
        popal  
        insb   byte ptr es:[edi], dx
        je     loc_1_2F0D
loc_1_2F0D:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], ch
        outsb  dx, byte ptr [esi]
        je     loc_1_2F88
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F1C:
        insb   byte ptr es:[edi], dx
        arpl   word ptr [edx + esi*2 + 0x6c], si
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x66], ch
        je     loc_1_2F29
loc_1_2F29:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x66], ch
        je     loc_1_2F92
        insb   byte ptr es:[edi], dx
        je     loc_1_2F34
loc_1_2F34:
        insb   byte ptr es:[edi], dx
        je     loc_1_2F9C
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x2faf
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        add    byte ptr [ebp + 0x66], ch
        je     loc_1_2FA8
        je     0x2fb9
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F4C:
        insb   byte ptr es:[edi], dx
        je     loc_1_2FC4
loc_1_2F51:
        push   0x746669
        add    byte ptr [eax], al
loc_1_2F58:
        insb   byte ptr es:[edi], dx
        jae    0x2fc3
        imul   esp, dword ptr [esi + 0x74], 0x756d0000
        insb   byte ptr es:[edi], dx
        je     loc_1_2FCE
loc_1_2F65:
        jo     loc_1_2FD3
        jns    loc_1_2F69
loc_1_2F69:
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x75], ch
        insd   dword ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        imul   eax, dword ptr [eax], 0
        add    byte ptr [esi + 0x75], ch
        insd   dword ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx], bp
loc_1_2F7C:
        jo     0x2fdf
        outsd  dx, dword ptr fs:[si]
        ja     loc_1_2FF2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2F88:
        jo     0x2feb
        jne    loc_1_2FFE
        add    byte ptr [eax], al
loc_1_2F90:
        jo     0x2ff3
loc_1_2F92:
        jne    0x3007
        add    byte ptr gs:[eax], al
        db  000h
loc_1_2F98:
        jo     0x2fff
        jb     0x3005
loc_1_2F9C:
        outsd  dx, dword ptr [esi]
        add    byte ptr fs:[eax], al
loc_1_2FA0:
        jo     0x3009
        outsb  dx, byte ptr fs:[esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2FA8:
        jo     0x3011
        jne    0x301c
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2FB0:
        jo     0x3024
        db  069h, 06Eh, 074h, 000h, 000h, 000h
loc_1_2FB8:
        jo     loc_1_302C
        imul   ebp, dword ptr [esi + 0x74], 0x65726373
        outsb  dx, byte ptr gs:[esi]
        db  000h
loc_1_2FC4:
        jo     0x3038
        je     0x303b
        arpl   word ptr [eax], ax
        add    byte ptr [eax], al
loc_1_2FCC:
        jb     loc_1_302F
loc_1_2FCE:
        insb   byte ptr es:[edi], dx
        je     loc_1_2FD1
loc_1_2FD1:
        add    byte ptr [eax], al
loc_1_2FD3:
        add    byte ptr [edx + 0x63], dh
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x304c
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2FE0:
        jb     loc_1_3045
        je     0x3056
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x65], dh
        je     loc_1_2FEC
loc_1_2FEC:
        jb     0x3053
        je     0x3065
        jb     loc_1_3060
loc_1_2FF2:
        add    byte ptr [eax], al
loc_1_2FF4:
        jb     0x305f
        push   0x74
loc_1_2FFC:
        jb     loc_1_3067
loc_1_2FFE:
        push   0x746c6174
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3008:
        jb     0x3073
        push   0x6e6f6374
        je     0x3084
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3018:
        jb     loc_1_3083
        push   0x72746374
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x69], dh
        push   0x69687374
loc_1_302C:
        je     loc_1_302F
loc_1_302F:
        add    byte ptr [edx + 0x73], dh
        push   0x746669
        add    byte ptr [ebx + 0x63], dh
        jb     loc_1_30AB
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
loc_1_3040:
        jae    0x30a5
        jb     0x30b3
        insb   byte ptr es:[edi], dx
loc_1_3045:
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx], bp
        add    byte ptr [ebx + 0x68], dh
        imul   esp, dword ptr [esi + 0x74], 0x73000000
        jo     0x30b8
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
loc_1_305C:
        jae    0x30d7
        jae    0x30d2
loc_1_3060:
        jno    loc_1_3063
loc_1_3063:
        add    byte ptr [ecx + 0x62], dh
loc_1_3067:
        add    byte ptr [ecx + ebp*2 + 0x6c], dh
        add    byte ptr gs:[eax], al
        add    byte ptr [ebp + 0x70], dh
        add    byte ptr [eax], al
loc_1_3074:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3078:
        cmp    byte ptr [0x3c3c0000], ch     ; -> loc_1_2D3C
        sub    eax, 0x480f0000               ; -> loc_1_2D48
loc_1_3083:
        sub    eax, 0x500f0000               ; -> obj1:0x2D50
        sub    eax, 0x587e0000               ; -> obj1:0x2D58
        sub    eax, 0x601e0000               ; -> loc_1_2D60
        sub    eax, 0x6c1e0000               ; -> obj1:0x2D6C
        sub    eax, 0x74350000               ; -> obj1:0x2D74
        sub    eax, 0x7c3a0000               ; -> loc_1_2D7C
        sub    eax, 0x843a0000               ; -> loc_1_2D84
        sub    eax, 0x88680000               ; -> obj1:0x2D88
loc_1_30AB:
        sub    eax, 0x904c0000               ; -> loc_1_2D90
        sub    eax, 0x985f0000               ; -> loc_1_2D98
        sub    eax, 0xa0540000               ; -> loc_1_2DA0
        sub    eax, 0xa4510000               ; -> obj1:0x2DA4
        sub    eax, 0xac2b0000               ; -> loc_1_2DAC
        sub    eax, 0xb06e0000               ; -> obj1:0x2DB0
        sub    eax, 0xb86e0000               ; -> loc_1_2DB8
        sub    eax, 0xbc700000               ; -> loc_1_2DBC
        sub    eax, 0xc0790000               ; -> obj1:0x2DC0
        sub    eax, 0xc47a0000               ; -> loc_1_2DC4
        sub    eax, 0xc87b0000               ; -> obj1:0x2DC8
        sub    eax, 0xcc710000               ; -> loc_1_2DCC
        sub    eax, 0xd0720000               ; -> obj1:0x2DD0
        sub    eax, 0xd4730000               ; -> loc_1_2DD4
        sub    eax, 0xd8740000               ; -> loc_1_2DD8
        sub    eax, 0xdc750000               ; -> loc_1_2DDC
        sub    eax, 0xe0760000               ; -> loc_1_2DE0
        sub    eax, 0xe4770000               ; -> obj1:0x2DE4
        sub    eax, 0xe8780000               ; -> loc_1_2DE8
        sub    eax, 0xf0500000               ; -> obj1:0x2DF0
        sub    eax, 0xf4630000               ; -> loc_1_2DF4
        sub    eax, 0xfc4b0000               ; -> loc_1_2DFC
        sub    eax, 0x640000                 ; -> loc_1_2E00
        add    byte ptr cs:[eax], al
        push   4                             ; -> loc_1_2E04
        add    byte ptr cs:[eax], al
        imul   ecx, dword ptr [eax], 0x5f00002e ; -> loc_1_2E08
        or     al, 0x2e
        add    byte ptr [eax], al
        arpl   word ptr [eax], dx            ; -> loc_1_2E10
        add    byte ptr cs:[eax], al
        pop    ebp
        adc    al, 0x2e
        add    byte ptr [eax], al
        bound  ebx, qword ptr [eax]          ; -> loc_1_2E18
        add    byte ptr cs:[eax], al
        sbb    al, 0x2e                      ; -> loc_1_2E1C
        add    byte ptr [eax], al
        pop    esp
        and    byte ptr [esi], ch
        add    byte ptr [eax], al
        popal  
        and    al, 0x2e
        add    byte ptr [eax], al
        sub    byte ptr [esi], ch            ; -> loc_1_2E28
        add    byte ptr [eax], al
        pop    ebx
        sub    al, 0x2e
        add    byte ptr [eax], al
        pushal 
        xor    byte ptr [esi], ch
        add    byte ptr [eax], al
        xor    al, 0x2e                      ; -> loc_1_2E34
        add    byte ptr [eax], al
        push   0x2e3c                        ; -> loc_1_2E3C
        push   0x2e48                        ; -> loc_1_2E48
        pop    edi
        push   esp
        add    byte ptr cs:[eax], al
        bound  ebx, qword ptr [esi + ebp]    ; -> loc_1_2E5C
        add    byte ptr [edx + 0x64], ah     ; -> loc_1_2E64
        add    byte ptr cs:[eax], al
        push   0x2e6c                        ; -> loc_1_2E6C
        pop    ebp
        je     0x31b1
        add    byte ptr [eax], al
        insb   byte ptr es:[edi], dx
        jl     0x31b6
        add    byte ptr [eax], al
        pop    ebx
        test   byte ptr [esi], ch
        add    byte ptr [eax], al
        arpl   word ptr [esi + ebp - 0x679d0000], cx ; -> loc_1_2E8C
        add    byte ptr cs:[eax], al
        pop    esp
        mov    al, byte ptr [0x6100002e]
        test   al, 0x2e
        add    byte ptr [eax], al
        imul   esi, dword ptr [eax + 0x6700002e], 0x2ebc ; -> obj1:0x2EB0
        enter  0x2e, 0                       ; -> loc_1_2EC8
        shr    byte ptr [0], 1               ; -> loc_1_2ED0
        fsubr  dword ptr gs:[esi]            ; -> loc_1_2ED8
loc_1_31BA:
        add    byte ptr [eax], al
        push   -0x20                         ; -> loc_1_2EE0
        add    byte ptr cs:[eax], al
        call   sub_1_2EE8
        add    byte ptr [edi - 0x10], bl     ; -> obj1:0x2EF0
        add    byte ptr cs:[eax], al
        clc                                  ; -> obj1:0x2EF8
        add    byte ptr cs:[eax], al
        add    byte ptr fs:[edi], ch         ; -> loc_1_2F00
        add    byte ptr [eax], al
        pushal 
        or     byte ptr [edi], ch
        add    byte ptr [eax], al
        cmp    al, 0x10                      ; -> obj1:0x2F10
        das    
        add    byte ptr [eax], al
        cmp    bl, byte ptr [edi + ebp]      ; -> loc_1_2F1C
        add    byte ptr [eax], al
        cmp    ah, byte ptr [edi + ebp]      ; -> obj1:0x2F24
        add    byte ptr [eax], al
        dec    edi
        sub    al, 0x2f
        add    byte ptr [eax], al
loc_1_31EE:
        cmp    al, 0x34                      ; -> loc_1_2F34
        das    
        add    byte ptr [eax], al
loc_1_31F3:
        cmp    al, byte ptr [eax + 0x2f]     ; -> obj1:0x2F40
        add    byte ptr [eax], al
        cmp    cl, byte ptr [edi + ebp]      ; -> loc_1_2F4C
        add    byte ptr [eax + ebx*2], ch    ; -> loc_1_2F58
loc_1_31FF:
        das    
        add    byte ptr [eax], al
        sub    al, 0x60                      ; -> obj1:0x2F60
        das    
        add    byte ptr [eax], al
        insb   byte ptr es:[edi], dx         ; -> obj1:0x2F6C
        das    
        add    byte ptr [eax], al
loc_1_320C:
        pop    edx
        je     loc_1_323E
        add    byte ptr [eax], al
        pop    edx
        jl     loc_1_3243
        add    byte ptr [eax], al
        push   esi
        mov    byte ptr [edi], ch
        add    byte ptr [eax], al
        push   ebp
        nop    
        das    
        add    byte ptr [eax], al
        jle    loc_1_2F98
        das    
        add    byte ptr [eax], al
        mov    al, byte ptr ss:[0x5600002f]  ; -> loc_1_2FA0
        test   al, 0x2f
        add    byte ptr [eax], al
        push   ebp
        mov    al, 0x2f
        add    byte ptr [eax], al
        jl     loc_1_2FB8
        das    
        add    byte ptr [eax], al
        jl     loc_1_2FC4
        das    
        add    byte ptr [eax], al
loc_1_323E:
        jl     loc_1_2FCC
        das    
        add    byte ptr [eax], al
loc_1_3243:
        aam    0x2f                          ; -> obj1:0x2FD4
        add    byte ptr [eax], al
        inc    eax
        loopne loc_1_327A
        add    byte ptr [eax], al
        inc    eax
        call   0x2b003282
        in     al, dx
        das    
        add    byte ptr [eax], al
        sub    esi, esp                      ; -> loc_1_2FF4
        das    
        add    byte ptr [eax], al
        pop    ecx
        cld    
        das    
        add    byte ptr [eax], al
        or     byte ptr ds:[eax], dh         ; -> loc_1_3008
        add    byte ptr [eax], al
        inc    eax
        sbb    byte ptr [eax], dh
        add    byte ptr [eax], al
        inc    eax
        and    al, 0x30
        add    byte ptr [eax], al
        cmp    dword ptr [eax], esi          ; -> obj1:0x3030
        xor    byte ptr [eax], al
        add    byte ptr [ecx], bh
        cmp    byte ptr [eax], dh
        add    byte ptr [eax], al
loc_1_327A:
        jge    loc_1_3040
        xor    byte ptr [eax], al
        add    byte ptr [ebp + 0x4c], bh     ; -> obj1:0x304C
        xor    byte ptr [eax], al
        add    byte ptr [esp + edx*2], ch    ; -> obj1:0x3054
        xor    byte ptr [eax], al
        add    byte ptr [0x305c], bh         ; -> loc_1_305C
        jl     0x32f4                        ; -> obj1:0x3064
        xor    byte ptr [eax], al
        add    byte ptr [eax], dl
        push   0x1000030
        jo     loc_1_32CB
        add    byte ptr [eax], al
        push   ebx
        je     0x32d0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_32A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    eax, 0x6000000
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
loc_1_32CB:
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        or     eax, 0
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        add    byte ptr [ebx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dl
        add    byte ptr [eax], al
        adc    eax, 0x16000000
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [edx], bl
        add    byte ptr [eax], al
        add    byte ptr [ebx], bl
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        add    byte ptr [eax], al
        add    byte ptr [esi], bl
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [ebx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        and    eax, 0x26000000
        add    byte ptr [eax], al
        add    byte ptr [edi], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 0
        add    byte ptr [eax], al
        sub    eax, 0x2e000000
        add    byte ptr [eax], al
        add    byte ptr [edi], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
        add    byte ptr [eax], al
        add    byte ptr [ebx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dh
        add    byte ptr [eax], al
        xor    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [esi], dh
        add    byte ptr [eax], al
        add    byte ptr [0], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        loopne loc_1_339F
loc_1_339F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0xe0], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx - 0x20], dl
        add    byte ptr [eax], al
        push   ebx
        loopne loc_1_33D7
loc_1_33D7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx - 0x20], cl
        add    byte ptr [eax], al
        inc    edi
        loopne loc_1_33E7
loc_1_33E7:
        add    byte ptr [edi - 0x20], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    eax
        loopne loc_1_33F3
loc_1_33F3:
        add    byte ptr [eax - 0x20], dl
        add    byte ptr [eax], al
        dec    ecx
        loopne loc_1_33FB
loc_1_33FB:
        add    byte ptr [ecx - 0x20], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        loopne loc_1_340B
loc_1_340B:
        add    byte ptr [ebp], al
        add    byte ptr [eax], al
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0x480000e0], dh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [edi], dh
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    eax, dword ptr [eax]
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    eax, 0x3e000000
        add    byte ptr [eax], al
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [edi], dh
        loopne loc_1_3497
loc_1_3497:
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x34], ch     ; -> obj1:0x34A8
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esp + esi], dh      ; -> obj1:0x34B4
        add    byte ptr [eax], al
loc_1_34CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3544:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3548:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_354C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3560:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3564:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_3568:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_356C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3570:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3574:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3578:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_357C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35EC:
        add    byte ptr [eax], al
loc_1_35EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3600:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3604:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3618:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_361C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3620:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3624:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3630:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3634:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3638:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_363C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3640:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_3644:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3648:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_364C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3650:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_3654:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3658:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_365C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3660:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3664:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3668:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x58  R-X
loc_2_0000:
        VxDCall  ENABLE.VMINI_GetVersion         ; cd2000003700
        jb     loc_2_0055
        VxDCall  BIOS.Service_0000h              ; cd2000002b00
        jb     loc_2_0055
        mov    eax, 0x370001
        mov    esi, 8                        ; -> loc_1_0008
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0x291c], esi       ; -> sub_1_291C
        jb     loc_2_0055
        mov    edi, 0x295a                   ; -> loc_1_295A
        mov    dword ptr [edi], 0x14
        VxDCall  ENABLE.VMINI_RegisterSK         ; cd2009003700
        jb     loc_2_0055
        xor    edx, edx
        mov    eax, 0x64
        mov    esi, 0xdb                     ; -> loc_1_00DB
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x2904], esi       ; -> obj1:0x2904
        clc    
        ret    
loc_2_0055:
        stc    
        ret    
        db  024h
VxD__LTEXT ENDS

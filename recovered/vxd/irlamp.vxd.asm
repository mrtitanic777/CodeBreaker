; IRLAMPEX -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 16346 instructions recovered by recursive descent

Declare_Virtual_Device IRLAMPEX, 1, 0, IRLAMPEX_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 030000001h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xe184  R-X
IRLAMPEX_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        cmp    eax, 0x1c
        jne    loc_1_0017
        call   sub_1_6206
        cmp    eax, 1
        ret    
loc_1_0017:
        clc    
        ret    
sub_1_0019:
        VxDCall  Dev_04A2.Service_8021h          ; cd202180a204
sub_1_001F:
        VxDCall  VTD.VTD_Get_Date_And_Time       ; cd2008800500
        VxDCall  VTD.VTD_Get_Real_Time           ; cd2007800500
loc_1_002B:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [0xe094], eax       ; -> loc_1_E094
        jbe    loc_1_003E
        jmp    dword ptr [eax*4 + 0xe088]    ; -> obj1:0xE088
loc_1_003E:
        mov    eax, 1
        ret    
sub_1_0044:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0xDE08
        je     loc_1_022C
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        shl    eax, 3
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        add    eax, 0x523
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1044
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        shl    eax, 3
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        add    eax, 0x1a3
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 4], 0        ; -> loc_1_05F3
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 8], 0        ; -> loc_1_0AF0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0xc], 0      ; -> loc_1_0F52
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_0B33
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 2]
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0xb8], eax
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_00FD
loc_1_00FA:
        inc    dword ptr [ebp - 8]
loc_1_00FD:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_017F
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x64
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x68], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x68]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x64], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x5c
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x60], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x60]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x5c], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        shl    eax, 3
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        add    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x104
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x64
        push   eax
        call   sub_1_2011
        add    esp, 8
        jmp    loc_1_00FA
loc_1_017F:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x54
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x58], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x58]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x54], eax
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_01B0
loc_1_01AD:
        inc    dword ptr [ebp - 8]
loc_1_01B0:
        cmp    dword ptr [ebp - 8], 8
        jae    loc_1_01DB
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x35
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x54
        push   eax
        call   sub_1_2011
        add    esp, 8
        add    dword ptr [ebp - 4], 0x50
        jmp    loc_1_01AD
loc_1_01DB:
        mov    dword ptr [0], 8              ; -> obj1:0xE180
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0xb4], eax
        add    dword ptr [ebp - 4], 0x100
        call   sub_1_001F
        mov    dword ptr [0], eax            ; -> loc_1_DE10
        call   sub_1_1078
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x4c
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x50], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x50]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x4c], eax
loc_1_022C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0231:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0xDE08
        jne    loc_1_024E
        mov    eax, 1
        jmp    loc_1_0287
loc_1_024E:
        cmp    dword ptr [0], 0              ; -> obj1:0xDE0C
        jne    loc_1_027D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x70], eax
        mov    dword ptr [0], 1              ; -> obj1:0xDE0C
        xor    eax, eax
        jmp    loc_1_0287
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_027D:
        mov    eax, 4
        jmp    loc_1_0287
loc_1_0287:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_028C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0xDE0C
        je     loc_1_02B1
        mov    dword ptr [0], 0              ; -> obj1:0xDE0C
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 6]
loc_1_02B1:
        xor    eax, eax
        jmp    loc_1_02B8
loc_1_02B8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_02BD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_02E8
        mov    eax, 4
        jmp    loc_1_0395
loc_1_02E8:
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x1e]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0305
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0395
loc_1_0305:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc6], 0xb
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_0B58
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0xbc], 1
        push   0
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x16]
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc2], 1
        push   1
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x1a]
        add    esp, 4
        test   eax, eax
        je     loc_1_0366
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_036D
loc_1_0366:
        mov    dword ptr [ebp - 4], 0
loc_1_036D:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_037F
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0395
loc_1_037F:
        push   0x24e
        push   7
        call   sub_1_12A4
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_0395
loc_1_0395:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_039A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_1301
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x22]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc2], 0
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_03C7
loc_1_03C7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_03CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x6c], 0     ; -> loc_1_0B58
        je     loc_1_03F8
        mov    eax, 1
        jmp    loc_1_04C4
loc_1_03F8:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xbc], 0
        je     loc_1_0414
        mov    eax, 0x17
        jmp    loc_1_04C4
loc_1_0414:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x45], 0xff
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x46], 0x13
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        add    eax, 2
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x44], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc6]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x3d], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x4f], 7
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0xbc], 1
        cmp    dword ptr [ebp + 8], 0x2580
        je     loc_1_0499
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x2e]
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x3e], 1
        jmp    loc_1_04A0
loc_1_0499:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x3e], 0
loc_1_04A0:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_04BC
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
loc_1_04BC:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_04C4
loc_1_04C4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_04C9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0xbc]
        jmp    loc_1_04DF
loc_1_04DF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_04E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_056A
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 0x3e8
        ja     loc_1_0507
        mov    eax, 0x3e8
loc_1_0507:
        mov    dword ptr [ebp + 0xc], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x38], 0
        jne    loc_1_0547
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x38], 0     ; -> loc_1_058C
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x30], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x30
        push   eax
        call   sub_1_1526
        add    esp, 4
        jmp    loc_1_0565
loc_1_0547:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x30]
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    eax, ecx
        jb     loc_1_055C
        mov    eax, ecx
loc_1_055C:
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x30], eax
loc_1_0565:
        jmp    loc_1_0587
loc_1_056A:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x30
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x38], 0
loc_1_0587:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_058C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x6c], 0     ; -> loc_1_0B58
        jne    loc_1_05EE
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xbc], 0
        jne    loc_1_05DD
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x90], 0xffffffff
        push   1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        push   eax
        push   0
        call   sub_1_1DD1
        add    esp, 0xc
loc_1_05DD:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x30
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_05EE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_05F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x78], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x94], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x98], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax]
        and    eax, 0xfe
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [ecx + 0xc8], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xef
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [ecx + 0xc9], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 1]
        and    eax, 0x10
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [ecx + 0xca], al
        call   sub_1_072C
        test   eax, eax
        je     loc_1_070E
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        test   al, 1
        jne    loc_1_06B6
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_06E1
loc_1_06B6:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        test   al, 2
        jne    loc_1_06DA
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_06E1
loc_1_06DA:
        mov    dword ptr [ebp - 4], 3
loc_1_06E1:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 1
        je     loc_1_06FF
        add    dword ptr [ebp - 4], 3
loc_1_06FF:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        call   dword ptr [eax + 0x6c]
        add    esp, 4
loc_1_070E:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xa]
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_0727
loc_1_0727:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_072C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 3
        cmp    al, 3
        jne    loc_1_0886
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc8]
        and    eax, 0xff
        cmp    eax, 0xfe
        jne    loc_1_0886
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 3
        je     loc_1_0798
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0xf
        jne    loc_1_07A2
loc_1_0798:
        mov    eax, 1
        jmp    loc_1_098C
loc_1_07A2:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0x2f
        je     loc_1_07D6
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0xaf
        jne    loc_1_07E9
loc_1_07D6:
        mov    dword ptr [ebp - 4], 0xe
        mov    dword ptr [ebp - 8], 3
        jmp    loc_1_07F7
loc_1_07E9:
        mov    dword ptr [ebp - 8], 2
        mov    dword ptr [ebp - 4], 0xa
loc_1_07F7:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x98], ecx
        jb     loc_1_087F
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0xa0], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx + 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x9c], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x9c], -1
        je     loc_1_0875
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x84]
        cmp    dword ptr [eax + 0x9c], ecx
        jne    loc_1_087F
loc_1_0875:
        mov    eax, 1
        jmp    loc_1_098C
loc_1_087F:
        xor    eax, eax
        jmp    loc_1_098C
loc_1_0886:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x80], 0
        je     loc_1_0985
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    cl, byte ptr [ecx + 0xc8]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_1_0985
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0x63
        jne    loc_1_097B
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x98], 0xa
        jb     loc_1_0962
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0xa0], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    eax, dword ptr [eax + 6]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x9c], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x9c], -1
        je     loc_1_095D
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x84]
        cmp    dword ptr [eax + 0x9c], ecx
        je     loc_1_095D
        xor    eax, eax
        jmp    loc_1_098C
loc_1_095D:
        jmp    loc_1_097B
loc_1_0962:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x6c], 0     ; -> sub_1_2330
        jne    loc_1_097B
        xor    eax, eax
        jmp    loc_1_098C
loc_1_097B:
        mov    eax, 1
        jmp    loc_1_098C
loc_1_0985:
        xor    eax, eax
        jmp    loc_1_098C
loc_1_098C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0991:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        add    ecx, 0x54
        cmp    dword ptr [eax + 0x54], ecx
        jne    loc_1_09B8
        xor    eax, eax
        jmp    loc_1_0A87
loc_1_09B8:
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_09E1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xcb]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_09E1
        xor    eax, eax
        jmp    loc_1_0A87
loc_1_09E1:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x54
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0x35
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x3e], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x44], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc6]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0x3d], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x4f], 7
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x3f], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 2], 1
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_0A70
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xcb], 1
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax + 3], 1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0xb4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 5], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax + 9], 0
        jmp    loc_1_0A79
loc_1_0A70:
        mov    eax, dword ptr [ebp - 0xc]
        mov    word ptr [eax + 3], 0
loc_1_0A79:
        dec    dword ptr [0]                 ; -> obj1:0xE180
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_0A87
loc_1_0A87:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0A8C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_0ACD
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0xb4]
        cmp    dword ptr [eax + 5], ecx
        jne    loc_1_0ACD
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xcb], 0
loc_1_0ACD:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x35
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x54
        push   eax
        call   sub_1_1F70
        add    esp, 8
        inc    dword ptr [0]                 ; -> obj1:0xE180
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0AF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x3f]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_0B15
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0A8C
        add    esp, 4
loc_1_0B15:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x7c], eax
        push   0xb
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        call   dword ptr [eax + 0x6c]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0B33:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x4d]
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0B58:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x34], eax
        jmp    loc_1_0F08
loc_1_0B6C:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x38], eax
        jmp    loc_1_0C24
loc_1_0B84:
        call   sub_1_1C01
        test   eax, eax
        jne    loc_1_0B96
        call   sub_1_10EB
loc_1_0B96:
        jmp    loc_1_0D3F
loc_1_0B9B:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0BB8
        call   sub_1_28C1
loc_1_0BB8:
        jmp    loc_1_0D3F
loc_1_0BBD:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x78]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x41]
        add    esp, 8
        jmp    loc_1_0D3F
loc_1_0BDB:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0BF8
        call   sub_1_1143
loc_1_0BF8:
        jmp    loc_1_0D3F
loc_1_0BFD:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_0C1A
        call   sub_1_10EB
loc_1_0C1A:
        jmp    loc_1_0D3F
        db  0E9h, 01Bh, 001h, 000h, 000h
loc_1_0C24:
        sub    dword ptr [ebp - 0x38], 3
        cmp    dword ptr [ebp - 0x38], 0xe0
        ja     loc_1_0BFD
        mov    eax, dword ptr [ebp - 0x38]
        mov    al, byte ptr [eax]            ; -> obj1:0x0C5E
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x0C4A
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_0BBD
        dd  loc_1_0B84
        dd  loc_1_0B9B
        dd  loc_1_0BDB
        dd  loc_1_0BFD
        db  000h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 001h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  002h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  003h
loc_1_0D3F:
        jmp    loc_1_0F4D
loc_1_0D44:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0xaf
        jne    loc_1_0D7F
        push   1
        lea    eax, [ebp - 0x30]
        push   eax
        call   sub_1_1AB0
        add    esp, 8
        lea    eax, [ebp - 0x30]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x29]
        add    esp, 4
loc_1_0D7F:
        jmp    loc_1_0F4D
loc_1_0D84:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0xbc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x12]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0xbc], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0DD9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xbc], 0
        jne    loc_1_0DD9
        push   0
        push   0x18
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x49]
        add    esp, 8
        jmp    loc_1_0E07
loc_1_0DD9:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0E07
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xbc], 0
        je     loc_1_0E07
        push   0
        push   0x17
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x49]
        add    esp, 8
loc_1_0E07:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xbc], 0
        je     loc_1_0E3A
        push   0
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x16]
        add    esp, 4
        push   0x24e
        push   7
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_0E49
loc_1_0E3A:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0xac], 0
loc_1_0E49:
        jmp    loc_1_0F4D
loc_1_0E4E:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        mov    al, byte ptr [eax + 0x45]
        and    eax, 0xff
        cmp    eax, 0xff
        jne    loc_1_0EF9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        mov    al, byte ptr [eax + 0x46]
        and    eax, 0xff
        cmp    eax, 0x13
        jne    loc_1_0EF9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        mov    al, byte ptr [eax + 0x3f]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_1_0EF9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        mov    al, byte ptr [eax + 0x3e]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_0EC4
        push   0x2580
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x2e]
        add    esp, 4
loc_1_0EC4:
        push   0
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x16]
        add    esp, 4
        push   0x24e
        push   7
        call   sub_1_12A4
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x45]
        add    esp, 8
loc_1_0EF9:
        jmp    loc_1_0F4D
loc_1_0EFE:
        jmp    loc_1_0F4D
        db  0E9h, 045h, 000h, 000h, 000h
loc_1_0F08:
        sub    dword ptr [ebp - 0x34], 3
        cmp    dword ptr [ebp - 0x34], 9
        ja     loc_1_0F4D
        mov    eax, dword ptr [ebp - 0x34]
        mov    al, byte ptr [eax]            ; -> obj1:0x0F43
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x0F2B
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_0D44
        dd  loc_1_0B6C
        dd  loc_1_0D84
        dd  loc_1_0E4E
        dd  loc_1_0D84
        dd  loc_1_0EFE
        db  000h, 005h, 005h, 001h, 002h, 005h, 005h, 005h, 003h, 004h
loc_1_0F4D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_0F52:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0xc
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        call   dword ptr [eax + 0x6c]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0F6A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_0F9E
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0xbc], 0
        mov    dword ptr [ebp - 4], 0x24e
        jmp    loc_1_0FAD
loc_1_0F9E:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0xbc], 1
loc_1_0FAD:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_0B58
        push   0
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x16]
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   7
        call   sub_1_12A4
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x38], 0
        je     loc_1_0FF4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x30
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_0FF4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0FF9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], edx
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_1016
loc_1_1013:
        dec    dword ptr [ebp - 4]
loc_1_1016:
        cmp    dword ptr [ebp - 4], 0
        jbe    loc_1_1035
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0xc]
        inc    dword ptr [ebp - 8]
        jmp    loc_1_1013
loc_1_1035:
        mov    eax, dword ptr [ebp + 8]
        jmp    loc_1_103D
loc_1_103D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_1044:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_105B
loc_1_1058:
        dec    dword ptr [ebp + 0xc]
loc_1_105B:
        cmp    dword ptr [ebp + 0xc], 0
        jbe    loc_1_1073
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
        inc    dword ptr [ebp - 4]
        jmp    loc_1_1058
loc_1_1073:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1078:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_1081:
        call   sub_1_10B1
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_1081
        cmp    dword ptr [ebp - 4], -1
        je     loc_1_1081
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x84], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_10B1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_DE10
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*2]
        lea    eax, [ecx + eax*4]
        lea    eax, [eax + eax*8]
        add    eax, 0x3619
        mov    dword ptr [0], eax            ; -> loc_1_DE10
        mov    eax, dword ptr [0]            ; -> loc_1_DE10
        jmp    loc_1_10E6
loc_1_10E6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_10EB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_1115
        mov    eax, 0xe
        jmp    loc_1_113E
loc_1_1115:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x45], 0xfe
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x46], 0x1f
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x44], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        jmp    loc_1_113E
loc_1_113E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1143:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_116D
        mov    eax, 0xe
        jmp    loc_1_129F
loc_1_116D:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc8]
        and    eax, 0xff
        cmp    eax, 0xfe
        jne    loc_1_11BD
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x45], 0xfe
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x47], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0xa0]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x4b], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x44], 0xa
        jmp    loc_1_11DB
loc_1_11BD:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x45], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x44], 2
loc_1_11DB:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x46], 0xf3
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x98]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx + 0x44]
        and    ecx, 0xff
        sub    eax, ecx
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jbe    loc_1_128B
        cmp    dword ptr [ebp - 8], 0x100
        ja     loc_1_128B
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xcb]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_128B
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xcb], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 3], 1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0xb4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 5], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 9], ax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, dword ptr [ecx + 0x94]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    edx, eax
        mov    ecx, dword ptr [ecx + 0xb4]
        call   sub_1_0FF9
loc_1_128B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        jmp    loc_1_129F
loc_1_129F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_12A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x1c], 0     ; -> loc_1_12E4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_1526
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_12E4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        call   dword ptr [eax + 0x6c]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1301:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x37
loc_1_1311:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        add    ecx, 0x4c
        cmp    dword ptr [eax + 0x4c], ecx
        je     loc_1_1378
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x4c
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0x14
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_DDA0
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x10], ecx
        jae    loc_1_1373
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 4], eax
loc_1_1373:
        jmp    loc_1_1311
loc_1_1378:
        cmp    dword ptr [ebp - 4], 0x37
        jae    loc_1_138E
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0000
loc_1_138B:
        add    esp, 4
loc_1_138E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1393:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x14
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_14E2
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        call   dword ptr [eax + 8]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_13DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x37
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x4c]
        mov    dword ptr [ebp - 8], eax
loc_1_13F7:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x4c
        cmp    eax, dword ptr [ebp - 8]
        je     loc_1_1436
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0x14
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x10], ecx
        jae    loc_1_1429
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 4], eax
loc_1_1429:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_13F7
loc_1_1436:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_143E
loc_1_143E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1443:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x37
        cmp    dword ptr [ebp + 8], 6
        jae    loc_1_1469
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_14D5
loc_1_1469:
        cmp    dword ptr [ebp + 8], 0xc
        jae    loc_1_147F
        mov    dword ptr [ebp - 4], 2
        jmp    loc_1_14D5
loc_1_147F:
        cmp    dword ptr [ebp + 8], 0x1e
        jae    loc_1_1495
        mov    dword ptr [ebp - 4], 3
        jmp    loc_1_14D5
loc_1_1495:
        cmp    dword ptr [ebp + 8], 0x3c
        ja     loc_1_14AB
        mov    dword ptr [ebp - 4], 5
        jmp    loc_1_14D5
loc_1_14AB:
        cmp    dword ptr [ebp + 8], 0x64
        ja     loc_1_14C1
        mov    dword ptr [ebp - 4], 0xa
        jmp    loc_1_14D5
loc_1_14C1:
        cmp    dword ptr [ebp + 8], 0xb4
        ja     loc_1_14D5
        mov    dword ptr [ebp - 4], 0x14
loc_1_14D5:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_14DD
loc_1_14DD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_14E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_13DC
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_1521
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_3_0000
loc_1_1508:
        add    esp, 4
        cmp    dword ptr [ebp - 4], 0x37
        jae    loc_1_1521
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0017
loc_1_151E:
        add    esp, 4
loc_1_1521:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1526:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1443
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x10], eax
        call   sub_1_13DC
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x10], ecx
        jae    loc_1_158B
        cmp    dword ptr [ebp - 4], 0x37
        jae    loc_1_157C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0000
loc_1_1579:
        add    esp, 4
loc_1_157C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_3_0017
loc_1_1588:
        add    esp, 4
loc_1_158B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        push   0                             ; -> loc_1_1393
        call   sub_1_DD84
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_15D2
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x14
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x4c
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xc], eax
        jmp    loc_1_15D2
loc_1_15D2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_15D7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_161E
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x14
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_DDA0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_14E2
        add    esp, 4
loc_1_161E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h
sub_1_1624:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x6c], 0     ; -> loc_1_0B58
        jne    loc_1_1651
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xbc], 0
        je     loc_1_165B
loc_1_1651:
        mov    eax, 0x17
        jmp    loc_1_178D
loc_1_165B:
        cmp    dword ptr [ebp + 8], -1
        jne    loc_1_16DA
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xac], 1
        jbe    loc_1_16BE
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0xac], 0
        call   sub_1_DDB0
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        sub    eax, dword ptr [ecx + 0xb0]
        cmp    eax, 0x24e
        jae    loc_1_16B9
        push   0x24e
        call   sub_1_0F6A
        add    esp, 4
        mov    eax, 0x17
        jmp    loc_1_178D
loc_1_16B9:
        jmp    loc_1_16DA
loc_1_16BE:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        inc    dword ptr [eax + 0xac]
        call   sub_1_DDB0
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0xb0], eax
loc_1_16DA:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x8c], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc5], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x74], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x90], eax
        mov    al, byte ptr [ebp + 0x14]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [ecx + 0xc7], al
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [eax + 2], 0
        push   1
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x16]
        add    esp, 4
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        push   eax
        push   1
        call   sub_1_1DD1
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_176C
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_178D
loc_1_176C:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_1792
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        inc    byte ptr [eax + 0xc5]
        mov    eax, 2
        jmp    loc_1_178D
loc_1_178D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1792:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_1900
loc_1_17AD:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc5]
        and    eax, 0xff
        cmp    eax, 0xff
        jne    loc_1_17EE
        push   0
        call   sub_1_0F6A
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x74]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x25]
        add    esp, 4
        jmp    loc_1_17FA
loc_1_17EE:
        push   0x23
        push   8
        call   sub_1_12A4
        add    esp, 8
loc_1_17FA:
        jmp    loc_1_1940
loc_1_17FF:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0xaf
        jne    loc_1_188C
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_188C
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x74]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, dword ptr [ecx + 0x74]
        add    eax, 4
        mov    dword ptr [ebp - 8], eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1AB0
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x74]
        inc    word ptr [eax]
        call   sub_1_1B77
loc_1_188C:
        jmp    loc_1_1940
loc_1_1891:
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x2a]
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 1
        je     loc_1_18C0
        cmp    dword ptr [ebp - 0xc], 2
        je     loc_1_18C0
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x32]
        test   eax, eax
        je     loc_1_18EC
loc_1_18C0:
        cmp    dword ptr [ebp + 8], 8
        jne    loc_1_18E0
        push   0x37
        push   0xa
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_1940
        db  0E9h, 00Ch, 000h, 000h, 000h
loc_1_18E0:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x74]
        inc    word ptr [eax + 2]
loc_1_18EC:
        call   sub_1_1B77
        jmp    loc_1_1940
loc_1_18F6:
        jmp    loc_1_1940
        db  0E9h, 040h, 000h, 000h, 000h
loc_1_1900:
        sub    dword ptr [ebp - 0x14], 3
        cmp    dword ptr [ebp - 0x14], 8
        ja     loc_1_1940
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax]            ; -> obj1:0x1937
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x1923
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_17FF
        dd  loc_1_1891
        dd  loc_1_1891
        dd  loc_1_17AD
        dd  loc_1_18F6
        db  000h, 004h, 004h, 004h, 004h, 001h, 004h, 002h, 003h
loc_1_1940:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_1945:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x34], eax
        jmp    loc_1_1A92
loc_1_1959:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0x2f
        jne    loc_1_1A5F
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_1A5F
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [ebp - 0x30], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x30]
        cmp    eax, 0xff
        jne    loc_1_1A06
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x88]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x84], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   1
        lea    eax, [ebp - 0x2c]
        push   eax
        call   sub_1_1AB0
        add    esp, 8
        lea    eax, [ebp - 0x2c]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x29]
        add    esp, 4
        push   0x4b
        call   sub_1_0F6A
        add    esp, 4
        jmp    loc_1_1A5F
loc_1_1A06:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc4]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_1A5F
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc5]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x30]
        cmp    eax, ecx
        jg     loc_1_1A5F
        push   1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x88]
        push   eax
        push   0
        call   sub_1_1DD1
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc4], 1
loc_1_1A5F:
        jmp    loc_1_1AAB
loc_1_1A64:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x88]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x84], eax
        push   0x24e
        call   sub_1_0F6A
        add    esp, 4
        jmp    loc_1_1AAB
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_1A92:
        cmp    dword ptr [ebp - 0x34], 6
        je     loc_1_1959
        cmp    dword ptr [ebp - 0x34], 0xa
        je     loc_1_1A64
        jmp    loc_1_1AAB
loc_1_1AAB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1AB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x98]
        sub    eax, 0xe
        cmp    eax, 0x20
        jb     loc_1_1AD5
        mov    eax, 0x20
loc_1_1AD5:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0xa0]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    edx, dword ptr [eax + 0x94]
        mov    ecx, dword ptr [ebp + 8]
        add    edx, 0xe
        add    ecx, 8
        call   sub_1_0FF9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 0xd]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x28], al
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0xaf
        jne    loc_1_1B6B
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x9c], -1
        jne    loc_1_1B6B
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1], 1
        jmp    loc_1_1B72
loc_1_1B6B:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1], 0
loc_1_1B72:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1B77:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc5]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        cmp    eax, dword ptr [ecx + 0x8c]
        jne    loc_1_1BBC
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc5], 0xff
        mov    dword ptr [ebp - 4], 1
loc_1_1BBC:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        push   eax
        push   1
        call   sub_1_1DD1
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc5]
        and    eax, 0xff
        cmp    eax, 0xff
        je     loc_1_1BFC
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        inc    byte ptr [eax + 0xc5]
loc_1_1BFC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1C01:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        cmp    eax, 1
        je     loc_1_1C2D
        xor    eax, eax
        jmp    loc_1_1DCC
loc_1_1C2D:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 0xb]
        mov    byte ptr [ebp - 0x10], al
        mov    al, byte ptr [ebp - 0x10]
        and    eax, 3
        mov    eax, dword ptr [eax*4]        ; -> loc_1_DE14
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x8c], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [ebp - 0x14], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x14]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        cmp    eax, dword ptr [ecx + 0x8c]
        jb     loc_1_1C89
        mov    eax, 1
        jmp    loc_1_1DCC
loc_1_1C89:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x8c]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x14]
        sub    eax, ecx
        mov    dword ptr [ebp - 4], eax
        call   sub_1_10B1
        shr    eax, 6
        sub    edx, edx
        div    dword ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x14]
        add    eax, edx
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [ecx + 0xc5], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        test   al, 4
        je     loc_1_1D15
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc7], 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        mov    dword ptr [ebp - 8], eax
        call   sub_1_1078
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x88], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x84], eax
        jmp    loc_1_1D38
loc_1_1D15:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x88], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc7], 0
loc_1_1D38:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0xa0]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x90], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc5]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x14]
        cmp    eax, ecx
        jne    loc_1_1D95
        push   1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        push   eax
        push   0
        call   sub_1_1DD1
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc4], 1
        jmp    loc_1_1DA1
loc_1_1D95:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [eax + 0xc4], 0
loc_1_1DA1:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_1945
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*4 + 0xa]
        shl    eax, 5
        push   eax
        push   0xa
        call   sub_1_12A4
        add    esp, 8
        mov    eax, 1
        jmp    loc_1_1DCC
loc_1_1DCC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1DD1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        push   1
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_1E02
        mov    eax, 0xe
        jmp    loc_1_1F6B
loc_1_1E02:
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 5]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        or     al, 0xfe
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x45], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 1
        jne    loc_1_1E32
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x46], 0x3f
        jmp    loc_1_1E39
loc_1_1E32:
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x46], 0xbf
loc_1_1E39:
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x44], 2
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x90]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 5], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x8c], 1
        jne    loc_1_1E7B
        mov    byte ptr [ebp - 4], 0
        jmp    loc_1_1EE2
loc_1_1E7B:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x8c], 6
        ja     loc_1_1EA5
        mov    byte ptr [ebp - 4], 1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x8c], 6
        jmp    loc_1_1EE2
loc_1_1EA5:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x8c], 8
        ja     loc_1_1ECF
        mov    byte ptr [ebp - 4], 2
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x8c], 8
        jmp    loc_1_1EE2
loc_1_1ECF:
        mov    byte ptr [ebp - 4], 3
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x8c], 0x10
loc_1_1EE2:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc7]
        and    eax, 0xff
        or     al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc5]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0xa], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xb], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 9], 0xc
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_1F57
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax + 3], 2
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        add    eax, 4
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ecx + 0xb], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        mov    al, byte ptr [eax + 0x24]
        and    eax, 0xffff00ff
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 0xf], ax
loc_1_1F57:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        jmp    loc_1_1F6B
loc_1_1F6B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1F70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1FA3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_1FD1
loc_1_1FD1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_1FD6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2011:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2042:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_2053:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_2082
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 4], eax
        jne    loc_1_2075
        mov    eax, 1
        jmp    loc_1_2089
loc_1_2075:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_2053
loc_1_2082:
        xor    eax, eax
        jmp    loc_1_2089
loc_1_2089:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_208E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        jne    loc_1_20BF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_20C6
loc_1_20BF:
        xor    eax, eax
        jmp    loc_1_211F
loc_1_20C6:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_20DB
loc_1_20D3:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_20DB:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        je     loc_1_2115
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jne    loc_1_2109
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], ecx
        je     loc_1_2110
loc_1_2109:
        xor    eax, eax
        jmp    loc_1_211F
loc_1_2110:
        jmp    loc_1_20D3
loc_1_2115:
        mov    eax, 1
        jmp    loc_1_211F
loc_1_211F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2124:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x6c], 0     ; -> loc_1_0B58
        jne    loc_1_2151
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xbc], 0
        je     loc_1_215B
loc_1_2151:
        mov    eax, 0x17
        jmp    loc_1_2277
loc_1_215B:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0xac], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0xa8], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        add    ecx, 0x64
        cmp    dword ptr [eax + 0x64], ecx
        jne    loc_1_219A
        mov    eax, 0x14
        jmp    loc_1_2277
loc_1_219A:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x64
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0x38
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x80], eax
        call   sub_1_2F3B
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x58], eax
        call   sub_1_3345
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x7c], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_30CE
        add    esp, 4
        call   sub_1_3631
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 2
        je     loc_1_2246
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb7], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x38
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x64
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_2277
loc_1_2246:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_2261
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_2655
        jmp    loc_1_226D
loc_1_2261:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> sub_1_2330
loc_1_226D:
        mov    eax, 2
        jmp    loc_1_2277
loc_1_2277:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_227C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x6c], 0     ; -> loc_1_27A2
        je     loc_1_22A1
        mov    eax, 1
        jmp    loc_1_232B
loc_1_22A1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30CE
        add    esp, 4
        call   sub_1_320A
        push   0
        call   sub_1_2996
        add    esp, 4
        call   sub_1_36EC
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_2324
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb7], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x38
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x64
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x80], 0
        push   0x24e
        call   sub_1_0F6A
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_232B
loc_1_2324:
        xor    eax, eax
        jmp    loc_1_232B
loc_1_232B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2330:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_2605
loc_1_2344:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xa8], 2
        jae    loc_1_2360
        call   sub_1_341E
        jmp    loc_1_236F
loc_1_2360:
        push   0x24e
        push   0x11
        call   sub_1_2846
        add    esp, 8
loc_1_236F:
        jmp    loc_1_2650
loc_1_2374:
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x2a]
        cmp    eax, 1
        jne    loc_1_2396
        push   0x30
        push   0xa
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_2650
loc_1_2396:
        call   sub_1_3631
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        inc    dword ptr [eax + 0xa8]
        jmp    loc_1_2650
loc_1_23AB:
        push   0x1f4
        push   0xd
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_2650
loc_1_23BF:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0x63
        jne    loc_1_2487
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0xe
        mov    edx, 0                        ; -> loc_1_DE2C
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [eax + 0x80]
        add    ecx, 0xc8
        call   sub_1_0FF9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0xc8
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x98]
        sub    eax, 0xa
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        add    eax, 0xa
        push   eax
        call   sub_1_3459
        add    esp, 0xc
        push   1
        call   sub_1_2996
        add    esp, 4
        call   sub_1_2ED1
        push   1
        push   0
        push   1
        push   1
        call   sub_1_5EA2
        add    esp, 0x10
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_2753
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x31]
        add    esp, 4
        jmp    loc_1_24BD
loc_1_2487:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0xf
        jne    loc_1_24BD
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0
        push   0x12
        call   sub_1_2846
        add    esp, 8
loc_1_24BD:
        jmp    loc_1_2650
loc_1_24C2:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0x83
        jne    loc_1_25C0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x84]
        cmp    dword ptr [eax + 0xa0], ecx
        jbe    loc_1_25BB
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0xa0]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x58], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xfe
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xa6], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0xc8
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x98]
        sub    eax, 0xb
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        add    eax, 0xb
        push   eax
        call   sub_1_3459
        add    esp, 0xc
        call   sub_1_320A
        push   0
        call   sub_1_2996
        add    esp, 4
        call   sub_1_36EC
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_27A2
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x31]
        add    esp, 4
loc_1_25BB:
        jmp    loc_1_25F6
loc_1_25C0:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0x43
        jne    loc_1_25F6
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0
        push   0x13
        call   sub_1_2846
        add    esp, 8
loc_1_25F6:
        jmp    loc_1_2650
loc_1_25FB:
        jmp    loc_1_2650
        db  0E9h, 04Bh, 000h, 000h, 000h
loc_1_2605:
        sub    dword ptr [ebp - 4], 3
        cmp    dword ptr [ebp - 4], 0xb
        ja     loc_1_2650
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj1:0x2644
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x2628
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_23BF
        dd  loc_1_24C2
        dd  loc_1_2396
        dd  loc_1_23AB
        dd  loc_1_2344
        dd  loc_1_2374
        dd  loc_1_25FB
        db  000h, 006h, 006h, 001h, 006h, 006h, 006h, 002h, 003h, 006h, 004h, 005h
loc_1_2650:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_2655:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_270E
loc_1_2669:
        push   0x898
        push   9
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_274E
loc_1_267D:
        push   0x24e
        push   0x11
        call   sub_1_2846
        add    esp, 8
        jmp    loc_1_274E
loc_1_2691:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0xaf
        jne    loc_1_26F3
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0xa0]
        cmp    dword ptr [eax + 0x58], ecx
        jne    loc_1_26F3
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        call   sub_1_3631
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> sub_1_2330
        jmp    loc_1_274E
loc_1_26F3:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2330
        add    esp, 4
        jmp    loc_1_274E
loc_1_2704:
        jmp    loc_1_274E
        db  0E9h, 040h, 000h, 000h, 000h
loc_1_270E:
        sub    dword ptr [ebp - 4], 3
        cmp    dword ptr [ebp - 4], 8
        ja     loc_1_274E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj1:0x2745
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x2731
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_2691
        dd  loc_1_26F3
        dd  loc_1_267D
        dd  loc_1_2669
        dd  loc_1_2704
        db  000h, 004h, 004h, 001h, 004h, 004h, 002h, 004h, 003h
loc_1_274E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_2753:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0xb
        jne    loc_1_2781
        push   0x190
        push   0x11
        call   sub_1_12A4
        add    esp, 8
        push   1
        call   sub_1_39AA
        add    esp, 4
        jmp    loc_1_279D
loc_1_2781:
        cmp    dword ptr [ebp + 8], 0xf
        jne    loc_1_279D
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xa7], 1
loc_1_279D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_27A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_2804
loc_1_27B6:
        push   1
        push   0
        push   0
        push   0xf
        call   sub_1_5EA2
        add    esp, 0x10
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_2822
        jmp    loc_1_281D
loc_1_27D7:
        call   sub_1_2ED1
        push   0x384
        push   0x11
        call   sub_1_12A4
        add    esp, 8
        push   0
        call   sub_1_39AA
        add    esp, 4
        jmp    loc_1_281D
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_1_2804:
        cmp    dword ptr [ebp - 4], 0xb
        je     loc_1_27D7
        cmp    dword ptr [ebp - 4], 0xf
        je     loc_1_27B6
        jmp    loc_1_281D
loc_1_281D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_2822:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0xb
        jne    loc_1_2841
        push   0x24e
        push   0x12
        call   sub_1_2846
        add    esp, 8
loc_1_2841:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2846:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_54C4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x35]
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb7], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x38
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x64
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x80], 0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0F6A
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_28C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x64
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0x38
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x80], eax
        call   sub_1_2F3B
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0xa0]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x58], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xfe
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xa6], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0xc8
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x98]
        sub    eax, 0xb
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        add    eax, 0xb
        push   eax
        call   sub_1_3459
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_27A2
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x2d]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2996:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xa4], 0
        jne    loc_1_2A4E
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_2A09
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xc8]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_29EA
        xor    eax, eax
        mov    ax, word ptr [0]              ; -> loc_1_DE2C
        mov    dword ptr [ebp - 0xc], eax
loc_1_29EA:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xba]
        and    eax, 0xffff
        and    dword ptr [ebp - 0xc], eax
        jmp    loc_1_2A23
loc_1_2A09:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xba]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
loc_1_2A23:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, 1
        mov    cl, byte ptr [ebp - 0x14]
        shl    eax, cl
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    word ptr [ecx + 0xc0], ax
        jmp    loc_1_2A6B
loc_1_2A4E:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ax, word ptr [eax + 0xc0]
        and    eax, 0xffff
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
loc_1_2A6B:
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax*4]        ; -> loc_1_DE3C
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x8c], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xca]
        and    eax, 0xf
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 2
        jb     loc_1_2ABF
        mov    eax, 2
loc_1_2ABF:
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    ax, word ptr [eax*2]          ; -> obj1:0xDE64
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    word ptr [ecx + 0x98], ax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xce]
        and    eax, 0x7f
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_307B
        add    esp, 4
        inc    eax
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFE8
        mov    al, byte ptr [eax + 0x1e]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    eax, ecx
        jb     loc_1_2B22
        mov    eax, ecx
loc_1_2B22:
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0x9a], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xc0]
        and    eax, 0x7f
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_307B
        add    esp, 4
        inc    eax
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0x9b], al
        cmp    dword ptr [ebp - 0x14], 6
        jae    loc_1_2C28
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xd0]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    cl, byte ptr [ecx]            ; -> loc_1_DF94
        and    ecx, 0xff
        mov    al, byte ptr [eax]            ; -> obj1:0xDF8C
        and    eax, 0xff
        cdq    
        idiv   ecx
        and    eax, 0xff
        inc    eax
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xac], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xc2]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    cl, byte ptr [ecx]            ; -> loc_1_DF94
        and    ecx, 0xff
        mov    al, byte ptr [eax]            ; -> obj1:0xDF8C
        and    eax, 0xff
        cdq    
        idiv   ecx
        inc    eax
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_2C41
loc_1_2C28:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xac], 1
        mov    dword ptr [ebp - 0x10], 1
loc_1_2C41:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xd2]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        mov    al, byte ptr [ebp - 0x18]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xab], al
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        shl    ecx, 4
        mov    ax, word ptr [ecx + eax*2]    ; -> obj1:0xDE6C
        and    eax, 0xffff
        inc    eax
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xcc]
        and    eax, 0x3f
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    ax, word ptr [eax*2]          ; -> obj1:0xDF9C
        and    eax, 0xffff
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFE8
        mov    ax, word ptr [eax + 0x1c]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x20]
        cmp    eax, ecx
        jl     loc_1_2CE8
        mov    eax, ecx
loc_1_2CE8:
        mov    dword ptr [ebp - 0x20], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x14]
        shl    ecx, 4
        mov    eax, dword ptr [ecx + eax*4]  ; -> obj1:0xDEFC
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x9a
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xac]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2FE8
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 0x20]
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x84], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xc4]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x14]
        shl    ecx, 4
        mov    ax, word ptr [ecx + eax*2]    ; -> obj1:0xDE6C
        and    eax, 0xffff
        inc    eax
        mov    dword ptr [ebp - 0x24], eax
        mov    dword ptr [ebp - 0x20], 1
        lea    eax, [ebp - 0x20]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x9b
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2FE8
        add    esp, 0x14
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_2E1F
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xd4]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_2E00
        xor    eax, eax
        mov    ax, word ptr [0xc]            ; -> loc_1_DE38
        mov    dword ptr [ebp - 0xc], eax
loc_1_2E00:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xc6]
        and    eax, 0xffff
        and    dword ptr [ebp - 0xc], eax
        jmp    loc_1_2E39
loc_1_2E1F:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xc6]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
loc_1_2E39:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_307B
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax*4]        ; -> obj1:0xDFAC
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x94], eax
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_2E88
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x90], 0xffffffff
        jmp    loc_1_2E9D
loc_1_2E88:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x90], 0xbb8
loc_1_2E9D:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0x9b]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xb0], al
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_3931
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2ED1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x8c]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x2e]
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0x9c], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x38
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x5c
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        inc    dword ptr [eax + 0xa4]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2F3B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0xd7
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        call   sub_1_1044
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x40
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x44], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x44]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x40], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x48
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x4c], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x4c]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x48], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb7], 1
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_2FE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
loc_1_3001:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 5
        add    eax, dword ptr [ebp + 0xc]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        imul   eax, ecx
        add    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 8], eax
        jl     loc_1_3029
        jmp    loc_1_3066
loc_1_3029:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 1
        jne    loc_1_305E
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        add    ecx, dword ptr [ebp + 0xc]
        add    ecx, 5
        sub    eax, ecx
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 3
        jge    loc_1_3059
        mov    dword ptr [ebp - 0xc], 3
loc_1_3059:
        jmp    loc_1_3066
loc_1_305E:
        dec    byte ptr [ebp - 8]
        jmp    loc_1_3001
loc_1_3066:
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_307B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        and    dword ptr [ebp + 8], 0xffff
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_309A
loc_1_3097:
        inc    dword ptr [ebp - 4]
loc_1_309A:
        cmp    dword ptr [ebp - 4], 0xf
        jae    loc_1_30BC
        shl    dword ptr [ebp + 8], 1
        test   byte ptr [ebp + 0xa], 1
        je     loc_1_30B7
        jmp    loc_1_30BC
loc_1_30B7:
        jmp    loc_1_3097
loc_1_30BC:
        mov    eax, 0xf
        sub    eax, dword ptr [ebp - 4]
        jmp    loc_1_30C9
loc_1_30C9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_30CE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0xba
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFE8
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [0]            ; -> loc_1_DFE8
        and    eax, dword ptr [ecx + 0x18]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2], 1
        mov    eax, dword ptr [0]            ; -> loc_1_DFE8
        mov    eax, dword ptr [eax + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [0]            ; -> loc_1_DFE8
        mov    eax, dword ptr [eax + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [0]            ; -> loc_1_DFE8
        mov    eax, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [0]            ; -> loc_1_DFE8
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0xc], 0xff
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_3205
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xa4], 0
        jbe    loc_1_3182
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ax, word ptr [eax + 0xc0]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        jmp    loc_1_31A1
loc_1_3182:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx]
        and    ecx, 0xffff
        and    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
loc_1_31A1:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 7
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 2]
        and    ecx, 0xffff
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0xffff
        and    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0xc]
        and    ecx, 0xffff
        and    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xc], ax
loc_1_3205:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_320A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xc8]
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    cx, word ptr [ecx + 0xba]
        and    ecx, 0xffff
        and    eax, ecx
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    word ptr [ecx + 0xba], ax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xd4]
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    cx, word ptr [ecx + 0xc6]
        and    ecx, 0xffff
        and    eax, ecx
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    word ptr [ecx + 0xc6], ax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xba]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_3308
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xa4], 0
        jbe    loc_1_32EF
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ax, word ptr [eax + 0xc0]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    word ptr [ecx + 0xba], ax
        jmp    loc_1_3308
loc_1_32EF:
        mov    ax, word ptr [0]              ; -> loc_1_DE2C
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    word ptr [ecx + 0xba], ax
loc_1_3308:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xc6]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_3340
        mov    ax, word ptr [0xc]            ; -> loc_1_DE38
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    word ptr [ecx + 0xc6], ax
loc_1_3340:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3345:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
loc_1_334E:
        call   sub_1_10B1
        mov    ecx, 0xff
        sub    edx, edx
        div    ecx
        mov    eax, edx
        and    eax, 0xfe
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xa6], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3414
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x5c]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], 0
loc_1_33A5:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x5c
        cmp    eax, dword ptr [ebp - 8]
        je     loc_1_3405
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0x38
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0x10]
        mov    cl, byte ptr [ecx + 0xa6]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_1_33F8
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_3405
loc_1_33F8:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_33A5
loc_1_3405:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_3414
        jmp    loc_1_3419
loc_1_3414:
        jmp    loc_1_334E
loc_1_3419:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_341E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_10B1
        mov    ecx, 0x30
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 4], edx
        cmp    dword ptr [ebp - 4], 0x10
        jae    loc_1_3446
        add    dword ptr [ebp - 4], 0x10
loc_1_3446:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0xe
        call   sub_1_12A4
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3459:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
loc_1_3469:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_3525
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0x7f
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 2
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 8], eax
        jbe    loc_1_34A8
        jmp    loc_1_3525
loc_1_34A8:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 8
        jg     loc_1_3517
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 7
        je     loc_1_3517
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 8
        jne    loc_1_34DB
        mov    byte ptr [ebp - 4], 6
        jmp    loc_1_34DE
loc_1_34DB:
        dec    byte ptr [ebp - 4]
loc_1_34DE:
        cmp    dword ptr [ebp - 0xc], 1
        jne    loc_1_3504
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xffff00ff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x10]
        mov    word ptr [edx + ecx*2], ax
        jmp    loc_1_3517
loc_1_3504:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x10]
        mov    word ptr [edx + ecx*2], ax
loc_1_3517:
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, 2
        add    dword ptr [ebp + 8], eax
        jmp    loc_1_3469
loc_1_3525:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_352A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0xba
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_1_355C
loc_1_3559:
        inc    dword ptr [ebp - 0xc]
loc_1_355C:
        cmp    dword ptr [ebp - 0xc], 7
        jae    loc_1_3624
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]            ; -> loc_1_DE24
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + eax*2]
        and    eax, 0xffff
        test   ah, ah
        je     loc_1_359B
        mov    dword ptr [ebp - 8], 2
        jmp    loc_1_35A2
loc_1_359B:
        mov    dword ptr [ebp - 8], 1
loc_1_35A2:
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 1], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + eax*2]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_35DF
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax*2]          ; -> loc_1_DE2C
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 2], al
        jmp    loc_1_3616
loc_1_35DF:
        cmp    dword ptr [ebp - 8], 2
        jne    loc_1_3603
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + eax*2]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 2], ax
        jmp    loc_1_3616
loc_1_3603:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [ecx + eax*2]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 2], al
loc_1_3616:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2
        add    dword ptr [ebp - 0x10], eax
        jmp    loc_1_3559
loc_1_3624:
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_1_362C
loc_1_362C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3631:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   1
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_365B
        mov    eax, 0xe
        jmp    loc_1_36E7
loc_1_365B:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 5]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x45], 0xff
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x46], 0x93
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x44], 2
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x58]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp - 8]
        add    eax, 9
        push   eax
        call   sub_1_352A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 9
        mov    ecx, dword ptr [ebp - 0xc]
        mov    word ptr [ecx + 9], ax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        jmp    loc_1_36E7
loc_1_36E7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_36EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   1
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_3716
        mov    eax, 0xe
        jmp    loc_1_37A2
loc_1_3716:
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax + 5]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    byte ptr [ecx + 0x45], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x46], 0x73
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x44], 2
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x58]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        call   sub_1_352A
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 0x3e], 1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx + 9], ax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        jmp    loc_1_37A2
loc_1_37A2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h
sub_1_37A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb7]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_37CE
        mov    eax, 1
        jmp    loc_1_3877
loc_1_37CE:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_37F9
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x35
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x40
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x50]
        jmp    loc_1_3815
loc_1_37F9:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x35
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x48
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x54]
loc_1_3815:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x3e], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        cmp    dword ptr [eax + 0x74], 0     ; -> loc_1_3A58
        jne    loc_1_386D
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb6]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_386D
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0x13
        call   sub_1_3A03
        add    esp, 4
loc_1_386D:
        mov    eax, 2
        jmp    loc_1_3877
loc_1_3877:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_387C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb7]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_38A2
        mov    eax, 1
        jmp    loc_1_392C
loc_1_38A2:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x80], ecx
        jne    loc_1_3915
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        cmp    dword ptr [eax + 0x74], 0     ; -> loc_1_3A58
        jne    loc_1_38EC
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb6]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_38FE
loc_1_38EC:
        push   0xf
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        call   dword ptr [eax + 0x6c]
        add    esp, 4
        jmp    loc_1_3910
loc_1_38FE:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xa7], 1
loc_1_3910:
        jmp    loc_1_3922
loc_1_3915:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x80], 0xf
loc_1_3922:
        mov    eax, 2
        jmp    loc_1_392C
loc_1_392C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3931:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0xa0], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0xa2], 5
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0xa4], 0xa
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4]        ; -> loc_1_DFCC
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x6c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax*4]        ; -> obj1:0xDFDC
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x70], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_39AA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    byte ptr [ecx + 0xc3], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> sub_1_3A03
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_39EC
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_3C77
        jmp    loc_1_39FE
loc_1_39EC:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_3E57
loc_1_39FE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_3A03:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        call   dword ptr [eax + 0x74]
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x80], 0
        je     loc_1_3A53
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        call   dword ptr [eax + 0x74]
        add    esp, 4
loc_1_3A53:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_3A58:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 8], 1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x80], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_3C22
loc_1_3A85:
        call   sub_1_4C16
        jmp    loc_1_3C72
loc_1_3A8F:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3AAB
        mov    byte ptr [ebp - 8], 0
loc_1_3AAB:
        push   1
        push   0
        mov    al, byte ptr [ebp - 8]
        push   eax
        push   1
        call   sub_1_5EA2
        add    esp, 0x10
        jmp    loc_1_3C72
loc_1_3AC2:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x78]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x78], 0
        jmp    loc_1_3C72
loc_1_3AF3:
        call   sub_1_5FC8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_426D
        jmp    loc_1_3C72
loc_1_3B0F:
        call   sub_1_1143
        jmp    loc_1_3C72
loc_1_3B19:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb6], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3B58
        push   1
        push   0
        push   1
        push   1
        call   sub_1_5EA2
        add    esp, 0x10
        jmp    loc_1_3B5D
loc_1_3B58:
        call   sub_1_44A0
loc_1_3B5D:
        jmp    loc_1_3C72
loc_1_3B62:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xae]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3B85
        call   sub_1_4F55
loc_1_3B85:
        jmp    loc_1_3C72
loc_1_3B8A:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        call   sub_1_4935
        jmp    loc_1_3C72
loc_1_3BA5:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        mov    al, byte ptr [eax + 0x46]
        mov    byte ptr [ebp - 0xc], al
        mov    al, byte ptr [ebp - 0xc]
        and    eax, 0xef
        cmp    al, 3
        jne    loc_1_3C01
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        mov    al, byte ptr [eax + 0x3f]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_3C01
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
loc_1_3C01:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        test   al, 0x10
        je     loc_1_3C13
        call   sub_1_44A0
loc_1_3C13:
        jmp    loc_1_3C72
loc_1_3C18:
        jmp    loc_1_3C72
        db  0E9h, 050h, 000h, 000h, 000h
loc_1_3C22:
        sub    dword ptr [ebp - 0x10], 0xb
        cmp    dword ptr [ebp - 0x10], 0xd
        ja     loc_1_3C72
        mov    eax, dword ptr [ebp - 0x10]
        jmp    dword ptr [eax*4]             ; -> obj1:0x3C3A
        ; ---- dispatch table (14 entries) ----
        dd  loc_1_3BA5
        dd  loc_1_3C18
        dd  loc_1_3C18
        dd  loc_1_3C18
        dd  loc_1_3B8A
        dd  loc_1_3AF3
        dd  loc_1_3C18
        dd  loc_1_3A85
        dd  loc_1_3B19
        dd  loc_1_3B62
        dd  loc_1_3A8F
        dd  loc_1_3AC2
        dd  loc_1_3B0F
        dd  loc_1_3A8F
loc_1_3C72:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_3C77:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_3DFB
loc_1_3C8B:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3CB4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3CB4:
        call   sub_1_5B85
        jmp    loc_1_3E52
loc_1_3CBE:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3CE7
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3CE7:
        call   sub_1_5CD9
        jmp    loc_1_3E52
loc_1_3CF1:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3D1A
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3D1A:
        call   sub_1_58DF
        jmp    loc_1_3E52
loc_1_3D24:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3D57
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0x12
        call   sub_1_458B
        add    esp, 4
loc_1_3D57:
        jmp    loc_1_3E52
loc_1_3D5C:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xa7], 1
        jmp    loc_1_3E52
loc_1_3D73:
        push   0x16
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_3E52
loc_1_3D82:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0x9e]
        and    eax, 0xffff
        cmp    eax, 4
        jl     loc_1_3DB1
        push   1
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_3DE2
loc_1_3DB1:
        push   1
        push   0
        push   1
        push   1
        call   sub_1_5EA2
        add    esp, 0x10
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    word ptr [eax + 0x9e]
        push   0x190
        push   0x11
        call   sub_1_12A4
        add    esp, 8
loc_1_3DE2:
        jmp    loc_1_3E52
loc_1_3DE7:
        call   sub_1_5DE5
        jmp    loc_1_3E52
loc_1_3DF1:
        jmp    loc_1_3E52
        db  0E9h, 057h, 000h, 000h, 000h
loc_1_3DFB:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0x10
        ja     loc_1_3E52
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj1:0x3E41
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x3E1D
        ; ---- dispatch table (9 entries) ----
        dd  loc_1_3C8B
        dd  loc_1_3CBE
        dd  loc_1_3CF1
        dd  loc_1_3D73
        dd  loc_1_3D24
        dd  loc_1_3DE7
        dd  loc_1_3D5C
        dd  loc_1_3D82
        dd  loc_1_3DF1
        db  000h, 001h, 002h, 003h, 003h, 004h, 008h, 008h, 008h, 008h, 008h, 008h, 005h, 008h, 006h, 008h
        db  007h
loc_1_3E52:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_3E57:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_3F4D
loc_1_3E6B:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3E94
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3E94:
        call   sub_1_5B85
        jmp    loc_1_3FA0
loc_1_3E9E:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3EC7
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3EC7:
        call   sub_1_5CD9
        jmp    loc_1_3FA0
loc_1_3ED1:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_3EFA
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_3EFA:
        call   sub_1_58DF
        jmp    loc_1_3FA0
loc_1_3F04:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xa7], 1
        jmp    loc_1_3FA0
loc_1_3F1B:
        push   0x16
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_3FA0
loc_1_3F2A:
        push   1
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_3FA0
loc_1_3F39:
        call   sub_1_5DE5
        jmp    loc_1_3FA0
loc_1_3F43:
        jmp    loc_1_3FA0
        db  0E9h, 053h, 000h, 000h, 000h
loc_1_3F4D:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0x10
        ja     loc_1_3FA0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj1:0x3F8F
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x3F6F
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_3F1B
        dd  loc_1_3E6B
        dd  loc_1_3E9E
        dd  loc_1_3ED1
        dd  loc_1_3F39
        dd  loc_1_3F04
        dd  loc_1_3F2A
        dd  loc_1_3F43
        db  000h, 000h, 000h, 001h, 002h, 003h, 007h, 007h, 007h, 007h, 007h, 007h, 004h, 007h, 005h, 007h
        db  006h
loc_1_3FA0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_3FA5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_40A9
loc_1_3FB9:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb2]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_3FFE
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0x98]
        and    eax, 0xffff
        push   eax
        push   0xd
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_4008
loc_1_3FFE:
        push   0x13
        call   sub_1_5628
        add    esp, 4
loc_1_4008:
        jmp    loc_1_40CC
loc_1_400D:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0x63
        je     loc_1_403F
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0xf
        jne    loc_1_4049
loc_1_403F:
        push   0x13
        call   sub_1_5628
        add    esp, 4
loc_1_4049:
        jmp    loc_1_40CC
loc_1_404E:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0x9c]
        and    eax, 0xffff
        cmp    eax, 3
        jl     loc_1_407D
        push   0x13
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_409F
loc_1_407D:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    word ptr [eax + 0x9c]
        push   1
        push   0
        push   1
        push   0x43
        call   sub_1_5EA2
        add    esp, 0x10
loc_1_409F:
        jmp    loc_1_40CC
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_40A9:
        cmp    dword ptr [ebp - 4], 3
        je     loc_1_400D
        cmp    dword ptr [ebp - 4], 0xb
        je     loc_1_3FB9
        cmp    dword ptr [ebp - 4], 0xd
        je     loc_1_404E
        jmp    loc_1_40CC
loc_1_40CC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_40D1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_4221
loc_1_40E5:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb2]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_412D
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0x98]
        and    eax, 0xffff
        lea    eax, [eax + eax*2]
        push   eax
        push   0xd
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_4268
loc_1_412D:
        push   0x13
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_4268
loc_1_413C:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0x43
        jne    loc_1_419A
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_419A
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        call   sub_1_5FC8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_426D
        jmp    loc_1_4268
loc_1_419A:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_4203
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0x9c]
        and    eax, 0xffff
        cmp    eax, 3
        jl     loc_1_41E1
        push   0x13
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_4203
loc_1_41E1:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    word ptr [eax + 0x9c]
        push   1
        push   0
        push   0
        push   0x43
        call   sub_1_5EA2
        add    esp, 0x10
loc_1_4203:
        jmp    loc_1_4268
loc_1_4208:
        push   0x13
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_4268
loc_1_4217:
        jmp    loc_1_4268
        db  0E9h, 047h, 000h, 000h, 000h
loc_1_4221:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0xc
        ja     loc_1_4268
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj1:0x425B
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x4243
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_412D
        dd  loc_1_419A
        dd  loc_1_413C
        dd  loc_1_40E5
        dd  loc_1_4208
        dd  loc_1_4217
        db  000h, 000h, 000h, 001h, 001h, 002h, 005h, 005h, 005h, 005h, 003h, 005h, 004h
loc_1_4268:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_426D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0xb
        jne    loc_1_4287
        push   0x13
        call   sub_1_5628
        add    esp, 4
loc_1_4287:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_428C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_42F1
loc_1_42A0:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_42BD
        jmp    loc_1_4334
loc_1_42BD:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
loc_1_42CE:
        call   sub_1_4935
        jmp    loc_1_4334
loc_1_42D8:
        push   0x16
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_4334
loc_1_42E7:
        jmp    loc_1_4334
        db  0E9h, 043h, 000h, 000h, 000h
loc_1_42F1:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0xc
        ja     loc_1_4334
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj1:0x4327
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x4313
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_42A0
        dd  loc_1_42D8
        dd  loc_1_42A0
        dd  loc_1_42CE
        dd  loc_1_42E7
        db  000h, 000h, 000h, 001h, 001h, 002h, 004h, 004h, 004h, 004h, 004h, 004h, 003h
loc_1_4334:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_4339:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_4456
loc_1_434D:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_43DE
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        cmp    eax, 0x43
        jne    loc_1_43D4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x78]
        push   eax
        call   sub_1_0A8C
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x78], 0
        call   sub_1_5FC8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_426D
        jmp    loc_1_43DE
loc_1_43D4:
        push   0x16
        call   sub_1_458B
        add    esp, 4
loc_1_43DE:
        jmp    loc_1_449B
loc_1_43E3:
        push   0x16
        call   sub_1_5628
        add    esp, 4
        jmp    loc_1_449B
loc_1_43F2:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xa7], 1
        jmp    loc_1_449B
loc_1_4409:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb3], 0
        call   sub_1_4609
        call   sub_1_4782
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0x98]
        and    eax, 0xffff
        push   eax
        push   0xd
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_449B
loc_1_444C:
        jmp    loc_1_449B
        db  0E9h, 045h, 000h, 000h, 000h
loc_1_4456:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0xe
        ja     loc_1_449B
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]            ; -> obj1:0x448C
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x4478
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_43E3
        dd  loc_1_434D
        dd  loc_1_4409
        dd  loc_1_43F2
        dd  loc_1_444C
        db  000h, 000h, 000h, 001h, 001h, 001h, 004h, 004h, 004h, 004h, 004h, 004h, 002h, 004h, 003h
loc_1_449B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_44A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0x98]
        and    eax, 0xffff
        push   eax
        push   0xd
        call   sub_1_12A4
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x80], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb3], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0x9b]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    cl, byte ptr [ecx + 0xa9]
        and    ecx, 0xff
        add    eax, ecx
        and    al, 7
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xb0], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb1], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_4574
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_3C77
        jmp    loc_1_4586
loc_1_4574:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_3E57
loc_1_4586:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_458B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_3A58
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x80], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa7]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_45E0
        cmp    dword ptr [ebp + 8], 0xf
        jne    loc_1_45FA
loc_1_45E0:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x80], 0xf
        jmp    loc_1_4604
loc_1_45FA:
        call   sub_1_4609
        call   sub_1_4782
loc_1_4604:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4609:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_471F
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb8]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_4679
        call   sub_1_DDB0
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x5c], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb8], 1
        jmp    loc_1_471A
loc_1_4679:
        call   sub_1_DDB0
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        sub    eax, dword ptr [ecx + 0x5c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x90], ecx
        ja     loc_1_46F6
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb2]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_46F6
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0x1b
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x49]
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb2], 1
loc_1_46F6:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x94], ecx
        ja     loc_1_471A
        push   0x1b
        call   sub_1_5628
        add    esp, 4
loc_1_471A:
        jmp    loc_1_477D
loc_1_471F:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb2]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_4759
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0x19
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x49]
        add    esp, 8
loc_1_4759:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb8], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb2], 0
loc_1_477D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4782:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xae]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_48C0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb9]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_47F2
        call   sub_1_DDB0
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x60], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb9], 1
        jmp    loc_1_48BB
loc_1_47F2:
        call   sub_1_DDB0
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        sub    eax, dword ptr [ecx + 0x60]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0x1770
        jb     loc_1_4862
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb4]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_4862
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0x2e
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x49]
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb4], 1
loc_1_4862:
        cmp    dword ptr [ebp - 4], 0x9c40
        jb     loc_1_48BB
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb5]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_48BB
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0x2f
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x49]
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb5], 1
loc_1_48BB:
        jmp    loc_1_4930
loc_1_48C0:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb4]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_48FA
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0x30
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x49]
        add    esp, 8
loc_1_48FA:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb9], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb4], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb5], 0
loc_1_4930:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4935:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_4971
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_3FA5
        mov    byte ptr [ebp - 4], 1
        jmp    loc_1_4987
loc_1_4971:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_40D1
        mov    byte ptr [ebp - 4], 0
loc_1_4987:
        push   1
        push   0
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   0x43
        call   sub_1_5EA2
        add    esp, 0x10
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x80], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0x9c], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_49C7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        shr    eax, 5
        and    al, 7
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaa]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        je     loc_1_4A2B
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + eax*4], 0
        je     loc_1_4A5D
loc_1_4A2B:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xf
        cmp    al, 0xd
        jne    loc_1_4A53
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + eax*4], 0
        je     loc_1_4A5D
loc_1_4A53:
        mov    eax, 1
        jmp    loc_1_4A64
loc_1_4A5D:
        xor    eax, eax
        jmp    loc_1_4A64
loc_1_4A64:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4A69:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        shr    eax, 5
        and    al, 7
        mov    byte ptr [ebp - 0x2c], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [ebp - 0x30], eax
        mov    byte ptr [ebp - 4], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaa]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    cl, byte ptr [ecx + 0xa8]
        and    ecx, 0xff
        sub    eax, ecx
        and    al, 7
        mov    byte ptr [ebp - 0x28], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x2c]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x28]
        sub    eax, ecx
        and    al, 7
        mov    byte ptr [ebp - 0x34], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa8]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x34]
        cmp    eax, ecx
        jge    loc_1_4B06
        int3   
loc_1_4B06:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaa]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x2c]
        cmp    eax, ecx
        jne    loc_1_4B3B
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb3], 1
loc_1_4B3B:
        jmp    loc_1_4B43
loc_1_4B40:
        dec    byte ptr [ebp - 0x34]
loc_1_4B43:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x34]
        test   eax, eax
        jle    loc_1_4BC8
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    eax, dword ptr [ecx + eax*4]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    dword ptr [ebp + ecx*4 - 0x24], eax
        inc    byte ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x28]
        mov    ecx, dword ptr [ebp - 0x30]
        mov    dword ptr [ecx + eax*4], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x28]
        inc    eax
        and    al, 7
        mov    byte ptr [ebp - 0x28], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa8]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_4BA0
        int3   
loc_1_4BA0:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        dec    byte ptr [eax + 0xa8]
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb3], 1
        jmp    loc_1_4B40
loc_1_4BC8:
        mov    byte ptr [ebp - 0x34], 0
        jmp    loc_1_4BD4
loc_1_4BD1:
        inc    byte ptr [ebp - 0x34]
loc_1_4BD4:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x34]
        cmp    eax, ecx
        jle    loc_1_4C11
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x34]
        mov    eax, dword ptr [ebp + eax*4 - 0x24]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
        jmp    loc_1_4BD1
loc_1_4C11:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4C16:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xad], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xae]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_4CAE
        call   sub_1_4DDE
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x54]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        add    eax, dword ptr [ecx + 0x50]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    cl, byte ptr [ecx + 0x9a]
        and    ecx, 0xff
        cmp    eax, ecx
        jb     loc_1_4C93
        mov    eax, ecx
loc_1_4C93:
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_52BD
        add    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_5046
        add    esp, 4
loc_1_4CAE:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xad]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_4DD9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_4DC9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xa0]
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    cx, word ptr [ecx + 0xa2]
        and    ecx, 0xffff
        cmp    eax, ecx
        jge    loc_1_4D31
        push   1
        push   0
        push   1
        push   1
        call   sub_1_5EA2
        add    esp, 0x10
        jmp    loc_1_4DB2
loc_1_4D31:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb6], 1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0xa0]
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    cx, word ptr [ecx + 0xa4]
        and    ecx, 0xffff
        cmp    eax, ecx
        jge    loc_1_4D99
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x6c]
        push   eax
        push   0x13
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_4DB2
loc_1_4D99:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x70]
        push   eax
        push   0x13
        call   sub_1_12A4
        add    esp, 8
loc_1_4DB2:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    word ptr [eax + 0xa0]
        jmp    loc_1_4DD9
loc_1_4DC9:
        push   1
        push   0
        push   0
        push   1
        call   sub_1_5EA2
        add    esp, 0x10
loc_1_4DD9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4DDE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa8]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        jbe    loc_1_4F50
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaa]
        mov    byte ptr [ebp - 8], al
        jmp    loc_1_4E26
loc_1_4E23:
        dec    dword ptr [ebp - 0xc]
loc_1_4E26:
        cmp    dword ptr [ebp - 0xc], 0
        jbe    loc_1_4EAF
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        dec    eax
        and    al, 7
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    eax, dword ptr [ecx + eax*4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        sub    eax, 2
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x44], al
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x35
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x40
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    dword ptr [eax + 0x50]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + eax*4], 0
        jmp    loc_1_4E23
loc_1_4EAF:
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xaa], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa8]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x64], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xd6]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_4F3E
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xd6], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa8]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x68], eax
loc_1_4F3E:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xa8], 0
loc_1_4F50:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_4F55:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaa]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    cl, byte ptr [ecx + 0xa8]
        and    ecx, 0xff
        sub    eax, ecx
        and    al, 7
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    eax, dword ptr [ecx + eax*4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa9]
        and    eax, 0xff
        shl    eax, 5
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        add    ecx, ecx
        or     eax, ecx
        or     al, 0x10
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x46], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xac]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x3d], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xab]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x4f], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0xa0], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    dword ptr [eax + 0x20]
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    dword ptr [eax + 0x24]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_5046:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaa]
        mov    byte ptr [ebp - 0xc], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
loc_1_506B:
        cmp    dword ptr [ebp - 0x10], 0
        jbe    loc_1_529B
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        add    ecx, 0x40
        cmp    dword ptr [eax + 0x40], ecx
        je     loc_1_529B
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x40
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        dec    dword ptr [eax + 0x50]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0x35
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x45], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_510B
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x45]
        and    eax, 0xff
        or     al, 1
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x45], al
loc_1_510B:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa9]
        and    eax, 0xff
        shl    eax, 5
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0xc]
        add    ecx, ecx
        or     al, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x46], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    byte ptr [eax + 0xa8]
        mov    eax, dword ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0xc]
        mov    edx, dword ptr [0]            ; -> obj1:0xDE08
        mov    edx, dword ptr [edx + 0x80]
        mov    dword ptr [edx + ecx*4], eax
        cmp    dword ptr [ebp - 0x10], 1
        jne    loc_1_5178
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x46]
        and    eax, 0xff
        or     al, 0x10
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x46], al
loc_1_5178:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xac]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x3d], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xad]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_51C9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xab]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x4f], al
        jmp    loc_1_51D0
loc_1_51C9:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x4f], 7
loc_1_51D0:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    byte ptr [eax + 0xad]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        add    eax, 2
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0x44], al
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0xa0], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        inc    eax
        and    al, 7
        mov    byte ptr [ebp - 0xc], al
        dec    dword ptr [ebp - 0x10]
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    dword ptr [eax + 0x20]
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        cmp    dword ptr [eax + 0x64], 0
        jbe    loc_1_5265
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        dec    dword ptr [eax + 0x64]
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    dword ptr [eax + 0x24]
loc_1_5265:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        cmp    dword ptr [eax + 0x68], 0
        jbe    loc_1_5296
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        dec    dword ptr [eax + 0x68]
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    dword ptr [eax + 0x28]
loc_1_5296:
        jmp    loc_1_506B
loc_1_529B:
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xaa], al
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_52BD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
loc_1_52D2:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        add    ecx, 0x48
        cmp    dword ptr [eax + 0x48], ecx
        je     loc_1_54B7
        cmp    dword ptr [ebp - 0x10], 0
        jbe    loc_1_54B7
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x48
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        dec    dword ptr [eax + 0x54]
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0x35
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_534C
        mov    byte ptr [ebp - 4], 1
loc_1_534C:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        and    eax, 0xff
        or     al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0x45], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x46], 3
        cmp    dword ptr [ebp - 0x10], 1
        jne    loc_1_53AD
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb1]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_53AD
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x46]
        and    eax, 0xff
        or     al, 0x10
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0x46], al
loc_1_53AD:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xac]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0x3d], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xad]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_53FE
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xab]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0x4f], al
        jmp    loc_1_5405
loc_1_53FE:
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 0x4f], 7
loc_1_5405:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    byte ptr [eax + 0xad]
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 0x44]
        and    eax, 0xff
        add    eax, 2
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 0x44], al
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        test   eax, eax
        jne    loc_1_5461
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
loc_1_5461:
        cmp    dword ptr [ebp - 0x10], 1
        jne    loc_1_549B
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xb1]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_549B
        push   0
        push   0
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   1
        call   sub_1_5EA2
        add    esp, 0x10
loc_1_549B:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0xa0], 0
        dec    dword ptr [ebp - 0x10]
        jmp    loc_1_52D2
loc_1_54B7:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_54C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_54DC
loc_1_54D9:
        inc    dword ptr [ebp - 8]
loc_1_54DC:
        cmp    dword ptr [ebp - 8], 8
        jge    loc_1_5531
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + ecx*4], 0
        je     loc_1_552C
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx*4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0x15
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
loc_1_552C:
        jmp    loc_1_54D9
loc_1_5531:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        add    ecx, 0x48
        cmp    dword ptr [eax + 0x48], ecx
        je     loc_1_55AA
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x48
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        dec    dword ptr [eax + 0x54]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0x35
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0x13
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
        jmp    loc_1_5531
loc_1_55AA:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        add    ecx, 0x40
        cmp    dword ptr [eax + 0x40], ecx
        je     loc_1_5623
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x40
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        dec    dword ptr [eax + 0x50]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0x35
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0x13
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
        jmp    loc_1_55AA
loc_1_5623:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_5628:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x24e
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x14
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x38
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        dec    dword ptr [eax + 0xa4]
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb7], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x38
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        add    eax, 0x64
        push   eax
        call   sub_1_2011
        add    esp, 8
        call   sub_1_54C4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        cmp    dword ptr [eax + 0x78], 0
        je     loc_1_56CE
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x78]
        push   eax
        call   sub_1_0A8C
        add    esp, 4
loc_1_56CE:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0xa4], 0
        jne    loc_1_5753
        push   0x2580
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x2e]
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_570F
        mov    dword ptr [ebp - 4], 0x26c
loc_1_570F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0F6A
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x80], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x35]
        add    esp, 8
        jmp    loc_1_5753
loc_1_5753:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_5758:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_57B2
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_579B
        call   sub_1_4935
        jmp    loc_1_57AD
loc_1_579B:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_428C
loc_1_57AD:
        jmp    loc_1_58DA
loc_1_57B2:
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x45], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x46], 0x97
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x47], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa9]
        and    eax, 0xff
        shl    eax, 5
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        or     al, byte ptr [ecx + 0xaa]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x48], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x94]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 1
        jne    loc_1_584C
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x48]
        and    eax, 0xff
        or     al, 0x10
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x48], al
loc_1_584C:
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x49], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x44], 5
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xac]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x3d], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xab]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x4f], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_58B6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        jmp    loc_1_58DA
loc_1_58B6:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    dword ptr [ecx + 0x78], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [eax + 0x74], 0     ; -> loc_1_4339
loc_1_58DA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_58DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x12
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc8]
        and    eax, 0xff
        cmp    eax, 0xfe
        jne    loc_1_590F
        jmp    loc_1_5B80
loc_1_590F:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_5A35
loc_1_5927:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_594B
        mov    dword ptr [ebp - 4], 0xf
        jmp    loc_1_5952
loc_1_594B:
        mov    dword ptr [ebp - 4], 0x10
loc_1_5952:
        jmp    loc_1_5B5C
loc_1_5957:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0xa0], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaf]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_59B0
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x78]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x3d]
        add    esp, 0x10
loc_1_59B0:
        jmp    loc_1_5B5C
loc_1_59B5:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_59D9
        mov    dword ptr [ebp - 4], 0x17
        jmp    loc_1_59E0
loc_1_59D9:
        mov    dword ptr [ebp - 4], 0x18
loc_1_59E0:
        jmp    loc_1_5B5C
loc_1_59E5:
        mov    dword ptr [ebp - 4], 0xf
        jmp    loc_1_5B5C
loc_1_59F1:
        mov    dword ptr [ebp - 4], 0x18
        jmp    loc_1_5B5C
loc_1_59FD:
        mov    dword ptr [ebp - 4], 0xf
loc_1_5A04:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_5A2B
        push   1
        call   sub_1_5758
        add    esp, 4
        jmp    loc_1_5B80
loc_1_5A2B:
        jmp    loc_1_5B5C
        db  0E9h, 027h, 001h, 000h, 000h
loc_1_5A35:
        sub    dword ptr [ebp - 8], 3
        cmp    dword ptr [ebp - 8], 0xe0
        ja     loc_1_5A04
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]            ; -> obj1:0x5A7B
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x5A5B
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_5957
        dd  loc_1_59F1
        dd  loc_1_5927
        dd  loc_1_59E5
        dd  loc_1_59FD
        dd  loc_1_59F1
        dd  loc_1_59B5
        dd  loc_1_5A04
        db  000h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 001h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  002h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  003h, 007h, 007h, 007h, 004h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 005h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h, 007h
        db  006h
loc_1_5B5C:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_5B80
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_458B
        add    esp, 4
loc_1_5B80:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_5B85:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xff
        shr    eax, 1
        and    al, 7
        mov    byte ptr [ebp - 8], al
        mov    dword ptr [ebp - 4], 0x12
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0xa0], 0
        call   sub_1_49C7
        test   eax, eax
        je     loc_1_5BF0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_5BEB
        call   sub_1_4A69
loc_1_5BEB:
        jmp    loc_1_5BFF
loc_1_5BF0:
        push   8
        call   sub_1_5758
        add    esp, 4
        jmp    loc_1_5CD4
loc_1_5BFF:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaf]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_5CB0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa9]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        jne    loc_1_5CA9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa9]
        and    eax, 0xff
        inc    eax
        and    al, 7
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    ecx, dword ptr [ecx + 0x80]
        mov    byte ptr [ecx + 0xa9], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb1], 1
        push   1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x78]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x3d]
        add    esp, 0x10
        jmp    loc_1_5CB0
loc_1_5CA9:
        mov    dword ptr [ebp - 4], 0x15
loc_1_5CB0:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_5CD4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_458B
        add    esp, 4
loc_1_5CD4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_5CD9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x12
        call   sub_1_49C7
        test   eax, eax
        je     loc_1_5DD6
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xca]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_5DD1
        call   sub_1_4A69
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc9]
        and    eax, 0xf
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0xd
        jne    loc_1_5D3E
        mov    dword ptr [ebp - 4], 0x14
        jmp    loc_1_5DC5
loc_1_5D3E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 5
        jne    loc_1_5D75
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xae], 1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb3], 1
        jmp    loc_1_5DC5
loc_1_5D75:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 1
        jne    loc_1_5DC5
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xae]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_5DC5
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xae], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xd6], 1
loc_1_5DC5:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_458B
        add    esp, 4
loc_1_5DD1:
        jmp    loc_1_5DE0
loc_1_5DD6:
        push   8
        call   sub_1_5758
        add    esp, 4
loc_1_5DE0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_5DE5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    ax, word ptr [eax + 0x9c]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_5E41
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x2a]
        cmp    eax, 1
        jne    loc_1_5E41
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        inc    word ptr [eax + 0x9c]
        push   0x12c
        push   0xd
        call   sub_1_12A4
        add    esp, 8
        jmp    loc_1_5E9D
loc_1_5E41:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    word ptr [eax + 0x9c], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    byte ptr [eax + 0xb3], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    al, byte ptr [eax + 0xc3]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_5E8E
        push   0x13
        call   sub_1_458B
        add    esp, 4
        jmp    loc_1_5E9D
loc_1_5E8E:
        call   sub_1_4609
        call   sub_1_4782
        call   sub_1_44A0
loc_1_5E9D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_5EA2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_5ECC
        mov    eax, 0xe
        jmp    loc_1_5FC3
loc_1_5ECC:
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 1
        jne    loc_1_5EFC
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaf]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_5EFC
        mov    byte ptr [ebp + 8], 5
loc_1_5EFC:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        and    eax, 0xff
        or     al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x45], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        or     al, 0x10
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x46], al
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x3e], al
        mov    al, byte ptr [ebp + 8]
        and    eax, 3
        cmp    al, 1
        jne    loc_1_5F64
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa9]
        and    eax, 0xff
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        or     al, byte ptr [ecx + 0x46]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x46], al
loc_1_5F64:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xac]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x3d], al
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_5FA1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xab]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x4f], al
        jmp    loc_1_5FA8
loc_1_5FA1:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x4f], 7
loc_1_5FA8:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x44], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        jmp    loc_1_5FC3
loc_1_5FC3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_5FC8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_5FF2
        mov    eax, 0xe
        jmp    loc_1_6085
loc_1_5FF2:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa6]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x45], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x46], 0x73
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x3e], 1
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x84]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x47], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x58]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x4b], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xac]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x3d], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xab]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x4f], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x44], 0xa
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0xe]
        add    esp, 4
        jmp    loc_1_6085
loc_1_6085:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h
sub_1_608C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x2a
        cmp    dword ptr [0], 0              ; -> loc_1_DFEC
        je     loc_1_60DF
        mov    eax, dword ptr [0]            ; -> loc_1_DFF0
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x3a]
        add    esp, 4
        mov    dword ptr [0], eax            ; -> loc_1_DFE8
        cmp    dword ptr [0], 0              ; -> loc_1_DFE8
        je     loc_1_60D8
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_60DF
loc_1_60D8:
        mov    dword ptr [ebp - 4], 0x2b
loc_1_60DF:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_60E7
loc_1_60E7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_60EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E098
        jne    loc_1_61CF
        mov    eax, dword ptr [0]            ; -> loc_1_DFF8
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*2]
        shl    eax, 3
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        add    eax, 0x523
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFF8
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        add    eax, 0xd37
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFF8
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, 0x5cc
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFF8
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        add    eax, 0xab5
        mov    dword ptr [ebp - 0xc], eax
        push   1
        mov    eax, dword ptr [ebp - 0xc]
        add    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 0x10]
        add    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_DD74
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_619D
        jmp    loc_1_61CF
loc_1_619D:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [0], eax            ; -> obj1:0xDE08
        mov    eax, dword ptr [ebp - 0x14]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [0], eax            ; -> loc_1_E098
        mov    eax, dword ptr [ebp - 0x10]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [0], eax            ; -> loc_1_E0A0
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [0], eax            ; -> obj1:0xE0F8
loc_1_61CF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_61D4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_DFEC
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> loc_1_DFF0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0], eax            ; -> loc_1_DFF4
        call   sub_1_60EC
        call   sub_1_6594
        jmp    loc_1_6201
loc_1_6201:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6206:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0xDE08
        je     loc_1_623D
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        push   eax
        call   sub_1_DD7C
        add    esp, 8
        mov    dword ptr [0], 0              ; -> obj1:0xDE08
        mov    dword ptr [0], 0              ; -> loc_1_E098
loc_1_623D:
        mov    eax, 1
        jmp    loc_1_6247
loc_1_6247:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_624C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xcaa]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        jmp    loc_1_626E
loc_1_626E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_6273:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_DFE8
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        jmp    loc_1_628D
loc_1_628D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_6292:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        dec    eax
        or     eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [0]            ; -> loc_1_DFE8
        mov    dword ptr [ecx + 0x18], eax
        xor    eax, eax
        jmp    loc_1_62AF
loc_1_62AF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_62B4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        cmp    dword ptr [eax + 0x80], 0
        jne    loc_1_62D9
        mov    eax, 1
        jmp    loc_1_6438
loc_1_62D9:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x6c]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [eax + 0x6c], 0     ; -> loc_1_6526
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x26]
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [0]            ; -> obj1:0xDE08
        mov    dword ptr [ecx + 0x6c], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_643D
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x48]
        mov    dword ptr [ebp - 8], eax
loc_1_6328:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x48
        cmp    eax, dword ptr [ebp - 8]
        je     loc_1_63A4
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0x35
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x40], ecx
        jne    loc_1_6399
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        dec    dword ptr [eax + 0x54]
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
loc_1_6399:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_6328
loc_1_63A4:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    eax, dword ptr [eax + 0x40]
        mov    dword ptr [ebp - 8], eax
loc_1_63B5:
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        add    eax, 0x40
        cmp    eax, dword ptr [ebp - 8]
        je     loc_1_6431
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0x35
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x40], ecx
        jne    loc_1_6426
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        dec    dword ptr [eax + 0x50]
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
loc_1_6426:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_63B5
loc_1_6431:
        xor    eax, eax
        jmp    loc_1_6438
loc_1_6438:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_643D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xa8]
        mov    byte ptr [ebp - 0x10], al
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        mov    al, byte ptr [eax + 0xaa]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0x10]
        sub    eax, ecx
        and    al, 7
        mov    byte ptr [ebp - 4], al
        jmp    loc_1_6492
loc_1_648F:
        dec    byte ptr [ebp - 0x10]
loc_1_6492:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        test   eax, eax
        jle    loc_1_6521
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ecx + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x40], ecx
        jne    loc_1_6511
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_64DB
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x3f], 3
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x44], 2
loc_1_64DB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ecx + eax*4]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   9
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [edx + ecx*4], eax
loc_1_6511:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        inc    eax
        and    al, 7
        mov    byte ptr [ebp - 4], al
        jmp    loc_1_648F
loc_1_6521:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_6526:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0xb
        jne    loc_1_658C
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        mov    al, byte ptr [eax + 0x46]
        and    eax, 0xef
        cmp    al, 3
        jne    loc_1_658C
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        mov    al, byte ptr [eax + 0x3f]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_658C
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x7c]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x80]
        push   eax
        push   0
        mov    eax, dword ptr [0]            ; -> obj1:0xDE08
        mov    eax, dword ptr [eax + 0x70]
        call   dword ptr [eax + 0x39]
        add    esp, 0xc
loc_1_658C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
sub_1_6594:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_67A2
        call   sub_1_608C
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_65CB
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_67AA
loc_1_65CB:
        cmp    dword ptr [0], 0              ; -> loc_1_E098
        jne    loc_1_65E2
        mov    eax, 0xe
        jmp    loc_1_67AA
loc_1_65E2:
        mov    eax, dword ptr [0]            ; -> loc_1_DFF8
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        add    eax, 0xd37
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        push   eax
        call   sub_1_1044
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> loc_1_DFF8
        push   eax
        call   sub_1_0044
        add    esp, 4
        call   sub_1_84D0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xc3d
        push   eax
        call   sub_1_0231
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x18
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x10
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x14]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 8
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [0]            ; -> loc_1_DFF8
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    word ptr [ecx + 0xccf], ax
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_66D4
loc_1_66D1:
        inc    dword ptr [ebp - 8]
loc_1_66D4:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ax, word ptr [eax + 0xccf]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_6744
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        add    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcd1
        push   eax
        call   sub_1_C9A4
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        add    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xce1
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 8
        push   eax
        call   sub_1_1F70
        add    esp, 8
        jmp    loc_1_66D1
loc_1_6744:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xcb3], 0    ; -> loc_1_D67F
        call   sub_1_02BD
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_6790
        mov    dword ptr [0], 1              ; -> loc_1_E09C
        mov    eax, dword ptr [0]            ; -> loc_1_DFF8
        push   eax
        call   sub_1_8EC4
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> loc_1_DFF8
        push   eax
        call   sub_1_A235
        add    esp, 4
        jmp    loc_1_6795
loc_1_6790:
        call   sub_1_028C
loc_1_6795:
        push   0
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x3e]
        add    esp, 4
loc_1_67A2:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_67AA
loc_1_67AA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_67AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        je     loc_1_67FE
        mov    dword ptr [0], 0              ; -> loc_1_E09C
        push   0
        push   0
        push   0x2d
        call   sub_1_8E68
        add    esp, 0xc
        call   sub_1_039A
        mov    dword ptr [ebp - 4], eax
        push   3
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x3e]
        add    esp, 4
        call   sub_1_028C
loc_1_67FE:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_6806
loc_1_6806:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_680B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_6831
        mov    eax, 0x2d
        jmp    loc_1_6850
loc_1_6831:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x29
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x2d], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x2d]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x29], eax
        xor    eax, eax
        jmp    loc_1_6850
loc_1_6850:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6855:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_6881
        mov    eax, 0x2d
        jmp    loc_1_68BF
loc_1_6881:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x29
        push   eax
        call   sub_1_208E
        add    esp, 4
        test   eax, eax
        jne    loc_1_68A2
        mov    eax, 0x21
        jmp    loc_1_68BF
loc_1_68A2:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2c
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x29
        push   eax
        call   sub_1_1F70
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_68BF
loc_1_68BF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_68C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_68F0
        mov    eax, 0x2d
        jmp    loc_1_6944
loc_1_68F0:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x29
        push   eax
        call   sub_1_208E
        add    esp, 4
        test   eax, eax
        jne    loc_1_6911
        mov    eax, 0x21
        jmp    loc_1_6944
loc_1_6911:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_692E
        mov    eax, 0x20
        jmp    loc_1_6944
loc_1_692E:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x2c
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_6944
loc_1_6944:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6949:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_6976
        mov    eax, 0x2d
        jmp    loc_1_6B4C
loc_1_6976:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_69B4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    loc_1_69B4
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x29
        push   eax
        call   sub_1_208E
        add    esp, 4
        test   eax, eax
        jne    loc_1_69BE
loc_1_69B4:
        mov    eax, 0x21
        jmp    loc_1_6B4C
loc_1_69BE:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_69D5
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax + 0x1c], 0
        jne    loc_1_69DF
loc_1_69D5:
        mov    eax, 0x23
        jmp    loc_1_6B4C
loc_1_69DF:
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_69F5
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_6ADC
loc_1_69F5:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_7A9C
        add    esp, 4
        test   eax, eax
        jne    loc_1_6AD5
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0xffff
        cmp    eax, 0xffff
        jne    loc_1_6A81
        mov    dword ptr [ebp - 8], 3
        jmp    loc_1_6A2B
loc_1_6A28:
        inc    dword ptr [ebp - 8]
loc_1_6A2B:
        cmp    dword ptr [ebp - 8], 0x70
        jge    loc_1_6A53
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [ecx + eax*4 + 0x34], 0
        jne    loc_1_6A4E
        jmp    loc_1_6A53
loc_1_6A4E:
        jmp    loc_1_6A28
loc_1_6A53:
        cmp    dword ptr [ebp - 8], 0x6f
        jg     loc_1_6A7C
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [edx + ecx*4 + 0x34], eax
        mov    dword ptr [ebp - 0xc], 0
loc_1_6A7C:
        jmp    loc_1_6AD0
loc_1_6A81:
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0xffff
        cmp    eax, 0x6f
        jg     loc_1_6AD0
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [ecx + eax*4 + 0x34], 0
        jne    loc_1_6AD0
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0xffff
        mov    edx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [edx + ecx*4 + 0x34], eax
        mov    dword ptr [ebp - 0xc], 0
loc_1_6AD0:
        jmp    loc_1_6ADC
loc_1_6AD5:
        mov    dword ptr [ebp - 0xc], 0x23
loc_1_6ADC:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_1_6B44
        mov    eax, dword ptr [ebp + 0x18]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x25], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1d], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x21], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x11], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x11]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xd], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x15
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x19], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x19]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x15], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        push   eax
        call   sub_1_1F70
        add    esp, 8
loc_1_6B44:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_6B4C
loc_1_6B4C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_6B51:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_6B77
        mov    eax, 0x2d
        jmp    loc_1_6C3A
loc_1_6B77:
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_6C2B
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        je     loc_1_6C2B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 0x15
        cmp    dword ptr [eax + 0x15], ecx
        jne    loc_1_6C1F
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xd], 0
        je     loc_1_6BDB
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0xd
        push   eax
        call   sub_1_1FD6
        add    esp, 4
loc_1_6BDB:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x21], 0
        je     loc_1_6C13
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        cmp    eax, 0x6f
        jg     loc_1_6C13
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + eax*4 + 0x34], 0
loc_1_6C13:
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_6C26
loc_1_6C1F:
        mov    dword ptr [ebp - 8], 0x22
loc_1_6C26:
        jmp    loc_1_6C32
loc_1_6C2B:
        mov    dword ptr [ebp - 8], 0x21
loc_1_6C32:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_6C3A
loc_1_6C3A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_6C3F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_6C65
        mov    eax, 0x2d
        jmp    loc_1_6CD4
loc_1_6C65:
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_6CBD
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        je     loc_1_6C99
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xd], 0
        je     loc_1_6CA3
loc_1_6C99:
        mov    eax, 0x21
        jmp    loc_1_6CD4
loc_1_6CA3:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x1d]
        cmp    dword ptr [eax + 4], 0
        jne    loc_1_6CBD
        mov    eax, 0x23
        jmp    loc_1_6CD4
loc_1_6CBD:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   -1
        call   sub_1_D781
        add    esp, 0xc
        jmp    loc_1_6CD4
loc_1_6CD4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6CD9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_6D05
        mov    eax, 0x2d
        jmp    loc_1_6E95
loc_1_6D05:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_6D3F
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        cmp    eax, 0x6f
        jg     loc_1_6D3F
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    edx, dword ptr [ebp - 0xc]
        cmp    dword ptr [ecx + eax*4 + 0x34], edx
        je     loc_1_6D4B
loc_1_6D3F:
        mov    dword ptr [ebp - 0x14], 0x21
        jmp    loc_1_6E8D
loc_1_6D4B:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x21]
        cmp    dword ptr [eax + 4], 0
        jne    loc_1_6D67
        mov    dword ptr [ebp - 0x14], 0x23
        jmp    loc_1_6E8D
loc_1_6D67:
        push   0x3c
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_6D87
        jmp    loc_1_6E8D
loc_1_6D87:
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C1CE
        add    esp, 8
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_6DA9
        jmp    loc_1_6E8D
loc_1_6DA9:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C450
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x14], eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C64E
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7CE6
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_6E4A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    dword ptr [ebp - 0x14], 2
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0xa
        je     loc_1_6E45
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0x28
        je     loc_1_6E45
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CC32
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C3A8
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
loc_1_6E45:
        jmp    loc_1_6E77
loc_1_6E4A:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CC6F
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0x14
        jne    loc_1_6E77
        call   sub_1_D0E9
        test   eax, eax
        je     loc_1_6E77
        mov    dword ptr [ebp - 0x14], 2
loc_1_6E77:
        cmp    dword ptr [ebp - 0x14], 2
        jne    loc_1_6E8D
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C5FF
        add    esp, 4
loc_1_6E8D:
        mov    eax, dword ptr [ebp - 0x14]
        jmp    loc_1_6E95
loc_1_6E95:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_6E9A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_6EC0
        mov    eax, 0x2d
        jmp    loc_1_6F3B
loc_1_6EC0:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C26E
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_6EE1
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_6F3B
loc_1_6EE1:
        push   0x3c
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_6F04
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_6F3B
loc_1_6F04:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x14], eax
        push   0x80
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C64E
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C552
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C402
        add    esp, 4
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
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0xc], 0
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_6F74
        mov    eax, 0x2d
        jmp    loc_1_7072
loc_1_6F74:
        push   0x3c
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_6F94
        jmp    loc_1_706A
loc_1_6F94:
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_C305
        add    esp, 4
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_1_6FB2
        jmp    loc_1_706A
loc_1_6FB2:
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xa]
        mov    byte ptr [ebp - 0x10], al
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        cmp    eax, 1
        je     loc_1_702E
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x3f], 1
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x40], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0xc]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 9]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_8281
        add    esp, 0x14
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 0x14], 2
        jne    loc_1_7029
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0x18]
        inc    dword ptr [eax + 0x34]
loc_1_7029:
        jmp    loc_1_7041
loc_1_702E:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_C84F
        add    esp, 8
        mov    dword ptr [ebp - 0x14], eax
loc_1_7041:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        cmp    eax, 2
        jne    loc_1_7056
        mov    dword ptr [ebp - 4], 1
loc_1_7056:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_D14E
        add    esp, 0xc
loc_1_706A:
        mov    eax, dword ptr [ebp - 0x14]
        jmp    loc_1_7072
loc_1_7072:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7077:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_709D
        mov    eax, 0x2d
        jmp    loc_1_70D0
loc_1_709D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C144
        add    esp, 4
        test   eax, eax
        je     loc_1_70BB
        mov    eax, 0x20
        jmp    loc_1_70D0
loc_1_70BB:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C795
        add    esp, 8
        jmp    loc_1_70D0
loc_1_70D0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_70D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0x1d
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_70FC
        mov    eax, 0x2d
        jmp    loc_1_7161
loc_1_70FC:
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_7159
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7CE6
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_7159
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x40]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0xa
        je     loc_1_7159
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_7159
        mov    esi, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0xc]
        add    esi, 0x20
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_1_7159:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_7161
loc_1_7161:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_7166:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_7186
        mov    eax, 0x2d
        jmp    loc_1_7200
loc_1_7186:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc3c]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_71C3
        call   sub_1_04C9
        test   eax, eax
        je     loc_1_71B7
        mov    dword ptr [ebp - 4], 0x17
        jmp    loc_1_71BE
loc_1_71B7:
        mov    dword ptr [ebp - 4], 0x18
loc_1_71BE:
        jmp    loc_1_71EF
loc_1_71C3:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc3c]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_1_71E8
        mov    dword ptr [ebp - 4], 0x1f
        jmp    loc_1_71EF
loc_1_71E8:
        mov    dword ptr [ebp - 4], 0x1e
loc_1_71EF:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_7200
loc_1_7200:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_7205:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_7222
        mov    eax, 0x2d
        jmp    loc_1_723B
loc_1_7222:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_1_0FF9
        xor    eax, eax
        jmp    loc_1_723B
loc_1_723B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7240:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_725D
        mov    eax, 0x2d
        jmp    loc_1_7274
loc_1_725D:
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C6BA
        add    esp, 0xc
        jmp    loc_1_7274
loc_1_7274:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_7279:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_7296
        mov    eax, 0x2d
        jmp    loc_1_72AD
loc_1_7296:
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C6BA
        add    esp, 0xc
        jmp    loc_1_72AD
loc_1_72AD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_72B2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_72D8
        mov    eax, 0x2d
        jmp    loc_1_7390
loc_1_72D8:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_72FF
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    loc_1_7309
loc_1_72FF:
        mov    eax, 0x21
        jmp    loc_1_7390
loc_1_7309:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x1d]
        cmp    dword ptr [eax + 0x14], 0
        jne    loc_1_7323
        mov    eax, 0x23
        jmp    loc_1_7390
loc_1_7323:
        push   0x190
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_7349
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_7390
loc_1_7349:
        push   2
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x3e]
        add    esp, 4
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [eax + 0x3f], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x40], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [eax + 0x47], 0x70
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [eax + 0x48], 0x70
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [eax + 0x44], 2
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_03CC
        add    esp, 8
        jmp    loc_1_7390
loc_1_7390:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_7395:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_73AF
        xor    eax, eax
        jmp    loc_1_73BC
loc_1_73AF:
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x36]
        jmp    loc_1_73BC
loc_1_73BC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_73C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_73E5
        xor    eax, eax
        jmp    loc_1_742C
loc_1_73E5:
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_7424
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7CE6
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_7424
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x18], 0
        je     loc_1_7424
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x18]
        add    eax, 0x20
        mov    dword ptr [ebp - 8], eax
loc_1_7424:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_742C
loc_1_742C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_7431:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_744B
        xor    eax, eax
        jmp    loc_1_745A
loc_1_744B:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        jmp    loc_1_745A
loc_1_745A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_745F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_747F
        mov    eax, 0x2d
        jmp    loc_1_74D7
loc_1_747F:
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 0x15
        jl     loc_1_7492
        mov    eax, 0x15
loc_1_7492:
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + 0xc8f], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    edx, dword ptr [ebp + 0xc]
        add    ecx, 0xc90
        call   sub_1_0FF9
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + 0xc8e], al
        call   sub_1_83FE
        xor    eax, eax
        jmp    loc_1_74D7
loc_1_74D7:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_74DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_7503
        mov    eax, 0x2d
        jmp    loc_1_756B
loc_1_7503:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xc41
        mov    dword ptr [ebp - 4], eax
loc_1_7510:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx], al
        cmp    dword ptr [ebp - 8], 0x16
        je     loc_1_7542
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        test   al, 0x80
        jne    loc_1_7547
loc_1_7542:
        jmp    loc_1_754F
loc_1_7547:
        inc    dword ptr [ebp - 8]
        jmp    loc_1_7510
loc_1_754F:
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + 0xca5], al
        call   sub_1_83FE
        xor    eax, eax
        jmp    loc_1_756B
loc_1_756B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_7570:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_758D
        mov    eax, 0x2d
        jmp    loc_1_75A2
loc_1_758D:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_04E4
        add    esp, 8
        jmp    loc_1_75A2
loc_1_75A2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_75A7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_75C4
        mov    eax, 0x2d
        jmp    loc_1_762D
loc_1_75C4:
        cmp    dword ptr [ebp + 8], 0
        jbe    loc_1_75F7
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0xbb8
        ja     loc_1_75E1
        mov    eax, 0xbb8
loc_1_75E1:
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 0xcc7], eax
        call   sub_1_D6DB
        jmp    loc_1_7626
loc_1_75F7:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xcc7], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcab
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   2
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x3e]
        add    esp, 4
loc_1_7626:
        xor    eax, eax
        jmp    loc_1_762D
loc_1_762D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_7632:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_764F
        mov    eax, 0x2d
        jmp    loc_1_7666
loc_1_764F:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0xcc7]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        jmp    loc_1_7666
loc_1_7666:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_766B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_768C
        jmp    loc_1_76C5
loc_1_768C:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_76C5
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_76C5
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        cmp    dword ptr [eax + 0x18], 0
        je     loc_1_76C5
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x10]
        mov    ecx, dword ptr [ecx + 0x18]
        mov    byte ptr [ecx + 0xaf], al
loc_1_76C5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_76CA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_76E7
        mov    eax, 0x2d
        jmp    loc_1_772C
loc_1_76E7:
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_7711
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_7711
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_771B
loc_1_7711:
        mov    eax, 0x20
        jmp    loc_1_772C
loc_1_771B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_62B4
        add    esp, 4
        jmp    loc_1_772C
loc_1_772C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
sub_1_7734:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    byte ptr [ebp - 0x14], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x1f4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_78D1
        mov    dword ptr [ebp - 0xc], 0
loc_1_776F:
        mov    eax, dword ptr [ebp - 0xc]
        inc    eax
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_777E
loc_1_777B:
        inc    dword ptr [ebp - 0x10]
loc_1_777E:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0x10]
        jbe    loc_1_77D7
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0xc]
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edx + ebx + 8]
        cmp    dword ptr [eax + ecx + 8], edx
        jne    loc_1_77D2
        inc    byte ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx + 5], 0xff
loc_1_77D2:
        jmp    loc_1_777B
loc_1_77D7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x14]
        test   eax, eax
        jle    loc_1_785E
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx + 8]
        mov    ecx, dword ptr [ebp - 4]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + edx + 0xbec], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx + 5], 0xff
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x14]
        cmp    eax, 2
        jle    loc_1_7843
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + ecx + 0xbf0], 0x10
        jmp    loc_1_7857
loc_1_7843:
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + ecx + 0xbf0], 8
loc_1_7857:
        inc    dword ptr [ebp - 4]
        mov    byte ptr [ebp - 0x14], 0
loc_1_785E:
        mov    eax, dword ptr [ebp - 0xc]
        inc    eax
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_786D
loc_1_786A:
        inc    dword ptr [ebp - 0x10]
loc_1_786D:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0x10]
        jbe    loc_1_78AD
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx + 5]
        and    eax, 0xff
        cmp    eax, 0xff
        je     loc_1_78A8
        jmp    loc_1_78AD
loc_1_78A8:
        jmp    loc_1_786A
loc_1_78AD:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0x10]
        jne    loc_1_78C6
        jmp    loc_1_78D1
loc_1_78C6:
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_776F
loc_1_78D1:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 0x2c], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_78E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x1f4
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_7907
loc_1_7904:
        inc    dword ptr [ebp - 8]
loc_1_7907:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_79E1
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 5]
        and    eax, 0xff
        cmp    eax, 0xff
        jne    loc_1_79DC
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        dec    eax
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_7954
loc_1_7951:
        dec    dword ptr [ebp - 0xc]
loc_1_7954:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_798C
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 5]
        and    eax, 0xff
        cmp    eax, 0xff
        je     loc_1_7987
        jmp    loc_1_798C
loc_1_7987:
        jmp    loc_1_7951
loc_1_798C:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        jne    loc_1_799D
        jmp    loc_1_79E1
loc_1_799D:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp - 4]
        lea    edi, [edx + ebx + 4]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 5], 0xff
loc_1_79DC:
        jmp    loc_1_7904
loc_1_79E1:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_79EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x1f4
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_7A14
loc_1_7A11:
        inc    dword ptr [ebp - 8]
loc_1_7A14:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_7A6A
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 4]
        cmp    dword ptr [eax + ecx + 8], edx
        jne    loc_1_7A65
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 5], 0xff
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax + 2]
        jmp    loc_1_7A97
loc_1_7A65:
        jmp    loc_1_7A11
loc_1_7A6A:
        cmp    dword ptr [ebp - 8], 0x14
        jae    loc_1_7A97
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        lea    edi, [eax + ecx + 4]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [eax]
loc_1_7A97:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7A9C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_7AC9
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_7AC9
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x18], 0
        jne    loc_1_7AD3
loc_1_7AC9:
        mov    eax, 0x23
        jmp    loc_1_7AFE
loc_1_7AD3:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], 0
        jne    loc_1_7AF7
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8], 0
        jne    loc_1_7AF7
        mov    eax, 0x23
        jmp    loc_1_7AFE
loc_1_7AF7:
        xor    eax, eax
        jmp    loc_1_7AFE
loc_1_7AFE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7B03:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_7B26
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp - 0x10], eax
loc_1_7B26:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
loc_1_7B30:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [0], eax            ; -> loc_1_E098
        je     loc_1_7B81
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 5
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x1d]
        cmp    dword ptr [eax + 0xc], 0
        je     loc_1_7B76
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x1d]
        call   dword ptr [eax + 0xc]
        add    esp, 0xc
loc_1_7B76:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_7B30
loc_1_7B81:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7B86:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x10], eax
loc_1_7BA6:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x10
        cmp    eax, dword ptr [ebp - 0x10]
        je     loc_1_7C17
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, 0x1c
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_7C0C
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_1_7BF0
        mov    eax, dword ptr [ebp - 0x14]
        cmp    dword ptr [eax + 0x14], 0
        jne    loc_1_7C0C
loc_1_7BF0:
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_1_1F70
        add    esp, 8
loc_1_7C0C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_7BA6
loc_1_7C17:
        lea    eax, [ebp - 0xc]
        cmp    dword ptr [ebp - 0xc], eax
        je     loc_1_7C5C
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, 0x1c
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0xc]
        add    esp, 0xc
        jmp    loc_1_7C17
loc_1_7C5C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7C61:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0x20], 0
        jne    loc_1_7CE1
loc_1_7C79:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        add    ecx, 0x10
        cmp    dword ptr [eax + 0x10], ecx
        je     loc_1_7CE1
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0x1c
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_CC6F
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 2
        je     loc_1_7CD7
        push   1
        push   3
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7B86
        add    esp, 0xc
        jmp    loc_1_7CDC
loc_1_7CD7:
        jmp    loc_1_7CE1
loc_1_7CDC:
        jmp    loc_1_7C79
loc_1_7CE1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7CE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_7CFE
loc_1_7CFB:
        inc    dword ptr [ebp - 8]
loc_1_7CFE:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ax, word ptr [eax + 0xccf]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_7D8C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [ecx + eax*2 + 0xd35]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_7D87
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [ecx + eax*2 + 0xced]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_7D87
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        lea    eax, [ecx + eax*2 + 0xcd1]
        jmp    loc_1_7D93
loc_1_7D87:
        jmp    loc_1_7CFB
loc_1_7D8C:
        xor    eax, eax
        jmp    loc_1_7D93
loc_1_7D93:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7D98:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_7DB0
loc_1_7DAD:
        inc    dword ptr [ebp - 8]
loc_1_7DB0:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ax, word ptr [eax + 0xccf]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_7E54
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [ecx + eax*2 + 0xd35]
        and    eax, 0xff
        cmp    eax, 0x14
        jne    loc_1_7E4F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    edx, dword ptr [ebp - 8]
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*2]
        mov    ebx, dword ptr [0]            ; -> loc_1_E098
        mov    edx, dword ptr [ebx + edx*2 + 0xd09]
        cmp    dword ptr [ecx + eax*2 + 0xd0d], edx
        jne    loc_1_7E4F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        lea    eax, [ecx + eax*2 + 0xcd1]
        jmp    loc_1_7E5B
loc_1_7E4F:
        jmp    loc_1_7DAD
loc_1_7E54:
        xor    eax, eax
        jmp    loc_1_7E5B
loc_1_7E5B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CE07
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_7F09
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_7F04
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C552
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        cmp    eax, 6
        jb     loc_1_7EB9
        mov    eax, 6
loc_1_7EB9:
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0xc]
        add    dword ptr [ecx + 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0xc]
        neg    eax
        mov    ecx, dword ptr [ebp + 0xc]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x10]
        add    eax, 0x20
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 4]
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
loc_1_7F04:
        jmp    loc_1_7F2D
loc_1_7F09:
        push   0
        push   6
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8281
        add    esp, 0x14
loc_1_7F2D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_7F32:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CE07
        add    esp, 8
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_7FA2
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_1_7F78
        mov    dword ptr [ebp - 8], 9
        jmp    loc_1_7F9D
loc_1_7F78:
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_7F98
        mov    dword ptr [ebp - 8], 0x101
        jmp    loc_1_7F9D
loc_1_7F98:
        jmp    loc_1_80C5
loc_1_7F9D:
        jmp    loc_1_80AD
loc_1_7FA2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0x7f
        mov    byte ptr [ebp - 0x10], al
        mov    dword ptr [ebp - 0xc], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        cmp    eax, 0x6f
        jg     loc_1_7FD8
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [ecx + eax*4 + 0x34]
        mov    dword ptr [ebp - 0xc], eax
loc_1_7FD8:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_80A6
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x21]
        cmp    dword ptr [eax + 8], 0
        je     loc_1_809A
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_CFC4
        add    esp, 4
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_809A
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_C450
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_C5A4
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        cmp    eax, 6
        jb     loc_1_804A
        mov    eax, 6
loc_1_804A:
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp + 0xc]
        add    dword ptr [ecx + 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0x14]
        neg    eax
        mov    ecx, dword ptr [ebp + 0xc]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0x10]
        add    eax, 0x20
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 8]
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_80C5
loc_1_809A:
        mov    dword ptr [ebp - 8], 1
        jmp    loc_1_80AD
loc_1_80A6:
        mov    dword ptr [ebp - 8], 8
loc_1_80AD:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_C888
        add    esp, 0x10
loc_1_80C5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_80CA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CE07
        add    esp, 8
        mov    dword ptr [ebp - 0x18], eax
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_81CA
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        cmp    eax, 3
        je     loc_1_811F
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_81CA
loc_1_811F:
        mov    eax, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0xa]
        mov    byte ptr [ebp - 0x14], al
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        mov    dword ptr [ebp - 0xc], 6
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], 6
        jae    loc_1_8153
        dec    dword ptr [ebp - 0xc]
        mov    byte ptr [ebp - 0x10], 0xff
        jmp    loc_1_815F
loc_1_8153:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 0x10], al
loc_1_815F:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 0xc]
        add    dword ptr [ecx + 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0xc]
        neg    eax
        mov    ecx, dword ptr [ebp + 0xc]
        sub    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0xc]
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x14]
        cmp    eax, 1
        jne    loc_1_81B8
        mov    dword ptr [ebp - 4], 0
loc_1_81B8:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D14E
        add    esp, 0xc
loc_1_81CA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_81CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], 7
        jae    loc_1_81E9
        jmp    loc_1_827C
loc_1_81E9:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 6]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_827C
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_821C
        jmp    loc_1_827C
loc_1_821C:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x3f], 3
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0xff
        or     al, 0x80
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x47], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0x7f
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x48], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x49], 0x83
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x4a], 0xff
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x4b], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x44], 5
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        call   sub_1_37A8
        add    esp, 0xc
loc_1_827C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_8281:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0x18], 0
        jne    loc_1_82B9
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp + 0x18], eax
        cmp    dword ptr [ebp + 0x18], 0
        jne    loc_1_82B2
        mov    eax, 0xe
        jmp    loc_1_8303
loc_1_82B2:
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x3f], 3
loc_1_82B9:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        or     al, 0x80
        mov    ecx, dword ptr [ebp + 0x18]
        mov    byte ptr [ecx + 0x47], al
        mov    al, byte ptr [ebp + 0x10]
        and    eax, 0x7f
        mov    ecx, dword ptr [ebp + 0x18]
        mov    byte ptr [ecx + 0x48], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x49], 2
        mov    al, byte ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    byte ptr [ecx + 0x4a], al
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [eax + 0x44], 4
        push   1
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        call   sub_1_37A8
        add    esp, 0xc
        jmp    loc_1_8303
loc_1_8303:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_8308:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_831E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_8349
        mov    eax, 3
        jmp    loc_1_83F9
loc_1_8349:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_83C5
loc_1_835C:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 7]
        and    eax, 0xffff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_83DE
loc_1_8370:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_837F
loc_1_837C:
        inc    dword ptr [ebp - 0xc]
loc_1_837F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0xc]
        jbe    loc_1_83AF
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 9]
        and    eax, 0xffff
        add    dword ptr [ebp - 4], eax
        jmp    loc_1_837C
loc_1_83AF:
        jmp    loc_1_83DE
loc_1_83B4:
        mov    dword ptr [ebp - 8], 3
        jmp    loc_1_83DE
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_83C5:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_835C
        cmp    dword ptr [ebp - 0x10], 1
        je     loc_1_8370
        jmp    loc_1_83B4
loc_1_83DE:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_1_83F1
        mov    dword ptr [ebp - 8], 0xc
loc_1_83F1:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_83F9
loc_1_83F9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_83FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xc41
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xca5]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0x16
        jge    loc_1_84BA
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc8e]
        and    eax, 0xff
        test   eax, eax
        jle    loc_1_84BA
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc8f]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + edx], al
        inc    byte ptr [ebp - 4]
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc8e]
        and    eax, 0xff
        mov    ecx, 0x17
        xor    edx, edx
        mov    dl, byte ptr [ebp - 4]
        sub    ecx, edx
        cmp    eax, ecx
        jl     loc_1_8487
        mov    eax, ecx
loc_1_8487:
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [0]            ; -> loc_1_E098
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        add    eax, dword ptr [ebp - 0xc]
        add    edx, 0xc90
        mov    ecx, eax
        call   sub_1_0FF9
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        add    eax, ecx
        mov    byte ptr [ebp - 4], al
loc_1_84BA:
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + 0xc61], al
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h
sub_1_84D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc62], 0    ; -> loc_1_8580
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc66], 0    ; -> loc_1_86E1
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc6e], 0    ; -> loc_1_875F
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc6a], 0    ; -> loc_1_87A8
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc72], 0    ; -> loc_1_8845
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc76], 0    ; -> loc_1_89FA
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc7a], 0    ; -> loc_1_8AA9
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc82], 0    ; -> loc_1_8D11
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc7e], 0    ; -> sub_1_8D3C
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc86], 0    ; -> loc_1_8DFD
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xc8a], 0    ; -> sub_1_8E68
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_8580:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc3c]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_8629
        call   sub_1_7734
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0x2c], 0
        jne    loc_1_85C0
        call   sub_1_DB14
        jmp    loc_1_8624
loc_1_85C0:
        call   sub_1_78E2
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x2c]
        lea    eax, [eax + eax*4 - 5]
        add    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xbec
        mov    dword ptr [ebp - 4], eax
        push   4
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x52c
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1624
        add    esp, 0x10
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 2
        jne    loc_1_861F
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xc3c], 2
        jmp    loc_1_8624
loc_1_861F:
        call   sub_1_DB14
loc_1_8624:
        jmp    loc_1_86DC
loc_1_8629:
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_1_8638
loc_1_8635:
        inc    dword ptr [ebp - 0x10]
loc_1_8638:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0x10]
        jbe    loc_1_866B
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 4
        push   eax
        call   sub_1_79EF
        add    esp, 4
        jmp    loc_1_8635
loc_1_866B:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        dec    dword ptr [eax + 0x2c]
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0x2c], 0
        jbe    loc_1_86C8
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x2c]
        lea    eax, [eax + eax*4 - 5]
        add    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xbec
        mov    dword ptr [ebp - 4], eax
        push   4
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x52c
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1624
        add    esp, 0x10
        mov    dword ptr [ebp - 0xc], eax
loc_1_86C8:
        cmp    dword ptr [ebp - 0xc], 2
        je     loc_1_86DC
        call   sub_1_78E2
        call   sub_1_DB14
loc_1_86DC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_86E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        mov    dword ptr [ebp - 0xc], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D3A1
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
loc_1_870F:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [0], eax            ; -> loc_1_E098
        je     loc_1_875A
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, 5
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x1d]
        cmp    dword ptr [eax + 8], 0
        je     loc_1_874F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x1d]
        call   dword ptr [eax + 8]
        add    esp, 4
loc_1_874F:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_870F
loc_1_875A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_875F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x7c]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0xa
        jne    loc_1_87A3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CADF
        add    esp, 8
        push   0x1a
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7B03
        add    esp, 8
loc_1_87A3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_87A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        call   sub_1_C9EF
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_8834
        push   2
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x3e]
        add    esp, 4
        push   0
        call   sub_1_227C
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CADF
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x7c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x58]
        mov    dword ptr [ebp - 0x30], eax
        mov    byte ptr [ebp - 0x34], 1
        mov    byte ptr [ebp - 0x33], 0
        mov    byte ptr [ebp - 0x31], 0
        mov    byte ptr [ebp - 0xc], 0
        lea    eax, [ebp - 0x34]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_DC25
        add    esp, 4
        push   0x1a
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7B03
        add    esp, 8
        jmp    loc_1_8840
loc_1_8834:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_387C
        add    esp, 4
loc_1_8840:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_8845:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x7c]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0xc], 3
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_886D
        jmp    loc_1_89F5
loc_1_886D:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x64]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_88D6
loc_1_8883:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        dec    dword ptr [eax + 0x24]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    byte ptr [eax + 2], 0
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_1_892C
loc_1_88A1:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        dec    dword ptr [eax + 0x28]
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    byte ptr [eax + 2], 0
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_1_892C
loc_1_88BF:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        dec    dword ptr [eax + 0x20]
        jmp    loc_1_892C
loc_1_88CC:
        jmp    loc_1_89F5
        db  0E9h, 056h, 000h, 000h, 000h
loc_1_88D6:
        sub    dword ptr [ebp - 0x10], 0xa
        cmp    dword ptr [ebp - 0x10], 0x1e
        ja     loc_1_89F5
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]            ; -> obj1:0x890D
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x88F9
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_88BF
        dd  loc_1_8883
        dd  loc_1_88A1
        dd  loc_1_88A1
        dd  loc_1_88CC
        db  000h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 001h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 002h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 003h
loc_1_892C:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CEC3
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        call   sub_1_CEC3
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CA6F
        add    esp, 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        sub    eax, 0xa
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_7B86
        add    esp, 0xc
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0xa
        je     loc_1_8995
        push   0x1d
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7B03
        add    esp, 8
loc_1_8995:
        cmp    dword ptr [ebp + 8], 0x1b
        jne    loc_1_89B2
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    word ptr [eax + 0x864], 0
        call   sub_1_DC87
loc_1_89B2:
        call   sub_1_7C61
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0x20], 0
        jne    loc_1_89F5
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_1_89F5
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0x28], 0
        jne    loc_1_89F5
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xc3c], 0
        call   sub_1_D6DB
loc_1_89F5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_89FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + 0x3f]
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_8A8B
loc_1_8A16:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x40]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x40]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0x18]
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x40]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C7E5
        add    esp, 8
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x7c]
        push   eax
        call   sub_1_D211
        add    esp, 8
        jmp    loc_1_8AA4
loc_1_8A62:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x7c]
        push   eax
        call   sub_1_D211
        add    esp, 8
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_0A8C
        add    esp, 4
        jmp    loc_1_8AA4
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_8A8B:
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_8A16
        cmp    dword ptr [ebp - 4], 3
        je     loc_1_8A62
        jmp    loc_1_8AA4
loc_1_8AA4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_8AA9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x7c]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax], 4
        jae    loc_1_8ACC
        jmp    loc_1_8D0C
loc_1_8ACC:
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_8C30
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax], 5
        jb     loc_1_8AF8
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_1_8AFD
loc_1_8AF8:
        jmp    loc_1_8D0C
loc_1_8AFD:
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_8B71
loc_1_8B13:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7F32
        add    esp, 8
        jmp    loc_1_8C2B
loc_1_8B28:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7E60
        add    esp, 8
        jmp    loc_1_8C2B
loc_1_8B3D:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_80CA
        add    esp, 8
        jmp    loc_1_8C2B
loc_1_8B52:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_81CF
        add    esp, 8
        jmp    loc_1_8C2B
loc_1_8B67:
        jmp    loc_1_8C2B
        db  0E9h, 0BAh, 000h, 000h, 000h
loc_1_8B71:
        dec    dword ptr [ebp - 0x14]
        cmp    dword ptr [ebp - 0x14], 0x80
        ja     loc_1_8C2B
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax]            ; -> obj1:0x8BAA
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0x8B96
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_8B13
        dd  loc_1_8B3D
        dd  loc_1_8B52
        dd  loc_1_8B28
        dd  loc_1_8B67
        db  000h, 001h, 002h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  003h
loc_1_8C2B:
        jmp    loc_1_8CFC
loc_1_8C30:
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        cmp    eax, 0x6f
        jg     loc_1_8CFC
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CE07
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_8CD8
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        add    dword ptr [eax + 4], 4
        mov    eax, dword ptr [ebp + 0x10]
        sub    dword ptr [eax], 4
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_1_8CA0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0x10]
        add    esp, 8
        jmp    loc_1_8CCA
loc_1_8CA0:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        cmp    dword ptr [eax + 0x14], 0
        je     loc_1_8CCA
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0x14]
        add    esp, 8
loc_1_8CCA:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_1_8D0C
loc_1_8CD8:
        push   0
        push   6
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_8281
        add    esp, 0x14
loc_1_8CFC:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8D3C
        add    esp, 8
loc_1_8D0C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_8D11:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x40]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x1d]
        call   dword ptr [eax + 0x14]
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_8D3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], 4
        jb     loc_1_8D7F
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        cmp    eax, 0x70
        jne    loc_1_8D7F
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0xff
        cmp    eax, 0x70
        je     loc_1_8D84
loc_1_8D7F:
        jmp    loc_1_8DF8
loc_1_8D84:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        add    dword ptr [eax + 4], 4
        mov    eax, dword ptr [ebp + 0xc]
        sub    dword ptr [eax], 4
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0x10], eax
loc_1_8DA4:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [0], eax            ; -> loc_1_E098
        je     loc_1_8DEF
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, 5
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x1d]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_8DE4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0x1d]
        call   dword ptr [eax + 0x10]
        add    esp, 4
loc_1_8DE4:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_8DA4
loc_1_8DEF:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 4], eax
loc_1_8DF8:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_8DFD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_8E47
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x7c]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_8E47
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_D060
        add    esp, 8
        test   eax, eax
        jne    loc_1_8E47
        jmp    loc_1_8E63
loc_1_8E47:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7B03
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D885
        add    esp, 4
loc_1_8E63:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_8E68:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
loc_1_8E7B:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [0], eax            ; -> loc_1_E098
        je     loc_1_8EBE
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 5
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x1d]
        call   dword ptr [eax + 0x1c]
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_8E7B
loc_1_8EBE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h
sub_1_8EC4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E0A0
        je     loc_1_90FD
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, 0x5cc
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        push   eax
        call   sub_1_1044
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    word ptr [ecx + 0x68], ax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x42], 0x1f4
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x4a], 0     ; -> loc_1_9D80
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x31
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [ecx + 0x35], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    eax, dword ptr [eax + 0x35]
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [ecx + 0x31], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x39
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [ecx + 0x3d], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    eax, dword ptr [eax + 0x3d]
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [ecx + 0x39], eax
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_8F7A
loc_1_8F77:
        inc    dword ptr [ebp - 4]
loc_1_8F7A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_90CD
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0xfb
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x39
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    byte ptr [eax + ecx + 0xac], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x1cc
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, ecx
        lea    ecx, [ecx + ecx*8]
        lea    ecx, [edx + ecx*2]
        lea    ecx, [edx + ecx*8]
        lea    ecx, [ecx + ecx*8]
        add    ecx, edx
        mov    edx, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [ecx + edx + 0xaf], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    word ptr [eax + ecx + 0xb3], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + ecx + 0x183], 0x7d0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + ecx + 0x18b], 0 ; -> loc_1_9DFA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + ecx + 0x19f], 0x4e20
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + ecx + 0x1a7], 0 ; -> loc_1_9E36
        jmp    loc_1_8F77
loc_1_90CD:
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        push   eax
        call   sub_1_680B
        add    esp, 4
        push   0                             ; -> loc_1_E0E4
        push   0                             ; -> loc_1_E0C8
        push   0                             ; -> loc_1_E0A8
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        push   eax
        push   0xffff
        call   sub_1_6949
        add    esp, 0x14
loc_1_90FD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_9102:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_9122
        mov    eax, 0x2d
        jmp    loc_1_923E
loc_1_9122:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B930
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_9143
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_923E
loc_1_9143:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x31
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9C83
        add    esp, 4
        mov    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        cmp    dword ptr [eax + 0x62], 0
        jne    loc_1_9236
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x20]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_95F0
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_9202
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x15e], ecx
        je     loc_1_9202
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_1_91FD
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0x15e], 0
        jne    loc_1_91FD
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x119
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7077
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_99F3
        add    esp, 4
loc_1_91FD:
        jmp    loc_1_9220
loc_1_9202:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_9219
        call   sub_1_96A8
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_9220
loc_1_9219:
        mov    dword ptr [ebp - 8], 0x25
loc_1_9220:
        cmp    dword ptr [ebp - 8], 2
        je     loc_1_9236
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
loc_1_9236:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_923E
loc_1_923E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_9243:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0xc], 0x18
        jne    loc_1_926E
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    al, byte ptr [eax + 0x41]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_926E
        call   sub_1_96A8
loc_1_926E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_9273:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_92EE
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        cmp    eax, 0x400
        jb     loc_1_92AE
        mov    eax, 0x400
loc_1_92AE:
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0xc7], ax
        mov    eax, dword ptr [ebp - 4]
        dec    word ptr [eax + 0xc7]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x90]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_92EE
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xc2], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_99F3
        add    esp, 4
loc_1_92EE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_92F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_9360
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x15e]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x31
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        je     loc_1_9353
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x15e]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        jmp    loc_1_9360
loc_1_9353:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x15e], 0
loc_1_9360:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_9384
        push   0x13
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_9F67
        add    esp, 8
loc_1_9384:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_9389:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_1_93D3
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xc3], 0
        je     loc_1_93D3
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 0xc3]
        add    esp, 8
loc_1_93D3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_93D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x40
        cmp    eax, dword ptr [ebp + 0x10]
        jne    loc_1_9449
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x90], 0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_9444
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc7]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_9444
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xc2], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_99F3
        add    esp, 4
loc_1_9444:
        jmp    loc_1_9457
loc_1_9449:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_9F67
        add    esp, 8
loc_1_9457:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_945C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x135
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], 0
        jbe    loc_1_94CA
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 0x40
        je     loc_1_94CA
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0x3f
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    cl, byte ptr [ecx + 0x2c]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_1_94CA
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B4B
        add    esp, 4
        jmp    loc_1_94D6
loc_1_94CA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_94D6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_94DB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x135
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], 0
        jbe    loc_1_95C7
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0x3f
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    cl, byte ptr [ecx + 0x2c]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_1_95C7
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 0x40
        jne    loc_1_95C0
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        call   dword ptr [eax + 0x14]
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_9596
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xc3], 0
        jne    loc_1_9591
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_99F3
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
loc_1_9591:
        jmp    loc_1_95BB
loc_1_9596:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xc3], 0
        je     loc_1_95BB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0
        call   sub_1_9E9F
        add    esp, 8
        mov    dword ptr [ebp - 4], 0
loc_1_95BB:
        jmp    loc_1_95C7
loc_1_95C0:
        mov    dword ptr [ebp - 4], 0
loc_1_95C7:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_95DD
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_95DD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_95E2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
sub_1_95F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_9608
loc_1_9605:
        inc    dword ptr [ebp - 4]
loc_1_9608:
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    ax, word ptr [eax + 0x68]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 4]
        jle    loc_1_969C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    al, byte ptr [eax + ecx + 0x12c]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_9697
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + ecx + 0x107], edx
        jne    loc_1_9697
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        lea    eax, [eax + ecx + 0x6a]
        jmp    loc_1_96A3
loc_1_9697:
        jmp    loc_1_9605
loc_1_969C:
        xor    eax, eax
        jmp    loc_1_96A3
loc_1_96A3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_96A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 2
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        add    ecx, 0x31
        cmp    dword ptr [eax + 0x31], ecx
        jne    loc_1_96D9
        mov    eax, 1
        jmp    loc_1_97FA
loc_1_96D9:
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        add    ecx, 0x39
        cmp    dword ptr [eax + 0x39], ecx
        je     loc_1_97ED
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    eax, dword ptr [eax + 0x39]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 0x91
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    eax, dword ptr [eax + 0x31]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 0x14]
        add    edi, 0x99
        mov    esi, dword ptr [eax + 0x20]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x99
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    word ptr [eax + 0x43], 0
        mov    eax, dword ptr [ebp - 0x14]
        mov    word ptr [eax + 0xc7], 0
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x40
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        push   eax
        call   sub_1_6CD9
        add    esp, 0x10
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 0x10], 2
        jne    loc_1_97CE
        mov    eax, dword ptr [ebp - 0x14]
        mov    byte ptr [eax + 0x90], 1
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x91
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x14]
        mov    byte ptr [eax + 0xc2], 1
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        inc    word ptr [eax + 0x66]
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    byte ptr [eax + 0x41], 2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 0x15e], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0xc], eax
        jmp    loc_1_97E8
loc_1_97CE:
        cmp    dword ptr [ebp - 0x10], 0x17
        jne    loc_1_97E8
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    byte ptr [eax + 0x41], 1
        mov    dword ptr [ebp - 0x10], 2
loc_1_97E8:
        jmp    loc_1_97F2
loc_1_97ED:
        call   sub_1_97FF
loc_1_97F2:
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_1_97FA
loc_1_97FA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_97FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_9817
loc_1_9814:
        inc    dword ptr [ebp - 4]
loc_1_9817:
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    ax, word ptr [eax + 0x68]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 4]
        jle    loc_1_9900
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    al, byte ptr [eax + ecx + 0x12c]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_1_98FB
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        cmp    dword ptr [eax + ecx + 0x1c8], 0
        jne    loc_1_98FB
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x183
        push   eax
        call   sub_1_15D7
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x6a
        push   eax
        call   sub_1_7077
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x6a
        push   eax
        call   sub_1_9F2B
        add    esp, 4
        jmp    loc_1_9900
loc_1_98FB:
        jmp    loc_1_9814
loc_1_9900:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_9905:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_991D
loc_1_991A:
        inc    dword ptr [ebp - 8]
loc_1_991D:
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    ax, word ptr [eax + 0x68]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jle    loc_1_99EE
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        mov    al, byte ptr [eax + ecx + 0x12c]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_99E9
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x6a
        push   eax
        call   sub_1_9F2B
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_99E9
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x6a
        push   eax
        call   sub_1_9F2B
        add    esp, 4
        cmp    eax, 2
        je     loc_1_99E9
        push   1
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        add    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x6a
        push   eax
        call   sub_1_9F67
        add    esp, 8
loc_1_99E9:
        jmp    loc_1_991A
loc_1_99EE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_99F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9ACA
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_9A7C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x15e], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x2c]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x162], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x155], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 0x10]
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B4B
        add    esp, 4
        mov    eax, 1
        jmp    loc_1_9AC5
loc_1_9A7C:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x15e], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x119
        push   eax
        call   sub_1_1526
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7077
        add    esp, 8
        call   sub_1_96A8
        cmp    eax, 2
        jne    loc_1_9ABD
        mov    dword ptr [ebp - 8], 1
loc_1_9ABD:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_9AC5
loc_1_9AC5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_9ACA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    eax, dword ptr [eax + 0x31]
        mov    dword ptr [ebp - 8], eax
loc_1_9ADE:
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x31
        cmp    eax, dword ptr [ebp - 8]
        je     loc_1_9B27
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x20]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x9d]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_9B1A
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_9B2E
        db  0E9h, 008h, 000h, 000h, 000h
loc_1_9B1A:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_9ADE
loc_1_9B27:
        xor    eax, eax
        jmp    loc_1_9B2E
loc_1_9B2E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_9B33:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xc3], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_9B4B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x155]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x162
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x4b], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x43], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x90]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_9C72
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xc7]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x151]
        jb     loc_1_9BEA
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x162]
        and    eax, 0xff
        or     al, 0x80
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x162], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xc3], 0     ; -> loc_1_94DB
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x151]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x4f], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 0
        jmp    loc_1_9C3F
loc_1_9BEA:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xc3], 0     ; -> loc_1_945C
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xc7]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x4f], ax
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0xc7]
        and    ecx, 0xffff
        sub    eax, ecx
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 0x151], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0xc7]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x155], eax
loc_1_9C3F:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x90], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x40
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7240
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x135
        push   eax
        call   sub_1_1526
        add    esp, 4
        jmp    loc_1_9C7E
loc_1_9C72:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_9C7E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_9C83:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        cmp    dword ptr [eax + 0x5e], 0
        jne    loc_1_9CB5
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x5e], 1
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x42
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_9CB5:
        call   sub_1_DDB0
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_9CC5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        call   sub_1_DDB0
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    eax, dword ptr [eax + 0x31]
        mov    dword ptr [ebp - 0x10], eax
loc_1_9CE1:
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x31
        cmp    eax, dword ptr [ebp - 0x10]
        je     loc_1_9D7B
        mov    eax, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0xc]
        sub    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    eax, dword ptr [ecx + 0x28]
        jb     loc_1_9D70
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ebp - 0xc]
        cmp    dword ptr [eax + 0x15e], ecx
        jne    loc_1_9D49
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [eax + 0x15e], 0
loc_1_9D49:
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x62], 1
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        push   0xf
        mov    eax, dword ptr [ebp - 0xc]
        call   dword ptr [eax + 0x1c]
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x62], 0
loc_1_9D70:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_9CE1
loc_1_9D7B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_9D80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    al, byte ptr [eax + 0x41]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_9DA1
        call   sub_1_96A8
loc_1_9DA1:
        call   sub_1_9CC5
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    al, byte ptr [eax + 0x41]
        and    eax, 0xff
        cmp    eax, 1
        je     loc_1_9DE9
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    ecx, dword ptr [0]            ; -> loc_1_E0A0
        add    ecx, 0x31
        cmp    dword ptr [eax + 0x31], ecx
        jne    loc_1_9DE9
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x5e], 0
        call   sub_1_9905
        jmp    loc_1_9DF5
loc_1_9DE9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_9DF5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_9DFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 0x119
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_1_9E31
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_9E31:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_9E36:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 0x135
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    al, byte ptr [eax + 0xb2]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_9E77
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1526
        add    esp, 4
        jmp    loc_1_9E9A
loc_1_9E77:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0xc2]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_1_9E9A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_9E9A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_9E9F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x90]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_9F1A
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x15e]
        mov    al, byte ptr [eax + 0x2c]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 8]
        or     eax, ecx
        or     al, 0x40
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x162], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x43], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x90], 1
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x40
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_7240
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x135
        push   eax
        call   sub_1_1526
        add    esp, 4
        jmp    loc_1_9F26
loc_1_9F1A:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_9F2B
        add    esp, 4
loc_1_9F26:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_9F2B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xc3], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc2], 3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0xc9
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F40
        add    esp, 8
        jmp    loc_1_9F62
loc_1_9F62:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_9F67:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x119
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x135
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xc2], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        dec    word ptr [eax + 0x66]
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x91
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x39
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x15e], 0
        je     loc_1_A011
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x15e], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x62], 1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 0x1c]
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x62], 0
loc_1_A011:
        call   sub_1_96A8
        cmp    eax, 2
        je     loc_1_A03E
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    ax, word ptr [eax + 0x66]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_A03E
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    byte ptr [eax + 0x41], 0
loc_1_A03E:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h
sub_1_A044:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_A064
        mov    eax, 0x2d
        jmp    loc_1_A0CB
loc_1_A064:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ACCF
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_A085
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_A0CB
loc_1_A085:
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    ax, word ptr [eax + 0x167]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_A0B8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 8
        push   eax
        call   sub_1_1F70
        add    esp, 8
        jmp    loc_1_A0C4
loc_1_A0B8:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AB18
        add    esp, 4
loc_1_A0C4:
        xor    eax, eax
        jmp    loc_1_A0CB
loc_1_A0CB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_A0D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_A0ED
        mov    eax, 0x2d
        jmp    loc_1_A1C0
loc_1_A0ED:
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_A101
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_A10B
loc_1_A101:
        mov    eax, 1
        jmp    loc_1_A1C0
loc_1_A10B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 8
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        je     loc_1_A145
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp + 0xc]
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_A1C0
loc_1_A145:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    loc_1_A169
        mov    eax, 1
        jmp    loc_1_A1C0
loc_1_A169:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    ax, word ptr [eax + 0x167]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_A1AD
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x10
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, 2
        jmp    loc_1_A1C0
loc_1_A1AD:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AB95
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_A1C0
loc_1_A1C0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_A1C5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> loc_1_E09C
        jne    loc_1_A1E2
        mov    eax, 0x2d
        jmp    loc_1_A230
loc_1_A1E2:
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cmp    eax, 0xff
        jl     loc_1_A1F7
        mov    eax, 0xff
loc_1_A1F7:
        mov    byte ptr [ebp + 8], al
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [ecx + 0x4b], al
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [ecx + 0x4c], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        push   eax
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    edx, dword ptr [ebp + 0xc]
        add    ecx, 0x51
        call   sub_1_0FF9
        xor    eax, eax
        jmp    loc_1_A230
loc_1_A230:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_A235:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0xE0F8
        je     loc_1_A480
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        add    eax, 0xab5
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        push   eax
        call   sub_1_1044
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 8
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    eax, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x10
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    eax, dword ptr [eax + 0x14]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    word ptr [ecx + 0x169], ax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [eax + 0x2f], 0     ; -> loc_1_E144
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [eax + 0x2e], 6
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [eax + 0x33], 0     ; -> loc_1_E13C
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    word ptr [eax + 0x37], 2
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [eax + 0x3a], 0     ; -> obj1:0xE14C
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [eax + 0x39], 0xc
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [eax + 0x3e], 2
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    word ptr [eax + 0x3f], 3
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [eax + 0x41], 0     ; -> loc_1_E140
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [eax + 0x46], 0     ; -> loc_1_E15C
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [eax + 0x45], 0xa
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [eax + 0x4a], 3
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [eax + 0x4b], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [eax + 0x4c], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x51
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [ecx + 0x4d], eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x18
        push   eax
        call   sub_1_A044
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x16b
        push   eax
        call   sub_1_680B
        add    esp, 4
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_A3BA
loc_1_A3B7:
        inc    dword ptr [ebp - 4]
loc_1_A3BA:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_A45C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [eax + ecx + 0x1e0], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x6ab
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, ecx
        lea    ecx, [ecx + ecx*8]
        lea    ecx, [edx + ecx*8]
        shl    ecx, 2
        sub    ecx, edx
        lea    ecx, [edx + ecx*8]
        mov    edx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [ecx + edx + 0x1e3], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x19c
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x16b
        push   eax
        call   sub_1_6855
        add    esp, 8
        jmp    loc_1_A3B7
loc_1_A45C:
        push   0                             ; -> loc_1_E168
        push   0                             ; -> loc_1_E120
        push   0                             ; -> loc_1_E100
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x16b
        push   eax
        push   0
        call   sub_1_6949
        add    esp, 0x14
loc_1_A480:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_A485:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_A8F7
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_A525
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x40], 2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xe2], 0     ; -> loc_1_A60F
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0xf9], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xe6], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 0x45], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax]
        cmp    eax, 0x40a
        jb     loc_1_A4FB
        mov    eax, 0x40a
loc_1_A4FB:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x10b], eax
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 0x10b]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x42
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6E9A
        add    esp, 8
        jmp    loc_1_A558
loc_1_A525:
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_A558
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x3f], 3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F40
        add    esp, 8
        jmp    loc_1_A558
loc_1_A558:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_A55D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_A9EC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_A57D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x40]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_1_A5C4
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0xe2], 0
        je     loc_1_A5C4
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 0xe2]
        add    esp, 8
loc_1_A5C4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_A5C9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x42
        cmp    eax, dword ptr [ebp + 0x10]
        jne    loc_1_A5F3
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x41], 0
        jmp    loc_1_A5FF
loc_1_A5F3:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_A9EC
        add    esp, 4
loc_1_A5FF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_A604:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_A60F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], 1
        jae    loc_1_A626
        jmp    loc_1_A7A0
loc_1_A626:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 0x40
        jne    loc_1_A79B
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xf9]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_A674
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7077
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AE94
        add    esp, 8
        jmp    loc_1_A69B
loc_1_A674:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0x3f
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 0xef]
        and    ecx, 0xff
        cmp    eax, ecx
        je     loc_1_A69B
        jmp    loc_1_A7A0
loc_1_A69B:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0xf1]
        add    esp, 8
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 0x80
        je     loc_1_A772
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xf9]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_A6F8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AA92
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABDF
        add    esp, 4
        jmp    loc_1_A76D
loc_1_A6F8:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xf9]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_A713
        jmp    loc_1_A7A0
loc_1_A713:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xfa], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0xf5]
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xfa]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_A761
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABBE
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xe2], 0     ; -> loc_1_A7B1
        jmp    loc_1_A76D
loc_1_A761:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABDF
        add    esp, 4
loc_1_A76D:
        jmp    loc_1_A79B
loc_1_A772:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xf9]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_A78D
        jmp    loc_1_A7A0
loc_1_A78D:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0
        call   sub_1_AA17
        add    esp, 8
loc_1_A79B:
        jmp    loc_1_A7AC
loc_1_A7A0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_A7AC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_A7B1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax], 0
        jbe    loc_1_A80F
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        test   al, 0x40
        je     loc_1_A80F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0xf5]
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xfa]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_A80A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABDF
        add    esp, 4
loc_1_A80A:
        jmp    loc_1_A81B
loc_1_A80F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_A81B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_A820:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   0x17
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x150
        push   eax
        call   sub_1_1044
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_A848:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [0], eax            ; -> obj1:0xE0F8
        je     loc_1_A8DF
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x1b], 0
        je     loc_1_A8D2
        mov    dword ptr [ebp - 0xc], 0
loc_1_A871:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x1b]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [0]            ; -> obj1:0xE0F8
        or     al, byte ptr [ecx + edx + 0x150]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    byte ptr [ecx + edx + 0x150], al
        cmp    dword ptr [ebp - 0xc], 0x16
        je     loc_1_A8C5
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0x1b]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        test   al, 0x80
        jne    loc_1_A8CA
loc_1_A8C5:
        jmp    loc_1_A8D2
loc_1_A8CA:
        inc    dword ptr [ebp - 0xc]
        jmp    loc_1_A871
loc_1_A8D2:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_A848
loc_1_A8DF:
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x150
        push   eax
        call   sub_1_74DC
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_A8F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_A90F
loc_1_A90C:
        inc    dword ptr [ebp - 4]
loc_1_A90F:
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    ax, word ptr [eax + 0x169]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 4]
        jle    loc_1_A9AC
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    al, byte ptr [eax + ecx + 0x1dc]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_A9A7
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        mov    eax, dword ptr [eax + ecx + 0x19c]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_A9A7
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        shl    eax, 2
        sub    eax, ecx
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        lea    eax, [eax + ecx + 0x19c]
        jmp    loc_1_A9B3
loc_1_A9A7:
        jmp    loc_1_A90C
loc_1_A9AC:
        xor    eax, eax
        jmp    loc_1_A9B3
loc_1_A9B3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_A9B8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x92
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F40
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xe2], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x40], 3
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_A9EC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x40], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xe2], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_ABDF
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_AA17:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0x41]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_AA81
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xef]
        and    eax, 0xff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 8]
        or     eax, ecx
        or     al, 0x40
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x50f], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x4b], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax + 0x45], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x41], 1
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x42
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_7240
        add    esp, 8
        jmp    loc_1_AA8D
loc_1_AA81:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_AA8D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_AA92:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x41]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_AB07
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xef]
        and    eax, 0xff
        or     al, 0x80
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x50f], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xf0]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x510], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x4b], 2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x45], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x41], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x42
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7240
        add    esp, 8
        jmp    loc_1_AB13
loc_1_AB07:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_AB13:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_AB18:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_AB32:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [0], eax            ; -> obj1:0xE0F8
        je     loc_1_AB71
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0x14]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0xc]
        jbe    loc_1_AB61
        jmp    loc_1_AB71
loc_1_AB61:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        inc    dword ptr [ebp - 0xc]
        jmp    loc_1_AB32
loc_1_AB71:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x14], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1F70
        add    esp, 8
        call   sub_1_A820
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_AB95:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        call   sub_1_A820
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 0x10]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_ABBE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xee], 1
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        inc    word ptr [eax + 0x167]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_ABDF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xee]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_ACA5
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xee], 0
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        dec    word ptr [eax + 0x167]
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    ax, word ptr [eax + 0x167]
        and    eax, 0xffff
        test   eax, eax
        jne    loc_1_ACA5
loc_1_AC2D:
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        add    ecx, 0x10
        cmp    dword ptr [eax + 0x10], ecx
        je     loc_1_AC69
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 0x10
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_AB95
        add    esp, 4
        jmp    loc_1_AC2D
loc_1_AC69:
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    ecx, dword ptr [0]            ; -> obj1:0xE0F8
        add    ecx, 8
        cmp    dword ptr [eax + 8], ecx
        je     loc_1_ACA5
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 8
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_AB18
        add    esp, 4
        jmp    loc_1_AC69
loc_1_ACA5:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xe2], 0     ; -> loc_1_A60F
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf9], 0
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7077
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_ACCF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 1
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        add    eax, 8
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x16]
        and    eax, 0xff
        test   eax, eax
        jle    loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x16]
        and    eax, 0xff
        cmp    eax, 0x3c
        jg     loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1f]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_AE84
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1f]
        and    eax, 0xffff
        cmp    eax, 0x100
        jg     loc_1_AE84
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_AD81
loc_1_AD7E:
        inc    dword ptr [ebp - 8]
loc_1_AD81:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1f]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_AE68
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + eax*4 + 0x21]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_ADCB
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + eax*4 + 0x21]
        and    eax, 0xff
        cmp    eax, 0x3c
        jle    loc_1_ADD0
loc_1_ADCB:
        jmp    loc_1_AE68
loc_1_ADD0:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + eax*4 + 0x26]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_AE31
loc_1_ADF1:
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*4 + 0x27]
        and    eax, 0xffff
        cmp    eax, 0x400
        jle    loc_1_AE16
        mov    dword ptr [ebp - 4], 0
loc_1_AE16:
        jmp    loc_1_AE54
        db  0E9h, 034h, 000h, 000h, 000h
loc_1_AE20:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_AE54
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_AE31:
        cmp    dword ptr [ebp - 0x10], 1
        je     loc_1_AE54
        cmp    dword ptr [ebp - 0x10], 2
        je     loc_1_ADF1
        cmp    dword ptr [ebp - 0x10], 3
        je     loc_1_AE54
        jmp    loc_1_AE20
loc_1_AE54:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_AE63
        jmp    loc_1_AE68
loc_1_AE63:
        jmp    loc_1_AD7E
loc_1_AE68:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1f]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jne    loc_1_AE84
        mov    dword ptr [ebp - 0xc], 0
loc_1_AE84:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_AE8C
loc_1_AE8C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
sub_1_AE94:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax]
        and    eax, 0x3f
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0xef], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xe6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf0], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xef]
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_AF3A
loc_1_AEDE:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xf1], 0     ; -> loc_1_AF64
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xf5], 0     ; -> loc_1_B117
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf9], 2
        jmp    loc_1_AF49
loc_1_AF07:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf0], 0xff
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf9], 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xf1], 0     ; -> loc_1_AF4E
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xf5], 0     ; -> loc_1_AF59
        jmp    loc_1_AF49
loc_1_AF3A:
        cmp    dword ptr [ebp - 4], 4
        je     loc_1_AEDE
        jmp    loc_1_AF07
loc_1_AF49:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_AF4E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_AF59:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_AF64:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
loc_1_AF74:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jbe    loc_1_B112
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xf9]
        and    eax, 0xff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_B0B6
loc_1_AF98:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xea], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xea], 0x3c
        ja     loc_1_AFD2
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xea], 0
        jne    loc_1_AFEB
loc_1_AFD2:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf9], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf0], 1
        jmp    loc_1_AFF5
loc_1_AFEB:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf9], 3
loc_1_AFF5:
        jmp    loc_1_B0E2
loc_1_AFFA:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0xea]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xea], 0
        jne    loc_1_B01D
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf9], 4
loc_1_B01D:
        jmp    loc_1_B0E2
loc_1_B022:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xea], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xea], 0x3c
        ja     loc_1_B05C
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xea], 0
        jne    loc_1_B075
loc_1_B05C:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf9], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf0], 2
        jmp    loc_1_B07F
loc_1_B075:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf9], 5
loc_1_B07F:
        jmp    loc_1_B0E2
loc_1_B084:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 0xea]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xea], 0
        jne    loc_1_B0A7
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf9], 0
loc_1_B0A7:
        jmp    loc_1_B0E2
loc_1_B0AC:
        jmp    loc_1_B112
        db  0E9h, 02Ch, 000h, 000h, 000h
loc_1_B0B6:
        cmp    dword ptr [ebp - 8], 5
        ja     loc_1_B0E2
        mov    eax, dword ptr [ebp - 8]
        jmp    dword ptr [eax*4]             ; -> obj1:0xB0CA
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_B0AC
        dd  loc_1_B0AC
        dd  loc_1_AF98
        dd  loc_1_AFFA
        dd  loc_1_B022
        dd  loc_1_B084
loc_1_B0E2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0xe6]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 0x10f], al
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0xe6]
        inc    dword ptr [ebp - 4]
        jmp    loc_1_AF74
loc_1_B112:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_B117:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xfa]
        and    eax, 0xff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_B1F0
loc_1_B136:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B218
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jbe    loc_1_B1B8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xef]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x50f], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfb], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf0], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x510], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x511], ah
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x512], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x107], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B27B
        add    esp, 4
        jmp    loc_1_B1C4
loc_1_B1B8:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AA92
        add    esp, 4
loc_1_B1C4:
        jmp    loc_1_B213
loc_1_B1C9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B27B
        add    esp, 4
        jmp    loc_1_B213
loc_1_B1DA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B397
        add    esp, 4
        jmp    loc_1_B213
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_B1F0:
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_B136
        cmp    dword ptr [ebp - 8], 1
        je     loc_1_B1C9
        cmp    dword ptr [ebp - 8], 2
        je     loc_1_B1DA
        jmp    loc_1_B213
loc_1_B213:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B218:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf0], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xfb], 0
loc_1_B23F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B5A8
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xf0]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_B266
        jmp    loc_1_B26E
loc_1_B266:
        inc    dword ptr [ebp - 4]
        jmp    loc_1_B23F
loc_1_B26E:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_B276
loc_1_B276:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B27B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x510
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x103], 1
loc_1_B29C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B5A8
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        add    dword ptr [ebp - 0xc], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xf0]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_B2D6
        jmp    loc_1_B386
loc_1_B2D6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x107]
        add    eax, dword ptr [ebp - 0xc]
        cmp    eax, 0x40a
        jbe    loc_1_B2FE
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xfb], eax
        jmp    loc_1_B386
loc_1_B2FE:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xff]
        lea    eax, [eax + eax*2]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xfb]
        add    eax, 0x21
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    ax, word ptr [eax + 0x14]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x107]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx], ah
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    al, byte ptr [eax + 0x14]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x107]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + edx + 1], al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x107]
        add    eax, dword ptr [ebp - 8]
        add    eax, 2
        push   eax
        call   sub_1_B413
        add    esp, 8
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x107], eax
        jmp    loc_1_B29C
loc_1_B386:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B397
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B397:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x10b]
        cmp    dword ptr [eax + 0x107], ecx
        jbe    loc_1_B3CB
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xfa], 2
        jmp    loc_1_B3FE
loc_1_B3CB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xf0]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_B3F4
        mov    byte ptr [ebp - 4], 0x80
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xfa], 0
        jmp    loc_1_B3FE
loc_1_B3F4:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xfa], 1
loc_1_B3FE:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B83F
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B413:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 5]
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_B4F5
loc_1_B43A:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 6]
        shr    eax, 0x18
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 6]
        shr    eax, 0x10
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 3], ah
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 4], al
        jmp    loc_1_B518
loc_1_B475:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], ah
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 8]
        add    ecx, 3
        call   sub_1_0FF9
        jmp    loc_1_B518
loc_1_B4B6:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 7]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 7]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 8]
        add    ecx, 3
        call   sub_1_0FF9
        jmp    loc_1_B518
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_B4F5:
        cmp    dword ptr [ebp - 4], 1
        je     loc_1_B43A
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_B475
        cmp    dword ptr [ebp - 4], 3
        je     loc_1_B4B6
        jmp    loc_1_B518
loc_1_B518:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B51D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0xff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_B577
loc_1_B539:
        mov    dword ptr [ebp - 4], 4
        jmp    loc_1_B59A
loc_1_B545:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0xffff
        add    eax, 2
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_B59A
loc_1_B55C:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 7]
        and    eax, 0xff
        add    eax, 2
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_B59A
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_B577:
        cmp    dword ptr [ebp - 8], 1
        je     loc_1_B539
        cmp    dword ptr [ebp - 8], 2
        je     loc_1_B545
        cmp    dword ptr [ebp - 8], 3
        je     loc_1_B55C
        jmp    loc_1_B59A
loc_1_B59A:
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        jmp    loc_1_B5A3
loc_1_B5A3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B5A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x10f]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x110
        mov    dword ptr [ebp - 0x18], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 0x110]
        mov    byte ptr [ebp - 0x10], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x111
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xfb], 0
        jne    loc_1_B607
        mov    eax, dword ptr [0]            ; -> obj1:0xE0F8
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_B613
loc_1_B607:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        mov    dword ptr [ebp - 0xc], eax
loc_1_B613:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_B6CA
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xfb], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xfb], 0
        jne    loc_1_B651
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf0], 1
        jmp    loc_1_B6BE
loc_1_B651:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xfb]
        push   eax
        call   sub_1_B73C
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xff], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0xff], -1
        jne    loc_1_B695
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xf0], 2
        jmp    loc_1_B6BE
loc_1_B695:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xff]
        lea    eax, [eax + eax*2]
        shl    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0xfb]
        add    eax, 0x21
        push   eax
        call   sub_1_B51D
        add    esp, 4
        jmp    loc_1_B6C5
loc_1_B6BE:
        xor    eax, eax
        jmp    loc_1_B6C5
loc_1_B6C5:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B6CA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 8], eax
loc_1_B6DB:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [0], eax            ; -> obj1:0xE0F8
        je     loc_1_B730
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x16]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x17]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_B7BE
        add    esp, 0x10
        test   eax, eax
        je     loc_1_B723
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_B737
loc_1_B723:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp + 8], eax
        jmp    loc_1_B6DB
loc_1_B730:
        xor    eax, eax
        jmp    loc_1_B737
loc_1_B737:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B73C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_B754
loc_1_B751:
        inc    dword ptr [ebp - 4]
loc_1_B754:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x1f]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 4]
        jbe    loc_1_B7AF
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + eax*4 + 0x21]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0x22]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_B7BE
        add    esp, 0x10
        test   eax, eax
        je     loc_1_B7AA
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_B7B9
loc_1_B7AA:
        jmp    loc_1_B751
loc_1_B7AF:
        mov    eax, 0xffffffff
        jmp    loc_1_B7B9
loc_1_B7B9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B7BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp + 0x14], eax
        jne    loc_1_B832
loc_1_B7E1:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 4], eax
        jae    loc_1_B81F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + edx]
        and    ecx, 0xff
        cmp    eax, ecx
        je     loc_1_B817
        jmp    loc_1_B81F
loc_1_B817:
        inc    dword ptr [ebp - 4]
        jmp    loc_1_B7E1
loc_1_B81F:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [ebp - 4], eax
        jne    loc_1_B832
        mov    dword ptr [ebp - 8], 1
loc_1_B832:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_B83A
loc_1_B83A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B83F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x4b], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x103]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0x50f
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x4d], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0x45], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x41]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_B91F
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x10b]
        cmp    dword ptr [eax + 0x107], ecx
        ja     loc_1_B8C8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x50f]
        and    eax, 0xff
        or     al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x50f], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x107]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x51], ax
        jmp    loc_1_B900
loc_1_B8C8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10b]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0x51], ax
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0x10b]
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 0x107], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10b]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x103], eax
loc_1_B900:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x41], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x42
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7240
        add    esp, 8
        jmp    loc_1_B92B
loc_1_B91F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B8
        add    esp, 4
loc_1_B92B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_B930:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x25
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_BA6F
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x20], 0
        je     loc_1_BA6F
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1c], 0
        je     loc_1_BA6F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        add    eax, 0x31
        push   eax
        call   sub_1_2042
        add    esp, 8
        test   eax, eax
        jne    loc_1_BA6F
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x28], 0x36ee80
        jbe    loc_1_B99B
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x28], 0x36ee80
loc_1_B99B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x2c]
        and    eax, 0xff
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_BA40
loc_1_B9AE:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2d], 0
        je     loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x35], 0
        je     loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2d]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        cmp    eax, 0x3c
        jg     loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x2d]
        mov    al, byte ptr [eax + 5]
        and    eax, 0xff
        cmp    eax, 0x3c
        jg     loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x33]
        and    eax, 0xffff
        cmp    eax, 0xb
        jb     loc_1_BA25
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x10], 0     ; -> loc_1_BA7C
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x14], 0     ; -> loc_1_BB3C
        mov    dword ptr [ebp - 4], 0
loc_1_BA25:
        jmp    loc_1_BA6F
loc_1_BA2A:
        mov    dword ptr [ebp - 4], 7
        jmp    loc_1_BA6F
        db  0E9h, 034h, 000h, 000h, 000h, 0E9h, 02Fh, 000h, 000h, 000h
loc_1_BA40:
        dec    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 5
        ja     loc_1_BA6F
        mov    eax, dword ptr [ebp - 8]
        jmp    dword ptr [eax*4]             ; -> obj1:0xBA57
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_BA2A
        dd  loc_1_BA2A
        dd  loc_1_BA2A
        dd  loc_1_B9AE
        dd  loc_1_BA2A
        dd  loc_1_BA2A
loc_1_BA6F:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_BA77
loc_1_BA77:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_BA7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x163
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    eax, dword ptr [eax + 0x2d]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    eax, dword ptr [eax + 0x2d]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 1]
        inc    ecx
        call   sub_1_0FF9
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        inc    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x151], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    eax, dword ptr [eax + 0x2d]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x151]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx], al
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x151]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    eax, dword ptr [eax + 0x2d]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x151]
        add    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 6]
        call   sub_1_0FF9
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0x151], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_BB3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    ax, word ptr [eax + 0x33]
        and    eax, 0xffff
        sub    eax, 9
        mov    dword ptr [ebp - 8], eax
loc_1_BB64:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax], ecx
        jbe    loc_1_BD4C
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x15d]
        and    eax, 0xff
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_BCF8
loc_1_BB88:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 1]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C0A9
        add    esp, 8
        test   eax, eax
        je     loc_1_BBC1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    eax, dword ptr [eax + 0x35]
        mov    word ptr [eax], 0
loc_1_BBC1:
        jmp    loc_1_BD44
loc_1_BBC6:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    ecx, dword ptr [ecx + 0x35]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 2
        jmp    loc_1_BD44
loc_1_BBF9:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    ecx, dword ptr [ecx + 0x35]
        mov    cx, word ptr [ecx + 2]
        and    ecx, 0xffff
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    ecx, dword ptr [ecx + 0x35]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 3
        jmp    loc_1_BD44
loc_1_BC41:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    ecx, dword ptr [ecx + 0x35]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 4
        jmp    loc_1_BD44
loc_1_BC74:
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    ecx, dword ptr [ecx + 0x35]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0xffff
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    ecx, dword ptr [ecx + 0x35]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 5
        jmp    loc_1_BD44
loc_1_BCBC:
        jmp    loc_1_BD44
loc_1_BCC1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    eax, dword ptr [eax + 0x35]
        add    eax, 6
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_BD51
        add    esp, 0x10
        jmp    loc_1_BD44
        db  0E9h, 04Ch, 000h, 000h, 000h
loc_1_BCF8:
        cmp    dword ptr [ebp - 0xc], 0x10
        ja     loc_1_BCC1
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]            ; -> obj1:0xBD33
        and    eax, 0xff
        jmp    dword ptr [eax*4]             ; -> obj1:0xBD17
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_BB88
        dd  loc_1_BBC6
        dd  loc_1_BBF9
        dd  loc_1_BC41
        dd  loc_1_BC74
        dd  loc_1_BCBC
        dd  loc_1_BCC1
        db  000h, 001h, 002h, 003h, 004h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  005h
loc_1_BD44:
        inc    dword ptr [ebp - 4]
        jmp    loc_1_BB64
loc_1_BD4C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_BD51:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x15d]
        and    eax, 0xff
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_BFE3
loc_1_BD70:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    byte ptr [eax + 0x32], 0
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    byte ptr [ecx], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_BDD5
loc_1_BD92:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C02C
        add    esp, 4
        jmp    loc_1_BDF9
loc_1_BDA3:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 6
        jmp    loc_1_BDF9
loc_1_BDB2:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 0xa
        jmp    loc_1_BDF9
loc_1_BDC1:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 0xd
        jmp    loc_1_BDF9
        db  0E9h, 024h, 000h, 000h, 000h
loc_1_BDD5:
        cmp    dword ptr [ebp - 8], 3
        ja     loc_1_BDF9
        mov    eax, dword ptr [ebp - 8]
        jmp    dword ptr [eax*4]             ; -> obj1:0xBDE9
        ; ---- dispatch table (4 entries) ----
        dd  loc_1_BD92
        dd  loc_1_BDA3
        dd  loc_1_BDB2
        dd  loc_1_BDC1
loc_1_BDF9:
        jmp    loc_1_C027
loc_1_BDFE:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        shl    eax, 0x18
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 1], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 7
        jmp    loc_1_C027
loc_1_BE1B:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        shl    eax, 0x10
        mov    ecx, dword ptr [ebp + 0x14]
        or     dword ptr [ecx + 1], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 8
        jmp    loc_1_C027
loc_1_BE38:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 0x14]
        or     dword ptr [ecx + 1], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 9
        jmp    loc_1_C027
loc_1_BE55:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        or     dword ptr [ecx + 1], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C02C
        add    esp, 4
        jmp    loc_1_C027
loc_1_BE71:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 0x14]
        mov    word ptr [ecx + 1], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 0xb
        jmp    loc_1_C027
loc_1_BE8F:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ax, word ptr [eax + 1]
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0xc]
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 0x14]
        mov    word ptr [ecx + 1], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x159], 0
        jmp    loc_1_C027
loc_1_BEC5:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax + 0x159], ecx
        jae    loc_1_BEEF
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x159]
        mov    edx, dword ptr [ebp + 0x14]
        mov    byte ptr [ecx + edx + 3], al
        jmp    loc_1_BEFC
loc_1_BEEF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    byte ptr [eax + 0x32], 1
loc_1_BEFC:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x159]
        mov    eax, dword ptr [ebp + 0x14]
        mov    ax, word ptr [eax + 1]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x159]
        jne    loc_1_BF2C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C02C
        add    esp, 4
loc_1_BF2C:
        jmp    loc_1_C027
loc_1_BF31:
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 0xe
        jmp    loc_1_C027
loc_1_BF49:
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 0xf
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x159], 0
        jmp    loc_1_C027
loc_1_BF6E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax + 0x159], ecx
        jae    loc_1_BF98
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x159]
        mov    edx, dword ptr [ebp + 0x14]
        mov    byte ptr [ecx + edx + 3], al
        jmp    loc_1_BFA5
loc_1_BF98:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    byte ptr [eax + 0x32], 1
loc_1_BFA5:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x159]
        mov    eax, dword ptr [ebp + 0x14]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x159]
        jne    loc_1_BFD4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C02C
        add    esp, 4
loc_1_BFD4:
        jmp    loc_1_C027
        db  0E9h, 049h, 000h, 000h, 000h, 0E9h, 044h, 000h, 000h, 000h
loc_1_BFE3:
        sub    dword ptr [ebp - 4], 5
        cmp    dword ptr [ebp - 4], 0xa
        ja     loc_1_C027
        mov    eax, dword ptr [ebp - 4]
        jmp    dword ptr [eax*4]             ; -> obj1:0xBFFB
        ; ---- dispatch table (11 entries) ----
        dd  loc_1_BD70
        dd  loc_1_BDFE
        dd  loc_1_BE1B
        dd  loc_1_BE38
        dd  loc_1_BE55
        dd  loc_1_BE71
        dd  loc_1_BE8F
        dd  loc_1_BEC5
        dd  loc_1_BF31
        dd  loc_1_BF49
        dd  loc_1_BF6E
loc_1_C027:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C02C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    eax, dword ptr [eax + 0x35]
        inc    word ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        mov    eax, dword ptr [eax + 0x35]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    ecx, dword ptr [ecx + 0x35]
        mov    cx, word ptr [ecx]
        and    ecx, 0xffff
        cmp    eax, ecx
        jne    loc_1_C08E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B33
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 0x10
        jmp    loc_1_C098
loc_1_C08E:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 5
loc_1_C098:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C103
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C0A9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x15e]
        mov    byte ptr [ecx + 0x31], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        test   eax, eax
        je     loc_1_C0F4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B33
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C103
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x15d], 0x10
        xor    eax, eax
        jmp    loc_1_C0FE
loc_1_C0F4:
        mov    eax, 1
        jmp    loc_1_C0FE
loc_1_C0FE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C103:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x62], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x15e]
        call   dword ptr [eax + 0x1c]
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> loc_1_E0A0
        mov    dword ptr [eax + 0x62], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h
sub_1_C144:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_C1B3
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_1_C1B3
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcd1
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x10]
        ja     loc_1_C1B3
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ax, word ptr [eax + 0xccf]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        add    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcd1
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x10]
        ja     loc_1_C1C2
loc_1_C1B3:
        mov    eax, 0x20
        jmp    loc_1_C1C9
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_C1C2:
        xor    eax, eax
        jmp    loc_1_C1C9
loc_1_C1C9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C1CE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_C25A
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x15]
        mov    dword ptr [ebp - 8], eax
loc_1_C1EA:
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0x15
        cmp    eax, dword ptr [ebp - 8]
        je     loc_1_C24E
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0x24
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 9]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx + 9]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_1_C241
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    ecx, dword ptr [ecx + 4]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_C241
        mov    eax, 0x24
        jmp    loc_1_C269
loc_1_C241:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_C1EA
loc_1_C24E:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_C261
loc_1_C25A:
        mov    dword ptr [ebp - 0xc], 0x20
loc_1_C261:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_C269
loc_1_C269:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C26E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_C2F6
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        cmp    eax, 2
        jne    loc_1_C2F6
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_C2F6
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcd1
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x10]
        ja     loc_1_C2F6
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ax, word ptr [eax + 0xccf]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        add    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcd1
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x10]
        jbe    loc_1_C2F6
        xor    eax, eax
        jmp    loc_1_C300
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_C2F6:
        mov    eax, 0x20
        jmp    loc_1_C300
loc_1_C300:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C305:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_C399
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_C399
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_C38D
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_C399
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcd1
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x10]
        ja     loc_1_C399
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ax, word ptr [eax + 0xccf]
        and    eax, 0xffff
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        add    eax, eax
        add    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcd1
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x10]
        jbe    loc_1_C399
loc_1_C38D:
        xor    eax, eax
        jmp    loc_1_C3A3
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_C399:
        mov    eax, 0x20
        jmp    loc_1_C3A3
loc_1_C3A3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C3A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        call   sub_1_37A8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    loc_1_C3DF
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x34]
loc_1_C3DF:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8308
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_C3FD
loc_1_C3FD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C402:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        call   sub_1_37A8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    loc_1_C439
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x34]
loc_1_C439:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_C44B
loc_1_C44B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C450:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x18], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xa], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x34], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xb], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x20]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x28], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x28]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x24], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C4CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1c
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        call   sub_1_1FD6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D022
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xa], 0
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_C54D
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0x13
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0x18]
        add    esp, 0xc
loc_1_C54D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C552:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1c], 0
        je     loc_1_C574
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1c
        push   eax
        call   sub_1_1FD6
        add    esp, 4
loc_1_C574:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1c
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xa], 3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C5A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xa], 2
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        add    eax, 0x15
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF3E
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C5FF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xa], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1c
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x10
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x24
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        add    eax, 0x15
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF87
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C64E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x14]
        mov    byte ptr [eax + 0x3f], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x14]
        mov    dword ptr [ecx + 0x40], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 9]
        and    eax, 0xff
        or     al, 0x80
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x14]
        mov    byte ptr [ecx + 0x47], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x14]
        mov    byte ptr [ecx + 0x48], al
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        or     al, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x14]
        mov    byte ptr [ecx + 0x49], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x14]
        mov    byte ptr [eax + 0x4a], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x14]
        mov    byte ptr [eax + 0x44], 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C6BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C144
        add    esp, 4
        test   eax, eax
        je     loc_1_C6E1
        mov    eax, 0x20
        jmp    loc_1_C790
loc_1_C6E1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x20]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_831E
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_C70C
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_C790
loc_1_C70C:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_C72A
        mov    eax, 0x29
        jmp    loc_1_C790
loc_1_C72A:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x3f], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx + 0x40], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 9]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x47], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [ecx + 0x48], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0x44], 2
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        call   sub_1_37A8
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    loc_1_C788
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0x34]
loc_1_C788:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_C790
loc_1_C790:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C795:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb]
        and    eax, 0xff
        cmp    eax, dword ptr [ebp + 0xc]
        je     loc_1_C7D9
        mov    al, byte ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0xb], al
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_1_C7D0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        inc    dword ptr [eax + 0x38]
        jmp    loc_1_C7D9
loc_1_C7D0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        dec    dword ptr [eax + 0x38]
loc_1_C7D9:
        xor    eax, eax
        jmp    loc_1_C7E0
loc_1_C7E0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C7E5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [eax + 0x34]
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        cmp    eax, 4
        jne    loc_1_C84A
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x34], 0
        jne    loc_1_C84A
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 0xa], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0x18]
        add    esp, 0xc
loc_1_C84A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C84F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x34], 0
        jbe    loc_1_C87C
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x14], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0xa], 4
        mov    eax, 2
        jmp    loc_1_C883
loc_1_C87C:
        xor    eax, eax
        jmp    loc_1_C883
loc_1_C883:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C888:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 1
        mov    dword ptr [ebp - 0x10], 1
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 0x14], eax
        jmp    loc_1_C8D5
loc_1_C8B8:
        mov    dword ptr [ebp - 0x10], 0
        jmp    loc_1_C8FB
loc_1_C8C4:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_C8FB
        db  0E9h, 026h, 000h, 000h, 000h
loc_1_C8D5:
        cmp    dword ptr [ebp - 0x14], 1
        je     loc_1_C8B8
        cmp    dword ptr [ebp - 0x14], 8
        je     loc_1_C8B8
        cmp    dword ptr [ebp - 0x14], 0x101
        je     loc_1_C8C4
        jmp    loc_1_C8FB
loc_1_C8FB:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_C94B
        push   0
        call   sub_1_0991
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_C923
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x3f], 3
loc_1_C923:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_8281
        add    esp, 0x14
loc_1_C94B:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_C98B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0xc]
        add    esp, 0xc
        cmp    dword ptr [ebp + 0x10], 9
        jne    loc_1_C98B
        mov    dword ptr [ebp - 4], 1
loc_1_C98B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_D14E
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C9A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x48], 0x7d0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x50], 0     ; -> loc_1_D272
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_C9EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        add    ecx, 8
        cmp    dword ptr [eax + 8], ecx
        je     loc_1_CA62
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 8
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0x10
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x3c], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x38], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x44], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x65], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], 0
loc_1_CA62:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_CA6A
loc_1_CA6A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CA6F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x10
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 8
        push   eax
        call   sub_1_2011
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x48
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x65], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x64], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CAAF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x40], 2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x64], 0xa
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xc3c], 3
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        inc    dword ptr [eax + 0x20]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CADF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x64]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x64], 0x14
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x40], 0x19
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        inc    dword ptr [eax + 0x24]
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x18], eax
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        add    edi, 0x20
        add    esi, 0x84
        mov    ecx, 6
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [eax + 0x20], 2
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x2c], -1
        jne    loc_1_CB47
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x2c], 0
loc_1_CB47:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xc3c], 3
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0xa
        jne    loc_1_CB75
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        dec    dword ptr [eax + 0x20]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CD19
        add    esp, 4
loc_1_CB75:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CB7A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0x14
        jne    loc_1_CBC1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x48
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x65], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x64], 0x1e
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        dec    dword ptr [eax + 0x24]
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        inc    dword ptr [eax + 0x28]
loc_1_CBC1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CBC6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x48
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x65], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0x14
        jne    loc_1_CC12
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x64], 0x28
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        dec    dword ptr [eax + 0x24]
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        inc    dword ptr [eax + 0x28]
        jmp    loc_1_CC2D
loc_1_CC12:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0x1e
        jne    loc_1_CC2D
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x64], 0x28
loc_1_CC2D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CC32:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0x1e
        jne    loc_1_CC6A
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x65], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x64], 0x14
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        inc    dword ptr [eax + 0x24]
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        dec    dword ptr [eax + 0x28]
loc_1_CC6A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CC6F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        call   sub_1_C9EF
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_CC94
        mov    eax, 0x14
        jmp    loc_1_CD14
loc_1_CC94:
        push   2
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x3e]
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 1]
        and    eax, 0xff
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_2124
        add    esp, 0x10
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 2
        je     loc_1_CCEC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CA6F
        add    esp, 4
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_CD14
loc_1_CCEC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CAAF
        add    esp, 4
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_CD14
loc_1_CD14:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CD19:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_DC25
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0xc], eax
loc_1_CD40:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x10
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_1_CDEC
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 0x1c
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx]
        mov    ecx, dword ptr [ecx + 4]
        cmp    dword ptr [eax + 4], ecx
        jne    loc_1_CDAF
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0x10], 0
        jne    loc_1_CD9B
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_CF87
        add    esp, 4
loc_1_CD9B:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C3A8
        add    esp, 4
        inc    dword ptr [ebp - 0x10]
        jmp    loc_1_CDE1
loc_1_CDAF:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0x20], 0
        jne    loc_1_CDE1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        push   0
        push   3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0xc]
        add    esp, 0xc
loc_1_CDE1:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_CD40
loc_1_CDEC:
        cmp    dword ptr [ebp - 0x10], 0
        jne    loc_1_CE02
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF18
        add    esp, 4
loc_1_CE02:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CE07:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 2]
        and    eax, 0x7f
        mov    byte ptr [ebp - 0x10], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        cmp    eax, 0x6f
        jg     loc_1_CE46
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x10]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [ecx + eax*4 + 0x34]
        mov    dword ptr [ebp - 4], eax
loc_1_CE46:
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_CEB7
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x15]
        mov    dword ptr [ebp - 0xc], eax
loc_1_CE59:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x15
        cmp    eax, dword ptr [ebp - 0xc]
        je     loc_1_CEB7
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 0x24
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0xff
        mov    ecx, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx + 9]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_1_CEAA
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], ecx
        jne    loc_1_CEAA
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_CEBE
loc_1_CEAA:
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_CE59
loc_1_CEB7:
        xor    eax, eax
        jmp    loc_1_CEBE
loc_1_CEBE:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CEC3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_CECC:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax], ecx
        je     loc_1_CF13
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0x1c
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_C4CC
        add    esp, 4
        push   0
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 0x21]
        call   dword ptr [eax + 0xc]
        add    esp, 0xc
        jmp    loc_1_CECC
loc_1_CF13:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CF18:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        call   sub_1_387C
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CBC6
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CF3E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        add    eax, 0x48
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    byte ptr [eax + 0x65], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1c
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        add    eax, 8
        push   eax
        call   sub_1_1F70
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        inc    dword ptr [eax + 0x3c]
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CF87:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_CFBF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        add    eax, 0x48
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    byte ptr [eax + 0x65], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        inc    dword ptr [eax + 0x3c]
loc_1_CFBF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_CFC4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x29]
        mov    dword ptr [ebp - 4], eax
loc_1_CFD6:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x29
        cmp    eax, dword ptr [ebp - 4]
        je     loc_1_D016
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0x2c
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_D009
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_D01D
loc_1_D009:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_CFD6
loc_1_D016:
        xor    eax, eax
        jmp    loc_1_D01D
loc_1_D01D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D022:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_D05B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        dec    dword ptr [eax + 0x3c]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0xb]
        and    eax, 0xff
        cmp    eax, 1
        jne    loc_1_D05B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        dec    dword ptr [eax + 0x38]
loc_1_D05B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D060:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x40], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0x14
        jne    loc_1_D09F
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x3c]
        cmp    dword ptr [eax + 0x38], ecx
        jne    loc_1_D09F
        cmp    dword ptr [ebp + 0xc], 0x1b
        je     loc_1_D0C7
loc_1_D09F:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0x1e
        jne    loc_1_D0DA
        cmp    dword ptr [ebp + 0xc], 0x2e
        je     loc_1_D0C7
        cmp    dword ptr [ebp + 0xc], 0x1b
        jne    loc_1_D0DA
loc_1_D0C7:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF18
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_D0E4
loc_1_D0DA:
        mov    eax, 1
        jmp    loc_1_D0E4
loc_1_D0E4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D0E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0x20], 0
        jne    loc_1_D141
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0x28], 0
        jne    loc_1_D141
        call   sub_1_7D98
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_D13A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CF18
        add    esp, 4
        jmp    loc_1_D141
loc_1_D13A:
        mov    dword ptr [ebp - 8], 0
loc_1_D141:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_D149
loc_1_D149:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D14E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x3c], 0
        jne    loc_1_D20C
        mov    al, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0x65], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        mov    al, byte ptr [eax + 0xb4]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_D1B0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        mov    al, byte ptr [eax + 0xb2]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_D1B0
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_1_D1C1
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_D1C1
loc_1_D1B0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF18
        add    esp, 4
        jmp    loc_1_D20C
loc_1_D1C1:
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_1_D1E5
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x44], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CB7A
        add    esp, 4
        jmp    loc_1_D20C
loc_1_D1E5:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_D1FD
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x44], eax
        jmp    loc_1_D20C
loc_1_D1FD:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x48
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_D20C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D211:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [eax + 0x44], ecx
        jne    loc_1_D26D
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x64]
        and    eax, 0xff
        cmp    eax, 0x1e
        jne    loc_1_D24B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CF18
        add    esp, 4
        jmp    loc_1_D26D
loc_1_D24B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0x65]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_D26D
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x48
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_D26D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_D272:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    eax, 0x48
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x65]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_D2A3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_CF18
        add    esp, 4
loc_1_D2A3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D2A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x1f4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xcaa], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xca8], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xca9], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0xcc7], 0
        jne    loc_1_D309
        mov    eax, dword ptr [ebp - 0x10]
        mov    word ptr [eax], 0
loc_1_D309:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_D318
loc_1_D315:
        inc    dword ptr [ebp - 0xc]
loc_1_D318:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0xc]
        jbe    loc_1_D342
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + eax*4 + 0xb9d], 0
        jmp    loc_1_D315
loc_1_D342:
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_1_D351
loc_1_D34E:
        inc    dword ptr [ebp - 0xc]
loc_1_D351:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 0xc]
        jbe    loc_1_D386
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        call   sub_1_D3A1
        add    esp, 8
        jmp    loc_1_D34E
loc_1_D386:
        call   sub_1_D4E0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0x30], 0
        call   sub_1_D8D4
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D3A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_D3C6
loc_1_D3C3:
        inc    dword ptr [ebp - 8]
loc_1_D3C6:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_D402
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 4]
        cmp    dword ptr [eax + ecx + 8], edx
        jne    loc_1_D3FD
        jmp    loc_1_D402
loc_1_D3FD:
        jmp    loc_1_D3C3
loc_1_D402:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_1_D420
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_D42C
loc_1_D420:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_D433
loc_1_D42C:
        mov    dword ptr [ebp - 4], 0
loc_1_D433:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_D44E
        cmp    dword ptr [ebp - 8], 0x14
        jb     loc_1_D44E
        mov    dword ptr [ebp - 8], 0x13
loc_1_D44E:
        cmp    dword ptr [ebp - 8], 0x14
        jae    loc_1_D4CC
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_D47F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 0xc]
        lea    edi, [eax + ecx + 4]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_D47F:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + eax*4 + 0xb9d], 1
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + eax*4 + 0xb9c], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        cmp    eax, dword ptr [ebp - 8]
        jne    loc_1_D4C7
        mov    eax, dword ptr [ebp - 0xc]
        inc    word ptr [eax]
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xca8], 1
loc_1_D4C7:
        jmp    loc_1_D4D3
loc_1_D4CC:
        mov    dword ptr [ebp - 8], 0xffffffff
loc_1_D4D3:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_D4DB
loc_1_D4DB:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D4E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 4], 0xffffffff
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], 0
        jmp    loc_1_D51A
loc_1_D517:
        inc    dword ptr [ebp - 8]
loc_1_D51A:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        jae    loc_1_D67A
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [ecx + eax*4 + 0xb9d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_D5A9
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        inc    byte ptr [ecx + eax*4 + 0xb9c]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [ecx + eax*4 + 0xb9c]
        and    eax, 0xff
        cmp    eax, 2
        jl     loc_1_D5A9
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + eax*4 + 0xb9e], 1
        cmp    dword ptr [ebp - 4], -1
        jne    loc_1_D592
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_1_D592:
        mov    eax, dword ptr [ebp - 0x10]
        dec    word ptr [eax]
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xca9], 1
        jmp    loc_1_D517
loc_1_D5A9:
        cmp    dword ptr [ebp - 4], -1
        je     loc_1_D664
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 4]
        mov    ebx, edx
        lea    edx, [edx + edx*4]
        lea    edx, [ebx + edx*8]
        mov    ebx, dword ptr [ebp - 0x10]
        lea    edi, [edx + ebx + 4]
        lea    esi, [eax + ecx + 4]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [ecx + eax*4 + 0xb9c]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [edx + ecx*4 + 0xb9c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + eax*4 + 0xb9e], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + eax*4 + 0xb9e], 1
        inc    dword ptr [ebp - 4]
        jmp    loc_1_D62C
loc_1_D629:
        inc    dword ptr [ebp - 4]
loc_1_D62C:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        jb     loc_1_D65F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [ecx + eax*4 + 0xb9e]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_D65A
        jmp    loc_1_D65F
loc_1_D65A:
        jmp    loc_1_D629
loc_1_D65F:
        jmp    loc_1_D675
loc_1_D664:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + eax*4 + 0xb9e], 0
loc_1_D675:
        jmp    loc_1_D517
loc_1_D67A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
loc_1_D67F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc3c]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_D6D6
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xca7]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_D6D1
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xcaa], 1
loc_1_D6D1:
        call   sub_1_D6DB
loc_1_D6D6:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D6DB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0xcc7], 0
        jbe    loc_1_D77C
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc3c]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_D77C
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcab
        push   eax
        call   sub_1_15D7
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xccb], 1
        push   8
        push   0
        push   -1
        call   sub_1_D781
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xccb], 0
        cmp    dword ptr [ebp - 4], 0x17
        jne    loc_1_D770
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xca7], 1
        call   sub_1_DBB6
        jmp    loc_1_D77C
loc_1_D770:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xca7], 0
loc_1_D77C:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D781:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc3c]
        and    eax, 0xff
        cmp    eax, 3
        jne    loc_1_D7CD
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_D7C6
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [eax + 0x1d]
        call   dword ptr [eax + 4]
        add    esp, 8
loc_1_D7C6:
        xor    eax, eax
        jmp    loc_1_D880
loc_1_D7CD:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc3c]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_D854
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0xccb], 0
        je     loc_1_D809
        push   1
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x3e]
        add    esp, 4
        jmp    loc_1_D816
loc_1_D809:
        push   2
        mov    eax, dword ptr [0]            ; -> loc_1_DFEC
        call   dword ptr [eax + 0x3e]
        add    esp, 4
loc_1_D816:
        push   0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x1f4
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1624
        add    esp, 0x10
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        je     loc_1_D848
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_D880
loc_1_D848:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xc3c], 1
loc_1_D854:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_D876
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, 0xd
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x18
        push   eax
        call   sub_1_1F70
        add    esp, 8
loc_1_D876:
        mov    eax, 2
        jmp    loc_1_D880
loc_1_D880:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D885:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_D8C0
loc_1_D899:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xca7]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_D8B6
        call   sub_1_D6DB
loc_1_D8B6:
        jmp    loc_1_D8CF
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_1_D8C0:
        cmp    dword ptr [ebp - 4], 0x18
        je     loc_1_D899
        jmp    loc_1_D8CF
loc_1_D8CF:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D8D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xca6], 0
        jmp    loc_1_D903
loc_1_D8FB:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        inc    dword ptr [eax + 0x30]
loc_1_D903:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        cmp    eax, dword ptr [ecx + 0x30]
        jbe    loc_1_D9A4
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x30]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [ecx + eax*4 + 0xb9d]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_D99F
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xccb], 1
        push   1
        push   0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x30]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx + 8]
        push   eax
        call   sub_1_D781
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [eax + 0xccb], 0
        cmp    dword ptr [ebp - 4], 2
        jne    loc_1_D99A
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xca6], 1
loc_1_D99A:
        jmp    loc_1_D9A4
loc_1_D99F:
        jmp    loc_1_D8FB
loc_1_D9A4:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_D9A9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x1f4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0xffff
        test   eax, eax
        jle    loc_1_D9ED
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        inc    dword ptr [eax + 0x30]
        jmp    loc_1_DB0F
loc_1_D9ED:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        test   eax, eax
        je     loc_1_DA23
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x30]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edx + 8]
        cmp    dword ptr [eax + ecx + 8], edx
        je     loc_1_DAE3
loc_1_DA23:
        mov    eax, dword ptr [ebp - 0xc]
        dec    word ptr [eax]
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xca9], 1
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        cmp    eax, dword ptr [ecx + 0x30]
        jbe    loc_1_DADE
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax]
        and    eax, 0xffff
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        sub    eax, dword ptr [ecx + 0x30]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x30]
        inc    eax
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [0]            ; -> loc_1_E098
        mov    ebx, dword ptr [edx + 0x30]
        mov    edx, ebx
        lea    ebx, [ebx + ebx*4]
        lea    ebx, [edx + ebx*8]
        mov    esi, dword ptr [ebp - 0xc]
        lea    edx, [eax + ecx + 4]
        lea    ecx, [ebx + esi + 4]
        call   sub_1_0FF9
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x30]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    edx, dword ptr [0]            ; -> loc_1_E098
        mov    ebx, dword ptr [edx + 0x30]
        mov    esi, dword ptr [0]            ; -> loc_1_E098
        lea    edx, [ecx + eax*4 + 0xba0]
        lea    ecx, [esi + ebx*4 + 0xb9c]
        call   sub_1_0FF9
loc_1_DADE:
        jmp    loc_1_DB0F
loc_1_DAE3:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x30]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + eax*4 + 0xb9d], 1
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0x30]
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [ecx + eax*4 + 0xb9c], 0
loc_1_DB0F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_DB14:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    byte ptr [eax + 0xc3c], 0
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xca6]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_DB50
        call   sub_1_D9A9
        call   sub_1_D8D4
        jmp    loc_1_DB55
loc_1_DB50:
        call   sub_1_D2A8
loc_1_DB55:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc3c]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_DBB1
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xca8]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_DB9D
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xca9]
        and    eax, 0xff
        test   eax, eax
        je     loc_1_DBA2
loc_1_DB9D:
        call   sub_1_DC87
loc_1_DBA2:
        call   sub_1_DCF1
        call   sub_1_7C61
        call   sub_1_DBB6
loc_1_DBB1:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_DBB6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        cmp    dword ptr [eax + 0xcc7], 0
        jbe    loc_1_DC20
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    al, byte ptr [eax + 0xc3c]
        and    eax, 0xff
        test   eax, eax
        jne    loc_1_DC20
        call   sub_1_10B1
        mov    ecx, 0x37
        sub    edx, edx
        div    ecx
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax + 0xcc7]
        add    eax, edx
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        mov    dword ptr [ecx + 0xcab], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0xcab
        push   eax
        call   sub_1_1526
        add    esp, 4
loc_1_DC20:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_DC25:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        mov    dword ptr [ebp - 4], eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x1c]
        push   eax
        call   sub_1_D3A1
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        add    eax, dword ptr [ebp - 4]
        add    eax, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [eax + ecx + 6], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2], 0
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_DC87:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], eax
loc_1_DC9A:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [0], eax            ; -> loc_1_E098
        je     loc_1_DCEC
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 5
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x1d]
        cmp    dword ptr [eax + 0x18], 0
        je     loc_1_DCE1
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x1d]
        call   dword ptr [eax + 0x18]
        add    esp, 4
loc_1_DCE1:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_DC9A
loc_1_DCEC:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_DCF1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
loc_1_DCFA:
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        mov    ecx, dword ptr [0]            ; -> loc_1_E098
        add    ecx, 0x18
        cmp    dword ptr [eax + 0x18], ecx
        je     loc_1_DD6F
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x18
        push   eax
        call   sub_1_1FA3
        add    esp, 4
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0xc]
        sub    eax, 0xd
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0]            ; -> loc_1_E098
        add    eax, 0x864
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x1d]
        call   dword ptr [eax + 4]
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_DCFA
loc_1_DD6F:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_DD74:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_1_DD7C:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_1_DD84:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_1_DDA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_1_DDB0:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf800100
        add    byte ptr [eax], al
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'IRLAMPEX'                  ; DDB_Name
        dd  030000001h                  ; DDB_Init_Order
        dd  IRLAMPEX_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_DE10:
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_DE14:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        db  000h
loc_1_DE24:
        add    dword ptr [edx - 0x797a7b7d], eax
        or     byte ptr [eax], al
loc_1_DE2C:
        add    eax, dword ptr [eax]
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        add    byte ptr [eax], 1
        db  000h
loc_1_DE38:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
loc_1_DE3C:
        pushal 
        or     dword ptr [eax], eax
        add    byte ptr [eax + 0x25], al
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [esi - 0x1f000000], dl
        add    byte ptr [eax], al
        add    dl, al
        add    dword ptr [eax], eax
        add    dl, cl
        or     byte ptr [eax], al
        add    byte ptr [ecx + edx + 0x3d090000], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    edx, edi
        add    byte ptr [eax + eax], ah
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [0x100], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dl
        or     al, byte ptr [eax]
        add    al, byte ptr [eax]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        adc    al, 0
        add    al, 0
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, byte ptr [eax]
        sbb    eax, 0x3000600
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], dh
        cmp    al, byte ptr [eax]
        or     al, 0
        push   es
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, dl
        add    ch, byte ptr [eax + 1]
        dec    eax
        add    byte ptr [eax + eax], ah
        pop    es
        add    byte ptr [eax + eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        mov    al, byte ptr [0x9002d005]
        add    byte ptr [eax], cl
        push   cs
        add    byte ptr [edi], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax - 0xbf63bed], cl
        add    edx, edi
        add    byte ptr [eax + eax], bh
        sbb    dword ptr [eax], eax
        add    eax, 0x64000000
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dl
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        nop    
        add    dword ptr [eax], eax
        add    al, cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    eax, dword ptr [eax]
        add    byte ptr [eax - 0x5fffffff], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x40000000], ah
        push   es
        add    byte ptr [eax], al
        and    byte ptr [ebx], al
        add    byte ptr [eax], al
        inc    eax
        add    dword ptr [eax], eax
        add    byte ptr [eax + 1], al
        add    byte ptr [eax], al
        cmp    byte ptr [ecx], cl
        add    byte ptr [eax], al
        pushfd 
        add    al, 0
        add    al, bl
        add    dword ptr [eax], eax
        add    al, bl
        add    dword ptr [eax], eax
        add    al, al
        adc    al, byte ptr [eax]
        add    byte ptr [eax + 9], ah
        add    byte ptr [eax], al
        rol    byte ptr [ebx], 0
        add    al, ah
        add    dword ptr [eax], eax
        add    byte ptr [eax + 0x70], al
        sub    eax, 0x16800000
        add    byte ptr [eax], al
        inc    eax
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        loope  loc_1_DF6F
loc_1_DF6F:
        add    byte ptr [eax + 0x70], al
        sub    eax, 0x16800000
        add    byte ptr [eax], al
        inc    eax
        or     eax, 0x86a00003
        add    dword ptr [eax], eax
        inc    eax
        pushfd 
        add    byte ptr [eax], al
        and    byte ptr [esi], cl
        add    byte ptr [eax], dh
        sbb    byte ptr [esi + eax], cl
        add    eax, dword ptr [edx]
        add    dword ptr [eax], eax
loc_1_DF94:
        xor    byte ptr [esi + eax], cl
        add    eax, dword ptr [edx]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    al, 0
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x4000000b], bh
        pop    ds
        add    byte ptr [eax], al
        loopne 0xdfe4
        add    byte ptr [eax], al
        cmp    byte ptr [esi], 0
        add    byte ptr [eax], ah
        dec    esi
        add    byte ptr [eax], al
        test   al, 0x61
        add    byte ptr [eax], al
        xor    byte ptr [ebp], dh
        add    byte ptr [eax - 0x64], al
        add    byte ptr [eax], al
loc_1_DFCC:
        cmp    al, 0
        add    byte ptr [eax], al
        cmp    al, 0
        add    byte ptr [eax], al
        push   ds
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, cl
        add    dword ptr [eax], eax
        add    ah, bl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
loc_1_DFE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_DFEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_DFF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_DFF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_DFF8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E000:
        mov    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ; ---- dispatch table (3 entries) ----
        dd  loc_1_624C
        dd  loc_1_6273
        dd  loc_1_6292
loc_1_E094:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_E098:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E09C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E0A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E0A8:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E0C8:
        sbb    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E0E4:
        dec    ecx
        jb     0xe133
        dec    ebp
        push   eax
        and    byte ptr [ecx + 0x41], cl
        push   ebx
        and    byte ptr [ebx + 0x6c], al
        imul   esp, dword ptr [ebp + 0x6e], 0x74
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_E100:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E120:
        sbb    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E13C:
        add    al, 0
        add    byte ptr [eax], al
loc_1_E140:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_E144:
        inc    esp
        jbe    0xe1b1
        arpl   word ptr [ebp], sp
        add    byte ptr [ecx + 0x72], cl
        dec    esp
        dec    ebp
        push   eax
        push   ebx
        jne    0xe1c4
        jo     0xe1c5
        jb     0xe1cc
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_E15C:
        inc    esp
        jbe    0xe1c9
        arpl   word ptr [ebp + 0x4e], sp
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
loc_1_E168:
        dec    ecx
        jb     0xe1b7
        dec    ebp
        push   eax
        and    byte ptr [ecx + 0x41], cl
        push   ebx
        and    byte ptr [ebx + 0x65], dl
        jb     0xe1ec
        jb     loc_1_E179
loc_1_E179:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x38  R-X
loc_2_0000:
        VxDCall  Dev_04A2.Service_0000h          ; cd200000a204
        jb     loc_2_000F
        call   sub_2_0014
        clc    
        ret    
loc_2_000F:
        stc    
        ret    
        db  000h, 000h, 000h
sub_2_0014:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0                             ; -> loc_1_E000
        call   sub_1_0019
loc_2_0024:
        add    esp, 4
        mov    eax, 1
        jmp    loc_2_0031
loc_2_0031:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x2e  R-X
sub_3_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VTD.VTD_End_Min_Int_Period      ; cd2004000500
        cmc    
        sbb    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_3_0017:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VTD.VTD_Begin_Min_Int_Period    ; cd2003000500
        cmc    
        sbb    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
VxD__LTEXT ENDS
